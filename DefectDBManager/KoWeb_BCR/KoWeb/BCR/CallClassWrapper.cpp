#include "StdAfx.h"
#include "CallClassWrapper.h"
#include "MarkingDataManager.h"
#include "../KoWebView.h"



CallClassWrapper::CallClassWrapper(HWND pParent)
{
	InitializeCriticalSection(&cs);
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
	DeleteCriticalSection(&cs);
	CoUninitialize();
}

void CallClassWrapper::SetParent(HWND pParent)
{
	m_pParent = pParent;
}

void CallClassWrapper::GetDefectsData()
{
	try
	{
		EnterCriticalSection(&cs);
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
		LeaveCriticalSection(&cs);
	}
	catch (...)
	{
		LeaveCriticalSection(&cs);
	}
}

void CallClassWrapper::ReadDBFile(std::string path)
{
	try
	{
		EnterCriticalSection(&cs);
		_bstr_t bs(path.c_str());
		m_pCallClass->ReadDBFile(bs);
		::SysFreeString(bs);
		LeaveCriticalSection(&cs);
	}
	catch (...)
	{
		LeaveCriticalSection(&cs);
	}
}

void CallClassWrapper::SearchDefectClass(int classID)
{
	try
	{
		EnterCriticalSection(&cs);
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
		LeaveCriticalSection(&cs);
	}
	catch (...)
	{
		LeaveCriticalSection(&cs);
	}
}

void CallClassWrapper::ReadCSVFile(std::string path)
{
	try
	{
		EnterCriticalSection(&cs);
		_bstr_t bs(path.c_str());
		m_pCallClass->ReadCSVFile(bs);
		::SysFreeString(bs);
		LeaveCriticalSection(&cs);
	}
	catch (...)
	{
		LeaveCriticalSection(&cs);
	}
}

void CallClassWrapper::ShowDefectView(bool isNext)
{
	try
	{
		EnterCriticalSection(&cs);
		m_pCallClass->ShowViewer(isNext);
		LeaveCriticalSection(&cs);
	}
	catch (...)
	{
		LeaveCriticalSection(&cs);
	}
}

void CallClassWrapper::AddEndCsvReading(ICsvReadingEvents* pThis)
{
	try
	{
		EnterCriticalSection(&cs);
		m_pCallClass->AddEventCsvReading(pThis);
		LeaveCriticalSection(&cs);
	}
	catch (...)
	{
		LeaveCriticalSection(&cs);
	}
}

void CallClassWrapper::RemoveEndCsvReading(ICsvReadingEvents* pThis)
{
	try
	{
		EnterCriticalSection(&cs);
		m_pCallClass->RemoveEventCsvReading(pThis);
		LeaveCriticalSection(&cs);
	}
	catch (...)
	{
		LeaveCriticalSection(&cs);
	}
}

int CallClassWrapper::GetMarkingData(bool isNext)
{
	int size = 0;
	try
	{
		EnterCriticalSection(&cs);

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

			int type = g_Param.m_nBcrCsvType;

			for (int i = 0; i < lDimSize; i++) {
				long rgIndices[1];
				MarkingData value;
				MARK_DEFECT markingDefect;
				memset(&value, 0, sizeof(value));
				rgIndices[0] = i;
				SafeArrayGetElement(array, rgIndices, (void FAR*) & value);
				splRecordInfo->RecordClear((PVOID)&value);
				markingDefect.x_pos = value.XPOS_M;
				markingDefect.y_pos = value.YPOS_M;
				markingDefect.offset = value.OFFSET;
				if (value.UseCSVResult == false)
				{
					if (type == eCSV_TYPE_NITTO)
					{
						if (value.DefectLine == 9) // 점착
							markingDefect.defect_class = value.CAM_NO * 100000000 + 12;//
						else  //if (value.DefectLine == 8) // 그외
							markingDefect.defect_class = value.CAM_NO * 100000000 + 13;//
					}
					else if (type == eCSV_TYPE_NITTO_RTS || type == eCSV_TYPE_NITTO_RK || type == eCSV_TYPE_KORENO_RK_IJP)
					{
						if (value.DefectLine == 9) //점착 
							markingDefect.defect_class = value.CAM_NO * 100000000 + 12;//
						else if (value.DefectLine == 8) // 연신 - 기타
							markingDefect.defect_class = value.CAM_NO * 100000000 + 11;//
						else //value.DefectLine = 7 그외
							markingDefect.defect_class = value.CAM_NO * 100000000 + 13;//
					}
					else // 나중에 정의해야 함
					{
						if (value.DefectLine == 9) //점착 
							markingDefect.defect_class = value.CAM_NO * 100000000 + 12;//
						else if (value.DefectLine == 8) // 연신 - 기타
							markingDefect.defect_class = value.CAM_NO * 100000000 + 11;//
						else //value.DefectLine = 7 그외
							markingDefect.defect_class = value.CAM_NO * 100000000 + 13;//
					}
				}
				else
				{
					if (value.DefectLine == 9) //점착 
						markingDefect.defect_class = value.CAM_NO * 100000000 + 12;
					else if (value.DefectLine == 8) // 연신 - 기타
						markingDefect.defect_class = value.CAM_NO * 100000000 + 11;//
					else //value.DefectLine = 7 그외
						markingDefect.defect_class = value.CAM_NO * 100000000 + 13;//
				}

				sprintf(g_Defect.m_BcrDefect[i].fltid, CW2A(value.FAULTID));
				markingDefect.mark = 1;

				pManager->AddData(isNext, markingDefect);
			}

			SafeArrayDestroy(array);
			array = NULL;
			CString strMsg;
			strMsg.Format(_T("%d"), lDimSize);
			MessageBox(NULL, strMsg, _T(""), MB_OK);

			size = lDimSize;
		}
		LeaveCriticalSection(&cs);
	}
	catch (...)
	{
		LeaveCriticalSection(&cs);
	}

	return size;
}

int CallClassWrapper::GetMarkingDefectMeter()
{
	int size = 0;
	try
	{
		EnterCriticalSection(&cs);
		size = m_pCallClass->GetMarkingDefectMeter();
		LeaveCriticalSection(&cs);
	}
	catch (...)
	{
		LeaveCriticalSection(&cs);
	}
	return size;
}

void CallClassWrapper::GetMarkDefectData(CString strBCno, double start, double end)
{
	try
	{
		EnterCriticalSection(&cs);
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

			int type = g_Param.m_nBcrCsvType;

			for (int i = 0; i < lDimSize; i++) {
				long rgIndices[1];
				MarkingData value;
				memset(&value, 0, sizeof(value));
				rgIndices[0] = i;
				SafeArrayGetElement(array, rgIndices, (void FAR*) & value);

				g_Defect.m_BcrDefect[i].x_pos = value.XPOS_M;
				g_Defect.m_BcrDefect[i].y_pos = value.YPOS_M;
				g_Defect.m_BcrDefect[i].offset = value.OFFSET;

				if (value.UseCSVResult == false)
				{
					if (type == eCSV_TYPE_NITTO)
					{
						if (value.DefectLine < 21) // Defect Edit에 포함되지 않는 Defect Line
						{
							if (value.DefectLine == 9) // 점착
								g_Defect.m_BcrDefect[i].defect_class = value.CAM_NO * 1000000 + 12;//
							else  //if (value.DefectLine == 8) // 그외
								g_Defect.m_BcrDefect[i].defect_class = value.CAM_NO * 1000000 + 13;//
						}
						else // Edit Defect에 의해서 만들어진 Defect Line
						{
							g_Defect.m_BcrDefect[i].defect_class = (value.DefectLine - 20) * 1000000 + 16;
						}
					}
					else if (type == eCSV_TYPE_NITTO_RTS || type == eCSV_TYPE_NITTO_RK || type == eCSV_TYPE_KORENO_RK_IJP)
					{
						if (value.DefectLine < 21) // Defect Edit에 포함되지 않는 Defect Line
						{
							if (value.DefectLine == 9) //점착 
								g_Defect.m_BcrDefect[i].defect_class = value.CAM_NO * 1000000 + 12;//
							else if (value.DefectLine == 8) // 연신 - 기타
								g_Defect.m_BcrDefect[i].defect_class = value.CAM_NO * 1000000 + 11;//
							else //value.DefectLine = 7 그외
								g_Defect.m_BcrDefect[i].defect_class = value.CAM_NO * 1000000 + 13;//
						}
						else
						{
							// 1000*CamNum+21+defect classNUM;
							// 나중에 어떻게 처리할지 확인 필요
							g_Defect.m_BcrDefect[i].defect_class = (value.DefectLine % 1000 - 20) * 1000000 + 16;
						}
					}
					else // 나중에 정의해야 함
					{
						if (value.DefectLine < 21) // Defect Edit에 포함되지 않는 Defect Line
						{
							if (value.DefectLine == 9) //점착 
								g_Defect.m_BcrDefect[i].defect_class = value.CAM_NO * 1000000 + 12;//
							else if (value.DefectLine == 8) // 연신 - 기타
								g_Defect.m_BcrDefect[i].defect_class = value.CAM_NO * 1000000 + 11;//
							else //value.DefectLine = 7 그외
								g_Defect.m_BcrDefect[i].defect_class = value.CAM_NO * 1000000 + 13;//
						}
						else
						{
							g_Defect.m_BcrDefect[i].defect_class = (value.DefectLine - 20) * 1000000 + 16;
						}
					}
				}
				else
				{
					if (value.DefectLine < 21) // Defect Edit에 포함되지 않는 Defect Line
					{
						if (value.DefectLine == 9) //점착 
							g_Defect.m_BcrDefect[i].defect_class = value.CAM_NO * 1000000 + 12;
						else if (value.DefectLine == 8) // 연신 - 기타
							g_Defect.m_BcrDefect[i].defect_class = value.CAM_NO * 1000000 + 11;//
						else //value.DefectLine = 7 그외
							g_Defect.m_BcrDefect[i].defect_class = value.CAM_NO * 1000000 + 13;//
					}
					else // Edit Defect에 의해서 만들어진 Defect Line
					{
						g_Defect.m_BcrDefect[i].defect_class = (value.DefectLine - 20) * 1000000 + 16;
					}
				}

				sprintf(g_Defect.m_BcrDefect[i].fltid, CW2A(value.FAULTID));
				g_Defect.m_BcrDefect[i].mark = 1;

				splRecordInfo->RecordClear((PVOID)&value);
			}

			SafeArrayDestroy(array);
			array = NULL;
		}

		::SysFreeString(bstr);
		LeaveCriticalSection(&cs);
	}
	catch (...)
	{
		LeaveCriticalSection(&cs);
	}
}

void CallClassWrapper::GetMarkAreaDefectData(double start, double end)
{
	try
	{
		EnterCriticalSection(&cs);
		CString strLog;
		long areaDelCnt = 0;
		SAFEARRAY* array = m_pCallClass->GetMarkAreaDelDefectData(start, end, &areaDelCnt);
		g_Defect.m_nBcrAreaDefectCount = areaDelCnt;
		if (areaDelCnt == 0)
		{
			LeaveCriticalSection(&cs);
			return;
		}


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
				g_Defect.m_BcrAreaDefect[i].index = value.idx;
				g_Defect.m_BcrAreaDefect[i].index2 = g_AreaDelSplice[value.idx];

				// Area Del Data Save
				strLog.Format(_T("AreaMaring Pos[%d_%d] : %.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f"), value.idx, g_AreaDelSplice[value.idx],
					value.stX, value.edX, stY, edY, g_Defect.m_BcrAreaDefect[i].x, g_Defect.m_BcrAreaDefect[i].width, g_Defect.m_BcrAreaDefect[i].y,
					g_Defect.m_BcrAreaDefect[i].height);
				WriteBcrDefectLog(g_Temp.m_strBcrLog, strLog);

				g_AreaDelSplice[value.idx]++;
				splRecordInfo->RecordClear((PVOID)&value);
			}

			SafeArrayDestroy(array);
			array = NULL;
		}
		LeaveCriticalSection(&cs);
	}
	catch (...)
	{
		LeaveCriticalSection(&cs);
	}
}

int CallClassWrapper::GetCSV_Type()
{

	int type = -1;
	try
	{
		EnterCriticalSection(&cs);
		type = m_pCallClass->GetCSV_Type();
		LeaveCriticalSection(&cs);
	}
	catch (...)
	{
		LeaveCriticalSection(&cs);
	}
	return type;
}

bool CallClassWrapper::GetUseES(bool isNext)
{
	bool isRes = false;
	try
	{
		EnterCriticalSection(&cs);
		VARIANT value;
		VariantInit(&value);

		value.boolVal = m_pCallClass->IsEsUse(isNext);
		if (value.boolVal == VARIANT_TRUE)
			isRes = true;
		VariantClear(&value);
		LeaveCriticalSection(&cs);
	}
	catch (...)
	{
		LeaveCriticalSection(&cs);
		return isRes;
	}
	return isRes;
}

bool CallClassWrapper::GetUseTG(bool isNext)
{
	bool isRes = false;
	try
	{
		EnterCriticalSection(&cs);
		VARIANT value;
		VariantInit(&value);

		value.boolVal = m_pCallClass->IsTgUse(isNext);
		if (value.boolVal == VARIANT_TRUE)
			isRes = true;
		VariantClear(&value);
		LeaveCriticalSection(&cs);
	}
	catch (...)
	{
		LeaveCriticalSection(&cs);
		return isRes;
	}
	return isRes;
}

bool CallClassWrapper::GetUseETC(bool isNext)
{
	bool isRes = false;
	try
	{
		EnterCriticalSection(&cs);
		VARIANT value;
		VariantInit(&value);

		value.boolVal = m_pCallClass->IsEtcUse(isNext);
		if (value.boolVal == VARIANT_TRUE)
			isRes = true;
		VariantClear(&value);
		LeaveCriticalSection(&cs);
	}
	catch (...)
	{
		LeaveCriticalSection(&cs);
		return isRes;
	}
	return isRes;
}

bool CallClassWrapper::UseAreaDelCheck()
{
	VARIANT_BOOL res = m_pCallClass->UseAreaDelCheck();
	bool isRes = false;
	if (res == VARIANT_TRUE)
		isRes = true;
	return true;
}

void CallClassWrapper::SearchLot(CString strLot, bool isNext, long vendor, bool useES, bool useTG, bool useETC)
{
	try
	{
		EnterCriticalSection(&cs);
		_bstr_t bstrLot;
		bstrLot = strLot.AllocSysString();

		m_pCallClass->SearchLot(bstrLot, isNext, vendor, useES, useTG, useETC);

		::SysFreeString(bstrLot);
		LeaveCriticalSection(&cs);
	}
	catch (...)
	{
		LeaveCriticalSection(&cs);
	}
}

int CallClassWrapper::GetSearchDBResult()
{
	long result = 0;
	try
	{
		result = m_pCallClass->GetSearchResut();
	}
	catch (...)
	{
		return 0;
	}
	return result;
}

int CallClassWrapper::GetSearchLotResult(bool isNext, CStringArray* arData)
{
	long lDimSize = 0;
	try
	{
		EnterCriticalSection(&cs);
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
				data.Format(_T("%s,$s,$s,$s,$s,$s,%d"), (LPCWSTR)value.LotNo, (LPCWSTR)value.Line,
					(LPCWSTR)value.DateST, (LPCWSTR)value.TimeST,
					(LPCWSTR)value.DateED, (LPCWSTR)value.TimeED,
					value.DefectCnt);
				arData->Add(data);
				splRecordInfo->RecordClear((PVOID)&value);
			}

			SafeArrayDestroy(array);
			array = NULL;
		}
		LeaveCriticalSection(&cs);
	}
	catch (...)
	{
		LeaveCriticalSection(&cs);
	}
	return lDimSize;
}

void CallClassWrapper::SearchModel(CString strLot)
{
	try
	{
		EnterCriticalSection(&cs);
		_bstr_t bstrLot;
		bstrLot = strLot.AllocSysString();
		m_pCallClass->SearchModel(bstrLot);
		::SysFreeString(bstrLot);
		LeaveCriticalSection(&cs);
	}
	catch (...)
	{
		LeaveCriticalSection(&cs);
	}
}

int CallClassWrapper::GetSearchModelResult(CStringArray* arModel)
{
	long lDimSize = 0;
	try
	{
		EnterCriticalSection(&cs);

		int modelCnt = m_pCallClass->GetSearchModelCount();
		if (modelCnt == 0)
		{
			LeaveCriticalSection(&cs);
			return 0;
		}


		SAFEARRAY* array = m_pCallClass->GetSearchModelResult();
		if (array)
		{
			VARTYPE vt;
			SafeArrayGetVartype(array, &vt);

			long lLbound = 0;
			long lUbound = 0;

			SafeArrayGetLBound(array, 1, &lLbound);
			SafeArrayGetUBound(array, 1, &lUbound);
			lDimSize = lUbound - lLbound + 1;

			for (int i = 0; i < lDimSize; i++) {
				long rgIndices[1];
				VARIANT value;
				VariantInit(&value);
				rgIndices[0] = i;
				SafeArrayGetElement(array, rgIndices, &value.bstrVal);
				CString model;
				model.Format(_T("%s"), value.bstrVal);
				arModel->Add(model);
				VariantClear(&value);
			}

			SafeArrayDestroy(array);
			array = NULL;
		}
		LeaveCriticalSection(&cs);
	}
	catch (...)
	{
		LeaveCriticalSection(&cs);
	}
	return lDimSize;
}

int CallClassWrapper::GetLoadedBCNO_Data(bool isNext, CStringArray* arBCNO)
{
	long size = 0;
	try
	{
		EnterCriticalSection(&cs);
		SAFEARRAY* array = m_pCallClass->GetLoadedBCNO(isNext, &size);

		if (size == 0)
		{
			LeaveCriticalSection(&cs);
			return size;
		}

		if (array)
		{
			long lDimSize = 0;
			VARTYPE vt;
			SafeArrayGetVartype(array, &vt);

			long lLbound = 0;
			long lUbound = 0;

			SafeArrayGetLBound(array, 1, &lLbound);
			SafeArrayGetUBound(array, 1, &lUbound);
			lDimSize = lUbound - lLbound + 1;

			for (int i = 0; i < lDimSize; i++) {
				long rgIndices[1];
				VARIANT value;
				VariantInit(&value);
				rgIndices[0] = i;
				SafeArrayGetElement(array, rgIndices, &value.bstrVal);
				CString model;
				model.Format(_T("%s"), value.bstrVal);
				arBCNO->Add(model);
				VariantClear(&value);
			}

			SafeArrayDestroy(array);
			array = NULL;
		}
		LeaveCriticalSection(&cs);
	}
	catch (...)
	{
		LeaveCriticalSection(&cs);
		return size;
	}

	return size;
}