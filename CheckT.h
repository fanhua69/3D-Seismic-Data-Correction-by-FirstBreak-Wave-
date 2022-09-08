#if !defined(AFX_CHECKT_H__5C58DD61_643F_11D1_B6D1_444553540000__INCLUDED_)
#define AFX_CHECKT_H__5C58DD61_643F_11D1_B6D1_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CheckT.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCheckT dialog

class CCheckT : public CDialog
{
// Construction
public:
	CCheckT(CWnd* pParent = NULL);   // standard constructor
	~CCheckT();

	long *m_RecieveNameList;
	long *m_ShotPointNameList;
	
	long m_RecieveNameNumber;
	long m_ShotNameNumber;	
	long m_ShotLineFrom;
	long m_ShotLineTo;

// Dialog Data
	//{{AFX_DATA(CCheckT)
	enum { IDD = IDD_CheckT };
	CButton	m_ListShot;
	CButton	m_ListRecieve;
	UINT	m_RecieveName;
	UINT	m_ShotName;
	int		m_bCheckShotOrRcv;
	int		m_bDrawOnShotOrRcv;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckT)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCheckT)
	afx_msg void OnListRecieve();
	afx_msg void OnListShot();
	afx_msg void OnCheckRecieve();
	afx_msg void OnCheckShot();
	afx_msg void OnDrawOnShotPoint();
	afx_msg void OnDrawOnShotLine();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKT_H__5C58DD61_643F_11D1_B6D1_444553540000__INCLUDED_)
