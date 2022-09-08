// ZDEquation.cpp: implementation of the CZDEquation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3dfstbrk.h"
#include "ZDEquation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZDEquation::CZDEquation()
{
	Reset();
	m_Couple=new CCouple[SHOT_LIMIT][HALF_RCV_LIMIT];
	m_SecondCouple=new CCouple[SHOT_LIMIT][QUARTER_RCV_LIMIT];
	m_ThirdCouple=new CCouple[SHOT_LIMIT][EIGHTH_RCV_LIMIT];

	m_dSmallDistance=-1;
	m_dBigDistance=-1;

	m_dPrecision=0.70;
}

CZDEquation::~CZDEquation()
{
	Reset();
	delete []m_Couple;
	delete []m_SecondCouple;
	delete []m_ThirdCouple;

}

bool CZDEquation::Construct(CString sA,CString sB,long nShotNum,long nRcvNum,float fInitVel)
{
	if(!m_Equation.IsIdle ())return false;
	if(!m_Equation.Construct(sA,sB,nShotNum+nRcvNum))return false;

	m_sEquationA=sA;
	m_sEquationB=sB;

	m_lShotPointNumber=nShotNum;
	m_lRcvPointNumber=nRcvNum;
	m_fInitVel=fInitVel;
}

void CZDEquation::SetFoldTime(int nFoldTime)
{
	if(nFoldTime<1)m_nFoldTime=1;
	if(nFoldTime>3)m_nFoldTime=3;
	m_nFoldTime=nFoldTime;
}

bool CZDEquation::Append(CShotRcv *pShotRcv,int nShotRcv)
{
	if(!m_Equation.IsMaking ())return false;
	if(m_dSmallDistance==-1||m_dBigDistance==-1){
		AfxMessageBox("Programming Error: You must firstly set a common group of shot point using SetCommonShotGroup() before append equations!");
		return false;
	}

	CalcCouple(pShotRcv,nShotRcv);

	long nShot,j,k;
	long nCoupleNumber=0;

	CBlockTrace BlockTrace[10];
	long r[10];
	CObArray array;
	float fFbk;

	if(m_nFoldTime==1){  
		array.SetSize (3);
		// The couple number should be same as the fold time, so 
		// we check the data here:

		for(nShot=0;nShot<nShotRcv;nShot++){
			for(j=0;j<m_nCoupleNumber[nShot];j++){  // loop of couples of this shot.
				r[0]=m_Couple[nShot][j].nRcvA;
				r[1]=m_Couple[nShot][j].nRcvB;
				if(r[0]==-1||r[1]==-1)continue;
				
				fFbk=0.0;
				for(k=0;k<2;k++){
					fFbk+=pShotRcv[nShot].Rcv [r[k]].Fbk ;
				}
				
				BlockTrace[0].Set(pShotRcv[nShot].Shot .lNo ,2);
				
				for(k=0;k<2;k++){
					BlockTrace[k+1].Set(pShotRcv[nShot].Rcv [r[k]].lNo +m_lShotPointNumber,1);
				}

				for(k=0;k<3;k++){
					array.SetAt(k,&BlockTrace[k]);					
				}

				if(!m_Equation.AppeEqua (array,fFbk))return false;
			}
		}
	}
	else if(m_nFoldTime==2){
		array.SetSize (5);
		// The couple number should be same as the fold time, so 
		// we check the data here:

		for(nShot=0;nShot<nShotRcv;nShot++){
			for(j=0;j<m_nSecondCoupleNumber[nShot];j++){  // loop of couples of this shot.
				r[0]=m_SecondCouple[nShot][j].nRcvA;
				r[1]=m_SecondCouple[nShot][j].nRcvB;
				
				r[2]=SearchCoupleData(r[0],m_Couple[nShot],m_nCoupleNumber[nShot]);
				r[3]=SearchCoupleData(r[1],m_Couple[nShot],m_nCoupleNumber[nShot]);
				if(r[0]==-1||r[1]==-1||r[2]==-1||r[3]==-1)continue;

				fFbk=0.0;
				for(k=0;k<4;k++){
					fFbk+=pShotRcv[nShot].Rcv [r[k]].Fbk ;
				}
				
				BlockTrace[0].Set(pShotRcv[nShot].Shot.lNo ,4);
				
				for(k=0;k<4;k++){
					BlockTrace[k+1].Set(pShotRcv[nShot].Rcv [r[k]].lNo +m_lShotPointNumber ,1);
				}

				for(k=0;k<5;k++){
					array.SetAt(k,&BlockTrace[k]);
				}

				if(!m_Equation.AppeEqua (array,fFbk))return false;
			}
		}
	}
	// make Three times fold .
	else if(m_nFoldTime==3){
		array.SetSize (9);

		// The couple number should be same as the fold time, so 
		// we check the data here:
		for(nShot=0;nShot<nShotRcv;nShot++){
			for(j=0;j<m_nThirdCoupleNumber[nShot];j++){  
				r[0]=m_ThirdCouple[nShot][j].nRcvA;
				r[1]=m_ThirdCouple[nShot][j].nRcvB;
				r[2]=SearchCoupleData(r[0],m_SecondCouple[nShot],m_nSecondCoupleNumber[nShot]);
				r[3]=SearchCoupleData(r[1],m_SecondCouple[nShot],m_nSecondCoupleNumber[nShot]);
				r[4]=SearchCoupleData(r[0],m_Couple[nShot],m_nCoupleNumber[nShot]);
				r[5]=SearchCoupleData(r[1],m_Couple[nShot],m_nCoupleNumber[nShot]);
				r[6]=SearchCoupleData(r[2],m_Couple[nShot],m_nCoupleNumber[nShot]);
				r[7]=SearchCoupleData(r[3],m_Couple[nShot],m_nCoupleNumber[nShot]);
				if(r[0]==-1||r[1]==-1||r[2]==-1||r[3]==-1||r[4]==-1||r[5]==-1||r[6]==-1||r[7]==-1)continue;
					

				fFbk=0.0;
				for(k=0;k<8;k++){
					fFbk+=pShotRcv[nShot].Rcv [r[k]].Fbk ;
				}
				
				BlockTrace[0].Set(pShotRcv[nShot].Shot.lNo ,8);
				
				for(k=0;k<8;k++){
					BlockTrace[k+1].Set(pShotRcv[nShot].Rcv [r[k]].lNo +m_lShotPointNumber,1);
				}

				for(k=0;k<9;k++){
					array.SetAt(k,&BlockTrace[k]);
				}

				if(!m_Equation.AppeEqua (array,fFbk))return false;
			}
		}
	}

	return true;
}

bool CZDEquation::Close()
{
	Reset();
	return true;
}

void CZDEquation::Reset()
{
	if(!m_Equation.IsIdle())m_Equation.Close ();

	m_sEquationA="";
	m_sEquationB="";

	m_lShotPointNumber=0;
	m_lRcvPointNumber=0;

	m_fInitVel=2000;

	for(int i=0;i<SHOT_LIMIT;i++){
		m_nCoupleNumber[i]=0;
		m_nSecondCoupleNumber[i]=0;
		m_nThirdCoupleNumber[i]=0;
	}

	m_nFoldTime=1;

}

bool CZDEquation::CalcCouple(CShotRcv *pShotRcv,int nShotRcv)
{
	long  i,j,k;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    Maximum permitted unformed group of one shot:         1000
	//    NumberLength : recorded the number of the unformed r-points 
	// and their distance from the shot point,  its not a calss member.
	for(i=0;i<nShotRcv;i++){  // 对炮点循环。		
		CalcWeight(&pShotRcv[i]);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//       Sort   NumberLength according to the distance 
	//  of the recieve points to the shot by every shot.
	double d,dPrecision=0.0;
	for(i=0;i<nShotRcv;i++){
		SortRcv(&pShotRcv[i]);
		for(j=1;j<pShotRcv[i].lRcvNum ;j++){
			d=pShotRcv[i].Rcv [j].dWeight -pShotRcv[i].Rcv [j-1].dWeight ;
			if(d>dPrecision){
				dPrecision=d;
			}
		}
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Because the distances of the r-p to the shot of all of the shots must be same,
	// so when plusing two R-P that is same far from their shot point, so must search 
	// for the distance that all of the shot have .
	// 1.Search for Longest distance of the nearest Unform r-point in all of the shots.
	// 2.Search for Shortest distance of the farest Unform r-point in all of the shots.
	// 3.Calculate the middle distance that split the first half plused group and the second hlf.
	double 	SmallDistance=0;
	double 	BigDistance=10000000;

	for(i=0;i<nShotRcv;i++){  // We have sorted the distances:
		if(pShotRcv[i].Rcv [0].dWeight >SmallDistance){
			SmallDistance=pShotRcv[i].Rcv [0].dWeight ;
		}
		if(pShotRcv[i].Rcv [pShotRcv[i].lRcvNum -1].dWeight <BigDistance){
			BigDistance=pShotRcv[i].Rcv [pShotRcv[i].lRcvNum -1].dWeight;
		}
	}	

	SmallDistance=m_dSmallDistance;
	BigDistance=m_dBigDistance;
	
	double MidDistance=(BigDistance-SmallDistance)/2+SmallDistance;	
	double QuaDistance=(BigDistance-SmallDistance)/4+SmallDistance;	   
	double EighthDistance=(BigDistance-SmallDistance)/8+SmallDistance;
																								
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    Calculate couple data to be plused , according distance.
	//    m_CouplePlused : recording the two r-point number that will be plused.
	//Make 3 copies of the below, to avoid too many parameters if make it a subroutine.

	double dsmall,dbig1,dbig2,d1,d2;  // Temporary variable.
	int num;  // stand for number of plused couples of any shot point .
	dPrecision=dPrecision*(1-m_dPrecision);
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1. Calculate the first plused couple according to the Middle distance.
	for(i=0;i<nShotRcv;i++){
		num=0;
		for(j=0;j<pShotRcv[i].lRcvNum ;j++){
			
			// diffrence of the the R point and the SmallDistance.
			dsmall=pShotRcv[i].Rcv [j].dWeight -SmallDistance;
			if(dsmall<0)continue;
			if(pShotRcv[i].Rcv [j].dWeight >MidDistance){
				m_nCoupleNumber[i]=num;
				break;		
			}

			m_Couple[i][num].nRcvA=j;

			//    search for the second R point whose diffrence with BigDistance 
			// match the dsmall.
			for(k=pShotRcv[i].lRcvNum -1;k>0;k--){   // should from sp._ to 0, see below.
				dbig1=BigDistance-pShotRcv[i].Rcv[k].dWeight ;
				dbig2=BigDistance-pShotRcv[i].Rcv[k-1].dWeight ;
				
				// Need not consider of dbig2<0, for dsmall>0.
				if(dbig2>=dsmall&&dbig1<=dsmall){
					d1=fabs(dsmall-dbig1);
					d2=fabs(dbig2-dsmall);

					d=(d1<d2)?d1:d2;
					if(d>dPrecision){
						m_Couple[i][num].nRcvB=-1;
					}
					else{
						m_Couple[i][num].nRcvB=(d1<d2)?k:k-1;
					}
					num++;
					break;
				}
			}
			
		}  // R point loop.
		
	}  // shot point loop to calculate the number of the two plused r-point.
	
	if(m_nFoldTime<2)return true;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 2. Calculate the second plused couple between the QuaDistance 
	//      on the first half distance.	
	BigDistance=MidDistance;
	for(i=0;i<nShotRcv;i++){    // Loop for all of the shots.
		num=0;
		for(j=0;j<pShotRcv[i].lRcvNum ;j++){  // Loop for all of the recieve points.
			
			// diffrence of the the R point and the SmallDistance.
			dsmall=pShotRcv[i].Rcv [j].dWeight -SmallDistance;
			if(dsmall<0)continue;
			if(pShotRcv[i].Rcv [j].dWeight >QuaDistance){
				m_nSecondCoupleNumber[i]=num;
				break;		
			}
			m_SecondCouple[i][num].nRcvA=j;

			//    search for the second R point whose diffrence with BigDistance 
			// match the dsmall.
			for(k=pShotRcv[i].lRcvNum -1;k>0;k--){   // should from sp._ to 0, see below.
				dbig1=BigDistance-pShotRcv[i].Rcv [k].dWeight ;
				dbig2=BigDistance-pShotRcv[i].Rcv [k-1].dWeight ;
				
				// Need not consider of dbig2<0, for dsmall>0.
				if(dbig2>=dsmall&&dbig1<=dsmall){  
					d1=fabs(dsmall-dbig1);
					d2=fabs(dbig2-dsmall);

					d=(d1<d2)?d1:d2;
					if(d>dPrecision){
						m_SecondCouple[i][num].nRcvB=-1;
					}
					else{
						m_SecondCouple[i][num].nRcvB=(d1<d2)?k:k-1;
					}

					num++;
					break;
				}
			}
		}  // R point loop.
	}  // shot point loop to calculate the number of the two plused r-point.
	if(m_nFoldTime<3)return true;


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 3. Calculate the third plused couple between the EighthDistance 
	//      on the first Quadistance.
	BigDistance=QuaDistance;
	for(i=0;i<nShotRcv;i++){    // Loop for all of the shots.
		num=0;
		for(j=0;j<pShotRcv[i].lRcvNum ;j++){  // Loop for all of the recieve points.
			
			// diffrence of the the R point and the SmallDistance.
			dsmall=pShotRcv[i].Rcv [j].dWeight -SmallDistance;
			if(dsmall<0)continue;
			if(pShotRcv[i].Rcv [j].dWeight >EighthDistance){
				m_nThirdCoupleNumber[i]=num;
				break;		
			}
			m_ThirdCouple[i][num].nRcvA=j;

			//    search for the second R point whose diffrence with BigDistance 
			// match the dsmall.
			for(k=pShotRcv[i].lRcvNum -1;k>0;k--){   // should from sp._ to 0, see below.
				dbig1=BigDistance-pShotRcv[i].Rcv [k].dWeight ;
				dbig2=BigDistance-pShotRcv[i].Rcv [k-1].dWeight ;
				
				// Need not consider of dbig2<0, for dsmall>0.
				if(dbig2>=dsmall&&dbig1<=dsmall){  
					d1=fabs(dsmall-dbig1);
					d2=fabs(dbig2-dsmall);

					d=(d1<d2)?d1:d2;
					if(d>dPrecision){
						m_ThirdCouple[i][num].nRcvB=-1;
					}
					else{
						m_ThirdCouple[i][num].nRcvB=(d1<d2)?k:k-1;
					}

					num++;
					break;
				}
			}
		}  // R point loop.
	}  // shot point loop to calculate the number of the two plused r-point.

	return true;
}

long CZDEquation::SearchCoupleData(long nRcv,CCouple *pCouple,long nCoupleNumber)
{
	for(long i=0;i<nCoupleNumber;i++){
		if(pCouple[i].nRcvA ==nRcv){
			return pCouple[i].nRcvB;
		}
	}
	return -1;
}

////////////////////////////////////////////////
// The programmer give a line of shot points,
// Not only one shot:
void CZDEquation::SetCommonShotGroup(CShotRcv *pShotRcv, int nShotRcv)
{
	m_dSmallDistance=-1000000;
	m_dBigDistance=1000000;

	for(int i=0;i<nShotRcv;i++){
		CalcWeight(&pShotRcv[i]);
		SortRcv(&pShotRcv[i]);
		if(pShotRcv[i].Rcv [0].dWeight >m_dSmallDistance)
			m_dSmallDistance=pShotRcv[i].Rcv [0].dWeight ;
		if(pShotRcv[i].Rcv [pShotRcv[i].lRcvNum -1].dWeight <m_dBigDistance)
			m_dBigDistance=pShotRcv[i].Rcv [pShotRcv[i].lRcvNum -1].dWeight ;
	}
}

void CZDEquation::SortRcv(CShotRcv *pShotRcv)
{
	CRcvPoint swap;
	double dTempLength;
	long j,k;
	for(j=0;j<pShotRcv->lRcvNum ;j++){
		swap=pShotRcv->Rcv [j];
		dTempLength=pShotRcv->Rcv [j].dWeight ;		

		for(k=j;k>0;k--){
			if(pShotRcv->Rcv [k-1].dWeight >dTempLength){
				pShotRcv->Rcv [k]=pShotRcv->Rcv [k-1];
			}
			else{
				break;
			}
		}
		pShotRcv->Rcv [k]=swap;
	}
}

void CZDEquation::CalcWeight(CShotRcv *pShotRcv)
{
	double dx,dy;
	for(long j=0;j<pShotRcv->lRcvNum ;j++){  // 对检波线循环。
		dx=pShotRcv->Shot.Pos.x-pShotRcv->Rcv[j].Pos.x;
		dy=pShotRcv->Shot.Pos.y-pShotRcv->Rcv[j].Pos.y;
		pShotRcv->Rcv[j].dWeight=sqrt(dx*dx+dy*dy);
		pShotRcv->Rcv[j].Fbk -=pShotRcv->Rcv[j].dWeight/m_fInitVel*1000;
	}
}

bool CZDEquation::SetPrecision(double dPrecision)
{
	if(dPrecision>=0&&dPrecision<=1){
		m_dPrecision=dPrecision;
		return true;
	}
	return false;
}

