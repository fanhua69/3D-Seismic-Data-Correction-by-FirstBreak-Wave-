#if !defined(AFX_CDSWPAR_H__A34CFB81_3B0D_11D1_AF86_444553540000__INCLUDED_)
#define AFX_CDSWPAR_H__A34CFB81_3B0D_11D1_AF86_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// cdswpar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// cdswpar dialog

class cdswpar : public CDialog
{
// Construction
public:
	cdswpar(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(cdswpar)
	enum { IDD = IDD_SwathParameter };
	UINT	m_Distance2;
	UINT	m_Distance1;
	UINT	m_Distance3;
	UINT	m_Distance4;
	UINT	m_FirstRecievePointNumber;
	UINT	m_FoldTime;
	UINT	m_InitialVelocity;
	UINT	m_ShotLineFrom;
	UINT	m_ShotLineTo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cdswpar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cdswpar)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDSWPAR_H__A34CFB81_3B0D_11D1_AF86_444553540000__INCLUDED_)
