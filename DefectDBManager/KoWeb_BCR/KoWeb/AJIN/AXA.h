#ifndef __AXT_AXA_H__
#define __AXT_AXA_H__

#include "AXHS.h"

#ifdef __cplusplus
extern "C" {
//Axl Naming 규정 적용 후 	
#endif	//__cplusplus
//========== 보드 및 모듈 정보 확인 함수 
	//AIO 모듈이 있는지 확인한다
	DWORD	__stdcall AxaInfoIsAIOModule(DWORD *upStatus);
	//모듈 No를 확인한다.
	DWORD	__stdcall AxaInfoGetModuleNo(long lBoardNo, long lModulePos, long *lpModuleNo);
	//AIO 모듈의 개수를 확인한다.
	DWORD	__stdcall AxaInfoGetModuleCount(long *lpModuleCount);
	//지정한 모듈의 입력 채널 수를 확인한다
	DWORD	__stdcall AxaInfoGetInputCount(long lModuleNo, long *lpCount);
	//지정한 모듈의 출력 채널 수를 확인한다.
	DWORD	__stdcall AxaInfoGetOutputCount(long lModuleNo, long *lpCount);
	//지정한 모듈의 첫 번째 채널 번호를 확인한다. 
	DWORD	__stdcall AxaInfoGetChannelNoOfModuleNo(long lModuleNo, long *lpChannelNo);
	//지정한 모듈 번호로 베이스 보드 번호, 모듈 위치, 모듈 ID를 확인한다.
	DWORD	__stdcall AxaInfoGetModule (long lModuleNo, long *lpBoardNo, long *lpModulePos, DWORD *upModuleID);

//========== 입력 모듈 정보 검색 함수
	//지정한 입력 채널 번호로 모듈 번호를 확인한다.
	DWORD	__stdcall AxaiInfoGetModuleNoOfChannelNo(long lChannelNo, long *lpModuleNo);
	//아날로그 입력 모듈의 전체 채널 개수를 확인한다.
	DWORD	__stdcall AxaiInfoGetChannelCount(long *lpChannelCount);
//=========== 로그 출력 관련 함수
	//지정한 입력 채널의 EzSpy에 로그 출력 여부를 설정한다.
	DWORD	__stdcall AxaiLogSetChannel(long lChannelNo, DWORD uUse);
	//지정한 입력 채널의 EzSpy에 로그 출력 여부를 확인한다.
	DWORD	__stdcall AxaiLogGetChannel(long lChannelNo, DWORD *upUse);

//========== 입력 모듈 인터럽트/채널 이벤트 설정 및 확인 함수
	//지정한 채널에 이벤트 메시지를 받아오기 위하여 윈도우 메시지, 콜백 함수 또는 이벤트 방식을 사용한다. H/W 타이머(Timer Trigger Mode, External Trigger Mode)를 이용, 연속적 데이터 수집 동작시(AxaStartMultiChannelAdc 참조)에 사용한다.
	DWORD	__stdcall AxaiEventSetChannel(long lChannelNo, HWND hWnd, DWORD uMesssage, AXT_EVENT_PROC pProc, HANDLE *pEvent);
	//지정한 입력 채널에 이벤트 사용 유무를 설정한다.
	DWORD	__stdcall AxaiEventSetChannelEnable(long lChannelNo, DWORD uUse);
	//지정한 입력 채널의 이벤트 사용 유무를 확인한다.
	DWORD	__stdcall AxaiEventGetChannelEnable(long lChannelNo, DWORD *upUse);
	//지정한 여러 입력 채널에 이벤트 사용 유무를 설정한다.
	DWORD	__stdcall AxaiEventSetMultiChannelEnable(long lSize, long *lpChannelNo, DWORD uUse);
	//지정한 입력 채널에 이벤트 종류를 설정한다.
	DWORD	__stdcall AxaiEventSetChannelMask(long lChannelNo, DWORD uMask);
	//지정한 입력 채널에 이벤트 종류를 확인한다.
	DWORD	__stdcall AxaiEventGetChannelMask(long lChannelNo, DWORD *upMask);
	//지정한 여러 입력 채널에 이벤트 종류를 설정한다.
	DWORD	__stdcall AxaiEventSetMultiChannelMask(long lSize, long *lpChannelNo, DWORD uMask);
	//이벤트 발생 위치를 확인한다.
	DWORD	__stdcall AxaiEventRead(long *lpChannelNo, DWORD *upMode);
	//지정한 모듈의 인터럽트 마스크를 설정한다. 이 함수는 연속적 신호감시를 할 경우에 하드웨어(모듈)의 FIFO 에서 사용자가 지정한 크기의 버퍼로 내부 인터럽트를 통한 데이터 이동 시점을 지정하기 위해 사용된다. (SIO-AI4RB는 지원하지 않는다.)
	DWORD	__stdcall AxaiInterruptSetModuleMask(long lModuleNo, DWORD uMask);
	//지정한 모듈의 인터럽트 마스크를 확인한다.
	DWORD	__stdcall AxaiInterruptGetModuleMask(long lModuleNo, DWORD *upMask);

//========== 입력 모듈 파라미터 설정 및 확인 함수
	//지정한 입력 채널에 입력 전압 범위를 설정한다.
	DWORD	__stdcall AxaiSetRange(long lChannelNo, double dMinVolt, double dMaxVolt);
	//지정한 입력 채널의 입력 전압 범위를 확인한다.
	DWORD	__stdcall AxaiGetRange(long lChannelNo, double *dpMinVolt, double *dpMaxVolt);
	//지정한 여러 입력 채널에 허용 입력 전압 범위를 설정한다.
	DWORD	__stdcall AxaiSetMultiRange(long lSize, long *lpChannelNo, double dMinVolt, double dMaxVolt);
	//지정한 입력 모듈에 트리거 모드를 설정한다.
	DWORD	__stdcall AxaiSetTriggerMode(long lModuleNo, DWORD uTriggerMode);
	//지정한 모듈에 트리거 모드를 확인한다.
	DWORD	__stdcall AxaiGetTriggerMode(long lModuleNo, DWORD *upTriggerMode);
	//지정한 입력모듈의 Offset을 mVolt 단위(mV)로 설정한다. 최대 -100~100mVolt
	DWORD	__stdcall AxaiSetModuleOffsetValue(long lModuleNo, double dMiliVolt); 
	//지정한 입력모듈의 Offset 값을 확인한다. mVolt 단위(mV)
	DWORD	__stdcall AxaiGetModuleOffsetValue(long lModuleNo, double *dpMiliVolt); 

//========== 입력 모듈 A/D 변환 함수 
//==Software Trigger Mode 함수
	//사용자가 지정한 입력 채널에 아날로그 입력 값을 A/D변환한 후 전압 값으로 반환한다.이 함수를 사용하기 전에 AxaSetTriggerModeAdc 함수를 사용하여 Normal Trigger Mode로 설정되어 있어야 한다.
	DWORD	__stdcall AxaiSwReadVoltage(long lChannelNo, double *dpVolt);
	//지정한 입력 채널에 아날로그 입력 값을 Digit 값으로 반환한다. Normal Trigger Mode로 설정되어 있어야 한다.
	DWORD	__stdcall AxaiSwReadDigit(long lChannelNo, DWORD *upDigit);
	//지정한 여러 입력 채널에 아날로그 입력 값을 전압 값으로 반환한다. Normal Trigger Mode로 설정되어 있어야 한다.
	DWORD	__stdcall AxaiSwReadMultiVoltage(long lSize, long *lpChannelNo, double *dpVolt);
	//지정한 여러 입력 채널에 아날로그 입력 값을 Digit 값으로 반환한다. Normal Trigger Mode로 설정되어 있어야 한다.
	DWORD	__stdcall AxaiSwReadMultiDigit(long lSize, long *lpChannelNo, DWORD *upDigit);
//==Hardware Trigger Mode 함수
	//지정한 여러 입력 채널에 Immediate모드를 사용하기 위해 설정 값을 설정한다. 이 함수를 사용하기 전에 AxaSetTriggerModeAdc 함수를 사용하여 Timer Trigger Mode로 설정되어 있어야 한다.
	DWORD	__stdcall AxaiHwSetMultiAccess(long lSize, long *lpChannelNo, long *lpWordSize);
	//지정한 개수만큼 A/D변환 후 전압 값을 반환한다. 이 함수를 사용하기 전에 AxaiHwSetMultiAccess함수를 이용 설정값을 지정해야 하며 , AxaSetTriggerModeAdc 함수를 사용하여 Timer Trigger Mode로 설정되어 있어야 한다.
	DWORD	__stdcall AxaiHwStartMultiAccess(double *dpBuffer[]);
	//지정한 모듈에 샘플링 간격을 주파수 단위로 설정한다.
	DWORD	__stdcall AxaiHwSetSampleFreq(long lModuleNo, double dSampleFreq);
	//지정한 모듈에 샘플링 간격을 주파수 단위로 설정된 값을 확인한다.
	DWORD	__stdcall AxaiHwGetSampleFreq(long lModuleNo, double *dpSampleFreq);
	//지정한 모듈에 샘플링 간격을 시간 단위(uSec)로 설정한다.
	DWORD	__stdcall AxaiHwSetSamplePeriod(long lModuleNo, double dSamplePeriod);
	//지정한 모듈에 샘플링 간격을 시간 단위(uSec)로 설정된 값을 확인한다.
	DWORD	__stdcall AxaiHwGetSamplePeriod(long lModuleNo, double *dpSamplePeriod);
	//지정한 입력 채널에 버퍼가 Full로 찼을 때 관리 방식을 설정한다.
	DWORD	__stdcall AxaiHwSetBufferOverflowMode(long lChannelNo, DWORD uFullMode);
	//지정한 입력 채널이 버퍼가 Full로 찼을 때 관리 방식을 확인한다.
	DWORD	__stdcall AxaiHwGetBufferOverflowMode(long lChannelNo, DWORD *upFullMode);
	//지정한 여러 입력 채널에 버퍼가 Full로 찼을 때 관리 방식을 설정한다.
	DWORD	__stdcall AxaiHwSetMultiBufferOverflowMode(long lSize, long *lpChannelNo, DWORD uFullMode);
	//지정한 입력 채널에 버퍼의 상한 값과 하한 값을 설정한다.
	DWORD	__stdcall AxaiHwSetLimit(long lChannelNo, long lLowLimit, long lUpLimit);
	//지정한 입력 채널에 버퍼의 상한 값과 하한 값을 확인한다.
	DWORD	__stdcall AxaiHwGetLimit(long lChannelNo, long *lpLowLimit, long *lpUpLimit);
	//지정한 여러 입력 채널에 버퍼의 상한 값과 하한 값을 설정한다.
	DWORD	__stdcall AxaiHwSetMultiLimit(long lSize, long *lpChannelNo, long lLowLimit, long lUpLimit);
	//지정한 여러 입력 채널에 H/W타이머를 이용한 A/D변환을 시작한다.
	DWORD	__stdcall AxaiHwStartMultiChannel(long lSize, long *lpChannelNo, long lBuffSize);
	//지정한 여러 입력 채널에 A/D변환을 시작 후 지정한 개수만큼 필터 처리해서 전압으로 반환한다.
	DWORD	__stdcall AxaiHwStartMultiFilter(long lSize, long *lpChannelNo, long lFilterCount, long lBuffSize);
	//H/W타이머를 이용한 연속 신호 A/D변환을 중지한다.
	DWORD	__stdcall AxaiHwStopMultiChannel(long lModuleNo);
	//지정한 입력 채널의 메모리 버퍼에 데이터가 몇 개인지 검사한다.
	DWORD	__stdcall AxaiHwReadDataLength(long lChannelNo, long *lpDataLength);
	//지정한 입력 채널에 H/W타이머를 이용하여 A/D변환된 값을 전압 값으로 읽는다.
	DWORD	__stdcall AxaiHwReadSampleVoltage(long lChannelNo, long *lpSize, double *dpVolt);
	//지정한 입력 채널에 H/W타이머를 이용하여 A/D변환된 값을 Digit 값으로 읽는다.
	DWORD	__stdcall AxaiHwReadSampleDigit(long lChannelNo, long *lpsize, DWORD *upDigit);

//========== 입력 모듈 버퍼 상태 체크 함수
	//지정한 입력 채널의 메모리 버퍼에 데이터가 없는 지 검사한다.
	DWORD	__stdcall AxaiHwIsBufferEmpty(long lChannelNo, DWORD *upEmpty);
	//지정한 입력 채널의 메모리 버퍼에 설정되어 있는 상한 값보다 데이터가 많은 지 검사한다
	DWORD	__stdcall AxaiHwIsBufferUpper(long lChannelNo, DWORD *upUpper);
	//지정한 입력 채널의 메모리 버퍼에 설정되어 있는 하한 값보다 데이터가 적은 지 검사한다.
	DWORD	__stdcall AxaiHwIsBufferLower(long lChannelNo, DWORD *upLower);
//==출력 모듈 Info
	//지정한 출력 채널 번호로 모듈 번호를 확인한다.
	DWORD	__stdcall AxaoInfoGetModuleNoOfChannelNo(long lChannelNo, long *lpModuleNo);
	//아날로그 출력 모듈의 전체 채널 개수를 확인한다.
	DWORD	__stdcall AxaoInfoGetChannelCount(long *lpChannelCount);
//==지정한 출력 채널의 EzSpy 로그 출력 
	//지정한 출력 채널의 EzSpy에 로그 출력 여부를 설정한다.
	DWORD	__stdcall AxaoLogSetChannel(long lChannelNo, DWORD uUse);
	//지정한 출력 채널의 EzSpy에 로그 출력 여부를 확인한다.
	DWORD	__stdcall AxaoLogGetChannel(long lChannelNo, DWORD *upUse);

//========== 출력 모듈 설정 및 확인 함수
	//지정한 출력 채널에 출력 전압 범위를 설정한다
	DWORD	__stdcall AxaoSetRange(long lChannelNo, double dMinVolt, double dMaxVolt);
	//지정한 출력 채널의 출력 전압 범위를 확인한다.
	DWORD	__stdcall AxaoGetRange(long lChannelNo, double *dpMinVolt, double *dpMaxVolt);
	//지정한 여러 출력 채널에 출력 전압 범위를 설정한다.
	DWORD	__stdcall AxaoSetMultiRange(long lSize, long *lpChannelNo, double dMinVolt, double dMaxVolt);
	//지정한 출력 채널에 입력된 전압이 출력 된다.
	DWORD	__stdcall AxaoWriteVoltage(long lChannelNo, double dVolt);
	//지정한 여러 출력 채널에 입력된 전압이 출력 된다.
	DWORD	__stdcall AxaoWriteMultiVoltage(long lSize, long *lpChannelNo, double *dpVolt);
	//지정한 출력 채널에 출력되는 전압 값을 확인한다.
	DWORD	__stdcall AxaoReadVoltage(long lChannelNo, double *dpVolt);
	//지정한 여러 출력 채널에 출력되는 전압 값을 확인한다.
	DWORD	__stdcall AxaoReadMultiVoltage(long lSize, long *lpChannelNo, double *dpVolt);
 

#ifdef __cplusplus

}
#endif	//__cplusplus

#endif	//__AXT_AXA_H__
