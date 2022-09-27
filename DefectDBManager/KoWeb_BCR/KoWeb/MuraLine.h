#ifdef USE_DAKANO_LINE

void WEB_FindTACLine_Level_NEW();
	void OneLineMapUpdate(int N, int nFrame, LPBYTE fmUp, LPBYTE fmDn, LPBYTE fmMap, int *pValueUp, int *pPosXUp, int *pValueDn, int *pPosXDn, int *pValueUpRel, int *pValueDnRel);
void WEB_FindTAC_TDLine_Level();
void DakanoDataReset();


void FuncBaseLineSetting();
int FuncBaseLineSave(CString sFileName);
int FuncBaseLineLoad(CString sFileName);

extern CString l_String[20];
extern int l_nStringCnt;

extern int l_nJumpEnd[2];
extern int l_nOneLineAvg[2];
extern int l_nOneLineLowCnt[2];
extern int l_nOneLineMax[2];
extern int l_nOneLineMin[2];
extern int l_nDKMaxLineUp[2];
extern int l_nDKMaxLineDn[2];
extern int l_nDKMaxLineUpRel[2];
extern int l_nDKMaxLineDnRel[2];



#endif




