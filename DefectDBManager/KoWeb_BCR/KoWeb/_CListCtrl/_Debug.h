
#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/*
<debug mode일 때 memory 할당> 
1. new 키워드
	각 모듈(*.cpp) 앞에 다음 코드가 추가되어 있다.
	#ifdef _DEBUG
	#define new DEBUG_NEW
	#endif

	<afx.h>에 아래 두 줄이 정의되어 있다.
	#define THIS_FILE          __FILE__
	#define DEBUG_NEW new(THIS_FILE, __LINE__)

	결국, new는
	#define new new(__FILE__, __LINE__)이다.

2. malloc(szie) 함수
	::malloc_dbg(size, _NORMAL_BLOCK, __FILE__, __LINE__ )로 풀어져서 수행된다.

<memory 누수 확인 방법>
	!!! DEBUG mode에서 아래의 방법을 이용했을 때
	memory 누수를 야기시키는 source code가 존재한다면, 
	Deubggin이 종료된 직후 '출력'창에
	'Detected memory leaks!' message를 시작으로
	누수를 야기시키는 source code 위치가 나열된다.

 The following macros set and clear, respectively, given bits
 of the C runtime library debug flag, as specified by a bitmask.
 1. 현 시점에서 free가 아직 되지 않은 할당 된 모든 것.
		_CrtMemDumpAllObjectsSince( NULL );
 2. 구간 내 할당된 상태
		char *p;
		_CrtMemState s1, s2, s3;
		_CrtMemCheckpoint( &s1 );
		p = malloc(10);
		_CrtMemCheckpoint( &s2 );
		if ( _CrtMemDifference( &s3, &s1, &s2 ) ) _CrtMemDumpStatistics( &s3 );
 3. free 이후 사용했을 때 손상 확인
		_CrtCheckMemory( );
		or 디버그 힙 사용법 :
		SET_CRT_DEBUG_FIELD(_CRTDBG_CHECK_CRT_DF);
 4. _malloc_dbg( 40, _CLIENT_BLOCK, __FILE__, __LINE__ ); 일때
		_CLIENT_BLOCK 을 사용하면 dbgheap.c에서 assert된다.
		_NORMAL_BLOCK을 사용할 것.
 5. free 안한 거 check
		_CrtDumpMemoryLeaks();
		or 디버그 힙 사용법 :
		SET_CRT_DEBUG_FIELD(_CRTDBG_LEAK_CHECK_DF);
 6.	_CRTDBG_MAP_ALLOC가 정의되면
		누수된 메모리를 할당한 파일을 찾을 수 있습니다
		현재, 이 파일(_Debug.h) 맨 앞에 정의되어 있다.
		그리고, 그 아래 include 된 파일의 앞뒤 순서가 바뀌어선 안된다고
		MSDN에 적혀 있다.
 7. 프로그램이 항상 동일한 위치에서 종료되는 경우에는
		종료 위치에서 _CrtDumpMemoryLeaks를 쉽게 호출할 수 있습니다.
		프로그램이 여러 위치에서 종료될 수 있는 경우에는
		종료할 수 있는 모든 위치에서 _CrtDumpMemoryLeaks를 호출하는 대신
		프로그램 시작 부분에 다음 호출을 포함할 수 있습니다.

		디버그 힙 사용법 : SET_CRT_DEBUG_FIELD(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		이때, _CRTDB_CHECK_CRT_DF을 사용하면
		stdenvp.c에서 leak가 발견될 수 있다. 이는 잘못된 것이 아니어서 무시해도 되지만 _CRTDB_CHECK_CRT_DF를 사용하지 않도록 한다.
		참고 : ms-help://MS.VSCC.v80/MS.MSDN.v80/MS.VisualStudio.v80.ko/dv_vsdebugnative/html/e25e1e38-8b09-4b5e-9910-1f1649054b3b.htm
		참고 : http://social.msdn.microsoft.com/Forums/en-US/vcgeneral/thread/58cfabb5-3695-4cb4-98d7-a2721c6e35e4/#448c88fc-7463-49c8-8c20-3382a71d8d40
*/
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef   _DEBUG
#define  SET_CRT_DEBUG_FIELD(a) \
            ::_CrtSetDbgFlag((a) | ::_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#define  CLEAR_CRT_DEBUG_FIELD(a) \
            ::_CrtSetDbgFlag(~(a) & ::_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else
#define  SET_CRT_DEBUG_FIELD(a)   ((void) 0)
#define  CLEAR_CRT_DEBUG_FIELD(a) ((void) 0)
#endif

/*
<ASSERT, VERIFY 대체 macro>
	아래 새로운 macro를 작성하게 된 목적은
	본래의 verify macro정의에서 (void)부분을 제외하여 if문에서도 사용할 수 있게 만들고
	식이 거짓일 때 뿐만 아니라 참일 때도 verify할 수 있도록 하기 위해서이다.

	!! MFC에서 일반적인 coding 방법은 ASSERT(expr);를 하고 if(expr)를 사용하는데
	이는 expr를 중복으로 써야 되기 때문에 줄 수가 길어지고 혹시나 expr이 서로 다르기라도 하면 낭패다.

	이 macro 중에 'M'이 붙어 있는 것은 release mode에서도 message box를 나타낼 수 있다.
	따라서, message box 출력을 debug, release mode에 상관없이 일관성있게 하나로 통일시킬 수 있고,
	program명, file명, function명, line번호, 조건식, GetLastError() message를 함께 나타낼 수 있다.

	debug mode에서는 굳이 사용자 message를 출력할 필요가 없기 때문에
	release mode에서도 작동하는 macro만 사용자 message 인수를 가지게 한다.

	권장 방법: framework, private classes and functions들과는 달리
	library 수준에서 release mode일 때 오류 message box가 난무하는 것은 좋지 않다고 판단된다.
	그래서 runtime library, api 수준의 함수에서는 꼭 필요한 경우를 제외하고 되도록이면 'M'를 뗀 macro를 사용하자.

	주의!!!
	program 종료 시점에서 assert, verify 를 사용하면
	window가 이미 파괴되었기 때문에
	message 창을 띄우지 못하고 memory leak을 발생시킨다.
	실행중일 때 assert, verify가 필요한 상황이면서 종료할 때도 수행이 된다면
	필히, assert, verify 이전에 조건을 미리 확인을 하는 판단문이 필요하다.


	작동 흐름
	_CrtRlsReportW에서 abort button을 누르면 exit을 수행한다.
	retry button을 누르면 계속해서 다음 code, 즉 _CrtDbgBreak를 수행한 후 진리값은 0이 되고
	ignore button을 누르면 _CrtDbgBreak을 수행하지 않고 진리값은 1이 된다.
	_CrtDbgReportW와 동일하게 동작하도록 작성하였을 뿐만아니라, 더 많은 정보를 출력하도록 작성하였다.
	(_CrtDbgBreak(), 0)이라는 문법은 _CrtDbgBreak이 void 함수이기 때문에 return 값을 임의로 정해주는 문법이다.
	_CrtDbgBreak에서 retry button은 F10(프로시저 단위 실행)에, ignore button은 F5(디버깅 시작)에 해당된다.
	얼핏 생각하면, _CrtDbgBreak는 필요가 없어 보인다.
	게다가 debug mode이고, '디버깅 않고 시작'으로 실행했을 때, retry button을 누르면 종료해버린다.
	그러나, break 된 위치를 자동적으로 찾아가기 위해서는 _CrtDbgBreak가 필요하다.
	release mode일 때는 _CrtDbgBreak가 (void)0이기 때문에 신경쓸 필요가 없다. 그리고 ::(global)을 붙이지 않아야 한다. ::(void(0))이 되기 때문에.
*/

/* winsock2.h와 _Debug.h를 include할 때 주의 사항
	 This prevents the Winsock.h(for Windows Sockets 1.1) from being included by the Windows.h header.
	#ifdef WIN32_LEAN_AND_MEAN
	#include "_Debug.h"
	#else
	#define WIN32_LEAN_AND_MEAN
	#include "_Debug.h"
	#undef WIN32_LEAN_AND_MEAN
	#endif
	또는
	winsock2.h 다음에 _Debug.h를 include 시킨다.
*/
#include <windows.h>
#include <strsafe.h> // MSDN 검색어 : Security Considerations: Windows User Interface, Using the Strsafe.h Functions

// *** Retrieves the calling thread's last-error message
// return된 buffer를 사용 후 ::LocalFree(...)해야 함
static LPTSTR _GetLastErrorMsg(LPCTSTR lpszFunction, bool bShowMsgBox = false)
{

	DWORD eNum = ::GetLastError();

	LPTSTR lpszSysMsg;
	::FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER
							| FORMAT_MESSAGE_FROM_SYSTEM
							| FORMAT_MESSAGE_IGNORE_INSERTS,
							NULL,
							eNum,
							MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
							(LPTSTR)&lpszSysMsg,
							0, NULL);

	//// Trim the end of the line and terminate it with a null
	//LPTSTR p = lpszSysMsg;
	//while( ( *p > 31 ) || ( *p == 9 ) )	++p;
	//do { *p-- = 0; } while( ( p >= lpszSysMsg ) && ( ( *p == '.' ) || ( *p < 33 ) ) );

    size_t cchLength;
	::StringCchLength(lpszSysMsg, 0xffff, &cchLength);
	LPTSTR lpszLastErrorMsg = (LPTSTR)::LocalAlloc(LMEM_ZEROINIT, (cchLength + 80/*여유분*/) * sizeof(TCHAR));
	::StringCchPrintf((LPTSTR)lpszLastErrorMsg, ::LocalSize(lpszLastErrorMsg) / sizeof(TCHAR),
						_T("GetLastError: code(%d), %s"), eNum, lpszSysMsg);
	::LocalFree(lpszSysMsg);

	if (bShowMsgBox)
	{
		::MessageBox(::GetDesktopWindow()/*NULL*/, lpszLastErrorMsg, lpszFunction, MB_ICONSTOP);
		//::MessageBox(::GetActiveWindow()/*NULL*/, pErrorMsgAllocBuf, lpszFunction, MB_ICONSTOP);
		::LocalFree(lpszLastErrorMsg);
		lpszLastErrorMsg = NULL;
	}

	return lpszLastErrorMsg;
}

// *** report할 문장 전체를 만든다.
// StringCchCat(_tcscat_s), StringCchPrintf(stprintf_s ), StringCchVPrintf(_vstprintf_s <= CString은 crt)
static  int __cdecl _CrtRlsReport(	LPCTSTR _FileName,
										LPCTSTR _FunctionName,
										int _LineNumber,
										LPCTSTR _Expression,
										LPCTSTR _Format,
										...)
{
	#define _REPORT_BUFFERSIZE	3072 // 출력할 전체 문장. 충분히 크게 확보
	#define _TEMP_BUFFERSIZE	2048 // 임시 buffer. 충분히 크게 확보
	LPTSTR lpszReport = new TCHAR[_REPORT_BUFFERSIZE];
	LPTSTR lpszT = new TCHAR[_TEMP_BUFFERSIZE];
	lpszReport[0] = 0;

	// 첫번째 줄 : Debug, Release
#ifdef _DEBUG
	::StringCchCat(lpszReport, _REPORT_BUFFERSIZE, _T("Debug Assertion Failed\r\n\r\nProgram: "));
#else
	::StringCchCat(lpszReport, _REPORT_BUFFERSIZE, _T("Release Verification Failed\r\n\r\nProgram: "));
#endif

	// 두번째 줄 : Program 이름
	::GetModuleFileName(NULL/*the current process*/, lpszT, _TEMP_BUFFERSIZE);
	::StringCchCat(lpszReport, _REPORT_BUFFERSIZE, lpszT);

	// 세번째 줄 : File 이름
	::StringCchPrintf(lpszT, _TEMP_BUFFERSIZE, _T("\r\n\r\nFile: %s"), _FileName);
	::StringCchCat(lpszReport, _REPORT_BUFFERSIZE, lpszT);

	// 네번째 줄 : Function 이름
	::StringCchPrintf(lpszT, _TEMP_BUFFERSIZE, _T("\r\n\r\nFunction: %s"), _FunctionName);
	::StringCchCat(lpszReport, _REPORT_BUFFERSIZE, lpszT);

	// 다섯번째 줄 : Line 번호
	::StringCchPrintf(lpszT, _TEMP_BUFFERSIZE, _T("\r\n\r\nLine: %d"), _LineNumber);
	::StringCchCat(lpszReport, _REPORT_BUFFERSIZE, lpszT);

	// 여섯번째 줄 : Expression 내용
	if (_Expression != NULL)
	{
		::StringCchPrintf(lpszT, _TEMP_BUFFERSIZE, _T("\r\n\r\nExpression: %s"), _Expression);
		::StringCchCat(lpszReport, _REPORT_BUFFERSIZE, lpszT);
	}

	// 일곱번째 줄 : GetLastError message
	::StringCchCat(lpszReport, _REPORT_BUFFERSIZE, _T("\r\n\r\n"));
	LPTSTR lpszLastErrorMsg = ::_GetLastErrorMsg(NULL, false);
	::StringCchCat(lpszReport, _REPORT_BUFFERSIZE, lpszLastErrorMsg);
	::LocalFree(lpszLastErrorMsg);

	// 여덟번째 줄 : User's message
	if (_Format != NULL)
	{
		::StringCchCat(lpszReport, _REPORT_BUFFERSIZE, _T("\r\nUserMessage: "));
		va_list argList;
		va_start(argList, _Format);
		::StringCchVPrintf(lpszT, _TEMP_BUFFERSIZE, _Format, argList);
		va_end(argList);
		::StringCchCat(lpszReport, _REPORT_BUFFERSIZE, lpszT);
	}

#ifdef _DEBUG //GetActiveWindow ?
	::StringCchCat(lpszReport, _REPORT_BUFFERSIZE, _T("\r\n\r\nFor information on how your program can cause an assertion failure, see the Visual C++ documentation on asserts."));
	::StringCchCat(lpszReport, _REPORT_BUFFERSIZE, _T("\r\n\r\n(Press Retry to debug the application)"));
	int nRet = ::MessageBox(::GetDesktopWindow(), lpszReport, _T("Microsoft Visual C++ Debug Library"), MB_ICONSTOP|MB_ABORTRETRYIGNORE|MB_DEFBUTTON1/*|MB_SETFOREGROUND*/|MB_TOPMOST);
#else
	::StringCchCat(lpszReport, _REPORT_BUFFERSIZE, _T("\r\n\r\nFor information on how your program can cause a verification failure, contact the developer, please."));
	::StringCchCat(lpszReport, _REPORT_BUFFERSIZE, _T("\r\n\r\n(Press Retry to continue the application)"));
	int nRet = ::MessageBox(::GetDesktopWindow(), lpszReport, _T("Microsoft Visual C++ Release Library"), MB_ICONSTOP|MB_ABORTRETRYIGNORE|MB_DEFBUTTON1/*|MB_SETFOREGROUND*/|MB_TOPMOST);
#endif

	delete [] lpszReport;
	delete [] lpszT;

	if (nRet == IDABORT) ::exit(0);
	if (nRet == IDRETRY) return 1;
	return 0; // ignore
}


// *** #문자열 형식을 매크로에서 사용할 때 # 연산자는 문자열 양쪽에 ""(큰따옴표)를 붙이는 연산자이다.
// #define 지시문이나 enum keyword로 정의된 식별자(indentifier)를
// 문자열로 바꾸고 싶을 때 사용하면 쓸모가 있다.
// 아래 매크로를 사용하지 않고 code 내부에 직접 풀어서 #identifier를 사용하면
// compile error가 발생한다. 
#define _ID2STR(id) #id
#define _ID2WIDESTR(id) L ## #id
#define _2WIDESTR(string) L ## string // __CRT_WIDE(string)


// *** ASSERT, VERIFY 대체 macro
#define __FDEBUG_RELEASE(expr, user_msg, ...) \
			((expr) \
			||  (1 != _CrtRlsReport(_T(__FILE__), _T(__FUNCTION__), __LINE__, _T(#expr), user_msg, ## __VA_ARGS__)) \
			|| (_CrtDbgBreak(), 0))
#define __TDEBUG_RELEASE(expr, user_msg, ...) \
			((!expr) \
			||  (1 != _CrtRlsReport(_T(__FILE__), _T(__FUNCTION__), __LINE__, _T(#expr), user_msg, ## __VA_ARGS__)) \
			|| (_CrtDbgBreak(), 0))

#ifdef _DEBUG
#define __ASSERT(e)			(__FDEBUG_RELEASE((e), NULL)) // Debug mode에서는 user_msg가 필요없을 것이다.
#define __FVERIFY(e)			(__FDEBUG_RELEASE((e), NULL))//, ## __VA_ARGS__))
#define __TVERIFY(e)			(!__TDEBUG_RELEASE((e), NULL))//, ## __VA_ARGS__))
#else
#define __ASSERT(e)			(/*(void)*/0)
#define __FVERIFY(e)			(e)	// no message box
#define __TVERIFY(e)			(e)	// no message box
#endif
#define __FVERIFYM(e, user_msg, ...)		(__FDEBUG_RELEASE((e), user_msg, ## __VA_ARGS__)) // Release mode에서도 message box를 출력한다. user_msg가 필요할 수도 있다.
#define __TVERIFYM(e, user_msg, ...)		(!__TDEBUG_RELEASE((e), user_msg, ## __VA_ARGS__))


/* 
 <if, while 대체 macro>
	 if, while 문의 조건식에서 !, !>, !<, !=, >, >=, <, <=, == 이외 다른 대입문이나 연산문을 사용한 경우를 assert한다.
	 if, while 문의 조건식에 대입문 = 을 사용하는 것(지양하는 coding)이 문법적으로 틀리지는 않으나 assert된다.
	 if, while 문의 조건식에 연산문 <<=, >>= 을 사용하는 것(지양하는 coding)이 문법적으로 틀리지는 않으나 assert된다.
	 debug mode에서 빠른 처리 속도를 요구하는 경우에는 주석 처리해야 한다.
		1) 빠른 처리 속도가 필요한 module이면 Debug.h가 최초로 include 되는 곳을 찾아 그 앞에 _SPEED_NEEDED를 define해야 한다.
		2) 빠른 처리 속도가 필요한 header이면 그 header가 최초의 Debug.h보다 앞에 include되도록 해야 하는데
			이를 알리기 위해 그 header가 Debug.h보다 뒤에 있는지 판단하는 compile 지시문을 그 header 맨 앞줄에 작성해 두어야 한다.
			그리고 _SPEED_NEEDED도 그 header 맨 앞줄에 define해야 한다.
			#if defined(_DEBUG) && defined(_ALLOW_KEYWORD_MACROS)
			#error This header file has to be processed before first _Debug.h. In other words, _SPEED_NEEDED has to be defined before first _Debug.h.
			#endif
			#define _SPEED_NEEDED
*/

/*
#if defined(_DEBUG) && !defined(_SPEED_NEEDED) && !defined(_DISABLE_KEYWORD_MACROS)
#define _ALLOW_KEYWORD_MACROS
static bool _CorrExpr(LPCTSTR _Expr)
{
	//::SetLastError(ERROR_NOT_SUPPORTED);
	//L"!", L"!>", L"!<", L"!=", L">", L">=", L"<",  L"<=", L"==", L""};
	size_t cchL = ::_tcslen(_Expr);
	::StringCchLength(_Expr, 0x004f, (size_t*)&cchL);

	TCHAR prev = _T('\0');
	for (size_t i = 0; i < cchL; i++)
	{
		switch (_Expr[i])
		{
			case _T('!') :
				if (prev != _T('\0')) return false;
				prev = _T('!');
				break;
			case _T('>') :
				if (prev != _T('\0') && prev != _T('!')) return false;
				if (prev == _T('\0')) prev = _T('>');
				else prev = _T('\0');
				break;
			case _T('<') :
				if (prev != _T('\0') && prev != _T('!')) return false;
				if (prev == _T('\0')) prev = _T('<');
				else prev = _T('\0');
				break;
			case _T('=') :
				if (prev != _T('\0') && prev != _T('!') && prev != _T('>') && prev != _T('<') && prev != _T('=')) return false;
				if (prev == _T('\0')) prev = _T('=');
				else prev = _T('\0');
				break;
			default:
				if (prev == _T('=')) return false;
				prev = _T('\0');
				break;
		}
	}

	return true;
}

#define if(e)		if (	::_CorrExpr(_T(#e)) ? (!!(e)) : \
							(1 != ::_CrtRlsReport(_T(__FILE__), _T(__FUNCTION__), __LINE__, _T(#e), _T("Invalid Criteria"))) \
							|| (::_CrtDbgBreak(), 0))
#define while(e)	while (::_CorrExpr(_T(#e)) ? (!!(e)) : \
							(1 != ::_CrtRlsReport(_T(__FILE__), _T(__FUNCTION__), __LINE__, _T(#e), _T("Invalid Criteria"))) \
							|| (::_CrtDbgBreak(), 0))

#endif
							
*/