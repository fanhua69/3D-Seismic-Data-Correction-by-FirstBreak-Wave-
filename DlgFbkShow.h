#if !defined(AFX_DLGFBKSHOW_H__8C5E7849_0EC3_11D4_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_DLGFBKSHOW_H__8C5E7849_0EC3_11D4_A4E4_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgFbkShow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgFbkShow dialog

class CDlgFbkShow : public CDialog
{
// Construction
public:
	CDlgFbkShow(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgFbkShow)
	enum { IDD = IDD_FbkShow };
	long	m_nStartStation;
	long	m_nEndStation;
	long	m_nStep;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgFbkShow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgFbkShow)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGFBKSHOW_H__8C5E7849_0EC3_11D4_A4E4_00C04FCCB957__INCLUDED_)
