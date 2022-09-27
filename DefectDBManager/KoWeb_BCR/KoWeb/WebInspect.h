
extern LPBYTE l_fmPyramid[5];	
extern int    l_nPyramidWidth[5];
extern int    l_nPyramidHeight[5];
extern double l_dTime[10];


void WEB_PenMark();
void WEB_SpotInspect();
	void MakeDefectRect(int nDefect, int nX, int nY, int nLeft, int nRight);
	void MakeDefectRect_BigDefect(int nX, int nY, int nLeft, int nRight, int *left, int *top);
	void MakeDefectRect_BigDefect1(int nDefect, int nX, int nY, int nLeft, int nRight);
	int CheckDefectOverlap();
	void CheckPriorLevel(int nIsPrior);

void WEB_WingInspect();

int CheckDefectOverlapAdjust();
void MakeDefectTempRect(int nX, int nY, int nLeft, int nRight);

void WEB_LineKipo();	//기포라인 (NEL7)

void WEB_MultiVerLine();//세로선얼룩 (NEL6)  2동코팅2호기만 사용


void WEB_AngleLine();   //꺽임검사 (남경? 요청사항)

void WEB_BATCurr();		//BAT 검사에서 Curr검사

//불량이미지 복사----------------------------------------------------------
int CopyNGImage(bool bFlat);
int CopyNGImageTemp(LPBYTE fm, LPBYTE fmD, int left, int top, int right, int bottom, int pitch);
int CopyNGImageFM(LPBYTE fm, int pitch);
void CopyNGImagePart(bool bFlat);
int CopyNGImageFMArea(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nGray);


void CheckArea(CRect *rect); //이미지 범위

void ImageCompression(unsigned char *pSrc, unsigned char *pDest, unsigned int nImageHeight, unsigned int nPitch, unsigned int nStartX, unsigned int  nEndX);


//경계반사 과검제거
void RemoveBumpOverkill(LPBYTE fm, int pitch, int nGrabHeight);

//DOT 마크 과검제거
void RemoveMarking(LPBYTE fm, int pitch, int nGrabHeight, int nThreshold);

//MY_REVE에서 사용함
void CheckKipoPress();

//nYPos주변에 있는 불량은 모두 제거함
void RemoveDefectByYPos(int nYPos,int nOffsetPixel);