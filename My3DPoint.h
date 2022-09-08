// My3DPoint.h: interface for the CMy3DPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MY3DPOINT_H__199DF6E1_FA65_11D3_AEF5_00C04FCCB957__INCLUDED_)
#define AFX_MY3DPOINT_H__199DF6E1_FA65_11D3_AEF5_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Mypoint.h"

class CMy3DPoint : public CMyPoint  
{
public:
	CMy3DPoint(double x,double y,double z);
	CMy3DPoint();
	virtual ~CMy3DPoint();
	double z;
	void operator =(CMy3DPoint &other);


};

#endif // !defined(AFX_MY3DPOINT_H__199DF6E1_FA65_11D3_AEF5_00C04FCCB957__INCLUDED_)
