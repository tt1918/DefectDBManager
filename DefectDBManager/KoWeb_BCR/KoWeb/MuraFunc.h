void WEB_MuraTAC();		//TAC얼룩 검사비슷하게 White
void WEB_MuraTACLine();		//청주 라인검사
void WEB_MuraSTD();
void WEB_MuraMalloc();
void WEB_MuraFree();


void WEB_MuraSTD_White();

int GetDefectInfoTAC(LPBYTE fm, int nIsBlack, int nX, int nY, int pitch,  double *pSize);
int CheckTACJumpping();

void WEB_MuraTACLine_COS6();		//오창 라인검사
void WEB_MuraTACLine_TD();