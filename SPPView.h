#if !defined(AFX_SPPVIEW_H__8F8D9024_35D2_11D3_9C0E_006097EFA52E__INCLUDED_)
#define AFX_SPPVIEW_H__8F8D9024_35D2_11D3_9C0E_006097EFA52E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SPPView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSPPView view
#include "afxcmn.h"
#include "FHEditView.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CSPPView : public CFHEditView
{
protected:
	CSPPView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSPPView)

// Attributes
public:

// Operations
public:
	void UpdateGrid();
	void UpdateDoc();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSPPView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSPPView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CSPPView)
	//}}AFX_MSG
	CSPPDoc* GetDocument();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPPVIEW_H__8F8D9024_35D2_11D3_9C0E_006097EFA52E__INCLUDED_)
