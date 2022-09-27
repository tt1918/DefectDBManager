// sio.cpp : implementation file
//

///////////////////////////////////////////////////////////////////////////////
// Turn off Optimization
#pragma optimize("",off)
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sio.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSio
CSio g_Sio;

#ifdef USE_ROTATION
	CSio g_SioMotion;
#endif
	
CSio::CSio()
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_sFileName = _T("Untitled.txt");
}

CSio::~CSio()
{
	CloseHandle(m_hFile);
}

/////////////////////////////////////////////////////////////////////////////
// CSio message handlers

/////////////////////////////////////////////////////////////////////////////
// Name:		InitComm
//
// Description:	This function will initialize the communications port.
//
// Entry:		
//				port - Comm port (1 - 256)
//				baud - serial baud rate (300 - 115200)
//				parity - NOPARITY, ODDPARITY, EVENPARITY, MARKPARITY, SPACEPARITY
//				data - 7 or 8
//				stop - ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS
//
// Returned:	True if successful
///////////////////////////////////////////////////////////////////////////////
BOOL CSio::InitComm(int nPort,int nBaud,int nParity,int nData,int nStop,
				   BOOL bNoError)
{
//	char s[100];
	TCHAR s[100];
	int err;
	DCB dcb;
	
	m_nBaud = nBaud;
	m_nPort = nPort;

	m_bErrorFlag = FALSE;
	// setup receive window handle

	///////////////////////////////////////////////////////////////////////////	     
//	sprintf(s,"COM%d", nPort);

	if(nPort>9)
	{
//		sprintf(s, "\\\\.\\COM%d", nPort);
		_stprintf(s, _T("\\\\.\\COM%d"), nPort);
	}
	else
	{
//		sprintf(s, "COM%d", nPort);
		_stprintf(s, _T("COM%d"), nPort);
	}


	m_hFile = CreateFile(
		s,							// pointer to name of the file 
	    GENERIC_READ|GENERIC_WRITE,	// access (read-write) mode 
	    0,							// share mode 
	    NULL,						// pointer to security descriptor 
	    OPEN_EXISTING,				// how to create 
		FILE_ATTRIBUTE_NORMAL,		// file attributes 
		NULL 						// handle to file with attributes to copy  
	);

	if(m_hFile == INVALID_HANDLE_VALUE)
	{
		m_bErrorFlag = TRUE;
		DWORD dwLastError = GetLastError();
		CString sError;
		switch(dwLastError)
		{
			///////////////////////////////////////////////////////////////////
			case ERROR_ACCESS_DENIED: 
				sError = _T("Access Denied!\n\n The communications port may be in use at this time. Please choose another port or close the application using the desired port.");
			break;

			///////////////////////////////////////////////////////////////////
			case ERROR_FILE_NOT_FOUND: 
				sError = _T("Communication Port Not Found!\n\n The communications port does not exist. Check your system configuration on the status of this Communication port");
			break;

			///////////////////////////////////////////////////////////////////
			default: 
				sError = _T("Comm Open Error! The communications port may be in use");
			break;
		}

		if(bNoError == FALSE) AfxMessageBox(sError,MB_ICONSTOP|MB_OK);
		
		return FALSE;
	}
	
	err = GetCommState(m_hFile,&dcb);

	if (err < 0)
	{
		m_bErrorFlag = TRUE;
		if(bNoError == FALSE) MessageBeep(MB_ICONSTOP);
	    if(bNoError == FALSE) AfxMessageBox(_T("Could not get comm state"),
			MB_OK|MB_ICONSTOP);
	    return FALSE;
	}


	///////////////////////////////////////////////////////////////////////////
	// Need to se te baud seperately
	dcb.BaudRate = (DWORD)nBaud;
	dcb.Parity = (BYTE)nParity;
	dcb.StopBits = (BYTE)nStop;
	dcb.ByteSize = (BYTE)nData;

	dcb.fBinary = TRUE;						// binary mode, no EOF check 
	dcb.fOutxCtsFlow = FALSE;				// CTS output flow control 
	dcb.fOutxDsrFlow = FALSE;				// DSR output flow control 
	dcb.fDtrControl = DTR_CONTROL_ENABLE;  // DTR flow control type 
	dcb.fDsrSensitivity = FALSE;			// DSR sensitivity 
	dcb.fOutX = FALSE;						// XON/XOFF out flow control 
	dcb.fInX =	FALSE;						// XON/XOFF in flow control 
	dcb.fNull = FALSE;						// enable null stripping 
	dcb.fRtsControl=RTS_CONTROL_ENABLE;	// RTS flow control 
	dcb.fAbortOnError = FALSE;				// abort reads/writes on error 

	err = SetCommState(m_hFile,&dcb);

	if (err < 0) 
	{
		m_bErrorFlag = TRUE;
		if(bNoError == FALSE) MessageBeep(MB_ICONSTOP);
	    if(bNoError == FALSE)
			AfxMessageBox(_T("Could not set communication state"),MB_OK|MB_ICONSTOP);
	    return FALSE;
	}

	///////////////////////////////////////////////////////////////////////////
	// Setup the comm timeouts
	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = MAXDWORD ;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0 ;
	CommTimeOuts.ReadTotalTimeoutConstant = 0 ;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0 ;
	CommTimeOuts.WriteTotalTimeoutConstant = 1000 ;
	SetCommTimeouts( m_hFile, &CommTimeOuts ) ;

	DWORD dwErrors;
	COMSTAT ComStat;
	ClearCommError(m_hFile,&dwErrors,&ComStat);

	SetupComm( m_hFile, 16384, 16384 ); 

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSio::Close()
{
	BOOL bVal = CloseHandle(m_hFile);
	m_hFile = INVALID_HANDLE_VALUE;
	Sleep(1000);
	return bVal;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
void CSio::OpenFile(CString sFileName)
{
	m_sFileName = sFileName;
	m_hFile = CreateFile(
		sFileName,					// pointer to name of the file 
	    GENERIC_READ,				// access (write) mode 
	    0,							// share mode 
	    NULL,						// pointer to security descriptor 
	    OPEN_EXISTING,				// how to create 
		FILE_ATTRIBUTE_READONLY,	// file attributes 
		NULL 						// handle to file with attributes to copy  
	);
	if(m_hFile == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("CSio:File Error!"), MB_ICONSTOP);
	}	
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
DWORD CSio::Read(BYTE *pData, DWORD dwLen)
{
	DWORD dwBytesRead;

	if(m_hFile == INVALID_HANDLE_VALUE)
		return 0;

	///////////////////////////////////////////////////////////////////////////
	ReadFile(
		m_hFile,			// handle of file to read 
		pData,				// address of buffer that receives data  
		dwLen,				// number of bytes to read 
		&dwBytesRead,		// address of number of bytes read 
		NULL				// address of structure for data 
	);

	///////////////////////////////////////////////////////////////////////////
	DWORD dwLastError = GetLastError();
	DWORD dwErrors;
	COMSTAT ComStat;
	ClearCommError(m_hFile,&dwErrors,&ComStat);
	///////////////////////////////////////////////////////////////////////////

	return dwBytesRead;
}

///////////////////////////////////////////////////////////////////////////////
// Name:		SendBMessage
//
// Description:	This function will send the passed binary message to the GPS
//				engine.
//
// Entry:		String to pass
///////////////////////////////////////////////////////////////////////////////
DWORD CSio::Write(BYTE *pBuff, DWORD dwLen)
{
	DWORD dwBytesWrittern;
	
	BOOL bResult = WriteFile(
	    m_hFile,				// handle to file to write to 
		pBuff,					// pointer to data to write to file 
		dwLen,					// number of bytes to write 
		&dwBytesWrittern,		// pointer to number of bytes written 
		NULL				 	// addr. of structure needed for overlapped I/O  
	);

	return dwBytesWrittern;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
