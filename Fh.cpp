/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        PUBLIC SUBROUTINES  MADE BY FAN HUA.												  //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "3dfstbrk.h"
#include "fh.h"

int AskSwathNumber()  // ѯ��������.
{  
		CDSwNum dlg;
		dlg.m_SwathNumber=1;
		if(dlg.DoModal()==IDOK){
			return dlg.m_SwathNumber;
		}
		return -1;
}

CString IntToName(int Swath)  // ��������Ϊ��0��ͷ����λ�ַ���.
{
	char a[10];
	_itoa(Swath,a,10);
	CString s1('0',3-strlen(a));
	CString FileName=s1+a;
	return FileName;
}

CString STR(float Value)// ����ֵΪ�ַ���
{
		char a[30];
		sprintf(a,"%f",Value);
		return CString(a);
}
CString STR(int Value)	 // ����ֵΪ�ַ���
{
		char a[30];
		sprintf(a,"%d",Value);
		return CString(a);
}

CString STR(double Value)// ����ֵΪ�ַ���
{
		char a[30];
		sprintf(a,"%f",Value);
		return CString(a);
}

CString STR(long Value) // ����ֵΪ�ַ���
{
		char a[30];
		sprintf(a,"%ld",Value);
		return CString(a);
}

BOOL CLS(CDC *pDC)  //����Ļ.
{
	CRect ClientRect;
	CWnd *cw=pDC->GetWindow();
	cw->GetClientRect(ClientRect);      // get current window size.
	pDC->FillRect(&ClientRect,&CBrush(RGB(255,255,255))); 	// ����Ļ.
	return TRUE;
}

long RevByte(long id)
{

    id = ((id & 0xff) << 24) | ((id & 0xff00) << 8)
          | ((id & 0xff0000) >> 8) | ((id & 0xff000000) >> 24);
    return id;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Name  :  	 IntegrateCFbkDoc
//  Function :   Integrare the single first breal files , to one ultimate file.
//	Date   	:      1997.9.8
// Programer : FAN HUA. ���� ��  �� ����
#include "svsys.h"
#include "swathpar.h"
#include "equation.h"
#include "progdlg.h"
#include "fbkdoc.h"

BOOL IntegrateFstBrk()
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  ѯ���û���Ҫ�ϲ���������.
	int i,j;
	CDSwNum dlg1;
	dlg1.m_SwathNumber=1;
	if(dlg1.DoModal()==IDCANCEL)return FALSE;
	int SwathNumber=dlg1.m_SwathNumber;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  ����һ���ļ��򿪴���, �Ի���û���Ҫ�ϲ����ļ���.
	char ff[]="�����ļ�(*.fbk)|*.fbk||";

	CFileDialog dlg(TRUE,NULL,"*.*",OFN_ALLOWMULTISELECT,ff); 
	char s[300]={0};
	dlg.m_ofn.lpstrFile=s;
	dlg.m_ofn.nMaxFile=300;

	if(dlg.DoModal()==IDCANCEL){
		AfxMessageBox("�����ļ�����ʧ��!");
		return FALSE;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	//   �������ļ������ڴ�����ת��Ϊ�ļ�������.
	char file[20][13];
	int row=0,col=0;
	for(i=0;i<240;i++){
		file[row][col]=s[i];
		col++;
		if(iscntrl(s[i])){
			row++;
			col=0;
			if(iscntrl(s[i+1]))break;
		}
	}
	int NumberOfFile;		   
	if(row==1){
		NumberOfFile=1; // ���ֻ��һ���ļ���ѡ��, ��ֻ��һ���ļ���.
		strcpy(file[1],(LPCTSTR)dlg.GetFileName());
	}
	else
		NumberOfFile=row-1;	// ���ж���ļ���ѡ��, ���һ���ļ�����·����,��β��Ǹ����ļ���.

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// �������г����ļ��ĵ�һ������ļ���.
	FILE *fp;
	int* filenumber=new int[NumberOfFile];
	for(i=0;i<NumberOfFile;i++){
		fp=fopen(file[i+1],"rb");   //��һ���ļ����� ·����.
		if(!fp){
			AfxMessageBox("�޷��򿪳����ļ�"+CString(file[i])+"!",MB_OK);   
			return FALSE;
		}
		fseek(fp,0,SEEK_SET);     // because this is first break file , so 0 position is the fst brk
		fread(&filenumber[i],sizeof(long),1,fp);  
		fclose(fp);
	}
			 
	////////////////////////////////////////////////////////////////////////////////////////
	//  �ϲ������ļ�.
	// ------------------------------------------------------------------------------------
	// �������ļ�.
	// Here do not consider if Equation has been successfully made, 
	//because we need only the  ultimate first break file name.
	Equation equa;
	if(!equa.Set(SwathNumber))return false;
	
	CString UltiFstBrkFile=equa.m_FileSwathFirstBreak;
	fp=fopen(UltiFstBrkFile,"wb");
	if(!fp){
		AfxMessageBox("�޷��������ճ����ļ�!",MB_OK);  // Can not make first break file.
		return FALSE;
	}

	// ����һ��������.
	CProgressDlg dlg3;
	dlg3.Create();
	dlg3.SetStatus("���ںϲ��� "+STR(SwathNumber)+" ���ߵĳ����ļ�:");	  
	dlg3.SetWindowText("�ϲ������ļ�");

	long RoomSize=500,nRead,nWrite,MinN,MinF;
	float n=0.0;
	FILE *fpSeperate;
	DataInFstBrkFile *data=new DataInFstBrkFile[RoomSize];
		
	// �������ļ�ѭ��.
	while(TRUE){
		n++;
		dlg3.SetPos(n/NumberOfFile*100);
		  
		// Ѱ����С�ļ��ŵ��ļ���.
		MinN=10000000,MinF=100000;
		for(j=0;j<NumberOfFile;j++){
			if(filenumber[j]<MinN){
				MinN=filenumber[j];
				MinF=j+1;   // ��һ���ļ�����·����.
			}
		}
		if(MinF>=100000)break;  // �����ļ����ѱ��ϲ�.
		filenumber[MinF-1]=10000000;
		
		// �򿪸��ļ�.
		fpSeperate=fopen(file[MinF],"rb");
		if(!fpSeperate){
			AfxMessageBox("���ܴ򿪳����ļ�"+CString(file[i])+"��");
			return FALSE;
		}

		// �����ļ��������ļ���.
		while(!feof(fpSeperate)){				// Loop for the seperate file.
			nRead=fread(data,sizeof(DataInFstBrkFile),RoomSize,fpSeperate);
			nWrite=fwrite(data,sizeof(DataInFstBrkFile),nRead,fp);
			if(nWrite<nRead){
				AfxMessageBox("�������ճ����ļ�"+UltiFstBrkFile+"��д����ʱ����");
				return FALSE;
			}
		}
		fclose(fpSeperate);						  // Loop for the seperate file.
	}												    // Loop for all first break files.
	fclose(fp);

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  Tell the user that all have been done.
	dlg3.DestroyWindow();
	AfxMessageBox("���ճ����ļ�"+UltiFstBrkFile+"�Ѿ��ϲ���ɣ�",MB_OK);

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ����.
	return TRUE;
}
//  Name  :  	 IntegrateCFbkDoc
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
