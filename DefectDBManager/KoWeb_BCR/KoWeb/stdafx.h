
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes
#include <locale.h>			//한글 사용


#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars
#pragma comment(lib, "UxTheme.lib")  //OnCtlColor
#pragma comment(lib,"Windowscodecs.lib")


//#define NANJING_OLED					//남경 OLED이면 사용함.(COS9)
//#define MARKING_VISION				//이 클라이언트를 마킹비전 클라이언트로 사용할때 살린다. (MAK-101, MAK-102) 
#define BARCODE_VISION				//Barcode사용하면 살린다. (BCR-101, BCR-102)	, BCR 클라이언트 선언

#ifdef BARCODE_VISION
#import "BCR/DefectDBManager.tlb" no_namespace, named_guids
#define USE_CODEREADERDLL				// 바코드 라이브러리 활성화
//#define BCR_SIMUL_TEST			// 바코드 시뮬레이션 테스트
#endif

//#define SERVER_TEST				    //3동정밀코팅에서 SERVER TEST 시에만 사용한다. (100.0.0.244)에 보냄
//#define USE_DECODING_RESET			//Decoding에서 Reset관련 신호가 오면 현재 검사파일을 지운다.& 스크라치 검사 Upper Cut값을 올림  [3동정밀라미 에서만 사용함]

//#define USE_COS9_LR					//3동정밀코팅2호B [COS9,LR반사] 에서만 사용함 Inspect9();


#define TOT_USE							//TOT에서 사용하는 define 묶음

#ifdef  TOT_USE
	#define USE_NITTO_AI				//불량영상을 Nitto AI프로세스로 보내서 처리 결과를 받아 같이 보냄
	#define RESULT_DELAY				//결과를 다음 Frame촬상끝나고 보내는 것이 아니라  지정된 다음에 보냄 [AI사용하는 경우 사용]
	#define USE_NEXT_GRAB				//Frame의 Grab이 끝나면  이전 Frame을 검사한다. (중간 Frame검사, 이전 Frame, 다음 Frame있음)
#ifndef BARCODE_VISION
	#define USE_HOLE_SKIP				//2~3mm Hole을 제거한다. 평활화된 영상에서 처리, 중간은 1/8 압축영상에서 Hole Skip, 가장자리는 3mm 정도영역만 따로 검사해서 Hole Skip)
#endif
	#define USE_PACKET_ANSI				//Packet 에서 사용하는 String은 1Byte로												[TOT 만사용]				
#endif




//#define USE_SK_BAT					//SK밧데리 검사일때만 살린다
#ifndef USE_SK_BAT						//SK밧데리 아닐떄

//#define MARKING_ORDER_OLD_VERSION		//마킹 광학계 순서를 크로스 1로 사용할때 살린다.
//#define USE_COSS_SERVER_CLIENT_NEL	/COSS 서버와 통신하지만 클라이언트 이름을 NEL인경우 사용(광학계순서를 크로스 0 부터 시작하는 경우)

//#define  INTERFACE_NEXTEYE_SERVER		//Nexteye 서버와 통신하려면 NEXTEYE Format에 맞게사용 (TCHAR 대신 char를 사용)  [넥스트아이서버 사용하면 살리고, COSS서버 사용하면 죽임]
#ifdef INTERFACE_NEXTEYE_SERVER
	#define  CHECK_LOT_CHANGE_SIGNAL	
	#define  USE_PACKET_ANSI					//Packet 에서 사용하는 String은 1Byte로	
#endif


//#define USE_20_LEVEL_MARK				//20Level마킹 사용하는 경우                        [오창 연신/코팅에 사용]

//#define USE_MISE_IMUL					//2동 코팅 3호기 미세이물검사기					   [2동3호기중 NEL1, NELB에만 사용]

			

//#define USE_DAKANO_LINE					//크로스 기포라인 사용하면 살린다. (8192x8192), 아래한줄도 엮임
#define DAKANO_WIDTH	64				//MAP 할줄 폭
#endif

//#define   COS5_GUANGJOO				//COS5(투영)의 불량순서가 광저우는 다름, 광저우 일때만 살린다.


//실제검사사용에서는 모두 죽이는 DEFINE---------------------------------------------------
#define TEST_MODE						//테스트용으로 사용하려면 살림.						[사용안함]
//#define GRAB_TEST_MODE				//PC번호를 설정하고 Grab하면서 테스트 할때 사용		[사용안함]
//#define SHOW_DEBUG_DATA				//디버그용 데이타를 표시 (검사시간 표시)			[사용안함]
//#define NO_GRABER_SYNC_FRAME_FROM_SERVER	//테스트용으로 Server로부터 동기신호받아 검사	[사용안함]
//---------------------------------------------------------------------------------------

#define LONG_SCRATCH_EMPHASIZE			//남경에서 사용함
					 

//#define PRESS_FIRST_USE			//사용하면 찍힘파라미터가  (param[110~], param[120~])  사용하지 않으면 (param[130~], param[140~])    //USE_SK_BAT일때는 사용하지 않는다.
									//중국, 라미1호기는 사용하고  국내 연신코팅에는 사용하지 않는다.
//#define MARKING_REMOVE			//마킹불량은 검사에서 제외함.
//#define REMOVE_BUMP_OVERKILL		//경계반사에서 울렁거리는 과검 없앤다.


//속도관련  ---------------------------------------------------------------------------------------------------------------------------------------
#define SHOW_USE_THREAD					//Show Image를 THREAD를 사용할떄													[사용가능]
#define USE_TOO_MURA					//투영 무라 검사할때 사용(투영 흑 얼룩검사)											[사용가능]
//#define SPECIAL_FUNC					//한광희팀장 만든 특별함수 사용														[사용안함] //변화없음
//-------------------------------------------------------------------------------------------------------------------------------------------------


//#define COSS_AJIN_MOTION_BOARD			//아진 모션보드 사용하면 살린다.
//#define COSS_CONTEC_USB_IO				//콘텍 USE IO 사용시

//검사항목추가-------------------------------------------------------------------------------------------------------------------------------------

//3D 정밀코팅관련>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//#define USE_3D_AREA					//3D Area
//#define USE_ROTATION					//정코 NEL1용 회전모듈


//#define WEB_TYPE_PVA					//WEB_10LEVEL 이고, PVA검사 일때 (여러 서버에게 결과 보내줌)
//#define FIND_TINYDEFECT				//작은 흑 이물 구할때 사용. Pyramid 에서 후보찾도록 수정 g_CandiTinyB
//-------------------------------------------------------------------------------------------------------------------------------------------------


//프로그램 큰 선택 (광저우는 WEB_10LEVEL 로 고정)
#define WEB_10LEVEL			0			//WEB 보통 
#define WEB_DAKANO			1			//DAKANO 라인얼룩, 물때얼룩 검사일때
#define GENERAL_TEACH		2			//범용검사기일때 사용
#define PROGRAM_TYPE		WEB_10LEVEL  //	


//프로그램 작은 선택
// (광정우) 투영은 USE_NORMAL
// (광저우) Edge경계는 TAC_LINE

#define  USE_DSFR			1			//청주 DSFR검사 (청주필름 검사, 덴트I(백점), 흑, 덴트II(백얼룩), 흑얼룩, 라인검출),  오창 TAC  NEL2, NEL6사용할때 
#define  TAC_LINE			2			//TAC 공정에서 사용함. COS7은 Edge경계로 사용함.  3동정코1호B, 1동정코1호  COS8:Edge경계  3동정코1호B에서 COS6: 경계A  COS2: 경계B
#define  USE_NORMAL			3			//보통 : 물때외
#define  INSPECT_TYPE		USE_NORMAL


#if INSPECT_TYPE==TAC_LINE
	#define  MURA_TAC_STYPE				//오창 TAC라인에 백얼룩/흑얼룩 찾을떄 사용(NEL2, NEL6)  NEL8은 사용안함
#endif



#ifdef USE_DAKANO_LINE
//	#define WEB_DAKANO_MULTAE				//물때얼룩
	#define LR_SERVER_EXIST					//전용 서버 있음

	#ifdef WEB_DAKANO_MULTAE
		#define MAX_WIDTH			8192	
		#define USE_X64	
	#else
		#define MAX_WIDTH			8192	
		#define USE_X64	
	#endif
#else
	#define MAX_WIDTH			8192	//최대 X폭 

#ifndef TEST_MODE
	#define USE_X64						//64비트 사용
#endif

#endif


#if PROGRAM_TYPE<GENERAL_TEACH
	#define MAX_IMAGE		1
#else
	#define MAX_IMAGE		4			//MAX_ALIGN_MODULE 이 4 이므로 4를 넘으면 안됨.
#endif


//Grabber설정-----------------------------------------------------------------------------------------------
#define NO_GRABBER			0			//시뮬레이션 Grab이 사용됨 (C:\\TEST 폴더에 이미지 넣어 두어야 함)
#define MIL_GRABBER			1			//Matrox 보드 사용
#define BASLER_GRABBER		2			//Basler Area 카메라 사용
#define EURESYS_GRABBER		3

#ifdef TEST_MODE
	#define GRABBER_TYPE		NO_GRABBER
#else
	#define GRABBER_TYPE		MIL_GRABBER
#endif
//----------------------------------------------------------------------------------------------------------


//CAMERA TYPE 설정------------------------------------------------------------------------------------------
//(광저우)투영은 USE_EXPOSURE_SERIAL 사용않고, CAMERA_BASLER 사용
//(광저우)Edge는 USE_EXPOSURE_SERIAL 사용하고, CAMERA_ELIIXA 사용


#if GRABBER_TYPE==MIL_GRABBER
	#define USE_EXPOSURE_SERIAL				//밝기 조절시 Serial통신 사용하는 경우 (CAMERA_ELIIXA, CAMERA_BASLER, CAMERA_PIRANHA)

	#define CAMERA_ELIIXA		1
	#define CAMERA_BASLER		2
	#define CAMERA_PIRANHA		3
	#define CAMERA_TYPE			CAMERA_ELIIXA
#endif

//#define USE_GAIN_SET			//CAMERA_ELIIXA 게인조정 사용할때만 살린다. ("2동 UV 접착전사라인" 만 사용함)
//-----------------------------------------------------------------------------------------------------------


//남경(광저우)관련 옵션들>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//남경 또는 광저우는 그대로 사용하면 됨.
//#define	CUNIC_NANJING		//남경은 쿠닉 구할때 X 위치의 밝기값을 고려해서 구함.
#define	LANGUAGE_ENGLISH	//영어사용 (Default임 코팅 2동3호기에는 사용하지 않음)
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



//각종 COMPILE 옵션들---------------------------------------------------------------------------------------
//#define  USE_HEX_FORMAT			//OPTIC종류를 16개 사용하는 경우 ( 2동코팅3호기 USE_MISE_IMUL  NELB만살린다)
//#define  USE_MULTIBYTE			//속성에서 MultiByte선택하면 이것도 살린다.(일반적으로 사용하면 안됨)
//#define  CHECK_INPOS				//구한 X위치가 OK Pos에 있는지 체크, (사용하면 l_SX, l_EX 설정 해야함)		[사용안함]



#ifndef LANGUAGE_ENGLISH				//일단 영문버전이면 아래 사용안함.
	#define USE_HOLE_SKIP				//TPW Hole이 있으면 없앰.													[2동3호기사용]
	#define SEND_14_DATA				//TPW같이 불량정보 보낼때 14개를 추가하는 경우 사용 (대부분 사용하지 않음)  [2동3호기사용]
#endif
//----------------------------------------------------------------------------------------------------------



#ifdef TEST_MODE
//	#define DONT_SAVE_IMAGE			//NG관련 이미지 저장하지 않도록 함(테스트할때 너무 많이 저장하면 부담)		[TEST외 사용안함]
//	#define DONT_SAVE_DATA			//NG관련 데이타 저장하지 않도록 함(테스트할때 너무 많이 저장하면 부담)		[TEST외 사용안함]
#endif



//각종 WEB 검사기 옵션들>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define SAVE_NG_THREAD				//NG영상(128x128)과 결과String을 Thread로 저장						[항상사용]
#define CHANGEABLE_MARKING			//가변마킹 사용														[항상사용]


#define	OVERKILL_CUNIC_LEVEL10		//쿠닉과검을 10Level로 보냄. 없으면 그냥 버림						[항상사용]	
#define CROSS_USE_BLACK				//Cross에서 흑점사용하면											[항상사용]
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



// SEND, RECV(16등분 DCF변경) 의 경우 기포눌림 사용할 때만 사용한다>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//#define MY_SEND							//내가 보내주는 거면
//#define SEND_OPTIC_NG_TYPE1		1		//해당불량이면 보냄
//#define SEND_OPTIC_NG_TYPE2		0		//해당불량이면 보냄

//#define MY_RECV							//내가 받는 거면
//#define RECV_CHECK_NG_TYPE1		3		//해당불량이면 체크함
//#define RECV_CHECK_NG_TYPE2		4		//해당불량이면 체크함
//#define RECV_CHANGE_NG_TYPE		5		//바꿀 불량TYPE

#if defined MY_SEND || defined MY_RECV
	#define SEND_OPTIC				COS2	//보내는 광학계(경계(미분같은)투과)
	#define RECV_OPTIC				COS8	//받는   광학계(Edge투과)
#endif
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<




#if GRABBER_TYPE==MIL_GRABBER

	#ifdef USE_X64
		#define	 USE_MIL10		//MIL Versino 10을 사용하면 살린다.	
	#endif

	#include "mil.h"	

#endif



#include "userdefine.h"
#include "CossTitle.h"
#include "UtilFunc.h"
#include "sio.h"				//serial 통신 사용
#include "UDP/XManageSocket.h"
#include "KoWebData.h"
#include "SpecialData.h"
#include "ParamData.h"			//서버로 부터 받은 Parameter Data
#include "KoInspectData.h"
#include "InspectFunc.h"
#include "Grabber.h"
#include "BaslerCam.h"
#include "EuresysGrabber.h"
#include "GlobalFunc.h"
#include "EdgeFunc.h"
#include "KROI.h"
#include "CossImage.h"
#include "VisionFunc.h"
#include "FlatFunc.h"
#include "THREAD.h"
#include "RealTimeMarking.h"
#include "NGSaveThread.h"

#include "OCRTeach.h"
#include "PatternMatch.h"
#include "AlignData.h"
#include "FChain.h"
#include "KoPyramid.h"
#include "SpotVision.h"
#include "WebInspect.h"
#include "InspectThread.h"
#include "PitFunc.h"
#include "PolaImage.h"

#include "ScratchFunc.h"	//스크라치검사 함수
#include "MuraFunc.h"		//얼룩검사 함수
#include "CrossAreaFunc.h"	//크로스검사 함수
#include "TooFunc.h"		//투영검사 함수
#include "LineFunc.h"		//라인검사 함수
#include "CunicFunc.h"		//쿠닉검사 함수(라인스캔)

#include "WebMarkingInspect.h"	//마킹검사
#include "WebKoonJip.h"         //군집검사 
#include "BondingSheet.h"       //본딩시트 주름과검 제거

#ifdef USE_HOLE_SKIP
	#include "HoleSkip.h" 
#endif

#ifdef BARCODE_VISION
	#include "WebBarcode.h" 
#endif

#ifdef USE_COS9_LR
	#include "cos9LR.h" 
#endif

#ifdef USE_NITTO_AI
#include "NittoAiClient.h"
#endif

//#CButtonST  1
#include "./BUTTON/ButtonST.h"
#include "./BUTTON/CdStatic.h"
#include "CustomButton.h"

#ifdef USE_3D_AREA
	#include "SerialMotion.h"
	#include "RotCrossAreaFunc.h"
#endif


//DAKANO 외
#include "MuraLine.h"		//DAKANO 라인검출

///////////////////////////////////////////////////////////////
//
//   A. I.
//  
//	AI적용해서 과검 없애려면 사용한다.
//#define AI_APPLY		
//#include "AbyssClient.h"
//
//
///////////////////////////////////////////////////////////////


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif






