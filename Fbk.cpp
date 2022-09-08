
#include "stdafx.h"
#include "3dfstbrk.h"
#include "fbk.h"

Fbk::Fbk()
{
	m_DataOK=FALSE;
	m_fpFbk=NULL;
	m_fFileName="";
}

Fbk::Fbk(CString FbkFile)
{
	Set(FbkFile);
}

BOOL Fbk::Set(CString FbkFile)
{
	m_fFileName=FbkFile;

	// 保证该文件的存在。
	m_fpFbk=fopen(FbkFile,"rb+");
	if(!m_fpFbk){
		m_fpFbk=fopen(FbkFile,"w");
		fclose(m_fpFbk);
		m_fpFbk=fopen(FbkFile,"rb+");
	}

	// 计算该文件中包含的总道数。
	fseek(m_fpFbk,0,SEEK_END);
	long n=ftell(m_fpFbk);
	m_TraceNumber=n/sizeof(DataInFstBrkFile);

	m_DataOK=TRUE;	

	return TRUE;
}

void Fbk::close()
{
	if(m_DataOK)
		fclose(m_fpFbk);
}

Fbk::~Fbk()
{
	if(m_DataOK)
		fclose(m_fpFbk);
}

long Fbk::Get(DataInFstBrkFile *room,long pos,long n)
{
	long l=0;
	for(int i=0;i<n;i++){
		room[i].FileNumber=0;
		room[i].FirstBreak=0;
	}

	if(m_DataOK){
		long p=pos*sizeof(DataInFstBrkFile);
		fseek(m_fpFbk,p,SEEK_SET);
		l=fread(room,sizeof(DataInFstBrkFile),n,m_fpFbk);
	}
	return l;
}

long Fbk::Put(DataInFstBrkFile *room,long pos,long n)
{
	long l=0;
	if(m_DataOK){
		long p=pos*sizeof(DataInFstBrkFile);
		fseek(m_fpFbk,p,SEEK_SET);
		l=fwrite(room,sizeof(DataInFstBrkFile),n,m_fpFbk);
	}
	return l;
}


bool Fbk::Save(CString sFile, DataInFstBrkFile * pRoom, long nDataNumber)
{
	
	FILE *fp=fopen(sFile,"w");
	if(!fp){
		AfxMessageBox("无法建立文件: "+sFile+" !");
		return false;
	}

	for(int i=0;i<nDataNumber;i++){
		fprintf(fp,"%i,%i,%ld\n",i,pRoom[i].FileNumber,pRoom[i].FirstBreak);
	}
	fclose(fp);
	
	return true;

}
