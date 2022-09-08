// dswnum.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDSwNum dialog
#ifndef tagCDSwNum
#define tagCDSwNum
class CDSwNum : public CDialog
{
// Construction
public:
	CDSwNum(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDSwNum)
	enum { IDD = IDD_DialogSwathNumber };
	long	m_SwathNumber;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDSwNum)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif