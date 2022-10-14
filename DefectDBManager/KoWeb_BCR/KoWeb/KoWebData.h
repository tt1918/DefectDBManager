


typedef struct SYSTEMDATA_
{
	int m_nImageW;				//이미지 정보  m_nImageW와 m_nPitch는 항상 같다.
	int m_nImageH;
	int m_nPitch;
	int m_nImagePart;			//이미지를 나눠서 촬영하는 경우 한 Frame에 포함되는 SubImage 수
	int m_nImagePartH;			//Sub이미지 높이
	int m_nGrabBright;			//기준 Grab평균밝기
	int m_nStartGrayDiff;		//자동밝기시 평균밝기가 이 값만큼 차이나면 자동밝기 시작
	int m_nTargetGrayDiff;		//이 값 이하로 들어오면 자동밝기 STOP
	int m_nAutoExpoStartCnt;	//자동밝기 시작을 위해 밝기차이가 연속으로 N 번 발생시 자동밝기 시작
	int m_nAutoExpoEndCnt;		//자동밝기 시도 횟수 (최대시도 횟수)
	int m_nAutoExpoMinValue;	//자동밝기 최소 단위
	int m_nGrabBrightBK;		//기준 Grab평균밝기백업
	int m_nFlatBright;			//평활화 했을떄 밝기
	int m_nMaxExposure;			//최대 Exposure
	int m_nMinExposure;			//최소 Exposure;
	int m_nBigDefect;			//4배 압축한 이미지에서 불량 검출

	int m_nServerTest;			//1이면서버테스트
	int m_nMarkServerTest[2];	//1이면서버테스트
	int m_nVisionBoardType;		//0이면 Solios   1이면 Radient
	int m_nServerType;			//0이면 100.0.0.254 253,252   1이면 100.0.0.244, 243, 242
	int m_nLineCAM;				//0 : AREA    1: Line Scan  Camera
	int m_nCAMAngle;			//카메라 각도
	int m_nGrabber;				//0 : Matrox  1: Basler GigE
	int m_nMaxDefect;			//한 Frame에서 검출할 최대 불량수
	int m_nMaxCandiDefect;		//한 Frame에서 후보 수
	int m_nMarkServerCnt;		//마킹서버 수
	int m_nMarkDataUSE;			//파라미터에 마킹정보 있고 그것을 사용한다면 1
	int m_nResultDaley;			//결과를 몇번 Delay해서 보낼것인지

	int m_nPerspectiveTR;		//투영, LR처럼 Perspective Trasform 하면 1 안하면 0
	int m_nLUTL;				//Perspective Trasform에 사용되는 왼쪽 데이타
	int m_nLUTR;				//Perspective Trasform에 사용되는 오른쪽 데이타
	int m_nDefectApplyLUT;		//결과이미지에 LUT적용
	int m_nBrightLUT;			//Vmaster에서의 Bright LUT
	int m_nContrastLUT;			//VMaster에서의 Contrast LUT

	int m_nResultSendMethod;	//1이면 검사결과나오면 바로, 0이면 다음 Frame촬상후

	int m_nBDOverkill;			//경계투과 또는 경계반사의 울렁거리는 과검제거하려면 1
	int m_nCandiRect;			//후보 격자크기
	int m_nOverlapX;			//Overlap Check X Range
	int m_nOverlapY;			//Overlap Check Y Range

	double m_dPeriodLimitX;		//주기 X 오차 (+/-) mm
	double m_dPeriodLimitY;     //주기 Y 오차 (+/-) mm

	TCHAR  m_sAIIP[20];			//AI IP Address
	int    m_nAIPort;			//AI 서버 Port
	int    m_nMakePyramid;		//영상이 너무커서 압축해서 사용함

	int    m_nSimulServer;		//서버와 연동해서 Simulation

	//------------------------------------------------------------------------------------------------------------
	int    m_nUseCamTapeSensor;	//카메라를 Tape 센서로 이용하고자 하면 1로 세팅한다.
	int    m_nTapeThreshold;	//Tape Threshold(한 Frame의 상하 10 Pixel Line의 평균값을 비교하여 Tape유무 판정)
	int    m_nTapeNotArea;		//Tape 센싱 하는데 사용하지 않는 영역
	//------------------------------------------------------------------------------------------------------------

#ifdef USE_DAKANO_LINE
	int    m_nDKOffsetW[128];	//DK Offset
	int    m_nDKOffsetB[128];	//DK Offset
	int    m_nDKLeftOff;		//128개중 왼쪽 검사안하는 갯수
	int    m_nDKRightOff;		//128개중 오른쪽 검사 안하는 갯수
#endif


}SYSTEMDATA;

typedef struct CONFIGDATA_
{
	int m_nImageType;			//0: BMP	 1:PNG		2:JPG
	int m_nImageSaveMethod;     //0: NoSave  1:NGSave   2:AllSave
	int m_nImageSaveCount;		//한번에 저장할 Frame 수
	int m_nImageInverse;
	int m_nShowProfile;			//Profile 보이게 함
	int m_nShowCalibWidth;
	double m_dCalibWidth;
	double m_dCalibStartX;
	int m_nModePeriod;
	int m_nComPort;

}CONFIGDATA;

typedef struct DEFECTDATA_
{
	int		m_nFrameNum;
	int		m_nDefectCount;
	RECT    m_rcDefect[MAX_DEFECT+1];
	DEFECT	m_Defect[MAX_DEFECT+1];	
	LPBYTE  m_pImage[MAX_DEFECT+1];

#ifdef BARCODE_VISION
	int					m_nBcrCount;
	int					m_nBcrAreaDefectCount;
	int					m_nBcrDefectCount;
	BCR_DEFECT			m_BMarkDefect;
	AREA_MARK_DEFECT	m_BcrAreaDefect[MAX_DEFECT];
	MARK_DEFECT			m_BcrDefect[MAX_DEFECT];
#endif
}DEFECTDATA;

typedef struct DEFECTTEMP_
{
	int		m_nFrameNum;
	int		m_nDefectCount;
	RECT    m_rcDefect;
	DEFECT	m_Defect;	
	LPBYTE  m_pImage;

}DEFECTTEMP;

typedef struct CANDIPOINTS_
{
	int m_nCandiCount;
	int m_nCandiCountX;
	int m_nCandiCountY;
	int m_nCandiValue[1024];	//(8192/256)=32  32x32 = 1024   최대이미지크기 8192x8192 임.
	int m_nCandiX[1024];
	int m_nCandiY[1024];
	int m_nCandiAvg[1024];
}CANDIPOINTS;


//투영에 사용
typedef struct CANDIDATA_
{
	//int nMaxX;
	//int nMaxY;
	//int nMaxValue;
	//int nMinX;
	//int nMinY;
	//int nMinValue;

	int nAreaMaxX;
	int nAreaMaxY;
	double dAreaMaxValue;
	int nAreaMinX;
	int nAreaMinY;
	double dAreaMinValue;
}CANDIDATA;

typedef struct FRAMEINFO_
{
	double m_dEdge;
	int    m_nMax;
	int    m_nMin;
	double m_dAveAllBright;
	double m_dAvgROIBright;
	double m_dSharpness;
	double m_dFPS;
	double m_dHDD;
	double m_dInspectTime;


}FRAMEINFO;

typedef struct TEMPDATA_
{
	int m_nLatestResult;
	int m_nInspectWhite;		//백점검사
	int m_nInspectBlack;		//흑점검사
	int m_nScratchInspect;		//스크라치 검사
	int m_nLongScratchInspect;	//Long스크라치 검사
	int m_nLRInspect;			//LR(꼽침)검사
	int m_nGroupInspect;		//군집검사
	int m_nMarkingInspect;		//정코마킹검사
	int m_nChangeExpoFrameID;	//노출이 바뀐 Frame
	int m_nUsePrevImage;		//직전 Frame과 밝기차가 거의 없어서 연결해서 사용
	double m_dFPS;				//초당 Frame수

	//TPW Hole ------------------------------------------
	int m_nHoleCheck;			//홀찾아서 지움
	int m_nHoleThres;			//Hole 찾을 TH
	double m_dHolePeriod;		//Hole주기	
	int m_nHoleFrame;			//Hole검출된 Frame
	int m_nHoleXpos;			//Hole검출된 X Pixel
	int m_nHoleYpos;			//Hole검출된 Y Pixel
	//----------------------------------------------------

	int m_nReWinder;			//리와인더 신호 들어옴
	int m_nRWBitNumber;			//리와인더 신호 IO번호 0부터 시작함
	int m_nMotionError;			//모션보드 Open실패

	int m_nPGStart;				//프로그램이 켜지면 1
	TCHAR m_nPCFullName[8];		//"AAA-405"같이 7자리
	int m_nPCID;				//902, 405 이런 3자리 수
	int m_nPCOptic;				//907이면 9, 103이면 1, 405면 4 (어떤 서버를 쓰느냐에 따라 달라짐)
	int m_nPCFirstNo;			//COS번호
	int m_nPCNum;				//907이면 7, 103이면 3, 405면 5
	int m_nSelfPort;			//자신의 포트 1~250
	int m_nSaveNumberID;		//Save ID
	int m_nSaveCount;
	int m_nSaveCountLot;		//Lot바뀔때 까지 저장
	int m_nConnectAIServer;		//1이면 Connected 0이면 Disconnected

	int m_nFrameDiff;			//결과보낼때 Frame차이
	int m_nFrameDiffSum[2];		//디버깅 용

	//Grab관련
	int m_bAuto;				//1이면 검사중
	int m_bLive;				//1이면 Live영상

	int m_nONInspecting;		//검사중이면 1
	int m_nSaveOneFrame;		//1이면 한 Frame저장

	//GUI
	int m_nBinaryDialogOpen;	//Binary Dialog가 있으면 1 

	//노출변경
	int m_nApplyExposure;		//1이면 Exposure를 적용한다.
	int m_nExposureData;		//노출시간 (usec)

	int m_nAutoExpoStart;		//자동밝기 시작

	
	//검사영역
	int m_nFoundEdge;			//구한Edge
	int m_nFoundEdgeDelay[3];	//구한Edge
	int m_nEdgeType;			//0:안쪽 1:최외곽
	int m_nEdgeDir;				//0: 왼쪽  1: 오른쪽 
	int m_nInspectX1;			
	int m_nInspectX2;	
	int m_nFlatSuccess;			//Flat성공
	int m_nMakePyramidDone;
	int m_nInspectEdgeX1;			
	int m_nInspectEdgeX2;	

	//COUNT-----------------------------------------------------------
	int m_nGrabDone;			//Grab완료되면 1
	int m_nGrabOne;				//1이면 한번만 Grab하고 Stop하란 것
	int m_nGrabFrame;			//Grab한 Frame수
	int m_nGrabFrameID;			//Grab한 Frame ID
	int m_nGrabFrameSub;		//1/16 짜리 한Frame증가할때
	int m_nFrameAfterTape;		//Tape신호 후 Frame수
	int m_nInspectFrame;		//검사한 Frame수
	int m_nNGFrame;				//NG Frame수
	int m_nNGCount;				//NG Count
	int m_nNGLevel[10][10];		//Class별 Level별 Count

	//화면표시데이타--------------------------------------------------
	double m_dInspTime;			//검사시간 msec
	double m_dMaxInspTime;      //최대 검사 시간
	double m_dMinInspTime;      //최대 검사 시간
	double m_dAverageBright;	//평균밝기
	double m_dAverageBrightForExpo;	//평균밝기
	double m_dAverageSharp;		//평균Sharpness
	double m_dHDDspace;			//C: HDD남은 용량(%)  20%이하면 Save중단
	double m_dAverageBrightDetail[3];			//평균Sharpness
	double m_dAverageSharpDetail[3];			//평균Sharpness

	//
	int m_nNBOImage;			//NBO이미지는 평활화 다르게 한다.
	
	int m_nImageCopy;			//이미지를 복사함.
	LPBYTE m_fmImageScr;
	LPBYTE m_fmImageDst;
	int    m_nImageSize;

	//군집관련
	int m_nKoonJipCountInArea;	//영역내 최고 갯수
	int m_nKoonJinCountInImage; //이미지내 군집후보 갯수

	//정코마킹검출
	double m_dMaxMark;

	//기포라인
	int m_nMaxLinekipo;
	int m_nMaxLinekipoPos;

	//세로선얼룩
	int m_nMaxMultiVerLineValue;

	int m_nFrameInfoSendCount;	//서버에 한번씩 Frame데이타를 보내는 주기 (대략 500mm에 한번)
	int m_nImageInfoCount;		//이미지 정보
	int m_nImageInfoSendCount;  //서버에 이미지 정보를 보내주는 주기(대략 750mm에 한번)

	int m_nGroupMaxCount;			//군집최대치

	int m_bDefectExist[2];		//RED(1), BLUE(1) 불량이 있으면 1
	//사용하는데 없음----------------------------------
	int m_LoadSizeX;			//load한 이미지 크기
	int m_LoadSizeY;            //

	int     m_nCycleCnt;
	double	m_dCycleX[CYCLELENGTH];
	double	m_dCycleY[CYCLELENGTH];
	int		m_nDefaultCycle[MAX_CYCLE_COUNT];
	int		m_nDefectFlag[10][MAX_CYCLE_COUNT];
	double	m_dCyclePrevY[MAX_CYCLE_COUNT];

	int m_nLotChangeSignal;	   //이신호가 오면 LOT 바뀜.
	int m_nLotChangeFrame;	   //m_nLotChangeSignal 왔을때 Frame수.
	int m_nLotChangeFMCount;   //m_nLotChangeSignal오면 0

	int m_nTapeSignal[3];	   //보호[1], 이형[2]
	int m_nTapeInFrame[3];	   //Tape들어올때 Frame수

	//Spot검사 
	int   m_nMaxGray;
	int   m_nMinGray;

	//얼룩검사
	double m_dMuraMaxValue;		//얼룩최대값(백)
	int    m_nMuraMaxPosX;
	int    m_nMuraMaxPosY;
	double m_dMuraMaxValueB;	//얼룩최대값(흑)
	int    m_nMuraMaxPosXB;
	int    m_nMuraMaxPosYB;

	//쿠닉검사
	double m_dCunicCanicMax;	//쿠닉 후보 최고값
	int    m_nCunicValue;

	//LR
	int m_nMaxLRValue;			//LR최고값
	int m_nMaxLRPosX;			//LR최고값 발생위치
	int m_nMaxLRWValue;			//LR(W)최고값
	int m_nMaxLRWPosX;			//LR(W)최고값 발생위치

	int m_nCkTape;				//Tape신호 들어오면 1,2,3바뀜
	int m_nCkSplice;			//2동,3동에서 Tape신호
	int m_nCkSpliceCount;		//3동에서 보내오는 Tape신호

	int m_nDayChange;			//바뀐날자 보관
	int m_nMonthChange;			//바뀐달 보관

	int m_nAutoAdjustStart;		//자동밝기조정시작

	int m_nErrorCode;			//1이면 밝기가 검사범위 벗어남.

	//투영검사
	int m_nMaxLineValue;
	int m_nMaxLineValueTD;
	int m_nMaxAngleLineValue;
	int m_nMaxLinePos;
	int m_nMaxLinePosTD;
	int m_nMaxAngleLinePos;
	int	   m_nMountainCnt[5];	//산 갯수

	//수직라인검사
	int m_nMaxWLineValue;
	int m_nMaxBLineValue;
	int m_nMaxWLineValuePos;
	int m_nMaxBLineValuePos;

	//검사중 나온데이타 표시
	int m_MaxScratch;			//스크라치 검사중 나온 최대 스크라치 값
	int m_MaxLongScratch;		//Long스크라치 검사중 나온 최대 스크라치 값
	int m_MaxLongScratchPos;	//Long스크라치 검사중 나온 최대 스크라치 값
	int m_AveScratch;

	//PATTERN 
	int m_nLastPatternID;

	//LUT
	int m_nMakeLUT;			//LUT를 만들었으면 1

	//GAIN 변경
	int m_FlagGainChange;	//1이면 Grab Stop후 Gain변경

	//Serial Rotation Motion
	int m_nResetFlag;
	int m_nMotionExist;		//모션이 있으면 TRUE
	int m_nMotionCheck;		//true되면 모션 찾음
	int m_nManualROT;		//모션 수동 설정이면 1
	int m_dMotionTarget;
	int m_dMotionPos;
	int m_nMotionCount;
	int m_dMotionBrightness;//현재 모션위치에서 평균 밝기값


	int  m_FlagNGSave;
	LPBYTE fmNGImage;		//[BAD_IMG_WIDTH*BAD_IMG_HEIGHT]; //가장 최근 불량영상 
	LPBYTE fmNGImageLoad;	//[BAD_IMG_WIDTH*BAD_IMG_HEIGHT]; //Load한 불량 

	CString m_sMyComName;
	CString m_slotName;		//Model Name(공통으로 사용)
	CString m_sNewlotName;

	CTime m_tLastSendTime;
	CTime m_tTimeCheckFrame;
	CTime m_tTimeCheckFrameOld;

	//투영 라인 Thread에사용되는 변수
	int m_TooX[10];
	int m_TooValue[10];
	int m_ToonAngle[10];
	int m_ToonOverKill[10];

	int m_TooTDX[25][8];
	int m_TooTDValue[25][8];

	//DAKANO
	int  m_nLRSendFrame;
	int  m_nLRSendFrameDiff;
	int  m_nMapLineCnt;
	int  m_nMapLineCntDiff;
	int  m_bShowMap;	//화면에 MAP표시
	int  m_bShowOldMap;
	int  m_nPortOpenCount;
	int  m_nESCRetry;		


	//물때 이미지 저장용
	int  m_nNG_MulTae;					//물때 불량 발생함
	int  m_nSaveNumberID_CompImage;		//Save ID (1/8압축)

	//SIMUL사용
	int  m_nReceiveServerFrame;		//서버로 부터 다음 Frame 하라고 신호받음
	bool m_bTestSyncRecieved;		//서버에서 


	//정코 COS1에서 사용하는 변수 >>>>>>>>>>>>>
	int m_nLeading;
	int m_nOldLeading;
	int m_nNEL2NotInspAreaX1;
	int m_nNEL2NotInspAreaX2;
	//Mark검출
	int  m_nMarkValueMax;
	int	 m_dMarkValueArray[100];
	int  m_nMarkValueCount;
	int  m_MarkValueThreshold;

	double m_dMaxStdev;
	int  m_nMaxTDValue;
	int  m_nMaxTDPosX;
	int  m_nMaxTDPosY;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	int  m_nSetBaseLine;
	int  m_nReadyBaseLine;
	
	int  m_GrabNextDelay;
#ifdef USE_SK_BAT
	int  m_nSideAvg;
	int  m_nSideBlackMin;
	double   m_dInnerEdge;		//안쪽Edge
	double   m_dOuterEdge;		//바깥쪽Edge	
#endif


	int m_nKnullingCheck;		//Knulling Check했으면 1
	int m_nKnullingValue;		//TAC_LINE에서 Knull값 구한것
	double m_dNullingRatio;   
	int m_nFoundKnulling;		//Knulling찾음 1 못찾음 0

	int m_nMuraMax;
	int m_nTestCnt[10];

	///////////////////////////////////////////////////////////////////////
	// BCR 용 - S
	bool	m_isBcrFirstCheck;	// 검사 시작 후 제일 처음 
	bool	m_isBcrInitRead;	// 바코드를 2번 이상 읽었을 때 초기화 처리

	int		m_nBcrPreEdge;		// 이전 프레임 에지 위치
	CRect	m_BcrRect;			// 1차 영역 추출
	CRect	m_BcrRectFine;		// 후처리 영역 추출
	CRect	m_BcrSavingRect;	// BCR 이미지 저장용
	CRect	m_BcrRectForMatch;	// BCR 확인 실패 시 중심 처리용
	CRect   m_BcrRectMatched;	// 패턴 매칭된 BCR 영역
	CRect	m_BcrRectCodeRead;	// CodeReader에 의해 획득한 중심점

	CPoint  m_BcrCenterPt;		// 바코드 중심점

	bool	m_isBcrSuccessRead;	// 바코드 인식 결과
	int		m_nBcrPatFind;		// 바코드 탐색 결과 순번
	bool	m_isBcrFirstCode;	// 최초 BCR 리딩
	int		m_nBcrFirstRead;	// BCR 리딩 상황
	int		m_nBcrDir;			// BCR 리딩 방향

	bool	m_bBcrForceInsert;  // BCR 강제 입력
	CString m_strBcrForceData;  // BCR 강제 입력 데이터
	bool	m_bBcrForceDir;		// BCR 강제 입력시 Dir
	bool	m_isBcrForceReading; // BCR 강제 입력 처리 확인
	double  m_dBCRForceREadingDist; // BCR 강제 입력 거리


	CString m_strBcrName;		// 인식 결과 바코드 정보
	int		m_nBcrPreInspFrame;	// 이전 BCR 인식된 Frame 번호
	CString m_strPreBcrName;	// 이전 인식 결과 바코드 정보
	double  m_dBcrCrtRealPos;		// 원단상의 실제 위치
	double  m_dBcrPreRealPos;	// 이전 BCR 원단상의 실제 위치
	double  m_dBcrOffsetY;		// 프레임 내에서 바코드 위치에 따른 OFFSET
	double  m_dBcrScale[3];		// Bcr 길이 방향 Scale 계산		

	int		m_nBcrReadOK;		// BCR 인식 갯수

	int		m_nBcrNoReadWarning;
	int		m_nBcrNoReadError;

	int		m_nBcrScaleIdx;		// Bcr Y scale 확인용
	int		m_nBcrOrder;		// Bcr order 확인용
	double  m_dBcrPreFramePos;	// 

	char	m_cBcrFileName[MAX_BADIMAGE_FILENAME];		// BCR 조각 영상 이름

	CString m_strBcrLog;		// BCR 로그 저장용
	// BCR 용 - E
	///////////////////////////////////////////////////////////////////////
}TEMPDATA;


extern int        g_ID;			//현재 검사할 이미지ID  0, 1 둘중한개
extern LPBYTE     g_fmGrab[MAX_IMAGE];	//Grab이미지 (원본)

extern LPBYTE     g_fmNext;				//검사할Frame의 다음 Frame
extern LPBYTE     g_fmSubPrev;			//검사할 Frame의 직전 128 Pixel
extern LPBYTE     g_fmSubNext;			//검사할 Frame의 다음 128 Pixel
extern LPBYTE     g_fmSubPrevFlat;		//검사할 Frame의 직전 128 Pixel
extern LPBYTE     g_fmSubNextFlat;		//검사할 Frame의 다음 128 Pixel

extern LPBYTE     g_fmFlat[MAX_IMAGE];	//평활화된 이미지
extern LPBYTE     g_fmGrabComp;	//Grab압축영상(1/2)
extern LPBYTE     g_fmGrabComp1;//Grab압축영상(W/2 H/4)
extern LPBYTE     g_fmHole[3];	//Hole Frame전체이미지저장
extern LPBYTE     g_fmSave;		//Save할 이미지는 여기 복사
extern LPBYTE     g_fmSaveComp;	//Save할 이미지는 여기 복사 (물때 압축이미지)
extern LPBYTE     g_fmTemp;		//검사중 각종 용도로 사용
extern LPBYTE     g_fmTemp1;	//검사중 각종 용도로 사용
extern LPBYTE	  g_fmMAP;
//extern LPBYTE	  g_fmOldMAP;

extern LPBYTE	  g_fmBinaryWing;

extern int*       g_Profile;	//프로파일 정보
extern int*       g_Profile100; //프로파일 정보(100배)
extern int*       g_ProfileH;	//프로파일 정보(W의 반)
extern int*       g_ProfileHor; //수평 Profile
extern int*		  g_LUT;        //투영 LUT
extern int        g_DefectLUT[256]; //Defect영상을 위한 LUT

extern BYTE       g_fmDefectTemp[BAD_IMG_WIDTH*BAD_IMG_HEIGHT];
extern BYTE       g_fmDefectGray[BAD_IMG_WIDTH*BAD_IMG_HEIGHT];
extern BYTE       g_fmDefectTemp1[BAD_IMG_WIDTH*BAD_IMG_HEIGHT];
extern BYTE       g_fmDefectBinary[BAD_IMG_WIDTH*BAD_IMG_HEIGHT];
extern BYTE       g_fmPrev[32*MAX_WIDTH];

extern SYSTEMDATA		g_System;
extern CONFIGDATA		g_Config;
extern DEFECTDATA		g_Defect;
extern DEFECTDATA		g_DefectSend;	//서버, 마킹서버에 보낼 데이타

//RESULT_DELAY ----------------------------------
	extern DEFECTDATA	   g_DefectSendDelay[6];  //최대 Delay 5개 + 1
	extern int             g_DelayIndex;
//-----------------------------------------------

extern DEFECTTEMP		g_DefectTemp;	
extern FRAMEINFO        g_FrameInfo[100];
extern TEMPDATA			g_Temp;
extern CImage			g_Save;
extern CImage			g_SaveNG;
extern CImage			g_SaveComp;	

//검출후보-----------------------------------------------------------
extern CANDIPOINTS      g_CandiW;      //한 Frame에서 백 Point 후보점
extern CANDIPOINTS      g_CandiB;      //한 Frame에서 흑 Point 후보점
extern CANDIPOINTS		g_CandiArea;   //한 Frame에서 Area 후보점
extern CANDIPOINTS      g_CandiTinyB;	  //작은 흑점후보

extern CANDIDATA		g_CanidData[200]; //투영에서 사용함.


extern int NG_SPOT_WHITE;	//백점, 휘점
extern int NG_LAMI_PRESS;	//Lami Press
extern int NG_SPOT_BLACK;	//흑점
extern int NG_SCRATCH;		//스크라치
extern int NG_LONGSCRATCH;	//Long스크라치
extern int NG_CUNIC;		//쿠닉
extern int NG_SPOT_PRESS;	//찍힘
extern int NG_KKOPCHIM;		//꼽침
extern int NG_GROUP;		//군집
extern int NG_MURA;			//얼룩
extern int NG_TAC_MURA;		//TAC성 얼룩
extern int NG_TAC_LINE;		//TAC라인
extern int NG_TAC_MURA_B;	//TAC성 (흑)얼룩
extern int NG_SPOT_KIPO;	//이물성기포(정투과)
extern int NG_LINE;			//라인
extern int NG_LINE_ANGLE;   //사선라인
extern int NG_LINE_TD;		//TD라인
extern int NG_COS4_105INCH;	//수평라인
extern int NG_KIPOPRESS;	//기포눌림 (NEL7 76)
extern int NG_LINEKIPO;		//라인기포(NEL7)
extern int NG_MARKING;		//정코마킹
extern int NG_BLACK_MURA;	//투영흑얼룩
extern int NG_BLACK_GROUP;	//투영흑군집
extern int NG_FISHEYE;			//Fisheye
extern int NG_LIPSHAPE;		//입술불량

extern int NG_WING_BLOCK;	//BAT Side 흑점
extern int g_BaseLine[4096];

int LoadSystemData();
int LoadConfigData();
int SaveSystemData();
void LoadPerspective();  //투영, LR 같은경우 읽어야 하는 데이타
void SavePerspective();  //투영, LR 왜곡데이타 저장
	void MakeLUT();			//Area영상 Transform
void MakeDefectLUT(int nBright, int nContrast);	//Defect을 표시할때 사용하는 LUT

int CossMemoryAlloc();
void CossMemoryFree();
void CossMakeDirectory();
void CossMakeDirectoryLot(CString sLotName);
void CossMakeImageDirectoryLot(CString sLotName);

void ResetCount();
int GetMarkOptic();

void LoadDKOffset(); //DK 얼룩 Offset구함
//가장 최근 LOT이름 쓰고/읽기
int SaveCurrentLotName();
CString LoadCurrentLotName();


//LOG사용하는 파일들-----------------------------
void WriteLog(CString str);
void WriteDebugLog(bool bMakeFile, CString str);

#ifdef BARCODE_VISION
void WriteBarcodeInfo(CString strMsg, CString strIdx, int nRst, int nframe);
void WriteBcrDefectLog(CString path, CString str);
void WriteBcrEdgeLog(float edgePos);
#endif