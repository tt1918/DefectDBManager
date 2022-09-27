void WebKoonJip();
	int IsOverKillForNEL7(LPBYTE fm, int nType, int nX, int nY, int pitch, int nGrabHeight, int nMinCount, int nUpValue, int nDnValue);

//128x128영역에 특정크기 이상이 몇개 있으면 1
int CheckMiseKoonJip(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nTh, double dSize, double dSFx, double dSFy, int nCount);
