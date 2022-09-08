
#include "stdafx.h"
#include "3dfstbrk.h"
#include "math.h"
#include "neunet.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ���캯��, ��������.															 ��ʼ
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
  // ���������йصĳ�Ա����.
  m_NodeNumberOfInput = 3;   //////5;
  m_NodeNumberOfHide = 16;		 // �ڵ�������ԸĶ�, �����������޸ĳ���.
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
  // ���������йصĳ�Ա����.
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
  m_MinimumNoiseDataNumber = 5; //������thinkʱʡ�Ե���β�ĵ���.

  // �����������ֵ.
  int nOutput = 1;
  if ( !bFirstBreak )nOutput = 0;
  for ( i = 0; i < m_NodeNumberOfOutput; i++ )
  {
    m_pExpectedOutputValue[i] = nOutput;
  }

  // �����ֵ��.
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
//  ��NeuNet�Ĺ��캯��,��������.										 ����
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//	����:		Think									BEGIN		
//  ����:	��������:
//				1.���û�������GivenAnswer, �����������������ֵ,
//						ѡ�����������ֵ֮����С�ĵ�,��Ϊ����ֵ.
//              2.���û�����GivenAnswer, ���û���Ҫ��������ڵ��
//						���ֵ, ����ֵ����.
//  ����:  GivenAnswer, Ŀ�����, �������еĵ�GivenAnswer����.
//  ����: 1997.6
//  �ر�����:1997.10.5
long NeuNet::Think()
{
  //FILE *fpTXT=fopen("1.txt","wt");

  // ��ʱ����.
  int i, j, k, n;
  float Sum, Sum1, Sum2, temp;

  // ���ĳһ�㿪һ��ʱ��, half Ϊ��ʱ����һ��.
  int Half = 15;

  // ����ڵ������ֵ.
  float Arms, Quadra, Rns, KF, KB, RatioToAverage;

  int CurrentPeak;

  // ����GivenAnswer��ֵȷ��������ʱ��.
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
  //   �Ըõ��������� , ������������ֵ, ������������Ĳ�.
  int TimeWindowLeft, TimeWindowRight;
  int LeftHalfLength, RightHalfLength;
  for ( i = BeginCalculation; i <= EndCalculation; i++ )
  {

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Ԥ��������. ��Ϊ�ڴ�ѭ������ continue ���.
    m_ErrorOfOutput[i] = 100000;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //ȷ���õ��ʱ��
    TimeWindowLeft = i - Half;
    TimeWindowRight = i + Half;
    if ( TimeWindowLeft < 0 )TimeWindowLeft = 0;
    if ( TimeWindowRight > m_DataNumber )TimeWindowRight = m_DataNumber;

    LeftHalfLength = i - TimeWindowLeft;
    RightHalfLength = TimeWindowRight - i;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //��������ڵ������ֵ.
    //  1. ����õ�ǰ�����ƽ����ֵ�ı�ֵ.
    Sum1 = 0.0, Sum2 = 0.0;

    for ( j = TimeWindowLeft; j < i; j++ )Sum1 += m_Square[j];
    Sum1 /= LeftHalfLength;  // ǰ�����ƽ����ƽ��ֵ.

    for ( j = i; j < TimeWindowRight; j++ )Sum2 += m_Square[j];
    Sum2 /= RightHalfLength;  // ������ƽ����ƽ��ֵ.

    Arms = (float)sqrt(Sum1 / Sum2);  // �������.
    m_pValueOfInputNode[0] = Arms;  // ������һ���ڵ�.

    // 2. ����ǰ�������.
    Sum1 = 0.0; Sum2 = 0.0;

    for ( j = TimeWindowLeft; j < i; j++ )Sum1 += m_Abs[j];
    Sum1 /= LeftHalfLength;

    for ( j = i; j < TimeWindowRight; j++ )Sum2 += m_Abs[j];
    Sum2 /= RightHalfLength;

    Rns = Sum1 / Sum2;
    m_pValueOfInputNode[1] = Rns;


    // 3. ��������Ĵη��ı�ֵ
    Sum1 = 0.0, Sum2 = 0.0;

    for ( j = TimeWindowLeft; j < i; j++ )Sum1 += m_Quadra[j];
    Sum1 /= LeftHalfLength;  // ǰ�����ƽ����ƽ��ֵ.

    for ( j = i; j < TimeWindowRight; j++ )Sum2 += m_Quadra[j];
    Sum2 /= RightHalfLength;  // ������ƽ����ƽ��ֵ.

    Quadra = (float)sqrt(Sum1 / Sum2);  // �������.
    m_pValueOfInputNode[2] = Quadra;  // ������һ���ڵ�.



    /*
    // 3. ����õ������������ƽ��ֵ�ı�ֵ.
    Sum=0;
    for(j=i;j<TimeWindowRight;j++)Sum+=m_Abs[j];
    Sum/=RightHalfLength;
    RatioToAverage=m_Abs[i]/Sum;
    m_pValueOfInputNode[2]=RatioToAverage;

    // 4. & 5.  ����ǰ����������б��.
    // KF & KB are let small than 1,to let the neural net united.
    CurrentPeak=SearchNearestPeak(i);
    if(CurrentPeak<1)continue;								  // �޷��õķ�ֵ��.
    if(CurrentPeak>(m_PeakNumber-2))continue;   // �����ڶ�, �޷��÷�ֵ��.

    KF=m_PeakWrapAngle[CurrentPeak];
    KB=m_PeakWrapAngle[CurrentPeak+1];

    m_pValueOfInputNode[3]=KF;
    m_pValueOfInputNode[4]=KB;
       */

       ////////////////////////////////////////////////////////////////////////////////////////////////////////////
       //  �������ؽڵ�����ֵ.
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
    //��������ڵ�����ֵ.
    m_ErrorOfOutput[i] = 0;
    for ( j = 0; j < m_NodeNumberOfOutput; j++ )
    {

      // �ýڵ������ֵ.
      Sum = 0.0;
      for ( k = 0; k < m_NodeNumberOfHide; k++ )
      {
        n = j*m_NodeNumberOfOutput + k;
        Sum += m_pWeightToOutputNode[n] * m_pValueOfHideNode[k];
      }

      // �ýڵ�����ֵ.
      temp = Sum - m_pThresholdOfOutputNode[j];
      m_pValueOfOutputNode[j] = 1 / (1 + exp(-temp));

      // ���ֵ����������Ĳ�ֵ.
      //m_ErrorOfOutput[i]+=fabs(m_pValueOfOutputNode[j]-m_pExpectedOutputValue[j]);
      m_ErrorOfOutput[i] += fabs(m_pValueOfOutputNode[j]);

    }// ѭ������ڵ�����ֵ.

    //fprintf(fpTXT,"%10d,    %10.9f,  %10.9f \n",i,m_pValueOfOutputNode[0],m_DataGroup[i]);
    //TRACE("%10d,    %10.9f,  %10.9f \n",i,m_pValueOfOutputNode[0],m_DataGroup[i]);
    //for(j=0;j<2;j++){
    //	TRACE("%10.1f    ,",m_pValueOfInputNode[j]);
    //}
    //TRACE("\n");

  }// ѭ������õ������е�.

  ////////////////////////////////////////////////////////////////////////////////////////////////
  //   �ҳ���������������С�ĵ�,��Ϊ��.
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
  // �������������֮����С�ĵ��.
  return MinErrorPoint;
}
//			              	float FstBrk::NeuNetThink									    END
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//							BOOL FstBrk::CalculatePeaks								BEGIN
//  ����:���㲨��.
//  ����:1997.10.5
void NeuNet::CalculatePeaks()
{
  float a, b, c;
  m_PeakNumber = 0;

  // �����õ���һ����, ��ΪҪ�õ���i-1����.
  // �����õ����һ����, ��ΪҪ�õ���i+1����.
  for ( long i = 1; i < m_DataNumber - 1; i++ )
  {
    a = m_DataGroup[i - 1];
    b = m_DataGroup[i];
    c = m_DataGroup[i + 1];

    // ֻ���µ��㱻��Ϊ�����ǲ���.
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
    m_PeakWrapAngle[i] = fabs(h / d);   // �����ǰ��(С��)������������.
  }
}
//							BOOL FstBrk::CalculatePeaks								END
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                          BOOL FstBrk::SearchNearest									BEGIN
//	����:�ҳ������������Ĳ���.
//  �ر�����:1997.10.5
long NeuNet::SearchNearestPeak(int SearchedNumber)
{
  int a, b;
  for ( long i = 1; i < m_PeakNumber; i++ )
  {   // �����1��ʼ, ��Ϊ�õ� i-1������.
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
//  ����: ����ѧϰ, �ı�Ȩֵ.
//  �ر�����: 1997.10.5
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
  //  ����һ��Ȩֵ�仯.
  //------------------------------------------------------------------------------------------------------------
  UINT LearnTime = 0;
  do
  {

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // ���õ�ǰȨֵ����һ�����ֵ.
    Think();

    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  ���� DeltaE: �����ƽ���͵Ŀ���,���ڵ���Yingta��Alpha.
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
    // ���㵽����ڵ��Ȩֵ�ı仯.
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
    // ���㵽���ؽڵ��Ȩֵ�仯.
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
  // �洢Ȩֵ.
  SaveWeight();

  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  // ɾ����ʱ������
  delete DeltaOfOutput;

  // ����.
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
    AfxMessageBox("�޷����������������ļ�!");
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

