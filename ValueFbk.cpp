// ValueFbk.cpp: implementation of the CValueFbk class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3dfstbrk.h"
#include "ValueFbk.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CValueFbk::CValueFbk()
{
	m_pGroup=NULL;
	m_nPointNumber=0;

}

CValueFbk::~CValueFbk()
{
	if(m_pGroup){
		delete []m_pGroup;
		m_pGroup=NULL;
	}
}

int CValueFbk::CalcFbk(float *pGroup)
{
	if(m_nPointNumber==0||!m_pGroup)return -1;

	memcpy(m_pGroup,pGroup,sizeof(float)*m_nPointNumber);
	
	//
	/*
	double dSum=0.0;
	for(int i=0;i<m_nPointNumber;i++){
		if(m_pGroup[i]<dSum)dSum=m_pGroup[i];
	}
	dSum/=5;

	//
	for(i=0;i<m_nPointNumber;i++){
		if(m_pGroup[i]<dSum)return i-1;
	} */

	double dSum=0.0;
	for(int i=0;i<m_nPointNumber;i++){
		dSum+=fabs(m_pGroup[i]);
	}
	dSum/=m_nPointNumber;

	dSum/=5;
	dSum=-dSum;

	//
	for(i=0;i<m_nPointNumber;i++){
		if(m_pGroup[i]<dSum)return i-1;
	} 


	return -1;
}
void CValueFbk::SetPointNumber(int nPoint)
{
	if(nPoint==m_nPointNumber)return;

	if(m_pGroup){
		delete []m_pGroup;
		m_pGroup=NULL;
	}

	m_pGroup=new float[nPoint];
	m_nPointNumber=nPoint;
}



