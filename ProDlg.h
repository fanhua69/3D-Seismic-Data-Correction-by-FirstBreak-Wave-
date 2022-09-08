// ProDlg.h : header file
// CG: This file was added by the Progress Dialog component

/////////////////////////////////////////////////////////////////////////////
// CProDlg dialog

#ifndef __PRODLG_H__
#define __PRODLG_H__

#include "afxcmn.h"

class CProDlg : public CDialog
{
// Construction / Destruction
public:
    CProDlg(UINT nCaptionID = 0);   // standard constructor
    ~CProDlg();

    BOOL Create(CWnd *pParent=NULL);

    // Checking for Cancel button
    BOOL CheckCancelButton();
    // Progress Dialog manipulation
    void SetStatus(LPCTSTR lpszMessage);
    void SetRange(int nLower,int nUpper);
    int  SetStep(int nStep);
    int  SetPos(int nPos);
    int  OffsetPos(int nPos);
    int  StepIt();
        
// Dialog Data
    //{{AFX_DATA(CProDlg)
    enum { IDD = CG_IDD_PROGRESS };
    CProgressCtrl	m_Progress;
    //}}AFX_DATA

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CProDlg)
    public:
    virtual BOOL DestroyWindow();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
	UINT m_nCaptionID;
    int m_nLower;
    int m_nUpper;
    int m_nStep;
    
    BOOL m_bCancel;
    BOOL m_bParentDisabled;

    void ReEnableParent();

    virtual void OnCancel();
    virtual void OnOK() {}; 
    void UpdatePercent(int nCurrent);
    void PumpMessages();

    // Generated message map functions
    //{{AFX_MSG(CProDlg)
    virtual BOOL OnInitDialog();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif // __PRODLG_H__
