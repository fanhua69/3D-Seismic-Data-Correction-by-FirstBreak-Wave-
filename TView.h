#if !defined(AFX_TVIEW_H__61469AA2_63E6_11D1_B6D1_444553540000__INCLUDED_)
#define AFX_TVIEW_H__61469AA2_63E6_11D1_B6D1_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTView view
class CTDoc;
#include "FHZoomView.h"

class CTView : public CFHZoomView
{
protected:
	CTView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTView)

// Attributes
public:

// Operations
public:
	CString GetInfo(double x,double y);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CPen m_penMidFbk;
	CPen m_penOutFbk;
	CPen m_penSnd;

	CTDoc* GetDocument();
	virtual ~CTView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTView)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TVIEW_H__61469AA2_63E6_11D1_B6D1_444553540000__INCLUDED_)
