// SPPMaker.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "SPPMaker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSPPMaker dialog


CSPPMaker::CSPPMaker(CWnd* pParent /*=NULL*/)
	: CDialog(CSPPMaker::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSPPMaker)
	m_FileNumberFile = _T("");
	m_OffsetFile = _T("");
	m_PlusCor = FALSE;
	m_PlusFileNumber = FALSE;
	m_PlusOffset = FALSE;
	m_PlusPh = FALSE;
	m_PlusTrace = FALSE;
	m_StaticDataFile = _T("");
	m_TraceFile = _T("");
	m_PhByProgram = -1;
	m_PhFile = _T("");
	m_KFFile = _T("");
	m_PlusKF = FALSE;
	//}}AFX_DATA_INIT
}


void CSPPMaker::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSPPMaker)
	DDX_Text(pDX, IDC_FileNumberFile, m_FileNumberFile);
	DDV_MaxChars(pDX, m_FileNumberFile, 13);
	DDX_Text(pDX, IDC_OffsetFile, m_OffsetFile);
	DDV_MaxChars(pDX, m_OffsetFile, 13);
	DDX_Check(pDX, IDC_PlusCor, m_PlusCor);
	DDX_Check(pDX, IDC_PlusFileNumber, m_PlusFileNumber);
	DDX_Check(pDX, IDC_PlusOffset, m_PlusOffset);
	DDX_Check(pDX, IDC_PlusPh, m_PlusPh);
	DDX_Check(pDX, IDC_PlusTrace, m_PlusTrace);
	DDX_Text(pDX, IDC_StaticDataFile, m_StaticDataFile);
	DDV_MaxChars(pDX, m_StaticDataFile, 13);
	DDX_Text(pDX, IDC_TraceFile, m_TraceFile);
	DDV_MaxChars(pDX, m_TraceFile, 13);
	DDX_Radio(pDX, IDC_PhByProgram, m_PhByProgram);
	DDX_Text(pDX, IDC_PhFile, m_PhFile);
	DDV_MaxChars(pDX, m_PhFile, 13);
	DDX_Text(pDX, IDC_KFFile, m_KFFile);
	DDX_Check(pDX, IDC_PlusKF, m_PlusKF);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSPPMaker, CDialog)
	//{{AFX_MSG_MAP(CSPPMaker)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSPPMaker message handlers
