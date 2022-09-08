// TDoc.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "TDoc.h"
#include "equation.h"
#include "fh.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTDoc

IMPLEMENT_DYNCREATE(CTDoc, CDocument)

CTDoc::CTDoc()
{
	m_pSndLine=NULL;   // 被显示的部分数据。
	m_pMidFbkLine=NULL;
	m_pOutFbkLine=NULL;
		
	// 被检查数据的状态：
	m_bDrawOnShot=0;
	m_bCheckShotOrRcv=0;
	m_ShotName=m_sp.ShotLineFrom;
	m_RecieveName=0;

	Reset();
	
}

CTDoc::~CTDoc()
{
	Reset();
}

void CTDoc::Reset()
{
	if(m_pSndLine){
		delete []m_pSndLine;
		m_pSndLine=NULL;
	}
	if(m_pMidFbkLine){
		delete []m_pMidFbkLine;
		m_pMidFbkLine=NULL;
	}
	if(m_pOutFbkLine){
		delete []m_pOutFbkLine;
		m_pOutFbkLine=NULL;
	}

	m_nSndLine=0;
	m_nMidFbkLine=0;
	m_nOutFbkLine=0;

}


BEGIN_MESSAGE_MAP(CTDoc, CDocument)
	//{{AFX_MSG_MAP(CTDoc)
	ON_COMMAND(ID_TChoose, OnTChoose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTDoc diagnostics

#ifdef _DEBUG
void CTDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTDoc serialization

void CTDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{

	}
}

/////////////////////////////////////////////////////////////////////////////
// CTDoc commands
void CTDoc::OnTChoose() 
{
	
	CCheckT dlg;

	dlg.m_bDrawOnShotOrRcv=m_bDrawOnShot;
	dlg.m_bCheckShotOrRcv=m_bCheckShotOrRcv;
	dlg.m_RecieveName=m_RecieveName;
	dlg.m_ShotName=m_ShotName;

	for(int i=0;i<m_ss.RecieveLineNumber;i++)
		dlg.m_RecieveNameList[i]=m_sp.RecieveLineName[i];
	for(i=0;i<m_ss.ShotPointNumber;i++)
		dlg.m_ShotPointNameList[i]=m_sp.ShotPointName[i];
	
	dlg.m_RecieveNameNumber=m_ss.RecieveLineNumber;
	dlg.m_ShotNameNumber=m_ss.ShotPointNumber;	
	
	dlg.m_ShotLineFrom=m_sp.ShotLineFrom-1;
	dlg.m_ShotLineTo=m_sp.ShotLineTo;

	if(dlg.DoModal()){
		m_bDrawOnShot=dlg.m_bDrawOnShotOrRcv;
		m_bCheckShotOrRcv=dlg.m_bCheckShotOrRcv;
		m_ShotName=dlg.m_ShotName;
		m_RecieveName=dlg.m_RecieveName;
	}

	DrawLine();
	UpdateAllViews(NULL);

}

bool CTDoc::DrawLine()
{

	long Ph,n,i,j;

	long nMidFbkShot=m_FileMidFbk.GetShotNumber();
	long nMidFbkRcv=m_FileMidFbk.GetRcvNumber();
	long nOutFbkShot=m_FileOutFbk.GetShotNumber();
	long nOutFbkRcv=m_FileOutFbk.GetRcvNumber();
	long nSndShot=m_FileSnd.GetShotNumber();
	long nSndRcv=m_FileSnd.GetRcvNumber();

	PXYV *pMidFbkShot=m_FileMidFbk.GetShotData();
	PXYV *pMidFbkRcv=m_FileMidFbk.GetRcvData();
	
	PXYV *pOutFbkShot=m_FileOutFbk.GetShotData();
	PXYV *pOutFbkRcv=m_FileOutFbk.GetRcvData();
	
	PXYV *pSndShot=m_FileSnd.GetShotData();
	PXYV *pSndRcv=m_FileSnd.GetRcvData();


	Reset();

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 抽取炮点数据：
	if(m_bCheckShotOrRcv==0){

		// Draw out the shot line:
		if(m_bDrawOnShot==0){

			//  Middle Fbk 数据：
			m_pMidFbkLine=new PXYV[m_ss.ShotPointNumber];
			memset(m_pMidFbkLine,0,sizeof(PXYV)*m_ss.ShotPointNumber);

			for(i=0;i<m_ss.ShotPointNumber;i++){
				Ph=m_ShotName*1000+m_sp.ShotPointName[i];
				for(j=0;j<nMidFbkShot;j++){
					if(pMidFbkShot[j].Ph==Ph){
						m_pMidFbkLine[i]=pMidFbkShot[j];
						break;
					}
				}
			}
			m_nMidFbkLine=m_ss.ShotPointNumber;

			//  Out Fbk 数据：
			m_pOutFbkLine=new PXYV[m_ss.ShotPointNumber];
			memset(m_pOutFbkLine,0,sizeof(PXYV)*m_ss.ShotPointNumber);

			for(i=0;i<m_ss.ShotPointNumber;i++){
				Ph=m_ShotName*1000+m_sp.ShotPointName[i];
				for(j=0;j<nOutFbkShot;j++){
					if(pOutFbkShot[j].Ph==Ph){
						m_pOutFbkLine[i]=pOutFbkShot[j];
						break;
					}
				}
			}
			m_nOutFbkLine=m_ss.ShotPointNumber;


			// Snd 数据：
			n=0;
			for(i=0;i<nSndShot;i++){
				if((int)(pSndShot[i].Ph/1000)==m_ShotName)n++;
			}
			m_nSndLine=n;

			m_pSndLine=new PXYV[m_nSndLine];
			memset(m_pSndLine,0,sizeof(PXYV)*m_nSndLine);
			n=0;
			for(i=0;i<nSndShot;i++){
				if((int)(pSndShot[i].Ph/1000)==m_ShotName){
					m_pSndLine[n]=pSndShot[i];
					n++;
				}
			}

			m_sMidFbkLine="中间成果大炮初至静校正量：炮线 "+STR(m_ShotName);
			m_sOutFbkLine="最终成果大炮初至静校正量：炮线 "+STR(m_ShotName);
			m_sSndLine="其他方法静校正量：炮线 "+STR(m_ShotName);

		}
		
		// Draw out the shot point line:
		else if(m_bDrawOnShot==1){
			
			//  Middle  Fbk 数据：
			m_nMidFbkLine=m_sp.ShotLineNumber;
			m_pMidFbkLine=new PXYV[m_nMidFbkLine];
			memset(m_pMidFbkLine,0,sizeof(PXYV)*m_nMidFbkLine);

			for(i=0;i<m_sp.ShotLineNumber;i++){
				Ph=(m_sp.ShotLineFrom+i)*1000+m_ShotName;
				for(j=0;j<nMidFbkShot;j++){
					if(pMidFbkShot[j].Ph==Ph){
						m_pMidFbkLine[i]=pMidFbkShot[j];
						m_pMidFbkLine[i].Ph/=1000;
						break;
					}
				}
			}
			

			//  Out Fbk 数据：
			m_nOutFbkLine=m_sp.ShotLineNumber;
			m_pOutFbkLine=new PXYV[m_nOutFbkLine];
			memset(m_pOutFbkLine,0,sizeof(PXYV)*m_nOutFbkLine);

			for(i=0;i<m_sp.ShotLineNumber;i++){
				Ph=(m_sp.ShotLineFrom+i)*1000+m_ShotName;
				for(j=0;j<nOutFbkShot;j++){
					if(pOutFbkShot[j].Ph==Ph){
						m_pOutFbkLine[i]=pOutFbkShot[j];
						m_pOutFbkLine[i].Ph/=1000;
						break;
					}
				}
			}
			


			// Snd 数据：
			n=0;
			for(i=0;i<nSndShot;i++){
				if((int)(pSndShot[i].Ph%1000)==m_ShotName)n++;
			}
			m_nSndLine=n;  

			m_pSndLine=new PXYV[m_nSndLine];
			memset(m_pSndLine,0,sizeof(PXYV)*m_nSndLine);

			n=0;
			for(i=0;i<nSndShot;i++){
				if((int)(pSndShot[i].Ph%1000)==m_ShotName){
					m_pSndLine[n]=pSndShot[i];
					m_pSndLine[n].Ph/=1000;
					n++;
				}
			}

			m_sMidFbkLine="中间成果大炮初至静校正量：炮点线 "+STR(m_ShotName);
			m_sOutFbkLine="最终大炮初至静校正量：炮点线 "+STR(m_ShotName);
			m_sSndLine="其他方法静校正量：炮点线 "+STR(m_ShotName);

		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 抽取检波点数据：
	else if(m_bCheckShotOrRcv==1){
		// Middle Fbk 数据：
		n=0;
		for(i=0;i<nMidFbkRcv;i++){
			if((int)(pMidFbkRcv[i].Ph/1000)==m_RecieveName)n++;
		}
		m_nMidFbkLine=n;

		m_pMidFbkLine=new PXYV[m_nMidFbkLine];
		memset(m_pMidFbkLine,0,sizeof(PXYV)*m_nMidFbkLine);

		n=0;
		for(i=0;i<nMidFbkRcv;i++){
			if((int)(pMidFbkRcv[i].Ph/1000)==m_RecieveName){
				m_pMidFbkLine[n]=pMidFbkRcv[i];
				n++;
			}
		}
		m_nMidFbkLine=n;


		// ultimate ,final Fbk 数据：
		n=0;
		for(i=0;i<nOutFbkRcv;i++){
			if((int)(pOutFbkRcv[i].Ph/1000)==m_RecieveName)n++;
		}
		m_nOutFbkLine=n;

		m_pOutFbkLine=new PXYV[m_nOutFbkLine];
		memset(m_pOutFbkLine,0,sizeof(PXYV)*m_nOutFbkLine);

		n=0;
		for(i=0;i<nOutFbkRcv;i++){
			if((int)(pOutFbkRcv[i].Ph/1000)==m_RecieveName){
				m_pOutFbkLine[n]=pOutFbkRcv[i];
				n++;
			}
		}
		m_nOutFbkLine=n;

		// Sand 数据：
		n=0;
		for(i=0;i<nSndRcv;i++){
			if((int)(pSndRcv[i].Ph/1000)==m_RecieveName)n++;
		}
		m_nSndLine=n;

		m_pSndLine=new PXYV[m_nSndLine];
		memset(m_pSndLine,0,sizeof(PXYV)*m_nSndLine);

		n=0;
		for(i=0;i<nSndRcv;i++){
			if((int)(pSndRcv[i].Ph/1000)==m_RecieveName){
				m_pSndLine[n]=pSndRcv[i];
				n++;
			}
		}
		m_nSndLine=n;
	
		m_sMidFbkLine="中间成果大炮初至静校正量：检波线 "+STR(m_RecieveName);
		m_sOutFbkLine="最终成果大炮初至静校正量：检波线 "+STR(m_RecieveName);
		m_sSndLine="其他方法静校正量：检波线 "+STR(m_RecieveName);

	}

	return TRUE;

}
//    抽取一条线的数据。
////////////////////////////////////////////////////////////////////////////////////////////////////////////////


BOOL CTDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	return true;	
}

BOOL CTDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if(!m_ss.Set())return false;
	if(!m_sp.Set())return false;

	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO: Add your specialized creation code here
	if(!m_FileMidFbk.ReadRcv()||!m_FileMidFbk.ReadShot(lpszPathName)){
		return false;
	}

	m_FileOutFbk.ReadRcv();
	m_FileOutFbk.ReadShot();

	m_FileSnd.ReadShot();
	m_FileSnd.ReadRcv();

	m_bDrawOnShot=0;
	m_bCheckShotOrRcv=0;
	m_ShotName=m_sp.ShotLineFrom;
	m_RecieveName=0;

	DrawLine();
	
	return TRUE;
}

BOOL CTDoc::OnNewDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDocument::OnNewDocument();
}
