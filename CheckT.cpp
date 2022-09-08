// CheckT.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "CheckT.h"
#include "MyList.h"
#include "fh.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckT dialog


CCheckT::CCheckT(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckT::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckT)
	m_RecieveName = 0;
	m_ShotName = 0;
	m_bCheckShotOrRcv = -1;
	m_bDrawOnShotOrRcv = -1;
	//}}AFX_DATA_INIT

	m_RecieveNameList=new long[100];
	m_ShotPointNameList=new long[100];
	
	m_RecieveNameNumber=0;
	m_ShotNameNumber=0;	
	m_ShotLineFrom=0;
	m_ShotLineTo=0;

}
CCheckT::~CCheckT()
{
	delete m_RecieveNameList;
	delete m_ShotPointNameList;
}


void CCheckT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckT)
	DDX_Control(pDX, IDC_ListShot, m_ListShot);
	DDX_Control(pDX, IDC_ListRecieve, m_ListRecieve);
	DDX_Text(pDX, IDC_RecieveName, m_RecieveName);
	DDX_Text(pDX, IDC_ShotName, m_ShotName);
	DDX_Radio(pDX, IDC_CheckShot, m_bCheckShotOrRcv);
	DDX_Radio(pDX, IDC_DrawOnShotLine, m_bDrawOnShotOrRcv);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckT, CDialog)
	//{{AFX_MSG_MAP(CCheckT)
	ON_BN_CLICKED(IDC_ListRecieve, OnListRecieve)
	ON_BN_CLICKED(IDC_ListShot, OnListShot)
	ON_BN_CLICKED(IDC_CheckRecieve, OnCheckRecieve)
	ON_BN_CLICKED(IDC_CheckShot, OnCheckShot)
	ON_BN_CLICKED(IDC_DrawOnShotPoint, OnDrawOnShotPoint)
	ON_BN_CLICKED(IDC_DrawOnShotLine, OnDrawOnShotLine)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckT message handlers

void CCheckT::OnListRecieve() 
{
	
	int i;

	CMyList dlg;
	for(i=0;i<m_RecieveNameNumber;i++){
		dlg.Data[i]=m_RecieveNameList[i];
	}
	dlg.DataNumber=m_RecieveNameNumber;

	// 设置对话框的题头.
	dlg.CaptainText="接收线名称";
	dlg.ContentText="接收线名称";

	if(dlg.DoModal()){
		if(dlg.Selected!=-1){
			m_RecieveName=m_RecieveNameList[dlg.Selected];
		}
		CEdit* pEdit=(CEdit*) GetDlgItem(IDC_RecieveName);
		pEdit->SetWindowText(STR((int)m_RecieveName));
	}

}

void CCheckT::OnListShot() 
{
	int i;
	if(m_bDrawOnShotOrRcv==0){  //炮线。

		CMyList dlg;			
		dlg.DataNumber=m_ShotLineTo-m_ShotLineFrom+1;
		for(i=m_ShotLineFrom;i<=m_ShotLineTo;i++){
			dlg.Data[i-m_ShotLineFrom]=i;
		}										 			

		// 设置对话框的题头.
		dlg.CaptainText="炮线名称";
		dlg.ContentText="炮线名称";

		if(dlg.DoModal())
			m_ShotName=m_ShotLineFrom+dlg.Selected;
	}
	else{
		CMyList dlg;
		dlg.DataNumber=m_ShotNameNumber;
		for(i=0;i<m_ShotNameNumber;i++){
			dlg.Data[i]=m_ShotPointNameList[i];
		}										 			

		// 设置对话框的题头.
		dlg.CaptainText="炮点线名称";
		dlg.ContentText="炮点线名称";

		if(dlg.DoModal())
			if(dlg.Selected!=-1)
				m_ShotName=m_ShotPointNameList[dlg.Selected];
	}

	CEdit* pEdit=(CEdit*) GetDlgItem(IDC_ShotName);
	pEdit->SetWindowText(STR((int)m_ShotName));

	return;																								  
}

void CCheckT::OnCheckRecieve() 
{
	m_bCheckShotOrRcv=1;
}

void CCheckT::OnCheckShot() 
{
	m_bCheckShotOrRcv=0;
}

void CCheckT::OnDrawOnShotPoint() 
{
	m_bDrawOnShotOrRcv=1;	
	m_ShotName=m_ShotPointNameList[0];
	CEdit* pEdit=(CEdit*) GetDlgItem(IDC_ShotName);
	pEdit->SetWindowText(STR((int)m_ShotName));
}

void CCheckT::OnDrawOnShotLine() 
{
	m_bDrawOnShotOrRcv=0;		
	m_ShotName=m_ShotLineFrom;
	CEdit* pEdit=(CEdit*) GetDlgItem(IDC_ShotName);
	pEdit->SetWindowText(STR((int)m_ShotName));
}

void CCheckT::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();
}
