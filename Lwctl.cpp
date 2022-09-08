
#include "stdafx.h"
#include "3dfstbrk.h"
#include "struct.h"
#include "LWCtl.h"
#include "progdlg.h"
#include "math.h"
#include "fh.h"
#include "TopoNet.h"
#include "OutFbkFile.h"
#include "MidFbkFile.h"
#include "SndFile.h"
#include "ControlFile.h"
#include "InP190.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructions, deconstructions.
LongWaveControl::LongWaveControl()
{
}

LongWaveControl::~LongWaveControl()
{
	Reset();
}

void LongWaveControl::Reset()
{
}

bool LongWaveControl::CreateNet()
{
	// 读取由其它程序计算得到的静校正量文件.
	CSndFile fileSnd;
	if(!fileSnd.ReadShot())return false;
	if(!fileSnd.ReadRcv())return false;

	CMidFbkFile fileMidFbk;
	if(!fileMidFbk.ReadShot())return false;
	if(!fileMidFbk.ReadRcv())return false;

	CControlFile fileControl;
	if(!fileControl.Read ())return false;

	//
	CTopoNet net;
	net.SetPrecision (10);

	// Set Shot Point Net:
	long nShotCtlPoint=fileControl.GetShotCtlNumber ();
	PXYV *pShotCtl=fileControl.GetShotCtl ();

	long nShotSndPoint=fileSnd.GetShotNumber ();
	PXYV *pShotSnd=fileSnd.GetShotData ();

	long nShotFbkPoint=fileMidFbk.GetShotNumber ();
	PXYV *pShotFbk=fileMidFbk.GetShotData ();

	
	net.SetPointNumber (nShotCtlPoint);
	long nOther,nThis;
	double dv;
	for(long i=0;i<nShotCtlPoint;i++){
		nOther=fileSnd.SearchShotStation(pShotCtl[i].Ph );
		nThis=fileMidFbk.SearchShotStation(pShotCtl[i].Ph );
		if(nOther==-1){
			CString s;
			s.Format("%ld",pShotCtl[i].Ph);
			AfxMessageBox("Can not find this Ph in other static correction file :"+s);
			continue;
		}

		if(nThis==-1){
			CString s;
			s.Format("%ld",pShotCtl[i].Ph);
			AfxMessageBox("Can not find this Ph in middling result file :"+s);
			continue;
		}

		dv=pShotSnd[nOther].Value-pShotFbk[nThis].Value;

		net.SetPoint(pShotSnd[nOther].east,pShotSnd[nOther].north,dv,i);
	}
	net.SetTitle ("Shot point long wave control net");
	if(net.CalcNet ())net.Save ("");


	// Set Rcv Point Net:
	long nRcvCtlPoint=fileControl.GetRcvCtlNumber ();
	PXYV *pRcvCtl=fileControl.GetRcvCtl ();

	long nRcvSndPoint=fileSnd.GetRcvNumber ();
	PXYV *pRcvSnd=fileSnd.GetRcvData ();

	long nRcvFbkPoint=fileMidFbk.GetRcvNumber ();
	PXYV *pRcvFbk=fileMidFbk.GetRcvData ();

	
	net.SetPointNumber (nRcvCtlPoint);
	for(i=0;i<nRcvCtlPoint;i++){
		nOther=fileSnd.SearchRcvStation(pRcvCtl[i].Ph );
		nThis=fileMidFbk.SearchRcvStation(pRcvCtl[i].Ph );
		if(nOther==-1){
			CString s;
			s.Format("%ld",pRcvCtl[i].Ph);
			AfxMessageBox("Can not find this Ph in other static correction file :"+s);
			continue;
		}

		if(nThis==-1){
			CString s;
			s.Format("%ld",pRcvCtl[i].Ph);
			AfxMessageBox("Can not find this Ph in middling result file :"+s);
			continue;
		}
		
		dv=pRcvSnd[nOther].Value-pRcvFbk[nThis].Value;

		net.SetPoint(pRcvSnd[nOther].east,pRcvSnd[nOther].north,dv,i);
	}
	net.SetTitle ("Recieve point long wave control net");
	if(net.CalcNet ())net.Save ("");

	return true;
}


bool LongWaveControl::ControlByNet()
{
	///////////////////////////////////////////////////////////////
	//  Read data:
	CMidFbkFile fileMidFbk;
	if(!fileMidFbk.ReadShot())return false;
	if(!fileMidFbk.ReadRcv())return false;

	CSndFile fileSnd;
	if(!fileSnd.ReadShot())return false;
	if(!fileSnd.ReadRcv())return false;
	

	long nShotFbkPoint=fileMidFbk.GetShotNumber ();
	PXYV *pShotFbk=fileMidFbk.GetShotData ();
	long nRcvFbkPoint=fileMidFbk.GetRcvNumber ();
	PXYV *pRcvFbk=fileMidFbk.GetRcvData ();

	
	COutFbkFile fileOutFbk;
	fileOutFbk.SetShot (pShotFbk,nShotFbkPoint);	
	fileOutFbk.SetRcv (pRcvFbk,nRcvFbkPoint);

	CInP190 p190;
	if(!p190.ReadRcvPar ())return false;
	if(!p190.ReadShotPar ())return false;


	PXYV *pOutShot=fileOutFbk.GetShotData ();
	PXYV *pOutRcv=fileOutFbk.GetRcvData ();

	/////////////////////////////////////////////
	//
	CTopoNet net;
	net.SetPrecision(10);
	
	/////////////////////////////////////////////
	// Control the shot point net:
	net.SetTitle("Shot point long wave control net");
	if(!net.Open())return false;

	float dv;
	long j,n,nThis,nLast;

	// Set the coordinates of the out fbk data:
	for(long i=0;i<nShotFbkPoint;i++){
		n=p190.SearchShotStation (pOutShot[i].Ph );
		if(n==-1){
			AfxMessageBox("Can not find the coordinates for the shot point:"+STR(pOutShot[i].Ph ));
			continue;
		}
		pOutShot[i].east =p190.m_pShotPhyPar [n].east ;
		pOutShot[i].north=p190.m_pShotPhyPar [n].north;
	}


	// Firstly deal with the shot points that has no value:
	for(i=0;i<nShotFbkPoint;i++){
		if(pOutShot[i].Value ==-9999){
			
			n=fileSnd.SearchShotStation (pOutShot[i].Ph );
			if(n==-1){
				CString s;
				s.Format("%ld",pOutShot[i].Ph );
				AfxMessageBox("Error ! Can not find the station in the other static correction file : "+s);
				break;
			}
			else{
				nLast=-1;
				for(j=i-1;j>=0;j--){
					if(pOutShot[j].Value ==-9999)continue;
					nLast=fileSnd.SearchShotStation (pOutShot[j].Ph );
					nThis=j;
					if(nLast>-1)break;
				}
				if(nLast==-1){					
					for(j=i+1;j<nShotFbkPoint;j++){
						if(pOutShot[j].Value ==-9999)continue;
						nLast=fileSnd.SearchShotStation (pOutShot[j].Ph );
						nThis=j;
						if(nLast>-1)break;
					}
					if(nLast==-1){
						AfxMessageBox("Error: can not find one station same with any of the fbk station!");
						break;
					}
				}
					
				dv=fileSnd.m_pShotData [n].Value -fileSnd.m_pShotData [nLast].Value;
				pOutShot[i].Value =pOutShot[nThis].Value +dv;
			}
		}
	}

	// Then deal with all of the shot points: 	
	for(i=0;i<nShotFbkPoint;i++){
		pOutShot[i].Value +=net.GetValue (pOutShot[i].east,pOutShot[i].north);
	}
	
	/////////////////////////////////////////////
	// Control the rcv point net:
	net.SetTitle("Recieve point long wave control net");
	if(!net.Open())return false;

	// Set the coordinates of the recieve out fbk data:
	for(i=0;i<nRcvFbkPoint;i++){
		n=p190.SearchRcvStation (pOutRcv[i].Ph );
		if(n==-1){
			AfxMessageBox("Can not find the coordinates for the reciever point:"+STR(pOutRcv[i].Ph ));
			continue;
		}
		pOutRcv[i].east =p190.m_pRcvPhyPar [n].east ;
		pOutRcv[i].north=p190.m_pRcvPhyPar [n].north;
	}



	for(i=0;i<nRcvFbkPoint;i++){
		pOutRcv[i].Value +=net.GetValue (pOutRcv[i].east,pOutRcv[i].north);
	}

	
	/////////////////////////////////////////////
	// 存储最终静校正成果.
	fileOutFbk.WriteRcv ();
	fileOutFbk.WriteShot();

	AfxMessageBox("长波长控制完毕!");
	
	return TRUE;

}





	/*
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//              对所有的中间成果点, 搜索距离最近的控制点.
	//	----------------------------------------------------------------------------------------------------------------------
	double dx,dy,d,TotalDistance,MidDataChange;
	double BiggestDistance;
	long SmallRow,BigRow,BeginPoint;
	long ControlPointNeeded=5;
	if(ControlPointNeeded>ControlPointNumber)ControlPointNeeded=ControlPointNumber;
	ND *ControlPointNearest=new ND[ControlPointNeeded];
	BOOL SmallDirectionOK,BigDirectionOK;

	CProgressDlg dlg;
	dlg.Create();
	dlg.SetWindowText("正在对中间成果点进行控制");
	dlg.SetStatus("已经控制了");
	
	
	for(i=0;i<MidDataNumber;i++){   
		
		//  设置进程条。
		if(i%10==0)dlg.SetPos((float)i/MidDataNumber*100);

		// 若中间成果值超出范围， 则该点是未控制点， 
		// 其值应采用用户数据。
		if(MidData[i].Value<-5000){
			n=SearchMatchValue(MidData[i].Ph,InputData,InputDataNumber);
			if(InputData[n].Ph!=MidData[i].Ph){
				AfxMessageBox("中间成果数据点（桩号为"+STR(MidData[i].Ph)+"），\n\
					的桩号在用户的输入数据中没有找到，无法继续！");
				return FALSE;
			}
			MidData[i].Value=InputData[n].Value;
			continue;
		}
		
		// 寻找距离该数据点最近的一行.
		SmallRow=SearchForControlRow(MidData[i].east,ControlPoint,ControlPointNumber,RowPosition,RowNumber);
		BigRow=SmallRow+1;
		
		// 先对ControlPointNearest赋较大值, 以便替换之用.
		for(j=0;j<ControlPointNeeded;j++){
			ControlPointNearest[j].Number=0;
			ControlPointNearest[j].Distance=10000000;
		}
		
		// 分大小两方向检测控制点, 搜索最近的几个控制点, 赋给ControlPointNearest.
		SmallDirectionOK=FALSE;
		BigDirectionOK=FALSE;
		while(!SmallDirectionOK&&!BigDirectionOK){
			
			// 检测小X坐标方向的一行.
			if(SmallRow>=0&&!SmallDirectionOK){

				// 把这一行的所有数据检查一遍, 
				//拣出其中较已经找到的控制点为小者替换ControlPointNearest。
				BeginPoint=RowPosition[SmallRow];
				dx=fabs(ControlPoint[BeginPoint].east-MidData[i].east);  // 需要fabs。
				for(j=0;j<NumInRow[SmallRow];j++){	//对该行中的控制点循环.

					//计算该点的距离.
					dy=ControlPoint[BeginPoint+j].north-MidData[i].north;
					d=sqrt(dx*dx+dy*dy);                  

					//寻找已经找到的控制点中距离最大者。
					n=0;
					for(k=0;k<ControlPointNeeded;k++){
						if(ControlPointNearest[k].Distance>ControlPointNearest[n].Distance){
							n=k;
						}
					}

					// 若此点距离小于已经找到的点的最大距离，则替换之。
					if(d<ControlPointNearest[n].Distance){			
							ControlPointNearest[n].Number=BeginPoint+j;
							ControlPointNearest[n].Distance=d;
					}
				}

				// 找出现有的ControlPointNearest中最大者, 
				// 若其小于等于该行与数据点的距离, 则不再继续搜索.
				BiggestDistance=0;
				for(j=0;j<ControlPointNeeded;j++){
					if(ControlPointNearest[j].Distance>BiggestDistance)BiggestDistance=ControlPointNearest[j].Distance;
				}
				if(BiggestDistance<=dx)SmallDirectionOK=TRUE;
			}

			// 检测大X坐标方向的一行.
			if(BigRow<RowNumber&&!BigDirectionOK){

				// 把这一行的所有数据检查一遍, 
				//拣出其中较小者替换ControlPointNearest中的控制点.
				BeginPoint=RowPosition[BigRow];
				dx=ControlPoint[BeginPoint].east-MidData[i].east;
				for(j=0;j<NumInRow[BigRow];j++){
				
					//计算该点的距离.
					dy=ControlPoint[BeginPoint+j].north-MidData[i].north;
					d=sqrt(dx*dx+dy*dy);                  

					//寻找已经找到的控制点中距离最大者。
					n=0;
					for(k=0;k<ControlPointNeeded;k++){
						if(ControlPointNearest[k].Distance>ControlPointNearest[n].Distance){
							n=k;
						}
					}

					// 若此点距离小于已经找到的点的最大距离，则替换之。
					if(d<ControlPointNearest[n].Distance){			
							ControlPointNearest[n].Number=BeginPoint+j;
							ControlPointNearest[n].Distance=d;
					}
				}
				
				// 找出现有的ControlPointNearest中最大者, 
				// 若其小于等于该行与数据点的距离, 则不再继续搜索.
				BiggestDistance=0;
				for(j=0;j<ControlPointNeeded;j++){
					if(ControlPointNearest[j].Distance>BiggestDistance)BiggestDistance=ControlPointNearest[j].Distance;
				}
				if(BiggestDistance<=dx)BigDirectionOK=TRUE;
			}	
			
			// 改变行号.
			SmallRow--;
			BigRow++;
			if(SmallRow<0&&BigRow>RowNumber-1)break;
		}
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//               按照搜索得到的距离最近的几个控制点,对数据点进行控制.
		//	--------------------------------------------------------------------------------------------------------------------------------
		// 公式: V=[(L-L1)*V1+(L-L2)*V2+...+(L-Ln)*V2]/[L*(n-1)];
		// 其中:  Li是该点与第i个控制点的距离,Vi是第i个控制点的值,n是控制点的数目.
		//            L是该点距所有控制点的距离之和,
		TotalDistance=0;
		for(j=0;j<ControlPointNeeded;j++){
			TotalDistance+=ControlPointNearest[j].Distance;
		}

		MidDataChange=0;
		for(j=0;j<ControlPointNeeded;j++){
			MidDataChange+=
				ControlPoint[ControlPointNearest[j].Number].Value*
				(TotalDistance-ControlPointNearest[j].Distance);
		}
		if(TotalDistance!=0)   //若TotalDistance==0, 则各个Disatnce 全部为0。
			MidDataChange/=(ControlPointNeeded-1)*TotalDistance;

		MidData[i].Value+=MidDataChange;

	} // 对所有数据点循环.

	///////////////////////////////////////////////////////////
	// 返回.
	return TRUE;
	*/
