#if !defined(AFX_TESTDOC_H__512EB266_3117_11D3_9C0E_006097EFA52E__INCLUDED_)
#define AFX_TESTDOC_H__512EB266_3117_11D3_9C0E_006097EFA52E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// testDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// testDoc document

class testDoc : public CDocument
{
protected:
	testDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(testDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(testDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~testDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(testDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTDOC_H__512EB266_3117_11D3_9C0E_006097EFA52E__INCLUDED_)
