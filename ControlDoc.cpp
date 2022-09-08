// ControlDoc.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "ControlDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlDoc

IMPLEMENT_DYNCREATE(CControlDoc, CFHEditDoc)

CControlDoc::CControlDoc()
{
}

BOOL CControlDoc::OnNewDocument()
{
	if (!CFHEditDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

CControlDoc::~CControlDoc()
{
}


BEGIN_MESSAGE_MAP(CControlDoc, CFHEditDoc)
	//{{AFX_MSG_MAP(CControlDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlDoc diagnostics

#ifdef _DEBUG
void CControlDoc::AssertValid() const
{
	CFHEditDoc::AssertValid();
}

void CControlDoc::Dump(CDumpContext& dc) const
{
	CFHEditDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CControlDoc serialization

void CControlDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CControlDoc commands

BOOL CControlDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CFHEditDoc::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here   
	return m_CtlFile.Read(lpszPathName);
}

BOOL CControlDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	return m_CtlFile.Write(lpszPathName);

}
