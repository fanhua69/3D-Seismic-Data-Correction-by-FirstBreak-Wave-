#if !defined(AFX_TDOC_H__61469AA1_63E6_11D1_B6D1_444553540000__INCLUDED_)
#define AFX_TDOC_H__61469AA1_63E6_11D1_B6D1_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TDoc.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CTDoc document

#include "MidFbkFile.h"
#include "OutFbkFile.h"
#include "SndFile.h"


#include "pxyv.h"
#include "CheckT.h"  // 对话框.
#include "Swathpar.h"

class CTDoc : public CDocument
{
protected:
	CTDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTDoc)

protected:
	SvSys m_ss;
	SwathParameter m_sp;
	CMidFbkFile m_FileMidFbk;
	COutFbkFile m_FileOutFbk;
	CSndFile m_FileSnd;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTDoc();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	void Reset();

	bool DrawLine();
	
	// 被检查数据的状态：
	int m_bDrawOnShot;
	int m_bCheckShotOrRcv;
	int m_ShotName;
	int m_RecieveName;	

	//{{AFX_MSG(CTDoc)
	afx_msg void OnTChoose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	PXYV *m_pSndLine;   // 被显示的部分数据。
	PXYV *m_pMidFbkLine;
	PXYV *m_pOutFbkLine;

	long m_nSndLine;
	long m_nMidFbkLine;
	long m_nOutFbkLine;

	// 当前所画的线的名称：
	CString m_sOutFbkLine;
	CString m_sMidFbkLine;
	CString m_sSndLine;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TDOC_H__61469AA1_63E6_11D1_B6D1_444553540000__INCLUDED_)
