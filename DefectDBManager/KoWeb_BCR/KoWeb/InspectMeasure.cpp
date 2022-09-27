#include "stdafx.h"
#include "KoWeb.h"
#include "math.h"
#include "KoWebDoc.h"
#include "KoWebView.h"

int CKoWebView::InspectMeasure(LPBYTE fm, int nSet, int nId, BOOL bShowGraphic)
{
	int i, j;
	int nBaseId		 = g_Inspect[nSet].m_InspMeasure[nId].m_nBaseId;
	int nPointId	 = g_Inspect[nSet].m_InspMeasure[nId].m_nPointId;
	double dDist;
	double t,a,b;
	double dRx, dRy;
	
	int nLineType[2];

	memset(&g_InspectResult[nId], 0, sizeof(INSPECTRESULT));									//°á°ú init
	g_InspectResult[nId].m_nInspectType=INSP_GAP2LINE;	

	if(nBaseId<0 || nPointId<0 || nBaseId>=MAX_INSPECT || nPointId>=MAX_INSPECT)
	{
		g_InspectResult[nId].m_nMeasureResult[0]=1; //NG
		return -1;
	}

	nLineType[0]=g_Inspect[nSet].m_InspFindLine[nBaseId].m_nLineType;
	nLineType[1]=g_Inspect[nSet].m_InspFindLine[nPointId].m_nLineType;

	if( (nLineType[0]==0 && nLineType[1]==1) && (nLineType[0]==1 && nLineType[1]==0) )
	{
		g_InspectResult[nId].m_nMeasureResult[0]=1; //NG
		return -2;
	}

	if(g_InspectResult[nBaseId].m_nFindLineResult[0]==1 || g_InspectResult[nPointId].m_nFindLineResult[0]==1)
	{
		g_InspectResult[nId].m_nMeasureResult[0]=1; //NG
		return -3;
	}

	t=g_InspectResult[nBaseId].m_t;
	a=g_InspectResult[nBaseId].m_a;
	b=g_InspectResult[nBaseId].m_b;
	dDist=fabs(t*g_InspectResult[nPointId].m_dCy-a*g_InspectResult[nPointId].m_dCx-b)/sqrt(t*t+a*a);

	FindPointLineCross(g_InspectResult[nPointId].m_dCx, g_InspectResult[nPointId].m_dCy, t, a, b, &dRx, &dRy);

	if(bShowGraphic)
	{
		m_pImage->DrawLine(NULL, (int)(g_InspectResult[nPointId].m_dCx+0.5), (int)(g_InspectResult[nPointId].m_dCy+0.5), (int)(dRx+0.5), (int)(dRy+0.5), RGB(0,255,0));
	}

	g_InspectResult[nId].m_dDistance=dDist;
	if(dDist<g_Inspect[nSet].m_InspMeasure[nId].m_dMinDist || dDist>g_Inspect[nSet].m_InspMeasure[nId].m_dMaxDist)
	{
		g_InspectResult[nId].m_nMeasureResult[1]=1; //NG
	}
}

void CKoWebView::ShowMeasureInspectResult(int nId)
{


}