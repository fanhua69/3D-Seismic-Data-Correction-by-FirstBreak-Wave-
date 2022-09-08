// ControlFile.cpp: implementation of the CControlFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3dfstbrk.h"
#include "ControlFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CControlFile::CControlFile()
{
	m_pShotCtlPoint=NULL;
	m_pRcvCtlPoint=NULL;

	Reset();


}

CControlFile::~CControlFile()
{
	Reset();

}
void CControlFile::Reset()
{
	if(m_pShotCtlPoint)delete []m_pShotCtlPoint;	
	if(m_pRcvCtlPoint)delete []m_pRcvCtlPoint;	

	m_pShotCtlPoint=NULL;
	m_pRcvCtlPoint=NULL;

 	m_nShotCtlPoint=0;
	m_nRcvCtlPoint=0;
	m_nRcvCtlLimit=0;
	m_nShotCtlLimit=0;

}

bool CControlFile::Read(CString sFile)
{
	if(sFile==""){
		CFileDialog dlg(true);
		dlg.m_ofn.lpstrFilter="The control point file(*.ctl)\0*.ctl";
		dlg.m_ofn.lpstrTitle="Open the control point";
		dlg.m_ofn.lpstrDefExt=".ctl";
		
		if(dlg.DoModal()==IDCANCEL)return false;
		sFile=dlg.GetPathName();
	}
	if(sFile=="")return false;

	//
	FILE *fp=fopen(sFile,"rt");
	if(!fp){
		AfxMessageBox("Can not open the control point:"+sFile);
		return false;
	}

	Reset();

	//
	char sLine[100];
	fgets(sLine,100,fp);
	fscanf(fp,"%ld\n",&m_nShotCtlPoint);
	m_pShotCtlPoint =new PXYV[m_nShotCtlPoint];
	m_nShotCtlLimit=m_nShotCtlPoint;


	int nRead;
	for(long i=0;i<m_nShotCtlPoint;i++){
		nRead=fscanf(fp,"%ld %lf %lf %f\n",
			&m_pShotCtlPoint[i].Ph ,
			&m_pShotCtlPoint[i].north, 
			&m_pShotCtlPoint[i].east,
			&m_pShotCtlPoint[i].Value);
		if(nRead<4&&nRead>0){
			AfxMessageBox("Control Point File Error: "+sFile);
			return false;
		}
	}

	fgets(sLine,100,fp);
	fscanf(fp,"%ld\n",&m_nRcvCtlPoint);
	m_pRcvCtlPoint=new PXYV[m_nRcvCtlPoint];
	m_nRcvCtlLimit=m_nRcvCtlPoint;
	
	for(i=0;i<m_nRcvCtlPoint;i++){
		nRead=fscanf(fp,"%ld %lf %lf %f\n",
			&m_pRcvCtlPoint[i].Ph ,
			&m_pRcvCtlPoint[i].north, 
			&m_pRcvCtlPoint[i].east,
			&m_pRcvCtlPoint[i].Value);
		if(nRead<4&&nRead>0){
			AfxMessageBox("Control Point File Error: "+sFile);
			return false;
		}
	}

	fclose(fp);

	return true;
}


bool  CControlFile::Write(CString sFile)
{
	if(sFile==""){
		CFileDialog dlg(false);
		dlg.m_ofn.lpstrFilter="The control point file(*.ctl)\0*.ctl";
		dlg.m_ofn.lpstrTitle="Save to the control point";
		dlg.m_ofn.lpstrDefExt=".ctl";
		
		if(dlg.DoModal()==IDCANCEL)return false;
		sFile=dlg.GetPathName();
	}
	if(sFile=="")return false;

	//
	FILE *fp=fopen(sFile,"wt");
	if(!fp){
		AfxMessageBox("Can not open the control point:"+sFile);
		return false;
	}

	//
	fprintf(fp,"Shot control Point\n");
	fprintf(fp,"%ld\n",m_nShotCtlPoint);
	for(long i=0;i<m_nShotCtlPoint;i++){
		fprintf(fp,"%ld %1.1lf %1.1lf %1.1f\n",
			m_pShotCtlPoint[i].Ph ,
			m_pShotCtlPoint[i].north, 
			m_pShotCtlPoint[i].east,
			m_pShotCtlPoint[i].Value);

	}

	fprintf(fp,"Recieve Control Point\n");
	fprintf(fp,"%ld\n",m_nRcvCtlPoint);
	for(i=0;i<m_nRcvCtlPoint;i++){
			fprintf(fp,"%ld %1.1lf %1.1lf %1.1f\n",
			m_pRcvCtlPoint[i].Ph ,
			m_pRcvCtlPoint[i].north, 
			m_pRcvCtlPoint[i].east,
			m_pRcvCtlPoint[i].Value);

	}

	fclose(fp);

	return true;


}

PXYV* CControlFile::GetShotCtl()
{
	return m_pShotCtlPoint;
}

PXYV * CControlFile::GetRcvCtl()
{
	return m_pRcvCtlPoint;
}

bool CControlFile::SetShotCtlNumber(long nShotCtlNumber)
{
	if(m_pShotCtlPoint){
		delete []m_pShotCtlPoint;
		m_pShotCtlPoint=NULL;
	}
	m_pShotCtlPoint=new PXYV[nShotCtlNumber];
	memset(m_pShotCtlPoint,0,sizeof(PXYV)*nShotCtlNumber);

	m_nShotCtlPoint=m_nShotCtlLimit=nShotCtlNumber;

	return true;

}

bool CControlFile::SetShotCtl(long nPos,PXYV &point)
{
	if(nPos<0||nPos>=m_nShotCtlLimit)return false;
	m_pShotCtlPoint[nPos]=point;
	return true;
}
bool CControlFile::SetRcvCtlNumber(long nRcvCtlNumber)
{
	if(m_pRcvCtlPoint){
		delete []m_pRcvCtlPoint;
		m_pRcvCtlPoint=NULL;
	}
	m_pRcvCtlPoint=new PXYV[nRcvCtlNumber];
	memset(m_pRcvCtlPoint,0,sizeof(PXYV)*nRcvCtlNumber);
	m_nRcvCtlPoint=m_nRcvCtlLimit=nRcvCtlNumber;

	return true;
}

bool CControlFile::SetRcvCtl(long nPos,PXYV &point)
{
	if(nPos<0||nPos>=m_nRcvCtlLimit)return false;
	m_pRcvCtlPoint[nPos]=point;
	return true;
}

long CControlFile::GetShotCtlNumber()
{
 	return m_nShotCtlPoint;
}
long CControlFile::GetRcvCtlNumber()
{
 	return m_nRcvCtlPoint;
}
