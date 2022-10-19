#pragma once
#include <string>

//struct ICallClass;

class CallClassWrapper
{
private:
	HWND m_pParent;
public:
	CallClassWrapper(HWND pParent);
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
	int GetMarkingData(bool isNext);
	int GetMarkingDefectMeter();
	void GetMarkDefectData(CString strBCno, double start, double end);
	void GetMarkAreaDefectData(double start, double end);
	int GetCSV_Type();
};

