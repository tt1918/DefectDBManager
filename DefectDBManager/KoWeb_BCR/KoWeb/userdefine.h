#ifdef INTERFACE_NEXTEYE_SERVER

	#define MAIN_FOLDER					_T("C:\\Nexteye\\")
	#define LOG_PATH					_T("C:\\Nexteye\\SystemLog\\")
	#define SYSTEM_PATH					_T("C:\\Nexteye\\System.ini")
	#define CONFIG_PATH					_T("C:\\Nexteye\\Config.ini")
	#define MODEL_PATH					_T("C:\\Nexteye\\Model\\")
	#define IMAGE_SAVE_PATH				_T("C:\\Nexteye\\Image\\")
	#define LOT_PATH					_T("C:\\Nexteye\\LOG\\")
	#define RESERVE_PATH				_T("C:\\Nexteye\\Reserve\\")
	#define LATEST_LOT_PATH				_T("C:\\Nexteye\\LatestLot.ini")
	#define NG_IMAGE_PATH				_T("C:\\Nexteye\\LOTDATA\\")
	#define PARAMETER_PATH				_T("C:\\Nexteye\\parameter.ini")
	#define PARAMETER_PATH_RESERVE		_T("C:\\Nexteye\\Reserve\\parameter.ini")
	#define SUBMARKPARAM_PATH_RESERVE	_T("C:\\Nexteye\\Reserve\\SubMarkParam.ini")		//USE_20_LEVEL_MARK
	#define KIPOPRESS_PATH				_T("C:\\Nexteye\\KipoPress.ini")
	#define PROJECTION_PATH				_T("C:\\Nexteye\\Projection.ini")
	#define PARAMNAME_PATH				_T("C:\\Nexteye\\ParamName.ini")
	#define SUBMARKPARAM_PATH			_T("C:\\Nexteye\\SubMarkParam.ini")				//USE_20_LEVEL_MARK
	#define PARAMGRADENAME_PATH			_T("C:\\Nexteye\\ParamGradeName.ini")
	#define PARAMLEVELNAME_PATH			_T("C:\\Nexteye\\ParamLevelName.ini")			
	#define LINEOFFSET_PATH				_T("C:\\Nexteye\\LineOffset.ini")
	#define LINEOFFSET_PATH1			_T("C:\\Nexteye\\LineOffset1.ini")
	#define LINEOFFSET_PATH2			_T("C:\\Nexteye\\LineOffset2.ini")
	#define LINEOFFSET_PATH3			_T("C:\\Nexteye\\LineOffset3.ini")
	#define DCF_FILE					_T("C:\\Nexteye\\nexteye.dcf")
	#define EURESYS_FILE				_T("C:\\Nexteye\\config.es")		//유레시스 grabber 설정파일

#else
	#define MAIN_FOLDER					_T("C:\\COSS\\")
	#define LOG_PATH					_T("C:\\COSS\\SystemLog\\")
	#define SYSTEM_PATH					_T("C:\\COSS\\System.ini")
	#define CONFIG_PATH					_T("C:\\COSS\\Config.ini")
	#define MODEL_PATH					_T("C:\\COSS\\Model\\")
	#define IMAGE_SAVE_PATH				_T("C:\\COSS\\Image\\")
	#define LOT_PATH					_T("C:\\COSS\\LOG\\")
	#define RESERVE_PATH				_T("C:\\COSS\\Reserve\\")
	#define LATEST_LOT_PATH				_T("C:\\COSS\\LatestLot.ini")
	#define NG_IMAGE_PATH				_T("C:\\COSS\\LOTDATA\\")
	#define PARAMETER_PATH				_T("C:\\COSS\\parameter.ini")
	#define PARAMETER_PATH_RESERVE		_T("C:\\COSS\\Reserve\\parameter.ini")
	#define SUBMARKPARAM_PATH_RESERVE	_T("C:\\COSS\\Reserve\\SubMarkParam.ini")
	#define KIPOPRESS_PATH				_T("C:\\COSS\\KipoPress.ini")
	#define PROJECTION_PATH				_T("C:\\COSS\\Projection.ini")
	#define PARAMNAME_PATH				_T("C:\\COSS\\ParamName.ini")
	#define SUBMARKPARAM_PATH			_T("C:\\COSS\\SubMarkParam.ini")
	#define PARAMGRADENAME_PATH			_T("C:\\COSS\\ParamGradeName.ini")
	#define PARAMLEVELNAME_PATH			_T("C:\\COSS\\ParamLevelName.ini")
	#define NGNAME_PATH					_T("C:\\COSS\\NGName.ini")
	#define LINEOFFSET_PATH				_T("C:\\COSS\\LineOffset.ini")
	#define LINEOFFSET_PATH1			_T("C:\\COSS\\LineOffset1.ini")
	#define LINEOFFSET_PATH2			_T("C:\\COSS\\LineOffset2.ini")
	#define LINEOFFSET_PATH3			_T("C:\\COSS\\LineOffset3.ini")
	#define DCF_FILE					_T("C:\\COSS\\CossCam.dcf")
	#define EURESYS_FILE				_T("C:\\COSS\\config.es")		//유레시스 grabber 설정파일
	#define COG_VPP_FILE				_T("C:\\COSS\\codeRead.vpp")
	#define BCR_PATH					_T("C:\\COSS\\Barcode\\")
	#define BCR_EDGE_PATH				_T("C:\\COSS\\EdgeInfo\\")
#endif


#define MAX_LEVEL					10		//최대 Level 수

#ifdef USE_HEX_FORMAT
	#define MILLION					0x1000000
	#define CLASSDEV				0x10
	#define OPTICDEV				0x100
	#define PERIODDEV				0x10000
	#define MAX_OPTIC				12		//최대 광학계 수  //미세이물, USE_HEX_FORMAT 사용할때 12개로 바꿈
#else
	#define MILLION					1000000
	#define CLASSDEV				10
	#define OPTICDEV				100
	#define PERIODDEV				10000
	#define MAX_OPTIC				10		//최대 광학계 수  //미세이물, USE_HEX_FORMAT 사용할때 12개로 바꿈 USE_MISE_IMUL NEL1일ㄸ때
#endif

#define CYCLELENGTH					10000
#define MAX_CYCLE_COUNT				15


#define MAX_DEFECT					100		//최대 불량수 g_System.m_nMaxDefect 를 사용함. g_System.m_nMaxDefect 이 값이 30개를 넘을 수 없음. (30->100 될때 메모리 1.1MByte 늘어남)
#define BAD_IMG_WIDTH				128
#define BAD_IMG_HEIGHT				128
#define	MAX_CROSS_WIDTH				2048    //Area MAX 폭

#define SUB_FRAME_HEIGHT			128		//앞뒤 저장하는 버퍼크기

//NEXTEYE 서버에서의 순서-----------------------------
#define COS1_NE			5	//SH 크로스
#define COS2_NE			6	//투영반사
#define COS3_NE			1	//미분투과 
#define COS4_NE			4	//정투과
#define COS5_NE			7	//투영/경계투과
#define COS6_NE			3	//정반사B(보호면검사)
#define COS7_NE			2	//정반사A(이형면검사) 
#define COS8_NE			8	//경계투과  
#define COS9_NE			0	//크로스
#define COSA_NE			9	//미정
#define COSB_NE			10	//미정
#define COSC_NE			11	//사용안함
#define COSD_NE			12	//사용안함
#define COSE_NE			13	//사용안함
#define COSF_NE			14	//사용안함
//---------------------------------------------------

#define COS1			0x01	//SH 크로스
#define COS2			0x02	//투영반사
#define COS3			0x03	//미분투과 
#define COS4			0x04	//정투과
#define COS5			0x05	//투영/경계투과
#define COS6			0x06	//정반사B(보호면검사)
#define COS7			0x07	//정반사A(이형면검사) 
#define COS8			0x08	//경계투과  
#define COS9			0x09	//크로스
#define COSA			0x00	//투영반사  =========================>원래 0x0A를 사용해야 하나 LG에서 이렇게 사용함.
#define COSB			0x0B	//크로스2 (2동코팅3에만 사용함)
#define COSC			0x0C	//
#define COSD			0x0D	//
#define CBCR			0x0E	//BCR	   14				
#define CMAK			0x0F	//마킹비전 15


#define		DEFAULT_PORT		6000		//서버
#define		MARKING_PORT_RED	7000		//RED 마킹
#define		MARKING_PORT_BLUE	7500		//BLUE마킹
#define     RECV_LR_PORT		7800		//LR서버 포트
#define     KIPO_PORT			7300		//투영이 정반사한테 데이타 주는 포트
#define     NITTO_AI_PORT		9000		//Nitto AI에게 주는 포트

#define		WM_XDATARRECIEVED	(WM_USER+102)
#define		WM_BCR_COMM			(WM_USER+103)


#define	WM_CONNECT			WM_USER+11
#define	WM_CAMYPOS			WM_USER+13
#define WM_RESTART			WM_USER+14


#define MAX_NITTO_AI_CLASS 10



//ERROR CODE
#define ERROR_BRIGHTNESS		1		//내부 밝기가 범위 벗어남.