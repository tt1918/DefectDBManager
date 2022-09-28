#include "pch.h"
#include "CsvReadingEvents.h"

HRESULT CsvReadingEvents::QueryInterface(const IID& iid, void** pp)
{
	if (iid == __uuidof(ICsvReadingEvents) ||
		iid == __uuidof(IUnknown))
	{
		*pp = this;
		AddRef();
		return S_OK;
	}
	return E_NOINTERFACE;
}

HRESULT CsvReadingEvents::raw_EventEndCsvReading()
{
	// 皋技瘤 贸府秦具窃.

	return S_OK;
}
