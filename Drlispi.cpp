// drlispi.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "drlispi.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDRLISPI dialog


CDRLISPI::CDRLISPI(CWnd* pParent /*=NULL*/)
	: CDialog(CDRLISPI::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDRLISPI)
	m_RLI1 = 0;
	m_RLI10 = 0;
	m_RLI11 = 0;
	m_RLI12 = 0;
	m_RLI13 = 0;
	m_RLI14 = 0;
	m_RLI15 = 0;
	m_RLI16 = 0;
	m_RLI17 = 0;
	m_RLI18 = 0;
	m_RLI19 = 0;
	m_RLI2 = 0;
	m_RLI20 = 0;
	m_RLI21 = 0;
	m_RLI22 = 0;
	m_RLI23 = 0;
	m_RLI24 = 0;
	m_RLI25 = 0;
	m_RLI26 = 0;
	m_RLI27 = 0;
	m_RLI28 = 0;
	m_RLI29 = 0;
	m_RLI3 = 0;
	m_RLI30 = 0;
	m_RLI31 = 0;
	m_RLI32 = 0;
	m_RLI33 = 0;
	m_RLI34 = 0;
	m_RLI35 = 0;
	m_RLI36 = 0;
	m_RLI37 = 0;
	m_RLI38 = 0;
	m_RLI39 = 0;
	m_RLI4 = 0;
	m_RLI40 = 0;
	m_RLI41 = 0;
	m_RLI42 = 0;
	m_RLI43 = 0;
	m_RLI44 = 0;
	m_RLI45 = 0;
	m_RLI46 = 0;
	m_RLI47 = 0;
	m_RLI48 = 0;
	m_RLI49 = 0;
	m_RLI5 = 0;
	m_RLI50 = 0;
	m_RLI6 = 0;
	m_RLI7 = 0;
	m_RLI8 = 0;
	m_RLI9 = 0;
	m_SPI1 = 0;
	m_SPI10 = 0;
	m_SPI11 = 0;
	m_SPI12 = 0;
	m_SPI13 = 0;
	m_SPI14 = 0;
	m_SPI15 = 0;
	m_SPI16 = 0;
	m_SPI17 = 0;
	m_SPI18 = 0;
	m_SPI19 = 0;
	m_SPI2 = 0;
	m_SPI20 = 0;
	m_SPI21 = 0;
	m_SPI22 = 0;
	m_SPI23 = 0;
	m_SPI24 = 0;
	m_SPI25 = 0;
	m_SPI26 = 0;
	m_SPI27 = 0;
	m_SPI28 = 0;
	m_SPI29 = 0;
	m_SPI3 = 0;
	m_SPI30 = 0;
	m_SPI31 = 0;
	m_SPI32 = 0;
	m_SPI33 = 0;
	m_SPI34 = 0;
	m_SPI35 = 0;
	m_SPI36 = 0;
	m_SPI37 = 0;
	m_SPI38 = 0;
	m_SPI39 = 0;
	m_SPI4 = 0;
	m_SPI40 = 0;
	m_SPI41 = 0;
	m_SPI42 = 0;
	m_SPI43 = 0;
	m_SPI44 = 0;
	m_SPI45 = 0;
	m_SPI46 = 0;
	m_SPI47 = 0;
	m_SPI48 = 0;
	m_SPI49 = 0;
	m_SPI5 = 0;
	m_SPI50 = 0;
	m_SPI6 = 0;
	m_SPI7 = 0;
	m_SPI8 = 0;
	m_SPI9 = 0;
	//}}AFX_DATA_INIT
}

void CDRLISPI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDRLISPI)
	DDX_Text(pDX, IDC_RLI1, m_RLI1);
	DDX_Text(pDX, IDC_RLI10, m_RLI10);
	DDX_Text(pDX, IDC_RLI11, m_RLI11);
	DDX_Text(pDX, IDC_RLI12, m_RLI12);
	DDX_Text(pDX, IDC_RLI13, m_RLI13);
	DDX_Text(pDX, IDC_RLI14, m_RLI14);
	DDX_Text(pDX, IDC_RLI15, m_RLI15);
	DDX_Text(pDX, IDC_RLI16, m_RLI16);
	DDX_Text(pDX, IDC_RLI17, m_RLI17);
	DDX_Text(pDX, IDC_RLI18, m_RLI18);
	DDX_Text(pDX, IDC_RLI19, m_RLI19);
	DDX_Text(pDX, IDC_RLI2, m_RLI2);
	DDX_Text(pDX, IDC_RLI20, m_RLI20);
	DDX_Text(pDX, IDC_RLI21, m_RLI21);
	DDX_Text(pDX, IDC_RLI22, m_RLI22);
	DDX_Text(pDX, IDC_RLI23, m_RLI23);
	DDX_Text(pDX, IDC_RLI24, m_RLI24);
	DDX_Text(pDX, IDC_RLI25, m_RLI25);
	DDX_Text(pDX, IDC_RLI26, m_RLI26);
	DDX_Text(pDX, IDC_RLI27, m_RLI27);
	DDX_Text(pDX, IDC_RLI28, m_RLI28);
	DDX_Text(pDX, IDC_RLI29, m_RLI29);
	DDX_Text(pDX, IDC_RLI3, m_RLI3);
	DDX_Text(pDX, IDC_RLI30, m_RLI30);
	DDX_Text(pDX, IDC_RLI31, m_RLI31);
	DDX_Text(pDX, IDC_RLI32, m_RLI32);
	DDX_Text(pDX, IDC_RLI33, m_RLI33);
	DDX_Text(pDX, IDC_RLI34, m_RLI34);
	DDX_Text(pDX, IDC_RLI35, m_RLI35);
	DDX_Text(pDX, IDC_RLI36, m_RLI36);
	DDX_Text(pDX, IDC_RLI37, m_RLI37);
	DDX_Text(pDX, IDC_RLI38, m_RLI38);
	DDX_Text(pDX, IDC_RLI39, m_RLI39);
	DDX_Text(pDX, IDC_RLI4, m_RLI4);
	DDX_Text(pDX, IDC_RLI40, m_RLI40);
	DDX_Text(pDX, IDC_RLI41, m_RLI41);
	DDX_Text(pDX, IDC_RLI42, m_RLI42);
	DDX_Text(pDX, IDC_RLI43, m_RLI43);
	DDX_Text(pDX, IDC_RLI44, m_RLI44);
	DDX_Text(pDX, IDC_RLI45, m_RLI45);
	DDX_Text(pDX, IDC_RLI46, m_RLI46);
	DDX_Text(pDX, IDC_RLI47, m_RLI47);
	DDX_Text(pDX, IDC_RLI48, m_RLI48);
	DDX_Text(pDX, IDC_RLI49, m_RLI49);
	DDX_Text(pDX, IDC_RLI5, m_RLI5);
	DDX_Text(pDX, IDC_RLI50, m_RLI50);
	DDX_Text(pDX, IDC_RLI6, m_RLI6);
	DDX_Text(pDX, IDC_RLI7, m_RLI7);
	DDX_Text(pDX, IDC_RLI8, m_RLI8);
	DDX_Text(pDX, IDC_RLI9, m_RLI9);
	DDX_Text(pDX, IDC_SPI1, m_SPI1);
	DDX_Text(pDX, IDC_SPI10, m_SPI10);
	DDX_Text(pDX, IDC_SPI11, m_SPI11);
	DDX_Text(pDX, IDC_SPI12, m_SPI12);
	DDX_Text(pDX, IDC_SPI13, m_SPI13);
	DDX_Text(pDX, IDC_SPI14, m_SPI14);
	DDX_Text(pDX, IDC_SPI15, m_SPI15);
	DDX_Text(pDX, IDC_SPI16, m_SPI16);
	DDX_Text(pDX, IDC_SPI17, m_SPI17);
	DDX_Text(pDX, IDC_SPI18, m_SPI18);
	DDX_Text(pDX, IDC_SPI19, m_SPI19);
	DDX_Text(pDX, IDC_SPI2, m_SPI2);
	DDX_Text(pDX, IDC_SPI20, m_SPI20);
	DDX_Text(pDX, IDC_SPI21, m_SPI21);
	DDX_Text(pDX, IDC_SPI22, m_SPI22);
	DDX_Text(pDX, IDC_SPI23, m_SPI23);
	DDX_Text(pDX, IDC_SPI24, m_SPI24);
	DDX_Text(pDX, IDC_SPI25, m_SPI25);
	DDX_Text(pDX, IDC_SPI26, m_SPI26);
	DDX_Text(pDX, IDC_SPI27, m_SPI27);
	DDX_Text(pDX, IDC_SPI28, m_SPI28);
	DDX_Text(pDX, IDC_SPI29, m_SPI29);
	DDX_Text(pDX, IDC_SPI3, m_SPI3);
	DDX_Text(pDX, IDC_SPI30, m_SPI30);
	DDX_Text(pDX, IDC_SPI31, m_SPI31);
	DDX_Text(pDX, IDC_SPI32, m_SPI32);
	DDX_Text(pDX, IDC_SPI33, m_SPI33);
	DDX_Text(pDX, IDC_SPI34, m_SPI34);
	DDX_Text(pDX, IDC_SPI35, m_SPI35);
	DDX_Text(pDX, IDC_SPI36, m_SPI36);
	DDX_Text(pDX, IDC_SPI37, m_SPI37);
	DDX_Text(pDX, IDC_SPI38, m_SPI38);
	DDX_Text(pDX, IDC_SPI39, m_SPI39);
	DDX_Text(pDX, IDC_SPI4, m_SPI4);
	DDX_Text(pDX, IDC_SPI40, m_SPI40);
	DDX_Text(pDX, IDC_SPI41, m_SPI41);
	DDX_Text(pDX, IDC_SPI42, m_SPI42);
	DDX_Text(pDX, IDC_SPI43, m_SPI43);
	DDX_Text(pDX, IDC_SPI44, m_SPI44);
	DDX_Text(pDX, IDC_SPI45, m_SPI45);
	DDX_Text(pDX, IDC_SPI46, m_SPI46);
	DDX_Text(pDX, IDC_SPI47, m_SPI47);
	DDX_Text(pDX, IDC_SPI48, m_SPI48);
	DDX_Text(pDX, IDC_SPI49, m_SPI49);
	DDX_Text(pDX, IDC_SPI5, m_SPI5);
	DDX_Text(pDX, IDC_SPI50, m_SPI50);
	DDX_Text(pDX, IDC_SPI6, m_SPI6);
	DDX_Text(pDX, IDC_SPI7, m_SPI7);
	DDX_Text(pDX, IDC_SPI8, m_SPI8);
	DDX_Text(pDX, IDC_SPI9, m_SPI9);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDRLISPI, CDialog)
	//{{AFX_MSG_MAP(CDRLISPI)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDRLISPI message handlers
