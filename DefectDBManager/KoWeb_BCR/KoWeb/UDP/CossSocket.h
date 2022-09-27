// KoSocket.h: interface for the CCossSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KOSOCKET_H_990605_INCLUDED_)
#define AFX_KOSOCKET_H_990605_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <winsock2.h>
#define WSA_ASYNC		(WM_USER+1)
#define	BUFFER_SIZE		20480	//UDP PACKET MAX SIZE
#define	DEFAULT_TIMEOUT	10000L	//10msec

class CCossSocket  
{

protected:
	static UINT WINAPI SocketThreadProc(LPVOID pParam);
	SOCKET			m_Sock;
	SOCKADDR_IN		m_Addr;
	HWND			m_pParent;
	HANDLE			m_hThread;		// Thread Comm handle
	unsigned short	m_iPort;

public:
	CCossSocket();
	virtual ~CCossSocket();
	virtual void OnDataReceived(const LPBYTE lpBuffer, DWORD dwCount);

	void Run();
	void WatchComm();
	void StopComm();
	int SendTo(const void* lpBuf, int nBufLen, int nFlags=0);
	DWORD WriteComm(LPBYTE lpBuffer,DWORD dwSize,DWORD dwTimeout,int nFlags =0);
	DWORD ReadComm(LPBYTE lpBuffer, DWORD dwSize, DWORD dwTimeOut);
	void Close();
	void ErrorMessage(HWND hwnd, TCHAR *error, int err_num);
	BOOL Create(HWND hWnd,
               UINT nSocketPort = 0,
               LPCTSTR lpszSocketAddress = NULL, 
               int nSocketType = SOCK_DGRAM,
               long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE
               );
};

#endif // !definedAFX_KOSOCKET_H_990605_INCLUDED_
