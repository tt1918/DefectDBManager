void WEB_Scratch();
	int FindScratch(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nType, int nUpperCut, int nLowerCut, int nInspY, int nJumpY, int nErode, int nThUp, int nThDn, int *pProj, int *posx, int *posy, int *value, int *AverageScratch, int *MaxScratch);
		int IsOverKillForScratch(LPBYTE fm, int nX, int nY, int pitch, int nHeight, int nValue);
		int CheckRealScratch(LPBYTE fm, int nX, int nY, int pitch, int nMaxWidth, int nMaxHeight, int nMinLength);

void WEB_LongScratch();
void WEB_LongScratchSimple();   //평활화 할때 구했던 Profile로 양 옆(Offset3)보다 밝은곳 제일 밝은곳 1개 구해서 100곱한 값으로 긴 라인 구함.

void WEB_ScratchBlob_White();


