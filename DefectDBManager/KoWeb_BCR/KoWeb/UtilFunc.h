

bool CheckProcess(CString strProcessName);



void SortData(int nCount, double *pData);
void SortData(int nCount, long *pData);
void SortData(int nCount, int *pData);


//폴더 지우는 함수
BOOL DeleteDirectory(LPCTSTR lpDirPath);
BOOL CopyDirectory(LPCTSTR lpDirPath, LPCTSTR lpDirTargetPath);  //lpDirPath폴더 모든 내용을 lpDirTargetPath에 복사
int Delete_Folder(CString path);