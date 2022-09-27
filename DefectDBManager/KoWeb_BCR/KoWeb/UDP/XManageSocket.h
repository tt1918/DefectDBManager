// XManageSocket.h: interface for the CXManageSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMANAGESOCKET_H__9F129CF5_44BF_4F3F_8CD7_C179C4EF9DF0__INCLUDED_)
#define AFX_XMANAGESOCKET_H__9F129CF5_44BF_4F3F_8CD7_C179C4EF9DF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CossSocket.h"
#include "Packet.h"

class CXManageSocket : public CCossSocket
{
public:
	void SendInsData(CPacket *packet);
	void SendInsData_Buffer(LPBYTE pBuffer, int nBufLength);
	CXManageSocket();
	virtual ~CXManageSocket();
	virtual void OnDataReceived(const LPBYTE lpBuffer, DWORD dwCount);
	void SendInstantData_LocalHost(LPCSTR param_name, LPSTR str_data);
	void SendInstantData_LocalHost(LPCSTR param_name, double value);
	void SendInstantData_LocalHostBCR(LPCSTR param_name, double value);
	void SendCommand_LocalHost(int nCommand);
	void SendInstantData_LocalHostLight(LPCSTR param_name, double value);

	void SendtoServer_Data(TCHAR *param_name, double value);
};

#endif // !defined(AFX_XMANAGESOCKET_H__9F129CF5_44BF_4F3F_8CD7_C179C4EF9DF0__INCLUDED_)
