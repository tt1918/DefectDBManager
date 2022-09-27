#ifndef __AXT_AXM_H__
#define __AXT_AXM_H__

#include "AXHS.h"

#ifdef __cplusplus
extern "C" {
#endif	//__cplusplus

//========== 보드 및 모듈 확인함수(Info) - Infomation =================================================================================

	// 해당 축의 보드번호, 모듈 위치, 모듈 아이디를 반환한다.
	DWORD	__stdcall AxmInfoGetAxis(long lAxisNo, long *lpBoardNo, long *lpModulePos, DWORD *upModuleID);
	// 모션 모듈이 존재하는지 반환한다.
	DWORD	__stdcall AxmInfoIsMotionModule(DWORD *upStatus);
	// 해당 축이 유효한지 반환한다.
	DWORD	__stdcall AxmInfoIsInvalidAxisNo(char *szInvalidAxisNo);
	// CAMC-IP, CAMC-QI 축 개수, 시스템에 장착된 유효한 모션 축수를 반환한다.
	DWORD	__stdcall AxmInfoGetAxisCount(long *lpAxisCount);
	// 해당 보드/모듈의 첫번째 축번호를 반환한다.
	DWORD	__stdcall AxmInfoGetFirstAxisNo(long lBoardNo, long lModulePos, long *lpAxisNo);
	
//========= 가상 축 함수 ============================================================================================	

	// 가상축을 설정한다.
	DWORD	__stdcall AxmVirtualSetAxisNoMap(long lRealAxisNo, long lVirtualAxisNo);
	// 설정한 가상채널(축) 번호를 반환한다.
	DWORD	__stdcall AxmVirtualGetAxisNoMap(long lRealAxisNo, long *lpVirtualAxisNo);
	// 멀티 가상축을 설정한다.
	DWORD	__stdcall AxmVirtualSetMultiAxisNoMap(long lSize, long *lpRealAxesNo, long *lpVirtualAxesNo);
    // 설정한 멀티 가상채널(축) 번호를 반환한다.
	DWORD	__stdcall AxmVirtualGetMultiAxisNoMap(long lSize, long *lpRealAxesNo, long *lpVirtualAxesNo);
    // 가상축 설정을 해지한다.
	DWORD	__stdcall AxmVirtualResetAxisMap();

//========= 로그 관련 함수 ==========================================================================================	

	// 설정 축의 함수 실행 결과를 EzSpy에서 모니터링 할 수 있도록 설정 또는 해제하는 함수이다.
	// uUse : 사용 유무 => DISABLE(0), ENABLE(1)
	DWORD	__stdcall AxmLogSetAxis(long lAxisNo, DWORD uUse);
	
	// EzSpy에서의 설정 축 함수 실행 결과 모니터링 여부를 확인하는 함수이다.
	DWORD	__stdcall AxmLogGetAxis(long lAxisNo, DWORD *upUse);

//========= 인터럽트 관련 함수 ======================================================================================
    // 콜백 함수 방식은 이벤트 발생 시점에 즉시 콜백 함수가 호출 됨으로 가장 빠르게 이벤트를 통지받을 수 있는 장점이 있으나
    // 콜백 함수가 완전히 종료 될 때까지 메인 프로세스가 정체되어 있게 된다.
    // 즉, 콜백 함수 내에 부하가 걸리는 작업이 있을 경우에는 사용에 주의를 요한다. 
    // 이벤트 방식은 쓰레드등을 이용하여 인터럽트 발생여부를 지속적으로 감시하고 있다가 인터럽트가 발생하면 
    // 처리해주는 방법으로, 쓰레드 등으로 인해 시스템 자원을 점유하고 있는 단점이 있지만
    // 가장 빠르게 인터럽트를 검출하고 처리해줄 수 있는 장점이 있다.
    // 일반적으로는 많이 쓰이지 않지만, 인터럽트의 빠른처리가 주요 관심사인 경우에 사용된다. 
    // 이벤트 방식은 이벤트의 발생 여부를 감시하는 특정 쓰레드를 사용하여 메인 프로세스와 별개로 동작되므로
    // MultiProcessor 시스템등에서 자원을 가장 효율적으로 사용할 수 있게 되어 특히 권장하는 방식이다.

	
    // 인터럽트 메시지를 받아오기 위하여 윈도우 메시지 또는 콜백 함수를 사용한다.
    // (메시지 핸들, 메시지 ID, 콜백함수, 인터럽트 이벤트)
    //    hWnd    : 윈도우 핸들, 윈도우 메세지를 받을때 사용. 사용하지 않으면 NULL을 입력.
    //    wMsg    : 윈도우 핸들의 메세지, 사용하지 않거나 디폴트값을 사용하려면 0을 입력.
    //    proc    : 인터럽트 발생시 호출될 함수의 포인터, 사용하지 않으면 NULL을 입력.
	//    pEvent  : 이벤트 방법사용시 이벤트 핸들
	DWORD	__stdcall AxmInterruptSetAxis(long lAxisNo, HWND hWnd, DWORD uMessage, AXT_INTERRUPT_PROC pProc, HANDLE *pEvent);

	// 설정 축의 인터럽트 사용 여부를 설정한다
	// 해당 축에 인터럽트 설정 / 확인
 	// uUse : 사용 유무 => DISABLE(0), ENABLE(1)
	DWORD	__stdcall AxmInterruptSetAxisEnable(long lAxisNo, DWORD uUse);
	// 설정 축의 인터럽트 사용 여부를 반환한다
	DWORD	__stdcall AxmInterruptGetAxisEnable(long lAxisNo, DWORD *upUse);

    //인터럽트를 이벤트 방식으로 사용할 경우 해당 인터럽트 정보 읽는다.
	DWORD	__stdcall AxmInterruptRead(long *lpAxisNo, DWORD *upFlag);
	
	// 해당 축의 인터럽트 플래그 값을 반환한다.
	DWORD	__stdcall AxmInterruptReadAxisFlag(long lAxisNo, long lBank, DWORD *upFlag);

	// 지정 축의 사용자가 설정한 인터럽트 발생 여부를 설정한다.
	// lBank         : 인터럽트 뱅크 번호 (0 - 1) 설정가능.
	// uInterruptNum : 인터럽트 번호 설정 비트번호로 설정 ( 0 - 31 )설정가능.
	DWORD	__stdcall AxmInterruptSetUserEnable(long lAxisNo, long lBank, DWORD uInterruptNum);

	// 지정 축의 사용자가 설정한 인터럽트 발생 여부를 확인한다.
	DWORD	__stdcall AxmInterruptGetUserEnable(long lAxisNo, long lBank, DWORD *upInterruptNum);

//======== 모션 파라메타 설정 ===========================================================================================================================================================
	// AxmMotLoadParaAll로 파일을 Load 시키지 않으면 초기 파라메타 설정시 기본 파라메타 설정. 
	// 현재 PC에 사용되는 모든축에 똑같이 적용된다. 기본파라메타는 아래와 같다. 
	// 00:AXIS_NO.             =0       01:PULSE_OUT_METHOD.    =4      02:ENC_INPUT_METHOD.    =3     03:INPOSITION.          =2
    // 04:ALARM.               =0       05:NEG_END_LIMIT.       =0      06:POS_END_LIMIT.       =0     07:MIN_VELOCITY.        =1
    // 08:MAX_VELOCITY.        =700000  09:HOME_SIGNAL.         =4      10:HOME_LEVEL.          =1     11:HOME_DIR.            =-1
    // 12:ZPHASE_LEVEL.        =1       13:ZPHASE_USE.          =0      14:STOP_SIGNAL_MODE.    =0     15:STOP_SIGNAL_LEVEL.   =0
    // 16:HOME_FIRST_VELOCITY. =10000   17:HOME_SECOND_VELOCITY.=10000  18:HOME_THIRD_VELOCITY. =2000  19:HOME_LAST_VELOCITY.  =100
    // 20:HOME_FIRST_ACCEL.    =40000   21:HOME_SECOND_ACCEL.   =40000  22:HOME_END_CLEAR_TIME. =1000  23:HOME_END_OFFSET.     =0
    // 24:NEG_SOFT_LIMIT.      =0.000   25:POS_SOFT_LIMIT.      =0      26:MOVE_PULSE.          =1     27:MOVE_UNIT.           =1
    // 28:INIT_POSITION.       =1000    29:INIT_VELOCITY.       =200    30:INIT_ACCEL.          =400   31:INIT_DECEL.          =400
    // 32:INIT_ABSRELMODE.     =0       33:INIT_PROFILEMODE.    =4

	// 00=[AXIS_NO             ]: 축 (0축 부터 시작함)
	// 01=[PULSE_OUT_METHOD    ]: Pulse out method TwocwccwHigh = 6
	// 02=[ENC_INPUT_METHOD    ]: disable = 0   1체배 = 1  2체배 = 2  4체배 = 3, 결선 관련방향 교체시(-).1체배 = 11  2체배 = 12  4체배 = 13
	// 03=[INPOSITION          ], 04=[ALARM     ], 05,06 =[END_LIMIT   ]  : 0 = A접점 1= B접점 2 = 사용안함. 3 = 기존상태 유지
	// 07=[MIN_VELOCITY        ]: 시작 속도(START VELOCITY)
	// 08=[MAX_VELOCITY        ]: 드라이버가 지령을 받아들일수 있는 지령 속도. 보통 일반 Servo는 700k
	// Ex> screw : 20mm pitch drive: 10000 pulse 모터: 400w
	// 09=[HOME_SIGNAL         ]: 4 - Home in0 , 0 :PosEndLimit , 1 : NegEndLimit // _HOME_SIGNAL참조.
	// 10=[HOME_LEVEL          ]: 0 = A접점 1= B접점 2 = 사용안함. 3 = 기존상태 유지
	// 11=[HOME_DIR            ]: 홈 방향(HOME DIRECTION) 1:+방향, 0:-방향
	// 12=[ZPHASE_LEVEL        ]: 0 = A접점 1= B접점 2 = 사용안함. 3 = 기존상태 유지
	// 13=[ZPHASE_USE          ]: Z상사용여부. 0: 사용안함 , 1: -방향, 2: +방향 
	// 14=[STOP_SIGNAL_MODE    ]: ESTOP, SSTOP 사용시 모드 0:감속정지, 1:급정지 
	// 15=[STOP_SIGNAL_LEVEL   ]: ESTOP, SSTOP 사용 레벨.  0 = A접점 1= B접점 2 = 사용안함. 3 = 기존상태 유지 
	// 16=[HOME_FIRST_VELOCITY ]: 1차구동속도 
	// 17=[HOME_SECOND_VELOCITY]: 검출후속도 
	// 18=[HOME_THIRD_VELOCITY ]: 마지막 속도 
	// 19=[HOME_LAST_VELOCITY  ]: index검색및 정밀하게 검색하기위한 속도. 
	// 20=[HOME_FIRST_ACCEL    ]: 1차 가속도 , 21=[HOME_SECOND_ACCEL   ] : 2차 가속도 
	// 22=[HOME_END_CLEAR_TIME ]: 원점 검색 Enc 값 Set하기 위한 대기시간,  23=[HOME_END_OFFSET] : 원점검출후 Offset만큼 이동.
	// 24=[NEG_SOFT_LIMIT      ]: - SoftWare Limit 같게 설정하면 사용안함, 25=[POS_SOFT_LIMIT ]: + SoftWare Limit 같게 설정하면 사용안함.
	// 26=[MOVE_PULSE          ]: 드라이버의 1회전당 펄스량              , 27=[MOVE_UNIT  ]: 드라이버 1회전당 이동량 즉:스크류 Pitch
	// 28=[INIT_POSITION       ]: 에이젼트 사용시 초기위치  , 사용자가 임의로 사용가능
	// 29=[INIT_VELOCITY       ]: 에이젼트 사용시 초기속도  , 사용자가 임의로 사용가능
	// 30=[INIT_ACCEL          ]: 에이젼트 사용시 초기가속도, 사용자가 임의로 사용가능
	// 31=[INIT_DECEL          ]: 에이젼트 사용시 초기감속도, 사용자가 임의로 사용가능
	// 32=[INIT_ABSRELMODE     ]: 절대(0)/상대(1) 위치 설정
	// 33=[INIT_PROFILEMODE    ]: 프로파일모드(0 - 4) 까지 설정
	//                            '0': 대칭 Trapezode, '1': 비대칭 Trapezode, '2': 대칭 Quasi-S Curve, '3':대칭 S Curve, '4':비대칭 S Curve
    
	// AxmMotSaveParaAll로 저장 되어진 .mot파일을 불러온다. 사용자가 임의로 불러서 변경가능하다.
	DWORD	__stdcall AxmMotLoadParaAll(char *szFilePath);
	// 현재 모든축에 대한 모든 파라메타를 축별로 저장한다. .mot파일로 저장한다. AxmMotLoadParaAll 사용해서 파일을 불러온다.
	DWORD	__stdcall AxmMotSaveParaAll(char *szFilePath);

	// 파라메타 28 - 31번까지 사용자가 프로그램내에서  이 함수를 이용해 설정 한다
	DWORD	__stdcall AxmMotSetParaLoad(long lAxisNo, double dInitPos, double dInitVel, double dInitAccel, double dInitDecel);	
	// 파라메타 28 - 31번까지 사용자가 프로그램내에서  이 함수를 이용해 확인 한다.
	DWORD	__stdcall AxmMotGetParaLoad(long lAxisNo, double *dpInitPos, double *dpInitVel, double *dpInitAccel, double *dpInitDecel);	


	//uMethod  0 :OneHighLowHigh, 1 :OneHighHighLow, 2 :OneLowLowHigh, 3 :OneLowHighLow, 4 :TwoCcwCwHigh
	//         5 :TwoCcwCwLow, 6 :TwoCwCcwHigh, 7 :TwoCwCcwLow, 8 :TwoPhase, 9 :TwoPhaseReverse
	// 단 IP는 Method 8-9 는 지원하지 않음.
	
	// 지정 축의 펄스 출력 방식을 설정한다.
	DWORD	__stdcall AxmMotSetPulseOutMethod(long lAxisNo, DWORD uMethod);
	// 지정 축의 펄스 출력 방식 설정을 반환한다,
	DWORD	__stdcall AxmMotGetPulseOutMethod(long lAxisNo, DWORD *upMethod);

	// 지정 축의 외부(Actual) 카운트의 증가 방향 설정을 포함하여 지정 축의 Encoder 입력 방식을 설정한다.
    // UpDownMode = 0x0    : Up/Down
    // Sqr1Mode   = 0x1    : 1체배
    // Sqr2Mode   = 0x2    : 2체배
    // Sqr4Mode   = 0x3    : 4체배
    // # Reverse 포함 (0x04 ~0x07)"
	DWORD	__stdcall AxmMotSetEncInputMethod(long lAxisNo, DWORD uMethod);
	// 지정 축의 외부(Actual) 카운트의 증가 방향 설정을 포함하여 지정 축의 Encoder 입력 방식을 반환한다.
	DWORD	__stdcall AxmMotGetEncInputMethod(long lAxisNo, DWORD *upMethod);

	// 설정 속도 단위가 RPM(Revolution Per Minute)으로 맞추고 싶다면.
    // ex>	rpm 계산:
	// 4500 rpm ?
	// unit/ pulse = 1 : 1이면      pulse/ sec 초당 펄스수가 되는데
    // 4500 rpm에 맞추고 싶다면     4500 / 60 초 : 75회전/ 1초
    // 모터가 1회전에 몇 펄스인지 알아야 된다. 이것은 Encoder에 Z상을 검색해보면 알수있다.
	// 1회전:1800 펄스라면 75 x 1800 = 135000 펄스가 필요하게 된다.
	// AxmMotSetMoveUnitPerPulse에 Unit = 1, Pulse = 1800 넣어 동작시킨다.
	// 주의할점 : rpm으로 제어하게 된다면 속도와 가속도 도 rpm단위로 바뀌게 된다.

	// 지정 축의 펄스 당 움직이는 거리를 설정한다.
	DWORD	__stdcall AxmMotSetMoveUnitPerPulse(long lAxisNo, double dUnit, long lPulse);
	// 지정 축의 펄스 당 움직이는 거리를 반환한다.
	DWORD	__stdcall AxmMotGetMoveUnitPerPulse(long lAxisNo, double *dpUnit, long *lpPulse);
	
	// 지정 축에 감속 시작 포인트 검출 방식을 설정한다.
    // 0x0 : 자동 가감속.
    // 0x1 : 수동 가감속."
	DWORD	__stdcall AxmMotSetDecelMode(long lAxisNo, DWORD uMethod);
	// 지정 축의 감속 시작 포인트 검출 방식을 반환한다
	DWORD	__stdcall AxmMotGetDecelMode(long lAxisNo, DWORD *upMethod);
	
	// 지정 축에 수동 감속 모드에서 잔량 펄스를 설정한다.
	DWORD	__stdcall AxmMotSetRemainPulse(long lAxisNo, DWORD uData);
    // 지정 축의 수동 감속 모드에서 잔량 펄스를 반환한다.
	DWORD	__stdcall AxmMotGetRemainPulse(long lAxisNo, DWORD *upData);

	// 지정 축에 등속도 구동 함수에서의 최고 속도를 설정한다.
	DWORD	__stdcall AxmMotSetMaxVel(long lAxisNo, double dVel);
	// 지정 축의 등속도 구동 함수에서의 최고 속도를 반환한다.
	DWORD	__stdcall AxmMotGetMaxVel(long lAxisNo, double *dpVel);

	// 지정 축의 이동 거리 계산 모드를 설정한다.
    //uAbsRelMode : '0' - 절대 좌표계
    //              '1' - 상대 좌표계
	DWORD	__stdcall AxmMotSetAbsRelMode(long lAxisNo, DWORD uAbsRelMode);
    // 지정 축의 설정된 이동 거리 계산 모드를 반환한다
	DWORD	__stdcall AxmMotGetAbsRelMode(long lAxisNo, DWORD *upAbsRelMode);

	//지정 축의 구동 속도 프로파일 모드를 설정한다.
    //ProfileMode : '0' - 대칭 Trapezode
     //             '1' - 비대칭 Trapezode
     //             '2' - 대칭 Quasi-S Curve
     //             '3' - 대칭 S Curve
     //             '4' - 비대칭 S Curve
	DWORD	__stdcall AxmMotSetProfileMode(long lAxisNo, DWORD uProfileMode);
	// 지정 축의 설정한 구동 속도 프로파일 모드를 반환한다.
	DWORD	__stdcall AxmMotGetProfileMode(long lAxisNo, DWORD *upProfileMode);
	
	//지정 축의 가속도 단위를 설정한다.
	//AccelUnit : '0' - 가감속 단위를 unit/sec2 사용
	//            '1' - 가감속 단위를 sec 사용
	DWORD	__stdcall AxmMotSetAccelUnit(long lAxisNo, DWORD uAccelUnit);
	// 지정 축의 설정된 가속도단위를 반환한다.
	DWORD	__stdcall AxmMotGetAccelUnit(long lAxisNo, DWORD *upAccelUnit);

	// 지정 축에 초기 속도를 설정한다.
	DWORD	__stdcall AxmMotSetMinVel(long lAxisNo, double dMinVel);
	// 지정 축의 초기 속도를 반환한다.
	DWORD	__stdcall AxmMotGetMinVel(long lAxisNo, double *dpMinVel);

	// 지정 축의 가속 저크값을 설정한다.[%].
	DWORD	__stdcall AxmMotSetAccelJerk(long lAxisNo, double dAccelJerk);
	// 지정 축의 설정된 가속 저크값을 반환한다.
	DWORD	__stdcall AxmMotGetAccelJerk(long lAxisNo, double *dpAccelJerk);

	// 지정 축의 감속 저크값을 설정한다.[%].
	DWORD	__stdcall AxmMotSetDecelJerk(long lAxisNo, double dDecelJerk);
	// 지정 축의 설정된 감속 저크값을 반환한다.
	DWORD	__stdcall AxmMotGetDecelJerk(long lAxisNo, double *dpDecelJerk);
	
//=========== 입출력 신호 관련 설정함수 ================================================================================

	// 지정 축의 Z 상 Level을 설정한다.
	// uLevel : LOW(0), HIGH(1)
	DWORD	__stdcall AxmSignalSetZphaseLevel(long lAxisNo, DWORD uLevel);
	// 지정 축의 Z 상 Level을 반환한다.
	DWORD	__stdcall AxmSignalGetZphaseLevel(long lAxisNo, DWORD *upLevel);

	// 지정 축의 Servo-On신호의 출력 레벨을 설정한다.
	// uLevel : LOW(0), HIGH(1)
	DWORD	__stdcall AxmSignalSetServoOnLevel(long lAxisNo, DWORD uLevel);
	// 지정 축의 Servo-On신호의 출력 레벨 설정을 반환한다.
	DWORD	__stdcall AxmSignalGetServoOnLevel(long lAxisNo, DWORD *upLevel);

	// 지정 축의 Servo-Alarm Reset 신호의 출력 레벨을 설정한다.
	// uLevel : LOW(0), HIGH(1)
	DWORD	__stdcall AxmSignalSetServoAlarmResetLevel(long lAxisNo, DWORD uLevel);
	// 지정 축의 Servo-Alarm Reset 신호의 출력 레벨을 설정을 반환한다.
	DWORD	__stdcall AxmSignalGetServoAlarmResetLevel(long lAxisNo, DWORD *upLevel);

	//	지정 축의 Inpositon 신호 사용 여부 및 신호 입력 레벨을 설정한다
	// uLevel : LOW(0), HIGH(1), UNUSED(2), USED(3)
	DWORD	__stdcall AxmSignalSetInpos(long lAxisNo, DWORD uUse);
	// 지정 축의 Inpositon 신호 사용 여부 및 신호 입력 레벨을 반환한다.
	DWORD	__stdcall AxmSignalGetInpos(long lAxisNo, DWORD *upUse);
	// 지정 축의 Inpositon 신호 입력 상태를 반환한다.
	DWORD	__stdcall AxmSignalReadInpos(long lAxisNo, DWORD *upStatus);

	//	지정 축의 알람 신호 입력 시 비상 정지의 사용 여부 및 신호 입력 레벨을 설정한다.
	// uLevel : LOW(0), HIGH(1), UNUSED(2), USED(3)
	DWORD	__stdcall AxmSignalSetServoAlarm(long lAxisNo, DWORD uUse);
	// 지정 축의 알람 신호 입력 시 비상 정지의 사용 여부 및 신호 입력 레벨을 반환한다.
	DWORD	__stdcall AxmSignalGetServoAlarm(long lAxisNo, DWORD *upUse);
	// 지정 축의 알람 신호의 입력 레벨을 반환한다.
	DWORD	__stdcall AxmSignalReadServoAlarm(long lAxisNo, DWORD *upStatus);

	// 지정 축의 end limit sensor의 사용 유무 및 신호의 입력 레벨을 설정한다. 
	// end limit sensor 신호 입력 시 감속정지 또는 급정지에 대한 설정도 가능하다.
    // uStopMode: EMERGENCY_STOP(0), SLOWDOWN_STOP(1)
    // uPositiveLevel, uNegativeLevel : LOW(0), HIGH(1), UNUSED(2), USED(3)
	DWORD	__stdcall AxmSignalSetLimit(long lAxisNo, DWORD uStopMode, DWORD uPositiveLevel, DWORD uNegativeLevel);
	// 지정 축의 end limit sensor의 사용 유무 및 신호의 입력 레벨, 신호 입력 시 정지모드를 반환한다
	DWORD	__stdcall AxmSignalGetLimit(long lAxisNo, DWORD *upStopMode, DWORD *upPositiveLevel, DWORD *upNegativeLevel);
	// 지정축의 end limit sensor의 입력 상태를 반환한다.
	DWORD	__stdcall AxmSignalReadLimit(long lAxisNo, DWORD *upPositiveStatus, DWORD *upNegativeStatus);

	// 지정 축의 Software limit의 사용 유무, 사용할 카운트, 그리고 정지 방법을 설정한다
    // uUse       : DISABLE(0), ENABLE(1)
    // uStopMode  : EMERGENCY_STOP(0), SLOWDOWN_STOP(1)
    // uSelection : COMMAND(0), ACTUAL(1)
	// 주의사항: 원점검색시 위함수를 이용하여 소프트웨어 리밋을 미리 설정해서 구동시 원점검색시 원점검색을 도중에 멈추어졌을경우 DISABLE이 된다. 
	
	DWORD	__stdcall AxmSignalSetSoftLimit(long lAxisNo, DWORD uUse, DWORD uStopMode, DWORD uSelection, double dPositivePos, double dNegativePos);
	// 지정 축의 Software limit의 사용 유무, 사용할 카운트, 그리고 정지 방법을 반환한다
	DWORD	__stdcall AxmSignalGetSoftLimit(long lAxisNo, DWORD *upUse, DWORD *upStopMode, DWORD *upSelection, double *dpPositivePos, double *dpNegativePos);

	// 비상 정지 신호의 정지 방법 (급정지/감속정지) 또는 사용 유무를 설정한다.
    // uStopMode  : EMERGENCY_STOP(0), SLOWDOWN_STOP(1)
	// uLevel : LOW(0), HIGH(1), UNUSED(2), USED(3)
	DWORD	__stdcall AxmSignalSetStop(long lAxisNo, DWORD uStopMode, DWORD uLevel);
	// 비상 정지 신호의 정지 방법 (급정지/감속정지) 또는 사용 유무를 반환한다.
	DWORD	__stdcall AxmSignalGetStop(long lAxisNo, DWORD *upStopMode, DWORD *upLevel);
	// 비상 정지 신호의 입력 상태를 반환한다.
	DWORD	__stdcall AxmSignalReadStop(long lAxisNo, DWORD *upStatus);

	// 지정 축의 Servo-On 신호를 출력한다.
	// uOnOff : FALSE(0), TRUE(1) ( 범용 0출력에 해당됨)
	DWORD	__stdcall AxmSignalServoOn(long lAxisNo, DWORD uOnOff);
	// 지정 축의 Servo-On 신호의 출력 상태를 반환한다.
	DWORD	__stdcall AxmSignalIsServoOn(long lAxisNo, DWORD *upOnOff);

	// 지정 축의 Servo-Alarm Reset 신호를 출력한다.
	// uOnOff : FALSE(0), TRUE(1) ( 범용 1출력에 해당됨)
	DWORD	__stdcall AxmSignalServoAlarmReset(long lAxisNo, DWORD uOnOff);
	
	//	범용 출력값을 설정한다.
    //  uValue : Hex Value 0x00
	DWORD	__stdcall AxmSignalWriteOutput(long lAxisNo, DWORD uValue);
	// 범용 출력값을 반환한다.
	DWORD	__stdcall AxmSignalReadOutput(long lAxisNo, DWORD *upValue);

    // lBitNo : Bit Number(0 - 4)
    // uOnOff : FALSE(0), TRUE(1)
	// 범용 출력값을 비트별로 설정한다.
	DWORD	__stdcall AxmSignalWriteOutputBit(long lAxisNo, long lBitNo, DWORD uOnOff);
	// 범용 출력값을 비트별로 반환한다.
	DWORD	__stdcall AxmSignalReadOutputBit(long lAxisNo, long lBitNo, DWORD *upOnOff);

	// 범용 입력값을 Hex값으로 반환한다.
	DWORD	__stdcall AxmSignalReadInput(long lAxisNo, DWORD *upValue);
	
    // lBitNo : Bit Number(0 - 4)
	// 범용 입력값을 비트별로 반환한다.
	DWORD	__stdcall AxmSignalReadInputBit(long lAxisNo, long lBitNo, DWORD *upOn);

//========== 모션 구동중 및 구동후에 상태 확인하는 함수============================================================

	// 지정 축의 펄스 출력 상태를 반환한다.
    // (구동상태)"
	DWORD	__stdcall AxmStatusReadInMotion(long lAxisNo, DWORD *upStatus);

	//  구동시작 이후 지정 축의 구동 펄스 카운터 값을 반환한다.
	//  (펄스 카운트 값)"
	DWORD	__stdcall AxmStatusReadDrivePulseCount(long lAxisNo, long *lpPulse);
	
	// 지정 축의 DriveStatus(모션중 상태) 레지스터를 반환한다
	DWORD	__stdcall AxmStatusReadMotion(long lAxisNo, DWORD *upStatus);
	
	// 지정 축의 EndStatus(정지 상태) 레지스터를 반환한다.
	DWORD	__stdcall AxmStatusReadStop(long lAxisNo, DWORD *upStatus);
	
	// 지정 축의 Mechanical Signal Data(현재 기계적인 신호상태) 를 반환한다.
	DWORD	__stdcall AxmStatusReadMechanical(long lAxisNo, DWORD *upStatus);
	
	// 지정 축의 현재 구동 속도를 읽어온다.
	DWORD	__stdcall AxmStatusReadVel(long lAxisNo, double *dpVel);
	
	// 지정 축의 Command Pos과 Actual Pos의 차를 반환한다.
	DWORD	__stdcall AxmStatusReadPosError(long lAxisNo, double *dpError);
	
	// 최종 드라이브로 이동하는(이동한) 거리를 확인 한다
	DWORD	__stdcall AxmStatusReadDriveDistance(long lAxisNo, double *dpUnit);

	// 지정 축의 Actual 위치를 설정한다.
	DWORD	__stdcall AxmStatusSetActPos(long lAxisNo, double dPos);
	// 지정 축의 Actual 위치를 반환한다.
	DWORD	__stdcall AxmStatusGetActPos(long lAxisNo, double *dpPos);

	// 지정 축의 Command 위치를 설정한다.
	DWORD	__stdcall AxmStatusSetCmdPos(long lAxisNo, double dPos);
	// 지정 축의 Command 위치를 반환한다.
	DWORD	__stdcall AxmStatusGetCmdPos(long lAxisNo, double *dpPos);

//======== 홈관련 함수=============================================================================================================================================================================================	

	// 지정 축의 Home 센서 Level 을 설정한다.
	// uLevel : LOW(0), HIGH(1)
	DWORD	__stdcall AxmHomeSetSignalLevel(long lAxisNo, DWORD uLevel);
	// 지정 축의 Home 센서 Level 을 반환한다.
	DWORD	__stdcall AxmHomeGetSignalLevel(long lAxisNo, DWORD *upLevel);
	// 현재 홈 신호 입력상태를 확인한다. 홈신호는 사용자가 임의로 AxmHomeSetMethod 함수를 이용하여 설정할수있다.
	// upStatus : OFF(0), ON(1)
	DWORD	__stdcall AxmHomeReadSignal(long lAxisNo, DWORD *upStatus);
	
    // 해당 축의 원점검색을 수행하기 위해서는 반드시 원점 검색관련 파라메타들이 설정되어 있어야 됩니다. 
	// 만약 MotionPara설정 파일을 이용해 초기화가 정상적으로 수행됐다면 별도의 설정은 필요하지 않다. 
    // 원점검색 방법 설정에는 검색 진행방향, 원점으로 사용할 신호, 원점센서 Active Level, 엔코더 Z상 검출 여부 등을 설정 한다.
	// 주의사항 : 레벨을 잘못 설정시 -방향으로 설정해도  +방향으로 동작할수 있으며, 홈을 찾는데 있어 문제가 될수있다.
    // (자세한 내용은 AxmMotSaveParaAll 설명 부분 참조)
	// 홈레벨은 AxmSignalSetHomeLevel 사용한다.
	// HClrTim : HomeClear Time : 원점 검색 Encoder 값 Set하기 위한 대기시간 
	// HmDir(홈 방향): 1 = +방향 , 1이하: -방향
	// HOffset - 원점검출후 이동거리.
	// uZphas: 1차 원점검색 완료 후 엔코더 Z상 검출 유무 설정(DISABLE(0), ENABLE(1))
	// HmSig : 0 -> +Limit
    //         1 -> -Limit
	//         4 -> 원점센서(범용 입력 0)
	DWORD   __stdcall AxmHomeSetMethod(long lAxisNo, long lHmDir, DWORD uHomeSignal, DWORD uZphas, double dHomeClrTime, double dHomeOffset);
	// 설정되어있는 홈 관련 파라메타들을 반환한다.
	DWORD   __stdcall AxmHomeGetMethod(long lAxisNo, long *lpHmDir, DWORD *upHomeSignal, DWORD *upZphas, double *dpHomeClrTime, double *dpHomeOffset);

	// 원점을 빠르고 정밀하게 검색하기 위해 여러 단계의 스탭으로 검출한다. 이때 각 스탭에 사용 될 속도를 설정한다. 
	// 이 속도들의 설정값에 따라 원점검색 시간과, 원점검색 정밀도가 결정된다. 
    // 각 스탭별 속도들을 적절히 바꿔가면서 각 축의 원점검색 속도를 설정하면 된다. 
    // (자세한 내용은 AxmMotSaveParaAll 설명 부분 참조)
	// 원점검색시 사용될 속도를 설정하는 함수
	// [dVelFirst]- 1차구동속도   [dVelSecond]-검출후속도   [dVelThird]- 마지막 속도  [dvelLast]- index검색및 정밀하게 검색하기위해. 
	// [dAccFirst]- 1차구동가속도 [dAccSecond]-검출후가속도 
	DWORD	__stdcall AxmHomeSetVel(long lAxisNo, double dVelFirst, double dVelSecond, double dVelThird, double dVelLast, double dAccFirst, double dAccSecond);
	// 설정되어있는 원점검색시 사용될 속도를 반환한다.
	DWORD	__stdcall AxmHomeGetVel(long lAxisNo, double *dpVelFirst, double *dpVelSecond, double *dpVelThird, double *dpVelLast, double *dpAccFirst, double *dpAccSecond);

	// 원점검색을 시작한다.
	// 원점검색 시작함수를 실행하면 라이브러리 내부에서 해당축의 원점검색을 수행 할 쓰레드가 자동 생성되어 원점검색을 순차적으로 수행한 후 자동 종료된다.
	DWORD	__stdcall AxmHomeSetStart(long lAxisNo);
	// 원점검색 결과를 사용자가 임의로 설정한다.
	// 원점검색 함수를 이용해 성공적으로 원점검색이 수행되고나면 검색 결과가 HOME_SUCCESS로 설정됩니다.
	// 이 함수는 사용자가 원점검색을 수행하지않고 결과를 임의로 설정할 수 있다. 
	DWORD	__stdcall AxmHomeSetResult(long lAxisNo, DWORD uHomeResult);
	// 원점검색 결과를 반환한다.
	// 원점검색 함수의 검색 결과를 확인한다. 원점검색이 시작되면 HOME_SEARCHING으로 설정되며 원점검색에 실패하면 실패원인이 설정된다. 실패 원인을 제거한 후 다시 원점검색을 진행하면 된다.
	DWORD	__stdcall AxmHomeGetResult(long lAxisNo, DWORD *upHomeResult);

	// 원점검색 진행률을 반환한다.
	// 원점검색 시작되면 진행율을 확인할 수 있다. 원점검색이 완료되면 성공여부와 관계없이 100을 반환하게 된다. 원점검색 성공여부는 GetHome Result함수를 이용해 확인할 수 있다.
	// upHomeMainStepNumber : Main Step 진행율이다. 
	// 겐트리 FALSE일 경우upHomeMainStepNumber : 0 일때면 선택한 축만 진행사항이고 홈 진행율은 upHomeStepNumber 표시한다.
	// 겐트리 TRUE일 경우 upHomeMainStepNumber : 0 일때면 마스터 홈을 진행사항이고 마스터 홈 진행율은 upHomeStepNumber 표시한다.
    // 겐트리 TRUE일 경우 upHomeMainStepNumber : 10 일때면 슬레이브 홈을 진행사항이고 마스터 홈 진행율은 upHomeStepNumber 표시한다.
    // upHomeStepNumber     : 선택한 축에대한 진행율을 표시한다. 
    // 겐트리 FALSE일 경우  : 선택한 축만 진행율을 표시한다.
	// 겐트리 TRUE일 경우 마스터축, 슬레이브축 순서로 진행율을 표시된다.
	DWORD	__stdcall AxmHomeGetRate(long lAxisNo, DWORD *upHomeMainStepNumber, DWORD *upHomeStepNumber);

//=======추가 홈 서치 (QI에만 해당됨.)=================================================================================

	// 사용자가 지정한 축의 홈설정 파라메타를 설정한다.(QI칩 전용 레지스터 이용).
	// uZphasCount : 홈 완료후에 Z상 카운트(0 - 15)
	// lHomeMode   : 홈 설정 모드( 0 - 12)
	// lClearSet   : 위치 클리어 , 잔여펄스 클리어 사용 선택 (0 - 3)
	//               0: 위치클리어 사용않함, 잔여펄스 클리어 사용 안함
	//				 1: 위치클리어 사용함, 잔여펄스 클리어 사용 안함
	//               2: 위치클리어 사용안함, 잔여펄스 클리어 사용함
	//               3: 위치클리어 사용함, 잔여펄스 클리어 사용함.
	// dOrgVel : 홈관련 Org  Speed 설정 
	// dLastVel: 홈관련 Last Speed 설정 
	DWORD	__stdcall AxmHomeSetConfig(long lAxisNo, DWORD uZphasCount, long lHomeMode, long lClearSet, double dOrgVel, double dLastVel, double dLeavePos);
	// 사용자가 지정한 축의 홈설정 파라메타를 반환한다.
	DWORD	__stdcall AxmHomeGetConfig(long lAxisNo, DWORD *upZphasCount, long *lpHomeMode, long *lpClearSet, double *dpOrgVel, double *dpLastVel, double *dpLeavePos); //KKJ(070215)
	
	// 사용자가 지정한 축의 홈 서치를 시작한다.
	// lHomeMode 사용시 설정 : 0 - 5 설정 (Move Return후에 Search를  시작한다.)
	// lHomeMode -1로 그대로 사용시 HomeConfig에서 사용한대로 그대로 설정됨.
	// 구동방향      : Vel값이 양수이면 CW, 음수이면 CCW.
	DWORD	__stdcall AxmHomeSetMoveSearch(long lAxisNo, double dVel, double dAccel, double dDecel);

	// 사용자가 지정한 축의 홈 리턴을 시작한다.
	// lHomeMode 사용시 설정 : 0 - 12 설정 
	// lHomeMode -1로 그대로 사용시 HomeConfig에서 사용한대로 그대로 설정됨.
	// 구동방향      : Vel값이 양수이면 CW, 음수이면 CCW.
	DWORD	__stdcall AxmHomeSetMoveReturn(long lAxisNo, double dVel, double dAccel, double dDecel);
	
	// 사용자가 지정한 축의 홈 이탈을 시작한다.
	// 구동방향      : Vel값이 양수이면 CW, 음수이면 CCW.
	DWORD	__stdcall AxmHomeSetMoveLeave(long lAxisNo, double dVel, double dAccel, double dDecel);

	// 사용자가 지정한 다축의 홈 서치을 시작한다.
	// lHomeMode 사용시 설정 : 0 - 5 설정 (Move Return후에 Search를  시작한다.)
	// lHomeMode -1로 그대로 사용시 HomeConfig에서 사용한대로 그대로 설정됨.
	// 구동방향      : Vel값이 양수이면 CW, 음수이면 CCW.
	DWORD	__stdcall AxmHomeSetMultiMoveSearch(long lArraySize, long *lpAxesNo, double *dpVel, double *dpAccel, double *dpDecel);

//========= 위치 구동함수 ===============================================================================================================
	
	// 설정 속도 단위가 RPM(Revolution Per Minute)으로 맞추고 싶다면.
    // ex>	rpm 계산:
	// 4500 rpm ?
	// unit/ pulse = 1 : 1이면      pulse/ sec 초당 펄스수가 되는데
    // 4500 rpm에 맞추고 싶다면     4500 / 60 초 : 75회전/ 1초
    // 모터가 1회전에 몇 펄스인지 알아야 된다. 이것은 Encoder에 Z상을 검색해보면 알수있다.
	// 1회전:1800 펄스라면 75 x 1800 = 135000 펄스가 필요하게 된다.
	// AxmMotSetMoveUnitPerPulse에 Unit = 1, Pulse = 1800 넣어 동작시킨다. 

	// 설정한 거리만큼 또는 위치까지 이동한다.
	// 지정 축의 절대 좌표/ 상대좌표 로 설정된 위치까지 설정된 속도와 가속율로 구동을 한다.
    // 속도 프로파일은 AxmMotSetProfileMode 함수에서 설정한다.
	// 펄스가 출력되는 시점에서 함수를 벗어난다.
	DWORD	__stdcall AxmMoveStartPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel);

	// 설정한 거리만큼 또는 위치까지 이동한다.
	// 지정 축의 절대 좌표/상대좌표로 설정된 위치까지 설정된 속도와 가속율로 구동을 한다.
    // 속도 프로파일은 AxmMotSetProfileMode 함수에서 설정한다. 
	// 펄스 출력이 종료되는 시점에서 함수를 벗어난다
	DWORD	__stdcall AxmMovePos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel);

	// 설정한 속도로 구동한다.
	// 지정 축에 대하여 설정된 속도와 가속율로 지속적으로 속도 모드 구동을 한다. 
	// 펄스 출력이 시작되는 시점에서 함수를 벗어난다.
	// Vel값이 양수이면 CW, 음수이면 CCW 방향으로 구동.
	DWORD	__stdcall AxmMoveVel(long lAxisNo, double dVel, double dAccel, double dDecel);

	// 지정된 다축에 대하여 설정된 속도와 가속율로 지속적으로 속도 모드 구동을 한다.
	// 펄스 출력이 시작되는 시점에서 함수를 벗어난다.
	// CAMC-QI 만 함수사용가능.
	// Vel값이 양수이면 CW, 음수이면 CCW 방향으로 구동.
	DWORD	__stdcall AxmMoveStartMultiVel(long lArraySize, long *lpAxesNo, double *dpVel, double *dpAccel, double *dpDecel);

	// 특정 Input 신호의 Edge를 검출하여 즉정지 또는 감속정지하는 함수.
	// lDetect Signal : edge 검출할 입력 신호 선택.
	// lDetectSignal  : PosEndLimit(0), NegEndLimit(1), HomeSensor(4), EncodZPhase(5), UniInput02(6), UniInput03(7)
	// Signal Edge   : 선택한 입력 신호의 edge 방향 선택 (rising or falling edge).
	//                  SIGNAL_DOWN_EDGE(0), SIGNAL_UP_EDGE(1)
	// 구동방향      : Vel값이 양수이면 CW, 음수이면 CCW.
	// SignalMethod  : 급정지 EMERGENCY_STOP(0), 감속정지 SLOWDOWN_STOP(1)
	// 주의사항: SignalMethod를 EMERGENCY_STOP(0)로 사용할경우 가감속이 무시되며 지정된 속도로 가속 급정지하게된다.
    //           QI를 사용할 경우 lDetectSignal이 PosEndLimit , NegEndLimit(0,1) 을 찾을경우 신호의레벨 Active 상태를 검출하게된다.
	DWORD	__stdcall AxmMoveSignalSearch(long lAxisNo, double dVel, double dAccel, long lDetectSignal, long lSignalEdge, long lSignalMethod);
    
	// 지정 축에서 설정된 신호를 검출하고 그 위치를 저장하기 위해 이동하는 함수이다.
	// 원하는 신호를 골라 찾아 움직이는 함수 찾을 경우 그 위치를 저장시켜놓고 AxmGetCapturePos사용하여 그값을 읽는다.
	// Signal Edge   : 선택한 입력 신호의 edge 방향 선택 (rising or falling edge).
	//                 SIGNAL_DOWN_EDGE(0), SIGNAL_UP_EDGE(1)
	// 구동방향      : Vel값이 양수이면 CW, 음수이면 CCW.
	// SignalMethod  : 급정지 EMERGENCY_STOP(0), 감속정지 SLOWDOWN_STOP(1)
	// lDetect Signal: edge 검출할 입력 신호 선택.
	// lDetectSignal : PosEndLimit(0), NegEndLimit(1), HomeSensor(4), EncodZPhase(5), UniInput02(6), UniInput03(7)
    // lTarget       : COMMAND(0), ACTUAL(1)
	// 주의사항: SignalMethod를 EMERGENCY_STOP(0)로 사용할경우 가감속이 무시되며 지정된 속도로 가속 급정지하게된다.
	//           QI를 사용할 경우 lDetectSignal이 PosEndLimit , NegEndLimit(0,1) 을 찾을경우 신호의레벨 Active 상태를 검출하게된다.
	DWORD	__stdcall AxmMoveSignalCapture(long lAxisNo, double dVel, double dAccel, long lDetectSignal, long lSignalEdge, long lTarget, long lSignalMethod);
    // 'AxmMoveSignalCapture' 함수에서 저장된 위치값을 확인하는 함수이다.
	DWORD	__stdcall AxmMoveGetCapturePos(long lAxisNo, double *dpCapPotition);

	// "설정한 거리만큼 또는 위치까지 이동하는 함수.
    // 함수를 실행하면 해당 Motion 동작을 시작한 후 Motion 이 완료될때까지 기다리지 않고 바로 함수를 빠져나간다."
	DWORD	__stdcall AxmMoveStartMultiPos(long lArraySize, long *lpAxisNo, double *dpPos, double *dpVel, double *dpAccel, double *dpDecel);
	
    // 다축을 설정한 거리만큼 또는 위치까지 이동한다.
	// 지정 축들의 절대 좌표로 설정된 위치까지 설정된 속도와 가속율로 구동을 한다.
	DWORD	__stdcall AxmMoveMultiPos(long lArraySize, long *lpAxisNo, double *dpPos, double *dpVel, double *dpAccel, double *dpDecel);

	// 지정 축을 설정한 감속도로 감속 정지 한다.
	// dDecel : 정지 시 감속율값
	DWORD	__stdcall AxmMoveStop(long lAxisNo, double dDecel);
	// 지정 축을 급 정지 한다.
	DWORD	__stdcall AxmMoveEStop(long lAxisNo);
	// 지정 축을 감속 정지한다.
	DWORD	__stdcall AxmMoveSStop(long lAxisNo);

//========= 오버라이드 함수 ============================================================================

	// 위치 오버라이드 한다.
	// 지정 축의 구동이 종료되기 전 지정된 출력 펄스 수를 조정한다.
	// QI사용시주의사항: 여기서 오버라이드할 위치를 넣을때는 가는방향에 이동거리는 Relative 형태의 
	//                   위치값으로 들어가기 때문에 위치는 반드시 Relative 형태의 위치값으로 넣어준다.
	DWORD	__stdcall AxmOverridePos(long lAxisNo, double dOverridePos);
	
    // 지정 축의 속도오버라이드 하기전에 오버라이드할 최고속도를 설정한다.
   	// 주의점 : 속도오버라이드를 5번한다면 그중에 최고 속도를 설정해야된다. 
	DWORD	__stdcall AxmOverrideSetMaxVel(long lAxisNo, double dOverrideMaxVel);
	
	// 속도 오버라이드 한다.
	// 지정 축의 구동 중에 속도를 가변 설정한다. (반드시 모션 중에 가변 설정한다.)
	// 주의점: AxmOverrideVel 함수를 사용하기전에. AxmOverrideMaxVel 최고로 설정할수있는 속도를 설정해놓는다.
	// EX> 속도오버라이드를 두번한다면 
	// 1. 두개중에 높은 속도를 AxmOverrideMaxVel 설정 최고 속도값 설정.
	// 2. AxmMoveStartPos 실행 지정 축의 구동 중(Move함수 모두 포함)에 속도를 첫번째 속도로 AxmOverrideVel 가변 설정한다.
	// 3. 지정 축의 구동 중(Move함수 모두 포함)에 속도를 두번째 속도로 AxmOverrideVel 가변 설정한다.
	DWORD	__stdcall AxmOverrideVel(long lAxisNo, double dOverrideVel);
	
	// 어느 시점에서 속도 오버라이드 한다.
	// 어느 위치 지점과 오버라이드할 속도를 입력시켜 그위치에서 속도오버라이드 되는 함수
	// lTarget : COMMAND(0), ACTUAL(1)
	DWORD	__stdcall AxmOverrideVelAtPos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel,double dOverridePos, double dOverrideVel, long lTarget);
	
//========= 마스터, 슬레이브  기어비로 구동 함수 ===========================================================================

	// Electric Gear 모드에서 Master 축과 Slave 축과의 기어비를 설정한다.
	// uStopMode   : 급정지 EMERGENCY_STOP(0), 감속정지 SLOWDOWN_STOP(1)
	// dSlaveRatio : 마스터축에 대한 슬레이브의 기어비( 0 : 0% , 0.5 : 50%, 1 : 100%)
	DWORD	__stdcall AxmLinkSetMode(long lMasterAxisNo, long lSlaveAxisNo, double dSlaveRatio);
	// Electric Gear 모드에서 설정된 Master 축과 Slave 축과의 기어비를 반환한다.
	DWORD	__stdcall AxmLinkGetMode(long lMasterAxisNo, long lSlaveAxisNo, double *dpGearRatio);
	// Master 축과 Slave축간의 전자기어비를 설정 해제 한다.
	DWORD	__stdcall AxmLinkResetMode(long lMasterAxisNo, long lSlaveAxisNo);

//======== 겐트리 관련 함수===========================================================================================================================================================
    // 모션모듈은 두 축이 기구적으로 Link되어있는 겐트리 구동시스템 제어를 지원한다. 
	// 이 함수를 이용해 Master축을 겐트리 제어로 설정하면 해당 Slave축은 Master축과 동기되어 구동됩니다. 
	// 만약 겐트리 설정 이후 Slave축에 구동명령이나 정지 명령등을 내려도 모두 무시됩니다.
	// uSlHomeUse     : 슬레이축 홈사용 우뮤 ( 0 - 2)
	//             (0 : 슬레이브축 홈을 사용안하고 마스터축을 홈을 찾는다.)
	//             (1 : 마스터축 , 슬레이브축 홈을 찾는다. 슬레이브 dSlOffset 값 적용해서 보정함.)
	//             (2 : 마스터축 , 슬레이브축 홈을 찾는다. 슬레이브 dSlOffset 값 적용해서 보정안함.)
	// dSlOffset      : 슬레이브축 옵셋값
	// dSlOffsetRange : 슬레이브축 옵셋값 레인지 설정
	// QI사용시주의사항: 갠트리 ENABLE시 슬레이브축은 모션중 AxmStatusReadMotion 함수로 확인하면 InMotion 중으로 True로 확인되야 정상동작이다. 
	//                   슬레이브축에 AxmStatusReadMotion로 확인했을때 InMotion 이 False되면 Gantry Enable이 안된것이므로 알람이 혹은 리밋트를 치고있는지 확인한다.

	DWORD	__stdcall AxmGantrySetEnable(long lMasterAxisNo, long lSlaveAxisNo, DWORD uSlHomeUse, double dSlOffset, double dSlOffsetRange);

    // Slave축의 Offset값을 알아내는방법.
    // A. 마스터, 슬레이브를 두개다 서보온을 시킨다. 		
    // B. AxmGantrySetEnable함수에서 uSlHomeUse = 2로 설정후 AxmHomeSetStart함수를 이용해서 홈을 찾는다. 
    // C. 홈을 찾고 나면 마스터축의 Command값을 읽어보면 마스터축과 슬레이브축의 틀어진 Offset값을 볼수있다.
    // D. Offset값을 읽어서 AxmGantrySetEnable함수의 dSlOffset인자에 넣어준다. 
	// E. dSlOffset값을 넣어줄때 마스터축에 대한 슬레이브 축 값이기때문에 부호를 반대로 -dSlOffset 넣어준다.
    // F. dSIOffsetRange 는 Slave Offset의 Range 범위를 말하는데 Range의 한계를 지정하여 한계를 벗어나면 에러를 발생시킬때 사용한다.		
    // G. AxmGantrySetEnable함수에 Offset값을 넣어줬으면  AxmGantrySetEnable함수에서 uSlHomeUse = 1로 설정후 AxmHomeSetStart함수를 이용해서 홈을 찾는다. 		
    
	// 겐트리 구동에 있어 사용자가 설정한 파라메타를 반환한다.
	DWORD	__stdcall AxmGantryGetEnable(long lMasterAxisNo, DWORD *upSlHomeUse, double *dpSlOffset, double *dpSlORange, DWORD *upGatryOn);
	// 모션 모듈은 두 축이 기구적으로 Link되어있는 겐트리 구동시스템 제어를 해제한다.
	DWORD	__stdcall AxmGantrySetDisable(long lMasterAxisNo, long lSlaveAxisNo);

//====일반 보간함수 ============================================================================================================================================;

	// 직선 보간 한다.
	// 시작점과 종료점을 지정하여 다축 직선 보간 구동하는 함수이다. 구동 시작 후 함수를 벗어난다.
    // AxmContiBeginNode, AxmContiEndNode와 같이사용시 지정된 좌표계에 시작점과 종료점을 지정하여 직선 보간 구동하는 Queue에 저장함수가된다. 
	// 직선 프로파일 연속 보간 구동을 위해 내부 Queue에 저장하여 AxmContiStart함수를 사용해서 시작한다.
	DWORD	__stdcall AxmLineMove(long lCoord, double *dpEndPos, double dVel, double dAccel, double dDecel);

	// IP : 사용안됨 , QI: 사용가능.
	// 시작점(x,y,z) , 중간점(x,y,z) , 끝점(x,y,z) , 중간점에서의 거리(d) 를 넣고 직선보간을 이용하여 원호보간을 한다. 
	// lAxisNo              : 2축 번호 , 3축 번호  배열로 넣는다.
	// dPosA , dPosB, dPosC : 위치 x,y,z 값 배열로 넣는다.
    // dDistance            : 중심점에서 A, C 위치 방향으로의 거리. 
	// lArcDivNum           : 원호를 직선으로 잘라지는 갯수.
	// 주의점 : Pos값은 반드시 3개 위치를 넣는다. 직선 2축일때는 x, y값만 넣고 z값을 0으로 놓는다.
    //        : 반드시 AxmContiBeginNode, AxmContiEndNode, AxmContiStart와 같이사용한다. 단위보간구동 사용안됨.
	DWORD   __stdcall AxmLineArcMove(long lCoord, long *lAxisNo, double *dPosA, double *dPosB, double *dPosC, double dDistance, long lArcDivNum, double dVel, double dAccel, double dDecel);

	// 2축 원호보간 한다.
	// 시작점, 종료점과 중심점을 지정하여 원호 보간 구동하는 함수이다. 구동 시작 후 함수를 벗어난다.
    // AxmContiBeginNode, AxmContiEndNode, 와 같이사용시 지정된 좌표계에 시작점, 종료점과 중심점을 지정하여 구동하는 원호 보간 Queue에 저장함수가된다.
	// 프로파일 원호 연속 보간 구동을 위해 내부 Queue에 저장하여 AxmContiStart함수를 사용해서 시작한다.
    // lAxisNo = 두축 배열 , dCenterPos = 중심점 X,Y 배열 , dEndPos = 종료점 X,Y 배열.
	DWORD   __stdcall AxmCircleCenterMove(long lCoord, long *lAxisNo, double *dCenterPos, double *dEndPos, double dVel, double dAccel, double dDecel, DWORD uCWDir);

    // 중간점, 종료점을 지정하여 원호 보간 구동하는 함수이다. 구동 시작 후 함수를 벗어난다.
    // AxmContiBeginNode, AxmContiEndNode와 같이사용시 지정된 좌표계에 중간점, 종료점을 지정하여 구동하는 원호 보간 Queue에 저장함수가된다.
	// 프로파일 원호 연속 보간 구동을 위해 내부 Queue에 저장하여 AxmContiStart함수를 사용해서 시작한다.
    // lAxisNo = 두축 배열 , dMidPos = 중간점 X,Y 배열 , dEndPos = 종료점 X,Y 배열, lArcCircle = 아크(0), 원(1)

	DWORD	__stdcall AxmCirclePointMove(long lCoord, long *lAxisNo, double *dMidPos, double *dEndPos, double dVel, double dAccel, double dDecel, long lArcCircle);
	
	// 시작점, 종료점과 반지름을 지정하여 원호 보간 구동하는 함수이다. 구동 시작 후 함수를 벗어난다.
    // AxmContiBeginNode, AxmContiEndNode와 같이사용시 지정된 좌표계에 시작점, 종료점과 반지름을 지정하여 원호 보간 구동하는 Queue에 저장함수가된다.
	// 프로파일 원호 연속 보간 구동을 위해 내부 Queue에 저장하여 AxmContiStart함수를 사용해서 시작한다.
    // lAxisNo = 두축 배열 , dRadius = 반지름, dEndPos = 종료점 X,Y 배열 , uShortDistance = 작은원(0), 큰원(1)

	DWORD   __stdcall AxmCircleRadiusMove(long lCoord, long *lAxisNo, double dRadius, double *dEndPos, double dVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance);

	// 시작점, 회전각도와 반지름을 지정하여 원호 보간 구동하는 함수이다. 구동 시작 후 함수를 벗어난다.
    // AxmContiBeginNode, AxmContiEndNode와 같이사용시 지정된 좌표계에 시작점, 회전각도와 반지름을 지정하여 원호 보간 구동하는 Queue에 저장함수가된다.
	// 프로파일 원호 연속 보간 구동을 위해 내부 Queue에 저장하여 AxmContiStart함수를 사용해서 시작한다.
    // lAxisNo = 두축 배열 , dCenterPos = 중심점 X,Y 배열 , dAngle = 각도.

	DWORD   __stdcall AxmCircleAngleMove(long lCoord, long *lAxisNo, double *dCenterPos, double dAngle, double dVel, double dAccel, double dDecel, DWORD uCWDir);

//====연속 보간 함수 ============================================================================================================================================;
	//지정된 좌표계에 연속보간 축 맵핑을 설정한다.
	//(축맵핑 번호는 0 부터 시작))
	// 주의점: 축맵핑할때는 반드시 작은 숫자부터 큰숫자를 넣는다.
	//         여기서 제일 작은숫자 축이 마스터가 된다.
	DWORD	__stdcall AxmContiSetAxisMap(long lCoord, long lSize, long *lpRealAxesNo);
	//지정된 좌표계에 연속보간 축 맵핑을 반환한다.
	DWORD	__stdcall AxmContiGetAxisMap(long lCoord, long *lpSize, long *lpRealAxesNo);
	
	// IP : 사용안됨. QI : 전용함수.
   	// Unit/Pulse를 다르게 설정할경우 지정좌표계의 지정축의 속도를 반환한다.
	// dpInVel  : 지정 좌표계의 사용자 하고자하는 속도 [입력으로 사용]
	// dpInPos  : 지정 좌표계의 위치 리스트 배열 [ 입력으로 사용 ]
    // dpOutVel : 지정 좌표계의 실제 사용되는 속도 값 [ 출력으로 사용 ]
	DWORD	__stdcall AxmContiGetMaxVel(long lCoord, long lAxisNo, double dpInVel, double *dpInPos, double *dpOutVel);

	// 지정된 좌표계에 연속보간 축 절대/상대 모드를 설정한다.
	// (주의점 : 반드시 축맵핑 하고 사용가능)
	// 지정 축의 이동 거리 계산 모드를 설정한다.
    //uAbsRelMode : '0' - 절대 좌표계
    //              '1' - 상대 좌표계
	DWORD	__stdcall AxmContiSetAbsRelMode(long lCoord, DWORD uAbsRelMode);
    // 지정된 좌표계에 연속보간 축 절대/상대 모드를 반환한다.
	DWORD	__stdcall AxmContiGetAbsRelMode(long lCoord, DWORD *upAbsRelMode);

	//지정된 좌표계의 구동 속도 프로파일 모드를 설정한다.
	// (주의점 : 반드시 축맵핑 하고 사용가능)
    // ProfileMode : '0' - 대칭 Trapezode
    //               '1' - 비대칭 Trapezode
    //               '2' - 대칭 Quasi-S Curve
    //               '3' - 대칭 S Curve
    //               '4' - 비대칭 S Curve
	DWORD	__stdcall AxmContiSetProfileMode(long lCoord, DWORD uProfileMode);
	// 지정된 좌표계의 구동 속도 프로파일 모드를 반환한다.
	DWORD	__stdcall AxmContiGetProfileMode(long lCoord, DWORD *upProfileMode);
	// 지정된 좌표계에 보간 구동을 위한 내부 Queue가 비어 있는지 확인하는 함수이다.
	DWORD	__stdcall AxmContiReadFree(long lCoord, DWORD *upQueueFree);
	// 지정된 좌표계에 보간 구동을 위한 내부 Queue에 저장되어 있는 보간 구동 개수를 확인하는 함수이다.
	DWORD	__stdcall AxmContiReadIndex(long lCoord, long *lpQueueIndex);
	// 지정된 좌표계에 연속 보간 구동을 위해 저장된 내부 Queue를 모두 삭제하는 함수이다.
	DWORD	__stdcall AxmContiWriteClear(long lCoord);

	// 지정된 좌표계에 연속보간에서 수행할 작업들의 등록을 시작한다. 이함수를 호출한후,
    // AxmContiEndNode함수가 호출되기 전까지 수행되는 모든 모션작업은 실제 모션을 수행하는 것이 아니라 연속보간 모션으로 등록 되는 것이며,
    // AxmContiStart 함수가 호출될 때 비로소 등록된모션이 실제로 수행된다.
    DWORD	__stdcall AxmContiBeginNode(long lCoord);
    // 지정된 좌표계에서 연속보간을 수행할 작업들의 등록을 종료한다.
	DWORD	__stdcall AxmContiEndNode(long lCoord);

	// 연속 보간 시작 한다.
	// CAMC-IP dwProfileset, lAngle 인자값 변수에 0을 넣어서 사용 한다. 
	// CAMC-QI dwProfileset이 0이면 연속 보간 사용
	//                        1이면 프로파일 보간 사용설정 
	// CAMC-QI dwProfileset이 2이면 자동 프로파일 보간 사용.
	DWORD	__stdcall AxmContiStart(long lCoord, DWORD dwProfileset, long lAngle); 
	// 지정된 좌표계에 연속 보간 구동 중인지 확인하는 함수이다.
	DWORD	__stdcall AxmContiIsMotion(long lCoord, DWORD *upInMotion);
	// 지정된 좌표계에 연속 보간 구동 중 현재 구동중인 연속 보간 인덱스 번호를 확인하는 함수이다.
	DWORD	__stdcall AxmContiGetNodeNum(long lCoord, long *lpNodeNum);
	// 지정된 좌표계에 설정한 연속 보간 구동 총 인덱스 갯수를 확인하는 함수이다.
	DWORD	__stdcall AxmContiGetTotalNodeNum(long lCoord, long *lpNodeNum);

	//----- 연속보간 하면서 속도오버라이드, 트리거, 인터럽트, 디지털 OUT 출력 설정 함수.
    //----------- 리스트에 미리 넣어놓고 구동한다..--------------------------------------------------------------------------------------------
	// AxmWriteContiClear 함수를 사용해서 리스트에 쌓인 데이타를 지울수있다.

    //============================= CAMC-IP ==========================================================//
	// (IP는 연속보간실행전에 직선보간, 원호보간 노드마다 속도를 다르게 넣으면 속도를 변경가능안함)  	
	// (주의점 : 반드시 축맵핑 하고 사용가능)
	// dVel : 오버라이드 할 속도값 -> IP만 해당됨
	// lDioOutNum: 출력할 out 값 헥사값으로 표시  ex> 0xf 면 1111 범용출력이 0,1,2,3 한번에 나감
	// (참고로 범용 출력 0은 서보온으로 사용) 서보모터같은경우 고려해야됨
	// lNodeNum  : 보간 노드 번호. 
	// IP 는 lTargetAxis 설정 (0 - 1) 실행할 축을 의미한다.
	
	// 내부 제어기 펄스 선택
	// lTarget는 직선 노드 일때 COMMAND(0)선택 
	// lTarget는 원호 노드 일때 COMMAND(0)선택

	// 외부 카운터 설정(장비 실제 Encoder값)
	// lTarget는 직선 노드 일때 Actual(1) = Encoder(외부위치카운터) 을 선택한다.
	// lTarget는 원호 노드 일때 Actual(2,3,4) = Encoder(외부위치카운터) 로 제어하고싶을때  
	// 외부위치카운터(Encoder) < (노드 위치값)(2)
	// 외부위치카운터(Encoder) = (노드 위치값)(3)
	// 외부위치카운터(Encoder) > (노드 위치값)(4)

	// IP는 dNodePlusAct1 는 사용안함
	// IP는 dNodePlusAct2 는 사용안함

	// lSelect 옵션
	// 0  : DISABLE (사용안함)
	// 1  : 속도오버라이드 사용
	// 2  : 트리거 사용
	// 3  : 인터럽트 사용
	// 4  : DO 출력  사용
	// 5  : 동시에 속도오버라이드, 트리거 사용
	// 6  : 동시에 속도오버라이드, 인터럽트 사용
	// 7  : 동시에 속도오버라이드, DO 출력 사용
	// 8  : 동시에 트리거, 인터럽트 사용
	// 9  : 동시에 트리거, DO 출력 사용
	// 10 : 동시에 인터럽트, DO 출력 사용
	// 11 : 동시에 속도오버라이드, 트리거, 인터럽트 사용
	// 12 : 동시에 속도오버라이드, 트리거, DO 출력 사용
	// 13 : 동시에 속도오버라이드, 인터럽트, DO 출력 사용
	// 14 : 동시에 트리거, 인터럽트, DO 출력 사용
	// 15 : 동시에 속도오버라이드, 트리거, 인터럽트, DO 출력 사용

	//============================= CAMC-QI ==========================================================//
    //( QI는 연속보간실행전에 직선보간, 원호보간 노드마다 속도를 다르게 넣으면 속도를 변경가능하다)
	// (주의점 : 반드시 축맵핑 하고 사용가능)
	// dVel : 0 으로 설정(다른값을 설정해도 의미없음)
	// lDioOutNum: 출력할 out 값 헥사값으로 표시  ex> 0xf 면 1111 범용출력이 0,1,2,3 한번에 나감
	// (참고로 범용 출력 0은 서보온으로 사용) 서보모터같은경우 고려해야됨
	// lNodeNum  : 보간 노드 번호. 
	// QI 는 lTargetAxis 설정 (0 - 3) 실행할 축을 의미한다.
	
	// 내부 제어기 펄스 선택
	// lTarget는 직선 노드 일때 COMMAND(0)선택
	// lTarget는 원호 노드 일때 COMMAND(0)선택

	// 외부 카운터 설정(장비 실제 Encoder값)
	// lTarget는 직선 노드 일때 Actual(1) = Encoder(외부위치카운터) 을 선택한다.
	// lTarget는 원호 노드 일때 Actual(2,3,4,5,6,7,8,9) = Encoder(외부위치카운터) 로 제어하고싶을때  
	// 외부위치카운터(Encoder) < (노드 위치값)(2)
	// 외부위치카운터(Encoder) = (노드 위치값)(3)
	// 외부위치카운터(Encoder) > (노드 위치값)(4)
	// 외부위치카운터(Encoder) ≤ (노드 위치값)(5)
	// 외부위치카운터(Encoder) ≥ (노드 위치값)(6)
	// 외부위치카운터(Encoder) = (노드 위치값)(7) Comparater event during counting up.
	// 외부위치카운터(Encoder) = (노드 위치값)(8) Comparater event during counting down.
	// 외부위치카운터(Encoder) > (노드 위치값)(9) Counter is same with boundary value.
	
	// QI는 dNodePlusAct1 는 ACTUAL 사용시 위치비교기1 Encoder값 
	// QI는 dNodePlusAct2 는 ACTUAL 사용시 위치비교기2 Encoder값 

	// lSelect 옵션
	// 0  : DISABLE (사용안함)
	// 1  : 트리거 사용
	// 2  : 인터럽트 사용
	// 3  : DO 출력  사용
	// 4  : 동시에 트리거, 인터럽트 사용
	// 5  : 동시에 트리거, DO 출력 사용
	// 6  : 동시에 인터럽트, DO 출력 사용
	// 7  : 동시에 트리거, 인터럽트, DO 출력 사용
	//===========================================================================================//

	// 트리거 사용시 주의점
	// 트리거 사용시 반드시 AxmContiSetOptionNodeNum 함수 사용하기전에 
	// AxmTriggerSetTimeLevel 에서 트리거 출력 레벨 을 반드시 설정한다.
	// 인터럽트 사용시 주의점.
	// 인터럽트 사용시 반드시 AxmContiSetOptionNodeNum 함수 사용하기전에 
	// AxmInterruptSetAxis, AxmInterruptSetAxisEnable 메세지,콜백, 이벤트 방식 설정 필요하다.
	// lComparatorAxis : ACTUAL(Encoder값 읽을축) 축 선택.(X,Y축 두축만 가능)
	//                 : COMMAND로 사용시 무시.

	// 지정된 좌표계에 연속 보간 구동 중 특정 위치에서 DO출력, 인터럽트, 트리거출력을 설정한다.
	DWORD	__stdcall AxmContiSetOptionNodeNum(long lCoord, long lDioOutNum, long lNodeNum, long lSelect, long lTarget, long lTargetAxis, double dVel, double dNodePlusAct1, double dNodePlusAct2, long lComparatorAxis);

//====================트리거 함수 ===============================================================================================================================

	// 지정 축에 트리거 기능의 사용 여부, 출력 레벨, 위치 비교기, 트리거 신호 지속 시간 및 트리거 출력 모드를 설정한다.
	//  dTrigTime  : 트리거 출력 시간 
	//                IP: 1usec - 최대 4msec ( 1 - 4000 까지 설정)
    //                QI: 1usec - 최대 50msec ( 1 - 50000 까지 설정)
	//  upTriggerLevel  : 사용 유무           => LOW(0), HIGH(1), UNUSED(2), USED(3)
    //  uSelect         : 사용할 기준 위치    => COMMAND(0), ACTUAL(1)
	//  uInterrupt      : 인터럽트 설정       => DISABLE(0), ENABLE(1)
    
	// 지정 축에 트리거 신호 지속 시간 및 트리거 출력 레벨, 트리거 출력방법을 설정한다.
 	DWORD	__stdcall AxmTriggerSetTimeLevel(long lAxisNo, double dTrigTime, DWORD uTriggerLevel, DWORD uSelect, DWORD uInterrupt);
    // 지정 축에 트리거 신호 지속 시간 및 트리거 출력 레벨, 트리거 출력방법을 반환한다.
	DWORD	__stdcall AxmTriggerGetTimeLevel(long lAxisNo, double *dpTrigTime, DWORD *upTriggerLevel, DWORD *upSelect, DWORD *upInterrupt);
	
	//  uMethod : //  0x0 : 트리거 위치값을 사용한 주기 트리거 방식
				  //  0x1 : 트리거 절대 위치에서 트리거 발생, 절대 위치 방식
    //  dPos : 주기 선택시 : 위치마다위치마다 출력하기때문에 그 위치
	//         절대 선택시 : 출력할 그 위치, 이 위치와같으면 무조건 출력이 나간다. 
	DWORD	__stdcall AxmTriggerSetAbsPeriod(long lAxisNo, DWORD uMethod, double dPos);
	// 지정 축에 트리거 기능의 사용 여부, 출력 레벨, 위치 비교기, 트리거 신호 지속 시간 및 트리거 출력 모드를 반환한다.
	DWORD	__stdcall AxmTriggerGetAbsPeriod(long lAxisNo, DWORD *upMethod, double *dpPos);

	//  사용자가 지정한 시작위치부터 종료위치까지 일정구간마다 트리거를 출력 한다.
	DWORD	__stdcall AxmTriggerSetBlock(long lAxisNo, double dStartPos, double dEndPos, double dPeriodPos);
	// 'AxmTriggerSetBlock' 함수의 트리거 설정한 값을 읽는다..
	DWORD	__stdcall AxmTriggerGetBlock(long lAxisNo, double *dpStartPos, double *dpEndPos, double *dpPeriodPos);
	// 사용자가 한 개의 트리거 펄스를 출력한다.
	DWORD	__stdcall AxmTriggerOneShot(long lAxisNo);
	// 사용자가 한 개의 트리거 펄스를 몇초후에 출력한다.
	DWORD	__stdcall AxmTriggerSetTimerOneshot(long lAxisNo, long lmSec);
	// 절대위치 트리거 무한대 절대위치 출력한다.
	DWORD	__stdcall AxmTriggerOnlyAbs(long lAxisNo,long lTrigNum, double* dpTrigPos);
	// 트리거 설정을 리셋한다.
	DWORD	__stdcall AxmTriggerSetReset(long lAxisNo); 

//======== CRC( 잔여 펄스 클리어 함수)=====================================================================	
	//Level   : IP: LOW(0), HIGH(1), UNUSED(2), USED(3) 
    //		    QI: LOW(0), HIGH(1) 레벨만 설정가능. 사용가능 AxmCrcSetEndLimit 함수 사용.
	//uMethod : 잔여펄스 제거 출력 신호 펄스 폭 0 - 6까지 설정가능.(QI만 사용, IP사용안함)
	//          0 : 10 uSec , 1 : 100 uSec, 2: 500 uSec, 3:1 mSec, 4:10 mSec, 5:50 mSec, 6:100 mSec

	//지정 축에 CRC 신호 사용 여부 및 출력 레벨을 설정한다.
    DWORD	__stdcall AxmCrcSetMaskLevel(long lAxisNo, DWORD uLevel, DWORD uMethod);
	// 지정 축의 CRC 신호 사용 여부 및 출력 레벨을 반환한다.
	DWORD	__stdcall AxmCrcGetMaskLevel(long lAxisNo, DWORD *upLevel, DWORD *upMethod);

	//uOnOff  : CRC 신호를 Program으로 발생 여부  (FALSE(0),TRUE(1))

	// 지정 축에 CRC 신호를 강제로 발생 시킨다.
	DWORD	__stdcall AxmCrcSetOutput(long lAxisNo, DWORD uOnOff);
	// 지정 축의 CRC 신호를 강제로 발생 여부를 반환한다.
	DWORD	__stdcall AxmCrcGetOutput(long lAxisNo, DWORD *upOnOff);

	//uUse : 리미트/알람/비상정지/동기정지 신호에 의한 정지시 서버 잔여 펄스 제거 신호 출력 사용유무(QI만 사용, IP사용안함)
	//         (DISABLE(0),ENABLE(1))
	//uPositiveUse : Positive Emeregency End limit에 대한 Clear출력 사용 레벨(IP만 사용, QI사용안함)
	//uNegativeUse : Negative Emeregency End limit에 대한 Clear출력 사용 레벨(IP만 사용, QI사용안함)
	
	// 지정 축에 리미트에 대한 CRC 신호의 사용 여부 및 출력 레벨을 설정한다.
	DWORD	__stdcall AxmCrcSetEndLimit(long lAxisNo, DWORD uPositiveLevel, DWORD uNegativeLevel);
	// 지정 축의 리미트에 대한 CRC 신호의 사용 여부 및 출력 레벨을 반환한다.
	DWORD	__stdcall AxmCrcGetEndLimit(long lAxisNo, DWORD *upPositiveLevel, DWORD *upNegativeLevel);

//======MPG(Manual Pulse Generation) 함수===========================================================
    // MPGPos : MPG 입력신호마다 이동하는 거리."

    //================ CAMC-IP ===========================================================
    // lInputMethod : 0-7 까지 설정가능. 0:OnePhase, 1:TwoPhase1, 2:TwoPhase2, 3:TwoPhase4
	//                                   4:Level One Phase, 5:Level Two Phase1, 6: Level Two Phase2, 7:Level Two Phase4
    // lDriveMode   : 0-2 까지 설정가능
	//                0 :MPG 슬레이브 모드 ,1 :MPG PRESET 모드, 2 :MPG 연속 모드
	// lDirMode     : 0 - 1 까지 설정가능.
	//                0 : MPG_SIGNAL_DIR 모드 선택, 1 :MPG_USER_DIR 모드 선택
	// lUserDir     : 0:정방향, 1:역방향
    // dMPGdenominator, dMPGnumerator 사용안함.
	

	//================ CAMC-QI============================================================
    // lInputMethod : 0-3 까지 설정가능. 0:OnePhase, 1:TwoPhase1, 2:TwoPhase2, 3:TwoPhase4
    // lDriveMode   : 0-5 까지 설정가능
	//                0 :MPG 연속모드 ,1 :MPG PRESET 모드 (설정한 펄스까지만 이동), 2 :COMMAND ABSOLUTE MPG PRESET 모드 
	//                3 :ACTUAL ABSOLUTE MPG PRESET 모드 ,4 :COMMAND ABSOLUTE ZERO MPG PRESET 모드, 5 :ACTUAL ABSOLUTE ZERO MPG PRESET 모드 
    // lDriveMode   : 사용안함
	// lDirMode     : 사용안함
	// lUserDir     : 0:정방향, 1:역방향
	// MPGdenominator // MPG(수동 펄스 발생 장치 입력)구동 시 나누기 값(입력 값에 +1로 계산됨)
	// dMPGnumerator  // MPG(수동 펄스 발생 장치 입력)구동 시 곱하기 값(입력 값에 +1로 계산됨)
    // dwDenominator : 최대(0 에서  4095) 까지 설정 가능
    // dwNumerator   : 최대(0 에서    63) 까지 설정 가능
    // MPG PULSE 계산식. = ( (dwNumerator +1 ) * (dwDenominator + 1)/ 4096 )

	// 지정 축에 MPG 드라이브 지정 펄스 구동 모드에서 사용할 이동 거리, 구동모드 및 구동 방향 등을 설정한다.
	DWORD	__stdcall AxmMPGSet(long lAxisNo, double dVel, double dAccel, double dDecel, long lInputMethod, long lDriveMode, long lDirMode, long lUserDir, double dMPGPos, double dMPGdenominator, double dMPGnumerator);
	// 지정 축에 MPG 드라이브 지정 펄스 구동 모드에서 사용할 이동 거리, 구동모드 및 구동 방향 등을 반환한다.
	DWORD	__stdcall AxmMPGGet(long lAxisNo, double *dpVel, double *dpAccel, double *dpDecel, long *lpInputMethod, long *lpDriveMode, long *lpDirMode, long *lpUserDir, double *dpMPGPos, double *dpMPGdenominator, double *dpMPGnumerator);
	// 지정 축에 MPG 드라이브 설정을 해지한다.
	DWORD	__stdcall AxmMPGReset(long lAxisNo);

//========= 센서 위치 구동 함수( 필독: IP만가능 , QI에는 기능없음)=========================================================================
	
	// 지정 축의 Sensor 신호의 사용 유무 및 신호 입력 레벨을 설정한다.
	// 사용 유무 LOW(0), HIGH(1), UNUSED(2), USED(3)
	DWORD	__stdcall AxmSensorSetSignal(long lAxisNo, DWORD uLevel);
	// 지정 축의 Sensor 신호의 사용 유무 및 신호 입력 레벨을 반환한다.
	DWORD	__stdcall AxmSensorGetSignal(long lAxisNo, DWORD *upLevel);
	// 지정 축의 Sensor 신호의 입력 상태를 반환한다
	DWORD	__stdcall AxmSensorReadSignal(long lAxisNo, DWORD *upStatus);
	
    // 지정 축의 설정된 속도와 가속율로 센서 위치 드라이버를 구동한다.
    // Sensor 신호의 Active level입력 이후 상대 좌표로 설정된 거리만큼 구동후 정지한다.
    // 펄스가 출력되는 시점에서 함수를 벗어난다.
    // lMethod :  0 - 일반 구동, 1 - 센서 신호 검출 전은 저속 구동. 신호 검출 후 일반 구동
	//            2 - 저속 구동
	DWORD	__stdcall AxmSensorMovePos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, long lMethod);

    // 지정 축의 설정된 속도와 가속율로 센서 위치 드라이버를 구동한다.
    // Sensor 신호의 Active level입력 이후 상대 좌표로 설정된 거리만큼 구동후 정지한다.
    // 펄스 출력이 종료되는 시점에서 함수를 벗어난다.
	DWORD	__stdcall AxmSensorStartMovePos(long lAxisNo, double dPos, double dVel, double dAccel, double dDecel, long lMethod);

//======= 헬리컬 이동  (QI에만 해당됨)===========================================================================
    // 지정된 좌표계에 시작점, 종료점과 중심점을 지정하여 헬리컬 보간 구동하는 함수이다.
    // AxmContiBeginNode, AxmContiEndNode와 같이사용시 지정된 좌표계에 시작점, 종료점과 중심점을 지정하여 헬리컬 연속보간 구동하는 함수이다. 
	// 원호 연속 보간 구동을 위해 내부 Queue에 저장하는 함수이다. AxmContiStart함수를 사용해서 시작한다. (연속보간 함수와 같이 이용한다)
	DWORD	__stdcall AxmHelixCenterMove(long lCoord, double dCenterXPos, double dCenterYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dAccel, double dDecel, DWORD uCWDir);

    // 지정된 좌표계에 시작점, 종료점과 반지름을 지정하여 헬리컬 보간 구동하는 함수이다. 
    // AxmContiBeginNode, AxmContiEndNode와 같이사용시 지정된 좌표계에 중간점, 종료점을 지정하여 헬리컬연속 보간 구동하는 함수이다. 
	// 원호 연속 보간 구동을 위해 내부 Queue에 저장하는 함수이다. AxmContiStart함수를 사용해서 시작한다. (연속보간 함수와 같이 이용한다.)
	DWORD	__stdcall AxmHelixPointMove(long lCoord, double dMidXPos, double dMidYPos, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dAccel, double dDecel);

    // 지정된 좌표계에 시작점, 종료점과 반지름을 지정하여 헬리컬 보간 구동하는 함수이다.
    // AxmContiBeginNode, AxmContiEndNode와 같이사용시 지정된 좌표계에 시작점, 종료점과 반지름을 지정하여 헬리컬연속 보간 구동하는 함수이다. 
	// 원호 연속 보간 구동을 위해 내부 Queue에 저장하는 함수이다. AxmContiStart함수를 사용해서 시작한다. (연속보간 함수와 같이 이용한다.)
	DWORD	__stdcall AxmHelixRadiusMove(long lCoord, double dRadius, double dEndXPos, double dEndYPos, double dZPos, double dVel, double dAccel, double dDecel, DWORD uCWDir, DWORD uShortDistance);

	// 지정된 좌표계에 시작점, 회전각도와 반지름을 지정하여 헬리컬 보간 구동하는 함수이다
    // AxmContiBeginNode, AxmContiEndNode와 같이사용시 지정된 좌표계에 시작점, 회전각도와 반지름을 지정하여 헬리컬연속 보간 구동하는 함수이다. 
	// 원호 연속 보간 구동을 위해 내부 Queue에 저장하는 함수이다. AxmContiStart함수를 사용해서 시작한다. (연속보간 함수와 같이 이용한다.)
	DWORD	__stdcall AxmHelixAngleMove(long lCoord, double dCenterXPos, double dCenterYPos, double dAngle, double dZPos, double dVel, double dAccel, double dDecel, DWORD uCWDir);

//======== 스플라인 이동 (QI에만 해당됨)=========================================================================== 

    // AxmContiBeginNode, AxmContiEndNode와 같이사용안함. 
	// 스플라인 연속 보간 구동하는 함수이다. 원호 연속 보간 구동을 위해 내부 Queue에 저장하는 함수이다.
	// AxmContiStart함수를 사용해서 시작한다. (연속보간 함수와 같이 이용한다.)	
	// lPosSize : 최소 3개 이상.
	// 2축으로 사용시 dPoZ값을 0으로 넣어주면 됨.
	// 3축으로 사용시 축맵핑을 3개및 dPosZ 값을 넣어준다.
	DWORD	__stdcall AxmSplineWrite(long lCoord, long lPosSize, double *dpPosX, double *dpPosY, double dVel, double dAccel, double dDecel, double dPosZ, long lPointFactor);
	
//--------------------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif	//__cplusplus

#endif	//__AXT_AXM_H__









  





































