// InP190.cpp: implementation of the CInP190 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3dfstbrk.h"
#include "InP190.h"
#include "stdio.h"
#include "ProDlg.h"
#include "ProgDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInP190::CInP190()
{

}

CInP190::~CInP190()
{

}

bool CInP190::Open(CString sFile)
{
	if(sFile==""){
		CFileDialog dlg(true);
		dlg.m_ofn.lpstrFilter="The control point file(*.SRS, *.GEO, *.REL)\0*.GEO;*.SRS;*.REL";
		dlg.m_ofn.lpstrTitle="Open the control point";
		dlg.m_ofn.lpstrDefExt=".";
		
		if(dlg.DoModal()==IDCANCEL)return false;
		sFile=dlg.GetPathName();
	}

	int n=sFile.ReverseFind ('.');
	sFile=sFile.Left (n);
	CString sShotFile=sFile+".SRS";
	CString sRcvFile=sFile+".GEO";
	CString sRelFile=sFile+".REL";

	if(!ReadShotPar(sShotFile))return false;
	if(!ReadRcvPar(sRcvFile))return false;
	if(!ReadShotRcvRel(sRelFile))return false;

	return true;
}

bool CInP190::ReadRcvPar(CString sRcvFile)
{
	if(sRcvFile==""){
		CFileDialog dlg(true);
		dlg.m_ofn.lpstrFilter="P190 Reciever File (*.GEO)\0*.GEO";
		dlg.m_ofn.lpstrTitle="Open P190 Reciever File";
		dlg.m_ofn.lpstrDefExt=".GEO";
		
		if(dlg.DoModal()==IDCANCEL)return false;
		sRcvFile=dlg.GetPathName();
	}
	if(sRcvFile=="")return false;



	FILE *fp=fopen(sRcvFile,"rt");
	if(!fp){
		AfxMessageBox("Can not open the P190 recieve point file "+sRcvFile);
		return false;
	}

	// Get the shot point number:
	char line[101];
	long nRcv=0;
	while(!feof(fp)){
		fgets(line,100,fp);
		if(line[0]=='G'){
			nRcv++;
		}		
	}
	fclose (fp);
	m_nRcvPhyNumber=nRcv;

	// Read the shot par:
	if(m_pRcvPhyPar){
		delete []m_pRcvPhyPar;
		m_pRcvPhyPar=NULL;
	}
	m_pRcvPhyPar=new CRcvPhyPar[nRcv];

	
	CProDlg dlg;
	dlg.Create();
	dlg.SetStatus ("Reading Reciever Parameters...");
	
	
	fp=fopen(sRcvFile,"rt");
	
	nRcv=0;
	
	CString sLine, sMid;
	float fScale;
	while(!feof(fp)){
		fScale=(nRcv/(float)m_nRcvPhyNumber)*100;
		dlg.SetPos(fScale);

		fgets(sLine.GetBuffer (200),200,fp);
		sLine.ReleaseBuffer ();
		if(sLine.Left (1)=='G'){
			m_pRcvPhyPar[nRcv].Ph =atol(LPCTSTR(sLine.Mid(19,6)));
			m_pRcvPhyPar[nRcv].east  =atof(LPCTSTR(sLine.Mid(46,9)));
			m_pRcvPhyPar[nRcv].north  =atof(LPCTSTR(sLine.Mid(55,9)));
			m_pRcvPhyPar[nRcv].gd =atof(LPCTSTR(sLine.Mid(25,10)));
			m_pRcvPhyPar[nRcv].h  =atof(LPCTSTR(sLine.Mid(64,6)));
			m_pRcvPhyPar[nRcv].rt =atoi(LPCTSTR(sLine.Mid(75,4)));
			nRcv++;
		}		
	}
	fclose (fp);
	dlg.SetPos(100);

	SortRcvPoint();

	dlg.DestroyWindow ();

	return true;


}

//////////////////////////////////////////
// Read the relation file:
bool CInP190::ReadShotRcvRel(CString sRelFile)
{
	if(sRelFile==""){
		CFileDialog dlg(true);
		dlg.m_ofn.lpstrFilter="P190 Relation File (*.REL)\0*.REL";
		dlg.m_ofn.lpstrTitle="Open P190 Relation File";
		dlg.m_ofn.lpstrDefExt=".REL";
		
		if(dlg.DoModal()==IDCANCEL)return false;
		sRelFile=dlg.GetPathName();
	}
	if(sRelFile=="")return false;

	
	FILE *fp=fopen(sRelFile,"rt");
	if(!fp){
		AfxMessageBox("Can not open the P190 relation file "+sRelFile);
		return false;
	}

	// Get the relation number:
	char line[101];	  
	char ph[7];
	ph[6]='\0';
	long nRel=0;
	while(!feof(fp)){
		fgets(line,100,fp);
		if(line[0]=='X'){
			strncpy(ph,&line[19],6);
			nRel=1;
			break;
		}		
	}
	
	char phthis[7];
	phthis[6]='\0';
	while(!feof(fp)){
		fgets(line,100,fp);
		if(line[0]=='X'){
			strncpy(phthis,&line[19],6);
			if(strcmp(ph,phthis)){
				strcpy(ph,phthis);
				nRel++;
			}
		}		
	}
	fclose (fp);
	m_nRelNumber=nRel;


	// Read the shot par:
	if(m_pShotRcvRel){
		delete []m_pShotRcvRel;
		m_pShotRcvRel=NULL;
	}
	m_pShotRcvRel=new CShotRcvRel[nRel];

	
	fp=fopen(sRelFile,"rt");

	
	CProDlg dlg;
	dlg.Create();
	dlg.SetStatus ("Reading Relation Data...");


	
	nRel=0;	
	CString sLine;
	int nRcvLine;

	// Find the first shot point relation line:
	while(!feof(fp)){
		fgets(sLine.GetBuffer (200),200,fp);
		sLine.ReleaseBuffer ();
		if(sLine.Left (1)=='X'){
			m_pShotRcvRel[0].PhShot=atol(LPCTSTR(sLine.Mid(19,6)));		
			m_pShotRcvRel[0].FileNumber =atol(LPCTSTR(sLine.Mid(57,4)));

			
			nRcvLine=0;
			m_pShotRcvRel[0].RcvRange [0].start =atol(LPCTSTR(sLine.Mid(40,6)));
			m_pShotRcvRel[0].RcvRange [0].end=atol(LPCTSTR(sLine.Mid(46,6)));

			break;
		}
	}

	// Continue to read the relations:
	long phShot;
	float fScale;
	int nStep=m_nRelNumber/100;
	while(!feof(fp)){
		if(nRel%nStep==0){
			fScale=(nRel/(float)m_nRelNumber)*100;
			dlg.SetPos(fScale);
		}

		fgets(sLine.GetBuffer (200),200,fp);
		sLine.ReleaseBuffer ();
		if(sLine.Left (1)=='X'){
			phShot=atol(LPCTSTR(sLine.Mid(19,6)));
			if(phShot!=m_pShotRcvRel[nRel].PhShot){
				m_pShotRcvRel[nRel].nRcvLine =nRcvLine+1;

				nRel++;
				nRcvLine=0;
				m_pShotRcvRel[nRel].PhShot=phShot;
				m_pShotRcvRel[nRel].FileNumber =atol(LPCTSTR(sLine.Mid(57,4)));

				m_pShotRcvRel[nRel].RcvRange [0].start =atol(LPCTSTR(sLine.Mid(40,6)));
				m_pShotRcvRel[nRel].RcvRange [0].end=atol(LPCTSTR(sLine.Mid(46,6)));
			}
			else{
				nRcvLine++;
				if(nRcvLine>=m_pShotRcvRel[nRel].nRcvLineLimit ){
					AfxMessageBox("Too much recieve lines , error?!");
					break;
				}
				m_pShotRcvRel[nRel].RcvRange [nRcvLine].start =atol(LPCTSTR(sLine.Mid(40,6)));
				m_pShotRcvRel[nRel].RcvRange [nRcvLine].end=atol(LPCTSTR(sLine.Mid(46,6)));
			}
		}
	}
	m_pShotRcvRel[nRel].nRcvLine=nRcvLine+1;

	// Calculate how many rcv points are there in one shot:
	// and set space for the rcv points to retur to the user:
	long nMaxRcv=-99999999;
	long i,j,n;
	for(i=0;i<m_nRelNumber;i++){
		n=0;
		for(j=0;j<m_pShotRcvRel[i].nRcvLine ;j++){
			n+=m_pShotRcvRel[i].RcvRange [j].end -m_pShotRcvRel[i].RcvRange [j].start+1;
		}
		if(n>=nMaxRcv)nMaxRcv=n;
	}

	if(m_pPointOfOneShot){
		delete []m_pPointOfOneShot;
		m_pPointOfOneShot=NULL;
	}

	m_pPointOfOneShot=new CMyPoint[nMaxRcv];

	
	//
	fclose (fp);

	dlg.SetPos(100);
	dlg.DestroyWindow ();

	return true;

}

bool CInP190::ReadShotPar(CString sShotFile)
{
	if(sShotFile==""){
		CFileDialog dlg(true);
		dlg.m_ofn.lpstrFilter="P190 Shot File (*.SRS)\0*.SRS";
		dlg.m_ofn.lpstrTitle="Open P190 Shot File";
		dlg.m_ofn.lpstrDefExt=".SRS";
		
		if(dlg.DoModal()==IDCANCEL)return false;
		sShotFile=dlg.GetPathName();
	}
	if(sShotFile=="")return false;


	
	FILE *fp=fopen(sShotFile,"rt");
	if(!fp){
		AfxMessageBox("Can not open the P190 file "+sShotFile);
		return false;
	}

	// Get the shot point number:
	char line[101];
	long nShot=0;
	while(!feof(fp)){
		fgets(line,100,fp);
		if(line[0]=='S'){
			nShot++;
		}		
	}
	fclose (fp);
	m_nShotPhyNumber=nShot;

		
	CProDlg dlg;
	if(!dlg.Create())return false;
	dlg.SetStatus ("Reading Shot Parameters...");


	// Read the shot par:
	if(m_pShotPhyPar){
		delete []m_pShotPhyPar;
		m_pShotPhyPar=NULL;
	}
	m_pShotPhyPar=new CShotPhyPar[nShot];

	
	fp=fopen(sShotFile,"rt");
	
	nShot=0;
	
	CString sLine, sMid;
	float fScale;
	while(!feof(fp)){
		fScale=(nShot/(float)m_nShotPhyNumber)*100;
		dlg.SetPos(fScale);

		fgets(sLine.GetBuffer (200),200,fp);
		sLine.ReleaseBuffer ();
		if(sLine.Left (1)=='S'){
			m_pShotPhyPar[nShot].Ph =atol(LPCTSTR(sLine.Mid(19,6)));
			m_pShotPhyPar[nShot].east=atof(LPCTSTR(sLine.Mid(46,9)));
			m_pShotPhyPar[nShot].north  =atof(LPCTSTR(sLine.Mid(55,9)));
			m_pShotPhyPar[nShot].gd =atof(LPCTSTR(sLine.Mid(25,10)));
			m_pShotPhyPar[nShot].h  =atof(LPCTSTR(sLine.Mid(64,6)));
			m_pShotPhyPar[nShot].st =atoi(LPCTSTR(sLine.Mid(76,4)));
			nShot++;
		}		
	}
	fclose (fp);

	dlg.SetPos(100);

	SortShotPoint();

	dlg.DestroyWindow ();

	return true;

}
