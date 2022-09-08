#if !defined(AFX_SPPDOC_H__8F8D9023_35D2_11D3_9C0E_006097EFA52E__INCLUDED_)
#define AFX_SPPDOC_H__8F8D9023_35D2_11D3_9C0E_006097EFA52E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SPPDoc.h : header file
//
#include "SPP.h"
#include "FHEditDoc.h"

/////////////////////////////////////////////////////////////////////////////
// CSPPDoc document
class CSPPDoc : public CFHEditDoc
{
friend class CSPPView;
public:
	CSPPDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSPPDoc)

// Attributes
public:
	public:
	long m_TotalSNumber;
	ShotPointPar *m_pSPP;   // Used to store the parameters of the shots of one swath.
	long m_nShotLimit;

	BOOL Read();
	BOOL Write();

	long GetShotNumber();
	
	BOOL ListNotFoundPh(long *NotFoundPh,long NotFoundPhNumber,CString Ext);
	long SearchPhInSPP(long WantedPh);

private:
	long m_SwathNumber;
	CString m_fileShotPointPar;
	
	void SortSwath();
	BOOL PlusPh(int ,CString sFile="");
	bool PlusKF(CString sFile="");
	BOOL PlusFileNumber(CString sFile="");
	BOOL PlusOffset(CString sFile="");
	BOOL PlusCoor(CString sFile="");

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSPPDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnNewDocument();
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL PlusTrace(CString sFile);
	bool Set(CString fileSelected);
	BOOL Set(int nSwath);
	virtual ~CSPPDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CSPPDoc)
	afx_msg void OnSPPIntegrate();
	afx_msg void OnSPPPlusFileNumber();
	afx_msg void OnSPPPlusKF();
	afx_msg void OnSPPPlusCoor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPPDOC_H__8F8D9023_35D2_11D3_9C0E_006097EFA52E__INCLUDED_)
