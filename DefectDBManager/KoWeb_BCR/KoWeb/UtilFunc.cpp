#include "stdafx.h"
#include "Tlhelp32.h"

bool GetProcessModule(DWORD dwPID,CString sProcessName);


#ifdef USE_X64






#else
bool CheckProcess(CString strProcessName)
{

    HANDLE         hProcessSnap = NULL; 
    BOOL           bRet      = FALSE; 
    PROCESSENTRY32 pe32      = {0}; 

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 

     if (hProcessSnap == (HANDLE)-1) 
        return false;

 
    pe32.dwSize = sizeof(PROCESSENTRY32); 

     //프로세스가 메모리상에 있으면 첫번째 프로세스를 얻는다
    if (Process32First(hProcessSnap, &pe32)) 
    { 
        BOOL          bCurrent = FALSE; 
        MODULEENTRY32 me32       = {0}; 
        do 
        { 
           bCurrent = GetProcessModule(pe32.th32ProcessID,strProcessName);
           if(bCurrent) 
           { 
              HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID); 
              if(hProcess)
              {
                 CloseHandle(hProcess); 
				 return true;
              }
           } 
        } 
       while (Process32Next(hProcessSnap, &pe32)); //다음 프로세스의 정보를 구하여 있으면 루프를 돈다.

    } 
    CloseHandle (hProcessSnap); 
    return false;

}
bool GetProcessModule(DWORD dwPID,CString sProcessName)
{ 

    HANDLE        hModuleSnap = NULL; 
    MODULEENTRY32 me32        = {0}; 

    hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID); 
    if (hModuleSnap == (HANDLE)-1) 
        return (FALSE); 

 

    me32.dwSize = sizeof(MODULEENTRY32); 

     //해당 프로세스의 모듈리스트를 루프로 돌려서 프로세스이름과 동일하면 
    //true를 리턴한다.

    if(Module32First(hModuleSnap, &me32)) 
    { 
       do 
       { 
          if(me32.szModule == sProcessName)
          { 
             CloseHandle (hModuleSnap); 
             return true;
          } 
       } 
       while(Module32Next(hModuleSnap, &me32)); 
    } 
    CloseHandle (hModuleSnap); 
    return false;

}
#endif


void SortData(int nCount, double *pData)
{
	int i,t;
	double dTmp=0;

	for(i=0; i<nCount; i++	)
		for(t=i; t>0; t--)
		{
			if(pData[t]<pData[t-1])
			{
				dTmp = pData[t];
				pData[t] = pData[t-1];
				pData[t-1] = dTmp;
			}
		}	
}
void SortData(int nCount, long *pData)
{
	int i,t;
	long dTmp=0;

	for(i=0; i<nCount; i++	)
		for(t=i; t>0; t--)
		{
			if(pData[t]<pData[t-1])
			{
				dTmp = pData[t];
				pData[t] = pData[t-1];
				pData[t-1] = dTmp;
			}
		}	
}
void SortData(int nCount, int *pData)
{
	int i,t;
	long dTmp=0;

	for(i=0; i<nCount; i++	)
		for(t=i; t>0; t--)
		{
			if(pData[t]<pData[t-1])
			{
				dTmp = pData[t];
				pData[t] = pData[t-1];
				pData[t-1] = dTmp;
			}
		}	
}



//한폴더를 통째로 지우는 함수 lpDirPath="C:\\COSS\\LOT\\*.*" 형태로 주어져야 한다.
BOOL DeleteDirectory(LPCTSTR lpDirPath)
{
	BOOL bRval=FALSE;
	int nRval=0;
	CString szNextDirPath=_T("");
	CString szRoot=_T("");
	CFileFind find;

	//폴더가 있는지
	bRval=find.FindFile(lpDirPath);
	if(!bRval) return FALSE;

	while(bRval)
	{
		bRval=find.FindNextFileW();
		if(find.IsDots()) continue;
		if(find.IsDirectory())
		{
			szNextDirPath.Format(_T("%s\\*.*"), find.GetFilePath());
			DeleteDirectory(szNextDirPath);
		}
		else
		{
			::DeleteFile(find.GetFilePath());
		}
	}
	szRoot=find.GetRoot();
	find.Close();
	nRval=RemoveDirectory(szRoot);
	return nRval;
}

BOOL CopyDirectory(LPCTSTR lpDirPath, LPCTSTR lpDirTargetPath)
{
	BOOL bRval=TRUE;
	int nRval=0;
	CString sSrcFileName;
	CString sTargetFileName;
	CFileFind find;

	sSrcFileName.Format(_T("%s%s\\*.*"), MODEL_PATH, lpDirPath);
	//폴더가 있는지
	bRval=find.FindFile(sSrcFileName);
	if(!bRval) return FALSE;

	while(bRval)
	{
		bRval=find.FindNextFileW();
		if(find.IsDots()) continue;
		if(find.IsDirectory())
		{
			
		}
		else
		{
			sSrcFileName=find.GetFilePath();
			sTargetFileName.Format(_T("%s%s\\%s"), MODEL_PATH, lpDirTargetPath, find.GetFileName());
			CopyFile(sSrcFileName, sTargetFileName, false);
		}
	}
	
	find.Close();
	return nRval;
}

//한폴더를 통째로 지우는 함수 path="C:\\COSS\\LOT" 형태로 주어져야 한다.
int Delete_Folder(CString path)
{
	SHFILEOPSTRUCT FileOp={0};
	TCHAR szTemp[200];

	_tcscpy(szTemp, path);
	szTemp[path.GetLength()+1]=NULL;

	FileOp.hwnd=NULL;
	FileOp.wFunc=FO_DELETE;
	FileOp.pTo=NULL;
	FileOp.fFlags=FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SIMPLEPROGRESS;
	FileOp.fAnyOperationsAborted=false;
	FileOp.hNameMappings=NULL;
	FileOp.lpszProgressTitle=NULL;
	FileOp.pFrom=szTemp;
	return SHFileOperation(&FileOp);
}
