// TopoNet.h: interface for the CTopoNet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOPONET_H__6C57E5A1_FA4A_11D3_AEF5_00C04FCCB957__INCLUDED_)
#define AFX_TOPONET_H__6C57E5A1_FA4A_11D3_AEF5_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "My3DPoint.h"
#include "TagLine.h"
#include "MyPrcPoint.h"

class CTopoNet  
{
public:	
	long GetPointNumber();
	void SetTitle(CString sTitle);
	bool Open(CString sFile="");
	bool Save(CString sFile="");
	void SetReturnError(double dError);
	double GetValue(double x,double y);
	CMy3DPoint GetPoint(int i);
	void SetPrecision(double dPrecision);
	bool SetPointNumber(long nPoint);
	void SetPoint(CMyPoint &point,long nPos);
	void SetValue(double dValue,long nPos);	
	void SetPoint(CMy3DPoint &point,long nPos);
	void SetPoint(double x,double y,double z,long nPos);
	
	bool CalcNet();	

	bool GetNetPoint(CMyPrcPoint &pointMid,CMy3DPoint *pPointReturn) ;
	bool GetValue(CMy3DPoint &pointMid);
	
	CTagLine * GetLine();
	long GetLineNumber();

	CTopoNet();
	virtual ~CTopoNet();

protected:
	bool GetOnLineNetPoint(CMyPrcPoint &pointMid,long nLine,CMy3DPoint *pPointReturn); 
	bool GetVertexNetPoint(CMyPrcPoint &pointMid,long nVertex,CMy3DPoint *pPointReturn); 
	bool GetOutNetPoint(CMyPrcPoint &pointMid,CMy3DPoint *pPointReturn); 
	bool GetInNetPoint(CMyPrcPoint &pointMid,CMy3DPoint *pPointReturn); 
	void Reset();

	CString m_sTagFile;

	CString m_sTitle;


	CMy3DPoint *m_pPoint;
	long m_nPoint;


	CTagLine *m_pLine;
	long m_nLine;
	long m_nLineLimit;

	double m_dPrecision;
	double m_dError;

	bool m_bReportError;
};

#endif // !defined(AFX_TOPONET_H__6C57E5A1_FA4A_11D3_AEF5_00C04FCCB957__INCLUDED_)
