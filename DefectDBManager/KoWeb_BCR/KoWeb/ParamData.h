

	#define NEW_MAX_COL_ALG_ITEM	220		//보내는 건 최종 220개	
	#define MAX_CONNECT				160		//새로운 버전은 CONNECT 160개로 한다. 
	#define N_CONNECT				20		//한번에 UDP통해 보낼 파라미터 카메라 수




//PARAMETER_VER1로 조금씩 받아서 PARAMETER_VER2에 채움 (실질적으로는 이것을 이용) -------------------------------
typedef struct tagPARAMETER_VER2    //총 Size : 12+160*(8+220*4+8*4)+10+100+60 = 147382, COSS서버의 경우 160*8+10+100증가=147382+270=148772 
{
	int    nTotalSend;								//총 몇번 보낼건지
	int    nCurrentSend;							//현재 보내는 건 몇번째인지 (다 받으면 서버에 받은 신호 보냄)

	int   nTotalPort;								//총 컴퓨터 수
#ifdef INTERFACE_NEXTEYE_SERVER
	char  ComName[MAX_CONNECT][8];					//컴퓨터이름 
#else
	wchar_t  ComName[MAX_CONNECT][8];				//컴퓨터이름 
#endif
	float param[MAX_CONNECT][NEW_MAX_COL_ALG_ITEM];	//Param데이타  (MAX_CONNECT=160 PC ,  NEW_MAX_COL_ALG_ITEM=220 Param)
	float system[MAX_CONNECT][8];					//Calib데이타
	
#ifdef INTERFACE_NEXTEYE_SERVER
	char  MachineName[10];							//10개면 충분함 
	char  ModelName[100];							//
#else
	TCHAR  MachineName[10];							//10개면 충분함 
	wchar_t  ModelName[100];							//
#endif
	int   nCycle[15];								//주기성불량 
}PARAMETER_VER2;


//서버에서 N_CONNECT(20) 개씩 받을때 사용하는 구조체 ------------------------------------------------------------
typedef struct tagPARAMETER_VER1    //총 Size : 12+20*(8+220*4+8*4)+10+100+60 = 18582, COSS서버의 경우 20*8+10+100증가=18582+270=18852
{
	int    nTotalSend;								//총 몇번 보낼건지
	int    nCurrentSend;							//현재 보내는 건 몇번째인지 (다 받으면 서버에 받은 신호 보냄)

	int   nTotalPort;								//총 컴퓨터 수
#ifdef INTERFACE_NEXTEYE_SERVER
	char  ComName[N_CONNECT][8];					//컴퓨터이름 
#else
	wchar_t ComName[N_CONNECT][8];					//컴퓨터이름 
#endif
	float param[N_CONNECT][NEW_MAX_COL_ALG_ITEM];	//Param데이타  총220개
	float system[N_CONNECT][8];						//Calib데이타
	
#ifdef INTERFACE_NEXTEYE_SERVER
	char  MachineName[10];							//10개면 충분함 
	char  ModelName[100];							//
#else
	wchar_t  MachineName[10];							//10개면 충분함 
	wchar_t  ModelName[100];							//
#endif
	int   nCycle[15];								//주기성불량 
}PARAMETER_VER1;


//마킹데이타 (파라미터 서버로 부터 받아서 저장할때만 사용함)----------------------------------------------------
typedef struct tagPARAMETER_MARK
{
	bool   nData[MAX_OPTIC*10][10];					//마킹데이타 (1 마킹함, 0 마킹안함)   [MAX_OPTIC:광학계수  10:Defect종류][10 Level]

	//가변마킹데이타-------------------------------------------------------------------------
	int    nLevelMCount[MAX_OPTIC*10][10];			//광학계별,마킹종류, 10Level M당 불량수
	int    nDefectMCount[MAX_OPTIC][10];			//광학계별, Defect종류별

	float  dBaseLength_mm;							//M당불량수 구하는 기준길이
	float  dBaseLength_M;							//큰영역 불량수 구하는 기준길이
	float  dBaseX_mm;								//큰영역 같은 같은 불량 간주 +/- 폭
	int    nUSE[2];
}PARAMETER_MARK;

//USE_20_LEVEL_MARK -----------------------------------
typedef struct tagMARKING_SUB
{
	BYTE   nDefectTypeMarking[10];	//10  해당하는 DefectType에 20Level마킹이 하나라도 있으면 1
    BYTE   nMarking[10][20]; 		//200   

    float  dValueMin[10][20];		//800
    float  dValueMax[10][20];		//800	

    float  dSizeMin[10][20];		//800
    float  dSizeMax[10][20];		//800	

    float  dBValueMin[10][20];		//800
    float  dBValueMax[10][20];		//800	
    float  dWValueMin[10][20];		//800
    float  dWValueMax[10][20];		//800	
}MARKING_SUB;           			//3410

typedef struct tagMARKING_SUB_NET
{
	BYTE   nDefectType[50];			//50
    BYTE   nMarking[50][20]; 		//1000 

    float  dValueMin[50][20];		//4000
    float  dValueMax[50][20];		//4000	

    float  dSizeMin[50][20];		//4000
    float  dSizeMax[50][20];		//4000	

}MARKING_SUB_NET;					//총 17050

typedef struct tagMARKING_SUB_ALL
{
	BYTE   nDefectType[150];		
    BYTE   nMarking[150][20]; 		

    float  dValueMin[150][20];		
    float  dValueMax[150][20];		

    float  dSizeMin[150][20];		
    float  dSizeMax[150][20];		

}MARKING_SUB_ALL;  
//-----------------------------------------------------

//실제프로그램에 사용 (MAX_OPTIC 값이 바뀌어도 아래 값은 안 바뀜) ---------------------------------------------
//한개 PC에서 사용하는 데이타
typedef struct tagPARAMETER_MARKOPTIC
{
	int    nData[10][10];						//마킹데이타 (1 마킹함, 0 마킹안함)   [10:Defect종류][10:Level]
	int    nLevelMCount[10][10];				//Defect종류, 10 Level의  M당 불량수
	int    nDefectMCount[10];					//Defect종류별 같은 X에서의 불량수
	float  dBaseLength_mm;						//M당불량수 구하는 기준길이
	float  dBaseLength_M;						//큰영역 불량수 구하는 기준길이
	float  dBaseX_mm;							//큰영역 같은 같은 불량 간주 +/- 폭
	int    nUSE[2];

	int    nCheckLevelMCount;					//가변마킹 1단계(군집성) 사용하면 1
	int    nCheckDefectMCount;                  //가변마킹 2단계(주기성) 사용하면 1
	int    nMarking[10][10];					//최종 마킹 (1 마킹함, 0 마킹안함)    [10:Defect종류][10:Level]
}PARAMETER_MARKOPTIC;

typedef struct tagPARAMETER_VER			//총 Size : 12+160*(8+220*4+8*4)+10+100+60 = 147382 
{
#ifdef INTERFACE_NEXTEYE_SERVER
	char  ComName[8];					//컴퓨터이름 
#else
	wchar_t  ComName[8];					//컴퓨터이름 
#endif
	float param[NEW_MAX_COL_ALG_ITEM];	//Param데이타  (MAX_CONNECT=160 PC ,  NEW_MAX_COL_ALG_ITEM=220 Param)
	float system[8];					//Calib데이타
	
	CString  m_strMachineName;			//CC02 DE03 이런것
	CString  m_sModelName;				//W3_NCB_FFF_DDD 이런거
	int   nCycle[15];					//주기성불량 
}PARAMETER_VER;
//----------------------------------------------------------------------------------------------------------------

typedef struct PARAMETERDATA_
{
	int  m_nNoInspect;		//1이면 검사안함
	int  m_nAutoEdgeFind;	//1이면 자동검사
	int  m_nNotInspArea;	//검사하지 않는 영역(양수면 왼쪽, 홀수면 오른쪽)
	int  m_nEdgeOffset;		//자동Edge후 Offset

	int  m_nTapeDist[3];	//[0]LOT구분TAPE [1]:보호 [2]:이형  Tape오기까지 Frame수
	int  m_nTapeSkipDist;	//Tape Skip 할 Frame 수
	int  m_nBrightMax;		//검사가능 최대밝기
	int  m_nBrightMin;		//검사가능 최소밝기
	int  m_nOffsetTapeFrame;//Unwinder신호후 이 Frame만큼 지난후 Tape거리(Tape Chcek 거리)내에서 Tape을 찾는다.

	int  m_nSetAutoExposure;//1이면 자동밝기
	int  m_nInspectType;	//3D 경계투과 또는 Edge투과 TAC_LINE에서 사용함. 1,2,3이면 KNULLING체크
	int  m_nNoKnullingFilm;	//널링없는 원단
	int  m_nKnullingValue;	//KNULLING VALUE
	double m_dNullingRatioLimit; //2.0을 Default값으로 사용함.
	int  m_nKnullingPC;		//KNULLING을 검사할 PC

	//당분간 USER 설정
	int    m_nEdgeThreshold;	//Edge라고 할 Threshold

	int	   m_nYOffset;			//카메라 nYOffset
	int	   m_nExposure;			//적용된 exposure값
	int    m_nGain;
	double m_dCamStartPosX;		//카메라시작 좌표
	double m_dCamStartPosY;		//카메라시작 좌표
	double m_dFrameLen;			//Frame 거리
	double m_dScaleFactorX;     //Scale
	double m_dScaleFactorY;     //Scale
	double m_dImageAngleFactorY;//CAMANGLE 영상기울어짐(왼쪽기준)  x좌표 곱해주면 Offset 구해짐

	//1차원 (각 한 항목이라도 만족하면 1Level) ------------------------------------------
	int    m_nValueOneUp;		//백 Value가 이 값보다 크면 Size 관계없이 1Level
	double m_dSizeOneUp;		//백 Size가 이 값보다 크면 Value 관계없이 1Level
	int    m_nValueOneDn;		//흑 Value가 이 값보다 크면 Size 관계없이 1Level
	double m_dSizeOneDn;		//흑 Size가 이 값보다 크면 Value 관계없이 1Level
	//-----------------------------------------------------------------------------------
	
	int    m_nPeriodLevel;		//주기를 1Level로 변경

	int    m_nPressInspect;		//1 이면 찍힘검사 함.
	int    m_nSizeMethod;		//Size구하는 방법 0 (가로, 세로, (가로+세로)/2), 1 (장축, 단축, 장축)   2 (장축, 단축, (장축+단축)/2) 
	int    m_nAttatchPixel;		//이웃하는 Blob붙이는 것
	int    m_nAreaThUp;			//Size구하기 위한 TH Up
	int    m_nAreaThDn;			//Size구하기 위한 TH Dn
	int    m_nPressMinUp;		//찍힘 흑 최소값
	int    m_nPressMinDn;		//찍힘 백 최소값

	int    m_nUseLevel[10];		//사용하는 Level
	int    m_nValueUp[10];		//White Value
	double m_dSizeUp[10];		//White Size
	int    m_nValueDn[10];		//Black Value
	double m_dSizeDn[10];		//Black Size
	int    m_nPressValue[10];	//찍힘 Value
	double m_nPressSize[10];	//찍힘 Size



	int    m_nSTDThUp[10];		//백얼룩
	double m_dSTDSizeUp[10];	//백얼룩 Size
	int    m_nSTDThDn[10];		//흑얼룩
	double m_dSTDSizeDn[10];	//흑얼룩 Size

	int    m_nScratch[10];		//스크라치 Value
	int    m_nLongScratch[10];  //롱 스크라치 Value

	int    m_nCunicInspArea;	//쿠닉검사 Area
	double m_dCunicCanidValue;	//쿠닉후보 최소값
	double m_dCunicStd[10];		//독립적인 쿠닉STD
	double m_dCunicStd1[10];	//쿠닉Value1와 짝을 이룸
	int    m_nCunicValue1[10];	//쿠닉 Std1과 짝을 이룸

	int    m_nScratchOffset;	//스크라치 검사갭
	int    m_nScratchDiff;		//스크라치 밝기차
	int    m_nScratchValue[10]; //스크라치 밝기Value
	double m_dScratchLength[10];//스크라치 길이

	int    m_nKipoValue[10];	//기포이물(COS4) Value
	double m_dKipoSize[10];		//기포이물(COS4) Size
	int    m_nKipoMinValue;		//기포이물(COS4) 최소 Value
	double m_dKipoBrightDiff;	//기포이물(COS4) 최소 밝기차

	int    m_nBMura[10];		//투영 흑 얼룩

	//마킹검출
	int      m_nMarkingLevelChange;		//NEL4에서 마킹을 구해서 다른 Level로 변경(0 이면 하지 않음)
	int      m_nMuraLevelChange;		//NEL4에서 얼룩을 구해서 다른 Level로 변경(0 이면 하지 않음)
	int      m_nMarkPointValue;			//NEL4에서 마킹 구할때 사용
	double   m_dMarkMinValueWithPoint;	//NEL4에서 마킹불량 m_nMarkPointValue값과 세트
	double   m_dMarkMinValue;			//NEL4에서 마킹불량
	int      m_nMarkNewValue;			//NEL4에서 마킹검출 새로운 방법
	int      m_nMarkNewTh;				//NEL4에서 마킹검출 새로운 방법

	//DOT마킹 빼기위한 것
	int     m_nDotMarkRemove;			//1이면 Dot마크 제거
	int     m_nDotMarkTh;				//마킹 구하기 위한 Threshold
	double  m_dDotMarkSize;				//마킹Size
	int     m_nDotLimitUp;				//평활화후 Value Up이 이값보다 크면 마크아님
	int     m_nDotLimitDn;				//평활화후 Value Dn이 이값보다 크면 마크아님
	int     m_nDotLevelChange;			//마킹을 없애지 않고 Level만 변경시킴

	//기포라인
	int		m_nLineKipoWidth;
	int		m_nLineKipoTh;
	int     m_nValueLineKipo[10];

	//세로선 얼룩
	int     m_nMuitlLineValue[10];

	//투영검사 ------------------------------------------------------------
	//주름 과검용 파라미터
	int	   m_nOverKillCount;
	int    m_nOverKillValue;

	double m_dOverKillSize;		//과검체크 Size(이 값보다 작으면 과검체크)
	double m_dOverKillSTD;		//과검 최소 STD
	double m_dOverKillDiff;		//과검 밝기 차


	//군집(흑군집(투영), Cross군집)
	int    m_nGroupArea;		//군집범위
	int    m_nGroupAreaTh;		//흑군집Area TH
	int    m_nGroupValue;		//흑군집 Value
	double m_dGroupSize;		//흑군집 Size
	int    m_nGroupSizeMethod;	//0 기존방식, 1대각선
	int    m_nGroupCount;		//흑군집 갯수
	int    m_nGruopLevel;		//군집을 변경할 Level
	int    m_nNEL5LevelChange;	//가변마킹으로 마킹이 된 불량은 다른 Lv로 마킹기에 전송함.(1~10까지 데이타가 있을때 유효함)
	int    m_nGroupAreaArray[10];	//아래4개를 설정해서 사용할떄 
	int    m_nGroupCountArray[10];
	int    m_nGroupValueArray[10];
	double m_nGroupSizeArray[10];

	//미세이물 밀집검사
	int    m_nMILStartLevel;	//밀집 시작 Level
	int    m_nMILTh;			//밀집 Th
	double m_dMILSize;			//밀집 Size
	int    m_nMILCount;			//밀집 Count
	int    m_nMILChangeLevel;	//밀집 변경 Level

	int    m_nLineThUp;			//백 Line 을 Blob으로 검출시 사용함
	int    m_nLineThDn;			//흑 Line 을 Blob으로 검출시 사용함

	//Line Scan의 라인검출
	int    m_nLineUpValue[10];
	int    m_nLineDnValue[10];
	int    m_nLineTDValue[10];		//라인얼룩용라인


	//Cross꼽침에사용
	int	   m_nSlimPercent;				//몇 %이상 차이나면 쓸림으로 판정, 최소값 10 (차이가 많이 나면 쓸림 즉, 꼽침이 아님)

	double m_dLineThresholdAngle[10];	//사선라인
	double m_dLineThreshold[10];		//MD Line, Cross 꼽침
	double m_dLineThresholdW[10];		//MD Line, Cross 꼽침
	double m_dLineThresholdTD[10];		//TD Line
	//------------------------------------------------------------------

	//기포눌림 마킹
	int    m_nKipoPressLevelMarking;

	//라미눌림
	int    m_nLamiTh;
	double m_dLamiMaxSize;
	double m_dLamiSize[10];

	//DAKANO
	int  m_nDKTapeValue;	//한줄이 이값보다 높으면 Tape
	int  m_nDKJumpEdgeValue;	//한줄이 이값보다 낮으면 Jummping
	int  m_nDKSlopePixel;	//기울어진 Pixel
	int  m_nDKLineCnt;		//검사라인수 
	int  m_nDKNoUseJumpping;//점핑영역 사용안함.
	int  m_nDKMarkingTest;	//1이면 마킹테스트용, 흑점을 라인으로 검출해서 보냄
	//물떄
	int  m_nSaveN;			//1이면 매 불량 Frame저장  2이면 짝수 Frame저장, 3이면 3의 배수 저장
	int  m_nDivisionY;

	//정밀코팅 NEL1
	int m_nOverkillSlope;		//과검값									//Param6
	int m_nMarkThreholdValue;	//MarkValue									//Param7
	int m_nMarkLevel;			//Mark로 잡은것 Level						//Param8
	int m_nRemoveFishEye;		//FishEye제거								//Param9
	int m_dOffsetAngle;			//가장 어두운곳 찾은 후 Offset각도로 이동	//Param11
	int m_nUSEHTO;
	double m_dHTOCloudW;
	double m_dHTOCloud;
	double m_dHTOCloudSize[10];
	
	//정밀코팅NEL2, NEL6
	int m_nBrightCompensation;	//밝기증배
	int m_nStdSizeW;			//STD 측정크기(백)
	int m_nStdSizeB;			//STD 측정크기(흑)

	//3D입술불량
	double m_dLipStdevLimit[10];//입술판단값
	int    m_nGrayLimit;		//입술불량 (밝기차 )
	double m_dStdevSizeThres;	//입술불량 
	double m_dLipMinSize;		//입술불량 최소 Size


	//본딩시트
	int   m_nRemoveBondingOverkill;	//본딩시트 과검제거

	int   m_nBaseLine;


	int		m_nShadowOffset;		//그림자 인식거리
	int		m_nShadowTh;			//PinHole에서 그림자 Size 구할때 사용하는 Th
	double	m_dShowSkipSize;		//그림자 Skip Size (그림자Size가 이값보다 크면 Skip한다)
	double  m_dShowdowNotSkipSize;	//그림자 Skip 제외 Size (원래 AreaTh로 구한 Size가 이 값보다 크면 그림자 관계없이 SKIP하지 않는다)

#ifdef USE_SK_BAT
	int      m_nEdgeType;		//1이면 Edge가 왼쪽에, -1이면 Edge가 오른쪽에
	int      m_nSearchX1;		//Edge서치범위
	int      m_nSearchX2;		//Edge서치범위
	double   m_dOffset1;		//맨 바깥쪽 에지부터 Offset
	double   m_dOffset2;		//안쪽Edge에서 바깥쪽으로 사용하지 않는 영역 Offset
	double   m_dOffset3;		//안쪽Edge에서 안쪽으로  사용하지 않는 영역 Offset
	int		m_nEdgeSlope;		//Edge Slope
	int		m_nCurrNoInspArea;		//Curr미검영역
	
	int      m_nAreaThDnWing;	//바깥쪽 Blob구하기 위한 TH
	int      m_nValueDnWing[10];
	double   m_dSizeDnWing[10];
#endif

#ifdef USE_HOLE_SKIP
	int     m_nHoleTh;
	double  m_dHoleSize;
	double  m_dHolePeriod;		//Hole발생주기(mm)
	double  m_dHoleErrorRatio;

#endif

}PARAMETERDATA;  //g_Param


extern PARAMETER_VER1	g_ParamVer1; //20개씩 받는 것
extern PARAMETER_VER2   g_ParamVer2; //전체 파라미터
extern PARAMETER_MARK	g_ParamMark; //마킹데이타: 파라미터 서버로 부터 받아서 저장할때만 사용함.


//실제 한 PC에서 사용하는 데이타
extern TCHAR					g_ParamName[NEW_MAX_COL_ALG_ITEM][20]; //220개 파라미터 이름
extern PARAMETER_VER			g_ParamVer;				//파라미터 읽고 쓸때 사용

extern PARAMETERDATA			g_Param;				//한PC의 검사에 사용
extern PARAMETER_MARKOPTIC		g_Mark;					//한PC의 마킹정보

void LoadParamName();
int CheckFileReceive20();
void CopyParam20();
int Check_Ini_Parameter(CString filename, CString str_name );
int Ini_file_MakingVer3(CString filename, int nMarkingData);

int ReadParameter(int nReadFromReserve); //Parameter.ini를 읽어서 Array에 넣는다
void ReadKipoPressRecvCalX();
int SaveParameter();
void ApplyParameter();


int SelectDefect(int nLevel1, int nClass1, int nLevel2, int nClass2); //2개의 Level과 Class를 받아서 더 중요한 것을 Return 한다.


//USE_20_LEVEL_MARK --------
#ifdef USE_20_LEVEL_MARK
  extern MARKING_SUB_NET			g_MarkSubNet;	//통신으로 받는것
  extern MARKING_SUB_ALL			g_MarkSubAll;	//통신으로 받은 것을 전체로 모음 
  extern MARKING_SUB				g_MarkSub;		//이 클라이언트에서 사용하는 것


void SetSubMarkType(int nVisionLine);
void ApplySubMarking( int N);
int  SaveSubParam();
int  ReadMarkParameter(int nReadFromReserve);
int  ReadSubParam();
int  CopySubMarkParam50(int N);
//--------------------------
#endif