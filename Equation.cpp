////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//      NAME : class EQUATION.CPP
// PARAMETERS:
//  2.The number of the Left-Top V-Block is 0.
//  3. X Direction :   direction perpendicular to the recieve line.                               
//      Y Direction :   direction parrall to the recieve line.                                       
//  4. ShotPointNumber and RecievePoint Number:
//                             are both definited as:          
//                             They increase on X direction firstly,
//                              and increase to the second Y line.
//  4.  Ph of Shot Points:    
//                           MUST be 6 caractars. the first 3 is for Shot Line 
//		                     and the second 3 is for shot point line.	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3dfstbrk.h"
#include "stdio.h"
#include "struct.h"
#include "svsys.h"
#include "swathpar.h"
#include "spp.h"
#include "equation.h"
#include "progdlg.h"
#include "fh.h"
#include "SPPDoc.h"
#include "MidFbkFile.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Construction Code  EQUATION()						                              		BEGIN
Equation::Equation()
{
	Reset();
}

void Equation::Reset()
{ 
	ss.Reset();
	sp.Reset();

}

//  Construction Code  EQUATION()						                              		END
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Construction Code  EQUATION()						                              		BEGIN
bool Equation::Set(int Swath)
{
	SetFileName(Swath);

	if(!ss.Set())return false;	;   
	if(!sp.Set(Swath))return false;

	// ���� 1.���ڵ���Ŀ,2.�ܼ첨����Ŀ,3. ��,�����Ŀ֮��.
	CSPPDoc spp;
	spp.Set(Swath);
	if(!spp.Read())return false;
	m_TotalSNumber=spp.m_TotalSNumber;
	m_TotalRNumber=sp.TotalRecievePointNumber;
	m_TotalRSNumber=m_TotalSNumber+m_TotalRNumber;
	
	//  the Above two kind of parameters are good.
	return TRUE;
}
//  Construction Code  EQUATION()													END
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   ������:	  Equation::ReadInputStaticData
//   ��    ��:     ��ȡ������������õľ�У����.
//   ��    ��:
//						 PXYV *S,�洢�ڵ㾲У�������ڴ���ʼ��ַ, 
//						 PXYV *R,�洢�첨�㾲У�������ڴ���ʼ��ַ, 
//					         			�ṹΪ: ׮��,X����,Y����,��У����.
//						long *SN , �ڵ���Ŀ�ĵ�ַ.
//						long *RN , �첨����Ŀ�ĵ�ַ.
//
//   ��    ��:      BOOL, �ɹ� TRUE.
//   ��    ��:      1997.9.24
BOOL Equation::ReadInputStaticData(PXYV *S,PXYV *R,long *SN,long *RN)
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  read shot point pxyv.
	FILE *fp=fopen(m_FileInputShot,"r");
	if(!fp){
		AfxMessageBox("��ʼ��У�����ļ�: "+m_FileInputShot+" ������!");
		return FALSE;
	}

	fseek(fp,0,SEEK_END);
	float TotalLen=(float)ftell(fp);
	long CurrPos;
	fseek(fp,0,SEEK_SET);
	
	CProgressDlg dlg;
	dlg.Create();
	dlg.SetWindowText("���ڶ�ȡ����");
	dlg.SetStatus("��ȡ�����������õ����ڵ㾲У���ļ���");

	long n=0;
	int nRead=0;
	while(!feof(fp)){
		nRead=fscanf(fp,"%d  %lf  %lf  %f\n",&S[n].Ph,&S[n].east,&S[n].north,&S[n].Value);
		if(nRead<4){
			AfxMessageBox("���������õ����ڵ㾲У�����ļ���Ԥ��Ҫ��!");
			return false;
		}

		n++;
		if(n%10==0){
			CurrPos=ftell(fp);
			dlg.SetPos(CurrPos/TotalLen*100.0);
		}
	}
	*SN=n;
	fclose(fp);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  read recieve point pxyv.
	fp=fopen(m_FileInputRecieve,"r");
	if(!fp){
		AfxMessageBox("��ʼ��У�����ļ�: "+m_FileInputRecieve+" ������!");
		return FALSE;
	}
	fseek(fp,0,SEEK_END);
	TotalLen=(float)ftell(fp);
	fseek(fp,0,SEEK_SET);

	dlg.SetWindowText("���ڶ�ȡ����");
	dlg.SetStatus("��ȡ�����������õ��ļ첨�㾲У���ļ���");
	n=0;
	while(!feof(fp)){
		nRead=fscanf(fp,"%ld %lf %lf %f\n",&R[n].Ph,&R[n].east,&R[n].north,&R[n].Value);
		if(nRead<4){
			AfxMessageBox("���������õ��ļ첨�㾲У�����ļ���Ԥ��Ҫ��!");
			return false;
		}
		n++;
		if(n%10==0){
			CurrPos=ftell(fp);
			dlg.SetPos(CurrPos/TotalLen*100.0);
		}
	}
	*RN=n;
	fclose(fp);
	dlg.DestroyWindow();

	return TRUE;
}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   ������:	  Equation::SaveInputStaticData
//   ��    ��:      ��������������õľ�У����,�����ļ�ת��.
//    ��    ��:
//						 PXYV *S,�洢�ڵ㾲У�������ڴ���ʼ��ַ, 
//						 PXYV *R,�洢�첨�㾲У�������ڴ���ʼ��ַ, 
//	         			�ṹΪ: ׮��,X����,Y����,��У����.
//						long SN , �ڵ���Ŀ.
//						long RN , �첨����Ŀ.
//   ��    ��:      BOOL, �ɹ� TRUE.
//   ��    ��:      1997.9.24
BOOL Equation::SaveInputStaticData(PXYV *S,PXYV *R,long SN,long RN)
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  ��ȡ�ڵ�����
	FILE *fp=fopen(m_FileInputShot,"w");
	if(!fp){
		AfxMessageBox("�޷�������ʼ��У�����ļ�: "+m_FileInputShot+" !");
		return FALSE;
	}

	for(int i=0;i<SN;i++){
		fprintf(fp,"%d  %lf  %lf  %f\n",S[i].Ph,S[i].east,S[i].north,S[i].Value);
	}
	fclose(fp);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  ��ȡ�첨������
	fp=fopen(m_FileInputRecieve,"w");
	if(!fp){
		AfxMessageBox("�޷�������ʼ��У�����ļ�: "+m_FileInputRecieve+" !");
		return FALSE;
	}

	for(i=0;i<RN;i++){
		fprintf(fp,"%d  %lf  %lf  %f\n",R[i].Ph,R[i].east,R[i].north,R[i].Value);
	}
	fclose(fp);

	// ����.
	return TRUE;
}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    ������:  Equation::ReadMidStaticData
//    ��    ��:  ��ȡ����������õľ�У�����ɹ��ļ�.
//    ��    ��:	double *RSRoom,���ڽ��վ�У�������ڴ���ʼ��ַ.
//    ��    ��: 1997.9.24
BOOL Equation::ReadMidStaticData(long *Ph,double *RSRoom)
{
	// ���ļ�.
	FILE *fps=fopen(m_FileMidShot,"r");
	if(!fps){
		AfxMessageBox("�ڵ��м�ɹ��ļ� :"+m_FileMidShot+" ������!");
		return FALSE;
	}
	FILE *fpr=fopen(m_FileMidRecieve,"r");
	if(!fpr){
		AfxMessageBox("�첨���м�ɹ��ļ� :"+m_FileMidRecieve+" ������!");
		return FALSE;
	}

	// ��ȡ�ڵ�����.
	for(int i=0;i<m_TotalSNumber;i++){
		if(feof(fps))return FALSE;
		fscanf(fps,"%ld %lf\n",&Ph[i],&RSRoom[i]);
	}

	// ��ȡ�첨������.
	for(i=0;i<m_TotalRNumber;i++){
		if(feof(fpr))return FALSE;
		fscanf(fpr,"%ld %lf",&Ph[m_TotalSNumber+i],&RSRoom[m_TotalSNumber+i]);
	}

	fclose(fps);
	fclose(fpr);

	// ����������.
	return TRUE;
}

//    ������:  Equation::ReadMidStaticData									����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    ������:  Equation::SaveMidStaticData
//    ��    ��:  �Ա���������õľ�У�����ɹ��ļ� ����.
//    ��    ��:	double *RSRoom,�洢��У�������ڴ���ʼ��ַ.
//    ��    ��: 1997.9.24
BOOL Equation::SaveMidStaticData(double * RSRoom)
{
	CMidFbkFile file;
	
	// Save the shot parameter:
	CSPPDoc sppd;
	sppd.Set(m_SwathNumber);
	if(!sppd.Read())return FALSE;

	file.SetShotNumber (m_TotalSNumber );
	for(long i=0;i<m_TotalSNumber ;i++){
		file.m_pShotData [i].Ph =sppd.m_pSPP [i].m_Ph ;
		file.m_pShotData [i].Value =-RSRoom[i];
	}
	if(!file.WriteShot ())return false;

	
	// Save the rcv data:
	file.SetRcvNumber (m_TotalRNumber );
	long j,n,Ph;
	for(i=0;i<m_TotalRNumber/ss.RecieveLineNumber;i++){
		for(j=0;j<ss.RecieveLineNumber;j++){
			long PhL=sp.RecieveLineName[j]*1000;
			long PhR=sp.FirstRecievePointNumber+i;
			Ph=PhL+PhR;

			//Ph=sp.RecieveLineName[j]*1000+sp.FirstRecievePointNumber+i;
			n=i*ss.RecieveLineNumber+j;  
			file.m_pRcvData [n].Ph =Ph;
			file.m_pRcvData [n].Value =-RSRoom[n+m_TotalSNumber];
		}
	}

	if(!file.WriteRcv ())return false;

	//
	return TRUE;
}

//    ������:  Equation::SaveMidStaticData									����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    ������:  Equation::ReadOutputStaticData
//    ��    ��:  ��ȡ����������, ����������У���ľ�У����, �����ļ���ʽת��
//    ��    ��:
//						 PXYV *S,�洢�ڵ㾲У�������ڴ���ʼ��ַ, 
//						 PXYV *R,�洢�첨�㾲У�������ڴ���ʼ��ַ, 
//	         			�ṹΪ: ׮��,X����,Y����,��У����.
//    ��    ��: 1997.9.24
BOOL Equation::ReadOutputStaticData(PXYV *S,PXYV *R)
{
	// ��ȡ�ڵ�����.
	FILE *fp=fopen(m_FileOutputShot,"r");
	if(!fp){
		AfxMessageBox("�ɹ��ļ�:"+m_FileOutputShot+" ������!");
		return FALSE;
	}

	for(int i=0;i<m_TotalSNumber;i++){
		if(feof(fp))return FALSE;
		fscanf(fp,"%ld  %lf  %lf  %f\n",&S[i].Ph,&S[i].east,&S[i].north,&S[i].Value);
	}

	fclose(fp);

	// ��ȡ�첨������.
	fp=fopen(m_FileOutputRecieve,"r");
	if(!fp){
		AfxMessageBox("�޷������ɹ��ļ�:"+m_FileOutputRecieve);
		return FALSE;
	}

	for(i=0;i<m_TotalRNumber;i++){
		if(feof(fp))return FALSE;
		fscanf(fp,"%ld  %lf  %lf  %f\n", &R[i].Ph,&R[i].east,&R[i].north,&R[i].Value);
	}

	fclose(fp);

	// ����������.
	return TRUE;
}
//    ������:  Equation::ReadOutputStaticData									����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    ������:  Equation::SaveOutputStaticData
//    ��    ��:  �Ա���������, ����������У���ľ�У����, ����.
//    ��    ��:
//						 PXYV *S,�洢�ڵ㾲У�������ڴ���ʼ��ַ, 
//						 PXYV *R,�洢�첨�㾲У�������ڴ���ʼ��ַ, 
//	         			�ṹΪ: ׮��,X����,Y����,��У����.
//
//    ��    ��: 1997.9.24
BOOL Equation::SaveOutputStaticData(PXYV *S,PXYV *R)
{
	// ���ڵ�����.
	FILE *fp=fopen(m_FileOutputShot,"w");
	if(!fp){
		AfxMessageBox("�޷������ɹ��ļ�:"+m_FileOutputShot);
		return FALSE;
	}

	CProgressDlg dlg;
	dlg.Create();
	dlg.SetWindowText("�洢����");
	dlg.SetStatus("���ڴ洢�ڵ����ճɹ���У����:");
	for(int i=0;i<m_TotalSNumber;i++){
		fprintf(fp,"%ld  %9.1f  %10.1f  %5.2f\n",S[i].Ph,S[i].east,S[i].north,S[i].Value);
		if(i%10==0){
			dlg.SetPos((float)i/m_TotalSNumber*100);
		}
	}
	fclose(fp);

	// ��첨������.
	fp=fopen(m_FileOutputRecieve,"w");
	if(!fp){
		AfxMessageBox("�޷������ɹ��ļ�:"+m_FileOutputRecieve);
		return FALSE;
	}
	dlg.SetStatus("���ڴ洢�첨�����ճɹ���У����:");
	for(i=0;i<m_TotalRNumber;i++){
		fprintf(fp,"%ld  %9.1f   %10.1f  %5.2f\n", R[i].Ph,R[i].east,R[i].north,R[i].Value);
		dlg.SetPos((float)i/m_TotalRNumber*100);
	}
	fclose(fp);

	dlg.DestroyWindow();

	// ����������.
	return TRUE;
}
//    ������:  Equation::SaveOutputStaticData							����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Equation::SetFileName(int Swath)
{
	// Make a common file name, which will be used by derived class.
	m_SwathNumber=Swath;
	m_FileName=IntToName(Swath);

	// �����ļ�������.
	m_FileSwathFirstBreak="swath"+m_FileName+".FBD";

	// Make file names that will be used in derived classes.
	m_FileRS="RS"+m_FileName+".DAT";        // Binary file recording which shot and recieve plused.
	m_Fileb="b"+m_FileName+".DAT";	// Binary file recording S & R value plused.
	m_FileNumInColOfRS="NCRS"+m_FileName+".DAT";
	m_FileRSTurned="RST"+m_FileName+".DAT";

	m_FileInputShot="swath"+m_FileName+".ST";           // Binary File including Ph,X,Y,V, calculated by Sand Wave Method.
	m_FileInputRecieve="swath"+m_FileName+".RT";     // Binary File including Ph,X,Y,V, calculated by Sand Wave Method.
	m_FileMidShot="SMID"+m_FileName+".DAT";	// Binary File including Static Value  ,Calculated by this Program , Not Controlled. 
	m_FileMidRecieve="RMID"+m_FileName+".DAT";	// Binary File including Static Value ,Calculated by this Program , Not Controlled. 
	m_FileOutputShot="SOUT"+m_FileName+".DAT";         // Binary  File that has been controlled.
	m_FileOutputRecieve="ROUT"+m_FileName+".DAT";   // Binary  File that has been controlled.

	m_FileEquationNumber="EN"+m_FileName+".TXT";  // Give value to m_EquationNumber .	

	return true;
}

