#if GRABBER_TYPE==BASLER_GRABBER


#define PYLON_2_2		1
#define PYLON_3_0		2
#define PYLON_4_2		3
#define PYLON_5_0		4
#define PYLON_5_1		5

#define BASLERVERSION  PYLON_2_2

#define MAX_CAM			1
#define MAX_BUFFER		3

#include <pylonc/PylonC.h>	

class CGrabber  
{
public:
//l/10
	//-------------------------원래 있던 함수및 변수 ----------------------
	BOOL Open(bool bMemInit, HWND hWnd);
	void Close(bool bMemInit);
	CGrabber(CWnd* pwnd=NULL);
	virtual ~CGrabber();
	void GetExposureTime(double *dValue);
	void SetExposureTime(int nMicroSec, int nApply);
	void SetYOffset(int nOffsetY);
	void SetTriggerMode(int nCam, bool bPeriod);
	void HookStart();
	void HookStop();
	LPBYTE GetProcessBuf();
	void SetTrigger(int flag);
	LPBYTE GrabImagePtr();
	long m_nGrabX;  //1회 Grab시 영상크기 X
	long m_nGrabY;  //1회 Grab시 영상크기 Y

	long m_nSizeX;
	long m_nSizeY;	
	long m_nExposure;
	CWnd*	m_pParent;	
	//---------------------------------------------------------------------

	//Scout Camera 세팅에 사용되는 변수----------
	size_t NumOfCams;		//2.2

	CString strError;
	TCHAR StrBuffer [100];
	TCHAR sCameraSerial[MAX_CAM][80];
	//---------------------------------------------
															
};
extern CGrabber	g_Grabber;
UINT BaslerGrabThread(LPVOID pParam);
UINT BaslerGrabThreadSub(LPVOID pParam);
#elif GRABBER_TYPE==NO_GRABBER













#endif