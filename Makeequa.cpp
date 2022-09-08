/*****************************************************************************************
					  File name : MakeEqua.CPP
                      Programer: FAN HUA
                      Start Time:1997.3.1
					  End  Time:1997.8.26
                     File Class : MakeEquation
******************************************************************************************/
#include "stdafx.h"
#include "3dfstbrk.h"
#include "spp.h"
#include "svsys.h"
#include "swathpar.h"
#include "Equation.h"
#include "MakeEqua.h"
#include "string.h"
#include "math.h"
#include "InP190.h"	
#include "FbkInFile.h"
#include "ProgDlg.h"
#include "SPPDoc.h"
#include "ZDEquation.h"
#include "fbk.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          Construction code  MakeEquation								BEGIN
MakeEquation::MakeEquation()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	   Set(int Swath)															BEGIN 
BOOL MakeEquation::Set(int Swath)
{
		// 调用基类的函数.
		if(!Equation::Set(Swath))return FALSE;  //在该函数中,需读取炮点参数.
		
		return TRUE;
}
//          void equation::MakeEquation:BEGIN									END
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//           Destruction Code 
MakeEquation::~MakeEquation()
{
}
//           ~MakeEquation
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		SearchMatchNumber																			 开始
//
bool MakeEquation::MakeEquationOnPhyPos()
{

	CInP190 p190;
	if(!p190.Open ())return false;	

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Open files.
	CFbkInFile fbk;
	if(!fbk.Open (m_FileSwathFirstBreak)){
		AfxMessageBox("无法打开初至文件"+m_FileSwathFirstBreak+" !");
		return FALSE;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  Read Shot Point Parameter data and Related parameters.
	CSPPDoc sppd;
	sppd.Set(m_SwathNumber);
	if(!sppd.Read())return FALSE;

	CZDEquation equation;
	equation.Construct(m_FileRS,m_Fileb,sppd.m_TotalSNumber ,sp.TotalRecievePointNumber ,sp.InitialVelocity );
	equation.SetFoldTime (sp.FoldTime );
	equation.SetPrecision (0.80);

	long ShotX=0,ShotY=0,ShotPointNumber=0;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Calculate couples.
	DataInFstBrkFile *pFirstBreak;

	int j,k,ShotDealing=0;

	CProgressDlg dlg;								 
	dlg.Create();
	dlg.SetRange(0,100);
	dlg.SetStep(1);
	dlg.SetStatus("  方程已经建立了:");
	dlg.SetWindowText("正在建立静校正量方程");


	FILE *fp=fopen("ShotRcv.txt","wt");

	long ph,nFileNumber;
	SvSys ss;
	if(!ss.Set()){
		AfxMessageBox("Can not find the survey system!");
		return false;
	}
	

	ShotPointPar *pSPPThis=NULL;
	CShotRcv *pShotRcv=new CShotRcv[ss.ShotPointNumber];
	CShotPoint ShotPoint;
	CRcvPoint RcvPoint;
	int nShotRcv;
	long nShotOrder;  
	long nStartGroup,nEndGroup;

	if(sp.Group1!=0){
		nStartGroup=sp.Group1-1;
		nEndGroup=sp.Group2-1;
	}
	else{
		nStartGroup=sp.Group3-1;
		nEndGroup=sp.Group4-1;
	}
	
	CMyPoint pointShot;
	CRcvPoint *pRcvPoint;
	for(long i=0;i<ss.ShotPointNumber;i++){
		ph=sp.ShotLineFrom*1000+sp.ShotPointName[i];
		
		pShotRcv[i].Shot.Pos=sp.GetShotPos(ph,0,0);

		pShotRcv[i].Shot .lNo =0;
		pShotRcv[i].lRcvNum =sp.TotalUnformRecievePointNumber;

		pRcvPoint=sp.GetRcvPoint(ph);
		
		// Set the rcv point:
		for(k=0;k<sp.TotalUnformRecievePointNumber;k++){
			pShotRcv[i].Rcv[k].lNo =pRcvPoint[k].lNo;
			pShotRcv[i].Rcv[k].Fbk =0;
			pShotRcv[i].Rcv[k].Pos =pRcvPoint[k].Pos; 
		}
	}
 	equation.SetCommonShotGroup (pShotRcv,ss.ShotPointNumber);

	
	CMyPoint *pPointRcvPos;
	CRcvPoint *pPointRcv;
	long nRcv;

	long nGroupNumber;
	for(long nShotLine=sp.ShotLineFrom ;nShotLine<=sp.ShotLineTo;nShotLine++){

		dlg.SetPos ((float)(nShotLine-sp.ShotLineFrom )/(float)(sp.ShotLineNumber)*100);
		
		nShotRcv=0;
		nShotOrder=-1;
		for(j=0;j<ss.ShotPointNumber ;j++){
			ph=nShotLine*1000+sp.ShotPointName[j];

			pSPPThis=NULL;
			for(k=0;k<sppd.m_TotalSNumber ;k++){				
				if(ph==sppd.m_pSPP [k].m_Ph ){
					pSPPThis=&sppd.m_pSPP [k];
					nShotOrder=k;
					break;
				}
			}
			if(pSPPThis==NULL||pSPPThis->m_FileNumber ==-1){
				CString s;
				s.Format("%ld",ph);
				AfxMessageBox("Can not find the shot point station, or this shot is abandoned: "+s);
				continue;
			}

			nFileNumber=pSPPThis->m_FileNumber ;

			// search for the first break:
			pFirstBreak=fbk.GetShotFbk (nFileNumber,nGroupNumber);
			if(!pFirstBreak||nGroupNumber!=sp.TotalUnformRecievePointNumber){
				CString s;
				s.Format("%ld",ph);	
				AfxMessageBox("Can not find the first break for station "+s+" , or first break data error!");
				continue;
			}

			// Find the shot position and recieve point position:
			pShotRcv[nShotRcv].Shot .lNo =nShotOrder;
			pShotRcv[nShotRcv].Shot.Pos=p190.GetShotPos (pSPPThis->m_Ph);
			pShotRcv[nShotRcv].lRcvNum =sp.TotalUnformRecievePointNumber;
			
			// Set the rcv point:
			pPointRcv=sp.GetRcvPoint(pSPPThis->m_Ph );
			if(!pPointRcv)continue;

			pPointRcvPos=p190.GetRcvPosition (pSPPThis->m_Ph ,nRcv,nStartGroup,nEndGroup);
			if(!pPointRcvPos)continue;

			for(k=0;k<sp.TotalUnformRecievePointNumber;k++){
				pShotRcv[nShotRcv].Rcv[k].lNo =pPointRcv[k].lNo ;
				pShotRcv[nShotRcv].Rcv[k].Fbk =pFirstBreak[k].FirstBreak ;
				pShotRcv[nShotRcv].Rcv[k].Pos=pPointRcvPos[k];
			}

			nShotRcv++; // The number of shot in one row ;
		}

		if(!equation.Append (pShotRcv,nShotRcv))break;
	}
	dlg.SetPos (100);


	equation.Close ();			

	delete []pShotRcv;
	
	dlg.DestroyWindow(); 

	AfxMessageBox("静校正量方程已经建好！");

	return TRUE;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	  
//		MakeRSEquationFile(CDC *pDC)				BEGIN
BOOL MakeEquation::MakeEquationFile()
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Open files.
	CFbkInFile fbk;
	if(!fbk.Open (m_FileSwathFirstBreak)){
		AfxMessageBox("无法打开初至文件"+m_FileSwathFirstBreak+" !");
		return FALSE;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  Read Shot Point Parameter data and Related parameters.
	CSPPDoc sppd;
	sppd.Set(m_SwathNumber);
	if(!sppd.Read()){
		AfxMessageBox("Can not open the shot point parameter file!");
		return FALSE;
	}

	CZDEquation equation;
	equation.Construct(m_FileRS,m_Fileb,sppd.m_TotalSNumber ,sp.TotalRecievePointNumber ,sp.InitialVelocity );
	equation.SetFoldTime (sp.FoldTime );
	equation.SetPrecision (0.80);

	long ShotX=0,ShotY=0,ShotPointNumber=0;	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Calculate couples.
	DataInFstBrkFile *pFirstBreak;

	int j,k,ShotDealing=0;

	CProgressDlg dlg;								 
	dlg.Create();
	dlg.SetRange(0,100);
	dlg.SetStep(1);
	dlg.SetStatus("  方程已经建立了:");
	dlg.SetWindowText("正在建立静校正量方程");


	FILE *fp=fopen("ShotRcv.txt","wt");

	long ph,nFileNumber;
	SvSys ss;
	if(!ss.DataOK ){
		AfxMessageBox("Can not find the survey system!");
		return false;
	}
	

	ShotPointPar *pSPPThis=NULL;
	CShotRcv *pShotRcv=new CShotRcv[ss.ShotPointNumber];
	CShotPoint ShotPoint;
	CRcvPoint RcvPoint;
	int nShotRcv;
	long nShotOrder;  
	CRcvPoint *pRcvPoint;
	
	for(long i=0;i<ss.ShotPointNumber;i++){
		ph=sp.ShotLineFrom *1000+sp.ShotPointName [i];
		
		pShotRcv[i].Shot.Pos=sp.GetShotPos (ph,0,0);
		pShotRcv[i].Shot .lNo =0;
		pShotRcv[i].lRcvNum =sp.TotalUnformRecievePointNumber;

		pRcvPoint=sp.GetRcvPoint (ph);
		
		// Set the rcv point:
		for(k=0;k<sp.TotalUnformRecievePointNumber;k++){
			pShotRcv[i].Rcv[k].lNo =pRcvPoint[k].lNo ;
			pShotRcv[i].Rcv[k].Fbk =0;
			pShotRcv[i].Rcv[k].Pos =pRcvPoint[k].Pos ;
		}
	}
 	equation.SetCommonShotGroup (pShotRcv,ss.ShotPointNumber);


	long nGroupNumber;
	bool bContinue=true;
	for(long nShotLine=sp.ShotLineFrom ;nShotLine<=sp.ShotLineTo;nShotLine++){

		dlg.SetPos ((float)(nShotLine-sp.ShotLineFrom )/(float)(sp.ShotLineNumber)*100);
		
		nShotRcv=0;
		for(j=0;j<ss.ShotPointNumber ;j++){
			ph=nShotLine*1000+sp.ShotPointName [j];
			
			// get the order of the shot point and the parameters of it:
			pSPPThis=NULL;
			nShotOrder=-1;
			for(k=0;k<sppd.m_TotalSNumber ;k++){				
				if(ph==sppd.m_pSPP [k].m_Ph ){
					pSPPThis=&sppd.m_pSPP [k];
					nShotOrder=k;
					break;
				}
			}
			
			// Can not find the shot point in the shot point par file:
			if(pSPPThis->m_FileNumber ==0)continue;

			bContinue=true;
			if(pSPPThis==NULL){
				CString s;
				s.Format("%ld",ph);
				if(AfxMessageBox("Can not find the shot point station "+s+" , Continue?",MB_YESNO)==IDYES)
					continue;
				else{
					bContinue=false;
					break;
				}
			}

			nFileNumber=pSPPThis->m_FileNumber ;

			// search for the first break:
			pFirstBreak=fbk.GetShotFbk (nFileNumber,nGroupNumber);
			if(!pFirstBreak||nGroupNumber!=sp.TotalUnformRecievePointNumber){
				CString s;
				s.Format("%ld",ph);	
				AfxMessageBox("Can not find the first break for station "+s+" , or first break data error!");
				continue;
			}			

			// Set the shot point:
			pShotRcv[nShotRcv].Shot.Pos=sp.GetShotPos (ph,pSPPThis->m_ZP ,pSPPThis->m_HP);
			pShotRcv[nShotRcv].Shot .lNo =nShotOrder;
			pShotRcv[nShotRcv].lRcvNum =sp.TotalUnformRecievePointNumber;
			
			// Set the rcv point:
			pRcvPoint=sp.GetRcvPoint (ph);
			for(k=0;k<sp.TotalUnformRecievePointNumber;k++){
				pShotRcv[nShotRcv].Rcv[k].lNo =pRcvPoint[k].lNo ;
				pShotRcv[nShotRcv].Rcv[k].Fbk =pFirstBreak[k].FirstBreak ;
				pShotRcv[nShotRcv].Rcv[k].Pos =pRcvPoint[k].Pos ;
			}

			nShotRcv++;
		}
		
		if(!bContinue)break;

		if(!equation.Append (pShotRcv,nShotRcv))break;
	}
	dlg.SetPos (100);


	equation.Close ();			

	delete []pShotRcv;

	
	dlg.DestroyWindow(); 

	AfxMessageBox("静校正量方程已经建好！");

	return TRUE;
}
//							BOOL ResoEquation::MakeRSEquationFile(CDC *pDC)						END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
