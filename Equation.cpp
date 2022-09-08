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

	// 设置 1.总炮点数目,2.总检波点数目,3. 炮,检点数目之和.
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
//   函数名:	  Equation::ReadInputStaticData
//   功    能:     读取由其它方法获得的静校正量.
//   更    改:
//						 PXYV *S,存储炮点静校正量的内存起始地址, 
//						 PXYV *R,存储检波点静校正量的内存起始地址, 
//					         			结构为: 桩号,X坐标,Y坐标,静校正量.
//						long *SN , 炮点数目的地址.
//						long *RN , 检波点数目的地址.
//
//   返    回:      BOOL, 成功 TRUE.
//   日    期:      1997.9.24
BOOL Equation::ReadInputStaticData(PXYV *S,PXYV *R,long *SN,long *RN)
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  read shot point pxyv.
	FILE *fp=fopen(m_FileInputShot,"r");
	if(!fp){
		AfxMessageBox("初始静校正量文件: "+m_FileInputShot+" 不存在!");
		return FALSE;
	}

	fseek(fp,0,SEEK_END);
	float TotalLen=(float)ftell(fp);
	long CurrPos;
	fseek(fp,0,SEEK_SET);
	
	CProgressDlg dlg;
	dlg.Create();
	dlg.SetWindowText("正在读取数据");
	dlg.SetStatus("读取由其它方法得到的炮点静校正文件：");

	long n=0;
	int nRead=0;
	while(!feof(fp)){
		nRead=fscanf(fp,"%d  %lf  %lf  %f\n",&S[n].Ph,&S[n].east,&S[n].north,&S[n].Value);
		if(nRead<4){
			AfxMessageBox("其他方法得到的炮点静校正量文件比预期要短!");
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
		AfxMessageBox("初始静校正量文件: "+m_FileInputRecieve+" 不存在!");
		return FALSE;
	}
	fseek(fp,0,SEEK_END);
	TotalLen=(float)ftell(fp);
	fseek(fp,0,SEEK_SET);

	dlg.SetWindowText("正在读取数据");
	dlg.SetStatus("读取由其它方法得到的检波点静校正文件：");
	n=0;
	while(!feof(fp)){
		nRead=fscanf(fp,"%ld %lf %lf %f\n",&R[n].Ph,&R[n].east,&R[n].north,&R[n].Value);
		if(nRead<4){
			AfxMessageBox("其他方法得到的检波点静校正量文件比预期要短!");
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
//   函数名:	  Equation::SaveInputStaticData
//   功    能:      存由其它方法获得的静校正量,用于文件转换.
//    输    入:
//						 PXYV *S,存储炮点静校正量的内存起始地址, 
//						 PXYV *R,存储检波点静校正量的内存起始地址, 
//	         			结构为: 桩号,X坐标,Y坐标,静校正量.
//						long SN , 炮点数目.
//						long RN , 检波点数目.
//   返    回:      BOOL, 成功 TRUE.
//   日    期:      1997.9.24
BOOL Equation::SaveInputStaticData(PXYV *S,PXYV *R,long SN,long RN)
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  读取炮点数据
	FILE *fp=fopen(m_FileInputShot,"w");
	if(!fp){
		AfxMessageBox("无法建立初始静校正量文件: "+m_FileInputShot+" !");
		return FALSE;
	}

	for(int i=0;i<SN;i++){
		fprintf(fp,"%d  %lf  %lf  %f\n",S[i].Ph,S[i].east,S[i].north,S[i].Value);
	}
	fclose(fp);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  读取检波点数据
	fp=fopen(m_FileInputRecieve,"w");
	if(!fp){
		AfxMessageBox("无法建立初始静校正量文件: "+m_FileInputRecieve+" !");
		return FALSE;
	}

	for(i=0;i<RN;i++){
		fprintf(fp,"%d  %lf  %lf  %f\n",R[i].Ph,R[i].east,R[i].north,R[i].Value);
	}
	fclose(fp);

	// 返回.
	return TRUE;
}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    函数名:  Equation::ReadMidStaticData
//    功    能:  读取本软件计算获得的静校正量成果文件.
//    改    变:	double *RSRoom,用于接收静校正量的内存起始地址.
//    日    期: 1997.9.24
BOOL Equation::ReadMidStaticData(long *Ph,double *RSRoom)
{
	// 打开文件.
	FILE *fps=fopen(m_FileMidShot,"r");
	if(!fps){
		AfxMessageBox("炮点中间成果文件 :"+m_FileMidShot+" 不存在!");
		return FALSE;
	}
	FILE *fpr=fopen(m_FileMidRecieve,"r");
	if(!fpr){
		AfxMessageBox("检波点中间成果文件 :"+m_FileMidRecieve+" 不存在!");
		return FALSE;
	}

	// 读取炮点数据.
	for(int i=0;i<m_TotalSNumber;i++){
		if(feof(fps))return FALSE;
		fscanf(fps,"%ld %lf\n",&Ph[i],&RSRoom[i]);
	}

	// 读取检波点数据.
	for(i=0;i<m_TotalRNumber;i++){
		if(feof(fpr))return FALSE;
		fscanf(fpr,"%ld %lf",&Ph[m_TotalSNumber+i],&RSRoom[m_TotalSNumber+i]);
	}

	fclose(fps);
	fclose(fpr);

	// 返回主程序.
	return TRUE;
}

//    函数名:  Equation::ReadMidStaticData									结束
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    函数名:  Equation::SaveMidStaticData
//    功    能:  对本软件计算获得的静校正量成果文件 存盘.
//    输    入:	double *RSRoom,存储静校正量的内存起始地址.
//    日    期: 1997.9.24
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

//    函数名:  Equation::SaveMidStaticData									结束
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    函数名:  Equation::ReadOutputStaticData
//    功    能:  读取本软件计算的, 经过长波长校正的静校正量, 用于文件格式转换
//    更    改:
//						 PXYV *S,存储炮点静校正量的内存起始地址, 
//						 PXYV *R,存储检波点静校正量的内存起始地址, 
//	         			结构为: 桩号,X坐标,Y坐标,静校正量.
//    日    期: 1997.9.24
BOOL Equation::ReadOutputStaticData(PXYV *S,PXYV *R)
{
	// 读取炮点数据.
	FILE *fp=fopen(m_FileOutputShot,"r");
	if(!fp){
		AfxMessageBox("成果文件:"+m_FileOutputShot+" 不存在!");
		return FALSE;
	}

	for(int i=0;i<m_TotalSNumber;i++){
		if(feof(fp))return FALSE;
		fscanf(fp,"%ld  %lf  %lf  %f\n",&S[i].Ph,&S[i].east,&S[i].north,&S[i].Value);
	}

	fclose(fp);

	// 读取检波点数据.
	fp=fopen(m_FileOutputRecieve,"r");
	if(!fp){
		AfxMessageBox("无法建立成果文件:"+m_FileOutputRecieve);
		return FALSE;
	}

	for(i=0;i<m_TotalRNumber;i++){
		if(feof(fp))return FALSE;
		fscanf(fp,"%ld  %lf  %lf  %f\n", &R[i].Ph,&R[i].east,&R[i].north,&R[i].Value);
	}

	fclose(fp);

	// 返回主程序.
	return TRUE;
}
//    函数名:  Equation::ReadOutputStaticData									结束
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    函数名:  Equation::SaveOutputStaticData
//    功    能:  对本软件计算的, 经过长波长校正的静校正量, 存盘.
//    输    入:
//						 PXYV *S,存储炮点静校正量的内存起始地址, 
//						 PXYV *R,存储检波点静校正量的内存起始地址, 
//	         			结构为: 桩号,X坐标,Y坐标,静校正量.
//
//    日    期: 1997.9.24
BOOL Equation::SaveOutputStaticData(PXYV *S,PXYV *R)
{
	// 存炮点数据.
	FILE *fp=fopen(m_FileOutputShot,"w");
	if(!fp){
		AfxMessageBox("无法建立成果文件:"+m_FileOutputShot);
		return FALSE;
	}

	CProgressDlg dlg;
	dlg.Create();
	dlg.SetWindowText("存储数据");
	dlg.SetStatus("正在存储炮点最终成果静校正量:");
	for(int i=0;i<m_TotalSNumber;i++){
		fprintf(fp,"%ld  %9.1f  %10.1f  %5.2f\n",S[i].Ph,S[i].east,S[i].north,S[i].Value);
		if(i%10==0){
			dlg.SetPos((float)i/m_TotalSNumber*100);
		}
	}
	fclose(fp);

	// 存检波点数据.
	fp=fopen(m_FileOutputRecieve,"w");
	if(!fp){
		AfxMessageBox("无法建立成果文件:"+m_FileOutputRecieve);
		return FALSE;
	}
	dlg.SetStatus("正在存储检波点最终成果静校正量:");
	for(i=0;i<m_TotalRNumber;i++){
		fprintf(fp,"%ld  %9.1f   %10.1f  %5.2f\n", R[i].Ph,R[i].east,R[i].north,R[i].Value);
		dlg.SetPos((float)i/m_TotalRNumber*100);
	}
	fclose(fp);

	dlg.DestroyWindow();

	// 返回主程序.
	return TRUE;
}
//    函数名:  Equation::SaveOutputStaticData							结束
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Equation::SetFileName(int Swath)
{
	// Make a common file name, which will be used by derived class.
	m_SwathNumber=Swath;
	m_FileName=IntToName(Swath);

	// 初至文件的名称.
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

