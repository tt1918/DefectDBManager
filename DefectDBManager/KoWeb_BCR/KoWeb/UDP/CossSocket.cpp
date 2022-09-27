// KoSocket.cpp: implementation of the CCossSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CossSocket.h"
#include <atlconv.h>
#include <process.h>
#include <stdio.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCossSocket::CCossSocket():
m_hThread(NULL)
{

}

CCossSocket::~CCossSocket()
{

}

void CCossSocket::WatchComm()
{
		HANDLE hThread;
		UINT uiThreadId = 0;
		hThread = (HANDLE)_beginthreadex(NULL,	// Security attributes
								  0,	// stack
					SocketThreadProc,	// Thread proc
								this,	// Thread param
					CREATE_SUSPENDED,	// creation mode
						&uiThreadId);	// Thread ID
		if ( NULL != hThread)
		{
			ResumeThread( hThread );
			m_hThread = hThread;
			return ;
		}
}

///////////////////////////////////////////////////////////////////////////////
// SocketThreadProc
///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//     Socket Thread function.  This function is the main thread for socket
//     communication - Asynchronous mode.
// PARAMETERS:
//     LPVOID pParam : Thread parameter - a CSocketComm pointer
// NOTES:
///////////////////////////////////////////////////////////////////////////////
UINT WINAPI CCossSocket::SocketThreadProc(LPVOID pParam)
{
	CCossSocket* pThis = reinterpret_cast<CCossSocket*>( pParam );
	_ASSERTE( pThis != NULL );

	pThis->Run();

	return 1L;
} // end SocketThreadProc

void CCossSocket::Run()
{
	BYTE	byData[BUFFER_SIZE];
	LPBYTE  lpData = byData;
	DWORD	dwBytes = 0L;

	while(m_hThread)
	{
		memset(byData,0,BUFFER_SIZE);
		dwBytes = ReadComm(lpData,BUFFER_SIZE,DEFAULT_TIMEOUT);
		if(dwBytes > 0L)
		{
			OnDataReceived( lpData, dwBytes);
		}
		Sleep(0);
	}
}

BOOL CCossSocket::Create(HWND hWnd, UINT nSocketPort, LPCTSTR lpszSocketAddress, int nSocketType, long lEvent)
{
	char sBuffer[20];
	int nLen;
	m_pParent = hWnd;
	WSADATA wsaData;
	
	if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
	{		
		ErrorMessage(hWnd, _T("WSAStartup() Failure!!!"), WSAGetLastError());
		return FALSE;
	}
	
	m_Sock = socket(AF_INET, nSocketType, 0);
	if(m_Sock == SOCKET_ERROR)
	{
		ErrorMessage(hWnd, _T("socket() Failure!!!"), WSAGetLastError());
		return FALSE;
	}
	
	m_iPort = nSocketPort;
	m_Addr.sin_family = AF_INET;
	m_Addr.sin_port = htons(nSocketPort);
	
	if(lpszSocketAddress)	//sending socket
	{
		WideCharToMultiByte(CP_ACP, 0, lpszSocketAddress, 20, sBuffer, 20, NULL, NULL); //char로 변경함
		//MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, sBuffer, strlen(sBuffer), aaa, 40); TCHAR aaa[40]={0,};
		m_Addr.sin_addr.s_addr = inet_addr(sBuffer);

		BOOL optval = TRUE;
		if ( SOCKET_ERROR == setsockopt( m_Sock, SOL_SOCKET, SO_BROADCAST, (char *) &optval, sizeof( BOOL ) ) )
		{
			ErrorMessage(hWnd, _T("socket error!!!"), WSAGetLastError());
			Close();
			return FALSE;
		}	
	}
	else					//recieve socket
	{
		m_Addr.sin_addr.s_addr = htonl(INADDR_ANY);
		// Inform Windows Sockets provider that broadcast messages are allowed

		if(bind(m_Sock, (struct sockaddr *)&m_Addr, sizeof(m_Addr)) == SOCKET_ERROR)
		{
			ErrorMessage(hWnd, _T("bind() Failure!!!"), WSAGetLastError());
			Close();
			return FALSE;
		}
		if(WSAAsyncSelect(m_Sock, hWnd, WSA_ASYNC, FD_ACCEPT | FD_READ | FD_CLOSE | FD_OOB) == SOCKET_ERROR) // | FD_READ | FD_CLOSE
		{
			ErrorMessage(hWnd, _T("WSAAsyncSelect(FD_ACCEPT) Failure!!!"), WSAGetLastError());
			return FALSE;
		}
	}
	
	return TRUE;
}

void CCossSocket::ErrorMessage(HWND hwnd, TCHAR *error, int err_num)
{
	TCHAR	szError[80];
	
	_stprintf(szError, _T("%s : %d"), error, err_num);
	MessageBox(hwnd, szError, _T("UDP Network"), MB_OK | MB_ICONERROR);
}

void CCossSocket::Close()
{
	closesocket(m_Sock);
	WSACleanup();
}

int CCossSocket::SendTo(const void* lpBuf, int nBufLen, int nFlags)
{
	_ASSERTE( nBufLen<BUFFER_SIZE );

	char buf[BUFFER_SIZE];
	memcpy(buf,lpBuf,nBufLen);
	int iResult = sendto(m_Sock, buf, nBufLen, nFlags, (const struct sockaddr *)&m_Addr, sizeof(m_Addr));

	return iResult;
}

DWORD CCossSocket::ReadComm(LPBYTE lpBuffer, DWORD dwSize,DWORD dwTimeout)
{
	LPBYTE lpData = lpBuffer;

	if (lpBuffer == NULL || dwSize < 1L)
		return 0L;

	fd_set	fdRead  = { 0 };
	TIMEVAL	stTime;
	TIMEVAL	*pstTime = NULL;

	if ( INFINITE != dwTimeout ) {
		stTime.tv_sec = 0;
		stTime.tv_usec = dwTimeout;
		pstTime = &stTime;
	}

	// Set Descriptor
	if ( !FD_ISSET( m_Sock, &fdRead ) )
		FD_SET( m_Sock, &fdRead );

	// Select function set read timeout
	DWORD dwBytesRead = 0L;
	int res = select( m_Sock+1, &fdRead, NULL, NULL, pstTime );
	if(res>0)
	{
		res = recv(m_Sock, (LPSTR)lpData, dwSize, 0);
	}

	dwBytesRead = (DWORD)((res >= 0)?(res) : (-1L));
	return dwBytesRead;
}

void CCossSocket::OnDataReceived(LPBYTE lpBuffer, DWORD dwCount)
{

}

DWORD CCossSocket::WriteComm(LPBYTE lpBuffer, DWORD dwSize,DWORD dwTimeout, int nFlags)
{
	// Accept 0 bytes message
	if (NULL == lpBuffer)
		return 0L;

	fd_set	fdWrite  = { 0 };
	TIMEVAL	stTime;
	TIMEVAL	*pstTime = NULL;

	if ( INFINITE != dwTimeout ) {
		stTime.tv_sec = 0;
		stTime.tv_usec = dwTimeout;
		pstTime = &stTime;
	}
	// Set Descriptor
	if ( !FD_ISSET( m_Sock, &fdWrite ) )
		FD_SET( m_Sock, &fdWrite );

	// Select function set write timeout
	DWORD dwBytesWritten = 0L;
	int res = select( m_Sock+1, NULL, &fdWrite, NULL, pstTime );
	if(res>0)
	{
		res = SendTo(lpBuffer,dwSize);
		dwBytesWritten = (DWORD)((res >= 0)?(res) : (-1));
	}
	return res;
}

void CCossSocket::StopComm()
{
	if (WaitForSingleObject(m_hThread, 1000L) == WAIT_TIMEOUT)
		TerminateThread(m_hThread, 1L);
	CloseHandle(m_hThread);
	m_hThread = NULL;
}




/*
int CCossSocket::GetIpAdr()
{
	char name[256];
	char ip[4];
	memset(ip,0,4);
	PHOSTENT hostinfo;
    int re = gethostname(name, sizeof(name));

    if(re == 0)
    {
        if((hostinfo = gethostbyname(name)) != NULL)
        {
            strcpy(name,inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list));
			re = strlen(name);
			int dot=0;
			for(int i=1;i<re&&dot!=3;i++)
			{
				if(name[i]=='.')
					dot++;
			}
			_ASSERTE( dot==3 );
			memcpy(ip,&name[i],re-i);
        }
    }
	
   return atoi(ip);
}
*/