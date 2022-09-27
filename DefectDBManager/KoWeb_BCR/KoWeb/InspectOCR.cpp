#include "stdafx.h"
#include "KoWeb.h"

#include "KoWebDoc.h"
#include "KoWebView.h"

//Char중심 찾음
int FindCharCenter(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int  nCharSizeX, int *pCx);

void CKoWebView::InspectOCR(LPBYTE fm, int nSet, int nId, BOOL bShowGraphic)
{
	int i, j;
	int left			= g_Inspect[nSet].m_ROI1[nId].rc.left;
	int top				= g_Inspect[nSet].m_ROI1[nId].rc.top;
	int right			= g_Inspect[nSet].m_ROI1[nId].rc.right;
	int bottom			= g_Inspect[nSet].m_ROI1[nId].rc.bottom;
	int nSizeX			= g_Inspect[nSet].m_InspOCR[nId].m_nSizeX;
	int nSizeY			= g_Inspect[nSet].m_InspOCR[nId].m_nSizeY;
	int nCharNum		= g_Inspect[nSet].m_InspOCR[nId].m_nCharNum;
	int nCharPitch		= g_Inspect[nSet].m_InspOCR[nId].m_nCharPitch;
	int nEqualPitch		= g_Inspect[nSet].m_InspOCR[nId].m_nEqualPitch;
	int ll,tt,rr,bb;
	int nX1, nX2, nCx, nPrevCx;
	int nOffX, nOffY;
	int ret;
	int nGX1,nGX2; //등간격일때 계산상 위치
	int nScore[5], nOCRID[5], nX, nY;

	if(bShowGraphic) m_pImage->DrawRectangle(NULL, left, top, right, bottom, RGB(128,128,64));   //검사영역표시
	
	memset(&g_InspectResult[nId], 0, sizeof(INSPECTRESULT));									//결과 init
	g_InspectResult[nId].m_nInspectType=INSP_OCR;												//Spot 검사임.


	g_InspectResult[nId].m_nOCRPosX[0]=left;
	g_InspectResult[nId].m_nOCRPosX[1]=right;
	g_InspectResult[nId].m_nOCRPosY[0]=top;
	g_InspectResult[nId].m_nOCRPosY[1]=bottom;
	g_InspectResult[nId].m_nOCRNum=nCharNum;

	nPrevCx=left-nSizeX/2;
	nOffY=bottom-top;
	tt=top;
	bb=bottom;

	for(i=0;i<nCharNum;i++)
	{
		if(nEqualPitch)
		{
			nX1=left+(right-left)*i/nCharNum-nSizeX/2;
			nX2=left+(right-left)*(i+1)/nCharNum+nSizeX/2;
			nCx=(nX1+nX2)/2;
		}
		else
		{
			nGX1=left+(right-left)*i/nCharNum;
			nGX2=left+(right-left)*(i+1)/nCharNum;
			if(nPrevCx>nGX1) nPrevCx=nGX1-nSizeX/4;


			ret=FindCharCenter(fm, nPrevCx+nSizeX/2, tt, nPrevCx+nCharPitch*2-nSizeX/2, bb, g_System.m_nPitch , nSizeX, &nCx);

			if(ret==0)
			{
				nX1=nCx-nCharPitch;//+nSizeX/2;
				nX2=nCx+nCharPitch;//-nSizeX/2;
			}
			else
			{
				nX1=nGX1-nSizeX/2;
				nX2=nGX2+nSizeX/2;
				nCx=(nX1+nX2)/2;
			}
			if(nX1>nGX1) nX1=nGX1;
			if(nX2<nGX2) nX2=nGX2;
		}

			
		FindOCR(fm, nX1,tt-nOffY/2, nX2, bb+nOffY/2, g_System.m_nPitch , nScore, nOCRID, 1, &nX, &nY);
	
		g_InspectResult[nId].m_nPosX[i]=nCx;
		g_InspectResult[nId].m_nPosY[i]=bb;

		if(nScore[0]>=50 && nOCRID[0]>=0 && nOCRID[0]<OCR_NUMBER)
		{
			nCx=nX+nSizeX/2;
			g_InspectResult[nId].m_sOCRScore[i]=nScore[0];
			g_InspectResult[nId].m_cOCR[i]=l_OCRCHAR[nOCRID[0]];
		}

		nPrevCx=nCx;
	}

//	if(bShowGraphic || g_Inspect[nSet].m_InspSpot[nId].m_bShowBinary) m_pImage->UpdateImage();

	if(bShowGraphic) ShowOCRInspectResult(nId);													//결과표시
}


void CKoWebView::ShowOCRInspectResult(int nId)
{
	int i, k;
	CString str;
	int nNum=g_InspectResult[nId].m_nOCRNum;
	int left  =g_InspectResult[nId].m_nOCRPosX[0];
	int right =g_InspectResult[nId].m_nOCRPosX[1];
	int top   =g_InspectResult[nId].m_nOCRPosY[0];
	int bottom=g_InspectResult[nId].m_nOCRPosY[1];
	int nX;

	if(g_InspectResult[nId].m_nInspectType!=INSP_OCR) return;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	LOGFONT lf;
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 24;

	m_pImage->SetAddGraphic(true);
	for(i=0;i<g_InspectResult[nId].m_nOCRNum;i++)
	{
		if(i>0)
		{
		//	nX=left+(right-left)*i/nNum;
			nX=(g_InspectResult[nId].m_nPosX[i-1]+g_InspectResult[nId].m_nPosX[i])/2;
			m_pImage->DrawLine(NULL, nX, top, nX, bottom, RGB(0,255,0));
		}

		if(g_InspectResult[nId].m_sOCRScore[i]>=50)
		{	
			str.Format(_T("%c"), g_InspectResult[nId].m_cOCR[i]);
			m_pImage->DrawTextABS(NULL,g_InspectResult[nId].m_nPosX[i], g_InspectResult[nId].m_nPosY[i], RGB(255,0,0), str, pDefaultGUIFont); 
		}
	}
}

int FindCharCenter(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int  nCharSizeX, int *pCx)
{
	int i, j, nTmp;
	int nSum, nMaxSum=0, nMaxPos=-1;
	int nProfile[OCR_MAX_WIDTH*3];

	if(right-left>OCR_MAX_WIDTH*3) return -1;

	memset(nProfile, 0, OCR_MAX_WIDTH*3*sizeof(int));
	for(i=top;i<bottom-3;i++)
		for(j=left;j<right;j++)
		{
			nTmp=*(fm+pitch*i+j)-*(fm+pitch*i+j+3);
			if(nTmp<0) nTmp*=-1;
			nProfile[j-left]+=nTmp;
		}

	nSum=0;
	for(j=0;j<nCharSizeX;j++)
		nSum+=nProfile[j];

	nMaxSum=nSum;
	nMaxPos=nCharSizeX/2;
	for(j=nCharSizeX; j<(right-left);j++)
	{
		nSum=nSum+nProfile[j]-nProfile[j-nCharSizeX];
		if(nSum>nMaxSum)
		{
			nMaxSum=nSum;
			nMaxPos=j-nCharSizeX+nCharSizeX/2;
		}
	}

	*pCx=left+nMaxPos;

	return 0;
}
