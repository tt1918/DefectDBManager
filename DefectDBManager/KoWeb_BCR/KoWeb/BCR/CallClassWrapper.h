#pragma once
#include <string>

//struct ICallClass;

class CallClassWrapper
{
public:
	CallClassWrapper(void);
	virtual ~CallClassWrapper(void);

private:
	ICallClass* m_pCallClass;

public:
	void GetDefectsData();
	void ReadDBFile(std::string path);
	void SerachDefectClass(int classID);
	void ReadCSVFile(std::string path);
	void ShowDefectView(bool isNext);
	void AddEndCsvReading(ICsvReadingEvents* pThis);
	void RemoveEndCsvReading(ICsvReadingEvents* pThis);
};

