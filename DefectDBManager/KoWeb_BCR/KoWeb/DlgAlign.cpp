// DlgAlign.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "DlgAlign.h"
#include "afxdialogex.h"

#include "KoWebDoc.h"
#include "KoWebView.h"

#include "DlgAlignMatch.h"
#include "DlgAlignEdge.h"
#include "math.h"
// CDlgAlign 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgAlign, CDialogEx)

CDlgAlign::CDlgAlign(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAlign::IDD, pParent)
{
	m_pParent=pParent;
	m_dMaxAngle = 0.0;
	m_dMaxOffsetX = 0.0;
	m_dMaxOffsetY = 0.0;

	m_bShowROIBK[0]=m_bShowROIBK[1]=m_bShowROIBK[2]=m_bShowROIBK[3]=0;
}

CDlgAlign::~CDlgAlign()
{
}

void CDlgAlign::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_ctrlType);
	DDX_Check(pDX, IDC_CHECK_SHOWROI, m_bShowROI);
	DDX_Check(pDX, IDC_CHECK_NOAPPLY, m_nNoApply);
	DDX_Control(pDX, IDC_LIST1, m_ctrlResult);
	DDX_Text(pDX, IDC_EDIT_MAXANGLE, m_dMaxAngle);
	DDX_Text(pDX, IDC_EDIT_MAXOFFSETX, m_dMaxOffsetX);
	DDX_Text(pDX, IDC_EDIT_MAXOFFSETY, m_dMaxOffsetY);
}


BEGIN_MESSAGE_MAP(CDlgAlign, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_SHOWROI, &CDlgAlign::OnBnClickedCheckShowroi)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CDlgAlign::OnCbnSelchangeComboType)
	ON_BN_CLICKED(IDC_BUTTON_DATASET0, &CDlgAlign::OnBnClickedButtonDataset0)
	ON_BN_CLICKED(IDC_BUTTON_DATASET1, &CDlgAlign::OnBnClickedButtonDataset1)
	ON_BN_CLICKED(IDC_BUTTON_DATASET2, &CDlgAlign::OnBnClickedButtonDataset2)
	ON_BN_CLICKED(IDC_BUTTON_DATASET3, &CDlgAlign::OnBnClickedButtonDataset3)
	ON_BN_CLICKED(IDC_BUTTON_FINDPOS, &CDlgAlign::OnBnClickedButtonFindpos)
	ON_BN_CLICKED(IDC_BUTTON_SETALIGN, &CDlgAlign::OnBnClickedButtonSetalign)
	ON_BN_CLICKED(IDOK, &CDlgAlign::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_LOADMASTER, &CDlgAlign::OnBnClickedButtonLoadmaster)
	ON_BN_CLICKED(IDC_BUTTON_SETALIGN2, &CDlgAlign::OnBnClickedButtonSetalign2)
END_MESSAGE_MAP()


// CDlgAlign 메시지 처리기입니다.


BOOL CDlgAlign::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int i;
	CString str;

	for(i=0;i<EDGE_TYPE_NUM;i++)
		m_ctrlType.AddString(g_sType[i]);

	Global2Local();

	if(g_AlignData[m_nSet].m_nRegistered) 
		GetDlgItem(IDC_BUTTON_SETALIGN2)->EnableWindow(true); //Reset Align

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


//메인View에서 카메라가 바뀔때 Align Dialog가 Show일때 함.
void CDlgAlign::UpdateGUI()
{
	if(g_ID==m_nSet) return;
	if(g_ID<0 || g_ID>=MAX_IMAGE) return;

	Local2Global(); //설정된 데이타를 Global로 저장하고
	if(m_bShowROI)  //ROI가 보이면 일단 현재 영역을 Global로 가져온다.
	{
		((CKoWebView *)m_pParent)->SetAlignROI(m_nSet);
		m_bShowROI=false;
		ShowROI();
	}

	m_nSet=g_ID;


	Global2Local(); //Global 데이타를 Local로 읽고
	m_bShowROI=m_bShowROIBK[m_nSet]; //직전 Show
	if(m_bShowROI)
	{
		ShowROI();
	}

	GetDlgItem(IDC_BUTTON_SETALIGN) ->EnableWindow(false); //Set Align 죽이고
	if(g_AlignData[m_nSet].m_nRegistered) 
	{
		GetDlgItem(IDC_BUTTON_SETALIGN2)->EnableWindow(true);  //Reset Align 살리고
	}
	else
	{
		GetDlgItem(IDC_BUTTON_SETALIGN2)->EnableWindow(false);  //Reset Align 살리고
	}
}



void CDlgAlign::SetID(int nSet)
{
	m_nSet=nSet;
}

void CDlgAlign::OnBnClickedCheckShowroi()
{
	UpdateData(true);

	g_AlignData[m_nSet].m_nAlignType=m_ctrlType.GetCurSel();
	if(g_AlignData[m_nSet].m_nAlignType<0 || g_AlignData[m_nSet].m_nAlignType>=EDGE_TYPE_NUM) return;

	ShowROI();
}


void CDlgAlign::OnCbnSelchangeComboType()
{
	int i;
	int N;
	m_bShowROI=0;
	ShowROI();
	UpdateData(false);

	N=m_ctrlType.GetCurSel();
	if(N<0) return;

	if(g_AlignData[m_nSet].m_nAlignType !=N)
	{
		for(i=0;i<4;i++)
			g_AlignData[m_nSet].m_rc[i].left=0;

		g_AlignData[m_nSet].m_nRegistered=0;
	}

	SetSelectAlignType(N);	
}

void CDlgAlign::ShowROI()
{
	if(m_bShowROI)
	{
		((CKoWebView *)m_pParent)->ShowAlignROI(m_nSet, 1);
	}
	else
	{
		((CKoWebView *)m_pParent)->ShowAlignROI(m_nSet, 0);
	}
}


void CDlgAlign::OnBnClickedButtonDataset0()
{	
	CString str;
	GetDlgItem(IDC_ROI_0)->GetWindowTextW(str);
	SetData(0, str);
}


void CDlgAlign::OnBnClickedButtonDataset1()
{
	CString str;
	GetDlgItem(IDC_ROI_1)->GetWindowTextW(str);
	SetData(1, str);
}


void CDlgAlign::OnBnClickedButtonDataset2()
{
	CString str;
	GetDlgItem(IDC_ROI_2)->GetWindowTextW(str);
	SetData(2, str);
}


void CDlgAlign::OnBnClickedButtonDataset3()
{
	CString str;
	GetDlgItem(IDC_ROI_3)->GetWindowTextW(str);
	SetData(3, str);
}

void CDlgAlign::SetData(int N, CString str)
{
	CDlgAlignMatch dlgMatch;
	CDlgAlignEdge  dlgEdge;

	if(str.Find(_T("M"))>=0)
	{
		g_AlignData[m_nSet].m_nROIType[N]=2;
		dlgMatch.SetAlignData(m_nSet, N);
		dlgMatch.DoModal();
	}
	else
	{
		if(str.Find(_T("X"))>=0) g_AlignData[m_nSet].m_nROIType[N]=1;
		else                     g_AlignData[m_nSet].m_nROIType[N]=0;

		dlgEdge.SetAlignData(m_nSet, N);
		dlgEdge.DoModal();
	}
}
void CDlgAlign::SetSelectAlignType(int N)
{
	int i;

	for(i=0;i<g_ROINum[N];i++)
	{
		GetDlgItem(IDC_ROI_0+i)->SetWindowTextW(g_ROIName[N][i]);
		GetDlgItem(IDC_ROI_0+i)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BUTTON_DATASET0+i)->ShowWindow(SW_SHOW);
	}
	for(i=g_ROINum[N];i<4;i++)
	{
		GetDlgItem(IDC_ROI_0+i)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_BUTTON_DATASET0+i)->ShowWindow(SW_HIDE);
	}
	g_AlignData[m_nSet].m_nAlignType=N;
}

void CDlgAlign::Local2Global()
{
	UpdateData(true);
	g_AlignData[m_nSet].m_nNoApply	=m_nNoApply;
	g_AlignData[m_nSet].m_dNGR		=m_dMaxAngle;
	g_AlignData[m_nSet].m_dNGX		=m_dMaxOffsetX;
	g_AlignData[m_nSet].m_dNGY		=m_dMaxOffsetY;
	m_bShowROIBK[m_nSet]			=m_bShowROI;
}

void CDlgAlign::Global2Local()
{	
	m_nNoApply		=g_AlignData[m_nSet].m_nNoApply;
	m_dMaxAngle		=g_AlignData[m_nSet].m_dNGR;
	m_dMaxOffsetX	=g_AlignData[m_nSet].m_dNGX;
	m_dMaxOffsetY	=g_AlignData[m_nSet].m_dNGY;
	m_bShowROI		=m_bShowROIBK[m_nSet];
	SetSelectAlignType(g_AlignData[m_nSet].m_nAlignType);
	m_ctrlType.SetCurSel(g_AlignData[m_nSet].m_nAlignType);
	UpdateData(false);
}



void CDlgAlign::OnBnClickedButtonFindpos()
{
	int i;
	int N, ret;
	double Px, Py;
	CString str;

	Local2Global();
	if(m_bShowROI)
		((CKoWebView *)m_pParent)->SetAlignROI(m_nSet); //ROI영역 가져옴
	
	N=g_AlignData[m_nSet].m_nAlignType;


	//각영역별 위치 검사
	ret=((CKoWebView *)m_pParent)->FindAlignPos(m_nSet);
	
	m_ctrlResult.ResetContent();
	for(i=0;i<g_ROINum[N];i++)  //ROI수만큼
	{
		if(g_AlignData[m_nSet].m_dX[i]>0)
		{
			str.Format(_T("%s: (%.3lf, %.3lf)"), g_ROIName[N][i], g_AlignData[m_nSet].m_dX[i], g_AlignData[m_nSet].m_dY[i]);
			m_ctrlResult.AddString(str);
		}
	}
	if(g_AlignData[m_nSet].m_nRegistered) //Align
	{
		CalAlignData(&g_AlignData[m_nSet]);

		if(fabs(g_AlignData[m_nSet].m_dOffsetX)<=m_dMaxOffsetX && fabs(g_AlignData[m_nSet].m_dOffsetY)<=m_dMaxOffsetY && fabs(g_AlignData[m_nSet].m_dAngleDeg)<=m_dMaxAngle)
		{
			str.Format(_T("[SUCCESS]"));
			m_ctrlResult.AddString(str);
		}
		else
		{
			str.Format(_T("[FAILURE]"));
			m_ctrlResult.AddString(str);
		}

		str.Format(_T("Offset: (%.3lf, %.3lf)"), g_AlignData[m_nSet].m_dOffsetX, g_AlignData[m_nSet].m_dOffsetY);
		m_ctrlResult.AddString(str);
		str.Format(_T("Angle:  %.5lf (deg)"), g_AlignData[m_nSet].m_dAngleDeg);
		m_ctrlResult.AddString(str);

		//3줄 테스트----------------------------------------
		FindNewPoint(&g_AlignData[m_nSet], 0,0,  &Px, &Py);
		str.Format(_T("(0,0)->(%.3lf, %.3lf)"),Px, Py);
		m_ctrlResult.AddString(str);
		//--------------------------------------------------
	}

	if(ret==0 && g_AlignData[m_nSet].m_nRegistered==0) //찾는것 모두 성공
	{
		GetDlgItem(IDC_BUTTON_SETALIGN)->EnableWindow(true);
	}
}

//Set Align버튼 방금 찾은 위치를 기준위치로 등록함. 그때의 영상도 저장함.
void CDlgAlign::OnBnClickedButtonSetalign()
{
	int i;
	CString str;
	Local2Global();

	g_AlignData[m_nSet].m_nRegistered=1;
	for(i=0;i<4;i++)
	{
		g_AlignData[m_nSet].m_dBaseX[i]=g_AlignData[m_nSet].m_dX[i];
		g_AlignData[m_nSet].m_dBaseY[i]=g_AlignData[m_nSet].m_dY[i];
		g_AlignData[m_nSet].bt[i]=g_AlignData[m_nSet].t[i];
		g_AlignData[m_nSet].ba[i]=g_AlignData[m_nSet].a[i];
		g_AlignData[m_nSet].bb[i]=g_AlignData[m_nSet].b[i];
	}
	str.Format(_T("%s%s\\AlignData%d.png"), MODEL_PATH, g_Temp.m_slotName, g_ID);
	((CKoWebView *)m_pParent)->m_pImage->SaveIMG(str);

	GetDlgItem(IDC_BUTTON_SETALIGN)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_SETALIGN2)->EnableWindow(true); //Reset Align
}

//OK 버튼
void CDlgAlign::OnBnClickedOk()
{
	int i,j;
#if MAX_IMAGE==1
	((CKoWebView *)m_pParent)->ShowAlignROI(m_nSet, 0);
#else
	for(i=0;i<MAX_IMAGE;i++)
	{
		for(j=0;j<g_ROINum[g_AlignData[i].m_nAlignType];j++)
		{
			((CKoWebView *)m_pParent)->m_pImageArray[i]->m_pROI->DeleteROI(g_ROIName[g_AlignData[i].m_nAlignType][j]);
			((CKoWebView *)m_pParent)->m_pImageArray[i]->UpdateImage();
		}
	}
#endif

	Local2Global();

	CDialogEx::OnOK();
}

//Master이미지(티칭시 이미지) Load
void CDlgAlign::OnBnClickedButtonLoadmaster()
{
	CString str;
	str.Format(_T("%s%s\\AlignData%d.png"), MODEL_PATH, g_Temp.m_slotName, g_ID);

	((CKoWebView *)m_pParent)->m_pImage->OpenIMG(str);
	memcpy(g_fmGrab[g_ID],  ((CKoWebView *)m_pParent)->m_pImage->m_fmDisplay, g_System.m_nImageW*g_System.m_nImageH);
}

//Reset버튼
void CDlgAlign::OnBnClickedButtonSetalign2()
{
	g_AlignData[m_nSet].m_nRegistered=0;
	GetDlgItem(IDC_BUTTON_SETALIGN2)->EnableWindow(false);
}
