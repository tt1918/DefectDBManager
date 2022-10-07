

int SavePNG(CString filename, LPBYTE fmS);	
int SaveBMP(CString filename, LPBYTE fmS);
int SaveJPG(CString filename, LPBYTE fmS);

int SavePNGComp(CString filename, LPBYTE fmS, int width, int height);	

double Clock_set();
int HandleAllMessage();

CString GetSWVersion();
double HDDSpace();			//HDD잔여 공간을 %로 리턴
CString GetPCName();

int GetIpAddress();			//IP 끝자리를 return함											
int GetClientOptic(CString sComName);	//OPTIC 읽는 함수
void Delete_All_in_Folder(CString path);

void CopyLotData2ServerbyFTP(CString sLotName);						//한 LOT끝난 .txt를 서버 특정 폴더로 복사하도록 틀정파일을 만듦(FTP가 알아서 복사)
void DeleteFolderFile(CString sDeleteFolder, int nEraseDay);


BOOL LoadBMP(LPCTSTR lpszFileName, LPBYTE fmImage, int width, int height); //NG 이미지 Load하는 함수
	