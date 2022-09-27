
void SetExposureTime_Serial(int nExposure);
void SetGain_Serial(int nGain);

#if GRABBER_TYPE==MIL_GRABBER
	#define MAX_BUFFER	3

#ifdef USE_MIL10
	MIL_INT MFTYPE ProcessingFunction(MIL_INT HookType, MIL_ID HookId, void MPTYPE *HookDataPtr);
#else
	long MFTYPE ProcessingFunction(long HookType, MIL_ID HookId, void MPTYPE *HookDataPtr);
#endif

	class CGrabber  
	{
	public:
		CWnd*	m_pParent;	

		CGrabber(CWnd* pwnd=NULL);
		virtual ~CGrabber();
		bool Open(bool flag,HWND hWnd);
		void Close(bool flag);

		void HookStart();
		void HookStop();

		void SetExposureTime(int nUserExpo, int nApply);
		void SetGainChange(int nGain);

		LPBYTE GrabImagePtr();
	};
	extern CGrabber	g_Grabber;
	extern bool	l_bMilExposureChange;

#if CAMERA_TYPE==CAMERA_BASLER
	const char* GetCLSerialDir();
	typedef CLINT32 ( __cdecl *PFN_CLSERIALINIT)(CLUINT32 serialIndex, hSerRef* serialRefPtr);
	typedef void    ( __cdecl *PFN_CLSERIALCLOSE)(hSerRef serialRef);
	typedef CLINT32 ( __cdecl *PFN_CLGETNUMSERIALPORTS)(CLUINT32* numSerialPorts);
	typedef CLINT32 ( __cdecl *PFN_CLGETSERIALPORTIDENTIFIER)(CLUINT32 serialIndex, CLINT8* PortID, CLUINT32 *bufferSize);
	typedef CLINT32 ( __cdecl *PFN_CLSETBAUDRATE)(hSerRef serialRef, CLUINT32 baudRate);
	void SetCameraSerialPort();

	extern hSerRef hRef;
    extern CL_SER_FUNCPOINTERS funcs;
#endif

#elif GRABBER_TYPE==NO_GRABBER

	class CGrabber  
	{
	public:	
		CGrabber(CWnd* pwnd=NULL);
		virtual ~CGrabber();
		bool Open(bool flag, HWND hWnd);
		void Close(bool flag);

		void HookStart();
		void HookStop();

		void SetExposureTime(int nUserExpo, int nApply);
		void SetYOffset(int nOffset);
		LPBYTE GrabImagePtr();
		void SetTrigger(int /*flag*/){};
		void SetTriggerMode(int /*nCam*/, bool /*bPeriod*/){};
	};
	extern CGrabber	g_Grabber;
	extern int m_nSimulRun;
	extern bool	l_bMilExposureChange;
#endif