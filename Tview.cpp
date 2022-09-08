// TView.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "afxcmn.h"
#include "PXYV.h"
#include "TDoc.h"
#include "TView.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTView

IMPLEMENT_DYNCREATE(CTView, CFHZoomView)

CTView::CTView()
{
	m_penMidFbk.CreatePen(PS_SOLID,0,RGB(255,0,0));;
	m_penOutFbk.CreatePen(PS_SOLID,0,RGB(0,255,0));;
	m_penSnd.CreatePen(PS_SOLID,0,RGB(0,0,255));;

}

CTView::~CTView()
{
}


BEGIN_MESSAGE_MAP(CTView, CFHZoomView)
	//{{AFX_MSG_MAP(CTView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTView drawing

void CTView::OnInitialUpdate()
{
	CFHZoomView::OnInitialUpdate();
}

void CTView::OnDraw(CDC* pDC)
{
	CTDoc* pDoc = GetDocument();
	double x,y;
	
	// 绘制 Mid Fbk 数据．
	CPen *pOldPen=pDC->SelectObject(&m_penMidFbk);
	
	x=pDoc->m_pMidFbkLine[0].Ph;
	y=pDoc->m_pMidFbkLine[0].Value;
	LPtoWorld(x,y);
	pDC->MoveTo(x,y);

	for(long i=1;i<pDoc->m_nMidFbkLine;i++){		
		x=pDoc->m_pMidFbkLine[i].Ph;
		y=pDoc->m_pMidFbkLine[i].Value;
		LPtoWorld(x,y);
		pDC->LineTo(x,y);
	}
		
	// 绘制 Out Fbk 数据．
	pDC->SelectObject(&m_penOutFbk);	
	
	x=pDoc->m_pOutFbkLine[0].Ph;
	y=pDoc->m_pOutFbkLine[0].Value;
	LPtoWorld(x,y);
	pDC->MoveTo(x,y);

	for(i=1;i<pDoc->m_nOutFbkLine;i++){		
		x=pDoc->m_pOutFbkLine[i].Ph;
		y=pDoc->m_pOutFbkLine[i].Value;
		LPtoWorld(x,y);
		pDC->LineTo(x,y);
	}

	// Draw sand data:
	pDC->SelectObject(&m_penSnd);	

	x=pDoc->m_pSndLine[0].Ph;
	y=pDoc->m_pSndLine[0].Value;
	LPtoWorld(x,y);
	pDC->MoveTo(x,y);

	for(i=1;i<pDoc->m_nSndLine;i++){		
		x=pDoc->m_pSndLine[i].Ph;
		y=pDoc->m_pSndLine[i].Value;
		LPtoWorld(x,y);
		pDC->LineTo(x,y);
	}

	//	
	pDC->SelectObject(pOldPen);

}

/////////////////////////////////////////////////////////////////////////////
// CTView diagnostics

#ifdef _DEBUG
void CTView::AssertValid() const
{
	CFHZoomView::AssertValid();
}

void CTView::Dump(CDumpContext& dc) const
{
	CFHZoomView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTView message handlers
CTDoc* CTView::GetDocument()
{										 
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTDoc)));
	return (CTDoc*)m_pDocument;
}


void CTView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	double xMin,xMax,yMin,yMax;

	CTDoc *pDoc=GetDocument();
	if(!pDoc->m_pMidFbkLine){
		SetDrawRange(0,0,1000,1000);
	}
	else{

		xMin=xMax=pDoc->m_pSndLine[0].Ph;
		yMin=yMax=pDoc->m_pSndLine[0].Value;	

		for(long i=0;i<pDoc->m_nSndLine;i++){	
			
			if(pDoc->m_pSndLine[i].Ph==0)continue;
			if(pDoc->m_pSndLine[i].Ph<xMin)xMin=pDoc->m_pSndLine[i].Ph;
			if(pDoc->m_pSndLine[i].Ph>xMax)xMax=pDoc->m_pSndLine[i].Ph;
			
			if(pDoc->m_pSndLine[i].Value==-9999)continue;
			if(pDoc->m_pSndLine[i].Value<yMin)yMin=pDoc->m_pSndLine[i].Value;
			if(pDoc->m_pSndLine[i].Value>yMax)yMax=pDoc->m_pSndLine[i].Value;	

		}
		
		double dXScale=100/(xMax-xMin);
		double dYScale=10/(yMax-yMin);
		SetDrawRange(xMin-100,yMin-150,xMax+100,yMax+150,dXScale,dYScale);
	}

	CFHZoomView::OnUpdate(pSender, lHint, pHint) ;
}

CString CTView::GetInfo(double x, double y)
{
	CTDoc *pDoc=GetDocument();
	CString s;
	s.Format("Line: %s, Station : %ld, Value: %1.1f",pDoc->m_sSndLine, (long)x,(long)y);
	return s;
}
