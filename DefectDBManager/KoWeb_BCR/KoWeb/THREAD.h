

UINT THREAD_Save(LPVOID lParam);
UINT THREAD_SaveCompImage(LPVOID lParam);
UINT THREAD_ShowImage(LPVOID lParam);
UINT THREAD_MultiProcess(LPVOID lParam);


void ThreadFunc_MakeFlat_LineImage();	//Grab이미지를 Flat이미지로 8분할 해서 처리함.
void ThreadFunc_MakeFlat_Too();			//Grab이미지를 Flat이미지로(투영)

void ThreadFunc_FindCandiPoints();		//후보점 구함
void ThreadFunc_FindCandiPointsToo();	//투영후보점 구함

void ThreadFunc_FindCandiArea() ;	//Area 성

int ThreadFunc_MakeRotateImage(LPBYTE fmS,  LPBYTE fmD, int left, int top, int width, int height, int pitch, int xLimit, int yLimit, double Cx, double Cy, double xOff,double yOff, double angle);


int ThreadPerspectiveTR(); //이미지 외곡보정 (투영 LR)

int ThreadMakeCompression(LPBYTE fmS, LPBYTE fmD);
int ThreadMakePyramid(LPBYTE fmS);
		
//Emphsize 이미지 만듦
int ThreadEmphasize(LPBYTE fmS, LPBYTE fmD, int width, int top, int bottom, int pitch, int nSizeX, int nSizeY, double dFactor);

//BInary
int ThreadBinary(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nTh);


//투영 MD라인
void ThreadFunc_FindMDLine(LPBYTE fm, int nX1, int nX2, int nHeightSub, int pitch);

//투영 TD라인
void ThreadFunc_FindTDLine(LPBYTE fm, int nX1, int nX2, int nHeightSub, int pitch);

//스크라치 검사일부
void ThreadFunc_FindScratch(LPBYTE fm, int nX1, int nX2, int nHeight, int pitch, long *pKProj, int  nInspY, int nJumpY, int  nUpperCut, int *pKProjLocalArray);

int ThreadFilterVerEdge(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch);

#define COMPRESSION_PART	4
#define PYRAMID_PART		4
#define IMAGE_FLAT_PART		4 
#define MEMCPY_PART			4