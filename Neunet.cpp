
#include "stdafx.h"
#include "3dfstbrk.h"
#include "math.h"
#include "neunet.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 构造函数, 析构函数.															 开始
NeuNet::NeuNet()
{
  m_FileNeuNet = "neunet.txt";

  m_DataNumber = 0;
  m_DataGroup = NULL;
  m_Abs = NULL;
  m_Square = NULL;
  m_Quadra = NULL;
  m_ErrorOfOutput = NULL;
  m_PeakRoom = NULL;
  m_PeakWrapAngle = NULL;


  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 与神经网络有关的成员变量.
  m_NodeNumberOfInput = 3;   //////5;
  m_NodeNumberOfHide = 16;		 // 节点个数可以改动, 但必须随着修改程序.
  m_NodeNumberOfOutput = 1;

  long n1 = m_NodeNumberOfInput*m_NodeNumberOfHide;
  long n2 = m_NodeNumberOfHide*m_NodeNumberOfOutput;

  m_pWeightToHideNode = new double[n1];
  m_pWeightToOutputNode = new double[n2];
  m_pWeightToHideNodeLast = new double[n1];
  m_pWeightToOutputNodeLast = new double[n2];

  m_pThresholdOfHideNode = new double[m_NodeNumberOfHide];
  m_pThresholdOfOutputNode = new double[m_NodeNumberOfOutput];

  m_pValueOfInputNode = new double[m_NodeNumberOfInput];
  m_pValueOfHideNode = new double[m_NodeNumberOfHide];
  m_pValueOfOutputNode = new double[m_NodeNumberOfOutput];

  m_pExpectedOutputValue = new double[m_NodeNumberOfOutput];

  if ( !ReadWeight() )RandomlyGiveWeight();
}

NeuNet::Set(float *DataGroup, long DataNumber, int ChosenPoint, bool bFirstBreak)
{

  if ( m_DataNumber == 0 )
  {
    m_Abs = new double[DataNumber];
    m_Square = new double[DataNumber];
    m_Quadra = new double[DataNumber];
    m_ErrorOfOutput = new double[DataNumber];
    m_PeakRoom = new int[DataNumber];
    m_PeakWrapAngle = new double[DataNumber];
  }
  else if ( DataNumber != m_DataNumber )
  {  // If changed shot that has a diffrent data point number:
    delete m_Abs;
    delete m_Square;
    delete m_Quadra;
    delete m_ErrorOfOutput;
    delete m_PeakRoom;
    delete m_PeakWrapAngle;
    m_Abs = new double[m_DataNumber];
    m_Square = new double[m_DataNumber];
    m_Quadra = new double[m_DataNumber];
    m_ErrorOfOutput = new double[m_DataNumber];
    m_PeakRoom = new int[m_DataNumber];
    m_PeakWrapAngle = new double[m_DataNumber];
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 与数据组有关的成员变量.
  m_DataGroup = DataGroup;
  m_DataNumber = DataNumber;
  m_PeakNumber = 0;
  m_ChosenPoint = ChosenPoint;

  double average = 0;
  for ( int i = 0; i < m_DataNumber; i++ )
  {
    m_Abs[i] = fabs(m_DataGroup[i]);
    average += m_Abs[i];
  }
  average /= (m_DataNumber * 5);

  for ( i = 0; i < m_DataNumber; i++ )
  {
    if ( m_Abs[i] != 0 )
    {
      break;
    }
  }
  long n = i;
  for ( i = 0; i < n; i++ )
  {
    m_DataGroup[i] = m_DataGroup[n];
    m_Abs[i] = m_Abs[n];
  }



  for ( i = 0; i < m_DataNumber - 1; i++ )
  {
    //	if(m_Abs[i]==0&&m_Abs[i+1]==0){ //<average){
    //		m_DataGroup[i]=average;
    ///		m_Abs[i]=average;
    //		m_Square[i]=average;
    //	}
    //	else{
    m_Square[i] = m_DataGroup[i] * m_DataGroup[i];
    m_Quadra[i] = m_Square[i] * m_Square[i];
    //	}
  }

  m_LearnErrorLimit = 0.00001;
  m_MinimumNoiseDataNumber = 5; //用于在think时省略的首尾的点数.

  // 给期望输出赋值.
  int nOutput = 1;
  if ( !bFirstBreak )nOutput = 0;
  for ( i = 0; i < m_NodeNumberOfOutput; i++ )
  {
    m_pExpectedOutputValue[i] = nOutput;
  }

  // 计算峰值点.
  //CalculatePeaks();

  return TRUE;
}
NeuNet::~NeuNet()
{
  if ( m_DataNumber > 0 )
  {
    delete m_Abs;
    delete m_Square;
    delete m_Quadra;
    delete m_ErrorOfOutput;
    delete m_PeakRoom;
    delete m_pWeightToHideNode;
    delete m_pWeightToOutputNode;
    delete m_pWeightToHideNodeLast;
    delete m_pWeightToOutputNodeLast;
    delete m_pThresholdOfHideNode;
    delete m_pThresholdOfOutputNode;
    delete m_pValueOfInputNode;
    delete m_pValueOfHideNode;
    delete m_pValueOfOutputNode;
    delete m_pExpectedOutputValue;
  }
}
//  类NeuNet的构造函数,析构函数.										 结束
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//	名称:		Think									BEGIN		
//  功能:	两个功能:
//				1.若用户不给出GivenAnswer, 则对整个数组计算输出值,
//						选出与期望输出值之差最小的点,作为返回值.
//              2.若用户给出GivenAnswer, 则用户需要的是输出节点的
//						输出值, 返回值不用.
//  输入:  GivenAnswer, 目标输出, 是数组中的第GivenAnswer个数.
//  日期: 1997.6
//  重编日期:1997.10.5
long NeuNet::Think()
{
  //FILE *fpTXT=fopen("1.txt","wt");

  // 临时变量.
  int i, j, k, n;
  float Sum, Sum1, Sum2, temp;

  // 针对某一点开一个时窗, half 为该时窗的一半.
  int Half = 15;

  // 输入节点的输入值.
  float Arms, Quadra, Rns, KF, KB, RatioToAverage;

  int CurrentPeak;

  // 根据GivenAnswer的值确定所开的时窗.
  long  BeginCalculation, EndCalculation;// The time window.
  if ( m_ChosenPoint == 0 )
  {  // user want to calculate the first break.
    BeginCalculation = m_MinimumNoiseDataNumber + 1;
    EndCalculation = m_DataNumber - m_MinimumNoiseDataNumber;
  }
  else
  {							// user want the neural net think.
    BeginCalculation = m_ChosenPoint;
    EndCalculation = m_ChosenPoint;
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //   对该道所有数据 , 计算网络的输出值, 和与期望输出的差.
  int TimeWindowLeft, TimeWindowRight;
  int LeftHalfLength, RightHalfLength;
  for ( i = BeginCalculation; i <= EndCalculation; i++ )
  {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 预置输出误差. 因为在此循环中有 continue 语句.
    m_ErrorOfOutput[i] = 100000;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //确定该点的时窗
    TimeWindowLeft = i - Half;
    TimeWindowRight = i + Half;
    if ( TimeWindowLeft < 0 )TimeWindowLeft = 0;
    if ( TimeWindowRight > m_DataNumber )TimeWindowRight = m_DataNumber;

    LeftHalfLength = i - TimeWindowLeft;
    RightHalfLength = TimeWindowRight - i;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //计算输入节点的输入值.
    //  1. 计算该点前后振幅平方均值的比值.
    Sum1 = 0.0, Sum2 = 0.0;

    for ( j = TimeWindowLeft; j < i; j++ )Sum1 += m_Square[j];
    Sum1 /= LeftHalfLength;  // 前边振幅平方的平均值.

    for ( j = i; j < TimeWindowRight; j++ )Sum2 += m_Square[j];
    Sum2 /= RightHalfLength;  // 后边振幅平方的平均值.

    Arms = (float)sqrt(Sum1 / Sum2);  // 两者相比.
    m_pValueOfInputNode[0] = Arms;  // 赋给第一个节点.

    // 2. 计算前后振幅比.
    Sum1 = 0.0; Sum2 = 0.0;

    for ( j = TimeWindowLeft; j < i; j++ )Sum1 += m_Abs[j];
    Sum1 /= LeftHalfLength;

    for ( j = i; j < TimeWindowRight; j++ )Sum2 += m_Abs[j];
    Sum2 /= RightHalfLength;

    Rns = Sum1 / Sum2;
    m_pValueOfInputNode[1] = Rns;


    // 3. 计算振幅四次方的比值
    Sum1 = 0.0, Sum2 = 0.0;

    for ( j = TimeWindowLeft; j < i; j++ )Sum1 += m_Quadra[j];
    Sum1 /= LeftHalfLength;  // 前边振幅平方的平均值.

    for ( j = i; j < TimeWindowRight; j++ )Sum2 += m_Quadra[j];
    Sum2 /= RightHalfLength;  // 后边振幅平方的平均值.

    Quadra = (float)sqrt(Sum1 / Sum2);  // 两者相比.
    m_pValueOfInputNode[2] = Quadra;  // 赋给第一个节点.



    /*
    // 3. 计算该点的振幅与后边振幅平均值的比值.
    Sum=0;
    for(j=i;j<TimeWindowRight;j++)Sum+=m_Abs[j];
    Sum/=RightHalfLength;
    RatioToAverage=m_Abs[i]/Sum;
    m_pValueOfInputNode[2]=RatioToAverage;

    // 4. & 5.  计算前后振幅包络的斜率.
    // KF & KB are let small than 1,to let the neural net united.
    CurrentPeak=SearchNearestPeak(i);
    if(CurrentPeak<1)continue;								  // 无法用的峰值点.
    if(CurrentPeak>(m_PeakNumber-2))continue;   // 倒数第二, 无法用峰值点.

    KF=m_PeakWrapAngle[CurrentPeak];
    KB=m_PeakWrapAngle[CurrentPeak+1];

    m_pValueOfInputNode[3]=KF;
    m_pValueOfInputNode[4]=KB;
       */

       ////////////////////////////////////////////////////////////////////////////////////////////////////////////
       //  计算隐藏节点的输出值.
    for ( j = 0; j < m_NodeNumberOfHide; j++ )
    {
      Sum = 0.0;
      for ( k = 0; k < m_NodeNumberOfInput; k++ )
      {
        n = j*m_NodeNumberOfInput + k;
        Sum += m_pWeightToHideNode[n] * m_pValueOfInputNode[k];
      }
      temp = Sum - m_pThresholdOfHideNode[j];
      m_pValueOfHideNode[j] = 1 / (1 + exp(-temp));
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //计算输出节点的输出值.
    m_ErrorOfOutput[i] = 0;
    for ( j = 0; j < m_NodeNumberOfOutput; j++ )
    {

      // 该节点的输入值.
      Sum = 0.0;
      for ( k = 0; k < m_NodeNumberOfHide; k++ )
      {
        n = j*m_NodeNumberOfOutput + k;
        Sum += m_pWeightToOutputNode[n] * m_pValueOfHideNode[k];
      }

      // 该节点的输出值.
      temp = Sum - m_pThresholdOfOutputNode[j];
      m_pValueOfOutputNode[j] = 1 / (1 + exp(-temp));

      // 输出值与期望输出的差值.
      //m_ErrorOfOutput[i]+=fabs(m_pValueOfOutputNode[j]-m_pExpectedOutputValue[j]);
      m_ErrorOfOutput[i] += fabs(m_pValueOfOutputNode[j]);

    }// 循环计算节点的输出值.

    //fprintf(fpTXT,"%10d,    %10.9f,  %10.9f \n",i,m_pValueOfOutputNode[0],m_DataGroup[i]);
    //TRACE("%10d,    %10.9f,  %10.9f \n",i,m_pValueOfOutputNode[0],m_DataGroup[i]);
    //for(j=0;j<2;j++){
    //	TRACE("%10.1f    ,",m_pValueOfInputNode[j]);
    //}
    //TRACE("\n");

  }// 循环计算该道的所有点.

  ////////////////////////////////////////////////////////////////////////////////////////////////
  //   找出与期望输出误差最小的点,作为答案.
  double MinError = 100000000;
  long MinErrorPoint = 0;
  for ( i = BeginCalculation; i <= EndCalculation; i++ )
  {
    if ( m_ErrorOfOutput[i] < MinError )
    {
      MinError = m_ErrorOfOutput[i];
      MinErrorPoint = i;
    }
  }

  //fprintf(fpTXT,"%i",MinErrorPoint);
  //TRACE("%i\n",MinErrorPoint);
  //fclose(fpTXT);

  ////////////////////////////////////////////////////////////////////////////////////////////////
  // 返回与期望输出之差最小的点号.
  return MinErrorPoint;
}
//			              	float FstBrk::NeuNetThink									    END
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//							BOOL FstBrk::CalculatePeaks								BEGIN
//  功能:计算波峰.
//  日期:1997.10.5
void NeuNet::CalculatePeaks()
{
  float a, b, c;
  m_PeakNumber = 0;

  // 不能用到第一个点, 因为要用到第i-1个点.
  // 不能用到最后一个点, 因为要用到第i+1个点.
  for ( long i = 1; i < m_DataNumber - 1; i++ )
  {
    a = m_DataGroup[i - 1];
    b = m_DataGroup[i];
    c = m_DataGroup[i + 1];

    // 只有下调点被认为可能是波峰.
    if ( (a > b) && (c > b) && (b < 0) )
    {
      m_PeakRoom[m_PeakNumber] = i;
      m_PeakNumber++;
      i++;         // Next position can NOT be a peak, so i++;
    }
  }

  double d, h, l;
  for ( i = 1; i < m_PeakNumber; i++ )
  {
    d = (m_PeakRoom[i] - m_PeakRoom[i - 1]);
    h = m_DataGroup[m_PeakRoom[i]] - m_DataGroup[m_PeakRoom[i - 1]];
    m_PeakWrapAngle[i] = fabs(h / d);   // 波峰点前边(小号)方向包络的正弦.
  }
}
//							BOOL FstBrk::CalculatePeaks								END
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                          BOOL FstBrk::SearchNearest									BEGIN
//	功能:找出与输入点最近的波峰.
//  重编日期:1997.10.5
long NeuNet::SearchNearestPeak(int SearchedNumber)
{
  int a, b;
  for ( long i = 1; i < m_PeakNumber; i++ )
  {   // 必须从1开始, 因为用到 i-1个波峰.
    if ( m_PeakRoom[i - 1]<SearchedNumber&&m_PeakRoom[i]>SearchedNumber )
    {
      a = abs(m_PeakRoom[i - 1] - SearchedNumber);
      b = abs(m_PeakRoom[i] - SearchedNumber);
      return (a > b) ? i : i - 1;
    }
  }

  return -1;
}
//                          BOOL FstBrk::SearchNearestPeak									END
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									BOOL FstBrk::Learn()								BEGIN
//  功能: 网络学习, 改变权值.
//  重编日期: 1997.10.5
BOOL NeuNet::Learn()
{
  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Prepare data :	
  // Temporary variables.
  int i, j, k;
  double a;
  int m, n;

  double OutputError;
  double ErrorSum;

  // Key space for calculation of Hide layer PE/PW
  double *DeltaOfOutput = new double[m_NodeNumberOfOutput];
  double DeltaOfHide;

  // Key constants:
  double AlphaZero = 0.2, Alpha;
  double YingtaZero = 0.3, Yingta;
  double BetaZero = 0.3;

  // Key variables.
  double DeltaE;  //Error of Even Root of Output Node.
  double DeltaW;  // Changed  quantity of Weight.		

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //  计算一次权值变化.
  //------------------------------------------------------------------------------------------------------------
  UINT LearnTime = 0;
  do
  {

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 利用当前权值计算一次输出值.
    Think();

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  计算 DeltaE: 输出的平方和的开方,用于调整Yingta和Alpha.
    DeltaE = 0.0;
    for ( j = 0; j < m_NodeNumberOfOutput; j++ )
    {
      a = m_pValueOfOutputNode[j] - m_pExpectedOutputValue[j];
      DeltaE += a*a;
    }
    DeltaE = sqrt(DeltaE);
    Yingta = YingtaZero + BetaZero*DeltaE;
    Alpha = AlphaZero + BetaZero*Yingta / YingtaZero;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 计算到输出节点的权值的变化.
    for ( j = 0; j < m_NodeNumberOfOutput; j++ )
    {
      OutputError = m_pExpectedOutputValue[j] - m_pValueOfOutputNode[j];
      DeltaOfOutput[j] = m_pValueOfOutputNode[j] * (1 - m_pValueOfOutputNode[j])*
        OutputError;  //   (dk-yk)fk'(Ako)  ===> f(x)(1-f(x))(dk-yk) , without ij.

      for ( k = 0; k < m_NodeNumberOfHide; k++ )
      {
        n = j*m_NodeNumberOfHide + k;
        DeltaW = m_pWeightToOutputNode[n] - m_pWeightToOutputNodeLast[n];
        m_pWeightToOutputNodeLast[n] = m_pWeightToOutputNode[n];
        m_pWeightToOutputNode[n] += Yingta*DeltaOfOutput[j] * m_pValueOfHideNode[k]
          + Alpha*DeltaW;  //   ij is here :m_pValueOfHideNode.
      }
    }// Calculate Changed Weight of Output Nodes.

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 计算到隐藏节点的权值变化.
    for ( j = 0; j < m_NodeNumberOfHide; j++ )
    {
      ErrorSum = 0;
      for ( k = 0; k < m_NodeNumberOfOutput; k++ )
      {
        n = k*m_NodeNumberOfOutput + j;
        ErrorSum += DeltaOfOutput[k] * m_pWeightToOutputNodeLast[n];
      }
      DeltaOfHide = ErrorSum*m_pValueOfHideNode[j] * (1 - m_pValueOfHideNode[j]);

      m = j*m_NodeNumberOfInput;
      for ( k = 0; k < m_NodeNumberOfInput; k++ )
      {
        n = m + k;
        DeltaW = m_pWeightToHideNode[n] - m_pWeightToHideNodeLast[n];
        m_pWeightToHideNodeLast[n] = m_pWeightToHideNode[n];
        m_pWeightToHideNode[n] +=
          Yingta*DeltaOfHide*m_pValueOfInputNode[k]
          + Alpha*DeltaW;
      }
    }  // Calculate Weight to Hide Nodes.
    LearnTime++;
    if ( LearnTime > 1000 )break;
  } while ( !CheckNeuNetLearned() );

  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 存储权值.
  SaveWeight();

  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  // 删除临时变量。
  delete DeltaOfOutput;

  // 返回.
  return TRUE;

}
//       BOOL FstBrk::NeuNetLearn()															END
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//         BOOL FstBrk::CheckNeuNetLearned()								BEGIN
BOOL NeuNet::CheckNeuNetLearned()
{
  int i;
  double ErrorSum = 0;

  // Two weight number.
  int NumberOfWeightToHide = m_NodeNumberOfInput*m_NodeNumberOfHide;
  int NumberOfWeightToOutput = m_NodeNumberOfHide*m_NodeNumberOfOutput;

  // check the weight to hide node .
  for ( i = 0; i < NumberOfWeightToHide; i++ )
  {
    ErrorSum += fabs(m_pWeightToHideNodeLast[i] - m_pWeightToHideNode[i]);
  }

  // check the weight to output node .
  for ( i = 0; i < NumberOfWeightToOutput; i++ )
  {
    ErrorSum += fabs(m_pWeightToOutputNodeLast[i] - m_pWeightToOutputNode[i]);
  }

  if ( ErrorSum > m_LearnErrorLimit )return FALSE;
  return TRUE;
}
//         BOOL FstBrk::CheckNeuNetLearned()								 END
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//				void FstBrk::RandomlyGiveWeight()							BEGIN
void NeuNet::RandomlyGiveWeight()
{

  int i;

  // Give value to thresholds,	 Give a number.Is it  OK?.
  for ( i = 0; i < m_NodeNumberOfHide; i++ )
    m_pThresholdOfHideNode[i] = 0.3;
  for ( i = 0; i < m_NodeNumberOfOutput; i++ )
    m_pThresholdOfOutputNode[i] = m_NodeNumberOfHide / 2.0;

  // Give value to weight of links of nodes.
  srand((unsigned)time(NULL));
  double x, y = RAND_MAX;

  for ( i = 0; i < m_NodeNumberOfInput*m_NodeNumberOfHide; i++ )
  {
    x = rand();
    m_pWeightToHideNode[i] = x / y;
    m_pWeightToHideNodeLast[i] = m_pWeightToHideNode[i];
  }

  for ( i = 0; i < m_NodeNumberOfHide*m_NodeNumberOfOutput; i++ )
  {
    x = rand();
    m_pWeightToOutputNode[i] = x / y;
    m_pWeightToOutputNodeLast[i] = m_pWeightToOutputNode[i];
  }
}
//				void FstBrk::RandomlyGiveWeight()							 END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		BOOL FstBrk::ReadWeight()												BEGIN
BOOL NeuNet::ReadWeight()
{
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Open Neu Net File.
  FILE *fp = fopen(m_FileNeuNet, "r");
  if ( !fp )return FALSE;

  // Read value for threshold.
  int i;
  for ( i = 0; i < m_NodeNumberOfHide; i++ )
    fscanf(fp, "%lf\n", &m_pThresholdOfHideNode[i]);
  for ( i = 0; i < m_NodeNumberOfOutput; i++ )
    fscanf(fp, "%lf\n", &m_pThresholdOfOutputNode[i]);

  // Read current value for weight.
  for ( i = 0; i < m_NodeNumberOfInput*m_NodeNumberOfHide; i++ )
    fscanf(fp, "%lf\n", &m_pWeightToHideNode[i]);
  for ( i = 0; i < m_NodeNumberOfHide*m_NodeNumberOfOutput; i++ )
    fscanf(fp, "%lf\n", &m_pWeightToOutputNode[i]);

  // Read last value for weight.
  for ( i = 0; i < m_NodeNumberOfInput*m_NodeNumberOfHide; i++ )
    fscanf(fp, "%lf\n", &m_pWeightToHideNodeLast[i]);
  for ( i = 0; i < m_NodeNumberOfHide*m_NodeNumberOfOutput; i++ )
    fscanf(fp, "%lf\n", &m_pWeightToOutputNodeLast[i]);

  // close file.
  fclose(fp);

  return TRUE;
}
//		BOOL FstBrk::ReadWeight()												 END
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									BOOL FstBrk::SaveWeight								BEGIN
BOOL NeuNet::SaveWeight()
{

  // open the file.
  FILE *fp = fopen(m_FileNeuNet, "w");
  if ( !fp )
  {
    AfxMessageBox("无法建立神经网络记忆库文件!");
    return FALSE;
  }

  //	write threshold of nodes into file.
  for ( int i = 0; i < m_NodeNumberOfHide; i++ )
  {
    fprintf(fp, "%10.8f\n", m_pThresholdOfHideNode[i]);
  }
  for ( i = 0; i < m_NodeNumberOfOutput; i++ )
  {
    fprintf(fp, "%10.8f\n", m_pThresholdOfOutputNode[i]);
  }

  // write current value for weight.
  for ( i = 0; i < m_NodeNumberOfHide*m_NodeNumberOfInput; i++ )
  {
    fprintf(fp, "%10.8f\n", m_pWeightToHideNode[i]);
  }
  for ( i = 0; i < m_NodeNumberOfOutput*m_NodeNumberOfHide; i++ )
  {
    fprintf(fp, "%10.8f\n", m_pWeightToOutputNode[i]);
  }

  // write last value for weight.
  for ( i = 0; i < m_NodeNumberOfHide*m_NodeNumberOfInput; i++ )
  {
    fprintf(fp, "%10.8f\n  ", m_pWeightToHideNodeLast[i]);
  }

  for ( i = 0; i < m_NodeNumberOfOutput*m_NodeNumberOfHide; i++ )
  {
    fprintf(fp, "%10.8f\n  ", m_pWeightToOutputNodeLast[i]);
  }

  // close the file.
  fclose(fp);
  return TRUE;

}
//									BOOL FstBrk::SaveWeight								      END
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

