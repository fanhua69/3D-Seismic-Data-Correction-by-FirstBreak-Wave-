// cdswpar.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "cdswpar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// cdswpar dialog


cdswpar::cdswpar(CWnd* pParent /*=NULL*/)
	: CDialog(cdswpar::IDD, pParent)
{
	//{{AFX_DATA_INIT(cdswpar)
	m_Distance2 = 0;
	m_Distance1 = 0;
	m_Distance3 = 0;
	m_Distance4 = 0;
	m_FirstRecievePointNumber = 0;
	m_FoldTime = 0;
	m_InitialVelocity = 0;
	m_ShotLineFrom = 0;
	m_ShotLineTo = 0;
	//}}AFX_DATA_INIT
}


void cdswpar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cdswpar)
	DDX_Text(pDX, IDC_Distance2, m_Distance2);
	DDV_MinMaxUInt(pDX, m_Distance2, 0, 5000);
	DDX_Text(pDX, IDC_Distance1, m_Distance1);
	DDV_MinMaxUInt(pDX, m_Distance1, 0, 5000);
	DDX_Text(pDX, IDC_Distance3, m_Distance3);
	DDV_MinMaxUInt(pDX, m_Distance3, 0, 5000);
	DDX_Text(pDX, IDC_Distance4, m_Distance4);
	DDV_MinMaxUInt(pDX, m_Distance4, 0, 5000);
	DDX_Text(pDX, IDC_FirstRecievePointNumber, m_FirstRecievePointNumber);
	DDV_MinMaxUInt(pDX, m_FirstRecievePointNumber, 0, 999);
	DDX_Text(pDX, IDC_FoldTime, m_FoldTime);
	DDV_MinMaxUInt(pDX, m_FoldTime, 1, 3);
	DDX_Text(pDX, IDC_InitialVelocity, m_InitialVelocity);
	DDV_MinMaxUInt(pDX, m_InitialVelocity, 1000, 3000);
	DDX_Text(pDX, IDC_ShotLineFrom, m_ShotLineFrom);
	DDV_MinMaxUInt(pDX, m_ShotLineFrom, 0, 999);
	DDX_Text(pDX, IDC_ShotLineTo, m_ShotLineTo);
	DDV_MinMaxUInt(pDX, m_ShotLineTo, 0, 999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(cdswpar, CDialog)
	//{{AFX_MSG_MAP(cdswpar)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cdswpar message handlers
