// TopoNetView.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "TopoNetView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTopoNetView

IMPLEMENT_DYNCREATE(CTopoNetView, CFHZoomView)

CTopoNetView::CTopoNetView()
{
	m_dLarger=100000;

	m_nValuePoint=0;;
	m_nValuePointLimit=100;;
	m_dValuePoint=new double [m_nValuePointLimit];

	m_dlgColor.Create();
	m_dlgColor.SetRange(0,10);

}

CTopoNetView::~CTopoNetView()
{
	delete []m_dValuePoint;
}


BEGIN_MESSAGE_MAP(CTopoNetView, CFHZoomView)
	//{{AFX_MSG_MAP(CTopoNetView)
	ON_COMMAND(ID_Test_TopoNet, OnTestTopoNet)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTopoNetView drawing

void CTopoNetView::OnInitialUpdate()
{
	CTopoNetDoc *pDoc=GetDocument();

	double xMin=999999999,xMax=-999999999,yMin=999999999,yMax=-999999999;
	CMy3DPoint point;
	for(long i=0;i<pDoc->m_nTopoPoint ;i++){
		point=pDoc->m_net .GetPoint (i);
		if(point.x<xMin)xMin=point.x;
		if(point.y<yMin)yMin=point.y;
		if(point.x>xMax)xMax=point.x;
		if(point.y>yMax)yMax=point.y;
	}

	SetDrawRange(xMin,yMin,xMax,yMax);	
	CFHZoomView::OnInitialUpdate();	
}

void CTopoNetView::OnDraw(CDC* pDC)
{
	CTopoNetDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CString s;
	CTagLine *pLine=pDoc->m_net.GetLine();
	long nLine=pDoc->m_net.GetLineNumber ();

	double x,y;
	for(int i=0;i<nLine;i++){
		x=pLine[i].x1;
		y=pLine[i].y1;
		LPtoWorld(x,y);
		pDC->MoveTo(x,y);

		x=pLine[i].x2;
		y=pLine[i].y2;
		LPtoWorld(x,y);
		pDC->LineTo(x,y);
	}

	COLORREF nColor;
	CMy3DPoint point;
	for(i=0;i<pDoc->m_nTopoPointLimit;i++){
		point=pDoc->m_net.GetPoint(i);
		nColor=m_dlgColor.GetColor(point.z );
		
		x=point.x;
		y=point.y;
		LPtoWorld(x,y);
		
		pDC->FillSolidRect (x,y,8,8,nColor);
	}

	for(i=0;i<3;i++){
		x=m_Point3D[i].x;
		y=m_Point3D[i].y;
		LPtoWorld(x,y);
		pDC->MoveTo(x,y);

		pDC->FillSolidRect (x,y,8,8,RGB(255,0,0));
	}

	x=m_pointWanted.x;
	y=m_pointWanted.y;

	pDC->FillSolidRect (x,y,8,8,m_dlgColor.GetColor(m_dValuePoint[m_nValuePoint]));

 }

/////////////////////////////////////////////////////////////////////////////
// CTopoNetView diagnostics

#ifdef _DEBUG
void CTopoNetView::AssertValid() const
{
	CFHZoomView::AssertValid();
}

void CTopoNetView::Dump(CDumpContext& dc) const
{
	CFHZoomView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTopoNetView message handlers

CTopoNetDoc* CTopoNetView::GetDocument()
{
	ASSERT_VALID(m_pDocument);
	return (CTopoNetDoc *)m_pDocument;

}

void CTopoNetView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_nValuePoint>=m_nValuePointLimit){
		AfxMessageBox("Too much value points!");
		return;
	}

	CTopoNetDoc *pDoc=GetDocument();
	
	CPoint pointOrg=GetDeviceScrollPosition();
	double x,y;
	x=point.x+pointOrg.x;
	y=point.y+pointOrg.y;
	WorldtoLP(x,y);

	m_pointWanted.x=x;
	m_pointWanted.y=y;

	m_dValuePoint[m_nValuePoint]=pDoc->m_net.GetValue(x,y);
	pDoc->m_net.GetNetPoint(CMyPrcPoint(x,y),m_Point3D);
	Invalidate();

	//m_nValuePoint++;


	CFHZoomView::OnLButtonDown(nFlags, point);
}

void CTopoNetView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	
	CFHZoomView::OnRButtonDown(nFlags, point);

}
void CTopoNetView::OnTestTopoNet() 
{
	// TODO: Add your command handler code here
	CTopoNetDoc *pDoc=GetDocument();
	pDoc->m_net.SetPoint (50*m_dLarger,50*m_dLarger,1,0);
	pDoc->m_net.SetPoint (50*m_dLarger,100*m_dLarger,2,1);
	pDoc->m_net.SetPoint (100*m_dLarger,50*m_dLarger,3,2);
	pDoc->m_net.SetPoint (100*m_dLarger,100*m_dLarger,4,3);
	pDoc->m_net.SetPoint (150*m_dLarger,100*m_dLarger,5,4);
	pDoc->m_net.SetPoint (150*m_dLarger,50*m_dLarger,6,5);
	pDoc->m_net.SetPoint (100*m_dLarger,150*m_dLarger,7,6);
	pDoc->m_net.SetPoint (100*m_dLarger,200*m_dLarger,8,7);
	pDoc->m_net.SetPoint (100*m_dLarger,250*m_dLarger,9,8);
	pDoc->m_net.SetPoint (100*m_dLarger,300*m_dLarger,10,9);
	pDoc->m_net.SetPoint (100*m_dLarger,350*m_dLarger,11,10);	

	pDoc->m_nTopoPoint =11;

	if(pDoc->m_net.CalcNet()){
		Invalidate();
	}

}

CString CTopoNetView::GetInfo(double x, double y)
{
	CTopoNetDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	long nLine=pDoc->m_net.GetLineNumber ();

	CString s;
	s.Format("Line Number: %ld , Value : %1.1lf",nLine,m_dValuePoint[m_nValuePoint]);
	
	return s;	
}

void CTopoNetView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CTopoNetDoc *pDoc=GetDocument();

	if(pDoc->m_nTopoPoint>=pDoc->m_nTopoPointLimit ){
		AfxMessageBox("Too much points!");
		return;
	}

	CPoint pointOrg=GetDeviceScrollPosition();
	double x=point.x+pointOrg.x;
	double y=point.y+pointOrg.y;
	WorldtoLP(x,y);
	
	pDoc->m_net.SetPoint(x,y,8,pDoc->m_nTopoPoint);
	pDoc->m_nTopoPoint++;
	
	
	if(pDoc->m_net.CalcNet()){
		Invalidate();
	}
	
	CFHZoomView::OnLButtonDblClk(nFlags, point);
}

void CTopoNetView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{

	
	CFHZoomView::OnRButtonDblClk(nFlags, point);
}
