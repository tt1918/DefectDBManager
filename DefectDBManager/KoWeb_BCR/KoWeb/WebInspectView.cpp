#include "Stdafx.h"
#include "KoWebDoc.h"
#include "KoWebView.h"
#include "math.h"


void CKoWebView::WebInspect()
{
	int i,j, nPosY;
	int nDefect, nLevel;
	int nJumpping=0;
	
	g_Temp.m_nErrorCode=0;

	//각종검사 Reset --------------------------------------------------
	g_Temp.m_nInspectWhite	=0;
	g_Temp.m_nInspectBlack	=0;
	g_Temp.m_nScratchInspect=0;
	g_Temp.m_nLongScratchInspect=0;
	g_Temp.m_nLRInspect		=0;
	g_Temp.m_nFlatSuccess	=0;
	g_Temp.m_nNG_MulTae		=0;	//물때 불량
	g_Temp.m_nCunicValue	=0;
	g_Temp.m_nMaxLRValue	=0;
	g_Temp.m_nMaxLRWValue	=0;
	g_Temp.m_nMaxLineValue	=0;
	g_Temp.m_nMaxAngleLineValue=0;
	g_Temp.m_nMaxWLineValue	=0;
	g_Temp.m_nMaxBLineValue	=0;
	//----------------------------------------------------------------



	for(i=1;i<10;i++) l_dTime[i]=l_dTime[0];

	//////////////////////////////////////////////////////
	// Defect수 Reset - S
	g_Defect.m_nDefectCount=0;	

#ifdef BARCODE_VISION
	g_Defect.m_nBcrAreaDefectCount = 0;
	g_Defect.m_nBcrCount = 0;
	g_Defect.m_nBcrDefectCount = 0;
#endif
	// Defect수 Reset - E
	//////////////////////////////////////////////////////

#ifdef MY_RECV
	if(g_Temp.m_nPCOptic==RECV_OPTIC && g_Kipo.m_nKipoPress!=0)  //받는 PC (COS7)
		KO_UpdateKipoPress(g_Temp.m_nGrabFrameSub);  
#endif

	//LR, 투영의 경우 영상왜곡 폄----------------------------------------------------------
	if(g_System.m_nPerspectiveTR && g_Temp.m_nMakeLUT) 
		ThreadPerspectiveTR();

	if(g_Param.m_nBrightCompensation>=2 && g_Param.m_nBrightCompensation<=4)
		BrightCompensation(g_fmGrab[g_ID], g_fmTemp, g_System.m_nImageW, g_System.m_nImageH, g_System.m_nPitch, g_Param.m_nBrightCompensation);


	//Profile구하고, 영역구하고, 평균밝기/ Sharpness구함-----------------------------------
	l_dTime[1]=Clock_set();

	GetImageProfile();	


	//자동밝기 - Exposure제어
#ifdef USE_ROTATION
	if(g_Temp.m_nMotionCheck==0 && (g_Param.m_nSetAutoExposure || g_Temp.m_nAutoAdjustStart || g_Temp.m_nAutoExpoStart)) 
		AutoExposure(g_System.m_nLineCAM, g_Temp.m_nGrabFrameID, g_Temp.m_dAverageBrightForExpo);
#else
	if(g_Param.m_nSetAutoExposure || g_Temp.m_nAutoAdjustStart || g_Temp.m_nAutoExpoStart) 
		AutoExposure(g_System.m_nLineCAM, g_Temp.m_nGrabFrameID, g_Temp.m_dAverageBrightForExpo);
#endif

	if(g_System.m_nMakePyramid)
		ThreadMakeCompression(g_fmGrab[g_ID], g_fmGrabComp);	//영상 압축 : THREAD

	l_dTime[2]=Clock_set();

#if INSPECT_TYPE==TAC_LINE
	RemoveHorlineNoise(g_fmGrab[g_ID], g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, g_System.m_nImageH, g_System.m_nImageW, 20);
	nJumpping=CheckTACJumpping();
#endif

	if(g_Temp.m_dAverageBright<g_Param.m_nBrightMin || g_Temp.m_dAverageBright>g_Param.m_nBrightMax )
	{
		g_Temp.m_nErrorCode=ERROR_BRIGHTNESS; //	[1]	내부 밝기가 범위 벗어남.
		
		if(m_bShowImage) //1msec 이하
		{
			m_pImage->PutImageData(g_fmGrab[g_ID], g_System.m_nImageW, g_System.m_nImageH,1);
			m_pImage->UpdateImage();
			m_pImage->SetAddGraphic(true);
		}
		g_Temp.m_nMapLineCnt=g_Temp.m_nGrabFrame;
		g_Temp.m_GrabNextDelay=0; //USE_HOLE_SKIP 에서만 사용함.
		return;
	}


	//평활화--------------------------------------------------------------------------------
	if(g_System.m_nFlatBright>0)	MakeImageFlat( 0, g_System.m_nImageW); //평활화 (LineScan, Area(Cross, 투영, LR)) 기존이미지보관, 평활화하면 g_fmFlat[g_ID]에 들어감.
	else							memcpy(g_fmFlat[g_ID], g_fmGrab[g_ID], g_System.m_nImageW*g_System.m_nImageH);


	l_dTime[2]=Clock_set();

#ifdef USE_HOLE_SKIP
	HoleRemoveProcess();
#endif

	l_dTime[3]=Clock_set();
	g_Temp.m_nMakePyramidDone=0;


	//해당광학계 검사-----------------
// 2동 3D 라인 에지경계에서 Knulling구분===================
#if INSPECT_TYPE==TAC_LINE
	g_Temp.m_nKnullingCheck=0;
	if(g_Temp.m_nPCOptic==COS6 || g_Temp.m_nPCOptic==COS8)
	{
		if(g_Param.m_nKnullingPC && g_Temp.m_nGrabFrame%2)
		{
			g_Temp.m_nKnullingCheck=1;
			CheckKnulling();
		}
	}
#endif
//========================================================

#ifdef USE_ROTATION
	if(g_Param.m_nNoInspect || g_Temp.m_nMotionCheck || g_Temp.m_nLeading)	//No검사거나 모션회전중이면 검사안함.
#else
	if(g_Param.m_nNoInspect || g_Temp.m_nLeading)
#endif
	{
		if(m_bShowImage) //1msec 이하
		{
			m_pImage->PutImageData(g_fmGrab[g_ID], g_System.m_nImageW, g_System.m_nImageH,1);
			m_pImage->UpdateImage();
			m_pImage->SetAddGraphic(true);
		}
		g_Temp.m_nMapLineCnt=g_Temp.m_nGrabFrame;
		return;
	}

	if(nJumpping==0) //점핑이면 그다음 검사하지 않음. TAC_LINE에서만 nJumpping 사용하고 그외는 nJumpping 이 항상 0 임.
	{
		switch(g_Temp.m_nPCOptic)
		{
			case COS1: Inspect1(); break;
			case COS2: Inspect2(); break;
			case COS3: Inspect3(); break;
			case COS4: Inspect4(); break;
			case COS5: Inspect5(); break;
			case COS6: Inspect6(); break;
			case COS7: Inspect7(); break;
			case COS8: Inspect8(); break;
			case COS9: Inspect9(); break;
			case COSA: InspectA(); break;
			case COSB: InspectB(); break;
			case COSC: InspectC(); break;   
			case COSD: InspectD(); break;   
			case CBCR: InspectBCR(); break;				//BCR 바코드
			case CMAK: InspectMarkingVision(); break;	//마킹비전
		}
	}


//셔터를 바꾸면 시간이 걸리면서 불량이 됨.(영상 짤림), 따라서 불량을 없앤다.------------------------------------------------------
	if(g_Temp.m_nChangeExpoFrameID>0 && g_Temp.m_nChangeExpoFrameID==g_Temp.m_nGrabFrameID)
	{
		g_Temp.m_nChangeExpoFrameID=-1;

		if(CheckDiffTopBottom(g_fmGrab[g_ID], g_System.m_nPitch, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, g_System.m_nImageH, &nPosY)>3)  //Frame 위/아래 밝기차가 3이상 나면 여기서 검추한 불량은 SKIP한다.
		{
			RemoveDefectByYPos(nPosY, 64);
		}
	}
//--------------------------------------------------------------------------------------------------------------------------------
//	RemoveDefectByHorNoize(2, 20);  수평노이즈 제거함.2는 흑/백 모두, 20은 라인의 평균밝기가 평균보다 차이나면

	
	//직전 Frame평활화 한것 저장
	memcpy(g_fmPrev, g_fmFlat[g_ID]+g_System.m_nPitch*(g_System.m_nImageH-32), g_System.m_nPitch*32);

	CheckPeriod(); //주기체크

	if(g_Defect.m_nDefectCount>0) g_Temp.m_nLatestResult=1;
	else						  g_Temp.m_nLatestResult=0;

}

//자동밝기 - Exposure제어
void CKoWebView::AutoExposure(int nLineCAM, int nGrabFrame, double dBright)
{
	int nBrightDiff, nTmp;
	double dTmp;
	static int nExposureDirection=1;
	static int nCurrExpo=0;				//자동밝기 시작시 셔터값
	static double dBrightRatio=1;
	static int n1StepValue=10;
	static int nExposure=100;
	static int nAutoExpoCount=0;
	static int nAddExposure=0;
	static int nAddSumExposure=0;


	//현재 밝기를 기준밝기와 비교해서 일정값 차이가 나야만 밝기조절을 수행한다>>>>>>>>
	if(g_Temp.m_nAutoExpoStart==0) //현재 자동밝기 조정중이 아닐때만 Check
	{
		dTmp=dBright-g_System.m_nGrabBright; if(dTmp<0) dTmp*=-1;

		if(dTmp>g_System.m_nStartGrayDiff)	nAutoExpoCount++;
		else								nAutoExpoCount=0;

		if(dTmp<=g_System.m_nTargetGrayDiff)   g_Temp.m_nAutoAdjustStart=0; //밝기차가 1이하면 다시 조정할 필요 없음.

		if(nAutoExpoCount>=g_System.m_nAutoExpoStartCnt || g_Temp.m_nAutoAdjustStart)
		{
			g_Temp.m_nAutoExpoStart=1;
			nAutoExpoCount=nAddExposure  =0;
		}
	}
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	if(g_Temp.m_nAutoExpoStart)
	{
		if(nGrabFrame%3 != 0) return; //Exposure 적용하고 다음 Frame에 반영되므로 (반드시 3, 2 같지만 셔터 변경하고 시간이 걸리므로 3번째 Frame에 영향을 받음)

		if(nLineCAM) //----------------------------------------------------------------------------------------------------
		{
			if(g_Temp.m_nAutoExpoStart==1)
			{
				nBrightDiff=dTmp=dBright-g_System.m_nGrabBright;
				nAddExposure=abs(g_Param.m_nExposure*nBrightDiff/g_System.m_nGrabBright);
				if(nAddExposure>g_Param.m_nExposure/10) nAddExposure=g_Param.m_nExposure/10;		//파라미터 셔터의 10%
					
				//밝기차가 5이하이면 최소값으로 바꾼다----------------
				if(dTmp<0) dTmp*=-1;
				if(dTmp<=5) nAddExposure=g_System.m_nAutoExpoMinValue;						
				//----------------------------------------------------

				nAddSumExposure=0;
				nCurrExpo=g_Temp.m_nExposureData;
				if(nBrightDiff>0) nExposureDirection=-1;  //현재가 더 밝으면 Exposure를 줄여야 함.
				else              nExposureDirection= 1;
			}

			if(dBright>g_System.m_nGrabBright+g_System.m_nTargetGrayDiff) //현재밝기가 크면 설정된 Exposure를 낮춘다.
			{
				if(nExposureDirection<0) nAddSumExposure-=nAddExposure;
				else                     
				{
					nExposureDirection=-1;
					nAddExposure/=2; if(nAddExposure<g_System.m_nAutoExpoMinValue) nAddExposure=g_System.m_nAutoExpoMinValue;
					nAddSumExposure-=nAddExposure;
				}	

				if(nCurrExpo+nAddSumExposure<g_System.m_nMinExposure)		nAddSumExposure=g_System.m_nMinExposure-nCurrExpo;
				else if(nCurrExpo+nAddSumExposure>g_System.m_nMaxExposure)  nAddSumExposure=g_System.m_nMaxExposure-nCurrExpo;

				g_Grabber.SetExposureTime( (nCurrExpo+nAddSumExposure), 0);
			}
			else if(dBright<g_System.m_nGrabBright-g_System.m_nTargetGrayDiff)
			{
				if(nExposureDirection>0) nAddSumExposure+=nAddExposure;
				else                     
				{
					nExposureDirection=1;
					nAddExposure/=2; if(nAddExposure<g_System.m_nAutoExpoMinValue) nAddExposure=g_System.m_nAutoExpoMinValue;
					nAddSumExposure+=nAddExposure;
				}	

				if(nCurrExpo+nAddSumExposure<g_System.m_nMinExposure)		nAddSumExposure=g_System.m_nMinExposure-nCurrExpo;
				else if(nCurrExpo+nAddSumExposure>g_System.m_nMaxExposure)  nAddSumExposure=g_System.m_nMaxExposure-nCurrExpo;

				g_Grabber.SetExposureTime( (nCurrExpo+nAddSumExposure), 0);
			}
			else 
			{
				g_Temp.m_nAutoExpoStart=g_Temp.m_nAutoAdjustStart=0;	//밝기가 맞아서 끝
				return;
			}	
		}
		else //=====================================================================================================================
		{
			if(g_Temp.m_nAutoExpoStart==1) //자동밝기시작하고 맨 처음 한번만 들어온다.
			{
				nBrightDiff=dBright-g_System.m_nGrabBright;

				nExposure=g_Temp.m_nExposureData; //시작 밝기(현재 Exposure)
				
				if( nBrightDiff>0  ) { nExposureDirection=-1; dBrightRatio=0.5;} //어둡게 하는 방향
				else			     { nExposureDirection= 1; dBrightRatio=1;  } //밝게 하는 방향 

				if     (nBrightDiff> 50)  n1StepValue=nExposure*0.5;
				else if(nBrightDiff> 40)  n1StepValue=nExposure*0.4;
				else if(nBrightDiff> 30)  n1StepValue=nExposure*0.3;
				else if(nBrightDiff> 20)  n1StepValue=nExposure*0.2;
				else if(nBrightDiff> 10)  n1StepValue=nExposure*0.1;
				else if(nBrightDiff<-40)  n1StepValue=nExposure*0.4;
				else if(nBrightDiff<-30)  n1StepValue=nExposure*0.3;
				else if(nBrightDiff<-20)  n1StepValue=nExposure*0.2;
				else if(nBrightDiff<-10)  n1StepValue=nExposure*0.1;
				else //(-10~10)은 작은 값으로 적용함.
				{
					if(nExposure*0.01<10)  n1StepValue=nExposure*0.01;
					else				   n1StepValue=10;				  
				}

				if(n1StepValue<g_System.m_nAutoExpoMinValue) n1StepValue=g_System.m_nAutoExpoMinValue;
			}

			if(dBright>g_System.m_nGrabBright+g_System.m_nTargetGrayDiff) 
			{
				if(nExposureDirection<0) //계속 어둡게 하는 중이면 같은 크기로 밝기를 줄인다.
				{
					nTmp=n1StepValue*dBrightRatio; if(nTmp<g_System.m_nAutoExpoMinValue) nTmp=g_System.m_nAutoExpoMinValue;
					nExposure-=nTmp;
				}
				else //밝게하는 중에 범위를 넘어가면 
				{
					nExposureDirection=-1; //어둡게 하는 방향으로 바꾼다.
					dBrightRatio*=0.5;     //변화하는 양을 반으로 줄인다.
					nTmp=n1StepValue*dBrightRatio; if(nTmp<g_System.m_nAutoExpoMinValue) nTmp=g_System.m_nAutoExpoMinValue;
					nExposure-=nTmp;
				}
				if(nExposure<g_System.m_nMinExposure) nExposure=g_System.m_nMinExposure;
			}
			else if(dBright<g_System.m_nGrabBright-g_System.m_nTargetGrayDiff)
			{
				if(nExposureDirection>1) //계속 밝게하는 중이면 같은 크기로 밝게 한다.
				{
					nTmp=n1StepValue*dBrightRatio; if(nTmp<g_System.m_nAutoExpoMinValue) nTmp=g_System.m_nAutoExpoMinValue;
					nExposure+=nTmp;
				}
				else
				{
					nExposureDirection=1; //밝게하는 방향으로 바꾼다.
					dBrightRatio*=0.5;    //변화하는 양을 반으로 줄인다.
					nTmp=n1StepValue*dBrightRatio; if(nTmp<g_System.m_nAutoExpoMinValue) nTmp=g_System.m_nAutoExpoMinValue;
					nExposure+=nTmp;
				}
				if(nExposure>g_System.m_nMaxExposure) nExposure=g_System.m_nMaxExposure;
			}
			else
			{
				g_Temp.m_nAutoExpoStart=g_Temp.m_nAutoAdjustStart=0; //잘맞아서 끝
				return;
			}

				
			g_Grabber.SetExposureTime(nExposure, 0);    //자동밝기 조정 
		}

		g_Temp.m_nAutoExpoStart++;
		if(g_Temp.m_nAutoExpoStart>g_System.m_nAutoExpoEndCnt) //자동밝기 최대시도 횟수
		{
			g_Temp.m_nAutoExpoStart=g_Temp.m_nAutoAdjustStart=0;  //횟수가 오버되서 끝
		}
	}	
}

int CheckPeriodSub(int nX, int nY, int nFrame, int *pPeriod);
void CKoWebView::CheckPeriod()
{
	int i;
	int nPeriod;
	int nLevel;
	int nDefectClass;

	for(i=0;i<g_Defect.m_nDefectCount;i++)
	{

#ifdef USE_DAKANO_LINE
		if(g_Defect.m_Defect[i].defect_class%10==4) continue;
#endif

		if(CheckPeriodSub(g_Defect.m_Defect[i].x_pos, g_Defect.m_Defect[i].y_pos, g_Defect.m_nFrameNum, &nPeriod))
		{
			nLevel=g_Defect.m_Defect[i].defect_class/MILLION;			//2
			if(g_Param.m_nPeriodLevel>0) nLevel=g_Param.m_nPeriodLevel;

			nDefectClass=g_Defect.m_Defect[i].defect_class%OPTICDEV;	//92
			g_Defect.m_Defect[i].defect_class=nLevel*MILLION+nDefectClass+OPTICDEV*nPeriod;  //nPeriod=120이면 => 2012092
		}
	}
}

#ifdef TOT_USE


//현재 불량에서 X가 같은 불량이 뒤 8주기 동안 3개이상 있으면 주기로 판정함.
//즉, (nX, nY)를 주기로 판정하기위해서 이전 8개 주기영역을 체크해서 X 일치하고
// 주기가 되는 불량 3개 더 있으면 주기로 판정함. 즉,자신제외하고 총 8개 위치체크
int CheckPeriodSub(int nX, int nY, int nFrame, int *pPeriod)
{
	int nPeriod=0;
	static int nOldFrame=-1;
	int i, j, k;
	double dX, dY;
	double dOffsetX, dOffsetY;
	int nOverCount, nPeriodCount;
	int nPx, nPx_=-1, nOffset,nMinOffset, nMinPos;
	double dOverOffset=0, dYEnd, dMaxPeriod=0;
	double dLimitX=g_System.m_dPeriodLimitX;
	double dLimitY=g_System.m_dPeriodLimitY;
	double dCycleLength[MAX_CYCLE_COUNT];

	*pPeriod=0;
	if(nFrame<1 || nY<0 || nY>=g_System.m_nImageH | nX<0 || nX>=g_System.m_nImageW) return 0; //데이타 불량


	dX=nX*g_Param.m_dScaleFactorX;
	if(g_System.m_nLineCAM==0)  //Area 카메라
	{
		nOverCount =nY/g_System.m_nImagePartH;
		if(g_System.m_nImagePart>1) dOverOffset=(g_System.m_nImageH*g_Param.m_dScaleFactorY-g_Param.m_dFrameLen)/(g_System.m_nImagePart-1);
		
		dY=nFrame*g_Param.m_dFrameLen+nY*g_Param.m_dScaleFactorY-nOverCount*dOverOffset; 
	}
	else //Line Scan 카메라
	{
		dY=nFrame*g_Param.m_dFrameLen+nY*g_Param.m_dScaleFactorY;
	}
	if(dY<3000) return 0; //최초 3M는 주기 찾지 않는다.
	

	for(k=0;k<MAX_CYCLE_COUNT;k++)
	{
		dCycleLength[k]=g_ParamVer.nCycle[k]*3.141592;
		if(dCycleLength[k]>dMaxPeriod) 
			dMaxPeriod=dCycleLength[k];
	}

	dYEnd=nFrame*g_Param.m_dFrameLen-dMaxPeriod*8; //현재위치보다 8주기 이상 제외


	//현재Frame 시작단보다 8주기 뺸 Y보다 작은 Y는 모두 없앤다.---------
	if(nFrame!=nOldFrame)
	{
		nOffset=0;
		for(i=g_Temp.m_nCycleCnt-1;i>=0;i--)
			if(g_Temp.m_dCycleY[i]<dYEnd)
			{
				nOffset=i+1;
				break;
			}
		if(nOffset>0)
		{
			for(j=nOffset;j<g_Temp.m_nCycleCnt;j++)
			{
				g_Temp.m_dCycleX[j-nOffset]=g_Temp.m_dCycleX[j];
				g_Temp.m_dCycleY[j-nOffset]=g_Temp.m_dCycleY[j];
			}
			g_Temp.m_nCycleCnt-=nOffset;
		}
		nOldFrame=nFrame;
	}
	//----------------------------------------------------------------------
	
	if(g_Temp.m_nCycleCnt>=3) //적어도 체크해 볼 불량이 3개 이상은 있어야 한다.
	{
		for(k=0;k<MAX_CYCLE_COUNT;k++)
		{
			if(dCycleLength[k]<10) continue;

			dMaxPeriod=dY-dCycleLength[k]*8-8*dLimitY; //주기 8개 길이
		
			nPeriodCount=0;
			for(i=g_Temp.m_nCycleCnt-1;i>=0;i--)   //Cycle[0]부터 우선순위로 검출
			{
				if(g_Temp.m_dCycleY[i]<dMaxPeriod) break; //해당 구간 끝
			
				dOffsetX=dX-g_Temp.m_dCycleX[i]; if(dOffsetX<0) dOffsetX*=-1;				

				if(dOffsetX<dLimitX) //X가 같아야 한다.----------------------------
				{
					for(j=0;j<8;j++)
					{
						dOffsetY=dY-g_Temp.m_dCycleY[i]-dCycleLength[k]*(j+1); if(dOffsetY<0) dOffsetY*=-1;
						
						if(dOffsetY<dLimitY*(j+1)) //Y가 범위내 들어야 한다---------
						{
							nPeriodCount++;
							break;
						}
					}
				}
			}

			if(nPeriodCount>=3)
			{
				nPeriod=1;
				*pPeriod=g_ParamVer.nCycle[k];
				break;
			}
		}
	}

	if(g_Temp.m_nCycleCnt<CYCLELENGTH)
	{
		g_Temp.m_dCycleX[g_Temp.m_nCycleCnt]=dX;
		g_Temp.m_dCycleY[g_Temp.m_nCycleCnt]=dY;
		g_Temp.m_nCycleCnt++;
	}

	return nPeriod;
}

#else

int CheckPeriodSub(int nX, int nY, int nFrame, int *pPeriod)
{
	int nPeriod=0;
	int i, j, k;
	double dX, dY;
	double dOffsetX, dOffsetY;
	int nOverCount;
	int nPx, nPx_=-1, nOffset,nMinOffset, nMinPos;
	double dOverOffset=0, dYEnd;
	double dLimitX=g_System.m_dPeriodLimitX;
	double dLimitY=g_System.m_dPeriodLimitY;
	double dCycleLength[MAX_CYCLE_COUNT];

	*pPeriod=0;
	if(nFrame<1 || nY<0 || nY>=g_System.m_nImageH | nX<0 || nX>=g_System.m_nImageW) return 0; //데이타 불량

	nPx=(int)(nX/(g_System.m_nImageW/10.0));

	//10등분 9개 경계중 어디와 가까운지....------------------
	nMinOffset=g_System.m_nImageW;
	for(i=0;i<9;i++) //10등분 9개 경계비교
	{
		nOffset=nX-(i+1)*g_System.m_nImageW/10;
		if(nOffset<0) nOffset*=-1;
		if(nOffset<nMinOffset)
		{
			nMinOffset=nOffset;
			nMinPos=i;
		}
	}
	if(nMinOffset*g_Param.m_dScaleFactorX<dLimitX)
	{
		if(nX> (int)(nPx*(g_System.m_nImageW/10)))
			nPx_=nPx-1;
		else
			nPx_=nPx+1;

		if(nPx_<0 || nPx_>=10) nPx_=-1;
	}
	//--------------------------------------------------------


	dX=nX*g_Param.m_dScaleFactorX;
	if(g_System.m_nLineCAM==0)  //Area 카메라
	{
		nOverCount =nY/g_System.m_nImagePartH;
		if(g_System.m_nImagePart>1) dOverOffset=(g_System.m_nImageH*g_Param.m_dScaleFactorY-g_Param.m_dFrameLen)/(g_System.m_nImagePart-1);
		
		dY=nFrame*g_Param.m_dFrameLen+nY*g_Param.m_dScaleFactorY-nOverCount*dOverOffset; 
	}
	else //Line Scan 카메라
	{
		dY=nFrame*g_Param.m_dFrameLen+nY*g_Param.m_dScaleFactorY;
	}
	if(dY<3000) return 0; //최초 3M는 주기 찾지 않는다.

	dYEnd=nFrame*g_Param.m_dFrameLen-15000; //현재위치보다 15M 전위치

	for(k=0;k<MAX_CYCLE_COUNT;k++)
		dCycleLength[k]=g_ParamVer.nCycle[k]*3.141592;


	//현재Frame 시작단보다 15M 작은 Y는 모두 없앤다.---------
	nOffset=0;
	for(i=g_Temp.m_nCycleCnt-1;i>=0;i--)
		if(g_Temp.m_dCycleY[i]<dYEnd)
		{
			nOffset=i+1;
			break;
		}
	if(nOffset>0)
	{
		for(j=nOffset;j<g_Temp.m_nCycleCnt;j++)
		{
			g_Temp.m_dCycleX[j-nOffset]=g_Temp.m_dCycleX[j];
			g_Temp.m_dCycleY[j-nOffset]=g_Temp.m_dCycleY[j];
		}
		g_Temp.m_nCycleCnt-=nOffset;
	}
	//-------------------------------------------------------


	for(i=0;i<g_Temp.m_nCycleCnt;i++)   //Cycle[0]부터 우선순위로 검출
	{
		dOffsetX=dX-g_Temp.m_dCycleX[i];
		if(dOffsetX<0) dOffsetX*=-1;
		if(dOffsetX<dLimitX)			//일단 X가 같으면
		{
			for(k=MAX_CYCLE_COUNT-1;k>=0;k--)
			{
				dOffsetY=dY-g_Temp.m_dCycleY[i]-dCycleLength[k];
				if(dOffsetY<0) dOffsetY*=-1;

				if(dOffsetY<dLimitY) //Y가 15개중 주기가 일치하면
				{					
					g_Temp.m_nDefectFlag[nPx][k]++;
					if(nPx_>0) g_Temp.m_nDefectFlag[nPx_][k]++;

					if(dY-g_Temp.m_dCyclePrevY[k]>dCycleLength[k]*8) //직전에 Cycle이었던 점이 8 Cycle거리 초과면 다시 시작
					{
						g_Temp.m_nDefectFlag[nPx][k]=1;
						if(nPx_>0) g_Temp.m_nDefectFlag[nPx_][k]=1;
					}

					if(g_Temp.m_nDefectFlag[nPx][k]>=3 || (nPx_>0 && g_Temp.m_nDefectFlag[nPx_][k]>=3) )  //3세트 즉 4개부터 주기
					{
						nPeriod=1;
						*pPeriod=g_ParamVer.nCycle[k];
					}
					g_Temp.m_dCyclePrevY[k]=dY;
				}
			}
		}
	}

	if(g_Temp.m_nCycleCnt<CYCLELENGTH)
	{
		g_Temp.m_dCycleX[g_Temp.m_nCycleCnt]=dX;
		g_Temp.m_dCycleY[g_Temp.m_nCycleCnt]=dY;
		g_Temp.m_nCycleCnt++;
	}

	return nPeriod;
}
#endif

//THREAD_Inspect()-ShowResults()에서 Call
void CKoWebView::DrawResults()
{
	int i, j;
	int nCol, nRow;
	int ll,tt,rr,bb;
	nCol=g_System.m_nImageW/g_System.m_nCandiRect; if(g_System.m_nImageW-nCol*g_System.m_nCandiRect) nCol++;
	nRow=g_System.m_nImageH/g_System.m_nCandiRect; if(g_System.m_nImageH-nRow*g_System.m_nCandiRect) nRow++;

	if(m_bShowImage)
	{
		//검사 분할 영역 보여주기-----------------------------------------------------------------------------------------------------
		if(m_bShowCandiArea)
		{
			if(g_System.m_nFlatBright<118 || g_System.m_nFlatBright>138)
			{
				for(i=0;i<nCol;i++)
					m_pImage->DrawLine(NULL,i*g_System.m_nCandiRect, 0, i*g_System.m_nCandiRect, g_System.m_nImageH-1, RGB(128,128,128));
				m_pImage->DrawLine(NULL,i*g_System.m_nCandiRect-1, 0, i*g_System.m_nCandiRect-1, g_System.m_nImageH-1, RGB(128,128,128));

				for(i=0;i<nRow;i++)
					m_pImage->DrawLine(NULL,0, i*g_System.m_nCandiRect, g_System.m_nImageW-4, i*g_System.m_nCandiRect, RGB(128,128,128));
				m_pImage->DrawLine(NULL,0, i*g_System.m_nCandiRect-1, g_System.m_nImageW-4, i*g_System.m_nCandiRect-1, RGB(128,128,128));
			}
			else
			{
				for(i=0;i<nCol;i++)
					m_pImage->DrawLine(NULL,i*g_System.m_nCandiRect, 0, i*g_System.m_nCandiRect, g_System.m_nImageH-1, RGB(108,108,108));
				m_pImage->DrawLine(NULL,i*g_System.m_nCandiRect-1, 0, i*g_System.m_nCandiRect-1, g_System.m_nImageH-1, RGB(108,108,108));

				for(i=0;i<nRow;i++)
					m_pImage->DrawLine(NULL,0, i*g_System.m_nCandiRect, g_System.m_nImageW-4, i*g_System.m_nCandiRect, RGB(108,108,108));
				m_pImage->DrawLine(NULL,0, i*g_System.m_nCandiRect-1, g_System.m_nImageW-4, i*g_System.m_nCandiRect-1, RGB(108,108,108));

			}
		}	

		//후보 모두 보여주기-----------------------------------------------------------------------------------------------------------------------------------------------
		if(m_bShowAllCandi)
		{
			for(i=0;i<nCol*nRow;i++)
			{
				if(g_CandiW.m_nCandiValue[i]-g_CandiW.m_nCandiAvg[i]>=7 || g_CandiW.m_nCandiValue[i]-g_CandiW.m_nCandiAvg[i]>=g_Param.m_nAreaThUp)
														m_pImage->DrawCross(NULL,g_CandiW.m_nCandiX[i], g_CandiW.m_nCandiY[i], 2, RGB(255,0,0));		//RED: 백
				if(g_CandiB.m_nCandiAvg[i]-g_CandiB.m_nCandiValue[i]>=7 || g_CandiB.m_nCandiAvg[i]-g_CandiB.m_nCandiValue[i]>=g_Param.m_nAreaThDn)
														m_pImage->DrawCross(NULL,g_CandiB.m_nCandiX[i], g_CandiB.m_nCandiY[i], 2, RGB(0,0,255));		//BLUE:흑

#ifdef FIND_TINYDEFECT
				if(g_CandiTinyB.m_nCandiValue[i]>=7 || g_CandiTinyB.m_nCandiValue[i]>=g_Param.m_nAreaThDn)
														m_pImage->DrawCross(NULL,g_CandiTinyB.m_nCandiX[i], g_CandiTinyB.m_nCandiY[i], 2, RGB(0,0,255));	//BLUE:흑
#endif	

				if(g_CandiArea.m_nCandiValue[i]>=2)		m_pImage->DrawCross(NULL,g_CandiArea.m_nCandiX[i], g_CandiArea.m_nCandiY[i], 2, RGB(255,255,0)); //YELLOW: 노랑
			}
		}	

		//유력후보 모두 보여주기------------------------------------------------------------------------------------------------------------------------------------------
		if(m_bShowValidCandi)
		{
			for(i=0;i<g_System.m_nMaxDefect;i++)
			{
				if(g_CandiW.m_nCandiValue[i]-g_CandiW.m_nCandiAvg[i]>=10) 
					  m_pImage->DrawCross(NULL,g_CandiW.m_nCandiX[i], g_CandiW.m_nCandiY[i], 5, RGB(255,0,0)); //백 후보 + 표시 

				if(g_CandiB.m_nCandiAvg[i]-g_CandiB.m_nCandiValue[i]>=10)
					  m_pImage->DrawCross(NULL,g_CandiB.m_nCandiX[i], g_CandiB.m_nCandiY[i], 5, RGB(0,0,255));

				if(g_CandiArea.m_nCandiValue[i]>=3)
					  m_pImage->DrawCross(NULL,g_CandiArea.m_nCandiX[i], g_CandiArea.m_nCandiY[i], 5, RGB(255,255,0));
			}
		}

#ifndef MARKING_VISION
		for(i=0;i<g_Defect.m_nDefectCount;i++)
			DrawDefectRect(i); 
#endif

#ifdef MY_RECV
		if(g_Temp.m_nPCOptic==RECV_OPTIC)
		{
			for(i=0;i<g_Kipo.m_nCurRectCount;i++)
			{
				ll=g_Kipo.m_nCurRect[i].left;	if(ll<0) ll=0;
				tt=g_Kipo.m_nCurRect[i].top;	if(tt<0) tt=0;
				rr=g_Kipo.m_nCurRect[i].right;	if(rr>=g_System.m_nImageW) rr=g_System.m_nImageW-1;
				bb=g_Kipo.m_nCurRect[i].bottom; if(bb>=g_System.m_nImageH) bb=g_System.m_nImageH-1;
				if(rr-ll>0 && bb-tt>0)
					m_pImage->DrawRectangle(NULL, ll,tt,rr,bb, RGB(0,0,255));
			}
		}
#endif
	}

	if(g_Config.m_nShowProfile)		ShowProfile();
	if(g_Config.m_nShowCalibWidth)  ShowCalibWidth();


	if(g_Param.m_nNotInspArea && m_bShowImage)  ShowNotInspectLine();
	
	//불량 작은 이미지 보이는 것------------------------------------------------------------------
	m_nShowNGID=0;
	if(g_Defect.m_nDefectCount>0)
	{
		m_pNGImage[0]->PutImageData(g_Defect.m_pImage[m_nShowNGID],BAD_IMG_WIDTH,BAD_IMG_HEIGHT,1);	
		ShowDefectInfo(0);
	}

	//Debug용------------------------------------------------
	if(g_Temp.m_nFrameDiff==2) g_Temp.m_nFrameDiffSum[0]++;
	if(g_Temp.m_nFrameDiff==3) g_Temp.m_nFrameDiffSum[1]++;


	//각종 정보 쓰는 부분-------------------------------------------------------------------------
	CString str;

	if(g_Temp.m_bShowOldMap) return;

	m_nResultStringCnt=0;					//m_ctrlResult.ResetContent();

#ifdef USE_SK_BAT
	if(g_Temp.m_nAutoExpoStart)
	{
		if(g_Param.m_nAutoEdgeFind) str.Format(_T("Inspect Area : %d - %d (Auto Edge(In)=%.1lf, Edge(Out)=%.1lf) Shutter:%d (Auto:%d)"), g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, g_Temp.m_dInnerEdge, g_Temp.m_dOuterEdge, g_Temp.m_nExposureData, g_Temp.m_nAutoExpoStart);
		else						str.Format(_T("Inspect Area : %d - %d (Not Auto)  Shutter:%d (Auto:%d)"), g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, g_Temp.m_nExposureData, g_Temp.m_nAutoExpoStart);
	}
	else
	{
		if(g_Param.m_nAutoEdgeFind) str.Format(_T("Inspect Area : %d - %d (Auto Edge(In)=%.1lf, Edge(Out)=%.1lf) Shutter:%d"), g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, g_Temp.m_dInnerEdge, g_Temp.m_dOuterEdge, g_Temp.m_nExposureData);
		else						str.Format(_T("Inspect Area : %d - %d (Not Auto)  Shutter:%d"), g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, g_Temp.m_nExposureData);
	}
#else
	if(g_Temp.m_nAutoExpoStart)
	{
		if(g_Param.m_nAutoEdgeFind) str.Format(_T("Inspect Area : %d - %d (Auto Edge=%d) Shutter:%d (Auto:%d)"), g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, g_Temp.m_nFoundEdge, g_Temp.m_nExposureData, g_Temp.m_nAutoExpoStart);
		else						str.Format(_T("Inspect Area : %d - %d (Not Auto)  Shutter:%d (Auto:%d)"), g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, g_Temp.m_nExposureData, g_Temp.m_nAutoExpoStart);
	}
	else
	{
		if(g_Param.m_nAutoEdgeFind) str.Format(_T("Inspect Area : %d - %d (Auto Edge=%d) Shutter:%d"), g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, g_Temp.m_nFoundEdge, g_Temp.m_nExposureData);
		else						str.Format(_T("Inspect Area : %d - %d (Not Auto)  Shutter:%d"), g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, g_Temp.m_nExposureData);
	}
#endif

	m_sResult[m_nResultStringCnt++]=str;		//m_ctrlResult.AddString(str);

#if INSPECT_TYPE==TAC_LINE
	if(g_Temp.m_nKnullingCheck)
	{
//		if(g_Temp.m_nFoundKnulling) str.Format(_T("Knulling = %d/%d (%.2lf/%.2lf)=>KNURLING"), g_Temp.m_nKnullingValue, g_Param.m_nKnullingValue, g_Temp.m_dNullingRatio,g_Param.m_dNullingRatioLimit);
//		else						str.Format(_T("Knulling = %d/%d (%.2lf/%.2lf)=>LEADING"),  g_Temp.m_nKnullingValue, g_Param.m_nKnullingValue, g_Temp.m_dNullingRatio,g_Param.m_dNullingRatioLimit);
		if(g_Temp.m_nFoundKnulling) str.Format(_T("Knulling = %d/%d =>KNURLING"), g_Temp.m_nKnullingValue, g_Param.m_nKnullingValue);
		else						
		{
			g_Temp.m_nKnullingValue=0;
			str.Format(_T("Knulling = %d/%d =>LEADING"),  g_Temp.m_nKnullingValue, g_Param.m_nKnullingValue);
		}
		m_sResult[m_nResultStringCnt++]=str;	
	}
#endif
	


#ifdef USE_DAKANO_LINE
	if(l_nStringCnt)
	{
		int i;
		m_nResultStringCnt=0;
		for(i=0;i<l_nStringCnt;i++)
			m_sResult[m_nResultStringCnt++]=l_String[i];
	}
#endif

	//최대-최소 표시--------------------------------------------------------------------
	if(g_Temp.m_nInspectWhite && g_Temp.m_nInspectBlack)
		str.Format(_T("Max : %d  Min : %d"), g_Temp.m_nMaxGray, g_Temp.m_nMinGray);
	else if(g_Temp.m_nInspectWhite)
		str.Format(_T("Max : %d  "), g_Temp.m_nMaxGray);
	else if(g_Temp.m_nInspectBlack)
		str.Format(_T("Min : %d  "), g_Temp.m_nMinGray);
	if(g_Temp.m_nInspectWhite || g_Temp.m_nInspectBlack)
		m_sResult[m_nResultStringCnt++]=str;   //m_ctrlResult.AddString(str);

#ifdef USE_SK_BAT
	if(g_Temp.m_nSideAvg>0)
	{
		str.Format(_T("Side Avg : %d  Min : %d"), g_Temp.m_nSideAvg, g_Temp.m_nSideBlackMin*(-1));
		m_sResult[m_nResultStringCnt++]=str; 
	}
#endif


	if(g_Temp.m_nCunicValue>0)
	{
		str.Format(_T("Cunic : %d  "), g_Temp.m_nCunicValue);
		m_sResult[m_nResultStringCnt++]=str;
	}


	if(g_Temp.m_nMaxLinekipo)
	{
		str.Format(_T("Line Bubble Value : %d (X: %d)"), g_Temp.m_nMaxLinekipo, g_Temp.m_nMaxLinekipoPos);
		m_sResult[m_nResultStringCnt++]=str;

	}
	if(g_Temp.m_nMaxWLineValue>0 || g_Temp.m_nMaxBLineValue>0)
	{
		str.Format(_T("W-Line Value : %d (X=%d) / B-Line Value : %d (X=%d)"), g_Temp.m_nMaxWLineValue, g_Temp.m_nMaxWLineValuePos, g_Temp.m_nMaxBLineValue, g_Temp.m_nMaxBLineValuePos);
		m_sResult[m_nResultStringCnt++]=str;
	}
	if(g_Temp.m_nMarkingInspect)
	{
		str.Format(_T("Mark Value : %.1lf"), g_Temp.m_dMaxMark);
		m_sResult[m_nResultStringCnt++]=str;
	}

	if(g_Temp.m_nMaxLRValue>0)
	{
		str.Format(_T("Line Value : %d (X=%d)"), g_Temp.m_nMaxLRValue, g_Temp.m_nMaxLRPosX);
		m_sResult[m_nResultStringCnt++]=str;
	}
#ifdef USE_COS9_LR
	if(g_Temp.m_nMaxLRWValue>0)
	{
		str.Format(_T("Line(W) Value : %d (X=%d)"), g_Temp.m_nMaxLRWValue, g_Temp.m_nMaxLRWPosX);
		m_sResult[m_nResultStringCnt++]=str;
	}
	if(g_Temp.m_nMuraMax>0)
	{
		str.Format(_T("Mura Value : %d "), g_Temp.m_nMuraMax);
		m_sResult[m_nResultStringCnt++]=str;
	}
#endif

	if(g_Temp.m_nMaxLineValue>0)
	{
		str.Format(_T("MD Line Value : %d (X=%d)"), g_Temp.m_nMaxLineValue, g_Temp.m_nMaxLinePos);
		m_sResult[m_nResultStringCnt++]=str;
	}
	if(g_Temp.m_nMaxLineValueTD>0)
	{
		str.Format(_T("TD Line Value : %d (X=%d)"), g_Temp.m_nMaxLineValueTD, g_Temp.m_nMaxLinePosTD);
		m_sResult[m_nResultStringCnt++]=str;
	}
	if(g_Temp.m_nMaxAngleLineValue>0)
	{
		str.Format(_T("Slope Line Value : %d (X=%d)"), g_Temp.m_nMaxAngleLineValue, g_Temp.m_nMaxAngleLinePos);
		m_sResult[m_nResultStringCnt++]=str;
	}

#ifndef USE_DAKANO_LINE
	if(g_Temp.m_dMuraMaxValue>0)
	{
		str.Format(_T("MURA(W) Value : %d  (%d,%d)"), (int)g_Temp.m_dMuraMaxValue, g_Temp.m_nMuraMaxPosX, g_Temp.m_nMuraMaxPosY);
		m_sResult[m_nResultStringCnt++]=str;
	}
#endif

	if(g_Temp.m_dMuraMaxValueB>0)
	{
		str.Format(_T("MURA(B) Value : %d  (%d,%d)"), (int)g_Temp.m_dMuraMaxValueB, g_Temp.m_nMuraMaxPosXB, g_Temp.m_nMuraMaxPosYB);
		m_sResult[m_nResultStringCnt++]=str;
	}

	if(g_Temp.m_nScratchInspect)
	{
		str.Format(_T("Scratch : %d"), g_Temp.m_MaxScratch);
		m_sResult[m_nResultStringCnt++]=str; //m_ctrlResult.AddString(str);
	}
	if(g_Temp.m_nLongScratchInspect)
	{
		str.Format(_T("(L)Scratch : %d (X=%d) "), g_Temp.m_MaxLongScratch,g_Temp.m_MaxLongScratchPos);
		m_sResult[m_nResultStringCnt++]=str; //m_ctrlResult.AddString(str);
	}

	if(g_Temp.m_nGroupInspect)
	{
		str.Format(_T("Group : Area: %d  All Image: %d"), g_Temp.m_nKoonJipCountInArea, g_Temp.m_nKoonJinCountInImage);
		m_sResult[m_nResultStringCnt++]=str; //m_ctrlResult.AddString(str);
	}

	if(g_Temp.m_nMaxMultiVerLineValue)
	{
		str.Format(_T("세로선얼룩 :  %d"), g_Temp.m_nMaxMultiVerLineValue);
		m_sResult[m_nResultStringCnt++]=str; //m_ctrlResult.AddString(str);
	}
#ifdef USE_ROTATION
	if(g_Temp.m_nMotionCheck)
	{
		str.Format(_T("%d Motion = %d(T:%d), Brightness=%d  Exposure=%d"), g_Temp.m_nMotionCount, g_Temp.m_dMotionPos, g_Temp.m_dMotionTarget,  g_Temp.m_dMotionBrightness, g_Temp.m_nExposureData);
		m_sResult[m_nResultStringCnt++]=str; //m_ctrlResult.AddString(str);
	}
#endif


#ifdef SHOW_DEBUG_DATA
	m_sResult[m_nResultStringCnt++]=_T("-----------------------------------------------------------------------------"); //m_ctrlResult.AddString(_T("-----------------------------------------------------------------------------"));
	str.Format(_T("Elapse Time:  %.1lf msec "), l_dTime[8]-l_dTime[0]);
	m_sResult[m_nResultStringCnt++]=str; //m_ctrlResult.AddString(str);
	str.Format(_T("Distortion: %.1lf  Profile: %.1lf  Image Flat: %.1lf"), l_dTime[1]-l_dTime[0], l_dTime[2]-l_dTime[1], l_dTime[3]-l_dTime[2]);
	m_sResult[m_nResultStringCnt++]=str; //m_ctrlResult.AddString(str);
	str.Format(_T("WEB_SpotInspect: Compress: %.1lf  Candidate: %.1lf Data %.1lf"),   l_dTime[4]-l_dTime[3], l_dTime[5]-l_dTime[4],l_dTime[6]-l_dTime[5]);
	m_sResult[m_nResultStringCnt++]=str; //m_ctrlResult.AddString(str);
	str.Format(_T("Scratch %.1lf  Mura %.1lf "),  l_dTime[7]-l_dTime[6], l_dTime[8]-l_dTime[7]);
	m_sResult[m_nResultStringCnt++]=str; //m_ctrlResult.AddString(str);
#endif


	DrawResultViewText();

}

#ifndef USE_3D_AREA
void CKoWebView::Inspect1()		//표준
{

#ifdef  USE_SK_BAT
		WEB_BATCurr();
		WEB_SpotInspect();
		WEB_WingInspect();
		return;
#endif


	if(g_System.m_nLineCAM==1)
	{
		if(WEB_ImageCheck()) return;			//이미지에 수평줄 있으면 return

		WEB_SpotInspect();						//(휘점, 쿠닉)
		
		l_dTime[6]=Clock_set(); //평활화, 후보위치 찾은후 부터 나머지...
		
		WEB_Scratch();							//(스크라치) (8192x8192 :50msec 걸림)

		if(g_Temp.m_nAutoAdjustStart==0)
			WEB_CrossGroup(0);					//(군집)	 (8192x8192 :13msec 걸림)

		l_dTime[7]=Clock_set();
		
		WEB_LineKkopChim();						//꼽침 (1/2압축후 중앙 1/4 만으로 구함) (8192x8192 :20msec 걸림)
	}
	else
	{
		if(g_System.m_nImageH==640)
			WEB_CunicInspect();					//(쿠닉검출) AREA 카메라
	}

	l_dTime[8]=Clock_set();

}

//물때얼룩 전용
#if PROGRAM_TYPE==WEB_DAKANO
void CKoWebView::Inspect2() 
{	
	//물때로 분류---------------------------------------------------------------------------
	WEB_MULTAE_Line_Level();	//라인검사
//	WEB_MULTAE_Mura_Level();

	
#ifdef TEST_MODE
	memcpy(m_pImage->m_fmDisplay, g_fmGrab[g_ID], g_System.m_nImageW*g_System.m_nImageH);
#endif

	WEB_SpotInspect();  //백점검사

	WebKoonJip();		//군집검사

	#ifdef WEB_DAKANO_MULTAE
		if(g_Defect.m_nDefectCount>0) g_Temp.m_nNG_MulTae=1;
	#endif
	//--------------------------------------------------------------------------------------------

	if(g_Defect.m_nDefectCount==0)	//불량없을때 검출한다.
		WEB_MuraSTD_White();


	if(l_nStringCnt)
	{
		int i;
		m_nResultStringCnt=0;
		for(i=0;i<l_nStringCnt;i++)
			m_sResult[m_nResultStringCnt++]=l_String[i];
	}
	DrawResultViewText();

}
#endif

#else

//USE_3D_AREA일때 사용하는 함수 Inspect1() , Inspect2() -------------------------------------------------
void CKoWebView::Inspect1()   //정밀코팅 AREA 1392x640 (흑, 백, 마킹, Fisheye, 얼룩, 액정라인, 군집)
{	
	WEB_RotCrossInspect();			//흑, 백, 마킹, Fisheye, 얼룩, 군집
	l_dTime[6]=Clock_set();	


	WEB_FindDefectCrossLR();		//액정라인
	l_dTime[8]=Clock_set();
}

void CKoWebView::Inspect2()
{
	WEB_FindDefectSTDEV();

	WEB_RotCrossInspect();			//흑, 백, 마킹, Fisheye, 얼룩, 군집
}
//-------------------------------------------------------------------------------------------------------

#endif



void CKoWebView::Inspect3()			//미분투과		
{
#ifdef USE_DAKANO_LINE

		WEB_SpotInspect();				//흑점백점
		WEB_FindTACLine_Level_NEW();	//라인얼룩

#else

	WEB_LongScratch();				//약간 사선허용 100곱한 값으로 긴 라인 구함.

	WEB_Scratch();						

	WEB_SpotInspect();				//(백, 라미눌림)

#endif
}


void CKoWebView::Inspect4()			//정투과
{
	WEB_PenMark();
	WEB_SpotInspect();				//(흑, 백, 이물기포)

	l_dTime[6]=Clock_set();	

//	WEB_Scratch();

	l_dTime[7]=Clock_set();
}

//투영검사-------------------------------------------------------------------------
void CKoWebView::Inspect5() 
{
	WEB_TooYoung();					//투영검사(흑/백검출)

l_dTime[6]=Clock_set();				

	TOO_FindMDLine();				//투영MD-LINE  (MD라인 4, 사선라인 3)
	TOO_FindTDLine();				//투영TD-LINE

l_dTime[7]=Clock_set();

#ifdef USE_TOO_MURA
	WEB_TooBlackMura();				//투영 흑 얼룩 검사
#endif

	TOO_FindBGroup();				//흑군집(개구리알)

l_dTime[8]=Clock_set();
}

//투영반사-----------------------------------------------------------------------
void CKoWebView::InspectA()			//투영반사
{ 
	WEB_TooYoung_Press();			//찍힘검출

	TOO_FindMDLine();				//투영MD-LINE (MD라인 4, 사선라인 5)
	TOO_FindTDLine();				//투영TD-LINE

	WEB_TooBlackMura();				//투영반사 흑 얼룩 검사


l_dTime[6]=Clock_set();				//평활화, 후보위치 찾은 후 부터 나머지...
l_dTime[7]=Clock_set();
l_dTime[8]=Clock_set();

}


#ifdef USE_DAKANO_LINE
	extern CString l_String[20];
	extern int l_nStringCnt;
#endif


#if INSPECT_TYPE==USE_DSFR
void CKoWebView::Inspect7()
{
	//흑점/백점/찍힘 검출 ------------------
	WEB_SpotInspect();

	WEB_MuraTAC();		//TAC얼룩같은 것 검사

	l_dTime[6]=Clock_set(); //평활화, 후보위치 찾은후 부터 나머지...

	WEB_MuraTACLine();	//TAC라인 검출

	l_dTime[7]=Clock_set();

	l_dTime[8]=Clock_set();
}
void CKoWebView::Inspect8()
{
	//흑점/백점/찍힘 검출 ------------------
	WEB_SpotInspect();	
}
void CKoWebView::Inspect9()
{
	//흑점/백점/찍힘 검출 ------------------
	WEB_SpotInspect();	
}
#elif INSPECT_TYPE==TAC_LINE
void CKoWebView::Inspect2()
{
	Inspect6();
	
}

void CKoWebView::Inspect6()
{
	WEB_SpotInspect();		//백/흑점 
	WEB_MuraTAC();			//TAC얼룩 검사
	WEB_MuraTACLine_COS6();	//TAC라인 검출 (MD)
	WEB_Scratch();	
	if(g_Temp.m_nPCOptic==COS2) WEB_MuraTACLine_TD();
}

void CKoWebView::Inspect7()
{
	//흑점/백점/찍힘 검출 ------------------
	WEB_SpotInspect();	
}

void CKoWebView::Inspect8()
{
	//흑점/백점/찍힘 검출 ------------------
	WEB_SpotInspect();	
}
void CKoWebView::Inspect9()
{
	//흑점/백점/찍힘 검출 ------------------
	WEB_SpotInspect();	
}
#else
void CKoWebView::Inspect2()
{

#ifdef  USE_SK_BAT
		Inspect1();
		return;
#endif


	WEB_SpotInspect();				//흑/백/찍힘
}

void CKoWebView::Inspect6()			
{
	WEB_SpotInspect();				//흑/백/찍힘

	//군집추가 (Rect, 갯수, Value, Size는 Level로 TH는 공통으로)
	WebKoonJip();

	WEB_MultiVerLine();
}
void CKoWebView::Inspect7()
{
	//정코마킹검출  ------------------
	WEB_FindMarking();

	//흑점/백점/찍힘 검출 ------------------
	WEB_SpotInspect();

	//군집추가 (Rect, 갯수, Value, Size는 Level로 TH는 공통으로)
	WebKoonJip();

	l_dTime[6]=Clock_set(); //평활화, 후보위치 찾은후 부터 나머지...

	//라인기포검출-------------------------
	WEB_LineKipo();

	l_dTime[7]=Clock_set();

	//얼룩검출-----------------------
//사용안함	
//	WEB_MuraSTD();

	//스크라치검출(Option)
	l_dTime[8]=Clock_set();
}

void CKoWebView::Inspect8()			//경계투과/반사
{
	WEB_SpotInspect();				//(흑, 백, 찍힘)

	

//	WebKoonJip();

//	WEB_AngleLine();				//(남경? 요청사항)   원단이 꺽인것


//아래는 스크라치를 검출할 것이냐? 약한 연속되는 라인을 검출할 것이냐에 따라 선택적으로 사용해야함.
//Param151, Param161 을 공통으로 사용함.
#if INSPECT_TYPE==USE_NORMAL		
	//WEB_VLine();					//연속으로 나오는 MD 라인검출(10 Frame에 5번이상)
	 WEB_ScratchBlob_White();		//(선형불량)	
#endif

}

void CKoWebView::Inspect9()			//크로스
{
#ifdef USE_COS9_LR
		WEB_CrossLR();					//LR라인 OK
		WEB_CrossLR_White();			//LR(White)라인 
		WEB_SpotInspect();				//흑/백  OK
		if(g_Temp.m_nAutoAdjustStart==0)
			WEB_CrossGroup(0);			//군집  OK

		WEB_LR_Mura();					//얼룩검사
		return;
#endif


	if(g_System.m_nLineCAM==1)
	{
		if(WEB_ImageCheck()) return;			//이미지에 수평줄 있으면 return

		WEB_LineKkopChim();						//꼽침 (1/2압축후 중앙 1/4 만으로 구함) (8192x8192 :20msec 걸림)


		WEB_SpotInspect();						//(휘점, S/C, 흑점, 쿠닉)
		
		l_dTime[6]=Clock_set(); //평활화, 후보위치 찾은후 부터 나머지...
		
#ifndef NANJING_OLED 
		WEB_Scratch();							//(스크라치) (8192x8192 :50msec 걸림)
#endif

		if(g_Temp.m_nAutoAdjustStart==0)
			WEB_CrossGroup(0);					//(군집)	 (8192x8192 :13msec 걸림)

		l_dTime[7]=Clock_set();
	}
	else
	{
		if(g_System.m_nImageH==640)
			WEB_CunicInspect();					//(쿠닉검출) Line Scan일때는 여기서 검출하지 않는다.
	}


	l_dTime[8]=Clock_set();
}
#endif



void CKoWebView::InspectB()			//TEST
{
	Inspect1();
}
void CKoWebView::InspectC()			//TEST
{

}
void CKoWebView::InspectD()			//TEST
{

}


void CKoWebView::InspectBCR()
{
#ifdef BARCODE_VISION
	 WEB_Barcode(this);
#endif
}
void CKoWebView::InspectMarkingVision()
{

#ifdef MARKING_VISION
	WEB_CheckMarkingData();
#endif

}

extern CXManageSocket  l_Send_Server;
int FindNulling(LPBYTE fm, int left, int top, int right, int bottom, int pitch);
void CKoWebView::CheckKnulling()
{

	g_Temp.m_nKnullingValue=FindNulling(g_fmGrab[g_ID], 0, 0, g_System.m_nImageW-1, g_System.m_nImageH-1, g_System.m_nPitch);

	if(g_Temp.m_nKnullingValue>g_Param.m_nKnullingValue)// && g_Temp.m_dNullingRatio>g_Param.m_dNullingRatioLimit) 
	{
		g_Temp.m_nFoundKnulling=1;
	}
	else
	{
		g_Temp.m_nFoundKnulling=0;
	}


	//NULLING 확인 보내기 --------------------------------------------------------
	int nOK=1;
	CPacket* packet = new CPacket;
	packet->MakeReturnPacket(NM_NULLING_MARK, g_Temp.m_nFoundKnulling);	//널링있음
	l_Send_Server.SendInsData(packet);
	delete packet;
	//------------------------------------------------------------------------
}

int FindNulling(LPBYTE fm, int left, int top, int right, int bottom, int pitch)
{
	int i,j,k, tmp;
	int min, max;
	long nHistoSum, nCount, nHistoSum_, nCount_;
	int nMaxHisto, nMaxPos;
	double dAvg;
	CString str;
	long l_nHisto[900], l_nHisto1[900];


	memset(l_nHisto, 0, 900*sizeof(long));

	for(k=0;k<20;k++)
	{
		for(j=left;j<right;j+=5)
		{
			min=255; max=0;
			for(i=k*90;i<k*90+50;i+=2)
			{	
				tmp=*(fm+pitch*i+j);
				if(tmp>max)      max=tmp;
				else if(tmp<min) min=tmp;
			}

			l_nHisto[j/10]+=(max-min);
		}
	}

	nHistoSum=0;
	for(j=0;j<20;j++) nHistoSum+=l_nHisto[j];
	l_nHisto1[0]=nHistoSum/20;

	for(j=20;j<900;j++)
	{
		nHistoSum+=(l_nHisto[j]-l_nHisto[j-20]);
		l_nHisto1[j-19]=nHistoSum/20;
	}
	
	nMaxHisto=0;
	for(j=0;j<900-20;j++)
	{
		if(l_nHisto1[j]>nMaxHisto)
		{
			nMaxHisto=l_nHisto1[j];
			nMaxPos=j;
		}
	}

	nHistoSum=nHistoSum_=nCount=nCount_=0;
	for(j=0;j<nMaxPos-20;j++)
	{ 
		nHistoSum+=l_nHisto1[j];
		nCount++;
	}
	for(j=nMaxPos+20;j<900-20;j++)
	{
		if(l_nHisto1[j]>0)
		{
			nHistoSum_+=l_nHisto1[j];
			nCount_++;
		}
	}
	
	if(nCount>10 && nCount_>10)
	{
		if(nHistoSum/nCount > nHistoSum_/nCount_) dAvg=(double)nHistoSum/nCount;
		else									  dAvg=(double)nHistoSum_/nCount_;
		
		g_Temp.m_dNullingRatio=(double)nMaxHisto/dAvg;
	}
	else if(nCount+nCount_>0)
	{
		dAvg=(double)(nHistoSum+nHistoSum_)/(nCount+nCount_);
		g_Temp.m_dNullingRatio=(double)nMaxHisto/dAvg;
	}
	else
		g_Temp.m_dNullingRatio=0;

#ifdef SLITTER
	g_Temp.m_nKnullingNotInspArea = nMaxPos*10;
#endif
	return nMaxHisto;
}
