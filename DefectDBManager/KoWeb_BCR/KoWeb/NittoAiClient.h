#pragma once

typedef struct _NittoAiDefect
{
	int frameNum;
	int totalNg;
	int aiClassCount;
	TCHAR lotNumber[50];
	BYTE image[MAX_DEFECT*BAD_IMG_WIDTH*BAD_IMG_HEIGHT];
}NittoAiDefect, *PNittoAiDefect;

class NittoAiClient
{
private:
	bool m_inited;

	HWND m_pParent;
	HANDLE m_hMap;
	CMutex m_Mutex;
	PNittoAiDefect m_pSharedMemory;

	CXManageSocket  m_Send_NittoAi;
	CXManageSocket  m_Recv_NittoAi;

	bool InitSharedMemory();
	void TermSharedMemory();
	bool InitUdp();
	void TermUdp();

public: 
	NittoAiClient(HWND parent);
	~NittoAiClient();
	bool Inited() { return m_inited; }
	bool Init();
	void Term();
	bool Send(DEFECTDATA* pDefect);
};