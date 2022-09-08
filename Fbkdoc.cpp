// FbkDoc.cpp : implementation file
//
#include "stdafx.h"
#include "afxcoll.h"
#include "struct.h"
#include "3dfstbrk.h"
#include "svsys.h"
#include "swathpar.h"
#include "equation.h"
#include "dswnum.h"
#include "dpgnum.h"
#include "Askgroup.h"
#include "math.h"
#include "ProgDlg.h"
#include "fh.h"
#include "fbk.h"
#include "FbkDoc.h"
#include "Relation.h"
#include "DlgRelation.h"
#include "ValueFbk.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFbkDoc

IMPLEMENT_DYNCREATE(CFbkDoc, CDocument)

CFbkDoc::CFbkDoc()
{
	m_pSelectedGroups=false;
	m_pGroupData=false;		
	m_pPointOfOneGroup=false;
	m_pDataInFstBrkFile=false;
	m_GroupNumber=0;

	m_DataRoomSize=0;

	// 与选择有关的变量.
	m_BeginSelectedGroup=0;
	m_EndSelectedGroup=0;

	//
	m_nGroupLineLimit=100; //=100
	m_nGroupLine=0;

	//
	m_nWindowLen=30;

	//
	m_nHeadNumber=128;

}

CFbkDoc::~CFbkDoc()
{

	if(m_pSelectedGroups)
		delete m_pSelectedGroups;
	if(m_pGroupData)
		delete m_pGroupData;		
	if(m_pPointOfOneGroup)
		delete m_pPointOfOneGroup;
	if(m_pDataInFstBrkFile)
		delete m_pDataInFstBrkFile;
}


BEGIN_MESSAGE_MAP(CFbkDoc, CDocument)
	//{{AFX_MSG_MAP(CFbkDoc)
	ON_COMMAND(ID_FBCalculate, OnFBCalc)
	ON_COMMAND(ID_FBEndPage, OnFBEndPage)
	ON_COMMAND(ID_FBFirstPage, OnFBFirstPage)
	ON_COMMAND(ID_FBLastPage, OnFBLastPage)
	ON_COMMAND(ID_FBLearn, OnFBLearn)
	ON_COMMAND(ID_FBNextPage, OnFBNextPage)
	ON_COMMAND(ID_FBSearchPage, OnFBSearchPage)
	ON_COMMAND(ID_FILE_MAKEINITIATEFILE_FROM52CHARACTARTEXTFILE, OnFileMakeFromText)
	ON_COMMAND(ID_FILE_MAKEINITIATEFILE_FROMDBFFILE, OnFileMakeFromDBF)
	ON_UPDATE_COMMAND_UI(ID_FBCalculate, OnUpdateFBCalc)
	ON_UPDATE_COMMAND_UI(ID_FBClose, OnUpdateFBClose)
	ON_UPDATE_COMMAND_UI(ID_FBEndPage, OnUpdateFBEndPage)
	ON_UPDATE_COMMAND_UI(ID_FBFirstPage, OnUpdateFBFirstPage)
	ON_UPDATE_COMMAND_UI(ID_FBLastPage, OnUpdateFBLastPage)
	ON_UPDATE_COMMAND_UI(ID_FBLearn, OnUpdateFBLearn)
	ON_UPDATE_COMMAND_UI(ID_FBNextPage, OnUpdateFBNextPage)
	ON_UPDATE_COMMAND_UI(ID_FBSearchPage, OnUpdateFBSearchPage)
	ON_COMMAND(ID_FB_RelativeCalc, OnFBRelativeCalc)
	ON_COMMAND(ID_MakeGKHFBD, MakeGKHFBD)
	ON_COMMAND(ID_FB_PreciseRead, OnFBPreciseRead)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFbkDoc diagnostics

#ifdef _DEBUG
void CFbkDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFbkDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFbkDoc serialization
void CFbkDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFbkDoc commands
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//			void CFbkDoc::Set(int Swath)					BEGIN
BOOL CFbkDoc::Set(CString sFile)
 {
	if(!UnformFile::Set(sFile)){
		return false;
	}

	// 设置初至文件对象.
	UINT n=sFile.Find(".");
	CString FileFbk;
	if(n==-1)
		FileFbk=sFile+".fbk";
	else
		FileFbk=sFile.Left(n)+".fbk";
	m_fbk.Set(FileFbk);
	
	//Page to be Drawn.
	m_CurrShot=0;
	m_GroupNumber=m_pShotMsg[m_CurrShot].GroupNumber;
	m_DataRoomSize=m_DataNumberOfOneGroup*m_MaxGroupNumber;// Needed

	if(m_pSelectedGroups)delete m_pSelectedGroups;
	if(m_pGroupData)delete m_pGroupData;
	if(m_pPointOfOneGroup)delete m_pPointOfOneGroup;
	if(m_pDataInFstBrkFile)delete m_pDataInFstBrkFile;

	m_pSelectedGroups=new BOOL[m_MaxGroupNumber];
	m_pGroupData=new float[m_DataRoomSize];// Needed.
	m_pPointOfOneGroup=new CPoint[m_PointNumberOfOneGroup];
	m_pDataInFstBrkFile=new DataInFstBrkFile[m_MaxGroupNumber];
		
	m_InflatingCoefficient=1.5;
	
	// 读取一炮的数据。
	ReadData(); 

	return TRUE;	 
}
//				CFbkDoc::Set(CString PathName)				    END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//			Subroutine Name:   MakeFBD
//          Destination :   Make a  group of Simulated First Break Data 
//                                  used to Simulated-calculation.
//          Input Parameter: Swath , the swath number.
//          output File  : "swath001.fbd".
void CFbkDoc::MakeFBD(int Swath)
{
	
	DataInFstBrkFile *data=new DataInFstBrkFile[31];
	SvSys ss;	
	if(!ss.Set()){
		AfxMessageBox("Survey system not found!");
		return;
	}

	SwathParameter sp;
	sp.Set(0);
	double a,b=RAND_MAX; 
	double noise;
	double x,y,L;
	double HV=1500.0;
	int n;

	long ReceivePointNumber=600;

	float *SV=new float[60];
	float *RV=new float[ReceivePointNumber];

	FILE *fp=fopen("swath000.st","r");
	if(!fp){
		AfxMessageBox("炮点静校正量文件未找到!");
		return;
	}

	for(int i=0;i<60;i++){
		fscanf(fp,"%d %lf %lf %f\n",&n,&x,&y,&SV[i]);
	}
	fclose(fp);
	
	fp=fopen("swath000.rt","r");
	for(i=0;i<ReceivePointNumber;i++){
		n=fscanf(fp,"%d %lf %lf %f\n",&n,&x,&y,&RV[i]);
		if(n<4){
			AfxMessageBox("File swath000.rt is shorter than expected!");
			break;
		}
	}
	fclose(fp);

	FILE *fpText=fopen("swath000.txt","w");
	
	CString FileName="swath"+IntToName(Swath)+".fbd";
	fp=fopen(FileName,"wt");

	
	FILE *fpRcvPos=fopen("RcvPos.txt","wt");
	for( i=0;i<4;i++){
		for(int j=0;j<31+2*10;j++){
			fprintf(fpRcvPos,"%i %i %i\n",(101+i)*1000+j+1,ss.RecieveLinePosition [i],j*ss.GroupInterval );
		}
	}
	fclose(fpRcvPos);

	FILE *fpShotPos=fopen("ShotPos.txt","wt");
	long RP;
	float rv;
	for(int ShotLine=0;ShotLine<10;ShotLine++){
		long RPlused=ShotLine*(ss.ShotLineInterval/ss.GroupInterval)*ss.RecieveLineNumber;
				
		for(int ShotPoint=0;ShotPoint<6;ShotPoint++){
			fprintf(fpShotPos,"%i %i %i\n",(501+ShotLine)*1000+ShotPoint+1,ss.ShotPointPosition[ShotPoint],-sp.Distance2+ShotLine *ss.ShotLineInterval );
			
			
			long ShotPointNumber=ShotLine*ss.ShotPointNumber+ShotPoint;
			float sv=SV[ShotPointNumber];
			for(int RecieveLine=0;RecieveLine<4;RecieveLine++){
				x=ss.RecieveLinePosition[RecieveLine]-ss.ShotPointPosition[ShotPoint];
				for(int RecievePoint=30;RecievePoint>=0;RecievePoint--){
					//RP=RPlused+(30-RecievePoint)*ss.RecieveLineNumber+RecieveLine;
					RP=RPlused+(30-RecievePoint)*ss.RecieveLineNumber+RecieveLine;

					rv=RV[RP];
					//TRACE("Recieve value: %i %f\n",RP,rv);
					data[30-RecievePoint].FileNumber=ShotLine*ss.ShotPointNumber+ShotPoint+1;
					long ShotPh=(501+ShotLine)*1000+ShotPoint+1;
					long RecievePh=(101+RecieveLine)*1000+(30-RecievePoint)+1+(ShotLine*ss.ShotLineInterval/ss.GroupInterval  );
						
					y=sp.Distance2+ss.GroupInterval*RecievePoint;					
					L=sqrt(x*x+y*y);
					a=rand();
					//noise=a/b*5.0;
					noise=0.0;
					data[30-RecievePoint].FirstBreak=noise+L/HV*1000+(-sv)+(-rv);  //*1000 :make ms from s.
					float FirstBreak=noise+L/HV*1000+(-sv)+(-rv);  //*1000 :make ms from s.
					fprintf(fp,"%ld %ld %5.1f\n",ShotPh,RecievePh,FirstBreak);
				}
				//fwrite(data,sizeof(DataInFstBrkFile),31,fp);
				
				for(int i=0;i<31;i++){
					fprintf(fpText,"%i,%i,%i,%10.4f\n",ShotLine,ShotPoint,RecieveLine,data[i].FirstBreak);
				}
	    		}
		}
	}
	fclose(fpShotPos);
	fclose(fp);
	delete data;

	AfxMessageBox("Immulative First Break Made Over!");	

}
//				MakeFBD																							  END
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void CFbkDoc::OnFBCalc() 
{
	SaveFstBrk();
	NeuNetCalculate();
	if(ReadData())
		UpdateAllViews(NULL);
}

void CFbkDoc::OnFBEndPage() 
{
	SaveFstBrk();
	m_CurrShot=m_TotalShotNumber-1;
	if(ReadData())UpdateAllViews(NULL);
}


void CFbkDoc::OnFBFirstPage() 
{
	SaveFstBrk();
	m_CurrShot=0;
	if(ReadData())UpdateAllViews(NULL);
}

void CFbkDoc::OnFBLastPage() 
{
	// TODO: Add your command handler code here
	SaveFstBrk();
	m_CurrShot--;
	if(ReadData())UpdateAllViews(NULL);	
}

void CFbkDoc::OnFBLearn() 
{
	NeuNetLearn();
}

void CFbkDoc::OnFBNextPage() 
{
	// TODO: Add your command handler code here
	SaveFstBrk();
	m_CurrShot++;
	if(ReadData())UpdateAllViews(NULL);
}


void CFbkDoc::OnFBSearchPage() 
{
	SaveFstBrk();
	if(AskSearchedPageNumber()){
		ReadData();
		UpdateAllViews(NULL);
	}
}


void CFbkDoc::OnFileMakeFromText() 
{
	// TODO: Add your command handler code here
	
}

void CFbkDoc::OnFileMakeFromDBF() 
{
	// TODO: Add your command handler code here
	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                   BOOL CFbkDoc::CalculateFirstBreak()					BEGIN
BOOL CFbkDoc::NeuNetCalculate()
{
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  设置要计算的起止道数.
	long BeginShot,EndShot;
	if(!AskCalculatedGroup(&BeginShot,&EndShot))return FALSE;
	
  	
	DataInFstBrkFile D;
	float *DataOfOneGroup=new float[m_DataNumberOfOneGroup*2];

	CProgressDlg dlg;
	dlg.Create();
	dlg.SetWindowText("正在求取初至");
	long i,j;
	for(i=BeginShot;i<=EndShot;i++){
	
		dlg.SetStatus("神经网络正在计算第 "+STR(i+1)+" 炮的初至!");
		for(j=m_pShotMsg[i].BeginGroup;j<=m_pShotMsg[i].EndGroup;j++){
			
			if(j%10==0){
				dlg.SetPos((j-m_pShotMsg[i].BeginGroup)
					/(float)m_pShotMsg[i].GroupNumber*100.0);
				if(dlg.CheckCancelButton())
				if(AfxMessageBox("你确实想停止求取初至吗?",MB_YESNO)==IDYES)
					goto EndCalc;
			}		

			GetGroup(DataOfOneGroup,j,2);  

			// Calculating first break.
			m_neunet.Set(&DataOfOneGroup[m_UnformGroupHead],m_PointNumberOfOneGroup);
			D.FirstBreak=(m_neunet.Think()+1)*m_UnformTimeInterval;  // n..Think , returned 0,1,2,...
			D.FileNumber=GetFileNumber(j);

			// Write File Number and First Break into first break file.
			m_fbk.Put(&D,j,1);
		}
	}

EndCalc:
	dlg.DestroyWindow();
		
	delete DataOfOneGroup;

	AfxMessageBox("初至计算完毕!");
	return TRUE;
}
//                   BOOL CFbkDoc::CalculateFirstBreak()					 END
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        UNIT CFbkDoc::AskSearchedPageNumber()          BEGIN
BOOL CFbkDoc::AskSearchedPageNumber()
{
	CDpgNum dlg;
	dlg.m_PageNumber=m_pShotMsg[m_CurrShot].FileNumber;
	if(dlg.DoModal()==IDOK){
		for(long i=0;i<m_TotalShotNumber;i++){
			if(m_pShotMsg[i].FileNumber==dlg.m_PageNumber){
				m_CurrShot=i;
				return TRUE;
			}
		}
	}
	return FALSE;
}
//        UNIT CFbkDoc::AskSearchedPageNumber()          END
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//             BOOL CFbkDoc::SaveCFbkDoc()						BEGIN
BOOL CFbkDoc::SaveFstBrk()
{
	if(m_GroupNumber>0)
		m_fbk.Put(m_pDataInFstBrkFile,
		m_pShotMsg[m_CurrShot].BeginGroup,
		m_GroupNumber);

	return TRUE;
}
//             BOOL CFbkDoc::SaveCFbkDoc()								END
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	名称:		AskCalculatedFileNumber											开始
//	功能:	询问用户起止计算的文件号.
//  改变:	*BeginCalculatedFileNumber,开始计算的文件号
//				*EndCalculatedFileNumber,终止计算的文件号.
// 日期:	1997.10.4
BOOL CFbkDoc::AskCalculatedGroup(long  *Bn, long  *En)
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	// 显示对话框.
	CAskGroup dlg;
	dlg.m_BN=m_pShotMsg[0].FileNumber ;
	dlg.m_EN=m_pShotMsg[m_TotalShotNumber-1].FileNumber ;
	if(dlg.DoModal()==IDCANCEL)return FALSE;
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// 赋值.
	*Bn=dlg.m_BN;
	*En=dlg.m_EN;
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// 控制.
	if(*Bn<m_pShotMsg[0].FileNumber)
		*Bn=m_pShotMsg[0].FileNumber;
	if(*En>m_pShotMsg[m_TotalShotNumber-1].FileNumber)
		*En=m_pShotMsg[m_TotalShotNumber-1].FileNumber;



	BOOL B=FALSE,E=FALSE;
	for(long i=0;i<m_TotalShotNumber;i++){
		if(*Bn==m_pShotMsg[i].FileNumber){
			*Bn=i;
			B=TRUE;
			break;
		}
	}

	for(i=0;i<m_TotalShotNumber;i++){
		if(*En==m_pShotMsg[i].FileNumber){
			*En=i;
			E=TRUE;
			break;
		}
	}

	if(!B)*Bn=0;
	if(!E)*En=m_TotalShotNumber-1;

	return TRUE;
}
//			BOOL CFbkDoc::AskCalculateShot()							  END
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  名称:   AdjustGroupData(float coef)															开始
//  功能:   对数据点值进行控制,使之适应屏幕的逻辑坐标.
//  输入:coef,放大系数.
//  日期:1997.10.5
void CFbkDoc::AdjustGroupData(float coef)
{
	// 对数据点值进行控制.不必每次绘制都控制了.
	long i,j,m;
	float Max,a;
	float Devider=50/1.5;
	if(coef==0){
		for(i=0;i<m_pShotMsg[m_CurrShot].GroupNumber;i++){
			m=i*m_DataNumberOfOneGroup+m_UnformGroupHead;
			Max=0;
			for(j=0;j<m_PointNumberOfOneGroup;j++){
				a=fabs(m_pGroupData[m+j]);
				if(a>Max)Max=a;
			}
			if(Max==0)continue;  // In case  of all of tha data in the group are zero.
			
			Max/=Devider;
			for(j=0;j<m_PointNumberOfOneGroup;j++){
				m_pGroupData[m+j]/=Max;
			}	
		}
	}
	else{
		 for(i=0;i<m_pShotMsg[m_CurrShot].GroupNumber;i++){
			m=i*m_DataNumberOfOneGroup+m_UnformGroupHead;
			for(j=0;j<m_PointNumberOfOneGroup;j++){
				m_pGroupData[m+j]*=coef;
			}	
		}
	}

}
//  名称:   AdjustGroupData(float coef)															  结束
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  名称:	NeuNetLearn																				   开始
//  功能:   对选择道逐道学习.
//  返回:   所有道都成功学习TRUE.
BOOL CFbkDoc::NeuNetLearn()
{
	UINT AllLearned;	  
	long Point,n,LearnTime=0;
	
	// 对该页上的所有道循环, 若有选取道, 则学习之.
	bool bSelected=false;
	for(int i=0;i<m_GroupNumber;i++){
		if(m_pSelectedGroups[i]){
			bSelected=true;
			break;
		}
	}

	bool bFirstBreak=false;
	if(bSelected){
		if(AfxMessageBox("你所选取的时间值是初至吗?",MB_YESNO)==IDYES)
			bFirstBreak=true;
	}

	AllLearned=0;
	for(i=0;i<m_GroupNumber;i++){
		if(m_pSelectedGroups[i]){
			AllLearned=2;
			Point=m_pDataInFstBrkFile[i].FirstBreak/m_UnformTimeInterval;
			n=i*m_DataNumberOfOneGroup+m_UnformGroupHead;
			m_neunet.Set(&m_pGroupData[n],m_PointNumberOfOneGroup,Point,bFirstBreak);
			if(!m_neunet.Learn())AllLearned=1;
		}
	}

	switch(AllLearned){
	case 0:
		AfxMessageBox("你并没有选取任何道!");
		return FALSE;
	case 1:
		AfxMessageBox("并非所有道都能被神经网络学习!");
		return FALSE;
	default:
		AfxMessageBox("很好,学习完毕!");
		return TRUE;
	}

}
//  名称:	NeuNetLearn																				   结束
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   
BOOL CFbkDoc::ReadData()
{	
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  检查页号的合法性.
	if(m_CurrShot<0){
		m_CurrShot=0;
		return FALSE;
	}
	if(m_CurrShot>=m_TotalShotNumber){
		m_CurrShot=m_TotalShotNumber-1;
		return FALSE;
	}
	
	//1.读取波形数据.
	GetShot(m_pGroupData,m_CurrShot);
	AdjustGroupData();

	// 2.读取初至数据
	m_fbk.Get(m_pDataInFstBrkFile,m_pShotMsg[m_CurrShot].BeginGroup,m_GroupNumber);
	
	// 3.因为数据变化了, 所以重置SelectedGroups,和m_GroupInverted.
	for(long i=0;i<m_GroupNumber;i++){
		m_pSelectedGroups[i]=FALSE;
	}

	SetGroupLine();

	m_BeginSelectedGroup=-2;
	m_EndSelectedGroup=-1;
	
	SetModifiedFlag(true);
	return TRUE;
}
//  ReadData.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CFbkDoc::OnUpdateFBCalc(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bEnable=false;
	if(m_GroupNumber>0)bEnable=true;
	pCmdUI->Enable(bEnable);
}

void CFbkDoc::OnUpdateFBClose(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bEnable=false;
	if(m_GroupNumber>0)bEnable=true;
	pCmdUI->Enable(bEnable);
}

void CFbkDoc::OnUpdateFBEndPage(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bEnable=false;
	if(m_GroupNumber>0)bEnable=true;
	pCmdUI->Enable(bEnable);
}

void CFbkDoc::OnUpdateFBFirstPage(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bEnable=false;
	if(m_GroupNumber>0)bEnable=true;
	pCmdUI->Enable(bEnable);
}

void CFbkDoc::OnUpdateFBLastPage(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bEnable=false;
	if(m_GroupNumber>0)bEnable=true;
	pCmdUI->Enable(bEnable);
}

void CFbkDoc::OnUpdateFBLearn(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bEnable=false;
	if(m_GroupNumber>0)bEnable=true;
	pCmdUI->Enable(bEnable);
}

void CFbkDoc::OnUpdateFBNextPage(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bEnable=false;
	if(m_GroupNumber>0)bEnable=true;
	pCmdUI->Enable(bEnable);
}

void CFbkDoc::OnUpdateFBSearchPage(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	bool bEnable=false;
	if(m_GroupNumber>0)bEnable=true;
	pCmdUI->Enable(bEnable);
}


BOOL CFbkDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class	
	return SaveFstBrk();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//			Subroutine Name:   MakeFBD
//          Destination :   Make a  group of Simulated First Break Data 
//                                  used to Simulated-calculation.
//          Input Parameter: Swath , the swath number.
//          output File  : "swath001.fbd".
void CFbkDoc::MakeGKHFBD()
{
	
	DataInFstBrkFile *data=new DataInFstBrkFile[31];
	SvSys ss;	
	if(!ss.DataOK){
		AfxMessageBox("Survey system not found!");
		return;
	}

	SwathParameter sp;
	sp.Set(0);
	double a,b=RAND_MAX; 
	double noise;
	double x,y,L;
	double HV=1500.0;
	int n;

	long ReceivePointNumber=600;

	float *SV=new float[60];
	float *RV=new float[ReceivePointNumber];

	FILE *fp=fopen("swath000.st","r");
	if(!fp){
		AfxMessageBox("炮点静校正量文件未找到!");
		return;
	}

	for(int i=0;i<60;i++){
		fscanf(fp,"%d %lf %lf %f\n",&n,&x,&y,&SV[i]);
	}
	fclose(fp);
	
	fp=fopen("swath000.rt","r");
	for(i=0;i<ReceivePointNumber;i++){
		n=fscanf(fp,"%d %lf %lf %f\n",&n,&x,&y,&RV[i]);
		if(n<4){
			AfxMessageBox("File swath000.rt is shorter than expected!");
			break;
		}
	}
	fclose(fp);

	FILE *fpText=fopen("swath000.txt","w");
	
	CString FileName="swath000.fbd";
	fp=fopen(FileName,"wt");

	
	FILE *fpRcvPos=fopen("RcvPos.txt","wt");
	for( i=0;i<4;i++){
		for(int j=0;j<31+2*10;j++){
			fprintf(fpRcvPos,"%i %i %i\n",(101+i)*1000+j+1,ss.RecieveLinePosition [i],j*ss.GroupInterval );
		}
	}
	fclose(fpRcvPos);

	FILE *fpShotPos=fopen("ShotPos.txt","wt");
	long RP;
	float rv;
	for(int ShotLine=0;ShotLine<10;ShotLine++){
		long RPlused=ShotLine*(ss.ShotLineInterval/ss.GroupInterval)*ss.RecieveLineNumber;
				
		for(int ShotPoint=0;ShotPoint<6;ShotPoint++){
			fprintf(fpShotPos,"%i %i %i\n",(501+ShotLine)*1000+ShotPoint+1,ss.ShotPointPosition[ShotPoint],-sp.Distance2+ShotLine *ss.ShotLineInterval );			
			
			long ShotPointNumber=ShotLine*ss.ShotPointNumber+ShotPoint;
			float sv=SV[ShotPointNumber];
			for(int RecieveLine=0;RecieveLine<4;RecieveLine++){
				x=ss.RecieveLinePosition[RecieveLine]-ss.ShotPointPosition[ShotPoint];
				for(int RecievePoint=30;RecievePoint>=0;RecievePoint--){
					RP=RPlused+(30-RecievePoint)*ss.RecieveLineNumber+RecieveLine;
					rv=RV[RP];
					data[30-RecievePoint].FileNumber=ShotLine*ss.ShotPointNumber+ShotPoint+1;
					long ShotPh=(501+ShotLine)*1000+ShotPoint+1;
					long RecievePh=(101+RecieveLine)*1000+(30-RecievePoint)+1+(ShotLine*ss.ShotLineInterval/ss.GroupInterval  );
						
					y=sp.Distance2+ss.GroupInterval*(30-RecievePoint);
					L=sqrt(x*x+y*y);
					a=rand();

					noise=0.0;
					data[30-RecievePoint].FirstBreak=noise+L/HV*1000+(-sv)+(-rv);  //*1000 :make ms from s.
					float FirstBreak=noise+L/HV*1000+(-sv)+(-rv);  //*1000 :make ms from s.
					fprintf(fp,"%ld %ld %5.1f\n",ShotPh,RecievePh,FirstBreak);
				}
				
				for(int i=0;i<31;i++){
					fprintf(fpText,"%i,%i,%i,%10.4f\n",ShotLine,ShotPoint,RecieveLine,data[i].FirstBreak);
				}
	    		}
		}
	}
	fclose(fpShotPos);
	fclose(fp);
	delete data;

	AfxMessageBox("Immulative First Break Made Over!");	

}
//				MakeFBD																							  END
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CFbkDoc::SetGroupLine ()
{
	int n=0;	
	long nGroupName=GetGroupLineName(0),nGroupNameLast;
	m_GroupLine[0].nSequence=0;
	m_GroupLine[0].nStartGroup=0;
	m_GroupLine[0].nStartGroupName=nGroupName;	

	for(int i=1;i<m_pShotMsg[m_CurrShot].GroupNumber;i++){
		nGroupNameLast=nGroupName;
		nGroupName=GetGroupLineName(i);
		if(nGroupName!=(nGroupNameLast+1)){
			n++;
			if(n>=m_nGroupLineLimit)break;
			m_GroupLine[n-1].nEndGroup=i-1;
			m_GroupLine[n-1].nEndGroupName=nGroupNameLast;
			m_GroupLine[n].nStartGroup=i;
			m_GroupLine[n].nStartGroupName=nGroupName;
		}
	}
	m_GroupLine[n].nEndGroup=m_pShotMsg[m_CurrShot].GroupNumber-1;
	m_GroupLine[n].nEndGroupName=nGroupName;

	m_nGroupLine=n+1;

	return true;
}

long CFbkDoc::GetGroupLineName(int nGroup)
{
	if(nGroup<0||nGroup>=m_GroupNumber){
		return -1;
	}

	long pos=nGroup*m_DataNumberOfOneGroup+m_nGroupNamePosition;
	long n=*(long*)&(m_pGroupData[pos]);
	return n;


}

BOOL CFbkDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	return Set(lpszPathName);
}

void CFbkDoc::OnFBRelativeCalc() 
{
	// TODO: Add your command handler code here
	CDlgRelation dlg;
	dlg.m_nModalGroup =0;
	dlg.m_nTimeRange =400;
	dlg.m_nShotFrom =m_pShotMsg[m_CurrShot].FileNumber;
	dlg.m_nShotTo=m_pShotMsg[m_CurrShot].FileNumber;
	dlg.m_nTotalShotFrom =m_TopFileNumber;
	dlg.m_nTotalShotTo=m_EndFileNumber;

	do{	
		if(dlg.DoModal()==IDCANCEL)return;
	}while(dlg.m_nModalGroup <0||dlg.m_nModalGroup >=m_GroupNumber);

	
	long nStartTime=m_pDataInFstBrkFile[dlg.m_nModalGroup].FirstBreak-dlg.m_nTimeRange/2;
	int nModalGroupPoint=dlg.m_nTimeRange/m_UnformTimeInterval;
	long nStartPoint=nStartTime/m_UnformTimeInterval+m_UnformGroupHead;
	
	m_relation.SetModelGroup(
		&m_pGroupData[dlg.m_nModalGroup*m_DataNumberOfOneGroup+nStartPoint],
		nModalGroupPoint);
	m_relation.SetGroupPointNumber(m_PointNumberOfOneGroup);

	long nShotBak=m_CurrShot,nShot;
	long i,j,nFbk;
	for(i=dlg.m_nShotFrom ;i<=dlg.m_nShotTo;i++){

		// Search which shot is the file number:
		nShot==-1;
		for(j=0;j<m_TotalShotNumber;j++){
			if(m_pShotMsg[j].FileNumber==i){
				nShot=j;
				break;
			}
		}
		if(nShot==-1)continue;			

		m_CurrShot=nShot;
		ReadData();
		for(j=0;j<m_GroupNumber;j++){
			m_relation.SetGroup(&m_pGroupData[j*m_DataNumberOfOneGroup]+m_UnformGroupHead);
			nFbk=m_relation.Calculate();
			m_pDataInFstBrkFile[j].FirstBreak=nFbk*m_UnformTimeInterval;
		}
		if(!OnSaveDocument(GetPathName())){
			AfxMessageBox("Error when saving first break!");
			break;
		}
	}

	m_CurrShot=nShotBak;
	ReadData();
	return;
	
}

bool CFbkDoc::CalcFbkInteractively(CMyPoint pointHead, CMyPoint pointTail)
{
	if(fabs(pointTail.x-pointHead.x)<0.1){
		m_pDataInFstBrkFile[(long)pointHead.x].FirstBreak=pointHead.y;		
	}
	else{

		if(pointTail.x<pointHead.x){
			double temp;
			temp=pointTail.x;
			pointTail.x=pointHead.x;
			pointHead.x=temp;

			temp=pointTail.y;
			pointTail.y=pointHead.y;
			pointHead.y=temp;
		}

		pointHead.y/=m_UnformTimeInterval ;
		pointTail.y/=m_UnformTimeInterval ;

		int nCalcLen=m_nWindowLen;
		int nHalfCalcLen=nCalcLen/2;
		
		CValueFbk valueFbk;
		valueFbk.SetPointNumber (nCalcLen);
		double k=(pointTail.y-pointHead.y)/(pointTail.x-pointHead.x);
		long nMidPoint,nTopPoint,nFbk;

		for(long i=pointHead.x;i<=pointTail.x;i++){
			nMidPoint=pointHead.y+(i-pointHead.x)*k;
			nTopPoint=nMidPoint-nHalfCalcLen;

			nFbk=valueFbk.CalcFbk (&m_pGroupData[i*m_DataNumberOfOneGroup+m_UnformGroupHead+nTopPoint] );
			if(nFbk==-1)
				nFbk=nMidPoint;
			else
				nFbk+=nTopPoint;

			m_pDataInFstBrkFile[i].FirstBreak=nFbk*m_UnformTimeInterval;
		}
	}
	
	if(!OnSaveDocument(GetPathName())){
		AfxMessageBox("Error when saving first break!");
		return false;
	}
	return true;

}

void CFbkDoc::OnFBPreciseRead() 
{
	// TODO: Add your command handler code here
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  设置要计算的起止道数.
	long BeginShot,EndShot;
	if(!AskCalculatedGroup(&BeginShot,&EndShot))return ;
	

	CProgressDlg dlg;
	dlg.Create();
	dlg.SetRange (BeginShot,EndShot+1);
	dlg.SetWindowText("正在精细求取初至");
	long i,j;

	//
	int nCalcLen=m_nWindowLen;
	if(nCalcLen>m_PointNumberOfOneGroup){
		nCalcLen=m_PointNumberOfOneGroup;
	}

	int nHalfCalcLen=nCalcLen/2;	
	CValueFbk valueFbk;
	long nMidPoint,nTopPoint,nEndPoint,nFbk,nLen;
	bool bSuccess=true;

	//
	for(i=BeginShot;i<=EndShot;i++){
		
		// Set the progress bar:
		dlg.SetStatus("正在计算第 "+STR(i+1)+" 炮的初至!");
		dlg.SetPos(i);
		if(dlg.CheckCancelButton()){
			if(AfxMessageBox("你确实想停止求取初至吗?",MB_YESNO)==IDYES)
				break;
		}		

		
		// Check which shot is the file number:
		m_CurrShot=i;
	
		// Get the shot and the first break:
		if(!ReadData())continue;

		for(j=0;j<m_pShotMsg[i].GroupNumber;j++){
			nMidPoint=m_pDataInFstBrkFile[j].FirstBreak/m_UnformTimeInterval;
			nTopPoint=nMidPoint-nHalfCalcLen;
			nEndPoint=nTopPoint+nCalcLen;

			if(nTopPoint<0){
				nTopPoint=0;
				nEndPoint=nCalcLen;
			}
			if(nEndPoint>=m_PointNumberOfOneGroup){
				nEndPoint=m_PointNumberOfOneGroup-1;
				nTopPoint=nEndPoint-nCalcLen;
			}

			nLen=nEndPoint-nTopPoint;
			valueFbk.SetPointNumber (nLen);		



			nFbk=valueFbk.CalcFbk (&m_pGroupData[j*m_DataNumberOfOneGroup+m_UnformGroupHead+nTopPoint ]);
			if(nFbk==-1)
				nFbk=nMidPoint;
			else
				nFbk+=nTopPoint;

			m_pDataInFstBrkFile[j].FirstBreak=nFbk*m_UnformTimeInterval;
		}

		if(!OnSaveDocument(GetPathName())){
			AfxMessageBox("Error when saving first break!");
			bSuccess=false;
			break;
		}

	}
	
	dlg.DestroyWindow();
		
	if(bSuccess)
		AfxMessageBox("初至计算完毕!");

	UpdateAllViews(NULL);

	
	return ;
	
}

long CFbkDoc::GetGroupHead(long nGroup, int nHead)
{
	if(nGroup<0)nGroup=0;
	if(nGroup>=m_GroupNumber)nGroup=m_GroupNumber-1;

	if(nHead<1)nHead=1;
	if(nHead>m_nHeadNumber)nHead=m_nHeadNumber;

	long nItem=*(long*)(&m_pGroupData[m_DataNumberOfOneGroup*nGroup+nHead-1]);

	return nItem;
}
