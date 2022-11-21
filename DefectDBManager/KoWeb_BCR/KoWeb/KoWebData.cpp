#include "stdafx.h"

int        g_ID=0;		//현재 검사할 이미지ID  0, 1 둘중한개
LPBYTE     g_fmGrab[MAX_IMAGE]; //Grab이미지 (원본)
LPBYTE     g_fmNext;			//검사할Frame의 다음 Frame
LPBYTE     g_fmSubPrev;			//검사할 Frame의 직전 128 Pixel
LPBYTE     g_fmSubNext;			//검사할 Frame의 다음 128 Pixel
LPBYTE     g_fmSubPrevFlat;		//검사할 Frame의 직전 128 Pixel
LPBYTE     g_fmSubNextFlat;		//검사할 Frame의 다음 128 Pixel
LPBYTE     g_fmFlat[MAX_IMAGE]; //평활화된 이미지

LPBYTE     g_fmGrabComp; //Grab압축영상(W/2 H/2)
LPBYTE     g_fmGrabComp1;//Grab압축영상(W/2 H/4)
LPBYTE     g_fmHole[3];
LPBYTE     g_fmSave;	//Save할 이미지는 여기 복사
LPBYTE     g_fmSaveComp;//Save할 이미지는 여기 복사	//(1/8)압축영상
LPBYTE     g_fmTemp;    //검사중 각종 용도로 사용
LPBYTE     g_fmTemp1;    //검사중 각종 용도로 사용
LPBYTE     g_fmMAP;
//LPBYTE     g_fmOldMAP;	//이전 MAP

LPBYTE g_fmBinaryWing;	//BAT


int*       g_Profile;	//프로파일 정보
int*       g_Profile100;//프로파일 정보(100배)
int*       g_ProfileH;	//프로파일을 W를 1/2로 만듦
int*       g_ProfileHor; //수평 Profile 노이즈 없앨때 사용함
int*	   g_LUT;       // [320][2048];   //투영 LUT
int        g_DefectLUT[256]; //Defect영상을 위한 LUT

//불량영상 가공 및 테스트용
BYTE       g_fmDefectTemp[BAD_IMG_WIDTH*BAD_IMG_HEIGHT];
BYTE       g_fmDefectGray[BAD_IMG_WIDTH*BAD_IMG_HEIGHT];
BYTE       g_fmDefectTemp1[BAD_IMG_WIDTH*BAD_IMG_HEIGHT];
BYTE       g_fmDefectBinary[BAD_IMG_WIDTH*BAD_IMG_HEIGHT];
BYTE       g_fmPrev[32*MAX_WIDTH];		//직전 Frame 32라인 저장해둠
int        g_BaseLine[4096];

SYSTEMDATA		g_System;
CONFIGDATA		g_Config;
DEFECTDATA		g_Defect;
DEFECTDATA		g_DefectSend;
int				g_AreaDelSplice[10000];// 영역 나눔 확인

//RESULT_DELAY -------------------------
DEFECTDATA		g_DefectSendDelay[6];   //[0]: 원래보낼것, [1] 1Frame Delay  ... [5] 5Frame Delay
int             g_DelayIndex;
//--------------------------------------

DEFECTTEMP		g_DefectTemp;
FRAMEINFO       g_FrameInfo[100];  //한Frame정보
TEMPDATA		g_Temp;	
CImage			g_Save;
CImage			g_SaveNG;
CImage			g_SaveComp;			//물때저장용

//검출후보-----------------------------------------------------------
CANDIPOINTS     g_CandiW;
CANDIPOINTS     g_CandiB;
CANDIPOINTS     g_CandiArea;
CANDIPOINTS     g_CandiTinyB;	  //작은 흑점후보	
CANDIDATA		g_CanidData[200]; //투영은 25개의 영역과 X로는 8개로 나눠지므로 200개임.		



//COS5(투영)   : g_System.m_nImageW=2048, g_System.m_nImageH=3200, g_System.m_nImagePart=25, g_System.m_nLineCAM=0, g_System.m_nCandiRect=128, g_System.m_nPerspectiveTR=1
//COS9(크로스) : g_System.m_nImageW=1392, g_System.m_nImageH=640,  g_System.m_nImagePart=2,  g_System.m_nLineCAM=0, g_System.m_nCandiRect=160, g_System.m_nPerspectiveTR=0
//COSA(투영)   : g_System.m_nImageW=2048, g_System.m_nImageH=3200, g_System.m_nImagePart=25, g_System.m_nLineCAM=0, g_System.m_nCandiRect=128, g_System.m_nPerspectiveTR=1

//불량종류 Default로 사용하는 것
int NG_SPOT_WHITE;		//백점, 휘점
int NG_LAMI_PRESS;		//Lami Press
int NG_SPOT_BLACK;		//흑점
int NG_SCRATCH;			//스크라치
int NG_LONGSCRATCH;		//Long스크라치
int NG_CUNIC;			//쿠닉
int NG_SPOT_PRESS;		//찍힘
int NG_KKOPCHIM;		//꼽침
int NG_GROUP;			//군집
int NG_MURA;			//얼룩
int NG_TAC_MURA;		//TAC성 얼룩
int NG_TAC_MURA_B;		//TAC성 (흑)얼룩
int NG_TAC_LINE;		//TAC라인
int NG_SPOT_KIPO;		//이물성기포(정투과)
int NG_LINE;			//라인
int NG_LINE_ANGLE;      //사선라인
int NG_LINE_TD;			//TD라인
int NG_COS4_105INCH;	//수평라인
int NG_KIPOPRESS;		//기포눌림 (NEL7 76)
int NG_LINEKIPO;		//라인기포(NEL7)
int NG_MARKING;			//정코마킹
int NG_BLACK_MURA;		//투영흑얼룩
int NG_BLACK_GROUP;		//투영흑군집
int NG_FISHEYE;			//Fisheye
int NG_LIPSHAPE;		//입술형태불량

int NG_WING_BLOCK=5;	//BAT Side 흑점


int LoadSystemData()
{
	int i, ok=0;
	TCHAR  sTemp[20];

	//기본 정보--------------------------------------------------------------------------------------------------
	g_System.m_nImageW			=GetPrivateProfileInt(_T("IMAGE"),_T("WIDTH"),				8192,	SYSTEM_PATH); 
	g_System.m_nPitch			=GetPrivateProfileInt(_T("IMAGE"),_T("PITCH"),				8192,	SYSTEM_PATH);  
	g_System.m_nImageH			=GetPrivateProfileInt(_T("IMAGE"),_T("HEIGHT"),				8192,	SYSTEM_PATH);  
	g_System.m_nImagePart		=GetPrivateProfileInt(_T("IMAGE"),_T("IMAGEPART"),			 1,		SYSTEM_PATH); //(1,2,25 중 선택) 1:라인스캔, 2:Area 2장촬영  25:투영 25장 촬영
	g_System.m_nFlatBright		=GetPrivateProfileInt(_T("IMAGE"),_T("IMAGEFLATVALUE"),		 80,	SYSTEM_PATH); //영상 평활화시 밝기(	0 이면 평활화 하지 않음)
	g_System.m_nBigDefect       =GetPrivateProfileInt(_T("IMAGE"),_T("BIGDEFECT"),			  0,	SYSTEM_PATH); //4배 압축한 영상에서 이미지 검출하면 1 그렇지 않으면 0

	//자동밝기에 사용될 데이타----------------------------------------------------------------------------------
	g_System.m_nGrabBright		=GetPrivateProfileInt(_T("IMAGE"),_T("IMAGEGRABVALUE"),		 80,	SYSTEM_PATH); //영상 Grab시 평균세팅밝기
	g_System.m_nMaxExposure		=GetPrivateProfileInt(_T("IMAGE"),_T("MAXEXPOSURE"),		 5000,	SYSTEM_PATH); //최대 노출량
	g_System.m_nMinExposure		=GetPrivateProfileInt(_T("IMAGE"),_T("MINEXPOSURE"),		 1200,	SYSTEM_PATH); //최소 노출량
	g_System.m_nStartGrayDiff   =GetPrivateProfileInt(_T("IMAGE"),_T("STARTGRAYDIFF"),		 3,	    SYSTEM_PATH); //이 값 초과로 평균대비 밝기차가 나면 COUNT
	g_System.m_nTargetGrayDiff	=GetPrivateProfileInt(_T("IMAGE"),_T("TARGETGRAYDIFF"),		 1,	    SYSTEM_PATH); //이 값 이하로 들어오면 자동밝기 STOP
	g_System.m_nAutoExpoStartCnt=GetPrivateProfileInt(_T("IMAGE"),_T("AUTOEXPOSTARTCOUNT"),	 3,	    SYSTEM_PATH); //자동밝기 시작을 위해 밝기차이가 연속으로 N 번 발생시 자동밝기 시작
	g_System.m_nAutoExpoEndCnt  =GetPrivateProfileInt(_T("IMAGE"),_T("AUTOEXPOENDCOUNT"),	 30,    SYSTEM_PATH); //자동밝기 시도 횟수 (최대시도 횟수)
	g_System.m_nAutoExpoMinValue=GetPrivateProfileInt(_T("IMAGE"),_T("AUTOEXPOMINVALUE"),	 10,	SYSTEM_PATH); //자동밝기 Exposure 최소단위

	//----------------------------------------------------------------------------------------------------------
	g_System.m_nVisionBoardType =GetPrivateProfileInt(_T("SYSTEM"),_T("GRABBERTYPE"),		  0,	SYSTEM_PATH); //0: Solios  1: Radient보드	
	g_System.m_nServerType		=GetPrivateProfileInt(_T("SYSTEM"),_T("SERVERTYPE"),		  0,	SYSTEM_PATH); //0: .254    1: .244
	g_System.m_nCAMAngle        =GetPrivateProfileInt(_T("SYSTEM"),_T("CAMANGLE"),			  0,	SYSTEM_PATH); //CAMANGLE MD방향에 대해서 기울어진 각도(오른쪽이 진행방향과 반대방향으로 기울어져 있으면 + 각도)
	g_System.m_nLineCAM         =GetPrivateProfileInt(_T("SYSTEM"),_T("LINESCAN"),			  1,	SYSTEM_PATH); //0: AREA   1:LineScan
	g_System.m_nMaxDefect		=GetPrivateProfileInt(_T("SYSTEM"),_T("MAXDEFECT"),			 100,	SYSTEM_PATH); //검출할 최대 불량 수  (MAX_DEFECT, g_System.m_nCandiRect, g_System.m_nOverlapX, g_System.m_nOverlapY 조정 잘해야 함)
	g_System.m_nMaxCandiDefect	=GetPrivateProfileInt(_T("SYSTEM"),_T("MAXCANDIDEFECT"),	 300,	SYSTEM_PATH); //검출할 최대 후보 수

#ifdef SERVER_TEST
	g_System.m_nServerTest      =GetPrivateProfileInt(_T("SYSTEM"),_T("SERVERTEST"),	 0,	SYSTEM_PATH);		  //1이면 서버테스트용
	g_System.m_nMarkServerTest[0]  =GetPrivateProfileInt(_T("SYSTEM"),_T("MARKSERVERTEST0"), 0,	SYSTEM_PATH);		  //1이면 서버테스트용
	g_System.m_nMarkServerTest[1]  =GetPrivateProfileInt(_T("SYSTEM"),_T("MARKSERVERTEST1"), 0,	SYSTEM_PATH);		  //1이면 서버테스트용
#endif

	//데이타 Check---------------------------------------------------------------------------------------------
	g_System.m_nGrabBrightBK    =g_System.m_nGrabBright;

	if(g_System.m_nGrabBright<=0)		 g_System.m_nGrabBright		=80;
	if(g_System.m_nFlatBright<=0)		 g_System.m_nFlatBright		=80;
	if(g_System.m_nTargetGrayDiff<1)	 g_System.m_nTargetGrayDiff	=1;
	if(g_System.m_nMaxDefect>MAX_DEFECT) g_System.m_nMaxDefect		=MAX_DEFECT;
	if(g_System.m_nMaxCandiDefect>1024)  g_System.m_nMaxCandiDefect	=1024;

	//최소 Exposure 설정------------------------------------------------
	if(g_System.m_nLineCAM)
	{
		if(g_System.m_nMinExposure<100) g_System.m_nMinExposure=100;
	}
	else
	{
		if(g_System.m_nMinExposure<40)  g_System.m_nMinExposure=40;
	}
	//--------------------------------------------------------------------------------------------------------

	//주기관련 
	::GetPrivateProfileString(_T("SYSTEM"),_T("PERIOD_LIMIT_X"), _T("3.0"),	sTemp, 20, SYSTEM_PATH); 
	g_System.m_dPeriodLimitX=_tstof(sTemp);
	::GetPrivateProfileString(_T("SYSTEM"),_T("PERIOD_LIMIT_Y"), _T("5.0"),	sTemp, 20, SYSTEM_PATH); 
	g_System.m_dPeriodLimitY=_tstof(sTemp);


	//마킹관련
	g_System.m_nMarkServerCnt   =GetPrivateProfileInt(_T("SYSTEM"),_T("MARKSERVERCOUNT"),	  0,	SYSTEM_PATH); //마킹서버 수
	g_System.m_nMarkDataUSE		=GetPrivateProfileInt(_T("SYSTEM"),_T("MARKINGDATA"),		  1,	SYSTEM_PATH); //파라미터에 마킹정보가 있고, 그것을 사용하려면 1	
	g_System.m_nResultSendMethod=GetPrivateProfileInt(_T("SYSTEM"),_T("RESULTSEND"),		  1,	SYSTEM_PATH); //1이면 검사결과나오면 바로, 0이면 다음 Frame촬상후, 2이면 결과 보내지 않음
	g_System.m_nResultDaley		=GetPrivateProfileInt(_T("SYSTEM"),_T("RESULT_DELAY"),		  0,	SYSTEM_PATH); //해당값 만큼 Delay
	if(g_System.m_nResultDaley>5) g_System.m_nResultDaley=5; //최대 5 Frame Delay

#ifndef RESULT_DELAY
	g_System.m_nResultDaley=0;	//RESULT_DELAY 아니면 무조건 0
#endif

	if(g_System.m_nMarkServerCnt>0) g_System.m_nResultSendMethod=0;												  //마킹서버를 사용하는 경우 검사결과는 바로 안보내고 다음 Frame촬상후


	g_System.m_nPerspectiveTR   =GetPrivateProfileInt(_T("SYSTEM"),_T("PERSPECTIVETR"),		  0,	SYSTEM_PATH); //투영, LR처럼 Perspective Trasform 하면 1 안하면 0
	g_System.m_nSimulServer		=GetPrivateProfileInt(_T("SYSTEM"),_T("SIMUL_SERVER"),		  0,	SYSTEM_PATH); //서버와 연동해서 Simulation	

	//CAMERA TAPE SENSING ---------------------------------------------------------------------------------------
	g_System.m_nUseCamTapeSensor=GetPrivateProfileInt(_T("SYSTEM"),_T("USECAMTAPESENSOR"),	  0,	SYSTEM_PATH); //1이면 카메라를 Tape Sensor로 이용함.
	g_System.m_nTapeThreshold   =GetPrivateProfileInt(_T("SYSTEM"),_T("TAPETHRESHOLD"),		  20,	SYSTEM_PATH); //Tape Threshold로 이용함. 수평라인이 평균값대비 이 값보다 낮으면 Tape간주
	g_System.m_nTapeNotArea     =GetPrivateProfileInt(_T("SYSTEM"),_T("TAPENOTAREA"),		  0,	SYSTEM_PATH); //(N 또는 -N) 이면 해당영역 제외하고 테입검사 : 카메라 영역에서 왼쪽 N Pixel 오른쪽 (-N) Pixel제외하고 검사


	//Defect 영상개선 (보여주는 것만개선함) : 특별한 경우만 사용함-----------------------------------------------
	g_System.m_nDefectApplyLUT  =GetPrivateProfileInt(_T("SYSTEM"),_T("DEFECTALLPYLUT"),	  0,	SYSTEM_PATH); //보여주는 Defect이미지에 LUT적용(미세이물 Cross일때만 적용 (1))
	g_System.m_nBrightLUT		=GetPrivateProfileInt(_T("SYSTEM"),_T("BRIGHTLUT"),			  220,	SYSTEM_PATH); //Defect이미지에 적용될 LUT 데이타(Brightness) //기준이 128
	g_System.m_nContrastLUT		=GetPrivateProfileInt(_T("SYSTEM"),_T("CONTRASTLUT"),		  200,	SYSTEM_PATH); //Defect이미지에 적용될 LUT 데이타(Contrast)   //기준이 128

	if(g_System.m_nDefectApplyLUT)
	{
		MakeDefectLUT(g_System.m_nBrightLUT, g_System.m_nContrastLUT);
	}
	else
	{
		for(i=0;i<256;i++) g_DefectLUT[i]=i;
	}
	//------------------------------------------------------------------------------------------------------------

/*
//광저우 투영 ------------------------------------
	g_System.m_nImageW=g_System.m_nPitch=2048;
	g_System.m_nImageH=3200;
	g_System.m_nImagePart=25;
	g_System.m_nLineCAM=0;
	g_System.m_nPerspectiveTR=1;
	g_System.m_nMaxExposure=2000;
	g_System.m_nMinExposure=40;

//광저우 경계
	g_System.m_nImageW=g_System.m_nPitch=g_System.m_nImageH=8192;
	g_System.m_nLineCAM=1;	 //나머지는 자동변경됨.
	g_System.m_nMaxExposure=3000;
	g_System.m_nMinExposure=100;
*/

	if(g_System.m_nImageW>4096) g_System.m_nMakePyramid=1;	//영상 압축해서 사용함. (4096보다 큰 이미지는 시간관계상 압축해서 후보등 구함)
	else						g_System.m_nMakePyramid=0;


#ifdef AI_APPLY
	::GetPrivateProfileString(_T("SYSTEM"),_T("AI_IP"),	_T("127.0.0.1"),  g_System.m_sAIIP,	 20,    SYSTEM_PATH);
	g_System.m_nAIPort			=GetPrivateProfileInt(_T("SYSTEM"),_T("AI_PORT"),		   4031,	SYSTEM_PATH);
#endif

	g_System.m_nBDOverkill		=GetPrivateProfileInt(_T("PARAM"),_T("BD_OVERKILL"),		  0,	SYSTEM_PATH); //경계투과(반사) 만 사용하도록...
	g_System.m_nCandiRect		=GetPrivateProfileInt(_T("PARAM"),_T("CANDI_RECT"),			256,	SYSTEM_PATH); //후보구하는 영역범위(X x Y)
	g_System.m_nOverlapX		=GetPrivateProfileInt(_T("PARAM"),_T("OVERLAP_OFFSETX"),	  5,	SYSTEM_PATH); //중복되는 영역X범위
	g_System.m_nOverlapY		=GetPrivateProfileInt(_T("PARAM"),_T("OVERLAP_OFFSETY"),	  5,	SYSTEM_PATH); //중복되는 영역Y범위

	//최대 격자수는 1024로 고정되어 있으므로 그걸 넘어가면 격자크기를 256으로함. 256 이면 이미지(8192x8192까지 Cover)-------
	if((g_System.m_nImageW/g_System.m_nCandiRect)*(g_System.m_nImageH/g_System.m_nCandiRect)>1024) g_System.m_nCandiRect=256;	
	if(g_System.m_nMaxDefect>MAX_DEFECT)	  g_System.m_nMaxDefect	    =MAX_DEFECT;		//한 Frame의 최대 불량수는 30개임.
	if(g_System.m_nImageW<=0 || g_System.m_nImageH<=0 || g_System.m_nPitch<=0) ok=-1;	    //FAILURE	
	
	//라인스캔이면 아래사항 해당없음
	if(g_System.m_nLineCAM)									
	{
		 g_System.m_nPerspectiveTR=0;	//LineSacn이면 Perspective Transform은 항상 0 이다.

#ifdef MY_RECV
		 g_System.m_nImagePart=16;	//이 PC가 받는것이면 16등분해서 Grab한다. (2048로 128Pixel 씩)
		 g_System.m_nImagePartH   =g_System.m_nImageH/16;
#else
		 g_System.m_nImagePart    =1;  
		 g_System.m_nImagePartH   =g_System.m_nImageH;
#endif
		  
	}
	else
	{
		if(g_System.m_nImagePart>0) g_System.m_nImagePartH=g_System.m_nImageH/g_System.m_nImagePart;
		if(g_System.m_nImagePartH*g_System.m_nImagePart != g_System.m_nImageH)
		{
			AfxMessageBox(_T("이미지 설정이 잘못 되었습니다. (HEIGHT, IMAGEPART)"));
			return -1;
		}
	}

	LoadConfigData();  //Config 데이타 Load


#ifdef USE_3D_AREA
	g_System.m_nImageW=g_System.m_nPitch=1392;
	#ifdef USE_ROTATION
		g_System.m_nImageH=640;		//NEL1 이면 640  NEL2면 320
	#else
		g_System.m_nImageH=320;		//NEL1 이면 640  NEL2면 320
	#endif
	g_System.m_nImagePart=2;
	g_System.m_nImagePartH=g_System.m_nImageH/g_System.m_nImagePart;
	g_System.m_nLineCAM=0;
	g_System.m_nGrabBright=g_System.m_nGrabBrightBK=50;
	g_System.m_nFlatBright=100;
	g_System.m_nMinExposure=40;
	g_System.m_nMaxExposure=2000;
	g_System.m_nMaxDefect=2;
	g_System.m_nMarkServerCnt=1;

#endif

#ifdef USE_MISE_IMUL
	g_System.m_nImageW=g_System.m_nPitch=8192;	
	g_System.m_nImageH=8192;
	g_System.m_nLineCAM=1;
	g_System.m_nImagePart=1;
	g_System.m_nGrabBright=g_System.m_nGrabBrightBK=60;
	g_System.m_nFlatBright=80;
	g_System.m_nMaxExposure=5000;
	g_System.m_nMinExposure=1200;
	
	g_System.m_nMarkServerCnt=2;
	g_System.m_nMarkDataUSE=1;
	g_System.m_nResultSendMethod=0;
#endif


#ifdef USE_SK_BAT
	g_System.m_nImageW=g_System.m_nPitch=8192;	
	g_System.m_nImageH=4096;
	g_System.m_nLineCAM=1;
	g_System.m_nImagePart=1;
	g_System.m_nGrabBright=g_System.m_nGrabBrightBK=80;
	g_System.m_nFlatBright=80;
	g_System.m_nMaxExposure=5000;
	g_System.m_nMinExposure=100;
	
	g_System.m_nMarkServerCnt=1;
	g_System.m_nMarkDataUSE=1;
	g_System.m_nResultSendMethod=0;
	g_System.m_nMakePyramid=1;
#endif

#ifdef USE_DAKANO_LINE
	g_System.m_nImageW=g_System.m_nPitch=8192;	
	g_System.m_nImageH		=8192;
	g_System.m_nLineCAM		=1;
	g_System.m_nImagePart	=1;
	g_System.m_nGrabBright	=g_System.m_nGrabBrightBK=128; //YKS  80->128
	g_System.m_nFlatBright	=128;//YKS 80->128
	g_System.m_nServerType	=1;
	g_System.m_nMaxExposure	=5000;
//	g_System.m_nMinExposure	=1200;
	
	g_System.m_nMarkServerCnt=2;
	g_System.m_nMarkDataUSE	=1;
	g_System.m_nResultSendMethod=0;
	g_System.m_nMakePyramid	=1;
	g_System.m_nCandiRect	=512;
#endif


#ifdef USE_COS9_LR
	g_System.m_nImageW=g_System.m_nPitch=1392;	
	g_System.m_nImageH=320*8;
	g_System.m_nImagePart=8;
	g_System.m_nImagePartH=320;
	g_System.m_nLineCAM=0;
	g_System.m_nMaxExposure=2000;
	g_System.m_nMinExposure=40;
	g_System.m_nCandiRect=160;
#endif

#ifdef MARKING_VISION
	//가능하면 4096x2048, 또는 8192x2048로 함. X는 1Pixel당 0.2~0.25  Y 는 1Pixel당 0.2 로 한다.
	g_System.m_nImagePart=16;	//이 PC가 받는것이면 16등분해서 Grab한다. (2048로 128Pixel 씩)
	g_System.m_nImagePartH   =g_System.m_nImageH/16;
#endif


#ifdef NANJING_OLED
	g_System.m_nImageW=g_System.m_nPitch=8192;	
	g_System.m_nImageH=8192;
	g_System.m_nLineCAM=1;
	g_System.m_nImagePart=1;
//	g_System.m_nGrabBright=g_System.m_nGrabBrightBK=60;
	g_System.m_nFlatBright=80;
	g_System.m_nMaxExposure=5000;
	g_System.m_nMinExposure=100;
#endif

#ifdef BARCODE_VISION
	g_System.m_nImageW = g_System.m_nPitch = 4096;
	g_System.m_nImageH = 2048;
	g_System.m_nMakePyramid = 0;

	// 테스트 용
	g_System.m_nFlatBright = 0;
#endif

	return ok;
}


int LoadConfigData()
{
	int ok=0;

	g_Config.m_nImageType		=GetPrivateProfileInt(_T("CONFIG"),_T("IMAGETYPE"),	0,	CONFIG_PATH);  
	g_Config.m_nImageSaveMethod	=GetPrivateProfileInt(_T("CONFIG"),_T("SAVETYPE"),	0,	CONFIG_PATH);  
	g_Config.m_nImageSaveMethod	=0;  
	g_Config.m_nImageSaveCount	=GetPrivateProfileInt(_T("CONFIG"),_T("SAVECOUNT"),	100,CONFIG_PATH);  
	g_Config.m_nImageInverse    =GetPrivateProfileInt(_T("CONFIG"),_T("IMAGEINVERSE"),	0,	CONFIG_PATH); 
	g_Config.m_nModePeriod      =GetPrivateProfileInt(_T("CONFIG"),_T("MODEPERIOD"),	0,	CONFIG_PATH); 

#ifdef USE_ROTATION
	g_Config.m_nComPort			=GetPrivateProfileInt(_T("CONFIG"),_T("COMPORT"),	21,	CONFIG_PATH); 
#else
	g_Config.m_nComPort			=GetPrivateProfileInt(_T("CONFIG"),_T("COMPORT"),	5,	CONFIG_PATH); 
#endif

#ifdef INTERFACE_NEXTEYE_SERVER	
	#ifdef USE_ROTATION
		g_Config.m_nComPort			=GetPrivateProfileInt(_T("SETTING"),_T("INDEX"),	21,	_T("C:\\Nexteye\\Serial.ini")); 
	#else
		g_Config.m_nComPort			=GetPrivateProfileInt(_T("SETTING"),_T("INDEX"),	5,	_T("C:\\Nexteye\\Serial.ini")); 
	#endif
#endif

	return ok;
}


int SaveSystemData()
{
	int ok=-1;
	CStdioFile f;
	CString str;

	if(f.Open(CONFIG_PATH, CFile::modeCreate|CFile::typeText|CFile::modeWrite))
	{
		str.Format(_T("[CONFIG]\n"));
		f.WriteString(str);
		str.Format(_T("IMAGETYPE=%d\n"),	g_Config.m_nImageType);
		f.WriteString(str);
//		str.Format(_T("SAVETYPE=%d\n"),		g_Config.m_nImageSaveMethod);
//		f.WriteString(str);
		str.Format(_T("SAVECOUNT=%d\n"),	g_Config.m_nImageSaveCount);
		f.WriteString(str);
		str.Format(_T("IMAGEINVERSE=%d\n"), g_Config.m_nImageInverse);
		f.WriteString(str);
		str.Format(_T("MODEPERIOD=%d\n"),	g_Config.m_nModePeriod);
		f.WriteString(str);
		str.Format(_T("COMPORT=%d\n"),		g_Config.m_nComPort);
		f.WriteString(str);

		f.Close();
		ok=0;
	}

#ifdef INTERFACE_NEXTEYE_SERVER	
	if(f.Open(_T("C:\\NEXTEYE\\Serial.ini"), CFile::modeCreate|CFile::typeText|CFile::modeWrite))
#else
	if(f.Open(_T("C:\\COSS\\Serial.ini"), CFile::modeCreate|CFile::typeText|CFile::modeWrite))
#endif
	{
		str.Format(_T("[SETTING]\n"));
		f.WriteString(str);
		str.Format(_T("INDEX=%d\n"),		g_Config.m_nComPort);
		f.WriteString(str);
		f.Close();
	}

	return ok;
}

void LoadDKOffset()
{
#ifdef USE_DAKANO_LINE
	int i;
	CString str;
	int nFlag=0;
	CString sFileName;

	if(g_ParamVer.m_sModelName.Find(_T("ASG5"))>=0 ||   g_ParamVer.m_sModelName.Find(_T("ASG1"))>=0 )
	{
		sFileName=LINEOFFSET_PATH;
	}
	else if(g_ParamVer.m_sModelName.Find(_T("NHP"))>=0 ||  g_ParamVer.m_sModelName.Find(_T("A25"))>=0 || g_ParamVer.m_sModelName.Find(_T("PNH"))>=0 )																																	
	{
		sFileName=LINEOFFSET_PATH1;
	}
	else if(g_ParamVer.m_sModelName.Find(_T("ASG7"))>=0)
	{
		sFileName=LINEOFFSET_PATH2;
	}
	else
	{
		sFileName=LINEOFFSET_PATH3;
	}

	for(i=0;i<128;i++)
	{
		str.Format(_T("OFFSET%d"), i);
		g_System.m_nDKOffsetW[i]=GetPrivateProfileInt(_T("W_OFFSET"),str,	0,	sFileName);
		if(g_System.m_nDKOffsetW[i]<0) g_System.m_nDKOffsetW[i]*=-1;
		if(g_System.m_nDKOffsetW[i]>50)  g_System.m_nDKOffsetW[i]=50;
	}
	for(i=0;i<128;i++)
	{
		str.Format(_T("OFFSET%d"), i);
		g_System.m_nDKOffsetB[i]=GetPrivateProfileInt(_T("B_OFFSET"),str,	0,	sFileName);
		if(g_System.m_nDKOffsetB[i]<0) g_System.m_nDKOffsetB[i]*=-1;
		if(g_System.m_nDKOffsetB[i]>50) g_System.m_nDKOffsetB[i]=50;
	}

	g_System.m_nDKLeftOff	=GetPrivateProfileInt(_T("SIDEOFFSET"),_T("LEFT"),	0,	sFileName);
	if(g_System.m_nDKLeftOff<0) g_System.m_nDKLeftOff=0;
	if(g_System.m_nDKLeftOff>10) g_System.m_nDKLeftOff=10;
	g_System.m_nDKRightOff  =GetPrivateProfileInt(_T("SIDEOFFSET"),_T("RIGHT"),	0,	sFileName);
	if(g_System.m_nDKRightOff<0) g_System.m_nDKRightOff=0;
	if(g_System.m_nDKRightOff>10) g_System.m_nDKRightOff=10;
	
#endif
}

int SaveCurrentLotName()
{
	int ok=-1;
	CStdioFile f;
	CString str;

	if(f.Open(LATEST_LOT_PATH, CFile::modeCreate|CFile::typeText|CFile::modeWrite))
	{
		str.Format(_T("[LOT]\n"));
		f.WriteString(str);
		str.Format(_T("LOTNAME=%s\n"), g_Temp.m_slotName);
		f.WriteString(str);
		f.Close();
		ok=0;
	}
	return ok;
}

CString LoadCurrentLotName()
{
	CString ret;
	TCHAR  sTemp[100];

	GetPrivateProfileString(_T("LOT"),_T("LOTNAME"), _T("NONE"), sTemp, 100, LATEST_LOT_PATH);  
	ret.Format(_T("%s"), sTemp);
	
	return ret;
}

int CossMemoryAlloc()
{
	int i, ok=0;
	g_fmSave	=new BYTE[g_System.m_nImageW* g_System.m_nImageH];

#ifdef WEB_DAKANO_MULTAE
	g_fmSaveComp=new BYTE[g_System.m_nImageW* g_System.m_nImageH/64];
#endif

	g_fmGrab[0]	=new BYTE[g_System.m_nImageW* g_System.m_nImageH];
	g_fmFlat[0]	=new BYTE[g_System.m_nImageW* g_System.m_nImageH];
	g_fmGrabComp=new BYTE[g_System.m_nImageW* g_System.m_nImageH/4];

#ifdef USE_NEXT_GRAB
	g_fmNext	=new BYTE[g_System.m_nImageW* g_System.m_nImageH];

	g_fmSubPrev		=new BYTE[g_System.m_nImageW* SUB_FRAME_HEIGHT];
	g_fmSubPrevFlat =new BYTE[g_System.m_nImageW* SUB_FRAME_HEIGHT];
	g_fmSubNext		=new BYTE[g_System.m_nImageW* SUB_FRAME_HEIGHT];
	g_fmSubNextFlat =new BYTE[g_System.m_nImageW* SUB_FRAME_HEIGHT];
#endif


#ifdef USE_DAKANO_LINE
	g_fmGrabComp1=new BYTE[g_System.m_nImageW* g_System.m_nImageH/8];
#endif

#ifdef USE_HOLE_SKIP
	g_fmGrabComp1=new BYTE[(g_System.m_nImageW/8)*(g_System.m_nImageH/8+2*SUB_FRAME_HEIGHT/8)];
	g_fmHole[0]=new BYTE[(g_System.m_nImageW/8)*(g_System.m_nImageH/8)];
	g_fmHole[1]=new BYTE[(g_System.m_nImageW/8)*(g_System.m_nImageH/8)];
	g_fmHole[2]=new BYTE[(g_System.m_nImageW/8)*(g_System.m_nImageH/8)];
#endif

	g_fmTemp	=new BYTE[g_System.m_nImageW* g_System.m_nImageH];
	g_fmTemp1	=new BYTE[g_System.m_nImageW* g_System.m_nImageH];
	g_Profile	=new int[g_System.m_nImageW];
	g_Profile100=new int[g_System.m_nImageW];
	g_ProfileH  =new int[g_System.m_nImageW/2];
	g_ProfileHor=new int[g_System.m_nImageH];
	g_LUT       =new int[g_System.m_nImageW*g_System.m_nImagePartH];
	g_Temp.fmNGImage	=new BYTE[BAD_IMG_WIDTH*BAD_IMG_HEIGHT];
	g_Temp.fmNGImageLoad=new BYTE[BAD_IMG_WIDTH*BAD_IMG_HEIGHT];

	if(g_fmSave==NULL || g_fmGrab[0]==NULL || g_fmFlat[0]==NULL ||
	   g_fmGrabComp==NULL || 
	   g_fmTemp==NULL || g_Profile==NULL || g_ProfileH==NULL ||
	   g_LUT==NULL || g_Temp.fmNGImage==NULL || g_Temp.fmNGImageLoad==NULL)  ok=-1;

#if PROGRAM_TYPE==GENERAL_TEACH
	for(i=1;i<MAX_IMAGE;i++)
	{
		g_fmGrab[i]	=new BYTE[g_System.m_nImageW* g_System.m_nImageH];
		g_fmFlat[i]	=new BYTE[g_System.m_nImageW* g_System.m_nImageH];
		if(g_fmGrab[i]==NULL || g_fmGrab[i]==NULL) ok=-1;
	}
#endif

#ifdef USE_DAKANO_LINE

	#if DAKANO_WIDTH==64
		g_fmMAP	  =new BYTE[4096*2048];
	#else
		g_fmMAP	  =new BYTE[8192*2048];
	#endif
//	g_fmOldMAP=new BYTE[8192*2048];
#endif

#ifdef USE_SK_BAT
	g_fmBinaryWing	=new BYTE[g_System.m_nImageW/4* g_System.m_nImageH];
#endif

	//PNG color table=======================================================================
	RGBQUAD   m_bmiColors[256];
	 for (i=0; i < 256; i++) 
	 {
        m_bmiColors[i].rgbRed = m_bmiColors[i].rgbGreen = m_bmiColors[i].rgbBlue = (BYTE)i;
        m_bmiColors[i].rgbReserved = 0;
	 }
	 g_Save.Create(g_System.m_nImageW, g_System.m_nImageH,8);
	 g_Save.SetColorTable(0, 256, m_bmiColors);
	 //======================================================================================

#ifdef WEB_DAKANO_MULTAE
	 g_SaveComp.Create(g_System.m_nImageW/8, g_System.m_nImageH/8,8);
	 g_SaveComp.SetColorTable(0, 256, m_bmiColors);
#endif


	 for(i=0;i<MAX_DEFECT+1;i++)
	{
		g_Defect.m_pImage[i]=new BYTE[BAD_IMG_WIDTH*BAD_IMG_HEIGHT];
		if(g_Defect.m_pImage[i]==NULL) ok=-1;
	 }
	 g_DefectTemp.m_pImage=new BYTE[BAD_IMG_WIDTH*BAD_IMG_HEIGHT];

	 InitNGSaveThread(); //NGImage 저장을 위한 데이타 생성

	return ok;
}

void CossMemoryFree()
{
	int i;
	if(g_fmSave)	delete g_fmSave;

#ifdef WEB_DAKANO_MULTAE
	if(g_fmSaveComp) delete g_fmSaveComp;
#endif

	if(g_fmGrab[0]) delete g_fmGrab[0];
	if(g_fmFlat[0]) delete g_fmFlat[0];
	if(g_fmGrabComp)delete g_fmGrabComp;

#ifdef USE_NEXT_GRAB
	if(g_fmNext)    delete g_fmNext;

	if(g_fmSubPrev)		delete g_fmSubPrev;
	if(g_fmSubPrevFlat) delete g_fmSubPrevFlat;
	if(g_fmSubNext)		delete g_fmSubNext;	
	if(g_fmSubNextFlat) delete g_fmSubNextFlat;
#endif

#ifdef USE_DAKANO_LINE
	if(g_fmGrabComp1)delete g_fmGrabComp1;

#endif	

#ifdef USE_HOLE_SKIP
	if(g_fmGrabComp1)delete g_fmGrabComp1;
	if(g_fmHole[0]) delete g_fmHole[0];
	if(g_fmHole[1]) delete g_fmHole[1];
	if(g_fmHole[2]) delete g_fmHole[2];
#endif

	if(g_fmTemp)	delete g_fmTemp;
	if(g_fmTemp1)	delete g_fmTemp1;
	if(g_Profile)   delete g_Profile;
	if(g_Profile100)delete g_Profile100;
	if(g_ProfileH)  delete g_ProfileH;
	if(g_ProfileHor)delete g_ProfileHor;
	if(g_LUT)		delete g_LUT;	
	if(g_Temp.fmNGImage)	 delete g_Temp.fmNGImage;
	if(g_Temp.fmNGImageLoad) delete g_Temp.fmNGImageLoad;

#ifdef USE_SK_BAT
	if(g_fmBinaryWing)		delete g_fmBinaryWing;  
#endif

#ifdef USE_DAKANO_LINE
	if(g_fmMAP)		delete g_fmMAP;  
//	if(g_fmOldMAP)	delete g_fmOldMAP;
#endif

#if PROGRAM_TYPE==GENERAL_TEACH
	for(i=1;i<MAX_IMAGE;i++)
	{
		if(g_fmGrab[i]) delete g_fmGrab[i];
		if(g_fmFlat[i]) delete g_fmFlat[i];
	}
#endif

	for(i=0;i<MAX_DEFECT+1;i++)
		if(g_Defect.m_pImage[i]) delete g_Defect.m_pImage[i];

	if(g_DefectTemp.m_pImage) delete g_DefectTemp.m_pImage;

	ClosePatternData();		//Pattern Close
	CloseOCRData();			//OCR Close

	CloseNGSaveThread();	//NGImage 저장을 위한 데이타 해제
}

void CossMakeDirectory()
{
	CString path;

	CreateDirectory(MODEL_PATH,NULL);		// IMAGE_MODEL_PATH	_T("C:\\Main\\Model\\")
	CreateDirectory(IMAGE_SAVE_PATH,NULL);	// IMAGE_SAVE_PATH	_T("C:\\Main\\Image\\")
	CreateDirectory(LOT_PATH,NULL);			// LOT_PATH			_T("C:\\Main\\LOT\\")
	CreateDirectory(NG_IMAGE_PATH,NULL);	// NG_IMAGE_PATH	_T("C:\\Main\\LOTDATA\\")
	CreateDirectory(LOG_PATH,NULL);			// LOG_PATH			_T("C:\\Main\\SystemLog\\")
	CreateDirectory(RESERVE_PATH,NULL);		// RESERVE_PATH	    _T("C:\\Main\\Reserve\\")
}

void CossMakeDirectoryLot(CString sLotName)
{
	CString path;
	
	CreateDirectory(LOT_PATH+sLotName,		 NULL);	
	CreateDirectory(NG_IMAGE_PATH+sLotName, NULL); 
	path.Format(_T("%s%s\\%s"),NG_IMAGE_PATH, sLotName, g_Temp.m_sMyComName);
	CreateDirectory(path,	 NULL); 

	CStdioFile f;
	CString sFileName;
	sFileName.Format(_T("%s%s\\%s.txt"), LOT_PATH, sLotName,g_Temp.m_sMyComName);
	if(!f.Open(sFileName, CFile::typeText|CFile::modeRead))
	{
		if(f.Open(sFileName,  CFile::modeCreate|CFile::typeText|CFile::modeWrite))
		{
			f.Close();
		}
	}
	else
	{
		f.Close();
	}
}


void CossMakeImageDirectoryLot(CString sLotName)
{
	CreateDirectory(IMAGE_SAVE_PATH+sLotName,NULL);	
}

void ResetCount()
{
	g_Temp.m_nGrabFrame=0;			//Grab한 Frame수
	g_Temp.m_nGrabFrameID=0;		//Grab한 Frame수
	g_Temp.m_nGrabFrameSub=0;		//Grab Sub Frame수(COS7(기포이물 있는것, COS9, COS5, COSA 에서만 사용)
	g_Temp.m_nInspectFrame=0;		//검사한 Frame수
	g_Temp.m_nNGFrame=0;			//NG Frame수
	g_Temp.m_nNGCount=0;			//NG Count

	memset(g_Temp.m_nNGLevel, 0, 10*10*sizeof(int)); //Class별 Level별 Count (아직 사용하는 곳 없음)

#ifdef USE_DAKANO_LINE
	g_Temp.m_nLRSendFrame=0;
	DakanoDataReset();
#endif

	g_Temp.m_GrabNextDelay=0;
}

//Perspective Transform 데이타 만들기
void LoadPerspective()
{
	CString str;
	int nLUTL[2], nLUTR[2];

	//NEL로 읽어보고 -------------------------------------------------------------
	#ifdef USE_HEX_FORMAT
		str.Format(_T("NEL-%X"), g_Temp.m_nPCID);
	#else
		str.Format(_T("NEL-%d"), g_Temp.m_nPCID);
	#endif


	nLUTL[0] = GetPrivateProfileInt(str,_T("LEFT"),			 0,	PROJECTION_PATH);
	nLUTR[0] = GetPrivateProfileInt(str,_T("RIGHT"),		 0,	PROJECTION_PATH);


	//COS로 읽어보고  값이 있는 것으로 사용한다----------------------------------
	#ifdef USE_HEX_FORMAT
		str.Format(_T("COS-%X"), g_Temp.m_nPCID);
	#else
		str.Format(_T("COS-%d"), g_Temp.m_nPCID);
	#endif

	nLUTL[1] = GetPrivateProfileInt(str,_T("LEFT"),			 0,	PROJECTION_PATH);
	nLUTR[1] = GetPrivateProfileInt(str,_T("RIGHT"),		 0,	PROJECTION_PATH);

	if(nLUTL[0])		g_System.m_nLUTL=nLUTL[0];
	else if(nLUTL[1])	g_System.m_nLUTL=nLUTL[1];
	else				g_System.m_nLUTL=0;

	if(nLUTR[0])		g_System.m_nLUTR=nLUTR[0];
	else if(nLUTR[1])	g_System.m_nLUTR=nLUTR[1];
	else				g_System.m_nLUTR=0;

	MakeLUT();
}

void SavePerspective()
{
	CString str;
	CStdioFile   file;
	
	if(file.Open(PROJECTION_PATH,CFile::modeCreate|CFile::modeWrite|CFile::typeText))
	{
		
#ifdef INTERFACE_NEXTEYE_SERVER
		str.Format(_T("[NEL-%d]\n"), g_Temp.m_nPCID); file.WriteString(str);
#else
		str.Format(_T("[COS-%d]\n"), g_Temp.m_nPCID); file.WriteString(str);
#endif
		str.Format(_T("LEFT=%d\n"),  g_System.m_nLUTL); file.WriteString(str);
		str.Format(_T("RIGHT=%d\n"), g_System.m_nLUTR); file.WriteString(str);

		file.Close();
	}
}


//한줄 촬상이 최대 2048x320까지 허용
void MakeLUT()
{
	int i, j, nTmp;
	double dTmp;
	int x1,x2;
	int nHeight=g_System.m_nImagePartH;
	
	g_Temp.m_nMakeLUT=0;
	if(g_System.m_nImagePart<=0)					return;
	if(g_System.m_nLUTL==0 && g_System.m_nLUTR==0)	return;
	if(nHeight==0 || nHeight>320)					return;

	for(i=0;i<nHeight;i++)
	{
		if(g_System.m_nLUTL>0)	 x1= g_System.m_nLUTL-g_System.m_nLUTL*i/nHeight;
		else					 x1=-g_System.m_nLUTL+g_System.m_nLUTL*(nHeight-i)/nHeight;

		if(g_System.m_nLUTR>0)   x2= g_System.m_nLUTR-g_System.m_nLUTR*i/nHeight;
		else				     x2=-g_System.m_nLUTR+g_System.m_nLUTR*(nHeight-i)/nHeight;

		for(j=0;j<g_System.m_nImageW;j++)
		{	
			dTmp=x1+(double)(g_System.m_nImageW-x1-x2)*j/g_System.m_nImageW;
			
			if(dTmp<0) dTmp=0;
			else if(dTmp>=g_System.m_nImageW) dTmp=g_System.m_nImageW-1;

			*(g_LUT+g_System.m_nImageW*i+j)=(int)(dTmp*1000);
		}
	}
	g_Temp.m_nMakeLUT=1;
}

void MakeDefectLUT(int nBright, int nContrast)
{
    int i,temp, offset;              

    int H_x,L_x,Data[256];

    H_x=255+(128-nContrast);
    L_x=(nContrast-128);
    
    if(L_x>0){
      for(i=0;i<L_x;i++) Data[i]=0;
      for(i=L_x;i<H_x;i++) Data[i]=(int)(255.0/(float)(H_x-L_x)*(i-L_x));
      for(i=H_x;i<256;i++) Data[i]=255;
    }
    else  for(i=0;i<256;i++) Data[i]=(int)(255.0/(float)(H_x-L_x)*(i-L_x));

    if(nContrast<128)
     {
		 for(i=0;i<256;i++)
		{
			 temp= Data[i]+(nBright-128);
			 if(temp>255) temp=255;
			 else if (temp<0) temp=0;
			 g_DefectLUT[i]=temp;
		}
	}
    else
    {  
	   offset=128-nBright;
       if(offset>0)
	   {  
			 for(i=0;i<L_x+offset;i++) 
				 if(i<256) g_DefectLUT[i]=0;

			 for(i= L_x+offset;i<=H_x+offset;i++) 
				if(i<256) g_DefectLUT[i]=Data[i-offset];

			 for(i=H_x+offset;i<256;i++) g_DefectLUT[i]=255;    
       }
       else
	   {
			 for(i=0;i<L_x+offset;i++) g_DefectLUT[i]=0;
			 temp=0;
			 for(i= L_x+offset;i<=H_x+offset;i++) 
			 {  
				if(i>=0) g_DefectLUT[i]=Data[L_x+temp];
				temp++;
			 }   
			 for(i=H_x+offset;i<256;i++) g_DefectLUT[i]=255;    
       }
    }
}

/* 월별로 저장
void WriteLog(CString str)
{
	CStdioFile f;
	int nMonth, nPrevMonth;
	CTime time = CTime::GetCurrentTime();
	CString sDate, sFileName;
	
	nMonth=time.GetMonth();
	nPrevMonth=nMonth-2;
	if(nPrevMonth<=0) nPrevMonth=12-nPrevMonth;

	//두달전 로그 지운다.
	sFileName.Format(_T("%sMainLog_%d.txt"), MAIN_FOLDER, nPrevMonth);
	if(f.Open(sFileName,  CFile::typeText|CFile::modeRead))
	{
		f.Close();
		DeleteFile(sFileName);
	}

	//이번달 로그 쓴다------------------------------------------------------------------------------
	sFileName.Format(_T("%sMainLog_%d.txt"), MAIN_FOLDER, nMonth);
	if(f.Open(sFileName,  CFile::modeCreate|CFile::modeNoTruncate|CFile::typeText|CFile::modeWrite))
	{
		sDate.Format(_T("\n%d/%d %02d:%02d:%02d %s"), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), str);
		f.SeekToEnd();
		f.WriteString(sDate);
		f.Close();
	}	
}
*/
/*
//일별로 저장
void WriteLog(CString str)
{
	CStdioFile f;
	int nDay, nMonth, nNextDay;
	static int nNextDayOld=0;
	CTime time = CTime::GetCurrentTime();
	CString sDate, sFileName;

	//내일 로그 지운다. ------------------------------------------------
	nMonth=time.GetMonth();
	nDay=time.GetDay();
	nNextDay=nDay+1;
	if(nMonth==1 || nMonth==3 || nMonth==5 || nMonth==7 || nMonth==8 || nMonth==10 || nMonth==12)
	{
		if(nNextDay>31) nNextDay=1;
	}
	else if(nMonth==2)
	{
		if(nNextDay>28) nNextDay=1;
	}
	else
	{
		if(nNextDay>30) nNextDay=1;
	}

	if(nNextDayOld!=nNextDay)
	{		
		sFileName.Format(_T("%sMainLog_%d.txt"), MAIN_FOLDER, nNextDay);
		if(f.Open(sFileName,  CFile::typeText|CFile::modeRead))
		{
			f.Close();
			DeleteFile(sFileName);
		}
		nNextDayOld=nNextDay;
	}
	//------------------------------------------------------------------

	//이번달 로그 쓴다------------------------------------------------------------------------------
	sFileName.Format(_T("%sMainLog_%d.txt"), MAIN_FOLDER, nDay);
	if(f.Open(sFileName,  CFile::modeCreate|CFile::modeNoTruncate|CFile::typeText|CFile::modeWrite))
	{
		sDate.Format(_T("\n%02d:%02d:%02d %s"), time.GetHour(), time.GetMinute(), time.GetSecond(), str);
		f.SeekToEnd();
		f.WriteString(sDate);
		f.Close();
	}	
}
*/
void WriteLog(CString str)
{
	CStdioFile f;
	int nDay, ret, nCreateFile=0;
	static int nDayOld=0;
	CTime time = CTime::GetCurrentTime();
	CString sDate, sFileName;

	//날짜가 바뀌었으면 새로 씀.
	nDay=time.GetDay();
	if(nDayOld>0 && nDayOld!=nDay)
	{
		nCreateFile=1;
	}
	nDayOld=nDay;
	//------------------------------------------------------------------

	//이번달 로그 쓴다------------------------------------------------------------------------------
	sFileName.Format(_T("%sDay_%d.txt"), LOG_PATH, nDay);
	if(nCreateFile) ret=f.Open(sFileName,  CFile::modeCreate|CFile::typeText|CFile::modeWrite);
	else			ret=f.Open(sFileName,  CFile::modeCreate|CFile::modeNoTruncate|CFile::typeText|CFile::modeWrite);
	if(ret)
	{
		if(nCreateFile)
		{
			sDate.Format(_T("%02d:%02d:%02d %s"), time.GetHour(), time.GetMinute(), time.GetSecond(), str);
		}
		else
		{
			sDate.Format(_T("\n%02d:%02d:%02d %s"), time.GetHour(), time.GetMinute(), time.GetSecond(), str);
			f.SeekToEnd();
		}
		f.WriteString(sDate);
		f.Close();
	}	
}

int GetMarkOptic()
{
	int nOptic=0;

#if defined INTERFACE_NEXTEYE_SERVER  ||  defined MARKING_ORDER_OLD_VERSION
	switch(g_Temp.m_nPCOptic)
	{
		case COS1: nOptic=COS1_NE; break;
		case COS2: nOptic=COS2_NE; break;
		case COS3: nOptic=COS3_NE; break;
		case COS4: nOptic=COS4_NE; break;
		case COS5: nOptic=COS5_NE; break;
		case COS6: nOptic=COS6_NE; break;
		case COS7: nOptic=COS7_NE; break;
		case COS8: nOptic=COS8_NE; break;
		case COS9: nOptic=COS9_NE; break;
		case COSA: nOptic=COSA_NE; break;
		case COSB: nOptic=COSB_NE; break;
		case COSC: nOptic=COSC_NE; break;
		case COSD: nOptic=COSD_NE; break;
	}
#else
	nOptic=g_Temp.m_nPCOptic-1;
#endif

	return nOptic;
}

//맨처음 한번 bMakeFile=true 해서 Call
//이후 bMakeFile=false 해서 Call
void WriteDebugLog(bool bMakeFile, CString str)
{
	CStdioFile f;
	static CString sLofTFileName=_T("");

	if(bMakeFile)
	{
		CTime time = CTime::GetCurrentTime();
#ifdef INTERFACE_NEXTEYE_SERVER	
		sLofTFileName = time.Format("C:\\Nexteye\\Log\\%Y%m%d_%H%M%S.txt");	
#else
		sLofTFileName = time.Format("C:\\COSS\\Log\\%Y%m%d_%H%M%S.txt");	
#endif

		if(f.Open(sLofTFileName,  CFile::modeCreate|CFile::typeText|CFile::modeWrite))
		{
			f.WriteString(str+"\n");
			f.Close();
		}
	}
	else
	{
		if(f.Open(sLofTFileName,  CFile::typeText|CFile::modeWrite))
		{
			f.SeekToEnd();
			f.WriteString(str);
			f.Close();
		}
	}
}

#ifdef BARCODE_VISION
// Defect Data는 추후 수정해야함.. 완료 안되었음....
void WriteBarcodeInfo(CString strMsg, CString strIdx, int nRst, int nframe)
{
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("%Y_%m_%d");//ttime.Format("%H_%M_%S");
	CString strTime = ttime.Format("[%Y %m %d]");
	CStdioFile   file;
	CString filename;
	CString strpath;
	CString str;
	CString strHeader;
	strHeader.Format(_T("\t검사시간\tFrame\tYposistion(mm)\t---Barcode No---\t처리\t--너비--\t--높이--"));
	SYSTEMTIME st;
	CString strTimeS;
	//GetSystemTime(&st);
	GetLocalTime(&st);
	strTimeS.Format(_T("[%02d:%02d:%02d]"), st.wHour, st.wMinute, st.wSecond);

	//	strpath.Format("C:\\NEXTEYE\\Log\\%s\\%s\\",kk,pDoc->m_str_lotFull);
	strpath.Format(_T("%s%s\\"), LOT_PATH, g_Temp.m_slotName);
	if (GetFileAttributes(strpath) == -1)
	{
		CreateDirectory(strpath, NULL);
	}

	filename.Format(_T("%s%s_BcdDetailInfo_%s.txt"), strpath, g_Temp.m_sMyComName, strMsg.Left(11));
	CRect bcrRect;

	if (nRst == 1)
	{
		bcrRect = g_Temp.m_BcrRectCodeRead;
		str.Format(_T("%s%s\t%d\t%f\t%s\t%s\tWidth:%d\tHeight:%d"), strTime, strTimeS, nframe, g_Defect.m_Defect[0].y_pos, strMsg, strIdx, bcrRect.Width(), bcrRect.Height());
	}
	else if (nRst == 2)
	{
		bcrRect = g_Temp.m_BcrRectFine;
		str.Format(_T("%s%s\t%d\t%f\t%s\t%s\tWidth:%d\tHeight:%d"), strTime, strTimeS, nframe, g_Defect.m_Defect[0].y_pos, strMsg, strIdx, bcrRect.Width(), bcrRect.Height());
	}
	else if (nRst == 10)
	{
		str.Format(_T("Missing Barcode"));
	}
	else
	{
		bcrRect = g_Temp.m_BcrRectFine;
		str.Format(_T("%s%s\t%d\t%f\t%s\t%s\tWidth:%d\tHeight:%d"), strTime, strTimeS, nframe, g_Defect.m_Defect[0].y_pos, strMsg, strIdx, bcrRect.Width(), bcrRect.Height());
	}

	if (!file.Open(filename, CFile::modeReadWrite | CFile::typeText))
	{
		if (file.Open(filename, CFile::modeCreate | CFile::modeReadWrite | CFile::typeText))
		{
			file.WriteString(strHeader);
			file.SeekToEnd();
			file.WriteString(_T("\n"));
			file.WriteString(str);
			file.Close();
		}
	}
	else
	{
		file.SeekToEnd();
		file.WriteString(_T("\n"));
		file.WriteString(str);
		file.Close();
	}
}

void WriteBcrDefectLog(CString path, CString str)
{
	CStdioFile f;
	int nDay, ret, nCreateFile = 0;
	CTime time = CTime::GetCurrentTime();
	CString sDate;

	ret = f.Open(path, CFile::modeCreate | CFile::modeNoTruncate | CFile::typeText | CFile::modeWrite);
	if (ret)
	{
		sDate.Format(_T("\n%02d:%02d:%02d %s"), time.GetHour(), time.GetMinute(), time.GetSecond(), str);
		f.SeekToEnd();
		f.WriteString(sDate);
		f.Close();
	}
}

void WriteBcrEdgeLog(float edgePos)
{
	CStdioFile   file;
	CString filename, tmp;

	CreateDirectory(BCR_EDGE_PATH, NULL);
	filename.Format(_T("%sEdge_Info.txt"), BCR_EDGE_PATH);

	if (file.Open(filename, CFile::modeCreate | CFile::modeReadWrite | CFile::typeText))
	{
		tmp.Format(_T("%f"), edgePos);
		file.WriteString(tmp);
		file.Close();
	}
}
#endif