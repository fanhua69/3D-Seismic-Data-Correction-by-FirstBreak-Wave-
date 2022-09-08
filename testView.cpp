// testView.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "testView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// testView

IMPLEMENT_DYNCREATE(testView, CScrollView)

testView::testView()
{
}

testView::~testView()
{
}


BEGIN_MESSAGE_MAP(testView, CScrollView)
	//{{AFX_MSG_MAP(testView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// testView drawing

void testView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// testView diagnostics

#ifdef _DEBUG
void testView::AssertValid() const
{
	CScrollView::AssertValid();
}

void testView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// testView message handlers

void testView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	
	SetScrollSizes(MM_TEXT,CSize(1000,1000),CSize(100,100),CSize(10,10));	
}
