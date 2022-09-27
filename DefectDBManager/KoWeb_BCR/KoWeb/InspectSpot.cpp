#include "stdafx.h"
#include "KoWeb.h"

#include "KoWebDoc.h"
#include "KoWebView.h"

//@항목추가 6 : 해당항목 검사함수
void CKoWebView::InspectSpot(LPBYTE fm, int nSet, int nId, BOOL bShowGraphic)
{
	int i, j;
	int left			= g_Inspect[nSet].m_ROI1[nId].rc.left;
	int top				= g_Inspect[nSet].m_ROI1[nId].rc.top;
	int right			= g_Inspect[nSet].m_ROI1[nId].rc.right;
	int bottom			= g_Inspect[nSet].m_ROI1[nId].rc.bottom;
	int nOffsetX=0, nOffsetY=0;
	double PNewx, PNewy;
	CSpotVision	 spotVision;

	if(bShowGraphic) m_pImage->DrawRectangle(NULL, left, top, right, bottom, RGB(128,128,64));   //검사영역표시
	
	memset(&g_InspectResult[nId], 0, sizeof(INSPECTRESULT));									//결과 init
	g_InspectResult[nId].m_nInspectType=INSP_SPOT;												//Spot 검사임.

	spotVision.SpotInit(g_Inspect[nSet].m_InspSpot[nId].m_bUseMask, (right-left), (bottom-top)); //메모리 Alloc (spotVision.SpotClose와 세트)
	spotVision.SpotInspect(fm, g_System.m_nPitch, nSet, nId);									 //Spot 검사
	
	if(g_Inspect[nSet].m_InspSpot[nId].m_bShowBinary)											 //마지막 압축영상을 Binary로 보여줌
	{
		for(i=top;i<bottom;i++)
			for(j=left;j<right;j++)
				*(m_pImage->m_fmDisplay+g_System.m_nPitch*i+j)=*(spotVision.m_fmBinary+(right-left)*(i-top)+j-left);
	}

	spotVision.SpotClose();																		//메모리 Alloc 해제 (spotVision.SpotInit와 세트)
		
	if(bShowGraphic || g_Inspect[nSet].m_InspSpot[nId].m_bShowBinary) m_pImage->UpdateImage();

	if(bShowGraphic) ShowSpotInspectResult(nId);													//결과표시
}

//@항목추가 7: 해당항목 결과 Display
void CKoWebView::ShowSpotInspectResult(int nId)
{
	int i, k;
	CString str;

	if(g_InspectResult[nId].m_nInspectType!=INSP_SPOT) return;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	LOGFONT lf;
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 24;

	for(k=0;k<7;k++)
	{
		if(g_InspectResult[nId].m_nLevelCount[k]<=0) continue;

		for(i=0;i<g_InspectResult[nId].m_nLevelCount[k];i++)
		{
			switch(g_InspectResult[nId].m_nSpotNGType[k][i])
			{
				case 0:	m_pImage->DrawRectangle(NULL, g_InspectResult[nId].m_rcSpot[k][i].left, g_InspectResult[nId].m_rcSpot[k][i].top,g_InspectResult[nId].m_rcSpot[k][i].right, g_InspectResult[nId].m_rcSpot[k][i].bottom, RGB(255,0,0)); break;//RED
				case 1:	m_pImage->DrawRectangle(NULL, g_InspectResult[nId].m_rcSpot[k][i].left, g_InspectResult[nId].m_rcSpot[k][i].top,g_InspectResult[nId].m_rcSpot[k][i].right, g_InspectResult[nId].m_rcSpot[k][i].bottom, RGB(0,0,255)); break;//BLUE
				case 2:	m_pImage->DrawRectangle(NULL, g_InspectResult[nId].m_rcSpot[k][i].left, g_InspectResult[nId].m_rcSpot[k][i].top,g_InspectResult[nId].m_rcSpot[k][i].right, g_InspectResult[nId].m_rcSpot[k][i].bottom, RGB(255,255,0)); break;//YELLOW
			}
			str.Format(_T("Lv%d S:%.3lf V:%d"), k, g_InspectResult[nId].m_dSpotNGSize[k][i], g_InspectResult[nId].m_nSpotNGValue[k][i]);
			m_pImage->DrawTextABS(NULL,g_InspectResult[nId].m_rcSpot[k][i].right, g_InspectResult[nId].m_rcSpot[k][i].top, RGB(0,255,255), str, pDefaultGUIFont); 
		}
	}

}

/*
//fm : Binary 이미지
//fmGray: Gray이미지
//g_fmTemp : Dilate 이미지
int	CKoWebView::InspectSpot_Blob(int nSet, int nId, LPBYTE fm, LPBYTE fmGray, int nWidth, int nHeight, int nStartX, int nStartY,    bool bShowGraphic)
{
	int i, k;
	int nBlob=0, nValue;
	int ll, tt, rr, bb;
	int nOffX, nOffY;
	bool bump  =g_Inspect[nSet].m_InspSpot[nId].m_bBumpOnly;
	bool bBlack=g_Inspect[nSet].m_InspSpot[nId].m_bBlack;
	bool bWhite=g_Inspect[nSet].m_InspSpot[nId].m_bWhite;
	int nBValue=g_Inspect[nSet].m_InspSpot[nId].m_nBValue[0];
	int nWValue=g_Inspect[nSet].m_InspSpot[nId].m_nWValue[0];
	double dWMin;
	double dWMax; 
	double dBMin;
	double dBMax; 
	int nBlobAddDist=2;
	double dMaxRatio=100;
	double dMinDefectArea=1;
	int nKernel;
	double PNewx, PNewy;
	double dMin, dMax, dSize;
	double dArea, dCheckArea;
	double pos[4][2], dist1, dist2, angle, cx, cy, dRatio;
	int nSizeX, nSizeY;
	int nX1[500], nY1[500];
	int nX2[500], nY2[500], nValueA[500];
	int  nCnt=0;
	int nMax[4], NN, nMaxSize;
	CString str;
	LPBYTE fmT;
	CChain chain(2000, 200000);

	nKernel=nBlobAddDist/2;
	if(nBValue>0) nBValue*=-1;
	if(nWValue<0) nWValue*=-1;

	//Dilate---------------------------------------------------------------------------------
	if(nKernel<1) nKernel=1;
	BinaryDilate(fm, g_fmTemp, 0,0, nWidth-1, nHeight-1,nWidth, 2*nKernel+1, 2*nKernel+1);
	fmT=g_fmTemp;
	//---------------------------------------------------------------------------------------

	chain.SetChainData(1, fmT, 2, 2, 4, 20000, nWidth, nHeight); 
	nBlob=chain.FastChain(1,1,nWidth-1, nHeight-1);

	for(i=0;i<nBlob;i++)
	{
		//Size 범위외 버림--------------------------------------------------------------------------------------------
		ll	=chain.FindMinX(i);
		tt	=chain.FindMinY(i);
		rr	=chain.FindMaxX(i);
		bb	=chain.FindMaxY(i);
		dSize=((rr-ll+1-2*nKernel)*g_InspectSys[nSet].m_dScaleFactorX+(bb-tt+1-2*nKernel)*g_InspectSys[nSet].m_dScaleFactorY)/2.0;		

		//Value 기준미달 버림--------------------------------------------------------------------------------------------
		if(bump)
		{
			nValue=GetBumpValue(fm, g_fmTemp, fmGray, ll,tt,rr,bb, nWidth);
			if(nValue<-nBValue)				continue;   //검정색을 사용한다.
			if(dSize<dBMin || dSize>dBMax)  continue; 
		}
		else
		{
			nValue=GetInValue(fm, g_fmTemp, fmGray, ll,tt,rr,bb, nWidth);
			if(nValue>0)
			{
				if(!bWhite || nValue<nWValue)  continue;
				if(dSize<dWMin || dSize>dWMax) continue; 
			}
			else
			{
				if(!bBlack || nValue>nBValue) continue;
				if(dSize<dBMin || dSize>dBMax) continue; 
			}
		}
		//slithor 관련 bValue 큰것 버림------------------------------------------------------------------------------
		if(nBValueMax !=0 && nValue*-1 > nBValueMax) //이물 빼기. 설정값 두배 이상일 시는 뺌 (표면 이물)
		{
			continue;
		}
		//(장변)/(단변) 이 너무 큰 것 버림.---------------------------------------------------------------------------
		angle=chain.FindAngle(i);
		chain.Chain_Center(i, &cx, &cy);
		chain.FindMinRect(i, cx, cy, angle,  pos);
		dist1=sqrt((pos[0][0]-pos[1][0])*(pos[0][0]-pos[1][0])+(pos[0][1]-pos[1][1])*(pos[0][1]-pos[1][1]))-2*nKernel;
		dist2=sqrt((pos[0][0]-pos[3][0])*(pos[0][0]-pos[3][0])+(pos[0][1]-pos[3][1])*(pos[0][1]-pos[3][1]))-2*nKernel;
		if(dist1<=1) dist1 =1;
		if(dist2<=1) dist2 =1;
		if(dist1<dist2) dRatio=dist2/dist1;
		else            dRatio=dist1/dist2;
		if(dRatio>dMaxRatio) continue;  
		//==============================================================================================================
		dist1 = rr-ll;
		dist2 = bb-tt;
		if(dist1<dist2) dRatio=dist2/dist1;
		else            dRatio=dist1/dist2;
		if(dRatio>dMaxRatio) continue;  

		//면적 비율 작은것 버림 {흑색 불량만 적용}----------------------------------------------------------------------------------------
		if(((rr-ll)*(bb-tt)) >0 && bBlack)
		{
			dCheckArea = chain.Chain_Area(i)/((rr-ll)*(bb-tt)) *100;
			if(dCheckArea<dMinDefectArea) continue;  
		}
		//--------------------------------------------------------------------------------------------------------------
		if(nCnt<500)  //최대 500개만 구함.
		{
			nX1[nCnt]=ll+nStartX+nKernel;
			nY1[nCnt]=tt+nStartY+nKernel;
			nX2[nCnt]=rr+nStartX-nKernel;
			nY2[nCnt]=bb+nStartY-nKernel;
			nValueA[nCnt]=nValue;
			nCnt++;
		}
	}

	for(i=0;i<nCnt;i++)
	{
		//g_AreaInspect 에 검사결과 넣는다.

		if(bShowGraphic)
		{			
			if(nValueA[i]>0) m_pDisplay[g_Temp.m_nDisplay]->DrawRectangle(NULL, nX1[i], nY1[i], nX2[i], nY2[i], RGB(255,127,0));
			else             m_pDisplay[g_Temp.m_nDisplay]->DrawRectangle(NULL, nX1[i], nY1[i], nX2[i], nY2[i], RGB(0,127,255));

			if(g_Temp.m_bShowResultText)
			{
				str.Format(_T("S:(%.2lf,%.2lf) V:%d"), (nX2[i]-nX1[i]+1)*g_Config.m_dScaleFactorX[0], (nY2[i]-nY1[i]+1)*g_Config.m_dScaleFactorY[0], nValueA[i]);
				m_pDisplay[g_Temp.m_nDisplay]->DrawTextABS(NULL, nX1[i], nY2[i], RGB(0,255,0), str);						
			}
		}
	}

	nMax[0]=nMax[1]=nMax[2]=nMax[3]=-1;
	for(k=0;k<4;k++)
	{
		nMaxSize=0;
		for(i=0;i<nCnt;i++)
		{
			if(i==nMax[0] || i==nMax[1] || i==nMax[2] || i==nMax[3]) continue;

			if( (nX2[i]-nX1[i])+(nY2[i]-nY1[i]) >nMaxSize)
			{
				nMaxSize=(nX2[i]-nX1[i])+(nY2[i]-nY1[i]);
				nMax[k]=i;
			}
		}
		if(nMax[k]>=0) //제일 큰 순서로 4개만 찾는다.
		{
			NN=nMax[k];
			if(g_AreaInspect.m_nDustCnt<4)
			{
				g_AreaInspect.m_nPosX[g_AreaInspect.m_nDustCnt]=(nX1[NN]+nX2[NN])/2;
				g_AreaInspect.m_nPosY[g_AreaInspect.m_nDustCnt]=(nY1[NN]+nY2[NN])/2;				
				g_AreaInspect.m_nDefectType[g_AreaInspect.m_nDustCnt]=1; //수정해야함
				g_AreaInspect.m_dDustSizeX[g_AreaInspect.m_nDustCnt]=(nX2[NN]-nX1[NN]+1)*g_Config.m_dScaleFactorX[0];
				g_AreaInspect.m_dDustSizeY[g_AreaInspect.m_nDustCnt]=(nY2[NN]-nY1[NN]+1)*g_Config.m_dScaleFactorY[0];
				g_AreaInspect.m_dDustSize[g_AreaInspect.m_nDustCnt]=(g_AreaInspect.m_dDustSizeX[g_AreaInspect.m_nDustCnt]+g_AreaInspect.m_dDustSizeY[g_AreaInspect.m_nDustCnt])/2.0;
				g_AreaInspect.m_nDustValue[g_AreaInspect.m_nDustCnt]=nValueA[NN];

				if(bShowGraphic)
				{
					m_pDisplay[g_Temp.m_nDisplay]->DrawRectangle(NULL, nX1[NN], nY1[NN], nX2[NN], nY2[NN], RGB(255,0,0));
					if(g_Temp.m_bShowResultText)
					{
						str.Format(_T("S:(%.2lf,%.2lf) V:%d"), g_AreaInspect.m_dDustSizeX[g_AreaInspect.m_nDustCnt], g_AreaInspect.m_dDustSizeY[g_AreaInspect.m_nDustCnt], nValueA[NN]);
						m_pDisplay[g_Temp.m_nDisplay]->DrawTextABS(NULL, nX1[NN], nY2[NN], RGB(0,255,0), str);
					}
				}
				g_AreaInspect.m_nDustCnt++;
			}
		}
	}

	return nBlob;
}
*/