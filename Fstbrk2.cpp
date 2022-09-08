///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Problems should be  resolved later:
//	1.Should let the user can choose more groups than there are in one shot.
//          but Doing that there is a problem now,I can not resolve it.
//			 现在此问题已经解决.
//	2.Should add a message bar, used to show help message when the user 
//          press left button  on one of the icons.
#include "stdafx.h"
#include "3dfstbrk.h"
#include "Unform.h"
#include "FbkDoc.h"
#include "dpgnum.h"
#include "AskGroup.h"
#include "dastn.h"
#include "dshwtme.h"
#include "progdlg.h"
#include "math.h"
#include "fh.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                   BOOL CFbkDoc::CalculateFirstBreak()					BEGIN
BOOL CFbkDoc::NeuNetCalculate()
{
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  设置要计算的起止道数.
	long BeginCalcGroup=1,EndCalcGroup=m_UnformGroupNumber;
	if(!AskCalculatedGroup(&BeginCalcGroup,&EndCalcGroup))return FALSE;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 打开初至文件. 并利用NeuNetThink计算初至.
	FILE *fpFstBrkFile=fopen(m_FileFirstBreak,"rb+");
  	
	DataInFstBrkFile D;
	float *DataOfOneGroup=new float[m_DataNumberOfOneGroup*2];

	CProgressDlg dlg;
	dlg.Create();
	dlg.SetWindowText("正在求取初至");
	float n=EndCalcGroup-BeginCalcGroup+1;
	for(int i=BeginCalcGroup;i<=EndCalcGroup;i++){
		if(i%10==0){
			dlg.SetStatus("神经网络已经计算了 "+STR(i)+" 道初至!");
			dlg.SetPos((i-BeginCalcGroup)/n*100);
			if(dlg.CheckCancelButton())
			if(AfxMessageBox(_T("你确实想停止求取初至吗?"),MB_YESNO)==IDYES)
				break;
		}
																						 		
		GetGroup(DataOfOneGroup,i-1,2);  //

		// Calculating first break.
		m_neunet.Set(&DataOfOneGroup[m_UnformGroupHead],m_PointNumberOfOneGroup);
		D.FirstBreak=(m_neunet.Think()+1)*m_UnformTimeInterval;  // n..Think , returned 0,1,2,...
		D.FileNumber=GetFileNumber(i);

		// Write File Number and First Break into first break file.
		fseek(fpFstBrkFile,(i-1)*sizeof(DataInFstBrkFile),SEEK_SET); 	
		fwrite(&D,sizeof(DataInFstBrkFile),1,fpFstBrkFile);
	}
	dlg.DestroyWindow();
		
	// close the two file.
	fclose(fpFstBrkFile);

	delete DataOfOneGroup;

	AfxMessageBox("初至计算完毕!");
	return TRUE;
}
//                   BOOL CFbkDoc::CalculateFirstBreak()					 END
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//               BOOL CFbkDoc::AskShowTrace()							BEGIN
BOOL CFbkDoc::AskShowTrace()
{
	CDASTN dlg;
	dlg.m_ShowTraceNumber=m_GrpNrOnOnePage;
	if(dlg.DoModal()==IDOK){
		if(m_GrpNrOnOnePage>m_MaxGrpNrOnOnePage)
			m_GrpNrOnOnePage=m_MaxGrpNrOnOnePage;

		//修改:
		//1.每页显示的道数
		//2.当前页.
		long n=(m_CurrentPageNumber-1)*m_GrpNrOnOnePage+1;
		m_GrpNrOnOnePage=dlg.m_ShowTraceNumber;
		m_CurrentPageNumber=(n-1)/m_GrpNrOnOnePage+1;
		
		// return.
		return TRUE;
	}
	return FALSE;
}
//               BOOL CFbkDoc::AskShowTrace()								   END
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        UNIT CFbkDoc::AskSearchedPageNumber()          BEGIN
BOOL CFbkDoc::AskSearchedPageNumber()
{
	CDpgNum dlg;
	dlg.m_PageNumber=m_CurrentPageNumber;
	if(dlg.DoModal()==IDOK){
		m_CurrentPageNumber=dlg.m_PageNumber;
		return TRUE;
	}
	return FALSE;
}
//        UNIT CFbkDoc::AskSearchedPageNumber()          END
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//			BOOL CFbkDoc::AskShowTime()								BEGIN	
BOOL CFbkDoc::AskShowTime()
{
	CDShwTme dlg;
	dlg.m_BeginShowTime=m_BeginShowTime;
	dlg.m_EndShowTime=m_EndShowTime;
	if(dlg.DoModal()==IDOK){
		if(dlg.m_BeginShowTime<=0)dlg.m_BeginShowTime=m_UnformTimeInterval;
		if(dlg.m_EndShowTime>m_UnformTimeLength)dlg.m_EndShowTime=m_UnformTimeLength;
		
		m_BeginShowTime=dlg.m_BeginShowTime;
		m_EndShowTime=dlg.m_EndShowTime;
		m_BeginShowPoint=m_BeginShowTime/m_UnformTimeInterval;
		m_EndShowPoint=m_EndShowTime/m_UnformTimeInterval;
		m_ShowPointNumber=m_EndShowPoint-m_BeginShowPoint;
		return TRUE;
	}
	return FALSE;
}
//			BOOL CFbkDoc::AskShowTime()								  END
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//             BOOL CFbkDoc::SaveCFbkDoc()						BEGIN
BOOL CFbkDoc::SaveFstBrk()
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  检查初至文件是否存在, 若否, 建立之.
	FILE *fpFstBrk=fopen(m_FileFirstBreak,"r");
	if(!fpFstBrk){
		fpFstBrk=fopen(m_FileFirstBreak,"w");
		if(!fpFstBrk){
			AfxMessageBox("无法建立初至文件:"+m_FileFirstBreak+" !");
			return FALSE;
		}
	}
	fclose(fpFstBrk);			

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	     若是最后一页, 则应写进文件的数据数目不是每页显示道数,
	//而是总道数对每页道数取余.
	long n;
	if(m_CurrentPageNumber==m_TotalPageNumber){
		n=m_UnformGroupNumber-m_GrpNrOnOnePage*(m_CurrentPageNumber-1);
	}
	else{
		n=m_GrpNrOnOnePage;
	}
		
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	 写数据进文件.
	fpFstBrk=fopen(m_FileFirstBreak,"rb+");

	long P=(m_CurrentPageNumber-1)*m_GrpNrOnOnePage*sizeof(DataInFstBrkFile);  
	fseek(fpFstBrk,P,SEEK_SET);
	fwrite(m_pDataInFstBrkFile,sizeof(DataInFstBrkFile),n,fpFstBrk);
	
	fclose(fpFstBrk);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//返回.
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
	dlg.m_BN=*Bn;
	dlg.m_EN=*En;
	if(dlg.DoModal()==IDCANCEL)return FALSE;
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// 赋值.
	*Bn=dlg.m_BN;
	*En=dlg.m_EN;
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// 控制.
	if(*Bn<1) *Bn=1;
	if(*En>m_UnformGroupNumber) *En=m_UnformGroupNumber;
	
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
	for(i=0;i<m_GrpNrOnOnePage;i++){
		m=i*m_DataNumberOfOneGroup+m_UnformGroupHead;
		for(j=0;j<m_PointNumberOfOneGroup;j++){
			m_pGroupData[m+j]*=coef;
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

	// 读取该页的数据.
	n=m_DataNumberOfOneGroup*m_GrpNrOnOnePage;
	float *DataOfGroup=new float[n];// m_DataNumberOfOneGroup*2];
	n=(m_CurrentPageNumber-1)*m_GrpNrOnOnePage;
	GetGroup(DataOfGroup,n,m_GrpNrOnOnePage);  // 本函数属于 UnformFile.
	
	// 对该页上的所有道循环, 若有选取道, 则学习之.
	AllLearned=0;
	for(int i=0;i<m_GrpNrOnOnePage;i++){
		if(m_pSelectedGroups[i]){
			AllLearned=2;
			Point=m_pDataInFstBrkFile[i].FirstBreak/m_UnformTimeInterval;
			n=i*m_DataNumberOfOneGroup+m_UnformGroupHead;
			m_neunet.Set(&DataOfGroup[n],m_PointNumberOfOneGroup,Point);
			if(!m_neunet.Learn())AllLearned=1;
		}
	}

	//删除临时数据内存.
	delete 	DataOfGroup;
	
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
	if(m_CurrentPageNumber<1){
		m_CurrentPageNumber=1;
		return FALSE;
	}
	if(m_CurrentPageNumber>m_TotalPageNumber){
		m_CurrentPageNumber=m_TotalPageNumber;
		return FALSE;
	}

	
	//1.读取波形数据.
	long BeginGroup=(m_CurrentPageNumber-1)*m_GrpNrOnOnePage; // UnformGroupHead: Predefined as 128 while using FORMAT_4 .
	GetGroup(m_pGroupData,BeginGroup,m_GrpNrOnOnePage);
	AdjustGroupData(m_ShowInflateTimes);

	// 2.读取初至数据
	for(i=0;i<m_GrpNrOnOnePage;i++){   // 首先充零,以备最后一页道数不足.
		m_pDataInFstBrkFile[i].FileNumber=0;
		m_pDataInFstBrkFile[i].FirstBreak=0;
	}
		
	FILE* fp=fopen(m_FileFirstBreak,"rb");  // 读取数据
	if(fp){
		long P=BeginGroup*sizeof(DataInFstBrkFile);  // position of file will be searched.
		fseek(fp,P,SEEK_SET);
		fread(m_pDataInFstBrkFile,sizeof(DataInFstBrkFile),m_GrpNrOnOnePage,fp);
		fclose(fp);
	}

	// 3.因为数据变化了, 所以重置SelectedGroups,和m_GroupInverted.
	for(i=0;i<m_GrpNrOnOnePage;i++){
		m_pSelectedGroups[i]=FALSE;
	}
}
//  RaedData.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		