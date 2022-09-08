#if !defined(AFX_TOPONETVIEW_H__A5C37862_0FE7_11D4_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_TOPONETVIEW_H__A5C37862_0FE7_11D4_A4E4_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TopoNetView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTopoNetView view
#include "TopoNet.h"
#include "ColorDlg.h"
#include "TopoNetDoc.h"
#include "FHZoomView.h"

class CTopoNetView : public CFHZoomView
{
protected:
	CTopoNetView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTopoNetView)

// Attributes
public:
	double m_dLarger;
	CColorDlg m_dlgColor;

	int m_nValuePoint;
	int m_nValuePointLimit;
	double *m_dValuePoint;
	CMy3DPoint m_Point3D[3];
	CMyPrcPoint m_pointWanted;



// Operations
public:
	CString GetInfo(double x,double y);
	CTopoNetDoc* GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTopoNetView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTopoNetView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTopoNetView)
	afx_msg void OnTestTopoNet();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOPONETVIEW_H__A5C37862_0FE7_11D4_A4E4_00C04FCCB957__INCLUDED_)
