#if !defined(AFX_FBKDOC_H__05625E41_5202_11D1_B6D1_A139AD15E33C__INCLUDED_)
#define       AFX_FBKDOC_H__05625E41_5202_11D1_B6D1_A139AD15E33C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FbkDoc.h : header file
//

#include "neunet.h"
#include "Unform.h"
#include "fbk.h"
#include "relation.h"

/////////////////////////////////////////////////////////////////////////////
// CFbkDoc document
class CGroupLine
{
public:
	int nSequence;
	int nStartGroup;
	int nEndGroup;
	int nStartGroupName;
	int nEndGroupName;

	CGroupLine(){
		nSequence=0;
		nStartGroup=nEndGroup=0;
		nStartGroupName=nEndGroupName=0;
	} ;
};




class CFbkDoc : public CDocument,UnformFile
{
protected:
	int m_nWindowLen; // used when read fbk precisely;

friend class CFbkView;
public:
	// ��ҳ���йصĲ���.
	long  m_CurrShot;
	long m_GroupNumber;	  // The Group Number in One Shot.
		
	// �뻭������йصĲ���.
	float m_InflatingCoefficient;

	//
	int m_nHeadNumber;

	long m_DataRoomSize;
	float *m_pGroupData;             // used to store data of one page.

	DataInFstBrkFile *m_pDataInFstBrkFile;
	CPoint *m_pPointOfOneGroup;   //  Used when drawing one page.

	// ��ѡ���йصı���.
	BOOL *m_pSelectedGroups;
	int m_BeginSelectedGroup;
	int m_EndSelectedGroup;

	// һ����������Ķ���, �Ա�ѧϰ,����.
	NeuNet m_neunet;
	Fbk m_fbk;

	//
	CRelation m_relation;

	// ��Ա����.
	BOOL ReadData();
	BOOL SaveFstBrk();
	BOOL AskSearchedPageNumber();
	BOOL AskCalculatedGroup(long  *, long  *);
	void AdjustGroupData(float coef=0);

	// Data to split one shot into lines of group:
	int m_nGroupLineLimit; //=100
	int m_nGroupLine;
	CGroupLine m_GroupLine[100];
	bool SetGroupLine();

public:

	// Main Bode Code:
	BOOL Set(CString sFile);
	BOOL NeuNetCalculate();
	BOOL NeuNetLearn();

	void MakeFBD(int Swath);
	void MakeGKHFBD();


public:
	// Construction Code:
	CFbkDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFbkDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFbkDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	bool CalcFbkInteractively(CMyPoint pointHead,CMyPoint pointTail);
	long GetGroupLineName(int  nGroup);
	virtual ~CFbkDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
public:
	long GetGroupHead(long nGroup,int nHead);

	//{{AFX_MSG(CFbkDoc)
	afx_msg void OnFBCalc();
	afx_msg void OnFBEndPage();
	afx_msg void OnFBFirstPage();
	afx_msg void OnFBLastPage();
	afx_msg void OnFBLearn();
	afx_msg void OnFBNextPage();
	afx_msg void OnFBSearchPage();
	afx_msg void OnFileMakeFromText();
	afx_msg void OnFileMakeFromDBF();
	afx_msg void OnUpdateFBCalc(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFBClose(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFBEndPage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFBFirstPage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFBLastPage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFBLearn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFBNextPage(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFBSearchPage(CCmdUI* pCmdUI);
	afx_msg void OnFBRelativeCalc();
	afx_msg void OnFBPreciseRead();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif   // !defined(AFX_FBKDOC_H__05625E41_5202_11D1_B6D1_A139AD15E33C__INCLUDED_)
