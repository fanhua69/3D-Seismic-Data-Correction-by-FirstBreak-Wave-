// mainfrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////
#ifndef CMainFrame_H
#define CMainFrame_H
class CMainFrame : public CMDIFrameWnd
{
public: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
// Operations
public:

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar    m_wndZoomViewToolBar;



// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFileMakeFromT();
	afx_msg void OnFileMakeFromDBF();
	afx_msg void OnFirstBreakSimulate();
	afx_msg void OnMakeGKHFBD() ;
	afx_msg void OnFstBrkCheck();
	afx_msg void OnFbkIntegrate();
	afx_msg void OnUnformFileDeal(); 
	afx_msg void OnParameterSurveysystem() ;
	afx_msg void OnParameterSwath() ;
	afx_msg void OnMainCheckT();
	afx_msg void OnEquaReso();
	afx_msg void OnEquaMake() ;	
	afx_msg void OnEquationMakeNet();
	afx_msg void OnEquationControlByNet();
	afx_msg void OnOutputAppend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif
/////////////////////////////////////////////////////////////////////////////
