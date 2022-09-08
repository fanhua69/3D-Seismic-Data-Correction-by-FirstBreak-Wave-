#if !defined(AFX_FBKPUREDOC_H__8C5E7844_0EC3_11D4_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_FBKPUREDOC_H__8C5E7844_0EC3_11D4_A4E4_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FbkPureDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFbkPureDoc document

#include "fbk.h"
#include "FbkInFile.h"

class CFbkPureDoc : public CDocument
{
protected:
	CFbkPureDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFbkPureDoc)

public:

	CFbkInFile m_fbk;

	DataInFstBrkFile *m_pShotFbk;
	long m_nCurrentShotOrder;
	long m_nCurrentShotFileNumber;
	long m_nGroupNumberInCurShot;
	


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFbkPureDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	bool SetShot(long nShot,bool bFileNumber=false);
	virtual ~CFbkPureDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CFbkPureDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FBKPUREDOC_H__8C5E7844_0EC3_11D4_A4E4_00C04FCCB957__INCLUDED_)
