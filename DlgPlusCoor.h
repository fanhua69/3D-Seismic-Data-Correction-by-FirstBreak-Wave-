#if !defined(AFX_DLGPLUSCOOR_H__89F7CE61_10B9_11D4_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_DLGPLUSCOOR_H__89F7CE61_10B9_11D4_A4E4_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPlusCoor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPlusCoor dialog

class CDlgPlusCoor : public CDialog
{
// Construction
public:
	CDlgPlusCoor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPlusCoor)
	enum { IDD = IDD_DlgMakeCoor };
	int		m_nReadP190;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPlusCoor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPlusCoor)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPLUSCOOR_H__89F7CE61_10B9_11D4_A4E4_00C04FCCB957__INCLUDED_)
