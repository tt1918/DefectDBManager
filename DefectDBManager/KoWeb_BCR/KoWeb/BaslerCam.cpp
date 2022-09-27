#include "stdafx.h"
#include "KoWebDoc.h"
#include "KoWebView.h"

#if GRABBER_TYPE==BASLER_GRABBER


CGrabber	g_Grabber;

#if BASLERVERSION==PYLON_2_2
	#pragma comment(lib, "./Basler22/PylonC_MD_vc80.lib")
	#pragma comment(lib, "./Basler22/PylonC_MDd_vc80.lib")
#elif BASLERVERSION==PYLON_3_0
	#pragma comment(lib, "./Basler30/PylonC_MD_vc100.lib")
#elif BASLERVERSION==PYLON_4_2
	#pragma comment(lib, "./Basler42/PylonC_MD_vc100.lib")
#elif BASLERVERSION==PYLON_5_0
	#ifdef USE_X64
		#pragma comment(lib, "./Basler50_64bit/PylonC.lib")
	#else
		#pragma comment(lib, "./Basler50/PylonC.lib")
	#endif
#elif BASLERVERSION==PYLON_5_1
	#ifdef USE_X64
		#pragma comment(lib, "./Basler51_64bit/PylonC.lib")
	#else
		#pragma comment(lib, "./Basler51/PylonC.lib")
	#endif
#endif


PYLON_DEVICE_HANDLE			hCam[MAX_CAM];					// Camera handle	
PYLON_STREAMBUFFER_HANDLE	l_hBuffer[MAX_CAM][MAX_BUFFER];	// Buffer handles (one for each)

PYLON_WAITOBJECT_HANDLE     l_hWait;						// Handle used for waiting for a grab to be finished.  
PYLON_STREAMGRABBER_HANDLE  l_hGrabber;						// Handle for the pylon stream grabber.  


LPBYTE ImageBuffer[MAX_CAM][MAX_BUFFER];
LPBYTE g_fmGrabImage;						// 가장 최근 촬상한 메모리
LPBYTE g_fmGrabBuffer[3];

int     g_GrabThreadRun, g_GrabThreadSubRun;
int     g_GrabStart=0;
int     g_GrabSubCount=0;

int    g_GrabBufferID=0;
int    g_PartialGrabDone=0;

int		g_FrameCnt=0;
double g_FrameStart=0, g_FrameEnd;

CGrabber::CGrabber(CWnd* pwnd)
{
	int i, j;
	m_pParent = pwnd;

}

CGrabber::~CGrabber()
{
//	g_Temp.m_GRAB1RUN=false;
//	g_Temp.m_GRAB1RUNSUB=false;
//	g_Temp.m_GRABControlRUN=false;
}


BOOL CGrabber::Open(bool bMemInit, HWND hWnd)
{
	int i,j;
	BOOL bRet=TRUE;
	long OffsetX;
	long OffsetY;
	long Width;
	long Height;
	long MaxWidth;
	long MaxHeight;
	long Num;
	long BufferSize;
	int  nGain400=0;
	BOOL bAvail;
	CString str;
	GENAPIC_RESULT     res;				// Return value of pylon methods.
	int32_t            payloadSize;		// Size of an image frame in bytes. 
	size_t             nStreams;		// The number of streams the device provides. 

	//데이타 초기화-----------------------------------------
	if(bMemInit)
	{
		m_nGrabX=g_System.m_nImageW;
		m_nGrabY=g_System.m_nImagePartH;

		m_nSizeX=m_nGrabX;
		m_nSizeY=g_System.m_nImageH;

		for(i=0;i<MAX_CAM;i++)
			for(j=0;j<MAX_BUFFER;j++)
				ImageBuffer[i][j]=new BYTE[m_nGrabX*m_nGrabY];

		for(i=0;i<3;i++)
			g_fmGrabBuffer[i]=new BYTE[m_nSizeX*m_nSizeY];
	
		g_fmGrabImage=new BYTE[m_nSizeX*m_nSizeY];
	}
	//------------------------------------------------------

	PylonInitialize ();

//	str=pDoc->m_data.m_strMachineName;
	///////////////////////////////
	// Open the camera
	///////////////////////////////
//	GetAreaCameraSerialNumber(pDoc->m_data.m_strComName);

	 res = PylonEnumerateDevices( &NumOfCams );
    if ( 0 == NumOfCams )
    {
        if(bMemInit) 
		{	
			AfxMessageBox(_T("연결된 카메라가 없습니다"));
			PylonTerminate(); 
			exit(EXIT_FAILURE);
		}
		else
		{
			PylonTerminate(); 
			return false;
		}
    }

	for(i=0;i<NumOfCams;i++)
	{
	/* Get a handle for the first device found.  */
		res = PylonCreateDeviceByIndex( 0, &hCam[i] );
    /* Before using the device, it must be opened. Open it for setting
       parameters and for grabbing images. */
		res = PylonDeviceOpen( hCam[i], PYLONC_ACCESS_MODE_CONTROL | PYLONC_ACCESS_MODE_STREAM );

		if(NULL == hCam[i]) continue;

//		res = PylonDeviceFeatureFromString (hCam[i], "DeviceID", StrBuffer);		
		PylonDeviceSetIntegerFeature( hCam[i], "Width", m_nGrabX );	
		PylonDeviceSetIntegerFeature( hCam[i], "Height", m_nGrabY );	

		PylonDeviceFeatureFromString( hCam[i], "AcquisitionMode", "Continuous" );

//		PylonDeviceSetIntegerFeature (hCam[i], "GainRaw", 210);



    /* ... Select the software trigger as the trigger source. */
		PylonDeviceFeatureFromString(hCam[i], "TriggerSelector", "AcquisitionStart");	


		if(g_Config.m_nModePeriod)  PylonDeviceFeatureFromString(hCam[i], "TriggerMode", "Off");					
		else						PylonDeviceFeatureFromString(hCam[i], "TriggerMode", "On");		

    /* Enable software triggering. */
    /* ... Select the software trigger as the trigger source. */
		res = PylonDeviceFeatureFromString( hCam[i], "TriggerSource", "Line1");			
		res = PylonDeviceFeatureFromString( hCam[i], "LineSelector", "Line1");			
		res = PylonDeviceSetFloatFeature( hCam[i], "LineDebouncerTimeAbs", 100 );		

//		res = PylonDeviceSetFloatFeature( hCam[i], "TriggerDelay", 1.0);

        res = PylonDeviceSetIntegerFeature( hCam[i], "GevSCPSPacketSize", 1500 );//9000	
	}
  
	for(i=0;i<NumOfCams;i++)
	{
		res = PylonDeviceGetNumStreamGrabberChannels( hCam[i], &nStreams );	
		if ( nStreams < 1 )
		{
//			TRACE("The transport layer doesn't support image streams.\n");
			PylonTerminate();
			exit(EXIT_FAILURE);
		}

    /* Create and open a stream grabber for the first channel. */ 
	    res = PylonDeviceGetStreamGrabber( hCam[i], 0, &l_hGrabber );			
	    res = PylonStreamGrabberOpen( l_hGrabber );							
    /* Get a handle for the stream grabber's wait object. The wait object
       allows waiting for buffers to be filled with grabbed data. */
	    res = PylonStreamGrabberGetWaitObject( l_hGrabber, &l_hWait );			
    /* Determine the required size of the grab buffer. Since activating chunks will increase the
       payload size and thus the required buffer size, do this after enabling the chunks. */
		res = PylonDeviceGetIntegerFeatureInt32( hCam[i], "PayloadSize", &payloadSize );		
    /* We must tell the stream grabber the number and size of the buffers 
       we are using. */
    /* .. We will not use more than NUM_BUFFERS for grabbing. */
		res = PylonStreamGrabberSetMaxNumBuffer( l_hGrabber, MAX_BUFFER );	
    /* .. We will not use buffers bigger than payloadSize bytes. */
		res = PylonStreamGrabberSetMaxBufferSize( l_hGrabber, payloadSize );	
    /*  Allocate the resources required for grabbing. After this, critical parameters 
        that impact the payload size must not be changed until FinishGrab() is called. */
		res = PylonStreamGrabberPrepareGrab( l_hGrabber );	

		for (j=0; j<MAX_BUFFER; j++)
		{
			PylonStreamGrabberRegisterBuffer (l_hGrabber, ImageBuffer[i][j], payloadSize,  &l_hBuffer[i][j] );	
			res = PylonStreamGrabberQueueBuffer( l_hGrabber, l_hBuffer[i][j], (void*) j );						
		}
	}

	return bRet;
}
void CGrabber::SetTriggerMode(int nCam, bool bPeriod)
{
	if(bPeriod)  PylonDeviceFeatureFromString(hCam[nCam], "TriggerMode", "Off");					
	else		 PylonDeviceFeatureFromString(hCam[nCam], "TriggerMode", "On");
}
void CGrabber::Close(bool bMemInit)
{
	int i,j;
	GENAPIC_RESULT     res;		// Return value of pylon methods.

	if(bMemInit)
	{
		g_GrabThreadRun=0;
		g_GrabThreadSubRun=0;
		Sleep(10);
	}

	for(i=0;i<NumOfCams;i++)
	{
		if(hCam[i]==NULL) continue;

		for (j=0; j<MAX_BUFFER; j++)
		{
			PylonStreamGrabberDeregisterBuffer( l_hGrabber, l_hBuffer[i][j] );	
		}

		PylonStreamGrabberCancelGrab( l_hGrabber );		
		res = PylonStreamGrabberFinishGrab( l_hGrabber );	
		res = PylonStreamGrabberClose( l_hGrabber );	    
		

	    res = PylonDeviceClose( hCam[i] );	 
		res = PylonDestroyDevice ( hCam[i] );

	}
	PylonTerminate ();

	if(bMemInit)
	{
		for(i=0;i<MAX_CAM;i++)
			for(j=0;j<MAX_BUFFER;j++)
				delete ImageBuffer[i][j];

		for(i=0;i<3;i++)
			delete g_fmGrabBuffer[i];

		delete g_fmGrabImage;
	}
}

void CGrabber::SetExposureTime(int nMicroSec, int nApply)
{
	static int nOldExposure=0;
	if(nOldExposure != nMicroSec)
	{
		g_Temp.m_nExposureData=nMicroSec;
		nOldExposure=nMicroSec;

		if(!g_Temp.m_bAuto && !g_Temp.m_bLive)
		{
			PylonDeviceSetFloatFeature( hCam[0], "ExposureTimeAbs", g_Temp.m_nExposureData );	
		}
		else
		{			
			g_Temp.m_nApplyExposure=1;			
		}
#ifdef USE_COS9_LR
		l_fmMuraCount=-1;
#endif
	}
}

void CGrabber::SetYOffset(int nOffsetY)
{
	static int nOldOffsetY = 0;
	if(nOldOffsetY!=nOffsetY) 
	{
		PylonDeviceSetIntegerFeatureInt32 (hCam[0], "OffsetY", nOffsetY);
		g_Param.m_nYOffset=nOffsetY;
	}
	nOldOffsetY=nOffsetY;
}

void CGrabber::HookStart()
{
	g_GrabSubCount		=0;
	g_GrabBufferID		=0;			//3개의 Buffer에순차적으로 이미지가 들어가는데 0부터 넣도록 설정
	g_PartialGrabDone   =false;		//Partial Grab성공
	g_GrabStart			=true;

	PylonDeviceExecuteCommandFeature( hCam[0], "AcquisitionStart");		
}

void CGrabber::HookStop()
{
	g_GrabStart			=false;
	Sleep(50);

	PylonDeviceExecuteCommandFeature( hCam[0], "AcquisitionStop");
}

//최근 촬상한 이미지
LPBYTE CGrabber::GrabImagePtr()
{
	return g_fmGrabImage;
}


UINT BaslerGrabThread(LPVOID pParam)
{
	CKoWebView *pView = (CKoWebView *)pParam;
		
	EPylonGrabStatus	GrabStatus;
	PylonGrabResult_t   grabResult;		// Stores the result of a grab operation. 
	_Bool            isReady;			// Used as an output parameter. 
	GENAPIC_RESULT     res;				// Return value of pylon methods.
	long BufferIx;
	int nCheckBuff = -999;
	int nID1 = 0;
	int i, j;
	int nWriteLog=0;
	LPBYTE fmDst, fmSrc;
	CString str;

	g_GrabThreadRun=true;
	while(g_GrabThreadRun)
    {
		if(g_GrabStart)
		{						
			PylonWaitObjectWait( l_hWait, INFINITE, &isReady );							 //INFINITE => 10000 바꿔서 사용가능
			res = PylonStreamGrabberRetrieveResult( l_hGrabber, &grabResult, &isReady );	
			BufferIx = (int) grabResult.Context;

			if(nCheckBuff ==BufferIx)
			{
				if(MAX_BUFFER>1)
				{
					AfxMessageBox(_T("Grab Error! 1"));
					break;
				}
			}
			else
			{
				if(g_GrabThreadRun)
				{
					if ( grabResult.Status == Grabbed )					
					{
						memcpy(g_fmGrabBuffer[g_GrabBufferID]+g_System.m_nImageW*g_System.m_nImagePartH*g_GrabSubCount, ImageBuffer[0][BufferIx],g_System.m_nImageW*g_System.m_nImagePartH); 

						g_GrabSubCount++;

						if(g_GrabSubCount>=g_System.m_nImagePart) 
						{
							//FPS 계산하는 부분---------------------------------------------------------------------------
							if(g_System.m_nImagePart==25)
							{
								g_FrameCnt++;
								if(g_FrameCnt==2)
								{
									g_FrameEnd=Clock_set();
									if(g_FrameStart!=0)
									{
										if(g_FrameEnd-g_FrameStart) g_Temp.m_dFPS=1000*2.0/(g_FrameEnd-g_FrameStart);
									}
									g_FrameStart=g_FrameEnd;
									g_FrameCnt=0;
								}
							}
							else
							{
								g_FrameCnt++;
								if(g_FrameCnt==20)
								{
									g_FrameEnd=Clock_set();
									if(g_FrameStart!=0)
									{
										if(g_FrameEnd-g_FrameStart) g_Temp.m_dFPS=1000*20.0/(g_FrameEnd-g_FrameStart);
									}
									g_FrameStart=g_FrameEnd;
									g_FrameCnt=0;
								}
							}
							//--------------------------------------------------------------------------------------------

							//한장 Grab이면 멈춤
							if(g_Temp.m_nGrabOne)
							{
								g_Temp.m_nGrabOne=0;
								PylonDeviceExecuteCommandFeature( hCam[0], "AcquisitionStop");
							}

							g_GrabSubCount=0;
							g_GrabBufferID=(g_GrabBufferID+1)%3;  
							//0,1,2,0,1,2,반복
							g_PartialGrabDone=true;					
						}
						nWriteLog=0;
					}
					else if ( grabResult.Status == Failed)							
					{
						if(nWriteLog==0)
						{
							WriteLog(_T("Basler Grab Failure"));
							nWriteLog=1;
						}

						nCheckBuff=-999;
						g_Grabber.Close(false);
						g_Grabber.Open(false, NULL);
						g_Grabber.HookStart();
						Sleep(1000);
						continue;
					}
					else 
					{
						AfxMessageBox(_T("Grab Error: Camera Reset!!!"));
						break;
					}

					PylonStreamGrabberQueueBuffer( l_hGrabber, grabResult.hBuffer, (void*) BufferIx );

					//노출변경: 아무때나 변경하지 않고, g_GrabSubCount 이 0이 될때.----------------------
					if(g_Temp.m_nApplyExposure && g_GrabSubCount==0)
					{
						PylonDeviceSetFloatFeature( hCam[0], "ExposureTimeAbs", g_Temp.m_nExposureData );	
						g_Temp.m_nApplyExposure=0;
#ifdef USE_COS9_LR
		l_fmMuraCount=-1;
#endif
					}
					//-----------------------------------------------------------------------------------

					nCheckBuff = BufferIx;
				}
			}
		}		
		Sleep(1);
	}


    return 0;
}

UINT BaslerGrabThreadSub(LPVOID pParam)
{
	CKoWebView *pView = (CKoWebView *)pParam;

	EPylonGrabStatus GrabStatus;
	long BufferIx;
	int nCheckBuff = -999;
	int i,j,nID = 0;
	int pitch;
	g_GrabThreadSubRun=true;

	while(g_GrabThreadSubRun)
    {
		//Grab이 끝날때 까지 대기---
		if(!g_PartialGrabDone)
		{
			Sleep(1);	//최대 ~2msec차이남   50M/Min 일때 1.6mm
			continue;
		}
		//--------------------------

		do
		{
			if(g_PartialGrabDone) //Grab되면 Frame 증가
			{
				g_PartialGrabDone=false;

				//방금 Grab완료된 이미지를 g_fmGrabImage에 복사한다.-----------------------------------
				nID=g_GrabBufferID-1;
				if(nID<0) nID=2;

				if(g_Config.m_nImageInverse)
				{
					pitch=g_System.m_nPitch;
					for(i=0;i<g_System.m_nImageH;i++)
						for(j=0;j<g_System.m_nImageW;j++)
							*(g_fmGrabImage+pitch*i+j)=255-*(g_fmGrabBuffer[nID]+pitch*i+j);
				}
				else
				{
					memcpy(g_fmGrabImage, g_fmGrabBuffer[nID], g_System.m_nImageW*g_System.m_nImageH);
				}
				//-------------------------------------------------------------------------------------

				g_Temp.m_nGrabFrame++;			//Basler Grab
				if(g_Temp.m_nONInspecting==0)	//아직 검사중이면 이번에 Grab한 이미지 버림.
					g_Temp.m_nGrabDone=1;	
			}
			else
			{
				Sleep(1);
			}

			if(g_GrabThreadSubRun==0) return 0;

		}while(g_Temp.m_nONInspecting); //검사가 끝나길 대기함.

		Sleep(1);
	}

    return 0;
}

#endif