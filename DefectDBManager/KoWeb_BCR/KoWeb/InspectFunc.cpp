#include "stdafx.h"
#include "math.h"
#include "ppl.h"

#define MAX_EDGE_COUNT	20
//Edge20개로 따라가기
int l_nEdgeCount = 0;
int l_nEdgeArray[MAX_EDGE_COUNT];

void ResetEdgeData()
{
	int i;
	l_nEdgeCount = 0;
	for (i = 0; i < MAX_EDGE_COUNT; i++) l_nEdgeArray[i] = -1;
	g_Temp.m_nFoundEdge = 0;
}

//512개로 Profile구한다.
void GetImageProfile()
{
	int i, j;
	LPBYTE fm = g_fmGrab[g_ID];
	int pitch = g_System.m_nPitch;
	int width = g_System.m_nImageW;
	int height = g_System.m_nImageH;
	int nSkipY;
	int nDevide = 512;
	int nSum = 0;
	int nSharpSum = 0, nBrightAvg = 0, nTmp, nDiv;
	long nAllSum = 0;
	int nMax = 0, nMin = 255;

	nSkipY = g_System.m_nImageH / nDevide;
	if (g_System.m_nImageH - nSkipY * nDevide)
	{
		if (g_System.m_nImageH > 1024)
		{
			nSkipY = 2;
			nDevide = height / 2;
		}
		else
		{
			nSkipY = 1;
			nDevide = height;
		}
	}

#ifdef USE_HOLE_SKIP
	memset(g_Profile, 0, width * sizeof(int));
	for (i = 0; i < height; i += nSkipY)
		for (j = 0; j < width; j++)
			*(g_Profile + j) += *(fm + pitch * i + j);

	for (j = 0; j < width; j++)
		*(g_Profile100 + j) = *(g_Profile + j) / nDevide;


	memset(g_Profile, 0, width * sizeof(int));
	for (i = 0; i < height; i += nSkipY)
		for (j = 0; j < width; j++)
		{
			if (*(fm + pitch * i + j) > *(g_Profile100 + j) + 10)	*(g_Profile + j) += *(g_Profile100 + j) + 10;
			else if (*(fm + pitch * i + j) < *(g_Profile100 + j) - 10)   *(g_Profile + j) += *(g_Profile100 + j) - 10;
			else											*(g_Profile + j) += *(fm + pitch * i + j);

			if (*(fm + pitch * i + j) > nMax) nMax = *(fm + pitch * i + j);
			if (*(fm + pitch * i + j) < nMin) nMin = *(fm + pitch * i + j);
		}
#else
	memset(g_Profile, 0, width * sizeof(int));
	for (i = 0; i < height; i += nSkipY)
		for (j = 0; j < width; j++)
		{
			*(g_Profile + j) += *(fm + pitch * i + j);
			if (*(fm + pitch * i + j) > nMax) nMax = *(fm + pitch * i + j);
			if (*(fm + pitch * i + j) < nMin) nMin = *(fm + pitch * i + j);
		}


#endif	


	//LognScratchSimple을위한 추가-----------------------------
	for (j = 0; j < width; j++)
		*(g_Profile100 + j) = *(g_Profile + j) * 100 / nDevide;
	//---------------------------------------------------------

	for (j = 0; j < width; j++)
	{
		*(g_Profile + j) /= nDevide;
		nAllSum += *(g_Profile + j);
	}

	for (j = 0; j < width / 2; j++)
		*(g_ProfileH + j) = (*(g_Profile + 2 * j) + *(g_Profile + 2 * j + 1)) / 2;


#ifdef USE_SK_BAT
	if (g_Param.m_nAutoEdgeFind && g_Param.m_nSearchX1 >= 0 && g_Param.m_nSearchX2 > 0)
	{
		GetImageEdge_BAT();
	}
	else
	{
		g_Temp.m_nInspectX1 = 0;
		g_Temp.m_nInspectX2 = width - 1;

		g_Temp.m_nInspectEdgeX1 = 0;
		g_Temp.m_nInspectEdgeX2 = width - 1;
	}
#else
	//자동 Edge설정시-------------------
	if (g_Param.m_nAutoEdgeFind && (g_Param.m_nNotInspArea > 0 || g_Param.m_nNotInspArea < 0))
	{
		GetImageEdge();
	}
	else
	{
		g_Temp.m_nInspectX1 = 0;
		g_Temp.m_nInspectX2 = width - 1;

		g_Temp.m_nInspectEdgeX1 = 0;
		g_Temp.m_nInspectEdgeX2 = width - 1;
	}
#endif

	//미검영역 설정되어 있으면 적용-------------------------------------------------------------------------------
	if (g_Param.m_nNotInspArea > 0)
	{
		if (g_Param.m_nNotInspArea > g_Temp.m_nInspectX1) g_Temp.m_nInspectX1 = g_Param.m_nNotInspArea;
	}
	else if (g_Param.m_nNotInspArea < 0)
	{
		if (width + g_Param.m_nNotInspArea < g_Temp.m_nInspectX2) g_Temp.m_nInspectX2 = width + g_Param.m_nNotInspArea;
	}
	if (g_Temp.m_nInspectX1 > g_Temp.m_nInspectX2) g_Temp.m_nInspectX2 = g_Temp.m_nInspectX1;
	//------------------------------------------------------------------------------------------------------------

	//검사영역으로 평균밝기 구한다.
	if (g_Temp.m_nInspectX2 - g_Temp.m_nInspectX1 > 10)
	{
		for (j = g_Temp.m_nInspectX1; j < g_Temp.m_nInspectX2; j++)
			nSum += *(g_Profile + j);

		g_Temp.m_dAverageBright = g_Temp.m_dAverageBrightForExpo = (double)nSum / (g_Temp.m_nInspectX2 - g_Temp.m_nInspectX1);

		for (i = height / 2 - 10; i < height / 2 + 10; i++)
			for (j = g_Temp.m_nInspectX1; j < g_Temp.m_nInspectX2 - 1; j++)
			{
				nTmp = *(fm + pitch * i + j + 1) - *(fm + pitch * i + j);
				if (nTmp > 0) nSharpSum += nTmp;
				else       nSharpSum -= nTmp;
			}
		g_Temp.m_dAverageSharp = (double)nSharpSum / (20 * (g_Temp.m_nInspectX2 - g_Temp.m_nInspectX1 - 1));

		//----------------------------------------------------------------------------------------------
		nSharpSum = nBrightAvg = 0;
		for (i = height / 2 - 10; i < height / 2 + 10; i++)
			for (j = 0; j < width / 3; j++)
			{
				nTmp = *(fm + pitch * i + j + 1) - *(fm + pitch * i + j);
				if (nTmp > 0) nSharpSum += nTmp;
				else       nSharpSum -= nTmp;
				nBrightAvg += *(fm + pitch * i + j);
			}
		nDiv = width / 3;
		g_Temp.m_dAverageSharpDetail[0] = (double)nSharpSum / (20 * nDiv);
		g_Temp.m_dAverageBrightDetail[0] = (double)nBrightAvg / (20 * nDiv);

		nSharpSum = nBrightAvg = 0;
		for (i = height / 2 - 10; i < height / 2 + 10; i++)
			for (j = width / 3; j < width * 2 / 3; j++)
			{
				nTmp = *(fm + pitch * i + j + 1) - *(fm + pitch * i + j);
				if (nTmp > 0) nSharpSum += nTmp;
				else       nSharpSum -= nTmp;
				nBrightAvg += *(fm + pitch * i + j);
			}
		nDiv = width * 2 / 3 - width / 3;
		g_Temp.m_dAverageSharpDetail[1] = (double)nSharpSum / (20 * nDiv);
		g_Temp.m_dAverageBrightDetail[1] = (double)nBrightAvg / (20 * nDiv);

		nSharpSum = nBrightAvg = 0;
		for (i = height / 2 - 10; i < height / 2 + 10; i++)
			for (j = width * 2 / 3; j < width - 1; j++)
			{
				nTmp = *(fm + pitch * i + j + 1) - *(fm + pitch * i + j);
				if (nTmp > 0) nSharpSum += nTmp;
				else       nSharpSum -= nTmp;
				nBrightAvg += *(fm + pitch * i + j);
			}
		nDiv = width - 1 - width * 2 / 3;
		g_Temp.m_dAverageSharpDetail[2] = (double)nSharpSum / (20 * nDiv);
		g_Temp.m_dAverageBrightDetail[2] = (double)nBrightAvg / (20 * nDiv);
		//------------------------------------------------------------------------------------------
	}
	else
	{
		g_Temp.m_dAverageBright = g_Temp.m_dAverageBrightForExpo = g_System.m_nGrabBright;

		g_Temp.m_dAverageBrightDetail[0] = g_Temp.m_dAverageBrightDetail[1] = g_Temp.m_dAverageBrightDetail[2] = 0;
		g_Temp.m_dAverageSharpDetail[0] = g_Temp.m_dAverageSharpDetail[1] = g_Temp.m_dAverageSharpDetail[2] = 0;

		if (g_Param.m_nNotInspArea == width || g_Param.m_nNotInspArea == width * (-1))
		{

		}
		else if (g_Param.m_nNotInspArea == 1)
		{
			for (j = width - 10; j < width; j++)
				nSum += *(g_Profile + j);

			g_Temp.m_dAverageBrightForExpo = (double)nSum / 10;
		}
		else if (g_Param.m_nNotInspArea == -1)
		{
			for (j = 0; j < 10; j++)
				nSum += *(g_Profile + j);

			g_Temp.m_dAverageBrightForExpo = (double)nSum / 10;
		}
		else if (g_Param.m_nNotInspArea<0 && g_Param.m_nNotInspArea>width * (-1))
		{
			for (j = 0; j < width + g_Param.m_nNotInspArea; j++)
				nSum += *(g_Profile + j);


			g_Temp.m_dAverageBrightForExpo = (double)nSum / (width + g_Param.m_nNotInspArea);
		}
		else if (g_Param.m_nNotInspArea > 0 && width + g_Param.m_nNotInspArea < width)
		{
			for (j = g_Param.m_nNotInspArea; j < width; j++)
				nSum += *(g_Profile + j);

			g_Temp.m_dAverageBrightForExpo = (double)nSum / (width - g_Param.m_nNotInspArea);
		}
		g_Temp.m_dAverageSharp = 0;
	}

	if (g_Temp.m_nImageInfoCount < 50)
	{
		if (g_Temp.m_nInspectEdgeX1 > 0)			  g_FrameInfo[g_Temp.m_nImageInfoCount].m_dEdge = g_Param.m_dCamStartPosX + g_Temp.m_nInspectEdgeX1 * g_Param.m_dScaleFactorX;
		else if (g_Temp.m_nInspectEdgeX2 < width - 1)  g_FrameInfo[g_Temp.m_nImageInfoCount].m_dEdge = g_Param.m_dCamStartPosX + g_Temp.m_nInspectEdgeX2 * g_Param.m_dScaleFactorX;
		else
		{
			if (g_Temp.m_nInspectX1 > 0 && g_Temp.m_nInspectX1 < width - 1)		g_FrameInfo[g_Temp.m_nImageInfoCount].m_dEdge = g_Param.m_dCamStartPosX + g_Temp.m_nInspectX1 * g_Param.m_dScaleFactorX;
			else if (g_Temp.m_nInspectX2 > 0 && g_Temp.m_nInspectX2 < width - 1)	g_FrameInfo[g_Temp.m_nImageInfoCount].m_dEdge = g_Param.m_dCamStartPosX + g_Temp.m_nInspectX2 * g_Param.m_dScaleFactorX;
			else															g_FrameInfo[g_Temp.m_nImageInfoCount].m_dEdge = 0;
		}

		g_FrameInfo[g_Temp.m_nImageInfoCount].m_dAveAllBright = (double)nAllSum / width;
		g_FrameInfo[g_Temp.m_nImageInfoCount].m_dAvgROIBright = g_Temp.m_dAverageBright;
		g_FrameInfo[g_Temp.m_nImageInfoCount].m_dSharpness = g_Temp.m_dAverageSharp;
		g_FrameInfo[g_Temp.m_nImageInfoCount].m_nMax = nMax;
		g_FrameInfo[g_Temp.m_nImageInfoCount].m_nMin = nMin;
	}
}

//자동Edge/자동에지
int GetMediumEdge(int nEdge); //Edge중 중간값을 찾음
int GetImageEdge_Sub8(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nOffset, int nThUp);
int GetImageEdge_Sub0(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nOffset, int nThUp);
int GetImageEdge_Sub1(LPBYTE fm, int left, int top, int width, int height, int pitch, int nThUp); //외곽에서 안쪽을 찾아가는 함수(반사류)
int GetImageEdge_Sub2(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nThUp);
int GetImageEdge_Sub3(LPBYTE fm, int pitch, int nThreshold);										//투영반사
int GetImageEdge_Sub4(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nThUp);

void GetImageEdge()
{
	int nEdge = 0;
	LPBYTE fm = g_fmGrab[g_ID];
	g_Temp.m_nFoundEdge = -1;
	g_Temp.m_nEdgeType = -1;

	if (g_Param.m_nEdgeThreshold < 5) //자동Edge라고 하고 1로 설정하는 경우 많음. 아래 주어진 값으로 Threshold 설정
	{
		switch (g_Temp.m_nPCOptic)
		{
		case COS1:  g_Param.m_nEdgeThreshold = 30;  break;
		case COS2:  g_Param.m_nEdgeThreshold = 10;  break;
		case COS3:  g_Param.m_nEdgeThreshold = 50;  break;
		case COS4:  g_Param.m_nEdgeThreshold = 120; break;
		case COS5:  g_Param.m_nEdgeThreshold = 160; break;
		case COS6:  g_Param.m_nEdgeThreshold = 30;  break;
		case COS7:  g_Param.m_nEdgeThreshold = 30;  break;
		case COS8:  g_Param.m_nEdgeThreshold = 30;  break;
		case COS9:  g_Param.m_nEdgeThreshold = 40;  break;
		case COSA:  g_Param.m_nEdgeThreshold = 35;  break;
		case COSB:  g_Param.m_nEdgeThreshold = 30;  break;
		case COSC:  g_Param.m_nEdgeThreshold = 50;  break;
		case COSD:  g_Param.m_nEdgeThreshold = 50;  break;
		case CBCR:  g_Param.m_nEdgeThreshold = 40;  break;
		case CMAK:  g_Param.m_nEdgeThreshold = 50;  break;
		}
	}
	//-------------------------------------------------------------------------------------------------------

	//광학계 따라서 검사방법 다르게...
	switch (g_Temp.m_nPCOptic)
	{
	case COS1:  g_Temp.m_nEdgeType = 0;		  break; //크로스(안쪽에서 바깥쪽으로 밝기차나면 STOP)
	case COS2:	g_Temp.m_nEdgeType = 1;		  break;
	case COS3:  g_Temp.m_nEdgeType = 0;		  break; //미분  (최외곽)
	case COS4:  g_Temp.m_nEdgeType = 2;		  break; //정투과 (안쪽)		
	case COS5:  g_Temp.m_nEdgeType = 2;		  break; //투영 (안쪽)
	case COS6:	g_Temp.m_nEdgeType = 0;		  break; //반사  (최외곽)
	case COS7:  g_Temp.m_nEdgeType = 0;		  break; //반사  (최외곽)
	case COS8:  g_Temp.m_nEdgeType = 0;		  break; //반사  (최외곽)
	case COS9:  g_Temp.m_nEdgeType = 0;		  break; //크로스(안쪽에서 바깥쪽으로 밝기차나면 STOP)
	case COSA:	g_Temp.m_nEdgeType = 3;		  break; //투영반사 25개 개별로 검사
	case COSB:  g_Temp.m_nEdgeType = 0;		  break; //반사  (최외곽)
	case COSC:  g_Temp.m_nEdgeType = 2;		  break; //
	case COSD:  g_Temp.m_nEdgeType = 2;		  break; //
	case CBCR:  g_Temp.m_nEdgeType = 4;		  break; //BCR(정투과)
	case CMAK:  g_Temp.m_nEdgeType = 2;		  break; //마킹비전(정투과)
	}

	if (g_Temp.m_nEdgeType == 0)	   //안쪽에서 바깥쪽으로 검사(크로스) : g_Param.m_nEdgeThreshold 절대값 차이나면 STOP
		nEdge = GetImageEdge_Sub0(fm, 0, 0, g_System.m_nImageW - 1, g_System.m_nImageH - 1, g_System.m_nImageW, (int)(3.0 / g_Param.m_dScaleFactorX), g_Param.m_nEdgeThreshold);
	else if (g_Temp.m_nEdgeType == 1) //바깥쪽에서 안쪽으로 검사(최외곽검출, 반사류) : g_Param.m_nEdgeThreshold 보다 밝으면 STOP
		nEdge = GetImageEdge_Sub1(fm, 0, 0, g_System.m_nImageW - 1, g_System.m_nImageH - 1, g_System.m_nImageW, g_Param.m_nEdgeThreshold);
	else if (g_Temp.m_nEdgeType == 2) //안쪽에서 바깥쪽으로 검사, g_Param.m_nEdgeThreshold 보다 밝으면 STOP
		nEdge = GetImageEdge_Sub2(fm, 0, 0, g_System.m_nImageW - 1, g_System.m_nImageH - 1, g_System.m_nImageW, g_Param.m_nEdgeThreshold);
	else if (g_Temp.m_nEdgeType == 3) //안쪽에서 바깥쪽으로 검사,(투영반사, 25개 개별로 검사해서 구함)
		nEdge = GetImageEdge_Sub3(fm, g_System.m_nImageW, g_Param.m_nEdgeThreshold);
	else if (g_Temp.m_nEdgeType == 4) // 에지 차이를 판단하여 경계 확인
		nEdge = GetImageEdge_Sub4(fm, 0, 0, g_System.m_nImageW - 1, g_System.m_nImageH - 1, g_System.m_nImageW, g_Param.m_nEdgeThreshold);
	else
		nEdge = -2;

	g_Temp.m_nInspectEdgeX1 = 0;
	g_Temp.m_nInspectEdgeX2 = g_System.m_nImageW - 1;

	if (nEdge < 0) //데이타 이상인 경우 전체검사
	{
		g_Temp.m_nInspectX1 = 0;
		g_Temp.m_nInspectX2 = g_System.m_nImageW - 1;
	}
	else
	{
		nEdge = GetMediumEdge(nEdge);
	}

	if (nEdge >= 0)
	{
		if (g_Temp.m_nEdgeDir == 0)
		{
			g_Temp.m_nInspectX2 = g_System.m_nImageW - 1;
			g_Temp.m_nInspectX1 = nEdge + g_Param.m_nEdgeOffset;
			g_Temp.m_nInspectEdgeX1 = nEdge;
			if (g_Temp.m_nInspectX1 > g_Temp.m_nInspectX2) g_Temp.m_nInspectX1 = g_Temp.m_nInspectX2;
		}
		else
		{
			g_Temp.m_nInspectX1 = 0;
			g_Temp.m_nInspectX2 = nEdge - abs(g_Param.m_nEdgeOffset);
			g_Temp.m_nInspectEdgeX2 = nEdge;
			if (g_Temp.m_nInspectX2 < g_Temp.m_nInspectX1) g_Temp.m_nInspectX2 = g_Temp.m_nInspectX1;
		}
		g_Temp.m_nFoundEdge = nEdge;
	}
	else
	{
		g_Temp.m_nInspectX1 = 0;
		g_Temp.m_nInspectX2 = g_System.m_nImageW - 1;
	}
}

#ifdef USE_SK_BAT
void GetImageEdge_BAT()
{
	int nEdge = 0;
	LPBYTE fm = g_fmGrab[g_ID];
	g_Temp.m_nFoundEdge = -1;
	g_Temp.m_nEdgeType = -1;

	int nLineEdge;
	int nLineType = 1;				//[ 0: 수평선찾기, 1: 수직선 찾기   꼭 수평, 수직아니라도 가까운쪽으로 선택하면 됨]
	int nObject = 1;					//[ 0: 흑  1: 백  2: 흑백상관없음		
	int nDir = 0;						//[ Edge서치방향 0: (왼쪽->오른쪽 or 위->아래)  1:(오른쪽->왼쪽 or 아래->위) ]
	int nMethod = 2;					//[ 0: Gray값으로  1:1stEdge   2: Max Edge 찾음  ]
	int nGrayTh = 0;					//[ 이 Gray값 을 넘거나 떨어질때 Edge 를 구함, nMethod=0일때만 사용함]
	int nSlopeTh = 5;					//[ 기울기가 이 값을 넘으면 Stop. 이부근에서 Max Edge 검색, , nMethod=1,2 일때만 사용함 ]
	int nAvgLine = 10;				//[ 몇 Line씩 평균해서 Edge 데이타를 구할 건지 설정 ]
	int nSkipPixel = 50;				//[ 한번 Edge찾으면 Skip하는 Pixel수]
	int nAllowGap = 30;				//[ 라인이 대략 수평이거나 수직일때 사용함. edge가 가장 많이 분포하는 범위, 이 범위 넘어가는 edge후보는 버린다, 0이면 사용하지 않는다.]

	double t1, a1, b1;				//안쪽
	double t2, a2, b2;				//바깥쪽
	double dCx, dCy;

	nSlopeTh = g_Param.m_nEdgeSlope;


	nEdge = -1;
	if (g_Param.m_nEdgeType > 0)  //Edge가 왼쪽에 있음
	{
		nDir = 1;
		nLineEdge = FindFitLine(fm, g_Param.m_nSearchX1, 0, g_Param.m_nSearchX2, g_System.m_nImageH - 1, g_System.m_nPitch,
			nLineType, nObject, nDir, nMethod, nGrayTh, nSlopeTh, nAvgLine, nSkipPixel, nAllowGap,
			&t1, &a1, &b1);

		if (nLineEdge)
		{
			FindCrossPoint(t1, a1, b1, 1, 0, g_System.m_nImageH / 2.0, &dCx, &dCy);
			g_Temp.m_dInnerEdge = dCx;
			nEdge = int(dCx + 0.5);
		}


		nDir = 0;
		nLineEdge = FindFitLine(fm, g_Param.m_nSearchX1, 0, g_Param.m_nSearchX2, g_System.m_nImageH - 1, g_System.m_nPitch,
			nLineType, nObject, nDir, nMethod, nGrayTh, nSlopeTh, nAvgLine, nSkipPixel, nAllowGap,
			&t2, &a2, &b2);
		if (nLineEdge)
		{
			FindCrossPoint(t2, a2, b2, 1, 0, g_System.m_nImageH / 2.0, &dCx, &dCy);
			g_Temp.m_dOuterEdge = dCx;
		}
	}
	else if (g_Param.m_nEdgeType < 0) //Edge가 오른쪽에 있음
	{
		nDir = 0;
		nLineEdge = FindFitLine(fm, g_Param.m_nSearchX1, 0, g_Param.m_nSearchX2, g_System.m_nImageH - 1, g_System.m_nPitch,
			nLineType, nObject, nDir, nMethod, nGrayTh, nSlopeTh, nAvgLine, nSkipPixel, nAllowGap,
			&t1, &a1, &b1);

		if (nLineEdge)
		{
			FindCrossPoint(t1, a1, b1, 1, 0, g_System.m_nImageH / 2.0, &dCx, &dCy);
			g_Temp.m_dInnerEdge = dCx;
			nEdge = int(dCx + 0.5);
		}


		nDir = 1;
		nLineEdge = FindFitLine(fm, g_Param.m_nSearchX1, 0, g_Param.m_nSearchX2, g_System.m_nImageH - 1, g_System.m_nPitch,
			nLineType, nObject, nDir, nMethod, nGrayTh, nSlopeTh, nAvgLine, nSkipPixel, nAllowGap,
			&t2, &a2, &b2);
		if (nLineEdge)
		{
			FindCrossPoint(t2, a2, b2, 1, 0, g_System.m_nImageH / 2.0, &dCx, &dCy);
			g_Temp.m_dOuterEdge = dCx;
		}
	}
	else
	{
		g_Temp.m_nInspectX1 = 0;
		g_Temp.m_nInspectX2 = g_System.m_nImageW - 1;
		return;
	}

	//-------------------------------------------------------------------------------------------------------



	g_Temp.m_nInspectEdgeX1 = 0;
	g_Temp.m_nInspectEdgeX2 = g_System.m_nImageW - 1;

	if (nEdge >= 0)
	{
		if (g_Param.m_nEdgeType > 0)
		{
			g_Temp.m_nInspectX2 = g_System.m_nImageW - 1;
			g_Temp.m_nInspectX1 = nEdge + g_Param.m_nEdgeOffset;
			g_Temp.m_nInspectEdgeX1 = nEdge;
			if (g_Temp.m_nInspectX1 > g_Temp.m_nInspectX2) g_Temp.m_nInspectX1 = g_Temp.m_nInspectX2;
		}
		else if (g_Param.m_nEdgeType < 0)
		{
			g_Temp.m_nInspectX1 = 0;
			g_Temp.m_nInspectX2 = nEdge - abs(g_Param.m_nEdgeOffset);
			g_Temp.m_nInspectEdgeX2 = nEdge;
			if (g_Temp.m_nInspectX2 < g_Temp.m_nInspectX1) g_Temp.m_nInspectX2 = g_Temp.m_nInspectX1;
		}
		g_Temp.m_nFoundEdge = nEdge;
	}
	else
	{
		g_Temp.m_nInspectX1 = 0;
		g_Temp.m_nInspectX2 = g_System.m_nImageW - 1;
	}
}
#endif

//MAX_EDGE_COUNT 개의 Edge데이타를 차례로 넣고 중간값을 구한다.
int GetMediumEdgeSub(int count, int* Value);
int GetMediumEdge(int nEdge)
{
	int nRet;
	int i, nCount = 0, nEdgeArray[MAX_EDGE_COUNT];
	if (l_nEdgeCount < MAX_EDGE_COUNT)
	{
		l_nEdgeArray[l_nEdgeCount++] = nEdge;
	}
	else
	{
		for (i = 0; i < MAX_EDGE_COUNT - 1; i++)
			l_nEdgeArray[i] = l_nEdgeArray[i + 1];

		l_nEdgeArray[MAX_EDGE_COUNT - 1] = nEdge;
	}
	for (i = 0; i < l_nEdgeCount; i++)
		if (l_nEdgeArray[i] >= 0)
			nEdgeArray[nCount++] = l_nEdgeArray[i];

	if (nCount > 0)
		nRet = GetMediumEdgeSub(nCount, nEdgeArray);
	else
		nRet = -1;

	return nRet;
}

int GetMediumEdgeSub(int count, int* Value)
{
	int i, j;
	int MaxValue;
	int Number;
	int StoreValue;
	int nValue[MAX_EDGE_COUNT];

	for (i = 0; i < count; i++)
		nValue[i] = Value[i];

	for (i = 0; i < count / 2; i++)
	{
		Number = i;
		MaxValue = StoreValue = nValue[i];

		for (j = i + 1; j < count; j++)
			if (nValue[j] > MaxValue) { MaxValue = nValue[j]; Number = j; }


		nValue[i] = nValue[Number]; //제일 큰 것

		nValue[Number] = StoreValue;
	}
	if (count < 2) return nValue[0];
	else        return nValue[count / 2 - 1];
}

//외곽에서 안쪽을 찾아가는 함수(반사류)
//5등분해서 한개라도 만족하면 됨.
int l_ProfileEdge[MAX_WIDTH];
int FindEdgeDetail(int nDir, int* pProfile, int N1, int N2);
int GetImageEdge_Sub0(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nOffset, int nThUp)
{
	int i, j, loop;
	int nY1, nY2, nOff;
	long nCount, nEdge[5] = { -1,-1,-1,-1,-1 };
	long nSum;
	int nStep = 5;
	int nDir = -1;

	if (g_Param.m_nNotInspArea > 0)		nDir = 0;   //왼쪽 못 씀 
	else if (g_Param.m_nNotInspArea < 0)   nDir = 1;   //오른쪽 못 씀
	else
	{
		return -1;
		//		if(g_Param.m_nEdgeOffset>0)			nDir=0; //왼쪽 못 씀
		//		else if(g_Param.m_nEdgeOffset<0)    nDir=1; //오른쪽 못 씀
	}
	if (nDir < 0) return -1;
	g_Temp.m_nEdgeDir = nDir;

	for (loop = 0; loop < nStep; loop++)
	{
		nY1 = top + (bottom - top + 1) * loop / nStep;
		nY2 = top + (bottom - top + 1) * (loop + 1) / nStep;
		nOff = (nY2 - nY1) / 50;
		if (nOff < 1) nOff = 1;
		nCount = 0;
		memset(l_ProfileEdge, 0, (right + 1) * sizeof(int));
		for (i = nY1; i < nY2; i += nOff)
		{
			nCount++;
			for (j = left; j <= right; j++)
				*(l_ProfileEdge + j) += *(fm + pitch * i + j);
		}
		if (nCount < 1) return 0;

		for (j = left; j <= right; j++)
			*(l_ProfileEdge + j) /= nCount;

		if (nDir == 0)
		{
			//nSum=0;
			//for(j=right;j>=left;j--)
			//{				
			//	if(nSum>5 && abs(*(l_ProfileEdge+j)-nSum/(right-j))>=nThUp || *(l_ProfileEdge+j)>200 || *(l_ProfileEdge+j)<20)
			//	{
			//		nEdge=j;
			//		break;
			//	}
			//	nSum+=*(l_ProfileEdge+j);
			//}
			for (j = right; j >= left + nOffset; j--)
			{
				if ((abs(*(l_ProfileEdge + j) - *(l_ProfileEdge + j - nOffset)) >= nThUp) || *(l_ProfileEdge + j) > 200) //  || *(l_ProfileEdge+j)<20)
				{
					nEdge[loop] = FindEdgeDetail(nDir, l_ProfileEdge, j - nOffset, j);
					break;
				}
			}
		}
		else
		{
			//nSum=0;
			//for(j=left;j<=right;j++)
			//{				
			//	if(nSum>5 && abs(*(l_ProfileEdge+j)-nSum/(j-left))>=nThUp || *(l_ProfileEdge+j)>200 || *(l_ProfileEdge+j)<20)
			//	{
			//		nEdge=j;
			//		break;
			//	}
			//	nSum+=*(l_ProfileEdge+j);
			//}
			for (j = left; j <= right - nOffset; j++)
			{
				if (abs(*(l_ProfileEdge + j) - *(l_ProfileEdge + j + nOffset)) >= nThUp || *(l_ProfileEdge + j) > 200) //|| *(l_ProfileEdge+j)<10 )
				{
					nEdge[loop] = FindEdgeDetail(nDir, l_ProfileEdge, j, j + nOffset);
					break;
				}
			}
		}
	}
	SortData(5, nEdge);
	if (nEdge[2] >= 0) return nEdge[2];
	else if (nEdge[1] >= 0) return nEdge[1];
	else if (nEdge[0] >= 0) return nEdge[0];
	else  return -2;
}
int FindEdgeDetail(int nDir, int* pProfile, int N1, int N2)
{
	int i, nFlag = 0;;
	int nMax = 0, nMin = 255, nThres;

	for (i = N1; i <= N2; i++)
	{
		if (*(pProfile + i) > nMax) nMax = *(pProfile + i);
		else if (*(pProfile + i) < nMin) nMin = *(pProfile + i);
	}

	nThres = (nMax + nMin) / 2;

	if (nDir == 0)
	{
		if (*(pProfile + N1) < *(pProfile + N2))
		{
			for (i = N2; i >= N1; i--)
				if (*(pProfile + i) <= nThres)
					return i;
		}
		else
		{
			for (i = N2; i >= N1; i--)
				if (*(pProfile + i) >= nThres)
					return i;
		}
	}
	else
	{
		if (*(pProfile + N1) < *(pProfile + N2))
		{
			for (i = N1; i <= N2; i++)
				if (*(pProfile + i) >= nThres)
					return i;
		}
		else
		{
			for (i = N1; i <= N2; i++)
				if (*(pProfile + i) <= nThres)
					return i;
		}
	}

	return 0;
}

int GetImageEdge_Sub1(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nThUp)
{
	int i, j, loop;
	int nY1, nY2, nOff;
	int nCount, nEdge = -1;
	int nStep = 5;
	int nDir = -1;

	if (g_Param.m_nNotInspArea > 0)		nDir = 0;   //왼쪽 못 씀 
	else if (g_Param.m_nNotInspArea < 0)   nDir = 1;   //오른쪽 못 씀
	else
	{
		if (g_Param.m_nEdgeOffset > 0)			nDir = 0; //왼쪽 못 씀
		else if (g_Param.m_nEdgeOffset < 0)    nDir = 1; //오른쪽 못 씀
	}
	if (nDir < 0) return -1;
	g_Temp.m_nEdgeDir = nDir;

	for (loop = 0; loop < nStep; loop++)
	{
		nY1 = top + (bottom - top + 1) * loop / nStep;
		nY2 = top + (bottom - top + 1) * (loop + 1) / nStep;
		nOff = (nY2 - nY2) / 50;
		if (nOff < 1) nOff = 1;
		nCount = 0;
		memset(l_ProfileEdge, 0, (right + 1) * sizeof(int));
		for (i = nY1; i < nY2; i += nOff)
		{
			nCount++;
			for (j = left; j <= right; j++)
				*(l_ProfileEdge + j) += *(fm + pitch * i + j);
		}
		if (nCount < 1) return 0;

		for (j = left; j <= right; j++)
			*(l_ProfileEdge + j) /= nCount;

		if (nDir == 0)
		{
			for (j = left; j <= right; j++)
				if (*(l_ProfileEdge + j) >= nThUp)
				{
					nEdge = j;

#ifdef WEB_DAKANO_MULTAE
					nEdge += 800;
					if (nEdge > 8192) nEdge = 8191;
#endif

					break;
				}
		}
		else
		{
			for (j = right; j >= left; j--)
				if (*(l_ProfileEdge + j) >= nThUp)
				{
					nEdge = j;

#ifdef WEB_DAKANO_MULTAE
					nEdge -= 800;
					if (nEdge < 0) nEdge = 0;
#endif

					break;
				}
		}
	}

	return nEdge;
}
int GetImageEdge_Sub2(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nThUp)
{
	int i, j, loop;
	int nY1, nY2, nOff;
	int nCount, nEdge = -1;
	int nStep = 5;
	int nDir = -1;

	if (g_Param.m_nNotInspArea > 0)		nDir = 0;   //왼쪽 못 씀 
	else if (g_Param.m_nNotInspArea < 0)   nDir = 1;   //오른쪽 못 씀
	else
	{
		if (g_Param.m_nEdgeOffset > 0)			nDir = 0; //왼쪽 못 씀
		else if (g_Param.m_nEdgeOffset < 0)    nDir = 1; //오른쪽 못 씀
	}
	if (nDir < 0) return -1;
	g_Temp.m_nEdgeDir = nDir;

	for (loop = 0; loop < nStep; loop++)
	{
		nY1 = top + (bottom - top + 1) * loop / nStep;
		nY2 = top + (bottom - top + 1) * (loop + 1) / nStep;
		nOff = (nY2 - nY2) / 50;
		if (nOff < 1) nOff = 1;
		nCount = 0;
		memset(l_ProfileEdge, 0, (right + 1) * sizeof(int));
		for (i = nY1; i < nY2; i += nOff)
		{
			nCount++;
			for (j = left; j <= right; j++)
				*(l_ProfileEdge + j) += *(fm + pitch * i + j);
		}
		if (nCount < 1) return 0;

		for (j = left; j <= right; j++)
			*(l_ProfileEdge + j) /= nCount;

		if (nDir == 0)
		{
			for (j = right; j >= left; j--)
				if (*(l_ProfileEdge + j) >= nThUp)
				{
					nEdge = j;
					break;
				}
		}
		else
		{
			for (j = left; j <= right; j++)
				if (*(l_ProfileEdge + j) >= nThUp)
				{
					nEdge = j;
					break;
				}
		}
	}

	return nEdge;
}


int l_nTooProfile[4096]; //투영, 투영반사 최대 폭
int GetImageEdge_Sub3(LPBYTE fm, int pitch, int nThreshold)
{
	int nFoundEdge = -1;
	int nEdgeData;
	int i, j, k;
	int nY1, nY2, nDiff;
	int nNotFound = 0, nFoundCount;
	long nEdge[50];
	int nDir;

	if (g_Param.m_nNotInspArea > 0)		nDir = 0;   //왼쪽 못 씀 
	else if (g_Param.m_nNotInspArea < 0)   nDir = 1;   //오른쪽 못 씀
	else
	{
		if (g_Param.m_nEdgeOffset > 0)			nDir = 0; //왼쪽 못 씀
		else if (g_Param.m_nEdgeOffset < 0)    nDir = 1; //오른쪽 못 씀
	}
	g_Temp.m_nEdgeDir = nDir;

	nThreshold *= 10;

	for (k = 0; k < g_System.m_nImagePart; k++)
	{
		memset(l_nTooProfile, 0, g_System.m_nImageW * sizeof(int));
		nY1 = g_System.m_nImagePartH * k + g_System.m_nImagePartH / 2 - 10;
		nY2 = nY1 + 20;
		for (i = nY1; i < nY2; i += 2)
			for (j = 0; j < g_System.m_nImageW; j++)
				*(l_nTooProfile + j) += *(fm + pitch * i + j);

		nEdgeData = -1;
		if (nDir == 0) //<----이방향 Scan
		{
			for (j = g_System.m_nImageW - 10; j >= 20; j--)
			{
				nDiff = *(l_nTooProfile + j - 10) - *(l_nTooProfile + j);
				if (nDiff < 0) nDiff *= -1;
				if (nDiff > nThreshold || *(l_nTooProfile + j) < 300)
				{
					nEdgeData = j - 5;
					break;
				}
			}
		}
		else	//--->이 방향 Scan
		{
			for (j = 10; j < g_System.m_nImageW - 20; j++)
			{
				nDiff = *(l_nTooProfile + j + 10) - *(l_nTooProfile + j);
				if (nDiff < 0) nDiff *= -1;
				if (nDiff > nThreshold || *(l_nTooProfile + j) < 300)
				{
					nEdgeData = j + 5;
					break;
				}
			}
		}
		if (nEdgeData < 0) nNotFound++;
		else            nEdge[k] = nEdgeData;
	}
	nFoundCount = g_System.m_nImagePart - nNotFound;
	if (nFoundCount < g_System.m_nImagePart / 2) return nFoundEdge;

	SortData(nFoundCount, nEdge);
	nFoundEdge = nEdge[nFoundCount / 2];

	//nDiff=nEdge[nFoundCount-2]-nEdge[1];
	//if(nDiff<100)
	//{
	//	nFoundEdge=nEdge[nFoundCount/2];
	//}
	//else
	//{
	//	if(nDir==0) nFoundEdge=nEdge[1];
	//	else         nFoundEdge=nEdge[nFoundCount-2];
	//}

	return nFoundEdge;
}

// 바코드 검사 확인용
int GetImageEdge_Sub4(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nThUp)
{
	int i, j, loop;
	int nY1, nY2, nOff;
	int nCount, nEdge = -1;
	int nStep = 5;
	int nDir = -1;

	if (g_Param.m_nNotInspArea > 0)		nDir = 0;   //왼쪽 못 씀 
	else if (g_Param.m_nNotInspArea < 0)   nDir = 1;   //오른쪽 못 씀
	else
	{
		if (g_Param.m_nEdgeOffset > 0)			nDir = 0; //왼쪽 못 씀
		else if (g_Param.m_nEdgeOffset < 0)    nDir = 1; //오른쪽 못 씀
	}
	if (nDir < 0) return -1;

	g_Temp.m_nEdgeDir = nDir;

	for (loop = 0; loop < nStep; loop++)
	{
		nY1 = top + (bottom - top + 1) * loop / nStep;
		nY2 = top + (bottom - top + 1) * (loop + 1) / nStep;
		nOff = (nY2 - nY2) / 50;
		if (nOff < 1) nOff = 1;
		nCount = 0;
		memset(l_ProfileEdge, 0, (right + 1) * sizeof(int));
		for (i = nY1; i < nY2; i += nOff)
		{
			nCount++;
			for (j = left; j <= right; j++)
				*(l_ProfileEdge + j) += *(fm + pitch * i + j);
		}
		if (nCount < 1) return 0;

		for (j = left; j <= right; j++)
			*(l_ProfileEdge + j) /= nCount;

		if (nDir == 0)
		{
			for (j = right-2; j >= left-2; j--)
				if (abs(*(l_ProfileEdge + j + 2) - *(l_ProfileEdge + j - 2)) >= nThUp)
				{
					nEdge = j;
					break;
				}

			if (nEdge != -1)
			{
				int val1, val2;
				val1 = *(l_ProfileEdge + j + 2);
				val2 = *(l_ProfileEdge + j - 2);
				int eth = val1+(val2 - val1) / 3;
				for (int ii = nEdge + 2; ii >= nEdge - 2; ii--)
				{
					if (*(l_ProfileEdge + ii) > eth)
					{
						nEdge = ii;
						break;
					}
				}
			}
		}
		else
		{
			for (j = left; j <= right; j++)
				if (abs(*(l_ProfileEdge + j + 2) - *(l_ProfileEdge + j - 2)) >= nThUp)
				{
					nEdge = j;
					break;
				}

			if (nEdge != -1)
			{
				int val1, val2;
				val1 = *(l_ProfileEdge + j - 2);
				val2 = *(l_ProfileEdge + j + 2);
				int eth = val1 + (val2 - val1) / 3;
				for (int ii = nEdge - 2; ii <= nEdge + 2; ii++)
				{
					if (*(l_ProfileEdge + ii) > eth)
					{
						nEdge = ii;
						break;
					}
				}
			}
		}

		if (nEdge != -1)
			break;
	}
	return nEdge;
}

BYTE l_fmBin[BAD_IMG_WIDTH * BAD_IMG_WIDTH];
//return 1이면 Overkill
/* 예전것
int CheckBoundaryOpticOverKill(LPBYTE fm, int nX, int nY, int pitch, int nGrabHeight, int Value)
{
	int i, j, ii, jj;
	BYTE fmS[BAD_IMG_WIDTH/4*BAD_IMG_WIDTH/4];
	int nOverKill=1;
	int lPitch=BAD_IMG_WIDTH/4;
	int nSum;
	int N=0, M=0;
	int left, top, right, bottom;
	int  nHCount=0;
	int nMax=-1, nMin=256, nMaxPosX, nMaxPosY, nMinPosX, nMinPosY;
	int nMaxA[4], nMinA[4];
	int nVDiffMax,nVDiffMin ,nDiff, nDiffMax;
	int Offset;
	int nHMin, nHMax, nMaxMin;
	int nArray[BAD_IMG_WIDTH];
	int nLineMax=0, nLineMin=255;
	int nProfile_20[128];
	int nSum_20=0, nSum_40=0;
	int nSum_20_Max=0, nBlobCount=0;
	int nNotOverKill=0, nRealOverKill=0;
	int nUpper=0, nUnder=0, nRetry=0;
	int nHCount1=0, nHMax1, nHMin1;

	int nTmp;


	left=nX-BAD_IMG_WIDTH/2; if(left<0) left=0; if(left+BAD_IMG_WIDTH>=pitch) left=pitch-BAD_IMG_WIDTH;
	right=left+BAD_IMG_WIDTH;
	top=nY-BAD_IMG_HEIGHT/2; if(top<0) top=0;  if((top+BAD_IMG_HEIGHT)>=nGrabHeight) top=nGrabHeight-BAD_IMG_HEIGHT;
	bottom=top+BAD_IMG_HEIGHT;


	for(i=top;i<bottom-1;i+=4)
	{
		M=0;
		for(j=left;j<right-1;j+=4)
		{
			nSum=0;
			for(ii=i;ii<i+4;ii++)
				for(jj=j;jj<j+4;jj++)
				{
					nSum+=*(fm+pitch*ii+jj);
				}
			*(fmS+N*lPitch+M)=nSum/16;
			M++;
		}
		N++;
	}
	M--;


	//---------------------------------------------------------------------
	//수평성분만 있는 이미지는 과검으로 처리
	double dMaxArea=0, dArea;
	for(i=1;i<N-1;i++)
		for(j=0;j<M-3;j++)
		{
			nDiff=*(fmS+i*lPitch+j)+*(fmS+(i-1)*lPitch+j)+*(fmS+(i+1)*lPitch+j)-*(fmS+i*lPitch+j+3)-*(fmS+(i-1)*lPitch+j+3)-*(fmS+(i+1)*lPitch+j+3);
			if(nDiff>14) *(l_fmBin+lPitch*i+j)=255;
			else         *(l_fmBin+lPitch*i+j)=0;
		}
	g_Chain.SetChainData(1, l_fmBin, 1, 1, 1, 200, lPitch, N-1);
	nBlobCount=g_Chain.FastChain(1, 1, M-4, N-1);
	for(i=0;i<nBlobCount;i++)
	{
		dArea=g_Chain.Chain_Area(i);
		if(dArea>dMaxArea) dMaxArea=dArea;
	}
	if(dMaxArea<3) return nOverKill;
	//--------------------------------------------------------------------


	//수평라인이면 과검------------------------------------
	nVDiffMax=0;
	nVDiffMin=255;
	for(j=0;j<M;j++)
	{
		nDiffMax=0;
		nHMax1=0;
		nHMin1=255;
		for(i=0;i<N-8;i++)
		{
			nDiff=*(fmS+i*lPitch+j)-*(fmS+(i+8)*lPitch+j);
			if(nDiff<0) nDiff*=-1;
			if(nDiff>nDiffMax) nDiffMax=nDiff;
		}
		for(i=0;i<N;i++)
		{
			if(*(fmS+i*lPitch+j)>nHMax1) nHMax1=*(fmS+i*lPitch+j);
			if(*(fmS+i*lPitch+j)<nHMin1) nHMin1=*(fmS+i*lPitch+j);
		}
		if(nDiffMax>nVDiffMax) nVDiffMax=nDiffMax;
		if(nDiffMax<nVDiffMin) nVDiffMin=nDiffMax;
		if(nHMax1-nHMin1>30) nHCount1++;
	}
	if((nVDiffMax-nVDiffMin)<10  || nHCount1>=M*0.9)
	{
		return nOverKill;
	}
	//----------------------------------------------------

	nHMin=255;
	nHMax=0;
	for(i=0;i<N;i++)
	{
		for(j=1;j<M-1;j++)
		{
			if(*(fmS+i*lPitch+j)>nMax)
			{
				nMax=*(fmS+i*lPitch+j);
				nMaxPosX=j;
				nMaxPosY=i;
			}
			else if(*(fmS+i*lPitch+j)<nMin)
			{
				nMin=*(fmS+i*lPitch+j);
				nMinPosX=j;
				nMinPosY=i;
			}
			if(*(fmS+i*lPitch+j)>nHMax) nHMax=*(fmS+i*lPitch+j);
			if(*(fmS+i*lPitch+j)<nHMin) nHMin=*(fmS+i*lPitch+j);
		}
	}

//	if(abs(16-nMaxPosX)>6 && abs(16-nMinPosX)>6)
//	{
//		nRealOverKill=1;  //무조건 과검
//		goto done;
//	}

//	if(abs(nMaxPosX-nMinPosX)<5 && nMax-nMin>15)
//	{
//		nOverKill=0;
//	}
	//----------------------------------------------------------------------


	Offset=4;
	if(nMaxPosX-Offset>0) nMaxA[0]=*(fmS+nMaxPosY*lPitch+nMaxPosX-Offset);
	else                  nMaxA[0]=*(fmS+nMaxPosY*lPitch);

	if(nMaxPosX+Offset<M) nMaxA[2]=*(fmS+nMaxPosY*lPitch+nMaxPosX+Offset);
	else                  nMaxA[2]=*(fmS+nMaxPosY*lPitch+M-1);

	if(nMinPosX-Offset>0) nMinA[0]=*(fmS+nMinPosY*lPitch+nMinPosX-Offset);
	else                  nMinA[0]=*(fmS+nMinPosY*lPitch);

	if(nMinPosX+Offset<M) nMinA[2]=*(fmS+nMinPosY*lPitch+nMinPosX+Offset);
	else                  nMinA[2]=*(fmS+nMinPosY*lPitch+M-1);

	Offset=7;
	if(nMaxPosX-Offset>0) nMaxA[1]=*(fmS+nMaxPosY*lPitch+nMaxPosX-Offset);
	else                  nMaxA[1]=*(fmS+nMaxPosY*lPitch);

	if(nMaxPosX+Offset<M) nMaxA[3]=*(fmS+nMaxPosY*lPitch+nMaxPosX+Offset);
	else                  nMaxA[3]=*(fmS+nMaxPosY*lPitch+M-1);

	if(nMinPosX-Offset>0) nMinA[1]=*(fmS+nMinPosY*lPitch+nMinPosX-Offset);
	else                  nMinA[1]=*(fmS+nMinPosY*lPitch);

	if(nMinPosX+Offset<M) nMinA[3]=*(fmS+nMinPosY*lPitch+nMinPosX+Offset);
	else                  nMinA[3]=*(fmS+nMinPosY*lPitch+M-1);

	if((nMax>=nMaxA[0]+Value || nMax>=nMaxA[1]+Value) && (nMax>=nMaxA[2]+Value || nMax>=nMaxA[3]+Value)) nNotOverKill=1;
	if((nMin<=nMinA[0]-Value || nMin<=nMinA[1]-Value) && (nMin<=nMinA[2]-Value || nMin<=nMinA[3]-Value)) nNotOverKill=1;

done:

	if(nRealOverKill==0)
	{
		double dArea, dAreaMax=0;
		int nBlobPos;
		int ll,tt,rr,bb;
		int ll_,tt_,rr_,bb_;

		nOverKill=1;
		memset(nProfile_20, 0, 128*sizeof(int));
		for(i=top+1;i<bottom-1;i++)
		{
			for(j=left+1;j<right-1;j++)
			{
				nTmp=*(fm+pitch*(i+1)+j-1)+*(fm+pitch*(i+1)+j)+*(fm+pitch*(i+1)+j+1)-*(fm+pitch*(i-1)+j-1)-*(fm+pitch*(i-1)+j)-*(fm+pitch*(i-1)+j+1);
				if(nTmp<0) nTmp*=-1;

				if(nTmp>=20) {*(l_fmBin+BAD_IMG_WIDTH*(i-top)+(j-left))=255; nSum_20++;}
				else		 *(l_fmBin+BAD_IMG_WIDTH*(i-top)+(j-left))=0;

				if(nTmp>=40) nSum_40++;

				nTmp=*(fm+pitch*i+j);
				if(nTmp>=100) nUpper++;
				if(nTmp<=50) nUnder++;
			}
		}

		g_Chain.SetChainData(1, l_fmBin, 1, 1, 1, 200, BAD_IMG_WIDTH, BAD_IMG_WIDTH);
		nBlobCount=g_Chain.FastChain(5, 5, BAD_IMG_WIDTH-5, BAD_IMG_WIDTH-5);
		if(nBlobCount>=100)
		{
			for(i=top+1;i<bottom-1;i++)
			{
				for(j=left+1;j<right-1;j++)
				{
					nTmp=*(fm+pitch*(i+1)+j-1)+*(fm+pitch*(i+1)+j)+*(fm+pitch*(i+1)+j+1)-*(fm+pitch*(i-1)+j-1)-*(fm+pitch*(i-1)+j)-*(fm+pitch*(i-1)+j+1);
					if(nTmp<0) nTmp*=-1;

					if(nTmp>=27) *(l_fmBin+BAD_IMG_WIDTH*(i-top)+(j-left))=255;
					else		 *(l_fmBin+BAD_IMG_WIDTH*(i-top)+(j-left))=0;
				}
			}
			g_Chain.SetChainData(1, l_fmBin, 1, 1, 1, 200, BAD_IMG_WIDTH, BAD_IMG_WIDTH);
			nBlobCount=g_Chain.FastChain(5, 5, BAD_IMG_WIDTH-5, BAD_IMG_WIDTH-5);
			nRetry=1;
		}

		if(nBlobCount)
		{
			for(i=0;i<nBlobCount;i++)
			{
				dArea=g_Chain.Chain_Area(i);
				if(dArea>dAreaMax)
				{
					dAreaMax=dArea;
					nBlobPos=i;
				}
			}
			if(dAreaMax>=15) nOverKill=0;
		}
		if(nSum_40>4+nRetry*10)
		{
			nOverKill=0;
		}

		if(nUnder>BAD_IMG_WIDTH*BAD_IMG_WIDTH*0.1 && nSum_40<=4)
		{
			nNotOverKill=0;
			nOverKill=1;
		}
	}

	if(nNotOverKill) nOverKill=0;

	return nOverKill;
}
*/

//return 1이면 Overkill
//경계투과사용
int CheckBoundaryOpticOverKill(LPBYTE fm, int nX, int nY, int pitch, int nGrabHeight, int Value)
{
	int i, j, ii, jj;
	BYTE fmS[BAD_IMG_WIDTH / 4 * BAD_IMG_WIDTH / 4];
	int nOverKill = 1;
	int lPitch = BAD_IMG_WIDTH / 4;
	int nSum, nAve, nDiffThres = 14;
	int N = 0, M = 0;
	int left, top, right, bottom;
	int  nHCount = 0;
	int nMax = -1, nMin = 256, nMaxPosX, nMaxPosY, nMinPosX, nMinPosY;
	int nMaxA[4], nMinA[4];
	int nVDiffMax, nVDiffMin, nDiff, nDiffMax;
	int Offset;
	int nHMin, nHMax, nMaxMin;
	int nArray[BAD_IMG_WIDTH];
	int nLineMax = 0, nLineMin = 255;
	int nProfile_20[128];
	int nSum_20 = 0, nSum_40 = 0;
	int nSum_20_Max = 0, nBlobCount = 0;
	int nNotOverKill = 0, nRealOverKill = 0;
	int nUpper = 0, nUnder = 0, nRetry = 0;
	int nHCount1 = 0, nHMax1, nHMin1;
	BYTE A[64 * 64];
	int nUPCnt = 0;
	int nDNCnt = 0;
	int nCenterUpCnt = 0;
	int nDiffOffset = 3;
	int nTmp, nMaxGray, nMinGray;


	left = nX - BAD_IMG_WIDTH / 2; if (left < 0) left = 0; if (left + BAD_IMG_WIDTH >= pitch) left = pitch - BAD_IMG_WIDTH;
	right = left + BAD_IMG_WIDTH;
	top = nY - BAD_IMG_HEIGHT / 2; if (top < 0) top = 0;  if ((top + BAD_IMG_HEIGHT) >= nGrabHeight) top = nGrabHeight - BAD_IMG_HEIGHT;
	bottom = top + BAD_IMG_HEIGHT;


	for (i = top; i < bottom - 1; i += 4)
	{
		M = 0;
		for (j = left; j < right - 1; j += 4)
		{
			nSum = 0;
			for (ii = i; ii < i + 4; ii++)
				for (jj = j; jj < j + 4; jj++)
				{
					nSum += *(fm + pitch * ii + jj);
				}
			*(fmS + N * lPitch + M) = nSum / 16;
			M++;
		}
		N++;
	}
	M--;

	nSum = 0;
	for (i = 0; i < N; i++)
		for (j = 0; j < M; j++)
			nSum += *(fmS + i * lPitch + j);
	nAve = nSum / (N * M);
	if (nAve <= 80)  nDiffThres = 14;
	else		  nDiffThres = 14 + ((nAve - 80) + 6) / 12;

	//---------------------------------------------------------------------
	//수평성분만 있는 이미지는 과검으로 처리
	double dMaxArea = 0, dArea;
	for (i = 1; i < N - 1; i++)
		for (j = 0; j < M - 3; j++)
		{
			nDiff = *(fmS + i * lPitch + j) + *(fmS + (i - 1) * lPitch + j) + *(fmS + (i + 1) * lPitch + j) - *(fmS + i * lPitch + j + 3) - *(fmS + (i - 1) * lPitch + j + 3) - *(fmS + (i + 1) * lPitch + j + 3);
			if (nDiff > nDiffThres)
				*(l_fmBin + lPitch * i + j) = 255;
			else				 *(l_fmBin + lPitch * i + j) = 0;
		}

	g_Chain.SetChainData(1, l_fmBin, 1, 1, 1, 200, lPitch, N - 1);
	nBlobCount = g_Chain.FastChain(1, 1, M - 4, N - 1);
	for (i = 0; i < nBlobCount; i++)
	{
		dArea = g_Chain.Chain_Area(i);
		if (dArea > dMaxArea) dMaxArea = dArea;
	}
	if (dMaxArea < 3)
		return nOverKill;
	//--------------------------------------------------------------------


	//수평라인이면 과검------------------------------------
	nVDiffMax = 0;
	nVDiffMin = 255;
	for (j = 0; j < M; j++)
	{
		nDiffMax = 0;
		nHMax1 = 0;
		nHMin1 = 255;
		for (i = 0; i < N - 8; i++)
		{
			nDiff = *(fmS + i * lPitch + j) - *(fmS + (i + 8) * lPitch + j);
			if (nDiff < 0) nDiff *= -1;
			if (nDiff > nDiffMax) nDiffMax = nDiff;
		}
		for (i = 0; i < N; i++)
		{
			if (*(fmS + i * lPitch + j) > nHMax1) nHMax1 = *(fmS + i * lPitch + j);
			if (*(fmS + i * lPitch + j) < nHMin1) nHMin1 = *(fmS + i * lPitch + j);
		}
		if (nDiffMax > nVDiffMax) nVDiffMax = nDiffMax;
		if (nDiffMax < nVDiffMin) nVDiffMin = nDiffMax;
		if (nHMax1 - nHMin1 > 30 && nHMax1 - nHMin1 < 100) nHCount1++;
		if (nHMax1 >= 200)
			break;
	}
	if ((nVDiffMax - nVDiffMin) < 10 || nHCount1 >= M * 0.9)
	{
		if (nVDiffMax < 150 && nHMax1 < 200)
			return nOverKill;
	}
	//----------------------------------------------------

	nHMin = 255;
	nHMax = 0;
	for (i = 0; i < N; i++)
	{
		for (j = 1; j < M - 1; j++)
		{
			if (*(fmS + i * lPitch + j) > nMax)
			{
				nMax = *(fmS + i * lPitch + j);
				nMaxPosX = j;
				nMaxPosY = i;
			}
			else if (*(fmS + i * lPitch + j) < nMin)
			{
				nMin = *(fmS + i * lPitch + j);
				nMinPosX = j;
				nMinPosY = i;
			}
			if (*(fmS + i * lPitch + j) > nHMax) nHMax = *(fmS + i * lPitch + j);
			if (*(fmS + i * lPitch + j) < nHMin) nHMin = *(fmS + i * lPitch + j);
		}
	}

	//	if(abs(16-nMaxPosX)>6 && abs(16-nMinPosX)>6)
	//	{
	//		nRealOverKill=1;  //무조건 과검
	//		goto done;
	//	}

	//	if(abs(nMaxPosX-nMinPosX)<5 && nMax-nMin>15) 
	//	{
	//		nOverKill=0;
	//	}
		//----------------------------------------------------------------------


	Offset = 4;
	if (nMaxPosX - Offset > 0) nMaxA[0] = *(fmS + nMaxPosY * lPitch + nMaxPosX - Offset);
	else                  nMaxA[0] = *(fmS + nMaxPosY * lPitch);

	if (nMaxPosX + Offset < M) nMaxA[2] = *(fmS + nMaxPosY * lPitch + nMaxPosX + Offset);
	else                  nMaxA[2] = *(fmS + nMaxPosY * lPitch + M - 1);

	if (nMinPosX - Offset > 0) nMinA[0] = *(fmS + nMinPosY * lPitch + nMinPosX - Offset);
	else                  nMinA[0] = *(fmS + nMinPosY * lPitch);

	if (nMinPosX + Offset < M) nMinA[2] = *(fmS + nMinPosY * lPitch + nMinPosX + Offset);
	else                  nMinA[2] = *(fmS + nMinPosY * lPitch + M - 1);

	Offset = 7;
	if (nMaxPosX - Offset > 0) nMaxA[1] = *(fmS + nMaxPosY * lPitch + nMaxPosX - Offset);
	else                  nMaxA[1] = *(fmS + nMaxPosY * lPitch);

	if (nMaxPosX + Offset < M) nMaxA[3] = *(fmS + nMaxPosY * lPitch + nMaxPosX + Offset);
	else                  nMaxA[3] = *(fmS + nMaxPosY * lPitch + M - 1);

	if (nMinPosX - Offset > 0) nMinA[1] = *(fmS + nMinPosY * lPitch + nMinPosX - Offset);
	else                  nMinA[1] = *(fmS + nMinPosY * lPitch);

	if (nMinPosX + Offset < M) nMinA[3] = *(fmS + nMinPosY * lPitch + nMinPosX + Offset);
	else                  nMinA[3] = *(fmS + nMinPosY * lPitch + M - 1);

	if ((nMax >= nMaxA[0] + Value || nMax >= nMaxA[1] + Value) && (nMax >= nMaxA[2] + Value || nMax >= nMaxA[3] + Value))
		nNotOverKill = 1;
	if ((nMin <= nMinA[0] - Value || nMin <= nMinA[1] - Value) && (nMin <= nMinA[2] - Value || nMin <= nMinA[3] - Value))
		nNotOverKill = 1;


#ifdef REMOVE_JJAGRI	
	//짜그리과검 없앰
	if (nNotOverKill || nOverKill == 0)
	{
		nMaxGray = 0;
		nMinGray = 255;
		for (i = top; i < bottom - 1; i += 2)
			for (j = left; j < right - 1; j += 2)
				*(A + 64 * (i - top) / 2 + (j - left) / 2) = (*(fm + pitch * i + j) + *(fm + pitch * i + j + 1) + *(fm + pitch * (i + 1) + j) + *(fm + pitch * (i + 1) + j + 1)) / 4;

		for (i = 5; i < 64 - 6; i++)
			for (j = 5; j < 64 - 6; j++)
			{
				if (*(A + 64 * i + j) > *(A + 64 * i + j + 5) + nDiffOffset && *(A + 64 * i + j) > *(A + 64 * i + j - 5) + nDiffOffset && *(A + 64 * i + j) > *(A + 64 * (i + 5) + j) + nDiffOffset && *(A + 64 * i + j) > *(A + 64 * (i - 5) + j) + nDiffOffset)  nUPCnt++;
				if (*(A + 64 * i + j) < *(A + 64 * i + j + 5) - nDiffOffset && *(A + 64 * i + j) < *(A + 64 * i + j - 5) - nDiffOffset && *(A + 64 * i + j) < *(A + 64 * (i + 5) + j) - nDiffOffset && *(A + 64 * i + j) < *(A + 64 * (i - 5) + j) - nDiffOffset)  nDNCnt++;
				if (*(A + 64 * i + j) > nMaxGray) nMaxGray = *(A + 64 * i + j);
				if (*(A + 64 * i + j) < nMinGray) nMinGray = *(A + 64 * i + j);
			}
		for (j = 0; j < 64 - 1; j++)
			for (i = 20; i < 64 - 20; i++)
			{
				if (*(A + 64 * i + j) > *(A + 64 * (i - 20) + j) + 10 && *(A + 64 * i + j) > *(A + 64 * (i + 20) + j) + 10)
				{
					nCenterUpCnt++;
					break;
				}
			}
		if (nMaxGray - nMinGray < 70)
		{
			if (nUPCnt + nDNCnt > 20 + (nMaxGray - nMinGray) || nCenterUpCnt > 50)
				return 2; //과검
		}
	}
#endif

done:

	if (nRealOverKill == 0)
	{
		double dArea, dAreaMax = 0;
		int nBlobPos;
		int ll, tt, rr, bb;
		int ll_, tt_, rr_, bb_;

		nOverKill = 1;
		memset(nProfile_20, 0, 128 * sizeof(int));
		for (i = top + 1; i < bottom - 1; i++)
		{
			for (j = left + 1; j < right - 1; j++)
			{
				nTmp = *(fm + pitch * (i + 1) + j - 1) + *(fm + pitch * (i + 1) + j) + *(fm + pitch * (i + 1) + j + 1) - *(fm + pitch * (i - 1) + j - 1) - *(fm + pitch * (i - 1) + j) - *(fm + pitch * (i - 1) + j + 1);
				if (nTmp < 0) nTmp *= -1;

				if (nTmp >= 20) { *(l_fmBin + BAD_IMG_WIDTH * (i - top) + (j - left)) = 255; nSum_20++; }
				else		 *(l_fmBin + BAD_IMG_WIDTH * (i - top) + (j - left)) = 0;

				if (nTmp >= 40) nSum_40++;

				nTmp = *(fm + pitch * i + j);
				if (nTmp >= 100) nUpper++;
				if (nTmp <= 50) nUnder++;
			}
		}

		g_Chain.SetChainData(1, l_fmBin, 1, 1, 1, 200, BAD_IMG_WIDTH, BAD_IMG_WIDTH);
		nBlobCount = g_Chain.FastChain(5, 5, BAD_IMG_WIDTH - 5, BAD_IMG_WIDTH - 5);

		if (nBlobCount >= 100)
		{
			for (i = top + 1; i < bottom - 1; i++)
			{
				for (j = left + 1; j < right - 1; j++)
				{
					nTmp = *(fm + pitch * (i + 1) + j - 1) + *(fm + pitch * (i + 1) + j) + *(fm + pitch * (i + 1) + j + 1) - *(fm + pitch * (i - 1) + j - 1) - *(fm + pitch * (i - 1) + j) - *(fm + pitch * (i - 1) + j + 1);
					if (nTmp < 0) nTmp *= -1;

					if (nTmp >= 27) *(l_fmBin + BAD_IMG_WIDTH * (i - top) + (j - left)) = 255;
					else		 *(l_fmBin + BAD_IMG_WIDTH * (i - top) + (j - left)) = 0;
				}
			}
			g_Chain.SetChainData(1, l_fmBin, 1, 1, 1, 200, BAD_IMG_WIDTH, BAD_IMG_WIDTH);
			nBlobCount = g_Chain.FastChain(5, 5, BAD_IMG_WIDTH - 5, BAD_IMG_WIDTH - 5);
			nRetry = 1;
		}

		if (nBlobCount)
		{
			for (i = 0; i < nBlobCount; i++)
			{
				dArea = g_Chain.Chain_Area(i);
				if (dArea > dAreaMax)
				{
					dAreaMax = dArea;
					nBlobPos = i;
				}
			}
			if (dAreaMax >= 15) nOverKill = 0;
		}
		if (nSum_40 > 4 + nRetry * 10)
		{
			nOverKill = 0;
		}

		if (nUnder > BAD_IMG_WIDTH * BAD_IMG_WIDTH * 0.1 && nSum_40 <= 4)
		{
			nNotOverKill = 0;
			nOverKill = 1;
		}
	}

	if (nNotOverKill)
		nOverKill = 0;

	return nOverKill;
}



double GetAverage(LPBYTE fm, int left, int top, int right, int bottom, int pitch)
{
	int i, j;
	long nSum = 0;

	for (i = top; i < bottom; i++)
		for (j = left; j < right; j++)
		{
			nSum += *(fm + pitch * i + j);
		}

	return (double)nSum / ((right - left) * (bottom - top));
}

void GetMinMax(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int* pMax, int* pMin, double* pSharp)
{
	int i, j;
	int nSum = 0;
	int nCount = 0;
	int nTmp;

	*pMax = 0;
	*pMin = 255;
	for (i = top; i < bottom; i++)
		for (j = left; j < right; j++)
		{
			if (*(fm + pitch * i + j) > *pMax) *pMax = *(fm + pitch * i + j);
			if (*(fm + pitch * i + j) < *pMin) *pMin = *(fm + pitch * i + j);

			if (i < bottom - 1 && j < right - 1)
			{
				nTmp = *(fm + pitch * i + j) - *(fm + pitch * i + 1 + j + 1);
				if (nTmp < 0) nTmp *= (-1);
				nSum += nTmp;
				nCount++;
			}
		}

	if (nCount > 0) *pSharp = (double)nSum / nCount;
	else         *pSharp = 0;
}

int GetWaterPrint(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nHighCut, int nOffset, int nOffsetValue, int nMinYLength, double* pValue)
{
	int i, j, k;
	int ll, tt, rr, bb;
	int nSum = 0;
	int nCount = 0;
	int nValue, nX1Value, nX2Value;
	int nTmp;
	int nX1, nX2;
	int nBlob;


	memset(g_fmTemp1, 0, pitch * bottom);
	for (i = top; i < bottom; i++)
		for (j = left; j < right; j++)
		{
			nX1 = j - nOffset;
			nX2 = j + nOffset;
			if (nX1 < 0 || nX2 >= pitch) continue;

			nX1Value = *(fm + pitch * i + nX1);
			nX2Value = *(fm + pitch * i + nX2);
			nValue = *(fm + pitch * i + j);
			nCount++;

			if (nValue - nX1Value >= nOffsetValue && nValue - nX2Value >= nOffsetValue)
			{
				*(g_fmTemp1 + pitch * i + j) = 255;
			}
		}
	g_Chain.SetChainData(1, g_fmTemp1, 2, 2, nMinYLength * 2, 5000, pitch, bottom);
	nBlob = g_Chain.FastChain(left, top, right, bottom);

	for (k = 0; k < nBlob; k++)
	{
		ll = g_Chain.FindMinX(k);
		tt = g_Chain.FindMinY(k);
		rr = g_Chain.FindMaxX(k);
		bb = g_Chain.FindMaxY(k);

		if ((rr - ll + 1) < (bb - tt + 1))
		{
			if ((bb - tt + 1) / (rr - ll + 1) < 2) continue;
		}
		else
		{
			if ((rr - ll + 1) / (bb - tt + 1) < 2) continue;
		}

		for (i = tt; i < bb; i++)
			for (j = ll; j < rr; j++)
			{
				if (*(g_fmTemp1 + pitch * i + j) == 0) continue;

				nX1 = j - nOffset;
				nX2 = j + nOffset;
				if (nX1 < 0 || nX2 >= pitch) continue;

				nX1Value = *(fm + pitch * i + nX1);
				nX2Value = *(fm + pitch * i + nX2);
				nValue = *(fm + pitch * i + j);
				nSum += (nValue - (nX1Value + nX2Value) / 2);
			}
	}

	if (nCount > 0) *pValue = (double)nSum / nCount;
	else         *pValue = 0;

	return nSum;
}

int GetLevel(double dValue, double* pValue)
{
	int i, nLevel = -1;

	if (dValue < 0) dValue *= -1;

	for (i = 0; i < MAX_LEVEL; i++)
	{
		if (g_Param.m_nUseLevel[i] && pValue[i] > 0 && dValue >= pValue[i])
		{
			nLevel = i + 1;
			break;
		}
	}

	return nLevel;
}

int GetLevel(int dValue, int* pValue)
{
	int i, nLevel = -1;

	if (dValue < 0) dValue *= -1;

	for (i = 0; i < MAX_LEVEL; i++)
	{
		if (g_Param.m_nUseLevel[i] && pValue[i] > 0 && dValue >= pValue[i])
		{
			nLevel = i + 1;
			break;
		}
	}

	return nLevel;
}

int GetLevel(double dValue, double dSize, int* pValue, double* pSize)
{
	int i, nLevel = -1;

	if (dValue < 0) dValue *= -1;

	for (i = 0; i < MAX_LEVEL; i++)
	{
		if (g_Param.m_nUseLevel[i] && (pValue[i] > 0.0001 || pSize[i] > 0.0001) && (int)dValue >= pValue[i] && dSize >= pSize[i])
		{
			nLevel = i + 1;
			break;
		}
	}

	return nLevel;
}

int GetLevel(double dValueUp, double dValueDn, double dSize, int* pValueUp, int* pValueDn, double* pSize)
{
	int i, nLevel = -1;

	if (dValueUp < 0) dValueUp *= -1;
	if (dValueDn < 0) dValueDn *= -1;

	for (i = 0; i < MAX_LEVEL; i++)
	{
		if (g_Param.m_nUseLevel[i] && ((pValueUp[i] > 0.0001 && pValueDn[i] > 0.0001) || pSize[i] > 0.0001) && (int)dValueUp >= pValueUp[i] && (int)dValueDn >= pValueDn[i] && dSize >= pSize[i])
		{
			nLevel = i + 1;
			break;
		}
	}

	return nLevel;
}

//구한 결과가 설정된 영역에 있으면 True
double l_SX[10] = { 0,0,0,0,0,0,0,0,0,0 };
double l_EX[10] = { 3000,0,0,0,0,0,0,0,0,0 };
bool IsInspectArea(int nX)
{
#ifdef CHECK_INPOS
	int i;
	double dPos;

	dPos = g_Param.m_dCamStartPosX + nX * g_Param.m_dScaleFactorX;
	for (i = 0; i < 10; i++)
	{
		if (dPos >= l_SX[i] && dPos <= l_EX[i]) return true;
	}
	return false;
#else
	return true;
#endif
}

int l_VProfile[8192];
int WEB_ImageCheck()
{
	int ret = 0;
	int i, j, m;
	LPBYTE fm = g_fmGrab[g_ID];
	int nWidth = g_System.m_nImageW;
	int nHeight = g_System.m_nImageH;
	int pitch = g_System.m_nPitch;
	int nX1 = g_Temp.m_nInspectX1;
	int nX2 = g_Temp.m_nInspectX2;
	int nCount = 0, nDiff;
	int nJump = 100;
	long nSum;


	nJump = (nX2 - nX1) / 100;
	if (nJump == 0) nJump = 1;
	else if (nJump > 100) nJump = 100;

	for (j = nX1; j < nX2; j += nJump) nCount++;

	if (nCount < 10) return ret;

	for (i = 0; i < nHeight; i++)
	{
		nSum = 0;
		for (j = nX1; j < nX2; j += nJump)
			nSum += *(fm + pitch * i + j);

		l_VProfile[i] = nSum / nCount;
		if (nSum / nCount > 230)
		{
			ret = 1;
			break;
		}
	}
	if (ret == 0)
	{
		for (i = 0; i < nHeight - 1; i++)
		{
			nDiff = l_VProfile[i] - l_VProfile[i + 1];
			if (nDiff > 10)
			{
				ret = 1;
				break;
			}
		}
	}

	return ret;
}

BYTE l_fmS[128 * 128];
BYTE l_fmSBinary[128 * 128];

int FindLamiPress(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nThUp, double* pCompactness, int* pSizeX, int* pSizeY)
{
	int i, j, k, nTmp;
	int nLamiPress = 0;
	int nHisto[256], nMaxHisto, nMaxHistoPos, nMaxPos, nMinPos;
	int nThreshold;
	int nImageSize = 128;
	int nBlob, nSizeX, nSizeY;
	RECT rcMax, rcMin, rcBlob;
	double dArea, dMaxArea, dMinArea, dHoleArea, nBlobCount;
	int nBlobTop, nBlobBot, nBlobCnt;
	int nBlobPlusCount;
	double dCompactness;

	*pCompactness = *pSizeX = *pSizeY = 0;

	for (i = top; i < bottom; i++)
		for (j = left; j < right; j++)
			*(l_fmS + 128 * (i - top) + (j - left)) = *(fm + pitch * i + j);

	//평균값 + nThUp을 기준으로 Binary---------------------------------------
	memset(nHisto, 0, 256 * sizeof(int));
	for (i = 0; i < nImageSize; i++)
		for (j = 0; j < nImageSize; j++)
			nHisto[*(l_fmS + nImageSize * i + j)]++;

	nMaxHistoPos = 0;
	nMaxHisto = nHisto[0];
	for (i = 1; i < 256; i++)
		if (nHisto[i] > nMaxHisto)
		{
			nMaxHisto = nHisto[i];
			nMaxHistoPos = i;
		}
	//----------------------------------------------------------------------

	for (k = 0; k < 2; k++)
	{
		nThreshold = nMaxHistoPos + nThUp * (1 + k);
		if (k == 1) nThreshold += 5;

		for (i = 0; i < nImageSize; i++)
			for (j = 0; j < nImageSize; j++)
				if (*(l_fmS + nImageSize * i + j) < nThreshold) *(l_fmSBinary + nImageSize * i + j) = 0;
				else								   *(l_fmSBinary + nImageSize * i + j) = 255;

		//Blob-------------------------------------------------------------------
		g_Chain.SetChainData(1, l_fmSBinary, 2, 2, 4, 3000, nImageSize, nImageSize);
		nBlob = g_Chain.FastChain(0, 0, nImageSize - 1, nImageSize - 1);
		if (nBlob < 1) goto done;

		dMaxArea = 0;
		dMinArea = 100000;
		nMaxPos = dHoleArea = 0;
		nMinPos = -1;
		nBlobCount = 0;
		nBlobCnt = 0; nBlobBot = 0; nBlobTop = 128;
		nBlobPlusCount = 0;
		for (i = 0; i < nBlob; i++)
		{
			dArea = g_Chain.Chain_Area(i);

			if (dArea > 15) nBlobPlusCount++;

			if (dArea > dMaxArea)
			{
				dMaxArea = dArea;
				nMaxPos = i;
			}
			else if (dArea < dMinArea)
			{
				dMinArea = dArea;
				nMinPos = i;
			}

			if (k == 0 && dArea > 20)
			{
				nTmp = g_Chain.FindMinY(i);
				if (nTmp < nBlobTop)   nBlobTop = nTmp;

				nTmp = g_Chain.FindMaxY(i);
				if (nTmp > nBlobBot) nBlobBot = nTmp;
				nBlobCnt++;
			}

			if (dArea < 0) dHoleArea += dArea;
			if (k == 1 && dArea > 0)
			{
				rcBlob.right = g_Chain.FindMaxX(i);
				rcBlob.left = g_Chain.FindMinX(i);
				rcBlob.bottom = g_Chain.FindMaxY(i);
				rcBlob.top = g_Chain.FindMinY(i);
				if (rcBlob.left >= rcMax.left && rcBlob.right <= rcMax.right && rcBlob.top >= rcMax.top && rcBlob.bottom <= rcMax.bottom)
					nBlobCount++;
			}
		}
		if (nBlobPlusCount > 10) goto done;
		if (nBlobCnt >= 2 && (nBlobBot - nBlobTop) > 64) goto done;
		rcMax.right = g_Chain.FindMaxX(nMaxPos);
		rcMax.left = g_Chain.FindMinX(nMaxPos);
		rcMax.bottom = g_Chain.FindMaxY(nMaxPos);
		rcMax.top = g_Chain.FindMinY(nMaxPos);

		nSizeX = rcMax.right - rcMax.left + 1;
		nSizeY = rcMax.bottom - rcMax.top + 1;

		*pSizeX = nSizeX;
		*pSizeY = nSizeY;

		if (nSizeX + nSizeY <= 20)        goto done;				//Size가 15Pixel 안되면 라이아님.
		if (nSizeX > 100 || nSizeY > 100) goto done;             //Size가 100보다 크면 라미아님
		if (nSizeY / nSizeX >= 2)      goto done;				//긴것은 제외

		dCompactness = g_Chain.FindCompactness(nMaxPos);
		*pCompactness = dCompactness;							//dCompactness가 0.5 이하는 라미.
		if (dCompactness <= 0.4)
		{
			nLamiPress = 1;
			goto done;
		}

		if (nMinPos > 0)
		{
			rcMin.right = g_Chain.FindMaxX(nMinPos);
			rcMin.left = g_Chain.FindMinX(nMinPos);
			rcMin.bottom = g_Chain.FindMaxY(nMinPos);
			rcMin.top = g_Chain.FindMinY(nMinPos);

			if (dHoleArea <= -8 && rcMin.left > rcMax.left && rcMin.right<rcMax.right && rcMin.top>rcMax.top && rcMin.bottom < rcMax.bottom)
			{
				nLamiPress = 1;
				goto done;
			}
		}
		if (nBlobCount >= 2) //Threshold를 올림에 따라서 불량이 쪼개지는 경우 
			nLamiPress = 1;
	}



done:
	return nLamiPress;
}


//마킹 Dot 이면 1
BYTE l_fmBB[512 * 256];
#define MARK_LIMIT_UP	20
#define MARK_LIMIT_DN   40
int CheckDotMarking(LPBYTE fm, int x, int y, int xMin, int xMax, int pitch, int nHeight, int nThreshold, double dScale)
{
	int nRet = 0;
	int i, j;
	int ll, tt, rr, bb;
	int ll_, tt_, rr_, bb_;
	int nY, nX, left, top, right, bottom;
	int nTmp, nBlob, nMaxPos = -1, nMarkCount = 0;
	int nMinValue = 255 * 4;
	int nMaxValue = 0;
	double dCx[10], dCy[10], area, dMaxArea = 0, dDist, dDistY;
	double dOutValue;
	int nMarkCandiCount = 0;
	int nHisto[256], nMaxHisto = 0, nTh, nMaxSize = 0;
	int nOutValue = 0, nOutValueCnt = 0, nInValue = 0, nInValueCnt = 0;
	int nLimitUp, nLimitDn;

	nLimitUp = MARK_LIMIT_UP;
	if (g_Param.m_nDotLimitUp > 0) nLimitUp = g_Param.m_nDotLimitUp;
	nLimitDn = MARK_LIMIT_DN;
	if (g_Param.m_nDotLimitDn > 0) nLimitDn = g_Param.m_nDotLimitDn;


	left = x - 64; if (left < xMin)   left = xMin;
	right = x + 64; if (right > xMax) right = xMax;
	top = y - 64;   if (top < 0) top = 0;
	bottom = y + 64; if (bottom > nHeight) bottom = nHeight;
	for (i = top; i < bottom - 1; i += 2)
	{
		for (j = left; j < right - 1; j += 2)
		{
			nTmp = 2 * (g_Profile[j] + g_Profile[j + 1]) - (*(fm + pitch * i + j) + *(fm + pitch * i + j + 1) + *(fm + pitch * (i + 1) + j) + *(fm + pitch * (i + 1) + j + 1));
			if (nTmp < nMinValue) nMinValue = nTmp;
			if (nTmp > nMaxValue) nMaxValue = nTmp;
		}
	}
	if (nMinValue / (-4) > nLimitUp) return nRet;  //20보다 밝은게 있으면 마킹아님
	if (nMaxValue / 4 > nLimitDn)    return nRet;  //40보다 어두우면 마킹아님

	memset(l_fmBB, 0, 128 * 128);

	for (i = 0; i < 256; i++)nHisto[i] = 0;
	for (i = top; i < bottom; i++)
		for (j = left; j < right; j++)
			nHisto[*(fm + pitch * i + j)]++;

	for (i = 1; i < 256; i++)
		if (nHisto[i] > nMaxHisto)
		{
			nMaxHisto = nHisto[i];
			nTh = i - nThreshold;
		}

	for (i = top; i < bottom; i++)
		for (j = left; j < right; j++)
			if (*(fm + pitch * i + j) < nTh)
			{
				*(l_fmBB + 128 * (i - top) + (j - left)) = 255;
			}

	g_Chain.SetChainData(1, l_fmBB, 5, 5, 15, 100000, 128, 128);
	nBlob = g_Chain.FastChain(0, 0, 127, 127);
	if (nBlob == 0) return nRet;

	for (i = 0; i < nBlob; i++)
	{
		ll = g_Chain.FindMinX(i);
		rr = g_Chain.FindMaxX(i);
		tt = g_Chain.FindMinY(i);
		bb = g_Chain.FindMaxY(i);
		if ((bb - tt) + (rr - ll) > nMaxSize)
		{
			nMaxSize = (bb - tt) + (rr - ll);
			nMaxPos = i;
		}
	}
	if (nMaxPos >= 0)
	{
		ll = g_Chain.FindMinX(nMaxPos);
		rr = g_Chain.FindMaxX(nMaxPos);
		tt = g_Chain.FindMinY(nMaxPos);
		bb = g_Chain.FindMaxY(nMaxPos);

		if ((rr - ll) * dScale > 1.5 && (rr - ll) * dScale < 4)
		{
			for (i = tt + 1; i < bb - 1; i++)
				for (j = ll + 1; j < rr - 1; j++)
					if (*(l_fmBB + 128 * i + j) == 255)
					{
						nOutValue += *(fm + pitch * (top + i) + (left + j));
						nOutValueCnt++;
					}
			ll_ = ll + (rr - ll) / 4;
			rr_ = rr - (rr - ll) / 4;
			tt_ = tt + (bb - tt) / 4;
			bb_ = bb - (bb - tt) / 4;
			for (i = tt_; i < bb_; i++)
				for (j = ll_; j < rr_; j++)
				{
					nInValue += *(fm + pitch * (top + i) + (left + j));
					nInValueCnt++;
				}
			if (nOutValueCnt > 0 && nInValueCnt > 0)
			{
				if ((double)nInValue / nInValueCnt > (double)nOutValue / nOutValueCnt + 1)
					nRet = 1;
			}
		}
	}

	return nRet;
}
int CheckDotMarking1(LPBYTE fm, int x, int y, int xMin, int xMax, int pitch, int nHeight, int nThreshold, double dScale)
{
	int nRet = 0;
	int i, j;
	int ll, tt, rr, bb;
	int nY, nX, left, top, right, bottom;
	int nTmp, nBlob, nMaxPos = -1, nMarkCount = 0, nLimitUp;
	int nMinValue = 255 * 4;;
	double dCx[10], dCy[10], area, dMaxArea = 0, dDist, dDistY;
	int nMarkCandiCount = 0;
	nThreshold *= 4;


	left = x - 64; if (left < xMin)   left = xMin;
	right = x + 64; if (right > xMax) right = xMax;
	top = y - 64;   if (top < 0) top = 0;
	bottom = y + 64; if (bottom > nHeight) bottom = nHeight;
	for (i = top; i < bottom - 1; i += 2)
	{
		for (j = left; j < right - 1; j += 2)
		{
			nTmp = 2 * (g_Profile[j] + g_Profile[j + 1]) - (*(fm + pitch * i + j) + *(fm + pitch * i + j + 1) + *(fm + pitch * (i + 1) + j) + *(fm + pitch * (i + 1) + j + 1));
			if (nTmp < nMinValue) nMinValue = nTmp;
		}
	}
	nLimitUp = MARK_LIMIT_UP;
	if (g_Param.m_nDotLimitUp > 0) nLimitUp = g_Param.m_nDotLimitUp;
	if (nMinValue / (-4) > nLimitUp) return nRet;  //20보다 밝은게 있으면 마킹아님


	left = x - 512; if (left < xMin)   left = xMin;
	right = x + 512; if (right > xMax) right = xMax;
	top = y - 256;   if (top < 0) top = 0;
	bottom = y + 256; if (bottom > nHeight) bottom = nHeight;

	memset(l_fmBB, 0, 512 * 256);

	nY = 0;
	for (i = top; i < bottom - 1; i += 2)
	{
		nX = 0;
		for (j = left; j < right - 1; j += 2)
		{
			nTmp = 2 * (g_Profile[j] + g_Profile[j + 1]) - (*(fm + pitch * i + j) + *(fm + pitch * i + j + 1) + *(fm + pitch * (i + 1) + j) + *(fm + pitch * (i + 1) + j + 1));
			if (nTmp > nThreshold)
			{
				*(l_fmBB + 512 * nY + nX) = 255;
			}
			nX++;
		}
		nY++;
	}

	g_Chain.SetChainData(1, l_fmBB, 3, 3, 15, 100000, 512, 256);
	nBlob = g_Chain.FastChain(0, 0, nX - 1, nY - 1);

	if (nBlob >= 2)
	{
		if (nBlob > 10) nBlob = 10;
		for (i = 0; i < nBlob; i++)
		{
			area = g_Chain.Chain_Area(i);
			if (area > 0)
				g_Chain.Chain_Center(i, &dCx[i], &dCy[i]);
			else
				dCx[i] = dCy[i] = -100000;

			if (area > dMaxArea)
			{
				dMaxArea = area;
				nMaxPos = i;
			}
			ll = g_Chain.FindMinX(i);
			rr = g_Chain.FindMaxX(i);
			if ((rr - ll) * 2 * dScale > 1.5 && (rr - ll) * 2 * dScale < 5)
				nMarkCandiCount++;
		}

		if (nMaxPos >= 0)
		{
			for (i = 0; i < nBlob; i++)
			{
				if (i == nMaxPos) continue;
				dDist = dScale * (dCx[nMaxPos] - dCx[i]) * 2;
				dDistY = dScale * (dCy[nMaxPos] - dCy[i]) * 2;

				if (dDist < 0) dDist *= -1;
				if (dDistY < 0) dDistY *= -1;

				if ((dDist < 2 && dDistY>1.0) || (dDist > 18 && dDist < 22) || (dDist > 8 && dDist < 12))
					nMarkCount++;
			}
		}
	}
	if (nMarkCount >= 1 || nMarkCandiCount >= 2) nRet = 1;

	return nRet;
}


//영역체크필요없는 함수(Call하지전에 함)
int CheckPressOverkill(LPBYTE fm, int left, int top, int nSizeX, int nSizeY, int pitch, int nKernel, int nBaseValue, double dLimitValue)
{
	int nOverkill = 0;
	int i, j, ii, jj;
	int nOffset = nKernel / 2;
	int right = left + nSizeX;
	int bottom = top + nSizeY;
	int nTmp;
	double dMax, dMin;
	int nMaxX = 0, nMaxY, nMax = -1, nMinX = 0, nMinY, nMin = 100000;
	BYTE fmB[128 * 128];
	if (nOffset < 1) nOffset = 1;
	for (i = top + nOffset; i < bottom - nOffset; i++)
		for (j = left + nOffset; j < right - nOffset; j++)
		{
			nTmp = 0;
			for (ii = i - nOffset; ii <= i + nOffset; ii++)
				for (jj = j - nOffset; jj <= j + nOffset; jj++)
					nTmp += *(fm + pitch * ii + jj);

			if (nTmp > nMax)
			{
				nMax = nTmp;
				nMaxX = j;
				nMaxY = i;
			}
			if (nTmp < nMin)
			{
				nMin = nTmp;
				nMinX = j;
				nMinY = i;
			}
		}
	dMax = (double)nMax / ((nOffset * 2 + 1) * (nOffset * 2 + 1));
	dMin = (double)nMin / ((nOffset * 2 + 1) * (nOffset * 2 + 1));

	if (dMax - nBaseValue < dLimitValue || nBaseValue - dMin < dLimitValue)  nOverkill = 1;
	if (nMaxX<left + 10 || nMaxX>right - 10 || nMinX<left + 10 || nMinX>right - 10) nOverkill = 1;
	//	if( abs(nMaxX-nMinX) > 10) nOverkill=1;

	return nOverkill;
}

int FindMinMaxPos(LPBYTE fmS, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY, int* nMinPosX, int* nMinPosY, int* nMinValue, int* nMaxPosX, int* nMaxPosY, int* nMaxValue)
{
	int ok;
	int i, j;
	int jj;
	int nSum, nAvg;
	int nHalfX, nHalfY;
	int nTmp;
	int nMin = nSizeX * nSizeY * 255 + 255;
	int nMax = -1;

	if (nSizeX % 2 == 0) nSizeX++;
	if (nSizeY % 2 == 0) nSizeY++;

	nHalfX = nSizeX / 2;
	nHalfY = nSizeY / 2;


	//맨 왼쪽 Y에 따른 nSizeX만큼 합 ------------
	for (i = top; i < bottom; i++)
	{
		nSum = 0;
		for (j = left; j < left + nSizeX; j++)
			nSum += *(fmS + pitch * i + j);

		l_SumVer[i] = nSum;
	}
	//--------------------------------------------

	for (j = left + nHalfX; j < right - nHalfX; j++)
	{
		if (j != left + nHalfX) //한칸씩 오른쪽으로 감에 따라 이 값도 변경
		{
			for (i = top; i < bottom; i++)
				l_SumVer[i] = l_SumVer[i] + *(fmS + pitch * i + j - nHalfX + nSizeX - 1) - *(fmS + pitch * i + j - nHalfX - 1);
		}

		nSum = 0;
		for (i = top; i < top + nSizeY; i++)
			nSum += l_SumVer[i];

		for (i = top + nHalfY; i < bottom - nHalfY; i++)
		{
			nTmp = nSum / (nSizeX * nSizeY);
			if (nTmp < 0)		  nTmp = 0;
			else if (nTmp > 255) nTmp = 255;


			if (nTmp < nMin)
			{
				nMin = nTmp;
				*nMinPosX = j;
				*nMinPosY = i;
			}
			else if (nTmp > nMax)
			{
				nMax = nTmp;
				*nMaxPosX = j;
				*nMaxPosY = i;
			}

			nSum = nSum + l_SumVer[i - nHalfY + nSizeY] - l_SumVer[i - nHalfY];
		}
	}

	*nMinValue = nMin;
	*nMaxValue = nMax;

	return 0;
}