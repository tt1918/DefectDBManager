#include "StdAfx.h"
#include "BcrParamRecv.h"
#include "../userdefine.h"
#include <tchar.h>
#include <stdio.h>

BcrParamRecv g_CommBcrParam;
BcrSearchInfo g_BcrSearchInfo;

extern CXManageSocket  l_Send_Server;
BcrParamRecv::BcrParamRecv()
{
	for (int i = 0; i < BCR_PARAM_RECV; i++)
	{
		m_data[i] = nullptr;
		m_dataSize[i] = 0;
	}

	m_total = 0;
	m_ctrID = -1;
}

BcrParamRecv::~BcrParamRecv()
{
	clearData();
}

void BcrParamRecv::SetType(int type)
{
	m_type = type;
}

void BcrParamRecv::SetData(int ctrID, int index, int total, int size, char* info)
{
	if (index == 0)
	{
		clearData();
		m_total = total;
		m_ctrID = ctrID;
	}

	if (m_ctrID != ctrID)
		return;

	int multibyteSize = size / 2;
		

	if (m_dataSize[index] != multibyteSize)
	{
		deleteData(index);
		createData(index, multibyteSize);
	}
	m_dataSize[index] = multibyteSize;
	memcpy(m_data[index], info, sizeof(char) * size);

	if (checkData() == true)
	{
		makeData();
		l_Send_Server.SendCommand_LocalHost(NM_BCR_SEND_PARAMETER_FILE_DEST_ACK);
	}
}

void BcrParamRecv::clearData()
{
	for (int i = 0; i < BCR_PARAM_RECV; i++)
	{
		if (m_data[i] != nullptr)
		{
			delete[] m_data[i];
			m_data[i] = nullptr;
		}
		m_dataSize[i] = 0;
	}
}

void BcrParamRecv::deleteData(int index)
{
	if (m_data[index] != nullptr)
	{
		delete[] m_data[index];
		m_data[index] = nullptr;
	}
}

void BcrParamRecv::createData(int index, int size)
{
	if (m_data[index] != nullptr)
		deleteData(index);

	m_data[index] = new WCHAR[size];
}

bool BcrParamRecv::checkData()
{
	for (int i = 0; i < m_total; i++)
	{
		if (m_dataSize[i] == 0)
			return false;
	}

	return true;
}

void BcrParamRecv::makeData()
{
	int totalSize = 0;
	WCHAR* data;
	int index = 0;
	for (int i = 0; i < m_total; i++)
	{
		totalSize += m_dataSize[i];
	}

	data = new WCHAR[totalSize];

	for (int i = 0; i < m_total; i++)
	{
		memcpy(data + index, m_data[i], sizeof(WCHAR) * m_dataSize[i]);
		index += m_dataSize[i];
	}

	FILE* fp;
	/*char path[MAX_PATH];
	strcpy(path, CW2A(BCR_DEST_INI_PATH));*/
	fp = _tfopen(BCR_DEST_INI_PATH, L"w,ccs=UNICODE");
	if (fp != nullptr)
	{
		fwrite(data, sizeof(wchar_t), totalSize, fp);
		fclose(fp);
	}

	delete[] data;
}