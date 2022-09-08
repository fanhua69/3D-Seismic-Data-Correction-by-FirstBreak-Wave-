#if !defined(AFX_COLORDLG_H__50E2A745_F9E3_11D3_AEF5_00C04FCCB957__INCLUDED_)
#define AFX_COLORDLG_H__50E2A745_F9E3_11D3_AEF5_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog

class CColorDlg : public CDialog
{
// Construction
public:
	CColorDlg(CWnd* pParent = NULL);   // standard constructor
	~CColorDlg();

	BOOL Create(CWnd *pParent=NULL);
	BOOL DestroyWindow(); 

	COLORREF GetColor(double dValue);
	void SetRange(double dMinValue,double dMaxValue);


// Dialog Data
	//{{AFX_DATA(CColorDlg)
	enum { IDD = IDD_DlgColorModal };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	double m_dMinValue;
	double m_dMaxValue;
	double m_dScale;

	int m_nColorNumber; // =512;

	COLORREF m_nColor[512];



	// Generated message map functions
	//{{AFX_MSG(CColorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnCancel();
	afx_msg void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
    
    BOOL m_bParentDisabled;
    void ReEnableParent();


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORDLG_H__50E2A745_F9E3_11D3_AEF5_00C04FCCB957__INCLUDED_)
