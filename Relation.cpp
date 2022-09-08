// Relation.cpp: implementation of the CRelation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3dfstbrk.h"
#include "Relation.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRelation::CRelation()
{
	m_pGroup=NULL;
	m_nGroupPointNumber=0;
	m_pGroupModal=NULL;
	m_nGroupModalPointNumber=0;

}

CRelation::~CRelation()
{
	// The group to be calculated:
	if(m_pGroup){
		delete []m_pGroup;
		m_pGroup=NULL;
	}

	m_nGroupPointNumber=0;

	// The modal grooup:
	if(m_pGroupModal){
		delete []m_pGroupModal;
		m_pGroupModal=NULL;
	}

	m_nGroupModalPointNumber=0;

}

void CRelation::SetGroupPointNumber(long n)
{
	if(m_pGroup){
		delete []m_pGroup;
		m_pGroup=NULL;
	}

	m_nGroupPointNumber=n;
	m_pGroup=new float[n];
}

void CRelation::SetGroup(float *pGroup)
{
	memcpy(m_pGroup,pGroup,sizeof(float)*m_nGroupPointNumber);
}

void CRelation::SetModelGroup(float *pModalGroup, long nPoint)
{
	if(m_pGroupModal){
		delete []m_pGroupModal;
		m_pGroupModal=NULL;
	}

	m_nGroupModalPointNumber=nPoint;
	m_pGroupModal=new float[nPoint];
	memcpy(m_pGroupModal,pModalGroup,sizeof(float)*nPoint);


	float dBig=0,dValue;
	for(long i=0;i<m_nGroupModalPointNumber;i++){
		dValue=fabs(m_pGroupModal[i]);
		if(dValue>dBig)dBig=dValue;
	}
	for(i=0;i<m_nGroupModalPointNumber;i++){
		m_pGroupModal[i]/=dBig;
	}

}

long CRelation::Calculate()
{
	if(m_nGroupModalPointNumber>m_nGroupPointNumber){
		return 0;
	}

	// Firstly adjust the amplititude:
	long i,j;
	float dBig=0,dValue;
	for(i=0;i<m_nGroupPointNumber;i++){
		dValue=fabs(m_pGroup[i]);
		if(dValue>dBig)dBig=dValue;
	}
	for(i=0;i<m_nGroupPointNumber;i++){
		m_pGroup[i]/=dBig;
	}

	// Then calc the first fbk:
	long nMovePoint=m_nGroupPointNumber-m_nGroupModalPointNumber;
	long nReturn,nHalfModal=m_nGroupModalPointNumber/2;
	double dSum,dBigSum=999999999999.0,dv;
	for(i=0;i<nMovePoint;i++){
		dSum=0.0;
		for(j=0;j<m_nGroupModalPointNumber;j++){
			//dv=m_pGroupModal[j]-m_pGroup[i+j];
			//dSum+=dv*dv;
			dv=m_pGroupModal[j]*m_pGroup[i+j];
			dSum+=dv;

		}

		if(dSum<dBigSum){
			dBigSum=dSum;
			nReturn=i+nHalfModal;
		}
	}

	return nReturn;
}
