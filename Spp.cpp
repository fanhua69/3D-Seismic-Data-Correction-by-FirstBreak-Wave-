
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	注意：所有物理点均计入炮点个数。只是空废炮的文件号设为0。
//				在建立、求解方程时，本软件考虑到空废炮的问题，专做处理。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include"3dfstbrk.h"
#include "struct.h"
#include "stdio.h"
#include "svsys.h" 
#include "swathpar.h"
#include "spp.h"
#include "SPPMaker.h"
#include "Mylist.h"
#include "fh.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//				operator=													                      BEGIN
ShotPointPar ShotPointPar::operator=(ShotPointPar Other)
{	
		m_Ph=Other.m_Ph;
		m_FileNumber=Other.m_FileNumber;
		m_ZP=Other.m_ZP;
		m_HP=Other.m_HP;
		m_BeginTrace=Other.m_BeginTrace;
		m_EndTrace=Other.m_EndTrace;
		m_BeginGapTrace=Other.m_BeginGapTrace;
		m_EndGapTrace=Other.m_EndGapTrace;
		m_XZB=Other.m_XZB;
		m_YZB=Other.m_YZB;
		return *this;
}
//				ShotPointPar ShotPointPar::operator=(ShotPointPar Other)			      END
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//					void ShotPointPar::Reset()																BEGIN
void ShotPointPar::Reset()
{	
	m_Ph=0;
	m_FileNumber=0;
	m_ZP=0;
	m_HP=0;
	m_BeginTrace=0;
	m_EndTrace=0;
	m_BeginGapTrace=0;
	m_EndGapTrace=0;
	m_XZB=0;
	m_YZB=0;
}
//				void ShotPointPar::Reset													END
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   CONSTRUCTION CODE:    ShotPointPar()									BEGIN
ShotPointParDealer::ShotPointParDealer(int Swath)
{
	m_DataOK=FALSE;
	m_SwathNumber=Swath;
	SetFileName(Swath);

		
	m_TotalSNumber=GetShotNumber();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    Calculate TotalRSNumber.
	SwathParameter sp(m_SwathNumber);
	if(!sp.DataOK)return;
	m_TotalRSNumber=m_TotalSNumber+sp.TotalRecievePointNumber;

	m_DataOK=true;
	
	//返回.
	return;
}

//   CONSTRUCTION CODE:    ShotPointParDealer()									BEGIN
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          Name : ReadSwath
//		Function : Read Shot point Parameter of one swath , and Sort them on m_Ph.
//		       Date: 1997.3
//  Repro Date: 1997.9.9
//  Programer :  FAN HUA
//	注意：所有物理点均计入炮点个数。只是空废炮的文件号设为0。
//				在建立、求解方程时，本软件考虑到空废炮的问题，专做处理。
BOOL ShotPointParDealer::ReadSwath(ShotPointPar *spp)
{                 
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Open the Shot Point Parameter file.
	FILE *fp=fopen(m_FileShotPointPar,"r");
	if(!fp){
		AfxMessageBox("炮点偏移文件 "+m_FileShotPointPar+" 没有找到！",MB_OK);
		return NULL;   //该函数应返回一个ShotPointPar地址.
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//   Loop to read the shot point parameter . 
	long n=0;
	while(!feof(fp)){
		fscanf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %lf %lf\n",
			&spp[n].m_Ph,
			&spp[n].m_FileNumber,
			&spp[n].m_ZP,
			&spp[n].m_HP,
			&spp[n].m_BeginTrace,
			&spp[n].m_EndTrace,
			&spp[n].m_BeginGapTrace,
			&spp[n].m_EndGapTrace,
			&spp[n].m_XZB,
			&spp[n].m_YZB);

		if(spp[n].m_Ph>9999999||spp[n].m_Ph<0) {
			AfxMessageBox("炮点参数文件错误，无法继续，请检查！");
			return FALSE;
		}
		
		n++;
	}
	fclose(fp);

	m_TotalSNumber=n;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    Calculate TotalRSNumber.
	SwathParameter sp(m_SwathNumber);
	if(!sp.DataOK)return FALSE;
	m_TotalRSNumber=m_TotalSNumber+sp.TotalRecievePointNumber;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Sort the pars on m_Ph	
	SortSwath(spp);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// return .
	return TRUE;
}
//                ShotPointPar*  ShotPointParDealer::ReadSwath()									END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          Name : GetShotNumber
//		Function : Only  for getting the number of the shot point .
//		       Date: 1998.11
//      return     : The number of the shots, 0 if can not find the file.
//  Programer :  FAN HUA
//	注意：所有物理点均计入炮点个数。只是空废炮的文件号设为0。
//				在建立、求解方程时，本软件考虑到空废炮的问题，专做处理。
//   
long ShotPointParDealer::GetShotNumber()
{                 
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Open the Shot Point Parameter file.
	FILE *fp=fopen(m_FileShotPointPar,"r");			 
	if(!fp){
		AfxMessageBox("炮点偏移文件 "+m_FileShotPointPar+" 没有找到！",MB_OK);
		return 0;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//   Loop to read the shot point parameter . 
	long n=0;
	long Ph,FileNumber,ZP,HP,BeginTrace,EndTrace,BeginGapTrace,EndGapTrace;
	double XZB,YZB;
	while(!feof(fp)){
		fscanf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %lf %lf\n",
			&Ph,
			&FileNumber,
			&ZP,
			&HP,
			&BeginTrace,
			&EndTrace,
			&BeginGapTrace,
			&EndGapTrace,
			&XZB,
			&YZB);

		if(Ph>9999999||Ph<0) {
			AfxMessageBox("炮点参数文件错误，无法继续，请检查！");
			return 0;
		}
		
		n++;
	}
	fclose(fp);

	return n;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// return .
	return TRUE;
}
//                ShotPointPar*  ShotPointParDealer::ReadSwath()									END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   名称: WriteSwath
//   功能:把用户送过来的炮点参数存入文件.
//   注释:不考虑空废炮,即把所有桩号全部装入文件,空废炮由文件号标识.  
//            m_TotalSNumber 变量由 ReadSwath 获得.
//   输入:ShotPointPar * spp, 炮点参数的起始地址, 
//             long TotalPointNumber, 炮点参数的总数.
//   返回:成功TRUE.
BOOL ShotPointParDealer::WriteSwath(ShotPointPar* spp,long TotalPointNumber)
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  Open the shot point file .
	FILE *fp=fopen(m_FileShotPointPar,"w");
	if(!fp){
		AfxMessageBox("无法建立炮点参数文件 "+m_FileShotPointPar);
		return FALSE;
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Write the data into the file.
	for(int i=0;i<TotalPointNumber;i++){
		fprintf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %9.1f %10.1f\n",
			spp[i].m_Ph,spp[i].m_FileNumber,spp[i].m_ZP,spp[i].m_HP,
			spp[i].m_BeginTrace,spp[i].m_EndTrace,spp[i].m_BeginGapTrace,
			spp[i].m_EndGapTrace,spp[i].m_XZB,spp[i].m_YZB);
	}
	fclose(fp);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// return.
	return TRUE;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  名称: Integrate.
//  功能: 合并各个单个文件成为一个总文件:炮点参数文件.
//	注意: 其它单个文件须由用户自己形成.
//  日期: 1997.9.29.
BOOL ShotPointParDealer::MakeFileFromOther()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//   若炮点参数文件已经存在，则询问用户是否继续。
	if(m_DataOK)  // 构造 *this 时, 已经调用了ReadSwath函数.
		if(AfxMessageBox("本束线的炮点参数文件已经存在,是否继续?",MB_YESNO)==IDNO)
			return FALSE;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  建立炮点参数框架。
	ShotPointPar *spp=new ShotPointPar[5000];
	for(int i=0;i<5000;i++)spp[i].Reset();

	ReadSwath(spp);
	long ItemNumber=m_TotalSNumber;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 询问用户所要合并的参数.
	CSPPMaker dlg;
	if(dlg.DoModal()==IDCANCEL)return FALSE;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  添加各种数据。
	if(dlg.m_PlusPh)PlusPh(spp,&ItemNumber,dlg.m_PhByProgram,dlg.m_PhFile);
	if(ItemNumber==0){
		AfxMessageBox("必须先有桩号,才能添加其它数据!");
		return FALSE;
	}

	if(dlg.m_PlusFileNumber)PlusFileNumber(spp,ItemNumber,dlg.m_FileNumberFile);
	if(dlg.m_PlusKF)PlusKF(spp,ItemNumber,dlg.m_KFFile);
	if(dlg.m_PlusOffset)PlusOffset(spp,ItemNumber,dlg.m_OffsetFile);
	if(dlg.m_PlusCor)PlusCor(spp,ItemNumber,dlg.m_StaticDataFile);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	存储数据.
	if(!WriteSwath(spp,ItemNumber)){
		AfxMessageBox("在存储炮点参数文件时出错!");	
		return FALSE;
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 返回.
	AfxMessageBox("炮点参数文件存储完毕!");	
	return TRUE;
}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  名称：PlusPh																											开始
//	 功能：建立起始桩号。
//  日期：1997.10.16
BOOL ShotPointParDealer::PlusPh(ShotPointPar *spp,long *ItemNumber,int PhByProgram,CString File)
{
		// 用户要求由程序生成理论桩号.
		if(PhByProgram<=0){
			long *Ph=new long[5000];   // allocated.
			SwathParameter sp(m_SwathNumber);
			if(!sp.DataOK){
				AfxMessageBox("请先输入本束线的参数,然后再形成炮点参数文件!");
				return FALSE;
			}
			sp.MakeShotPh(Ph,ItemNumber);
			for(int i=0;i<*ItemNumber;i++){
				spp[i].m_Ph=Ph[i];
			}
			delete Ph;							   // deleted	.
		}

		// 用户要从桩号文件中得到桩号.
		else{
			if(File=="")File="SWATH"+IntToName(m_SwathNumber)+".PH";
			FILE *fp=fopen(File,"r");
			if(!fp){
				AfxMessageBox("无法打开桩号文件"+File+"!");
				return FALSE;
			}

			long i=0;
			while(!feof(fp)){
				fscanf(fp,"%ld\n",&spp[i].m_Ph);
				i++;
			}
			*ItemNumber=i;
			fclose(fp);
		}

		AfxMessageBox("桩号已经被添加进炮点参数中了!");
		return TRUE;
}
//   名称：PlusPh 																													  结束
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	名称：PlusKF																													   开始
//  功能：在炮点参数的框架内， 寻找空废炮的桩号，令该桩号的文件号为零。
//	输入：spp，炮点参数框架的起始位置，
//				ItemNumber,炮点框架内的参数数目。
//				File,空废炮文件的文件名。
//  改变：spp.
//  返回：成功TRUE.
BOOL ShotPointParDealer::PlusKF(ShotPointPar *spp,long ItemNumber,CString File)
{
	//  检查文件名是否合法。
	if(File=="")File="SWATH"+IntToName(m_SwathNumber)+".KF";
			
	FILE *fp=fopen(File,"r");
	if(!fp){
		AfxMessageBox("无法打开空废炮桩号文件"+File+"！");
		return FALSE;
	}

	// 读入桩号。
	long Ph,n,NotFoundPhNumber=0;
	long* NotFoundPh=new long[4000];
	while(!feof(fp)){
		fscanf(fp,"%ld\n",&Ph);
		n=SearchPhInSPP(Ph, spp, ItemNumber);
		if(n==-1&&NotFoundPhNumber<4000){
			NotFoundPh[NotFoundPhNumber]=Ph;
			NotFoundPhNumber++;
			continue;
		}
		spp[n].m_FileNumber=-1;
	}
	
	// 列出未发现的桩号。
	ListNotFoundPh(NotFoundPh,NotFoundPhNumber,"NotFndKF.TXT");
	AfxMessageBox("空废炮已经被添加进炮点参数文件中了!");
	delete NotFoundPh;
	
	//  返回。
	return TRUE;
}
//	名称：PlusKF																													   结束
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	名称：PlusFileNumber																									   开始
//  功能：在炮点参数的框架内， 寻找文件号的桩号，将该桩号的文件号置入。
//	输入：spp，炮点参数框架的起始位置，
//				ItemNumber,炮点框架内的参数数目。
//				KFFile,空废炮文件的文件名。
//  改变：spp.
//  返回：成功TRUE.
BOOL ShotPointParDealer::PlusFileNumber(ShotPointPar *spp,long ItemNumber,CString File)
{
	// 检查文件名的合法性。
	if(File=="")File="SWATH"+IntToName(m_SwathNumber)+".FN";
	
	FILE *fp=fopen(File,"r");
	if(!fp){
		AfxMessageBox("无法打开文件号文件:"+File);
		return FALSE;
	}
	
	//  读入数据。
	long NotFoundPhNumber=0,Ph,n,FileNumber;		
	long *NotFoundPh=new long[4000];
	while(!feof(fp)){
		fscanf(fp,"%ld %ld\n",&Ph,&FileNumber);
		n=SearchPhInSPP(Ph, spp, ItemNumber);
		if(n==-1&&NotFoundPhNumber<4000){
			NotFoundPh[NotFoundPhNumber]=Ph;
			NotFoundPhNumber++;
			continue;
		}
		spp[n].m_FileNumber=FileNumber;
	}
	fclose(fp);

	// 列出未发现的文件号。
	ListNotFoundPh(NotFoundPh,NotFoundPhNumber,"NotFndFn.TXT");
	AfxMessageBox("文件号已经被添加进炮点参数文件中了!");
	delete NotFoundPh;

	// 返回。
	return TRUE;
}
//	名称：PlusFileNumber																	   结束
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  名称: PlusOffset																				 开始
//  功能:  将偏移距置入炮点参数框架中去.
//  输入:  spp,炮点参数框架起始地址.
//				ItemNumber, 框架的条目数目.
//				File ,偏移距文件.
//  改变:  spp.
//  日期:  1997.10.16
BOOL ShotPointParDealer::PlusOffset(ShotPointPar *spp,long ItemNumber,CString File)
{
	//  检查文件的合法性。
	if(File=="")File="SWATH"+IntToName(m_SwathNumber)+".OFF";
			
	FILE *fp=fopen(File,"r");
	if(!fp){
		AfxMessageBox("无法打开偏移距文件:"+File);
		return FALSE;
	}

	// 读入数据。
	long Ph,n,ZP,HP,NotFoundPhNumber=0;
	long *NotFoundPh=new long[4000];
	while(!feof(fp)){
		fscanf(fp,"%ld %ld %ld\n",&Ph,&ZP,&HP);
		n=SearchPhInSPP(Ph,spp,ItemNumber);
		if(n==-1&&NotFoundPhNumber<4000){
			NotFoundPh[NotFoundPhNumber]=Ph;
			NotFoundPhNumber++;
			continue;
		}
		spp[n].m_ZP=ZP;
		spp[n].m_HP=HP;
	}
	fclose(fp);

	// 列出未发现的桩号。
	ListNotFoundPh(NotFoundPh,NotFoundPhNumber,"NotFndOff.TXT");
	AfxMessageBox("偏移距已经被添加进炮点参数文件中了!");
	delete NotFoundPh;

	//返回。
	return TRUE;
}
//  名称: PlusOffset																				结束
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  名称: ListNotFoundPh																										   开始
//  功能: 将未发现的桩号列出, 若用户要存盘, 则存入文件.
//  输入: NotFoundPh, 未发现桩号的起始指针,
//			NotFoundPhNumber, 	未发现桩号的数目,
//			SaveFile, 存盘的文件名.
//  日期: 1997.10.2.
BOOL ShotPointParDealer::ListNotFoundPh(long * NotFoundPh, long NotFoundPhNumber,CString FileSave)
{
	// 若未发现桩号数目为零, 则返回.
	if(NotFoundPhNumber==0)return FALSE;

	// 对列表框赋值.
	long i;
	CMyList dlg;
	long n=NotFoundPhNumber;
	if(n>ListMaxNumber)n=ListMaxNumber;
	for(i=0;i<n;i++){
		dlg.Data[i]=NotFoundPh[i];
	}
	dlg.DataNumber=n;

	// 设置对话框的题头.
	dlg.CaptainText="桩号列表";
	dlg.ContentText="在炮点参数文件中未找到的桩号:";
	
	//显示对话框并将桩号存入文件.	
	if(dlg.DoModal()==IDOK){
		AfxMessageBox("这些数据将被存入文件:"+FileSave+" 中!");
		FILE *fp=fopen(FileSave,"w");
		if(!fp){
			AfxMessageBox("无法建立输出文件:"+FileSave+"!");
			return FALSE;
		}
		for(i=0;i<NotFoundPhNumber;i++){
			fprintf(fp,"%ld\n",NotFoundPh[i]);
		}
		fclose(fp);
	}

	// 返回.
	return TRUE;
}
//  名称: ListNotFoundPh																							  结束
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	SearchPhInSPP
long ShotPointParDealer::SearchPhInSPP(long WantedPh, ShotPointPar * spp, long sppNumber)
{
	for(int i=0;i<sppNumber;i++){
		if(spp[i].m_Ph==WantedPh)return i;
	}
	return -1;
}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	   Name        :  SortSwath
//		Function   :  Sort the Shot point parameters on m_Ph.
//      Input		  :	 ShotPointPar *spp , the Data group of ShotPointPar.
//		Change     :  The sequence of the pars. 						
//      Date          : 1997.3
//  Repro Date   : 1997.9.9
void  ShotPointParDealer::SortSwath(ShotPointPar *spp)
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	// Sort the Shot point par data on Ph.
	ShotPointPar  SwapBlock;
	long TempLength;
	long i,j;

	for(i=0;i<m_TotalSNumber;i++){
		SwapBlock=spp[i];
		TempLength=spp[i].m_Ph;		

		for(j=i;j>0;j--){
			if(spp[j-1].m_Ph>TempLength)spp[j]=spp[j-1];
			else break;
		}
		spp[j]=SwapBlock;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	// return.
	return ;
}
//  SortSwath().
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  名称: PlusCor																				 开始
//  功能:  将坐标置入炮点参数框架中去.
//  输入:  spp,炮点参数框架起始地址.
//				ItemNumber, 框架的条目数目.
//				File ,静校正量文件.
//  改变:  spp.
//  日期:  1997.10.16
BOOL ShotPointParDealer::PlusCor(ShotPointPar *spp,long ItemNumber,CString File)
{
	return TRUE;
}


bool ShotPointParDealer::SetFileName(int nSwath)
{
	m_FileShotPointPar="Swath"+IntToName(nSwath)+".SPP";
	return true;
}
