#include "stdafx.h"
#include "NittoAiClient.h"

NittoAiClient::NittoAiClient(HWND parent)
: m_hMap(NULL), m_pParent(parent), m_pSharedMemory(NULL), m_Mutex(FALSE, TEXT("IPC_NITTO_MUTEX"))
, m_inited(false)
{

}
NittoAiClient::~NittoAiClient()
{
	Term();
}
bool NittoAiClient::Init()
{
	return m_inited = InitSharedMemory() && InitUdp();
}

void NittoAiClient::Term()
{
	TermSharedMemory();
	TermUdp();
}

bool NittoAiClient::InitSharedMemory()
{
	m_hMap = ::CreateFileMapping(INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(NittoAiDefect),
		_T("IPC_NITTO_SHARED_MEMORY"));

	if (::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		m_hMap = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE,
			_T("IPC_NITTO_SHARED_MEMORY"));
	}

	if (m_hMap == NULL)
	{
		AfxMessageBox(_T("ERROR: Failed to create(open) file mapping object!"));
		return false;
	}

	m_pSharedMemory = (PNittoAiDefect)::MapViewOfFile(m_hMap,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(NittoAiDefect));
	if (m_pSharedMemory == NULL)
	{
		AfxMessageBox(_T("ERROR: Failed to get shared memory!"));
		return false;
	}

	return true;
}
void NittoAiClient::TermSharedMemory()
{
	if (m_pSharedMemory != NULL)	::UnmapViewOfFile(m_pSharedMemory);
	if (m_hMap != NULL)				::CloseHandle(m_hMap);
}

bool NittoAiClient::InitUdp()
{
#ifdef USE_NITTO_AI
	bool ret = m_Send_NittoAi.Create(m_pParent, NITTO_AI_PORT, _T("127.0.0.1"));	///Nitto AI 보내는것
	bool ret1 = m_Recv_NittoAi.Create(m_pParent, NITTO_AI_PORT + 1);	///Nitto AI 받는것
	if (ret1) m_Recv_NittoAi.WatchComm();
	if (ret && ret1)
		return true;
#endif
	return false;
}

void NittoAiClient::TermUdp()
{
#ifdef USE_NITTO_AI
	m_Recv_NittoAi.StopComm();
	m_Send_NittoAi.Close();
	m_Recv_NittoAi.Close();
#endif
}

bool NittoAiClient::Send(DEFECTDATA* pDefect)
{
	if (m_inited)
	{
		if (m_Mutex.Lock(1000))
		{
			//shared memory에 데이터를 쓴다
			m_pSharedMemory->frameNum = pDefect->m_nFrameNum;
			m_pSharedMemory->totalNg = pDefect->m_nDefectCount;
			_stprintf(m_pSharedMemory->lotNumber, g_Temp.m_slotName);
			memcpy(m_pSharedMemory->image, pDefect->m_pImage, BAD_IMG_HEIGHT * BAD_IMG_WIDTH * pDefect->m_nDefectCount);
			m_Mutex.Unlock();

			//패킷으로 데이터를 읽으라고 한다
			m_Send_NittoAi.SendCommand_LocalHost(NM_NITTO_AI_IMAGE);

			return true;
		}
	}

	return false;
}