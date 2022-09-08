// DlgFbkShow.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "DlgFbkShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFbkShow dialog


CDlgFbkShow::CDlgFbkShow(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFbkShow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFbkShow)
	m_nStartStation = 0;
	m_nEndStation = 0;
	m_nStep = 0;
	//}}AFX_DATA_INIT
}


void CDlgFbkShow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFbkShow)
	DDX_Text(pDX, IDC_nStartStation, m_nStartStation);
	DDX_Text(pDX, IDC_nEndStation, m_nEndStation);
	DDX_Text(pDX, IDC_nStep, m_nStep);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFbkShow, CDialog)
	//{{AFX_MSG_MAP(CDlgFbkShow)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFbkShow message handlers
