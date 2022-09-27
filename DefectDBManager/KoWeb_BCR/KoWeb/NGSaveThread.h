

#define SAVE_NG_NUM		100

#ifdef SAVE_NG_THREAD 
	extern	LPBYTE  g_SaveImage[SAVE_NG_NUM];
	extern	CString g_SaveStr[SAVE_NG_NUM];
	extern	int     g_SaveIn;
	extern	int     g_SaveOut;
	extern	int     g_SaveOver;

	extern  int     g_SaveNGCnt;  //불량 한줄 쓰는 것
#endif

void InitNGSaveThread();
void CloseNGSaveThread();
UINT ThreadSaveNGImage(LPVOID lParam);
UINT ThreadSaveNGString(LPVOID lParam);

void InputNGImage(LPBYTE pImage, CString str);
void OutputNGImage();

void SaveNGString(int nCnt);

//InputNGImage(g_Defect.m_pImage[m_nShowNGID], _T("C:\\Nexteye\\AAA.bmp"));	저장하려면 이렇게 하면 됨. 불량과 불량명