// ROIAngle.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "ROIAngle.h"
#include "afxdialogex.h"
 

// CROIAngle 대화 상자입니다.

IMPLEMENT_DYNAMIC(CROIAngle, CDialogEx)

CROIAngle::CROIAngle(CWnd* pParent /*=NULL*/)
	: CDialogEx(CROIAngle::IDD, pParent)
{

}

CROIAngle::~CROIAngle()
{
}

void CROIAngle::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CROIAngle, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ZERO, &CROIAngle::OnBnClickedButtonZero)
	ON_BN_CLICKED(IDC_BUTTON_GRAY, &CROIAngle::OnBnClickedButtonGray)
	ON_BN_CLICKED(IDC_CHECK_AUTO, &CROIAngle::OnBnClickedCheckAuto)
	ON_BN_CLICKED(IDC_CHECK_REVERSE, &CROIAngle::OnBnClickedCheckReverse)
	ON_EN_CHANGE(IDC_EDIT_ANGLE, &CROIAngle::OnEnChangeEditAngle)
END_MESSAGE_MAP()


void CROIAngle::Binary_OnlyDisplay()
{
	LPBYTE fmS=((CDispImage*)m_pParent)->m_fmProcess;
	LPBYTE fmD=((CDispImage*)m_pParent)->m_fmDisplay;
	if(fmS)
	{
		m_pROI->NV_BinaryDisplay(fmS, fmD, m_pROI->m_Tracker[m_nROINumber]);
		((CDispImage*)m_pParent)->UpdateImage();
	}
}

// CROIAngle 메시지 처리기입니다.

BOOL CROIAngle::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_nAngle=m_pROI->GetROIAngle(m_nROINumber);
	m_pROI->GetROIThreshold(m_nROINumber, &m_bAutoThreshold, &m_bInverse, &m_nThreshold, &m_nThresholdL);
	m_nThresholdOld=m_nThreshold;
	m_nThresholdLOld=m_nThresholdL;

	m_ctrlAngle.SetRange(0,180,false);
	m_ctrlAngle.SetPos(m_nAngle+90);

	m_ctrlThreshold.SetRange(0,255,false);
	m_ctrlThreshold.SetPos(m_nThreshold);

	m_ctrlThresholdL.SetRange(0,255,false);
	m_ctrlThresholdL.SetPos(m_nThresholdL);

	UpdateData(false);

	if(m_pROI->m_Tracker[m_nROINumber].full)
	{
		GetDlgItem(IDC_BUTTON_ZERO)->EnableWindow(false);
		GetDlgItem(IDC_SLIDER1)->EnableWindow(false);
	}

	int nWidth=m_pROI->m_Tracker[m_nROINumber].rect.right-m_pROI->m_Tracker[m_nROINumber].rect.left;
	int nHeight=m_pROI->m_Tracker[m_nROINumber].rect.bottom-m_pROI->m_Tracker[m_nROINumber].rect.top;
	CString str;
	str.Format(_T("ROI:%s   폭:%d  높이:%d"), m_pROI->m_TrackerName[m_nROINumber], nWidth,nHeight);
	SetWindowText(str);

	CRect rc;
	GetWindowRect(&rc);
	rc.right=(rc.right-rc.left);
	rc.bottom=(rc.bottom-rc.top);
	rc.left=0;
	rc.top=0;
	MoveWindow(rc,true);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CROIAngle::OnCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_nAngle=m_ctrlAngle.GetPos()-90;

	m_pROI->SetROIAngle(m_nROINumber, m_nAngle);
	UpdateData(false);
	
	((CDispImage*)m_pParent)->UpdateImage();

	*pResult = 0;
}

void CROIAngle::OnBnClickedButtonZero()
{
	m_nAngle=0;
	m_ctrlAngle.SetPos(m_nAngle+90);

	m_pROI->SetROIAngle(m_nROINumber, m_nAngle);
	UpdateData(false);

	((CDispImage*)m_pParent)->UpdateImage();
}


void CROIAngle::OnBnClickedButtonGray()
{
	LPBYTE fmS=((CDispImage*)m_pParent)->m_fmProcess;
	LPBYTE fmD=((CDispImage*)m_pParent)->m_fmDisplay;
	if(fmS)
	{
		m_pROI->NV_GrayDisplay(fmS, fmD, m_pROI->m_Tracker[m_nROINumber]);
		((CDispImage*)m_pParent)->UpdateImage();
	}
}




void CROIAngle::OnCustomdrawSlider2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_pROI->SetROIThreshold(m_nROINumber, m_bAutoThreshold, m_bInverse, m_nThreshold, m_nThresholdL);
	m_nThreshold=m_ctrlThreshold.GetPos();
	if(m_nThreshold!=m_nThresholdOld)
	{
		UpdateData(false);
		Binary_OnlyDisplay();
		m_nThresholdOld=m_nThreshold;
	}
	*pResult = 0;
}
void CROIAngle::OnReleasedcaptureSlider2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_bAutoThreshold=false;
	if(m_nThresholdL>=m_nThreshold)
	{
		m_nThreshold=m_nThresholdL;
		m_ctrlThreshold.SetPos(m_nThreshold);
	}

	UpdateData(false);
	m_pROI->SetROIThreshold(m_nROINumber, m_bAutoThreshold,m_bInverse, m_nThreshold, m_nThresholdL);
	Binary_OnlyDisplay();
	*pResult = 0;
}


void CROIAngle::OnCustomdrawSlider3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_nThresholdL=m_ctrlThresholdL.GetPos();


	m_pROI->SetROIThreshold(m_nROINumber, m_bAutoThreshold, m_bInverse, m_nThreshold, m_nThresholdL);
	if(m_nThresholdL!=m_nThresholdLOld)
	{
		UpdateData(false);
		Binary_OnlyDisplay();
		m_nThresholdLOld=m_nThresholdL;
	}
	*pResult = 0;
}

void CROIAngle::OnReleasedcaptureSlider3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_nThresholdL=m_ctrlThresholdL.GetPos();
	if(m_nThresholdL>=m_nThreshold)
	{
		m_nThresholdL=m_nThreshold;
		m_ctrlThresholdL.SetPos(m_nThresholdL);
	}
	m_bAutoThreshold=false;
	UpdateData(false);
	m_pROI->SetROIThreshold(m_nROINumber, m_bAutoThreshold,m_bInverse, m_nThreshold, m_nThresholdL);
	Binary_OnlyDisplay();
	*pResult = 0;
}


void CROIAngle::OnBnClickedCheckAuto()
{
	UpdateData(true);	
	LPBYTE fmS=((CDispImage*)m_pParent)->m_fmProcess;

	if(fmS)
	{
		if(m_bAutoThreshold)
		{
			m_pROI->NV_AutoThreshold(m_bInverse, fmS,m_pROI->m_Tracker[m_nROINumber], &m_nThreshold, &m_nThresholdL);
			m_ctrlThreshold.SetPos(m_nThreshold);
			m_ctrlThresholdL.SetPos(m_nThresholdL);
		}
		else
		{
			m_nThresholdL=m_ctrlThresholdL.GetPos();
		}		

		m_pROI->SetROIThreshold(m_nROINumber, m_bAutoThreshold, m_bInverse, m_nThreshold, m_nThresholdL);
		UpdateData(false);

		Binary_OnlyDisplay();
	}
}


void CROIAngle::OnBnClickedCheckReverse()
{
	UpdateData(true);	
	m_pROI->SetROIThreshold(m_nROINumber, m_bAutoThreshold, m_bInverse, m_nThreshold, m_nThresholdL);
	Binary_OnlyDisplay();	
}


void CROIAngle::OnEnChangeEditAngle()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
