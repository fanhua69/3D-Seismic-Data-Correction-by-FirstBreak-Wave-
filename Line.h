// Line.h: interface for the CLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINE_H__1924F321_D569_11D2_AD59_444553540000__INCLUDED_)
#define AFX_LINE_H__1924F321_D569_11D2_AD59_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "MyPoint.h"
#include "MyPrcPoint.h"	// Added by ClassView

class CLine  
{
public:
	CLine();
	virtual ~CLine();

public:
	CMyPrcPoint GetPerpFoot(CMyPrcPoint &point);
	bool PtInLine(CMyPrcPoint &point);
	CMyPrcPoint PointA();
	CMyPrcPoint PointB();
	bool BetweenValue(double x,double x1,double x2,double dPrecision=-1.0);
	void SetPrecision(double dPrecision);
	bool Equal(CLine& pOther);
	CLine(double x1,double y1,double x2,double y2);
	bool CrossIn(CLine &other,CMyPoint *pPointCross=NULL);
	double Distance(CMyPoint &point);
	void CalcLen();
	CMyPoint Cross(CLine &pOther,bool *bOnLine=false);

	void operator =(CLine &other);
	bool operator ==(CLine &other);

	double x1,y1,x2,y2;
	double m_dLen;

protected:
	long m_dPrecision;
	
	
};

#endif // !defined(AFX_LINE_H__1924F321_D569_11D2_AD59_444553540000__INCLUDED_)
