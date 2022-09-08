// FbkPureDoc.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "FbkPureDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFbkPureDoc

IMPLEMENT_DYNCREATE(CFbkPureDoc, CDocument)

CFbkPureDoc::CFbkPureDoc()
{
	m_pShotFbk=NULL;   // Do not delete this pointer, its NOT in this document;
	m_nGroupNumberInCurShot=0;
	m_nCurrentShotFileNumber=0;
}

BOOL CFbkPureDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CFbkPureDoc::~CFbkPureDoc()
{
}


BEGIN_MESSAGE_MAP(CFbkPureDoc, CDocument)
	//{{AFX_MSG_MAP(CFbkPureDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFbkPureDoc diagnostics

#ifdef _DEBUG
void CFbkPureDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFbkPureDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFbkPureDoc serialization

void CFbkPureDoc::Serialize(CArchive& ar)
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
// CFbkPureDoc commands

BOOL CFbkPureDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here

	return m_fbk.Open (lpszPathName);
}

bool CFbkPureDoc::SetShot(long nShotOrder,bool bFileNumber)
{
	if(bFileNumber){
		long n=-1;
		for(long i=0;i<m_fbk.m_nShotNumber;i++){
			if(m_fbk.m_pFbkMsg [i].nFileNumber ==nShotOrder){
				n=i;
				break;
			}
		}
		if(n!=-1){			
			m_nCurrentShotOrder=n;
			m_nCurrentShotFileNumber=nShotOrder;
			m_pShotFbk=m_fbk.GetShotFbk (nShotOrder,m_nGroupNumberInCurShot);
		}
		else{
			AfxMessageBox("Can not find the file number!");
		}
		return m_pShotFbk;
	}
	else{
		if(nShotOrder<0||nShotOrder>=m_fbk.m_nShotNumber )
		return false;

		m_nCurrentShotOrder=nShotOrder; 
		m_nCurrentShotFileNumber=m_fbk.m_pFbkMsg [m_nCurrentShotOrder].nFileNumber ;
		m_pShotFbk=m_fbk.GetShotFbk (m_nCurrentShotFileNumber,m_nGroupNumberInCurShot);
		return (m_pShotFbk!=NULL);
	}

}
