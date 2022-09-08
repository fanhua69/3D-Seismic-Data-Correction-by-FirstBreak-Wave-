#if !defined(AFX_TOPONETDOC_H__A5C37861_0FE7_11D4_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_TOPONETDOC_H__A5C37861_0FE7_11D4_A4E4_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TopoNetDoc.h : header file
//

#include "TopoNet.h"

/////////////////////////////////////////////////////////////////////////////
// CTopoNetDoc document

class CTopoNetDoc : public CDocument
{
protected:
	CTopoNetDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTopoNetDoc)

// Attributes
public:
	CTopoNet m_net;
	int m_nTopoPoint;
	int m_nTopoPointLimit;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTopoNetDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTopoNetDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CTopoNetDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOPONETDOC_H__A5C37861_0FE7_11D4_A4E4_00C04FCCB957__INCLUDED_)
