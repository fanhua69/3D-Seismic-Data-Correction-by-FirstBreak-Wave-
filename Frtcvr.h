#if !defined(AFX_FRTCVR_H__27890D21_5103_11D1_B6D1_A139AD15E33C__INCLUDED_)
#define AFX_FRTCVR_H__27890D21_5103_11D1_B6D1_A139AD15E33C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FrtCvr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFrtCvr dialog

class CFrtCvr : public CDialog
{
// Construction
public:
	CFrtCvr(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFrtCvr)
	enum { IDD = IDD_FrontCover };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	BOOL tag;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrtCvr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFrtCvr)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRTCVR_H__27890D21_5103_11D1_B6D1_A139AD15E33C__INCLUDED_)
