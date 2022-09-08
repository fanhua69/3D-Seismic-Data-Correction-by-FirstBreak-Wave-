#if !defined(AFX_FBKVIEW_H__51A1A541_4F6E_11D1_B6D1_A139AD15E33C__INCLUDED_)
#define AFX_FBKVIEW_H__51A1A541_4F6E_11D1_B6D1_A139AD15E33C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FbkView.h : header file
//
class CFbkDoc;
class DataInFstBrkFile;

#include "MyPoint.h"

/////////////////////////////////////////////////////////////////////////////
// CFbkView view
#include "FHZoomView.h"

class CFbkView : public CFHZoomView
{
protected:
	void CalcMousePos(CPoint point,long &nPointedGroup,double &dPointedTime);

protected:
	

	CFbkView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFbkView)

	CPen m_penFbk,m_penGroup;
	CPen m_penSelection,m_penWrongFbk;
	CPen m_penCleaner;


	BOOL  *m_AbFbk;  // 判断某道是否初至错误。
	BOOL  *m_AbFbkLast;  // 判断某道是否初至错误。
	
	BOOL *m_GroupChanged;  // 用于鼠标左键按下。

	long *m_UnconGroup;  // 不连续道。
	long m_nUnconGroup;  // 不连续道数目。

	BOOL m_DrawCurve;
	BOOL m_DrawFbk;

	// related to modify the fbk manual-machinely:
	bool m_bModifyingFbk; // indicating that the user is modifying the fbk by a line;

	bool m_bAnchor;
	CMyPoint m_pointHead;
	CMyPoint m_pointTail;




	////////////////////////////////////////
	// For checking the first breaks:
	int m_nPenLineFBK; // =14
	CPen m_penLineFBK[15]; 
	bool m_bCheckingFBK;
	bool m_bDrawPartGroup;

	bool m_bAreaShow;

	double m_dFatScale;


// Attributes
public:

// Operations
public:
	void GotoNextPage();
	void GotoPrevPage();

	CString GetInfo(double x,double y);
	enumZoomStatue GetZoomStatue();
	BOOL CheckFirstBreak();
	CFbkDoc* GetDocument();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFbkView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFbkView();
	BOOL PrepareScreen(CDC *,BOOL CLS=FALSE);
	BOOL DrawGroup(CDC *pDC);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	// Generated message map functions
	//{{AFX_MSG(CFbkView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnFBDrawCurve();
	afx_msg void OnFBDrawFbk();
	afx_msg void OnFbkCheckFBK();
	afx_msg void OnUpdateFbkCheckFBK(CCmdUI* pCmdUI);
	afx_msg void OnFbkAreaShow();
	afx_msg void OnUpdateFbkAreaShow(CCmdUI* pCmdUI);
	afx_msg void OnFstBrkDrawPart();
	afx_msg void OnUpdateFstBrkDrawPart(CCmdUI* pCmdUI);
	afx_msg void OnFBThinner();
	afx_msg void OnFBFatter();
	afx_msg void OnFBModify();
	afx_msg void OnUpdateFBModify(CCmdUI* pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void Reset();
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FBKVIEW_H__51A1A541_4F6E_11D1_B6D1_A139AD15E33C__INCLUDED_)
