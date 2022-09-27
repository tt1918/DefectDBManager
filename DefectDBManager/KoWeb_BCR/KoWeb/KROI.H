#ifndef KROI_DEFINE
#define KROI_DEFINE


#define MAXTRACKERNUMBER				200		//최대 ROI의 수 
#define MAXNAMELENGTH					24		//ROI 이름의 최대 길이 
#define ROI_PI							3.141592653589793238466433832795


typedef struct TPOINT_
{
	double x;
	double y;
}TPoint;


typedef struct KROI_
{
	bool        isResize;		//Resize모양이면 true임. 
	bool        full;			//ROI의 크기가 영상 전체이면 true
	
	RECT		rect;
	int			nAngle;			//-90 ~ 90

	bool		bAutoThreshold;
	bool        bInverse;
	int			nThreshold;
	int			nThresholdL;	//아래쪽 Threshold		//@A1

	TPoint		pt[4];			//ROI의 각도가 있을때 4점의 좌표 

	bool		bShow;			//True면 보이고 그렇지 않으면 숨김.

	int         nNGCount;

}KROI;

class CROIControl
{
public:
	int			g_CurWindowNum;

	int			m_WIDTH;
	int			m_HEIGHT;
	LPBYTE      m_fmD;

	//저장되는 데이타 -------------------------------------------------------
	BOOL		m_FlagShowROI;								//false면 모든 ROI 안보임. 
	BOOL		m_FlagEnableROI, m_FlagEnableROIOld;		//false면 보이지만 변경시킬수는 없다. , m_FlagEnableROI의 저장
	BOOL		m_FlagEnableOneROI;							//false면 보이지만 변경시킬수는 없다. 
	BOOL		m_FlagShowText;
	BOOL		m_FlagShowCenterLine;
	int			m_TrackerNumber;
	COLORREF	m_ROIColor;
	COLORREF    m_TextColor;
	KROI		m_Tracker[MAXTRACKERNUMBER];
	TCHAR 		m_TrackerName[MAXTRACKERNUMBER][MAXNAMELENGTH];
	TCHAR		m_OneTrackerName[MAXNAMELENGTH];

	//저장되지 않는 데이타 --------------------------------------------------
	double		m_dScaleX;									//현재 Display Scale 1이면 Origin  <1 이면축소 >1 이면 확대
	double		m_dScaleY;									//현재 Display Scale 1이면 Origin  <1 이면축소 >1 이면 확대
	int         m_nPanX, m_nPanY;							//Display Pan 값 
	int			m_nResizeCount;								//ROI중에서 Size변경 가능하게 표시된 ROI수 
	bool        m_IsROIClicked;								//마우스 왼쪽 버튼에서 True가 되면 Mousemove시 ROI를 변경시킨다.
	bool		m_TrackerON;								//왼쪽 버튼을 눌러서 Size를 변경할수 있는 상황이 됨
	int         m_nHitTest;
	bool        m_bTrackerHandleIN;							//Tracker의 Coner를 IN으로 표시 
	POINT		m_cOldPnt;									//마우스 왼쪽 버튼 눌렀을 때의 시작지점 
	double		m_dOffsetX;
	double		m_dOffsetY;
	double		m_dOffsetR;
public:

	void Initialize(int nWidth, int nHeight);				//사용하고자하는 AREA 설정 
	void Close();
	void LoadDefaultROI();									//
	int  SaveROI(CString filename);
	int  LoadROI(CString filename);

	void ShowROI(bool flag);								//ROI 보였다 안보였다 하는 함수 
	bool GetShowROI();
	int  SetShowROI(TCHAR * RoiName, bool flag);			//ROI를 보였다 안보였다함.
	int  SetShowROI_ONE(TCHAR * RoiName, bool flag);		//1개 ROI만 보이고 나머지는 보이지 않음
	void EnableROI(bool flag);								//false면 ROI가 보인다 하더라도 크기를 벼경할수 없다.
	bool GetEnableROI();
	void EnableROIOne(bool flag, TCHAR * RoiName);			//해당한는 ROI만 Enable됨.
	void ShowTextROI(bool flag);							//Text를 보였다 안보였다 하는 함수 
	void ShowCenterLine(bool flag);							//중심에 Center Line을 보일것면 True로 set한다.
	void ResetResize();										// isResize를 모두 false로 시킴.
	void SetActiveROI(TCHAR * RoiName);						//해당 이름의 ROI를 Active시킴
	int  GetActiveROI(TCHAR *RoiName);						//현재 Active된 ROI이름을 읽어온다.
	int  GetROINumber(TCHAR * RoiName);						//해당이름으로 ROI번호를 알아내는 함수 
	int  GetROIName(int nRoiNumber, TCHAR *RoiName);			//ROI번호를 주면 ROI의 이름을 알아내는 함수 
	void SetTrackerHandleIN(bool flag) {m_bTrackerHandleIN=flag;} //Tracker Handle을 안쪽 또는 바깥쪽으로 
	void SetROIColor(COLORREF color){ m_ROIColor=color;};	//ROI 색깔 바꾸는 함수 
	void SetTextColor(COLORREF color){m_TextColor=color;};	//ROI Text 색깔 바꾸는 함수 
	int SetROIRect(TCHAR * RoiName, int left, int top, int right, int bottom);   //ROI의 크기를 변경한다.
	void SetROIMaxSize(TCHAR * RoiName, bool flag);								 //flag=true면 ROI의 크기가 변경가능한 State가 된다.

	int  AddROI(TCHAR * RoiName, int left, int top, int right, int bottom);		//ROI추가 함수 
	int  DeleteROI(TCHAR * RoiName);											//ROI삭제 함수 
	void  DeleteAllROI();														//모든 ROI삭제 함수 
	int  GetROIData(TCHAR * RoiName, KROI *pROI);								//ROI의 정보를 얻어오는 함수 (맨 처음이 @또는 /인경우는 Offset을 주지 않는다.)
	int  GetROIData_(TCHAR * RoiName, KROI *pROI);								//ROI의 정보를 얻어오는 함수 (그런것 없다.)
	int  GetROIRegion(TCHAR * RoiName, int *left, int *top, int *right, int *bottom);	//해당 ROI영역얻는 함수 
	int  GetROIRegionPt(TCHAR * RoiName, TPoint *pt);

	int  LButtonDown(UINT nFlags, CPoint point, CWnd *pWnd);					//
	bool OnMouseMove(UINT nFlags, CPoint point, CWnd *pWnd);	//마우스 Click한 상태에서 Call되었으면 true return
	void OnLButtonUp(UINT nFlags, CPoint point, CWnd *pWnd);

	double GetScale()	{return m_dScaleX;};
	double GetScaleX()	{return m_dScaleX;};
	double GetScaleY()	{return m_dScaleY;};
	int  GetActiveNumber(POINT point);				//point에 해당하는 ROI번호를 return 한다.(이번호는 임의로 바뀐다)
	int  GetROIAngle(int nROINumber);
	int	 SetROIAngle(TCHAR * RoiName, int nDegree);	//ROI에 각도를 넣는다.(-90~90)
	int  SetROIAngle(int nROINumber, int nDegree);	//ROI에 각도를 넣는다.(-90~90)
	int  SetROIOffset(double dOffsetX, double dOffsetY, double dOffsetR);	//ROI에 Offset을 설정한다.
	int  GetROIThreshold(int nROINumber, BOOL *bAutoThreshold, BOOL *bInverse, int *nThreshold, int *nThresholdL);//@A	
	int  SetROIThreshold(int nROINumber, BOOL bAutoThreshold, BOOL bInverse, int nThreshold, int nThresholdL);//@A
	bool PtInPolygon(TPoint poly[], int nSize, POINT pt);
	int  PtInROI(int nNum, CPoint point);

	int  SetResizeShapeROI(CWnd *pWnd, int left, int top, int right, int bottom); //해당영역에 있는 ROI를 Active모양으로 바꿈
	int  ResetResizeShapeROI();
	int  ChangePointPosition(long Px, long Py, long *Px_, long *Py_);
	int  GetActiveROIRegion(int *left, int *top, int *right, int *bottom);
	int  ROIMove(int x, int y);
	void ROISize(int x, int y);


	void NV_Binary(LPBYTE fm, KROI cROI);
	int  NV_AutoThreshold(int nInverse, LPBYTE fm, KROI cROI, int *threshold, int *thresholdL);
	void NV_GrayDisplay(LPBYTE fmSrc, LPBYTE fmDst, KROI cROI);
	void NV_BinaryDisplay(LPBYTE fmSrc, LPBYTE fmDst, KROI cROI);
	void NV_GetAreaNBinary(LPBYTE fm, KROI cROI, int threshold, int *BlackArea, int *WhiteArea, bool flag);
	void NV_GetAreaCenterNBinary(LPBYTE fm, KROI cROI, int threshold, int IsBlackCenter,
								  int *BlackArea, int *WhiteArea, double *Cx, double *Cy, bool flag);

	void SetScale(double dScaleX, double dScaleY);
	int SetScaleNOffset(int nZoom, int nPanX, int nPanY);

	void DrawROI(CDC *pDC);
	void DrawRect(CDC *pDC, CBrush *pBrush, int nROI);

	//Mapping함수 
	int NV_MAPImage(LPBYTE fm, KROI cROI, LPBYTE *fmD, int *Width, int *Height);
	int NV_MAPImagePixel(LPBYTE fm, KROI cROI, LPBYTE *fmD, int *Width, int *Height);
	void NV_GetOriginalPos(KROI cROI, double x, double y, double *newx, double *newy);

	//ROI와 관계없이 사용할 수 있는 함수
	int  NV_MAPImage_USER(LPBYTE fm, int pitch, TPoint *pt, int nAngle, LPBYTE fmD, int nWidth, int nHeight, int nLimitX, int nLimitY, int nTrans);
	void NV_GetOriginalPos_USER(TPoint *pt, int X0, int Y0, int nWidth, int nHeight,  int nAngle, int nTrans, double x, double y, double *newx, double *newy);
private:
	int  PointRotation(double Cx, double Cy, double dAngleDeg, double Px, double Py,  double *Px_, double *Py_);
	void NV_GetAreaInRect_SubRoutine(TPoint p1, TPoint p2, TPoint p3, TPoint p4, double *a, double *b, double *minY, double *maxY);
	int  GetMinLength(int nNum, CPoint point);

};

extern CROIControl g_ROI[MAX_IMAGE];
#endif


