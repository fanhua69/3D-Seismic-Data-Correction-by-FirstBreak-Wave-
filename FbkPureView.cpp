// FbkPureView.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "FbkPureView.h"
#include "fbk.h"
#include "Dpgnum.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFbkPureView

IMPLEMENT_DYNCREATE(CFbkPureView, CFHZoomView)

CFbkPureView::CFbkPureView()
{
	m_nCurrentShot=1;
	m_bAnchor=false;

	m_nGroupAnchored=0;
	m_dAnchorPos=0;

	m_penCommon.CreatePen (PS_SOLID,0,RGB(255,0,0));
	m_penAnchored.CreatePen (PS_SOLID,0,RGB(255,0,0));

	m_brushCommon.CreateSolidBrush (RGB(0,255,0));
	m_brushAnchored.CreateSolidBrush (RGB(255,0,0));
}

CFbkPureView::~CFbkPureView()
{
}


BEGIN_MESSAGE_MAP(CFbkPureView, CFHZoomView)
	//{{AFX_MSG_MAP(CFbkPureView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_FbkPure_FindFileNumber, OnFbkPureFindFileNumber)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFbkPureView drawing

void CFbkPureView::OnInitialUpdate()
{
	CFbkPureDoc *pDoc=GetDocument();
	pDoc->SetShot (0);
	m_nCurrentShot=pDoc->m_nCurrentShotOrder;

	double x1=-10;
	double x2=pDoc->m_fbk.GetMaxGroupNumber();
	double y1=0;
	double y2=2000;
	SetDrawRange(x1,y1,x2,y2,20,1);
	SetDirection(false);

	CFHZoomView::OnInitialUpdate();
}

void CFbkPureView::OnDraw(CDC* pDC)
{
	CFbkPureDoc* pDoc = GetDocument();
	// TODO: add draw code here

	
	DataInFstBrkFile *pFbk=pDoc->m_pShotFbk ;
	if(!pFbk)return ;
	long nGroupNumber=pDoc->m_nGroupNumberInCurShot ;


	CPen *pOldPen=pDC->SelectObject(&m_penCommon);
	CBrush *pOldBrush=pDC->SelectObject(&m_brushCommon);

	double x,y;
	x=0;
	y=pFbk[0].FirstBreak ;
	LPtoWorld(x,y);
	pDC->MoveTo(x,y);
	for(long i=0;i<nGroupNumber;i++){
		x=i;
		y=pFbk[i].FirstBreak ;
		LPtoWorld(x,y);
		pDC->LineTo(x,y);
	}

	int nR=3;
	for(i=0;i<nGroupNumber;i++){
		x=i;
		y=pFbk[i].FirstBreak ;
		LPtoWorld(x,y);
		pDC->Ellipse(x-nR,y-nR,x+nR,y+nR);
	}

	if(m_bAnchor){
		nR=4 ;
		pDC->SelectObject(&m_penAnchored);
		pDC->SelectObject(&m_brushAnchored);

		x=m_nGroupAnchored;
		y=pFbk[m_nGroupAnchored].FirstBreak ;
		LPtoWorld(x,y);
		pDC->Ellipse(x-nR,y-nR,x+nR,y+nR);
	}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);

}

/////////////////////////////////////////////////////////////////////////////
// CFbkPureView diagnostics

#ifdef _DEBUG
void CFbkPureView::AssertValid() const
{
	CFHZoomView::AssertValid();
}

void CFbkPureView::Dump(CDumpContext& dc) const
{
	CFHZoomView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFbkPureView message handlers

CFbkPureDoc * CFbkPureView::GetDocument()
{
	return (CFbkPureDoc * )m_pDocument;
}

void CFbkPureView::GotoPrevPage()
{
	CFbkPureDoc *pDoc=GetDocument();

	pDoc->m_fbk.WriteShotFbk ();

	m_nCurrentShot--;
	if(m_nCurrentShot<0){
		m_nCurrentShot=0;
	}
	else{
		pDoc->SetShot (m_nCurrentShot);
		m_nCurrentShot=pDoc->m_nCurrentShotOrder;
		Invalidate();
	}

	m_dlgInfo.Show (GetInfo(0,0));

}

void CFbkPureView::GotoNextPage()
{
	CFbkPureDoc *pDoc=GetDocument();

	pDoc->m_fbk.WriteShotFbk ();

	m_nCurrentShot++;
	if(m_nCurrentShot>=pDoc->m_fbk.m_nShotNumber ){
		m_nCurrentShot=pDoc->m_fbk.m_nShotNumber -1;
	}
	else{
		pDoc->SetShot (m_nCurrentShot);
		m_nCurrentShot=pDoc->m_nCurrentShotOrder;
		Invalidate();
	}
	
	m_dlgInfo.Show (GetInfo(0,0));


}

void CFbkPureView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CFHZoomView::OnLButtonDown(nFlags, point);


	// TODO: Add your message handler code here and/or call default
	CFbkPureDoc *pDoc=GetDocument();
	if(!pDoc->m_pShotFbk)return;
	
	m_bAnchor=true;

	CPoint pointOrg=GetDeviceScrollPosition();
	point+=pointOrg;
	double x,y;
	x=point.x;
	y=point.y;
	WorldtoLP(x,y);

	long nGroup=long(x+0.5);
	if(nGroup<0)nGroup=0;
	if(nGroup>=pDoc->m_nGroupNumberInCurShot )nGroup=pDoc->m_nGroupNumberInCurShot-1;
	m_nGroupAnchored=nGroup;

	m_dAnchorPos=y-pDoc->m_pShotFbk [m_nGroupAnchored].FirstBreak ;

}

void CFbkPureView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFbkPureDoc *pDoc=GetDocument();			
	if(pDoc->m_pShotFbk&&m_bAnchor){
		CPoint pointOrg=GetDeviceScrollPosition();
		point+=pointOrg;
		double x,y;
		x=point.x;
		y=point.y;
		WorldtoLP(x,y);

		
		pDoc->m_pShotFbk [m_nGroupAnchored].FirstBreak=y-m_dAnchorPos ;

		Invalidate();
	}
	
	CFHZoomView::OnMouseMove(nFlags, point);
}

void CFbkPureView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CFbkPureDoc *pDoc=GetDocument();			
	if(pDoc->m_pShotFbk){
		m_bAnchor=false;
		Invalidate();
	}
	
	CFHZoomView::OnLButtonUp(nFlags, point);
}

CString CFbkPureView::GetInfo(double x, double y)
{
	CFbkPureDoc *pDoc=GetDocument();
	
	CString s;
	s.Format("Shot Order %ld, File Number: %ld",m_nCurrentShot,pDoc->m_nCurrentShotFileNumber);
	return s;
}

void CFbkPureView::OnFbkPureFindFileNumber() 
{
	// TODO: Add your command handler code here
	CFbkPureDoc *pDoc=GetDocument();
	pDoc->m_fbk.WriteShotFbk ();

	CDpgNum dlg;
	if(dlg.DoModal()==IDCANCEL)return;

	long nNum=dlg.m_PageNumber ;
	pDoc->SetShot (nNum,true);
	m_nCurrentShot=pDoc->m_nCurrentShotOrder;
	Invalidate();

	m_dlgInfo.Show (GetInfo(0,0));

	
}
