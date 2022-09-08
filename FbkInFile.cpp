// FbkInFile.cpp: implementation of the CFbkInFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "3dfstbrk.h"
#include "FbkInFile.h"

#include "fbk.h"
#include "ProgDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFbkInFile::CFbkInFile()
{
	m_fp=NULL;
	m_pShotFbk=NULL;
	m_nShotLimit=10000;
	m_pFbkMsg=new CFbkMsg[10000];

	Reset();
}

CFbkInFile::~CFbkInFile()
{
	Reset();

	if(m_pFbkMsg){
		delete []m_pFbkMsg;
		m_pFbkMsg=NULL;
	}
	if(m_pShotFbk){
		delete []m_pShotFbk;
		m_pShotFbk=NULL;
	}

}
bool CFbkInFile::Reset()
{
	if(m_fp){
		fclose(m_fp);
		m_fp=NULL;
	}
	m_nFileLen=0;
	m_nGroupNumber=0;
	m_nMaxGroupNumber=0;
	m_sFileName="";
	
	memset(&m_pFbkMsg[0],0,sizeof(CFbkMsg)*m_nShotLimit);

	return true;
}

bool CFbkInFile::Open(CString sFile)
{
	m_fp=fopen(sFile,"rb+");
	if(!m_fp)return false;
	
	// get the length of the file:
	fseek(m_fp,0,SEEK_END);
	m_nFileLen=ftell(m_fp);

	m_nGroupNumber=(double)m_nFileLen/sizeof(DataInFstBrkFile);
	if(m_nGroupNumber*sizeof(DataInFstBrkFile)!=m_nFileLen){
		Reset();
		return false;
	}

	m_sFileName=sFile;

	// Get the file number message:
	if(!Analyse()){
		Reset();
		return false;
	}
}

bool CFbkInFile::Analyse()
{
	m_nShotNumber=0;


	//////////
	CProgressDlg dlg;
	dlg.Create();
	dlg.SetWindowText("Analysing the First Break File");
	dlg.SetStatus("Analysed:");
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	//     首先获得每炮大约多少道：GroupNumberInOneShot
	// 该值随着以后炮的读取可能有变化。
	long FirstFileNumber=GetFileNumber(0L);
	long FileNumber,pos,GroupNumberInOneShot;
	double n;
	
	// 大步向前搜索。
	int nLongStep=30;
	for(long i=nLongStep;TRUE;i+=nLongStep){  // 第一炮不用读了。
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
	m_pFbkMsg[0].nFileNumber =FirstFileNumber;
	m_pFbkMsg[0].nStartGroup =0;
	m_pFbkMsg[0].nEndGroup =i;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 然后循环向后搜索每炮的起始、结束道。
	long CurrShot=0;	
	do{
		CurrShot++;		
		
		if(CurrShot%10==1){
			n=(double)ftell(m_fp);
			dlg.SetPos(n/m_nFileLen*100);
		}

		if(CurrShot>=m_nShotLimit){
			AfxMessageBox("Too much shots in the first break file!");
			return false;
		}

		// 设置该炮的起始道、文件号
		pos=m_pFbkMsg[CurrShot-1].nEndGroup +1;
		
		FileNumber=GetFileNumber(pos);
		if(FileNumber==-1)break;  // 超过文件尾。

		m_pFbkMsg[CurrShot].nFileNumber =FileNumber;
		m_pFbkMsg[CurrShot].nStartGroup=pos;

		// 寻找结束道：		
		pos+=GroupNumberInOneShot;
		FileNumber=GetFileNumber(pos);
		if(FileNumber==585){
			int mm=0;
		}
		if(FileNumber!=m_pFbkMsg[CurrShot].nFileNumber||FileNumber==-1){
			for(pos--;pos>0;pos--){
				FileNumber=GetFileNumber(pos);
				if(FileNumber==m_pFbkMsg[CurrShot].nFileNumber ){
					m_pFbkMsg[CurrShot].nEndGroup=pos;
					break;
				}
			}
		}
		else if(FileNumber==m_pFbkMsg[CurrShot].nFileNumber){
			 for(;;pos++){
				FileNumber=GetFileNumber(pos);
				if(FileNumber!=m_pFbkMsg[CurrShot].nFileNumber){
					m_pFbkMsg[CurrShot].nEndGroup=pos-1;
					break;
				}
			}
		}
				
	} while(TRUE);

	dlg.SetPos(100);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 该文件中包含的总炮数:
	m_nShotNumber=CurrShot; 	//  不应减1，因为一开始已经读了一炮。

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  各炮的总道数：	
	for(i=0;i<m_nShotNumber;i++){
		m_pFbkMsg[i].nGroupNumber =
			m_pFbkMsg[i].nEndGroup-
			m_pFbkMsg[i].nStartGroup+1;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 一炮中包含的最大道数：
	m_nMaxGroupNumber=0;
	for(i=0;i<m_nShotNumber;i++){
		if(m_pFbkMsg[i].nGroupNumber>m_nMaxGroupNumber){
			m_nMaxGroupNumber=m_pFbkMsg[i].nGroupNumber;
		}
	}

	// For returning to the user:
	if(m_pShotFbk){
		delete []m_pShotFbk;
		m_pShotFbk=NULL;
	}
	m_pShotFbk=new DataInFstBrkFile[m_nMaxGroupNumber];	
	
	// get the min and max file number:
	m_nSmallFileNumber=m_pFbkMsg[0].nFileNumber ;
	m_nBigFileNumber=m_pFbkMsg[0].nFileNumber ;
	for(i=1;i<m_nShotNumber;i++){
		if(m_pFbkMsg[i].nFileNumber ==0)continue;
		if(m_pFbkMsg[i].nFileNumber >m_nBigFileNumber){
			m_nBigFileNumber=m_pFbkMsg[i].nFileNumber ;
		}
		if(m_pFbkMsg[i].nFileNumber <m_nSmallFileNumber){
			m_nSmallFileNumber=m_pFbkMsg[i].nFileNumber ;
		}
	}

	//
	dlg.DestroyWindow();

	return TRUE;



}


long CFbkInFile::GetFileNumber(long nGroup)
{
	if(!m_fp)return -1;

	if(nGroup<0||nGroup>=m_nGroupNumber)return -1;

	DataInFstBrkFile data;
	long pos=nGroup*sizeof(DataInFstBrkFile);
	fseek(m_fp,pos,SEEK_SET);
	fread(&data,sizeof(DataInFstBrkFile),1,m_fp);
	return data.FileNumber ;
}

DataInFstBrkFile * CFbkInFile::GetShotFbk(
		long nFileNumber, 
		long &nGroupNumber)

{
	//
	if(!m_fp){
		return NULL;
	}

	// Get which shot is the filenumber:
	long i,n=-1;
	for(i=0;i<m_nShotNumber;i++){
		if(m_pFbkMsg[i].nFileNumber ==nFileNumber){
			n=i;
			break;
		}
	}

	if(n==-1)return NULL;

	//	 read the groups of first break of the shot:
	memset(m_pShotFbk,0,sizeof(DataInFstBrkFile)*m_nMaxGroupNumber);
	nGroupNumber=m_pFbkMsg[n].nGroupNumber ;
	fseek(m_fp,m_pFbkMsg[n].nStartGroup *sizeof(DataInFstBrkFile),SEEK_SET);
	long nRead=fread(
		m_pShotFbk,
		sizeof(DataInFstBrkFile),nGroupNumber,
		m_fp);
	

	// If not read rightly, return false:
	if(nRead==nGroupNumber)
		return m_pShotFbk;
	else
		return NULL;
}

bool CFbkInFile::ShowMsg(CString sFile)
{
	FILE *fp=fopen(sFile,"wt");
	if(!fp)return false;

	long nGroup;
	fprintf(fp,"Shot Number: %ld\n",m_nShotNumber);
	for (long i=0;i<m_nShotNumber;i++){
		nGroup=m_pFbkMsg[i].nEndGroup -m_pFbkMsg[i].nStartGroup +1;
		fprintf(fp,"No. %ld ,FileNumber: %ld,GN: %ld, Start:%ld, End: %ld \n",i,m_pFbkMsg[i].nFileNumber ,nGroup,m_pFbkMsg[i].nStartGroup ,m_pFbkMsg[i].nEndGroup );
	}
	fclose(fp);

	return true;

}

bool CFbkInFile::WriteShotFbk()
{
	//
	if(!m_fp){
		return false;
	}

	// Get which shot is the filenumber:
	long i,n=-1;
	for(i=0;i<m_nShotNumber;i++){
		if(m_pFbkMsg[i].nFileNumber ==m_pShotFbk[0].FileNumber){
			n=i;
			break;
		}
	}

	if(n==-1)return false;

	//	 Write the groups of first break of the shot:
	long nGroupNumber=m_pFbkMsg[n].nGroupNumber ;
	fseek(m_fp,m_pFbkMsg[n].nStartGroup *sizeof(DataInFstBrkFile),SEEK_SET);
	long nWrite=fwrite(
		m_pShotFbk,
		sizeof(DataInFstBrkFile),nGroupNumber,
		m_fp);
	

	// If not read rightly, return false:
	if(nWrite==nGroupNumber)
		return true;
	else
		return false;

}
