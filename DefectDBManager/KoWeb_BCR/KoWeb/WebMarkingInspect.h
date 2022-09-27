
void WEB_FindMarking();
	int MakePyramidImage_Lowcut(LPBYTE fm, int width, int height, int pitch, int nLowCut, LPBYTE Profile, int left, int right, double dLimit, double *dValue, int *nPosX, int *nPosY);
	int IsOverKillForNEL4Mark(LPBYTE fm, int nType, int nX, int nY, int pitch, int nGrabHeight, int nDnValue, double dMarkValue, int *pRetValue);
	int IsNGImage(LPBYTE fm, int left, int top, int right, int bottom, int pitch);