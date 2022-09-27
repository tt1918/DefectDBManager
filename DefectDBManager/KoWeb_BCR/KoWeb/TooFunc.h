#define LINE_YSTEP	15
#define LINE_XSTEP	64
#define LINE_XGAP	32


void WEB_TooYoung();
int FindCandiOnePoint(LPBYTE fmS, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY, CANDIDATA *pCandi);

//투영라인 검출 (MD라인 살짝 각도 있는 것도 됨)
void TOO_FindMDLine(); //투영 라인검출
	int FindLineBW_TOO_A(int nID, int nType, LPBYTE fm, int left, int top, int right, int bottom, int pitch, int *pX, int *pValue, int *pAngle, int *pOverKill);
		int FindMountainCount(long *pProfile, int left, int right, int nValue);
		int FindBestLine(int nPos, int nXCnt, int nYCnt, long ArrayValue[LINE_YSTEP][LINE_XSTEP], int *nXDiffMaxArray, int *nAngle);	
	int CheckOverKillNELALine(LPBYTE fm, int nPosX, int nPosY, int pitch, int nShowOffset, int nX1, int nX2, double dLimit);

//투영 TD라인 검출
void TOO_FindTDLine();
	int FindLineBW_TOO_TD(int N,LPBYTE fm, int left, int top, int right, int bottom, int pitch, long *pProfile, int *pX, int *pValue);

//투영 흑 얼룩검사
void WEB_TooBlackMura();

//투영 흑 군집
void TOO_FindBGroup();


//투영반사 : 찍힘검출
void WEB_TooYoung_Press();