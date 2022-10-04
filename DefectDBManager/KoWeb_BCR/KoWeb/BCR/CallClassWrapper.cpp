#include "StdAfx.h"
#include "CallClassWrapper.h"

CallClassWrapper::CallClassWrapper(void) 
{
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
			memset(&value, 0, sizeof(value));
			rgIndices[0] = i;
			SafeArrayGetElement(array, rgIndices, (void FAR*) & value);
			splRecordInfo->RecordClear((PVOID)&value);

			int a;
			a = 0;

		}

		SafeArrayDestroy(array);
		array = NULL;
		CString strMsg;
		strMsg.Format(_T("%d"), lDimSize);
		MessageBox(NULL, strMsg, _T(""), MB_OK);

		return lDimSize;
	}
}