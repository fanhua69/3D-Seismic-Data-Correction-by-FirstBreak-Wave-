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
//	ע�⣺���������������ڵ������ֻ�ǿշ��ڵ��ļ�����Ϊ0��
//				�ڽ�������ⷽ��ʱ����������ǵ��շ��ڵ����⣬ר������
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
//	ע�⣺���������������ڵ������ֻ�ǿշ��ڵ��ļ�����Ϊ0��
//				�ڽ�������ⷽ��ʱ����������ǵ��շ��ڵ����⣬ר������
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
		AfxMessageBox("�ڵ�ƫ���ļ� "+m_fileShotPointPar+" û���ҵ���",MB_OK);
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
			AfxMessageBox("�ڵ�����ļ������޷����������飡");
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
//	ע�⣺���������������ڵ������ֻ�ǿշ��ڵ��ļ�����Ϊ0��
//				�ڽ�������ⷽ��ʱ����������ǵ��շ��ڵ����⣬ר������
//   
long CSPPDoc::GetShotNumber()
{                 
	return m_TotalSNumber;
}
//                ShotPointPar*  CSPPDoc::ReadSwath()									END
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   ����: WriteSwath
//   ����:���û��͹������ڵ���������ļ�.
//   ע��:�����ǿշ���,��������׮��ȫ��װ���ļ�,�շ������ļ��ű�ʶ.  
//            m_TotalSNumber ������ Read���.
//   ����:m_pSPP, �ڵ��������ʼ��ַ, 
//        m_TotalSNumber, �ڵ����������.
//   ����:�ɹ�true.
BOOL CSPPDoc::Write()
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  Open the shot point file .
	FILE *fp=fopen(m_fileShotPointPar,"w");
	if(!fp){
		AfxMessageBox("�޷������ڵ�����ļ� "+m_fileShotPointPar);
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
// ���ƣ�PlusPh																											��ʼ
// ���ܣ�������ʼ׮�š�
// ���ڣ�1997.10.16
BOOL CSPPDoc::PlusPh(int PhByProgram,CString sFile)
{
	// �û�Ҫ���ɳ�����������׮��.
	if(PhByProgram<=0){
		SwathParameter sp;
		if(!sp.Set(m_SwathNumber)){
			AfxMessageBox("CSPPDoc:If you want the program to make theorical station NO. , \n�������뱾���ߵĲ���,Ȼ�����γ��ڵ�����ļ�!");
			return false;
		}
		long *pPh=new long[m_nShotLimit];   // allocated.
		sp.MakeShotPh(pPh,&m_TotalSNumber);
		for(int i=0;i<m_TotalSNumber;i++){
			m_pSPP[i].m_Ph=pPh[i];
		}
		delete []pPh;
	}

	// �û�Ҫ��׮���ļ��еõ�׮��.
	else{
		if(sFile=="")sFile="SWATH"+IntToName(m_SwathNumber)+".PH";
		FILE *fp=fopen(sFile,"r");
		if(!fp){
			AfxMessageBox("�޷���׮���ļ�"+sFile+"!");
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

	AfxMessageBox("׮���Ѿ�����ӽ��ڵ��������!");
	return true;
}
//   ���ƣ�PlusPh 																													  ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	���ƣ�PlusFileNumber																									   ��ʼ
//  ���ܣ����ڵ�����Ŀ���ڣ� Ѱ���ļ��ŵ�׮�ţ�����׮�ŵ��ļ������롣
//	���룺spp���ڵ������ܵ���ʼλ�ã�
//				ItemNumber,�ڵ����ڵĲ�����Ŀ��
//				KFFile,�շ����ļ����ļ�����
//  �ı䣺spp.
//  ���أ��ɹ�true.
BOOL CSPPDoc::PlusFileNumber(CString sFile)
{
	// ����ļ����ĺϷ��ԡ�
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
		AfxMessageBox("�޷����ļ����ļ�:"+sFile);
		return false;
	}
	
	//  �������ݡ�
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

	// �г�δ���ֵ��ļ��š�
	ListNotFoundPh(NotFoundPh,NotFoundPhNumber,"NotFndFn.TXT");
	AfxMessageBox("�ļ����Ѿ�����ӽ��ڵ�����ļ�����!");
	delete []NotFoundPh;

	// ���ء�
	return true;
}
//	���ƣ�PlusFileNumber																	   ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ����: PlusOffset																				 ��ʼ
//  ����:  ��ƫ�ƾ������ڵ���������ȥ.
//  ����:  spp,�ڵ���������ʼ��ַ.
//				ItemNumber, ��ܵ���Ŀ��Ŀ.
//				File ,ƫ�ƾ��ļ�.
//  �ı�:  spp.
//  ����:  1997.10.16
BOOL CSPPDoc::PlusOffset(CString sFile)
{
	//  ����ļ��ĺϷ��ԡ�
	if(sFile=="")sFile="SWATH"+IntToName(m_SwathNumber)+".OFF";
			
	FILE *fp=fopen(sFile,"r");
	if(!fp){
		AfxMessageBox("�޷���ƫ�ƾ��ļ�:"+sFile);
		return false;
	}

	// �������ݡ�
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

	// �г�δ���ֵ�׮�š�
	ListNotFoundPh(NotFoundPh,NotFoundPhNumber,"NotFndOff.TXT");
	AfxMessageBox("ƫ�ƾ��Ѿ�����ӽ��ڵ�����ļ�����!");
	delete NotFoundPh;

	//���ء�
	return true;
}
//  ����: PlusOffset																				����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ����: ListNotFoundPh																										   ��ʼ
//  ����: ��δ���ֵ�׮���г�, ���û�Ҫ����, ������ļ�.
//  ����: NotFoundPh, δ����׮�ŵ���ʼָ��,
//			NotFoundPhNumber, 	δ����׮�ŵ���Ŀ,
//			SaveFile, ���̵��ļ���.
//  ����: 1997.10.2.
BOOL CSPPDoc::ListNotFoundPh(long * NotFoundPh, long NotFoundPhNumber,CString FileSave)
{
	// ��δ����׮����ĿΪ��, �򷵻�.
	if(NotFoundPhNumber==0)return false;

	// ���б��ֵ.
	long i;
	CMyList dlg;
	long n=NotFoundPhNumber;
	if(n>ListMaxNumber)n=ListMaxNumber;
	for(i=0;i<n;i++){
		dlg.Data[i]=NotFoundPh[i];
	}
	dlg.DataNumber=n;

	// ���öԻ������ͷ.
	dlg.CaptainText="׮���б�";
	dlg.ContentText="���ڵ�����ļ���δ�ҵ���׮��:";
	
	//��ʾ�Ի��򲢽�׮�Ŵ����ļ�.	
	if(dlg.DoModal()==IDOK){
		AfxMessageBox("��Щ���ݽ��������ļ�:"+FileSave+" ��!");
		FILE *fp=fopen(FileSave,"w");
		if(!fp){
			AfxMessageBox("�޷���������ļ�:"+FileSave+"!");
			return false;
		}
		for(i=0;i<NotFoundPhNumber;i++){
			fprintf(fp,"%ld\n",NotFoundPh[i]);
		}
		fclose(fp);
	}

	// ����.
	return true;
}
//  ����: ListNotFoundPh																							  ����
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
//  ����: Integrate.
//  ����: �ϲ����������ļ���Ϊһ�����ļ�:�ڵ�����ļ�.
//	ע��: ���������ļ������û��Լ��γ�.
//  ����: 1997.9.29.
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
	//   ���ڵ�����ļ��Ѿ����ڣ���ѯ���û��Ƿ������
	if(m_TotalSNumber>0)  
		if(AfxMessageBox("Do you want to discard the current data?",MB_YESNO)==IDNO)
			return;

	long ItemNumber=m_TotalSNumber;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ѯ���û���Ҫ�ϲ��Ĳ���.
	CSPPMaker dlg;
	if(dlg.DoModal()==IDCANCEL)return;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  ��Ӹ������ݡ�
	if(dlg.m_PlusPh)PlusPh(dlg.m_PhByProgram,dlg.m_PhFile);
	if(m_TotalSNumber==0){
		AfxMessageBox("��������׮��,���������������!");
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
	//  ����ļ��ĺϷ��ԡ�
	if(sFile=="")sFile="SWATH"+IntToName(m_SwathNumber)+".RN";
			
	FILE *fp=fopen(sFile,"r");
	if(!fp){
		AfxMessageBox("�޷��򿪽��յ��ļ�:"+sFile);
		return false;
	}

	// �������ݡ�
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

	// �г�δ���ֵ�׮�š�
	ListNotFoundPh(NotFoundPh,NotFoundPhNumber,"NotFndOff.TXT");
	AfxMessageBox("ƫ�ƾ��Ѿ�����ӽ��ڵ�����ļ�����!");
	delete NotFoundPh;

	//���ء�
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
		AfxMessageBox("�޷��򿪿շ���׮���ļ�"+sFile+"��");
		return false;
	}

	// ����׮�š�
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
	
	// �г�δ���ֵ�׮�š�
	ListNotFoundPh(NotFoundPh,NotFoundPhNumber,"NotFndKF.TXT");
	AfxMessageBox("�շ����Ѿ�����ӽ��ڵ�����ļ�����!");

	UpdateAllViews(NULL);
	
	//  ���ء�
	return true;

}

BOOL CSPPDoc::PlusCoor(CString sFile)
{
	// ����ļ����ĺϷ��ԡ�
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
	
	// �г�δ���ֵ��ļ��š�
	ListNotFoundPh(NotFoundPh,NotFoundPhNumber,"NotFndFn.TXT");
	AfxMessageBox("�����Ѿ�����ӽ��ڵ�����ļ�����!");
	delete []NotFoundPh;

	// ���ء�
	return true;
}
//	���ƣ�PlusFileNumber																	   ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
