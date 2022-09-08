#if !defined(AFX_DLGRELATION_H__4C73D3E2_20D1_11D4_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_DLGRELATION_H__4C73D3E2_20D1_11D4_A4E4_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRelation.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRelation dialog

class CDlgRelation : public CDialog
{
// Construction
public:
	CDlgRelation(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRelation)
	enum { IDD = IDD_DialogRelation };
	UINT	m_nModalGroup;
	UINT	m_nShotFrom;
	UINT	m_nShotTo;
	long	m_nTimeRange;
	long	m_nTotalShotFrom;
	long	m_nTotalShotTo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRelation)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRelation)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRELATION_H__4C73D3E2_20D1_11D4_A4E4_00C04FCCB957__INCLUDED_)
