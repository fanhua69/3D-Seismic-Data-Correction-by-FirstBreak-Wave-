// ZDEquation.h: interface for the CZDEquation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZDEQUATION_H__72B37B17_F520_11D3_AEF5_00C04FCCB957__INCLUDED_)
#define AFX_ZDEQUATION_H__72B37B17_F520_11D3_AEF5_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define RCV_LIMIT 800
#define HALF_RCV_LIMIT 400
#define QUARTER_RCV_LIMIT 200
#define EIGHTH_RCV_LIMIT 100

#define SHOT_LIMIT 20

#include "MyPoint.h"
#include "MemEqua.h"
#include "RcvPoint.h"

class CCouple
{
public:
	long nRcvA;
	long nRcvB;

	CCouple()
	{
		nRcvA=-1;
		nRcvB=-1;
	};

};

class CShotPoint
{
public:
	long lNo;
	CMyPoint Pos;

	CShotPoint()
	{
		lNo=0;
	};

};

class CShotRcv
{
public:
	// Shot point par:
	CShotPoint Shot;

	// Recieve point par:
	long lRcvNum;
	CRcvPoint *Rcv;
	CShotRcv()
	{
		lRcvNum=0;
		Rcv=new CRcvPoint[RCV_LIMIT];
	}

	~CShotRcv()
	{
		delete []Rcv;
	}

};

class CZDEquation  
{
public:
	bool SetPrecision(double dPrecision);
	
	CZDEquation();
	virtual ~CZDEquation();

	void SetFoldTime(int nFoldTime);
	void SetCommonShotGroup(CShotRcv *pShotRcv,int nShotRcv);
	bool Construct(CString sA,CString sB,long nShotNum,long nRcvNum,float fInitVel);
	bool Append(CShotRcv *pShotRcv,int nShotRcv);
	bool Close();


private:
	bool CalcCouple(CShotRcv *pShotRcv,int nShotRcv);
	long SearchCoupleData(long nRcv,CCouple *pCouple,long nCoupleNumber);
	void Reset();

	int m_nFoldTime;  // =1,2,3

	long m_nCoupleNumber[SHOT_LIMIT];
	long m_nSecondCoupleNumber[SHOT_LIMIT];
	long m_nThirdCoupleNumber[SHOT_LIMIT];

	CCouple (*m_Couple)[HALF_RCV_LIMIT ];
	CCouple (*m_SecondCouple)[QUARTER_RCV_LIMIT ];
	CCouple (*m_ThirdCouple)[EIGHTH_RCV_LIMIT ];
	
	CMemEquation m_Equation;
	CString m_sEquationA;
	CString m_sEquationB;

	long m_lShotPointNumber;
	long m_lRcvPointNumber;
	double m_dSmallDistance;
	double m_dBigDistance;


	float m_fInitVel;
	double m_dPrecision;

protected:
	void CalcWeight(CShotRcv *pShotRcv);
	void SortRcv(CShotRcv *pShotRcv);
};

#endif // !defined(AFX_ZDEQUATION_H__72B37B17_F520_11D3_AEF5_00C04FCCB957__INCLUDED_)
