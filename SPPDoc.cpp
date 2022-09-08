// SPPDoc.cpp : implementation file
//

#include "stdafx.h"
#include "3dfstbrk.h"
#include "spp.h"
#include "SPPDoc.h"
#include "struct.h"
#include "stdio.h"
#include "svsys.h" 
#include "swathpar.h"
#include "SPPMaker.h"
#include "Mylist.h"
#include "fh.h"
#include "DlgPlusCoor.h"
#include "InP190.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif		   


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	注意：所有物理点均计入炮点个数。只是空废炮的文件号设为0。
//				在建立、求解方程时，本软件考虑到空废炮的问题，专做处理。
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CSPPDoc

IMPLEMENT_DYNCREATE(CSPPDoc, CFHEditDoc)

CSPPDoc::CSPPDoc()
{
	m_SwathNumber=-1;
	m_fileShotPointPar=""; //"Swath"+IntToName(Swath)+".SPP";
	m_TotalSNumber=0;      //GetShotNumber();
	m_nShotLimit=10000;
	m_pSPP=new ShotPointPar[m_nShotLimit];
}

BOOL CSPPDoc::OnNewDocument()
{
	if (!CFHEditDoc::OnNewDocument())
		return false;
	m_SwathNumber=-1;
	m_TotalSNumber=0;

	return true;
}

CSPPDoc::~CSPPDoc()
{
	if(m_pSPP)delete m_pSPP;
}


BEGIN_MESSAGE_MAP(CSPPDoc, CFHEditDoc)
	//{{AFX_MSG_MAP(CSPPDoc)
	ON_COMMAND(ID_SPPIntegrate, OnSPPIntegrate)
	ON_COMMAND(ID_SPP_PlusFileNumber, OnSPPPlusFileNumber)
	ON_COMMAND(ID_SPP_PlusKF, OnSPPPlusKF)
	ON_COMMAND(ID_SPP_PlusCoor, OnSPPPlusCoor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSPPDoc diagnostics

#ifdef _DEBUG
void CSPPDoc::AssertValid() const
{
	CFHEditDoc::AssertValid();
}

void CSPPDoc::Dump(CDumpContext& dc) const
{
	CFHEditDoc::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSPPDoc serialization

void CSPPDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		CString sLine;
		for(int i=0;i<m_TotalSNumber;i++){
			sprintf(sLine.GetBuffer(255),"%ld %ld %ld %ld %ld %ld %ld %ld %9.1f %10.1f\r\n",
			m_pSPP[i].m_Ph,
			m_pSPP[i].m_FileNumber,
			m_pSPP[i].m_ZP,
			m_pSPP[i].m_HP,
			m_pSPP[i].m_BeginTrace,
			m_pSPP[i].m_EndTrace,
			m_pSPP[i].m_BeginGapTrace,
			m_pSPP[i].m_EndGapTrace,
			m_pSPP[i].m_XZB,
			m_pSPP[i].m_YZB);
			sLine.ReleaseBuffer();
			ar.WriteString(sLine);
		}
	}
	else
	{
		// TODO: add loading code here
		CString fileSelected=ar.GetFile()->GetFileName();
		fileSelected.MakeUpper();
		Set(fileSelected);		
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSPPDoc commands
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//          Name : ReadSwath
//		Function : Read Shot point Parameter of one swath , and Sort them on m_Ph.
//		       Date: 1997.3
//  Repro Date: 1997.9.9
//  Programer :  FAN HUA
//	注意：所有物理点均计入炮点个数。只是空废炮的文件号设为0。
//				在建立、求解方程时，本软件考虑到空废炮的问题，专做处理。
BOOL CSPPDoc::Read()
{   
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Open the Shot Point Parameter file.
	if(m_fileShotPointPar==""){
		AfxMessageBox("The file name cannot be empty!");
		return false;
	}
	FILE *fp=fopen(m_fileShotPointPar,"r");
	if(!fp){
		AfxMessageBox("炮点偏移文件 "+m_fileShotPointPar+" 没有找到！",MB_OK);
		return false;   
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	//   Loop to read the shot point parameter . 
	long n=0;
	while(!feof(fp)){
		fscanf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %lf %lf\n",
			&m_pSPP[n].m_Ph,
			&m_pSPP[n].m_FileNumber,
			&m_pSPP[n].m_ZP,
			&m_pSPP[n].m_HP,
			&m_pSPP[n].m_BeginTrace,
			&m_pSPP[n].m_EndTrace,
			&m_pSPP[n].m_BeginGapTrace,
			&m_pSPP[n].m_EndGapTrace,
			&m_pSPP[n].m_XZB,
			&m_pSPP[n].m_YZB);

		if(m_pSPP[n].m_Ph>9999999||m_pSPP[n].m_Ph<0) {
			AfxMessageBox("炮点参数文件错误，无法继续，请检查！");
			return false;
		}
		
		n++;
	}
	fclose(fp);

	m_TotalSNumber=n;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Sort the pars on m_Ph	
	SortSwath();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// return .
	return true;
}
//                ShotPointPar*  CSPPDoc::ReadSwath()									END
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
long CSPPDoc::GetShotNumber()
{                 
	return m_TotalSNumber;
}
//                ShotPointPar*  CSPPDoc::ReadSwath()									END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   名称: WriteSwath
//   功能:把用户送过来的炮点参数存入文件.
//   注释:不考虑空废炮,即把所有桩号全部装入文件,空废炮由文件号标识.  
//            m_TotalSNumber 变量由 Read获得.
//   输入:m_pSPP, 炮点参数的起始地址, 
//        m_TotalSNumber, 炮点参数的总数.
//   返回:成功true.
BOOL CSPPDoc::Write()
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  Open the shot point file .
	FILE *fp=fopen(m_fileShotPointPar,"w");
	if(!fp){
		AfxMessageBox("无法建立炮点参数文件 "+m_fileShotPointPar);
		return false;
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Write the data into the file.
	for(int i=0;i<m_TotalSNumber;i++){
		fprintf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %9.1f %10.1f\n",
			m_pSPP[i].m_Ph,
			m_pSPP[i].m_FileNumber,
			m_pSPP[i].m_ZP,
			m_pSPP[i].m_HP,
			m_pSPP[i].m_BeginTrace,
			m_pSPP[i].m_EndTrace,
			m_pSPP[i].m_BeginGapTrace,
			m_pSPP[i].m_EndGapTrace,
			m_pSPP[i].m_XZB,
			m_pSPP[i].m_YZB);
	}
	fclose(fp);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// return.
	return true;
}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 名称：PlusPh																											开始
// 功能：建立起始桩号。
// 日期：1997.10.16
BOOL CSPPDoc::PlusPh(int PhByProgram,CString sFile)
{
	// 用户要求由程序生成理论桩号.
	if(PhByProgram<=0){
		SwathParameter sp;
		if(!sp.Set(m_SwathNumber)){
			AfxMessageBox("CSPPDoc:If you want the program to make theorical station NO. , \n请先输入本束线的参数,然后再形成炮点参数文件!");
			return false;
		}
		long *pPh=new long[m_nShotLimit];   // allocated.
		sp.MakeShotPh(pPh,&m_TotalSNumber);
		for(int i=0;i<m_TotalSNumber;i++){
			m_pSPP[i].m_Ph=pPh[i];
		}
		delete []pPh;
	}

	// 用户要从桩号文件中得到桩号.
	else{
		if(sFile=="")sFile="SWATH"+IntToName(m_SwathNumber)+".PH";
		FILE *fp=fopen(sFile,"r");
		if(!fp){
			AfxMessageBox("无法打开桩号文件"+sFile+"!");
			return false;
		}

		long i=0;
		while(!feof(fp)){
			fscanf(fp,"%ld\n",&m_pSPP[i].m_Ph);
			i++;
		}
		m_TotalSNumber=i;
		fclose(fp);
	}

	AfxMessageBox("桩号已经被添加进炮点参数中了!");
	return true;
}
//   名称：PlusPh 																													  结束
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	名称：PlusFileNumber																									   开始
//  功能：在炮点参数的框架内， 寻找文件号的桩号，将该桩号的文件号置入。
//	输入：spp，炮点参数框架的起始位置，
//				ItemNumber,炮点框架内的参数数目。
//				KFFile,空废炮文件的文件名。
//  改变：spp.
//  返回：成功true.
BOOL CSPPDoc::PlusFileNumber(CString sFile)
{
	// 检查文件名的合法性。
	if(sFile==""){
		CFileDialog dlg(true);
		dlg.m_ofn.lpstrFilter="The skipped shot file name(*.txt)\0*.txt";
		dlg.m_ofn.lpstrTitle="The skipped shot file name";
		dlg.m_ofn.lpstrDefExt=".txt";
		
		if(dlg.DoModal()==IDCANCEL)return false;
		sFile=dlg.GetPathName();
	}		
	
	FILE *fp=fopen(sFile,"r");
	if(!fp){
		AfxMessageBox("无法打开文件号文件:"+sFile);
		return false;
	}
	
	//  读入数据。
	long NotFoundPhNumber=0,Ph,n,FileNumber;		
	long *NotFoundPh=new long[m_nShotLimit];
	while(!feof(fp)){
		fscanf(fp,"%ld %ld\n",&Ph,&FileNumber);
		n=SearchPhInSPP(Ph);
		if(n==-1&&NotFoundPhNumber<m_nShotLimit){
			NotFoundPh[NotFoundPhNumber]=Ph;
			NotFoundPhNumber++;
			continue;
		}
		m_pSPP[n].m_FileNumber=FileNumber;
	}
	fclose(fp);

	// 列出未发现的文件号。
	ListNotFoundPh(NotFoundPh,NotFoundPhNumber,"NotFndFn.TXT");
	AfxMessageBox("文件号已经被添加进炮点参数文件中了!");
	delete []NotFoundPh;

	// 返回。
	return true;
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
BOOL CSPPDoc::PlusOffset(CString sFile)
{
	//  检查文件的合法性。
	if(sFile=="")sFile="SWATH"+IntToName(m_SwathNumber)+".OFF";
			
	FILE *fp=fopen(sFile,"r");
	if(!fp){
		AfxMessageBox("无法打开偏移距文件:"+sFile);
		return false;
	}

	// 读入数据。
	long Ph,n,ZP,HP,NotFoundPhNumber=0;
	long *NotFoundPh=new long[4000];
	while(!feof(fp)){
		fscanf(fp,"%ld %ld %ld\n",&Ph,&ZP,&HP);
		n=SearchPhInSPP(Ph);
		if(n==-1&&NotFoundPhNumber<4000){
			NotFoundPh[NotFoundPhNumber]=Ph;
			NotFoundPhNumber++;
			continue;
		}
		m_pSPP[n].m_ZP=ZP;
		m_pSPP[n].m_HP=HP;
	}
	fclose(fp);

	// 列出未发现的桩号。
	ListNotFoundPh(NotFoundPh,NotFoundPhNumber,"NotFndOff.TXT");
	AfxMessageBox("偏移距已经被添加进炮点参数文件中了!");
	delete NotFoundPh;

	//返回。
	return true;
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
BOOL CSPPDoc::ListNotFoundPh(long * NotFoundPh, long NotFoundPhNumber,CString FileSave)
{
	// 若未发现桩号数目为零, 则返回.
	if(NotFoundPhNumber==0)return false;

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
			return false;
		}
		for(i=0;i<NotFoundPhNumber;i++){
			fprintf(fp,"%ld\n",NotFoundPh[i]);
		}
		fclose(fp);
	}

	// 返回.
	return true;
}
//  名称: ListNotFoundPh																							  结束
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	SearchPhInSPP
long CSPPDoc::SearchPhInSPP(long WantedPh)
{
	for(int i=0;i<m_TotalSNumber;i++){
		if(m_pSPP[i].m_Ph==WantedPh)return i;
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
void  CSPPDoc::SortSwath()
{
	////////////////////////////////////////////////////////////////////////////////////////////////
	// Sort the Shot point par data on Ph.
	ShotPointPar  SwapBlock;
	long TempLength;
	long i,j;

	for(i=0;i<m_TotalSNumber;i++){
		SwapBlock=m_pSPP[i];
		TempLength=m_pSPP[i].m_Ph;		

		for(j=i;j>0;j--){
			if(m_pSPP[j-1].m_Ph>TempLength)m_pSPP[j]=m_pSPP[j-1];
			else break;
		}
		m_pSPP[j]=SwapBlock;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	// return.
	return ;
}
//  SortSwath().
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSPPDoc::Set(int nSwath)
{
	m_fileShotPointPar="Swath"+IntToName(nSwath)+".SPP";
	m_fileShotPointPar.MakeUpper();
	m_SwathNumber=nSwath;
	return true;
}

BOOL CSPPDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	FILE *fp=fopen(lpszPathName,"wt");
	if(!fp)return false;

	for(int i=0;i<m_TotalSNumber;i++){  
		fprintf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %lf %lf\n",
			m_pSPP[i].m_Ph,
			m_pSPP[i].m_FileNumber,
			m_pSPP[i].m_ZP,
			m_pSPP[i].m_HP,
			m_pSPP[i].m_BeginTrace,
			m_pSPP[i].m_EndTrace,
			m_pSPP[i].m_BeginGapTrace,
			m_pSPP[i].m_EndGapTrace,
			m_pSPP[i].m_XZB,
			m_pSPP[i].m_YZB);
	}

	fclose(fp);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  名称: Integrate.
//  功能: 合并各个单个文件成为一个总文件:炮点参数文件.
//	注意: 其它单个文件须由用户自己形成.
//  日期: 1997.9.29.
void CSPPDoc::OnSPPIntegrate() 
{
	// When the user opened a new document , then ask the swath number:
	if(m_SwathNumber==-1){
		CDSwNum dlg;
		if(!dlg.DoModal())return;
		m_SwathNumber=dlg.m_SwathNumber;
		SwathParameter sp;
		if(!sp.Set(m_SwathNumber)){
			AfxMessageBox("You must firstly input the swath parameter!");
			m_SwathNumber=-1;
			return;
		}
		Set(m_SwathNumber);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//   若炮点参数文件已经存在，则询问用户是否继续。
	if(m_TotalSNumber>0)  
		if(AfxMessageBox("Do you want to discard the current data?",MB_YESNO)==IDNO)
			return;

	long ItemNumber=m_TotalSNumber;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 询问用户所要合并的参数.
	CSPPMaker dlg;
	if(dlg.DoModal()==IDCANCEL)return;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  添加各种数据。
	if(dlg.m_PlusPh)PlusPh(dlg.m_PhByProgram,dlg.m_PhFile);
	if(m_TotalSNumber==0){
		AfxMessageBox("必须先有桩号,才能添加其它数据!");
		return;
	}

	if(dlg.m_PlusFileNumber)PlusFileNumber(dlg.m_FileNumberFile);
	if(dlg.m_PlusKF)PlusKF(dlg.m_KFFile);
	if(dlg.m_PlusOffset)PlusOffset(dlg.m_OffsetFile);
	if(dlg.m_PlusTrace)PlusTrace(dlg.m_TraceFile);
	if(dlg.m_PlusCor)PlusCoor(dlg.m_StaticDataFile);

	UpdateAllViews(NULL);

	return;
}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CSPPDoc::Set(CString fileSelected)
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//    Calculate TotalRSNumber.
	// Find which swath is the selected file belong to:
	for(int i=0;i<100;i++){
		Set(i);
		if(m_fileShotPointPar==fileSelected){
			m_SwathNumber=i;
			Set(i);
			return true;
		}
	}
	m_fileShotPointPar=fileSelected;
	m_fileShotPointPar.MakeUpper();
	m_SwathNumber=-1;
	return false;
}

BOOL CSPPDoc::PlusTrace(CString sFile)
{
	//  检查文件的合法性。
	if(sFile=="")sFile="SWATH"+IntToName(m_SwathNumber)+".RN";
			
	FILE *fp=fopen(sFile,"r");
	if(!fp){
		AfxMessageBox("无法打开接收道文件:"+sFile);
		return false;
	}

	// 读入数据。
	long Ph,n,R1,R2,R3,R4,NotFoundPhNumber=0;
	long *NotFoundPh=new long[4000];
	while(!feof(fp)){
		fscanf(fp,"%ld %ld %ld %ld %ld\n",&Ph,&R1,&R2,&R3,&R4);
		n=SearchPhInSPP(Ph);
		if(n==-1&&NotFoundPhNumber<4000){
			NotFoundPh[NotFoundPhNumber]=Ph;
			NotFoundPhNumber++;
			continue;
		}
		m_pSPP[n].m_BeginTrace=R1;
		m_pSPP[n].m_EndTrace=R2;
		m_pSPP[n].m_BeginGapTrace=R3;
		m_pSPP[n].m_EndGapTrace=R4;
	}
	fclose(fp);

	// 列出未发现的桩号。
	ListNotFoundPh(NotFoundPh,NotFoundPhNumber,"NotFndOff.TXT");
	AfxMessageBox("偏移距已经被添加进炮点参数文件中了!");
	delete NotFoundPh;

	//返回。
	return true;
}

BOOL CSPPDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CFHEditDoc::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	FILE *fp=fopen(lpszPathName,"rt");
	if(!fp)return false;

	int i=0;
	while(!feof(fp)){
		fscanf(fp,"%ld %ld %ld %ld %ld %ld %ld %ld %lf %lf",
			&m_pSPP[i].m_Ph,
			&m_pSPP[i].m_FileNumber,
			&m_pSPP[i].m_ZP,
			&m_pSPP[i].m_HP,
			&m_pSPP[i].m_BeginTrace,
			&m_pSPP[i].m_EndTrace,
			&m_pSPP[i].m_BeginGapTrace,
			&m_pSPP[i].m_EndGapTrace,
			&m_pSPP[i].m_XZB,
			&m_pSPP[i].m_YZB);
		i++;
	}

	m_TotalSNumber=i-1;

	fclose(fp);

	return true;
	
}

void CSPPDoc::OnSPPPlusFileNumber() 
{
	// TODO: Add your command handler code here
	PlusFileNumber();
	
}

void CSPPDoc::OnSPPPlusKF() 
{
	// TODO: Add your command handler code here
	PlusKF();	
}

void CSPPDoc::OnSPPPlusCoor() 
{
	// TODO: Add your command handler code here
	PlusCoor();
}


bool CSPPDoc::PlusKF(CString sFile)
{
	if(sFile==""){
		CFileDialog dlg(true);
		dlg.m_ofn.lpstrFilter="The skipped shot file name(*.txt)\0*.txt";
		dlg.m_ofn.lpstrTitle="The skipped shot file name";
		dlg.m_ofn.lpstrDefExt=".txt";
		
		if(dlg.DoModal()==IDCANCEL)return false;
		sFile=dlg.GetPathName();
	}
		
	FILE *fp=fopen(sFile,"rt");
	if(!fp){
		AfxMessageBox("无法打开空废炮桩号文件"+sFile+"！");
		return false;
	}

	// 读入桩号。
	long Ph,n,NotFoundPhNumber=0;
	long NotFoundPh[4000];
	while(!feof(fp)){
		fscanf(fp,"%ld\n",&Ph);
		n=SearchPhInSPP(Ph);
		if(n==-1&&NotFoundPhNumber<4000){
			NotFoundPh[NotFoundPhNumber]=Ph;
			NotFoundPhNumber++;
			continue;
		}
		m_pSPP[n].m_FileNumber=-1;
	}
	
	// 列出未发现的桩号。
	ListNotFoundPh(NotFoundPh,NotFoundPhNumber,"NotFndKF.TXT");
	AfxMessageBox("空废炮已经被添加进炮点参数文件中了!");

	UpdateAllViews(NULL);
	
	//  返回。
	return true;

}

BOOL CSPPDoc::PlusCoor(CString sFile)
{
	// 检查文件名的合法性。
	if(sFile==""){
		CDlgPlusCoor  dlg;
		if(dlg.DoModal()==IDCANCEL)return false;
		
		if(dlg.m_nReadP190 ==1){
			CFileDialog dlg2(true);
			dlg2.m_ofn.lpstrFilter="The P190 file name(*.SRS)\0*.SRS";
			dlg2.m_ofn.lpstrTitle="The skipped shot file name";
			dlg2.m_ofn.lpstrDefExt=".SRS";
			
			if(dlg2.DoModal()==IDCANCEL)return false;
			sFile=dlg2.GetPathName();
		}
	}	
	

	//	
	long NotFoundPhNumber=0;
	long *NotFoundPh=new long[m_nShotLimit];

	if(sFile!=""){
		SwathParameter sp;
		for(int i=0;i<m_TotalSNumber;i++){
			CMyPoint point=sp.GetShotPos (m_pSPP[i].m_Ph,m_pSPP[i].m_ZP,m_pSPP[i].m_HP);
			if(point.x==-1){
				NotFoundPh[NotFoundPhNumber]=m_pSPP[i].m_Ph;
				NotFoundPhNumber++;
			}
			else{
				m_pSPP[i].m_XZB=point.y;
				m_pSPP[i].m_YZB=point.x;
			}
		}
	}
	else {
		CInP190 p190;
		if(!p190.Open (sFile))return false;
		for(int i=0;i<m_TotalSNumber;i++){
			CMyPoint point=p190.GetShotPos (m_pSPP[i].m_Ph);
			if(point.x==-1){
				NotFoundPh[NotFoundPhNumber]=m_pSPP[i].m_Ph;
				NotFoundPhNumber++;
			}
			else{
				m_pSPP[i].m_XZB=point.x;
				m_pSPP[i].m_YZB=point.y;
			}
		}
	}
	
	// 列出未发现的文件号。
	ListNotFoundPh(NotFoundPh,NotFoundPhNumber,"NotFndFn.TXT");
	AfxMessageBox("坐标已经被添加进炮点参数文件中了!");
	delete []NotFoundPh;

	// 返回。
	return true;
}
//	名称：PlusFileNumber																	   结束
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
