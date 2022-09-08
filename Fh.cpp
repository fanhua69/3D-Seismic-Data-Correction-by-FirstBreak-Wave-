/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//        PUBLIC SUBROUTINES  MADE BY FAN HUA.												  //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "3dfstbrk.h"
#include "fh.h"

int AskSwathNumber()  // 询问线束号.
{  
		CDSwNum dlg;
		dlg.m_SwathNumber=1;
		if(dlg.DoModal()==IDOK){
			return dlg.m_SwathNumber;
		}
		return -1;
}

CString IntToName(int Swath)  // 变线束号为以0打头的三位字符串.
{
	char a[10];
	_itoa(Swath,a,10);
	CString s1('0',3-strlen(a));
	CString FileName=s1+a;
	return FileName;
}

CString STR(float Value)// 变数值为字符串
{
		char a[30];
		sprintf(a,"%f",Value);
		return CString(a);
}
CString STR(int Value)	 // 变数值为字符串
{
		char a[30];
		sprintf(a,"%d",Value);
		return CString(a);
}

CString STR(double Value)// 变数值为字符串
{
		char a[30];
		sprintf(a,"%f",Value);
		return CString(a);
}

CString STR(long Value) // 变数值为字符串
{
		char a[30];
		sprintf(a,"%ld",Value);
		return CString(a);
}

BOOL CLS(CDC *pDC)  //清屏幕.
{
	CRect ClientRect;
	CWnd *cw=pDC->GetWindow();
	cw->GetClientRect(ClientRect);      // get current window size.
	pDC->FillRect(&ClientRect,&CBrush(RGB(255,255,255))); 	// 清屏幕.
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
// Programer : FAN HUA. －－ 范  华 －－
#include "svsys.h"
#include "swathpar.h"
#include "equation.h"
#include "progdlg.h"
#include "fbkdoc.h"

BOOL IntegrateFstBrk()
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  询问用户所要合并的线束号.
	int i,j;
	CDSwNum dlg1;
	dlg1.m_SwathNumber=1;
	if(dlg1.DoModal()==IDCANCEL)return FALSE;
	int SwathNumber=dlg1.m_SwathNumber;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  建立一个文件打开窗口, 以获得用户所要合并的文件名.
	char ff[]="初至文件(*.fbk)|*.fbk||";

	CFileDialog dlg(TRUE,NULL,"*.*",OFN_ALLOWMULTISELECT,ff); 
	char s[300]={0};
	dlg.m_ofn.lpstrFile=s;
	dlg.m_ofn.nMaxFile=300;

	if(dlg.DoModal()==IDCANCEL){
		AfxMessageBox("输入文件名称失败!");
		return FALSE;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	//   将存有文件名的内存区域转化为文件名数组.
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
		NumberOfFile=1; // 如果只有一个文件被选中, 则只有一个文件名.
		strcpy(file[1],(LPCTSTR)dlg.GetFileName());
	}
	else
		NumberOfFile=row-1;	// 若有多个文件被选中, 则第一个文件名是路径名,其次才是各个文件名.

	//////////////////////////////////////////////////////////////////////////////////////////////////
	// 读入所有初至文件的第一个点的文件号.
	FILE *fp;
	int* filenumber=new int[NumberOfFile];
	for(i=0;i<NumberOfFile;i++){
		fp=fopen(file[i+1],"rb");   //第一个文件名是 路径名.
		if(!fp){
			AfxMessageBox("无法打开初至文件"+CString(file[i])+"!",MB_OK);   
			return FALSE;
		}
		fseek(fp,0,SEEK_SET);     // because this is first break file , so 0 position is the fst brk
		fread(&filenumber[i],sizeof(long),1,fp);  
		fclose(fp);
	}
			 
	////////////////////////////////////////////////////////////////////////////////////////
	//  合并初至文件.
	// ------------------------------------------------------------------------------------
	// 打开最终文件.
	// Here do not consider if Equation has been successfully made, 
	//because we need only the  ultimate first break file name.
	Equation equa;
	if(!equa.Set(SwathNumber))return false;
	
	CString UltiFstBrkFile=equa.m_FileSwathFirstBreak;
	fp=fopen(UltiFstBrkFile,"wb");
	if(!fp){
		AfxMessageBox("无法建立最终初至文件!",MB_OK);  // Can not make first break file.
		return FALSE;
	}

	// 建立一个进程条.
	CProgressDlg dlg3;
	dlg3.Create();
	dlg3.SetStatus("正在合并第 "+STR(SwathNumber)+" 束线的初至文件:");	  
	dlg3.SetWindowText("合并初至文件");

	long RoomSize=500,nRead,nWrite,MinN,MinF;
	float n=0.0;
	FILE *fpSeperate;
	DataInFstBrkFile *data=new DataInFstBrkFile[RoomSize];
		
	// 对所有文件循环.
	while(TRUE){
		n++;
		dlg3.SetPos(n/NumberOfFile*100);
		  
		// 寻找最小文件号的文件名.
		MinN=10000000,MinF=100000;
		for(j=0;j<NumberOfFile;j++){
			if(filenumber[j]<MinN){
				MinN=filenumber[j];
				MinF=j+1;   // 第一个文件名是路径名.
			}
		}
		if(MinF>=100000)break;  // 所有文件均已被合并.
		filenumber[MinF-1]=10000000;
		
		// 打开该文件.
		fpSeperate=fopen(file[MinF],"rb");
		if(!fpSeperate){
			AfxMessageBox("不能打开初至文件"+CString(file[i])+"！");
			return FALSE;
		}

		// 读该文件至最终文件中.
		while(!feof(fpSeperate)){				// Loop for the seperate file.
			nRead=fread(data,sizeof(DataInFstBrkFile),RoomSize,fpSeperate);
			nWrite=fwrite(data,sizeof(DataInFstBrkFile),nRead,fp);
			if(nWrite<nRead){
				AfxMessageBox("当向最终初至文件"+UltiFstBrkFile+"中写数据时出错！");
				return FALSE;
			}
		}
		fclose(fpSeperate);						  // Loop for the seperate file.
	}												    // Loop for all first break files.
	fclose(fp);

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  Tell the user that all have been done.
	dlg3.DestroyWindow();
	AfxMessageBox("最终初至文件"+UltiFstBrkFile+"已经合并完成！",MB_OK);

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 返回.
	return TRUE;
}
//  Name  :  	 IntegrateCFbkDoc
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
