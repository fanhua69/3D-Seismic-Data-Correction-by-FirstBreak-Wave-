#if !defined(AFX_FBKPUREVIEW_H__8C5E7845_0EC3_11D4_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_FBKPUREVIEW_H__8C5E7845_0EC3_11D4_A4E4_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FbkPureView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFbkPureView view
#include "FHZoomView.h"
#include "FbkPureDoc.h"

class CFbkPureView : public CFHZoomView
{
protected:
	CFbkPureView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFbkPureView)

private:
	long m_nCurrentShot;
	bool m_bAnchor;
	long m_nGroupAnchored;
	double m_dAnchorPos;

	CPen m_penCommon;
	CPen m_penAnchored;
	CBrush m_brushCommon;
	CBrush m_brushAnchored;



// Attributes
public:

// Operations
public:
	CString GetInfo(double x,double y);
	void GotoPrevPage();
	void GotoNextPage();
	CFbkPureDoc * GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFbkPureView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFbkPureView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFbkPureView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnFbkPureFindFileNumber();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FBKPUREVIEW_H__8C5E7845_0EC3_11D4_A4E4_00C04FCCB957__INCLUDED_)
