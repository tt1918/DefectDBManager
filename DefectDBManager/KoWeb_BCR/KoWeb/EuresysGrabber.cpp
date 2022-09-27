#include "StdAfx.h"
#include "EuresysGrabber.h"

#if GRABBER_TYPE==EURESYS_GRABBER
CGrabber	g_Grabber;

/********************************************************************************/
// Begin CGrabber class

CGrabber::CGrabber(CWnd* pwnd)
: m_pCoaxlink(nullptr)
, m_width(0)
, m_height(0)
, m_nImageInverse(0)
, m_nGrabFrameSub(0)
{
}

CGrabber::~CGrabber()
{
	Close(true);
}

bool CGrabber::Open(bool flag, HWND hWnd)
{
	if(!m_pCoaxlink)
	{
		m_pCoaxlink = new CCoaxlink(this);

		int nGrabberID	= 0;		//grabber 여러개일 경우 선택
		int nCameraID	= 0;		//camera 여러개일 경우 선택

		if(m_pCoaxlink->Open(nGrabberID, nCameraID))
		{
			m_pCoaxlink->SetConfigure(MODE_CONFIG_FILE);
			TRACE("Open Success\n");

			//전역으로 설정된 영상버퍼 크기를 받아온다
			m_width = g_System.m_nImageW;
			m_height = g_System.m_nImageH;

			//전역으로 설정된 값을 받아온다
			m_nImageInverse = g_Config.m_nImageInverse;
			m_nGrabFrameSub = g_System.m_nImagePart;

			//출력버퍼 생성
			for(int i=0;i<MAX_BUFFER;i++)
				m_buffer[i] = new BYTE[m_width*m_height];
		}
		else
		{
			delete m_pCoaxlink;
			m_pCoaxlink = nullptr;
		}
	}

	return false;
}

void CGrabber::Close(bool flag)
{
	if(m_pCoaxlink)
	{
		if(m_pCoaxlink->IsOpen())
		{
			m_pCoaxlink->Close();
			delete m_pCoaxlink;

			m_pCoaxlink = nullptr;
		}
	}
	for(int i=0;i<MAX_BUFFER;i++)
	{
		if(m_buffer[i])
		{
			delete m_buffer[i];
			m_buffer[i] = nullptr;
		}
	}
}

void CGrabber::HookStart()
{
	if(m_pCoaxlink)
	{
		if(m_pCoaxlink->IsOpen() && !m_pCoaxlink->IsGrabbing())
		{
			m_pCoaxlink->Grab(GENTL_INFINITE);
		}
	}
}

void CGrabber::HookStop()
{
	if(m_pCoaxlink)
	{
		if(m_pCoaxlink->IsOpen() && m_pCoaxlink->IsGrabbing())
		{
			m_pCoaxlink->Stop();
		}
	}
}

void CGrabber::SetExposureTime(int nUserExpo, int nApply)
{
	if(m_pCoaxlink)
	{
		if(m_pCoaxlink->IsOpen())
		{
			m_pCoaxlink->SetExposureTime(nUserExpo);
		}
	}
}

void CGrabber::SetYOffset(int nOffset)
{
}

LPBYTE CGrabber::GrabImagePtr()
{
	if(m_pCoaxlink)
	{
		if(m_pCoaxlink->IsOpen())
		{
			return m_buffer[m_pCoaxlink->GetGrabIndex()];
		}
	}

	return nullptr;
}

void CGrabber::SetTrigger(int flag)
{
}

void CGrabber::SetTriggerMode(int nCam, bool bPeriod)
{
}
// End CGrabber class
/********************************************************************************/

/********************************************************************************/
// Begin CCoaxlink class

CCoaxlink::CCoaxlink(void* parent)
: m_pParent(parent)
, m_nBuffer_cnt(MAX_BOARD_BUFFER)
, m_bOpen(false)
, m_bGrabbing(false)
, m_pCoaxlinkGrabber(nullptr)
, m_nContext(0)
, m_Pitch(0)
, m_Width(0)
, m_Height(0)
, m_Grabmode(MODE_CONTINUOUS)
, m_NewExposure(0)
, m_Exposure(0)
, m_nGrabIndex(0)
, m_nSubGrabIndex(0)
, m_nSubCopyIndex(0)
{
}

CCoaxlink::~CCoaxlink(void)
{
	Close();
}

/** 시스템에 설치된 카드 및 카메라 정보를 읽어온다
    @return 시스템에 설치된 카드 개수
**/
int CCoaxlink::ListCards() 
{
	try
	{
		GenTL::TL_HANDLE tl = m_GenTL.tlOpen();
		auto numCards = m_GenTL.tlGetNumInterfaces(tl);
		for (auto i = 0; i < numCards; i++) 
		{
			std::string id = m_GenTL.tlGetInterfaceID(tl, i);
			TRACE(_T("[%s]\n"),id.c_str());

			gc::IF_HANDLE ifh = m_GenTL.tlOpenInterface(tl, id);
			auto numDevices = m_GenTL.ifGetNumDevices(ifh);
			for(auto j = 0; j < numDevices; j++)
			{
				TRACE(_T("[%s]\n"),m_GenTL.ifGetDeviceID(ifh, j).c_str());
			}
			m_GenTL.ifClose(ifh);
		}
		m_GenTL.tlClose(tl);
		return numCards;
	}
	catch(const std::exception &e)
	{
		TRACE("%s\n", e.what());
		return -1;
	}
}

bool CCoaxlink::Open(int nGrabberID, int nCameraID)
{
	bool res = false;

	if(!m_pCoaxlinkGrabber)
	{
		try
		{
			m_pCoaxlinkGrabber = new CCoaxlinkGrabber(m_GenTL, nGrabberID, nCameraID, 0, this);

			if(!ListCards())
			{
				TRACE("No grabber card\n");
			}

			m_bOpen = true;

			res = true;
		}
		catch (const std::exception &e)
		{
			m_GenTL.memento(e.what());
			TRACE("%s\n", e.what());
			m_bOpen = false;
			res = false;
		}
	}

	return res;
}

void CCoaxlink::Close(void)
{
	if(m_bGrabbing)
		Stop();

	if(m_pCoaxlinkGrabber != nullptr)
	{
		delete m_pCoaxlinkGrabber;
		m_pCoaxlinkGrabber = nullptr;
	}

	m_bOpen = false;
}

void CCoaxlink::Grab(uint64_t grab_cnt/* = GENTL_INFINITE*/)
{
	if(!m_bGrabbing)
	{
		m_Width			= m_pCoaxlinkGrabber->getInteger<RemoteModule>("Width");
		m_Height		= m_pCoaxlinkGrabber->getInteger<StreamModule>("ScanLength");		//LineScan
		//m_Height		= m_pCoaxlinkGrabber->getInteger<RemoteModule>("Height");			//AreaScan	
		m_Pitch			= m_pCoaxlinkGrabber->getInteger<StreamModule>("LinePitch");

		m_nGrabIndex=0;
		m_nSubGrabIndex=0;
		m_nSubCopyIndex=0;

		//SetAllEvent();
		// ** Auto Buffer Allocation
		m_pCoaxlinkGrabber->reallocBuffers(m_nBuffer_cnt, m_Width*m_Height);

		m_pCoaxlinkGrabber->start(grab_cnt);
		m_bGrabbing = true;
		m_pCoaxlinkGrabber->memento("Grab started");
	}
}

void CCoaxlink::Stop(void)
{
	try
	{
		m_pCoaxlinkGrabber->stop();
	}
	catch (std::exception& )
	{
		m_pCoaxlinkGrabber->memento("m_Coaxlink.Stop() - Timeout");
	}

	m_bGrabbing = false;
}

/** Grab방식을 세팅
    @param mode Grab방식
**/
void CCoaxlink::SetConfigure(Grabmode_t mode)
{
#ifdef _UNICODE
	std::wstring path(EURESYS_FILE);
#else
	std::string path(EURESYS_FILE);
#endif

	switch (mode)
	{
	case MODE_CONTINUOUS:
		// Camera
		setString<RemoteModule>("TriggerMode", "Off"); // On, Off
		setString<RemoteModule>("AcquisitionMode", "Continuous"); // Continuous, SingleFrame, MultiFrame
		setString<DeviceModule>("CameraControlMethod", "NC");
		break;

	case MODE_HARDTRIGGER_FG:
		setString<RemoteModule>("TriggerMode", "On"); // On, Off
		setString<RemoteModule>("TriggerSource", "CXPin"); //LineIn0, CXPin
		setString<DeviceModule>("CameraControlMethod", "RC"); // NC, RC, RG
		setInteger<DeviceModule>("ExposureReadoutOverlap", true);
		setFloat<DeviceModule>("ExposureRecoveryTime", 0);
		setFloat<DeviceModule>("StrobeDuration", 10.0);
		setString<DeviceModule>("CycleTriggerSource", "MDV1"); // Immediate, StartCycle, LIN1, LIN2, LIN3, LIN4, MDV1
		setFloat<DeviceModule>("CycleMinimumPeriod", 10.0);
		setString<DeviceModule>("CxpTriggerMessageFormat", "Pulse");
		setFloat<DeviceModule>("CxpTriggerAckTimeout", 20.0);
		setString<InterfaceModule>("LineInputToolSelector", "LIN1"); 
		setString<InterfaceModule>("LineInputToolSource", "IIN11"); 
		setString<InterfaceModule>("LineInputToolActivation", "RisingEdge");
		setString<InterfaceModule>("MultiplierDividerToolSource", "LIN1");
		setString<InterfaceModule>("MultiplierDividerToolOutputControl", "Enable");	//Enable, Disable
		setFloat<InterfaceModule>("MultiplierDividerToolMultiplicationFactor", 1.0);
		setFloat<InterfaceModule>("MultiplierDividerToolDivisionFactor", 1.0);
		break;

	case MODE_HARDTRIGGER_CAM:
		setString<RemoteModule>("TriggerMode", "On"); // On, Off
		setString<RemoteModule>("TriggerSource", "CXPin"); //LineIn0, CXPin
		setString<RemoteModule>("TriggerRescalerMode", "On");	//On, Off
		setFloat<RemoteModule>("TriggerRescalerRate", 9.0);
		setString<DeviceModule>("CameraControlMethod", "RG"); // NC, RC, RG
		setInteger<DeviceModule>("ExposureReadoutOverlap", true);
		setFloat<DeviceModule>("ExposureRecoveryTime", 10.0);
		setFloat<DeviceModule>("StrobeDuration", 10.0);
		setString<DeviceModule>("CycleTriggerSource", "LIN1"); // Immediate, StartCycle, LIN1, LIN2, LIN3, LIN4, MDV1
		setFloat<DeviceModule>("CycleTargetPeriod", 10.0);
		setString<InterfaceModule>("LineInputToolSelector", "LIN1"); 
		setString<InterfaceModule>("LineInputToolSource", "IIN11"); 
		setString<InterfaceModule>("LineInputToolActivation", "RisingEdge");
		break;

	case MODE_CONFIG_FILE:
		try
		{
			m_pCoaxlinkGrabber->runScript(std::string(path.begin(), path.end()));
		}
		catch (std::exception&)
		{
			m_pCoaxlinkGrabber->memento("m_Coaxlink.runScript() - Exception");
		}
		break;

	default:
		break;
	}

	m_Grabmode = mode;

}

void CCoaxlink::SetAllEvent()
{
	setString<DeviceModule>("EventSelector", "CameraTriggerRisingEdge");
	setInteger<DeviceModule>("EventNotification", true);

	setString<DeviceModule>("EventSelector", "CameraTriggerFallingEdge");
	setInteger<DeviceModule>("EventNotification", true);

	setString<DeviceModule>("EventSelector", "StrobeRisingEdge");
	setInteger<DeviceModule>("EventNotification", true);

	setString<DeviceModule>("EventSelector", "StrobeFallingEdge");
	setInteger<DeviceModule>("EventNotification", true);

	setString<DeviceModule>("EventSelector", "AllowNextCycle");
	setInteger<DeviceModule>("EventNotification", false);

	setString<DeviceModule>("EventSelector", "DiscardedCicTrigger");
	setInteger<DeviceModule>("EventNotification", true);

	setString<DeviceModule>("EventSelector", "PendingCicTrigger");
	setInteger<DeviceModule>("EventNotification", false);

	setString<DeviceModule>("EventSelector", "CxpTriggerAck");
	setInteger<DeviceModule>("EventNotification", false);

	setString<DeviceModule>("EventSelector", "CxpTriggerResend");
	setInteger<DeviceModule>("EventNotification", false);

	setString<DeviceModule>("EventSelector", "Trigger");
	setInteger<DeviceModule>("EventNotification", false);

	//enableEvent<CicData>();

 	setString<StreamModule>("EventSelector", "StartOfCameraReadout");
 	setInteger<StreamModule>("EventNotification", true);
 
 	setString<StreamModule>("EventSelector", "EndOfCameraReadout");
 	setInteger<StreamModule>("EventNotification", true);
 
 	setString<StreamModule>("EventSelector", "StartOfScan");
 	setInteger<StreamModule>("EventNotification", true);
 
 	setString<StreamModule>("EventSelector", "EndOfScan");
 	setInteger<StreamModule>("EventNotification", true);
	
	setString<StreamModule>("EventSelector", "RejectedFrame");
	setInteger<StreamModule>("EventNotification", true);

	setString<StreamModule>("EventSelector", "RejectedScan");
	setInteger<StreamModule>("EventNotification", true);
	enableEvent<DataStreamData>();

	setString<InterfaceModule>("EventSelector", "LIN1");
	setInteger<InterfaceModule>("EventNotification", true);
}

void CCoaxlink::SetExposureTime(int nUserExpo)
{
	if(m_bGrabbing)
	{
		m_NewExposure = nUserExpo;
	}
	else
	{
		if(m_Grabmode==MODE_HARDTRIGGER_FG)
			setInteger<RemoteModule>("ExposureReadoutOverlap", true);
		else if(m_Grabmode==MODE_HARDTRIGGER_CAM)
			setInteger<RemoteModule>("ExposureReadoutOverlap", true);
		else if(m_Grabmode==MODE_CONFIG_FILE)
		{

		}
		m_Exposure = m_NewExposure = nUserExpo;
	}
}

void CCoaxlink::OnImageEvent(unsigned char* pBuffer)
{
	int i, j, N, M;
	LPBYTE fmT, fmS;

	 CGrabber* parent = static_cast<CGrabber*>(m_pParent);

	 //제일 먼저 Exposure를 바꾼다.(큰 한 Frame 끝나면 Exposure 바꾼다)
	if(m_Exposure != m_NewExposure && (parent->m_nGrabFrameSub<=1  ||  (m_nSubCopyIndex+1)==parent->m_nGrabFrameSub))
	{
		setInteger<RemoteModule>("Exposure", m_NewExposure);
		m_Exposure = m_NewExposure;
	}

	if(parent->m_nGrabFrameSub<=1)
	{
		M=N=m_nGrabIndex; //m_nGrabIndex: 현재 1회 Grab 마다 증가되는 값
		if(m_Width==m_Pitch && parent->m_width==m_Width && parent->m_height==m_Height)
		{
			if(parent->m_nImageInverse)
			{
				for(i=0;i<m_Height;i++)
					for(j=0;j<m_Width;j++)
						*(parent->m_buffer[M]+m_Width*i+j)=255-*(pBuffer+m_Width*i+j);
			}
			else
			{
				memcpy(parent->m_buffer[M], pBuffer, m_Width*m_Height);
			}
		}
		else
		{
			int width = min(m_Width, parent->m_width);
			int height = min(m_Height, parent->m_height);
			if(parent->m_nImageInverse)
			{
				for(i=0;i<height;i++)
				{
					fmT=parent->m_buffer[M]+parent->m_width*i;
					fmS=pBuffer+m_Pitch*i;
					for(j=0;j<width;j++)
						*(fmT+j)=255-*(fmS+j);
				}
			}
			else
			{
				for(i=0;i<height;i++)
					memcpy(parent->m_buffer[M]+parent->m_width*i, pBuffer+m_Pitch*i, width);
			}
		}
	
		if(m_nGrabIndex+1>=MAX_BUFFER) m_nGrabIndex=0;
		else						   m_nGrabIndex++;

		g_Temp.m_nGrabFrame++;		//Subframe나눈게 다 찰때 Grab
		g_Temp.m_nGrabDone=1;	
	}
	else
	{
		N=m_nSubGrabIndex; //m_nSubGrabIndex: 현재 1회 Grab 마다 증가되는 값
		M=m_nGrabIndex;	   //m_nGrabIndex: 한 Frame 마다 증가되는 값
		if(m_Width==m_Pitch && parent->m_width==m_Width && parent->m_height==m_Height*parent->m_nGrabFrameSub)
		{
			if(parent->m_nImageInverse)
			{
				fmT=parent->m_buffer[M]+m_nSubCopyIndex*(m_Width*m_Height);
				for(i=0;i<m_Height;i++)
					for(j=0;j<m_Width;j++)
						*(fmT+m_Width*i+j)=255-*(pBuffer+m_Width*i+j);
			}
			else
			{
				memcpy(parent->m_buffer[M]+m_nSubCopyIndex*(m_Width*m_Height), pBuffer, m_Width*m_Height);		
			}
		}
		else
		{
			int width = min(m_Width, parent->m_width);
			int heightSub = min(m_Height, parent->m_height/parent->m_nGrabFrameSub);
			if(parent->m_nImageInverse)
			{
				for(i=0;i<heightSub;i++)
				{
					fmT=parent->m_buffer[M]+parent->m_width*(m_nSubCopyIndex*heightSub+i);
					fmS=pBuffer+m_Pitch*i;
					for(j=0;j<width;j++)
						*(fmT+j)=255-*(fmS+j);
				}
			}
			else
			{
				for(i=0;i<heightSub;i++)
					CopyMemory(parent->m_buffer[M]+parent->m_width*(m_nSubCopyIndex*heightSub+i), pBuffer+m_Pitch*i, width);
			}
		}
		
		if(m_nSubGrabIndex+1>=MAX_BUFFER) m_nSubGrabIndex=0;
		else							  m_nSubGrabIndex++;

		m_nSubCopyIndex++;
		if(m_nSubCopyIndex>=parent->m_nGrabFrameSub)
		{
			m_nSubCopyIndex=0;			
			if(m_nGrabIndex+1>=MAX_BUFFER) m_nGrabIndex=0;
			else						   m_nGrabIndex++;

			g_Temp.m_nGrabFrame++;		//Matrox Grab
			g_Temp.m_nGrabDone=1;	
		}
	}
}

int CCoaxlink::GetGrabIndex()
{
	int N=m_nGrabIndex-1;
	if(N<0) N=MAX_BUFFER-1;
	return N;
}

//End CCoaxlink class
/********************************************************************************/

/********************************************************************************/
// Begin CCoaxlinkGrabber class
CCoaxlinkGrabber::CCoaxlinkGrabber(EGenTL &GenTL, int interfaceIndex, int deviceIndex, int dataStreamIndex, void* pParent)
:EGrabber<CallbackMultiThread>(GenTL, interfaceIndex, deviceIndex, dataStreamIndex)
,m_pParent(pParent)
{
	m_poImgPtr = NULL;
}


CCoaxlinkGrabber::~CCoaxlinkGrabber(void)
{
}

void CCoaxlinkGrabber::onCicEvent(const CicData& data)
{
	switch (data.numid)
	{
	case ge::EVENT_DATA_NUMID_CIC_CAMERA_TRIGGER_RISING_EDGE:
		TRACE("TRIGGER_RISING_EDGE\n");
		break;
	case ge::EVENT_DATA_NUMID_CIC_CAMERA_TRIGGER_FALLING_EDGE:
		TRACE("TRIGGER_FALLING_EDGE\n");
		break;
	case ge::EVENT_DATA_NUMID_CIC_STROBE_RISING_EDGE:
		TRACE("STROBE_RISING_EDGE\n");
		break;
	case ge::EVENT_DATA_NUMID_CIC_STROBE_FALLING_EDGE:
		TRACE("STROBE_FALLING_EDGE\n");
		break;
	case ge::EVENT_DATA_NUMID_CIC_ALLOW_NEXT_CYCLE:
		TRACE("ALLOW_NEXT_CYCLE\n");
		break;
	case ge::EVENT_DATA_NUMID_CIC_DISCARDED_CIC_TRIGGER:
		TRACE("DISCARDED_CIC_TRIGGER\n");
		break;
	case ge::EVENT_DATA_NUMID_CIC_PENDING_CIC_TRIGGER:
		TRACE("PENDING_CIC_TRIGGER\n");
		break;
	case ge::EVENT_DATA_NUMID_CIC_CXP_TRIGGER_ACK:
		TRACE("CXP_TRIGGER_ACK\n");
		break;
	case ge::EVENT_DATA_NUMID_CIC_CXP_TRIGGER_RESEND:
		TRACE("CXP_TRIGGER_RESEND\n");
		break;
	case ge::EVENT_DATA_NUMID_CIC_TRIGGER:
		TRACE("TRIGGER\n");
		break;
	default:
		break;
	}  
}

void CCoaxlinkGrabber::onIoToolboxEvent(const IoToolboxData &data)
{
	TRACE("onIoToolboxEvent\n");
}

void CCoaxlinkGrabber::onDataStreamEvent(const DataStreamData &data)
{
	switch (data.numid)
	{
	case ge::EVENT_DATA_NUMID_DATASTREAM_START_OF_CAMERA_READOUT:
		TRACE("START_OF_CAMERA_READOUT\n");
		break;
	case ge::EVENT_DATA_NUMID_DATASTREAM_END_OF_CAMERA_READOUT:
		TRACE("END_OF_CAMERA_READOUT\n");
		break;
	case ge::EVENT_DATA_NUMID_DATASTREAM_START_OF_SCAN:
		TRACE("START_OF_SCAN\n");
		break;
	case ge::EVENT_DATA_NUMID_DATASTREAM_END_OF_SCAN:
		TRACE("END_OF_SCAN\n");
		break;
	case ge::EVENT_DATA_NUMID_DATASTREAM_REJECTED_FRAME:
		TRACE("REJECTED_FRAME\n");
		break;
	case ge::EVENT_DATA_NUMID_DATASTREAM_REJECTED_SCAN:
		TRACE("REJECTED_SCAN\n");
		break;
	default:
		break;
	}  
}

void CCoaxlinkGrabber::onCxpInterfaceEvent(const CxpInterfaceData &data)
{
	switch (data.numid)
	{
	case ge::EVENT_DATA_NUMID_IO_TOOLBOX_LIN1:
		TRACE("IO_TOOLBOX_LIN1\n");
		break;
	case ge::EVENT_DATA_NUMID_IO_TOOLBOX_LIN2:
		TRACE("IO_TOOLBOX_LIN2\n");
		break;
	case ge::EVENT_DATA_NUMID_IO_TOOLBOX_LIN3:
		TRACE("IO_TOOLBOX_LIN3\n");
		break;
	case ge::EVENT_DATA_NUMID_IO_TOOLBOX_LIN4:
		TRACE("IO_TOOLBOX_LIN4\n");
		break;
	default:
		break;
	}  
}


void CCoaxlinkGrabber::onNewBufferEvent(const NewBufferData& data)
{
	memento("Entering Callback");

	ScopedBuffer buffer(*this, data);    
	m_poImgPtr = buffer.getInfo<unsigned char *>(gc::BUFFER_INFO_BASE);

	CCoaxlink* pCoaxlinkLinescan = (CCoaxlink*)m_pParent;
	pCoaxlinkLinescan->OnImageEvent(m_poImgPtr);
}

//End CCoaxlinkGrabber class
/********************************************************************************/

#endif