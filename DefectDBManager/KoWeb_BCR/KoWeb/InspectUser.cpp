#include "stdafx.h"
#include "KoWeb.h"
#include "math.h"
#include "KoWebDoc.h"
#include "KoWebView.h"

void CKoWebView::InspectUser(LPBYTE fm, int nSet, int nId, BOOL bShowGraphic)
{
	int i, j;
	int left		 = g_Inspect[nSet].m_ROI1[nId].rc.left;
	int top			 = g_Inspect[nSet].m_ROI1[nId].rc.top;
	int right		 = g_Inspect[nSet].m_ROI1[nId].rc.right;
	int bottom		 = g_Inspect[nSet].m_ROI1[nId].rc.bottom;
	int nData		 = g_Inspect[nSet].m_InspUser[nId].m_nData;
	


}

void CKoWebView::ShowUserInspectResult(int nId)
{


}