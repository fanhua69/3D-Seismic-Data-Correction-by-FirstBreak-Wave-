// MyPrcPoint.cpp: implementation of the CMyPrcPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3dfstbrk.h"
#include "MyPrcPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyPrcPoint::CMyPrcPoint()
{

}

CMyPrcPoint::~CMyPrcPoint()
{

}

CMyPrcPoint::CMyPrcPoint(double ox, double oy, double dPrecision)
{
	x=ox;
	y=oy;
	m_dPrecision=dPrecision;
}

bool CMyPrcPoint::operator ==(CMyPrcPoint &point)
{
	double x1=point.x-m_dPrecision ;
	double x2=point.x+m_dPrecision ;
	double y1=point.y-m_dPrecision ;
	double y2=point.y+m_dPrecision ;


	return (x>=x1&&x<=x2&&y>=y1&&y<=y2); 
}

bool CMyPrcPoint::operator ==(CMy3DPoint &point)
{
	double x1=point.x-m_dPrecision ;
	double x2=point.x+m_dPrecision ;
	double y1=point.y-m_dPrecision ;
	double y2=point.y+m_dPrecision ;

	return (x>=x1&&x<=x2&&y>=y1&&y<=y2); 
}

