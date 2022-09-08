// InP190.h: interface for the CInP190 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INP190_H__4436C302_0ACD_11D4_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_INP190_H__4436C302_0ACD_11D4_A4E4_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "P190.h"

class CInP190 : public CP190  
{
public:
	bool ReadShotPar(CString sFile="");
	bool ReadShotRcvRel(CString sFile="");
	bool ReadRcvPar(CString sFile="");
	bool Open(CString sFile="");
	CInP190();
	virtual ~CInP190();

};

#endif // !defined(AFX_INP190_H__4436C302_0ACD_11D4_A4E4_00C04FCCB957__INCLUDED_)
