#include "stdafx.h"
	

#if GRABBER_TYPE==MIL_GRABBER
#pragma comment(lib, "Mil.lib")

MIL_ID	MilApplication;
MIL_ID	MilSystem;
MIL_ID	MilDigitizer;
MIL_ID	MilBuffer[MAX_BUFFER];

long	m_nBand;
long	m_nSizeX;
long	m_nSizeY;
long	m_nPitch;

LPBYTE  g_Buffer[MAX_BUFFER];
LPBYTE  g_fmMIL[MAX_BUFFER];
long	m_nGrabIndex=0;				//Grab마다 증가됨. Grab시작시 0 MAX_BUFFER(3)에 도달하면 0 (즉 3개의 버프를 돌아가며 사용하기 위한 Index)
long	m_nSubGrabIndex=0;			//1 Frame이 여러개 쪼개서 계산할때 Grab한 횟수
long	m_nSubCopyIndex=0;			//1 Frame이 여러개 쪼개서 계산할때 복사한 횟수, 한 Frame에 다 차면 0이 됨.                              
			
long	l_MilExposure=100;
bool	l_bMilExposureChange=false;
int		g_FrameCnt=0;
double g_FrameStart=0, g_FrameEnd;

CGrabber	g_Grabber;

CGrabber::CGrabber(CWnd* pwnd)
{
	m_pParent = pwnd;
}


CGrabber::~CGrabber()
{

}

bool CGrabber::Open(bool flag, HWND hWnd)
{
	int i;
	bool ret=true;
	CString str;


	for(i=0;i<MAX_BUFFER;i++)
		g_Buffer[i]=new BYTE[g_System.m_nImageW*g_System.m_nImageH];


	MappAlloc(M_DEFAULT, &MilApplication);
	if(g_System.m_nVisionBoardType==0)  MsysAlloc(M_SYSTEM_SOLIOS,  M_DEFAULT, M_DEFAULT, &MilSystem);	//M_SYSTEM_RADIENTEVCL, ..	
	else								MsysAlloc(M_SYSTEM_RADIENTEVCL,  M_DEFAULT, M_DEFAULT, &MilSystem);
	MappControl(M_ERROR,M_PRINT_DISABLE);

//	AfxMessageBox(_T("1"));

	if(M_NULL == MdigAlloc(MilSystem, M_DEV0, DCF_FILE, M_DEFAULT, &MilDigitizer))
	{
		AfxMessageBox(_T("DCF File Open Error."));
		ret = FALSE;
	}
	MdigControl(MilDigitizer, M_GRAB_TIMEOUT, M_INFINITE);

	// CC1 Output를 정의해 주지 않으면, CC출력이 되지 않음. 반드시 추가해 주어야 함.
	MdigControl(MilDigitizer, M_CAMERALINK_CC1_SOURCE, M_GRAB_EXPOSURE+M_TIMER1);
	MdigControl(MilDigitizer, M_GRAB_MODE, M_ASYNCHRONOUS);


#ifdef USE_MIL10
	m_nBand		= (long)MdigInquire(MilDigitizer, M_SIZE_BAND,	M_NULL);
	m_nSizeX	= (long)MdigInquire(MilDigitizer, M_SIZE_X,		M_NULL);
	m_nSizeY	= (long)MdigInquire(MilDigitizer, M_SIZE_Y,		M_NULL);
	m_nPitch	= (long)MdigInquire(MilDigitizer, M_PITCH,		M_NULL);
#else
	MdigInquire(MilDigitizer, M_SIZE_BAND, &m_nBand);
	MdigInquire(MilDigitizer, M_SIZE_X,	   &m_nSizeX);
	MdigInquire(MilDigitizer, M_SIZE_Y,    &m_nSizeY);
	MdigInquire(MilDigitizer, M_PITCH,     &m_nPitch);
#endif

//	AfxMessageBox(_T("2"));

	for(i=0; i<MAX_BUFFER; i++)
	{
		MbufAlloc2d(MilSystem, m_nSizeX, m_nSizeY, 8+M_UNSIGNED, M_IMAGE+M_GRAB, &MilBuffer[i]);
		g_fmMIL[i]=(LPBYTE)MbufInquire(MilBuffer[i], M_HOST_ADDRESS, M_NULL);
		MbufClear(MilBuffer[i], 0xff);
	}

#ifdef USE_MIL10
	m_nPitch	= (long)MdigInquire(MilBuffer[0], M_PITCH,	M_NULL);
	m_nPitch=g_System.m_nPitch;
#else
	MbufInquire(MilBuffer[0], M_PITCH, &m_nPitch);
#endif


#if CAMERA_TYPE==CAMERA_BASLER
	SetCameraSerialPort();
#endif


#ifdef USE_EXPOSURE_SERIAL
	g_Sio.InitComm(g_Config.m_nComPort, 9600, NOPARITY, 8, ONESTOPBIT, true);

	#if CAMERA_TYPE==CAMERA_PIRANHA
		Sleep(200);
		char szCmd[5];
		memset(szCmd, 0, 5);
		szCmd[0]=27; //ESC
		g_Sio.Write((LPBYTE)szCmd, 1);	
	#endif
#endif

//	AfxMessageBox(_T("3"));

  	return ret;
}

void CGrabber::Close(bool flag)
{
	int i;

	for(i=0;i<MAX_BUFFER;i++)
		delete g_Buffer[i];

	for(i=0; i<MAX_BUFFER; i++)
		MbufFree(MilBuffer[i]);

	MdigFree(MilDigitizer);
	MsysFree(MilSystem);
	MappFree(MilApplication);

#ifdef USE_EXPOSURE_SERIAL
	g_Sio.Close();
#endif
}

void CGrabber::HookStart()
{
	m_nGrabIndex=0;
	m_nSubGrabIndex=0;
	m_nSubCopyIndex=0;

	MdigProcess(MilDigitizer, MilBuffer, MAX_BUFFER, M_START, M_DEFAULT, ProcessingFunction, (void*)m_pParent);
}

void CGrabber::HookStop()
{
	MdigProcess(MilDigitizer, MilBuffer, MAX_BUFFER, M_STOP, M_DEFAULT, ProcessingFunction, (void*)m_pParent);
}



void CGrabber::SetExposureTime(int nUserExpo, int nApply)
{	
	static int nOldExposure=0;
	int nExposure;

	if(nUserExpo<10) return;
	if(nOldExposure==nUserExpo) return;

	if(g_System.m_nUseCamTapeSensor && g_Temp.m_nLotChangeSignal==2) return; //CAMERASENSING: Tape을 카메라로 센싱하는 경우, Unwinder후 Tape검사 범위에 진입하면 Lot Reset전까지는 셔터 바꾸지 않음.
	 
#ifdef USE_EXPOSURE_SERIAL

	#if CAMERA_TYPE==CAMERA_PIRANHA		
		nExposure=nUserExpo;		//유저입력값에 x25를 적용하면 Nano Sec(즉 1000이면 25000 nsec =>25usec)	
	#elif CAMERA_TYPE==CAMERA_BASLER
		nExposure=nUserExpo/10;	//유저입력값에 /100을 적용하면 usec임 (즉 1200이면 12usec)   여기서 10을 나눈이유? 원래는 100을 해야하는데 여기서 10나누고  최종 적용하기전 10을 나눠준다.
	#elif CAMERA_TYPE==CAMERA_ELIIXA		
		nExposure=nUserExpo/10;	//유저입력값에 /10을 적용하면 usec임 (확인요망)
	#endif


	if(nApply == 1)
		SetExposureTime_Serial(nExposure); //바로적용
	else
	{
		l_MilExposure=nExposure;		  //Frame끝나고 적용
		l_bMilExposureChange=true;
	}	
	g_Temp.m_nExposureData=nUserExpo;
#else
	
	if(nApply)
	{
		MdigControl(MilDigitizer, M_GRAB_EXPOSURE_TIME, nUserExpo*25); //nsec (지금 바로 적용)
	}
	else
	{
		l_MilExposure=nUserExpo*25;									  //nsec (지금 바로 안하고 한 Frame 끝나고 함)
		l_bMilExposureChange=true;
	}
	g_Temp.m_nExposureData=nUserExpo;
#endif

	nOldExposure = nUserExpo;
}

void CGrabber::SetGainChange(int nGain)
{
	char szCmd[30];

	if(nGain>=0 && nGain<=2)
	{
		sprintf(szCmd, "w pamp %d\r",nGain);
		g_Sio.Write((LPBYTE)szCmd, strlen(szCmd));
		Sleep(200);
	}
}


#ifdef USE_MIL10
	MIL_INT MFTYPE ProcessingFunction(MIL_INT HookType, MIL_ID HookId, void MPTYPE *HookDataPtr)
#else
	long MFTYPE ProcessingFunction(long HookType, MIL_ID HookId, void MPTYPE *HookDataPtr)
#endif
{
	int i, j, N, M;
	LPBYTE fmT, fmS;
	
	g_FrameCnt++;
	if(g_FrameCnt==2)
	{
		g_FrameEnd=Clock_set();
		if(g_FrameStart!=0)
		{
			if(g_FrameEnd-g_FrameStart) g_Temp.m_dFPS=1000*2.0/((g_FrameEnd-g_FrameStart)*g_System.m_nImagePart);
		}
		g_FrameStart=g_FrameEnd;
		g_FrameCnt=0;
	}

	//제일 먼저 Exposure를 바꾼다.(큰 한 Frame 끝나면 Exposure 바꾼다)
	if(l_bMilExposureChange && (g_Temp.m_nGrabFrameSub%g_System.m_nImagePart==0  ||  (m_nSubGrabIndex+1)==g_System.m_nImagePart))
	{
#ifdef USE_EXPOSURE_SERIAL
		SetExposureTime_Serial(l_MilExposure);
		g_Temp.m_nChangeExpoFrameID=g_Temp.m_nGrabFrame+2;
#else
		MdigControl(MilDigitizer, M_GRAB_EXPOSURE_TIME, l_MilExposure);
		g_Temp.m_nChangeExpoFrameID=g_Temp.m_nGrabFrame+2;
#endif
		l_bMilExposureChange=false;
	}

#ifdef USE_GAIN_SET
	if(g_Temp.m_FlagGainChange && (g_Temp.m_nGrabFrameSub<=1  ||  (m_nSubGrabIndex+1)==g_System.m_nImagePart))
	{
		g_Grabber.HookStop();
		g_Temp.m_FlagGainChange=2;
	}
#endif

	if(g_System.m_nImagePart<=1)
	{
		M=N=m_nGrabIndex; //m_nGrabIndex: 현재 1회 Grab 마다 증가되는 값
		MdigGetHookInfo(HookId, M_MODIFIED_BUFFER+M_BUFFER_ID, &MilBuffer[N]);	
		if(m_nSizeX==m_nPitch)
		{
			if(g_Config.m_nImageInverse)
			{
				fmT=g_Buffer[M];
				for(i=0;i<m_nSizeY;i++)
					for(j=0;j<m_nSizeX;j++)
						*(fmT+m_nSizeX*i+j)=255-*(g_fmMIL[N]+m_nSizeX*i+j);
			}
			else
			{
				memcpy(g_Buffer[M], g_fmMIL[N], m_nSizeX*m_nSizeY);     //<---보통은 여기
			}
		}
		else
		{
			if(g_Config.m_nImageInverse)
			{
				for(i=0;i<g_System.m_nImageH;i++)
				{
					fmT=g_Buffer[M]+m_nSizeX*i;
					fmS=g_fmMIL[N]+m_nPitch*i;
					for(j=0;j<m_nSizeX;j++)
						*(fmT+j)=255-*(fmS+j);
				}
			}
			else
			{
				for(i=0;i<g_System.m_nImageH;i++)
					CopyMemory(g_Buffer[M]+m_nSizeX*i, g_fmMIL[N]+m_nPitch*i, m_nSizeX);
			}
		}
	
		if(m_nGrabIndex+1>=MAX_BUFFER) m_nGrabIndex=0;
		else						   m_nGrabIndex++;

		g_Temp.m_nGrabFrame++;		//Matrox Subframe나눈게 다 찾을때 Grab
		g_Temp.m_nGrabDone=1;	
	}
	else //정반사( 기포눌림 검출시 사용)
	{

#ifdef MARKING_VISION
		N=m_nSubGrabIndex; //m_nSubGrabIndex: 현재 1회 Grab 마다 증가되는 값
		M=m_nGrabIndex;	   //m_nGrabIndex: 한 Frame 마다 증가되는 값
		
		memcpy(g_Buffer[M]+m_nSizeX*m_nSubCopyIndex*m_nSizeY, g_fmMIL[N], m_nSizeX*m_nSizeY);					
						
		if(m_nSubGrabIndex+1>=MAX_BUFFER) m_nSubGrabIndex=0;
		else							  m_nSubGrabIndex++;

		g_Temp.m_nGrabFrameSub++;
		m_nSubCopyIndex++;
		if(m_nSubCopyIndex>=g_System.m_nImagePart/2)
		{
			m_nSubCopyIndex=0;			
			if(m_nGrabIndex+1>=MAX_BUFFER) m_nGrabIndex=0;
			else						   m_nGrabIndex++;

			g_Temp.m_nGrabFrame++;		//Matrox Grab
			g_Temp.m_nGrabDone=1;	
		}
#else
		N=m_nSubGrabIndex; //m_nSubGrabIndex: 현재 1회 Grab 마다 증가되는 값
		M=m_nGrabIndex;	   //m_nGrabIndex: 한 Frame 마다 증가되는 값
		if(m_nSizeX==m_nPitch)
		{
			if(g_Config.m_nImageInverse)
			{
				fmT=g_Buffer[M]+m_nSizeX*(m_nSubCopyIndex*m_nSizeY);
				for(i=0;i<m_nSizeY;i++)
					for(j=0;j<m_nSizeX;j++)
						*(fmT+m_nSizeX*i+j)=255-*(g_fmMIL[N]+m_nSizeX*i+j);
			}
			else
			{
				memcpy(g_Buffer[M]+m_nSizeX*m_nSubCopyIndex*m_nSizeY, g_fmMIL[N], m_nSizeX*m_nSizeY);		
			}
		}
		else
		{
			if(g_Config.m_nImageInverse)
			{
				for(i=0;i<m_nSizeY;i++)
				{
					fmT=g_Buffer[M]+m_nSizeX*(m_nSubCopyIndex*m_nSizeY+i);
					fmS=g_fmMIL[N]+m_nPitch*i;
					for(j=0;j<m_nSizeX;j++)
						*(fmT+j)=255-*(fmS+j);
				}
			}
			else
			{
				for(i=0;i<m_nSizeY;i++)
					CopyMemory(g_Buffer[M]+m_nSizeX*(m_nSubCopyIndex*m_nSizeY+i), g_fmMIL[N]+m_nPitch*i, m_nSizeX);
			}
		}
		
		if(m_nSubGrabIndex+1>=MAX_BUFFER) m_nSubGrabIndex=0;
		else							  m_nSubGrabIndex++;

		g_Temp.m_nGrabFrameSub++;
		m_nSubCopyIndex++;
		if(m_nSubCopyIndex>=g_System.m_nImagePart)
		{
			m_nSubCopyIndex=0;			
			if(m_nGrabIndex+1>=MAX_BUFFER) m_nGrabIndex=0;
			else						   m_nGrabIndex++;

			g_Temp.m_nGrabFrame++;		//Matrox Grab
			g_Temp.m_nGrabDone=1;	
		}
#endif
	}

#ifdef USE_GAIN_SET
	if(g_Temp.m_FlagGainChange==2)
	{	
		g_Temp.m_FlagGainChange=0;
		g_Grabber.SetGainChange(g_Param.m_nGain); //Serial로 Gain을 변경후 
		Sleep(300);
		g_Grabber.HookStart();			
	}
#endif

	return 0;
}

LPBYTE CGrabber::GrabImagePtr()
{
	int N=m_nGrabIndex-1;
	if(N<0) N=MAX_BUFFER-1;
	return g_Buffer[N];
}

#if CAMERA_TYPE==CAMERA_ELIIXA
	void SetExposureTime_Serial(int nExposure)
	{
		//char *szCmd={"w tint 100\r"};
		if(nExposure<10) return;

		char szCmd[30];
		sprintf(szCmd, "w tint %d\r",nExposure);
		g_Sio.Write((LPBYTE)szCmd, strlen(szCmd));
	}
	
	void SetGain_Serial(int nGain)
	{
		static int nOldGain=-1;
		
		if(g_Temp.m_FlagGainChange) return; //0일때만 들어옴.
		if(nGain<0 || nGain>2) return;

		if(nOldGain!=nGain)
		{
			g_Temp.m_FlagGainChange=1;
			nOldGain=nGain;
		}
	}

#elif CAMERA_TYPE==CAMERA_PIRANHA
	void SetExposureTime_Serial(int nExposure)
	{
		if(nExposure<10) return;

		char szCmd[20];
		sprintf(szCmd, "SET %d\r",nExposure*25);
		
		g_Sio.Write((LPBYTE)szCmd, strlen(szCmd));		
	}
	void SetGain_Serial(int nGain)
	{
		static int nOldGain=0;
		char szCmd[30];
		if(nGain>8) return;

		if(nOldGain!=nGain)
		{
			char szCmd[20];
			sprintf(szCmd, "SSG 0 f%d\r",nGain);
			nOldGain=nGain;
		}
	}
#elif CAMERA_TYPE==CAMERA_BASLER
	hSerRef hRef;
    CL_SER_FUNCPOINTERS funcs;

	const char* GetCLSerialDir()
	{
		static char szCLSerialDir[FILENAME_MAX+1];
	
		if (szCLSerialDir[0] == '\0')
		{
			// open the regkey (on win64 builds use the 32bit registry (KEY_WOW64_32KEY))
			HKEY baseKey = NULL;
			if (::RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Cameralink", 0, KEY_QUERY_VALUE, &baseKey) == ERROR_SUCCESS)
			{
				DWORD charCount = (sizeof(szCLSerialDir)/sizeof(*szCLSerialDir));
				if ((::RegQueryValueExA(baseKey, "CLSERIALPATH", 0, NULL, reinterpret_cast<LPBYTE>(szCLSerialDir), &charCount) == ERROR_SUCCESS) && charCount > 1)
				{
					--charCount; // charCount includes term null
					// remove trailing pathsep
					if (szCLSerialDir[charCount] == '\\')
					{
						szCLSerialDir[charCount] = '\0';
					}
				}
				else
				{
					szCLSerialDir[0] ='\0';
				}
				::RegCloseKey(baseKey);
			}
		}
	
		return szCLSerialDir;
	}

	void SetCameraSerialPort()
	{
		CLINT32 res = 0;
		hRef = NULL;
		// build the full path to the DLL
		char szDLLPath[FILENAME_MAX] = {0};
		TCHAR szDLLPathW[FILENAME_MAX] = {0};
   
		strcpy(szDLLPath, GetCLSerialDir());
		strcat(szDLLPath, "\\");
		strcat(szDLLPath, "clSerCom.dll");
  
		_stprintf(szDLLPathW, CA2W(szDLLPath));  //		HMODULE hDLL = LoadLibrary(szDLLPath); 
		HMODULE hDLL = LoadLibrary(szDLLPathW);
		
		if (hDLL == NULL)
		{
			hDLL = NULL;
			AfxMessageBox(_T("clSerCom.dll load fail"));
		}

		// setup funcpointers struct to use with clbpp functions

		funcs.pfnFlushPort =    (PFN_CLFLUSHPORT)   GetProcAddress(hDLL, "clFlushPort");
		funcs.pfnSerialRead =   (PFN_CLSERIALREAD)  GetProcAddress(hDLL, "clSerialRead");
		funcs.pfnSerialWrite =  (PFN_CLSERIALWRITE) GetProcAddress(hDLL, "clSerialWrite");
		funcs.pfnGetErrorText = (PFN_CLGETERRORTEXT)GetProcAddress(hDLL, "clGetErrorText");
	
		// get functionpointers for functions we call in directly clSerXXX DLL
		PFN_CLSERIALINIT              pfn_clSerialInit =              (PFN_CLSERIALINIT)GetProcAddress(hDLL, "clSerialInit");
		PFN_CLGETNUMSERIALPORTS       pfn_clGetNumSerialPorts =       (PFN_CLGETNUMSERIALPORTS)GetProcAddress(hDLL, "clGetNumSerialPorts");
		PFN_CLGETSERIALPORTIDENTIFIER pfn_clGetSerialPortIdentifier = (PFN_CLGETSERIALPORTIDENTIFIER)GetProcAddress(hDLL, "clGetSerialPortIdentifier");
		PFN_CLSERIALCLOSE             pfn_clSerialClose =             (PFN_CLSERIALCLOSE)GetProcAddress(hDLL, "clSerialClose");
		PFN_CLSETBAUDRATE             pfn_clSetBaudRate =             (PFN_CLSETBAUDRATE)GetProcAddress(hDLL, "clSetBaudRate");

		/////////////////////////////////////////////////////////////////
		// get the number of ports available
		CLUINT32 numPorts = 0;
		res = pfn_clGetNumSerialPorts(&numPorts);			
	
		CLUINT32 portIndex((CLUINT32)-1);

		//노출을 MAX 올려도 영상 밝기가 80이 안나오면 그냥 그밝기에 ADD값 만큼 더해서 평균밝기를 올림 >>>>>>>>>>>>>>
		//H/W 문제없으면 사용안함  "C:\\nexteye\\serial.ini"
		//INDEX 는 표준은 5이나 혹시 다른 값을 사용하면 바꿈(이병성)
		char cReturn[50];
		int nSerialIndex=5;	

#ifdef INTERFACE_NEXTEYE_SERVER
		nSerialIndex			=GetPrivateProfileInt(_T("SETTING"),_T("INDEX"),	5,	_T("C:\\Nexteye\\Serial.ini")); 
#else
		nSerialIndex		=GetPrivateProfileInt(_T("SETTING"),_T("INDEX"),	5,	CONFIG_PATH); 
#endif		
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

		if (numPorts > 1)
		{
				portIndex = nSerialIndex;
				/////////////////////////////////////////////////////////////////
				// open the port
				if(CL_ERR_NO_ERR == pfn_clSerialInit(portIndex, &hRef))
				{
					pfn_clSetBaudRate(hRef, CL_BAUDRATE_9600);
					/////////////////////////////////////////////////////////////////
				}			
				else
				{
					AfxMessageBox(_T("Serial Index 확인해 주세요"));
				}
		 }
		else
		{
			// there is only one port, so use it
			portIndex = 0;
		}
	}

	#define ADDRESS_EXPOSURE_TIME 0x01501	//Exposure
	void SetExposureTime_Serial(int nExposure)
	{
		if(nExposure<50) return;

		float frawgain = (float)nExposure/10.0;
		if(frawgain>70) frawgain=70;
		clbbp_SerialWriteRegister(hRef, ADDRESS_EXPOSURE_TIME, (CLINT8*)&frawgain, sizeof(frawgain), 1000, &funcs);
	}
	void SetGain_Serial(int nGain)
	{
		static int nOldGain=0;
		char szCmd[30];
		if(nGain>10) return;

		if(nOldGain!=nGain)
		{
			nOldGain=nGain;
		}
	}
#endif



#elif GRABBER_TYPE==NO_GRABBER
UINT THREAD_Simul(LPVOID lParam);
CGrabber	g_Grabber;
int m_nSimulRun=0;
LPBYTE g_Buffer;
CGrabber::CGrabber(CWnd* pwnd) { }
CGrabber::~CGrabber(){}
bool CGrabber::Open(bool flag, HWND hWnd){g_Buffer=new BYTE[g_System.m_nImageW*g_System.m_nImageH]; return true;}
void CGrabber::Close(bool flag){delete g_Buffer;}
void CGrabber::SetExposureTime(int nUserExpo, int nApply){}
void CGrabber::SetYOffset(int nOffset){}
void CGrabber::HookStart()
{
	int i;

	if(m_nSimulRun) 
		return;

	m_nSimulRun=1;
	AfxBeginThread(THREAD_Simul,		this,THREAD_PRIORITY_NORMAL,0);
}
void CGrabber::HookStop()
{
	m_nSimulRun=0;
}
LPBYTE CGrabber::GrabImagePtr()
{
	return g_Buffer;
}
void SetGain_Serial(int nGain)
{
		
}
/* 이것도 사용하니 지우지 말것---------------------------------------------------
//특정폴더(C:\\TEST\\) 에 있는 File을 1~5까지 반복해서 읽는 THREAD
UINT THREAD_Simul(LPVOID lParam)
{
	int i, pitch;
	CImage image;	
	CString str;
	LPBYTE fm, dst;
	
	while(m_nSimulRun)
	{
		g_Temp.m_nGrabFrame++;   //Simulationb

		str.Format(_T("C:\\TEST\\%d.bmp"), (g_Temp.m_nGrabFrame-1)%5+1);
		
		image.Load(str);
		g_Temp.m_LoadSizeX=image.GetWidth();
		g_Temp.m_LoadSizeY=image.GetHeight();

		fm=(LPBYTE)image.GetBits();
		dst=g_Buffer;
		pitch=image.GetPitch();

		for (i=0; i < g_Temp.m_LoadSizeY; i++,fm+=pitch,dst+=g_System.m_nImageW)
				CopyMemory(dst, fm, g_Temp.m_LoadSizeX);
		
		image.Destroy();

		g_Temp.m_nGrabDone=1;	//Simulationb

		Sleep(1000);
	}
	return 0;
}
*/

//C:\TEST\폴더의 BMP 읽어서 검사
//특정폴더에 있는 File을 차례로 다 읽는 THREAD
UINT THREAD_Simul(LPVOID lParam)
{
	int i;
	int loop;
	int nTime=1000;	//
	int pitch;
	CImage image;	
	LPBYTE fm;
	LPBYTE dst;
	CString str, strfile;
	CString sFolder(_T("C:\\TEST\\"));

	HANDLE hFind;
	BOOL bRet = TRUE;
	WIN32_FIND_DATA	fd;	
	
	str.Format(_T("%s\*.bmp"), sFolder);

	while(m_nSimulRun)
	{		
		hFind = FindFirstFile( str, &fd);
		bRet=true;

		while(hFind != INVALID_HANDLE_VALUE && bRet)
		{
			//if(g_System.m_nSimulServer)
			//{
			//	while(g_Temp.m_nReceiveServerFrame!=1)
			//	{
			//		Sleep(10); 
			//		if(!m_nSimulRun) break;
			//	}
			//	g_Temp.m_nReceiveServerFrame=0;
			//}

			strfile.Format(_T("%s%s"), sFolder, fd.cFileName);
			if( !(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )	
			{
				image.Load(strfile);
				g_Temp.m_LoadSizeX=image.GetWidth();
				g_Temp.m_LoadSizeY=image.GetHeight();

				fm=(LPBYTE)image.GetBits();
				dst=g_Buffer;
				pitch=image.GetPitch();

				for (i=0; i < g_Temp.m_LoadSizeY; i++,fm+=pitch,dst+=g_System.m_nImageW)
					CopyMemory(dst, fm, g_Temp.m_LoadSizeX);
		
				image.Destroy();

#ifdef NO_GRABER_SYNC_FRAME_FROM_SERVER
				while(!g_Temp.m_bTestSyncRecieved)
				{
					Sleep(1);
				}
				g_Temp.m_bTestSyncRecieved = false;
#endif

				g_Temp.m_nGrabFrame++;		//Simulation
				g_Temp.m_nGrabFrameSub+=16;
				g_Temp.m_nGrabDone=1;		//Simulationb

#ifdef NO_GRABER_SYNC_FRAME_FROM_SERVER
					Sleep(10);
#else
					Sleep(nTime);
#endif
			}
			bRet= FindNextFile(hFind, &fd);
			if(!m_nSimulRun) break;
		}

		::FindClose(hFind);

		if(!m_nSimulRun) break;
	}
	
	return 0;
}

#endif