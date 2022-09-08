#if !defined(AFX_DLGAPPEDATA_H__BADC7E61_1797_11D4_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_DLGAPPEDATA_H__BADC7E61_1797_11D4_A4E4_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAppeData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAppeData dialog

class CDlgAppeData : public CDialog
{
// Construction
public:
	CDlgAppeData(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAppeData)
	enum { IDD = IDD_AppendData };
	long	m_nRcvLineTo;
	long	m_nRcvPointFrom;
	long	m_nRcvPointTo;
	long	m_nRcvLineFrom;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAppeData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAppeData)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGAPPEDATA_H__BADC7E61_1797_11D4_A4E4_00C04FCCB957__INCLUDED_)
