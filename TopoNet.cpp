// TopoNet.cpp: implementation of the CTopoNet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3dfstbrk.h"
#include "TopoNet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTopoNet::CTopoNet()
{
	m_sTagFile="Toponet File";
	m_pPoint=NULL;
	m_pLine=NULL;
	Reset();
}

CTopoNet::~CTopoNet()
{
	Reset();
}


bool CTopoNet::SetPointNumber(long nPoint)
{
	if(nPoint<3){
		if(m_bReportError)AfxMessageBox("The vertex number in the net must be at least 3!");
		return false;
	}

	Reset();
	m_pPoint=new CMy3DPoint[nPoint];
	m_nPoint=nPoint;

	for(long i=0;i<nPoint;i++){
		m_pPoint[i].x=m_pPoint[i].y=m_pPoint[i].z=0;
	}

	m_nLineLimit=nPoint*(nPoint-1)/2;
	m_pLine=new CTagLine[m_nLineLimit];
	m_nLine=0;

	return true;

}

void CTopoNet::Reset()
{
	m_bReportError=false;
	m_dError=-9999;

	m_nPoint=0;
	m_nLine=0;
	m_nLineLimit=0;
	m_sTitle="Topography Net";

	if(m_pPoint){
		delete []m_pPoint;
		m_pPoint=NULL;
	}

	if(m_pLine){
		delete []m_pLine;
		m_pLine=NULL;
	}
}

void CTopoNet::SetPoint(CMyPoint &point, long nPos)
{
	if(nPos<0||nPos>=m_nPoint){
		if(m_bReportError)AfxMessageBox("The point position given is out of range!");
		return;
	}
	m_pPoint[nPos].x=point.x;
	m_pPoint[nPos].y=point.y;
}

void CTopoNet::SetPoint(double x,double y,double z,long nPos)
{
	if(nPos<0||nPos>=m_nPoint){
		if(m_bReportError)AfxMessageBox("The point position given is out of range!");
		return;
	}
	m_pPoint[nPos].x=x;
	m_pPoint[nPos].y=y;
	m_pPoint[nPos].z=z;

}


void CTopoNet::SetValue(double dValue,long nPos)
{
	if(nPos<0||nPos>=m_nPoint){
		if(m_bReportError)AfxMessageBox("The point position given is out of range!");
		return;
	}
	m_pPoint[nPos].z=dValue;
}

void CTopoNet::SetPoint(CMy3DPoint &point,long nPos)
{
	if(nPos<0||nPos>=m_nPoint){
		if(m_bReportError)AfxMessageBox("The point position given is out of range!");
		return;
	}
	m_pPoint[nPos]=point;
}

////////////////////////////////////////////////////////////
// 1.  You should set all of the points before call this function.
//    Unable check if they are all set because the org value is (0,0),
//    and the user may also set to (0,0);
// 2.  Calc all the needed lines between the points:
// 3.  2000.3.15  
// 4.  FanHua
bool CTopoNet::CalcNet()
{
	long i,j,k,m;
	m_nLine=0;
	bool bStore;
	
	CTagLine lineTemp;
	lineTemp.SetPrecision (m_dPrecision);

	for(i=0;i<m_nPoint;i++){
		lineTemp.x1 =m_pPoint[i].x;
		lineTemp.y1 =m_pPoint[i].y;
		lineTemp.nPointHead =i;

		for(j=0;j<m_nPoint;j++){
			if(j==i)continue;
			if(m_pPoint[j]==m_pPoint[i])continue;

			lineTemp.x2 =m_pPoint[j].x;
			lineTemp.y2 =m_pPoint[j].y;
			lineTemp.nPointTail =j;
			lineTemp.CalcLen ();

			// Check if there is any shorter lines, 
			// if only one shorter line exists, this line will be omited:
			bStore=true;
			for(k=0;k<m_nLine;k++){
				// save the shorter one line;
				if(lineTemp.CrossIn (m_pLine[k])&&m_pLine[k].m_dLen<=lineTemp.m_dLen){ // <= , = : for same lines:
					bStore=false;
					break;
				}
			}

			// All of the crossed lines are longer , 
			// so delete all of the crossed lines:
			if(bStore){
				
				// delete all of the crossed lines:
				for(k=0;k<m_nLine;k++){			

					if(lineTemp.CrossIn (m_pLine[k])&&m_pLine[k].m_dLen>lineTemp.m_dLen){
					
						////////////////////////////////////////
						// Delete the long line from the pointer:
						for(m=k;m<m_nLine-1;m++){
							m_pLine[m]=m_pLine[m+1];
						}
						m_nLine--;
						k--;
						////////////////////////////////////////
					}
				}

				// And plus this line at the queue end:
				if(m_nLine>=m_nLineLimit){
					if(m_bReportError)AfxMessageBox("CTomoNet error: too much lines!");
					return false;
				}
				else{
					m_pLine[m_nLine]=lineTemp;
					m_nLine++;
				}


			}
		}
	}
	return true;
}

//////////////////////////////////////////////////////
// There must be at least 3 points in pPointReturn:
bool CTopoNet::GetNetPoint(CMyPrcPoint &pointMid,CMy3DPoint *pPointReturn) 
{
	if(m_nPoint<3){
		if(m_bReportError)AfxMessageBox("CTomoNet Error: you must set more than 3 points , and call CalcNet()!");
		return false;
	}

	long i;

	///////////////////////////////////////////////
	// If the wanted point is one of the vertexes:		
	bool bVertex=false;
	for(i=0;i<m_nPoint;i++){
		if(pointMid== m_pPoint[i]){
			bVertex=true;
			break;
		}
	}
	long nVertex=i;
	if(bVertex)return GetVertexNetPoint(pointMid,nVertex,pPointReturn);

	///////////////////////////////////////////////
	// or is on a line:
	bool bOnLine=false;
	for(i=0;i<m_nLine;i++){
		if(m_pLine[i].PtInLine(pointMid)){
			bOnLine=true;
			break;
		}
	}
	long nLineOn=i;
	if(bOnLine)return GetOnLineNetPoint(pointMid,nLineOn,pPointReturn) ;

	////////////////////////////////////////////////
	// Check if the point is out of the net:
	double dbAngle1=-10000,dbAngle2=-10000;
	double dsAngle1=10000,dsAngle2=10000;
	double dAngle1,dAngle2;
								
	double dx,dy,dl;
	for(i=0;i<m_nPoint;i++){
		dy=m_pPoint[i].y-pointMid.y;
		dx=m_pPoint[i].x-pointMid.x;
		dl=sqrt(dx*dx+dy*dy);

		dAngle1=dx/dl+1;
		if(dy<0)dAngle1=-dAngle1;

		dAngle2=dx/dl-1;
		if(dy<0)dAngle2=-dAngle2;

		if(dAngle1<dsAngle1)
			dsAngle1=dAngle1;
		else if(dAngle1>dbAngle1)
			dbAngle1=dAngle1;

		if(dAngle2<dsAngle2)
			dsAngle2=dAngle2;
		else if(dAngle2>dbAngle2)
			dbAngle2=dAngle2;

	}
	bool bOutNet=(dbAngle1-dsAngle1)<2||(dbAngle2-dsAngle2)<2;
	
	if(bOutNet)return GetOutNetPoint(pointMid,pPointReturn) ;

	////////////////////////////////////////////////
	// It's a normal point:
	return GetInNetPoint(pointMid,pPointReturn) ;
}



bool CTopoNet::GetValue(CMy3DPoint &pointMid)
{
	pointMid.z=GetValue(pointMid.x,pointMid.y);
	return true;	
}
	

CTagLine * CTopoNet::GetLine()
{
	return m_pLine;
}

long CTopoNet::GetLineNumber()
{
	return m_nLine;
}

void CTopoNet::SetPrecision(double dPrecision)
{
	m_dPrecision=dPrecision;
}

CMy3DPoint CTopoNet::GetPoint(int i)
{
	if(i<0||i>=m_nPoint){
		AfxMessageBox("The point position given is out of range!");
		return CMy3DPoint(0,0,0);
	}
	return (m_pPoint[i]);
}

double CTopoNet::GetValue(double x, double y)
{
	CMy3DPoint pointReturn[3];
	
	GetNetPoint(CMyPrcPoint(x,y),pointReturn) ;

	// The plane:
	double a11,a12,a13;
	double a21,a22,a23;
	double a31,a32,a33;

	a11=x-pointReturn[0].x;
	a12=y-pointReturn[0].y;
	
	a21=pointReturn[1].x-pointReturn[0].x;
	a22=pointReturn[1].y-pointReturn[0].y;
	a23=pointReturn[1].z-pointReturn[0].z;
	
	a31=pointReturn[2].x-pointReturn[0].x;
	a32=pointReturn[2].y-pointReturn[0].y;
	a33=pointReturn[2].z-pointReturn[0].z;

	a13=(a23*a32*a11+a33*a21*a12-a11*a22*a33-a12*a23*a31)/(a21*a32-a22*a31);

	if((a21*a32-a22*a31)==0){
		if(m_bReportError)AfxMessageBox("CTopoNet Report: GetValue() error, the coordinates given are in one vertical plane!");
		return m_dError;
	}

	return a13+pointReturn[0].z;
}

bool CTopoNet::GetVertexNetPoint(CMyPrcPoint &pointMid,long nVertex,CMy3DPoint *pPointReturn) 
{
	
	if(m_nLine<3){
		if(m_bReportError)AfxMessageBox("Too less lines in the net!");
		return false;
	}

	//////////////////////////////////////////
	// If it is a vertex:
	CLine lineTemp;
	CMyPrcPoint *pPointLinked=new CMyPrcPoint[m_nLineLimit];
	long i,j,k,nPointLinked=0;

	CMyPrcPoint pointFst(-1,-1),pointSnd(-1,-1);
	for(i=0;i<m_nLine;i++){			
		// Search for one line linking to the vertex:
		if(m_pLine[i].PointA()==pointMid){
			pPointLinked[nPointLinked]=m_pLine[i].PointB();
			nPointLinked++;
		}
		else if(m_pLine[i].PointB()==pointMid){
			pPointLinked[nPointLinked]=m_pLine[i].PointA();
			nPointLinked++;
		}
	}

	if(nPointLinked==0){
		if(m_bReportError)AfxMessageBox("CTopoNet Error: Can not find a line linking with the vertex!");
		return false;
	}

	// Search for one line linking any two of the points linked to thw vertex:
	long nLine=-1;
	for(i=0;i<nPointLinked;i++){
		lineTemp.x1 =pPointLinked[i].x;
		lineTemp.y1 =pPointLinked[i].y;
		for(j=0;j<nPointLinked;j++){
			if(j==i)continue;
			lineTemp.x2 =pPointLinked[j].x;
			lineTemp.y2 =pPointLinked[j].y;

			for(k=0;k<m_nLine;k++){
				if(m_pLine[k].Equal(lineTemp)){
					nLine=k;
					break;
				}
			}
			if(nLine!=-1)break;
		}
		if(nLine!=-1)break;
	}
	delete []pPointLinked;

	if(nLine==-1){
		if(m_bReportError)AfxMessageBox("CTopoNet Error: can not find 2 vertex linking with the wanted vertex!");
		return false;
	}
	pPointReturn[0]=m_pPoint[nVertex];
	pPointReturn[1]=m_pPoint[m_pLine[nLine].nPointHead];
	pPointReturn[2]=m_pPoint[m_pLine[nLine].nPointTail];	

	return true;
}


bool CTopoNet::GetOnLineNetPoint(CMyPrcPoint &pointMid,long nLineOn,CMy3DPoint *pPointReturn) 
{
	//////////////////////////////////////////
	// If it is on a line:
	CTagLine lineTemp;
	pPointReturn[0]=m_pPoint[m_pLine[nLineOn].nPointHead];
	pPointReturn[1]=m_pPoint[m_pLine[nLineOn].nPointTail];

	bool bFstLineBe=false;
	bool bSndLineBe=false;
	long i,j;

	for(i=0;i<m_nPoint;i++){
		lineTemp.x1=m_pPoint[i].x;
		lineTemp.y1=m_pPoint[i].y;

		lineTemp.x2=m_pLine[nLineOn].x1;
		lineTemp.y2=m_pLine[nLineOn].y1;

		bFstLineBe=false;
		for(j=0;j<m_nLine;j++){
			if(j==nLineOn)continue;
			if(m_pLine[j].Equal (lineTemp)){
				bFstLineBe=true;
				break;
			}
		}

		if(!bFstLineBe)continue;

		lineTemp.x2=m_pLine[nLineOn].x2;
		lineTemp.y2=m_pLine[nLineOn].y2;

		bSndLineBe=false;
		for(j=0;j<m_nLine;j++){
			if(j==nLineOn)continue;
			if(m_pLine[j].Equal (lineTemp)){
				bSndLineBe=true;
				break;
			}
		}

		if(bFstLineBe&&bSndLineBe){
			pPointReturn[2]=m_pPoint[i];
			break;
		}
	}

	if(!bFstLineBe||!bSndLineBe){		
		if(m_bReportError)AfxMessageBox("CTopoNet Error: can not find the point that linked to the line on which is the wanted point!");
		return false;
	}
	return true;

}

//////////////////////////////////////////
// If it is out of the net:
bool CTopoNet::GetOutNetPoint(CMyPrcPoint &pointMid,CMy3DPoint *pPointReturn) 
{
	bool bThirdPointFound;
	long nNearestLine=-1;
	long nPointThird;
	long i,j;
	CMyPrcPoint pointFoot;

	while(true){
		
		////////////////////////////////////////////////////////////
		// Find not crossed lines:
		double dLen,dMinLen=9999999999999999;
		CTagLine lineTemp;
		double  dLenFromFoot,dMinPerpLen=99999999999;
		long nMinLenLine=-1;

		bool bCross,bFootInLine=false;
		lineTemp.x1=pointMid.x;
		lineTemp.y1=pointMid.y;
		for(i=0;i<m_nLine;i++){
			lineTemp.x2=m_pLine[i].x1+(m_pLine[i].x2-m_pLine[i].x1)/2;
			lineTemp.y2=m_pLine[i].y1+(m_pLine[i].y2-m_pLine[i].y1)/2;

			bCross=false;
			for(j=0;j<m_nLine;j++){
				if(j==i)continue;
				if(lineTemp.CrossIn(m_pLine[j])){
					bCross=true;
					break;
				}
			}			
			
			if(!bCross){
				// Calc the len from the foot to the wanted point:
				pointFoot=m_pLine[i].GetPerpFoot(pointMid);
				if(m_pLine[i].PtInLine(pointFoot)){ 				
					bFootInLine=true;
					dLenFromFoot=pointFoot.Distance (pointMid);
					if(dLenFromFoot<dMinPerpLen){
						dMinPerpLen=dLenFromFoot;
						nMinLenLine=i;
					}
				}

				// Calc the length from the middle point to the wanted point:
				else{
					lineTemp.CalcLen();
					if(lineTemp.m_dLen<dMinLen){
						dMinLen=lineTemp.m_dLen;
						nNearestLine=i;
					}
				}
			}
		}

		if(bFootInLine){
			nNearestLine=nMinLenLine;
		}
		else{
			nNearestLine=nNearestLine;
		}	


		if(nNearestLine==-1){
			if(m_bReportError)AfxMessageBox("CTopoNet Error: Can not find the nearest line from the net!");
			break;
		}


		////////////////////////////////////////////////////////////
		// Find the lines linking the head of the nearest line:
		long nHead=m_pLine[nNearestLine].nPointHead ;
		long nTail=m_pLine[nNearestLine].nPointTail ;
		nPointThird=-1;
		bThirdPointFound=false;
		double dDis,dMinDis=9999999999999999;
		long n;	


		for(i=0;i<m_nLine;i++){
			if(i==nNearestLine)continue;
			if(m_pLine[i].Linking (nHead)){
				for(j=0;j<m_nLine;j++){
					if(j==i||j==nNearestLine)continue;
					n=m_pLine[j].CommonPoint(m_pLine[i]);
					if(m_pLine[j].Linking(nTail)&&n>-1){
						bThirdPointFound=true;
						dDis=m_pLine[nNearestLine].Distance(m_pPoint[n]);
						if(dDis<dMinDis){
							dMinDis=dDis;
							nPointThird=n;
						}
					}
				}
			}
		}

		break;

	} // While true, just take advantage of its "break" function;

	if(bThirdPointFound){
		pPointReturn[0]=m_pPoint[m_pLine[nNearestLine].nPointHead ];
		pPointReturn[1]=m_pPoint[m_pLine[nNearestLine].nPointTail ];
		pPointReturn[2]=m_pPoint[nPointThird];
		return true;
	}
	else{
		if(m_bReportError)AfxMessageBox("Can not find 3 point approciate to the wanted point from the net!");
		return false;
	}		

}


bool CTopoNet::GetInNetPoint(CMyPrcPoint &pointMid,CMy3DPoint *pPointReturn) 
{
	//////////////////////////////////////////
	// It is a common point:
	CTagLine lineTemp;
	lineTemp.x1=pointMid.x;
	lineTemp.y1=pointMid.y;
	
	bool bCross;
	bool bError=false;
	long i,j;

	int nPoint=0;
	for(i=0;i<m_nPoint;i++){
		lineTemp.x2=m_pPoint[i].x;
		lineTemp.y2=m_pPoint[i].y;

		bCross=false;
		for(j=0;j<m_nLine;j++){
			if(lineTemp.CrossIn(m_pLine[j])){
				bCross=true;
				break;
			}
		}
			
		if(!bCross){
			if(nPoint<3){
				pPointReturn[nPoint]=m_pPoint[i];
				nPoint++;
			}
			else{
				bError=true;
				break;
			}
		}
	}

	return (nPoint<3||bError);
}


void CTopoNet::SetReturnError(double dError)
{
	m_dError=dError;
}


bool CTopoNet::Save(CString sFile)
{
	if(sFile==""){
		CFileDialog dlg(false);
		dlg.m_ofn.lpstrFilter="Target Toponet file (*.net)\0*.net";
		dlg.m_ofn.lpstrTitle=m_sTitle;
		dlg.m_ofn.lpstrDefExt=".net";
		
		if(dlg.DoModal()==IDCANCEL)return false;
		sFile=dlg.GetPathName();
	}

	FILE *fp=fopen(sFile,"wt");
	if(!fp){
		AfxMessageBox("Can not open file "+sFile);
		return false;
	}

	fprintf(fp,"%s\n",(LPCTSTR)m_sTagFile);
	
	// Write the point into the file:
	fprintf(fp,"%ld\n",m_nPoint);
	for(long i=0;i<m_nPoint;i++){
		fprintf(fp,"%1.1lf %1.1lf %1.1lf\n",m_pPoint[i].x,m_pPoint[i].y,m_pPoint[i].z);
	}
	
	// Write the line into the file:
	fprintf(fp,"%ld\n",m_nLine);
	for(i=0;i<m_nLine;i++){
		fprintf(fp,"%1.1lf %1.1lf %ld %1.1lf %1.1lf %ld \n",
			m_pLine[i].x1,m_pLine[i].y1,m_pLine[i].nPointHead ,
			m_pLine[i].x2,m_pLine[i].y2,m_pLine[i].nPointTail);
	}

	fclose(fp);

	return true;
}

bool CTopoNet::Open(CString sFile)
{
	if(sFile==""){
		CFileDialog dlg(true);
		dlg.m_ofn.lpstrFilter="Toponet file (*.net)\0*.net";

		CString sTitle="Open "+m_sTitle;
		dlg.m_ofn.lpstrTitle=LPCTSTR(sTitle);
		dlg.m_ofn.lpstrDefExt=".net";
		
		if(dlg.DoModal()==IDCANCEL)return false;
		sFile=dlg.GetPathName();
	}

	FILE *fp=fopen(sFile,"rt");
	if(!fp){
		AfxMessageBox("Can Not open the toponet file :"+sFile);
		return false;
	}
	
	// Check if this file is a toponet file:
	CString sCheck;	
	fgets(sCheck.GetBuffer(100),100,fp);
	sCheck.ReleaseBuffer ();
	sCheck=sCheck.Left(m_sTagFile.GetLength());
	if(m_sTagFile!=sCheck){
		AfxMessageBox("This is not a valid Toponet file!");
		return false;
	}
	
	// Readthe point :
	fscanf(fp,"%ld\n",&m_nPoint);
	SetPointNumber(m_nPoint);
	
	for(long i=0;i<m_nPoint;i++){
		fscanf(fp,"%lf %lf %lf\n",&m_pPoint[i].x,&m_pPoint[i].y,&m_pPoint[i].z);
	}
	
	// Write the line into the file:
	fscanf(fp,"%ld\n",&m_nLine);
	if(m_nLine>m_nLineLimit){
		AfxMessageBox("Toponet file error: too much lines in the net!");
		Reset();
		return false;
	}

	for(i=0;i<m_nLine;i++){
		if(feof(fp)){
			AfxMessageBox("Toponet file error: data is not enough!");
			Reset();
			return false;
		}

		fscanf(fp,"%lf %lf %ld %lf %lf %ld \n",
			&m_pLine[i].x1,&m_pLine[i].y1,&m_pLine[i].nPointHead ,
			&m_pLine[i].x2,&m_pLine[i].y2,&m_pLine[i].nPointTail
		);
	}

	fclose(fp);

	return true;

}

void CTopoNet::SetTitle(CString sTitle)
{
	m_sTitle=sTitle;
}

long CTopoNet::GetPointNumber()
{
	return m_nPoint;
}
