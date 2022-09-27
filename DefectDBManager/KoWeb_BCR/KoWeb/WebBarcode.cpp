#include "Stdafx.h"

#include <memory.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include <dvec.h>
#include <math.h>

#include "KoWebDoc.h"
#include "KoWebView.h"

//BARCODE_VISION

BYTE l_fmBCRBK[512*128];
BYTE l_fmBCR[512*2178];

int GetBCRData(LPBYTE fm, int left, int top, int w, int h, int pitch, int *pX, int *pY, TCHAR sBcr[][30]);

void WEB_Barcode()
{
	int i, j, m, nOverlap;
	int nBcrPitch=512;
	LPBYTE fm=g_fmGrab[g_ID];
	int pitch=g_System.m_nPitch;
	int nX, nBCRCount;
	int nBcrX[10], nBcrY[10];
	TCHAR sBCR[10][30];

	g_Temp.m_nMaxGray=0;
	g_Temp.m_nMinGray=255;

	if(g_Param.m_nNotInspArea==0) return;


	if(g_Temp.m_nEdgeDir==0) nX=g_Temp.m_nInspectX1;
	else                     nX=g_Temp.m_nInspectX2-nBcrPitch;


	for(i=0;i<128;i++)
		memcpy( l_fmBCR+nBcrPitch*i, l_fmBCRBK+nBcrPitch*i, nBcrPitch);
	for(i=0;i<g_System.m_nImageH;i++)
		memcpy( l_fmBCR+nBcrPitch*(i+128), fm+pitch*i+nX, nBcrPitch);

	nBCRCount=GetBCRData(l_fmBCR, 0, 0, nBcrPitch, 2178, nBcrPitch, nBcrX, nBcrY, sBCR);


	for(i=0;i<nBCRCount;i++)
	{












	}


	//가장 마지막 촬상한것 보관-----------------------------------
	for(i=0;i<128;i++)
		memcpy( l_fmBCRBK+nBcrPitch*i, fm+pitch*i+nX, nBcrPitch);
	//------------------------------------------------------------
	
}

//최대 10개이나 현실적으로 3개만, 상, 하 붙은것은 제외.
//바코드 찾을때 필요한 파라미터 알려줘요
int GetBCRData(LPBYTE fm, int left, int top, int w, int h, int pitch, int *pX, int *pY, TCHAR sBcr[][30])
{
	int nBCRCount=0;




	return nBCRCount;
}