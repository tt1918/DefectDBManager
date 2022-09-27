// sio.h : header file
//
#ifndef SIO_H
#define SIO_H

/////////////////////////////////////////////////////////////////////////////
// CSio window

class CSio
{
	int m_bErrorFlag;

public:	
	HANDLE m_hFile;		// handle to comm device
	CString m_sFileName;
	int m_nBaud;
	int m_nPort;

// Construction
public:
	CSio();
	~CSio();
// Attributes
public:

// Operations
public:
	BOOL IsConnectError(){ return m_bErrorFlag; }
	BOOL InitComm(int nPort,int nBaud,int nParity,int nData,int nStop, BOOL bNoError);
	BOOL Close();
	void OpenFile(CString sFileName);
	DWORD Write(BYTE *pBuff, DWORD dwLen);
	DWORD Read(BYTE *pData, DWORD dwLen);
};

extern CSio g_Sio;

#ifdef USE_ROTATION
	extern CSio g_SioMotion;
#endif
/////////////////////////////////////////////////////////////////////////////
#endif
