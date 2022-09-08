// DpgNum.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "DpgNum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDpgNum dialog


CDpgNum::CDpgNum(CWnd* pParent /*=NULL*/)
	: CDialog(CDpgNum::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDpgNum)
	m_PageNumber = 0;
	//}}AFX_DATA_INIT
}


void CDpgNum::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDpgNum)
	DDX_Text(pDX, IDC_PageNumber, m_PageNumber);
	DDV_MinMaxUInt(pDX, m_PageNumber, 1, 100000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDpgNum, CDialog)
	//{{AFX_MSG_MAP(CDpgNum)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDpgNum message handlers
