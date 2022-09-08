 #ifndef tagSwathParameter

#define tagSwathParameter
#include "cdswpar.h"
#include "drlnspn.h"	
#include "Mypoint.h"	// Added by ClassView

#include "RcvPoint.h" 
#include "SvSys.h"


class  SwathParameter
{
public:
	CMyPoint GetShotPos(long nPh,long nZP,long nHP);
	bool SetFileName(int nSwath);
    CString SwathParameterFileName; 
	CString SwathParameterFileName1;
	CString SwathParameterFileName2;
                                   
	long Distance1;
	long Distance2;
	long Distance3;
	long Distance4;

	long Group1;
	long Group2;
	long Group3;
	long Group4;

	long ShotLineFrom;
	long ShotLineTo;            
                    
	long ShotLineNumber;
	long TotalUnformRecievePointNumber;
	long TotalRecievePointNumber;
	long InitialVelocity;
	long FoldTime;
	long FirstRecievePointNumber;

	long TopPlusSquareLength;
	long BottomPlusSquareLength;

	int RLNBoxNumber;
	int SPNBoxNumber;

	long RecieveLineName[50]; // RLNBoxNumber
	long ShotPointName[50];   // SPNBoxNumber 
                
	BOOL DataOK;

public :
	CRcvPoint * GetRcvPoint(long nShotPh);
	// Functions:
                                                                
	SwathParameter();    
	~SwathParameter();
	
	void Reset();
	BOOL Set(int Swath=-1);
	BOOL AskCommonParameter();
	BOOL AskRLNSPN();
	BOOL MakeShotPh(long *Ph,long *PhNumber);

private:
	SvSys ss;
    BOOL ReadCommonParameter();
    BOOL ReadRLNSPN();
    BOOL WriteCommonParameter(cdswpar *dlg);
	BOOL WriteRLNSPN(CDRLNSPN *dlg);
    BOOL CalculateParameters();
	long MakePh(long nShotLine,long nShotPoint);

	CRcvPoint *m_pRcvForOneShot;
};        
                                 
#endif
