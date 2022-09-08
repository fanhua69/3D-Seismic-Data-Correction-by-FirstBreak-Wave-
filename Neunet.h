
#ifndef NeuNet_H
#define NeuNet_H
class NeuNet
{
private:
	CString m_FileNeuNet;

	float *m_DataGroup;	  //���������
	long m_DataNumber;  // ��������ֵ����.
	double *m_Abs;       //��������ֵ�ľ���ֵ.
	double *m_Square;  //��������ֵ�����ƽ��.
	double *m_Quadra;  //��������ֵ�����ƽ����ƽ��.
	double *m_ErrorOfOutput;  //��������ֵ����������������Ĳ�.
	int *m_PeakRoom;  // ��n����ֵ����Ӧ�ĵ��.
	double *m_PeakWrapAngle;   // �����ǰ��(С��)������������.

	long m_PeakNumber;  //�������.
	long m_ChosenPoint;  // �û����Ĵ𰸵����.
	double m_LearnErrorLimit;  // ѧϰʱ, ������ѧϰ����������.
	
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