// testDoc.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "testDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// testDoc

IMPLEMENT_DYNCREATE(testDoc, CDocument)

testDoc::testDoc()
{
}

BOOL testDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

testDoc::~testDoc()
{
}


BEGIN_MESSAGE_MAP(testDoc, CDocument)
	//{{AFX_MSG_MAP(testDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// testDoc diagnostics

#ifdef _DEBUG
void testDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void testDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// testDoc serialization

void testDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
		AfxMessageBox("EEQRE");
	}
}

/////////////////////////////////////////////////////////////////////////////
// testDoc commands
