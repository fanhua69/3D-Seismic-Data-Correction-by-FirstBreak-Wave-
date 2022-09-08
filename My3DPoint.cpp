// My3DPoint.cpp: implementation of the CMy3DPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3dfstbrk.h"
#include "My3DPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMy3DPoint::CMy3DPoint()
{

}

CMy3DPoint::~CMy3DPoint()
{

}

void CMy3DPoint::operator =(CMy3DPoint &other)
{
	x=other.x;
	y=other.y;
	z=other.z;
}

CMy3DPoint::CMy3DPoint(double ox, double oy, double oz)
{
	x=ox;
	y=oy;
	z=oz;

}
