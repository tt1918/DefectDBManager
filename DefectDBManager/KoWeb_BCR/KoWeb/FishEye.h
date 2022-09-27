int CheckFishEye(LPBYTE fm, int left, int top, int right, int bottom, int pitch);

int FishEyeFlat(LPBYTE fm, LPBYTE fmD, int left, int top, int right, int bottom, int pitch,  int *pWValue, int *pBValue, double *pDist, double *pSTD);
int FindBlob(LPBYTE fmGray, LPBYTE fm, int left, int top, int right, int bottom, int pitch, int ThUp, int *pArea,  int *pCompactness, double *dDiff);

int BinaryDilate(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY);
int BinaryErode(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY);


void ImageCompress1(LPBYTE fm, LPBYTE fmD, int width, int height, int pitch, int nLevel, int lowcut, int *pWidth, int *pHeight);