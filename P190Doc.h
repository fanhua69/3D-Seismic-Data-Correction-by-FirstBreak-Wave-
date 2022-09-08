#if !defined(AFX_P190DOC_H__71D647C1_0AD4_11D4_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_P190DOC_H__71D647C1_0AD4_11D4_A4E4_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// P190Doc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CP190Doc document
#include "InP190.h"

class CP190Doc : public CDocument
{

	friend class CP190View;

protected:
	CP190Doc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CP190Doc)

// Attributes
public:

private:
	CInP190 m_P190;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CP190Doc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CP190Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CP190Doc)
	afx_msg void OnP190FormatChange();
	afx_msg void OnP190SaveRcvStaticValue();
	afx_msg void OnP190SaveShotStaticValue();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_P190DOC_H__71D647C1_0AD4_11D4_A4E4_00C04FCCB957__INCLUDED_)
