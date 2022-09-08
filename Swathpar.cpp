
#include "stdafx.h"
#include "struct.h"
#include "svsys.h"
#include "3dfstbrk.h"
#include "stdlib.h"   
#include "swathpar.h"
#include "fh.h"
   
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///     Construction code void SwathParameter::SwathParameter(int Swath)       BEGIN
SwathParameter::SwathParameter()   
{
	m_pRcvForOneShot=NULL;
	Reset();
}

SwathParameter::~SwathParameter()   
{
	Reset();
}

void SwathParameter::Reset()
{
	if(m_pRcvForOneShot){
		delete []m_pRcvForOneShot;
		m_pRcvForOneShot=NULL;
	}
    
	SwathParameterFileName="";

	Distance1=0;Distance2=0;
	Distance3=0;Distance4=0;
	ShotLineFrom=0;
	ShotLineTo=0;
	ShotLineNumber=0;
	TotalRecievePointNumber=0;
	InitialVelocity=1500;
	FirstRecievePointNumber=1;
	FoldTime=2;

	Group1=0;
	Group2=0;
	Group3=0;
	Group4=0;
	TotalUnformRecievePointNumber=0;
	SPNBoxNumber=50;
	RLNBoxNumber=50;

	TopPlusSquareLength=0;
	BottomPlusSquareLength=0;
	//LeftPlusSquareLength=0;
	//RightPlusSquareLength=0;

	DataOK=FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     Set Data for of swath.
BOOL SwathParameter::Set(int Swath)   
{
	Reset();		

	if(!ss.Set())return false;

	//
	if(Swath==-1){
		CDSwNum dlg;
		if(dlg.DoModal()==IDCANCEL)return false;
		Swath=dlg.m_SwathNumber ;
	}	


	// there are two swath parameter files with diffrent extentions.
	// so , here do not give extension,	 but save the right for later use.
	SetFileName(Swath);
			 
	if(!ReadCommonParameter())return FALSE;
	if(!ReadRLNSPN())return FALSE;
	
	CalculateParameters();

	DataOK=TRUE;
	return true;
}
///     BOOL SwathParameter::Set(int Swath)									   END    
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///     BOOL AskCommonParameter()                        BEGIN  //
BOOL SwathParameter::AskCommonParameter()
{
	cdswpar dlg;
	if(ReadCommonParameter()){
		dlg.m_Distance1=Distance1;
		dlg.m_Distance2=Distance2;
		dlg.m_Distance3=Distance3;
		dlg.m_Distance4=Distance4;
		dlg.m_ShotLineFrom=ShotLineFrom;
		dlg.m_ShotLineTo=ShotLineTo;
		dlg.m_InitialVelocity=InitialVelocity;
		dlg.m_FirstRecievePointNumber=FirstRecievePointNumber;
		dlg.m_FoldTime=FoldTime;
	}

	if(dlg.DoModal()==IDOK){
		WriteCommonParameter(&dlg);
		return TRUE;
	}
	return FALSE;
}
///     BOOL AskCommonParameter()                        END     //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////           
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///     BOOL AskRLNSPN()                        BEGIN  //
BOOL SwathParameter::AskRLNSPN()
{
	CDRLNSPN dlg;
    FILE *fp=fopen(SwathParameterFileName+".pa2","r");
    if(fp){
		 fscanf(fp,"%d %d %d %d %d %d %d %d %d %d\n",&dlg.m_RLN1,&dlg.m_RLN2,&dlg.m_RLN3,&dlg.m_RLN4,&dlg.m_RLN5,&dlg.m_RLN6,&dlg.m_RLN7,&dlg.m_RLN8,&dlg.m_RLN9,&dlg.m_RLN10);
		 fscanf(fp,"%d %d %d %d %d %d %d %d %d %d\n",&dlg.m_RLN11,&dlg.m_RLN12,&dlg.m_RLN13,&dlg.m_RLN14,&dlg.m_RLN15,&dlg.m_RLN16,&dlg.m_RLN17,&dlg.m_RLN18,&dlg.m_RLN19,&dlg.m_RLN20);
		 fscanf(fp,"%d %d %d %d %d %d %d %d %d %d\n",&dlg.m_RLN21,&dlg.m_RLN22,&dlg.m_RLN23,&dlg.m_RLN24,&dlg.m_RLN25,&dlg.m_RLN26,&dlg.m_RLN27,&dlg.m_RLN28,&dlg.m_RLN29,&dlg.m_RLN30);
		 fscanf(fp,"%d %d %d %d %d %d %d %d %d %d\n",&dlg.m_RLN31,&dlg.m_RLN32,&dlg.m_RLN33,&dlg.m_RLN34,&dlg.m_RLN35,&dlg.m_RLN36,&dlg.m_RLN37,&dlg.m_RLN38,&dlg.m_RLN39,&dlg.m_RLN40);
		 fscanf(fp,"%d %d %d %d %d %d %d %d %d %d\n",&dlg.m_RLN41,&dlg.m_RLN42,&dlg.m_RLN43,&dlg.m_RLN44,&dlg.m_RLN45,&dlg.m_RLN46,&dlg.m_RLN47,&dlg.m_RLN48,&dlg.m_RLN49,&dlg.m_RLN50);
    
		 fscanf(fp,"%d %d %d %d %d %d %d %d %d %d\n",&dlg.m_SPN1,&dlg.m_SPN2,&dlg.m_SPN3,&dlg.m_SPN4,&dlg.m_SPN5,&dlg.m_SPN6,&dlg.m_SPN7,&dlg.m_SPN8,&dlg.m_SPN9,&dlg.m_SPN10);
		 fscanf(fp,"%d %d %d %d %d %d %d %d %d %d\n",&dlg.m_SPN11,&dlg.m_SPN12,&dlg.m_SPN13,&dlg.m_SPN14,&dlg.m_SPN15,&dlg.m_SPN16,&dlg.m_SPN17,&dlg.m_SPN18,&dlg.m_SPN19,&dlg.m_SPN20);
		 fscanf(fp,"%d %d %d %d %d %d %d %d %d %d\n",&dlg.m_SPN21,&dlg.m_SPN22,&dlg.m_SPN23,&dlg.m_SPN24,&dlg.m_SPN25,&dlg.m_SPN26,&dlg.m_SPN27,&dlg.m_SPN28,&dlg.m_SPN29,&dlg.m_SPN30);
		 fscanf(fp,"%d %d %d %d %d %d %d %d %d %d\n",&dlg.m_SPN31,&dlg.m_SPN32,&dlg.m_SPN33,&dlg.m_SPN34,&dlg.m_SPN35,&dlg.m_SPN36,&dlg.m_SPN37,&dlg.m_SPN38,&dlg.m_SPN39,&dlg.m_SPN40);
		 fscanf(fp,"%d %d %d %d %d %d %d %d %d %d\n",&dlg.m_SPN41,&dlg.m_SPN42,&dlg.m_SPN43,&dlg.m_SPN44,&dlg.m_SPN45,&dlg.m_SPN46,&dlg.m_SPN47,&dlg.m_SPN48,&dlg.m_SPN49,&dlg.m_SPN50);
		 fclose(fp);
	}						
				 
	if(dlg.DoModal()==IDOK){
		WriteRLNSPN(&dlg);
		return TRUE;
	}
	return FALSE;
}
///     BOOL AskRLNSPN()                        END                 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////           
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     BOOL ReadCommonParameter(int Swath)	 BEGIN  
BOOL SwathParameter::ReadCommonParameter()
{
	FILE *fp=fopen(SwathParameterFileName+".pa1","r");
	if(!fp){
		AfxMessageBox("本束线的参数文件没有找到!");
		return FALSE;
	 }
	fscanf(fp,"%ld",&ShotLineFrom);
	fscanf(fp,"%ld",&ShotLineTo);  
	fscanf(fp,"%ld",&Distance1);
	fscanf(fp,"%ld",&Distance2);
	fscanf(fp,"%ld",&Distance3);
	fscanf(fp,"%ld",&Distance4);
	fscanf(fp,"%ld",&InitialVelocity);
	fscanf(fp,"%ld",&FirstRecievePointNumber);
	fscanf(fp,"%ld",&FoldTime);

	if(ShotLineFrom>ShotLineTo){
		long n=ShotLineFrom;
		ShotLineFrom=ShotLineTo;
		ShotLineTo=n;
	}

	fclose(fp);

	return TRUE;
}	        
//     BOOL ReadCommonParameter(int Swath)											END  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    BOOL ReadRLNSPN()																			BEGIN
BOOL SwathParameter::ReadRLNSPN()	        
{	        
	FILE *fp=fopen(SwathParameterFileName+".pa2","r");
	if(!fp){
		AfxMessageBox("本束线的第二参数文件:"+SwathParameterFileName+".PA2 没有找到!");
	    return FALSE;
	}
	
	int i;
	for(i=0;i<RLNBoxNumber;i++)fscanf(fp,"%d",&RecieveLineName[i]);
	for(i=0;i<SPNBoxNumber;i++)fscanf(fp,"%d",&ShotPointName[i]);

	return TRUE;
}	        
//    BOOL ReadRLNSPN()                        													END
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	        	       
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     BOOL WriteCommonParameter(int Swath)										BEGIN  
BOOL SwathParameter::WriteCommonParameter(cdswpar *dlg)
{
	FILE *fp=fopen(LPCTSTR(SwathParameterFileName+".pa1"),"w");
	if(!fp){
		AfxMessageBox("无法建立本束线的第一参数文件"+SwathParameterFileName+".PA1 !");
		return FALSE;
	}
	fprintf(fp,"%ld\n",dlg->m_ShotLineFrom);
	fprintf(fp,"%ld\n",dlg->m_ShotLineTo);  
	fprintf(fp,"%ld\n",dlg->m_Distance1);
	fprintf(fp,"%ld\n",dlg->m_Distance2);
	fprintf(fp,"%ld\n",dlg->m_Distance3);
	fprintf(fp,"%ld\n",dlg->m_Distance4);
	fprintf(fp,"%ld\n",dlg->m_InitialVelocity);
	fprintf(fp,"%ld\n",dlg->m_FirstRecievePointNumber);
	fprintf(fp,"%ld\n",dlg->m_FoldTime);

	fclose(fp);
	return TRUE;
}
//     BOOL WriteCommonParameter(int Swath)											END  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	        
//    BOOL WriteRLNSPN()																		BEGIN
BOOL SwathParameter::WriteRLNSPN(CDRLNSPN *dlg)	        
{
        FILE *fp=fopen(SwathParameterFileName+".pa2","w");
        if(!fp){
			AfxMessageBox("无法建立本束线的第二参数文件"+SwathParameterFileName+".PA2");
			return FALSE;
		}
		else{
	         fprintf(fp,"%d %d %d %d %d %d %d %d %d %d\n",dlg->m_RLN1,dlg->m_RLN2,dlg->m_RLN3,dlg->m_RLN4,dlg->m_RLN5,dlg->m_RLN6,dlg->m_RLN7,dlg->m_RLN8,dlg->m_RLN9,dlg->m_RLN10);
             fprintf(fp,"%d %d %d %d %d %d %d %d %d %d\n",dlg->m_RLN11,dlg->m_RLN12,dlg->m_RLN13,dlg->m_RLN14,dlg->m_RLN15,dlg->m_RLN16,dlg->m_RLN17,dlg->m_RLN18,dlg->m_RLN19,dlg->m_RLN20);
             fprintf(fp,"%d %d %d %d %d %d %d %d %d %d\n",dlg->m_RLN21,dlg->m_RLN22,dlg->m_RLN23,dlg->m_RLN24,dlg->m_RLN25,dlg->m_RLN26,dlg->m_RLN27,dlg->m_RLN28,dlg->m_RLN29,dlg->m_RLN30);
             fprintf(fp,"%d %d %d %d %d %d %d %d %d %d\n",dlg->m_RLN31,dlg->m_RLN32,dlg->m_RLN33,dlg->m_RLN34,dlg->m_RLN35,dlg->m_RLN36,dlg->m_RLN37,dlg->m_RLN38,dlg->m_RLN39,dlg->m_RLN40);
             fprintf(fp,"%d %d %d %d %d %d %d %d %d %d\n",dlg->m_RLN41,dlg->m_RLN42,dlg->m_RLN43,dlg->m_RLN44,dlg->m_RLN45,dlg->m_RLN46,dlg->m_RLN47,dlg->m_RLN48,dlg->m_RLN49,dlg->m_RLN50);
                    
             fprintf(fp,"%d %d %d %d %d %d %d %d %d %d\n",dlg->m_SPN1,dlg->m_SPN2,dlg->m_SPN3,dlg->m_SPN4,dlg->m_SPN5,dlg->m_SPN6,dlg->m_SPN7,dlg->m_SPN8,dlg->m_SPN9,dlg->m_SPN10);
             fprintf(fp,"%d %d %d %d %d %d %d %d %d %d\n",dlg->m_SPN11,dlg->m_SPN12,dlg->m_SPN13,dlg->m_SPN14,dlg->m_SPN15,dlg->m_SPN16,dlg->m_SPN17,dlg->m_SPN18,dlg->m_SPN19,dlg->m_SPN20);
             fprintf(fp,"%d %d %d %d %d %d %d %d %d %d\n",dlg->m_SPN21,dlg->m_SPN22,dlg->m_SPN23,dlg->m_SPN24,dlg->m_SPN25,dlg->m_SPN26,dlg->m_SPN27,dlg->m_SPN28,dlg->m_SPN29,dlg->m_SPN30);
             fprintf(fp,"%d %d %d %d %d %d %d %d %d %d\n",dlg->m_SPN31,dlg->m_SPN32,dlg->m_SPN33,dlg->m_SPN34,dlg->m_SPN35,dlg->m_SPN36,dlg->m_SPN37,dlg->m_SPN38,dlg->m_SPN39,dlg->m_SPN40);
             fprintf(fp,"%d %d %d %d %d %d %d %d %d %d\n",dlg->m_SPN41,dlg->m_SPN42,dlg->m_SPN43,dlg->m_SPN44,dlg->m_SPN45,dlg->m_SPN46,dlg->m_SPN47,dlg->m_SPN48,dlg->m_SPN49,dlg->m_SPN50);
             fclose(fp);
			 return TRUE;
         }
}         
//    BOOL WriteRLNSPN()																			END
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	     
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//     BOOL CalculateParameters()															BEGIN  
BOOL SwathParameter::CalculateParameters()
{
	long  L;
	
	//Calculate 1.	ShotLineNumber
	ShotLineNumber=ShotLineTo-ShotLineFrom+1;

	//Calculate 4.  Group1,2,3,4.  thay have been set to 0 in "init".
	if(Distance1!=0)
		Group1=ss.GroupNumberOfSmallNumber-
			(Distance1-ss.GapOfSmallNumber)/ss.GroupInterval;	  // right.
	if(Distance2!=0)
		Group2=ss.GroupNumberOfSmallNumber-
			(Distance2-ss.GapOfSmallNumber)/ss.GroupInterval;   // right.
	if(Distance3!=0)
		Group3=ss.GroupNumberOfSmallNumber+
			(Distance3-ss.GapOfBigNumber)/ss.GroupInterval+1;
	if(Distance4!=0)
		Group4=ss.GroupNumberOfSmallNumber+
			(Distance4-ss.GapOfBigNumber)/ss.GroupInterval+1;


	//Calculate 6.TotalRecievePointNumber:
	L=0;
	if((Distance1!=0)&&(Distance4!=0)){
		L=Distance2+Distance3;
	}
	L+=(ShotLineNumber-1)*ss.ShotLineInterval;   // ShotLineNumber was calculated above.
	L+=(Distance1-Distance2)+(Distance4-Distance3);
	L=L/ss.GroupInterval+1;
	TotalRecievePointNumber=L*ss.RecieveLineNumber;

	// Calculate 8.TotalUnformRecievePointNumber.
	TotalUnformRecievePointNumber=ss.RecieveLineNumber*
			(Group2-Group1+Group4-Group3+2);
	if(Group1==0)TotalUnformRecievePointNumber-=ss.RecieveLineNumber;
	if(Group4==0)TotalUnformRecievePointNumber-=ss.RecieveLineNumber;

	if(m_pRcvForOneShot){
		delete []m_pRcvForOneShot;
		m_pRcvForOneShot=NULL;
	}
	m_pRcvForOneShot=new CRcvPoint[TotalUnformRecievePointNumber];	
	
	// Parameters are calculated over!
	return TRUE;

}                

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  名称: MakePh
//  功能: 根据炮点位置制造理论桩号.
//  输入:  Long *Ph, 接收理论桩号的起始地址,
//			  PhNumber, 接收理论桩号数目的变量地址.	
//	改变: 对Ph赋值.
//            对PhNumber赋值. 
//  日期:1997.9.29
BOOL SwathParameter::MakeShotPh(long *Ph,long *PhNumber)
{
			   
	SvSys ss;
	if(!ss.DataOK){
		AfxMessageBox("观测系统参数尚未输入!");
		return FALSE;
	}

	long i,j,n;
	for(i=ShotLineFrom;i<=ShotLineTo;i++){
		for(j=0;j<ss.ShotPointNumber;j++){
			n=(i-ShotLineFrom)*ss.ShotPointNumber+j;
			Ph[n]=i*1000+ShotPointName[j];
		}
	}

	*PhNumber=ss.ShotPointNumber*(ShotLineTo-ShotLineFrom+1);
	return TRUE;
}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//	功能:		根据炮线号和炮点号形成桩号.
//          		目前仅为炮点号和炮线号各3位数,桩号为6位数,
//                   有待改进.
//  日期:  1998.10.5
long SwathParameter::MakePh(long nShotLine,long nShotPoint)
{
	return nShotLine*1000+nShotPoint;
}


bool SwathParameter::SetFileName(int nSwath)
{
	SwathParameterFileName="swath"+IntToName(nSwath);  
	SwathParameterFileName1=SwathParameterFileName+".pa1";
	SwathParameterFileName2=SwathParameterFileName+".pa2";
	return true;
}

////////////////////////////////////////////////////////////
//    炮点的X,Y 坐标是相对于解编的检波点矩形来计算的.
CMyPoint SwathParameter::GetShotPos(long nShotPh,long nZP,long nHP)
{
	// Calculate which point  in the shot line is it.
	int nShotLine=nShotPh/1000;
	int nShotPointName=nShotPh-nShotLine*1000;
	
	double x=-1,y=-1;
	for(long i=0;i<SPNBoxNumber;i++){
		if(ShotPointName[i]==nShotPointName){
			x=ss.ShotPointPosition[i];
			break;
		}
	}
	x+=nHP;

	// Calc the y position of the shot point:
	// 计算炮点的Y坐标.根据三种情况: 大号,小号,两边解编.
	y=(nShotLine-ShotLineFrom)*ss.ShotLineInterval +nZP;
	if(Distance1==0)                            // 大号解编.
		y-=Distance3;  			    
	else													// 两边解编或小号解编.
		y+=Distance1;              // 无论两边解编还是小号解编, 这个公式都适合.

	// return:
	return CMyPoint(x,y);

}

CRcvPoint * SwathParameter::GetRcvPoint(long nShotPh)
{

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//3.  计算该炮点的所有检波点的序号. 
	//  根据三种情况 1.大号,小号,两边解编.
	//       RN, is according picking sequence -- along recieve line,
	//              well its recieve point number , is perpendicular to
	//              to recieve line.
	int nShotLine=nShotPh/1000;
	int nShotPointName=nShotPh-nShotLine*1000;
	
	long i,j,n,PlusedNumberRow;
	
	// Group Number before the shot , that should be plused.
	long PlusedNumberTotal=((nShotLine-ShotLineFrom)*ss.ShotLineInterval/ss.GroupInterval)*ss.RecieveLineNumber;

	// Calculate GroupNumberEveryUnformLine.
	long nGroupNumberEveryUnformLine=0;
   	if(Group1!=0)nGroupNumberEveryUnformLine=Group2-Group1+1;
	if(Group3!=0)nGroupNumberEveryUnformLine+=Group4-Group3+1;
	
	// Loop to calculate RecievePointNumber.
	long RecievePointNumber;
	if(Distance1==0){         // One direction Pick First Break. Big Number Pick.
			// Loop according Recieve points in Y direction.
			for(i=0;i<Group4-Group3+1;i++){  
					PlusedNumberRow=PlusedNumberTotal+i*ss.RecieveLineNumber;
					n=i-nGroupNumberEveryUnformLine;
					for(j=0;j<ss.RecieveLineNumber;j++){
							RecievePointNumber=PlusedNumberRow+j;
	
							// to raise the velocity, calculate n this way,
							//not " j*nGroupNumberEveryUnformLine+i",
							// avoiding one time of " time * ".
							n+=nGroupNumberEveryUnformLine;				   
							m_pRcvForOneShot[n].lNo=RecievePointNumber;
					}
			}      
	}              
	else if(Distance3==0){          // Small Number direction pick .
			for(i=0;i<(Group2-Group1+1);i++){  
					PlusedNumberRow=PlusedNumberTotal+i*ss.RecieveLineNumber;
					n=i-nGroupNumberEveryUnformLine;      // below will plus GNEUL every r line .
					for(j=0;j<ss.RecieveLineNumber;j++){
							RecievePointNumber=PlusedNumberRow+j;
	
							// to raise the velocity, calculate n this way,
							//not " j*nGroupNumberEveryUnformLine+i",
							// avoiding one time of " time * ".
							n+=nGroupNumberEveryUnformLine;				   
							m_pRcvForOneShot[n].lNo=RecievePointNumber;
					}
			}      
	
	}
	else{	// Two diretion Picking First Break.

			// First half of RecievePointNumber: RN
			for(i=0;i<(Group2-Group1+1);i++){
					PlusedNumberRow=PlusedNumberTotal+i*ss.RecieveLineNumber;	
					n=i-nGroupNumberEveryUnformLine;
					for(j=0;j<ss.RecieveLineNumber;j++){
							RecievePointNumber=PlusedNumberRow+j;
							n+=nGroupNumberEveryUnformLine;
							m_pRcvForOneShot[n].lNo=RecievePointNumber;	
					}
			}
			// Second  half of RecievePointNumber.
			// Firstly, the total plused recieve point number should 
			// plus first half recieve point.
			PlusedNumberTotal+=(Distance1+Distance3)/ss.GroupInterval*ss.RecieveLineNumber;

			int sn=Group2-Group1+1;
			for(i=0;i<(Group4-Group3+1);i++){
					PlusedNumberRow=PlusedNumberTotal+i*ss.RecieveLineNumber;	
					n=sn+i-nGroupNumberEveryUnformLine;
					for(j=0;j<ss.RecieveLineNumber;j++){
							RecievePointNumber=PlusedNumberRow+j;
							n+=nGroupNumberEveryUnformLine;
							m_pRcvForOneShot[n].lNo=RecievePointNumber;	
					}
			}
	}// Two shot direction .
	
	////////////////////////////////////////////////////////////////////////////////////
	//	Calc the positions of the rcv points of one shot:
	for(i=0;i<TotalUnformRecievePointNumber;i++){
		n=m_pRcvForOneShot[i].lNo;

		m_pRcvForOneShot[i].Pos.x=ss.RecieveLinePosition[n%ss.RecieveLineNumber];
		m_pRcvForOneShot[i].Pos.y=n/ss.RecieveLineNumber*ss.GroupInterval;
	}

	// return
	return m_pRcvForOneShot;

}
