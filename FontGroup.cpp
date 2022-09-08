// FontGroup.cpp: implementation of the CFontGroup class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3dfstbrk.h"
#include "FontGroup.h"
#include "math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFontGroup::CFontGroup()
{
	m_pValueGroup=NULL;
	Reset();
}

CFontGroup::~CFontGroup()
{
	Reset();
}

void CFontGroup::Reset()
{
	m_dMinValue=0.0;
	m_dMaxValue=0.0;
	m_nWindowHeight=0;
	m_nFontHeight=0;
 
	m_nValueStepNumber=16;
	m_dMinusValueStep[0]=-10000000.0;
	m_dMinusValueStep[1]=-5000000.0;
	m_dMinusValueStep[2]=-100000.0;
	m_dMinusValueStep[3]=-50000.0;
	m_dMinusValueStep[4]=-10000.0;
	m_dMinusValueStep[5]=-5000.0;	
	m_dMinusValueStep[6]=-1000.0;
	m_dMinusValueStep[7]=-500.0;
	m_dMinusValueStep[8]=-100.0;
	m_dMinusValueStep[9]=-50.0;
	m_dMinusValueStep[10]=-10.0;
	m_dMinusValueStep[11]=-5.0;
	m_dMinusValueStep[12]=-2.0;
	m_dMinusValueStep[13]=-1.0;
	m_dMinusValueStep[14]=-0.5;
	m_dMinusValueStep[15]=-0.1;
	m_dMinusValueStep[16]=0;



	m_dPlusValueStep[0]=10000000;
	m_dPlusValueStep[1]=5000000;
	m_dPlusValueStep[2]=100000;
	m_dPlusValueStep[3]=50000;
	m_dPlusValueStep[4]=10000;
	m_dPlusValueStep[5]=5000;
	m_dPlusValueStep[6]=1000;
	m_dPlusValueStep[7]=500;
	m_dPlusValueStep[8]=100;
	m_dPlusValueStep[9]=50;
	m_dPlusValueStep[10]=10;
	m_dPlusValueStep[11]=5;
	m_dPlusValueStep[12]=2;
	m_dPlusValueStep[13]=1;
	m_dPlusValueStep[14]=0.5;
	m_dPlusValueStep[15]=0.1;
	m_dPlusValueStep[16]=0;


	m_nValue=0;

	if(m_pValueGroup){
		delete []m_pValueGroup;
		m_pValueGroup=NULL;
	}
}

void CFontGroup::SetValueRange(double dMinValue, double dMaxValue)
{
	m_dMinValue=dMinValue;
	m_dMaxValue=dMaxValue;
}
void CFontGroup::SetWindowHeight(int nWindowHeight)
{
	m_nWindowHeight=nWindowHeight;
}
void CFontGroup::SetFontHeight(int nFontHeight)
{
	m_nFontHeight=nFontHeight;
}

void CFontGroup::Set(double dMinValue, double dMaxValue,int nWindowHeight,int nFontHeight)
{
	m_dMinValue=dMinValue;
	m_dMaxValue=dMaxValue;
	m_nWindowHeight=nWindowHeight;
	m_nFontHeight=nFontHeight;
}

double * CFontGroup::CalcFontGroup(int &nValueNumber)
{	

	// Calc the value step:
	int nValue=m_nWindowHeight/(m_nFontHeight*1.5);		
	double dValueStep=(m_dMaxValue-m_dMinValue)/nValue;
	
	CString sValue;
	sValue.Format("%1.0lf",dValueStep);
	if(dValueStep<0){
		if(dValueStep<m_dMinusValueStep[0])dValueStep=m_dMinusValueStep[0];
		for(int i=1;i<m_nValueStepNumber;i++){
			if(dValueStep>m_dMinusValueStep[i-1]&&dValueStep<m_dMinusValueStep[i]){
				dValueStep=m_dMinusValueStep[i-1];
			}
		}
	}
	else{
		if(dValueStep>m_dPlusValueStep[0])dValueStep=m_dPlusValueStep[0];
		for(int i=1;i<m_nValueStepNumber;i++){
			if(dValueStep<m_dPlusValueStep[i-1]&&dValueStep>m_dPlusValueStep[i]){
				dValueStep=m_dPlusValueStep[i-1];
			}
		}
	}

	
	// Calc from which value :
	double dValueFrom=(int(m_dMinValue/dValueStep)+1)*dValueStep;
	double dValue=dValueFrom;


	// Calc the value number:
	int n=1;
	while(dValue<m_dMaxValue){
		dValue+=dValueStep;
		n++;
	}
	m_nValue=n;

	// set the value group:
	if(m_pValueGroup){
		delete []m_pValueGroup;
		m_pValueGroup=NULL;
	}
	m_pValueGroup=new double[m_nValue];

	m_pValueGroup[0]=dValueFrom;
	n=1;
	dValue=dValueFrom;
	while(dValue<m_dMaxValue){
		dValue+=dValueStep;
		m_pValueGroup[n]=dValue;
		n++;
	}

	nValueNumber=m_nValue;
	return m_pValueGroup;
}
	

