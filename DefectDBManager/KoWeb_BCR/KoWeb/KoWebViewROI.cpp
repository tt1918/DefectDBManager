#include "stdafx.h"
#include "KoWebDoc.h"
#include "KoWebView.h"

void CKoWebView::TeachROI(bool bShow, bool bImageUpdate, CString str, ROIDATA *pROI)
{
	int nX, nY;
	int left, top, right, bottom;
	TCHAR sROIName[256];
	
	if(str.GetLength()<1) return;

	wsprintf(sROIName, _T("%s"), str);
	if(bShow)
	{
		if(pROI->dAngle==0)
		{
			if(pROI->rc.left<0) //맨처음이면
			{
				m_pImage->Local2Global(300, 300, &nX, &nY);	
				m_pImage->m_pROI->AddROI(sROIName, nX-250, nY-250, nX+250, nY+250);				
			}
			else //기존 티칭이면
			{
				m_pImage->m_pROI->AddROI(sROIName, pROI->rc.left, pROI->rc.top, pROI->rc.right, pROI->rc.bottom);	
			}
		}
		else
		{

		}
	}
	else
	{
		if(m_pImage->m_pROI->GetROIRegion(sROIName, &left, &top, &right, &bottom)==0)
		{
			pROI->rc.left	=left;
			pROI->rc.top	=top;
			pROI->rc.right	=right;
			pROI->rc.bottom	=bottom;
		}
		m_pImage->m_pROI->DeleteROI(sROIName);
	}

	if(bImageUpdate) m_pImage->UpdateImage();
}

void CKoWebView::GetTeachROI(int nSet, int nID)
{
	TCHAR sROIName[256];
	int left, top, right, bottom;

	wsprintf(sROIName, _T("%d"), nID);
	if(m_pImage->m_pROI->GetROIRegion(sROIName, &left, &top, &right, &bottom)==0)
	{
		g_Inspect[nSet].m_ROI1[nID].rc.left		=left;
		g_Inspect[nSet].m_ROI1[nID].rc.top		=top;
		g_Inspect[nSet].m_ROI1[nID].rc.right	=right;
		g_Inspect[nSet].m_ROI1[nID].rc.bottom	=bottom;		
	}
	return;
}

void CKoWebView::ShowGrayImage()
{
	m_pImage->PutImageData(g_fmGrab[g_ID], g_System.m_nImageW, g_System.m_nImageH,1);
}

int CKoWebView::PatternMatch(int nId, LPBYTE fmSearch, int left, int top, int right, int bottom, int pitch, double *dScore, double *dRx, double *dRy)
{
	int ret=1;
	double dReturnX, dReturnY;

	*dScore=*dRx=*dRy=0;

	if((right-left)<g_Pattern[nId].m_nWidth-1 || (bottom-top)<g_Pattern[nId].m_nHeight-1) return ret;

	ret=m_KOMatch.TeachTemplate(g_Pattern[nId].m_fm[0], 0,0,g_Pattern[nId].m_nWidth-1, g_Pattern[nId].m_nHeight-1, g_Pattern[nId].m_nWidth);
	if(ret==0) 
	{
		m_KOMatch.SetMatchingData((double)g_Pattern[nId].m_nMinScore/100., 0, 1, 10, 0.9);
		ret=m_KOMatch.Matching(fmSearch,left, top, right, bottom, pitch);
		if(ret==0)
		{
			dReturnX=m_KOMatch.GetX();
			dReturnY=m_KOMatch.GetY();
			*dRx=dReturnX+g_Pattern[nId].m_dReturnPosX;
			*dRy=dReturnY+g_Pattern[nId].m_dReturnPosY;
			*dScore=100.0*m_KOMatch.GetValue();
		}
	}

	return ret;
}

void CKoWebView::OneInspect(int nSet)
{
	int i, j, nId;
	int nNG=0;
	int ret;
	double start,end;
	
	m_pImage->SetAddGraphic(true);
	g_Temp.m_nFlatSuccess=0;		//Align (General Inspect에서는 Align성공으로 사용함)

	//ALign
	//---------------------------------------------------------------------------
	start=Clock_set();
	ret=FindAlignPos(nSet);								//Align을 위한 좌표구하기
	if(ret==0) //성공
	{
		CalAlignData(&g_AlignData[nSet]);				//Align데이타 구하기(Rotation, Translation 회전중심)
		
		ThreadFunc_MakeRotateImage(g_fmGrab[nSet],  g_fmTemp, 0,0, g_System.m_nImageW, g_System.m_nImageH, g_System.m_nImageW, g_System.m_nImageW, g_System.m_nImageH, g_AlignData[nSet].m_dCx, g_AlignData[nSet].m_dCy, -g_AlignData[nSet].m_dOffsetX,-g_AlignData[nSet].m_dOffsetY,-g_AlignData[nSet].m_dAngleDeg); //영상회전
		g_Temp.m_nFlatSuccess=1;
	}

	end=Clock_set();

	memcpy(g_fmFlat[nSet], g_fmTemp, g_System.m_nImageW*g_System.m_nImageH);
	//----------------------------------------------------------


	for(i=0;i<g_Inspect[nSet].m_nInspectCount;i++)
	{
		nId=g_Inspect[nSet].m_nID[i];
		if(!g_Inspect[nSet].m_nUSE[nId]) continue;
		switch(g_Inspect[nSet].m_nInspectType[i])
		{
		case INSP_BLOB:		InspectBlob(g_fmFlat[g_ID], g_InspSet, nId, true);			break;  
		case INSP_PATTERN:	InspectPattern(g_fmFlat[g_ID], g_InspSet, nId, true);		break; 
		case INSP_FINDLINE: InspectFindLine(g_fmFlat[g_ID], g_InspSet, nId, true);		break; 
		case INSP_GAP2LINE: InspectMeasure(g_fmFlat[g_ID], g_InspSet, nId, true);		break; 
		case INSP_SPOT:		InspectSpot(g_fmFlat[g_ID], g_InspSet,nId, true);			break;  //@항목추가 11
		case INSP_COMPARE:	InspectCompare(g_fmFlat[g_ID], g_InspSet, nId, true);		break; 
		case INSP_OCR:      InspectOCR(g_fmFlat[g_ID], g_InspSet, nId, true);			break; 
		case INSP_USER:		InspectUser(g_fmFlat[g_ID], g_InspSet, nId, true);			break; 
		case INSP_GAP:		break;
		case INSP_STAIN:	break;
		
		}
	}
	
	//모든 검사의 결과를 정리--------------------------
	g_Temp.m_nLatestResult=0;
	for(i=0;i<g_Inspect[nSet].m_nInspectCount;i++)
	{
		nId=g_Inspect[nSet].m_nID[i];
		if(!g_Inspect[nSet].m_nUSE[nId]) continue;

		ret=IsInspectResultNG(nId);
		
		if(ret)
		{
			g_Temp.m_nLatestResult=1;
			break;
		}
	}
	//-------------------------------------------------

	

	if(m_bShowImage)
	{
		if(g_Config.m_nShowProfile)		ShowProfile();
		if(g_Config.m_nShowCalibWidth)  ShowCalibWidth();

		if(m_bShowFlat)	m_pImage->PutImageData(g_fmFlat[nSet], g_System.m_nImageW, g_System.m_nImageH,1);
		else		    m_pImage->PutImageData(g_fmGrab[nSet], g_System.m_nImageW, g_System.m_nImageH,1);
	}


	if(m_pTeach && m_pTeach->IsWindowVisible())
		m_pTeach->UpdateResult();


}