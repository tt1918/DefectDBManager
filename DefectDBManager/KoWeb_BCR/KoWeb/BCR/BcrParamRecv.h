#pragma once
#include <WTypes.h>


#define BCR_PARAM_RECV 20
class BcrParamRecv
{
private:
	WCHAR* m_data[BCR_PARAM_RECV];
	int	  m_dataSize[BCR_PARAM_RECV];
	
	int   m_total;
	int	  m_type;

	int	  m_ctrID;

public:
	BcrParamRecv();
	~BcrParamRecv();

	void SetType(int type);
	void SetData(int ctrID, int index, int total, int size, char* info);

private:
	void clearData();
	void deleteData(int index);
	void createData(int index, int size);
	bool checkData();
	void makeData();
};

extern BcrParamRecv g_CommBcrParam;

class BcrSearchInfo
{
public :
	char strLot[100];
	int vendor;
	bool isNext;
	bool useES;
	bool useTG;
	bool useETC;
	bool useBMark;

public:
	BcrSearchInfo()
	{
		memset(strLot, 0x00, sizeof(char) * 100);
		vendor = -1;
		isNext = false;
		useES = false;
		useTG = false;
		useETC = false;
		useBMark = false;
	}
};

extern BcrSearchInfo g_BcrSearchInfo;