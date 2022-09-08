// 3dfstbrk.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "mainfrm.h"
#include "ChildFrm.h"
#include "TopoNetDoc.h"
#include "TopoNetView.h"
#include "frtcvr.h"
#include "FbkDoc.h"
#include "TDoc.h"
#include "TView.h"
#include "SPPDoc.h"
#include "SPPView.h"
#include "fh.h"

#include "ControlDoc.h"
#include "ControlView.h"


#include "FbkPureView.h"
#include "FbkPureDoc.h"


#include "P190.h"
#include "p190doc.h"
#include "p190view.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C3dfstbrkApp

BEGIN_MESSAGE_MAP(C3dfstbrkApp, CWinApp)
	//{{AFX_MSG_MAP(C3dfstbrkApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

// toolbar buttons - IDs are command buttons
static UINT BASED_CODE buttons[] =
{
	// same order as in the bitmap 'toolbar.bmp'
	ID_FILE_NEW,
	ID_FILE_OPEN,
	ID_FILE_SAVE,
		ID_SEPARATOR,
	ID_PARAMETER_SURVEYSYSTEM,
	ID_PARAMETER_PARAMETEROFONESWATH,
	IDM_UnformFileDeal,
	ID_MainCheckT,
		ID_SEPARATOR,
	ID_EDIT_CUT,
	ID_EDIT_COPY,
	ID_EDIT_PASTE,
		ID_SEPARATOR,
	ID_FILE_PRINT,
	ID_APP_ABOUT,
	ID_CONTEXT_HELP,
};
/////////////////////////////////////////////////////////////////////////////
// C3dfstbrkApp construction

C3dfstbrkApp::C3dfstbrkApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only C3dfstbrkApp object

C3dfstbrkApp NEAR theApp;
											//
/////////////////////////////////////////////////////////////////////////////
// C3dfstbrkApp initialization

BOOL C3dfstbrkApp::InitInstance()
{
	AfxEnableControlContainer();  // to enable the CGridCtrl 


	// front cover:
	CFrtCvr dlg;
	dlg.DoModal();
	
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	SetDialogBkColor();        // Set dialog background color to gray
	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	  	   
	m_pTopoTemplate = new CMultiDocTemplate(
		IDR_TopoNet,
		RUNTIME_CLASS(CTopoNetDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTopoNetView));
	AddDocTemplate(m_pTopoTemplate );

	m_pFbkTemplate = new CMultiDocTemplate(
		IDR_MenuFstBrk,
		RUNTIME_CLASS(CFbkDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CFbkView));
	AddDocTemplate(m_pFbkTemplate);
	
	m_pTTemplate = new CMultiDocTemplate(
		IDR_MenuStaticCorrection,
		RUNTIME_CLASS(CTDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CTView));
	AddDocTemplate(m_pTTemplate);

	m_pSPPTemplate = new CMultiDocTemplate(
		IDR_MenuSPP,
		RUNTIME_CLASS(CSPPDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CSPPView));
	AddDocTemplate(m_pSPPTemplate);
			
	m_pTemplateP190= new CMultiDocTemplate(
		IDR_MenuP190,
		RUNTIME_CLASS(CP190Doc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CP190View));
	AddDocTemplate(m_pTemplateP190);

	m_pFbkPureTemplate = new CMultiDocTemplate(
		IDR_MenuFbkPure,
		RUNTIME_CLASS(CFbkPureDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CFbkPureView));
	AddDocTemplate(m_pFbkPureTemplate );


	// Control points:
	m_pControlTemplate = new CMultiDocTemplate(
		IDR_Control,
		RUNTIME_CLASS(CControlDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CControlView));
	AddDocTemplate(m_pControlTemplate );


	// create main MDI Frame window	
	m_pMainFrame = new CMainFrame;
	if (!m_pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = m_pMainFrame;

	m_pMainWnd->ShowWindow(m_nCmdShow);

	if (m_lpCmdLine[0] == '\0')
	{
		// TODO: add command line processing here
		// create a new (empty) document
	}
	else if(m_lpCmdLine[0]=='-'||m_lpCmdLine[0]=='/'&&
		(m_lpCmdLine[1]=='e'||m_lpCmdLine[1]=='E'))
	{
		OnFileNew();
	}
	else{
		OpenDocumentFile(m_lpCmdLine);
	} 

    return TRUE;
}

int C3dfstbrkApp::ExitInstance()
{
	// The document template should not be deleted here manually,
	// because they will be deleted by CWinApp::Exitinstance()!!!!!
	// This error cost one total night and one half morning!!!!!!!
	return CWinApp::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void C3dfstbrkApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// C3dfstbrkApp 命令处理函数：
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL C3dfstbrkApp::PreTranslateMessage(MSG* pMsg)
{
	return CWinApp::PreTranslateMessage(pMsg);
}

void C3dfstbrkApp::CreateMainFrame() 
{
	// 返回主工具条.
	m_pMainFrame->m_wndToolBar.LoadToolBar(IDR_MAINFRAME);
	m_pMainFrame->m_wndToolBar.SetButtons(buttons,sizeof(buttons)/sizeof(UINT));
}
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

