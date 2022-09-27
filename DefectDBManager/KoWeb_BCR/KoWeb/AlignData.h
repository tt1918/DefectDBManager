#define EDGE_TYPE_NUM		13
#define MAX_ALIGN_MODULE	4

typedef struct ALIGNDATA_
{
	int m_nAlignType;			//EDGE_TYPE_NUM (13)개 (0~12)
								//0~6  보정안함 X만, Y만  XY  XYY  YXX  XYXY 
	int m_nNoApply;				//Align데이타 구하되 적용하지 않음 
	int m_nShowAlignROI;		//보정 ROI 표시하려면 1	
	int m_nNoApplyAngle;		//Angle을 적용하지 않기
	int m_nMoveImage;			//1이면 이미지 이동, 0이면 ROI이동
	
	//------------------------------------------------------------------------------
	int m_nROIType[4];			//0: Y Edge(수평선) 1:X Edge(수직선)  2 Match

	int m_nPatternID[4];		//보정 패턴 번호 4개
	int m_nPatternMinScore[4];  //패턴매칭 최소 Score

	int m_nDir[4];				//서치방향 0, 1
	int m_nObject[4];			//대상 색
	int m_nMethod[4];			//Edge찾는 방법 0: Gray값으로 1:1stEdge 2: Max Edge
	int m_nGrayTh[4];			//밝기
	int m_nSlope[4];			//Edge Slope
	int m_nAvgLine[4];			//Edge구할때 평균할 Line 수
	//-------------------------------------------------------------------------------

	int m_nAlignNG;				//0: Aling OK  1:Align NG
	double m_dMinR;				//최소 보정 X : 이값 미만이면 각도 보정안함.
	double m_dMinX;				//최소 보정 Y : 이값 미만이면 각도 보정안함.
	double m_dMinY;				//최소 보정각도 : 이값 미만이면 각도 보정안함.
	double m_dNGR;				//NG각도: 구한 각도가 이 값보다 크면 Align ERROR
	double m_dNGX;				//NG X: 구한 dX가 이 값보다 크면 Align ERROR
	double m_dNGY;				//NG Y: 구한 dY가 이 값보다 크면 Align ERROR

	RECT m_rc[4];				//위치보정 서치영역 
	double t[4], a[4], b[4];	//ty=ax+b
	double m_dX[4], m_dY[4];	//Edge구한 중심점

	//SetAlign Data-------------------------------------------
	int  m_nRegistered;					//위치보정 등록됐으면 1
	double m_dBaseX[4], m_dBaseY[4];	//기준위치
	double bt[4], ba[4], bb[4];			//bty=bax+bb

	//Align 결과------------------------------------------------------------------------
	int    m_nFoundAlign;		//FindAlignOffset()를 Call해서 아래 데이타를 구한경우 1
	double m_dCx, m_dCy;		//회전 중심	
	double m_dOffsetX;			//구한 Offset 결과값
	double m_dOffsetY;
	double m_dAngleDeg;			//Angle (Degree)
	
	BYTE m_Temp[100];
}ALIGNDATA;

extern ALIGNDATA  g_AlignData[MAX_ALIGN_MODULE];
extern TCHAR g_sType[EDGE_TYPE_NUM][50];
extern int g_ROINum[EDGE_TYPE_NUM];
extern TCHAR g_ROIName[EDGE_TYPE_NUM][4][3];

int SaveAlignData(CString sFileName);
int LoadAlignData(CString sFileName);

int CalAlignData(ALIGNDATA *pAlign);
int FindNewPoint(ALIGNDATA *pAlign,double Px, double Py,  double *Px_, double *Py_); //현재위치(Px, Py) 가 Align적용하면 (*Px_, *Py_)로 이동
int FindLineFunction(double x1, double y1, double x2, double y2, double *t, double *a, double *b);


int FindLine(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nDir, int nLineType, int nMethod, int nObject, int nGrayTh, int nSlopeTh, int nAvgLine,  BOOL bShowGraphic, CCossImage *m_pImage,
			  double *lt, double *la, double *lb, double *pX, double *pY);