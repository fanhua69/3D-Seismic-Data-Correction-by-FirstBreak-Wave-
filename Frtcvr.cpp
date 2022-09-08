// FrtCvr.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "FrtCvr.h"
#include "mmsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrtCvr dialog


CFrtCvr::CFrtCvr(CWnd* pParent /*=NULL*/)
	: CDialog(CFrtCvr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFrtCvr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	tag=FALSE;
}


void CFrtCvr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFrtCvr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFrtCvr, CDialog)
	//{{AFX_MSG_MAP(CFrtCvr)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrtCvr message handlers

void CFrtCvr::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	tag=TRUE;		
	CDialog::OnLButtonDown(nFlags, point);
}

void CFrtCvr::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(tag==TRUE)OnOK();
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CFrtCvr::OnPaint() 
{
	//sndPlaySound("frtcover.wav",SND_ASYNC);
	CPaintDC dc(this); // device context for painting
	BITMAP bm;
	CSize oldbmpsize;
	CPoint ptorg;
	CRect rect;
	int dx,dy;

	CBitmap *poldbmp,*frontbmp=new CBitmap;
	CDC hMemDCsrc;

	frontbmp->LoadBitmap(IDB_FrontCover);
	hMemDCsrc.CreateCompatibleDC(&dc);
	poldbmp=hMemDCsrc.SelectObject(frontbmp);
	frontbmp->GetObject(sizeof(BITMAP),&bm);

	oldbmpsize.cx=bm.bmWidth;
	oldbmpsize.cy=bm.bmHeight;

	ptorg.x=ptorg.y=0;

	dc.DPtoLP(&ptorg,1);
	GetClientRect(&rect);

	dx=rect.right-rect.left;
	dy=rect.bottom-rect.top;

	dc.StretchBlt(0,0,dx,dy,&hMemDCsrc,ptorg.x,ptorg.y,
		oldbmpsize.cx,oldbmpsize.cy,SRCCOPY);
	hMemDCsrc.SelectObject(poldbmp);
	hMemDCsrc.DeleteDC();

	delete frontbmp;
	// Do not call CDialog::OnPaint() for painting messages
}
