// DlgPlusCoor.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "DlgPlusCoor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPlusCoor dialog


CDlgPlusCoor::CDlgPlusCoor(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPlusCoor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPlusCoor)
	m_nReadP190 = 0;
	//}}AFX_DATA_INIT
}


void CDlgPlusCoor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPlusCoor)
	DDX_Radio(pDX, IDC_ReadP190, m_nReadP190);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPlusCoor, CDialog)
	//{{AFX_MSG_MAP(CDlgPlusCoor)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPlusCoor message handlers
