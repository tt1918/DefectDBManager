// DlgThreshold.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "DlgThreshold.h"
#include "afxdialogex.h"
#include "KoWebDoc.h"
#include "KoWebView.h"

// CDlgThreshold 대화 상자입니다.
CString l_sFlat[4]={_T("LINESCAN"), _T("AREA 2"), _T("AREA 8"), _T("AREA 25") };


IMPLEMENT_DYNAMIC(CDlgThreshold, CDialogEx)

CDlgThreshold::CDlgThreshold(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgThreshold::IDD, pParent)
{
	m_pWnd=pParent;
	m_nThreshold = 0;
	m_nThSub = 0;
	m_dAngle = 0.0;
	m_bCheckBinary=false;
	m_nElongX = 0;
	m_nElongY = 0;
	m_nEmpX = 30;
	m_nEmpY = 30;
	m_nBaseGray = 80;
	m_dData1 = 4;
	m_dData2 = 50;
	m_dData3 = 45;
	m_dData4 = 4.5;
	m_nNoiseTh = 15;
	m_nCutHigh = 128;
	m_nCutLow = 0;
	m_nOffset = 20;
	m_nOffsetValue = 5;
	m_bWaterPrint=0;
	m_nRemoveKernel = 3;
	m_nMinYLength = 1;
}

CDlgThreshold::~CDlgThreshold()
{
}

void CDlgThreshold::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_ctrlBinary);
	DDX_Text(pDX, IDC_EDIT_TH, m_nThreshold);
	DDX_Text(pDX, IDC_EDIT_TH2, m_nThSub);
	DDX_Text(pDX, IDC_EDIT_ANGLE, m_dAngle);
	DDX_Check(pDX, IDC_CHECK_BINARY, m_bCheckBinary);
	DDX_Text(pDX, IDC_EDIT_ENLONGX, m_nElongX);
	DDX_Text(pDX, IDC_EDIT_ENLONGY, m_nElongY);
	DDX_Text(pDX, IDC_EDIT_EMP_X, m_nEmpX);
	DDX_Text(pDX, IDC_EDIT_EMP_Y, m_nEmpY);
	DDX_Control(pDX, IDC_LIST_INFO, m_ctrlImageInfo);
	DDX_Check(pDX, IDC_CHECK_FULLIMAGE, m_bFullImage);
	DDX_Control(pDX, IDC_COMBO_FLAT, m_ctrlFlat);
	DDX_Text(pDX, IDC_EDIT_BASEGRAY, m_nBaseGray);
	DDX_Text(pDX, IDC_EDIT_DATA1, m_dData1);
	DDX_Text(pDX, IDC_EDIT_DATA2, m_dData2);
	DDX_Text(pDX, IDC_EDIT_DATA3, m_dData3);
	DDX_Text(pDX, IDC_EDIT_DATA4, m_dData4);
	DDX_Text(pDX, IDC_EDIT_NOISE_TH, m_nNoiseTh);
	DDX_Text(pDX, IDC_EDIT_CUT_HIGH, m_nCutHigh);
	DDV_MinMaxInt(pDX, m_nCutHigh, 0, 255);
	DDX_Text(pDX, IDC_EDIT_CUT_LOW, m_nCutLow);
	DDV_MinMaxInt(pDX, m_nCutLow, 0, 255);
	DDX_Text(pDX, IDC_EDIT_OFFSET, m_nOffset);
	DDV_MinMaxInt(pDX, m_nOffset, 1, 100);
	DDX_Text(pDX, IDC_EDIT_OFFSETVALUE, m_nOffsetValue);
	DDV_MinMaxInt(pDX, m_nOffsetValue, 1, 255);
	DDX_Check(pDX, IDC_CHECK_WATERPRINT, m_bWaterPrint);
	DDX_Text(pDX, IDC_EDIT_CUT_KERNEL, m_nRemoveKernel);
	DDV_MinMaxInt(pDX, m_nRemoveKernel, 0, 20);
	DDX_Text(pDX, IDC_EDIT_MIN_Y_LENGTH, m_nMinYLength);
	DDV_MinMaxInt(pDX, m_nMinYLength, 1, 20);
}


BEGIN_MESSAGE_MAP(CDlgThreshold, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgThreshold::OnBnClickedOk)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CDlgThreshold::OnNMCustomdrawSlider1)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_GRAY, &CDlgThreshold::OnBnClickedButtonShowGray)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE, &CDlgThreshold::OnBnClickedButtonRotate)
	ON_BN_CLICKED(IDC_CHECK_BINARY, &CDlgThreshold::OnBnClickedCheckBinary)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CDlgThreshold::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_FLIP_HOR, &CDlgThreshold::OnBnClickedButtonFlipHor)
	ON_BN_CLICKED(IDC_BUTTON_FLIP_VER, &CDlgThreshold::OnBnClickedButtonFlipVer)
	ON_BN_CLICKED(IDC_BUTTON_ENLONG, &CDlgThreshold::OnBnClickedButtonElong)
	ON_BN_CLICKED(IDC_BUTTON_INVERSE, &CDlgThreshold::OnBnClickedButtonInverse)
	ON_BN_CLICKED(IDC_BUTTON_SMOOTHING, &CDlgThreshold::OnBnClickedButtonSmoothing)
	ON_BN_CLICKED(IDC_BUTTON_EMPH, &CDlgThreshold::OnBnClickedButtonEmph)
	ON_BN_CLICKED(IDC_CHECK_FULLIMAGE, &CDlgThreshold::OnBnClickedCheckFullimage)
	ON_BN_CLICKED(IDC_BUTTON_FLAT, &CDlgThreshold::OnBnClickedButtonFlat)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDlgThreshold::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CDlgThreshold::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_REMOVENOISE, &CDlgThreshold::OnBnClickedButtonRemovenoise)
	ON_BN_CLICKED(IDC_BUTTON_TEST2, &CDlgThreshold::OnBnClickedButtonTest2)
	ON_BN_CLICKED(IDC_BUTTON_TEST3, &CDlgThreshold::OnBnClickedButtonTest3)
	ON_BN_CLICKED(IDC_BUTTON_CUT_LOWHIGH, &CDlgThreshold::OnBnClickedButtonCutLowhigh)
	ON_BN_CLICKED(IDC_CHECK_WATERPRINT, &CDlgThreshold::OnBnClickedCheckWaterprint)
	ON_BN_CLICKED(IDC_BUTTON_SMOOTHING2, &CDlgThreshold::OnBnClickedButtonSmoothing2)
END_MESSAGE_MAP()


// CDlgThreshold 메시지 처리기입니다.
BOOL CDlgThreshold::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int i;
	m_ctrlBinary.SetRange(0,255);

	m_nBaseGray=g_System.m_nFlatBright;

	if(g_Temp.m_bShowMap)
		memcpy(g_fmTemp, ((CKoWebView *)m_pWnd)->m_pImage->m_fmDisplay, g_System.m_nImageW*g_System.m_nImageH);
	else
		memcpy(g_fmTemp, g_fmGrab[g_ID], g_System.m_nImageW*g_System.m_nImageH);

	for(i=0;i<4;i++)
	{
		m_ctrlFlat.AddString(l_sFlat[i]);

	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgThreshold::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CKoWebView *)m_pWnd)->CloseBinary(m_bCheckBinary);
	CDialogEx::OnOK();
}


void CDlgThreshold::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_nThreshold=m_ctrlBinary.GetPos();
	m_nThSub=m_nThreshold-m_nAve;
	UpdateData(false);

	if(m_bCheckBinary) 
		((CKoWebView *)m_pWnd)->Binary(m_nThreshold);

	*pResult = 0;
}


void CDlgThreshold::SetImageInfo(double dAve, double dStdev, int nMax, int nMin, double dSharp, int nSizeX, int nSizeY, int nWaterPrintValue, double dWaterValue)
{
	CString str;
	
	m_ctrlImageInfo.ResetContent();
	m_nAve=(int)dAve;

	str.Format(_T("Average : %.1lf  Max:%d  Min:%d"), dAve, nMax, nMin);
	m_ctrlImageInfo.AddString(str);

	str.Format(_T("Stdev: %.2lf Sharpness:%.3lf"), dStdev, dSharp);
	m_ctrlImageInfo.AddString(str);

	str.Format(_T("ROI Size : %d x %d"), nSizeX, nSizeY);
	m_ctrlImageInfo.AddString(str);

	if(m_bWaterPrint && nWaterPrintValue>=0)
	{
		str.Format(_T("물때Value : %d  AVG: %.2lf"), nWaterPrintValue, dWaterValue);
		m_ctrlImageInfo.AddString(str);
	}

	m_ctrlBinary.SetPos(m_nAve);
}

void CDlgThreshold::OnBnClickedButtonShowGray()
{
	((CKoWebView *)m_pWnd)->ShowOriginal();
}


void CDlgThreshold::OnBnClickedButtonRotate()
{
	UpdateData(true);
	((CKoWebView *)m_pWnd)->RotateImage(m_dAngle);
}

void CDlgThreshold::OnBnClickedButtonSmoothing()
{
	((CKoWebView *)m_pWnd)->SmoothingImage();
}

void CDlgThreshold::OnBnClickedButtonEmph()
{
	UpdateData(true);
	double start, end;
	CString str;
	start=Clock_set();

	((CKoWebView *)m_pWnd)->EmphasizeImage(m_nEmpX, m_nEmpY, 5);

	end=Clock_set();
	str.Format(_T("Elapse Time=%.3lf msec"), end-start);
	m_ctrlImageInfo.ResetContent();
	m_ctrlImageInfo.AddString(str);
}


void CDlgThreshold::OnBnClickedCheckBinary()
{
	UpdateData(true);
	if(m_bCheckBinary)
	{
		m_nThreshold=m_ctrlBinary.GetPos();
		((CKoWebView *)m_pWnd)->Binary(m_nThreshold);
	}
	else
	{
		((CKoWebView *)m_pWnd)->ShowApplyImage();
	}
}



void CDlgThreshold::OnBnClickedButtonFlipHor()
{
	((CKoWebView *)m_pWnd)->FlipHor();
}


void CDlgThreshold::OnBnClickedButtonFlipVer()
{
	((CKoWebView *)m_pWnd)->FlipVer();
}


void CDlgThreshold::OnBnClickedButtonElong()
{
	UpdateData(true);
	
	double start, end;
	CString str;
	start=Clock_set();

	((CKoWebView *)m_pWnd)->ImageElongation(m_nElongX, m_nElongY);

	end=Clock_set();
	str.Format(_T("Elapse Time=%.3lf msec"), end-start);
	m_ctrlImageInfo.ResetContent();
	m_ctrlImageInfo.AddString(str);
}
void CDlgThreshold::OnBnClickedButtonInverse()
{
	((CKoWebView *)m_pWnd)->InverseImage();
}


void CDlgThreshold::OnBnClickedButtonApply()
{
	((CKoWebView *)m_pWnd)->AppplyImage();
}







void CDlgThreshold::OnBnClickedCheckFullimage()
{
	UpdateData(true);
	GetDlgItem(IDC_COMBO_FLAT)->EnableWindow(m_bFullImage);

	((CKoWebView *)m_pWnd)->ROIFull(m_bFullImage);
}


void CDlgThreshold::OnBnClickedButtonFlat()
{
	UpdateData(true);

	int nType=m_ctrlFlat.GetCurSel();
	double start, end;
	CString str;
	start=Clock_set();

	if(m_bFullImage)
	{
		if(nType<0) return;

		((CKoWebView *)m_pWnd)->FlatImageFull(nType, m_nBaseGray, 0, g_System.m_nImageW);
	}
	else  //ROI 이미지에서 Flat
	{
		((CKoWebView *)m_pWnd)->FlatImageROI(m_nBaseGray);
	}

	end=Clock_set();
	str.Format(_T("Elapse Time=%.3lf msec"), end-start);
	m_ctrlImageInfo.ResetContent();
	m_ctrlImageInfo.AddString(str);
}


void CDlgThreshold::OnBnClickedButtonSave()
{
	int ll,tt,rr,bb;
	CKoWebView *pView=(CKoWebView *)m_pWnd;

	CFileDialog dlg(FALSE, _T("bmp"), _T("Image"));
		if(dlg.DoModal() !=IDOK) return;

	pView->m_pImage->m_pROI->GetROIRegion(_T("t"), &ll, &tt, &rr, &bb);
	pView->m_pImage->SaveROIImage(dlg.GetPathName(), pView->m_pImage->m_fmDisplay, ll,tt,rr-1,bb-1, g_System.m_nPitch, 0);
}

extern  BYTE fmPitGray[128*128];
extern  BYTE fmPitBin[128*128];
void CDlgThreshold::OnBnClickedButtonTest()
{
	UpdateData(true);
	CString str;
	int i,j,ll,tt,rr,bb;
	double start, end, dValue, dCloudValue;
	int n2ndSize, nCoreSize, nCoreValue;
	int CloudSize;
	CKoWebView *pView=(CKoWebView *)m_pWnd;

// HTO Size------------------
	start=Clock_set();

	((CKoWebView *)m_pWnd)->FindHBOSize(m_dData1, (int)m_dData2, m_dData3, m_dData4, &nCoreValue, &nCoreSize, &dCloudValue, &CloudSize);

	end=Clock_set();
	m_ctrlImageInfo.ResetContent();
	str.Format(_T("Elapse Time=%.3lf msec"), end-start);
	m_ctrlImageInfo.AddString(str);
	str.Format(_T("Core Size=%d Value=%d"), nCoreSize, nCoreValue);
	m_ctrlImageInfo.AddString(str);
	str.Format(_T("Cloud Value=%.1lf, Size=%d,  %.3lf"), dCloudValue, CloudSize, CloudSize*0.035);
	m_ctrlImageInfo.AddString(str);

	/*
	//PIT 구하기
	pView->m_pImage->m_pROI->GetROIRegion(_T("t"), &ll, &tt, &rr, &bb);

	start=Clock_set();
	dValue=GetPitValue(pView->m_pImage->m_fmDisplay, (ll+rr)/2-32, (tt+bb)/2-32, 64,64, g_System.m_nPitch, (int)m_dData3);
	end=Clock_set();
	m_ctrlImageInfo.ResetContent();
	str.Format(_T("Elapse Time=%.3lf msec"), end-start);
	m_ctrlImageInfo.AddString(str);
	str.Format(_T("Value=%.3lf"), dValue);
	m_ctrlImageInfo.AddString(str);

	for(i=0;i<128;i++)
		for(j=0;j<128;j++)
			*(pView->m_pImage->m_fmDisplay+g_System.m_nPitch*(tt+i)+(ll+j))=*(fmPitGray+128*i+j);
	for(i=0;i<128;i++)
		for(j=0;j<128;j++)
			*(pView->m_pImage->m_fmDisplay+g_System.m_nPitch*(tt+i+128)+(ll+j))=*(fmPitBin+128*i+j);
	pView->m_pImage->UpdateImage();
	*/
}





void CDlgThreshold::OnBnClickedButtonRemovenoise()
{
	UpdateData(true);
	CKoWebView *pView=(CKoWebView *)m_pWnd;
	pView->RemoveHorNoise(m_nNoiseTh);
}


void CDlgThreshold::OnBnClickedButtonTest2()
{
	CKoWebView *pView=(CKoWebView *)m_pWnd;
	double start, end;
	CString str;
	start=Clock_set();

	pView->MakeSeparatemage();

	end=Clock_set();
	str.Format(_T("Elapse Time=%.3lf msec"), end-start);
	m_ctrlImageInfo.ResetContent();
	m_ctrlImageInfo.AddString(str);
}


void CDlgThreshold::OnBnClickedButtonTest3()
{
	CKoWebView *pView=(CKoWebView *)m_pWnd;
	pView->MakePolaImage();
}


void CDlgThreshold::OnBnClickedButtonCutLowhigh()
{
	UpdateData(true);
	double start, end;
	CString str;
	start=Clock_set();

	((CKoWebView *)m_pWnd)->RemoveLowHigh(m_nCutLow, m_nCutHigh, m_nRemoveKernel);

	end=Clock_set();
	str.Format(_T("Elapse Time=%.3lf msec"), end-start);
	m_ctrlImageInfo.ResetContent();
	m_ctrlImageInfo.AddString(str);
	((CKoWebView *)m_pWnd)->AppplyImage();
}


void CDlgThreshold::OnBnClickedCheckWaterprint()
{
	UpdateData(true);
}


void CDlgThreshold::OnBnClickedButtonSmoothing2()
{
		((CKoWebView *)m_pWnd)->ErodeImage();
}
