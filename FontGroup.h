// FontGroup.h: interface for the CFontGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FONTGROUP_H__199DF6E4_FA65_11D3_AEF5_00C04FCCB957__INCLUDED_)
#define AFX_FONTGROUP_H__199DF6E4_FA65_11D3_AEF5_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFontGroup  
{
public:
	void SetValueRange(double dMinValue,double dMaxValue);
	CFontGroup();
	virtual ~CFontGroup();

	void SetWindowHeight(int nWindowHeight);
	void SetFontHeight(int nFontHeight);
	void Set(double dMinValue, double dMaxValue,int nWindowHeight,int nFontHeight);
	double * CalcFontGroup(int &nValueNumber);

protected:
	void Reset();

	double m_dMinValue;
	double m_dMaxValue;
	int m_nWindowHeight;
	int m_nFontHeight;
	int m_nValue;
	int m_nValueStepNumber;	// 15

	
	double m_dMinusValueStep[30];
	double m_dPlusValueStep[30];

	double *m_pValueGroup;
};

#endif // !defined(AFX_FONTGROUP_H__199DF6E4_FA65_11D3_AEF5_00C04FCCB957__INCLUDED_)
