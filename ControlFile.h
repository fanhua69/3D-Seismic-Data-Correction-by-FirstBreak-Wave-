// ControlFile.h: interface for the CControlFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTROLFILE_H__B302C7C1_155E_11D4_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_CONTROLFILE_H__B302C7C1_155E_11D4_A4E4_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PXYV.h"

class CControlFile  
{
public:
	bool SetShotCtl(long nPos,PXYV &point);
	bool SetShotCtlNumber(long nShotCtlNumber);
	bool SetRcvCtl(long nPos,PXYV &point);
	bool SetRcvCtlNumber(long nRcvCtlNumber);
	
	long GetRcvCtlNumber();
	long GetShotCtlNumber();
	PXYV * GetShotCtl();
	PXYV * GetRcvCtl();

	CControlFile();
	virtual ~CControlFile();
	void Reset();

	bool Read(CString sFile="");
	bool Write(CString sFile="");

public:	
	PXYV *m_pShotCtlPoint;
	PXYV *m_pRcvCtlPoint;

private:
	long m_nShotCtlPoint;
	long m_nRcvCtlPoint;
	long m_nRcvCtlLimit;
	long m_nShotCtlLimit;

};

#endif // !defined(AFX_CONTROLFILE_H__B302C7C1_155E_11D4_A4E4_00C04FCCB957__INCLUDED_)
