#if !defined(AFX_TESTVIEW_H__512EB267_3117_11D3_9C0E_006097EFA52E__INCLUDED_)
#define AFX_TESTVIEW_H__512EB267_3117_11D3_9C0E_006097EFA52E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// testView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// testView view

class testView : public CScrollView
{
protected:
	testView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(testView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(testView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~testView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(testView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTVIEW_H__512EB267_3117_11D3_9C0E_006097EFA52E__INCLUDED_)
