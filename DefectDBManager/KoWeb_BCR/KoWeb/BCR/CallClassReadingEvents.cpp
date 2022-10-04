#include "StdAfx.h"
#include "CallClassReadingEvents.h"

HRESULT CallClassReadingEvents::QueryInterface(const IID& iid, void** pp)
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

HRESULT CallClassReadingEvents::raw_EventEndCsvReading(long evtIdx)
{
	// 皋技瘤 贸府秦具窃.

	::SendMessage(this->m_pParent, WM_BCR_COMM, (int)evtIdx, NULL);

	return S_OK;
}
