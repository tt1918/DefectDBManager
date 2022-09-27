#include "stdafx.h"
#include "Direct.h"


void HDDSpace_Sub(CString strPath,  double *dSpace);

int SavePNG(CString filename, LPBYTE fmS) 
{
    BYTE *src = fmS;
    BYTE *dst = (BYTE *)g_Save.GetBits();
	int pitch=g_Save.GetPitch();
	int y;
	for (y=0; y < g_System.m_nImageH; y++,src+=g_System.m_nImageW,dst+=pitch)
        CopyMemory(dst, src, g_System.m_nImageW);
	
	g_Save.Save(filename, Gdiplus::ImageFormatPNG);

	return 0;
}

int SavePNGComp(CString filename, LPBYTE fmS, int width, int height) 
{
    BYTE *src = fmS;
    BYTE *dst = (BYTE *)g_SaveComp.GetBits();
	int pitch=g_SaveComp.GetPitch();
	int y;
	for (y=0; y < height; y++,src+=width,dst+=pitch)
        CopyMemory(dst, src, width);
	
	g_SaveComp.Save(filename, Gdiplus::ImageFormatPNG);

	return 0;
}


int SaveBMP(CString filename, LPBYTE fmS) 
{
    BYTE *src = fmS;
    BYTE *dst = (BYTE *)g_Save.GetBits();
	int pitch=g_Save.GetPitch();
	int y;

	for (y=0; y < g_System.m_nImageH; y++,src+=g_System.m_nImageW,dst+=pitch)
        CopyMemory(dst, src, g_System.m_nImageW);
	
	g_Save.Save(filename, Gdiplus::ImageFormatBMP);

	return 0;
}

int SaveJPG(CString filename, LPBYTE fmS) 
{
    BYTE *src = fmS;
    BYTE *dst = (BYTE *)g_Save.GetBits();
	int pitch=g_Save.GetPitch();
	int y;

	for (y=0; y < g_System.m_nImageH; y++,src+=g_System.m_nImageW,dst+=pitch)
        CopyMemory(dst, src, g_System.m_nImageW);
	
	g_Save.Save(filename, Gdiplus::ImageFormatJPEG);

	return 0;
}

LARGE_INTEGER counter_start;
double Clock_set()
{
	double duration_milisec;
	LARGE_INTEGER freq, set;
	if(!QueryPerformanceFrequency(&freq))
		return -1000.;	

	QueryPerformanceCounter(&set);

	LARGE_INTEGER duration;
	duration.QuadPart = (set.QuadPart - counter_start.QuadPart);
	duration_milisec = (double)duration.QuadPart/(freq.QuadPart/1000.0);

	return duration_milisec;
}

int HandleAllMessage()
{
	int returnValue;
	MSG Mess;
	do
	{
		returnValue=::PeekMessage(&Mess, NULL,0,0, PM_REMOVE);
		if(returnValue)
		{
			::TranslateMessage(&Mess);
			::DispatchMessage (&Mess);
		}
	}while(returnValue);

	return returnValue;
}

//0.1msec 이하
double HDDSpace()
{
	double  dSpace;
	
	HDDSpace_Sub(_T("C:\\"), &dSpace);

	return dSpace;
}

//0.1msec 이하
void HDDSpace_Sub(CString strPath,  double *dSpace)
{
	ULARGE_INTEGER lpFree;
	ULARGE_INTEGER lpTotal;
	ULARGE_INTEGER lpOccupied;

	double dTotal = 0;
	double dRemaining = 0;
	double dOccupied = 0;

	GetDiskFreeSpaceEx(strPath, &lpFree, &lpTotal, &lpOccupied);

	dRemaining = ((double)lpFree.HighPart * UINT_MAX) + lpFree.LowPart; // 남은 공간
	dTotal = ((double)lpTotal.HighPart * UINT_MAX) + lpTotal.LowPart;   // 총 C:공간
	dOccupied = dTotal - dRemaining;									// 사용한 공간

	if(dTotal) *dSpace = 100.*dRemaining /dTotal;	
	else       *dSpace = 0;
}

//맨앞 첫째자리
//1: 코팅
//2: 연신
//3: 정밀코팅
CString GetSWVersion()
{
//	return _T("2 . 21 . 1 . 25");  //1동 정코 1호기 Edge투과 검사하도록 설정  

	return _T("1 . 22 . 1 . 1");







/*
#ifndef USE_X64	
	// 확인 필요사항!--> C:\Program Files\Microsoft Visual Studio\VC98\Lib 의
	// version.lib 를 프로젝트에 포함해야 한다.
	BYTE       *block;
	DWORD FAR  *translation;
	DWORD FAR  *buffer;
	DWORD        handle;
	UINT        bytes;

#ifdef INTERFACE_NEXTEYE_SERVER
	TCHAR       *lpszFileName  = _T("c:\\nexteye\\VisWeb.exe");
#else
	TCHAR       *lpszFileName  = _T("c:\\COSS\\CossWeb.exe");
#endif

	TCHAR        name[512];
	CString      szString;
	
	// 버전 정보의 실제 크기를 가져온다.
	bytes = (UINT)::GetFileVersionInfoSize( lpszFileName, &handle);
	
	if(bytes) {
		block = new BYTE[bytes];
		
		// 버전 정보에 대한 실제 블록을 가져온다.
		if( ::GetFileVersionInfo( lpszFileName, handle, bytes, block) ) {
			if( ::VerQueryValue( block, _T("\\VarFileInfo\\Translation"), (LPVOID *)&translation,
				(UINT FAR *)&bytes) ) {
				
				// 버전 정보
				wsprintf(name, _T("\\StringFileInfo\\%04x%04x\\FileVersion"), 
					LOWORD(*translation), HIWORD(*translation) );
				
				if( ::VerQueryValue(block, name, (LPVOID *)&buffer, (UINT FAR *)&bytes) ) {
					szString.Format(_T("%s"), (LPCTSTR)buffer );

				delete[] block;
				return szString;

				}
				else	
					szString.Format(_T("Unable to get Version Info"));
				
				// Product Name 정보
				wsprintf(name, _T("\\StringFileInfo\\%04x%04x\\ProductName"), LOWORD(*translation),          HIWORD(*translation) );
				
				if( ::VerQueryValue(block, name, (LPVOID *)&buffer, (UINT FAR *)&bytes) ) {
					szString.Format( _T("%s"), (LPCTSTR)buffer );
				}
				else
					szString.Format( _T("Unable to get Product Name") );
			}
			else 
				szString.Format( _T("(Unable to get translation type)") );
		}
		else 
			szString.Format( _T("(Unable to get FileVersionInfo)") );
		
		delete [] block;
	}
	else {
		szString.Format( _T("(Unable to get FileVersionInfo size)") );
	}
	return szString;
#else
	return _T("1.21.1.14");
#endif
*/
}


CString GetPCName()
{
	int i;
	TCHAR nTmp[4];
	CString str;
	char aaa[256];
	TCHAR sPCName[MAX_COMPUTERNAME_LENGTH + 1] = _T("\0") ;
	DWORD dwSize = MAX_COMPUTERNAME_LENGTH + 1 ;
	GetComputerName( sPCName, &dwSize ) ;
//	AfxMessageBox(sPCName);

	str.Format(_T("%s"), sPCName);

#ifdef TEST_MODE      
	
		//str = "NEL-101";  
		//str = "COS-204";   //보내는 PC
		str = "COS-905";  
		//str = "NEL-401"; 
		//str = "COS-504";  
		//str = "COS-804";	//받는 PC
		//str = "NEL-604"; 
		//str = "NEL-704";  
		//str = "NEL-701";
		//str = "NEL-802";   //EDGE

		//str = "NEL-905";  
		//str = "COS-A04";  		
		//str = "NEL-B04";	
	#ifdef USE_SK_BAT
		str = "NEL-201";  
	#endif

	#ifdef BARCODE_VISION
		str = "BCR-101"; 
	#endif

	#ifdef MARKING_VISION
		str = "MAK-101"; 
	#endif

		_stscanf(str, _T("%s"), sPCName);
#endif
#ifdef GRAB_TEST_MODE
	str = "NEL-804" ; 
	_stscanf(str, _T("%s"), sPCName);
#endif


	g_Temp.m_sMyComName.Format(_T("%s"), str);

	if(str.GetLength()==7)
	{
		for(i=0;i<7;i++)
			g_Temp.m_nPCFullName[i]=sPCName[i];
		g_Temp.m_nPCFullName[7]=NULL;

		nTmp[0]=sPCName[4];
		nTmp[1]=sPCName[5];
		nTmp[2]=sPCName[6];
		nTmp[3]=NULL;

		g_Temp.m_nPCNum  =_ttoi(&nTmp[1]);

		nTmp[1]=NULL;
		if(nTmp[0]==_T('A'))		g_Temp.m_nPCFirstNo=COSA;
		else if(nTmp[0]==_T('B'))	g_Temp.m_nPCFirstNo=COSB;
		else if(nTmp[0]==_T('C'))	g_Temp.m_nPCFirstNo=COSC;
		else						g_Temp.m_nPCFirstNo=_ttoi(nTmp);

		g_Temp.m_nPCID=g_Temp.m_nPCFirstNo*OPTICDEV+g_Temp.m_nPCNum;   //OPTICDEVL 0x100

		g_Temp.m_nPCOptic=GetClientOptic(g_Temp.m_sMyComName);  //광학계는 NEL-9 가 9가 아닐수 있지만 g_Temp.m_nPCFirstNo는 "9" 임.
	}	

	return str;
}

int GetClientOptic(CString sComName)
{
	int nRet=0; 

	if(sComName.Find(_T("NEL-1"))>=0	  || sComName.Find(_T("COS-1"))>=0)	{	nRet = COS1;	}	//0x01
	else if(sComName.Find(_T("NEL-2"))>=0 || sComName.Find(_T("COS-2"))>=0)	{	nRet = COS2;	}	//0x02
	else if(sComName.Find(_T("NEL-3"))>=0 || sComName.Find(_T("COS-3"))>=0)	{	nRet = COS3;	}	//0x03
	else if(sComName.Find(_T("NEL-4"))>=0 || sComName.Find(_T("COS-4"))>=0)	{	nRet = COS4;	}	//0x04
	else if(sComName.Find(_T("NEL-5"))>=0 || sComName.Find(_T("COS-5"))>=0)	{	nRet = COS5;	}	//0x05
	else if(sComName.Find(_T("NEL-6"))>=0 || sComName.Find(_T("COS-6"))>=0)	{	nRet = COS6;	}	//0x06
	else if(sComName.Find(_T("NEL-7"))>=0 || sComName.Find(_T("COS-7"))>=0)	{	nRet = COS7;	}	//0x07
	else if(sComName.Find(_T("NEL-8"))>=0 || sComName.Find(_T("COS-8"))>=0)	{	nRet = COS8;	}	//0x08
	else if(sComName.Find(_T("NEL-9"))>=0 || sComName.Find(_T("COS-9"))>=0)	{	nRet = COS9;	}	//0x09	
	else if(sComName.Find(_T("NEL-A"))>=0 || sComName.Find(_T("COS-A"))>=0)	{	nRet = COSA;	}	//0x0A
	else if(sComName.Find(_T("NEL-B"))>=0 || sComName.Find(_T("COS-B"))>=0)	{	nRet = COSB;	}	//0x0B  0x0F까지확장가능 (총 16개)	
	else if(sComName.Find(_T("NEL-C"))>=0 || sComName.Find(_T("COS-C"))>=0)	{	nRet = COSC;	}	//0x0C
	else if(sComName.Find(_T("NEL-D"))>=0 || sComName.Find(_T("COS-D"))>=0)	{	nRet = COSD;	}	//0x0D
	else if(sComName.Find(_T("NEB"))>=0   || sComName.Find(_T("BCR"))>=0)	{	nRet = CBCR;	}	//0x0E  BCR
	else if(sComName.Find(_T("NEM"))>=0	  || sComName.Find(_T("MAK"))>=0)	{	nRet = CMAK;	}	//0x0F  MARKING VISION

	return nRet;
}

int GetIpAddress()
{
	int i;
	char szHostName[256];
	HOSTENT *pHostInfo = NULL;
	IN_ADDR inAddr;
	
	int re = gethostname(szHostName, 256);
	pHostInfo = gethostbyname(szHostName);
	
    if(re == 0)
    {
		for(i = 0 ; pHostInfo->h_addr_list[i] != NULL ; i++)
		{
			memcpy(&inAddr, pHostInfo->h_addr_list[i], sizeof(IN_ADDR));
			inet_ntoa(inAddr);  // 리턴값이 자신의 IP
			if(inAddr.S_un.S_un_b.s_b1 == 100)
				return inAddr.S_un.S_un_b.s_b4;
		}
	}
	else
		return 1;

	return 0;
}

//해당폴더는 두고 그 내부(파일, 폴더)는 모두 없앤다
void Delete_All_in_Folder(CString path)
{
	CString file;
	
	HANDLE hFind;				// FindFirstFile 핸들
	WIN32_FIND_DATA	fd;			// 검색한 화일의 정보
	BOOL bRet = TRUE;
	CString strfile, First_File("*.*");

	CString transient;
	TCHAR temp[100];
	::GetCurrentDirectory(100, temp);
	::SetCurrentDirectory(path);

	hFind = FindFirstFile(First_File, &fd);
	while(hFind != INVALID_HANDLE_VALUE && bRet)
	{			
		strfile = fd.cFileName;

		if( !(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )		// Not directory, which means file.
			::DeleteFile(path+_T("\\")+strfile);
		else if( strfile == "." || strfile == "..")
			;
		else	// it is directory
		{
			transient = path+_T("\\")+strfile;
			Delete_All_in_Folder(transient);
			RemoveDirectory(transient);
		}
		bRet= FindNextFile(hFind, &fd);
	}
	::SetCurrentDirectory(temp);

	::FindClose(hFind);
}

//FTP
void CopyLotData2ServerbyFTP(CString sLotName)
{
	CStdioFile f;
	CString sFileName, str;

	str.Format(_T("%sFullImage"),	MAIN_FOLDER); DeleteFolderFile(str, 30);
	str.Format(_T("%sLog"),			MAIN_FOLDER); DeleteFolderFile(str, 30);
	str.Format(_T("%sGray"),		MAIN_FOLDER); DeleteFolderFile(str, 30);
	str.Format(_T("%sSplice"),		MAIN_FOLDER); DeleteFolderFile(str, 30);
	str.Format(_T("%sFTP"),			MAIN_FOLDER); DeleteFolderFile(str, 0);


	sFileName.Format(_T("%sFTP\\%s.txt"), MAIN_FOLDER, sLotName);
	if(f.Open(sFileName,  CFile::modeCreate|CFile::typeText|CFile::modeWrite))
	{
		str.Format(_T("%sLOG\\%s\\%s.txt,  %s.txt"), MAIN_FOLDER, sLotName, g_Temp.m_sMyComName, g_Temp.m_sMyComName);
		f.WriteString(str);			
		f.Close();
	}
}


#define YEAR 365
#define MONTH 30


//폴더는 모두 그대로 두고 그 폴더 내부 파일만 모두 없앤다
//nEraseDay 이날포함하고 이후는 모두 지운다. 
//nEraseDay=3이면 3일포함해서 그 이전파일 모두 지움
void DeleteFolderFile(CString sDeleteFolder, int nEraseDay)
{
	CFileFind ff;
	CString strFile, deletefile, strYear, strMonth, strDay;
	CTime lastwritetime;
	BOOL  bResult, bWorking=TRUE;
	int ret;
	int i, nYear, nMonth, nDay, nFileYear, nFileMonth, nFileDay, nDiffYear, nDiffMonth, nDiffDay, nMaintain_Day;


	CString str;
	CString filepath;

	filepath.Format(_T("%s\\*.*"), sDeleteFolder);	//지울폴더.

	
	if(ff.FindFile(filepath))
	{		
		bWorking=true;
		while(bWorking)
		{
			bWorking=ff.FindNextFile();
			strFile=ff.GetFileName();  
			bResult=ff.GetLastWriteTime(lastwritetime);
				
			if(strFile=="." || strFile=="..")
				continue;
				
			CTime t = CTime::GetCurrentTime();
				
			nYear=t.GetYear();
			nMonth=t.GetMonth();
			nDay=t.GetDay(); 

			nFileYear=lastwritetime.GetYear();
			nFileMonth=lastwritetime.GetMonth();
			nFileDay=lastwritetime.GetDay();

			nDiffYear=nYear-nFileYear;
			nDiffMonth=nMonth-nFileMonth;
			nDiffDay=nDay-nFileDay;				
				
			nMaintain_Day=nDiffYear*YEAR+nDiffMonth*MONTH+nDiffDay;
				
			if(nMaintain_Day>=nEraseDay)
			{
				deletefile.Format(_T("%s\\%s"), sDeleteFolder,strFile);
				ret=DeleteFile(deletefile);
				if(ret==0)
				{
					RemoveDirectory(deletefile);
				}
			}
			else 
				continue;

			Sleep(1);
			HandleAllMessage();
		}
	}

	ff.Close();
}


BOOL LoadBMP(LPCTSTR lpszFileName, LPBYTE fmImage, int width, int height)
{
	CFile file;
	CFileException fe;
	DWORD dwBitsSize;
	BYTE bHeader[1078];

	// 읽기 모드로 파일 열기
	if(!file.Open(lpszFileName, CFile::modeRead, &fe))
		return FALSE;

	// 파일의 길이를 구함
	dwBitsSize = file.GetLength();
	if(dwBitsSize!=(1078+width*height)) return false;

	// 파일 헤더 읽기
	if(!file.Read(bHeader, 1078)) return FALSE;

	// 파일 읽기
	if (!file.Read(fmImage, width*height) ) return FALSE;
	

	return TRUE;
}
