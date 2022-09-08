#if !defined(AFX_SPPMAKER_H__83E93321_385C_11D1_AF86_444553540000__INCLUDED_)
#define AFX_SPPMAKER_H__83E93321_385C_11D1_AF86_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SPPMaker.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSPPMaker dialog

class CSPPMaker : public CDialog
{
// Construction
public:
	CSPPMaker(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSPPMaker)
	enum { IDD = IDD_SPPFileMaker };
	CString	m_FileNumberFile;
	CString	m_OffsetFile;
	BOOL	m_PlusCor;
	BOOL	m_PlusFileNumber;
	BOOL	m_PlusOffset;
	BOOL	m_PlusPh;
	BOOL	m_PlusTrace;
	CString	m_StaticDataFile;
	CString	m_TraceFile;
	int		m_PhByProgram;
	CString	m_PhFile;
	CString	m_KFFile;
	BOOL	m_PlusKF;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSPPMaker)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSPPMaker)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPPMAKER_H__83E93321_385C_11D1_AF86_444553540000__INCLUDED_)
