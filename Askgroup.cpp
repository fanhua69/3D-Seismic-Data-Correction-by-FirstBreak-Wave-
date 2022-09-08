// AskGroup.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "AskGroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAskGroup dialog


CAskGroup::CAskGroup(CWnd* pParent /*=NULL*/)
	: CDialog(CAskGroup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAskGroup)
	m_BN = 0;
	m_EN = 0;
	//}}AFX_DATA_INIT
}


void CAskGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAskGroup)
	DDX_Text(pDX, IDC_BN, m_BN);
	DDV_MinMaxUInt(pDX, m_BN, 1, 10000000);
	DDX_Text(pDX, IDC_EN, m_EN);
	DDV_MinMaxUInt(pDX, m_EN, 1, 100000000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAskGroup, CDialog)
	//{{AFX_MSG_MAP(CAskGroup)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAskGroup message handlers
