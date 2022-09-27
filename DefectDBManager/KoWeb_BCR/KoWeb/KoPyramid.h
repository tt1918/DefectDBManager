#define DEFECT_IMAGE_SIZE		128

//압축----------------------------------------------------
bool OpenPyramid(int nWidth, int nHeight);
void ClosePyramid();
void MakePyramid(LPBYTE fmSrc, int nStartX, int nWidth);
void MakePyramidSub(int N);

//불량 후보 구하기-----------------------------------------
int FindCandiFromPyramid(int nStartX, int nWidth, int nGap);


int FindCandiSorting(int nStartX, int nWidth, int nGap);

//Size와 Value구하는 함수--------------------------------------------------------
void GetSizeNValue(int nType, LPBYTE fm, int nDefectY, int left, int top, int width, int height, int pitch, int nAveValue, int nThUp, int nThDn, int nAttachPixel, int nSizeMethod, 
					double dScaleX, double dScaleY, int *pValueUp, int *pValueDn, double *pSize, double *pImulKipoSize, int *pCx, int *pCy);
	int GetAveValueByHistogram(LPBYTE fm, int left, int top, int width, int height, int pitch); //Histogram으로 평균밝기를 구한다

void GetSizeNValueTinyB(LPBYTE fm, int left, int top, int width, int height, int pitch, int nAveValue, int nThDn, int nAttachPixel, int nSizeMethod, double dScaleX, double dScaleY, int *pValueDn, double *pSize); //Tiny Black	

//단순한 Gradient인지 Check	 :   1 이면 단순 Gradient
int CheckSimpleGradient( int nCenterX, int nCenterY,  int X1, int X2, int nY2, double dLimitvalue);


//0 : TPW아님
//1 : TPW임
int HoleCheck(LPBYTE fm, int nCx, int nCy, int pitch, int imageH, int nAveValue, int nThUp, double dScaleX, double dScaleY, int nFrameID);