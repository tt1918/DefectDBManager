#include "StdAfx.h"
#include "MarkingDataManager.h"

MarkingDataManager::MarkingDataManager()
{
	ResetAll();
}

MarkingDataManager::~MarkingDataManager()
{
	ResetAll();
}

void MarkingDataManager::ResetAll()
{
	m_markingNow.data = nullptr;
	m_markingNow.idx = -1;

	m_markingNext.data = nullptr;
	m_markingNext.idx = -1;

	m_data[0].clear();
	m_data[1].clear();
	m_data[2].clear();
}

/// <summary>
/// Now/Next에서 사용하지 않는 데이터는 초기화 함
/// </summary>
void MarkingDataManager::ResetReserve()
{
	bool isUse[3];
	memset(isUse, 0x00, sizeof(bool) * 3);
	
	if (m_markingNow.idx != -1)
		isUse[m_markingNow.idx] = true;
	if(m_markingNext.idx != -1)
		isUse[m_markingNext.idx] = true;

	for (int i = 0; i < 3; i++)
	{
		if (isUse[i] == true) continue;
		m_data[i].clear();
	}
}

bool MarkingDataManager::SetBuffer(bool isNext)
{
	int idx = checkReserve();
	if (idx == -1)
		return false;

	m_cs.Lock();
	// 기존 데이터는 초기화한다. 
	resetData(isNext);
	if (isNext == false) //  현재랏
	{
		m_markingNow.data = &m_data[idx];
		m_markingNow.idx = idx;
	}
	else // 예약랏
	{
		m_markingNext.data = &m_data[idx];
		m_markingNext.idx = idx;
	}
	m_cs.Unlock();
}

bool MarkingDataManager::SwapNext2Now()
{
	if (m_markingNext.idx == -1 || m_markingNext.data == nullptr)
		return false;
	m_cs.Lock();
	// 데이터 주소 복사해준다.
	m_markingNow.data = m_markingNext.data;
	m_markingNow.idx = m_markingNext.idx;
	resetData(true);
	m_cs.Unlock();
}

bool MarkingDataManager::IsValid(bool isNext)
{
	bool isValid = true;
	if (isNext == false) //  현재랏
	{
		if (m_markingNow.data == nullptr || m_markingNow.idx == -1)
			isValid = false;
	}
	else // 예약랏
	{
		if (m_markingNext.data == nullptr || m_markingNext.idx == -1)
			isValid = false;
	}

	return isValid;
}

int MarkingDataManager::GetData(double startY, double endY, DEFECT* data)
{
	if (IsValid(false) == false)
		return 0;

	int size = m_markingNow.data->size();
	std::vector<DEFECT>::iterator iter;
	
	int cnt = 0;
	for (iter = m_markingNow.data->begin(); iter != m_markingNow.data->end(); iter++)
	{
		if (iter->y_pos >= startY && iter->y_pos <= endY)
		{
			memcpy(&data[cnt], &iter, sizeof(DEFECT));
			cnt++;
		}
	}

	return cnt;
}

void MarkingDataManager::resetData(bool isNext)
{
	if (isNext == false) //  현재랏
	{
		m_markingNow.data = nullptr;
		m_markingNow.idx = -1;
	}
	else // 예약랏
	{
		m_markingNext.data = nullptr;
		m_markingNext.idx = -1;
	}
}

int MarkingDataManager::checkReserve()
{
	bool isUse[3];
	memset(isUse, 0x00, sizeof(bool) * 3);

	if (m_markingNow.idx != -1)
		isUse[m_markingNow.idx] = true;
	if (m_markingNext.idx != -1)
		isUse[m_markingNext.idx] = true;

	for (int i = 0; i < 3; i++)
	{
		if (isUse[i] == false)
			return i;
	}

	return -1;
}