// mainfrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "afxcmn.h"
#include "mainfrm.h"
#include "svsys.h"
#include "swathpar.h"
#include "spp.h"
#include "MakeEqua.h"
#include "LWCtl.h"
#include "Dswnum.h"
#include "fh.h"
#include "equation.h"
#include "MemEqua.h"
#include "FileFace.h"
#include "FbkDoc.h"
#include "DlgAppeData.h"
#include "FbkInFile.h"
#include "OutFbkFile.h"
#include "SndFile.h"
//#include "ZoomView.h"



#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_MAKEINITIATEFILE_FROM52CHARACTARTEXTFILE, OnFileMakeFromT)
	ON_COMMAND(ID_FILE_MAKEINITIATEFILE_FROMDBFFILE, OnFileMakeFromDBF)
	ON_COMMAND(ID_FirstBreakSimulate, OnFirstBreakSimulate)
	ON_COMMAND(ID_MakeGKHFBD,OnMakeGKHFBD)
	ON_COMMAND(ID_FstBrkCheck, OnFstBrkCheck)
	ON_COMMAND(ID_Fbk_Integrate, OnFbkIntegrate)
	ON_COMMAND(IDM_UnformFileDeal,OnUnformFileDeal)
	ON_COMMAND(ID_PARAMETER_SURVEYSYSTEM,OnParameterSurveysystem)
	ON_COMMAND(ID_PARAMETER_PARAMETEROFONESWATH,OnParameterSwath)	
	ON_COMMAND(ID_MainCheckT,OnMainCheckT)
	ON_COMMAND(ID_EQUATION_RESOLVEEQUATION,OnEquaReso)
	ON_COMMAND(ID_EQUATION_MAKEEQUATION,OnEquaMake)
	ON_COMMAND(ID_Equation_MakeNet, OnEquationMakeNet)
	ON_COMMAND(ID_Equation_ControlByNet, OnEquationControlByNet)
	ON_COMMAND(ID_Output_Append, OnOutputAppend)
	//}}AFX_MSG_MAP
	// Global help commands
	ON_COMMAND(ID_HELP_INDEX, CMDIFrameWnd::OnHelpIndex)
	ON_COMMAND(ID_HELP_USING, CMDIFrameWnd::OnHelpUsing)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpIndex)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// arrays of IDs used to initialize control bars

static UINT BASED_CODE indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

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
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	UINT n=IDR_FHZoomViewToolBar;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME) ||
		!m_wndToolBar.SetButtons(buttons,
		  sizeof(buttons)/sizeof(UINT)))
	{
		TRACE("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	UINT n=IDR_FHZoomViewToolBar;
	if (!m_wndZoomViewToolBar.Create(this) ||
		!m_wndZoomViewToolBar.LoadToolBar(IDR_FHZoomViewToolBar))
	{
		TRACE("Failed to create toolbar\n");
		return -1;      // fail to create
	} 

	CToolBarCtrl *pBarCtrl=&(m_wndToolBar.GetToolBarCtrl());
	pBarCtrl->AddBitmap(9,IDR_MenuFirstBreak);
	pBarCtrl->AddBitmap(1,IDR_MenuT);
	pBarCtrl->AddBitmap(1,IDR_MenuSPP);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE("Failed to create status bar\n");
		return -1;      // fail to create
	}


	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndZoomViewToolBar.SetBarStyle(m_wndZoomViewToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);


	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndZoomViewToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndZoomViewToolBar.SetButtonStyle(2,TBBS_CHECKBOX);

	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndZoomViewToolBar);	

	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers






void CMainFrame::OnFstBrkCheck() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg2(true);
	dlg2.m_ofn.lpstrFilter="First Break File(*.fbk)\0*.fbk";
	dlg2.m_ofn.lpstrTitle="Give me the fbk file name";
	dlg2.m_ofn.lpstrDefExt=".fbk";
	
	if(dlg2.DoModal()==IDCANCEL)return;
	CString sFbkFile=dlg2.GetPathName();

	CFileDialog dlg(false);
	dlg.m_ofn.lpstrFilter="Target text file (*.txt)\0*.txt";
	dlg.m_ofn.lpstrTitle="Give me the fbk infomation file";
	dlg.m_ofn.lpstrDefExt=".txt";
	
	if(dlg.DoModal()==IDCANCEL)return;
	CString sTarget=dlg.GetPathName();
	
	
	CFbkInFile fbk;
	if(!fbk.Open (sFbkFile))return;

	fbk.ShowMsg(sTarget);
}

void CMainFrame::OnFbkIntegrate() 
{
	// TODO: Add your command handler code here
	IntegrateFstBrk();
	
}

void CMainFrame::OnUnformFileDeal() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg(true);
	dlg.m_ofn.lpstrFilter="Unform file name(*.jb)\0*.jb";
	dlg.m_ofn.lpstrTitle="Give me the unform file name";
	dlg.m_ofn.lpstrDefExt=".jb";
	
	if(dlg.DoModal()==IDCANCEL)return;
	CString sFile=dlg.GetPathName();

	C3dfstbrkApp *pApp=(C3dfstbrkApp *)AfxGetApp();
	pApp->m_pFbkTemplate->OpenDocumentFile (sFile);
}

void CMainFrame::OnParameterSurveysystem() 
{
	// TODO: Add your command handler code here
	SvSys ss;
	if(!ss.AskCommonParameter())return;
	ss.AskRLISPI();
}

void CMainFrame::OnParameterSwath() 
{
	// TODO: Add your command handler code here
	int swath=AskSwathNumber();
	if(swath==-1)return;
	SwathParameter sp;
	sp.Set(swath);
	if(!sp.AskCommonParameter())return;
	sp.AskRLNSPN();	
}


void CMainFrame::OnMainCheckT()
{
	CFileDialog dlg(true);
	dlg.m_ofn.lpstrFilter="Shot Static Correction Data(*.mis)\0*.mis";
	dlg.m_ofn.lpstrTitle="Middling Shot Static Data";
	dlg.m_ofn.lpstrDefExt=".mis";
	
	if(dlg.DoModal()==IDCANCEL)return;
	CString sFile=dlg.GetPathName();

	C3dfstbrkApp *pApp=(C3dfstbrkApp *)AfxGetApp();
	pApp->m_pTTemplate->OpenDocumentFile (sFile);
} 

void CMainFrame::OnEquaMake() 
{
	int swath=AskSwathNumber();
	if(swath==-1)return;

	MakeEquation me;
	if(!me.Set(swath))return;

	me.MakeEquationOnPhyPos();
}

void CMainFrame::OnEquaReso() 
{
	int swath=AskSwathNumber();
	if(swath==-1)return;

	Equation e;
	if(!e.Set(swath))return;

	CMemEquation equation;
	if(!equation.Use (e.m_FileRS ,e.m_Fileb)){
		AfxMessageBox("Can not use the equation file :"+e.m_FileRS +e.m_Fileb);
		return ;
	}

	equation.Resolve ();

	double *pX=equation.GetX();

	if(!e.SaveMidStaticData (pX))return ;

	
}
void CMainFrame::OnFileMakeFromT() 
{
	// TODO: Add your command handler code here
	CDC *ClientDC=GetDC();
	int swath=AskSwathNumber();
	if(swath==-1)return;
	FileInterface fi;
	if(!fi.Set(swath))return;
	fi.TurnInFromT(ClientDC);	
}

void CMainFrame::OnFileMakeFromDBF() 
{
	// TODO: Add your command handler code here
	CDC *ClientDC=GetDC();
	int swath=AskSwathNumber();
	if(swath==-1)return;
	FileInterface fi;
	if(!fi.Set(swath))return;
	fi.TurnInFromDBF(ClientDC);
}

void CMainFrame::OnFirstBreakSimulate() 
{
	 CFbkDoc fb;
	 fb.MakeFBD(0);	
}

void CMainFrame::OnMakeGKHFBD() 
{
	// TODO: Add your command handler code here
	 CFbkDoc fb;
	 fb.MakeGKHFBD();		
}


void CMainFrame::OnEquationMakeNet() 
{
	LongWaveControl wc;
	wc.CreateNet ();
}

void CMainFrame::OnEquationControlByNet() 
{
	LongWaveControl wc;
	wc.ControlByNet ();	
}

void CMainFrame::OnOutputAppend() 
{
	// TODO: Add your command handler code here
	COutFbkFile fileOut;
	if(!fileOut.ReadRcv ())return;
	if(!fileOut.ReadShot())return;

	CSndFile fileSnd;
	if(!fileSnd.ReadRcv ())return;
	if(!fileSnd.ReadShot())return;

	CDlgAppeData dlg;
	dlg.m_nRcvLineFrom =106;
	dlg.m_nRcvLineTo =115;
	dlg.m_nRcvPointFrom =393;
	dlg.m_nRcvPointTo =792;



	if(dlg.DoModal()==IDCANCEL)return;
	long nRcvLineFrom=dlg.m_nRcvLineFrom ;
	long nRcvLineTo=dlg.m_nRcvLineTo ;
	long nRcvPointFrom=dlg.m_nRcvPointFrom ;
	long nRcvPointTo=dlg.m_nRcvPointTo;

	//
	COutFbkFile fileTarget;
	long nRow=nRcvLineTo-nRcvLineFrom+1;
	long nCol=nRcvPointTo-nRcvPointFrom+1;
	fileTarget.SetRcvNumber (nRow*nCol);

	long Ph;
	long n,nData=0;
	bool bStop=false;
	for(long i=nRcvLineFrom;i<=nRcvLineTo;i++){
		for(long j=nRcvPointFrom;j<=nRcvPointTo;j++){
			Ph=i*1000+j;
			n=fileOut.SearchRcvStation (Ph);

			if(n!=-1){ 
				fileTarget.m_pRcvData [nData]=fileOut.m_pRcvData [n];
				nData++;
				continue;
			}

			n=fileSnd.SearchRcvStation (Ph);

			if(n!=-1){
				fileTarget.m_pRcvData [nData]=fileSnd.m_pRcvData [n];
				nData++;
				continue;
			}

			if(AfxMessageBox("Append Data Error: Can not find "+STR(Ph)+" , in the output fbk file and the other kind of file, continue?",MB_YESNO)==IDNO){
				bStop=true;
				break;
			}
		}
		if(bStop)break;
	}

	fileTarget.WriteRcv ();
}



			






	
