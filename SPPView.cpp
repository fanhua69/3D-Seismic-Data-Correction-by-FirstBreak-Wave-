// SPPView.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "spp.h"
#include "SPPDoc.h"
#include "SPPView.h"
#include "afxcmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSPPView

IMPLEMENT_DYNCREATE(CSPPView, CFHEditView)
#include "Gridctrl.h"

CSPPView::CSPPView()
{
	
}

CSPPView::~CSPPView()
{
}


CSPPDoc* CSPPView::GetDocument()
{										 
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSPPDoc)));
	return (CSPPDoc*)m_pDocument;
}


BEGIN_MESSAGE_MAP(CSPPView, CFHEditView)
	//{{AFX_MSG_MAP(CSPPView)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSPPView drawing

void CSPPView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CSPPView diagnostics

#ifdef _DEBUG
void CSPPView::AssertValid() const
{
	CFHEditView::AssertValid();
}

void CSPPView::Dump(CDumpContext& dc) const
{
	CFHEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSPPView message handlers

void CSPPView::OnInitialUpdate() 
{
	
	CSPPDoc *pDoc=GetDocument();
	int nRow=100;
	if(pDoc->m_TotalSNumber>nRow)nRow = pDoc->m_TotalSNumber;
	SetSize(nRow,11);
	
	CFHEditView::OnInitialUpdate();	
}



void CSPPView::UpdateDoc()
{
	CSPPDoc *pDoc=GetDocument();

	
	long nRow=m_Grid.GetRowCount();
	long Ph;
	for(long i=1;i<nRow;i++){
		GetItem(i,1,&Ph);
		if(Ph==0)break;
		if(i-1>=pDoc->m_nShotLimit ){
			AfxMessageBox("Too much shot parameters, can not save all of them!");
			break;
		}

		GetItem(i,1,&pDoc->m_pSPP [i-1].m_Ph );
		GetItem(i,2,&pDoc->m_pSPP [i-1].m_FileNumber );
		GetItem(i,3,&pDoc->m_pSPP[i-1].m_ZP);
		GetItem(i,4,&pDoc->m_pSPP[i-1].m_HP);
		GetItem(i,5,&pDoc->m_pSPP[i-1].m_BeginTrace);
		GetItem(i,6,&pDoc->m_pSPP[i-1].m_EndTrace);
		GetItem(i,7,&pDoc->m_pSPP[i-1].m_BeginGapTrace);
		GetItem(i,8,&pDoc->m_pSPP[i-1].m_EndGapTrace);
		GetItem(i,9,&pDoc->m_pSPP[i-1].m_XZB);
		GetItem(i,10,&pDoc->m_pSPP[i-1].m_YZB);
	}
	pDoc->m_TotalSNumber =i-1;

}

void CSPPView::UpdateGrid()
{
	CFHEditView::UpdateGrid();

	CSPPDoc *pDoc=GetDocument();

	SetItem(0,0,"No.");
	SetItem(0,1,"桩号");
    SetItem(0,2,"文件号");
	SetItem(0,3,"纵向偏移");
	SetItem(0,4,"横向偏移");
	SetItem(0,5,"起始道");
	SetItem(0,6,"终止道");
	SetItem(0,7,"裂口起始道");
	SetItem(0,8,"裂口终止道");
	SetItem(0,9,"南北坐标");
	SetItem(0,10,"东西坐标");

	for(int i=0;i<pDoc->m_TotalSNumber;i++){
		SetItem(i+1,0,long(i+1));
		SetItem(i+1,1,pDoc->m_pSPP[i].m_Ph);
		SetItem(i+1,2,pDoc->m_pSPP[i].m_FileNumber);
		SetItem(i+1,3,pDoc->m_pSPP[i].m_ZP);
		SetItem(i+1,4,pDoc->m_pSPP[i].m_HP);
		SetItem(i+1,5,pDoc->m_pSPP[i].m_BeginTrace);
		SetItem(i+1,6,pDoc->m_pSPP[i].m_EndTrace);
		SetItem(i+1,7,pDoc->m_pSPP[i].m_BeginGapTrace);
		SetItem(i+1,8,pDoc->m_pSPP[i].m_EndGapTrace);
		SetItem(i+1,9,pDoc->m_pSPP[i].m_XZB);
		SetItem(i+1,10,pDoc->m_pSPP[i].m_YZB);
	}

	for(i=0;i<m_Grid.GetRowCount();i++){
		SetItem(i+1,0,long(i+1));	
	}
}
