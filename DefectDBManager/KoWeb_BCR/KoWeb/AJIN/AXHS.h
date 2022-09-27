#ifndef __AXT_AXHS_H__
#define __AXT_AXHS_H__

#ifndef __DATA_TYPES__
	#define __DATA_TYPES__
	typedef unsigned long int					DWORD;
	typedef unsigned short int					WORD;
	typedef unsigned char						BYTE;
	typedef int									BOOL;
	typedef void								*HANDLE;
#endif

// 베이스보드 정의
#ifndef __AXT_BASE_BOARD__
	#define __AXT_BASE_BOARD__
	typedef enum _AXT_BASE_BOARD
	{
		AXT_UNKNOWN								= 0x00,			// Unknown Baseboard
		AXT_BIHR								= 0x01,			// ISA bus, Half size
		AXT_BIFR								= 0x02,			// ISA bus, Full size
		AXT_BPHR								= 0x03,			// PCI bus, Half size
		AXT_BPFR								= 0x04,			// PCI bus, Full size
		AXT_BV3R								= 0x05,			// VME bus, 3U size
		AXT_BV6R								= 0x06,			// VME bus, 6U size
		AXT_BC3R								= 0x07,			// cPCI bus, 3U size
		AXT_BC6R								= 0x08,			// cPCI bus, 6U size
		AXT_FMNSH4D								= 0x52,			// ISA bus, Full size, DB-32T, SIO-2V03 * 2
		AXT_PCI_DI64R							= 0x43,			// PCI bus, Digital IN 64점
		AXT_PCI_DO64R							= 0x53,			// PCI bus, Digital OUT 64점
		AXT_PCI_DB64R							= 0x63,			// PCI bus, Digital IN 32점, OUT 32점
		AXT_BPHD								= 0x83,			// PCI bus, Half size, DB-32T
		AXT_PCIN404							    = 0x84,			// PCI bus, Half size On-Board 4 Axis controller.	
		AXT_PCIN804							    = 0x85,			// PCI bus, Half size On-Board 8 Axis controller.
		AXT_PCI_AIO1602HR						= 0x93			// PCI bus, Half size, AI-16ch, AO-2ch AI16HR
	} AXT_BASE_BOARD;
#endif

// 모듈 정의
#ifndef __AXT_FUNC_MODULE__
	#define __AXT_FUNC_MODULE__
	typedef enum _AXT_FUNC_MODULE
	{
		AXT_SMC_2V01							= 0x01,			// CAMC-5M, 2 Axis
		AXT_SMC_2V02							= 0x02,			// CAMC-FS, 2 Axis
		AXT_SMC_1V01							= 0x03,			// CAMC-5M, 1 Axis
		AXT_SMC_1V02							= 0x04,			// CAMC-FS, 1 Axis
		AXT_SMC_2V03							= 0x05,			// CAMC-IP, 2 Axis
		AXT_SMC_4V04							= 0x06,			// CAMC-QI, 4 Axis
		AXT_SMC_4V51							= 0x33,			// MCX314,  4 Axis
		AXT_SMC_2V53							= 0x35,			// PMD, 2 Axis
		AXT_SMC_2V54							= 0x36,			// MCX312,  2 Axis
		AXT_SIO_DI32							= 0x97,			// Digital IN  32점
		AXT_SIO_DO32P							= 0x98,			// Digital OUT 32점
		AXT_SIO_DB32P							= 0x99,			// Digital IN 16점 / OUT 16점
		AXT_SIO_DO32T							= 0x9E,			// Digital OUT 16점, Power TR 출력
		AXT_SIO_DB32T							= 0x9F,			// Digital IN 16점 / OUT 16점, Power TR 출력
		AXT_SIO_AI4RB							= 0xA1,			// A1h(161) : AI 4Ch, 12 bit
		AXT_SIO_AO4RB							= 0xA2,			// A2h(162) : AO 4Ch, 12 bit
		AXT_SIO_AI16H							= 0xA3,			// A3h(163) : AI 4Ch, 16 bit
		AXT_SIO_AO8H							= 0xA4,			// A4h(164) : AO 4Ch, 16 bit
		AXT_SIO_AI16HB							= 0xA5,			// A5h(165) : AI 16Ch, 16 bit (SIO-AI16HR(input module))
		AXT_SIO_AO2HB							= 0xA6,			// A6h(166) : AO 2Ch, 16 bit  (SIO-AI16HR(output module))
		AXT_COM_234R							= 0xD3,			// COM-234R
		AXT_COM_484R							= 0xD4,			// COM-484R
	} AXT_FUNC_MODULE;
#endif

#ifndef __AXT_FUNC_RESULT__
	#define __AXT_FUNC_RESULT__
	typedef enum _AXT_FUNC_RESULT
	{
		AXT_RT_SUCCESS							= 0000,
		AXT_RT_OPEN_ERROR						= 1001,
		AXT_RT_OPEN_ALREADY						= 1002,
		AXT_RT_NOT_OPEN							= 1053,
		AXT_RT_NOT_SUPPORT_VERSION				= 1054,
		AXT_RT_INVALID_BOARD_NO					= 1101,
		AXT_RT_INVALID_MODULE_POS				= 1102,
		AXT_RT_INVALID_LEVEL					= 1103,
		AXT_RT_ERROR_VERSION_READ				= 1151,

		AXT_RT_AIO_OPEN_ERROR					= 2001,
		AXT_RT_AIO_NOT_MODULE					= 2051,
		AXT_RT_AIO_NOT_EVENT					= 2052, // 수정 
		AXT_RT_AIO_INVALID_MODULE_NO			= 2101,
		AXT_RT_AIO_INVALID_CHANNEL_NO			= 2102,
		AXT_RT_AIO_INVALID_USE					= 2106,
		AXT_RT_AIO_INVALID_TRIGGER_MODE			= 2107,

		AXT_RT_DIO_OPEN_ERROR					= 3001,
		AXT_RT_DIO_NOT_MODULE					= 3051,
		AXT_RT_DIO_NOT_INTERRUPT				= 3052,
		AXT_RT_DIO_INVALID_MODULE_NO			= 3101,
		AXT_RT_DIO_INVALID_OFFSET_NO			= 3102,
		AXT_RT_DIO_INVALID_LEVEL				= 3103,
		AXT_RT_DIO_INVALID_MODE					= 3104,
		AXT_RT_DIO_INVALID_VALUE				= 3105,
		AXT_RT_DIO_INVALID_USE					= 3106,

		AXT_RT_MOTION_OPEN_ERROR				= 4001,
		AXT_RT_MOTION_NOT_MODULE				= 4051,
		AXT_RT_MOTION_NOT_INTERRUPT				= 4052,
		AXT_RT_MOTION_NOT_INITIAL_AXIS_NO		= 4053,
		AXT_RT_MOTION_NOT_IN_CONT_INTERPOL		= 4054,
		AXT_RT_MOTION_NOT_PARA_READ     		= 4055,
		AXT_RT_MOTION_INVALID_AXIS_NO			= 4101,
		AXT_RT_MOTION_INVALID_METHOD			= 4102,
		AXT_RT_MOTION_INVALID_USE				= 4103,
		AXT_RT_MOTION_INVALID_LEVEL				= 4104,
		AXT_RT_MOTION_INVALID_BIT_NO			= 4105,
		AXT_RT_MOTION_INVALID_STOP_MODE			= 4106,
		AXT_RT_MOTION_INVALID_TRIGGER_MODE		= 4107,
		AXT_RT_MOTION_INVALID_TRIGGER_LEVEL		= 4108,
		AXT_RT_MOTION_INVALID_SELECTION			= 4109,
		AXT_RT_MOTION_INVALID_TIME				= 4110,
		AXT_RT_MOTION_INVALID_FILE_LOAD			= 4111,
		AXT_RT_MOTION_INVALID_FILE_SAVE			= 4112,
		AXT_RT_MOTION_INVALID_VELOCITY			= 4113,
		AXT_RT_MOTION_INVALID_ACCELTIME			= 4114,
		AXT_RT_MOTION_INVALID_PULSE_VALUE		= 4115,
		AXT_RT_MOTION_INVALID_NODE_NUMBER		= 4116,
		AXT_RT_MOTION_INVALID_TARGET			= 4117,
		AXT_RT_MOTION_ERROR_IN_NONMOTION		= 4151,
		AXT_RT_MOTION_ERROR_IN_MOTION			= 4152,
		AXT_RT_MOTION_ERROR						= 4153,
	   
		AXT_RT_MOTION_ERROR_GANTRY_ENABLE		= 4154,
	   	AXT_RT_MOTION_ERROR_GANTRY_AXIS 		= 4155,
		AXT_RT_MOTION_ERROR_MASTER_SERVOON 		= 4156,
		AXT_RT_MOTION_ERROR_SLAVE_SERVOON 		= 4157,
		AXT_RT_MOTION_INVALID_POSITION		    = 4158,

		AXT_RT_NOT_CAPTURED					    = 4162,
		AXT_RT_ERROR_NOT_SAME_IC			    = 4163,
		AXT_RT_ERROR_NOT_GEARMODE				= 4164,
		AXT_ERROR_CONTI_INVALID_AXIS_NO			= 4165,
		AXT_ERROR_CONTI_INVALID_MAP_NO			= 4166,
		AXT_ERROR_CONTI_EMPTY_MAP_NO			= 4167,
		AXT_RT_MOTION_ERROR_CACULATION			= 4168,

		AXT_ERROR_HELICAL_INVALID_AXIS_NO		= 4170,
		AXT_ERROR_HELICAL_INVALID_MAP_NO		= 4171,
		AXT_ERROR_HELICAL_EMPTY_MAP_NO			= 4172,

		AXT_ERROR_SPLINE_INVALID_AXIS_NO		= 4180,
		AXT_ERROR_SPLINE_INVALID_MAP_NO			= 4181,
		AXT_ERROR_SPLINE_EMPTY_MAP_NO			= 4182,
		AXT_ERROR_SPLINE_NUM_ERROR              = 4183,
		AXT_RT_MOTION_INTERPOL_VALUE			= 4184,
		AXT_RT_ERROR_NOT_CONTIBEGIN             = 4185,
		AXT_RT_ERROR_NOT_CONTIEND               = 4186,

		AXT_RT_MOTION_HOME_SEARCHING			= 4201,
		AXT_RT_MOTION_HOME_ERROR_SEARCHING		= 4202,
		AXT_RT_MOTION_HOME_ERROR_START			= 4203,
		AXT_RT_MOTION_HOME_ERROR_GANTRY			= 4204,
		AXT_RT_MOTION_POSITION_OUTOFBOUND		= 4251,
		AXT_RT_MOTION_PROFILE_INVALID			= 4252,
		AXT_RT_MOTION_VELOCITY_OUTOFBOUND		= 4253,
		AXT_RT_MOTION_MOVE_UNIT_IS_ZERO			= 4254,
		AXT_RT_MOTION_SETTING_ERROR				= 4255,
		AXT_RT_MOTION_IN_CONT_INTERPOL			= 4256,
		AXT_RT_MOTION_DISABLE_TRIGGER			= 4257,
		AXT_RT_MOTION_INVALID_CONT_INDEX		= 4258,
		AXT_RT_MOTION_CONT_QUEUE_FULL			= 4259
	} AXT_FUNC_RESULT;
#endif
/*
#ifndef PASCAL
	#define PASCAL								__stdcall
#endif	// PASCAL

#ifndef EXPORT
	#define EXPORT
#endif	// EXPORT
*/
#ifndef __AFX_H__
	#define HWND								HANDLE
#endif

#ifndef FALSE
#define FALSE									0
#define TRUE									1
#endif

#ifndef WM_USER
	#define WM_USER								0x0400
#endif

#ifndef WM_AXL_INTERRUPT
	#define WM_AXL_INTERRUPT					(WM_USER + 1001)
#endif

#ifndef __AXT_LOG_LEVEL__
	#define __AXT_LOG_LEVEL__
	typedef enum _AXT_LOG_LEVEL
	{
		LEVEL_NONE,
		LEVEL_ERROR,
		LEVEL_RUNSTOP,
		LEVEL_FUNCTION
	} AXT_LOG_LEVEL;	// 로그 레벨
#endif

#ifndef __AXT_EXISTENCE__
	#define __AXT_EXISTENCE__
	typedef enum _AXT_EXISTENCE
	{
		STATUS_NOTEXIST,
		STATUS_EXIST
	} AXT_EXISTENCE;
#endif
 
#ifndef __AXT_USE__
	#define __AXT_USE__
	typedef enum _AXT_USE
	{
		DISABLE,
		ENABLE
	} AXT_USE;
#endif

#ifndef __AXT_AIO_TRIGGER_MODE__
	#define __AXT_AIO_TRIGGER_MODE__
	typedef enum _AXT_AIO_TRIGGER_MODE
	{
		DISABLE_MODE							= 0,
		NORMAL_MODE								= 1,
		TIMER_MODE, 
		EXTERNAL_MODE
	} AXT_AIO_TRIGGER_MODE;
#endif

#ifndef __AXT_AIO_FULL_MODE__
	#define __AXT_AIO_FULL_MODE__
	typedef enum _AXT_AIO_FULL_MODE
	{
		NEW_DATA_KEEP,
		CURR_DATA_KEEP
	} AXT_AIO_FULL_MODE;
#endif

#ifndef __AXT_AIO_EVENT_MASK__
	#define __AXT_AIO_EVENT_MASK__
	typedef enum _AXT_AIO_EVENT_MASK //AIO 쪽 하드웨어 인터럽트가 추가 되면서 명칭의 혼란을 우려해서 수정함. 
	{
		DATA_EMPTY								= 0x01,
		DATA_MANY								= 0x02,
		DATA_SMALL								= 0x04,
		DATA_FULL								= 0x08
	} AXT_AIO_EVENT_MASK;
#endif

#ifndef __AXT_AIO_INTERRUPT_MASK__
	#define __AXT_AIO_INTERRUPT_MASK__
	typedef enum _AXT_AIO_INTERRUPT_MASK //AIO 쪽 하드웨어 인터럽트가 추가되면서 기존의 내용을 변경함. 
	{
		ADC_DONE									= 0x00,
		SCAN_END									= 0x01,
		FIFO_HALF_FULL								= 0x02,
		NO_SIGNAL									= 0x03
	} AXT_AIO_INTERRUPT_MASK;
#endif
#ifndef __AXT_AIO_QUEUE_EVENT__
	#define __AXT_AIO_QUEUE_EVENT__		//AIO쪽 이벤트 /콜백 /윈도우 메시지의 파라메터 값에 대한 정의 추가 
	typedef enum  _AIO_EVENT_MODE  
	{ 
		AIO_EVENT_DATA_RESET						= 0x00, 
		AIO_EVENT_DATA_UPPER, 
		AIO_EVENT_DATA_LOWER, 
		AIO_EVENT_DATA_FULL, 
		AIO_EVENT_DATA_EMPTY 
	} AIO_EVENT_MODE;
#endif
	
#ifndef __AXT_DIO_EDGE__
	#define __AXT_DIO_EDGE__
	typedef enum _AXT_DIO_EDGE
	{
		DOWN_EDGE,
		UP_EDGE
	} AXT_DIO_EDGE;
#endif

#ifndef __AXT_DIO_STATE__
	#define __AXT_DIO_STATE__
	typedef enum _AXT_DIO_STATE
	{
		OFF_STATE,
		ON_STATE
	} AXT_DIO_STATE;
#endif

#ifndef AXT_MOTION_STOPMODE_DEF
	#define AXT_MOTION_STOPMODE_DEF
	typedef enum _AXT_MOTION_STOPMODE
	{
		EMERGENCY_STOP,
		SLOWDOWN_STOP
	} AXT_MOTION_STOPMODE;
#endif

#ifndef AXT_MOTION_EDGE_DEF
	#define AXT_MOTION_EDGE_DEF
	typedef enum _AXT_MOTION_EDGE
	{
		SIGNAL_UP_EDGE,
		SIGNAL_DOWN_EDGE
	} AXT_MOTION_EDGE;
#endif

#ifndef AXT_MOTION_SELECTION_DEF
	#define AXT_MOTION_SELECTION_DEF
	typedef enum _AXT_MOTION_SELECTION
	{
		COMMAND,
		ACTUAL
	} AXT_MOTION_SELECTION;
#endif

#ifndef AXT_MOTION_TRIGGER_MODE_DEF
	#define AXT_MOTION_TRIGGER_MODE_DEF
	typedef enum _AXT_MOTION_TRIGGER_MODE
	{
		PERIOD_MODE,
		ABS_POS_MODE
	} AXT_MOTION_TRIGGER_MODE;
#endif

#ifndef AXT_MOTION_LEVEL_MODE_DEF
	#define AXT_MOTION_LEVEL_MODE_DEF
	typedef enum _AXT_MOTION_LEVEL_MODE
	{
		LOW,
		HIGH,
		UNUSED,
		USED
	} AXT_MOTION_LEVEL_MODE;
#endif

#ifndef AXT_MOTION_ABSREL_MODE_DEF
	#define AXT_MOTION_ABSREL_MODE_DEF
	typedef enum _AXT_MOTION_ABSREL_MODE
	{
		POS_ABS_MODE,
		POS_REL_MODE
	} AXT_MOTION_ABSREL_MODE;
#endif

#ifndef AXT_MOTION_PROFILE_MODE_DEF
	#define AXT_MOTION_PROFILE_MODE_DEF
	typedef enum _AXT_MOTION_PROFILE_MODE
	{
		SYM_TRAPEZOIDE_MODE,
		ASYM_TRAPEZOIDE_MODE,
		QUASI_S_CURVE_MODE,
		SYM_S_CURVE_MODE,
		ASYM_S_CURVE_MODE

	} AXT_MOTION_PROFILE_MODE;
#endif

#ifndef AXT_MOTION_SELECTION_DEF
	#define AXT_MOTION_SELECTION_DEF
	typedef enum _AXT_MOTION_SIGNAL_LEVEL
	{
		INACTIVE,
		ACTIVE
	} AXT_MOTION_SIGNAL_LEVEL;
#endif

#ifndef AXT_MOTION_HOME_RESULT_DEF
	#define AXT_MOTION_HOME_RESULT_DEF
	typedef enum AXT_MOTION_HOME_RESULT
	{
		HOME_SUCCESS							= 0x01,
		HOME_SEARCHING							= 0x02,
		HOME_ERR_GNT_RANGE						= 0x10,
		HOME_ERR_USER_BREAK						= 0x11,
		HOME_ERR_VELOCITY						= 0x12,	
		HOME_ERR_AMP_FAULT						= 0x13,		// 서보팩 알람 발생 에러
		HOME_ERR_NEG_LIMIT						= 0x14,		// (-)방향 구동중 (+)리미트 센서 감지 에러
		HOME_ERR_POS_LIMIT						= 0x15,		// (+)방향 구동중 (-)리미트 센서 감지 에러
		HOME_ERR_NOT_DETECT						= 0x16,		// 지정한 신호 검출하지 못 할 경우 에러
		HOME_ERR_UNKNOWN						= 0xFF,
	} AXT_MOTION_HOME_RESULT;
#endif

#ifndef AXT_MOTION_UNIV_INPUT_DEF
	#define AXT_MOTION_UNIV_INPUT_DEF
	typedef enum _AXT_MOTION_UNIV_INPUT
	{
		UIO_INP0,
		UIO_INP1,
		UIO_INP2,
		UIO_INP3,
		UIO_INP4
	} AXT_MOTION_UNIV_INPUT;
#endif

#ifndef AXT_MOTION_UNIV_OUTPUT_DEF
	#define AXT_MOTION_UNIV_OUTPUT_DEF
	typedef enum _AXT_MOTION_UNIV_OUTPUT
	{
		UIO_OUT0,
		UIO_OUT1,
		UIO_OUT2,
		UIO_OUT3,
		UIO_OUT4
	} AXT_MOTION_UNIV_OUTPUT;
#endif


#ifndef AXT_MOTION_DETECT_DOWN_START_POINT_DEF
	#define AXT_MOTION_DETECT_DOWN_START_POI_DEFNT
	typedef enum _AXT_MOTION_DETECT_DOWN_START_POINT
	{
		AutoDetect,
		RestPulse
	} AXT_MOTION_DETECT_DOWN_START_POINT;
#endif

#ifndef AXT_MOTION_PULSE_OUTPUT_DEF
	#define AXT_MOTION_PULSE_OUTPUT_DEF
	/* Pulse Output Method					*/
	typedef enum _AXT_MOTION__PULSE_OUTPUT 
	{
		OneHighLowHigh							= 0x0,			// 1펄스 방식, PULSE(Active High), 정방향(DIR=Low)  / 역방향(DIR=High)
		OneHighHighLow							= 0x1,			// 1펄스 방식, PULSE(Active High), 정방향(DIR=High) / 역방향(DIR=Low)
		OneLowLowHigh							= 0x2,			// 1펄스 방식, PULSE(Active Low),  정방향(DIR=Low)  / 역방향(DIR=High)
		OneLowHighLow							= 0x3,			// 1펄스 방식, PULSE(Active Low),  정방향(DIR=High) / 역방향(DIR=Low)
		TwoCcwCwHigh							= 0x4,			// 2펄스 방식, PULSE(CCW:역방향),  DIR(CW:정방향),  Active High	 
		TwoCcwCwLow								= 0x5,			// 2펄스 방식, PULSE(CCW:역방향),  DIR(CW:정방향),  Active Low	 
		TwoCwCcwHigh							= 0x6,			// 2펄스 방식, PULSE(CW:정방향),   DIR(CCW:역방향), Active High
		TwoCwCcwLow								= 0x7,			// 2펄스 방식, PULSE(CW:정방향),   DIR(CCW:역방향), Active Low
		TwoPhase								= 0x8,			// 2상(90' 위상차),  PULSE lead DIR(CW: 정방향), PULSE lag DIR(CCW:역방향)
		TwoPhaseReverse							= 0x9			// 2상(90' 위상차),  PULSE lead DIR(CCW: 정방향), PULSE lag DIR(CW:역방향)
	
	
	} AXT_MOTION_PULSE_OUTPUT;
#endif

#ifndef AXT_MOTION_EXTERNAL_COUNTER_INPUT_DEF
	#define AXT_MOTION_EXTERNAL_COUNTER_INPUT_DEF
	/* Mode2 Data   External Counter Input */
	typedef enum _AXT_MOTION__EXTERNAL_COUNTER_INPUT
	{
		ObverseUpDownMode						= 0x0,			// 정방향 Up/Down
		ObverseSqr1Mode							= 0x1,			// 정방향 1체배
		ObverseSqr2Mode							= 0x2,			// 정방향 2체배
		ObverseSqr4Mode  						= 0x3,			// 정방향 4체배
		ReverseUpDownMode						= 0x4,			// 역방향 Up/Down
		ReverseSqr1Mode 					  	= 0x5,			// 역방향 1체배
		ReverseSqr2Mode 					  	= 0x6,			// 역방향 2체배
		ReverseSqr4Mode 					 	= 0x7			// 역방향 4체배
	} AXT_MOTION_EXTERNAL_COUNTER_INPUT;
#endif

#ifndef AXT_MOTION_ACC_UNIT_DEF
	#define AXT_MOTION_ACC_UNIT_DEF
	typedef enum _AXT_MOTION_ACC_UNIT
	{
		UNIT_SEC2						= 0x0,			// unit/sec2
		SEC								= 0x1			// sec
	} AXT_MOTION_ACC_UNIT;
#endif

#ifndef AXT_MOTION_MOVE_DIR_DEF
	#define AXT_MOTION_MOVE_DIR_DEF
	typedef enum _AXT_MOTION_MOVE_DIR
	{
		DIR_CCW						= 0x0,			// 반시계방향
		DIR_CW						= 0x1 			// 시계방향
	} AXT_MOTION_MOVE_DIR;
#endif

#ifndef AXT_MOTION_RADIUS_DISTANCE_DEF
	#define AXT_MOTION_RADIUS_DISTANCE_DEF
	typedef enum _AXT_MOTION_RADIUS_DISTANCE
	{
		SHORT_DISTANCE					= 0x0,			// 짧은 거리의 원호 이동 
		LONG_DISTANCE					= 0x1 			// 긴 거리의 원호 이동 
	} AXT_MOTION_RADIUS_DISTANCE;
#endif

#ifndef AXT_MOTION_INTERPOLATION_AXIS_DEF
	#define AXT_MOTION_INTERPOLATION_AXIS_DEF
	typedef enum _AXT_MOTION_INTERPOLATION_AXIS
	{
		INTERPOLATION_AXIS2				= 0x0,			// 2축을 보간으로 사용할 때
		INTERPOLATION_AXIS3				= 0x1,			// 3축을 보간으로 사용할 때
		INTERPOLATION_AXIS4				= 0x2			// 4축을 보간으로 사용할 때
	} AXT_MOTION_INTERPOLATION_AXIS;
#endif

#ifndef AXT_MOTION_CONTISTART_NODE_DEF
	#define AXT_MOTION_CONTISTART_NODE_DEF
	typedef enum _AXT_MOTION_CONTISTART_NODE
	{
		CONTI_NODE_VELOCITY				= 0x0,			// 속도 지정 보간 모드
		CONTI_NODE_MANUAL				= 0x1,			// 노드 가감속 보간 모드
		CONTI_NODE_AUTO  				= 0x2			// 자동 가감속 보간 모드
	} AXT_MOTION_CONTISTART_NODE;
#endif

#ifndef AXT_MOTION_HOME_DETECT_SIGNAL_DEF
	#define AXT_MOTION_HOME_DETECT_SIGNAL_DEF
	typedef enum _AXT_MOTION_HOME_DETECT_SIGNAL
	{
		PosEndLimit								= 0x0,			// +Elm(End limit) +방향 리미트 센서 신호
		NegEndLimit								= 0x1,			// -Elm(End limit) -방향 리미트 센서 신호
		PosSloLimit								= 0x2,			// +Slm(Slow Down limit) 신호 - 사용하지 않음
		NegSloLimit								= 0x3,			// -Slm(Slow Down limit) 신호 - 사용하지 않음
		HomeSensor								= 0x4,			// IN0(ORG)  원점 센서 신호
		EncodZPhase								= 0x5,			// IN1(Z상)  Encoder Z상 신호
		UniInput02								= 0x6,			// IN2(범용) 범용 입력 2번 신호
		UniInput03								= 0x7,			// IN3(범용) 범용 입력 3번 신호
	} AXT_MOTION_HOME_DETECT_SIGNAL;
#endif


#ifndef AXT_MOTION_MPG_INPUT_METHOD_DEF
	#define AXT_MOTION_MPG_INPUT_METHOD_DEF
	typedef enum _AXT_MOTION_MPG_INPUT_METHOD
	{
		MPG_DIFF_ONE_PHASE		            				= 0x0,			// MPG 입력 방식 One Phase
		MPG_DIFF_TWO_PHASE_1X								= 0x1,			// MPG 입력 방식 TwoPhase1
		MPG_DIFF_TWO_PHASE_2X								= 0x2,			// MPG 입력 방식 TwoPhase2
		MPG_DIFF_TWO_PHASE_4X								= 0x3,			// MPG 입력 방식 TwoPhase4
		MPG_LEVEL_ONE_PHASE	    							= 0x4,			// MPG 입력 방식 Level One Phase
		MPG_LEVEL_TWO_PHASE_1X								= 0x5,			// MPG 입력 방식 Level Two Phase1
		MPG_LEVEL_TWO_PHASE_2X								= 0x6,			// MPG 입력 방식 Level Two Phase2
		MPG_LEVEL_TWO_PHASE_4X								= 0x7,			// MPG 입력 방식 Level Two Phase4
	} AXT_MOTION_MPG_INPUT_METHOD;
#endif

#ifndef AXT_MOTION_SENSOR_INPUT_METHOD_DEF
	#define AXT_MOTION_SENSOR_INPUT_METHOD_DEF
	typedef enum _AXT_MOTION_SENSOR_INPUT_METHOD
	{
		SENSOR_METHOD1				= 0x0,			// 일반 구동
		SENSOR_METHOD2				= 0x1,			// 센서 신호 검출 전은 저속 구동. 신호 검출 후 일반 구동
		SENSOR_METHOD3				= 0x2			// 저속 구동
	} AXT_MOTION_SENSOR_INPUT_METHOD;
#endif

#ifndef AXT_MOTION_HOME_CRC_SELECT_DEF
	#define AXT_MOTION_HOME_CRC_SELECT_DEF
	typedef enum _AXT_MOTION_HOME_CRC_SELECT
	{
		CRC_SELECT1				= 0x0,			// 위치클리어 사용않함, 잔여펄스 클리어 사용 안함
		CRC_SELECT2				= 0x1,			// 위치클리어 사용함, 잔여펄스 클리어 사용 안함
		CRC_SELECT3				= 0x2,			// 위치클리어 사용안함, 잔여펄스 클리어 사용함
		CRC_SELECT4             = 0x3			// 위치클리어 사용함, 잔여펄스 클리어 사용함
	} AXT_MOTION_HOME_CRC_SELECT;
#endif

#ifndef AXT_MOTION_IPDETECT_DESTINATION_SIGNAL_DEF
	#define AXT_MOTION_IPDETECT_DESTINATION_SIGNAL_DEF
	/* Detect Destination Signal			*/
	typedef enum _AXT_MOTION_IPDETECT_DESTINATION_SIGNAL
	{
		PElmNegativeEdge						= 0x0,			// +Elm(End limit) 하강 edge
		NElmNegativeEdge						= 0x1,			// -Elm(End limit) 하강 edge
		PSlmNegativeEdge						= 0x2,			// +Slm(Slowdown limit) 하강 edge
		NSlmNegativeEdge						= 0x3,			// -Slm(Slowdown limit) 하강 edge
		In0DownEdge								= 0x4,			// IN0(ORG) 하강 edge
		In1DownEdge								= 0x5,			// IN1(Z상) 하강 edge
		In2DownEdge								= 0x6,			// IN2(범용) 하강 edge
		In3DownEdge								= 0x7,			// IN3(범용) 하강 edge
		PElmPositiveEdge						= 0x8,			// +Elm(End limit) 상승 edge
		NElmPositiveEdge						= 0x9,			// -Elm(End limit) 상승 edge
		PSlmPositiveEdge						= 0xa,			// +Slm(Slowdown limit) 상승 edge
		NSlmPositiveEdge						= 0xb,			// -Slm(Slowdown limit) 상승 edge
		In0UpEdge								= 0xc,			// IN0(ORG) 상승 edge
		In1UpEdge								= 0xd,			// IN1(Z상) 상승 edge
		In2UpEdge								= 0xe,			// IN2(범용) 상승 edge
		In3UpEdge								= 0xf			// IN3(범용) 상승 edge
	} AXT_MOTION_IPDETECT_DESTINATION_SIGNAL;
#endif

#ifndef AXT_MOTION_IPEND_STATUS_DEF
	#define AXT_MOTION_IPEND_STATUS_DEF
	/* IP End status : 0x0000이면 정상종료	*/
	typedef	enum _AXT_MOTION_IPEND_STATUS
	{
		IPEND_STATUS_SLM						= 0x0001,		// Bit 0, limit 감속정지 신호 입력에 의한 종료
		IPEND_STATUS_ELM						= 0x0002,		// Bit 1, limit 급정지 신호 입력에 의한 종료
		IPEND_STATUS_SSTOP_SIGNAL				= 0x0004,		// Bit 2, 감속 정지 신호 입력에 의한 종료
		IPEND_STATUS_ESTOP_SIGNAL				= 0x0008,		// Bit 3, 급정지 신호 입력에 의한 종료
		IPEND_STATUS_SSTOP_COMMAND				= 0x0010,		// Bit 4, 감속 정지 명령에 의한 종료
		IPEND_STATUS_ESTOP_COMMAND				= 0x0020,		// Bit 5, 급정지 정지 명령에 의한 종료
		IPEND_STATUS_ALARM_SIGNAL				= 0x0040,		// Bit 6, Alarm 신호 입력에 희한 종료
		IPEND_STATUS_DATA_ERROR					= 0x0080,		// Bit 7, 데이터 설정 에러에 의한 종료
		IPEND_STATUS_DEVIATION_ERROR			= 0x0100,		// Bit 8, 탈조 에러에 의한 종료
		IPEND_STATUS_ORIGIN_DETECT				= 0x0200,		// Bit 9, 원점 검출에 의한 종료
		IPEND_STATUS_SIGNAL_DETECT				= 0x0400,		// Bit 10, 신호 검출에 의한 종료(Signal search-1/2 drive 종료)
		IPEND_STATUS_PRESET_PULSE_DRIVE			= 0x0800,		// Bit 11, Preset pulse drive 종료
		IPEND_STATUS_SENSOR_PULSE_DRIVE			= 0x1000,		// Bit 12, Sensor pulse drive 종료
		IPEND_STATUS_LIMIT						= 0x2000,		// Bit 13, Limit 완전정지에 의한 종료
		IPEND_STATUS_SOFTLIMIT					= 0x4000,		// Bit 14, Soft limit에 의한 종료
		IPEND_STATUS_INTERPOLATION_DRIVE		= 0x8000		// Bit 15, 보간 드라이브에 의한 종료.
	} AXT_MOTION_IPEND_STATUS;
#endif

#ifndef AXT_MOTION_IPDRIVE_STATUS_DEF
	#define AXT_MOTION_IPDRIVE_STATUS_DEF
	/* FS Drive status						*/
	typedef	enum _AXT_MOTION_IPDRIVE_STATUS
	{
		IPDRIVE_STATUS_BUSY						= 0x00001,		// Bit 0, BUSY(드라이브 구동 중)
		IPDRIVE_STATUS_DOWN						= 0x00002,		// Bit 1, DOWN(감속 중)
		IPDRIVE_STATUS_CONST					= 0x00004,		// Bit 2, CONST(등속 중)
		IPDRIVE_STATUS_UP						= 0x00008,		// Bit 3, UP(가속 중)
		IPDRIVE_STATUS_ICL						= 0x00010,		// Bit 4, ICL(내부 위치 카운터 < 내부 위치 카운터 비교값)
		IPDRIVE_STATUS_ICG						= 0x00020,		// Bit 5, ICG(내부 위치 카운터 > 내부 위치 카운터 비교값)
		IPDRIVE_STATUS_ECL						= 0x00040,		// Bit 6, ECL(외부 위치 카운터 < 외부 위치 카운터 비교값)
		IPDRIVE_STATUS_ECG						= 0x00080,		// Bit 7, ECG(외부 위치 카운터 > 외부 위치 카운터 비교값)
		IPDRIVE_STATUS_DRIVE_DIRECTION			= 0x00100,		// Bit 8, 드라이브 방향 신호(0=CW/1=CCW)
		IPDRIVE_STATUS_COMMAND_BUSY				= 0x00200,		// Bit 9, 명령어 수행중
		IPDRIVE_STATUS_PRESET_DRIVING			= 0x00400,		// Bit 10, Preset pulse drive 중
		IPDRIVE_STATUS_CONTINUOUS_DRIVING		= 0x00800,		// Bit 11, Continuouse speed drive 중
		IPDRIVE_STATUS_SIGNAL_SEARCH_DRIVING	= 0x01000,		// Bit 12, Signal search-1/2 drive 중
		IPDRIVE_STATUS_ORG_SEARCH_DRIVING		= 0x02000,		// Bit 13, 원점 검출 drive 중
		IPDRIVE_STATUS_MPG_DRIVING				= 0x04000,		// Bit 14, MPG drive 중
		IPDRIVE_STATUS_SENSOR_DRIVING			= 0x08000,		// Bit 15, Sensor positioning drive 중
		IPDRIVE_STATUS_L_C_INTERPOLATION		= 0x10000,		// Bit 16, 직선/원호 보간 중
		IPDRIVE_STATUS_PATTERN_INTERPOLATION	= 0x20000,		// Bit 17, 비트 패턴 보간 중
		IPDRIVE_STATUS_INTERRUPT_BANK1			= 0x40000,		// Bit 18, 인터럽트 bank1에서 발생
		IPDRIVE_STATUS_INTERRUPT_BANK2			= 0x80000		// Bit 19, 인터럽트 bank2에서 발생
	} AXT_MOTION_IPDRIVE_STATUS;
#endif

#ifndef AXT_MOTION_IPINTERRUPT_BANK1_DEF
	#define AXT_MOTION_IPINTERRUPT_BANK1_DEF
	/* IP Interrupt MASK setting			*/
	typedef	enum _AXT_MOTION_IPINTERRUPT_BANK1
	{
		IPINTBANK1_DONTUSE						= 0x00000000,	// INTERRUT DISABLED.
		IPINTBANK1_DRIVE_END					= 0x00000001,	// Bit 0, Drive end(default value : 1).
		IPINTBANK1_ICG							= 0x00000002,	// Bit 1, INCNT is greater than INCNTCMP.
		IPINTBANK1_ICE							= 0x00000004,	// Bit 2, INCNT is equal with INCNTCMP.
		IPINTBANK1_ICL							= 0x00000008,	// Bit 3, INCNT is less than INCNTCMP.
		IPINTBANK1_ECG							= 0x00000010,	// Bit 4, EXCNT is greater than EXCNTCMP.
		IPINTBANK1_ECE							= 0x00000020,	// Bit 5, EXCNT is equal with EXCNTCMP.
		IPINTBANK1_ECL							= 0x00000040,	// Bit 6, EXCNT is less than EXCNTCMP.
		IPINTBANK1_SCRQEMPTY					= 0x00000080,	// Bit 7, Script control queue is empty.
		IPINTBANK1_CAPRQEMPTY					= 0x00000100,	// Bit 8, Caption result data queue is empty.
		IPINTBANK1_SCRREG1EXE					= 0x00000200,	// Bit 9, Script control register-1 command is executed.
		IPINTBANK1_SCRREG2EXE					= 0x00000400,	// Bit 10, Script control register-2 command is executed.
		IPINTBANK1_SCRREG3EXE					= 0x00000800,	// Bit 11, Script control register-3 command is executed.
		IPINTBANK1_CAPREG1EXE					= 0x00001000,	// Bit 12, Caption control register-1 command is executed.
		IPINTBANK1_CAPREG2EXE					= 0x00002000,	// Bit 13, Caption control register-2 command is executed.
		IPINTBANK1_CAPREG3EXE					= 0x00004000,	// Bit 14, Caption control register-3 command is executed.
		IPINTBANK1_INTGGENCMD					= 0x00008000,	// Bit 15, Interrupt generation command is executed(0xFF)
		IPINTBANK1_DOWN							= 0x00010000,	// Bit 16, At starting point for deceleration drive.
		IPINTBANK1_CONT							= 0x00020000,	// Bit 17, At starting point for constant speed drive.
		IPINTBANK1_UP							= 0x00040000,	// Bit 18, At starting point for acceleration drive.
		IPINTBANK1_SIGNALDETECTED				= 0x00080000,	// Bit 19, Signal assigned in MODE1 is detected.
		IPINTBANK1_SP23E						= 0x00100000,	// Bit 20, Current speed is equal with rate change point RCP23.
		IPINTBANK1_SP12E						= 0x00200000,	// Bit 21, Current speed is equal with rate change point RCP12.
		IPINTBANK1_SPE							= 0x00400000,	// Bit 22, Current speed is equal with speed comparison data(SPDCMP).
		IPINTBANK1_INCEICM						= 0x00800000,	// Bit 23, INTCNT(1'st counter) is equal with ICM(1'st count minus limit data)
		IPINTBANK1_SCRQEXE						= 0x01000000,	// Bit 24, Script queue command is executed When SCRCONQ's 30 bit is '1'.
		IPINTBANK1_CAPQEXE						= 0x02000000,	// Bit 25, Caption queue command is executed When CAPCONQ's 30 bit is '1'.
		IPINTBANK1_SLM							= 0x04000000,	// Bit 26, NSLM/PSLM input signal is activated.
		IPINTBANK1_ELM							= 0x08000000,	// Bit 27, NELM/PELM input signal is activated.
		IPINTBANK1_USERDEFINE1					= 0x10000000,	// Bit 28, Selectable interrupt source 0(refer "0xFE" command).
		IPINTBANK1_USERDEFINE2					= 0x20000000,	// Bit 29, Selectable interrupt source 1(refer "0xFE" command).
		IPINTBANK1_USERDEFINE3					= 0x40000000,	// Bit 30, Selectable interrupt source 2(refer "0xFE" command).
		IPINTBANK1_USERDEFINE4					= 0x80000000	// Bit 31, Selectable interrupt source 3(refer "0xFE" command).
	} AXT_MOTION_IPINTERRUPT_BANK1;
#endif

#ifndef AXT_MOTION_IPINTERRUPT_BANK2_DEF
	#define AXT_MOTION_IPINTERRUPT_BANK2_DEF
	typedef	enum _AXT_MOTION_IPINTERRUPT_BANK2
	{
		IPINTBANK2_DONTUSE						= 0x00000000,	// INTERRUT DISABLED.
		IPINTBANK2_L_C_INP_Q_EMPTY				= 0x00000001,	// Bit 0, Linear/Circular interpolation parameter queue is empty.
		IPINTBANK2_P_INP_Q_EMPTY				= 0x00000002,	// Bit 1, Bit pattern interpolation queue is empty.
		IPINTBANK2_ALARM_ERROR					= 0x00000004,	// Bit 2, Alarm input signal is activated.
		IPINTBANK2_INPOSITION					= 0x00000008,	// Bit 3, Inposition input signal is activated.
		IPINTBANK2_MARK_SIGNAL_HIGH				= 0x00000010,	// Bit 4, Mark input signal is activated.
		IPINTBANK2_SSTOP_SIGNAL					= 0x00000020,	// Bit 5, SSTOP input signal is activated.
		IPINTBANK2_ESTOP_SIGNAL					= 0x00000040,	// Bit 6, ESTOP input signal is activated.
		IPINTBANK2_SYNC_ACTIVATED				= 0x00000080,	// Bit 7, SYNC input signal is activated.
		IPINTBANK2_TRIGGER_ENABLE				= 0x00000100,	// Bit 8, Trigger output is activated.
		IPINTBANK2_EXCNTCLR						= 0x00000200,	// Bit 9, External(2'nd) counter is cleard by EXCNTCLR setting.
		IPINTBANK2_FSTCOMPARE_RESULT_BIT0		= 0x00000400,	// Bit 10, ALU1's compare result bit 0 is activated.
		IPINTBANK2_FSTCOMPARE_RESULT_BIT1		= 0x00000800,	// Bit 11, ALU1's compare result bit 1 is activated.
		IPINTBANK2_FSTCOMPARE_RESULT_BIT2		= 0x00001000,	// Bit 12, ALU1's compare result bit 2 is activated.
		IPINTBANK2_FSTCOMPARE_RESULT_BIT3		= 0x00002000,	// Bit 13, ALU1's compare result bit 3 is activated.
		IPINTBANK2_FSTCOMPARE_RESULT_BIT4		= 0x00004000,	// Bit 14, ALU1's compare result bit 4 is activated.
		IPINTBANK2_SNDCOMPARE_RESULT_BIT0		= 0x00008000,	// Bit 15, ALU2's compare result bit 0 is activated.
		IPINTBANK2_SNDCOMPARE_RESULT_BIT1		= 0x00010000,	// Bit 16, ALU2's compare result bit 1 is activated.
		IPINTBANK2_SNDCOMPARE_RESULT_BIT2		= 0x00020000,	// Bit 17, ALU2's compare result bit 2 is activated.
		IPINTBANK2_SNDCOMPARE_RESULT_BIT3		= 0x00040000,	// Bit 18, ALU2's compare result bit 3 is activated.
		IPINTBANK2_SNDCOMPARE_RESULT_BIT4		= 0x00080000,	// Bit 19, ALU2's compare result bit 4 is activated.
		IPINTBANK2_L_C_INP_Q_LESS_4				= 0x00100000,	// Bit 20, Linear/Circular interpolation parameter queue is less than 4.
		IPINTBANK2_P_INP_Q_LESS_4				= 0x00200000,	// Bit 21, Pattern interpolation parameter queue is less than 4.
		IPINTBANK2_XSYNC_ACTIVATED				= 0x00400000,	// Bit 22, X axis sync input signal is activated.
		IPINTBANK2_YSYNC_ACTIVATED				= 0x00800000,	// Bit 23, Y axis sync input siangl is activated.
		IPINTBANK2_P_INP_END_BY_END_PATTERN		= 0x01000000	// Bit 24, Bit pattern interpolation is terminated by end pattern.
	//	IPINTBANK2_								= 0x02000000,	// Bit 25, Don't care.
	//	IPINTBANK2_								= 0x04000000,	// Bit 26, Don't care.
	//	IPINTBANK2_								= 0x08000000,	// Bit 27, Don't care.
	//	IPINTBANK2_								= 0x10000000,	// Bit 28, Don't care.
	//	IPINTBANK2_								= 0x20000000,	// Bit 29, Don't care.
	//	IPINTBANK2_								= 0x40000000,	// Bit 30, Don't care.
	//	IPINTBANK2_								= 0x80000000	// Bit 31, Don't care.
	} AXT_MOTION_IPINTERRUPT_BANK2;
#endif


#ifndef AXT_MOTION_IPMECHANICAL_SIGNAL_DEF
	#define AXT_MOTION_IPMECHANICAL_SIGNAL_DEF
	/* FS Drive status						*/
	typedef	enum _AXT_MOTION_IPMECHANICAL_SIGNAL
	{

		IPMECHANICAL_PELM_LEVEL				= 0x0001,			// Bit 0, +Limit 급정지 신호가 액티브 됨
		IPMECHANICAL_NELM_LEVEL				= 0x0002,			// Bit 1, -Limit 급정지 신호 액티브 됨
		IPMECHANICAL_PSLM_LEVEL				= 0x0004,			// Bit 2, +limit 감속정지 신호 액티브 됨
		IPMECHANICAL_NSLM_LEVEL				= 0x0008,			// Bit 3, -limit 감속정지 신호 액티브 됨
		IPMECHANICAL_ALARM_LEVEL			= 0x0010,			// Bit 4, Alarm 신호 액티브 됨
		IPMECHANICAL_INP_LEVEL				= 0x0020,			// Bit 5, Inposition 신호 액티브 됨
		IPMECHANICAL_ENC_DOWN_LEVEL			= 0x0040,			// Bit 6, 엔코더 DOWN(B상) 신호 입력 Level
		IPMECHANICAL_ENC_UP_LEVEL			= 0x0080,			// Bit 7, 엔코더 UP(A상) 신호 입력 Level
		IPMECHANICAL_EXMP_LEVEL				= 0x0100,			// Bit 8, EXMP 신호 입력 Level
		IPMECHANICAL_EXPP_LEVEL				= 0x0200,			// Bit 9, EXPP 신호 입력 Level
		IPMECHANICAL_MARK_LEVEL				= 0x0400,			// Bit 10, MARK# 신호 액티브 됨
		IPMECHANICAL_SSTOP_LEVEL			= 0x0800,			// Bit 11, SSTOP 신호 액티브 됨
		IPMECHANICAL_ESTOP_LEVEL			= 0x1000,			// Bit 12, ESTOP 신호 액티브 됨
		IPMECHANICAL_SYNC_LEVEL				= 0x2000,			// Bit 13, SYNC 신호 입력 Level
		IPMECHANICAL_MODE8_16_LEVEL			= 0x4000			// Bit 14, MODE8_16 신호 입력 Level

	} AXT_MOTION_IPMECHANICAL_SIGNAL;
#endif

#ifndef AXT_MOTION_QIDETECT_DESTINATION_SIGNAL_DEF
	#define AXT_MOTION_QIDETECT_DESTINATION_SIGNAL_DEF
	/* Detect Destination Signal			*/
	typedef enum _AXT_MOTION_QIDETECT_DESTINATION_SIGNAL
	{
		Signal_PosEndLimit								= 0x0,			// +Elm(End limit) +방향 리미트 센서 신호
		Signal_NegEndLimit								= 0x1,			// -Elm(End limit) -방향 리미트 센서 신호
		Signal_PosSloLimit								= 0x2,			// +Slm(Slow Down limit) 신호 - 사용하지 않음
		Signal_NegSloLimit								= 0x3,			// -Slm(Slow Down limit) 신호 - 사용하지 않음
		Signal_HomeSensor								= 0x4,			// IN0(ORG)  원점 센서 신호
		Signal_EncodZPhase								= 0x5,			// IN1(Z상)  Encoder Z상 신호
		Signal_UniInput02								= 0x6,			// IN2(범용) 범용 입력 2번 신호
		Signal_UniInput03								= 0x7			// IN3(범용) 범용 입력 3번 신호
	} AXT_MOTION_QIDETECT_DESTINATION_SIGNAL;
#endif

#ifndef AXT_MOTION_QIMECHANICAL_SIGNAL_DEF
	#define AXT_MOTION_QIMECHANICAL_SIGNAL_DEF
	/* FS Drive status						*/
	typedef	enum _AXT_MOTION_QIMECHANICAL_SIGNAL
	{

		QIMECHANICAL_PELM_LEVEL				= 0x00001,			// Bit 0, +Limit 급정지 신호 현재 상태
		QIMECHANICAL_NELM_LEVEL				= 0x00002,			// Bit 1, -Limit 급정지 신호 현재 상태
		QIMECHANICAL_PSLM_LEVEL				= 0x00004,			// Bit 2, +limit 감속정지 현재 상태.
		QIMECHANICAL_NSLM_LEVEL				= 0x00008,			// Bit 3, -limit 감속정지 현재 상태
		QIMECHANICAL_ALARM_LEVEL			= 0x00010,			// Bit 4, Alarm 신호 현재 상태
		QIMECHANICAL_INP_LEVEL				= 0x00020,			// Bit 5, Inposition 신호 현재 상태
		QIMECHANICAL_ESTOP_LEVEL			= 0x00040,			// Bit 6, 비상 정지 신호(ESTOP) 현재 상태.
		QIMECHANICAL_ORG_LEVEL				= 0x00080,			// Bit 7, 원점 신호 헌재 상태
		QIMECHANICAL_ZPHASE_LEVEL			= 0x00100,			// Bit 8, Z 상 입력 신호 현재 상태
		QIMECHANICAL_ECUP_LEVEL				= 0x00200,			// Bit 9, ECUP 터미널 신호 상태.
		QIMECHANICAL_ECDN_LEVEL				= 0x00400,			// Bit 10, ECDN 터미널 신호 상태.
		QIMECHANICAL_EXPP_LEVEL				= 0x00800,			// Bit 11, EXPP 터미널 신호 상태
		QIMECHANICAL_EXMP_LEVEL				= 0x01000,			// Bit 12, EXMP 터미널 신호 상태
		QIMECHANICAL_SQSTR1_LEVEL			= 0x02000,			// Bit 13, SQSTR1 터미널 신호 상태
	    QIMECHANICAL_SQSTR2_LEVEL			= 0x04000,			// Bit 14, SQSTR2 터미널 신호 상태
		QIMECHANICAL_SQSTP1_LEVEL			= 0x08000,			// Bit 15, SQSTP1 터미널 신호 상태
		QIMECHANICAL_SQSTP2_LEVEL			= 0x10000,			// Bit 16, SQSTP2 터미널 신호 상태
		QIMECHANICAL_MODE_LEVEL				= 0x20000			// Bit 17, MODE 터미널 신호 상태.

	} AXT_MOTION_QIMECHANICAL_SIGNAL;
#endif

#ifndef AXT_MOTION_QIEND_STATUS_DEF
	#define AXT_MOTION_QIEND_STATUS_DEF
	/* IP End status : 0x0000이면 정상종료	*/
	typedef	enum _AXT_MOTION_QIEND_STATUS
	{
		QIEND_STATUS_0							= 0x00000001,		// Bit 0, 정방향 리미트 신호(PELM)에 의한 종료
		QIEND_STATUS_1							= 0x00000002,		// Bit 1, 역방향 리미트 신호(NELM)에 의한 종료
		QIEND_STATUS_2							= 0x00000004,		// Bit 2, 정방향 부가 리미트 신호(PSLM)에 의한 구동 종료
		QIEND_STATUS_3							= 0x00000008,		// Bit 3, 역방향 부가 리미트 신호(NSLM)에 의한 구동 종료
		QIEND_STATUS_4							= 0x00000010,		// Bit 4, 정방향 소프트 리미트 급정지 기능에 의한 구동 종료
		QIEND_STATUS_5							= 0x00000020,		// Bit 5, 역방향 소프트 리미트 급정지 기능에 의한 구동 종료
		QIEND_STATUS_6							= 0x00000040,		// Bit 6, 정방향 소프트 리미트 감속정지 기능에 의한 구동 종료
		QIEND_STATUS_7							= 0x00000080,		// Bit 7, 역방향 소프트 리미트 감속정지 기능에 의한 구동 종료
		QIEND_STATUS_8							= 0x00000100,		// Bit 8, 서보 알람 기능에 의한 구동 종료.
		QIEND_STATUS_9							= 0x00000200,		// Bit 9, 비상 정지 신호 입력에 의한 구동 종료.
		QIEND_STATUS_10							= 0x00000400,		// Bit 10, 급 정지 명령에 의한 구동 종료.
		QIEND_STATUS_11							= 0x00000800,		// Bit 11, 감속 정지 명령에 의한 구동 종료.
		QIEND_STATUS_12							= 0x00001000,		// Bit 12, 전축 급정지 명령에 의한 구동 종료
		QIEND_STATUS_13							= 0x00002000,		// Bit 13, 동기 정지 기능 #1(SQSTP1)에 의한 구동 종료.
		QIEND_STATUS_14							= 0x00004000,		// Bit 14, 동기 정지 기능 #2(SQSTP2)에 의한 구동 종료.
		QIEND_STATUS_15							= 0x00008000,		// Bit 15, 인코더 입력(ECUP,ECDN) 오류 발생
		QIEND_STATUS_16							= 0x00010000,		// Bit 16, MPG 입력(EXPP,EXMP) 오류 발생
		QIEND_STATUS_17							= 0x00020000,		// Bit 17, 원점 검색 성공 종료.
		QIEND_STATUS_18							= 0x00040000,		// Bit 18, 신호 검색 성공 종료.
		QIEND_STATUS_19							= 0x00080000,		// Bit 19, 보간 데이터 이상으로 구동 종료.
		QIEND_STATUS_20							= 0x00100000,		// Bit 20, 비정상 구동 정지발생.
		QIEND_STATUS_21							= 0x00200000,		// Bit 21, MPG 기능 블록 펄스 버퍼 오버플로우 발생
		QIEND_STATUS_22							= 0x00400000,		// Bit 22, DON'CARE
		QIEND_STATUS_23							= 0x00800000,		// Bit 23, DON'CARE
		QIEND_STATUS_24							= 0x01000000,		// Bit 24, DON'CARE
		QIEND_STATUS_25							= 0x02000000,		// Bit 25, DON'CARE
		QIEND_STATUS_26							= 0x04000000,		// Bit 26, DON'CARE
		QIEND_STATUS_27							= 0x08000000,		// Bit 27, DON'CARE
		QIEND_STATUS_28							= 0x10000000,		// Bit 28, 현재/마지막 구동 드라이브 방향
		QIEND_STATUS_29							= 0x20000000,		// Bit 29, 잔여 펄스 제거 신호 출력 중.
		QIEND_STATUS_30							= 0x40000000,		// Bit 30, 비정상 구동 정지 원인 상태
		QIEND_STATUS_31							= 0x80000000		// Bit 31, 보간 드라이브 데이타 오류 상태.

	} AXT_MOTION_QIEND_STATUS;
#endif

#ifndef AXT_MOTION_QIDRIVE_STATUS_DEF
	#define AXT_MOTION_QIDRIVE_STATUS_DEF
	/* FS Drive status						*/
	typedef	enum _AXT_MOTION_QIDRIVE_STATUS
	{
		QIDRIVE_STATUS_0						= 0x0000001,		// Bit 0, BUSY(드라이브 구동 중)
		QIDRIVE_STATUS_1						= 0x0000002,		// Bit 1, DOWN(감속 중)
		QIDRIVE_STATUS_2						= 0x0000004,		// Bit 2, CONST(등속 중)
		QIDRIVE_STATUS_3						= 0x0000008,		// Bit 3, UP(가속 중)
		QIDRIVE_STATUS_4						= 0x0000010,		// Bit 4, 연속 드라이브 구동 중
		QIDRIVE_STATUS_5						= 0x0000020,		// Bit 5, 지정 거리 드라이브 구동 중
		QIDRIVE_STATUS_6						= 0x0000040,		// Bit 6, MPG 드라이브 구동 중
		QIDRIVE_STATUS_7						= 0x0000080,		// Bit 7, 원점검색 드라이브 구동중
		QIDRIVE_STATUS_8						= 0x0000100,		// Bit 8, 신호 검색 드라이브 구동 중
		QIDRIVE_STATUS_9						= 0x0000200,		// Bit 9, 보간 드라이브 구동 중
		QIDRIVE_STATUS_10						= 0x0000400,		// Bit 10, Slave 드라이브 구동중
		QIDRIVE_STATUS_11						= 0x0000800,		// Bit 11, 현재 구동 드라이브 방향(보간 드라이브에서는 표시 정보 다름)
		QIDRIVE_STATUS_12						= 0x0001000,		// Bit 12, 펄스 출력후 서보위치 완료 신호 대기중.
		QIDRIVE_STATUS_13						= 0x0002000,		// Bit 13, 직선 보간 드라이브 구동중.
		QIDRIVE_STATUS_14						= 0x0004000,		// Bit 14, 원호 보간 드라이브 구동중.
		QIDRIVE_STATUS_15						= 0x0008000,		// Bit 15, 펄스 출력 중.
		QIDRIVE_STATUS_16						= 0x0010000,		// Bit 16, 구동 예약 데이터 개수(처음)(0-7)
		QIDRIVE_STATUS_17						= 0x0020000,		// Bit 17, 구동 예약 데이터 개수(중간)(0-7)
		QIDRIVE_STATUS_18						= 0x0040000,		// Bit 18, 구동 예약 데이터 갯수(끝)(0-7)
		QIDRIVE_STATUS_19						= 0x0100000,		// Bit 19, 구동 예약 Queue 비어 있음.
		QIDRIVE_STATUS_20						= 0x0200000,		// Bit 20, 구동 예약 Queue 가득 찲
		QIDRIVE_STATUS_21						= 0x0400000,		// Bit 21, 현재 구동 드라이브의 속도 모드(처음)
		QIDRIVE_STATUS_22						= 0x0800000,		// Bit 22, 현재 구동 드라이브의 속도 모드(끝)
		QIDRIVE_STATUS_23						= 0x1000000,		// Bit 23, MPG 버퍼 #1 Full
		QIDRIVE_STATUS_24						= 0x2000000,		// Bit 24, MPG 버퍼 #2 Full
		QIDRIVE_STATUS_25						= 0x4000000,		// Bit 25, MPG 버퍼 #3 Full
		QIDRIVE_STATUS_26						= 0x8000000			// Bit 26, MPG 버퍼 데이터 OverFlow

	} AXT_MOTION_QIDRIVE_STATUS;
#endif

#ifndef AXT_MOTION_QIINTERRUPT_BANK1_DEF
	#define AXT_MOTION_QIINTERRUPT_BANK1_DEF
	/* IP Interrupt MASK setting			*/
	typedef	enum _AXT_MOTION_QIINTERRUPT_BANK1
	{
		QIINTBANK1_DISABLE						= 0x00000000,	// INTERRUT DISABLED.
		QIINTBANK1_0							= 0x00000001,	// Bit 0,  인터럽트 발생 사용 설정된 구동 종료시.
		QIINTBANK1_1							= 0x00000002,	// Bit 1,  구동 종료시
		QIINTBANK1_2							= 0x00000004,	// Bit 2,  구동 시작시.
		QIINTBANK1_3							= 0x00000008,	// Bit 3,  카운터 #1 < 비교기 #1 이벤트 발생
		QIINTBANK1_4							= 0x00000010,	// Bit 4,  카운터 #1 = 비교기 #1 이벤트 발생
		QIINTBANK1_5							= 0x00000020,	// Bit 5,  카운터 #1 > 비교기 #1 이벤트 발생
		QIINTBANK1_6							= 0x00000040,	// Bit 6,  카운터 #2 < 비교기 #2 이벤트 발생
		QIINTBANK1_7							= 0x00000080,	// Bit 7,  카운터 #2 = 비교기 #2 이벤트 발생
		QIINTBANK1_8							= 0x00000100,	// Bit 8,  카운터 #2 > 비교기 #2 이벤트 발생
		QIINTBANK1_9							= 0x00000200,	// Bit 9,  카운터 #3 < 비교기 #3 이벤트 발생
		QIINTBANK1_10							= 0x00000400,	// Bit 10, 카운터 #3 = 비교기 #3 이벤트 발생
		QIINTBANK1_11							= 0x00000800,	// Bit 11, 카운터 #3 > 비교기 #3 이벤트 발생
		QIINTBANK1_12							= 0x00001000,	// Bit 12, 카운터 #4 < 비교기 #4 이벤트 발생
		QIINTBANK1_13							= 0x00002000,	// Bit 13, 카운터 #4 = 비교기 #4 이벤트 발생
		QIINTBANK1_14							= 0x00004000,	// Bit 14, 카운터 #4 < 비교기 #4 이벤트 발생
		QIINTBANK1_15							= 0x00008000,	// Bit 15, 카운터 #5 < 비교기 #5 이벤트 발생
		QIINTBANK1_16							= 0x00010000,	// Bit 16, 카운터 #5 = 비교기 #5 이벤트 발생
		QIINTBANK1_17							= 0x00020000,	// Bit 17, 카운터 #5 > 비교기 #5 이벤트 발생
		QIINTBANK1_18							= 0x00040000,	// Bit 18, 타이머 #1 이벤트 발생.
		QIINTBANK1_19							= 0x00080000,	// Bit 19, 타이머 #2 이벤트 발생.
		QIINTBANK1_20							= 0x00100000,	// Bit 20, 구동 예약 설정 Queue 비워짐.
		QIINTBANK1_21							= 0x00200000,	// Bit 21, 구동 예약 설정 Queue 가득찲
		QIINTBANK1_22							= 0x00400000,	// Bit 22, 트리거 발생거리 주기/절대위치 Queue 비워짐.
		QIINTBANK1_23							= 0x00800000,	// Bit 23, 트리거 발생거리 주기/절대위치 Queue 가득찲
		QIINTBANK1_24							= 0x01000000,	// Bit 24, 트리거 신호 발생 이벤트
		QIINTBANK1_25							= 0x02000000,	// Bit 25, 스크립트 #1 명령어 예약 설정 Queue 비워짐.
		QIINTBANK1_26							= 0x04000000,	// Bit 26, 스크립트 #2 명령어 예약 설정 Queue 비워짐.
		QIINTBANK1_27							= 0x08000000,	// Bit 27, 스크립트 #3 명령어 예약 설정 레지스터 실행되어 초기화 됨.
		QIINTBANK1_28							= 0x10000000,	// Bit 28, 스크립트 #4 명령어 예약 설정 레지스터 실행되어 초기화 됨.
		QIINTBANK1_29							= 0x20000000,	// Bit 29, 서보 알람신호 인가됨.
		QIINTBANK1_30							= 0x40000000,	// Bit 30, |CNT1| - |CNT2| >= |CNT4| 이벤트 발생.
		QIINTBANK1_31							= 0x80000000	// Bit 31, 인터럽트 발생 명령어|INTGEN| 실행.
	} AXT_MOTION_QIINTERRUPT_BANK1;
#endif

#ifndef AXT_MOTION_QIINTERRUPT_BANK2_DEF
	#define AXT_MOTION_QIINTERRUPT_BANK2_DEF
	typedef	enum _AXT_MOTION_QIINTERRUPT_BANK2
	{
		QIINTBANK2_DISABLE						= 0x00000000,	// INTERRUT DISABLED.
		QIINTBANK2_0							= 0x00000001,	// Bit 0,  스크립트 #1 읽기 명령 결과 Queue 가 가득찲.
		QIINTBANK2_1							= 0x00000002,	// Bit 1,  스크립트 #2 읽기 명령 결과 Queue 가 가득찲.
		QIINTBANK2_2							= 0x00000004,	// Bit 2,  스크립트 #3 읽기 명령 결과 레지스터가 새로운 데이터로 갱신됨.
		QIINTBANK2_3							= 0x00000008,	// Bit 3,  스크립트 #4 읽기 명령 결과 레지스터가 새로운 데이터로 갱신됨.
		QIINTBANK2_4							= 0x00000010,	// Bit 4,  스크립트 #1 의 예약 명령어 중 실행 시 인터럽트 발생으로 설정된 명령어 실행됨.
		QIINTBANK2_5							= 0x00000020,	// Bit 5,  스크립트 #2 의 예약 명령어 중 실행 시 인터럽트 발생으로 설정된 명령어 실행됨.
		QIINTBANK2_6							= 0x00000040,	// Bit 6,  스크립트 #3 의 예약 명령어 실행 시 인터럽트 발생으로 설정된 명령어 실행됨.
		QIINTBANK2_7							= 0x00000080,	// Bit 7,  스크립트 #4 의 예약 명령어 실행 시 인터럽트 발생으로 설정된 명령어 실행됨.
		QIINTBANK2_8							= 0x00000100,	// Bit 8,  구동 시작
		QIINTBANK2_9							= 0x00000200,	// Bit 9,  서보 위치 결정 완료(Inposition)기능을 사용한 구동,종료 조건 발생.
		QIINTBANK2_10							= 0x00000400,	// Bit 10, 이벤트 카운터로 동작 시 사용할 이벤트 선택 #1 조건 발생.
		QIINTBANK2_11							= 0x00000800,	// Bit 11, 이벤트 카운터로 동작 시 사용할 이벤트 선택 #2 조건 발생.
		QIINTBANK2_12							= 0x00001000,	// Bit 12, SQSTR1 신호 인가 됨.
		QIINTBANK2_13							= 0x00002000,	// Bit 13, SQSTR2 신호 인가 됨.
		QIINTBANK2_14							= 0x00004000,	// Bit 14, UIO0 터미널 신호가 '1'로 변함.
		QIINTBANK2_15							= 0x00008000,	// Bit 15, UIO1 터미널 신호가 '1'로 변함.
		QIINTBANK2_16							= 0x00010000,	// Bit 16, UIO2 터미널 신호가 '1'로 변함.
		QIINTBANK2_17							= 0x00020000,	// Bit 17, UIO3 터미널 신호가 '1'로 변함.
		QIINTBANK2_18							= 0x00040000,	// Bit 18, UIO4 터미널 신호가 '1'로 변함.
		QIINTBANK2_19							= 0x00080000,	// Bit 19, UIO5 터미널 신호가 '1'로 변함.
		QIINTBANK2_20							= 0x00100000,	// Bit 20, UIO6 터미널 신호가 '1'로 변함.
		QIINTBANK2_21							= 0x00200000,	// Bit 21, UIO7 터미널 신호가 '1'로 변함.
		QIINTBANK2_22							= 0x00400000,	// Bit 22, UIO8 터미널 신호가 '1'로 변함.
		QIINTBANK2_23							= 0x00800000,	// Bit 23, UIO9 터미널 신호가 '1'로 변함.
		QIINTBANK2_24							= 0x01000000,	// Bit 24, UIO10 터미널 신호가 '1'로 변함.
		QIINTBANK2_25							= 0x02000000,	// Bit 25, UIO11 터미널 신호가 '1'로 변함.
		QIINTBANK2_26							= 0x04000000,	// Bit 26, 오류 정지 조건(LMT, ESTOP, STOP, ESTOP, CMD, ALARM) 발생.
		QIINTBANK2_27							= 0x08000000,	// Bit 27, 보간 중 데이터 설정 오류 발생.
		QIINTBANK2_28							= 0x10000000,	// Bit 28, Don't Care
		QIINTBANK2_29							= 0x20000000,	// Bit 29, 리미트 신호(PELM, NELM)신호가 입력 됨.
		QIINTBANK2_30							= 0x40000000,	// Bit 30, 부가 리미트 신호(PSLM, NSLM)신호가 입력 됨.
		QIINTBANK2_31							= 0x80000000	// Bit 31, 비상 정지 신호(ESTOP)신호가 입력됨.

	} AXT_MOTION_QIINTERRUPT_BANK2;
#endif

typedef void (__stdcall *AXT_EVENT_PROC)(long lActiveNo, DWORD uFlag);
typedef void (__stdcall *AXT_INTERRUPT_PROC)(long lActiveNo, DWORD uFlag);

#endif	//__AXT_AXHS_H__
