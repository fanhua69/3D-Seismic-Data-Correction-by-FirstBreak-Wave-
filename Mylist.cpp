// MyList.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "MyList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyList dialog


CMyList::CMyList(CWnd* pParent /*=NULL*/)
	: CDialog(CMyList::IDD, pParent)
{
	Selected=-1;    // Chosen Nothing!
	//{{AFX_DATA_INIT(CMyList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMyList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyList)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyList, CDialog)
	//{{AFX_MSG_MAP(CMyList)
	ON_LBN_DBLCLK(IDC_ListBox, OnDblclkListBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyList message handlers

BOOL CMyList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetWindowText(CaptainText);

	CWnd *pWnd=(CWnd *)GetDlgItem(IDC_ListContent);
	pWnd->SetWindowText(ContentText);  

	CListBox* pLB=(CListBox*) GetDlgItem(IDC_ListBox);
	char a[30];
	for(int i=0;i<DataNumber;i++){
		sprintf(a,"%ld",Data[i]);
		pLB->InsertString(-1,a);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CMyList::OnDblclkListBox() 
{
	// Added by FAN , if selected a item then quit.
	CListBox* pLB=(CListBox*) GetDlgItem(IDC_ListBox);
	Selected=pLB->GetCurSel();
	CDialog::OnOK();
}


void CMyList::OnOK() 
{
	// TODO: Add extra validation here
	CListBox* pLB=(CListBox*) GetDlgItem(IDC_ListBox);
	Selected=pLB->GetCurSel();
	
	CDialog::OnOK();
}

void CMyList::OnCancel() 
{
	Selected=-1;
	CDialog::OnCancel();
}
