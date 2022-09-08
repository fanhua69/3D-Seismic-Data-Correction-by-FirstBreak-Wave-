// ControlView.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "ControlView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlView

IMPLEMENT_DYNCREATE(CControlView, CFHEditView)

CControlView::CControlView()
{
}

CControlView::~CControlView()
{
}


BEGIN_MESSAGE_MAP(CControlView, CFHEditView)
	//{{AFX_MSG_MAP(CControlView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlView drawing

void CControlView::OnInitialUpdate()
{
	CFHEditView::OnInitialUpdate();
}

void CControlView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CControlView diagnostics

#ifdef _DEBUG
void CControlView::AssertValid() const
{
	CFHEditView::AssertValid();
}

void CControlView::Dump(CDumpContext& dc) const
{
	CFHEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CControlView message handlers

void CControlView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CFHEditView::OnUpdate(pSender, lHint, pHint); 
	
}


CControlDoc * CControlView::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CControlDoc)));
	return (CControlDoc*)m_pDocument;
}
void CControlView::UpdateGrid()
{
	
	// TODO: Add your specialized code here and/or call the base class
	m_Grid.DeleteAllItems ();
	CControlDoc *pDoc=GetDocument();

	long nShot=pDoc->m_CtlFile .GetShotCtlNumber ();
	long nRcv=pDoc->m_CtlFile .GetRcvCtlNumber ();
	long nRow=m_Grid.GetRowCount ();

	if(nRcv>nRow)nRow=nRcv;
	if(nShot>nRow)nRow=nShot;
	if(nRow<10)nRow=10;

	SetSize(nRow,3);

	SetItem(0,0,"No.");
	SetItem(0,1,"Shot Ctl");
	SetItem(0,2,"Rcv Ctl");

	for(long i=0;i<nShot;i++){
		SetItem(i+1,1,pDoc->m_CtlFile .m_pShotCtlPoint [i].Ph );
	}

	for(i=0;i<nRcv;i++){
		SetItem(i+1,2,pDoc->m_CtlFile .m_pRcvCtlPoint [i].Ph );
	}

	for(i=1;i<m_Grid.GetRowCount ();i++){
		SetItem(i,0,i);
	}

	CFHEditView::UpdateGrid();
}


void CControlView::UpdateDoc()
{
	CControlDoc *pDoc=GetDocument();
	pDoc->m_CtlFile .Reset ();

	long nRow=m_Grid.GetRowCount ();

	long nShot=0,nRcv=0,Ph;
	for(long i=nRow-1;i>0;i--){
		GetItem(i,1,&Ph);
		if(Ph!=0){
			break;
		}
	}
	
	if(Ph!=0)nShot=i;

	//
	for(i=nRow-1;i>0;i--){
		GetItem(i,2,&Ph);
		if(Ph!=0){
			break;
		}
	}
	
	if(Ph!=0)nRcv=i;
	
	//
	pDoc->m_CtlFile.SetRcvCtlNumber (nRcv);
	pDoc->m_CtlFile.SetShotCtlNumber (nShot);

	for(i=1;i<nRow;i++){
		GetItem(i,1,&Ph);
		if(Ph==0){
			break;
		}

		pDoc->m_CtlFile.m_pShotCtlPoint [i-1].Ph =Ph;
	}

	for(i=1;i<nRow;i++){
		GetItem(i,2,&Ph);
		if(Ph==0){
			break;
		}

		pDoc->m_CtlFile.m_pRcvCtlPoint [i-1].Ph =Ph;
	}


}
