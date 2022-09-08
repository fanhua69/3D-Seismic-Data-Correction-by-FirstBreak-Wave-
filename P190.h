// P190.h: interface for the CP190 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_P190_H__4436C301_0ACD_11D4_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_P190_H__4436C301_0ACD_11D4_A4E4_00C04FCCB957__INCLUDED_

#include "Mypoint.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CShotPhyPar
{
public:
	long Ph;
	double east;
	double north;
	float gd;
	float h;
	int st;
	CShotPhyPar()
	{
		Ph=east=north=gd=h=st=0.0;
	};

	void operator =(CShotPhyPar& other)
	{
		Ph=other.Ph;
		east=other.east;
		north=other.north;
		gd=other.gd;
		st=other.st;
		h=other.h;
	};


};

class CRcvPhyPar
{
public:
	long Ph;
	double east;
	double north;
	float gd;
	float h;
	int rt;
	CRcvPhyPar()
	{
		Ph=east=north=gd=h=rt=0.0;
	};

	void operator =(CRcvPhyPar& other)
	{
		Ph=other.Ph;
		east=other.east;
		north=other.north;
		gd=other.gd;
		rt=other.rt;
		h=other.h;
	};

};

class CRcvRange 
{
public:
	long start;
	long end;
};

class CShotRcvRel
{
public:
	long PhShot;
	long FileNumber;
	
	long nRcvLineLimit;
	long nRcvLine;
	CRcvRange RcvRange[100];
	CShotRcvRel(){
		PhShot=0;
		FileNumber=0;
		nRcvLineLimit=100;
		nRcvLine=0;
	};

};



class CP190  
{
public:
	bool CalcShotOrgPos();
	CP190();
	virtual ~CP190();

public:
	bool SaveShotStatic();
	bool SaveRcvStatic();

	bool FormatChange(CString sFile="");
	long GetShotRcvRel(long nShotPh);
	CMyPoint * GetRcvPosition(long nShotPh,long &nRcvPoint,long nStartPoint=0, long nEndPoint=0);
	CMyPoint GetShotPos(long nPh);
	long SearchRcvStation(long nStation);
	long SearchShotStation(long nStation);
	void SortRcvPoint();
	void SortShotPoint();
	long m_nShotPhyNumber;
	long m_nRcvPhyNumber;
	long m_nRelNumber;
	
	CShotPhyPar *m_pShotPhyPar;
	CRcvPhyPar *m_pRcvPhyPar;
	CShotRcvRel *m_pShotRcvRel;
	CMyPoint *m_pPointOfOneShot;

protected:
	void Reset();

};

#endif // !defined(AFX_P190_H__4436C301_0ACD_11D4_A4E4_00C04FCCB957__INCLUDED_)
