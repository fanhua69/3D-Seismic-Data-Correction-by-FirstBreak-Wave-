
#include "stdafx.h"
#include "3dfstbrk.h"
#include "shotmsg.h"
#include "Unform.h"
#include "ProgDlg.h"
#include "stdio.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  构造函数, 析构函数
UnformFile::UnformFile()
{
	m_UnDataOK=FALSE;
	m_pShotMsg=NULL;
	m_fpUnform=NULL;
	m_FileUnform="";
	m_TopFileNumber=0;
	m_EndFileNumber=0;
	m_UnformGroupNumber=0;  //该文件中共有多少道.
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

	m_fpUnform=fopen(FileName,"rb");	// 打开文件, 并检查其合法性.
	if(!m_fpUnform){
		AfxMessageBox("该解编文件不存在！");
		return FALSE;
	}
	
	m_FileNumberPosition=4 ; //220;  // 本机浮点格式(format 4)的文件号位置(在一道数据中).
	m_UnformGroupHead=128;  // 本机浮点格式道头的数据个数.
	m_nGroupNamePosition=16;//
	
	fseek(m_fpUnform,32,SEEK_SET);
	fread(&m_UnformTimeInterval,sizeof(long),1,m_fpUnform);    // 读取采样间隔
	fread(&m_UnformTimeLength,sizeof(long),1,m_fpUnform);  //读取解编长度

	if(m_UnformTimeInterval>16||m_UnformTimeInterval<2
		||m_UnformTimeLength<0||m_UnformTimeLength>20000){
		AfxMessageBox(FileName+"文件格式不对,本软件只能处理GRISYS的Format 4格式!");
		return FALSE;
	}

	fseek(m_fpUnform,m_FileNumberPosition,SEEK_SET);
	fread(&m_TopFileNumber,sizeof(long),1,m_fpUnform);           //首文件号
	
	fseek(m_fpUnform,0,SEEK_END);
	m_UnformFileLength=ftell(m_fpUnform);

	m_PointNumberOfOneGroup=m_UnformTimeLength/m_UnformTimeInterval; //每道中包含的数据个数.
	m_DataNumberOfOneGroup=m_PointNumberOfOneGroup+m_UnformGroupHead; // 每道的数据点加上道头的数据个数.
	m_ByteNumberOfOneGroup=(m_UnformGroupHead+m_PointNumberOfOneGroup)*4;
	m_UnformGroupNumber=m_UnformFileLength/m_ByteNumberOfOneGroup; //总共解编了多少道.

	fseek(m_fpUnform,-(m_ByteNumberOfOneGroup-m_FileNumberPosition),SEEK_END);
	fread(&m_EndFileNumber,sizeof(long),1,m_fpUnform);    //最后一个文件号.

	if(m_pShotMsg==NULL)m_pShotMsg=new ShotMsg[5000];  //若该指针尚未分配内存，则分配。
	SetShotMsg();

	
	return TRUE;	
}

UnformFile::~UnformFile()
{
	if(m_UnDataOK)fclose(m_fpUnform);
	if(m_pShotMsg)delete m_pShotMsg;
}
//	 构造函数.																									  结束
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	名称:GetFileNumber(long Group)
//  功能:返回用户所给道数的文件号
//	说明:考虑到文件格式的不同, 必须对与文件有关的操作用函数表示.
//  日期:1997.10.4
long UnformFile::GetFileNumber(long Group)
{
	// 计算文件号的位置.
	long P=Group*(m_UnformGroupHead+m_PointNumberOfOneGroup)*4
		+m_FileNumberPosition;  // File Number Position : 220.
	
	// 若该位置违法.
	if(P<0)return -2;
	if(P>m_UnformFileLength)return -1;

	// 按位置读取文件号.
	long FileNumber;
	fseek(m_fpUnform,P,SEEK_SET);
	fread(&FileNumber,sizeof(long),1,m_fpUnform);

	// 返回获得的文件号.
	return FileNumber;
}
//	名称:GetFileNumber(long Group)															   结束
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	名称:GetGroupNumber(long Group)															 开始
//  功能:返回用户所给道数据的道号
//	说明:考虑到文件格式的不同, 必须对与文件有关的操作用函数表示.
//  日期:1997.10.14
long UnformFile::GetFileNumber(float* GroupData)
{
	long *n=(long*)&GroupData[m_FileNumberPosition/4];
	return *n;
}
//	名称:GetGroupNumber(long Group)															 结束
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	名称:GetGroupNumber(long Group)															 开始
//  功能:返回用户所给道数据的道号
//	说明:考虑到文件格式的不同, 必须对与文件有关的操作用函数表示.
//  日期:1997.10.14
long UnformFile::GetGroupNumber(float* GroupData)
{
	long *n=(long*)&GroupData[16];
	return *n;
}
//	名称:GetGroupNumber(long Group)															 结束
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  名称:GetGroup()																				   开始
//  功能:读取多道的数据, 连道头一起读.
//  日期:1997.10.4
long UnformFile::GetGroup(float *GroupData,long BeginGroup,long GroupNumber  /* =1 */)
{
	//  先充零.
	long WantRead=m_DataNumberOfOneGroup*GroupNumber;
	for(int i=0;i<WantRead;i++)GroupData[i]=0;
	
	// 读数据. 若想读第n道, BeginGroup 置为n-1,是为了与C语言匹配.
	long P=BeginGroup*m_DataNumberOfOneGroup*4; 
	fseek(m_fpUnform,P,SEEK_SET);
	long RealRead=fread(GroupData,sizeof(float),WantRead,m_fpUnform); 

	// 返回实际读入的道数.
	return RealRead/m_DataNumberOfOneGroup;
}
//  名称:GetGroupData()																					结束
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	  名称： SetShotMsg()
//    功能：设置解编文件中解编炮的信息。
//    注意：BeginGroup , 第一道为0.
//				第一炮为0炮。	
BOOL  UnformFile::SetShotMsg()
{
	CProgressDlg dlg;
	dlg.Create();
	dlg.SetWindowText("正在分析解编文件");
	dlg.SetStatus("已经分析了：");
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	//     首先获得每炮大约多少道：GroupNumberInOneShot
	// 该值随着以后炮的读取可能有变化。
	long FirstFileNumber=GetFileNumber(0L);
	long FileNumber,pos,GroupNumberInOneShot;
	double n;
	
	// 大步向前搜索。
	for(long i=10;TRUE;i+=10){  // 第一炮不用读了。
		FileNumber=GetFileNumber(i);
		if(FileNumber!=FirstFileNumber){
			pos=i;
			break;
		}
	}

	// 向后逐道搜索。
	for(i=pos-1;i>0;i--){   // pos位置刚才已经读了。
		FileNumber=GetFileNumber(i);
		if(FileNumber==FirstFileNumber){
			GroupNumberInOneShot=i+1;
			break;
		}
	}

	// 置该炮的数值。
	m_pShotMsg[0].FileNumber=FirstFileNumber;
	m_pShotMsg[0].BeginGroup=0;
	m_pShotMsg[0].EndGroup=i;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 然后循环向后搜索每炮的起始、结束道。
	long CurrShot=0;
	do{
		CurrShot++;		
		
		if(CurrShot%10==1){
			n=(double)ftell(m_fpUnform);
			dlg.SetPos(n/m_UnformFileLength*100);
		}

		// 设置该炮的起始道、文件号
		pos=m_pShotMsg[CurrShot-1].EndGroup+1;
		
		FileNumber=GetFileNumber(pos);
		if(FileNumber==-1)break;  // 超过文件尾。

		m_pShotMsg[CurrShot].FileNumber=FileNumber;
		m_pShotMsg[CurrShot].BeginGroup=pos;

		// 寻找结束道：		
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
	// 该文件中包含的总炮数:
	m_TotalShotNumber=CurrShot; 	//  不应减1，因为一开始已经读了一炮。

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  各炮的总道数：
	for(i=0;i<m_TotalShotNumber;i++){
		m_pShotMsg[i].GroupNumber=
			m_pShotMsg[i].EndGroup-
			m_pShotMsg[i].BeginGroup+1;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 一炮中包含的最大道数：
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
//   获得一炮的数据。
//    GetShot
BOOL  UnformFile::GetShot(float *DataRoom,long Shot)
{
	if(!GetGroup(DataRoom,
		m_pShotMsg[Shot].BeginGroup,
		m_pShotMsg[Shot].GroupNumber))return FALSE;
	return TRUE;
}
//  获得一炮的数据。
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

