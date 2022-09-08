#if !defined(AFX_ASKGROUP_H__8E86BE41_3DC2_11D1_AF86_444553540000__INCLUDED_)
#define AFX_ASKGROUP_H__8E86BE41_3DC2_11D1_AF86_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// AskGroup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAskGroup dialog

class CAskGroup : public CDialog
{
// Construction
public:
	CAskGroup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAskGroup)
	enum { IDD = IDD_AskCalculateGroup };
	UINT	m_BN;
	UINT	m_EN;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAskGroup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAskGroup)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASKGROUP_H__8E86BE41_3DC2_11D1_AF86_444553540000__INCLUDED_)
