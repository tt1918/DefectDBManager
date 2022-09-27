// ConectSocket.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ConnectSocket.h"
#include "AbyssClient.h"

// CConectSocket

CConectSocket::CConectSocket(void *pAbyssClient)
{
	m_pAbyssClient = pAbyssClient;
}

CConectSocket::~CConectSocket()
{
}


// CConectSocket 멤버 함수

void CConectSocket::OnConnect(int nErrorCode)
{
	((AbyssClient *)m_pAbyssClient)->OnConnect(nErrorCode);
	CAsyncSocket::OnConnect(nErrorCode);
}

void CConectSocket::OnClose(int nErrorCode)
{
	ShutDown();
	Close();
	
	((AbyssClient *)m_pAbyssClient)->OnClose(nErrorCode);

	CAsyncSocket::OnClose(nErrorCode);

	//AfxMessageBox(_T("ERROR:Disconnected from server!"));
	//::PostQuitMessage(0);
}

//  OnReceive가 CAsyncSocket 클래스의 가상함수 인데, 여기서 Receive를 두번이상 호출 하면 안정성 보장이 안된다!!!
// http://lab.deepaivision.com/2014/11/casyncsocket-onreceive.html
void CConectSocket::OnReceive(int nErrorCode)
{
	//Receive를 분할 수신하면... 랜덤하게 100000번 내외에서 못받음 현상발생
	DWORD dwReadLen;
	IOCtl(FIONREAD, &dwReadLen); //패킷 길이를 먼저 파악하고

	int nReadLen = 0;
	nReadLen = Receive(m_buf, dwReadLen); //한번에 다받아야 한다..

	// predictID(4) + double(8)*5 = 44 bytes

	//if (nReadLen != 44)
	//{
	//	int i = 10;
	//}

	//if(0)
	//{
	//	CString STR;
	//	STR.Format(_T("OnReceive : %d\n"), nReadLen);
	//	OutputDebugString(STR);
	//}


	((AbyssClient *)m_pAbyssClient)->OnReceive(m_buf, nReadLen);

	CAsyncSocket::OnReceive(nErrorCode);
}
