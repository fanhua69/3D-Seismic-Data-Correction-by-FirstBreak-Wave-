// DlgAppeData.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "DlgAppeData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAppeData dialog


CDlgAppeData::CDlgAppeData(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAppeData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAppeData)
	m_nRcvLineTo = 0;
	m_nRcvPointFrom = 0;
	m_nRcvPointTo = 0;
	m_nRcvLineFrom = 0;
	//}}AFX_DATA_INIT
}


void CDlgAppeData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAppeData)
	DDX_Text(pDX, IDC_RcvLineTo, m_nRcvLineTo);
	DDX_Text(pDX, IDC_RcvPointFrom, m_nRcvPointFrom);
	DDX_Text(pDX, IDC_RcvPointTo, m_nRcvPointTo);
	DDX_Text(pDX, IDC_RcvLineFrom, m_nRcvLineFrom);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAppeData, CDialog)
	//{{AFX_MSG_MAP(CDlgAppeData)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAppeData message handlers
