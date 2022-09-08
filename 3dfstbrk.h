// 3dfstbrk.h : main header file for the 3DFSTBRK application
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "fbkview.h"
#include "mainfrm.h"

/////////////////////////////////////////////////////////////////////////////
// C3dfstbrkApp:
// See 3dfstbrk.cpp for the implementation of this class
//

class C3dfstbrkApp : public CWinApp
{
public:
	CMainFrame* m_pMainFrame;
	CMultiDocTemplate* m_pFbkTemplate;
	CMultiDocTemplate* m_pTTemplate;
	CMultiDocTemplate* m_pSPPTemplate;
	CMultiDocTemplate* m_pTopoTemplate ;
	CMultiDocTemplate* m_pFbkPureTemplate ;
	CMultiDocTemplate* m_pControlTemplate;



	// p190
	CMultiDocTemplate* m_pTemplateP190;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	C3dfstbrkApp();
	
	void CreateMainFrame() ;
	bool CreateFrame(CMultiDocTemplate* ,CString );
	
// Overrides
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

	//{{AFX_MSG(C3dfstbrkApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
