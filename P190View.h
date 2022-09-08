#if !defined(AFX_P190VIEW_H__71D647C2_0AD4_11D4_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_P190VIEW_H__71D647C2_0AD4_11D4_A4E4_00C04FCCB957__INCLUDED_

#include "P190Doc.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// P190View.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CP190View view
#include "FHZoomView.h"

class CP190View : public CFHZoomView
{
protected:
	CP190View();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CP190View)

protected:
	double m_dMinX;
	double m_dMinY;
	double m_dMaxX;
	double m_dMaxY;

	long m_nShotCurrent;



// Attributes
public:

// Operations
public:
	CString GetInfo(double x,double y);
	CP190Doc *GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CP190View)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CP190View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CP190View)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_P190VIEW_H__71D647C2_0AD4_11D4_A4E4_00C04FCCB957__INCLUDED_)
