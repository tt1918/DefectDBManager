#if !defined(AFX_ABYSSCLIENT_H__E6AEB86B_5CC9_4A64_A31F_ED76D1F5CBC4__INCLUDED_)
#define AFX_ABYSSCLIENT_H__E6AEB86B_5CC9_4A64_A31F_ED76D1F5CBC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AbyssClient.h : header file
//

#include <functional>
#include "ConnectSocket.h"
#include <afxsock.h>

#ifndef SAFE_DELETE
#define SAFE_DELETE(x) if(x) {delete x; x = nullptr;}
#endif

// 서버 연결완료 메시지
#define WM_ABYSS_CONNECT_MSG		(WM_USER+1000)
// 서버와 연결이 끊어짐
#define WM_ABYSS_DISCONNECT_MSG		(WM_USER+1001)
// 이미지 인식 완료 메시지
#define WM_ABYSS_PREDICT_DONE_MSG	(WM_USER+1002)

#define ABYSS_CLIENT_SUCCESS				0
#define ABYSS_CLIENT_ERR_NOT_CONNECTED		-1
#define ABYSS_CLIENT_ERR_SEND_FAIL			-2

#define ABYSS_CLIENT_RECONNECT_TIMER_ID		3000

enum
{
	COMMAND_NONE,
	COMMAND_CONNECT_SERVER,
	COMMAND_SET_IMAGE_SIZE,
	COMMAND_PREDICT
};


typedef void(*AbyssClientCallback)(double *pConfidenceArray);

/////////////////////////////////////////////////////////////////////////////
// AbyssClient command target

class AbyssClient
{
// Attributes
public:

// Operations
public:
	//AbyssClient(AbyssClientCallback cb);
	AbyssClient();
	virtual ~AbyssClient();

	int ConnectServer(LPCTSTR lpszHostAddress, UINT nHostPort);

// Overrides
public:

	int RetryConnect();
	int ConnectServerStart(LPCTSTR lpszHostAddress, UINT nHostPort);

	// return <= -1 : 에러, ABYSS_CLIENT_ERR_NOT_CONNECTED or ABYSS_CLIENT_ERR_SEND_FAIL
	// return >= 1 : predictID값, 서버에서 결과를 받은 후(OnReceive()함수 호출됨) m_receiveID변수에 인식결과 ID 업데이트 됨
	int Predict(byte *pBuf, int width, int height);
	// AbyssClient의 내부변수 m_pConfidence[5]를 return
	double* GetPredictResult();

	void OnReceive(byte *pBuf, int bufSize);
	void OnClose(int nErrorCode);
	void OnConnect(int nErrorCode);

// Implementation
protected:
	HWND m_hMainWnd;
	CConectSocket* m_pSocket;
	int m_imageWidth;
	int m_imageHeight;
	int m_imageSize;
	byte *m_pBuf;
	double m_pConfidence[5];
	int m_predictID;
	int m_receiveID;
	int m_command;

	CConectSocket* CreateSocket();

	void GetBytes(byte *pBuf, int n);
	int SendCheckImage(int width, int height, byte *pBuf);
	int SendCheckImage(byte *pBuf);

public:
	bool m_bAfxSocketInit;
	LPCTSTR m_lpszHostAddress;
	UINT m_nHostPort;
	bool m_bConnect;
	bool m_bRemakeSocket;
	bool m_bAutoReconnect;
	int m_nStatus;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABYSSCLIENT_H__E6AEB86B_5CC9_4A64_A31F_ED76D1F5CBC4__INCLUDED_)
