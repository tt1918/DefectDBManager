long FIndROIInfo(LPBYTE fm, int left, int top, int right, int bottom, int pitch);
int PointRotation(double Cx, double Cy, double dAngleDeg, double Dx, double Dy, double Px, double Py,  double *Px_, double *Py_);
int FindLineFunction(double x1, double y1, double x2, double y2, double *t, double *a, double *b);

double GetStdevArea(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch);
double FindAveArea(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, int nSkip);
double FindStdevArea(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch);

void Binary(LPBYTE fmS, LPBYTE fmD,int left, int top, int right, int bottom, int pitch, int nThreshold);
void CompressSampling(LPBYTE fmS, int left, int top, int right, int bottom, int pitch, int nSkip, LPBYTE fmD, int nThreshold, int *pWidth, int *pHeight);
int  InterpolationTransform_SubPixel(CPoint *POrg, LPBYTE fmOrg, int OrgPitch, CPoint *PMap, LPBYTE fmMap, int MapPitch);


void Registermaster(LPBYTE fmS,int nID, int nSmooth, int nAllowPixel) ;
long MakeMaxMinImage(LPBYTE fm, LPBYTE fmMax, LPBYTE fmMin,  long left, long top, long right, long bottom, int pitch, int nAllowLimit);
long FindDiffImage(int nSmooth, int nColor, LPBYTE fmSrc, LPBYTE fmMax, LPBYTE fmMin, LPBYTE fmThUp, LPBYTE fmThDn, LPBYTE fmDiff, LPBYTE fmBinary,
				   int nMakeDiffImage, long left, long top, long right, long bottom, long pitch); 

void Dilate_Gray(LPBYTE fmSour, LPBYTE fmDest, int left, int top, int right, int bottom, int nPitch,long* pKernel, int nKernelSizeX, int nKernelSizeY);
void Erode_Gray(LPBYTE fmSour, LPBYTE fmDest, int left, int top, int right, int bottom, int nPitch,long* pKernel, int nKernelSizeX, int nKernelSizeY);

//할콘처럼 만든 함수
int Emphasize(int N, LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY, double dFactor);
int MakeEmphasize(int N);
int HighPass(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY);
int COSS_Mean(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY);
int Mean_1(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY);
void Filter2DFVer(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY, int nAbsolute, int nDivide);
int BinaryDilate(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY);
int BinaryErode(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY);

int FindImageAngle(LPBYTE fm,  int left, int top, int right, int bottom, int pitch, double *angle);

BOOL PtInPolygon(POINT poly[], int nCount, int nX, int nY);

int GetStdevNBrightArea(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, double *pStdev, double *pAverage);

//이미지를 마스트이미지와 맞추기 위해서 LUT구하는 함수 (nMasterTH: Master이미지  AutoThreshold, 좌우피크:nMasterMaxL, nMasterMaxR
//HistoMaster 마스터이미지 히스토그램, Histo:비교이미지 히스토그램
void GetImageLUT(int nMasterTH, int nMasterMaxL, int nMasterMaxR,  int *HistoMaster, int *Histo, LPBYTE LUT);
int AutoThresholdFromHisto(int m_nHistoData[256]);

int MakeRotateImage(LPBYTE fmS, LPBYTE fmD,  int left, int top, int right, int bottom, int pitch, int xLimit, int yLimit, 
					double Cx, double Cy, double xOff, double yOff, double angle);

void Rotate(LPBYTE dest, LPBYTE src, int width, int height, float x0,float y0,float angle);

int GetStdevMap(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, int nW, int nH, double *pStdev, int *nSW, int *nSH);
int GetAvgMap(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, int nW, int nH, double *pStdev, int *nSW, int *nSH);

void Func_Filter(int nFilter, LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch);


void ImageFlipHor(LPBYTE fm, int left, int top, int right, int bottom, int pitch);
void ImageFlipVer(LPBYTE fm, int left, int top, int right, int bottom, int pitch);
int ImageElongate(LPBYTE fm, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nX, int nY);


int GetHBOSize(LPBYTE fm, int left, int top, int right, int bottom, int pitch, double dAvg, int nCoreTh, double dCloudTh, int *IsWhiteSpot, int *Cx, int *Cy, int *pValue, int *CoreSize, double *dCloudValue,int *CloudSize);
//int GetHBOSize(LPBYTE fm, int left, int top, int right, int bottom, int pitch, double dAvg, int nCoreTh, int n2ndTh, double dCloudTh, int *IsWhiteSpot, int *Cx, int *Cy, int *pValue, int *CoreSize, int *MiddleSize, int *CloudSize);
	void ImageFilter_withStd(LPBYTE fm, LPBYTE fmD,int left, int top, int right, int bottom, int pitch, double dAvg, double dBase, int nKerSize, int nUpperLimtit, int nDownLimit);


int Get2Size(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nCoreColor, int nCloudColor, double dAvg, int nCoreThW, int nCoreThB, double dCloudThW, double dCloudThB, int *Cx, int *Cy, int *pValue, int *CoreSize,  double *dCloudValue, int *CloudSize);

//수평밝은 수평라인 노이즈 없앰.
void RemoveHorlineNoise(LPBYTE fm, int nX1, int nX2, int height, int pitch, int nThres);

double CheckDiffTopBottom(LPBYTE fm, int pitch, int nX1, int nX2, int height);
double CheckDiffTopBottom(LPBYTE fm, int pitch, int nX1, int nX2, int height, int *pYPos);

double GetStdevData(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch);

//가장 높은 Stdev찾는 함수
double SearchMaxStd_Cut(LPBYTE fm, int nX, int nY, int pitch, int MURA_RECT, int nBaseValue, int nHighCut, int nLowCut, int nLimitX1, int nLimitX2, int nHeight, int nSearchRange, int *pRx, int *pRy);
double FindStd_Cut(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, int nCutH, int nCutL);

//밝기보상
void BrightCompensation(LPBYTE fm, LPBYTE fmT, int nWidth, int nHeight, int pitch, int nType);

extern int l_SumVer[MAX_WIDTH];

//Defect검출후 수평노이즈 없앰
int IsHorNoize(int nNoizeType, LPBYTE fm, int nW, int nH, int pitch, int nDiffValue);
