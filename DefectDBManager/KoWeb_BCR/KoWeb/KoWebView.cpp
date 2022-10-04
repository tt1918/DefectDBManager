
// KoWebView.cpp : implementation of the CKoWebView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "KoWeb.h"
#endif

#include "KoWebDoc.h"
#include "KoWebView.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMER_ONE_SECOND	1
#define TIMER_TEN_SECOND    2
#define TIMER_ONE_MINUTE	3
#define TIMER_TEST			4

CWinThread	*g_GrabThread, *g_GrabThreadSUB;
int l_PressDebug=0;
// CKoWebView
extern CXManageSocket  l_Send_Server;

#ifdef SERVER_TEST
extern CXManageSocket  l_Send_ServerTest;
extern CXManageSocket  l_Send_MarkServerTest[2];
#endif

IMPLEMENT_DYNCREATE(CKoWebView, CFormView)

BEGIN_MESSAGE_MAP(CKoWebView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_LOADIMAGE, &CKoWebView::OnBnClickedButtonLoadimage)
	ON_BN_CLICKED(IDC_BUTTON_SAVEIMAGE, &CKoWebView::OnBnClickedButtonSaveimage)
	ON_BN_CLICKED(IDC_BUTTON_INSPECT, &CKoWebView::OnBnClickedButtonInspect)
	ON_BN_CLICKED(IDC_BUTTON_CONFIG, &CKoWebView::OnBnClickedButtonConfig)
	ON_BN_CLICKED(IDC_BUTTON_PARAM, &CKoWebView::OnBnClickedButtonParam)
	ON_BN_CLICKED(IDC_BUTTON_TEACH, &CKoWebView::OnBnClickedButtonTeach)
	ON_BN_CLICKED(IDC_CHECK_AUTO_START, &CKoWebView::OnBnClickedCheckAutoStart)
	ON_BN_CLICKED(IDC_CHECK_SHOW_INFO, &CKoWebView::OnBnClickedCheckShowInfo)
	ON_BN_CLICKED(IDC_CHECK_SHOWIMGE, &CKoWebView::OnBnClickedCheckShowimge)
	ON_CBN_SELCHANGE(IDC_COMBO_ZOOM, &CKoWebView::OnCbnSelchangeComboZoom)
	ON_BN_CLICKED(IDC_BUTTON_BINARY, &CKoWebView::OnBnClickedButtonBinary)
	ON_BN_CLICKED(IDC_BUTTON_PATTERN, &CKoWebView::OnBnClickedButtonPattern)
	ON_BN_CLICKED(IDC_BUTTON_FUNC_TEST, &CKoWebView::OnBnClickedButtonFuncTest)
	ON_BN_CLICKED(IDC_BUTTON_ALIGN, &CKoWebView::OnBnClickedButtonAlign)
	ON_BN_CLICKED(IDC_BUTTON_GRAB, &CKoWebView::OnBnClickedButtonGrab)
	ON_BN_CLICKED(IDC_CHECK_LIVE, &CKoWebView::OnBnClickedCheckLive)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CKoWebView::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_PREV, &CKoWebView::OnBnClickedButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_OCR, &CKoWebView::OnBnClickedButtonOcr)
	ON_BN_CLICKED(IDC_BUTTON_DEBUG, &CKoWebView::OnBnClickedButtonDebug)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CKoWebView::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_CHECK_SHOWMAP, &CKoWebView::OnBnClickedCheckShowmap)
	ON_BN_CLICKED(IDC_RADIO_CAM1, &CKoWebView::OnBnClickedRadioCam1)
	ON_BN_CLICKED(IDC_RADIO_CAM2, &CKoWebView::OnBnClickedRadioCam2)
	ON_BN_CLICKED(IDC_RADIO_CAM3, &CKoWebView::OnBnClickedRadioCam3)
	ON_BN_CLICKED(IDC_RADIO_CAM4, &CKoWebView::OnBnClickedRadioCam4)
	ON_BN_CLICKED(IDC_BUTTON_MODEL, &CKoWebView::OnBnClickedButtonModel)
	ON_BN_CLICKED(IDC_CHECK_SHOWMAX, &CKoWebView::OnBnClickedCheckShowmax)
	ON_BN_CLICKED(IDC_CHECK_SHOWOLDMAP, &CKoWebView::OnBnClickedCheckShowoldmap)	
	ON_BN_CLICKED(IDC_BUTTON_INSPECT3, &CKoWebView::OnBnClickedButtonInspect3)
	ON_BN_CLICKED(IDC_CHECK_ALL, &CKoWebView::OnBnClickedCheckAll)
	ON_BN_CLICKED(IDC_BTN_SHOW_DEFECT_NOW, &CKoWebView::OnBnClickedBtnShowDefectNow)
	ON_BN_CLICKED(IDC_BTN_SHOW_DEFECT_NEXT, &CKoWebView::OnBnClickedBtnShowDefectNext)
	ON_MESSAGE(WM_BCR_COMM, &CKoWebView::OnBCrComm)

END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CKoWebView, CFormView)
END_EVENTSINK_MAP()

// CKoWebView construction/destruction


CKoWebView::CKoWebView()
	: CFormView(CKoWebView::IDD)
	, m_AllLength(FALSE)
{
	m_pConfig=NULL;
	m_pParam =NULL;
	m_pDebug =NULL;
	m_pShowInfo=NULL;

	//범용검사기----------
	m_pBinary		=NULL;
	m_pPattern		=NULL;
	m_pOCR			=NULL;
	m_pAlign		=NULL;

	m_pTeach		=NULL;
	m_pBlobSet		=NULL;
	m_pPatternSet	=NULL;
	m_pFindLineSet	=NULL;
	m_pMeasureSet	=NULL;
	m_pSpotSet		=NULL;   //@항목추가5
	m_pCompareSet	=NULL;
	m_pOCRSet		=NULL;
	m_pUserSet		=NULL;
	m_pModel		=NULL;
	//--------------------

	m_nShowNGID=0;
	m_bShowImage=0;		//이미지 Update NO
	m_bShowFlat	=0;		//이미지 평활화 보여주기 NO
	m_bShowCandiArea=0; //후보 영역 보여주기 NO
	m_bShowAllCandi=0;  //모든 후보 보여주기 NO
	m_bShowValidCandi=0;//유력 후보 보여주기 NO

	g_Temp.m_bLive=0;
	g_Temp.m_bAuto=0;
	m_AllLength   =1;
	m_dYLength    = 0.0;
	m_nCheckNewLot=0;

	m_DefectCallClass = nullptr;
	m_DefectReadingEvent = nullptr;
}

CKoWebView::~CKoWebView()
{
	int i;

#if MAX_IMAGE==1
	if(m_pImage) delete m_pImage;				//메인이미지
#else
	for(i=0;i<4;i++)
		if(m_pImageArray[i]) delete m_pImageArray[i];
#endif

	for(i=0;i<SHOW_NG_IMAGE;i++)
	{
		if(m_pNGImage[i]) delete m_pNGImage[i];	//불량영상(작은이미지) 표시
	}

	if(m_pConfig)	delete m_pConfig;			//Config
	if(m_pParam)    delete m_pParam;			//파라미터 설정/
	if(m_pDebug)    delete m_pDebug;
	if(m_pShowInfo) delete m_pShowInfo;

	//범용검사기-------------------------------------------
	if(m_pBinary)		delete m_pBinary;		//Tool
	if(m_pPattern)		delete m_pPattern;		//패턴티칭
	if(m_pOCR)			delete m_pOCR;			//OCR
	if(m_pAlign)		delete m_pAlign;		//Align

	if(m_pTeach)		delete m_pTeach;		//Teach(아래는 Teach 검사항목들)
	 if(m_pBlobSet)		delete m_pBlobSet;
	 if(m_pPatternSet)	delete m_pPatternSet;
	 if(m_pFindLineSet)	delete m_pFindLineSet;
	 if(m_pMeasureSet)	delete m_pMeasureSet;
	 if(m_pSpotSet)		delete m_pSpotSet;		//@항목추가5-1
	 if(m_pCompareSet)	delete m_pCompareSet;
	 if(m_pOCRSet)		delete m_pOCRSet;
	 if(m_pUserSet)		delete m_pUserSet;
	 if(m_pModel)       delete m_pModel;
	//------------------------------------------------------	 

	 WriteLog(_T("Program Closed By User"));
}

void CKoWebView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDC_LIST_RESULT, m_ctrlResult);
	DDX_Check(pDX, IDC_CHECK_SHOW_INFO, m_bShowInfo);
	DDX_Check(pDX, IDC_CHECK_SHOWIMGE, m_bShowImage);
	DDX_Control(pDX, IDC_COMBO_ZOOM, m_ctrlZoom);
	//	DDX_Control(pDX, IDC_LIST_DEFECTINFO, m_ctrlDefectInfo);
	DDX_Control(pDX, IDC_BUTTON_CONFIG,		m_btConfig);	
	DDX_Control(pDX, IDC_BUTTON_LOADIMAGE,	m_btLoadIMG);	
	DDX_Control(pDX, IDC_BUTTON_SAVEIMAGE,	m_btSaveIMG);	
	DDX_Control(pDX, IDC_BUTTON_GRAB,		m_btGrab);	
	DDX_Control(pDX, IDC_BUTTON_PARAM,		m_btParam);		//#CButtonST  3
	DDX_Control(pDX, IDC_BUTTON_BINARY,		m_btTool);		//#OwnerDraw 1
	DDX_Check(pDX, IDC_CHECK_SHOWMAP, m_bShowMap);
	DDX_Check(pDX, IDC_CHECK_SHOWMAX, m_bShowMax);
	DDX_Check(pDX, IDC_CHECK_SHOWOLDMAP, m_bShowOldMap);
	DDX_Check(pDX, IDC_CHECK_ALL, m_AllLength);
	DDX_Text(pDX, IDC_EDIT_YLENGTH, m_dYLength);
}

BOOL CKoWebView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CKoWebView::OnInitialUpdate()
{
	int i;

	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	ResetCount();

	setlocale(LC_ALL, "Korean");
#if PROGRAM_TYPE!=GENERAL_TEACH
	GetDlgItem(IDC_STATIC_SETTING)		->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_PATTERN)		->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_ALIGN)		->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_OCR)			->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_MODEL)		->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_TEACH)		->ShowWindow(SW_HIDE);
#endif

#if MAX_IMAGE==1
	GetDlgItem(IDC_RADIO_CAM1)			->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_CAM2)			->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_CAM3)			->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_CAM4)			->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK_SHOWMAX)		->ShowWindow(SW_HIDE);
#endif

	//OnCtlColor ----------------------------------------------------------------------
	::SetWindowTheme(GetDlgItem(IDC_CHECK_SHOWIMGE)			->GetSafeHwnd(), L"", L""); 
	::SetWindowTheme(GetDlgItem(IDC_CHECK_LIVE)				->GetSafeHwnd(), L"", L""); 
	::SetWindowTheme(GetDlgItem(IDC_CHECK_SHOW_INFO)		->GetSafeHwnd(), L"", L""); 
	::SetWindowTheme(GetDlgItem(IDC_CHECK_SHOWMAP)			->GetSafeHwnd(), L"", L""); 
	::SetWindowTheme(GetDlgItem(IDC_CHECK_SHOWOLDMAP)		->GetSafeHwnd(), L"", L""); 
	::SetWindowTheme(GetDlgItem(IDC_STATIC_WEB)				->GetSafeHwnd(), L"", L"");  
	::SetWindowTheme(GetDlgItem(IDC_STATIC_SETTING)			->GetSafeHwnd(), L"", L""); 
	::SetWindowTheme(GetDlgItem(IDC_STATIC_UDP)				->GetSafeHwnd(), L"", L""); 
	::SetWindowTheme(GetDlgItem(IDC_STATIC_RECV)			->GetSafeHwnd(), L"", L""); 
	::SetWindowTheme(GetDlgItem(IDC_STATIC_ZOOM)			->GetSafeHwnd(), L"", L""); 
	::SetWindowTheme(GetDlgItem(IDC_CHECK_ALL)				->GetSafeHwnd(), L"", L""); 
	//---------------------------------------------------------------------------------


	//--------------프로그램 시작 -----------------------------------------------
	CossMakeDirectory(); 
	WriteLog(_T("Program Start"));	//한달에 1개 생기는 로그, 굵직한 것만 표시
//	WriteDebugLog(1, _T("Start"));  //프로그램시작과 함께 생김. 이후부터 WriteLogStr(0, _T("1")); 이런식으로 Call 하면 됨.(디버깅용)

	MakeMainDisplay();				//메인 이미지 생성
	MakeNGDisplay();				//불량 이미지 생성
	MakeGUI();						//우측에 버전, PC 이름 최초 읽어서 표시
	LoadNGID(g_Temp.m_nPCOptic);	//광학계별 NG번호 지정
	InitData();						//데이타 Reset한번 함.
	if(g_Temp.m_nPCOptic==COS7
#ifdef WEB_DAKANO_MULTAE		
		|| g_Temp.m_nPCOptic==COS2
#endif
		) WEB_MuraMalloc(); //MURA Free


	////필요한 폴더 생성 (LOT LOTDATA IMAGE, 각 Lot이름 폴더 만듦---------	
	g_Temp.m_slotName=LoadCurrentLotName();
	CossMakeDirectoryLot(g_Temp.m_slotName);

#if PROGRAM_TYPE==GENERAL_TEACH
	LoadInspSystemData();
	LoadModel(g_Temp.m_slotName);
#else

	if(g_System.m_nPerspectiveTR)	//투영, LR의 경우 LUT 데이타 만듦.
		LoadPerspective();
#endif

	m_ctrlZoom.SetCurSel(0);


#ifdef USE_ROTATION	
	g_Temp.m_nMotionExist=(int)g_SioMotion.InitComm(g_Config.m_nComPort, 9600, NOPARITY,8,ONESTOPBIT,FALSE);
	char szCmd[20];
	sprintf(szCmd, "1VE10\r");   g_SioMotion.Write((LPBYTE)szCmd, strlen(szCmd)); //속도 10	
	sprintf(szCmd, "1AC50\r");   g_SioMotion.Write((LPBYTE)szCmd, strlen(szCmd)); //가속 50	
	sprintf(szCmd, "1DC50\r");   g_SioMotion.Write((LPBYTE)szCmd, strlen(szCmd)); //감속 50		
	//g_SioMotion.Write((BYTE *)"1VE10\r", 7); //속도 10
	//g_SioMotion.Write((BYTE *)"1AC50\r", 7); //가속 50
	//g_SioMotion.Write((BYTE *)"1DC50\r", 7); //감속 50	
#endif


	//각종 THREAD 생성 -----------------------------------------------------------------------------------------------------------------
	g_Temp.m_nPGStart=1;			//THREAD_Auto,  THREAD_Inspect, THREAD_Save, ThreadSaveNGImage, THREAD_MultiProcess 의 중심
	AfxBeginThread(THREAD_Auto,		  this,THREAD_PRIORITY_ABOVE_NORMAL,0);
	AfxBeginThread(THREAD_Inspect,	  this,THREAD_PRIORITY_ABOVE_NORMAL,0);
//	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	AfxBeginThread(THREAD_Save,		  this,THREAD_PRIORITY_NORMAL,0);				//전체영상 저장을 위한 Thread

#ifdef WEB_DAKANO_MULTAE
	AfxBeginThread(THREAD_SaveCompImage,  this,THREAD_PRIORITY_NORMAL,0);			//물때 부분이미지 저장위한 Thread
#endif

#ifdef SHOW_USE_THREAD
	AfxBeginThread(THREAD_ShowImage,  this,THREAD_PRIORITY_NORMAL,0);				//화면보이기 위한 Thread
#endif

	AfxBeginThread(ThreadSaveNGImage, this,THREAD_PRIORITY_NORMAL,0);				//불량영상 저장을 위한 Thread
	AfxBeginThread(ThreadSaveNGString,this,THREAD_PRIORITY_NORMAL,0);				//불량Text 저장을 위한 Thread

	for(i=0;i<4;i++)
		AfxBeginThread(THREAD_MultiProcess, (LPVOID)i, THREAD_PRIORITY_NORMAL, 0);  //함수 Multi Thread로 수행하기 위한 4개의 Thread 시작
	//-----------------------------------------------------------------------------------------------------------------------------------

#ifdef USE_ROTATION
	if(g_System.m_nLineCAM==0)
		AfxBeginThread(MotorControlThread, this, THREAD_PRIORITY_NORMAL, 0);
#endif

	//OCR Init
	InitOCRData();

#if GRABBER_TYPE==MIL_GRABBER
	//Grabber Open
	g_Grabber.Open(true, this->m_hWnd);	//비전보드 OPEN


#elif GRABBER_TYPE==BASLER_GRABBER
	g_Grabber.Open(true, this->m_hWnd);	//비전보드 OPEN
	g_GrabThread=AfxBeginThread   (BaslerGrabThread,    this,THREAD_PRIORITY_NORMAL,0);
	g_GrabThreadSUB=AfxBeginThread(BaslerGrabThreadSub, this,THREAD_PRIORITY_NORMAL,0);

	SetThreadPriority(g_GrabThread,    THREAD_PRIORITY_TIME_CRITICAL);
	SetThreadPriority(g_GrabThreadSUB, THREAD_PRIORITY_TIME_CRITICAL);
#else
	g_Grabber.Open(true, this->m_hWnd);	//비전보드 OPEN
#endif

	//LoadParameter -------------------------------------------------------------
	if(ReadParameter(0)==0)		//파라미터 읽기
	{
		CString str=g_Temp.m_sMyComName;
		if(g_Temp.m_sMyComName.Find(_T("NEL"))>=0)
		{
			g_Temp.m_sMyComName.Format(_T("COS-%d%02d"), g_Temp.m_nPCFirstNo, g_Temp.m_nPCNum);
		}
		else if(g_Temp.m_sMyComName.Find(_T("COS"))>=0)
		{
			g_Temp.m_sMyComName.Format(_T("NEL-%d%02d"), g_Temp.m_nPCFirstNo, g_Temp.m_nPCNum);
		}
		
		ReadParameter(0);
		
		g_Temp.m_sMyComName=str;
	}

	LoadParamName();		//파라미터 이름 읽기

	ShowParameter();		//노출값 화면표시
	
	ConnectUDP();			//각종 UDP통신 OPEN
	
	//Connect AI 서버
	AI_ConnectServer();		//AI_APPLY 이 Define이 되어 있을때 사용함.
	NittoAI_ConnectServer();//USE_NITTO_AI가 Define이 되어 있을때 사용함.

	//검사를 위한 메모리 Alloc----------------------------------------------------------
	OpenPyramid(g_System.m_nImageW, g_System.m_nImageH); //영상압축시 사용할 메모리 확보



#if PROGRAM_TYPE==GENERAL_TEACH
	SetTimer(TIMER_TEST, 200, NULL);	//0.2초에 한번 실행
#endif
	SetTimer(TIMER_ONE_SECOND, 1000,  NULL);	//1초에 한번 실행
	SetTimer(TIMER_TEN_SECOND, 10000, NULL);//10초에 한번 실행
	SetTimer(TIMER_ONE_MINUTE, 60000, NULL);	//1분에 한번 실행
	SetSpreadData(3,g_Temp.m_slotName);

#if PROGRAM_TYPE==WEB_DAKANO
	GetDlgItem(IDC_CHECK_SHOWMAP) ->ShowWindow(SW_SHOW);	
	GetDlgItem(IDC_BUTTON_INSPECT3) ->ShowWindow(SW_SHOW);
//	GetDlgItem(IDC_CHECK_SHOWOLDMAP)->ShowWindow(SW_SHOW);
#elif PROGRAM_TYPE==GENERAL_TEACH
	SelectImage(-1);			
	
#endif

#ifdef USE_DAKANO_LINE
	GetDlgItem(IDC_CHECK_SHOWMAP) ->ShowWindow(SW_SHOW);	
	GetDlgItem(IDC_BUTTON_INSPECT3) ->ShowWindow(SW_SHOW);
#endif

#if PROGRAM_TYPE==WEB_DAKANO
	FuncBaseLineLoad(_T("C:\\COSS\\BaseLine.dat"));
#endif

	CreateDefectCallCallss();
}




void CKoWebView::MakeGUI()
{
	WINDOWPLACEMENT wp;
	GetDlgItem(IDC_IMAGE)->GetWindowPlacement(&wp);

	m_pShowInfo=new CShowInfo1(this);
	m_pShowInfo->Create(IDD_DIALOG_SHOWINFO1, this);

//#if PROGRAM_TYPE<2
	m_pShowInfo->MoveWindow(wp.rcNormalPosition.right+5,0, m_pShowInfo->m_nXEnd,510);
//#else
//	m_pShowInfo->MoveWindow(wp.rcNormalPosition.right+5,0, m_pShowInfo->m_nXEnd,360);
//#endif

	m_pShowInfo->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_RESULT_FRAME)->GetWindowPlacement(&wp);
	m_rcResult=wp.rcNormalPosition;

	GetDlgItem(IDC_DEFECT_FRAME)->GetWindowPlacement(&wp);
	m_rcDefect=wp.rcNormalPosition;
	

	CString str, sVersion=_T("1.0.0.10");
	
	sVersion=GetSWVersion();
	SetSpreadData(1, sVersion);
	SetSpreadData(2, GetPCName());

	//#CButtonST  4---------------------------------------
	m_BTFont.CreatePointFont(100, _T("Segoe UI Symbol"));
	initButtonST(0, &m_btConfig);
	initButtonST(0, &m_btLoadIMG);
	initButtonST(0, &m_btSaveIMG);
	initButtonST(0, &m_btGrab);
	initButtonST(0, &m_btParam);
	//----------------------------------------------------
}

void CKoWebView::MakeMainDisplay()
{
	int i;
	CString str;
	WINDOWPLACEMENT wp;
	CRect rc;
	GetDlgItem(IDC_IMAGE)->GetWindowPlacement(&wp);

#if MAX_IMAGE==1
	m_nDisplayX=wp.rcNormalPosition.right-wp.rcNormalPosition.left;
	m_nDisplayY=wp.rcNormalPosition.bottom-wp.rcNormalPosition.top;

	g_ROI[0].Initialize(g_System.m_nImageW, g_System.m_nImageH);

	m_pImage=new CCossImage(this);
	m_pImage->IMAGE_COLOR=1;
	m_pImage->SetImageData(g_System.m_nImageW, g_System.m_nImageH);
	m_pImage->Create(IDD_DIALOG_COSSIMAGE,this);
	m_pImage->m_UseMask=0;
	m_pImage->m_pROI=&g_ROI[0];
	m_pImage->m_fmProcess=NULL;
	m_pImage->m_nDisplayID=0;
	m_pImage->MoveWindow(&wp.rcNormalPosition);
	m_pImage->SetTitle(_T(""));
	m_pImage->FitZoomXY();
	m_pImage->FitZoomXY();
	m_pImage->ShowWindow(SW_SHOWNORMAL);
#else
	m_nDisplayX=(wp.rcNormalPosition.right-wp.rcNormalPosition.left)/2;
	m_nDisplayY=(wp.rcNormalPosition.bottom-wp.rcNormalPosition.top)/2;

	m_rcImageMax=wp.rcNormalPosition;
	m_rcImage[0].left=wp.rcNormalPosition.left;					m_rcImage[0].top=wp.rcNormalPosition.top;				m_rcImage[0].right=wp.rcNormalPosition.left+m_nDisplayX; m_rcImage[0].bottom=wp.rcNormalPosition.top+m_nDisplayY;
	m_rcImage[1].left=wp.rcNormalPosition.left+m_nDisplayX+1;	m_rcImage[1].top=wp.rcNormalPosition.top;				m_rcImage[1].right=wp.rcNormalPosition.right;			 m_rcImage[1].bottom=wp.rcNormalPosition.top+m_nDisplayY;
	m_rcImage[2].left=wp.rcNormalPosition.left;					m_rcImage[2].top=wp.rcNormalPosition.top+m_nDisplayY+1;	m_rcImage[2].right=wp.rcNormalPosition.left+m_nDisplayX; m_rcImage[2].bottom=wp.rcNormalPosition.bottom;
	m_rcImage[3].left=wp.rcNormalPosition.left+m_nDisplayX+1;	m_rcImage[3].top=wp.rcNormalPosition.top+m_nDisplayY+1;	m_rcImage[3].right=wp.rcNormalPosition.right;			 m_rcImage[3].bottom=wp.rcNormalPosition.bottom;

	for(i=0;i<4;i++)
	{
		g_ROI[i].Initialize(g_System.m_nImageW, g_System.m_nImageH);

		m_pImageArray[i]=new CCossImage(this);
		m_pImageArray[i]->IMAGE_COLOR=1;
		m_pImageArray[i]->SetImageData(g_System.m_nImageW, g_System.m_nImageH);
		m_pImageArray[i]->Create(IDD_DIALOG_COSSIMAGE,this);
		m_pImageArray[i]->m_UseMask=0;
		m_pImageArray[i]->m_pROI=&g_ROI[i];
		m_pImageArray[i]->m_fmProcess=NULL;
		m_pImageArray[i]->m_nDisplayID=0;
		m_pImageArray[i]->MoveWindow(m_rcImage[i]);
		str.Format(_T("Cam %d"),i+1);
		
		m_pImageArray[i]->SetImageName(str);
		m_pImageArray[i]->FitZoomXY();
		m_pImageArray[i]->FitZoomXY();
		m_pImageArray[i]->ShowWindow(SW_SHOWNORMAL);

		m_pImage=m_pImageArray[0];
	}
#endif
}


void CKoWebView::MakeNGDisplay()
{
	int i;
	int nStartX=540;
	int nStartY=730;
	int nW=128;
	int nH=128;
	WINDOWPLACEMENT wp;

	GetDlgItem(IDC_STATIC_NG_DISPLAY)->GetWindowPlacement(&wp);
	nStartX=wp.rcNormalPosition.left;
	nStartY=wp.rcNormalPosition.top;

	for(i=0;i<SHOW_NG_IMAGE;i++)
	{
		m_pNGImage[i]=new CCossImage(this);
		m_pNGImage[i]->IMAGE_COLOR=1;
		m_pNGImage[i]->SetImageData(nW, nH);
		m_pNGImage[i]->Create(IDD_DIALOG_COSSIMAGE,this);
		m_pNGImage[i]->m_UseMask=0;
		m_pNGImage[i]->m_pROI=&g_ROI[i];
		m_pNGImage[i]->m_fmProcess=NULL;
		m_pNGImage[i]->m_nDisplayID=i+1;
		m_pNGImage[i]->MoveWindow(CRect(nStartX,nStartY, nStartX+nW, nStartY+nH));
		m_pNGImage[i]->SetTitle(_T(""));
		m_pNGImage[i]->ShowGrayInfo(false);
		m_pNGImage[i]->FitZoom();
		m_pNGImage[i]->ShowWindow(SW_SHOWNORMAL);
	}
}


void CKoWebView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CKoWebView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CKoWebView diagnostics

#ifdef _DEBUG
void CKoWebView::AssertValid() const
{
	CFormView::AssertValid();
}

void CKoWebView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CKoWebDoc* CKoWebView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKoWebDoc)));
	return (CKoWebDoc*)m_pDocument;
}
#endif //_DEBUG

void CKoWebView::OnBnClickedCheckLive()
{
	if(g_Temp.m_bAuto) return;

	g_Temp.m_bLive=((CButton *)GetDlgItem(IDC_CHECK_LIVE))->GetCheck();  
	
	if(g_Temp.m_bLive)
	{
		m_bShowImage=1;
		((CButton *)GetDlgItem(IDC_CHECK_SHOWIMGE))->SetCheck(1); 
		g_Grabber.HookStart();
	}
	else
	{
		g_Grabber.HookStop();
	}
}

// CKoWebView message handlers
void CKoWebView::OnBnClickedButtonGrab()
{
	int i, nID;
	if(g_Temp.m_bAuto) return;

	if(g_Temp.m_bLive) //현재 Live면 Live정지
	{
		g_Temp.m_bLive=false;
		g_Grabber.HookStop();
		((CButton *)GetDlgItem(IDC_CHECK_LIVE))->SetCheck(false);  
		
		memcpy(g_fmGrab[g_ID], m_pImage->m_fmDisplay, g_System.m_nImageW*g_System.m_nImageH);
	}
	else  //한번 촬상
	{
		g_Temp.m_nGrabDone=0;
		g_Temp.m_nGrabOne=1;
		g_Grabber.HookStart();
		
		//5초간 촬상대기----------------------------------------------------------------------------
		for(i=0;i<500;i++)		
		{
			if(g_Temp.m_nGrabDone)
			{
				ShowGrabImage();
				break;
			}
			Sleep(10);
		}
		//--------------------------------------------------------------------------------------------

		if(g_Temp.m_nGrabOne)
			g_Grabber.HookStop();
	}
}


void CKoWebView::OnBnClickedButtonLoadimage()
{
	if(g_Temp.m_bShowMap && !g_Temp.m_bShowOldMap) return;

	CFileDialog dlg(TRUE, _T("Image"), _T("Image"));

#if (_WIN32_WINNT >= 0x0500) 
	dlg.m_ofn.pvReserved = NULL; 
	dlg.m_ofn.dwReserved = 0; 
	dlg.m_ofn.FlagsEx = 0; 
#endif

	if(dlg.DoModal() !=IDOK) return;



	//CImage 이용해 파일 읽기------------------------------------------------------------------
	m_pImage->OpenIMG(dlg.GetPathName());
	m_pImage->ResetGraphic();
	//------------------------------------------------------------------------------------------

	CString str;
	double end, start=Clock_set();
	
	//------ 방금 Load 한 영상 g_fmGrab 메모리에 복사 ------------------------------------
#if PROGRAM_TYPE==WEB_DAKANO
	g_ID=0;
	if(g_Temp.m_bShowOldMap)
		memcpy(g_fmOldMAP,  m_pImage->m_fmDisplay, 4096*2048);
	else
#endif

	#ifdef	USE_NEXT_GRAB		

		memcpy(g_fmSubPrev, g_fmGrab[g_ID]+g_System.m_nImageW*(g_System.m_nImageH-SUB_FRAME_HEIGHT), g_System.m_nImageW*SUB_FRAME_HEIGHT);  //직전 검사에 사용한 이미지의 마지막 128 영역

		memcpy(g_fmGrab[g_ID],  g_fmNext, g_System.m_nImageW*g_System.m_nImageH);			//현재 검사에 사용할 이미지
		memcpy(g_fmNext,  m_pImage->m_fmDisplay, g_System.m_nImageW*g_System.m_nImageH);	//방금 촬상한 이미지를 다음 검사를 위해서 보관

		memcpy(g_fmSubNext, g_fmNext,  g_System.m_nImageW*SUB_FRAME_HEIGHT);		//다음 Frame의 처음 128영역
		
		if(g_Temp.m_GrabNextDelay==0)		g_Temp.m_GrabNextDelay=1;				//  g_fmSubNext만 사용가능
		else if(g_Temp.m_GrabNextDelay==1)  g_Temp.m_GrabNextDelay=2;				//  g_fmSubNext, g_fmSubPrev 모두 사용가능 

	#else
		memcpy(g_fmGrab[g_ID],  m_pImage->m_fmDisplay, g_System.m_nImageW*g_System.m_nImageH);
	#endif
	
	
	if(g_Temp.m_nBinaryDialogOpen)
		memcpy(g_fmTemp,  m_pImage->m_fmDisplay, g_System.m_nImageW*g_System.m_nImageH);  //Tool Dialog가 열려있으면 g_fmTemp에도 복사해야 함.
	//------------------------------------------------------------------------------------

	end=Clock_set();
	m_nResultStringCnt=0;				 //m_ctrlResult.ResetContent();
	str.Format(_T("%.3lf"), end-start);
	m_sResult[m_nResultStringCnt++]=str; //m_ctrlResult.AddString(str);
	DrawResultViewText();

	UpdateImageInfo();
}

//자동으로 저장됩니다. 저장되는 폴더 IMAGE_SAVE_PATH 
void CKoWebView::OnBnClickedButtonSaveimage()
{
	if(!g_Temp.m_bAuto)
	{
		CFileDialog dlg(FALSE, _T("bmp"), _T("Image"));
		if(dlg.DoModal() !=IDOK) return;
		m_pImage->SaveIMG(dlg.GetPathName());
	}
	else
	{		
		CossMakeImageDirectoryLot(g_Temp.m_slotName);
		g_Temp.m_nSaveOneFrame=1;		
	}
}

//한번검사(One Inspect)
void CKoWebView::OnBnClickedButtonInspect()
{
	if(g_Temp.m_bAuto || g_Temp.m_bLive) return;

//WEB은 실제 검사와 똑같이 하려고 아래와 같이 함.
#if PROGRAM_TYPE<GENERAL_TEACH
	#ifdef TEST_MODE
		g_Temp.m_bAuto = TRUE;	
		Sleep(100);

		memcpy(g_fmGrab[g_ID],  m_pImage->m_fmDisplay, g_System.m_nImageW*g_System.m_nImageH);
		g_Temp.m_nGrabFrame++;
		g_Temp.m_nGrabFrameSub+=16;
		g_Temp.m_nGrabDone=1;

		Sleep(100);
		g_Temp.m_bAuto = FALSE;
		return;
	#endif
#endif

//	if(g_Temp.m_bAuto) return;

	l_dTime[0]=Clock_set();

	ViewInspect();		//One Inspect
	MakeResultData();

	l_dTime[8]=Clock_set();
	g_Temp.m_dInspTime=l_dTime[8]-l_dTime[0];

	UpdateInfo();

#if PROGRAM_TYPE<GENERAL_TEACH
	ShowResults();	
#endif

}


void CKoWebView::OnBnClickedButtonConfig()
{
	if(m_pConfig!=NULL)
	{
		if(m_pConfig->IsWindowVisible()) return;
		else
		{
			delete m_pConfig;
			m_pConfig=NULL;
		}
	}
	m_pConfig=new CDlgConfig(this);
	m_pConfig->Create(IDD_DIALOG_CONFIG, this);
}

void CKoWebView::OnBnClickedButtonParam()
{
	if(m_pParam!=NULL)
	{
		if(m_pParam->IsWindowVisible()) return;
		else
		{
			delete m_pParam;
			m_pParam=NULL;
		}
	}
	m_pParam=new CDlgParam(this);
	m_pParam->Create(IDD_DIALOG_PARAM, this);
}


//매 Frame Update (Inspect_Frame, NG Frame Count, NGCount)
void CKoWebView::SetSpreadCount()
{
	CString str;
	static int nNGCount=-1;

#if PROGRAM_TYPE==GENERAL_TEACH
	str.Format(_T("%d"), g_Temp.m_nInspectFrame);
#else
	str.Format(_T("%d/%d"), g_Temp.m_nInspectFrame, g_Temp.m_nGrabFrameID);
#endif
	SetSpreadData(4, str);

	if(nNGCount!=g_Temp.m_nNGCount)
	{
		str.Format(_T("%d"), g_Temp.m_nNGFrame);
		SetSpreadData(5, str);

		str.Format(_T("%d"), g_Temp.m_nNGCount);
		SetSpreadData(6, str);
	}
}

//FPS 보여줌
void CKoWebView::SetSpreadFPS()
{
	CString str;
	static double dFPS=0;
	static double dHDDspace=0;

	if(dFPS!=g_Temp.m_dFPS)
	{
		dFPS=g_Temp.m_dFPS;

		str.Format(_T("%.2lf"), dFPS);
		SetSpreadData(7, str);
	}

	if(dHDDspace!=g_Temp.m_dHDDspace)
	{
		dHDDspace=g_Temp.m_dHDDspace;

		str.Format(_T("%.1lf%%"), dHDDspace);
		if(dHDDspace<15.0) SetSpreadData(10, str, 1);
		else			   SetSpreadData(10, str);
	}
}

void CKoWebView::UpdateInfo()
{
	CString str;
	static double dInspTime=0;
	static double dAverageBright=0;
	static double dAverageSharp=0;
	static double dStart=0, old=0;

	dStart=Clock_set();
	if(dStart-old>100)
	{
		if(dInspTime!=g_Temp.m_dInspTime)
		{
			dInspTime=g_Temp.m_dInspTime;

			str.Format(_T("%.1lf"), dInspTime+5);
			SetSpreadData(8, str);
		}

		if(dAverageBright!=g_Temp.m_dAverageBright || dAverageSharp!=g_Temp.m_dAverageSharp)
		{
			dAverageBright=g_Temp.m_dAverageBright;
			dAverageSharp =g_Temp.m_dAverageSharp;

			str.Format(_T("%.1lf / %.2lf"), dAverageBright, dAverageSharp);
			if(dAverageBright<g_Param.m_nBrightMin || dAverageBright>g_Param.m_nBrightMax)
				SetSpreadData(9, str, 1);
			else
				SetSpreadData(9, str);
		}
		old=dStart;
	}
}

void CKoWebView::SetSpreadData(int nRow, CString str, int nColor)
{
	m_pShowInfo->SetData(nRow,str, nColor);
}

void CKoWebView::ShowParameter()
{
	m_pShowInfo->ShowParamData(g_Param.m_nExposure, g_Param.m_nNotInspArea, g_Param.m_nEdgeOffset);
}





void CKoWebView::OnBnClickedButtonTeach()
{
	if(m_pTeach!=NULL)
	{
		if(m_pTeach->IsWindowVisible()) return;
		else
		{
			delete m_pTeach;
			m_pTeach=NULL;
		}
	}
	m_pTeach=new CDlgTeach(this);
	m_pTeach->Create(IDD_DIALOG_TEACH, this);
}


void CKoWebView::OnTimer(UINT_PTR nIDEvent)
{
	static long nData[3]={0,};
	static int nCount60=0;

#if PROGRAM_TYPE==GENERAL_TEACH
	if(nIDEvent==TIMER_TEST)
	{
		if(g_Temp.m_bLive && g_Temp.m_nBinaryDialogOpen)
		{
			memcpy(g_fmTemp, g_fmGrab[g_ID], g_System.m_nImageW*g_System.m_nImageH);
			UpdateImageInfo();
		}
	}
#endif

	if(nIDEvent==TIMER_ONE_SECOND)//1초에 한번 실행
	{
		if(nData[0]!=g_Param.m_nExposure || nData[1]!=g_Param.m_nNotInspArea  || nData[2]!=g_Param.m_nEdgeOffset) //Exposure, nNotInspectArea, nEdgeOffset 바뀌면 화면에 표시
		{
			nData[0]=g_Param.m_nExposure;
			nData[1]=g_Param.m_nNotInspArea;
			nData[2]=g_Param.m_nEdgeOffset;
			ShowParameter();
		}
		SetSpreadFPS();

		
	}

	if(nIDEvent==TIMER_TEN_SECOND) //10초에 한번 실행
	{
		if(!m_pShowInfo->IsWindowVisible()) 
			m_pShowInfo->ShowWindow(SW_SHOW);
	}

	if(nIDEvent==TIMER_ONE_MINUTE) //60초에 한번 실행
	{
		g_Temp.m_dHDDspace=HDDSpace();	//HDD잔량

		if(nCount60>0) nCount60--;
		if(g_Temp.m_dHDDspace<15.0 && nCount60==0)  //30분에 한번만 보냄.
		{
			//서버에 ERROR보냄 --------------------------------------------------------
			CPacket* packet = new CPacket;
			packet->MakeReturnPacket(NM_SEND_HDD_ERROR, 1);
			l_Send_Server.SendInsData(packet);

#ifdef SERVER_TEST
			if(g_System.m_nServerTest) l_Send_ServerTest.SendInsData(packet);
#endif

			delete packet;
			//------------------------------------------------------------------------
			nCount60=30;
		}
	}

	CFormView::OnTimer(nIDEvent);
}


void CKoWebView::ResetTempData()   //OnButtonFrameReset
{
	int i, n;

	g_Temp.m_nLotChangeSignal	= g_Temp.m_nLotChangeFrame = g_Temp.m_nLotChangeFMCount = 0;
	g_Temp.m_nTapeSignal[0]		=g_Temp.m_nTapeSignal[1]   =g_Temp.m_nTapeSignal[2]=0;
	g_Temp.m_nImageInfoCount	= 0;


	g_Temp.m_nGrabDone			= 0;	
	g_Temp.m_nResetFlag			= 0;	
	ResetCount();
	
	g_Temp.m_nChangeExpoFrameID =-1;
	//주기 Reset----------------------------------------------------
	g_Temp.m_nCycleCnt=0;
	memset(g_Temp.m_nDefectFlag, 0, 10*MAX_CYCLE_COUNT*sizeof(int));
	//--------------------------------------------------------------

	g_Kipo.m_nCOSRecvTotal=0;
	g_DelayIndex=0;

	g_Temp.m_tLastSendTime	= CTime::GetCurrentTime();			//서버에 마지막으로 보낸 시간.
	m_tTimeCheckFrameOld	=g_Temp.m_tLastSendTime;			//이전 CheckFrame()에 들어온 시간.
	m_tTimeCheckFrame		=g_Temp.m_tLastSendTime;			//현재 CheckFrame에 들어온 시간.


	//LOT끝날때까지 저장이면 LotReset될때 멈춤----------------
	if(g_Temp.m_nSaveCountLot)
	{
		g_Temp.m_nSaveCountLot=0;
		g_Config.m_nImageSaveMethod=0;
		g_Config.m_nImageSaveCount=g_Temp.m_nSaveCount=100;
	}
	//--------------------------------------------------------


	SetSpreadCount();

}


void CKoWebView::AutoStart()
{
	if(!g_Temp.m_bAuto) OnBnClickedCheckAutoStart();
}
void CKoWebView::AutoStop()
{
	if(g_Temp.m_bAuto) OnBnClickedCheckAutoStart();
}


void CKoWebView::OnBnClickedCheckAutoStart()
{	
	if(g_Temp.m_bAuto)
	{
		g_Temp.m_bAuto = FALSE;
		ButtonState(FALSE);	
		Sleep(1);	
	
		g_Grabber.HookStop();
	}
	else
	{
		InitData();

#ifdef USE_DAKANO_LINE
	#if DAKANO_WIDTH==64
		memset(g_fmMAP, 0, 4096*2048);
	#else
		memset(g_fmMAP, 0, 8192*2048);
	#endif
#endif

		if(g_Temp.m_bLive)
		{			
			g_Temp.m_bLive=0;
			((CButton *)GetDlgItem(IDC_CHECK_LIVE))->SetCheck(false); 
		}
		else
		{
			g_Grabber.HookStart();
		}
		g_Temp.m_bAuto = TRUE;				
		ButtonState(TRUE);			
	}
}



void CKoWebView::InitData()
{
	ResetTempData();

#if PROGRAM_TYPE==WEB_DAKANO
	DakanoDataReset();	
#endif


	//Debug용
	g_Temp.m_dMaxInspTime=0;
	g_Temp.m_dMinInspTime=10000;
	g_Temp.m_nFrameDiffSum[0]=g_Temp.m_nFrameDiffSum[1]=0;
}

void CKoWebView::ButtonState(BOOL bStart)
{
	if(bStart)
	{
		GetDlgItem(IDC_CHECK_AUTO_START)->SetWindowTextW(_T("STOP"));
	}
	else
	{
		GetDlgItem(IDC_CHECK_AUTO_START)->SetWindowTextW(_T("START"));
	}
//	GetDlgItem(IDC_BUTTON_LOADIMAGE)->EnableWindow(!bStart); //LoadImage 버튼 Disable
	GetDlgItem(IDC_CHECK_LIVE)		->EnableWindow(!bStart); //Live Check 버튼 Disable
	GetDlgItem(IDC_BUTTON_GRAB)		->EnableWindow(!bStart); //Grab 버튼 Disable
	
}


void CKoWebView::DrawDefectRect(int nID) 
{
	CString str;
	COLORREF color=RGB(0,255,0);
	if(nID>=0 && nID<g_System.m_nMaxDefect)
	{
//		if(g_Defect.m_Defect[nID].defect_class%CLASSDEV==1)			color=RGB(255,0,0);
//		else if(g_Defect.m_Defect[nID].defect_class%CLASSDEV==2)	color=RGB(0,0,255);
//		else if(g_Defect.m_Defect[nID].defect_class%CLASSDEV==3)	color=RGB(0,255,255);
		color=RGB(255,0,0);

		str.Format(_T("%d"), nID+1);
		m_pImage->DrawTextABS(NULL, g_Defect.m_rcDefect[nID].left,g_Defect.m_rcDefect[nID].top, RGB(0,255,255), str);
		m_pImage->DrawRectangle(NULL,g_Defect.m_rcDefect[nID].left,g_Defect.m_rcDefect[nID].top,g_Defect.m_rcDefect[nID].right,g_Defect.m_rcDefect[nID].bottom, color);
	}
}

void CKoWebView::OnBnClickedCheckShowInfo()
{
	UpdateData(true);
	m_pNGImage[0]->ShowGrayInfo(m_bShowInfo);
	if(!m_bShowInfo)
		m_pNGImage[0]->UpdateImage();

#ifdef TEST_MODE
	if(g_System.m_nSimulServer==1) g_Temp.m_nReceiveServerFrame=1;
#endif
}
void CKoWebView::ShowProfile()
{
	int i, j;
	int nX1, nX2, nY;
	int nSkip;
	int tmp;
	int nSx, nSy, nEx, nEy;
	long nXX[2048], nYY[2048];
	int nCnt=0;
	long nSum=0;
	CString str;

#if PROGRAM_TYPE==GENERAL_TEACH
	GetImageProfile();
#endif	

	m_pImage->Local2Global(0, m_nDisplayY-1, &nX1, &nY);	
	m_pImage->Local2Global(m_nDisplayX-1, m_nDisplayY-1, &nX2, &nY);	

	for(i=0;i<m_nDisplayX;i++)
	{
		tmp=nX1+(nX2-nX1)*i/m_nDisplayX;
		if(tmp>=g_System.m_nImageW) break;

		nYY[nCnt]=m_nDisplayY-1-*(g_Profile+tmp);
		nXX[nCnt]=nCnt;
		nSum+=*(g_Profile+tmp);
		nCnt++;
		if(nCnt>1024) break;
	}
	m_pImage->DrawPixelArray_DisplayCoodinate(NULL, nCnt, nXX, nYY, RGB(0,255,0));
	if(nCnt<=0) return;

	//왼쪽
//	str.Format(_T("%d"), m_nDisplayY-1-(nYY[0]+nYY[1]+nYY[2]+nYY[3]+nYY[4])/5);
	str.Format(_T("%d (%.1lf, %.2lf)"), m_nDisplayY-1-(nYY[0]+nYY[1]+nYY[2]+nYY[3]+nYY[4])/5, g_Temp.m_dAverageBrightDetail[0], g_Temp.m_dAverageSharpDetail[0]);
	m_pImage->DrawText(NULL, 0, nYY[0]-20, RGB(0,255,255), str);
	//오른쪽
//	str.Format(_T("%d"), m_nDisplayY-1-(nYY[nCnt-1]+nYY[nCnt-2]+nYY[nCnt-3]+nYY[nCnt-4]+nYY[nCnt-5])/5);
	str.Format(_T("%d (%.1lf, %.2lf)"), m_nDisplayY-1-(nYY[nCnt-1]+nYY[nCnt-2]+nYY[nCnt-3]+nYY[nCnt-4]+nYY[nCnt-5])/5, g_Temp.m_dAverageBrightDetail[2], g_Temp.m_dAverageSharpDetail[2]);
	m_pImage->DrawText(NULL, m_nDisplayX-113, nYY[nCnt-1]-20, RGB(0,255,255), str);
	//중앙
//	str.Format(_T("%d"), m_nDisplayY-1-(nYY[nCnt/2-2]+nYY[nCnt/2-1]+nYY[nCnt/2]+nYY[nCnt/2+1]+nYY[nCnt/2+2])/5);
	str.Format(_T("%d (%.1lf, %.2lf)"), m_nDisplayY-1-(nYY[nCnt/2-2]+nYY[nCnt/2-1]+nYY[nCnt/2]+nYY[nCnt/2+1]+nYY[nCnt/2+2])/5, g_Temp.m_dAverageBrightDetail[1], g_Temp.m_dAverageSharpDetail[1]);
	m_pImage->DrawText(NULL, m_nDisplayX/2-55, nYY[nCnt/2]-20, RGB(0,255,255), str);

//	m_pImage->SetTextFont(1, 30, _T("Segoe UI Symbol"));
	str.Format(_T("AVG:%.1f"), (double)nSum/nCnt);
	m_pImage->DrawText(NULL, 0, nYY[0]-40, RGB(0,255,255), str);
//	m_pImage->SetTextFont(1, 30, _T("Segoe UI Symbol"))

	m_pImage->DrawLineABS(NULL, 0, m_nDisplayY-1-g_System.m_nGrabBright, m_nDisplayX, m_nDisplayY-1-g_System.m_nGrabBright, RGB(255,0,0));

}
void CKoWebView::ShowNotInspectLine()
{


#ifdef USE_SK_BAT
	if(g_Temp.m_nFoundEdge>=0 && g_Param.m_nAutoEdgeFind)
	{		
	
		m_pImage->DrawLine(NULL,g_Temp.m_dInnerEdge, 0, g_Temp.m_dInnerEdge, g_System.m_nImageH, RGB(0,0,255));

		if(g_Temp.m_dOuterEdge>0)
			m_pImage->DrawLine(NULL,g_Temp.m_dOuterEdge, 0, g_Temp.m_dOuterEdge, g_System.m_nImageH, RGB(0,255,0));
	

		if(g_Param.m_nEdgeType>0) 
		{
			m_pImage->DrawLine(NULL,g_Temp.m_nInspectX1, 0, g_Temp.m_nInspectX1, g_System.m_nImageH, RGB(255,0,0));
			if(g_Temp.m_dInnerEdge-g_Param.m_dOffset2>0)
				m_pImage->DrawLine(NULL,g_Temp.m_dInnerEdge-g_Param.m_dOffset2, 0, g_Temp.m_dInnerEdge-g_Param.m_dOffset2, g_System.m_nImageH, RGB(255,0,0));

			if(g_Temp.m_dOuterEdge>0 && g_Temp.m_dOuterEdge+g_Param.m_dOffset1<g_System.m_nImageW)
				m_pImage->DrawLine(NULL,g_Temp.m_dOuterEdge+g_Param.m_dOffset1, 0, g_Temp.m_dOuterEdge+g_Param.m_dOffset1, g_System.m_nImageH, RGB(255,0,0));

		}
		else if(g_Param.m_nEdgeType<0) 
		{
			m_pImage->DrawLine(NULL,g_Temp.m_nInspectX2, 0, g_Temp.m_nInspectX2, g_System.m_nImageH, RGB(255,0,0));
			if(g_Temp.m_dInnerEdge+g_Param.m_dOffset2<g_System.m_nImageW)
				m_pImage->DrawLine(NULL,g_Temp.m_dInnerEdge+g_Param.m_dOffset2, 0, g_Temp.m_dInnerEdge+g_Param.m_dOffset2, g_System.m_nImageH, RGB(255,0,0));

			if(g_Temp.m_dOuterEdge>0 && g_Temp.m_dOuterEdge-g_Param.m_dOffset1>0)
				m_pImage->DrawLine(NULL,g_Temp.m_dOuterEdge-g_Param.m_dOffset1, 0, g_Temp.m_dOuterEdge-g_Param.m_dOffset1, g_System.m_nImageH, RGB(255,0,0));
		}

	}
#else
	if(g_Temp.m_nFoundEdge>=0 && g_Param.m_nAutoEdgeFind)
		m_pImage->DrawLine(NULL,g_Temp.m_nFoundEdge, 0, g_Temp.m_nFoundEdge, g_System.m_nImageH, RGB(0,0,255));

	if(g_Temp.m_nInspectX1>0) 
	{
		m_pImage->DrawLine(NULL,g_Temp.m_nInspectX1, 0, g_Temp.m_nInspectX1, g_System.m_nImageH, RGB(255,0,0));
		m_pImage->DrawLine(NULL,0, 0, g_Temp.m_nInspectX1, g_System.m_nImageH,RGB(255,0,0));
		m_pImage->DrawLine(NULL,0, g_System.m_nImageH, g_Temp.m_nInspectX1, 0, RGB(255,0,0));
	}
	if(g_Temp.m_nInspectX2<g_System.m_nImageW-1) 
	{
		m_pImage->DrawLine(NULL,g_Temp.m_nInspectX2, 0, g_Temp.m_nInspectX2, g_System.m_nImageH, RGB(255,0,0));
		m_pImage->DrawLine(NULL,g_Temp.m_nInspectX2, 0, g_System.m_nImageW, g_System.m_nImageH,RGB(255,0,0));
		m_pImage->DrawLine(NULL,g_Temp.m_nInspectX2, g_System.m_nImageH, g_System.m_nImageW, 0,RGB(255,0,0));
	}
#endif
}

void CKoWebView::ShowCalibWidth()
{
	int i, j;
	int nX1, nX2, nY;
	int nSkip;
	int tmp;
	int nSx, nSy, nEx, nEy;
	long nXX[1024], nYY[1024];
	int nCnt=0;
	int nPixel, nPixelStartX;
	CString str;
	
	if(g_Param.m_dScaleFactorX<=0 || g_Config.m_dCalibWidth<=0.001) return;

	nPixel=g_Config.m_dCalibWidth/g_Param.m_dScaleFactorX;

	if(g_Config.m_dCalibStartX>0)
	{
		nPixelStartX=g_Config.m_dCalibStartX/g_Param.m_dScaleFactorX;
		nSx=nPixelStartX;
	}
	else
	{
		nPixelStartX=g_Config.m_dCalibStartX/g_Param.m_dScaleFactorX;
		nSx=(g_System.m_nImageW-nPixel)/2;
	}
	nEx= nSx+nPixel;
	
	if(nSx>0)   
	{
		m_pImage->DrawLine(NULL, nSx, 0, nSx, g_System.m_nImageH/2, RGB(0,255,0));
		if(g_Config.m_dCalibStartX>0) str.Format(_T("%.2lf mm"), g_Config.m_dCalibStartX);
		else						  str.Format(_T("%.2lf mm"), nSx*g_Param.m_dScaleFactorX);
		m_pImage->DrawTextABS(NULL, nSx+10, g_System.m_nImageH*0.25-15, RGB(0,255,255), str);
	}
	if(nEx<g_System.m_nImageW) m_pImage->DrawLine(NULL, nEx, 0, nEx, g_System.m_nImageH/2, RGB(0,255,0));	

	if(nSx>0 && nEx<g_System.m_nImageW)
	{
		 m_pImage->DrawLine(NULL, (nSx+nEx)/2, g_System.m_nImageH*0.25, (nSx+nEx)/2, g_System.m_nImageH*0.5, RGB(255,0,0));
		 m_pImage->DrawLine(NULL, nSx, g_System.m_nImageH*0.25, nEx, g_System.m_nImageH*0.25, RGB(0,0,0));
		 str.Format(_T("%.2lf mm"), g_Config.m_dCalibWidth);
		 m_pImage->DrawTextABS(NULL, (nSx+nEx)/2-80, g_System.m_nImageH*0.25-15, RGB(0,255,255), str);
	}

}



void CKoWebView::OnBnClickedCheckShowimge()
{
//CossMakeDirectoryLot(_T("aaaa"));

	if(g_Temp.m_bShowMap || g_Temp.m_bShowOldMap)
	{
		UpdateData(false);
		return;
	}

	UpdateData(true);
	if(!g_Temp.m_bAuto && m_bShowImage==0)
	{
		m_pImage->ResetGraphic();
		m_pImage->UpdateImage();
	}

	//DEBUG
	if(l_PressDebug)
	{
		GetDlgItem(IDC_BUTTON_DEBUG)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_BINARY)->ShowWindow(SW_HIDE);
		l_PressDebug=0;

#if PROGRAM_TYPE==WEB_DAKANO
	GetDlgItem(IDC_CHECK_SHOWOLDMAP)->ShowWindow(SW_HIDE);
#endif

	}
}


void CKoWebView::OnCbnSelchangeComboZoom()
{
	int nIndex=m_ctrlZoom.GetCurSel();

	switch(nIndex)
	{
		case 0: m_pImage->FitZoomXY();
			    m_pImage->FitZoomXY();break;
		case 1: m_pImage->Zoom(0.125, 0.125); break;
		case 2: m_pImage->Zoom(0.25, 0.25); break;
		case 3: m_pImage->Zoom(0.5, 0.5); break;
		case 4: m_pImage->Zoom(1, 1); break;
		case 5: m_pImage->Zoom(2, 2); break;
		case 6: m_pImage->Zoom(4, 4); break;
		case 7: m_pImage->Zoom(8, 8); break;
	}	
	
}

void CKoWebView::SetShowImage(bool flag)
{
	m_bShowImage=flag;
	((CButton *)GetDlgItem(IDC_CHECK_SHOWIMGE))->SetCheck(1);
}

//Binary 보이기>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void CKoWebView::OnBnClickedButtonBinary()
{
	if(g_Temp.m_bAuto) return;

	if(m_pBinary!=NULL)
	{
		if(m_pBinary->IsWindowVisible()) return;
		else
		{
			delete m_pBinary;
			m_pBinary=NULL;
		}
	}
	m_pBinary=new CDlgThreshold(this);
	m_pBinary->Create(IDD_DIALOG_THRESHOLD, this);

#ifdef TEST_MODE
	m_pBinary->MoveWindow(950,0,810,685);
#else
	m_pBinary->MoveWindow(950,0,410,685);
#endif

	int nX, nY, nMax, nMin;
	double dAve, dStdev, dSharp;
	m_pImage->Local2Global(300, 300, &nX, &nY);	
	m_pImage->m_pROI->AddROI(_T("t"), nX-BAD_IMG_WIDTH/2, nY-BAD_IMG_WIDTH/2, nX+BAD_IMG_WIDTH/2, nY+BAD_IMG_WIDTH/2);	
	m_pImage->m_pROI->ShowROI(true);
	m_pImage->m_pROI->EnableROI(true);
	m_pImage->UpdateImage();

	dAve=GetAverage(g_fmGrab[g_ID], nX-BAD_IMG_WIDTH/2, nY-BAD_IMG_WIDTH/2, nX+BAD_IMG_WIDTH/2-1, nY+BAD_IMG_WIDTH/2-1, g_System.m_nPitch);
	GetMinMax(g_fmGrab[g_ID], nX-BAD_IMG_WIDTH/2, nY-BAD_IMG_WIDTH/2, nX+BAD_IMG_WIDTH/2-1, nY+BAD_IMG_WIDTH/2-1, g_System.m_nPitch, &nMax, &nMin, &dSharp);
	dStdev=GetStdevArea(g_fmGrab[g_ID] ,nX-BAD_IMG_WIDTH/2, nY-BAD_IMG_WIDTH/2, nX+BAD_IMG_WIDTH/2-1, nY+BAD_IMG_WIDTH/2-1, g_System.m_nPitch);
	m_pBinary->SetImageInfo(dAve, dStdev, nMax, nMin, dSharp, BAD_IMG_WIDTH, BAD_IMG_WIDTH, -1, 0);
	g_Temp.m_nBinaryDialogOpen=1;
}

void CKoWebView::CloseBinary(bool bOriginal)
{
	m_pImage->m_pROI->DeleteROI(_T("t"));

	bOriginal=false;
	if(bOriginal)
	{
		memcpy( m_pImage->m_fmDisplay, g_fmGrab[g_ID], g_System.m_nImageW*g_System.m_nImageH);
		m_pImage->UpdateImage();
	}
	else
	{
		memcpy( g_fmGrab[g_ID], g_fmTemp, g_System.m_nImageW*g_System.m_nImageH);
		memcpy( m_pImage->m_fmDisplay, g_fmGrab[g_ID], g_System.m_nImageW*g_System.m_nImageH);
		m_pImage->UpdateImage();
	}
	g_Temp.m_nBinaryDialogOpen=0;
}

void CKoWebView::ROIFull(bool bFull)
{
	m_pImage->m_pROI->SetROIMaxSize(_T("t"),bFull);
	m_pImage->UpdateImage();
}

void CKoWebView::ShowOriginal()
{
	memcpy( m_pImage->m_fmDisplay, g_fmGrab[g_ID], g_System.m_nImageW*g_System.m_nImageH);
	memcpy( g_fmTemp, g_fmGrab[g_ID], g_System.m_nImageW*g_System.m_nImageH);
	m_pImage->UpdateImage();
}

void CKoWebView::AppplyImage()
{
	memcpy(g_fmTemp, m_pImage->m_fmDisplay, g_System.m_nImageW*g_System.m_nImageH);
	m_pImage->UpdateImage();
}
void CKoWebView::ShowApplyImage()
{
	memcpy( m_pImage->m_fmDisplay, g_fmTemp, g_System.m_nImageW*g_System.m_nImageH);
	m_pImage->UpdateImage();
}

void CKoWebView::Binary(int nThreshold)
{
	int i, j;
	int ll,tt,rr,bb;

	m_pImage->m_pROI->GetROIRegion(_T("t"), &ll, &tt, &rr, &bb);
	if(ll==0 && rr==0) return;

#if 1
	for(i=tt;i<bb-1;i++)
		for(j=ll;j<rr-1;j++)
			if(*(g_fmTemp+g_System.m_nPitch*i+j)>nThreshold) *(m_pImage->m_fmDisplay+g_System.m_nPitch*i+j)=255;
			else										     *(m_pImage->m_fmDisplay+g_System.m_nPitch*i+j)=0;

#else
	SmoothAndBinary(10, 5, 3);
#endif

	m_pImage->UpdateImage();
}


void CKoWebView::FlipHor()
{
	int i, j;
	int ll,tt,rr,bb;

	m_pImage->m_pROI->GetROIRegion(_T("t"), &ll, &tt, &rr, &bb);

	ImageFlipHor(m_pImage->m_fmDisplay, ll, tt, rr, bb, g_System.m_nPitch);
	
	m_pImage->UpdateImage();
}
void CKoWebView::FlipVer()
{
	int i, j;
	int ll,tt,rr,bb;

	m_pImage->m_pROI->GetROIRegion(_T("t"), &ll, &tt, &rr, &bb);

	ImageFlipVer(m_pImage->m_fmDisplay, ll, tt, rr, bb, g_System.m_nPitch);
	
	m_pImage->UpdateImage();
}
void CKoWebView::ImageElongation(int nX, int nY)
{
	int ret;
	int ll,tt,rr,bb;

	m_pImage->m_pROI->GetROIRegion(_T("t"), &ll, &tt, &rr, &bb);

	ret=ImageElongate(g_fmTemp, m_pImage->m_fmDisplay, ll, tt, rr, bb, g_System.m_nPitch, nX, nY);
	
	m_pImage->UpdateImage();
}


void CKoWebView::InverseImage()
{
	int i, j;
	int ll,tt,rr,bb;

	m_pImage->m_pROI->GetROIRegion(_T("t"), &ll, &tt, &rr, &bb);

	for(i=tt;i<bb;i++)
		for(j=ll;j<rr;j++)
			 *(m_pImage->m_fmDisplay+g_System.m_nPitch*i+j)=255-*(m_pImage->m_fmDisplay+g_System.m_nPitch*i+j);
	
	m_pImage->UpdateImage();
}


void CKoWebView::Binary(int nThreshold, int ll, int tt, int rr, int bb)
{
	int i, j;	

	for(i=tt;i<bb;i++)
		for(j=ll;j<rr;j++)
			if(*(g_fmGrab[g_ID]+g_System.m_nPitch*i+j)>nThreshold) *(m_pImage->m_fmDisplay+g_System.m_nPitch*i+j)=255;
			else												   *(m_pImage->m_fmDisplay+g_System.m_nPitch*i+j)=0;
	
	m_pImage->UpdateImage();
}

void CKoWebView::RotateImage(double  dAngle)
{
	int i, j;
	int ll,tt,rr,bb;

	m_pImage->m_pROI->GetROIRegion(_T("t"), &ll, &tt, &rr, &bb);

	MakeRotateImage(g_fmTemp, m_pImage->m_fmDisplay, ll,tt,rr,bb, g_System.m_nPitch, g_System.m_nImageW, g_System.m_nImageH, (ll+rr)/2.0, (tt+bb)/2.0, 0,0, dAngle);

	
	m_pImage->UpdateImage();
}
void CKoWebView::SmoothingImage()
{
	int i, j;
	int ll,tt,rr,bb;

	m_pImage->m_pROI->GetROIRegion(_T("t"), &ll, &tt, &rr, &bb);
	for(i=tt;i<bb;i++)
		for(j=ll;j<rr;j++)
			*(g_fmTemp1+g_System.m_nPitch*i+j)=*(m_pImage->m_fmDisplay+g_System.m_nPitch*i+j);
	COSS_Mean(g_fmTemp1,m_pImage->m_fmDisplay, ll,tt,rr,bb, g_System.m_nPitch,3,3);
	
	m_pImage->UpdateImage();
}
void CKoWebView::ErodeImage()
{
	int i, j;
	int ll,tt,rr,bb;

	m_pImage->m_pROI->GetROIRegion(_T("t"), &ll, &tt, &rr, &bb);
	for(i=tt;i<bb;i++)
		for(j=ll;j<rr;j++)
			*(g_fmTemp1+g_System.m_nPitch*i+j)=*(m_pImage->m_fmDisplay+g_System.m_nPitch*i+j);
	Erode_Gray(g_fmTemp1,m_pImage->m_fmDisplay, ll,tt,rr,bb, g_System.m_nPitch,NULL,3,3);
	
	m_pImage->UpdateImage();
}
void CKoWebView::EmphasizeImage(int nX, int nY, double dFactor)
{
	int i, j;
	int ll,tt,rr,bb;

	m_pImage->m_pROI->GetROIRegion(_T("t"), &ll, &tt, &rr, &bb);
	Emphasize(0, g_fmTemp,m_pImage->m_fmDisplay, ll,tt,rr,bb, g_System.m_nPitch, nX, nY, dFactor);
//TEST	ThreadEmphasize(g_fmTemp,m_pImage->m_fmDisplay, 4096, tt, bb, g_System.m_nPitch, nX, nY, 50);
	m_pImage->UpdateImage();
}

void CKoWebView::RemoveLowHigh(int nCutLow, int nCutHigh, int nKernel)
{
	int i, j;
	int ll,tt,rr,bb;
	int pitch=g_System.m_nPitch;
	long nHisto[256];
	int nMax=0, nAve;
	LPBYTE fm=m_pImage->m_fmDisplay;

	m_pImage->m_pROI->GetROIRegion(_T("t"), &ll, &tt, &rr, &bb);

	memset(nHisto, 0, 256*sizeof(long));
	for(i=tt;i<bb;i++)
		for(j=ll;j<rr;j++)
			nHisto[*(g_fmTemp+pitch*i+j)]++;
	
	for(i=0;i<256;i++)
		if(nHisto[i]>nMax)
		{
			nMax=nHisto[i];
			nAve=i;
		}

	for(i=tt;i<bb;i++)
		for(j=ll;j<rr;j++)
			if(*(g_fmTemp+pitch*i+j)>nCutHigh)		*(g_fmTemp1+pitch*i+j)=255;
			else if(*(g_fmTemp+pitch*i+j)<nCutLow)	*(g_fmTemp1+pitch*i+j)=255;
			else									*(g_fmTemp1+pitch*i+j)=0;

	BinaryDilate(g_fmTemp1, g_fmSave, ll,tt,rr,bb, pitch, 2*nKernel+1, 2*nKernel+1);
		
	for(i=tt;i<bb;i++)
		for(j=ll;j<rr;j++)
			if(*(g_fmSave+pitch*i+j)==255)		*(fm+pitch*i+j)=nAve;
			else								*(fm+pitch*i+j)=*(g_fmTemp+pitch*i+j);
	
	m_pImage->UpdateImage();
}

void CKoWebView::ShowGray(int ll, int tt, int rr, int bb)
{
	int i, j;	

	for(i=tt;i<bb;i++)
		for(j=ll;j<rr;j++)
			*(m_pImage->m_fmDisplay+g_System.m_nPitch*i+j)=*(g_fmGrab[g_ID]+g_System.m_nPitch*i+j);
	
	m_pImage->UpdateImage();
}

void CKoWebView::FlatImageROI(int nBaseGray)
{
	int ll,tt,rr,bb;
	m_pImage->m_pROI->GetROIRegion(_T("t"), &ll, &tt, &rr, &bb);
	MakeFlatROI(g_fmTemp, m_pImage->m_fmDisplay, nBaseGray, ll,tt,(rr-ll),(bb-tt), g_System.m_nPitch);
	m_pImage->UpdateImage();
}
void CKoWebView::FlatImageFull(int nType, int nBaseGray, int left, int width)
{
	switch(nType)
	{
		case 0: GetImageProfile();
			    MakeFlatLineScan(g_fmTemp, m_pImage->m_fmDisplay,nBaseGray, left, width, g_System.m_nImageH, g_System.m_nPitch, g_Temp.m_dAverageBright);
				break;
		case 1: Flat_Area_320_2(g_fmTemp, m_pImage->m_fmDisplay, nBaseGray, left, width, g_System.m_nImageH, g_System.m_nPitch);
				break;
		case 2:
				
		case 3: 
				Flat_Area(25, g_fmTemp, m_pImage->m_fmDisplay, nBaseGray, left, width, g_System.m_nImageH, g_System.m_nPitch);
				break;

	}


	m_pImage->UpdateImage();
}

void CKoWebView::RemoveHorNoise(int nTh)
{	
	RemoveHorlineNoise(m_pImage->m_fmDisplay, 0,g_System.m_nImageW, g_System.m_nImageH, g_System.m_nPitch, nTh);
	m_pImage->UpdateImage();
}

void CKoWebView::MakePolaImage()
{	
	LPBYTE fm0=m_pImage->m_fmDisplay;
	int nWidth=1232;
	int nHeight=1028;
	int pitch=g_System.m_nPitch;
	int i,j;

	for(i=0;i<nHeight;i++)
		for(j=0;j<nWidth;j++)
		{
			*(fm0+pitch*2*i+2*j)=*(g_fmGrab[2]+pitch*i+j);		//90
			*(fm0+pitch*2*i+2*j+1)=*(g_fmGrab[1]+pitch*i+j);		//45
			*(fm0+pitch*(2*i+1)+2*j)=*(g_fmGrab[3]+pitch*i+j);	//135
			*(fm0+pitch*(2*i+1)+2*j+1)=*(g_fmGrab[0]+pitch*i+j); //0
		}

	
	m_pImage->UpdateImage();
}

void CKoWebView::MakeSeparatemage()
{	
	LPBYTE fm0,fm1,fm2,fm3, fmS0, fmS1, fmS2, fmAoP, fmDoLP;
	int i, j;
	int nWidth=1232;
	int nHeight=1024;
	int pitch=g_System.m_nPitch;

	fm0=new BYTE[nWidth*nHeight];
	fm1=new BYTE[nWidth*nHeight];
	fm2=new BYTE[nWidth*nHeight];
	fm3=new BYTE[nWidth*nHeight];
	fmS0=new BYTE[nWidth*nHeight];
	fmS1=new BYTE[nWidth*nHeight];
	fmS2=new BYTE[nWidth*nHeight];
	fmAoP=new BYTE[nWidth*nHeight];
	fmDoLP=new BYTE[nWidth*nHeight];

	MakeMultiImage(m_pImage->m_fmDisplay, fm0,fm1,fm2,fm3, fmS0, fmS1, fmS2, fmAoP, fmDoLP, nWidth, nHeight);

	//좌상단
	for(i=0;i<nHeight;i++)
		for(j=0;j<nWidth;j++)
		{
			*(m_pImage->m_fmDisplay+pitch*i+j)=*(fmS1+nWidth*i+j);				
		}
	//우상단
	for(i=0;i<nHeight;i++)
		for(j=0;j<nWidth;j++)
		{
			*(m_pImage->m_fmDisplay+pitch*i+nWidth+j)=*(fmS2+nWidth*i+j);				
		}

	//좌하단
	for(i=0;i<nHeight;i++)
		for(j=0;j<nWidth;j++)
		{
			*(m_pImage->m_fmDisplay+pitch*(i+nHeight)+j)=*(fmAoP+nWidth*i+j);				
		}
	//우하단
	for(i=0;i<nHeight;i++)
		for(j=0;j<nWidth;j++)
		{
			*(m_pImage->m_fmDisplay+pitch*(i+nHeight)+nWidth+j)=*(fmDoLP+nWidth*i+j);				
		}

	delete fm0;
	delete fm1;
	delete fm2;
	delete fm3;
	delete fmS0;
	delete fmS1;
	delete fmS2;
	delete fmAoP;
	delete fmDoLP;


	m_pImage->UpdateImage();
}


void CKoWebView::FindHBOSize(double dBase, int nThUp, int nCoreTh, double dCloudTh, int *pValue, int *CoreSize, double *pCloudValue, int *pCloudSize)
{
	int i, j;
	int ll,tt,rr,bb;
	int pitch=g_System.m_nPitch;
	int Cx, Cy;
	double dAvg, dCloudValue;
	int nIsWhite;
	int nSum=0;


	*CoreSize=*pCloudValue=*pCloudSize=0;
	m_pImage->m_pROI->GetROIRegion(_T("t"), &ll, &tt, &rr, &bb);

	if(rr-ll>BAD_IMG_WIDTH || bb-tt>BAD_IMG_HEIGHT) return;


	//영역 평균 구하기--------------------------------------------
	for(i=tt;i<tt+128;i++)
		for(j=ll;j<ll+128;j++)
		{
			nSum+=*(m_pImage->m_fmDisplay+pitch*i+j);
			*(g_fmDefectTemp+BAD_IMG_WIDTH*(i-tt)+(j-ll))=*(m_pImage->m_fmDisplay+pitch*i+j);
		}
	dAvg=(double)nSum/(128*128);
	//-----------------------------------------------------------


//	GetHBOSize(g_fmDefectTemp, 0, 0, BAD_IMG_WIDTH, BAD_IMG_HEIGHT, BAD_IMG_WIDTH, dAvg, nCoreTh, dCloudTh, &nIsWhite, &Cx, &Cy, pValue, CoreSize, pCloudValue, pCloudSize);
//	if(nIsWhite==0) *pCloudValue*=-1;

    Get2Size(g_fmDefectTemp, 0, 0, BAD_IMG_WIDTH, BAD_IMG_HEIGHT, BAD_IMG_WIDTH, 2,2, dAvg, nCoreTh, nCoreTh, dCloudTh, dCloudTh, &Cx, &Cy, pValue, CoreSize, pCloudValue, pCloudSize);


	
//	for(i=tt;i<bb;i++)
//		for(j=ll;j<rr;j++)
//			*(m_pImage->m_fmDisplay+g_System.m_nPitch*i+j)=*(g_fmDefectTemp1+BAD_IMG_WIDTH*(i-tt)+j-ll);

	//Show Graphic
	m_pImage->SetAddGraphic(true);
	m_pImage->DrawCircle(NULL, ll+Cx, tt+Cy, *CoreSize/2, RGB(255,0,0));    //Core Circle
//	m_pImage->DrawCircle(NULL, ll+Cx, tt+Cy, *Size/2, RGB(255,0,0));		//2nd Circle
	m_pImage->DrawCircle(NULL, ll+Cx, tt+Cy, (int)*pCloudSize/2, RGB(0,255,255));   //Cloud Circle
		
	m_pImage->UpdateImage();
}

void CKoWebView::CossFilter_MakeImageWithSobel(int nTh)
{
	
}


void CKoWebView::OnLButtonUp(UINT nFlags, CPoint point)
{
	UpdateImageInfo();

	CFormView::OnLButtonUp(nFlags, point);
}
void CKoWebView::UpdateImageInfo()
{
	int ll,tt,rr,bb, nMax, nMin, nWaterPrintValue=-1;
	double dAve, dStdev, dSharp, dWaterValue=0;

	if(g_Temp.m_nBinaryDialogOpen)
	{
		m_pImage->m_pROI->GetROIRegion(_T("t"), &ll, &tt, &rr, &bb);
		dAve=GetAverage(g_fmTemp, ll,tt,rr,bb, g_System.m_nPitch);
		dStdev=GetStdevArea(g_fmTemp, ll,tt,rr,bb, g_System.m_nPitch);
		GetMinMax(g_fmTemp, ll,tt,rr,bb, g_System.m_nPitch, &nMax, &nMin, &dSharp);
		if(m_pBinary->m_bWaterPrint)
			nWaterPrintValue=GetWaterPrint(g_fmTemp, ll,tt,rr,bb, g_System.m_nPitch, m_pBinary->m_nCutHigh, m_pBinary->m_nOffset, m_pBinary->m_nOffsetValue, m_pBinary->m_nMinYLength, &dWaterValue);
	
		m_pBinary->SetImageInfo(dAve, dStdev, nMax, nMin, dSharp, rr-ll, bb-tt, nWaterPrintValue, dWaterValue*100.0);
	}

}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


void CKoWebView::OnBnClickedButtonPattern()
{
	if(m_pPattern!=NULL)
	{
		if(m_pPattern->IsWindowVisible()) return;
		else
		{
			delete m_pPattern;
			m_pPattern=NULL;
		}
	}
	m_pPattern=new CDlgPattern(this);
	m_pPattern->Create(IDD_DIALOG_PATTERN, this);
}

void CKoWebView::OnBnClickedButtonOcr()
{
	if(m_pOCR!=NULL)
	{
		if(m_pOCR->IsWindowVisible()) return;
		else
		{
			delete m_pOCR;
			m_pOCR=NULL;
		}
	}
	m_pOCR=new CDlgTeachOCR(this);
	m_pOCR->Create(IDD_DIALOG_TEACHOCR, this);
}

void CKoWebView::OnBnClickedButtonAlign()
{
	if(m_pAlign!=NULL)
	{
		if(m_pAlign->IsWindowVisible()) return;
		else
		{
			delete m_pAlign;
			m_pAlign=NULL;
		}
	}
	m_pAlign=new CDlgAlign(this);
	m_pAlign->SetID(g_InspSet);
	m_pAlign->Create(IDD_DIALOG_ALIGN, this);
}

//각종 함수들 테스트하는 곳
//1) Align+Image Rotation (4096x2048) 이미지로 35msec I7-8700 3.2GHz
void CKoWebView::OnBnClickedButtonFuncTest()
{
	int i, j;
	LPBYTE fm=m_pImage->m_fmDisplay;
	CString str;
	
	int nMaxPos;
	double end[10], start;
	int ret;


//	for(i=0;i<1000;i++)
//		SendLotName(_T("1234567890ABCDE")); // Cossclean에 lot정보 보내기 테스트
//	AfxMessageBox(_T("1"));
	
/*
//Align후 영상을 회전한다----------------------------------------------------
	str.Format(_T("%sAlignData.dat"), MAIN_FOLDER);
	LoadAlignData(str);	//Align데이타 읽는 함수

	start=Clock_set();
	ret=FindAlignPos(g_ID);								//Align을 위한 좌표구하기
	if(ret==0) //성공
	{
		CalAlignData(&g_AlignData[g_ID]);				//Align데이타 구하기(Rotation, Translation 회전중심)
		
		ThreadFunc_MakeRotateImage(g_fmGrab[g_ID],  fm, 0,0, g_System.m_nImageW, g_System.m_nImageH, g_System.m_nImageW, g_System.m_nImageW, g_System.m_nImageH, g_AlignData[g_ID].m_dCx, g_AlignData[g_ID].m_dCy, -g_AlignData[g_ID].m_dOffsetX,-g_AlignData[g_ID].m_dOffsetY,-g_AlignData[g_ID].m_dAngleDeg); //영상회전
	}
//---------------------------------------------------------------------------
	end[0]=Clock_set();

	memcpy(g_fmGrab[g_ID], fm, g_System.m_nImageW*g_System.m_nImageH);
	m_pImage->UpdateImage();

	end[1]=Clock_set();

	m_ctrlResult.ResetContent();
	str.Format(_T("Align+Rotation %.3lf  ShowImage:%.3lf"), end[0]-start, end[1]-end[0] );
	m_ctrlResult.AddString(str);
*/


#ifdef MY_RECV
	int nCode, nNELRecvCount=2, nNELRecvX[20], nNELRecvY[20];
		
	nNELRecvX[0]=660;
	nNELRecvY[0]=20+500;

	nNELRecvX[1]=800;
	nNELRecvY[1]=100+500;


	KO_SetKipoPress(8, nNELRecvCount, nNELRecvX, nNELRecvY);
#endif

#ifdef AI_APPLY
	int nIndex;
	double dMaxTime=0, dConfidenceArray[5];
	long nCount=0;
	int nNGCount[5];

	//테스트할 이미지-------------------------------------
	LPBYTE fmM=new BYTE[128*128];
	for(i=0;i<128;i++)
		for(j=0;j<128;j++)
			*(fmM+128*i+j)=*(fm+g_System.m_nImageW*i+j);
	//---------------------------------------------------
	nNGCount[0]=nNGCount[1]=nNGCount[2]=nNGCount[3]=nNGCount[4]=0;
	while(1)
	{
	
		start=Clock_set();

		//분류 AI Call-------------------------------------------------
		//필요한 함수 AI_FindDefectName(..) 한개임, 아무데서나 Call하면 됨.
		nIndex=AI_FindDefectName(fmM, 128,128, 50, dConfidenceArray);
		
		end[1]=Clock_set();
		
		if(end[1]-start>dMaxTime) dMaxTime=end[1]-start;

		nCount++;
		if(nIndex<0) nNGCount[nIndex*(-1)]++;
		
		str.Format(_T("Al Test( %d NG:%d-%d-%d-%d ): %.3lf (Max:%.3lf)  =>%d"), nCount, nNGCount[1], nNGCount[2],nNGCount[3],nNGCount[4], end[1]-start ,dMaxTime, nIndex);
		m_nResultStringCnt=0;				 //m_ctrlResult.ResetContent();
		m_sResult[m_nResultStringCnt++]=str; //m_ctrlResult.AddString(str);
		if(nIndex>0 )
		{
			str.Format(_T("%.5lf %.5lf %.5lf %.5lf %.5lf"), dConfidenceArray[0],dConfidenceArray[1],dConfidenceArray[2],dConfidenceArray[3],dConfidenceArray[4]);
			m_sResult[m_nResultStringCnt++]=str;  //m_ctrlResult.AddString(str);
		}

		HandleAllMessage();
		if(m_bShowFlat) 
			break;
	}
	delete fmM;
#endif
}



void CKoWebView::SetAlignROI(int nSet)
{
	int i;
	int left,top,right, bottom;

	for(i=0;i<g_ROINum[g_AlignData[nSet].m_nAlignType];i++)
	{
		m_pImage->m_pROI->GetROIRegion(g_ROIName[g_AlignData[nSet].m_nAlignType][i], &left, &top, &right, &bottom);			

		g_AlignData[nSet].m_rc[i].left		=left;
		g_AlignData[nSet].m_rc[i].top		=top;
		g_AlignData[nSet].m_rc[i].right		=right;
		g_AlignData[nSet].m_rc[i].bottom	=bottom;
	}
}
void CKoWebView::ShowAlignROI(int nSet, BOOL bShow)
{
	int i;
	
	if(bShow)
	{
		m_pImage->m_pROI->EnableROI(true);
		m_pImage->m_pROI->ShowROI(true);
		for(i=0;i<g_ROINum[g_AlignData[nSet].m_nAlignType];i++)
		{
			if(g_AlignData[nSet].m_rc[i].left>0)
				m_pImage->m_pROI->AddROI(g_ROIName[g_AlignData[nSet].m_nAlignType][i], g_AlignData[nSet].m_rc[i].left, g_AlignData[nSet].m_rc[i].top,g_AlignData[nSet].m_rc[i].right, g_AlignData[nSet].m_rc[i].bottom);			
			else
				m_pImage->m_pROI->AddROI(g_ROIName[g_AlignData[nSet].m_nAlignType][i], 200+200*i, 200, 380+200*i, 380);
		}
	}	
	else
	{
		for(i=0;i<g_ROINum[g_AlignData[nSet].m_nAlignType];i++)
			m_pImage->m_pROI->DeleteROI(g_ROIName[g_AlignData[nSet].m_nAlignType][i]);			
	}
	m_pImage->UpdateImage();
}
/*
void CKoWebView::ShowAlignROI(int nSet, BOOL bShow)
{
	int i;
	if(g_AlignData[nSet].m_nRegistered)
	{
		if(bShow)
		{
			m_pImage->m_pROI->EnableROI(true);
			m_pImage->m_pROI->ShowROI(true);
			for(i=0;i<g_ROINum[g_AlignData[nSet].m_nAlignType];i++)
			{
				if(g_AlignData[nSet].m_rc[i].left>0)
					m_pImage->m_pROI->AddROI(g_ROIName[g_AlignData[nSet].m_nAlignType][i], g_AlignData[nSet].m_rc[i].left, g_AlignData[nSet].m_rc[i].top,g_AlignData[nSet].m_rc[i].right, g_AlignData[nSet].m_rc[i].bottom);			
				else
					m_pImage->m_pROI->AddROI(g_ROIName[g_AlignData[nSet].m_nAlignType][i], 200+200*i, 200, 380+200*i, 380);
			}
		}	
		else
		{
			for(i=0;i<g_ROINum[g_AlignData[nSet].m_nAlignType];i++)
				m_pImage->m_pROI->DeleteROI(g_ROIName[g_AlignData[nSet].m_nAlignType][i]);			
		}
		m_pImage->UpdateImage();
	}
	else
	{
		if(bShow)
		{
			m_pImage->m_pROI->EnableROI(true);
			m_pImage->m_pROI->ShowROI(true);
			for(i=0;i<g_ROINum[g_AlignData[nSet].m_nAlignType];i++)
				m_pImage->m_pROI->AddROI(g_ROIName[g_AlignData[nSet].m_nAlignType][i], 200+200*i, 200, 380+200*i, 380);			
		}
		else
		{
			for(i=0;i<g_ROINum[g_AlignData[nSet].m_nAlignType];i++)
				m_pImage->m_pROI->DeleteROI(g_ROIName[g_AlignData[nSet].m_nAlignType][i]);			
		}
		m_pImage->UpdateImage();
	}
	*/

int CKoWebView::FindAlignPos(int nSet)
{
	int i, ret=0, retAll=1;
	double t,a,b,dCx, dCy;
	double dScore;
	CString str=_T(""), strTmp;
	int N=g_AlignData[nSet].m_nAlignType;
	//if(!g_AlignData[nSet].m_nRegistered)
	//{
	//	AfxMessageBox(_T("Not Registered Align"));
	//	return;
	//}

	for(i=0;i<g_ROINum[N];i++)  //ROI수만큼
	{
		g_AlignData[nSet].m_dX[i]=0;
		switch(g_AlignData[nSet].m_nROIType[i])
		{
		case 0:	//Y찾기 (수평Edge)
				ret=FindLine(g_fmGrab[g_ID], g_AlignData[nSet].m_rc[i].left, g_AlignData[nSet].m_rc[i].top, g_AlignData[nSet].m_rc[i].right, g_AlignData[nSet].m_rc[i].bottom, g_System.m_nImageW, 
						g_AlignData[nSet].m_nDir[i], 0, g_AlignData[nSet].m_nMethod[i], g_AlignData[nSet].m_nObject[i], g_AlignData[nSet].m_nGrayTh[i], g_AlignData[nSet].m_nSlope[i], g_AlignData[nSet].m_nAvgLine[i], true, m_pImage, &t, &a, &b, &dCx, &dCy);
				if(ret)
				{
					g_AlignData[nSet].t[i]=t;
					g_AlignData[nSet].a[i]=a;
					g_AlignData[nSet].b[i]=b;
					g_AlignData[nSet].m_dX[i]=dCx;
					g_AlignData[nSet].m_dY[i]=dCy;
				}
				else
				{
					retAll=0;
					strTmp.Format(_T("%s, "), g_ROIName[N][i]);
					str+=strTmp;
				}
				break;
		case 1: //X찾기 (수직Edge)
				ret=FindLine(g_fmGrab[g_ID], g_AlignData[nSet].m_rc[i].left, g_AlignData[nSet].m_rc[i].top, g_AlignData[nSet].m_rc[i].right, g_AlignData[nSet].m_rc[i].bottom, g_System.m_nImageW, 
						g_AlignData[nSet].m_nDir[i], 1, g_AlignData[nSet].m_nMethod[i], g_AlignData[nSet].m_nObject[i], g_AlignData[nSet].m_nGrayTh[i], g_AlignData[nSet].m_nSlope[i], g_AlignData[nSet].m_nAvgLine[i], true, m_pImage, &t, &a, &b, &dCx, &dCy);
				
				if(ret)
				{
					g_AlignData[nSet].t[i]=t;
					g_AlignData[nSet].a[i]=a;
					g_AlignData[nSet].b[i]=b;
					g_AlignData[nSet].m_dX[i]=dCx;
					g_AlignData[nSet].m_dY[i]=dCy;
				}
				else
				{
					retAll=0;
					strTmp.Format(_T("%s, "), g_ROIName[N][i]);
					str+=strTmp;
				}
				break;
		case 2: //X,Y찾기 (패턴매칭)
				PatternMatch(g_AlignData[nSet].m_nPatternID[i], g_fmGrab[g_ID], g_AlignData[nSet].m_rc[i].left, g_AlignData[nSet].m_rc[i].top, g_AlignData[nSet].m_rc[i].right, g_AlignData[nSet].m_rc[i].bottom, g_System.m_nImageW, &dScore, &dCx, &dCy);
				if(dScore>=g_AlignData[nSet].m_nPatternMinScore[i])
				{
					g_AlignData[nSet].m_dX[i]=dCx;
					g_AlignData[nSet].m_dY[i]=dCy;
					m_pImage->DrawCross(NULL, dCx, dCy, 10, RGB(0,0,255));
				}
				else
				{
					retAll=0;
					strTmp.Format(_T("%s, "), g_ROIName[N][i]);
					str+=strTmp;
				}
				break;
		}
	}

	if(retAll==0)
	{
		if(!g_Temp.m_bAuto)
		{
			strTmp.Format(_T("%s Failure"), str);
			AfxMessageBox(strTmp);
		}
		return -1;
	}
	return 0;
}






void CKoWebView::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	

	CFormView::OnClose();
}


void CKoWebView::OnBnClickedButtonNext()
{
	if(m_nShowNGID+1<g_Defect.m_nDefectCount)
	{
		m_nShowNGID++;
		m_pNGImage[0]->PutImageData(g_Defect.m_pImage[m_nShowNGID],BAD_IMG_WIDTH,BAD_IMG_HEIGHT,1);		
		ShowDefectInfo(m_nShowNGID);
	}

	//TEST(모드변경)  MODE 2S
//	char szCmd[30];
//	sprintf(szCmd, "w smod 1\r");
//	g_Sio.Write((LPBYTE)szCmd, strlen(szCmd));

/* 
   LOT끊는 것
	if(g_Temp.m_nLotChangeSignal>0) return;
			
	g_Temp.m_nLotChangeSignal=1;
	g_Temp.m_nLotChangeFrame =g_Temp.m_nGrabFrame;


	g_Temp.m_nCkSplice = 2;
*/
	#ifdef TEST_MODE
//	LotReset();
//	ReadSubParam();	//20 Level 데이타 읽는 것 테스트

	//------------------------RESET 테스트----------------------------------------------
	//int i,flagRest=0;
	//		CString sFileName;
	//		sFileName.Format(_T("%s%s\\%s.txt"), LOT_PATH, g_Temp.m_slotName,g_Temp.m_sMyComName);
	//		for(i=0;i<100;i++)  //1초기다림.
	//		{
	//			if(g_SaveNGCnt==0)
	//			{
	//				DeleteFile(sFileName);
	//				InitData();
	//				ResetCount();
	//				flagRest=1;
	//				break;
	//			}
	//			Sleep(10);
	//		}

	//		if(flagRest==0) //1초동안 못지우면 그냥 강제 지움.
	//		{
	//			DeleteFile(sFileName);
	//			InitData();
	//			ResetCount();
	//		}
	//------------------------------------------------------------------------------------------


	#endif		
}


void CKoWebView::OnBnClickedButtonPrev()
{
	if(m_nShowNGID-1>=0)
	{
		m_nShowNGID--;
		m_pNGImage[0]->PutImageData(g_Defect.m_pImage[m_nShowNGID],BAD_IMG_WIDTH,BAD_IMG_HEIGHT,1);	
		ShowDefectInfo(m_nShowNGID);
	}

	//TEST  MODE 1S
//	char szCmd[30];
//	sprintf(szCmd, "w smod 0\r");
//	g_Sio.Write((LPBYTE)szCmd, strlen(szCmd));

#ifdef TEST_MODE
//	SaveSubParam();
#endif
}


//View화면에 불량정보 결과표시
void CKoWebView::ShowDefectInfo(int nID)
{
	CString str;
	m_nDefecttextCnt=0;//m_ctrlDefectInfo.ResetContent();

	str.Format(_T("%s  %d/%d"), g_Temp.m_sMyComName, nID+1, g_Defect.m_nDefectCount);
	m_sDefectText[m_nDefecttextCnt++]=str;//m_ctrlDefectInfo.AddString(str);

	#ifdef USE_HEX_FORMAT
		if((int)g_DefectSend.m_Defect[nID].dTemp[0]%2==1)
			str.Format(_T("Level: %d  (%X)  M"), g_DefectSend.m_Defect[nID].defect_class/MILLION, g_DefectSend.m_Defect[nID].defect_class);
		else
			str.Format(_T("Level: %d  (%X)"), g_DefectSend.m_Defect[nID].defect_class/MILLION, g_DefectSend.m_Defect[nID].defect_class);
	#else
		if((int)g_DefectSend.m_Defect[nID].dTemp[0]%2==1)
			str.Format(_T("Level: %d  (%d)  M"), g_DefectSend.m_Defect[nID].defect_class/MILLION, g_DefectSend.m_Defect[nID].defect_class);
		else
			str.Format(_T("Level: %d  (%d)"), g_DefectSend.m_Defect[nID].defect_class/MILLION, g_DefectSend.m_Defect[nID].defect_class);
	#endif

	m_sDefectText[m_nDefecttextCnt++]=str;//m_ctrlDefectInfo.AddString(str);


	
	if( (g_DefectSend.m_Defect[nID].defect_class/OPTICDEV)%PERIODDEV>0)
		str.Format(_T("Type: %s (Period)"), g_sDefectName[g_Temp.m_nPCOptic][g_DefectSend.m_Defect[nID].defect_class%CLASSDEV]);
	else
		str.Format(_T("Type: %s"), g_sDefectName[g_Temp.m_nPCOptic][g_DefectSend.m_Defect[nID].defect_class%CLASSDEV]);

	m_sDefectText[m_nDefecttextCnt++]=str;//m_ctrlDefectInfo.AddString(str);
//#endif

	str.Format(_T("Value: %d  "), (int)g_DefectSend.m_Defect[nID].value);
	m_sDefectText[m_nDefecttextCnt++]=str;//m_ctrlDefectInfo.AddString(str);

#ifdef USE_HEX_FORMAT
	if(g_DefectSend.m_Defect[nID].defect_class%OPTICDEV==0x44 || g_DefectSend.m_Defect[nID].defect_class%OPTICDEV==0x78)
#else
	if(g_DefectSend.m_Defect[nID].defect_class%OPTICDEV==44 || g_DefectSend.m_Defect[nID].defect_class%OPTICDEV==78)
#endif
	{
		str.Format(_T("Mark: %.3lf "), g_DefectSend.m_Defect[nID].size);
	}
	else
	{
		if(g_DefectSend.m_Defect[nID].defect_class%OPTICDEV==56)
		{
			str.Format(_T("Count: %d"), (int)g_DefectSend.m_Defect[nID].size);
		}
		else
		{
			if(g_Param.m_nSizeMethod)
				str.Format(_T("Size: %.3lf  (L:%.3lf  S:%.3lf)"), g_DefectSend.m_Defect[nID].size, g_DefectSend.m_Defect[nID].sizeX, g_DefectSend.m_Defect[nID].sizeY);
			else		
				str.Format(_T("Size: %.3lf  (%.3lfx%.3lf)"), g_DefectSend.m_Defect[nID].size, g_DefectSend.m_Defect[nID].sizeX, g_DefectSend.m_Defect[nID].sizeY);
		}
	}

	m_sDefectText[m_nDefecttextCnt++]=str;//m_ctrlDefectInfo.AddString(str);

	if(g_Param.m_dScaleFactorX>0) str.Format(_T("X: %.2lf (%d Pxl)"), g_DefectSend.m_Defect[nID].x_pos, (int)((g_DefectSend.m_Defect[nID].x_pos-g_Param.m_dCamStartPosX)/g_Param.m_dScaleFactorX));
	else                          str.Format(_T("X: %.2lf"), g_DefectSend.m_Defect[nID].x_pos);
	m_sDefectText[m_nDefecttextCnt++]=str;//m_ctrlDefectInfo.AddString(str);

	if(g_Param.m_dScaleFactorY>0) str.Format(_T("Y: %.2lf (%d Pxl)"), g_DefectSend.m_Defect[nID].y_pos, (int)((g_DefectSend.m_Defect[nID].y_pos-g_Param.m_dCamStartPosY)/g_Param.m_dScaleFactorY));
	else						  str.Format(_T("Y: %.2lf"), g_DefectSend.m_Defect[nID].y_pos);
	m_sDefectText[m_nDefecttextCnt++]=str;//m_ctrlDefectInfo.AddString(str);

	//2줄 추가 2020.7.16
	str.Format(_T("Frame: %d"), g_Temp.m_nGrabFrameID);
	m_sDefectText[m_nDefecttextCnt++]=str;//m_ctrlDefectInfo.AddString(str);


	DrawDefectViewText();
}


void CKoWebView::OnDestroy()
{
	DestroyDefectCallClass();
	ViewClose();
	CFormView::OnDestroy();
}


void CKoWebView::ViewClose()
{

#if GRABBER_TYPE==NO_GRABBER
	if(m_nSimulRun)
	{
		m_nSimulRun=0;
		Sleep(500);
	}
#endif
	AI_Close();
	NittoAI_Close();


	AutoStop();
	if(g_Temp.m_bLive) g_Grabber.HookStop();

	g_Grabber.Close(true);	//g_GrabThreadRun=g_GrabThreadSubRun=0;

	ClosePyramid();			//delete l_fmPyramid
	CloseUDP();				//모든 통신 Close
	CloseInspData();		//Inspect 데이타 Memory Free

	g_Temp.m_bLive=0;
	g_Temp.m_bAuto=0;
	g_Temp.m_nPGStart=0;
	Sleep(100);
}

void CKoWebView::OnLButtonDown(UINT nFlags, CPoint point)
{
	WINDOWPLACEMENT wp;
	GetDlgItem(IDC_BUTTON_DEBUG)->GetWindowPlacement(&wp);
	if(point.x>wp.rcNormalPosition.left && point.x<wp.rcNormalPosition.right &&
       point.y>wp.rcNormalPosition.top && point.y<wp.rcNormalPosition.bottom)
	{
		l_PressDebug++;
		if(l_PressDebug==5)
		{
			GetDlgItem(IDC_BUTTON_DEBUG)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_BUTTON_BINARY)->ShowWindow(SW_SHOW);

#if PROGRAM_TYPE==WEB_DAKANO
	GetDlgItem(IDC_CHECK_SHOWOLDMAP)->ShowWindow(SW_SHOW);
#endif

		}
	}
	CFormView::OnLButtonDown(nFlags, point);
}


void CKoWebView::OnBnClickedButtonDebug()
{
	if(m_pDebug!=NULL)
	{
		if(m_pDebug->IsWindowVisible()) return;
		else
		{
			delete m_pDebug;
			m_pDebug=NULL;
		}
	}
	m_pDebug=new CDlgDebug(this);
	m_pDebug->Create(IDD_DIALOG_DEBUG, this);
}


void CKoWebView::OnDraw(CDC* pDC)
{
	DrawBackGround(pDC);
	DrawMainViewText(pDC);	 //각종 정보 (PC, 밝기 Lot등)
	DrawResultViewText(pDC); //Result 정보
	DrawDefectViewText(pDC); //Defect Info

#ifdef MARKING_VISION
	if(m_bShowImage)
		DrawMarkingVisionROI(pDC);
#endif
/*
    //안깜박이게 아래와 같이 했는데 별 의미 없음. 이미 DrawBackGround 가 비슷한 역할 함
	CRect rc;
	GetClientRect(rc);

	//MEMDC(설정)---------------------------------------------------------------
	CDC dc;			  
	CBitmap *pOldbmp;
    CBitmap *pOldbmpBac;
	CBitmap bitmap;
	dc.CreateCompatibleDC(pDC);
	bitmap.CreateCompatibleBitmap(pDC,  rc.Width(), rc.Height());
	pOldbmp = dc.SelectObject(&bitmap);
	//--------------------------------------------------------------------------

	DrawBackGround(&dc);

	//MEMDC(적용)-------------------------------------------------------------------------------------
	pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(),&dc, 0,0 , SRCCOPY);
	dc.SelectObject(pOldbmp);
    dc.DeleteDC();
	//------------------------------------------------------------------------------------------------
*/
}

#ifdef MARKING_VISION
void CKoWebView::DrawMarkingVisionROI(CDC *pDC)
{
	int i;
	CString str;
	COLORREF colorYELLOW=RGB(255,255,0);
	COLORREF colorBLUE=RGB(0,0,255);

//	m_pImage->ResetGraphic();
	for(i=0;i<g_MakringVision.m_rcCnt;i++)
	{
		str.Format(_T("V: %,1lf, %,1lf"), g_MakringVision.m_nResultData[i][0], g_MakringVision.m_nResultData[i][1]);
		m_pImage->DrawTextABS(NULL, g_MakringVision.m_rcInsp[i].left, g_MakringVision.m_rcInsp[i].top, RGB(255,255,255), str);
		if(g_MakringVision.m_nResult[i]==2) m_pImage->DrawRectangle(NULL,g_MakringVision.m_rcInsp[i].left,g_MakringVision.m_rcInsp[i].top,g_MakringVision.m_rcInsp[i].right,g_MakringVision.m_rcInsp[i].bottom, colorBLUE);
		else								m_pImage->DrawRectangle(NULL,g_MakringVision.m_rcInsp[i].left,g_MakringVision.m_rcInsp[i].top,g_MakringVision.m_rcInsp[i].right,g_MakringVision.m_rcInsp[i].bottom, colorYELLOW);
	}
}
#endif

void CKoWebView::DrawMainViewText(CDC *pDC)
{
	int i;
	CFont *oldFont, font;
	font.CreatePointFont(100, _T("Segoe UI Symbol"));
	oldFont=pDC->SelectObject(&font);

	pDC->SetBkMode(TRANSPARENT);
	for(i=0;i<g_nMainViewCnt;i++)
	{
		pDC->SetTextColor(g_cMainView[i]);
		pDC->DrawText(g_sMainView[i], _tcsclen(g_sMainView[i]), g_rcMainView[i], DT_CENTER|DT_VCENTER|DT_SINGLELINE	);
	}

	pDC->SelectObject(oldFont);
}

void CKoWebView::DrawResultViewText()
{
	static double start=0, old=0;
	start=Clock_set();
	if(start-old>100)
	{
		InvalidateRect(m_rcResult, false);
		old=start;
	}
}
void CKoWebView::DrawResultViewText(CDC *pDC)
{
	int i;
	CFont *oldFont, font;
	CRect rc;
	int nLineGap=18;
	COLORREF csText(RGB(0,0,0));
	font.CreatePointFont(100, _T("Segoe UI Symbol"));
	oldFont=pDC->SelectObject(&font);
	CBrush backGD(RGB(200,200,200));
	pDC->FillRect(m_rcResult, &backGD);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(csText);
	rc=m_rcResult;
	rc.left+=10;
	for(i=0;i<m_nResultStringCnt;i++)
	{		
		rc.top   =m_rcResult.top+i*nLineGap;
		rc.bottom=m_rcResult.top+(i+1)*nLineGap;
		pDC->DrawText(m_sResult[i], _tcsclen(m_sResult[i]), rc , DT_LEFT|DT_VCENTER|DT_SINGLELINE	);
	}

	pDC->SelectObject(oldFont);
}

void CKoWebView::DrawDefectViewText()
{
	static double start=0, old=0;
	start=Clock_set();
	
	if(start-old>100)
	{
		InvalidateRect(m_rcDefect, false);
		old=start;
	}
}

void CKoWebView::DrawDefectViewText(CDC *pDC)
{
	int i;
	CFont *oldFont, font;
	CRect rc;
	int nLineGap=18;
	COLORREF csText(RGB(0,0,0));
	font.CreatePointFont(100, _T("Segoe UI Symbol"));
	oldFont=pDC->SelectObject(&font);
	CBrush backGD(RGB(200,200,200));
	pDC->FillRect(m_rcDefect, &backGD);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(csText);
	rc=m_rcDefect;
	rc.left+=5;
	for(i=0;i<m_nDefecttextCnt;i++)
	{		
		rc.top   =m_rcDefect.top+i*nLineGap;
		rc.bottom=m_rcDefect.top+(i+1)*nLineGap;
		pDC->DrawText(m_sDefectText[i], _tcsclen(m_sDefectText[i]), rc , DT_LEFT|DT_VCENTER|DT_SINGLELINE	);
	}

	pDC->SelectObject(oldFont);
}

//IDB_MAINIMAGE Bitmap을 배경화면에 깔기
void CKoWebView::DrawBackGround(CDC *pDC)
{
	 CRect rect;
	 GetClientRect(rect);
	 CDC dc;
	 CBitmap bmp,*pOldBmp;
	 BITMAP bm;
	 int eTile=0;
	 dc.CreateCompatibleDC(pDC);
	 bmp.LoadBitmap(IDB_MAINIMAGE);
	 bmp.GetObject(sizeof(bm),&bm);
	 pOldBmp=dc.SelectObject(&bmp);
	 if(eTile)
	 {
		for(int x=0;x<rect.Width();x+=bm.bmWidth)
			for(int y=0;y<rect.Height();y+=bm.bmHeight)
				pDC->BitBlt(x,y,bm.bmWidth,bm.bmHeight,&dc,0,0,SRCCOPY);
	 }
	 else
	 {
//		pDC->StretchBlt(0,0,rect.Width(),rect.Height(),&dc,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
		pDC->BitBlt(0,0,bm.bmWidth,bm.bmHeight,&dc,0,0,SRCCOPY);
	 }
	 dc.SelectObject(pOldBmp);
}

//임시로 만듦
void CKoWebView::ShowResults_General()
{
	int i, nId, NG=0;

	for(i=0;i<MAX_INSPECT;i++)
	{
		nId=g_Inspect[g_InspSet].m_nID[i];
		if(nId<0 || nId>=MAX_INSPECT) continue;

		if(!g_Inspect[g_InspSet].m_nUSE[nId]) continue;

		NG=IsInspectResultNG(nId);
		/*for(j=0;j<MAX_INSPECT;j++)
			if(nId==nIdArray[j])
			{
				m_Spread.SetRow(j+2);
				if(NG) str=_T("NG");
				else   str=_T("OK");
				m_Spread.SetValue(str);
				break;
			}		*/
	}
	if(NG) g_Defect.m_nDefectCount=1;
	else   g_Defect.m_nDefectCount=0;
}

void CKoWebView::ShowResults()
{
	//화면에 이미지 Update------------------------------------------------------------------
	if(m_bShowImage)
	{
		m_pImage->SetAddGraphic(true);
	}
	//---------------------------------------------------------------------------------------

	DrawResults();
	if(m_bShowImage)
	{
#ifdef SHOW_USE_THREAD
		g_Temp.m_nImageSize=g_System.m_nImageW*g_System.m_nImageH;
		g_Temp.m_fmImageDst=m_pImage->m_fmDisplay;
		if(m_bShowFlat && g_Temp.m_nFlatSuccess) g_Temp.m_fmImageScr=g_fmFlat[g_ID];
		else									 g_Temp.m_fmImageScr=g_fmGrab[g_ID];
		g_Temp.m_nImageCopy=1;
#else		
		m_pImage->UpdateImage();
		if(m_bShowFlat && g_Temp.m_nFlatSuccess)
		{
			m_pImage->PutImageData(g_fmFlat[g_ID], g_System.m_nImageW, g_System.m_nImageH,1);			
		}
		else
		{
			m_pImage->PutImageData(g_fmGrab[g_ID], g_System.m_nImageW, g_System.m_nImageH,1);
		}
#endif

	}
}

HBRUSH CKoWebView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	int nID=pWnd->GetDlgCtrlID();
	
	switch(nID)
	{
		case IDC_CHECK_SHOWIMGE:
		case IDC_CHECK_LIVE:
		case IDC_CHECK_SHOW_INFO:
		case IDC_STATIC_UDP:
		case IDC_STATIC_ZOOM:
		case IDC_CHECK_SHOWMAP:
		case IDC_CHECK_SHOWOLDMAP:
		case IDC_CHECK_ALL:		
		

		//	pDC->SetBkColor(RGB(0,0,0));
			pDC->SetTextColor(RGB(255,255,255));
			pDC->SetBkMode(TRANSPARENT);
			return (HBRUSH)::GetStockObject(NULL_BRUSH);
			break;
		case IDC_STATIC_RECV:
			pDC->SetBkColor(RGB(40,40,40));
			pDC->SetTextColor(RGB(255,255,255));
			return (HBRUSH)::GetStockObject(NULL_BRUSH);
			break;
		case IDC_STATIC_WEB:
		case IDC_STATIC_SETTING:
				pDC->SetBkColor(RGB(40,40,40));
				pDC->SetTextColor(RGB(255,255,255));
				return (HBRUSH)::GetStockObject(NULL_BRUSH);
				break;
		default: break;	
	}
	return hbr;
}


BOOL CKoWebView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE) return FALSE;
	}

	return CFormView::PreTranslateMessage(pMsg);
}


void CKoWebView::SendLotName(CString sLotName) 
{    
	HWND Receiver_hwnd, Sender_hwnd;
	TCHAR str[100];
	COPYDATASTRUCT cds;
			
    Receiver_hwnd=::FindWindow(NULL, _T("Cossclean"));
    if(Receiver_hwnd==NULL) return;

	cds.dwData=-1;
	cds.cbData=40;
	_stprintf(str,_T("%s"), g_Temp.m_slotName);
	cds.lpData =str;

	Sender_hwnd=AfxGetApp()->m_pMainWnd->GetSafeHwnd();
	::SendMessage(Receiver_hwnd, WM_COPYDATA, (WPARAM)Sender_hwnd, (LPARAM)&cds );	
}



void CKoWebView::OnBnClickedButtonClose()
{
	if(AfxMessageBox(_T("Quit?"), MB_OKCANCEL) == IDCANCEL)	return;
	WriteLog(_T("Close Button"));
	GetParentFrame()->SendMessage(WM_COMMAND, ID_APP_EXIT, NULL);
}


void CKoWebView::OnBnClickedCheckShowmap()
{
	UpdateData(true);
	g_Temp.m_bShowMap=m_bShowMap;
	if(m_bShowMap)
	{
		m_bShowImage=0;
		m_pImage->ResetGraphic();
		m_pImage->UpdateImage();
#if DAKANO_WIDTH==64
		m_pImage->PutImageData(g_fmMAP, 4096,2048,1);
#else
		m_pImage->PutImageData(g_fmMAP, 8192,2048,1);
#endif
		UpdateData(false);
		m_pShowInfo->CheckAllReset();
		GetDlgItem(IDC_BUTTON_INSPECT)->EnableWindow(false);
	}
	else
	{
		m_pImage->PutImageData(g_fmGrab[g_ID], g_System.m_nImageW, g_System.m_nImageH,1);
		GetDlgItem(IDC_BUTTON_INSPECT)->EnableWindow(true);
	}
}

void CKoWebView::OnBnClickedCheckShowoldmap()
{
/*	UpdateData(true);
	g_Temp.m_bShowOldMap=m_bShowOldMap;
	if(m_bShowOldMap)
	{
		m_bShowImage=0;
		if(!m_bShowMap) g_Temp.m_bShowMap=m_bShowMap=true;
		m_pImage->PutImageData(g_fmOldMAP, 4096, 2048,1);
		m_pShowInfo->CheckAllReset();
		GetDlgItem(IDC_CHECK_SHOWMAP)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_INSPECT)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_INSPECT3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK_ALL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_EDIT_YLENGTH)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_M)->ShowWindow(SW_SHOW);
	}
	else
	{
		g_Temp.m_bShowMap=m_bShowMap=false;
		m_pImage->PutImageData(g_fmGrab[g_ID], g_System.m_nImageW, g_System.m_nImageH,1);
		GetDlgItem(IDC_CHECK_SHOWMAP)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_INSPECT)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_INSPECT3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_ALL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_YLENGTH)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_M)->ShowWindow(SW_HIDE);
	}
	UpdateData(false);
*/
}


void CKoWebView::OnBnClickedRadioCam1()
{
	SelectImage(0);
}

void CKoWebView::OnBnClickedRadioCam2()
{
	SelectImage(1);
}

void CKoWebView::OnBnClickedRadioCam3()
{
	SelectImage(2);
}

void CKoWebView::OnBnClickedRadioCam4()
{
	SelectImage(3);
}

void CKoWebView::SelectImage(int nSet)
{
#if MAX_IMAGE>1
	if(g_InspSet==nSet) return;
	if(nSet<0) nSet=0;

	g_InspSet=g_ID=nSet;
	
	m_pImage=m_pImageArray[nSet];
	//Align Dialog가 열려 있으면 Align Dialog를 카메라에 맞게 설정한다.---
	if(m_pAlign && m_pAlign->IsWindowVisible()) 
		m_pAlign->UpdateGUI();
	//--------------------------------------------------------------------

	((CButton *)GetDlgItem(IDC_RADIO_CAM1+nSet))->SetCheck(1);
	if(m_pTeach&& m_pTeach->IsWindowVisible())
	{
		delete m_pTeach;
		m_pTeach=NULL;
	}
	if(m_bShowMax)
		ShowMax(true);
	m_pImage->PutImageData(g_fmGrab[g_ID], g_System.m_nImageW, g_System.m_nImageH,1);
#endif
}

void CKoWebView::OnBnClickedButtonModel()
{
	if(g_Temp.m_bAuto) return;

	if(m_pModel!=NULL)
	{
		if(m_pModel->IsWindowVisible()) return;
		else
		{
			delete m_pModel;
			m_pModel=NULL;
		}
	}
	m_pModel=new CDlgModel(this);
	m_pModel->Create(IDD_DIALOG_MODEL, this);
}


void CKoWebView::OnBnClickedCheckShowmax()
{
	UpdateData(true);
	ShowMax(m_bShowMax);
}

void CKoWebView::ShowMax(bool bShowMax)
{
#if MAX_IMAGE>1
	int i;
	if(bShowMax)
	{
		for(i=0;i<4;i++)
		{
			if(i==g_ID)
			{
				m_pImageArray[i]->MoveWindow(m_rcImageMax);
				m_pImageArray[i]->ShowWindow(SW_SHOW);
				m_pImageArray[i]->FitZoomXY();
			}
			else
				m_pImageArray[i]->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		for(i=0;i<4;i++)
		{
			m_pImageArray[i]->MoveWindow(m_rcImage[i]);
			m_pImageArray[i]->ShowWindow(SW_SHOW);
			m_pImageArray[i]->FitZoomXY();
			m_pImageArray[i]->FitZoomXY();
		}
	}
#endif
}

void CKoWebView::SmoothAndBinary(int nThup, int nThDn, int wingsize)
{
	int i, j;
	int ll,tt,rr,bb;

	m_pImage->m_pROI->GetROIRegion(_T("t"), &ll, &tt, &rr, &bb);
	if(ll==0 && rr==0) return;

	int pitch = g_System.m_nPitch;
	int height = g_System.m_nImageH;
	int width = g_System.m_nImageW;

	unsigned char *pMask33 = new unsigned char[pitch*height];
	unsigned char *pAtt = new unsigned char[pitch*height];

	FilterAverageMean(g_fmTemp, pitch, height, wingsize,pMask33);
	NearBrgDiffAll(nThup, nThDn, CRect(ll,tt,rr,bb), pMask33,pitch,height,pAtt, wingsize);

	for(i=tt;i<bb-1;i++)
		for(j=ll;j<rr-1;j++)
			if(*(pAtt+pitch*i+j)==0)
				*(pAtt+pitch*i+j)=255;
			else
				*(pAtt+pitch*i+j)=0;

	for(i=tt;i<bb-1;i++)
		for(j=ll;j<rr-1;j++)
			*(m_pImage->m_fmDisplay+g_System.m_nPitch*i+j)=*(pAtt+pitch*i+j);

	delete[] pMask33;
	delete[] pAtt;
}

//윙사이즈 만큼 평균값을 구함
void CKoWebView::FilterAverageMean(unsigned char *pSImg, int nImgXSize, int nImgYSize, int nWingSize,unsigned char* pDImg)
{
	int i;
	if(nWingSize==0)
	{
		for(int i=0;i<nImgYSize;i++)
			for(int j=0;j<nImgXSize;j++)
				*(pDImg+i*nImgXSize+j) = *(pSImg+(i)*nImgXSize+j);
		return;
	}
	double meanVal = 0;
	double dSmallSize = ((nWingSize*2+1)*(nWingSize*2+1));
	for(i=nWingSize;i<nImgYSize-nWingSize;i++)
	{

		double sStartMean = 0;
		for(int k=-nWingSize;k<=nWingSize;k++)
			for(int l=-nWingSize;l<=nWingSize;l++)
				{
					sStartMean += *(pSImg+(i+k)*nImgXSize+(nWingSize+l));
				}
		sStartMean/=((nWingSize*2+1)*(nWingSize*2+1));
		*(pDImg+i*nImgXSize+nWingSize) = (sStartMean);

		for(int j=nWingSize+1;j<nImgXSize-nWingSize;j++)
		{
		
			double dMinusSum = 0;
			double dPlusSum = 0;
			for(int k=i-nWingSize;k<=(i+nWingSize);k++)
			{
				dPlusSum += *(pSImg+(k)*nImgXSize+j+nWingSize);
				dMinusSum += *(pSImg+(k)*nImgXSize+j-nWingSize-1);
			}
			dMinusSum /=dSmallSize;
			dPlusSum /=dSmallSize;
			sStartMean += dPlusSum;
			sStartMean -= dMinusSum;
			
			*(pDImg+i*nImgXSize+j) = (sStartMean);
		}
	}
	for(i=nWingSize;i<nImgXSize-nWingSize;i++)
	{
		for(int j=0;j<nWingSize;j++)
		{
			*(pDImg+j*nImgXSize+i) = *(pDImg+(nWingSize)*nImgXSize+i);
			*(pDImg+(j+nImgYSize-nWingSize)*nImgXSize+i) = *(pDImg+(nImgYSize-1-nWingSize)*nImgXSize+i);
		}
	}
	for(i=nWingSize;i<nImgYSize-nWingSize;i++)
	{
		for(int j=0;j<nWingSize;j++)
		{
			*(pDImg+i*nImgXSize+j) = *(pDImg+(i)*nImgXSize+nWingSize);
			*(pDImg+(i)*nImgXSize+nImgXSize-nWingSize+j) = *(pDImg+(i)*nImgXSize+nImgXSize-1-nWingSize);
		}
	}


	double nVal = *(pDImg+nWingSize*nImgXSize+nWingSize);
	for(i=0;i<nWingSize;i++)
		for(int j=0;j<nWingSize;j++)
		{
			*(pDImg+i*nImgXSize+j) = nVal;
		}
	nVal = *(pDImg+nWingSize*nImgXSize+nImgXSize-1-nWingSize);
	for(i=0;i<nWingSize;i++)
		for(int j=nImgXSize-nWingSize;j<nImgXSize;j++)
		{
			*(pDImg+i*nImgXSize+j) = nVal;
		}

	 nVal = *(pDImg+(nImgYSize-1-nWingSize)*nImgXSize+nWingSize);
	for(i=nImgYSize-nWingSize;i<nImgYSize;i++)
		for(int j=0;j<nWingSize;j++)
		{
			*(pDImg+i*nImgXSize+j) = nVal;
		}


	nVal = *(pDImg+(nImgYSize-1-nWingSize)*nImgXSize+nImgXSize-1-nWingSize);
	for(i=nImgYSize-nWingSize;i<nImgYSize;i++)
		for(int j=nImgXSize-nWingSize;j<nImgXSize;j++)
		{
			*(pDImg+i*nImgXSize+j) = nVal;
		}
}

void CKoWebView::NearBrgDiffAll(int thup, int thdn, CRect insRect, unsigned char* pMask33,int nWidth,int nHeight,unsigned char* pAtt, int nWingSize)
{
	CRect rShrAtt = insRect;
//		rShrAtt.OffsetRect(-m_rAranged.left,-m_rAranged.top);
	rShrAtt.DeflateRect(2,2);
	int nCenterVal;
	int nLeftVal;
	int nRightVal;
	int nLeftVal2;
	int nRightVal2;	
	int nLeftVal3;
	int nRightVal3;
	int nLeftVal4;
	int nRightVal4;

	if(nWingSize==0)
		nWingSize = 1;

	int nWing=(nWingSize)*3;
	int nWIngDist = (nWing+nWingSize);
	memset(pAtt,255,nWidth*nHeight);
	int nBrgH,nBrgL;
	int gradeVal;

	nBrgH = thup;
	nBrgL = thdn;
	for(int i=rShrAtt.top+(nWIngDist);i<rShrAtt.bottom-(nWIngDist);i+=1)
	{
		for(int j=rShrAtt.left+(nWIngDist)+1;j<rShrAtt.right-(nWIngDist)-1;j+=1)
		{
			nCenterVal = *(pMask33+nWidth*i+j);
			nLeftVal = *(pMask33+nWidth*(i-nWIngDist)+j-nWIngDist);
			nRightVal = *(pMask33+nWidth*(i+nWIngDist)+j+nWIngDist);

			nLeftVal2 = *(pMask33+nWidth*(i+nWIngDist)+j-nWIngDist);
			nRightVal2 = *(pMask33+nWidth*(i-nWIngDist)+j+nWIngDist);

			nLeftVal3 = *(pMask33+nWidth*(i+nWIngDist)+j);				
			nRightVal3 = *(pMask33+nWidth*(i-nWIngDist)+j);

			nLeftVal4 = *(pMask33+nWidth*(i)+j-nWIngDist);
			nRightVal4 = *(pMask33+nWidth*(i)+j+nWIngDist);


			if((((nCenterVal - nLeftVal)>=nBrgH)&&((nCenterVal-nRightVal)>=nBrgH))||
			(((nCenterVal - nLeftVal2)>=nBrgH)&&((nCenterVal-nRightVal2)>=nBrgH))||
			(((nCenterVal - nLeftVal3)>=nBrgH)&&((nCenterVal-nRightVal3)>=nBrgH))||
			(((nCenterVal - nLeftVal4)>=nBrgH)&&((nCenterVal-nRightVal4)>=nBrgH))||
			(((nLeftVal-nCenterVal)>=nBrgL)&&((nRightVal-nCenterVal)>=nBrgL))||
			(((nLeftVal2-nCenterVal)>=nBrgL)&&((nRightVal2-nCenterVal)>=nBrgL))||
			(((nLeftVal3-nCenterVal)>=nBrgL)&&((nRightVal3-nCenterVal)>=nBrgL))||
			(((nLeftVal4-nCenterVal)>=nBrgL)&&((nRightVal4-nCenterVal)>=nBrgL)))
			{
				for(int k=-nWingSize;k<=nWingSize;k++)
				{
					for(int l=-nWingSize;l<=nWingSize;l++)
						*(pAtt+(i+k)*nWidth+(j+l))=0;
				}
			}
		}
	}
}

void CKoWebView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	double dZoom=m_pImage->GetZoom();
	int nOldPos=m_ctrlZoom.GetCurSel();
	int nPos=0;
	if(fabs(dZoom-0.125)<0.001) nPos=1;
	else if(fabs(dZoom-0.25)<0.001) nPos=2;
	else if(fabs(dZoom-0.5)<0.001) nPos=3;
	else if(fabs(dZoom-1)<0.001) nPos=4;
	else if(fabs(dZoom-2)<0.001) nPos=5;
	else if(fabs(dZoom-4)<0.001) nPos=6;
	else if(fabs(dZoom-8)<0.001) nPos=7;
	if(nOldPos!=nPos)
		m_ctrlZoom.SetCurSel(nPos);
	

	CFormView::OnLButtonDblClk(nFlags, point);
}

void CKoWebView::OnBnClickedButtonInspect3() 
{
#ifdef USE_DAKANO_LINE
	FuncBaseLineSetting();	
#endif
}



void CKoWebView::OnBnClickedCheckAll()
{
	UpdateData(true);
}



void CKoWebView::CreateDefectCallCallss()
{
	if (m_DefectCallClass == nullptr)
		m_DefectCallClass = new CallClassWrapper();
	if (m_DefectReadingEvent == nullptr)
	{
		m_DefectReadingEvent = new CallClassReadingEvents(this->m_hWnd);
		m_DefectCallClass->AddEndCsvReading(m_DefectReadingEvent);
	}
}

void CKoWebView::DestroyDefectCallClass()
{
	if (m_DefectCallClass != nullptr)
	{
		m_DefectCallClass->RemoveEndCsvReading(m_DefectReadingEvent);
		delete m_DefectCallClass;
	}

	if (m_DefectReadingEvent != nullptr)
		delete m_DefectReadingEvent;
}

void CKoWebView::OnBnClickedBtnShowDefectNow()
{
	if (m_DefectCallClass == nullptr)
		return;

	m_DefectCallClass->ShowDefectView(false);
}

void CKoWebView::OnBnClickedBtnShowDefectNext()
{
	if (m_DefectCallClass == nullptr)
		return;

	m_DefectCallClass->ShowDefectView(true);
}

LRESULT CKoWebView::OnBCrComm(WPARAM wParam, LPARAM lParam)
{
	// index는 ICallClass tlh에 있는 enum 기준으로 처리
	int evtIdx = (int)wParam;
	
	switch (evtIdx)
	{
	case eEventReport_eReadCSVNow:
		m_DefectCallClass->GetMarkingData(false);
		break;
	case eEventReport_eReadCSVNext:

		break;

	case eEventReport_eReadDBNow:

		break;

	case eEventReport_eReadDBNext:

		break;

	case eEventReport_eResetDataNow:

		break;

	case eEventReport_eResetDataNext:

		break;
	}
	return 0;
}