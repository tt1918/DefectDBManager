#include "Stdafx.h"

#include <memory.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include <dvec.h>
#include <math.h>

#include "KoWebDoc.h"
#include "KoWebView.h"

double l_dTime[10];


//찍힘 검사시 Size가 1Pixel 차이날수 있는데 이는 Binary할때 4개 평균값을 구하기 때문. 이렇게 하지 않으면 1Pixel 노이즈가 많아 Binary후 Dilation할때 붙어서 Size가 커지는 과검이 발생하기 때문
//같은조건이면 찍힘-백-흑 으로 분류한다.
void WEB_SpotInspect()
{
	int i, j, m, nOverlap;
	int nLevel,nLevel1,nTempLevel, nValueUp, nValueDn, nValueUp1, nValueDn1, nValueUpBig, nValueDnBig;
	double dSize[5], dSize1[5], dSizeBig[5];
	int nPLevel,nPValueUp, nPValueDn;	//찍힘검사
	int nClass, nClass_, nNGType;
	double dPSize[5], dImulKipoSize, dImulKipoSize1, dImulKipoSizeBig;
	int nWhiteInspect=0, nBlackInspect=0, nCunicInspect=0;
	int nSkip, leftBig, topBig, nDefectPosX, nDefectPosY, nDefectBigPosX, nDefectBigPosY;
	double dCunic;
	int nCunicValue;
	int nFoundLami=0;
	int nOverkill=0, nBigDefect;

	g_Temp.m_nMaxGray=0;
	g_Temp.m_nMinGray=255;

	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])
		{
			if(g_Param.m_nValueUp[i]>0  || g_Param.m_dSizeUp[i])	  nWhiteInspect=1;
			if(g_Param.m_nValueDn[i]>0  || g_Param.m_dSizeDn[i])	  nBlackInspect=1;
			if(g_Param.m_dCunicStd[i]>0 || g_Param.m_dCunicStd1[i]>0) nCunicInspect=1;
		}
	}
	if(g_Param.m_nValueOneUp>0 || g_Param.m_dSizeOneUp>0.001) nWhiteInspect=1;
	if(g_Param.m_nValueOneDn>0 || g_Param.m_dSizeOneDn>0.001) nBlackInspect=1;
	if(g_Param.m_nPressInspect) nWhiteInspect=nBlackInspect=1;

	g_Temp.m_nInspectWhite=nWhiteInspect;
	g_Temp.m_nInspectBlack=nBlackInspect;
	if(g_Temp.m_nAutoAdjustStart>0) nCunicInspect=0; //밝기 조정 중에는 쿠닉 검사하지 않는다.


	//평활화 영상을 압축--------------------------------------------------------------------
	if(g_Temp.m_nMakePyramidDone==0)
	{
		ThreadMakePyramid(g_fmFlat[g_ID]);  //Thread로 변경
		g_Temp.m_nMakePyramidDone=1;
	}

	l_dTime[4]=Clock_set();
	
	//NxN구간으로 나눠서 후보 찾기-------------------------------------------------------
	int nCol, nRow;
	nCol=g_System.m_nImageW/g_System.m_nCandiRect; if(g_System.m_nImageW-nCol*g_System.m_nCandiRect) nCol++;
	nRow=g_System.m_nImageH/g_System.m_nCandiRect; if(g_System.m_nImageH-nRow*g_System.m_nCandiRect) nRow++;
	FindCandiFromPyramid(0, g_System.m_nImageW, g_System.m_nCandiRect);

	l_dTime[5]=Clock_set();

	//범위외 버림

//압축영상보기--------------------------------------------------------------------------------
//(1/4) 보기.. 다양한 압축 볼 수 있음
//	for(i=0;i<l_nPyramidHeight[2];i++)
//		for(j=0;j<l_nPyramidWidth[2];j++)
//			*(g_fmFlat[g_ID]+g_System.m_nImageW*i+j)=*(l_fmPyramid[2]+l_nPyramidWidth[2]*i+j);
//--------------------------------------------------------------------------------------------

	//큰 순서로 소팅 각 항목별 MAX_DEFECT (30개만,  흑10  백10  찍힘10)-----
	FindCandiSorting(0, g_System.m_nImageW,  g_System.m_nCandiRect);
	
//	if(nCunicInspect)
//		COSS_Mean(g_fmFlat[g_ID], g_fmTemp, 0, 0, g_System.m_nImageW, g_System.m_nImageH, g_System.m_nImageW, 3,3);


	//(1/2)영상에서 MAX/Min 10개 표시---------------------------------------------------
	//백후보든 흑후보든 찍힘도 같이 검사한다.
	for(i=0;i<g_System.m_nMaxCandiDefect;i++)
	{
//		if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) break;  //마킹우선사용

		if(g_CandiW.m_nCandiValue[i]-g_CandiW.m_nCandiAvg[i]>g_Temp.m_nMaxGray) g_Temp.m_nMaxGray=g_CandiW.m_nCandiValue[i]-g_CandiW.m_nCandiAvg[i];
		if(g_CandiB.m_nCandiValue[i]-g_CandiB.m_nCandiAvg[i]<g_Temp.m_nMinGray) g_Temp.m_nMinGray=g_CandiB.m_nCandiValue[i]-g_CandiB.m_nCandiAvg[i];


		//백점
		if(nWhiteInspect && g_CandiW.m_nCandiValue[i]-g_CandiW.m_nCandiAvg[i]>=10) 
		{			
			nSkip=0;
			//경계투과(반사) 면 이미지가 울렁거려 단순울렁인건지 Defect인지 한번더 판단한다.
			if(g_System.m_nBDOverkill && CheckBoundaryOpticOverKill(g_fmFlat[g_ID], g_CandiW.m_nCandiX[i], g_CandiW.m_nCandiY[i], g_System.m_nImageW, g_System.m_nImageH, 13)) nSkip=1;					
			if(nSkip==0 && CheckSimpleGradient( g_CandiW.m_nCandiX[i], g_CandiW.m_nCandiY[i],  0, g_System.m_nImageW, g_System.m_nImageH, 3.5))								   nSkip=1; //단순 Gradient변화이면 Skip
			
			if(nSkip==0) //+++++++++++++++++++++++++++++++++++++++++++++++++++
			{
				dSize1[0]=dSize1[1]=dSize1[2]=dImulKipoSize1=nValueUp1=nValueDn1=0;
								

				MakeDefectRect(g_Defect.m_nDefectCount, g_CandiW.m_nCandiX[i], g_CandiW.m_nCandiY[i], g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
				GetSizeNValue(0,g_fmFlat[g_ID], g_CandiW.m_nCandiY[i], g_Defect.m_rcDefect[g_Defect.m_nDefectCount].left, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].top, BAD_IMG_WIDTH,BAD_IMG_WIDTH, g_System.m_nImageW, 0,g_Param.m_nAreaThUp,g_Param.m_nAreaThDn, g_Param.m_nAttatchPixel, g_Param.m_nSizeMethod,g_Param.m_dScaleFactorX, g_Param.m_dScaleFactorY, &nValueUp, &nValueDn, dSize, &dImulKipoSize, &nDefectPosX, &nDefectPosY);

				//4배 압축영상에서 Size구함-----------------------------------------------------------	
				nBigDefect=0;
				if(g_System.m_nBigDefect && (dSize[0]>=BAD_IMG_WIDTH*g_Param.m_dScaleFactorX/2 || dSize[1]>=BAD_IMG_WIDTH*g_Param.m_dScaleFactorY/2))
				{
					MakeDefectRect_BigDefect(g_CandiW.m_nCandiX[i], g_CandiW.m_nCandiY[i], g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, &leftBig, &topBig);
					GetSizeNValue(0,l_fmPyramid[2], g_CandiW.m_nCandiY[i]/4, leftBig, topBig, BAD_IMG_WIDTH, BAD_IMG_WIDTH, g_System.m_nImageW/4, 0,g_Param.m_nAreaThUp,g_Param.m_nAreaThDn, 0, g_Param.m_nSizeMethod,g_Param.m_dScaleFactorX*4, g_Param.m_dScaleFactorY*4, &nValueUpBig, &nValueDnBig, dSizeBig, &dImulKipoSizeBig, &nDefectBigPosX, &nDefectBigPosY);

					if(dSizeBig[2]>dSize[2])
					{
						nBigDefect=1;
						dSize[0]=dSizeBig[0];
						dSize[1]=dSizeBig[1];
						dSize[2]=dSizeBig[2];
					}
				}
				//-----------------------------------------------------------------------------------
				else
				{
					if(nBlackInspect)
						GetSizeNValue(1,g_fmFlat[g_ID], g_CandiW.m_nCandiY[i], g_Defect.m_rcDefect[g_Defect.m_nDefectCount].left, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].top, BAD_IMG_WIDTH,BAD_IMG_WIDTH, g_System.m_nImageW, 0,g_Param.m_nAreaThUp,g_Param.m_nAreaThDn, g_Param.m_nAttatchPixel, g_Param.m_nSizeMethod,g_Param.m_dScaleFactorX, g_Param.m_dScaleFactorY, &nValueUp1, &nValueDn1, dSize1, &dImulKipoSize1, &nDefectPosX, &nDefectPosY);
				}				

				if(nValueUp>g_Temp.m_nMaxGray) g_Temp.m_nMaxGray=nValueUp;

				nLevel=GetLevel(nValueUp, dSize[2], g_Param.m_nValueUp, g_Param.m_dSizeUp);

				//Param 1,2 적용----------------------------------------------------------------
				if((g_Param.m_nValueOneUp>0 && nValueUp>= g_Param.m_nValueOneUp) || 
				   (g_Param.m_dSizeOneUp>0.001 && dSize[2]>=g_Param.m_dSizeOneUp)   ) nLevel=1;

			
/* 밀집관련: 당분간 사용안함 (128x128 영역에 군집불량인지 체크하는 부분)
				if(g_Param.m_nMILStartLevel>0 && nLevel!=10 && nLevel>=g_Param.m_nMILStartLevel)
				{
					if(CheckMiseKoonJip(g_fmFlat[g_ID],g_Defect.m_rcDefect[g_Defect.m_nDefectCount].left,g_Defect.m_rcDefect[g_Defect.m_nDefectCount].top,
						g_Defect.m_rcDefect[g_Defect.m_nDefectCount].right,g_Defect.m_rcDefect[g_Defect.m_nDefectCount].bottom, g_System.m_nImageW, g_Param.m_nMILTh, g_Param.m_dMILSize, g_Param.m_dScaleFactorX, g_Param.m_dScaleFactorY, g_Param.m_nMILCount))
					{
						nLevel=g_Param.m_nMILChangeLevel;
					}
				}
*/

				//라미체크(미분투과) : 백점검출뒤 라미불량인지 체크하는 부분.>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
				if(g_Param.m_nLamiTh>0 && nValueUp>g_Param.m_nLamiTh && g_Temp.m_nInspectX2-g_Temp.m_nInspectX1>=BAD_IMG_WIDTH)
				{
					int nLamiValue, nLamiPress, nSizeX, nSizeY;
					int nX1, nX2, nY1, nY2, nLamiLevel;
					double dLamiSize[5], dCompactness;
					GetSizeNValue(0,g_fmFlat[g_ID], g_CandiW.m_nCandiY[i], g_Defect.m_rcDefect[g_Defect.m_nDefectCount].left, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].top, BAD_IMG_WIDTH,BAD_IMG_WIDTH, g_System.m_nImageW, 0, g_Param.m_nLamiTh, 255, 1, 0,g_Param.m_dScaleFactorX, g_Param.m_dScaleFactorY, &nValueUp, &nValueDn, dLamiSize, &dImulKipoSize, &nDefectPosX, &nDefectPosY);
					if(dLamiSize[2]<=g_Param.m_dLamiMaxSize)
					{
						nX1=g_CandiW.m_nCandiX[i]-BAD_IMG_WIDTH/2; if(nX1<0) nX1=0; if(nX1+BAD_IMG_WIDTH>=g_Temp.m_nInspectX2) nX1=g_Temp.m_nInspectX2-BAD_IMG_WIDTH;
						nX2=nX1+BAD_IMG_WIDTH; 
						nY1=g_CandiW.m_nCandiY[i]-BAD_IMG_HEIGHT/2; if(nY1<0) nY1=0;  if((nY1+BAD_IMG_HEIGHT)>=g_System.m_nImageH) nY1=g_System.m_nImageH-BAD_IMG_HEIGHT;
						nY2=nY1+BAD_IMG_HEIGHT;	
						nLamiPress=nLamiPress=FindLamiPress(g_fmFlat[g_ID], nX1, nY1, nX2, nY2,  g_System.m_nPitch, 10, &dCompactness, &nSizeX, &nSizeY);
						if(nLamiPress)
						{
							nLamiLevel=GetLevel(dLamiSize[2], g_Param.m_dLamiSize);
							if(nLamiLevel>0)
							{
								nLevel=nLamiLevel;
								dSize[2]=dLamiSize[2];
								nFoundLami=1;
							}
						}
					}
				}
				//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#ifdef USE_HOLE_SKIP				
				if(g_Temp.m_nMakePyramidDone)
				{
					if(g_Temp.m_nHoleCheck && dSize[2]>0 && HoleCheck(l_fmPyramid[1], g_CandiW.m_nCandiX[i]/2, g_CandiW.m_nCandiY[i]/2, g_System.m_nImageW/2, g_System.m_nImageH/2, g_System.m_nFlatBright, g_Temp.m_nHoleThres, g_Param.m_dScaleFactorX*4, g_Param.m_dScaleFactorY*4, g_Temp.m_nGrabFrameID))
						continue;
				}
				else
				{
					if(g_Temp.m_nHoleCheck && dSize[2]>0 && HoleCheck(g_fmFlat[g_ID],g_CandiW.m_nCandiX[i], g_CandiW.m_nCandiY[i], g_System.m_nImageW, g_System.m_nImageH, g_System.m_nFlatBright, g_Temp.m_nHoleThres,  g_Param.m_dScaleFactorX*2, g_Param.m_dScaleFactorY*2, g_Temp.m_nGrabFrameID))
						continue;
				}
#endif


				//nLevel=0;
				nClass=nClass_=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_SPOT_WHITE;  //백점
				if(nFoundLami)
				{
					nClass=nClass_=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_LAMI_PRESS;  //Lami Press				}
				}

				if(nFoundLami==0) //라미불량아닐때만...(흑배이 같이 있는 경우
				{
					//흑 Level을 구한다.
					nLevel1=GetLevel(nValueDn1, dSize1[2], g_Param.m_nValueDn, g_Param.m_dSizeDn);
					if(nLevel1>0)
					{
						nClass_=SelectDefect(nLevel, NG_SPOT_WHITE, nLevel1, NG_SPOT_BLACK); //마킹상태같고, Level값으면 여기서는 백을 선택한다. 
						if(nClass!=nClass_) //흑점으로 변경한다.
						{
							nClass=nClass_;
							nLevel=nLevel1;
							dSize[0]=dSize1[0];
							dSize[1]=dSize1[1];
							dSize[2]=dSize1[2];
						}
					}
				}

				//찍힘검사 설정되어 있으면 찍힘검사한다.----------
				if(g_Param.m_nPressInspect) 
				{
					GetSizeNValue(2,g_fmFlat[g_ID], g_CandiW.m_nCandiY[i], g_Defect.m_rcDefect[g_Defect.m_nDefectCount].left, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].top, BAD_IMG_WIDTH,BAD_IMG_WIDTH, g_System.m_nImageW, 0,g_Param.m_nAreaThUp,g_Param.m_nAreaThDn, g_Param.m_nAttatchPixel, g_Param.m_nSizeMethod,g_Param.m_dScaleFactorX, g_Param.m_dScaleFactorY, &nPValueUp, &nPValueDn, dPSize, &dImulKipoSize, &nDefectPosX, &nDefectPosY);
					if(nPValueUp>=g_Param.m_nPressMinUp && nPValueDn>=g_Param.m_nPressMinDn)
					{
						nPLevel=GetLevel(nPValueUp+nPValueDn, dPSize[2], g_Param.m_nPressValue, g_Param.m_nPressSize);
					}
					else
					{
						nPLevel=0;
					}					

					if(nPLevel>0)
					{	
						nClass_=SelectDefect(nPLevel, NG_SPOT_PRESS, nLevel, nClass%CLASSDEV); //백점과 찍힘중에 선택한다. 
						if(nClass_%CLASSDEV==NG_SPOT_PRESS)
						{
							nValueUp=nPValueUp;
							nValueDn=nPValueDn;
						}
					}
				}
				//------------------------------------------------


			



				if(nClass>MILLION || nClass_>MILLION)
				{					
					g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
					g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=g_CandiW.m_nCandiX[i];			//불량위치 X (Pixel)
					g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=g_CandiW.m_nCandiY[i];			//불량위치 Y (Pixel)

					if(nClass==nClass_) //(백점 또는 흑점)이라는 뜻
					{
						if(g_System.m_nBigDefect && nBigDefect)
						{
							g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nDefectBigPosX*4;
							g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nDefectBigPosY*4;
							MakeDefectRect_BigDefect1(g_Defect.m_nDefectCount, nDefectBigPosX*4, nDefectBigPosY*4, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
							MakeDefectRect_BigDefect(nDefectBigPosX*4, nDefectBigPosY*4, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, &leftBig, &topBig);
							CopyNGImageFMArea(l_fmPyramid[2], leftBig, topBig, leftBig+BAD_IMG_WIDTH, topBig+BAD_IMG_WIDTH, g_System.m_nImageW/4, 255);
						}
						else
							CopyNGImage(true);

#if defined NANJING_OLED || defined USE_DAKANO_LINE
						//백점중에서 Scratch Check 해서 스크라치면 우선한다.
						if(nClass%CLASSDEV==NG_SPOT_WHITE)
						{
							if(dSize[0]>0 && dSize[1]/dSize[0]>4)
							{
								nLevel=GetLevel(dSize[1], g_Param.m_dScratchLength);
								if(nLevel>0)
								{
									nClass=SelectDefect( nLevel, NG_SCRATCH, (nClass/MILLION), NG_SPOT_WHITE);
								}
							}
						}
#endif						

						g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;				//불량TYPE
						g_Defect.m_Defect[g_Defect.m_nDefectCount].size		   =dSize[2];			//Size	(넥스트아이 서버와 맞추기 위해)

#if defined NANJING_OLED || defined USE_DAKANO_LINE
						if(nClass%CLASSDEV==NG_SPOT_WHITE || nClass%CLASSDEV==NG_SCRATCH)
#else
						if(nClass%CLASSDEV==NG_SPOT_WHITE)
#endif
							g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nValueUp;		//Value	(넥스트아이 서버와 맞추기 위해)		
						else 
							g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nValueDn1;		//Value	(넥스트아이 서버와 맞추기 위해)	

						g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX	   =dSize[0];			//Size X
						g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY	   =dSize[1];			//Size Y
					}
					else				//찍힘이라는 뜻
					{
						CopyNGImage(true);
						g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass_;			//불량TYPE
						g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =dPSize[2];			//Size
						g_Defect.m_Defect[g_Defect.m_nDefectCount].value       =nValueUp+nValueDn;	//Value					
						g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =dPSize[0];			//Size X
						g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =dPSize[1];			//Size Y
					}

					//본딩시트 과검체크 
					if(g_Param.m_nRemoveBondingOverkill>0)
					{
						nOverkill=CheckBondingSheet(g_Defect.m_pImage[g_Defect.m_nDefectCount], 0,0,BAD_IMG_WIDTH-1, BAD_IMG_WIDTH-1, BAD_IMG_WIDTH, g_Param.m_nRemoveBondingOverkill);
					}
					if(nOverkill==0)
					{
						//겹치는 불량 점검-----------------------------------------------------------------------------------------------
						nOverlap=CheckDefectOverlap();
						if(nOverlap==0 && g_Defect.m_nDefectCount<g_System.m_nMaxDefect) //안겹치고 불량갯수도 최대치보다 작으면 추가함.
						{
							g_Defect.m_nDefectCount++;
						}
						else
						{
							if(nOverlap==3)		//불량수가 최대갯수 이상이면
							{
								if(g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class%CLASSDEV==NG_SPOT_PRESS)  CheckPriorLevel(1);
								else																				 CheckPriorLevel(0); 			
							}
						}
						//---------------------------------------------------------------------------------------------------------------						
					}
//					if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) break;
				}
			}//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		}

		//흑점
		if(nBlackInspect && g_CandiB.m_nCandiAvg[i]-g_CandiB.m_nCandiValue[i]>=10)
		{
			nSkip=0;
#ifdef MARKING_REMOVE
		if(g_CandiB.m_nCandiX[i]<64 || g_CandiB.m_nCandiX[i]>g_System.m_nImageW-64) nSkip=1;
#endif
			
			//경계투과(반사) 면 이미지가 울렁거려 단순울렁인건지 Defect인지 한번더 판단한다.
			if(nSkip==0 && g_System.m_nBDOverkill && CheckBoundaryOpticOverKill(g_fmFlat[g_ID], g_CandiB.m_nCandiX[i], g_CandiB.m_nCandiY[i], g_System.m_nImageW, g_System.m_nImageH, 13)) nSkip=1;
			if(nSkip==0 && CheckSimpleGradient( g_CandiB.m_nCandiX[i], g_CandiB.m_nCandiY[i],  0, g_System.m_nImageW, g_System.m_nImageH, 3.5))											   nSkip=1;
			
			if(nSkip==0) //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			{
				dSize1[0]=dSize1[1]=dSize1[2]=dImulKipoSize1=nValueUp1=nValueDn1=0;
				
				MakeDefectRect(g_Defect.m_nDefectCount, g_CandiB.m_nCandiX[i], g_CandiB.m_nCandiY[i], g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
				GetSizeNValue(1,g_fmFlat[g_ID], g_CandiB.m_nCandiY[i], g_Defect.m_rcDefect[g_Defect.m_nDefectCount].left, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].top, BAD_IMG_WIDTH,BAD_IMG_WIDTH, g_System.m_nImageW, 0,g_Param.m_nAreaThUp,g_Param.m_nAreaThDn, g_Param.m_nAttatchPixel, g_Param.m_nSizeMethod,g_Param.m_dScaleFactorX, g_Param.m_dScaleFactorY, &nValueUp, &nValueDn, dSize, &dImulKipoSize, &nDefectPosX, &nDefectPosY);
				
				//4배 압축영상에서 Size구함-----------------------------------------------------------	
				nBigDefect=0;
				if(g_System.m_nBigDefect && (dSize[0]>=BAD_IMG_WIDTH*g_Param.m_dScaleFactorX/2 || dSize[1]>=BAD_IMG_WIDTH*g_Param.m_dScaleFactorY/2))
				{
					MakeDefectRect_BigDefect(g_CandiB.m_nCandiX[i], g_CandiB.m_nCandiY[i], g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, &leftBig, &topBig);
					GetSizeNValue(1,l_fmPyramid[2], g_CandiW.m_nCandiY[i]/4, leftBig, topBig, BAD_IMG_WIDTH, BAD_IMG_WIDTH, g_System.m_nImageW/4, 0,g_Param.m_nAreaThUp,g_Param.m_nAreaThDn, 0, g_Param.m_nSizeMethod,g_Param.m_dScaleFactorX*4, g_Param.m_dScaleFactorY*4, &nValueUpBig, &nValueDnBig, dSizeBig, &dImulKipoSizeBig, &nDefectBigPosX, &nDefectBigPosY);
			
					if(dSizeBig[2]>dSize[2])
					{
						nBigDefect=1;
						dSize[0]=dSizeBig[0];
						dSize[1]=dSizeBig[1];
						dSize[2]=dSizeBig[2];
					}
				}
				//-----------------------------------------------------------------------------------
				else
				{
					if(nWhiteInspect) GetSizeNValue(0,g_fmFlat[g_ID], g_CandiB.m_nCandiY[i], g_Defect.m_rcDefect[g_Defect.m_nDefectCount].left, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].top, BAD_IMG_WIDTH,BAD_IMG_WIDTH, g_System.m_nImageW, 0,g_Param.m_nAreaThUp,g_Param.m_nAreaThDn, g_Param.m_nAttatchPixel, g_Param.m_nSizeMethod,g_Param.m_dScaleFactorX, g_Param.m_dScaleFactorY, &nValueUp1, &nValueDn1, dSize1, &dImulKipoSize1, &nDefectPosX, &nDefectPosY);
				}

				if(-nValueDn<g_Temp.m_nMinGray) g_Temp.m_nMinGray=-nValueDn;

				nLevel=GetLevel(nValueDn, dSize[2], g_Param.m_nValueDn, g_Param.m_dSizeDn);

				if((g_Param.m_nValueOneDn>0 && nValueDn>= g_Param.m_nValueOneDn) || 
				   (g_Param.m_dSizeOneDn>0.001 && dSize[2]>=g_Param.m_dSizeOneDn)   )nLevel=1;


				if(nCunicInspect)   nClass=nClass_=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_SPOT_BLACK;	//쿠닉검사할때 흑점도 검사하면 불량번호가 일반적인것과 다름. 3으로 끝남.
				else                nClass=nClass_=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_SPOT_BLACK;	//흑점 (2로 끝남)

				//이물기포 Size를 구했으면
				if(dImulKipoSize>0.01)
				{
					nPLevel=GetLevel(nValueDn, dImulKipoSize, g_Param.m_nKipoValue, g_Param.m_dKipoSize);
					if(nPLevel>0)
					{
						nClass_=SelectDefect(nPLevel, NG_SPOT_KIPO, nLevel, NG_SPOT_BLACK);  //이물기포와 흑점중에 선택한다.(같은 조건이면 앞에 있는 것, 이물기포)
						if(nClass!=nClass_)
						{
							dPSize[0]=dPSize[1]=dPSize[2]=dImulKipoSize;
						}
					}
				}
				else //이물기포와 찍힘은 같이 사용하지 않는다.
				{				
					//백 Level을 구한다.: -----------------------------------------------------------------------------------
					nLevel1=GetLevel(nValueUp1, dSize1[2], g_Param.m_nValueUp, g_Param.m_dSizeUp);
					if(nLevel1>0)
					{
						nClass_=SelectDefect(nLevel1, NG_SPOT_WHITE, nLevel, NG_SPOT_BLACK );  //마킹상태같고, Level값으면 여기서는 백을 선택한다.
						if(nClass!=nClass_) //백점으로 변경한다.
						{
							nClass=nClass_;
							nLevel=nLevel1;
							dSize[0]=dSize1[0];
							dSize[1]=dSize1[1];
							dSize[2]=dSize1[2];
						}
					}
					//--------------------------------------------------------------------------------------------------------
				
					//찍힘검사 설정되어 있으면 찍힘검사한다.----------
					if(g_Param.m_nPressInspect)
					{
						GetSizeNValue(2,g_fmFlat[g_ID], g_CandiW.m_nCandiY[i], g_Defect.m_rcDefect[g_Defect.m_nDefectCount].left, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].top, BAD_IMG_WIDTH,BAD_IMG_WIDTH, g_System.m_nImageW, 0,g_Param.m_nAreaThUp,g_Param.m_nAreaThDn, g_Param.m_nAttatchPixel, g_Param.m_nSizeMethod,g_Param.m_dScaleFactorX, g_Param.m_dScaleFactorY, &nPValueUp, &nPValueDn, dPSize, &dImulKipoSize, &nDefectPosX, &nDefectPosY);
						if(nPValueUp>=g_Param.m_nPressMinUp && nPValueDn>=g_Param.m_nPressMinDn)
						{
							nPLevel=GetLevel(nPValueUp+nPValueDn, dPSize[2], g_Param.m_nPressValue, g_Param.m_nPressSize);
						}
						else
						{
							nPLevel=0;
						}

						if(nPLevel)
						{
							nClass_=SelectDefect(nPLevel, NG_SPOT_PRESS, nLevel, nClass%CLASSDEV);  //찍힘과 흑점중에 선택한다.(같은 조건이면 앞에 있는 것,찍힘)
							if(nClass_%CLASSDEV==NG_SPOT_PRESS)
							{
								nValueUp=nPValueUp;
								nValueDn=nPValueDn;
							}
						}		
					}
					//------------------------------------------------
				}

				

				if(nClass>MILLION || nClass_>MILLION)
				{
					
					g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
					g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=g_CandiB.m_nCandiX[i];			//불량위치 X (Pixel)
					g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=g_CandiB.m_nCandiY[i];			//불량위치 Y (Pixel)
					if(nClass==nClass_) //흑점이라는 뜻
					{
						if(g_System.m_nBigDefect && nBigDefect)
						{
							g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nDefectBigPosX*4;
							g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nDefectBigPosY*4;
							MakeDefectRect_BigDefect1(g_Defect.m_nDefectCount, nDefectBigPosX*4, nDefectBigPosY*4, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
							MakeDefectRect_BigDefect(nDefectBigPosX*4, nDefectBigPosY*4, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, &leftBig, &topBig);
							CopyNGImageFMArea(l_fmPyramid[2], leftBig, topBig, leftBig+BAD_IMG_WIDTH, topBig+BAD_IMG_WIDTH, g_System.m_nImageW/4, 255);
						}
						else
							CopyNGImage(true);

						g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;				//불량TYPE
						g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =dSize[2];			//Size
						
						if(nClass%CLASSDEV==NG_SPOT_BLACK)
							g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nValueDn;		//Value	(넥스트아이 서버와 맞추기 위해)		
						else 
							g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nValueUp1;		//Value	(넥스트아이 서버와 맞추기 위해)	

						g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =dSize[0];			//Size X
						g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =dSize[1];			//Size Y
					}
					else				//이물기포 또는 찍힘 이라는 뜻
					{
						CopyNGImage(true);
						g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass_;			//불량TYPE
						g_Defect.m_Defect[g_Defect.m_nDefectCount].size       =dPSize[2];			//Size
						g_Defect.m_Defect[g_Defect.m_nDefectCount].value      =nValueUp+nValueDn;	//Value					
						g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX      =dPSize[0];			//Size X
						g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY      =dPSize[1];			//Size Y
					}

					//본딩시트 과검체크 
					if(g_Param.m_nRemoveBondingOverkill>0)
					{
						nOverkill=CheckBondingSheet(g_Defect.m_pImage[g_Defect.m_nDefectCount], 0,0,BAD_IMG_WIDTH-1, BAD_IMG_WIDTH-1, BAD_IMG_WIDTH, g_Param.m_nRemoveBondingOverkill);
					}
					if(nOverkill==0)
					{
						//겹치는 불량 점검-----------------------------------------------------------------------------------------------
						nOverlap=CheckDefectOverlap();
						if(nOverlap==0 && g_Defect.m_nDefectCount<g_System.m_nMaxDefect) //안겹치고 불량갯수도 최대치보다 작으면 추가함.
						{
							g_Defect.m_nDefectCount++;
						}
						else
						{
							if(nOverlap==3)		//불량수가 최대갯수 이상이면
							{
								if(g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class%CLASSDEV==NG_SPOT_PRESS)  CheckPriorLevel(1);
								else																				 CheckPriorLevel(0); 			
							}
						}
						//---------------------------------------------------------------------------------------------------------------						
					}					
	//				if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) break;
				}	
			}//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		}


#ifdef FIND_TINYDEFECT		
		//흑 Tiny
		if(nBlackInspect && g_CandiTinyB.m_nCandiValue[i]>=g_Param.m_nAreaThDn)
		{
			nSkip=0;
			
			if(nSkip==0) //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			{
				MakeDefectRect(g_Defect.m_nDefectCount, g_CandiTinyB.m_nCandiX[i], g_CandiTinyB.m_nCandiY[i], g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
				GetSizeNValueTinyB(g_fmFlat[g_ID], g_Defect.m_rcDefect[g_Defect.m_nDefectCount].left+BAD_IMG_WIDTH/4, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].top+BAD_IMG_WIDTH/4, BAD_IMG_WIDTH/2,BAD_IMG_WIDTH/2, g_System.m_nImageW, 0,g_Param.m_nAreaThDn, g_Param.m_nAttatchPixel, g_Param.m_nSizeMethod,g_Param.m_dScaleFactorX, g_Param.m_dScaleFactorY,  &nValueDn, dSize);
				g_Temp.m_nMinGray=-nValueDn;

				nLevel=GetLevel(nValueDn, dSize[2], g_Param.m_nUseLevel, g_Param.m_nValueDn, g_Param.m_dSizeDn);

				nClass=nClass_=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_SPOT_BLACK;	//흑점 (2로 끝남)				
				
				if(nClass>MILLION || nClass_>MILLION)
				{
					CopyNGImage(true);
					g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
					g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=g_CandiTinyB.m_nCandiX[i];			//불량위치 X (Pixel)
					g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=g_CandiTinyB.m_nCandiY[i];			//불량위치 Y (Pixel)
					
					g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;				//불량TYPE
					g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =dSize[2];			//Size
					g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nValueDn;			//Value	
					g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =dSize[0];			//Size X
					g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =dSize[1];			//Size 
					
					if(CheckDefectOverlap()==0 && g_Defect.m_nDefectCount<g_System.m_nMaxDefect)   							
						g_Defect.m_nDefectCount++;

					if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) break;
				}	
			}//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		}
#endif

		//AREA(찍힘) --------------------------------------------------------------------------------------------------------------------------------
		if(g_Param.m_nPressInspect && g_CandiArea.m_nCandiValue[i]>=3)
		{
			nSkip=0;
			//경계투과(반사) 면 이미지가 울렁거려 단순울렁인건지 Defect인지 한번더 판단한다.
			if(g_System.m_nBDOverkill && CheckBoundaryOpticOverKill(g_fmFlat[g_ID], g_CandiArea.m_nCandiX[i], g_CandiArea.m_nCandiY[i], g_System.m_nImageW, g_System.m_nImageH, 13)) nSkip=1;				
			if(nSkip==0 && CheckSimpleGradient( g_CandiArea.m_nCandiX[i], g_CandiArea.m_nCandiY[i],  0, g_System.m_nImageW, g_System.m_nImageH, 3.5))								 nSkip=1;			
			
			if(nSkip==0) //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			{
				MakeDefectRect(g_Defect.m_nDefectCount, g_CandiArea.m_nCandiX[i], g_CandiArea.m_nCandiY[i], g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
				GetSizeNValue(2,g_fmFlat[g_ID], g_CandiW.m_nCandiY[i], g_Defect.m_rcDefect[g_Defect.m_nDefectCount].left, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].top,  BAD_IMG_WIDTH,BAD_IMG_HEIGHT, g_System.m_nImageW, 0,g_Param.m_nAreaThUp,g_Param.m_nAreaThDn,g_Param.m_nAttatchPixel, g_Param.m_nSizeMethod,g_Param.m_dScaleFactorX, g_Param.m_dScaleFactorY, &nValueUp, &nValueDn, dSize, &dImulKipoSize, &nDefectPosX, &nDefectPosY);			
				if(nValueUp>=g_Param.m_nPressMinUp && nValueDn>=g_Param.m_nPressMinDn)
				{
					nLevel=GetLevel(nValueUp+nValueDn, dSize[2], g_Param.m_nPressValue, g_Param.m_nPressSize);
					nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+3;  //찍힘			

					if(nClass>MILLION)
					{
						CopyNGImage(true);
						g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
						g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=g_CandiArea.m_nCandiX[i];	 //불량위치 X (Pixel)
						g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=g_CandiArea.m_nCandiY[i];	 //불량위치 Y (Pixel)
						g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;				 //불량TYPE
						g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =dSize[2];			 //Size
						g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nValueUp+nValueDn;	 //Value	
						g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =dSize[0];			 //Size X
						g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =dSize[1];			 //Size Y

						//겹치는 불량 점검-----------------------------------------------------------------------------------------------
						nOverlap=CheckDefectOverlap();
						if(nOverlap==0 && g_Defect.m_nDefectCount<g_System.m_nMaxDefect) //안겹치고 불량갯수도 최대치보다 작으면 추가함.
						{
							g_Defect.m_nDefectCount++;
						}
						else
						{
							if(nOverlap==3) CheckPriorLevel(1); //겹치는것 없고, 불량갯수가 이미 최대치에 도달했으면				
						}
						//---------------------------------------------------------------------------------------------------------------		
//						if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) break;
					}
				}
			}//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		}//----------------------------------------------------------------------------------------------------------------------------------------------	

		//쿠닉검사
		if(nCunicInspect && g_CandiArea.m_nCandiValue[i]>=2)
		{
			nSkip=0;
			if(CheckSimpleGradient( g_CandiArea.m_nCandiX[i], g_CandiArea.m_nCandiY[i],  0, g_System.m_nImageW, g_System.m_nImageH, 3.5)) nSkip=1;

#ifdef MY_RECV
			MakeDefectRect(g_Defect.m_nDefectCount, g_CandiArea.m_nCandiX[i], g_CandiArea.m_nCandiY[i], g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			nSkip=CheckPressOverkill(g_fmFlat[g_ID], g_Defect.m_rcDefect[g_Defect.m_nDefectCount].left, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].top,  BAD_IMG_WIDTH,BAD_IMG_HEIGHT, g_System.m_nImageW, 3, g_System.m_nFlatBright, 3); 
#endif
			if(nSkip==0) //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			{
				MakeDefectRect(g_Defect.m_nDefectCount, g_CandiArea.m_nCandiX[i], g_CandiArea.m_nCandiY[i], g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
				if(g_System.m_nLineCAM==1) //Line 스캔
					GetCunicValue_SameFrame(g_fmFlat[g_ID], g_CandiArea.m_nCandiX[i], g_CandiArea.m_nCandiY[i], g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, g_System.m_nPitch, g_System.m_nImageH, g_Param.m_nCunicInspArea, &dCunic, &nCunicValue);			
				else					   //Area 스캔
					GetCunicValue_OtherFrame(g_fmFlat[g_ID], g_CandiArea.m_nCandiX[i], g_CandiArea.m_nCandiY[i], g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, g_System.m_nPitch, g_System.m_nImageH, g_Param.m_nCunicInspArea, &dCunic, &nCunicValue);				
				
				nLevel =GetLevel(dCunic, g_Param.m_dCunicStd);
				nLevel1=GetLevel(nCunicValue, dCunic, g_Param.m_nCunicValue1, g_Param.m_dCunicStd1);

				if(dCunic*1000>g_Temp.m_nCunicValue) g_Temp.m_nCunicValue=dCunic*1000;
				if(nLevel>0 && nLevel1>0)
				{
					if(nLevel1<=nLevel)
					{
						nClass=nLevel1*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_CUNIC;  //쿠닉
						nValueUp=100*(int)(dCunic*1000)+nCunicValue;
					}
					else
					{
						nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_CUNIC;  //쿠닉
						nValueUp=dCunic*1000;
					}
				}
				else if(nLevel>0)
				{
					nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_CUNIC;  //쿠닉
					nValueUp=dCunic*1000;
				}
				else if(nLevel1>0)
				{
					nClass=nLevel1*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_CUNIC;  //쿠닉
					nValueUp=100*(int)(dCunic*1000)+nCunicValue;
				}
				else
				{
					nClass=0;
				}
								

				if(nClass>MILLION)
				{
					CopyNGImage(true);
					g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
					g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=g_CandiArea.m_nCandiX[i];	 //불량위치 X (Pixel)
					g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=g_CandiArea.m_nCandiY[i];	 //불량위치 Y (Pixel)
					g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;				 //불량TYPE
					g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0;			 //Size
					g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nValueUp;	 //Value	
					g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0;			 //Size X
					g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0;			 //Size Y

//겹치는 불량 점검-----------------------------------------------------------------------------------------------
					nOverlap=CheckDefectOverlap();
					if(nOverlap==0 && g_Defect.m_nDefectCount<g_System.m_nMaxDefect) //안겹치고 불량갯수도 최대치보다 작으면 추가함.
					{
						g_Defect.m_nDefectCount++;
					}
					else
					{
						if(nOverlap==3) CheckPriorLevel(0); //겹치는것 없고, 불량갯수가 이미 최대치에 도달했으면				
					}
					//---------------------------------------------------------------------------------------------------------------		

//					if(CheckDefectOverlap()==0 && g_Defect.m_nDefectCount<g_System.m_nMaxDefect)   							
//						g_Defect.m_nDefectCount++;

//					if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) break;
				}
			}//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		}
	}

#ifdef REMOVE_BUMP_OVERKILL
	if(g_Defect.m_nDefectCount) RemoveBumpOverkill(g_fmFlat[g_ID], g_System.m_nPitch, g_System.m_nImageH);
#endif

	if(g_Defect.m_nDefectCount &&  g_Param.m_nDotMarkRemove ) RemoveMarking(g_fmFlat[g_ID], g_System.m_nPitch, g_System.m_nImageH, g_Param.m_nDotMarkTh);

#ifdef MY_RECV
	if(g_Temp.m_nPCOptic==RECV_OPTIC && g_Defect.m_nDefectCount && g_Kipo.m_nCurRectCount>0) 
		CheckKipoPress();
#endif

}


#ifdef USE_SK_BAT
void WEB_WingInspect()
{
	int i, j, loop, nTmp;
	int nLevel;
	int nX1, nX2, nPitchW;
	double dSize[3];
	int nClass, nAvg;
	double dPSize[3], dArea, dMaxArea;
	int nBlackInspect=0, nTh, nPos, nMinValue, nBlobValue;
	int pitch=g_System.m_nPitch;
	long nSum=0;
	double dBlobSize[100];
	int ll,tt,rr,bb;
	
	int nOverkill=0, nBigDefect, nBlob;
	LPBYTE fm=g_fmGrab[g_ID];
	LPBYTE fmBinary=g_fmBinaryWing;

	g_Temp.m_nSideAvg=0;
	g_Temp.m_nSideBlackMin=0;

	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])
		{			
			if(g_Param.m_nValueDnWing[i]>0  || g_Param.m_dSizeDnWing[i])	  nBlackInspect=1;
		}
	}
	if(nBlackInspect==0) return;

	if(g_Param.m_nEdgeType>0) 
	{
		nX1=g_Temp.m_dOuterEdge+g_Param.m_dOffset1;
		nX2=g_Temp.m_dInnerEdge-g_Param.m_dOffset2;
	}
	else if(g_Param.m_nEdgeType<0)
	{
		nX1=g_Temp.m_dInnerEdge+g_Param.m_dOffset2;
		nX2=g_Temp.m_dOuterEdge-g_Param.m_dOffset1;
	}
	else return;

	nPitchW=nX2-nX1;
	if(nPitchW<2) return;

	//영역평균구하기--------------------------------------------------------------------------
	for(i=0;i<g_System.m_nImageH;i+=4)
		for(j=nX1;j<nX2;j++)
			nSum+=*(fm+pitch*i+j);
			
	
	nAvg=nSum/(nPitchW*g_System.m_nImageH/4);
	nTh=nAvg-g_Param.m_nAreaThDnWing;
	g_Temp.m_nSideAvg=nAvg;

	//Binary----------------------------------------------------------------------------------
	for(i=0;i<g_System.m_nImageH;i++)
	{
		for(j=nX1;j<nX2;j++)
		{
			nTmp=*(fm+pitch*i+j);
			if(nTmp<nTh) *(fmBinary+nPitchW*i+j-nX1)=255;
			else         *(fmBinary+nPitchW*i+j-nX1)=0;
		}
	}

	//Blob------------------------------------------------------------------------------------
	g_Chain.SetChainData(1, fmBinary, 3, 3, 4, 20000, nPitchW, g_System.m_nImageH);
	
	nBlob=g_Chain.FastChain(1, 1 ,nPitchW-1, g_System.m_nImageH-1);	
	if(nBlob<1) return;
	if(nBlob>100) nBlob=100;

	for(i=0;i<nBlob;i++)
	{
		dArea=g_Chain.Chain_LogicalArea(i);
		dBlobSize[i]=dArea;
	}

	for(loop=0;loop<g_System.m_nMaxDefect;loop++)
	{
		//제일 큰 순서대로 불량검출
		nPos=-1;
		dMaxArea=1;
		for(i=0;i<nBlob;i++)
		{
			if(dBlobSize[i]>dMaxArea)
			{
				dMaxArea=dBlobSize[i];
				nPos=i;
			}
		}
		if(nPos<0) 
			break;
		dBlobSize[nPos]=0;

		//Value구하기----------------------------
		ll	=g_Chain.FindMinX(nPos)+nX1;
		tt	=g_Chain.FindMinY(nPos);
		rr	=g_Chain.FindMaxX(nPos)+nX1;
		bb	=g_Chain.FindMaxY(nPos);

		nMinValue=255;
		for(i=tt+1;i<bb+1;i++)
			for(j=ll+1;j<rr+1;j++)
			{
				nTmp=(*(fm+pitch*(i-1)+j-1)+*(fm+pitch*(i-1)+j)+*(fm+pitch*(i-1)+j+1)+
					  *(fm+pitch*(i  )+j-1)+*(fm+pitch*(i  )+j)+*(fm+pitch*(i  )+j+1)+
					  *(fm+pitch*(i+1)+j-1)+*(fm+pitch*(i+1)+j)+*(fm+pitch*(i+1)+j+1))/9;
				if(nTmp<nMinValue)
				{
					nMinValue=nTmp;
				}
			}
		//--------------------------------------		

		dSize[0]=(rr-ll+1)*g_Param.m_dScaleFactorX;
		dSize[1]=(bb-tt+1)*g_Param.m_dScaleFactorY;

		dSize[2]=(dSize[0]+dSize[1])/2;
		nBlobValue=nAvg-nMinValue;

		if(nBlobValue>g_Temp.m_nSideBlackMin) g_Temp.m_nSideBlackMin=nBlobValue;

		nLevel=GetLevel(nBlobValue, dSize[2], g_Param.m_nUseLevel, g_Param.m_nValueDnWing, g_Param.m_dSizeDnWing);

		if(nLevel>0 && g_Defect.m_nDefectCount<g_System.m_nMaxDefect)
		{
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=(ll+rr)/2;					//불량위치 X (Pixel)
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=(tt+bb)/2;					//불량위치 Y (Pixel)
			MakeDefectRect(g_Defect.m_nDefectCount, (ll+rr)/2, (tt+bb)/2, 0, g_System.m_nImageW);
			CopyNGImage(false);
			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_WING_BLOCK;	//불량TYPE
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =dSize[2];			//Size
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value       =nBlobValue;	//Value					
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =dSize[0];			//Size X
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =dSize[1];			//Size Y


			g_Defect.m_nDefectCount++;
			if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) break;
		}
	}
}
#endif

void RemoveBumpOverkill(LPBYTE fm, int pitch, int nGrabHeight)
{
	int i, j, k, m, N;
	int nRemoveCnt=0;
	BYTE nInValid[100];

	memset(nInValid, 0, g_Defect.m_nDefectCount);

	for(i=0;i<g_Defect.m_nDefectCount;i++)
	{
		nInValid[i]=CheckBoundaryOpticOverKill(fm,  g_Defect.m_Defect[i].x_pos, g_Defect.m_Defect[i].y_pos, pitch, nGrabHeight, 13);
		if(nInValid[i]) 
			nRemoveCnt++;
	}


	if(g_Defect.m_nDefectCount==nRemoveCnt) 
	{
		g_Defect.m_nDefectCount=0;
		return;
	}

	for(i=0;i<nRemoveCnt;i++)
	{
		for(j=g_Defect.m_nDefectCount-1;j>=0;j--)
			if(nInValid[j])
			{
				break;
			}
		
		for(k=j;k<g_Defect.m_nDefectCount-1;k++)
		{
			N=k+1;

			g_Defect.m_Defect[k].x_pos			=g_Defect.m_Defect[N].x_pos;
			g_Defect.m_Defect[k].y_pos			=g_Defect.m_Defect[N].y_pos;
			g_Defect.m_Defect[k].defect_class	=g_Defect.m_Defect[N].defect_class;
			g_Defect.m_Defect[k].size			=g_Defect.m_Defect[N].size; 
			g_Defect.m_Defect[k].value			=g_Defect.m_Defect[N].value; 
			g_Defect.m_Defect[k].sizeX			=g_Defect.m_Defect[N].sizeX; 
			g_Defect.m_Defect[k].sizeY			=g_Defect.m_Defect[N].sizeY; 
			for(m=0;m<2;m++)
				g_Defect.m_Defect[k].dTemp[m]=g_Defect.m_Defect[N].dTemp[m];

			//파일이름
			memcpy(g_Defect.m_Defect[k].filename, g_Defect.m_Defect[N].filename, MAX_BADIMAGE_FILENAME);

			//사각형--------------------------------------------------------------
			g_Defect.m_rcDefect[k].left		=g_Defect.m_rcDefect[N].left;
			g_Defect.m_rcDefect[k].top		=g_Defect.m_rcDefect[N].top;
			g_Defect.m_rcDefect[k].right	=g_Defect.m_rcDefect[N].right;
			g_Defect.m_rcDefect[k].bottom	=g_Defect.m_rcDefect[N].bottom;

			//Image--------------------------------------------------------------
			memcpy(g_Defect.m_pImage[k], g_Defect.m_pImage[N], BAD_IMG_WIDTH*BAD_IMG_HEIGHT);

			nInValid[k]=nInValid[N];
		}

		g_Defect.m_nDefectCount--;
	}

}

void RemoveMarking(LPBYTE fm, int pitch, int nGrabHeight, int nThreshold)
{
		int i, j, k, m, N;
	int nRemoveCnt=0;
	BYTE nInValid[100];

	memset(nInValid, 0, g_Defect.m_nDefectCount);

	//불량에 대해서 DOT 마크인지 확인
	for(i=0;i<g_Defect.m_nDefectCount;i++)
	{
		if(g_Defect.m_Defect[i].defect_class%10!=2) continue;

		if(g_Param.m_dDotMarkSize>0.1)  //Size조건이 있으면 확인
		{
			if(g_Defect.m_Defect[i].size>g_Param.m_dDotMarkSize) nInValid[i]=1;
		}

		if(nInValid[i]==0) 
			nInValid[i]=CheckDotMarking(fm,  g_Defect.m_Defect[i].x_pos, g_Defect.m_Defect[i].y_pos,  g_Temp.m_nInspectX1,  g_Temp.m_nInspectX2, pitch, nGrabHeight, nThreshold, g_Param.m_dScaleFactorX);
		
		if(nInValid[i]==0) 
			nInValid[i]=CheckDotMarking1(fm,  g_Defect.m_Defect[i].x_pos, g_Defect.m_Defect[i].y_pos,  g_Temp.m_nInspectX1,  g_Temp.m_nInspectX2, pitch, nGrabHeight, nThreshold, g_Param.m_dScaleFactorX);

		if(nInValid[i])
			nRemoveCnt++;

		if(g_Param.m_nDotLevelChange>0)
			g_Defect.m_Defect[i].defect_class=MILLION*g_Param.m_nDotLevelChange+g_Defect.m_Defect[i].defect_class%MILLION;
	}
	if(g_Param.m_nDotLevelChange) return;   //없애는게 아니고 Level만 변경하는 거면  여기서 Return


	if(g_Defect.m_nDefectCount==nRemoveCnt) 
	{
		g_Defect.m_nDefectCount=0;
		return;
	}

	for(i=0;i<nRemoveCnt;i++)
	{
		for(j=g_Defect.m_nDefectCount-1;j>=0;j--)
			if(nInValid[j])
			{
				break;
			}
		
		for(k=j;k<g_Defect.m_nDefectCount-1;k++)
		{
			N=k+1;

			g_Defect.m_Defect[k].x_pos			=g_Defect.m_Defect[N].x_pos;
			g_Defect.m_Defect[k].y_pos			=g_Defect.m_Defect[N].y_pos;
			g_Defect.m_Defect[k].defect_class	=g_Defect.m_Defect[N].defect_class;
			g_Defect.m_Defect[k].size			=g_Defect.m_Defect[N].size; 
			g_Defect.m_Defect[k].value			=g_Defect.m_Defect[N].value; 
			g_Defect.m_Defect[k].sizeX			=g_Defect.m_Defect[N].sizeX; 
			g_Defect.m_Defect[k].sizeY			=g_Defect.m_Defect[N].sizeY; 
			for(m=0;m<2;m++)
				g_Defect.m_Defect[k].dTemp[m]=g_Defect.m_Defect[N].dTemp[m];

			//파일이름
			memcpy(g_Defect.m_Defect[k].filename, g_Defect.m_Defect[N].filename, MAX_BADIMAGE_FILENAME);

			//사각형--------------------------------------------------------------
			g_Defect.m_rcDefect[k].left		=g_Defect.m_rcDefect[N].left;
			g_Defect.m_rcDefect[k].top		=g_Defect.m_rcDefect[N].top;
			g_Defect.m_rcDefect[k].right	=g_Defect.m_rcDefect[N].right;
			g_Defect.m_rcDefect[k].bottom	=g_Defect.m_rcDefect[N].bottom;

			//Image--------------------------------------------------------------
			memcpy(g_Defect.m_pImage[k], g_Defect.m_pImage[N], BAD_IMG_WIDTH*BAD_IMG_HEIGHT);

			nInValid[k]=nInValid[N];
		}

		g_Defect.m_nDefectCount--;
	}
}

void CheckKipoPress()
{
#ifdef MY_RECV
	int i, m, nX, nY;
	int nType, nTempLevel;
	int nClass, nLevel, nPeriod;
		
	for(i=0;i<g_Defect.m_nDefectCount;i++)
	{
		nType=g_Defect.m_Defect[i].defect_class%CLASSDEV;

//검출한 불량이 마킹하는 불량이면 SKIP
		nLevel=g_Defect.m_Defect[i].defect_class/MILLION;
//		if(nType-1>=0 && nLevel-1>=0 && g_Mark.nData[nType-1][nLevel-1]) continue;

		if(nType==RECV_CHECK_NG_TYPE1 || nType==RECV_CHECK_NG_TYPE2)
		{
			nTempLevel=(int)(g_Defect.m_Defect[i].defect_class/MILLION); if(nTempLevel==0) nTempLevel=10;	//0이면 10Level
			nClass=(g_Defect.m_Defect[i].defect_class/CLASSDEV)%CLASSDEV;
			nPeriod=(g_Defect.m_Defect[i].defect_class/OPTICDEV)%PERIODDEV;
			nX=g_Defect.m_Defect[i].x_pos;
			nY=g_Defect.m_Defect[i].y_pos;

			//SEND에서 검출한 영역중에 백점이 있으면 기포눌림으로 변경한다. g_Kipo.m_nKipoPress가 1이면 기포눌림으로 표시된것은 모두 1 Level로 변경, 1이 아니면 원래 Level로 둠.
			if( nTempLevel>0 && g_Kipo.m_nKipoPress!=0 && g_Kipo.m_nCurRectCount>0)
			{
				for(m=0;m<g_Kipo.m_nCurRectCount;m++) //영역이 일치하면 기포눌림.
				{
					if(nX>=g_Kipo.m_nCurRect[m].left && nX<=g_Kipo.m_nCurRect[m].right && nY>=g_Kipo.m_nCurRect[m].top && nY<=g_Kipo.m_nCurRect[m].bottom)
					{
						if(g_Kipo.m_nKipoPress>0 && g_Kipo.m_nKipoPress<=10)  nLevel=g_Kipo.m_nKipoPress;
						else												  nLevel=nTempLevel;           //g_Kipo.m_nKipoPress=-1 이면 원래 Level로
	
						g_Defect.m_Defect[i].defect_class	=nLevel*MILLION+nPeriod*OPTICDEV+nClass*CLASSDEV+RECV_CHANGE_NG_TYPE;
						break;
					}
				}
			}
		}	
	}	
#endif
}

//펜마킹 검출 루틴 추가
//바깥으로 뺄수 있는 항목 nOffset, nDiff
void WEB_PenMark()
{
	int i, j, ii,jj,m;
	int nTmp;
	int pitch=g_System.m_nPitch;
	int pitchH=g_System.m_nPitch/4;
	int widthH=g_System.m_nImageW/4;
	int heightH=g_System.m_nImageH/4;
	int nOffset=6;
	int nDiff=10;
	int nBlob;
	int ll,tt,rr,bb;
	double dArea, dMaxArea;
	int nMaxPos;
	double Cx, Cy;
	int nLimitX1=g_Temp.m_nInspectX1/4;
	int nLimitX2=g_Temp.m_nInspectX2/4;

	LPBYTE fm=g_fmGrab[g_ID];

	for(i=0;i<g_System.m_nImageH-4;i+=4)
		for(j=0;j<g_System.m_nImageW-4;j+=4)
		{
			nTmp=0;
			for(ii=i;ii<i+4;ii++)
				for(jj=j;jj<j+4;jj++)
					nTmp+=*(fm+pitch*ii+jj);
			*(g_fmTemp+pitchH*i/4+j/4)=nTmp/16;
		}

	memset(g_fmTemp1, 0, widthH*heightH);
	for(i=nOffset;i<heightH-nOffset;i++)
		for(j=nLimitX1+nOffset;j<nLimitX2-nOffset;j++)
		{
			nTmp= *(g_fmTemp+pitchH*i+j)+nDiff;
			if(  (*(g_fmTemp+pitchH*i+j-nOffset) > nTmp  &&  *(g_fmTemp+pitchH*i+j+nOffset) > nTmp) ||
				 (*(g_fmTemp+pitchH*(i-nOffset)+j) > nTmp  &&  *(g_fmTemp+pitchH*(i+nOffset)+j) > nTmp) ||
				 (*(g_fmTemp+pitchH*(i-nOffset)+j-nOffset) > nTmp  &&  *(g_fmTemp+pitchH*(i+nOffset)+j+nOffset) > nTmp) ||
				 (*(g_fmTemp+pitchH*(i+nOffset)+j-nOffset) > nTmp  &&  *(g_fmTemp+pitchH*(i-nOffset)+j+nOffset) > nTmp) )
				*(g_fmTemp1+pitchH*i+j)=255;
		}

	g_Chain.SetChainData(1, g_fmTemp1, 2, 2, 50, 20000, widthH, heightH);
	for(i=0;i<4;i++)
	{
		nBlob=g_Chain.FastChain(nLimitX1+nOffset, i*heightH/4+nOffset ,nLimitX2-nOffset-1, (i+1)*heightH/4+nOffset-1);	
		dMaxArea=0;
		nMaxPos=-1;
		for(j=0;j<nBlob;j++)
		{
			dArea=g_Chain.Chain_LogicalArea(j);
			if(dArea>dMaxArea)
			{
				dMaxArea=dArea;
				nMaxPos=j;
			}
		}
		if(nMaxPos>=0)
		{
			g_Chain.Chain_Center(nMaxPos, &Cx, &Cy);

			MakeDefectRect(g_Defect.m_nDefectCount, (int)Cx*4, (int)Cy*4, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			
			ll=Cx-BAD_IMG_WIDTH/2; if(ll<nLimitX1) ll=nLimitX1;
			rr=ll+BAD_IMG_WIDTH;   if(rr>=nLimitX2) rr=nLimitX2;
			tt=Cy-BAD_IMG_HEIGHT/2;if(tt<0)        tt=0;
			bb=tt+BAD_IMG_HEIGHT;  if(bb>=heightH) {bb=heightH-1; tt=bb-BAD_IMG_HEIGHT;}
			CopyNGImageFMArea(g_fmTemp, ll,tt,rr,bb, pitchH, -1);


			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=(int)Cx*4;	 //불량위치 X (Pixel)
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=(int)Cy*4;	 //불량위치 Y (Pixel)
			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+4;	//펜마킹
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =10;			 //Size
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =0;			 //Value	
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =10;			 //Size X
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =10;			 //Size Y

			if(CheckDefectOverlap()==0)   							
				g_Defect.m_nDefectCount++;
		}
	}
}

void MakeDefectRect(int nDefect, int nX, int nY, int nLeft, int nRight)
{
	int ll,rr,tt,bb;

	if(nDefect<0 || nDefect>=g_System.m_nMaxDefect) return;

	ll=nX-BAD_IMG_WIDTH/2;
	rr=ll+BAD_IMG_WIDTH;

	tt=nY-BAD_IMG_HEIGHT/2;
	bb=tt+BAD_IMG_HEIGHT;

	if(ll<nLeft)
	{
		ll=nLeft; rr=ll+BAD_IMG_WIDTH;
	}
	if(rr>nRight)
	{
		rr=nRight; ll=rr-BAD_IMG_WIDTH;
	}
	if(ll<0)
	{
		ll=0; rr=ll+BAD_IMG_WIDTH;
	}

	if(tt<0)
	{
		tt=0; bb=tt+BAD_IMG_HEIGHT;
	}
	if(bb>=g_System.m_nImageH)
	{
		bb=g_System.m_nImageH-1;
		tt=bb-BAD_IMG_HEIGHT;
	}

	
	g_Defect.m_rcDefect[nDefect].left	=ll;
	g_Defect.m_rcDefect[nDefect].top	=tt;
	g_Defect.m_rcDefect[nDefect].right	=rr;
	g_Defect.m_rcDefect[nDefect].bottom	=bb;	
}
void MakeDefectRect_BigDefect(int nX, int nY, int nLeft, int nRight, int *left, int *top)
{
	int ll,rr,tt,bb;


	ll=nX-4*BAD_IMG_WIDTH/2;
	rr=ll+4*BAD_IMG_WIDTH;

	tt=nY-4*BAD_IMG_HEIGHT/2;
	bb=tt+4*BAD_IMG_HEIGHT;

	if(ll<nLeft)
	{
		ll=nLeft; rr=ll+4*BAD_IMG_WIDTH;
	}
	if(rr>nRight)
	{
		rr=nRight; ll=rr-4*BAD_IMG_WIDTH;
	}
	if(ll<0)
	{
		ll=0; rr=ll+4*BAD_IMG_WIDTH;
	}

	if(tt<0)
	{
		tt=0; bb=tt+4*BAD_IMG_HEIGHT;
	}
	if(bb>=g_System.m_nImageH)
	{
		bb=g_System.m_nImageH-1;
		tt=bb-4*BAD_IMG_HEIGHT;
	}
	
	*left	=ll/4;
	*top	=tt/4;
}
void MakeDefectRect_BigDefect1(int nDefect, int nX, int nY, int nLeft, int nRight)
{
	int ll,rr,tt,bb;

	if(nDefect<0 || nDefect>=g_System.m_nMaxDefect) return;

	ll=nX-4*BAD_IMG_WIDTH/2;
	rr=ll+4*BAD_IMG_WIDTH;

	tt=nY-4*BAD_IMG_HEIGHT/2;
	bb=tt+4*BAD_IMG_HEIGHT;

	if(ll<nLeft)
	{
		ll=nLeft; rr=ll+4*BAD_IMG_WIDTH;
	}
	if(rr>nRight)
	{
		rr=nRight; ll=rr-4*BAD_IMG_WIDTH;
	}
	if(ll<0)
	{
		ll=0; rr=ll+4*BAD_IMG_WIDTH;
	}

	if(tt<0)
	{
		tt=0; bb=tt+4*BAD_IMG_HEIGHT;
	}
	if(bb>=g_System.m_nImageH)
	{
		bb=g_System.m_nImageH-1;
		tt=bb-4*BAD_IMG_HEIGHT;
	}

	
	g_Defect.m_rcDefect[nDefect].left	=ll;
	g_Defect.m_rcDefect[nDefect].top	=tt;
	g_Defect.m_rcDefect[nDefect].right	=rr;
	g_Defect.m_rcDefect[nDefect].bottom	=bb;	
}


void MakeDefectTempRect(int nX, int nY, int nLeft, int nRight)
{
	int ll,rr,tt,bb;

	ll=nX-BAD_IMG_WIDTH/2;
	rr=ll+BAD_IMG_WIDTH;

	tt=nY-BAD_IMG_HEIGHT/2;
	bb=tt+BAD_IMG_HEIGHT;

	if(ll<nLeft)
	{
		ll=nLeft; rr=ll+BAD_IMG_WIDTH;
	}
	if(rr>nRight)
	{
		rr=nRight; ll=rr-BAD_IMG_WIDTH;
	}
	if(ll<0)
	{
		ll=0; rr=ll+BAD_IMG_WIDTH;
	}

	if(tt<0)
	{
		tt=0; bb=tt+BAD_IMG_HEIGHT;
	}
	if(bb>=g_System.m_nImageH)
	{
		bb=g_System.m_nImageH-1;
		tt=bb-BAD_IMG_HEIGHT;
	}

	
	g_DefectTemp.m_rcDefect.left	=ll;
	g_DefectTemp.m_rcDefect.top		=tt;
	g_DefectTemp.m_rcDefect.right	=rr;
	g_DefectTemp.m_rcDefect.bottom	=bb;	
}

//bFlat=true : 평활화 영상
//     =false: 원본영상
int CopyNGImage(bool bFlat)
{
	int i, j, nTmp;
	int nOffX=g_Defect.m_rcDefect[g_Defect.m_nDefectCount].left;
	int nOffY=g_Defect.m_rcDefect[g_Defect.m_nDefectCount].top;
	int pitch=g_System.m_nPitch;
	LPBYTE fm;
	LPBYTE fmD=g_Defect.m_pImage[g_Defect.m_nDefectCount];

	if(bFlat)
	{
		fm=g_fmFlat[g_ID];
	}
	else 
	{
		fm=g_fmGrab[g_ID];
		g_Temp.m_nUsePrevImage=0;
	}

	if(g_System.m_nLineCAM==0 && g_System.m_nImagePart>1)
	{
		nTmp=(nOffY+BAD_IMG_HEIGHT/2)/g_System.m_nImagePartH;
		if(nOffY<nTmp*g_System.m_nImagePartH) { nOffY=nTmp*g_System.m_nImagePartH;}
		else if(nOffY+BAD_IMG_HEIGHT>=(nTmp+1)*g_System.m_nImagePartH){ nOffY=(nTmp+1)*g_System.m_nImagePartH-BAD_IMG_HEIGHT;}
	}


	if(g_System.m_nDefectApplyLUT)
	{
		if(g_Temp.m_nUsePrevImage)
		{
			for(i=0;i<32;i++)
				for(j=0;j<BAD_IMG_WIDTH;j++)
				{
					*(fmD+BAD_IMG_WIDTH*i+j)=g_DefectLUT[*(g_fmPrev+pitch*i+j+nOffX)];
				}
			for(i=32;i<BAD_IMG_HEIGHT;i++)
				for(j=0;j<BAD_IMG_WIDTH;j++)
				{
					*(fmD+BAD_IMG_WIDTH*i+j)=g_DefectLUT[*(fm+pitch*(i+nOffY-32)+j+nOffX)];
				}
		}
		else
		{
			for(i=0;i<BAD_IMG_HEIGHT;i++)
				for(j=0;j<BAD_IMG_WIDTH;j++)
				{
					*(fmD+BAD_IMG_WIDTH*i+j)=g_DefectLUT[*(fm+pitch*(i+nOffY)+j+nOffX)];
				}
		}
	}
	else
	{
		if(g_Temp.m_nUsePrevImage)
		{
			for(i=0;i<32;i++)
				for(j=0;j<BAD_IMG_WIDTH;j++)
				{
					*(fmD+BAD_IMG_WIDTH*i+j)=*(g_fmPrev+pitch*i+j+nOffX);
				}
			for(i=32;i<BAD_IMG_HEIGHT;i++)
				for(j=0;j<BAD_IMG_WIDTH;j++)
				{
					*(fmD+BAD_IMG_WIDTH*i+j)=*(fm+pitch*(i+nOffY-32)+j+nOffX);
				}
		}
		else
		{
			for(i=0;i<BAD_IMG_HEIGHT;i++)
				for(j=0;j<BAD_IMG_WIDTH;j++)
				{
					*(fmD+BAD_IMG_WIDTH*i+j)=*(fm+pitch*(i+nOffY)+j+nOffX);
				}
		}
	}

	return 0;
}
int CopyNGImageTemp(LPBYTE fm, LPBYTE fmD, int left, int top, int right, int bottom, int pitch)
{
	int i, j;
	int nOffX, nOffY;

	if(g_System.m_nDefectApplyLUT)
	{
		for(i=0;i<BAD_IMG_HEIGHT;i++)
		{
			nOffY=top+(bottom-top)*i/BAD_IMG_HEIGHT;
			for(j=0;j<BAD_IMG_WIDTH;j++)
			{
				nOffX=left+(right-left)*j/BAD_IMG_WIDTH;
				*(fmD+BAD_IMG_WIDTH*i+j)=g_DefectLUT[*(fm+pitch*nOffY+nOffX)];
			}
		}
	}
	else
	{
		for(i=0;i<BAD_IMG_HEIGHT;i++)
		{
			nOffY=top+(bottom-top)*i/BAD_IMG_HEIGHT;
			for(j=0;j<BAD_IMG_WIDTH;j++)
			{
				nOffX=left+(right-left)*j/BAD_IMG_WIDTH;
				*(fmD+BAD_IMG_WIDTH*i+j)=*(fm+pitch*nOffY+nOffX);
			}
		}
	}

	return 0;
}
int CopyNGImageFM(LPBYTE fm, int pitch)
{
	int i, j;
	int nOffX=g_Defect.m_rcDefect[g_Defect.m_nDefectCount].left;
	int nOffY=g_Defect.m_rcDefect[g_Defect.m_nDefectCount].top;
	int nOrgPitch=g_System.m_nPitch;
	LPBYTE fmD=g_Defect.m_pImage[g_Defect.m_nDefectCount];

	if(pitch/nOrgPitch<1)
	{
		nOffX=nOffX*pitch/nOrgPitch-32; if(nOffX<0) nOffX=0;
		nOffY=nOffY*pitch/nOrgPitch-32; if(nOffY<0) nOffY=0;
	}

	if(g_System.m_nDefectApplyLUT)
	{
		for(i=0;i<BAD_IMG_HEIGHT;i++)
			for(j=0;j<BAD_IMG_WIDTH;j++)
			{
				*(fmD+BAD_IMG_WIDTH*i+j)=g_DefectLUT[*(fm+pitch*(i+nOffY)+j+nOffX)];
			}
	}
	else
	{
		for(i=0;i<BAD_IMG_HEIGHT;i++)
			for(j=0;j<BAD_IMG_WIDTH;j++)
			{
				*(fmD+BAD_IMG_WIDTH*i+j)=*(fm+pitch*(i+nOffY)+j+nOffX);
			}
	}

	return 0;
}

//(0,0)에 Gray로 표시
int CopyNGImageFMArea(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nGray)
{
	int i, j;
	int nOffX, nOffY;
	LPBYTE fmD=g_Defect.m_pImage[g_Defect.m_nDefectCount];

	if(g_System.m_nDefectApplyLUT)
	{
		for(i=0;i<BAD_IMG_HEIGHT;i++)
		{
			nOffY=top+(bottom-top)*i/BAD_IMG_HEIGHT;
			for(j=0;j<BAD_IMG_WIDTH;j++)
			{
				nOffX=left+(right-left)*j/BAD_IMG_WIDTH;
				*(fmD+BAD_IMG_WIDTH*i+j)=g_DefectLUT[*(fm+pitch*nOffY+nOffX)];
			}
		}
	}
	else
	{
		for(i=0;i<BAD_IMG_HEIGHT;i++)
		{
			nOffY=top+(bottom-top)*i/BAD_IMG_HEIGHT;
			for(j=0;j<BAD_IMG_WIDTH;j++)
			{
				nOffX=left+(right-left)*j/BAD_IMG_WIDTH;
				*(fmD+BAD_IMG_WIDTH*i+j)=*(fm+pitch*nOffY+nOffX);
			}
		}
		
	}
	if(nGray>=0 && nGray<=255) *(fmD)=nGray;

	return 0;
}

void CopyNGImagePart(bool bFlat) 
{	
	CRect rect;
	int i, j, nPreUse=0;
	int nOffX, nOffY, nY1, nY2;
	int nOffset;
	int pitch=g_System.m_nPitch;
	LPBYTE fm;
	LPBYTE fmD=g_Defect.m_pImage[g_Defect.m_nDefectCount];

	if(bFlat) fm=g_fmFlat[g_ID];
	else      fm=g_fmGrab[g_ID];

	rect.left = rect.right = g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos;
	rect.top = rect.bottom = g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos;

	rect.InflateRect(BAD_IMG_WIDTH/2, BAD_IMG_HEIGHT/2);
	CheckArea(&rect);


	for(i=0;i<g_System.m_nImagePart;i++)
	{

		if( (g_System.m_nImagePartH*(i+1)-rect.top)*(g_System.m_nImagePartH*(i+1)-rect.bottom)<0)
		{
			if((g_System.m_nImagePartH*(i+1)-rect.top) > (rect.bottom-g_System.m_nImagePartH*(i+1)))
			{
				nOffset=(rect.bottom-g_System.m_nImagePartH*(i+1));
				rect.top   -= nOffset;
				rect.bottom-= nOffset;
			}
			else
			{
				nOffset=(g_System.m_nImagePartH*(i+1)-rect.top);
				rect.top   += nOffset;
				rect.bottom+= nOffset;
			}
			break;
		}
	}
	nOffX=rect.left;
	nOffY=rect.top;
	for(i=0;i<BAD_IMG_HEIGHT;i++)
		for(j=0;j<BAD_IMG_WIDTH;j++)
		{
			*(fmD+BAD_IMG_WIDTH*i+j)=g_DefectLUT[*(fm+pitch*(i+nOffY)+j+nOffX)];
		}
}


//0: 겹치는 것 없음
//1: 겹치는데 현재 검출한 것이 우선
//2: 겹치는데 현재 검출한 것을 버림.
int CheckDefectOverlap()
{
	if(g_Defect.m_nDefectCount>g_System.m_nMaxDefect) return 2;

	int nResult=0, i;
	int nLevelC=g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class/MILLION;
	int nTypeC =g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class%CLASSDEV;
	int nMarkC, nXC, nYC;

	int nMark, nX, nY;
	int nLevel, nType;
	int nDiffX, nDiffY;
	int nExistNum=-1, N;
	int dOffsetX, dOffsetY;		//8mmx8mm 영역에 있으면 겹치는 것으로 간주함. (최소 64x64)

	dOffsetX=g_System.m_nOverlapX;	//Overlap Range X
	dOffsetY=g_System.m_nOverlapY;  //Overlap Range Y
	if(g_Param.m_dScaleFactorX>0) dOffsetX=dOffsetX/g_Param.m_dScaleFactorX; 
	if(g_Param.m_dScaleFactorY>0) dOffsetY=dOffsetY/g_Param.m_dScaleFactorY; 
	if(dOffsetX<64) dOffsetX=64;	//Pixel로 따져서 64x64 보다 안쪽이면 같은 위치로 간주함.
	if(dOffsetY<64) dOffsetY=64;

	if(nLevelC < 1 || nLevelC > 10 || nTypeC < 1 || nTypeC > 9) return 2; //현재 검출한 데이타가 쓰레기라 버린다.

	nMarkC=g_Mark.nMarking[nTypeC-1][nLevelC-1];
	nXC   =g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos;
	nYC   =g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos;

	for(i=0;i<g_Defect.m_nDefectCount;i++)
	{
		nDiffX=nXC-g_Defect.m_Defect[i].x_pos; if(nDiffX<0) nDiffX*=-1;
		nDiffY=nYC-g_Defect.m_Defect[i].y_pos; if(nDiffY<0) nDiffY*=-1;

		if(nDiffX<dOffsetX && nDiffY<dOffsetY)
		{
			nLevel=g_Defect.m_Defect[i].defect_class/MILLION;
			nType =g_Defect.m_Defect[i].defect_class%CLASSDEV;
			nMark =g_Mark.nMarking[nType-1][nLevel-1];

			if(nMarkC==nMark) //1. 같은 마킹이면------------------------------------------------------------
			{
				if(nLevelC==nLevel)		//A. Level이 같으면 Value 또는 Size가 큰 것으로 선택
				{					
					if(nType == NG_SCRATCH && nTypeC==NG_SCRATCH)  //둘다 스크라치면 Value가 큰 것으로 선택
					{
						if(g_Defect.m_Defect[g_Defect.m_nDefectCount].value>g_Defect.m_Defect[i].value)
						{
							nExistNum=i;    //현재검출한 불량 선택
						}
						else
						{
							nResult=2;		//현재불량 버림
						}
						break;
					}
					else
					{
						if(nType == NG_SCRATCH) //기존불량만 스크라치이면 현재불량 버림
						{
							nResult=2;			//현재불량 버림
							break;
						}
						if(nTypeC==NG_SCRATCH) //현재검출한게 스크라치이면 기존불량을 엎어침.
						{
							nExistNum=i;		//현재검출한 불량 선택
							break;
						}

						//둘다 스크라치가 아니면 Size로 구분
						if(g_Defect.m_Defect[g_Defect.m_nDefectCount].size>g_Defect.m_Defect[i].size)
						{
							nExistNum=i;    //현재검출한 불량 선택
						}
						else if(g_Defect.m_Defect[g_Defect.m_nDefectCount].size==g_Defect.m_Defect[i].size) //Size가 같으면 Value 비교
						{
							if(g_Defect.m_Defect[g_Defect.m_nDefectCount].value>g_Defect.m_Defect[i].value) 
							{
								nExistNum=i;    
							}						
							else
							{
								nResult=2;		//현재불량 버림
							}
						}
						else
						{
							nResult=2;		//현재불량 버림
						}
					}
				}
				else					//B. Level이 다르면 Level낮은것 선택(강불량 선택)
				{
					if(nLevel>nLevelC)	//B-1. 기존 불량이 Level이 높으면(약불량)이면 현재것으로 대체한다.
					{
						nExistNum=i;
					}
					else				//B-2. 현재불량이 Level이 높으면(약불량)이면 현재것 버린다.
					{
						nResult=2;
					}
				}
			}
			else	//2.  마킹이 다르면 마킹하는 것 우선-------------------------------------------------
			{
				if(nMarkC)  //현재 검출한 것이 마킹이면 기존데이타를 현재 검출한 것으로 대체해버림.
				{
					nExistNum=i;					
				}
				else //같은 위치에 마킹이 있으면 현재 검출한 것은 버린다.
				{
					nResult=2;
				}
			}
			break;
		}
	}

	if(nExistNum>=0 && nExistNum<g_System.m_nMaxDefect)
	{
		N=g_Defect.m_nDefectCount;

		g_Defect.m_Defect[nExistNum].x_pos			=g_Defect.m_Defect[N].x_pos;
		g_Defect.m_Defect[nExistNum].y_pos			=g_Defect.m_Defect[N].y_pos;
		g_Defect.m_Defect[nExistNum].defect_class	=g_Defect.m_Defect[N].defect_class;
		g_Defect.m_Defect[nExistNum].size			=g_Defect.m_Defect[N].size; 
		g_Defect.m_Defect[nExistNum].value			=g_Defect.m_Defect[N].value; 
		g_Defect.m_Defect[nExistNum].sizeX			=g_Defect.m_Defect[N].sizeX; 
		g_Defect.m_Defect[nExistNum].sizeY			=g_Defect.m_Defect[N].sizeY; 
		for(i=0;i<2;i++)
			g_Defect.m_Defect[nExistNum].dTemp[i]=g_Defect.m_Defect[N].dTemp[i];

		//파일이름
		memcpy(g_Defect.m_Defect[nExistNum].filename, g_Defect.m_Defect[N].filename, MAX_BADIMAGE_FILENAME);

		//사각형--------------------------------------------------------------
		g_Defect.m_rcDefect[nExistNum].left		=g_Defect.m_rcDefect[N].left;
		g_Defect.m_rcDefect[nExistNum].top		=g_Defect.m_rcDefect[N].top;
		g_Defect.m_rcDefect[nExistNum].right	=g_Defect.m_rcDefect[N].right;
		g_Defect.m_rcDefect[nExistNum].bottom	=g_Defect.m_rcDefect[N].bottom;

		//Image--------------------------------------------------------------
		memcpy(g_Defect.m_pImage[nExistNum], g_Defect.m_pImage[N], BAD_IMG_WIDTH*BAD_IMG_HEIGHT);

		nResult=1;
	}
	else
	{
		if(nResult==0 && g_Defect.m_nDefectCount>=g_System.m_nMaxDefect)
			nResult=3;
	}


	return nResult;
}


void RemoveDefectByYPos(int nYPos,int nOffsetPixel)
{
	int i, j, nDiff, nDiff1;
	int N=g_Defect.m_nDefectCount;
	int nDefectCount=g_Defect.m_nDefectCount;
	bool flag[MAX_DEFECT];

	for(i=0;i<N;i++) flag[i]=true;
	
	for(i=0;i<N;i++)
	{
		nDiff=(g_Defect.m_rcDefect[i].top+g_Defect.m_rcDefect[i].bottom)/2-nYPos;
		if(nDiff<0) nDiff*=-1;
		if(nDiff<=nOffsetPixel) //i Defect 없앰
		{
			nDefectCount--;
			for(j=i+1;j<N;j++)
			{
				if(!flag[j]) continue;
				nDiff1=(g_Defect.m_rcDefect[j].top+g_Defect.m_rcDefect[j].bottom)/2-nYPos;
				if(nDiff1<0) nDiff1*=-1;
				if(nDiff1>nOffsetPixel)
				{
					g_Defect.m_Defect[i].x_pos			=g_Defect.m_Defect[j].x_pos;
					g_Defect.m_Defect[i].y_pos			=g_Defect.m_Defect[j].y_pos;
					g_Defect.m_Defect[i].defect_class	=g_Defect.m_Defect[j].defect_class;
					g_Defect.m_Defect[i].size			=g_Defect.m_Defect[j].size; 
					g_Defect.m_Defect[i].value			=g_Defect.m_Defect[j].value; 
					g_Defect.m_Defect[i].sizeX			=g_Defect.m_Defect[j].sizeX; 
					g_Defect.m_Defect[i].sizeY			=g_Defect.m_Defect[j].sizeY; 
					
					g_Defect.m_Defect[i].dTemp[0]=g_Defect.m_Defect[j].dTemp[0];
					g_Defect.m_Defect[i].dTemp[1]=g_Defect.m_Defect[j].dTemp[1];

					//파일이름
					memcpy(g_Defect.m_Defect[i].filename, g_Defect.m_Defect[j].filename, MAX_BADIMAGE_FILENAME);

					//사각형--------------------------------------------------------------
					g_Defect.m_rcDefect[i].left		=g_Defect.m_rcDefect[j].left;
					g_Defect.m_rcDefect[i].top		=g_Defect.m_rcDefect[j].top;
					g_Defect.m_rcDefect[i].right	=g_Defect.m_rcDefect[j].right;
					g_Defect.m_rcDefect[i].bottom	=g_Defect.m_rcDefect[j].bottom;

					//Image--------------------------------------------------------------
					memcpy(g_Defect.m_pImage[i], g_Defect.m_pImage[j], BAD_IMG_WIDTH*BAD_IMG_HEIGHT);
					flag[j]=false;
					break;
				}
			}
		}
	}
	g_Defect.m_nDefectCount=nDefectCount;
}


//수평노이즈 제거
//nBlackorWhite=0 흑 수평라인 제거
//              1 백 수평라인 제거
//              2 흑/백 수평라인제거
//nDiffValue : 수평라인이 평균값 대비 얼마나 어두우면 수평라인이라고 판정할 것인지
void RemoveDefectByHorNoize(int nBlackorWhite, int nDiffValue)
{
	int i, j, nDiff, nDiff1;
	int N=g_Defect.m_nDefectCount;
	int nDefectCount=g_Defect.m_nDefectCount;
	bool flag[MAX_DEFECT];
	int  nOverkill[MAX_DEFECT];

	for(i=0;i<N;i++) flag[i]=true;

	
	for(i=0;i<N;i++)
	{	
		nOverkill[i]=IsHorNoize(nBlackorWhite, g_Defect.m_pImage[i], 128, 128, 128, 15);
	}

	for(i=0;i<N;i++)
	{
		if(nOverkill[i]) //i Defect 없앰
		{
			nDefectCount--;
			for(j=i+1;j<N;j++)
			{
				if(!flag[j]) continue;
				
				if(nOverkill[j])
				{
					g_Defect.m_Defect[i].x_pos			=g_Defect.m_Defect[j].x_pos;
					g_Defect.m_Defect[i].y_pos			=g_Defect.m_Defect[j].y_pos;
					g_Defect.m_Defect[i].defect_class	=g_Defect.m_Defect[j].defect_class;
					g_Defect.m_Defect[i].size			=g_Defect.m_Defect[j].size; 
					g_Defect.m_Defect[i].value			=g_Defect.m_Defect[j].value; 
					g_Defect.m_Defect[i].sizeX			=g_Defect.m_Defect[j].sizeX; 
					g_Defect.m_Defect[i].sizeY			=g_Defect.m_Defect[j].sizeY; 
					
					g_Defect.m_Defect[i].dTemp[0]=g_Defect.m_Defect[j].dTemp[0];
					g_Defect.m_Defect[i].dTemp[1]=g_Defect.m_Defect[j].dTemp[1];

					//파일이름
					memcpy(g_Defect.m_Defect[i].filename, g_Defect.m_Defect[j].filename, MAX_BADIMAGE_FILENAME);

					//사각형--------------------------------------------------------------
					g_Defect.m_rcDefect[i].left		=g_Defect.m_rcDefect[j].left;
					g_Defect.m_rcDefect[i].top		=g_Defect.m_rcDefect[j].top;
					g_Defect.m_rcDefect[i].right	=g_Defect.m_rcDefect[j].right;
					g_Defect.m_rcDefect[i].bottom	=g_Defect.m_rcDefect[j].bottom;

					//Image--------------------------------------------------------------
					memcpy(g_Defect.m_pImage[i], g_Defect.m_pImage[j], BAD_IMG_WIDTH*BAD_IMG_HEIGHT);
					flag[j]=false;
					break;
				}
			}
		}
	}
	g_Defect.m_nDefectCount=nDefectCount;
}


//불량이 최대갯수보다 많을때 검사불량을 기존불량중에 약한 것으로 대체하는 함수
//현재 검출한 불량을 최우선으로 할꺼면 nIsPrior=1 아니면 0
void CheckPriorLevel(int nIsPrior)
{
	int nLevelC=g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class/MILLION;
	int nTypeC =g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class%CLASSDEV;	//현재불량 TYPE
	int nMarkC, nXC, nYC;
	
	int nLevel, nType, nMark, nX, nY;
	
	int i, N, nLevelMax=0, nExistNum=-1;

	if(nLevelC < 1 || nLevelC > 10 || nTypeC < 1 || nTypeC > 9) return; //현재 검출한 데이타가 불량이라 버린다.
	if(g_Defect.m_nDefectCount<g_System.m_nMaxDefect)           return; //불량갯수가 최대보다 작으면 넘어감.

	nMarkC=g_Mark.nMarking[nTypeC-1][nLevelC-1];
	nXC   =g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos;
	nYC   =g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos;

	if(nMarkC) //현재불량이 마킹하는 불량이면
	{		
		//마킹안하는 불량중에 Level이 약한 불량 선택
		for(i=g_Defect.m_nDefectCount-1;i>=0;i--)
		{
			nLevel=g_Defect.m_Defect[i].defect_class/MILLION;	
			nType =g_Defect.m_Defect[i].defect_class%CLASSDEV;
			nMark =g_Mark.nMarking[nType-1][nLevel-1];		         

			if(nMark==0)
			{
				if(nLevel>nLevelMax)
				{
					nLevelMax=nLevel;
					nExistNum=i;
				}
			}
		}
		//마킹하는 불량중에 Level이 약한 불량선택
		if(nLevelMax==0)
		{
			for(i=g_Defect.m_nDefectCount-1;i>=0;i--)
			{
				nLevel=g_Defect.m_Defect[i].defect_class/MILLION;	
				nType =g_Defect.m_Defect[i].defect_class%CLASSDEV;

				if(nType==nTypeC && nLevel==nLevelC) continue;	//같은 불량 같은 Level은					 PASS
				if(nLevel<nLevelC)                   continue;	//일단 Level이 낮으면						 PASS			
				if(nIsPrior==0 &&  nLevel==nLevelC)  continue;	//우선순위가 있는 불량이 아니면 같은 Level도 PASS 			

				if(nLevel>nLevelMax)
				{
					nLevelMax=nLevel;
					nExistNum=i;				
				}
			}
		}
	}
	else
	{
		for(i=g_Defect.m_nDefectCount-1;i>=0;i--)
		{
			nLevel=g_Defect.m_Defect[i].defect_class/MILLION;	
			nType =g_Defect.m_Defect[i].defect_class%CLASSDEV;
			nMark =g_Mark.nMarking[nType-1][nLevel-1];

			if(nMark==0) //현재 검출한 불량이 마킹안하는 불량이므로 마킹안하는 불량중에서 고른다.
			{
				if(nType==nTypeC && nLevel==nLevelC) continue; //같은 불량 같은 Level은						 PASS
				if(nLevel<nLevelC)                   continue;	//일단 Level이 낮으면						 PASS		
				if(nIsPrior==0 &&  nLevel==nLevelC)  continue;	//우선순위가 있는 불량이 아니면 같은 Level도 PASS 

				if(nLevel>nLevelMax)
				{
					nLevelMax=nLevel;
					nExistNum=i;
				}
			}
		}
	}

	if(nLevelMax<1) return;	//아무것도 못찾았으면 PASS
	
	if(nExistNum>=0 && nExistNum<g_System.m_nMaxDefect)
	{
		N=g_Defect.m_nDefectCount;

		g_Defect.m_Defect[nExistNum].x_pos			=g_Defect.m_Defect[N].x_pos;
		g_Defect.m_Defect[nExistNum].y_pos			=g_Defect.m_Defect[N].y_pos;
		g_Defect.m_Defect[nExistNum].defect_class	=g_Defect.m_Defect[N].defect_class;
		g_Defect.m_Defect[nExistNum].size			=g_Defect.m_Defect[N].size; 
		g_Defect.m_Defect[nExistNum].value			=g_Defect.m_Defect[N].value; 
		g_Defect.m_Defect[nExistNum].sizeX			=g_Defect.m_Defect[N].sizeX; 
		g_Defect.m_Defect[nExistNum].sizeY			=g_Defect.m_Defect[N].sizeY; 
		for(i=0;i<2;i++)
			g_Defect.m_Defect[nExistNum].dTemp[i]=g_Defect.m_Defect[N].dTemp[i];

		//파일이름
		memcpy(g_Defect.m_Defect[nExistNum].filename, g_Defect.m_Defect[N].filename, MAX_BADIMAGE_FILENAME);

		//사각형--------------------------------------------------------------
		g_Defect.m_rcDefect[nExistNum].left		=g_Defect.m_rcDefect[N].left;
		g_Defect.m_rcDefect[nExistNum].top		=g_Defect.m_rcDefect[N].top;
		g_Defect.m_rcDefect[nExistNum].right	=g_Defect.m_rcDefect[N].right;
		g_Defect.m_rcDefect[nExistNum].bottom	=g_Defect.m_rcDefect[N].bottom;

		//Image--------------------------------------------------------------
		memcpy(g_Defect.m_pImage[nExistNum], g_Defect.m_pImage[N], BAD_IMG_WIDTH*BAD_IMG_HEIGHT);
	}
	
	return;
}


//1: 정상처리
//2: 방금 검출한 불량 버림.
int CheckDefectOverlapAdjust()
{
	int nResult=0, i;
	int nLevelC=g_DefectTemp.m_Defect.defect_class/MILLION;
	int nTypeC =g_DefectTemp.m_Defect.defect_class%CLASSDEV;
	int nMarkC, nXC, nYC;

	int nExistNum=-1, N;
	int nLevel, nType, nLevelMax=0, nCandiNum;
	int nMark, nX, nY;
	int nDiffX, nDiffY;
	int dOffsetX, dOffsetY;			//8mmx8mm 영역에 있으면 겹치는 것으로 간주함.

	 dOffsetX=g_System.m_nOverlapX;	 //Overlap Range X
	 dOffsetY=g_System.m_nOverlapY;  //Overlap Range Y
	if(g_Param.m_dScaleFactorX>0) dOffsetX=dOffsetX/g_Param.m_dScaleFactorX; 
	if(g_Param.m_dScaleFactorY>0) dOffsetY=dOffsetY/g_Param.m_dScaleFactorY; 
	if(dOffsetX<64) dOffsetX=64;	//Pixel로 따져서 64x64 보다 안쪽이면 같은 위치로 간주함.
	if(dOffsetY<64) dOffsetY=64;

	if(nLevelC < 1 || nLevelC > 10 || nTypeC < 1 || nTypeC > 9) return 2; //현재 검출한 데이타가 못쓰는 데이타라 버린다.

	nMarkC=g_Mark.nMarking[nTypeC-1][nLevelC-1];
	nXC   =g_DefectTemp.m_Defect.x_pos;
	nYC   =g_DefectTemp.m_Defect.y_pos;

	for(i=0;i<g_Defect.m_nDefectCount;i++)
	{
		nDiffX=nXC-g_Defect.m_Defect[i].x_pos; if(nDiffX<0) nDiffX*=-1;
		nDiffY=nYC-g_Defect.m_Defect[i].y_pos; if(nDiffY<0) nDiffY*=-1;

		if(nDiffX<dOffsetX && nDiffY<dOffsetY)
		{
			nLevel=g_Defect.m_Defect[i].defect_class/MILLION;
			nType =g_Defect.m_Defect[i].defect_class%CLASSDEV;
			nMark =g_Mark.nMarking[nType-1][nLevel-1];

			if(nMarkC==nMark) //1. 같은 마킹이면------------------------------------------------------------
			{
				if(nTypeC==NG_GROUP)
				{
					nResult=2;
					break;
				}

				if(nLevelC==nLevel)		//A. Level이 같으면 Size가 큰 것으로 선택
				{
					//마킹, Level 같은에 기존불량이 아래와 같으면 방금검출한 불량을 버린다.
					if(nType == NG_SCRATCH || nType == NG_KKOPCHIM)
					{
						nResult=2;
						break;
					}

					if(g_DefectTemp.m_Defect.size>g_Defect.m_Defect[i].size)
					{
						nExistNum=i;    //기존불량 선택
					}
					else
					{
						nResult=2;		//현재불량 버림
					}
				}
				else					//B. Level이 다르면 Level낮은것 선택(강불량 선택)
				{
					if(nLevel>nLevelC)	//B-1. 기존 불량이 Level이 높으면(약불량)이면 현재것으로 대체한다.
					{
						nExistNum=i;
					}
					else				//B-2. 현재불량이 Level이 높으면(약불량)이면 현재것 버린다.
					{
						nResult=2;
					}
				}
			}
			else	//2.  마킹상황이 다르면 마킹하는 것 우선-------------------------------------------------
			{
				if(nMarkC)  //현재 검출한 것이 마킹이면 기존데이타를 현재 검출한 것으로 대체해버림.
				{
					nExistNum=i;					
				}
				else //같은 위치에 마킹이 있으면 현재 검출한 것은 버린다.
				{
					nResult=2;
				}
			}
			break;
		}
	}
	if(nResult==2) return nResult;

	if(nExistNum<0) //일치하는게 없으면
	{
		if(g_Defect.m_nDefectCount<g_System.m_nMaxDefect) //아직 MAX Defect에 도달 하지 못했으면 그냥 추가
		{
			nExistNum=g_Defect.m_nDefectCount;
			g_Defect.m_nDefectCount++;
		}
		else	//MAX Defect에 도달했으면 막 검출한 불량보다 마킹을 안하거나 Level이 낮으면 제거
		{
			//마킹안하는 것중에 Level이 가장 높은 것 구함. -------------------------
			nCandiNum=-1;
			for(i=0;i<g_Defect.m_nDefectCount;i++)
			{
				nLevel=g_Defect.m_Defect[i].defect_class/MILLION;
				nType =g_Defect.m_Defect[i].defect_class%CLASSDEV;
				nMark =g_Mark.nMarking[nType-1][nLevel-1];
				if(nMark==0)
				{
					if(nLevel>nLevelMax)
					{
						nLevelMax=nLevel;
						nCandiNum=i;
					}
				}
			}
			if(nCandiNum>=0 && nLevelC<nLevelMax) {nExistNum=nCandiNum;} //그것이 현재 검출한 것보다 Level이 높으면 현재 검출한 것으로 대체
			else
			{
				if(nMarkC==0) return nResult;		//현재 검출한 것이 마킹을 하지 않으면 여기서 STOP
				else								//현재 검출한 것이 마킹을 하면 마킹하는 것 중에서 Level이 가장 높은 것 찾음
				{
						nCandiNum=-1;
						nLevelMax=0;
						for(i=0;i<g_Defect.m_nDefectCount;i++)
						{
							nLevel=g_Defect.m_Defect[i].defect_class/MILLION;
							nType =g_Defect.m_Defect[i].defect_class%CLASSDEV;
							nMark =g_Mark.nMarking[nType-1][nLevel-1];
							if(nMark)
							{
								if(nLevel>nLevelMax)
								{
									nLevelMax=nLevel;
									nCandiNum=i;
								}
							}
						}
						if(nCandiNum>=0 && nLevelC<nLevelMax) {nExistNum=nCandiNum;} //그것이 현재 검출한 것보다 Level이 높으면 현재 검출한 것으로 대체
				}
			}
		}
	}

	if(nExistNum>=0)
	{
		g_Defect.m_nFrameNum						=g_DefectTemp.m_nFrameNum;
		g_Defect.m_Defect[nExistNum].x_pos			=g_DefectTemp.m_Defect.x_pos;
		g_Defect.m_Defect[nExistNum].y_pos			=g_DefectTemp.m_Defect.y_pos;
		g_Defect.m_Defect[nExistNum].defect_class	=g_DefectTemp.m_Defect.defect_class;
		g_Defect.m_Defect[nExistNum].size			=g_DefectTemp.m_Defect.size; 
		g_Defect.m_Defect[nExistNum].value			=g_DefectTemp.m_Defect.value; 
		g_Defect.m_Defect[nExistNum].sizeX			=g_DefectTemp.m_Defect.sizeX; 
		g_Defect.m_Defect[nExistNum].sizeY			=g_DefectTemp.m_Defect.sizeY; 
		for(i=0;i<2;i++)
			g_Defect.m_Defect[nExistNum].dTemp[i]=g_DefectTemp.m_Defect.dTemp[i];

		//파일이름
		memcpy(g_Defect.m_Defect[nExistNum].filename, g_DefectTemp.m_Defect.filename, MAX_BADIMAGE_FILENAME);

		//사각형--------------------------------------------------------------
		g_Defect.m_rcDefect[nExistNum].left		=g_DefectTemp.m_rcDefect.left;
		g_Defect.m_rcDefect[nExistNum].top		=g_DefectTemp.m_rcDefect.top;
		g_Defect.m_rcDefect[nExistNum].right	=g_DefectTemp.m_rcDefect.right;
		g_Defect.m_rcDefect[nExistNum].bottom	=g_DefectTemp.m_rcDefect.bottom;

		//Image--------------------------------------------------------------
		memcpy(g_Defect.m_pImage[nExistNum], g_DefectTemp.m_pImage, BAD_IMG_WIDTH*BAD_IMG_HEIGHT);

		nResult=1;
	}

	return nResult;
}

void CheckArea(CRect *rect)
{
	if(rect->left	< 0)					rect->OffsetRect(-rect->left, 0);
	if(rect->top	< 0)					rect->OffsetRect(0, -rect->top);
	if(rect->right	>= g_System.m_nImageW)	rect->OffsetRect(g_System.m_nImageW-rect->right, 0);
	if(rect->bottom >= g_System.m_nImageH)	rect->OffsetRect(0, g_System.m_nImageH-rect->bottom);
}


#define LINE_KIPO_OFFSET	5
int l_nLineKipoProfile[MAX_WIDTH];
int l_nLineKipoProfileMV[MAX_WIDTH];
void WEB_LineKipo()
{
	int nIndex, nKipoMarking=0;
	int i,j, ii,jj,k,m,nX, nY=0;
	CRect rect;
	LPBYTE fm = g_fmGrab[g_ID];//pImage->GetImagePtr();
	int tt,bb,ll,rr, nXi, nYi;
	int nl, nR, nTmp, nTmp1, nLevel, nClass, nMaxLevel=1;
	int nTH=g_Param.m_nLineKipoTh;
	int nKipoW=g_Param.m_nLineKipoWidth, nPitchDest;
	int nSum=0, nMax=0, nMaxPos=-1, nMaxY=0, nMaxYPos=-1, nMaxPosJ=0, nMaxPos1, nMaxData=0;
	int nLevelInapect=0, nKipoMinValue=9999;
	long nSumAll=0;
	int nOffset=2;
	int nWidth, nHeight, nPitch, nInspectX1, nInspectX2, nMulti=1;
	LPBYTE fmDest;

	g_Temp.m_nMaxLinekipo=0;
	g_Temp.m_nMaxLinekipoPos=0;


	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])
		{
			if(g_Param.m_nValueLineKipo[i]>100 )	
			{
				if(g_Param.m_nValueLineKipo[i]<nKipoMinValue)
				{
					nKipoMinValue=g_Param.m_nValueLineKipo[i];
					nLevelInapect=1;
				}
			}
		}
	}
	if(nLevelInapect==0) return;

	nWidth		=g_System.m_nImageW;
	nHeight		=g_System.m_nImageH;
	nPitch		=g_System.m_nPitch;
	nInspectX1	=g_Temp.m_nInspectX1;
	nInspectX2	=g_Temp.m_nInspectX2;

	if(g_System.m_nMakePyramid)  //8192이면 1/2영상에서 t
	{
		fm=g_fmGrabComp;
		nKipoW	/=2;
		nWidth	/=2;
		nHeight	/=2;
		nPitch	/=2;
		nInspectX1/=2;
		nInspectX2/=2;
		nMulti=2;
	}
	rect.top=0;
	rect.bottom=nHeight-1;
	rect.left=nInspectX1;
	rect.right=nInspectX2;




	if(g_Defect.m_nDefectCount >= g_System.m_nMaxDefect)
	{		
		for(j=0;j<10;j++)
			if(g_Mark.nData[4][j])	//[4] 5번째가 라인기포마킹인데, 기포마킹을 하면 마킹안하는것에 기포마킹을 엎는다.
				nKipoMarking=1;

		nIndex=-1;
		if(nKipoMarking)  //마킹 안하는 것중에 하나 고름
		{
			for(j=g_Defect.m_nDefectCount-1;j>=0;j--)
			{
				nLevel=g_Defect.m_Defect[j].defect_class/MILLION;
				nClass =g_Defect.m_Defect[j].defect_class%CLASSDEV;
				if(nClass<1 || nClass>10 || nLevel<1 || nLevel>10) continue;
				if(g_Mark.nData[nClass-1][nLevel-1]==0)
				{
					nIndex=j;
					break;
				}
			}
		}
		else	//Level이 높은숫자를 하나 고름
		{
			for(j=g_Defect.m_nDefectCount-1;j>=0;j--)
			{
				nLevel=g_Defect.m_Defect[j].defect_class/MILLION;
				if(nLevel>nMaxLevel)
				{
					nMaxLevel=nLevel;
					nIndex=j;
				}
			}
			if(nMaxLevel==1) nIndex=-1;
		}

		if(nIndex<0)
		{
			return;
		}
	}
	else
	{
		nIndex=g_Defect.m_nDefectCount;
	}



	if(nTH<=2 || nKipoW<1) return;
	nl  = rect.left+3;
	nR	= rect.right-3;



	if(nR-nl<2*nKipoW || nKipoW<=0) return;

	memset(l_nLineKipoProfile, 0, MAX_WIDTH*sizeof(int));
	memset(l_nLineKipoProfileMV, 0, MAX_WIDTH*sizeof(int));

	for(i=10;i<rect.bottom-10;i+=nOffset)
	{
		nY++;
		for(j=nl+LINE_KIPO_OFFSET;j<nR-LINE_KIPO_OFFSET;j++)
		{
			nTmp=*(fm+ nPitch*i+j)-*(fm+ nPitch*i+j+LINE_KIPO_OFFSET);
			nTmp1=*(fm+ nPitch*i+j)-*(fm+ nPitch*i+j-LINE_KIPO_OFFSET);
			if(nTmp>=nTH && nTmp1>=nTH)
				l_nLineKipoProfile[j]+=1;
		}		
	}

	for(j=nl+LINE_KIPO_OFFSET;j<nR-LINE_KIPO_OFFSET;j++)
		l_nLineKipoProfile[j]=(int)(l_nLineKipoProfile[j]*((double)nHeight/nY));

	//이동평균시작점
	for(j=nl+LINE_KIPO_OFFSET;j<nl+LINE_KIPO_OFFSET+nKipoW;j++)
	{
		nSum+=l_nLineKipoProfile[j];
	}
	l_nLineKipoProfileMV[nl]=nSum;
	//이동평균구하기
	for(j=nl+LINE_KIPO_OFFSET+1;j<nR-LINE_KIPO_OFFSET-nKipoW;j++)
	{
		nSum=nSum-l_nLineKipoProfile[j-1]+l_nLineKipoProfile[j+nKipoW-1];
		l_nLineKipoProfileMV[j]=nSum;
	}

	for(j=nl+LINE_KIPO_OFFSET;j<nR-LINE_KIPO_OFFSET-nKipoW;j++)
	{
		if(l_nLineKipoProfileMV[j]>nMax)
		{
			nMax=l_nLineKipoProfileMV[j];
			nMaxPos=j+nKipoW/2;
			nMaxPosJ=j;
		}		
	}

	nSumAll=0;
	if(nMaxPosJ>100 && nMaxPosJ<nWidth-100)
	{
		if(nMaxPosJ>nl+LINE_KIPO_OFFSET+100 && nMaxPosJ<nR-LINE_KIPO_OFFSET-nKipoW-101)  nSumAll=(l_nLineKipoProfileMV[nMaxPosJ-100]+l_nLineKipoProfileMV[nMaxPosJ+100])/2;
		else if(nMaxPosJ<=nl+LINE_KIPO_OFFSET+100)			nSumAll=l_nLineKipoProfileMV[nMaxPosJ+100];
		else if(nMaxPosJ>=nR-LINE_KIPO_OFFSET-nKipoW-101)   nSumAll=l_nLineKipoProfileMV[nMaxPosJ-100];
	}

	nMax-=nSumAll; if(nMax<0) nMax=0;
	nMax*=nMulti;
	g_Temp.m_nMaxLinekipo=nMax;
	g_Temp.m_nMaxLinekipoPos=nMaxPos*nMulti;

#ifdef USE_HOLE_FIND	
	if(!IsDefectInTP(nMaxPos, 0)) return;
#endif

	if(nMaxPos>0 && nMax>=nKipoMinValue)
	{
		if(nMaxPos<BAD_IMG_WIDTH/2)					nMaxPos=BAD_IMG_WIDTH/2;
		if(nMaxPos>=nPitch-BAD_IMG_WIDTH/2)       nMaxPos=nPitch-BAD_IMG_WIDTH/2;
		nLevel=GetLevel(nMax, g_Param.m_nValueLineKipo);

		//--------MAX Y찾기----------------------------------------------
		nl=nMaxPos-nKipoW/2;
		nR=nMaxPos+nKipoW/2;

		//추가함------------------------------------------
		nMaxPos1=nMaxData=0;
		for(i=nl;i<=nR;i++)
		{
			if(i<rect.left || i>rect.right) continue;

			if(l_nLineKipoProfile[i]>nMaxData)
			{
				nMaxData=l_nLineKipoProfile[i];
				nMaxPos1=i;
			}
		}
		if(nMaxData>0)
		{
			nMaxPos=nMaxPos1;
			nl=nMaxPos-nKipoW/2;
			nR=nMaxPos+nKipoW/2;
		}
		//------------------------------------------------


		memset(l_nLineKipoProfile, 0, nHeight*sizeof(int));
		for(i=10;i<rect.bottom-10;i++)
		{
			for(j=nl+LINE_KIPO_OFFSET;j<nR-LINE_KIPO_OFFSET;j++)
			{
				nTmp=*(fm+ nPitch*i+j)-*(fm+ nPitch*i+j+LINE_KIPO_OFFSET);
				nTmp1=*(fm+ nPitch*i+j)-*(fm+ nPitch*i+j-LINE_KIPO_OFFSET);
				if(nTmp>=nTH && nTmp1>=nTH)
					l_nLineKipoProfile[i/32]+=1;
			}		
		}
		for(i=0;i<nHeight/32;i++)
		{
			if(l_nLineKipoProfile[i]>nMaxY)
			{
				nMaxY=l_nLineKipoProfile[i];
				nMaxYPos=i*32;
			}
		}
		if(nMaxYPos>=0)
		{
			nX=nMaxPos*2; 
			nY=nMaxYPos*2;
		
			nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_LINEKIPO;
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			MakeDefectRect(nIndex, nX, nY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			g_Defect.m_Defect[nIndex].x_pos=nX;
			g_Defect.m_Defect[nIndex].y_pos=nY;
			g_Defect.m_Defect[nIndex].defect_class=nClass;
			g_Defect.m_Defect[nIndex].value	   =nMax; //Value	
			g_Defect.m_Defect[nIndex].size        =1; //Size
			g_Defect.m_Defect[nIndex].sizeX       =1; //Size X
			g_Defect.m_Defect[nIndex].sizeY       =1; //Size Y

			
//			UpdateNGImage(pImage, pData);
//불량영상저장(1/4 압축영상)------------------------------------------------------------
			fmDest	    = g_Defect.m_pImage[nIndex];
			memset(fmDest,0, BAD_IMG_WIDTH*BAD_IMG_HEIGHT);
			nPitchDest	= BAD_IMG_HEIGHT;
	
			//Y범위
			tt=nMaxYPos-BAD_IMG_HEIGHT/2;
			if(tt<0) tt=0;
			bb=tt+BAD_IMG_HEIGHT;
			if(bb>=nHeight) {bb=nHeight-1;tt=nHeight-BAD_IMG_HEIGHT;}
			
			//X범위
			ll=nMaxPos-BAD_IMG_HEIGHT/2; if(ll<nInspectX1+LINE_KIPO_OFFSET)  ll=nInspectX1+LINE_KIPO_OFFSET;
			rr=ll+BAD_IMG_HEIGHT;
			if(rr>nInspectX2-LINE_KIPO_OFFSET)
			{
				rr=nInspectX2-LINE_KIPO_OFFSET;
				ll=rr-BAD_IMG_HEIGHT;
				if(ll<nInspectX1+LINE_KIPO_OFFSET) ll=nInspectX1+LINE_KIPO_OFFSET;
			}

			for(ii=tt;ii<bb;ii++)
			{						
				for(jj=ll;jj<rr;jj++)
				{
					nTmp=*(fm+nPitch*ii+jj)-*(fm+nPitch*ii+jj+LINE_KIPO_OFFSET);
					nTmp1=*(fm+nPitch*ii+jj)-*(fm+nPitch*ii+jj-LINE_KIPO_OFFSET);
					if(nTmp>=nTH && nTmp1>=nTH)
					*(fmDest+nPitchDest*(ii-tt)+jj-ll)=255;	
				}
			}
			if(nIndex<g_System.m_nMaxDefect && g_Defect.m_nDefectCount<g_System.m_nMaxDefect) 
				g_Defect.m_nDefectCount++;
		}			
	}
}





void ImageCompression(unsigned char *pSrc, unsigned char *pDest, unsigned int nImageHeight, unsigned int nPitch, unsigned int nStartX, unsigned int  nEndX)
{
	__m128i ZeroData = _mm_setzero_si128();
	unsigned int nCompressionWidth = (unsigned int)(nPitch / 2);

	for(unsigned int j = 0; j < nImageHeight; j+=2)
	{
		for( unsigned int i = nStartX; i < nEndX; i+=32)//pitch가 8192인 경우 32의 배수 이므로 나머지 처리를 하지 않음.
		{
			__m128i ImageLowSrc[4];
			__m128i ImageHighSrc[4];
			__m128i TempHigh[2];
			__m128i TempLow[2];
			__m128i Result;
			int iterSrc[4] = {0,};
			int iterDest = ( i / 2 ) + nCompressionWidth * ( j / 2);

			iterSrc[0] = i + nPitch * j;
			iterSrc[1] = i + nPitch * ( j + 1 );
			iterSrc[2] = ( i + 16 ) + nPitch * j;
			iterSrc[3] = ( i + 16 ) + nPitch * ( j + 1 );

			Result = _mm_setzero_si128();

			ImageLowSrc[0] = _mm_loadu_si128((__m128i*)(pSrc+iterSrc[0]));
			ImageLowSrc[1] = _mm_loadu_si128((__m128i*)(pSrc+iterSrc[1]));
			ImageLowSrc[2] = _mm_loadu_si128((__m128i*)(pSrc+iterSrc[2]));
			ImageLowSrc[3] = _mm_loadu_si128((__m128i*)(pSrc+iterSrc[3]));

			//상위 위치 short형 변환
			//하위 위치 short형 변환
			ImageHighSrc[0] = _mm_unpackhi_epi8(ImageLowSrc[0],ZeroData);
			ImageLowSrc[0] = _mm_unpacklo_epi8(ImageLowSrc[0],ZeroData);
			ImageHighSrc[1] = _mm_unpackhi_epi8(ImageLowSrc[1],ZeroData);
			ImageLowSrc[1] = _mm_unpacklo_epi8(ImageLowSrc[1],ZeroData);
			ImageHighSrc[2] = _mm_unpackhi_epi8(ImageLowSrc[2],ZeroData);
			ImageLowSrc[2] = _mm_unpacklo_epi8(ImageLowSrc[2],ZeroData);
			ImageHighSrc[3] = _mm_unpackhi_epi8(ImageLowSrc[3],ZeroData);
			ImageLowSrc[3] = _mm_unpacklo_epi8(ImageLowSrc[3],ZeroData);

			TempHigh[0] = _mm_avg_epu16(ImageHighSrc[0], ImageHighSrc[1]);
			TempLow[0] = _mm_avg_epu16(ImageLowSrc[0], ImageLowSrc[1]);
			TempHigh[1] = _mm_avg_epu16(ImageHighSrc[2], ImageHighSrc[3]);
			TempLow[1] = _mm_avg_epu16(ImageLowSrc[2], ImageLowSrc[3]);

			TempLow[0] = _mm_packus_epi16(TempLow[0], TempHigh[0]);
			TempLow[1] = _mm_packus_epi16(TempLow[1], TempHigh[1]);

			Result.m128i_i8[0] = (char)((TempLow[0].m128i_u8[0] + TempLow[0].m128i_u8[1]) / 2);
			Result.m128i_i8[1] = (char)((TempLow[0].m128i_u8[2] + TempLow[0].m128i_u8[3]) / 2);
			Result.m128i_i8[2] = (char)((TempLow[0].m128i_u8[4] + TempLow[0].m128i_u8[5]) / 2);
			Result.m128i_i8[3] = (char)((TempLow[0].m128i_u8[6] + TempLow[0].m128i_u8[7]) / 2);
			Result.m128i_i8[4] = (char)((TempLow[0].m128i_u8[8] + TempLow[0].m128i_u8[9]) / 2);
			Result.m128i_i8[5] = (char)((TempLow[0].m128i_u8[10] + TempLow[0].m128i_u8[11]) / 2);
			Result.m128i_i8[6] = (char)((TempLow[0].m128i_u8[12] + TempLow[0].m128i_u8[13]) / 2);
			Result.m128i_i8[7] = (char)((TempLow[0].m128i_u8[14] + TempLow[0].m128i_u8[15]) / 2);
			Result.m128i_i8[8] = (char)((TempLow[1].m128i_u8[0] + TempLow[1].m128i_u8[1]) / 2);
			Result.m128i_i8[9] = (char)((TempLow[1].m128i_u8[2] + TempLow[1].m128i_u8[3]) / 2);
			Result.m128i_i8[10] = (char)((TempLow[1].m128i_u8[4] + TempLow[1].m128i_u8[5]) / 2);
			Result.m128i_i8[11] = (char)((TempLow[1].m128i_u8[6] + TempLow[1].m128i_u8[7]) / 2);
			Result.m128i_i8[12] = (char)((TempLow[1].m128i_u8[8] + TempLow[1].m128i_u8[9]) / 2);
			Result.m128i_i8[13] = (char)((TempLow[1].m128i_u8[10] + TempLow[1].m128i_u8[11]) / 2);
			Result.m128i_i8[14] = (char)((TempLow[1].m128i_u8[12] + TempLow[1].m128i_u8[13]) / 2);
			Result.m128i_i8[15] = (char)((TempLow[1].m128i_u8[14] + TempLow[1].m128i_u8[15]) / 2);

			_mm_storeu_si128( (__m128i*)(pDest+iterDest),Result);
		}
	}
}

int FindAngleLine(LPBYTE fm, int left, int right, int nHeightT, int pitch, int nLimitAngle, int nOffsetAngle, int *pAngle, int *pPos, int *pValue);
int LineSum(LPBYTE FM, int x1, int y1, int x2, int y2, int pitch);
int FindLineBW_TOO_Angle(int left, int right, int *pProfile, int *pX, int *pValue);
BYTE l_fmALine[256*256];
void WEB_AngleLine()
{
	int i, j, m, loop, nOverlap;
	int nLevel;
	int nClass, nClass_, nNGType;
	int nAngleLineInspect=0;
	int pitch, width, height;
	int nX1, nX2;
	int nAngle, nPosX, nValue;
	int nMaxValue=0, nMaxPosX, nMaxPosY, nMaxAngle;
	int ll,tt,bb,rr;
	LPBYTE fm;

	g_Temp.m_nMaxGray=0;
	g_Temp.m_nMinGray=255;
	nX1=g_Temp.m_nInspectX1/32;	
	nX2=g_Temp.m_nInspectX2/32;

	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])
		{
			if(g_Param.m_dLineThresholdAngle[i]>0 )	  nAngleLineInspect=1;
		}
	}
	if(nAngleLineInspect==0) return;

	fm=l_fmPyramid[4];
	width = l_nPyramidWidth[4];
	height=l_nPyramidHeight[4];
	pitch =width/2;

	for(i=0;i<height;i+=2)
		for(j=0;j<width;j+=2)
			*(l_fmALine+pitch*(i/2)+j/2)=(*(fm+width*i+j)+*(fm+width*i+j+1)+*(fm+width*i+j+width)+*(fm+width*i+j+width+1))/4;

	for(loop=0;loop<4;loop++)
	{
		FindAngleLine(l_fmALine+loop*64*pitch, nX1,nX2, 64, pitch, 35, 2, &nAngle, &nPosX, &nValue);

		if(nValue>nMaxValue)
		{
			nMaxValue=nValue;
			nMaxPosX=nPosX;
			nMaxPosY=loop*2048+1024;
			nMaxAngle=nAngle;
		}
	}

	nLevel=GetLevel(nMaxValue, g_Param.m_dLineThresholdAngle);

	if(nLevel>0)
	{
		MakeDefectRect(g_Defect.m_nDefectCount, nMaxPosX, nMaxPosY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
		nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_LINEKIPO;
//CopyNGImage(true);

		LPBYTE fmD=g_Defect.m_pImage[g_Defect.m_nDefectCount];
		ll=nMaxPosX/8-64; if(ll<0) ll=0;
		rr=ll+128;        if(rr>width*2) {rr=width*2-1; ll=rr-128;}
		tt=nMaxPosY/8-64; if(tt<0) tt=0;
		bb=tt+128;        if(bb>width/2) {bb=height/2-1;tt=bb-128;}

		for(i=tt;i<bb;i++)
			for(j=ll;j<rr;j++)
				*(fmD+(i-tt)*128+(j-ll))=*(l_fmPyramid[3]+width*2*i+j);


		g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nMaxPosX;	 //불량위치 X (Pixel)
		g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nMaxPosY;	 //불량위치 Y (Pixel)
		g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;				 //불량TYPE
		g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =nMaxAngle;			 //Size
		g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nMaxValue;	 //Value	
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =nMaxAngle;			 //Size X
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =nMaxAngle;			 //Size Y

		//겹치는 불량 점검-----------------------------------------------------------------------------------------------
		nOverlap=CheckDefectOverlap();
		if(nOverlap==0 && g_Defect.m_nDefectCount<g_System.m_nMaxDefect) //안겹치고 불량갯수도 최대치보다 작으면 추가함.
		{
			g_Defect.m_nDefectCount++;
		}
		else
		{
			if(nOverlap==3) CheckPriorLevel(1); //겹치는것 없고, 불량갯수가 이미 최대치에 도달했으면				
		}
		//---------------------------------------------------------------------------------------------------------------		
	}

}

//256x64로 만들어서 검사
//[i]-[i+1], [i]-[i+2], [i]-[i+3], [i]-[i+4] 비교
int FindAngleLine(LPBYTE fm, int nl, int nR, int nHeightT, int pitch, int nLimitAngle, int nOffsetAngle, int *pAngle, int *pPos, int *pValue)
{
	int i, m, mm, j, k, a, nTmp;
	int ok=-1;
	int nProfile[256];
	int nX, nValue, nValueMax=0, nValueMaxPos, nValueM;

	for(m=1;m<nLimitAngle;m+=2)
	{
		for(mm=0;mm<2;mm++)
		{
			if(mm==0)
			{
				for(j=nl;j<nR-m;j++)
				{
					nProfile[j]=LineSum(fm, j, 0, j+m, nHeightT-1, pitch);
				}
			}
			else
			{
				for(j=nl;j<nR-m;j++)
				{
					nProfile[j]=LineSum(fm, j+m, 0, j, nHeightT-1, pitch);
				}
			}

			//-------------------------------------------------------------
			for(k=0;k<4;k++)
			{
				//---------------------------------------------------------
				if(k==0) 
				{
					for(a=nl;a<nR-m-2;a++)
					{
						nTmp=nProfile[a]-nProfile[a+1]; if(nTmp<0) nTmp*=-1;
						if(nTmp>nValueMax)
						{
							nValueMax=nTmp;
							if(mm==0) nValueM=m;
							else     nValueM=-m;
							nValueMaxPos=(a+m/2)*32;
						}
					}
				}
				if(k==1) 
				{
					for(a=nl;a<nR-m-3;a++)
					{
						nTmp=nProfile[a]-nProfile[a+2]; if(nTmp<0) nTmp*=-1;
						if(nTmp>nValueMax)
						{
							nValueMax=nTmp;
							if(mm==0) nValueM=m;
							else     nValueM=-m;
							nValueMaxPos=(a+m/2)*32;
						}
					}
				}
				if(k==2) 
				{
					for(a=nl;a<nR-m-4;a++)
					{
						nTmp=nProfile[a]-nProfile[a+3]; if(nTmp<0) nTmp*=-1;
						if(nTmp>nValueMax)
						{
							nValueMax=nTmp;
							if(mm==0) nValueM=m;
							else     nValueM=-m;
							nValueMaxPos=(a+m/2)*32;
						}
					}
				}
				if(k==3) 
				{
					for(a=nl;a<nR-m-5;a++)
					{
						nTmp=nProfile[a]-nProfile[a+4]; if(nTmp<0) nTmp*=-1;
						if(nTmp>nValueMax)
						{
							nValueMax=nTmp;
							if(mm==0) nValueM=m;
							else     nValueM=-m;
							nValueMaxPos=(a+m/2)*32;
						}
					}
				}
				//------------------------------------------------------------
			}
			//----------------------------------------------------------------
		}
	}
	*pAngle=nValueM;
	*pPos=nValueMaxPos;
	*pValue=nValueMax*100/64;
	ok=1;

	g_Temp.m_nMaxAngleLineValue=nValueM;
	g_Temp.m_nMaxAngleLinePos  =nValueMaxPos;

	return ok;
}

int LineSum(LPBYTE FM, int x1, int y1, int x2, int y2, int pitch)
 {
	int x,y,dx,dy,  d, inc1, inc2, xend, yend, pixel;
	int nSum=0;

	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	if(dx > dy)
	{									/* slope between 0 and 1 */
		d = (dy<<1) -dx;
		inc1 = dy<<1;
		inc2 = 2 * (dy - dx);
										/* draw from smallest x to largest */
		if(x1 >x2)
		{
			x = x2;	y = y2;	xend = x1;
		}
		else
		{
			x = x1;	y = y1;	xend = x2;
		}
		nSum+= *(FM+x+y*pitch);
		
		if( (y1 < y2 && xend == x2) || (y1>y2 && xend == x1))
			while(x++ < xend)
			{
				if(d<0) d += inc1;
				else
				{
					y++;	d += inc2;
				}
				nSum+=  *(FM+x+y*pitch);				
			}
		else while(x++ < xend)
		{
			if(d<0) d += inc1;
			else
			{
				y--;
				d += inc2;
			}
			nSum+= *(FM+x+y*pitch);			
		}
	}
	else
	{
		d = (dx <<1) - dy;
		inc1 = dx <<1;
		inc2 = 2 * (dx - dy);
		if(y1 > y2)
		{
			y = y2;
			x = x2;
			yend = y1;
		}
		else
		{
			y = y1;
			x = x1;
			yend = y2;
		}
		nSum+= *(FM+x+y*pitch);
		
		if( (x1 < x2 && yend == y2) || (x1 > x2 && yend == y1) )
			while(y++ < yend)
			{
				if( d<0)
				{
					d += inc1;
				}
				else
				{
					x++;
					d += inc2;
				}
				nSum+= *(FM+x+y*pitch);				
			}
		else
			while(y++ < yend)
			{
				if( d<0) d += inc1;
				else
				{
					x--;
					d += inc2;
				}
				nSum+= *(FM+x+y*pitch);				
			}
	 }
	 return nSum;
 }


int l_ProfileUpper[8192];
int l_ProfileLower[8192];
int l_LineProfile[1024];
int l_LineProfileMV[1024];
BYTE l_fmL2[2048*1024];
BYTE l_fmL4[1024*512];
int GetVerMultiLineMura(LPBYTE fm, int left, int top, int right, int bottom,  int nHeight, int pitch, int nUpperLimit, int nLowerLimit, int *pProfile);
void WEB_MultiVerLine()
{
	int i, j, ii,jj,m;
	int nTmp;
	int pitch=g_System.m_nPitch;
	int nOffset=6;
	int nDiff=10;
	int nLevel;
	int ll,tt,rr,bb;
	double dArea, dMaxArea;
	int nLevelInapect;
	double Cx, Cy;
	int nWidth, nHeight, nInspectX1, nInspectX2;
	int nX1, nY1, nX2, nY2, mm, nn;
	LPBYTE fm, fmD;


	fm=g_fmGrab[g_ID];


	g_Temp.m_nMaxMultiVerLineValue=0;
	
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i] && g_Param.m_nMuitlLineValue[i]>10)
		{
			nLevelInapect=1;
		}
	}
	if(nLevelInapect==0) return;

	nWidth		=g_System.m_nImageW;
	nHeight		=g_System.m_nImageH;
	nInspectX1	=g_Temp.m_nInspectX1;
	nInspectX2	=g_Temp.m_nInspectX2;


	g_Temp.m_nMaxMultiVerLineValue=GetVerMultiLineMura(fm, nInspectX1, 0, nInspectX2, nHeight,  nHeight, pitch, 15, 15, g_Profile);


	//-------------------------------------------------------------------------
	nLevel=GetLevel(g_Temp.m_nMaxMultiVerLineValue, g_Param.m_nMuitlLineValue);
	if(nLevel>0)
	{
		Cx=(nInspectX1+nInspectX2)/2;
		Cy=nHeight/2;
		MakeDefectRect(g_Defect.m_nDefectCount, Cx, Cy, nInspectX1, nInspectX2);
			
		//영상은 1/8 압축영상----------------------------------------------------------
		nX1=Cx/4-128;  if(nX1<0) nX1=0;
		nX2=nX1+128*2; if(nX2>pitch/4) { nX2=pitch/4; nX1=nX2-128*2;}
		nY1=nHeight/8-128;
		nY2=nY1+128*2;

		fmD	=g_Defect.m_pImage[g_Defect.m_nDefectCount];
		mm=0;
		for(i=nY1;i<nY2;i+=2)
		{
			nn=0;
			for(j=nX1;j<nX2;j+=2)
			{
				*(fmD+128*mm+nn)=*(l_fmL4+(pitch/4)*i+j);
				nn++;
			}
			mm++;
		}

		g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=(int)Cx;	 //불량위치 X (Pixel)
		g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=(int)Cy;	 //불량위치 Y (Pixel)
		g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+6;	//
		g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0;			 //Size
		g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =g_Temp.m_nMaxMultiVerLineValue;			 //Value	
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0;			 //Size X
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0;			 //Size Y

		if(CheckDefectOverlap()==0)   							
			g_Defect.m_nDefectCount++;
		
	}
}


//4096x2048을 1024x512로 압축. 위로부터 4등분해서 각각의 Profile을 구하고 Moving Avg와 기존 Profile의 차이값의 합을 구함.
// Projection 과 Projection Moving Avg차이가 0.5미만이면 더하지 않는다.
//평균밝기 80 기준으로 Value재조정한다.
//검사면적으로 재조정한다. (면적이 1/4안되면 SKIP, 1 보다 작으면 1로 맞춤, 즉 면적이 1/2이면 Value에 2를 곱해준다.)
int GetVerMultiLineMura(LPBYTE fm, int left, int top, int right, int bottom,  int nHeight, int pitch, int nUpperLimit, int nLowerLimit, int *pProfile)
{
	int i, j, k, nRet=0;
	int nTmp, nSum;
	int pitchH, pitchQ;
	int nY1, nY2, nX1, nX2;
	long nValue=0, nValueCnt=0;
	int  nMovAvgOffset4=10; //80 Pixel 로 Moving Avg구함.
	int  nMovAvgOffset4H;   
	long nAvgImage=0;
	int nXRange;


	nMovAvgOffset4H=nMovAvgOffset4/2;

	// Spot성 튀는 값 제거하고 1/2압축한다-------------------------------
	for(j=0;j<right;j++)
	{
		l_ProfileUpper[j]=pProfile[j]+nUpperLimit; 
		l_ProfileLower[j]=pProfile[j]-nLowerLimit; 
	}

	//(1/2)압축
	pitchH=pitch/2;
	for(i=top;i<bottom-1;i+=2)
		for(j=left;j<right-1;j+=2)
		{
			nSum=0;
			nTmp=*(fm+pitch*i+j);
			if(nTmp>l_ProfileUpper[j])		nSum+=l_ProfileUpper[j];
			else if(nTmp<l_ProfileLower[j]) nSum+=l_ProfileLower[j];
			else                            nSum+=nTmp;

			
			nTmp=*(fm+pitch*i+j+1);
			if(nTmp>l_ProfileUpper[j+1])	  nSum+=l_ProfileUpper[j+1];
			else if(nTmp<l_ProfileLower[j+1]) nSum+=l_ProfileLower[j+1];
			else                              nSum+=nTmp;

			
			nTmp=*(fm+pitch*(i+1)+j);
			if(nTmp>l_ProfileUpper[j])		nSum+=l_ProfileUpper[j];
			else if(nTmp<l_ProfileLower[j]) nSum+=l_ProfileLower[j];
			else                            nSum+=nTmp;

			
			nTmp=*(fm+pitch*(i+1)+j+1);
			if(nTmp>l_ProfileUpper[j+1])	  nSum+=l_ProfileUpper[j+1];
			else if(nTmp<l_ProfileLower[j+1]) nSum+=l_ProfileLower[j+1];
			else                              nSum+=nTmp;
			*(l_fmL2+pitchH*(i/2)+(j/2))=(nSum/4);
		}
	//-------------------------------------------------------------------

	//(1/4)압축 ---------------------------------------------------------
	pitchQ=pitch/4;
	for(i=top/2;i<bottom/2-1;i+=2)
		for(j=left/2;j<right/2-1;j+=2)
			*(l_fmL4+pitchQ*(i/2)+(j/2))=(*(l_fmL2+pitchH*i+j)+*(l_fmL2+pitchH*i+j+1)+*(l_fmL2+pitchH*(i+1)+j)+*(l_fmL2+pitchH*(i+1)+j+1))/4;	
	//------------------------------------------------------------------
	
	nHeight/=4; //(1/4)압축해서
	nX1=left/4;
	nX2=right/4;
	nXRange=nX2-nX1;
	for(k=0;k<4;k++)
	{
		memset(l_LineProfile, 0, pitchQ*sizeof(int));
		nY1=k*nHeight/4;
		nY2=nY1+nHeight/4;

		for(i=nY1;i<nY2;i++)
			for(j=nX1;j<nX2;j++)
				l_LineProfile[j]+=*(l_fmL4+pitchQ*i+j);

		nSum=0;
		for(j=nX1;j<nX1+nMovAvgOffset4;j++)
			nSum+=l_LineProfile[j];

		for(j=nX1;j<nX2-nMovAvgOffset4;j++)
		{
			l_LineProfileMV[j+nMovAvgOffset4H]=nSum/nMovAvgOffset4;

			nSum=nSum+l_LineProfile[j+nMovAvgOffset4]-l_LineProfile[j];
		}

		for(j=nX1+nMovAvgOffset4H;j<nX2-nMovAvgOffset4H-1;j++)
		{
			nTmp=l_LineProfile[j]-l_LineProfileMV[j];
			if(nTmp<0) nTmp*=-1;
			if(nTmp<nHeight/8) nTmp=0;
			nValue+=nTmp;
			nValueCnt++;
			nAvgImage+=l_LineProfile[j];
		}
	}

	if(nValueCnt>0)
	{
		nAvgImage/=nValueCnt;

		if(nAvgImage>40 && nXRange>=(pitchQ/4))
			nRet=nValue*80/nAvgImage*(pitchQ/nXRange);
	}

	//for(i=top/4;i<bottom/4;i++)
	//	for(j=left/4;j<right/4;j++)
	//		*(fm+pitch*i+j)=*(l_fmL4+pitchQ*i+j);

	return nRet;
}






/*
extern __m256i _mm256_stream_load_si256(__m256i const *);

void fastMemcpy(void *pvDest, void *pvSrc, size_t nBytes) {
  ASSERT(nBytes % 32 == 0);
  ASSERT((intptr_t(pvDest) & 31) == 0);
  ASSERT((intptr_t(pvSrc) & 31) == 0);
  const __m256i *pSrc = reinterpret_cast<const __m256i*>(pvSrc);
  __m256i *pDest = reinterpret_cast<__m256i*>(pvDest);
  int64_t nVects = nBytes / sizeof(*pSrc);
  for (; nVects > 0; nVects--, pSrc++, pDest++) {
    const __m256i loaded = _mm256_stream_load_si256(pSrc);
    _mm256_stream_si256(pDest, loaded);
  }
  _mm_sfence();
}
*/