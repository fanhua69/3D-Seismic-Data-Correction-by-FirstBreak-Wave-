// MyPrcPoint.h: interface for the CMyPrcPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYPRCPOINT_H__22D63BE1_FE3A_11D3_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_MYPRCPOINT_H__22D63BE1_FE3A_11D3_A4E4_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Mypoint.h"
#include "My3DPoint.h"

class CMyPrcPoint : public CMyPoint  
{
public:
	CMyPrcPoint(double x,double y,double Precision=0);
	CMyPrcPoint();
	virtual ~CMyPrcPoint();
	double m_dPrecision;

	bool operator ==(CMyPrcPoint &point);
	bool operator ==(CMy3DPoint &point);
};

#endif // !defined(AFX_MYPRCPOINT_H__22D63BE1_FE3A_11D3_A4E4_00C04FCCB957__INCLUDED_)
