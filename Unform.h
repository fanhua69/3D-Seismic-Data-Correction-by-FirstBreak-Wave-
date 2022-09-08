
#ifndef tagUnformFile
#define tagUnformFile
	#include "shotmsg.h"
	class UnformFile
	{
	private:
		FILE *m_fpUnform;
		
	public:
		BOOL m_UnDataOK;

		CString m_FileUnform;
		long m_TopFileNumber;
		long m_EndFileNumber;
		long m_UnformGroupNumber;  //���ļ��й��ж��ٵ�.
		long m_UnformGroupHead;     // =128 in format 4
		long m_FileNumberPosition;  //=220	in format 4
		long m_nGroupNamePosition; //=17*4;

		long m_UnformFileLength;
		long m_UnformTimeInterval;
		long m_UnformTimeLength;
		long m_ByteNumberOfOneGroup;
		long m_PointNumberOfOneGroup;
		long m_DataNumberOfOneGroup;

		long m_MaxGroupNumber;

		long m_TotalShotNumber;
		ShotMsg* m_pShotMsg;
	
		UnformFile();
		BOOL Set(CString );
		~UnformFile();

		long GetFileNumber(long Group);
		long GetFileNumber(float* GroupData);
		long GetGroupNumber(float *GroupData);
		long GetGroup(float *GroupData,long BeginGroup,long GroupNumber=1);
		BOOL  GetShot(float *DataRoom,long Shot);
		BOOL  SetShotMsg();
	};

#endif

/*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	GRISYS ��ͷ��Ϣ��:

  9      1-32   SamInt			�������, ������16, ������΢��mus.
10 	    1-32   TRACEL 		 Group Length ms.
17      1-32						Ұ�����.
55		1-32   FORMAT       Data format .
										=1,  16 bits �����ʽ, 
										=2,   12 bits data + 4 bits amplification.
										=3,   IBM 32 bits floating point format.
										=4,   32 bits floating point data ( only used in this system I/O.
56     1-32    MNUMFIL     field file number.
124   1-32    STNUMB      Group number of one shot.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
