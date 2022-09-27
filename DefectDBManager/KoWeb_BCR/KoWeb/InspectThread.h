UINT THREAD_Auto(LPVOID lParam);
UINT THREAD_Inspect(LPVOID lParam);
	void MakeResultData();				//서버에 보낼데이타 만듦 (가변마킹외 )

void SetInspectFlag(bool flag);

void CompNCopy(LPBYTE fmS, LPBYTE fmD, int width, int height); //영상 1/8압축