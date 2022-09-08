
//   File Format   at   the end of this file.
#ifndef tagSRR
#define tagSRR
class SRR{
public:
	long Shot;
	long ShotTime;
	long Recieve[8];
};
#endif

#include "pxyv.h"
#include "svsys.h"
#include "swathpar.h"

#ifndef tagEquation
	#define tagEquation
	class Equation
	{
    public:
		Equation();
		bool Set(int Swath);
		SvSys ss;
		SwathParameter sp;

		// 初至文件的名称.
		CString m_FileSwathFirstBreak;
		long m_TotalSNumber;
		long m_TotalRNumber;
		long m_TotalRSNumber;		

		//用于对三个静校正量文件的存取:
		BOOL ReadInputStaticData(PXYV *S,PXYV *R,long *SN,long *RN);
		BOOL SaveInputStaticData(PXYV *S,PXYV *R,long SN,long RN);
		BOOL ReadMidStaticData(long *Ph,double *RSRoom);
		BOOL SaveMidStaticData(double *RSRoom);
		BOOL ReadOutputStaticData(PXYV *S,PXYV *R);
		BOOL SaveOutputStaticData(PXYV *S,PXYV *R);

	public:

		long m_SwathNumber;

		// Equation File.
		CString m_FileName;   // It will be used by derived class, need be a class member.
		CString m_FileEquationNumber;   // Text file including m_RSEquationNumber .
		CString m_FileRS;     // Binary file recording which shot and recieve plused.
		CString m_Fileb;	// Binary file recording S & R value plused.
		CString m_FileNumInColOfRS;
		CString m_FileRSTurned;

		// Static Value files.
		CString m_FileInputRecieve;     // Binary File including Ph,X,Y,V, calculated by Sand Wave Method.
		CString m_FileMidShot;             // Binary File including Static Value  ,Calculated by this Program , Not Controlled. 
		CString m_FileMidRecieve;       // Binary File including Static Value ,Calculated by this Program , Not Controlled. 
		CString m_FileOutputShot;         // Binary  File that has been controlled.
		CString m_FileOutputRecieve;   // Binary  File that has been controlled.
	
	public:
		void Reset();
		bool SetFileName(int n);
		// Special for CTDoc, to determine if the selcted file is a valid file according to the name.
		CString m_FileInputShot;           // Binary File including Ph,X,Y,V, calculated by Sand Wave Method.
			
	};

#endif

#ifndef tagTwoData
#define tagTwoData
	
	typedef struct{
		long Data1;
		float Data2;
	}TwoData;

	typedef struct{
		long Data1;
		long Data2;
	}TwoLongData;

#endif

/* FILE FORMAT:

		  A.   Recieve Point Static Value File,Named in "NameOfRecievePointValue":
					1.  Binary file.
					2.  Every two bytes stand for one INTEGER value.
					3.  Total number is sp->TotalRecievePointNumber.
		  A.   Recieve Point Static Value File,Named in "NameOfRecievePointValue":
					1.  Binary file. 
					2.  Every two bytes stand for one INTEGER value.
					3.  Total number is sp->TotalShotPointNumber.
*/
   


