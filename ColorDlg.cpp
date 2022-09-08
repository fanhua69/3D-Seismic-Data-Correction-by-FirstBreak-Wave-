// ColorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "ColorDlg.h"
#include "FontGroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog


CColorDlg::CColorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CColorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    m_bParentDisabled = FALSE;

	m_dMinValue=0;
	m_dMaxValue=512;
	m_nColorNumber=512;
	m_dScale=1;

	int n;
	for(int j=0;j<100;j++){
		m_nColor[j]=RGB(0,0,j*2.5);
	}
					
	for(j=100;j<200;j++){
		n=(j-100)*2.5;
		m_nColor[j]=RGB(0,n,255);
	}
	
	for(j=200;j<300;j++){
		n=(300-j)*2.5;
		m_nColor[j]=RGB(0,255,n);
	}

	for(j=300;j<400;j++){
		n=(j-300)*2.5;
		m_nColor[j]=RGB(n,255,0);
	}

	for(j=400;j<512;j++){
		n=(512-j)*2;
		m_nColor[j]=RGB(255,n,0);
	}



}
CColorDlg::~CColorDlg()
{
	if(m_hWnd!=NULL)
		  DestroyWindow();
}

void CColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CColorDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CColorDlg, CDialog)
	//{{AFX_MSG_MAP(CColorDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorDlg message handlers

BOOL CColorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CColorDlg::Create(CWnd *pParent)
{
	// Get the true parent of the dialog
    m_pParentWnd = CWnd::GetSafeOwner(pParent);

    // m_bParentDisabled is used to re-enable the parent window
    // when the dialog is destroyed. So we don't want to set
    // it to TRUE unless the parent was already enabled.

    if((m_pParentWnd!=NULL) && m_pParentWnd->IsWindowEnabled())
    {
      m_pParentWnd->EnableWindow(FALSE);
      m_bParentDisabled = TRUE;
    }

    if(!CDialog::Create(CColorDlg::IDD,pParent))
    {
      return FALSE;
    }
    ReEnableParent();

    return TRUE;
}
void CColorDlg::ReEnableParent()
{
    if(m_bParentDisabled && (m_pParentWnd!=NULL))
      m_pParentWnd->EnableWindow(TRUE);
    m_bParentDisabled=FALSE;
}

BOOL CColorDlg::DestroyWindow()
{
    ReEnableParent();
    return CDialog::DestroyWindow();
}
void CColorDlg::SetRange(double dMinValue, double dMaxValue)
{
	m_dMinValue=dMinValue;
	m_dMaxValue=dMaxValue;
	m_dScale=m_nColorNumber/(m_dMaxValue-m_dMinValue);
	if(m_hWnd){
		Invalidate();
	}
}

COLORREF CColorDlg::GetColor(double dValue)
{
	if(dValue<m_dMinValue)dValue=m_dMinValue;
	if(dValue>m_dMaxValue)dValue=m_dMaxValue;
	return m_nColor[int((dValue-m_dMinValue)*m_dScale)];
}

void CColorDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// The fonts:
	CString s;
	s.Format("%1.0lf",m_dMinValue);
	CSize sizeA=dc.GetTextExtent (s);
	s.Format("%1.0lf",m_dMaxValue);
	CSize sizeB=dc.GetTextExtent (s);
	int nFontWidth=sizeA.cx;
	if(sizeB.cx>nFontWidth)nFontWidth=sizeB.cx;
	int nFontHeight=sizeA.cy ;
	
	// TODO: Add your message handler code here
	CRect rect;
	this->GetWindowRect (&rect);
	int cx=rect.Width ();
	int cy=rect.Height ();

	rect.left =cx/10;
	rect.right =cx/10*9;
	rect.top =cy/20;
	rect.bottom =cy/20*18;  
	dc.Rectangle (rect.left-1,rect.bottom+1,rect.right+1,rect.top-1);	

	CFontGroup fontGroup;
	fontGroup.Set(m_dMinValue,m_dMaxValue,rect.Height (),nFontHeight);
	int nValue;
	double *pGroup=fontGroup.CalcFontGroup (nValue);

	double dScale=(double)rect.Height ()/(double)m_nColorNumber;
	int bottom,left=rect.left ,width=rect.Width ()-nFontWidth-2,height=int(dScale)+1;
	dc.Rectangle (left-1,rect.bottom+1,left+width+1,rect.top-1);

	for(int i=0;i<m_nColorNumber;i++){
		bottom=rect.bottom-i*dScale;
		dc.FillSolidRect(left,bottom ,width,height,m_nColor[i]);
	}

	COLORREF nColor,nBackColor=RGB(255,255,255);
	CString sValue;
	left=rect.right-nFontWidth-1;
	
	CBrush brush;
	brush.CreateSolidBrush (nBackColor);
	CBrush *pOldBrush=dc.SelectObject(&brush);
	dc.SetBkColor (nBackColor);
	//dc.Rectangle (rect.right,rect.top,cx/10*9,rect.bottom );
	for(i=0;i<nValue-1;i++){
		sValue.Format("%1.0lf",pGroup[i]);
		nColor=int((pGroup[i]-m_dMinValue)*m_dScale);
		bottom=rect.bottom-nColor*dScale;
		dc.TextOut (left,bottom,sValue);
	}
	dc.SelectObject(pOldBrush);
	
	// Do not call CDialog::OnPaint() for painting messages
}



void CColorDlg::OnCancel() 
{
	//CDialog::OnCancel();
	
	// TODO: Add your message handler code here
	
}
void CColorDlg::OnOK() 
{
	//CDialog::OnCancel();
	
	// TODO: Add your message handler code here
	
}
