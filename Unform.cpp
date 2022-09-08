
#include "stdafx.h"
#include "3dfstbrk.h"
#include "shotmsg.h"
#include "Unform.h"
#include "ProgDlg.h"
#include "stdio.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ���캯��, ��������
UnformFile::UnformFile()
{
	m_UnDataOK=FALSE;
	m_pShotMsg=NULL;
	m_fpUnform=NULL;
	m_FileUnform="";
	m_TopFileNumber=0;
	m_EndFileNumber=0;
	m_UnformGroupNumber=0;  //���ļ��й��ж��ٵ�.
	m_UnformGroupHead=0;     // =128 in format 4
	m_FileNumberPosition=0;  //=220	in format 4
	m_nGroupNamePosition=0;
	m_UnformFileLength=0;
	m_UnformTimeInterval=0;
	m_UnformTimeLength=0;
	m_ByteNumberOfOneGroup=0;
	m_PointNumberOfOneGroup=0;
	m_DataNumberOfOneGroup=0;
	m_MaxGroupNumber=0;
	m_TotalShotNumber=0;
}

BOOL UnformFile::Set(CString FileName)
{

	m_UnDataOK=FALSE;

	if(FileName==""){
		AfxMessageBox("File Name is NULL!");
		return  false;
	}

	m_fpUnform=fopen(FileName,"rb");	// ���ļ�, �������Ϸ���.
	if(!m_fpUnform){
		AfxMessageBox("�ý���ļ������ڣ�");
		return FALSE;
	}
	
	m_FileNumberPosition=4 ; //220;  // ���������ʽ(format 4)���ļ���λ��(��һ��������).
	m_UnformGroupHead=128;  // ���������ʽ��ͷ�����ݸ���.
	m_nGroupNamePosition=16;//
	
	fseek(m_fpUnform,32,SEEK_SET);
	fread(&m_UnformTimeInterval,sizeof(long),1,m_fpUnform);    // ��ȡ�������
	fread(&m_UnformTimeLength,sizeof(long),1,m_fpUnform);  //��ȡ��೤��

	if(m_UnformTimeInterval>16||m_UnformTimeInterval<2
		||m_UnformTimeLength<0||m_UnformTimeLength>20000){
		AfxMessageBox(FileName+"�ļ���ʽ����,�����ֻ�ܴ���GRISYS��Format 4��ʽ!");
		return FALSE;
	}

	fseek(m_fpUnform,m_FileNumberPosition,SEEK_SET);
	fread(&m_TopFileNumber,sizeof(long),1,m_fpUnform);           //���ļ���
	
	fseek(m_fpUnform,0,SEEK_END);
	m_UnformFileLength=ftell(m_fpUnform);

	m_PointNumberOfOneGroup=m_UnformTimeLength/m_UnformTimeInterval; //ÿ���а��������ݸ���.
	m_DataNumberOfOneGroup=m_PointNumberOfOneGroup+m_UnformGroupHead; // ÿ�������ݵ���ϵ�ͷ�����ݸ���.
	m_ByteNumberOfOneGroup=(m_UnformGroupHead+m_PointNumberOfOneGroup)*4;
	m_UnformGroupNumber=m_UnformFileLength/m_ByteNumberOfOneGroup; //�ܹ�����˶��ٵ�.

	fseek(m_fpUnform,-(m_ByteNumberOfOneGroup-m_FileNumberPosition),SEEK_END);
	fread(&m_EndFileNumber,sizeof(long),1,m_fpUnform);    //���һ���ļ���.

	if(m_pShotMsg==NULL)m_pShotMsg=new ShotMsg[5000];  //����ָ����δ�����ڴ棬����䡣
	SetShotMsg();

	
	return TRUE;	
}

UnformFile::~UnformFile()
{
	if(m_UnDataOK)fclose(m_fpUnform);
	if(m_pShotMsg)delete m_pShotMsg;
}
//	 ���캯��.																									  ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	����:GetFileNumber(long Group)
//  ����:�����û������������ļ���
//	˵��:���ǵ��ļ���ʽ�Ĳ�ͬ, ��������ļ��йصĲ����ú�����ʾ.
//  ����:1997.10.4
long UnformFile::GetFileNumber(long Group)
{
	// �����ļ��ŵ�λ��.
	long P=Group*(m_UnformGroupHead+m_PointNumberOfOneGroup)*4
		+m_FileNumberPosition;  // File Number Position : 220.
	
	// ����λ��Υ��.
	if(P<0)return -2;
	if(P>m_UnformFileLength)return -1;

	// ��λ�ö�ȡ�ļ���.
	long FileNumber;
	fseek(m_fpUnform,P,SEEK_SET);
	fread(&FileNumber,sizeof(long),1,m_fpUnform);

	// ���ػ�õ��ļ���.
	return FileNumber;
}
//	����:GetFileNumber(long Group)															   ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	����:GetGroupNumber(long Group)															 ��ʼ
//  ����:�����û����������ݵĵ���
//	˵��:���ǵ��ļ���ʽ�Ĳ�ͬ, ��������ļ��йصĲ����ú�����ʾ.
//  ����:1997.10.14
long UnformFile::GetFileNumber(float* GroupData)
{
	long *n=(long*)&GroupData[m_FileNumberPosition/4];
	return *n;
}
//	����:GetGroupNumber(long Group)															 ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	����:GetGroupNumber(long Group)															 ��ʼ
//  ����:�����û����������ݵĵ���
//	˵��:���ǵ��ļ���ʽ�Ĳ�ͬ, ��������ļ��йصĲ����ú�����ʾ.
//  ����:1997.10.14
long UnformFile::GetGroupNumber(float* GroupData)
{
	long *n=(long*)&GroupData[16];
	return *n;
}
//	����:GetGroupNumber(long Group)															 ����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ����:GetGroup()																				   ��ʼ
//  ����:��ȡ���������, ����ͷһ���.
//  ����:1997.10.4
long UnformFile::GetGroup(float *GroupData,long BeginGroup,long GroupNumber  /* =1 */)
{
	//  �ȳ���.
	long WantRead=m_DataNumberOfOneGroup*GroupNumber;
	for(int i=0;i<WantRead;i++)GroupData[i]=0;
	
	// ������. �������n��, BeginGroup ��Ϊn-1,��Ϊ����C����ƥ��.
	long P=BeginGroup*m_DataNumberOfOneGroup*4; 
	fseek(m_fpUnform,P,SEEK_SET);
	long RealRead=fread(GroupData,sizeof(float),WantRead,m_fpUnform); 

	// ����ʵ�ʶ���ĵ���.
	return RealRead/m_DataNumberOfOneGroup;
}
//  ����:GetGroupData()																					����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	  ���ƣ� SetShotMsg()
//    ���ܣ����ý���ļ��н���ڵ���Ϣ��
//    ע�⣺BeginGroup , ��һ��Ϊ0.
//				��һ��Ϊ0�ڡ�	
BOOL  UnformFile::SetShotMsg()
{
	CProgressDlg dlg;
	dlg.Create();
	dlg.SetWindowText("���ڷ�������ļ�");
	dlg.SetStatus("�Ѿ������ˣ�");
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	//     ���Ȼ��ÿ�ڴ�Լ���ٵ���GroupNumberInOneShot
	// ��ֵ�����Ժ��ڵĶ�ȡ�����б仯��
	long FirstFileNumber=GetFileNumber(0L);
	long FileNumber,pos,GroupNumberInOneShot;
	double n;
	
	// ����ǰ������
	for(long i=10;TRUE;i+=10){  // ��һ�ڲ��ö��ˡ�
		FileNumber=GetFileNumber(i);
		if(FileNumber!=FirstFileNumber){
			pos=i;
			break;
		}
	}

	// ������������
	for(i=pos-1;i>0;i--){   // posλ�øղ��Ѿ����ˡ�
		FileNumber=GetFileNumber(i);
		if(FileNumber==FirstFileNumber){
			GroupNumberInOneShot=i+1;
			break;
		}
	}

	// �ø��ڵ���ֵ��
	m_pShotMsg[0].FileNumber=FirstFileNumber;
	m_pShotMsg[0].BeginGroup=0;
	m_pShotMsg[0].EndGroup=i;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Ȼ��ѭ���������ÿ�ڵ���ʼ����������
	long CurrShot=0;
	do{
		CurrShot++;		
		
		if(CurrShot%10==1){
			n=(double)ftell(m_fpUnform);
			dlg.SetPos(n/m_UnformFileLength*100);
		}

		// ���ø��ڵ���ʼ�����ļ���
		pos=m_pShotMsg[CurrShot-1].EndGroup+1;
		
		FileNumber=GetFileNumber(pos);
		if(FileNumber==-1)break;  // �����ļ�β��

		m_pShotMsg[CurrShot].FileNumber=FileNumber;
		m_pShotMsg[CurrShot].BeginGroup=pos;

		// Ѱ�ҽ�������		
		pos+=GroupNumberInOneShot;
		FileNumber=GetFileNumber(pos);
		if(FileNumber!=m_pShotMsg[CurrShot].FileNumber||FileNumber==-1){
			for(pos--;pos>0;pos--){
				FileNumber=GetFileNumber(pos);
				if(FileNumber==m_pShotMsg[CurrShot].FileNumber){
					m_pShotMsg[CurrShot].EndGroup=pos;
					break;
				}
			}
		}
		else if(FileNumber==m_pShotMsg[CurrShot].FileNumber){
			 for(;;pos++){
				FileNumber=GetFileNumber(pos);
				if(FileNumber!=m_pShotMsg[CurrShot].FileNumber){
					m_pShotMsg[CurrShot].EndGroup=pos-1;
					break;
				}
			}
		}
				
	} while(TRUE);

	dlg.SetPos(100);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ���ļ��а�����������:
	m_TotalShotNumber=CurrShot; 	//  ��Ӧ��1����Ϊһ��ʼ�Ѿ�����һ�ڡ�

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  ���ڵ��ܵ�����
	for(i=0;i<m_TotalShotNumber;i++){
		m_pShotMsg[i].GroupNumber=
			m_pShotMsg[i].EndGroup-
			m_pShotMsg[i].BeginGroup+1;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// һ���а�������������
	m_MaxGroupNumber=0;
	for(i=0;i<m_TotalShotNumber;i++){
		if(m_pShotMsg[i].GroupNumber>m_MaxGroupNumber){
			m_MaxGroupNumber=m_pShotMsg[i].GroupNumber;
		}
	}
	
	dlg.DestroyWindow();

	return TRUE;
}
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   ���һ�ڵ����ݡ�
//    GetShot
BOOL  UnformFile::GetShot(float *DataRoom,long Shot)
{
	if(!GetGroup(DataRoom,
		m_pShotMsg[Shot].BeginGroup,
		m_pShotMsg[Shot].GroupNumber))return FALSE;
	return TRUE;
}
//  ���һ�ڵ����ݡ�
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

