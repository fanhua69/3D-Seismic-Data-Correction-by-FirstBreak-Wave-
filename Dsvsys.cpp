// dsvsys.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "dsvsys.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDSvSys dialog


CDSvSys::CDSvSys(CWnd* pParent /*=NULL*/)
	: CDialog(CDSvSys::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDSvSys)
	m_Area = "";
	m_Crew = "";
	m_GapOfSmallNumber = 0;
	m_GapOfBigNumber = 0;
	m_GroupInterval = 0;
	m_GroupNumberOfBigNumber = 0;
	m_GroupNumberOfSmallNumber = 0;
	m_RecieveLineNumber = 0;
	m_ShotLineInterval = 0;
	m_ShotPointNumber = 0;
	m_FirstShotPointPosition = 0;
	//}}AFX_DATA_INIT
}

void CDSvSys::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDSvSys)
	DDX_Text(pDX, IDC_Area, m_Area);
	DDX_Text(pDX, IDC_Crew, m_Crew);
	DDX_Text(pDX, IDC_GapOfSmallNumber, m_GapOfSmallNumber);
	DDV_MinMaxLong(pDX, m_GapOfSmallNumber, 0, 5000);
	DDX_Text(pDX, IDC_GapOfBigNumber, m_GapOfBigNumber);
	DDV_MinMaxLong(pDX, m_GapOfBigNumber, 0, 5000);
	DDX_Text(pDX, IDC_GroupInterval, m_GroupInterval);
	DDV_MinMaxLong(pDX, m_GroupInterval, 1, 500);
	DDX_Text(pDX, IDC_GroupNumberOfBigNumber, m_GroupNumberOfBigNumber);
	DDV_MinMaxLong(pDX, m_GroupNumberOfBigNumber, 0, 5000);
	DDX_Text(pDX, IDC_GroupNumberOfSmallNumber, m_GroupNumberOfSmallNumber);
	DDV_MinMaxLong(pDX, m_GroupNumberOfSmallNumber, 0, 5000);
	DDX_Text(pDX, IDC_RecieveLineNumber, m_RecieveLineNumber);
	DDV_MinMaxLong(pDX, m_RecieveLineNumber, 1, 51);
	DDX_Text(pDX, IDC_ShotLineInterval, m_ShotLineInterval);
	DDV_MinMaxLong(pDX, m_ShotLineInterval, 1, 1000);
	DDX_Text(pDX, IDC_ShotPointNumber, m_ShotPointNumber);
	DDV_MinMaxLong(pDX, m_ShotPointNumber, 1, 51);
	DDX_Text(pDX, IDC_FirstShotPointPosition, m_FirstShotPointPosition);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDSvSys, CDialog)
	//{{AFX_MSG_MAP(CDSvSys)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
