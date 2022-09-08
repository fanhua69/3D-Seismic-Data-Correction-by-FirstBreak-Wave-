        
#include "stdafx.h"
#include "3dfstbrk.h"
#include "drlispi.h"
#include "struct.h"                 
#include "svsys.h"
#include "dsvsys.h"
#include"stdio.h"
                
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      SvSys Construction Code								BEGIN
SvSys::SvSys()
{
	Reset();
}
//      SvSys Construction Code								END
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////        	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////        	
//      BOOL  SvSys::ReadCommon Parameter                                 BEGIN
BOOL  SvSys::ReadCommonParameter() 
{
   FILE *fp=fopen("svsys1.par","rt");
   if(!fp)return false;
   char c[20];   
   
   fscanf(fp,"%s",c);Area=CString(c);
   fscanf(fp,"%s",c);Crew=CString(c);
   fscanf(fp,"%ld",&GroupInterval);
   fscanf(fp,"%ld",&RecieveLineNumber);
   fscanf(fp,"%ld",&ShotPointNumber);
   fscanf(fp,"%ld",&ShotLineInterval);                                  
   fscanf(fp,"%ld",&GroupNumberOfSmallNumber);
   fscanf(fp,"%ld",&GroupNumberOfBigNumber);
   fscanf(fp,"%ld",&GapOfSmallNumber);
   fscanf(fp,"%ld",&GapOfBigNumber);
   fscanf(fp,"%ld",&FirstShotPointPosition);

   fclose(fp);
   return true;
}
//                           BOOL  SvSys::ReadCommonParameter()                          END
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////        	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                           BOOL  SvSys::ReadRLISPI()
BOOL  SvSys::ReadRLISPI() 
{                          
	FILE* fp;
	if((fp=fopen("svsys2.par","r"))==NULL)return FALSE;

	int i;
	for(i=0;i<RLIBoxNumber;i++)fscanf(fp,"%ld ",&RecieveLineInterval[i]);
	for(i=0;i<SPIBoxNumber;i++)fscanf(fp,"%ld ",&ShotPointInterval[i]);
	return TRUE;
}                
//                           BOOL  SvSys::ReadRLISPI()                                             END
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                        BOOL  SvSys::AskCommonParameter()                               BEGIN
BOOL  SvSys::AskCommonParameter() 
{
	CDSvSys dlg;
	if(ReadCommonParameter()){
		dlg.m_Area=Area;
		dlg.m_Crew=Crew;
		dlg.m_GroupInterval=GroupInterval;
		dlg.m_RecieveLineNumber=RecieveLineNumber;
		dlg.m_ShotPointNumber=ShotPointNumber;
		dlg.m_ShotLineInterval=ShotLineInterval;
		dlg.m_GroupNumberOfSmallNumber=GroupNumberOfSmallNumber;
		dlg.m_GroupNumberOfBigNumber=GroupNumberOfBigNumber;
		dlg.m_GapOfSmallNumber=GapOfSmallNumber;
		dlg.m_GapOfBigNumber=GapOfBigNumber;
		dlg.m_FirstShotPointPosition=FirstShotPointPosition;		
	}

	if(dlg.DoModal()==IDCANCEL)return false;

	FILE* fp=fopen("svsys1.par","w");
	if(!fp)return false;

    fprintf(fp,"%s\n",dlg.m_Area);
    fprintf(fp,"%s\n",dlg.m_Crew);
    fprintf(fp,"%ld\n",dlg.m_GroupInterval);
    fprintf(fp,"%ld\n",dlg.m_RecieveLineNumber);
    fprintf(fp,"%ld\n",dlg.m_ShotPointNumber);
    fprintf(fp,"%ld\n",dlg.m_ShotLineInterval);
    fprintf(fp,"%ld\n",dlg.m_GroupNumberOfSmallNumber);
    fprintf(fp,"%ld\n",dlg.m_GroupNumberOfBigNumber);
    fprintf(fp,"%ld\n",dlg.m_GapOfSmallNumber);
    fprintf(fp,"%ld\n",dlg.m_GapOfBigNumber);
    fprintf(fp,"%ld\n",dlg.m_FirstShotPointPosition);
    
	fclose(fp);      
    
	return TRUE;
}       
//                        BOOL  SvSys::AskCommonParameter()                              END
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                        BOOL  SvSys::AskRLISPI()                                                   BEGIN
BOOL  SvSys::AskRLISPI()                  
{
	FILE* fp;
	CDRLISPI d;
	if(fp=fopen("svsys2.par","r")){
	   fscanf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",&d.m_RLI1,&d.m_RLI2,&d.m_RLI3,&d.m_RLI4,&d.m_RLI5,&d.m_RLI6,&d.m_RLI7,&d.m_RLI8,&d.m_RLI9,&d.m_RLI10);
           fscanf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",&d.m_RLI11,&d.m_RLI12,&d.m_RLI13,&d.m_RLI14,&d.m_RLI15,&d.m_RLI16,&d.m_RLI17,&d.m_RLI18,&d.m_RLI19,&d.m_RLI20);
           fscanf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",&d.m_RLI21,&d.m_RLI22,&d.m_RLI23,&d.m_RLI24,&d.m_RLI25,&d.m_RLI26,&d.m_RLI27,&d.m_RLI28,&d.m_RLI29,&d.m_RLI30);
           fscanf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",&d.m_RLI31,&d.m_RLI32,&d.m_RLI33,&d.m_RLI34,&d.m_RLI35,&d.m_RLI36,&d.m_RLI37,&d.m_RLI38,&d.m_RLI39,&d.m_RLI40);
           fscanf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",&d.m_RLI41,&d.m_RLI42,&d.m_RLI43,&d.m_RLI44,&d.m_RLI45,&d.m_RLI46,&d.m_RLI47,&d.m_RLI48,&d.m_RLI49,&d.m_RLI50);

           fscanf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",&d.m_SPI1,&d.m_SPI2,&d.m_SPI3,&d.m_SPI4,&d.m_SPI5,&d.m_SPI6,&d.m_SPI7,&d.m_SPI8,&d.m_SPI9,&d.m_SPI10);
           fscanf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",&d.m_SPI11,&d.m_SPI12,&d.m_SPI13,&d.m_SPI14,&d.m_SPI15,&d.m_SPI16,&d.m_SPI17,&d.m_SPI18,&d.m_SPI19,&d.m_SPI20);
           fscanf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",&d.m_SPI21,&d.m_SPI22,&d.m_SPI23,&d.m_SPI24,&d.m_SPI25,&d.m_SPI26,&d.m_SPI27,&d.m_SPI28,&d.m_SPI29,&d.m_SPI30);
           fscanf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",&d.m_SPI31,&d.m_SPI32,&d.m_SPI33,&d.m_SPI34,&d.m_SPI35,&d.m_SPI36,&d.m_SPI37,&d.m_SPI38,&d.m_SPI39,&d.m_SPI40);
           fscanf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",&d.m_SPI41,&d.m_SPI42,&d.m_SPI43,&d.m_SPI44,&d.m_SPI45,&d.m_SPI46,&d.m_SPI47,&d.m_SPI48,&d.m_SPI49,&d.m_SPI50);
           
		fclose(fp);
    }                                     
    if(d.DoModal()==IDCANCEL)
		return false;

	fp=fopen("svsys2.par","w");                       

	fprintf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",d.m_RLI1,d.m_RLI2,d.m_RLI3,d.m_RLI4,d.m_RLI5,d.m_RLI6,d.m_RLI7,d.m_RLI8,d.m_RLI9,d.m_RLI10);
	fprintf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",d.m_RLI11,d.m_RLI12,d.m_RLI13,d.m_RLI14,d.m_RLI15,d.m_RLI16,d.m_RLI17,d.m_RLI18,d.m_RLI19,d.m_RLI20);
	fprintf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",d.m_RLI21,d.m_RLI22,d.m_RLI23,d.m_RLI24,d.m_RLI25,d.m_RLI26,d.m_RLI27,d.m_RLI28,d.m_RLI29,d.m_RLI30);
	fprintf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",d.m_RLI31,d.m_RLI32,d.m_RLI33,d.m_RLI34,d.m_RLI35,d.m_RLI36,d.m_RLI37,d.m_RLI38,d.m_RLI39,d.m_RLI40);
	fprintf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",d.m_RLI41,d.m_RLI42,d.m_RLI43,d.m_RLI44,d.m_RLI45,d.m_RLI46,d.m_RLI47,d.m_RLI48,d.m_RLI49,d.m_RLI50);

	fprintf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",d.m_SPI1,d.m_SPI2,d.m_SPI3,d.m_SPI4,d.m_SPI5,d.m_SPI6,d.m_SPI7,d.m_SPI8,d.m_SPI9,d.m_SPI10);
	fprintf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",d.m_SPI11,d.m_SPI12,d.m_SPI13,d.m_SPI14,d.m_SPI15,d.m_SPI16,d.m_SPI17,d.m_SPI18,d.m_SPI19,d.m_SPI20);
	fprintf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",d.m_SPI21,d.m_SPI22,d.m_SPI23,d.m_SPI24,d.m_SPI25,d.m_SPI26,d.m_SPI27,d.m_SPI28,d.m_SPI29,d.m_SPI30);
	fprintf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",d.m_SPI31,d.m_SPI32,d.m_SPI33,d.m_SPI34,d.m_SPI35,d.m_SPI36,d.m_SPI37,d.m_SPI38,d.m_SPI39,d.m_SPI40);
	fprintf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",d.m_SPI41,d.m_SPI42,d.m_SPI43,d.m_SPI44,d.m_SPI45,d.m_SPI46,d.m_SPI47,d.m_SPI48,d.m_SPI49,d.m_SPI50);

	fclose(fp);
	return TRUE;
}                
//                        BOOL  SvSys::AskRLISPI()                                                   END
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//			  BOOL  SvSys::CalculateParameter()					BEGIN
BOOL  SvSys::CalculateParameters()
{   
	// 1. RecieveLinePosition
	RecieveLinePosition[0]=0;
	for(int i=1;i<RLIBoxNumber+1;i++){
		RecieveLinePosition[i]=RecieveLinePosition[i-1]+RecieveLineInterval[i-1];
	}

	// 2.  ShotPointPosition
	ShotPointPosition[0]=FirstShotPointPosition;
	for(i=1;i<SPIBoxNumber+1;i++){
			 ShotPointPosition[i]=ShotPointPosition[i-1]+ShotPointInterval[i-1];
	}   
	
	//
	WidthOfShotPoint=0;
	WidthOfRecievePoint=0;
	for(i=0;i<SPIBoxNumber;i++)
		WidthOfShotPoint+=ShotPointInterval[i];
	for(i=0;i<RLIBoxNumber;i++)
		WidthOfRecievePoint+=RecieveLineInterval[i];
	
	//	=====  Everything is OK.	======= //
	DataOK=TRUE;
	return TRUE;

} 
//			  BOOL  SvSys::CalculateParameter()	             				END
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////        
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////        

bool SvSys::Set()
{

    if(ReadCommonParameter()&&ReadRLISPI()){
        CalculateParameters();
		return true;
	}
	else{
		return false;
	}

}

void SvSys::Reset()
{
	Area="";
	Crew="";
	GroupInterval=0;
	RecieveLineNumber=0;
	ShotPointNumber=0;
	ShotLineInterval=0;
    GroupNumberOfSmallNumber=0;
    GroupNumberOfBigNumber=0;
    GapOfSmallNumber=0;
    GapOfBigNumber=0;
    FirstShotPointPosition=0;

    LeftPlusSquareLength=0;
    RightPlusSquareLength=0;     
    
	RLIBoxNumber=50;
	SPIBoxNumber=50;
	
	DataOK=FALSE;

}
