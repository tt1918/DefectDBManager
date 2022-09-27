// SpotSet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "SpotSet.h"
#include "afxdialogex.h"

#include "KoWebDoc.h"
#include "KoWebView.h"
#include "SpotSetDontCare.h"
// CSpotSet 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSpotSet, CDialogEx)

CSpotSet::CSpotSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSpotSet::IDD, pParent)
{
	m_pParent=pParent;
}

CSpotSet::~CSpotSet()
{
}

void CSpotSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FPSPREAD1, m_Param);
}


BEGIN_MESSAGE_MAP(CSpotSet, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_DONTCARE, &CSpotSet::OnBnClickedButtonDontcare)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CSpotSet::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDOK, &CSpotSet::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_COMP_4, &CSpotSet::OnBnClickedCheckComp4)
	ON_BN_CLICKED(IDC_CHECK_COMP_0, &CSpotSet::OnBnClickedCheckComp0)
	ON_BN_CLICKED(IDC_CHECK_COMP_1, &CSpotSet::OnBnClickedCheckComp1)
	ON_BN_CLICKED(IDC_CHECK_COMP_2, &CSpotSet::OnBnClickedCheckComp2)
	ON_BN_CLICKED(IDC_CHECK_COMP_3, &CSpotSet::OnBnClickedCheckComp3)
	ON_BN_CLICKED(IDC_RADIO_BUMP, &CSpotSet::OnBnClickedRadioBump)
	ON_BN_CLICKED(IDC_RADIO_INSP_TYPE, &CSpotSet::OnBnClickedRadioInspType)
	ON_BN_CLICKED(IDCANCEL, &CSpotSet::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSpotSet 메시지 처리기입니다.
void CSpotSet::SetInspectID(int nSet, int nID)
{
	m_nSet=nSet;
	m_nID =nID;
}

BOOL CSpotSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Global2Local();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSpotSet::OnBnClickedButtonDontcare()
{
	if(g_Inspect[m_nSet].m_nROIShow[m_nID]) ((CKoWebView *)m_pParent)->GetTeachROI(m_nSet, m_nID);
	if(g_Inspect[m_nSet].m_ROI1[m_nID].rc.right-g_Inspect[m_nSet].m_ROI1[m_nID].rc.left<2 ||
       g_Inspect[m_nSet].m_ROI1[m_nID].rc.bottom-g_Inspect[m_nSet].m_ROI1[m_nID].rc.top<2) 
	{
		AfxMessageBox(_T("Inspect Area Not Set!"));
		return;
	}

	CSpotSetDontCare dlg;
	dlg.SetImageInfo(m_nSet, m_nID, g_fmGrab[g_ID], g_System.m_nPitch);
	dlg.DoModal();
}


void CSpotSet::OnBnClickedButtonTest()
{
	UpdateSet(true);
	
	((CKoWebView *)m_pParent)->m_pImage->SetAddGraphic(true);
	((CKoWebView *)m_pParent)->m_pImage->UpdateImage();
	((CKoWebView *)m_pParent)->InspectSpot(g_fmGrab[g_ID], m_nSet, m_nID, true);
}


void CSpotSet::OnBnClickedOk()
{
	Local2Global();

	CDialogEx::OnOK();
}
void CSpotSet::UpdateSet(bool flag)
{
	if(g_Inspect[m_nSet].m_nROIShow[m_nID]) ((CKoWebView *)m_pParent)->GetTeachROI(m_nSet, m_nID);		//현재 ROI위치읽는다.
	Local2Global();
}

void CSpotSet::Global2Local()
{
	int i, j;
	CString str;
	((CButton *)GetDlgItem(IDC_CHECK_USE_GRAY_RANGE))->SetCheck(g_Inspect[m_nSet].m_InspSpot[m_nID].m_bUseGrayRange);
	SetDlgItemInt(IDC_EDIT_GRAYMIN, g_Inspect[m_nSet].m_InspSpot[m_nID].m_nGrayMin);
	SetDlgItemInt(IDC_EDIT_GRAYMAX, g_Inspect[m_nSet].m_InspSpot[m_nID].m_nGrayMax);
	((CButton *)GetDlgItem(IDC_CHECK_APPLY_AVERGE_FILTER))->SetCheck(g_Inspect[m_nSet].m_InspSpot[m_nID].m_bUseMeanFilter);
	SetDlgItemInt(IDC_EDIT_AVE_FILTER, g_Inspect[m_nSet].m_InspSpot[m_nID].m_nMeanKernel);

	if(g_Inspect[m_nSet].m_InspSpot[m_nID].m_bBumpOnly)
	{
		((CButton *)GetDlgItem(IDC_RADIO_INSP_TYPE)) ->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RADIO_BUMP)) ->SetCheck(1);
		GetDlgItem(IDC_CHECK_COMP_0)->EnableWindow(0);
		GetDlgItem(IDC_CHECK_COMP_1)->EnableWindow(0);
		GetDlgItem(IDC_CHECK_COMP_2)->EnableWindow(0);
		GetDlgItem(IDC_CHECK_COMP_3)->EnableWindow(0);
		GetDlgItem(IDC_CHECK_COMP_4)->EnableWindow(0);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_RADIO_INSP_TYPE)) ->SetCheck(1);
		((CButton *)GetDlgItem(IDC_RADIO_BUMP)) ->SetCheck(0);
	}

	((CButton *)GetDlgItem(IDC_CHECK_COMP_0))->SetCheck(g_Inspect[m_nSet].m_InspSpot[m_nID].m_bDirection[0]);
	((CButton *)GetDlgItem(IDC_CHECK_COMP_1))->SetCheck(g_Inspect[m_nSet].m_InspSpot[m_nID].m_bDirection[1]);
	((CButton *)GetDlgItem(IDC_CHECK_COMP_2))->SetCheck(g_Inspect[m_nSet].m_InspSpot[m_nID].m_bDirection[2]);
	((CButton *)GetDlgItem(IDC_CHECK_COMP_3))->SetCheck(g_Inspect[m_nSet].m_InspSpot[m_nID].m_bDirection[3]);
	((CButton *)GetDlgItem(IDC_CHECK_SHOWBINARY))->SetCheck(g_Inspect[m_nSet].m_InspSpot[m_nID].m_bShowBinary);

	//PARAM사용
	for(j=0;j<10;j++)
	{
		m_Param.SetCol(j+1);
		for(i=0;i<7;i++)
		{
			m_Param.SetRow(i+2);
			if(j==0)		str.Format(_T("%d"), g_Inspect[m_nSet].m_InspSpot[m_nID].m_bUseLevel[i]);
			else if(j==1)   str.Format(_T("%d"), g_Inspect[m_nSet].m_InspSpot[m_nID].m_nThUp[i]);
			else if(j==2)   str.Format(_T("%d"), g_Inspect[m_nSet].m_InspSpot[m_nID].m_nThDn[i]);
			else if(j==3)   str.Format(_T("%d"), g_Inspect[m_nSet].m_InspSpot[m_nID].m_nOffset[i]);
			else if(j==4)   str.Format(_T("%d"), g_Inspect[m_nSet].m_InspSpot[m_nID].m_nWValue[i]);
			else if(j==5)   str.Format(_T("%.3lf"), g_Inspect[m_nSet].m_InspSpot[m_nID].m_dWSizeMin[i]);
			else if(j==6)   str.Format(_T("%.3lf"), g_Inspect[m_nSet].m_InspSpot[m_nID].m_dWSizeMax[i]);
			else if(j==7)   str.Format(_T("%d"), g_Inspect[m_nSet].m_InspSpot[m_nID].m_nBValue[i]);
			else if(j==8)   str.Format(_T("%.3lf"), g_Inspect[m_nSet].m_InspSpot[m_nID].m_dBSizeMin[i]);
			else if(j==9)   str.Format(_T("%.3lf"), g_Inspect[m_nSet].m_InspSpot[m_nID].m_dBSizeMax[i]);
			m_Param.SetValue(str);
		}
	}
}
void CSpotSet::Local2Global()
{
	int i, j;
	CString str;
	TCHAR sData[20];

	g_Inspect[m_nSet].m_InspSpot[m_nID].m_bUseGrayRange	=((CButton *)GetDlgItem(IDC_CHECK_USE_GRAY_RANGE))->GetCheck();
	g_Inspect[m_nSet].m_InspSpot[m_nID].m_nGrayMin		=GetDlgItemInt(IDC_EDIT_GRAYMIN);
	g_Inspect[m_nSet].m_InspSpot[m_nID].m_nGrayMax		=GetDlgItemInt(IDC_EDIT_GRAYMAX);
	g_Inspect[m_nSet].m_InspSpot[m_nID].m_bUseMeanFilter=((CButton *)GetDlgItem(IDC_CHECK_APPLY_AVERGE_FILTER))->GetCheck();
	g_Inspect[m_nSet].m_InspSpot[m_nID].m_nMeanKernel	=GetDlgItemInt(IDC_EDIT_AVE_FILTER);

	g_Inspect[m_nSet].m_InspSpot[m_nID].m_bBumpOnly		=((CButton *)GetDlgItem(IDC_RADIO_BUMP))->GetCheck();
	g_Inspect[m_nSet].m_InspSpot[m_nID].m_bDirection[0]	=((CButton *)GetDlgItem(IDC_CHECK_COMP_0))->GetCheck();
	g_Inspect[m_nSet].m_InspSpot[m_nID].m_bDirection[1]	=((CButton *)GetDlgItem(IDC_CHECK_COMP_1))->GetCheck();
	g_Inspect[m_nSet].m_InspSpot[m_nID].m_bDirection[2]	=((CButton *)GetDlgItem(IDC_CHECK_COMP_2))->GetCheck();
	g_Inspect[m_nSet].m_InspSpot[m_nID].m_bDirection[3]	=((CButton *)GetDlgItem(IDC_CHECK_COMP_3))->GetCheck();
	g_Inspect[m_nSet].m_InspSpot[m_nID].m_bShowBinary	=((CButton *)GetDlgItem(IDC_CHECK_SHOWBINARY))->GetCheck();

	//((CButton *)GetDlgItem(IDC_EDIT_WMIN))->GetWindowTextW(sData, 20); g_Inspect[m_nSet].m_InspSpot[m_nID].m_dWSizeMin=_tstof(sData);
	//((CButton *)GetDlgItem(IDC_EDIT_WMAX))->GetWindowTextW(sData, 20); g_Inspect[m_nSet].m_InspSpot[m_nID].m_dWSizeMax=_tstof(sData);
	//((CButton *)GetDlgItem(IDC_EDIT_BMIN))->GetWindowTextW(sData, 20); g_Inspect[m_nSet].m_InspSpot[m_nID].m_dBSizeMin=_tstof(sData);
	//((CButton *)GetDlgItem(IDC_EDIT_BMAX))->GetWindowTextW(sData, 20); g_Inspect[m_nSet].m_InspSpot[m_nID].m_dBSizeMax=_tstof(sData);

	//PARAM사용
	for(j=0;j<10;j++)
	{
		m_Param.SetCol(j+1);
		for(i=0;i<7;i++)
		{
			m_Param.SetRow(i+2);
			str=m_Param.GetValue();
			if(j==0)		g_Inspect[m_nSet].m_InspSpot[m_nID].m_bUseLevel[i]=_ttoi(str);
			else if(j==1)   g_Inspect[m_nSet].m_InspSpot[m_nID].m_nThUp[i]=_ttoi(str);
			else if(j==2)   g_Inspect[m_nSet].m_InspSpot[m_nID].m_nThDn[i]=_ttoi(str);
			else if(j==3)   g_Inspect[m_nSet].m_InspSpot[m_nID].m_nOffset[i]=_ttoi(str);			
			else if(j==4)   g_Inspect[m_nSet].m_InspSpot[m_nID].m_nWValue[i]=_ttoi(str);
			else if(j==5)   g_Inspect[m_nSet].m_InspSpot[m_nID].m_dWSizeMin[i]=_tstof(str);
			else if(j==6)   g_Inspect[m_nSet].m_InspSpot[m_nID].m_dWSizeMax[i]=_tstof(str);
			else if(j==7)   g_Inspect[m_nSet].m_InspSpot[m_nID].m_nBValue[i]=_ttoi(str);
			else if(j==8)   g_Inspect[m_nSet].m_InspSpot[m_nID].m_dBSizeMin[i]=_tstof(str);
			else if(j==9)   g_Inspect[m_nSet].m_InspSpot[m_nID].m_dBSizeMax[i]=_tstof(str);
		}
	}
}



void CSpotSet::OnBnClickedCheckComp4()
{
	((CButton *)GetDlgItem(IDC_CHECK_COMP_0))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_CHECK_COMP_1))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_CHECK_COMP_2))->SetCheck(0);
	((CButton *)GetDlgItem(IDC_CHECK_COMP_3))->SetCheck(0);
}


void CSpotSet::OnBnClickedCheckComp0()
{
	((CButton *)GetDlgItem(IDC_CHECK_COMP_4))->SetCheck(0);
}


void CSpotSet::OnBnClickedCheckComp1()
{
	((CButton *)GetDlgItem(IDC_CHECK_COMP_4))->SetCheck(0);
}


void CSpotSet::OnBnClickedCheckComp2()
{
	((CButton *)GetDlgItem(IDC_CHECK_COMP_4))->SetCheck(0);
}


void CSpotSet::OnBnClickedCheckComp3()
{
	((CButton *)GetDlgItem(IDC_CHECK_COMP_4))->SetCheck(0);
}


void CSpotSet::OnBnClickedRadioBump()
{
	GetDlgItem(IDC_CHECK_COMP_0)->EnableWindow(0);
	GetDlgItem(IDC_CHECK_COMP_1)->EnableWindow(0);
	GetDlgItem(IDC_CHECK_COMP_2)->EnableWindow(0);
	GetDlgItem(IDC_CHECK_COMP_3)->EnableWindow(0);
	GetDlgItem(IDC_CHECK_COMP_4)->EnableWindow(0);
}


void CSpotSet::OnBnClickedRadioInspType()
{
	GetDlgItem(IDC_CHECK_COMP_0)->EnableWindow(1);
	GetDlgItem(IDC_CHECK_COMP_1)->EnableWindow(1);
	GetDlgItem(IDC_CHECK_COMP_2)->EnableWindow(1);
	GetDlgItem(IDC_CHECK_COMP_3)->EnableWindow(1);
	GetDlgItem(IDC_CHECK_COMP_4)->EnableWindow(1);
}


void CSpotSet::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}
