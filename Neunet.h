
#ifndef NeuNet_H
#define NeuNet_H
class NeuNet
{
private:
	CString m_FileNeuNet;

	float *m_DataGroup;	  //输入的数组
	long m_DataNumber;  // 数组中数值个数.
	double *m_Abs;       //数组中数值的绝对值.
	double *m_Square;  //数组中数值的振幅平方.
	double *m_Quadra;  //数组中数值的振幅平方的平方.
	double *m_ErrorOfOutput;  //数组中数值点的输出与期望输出的差.
	int *m_PeakRoom;  // 第n个峰值所对应的点号.
	double *m_PeakWrapAngle;   // 波峰点前边(小号)方向包络的正弦.

	long m_PeakNumber;  //波峰个数.
	long m_ChosenPoint;  // 用户给的答案点号码.
	double m_LearnErrorLimit;  // 学习时, 给定的学习完毕误差限制.
	
	int m_NodeNumberOfInput;
	int m_NodeNumberOfHide;
	int m_NodeNumberOfOutput;

	double *m_pWeightToHideNode;
	double *m_pWeightToOutputNode;
	double *m_pWeightToHideNodeLast;
	double *m_pWeightToOutputNodeLast;
																					   
	double *m_pThresholdOfHideNode;
	double *m_pThresholdOfOutputNode;
			 
	double *m_pValueOfInputNode;
	double *m_pValueOfHideNode;
	double *m_pValueOfOutputNode;

	double *m_pExpectedOutputValue;

	int m_MinimumNoiseDataNumber;  // Minimum Noise Data Number, to let neural net calculate.
	
	BOOL CalculateOutputValue(float *Weight,float *Value);
	long SearchNearestPeak(int );
	BOOL ReadWeight();
	BOOL SaveWeight();
	void CalculatePeaks();
	void RandomlyGiveWeight();

public:
	NeuNet();
	Set(float *DataGroup,long DataNumber,int ChosenPoint=0,bool bFirstBreak=true);
	~NeuNet();

	BOOL CheckNeuNetLearned();

	long Think();
	BOOL Learn();
};
#endif