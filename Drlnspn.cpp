// drlnspn.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "drlnspn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDRLNSPN dialog


CDRLNSPN::CDRLNSPN(CWnd* pParent /*=NULL*/)
	: CDialog(CDRLNSPN::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDRLNSPN)
	m_RLN1 = 0;
	m_RLN10 = 0;
	m_RLN11 = 0;
	m_RLN12 = 0;
	m_RLN13 = 0;
	m_RLN14 = 0;
	m_RLN15 = 0;
	m_RLN16 = 0;
	m_RLN17 = 0;
	m_RLN18 = 0;
	m_RLN19 = 0;
	m_RLN2 = 0;
	m_RLN20 = 0;
	m_RLN21 = 0;
	m_RLN22 = 0;
	m_RLN23 = 0;
	m_RLN24 = 0;
	m_RLN25 = 0;
	m_RLN26 = 0;
	m_RLN27 = 0;
	m_RLN28 = 0;
	m_RLN29 = 0;
	m_RLN3 = 0;
	m_RLN30 = 0;
	m_RLN31 = 0;
	m_RLN32 = 0;
	m_RLN33 = 0;
	m_RLN34 = 0;
	m_RLN35 = 0;
	m_RLN36 = 0;
	m_RLN37 = 0;
	m_RLN38 = 0;
	m_RLN39 = 0;
	m_RLN4 = 0;
	m_RLN40 = 0;
	m_RLN41 = 0;
	m_RLN42 = 0;
	m_RLN43 = 0;
	m_RLN44 = 0;
	m_RLN45 = 0;
	m_RLN46 = 0;
	m_RLN47 = 0;
	m_RLN48 = 0;
	m_RLN49 = 0;
	m_RLN5 = 0;
	m_RLN50 = 0;
	m_RLN6 = 0;
	m_RLN7 = 0;
	m_RLN8 = 0;
	m_RLN9 = 0;
	m_SPN1 = 0;
	m_SPN10 = 0;
	m_SPN11 = 0;
	m_SPN12 = 0;
	m_SPN13 = 0;
	m_SPN14 = 0;
	m_SPN15 = 0;
	m_SPN16 = 0;
	m_SPN17 = 0;
	m_SPN18 = 0;
	m_SPN19 = 0;
	m_SPN2 = 0;
	m_SPN20 = 0;
	m_SPN21 = 0;
	m_SPN22 = 0;
	m_SPN23 = 0;
	m_SPN24 = 0;
	m_SPN25 = 0;
	m_SPN26 = 0;
	m_SPN27 = 0;
	m_SPN28 = 0;
	m_SPN29 = 0;
	m_SPN3 = 0;
	m_SPN30 = 0;
	m_SPN31 = 0;
	m_SPN32 = 0;
	m_SPN33 = 0;
	m_SPN34 = 0;
	m_SPN35 = 0;
	m_SPN36 = 0;
	m_SPN37 = 0;
	m_SPN38 = 0;
	m_SPN39 = 0;
	m_SPN4 = 0;
	m_SPN40 = 0;
	m_SPN41 = 0;
	m_SPN42 = 0;
	m_SPN43 = 0;
	m_SPN44 = 0;
	m_SPN45 = 0;
	m_SPN46 = 0;
	m_SPN47 = 0;
	m_SPN48 = 0;
	m_SPN49 = 0;
	m_SPN5 = 0;
	m_SPN50 = 0;
	m_SPN6 = 0;
	m_SPN7 = 0;
	m_SPN8 = 0;
	m_SPN9 = 0;
	//}}AFX_DATA_INIT
}

void CDRLNSPN::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDRLNSPN)
	DDX_Text(pDX, IDC_RLN1, m_RLN1);
	DDX_Text(pDX, IDC_RLN10, m_RLN10);
	DDX_Text(pDX, IDC_RLN11, m_RLN11);
	DDX_Text(pDX, IDC_RLN12, m_RLN12);
	DDX_Text(pDX, IDC_RLN13, m_RLN13);
	DDX_Text(pDX, IDC_RLN14, m_RLN14);
	DDX_Text(pDX, IDC_RLN15, m_RLN15);
	DDX_Text(pDX, IDC_RLN16, m_RLN16);
	DDX_Text(pDX, IDC_RLN17, m_RLN17);
	DDX_Text(pDX, IDC_RLN18, m_RLN18);
	DDX_Text(pDX, IDC_RLN19, m_RLN19);
	DDX_Text(pDX, IDC_RLN2, m_RLN2);
	DDX_Text(pDX, IDC_RLN20, m_RLN20);
	DDX_Text(pDX, IDC_RLN21, m_RLN21);
	DDX_Text(pDX, IDC_RLN22, m_RLN22);
	DDX_Text(pDX, IDC_RLN23, m_RLN23);
	DDX_Text(pDX, IDC_RLN24, m_RLN24);
	DDX_Text(pDX, IDC_RLN25, m_RLN25);
	DDX_Text(pDX, IDC_RLN26, m_RLN26);
	DDX_Text(pDX, IDC_RLN27, m_RLN27);
	DDX_Text(pDX, IDC_RLN28, m_RLN28);
	DDX_Text(pDX, IDC_RLN29, m_RLN29);
	DDX_Text(pDX, IDC_RLN3, m_RLN3);
	DDX_Text(pDX, IDC_RLN30, m_RLN30);
	DDX_Text(pDX, IDC_RLN31, m_RLN31);
	DDX_Text(pDX, IDC_RLN32, m_RLN32);
	DDX_Text(pDX, IDC_RLN33, m_RLN33);
	DDX_Text(pDX, IDC_RLN34, m_RLN34);
	DDX_Text(pDX, IDC_RLN35, m_RLN35);
	DDX_Text(pDX, IDC_RLN36, m_RLN36);
	DDX_Text(pDX, IDC_RLN37, m_RLN37);
	DDX_Text(pDX, IDC_RLN38, m_RLN38);
	DDX_Text(pDX, IDC_RLN39, m_RLN39);
	DDX_Text(pDX, IDC_RLN4, m_RLN4);
	DDX_Text(pDX, IDC_RLN40, m_RLN40);
	DDX_Text(pDX, IDC_RLN41, m_RLN41);
	DDX_Text(pDX, IDC_RLN42, m_RLN42);
	DDX_Text(pDX, IDC_RLN43, m_RLN43);
	DDX_Text(pDX, IDC_RLN44, m_RLN44);
	DDX_Text(pDX, IDC_RLN45, m_RLN45);
	DDX_Text(pDX, IDC_RLN46, m_RLN46);
	DDX_Text(pDX, IDC_RLN47, m_RLN47);
	DDX_Text(pDX, IDC_RLN48, m_RLN48);
	DDX_Text(pDX, IDC_RLN49, m_RLN49);
	DDX_Text(pDX, IDC_RLN5, m_RLN5);
	DDX_Text(pDX, IDC_RLN50, m_RLN50);
	DDX_Text(pDX, IDC_RLN6, m_RLN6);
	DDX_Text(pDX, IDC_RLN7, m_RLN7);
	DDX_Text(pDX, IDC_RLN8, m_RLN8);
	DDX_Text(pDX, IDC_RLN9, m_RLN9);
	DDX_Text(pDX, IDC_SPN1, m_SPN1);
	DDX_Text(pDX, IDC_SPN10, m_SPN10);
	DDX_Text(pDX, IDC_SPN11, m_SPN11);
	DDX_Text(pDX, IDC_SPN12, m_SPN12);
	DDX_Text(pDX, IDC_SPN13, m_SPN13);
	DDX_Text(pDX, IDC_SPN14, m_SPN14);
	DDX_Text(pDX, IDC_SPN15, m_SPN15);
	DDX_Text(pDX, IDC_SPN16, m_SPN16);
	DDX_Text(pDX, IDC_SPN17, m_SPN17);
	DDX_Text(pDX, IDC_SPN18, m_SPN18);
	DDX_Text(pDX, IDC_SPN19, m_SPN19);
	DDX_Text(pDX, IDC_SPN2, m_SPN2);
	DDX_Text(pDX, IDC_SPN20, m_SPN20);
	DDX_Text(pDX, IDC_SPN21, m_SPN21);
	DDX_Text(pDX, IDC_SPN22, m_SPN22);
	DDX_Text(pDX, IDC_SPN23, m_SPN23);
	DDX_Text(pDX, IDC_SPN24, m_SPN24);
	DDX_Text(pDX, IDC_SPN25, m_SPN25);
	DDX_Text(pDX, IDC_SPN26, m_SPN26);
	DDX_Text(pDX, IDC_SPN27, m_SPN27);
	DDX_Text(pDX, IDC_SPN28, m_SPN28);
	DDX_Text(pDX, IDC_SPN29, m_SPN29);
	DDX_Text(pDX, IDC_SPN3, m_SPN3);
	DDX_Text(pDX, IDC_SPN30, m_SPN30);
	DDX_Text(pDX, IDC_SPN31, m_SPN31);
	DDX_Text(pDX, IDC_SPN32, m_SPN32);
	DDX_Text(pDX, IDC_SPN33, m_SPN33);
	DDX_Text(pDX, IDC_SPN34, m_SPN34);
	DDX_Text(pDX, IDC_SPN35, m_SPN35);
	DDX_Text(pDX, IDC_SPN36, m_SPN36);
	DDX_Text(pDX, IDC_SPN37, m_SPN37);
	DDX_Text(pDX, IDC_SPN38, m_SPN38);
	DDX_Text(pDX, IDC_SPN39, m_SPN39);
	DDX_Text(pDX, IDC_SPN4, m_SPN4);
	DDX_Text(pDX, IDC_SPN40, m_SPN40);
	DDX_Text(pDX, IDC_SPN41, m_SPN41);
	DDX_Text(pDX, IDC_SPN42, m_SPN42);
	DDX_Text(pDX, IDC_SPN43, m_SPN43);
	DDX_Text(pDX, IDC_SPN44, m_SPN44);
	DDX_Text(pDX, IDC_SPN45, m_SPN45);
	DDX_Text(pDX, IDC_SPN46, m_SPN46);
	DDX_Text(pDX, IDC_SPN47, m_SPN47);
	DDX_Text(pDX, IDC_SPN48, m_SPN48);
	DDX_Text(pDX, IDC_SPN49, m_SPN49);
	DDX_Text(pDX, IDC_SPN5, m_SPN5);
	DDX_Text(pDX, IDC_SPN50, m_SPN50);
	DDX_Text(pDX, IDC_SPN6, m_SPN6);
	DDX_Text(pDX, IDC_SPN7, m_SPN7);
	DDX_Text(pDX, IDC_SPN8, m_SPN8);
	DDX_Text(pDX, IDC_SPN9, m_SPN9);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDRLNSPN, CDialog)
	//{{AFX_MSG_MAP(CDRLNSPN)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDRLNSPN message handlers
