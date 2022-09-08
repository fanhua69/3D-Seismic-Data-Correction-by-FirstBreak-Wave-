
#ifndef tag_BlockTrace
#define tag_BlockTrace
class CBlockTrace:public CObject
{
public:
	long m_nBlock;
	double m_dLen;

	CBlockTrace(){
		m_nBlock=0;
		m_dLen=0.0;
	}

	CBlockTrace(long nBlock,double dLen){
		m_nBlock=nBlock;
		m_dLen=dLen;
	}

	void Set(long nBlock,double dLen){
		m_nBlock=nBlock;
		m_dLen=dLen;
	}
};
#endif




#ifndef tag_MemEuaqtion
#define tag_MemEuaqtion

//////////////////////////////////////////////////////////////////////////////////
//   "Equation" ���ض�Ϊ "Equa" , �������⡣ 
//  To Resolve   AX=B.
class CMemEquation{
private:

	int m_nUsing;    //  Indicating what is doing, Making or Resolving  ,Reading or Writing.
								// 0, NOT using ,  1 	Making ,  2  Resolving .
	CString m_fileA;
	CString m_fileB;
	CString m_fileX;
	CString m_fileEquaTurn;
	CString m_fileVacant;	

	FILE *m_fpA;
	FILE *m_fpAT;
	FILE *m_fpB;

	double *m_pAi2;
	double *m_pAiX;
	double *m_pFractor;
	double *m_pB;
	double *m_dpX;
	
	double *m_dpThirdX;
	double *m_dpSecondX;
	double *m_dpFirstX;

	double *m_dpMidFirstX;
	double *m_dpMidSecondX;
	double *m_dpMidThirdX;

	double *m_dpLongFirstX;
	double *m_dpLongSecondX;
	double *m_dpLongThirdX;

	double *m_dpUltraLongFirstX;
	double *m_dpUltraLongSecondX;
	double *m_dpUltraLongThirdX;


	
	long *m_lpNumInRow;
	long *m_lpNumInCol;
	long *m_lpPosInRow;

	double m_dErrorLimit;
	
	long m_lRowNum;
	long m_lColNum;
	long m_lMaxColEle;
	long m_lMaxRowEle;
		
	bool CalculateAi2();
	double CheckX();

	bool RecordVacant();
	bool FillVacant();
	
public:
	bool IsVarVacant(long nVar);
	double m_dVacantValue;
	long GetX(double *);
	bool SaveX();
	double * GetX();
	double  SetErrorLimit(double dErrorLimit);
	bool  ReadAnswer(double *pRoom);
	 CMemEquation();
	 ~CMemEquation();

	bool  Use(CString fileA,CString fileB,CString fileX="",int nInitialMethod=2,double *pInitialvalue=NULL);								// ��ʼ���÷���ʱʹ�á�
	bool  Construct(CString fileA, CString fileB,long nColumn);
	bool  Close();  // Ϊ�����������֮��,�����������֮��ʹ��.

	bool IsIdle(){return m_nUsing==0;};
	bool IsMaking(){return m_nUsing==1;};
	bool IsResolving(){return m_nUsing==2;};

	// ׷�ӷ��̣�
	bool  AppeEqua(CObArray& array,double B);

	// ת�÷��̣�
	bool Turn(); 

	//  ��ⷽ�̣�
	bool  Resolve();

	// ��ý⣺
	double  *ReadX();

	//  ���ӹ��ܣ�������ɸ�������
	bool  GetEqua(CBlockTrace *room,double *B,long EquaNo);
	long  GetXNum(){return m_lColNum;}
protected:
	bool Reset();
};

#endif