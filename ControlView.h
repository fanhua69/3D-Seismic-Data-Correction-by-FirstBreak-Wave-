#if !defined(AFX_CONTROLVIEW_H__0DDEAEC2_15D2_11D4_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_CONTROLVIEW_H__0DDEAEC2_15D2_11D4_A4E4_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ControlView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CControlView view
#include "FHEditView.h"
#include "Controldoc.h"

class CControlView : public CFHEditView
{
protected:
	CControlView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CControlView)

// Attributes
public:

// Operations
public:
	void UpdateDoc();
	CControlDoc * GetDocument();
	void UpdateGrid();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CControlView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CControlView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROLVIEW_H__0DDEAEC2_15D2_11D4_A4E4_00C04FCCB957__INCLUDED_)
