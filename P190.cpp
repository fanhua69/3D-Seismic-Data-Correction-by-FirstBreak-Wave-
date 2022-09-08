// P190.cpp: implementation of the CP190 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3dfstbrk.h"
#include "P190.h"
#include "SndFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CP190::CP190()
{
	m_pRcvPhyPar=NULL;
	m_pShotRcvRel=NULL;
	m_pShotPhyPar=NULL;
	m_pPointOfOneShot=NULL;

	Reset();
}

CP190::~CP190()
{
	Reset();
}

bool CP190::CalcShotOrgPos()
{
	return true;
}

void CP190::Reset()
{
	m_nShotPhyNumber=0;
	m_nRcvPhyNumber=0;
	m_nRelNumber=0;

	if(m_pShotPhyPar){
		delete []m_pShotPhyPar;
		m_pShotPhyPar=NULL;
	}
	if(m_pShotRcvRel){
		delete []m_pShotRcvRel;
		m_pShotRcvRel=NULL;
	}
	if(m_pShotPhyPar){
		delete []m_pShotPhyPar;
		m_pShotPhyPar=NULL;
	}

	if(m_pPointOfOneShot){
		delete []m_pPointOfOneShot;
		m_pPointOfOneShot=NULL;
	}

}

void CP190::SortShotPoint()
{
	// Set comparison offset to half the number of records in SortArray:
	int Offset = m_nShotPhyNumber/ 2;
	int Limit;
	int Switch;
	CShotPhyPar temp;
	// Loop until offset gets to zero.
	while(Offset > 0){ //
		Limit = m_nShotPhyNumber- Offset;
		do{
		   Switch = 0; // Assume no switches at this offset.

		   // Compare elements and switch ones out of order:
		   for(int Row = 0;Row<Limit;Row++){
			   if( m_pShotPhyPar[Row].Ph > m_pShotPhyPar[Row + Offset].Ph){
				   temp=m_pShotPhyPar[Row];
				   m_pShotPhyPar[Row]=m_pShotPhyPar[Row+Offset];
				   m_pShotPhyPar[Row+Offset]=temp;
				   Switch = Row;
			   }
		   }

		   // Sort on next pass only to where last switch was made:
		   Limit = Switch - Offset;
		}while(Switch);

		// No switches at last offset, try one half as big:
		Offset = Offset / 2;
	}

	return ;

}

void CP190::SortRcvPoint()
{
		// Set comparison offset to half the number of records in SortArray:
	int Offset = m_nRcvPhyNumber/ 2;
	int Limit;
	int Switch;
	CRcvPhyPar temp;
	// Loop until offset gets to zero.
	while(Offset > 0){ //
		Limit = m_nRcvPhyNumber- Offset;
		do{
		   Switch = 0; // Assume no switches at this offset.

		   // Compare elements and switch ones out of order:
		   for(int Row = 0;Row<Limit;Row++){
			   if( m_pRcvPhyPar[Row].Ph > m_pRcvPhyPar[Row + Offset].Ph){
				   temp=m_pRcvPhyPar[Row];
				   m_pRcvPhyPar[Row]=m_pRcvPhyPar[Row+Offset];
				   m_pRcvPhyPar[Row+Offset]=temp;
				   Switch = Row;
			   }
		   }

		   // Sort on next pass only to where last switch was made:
		   Limit = Switch - Offset;
		}while(Switch);

		// No switches at last offset, try one half as big:
		Offset = Offset / 2;
	}

	return ;


}

long CP190::SearchShotStation(long station)
{
	int nStart=0,nEnd=m_nShotPhyNumber-1,nMid;

	while(true){
		nMid=nStart+(nEnd-nStart)/2;
		
		//
		if(m_pShotPhyPar[nMid].Ph ==station)return nMid;
		if(nMid==nStart||nMid==nEnd){
			if(m_pShotPhyPar[nEnd].Ph ==station)
				return nEnd;
			else
				return -1;
		}

		// 
		if(m_pShotPhyPar[nMid].Ph<station)
			nStart=nMid;
		else
			nEnd=nMid;
		
	}

}

long CP190::SearchRcvStation(long nStation)
{
	int nStart=0,nEnd=m_nRcvPhyNumber-1,nMid;

	while(true){
		nMid=nStart+(nEnd-nStart)/2;
		
		//
		if(m_pRcvPhyPar[nMid].Ph ==nStation)return nMid;
		if(nMid==nStart||nMid==nEnd){
			if(m_pRcvPhyPar[nEnd].Ph ==nStation)
				return nEnd;
			else
				return -1;
		}

		// 
		if(m_pRcvPhyPar[nMid].Ph<nStation)
			nStart=nMid;
		else
			nEnd=nMid;
		
	}

}


CMyPoint CP190::GetShotPos(long nPh)
{
	
	long n=SearchShotStation(nPh);
	if(n!=-1&&n>=0&&n<m_nShotPhyNumber){
		return CMyPoint(m_pShotPhyPar[n].north,m_pShotPhyPar[n].east);
	}
	else{
		return CMyPoint(-1,-1);
	};

}

CMyPoint * CP190::GetRcvPosition(long nShotPh, long &nRcvPoint,long nStartPoint, long nEndPoint)
{  
	if(nStartPoint<0)nStartPoint=0;
	if(nEndPoint<0)nEndPoint=0;

	
	long nShot=GetShotRcvRel(nShotPh);
	if(nShot==-1)return NULL;

	long nStart,nEnd;	
	long i,j,n=0,nRcv;
	for( i=0;i<m_pShotRcvRel[nShot].nRcvLine ;i++){
		
		nStart=m_pShotRcvRel[nShot].RcvRange [i].start +nStartPoint;
		
		if(nEndPoint==0)
			nEnd=m_pShotRcvRel[nShot].RcvRange [i].end;
		else
			nEnd=m_pShotRcvRel[nShot].RcvRange [i].start+nEndPoint;

		if(nStart>m_pShotRcvRel[nShot].RcvRange [i].end)
			nStart=m_pShotRcvRel[nShot].RcvRange [i].end;

		if(nEnd>m_pShotRcvRel[nShot].RcvRange [i].end)
			nEnd=m_pShotRcvRel[nShot].RcvRange [i].end;


		for(j=nStart;j<=nEnd;j++){
			nRcv=SearchRcvStation(j);
			if(nRcv==-1){
				CString s;
				s.Format ("%ld",j);
				AfxMessageBox("Error: the reciever station "+s+" not found in the parameter!");
				return NULL;
			}
			
			m_pPointOfOneShot[n].x=m_pRcvPhyPar[nRcv].north;
			m_pPointOfOneShot[n].y=m_pRcvPhyPar[nRcv].east;

			n++;
		}

	}

	// Return:
	nRcvPoint=n;
	return m_pPointOfOneShot;


}

long CP190::GetShotRcvRel(long nShotPh)
{
	int nStart=0,nEnd=m_nRelNumber-1,nMid;

	while(true){
		nMid=nStart+(nEnd-nStart)/2;
		
		//
		if(m_pShotRcvRel[nMid].PhShot ==nShotPh)return nMid;
		if(nMid==nStart||nMid==nEnd){
			if(m_pShotRcvRel[nEnd].PhShot ==nShotPh)
				return nEnd;
			else
				return -1;
		}

		// 
		if(m_pShotRcvRel[nMid].PhShot <nShotPh)
			nStart=nMid;
		else
			nEnd=nMid;
		
	}

}

bool CP190::FormatChange(CString sFile)
{
	if(sFile==""){
		CFileDialog dlg(false);
		dlg.m_ofn.lpstrFilter="Target shot point par file (*.spp)\0*.spp";
		dlg.m_ofn.lpstrTitle="Give me the shot point par file name";
		dlg.m_ofn.lpstrDefExt=".spp";
		
		if(dlg.DoModal()==IDCANCEL)return false;
		sFile=dlg.GetPathName();
	}

	FILE *fp=fopen(sFile,"wt");
	if(!fp)return false;

	long zp,hp,bt,rt,bgt,egt;
	zp=hp=bt=rt=bgt=egt=0;

	CMyPoint pointShot;
	for(long i=0;i<m_nRelNumber;i++){
		pointShot=GetShotPos(m_pShotRcvRel[i].PhShot);

		fprintf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %1.1lf %1.1lf\n",
			m_pShotRcvRel[i].PhShot ,
			m_pShotRcvRel[i].FileNumber  ,
			zp,hp,bt,rt,bgt,egt,
			pointShot.y,
			pointShot.x);
	}

	fclose(fp);

	return true;
}




bool CP190::SaveShotStatic()
{
	CSndFile file;
	file.SetShotNumber (m_nShotPhyNumber);

	for(long i=0;i<m_nShotPhyNumber;i++){
		file.m_pShotData [i].Ph =m_pShotPhyPar[i].Ph ;
		file.m_pShotData [i].east  =m_pShotPhyPar[i].east ;
		file.m_pShotData [i].north  =m_pShotPhyPar[i].north ;
		file.m_pShotData [i].Value=m_pShotPhyPar[i].st ;
	}

	return file.WriteShot ();
}
bool CP190::SaveRcvStatic()
{
	CSndFile file;
	file.SetRcvNumber (m_nRcvPhyNumber);

	for(long i=0;i<m_nRcvPhyNumber;i++){
		file.m_pRcvData [i].Ph =m_pRcvPhyPar[i].Ph ;
		file.m_pRcvData [i].east  =m_pRcvPhyPar[i].east ;
		file.m_pRcvData [i].north  =m_pRcvPhyPar[i].north ;
		file.m_pRcvData [i].Value=m_pRcvPhyPar[i].rt;
	}

	return file.WriteRcv ();
}
