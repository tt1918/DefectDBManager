#ifndef __AXT_AXD_H__
#define __AXT_AXD_H__

#include "AXHS.h"

#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

//========== 보드 및 모듈 정보 
	// DIO 모듈이 있는지 확인
	DWORD	__stdcall AxdInfoIsDIOModule(DWORD *upStatus);
	// DIO 모듈 No 확인
	DWORD	__stdcall AxdInfoGetModuleNo(long lBoardNo, long lModulePos, long *lpModuleNo);
	// DIO 입출력 모듈의 개수 확인
	DWORD	__stdcall AxdInfoGetModuleCount(long *lpModuleCount);
	// 지정한 모듈의 입력 접점 개수 확인
	DWORD	__stdcall AxdInfoGetInputCount(long lModuleNo, long *lpCount);
	// 지정한 모듈의 출력 접점 개수 확인
	DWORD	__stdcall AxdInfoGetOutputCount(long lModuleNo, long *lpCount);
	// 지정한 모듈 번호로 베이스 보드 번호, 모듈 위치, 모듈 ID 확인
	DWORD	__stdcall AxdInfoGetModule(long lModuleNo, long *lpBoardNo, long *lpModulePos, DWORD *upModuleID);
//==Log
	// 지정한 모듈의 EzSpy에 로그 출력 여부 설정
	DWORD	__stdcall AxdLogSetModule(long lModuleNo, DWORD uUse);
	// 지정한 모듈의 EzSpy에 로그 출력 여부 확인
	DWORD	__stdcall AxdLogGetModule(long lModuleNo, DWORD *upUse);

//========== 인터럽트 설정 확인 
	// 지정한 모듈에 인터럽트 메시지를 받아오기 위하여 윈도우 메시지, 콜백 함수 또는 이벤트 방식을 사용
	DWORD	__stdcall AxdiInterruptSetModule(long lModuleNo, HWND hWnd, DWORD uMessage, AXT_INTERRUPT_PROC pProc, HANDLE *pEvent);
	// 지정한 모듈의 인터럽트 사용 유무 설정
	DWORD	__stdcall AxdiInterruptSetModuleEnable(long lModuleNo, DWORD uUse);
	// 지정한 모듈의 인터럽트 사용 유무 확인
	DWORD	__stdcall AxdiInterruptGetModuleEnable(long lModuleNo, DWORD *upUse);
	// 인터럽트 발생 위치 확인
	DWORD	__stdcall AxdiInterruptRead(long *lpModuleNo, DWORD *upFlag);
//========== 인터럽트 플래그 읽기 
	// 지정한 입력 접점 모듈, Interrupt Flag Register의 Offset 위치에서 bit 단위로 인터럽트 발생 상태 값을 읽음
	DWORD	__stdcall AxdiInterruptFlagReadBit(long lModuleNo, long lOffset, DWORD *upValue);
	// 지정한 입력 접점 모듈, Interrupt Flag Register의 Offset 위치에서 byte 단위로 인터럽트 발생 상태 값을 읽음
	DWORD	__stdcall AxdiInterruptFlagReadByte(long lModuleNo, long lOffset, DWORD *upValue);
	// 지정한 입력 접점 모듈, Interrupt Flag Register의 Offset 위치에서 word 단위로 인터럽트 발생 상태 값을 읽음
	DWORD	__stdcall AxdiInterruptFlagReadWord(long lModuleNo, long lOffset, DWORD *upValue);
	// 지정한 입력 접점 모듈, Interrupt Flag Register의 Offset 위치에서 double word 단위로 인터럽트 발생 상태 값을 읽음
	DWORD	__stdcall AxdiInterruptFlagReadDword(long lModuleNo, long lOffset, DWORD *upValue);
	// 전체 입력 접점 모듈, Interrupt Flag Register의 Offset 위치에서 bit 단위로 인터럽트 발생 상태 값을 읽음
	DWORD	__stdcall AxdiInterruptFlagRead(long lOffset, DWORD *upValue);
//========== 인터럽트 상승 / 하강 에지 설정 확인 
	// 지정한 입력 접점 모듈, Interrupt Rising / Falling Edge register의 Offset 위치에서 bit 단위로 상승 또는 하강 에지 값을 설정
	DWORD	__stdcall AxdiInterruptEdgeSetBit(long lModuleNo, long lOffset, DWORD uMode, DWORD uValue);
	// 지정한 입력 접점 모듈, Interrupt Rising / Falling Edge register의 Offset 위치에서 byte 단위로 상승 또는 하강 에지 값을 설정
	DWORD	__stdcall AxdiInterruptEdgeSetByte(long lModuleNo, long lOffset, DWORD uMode, DWORD uValue);
	// 지정한 입력 접점 모듈, Interrupt Rising / Falling Edge register의 Offset 위치에서 word 단위로 상승 또는 하강 에지 값을 설정
	DWORD	__stdcall AxdiInterruptEdgeSetWord(long lModuleNo, long lOffset, DWORD uMode, DWORD uValue);
	// 지정한 입력 접점 모듈, Interrupt Rising / Falling Edge register의 Offset 위치에서 double word 단위로 상승 또는 하강 에지 값을 설정
	DWORD	__stdcall AxdiInterruptEdgeSetDword(long lModuleNo, long lOffset, DWORD uMode, DWORD uValue);
	// 지정한 입력 접점 모듈, Interrupt Rising / Falling Edge register의 Offset 위치에서 bit 단위로 상승 또는 하강 에지 값을 확인
	DWORD	__stdcall AxdiInterruptEdgeGetBit(long lModuleNo, long lOffset, DWORD uMode, DWORD *upValue);
	// 지정한 입력 접점 모듈, Interrupt Rising / Falling Edge register의 Offset 위치에서 byte 단위로 상승 또는 하강 에지 값을 확인
	DWORD	__stdcall AxdiInterruptEdgeGetByte(long lModuleNo, long lOffset, DWORD uMode, DWORD *upValue);
	// 지정한 입력 접점 모듈, Interrupt Rising / Falling Edge register의 Offset 위치에서 word 단위로 상승 또는 하강 에지 값을 확인
	DWORD	__stdcall AxdiInterruptEdgeGetWord(long lModuleNo, long lOffset, DWORD uMode, DWORD *upValue);
	// 지정한 입력 접점 모듈, Interrupt Rising / Falling Edge register의 Offset 위치에서 double word 단위로 상승 또는 하강 에지 값을 확인
	DWORD	__stdcall AxdiInterruptEdgeGetDword(long lModuleNo, long lOffset, DWORD uMode, DWORD *upValue);
	// 전체 입력 접점 모듈, Interrupt Rising / Falling Edge register의 Offset 위치에서 bit 단위로 상승 또는 하강 에지 값을 설정
	DWORD	__stdcall AxdiInterruptEdgeSet(long lOffset, DWORD uMode, DWORD uValue);
	// 전체 입력 접점 모듈, Interrupt Rising / Falling Edge register의 Offset 위정에서 bit 단위로 상승 또는 하강 에지 값을 확인
	DWORD	__stdcall AxdiInterruptEdgeGet(long lOffset, DWORD uMode, DWORD *upValue);

//========== 입출력 레벨 설정 확인 
//==입력 레벨 설정 인
	// 지정한 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 설정
	DWORD	__stdcall AxdiLevelSetInportBit(long lModuleNo, long lOffset, DWORD uLevel);
	// 지정한 입력 접점 모듈의 Offset 위치에서 byte 단위로 데이터 레벨을 설정
	DWORD	__stdcall AxdiLevelSetInportByte(long lModuleNo, long lOffset, DWORD uLevel);
	// 지정한 입력 접점 모듈의 Offset 위치에서 word 단위로 데이터 레벨을 설정
	DWORD	__stdcall AxdiLevelSetInportWord(long lModuleNo, long lOffset, DWORD uLevel);
	// 지정한 입력 접점 모듈의 Offset 위치에서 double word 단위로 데이터 레벨을 설정
	DWORD	__stdcall AxdiLevelSetInportDword(long lModuleNo, long lOffset, DWORD uLevel);
	// 지정한 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 확인
	DWORD	__stdcall AxdiLevelGetInportBit(long lModuleNo, long lOffset, DWORD *upLevel);
	// 지정한 입력 접점 모듈의 Offset 위치에서 byte 단위로 데이터 레벨을 확인
	DWORD	__stdcall AxdiLevelGetInportByte(long lModuleNo, long lOffset, DWORD *upLevel);
	// 지정한 입력 접점 모듈의 Offset 위치에서 word 단위로 데이터 레벨을 확인
	DWORD	__stdcall AxdiLevelGetInportWord(long lModuleNo, long lOffset, DWORD *upLevel);
	// 지정한 입력 접점 모듈의 Offset 위치에서 double word 단위로 데이터 레벨을 확인
	DWORD	__stdcall AxdiLevelGetInportDword(long lModuleNo, long lOffset, DWORD *upLevel);
	// 전체 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 설정
	DWORD	__stdcall AxdiLevelSetInport(long lOffset, DWORD uLevel);
	// 전체 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 확인
	DWORD	__stdcall AxdiLevelGetInport(long lOffset, DWORD *upLevel);
//==출력 레벨 설정 확인
	// 지정한 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 설정
	DWORD	__stdcall AxdoLevelSetOutportBit(long lModuleNo, long lOffset, DWORD uLevel);
	// 지정한 출력 접점 모듈의 Offset 위치에서 byte 단위로 데이터 레벨을 설정
	DWORD	__stdcall AxdoLevelSetOutportByte(long lModuleNo, long lOffset, DWORD uLevel);
	// 지정한 출력 접점 모듈의 Offset 위치에서 word 단위로 데이터 레벨을 설정
	DWORD	__stdcall AxdoLevelSetOutportWord(long lModuleNo, long lOffset, DWORD uLevel);
	// 지정한 출력 접점 모듈의 Offset 위치에서 double word 단위로 데이터 레벨을 설정
	DWORD	__stdcall AxdoLevelSetOutportDword(long lModuleNo, long lOffset, DWORD uLevel);
	// 지정한 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 확인
	DWORD	__stdcall AxdoLevelGetOutportBit(long lModuleNo, long lOffset, DWORD *upLevel);
	// 지정한 출력 접점 모듈의 Offset 위치에서 byte 단위로 데이터 레벨을 확인
	DWORD	__stdcall AxdoLevelGetOutportByte(long lModuleNo, long lOffset, DWORD *upLevel);
	// 지정한 출력 접점 모듈의 Offset 위치에서 word 단위로 데이터 레벨을 확인
	DWORD	__stdcall AxdoLevelGetOutportWord(long lModuleNo, long lOffset, DWORD *upLevel);
	// 지정한 출력 접점 모듈의 Offset 위치에서 double word 단위로 데이터 레벨을 확인
	DWORD	__stdcall AxdoLevelGetOutportDword(long lModuleNo, long lOffset, DWORD *upLevel);
	// 전체 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 설정
	DWORD	__stdcall AxdoLevelSetOutport(long lOffset, DWORD uLevel);
	// 전체 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터 레벨을 확인
	DWORD	__stdcall AxdoLevelGetOutport(long lOffset, DWORD *upLevel);

//========== 입출력 포트 쓰기 읽기 
//==출력 포트 쓰기
	// 전체 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 출력
	DWORD	__stdcall AxdoWriteOutport(long lOffset, DWORD uValue);
	// 지정한 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 출력
	DWORD	__stdcall AxdoWriteOutportBit(long lModuleNo, long lOffset, DWORD uValue);
	// 지정한 출력 접점 모듈의 Offset 위치에서 byte 단위로 데이터를 출력
	DWORD	__stdcall AxdoWriteOutportByte(long lModuleNo, long lOffset, DWORD uValue);
	// 지정한 출력 접점 모듈의 Offset 위치에서 word 단위로 데이터를 출력
	DWORD	__stdcall AxdoWriteOutportWord(long lModuleNo, long lOffset, DWORD uValue);
	// 지정한 출력 접점 모듈의 Offset 위치에서 double word 단위로 데이터를 출력
	DWORD	__stdcall AxdoWriteOutportDword(long lModuleNo, long lOffset, DWORD uValue);
//==출력 포트 읽기
	// 전체 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 읽기
	DWORD	__stdcall AxdoReadOutport(long lOffset, DWORD *upValue);
	// 지정한 출력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 읽기
	DWORD	__stdcall AxdoReadOutportBit(long lModuleNo, long lOffset, DWORD *upValue);
	// 지정한 출력 접점 모듈의 Offset 위치에서 byte 단위로 데이터를 읽기
	DWORD	__stdcall AxdoReadOutportByte(long lModuleNo, long lOffset, DWORD *upValue);
	// 지정한 출력 접점 모듈의 Offset 위치에서 word 단위로 데이터를 읽기
	DWORD	__stdcall AxdoReadOutportWord(long lModuleNo, long lOffset, DWORD *upValue);
	// 지정한 출력 접점 모듈의 Offset 위치에서 double word 단위로 데이터를 읽기
	DWORD	__stdcall AxdoReadOutportDword(long lModuleNo, long lOffset, DWORD *upValue);
//==입력 포트 읽기
	// 전체 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 읽기
	DWORD	__stdcall AxdiReadInport(long lOffset, DWORD *upValue);
	// 지정한 입력 접점 모듈의 Offset 위치에서 bit 단위로 데이터를 읽기
	DWORD	__stdcall AxdiReadInportBit(long lModuleNo, long lOffset, DWORD *upValue);
	// 지정한 입력 접점 모듈의 Offset 위치에서 byte 단위로 데이터를 읽기
	DWORD	__stdcall AxdiReadInportByte(long lModuleNo, long lOffset, DWORD *upValue);
	// 지정한 입력 접점 모듈의 Offset 위치에서 word 단위로 데이터를 읽기
	DWORD	__stdcall AxdiReadInportWord(long lModuleNo, long lOffset, DWORD *upValue);
	// 지정한 입력 접점 모듈의 Offset 위치에서 double word 단위로 데이터를 읽기
	DWORD	__stdcall AxdiReadInportDword(long lModuleNo, long lOffset, DWORD *upValue);

//========== 고급 함수 
	// 지정한 입력 접점 모듈의 Offset 위치에서 신호가 Off에서 On으로 바뀌었는지 확인
	DWORD	__stdcall AxdiIsPulseOn(long lModuleNo, long lOffset, DWORD *upValue);
	// 지정한 입력 접점 모듈의 Offset 위치에서 신호가 On에서 Off으로 바뀌었는지 확인
	DWORD	__stdcall AxdiIsPulseOff(long lModuleNo, long lOffset, DWORD *upValue);
	// 지정한 입력 접점 모듈의 Offset 위치에서 신호가 count 만큼 호출될 동안 On 상태로 유지하는지 확인
	DWORD	__stdcall AxdiIsOn(long lModuleNo, long lOffset, long lCount, DWORD *upValue, long lStart);
	// 지정한 입력 접점 모듈의 Offset 위치에서 신호가 count 만큼 호출될 동안 Off 상태로 유지하는지 확인
	DWORD	__stdcall AxdiIsOff(long lModuleNo, long lOffset, long lCount, DWORD *upValue, long lStart);

	// 지정한 출력 접점 모듈의 Offset 위치에서 설정한 mSec동안 On을 유지하다가 Off 시킴
	DWORD	__stdcall AxdoOutPulseOn(long lModuleNo, long lOffset, long lmSec);
	// 지정한 출력 접점 모듈의 Offset 위치에서 설정한 mSec동안 Off를 유지하다가 On 시킴
	DWORD	__stdcall AxdoOutPulseOff(long lModuleNo, long lOffset, long lmSec);
	// 지정한 출력 접점 모듈의 Offset 위치에서 설정한 횟수, 설정한 간격으로 토글한 후 원래의 출력상태를 유지함
	DWORD	__stdcall AxdoToggleStart(long lModuleNo, long lOffset, long lInitState, long lmSecOn, long lmSecOff, long lCount);
	// 지정한 출력 접점 모듈의 Offset 위치에서 토글중인 출력을 설정한 신호 상태로 정지 시킴
	DWORD	__stdcall AxdoToggleStop(long lModuleNo, long lOffset, DWORD uOnOff);

 

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__AXT_AXD_H__
