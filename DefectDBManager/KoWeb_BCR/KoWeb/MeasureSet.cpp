// MeasureSet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "MeasureSet.h"
#include "afxdialogex.h"

#include "KoWebDoc.h"
#include "KoWebView.h"
// CMeasureSet 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMeasureSet, CDialogEx)

CMeasureSet::CMeasureSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMeasureSet::IDD, pParent)
{
	m_pParent=pParent;
	m_dMinDist = 0.0;
	m_dMaxDist = 0.0;
	m_nBaseId = 0;
	m_nPointId = 0;
}

CMeasureSet::~CMeasureSet()
{
}

void CMeasureSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MIN, m_dMinDist);
	DDX_Text(pDX, IDC_EDIT_MAX, m_dMaxDist);
	DDX_Control(pDX, IDC_LIST1, m_ctrlResult);
	DDX_Text(pDX, IDC_EDIT_BASELINE_ID, m_nBaseId);
	DDX_Text(pDX, IDC_EDIT_POINT_ID, m_nPointId);
}


BEGIN_MESSAGE_MAP(CMeasureSet, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CMeasureSet::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDOK, &CMeasureSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CMeasureSet 메시지 처리기입니다.
void CMeasureSet::SetInspectID(int nSet, int nID)
{
	m_nSet=nSet;
	m_nID =nID;
}


BOOL CMeasureSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Global2Local();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMeasureSet::OnBnClickedButtonTest()
{
	CString str;
	int ret;
	if(g_Inspect[m_nSet].m_nROIShow[m_nID]) ((CKoWebView *)m_pParent)->GetTeachROI(m_nSet, m_nID);
	Local2Global();

//	((CKoWebView *)m_pParent)->m_pImage->SetAddGraphic(true);
//	((CKoWebView *)m_pParent)->m_pImage->UpdateImage();
//	((CKoWebView *)m_pParent)->UpdateWindow();

	ret=((CKoWebView *)m_pParent)->InspectMeasure(g_fmGrab[g_ID], m_nSet, m_nID, true);

	m_ctrlResult.ResetContent();

	if(g_InspectResult[m_nID].m_nMeasureResult[0])
	{
		str.Format(_T("[NG] Line Error"));	m_ctrlResult.AddString(str);
	}
	else
	{
		if(g_InspectResult[m_nID].m_nMeasureResult[1])
		{
			str.Format(_T("[NG]DIstance=%.3lf(Pixel)"), g_InspectResult[m_nID].m_dDistance );	m_ctrlResult.AddString(str);
		}
		else
		{
			str.Format(_T("[OK]DIstance=%.3lf(Pixel)"), g_InspectResult[m_nID].m_dDistance );	m_ctrlResult.AddString(str);
		}
	}
}


void CMeasureSet::OnBnClickedOk()
{
	if(g_Inspect[m_nSet].m_nROIShow[m_nID]) ((CKoWebView *)m_pParent)->GetTeachROI(m_nSet, m_nID);
	Local2Global();

	CDialogEx::OnOK();
}


void CMeasureSet::Global2Local()
{
	m_dMinDist = g_Inspect[m_nSet].m_InspMeasure[m_nID].m_dMinDist;
	m_dMaxDist = g_Inspect[m_nSet].m_InspMeasure[m_nID].m_dMaxDist;
	m_nBaseId  = g_Inspect[m_nSet].m_InspMeasure[m_nID].m_nBaseId;
	m_nPointId = g_Inspect[m_nSet].m_InspMeasure[m_nID].m_nPointId;
	UpdateData(false);
}
void CMeasureSet::Local2Global()
{
	UpdateData(true);
	g_Inspect[m_nSet].m_InspMeasure[m_nID].m_dMinDist = m_dMinDist;
	g_Inspect[m_nSet].m_InspMeasure[m_nID].m_dMaxDist = m_dMaxDist;
	g_Inspect[m_nSet].m_InspMeasure[m_nID].m_nBaseId  = m_nBaseId;
	g_Inspect[m_nSet].m_InspMeasure[m_nID].m_nPointId = m_nPointId;
}

