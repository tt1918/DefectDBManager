

//COS2-COS7에서 받은 데이타 저장하는 곳 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//COS7에서 데이타 받으면 N=(X-CalX)/5  해서  [N-1],[N],[N+1]  Set
//                       M=Y/(Frame/5) 해서  [M-1], [M], [M+1] Set
#define MAX_COS_RECV_PRE_NG	1600	//한 Frame을 16개로 나눔. 따라서 100Frame거리 (NEL5-NEL7은 NEL7기준 100Frame이내 떨어져야 함)
typedef struct KIPOPRESS_
{
	int    m_nKipoPress;
	BYTE   m_COSRecvPreNG[MAX_COS_RECV_PRE_NG][24];  //X는 5mm씩 24개
	int    m_COSRecvIP[21];			//받는 PC별 포트(IP) (COS4, COS7, COS2는 최대 20개 넘지 않음)
	double m_COSRecvCalX[21];		//받는 PC의 Cal X    (   "                    "             )
	int    m_nPort[4];				//기포눌림에 사용되는 포트
	double m_KIPODistance[4];		//SEND에서 RECV까지 거리 Offset (mm)

	int    m_SlitFrameAtReceive[3];	//SEND에서 보내온 데이타를 받을 당시 CurFrame
	int    m_nCOSSendCount[3];		//SEND에서 보내온 데이타 수  (RECV는 최대 3대 연결 될 수 있으므로)
	int    m_nCOSSendX[10], m_nCOSSendY[10]; //COS7에서 받은 데이타 
	int    m_nIsHole[10];			//Hole 이면 1
	int    m_nCOSRecvTotal;			//총받은 데이타 수 

	int    m_nCurRectCount;			//현재 COS2의 검사 이미지에서 Kipo받은 데이타가 있으면 그 수 
	int    m_nShowRect;
	RECT   m_nCurRect[4];			//최대 COS2의 한 Frame에서 4개 까지 지원 
	int    m_nCurRectHole[4];		//이 사각형이 Hole사각형이면 1

}KIPOPRESS;
extern KIPOPRESS g_Kipo;
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 

typedef struct MARKINGDATA_
{
	int			m_nPC;
	int			m_nMarkingPort;  //0~200
	float       m_dX;			 //다시 서버로 줄데이타 X좌표(MM)
	float       m_dY;			 //다시 서버로 줄데이타 Y좌표(M)
		
}MARKINGDATA;


//MARKING_VISION ------------------------------------------------------------------------------------------------------------
#define MAX_MKDATA_STORE	800    //128Pixel은 대략 25mm, 마킹기와 마킹비전 최대 20M 떨어져 있으면 최대 800개 저장
#define MAX_MKDATA_PORT		200	   //포트수 최대 200개	
#define MAX_INDEX			1000   //마킹서버와 마킹비전사이에 마킹이 1000개가 넘어가면 안됨
#define MARKING_GAP			10.0   //마킹 Port한개의 실제거리
typedef struct MAKRINGVISION_
{
	int    m_nFrameOffset;		//마킹기와 마킹비전간 Offset (128PixelxScale)기준 (RED마킹)
	int    m_nFrameOffset1;		//마킹기와 마킹비전간 Offset (128PixelxScale)기준 (BLUE마킹)
	double m_dTh;				//마킹 Threshold

	int    m_nROIOffsetX1;		//검사영역X시작 Offset
	int    m_nROIOffsetX2;		//검사영역X끝 Offset
	int    m_nROIHeight;		//검사영역Y폭

	int   m_nMarkingData[MAX_MKDATA_STORE][MAX_MKDATA_PORT];  //마킹서버에서 보내오는 순서대로 Index넣는다. 1~(MAX_INDEX-1)
	MARKINGDATA m_MKData[MAX_INDEX];
	int    m_nMKIndex;

	int    m_rcCnt;				//검사 ROI수
	int    m_nInspIndex[10];
	RECT   m_rcInsp[10];		//한 Frame에서 최대 10개 체크


	//결과----------------------------------------------
	int    m_nResult[10];		//각 Frame결과값 (최대 10개 ROI에 대해서)
	float  m_nResultData[10][2];//각 Frame 결과값
	int    m_nResultIndex[10];  //그 ROI의 Index(1~999)
	//--------------------------------------------------

}MAKRINGVISION;
extern MAKRINGVISION g_MakringVision;
//--------------------------------------------------------------------------------------------------------------------------------


/*
typedef struct TEACHDATA_
{
	int     nXDirection;							//0이면 그대로 1이면 X를 뒤집는다. 
	int     nOneFrameGrabCount;						//1 Frame은 몇개의 Area Frame로 구성되어 있나.
	double  dOneFrameLength;						//1 Frame 실제 길이      
	int		nMinSharpness;
	int		nSkip;									//항상 2로 사용함.
	int     nMinEdgeCount;							//Y방향 1줄 최소 Edge수 
	int		nMinDepth;								//20~50 정도가 적절함 (아주 작은 변동도 보려면 낮추면 됨)
	int     nMaxMinRange;							// nBlackValueMaxMin, nWhiteValueMaxMin, dBlackThickMaxMin, dWhiteThickMaxMin 구할때 사용하는 Max-Min값을 찾을 Range


	int     m_nKipoPress;							//기포눌림 


	int     m_n2Level;								//2Level검사면 1
	int     nTemp[50];
	double  dTemp[50];

}TEACHDATA;

extern TEACHDATA	g_Teach;
*/
typedef struct FINDHOLEDATA_
{
	int m_nHoleFind;		  // Hole Find가 1이면 Hole검사한다. 그렇지 않으면 Hole검사 안함
	int m_nXSkipUse;		  // X 안쓰는 범위 사용하면 1
	double m_dPeriod;	      // 파라미터로에 받은 주기 mm
	double m_dHolePitch;	  // Hole X 거리
	double m_dHoleDia;		  // Hole Diameter;
	int m_nHoleCount;		  // Hole 수
	int m_nThreshold;		  // Hole Binary하기 위한 Threshold Offset
	int m_nPeriodPixel;       // 파라미터로에 받은주기 Pixel로 환산한 값
	int m_nHolePitchPixel;	  // Hole Pitch를 Pixel로계산
	int m_nHoleDiaXPixel;     // 실제로 구한 Hole의 지름
	int m_nHoleDiaYPixel;     // 실제로 구한 Hole의 지름
	int m_nPeriodRangeX;	  // 주기로 판정하기 위한 X값 범위 (20Pixel)  1mm  (0.05기준)
	int m_nPeriodRangeY;	  // 주기로 판정하기 위한 Y값 범위 (20Pixel)  1mm  (0.05기준)
	int m_nCunicNotArea;	  // 쿠닉 검출 하지 않는 영역
	double m_dSizeX1;		  // Hole중심부터(X) 왼쪽 사용영역
	double m_dSizeX2;		  // Hole중심부터(X) 오른쪽 사용영역
	double m_dSizeY1;         // Hole중심부터(Y) 먼저촬상한 영역에서 사용영역
	double m_dSizeY2;         // Hole중심부터(Y) 나중촬상한 영역에서 사용영역
	
	int m_nPrevThreshold;	  //직전 Threshold  Hole이 중앙에 있지 않고 바닥에 붙은 경우
	int m_nBottomTouch;		  //직전 Frame에서 Hole이 Bottom에의해 짤린경우 1

	int	m_nHoleDiaXLimit;
	int	m_nHoleDiaYLimit;

	int m_nSizeX;			  //Hole의 크기 X
	int m_nSizeY;             //Hole의 크기 Y
	int m_nCx;				  //Hole의 중심
	int m_nCy;                //Hole의 중심
	int m_nFoundNext;		  //다음번 Hole의 위치를 알면 1
	int m_nFoundPrevEnd;	  //직전 END에서 Hole을 검출했으면 다음번 시작위치에서 없앰
	int m_nFoundThis;		  //이번 검사에서 영역이 Hole이 있으면 1
	int m_nThisX;			  //이번 검사에서 추정치 (X)
	int m_nThisY;             //이번 검사에서 추정치 (Y)
	int m_nNextX;			  //다음번에 나올 위치 (X)
	int m_nNextY;			  //다음번에 나올 위치 (Y)

	int m_nFoundHoleCount;	  //이번 Frame에서 발견한  Hole의 수
	RECT m_nrcHole[2];		  //최대 2개 

	//X 검사하는 영역(서버에서 받는 것)  [0]~[1], [2]~[3], .. 외는 검사하지 않음.
	int    m_nUseInspArea;   //1이면 검사영역사용
	double m_dInspAreaX[6]; //

	//당분간 사용하지는 않으나 일단 보류함--------------------------------------------------------
	int    m_nNotInspAreaYPixelS[4];//m_nNotInspAreaYPixelS[i]~m_nNotInspAreaYPixelE[i] 영역만 검사
	int    m_nNotInspAreaYPixelE[4];
	//--------------------------------------------------------------------------------------------

	int m_nHoleFound;
	int m_nHoleFoundX;

	int m_nHoleFoundPrevFrame;
	int m_nHolePrevX;
	int m_nHolePrevY;

	int  m_nServerX[10];
	int  m_nServerY[10];
	int  m_nServerCount;

	int  m_nSendHoleData;
	double m_dCalX[3];

	int  m_nTMethod;		//0 제자리 1: 끝측정
	double m_dBaseAvg;		//Base이미지 평균밝기
	double m_dBaseOffset;   //Base Offset               
	double m_dBaseOffsetArray[25];	//-60, -55, ..0. 5, 10, 15, ...60  25개 
}FINDHOLEDATA;


void KO_SetKipoPress(int nFrame, int nCount, int *pX, int *pY);
void KO_UpdateKipoPress(int nFrame);
void KO_SendDefect();



//MARKING_VISION
void WEB_CheckMarkingData();
int COSS_CheckMarkingROI();