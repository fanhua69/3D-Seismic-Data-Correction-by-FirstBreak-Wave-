// MidFbkFile.h: interface for the CMidFbkFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OutFBKFILE_H__F4E18285_143C_11D4_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_OutFBKFILE_H__F4E18285_143C_11D4_A4E4_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PXYV.h"

class COutFbkFile  
{
public:
	bool WriteShot(CString sFile="");
	bool WriteRcv(CString sFile="");
	bool ReadRcv(CString sFile="");
	bool ReadShot(CString sFile="");
	bool SetRcv(PXYV *pRcv, long nData);
	bool SetShot(PXYV *pRcv, long nData);
	bool SetShotNumber(long n);
	bool SetRcvNumber(long n);
	long SearchShotStation(long nPh);
	long SearchRcvStation(long nPh);
	void SortShotData();
	void SortRcvData();

	COutFbkFile();
	virtual ~COutFbkFile();

	long GetShotNumber(){return m_nShotData;};
	long GetRcvNumber(){return m_nRcvData;};
	PXYV* GetShotData(){return m_pShotData;};
	PXYV* GetRcvData(){return m_pRcvData;};

	PXYV *m_pShotData;
	PXYV *m_pRcvData;

protected:
	long m_nShotData;
	long m_nRcvData;
	long m_nShotDataLimit;
	long m_nRcvDataLimit;

	void Reset();


};

#endif // !defined(AFX_OutFBKFILE_H__F4E18285_143C_11D4_A4E4_00C04FCCB957__INCLUDED_)
