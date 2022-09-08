// TagLine.h: interface for the CTagLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAGLINE_H__C9E8C0A1_FF35_11D3_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_TAGLINE_H__C9E8C0A1_FF35_11D3_A4E4_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Line.h"
#include "MyPrcPoint.h"	// Added by ClassView

class CTagLine : public CLine  
{
public:
	CMyPrcPoint MidPoint();
	long CommonPoint(CTagLine &other);
	bool Linking(long i);
	CTagLine();
	virtual ~CTagLine();
	void operator =(CTagLine &other);

	long nPointHead;
	long nPointTail;

};

#endif // !defined(AFX_TAGLINE_H__C9E8C0A1_FF35_11D3_A4E4_00C04FCCB957__INCLUDED_)
