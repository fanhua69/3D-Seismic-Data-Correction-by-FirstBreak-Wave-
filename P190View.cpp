// P190View.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "P190View.h"
#include "math.h"
#include "MyPoint.h"
#include "colordlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CP190View

IMPLEMENT_DYNCREATE(CP190View, CFHZoomView)

CP190View::CP190View()
{
	m_nShotCurrent=0;
}

CP190View::~CP190View()
{
}


BEGIN_MESSAGE_MAP(CP190View, CFHZoomView)
	//{{AFX_MSG_MAP(CP190View)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CP190View drawing

void CP190View::OnInitialUpdate()
{	
	CP190Doc *pDoc=GetDocument();
	
	m_dMinX=999999999999999;
	m_dMinY=999999999999999;
	m_dMaxX=-99999999999999;
	m_dMaxY=-99999999999999;

	for(long i=0;i<pDoc->m_P190 .m_nShotPhyNumber;i++){
		if(pDoc->m_P190 .m_pShotPhyPar [i].east <m_dMinX){
			m_dMinX=pDoc->m_P190 .m_pShotPhyPar [i].east ;
		}
		if(pDoc->m_P190 .m_pShotPhyPar [i].north <m_dMinY){
			m_dMinY=pDoc->m_P190 .m_pShotPhyPar [i].north ;
		}
		if(pDoc->m_P190 .m_pShotPhyPar [i].east >m_dMaxX){
			m_dMaxX=pDoc->m_P190 .m_pShotPhyPar [i].east ;
		}
		if(pDoc->m_P190 .m_pShotPhyPar [i].north >m_dMaxY){
			m_dMaxY=pDoc->m_P190 .m_pShotPhyPar [i].north ;
		}
	}

	SetDrawRange(m_dMinX,m_dMinY,m_dMaxX,m_dMaxY);

	CFHZoomView::OnInitialUpdate();
}

void CP190View::OnDraw(CDC* pDC)
{
	CP190Doc* pDoc = (CP190Doc* )GetDocument();
	// TODO: add draw code here

	CPen penRed,penGreen,penBlue;
	penRed.CreatePen(PS_SOLID,0,RGB(255,0,0));
	penGreen.CreatePen(PS_SOLID,0,RGB(0,255,0));
	penBlue.CreatePen(PS_SOLID,0,RGB(0,0,255));

	CPen *pOldPen=pDC->SelectObject(&penRed);

	double x,y;

	// Draw the shot:
	int nRadius=3;
	for(int i=0;i<pDoc->m_P190 .m_nShotPhyNumber ;i++){
		x=pDoc->m_P190 .m_pShotPhyPar [i].east ;
		y=pDoc->m_P190 .m_pShotPhyPar [i].north ;

		LPtoWorld(x,y);

		pDC->Ellipse(x-nRadius,y-nRadius,x+nRadius,y+nRadius);
	}

	// Draw the Rcv:
	pDC->SelectObject(&penBlue);
	nRadius=2;
  	for(i=0;i<pDoc->m_P190 .m_nRcvPhyNumber ;i++){
		x=pDoc->m_P190 .m_pRcvPhyPar [i].east ;
		y=pDoc->m_P190 .m_pRcvPhyPar [i].north ;

		LPtoWorld(x,y);

		pDC->Ellipse(x-nRadius,y-nRadius,x+nRadius,y+nRadius);
	}


	pDC->SelectObject(&penBlue);

	long ph,j,n;

	CBrush brush;
	brush.CreateSolidBrush (RGB(255,0,0));
	CBrush *pOldBrush=pDC->SelectObject (&brush);
	
	// Draw all of the rcvs of the shot:
	nRadius=3;
	for(i=0;i<pDoc->m_P190.m_pShotRcvRel[m_nShotCurrent].nRcvLine;i++){
		for(j=pDoc->m_P190.m_pShotRcvRel[m_nShotCurrent].RcvRange[i].start;
			j<pDoc->m_P190.m_pShotRcvRel[m_nShotCurrent].RcvRange[i].end;
			j++){
			
				// Check which rcv is the ph of j:
				n=pDoc->m_P190.SearchRcvStation(j);
				if(n==-1)continue;

				x=pDoc->m_P190.m_pRcvPhyPar[n].east;
				y=pDoc->m_P190.m_pRcvPhyPar[n].north;

				LPtoWorld(x,y);
				pDC->Ellipse(x-nRadius,y-nRadius,x+nRadius,y+nRadius);
			}
	}
	
	// Draw the pointed shot point:
	CMyPoint pointShot=pDoc->m_P190.GetShotPos(pDoc->m_P190.m_pShotRcvRel[m_nShotCurrent].PhShot);
	LPtoWorld(pointShot.y,pointShot.x);
	nRadius=5;
	pDC->Ellipse(pointShot.y-nRadius,pointShot.x-nRadius,pointShot.y+nRadius,pointShot.x+nRadius);
	
	
	/*
	// Draw the used rcvs:
	long nRcv;	
	nRadius=3;
	CMyPoint * pPoint=pDoc->m_P190.GetRcvPosition (pDoc->m_P190.m_pShotRcvRel[m_nShotCurrent].PhShot ,nRcv,0,30);
	for(i=0;i<nRcv;i++){
		x=pPoint[i].x;
		y=pPoint[i].y;

		LPtoWorld(x,y);
		pDC->Ellipse(x-nRadius,y-nRadius,x+nRadius,y+nRadius);
	}
	*/

	

	pDC->SelectObject (pOldBrush);
	pDC->SelectObject(pOldPen);


}

/////////////////////////////////////////////////////////////////////////////
// CP190View diagnostics

#ifdef _DEBUG
void CP190View::AssertValid() const
{
	CFHZoomView::AssertValid();
}

void CP190View::Dump(CDumpContext& dc) const
{
	CFHZoomView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CP190View message handlers

CP190Doc *CP190View::GetDocument()
{
	return (CP190Doc*)m_pDocument;
}

CString CP190View::GetInfo(double x, double y)
{
	CP190Doc *pDoc=GetDocument();

	double dx,dy,dl,dlMin=999999999999;
	long Ph;
	for(long i=0;i<pDoc->m_P190.m_nShotPhyNumber;i++){
		dx=x-pDoc->m_P190 .m_pShotPhyPar [i].east ;
		dy=y-pDoc->m_P190 .m_pShotPhyPar [i].north ;

		dl=sqrt(dx*dx+dy*dy);
		if(dl<dlMin){
			dlMin=dl;
			Ph=pDoc->m_P190 .m_pShotPhyPar [i].Ph;
		}
	} 

	
	m_nShotCurrent=pDoc->m_P190.SearchShotStation(Ph);
	if(m_nShotCurrent==-1)m_nShotCurrent=0;

	Invalidate();

	CString s;
	s.Format("File Number:%ld,Station :%ld",pDoc->m_P190.m_pShotRcvRel[m_nShotCurrent].FileNumber,
		pDoc->m_P190.m_pShotRcvRel[m_nShotCurrent].PhShot);
	return s;
}
