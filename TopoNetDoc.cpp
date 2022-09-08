// TopoNetDoc.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "TopoNetDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTopoNetDoc

IMPLEMENT_DYNCREATE(CTopoNetDoc, CDocument)

CTopoNetDoc::CTopoNetDoc()
{
	m_nTopoPoint=0;
	m_nTopoPointLimit=0;
}

BOOL CTopoNetDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_nTopoPointLimit=100;
	m_net.SetPointNumber(m_nTopoPointLimit);
	m_nTopoPoint=0;
	
	return TRUE;
}

CTopoNetDoc::~CTopoNetDoc()
{
}


BEGIN_MESSAGE_MAP(CTopoNetDoc, CDocument)
	//{{AFX_MSG_MAP(CTopoNetDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTopoNetDoc diagnostics

#ifdef _DEBUG
void CTopoNetDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTopoNetDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTopoNetDoc serialization

void CTopoNetDoc::Serialize(CArchive& ar)
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
// CTopoNetDoc commands

BOOL CTopoNetDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return m_net.Save(lpszPathName)	;
}

BOOL CTopoNetDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	if(!m_net.Open (lpszPathName))return false;
	m_nTopoPoint=m_nTopoPointLimit=m_net.GetPointNumber();
	
	CMy3DPoint point;
	for(int i=0;i<m_net.GetPointNumber();i++){
		point=m_net.GetPoint(i);
		if(point.x==0&&point.y==0){
			m_nTopoPoint=i;
			break;
		}
	}
	
	return TRUE;
}
