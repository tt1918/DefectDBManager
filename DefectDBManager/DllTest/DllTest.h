
// DllTest.h : main header file for the DllTest application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CDllTestApp:
// See DllTest.cpp for the implementation of this class
//

class CDllTestApp : public CWinApp
{
public:
	CDllTestApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDllTestApp theApp;
