#if !defined(AFX_CONTROLDOC_H__0DDEAEC1_15D2_11D4_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_CONTROLDOC_H__0DDEAEC1_15D2_11D4_A4E4_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ControlDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CControlDoc document
#include "FHEditDoc.h"
#include "ControlFile.h"

class CControlDoc : public CFHEditDoc
{
protected:
	CControlDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CControlDoc)

protected:
	


// Attributes
public:
	CControlFile m_CtlFile;

// Operations
public:



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CControlDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROLDOC_H__0DDEAEC1_15D2_11D4_A4E4_00C04FCCB957__INCLUDED_)
