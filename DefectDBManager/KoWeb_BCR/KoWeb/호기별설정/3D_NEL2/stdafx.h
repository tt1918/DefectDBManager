
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

#define  OLD_CPU

#define  INTERFACE_NEXTEYE_SERVER		//Nexteye 서버와 통신하려면 NEXTEYE Format에 맞게사용 (TCHAR 대신 char를 사용)  [넥스트아이서버 사용하면 살리고, COSS서버 사용하면 죽임]

//실제검사사용에서는 모두 죽이는 DEFINE---------------------------------------------------
//#define TEST_MODE						//테스트용으로 사용하려면 살림.						[사용안함]
//#define GRAB_TEST_MODE				//PC번호를 설정하고 Grab하면서 테스트 할때 사용		[사용안함]
//#define SHOW_DEBUG_DATA				//디버그용 데이타를 표시 (검사시간 표시)			[사용안함]
//#define NO_GRABER_SYNC_FRAME_FROM_SERVER	//테스트용으로 Server로부터 동기신호받아 검사	[사용안함]
//---------------------------------------------------------------------------------------



//속도관련  ---------------------------------------------------------------------------------------------------------------------------------------
#define SHOW_USE_THREAD					//Show Image를 THREAD를 사용할떄													[사용가능]
//#define MEMCPY_USE_THREAD				//전체 Frame을 복사할때 memcpy 대신 Thread_Memcpy를 사용한다.						[사용안함] //속도느려짐
//#define SPECIAL_FUNC					//한광희팀장 만든 특별함수 사용														[사용안함] //변화없음
#define USE_TOO_MURA					//투영 무라 검사할때 사용(투영 흑 얼룩검사)											[사용가능]
//-------------------------------------------------------------------------------------------------------------------------------------------------



//검사항목추가-------------------------------------------------------------------------------------------------------------------------------------

//3D 정밀코팅관련>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define USE_3D_AREA						//3D Area
//#define USE_ROTATION					//정코 NEL1용 회전모듈


//#define WEB_TYPE_PVA					//WEB_10LEVEL 이고, PVA검사 일때 (여러 서버에게 결과 보내줌)
//#define FIND_TINYDEFECT				//작은 흑 이물 구할때 사용. Pyramid 에서 후보찾도록 수정 g_CandiTinyB
//-------------------------------------------------------------------------------------------------------------------------------------------------


//프로그램 큰 선택
#define WEB_10LEVEL			0			//WEB 보통 
#define WEB_DAKANO			1			//DAKANO 라인얼룩 검사일때
#define GENERAL_TEACH		2			//범용검사기일때 사용
#define PROGRAM_TYPE		WEB_10LEVEL


//프로그램 작은 선택
#define  USE_DSFR			1			//청주 DSFR검사 (청주필름 검사, 덴트I(백점), 흑, 덴트II(백얼룩), 흑얼룩, 라인검출),  오창 TAC  NEL2, NEL6사용할때 
#define  TAC_LINE			2			//TAC 공정에서 사용함. COS7은 Edge경계로 사용함.
#define  USE_NORMAL			3			//보통 
#define  INSPECT_TYPE		USE_NORMAL


#if INSPECT_TYPE==USE_DSFR
//	#define  MURA_TAC_STYPE				//오창 TAC라인에 백얼룩/흑얼룩 찾을떄 사용(NEL2, NEL6)
#endif

#if PROGRAM_TYPE==WEB_DAKANO
	#define LR_SERVER_EXIST
	#define MAX_WIDTH			4096
#else
	#define MAX_WIDTH			8192	//최대 X폭 
//	#define USE_X64						//64비트 사용
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

#define GRABBER_TYPE		BASLER_GRABBER
//----------------------------------------------------------------------------------------------------------


//CAMERA TYPE 설정------------------------------------------------------------------------------------------
#if GRABBER_TYPE==MIL_GRABBER
	#define USE_EXPOSURE_SERIAL				//밝기 조절시 Serial통신 사용하는 경우 (CAMERA_ELIIXA, CAMERA_BASLER, CAMERA_PIRANHA)

	#define CAMERA_ELIIXA		1
	#define CAMERA_BASLER		2
	#define CAMERA_PIRANHA		3
	#define CAMERA_TYPE			CAMERA_ELIIXA
#endif

//#define USE_GAIN_SET						//CAMERA_ELIIXA 게인조정 사용할때만 살린다. (당분간 UV만 크로스에서만 사용함)
//-----------------------------------------------------------------------------------------------------------


//남경(광저우)관련 옵션들>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//#define	CUNIC_NANJING		//남경은 쿠닉 구할때 X 위치의 밝기값을 고려해서 구함.
#define	LANGUAGE_ENGLISH		//영어사용 (Default임)
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



//각종 COMPILE 옵션들---------------------------------------------------------------------------------------
//#define  USE_HEX_FORMAT			//OPTIC종류를 16개 사용하는 경우
//#define  USE_MULTIBYTE			//속성에서 MultiByte선택하면 이것도 살린다.(일반적으로 사용하면 안됨)
//#define  CHECK_INPOS				//구한 X위치가 OK Pos에 있는지 체크, (사용하면 l_SX, l_EX 설정 해야함)		[사용안함]



#ifndef LANGUAGE_ENGLISH				//일단 영문버전이면 아래 사용안함.
	#define HOLE_REMOVE					//TPW Hole이 있으면 없앰.													[2동3호기사용]
	#define SEND_14_DATA				//TPW같이 불량정보 보낼때 14개를 추가하는 경우 사용 (대부분 사용하지 않음)  [2동3호기사용]
#endif
//----------------------------------------------------------------------------------------------------------



#ifdef TEST_MODE
//	#define DONT_SAVE_IMAGE			//NG관련 이미지 저장하지 않도록 함(테스트할때 너무 많이 저장하면 부담)		[TEST외 사용안함]
//	#define DONT_SAVE_DATA			//NG관련 데이타 저장하지 않도록 함(테스트할때 너무 많이 저장하면 부담)		[TEST외 사용안함]
#endif



//각종 WEB 검사기 옵션들>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define SAVE_NG_THREAD				//NG영상(128x128)과 결과String을 Thread로 저장						[항상사용]
#define AUTO_MARKING_LEVEL			//마킹자동변경이면 사용(데이타 없으면 어차피 사용안함)				[항상사용]


#define	OVERKILL_CUNIC_LEVEL10		//쿠닉과검을 10Level로 보냄. 없으면 그냥 버림						[항상사용]	
#define CROSS_USE_BLACK				//Cross에서 흑점사용하면											[항상사용]
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


// NEL5, NEL7(16등분 DCF변경) 의 경우 기포눌림 사용할 때만 사용한다>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//#define KIPO_COS5_SEND				//기포눌림  
//#define SEND_OPTIC		    5		//1 또는 5   (1은 NEL1에서 휘점을 NEL4로 보냄) (5는 NEL5에서 눌림을 NEL7로 보냄)
//#define RECEIVE_OPTIC			7		//4 또는 7   (4는 NEL4에서 NEL1의 데이타 받음) (7은 NEL7에서 NEL5의 데이타 받음)
//#define LINE_NUM_BUFFERS		16	    //"KoWebData.cpp" 에서 g_System.m_nImagePart를 16으로 설정해야 함.
//#define LINE_BUFFER_HEIGHT		(LINE_HEIGHT/LINE_NUM_BUFFERS)	//NEL7의 경우 4096/16=256 또는 2048/16=128  nexteye.dcf 반드시교체해야함
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






