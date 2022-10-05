
class CallClassReadingEvents :public ICsvReadingEvents
{
private:
	HWND m_pParent;
public:
	CallClassReadingEvents(HWND pParent)
	{
		m_pParent = pParent;
	}
	~CallClassReadingEvents()
	{
		m_pParent = nullptr;
	}

public:
	virtual HRESULT __stdcall QueryInterface(const IID&, void**);
	virtual HRESULT __stdcall raw_EventEndCsvReading(long evtIdx);
	ULONG __stdcall AddRef(void) { return 1; }
	ULONG __stdcall Release(void) { return 1; }

};
