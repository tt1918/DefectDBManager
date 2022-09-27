#pragma once

#include <afxsock.h>

// CConectSocket 명령 대상입니다.

class CConectSocket : public CAsyncSocket
{
public:
	CConectSocket(void *pAbyssClient);
	virtual ~CConectSocket();


	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);

protected:
	void *m_pAbyssClient;
	byte m_buf[128];
};


