#include "stdafx.h"
#include "KoWeb.h"
#include "math.h"
#include "KoWebDoc.h"
#include "KoWebView.h"

void CKoWebView::InspectPattern(LPBYTE fm, int nSet, int nId, BOOL bShowGraphic)
{
	int nPat1=g_Inspect[nSet].m_InspPatt[nId].m_nPatternID1;
	int nPat2=g_Inspect[nSet].m_InspPatt[nId].m_nPatternID2;
	double dMinScore=g_Inspect[nSet].m_InspPatt[nId].m_dMinScore;
	double dMaxOffset=g_Inspect[nSet].m_InspPatt[nId].m_dMaxOffset;
	int left			= g_Inspect[nSet].m_ROI1[nId].rc.left;
	int top				= g_Inspect[nSet].m_ROI1[nId].rc.top;
	int right			= g_Inspect[nSet].m_ROI1[nId].rc.right;
	int bottom			= g_Inspect[nSet].m_ROI1[nId].rc.bottom;
	double dScore_, dX_, dY_;
	double dScore=0, dX, dY, dCx, dCy;
	int i, ret=1;
	COLORREF  cColor[2]={RGB(0,255,0), RGB(255,0,0)};
	int nNG=0;

	dCx=(right+left)*0.5;
	dCy=(bottom+top)*0.5;

	for(i=nPat1;i<=nPat2;i++)
	{
		ret=PatternMatch(i, fm, left, top, right, bottom, g_System.m_nImageW, &dScore_, &dX_, &dY_);
		
		if(ret==0)
		{
			if(dScore_>dScore)
			{
				dScore=dScore_;
				dX=dX_;
				dY=dY_;
			}
			if(dScore>=dMinScore) 
			{
				break;
			}
		}
	}

	g_InspectResult[nId].m_nInspectType=INSP_PATTERN;		
	g_InspectResult[nId].m_nPatternResult[0]=g_InspectResult[nId].m_nPatternResult[1]=1;
	g_InspectResult[nId].m_nPatternScore=(int)dScore;
	g_InspectResult[nId].m_dPatternOffset=0;
	
	if(ret==0 && dScore>=dMinScore) 
	{		
		g_InspectResult[nId].m_nPatternScore=(int)dScore;
		if(g_InspectResult[nId].m_nPatternScore>=dMinScore)   g_InspectResult[nId].m_nPatternResult[0]=0; //OK
		g_InspectResult[nId].m_dPatternOffset=sqrt((dCx-dX)*(dCx-dX)+(dCy-dY)*(dCy-dY));
		if(g_InspectResult[nId].m_dPatternOffset<=dMaxOffset) g_InspectResult[nId].m_nPatternResult[1]=0; //OK

		g_InspectResult[nId].m_dPatternRx=dX;
		g_InspectResult[nId].m_dPatternRy=dY;

		if(g_InspectResult[nId].m_nPatternResult[0]+g_InspectResult[nId].m_nPatternResult[1]==0) nNG=0;
		else                                                                                     nNG=1;

		m_pImage->DrawCross(NULL,(dX+0.5),(dY+0.5),10, cColor[nNG]);
	}
}

void CKoWebView::ShowPatternInspectResult(int nId)
{


}