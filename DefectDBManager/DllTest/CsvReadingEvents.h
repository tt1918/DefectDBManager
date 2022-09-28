
class CsvReadingEvents :public ICsvReadingEvents
{
public:
	HWND m_pParent;
public:
	CsvReadingEvents(HWND pParent)
	{
		m_pParent = pParent;
	}
	~CsvReadingEvents()
	{
		m_pParent = nullptr;
	}

public:
	virtual HRESULT __stdcall QueryInterface(const IID&, void**);
	virtual HRESULT __stdcall raw_EventEndCsvReading();
	ULONG __stdcall AddRef(void) { return 1; }
	ULONG __stdcall Release(void) { return 1; }

};
