#include "StdAfx.h"
#include "CallClassWrapper.h"
#include "MarkingDataManager.h"
#include "../KoWebView.h"
CallClassWrapper::CallClassWrapper(HWND pParent)
{
	m_pParent = pParent;
	CoInitialize(NULL);
	HRESULT hr = CoCreateInstance(CLSID_Defects, NULL, CLSCTX_INPROC_SERVER, IID_ICallClass, reinterpret_cast<void**>(&m_pCallClass));

	if (FAILED(hr))
		CoUninitialize();
	else
	{
		
	}
}

CallClassWrapper::~CallClassWrapper(void)
{
	CoUninitialize();
}

void CallClassWrapper::SetParent(HWND pParent)
{
	m_pParent = pParent;
}

void CallClassWrapper::GetDefectsData()
{
	SAFEARRAY* array = m_pCallClass->GetDefect();

	if (array)
	{
		VARTYPE vt;
		SafeArrayGetVartype(array, &vt);
		IRecordInfoPtr splRecordInfo = NULL;
		SafeArrayGetRecordInfo(array, &splRecordInfo);
		GUID guid;
		splRecordInfo->GetGuid(&guid);

		long lLbound = 0;
		long lUbound = 0;

		SafeArrayGetLBound(array, 1, &lLbound);
		SafeArrayGetUBound(array, 1, &lUbound);
		long lDimSize = lUbound - lLbound + 1;

		for (int i = 0; i < lDimSize; i++) {
			long rgIndices[1];
			Defect value;
			memset(&value, 0, sizeof(value));
			rgIndices[0] = i;
			SafeArrayGetElement(array, rgIndices, (void FAR*) & value);
			splRecordInfo->RecordClear((PVOID)&value);
		}

		SafeArrayDestroy(array);
		array = NULL;
		CString strMsg;
		strMsg.Format(_T("%d"), lDimSize);
		MessageBox(NULL, strMsg, _T(""), MB_OK);
	}
}

void CallClassWrapper::ReadDBFile(std::string path)
{
	_bstr_t bs(path.c_str());
	m_pCallClass->ReadDBFile(bs);
	::SysFreeString(bs);
}

void CallClassWrapper::SearchDefectClass(int classID) 
{
	SAFEARRAY* array = m_pCallClass->SearchDefects(classID);
	if (array)
	{
		VARTYPE vt;
		SafeArrayGetVartype(array, &vt);
		IRecordInfoPtr splRecordInfo = NULL;
		SafeArrayGetRecordInfo(array, &splRecordInfo);
		GUID guid;
		splRecordInfo->GetGuid(&guid);

		long lLbound = 0;
		long lUbound = 0;

		SafeArrayGetLBound(array, 1, &lLbound);
		SafeArrayGetUBound(array, 1, &lUbound);
		long lDimSize = lUbound - lLbound + 1;

		for (int i = 0; i < lDimSize; i++) {
			long rgIndices[1];
			Defect value;
			memset(&value, 0, sizeof(value));
			rgIndices[0] = i;
			SafeArrayGetElement(array, rgIndices, (void FAR*) & value);
			splRecordInfo->RecordClear((PVOID)&value);
		}

		SafeArrayDestroy(array);
		array = NULL;
		CString strMsg;
		strMsg.Format(_T("%d"), lDimSize);
		MessageBox(NULL, strMsg, _T(""), MB_OK);
	}
}

void CallClassWrapper::ReadCSVFile(std::string path)
{
	_bstr_t bs(path.c_str());
	m_pCallClass->ReadCSVFile(bs);
	::SysFreeString(bs);
}

void CallClassWrapper::ShowDefectView(bool isNext)
{
	m_pCallClass->ShowViewer(isNext);
}

void CallClassWrapper::AddEndCsvReading(ICsvReadingEvents* pThis)
{
	m_pCallClass->AddEventCsvReading(pThis);
}

void CallClassWrapper::RemoveEndCsvReading(ICsvReadingEvents* pThis)
{
	m_pCallClass->RemoveEventCsvReading(pThis);
}

int CallClassWrapper::GetMarkingData(bool isNext)
{
	SAFEARRAY* array = m_pCallClass->GetMarkingData(isNext);
	MarkingDataManager* pManager = &((CKoWebView*)m_pParent)->m_MarkingDataMgr;
	
	if (pManager->SetBuffer(isNext) == false)
		return -1;

	if (array)
	{
		VARTYPE vt;
		SafeArrayGetVartype(array, &vt);
		IRecordInfoPtr splRecordInfo = NULL;
		SafeArrayGetRecordInfo(array, &splRecordInfo);
		GUID guid;
		splRecordInfo->GetGuid(&guid);

		long lLbound = 0;
		long lUbound = 0;

		SafeArrayGetLBound(array, 1, &lLbound);
		SafeArrayGetUBound(array, 1, &lUbound);
		long lDimSize = lUbound - lLbound + 1;

		for (int i = 0; i < lDimSize; i++) {
			long rgIndices[1];
			MarkingData value;
			MARK_DEFECT markingDefect;
			memset(&value, 0, sizeof(value));
			rgIndices[0] = i;
			SafeArrayGetElement(array, rgIndices, (void FAR*) & value);
			splRecordInfo->RecordClear((PVOID)&value);
			markingDefect.defect_class = value.DefectLine; // 추가 후처리 필요함.
			
			markingDefect.x_pos = value.XPOS_M;
			markingDefect.y_pos = value.YPOS_M;
			markingDefect.offset = value.OFFSET;

			int cLen = ::SysStringLen(value.FAULTID);
			strcpy_s(markingDefect.fltid, cLen, CW2A(value.FAULTID));
			markingDefect.mark = 1;
			
			pManager->AddData(isNext, markingDefect);
		}

		SafeArrayDestroy(array);
		array = NULL;
		CString strMsg;
		strMsg.Format(_T("%d"), lDimSize);
		MessageBox(NULL, strMsg, _T(""), MB_OK);

		return lDimSize;
	}

	return 0;
}

int CallClassWrapper::GetMarkingDefectMeter()
{
	return m_pCallClass->GetMarkingDefectMeter();
}

void CallClassWrapper::GetMarkDefectData(CString strBCno, double start, double end)
{
	BSTR bstr;
	bstr = strBCno.AllocSysString();

	SAFEARRAY* array = m_pCallClass->GetMarkDefectData(bstr, start, end);
	if (array)
	{
		VARTYPE vt;
		SafeArrayGetVartype(array, &vt);
		IRecordInfoPtr splRecordInfo = NULL;
		SafeArrayGetRecordInfo(array, &splRecordInfo);
		GUID guid;
		splRecordInfo->GetGuid(&guid);

		long lLbound = 0;
		long lUbound = 0;

		SafeArrayGetLBound(array, 1, &lLbound);
		SafeArrayGetUBound(array, 1, &lUbound);
		long lDimSize = lUbound - lLbound + 1;

		g_Defect.m_nBcrDefectCount = lDimSize;

		for (int i = 0; i < lDimSize; i++) {
			long rgIndices[1];
			MarkingData value;
			memset(&value, 0, sizeof(value));
			rgIndices[0] = i;
			SafeArrayGetElement(array, rgIndices, (void FAR*) & value);

			g_Defect.m_BcrDefect[i].defect_class = value.DefectLine; // defect Class 재정의 해야함.
			g_Defect.m_BcrDefect[i].x_pos = value.XPOS_M;
			g_Defect.m_BcrDefect[i].y_pos = value.YPOS_M;
			g_Defect.m_BcrDefect[i].offset = value.OFFSET;

			int cLen = ::SysStringLen(value.FAULTID);
			strcpy_s(g_Defect.m_BcrDefect[i].fltid, cLen, CW2A(value.FAULTID));
			g_Defect.m_BcrDefect[i].mark = 1;

			splRecordInfo->RecordClear((PVOID)&value);
		}

		SafeArrayDestroy(array);
		array = NULL;
	}

	::SysFreeString(bstr);
}

void CallClassWrapper::GetMarkAreaDefectData(double start, double end)
{
	CString strLog;
	SAFEARRAY* array = m_pCallClass->GetMarkAreaDefectData(start, end);
	if (array)
	{
		VARTYPE vt;
		SafeArrayGetVartype(array, &vt);
		IRecordInfoPtr splRecordInfo = NULL;
		SafeArrayGetRecordInfo(array, &splRecordInfo);
		GUID guid;
		splRecordInfo->GetGuid(&guid);

		long lLbound = 0;
		long lUbound = 0;

		SafeArrayGetLBound(array, 1, &lLbound);
		SafeArrayGetUBound(array, 1, &lUbound);
		long lDimSize = lUbound - lLbound + 1;

		g_Defect.m_nBcrAreaDefectCount = lDimSize;

		double stY, edY;

		for (int i = 0; i < lDimSize; i++) {
			long rgIndices[1];
			MarkingAreaDefect value;
			memset(&value, 0, sizeof(value));
			rgIndices[0] = i;
			SafeArrayGetElement(array, rgIndices, (void FAR*) & value);

			//현재위치에 AreaDel마킹영역이 존재시
			if (g_Param.m_nBcrOddEven == 1)
			{
				g_Defect.m_BcrAreaDefect[i].x = value.stX + (float)(g_Temp.m_nFoundEdge * g_Param.m_dScaleFactorX + g_Param.m_dCamStartPosX);
				g_Defect.m_BcrAreaDefect[i].width = value.edX + (float)(g_Temp.m_nFoundEdge * g_Param.m_dScaleFactorX + g_Param.m_dCamStartPosX - g_Defect.m_BcrAreaDefect[i].x);
			}
			else if (g_Param.m_nBcrOddEven == 0)
			{
				g_Defect.m_BcrAreaDefect[i].x = (float)(g_Temp.m_nFoundEdge * g_Param.m_dScaleFactorX + g_Param.m_dCamStartPosX) - value.edX;
				g_Defect.m_BcrAreaDefect[i].width = (float)(g_Temp.m_nFoundEdge * g_Param.m_dScaleFactorX + g_Param.m_dCamStartPosX) - value.stX - g_Defect.m_BcrAreaDefect[i].x;
			}
			if (g_Temp.m_nBcrDir == 1)	//증가
			{
				stY = __max(start, value.stY);
				edY = __min(end, value.edY);
				g_Defect.m_BcrAreaDefect[i].y = (float)(stY - start);	//1frame 기준으로 변환
				g_Defect.m_BcrAreaDefect[i].height = (float)(edY - start) - g_Defect.m_BcrAreaDefect[i].y;	//1frame 기준으로 변환
			}
			else	//감소
			{
				stY = __min(start, value.edY);
				edY = __max(end, value.stY);
				g_Defect.m_BcrAreaDefect[i].y = (float)(start - stY);	//1frame 기준으로 변환
				g_Defect.m_BcrAreaDefect[i].height = (float)(start - edY) - g_Defect.m_BcrAreaDefect[i].y;	//1frame 기준으로 변환
			}
			g_Defect.m_BcrAreaDefect[i].defect_class = 1;	// 추후 클래스 정의 해야함.
			g_Defect.m_BcrAreaDefect[i].mark = 1;

			// Area Del Data Save
			strLog.Format(_T("AreaMaring Pos : %.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f"), value.stX, value.edX, stY, edY,
				g_Defect.m_BcrAreaDefect[i].x, g_Defect.m_BcrAreaDefect[i].width, g_Defect.m_BcrAreaDefect[i].y,
				g_Defect.m_BcrAreaDefect[i].height);
			WriteBcrDefectLog(g_Temp.m_strBcrLog, strLog);

			splRecordInfo->RecordClear((PVOID)&value);
		}

		SafeArrayDestroy(array);
		array = NULL;
	}

}

int CallClassWrapper::GetCSV_Type()
{
	return m_pCallClass->GetCSV_Type();
}

void CallClassWrapper::SearchLot(CString strLot, bool isNext, long vendor, bool useES, bool useTG, bool useETC)
{
	_bstr_t bstrLot;
	bstrLot = strLot.AllocSysString();

	m_pCallClass->SearchLot(bstrLot, isNext, vendor, useES, useTG, useETC);

	::SysFreeString(bstrLot);
}

int CallClassWrapper::GetSearchLotResult(bool isNext, CStringArray* arData)
{
	long lDimSize = 0;
	SAFEARRAY* array = m_pCallClass->GetSearchLotResults(isNext);
	if (array)
	{
		VARTYPE vt;
		SafeArrayGetVartype(array, &vt);
		IRecordInfoPtr splRecordInfo = NULL;
		SafeArrayGetRecordInfo(array, &splRecordInfo);
		GUID guid;
		splRecordInfo->GetGuid(&guid);

		long lLbound = 0;
		long lUbound = 0;

		SafeArrayGetLBound(array, 1, &lLbound);
		SafeArrayGetUBound(array, 1, &lUbound);
		lDimSize = lUbound - lLbound + 1;

		for (int i = 0; i < lDimSize; i++) {
			long rgIndices[1];
			LotSearchResult value;
			memset(&value, 0, sizeof(value));
			rgIndices[0] = i;
			SafeArrayGetElement(array, rgIndices, (void FAR*) & value);
			CString data;
			data.Format(_T("%s,$s,$s,$s,$s,%d"), (LPCWSTR)value.LotNo, (LPCWSTR)value.Line, 
				(LPCWSTR)value.DateST, (LPCWSTR)value.TimeST, 
				(LPCWSTR)value.DateED, (LPCWSTR)value.TimeED, 
				value.DefectCnt);
			arData->Add(data);
			splRecordInfo->RecordClear((PVOID)&value);
		}

		SafeArrayDestroy(array);
		array = NULL;
	}

	return lDimSize;
}

void CallClassWrapper::SearchModel(CString strLot)
{
	_bstr_t bstrLot;
	bstrLot = strLot.AllocSysString();
	m_pCallClass->SearchModel(bstrLot);
	::SysFreeString(bstrLot);
}

int CallClassWrapper::GetSearchModelResult(CStringArray* arModel)
{
	long lDimSize = 0;
	SAFEARRAY* array = m_pCallClass->GetSearchModelResult();
	if (array)
	{
		VARTYPE vt;
		SafeArrayGetVartype(array, &vt);
		IRecordInfoPtr splRecordInfo = NULL;
		SafeArrayGetRecordInfo(array, &splRecordInfo);
		GUID guid;
		splRecordInfo->GetGuid(&guid);

		long lLbound = 0;
		long lUbound = 0;

		SafeArrayGetLBound(array, 1, &lLbound);
		SafeArrayGetUBound(array, 1, &lUbound);
		lDimSize = lUbound - lLbound + 1;

		for (int i = 0; i < lDimSize; i++) {
			long rgIndices[1];
			_bstr_t value;
			memset(&value, 0, sizeof(value));
			rgIndices[0] = i;
			SafeArrayGetElement(array, rgIndices, (void FAR*) & value);
			CString model;
			model.Format(_T("%s"), (LPCWSTR)value);
			arModel->Add(model);
			splRecordInfo->RecordClear((PVOID)&value);
		}

		SafeArrayDestroy(array);
		array = NULL;
	}

	return lDimSize;
}