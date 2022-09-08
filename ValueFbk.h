// ValueFbk.h: interface for the CValueFbk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUEFBK_H__E294D741_2688_11D4_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_VALUEFBK_H__E294D741_2688_11D4_A4E4_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CValueFbk  
{
public:
	int CalcFbk(float *pGroup);
	void SetPointNumber(int nPoint);

	CValueFbk();
	virtual ~CValueFbk();

protected:
	float *m_pGroup;
	int m_nPointNumber;

};

#endif // !defined(AFX_VALUEFBK_H__E294D741_2688_11D4_A4E4_00C04FCCB957__INCLUDED_)
