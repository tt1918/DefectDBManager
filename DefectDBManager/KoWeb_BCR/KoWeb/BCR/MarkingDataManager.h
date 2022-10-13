#pragma once
#include <vector>
#include "../UDP/Packet.h"
#include <afxmt.h>
class MarkingDataManager
{
	struct _tagMarkingManager
	{
		std::vector<MARK_DEFECT>* data;	// ≈∏∞Ÿ µ•¿Ã≈Õ
		int idx;						// ≈∏∞Ÿ ¿Œµ¶Ω∫
	};

private:
	CCriticalSection m_cs;
	std::vector<MARK_DEFECT> m_data[3];

	_tagMarkingManager m_markingNow;
	_tagMarkingManager m_markingNext;
	
public:
	MarkingDataManager();
	~MarkingDataManager();
	void ResetAll();
	void ResetReserve();
	bool SetBuffer(bool isNext);
	bool SwapNext2Now();
	
	bool IsValid(bool isNext);
	void AddData(bool isNext, MARK_DEFECT data);
	int GetData(double startY, double endY, MARK_DEFECT* data);
	int GetData(CString strBcno, double startY, double endY, MARK_DEFECT* data);
private:
	int checkReserve();
	void resetData(bool isNext);
};

