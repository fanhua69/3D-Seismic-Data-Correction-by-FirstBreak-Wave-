// SndFile.cpp: implementation of the CSndFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3dfstbrk.h"
#include "SndFile.h"
#include "PXYV.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSndFile::CSndFile()
{
	m_pShotData=NULL;
	m_pRcvData=NULL;
}

CSndFile::~CSndFile()
{
	Reset();
}

void CSndFile::Reset()
{
	if(m_pShotData)delete []m_pShotData;
	if(m_pRcvData)delete []m_pRcvData;
	m_pRcvData=m_pRcvData=NULL;

	m_nShotData=0;
	m_nRcvData=0;
	m_nShotDataLimit=0;
	m_nRcvDataLimit=0;
}

bool CSndFile::ReadShot(CString sFile)
{
	if(sFile==""){
		CFileDialog dlg(true);
		dlg.m_ofn.lpstrFilter="Shot file(*.sns)\0*.sns";
		dlg.m_ofn.lpstrTitle="Other Static Correction File";
		dlg.m_ofn.lpstrDefExt=".mis";
		
		if(dlg.DoModal()==IDCANCEL)return false;
		sFile=dlg.GetPathName();
	}
	if(sFile=="")return false;

	//
	FILE *fp=fopen(sFile,"rt");
	if(!fp){
		AfxMessageBox("Can not open other static correction file :"+sFile);
		return false;
	}

	//
	int nRead;
	long ph,nLine=0;
	double x,y;
	float v;

	while (!feof(fp)){
		nRead=fscanf(fp,"%ld %lf %lf %f\n",&ph,&x,&y,&v);
		if(nRead>0&&nRead<4)break;
		nLine++;
	}
	fclose(fp);
	if(nRead>0&&nRead<4)return false;


	fp=fopen(sFile,"rt");

	m_nShotData=nLine;
	if(m_pShotData){
		delete []m_pShotData;
		m_pShotData=NULL;
	}
	m_pShotData=new PXYV[m_nShotData];

	//
	for(long i=0;i<m_nShotData;i++){
		fscanf(fp,"%ld %lf %lf %f\n",
			&m_pShotData[i].Ph,
			&m_pShotData[i].east,
			&m_pShotData[i].north,
			&m_pShotData[i].Value);
	};

	fclose(fp);

	//
	SortShotData();


	return true;
}

bool CSndFile::ReadRcv(CString sFile)
{
	if(sFile==""){
		CFileDialog dlg(true);
		dlg.m_ofn.lpstrFilter="recieve file(*.mir)\0*.snr";
		dlg.m_ofn.lpstrTitle="Other Static Correction File";
		dlg.m_ofn.lpstrDefExt=".snr";
		
		if(dlg.DoModal()==IDCANCEL)return false;
		sFile=dlg.GetPathName();
	}
	if(sFile=="")return false;

	//
	FILE *fp=fopen(sFile,"rt");
	if(!fp){
		AfxMessageBox("Can not open the Other static correction file :"+sFile);
		return false;
	}

	//
	int nRead;
	long ph,nLine=0;
	double x,y;
	float v;

	while (!feof(fp)){
		nRead=fscanf(fp,"%ld %lf %lf %f\n",&ph,&x,&y,&v);
		if(nRead>0&&nRead<4)break;
		nLine++;
	}
	fclose(fp);
	if(nRead>0&&nRead<4)return false;

	fp=fopen(sFile,"rt");
	m_nRcvData=nLine;
	if(m_pRcvData){
		delete []m_pRcvData;
		m_pRcvData=NULL;
	}
	m_pRcvData=new PXYV[m_nRcvData];

	//
	for(long i=0;i<m_nRcvData;i++){
		fscanf(fp,"%ld %lf %lf %f\n",
			&m_pRcvData[i].Ph,
			&m_pRcvData[i].east,
			&m_pRcvData[i].north,
			&m_pRcvData[i].Value);
	};

	fclose(fp);

	SortRcvData();

	return true;

}

bool CSndFile::WriteRcv(CString sFile)
{
	if(sFile==""){
		CFileDialog dlg(false);
		dlg.m_ofn.lpstrFilter="recieve file(*.snr)\0*.snr";
		dlg.m_ofn.lpstrTitle="Save Other Static Correction File";
		dlg.m_ofn.lpstrDefExt=".snr";
		
		if(dlg.DoModal()==IDCANCEL)return false;
		sFile=dlg.GetPathName();
	}
	if(sFile=="")return false;

	//
	FILE *fp=fopen(sFile,"wt");
	if(!fp){
		AfxMessageBox("Can not open the other static correction file :"+sFile);
		return false;
	}

	//
	for(long i=0;i<m_nRcvData;i++){
		fprintf(fp,"%ld %1.1lf %1.1lf %1.1f\n",
			m_pRcvData[i].Ph,
			m_pRcvData[i].east,
			m_pRcvData[i].north,
			m_pRcvData[i].Value);
	};

	fclose(fp);

	return true;

}

bool CSndFile::WriteShot(CString sFile)
{			
	if(sFile==""){
		CFileDialog dlg(false);
		dlg.m_ofn.lpstrFilter="shot file(*.sns)\0*.sns";
		dlg.m_ofn.lpstrTitle="Other Static Correction File";
		dlg.m_ofn.lpstrDefExt=".sns";
		
		if(dlg.DoModal()==IDCANCEL)return false;
		sFile=dlg.GetPathName();
	}
	if(sFile=="")return false;

	//
	FILE *fp=fopen(sFile,"wt");
	if(!fp){
		AfxMessageBox("Can not open the Other static correction file :"+sFile);
		return false;
	}

	//
	for(long i=0;i<m_nShotData;i++){
		fprintf(fp,"%ld %1.1lf %1.1lf %1.1f\n",
			m_pShotData[i].Ph,
			m_pShotData[i].east,
			m_pShotData[i].north,
			m_pShotData[i].Value);
	};

	fclose(fp);

	return true;



}

bool CSndFile::SetRcv(PXYV *pRcv, long nData)
{
	if(m_pRcvData){
		delete []m_pRcvData;
		m_pRcvData=NULL;
	}

	m_pRcvData=new PXYV[nData];

	for(long i=0;i<nData;i++){
		m_pRcvData[i]=pRcv[i];
	}

	return true;

}
bool CSndFile::SetShot(PXYV *pShot, long nData)
{
	if(m_pShotData){
		delete []m_pShotData;
		m_pShotData=NULL;
	}

	m_pShotData=new PXYV[nData];

	for(long i=0;i<nData;i++){
		m_pShotData[i]=pShot[i];
	}

	return true;

}

long CSndFile::SearchShotStation(long nPh)
{
	int nStart=0,nEnd=m_nShotData-1,nMid;

	while(true){
		nMid=nStart+(nEnd-nStart)/2;

		//
		if(m_pShotData[nMid].Ph ==nPh)return nMid;
		if(nMid==nStart||nMid==nEnd){
			if(m_pShotData[nEnd].Ph ==nPh)
				return nEnd;
			else
				return -1;
		}

		// 
		if(m_pShotData[nMid].Ph<nPh)
			nStart=nMid;
		else
			nEnd=nMid;
	}
	
}

long CSndFile::SearchRcvStation(long nPh)
{
	int nStart=0,nEnd=m_nRcvData-1,nMid;

	while(true){
		nMid=nStart+(nEnd-nStart)/2;

		//
		if(m_pRcvData[nMid].Ph ==nPh)return nMid;
		if(nMid==nStart||nMid==nEnd){
			if(m_pRcvData[nEnd].Ph ==nPh)
				return nEnd;
			else
				return -1;
		}

		// 
		if(m_pRcvData[nMid].Ph<nPh)
			nStart=nMid;
		else
			nEnd=nMid;
	}
}

void CSndFile::SortShotData()
{
	// Set comparison offset to half the number of records in SortArray:
	int Offset = m_nShotData/ 2;
	int Limit;
	int Switch;
	PXYV temp;
	// Loop until offset gets to zero.
	while(Offset > 0){ //
		Limit = m_nShotData- Offset;
		do{
		   Switch = 0; // Assume no switches at this offset.

		   // Compare elements and switch ones out of order:
		   for(int Row = 0;Row<Limit;Row++){
			   if( m_pShotData[Row].Ph > m_pShotData[Row + Offset].Ph){
				   temp=m_pShotData[Row];
				   m_pShotData[Row]=m_pShotData[Row+Offset];
				   m_pShotData[Row+Offset]=temp;
				   Switch = Row;
			   }
		   }

		   // Sort on next pass only to where last switch was made:
		   Limit = Switch - Offset;
		}while(Switch);

		// No switches at last offset, try one half as big:
		if(Offset==1)
			break;
		else
			Offset = (Offset+1) / 2;

	}

}


void CSndFile::SortRcvData()
{
	// Set comparison offset to half the number of records in SortArray:
	int Offset = m_nRcvData/ 2;
	int Limit;
	int Switch;
	PXYV temp;
	// Loop until offset gets to zero.
	while(Offset > 0){ //
		Limit = m_nRcvData- Offset;
		do{
		   Switch = 0; // Assume no switches at this offset.

		   // Compare elements and switch ones out of order:
		   for(int Row = 0;Row<Limit;Row++){
			   if( m_pRcvData[Row].Ph > m_pRcvData[Row + Offset].Ph){
				   temp=m_pRcvData[Row];
				   m_pRcvData[Row]=m_pRcvData[Row+Offset];
				   m_pRcvData[Row+Offset]=temp;
				   Switch = Row;
			   }
		   }

		   // Sort on next pass only to where last switch was made:
		   Limit = Switch - Offset;
		}while(Switch);

		// No switches at last offset, try one half as big:
		if(Offset==1)
			break;
		else
			Offset = (Offset+1) / 2;

	}

}

bool CSndFile::SetShotNumber(long n)
{
	if(m_pShotData){
		delete []m_pShotData;
		m_pShotData=NULL;
	}
	m_pShotData=new PXYV[n];
	m_nShotData=m_nShotDataLimit=n;

	return true;
}

bool CSndFile::SetRcvNumber(long n)
{
	if(m_pRcvData){
		delete []m_pRcvData;
		m_pRcvData=NULL;
	}
	m_pRcvData=new PXYV[n];
	m_nRcvData=m_nRcvDataLimit=n;

	return true;

}
