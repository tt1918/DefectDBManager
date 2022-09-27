
extern double l_SX[10];
extern double l_EX[10];

void ResetEdgeData();

void GetImageProfile();
	void GetImageEdge();

int CheckBoundaryOpticOverKill(LPBYTE fm, int nX, int nY, int pitch, int nGrabHeight, int Value); //경계반사 과검인지 아닌지 Check, 과검이면 1

double GetAverage(LPBYTE fm, int left, int top, int right, int bottom, int pitch);
void GetMinMax(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int *pMax, int *pMin, double *pSharp);
int GetWaterPrint(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nHighCut, int nOffset, int nOffsetValue, int nMinYLength, double *pValue); //물때 얼룩 값

//Level값 얻는 함수
int GetLevel(double dValue, double *pValue);
int GetLevel(int dValue, int *pValue);
int GetLevel(double dValue, double dSize, int *pValue, double *pSize);
int GetLevel(double dValueUp, double dValueDn, double dSize, int *pValueUp, int *pValueDn, double *pSize);

bool IsInspectArea(int nX); //nX Pixel의 위치가 In Position인지 체크하는 함수

int WEB_ImageCheck();

//Check Lami Press
int FindLamiPress(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nThUp, double *pCompactness, int *pSizeX, int *pSizeY);

//마킹인지 확인하는 함수
int CheckDotMarking(LPBYTE fm, int x, int y, int xMin, int xMax, int pitch, int nHeight, int nThreshold, double dScale);
int CheckDotMarking1(LPBYTE fm, int x, int y, int xMin, int xMax, int pitch, int nHeight, int nThreshold, double dScale);

//눌림과검체크
int CheckPressOverkill(LPBYTE fm, int left, int top, int nSizeX, int nSizeY, int pitch, int nKernel, int nBaseValue, double dLimitValue);

//일정영역의 합으로 Min Max 값과 그 위치 구하는 함수
int FindMinMaxPos(LPBYTE fmS, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY, int *nMinPosX,int *nMinPosY, int *nMinValue, int *nMaxPosX, int *nMaxPosY, int *nMaxValue);


#ifdef USE_SK_BAT
void GetImageEdge_BAT();
int FindFitLine(LPBYTE fm, int left, int top, int right, int bottom, int pitch, 
	int nLineType, int nObject, int nDir,  int nMethod,  int nGrayTh, int nSlopeTh, int nAvgLine, int nSkipPixel,int nAllowGap,
	double *pT, double *pA, double *pB);
#endif