// DlgConfig.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "DlgConfig.h"
#include "afxdialogex.h"


// CDlgConfig 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgConfig, CDialogEx)

CDlgConfig::CDlgConfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgConfig::IDD, pParent)
{

	m_nImageType = 0;
	m_nSaveMethod = 0;
	m_nFrameNumber = 0;
	m_dCalibWidth = 50.0;
	m_dScaleFactorX = 0.05;
	m_nComPort = 0;
	m_nSaveNumber = 0;

	m_pCameraCalib=NULL;
	m_dCalibStartX = 0.0;
}

CDlgConfig::~CDlgConfig()
{
	if(m_pCameraCalib)	delete m_pCameraCalib;
}

void CDlgConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_IMAGE_TYPE, m_nImageType);
	DDX_Radio(pDX, IDC_RADIO_SAVE_METHOD, m_nSaveMethod);
	DDX_Text(pDX, IDC_EDIT_FRAME_NUMBER, m_nFrameNumber);
	DDX_Check(pDX, IDC_CHECK_SHOWPROFILE, m_bShowProfile);
	DDX_Text(pDX, IDC_EDIT_CALIBWIDTH, m_dCalibWidth);
	DDX_Check(pDX, IDC_CHECK_CALIBWIDTH, m_bCalibWidth);
	DDX_Text(pDX, IDC_EDIT_SCALEFACTORX, m_dScaleFactorX);
	DDX_Check(pDX, IDC_CHECK_INVERSE, m_bImageInverse);
	DDX_Check(pDX, IDC_CHECK_PERIOD, m_nModePeriod);
	DDX_Text(pDX, IDC_EDIT_COMPORT, m_nComPort);
	DDV_MinMaxInt(pDX, m_nComPort, 1, 100);
	DDX_Text(pDX, IDC_EDIT_SAVE_NUMBER, m_nSaveNumber);
	DDX_Text(pDX, IDC_EDIT_CALIBSTARTX, m_dCalibStartX);
}


BEGIN_MESSAGE_MAP(CDlgConfig, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgConfig::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SET_FRAME_NUMBER, &CDlgConfig::OnBnClickedButtonSetFrameNumber)
	ON_BN_CLICKED(IDC_BUTTON_SET_SCALE, &CDlgConfig::OnBnClickedButtonSetScale)
	ON_BN_CLICKED(IDC_CHECK_CALIBWIDTH, &CDlgConfig::OnBnClickedCheckCalibwidth)
	ON_BN_CLICKED(IDC_CHECK_PERIOD, &CDlgConfig::OnBnClickedCheckPeriod)
	ON_BN_CLICKED(IDC_BUTTON_COMPORTSET, &CDlgConfig::OnBnClickedButtonComportset)
	ON_BN_CLICKED(IDC_RADIO_SAVE_METHOD, &CDlgConfig::OnBnClickedRadioSaveMethod)
	ON_BN_CLICKED(IDC_RADIO_SAVE_METHOD2, &CDlgConfig::OnBnClickedRadioSaveMethod2)
	ON_BN_CLICKED(IDC_RADIO_SAVE_METHOD3, &CDlgConfig::OnBnClickedRadioSaveMethod3)
	ON_BN_CLICKED(IDC_BUTTON_TOOSET, &CDlgConfig::OnBnClickedButtonTooset)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MOTION, &CDlgConfig::OnDeltaposSpinMotion)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE, &CDlgConfig::OnBnClickedButtonRotate)
	ON_BN_CLICKED(IDC_CHECK_SHOWPROFILE, &CDlgConfig::OnBnClickedCheckShowprofile)
END_MESSAGE_MAP()


// CDlgConfig 메시지 처리기입니다.


BOOL CDlgConfig::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LoadConfigData();

	m_nImageType  =g_Config.m_nImageType;
	m_nSaveMethod =g_Config.m_nImageSaveMethod;
	m_nSaveNumber =g_Config.m_nImageSaveCount;
	m_bShowProfile=g_Config.m_nShowProfile;
	m_bCalibWidth =g_Config.m_nShowCalibWidth;
	m_dCalibWidth =g_Config.m_dCalibWidth;
	m_dCalibStartX =g_Config.m_dCalibStartX;
	m_dScaleFactorX=g_Param.m_dScaleFactorX;
	m_bImageInverse=g_Config.m_nImageInverse;
	m_nModePeriod  =g_Config.m_nModePeriod;
	m_nComPort     =g_Config.m_nComPort;

	UpdateData(false);

#ifdef USE_EXPOSURE_SERIAL
	GetDlgItem(IDC_STATIC_COMM)			->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_EDIT_COMPORT)		->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_BUTTON_COMPORTSET)	->ShowWindow(SW_SHOW);
#endif

	if(g_System.m_nLineCAM==0 && g_System.m_nImagePart>2)	//투영 투영반사만 사용함.
		GetDlgItem(IDC_BUTTON_TOOSET)		->ShowWindow(SW_SHOW);

#ifdef USE_ROTATION
	GetDlgItem(IDC_SPIN_MOTION)		->ShowWindow(SW_SHOW);
#endif

#ifdef USE_3D_AREA
		GetDlgItem(IDC_BUTTON_ROTATE)	->ShowWindow(SW_SHOW);
	#ifndef USE_ROTATION
		GetDlgItem(IDC_BUTTON_ROTATE)	->SetWindowText(_T("Auto Bright"));
	#endif
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgConfig::OnBnClickedOk()
{
	UpdateData(true);

	//Config값이 달라지면 저장함.-----------------
	if(g_Config.m_nImageType !=m_nImageType			||
//	   g_Config.m_nImageSaveMethod !=m_nSaveMethod	||
	   g_Config.m_nImageInverse != m_bImageInverse	||
	   g_Config.m_nComPort		!= m_nComPort		||
	   g_Config.m_nImageSaveCount !=m_nSaveNumber   ||
	   g_Config.m_nModePeriod   !=m_nModePeriod)
	{
		g_Config.m_nImageType		=m_nImageType;
		g_Config.m_nImageSaveMethod	=m_nSaveMethod;
		g_Config.m_nImageInverse	=m_bImageInverse;
		g_Config.m_nModePeriod      =m_nModePeriod;
		g_Config.m_nComPort			=m_nComPort;
		g_Config.m_nImageSaveCount  =m_nSaveNumber;

		SaveSystemData();
	}
	g_Config.m_nImageSaveMethod	=m_nSaveMethod;
	g_Config.m_nShowProfile=m_bShowProfile;
	g_Config.m_nShowCalibWidth=m_bCalibWidth;
	g_Config.m_dCalibWidth=m_dCalibWidth;
	g_Config.m_dCalibStartX=m_dCalibStartX;
	
	if(m_bCalibWidth)
	{
		g_Param.m_nBrightMax=255;
		g_Param.m_nBrightMin=0;
	}

	if(m_nSaveMethod>0) CossMakeImageDirectoryLot(g_Temp.m_slotName);

	CDialogEx::OnOK();
}


//Frame Number를 강제 지정합니다.
void CDlgConfig::OnBnClickedButtonSetFrameNumber()
{
	UpdateData(true);
	g_Temp.m_nGrabFrame   =m_nFrameNumber;
	g_Temp.m_nInspectFrame=m_nFrameNumber;
}


void CDlgConfig::OnBnClickedButtonSetScale()
{
	UpdateData(true);
	g_Param.m_dScaleFactorX	  =m_dScaleFactorX;
	g_Config.m_dCalibWidth=m_dCalibWidth;
	g_Config.m_dCalibStartX=m_dCalibStartX;
}


void CDlgConfig::OnBnClickedCheckCalibwidth()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlgConfig::OnBnClickedCheckPeriod()
{
	UpdateData(true);

#if GRABBER_TYPE==BASLER_GRABBER
	g_Grabber.SetTriggerMode(g_ID,m_nModePeriod);
#endif

}


void CDlgConfig::OnBnClickedButtonComportset()
{
	UpdateData(true);
	if(m_nComPort>0)
	{
		g_Config.m_nComPort			=m_nComPort;
		//Serial 다시 Open
#ifdef USE_EXPOSURE_SERIAL
		g_Sio.Close();
		g_Sio.InitComm(m_nComPort, 9600, NOPARITY, 8, ONESTOPBIT, true);
#endif

#ifdef USE_ROTATION
		g_SioMotion.Close();
		g_Temp.m_nMotionExist=(int)g_SioMotion.InitComm(m_nComPort, 9600, NOPARITY, 8, ONESTOPBIT, true);
#endif
	}
}


void CDlgConfig::OnBnClickedRadioSaveMethod()
{
}


void CDlgConfig::OnBnClickedRadioSaveMethod2()
{	
	g_Temp.m_nSaveCount	=0;
}


void CDlgConfig::OnBnClickedRadioSaveMethod3()
{	
	g_Temp.m_nSaveCount	=0;
}


void CDlgConfig::OnBnClickedButtonTooset()
{
	if(m_pCameraCalib!=NULL)
	{  
		if(m_pCameraCalib->IsWindowVisible()) return;
		else
		{
			delete m_pCameraCalib;
			m_pCameraCalib=NULL;
		}
	}
	m_pCameraCalib=new CCameraCalib(this);
	m_pCameraCalib->Create(IDD_DIALOG_CAMERA_CALIB, this);
}



void CDlgConfig::OnDeltaposSpinMotion(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
#ifdef USE_ROTATION
	char sMoveData[100];
	if(pNMUpDown->iDelta<0)
	{
		g_Temp.m_dMotionPos+=500;
		sprintf(sMoveData,"1FL%6d\r", 500);		
		g_SioMotion.Write((LPBYTE)sMoveData, strlen(sMoveData)); 
	}
	else
	{

		g_Temp.m_dMotionPos-=500;
		sprintf(sMoveData,"1FL%6d\r", -500);
		g_SioMotion.Write((LPBYTE)sMoveData, strlen(sMoveData)); 
	}
#endif

	*pResult = 0;
}


void CDlgConfig::OnBnClickedButtonRotate()
{
#ifdef USE_ROTATION
	g_Temp.m_nMotionCheck=1;
#else
	g_Temp.m_nAutoAdjustStart=1;
#endif
}


void CDlgConfig::OnBnClickedCheckShowprofile()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
