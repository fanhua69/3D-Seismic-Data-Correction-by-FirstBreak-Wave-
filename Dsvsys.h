// dsvsys.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDSvSys dialog

class CDSvSys : public CDialog
{
// Construction
public:
	CDSvSys(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDSvSys)
	enum { IDD = IDD_DialogSvSys };
	CString	m_Area;
	CString	m_Crew;
	long	m_GapOfSmallNumber;
	long	m_GapOfBigNumber;
	long	m_GroupInterval;
	long	m_GroupNumberOfBigNumber;
	long	m_GroupNumberOfSmallNumber;
	long	m_RecieveLineNumber;
	long	m_ShotLineInterval;
	long	m_ShotPointNumber;
	long	m_FirstShotPointPosition;
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CDSvSys)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
