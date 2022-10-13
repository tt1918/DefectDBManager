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

void CallClassWrapper::SerachDefectClass(int classID) 
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

		g_Defect.m_nMarkDefectCount = lDimSize;

		for (int i = 0; i < lDimSize; i++) {
			long rgIndices[1];
			MarkingData value;
			memset(&value, 0, sizeof(value));
			rgIndices[0] = i;
			SafeArrayGetElement(array, rgIndices, (void FAR*) & value);

			g_Defect.m_MarkDefect[i].defect_class = value.DefectLine; // defect Class 재정의 해야함.
			g_Defect.m_MarkDefect[i].x_pos = value.XPOS_M;
			g_Defect.m_MarkDefect[i].y_pos = value.YPOS_M;
			g_Defect.m_MarkDefect[i].offset = value.OFFSET;

			int cLen = ::SysStringLen(value.FAULTID);
			strcpy_s(g_Defect.m_MarkDefect[i].fltid, cLen, CW2A(value.FAULTID));
			g_Defect.m_MarkDefect[i].mark = 1;

			splRecordInfo->RecordClear((PVOID)&value);
		}

		SafeArrayDestroy(array);
		array = NULL;
	}

	::SysFreeString(bstr);
}

void CallClassWrapper::GetMarkAreaDefectData(double start, double end)
{
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

		g_Defect.m_nAreaDefectCount = lDimSize;

		for (int i = 0; i < lDimSize; i++) {
			long rgIndices[1];
			MarkingAreaDefect value;
			memset(&value, 0, sizeof(value));
			rgIndices[0] = i;
			SafeArrayGetElement(array, rgIndices, (void FAR*) & value);

			//현재위치에 AreaDel마킹영역이 존재시
			if (g_Param.m_nBcrOddEven == 1)
			{
				g_Defect.m_AreaDefect[i].x = value.stX + (float)(g_Temp.m_nFoundEdge * g_Param.m_dScaleFactorX + g_Param.m_dCamStartPosX);
				g_Defect.m_AreaDefect[i].width = value.edX + (float)(g_Temp.m_nFoundEdge * g_Param.m_dScaleFactorX + g_Param.m_dCamStartPosX - g_Defect.m_AreaDefect[i].x);
			}
			else if (g_Param.m_nBcrOddEven == 0)
			{
				g_Defect.m_AreaDefect[i].x = (float)(g_Temp.m_nFoundEdge * g_Param.m_dScaleFactorX + g_Param.m_dCamStartPosX) - value.edX;
				g_Defect.m_AreaDefect[i].width = (float)(g_Temp.m_nFoundEdge * g_Param.m_dScaleFactorX + g_Param.m_dCamStartPosX) - value.stX - g_Defect.m_AreaDefect[i].x;
			}
			if (g_Temp.m_nBcrDir == 1)	//증가
			{
				g_Defect.m_AreaDefect[i].y = (float)(__max(start, value.stY) - start);	//1frame 기준으로 변환
				g_Defect.m_AreaDefect[i].height = (float)(__min(end, value.edY) - start) - g_Defect.m_AreaDefect[i].y;	//1frame 기준으로 변환
			}
			else	//감소
			{
				g_Defect.m_AreaDefect[i].y = (float)(start - __min(start, value.edY));	//1frame 기준으로 변환
				g_Defect.m_AreaDefect[i].height = (float)(start - __max(end, value.stY)) - g_Defect.m_AreaDefect[i].y;	//1frame 기준으로 변환
			}
			g_Defect.m_AreaDefect[i].defect_class = 1;	// 추후 클래스 정의 해야함.
			g_Defect.m_AreaDefect[i].mark = 1;
			splRecordInfo->RecordClear((PVOID)&value);
		}

		SafeArrayDestroy(array);
		array = NULL;
	}

}