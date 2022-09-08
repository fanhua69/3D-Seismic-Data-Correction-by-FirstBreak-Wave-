// DlgRelation.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "DlgRelation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgRelation dialog


CDlgRelation::CDlgRelation(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgRelation::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgRelation)
	m_nModalGroup = 0;
	m_nShotFrom = 0;
	m_nShotTo = 0;
	m_nTimeRange = 0;
	m_nTotalShotFrom = 0;
	m_nTotalShotTo = 0;
	//}}AFX_DATA_INIT
}


void CDlgRelation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRelation)
	DDX_Text(pDX, IDC_nModalGroup, m_nModalGroup);
	DDX_Text(pDX, IDC_nShotFrom, m_nShotFrom);
	DDX_Text(pDX, IDC_nShotTo, m_nShotTo);
	DDX_Text(pDX, IDC_nTimeRange, m_nTimeRange);
	DDX_Text(pDX, IDC_nTotalShotFrom, m_nTotalShotFrom);
	DDX_Text(pDX, IDC_nTotalShotTo, m_nTotalShotTo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgRelation, CDialog)
	//{{AFX_MSG_MAP(CDlgRelation)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRelation message handlers
