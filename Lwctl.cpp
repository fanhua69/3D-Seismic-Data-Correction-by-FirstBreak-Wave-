
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
	// ��ȡ�������������õ��ľ�У�����ļ�.
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
	// �洢���վ�У���ɹ�.
	fileOutFbk.WriteRcv ();
	fileOutFbk.WriteShot();

	AfxMessageBox("�������������!");
	
	return TRUE;

}





	/*
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//              �����е��м�ɹ���, ������������Ŀ��Ƶ�.
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
	dlg.SetWindowText("���ڶ��м�ɹ�����п���");
	dlg.SetStatus("�Ѿ�������");
	
	
	for(i=0;i<MidDataNumber;i++){   
		
		//  ���ý�������
		if(i%10==0)dlg.SetPos((float)i/MidDataNumber*100);

		// ���м�ɹ�ֵ������Χ�� ��õ���δ���Ƶ㣬 
		// ��ֵӦ�����û����ݡ�
		if(MidData[i].Value<-5000){
			n=SearchMatchValue(MidData[i].Ph,InputData,InputDataNumber);
			if(InputData[n].Ph!=MidData[i].Ph){
				AfxMessageBox("�м�ɹ����ݵ㣨׮��Ϊ"+STR(MidData[i].Ph)+"����\n\
					��׮�����û�������������û���ҵ����޷�������");
				return FALSE;
			}
			MidData[i].Value=InputData[n].Value;
			continue;
		}
		
		// Ѱ�Ҿ�������ݵ������һ��.
		SmallRow=SearchForControlRow(MidData[i].east,ControlPoint,ControlPointNumber,RowPosition,RowNumber);
		BigRow=SmallRow+1;
		
		// �ȶ�ControlPointNearest���ϴ�ֵ, �Ա��滻֮��.
		for(j=0;j<ControlPointNeeded;j++){
			ControlPointNearest[j].Number=0;
			ControlPointNearest[j].Distance=10000000;
		}
		
		// �ִ�С����������Ƶ�, ��������ļ������Ƶ�, ����ControlPointNearest.
		SmallDirectionOK=FALSE;
		BigDirectionOK=FALSE;
		while(!SmallDirectionOK&&!BigDirectionOK){
			
			// ���СX���귽���һ��.
			if(SmallRow>=0&&!SmallDirectionOK){

				// ����һ�е��������ݼ��һ��, 
				//������н��Ѿ��ҵ��Ŀ��Ƶ�ΪС���滻ControlPointNearest��
				BeginPoint=RowPosition[SmallRow];
				dx=fabs(ControlPoint[BeginPoint].east-MidData[i].east);  // ��Ҫfabs��
				for(j=0;j<NumInRow[SmallRow];j++){	//�Ը����еĿ��Ƶ�ѭ��.

					//����õ�ľ���.
					dy=ControlPoint[BeginPoint+j].north-MidData[i].north;
					d=sqrt(dx*dx+dy*dy);                  

					//Ѱ���Ѿ��ҵ��Ŀ��Ƶ��о�������ߡ�
					n=0;
					for(k=0;k<ControlPointNeeded;k++){
						if(ControlPointNearest[k].Distance>ControlPointNearest[n].Distance){
							n=k;
						}
					}

					// ���˵����С���Ѿ��ҵ��ĵ�������룬���滻֮��
					if(d<ControlPointNearest[n].Distance){			
							ControlPointNearest[n].Number=BeginPoint+j;
							ControlPointNearest[n].Distance=d;
					}
				}

				// �ҳ����е�ControlPointNearest�������, 
				// ����С�ڵ��ڸ��������ݵ�ľ���, ���ټ�������.
				BiggestDistance=0;
				for(j=0;j<ControlPointNeeded;j++){
					if(ControlPointNearest[j].Distance>BiggestDistance)BiggestDistance=ControlPointNearest[j].Distance;
				}
				if(BiggestDistance<=dx)SmallDirectionOK=TRUE;
			}

			// ����X���귽���һ��.
			if(BigRow<RowNumber&&!BigDirectionOK){

				// ����һ�е��������ݼ��һ��, 
				//������н�С���滻ControlPointNearest�еĿ��Ƶ�.
				BeginPoint=RowPosition[BigRow];
				dx=ControlPoint[BeginPoint].east-MidData[i].east;
				for(j=0;j<NumInRow[BigRow];j++){
				
					//����õ�ľ���.
					dy=ControlPoint[BeginPoint+j].north-MidData[i].north;
					d=sqrt(dx*dx+dy*dy);                  

					//Ѱ���Ѿ��ҵ��Ŀ��Ƶ��о�������ߡ�
					n=0;
					for(k=0;k<ControlPointNeeded;k++){
						if(ControlPointNearest[k].Distance>ControlPointNearest[n].Distance){
							n=k;
						}
					}

					// ���˵����С���Ѿ��ҵ��ĵ�������룬���滻֮��
					if(d<ControlPointNearest[n].Distance){			
							ControlPointNearest[n].Number=BeginPoint+j;
							ControlPointNearest[n].Distance=d;
					}
				}
				
				// �ҳ����е�ControlPointNearest�������, 
				// ����С�ڵ��ڸ��������ݵ�ľ���, ���ټ�������.
				BiggestDistance=0;
				for(j=0;j<ControlPointNeeded;j++){
					if(ControlPointNearest[j].Distance>BiggestDistance)BiggestDistance=ControlPointNearest[j].Distance;
				}
				if(BiggestDistance<=dx)BigDirectionOK=TRUE;
			}	
			
			// �ı��к�.
			SmallRow--;
			BigRow++;
			if(SmallRow<0&&BigRow>RowNumber-1)break;
		}
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//               ���������õ��ľ�������ļ������Ƶ�,�����ݵ���п���.
		//	--------------------------------------------------------------------------------------------------------------------------------
		// ��ʽ: V=[(L-L1)*V1+(L-L2)*V2+...+(L-Ln)*V2]/[L*(n-1)];
		// ����:  Li�Ǹõ����i�����Ƶ�ľ���,Vi�ǵ�i�����Ƶ��ֵ,n�ǿ��Ƶ����Ŀ.
		//            L�Ǹõ�����п��Ƶ�ľ���֮��,
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
		if(TotalDistance!=0)   //��TotalDistance==0, �����Disatnce ȫ��Ϊ0��
			MidDataChange/=(ControlPointNeeded-1)*TotalDistance;

		MidData[i].Value+=MidDataChange;

	} // ���������ݵ�ѭ��.

	///////////////////////////////////////////////////////////
	// ����.
	return TRUE;
	*/
