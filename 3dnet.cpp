// 3DNet.cpp: implementation of the C3DNet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3dfstbrk.h"
#include "3DNet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DNet::C3DNet()
{
}

bool C3DNet::Set(int Swath)
{
	SvSys SS;
	if(SS.DataOK)return false;
	SwathParameter SP;
	if(!SP.Set(Swath))return false;
	ShotPointParDealer SPPD(Swath);
	ShotPointPar  *SPP=new ShotPointPar[SPPD.m_TotalRSNumber];
	if(!SPPD.ReadSwath(SPP))return false;


	/*      TopPlusSquareLength,
            LeftPlusSquareLength,
            RightPlusSquareLength
                
                  *  *       *  *       *  * ----
                                                 TP
                  *  * .    .*  *.    . *  * ----
                        .    .    .    .
                  *  * .    .*  *.    . *  *
                  | LP |              | RP | 
     */

	
	if(SP.Distance3==0)BottomPlusLength=SP.Distance2;
	if(SP.FirstShotPointPosition<0)
		LeftPlusLength=-SS.FirstShotPointPosition;

	long LS=SS.WidthOfShotPoint+SS.FirstShotPointPosition;
	if(LS>WidthOfRecievePoint)
		RightPlusSquareLength=LS-SS.WidthOfRecievePoint;

	long Ph;
	long n;
	float PY=0;

	// º∆À„ TopPlusLen
	if(SP.Distance1==0)TopPlusLen=SP.Distance3;
	for(int i=0;i<SS.ShotPointNumber;i++){
		Ph=SP.MakePh(SP.ShotLineFrom,i);
		n=SPPD.SearchPhInSPP(Ph,SPP,SPPD.m_TotalRSNumber);
		if(n==-1)return false;
		if(SPP[n].ZP<PY)PY=SPP[n].ZP;
	}
	if(-PY>SP.Distance4)
		TopPlusLen+=(-ZP;
	}




	double dWidth=SP.WidthOfRecievePoint


	
	return true;
}

C3DNet::~C3DNet()
{
}

long C3DNet::GetTrace(CMyPoint p1,
					  CMyPoint p2,
					  CBlockTrace* pBlock,
					  long nLimit)
{

