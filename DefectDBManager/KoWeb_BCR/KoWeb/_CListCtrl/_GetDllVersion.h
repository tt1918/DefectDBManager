#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#pragma warning (disable: 4995)
#include "shlwapi.h"
#pragma warning (default: 4995)

// return value : MAKELONG(DLLVERSIONINFO2의 info1.dwMinorVersion, DLLVERSIONINFO2의 info1.dwMajorVersion);

static DWORD _GetDllVersion(__in HMODULE hModule) // Handle to the module
{
	DWORD dwVersion = 0;
	if (hModule)
	{
		DLLGETVERSIONPROC pDllGetVersion;
		pDllGetVersion = (DLLGETVERSIONPROC)::GetProcAddress(hModule, "DllGetVersion");

		// Because some DLLs might not implement this function, you must test for 
		// it explicitly. Depending on the particular DLL, the lack of a DllGetVersion 
		// function can be a useful indicator of the version. 
		if (pDllGetVersion)
		{
			DLLVERSIONINFO2 dvi2;
			::ZeroMemory(&dvi2, sizeof(DLLVERSIONINFO2));
			dvi2.info1.cbSize = sizeof(DLLVERSIONINFO2);
			if (SUCCEEDED((*pDllGetVersion)(&dvi2.info1))) // dvi.info1을 넘겨도 dvi.ullVersion이 설정되어진다.
				dwVersion = MAKELONG(dvi2.info1.dwMinorVersion, dvi2.info1.dwMajorVersion);
		}
	}
	return dwVersion;
}

static DWORD _GetDllVersion(__in_z LPCTSTR lpModuleName) // The name of the loaded module (.dll file)
{
	DWORD dwVersion = 0;

	if (lpModuleName)
	{
		HMODULE hModule;
		::GetModuleHandleEx(0, lpModuleName, &hModule);

		if (hModule)
		{
			dwVersion = ::_GetDllVersion(hModule);
			::FreeLibrary(hModule);
		}
	}
	return dwVersion;
}

static DWORD _GetDllVersion(__in_z LPCTSTR lpFullPath,  // fully qualified path
									__in_z LPCTSTR lpModuleName)
{
	DWORD dwVersion = 0;

	// For security purposes, LoadLibrary should be provided with a fully qualified 
	// path to the DLL. The lpszDllName variable should be tested to ensure that it 
	// is a fully qualified path before it is used.
	if (lpFullPath && lpModuleName)
	{
		TCHAR lpszDllName[MAX_PATH] = {0};
		::_tcscat_s(lpszDllName, MAX_PATH, lpFullPath);
		::_tcscat_s(lpszDllName, MAX_PATH, _T("\\"));
		::_tcscat_s(lpszDllName, MAX_PATH, lpModuleName);

		HMODULE hModule = ::LoadLibrary(lpszDllName);

		if (hModule)
		{
			dwVersion = ::_GetDllVersion(hModule);
			::FreeLibrary(hModule);
		}
	}
	return dwVersion;
}
