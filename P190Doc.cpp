// P190Doc.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "P190Doc.h"
#include "ProDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CP190Doc

IMPLEMENT_DYNCREATE(CP190Doc, CDocument)

CP190Doc::CP190Doc()
{
}

BOOL CP190Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	AfxMessageBox("Can NOT make P190 file now!");
	return false;
}

CP190Doc::~CP190Doc()
{
}


BEGIN_MESSAGE_MAP(CP190Doc, CDocument)
	//{{AFX_MSG_MAP(CP190Doc)
	ON_COMMAND(ID_P190_FormatChange, OnP190FormatChange)
	ON_COMMAND(ID_P190_SaveRcvStaticValue, OnP190SaveRcvStaticValue)
	ON_COMMAND(ID_P190_SaveShotStaticValue, OnP190SaveShotStaticValue)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CP190Doc diagnostics

#ifdef _DEBUG
void CP190Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CP190Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CP190Doc serialization

void CP190Doc::Serialize(CArchive& ar)
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
// CP190Doc commands

BOOL CP190Doc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	return m_P190.Open (lpszPathName);
	
}

void CP190Doc::OnP190FormatChange() 
{
	// TODO: Add your command handler code here
	m_P190.FormatChange ();
	
}

void CP190Doc::OnP190SaveRcvStaticValue() 
{
	// TODO: Add your command handler code here
	m_P190.SaveRcvStatic ();	
	
}

void CP190Doc::OnP190SaveShotStaticValue() 
{
	// TODO: Add your command handler code here
	m_P190.SaveShotStatic ();
	
}
