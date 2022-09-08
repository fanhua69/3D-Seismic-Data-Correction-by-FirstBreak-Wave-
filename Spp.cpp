
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	ע�⣺���������������ڵ������ֻ�ǿշ��ڵ��ļ�����Ϊ0��
//				�ڽ�������ⷽ��ʱ����������ǵ��շ��ڵ����⣬ר������
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
	
	//����.
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
//	ע�⣺���������������ڵ������ֻ�ǿշ��ڵ��ļ�����Ϊ0��
//				�ڽ�������ⷽ��ʱ����������ǵ��շ��ڵ����⣬ר������
BOOL ShotPointParDealer::ReadSwath(ShotPointPar *spp)
{                 
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Open the Shot Point Parameter file.
	FILE *fp=fopen(m_FileShotPointPar,"r");
	if(!fp){
		AfxMessageBox("�ڵ�ƫ���ļ� "+m_FileShotPointPar+" û���ҵ���",MB_OK);
		return NULL;   //�ú���Ӧ����һ��ShotPointPar��ַ.
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
			AfxMessageBox("�ڵ�����ļ������޷����������飡");
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
//	ע�⣺���������������ڵ������ֻ�ǿշ��ڵ��ļ�����Ϊ0��
//				�ڽ�������ⷽ��ʱ����������ǵ��շ��ڵ����⣬ר������
//   
long ShotPointParDealer::GetShotNumber()
{                 
	////////////////////////////////////////////////////////////////////////////////////////////////////
	// Open the Shot Point Parameter file.
	FILE *fp=fopen(m_FileShotPointPar,"r");			 
	if(!fp){
		AfxMessageBox("�ڵ�ƫ���ļ� "+m_FileShotPointPar+" û���ҵ���",MB_OK);
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
			AfxMessageBox("�ڵ�����ļ������޷����������飡");
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
//   ����: WriteSwath
//   ����:���û��͹������ڵ���������ļ�.
//   ע��:�����ǿշ���,��������׮��ȫ��װ���ļ�,�շ������ļ��ű�ʶ.  
//            m_TotalSNumber ������ ReadSwath ���.
//   ����:ShotPointPar * spp, �ڵ��������ʼ��ַ, 
//             long TotalPointNumber, �ڵ����������.
//   ����:�ɹ�TRUE.
BOOL ShotPointParDealer::WriteSwath(ShotPointPar* spp,long TotalPointNumber)
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  Open the shot point file .
	FILE *fp=fopen(m_FileShotPointPar,"w");
	if(!fp){
		AfxMessageBox("�޷������ڵ�����ļ� "+m_FileShotPointPar);
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
//  ����: Integrate.
//  ����: �ϲ����������ļ���Ϊһ�����ļ�:�ڵ�����ļ�.
//	ע��: ���������ļ������û��Լ��γ�.
//  ����: 1997.9.29.
BOOL ShotPointParDealer::MakeFileFromOther()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//   ���ڵ�����ļ��Ѿ����ڣ���ѯ���û��Ƿ������
	if(m_DataOK)  // ���� *this ʱ, �Ѿ�������ReadSwath����.
		if(AfxMessageBox("�����ߵ��ڵ�����ļ��Ѿ�����,�Ƿ����?",MB_YESNO)==IDNO)
			return FALSE;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  �����ڵ������ܡ�
	ShotPointPar *spp=new ShotPointPar[5000];
	for(int i=0;i<5000;i++)spp[i].Reset();

	ReadSwath(spp);
	long ItemNumber=m_TotalSNumber;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ѯ���û���Ҫ�ϲ��Ĳ���.
	CSPPMaker dlg;
	if(dlg.DoModal()==IDCANCEL)return FALSE;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  ��Ӹ������ݡ�
	if(dlg.m_PlusPh)PlusPh(spp,&ItemNumber,dlg.m_PhByProgram,dlg.m_PhFile);
	if(ItemNumber==0){
		AfxMessageBox("��������׮��,���������������!");
		return FALSE;
	}

	if(dlg.m_PlusFileNumber)PlusFileNumber(spp,ItemNumber,dlg.m_FileNumberFile);
	if(dlg.m_PlusKF)PlusKF(spp,ItemNumber,dlg.m_KFFile);
	if(dlg.m_PlusOffset)PlusOffset(spp,ItemNumber,dlg.m_OffsetFile);
	if(dlg.m_PlusCor)PlusCor(spp,ItemNumber,dlg.m_StaticDataFile);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//	�洢����.
	if(!WriteSwath(spp,ItemNumber)){
		AfxMessageBox("�ڴ洢�ڵ�����ļ�ʱ����!");	
		return FALSE;
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����.
	AfxMessageBox("�ڵ�����ļ��洢���!");	
	return TRUE;
}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ���ƣ�PlusPh																											��ʼ
//	 ���ܣ�������ʼ׮�š�
//  ���ڣ�1997.10.16
BOOL ShotPointParDealer::PlusPh(ShotPointPar *spp,long *ItemNumber,int PhByProgram,CString File)
{
		// �û�Ҫ���ɳ�����������׮��.
		if(PhByProgram<=0){
			long *Ph=new long[5000];   // allocated.
			SwathParameter sp(m_SwathNumber);
			if(!sp.DataOK){
				AfxMessageBox("�������뱾���ߵĲ���,Ȼ�����γ��ڵ�����ļ�!");
				return FALSE;
			}
			sp.MakeShotPh(Ph,ItemNumber);
			for(int i=0;i<*ItemNumber;i++){
				spp[i].m_Ph=Ph[i];
			}
			delete Ph;							   // deleted	.
		}

		// �û�Ҫ��׮���ļ��еõ�׮��.
		else{
			if(File=="")File="SWATH"+IntToName(m_SwathNumber)+".PH";
			FILE *fp=fopen(File,"r");
			if(!fp){
				AfxMessageBox("�޷���׮���ļ�"+File+"!");
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

		AfxMessageBox("׮���Ѿ�����ӽ��ڵ��������!");
		return TRUE;
}
//   ���ƣ�PlusPh 																													  ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	���ƣ�PlusKF																													   ��ʼ
//  ���ܣ����ڵ�����Ŀ���ڣ� Ѱ�ҿշ��ڵ�׮�ţ����׮�ŵ��ļ���Ϊ�㡣
//	���룺spp���ڵ������ܵ���ʼλ�ã�
//				ItemNumber,�ڵ����ڵĲ�����Ŀ��
//				File,�շ����ļ����ļ�����
//  �ı䣺spp.
//  ���أ��ɹ�TRUE.
BOOL ShotPointParDealer::PlusKF(ShotPointPar *spp,long ItemNumber,CString File)
{
	//  ����ļ����Ƿ�Ϸ���
	if(File=="")File="SWATH"+IntToName(m_SwathNumber)+".KF";
			
	FILE *fp=fopen(File,"r");
	if(!fp){
		AfxMessageBox("�޷��򿪿շ���׮���ļ�"+File+"��");
		return FALSE;
	}

	// ����׮�š�
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
	
	// �г�δ���ֵ�׮�š�
	ListNotFoundPh(NotFoundPh,NotFoundPhNumber,"NotFndKF.TXT");
	AfxMessageBox("�շ����Ѿ�����ӽ��ڵ�����ļ�����!");
	delete NotFoundPh;
	
	//  ���ء�
	return TRUE;
}
//	���ƣ�PlusKF																													   ����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	���ƣ�PlusFileNumber																									   ��ʼ
//  ���ܣ����ڵ�����Ŀ���ڣ� Ѱ���ļ��ŵ�׮�ţ�����׮�ŵ��ļ������롣
//	���룺spp���ڵ������ܵ���ʼλ�ã�
//				ItemNumber,�ڵ����ڵĲ�����Ŀ��
//				KFFile,�շ����ļ����ļ�����
//  �ı䣺spp.
//  ���أ��ɹ�TRUE.
BOOL ShotPointParDealer::PlusFileNumber(ShotPointPar *spp,long ItemNumber,CString File)
{
	// ����ļ����ĺϷ��ԡ�
	if(File=="")File="SWATH"+IntToName(m_SwathNumber)+".FN";
	
	FILE *fp=fopen(File,"r");
	if(!fp){
		AfxMessageBox("�޷����ļ����ļ�:"+File);
		return FALSE;
	}
	
	//  �������ݡ�
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

	// �г�δ���ֵ��ļ��š�
	ListNotFoundPh(NotFoundPh,NotFoundPhNumber,"NotFndFn.TXT");
	AfxMessageBox("�ļ����Ѿ�����ӽ��ڵ�����ļ�����!");
	delete NotFoundPh;

	// ���ء�
	return TRUE;
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
BOOL ShotPointParDealer::PlusOffset(ShotPointPar *spp,long ItemNumber,CString File)
{
	//  ����ļ��ĺϷ��ԡ�
	if(File=="")File="SWATH"+IntToName(m_SwathNumber)+".OFF";
			
	FILE *fp=fopen(File,"r");
	if(!fp){
		AfxMessageBox("�޷���ƫ�ƾ��ļ�:"+File);
		return FALSE;
	}

	// �������ݡ�
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

	// �г�δ���ֵ�׮�š�
	ListNotFoundPh(NotFoundPh,NotFoundPhNumber,"NotFndOff.TXT");
	AfxMessageBox("ƫ�ƾ��Ѿ�����ӽ��ڵ�����ļ�����!");
	delete NotFoundPh;

	//���ء�
	return TRUE;
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
BOOL ShotPointParDealer::ListNotFoundPh(long * NotFoundPh, long NotFoundPhNumber,CString FileSave)
{
	// ��δ����׮����ĿΪ��, �򷵻�.
	if(NotFoundPhNumber==0)return FALSE;

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
			return FALSE;
		}
		for(i=0;i<NotFoundPhNumber;i++){
			fprintf(fp,"%ld\n",NotFoundPh[i]);
		}
		fclose(fp);
	}

	// ����.
	return TRUE;
}
//  ����: ListNotFoundPh																							  ����
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
//  ����: PlusCor																				 ��ʼ
//  ����:  �����������ڵ���������ȥ.
//  ����:  spp,�ڵ���������ʼ��ַ.
//				ItemNumber, ��ܵ���Ŀ��Ŀ.
//				File ,��У�����ļ�.
//  �ı�:  spp.
//  ����:  1997.10.16
BOOL ShotPointParDealer::PlusCor(ShotPointPar *spp,long ItemNumber,CString File)
{
	return TRUE;
}


bool ShotPointParDealer::SetFileName(int nSwath)
{
	m_FileShotPointPar="Swath"+IntToName(nSwath)+".SPP";
	return true;
}
