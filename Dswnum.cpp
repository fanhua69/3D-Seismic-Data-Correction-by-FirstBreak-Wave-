// dswnum.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "dswnum.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDSwNum dialog


CDSwNum::CDSwNum(CWnd* pParent /*=NULL*/)
	: CDialog(CDSwNum::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDSwNum)
	m_SwathNumber = 0;
	//}}AFX_DATA_INIT
}

void CDSwNum::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDSwNum)
	DDX_Text(pDX, IDC_EDIT1, m_SwathNumber);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDSwNum, CDialog)
	//{{AFX_MSG_MAP(CDSwNum)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDSwNum message handlers
