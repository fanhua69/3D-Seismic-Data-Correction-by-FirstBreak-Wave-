///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Problems should be  resolved later:
//	1.Should let the user can choose more groups than there are in one shot.
//          but Doing that there is a problem now,I can not resolve it.
//			 ���ڴ������Ѿ����.
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
	//  ����Ҫ�������ֹ����.
	long BeginCalcGroup=1,EndCalcGroup=m_UnformGroupNumber;
	if(!AskCalculatedGroup(&BeginCalcGroup,&EndCalcGroup))return FALSE;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// �򿪳����ļ�. ������NeuNetThink�������.
	FILE *fpFstBrkFile=fopen(m_FileFirstBreak,"rb+");
  	
	DataInFstBrkFile D;
	float *DataOfOneGroup=new float[m_DataNumberOfOneGroup*2];

	CProgressDlg dlg;
	dlg.Create();
	dlg.SetWindowText("������ȡ����");
	float n=EndCalcGroup-BeginCalcGroup+1;
	for(int i=BeginCalcGroup;i<=EndCalcGroup;i++){
		if(i%10==0){
			dlg.SetStatus("�������Ѿ������� "+STR(i)+" ������!");
			dlg.SetPos((i-BeginCalcGroup)/n*100);
			if(dlg.CheckCancelButton())
			if(AfxMessageBox(_T("��ȷʵ��ֹͣ��ȡ������?"),MB_YESNO)==IDYES)
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

	AfxMessageBox("�����������!");
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

		//�޸�:
		//1.ÿҳ��ʾ�ĵ���
		//2.��ǰҳ.
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
	//  �������ļ��Ƿ����, ����, ����֮.
	FILE *fpFstBrk=fopen(m_FileFirstBreak,"r");
	if(!fpFstBrk){
		fpFstBrk=fopen(m_FileFirstBreak,"w");
		if(!fpFstBrk){
			AfxMessageBox("�޷����������ļ�:"+m_FileFirstBreak+" !");
			return FALSE;
		}
	}
	fclose(fpFstBrk);			

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	     �������һҳ, ��Ӧд���ļ���������Ŀ����ÿҳ��ʾ����,
	//�����ܵ�����ÿҳ����ȡ��.
	long n;
	if(m_CurrentPageNumber==m_TotalPageNumber){
		n=m_UnformGroupNumber-m_GrpNrOnOnePage*(m_CurrentPageNumber-1);
	}
	else{
		n=m_GrpNrOnOnePage;
	}
		
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	 д���ݽ��ļ�.
	fpFstBrk=fopen(m_FileFirstBreak,"rb+");

	long P=(m_CurrentPageNumber-1)*m_GrpNrOnOnePage*sizeof(DataInFstBrkFile);  
	fseek(fpFstBrk,P,SEEK_SET);
	fwrite(m_pDataInFstBrkFile,sizeof(DataInFstBrkFile),n,fpFstBrk);
	
	fclose(fpFstBrk);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//����.
	return TRUE;
}
//             BOOL CFbkDoc::SaveCFbkDoc()								END
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	����:		AskCalculatedFileNumber											��ʼ
//	����:	ѯ���û���ֹ������ļ���.
//  �ı�:	*BeginCalculatedFileNumber,��ʼ������ļ���
//				*EndCalculatedFileNumber,��ֹ������ļ���.
// ����:	1997.10.4
BOOL CFbkDoc::AskCalculatedGroup(long  *Bn, long  *En)
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	// ��ʾ�Ի���.
	CAskGroup dlg;
	dlg.m_BN=*Bn;
	dlg.m_EN=*En;
	if(dlg.DoModal()==IDCANCEL)return FALSE;
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// ��ֵ.
	*Bn=dlg.m_BN;
	*En=dlg.m_EN;
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// ����.
	if(*Bn<1) *Bn=1;
	if(*En>m_UnformGroupNumber) *En=m_UnformGroupNumber;
	
	return TRUE;
}
//			BOOL CFbkDoc::AskCalculateShot()							  END
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ����:   AdjustGroupData(float coef)															��ʼ
//  ����:   �����ݵ�ֵ���п���,ʹ֮��Ӧ��Ļ���߼�����.
//  ����:coef,�Ŵ�ϵ��.
//  ����:1997.10.5
void CFbkDoc::AdjustGroupData(float coef)
{
	// �����ݵ�ֵ���п���.����ÿ�λ��ƶ�������.
	long i,j,m;
	for(i=0;i<m_GrpNrOnOnePage;i++){
		m=i*m_DataNumberOfOneGroup+m_UnformGroupHead;
		for(j=0;j<m_PointNumberOfOneGroup;j++){
			m_pGroupData[m+j]*=coef;
		}
	}

}
//  ����:   AdjustGroupData(float coef)															  ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ����:	NeuNetLearn																				   ��ʼ
//  ����:   ��ѡ������ѧϰ.
//  ����:   ���е����ɹ�ѧϰTRUE.
BOOL CFbkDoc::NeuNetLearn()
{
	UINT AllLearned;	  
	long Point,n,LearnTime=0;

	// ��ȡ��ҳ������.
	n=m_DataNumberOfOneGroup*m_GrpNrOnOnePage;
	float *DataOfGroup=new float[n];// m_DataNumberOfOneGroup*2];
	n=(m_CurrentPageNumber-1)*m_GrpNrOnOnePage;
	GetGroup(DataOfGroup,n,m_GrpNrOnOnePage);  // ���������� UnformFile.
	
	// �Ը�ҳ�ϵ����е�ѭ��, ����ѡȡ��, ��ѧϰ֮.
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

	//ɾ����ʱ�����ڴ�.
	delete 	DataOfGroup;
	
	switch(AllLearned){
	case 0:
		AfxMessageBox("�㲢û��ѡȡ�κε�!");
		return FALSE;
	case 1:
		AfxMessageBox("�������е����ܱ�������ѧϰ!");
		return FALSE;
	default:
		AfxMessageBox("�ܺ�,ѧϰ���!");
		return TRUE;
	}

}
//  ����:	NeuNetLearn																				   ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   
BOOL CFbkDoc::ReadData()
{	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  ���ҳ�ŵĺϷ���.
	if(m_CurrentPageNumber<1){
		m_CurrentPageNumber=1;
		return FALSE;
	}
	if(m_CurrentPageNumber>m_TotalPageNumber){
		m_CurrentPageNumber=m_TotalPageNumber;
		return FALSE;
	}

	
	//1.��ȡ��������.
	long BeginGroup=(m_CurrentPageNumber-1)*m_GrpNrOnOnePage; // UnformGroupHead: Predefined as 128 while using FORMAT_4 .
	GetGroup(m_pGroupData,BeginGroup,m_GrpNrOnOnePage);
	AdjustGroupData(m_ShowInflateTimes);

	// 2.��ȡ��������
	for(i=0;i<m_GrpNrOnOnePage;i++){   // ���ȳ���,�Ա����һҳ��������.
		m_pDataInFstBrkFile[i].FileNumber=0;
		m_pDataInFstBrkFile[i].FirstBreak=0;
	}
		
	FILE* fp=fopen(m_FileFirstBreak,"rb");  // ��ȡ����
	if(fp){
		long P=BeginGroup*sizeof(DataInFstBrkFile);  // position of file will be searched.
		fseek(fp,P,SEEK_SET);
		fread(m_pDataInFstBrkFile,sizeof(DataInFstBrkFile),m_GrpNrOnOnePage,fp);
		fclose(fp);
	}

	// 3.��Ϊ���ݱ仯��, ��������SelectedGroups,��m_GroupInverted.
	for(i=0;i<m_GrpNrOnOnePage;i++){
		m_pSelectedGroups[i]=FALSE;
	}
}
//  RaedData.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		