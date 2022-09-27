#ifndef __AXT_AXL_H__
#define __AXT_AXL_H__

#include "AXHS.h"
#include "AXA.h"
#include "AXD.h"
#include "AXM.h"

#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

//========== 라이브러리 초기화 =================================================================================

	// 라이브러리 초기화
	DWORD	__stdcall AxlOpen(long lIrqNo);
	// 라이브러리 사용을 종료
	BOOL	__stdcall AxlClose();
	// 라이브러리가 초기화 되어 있는 지 확인
	BOOL	__stdcall AxlIsOpened();

//========== 라이브러리 및 베이스 보드 정보 =================================================================================

	// 등록된 베이스 보드의 개수 확인
	DWORD	__stdcall AxlGetBoardCount(long *lpBoardCount);
	// 라이브러리 버전 확인
	DWORD	__stdcall AxlGetLibVersion(char *szVersion);

//========== 로그 레벨 =================================================================================

	// EzSpy에 출력할 메시지 레벨 설정
	DWORD	__stdcall AxlSetLogLevel(DWORD uLevel);
	// EzSpy에 출력할 메시지 레벨 확인
	DWORD	__stdcall AxlGetLogLevel(DWORD *upLevel);

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__AXT_AXL_H__
