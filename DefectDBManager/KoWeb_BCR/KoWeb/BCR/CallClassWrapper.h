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
	CRITICAL_SECTION cs;

public:
	void SetParent(HWND pParent);
	void GetDefectsData();
	void ReadDBFile(std::string path);
	void SearchDefectClass(int classID);
	void ReadCSVFile(std::string path);
	void ShowDefectView(bool isNext);
	void AddEndCsvReading(ICsvReadingEvents* pThis);
	void RemoveEndCsvReading(ICsvReadingEvents* pThis);
	int GetMarkingData(bool isNext);
	int GetMarkingDefectMeter();
	void GetMarkDefectData(CString strBCno, double start, double end);
	bool UseAreaDelCheck();
	void GetMarkAreaDefectData(double start, double end);
	int GetCSV_Type();
	bool GetUseES(bool isNext);
	bool GetUseTG(bool isNext);
	bool GetUseETC(bool isNext);
	void SearchLot(CString strLot, bool isNext, long vendor, bool useES, bool useTG, bool useETC);
	int GetSearchDBResult();
	int GetSearchLotResult(bool isNext, CStringArray* arData);
	void SearchModel(CString strLot);
	int GetSearchModelResult(CStringArray *arModel);
	int GetLoadedBCNO_Data(bool isNext, CStringArray* arBCNO);
};

