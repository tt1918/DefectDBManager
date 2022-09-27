#include "stdafx.h"
#include "KoWeb.h"
#include "math.h"
#include "KoWebDoc.h"
#include "KoWebView.h"

void CKoWebView::InspectBlob(LPBYTE fm, int nSet, int nId, BOOL bShowGraphic)
{
	int i, j, ll, tt, rr, bb;
	int left			= g_Inspect[nSet].m_ROI1[nId].rc.left;
	int top				= g_Inspect[nSet].m_ROI1[nId].rc.top;
	int right			= g_Inspect[nSet].m_ROI1[nId].rc.right;
	int bottom			= g_Inspect[nSet].m_ROI1[nId].rc.bottom;
	int nThreshold		= g_Inspect[nSet].m_InspBlob[nId].m_nThreshold;
	int nOffsetX=0, nOffsetY=0;
	int nWidth=(right-left);
	int nHeight=(bottom-top);
	int pitch=g_System.m_nPitch;
	int nBlob, nMaxPos;
	int nObjectCount;
	double dCx, dCy, dDist=0;
	double dArea, dMaxArea, dAreaSum;
	LPBYTE fmB;	
	CChain chain(2000, 50000);

	if(bShowGraphic) m_pImage->DrawRectangle(NULL, left, top, right, bottom, RGB(128,128,64));   //검사영역표시
	
	memset(&g_InspectResult[nId], 0, sizeof(INSPECTRESULT));									//결과 init
	g_InspectResult[nId].m_nInspectType=INSP_BLOB;												//Blob 검사임.

	fmB=new BYTE[nWidth*nHeight];

	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
		{
			if(*(fm+pitch*i+j)>nThreshold) *(fmB+nWidth*(i-top)+(j-left))=255;
			else						   *(fmB+nWidth*(i-top)+(j-left))=0;
		}
	
	chain.SetChainData(g_Inspect[nSet].m_InspBlob[nId].m_nObject, fmB, 1,1,4,5000, nWidth, nHeight);
	nBlob=chain.FastChain(0,0,nWidth-1, nHeight-1);

	nObjectCount=0;
	dMaxArea=dAreaSum=0;
	nMaxPos=-1;
	for(i=0;i<nBlob;i++)
	{
		dArea=chain.Chain_Area(i);		
		if(dArea<0) continue;
		if(dArea<g_Inspect[nSet].m_InspBlob[nId].m_nMinArea) continue;

		nObjectCount++;

		dAreaSum+=dArea;
		if(dArea>dMaxArea)
		{
			dMaxArea=dArea;
			nMaxPos=i;
		}
		if(bShowGraphic)
		{
			ll=chain.FindMinX(i);
			tt=chain.FindMinY(i);
			rr=chain.FindMaxX(i);
			bb=chain.FindMaxY(i);
			m_pImage->DrawRectangle(NULL, ll+left,tt+top,rr+left+1,bb+top+1, RGB(0,128,0));
		}
	}
	if(nMaxPos>=0)
	{
		chain.Chain_Center(nMaxPos, &dCx, &dCy);
		dDist=sqrt((dCx-(nWidth-1)*0.5)*(dCx-(nWidth-1)*0.5)+(dCy-(nHeight-1)*0.5)*(dCy-(nHeight-1)*0.5));
		ll=chain.FindMinX(nMaxPos);
		tt=chain.FindMinY(nMaxPos);
		rr=chain.FindMaxX(nMaxPos);
		bb=chain.FindMaxY(nMaxPos);
	}
	else
	{
		rr=bb=0;
		ll=tt=1;
	}

	g_InspectResult[nId].m_dBlobData[0]=nObjectCount;
	g_InspectResult[nId].m_dBlobData[1]=dAreaSum;
	g_InspectResult[nId].m_dBlobData[2]=dMaxArea;
	g_InspectResult[nId].m_dBlobData[3]=dDist;
	g_InspectResult[nId].m_dBlobData[4]=rr-ll+1;
	g_InspectResult[nId].m_dBlobData[5]=bb-tt+1;

	for(i=0;i<BLOB_INPECT_ITEM;i++)
	{
		if(!g_Inspect[nSet].m_InspBlob[nId].m_nUSE[i]) continue;
		if(g_InspectResult[nId].m_dBlobData[i]<g_Inspect[nSet].m_InspBlob[nId].m_dMin[i] || g_InspectResult[nId].m_dBlobData[i]>g_Inspect[nSet].m_InspBlob[nId].m_dMax[i])
			g_InspectResult[nId].m_nBlobResult[i]=1;
	}
																
		
	delete fmB;
	if(bShowGraphic)
	{
		m_pImage->UpdateImage();
		ShowSpotInspectResult(nId);													//결과표시
	}
}

void CKoWebView::ShowBlobInspectResult(int nId)
{


}