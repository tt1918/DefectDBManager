#include "Stdafx.h"
#include "math.h"
#include "KoWebDoc.h"
#include "KoWebView.h"


int FindCandiAllPoint(LPBYTE fm, CRect rc, int pitch, int nAreaX, int nAreaY, int nSMoothSize, CANDIDATA *pCandi);
	int FindCandiOnePoint(LPBYTE fmS, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY, CANDIDATA *pCandi);
int GetValue(LPBYTE fm, CRect rc, int nPosX, int nPosY, int pitch, int nBaseValue, int *Max, int *MaxX, int *MaxY, int *Min, int *MinX, int *MinY);
	int TOO_FindMinMaxPos(LPBYTE fmS, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY, int *nMinPosX,int *nMinPosY, int *nMinValue, int *nMaxPosX, int *nMaxPosY, int *nMaxValue);
int GetSizeVolumn(bool bPress, LPBYTE fm, CRect rc, int nX, int nY, int pitch, int nAveValue, int nIsBlack, int nThresholdUp, int nThresholdDn, double *pSizeV);
double FindStdevDataForNEL5(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, int *pDiffUp, int *pDiffDn);

double CheckOverKillNEL5(LPBYTE fm, CRect rc, int nX, int nY, int pitch, int nSize, int *pDiff); //투영과검
//
//long l_ProjProfile[MAX_WIDTH];
long l_nProfSum[5][MAX_WIDTH];
long l_nProfSumBackup[5][MAX_WIDTH];
long l_nProfAvg[5][MAX_WIDTH];


long l_ProjProfileTD[MAX_WIDTH];
long l_nProfSumTD[25][MAX_WIDTH];
long l_nProfSumBackupTD[25][MAX_WIDTH];
long l_nProfAvgTD[25][MAX_WIDTH];


void WEB_TooYoung()
{
	int i,k,m;
	CRect rect;
	int nLevel, nTempLevel, nFlagInspect=0;
	int nCount, nDefect, nTmp, nDiffValue;
	double dMin, dMax, dStdDiff=1;
	int nWidth=g_System.m_nImageW;
	int nHeight=g_System.m_nImageH;
	int nHeightSub=g_System.m_nImagePartH;
	int pitch=g_System.m_nPitch;
	int nPosX, nPosY, nID=0,  dX, dY;
	int ThUp=256, ThDn=256;
	int nBaseValue=g_System.m_nFlatBright;	//투영 기준밝기
	int nOffsetX=50;
	int nOffsetY=128;
	int nMaxValue=0, nMinValue=255;
	double size[2], sizeShow[2]; 
	int nIsBlack=0, nOverKill;
	int nMaxV, nMinV, nMaxX, nMaxY, nMinX, nMinY;
	double dOverlapPos[2];
	int    nMarkingDefect[2];
	int nBLevel, nWLevel;

	if(g_System.m_nImagePart>25) return;

	rect.left  =g_Temp.m_nInspectX1;
	rect.right =g_Temp.m_nInspectX2;
	rect.top   =0;
	rect.bottom=g_System.m_nImageH;
	
	LPBYTE fm = g_fmFlat[g_ID];

	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])
		{
			if(g_Param.m_nValueUp[i]>0 && g_Param.m_nValueUp[i]<ThUp ) ThUp=g_Param.m_nValueUp[i]; 
			if(g_Param.m_nValueDn[i]>0 && g_Param.m_nValueDn[i]<ThDn ) ThDn=g_Param.m_nValueDn[i]; 			
			if(g_Param.m_nValueUp[i]>0 || g_Param.m_nValueDn[i]>0 || g_Param.m_dSizeUp[i]>0 || g_Param.m_dSizeDn[i]>0) nFlagInspect=1;
		}
	}
	if(ThUp<g_Param.m_nAreaThUp) ThUp=g_Param.m_nAreaThUp;
	if(ThDn<g_Param.m_nAreaThDn) ThDn=g_Param.m_nAreaThDn;
	if(nFlagInspect==0) return;

	l_dTime[4]=Clock_set();

	nCount=g_System.m_nImagePart*(g_System.m_nImageW/256);	// Thread를 이용하여 후보 200개 구함.
	ThreadFunc_FindCandiPointsToo();						// 아래 함수 사용할때와 비교해서 4배 빠른속도
//	nCount=FindCandiAllPoint(fm, rect, nWidth, 256, nHeightSub, 3, g_CanidData); //한개의 함수 내에서 후보 200개 뽑기 (I7에서 24msec)

	l_dTime[5]=Clock_set();

	g_CandiW.m_nCandiCount=g_CandiB.m_nCandiCount=0;
	for(k=0;k<g_System.m_nMaxCandiDefect*2;k++)
	{			
		if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) break;

		if(k%2==0) //백점 흑점 순서대로 진행
		{
			dMax=0;
			nID=-1;
			for(i=0;i<nCount;i++)
			{
				if(g_CanidData[i].dAreaMaxValue>dMax)
				{
					dMax =g_CanidData[i].dAreaMaxValue;
					nPosX=g_CanidData[i].nAreaMaxX;
					nPosY=g_CanidData[i].nAreaMaxY;
					nID=i;
				}
			}
			if(dMax==0 || nID<0) continue;

			//화면표시용--------------------------------------------
			g_CandiW.m_nCandiValue[g_CandiW.m_nCandiCount]	=dMax;
			g_CandiW.m_nCandiX[g_CandiW.m_nCandiCount]		=nPosX;
			g_CandiW.m_nCandiY[g_CandiW.m_nCandiCount]		=nPosY;
			g_CandiW.m_nCandiAvg[g_CandiW.m_nCandiCount]		=0;
			g_CandiW.m_nCandiCount++;
			//------------------------------------------------------

			g_CanidData[nID].dAreaMaxValue=0;   //똑같은 곳이 다시 걸리지 않게
		}
		else   
		{
			dMin=255;
			nID=-1;
			for(i=0;i<nCount;i++)
			{
				if(g_CanidData[i].dAreaMinValue<dMin)
				{
					dMin =g_CanidData[i].dAreaMinValue;
					nPosX=g_CanidData[i].nAreaMinX;
					nPosY=g_CanidData[i].nAreaMinY;
					nID=i;
				}
			}
			if(dMin==255 || nID<0) continue;

			//화면표시용--------------------------------------------
			g_CandiB.m_nCandiValue[g_CandiB.m_nCandiCount]	=-dMin;
			g_CandiB.m_nCandiX[g_CandiB.m_nCandiCount]		=nPosX;
			g_CandiB.m_nCandiY[g_CandiB.m_nCandiCount]		=nPosY;
			g_CandiB.m_nCandiAvg[g_CandiB.m_nCandiCount]		=0;
			g_CandiB.m_nCandiCount++;
			//------------------------------------------------------

			g_CanidData[nID].dAreaMinValue=255;  //똑같은 곳이 다시 걸리지 않게
		}


		nTmp=GetValue(fm, rect, nPosX, nPosY, nWidth, nBaseValue, &nMaxV, &nMaxX, &nMaxY, &nMinV, &nMinX, &nMinY); //3x3으로 Min, Max를 다시 구한다.

		if(nTmp==0) continue;
		if(nMaxV>nMaxValue)		 nMaxValue=nMaxV;
		else if(nMinV<nMinValue) nMinValue=nMinV;
		if(nTmp>0)
		{
			nPosX=nMaxX;
			nPosY=nMaxY;
		}
		else
		{
			nPosX=nMinX;
			nPosY=nMinY;
		}

		if(nMaxV>=ThUp || nMinV<= -ThDn)
		{
			GetSizeVolumn(false, fm, rect, nPosX, nPosY, nWidth, nBaseValue, 0, g_Param.m_nAreaThUp, g_Param.m_nAreaThDn, size);

			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX=size[0]*g_Param.m_dScaleFactorX;					//Size[0]=Size[1]
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY=size[1]*g_Param.m_dScaleFactorX;					//
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size =(size[0]+size[1])*0.5*g_Param.m_dScaleFactorX;	//Size

			nWLevel=GetLevel((double)nMaxV, g_Defect.m_Defect[g_Defect.m_nDefectCount].size, g_Param.m_nValueUp, g_Param.m_dSizeUp); //백Level
			if(nMaxV>= g_Param.m_nValueOneUp && g_Param.m_nValueOneUp>0) nWLevel=1;
			if(nMaxV>= ThUp && g_Param.m_dSizeOneUp>0.1 && g_Defect.m_Defect[g_Defect.m_nDefectCount].size>=g_Param.m_dSizeOneUp) nWLevel=1;

			nBLevel=GetLevel((double)nMinV, g_Defect.m_Defect[g_Defect.m_nDefectCount].size, g_Param.m_nValueDn, g_Param.m_dSizeDn); //흑Level			
			if(nMinV<=-g_Param.m_nValueOneDn && g_Param.m_nValueOneDn>0) nBLevel=1;
			if(nMinV<=-ThDn && g_Param.m_dSizeOneDn>0.1 && g_Defect.m_Defect[g_Defect.m_nDefectCount].size>=g_Param.m_dSizeOneDn) nBLevel=1;

			if(nWLevel>0 && nBLevel>0)
			{
				//NEL5, NELA 구분안해도 됨. 어차피 1 또는 2이므로 
				nMarkingDefect[0]=g_Mark.nMarking[NG_SPOT_WHITE-1][nWLevel-1]; //백 Level 마킹상태					
				nMarkingDefect[1]=g_Mark.nMarking[NG_SPOT_BLACK-1][nBLevel-1]; //흑 Level 마킹상태				

				if(nMarkingDefect[0]==nMarkingDefect[1]) //둘다 마킹하는 불량이거나 마킹하지 않는 불량이면
				{
					if(nWLevel<=nBLevel) nIsBlack=0;    //백우선으로 Level이 낮은 쪽 선택
					else				 nIsBlack=1;    
				}
				else if(nMarkingDefect[0]) //백만 마킹하는 불량이면 백으로 분류
				{
					nIsBlack=0;
				}
				else if(nMarkingDefect[1]) //흑만 마킹하는 불량이면 흑으로 분류
				{
					nIsBlack=1;
				}
			}
			else if(nWLevel>0) //백만 Level내에 들면 백으로 분류
			{
				nIsBlack=0;
			}
			else if(nBLevel>0) //흑만 Level내에 들면 흑으로 분류
			{
				nIsBlack=1;
			}
			else
				continue;		//둘다 Level에 안들면 다른 것으로 TRY	
		}

		if(nIsBlack==1 && nMinV<= -ThDn)  //흑
		{
			nTmp=nMinV;
			nLevel=nBLevel;

			//과검체크---------------------------------------------------------------------------------
			if(g_Defect.m_Defect[g_Defect.m_nDefectCount].size <g_Param.m_dOverKillSize)
			{
				nOverKill=0;
				dStdDiff=CheckOverKillNEL5(fm, rect, nPosX, nPosY, nWidth, 32, &nDiffValue);	
				if(dStdDiff< g_Param.m_dOverKillSTD || nDiffValue<g_Param.m_dOverKillDiff) nOverKill=1;						

				if(nOverKill) continue;
			}			
			//------------------------------------------------------------------------------------------

			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_SPOT_BLACK;     
			MakeDefectRect(g_Defect.m_nDefectCount, nPosX, nPosY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value=nTmp;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nPosX;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nPosY;

			CopyNGImagePart(true);

			if(CheckDefectOverlap()==0)   							
				g_Defect.m_nDefectCount++;
		}
		else if(nIsBlack==0 && nMaxV>=ThUp)  //백
		{
			nTmp=nMaxV;
			nLevel=nWLevel;

			//과검체크---------------------------------------------------------------------------------
			if(g_Defect.m_Defect[g_Defect.m_nDefectCount].size <g_Param.m_dOverKillSize)
			{
				nOverKill=0;
				dStdDiff=CheckOverKillNEL5(fm, rect, nPosX, nPosY, nWidth, 32, &nDiffValue);	
				if(dStdDiff< g_Param.m_dOverKillSTD || nDiffValue<g_Param.m_dOverKillDiff) nOverKill=1;						

				if(nOverKill) continue;
			}			
			//------------------------------------------------------------------------------------------


			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_SPOT_WHITE;     
			MakeDefectRect(g_Defect.m_nDefectCount, nPosX, nPosY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value=nTmp;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nPosX;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nPosY;

			CopyNGImagePart(true);

			if(CheckDefectOverlap()==0)   							
				g_Defect.m_nDefectCount++;
		}
	}
	g_Temp.m_nMaxGray=nMaxValue;
	g_Temp.m_nMinGray=nMinValue;

}

void WEB_TooYoung_Press()
{
	int i,j,k,m;
	CRect rect;
	int nTempLevel;
	int nLevel, nCount,  nTmp, nDiffValue;
	int nFlagInspect=0;
	double dMin, dMax, dStdDiff=1;
	int nWidth=g_System.m_nImageW;
	int nHeight=g_System.m_nImageH;
	int nHeightSub=g_System.m_nImagePartH;
	int pitch=g_System.m_nPitch;
	int nPosX, nPosY, nID=0,  dX, dY;
	int ThUp=256, ThDn=256;
	int nBaseValue=g_System.m_nFlatBright;	//투영 기준밝기
	int nOffsetX=50;
	int nOffsetY=128;
	int nMaxValue=0, nMinValue=255;
	double size[2], sizeShow[2]; 
	int nIsBlack=0, nOverKill;
	int nMaxV, nMinV, nMaxX, nMaxY, nMinX, nMinY;
	double dOverlapPos[2];
	int    nMarkingDefect[2];
	int nWLevel;

	if(g_System.m_nImagePart>25) return;

	rect.left  =g_Temp.m_nInspectX1;
	rect.right =g_Temp.m_nInspectX2;
	rect.top   =0;
	rect.bottom=g_System.m_nImageH;
	
	LPBYTE fm = g_fmFlat[g_ID];

	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])
		{
			if(g_Param.m_nValueUp[i]>0 && g_Param.m_nValueUp[i]<ThUp ) ThUp=g_Param.m_nValueUp[i]; 
			if(g_Param.m_nValueDn[i]>0 && g_Param.m_nValueDn[i]<ThDn ) ThDn=g_Param.m_nValueDn[i]; 			
			if((g_Param.m_nValueUp[i]>0 && g_Param.m_nValueDn[i]>0) && g_Param.m_dSizeUp[i]>0)
			{
				if(g_Param.m_nValueUp[i]<ThUp ) ThUp=g_Param.m_nValueUp[i]; 
			    if(g_Param.m_nValueDn[i]<ThDn ) ThDn=g_Param.m_nValueDn[i]; 	

				nFlagInspect=1;
			}
		}
	}
	if(nFlagInspect==0) return;
	if(ThUp<g_Param.m_nAreaThUp) ThUp=g_Param.m_nAreaThUp;
	if(ThDn<g_Param.m_nAreaThDn) ThDn=g_Param.m_nAreaThDn;

	l_dTime[4]=Clock_set();

	nCount=g_System.m_nImagePart*(g_System.m_nImageW/256);	// Thread를 이용하여 후보 200개 구함.
	ThreadFunc_FindCandiPointsToo();						// 후보 구한다

	l_dTime[5]=Clock_set();

	g_CandiW.m_nCandiCount=g_CandiB.m_nCandiCount=0;
	for(k=0;k<g_System.m_nMaxCandiDefect*2;k++)
	{			
		if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) break;

		if(k%2==0) //백점 흑점 순서대로 진행
		{
			dMax=0;
			nID=-1;
			for(i=0;i<nCount;i++)
			{
				if(g_CanidData[i].dAreaMaxValue>dMax)
				{
					dMax =g_CanidData[i].dAreaMaxValue;
					nPosX=g_CanidData[i].nAreaMaxX;
					nPosY=g_CanidData[i].nAreaMaxY;
					nID=i;
				}
			}
			if(dMax==0 || nID<0) continue;

			//화면표시용--------------------------------------------
			g_CandiW.m_nCandiValue[g_CandiW.m_nCandiCount]	=dMax;
			g_CandiW.m_nCandiX[g_CandiW.m_nCandiCount]		=nPosX;
			g_CandiW.m_nCandiY[g_CandiW.m_nCandiCount]		=nPosY;
			g_CandiW.m_nCandiAvg[g_CandiW.m_nCandiCount]		=0;
			g_CandiW.m_nCandiCount++;
			//------------------------------------------------------

			g_CanidData[nID].dAreaMaxValue=0;   //똑같은 곳이 다시 걸리지 않게
		}
		else   
		{
			dMin=255;
			nID=-1;
			for(i=0;i<nCount;i++)
			{
				if(g_CanidData[i].dAreaMinValue<dMin)
				{
					dMin =g_CanidData[i].dAreaMinValue;
					nPosX=g_CanidData[i].nAreaMinX;
					nPosY=g_CanidData[i].nAreaMinY;
					nID=i;
				}
			}
			if(dMin==255 || nID<0) continue;

			//화면표시용--------------------------------------------
			g_CandiB.m_nCandiValue[g_CandiB.m_nCandiCount]	=-dMin;
			g_CandiB.m_nCandiX[g_CandiB.m_nCandiCount]		=nPosX;
			g_CandiB.m_nCandiY[g_CandiB.m_nCandiCount]		=nPosY;
			g_CandiB.m_nCandiAvg[g_CandiB.m_nCandiCount]		=0;
			g_CandiB.m_nCandiCount++;
			//------------------------------------------------------

			g_CanidData[nID].dAreaMinValue=255;  //똑같은 곳이 다시 걸리지 않게
		}


		nTmp=GetValue(fm, rect, nPosX, nPosY, nWidth, nBaseValue, &nMaxV, &nMaxX, &nMaxY, &nMinV, &nMinX, &nMinY); //3x3으로 Min, Max를 다시 구한다.

		if(nTmp==0) continue;
		if(nMaxV>nMaxValue)		 nMaxValue=nMaxV;
		else if(nMinV<nMinValue) nMinValue=nMinV;
		if(nTmp>0)
		{
			nPosX=nMaxX;
			nPosY=nMaxY;
		}
		else
		{
			nPosX=nMinX;
			nPosY=nMinY;
		}


		if(nMaxV>=ThUp && nMinV<= -ThDn)
		{
			GetSizeVolumn(true, fm, rect, nPosX, nPosY, nWidth, nBaseValue, 0, g_Param.m_nAreaThUp, g_Param.m_nAreaThDn, size);

			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX=size[0]*g_Param.m_dScaleFactorX;					//Size[0]=Size[1]
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY=size[1]*g_Param.m_dScaleFactorX;					//
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size =(size[0]+size[1])*0.5*g_Param.m_dScaleFactorX;	//Size

			nWLevel=GetLevel((double)nMaxV, -1.0*nMinV, g_Defect.m_Defect[g_Defect.m_nDefectCount].size, g_Param.m_nValueUp, g_Param.m_nValueDn, g_Param.m_dSizeUp); //백Level
				

			if(nWLevel>0)  //찍힘
			{
				nTmp=nMaxV-nMinV;
				nLevel=nWLevel;

				//과검체크---------------------------------------------------------------------------------
				if(g_Defect.m_Defect[g_Defect.m_nDefectCount].size <g_Param.m_dOverKillSize)
				{
					nOverKill=0;
					dStdDiff=CheckOverKillNEL5(fm, rect, nPosX, nPosY, nWidth, 32, &nDiffValue);	
					if(dStdDiff< g_Param.m_dOverKillSTD || nDiffValue<g_Param.m_dOverKillDiff) nOverKill=1;						

					if(nOverKill) continue;
				}			
				//------------------------------------------------------------------------------------------


				g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_SPOT_WHITE;     
				MakeDefectRect(g_Defect.m_nDefectCount, nPosX, nPosY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
				g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
				g_Defect.m_Defect[g_Defect.m_nDefectCount].value=nTmp;
				g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nPosX;
				g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nPosY;

				CopyNGImagePart(true);

				if(CheckDefectOverlap()==0)   							
					g_Defect.m_nDefectCount++;
			}
		}
	}
	g_Temp.m_nMaxGray=nMaxValue;
	g_Temp.m_nMinGray=nMinValue;
}


//Thread에서 처리하면서 사용안함.>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int FindCandiAllPoint(LPBYTE fm, CRect rc, int pitch, int nAreaX, int nAreaY, int nSMoothSize, CANDIDATA *pCandi)
{
	int i,j,nCount=0;
	CANDIDATA nData;
	int nCol, nRow;
	int left, top, right, bottom;

	if(rc.right-rc.left<100) return 0;

	nCol=(rc.right-rc.left)/nAreaX;  if((rc.right-rc.left)>nCol*nAreaX) nCol++;
	nRow=(rc.bottom-rc.top)/nAreaY;  if((rc.bottom-rc.top)>nRow*nAreaY) nRow++;

	for(i=0;i<nRow;i++)
	{
		top     =rc.top+nAreaY*i;
		bottom  =top+nAreaY;       if(bottom>rc.bottom) bottom=rc.bottom;

		for(j=0;j<nCol;j++)
		{
			left	=rc.left+nAreaX*j;
			right   =left+nAreaX;  if(right>rc.right) right=rc.right;
			if(j>0) left=left-nSMoothSize;

			FindCandiOnePoint(fm, left,top,right,bottom,pitch, nSMoothSize, nSMoothSize, &pCandi[nCount]);
			nCount++;
		}
	}
	return nCount;
}


//주어진 영역에서 (nSizeX, nSizeY) 영역으로 나누었을때 가장 큰 값과 가장 작은 값을 구함.
int l_nSumVer[MAX_WIDTH];  //이정도면 충분 8192사용하면 넓혀야 함.
int FindCandiOnePoint(LPBYTE fmS, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY, CANDIDATA *pCandi)
{
	int i, j;
	int nSum, nAvg;
	int nHalfX, nHalfY;
	double dTmp, dMax=0, dMin=nSizeX*nSizeY*255;
	int    nMaxPosX=0, nMaxPosY=0, nMinPosX=0, nMinPosY=0;

	if(nSizeX%2==0) nSizeX++;
	if(nSizeY%2==0) nSizeY++;

	nHalfX=nSizeX/2;
	nHalfY=nSizeY/2;


	//맨 왼쪽 Y에 따른 nSizeX만큼 합 ------------
	for(i=top;i<bottom;i++)
	{	
		nSum=0;
		for(j=left;j<left+nSizeX;j++)
			nSum+=*(fmS+pitch*i+j);

		l_nSumVer[i]=nSum;
	}
	//--------------------------------------------

	for(j=left+nHalfX;j<right-nHalfX;j++)
	{
		if(j!=left+nHalfX) //한칸씩 오른쪽으로 감에 따라 이 값도 변경
		{
			for(i=top;i<bottom;i++)
				l_nSumVer[i]=l_nSumVer[i]+*(fmS+pitch*i+j-nHalfX+nSizeX-1)-*(fmS+pitch*i+j-nHalfX-1);
		}

		nSum=0;
		for(i=top;i<top+nSizeY;i++)
			nSum+=l_nSumVer[i];

		for(i=top+nHalfY;i<bottom-nHalfY;i++)
		{
			dTmp=(double)nSum/(nSizeX*nSizeY);						

			if(dTmp<dMin)
			{
				dMin=dTmp;
				nMinPosX=j;
				nMinPosY=i;
			}
			else if(dTmp>dMax)
			{
				dMax=dTmp;
				nMaxPosX=j;
				nMaxPosY=i;
			}
			nSum=nSum+l_nSumVer[i-nHalfY+nSizeY]-l_nSumVer[i-nHalfY];
		}
	}

	pCandi->dAreaMaxValue=dMax;
	pCandi->nAreaMaxX    =nMaxPosX;
	pCandi->nAreaMaxY    =nMaxPosY;

	pCandi->dAreaMinValue=dMin;
	pCandi->nAreaMinX    =nMinPosX;
	pCandi->nAreaMinY    =nMinPosY;

	return 0;
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


int GetValue(LPBYTE fm, CRect rc, int nPosX, int nPosY, int pitch, int nBaseValue, int *Max, int *MaxX, int *MaxY, int *Min, int *MinX, int *MinY)
{
	int i,j;
	int left, top, right, bottom;
	int nSum=0, nCount=0;
	int nTmp, nMax, nMin;
	int nMinPosX, nMinPosY, nMinValue;
	int nMaxPosX, nMaxPosY, nMaxValue;

	if(nPosX<rc.left || nPosX>rc.right) return 0;

	left =nPosX-g_System.m_nImagePartH/2;	 if(left<rc.left)	left=rc.left;
	right=nPosX+g_System.m_nImagePartH/2;    if(right>rc.right)	right=rc.right;
	top  =(int)(nPosY/g_System.m_nImagePartH)*g_System.m_nImagePartH;  
	bottom=top+g_System.m_nImagePartH; 


	TOO_FindMinMaxPos(fm, left, top, right, bottom, pitch, 3, 3, &nMinPosX, &nMinPosY, &nMinValue, &nMaxPosX, &nMaxPosY, &nMaxValue);


	nMin=nMinValue;
	nMax=nMaxValue;

	*Max=(nMax-nBaseValue);
	*MaxX=nMaxPosX;
	*MaxY=nMaxPosY;
	*Min=(nMin-nBaseValue);
	*MinX=nMinPosX;
	*MinY=nMinPosY;

	if((nMax-nBaseValue)>1.5*(nBaseValue-nMin)) return (nMax-nBaseValue);
	else										return (nMin-nBaseValue);
}

extern int l_SumVer[MAX_WIDTH];
int TOO_FindMinMaxPos(LPBYTE fmS, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY, int *nMinPosX,int *nMinPosY, int *nMinValue, int *nMaxPosX, int *nMaxPosY, int *nMaxValue)
{
	int ok;
	int i, j;
	int jj;
	int nSum, nAvg;
	int nHalfX, nHalfY;
	int nTmp;
	int nMin=nSizeX*nSizeY*255+255;
	int nMax=-1;

	if(nSizeX%2==0) nSizeX++;
	if(nSizeY%2==0) nSizeY++;

	nHalfX=nSizeX/2;
	nHalfY=nSizeY/2;


	//맨 왼쪽 Y에 따른 nSizeX만큼 합 ------------
	for(i=top;i<bottom;i++)
	{	
		nSum=0;
		for(j=left;j<left+nSizeX;j++)
			nSum+=*(fmS+pitch*i+j);

		l_SumVer[i]=nSum;
	}
	//--------------------------------------------

	for(j=left+nHalfX;j<right-nHalfX;j++)
	{
		if(j!=left+nHalfX) //한칸씩 오른쪽으로 감에 따라 이 값도 변경
		{
			for(i=top;i<bottom;i++)
				l_SumVer[i]=l_SumVer[i]+*(fmS+pitch*i+j-nHalfX+nSizeX-1)-*(fmS+pitch*i+j-nHalfX-1);
		}

		nSum=0;
		for(i=top;i<top+nSizeY;i++)
			nSum+=l_SumVer[i];

		for(i=top+nHalfY;i<bottom-nHalfY;i++)
		{
			nTmp=nSum/(nSizeX*nSizeY);
			if(nTmp<0)		  nTmp=0;
			else if(nTmp>255) nTmp=255;
			

			if(nTmp<nMin)
			{
				nMin=nTmp;
				*nMinPosX=j;
				*nMinPosY=i;
			}
			else if(nTmp>nMax)
			{
				nMax=nTmp;
				*nMaxPosX=j;
				*nMaxPosY=i;
			}

			nSum=nSum+l_SumVer[i-nHalfY+nSizeY]-l_SumVer[i-nHalfY];
		}
	}

	*nMinValue=nMin;
	*nMaxValue=nMax;

	return 0;
}


BYTE l_fm128Binary[BAD_IMG_WIDTH*128];
BYTE l_fm128BinaryErode[BAD_IMG_WIDTH*128];
int GetSizeVolumn(bool bPress, LPBYTE fm, CRect rc, int nX, int nY, int pitch, int nAveValue, int nIsBlack, int nThresholdUp, int nThresholdDn, double *pSizeV)
{
	int ok=0;
	int i, j, ii,jj,loop, nTmp;
	int left, top, right, bottom, Cx, Cy;
	double dist, cx, cy;
	int nThUp, nThDn;
	int ll,rr,tt,bb;
	double dArea, dAreaMax=0, dTmp;
	int nBlob, nMaxPos=-1;
	int nSumVolumn=0;
	double dMax[2];
	int nMax[2], flag;
	int nHeight=g_System.m_nImagePartH;
	double nCanHeight=16;
	double dRatio=(double)g_System.m_nFlatBright/nAveValue;
	bool bFoundWhite=false;
	bool bFoundBlack=false;
	

	pSizeV[0]=pSizeV[1]=0;

	if(nThresholdUp<5) nThresholdUp=5;
	if(nThresholdDn<5) nThresholdDn=5;

	nThUp=nAveValue+nThresholdUp;
	nThDn=nAveValue-nThresholdDn;

	//범위 구함 ------------------------------------------------------
	top=(nY/nHeight)*nHeight;
	bottom=top+nHeight;
	left=nX-64;     if(left<rc.left) left=rc.left;
	right=left+128; if(right>rc.right){right=rc.right; left=right-128;}
	if(left<rc.left) left=rc.left;
	//-----------------------------------------------------------------
	Cx=nX-left;
	Cy=nY-top;

	//WHITE Binary 구하기-----------------------------------------------------------------
	flag=0;
	for(ii=0;ii<3;ii++)
	{
		for(i=top;i<bottom;i++)
				for(j=left;j<right;j++)
				{
					nTmp=*(fm+pitch*i+j);

					if(nTmp>=nThUp)	*(l_fm128Binary+128*(i-top)+(j-left))=255;
					else			*(l_fm128Binary+128*(i-top)+(j-left))=0;
				}

		g_Chain.SetChainData(1, l_fm128Binary, 1, 1, 4, 1000, 128, nHeight);
		nBlob=g_Chain.FastChain(1,1,(right-left)-2,nHeight-2);
		if(nBlob>=10)
		{
			nThUp+=6;
			flag=1;
		}
	}

	dMax[0]=dMax[1]=0;
	nMax[0]=nMax[1]=-1;
	for(i=0;i<nBlob;i++)
	{
		dArea=g_Chain.Chain_Area(i);
		if(dArea>0)
		{
			bFoundWhite=true;
			g_Chain.Chain_Center(i,&cx, &cy);			
			dist=sqrt((cx-Cx)*(cx-Cx)+(cy-Cy)*(cy-Cy));
			if(dist<32)
			{
				if(dArea>=dMax[0] || dArea>=dMax[1])
				{
					if(dArea>=dMax[0])
					{
						dMax[1]=dMax[0];
						nMax[1]=nMax[0];

						dMax[0]=dArea;
						nMax[0]=i;
					}
					else if(dArea>=dMax[1])
					{
						dMax[1]=dArea;
						nMax[1]=i;
					}									
				}
			}
		}
	}

	for(loop=0;loop<2;loop++)
	{
		if(nMax[loop]>=0) i=nMax[loop];
		else              continue; 

		ll=g_Chain.FindMinX(i);
		rr=g_Chain.FindMaxX(i);
		tt=g_Chain.FindMinY(i);
		bb=g_Chain.FindMaxY(i);
		for(ii=tt;ii<=bb;ii++)
			for(jj=ll;jj<=rr;jj++)
			{
				if(*(l_fm128Binary+128*ii+jj))
				{
					nTmp=*(fm+pitch*(top+ii)+(left+jj))-nThUp; 
					if(nTmp<0) 
						nTmp*=-1;
					nSumVolumn+=nTmp;
				}
			}	

		if(flag) break;
	}

	flag=0;
	for(ii=0;ii<3;ii++)
	{
		for(i=top;i<bottom;i++)
				for(j=left;j<right;j++)
				{
					nTmp=*(fm+pitch*i+j);

					if(nTmp <=nThDn)	*(l_fm128Binary+128*(i-top)+(j-left))=255;
					else				*(l_fm128Binary+128*(i-top)+(j-left))=0;
				}


		g_Chain.SetChainData(1, l_fm128Binary, 1, 1, 4, 1000, 128, nHeight);
		nBlob=g_Chain.FastChain(1,1,(right-left)-2,nHeight-2);

		if(nBlob>=10)
		{
			nThDn-=6;
			flag=1;
		}
	}

	dMax[0]=dMax[1]=0;
	nMax[0]=nMax[1]=-1;
	for(i=0;i<nBlob;i++)
	{
		dArea=g_Chain.Chain_Area(i);
		if(dArea>0)
		{
			bFoundBlack=true;
			g_Chain.Chain_Center(i,&cx, &cy);
			dist=sqrt((cx-Cx)*(cx-Cx)+(cy-Cy)*(cy-Cy));
			if(dist<32)
			{
				if(dArea>=dMax[0] || dArea>=dMax[1])
				{
					if(dArea>=dMax[0])
					{
						dMax[1]=dMax[0];
						nMax[1]=nMax[0];

						dMax[0]=dArea;
						nMax[0]=i;
					}
					else if(dArea>=dMax[1])
					{
						dMax[1]=dArea;
						nMax[1]=i;
					}									
				}
			}
		}
	}

	for(loop=0;loop<2;loop++)
	{
		if(nMax[loop]>=0) i=nMax[loop];
		else              continue;

		ll=g_Chain.FindMinX(i);
		rr=g_Chain.FindMaxX(i);
		tt=g_Chain.FindMinY(i);
		bb=g_Chain.FindMaxY(i);
		for(ii=tt;ii<=bb;ii++)
			for(jj=ll;jj<=rr;jj++)
			{
				if(*(l_fm128Binary+128*ii+jj))
				{
					nTmp=nThDn-*(fm+pitch*(top+ii)+(left+jj));
					if(nTmp<0)
						nTmp*=-1;

					nSumVolumn+=nTmp;
				}
			}	

		if(flag) break;
	}
	if(bPress)
	{
		if(bFoundBlack && bFoundWhite)
		{
	if(nSumVolumn>20) pSizeV[0]=pSizeV[1]=2.0*sqrt((nSumVolumn*dRatio/nCanHeight)/3.141592);
		}
	}
	else
	{
		if(nSumVolumn>20) pSizeV[0]=pSizeV[1]=2.0*sqrt((nSumVolumn*dRatio/nCanHeight)/3.141592);
	}
	return ok;
}

double CheckOverKillNEL5(LPBYTE fm, CRect rc, int nX, int nY, int pitch, int nSize, int *pDiff) //투영과검
{
	int i,j,k;
	double dSTD[3], dStd1, dStd2, dStd3, dMaxSTD=0, dSum=0;
	int left, top, right, bottom, nPos;
	int nDiffUp[3], nDiffDn[3], nDiff1, nDiff2, nDiff3, nDiffSumUp=0, nDiffSumDn=0;
	int nBlock;

	//범위 구함 ------------------------------------------------------
	top=nY-nSize/2; if(top<0) top=0;
	bottom=top+nSize; if(bottom>=g_System.m_nImageH){bottom=g_System.m_nImageH-1; top=bottom-nSize;}

	for(i=1;i<g_System.m_nImagePart;i++)
	{
		if( (g_System.m_nImagePartH*i-top)*(g_System.m_nImagePartH*i-bottom)<0)
		{
			if((int)(nY/g_System.m_nImagePartH)==(i-1))
			{
				bottom=g_System.m_nImagePartH*i-1;
				top=bottom-nSize;
			}
			else
			{
				top=g_System.m_nImagePartH*i;
				bottom=top+nSize;
			}
		}
	}

	left=nX-64;     if(left<rc.left) left=rc.left;
	right=left+128; if(right>rc.right){right=rc.right; left=right-128;}
	if(left<rc.left) left=rc.left;
	//-----------------------------------------------------------------

	for(k=0;k<3;k++)
	{
		dSTD[k]=FindStdevDataForNEL5(fm, left+k*48, top, left+k*48+nSize, bottom, pitch, &nDiffUp[k], &nDiffDn[k]);
		if(dSTD[k]>dMaxSTD)
		{
			dMaxSTD=dSTD[k];
			nPos=k;
		}
	}
	for(k=0;k<3;k++)
	{
		if(k==nPos) continue;
		dSum+=dSTD[k];
		nDiffSumUp+=nDiffUp[k];
		nDiffSumDn+=nDiffDn[k];
	}

	dStd1=FindStdevDataForNEL5(fm, left+nPos*48, top%g_System.m_nImagePartH, left+nPos*48+nSize, bottom%g_System.m_nImagePartH, pitch, &nDiff1, &nDiff2);
	dStd2=FindStdevDataForNEL5(fm, left+nPos*48, g_System.m_nImagePartH*(g_System.m_nImagePart-1)+top%g_System.m_nImagePartH, left+nPos*48+nSize, g_System.m_nImagePartH*(g_System.m_nImagePart-1)+bottom%g_System.m_nImagePartH, pitch, &nDiff1, &nDiff2);
	nBlock=(int)nY/g_System.m_nImagePartH;

	if(nBlock==0)								{dStd3=dStd2; }
	else if(nBlock==(g_System.m_nImagePart-1))	{dStd3=dStd1; }
	else										{dStd3=(dStd1+dStd2)/2.0;}

	if(nDiffUp[nPos]-nDiffSumUp/2>nDiffDn[nPos]-nDiffSumDn/2) *pDiff=nDiffUp[nPos]-nDiffSumUp/2;
	else													  *pDiff=nDiffDn[nPos]-nDiffSumDn/2;

	if(dStd3>dSum/2.0)  return (dMaxSTD-dStd3);
	else				return (dMaxSTD-dSum/2.0);
}

double FindStdevDataForNEL5(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, int *pDiffUp, int *pDiffDn)
{
	int i,j;
	long nSum=0;
	int tmp, nTop, nBottom;
	double dSum2=0, stdev=0; 
	long   nArea;
	int nMin=255, nMax=0;

	nArea=(x2-x1)*(y2-y1);

	for(i=y1;i<y2;i++)
		for(j=x1;j<x2;j++)
		{
			tmp=*(fm+pitch*i+j);
			nSum+=tmp;
			dSum2+=(tmp*tmp);			
			if(tmp<nMin) nMin=tmp;
			if(tmp>nMax) nMax=tmp;
		}

	if((nArea*dSum2-(double)nSum*(double)nSum)>0 && ((double)nArea*((double)nArea-1.))>0)
		stdev=sqrt((nArea*dSum2-(double)nSum*(double)nSum)/((double)nArea*((double)nArea-1.)));	

	*pDiffDn=g_System.m_nFlatBright-nMin;
	*pDiffUp=nMax-g_System.m_nFlatBright;

	return stdev;
}


//투영 MD라인 검출-----------------------------------------------------------------------------------------------------
void TOO_FindMDLine()
{
	int nWidth=g_System.m_nImageW;
	int nHeight=g_System.m_nImageH;
	int nHeightSub=g_System.m_nImagePartH;
	int pitch=g_System.m_nPitch;

	int nLevel;
	int i,j,k,m,nX[2], nY[2], nValue[2], nAngle[2], nX_[2], nValue_[2], nAngle_[2],nMaxValue=0, nMaxValue1=0, nMaxX=0, nMaxY=0;
	int nPosX, nPosY;
	int nFlagInspect=0;
	CRect rect;
	LPBYTE fm = g_fmGrab[g_ID];
	LPBYTE fmDest;

	rect.left  =g_Temp.m_nInspectX1;
	rect.right =g_Temp.m_nInspectX2;
	rect.top   =0;
	rect.bottom=g_System.m_nImageH;

	int nPitchDest;
	int nOverKill;
	int nOverKillLine[2];
	int tt,rr,bb,ll,nShowOffset=640, nYi, nXi, ii, jj, nYY;
	int nValueArray[10], nValuePosArray[10],nValueAngle[10], nMaxM, nMinM;
	int nMethod=0;
	int nMinLineThreshold=10000, nMinLineThresholdAngle=10000;
	int nNGType;

	//검사하기위한 데이타가 들어 있는지 확인---------------
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])
		{
			if(g_Param.m_dLineThreshold[i]>100 && g_Param.m_dLineThreshold[i]<nMinLineThreshold )				 nMinLineThreshold=g_Param.m_dLineThreshold[i]; 
			if(g_Param.m_dLineThresholdAngle[i]>100 && g_Param.m_dLineThresholdAngle[i]<nMinLineThresholdAngle ) nMinLineThresholdAngle=g_Param.m_dLineThresholdAngle[i]; 
 			
			if(nMinLineThreshold<10000 || nMinLineThresholdAngle<10000) nFlagInspect=1;
		}
	}
	if(nFlagInspect==0) return;
	//-----------------------------------------------------


	if(rect.left<0) rect.left=0;
	else if(rect.left>=pitch)   rect.left=pitch-1;
	if(rect.right<0) rect.right=0;
	else if(rect.right>=pitch) rect.right=pitch-1;


	//영상 전체 프로파일 구함----------------------
	//memset(	l_ProjProfile, 0, pitch*sizeof(long));
	//for(i=0;i<nHeight;i+=4)
	//	for(j=0;j<pitch;j++)
	//		l_ProjProfile[j]+=*(fm+pitch*i+j);

	//for(j=0;j<pitch;j++)
	//	l_ProjProfile[j]/=(nHeight/4);
	//----------------------------------------------


	//LINE검사 (Y를 5영역으로 나누어서 한영역에서 최대 2개 검출)---------------------------------------------------------------------
	g_Temp.m_nMaxLineValue=0;
	g_Temp.m_nMaxAngleLineValue=0;
	nMaxValue=0;
	for(i=0;i<10;i++)
		g_Temp.m_TooValue[i]=g_Temp.m_ToonAngle[i]=0;

	ThreadFunc_FindMDLine(fm, g_Temp.m_nInspectX1 , g_Temp.m_nInspectX2,  nHeightSub, pitch);	//원래 20msec 걸리던 것을 Thread로 만들고 7~9 msec로 됨.

	for(i=0;i<5;i++)
	{
		nX_[0]			=g_Temp.m_TooX[2*i];
		nValue_[0]		=g_Temp.m_TooValue[2*i];
		nAngle_[0]		=g_Temp.m_ToonAngle[2*i];
		nOverKillLine[0]=g_Temp.m_ToonOverKill[2*i];
	
		nX_[1]			=g_Temp.m_TooX[2*i+1];
		nValue_[1]		=g_Temp.m_TooValue[2*i+1];
		nAngle_[1]		=g_Temp.m_ToonAngle[2*i+1];
		nOverKillLine[1]=g_Temp.m_ToonOverKill[2*i+1];
					
//		FindLineBW_TOO_A(i, 1, fm, rect.left, 5*i*nHeightSub, rect.right, 5*(i+1)*nHeightSub, pitch, nX_, nValue_, nAngle_, nOverKillLine); (원래)

		for(j=0;j<2;j++)
		{
			if(nOverKillLine[j]==1)
			{
				nValueArray[2*i+j]	   =0;
				nValuePosArray[2*i+j]  =0;
				nValueAngle[2*i+j]     =0;
			}
			else
			{
				nValueArray[2*i+j]	   =nValue_[j];
				nValuePosArray[2*i+j]  =nX_[j];
				nValueAngle[2*i+j]     =nAngle_[j];
			}
			
			if(nAngle_[j]<=82)
			{
				if(nValue_[j]>g_Temp.m_nMaxAngleLineValue)
				{
					g_Temp.m_nMaxAngleLineValue=  nValue_[j];
					g_Temp.m_nMaxAngleLinePos  =  nX_[j];
				}
			}
			else
			{
				if(nValue_[j]>g_Temp.m_nMaxLineValue)
				{
					g_Temp.m_nMaxLineValue=nValue_[j];
					g_Temp.m_nMaxLinePos  =nX_[j];
				}
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------------------------------	

	for(i=0;i<5;i++)	//MD라인 최대 4개 구함.
	{
		nValue[0]=0;
		nMaxM=-1;
		for(m=0;m<10;m++)
		{
			if(nValueArray[m]>nValue[0])
			{
				nValue[0]=nValueArray[m];
				nX[0]    =nValuePosArray[m];
				nY[0]    =((m/2)*5+2.5)*nHeightSub;
				nAngle[0]=nValueAngle[m];
				nMaxM    =m;
			}
		}
		if(nMaxM<0) continue;
		nValueArray[nMaxM]=0;
		if(nValuePosArray[nMaxM]<0) continue;

		
#ifdef USE_HOLE_FIND	
		if(!IsDefectInTP(nX[0], 0)) continue;
#endif
		if(nAngle[0]<=82) { nLevel=GetLevel(nValue[0], g_Param.m_dLineThresholdAngle); nNGType=NG_LINE_ANGLE;}   //사선라인: 투영일때 3  투영반사 5
		else              { nLevel=GetLevel(nValue[0], g_Param.m_dLineThreshold);      nNGType=NG_LINE;}         //MD라인  : 투영일때 4  투영반사 4

		if(nLevel>0 )
		{
			nPosX=nX[0];
			nPosY=nY[0];

			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+nNGType;     
			MakeDefectRect(g_Defect.m_nDefectCount, nPosX, nPosY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value=nValue[0];
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nPosX;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nPosY;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX=1;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY=1;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size =1;

//			CopyNGImagePart(true);			
			//불량영상저장---------------------------------------------------------------
			{
				fmDest	    = g_Defect.m_pImage[g_Defect.m_nDefectCount];
				nPitchDest	= BAD_IMG_WIDTH;
				
				//Y범위
				tt=nPosY-nShowOffset/2; if(tt<0) tt=0;
				bb=tt+nShowOffset;
				if(bb>nHeight)
				{
					bb=nHeight;
					tt=bb-nShowOffset;
				}
				//X범위
				ll=nPosX-nShowOffset/2; if(ll<rect.left) ll=rect.left;
				rr=ll+nShowOffset;
				if(rr>rect.right-1)
				{
					rr=rect.right-1;
					ll=rr-nShowOffset;
					if(ll<rect.left) ll=rect.left;
				}

				nOverKill=CheckOverKillNELALine(fm, nPosX, nPosY, pitch, nShowOffset, rect.left, rect.right, 0.05);
				if(nOverKill) continue;				

				for(ii=0;ii<BAD_IMG_HEIGHT;ii++)
				{
					if(ii<100) nYY=ii/25;
					else       nYY=4;

					if(ii<125) nYi=tt+nYY*nHeightSub+26+(ii%25)*3;
					else	   nYi=tt+nYY*nHeightSub+26+(ii-100)*3;

					for(jj=0;jj<BAD_IMG_HEIGHT;jj++)
					{
						nXi=ll+(jj)*(rr-ll)/BAD_IMG_HEIGHT;
						*(fmDest+nPitchDest*ii+jj)=(*(fm+pitch*nYi+nXi)+*(fm+pitch*nYi+nXi+1))/2;
					}
				}
				//---------------------------------------------------------------------

				if(CheckDefectOverlap()==0)   							
					g_Defect.m_nDefectCount++;
			}			
		}
	}
}

//투영 TD라인 검출-----------------------------------------------------------------------------------------------------
void TOO_FindTDLine()
{
	int nWidth=g_System.m_nImageW;
	int nHeight=g_System.m_nImageH;
	int nHeightSub=g_System.m_nImagePartH;
	int pitch=g_System.m_nPitch;

	int nLevel, nNGCount=0;
	int i,j, ii, jj, k,m, nValue, nX_, nValue_, nMaxValue=0, nMaxValue1=0, nMaxX=0, nMaxY=0;
	int nPosX, nPosY;
	int nFlagInspect=0;
	CRect rect;
	LPBYTE fm = g_fmFlat[g_ID];
	LPBYTE fmDest;

	rect.left  =g_Temp.m_nInspectX1;
	rect.right =g_Temp.m_nInspectX2;
	rect.top   =0;
	rect.bottom=nHeight;

	int nPitchDest;
	int nOverKill;
	int nOverKillLine[2];
	int tt,rr,bb,ll,nShowOffset=640, nYi, nXi, nYY;
	int nMinLineThreshold=10000;
	int nTDSum=0, nTDSumCnt=0, nTDAvg, nStart, nEnd;
	int nMaxPosI, nMaxPosJ;
	int NUM_BUFFERS=25;
	int  nCandiX[25][8], nCandiY[25][8], nCandiV[25][8];


	//검사하기위한 데이타가 들어 있는지 확인---------------
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])
		{
			if(g_Param.m_dLineThresholdTD[i]>100 && g_Param.m_dLineThresholdTD[i]<nMinLineThreshold )	nMinLineThreshold=g_Param.m_dLineThresholdTD[i]; 
			
			if(nMinLineThreshold<10000) nFlagInspect=1;
		}
	}
	if(nFlagInspect==0) return;
	//-----------------------------------------------------


	if(rect.left<0) rect.left=0;
	else if(rect.left>=pitch)   rect.left=pitch-1;
	if(rect.right<0) rect.right=0;
	else if(rect.right>=pitch) rect.right=pitch-1;

	//LINE검사 (Y를 5영역으로 나누어서 한영역에서 최대 2개 검출)---------------------------------------------------------------------
	


	//수평라인 모두 찾음(256x128영역으로) 
	for(j=0;j<pitch;j++)
		l_ProjProfileTD[j]=g_System.m_nFlatBright;

	g_Temp.m_nMaxLineValueTD=0;
	nTDSum=nTDSumCnt=0;
	memset(g_Temp.m_TooTDValue, 0, 25*8*sizeof(int));
	ThreadFunc_FindTDLine(fm, rect.left , rect.right,  nHeightSub, pitch);

	for(i=0;i<NUM_BUFFERS;i++)
		for(j=0;j<8;j++)
		{
			nTDSum+=g_Temp.m_TooTDValue[i][j];
			nTDSumCnt++;

			nCandiV[i][j]=g_Temp.m_TooTDValue[i][j];
			nCandiX[i][j]=g_Temp.m_TooTDX[i][j];
			nCandiY[i][j]=i*nHeightSub+nHeightSub/2;
		}
	if(nTDSumCnt>0) nTDAvg=nTDSum/nTDSumCnt;
	else            nTDAvg=0;


	for(j=0;j<20;j++) //최대 8개 검출
	{
		nValue=0;
		nMaxPosI=-1;
		for(i=0;i<NUM_BUFFERS;i++)
			for(jj=0;jj<8;jj++)
			{
				if(nCandiV[i][jj]>nValue)
				{
					nValue=nCandiV[i][jj];
					nMaxPosI=i;
					nMaxPosJ=jj;
				}
			}		
		if(nMaxPosI<0) continue;

		nCandiV[nMaxPosI][nMaxPosJ]=0;

		nValue=nValue-nTDAvg;
		nPosX=nCandiX[nMaxPosI][nMaxPosJ];
		nPosY=nCandiY[nMaxPosI][nMaxPosJ];

		//제일 큰 것 화면에 보이기 위해서 backup해 둠----------
		if(j==0)
		{
			g_Temp.m_nMaxLineValueTD=  nValue;
			g_Temp.m_nMaxLinePosTD  =  nPosX;
		}
		//-----------------------------------------------------

		if(nValue<nMinLineThreshold) break;
		if(g_Temp.m_nPCOptic==COSA)
		{
			nOverKill=CheckOverKillNELALine(fm, nPosX, nPosY, pitch, nShowOffset, rect.left, rect.right, 0.1);
				if(nOverKill) continue;
		}


		
#ifdef USE_HOLE_FIND	
		if(!IsDefectInTP(nX[0], 0)) continue;
#endif
		nLevel=GetLevel(nValue, g_Param.m_dLineThresholdTD);

		if(nLevel>0 )
		{
		    g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_LINE_TD;     
			MakeDefectRect(g_Defect.m_nDefectCount, nPosX, nPosY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value=nValue;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nPosX;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nPosY;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX=2;		//TD라인은 SIze 2
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY=2;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size =2;

//			CopyNGImagePart(true);			
			//불량영상저장---------------------------------------------------------------
			{
				fmDest	    = g_Defect.m_pImage[g_Defect.m_nDefectCount];
				nPitchDest	= BAD_IMG_WIDTH;
				
//Y범위
				tt=nPosY-nShowOffset/2; if(tt<0) tt=0;
				bb=tt+nShowOffset;
				if(bb>nHeightSub*NUM_BUFFERS)
				{
					bb=nHeightSub*NUM_BUFFERS;
					tt=bb-nShowOffset;
				}

				//X범위
				ll=nPosX-nShowOffset/2; if(ll<rect.left) ll=rect.left;
				rr=ll+nShowOffset;
				if(rr>rect.right-1)
				{
					rr=rect.right-1;
					ll=rr-nShowOffset;
					if(ll<rect.left) ll=rect.left;
				}
			
				for(ii=0;ii<BAD_IMG_HEIGHT;ii++)
				{
					nYi=tt+(ii)*(bb-tt)/BAD_IMG_HEIGHT;
					for(jj=0;jj<BAD_IMG_WIDTH;jj++)
					{
						nXi=ll+(jj)*(rr-ll)/BAD_IMG_WIDTH;
						*(fmDest+nPitchDest*ii+jj)=*(fm+pitch*nYi+nXi);
					}
				}					

				//---------------------------------------------------------------------

				if(CheckDefectOverlap()==0)   							
				{
					g_Defect.m_nDefectCount++;
					nNGCount++;
				}
				if(nNGCount>=8) break;
			}			
		}
	}
}

int FindLineBW_TOO_A(int nID, int nType, LPBYTE fm, int left, int top, int right, int bottom, int pitch, int *pX, int *pValue, int *pAngle, int *pOverKill)
{
	int i, j, k, ii, loop;
	int nTmp, nTmp1;
	long nSum, nMaxSum[2], nMaxPos[2];
	double dOffset, nEndOFf=12;
	int nLeft, nRight;
	int nDiff, nDiffSum=0;
	int nCnt, nCount, nY1, nY2, nX1, nX2,  nXOff=1;
	CString str;
	int nDiffArraySum=0, nDiffArrayCnt=0, nDiffAvg;
	int nMinAreaAvg, nMaxAreaAvg;
	int  nMaxI, nMaxJ[2], nPos;
	long nXDiffArray[LINE_YSTEP][LINE_XSTEP],nXDiff[LINE_XSTEP], nXDiffMax;
	int nAngle, nXDiffMaxArray[LINE_YSTEP];
//	int nOFF=g_Level.m_nOverKillFilter/2;
	int nNoApply=1, nFoundMountain=0, nAvgCrest=0;
	int nMaxProfile, nMinProfile, nMaxProfilePos, nMinProfilePos;
	long *pProfSum, *nProfSumBackup, *pProfAvg;

	if(left<5)			left=5;										//1mm씩 버림(노이즈)
	if(right>g_System.m_nImageW-5)   right=g_System.m_nImageW-5;	//1mm씩 버림(노이즈)

	pValue[0]=pX[0]=pValue[1]=pX[1]=pOverKill[0]=pOverKill[1]=0;
	pAngle[0]=pAngle[1]=90;
	nMaxPos[0]=nMaxPos[1]=-1;

	if(right-left<128) return 0; 

	pProfSum=l_nProfSum[nID];
	nProfSumBackup=l_nProfSumBackup[nID];
	pProfAvg=l_nProfAvg[nID];
	
	//Profile 구하기---------------------------------
	memset(nProfSumBackup, 0, right*sizeof(long));
	memset(pProfSum, 0, right*sizeof(long));	
	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
			pProfSum[j]+=*(fm+pitch*i+j);

	for(j=left;j<right;j++)
		pProfAvg[j]=pProfSum[j]/(bottom-top);

	nFoundMountain=0;
	if(g_Param.m_nOverKillCount>0 && g_Param.m_nOverKillValue>100) 
	{
		nAvgCrest=g_Temp.m_nMountainCnt[0]+g_Temp.m_nMountainCnt[1]+g_Temp.m_nMountainCnt[2]+g_Temp.m_nMountainCnt[3]+g_Temp.m_nMountainCnt[4];
		nFoundMountain=FindMountainCount(pProfSum, left, right, g_Param.m_nOverKillValue*(bottom-top)/100);
		if(top/(g_System.m_nImagePartH*5)<5) g_Temp.m_nMountainCnt[top/(g_System.m_nImagePartH*5)]=nFoundMountain;
		if(nFoundMountain>=g_Param.m_nOverKillCount || nAvgCrest>=g_Param.m_nOverKillCount*5) //현재영역의 산, 또는 직전 5영역의 산의 평값이 설정된 산보다 크면 신규로직 적용한다.
			nNoApply=0; //주변데이타로 이걸 설정해야 한다.

		if(nNoApply==0)
		{
			nEndOFf=4;
		}
	}

	for(loop=0;loop<LINE_YSTEP;loop++)
	{
		memset(nXDiff, 0, LINE_XSTEP*sizeof(long));

		nY1=top+g_System.m_nImagePartH*(loop/3)+16+32*(loop%3);
		nY2=nY1+32;

		//상 하 +/- 25 CUT 해서 다시 Profile구함 --------------------
		memset(pProfSum, 0, right*sizeof(long));
		for(j=left;j<right;j++)
		{
			for(i=nY1;i<nY2;i++)
			{
				if(*(fm+pitch*i+j)<pProfAvg[j]-25)
				{
					pProfSum[j]+=(pProfAvg[j]-25);
				}
				else if(*(fm+pitch*i+j)>pProfAvg[j]+25)
				{
					pProfSum[j]+=(pProfAvg[j]+25);
				}
				else
					pProfSum[j]+=*(fm+pitch*i+j);
			}
		}
		for(j=left;j<right;j++) nProfSumBackup[j]+=pProfSum[j];
		//------------------------------------------------------------

		for(j=left;j<right-40;j++)
		{
			dOffset=(nY2-nY1)*abs(j-g_System.m_nImageW/2)/(g_System.m_nImageW/2);
			if(j<100)							dOffset*=3;
			else if(i<150)						dOffset*=2;
			else if(j>g_System.m_nImageW-75)	dOffset*=3;
			else if(j>g_System.m_nImageW-150)	dOffset*=2;

//			if(nNoApply || g_Level.m_nOverKillFilter<10 || g_Level.m_nOverKillFilter>=50)  //기존과 같음
//			{
				if(nType==0)
				{
					nTmp=pProfSum[j]+pProfSum[j+1]+pProfSum[j+2]+pProfSum[j+3]+pProfSum[j+4];

					//30Pixel 간격
					nDiff=pProfSum[j+30]+pProfSum[j+31]+pProfSum[j+32]+pProfSum[j+33]+pProfSum[j+34]-nTmp;
					nDiff/=5;
					if(nDiff<0) nDiff*=-1;
					nDiff-=2*dOffset;
					if(nDiff>nXDiff[(int)(j+17)/LINE_XGAP])  //17인 이유는 중간값 2개 더해서 /2 임 (2+32)/2=17
						nXDiff[(int)(j+17)/LINE_XGAP]=nDiff;	

					//////20Pixel 간격
					nDiff=pProfSum[j+20]+pProfSum[j+21]+pProfSum[j+22]+pProfSum[j+23]+pProfSum[j+24]-nTmp;
					nDiff/=5;
					if(nDiff<0) nDiff*=-1;
					nDiff-=1.33*dOffset;
					if(nDiff>nXDiff[(int)(j+12)/LINE_XGAP])
						nXDiff[(int)(j+12)/LINE_XGAP]=nDiff;

					////7Pixel을 띄고
					nDiff=(pProfSum[j+7]+pProfSum[j+8]+pProfSum[j+9]-(pProfSum[j]+pProfSum[j+1]+pProfSum[j+2]))*1.8;
					nDiff/=3;

					if(nDiff<0) nDiff*=-1;
					nDiff-=(200+0.53*dOffset); if(nDiff<0) nDiff=0;

					if(nDiff>nXDiff[(int)(j+4)/LINE_XGAP])
						nXDiff[(int)(j+4)/LINE_XGAP]=nDiff;
				}
				else
				{
					//15Pixel내에서 최고점-최저점---------------------------
					nMaxProfile=nMaxProfilePos=nMinProfilePos=0;
					nMinProfile=100000;
					for(k=j;k<j+nEndOFf;k++)
					{
						if(pProfSum[k]+pProfSum[k+1]+pProfSum[k+2]>nMaxProfile) {nMaxProfile=pProfSum[k]+pProfSum[k+1]+pProfSum[k+2];nMaxProfilePos=k;}
						if(pProfSum[k]+pProfSum[k+1]+pProfSum[k+2]<nMinProfile) {nMinProfile=pProfSum[k]+pProfSum[k+1]+pProfSum[k+2];nMinProfilePos=k;}
					}
					nDiff=(nMaxProfile-nMinProfile)/2; //1.5배 한것임.
					nTmp=nMaxProfilePos-nMinProfilePos; if(nTmp<0) nTmp*=(-1);
					nDiff-=(64+(nTmp*dOffset/15)); if(nDiff<0) nDiff=0;
					nMaxProfilePos=(nMaxProfilePos+nMinProfilePos)/2;
					if(nDiff>nXDiff[(int)(nMaxProfilePos)/LINE_XGAP])
						nXDiff[(int)(nMaxProfilePos)/LINE_XGAP]=nDiff;
					//-------------------------------------------------------
				}
			
		}

		for(j=0;j<LINE_XSTEP;j++)
			nXDiffArray[loop][j]=nXDiff[j];
	}
	//X 5칸식 합을 구함----------------------------------------------------------
	memset(nXDiff, 0, LINE_XSTEP*sizeof(long));
#if LINE_XGAP==16
	for(i=0;i<LINE_YSTEP;i++)
		for(j=2;j<LINE_XSTEP-2;j++)
			nXDiff[j]+=nXDiffArray[i][j-2]+nXDiffArray[i][j-1]+nXDiffArray[i][j]+nXDiffArray[i][j+1]+nXDiffArray[i][j+2];

	nXOff=2;
#else
	for(i=0;i<LINE_YSTEP;i++)
		for(j=1;j<LINE_XSTEP-1;j++)
			nXDiff[j]+=nXDiffArray[i][j-1]+nXDiffArray[i][j]+nXDiffArray[i][j+1];   //3개씩합

	nXOff=1;
#endif

	for(loop=0;loop<2;loop++)
	{
		if(loop==0) nMaxI=-100;
		else        nMaxI=nMaxJ[0];

		//X 가장 높은 점 구함-------------------
		nXDiffMax=nMaxJ[loop]=0;
		for(j=nXOff;j<LINE_XSTEP-nXOff;j++)
		{
			nTmp=nMaxI-j; if(nTmp<0) nTmp*=-1;
			if(nTmp<10) continue;

			if(nXDiff[j]>nXDiffMax)
			{
				nXDiffMax=nXDiff[j];
				nMaxJ[loop]=j;
			}
		}
		if(nMaxJ[loop]==0) continue;

		if(nMaxJ[loop]==1)					nMaxJ[loop]=2;				//+1
		else if(nMaxJ[loop]==LINE_XSTEP-2)	nMaxJ[loop]=LINE_XSTEP-3;	//-1

		//25개 경우의 수를 구해서 라인 구함. (여기서 라인 과검도 구함)
		if(FindBestLine(nMaxJ[loop], 5,LINE_YSTEP, nXDiffArray, nXDiffMaxArray, &nAngle)==0)
		{
			//거기서 5개중 1개를 선택해서 더함---------------------------
			for(i=0;i<LINE_YSTEP;i++)
			{
				nXDiffMax=0;
				for(j=nMaxJ[loop]-2;j<=nMaxJ[loop]+2;j++)
					if(nXDiffArray[i][j]>nXDiffMax)
						nXDiffMax=nXDiffArray[i][j];
		
				nXDiffMaxArray[i]=nXDiffMax;
			}
			nAngle=1;
			//-------------------------------------------------------
		}

		//#20181221 막음(짧은 라인도 검출하도록)
		//128영역의 합이 600이 안되면 과검-----------------------------------------------------------------------
		//for(i=0;i<LINE_YSTEP;i+=3)
		//{
		//	if(	nXDiffMaxArray[i]+nXDiffMaxArray[i+1]+nXDiffMaxArray[i+2]<600) pOverKill[loop]=1;	
		//}
		//--------------------------------------------------------------------------------------------------------

		//제일 큰 2개 버림 (Spot성)----------------
		for(i=0;i<2;i++)
		{
			nPos=-1;
			nXDiffMax=0;
			for(j=0;j<LINE_YSTEP;j++)
			{			
				if(nXDiffMaxArray[j]>nXDiffMax)
				{
					nXDiffMax=nXDiffMaxArray[j];
					nPos=j;
				}
			}
			if(nPos>=0)
			{
				nXDiffMaxArray[nPos]=0;
			}
		}
		//----------------------------------------

		//(LINE_YSTEP-2)개의 합을 구함.-----------------------------------------------
		nMaxSum[loop]=0;
		for(i=0;i<LINE_YSTEP;i++)
		{
			if(nXDiffMaxArray[i]>0) nMaxSum[loop]+=nXDiffMaxArray[i];
		}
		nMaxSum[loop]=nMaxSum[loop]*LINE_YSTEP/(LINE_YSTEP-2);  //빠진 2개만큼 더해줌
		//----------------------------------------------------------------------------

		
		//X정확한 위치 구함--------------------------------
		nY1=top+(bottom-top)*2/5;
		nY2=top+(bottom-top)*3/5;
		nX1=(nMaxJ[loop]-1)*32; if(nX1<left)  nX1=left;
		nX2=(nMaxJ[loop]+2)*32; if(nX2>right) nX2=right;
		memset(pProfSum, 0, right*sizeof(long));
		for(j=nX1;j<nX2;j++)
		{
			for(i=nY1;i<nY2;i++)
				pProfSum[j]+=*(fm+pitch*i+j);
		}

		nXDiffMax=0;
		for(j=nX1;j<nX2;j++)
			nXDiffMax+=pProfSum[j];
		if(nX2-nX1) nDiffAvg=nXDiffMax/(nX2-nX1);
		else		nDiffAvg=80*(nY2-nY1);

		nXDiffMax=0;
		for(j=nX1;j<nX2;j++)
		{
			nTmp=pProfSum[j]-nDiffAvg;
			if(nTmp<0) nTmp*=-1;
			if(nTmp>nXDiffMax)
			{
				nXDiffMax=nTmp;
				nMaxPos[loop]=j;
				pAngle[loop]=nAngle;
			}
		}
		//--------------------------------------------------
	}
	

	for(j=left;j<right;j++) nProfSumBackup[j]/=((bottom-top)*0.75);
	

	//최종데이타 찾기 (검사영역에 따라 전체 밝기찿가 달라지므로 약간의 가감이 들어간다)
	for(i=0;i<2;i++)
	{
		if(nMaxPos[i]<0) continue;


		//주변 노이즈 LEVEL을 구한다------------------------------------------------------------------
		nDiffArraySum=nDiffArrayCnt=nDiffAvg=0;
		for(ii=0;ii<LINE_YSTEP;ii++)
			for(j=nMaxJ[i]-5;j<nMaxJ[i]+5;j++)
			{
				if(j<0 || j>=LINE_XSTEP) continue;

				if( (j>=nMaxJ[i]-1 && i<=nMaxJ[i]+1) ) continue;
				if(nXDiffArray[ii][j])
				{
					nDiffArraySum+=nXDiffArray[ii][j];
					nDiffArrayCnt++;
				}
			}
		if(nDiffArrayCnt) nDiffAvg=nDiffArraySum*LINE_YSTEP/nDiffArrayCnt;
		else              nDiffAvg=1000;
		//-------------------------------------------------------------------------------------------

		pValue[i]=nMaxSum[i]-nDiffAvg;  if(pValue[i]<0) pValue[i]=0;
		pValue[i]=pValue[i]*100/((bottom-top)*0.75);
		pX[i]    =nMaxPos[i];

		//주변밝기 보상-----------------------------------------------
		nLeft =nMaxPos[i]-64; if(nLeft<left) nLeft=left;
		nRight=nLeft+128;     
		if(nRight>right)
		{ nRight=right; nLeft=nRight-128;}
		nSum=0;
		for(j=nLeft;j<nRight;j++) nSum+=nProfSumBackup[j];
		nSum/=128;
		if(nSum<80) nSum=80; //아무리 작아도 80은 되어야 한다. 
		else if(nSum>100) nSum=100;

		if(nNoApply==0) pValue[i]=pValue[i]*80/nSum-nFoundMountain*25;
		else			pValue[i]=pValue[i]*80/nSum;
		//--------------------------------------------------------------

		//맨왼쪽 카메라 또는 맨 오른쪽 카메라의 가장자리 데이타 보정-------------------
		if(left>30)
		{
			if(pX[i]-left<500 && pValue[i]-(500-(pX[i]-left))*1.5>0)
			{
				pValue[i]=pValue[i]-(500-(pX[i]-left))*1.5;
				if(pValue[i]<0) pValue[i]=0;
			}			
			else if(pX[i]-left<500 && pValue[i]-(500-(pX[i]-left))>0)
			{
				pValue[i]=pValue[i]-(500-(pX[i]-left));
				if(pValue[i]<0) pValue[i]=0;
			}
			else if(pX[i]-left<500 && pValue[i]-(500-(pX[i]-left))*0.5>0)
			{
				pValue[i]=pValue[i]-(500-(pX[i]-left))*0.5;
				if(pValue[i]<0) pValue[i]=0;
			}
		}
	
		else if(right<pitch-30)
		{
			if(right-pX[i]<500 && pValue[i]-(500-(right-pX[i]))*1.5>0 )
			{
				pValue[i]=pValue[i]-(500-(right-pX[i]))*1.5;
				if(pValue[i]<0) pValue[i]=0;
			}
			else if(right-pX[i]<500 && pValue[i]-(500-(right-pX[i]))>0 )
			{
				pValue[i]=pValue[i]-(500-(right-pX[i]));
				if(pValue[i]<0) pValue[i]=0;
			}
			else if(right-pX[i]<500 && pValue[i]-(500-(right-pX[i]))*0.5>0 )
			{
				pValue[i]=pValue[i]-(500-(right-pX[i]))*0.5;
				if(pValue[i]<0) pValue[i]=0;
			}
		}
		//-----------------------------------------------------------------------------

	}
	
	return 0;	
}

//산이 몇개 있는지 체크 
int FindMountainCount(long *pProfile, int left, int right, int nValue)
{
	int i,j,nCount=0;
	int nMinGap=50;

	long nMin;
	long nMax;
	long nStart; 
	int nStartPos, nMountain=0, nFlag=0;
	   
	 nMin=nMax=nStart=pProfile[left];
	 nStartPos=left;
	for(j=left+1;j<right;j++)
	{
		if(pProfile[j]<nMin) nMin=pProfile[j];
		if(pProfile[j]>nMax) nMax=pProfile[j];

		if(nFlag==0)
		{
			if(pProfile[j]-nMin>nValue) 
			{
				if(j-nStartPos>nMinGap)
				{
					nMountain++;
					nStartPos=j;
					nFlag=1;
				}
			}
			else if(nMax-pProfile[j]>nValue)		
			{
				if(j-nStartPos>nMinGap)
				{
					nMountain++;
					nStartPos=j;
					nFlag=2;
				}
			}
		}
		else if(nFlag==1)
		{
			if(nMax-pProfile[j]>nValue)	
			{
				if(j-nStartPos>nMinGap)
				{
					nStartPos=j;
					nMin=pProfile[j];
					nFlag=2;
				}
			}
		}
		else if(nFlag==2)
		{
			if(pProfile[j]-nMin>nValue)
			{
				nMountain++;
				nStartPos=j;
				nFlag=1;
				nMax=pProfile[j];
			}
		}
	}
	return nMountain;
}

int FindBestLine(int nPos, int nXCnt, int nYCnt, long ArrayValue[LINE_YSTEP][LINE_XSTEP], int *nXDiffMaxArray, int *nAngle)
{
	int i, j, k, ii, jj;
	int ret=0;
	int nLeft=nPos-2;
	int nRight=nPos+2;
	long nValue, nValueSum, nMaxValueSum;
	long nMaxValue[5][5][15];
	int nX, nMaxI, nMaxJ;
	double dX, dRatio;

	nMaxValueSum=0;
	for(i=nLeft;i<=nRight;i++)
	{
		for(j=nLeft;j<=nRight;j++)
		{
			nValueSum=0;
			for(k=0;k<nYCnt;k++)
			{
				dX=i+(double)(j-i)*k/nYCnt;
				nX=(int)dX;
				dRatio=dX-nX;

				if(nX+1>=LINE_XSTEP) nValue=ArrayValue[k][nX];
				else                 nValue=ArrayValue[k][nX]*(1.0-dRatio)+ArrayValue[k][nX+1]*dRatio;

				nMaxValue[i-nLeft][j-nLeft][k]=nValue;
				nValueSum+=nValue;
			}
			if(nValueSum>nMaxValueSum)
			{
				nMaxValueSum=nValueSum;
				nMaxI=i-nLeft;
				nMaxJ=j-nLeft;
			}
		}
	}

	if(nMaxValueSum>0)
	{
		for(k=0;k<nYCnt;k++)
			nXDiffMaxArray[k]=nMaxValue[nMaxI][nMaxJ][k];

		*nAngle=90;
		if(abs(nMaxI-nMaxJ)==2)		  *nAngle=82;
		else if(abs(nMaxI-nMaxJ)==3)  *nAngle=80;
		else if(abs(nMaxI-nMaxJ)==4)  *nAngle=78;

		ret=1;
	}

	return ret;
}

int CheckOverKillNELALine(LPBYTE fm, int nPosX, int nPosY, int pitch, int nShowOffset, int nX1, int nX2, double dLimit)
{
	int ii, jj;
	int nGray[10], nGrayCnt;
	int tt, bb, ll, rr;
	long nMaxM, nMinM;

	tt=nPosY-nShowOffset/2; if(tt<0) tt=0;
	bb=tt+nShowOffset;
	if(bb>g_System.m_nImageH)
	{
		bb=g_System.m_nImageH;
		tt=bb-nShowOffset;
	}
	//X범위
	ll=nPosX-nShowOffset/2; if(ll<nX1) ll=nX1;
	rr=ll+nShowOffset;
	if(rr>nX2)
	{
		rr=nX2;
		ll=rr-nShowOffset;
		if(ll<nX1) ll=nX1;
	}

	//LINE 과검제거------------------------------------------
	for(ii=0;ii<5;ii++) nGray[ii]=0;
	for(ii=tt;ii<bb;ii++)
	{
		nGrayCnt=(ii-tt)/g_System.m_nImagePartH;
		for(jj=ll;jj<rr;jj++)
		{
			nGray[nGrayCnt]+=*(fm+pitch*ii+jj);					
		}
	}
	nMaxM=nMinM=nGray[0];
	for(ii=1;ii<5;ii++) 
	{
		if(nGray[ii]>nMaxM) nMaxM=nGray[ii];
		if(nGray[ii]<nMinM) nMinM=nGray[ii];
	}
	if(nMinM>0 && (double)(nMaxM-nMinM)/nMinM>dLimit) return 1; //과검
	else											  return 0; //라인
	//------------------------------------------------------
}


int FindLineBW_TOO_TD(int N, LPBYTE fm, int left, int top, int right, int bottom, int pitch, long *pProfile, int *pX, int *pValue)
{
	int i, j, nSmooth;
	int nTmp, nTmp1;
	long nSum, nMaxSum=0;
	int nMax, nMin, nMaxG, nMinG;
	int nLeft, nRight, nRight1;
	int nAvgArea;
	int nHeightSub=128;

	*pValue=*pX=0;
	if(right-left<150) return 0; //원래는 최소 120 [ Emphasize (25,25), Moveing Avg (35, 35) ]

	//Profile 구하기---------------------------------
	if(pProfile==NULL)
	{
		for(j=left;j<right;j++) l_nProfSumTD[N][j]=0;

		for(i=top;i<bottom;i++)
			for(j=left;j<right;j++)
				l_nProfSumTD[N][j]+=*(fm+pitch*i+j);

		for(j=left;j<right;j++)
		{
			l_nProfAvgTD[N][j]=l_nProfSumTD[N][j]/(bottom-top);
		}
	}
	else
	{
		for(j=left;j<right;j++)
			l_nProfAvgTD[N][j]=pProfile[j];
	}

	//상 하 +/- 25 CUT 해서 다시 Profile구함 -------------
	for(i=0;i<nHeightSub;i++) l_nProfSumTD[N][i]=0;

	for(i=top;i<bottom;i++)
	{
		for(j=left;j<right;j++)
		{
			if(*(fm+pitch*i+j)<l_nProfAvgTD[N][j]-25)
			{
				l_nProfSumTD[N][i-top]+=(l_nProfAvgTD[N][j]-25);
			}
			else if(*(fm+pitch*i+j)>l_nProfAvgTD[N][j]+25)
			{
				l_nProfSumTD[N][i-top]+=(l_nProfAvgTD[N][j]+25);
			}
			else
				l_nProfSumTD[N][i-top]+=*(fm+pitch*i+j);
		}
	}
	//----------------------------------------------------

	nSmooth=3;
	if(nSmooth)
	{
		memcpy(l_nProfSumBackupTD[N], l_nProfSumTD[N], nHeightSub*sizeof(long));

		nSum=0;
		for(i=top;i<top+nSmooth;i++)
				nSum+=l_nProfSumBackupTD[N][i-top];

		for(i=top;i<bottom-nSmooth;i++)
		{
			l_nProfSumTD[N][i-top+nSmooth/2]=nSum/nSmooth;
			nSum+=l_nProfSumBackupTD[N][i-top+nSmooth]-l_nProfSumBackupTD[N][i-top];
		}
	}


	for(i=top+nSmooth+10;i<bottom-nSmooth-32;i+=10)
	{
		nMax=0;
		nMin=255000;
		for(j=i;j<i+32;j++)
		{
			if(l_nProfSumTD[N][j-top]>nMax)		 nMax=l_nProfSumTD[N][j-top]; 
			else if(l_nProfSumTD[N][j-top]<nMin)  nMin=l_nProfSumTD[N][j-top];
		}
		if((nMax-nMin)>nMaxSum) nMaxSum=(nMax-nMin);
	}
	
	nAvgArea=0;
	for(j=(left+right)/2-25;j<(left+right)/2+25;j++)
		nAvgArea+=l_nProfAvgTD[N][j];
	nAvgArea=nAvgArea/50;
	if(nAvgArea<80) nAvgArea=80;


	*pValue=(nMaxSum*100/256)*80/nAvgArea;  *pX=(left+right)/2;  //100->80으로 변경 
	if(*pValue>300) *pValue-=300;

	return 0;
}

//투영 흑얼룩 검출-----------------------------------------------------------------------------------------------------
void WEB_TooBlackMura()
{
	int nWidth=g_System.m_nImageW;
	int nHeight=g_System.m_nImageH;
	int nHeightSub=g_System.m_nImagePartH;
	int pitch=g_System.m_nPitch;

	int nLevel, nNGCount=0;
	int i,j, k, ii, jj;
	int nFlagInspect=0;

	LPBYTE fm = g_fmTemp1;   //Thread.cpp의 MakeFlatImageToo()함수에서 확인


	double dTmp, dSizeMax;
	int  nTmp, nLowExist;  //512x512영역안에 이미 불량이 있으면 뺀다.
	double dThUp, dThDn;
	int nMaxX[25], nMaxY[25], nMaxValue[25];
	int nMaxXR[25], nMaxYR[25], nMaxValueR[25];
	double dMinStd[5], dMaxStd[5], dHor=0, dVer=0;
	int nJump=0, nExistNum, nTempLevel;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//2Level
	int  nMinMura=9999;
	double dOverlapPos[2];
	int nMarkingDefect[2];
	int nX1, nX2, nY1, nY2, nSum, Ni, Nj, nPosX, nPosY, nMin;
	int ll,tt,rr,bb;
	int NUM_BUFFERS=25;
//	int nLocal[g_System.m_nImageW/4 * g_System.m_nImagePartH/4], width4=g_System.m_nImageW/4, height4=g_System.m_nImagePartH/4, pitch4=g_System.m_nImageW/4;
	int nLocal[2048/4 * 128/4], width4=g_System.m_nImageW/4, height4=g_System.m_nImagePartH/4, pitch4=g_System.m_nImageW/4;

	g_Temp.m_dMuraMaxValue=0;
	//검사하기위한 데이타가 들어 있는지 확인---------------
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])
		{
			if(g_Param.m_nBMura[i]>0 && g_Param.m_nBMura[i]<nMinMura )	nMinMura=g_Param.m_dLineThresholdTD[i]; 
			
			if(nMinMura<9999) nFlagInspect=1;
		}
	}
	if(nFlagInspect==0) return;
	if(nMinMura<=10) return;
	//-----------------------------------------------------
	
	nX1=g_Temp.m_nInspectX1;
	nX2=g_Temp.m_nInspectX2;

	nMin=256*4;
	for(k=0;k<NUM_BUFFERS;k++)
	{
		nY1=nHeightSub*k;
		nY2=nHeightSub*(k+1);

		Ni=0;
		for(i=nY1;i<nY2-8;i+=8)
		{
			Nj=0;
			for(j=nX1;j<nX2-8;j+=8)
			{
				nSum=0;
				for(ii=i;ii<i+8;ii++)
					for(jj=j;jj<j+8;jj++)
						nSum+=*(fm+pitch*ii+jj);
			
				*(nLocal+pitch4*Ni+Nj)=nSum/64;
				Nj++;
			}
			Ni++;
		}

		for(i=0;i<Ni-1;i++)
			for(j=0;j<Nj-1;j++)
			{
				nTmp=*(nLocal+pitch4*i+j)+*(nLocal+pitch4*i+j+1)+*(nLocal+pitch4*(i+1)+j)+*(nLocal+pitch4*(i+1)+(j+1));
				if(nTmp<nMin)
				{
					nMin=nTmp;
					nPosX=(j+1)*8;
					nPosY=k*nHeightSub+(i+1)*8;
					if((i+1)*8-24<0)			nPosY=k*nHeightSub+24;
					else if((i+1)*8+24>nHeightSub)  nPosY=k*nHeightSub+nHeightSub-24;
				}
			}
	}
	ll=nPosX-24; if(ll<nX1) ll=nX1;
	rr=ll+48;	 if(rr>nX2) { rr=nX2;ll=rr-48;}
	tt=nPosY-24; 
	bb=tt+48;

	dMinStd[0]=GetStdevData(fm, ll,tt,rr,bb, pitch);
	if(nPosY<nHeightSub)
	{
		dMinStd[1]=GetStdevData(fm, ll,tt+nHeightSub,rr,bb+nHeightSub, pitch);
		dMinStd[2]=GetStdevData(fm, ll,tt+nHeightSub*2,rr,bb+nHeightSub*2, pitch);
	}
	else if(nPosY>nHeightSub*(NUM_BUFFERS-1))
	{
		dMinStd[1]=GetStdevData(fm, ll,tt-nHeightSub,rr,bb-nHeightSub, pitch);
		dMinStd[2]=GetStdevData(fm, ll,tt-nHeightSub*2,rr,bb-nHeightSub*2, pitch);
	}
	else
	{
		dMinStd[1]=GetStdevData(fm, ll,tt+nHeightSub,rr,bb+nHeightSub, pitch);
		dMinStd[2]=GetStdevData(fm, ll,tt-nHeightSub,rr,bb-nHeightSub, pitch);
	}
	dMinStd[0]=100*(dMinStd[0]-(dMinStd[1]+dMinStd[2])*0.5);
	g_Temp.m_dMuraMaxValue=dMinStd[0];
	//---------------------------------------------

	nLevel=GetLevel(dMinStd[0], g_Param.m_nBMura);

	if(nLevel>0 )
	{
		g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_BLACK_MURA;     
		MakeDefectRect(g_Defect.m_nDefectCount, nPosX, nPosY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
		g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].value=dMinStd[0];
		g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nPosX;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nPosY;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX=0;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY=0;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].size =0;

		CopyNGImagePart(true);			

		if(CheckDefectOverlap()==0)   							
		{
			g_Defect.m_nDefectCount++;					
		}							
	}
}


//투영 흑군집 검출-----------------------------------------------------------------------------------------------------
int FindGroupInfo(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int *pX, int *pY, int *pCount, int *pValue, double *pSize);
void TOO_FindBGroup()
{
	int nWidth=g_System.m_nImageW;
	int nHeight=g_System.m_nImageH;
	int nHeightSub=g_System.m_nImagePartH;
	int pitch=g_System.m_nPitch;

	int nGroup;
	int i,nX1, nX2;
	double dSize;
	LPBYTE fm = g_fmFlat[g_ID];

	nX1=g_Temp.m_nInspectX1;
	nX2=g_Temp.m_nInspectX2;

	int nPosX, nPosY, nCount, nValue, nLevel;

	if(g_Param.m_nGroupArea<10 || g_Param.m_nGroupCount<2 || g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) return;
	if(g_Temp.m_nInspectX2-g_Temp.m_nInspectX1<g_Param.m_nGroupArea) return;
	//-----------------------------------------------------
	
	for(i=0;i<25;i++)
	{
		nGroup=FindGroupInfo(fm, nX1, i*nHeightSub, nX2, (i+1)*nHeightSub, pitch, &nPosX, &nPosY, &nCount, &nValue, &dSize);
		if(nGroup)
		{
			nLevel=g_Param.m_nGruopLevel;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_BLACK_GROUP;     
			MakeDefectRect(g_Defect.m_nDefectCount, nPosX, nPosY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value=nValue;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nPosX;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nPosY;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX=dSize;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY=dSize;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size =dSize;

			CopyNGImagePart(true);			

			if(CheckDefectOverlap()==0)   							
			{
				g_Defect.m_nDefectCount++;					
			}		
		}
	}
}

BYTE l_GBin[2048*128];
int  l_GHisto[2048];
int FindGroupInfo(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int *pX, int *pY, int *pCount, int *pValue, double *pSize)
{
	int i, j, k;
	int nGroup=0;
	int nThreshold=g_System.m_nFlatBright-g_Param.m_nGroupAreaTh+1;
	LPBYTE fmB;
	int nRow=0;
	int nBlob, nBlobOKCount=0;
	int ll,tt,rr,bb;
	int y1, y2, nMin;
	int nCount,  nMaxCount, nMaxPos;
	int nXSum=0;
	double dSize;
	int nXCount=0;
	int nWIDTH=g_System.m_nImageW;
	int nHEIGHT=g_System.m_nImagePartH;

	g_Chain.SetChainData(1, l_GBin, 2, 2, 4, 1000, nWIDTH, nHEIGHT);

	memset(l_GBin, 0, nWIDTH*nHEIGHT);
	memset(l_GHisto, 0, nWIDTH*sizeof(int));
	for(i=top;i<bottom;i++)
	{
		fmB=l_GBin+nRow*nWIDTH;
		for(j=left;j<right;j++)
		{
			if(*(fm+pitch*i+j)<nThreshold)
				*(fmB+j)=255;
		}
		nRow++;
	}
	
	nBlob=g_Chain.FastChain(left,0,right,nHEIGHT-1);
	if(nBlob<g_Param.m_nGroupCount) return 0;

	for(k=0;k<nBlob;k++)
	{
		ll=g_Chain.FindMinX(k);
		rr=g_Chain.FindMaxX(k);
		tt=g_Chain.FindMinY(k);
		bb=g_Chain.FindMaxY(k);

		dSize=((rr-ll+1)+(bb-tt+1))*g_Param.m_dScaleFactorX/2;
		if(dSize<g_Param.m_dGroupSize) continue;  //Size 만족못함

		y1=top+tt;
		y2=top+bb;
		nMin=255;
		for(i=y1;i<=y2;i++)
			for(j=ll;j<=rr;j++)
			{
				if(*(fm+pitch*i+j)<nMin)
					nMin=*(fm+pitch*i+j);
			}
		if(g_System.m_nFlatBright-nMin<g_Param.m_nGroupValue) continue;//Value만족못함

		nBlobOKCount++;
		l_GHisto[(ll+rr)/2]++;
	}
	if(nBlobOKCount<g_Param.m_nGroupCount) return 0;
	

	nCount=0;
	for(j=left;j<left+g_Param.m_nGroupArea;j++)
	{
		nCount+=l_GHisto[j];
	}

	//가장 많은 영역찾기------------------------------------------------
	nMaxCount=nCount;
	nMaxPos=left+g_Param.m_nGroupArea/2;
	for(j=left+1;j<right-g_Param.m_nGroupArea;j++)
	{
		nCount=nCount+l_GHisto[j-1+g_Param.m_nGroupArea]-l_GHisto[j-1];
		if(nCount>nMaxCount)
		{
			nMaxCount=nCount;
			nMaxPos=j+g_Param.m_nGroupArea/2;
		}
	}
	//------------------------------------------------------------------

	if(nMaxCount>=g_Param.m_nGroupCount)
	{
		nGroup=1;
		*pY=(top+bottom)/2;

		nXCount=nXSum=0;
		for(j=nMaxPos-g_Param.m_nGroupArea/2;j<nMaxPos+g_Param.m_nGroupArea/2;j++)
		{
			if(l_GHisto[j])
			{
				nXSum+=(j*l_GHisto[j]);
				nXCount+=l_GHisto[j];
			}
		}
		if(nXCount) *pX=nMaxPos=nXSum/nXCount;
		else        *pX=nMaxPos;

		nMin=255;
		for(i=top;i<bottom;i++)
			for(j=nMaxPos-g_Param.m_nGroupArea/2;j<nMaxPos+g_Param.m_nGroupArea/2;j++)
			{
				if(j<left || j>=right) continue;

				if(*(fm+pitch*i+j)<nMin)
					nMin=*(fm+pitch*i+j);
			}

		*pValue=g_System.m_nFlatBright-nMin;
		*pSize=nMaxCount;
	}

	return nGroup;
}