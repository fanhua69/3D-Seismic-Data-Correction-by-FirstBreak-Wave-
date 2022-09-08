// DpgNum.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDpgNum dialog

class CDpgNum : public CDialog
{
// Construction
public:
	CDpgNum(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDpgNum)
	enum { IDD = IDD_PageNumber };
	UINT	m_PageNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDpgNum)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDpgNum)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
