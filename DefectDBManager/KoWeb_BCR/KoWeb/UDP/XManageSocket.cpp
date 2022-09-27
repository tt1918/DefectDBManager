// XManageSocket.cpp: implementation of the CXManageSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XManageSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXManageSocket::CXManageSocket()
{

}

CXManageSocket::~CXManageSocket()
{

}


MARKINGDATA l_MK[10];
void CXManageSocket::OnDataReceived(LPBYTE lpBuffer, DWORD dwCount)
{

//-----------------------------------------------------------------------------------------------------
#ifdef MY_RECV
	if(g_Kipo.m_nKipoPress!=0 ) //들어오는 순간 바로 처리함. (기포 눌림 또는 Hole 주기 있을때) 
	{
		int i, nCode, nCOSRecvCount, nCOSRecvX[10], nCOSRecvY[10];
		memcpy(&nCode, (char*)lpBuffer+4, 4);

		if(nCode==NM_KIPOPRESS)	//91
		{
			memcpy(&nCOSRecvCount, (char*)lpBuffer+8,  4);			//SEND PC에서 보내준 갯수 
			if(nCOSRecvCount>10) nCOSRecvCount=10;

			for(i=0;i<nCOSRecvCount;i++)
			{
				memcpy(&nCOSRecvX[i], (char*)lpBuffer+12+i*8, 4);  //X좌표 (mm)
				memcpy(&nCOSRecvY[i], (char*)lpBuffer+16+i*8, 4);  //Y좌표 (mm)
			}

			KO_SetKipoPress(g_Temp.m_nGrabFrameSub, nCOSRecvCount, nCOSRecvX, nCOSRecvY);
		}
	}
#endif

#ifdef MARKING_VISION
	//마킹서버에서 클라이언트로 보낸경우 
	if(g_Temp.m_nPCOptic==CMAK)
	{
		if(m_iPort==MARKING_PORT_RED || m_iPort==MARKING_PORT_BLUE)
		{
			int i, nCode, nX, nY, nCOSRecvCount, nIndexBK;
			memcpy(&nCode, (char*)lpBuffer+4, 4);		//CODE  51

			if(nCode==COS_MARKING_DATA)	//51
			{
				memcpy(&nCOSRecvCount, (char*)lpBuffer+16, 4);  //마킹서버에서 보내은 마킹 수  최대 10개만 사용(마킹서버에서도 한번 보낼때 10개까지만 보내면 됨)
				if(nCOSRecvCount>10) nCOSRecvCount=10;

				memcpy(l_MK, (char*)lpBuffer+16, nCOSRecvCount*sizeof(MARKINGDATA));

				for(i=0;i<nCOSRecvCount;i++) 
				{
					nIndexBK=g_MakringVision.m_nMKIndex;

					g_MakringVision.m_nMKIndex++; if(g_MakringVision.m_nMKIndex>=MAX_INDEX) g_MakringVision.m_nMKIndex=1;
					
					//데이타가 MAX_INDEX 개 꽉 찾으면 더이상 받지 않는다.---------------
					if(g_MakringVision.m_MKData[g_MakringVision.m_nMKIndex].m_nPC)
					{
						g_MakringVision.m_nMKIndex=nIndexBK;
						return;
					}
					//------------------------------------------------------------------

					memcpy(&g_MakringVision.m_MKData[g_MakringVision.m_nMKIndex], &l_MK[i], sizeof(MARKINGDATA));

					nX=l_MK[i].m_nMarkingPort; //마킹한 포트 번호
					if(m_iPort==MARKING_PORT_RED) nY=g_Temp.m_nGrabFrameSub-g_MakringVision.m_nFrameOffset;    //nFrameSub현재 SubGrab Frame수
					else						  nY=g_Temp.m_nGrabFrameSub-g_MakringVision.m_nFrameOffset1;
		
					if(nX<0 || nX>=MAX_MKDATA_PORT || nY<0 || nY>=MAX_MKDATA_STORE) continue;

					g_MakringVision.m_nMarkingData[nY][nX]=g_MakringVision.m_nMKIndex%MAX_INDEX;  //마킹비전 내부 맵에 Index저장 (1~999)		
				}
			
				return;
			}
		}
	}
#endif


//-----------------------------------------------------------------------------------------------------
	SendMessage((HWND)m_pParent,WM_XDATARRECIEVED,(DWORD)m_iPort-DEFAULT_PORT,(LPARAM)lpBuffer);

}


void CXManageSocket::SendInsData(CPacket* packet)
{
	WriteComm((LPBYTE)packet->m_pBuf, packet->m_nBuflen,DEFAULT_TIMEOUT);
}

void CXManageSocket::SendInsData_Buffer(LPBYTE pBuffer, int nBufLength)
{
	WriteComm(pBuffer, nBufLength,DEFAULT_TIMEOUT);
}

void CXManageSocket::SendCommand_LocalHost(int nCommand)
{
	CPacket packet;
	packet.MakeInstantPacket(nCommand, PARAM_DUMMY, -1);	

	WriteComm((LPBYTE)packet.m_pBuf, packet.m_nBuflen,DEFAULT_TIMEOUT);
}

void CXManageSocket::SendInstantData_LocalHost(LPCSTR param_name, double value)
{
	CPacket packet;
//	packet.MakeInstantPacket(NM_PARAM_INSTANT, param_name, value);
	packet.MakeInstantPacket(NM_ACK_CAM_AVERAGE, param_name, value);

	WriteComm((LPBYTE)packet.m_pBuf, packet.m_nBuflen,DEFAULT_TIMEOUT);
}




void CXManageSocket::SendInstantData_LocalHost(LPCSTR param_name, LPSTR str_data)
{
	CPacket packet;

	int len = strlen(str_data);
	packet.MakeInstantPacket(NM_PARAM_INSTANT, param_name, str_data, len);

	WriteComm((LPBYTE)packet.m_pBuf, packet.m_nBuflen,DEFAULT_TIMEOUT);
}

void CXManageSocket::SendInstantData_LocalHostBCR(LPCSTR param_name, double value)
{
	CPacket packet;
	packet.MakeInstantPacket(MN_DATA_NEB, param_name, value);

	WriteComm((LPBYTE)packet.m_pBuf, packet.m_nBuflen,DEFAULT_TIMEOUT);
}

void CXManageSocket::SendInstantData_LocalHostLight(LPCSTR param_name, double value)
{
	CPacket packet;
//	packet.MakeInstantPacket(NM_PARAM_INSTANT, param_name, value);
	packet.MakeInstantPacket(NM_ACK_LIGHT_DATA, param_name, value);

	WriteComm((LPBYTE)packet.m_pBuf, packet.m_nBuflen,DEFAULT_TIMEOUT);
}

void CXManageSocket::SendtoServer_Data(TCHAR *param_name, double value)
{
	CPacket packet;
//	packet.MakeInstantPacket(NM_PARAM_INSTANT, param_name, value);
	packet.MakeInfoPacket(NM_ACK_CAM_AVERAGE, param_name, value);

	WriteComm((LPBYTE)packet.m_pBuf, packet.m_nBuflen,DEFAULT_TIMEOUT);
}