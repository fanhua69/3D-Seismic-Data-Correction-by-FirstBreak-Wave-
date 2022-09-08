// FbkInFile.h: interface for the CFbkInFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FBKINFILE_H__12487FE4_06DC_11D4_A4E4_00C04FCCB957__INCLUDED_)
#define AFX_FBKINFILE_H__12487FE4_06DC_11D4_A4E4_00C04FCCB957__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFbkMsg
{
public:
	long nFileNumber;
	long nStartGroup;  // of the total group in the file;
	long nEndGroup;    // of the total group in the file;
	long nGroupNumber;
};

class CFbkInFile  
{
public:
	bool ShowMsg(CString sFile);
	DataInFstBrkFile * GetShotFbk(long nFileNumber, long &nGroupNumber);
	long GetFileNumber(long nGroup);
	bool Reset();
	bool Open(CString sFile);
	CFbkInFile();
	virtual ~CFbkInFile();

	CString GetFileName(){return m_sFileName;};
	long GetMaxGroupNumber(){return m_nMaxGroupNumber;};

protected:
	bool Analyse();


	FILE *m_fp;
	long m_nFileLen;
	long m_nMaxGroupNumber;
	CString m_sFileName;

public:
	bool WriteShotFbk();
	CFbkMsg *m_pFbkMsg;
	long m_nShotLimit; 

	DataInFstBrkFile *m_pShotFbk;

	long m_nSmallFileNumber;
	long m_nBigFileNumber;
	long m_nShotNumber;

	long m_nGroupNumber;
	
};

#endif // !defined(AFX_FBKINFILE_H__12487FE4_06DC_11D4_A4E4_00C04FCCB957__INCLUDED_)
