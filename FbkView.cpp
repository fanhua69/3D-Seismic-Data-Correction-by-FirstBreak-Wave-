// FbkView.cpp : implementation file
//

#include "stdafx.h"
#include "afxcmn.h"
#include "3dfstbrk.h"
#include "FbkDoc.h"
#include "FbkView.h"
#include "childfrm.h"
#include "posvalue.h"
#include "fh.h"
#include "math.h"
#include "relation.h"
#include "ValueFbk.h"

#include "FbkInFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFbkView

IMPLEMENT_DYNCREATE(CFbkView, CFHZoomView)

CFbkView::CFbkView()
{
	m_dFatScale=0.2;

	m_AbFbk=NULL;  // 判断某道是否初至错误。
	m_AbFbkLast=NULL;  // 判断某道是否初至错误。
	
	m_GroupChanged=NULL;  // 用于鼠标左键按下。

	m_UnconGroup=NULL;  // 不连续道。
	m_nUnconGroup=0;  // 不连续道数目。

	m_DrawCurve=true;
	m_DrawFbk=true;

	m_penFbk.CreatePen(PS_COSMETIC,2,RGB(255,0,0));
	m_penGroup.CreatePen(PS_COSMETIC,1,RGB(0,0,0));
	m_penSelection.CreatePen(PS_COSMETIC,1,RGB(0,255,0));
	m_penWrongFbk.CreatePen(PS_COSMETIC,1,RGB(255,0,0));
	m_penCleaner.CreatePen(PS_SOLID,1,RGB(90,90,90));

	m_nPenLineFBK=3;	
	m_penLineFBK[0].CreatePen(PS_SOLID,0,RGB(255,0,0)); 
	m_penLineFBK[1].CreatePen(PS_SOLID,0,RGB(0,255,0)); 
	m_penLineFBK[2].CreatePen(PS_SOLID,0,RGB(0,0,255)); 

	this->m_bCheckingFBK =false;
	m_bAreaShow=true;
	m_bDrawPartGroup=false;

	// Related with modifying the first break manually;
	m_bAnchor=false;
	m_bModifyingFbk=false;
	
	SetDirection(false);

}


CFbkView::~CFbkView()
{
	Reset();

}


BEGIN_MESSAGE_MAP(CFbkView, CFHZoomView)
	//{{AFX_MSG_MAP(CFbkView)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_FBDrawCurve, OnFBDrawCurve)
	ON_COMMAND(ID_FBDrawFbk, OnFBDrawFbk)					 
	ON_COMMAND(ID_Fbk_CheckFBK, OnFbkCheckFBK)
	ON_UPDATE_COMMAND_UI(ID_Fbk_CheckFBK, OnUpdateFbkCheckFBK)
	ON_COMMAND(ID_Fbk_AreaShow, OnFbkAreaShow)
	ON_UPDATE_COMMAND_UI(ID_Fbk_AreaShow, OnUpdateFbkAreaShow)
	ON_COMMAND(ID_FstBrk_DrawPart, OnFstBrkDrawPart)
	ON_UPDATE_COMMAND_UI(ID_FstBrk_DrawPart, OnUpdateFstBrkDrawPart)
	ON_COMMAND(ID_FBThinner, OnFBThinner)
	ON_COMMAND(ID_FBFatter, OnFBFatter)
	ON_COMMAND(ID_FB_Modify, OnFBModify)
	ON_UPDATE_COMMAND_UI(ID_FB_Modify, OnUpdateFBModify)
	ON_WM_MOUSEMOVE()
	ON_WM_HSCROLL()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CFbkView::OnDraw(CDC* pDC)
{

	CFbkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->m_GroupNumber==0)return;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  当系由于系统改变而调用了OnDraw函数时，所有道全画。
	int i,j;	
	for(i=0;i<pDoc->m_GroupNumber;i++){
		m_GroupChanged[i]=TRUE;
	}

	if(!m_bCheckingFBK){
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//    绘制波形图. 
		if(m_DrawCurve){
			DrawGroup(pDC);
		}
		
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//  绘制初至.					
		if(m_DrawFbk){
			CPen *OldPen=pDC->SelectObject(&m_penFbk);
			//pDC->SetROP2(R2_XORPEN);

			double x,y;
			for(i=0;i<pDoc->m_GroupNumber;i++){

				// 绘制当前初至。
				x=i-0.5;
				y=pDoc->m_pDataInFstBrkFile[i].FirstBreak;
				LPtoWorld(x,y);				
				pDC->MoveTo(x,y);

				x=i+0.5;
				y=pDoc->m_pDataInFstBrkFile[i].FirstBreak;
				LPtoWorld(x,y);				
				pDC->LineTo(x,y);
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//  复原原来的笔。
			pDC->SelectObject(OldPen);

		} 
	}
	else{
		CPen *OldPen=pDC->SelectObject(&m_penFbk);
	
		double x,y;
		int nPen,nStart,nEnd;
		for(i=0;i<pDoc->m_nGroupLine ;i++){
			nPen=i%m_nPenLineFBK;

			pDC->SelectObject(&m_penLineFBK[nPen]);
			nStart=pDoc->m_GroupLine [i].nStartGroup;
			nEnd=pDoc->m_GroupLine [i].nEndGroup;

			x=nStart;
			y=pDoc->m_pDataInFstBrkFile[nStart].FirstBreak;
			LPtoWorld(x,y);
			pDC->MoveTo(x,y);

			for(j=nStart;j<=nEnd;j++){

				// 绘制当前初至。
				x=j;
				y=pDoc->m_pDataInFstBrkFile[j].FirstBreak;
				LPtoWorld(x,y);			
				pDC->LineTo(x,y);
			}
		}

		int dRadius=4;
		for(i=0;i<pDoc->m_nGroupLine ;i++){
			nPen=i%m_nPenLineFBK;

			pDC->SelectObject(&m_penLineFBK[nPen]);
			nStart=pDoc->m_GroupLine [i].nStartGroup;
			nEnd=pDoc->m_GroupLine [i].nEndGroup;

			for(j=nStart;j<=nEnd;j++){

				x=j;
				y=pDoc->m_pDataInFstBrkFile[j].FirstBreak;
				LPtoWorld(x,y);			
				pDC->Ellipse(x-dRadius,y-dRadius,x+dRadius,y+dRadius);
			}
		}




		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//  复原原来的笔。
		pDC->SelectObject(OldPen);
	}

	if(m_bModifyingFbk){
		CPen pen;
		pen.CreatePen (PS_SOLID,0,RGB(255,0,0));
		CPen *pOldPen=pDC->SelectObject(&pen);
		double x,y;
		x=m_pointHead.x;
		y=m_pointHead.y;
		LPtoWorld(x,y);
		pDC->MoveTo(x,y);

		x=m_pointTail.x;
		y=m_pointTail.y;		
		LPtoWorld(x,y);
		pDC->LineTo(x,y);
		pDC->SelectObject(pOldPen);
	}
	
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CFbkView diagnostics

#ifdef _DEBUG
void CFbkView::AssertValid() const
{
	CFHZoomView::AssertValid();
}

void CFbkView::Dump(CDumpContext& dc) const
{
	CFHZoomView::Dump(dc);
}
#endif //_DEBUG

void CFbkView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(GetZoomStatue()!=statueIdle){
		CFHZoomView::OnLButtonDown(nFlags, point) ;
	}
	else if(m_bModifyingFbk){
		long nPointedGroup;
		double dPointedTime;
		CalcMousePos(point,nPointedGroup,dPointedTime);

		m_bAnchor=true;
		m_pointTail.x=m_pointHead.x=nPointedGroup;
		m_pointTail.y=m_pointHead.y=dPointedTime;
		SetCapture();

		Invalidate();

	}
	else{

		CFbkDoc* pDoc =GetDocument();
		ASSERT_VALID(pDoc);

		for(int i=0;i<pDoc->m_GroupNumber;i++){
			m_GroupChanged[i]=FALSE;
		}

		long nPointedGroup;
		double dPointedTime;
		CalcMousePos(point,nPointedGroup,dPointedTime);
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//   状态:  用户按左键的同时按了CTRL键.
		//   转换该道的选择状态.
		if(nFlags==(MK_CONTROL+MK_LBUTTON)){
			pDoc->m_pSelectedGroups[nPointedGroup]=!pDoc->m_pSelectedGroups[nPointedGroup];
			m_GroupChanged[nPointedGroup]=TRUE;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//   状态:  用户按左键的同时按了SHIFT键.
		//   设置该道的选择状态.
		else if(nFlags==(MK_SHIFT+MK_LBUTTON)){
			if(pDoc->m_BeginSelectedGroup!=pDoc->m_EndSelectedGroup){
				pDoc->m_BeginSelectedGroup=nPointedGroup;
				pDoc->m_EndSelectedGroup=nPointedGroup;}
			else{
				pDoc->m_EndSelectedGroup=nPointedGroup;
				if(pDoc->m_BeginSelectedGroup>pDoc->m_EndSelectedGroup){
					Swap(&pDoc->m_BeginSelectedGroup,&pDoc->m_EndSelectedGroup);
				}
			}

			// 设置选择标志, 不会出现m_BeginSelectedGroup==-2的情况.
			for(i=pDoc->m_BeginSelectedGroup;i<=pDoc->m_EndSelectedGroup;i++){
				pDoc->m_pSelectedGroups[i]=TRUE;
				m_GroupChanged[i]=TRUE;
			}
		}		

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//   状态:  用户只按了左键
		//     1. 用户放弃选择道, 2. 用户改变初至.同时处理之.
		else{ 
			// 取消所有道的选择标志.
			pDoc->m_BeginSelectedGroup=-2;
			pDoc->m_EndSelectedGroup=-1;
			for(i=0;i<pDoc->m_GroupNumber;i++){
				if(pDoc->m_pSelectedGroups[i]){
					pDoc->m_pSelectedGroups[i]=FALSE;
					m_GroupChanged[i]=TRUE;
				}
			}

			m_GroupChanged[nPointedGroup]=TRUE;

			// 改变初至。
			pDoc->m_pDataInFstBrkFile[nPointedGroup].FirstBreak=dPointedTime;

		}

		
		//InvalidateRect(CRect(point.x-10,point.y-100,point.x+10,point.y+100));
		Invalidate();

	}

	return ;
}											 

//             BOOL CFbkView::DealLButtonDown                    END	
/////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CFbkDoc* CFbkView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFbkDoc)));
	return (CFbkDoc*)m_pDocument;
}


BOOL CFbkView::CheckFirstBreak()
{
	CFbkDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);
					 	
	long i,j,n;
	float Sti,Sti2,Stiyi,Syi;
	float a,b;
	float y;  // 存储正确位置。
																			  	
	// 若某道不正常，则该道的m_AbFbk为真。
	for(i=0;i<pDoc->m_GroupNumber;i++){
		m_AbFbk[i]=FALSE;
	}

	//  对道组循环：
	for(i=0;i<m_nUnconGroup;i++){

		//  求拟合直线的y=ax+b的a,b值：
		Sti=Sti2=Stiyi=Syi=0;
		for(j=m_UnconGroup[i];j<m_UnconGroup[i+1];j++){
			Sti+=j;
			Sti2+=j*j;
			Syi+=pDoc->m_pDataInFstBrkFile[j].FirstBreak;
			Stiyi+=pDoc->m_pDataInFstBrkFile[j].FirstBreak*j;
		}
		n=m_UnconGroup[i+1]-m_UnconGroup[i];
		
		a=(Syi/n-Stiyi/Sti)/(Sti/n-Sti2/Sti);
		b=(Syi-Sti*a)/n;

		// 	求取不正常初至道：
		for(j=m_UnconGroup[i];j<m_UnconGroup[i+1];j++){
			y=a*j+b;
			if(fabs(pDoc->m_pDataInFstBrkFile[j].FirstBreak-y)>200)m_AbFbk[j]=TRUE;
		}

	}  // 对道组循环。
	return TRUE;
}
//  BOOL CFbkView::CheckFirstBreak()
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
BOOL CFbkView::DrawGroup(CDC *pDC)
{
	CFbkDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	double x1,y1,x2,y2;
	GetSeenArea(x1,y1,x2,y2);

	// 获得应画的范围－道数和点数：
	long BeginGroup=x1;
	long BeginPoint=y1/pDoc->m_UnformTimeInterval ;
	long EndGroup=x2+1;
	long EndPoint=y2/pDoc->m_UnformTimeInterval+2 ;
	if(BeginGroup<0)BeginGroup=0;
	if(BeginPoint<0)BeginPoint=0;
	if(EndPoint>=pDoc->m_PointNumberOfOneGroup )
		EndPoint=pDoc->m_PointNumberOfOneGroup;
	if(EndGroup>=pDoc->m_GroupNumber)
		EndGroup=pDoc->m_GroupNumber-1;
	long TotalPoint=EndPoint-BeginPoint;

	// 绘制：
	CPen *OldPen=pDC->SelectObject(&m_penGroup);
	long i,j,p=0;
	
	for(i=0;i<pDoc->m_GroupNumber;i++)m_AbFbkLast[i]=m_AbFbk[i];
	CheckFirstBreak();

	double x,y,Extent;
		
	for(i=BeginGroup;i<=EndGroup;i++){
		
		//if(!m_GroupChanged[i]&&(m_AbFbkLast[i]==m_AbFbk[i]))continue;

		p=i*pDoc->m_DataNumberOfOneGroup+pDoc->m_UnformGroupHead;

		if(m_bDrawPartGroup){ // Draw the part of group that is around the first break;
			y=pDoc->m_pDataInFstBrkFile[i].FirstBreak/pDoc->m_UnformTimeInterval;
			BeginPoint=y-50;
			EndPoint=y+50;
			if(BeginPoint<0)BeginPoint=0;
			if(EndPoint>=pDoc->m_PointNumberOfOneGroup)
				EndPoint=pDoc->m_PointNumberOfOneGroup-1;
			TotalPoint=EndPoint-BeginPoint;
		}


		for(j=BeginPoint;j<EndPoint;j++){
			Extent=pDoc->m_pGroupData[p+j]*m_dFatScale;

			if(Extent>0.5)Extent=0.5;
			if(Extent<-0.5)Extent=-0.5;

			x=i+Extent;
			y=j*pDoc->m_UnformTimeInterval ;
			LPtoWorld(x,y);

			pDoc->m_pPointOfOneGroup[j].x=x;
			pDoc->m_pPointOfOneGroup[j].y=y;
		}

		//  根据不同情况选择不同的笔。
		if(pDoc->m_pSelectedGroups[i])
			pDC->SelectObject(&m_penSelection);
		else if(m_AbFbk[i])
			pDC->SelectObject(&m_penWrongFbk);
		else
			pDC->SelectObject(&m_penGroup);

		// 绘制该道。
		pDC->Polyline(&pDoc->m_pPointOfOneGroup[BeginPoint],TotalPoint);
		
		if(m_bAreaShow){
			for(int j=BeginPoint;j<BeginPoint+TotalPoint;j++){
				if(pDoc->m_pGroupData[p+j]>0){
					x=i;
					y=j*pDoc->m_UnformTimeInterval ;
					LPtoWorld(x,y);
					pDC->MoveTo(x,y);

					Extent=pDoc->m_pGroupData[p+j]*m_dFatScale;
					if(Extent>0.5)Extent=0.5;
					if(Extent<-0.5)Extent=-0.5;

					x=i+Extent;
					y=j*pDoc->m_UnformTimeInterval ;
					LPtoWorld(x,y);
					pDC->LineTo(x,y);
				}
			}
		}

	}
								

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  复原原来的笔。
	pDC->SelectObject(OldPen);

	return TRUE;

}

void CFbkView::OnFBDrawCurve() 
{
   m_DrawCurve=!m_DrawCurve;
	   	
	CMenu *menu=AfxGetMainWnd()->GetMenu();
	if(m_DrawCurve)
		menu->CheckMenuItem( ID_FBDrawCurve, MF_CHECKED);
	else
		menu->CheckMenuItem( ID_FBDrawCurve, MF_UNCHECKED);

	Invalidate();

}

void CFbkView::OnFBDrawFbk() 
{
	
	m_DrawFbk=!m_DrawFbk;
	
	CMenu *menu=AfxGetMainWnd()->GetMenu();
	if(m_DrawFbk)
		menu->CheckMenuItem( ID_FBDrawFbk, MF_CHECKED);
	else
		menu->CheckMenuItem( ID_FBDrawFbk, MF_UNCHECKED);

	Invalidate();
	
	
}



void CFbkView::Reset()
{
	if(m_AbFbk)delete []m_AbFbk;
	if(m_AbFbkLast)delete []m_AbFbkLast;
	if(m_UnconGroup)delete []m_UnconGroup;
	if(m_GroupChanged)delete []m_GroupChanged;
	
	m_AbFbk=NULL;
	m_AbFbkLast=NULL;
	m_UnconGroup=NULL;
	m_GroupChanged=NULL;
}

void CFbkView::OnFbkCheckFBK() 
{
	// TODO: Add your command handler code here
	m_bCheckingFBK=!m_bCheckingFBK;
	Invalidate();
	
}

void CFbkView::OnUpdateFbkCheckFBK(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (m_bCheckingFBK);
	
}

void CFbkView::OnFbkAreaShow() 
{
	// TODO: Add your command handler code here
	m_bAreaShow=!m_bAreaShow;
	Invalidate();
	
}

void CFbkView::OnUpdateFbkAreaShow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (m_bAreaShow);
	
}


void CFbkView::OnFstBrkDrawPart() 
{
	// TODO: Add your command handler code here
	m_bDrawPartGroup=!m_bDrawPartGroup;
	Invalidate();
	
}

void CFbkView::OnUpdateFstBrkDrawPart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (m_bDrawPartGroup);
	
}

void CFbkView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class	
	
	CFbkDoc *pDoc=GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->m_GroupNumber==0)return;

	SetDrawRange(-5,-100,pDoc->m_GroupNumber+5,pDoc->m_UnformTimeLength+100,10,0.1);
	
	/////////////////////////////////////////////////////////
	//  Allocate memory:
	Reset();

	m_AbFbk=new BOOL[pDoc->m_GroupNumber];
	m_AbFbkLast=new BOOL[pDoc->m_GroupNumber];
	m_UnconGroup=new long[pDoc->m_GroupNumber];
	m_nUnconGroup=0;
	m_GroupChanged=new BOOL[pDoc->m_GroupNumber];

	for(int i=0;i<pDoc->m_GroupNumber;i++){
		m_GroupChanged[i]=TRUE;
		m_AbFbk[i]=FALSE;
		m_AbFbkLast[i]=FALSE;
	}

	
	// 做这项工作是为检查初至时计算方便。
	m_UnconGroup[m_nUnconGroup]=pDoc->m_GroupNumber;

	CFHZoomView::OnUpdate(pSender, lHint, pHint) ;
	return ;

	


}

enumZoomStatue CFbkView::GetZoomStatue()
{
	return m_statueZoom;
}

void CFbkView::OnFBThinner() 
{
	// TODO: Add your command handler code here
	m_dFatScale/=1.2;	
	Invalidate();
}

void CFbkView::OnFBFatter() 
{
	// TODO: Add your command handler code here
	m_dFatScale*=1.2;
	Invalidate();
}

CString CFbkView::GetInfo(double x, double y)
{
	CFbkDoc *pDoc=GetDocument();
	CString s;
	if(pDoc->m_pShotMsg==NULL)return CString("");
	x+=0.5;
	s.Format("File Number: %ld, Time:%1.1lf, Group Name:%ld",pDoc->m_pShotMsg[pDoc->m_CurrShot].FileNumber ,y,pDoc->GetGroupHead(x,17));
	return s;


}

void CFbkView::OnFBModify() 
{
	// TODO: Add your command handler code here
	m_bModifyingFbk=!m_bModifyingFbk;

}

void CFbkView::OnUpdateFBModify(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (m_bModifyingFbk);	
}

void CFbkView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bAnchor){
		long nPointedGroup;
		double dPointedTime;
		CalcMousePos(point,nPointedGroup,dPointedTime);

		m_pointTail.x=nPointedGroup;
		m_pointTail.y=dPointedTime;

		Invalidate();
	}

	
	CFHZoomView::OnMouseMove(nFlags, point);
}

void CFbkView::CalcMousePos(CPoint point,long &nPointedGroup,double &dPointedTime)
{
			
	CPoint pointOrg=GetDeviceScrollPosition();
	point+=pointOrg;
	double x=point.x;
	double y=point.y;
	WorldtoLP(x,y);
	nPointedGroup=long(x+0.5);
	dPointedTime=y;

	CFbkDoc *pDoc=GetDocument();
	if(nPointedGroup<0)nPointedGroup=0;
	if(nPointedGroup>=pDoc->m_GroupNumber )nPointedGroup=pDoc->m_GroupNumber-1 ;
	if(dPointedTime<0)dPointedTime=0;
	if(dPointedTime>pDoc->m_UnformTimeLength)dPointedTime=pDoc->m_UnformTimeLength;

}

void CFbkView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bAnchor){
		long nPointedGroup;
		double dPointedTime;
		CalcMousePos(point,nPointedGroup,dPointedTime);

		m_pointTail.x=nPointedGroup;
		m_pointTail.y=dPointedTime;

		CFbkDoc *pDoc=GetDocument();
		pDoc->CalcFbkInteractively(m_pointHead,m_pointTail);

		Invalidate();
		ReleaseCapture();
		m_bAnchor=false;
	}

	
	CFHZoomView::OnLButtonUp(nFlags, point);
}


void CFbkView::GotoNextPage()
{
	CFbkDoc *pDoc=GetDocument();
	pDoc->OnFBNextPage ();

}
void CFbkView::GotoPrevPage()
{
	CFbkDoc *pDoc=GetDocument();
	pDoc->OnFBLastPage ();
}
