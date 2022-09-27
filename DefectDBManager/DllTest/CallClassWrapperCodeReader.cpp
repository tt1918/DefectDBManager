#include "pch.h"
#include "CallClassWrapperCodeReader.h"

#import "CodeReaderDLL.tlb" no_namespace named_guids

CallClassWrapperCodeReader::CallClassWrapperCodeReader(void)
{
	CoInitialize(NULL);
	HRESULT hr = CoCreateInstance(CLSID_CodeReader, NULL, CLSCTX_INPROC_SERVER, IID_ICallCodeReader, reinterpret_cast<void**>(&m_pCallCodeReader));
	if (FAILED(hr))
	{
		CoUninitialize();
	}
}

CallClassWrapperCodeReader::~CallClassWrapperCodeReader(void)
{
	CoUninitialize();
}


bool CallClassWrapperCodeReader::Initialize(bool isUseMil, int sizeX, int sizeY, CString path)
{
	_bstr_t filePath = SysAllocString(path);
	bool ret = m_pCallCodeReader->Initialize(isUseMil, sizeX, sizeY, filePath);
	SysFreeString(filePath);

	return ret;
}

bool CallClassWrapperCodeReader::Terminate()
{
	return false;
}

std::string CallClassWrapperCodeReader::CodeRead(cv::Mat img, int width, int height, bool usePreprocess)
{
	__int64 addr = reinterpret_cast<__int64>(img.data);
	bstr_t code = m_pCallCodeReader->CodeRead(addr, img.cols, img.rows, usePreprocess);
	std::string strCode(code, SysStringByteLen(code));
	SysFreeString(code);

	return strCode;
}