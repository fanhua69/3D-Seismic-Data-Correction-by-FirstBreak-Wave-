// Line.cpp: implementation of the CLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Line.h"
#include "math.h"
#include "MyPrcPoint.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLine::CLine()
{
	x1=y1=x2=y2=0.0;	
	m_dLen=-1;
	m_dPrecision=0;
}

CLine::~CLine()
{

}

CMyPoint CLine::Cross(CLine &pOther,bool *bOnLine)
{
	double k1,k2;
	if(pOther.x2!=pOther.x1){
		k2=(pOther.y2-pOther.y1)/(pOther.x2-pOther.x1);
	}

	if(x1!=x2){
		k1=(y2-y1)/(x2-x1);
	}

	// Check the extreme problems:
	bool bCross=false;
	double x = -1,y = -1;
	if(x2==x1&&pOther.x1!=pOther.x2){
		x=x1;
		y=k2*(x-x1)+y1;
		bCross=true;
	}
	else if(pOther.x1==pOther.x2&&x2!=x1){
		x=pOther.x1;
		y=k1*(x-x1)+y1;
		bCross=true;
	}
	else if((x2==x1 && pOther.x2==pOther.x1)||(k1==k2)){
		double dx=x2-x1,dy=y2-y1;
		double dLenLine1=sqrt(dx*dx+dy*dy);
		
		// The distance from the FIRST point of the second line to the this line's two points:
		dx=pOther.x1-x1,dy=pOther.y1-y1;
		double l1=sqrt(dx*dx+dy*dy);

		dx=pOther.x1-x2,dy=pOther.y1-y2;
		l1+=sqrt(dx*dx+dy*dy);

		// The distance from the SECOND point of the second line to the this line's two points:
		dx=pOther.x2-x1,dy=pOther.y2-y1;
		double l2=sqrt(dx*dx+dy*dy);
		dx=pOther.x2-x2;dy=pOther.y2-y2;
		l2+=sqrt(dx*dx+dy*dy);

		if((l1-dLenLine1)<0.5){
			x=pOther.x1;
			y=pOther.y1;
			bCross=true;
		}
		else if((l2-dLenLine1)<0.5){
			x=pOther.x2;
			y=pOther.y2;
			bCross=true;
		}
	}
	else{
		x=k2*pOther.x1-k1*x1-pOther.y1+y1;
		x/=k2-k1;

		y=k1*(x-x1)+y1;
		bCross=true;
	}

	// bOnLine:
	// This value is both for indicating for weather check , 
	//  and for returning the sign for if the meet point is on both line
	if(!bCross){
		*bOnLine=false;
	}
	else{
		if(*bOnLine){  
			/* Check if the meet point is on both lines:
			' make sure 1,3 point are on the smaller end,
			' maily for checking
			' if the meet point is on the two lines.
			' calculate the meet point.
			*/

			double xon1=(x1-x)*(x2-x);
			double yon1=(y1-y)*(y2-y);
			double xon2=(pOther.x1-x)*(pOther.x2-x);
			double yon2=(pOther.y1-y)*(pOther.y2-y);

			if(xon1>0||yon1>0||xon2>0||yon2>0){
				*bOnLine=false;
			}
			else{
				*bOnLine=true;
			}		
		}
	}

	return CMyPoint(x,y);

}

void CLine::CalcLen()
{
	m_dLen=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

double CLine::Distance(CMyPoint &point)
{
	double k=(x2-x1)/(y2-y1);

	// Line equation: x-ky+(ky1-x1)=0	
	double a=1;
	double b=-k;
	double c=k*y1-x1;

	return fabs(a*point.x+b*point.y+c)/sqrt(a*a+b*b);

}

void CLine::operator =(CLine &other)
{
	x1=other.x1;
	x2=other.x2;
	y1=other.y1;
	y2=other.y2;
	m_dLen=other.m_dLen;
	m_dPrecision=other.m_dPrecision;
}

bool CLine::CrossIn(CLine &pOther,CMyPoint *pPointCross)
{

	double ox1,oy1,ox2,oy2;

	ox1=pOther.x1;
	oy1=pOther.y1;
	ox2=pOther.x2;
	oy2=pOther.y2;


	// The 2 Lines are same:
	if(this->Equal(pOther)){
		if(pPointCross){
			pPointCross->x=x1;
			pPointCross->y=y1;
		}
		return true;
	}

	// Calc the k of the 2 lines:
	double k1,k2;
	if(ox2!=ox1){
		k2=(oy2-oy1)/(ox2-ox1);
	}

	if(x1!=x2){
		k1=(y2-y1)/(x2-x1);
	}

	// Check the extreme problems:
	bool bCross=false;
	double d1,d2;
	double x = -1,y = -1;
	
	///////////////////////////////////////
	// This line is vertical:
	if(x2==x1&&ox1!=ox2){
		x=x1;					  
		y=k2*(x-ox1)+oy1;

		if(oy1!=oy2){
			bCross=BetweenValue(y,y1,y2)&&
				BetweenValue(x,ox1,ox2)&&
				BetweenValue(y,oy1,oy2);
		}
		else{
			bCross=BetweenValue(y,y1,y2)&&
				BetweenValue(x,ox1,ox2);
		}
	}
	
	///////////////////////////////////////
	// Other line is vertical:	
	else if(ox1==ox2&&x2!=x1){
		x=ox1;
		y=k1*(x-x1)+y1;

		if(y2!=y1){
			bCross=BetweenValue(x,x1,x2)&&
				BetweenValue(y,y1,y2)&&
				BetweenValue(y,oy1,oy2);
		}
		else{
			bCross=BetweenValue(x,x1,x2)&&
				BetweenValue(y,oy1,oy2);
		}
	}

	//////////////////////////////////////////
	//   These 2 lines are vertical:
	else if((x2==x1 && ox2==ox1)){
		
		// Firstly Check if the 2 section are on same line:
		if(x1!=ox1){
			bCross=false;
			x=y==-99999;
		}
		else{
			bool boy1=BetweenValue(oy1,y1,y2);
			bool boy2=BetweenValue(oy2,y1,y2);
			bool by1=BetweenValue(y1,oy1,oy2);
			bool by2=BetweenValue(y2,oy1,oy2);

			bCross=(boy1||boy2||by1||by2);

			x=x1;
			y=-9999;
			if(boy1<0)y=oy1;
			if(boy2<0)y=oy2;
			if(by1<0)y=y1;
			if(by2<0)y=y2;
		}
	}


	/////////////////////////////////////////////////////
	//   These 2 lines are Not vertical but with same k:
	else if(k1==k2){
		double b1=-k1*x1+y1;
		double b2=-k2*ox1+oy1;
		if(b1!=b2){
			bCross=false;
			x=y=-9999;
		}
		else{
			bool box1=BetweenValue(ox1,x1,x2);
			bool box2=BetweenValue(ox2,x1,x2);
			bool bx1=BetweenValue(x1,ox1,ox2);
			bool bx2=BetweenValue(x2,ox1,ox2);


			bCross=(box1||box2||bx1||bx2);

			x=x1;
			y=-9999;
			if(box1)x=ox1;y=oy1;
			if(box2)x=ox2;y=oy2;
			if(bx1)x=x1;y=y1;
			if(bx2)x=x2;y=y2;
		}
	}				

	////////////////////////////////////////////////////
	//   These 2 lines are Not vertical also with not same k:
	else{
		x=k2*ox1-k1*x1-oy1+y1;
		x/=k2-k1;

		y=k1*(x-x1)+y1;

		bool bx1=BetweenValue(x,x1,x2);
		bool bx2=BetweenValue(x,ox1,ox2);
		bool by1=BetweenValue(y,y1,y2);
		bool by2=BetweenValue(y,oy1,oy2);

		CMyPrcPoint pa1(x1,y1);
		CMyPrcPoint pa2(x2,y2);
		CMyPrcPoint pb1(ox1,oy1);
		CMyPrcPoint pb2(ox2,oy2);


		if(pa1==pb1||pa1==pb2||pa2==pb1||pa2==pb2){
			bCross=false;			// Just for when only one point same , 
									// but the computer 's precision give wrong output:
		}
		else if(k1==0){
			bCross=BetweenValue(x,x1,x2)
				&&BetweenValue(x,ox1,ox2)
				&&BetweenValue(y,oy1,oy2);			
		}
		else if(k2==0){
			bCross=BetweenValue(x,x1,x2)
				&&BetweenValue(y,y1,y2)
				&&BetweenValue(x,ox1,ox2);
		}
		else{			 
			bCross=BetweenValue(x,x1,x2)
				&&BetweenValue(y,y1,y2)
				&&BetweenValue(x,ox1,ox2)
				&&BetweenValue(y,oy1,oy2);
		}
	}

	///////////////////////////////////////
	//  Return:
	if(pPointCross){
		pPointCross->x=x;
		pPointCross->y=y;
	}

	return bCross;
	
}

CLine::CLine(double dx1, double dy1, double dx2, double dy2)
{
	x1=dx1;
	y1=dy1;
	x2=dx2;
	y2=dy2;
	m_dPrecision=0.0;
}

bool CLine::operator ==(CLine &other)
{
	return (x1==other.x1&&y1==other.y1&&x2==other.x2&&y2==other.y2);
}

bool CLine::Equal(CLine &other)
{
	CMyPrcPoint pa1(x1,y1,m_dPrecision);
	CMyPrcPoint pa2(x2,y2,m_dPrecision);
	CMyPrcPoint pb1(other.x1,other.y1,m_dPrecision);
	CMyPrcPoint pb2(other.x2,other.y2,m_dPrecision);

	return(pa1==pb1&&pa2==pb2||pa1==pb2&&pa2==pb1);
}

void CLine::SetPrecision(double dPrecision)
{
	m_dPrecision=dPrecision;
}

bool CLine::BetweenValue(double x, double x1, double x2,double dPrecision)
{
	if(x2<x1){
		double temp=x1;
		x1=x2;
		x2=temp;
	}

	if(dPrecision==-1)dPrecision=m_dPrecision;
	double sx1=x1-dPrecision;
	double bx2=x2+dPrecision;
	
	return(x>sx1&&x<bx2);
}

CMyPrcPoint CLine::PointA()
{
	return CMyPrcPoint(x1,y1);
}

CMyPrcPoint CLine::PointB()
{
	return CMyPrcPoint(x2,y2);
}

bool CLine::PtInLine(CMyPrcPoint &point)
{
	// The line is a point:
	if(x2==x1&&y2==y1){
		return false; // because this function is Inline!
	}

	// The line is vertical:
	if(x2==x1){
		if(point.x!=x1)return false;

		double sy=y1;
		double by=y2;
		if(y2<sy){
			sy=y2;
			by=y1;
		}

		return (point.y>sy&&point.y<by);
	}

	// The line is horizontal:
	if(y2==y1){
		if(point.y!=y1)return false;

		double sx=x1;
		double bx=x2;
		if(x2<sx){
			sx=x2;
			bx=x1;
		}

		return (point.x>sx&&point.x<bx);
	}

	// if the point is at the edge of the line:
	if(point.x==x1||point.x==x2||point.y==y1||point.y==y2)return false;

	// the line is normal:
	double k=(y2-y1)/(x2-x1);
	double y=k*(point.x-x1)+y1;
	double sy=y-m_dPrecision;
	double by=y+m_dPrecision;
	if(point.y<sy||point.y>by)return false;

	double sx=x1;
	double bx=x2;
	if(x2<sx){
		sx=x2;
		bx=x1;
	}
	if(point.x<sx||point.x>bx)return false;

	sy=y1;
	by=y2;
	if(y2<sy){
		sy=y2;
		by=y1;
	}
	if(point.y<sy||point.y>by)return false;

	return true;

}

CMyPrcPoint CLine::GetPerpFoot(CMyPrcPoint &point)
{
	double x,y;
	if(x2==x1){
		return CMyPrcPoint(x1,point.y);
	}

	if(y2==y1){
		return CMyPrcPoint(point.x,y1);
	}

	double k1=(y2-y1)/(x2-x1);
	double k2=-1/k1;

	x=k2*point.x-k1*x1-point.y+y1;
	x/=k2-k1;

	y=k1*(x-x1)+y1;

	return CMyPrcPoint(x,y);
}
