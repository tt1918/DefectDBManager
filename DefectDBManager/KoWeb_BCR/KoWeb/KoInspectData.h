#define MAX_INSPECT_CLASS					10		//불량종류 수
#define MAX_INSPECT							50		//최대 검사 설정 수
#define MAX_INSPECT_SET						4		//50개씩 4세트
#define MAX_INSPECT_SPOT_LEVEL_NG_COUNT		10		//Spot검사 각 Level별 최대 불량수	

#define MAX_INSPECT_BLOB					200		//Blob은 최대 200개만 검출한다.
#define BLOB_INPECT_ITEM					6		//Blob검사 항목 수

#define INSP_BLOB		0			
#define INSP_PATTERN	1
#define INSP_FINDLINE	2
#define INSP_GAP2LINE	3
#define INSP_SPOT		4		//@항목추가1 (Dialog Combobox에 "Spot"추가
#define INSP_COMPARE	5
#define INSP_OCR		6
#define INSP_USER		7
#define INSP_STAIN		8
#define INSP_GAP		9


#define MAX_PATTERN		100		//등록할 수 있는 패턴 수

//SPREAD COLUMN
#define TEACH_ID		3


extern TCHAR g_sInspectTypeName[MAX_INSPECT_CLASS][30];

typedef struct ROIDATA_
{
	double  dAngle;
	RECT	rc;
	double  Px[4];
	double  Py[4];
}ROIDATA;

typedef struct PATTERNDATA_
{
	int m_nAddCount;		//0:티칭전, 1이상 티칭
	int m_nWidth;
	int m_nHeight;
	LPBYTE m_fm[2];			//[0]:원본 [1]: MASK
	int m_nMultiFound;		//1이면 Multi로 찾음, 보통은 0
	int m_nMinScore;		//최소 검출 Size
	double m_dReturnPosX;	//Return PosX	
	double m_dReturnPosY;	//Return PosY	
	TCHAR m_sName[100];		//패턴이름 

}PATTERNDATA;


typedef struct INSPECTBLOB_
{
	int m_nThreshold;
	int m_nObject;
	int m_nMinArea;
	int m_nUSE[10];
	double m_dMin[10];
	double m_dMax[10];

	int m_nTemp[10];
	double m_dTemp[10];

}INSPECTBLOB;

typedef struct INSPECTPATTERN_
{
	int m_nPatternID1;
	int m_nPatternID2;
	double m_dMinScore;
	double m_dMaxOffset;

	int m_nTemp[10];
	double m_dTemp[10];

}INSPECTPATTERN;

typedef struct INSPECTFINDLINE_
{
	int m_nLineType;
	int m_ndiagonal;
	int m_nDir;
	int m_nObject;
	int m_nMethod;

	int m_nGrayTh;
	int m_nSlopeTh;
	int m_nAvgLine;

	double m_dMinAngle;
	double m_dMaxAngle;

	int m_nTemp[10];
	double m_dTemp[10];

}INSPECTFINDLINE;

typedef struct INSPECTMEASURE_
{
	int m_nBaseId;
	int m_nPointId;
	double m_dMinDist;
	double m_dMaxDist;

	int m_nTemp[10];
	double m_dTemp[10];

}INSPECTMEASURE;

typedef struct INSPECTCOMPARE_
{
	int m_nData;

	int m_nTemp[10];
	double m_dTemp[10];

}INSPECTCOMPARE;

typedef struct INSPECTOCR_
{
	int m_nSizeX;
	int m_nSizeY;
	int m_nCharNum;
	int m_nCharPitch;
	int m_nEqualPitch;

	int m_nTemp[8];
	double m_dTemp[10];

}INSPECTOCR;

typedef struct INSPECTUSER_
{
	int m_nData;

	int m_nTemp[10];
	double m_dTemp[10];

}INSPECTUSER;

//@항목추가 8: 검사 Struct
typedef struct INSPECTSPOT_
{
	int		m_bUseGrayRange;	//Gray Min-Max 사용
	int     m_nGrayMin;			//Gray Min
	int     m_nGrayMax;			//Gray Max
	int     m_bUseMeanFilter;	//Average Filter사용
	int     m_nMeanKernel;		//MeanFilter (N x N)
	int		m_bBumpOnly;		//찍힘ONLY
	int		m_bDirection[4];	//0:수평, 1수직 2:45도 3:-45도
	int     m_bShowBinary;

	int     m_bUseLevel[7];		//Level사용여부
	int		m_nThUp[7];         //White TH값 3개 
	int		m_nThDn[7];			//Black TH값 3개
	int     m_nOffset[7];		//검사 Offset
	int     m_nWValue[7];		//백점 최소 Value
	double	m_dWSizeMin[7];		//White Size 최소값
	double	m_dWSizeMax[7];		//     "     최대값
	int     m_nBValue[7];		//흑점 최소 Value
	double	m_dBSizeMin[7];		//Black Size 최소값
	double	m_dBSizeMax[7];		//     "     최대값

	int		m_nBlobAddDist;     //이웃한 Blob붙이기위한 거리
	double m_dMaxRatio;			//(장축/단축) 값이 이값보다 크면 SKIP
	double m_dMinDefectArea;	//불량 최소 면적 비율

	//MASK DATA-----------
	int     m_bUseMask;			//Dont Care Mask사용
	LPBYTE   m_fmOrg;			//원본정보
	LPBYTE   m_fmMask;			//마스크정보
	int      m_nMaskWidth;		//마스크크기X
	int      m_nMaskHeight;		//마스크크기Y
	//--------------------

	int m_nTemp[10];
	double m_dTemp[10];

}INSPECTSPOT;

typedef struct INSPECTDATA_
{
	int   m_nInspectCount;            //불량검사 설정 수 
	int   m_nInspectType[MAX_INSPECT];//불량검사 종류
	int   m_nID[MAX_INSPECT];		  //ID
	int   m_nAlign[MAX_INSPECT];	  //0 Align사용안함. Nonzero 사용할 Align 번호
	int   m_nROINum[MAX_INSPECT];	  //이 불량을 검사하기 위해 사용할 ROI수
	int   m_nROIShow[MAX_INSPECT];	  //ROI 보이느야 안보이느냐
	int   m_nUSE[MAX_INSPECT];		  //이 검사의 사용여부
	int   m_nTeachDone[MAX_INSPECT];  

	ROIDATA m_ROI1[MAX_INSPECT];	  //사용할 ROI
	ROIDATA m_ROI2[MAX_INSPECT];      //사용할 ROI

	INSPECTBLOB		m_InspBlob[MAX_INSPECT];
	INSPECTPATTERN	m_InspPatt[MAX_INSPECT];	
	INSPECTFINDLINE m_InspFindLine[MAX_INSPECT];
	INSPECTMEASURE  m_InspMeasure[MAX_INSPECT];
	INSPECTSPOT		m_InspSpot[MAX_INSPECT];	//@항목추가 9 :검사항목에 대한 데이타 struct만들고 여기 추가함.
	INSPECTCOMPARE  m_InspCompare[MAX_INSPECT];
	INSPECTOCR		m_InspOCR[MAX_INSPECT];
	INSPECTUSER		m_InspUser[MAX_INSPECT];	

}INSPECTDATA;

typedef struct INSPECTSYSTEM_
{
	double m_dScaleFactorX;
	double m_dScaleFactorY;


}INSPECTSYSTEM;

//ID별로 결과가 저장됨
typedef struct INSPECTRESULT_
{
	int    m_nInspectType;			//검사종류


	//Blob데이타
	int    m_nBlobResult[10];			//0:OK 1:NG
	double m_dBlobData[10];				//Blob결과
	RECT   m_rcBlob[MAX_INSPECT_BLOB];	//Blob은 최대 200개

	//Pattern Match
	int    m_nPatternResult[2];  //[0]:Score  [1]:Offset   결과 0:OK 1:NG
	int    m_nPatternScore;
	double m_dPatternOffset;
	double m_dPatternRx;
	double m_dPatternRy; 

	//FindLine
	int    m_nFindLineResult[2]; //[0]:라인찾음  [1]:각도   //0:OK 1:NG
	double m_t, m_a, m_b;		//직선	
	double m_dAngle;			//구한 각도
	double m_dCx, m_dCy;		//직선위의 대표값

	//Measure
	int    m_nMeasureResult[2]; //[0]:두라인이 없으면 NG  [1]: 측정값 //0:OK 1:NG
	double m_dDistance;			//측정값

	//@항목추가 10 :Sopt데이타  -------------------------------
	int		m_nLevelCount[7];									//각 Level(7개 Level) 에서 NG수(최대 10개)
	RECT	m_rcSpot[7][MAX_INSPECT_SPOT_LEVEL_NG_COUNT];		//NG사각형 각 Level당 최대 10개
	int     m_nSpotNGType[7][MAX_INSPECT_SPOT_LEVEL_NG_COUNT];	//0:WHITE 1:Black 2:Bump
	int     m_nSpotNGValue[7][MAX_INSPECT_SPOT_LEVEL_NG_COUNT]; //불량 Value
	double  m_dSpotNGSize[7][MAX_INSPECT_SPOT_LEVEL_NG_COUNT];	//불량 Size
	//---------------------------------------------------------

	//OCR
	int	  m_nOCRResult;		//OCR찾음
	int   m_nOCRNum;		//찾은 OCR수
	int   m_nOCRPosX[2];	//검사영역
	int   m_nOCRPosY[2];	//검사영역
	int   m_sOCRScore[100]; //찾은 문자 Score
	int   m_nPosX[100];		//찾은위치
	int   m_nPosY[100];
	TCHAR m_cOCR[100];		//찾은 문자



	//Compare
	int    m_nCompareResult[2];

	//User Defined
	int    m_nUserResult[2];    //[0]:  [1]: //0:OK 1:NG


}INSPECTRESULT;

extern	int           g_InspSet;						//현재 검사 Set
extern	INSPECTDATA   g_Inspect[MAX_INSPECT_SET];		//4개
extern	INSPECTSYSTEM g_InspectSys[MAX_INSPECT_SET];	//4개

extern  INSPECTRESULT g_InspectResult[MAX_INSPECT];		//50개
extern	PATTERNDATA	  g_Pattern[MAX_PATTERN];	

int SaveInspData(CString sModelFolder);
int LoadInspData(CString sModelFolder);
void CloseInspData();
void ClosePatternData();   //패턴 등록된것 있으면 해제


int LoadInspSystemData();

bool IsInspectResultNG(int nId);	//현재 검사항목이 NG인지 아닌지, return (true)1이면 NG	


//Model데이타
int NewModel();
int LoadModel(CString sModelName);
int SaveModel();