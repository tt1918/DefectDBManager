// ClientSock.cpp : implementation file
//

#include "stdafx.h"
#include "AbyssClient.h"
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AbyssClient

CConectSocket* AbyssClient::CreateSocket()
{
	CConectSocket* pSocket = new CConectSocket(this);

	pSocket->Create();

	//int nBufLen = 4096;
	//pSocket->SetSockOpt(SO_SNDBUF, &nBufLen, 4);
	//pSocket->SetSockOpt(SO_RCVBUF, &nBufLen, 4);

	m_imageWidth = 0;
	m_imageHeight = 0;
	m_command = COMMAND_NONE;

	m_predictID = 1;
	m_bAutoReconnect = true;
	m_bRemakeSocket = false;

	return pSocket;
}

AbyssClient::AbyssClient()
	: m_nStatus(0)
{
	// http://egloos.zum.com/Charko/v/1405154
		//Socket init
	if (AfxSocketInit(NULL) == 0)			// 이걸 해줘야 소켓이 된다 !!!! ㅜㅜ
	{
		// init fail
		m_bAfxSocketInit = false;
		return;
	}

	m_bAfxSocketInit = true;

	CWnd *pWnd = AfxGetMainWnd();
	m_hMainWnd = pWnd->m_hWnd;

	m_pSocket = CreateSocket();

}

AbyssClient::~AbyssClient()
{
	SAFE_DELETE(m_pSocket);
}


/////////////////////////////////////////////////////////////////////////////
// AbyssClient member functions



int AbyssClient::ConnectServerStart(LPCTSTR lpszHostAddress, UINT nHostPort)
{
	m_command = COMMAND_CONNECT_SERVER;
	return m_pSocket->Connect(lpszHostAddress, nHostPort);
}


int AbyssClient::ConnectServer(LPCTSTR lpszHostAddress, UINT nHostPort)
{
	if (m_command != COMMAND_NONE) return 1;			// connect fail

	m_lpszHostAddress = lpszHostAddress;
	m_nHostPort = nHostPort;

	return ConnectServerStart(m_lpszHostAddress, m_nHostPort);
}

int AbyssClient::RetryConnect()
{
	//OutputDebugString(_T("RetryConnect\n"));

	if (m_bRemakeSocket)
	{
		SAFE_DELETE(m_pSocket);
		m_pSocket = CreateSocket();
		m_bRemakeSocket = false;
	}

	return ConnectServerStart(m_lpszHostAddress, m_nHostPort);
}

int AbyssClient::Predict(byte *pBuf, int width, int height)
{
	int ret;
	if (m_bConnect == false)
	{
		if (m_bAutoReconnect)
		{
			ret=RetryConnect();
		}
		
		return ABYSS_CLIENT_ERR_NOT_CONNECTED;
	}
		

	m_pBuf = pBuf;


	m_predictID++;
	if(m_predictID <= 0 || m_predictID > 1000) m_predictID = 1;   

	if (width != m_imageWidth || height != m_imageHeight)
	{
		// 이미지 크기가 달라져서 먼저 서버에 SendSetImageSize()먼저 보낸다..
		ret = SendCheckImage(width, height, m_pBuf);
	}
	else
	{
		ret = SendCheckImage(m_pBuf);
	}
	
	return ret;		// 1이상이면 predictID, -1는 에러코드
}



double* AbyssClient::GetPredictResult()
{
	return m_pConfidence;
}

void AbyssClient::GetBytes(byte *pBuf, int n)
{
	pBuf[0] = (unsigned char)n & 0xFF;
	pBuf[1] = (unsigned char)((n >> 8) & 0xFF);
	pBuf[2] = (unsigned char)((n >> 16) & 0xFF);
	pBuf[3] = (unsigned char)((n >> 24) & 0xFF);
}

int AbyssClient::SendCheckImage(int width, int height, byte *pBuf)
{
	byte buf[64];			// set image size : 'OK', check image : 40 bytes (8(double)*5)
	int send_count = 0;

	m_imageWidth = width;
	m_imageHeight = height;
	m_imageSize = m_imageWidth * m_imageHeight;

	m_command = COMMAND_PREDICT;

	// 1. send command : 'S'
	//buf[0] = 'S';
	//GetBytes(buf+1, m_imageWidth);
	//GetBytes(buf+5, m_imageHeight);
	//int ret = m_pSocket->Send(buf, 9);		// 9 byte('S' + width + height) send

	// 1. send command : 'S'
	buf[0] = 'S';
	send_count = m_pSocket->Send(buf, 1);
	if (send_count != 1) return ABYSS_CLIENT_ERR_SEND_FAIL;

	// send predictID
	send_count = m_pSocket->Send(&m_predictID, 4);
	if (send_count != 4) return ABYSS_CLIENT_ERR_SEND_FAIL;

	// 2. send width : 4 bytes
	//GetBytes(buf, m_imageWidth);
	//send_count = m_pSocket->Send(buf, 4);
	send_count = m_pSocket->Send(&m_imageWidth, 4);
	if (send_count != 4) return ABYSS_CLIENT_ERR_SEND_FAIL;

	// 3. send height : 4 bytes
	//GetBytes(buf, m_imageHeight);
	//send_count = m_pSocket->Send(buf, 4);
	send_count = m_pSocket->Send(&m_imageHeight, 4);
	if (send_count != 4) return ABYSS_CLIENT_ERR_SEND_FAIL;

	// 4. send image data
	int total_count = 0;
	int left_count = m_imageSize;

	// 실제 데이터 전송
	while (total_count < m_imageSize)
	{
		send_count = m_pSocket->Send(pBuf + total_count, left_count);
		if (send_count == SOCKET_ERROR)
		{
			int err = GetLastError();
			if (err != WSAEWOULDBLOCK) return ABYSS_CLIENT_ERR_SEND_FAIL;
		}
		total_count += send_count;
		left_count -= send_count;
	}

	return m_predictID;
}

int AbyssClient::SendCheckImage(byte *pBuf)
{
	byte buf[16];
	int send_count = 0;

	m_command = COMMAND_PREDICT;

	// 1. send command : 'C'
	buf[0] = 'C';
	send_count = m_pSocket->Send(buf, 1);
	if (send_count != 1) return ABYSS_CLIENT_ERR_SEND_FAIL;

	// send predictID
	send_count = m_pSocket->Send(&m_predictID, 4);
	if (send_count != 4) return ABYSS_CLIENT_ERR_SEND_FAIL;

	// 2. send image data
	int total_count = 0;
	int left_count = m_imageSize;

	// 실제 데이터 전송
	while (total_count < m_imageSize)
	{
		send_count = m_pSocket->Send(pBuf + total_count, left_count);
		if (send_count == SOCKET_ERROR)
		{
			int err = GetLastError();
			if (err != WSAEWOULDBLOCK) return ABYSS_CLIENT_ERR_SEND_FAIL;
		}
		total_count += send_count;
		left_count -= send_count;
	}

	if (total_count != m_imageSize) return ABYSS_CLIENT_ERR_SEND_FAIL;

	return m_predictID;
}

extern int l_ReceiveAIData;
void AbyssClient::OnReceive(byte *pBuf, int bufSize)
{
	if (m_command == COMMAND_PREDICT)
	{
		memcpy(&m_receiveID, pBuf, 4);
		memcpy(m_pConfidence, pBuf+4, sizeof(double) * 5);
		m_command = COMMAND_NONE;

		if(m_predictID==m_receiveID)
			l_ReceiveAIData=1;

		::PostMessage(m_hMainWnd, WM_ABYSS_PREDICT_DONE_MSG, 0, 0);		
	}
}

void AbyssClient::OnClose(int nErrorCode)
{
	// server connection is cloased
	m_bConnect = false;
	m_bRemakeSocket = true;		// 다음 RetryConnect()시 소켓을 새로 만들게 설정
	::PostMessage(m_hMainWnd, WM_ABYSS_DISCONNECT_MSG, 0, 0);
}


void AbyssClient::OnConnect(int nErrorCode) 
	// derived from CAsyncSocket
{
	if (nErrorCode == 0)
	{
		m_bConnect = true;
		// connect success
		::PostMessage(m_hMainWnd, WM_ABYSS_CONNECT_MSG, 0, 0);
	}
	else m_bConnect = false;	
}
