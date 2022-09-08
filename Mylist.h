#if !defined(AFX_MYLIST_H__EA5711A1_3B67_11D1_AF86_444553540000__INCLUDED_)
#define AFX_MYLIST_H__EA5711A1_3B67_11D1_AF86_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MyList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyList dialog
#define ListMaxNumber 300
class CMyList : public CDialog
{
// Construction
public:
	CMyList(CWnd* pParent = NULL);   // standard constructor
	long Data[ListMaxNumber];
	long DataNumber;
	long Selected;
	CString CaptainText;
	CString ContentText;


// Dialog Data
	//{{AFX_DATA(CMyList)
	enum { IDD = IDD_ListBox };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMyList)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListBox();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLIST_H__EA5711A1_3B67_11D1_AF86_444553540000__INCLUDED_)
