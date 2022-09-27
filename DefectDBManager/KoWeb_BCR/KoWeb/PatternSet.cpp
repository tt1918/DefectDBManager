// PatternSet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "PatternSet.h"
#include "afxdialogex.h"

#include "KoWebDoc.h"
#include "KoWebView.h"
// CPatternSet 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPatternSet, CDialogEx)

CPatternSet::CPatternSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPatternSet::IDD, pParent)
{
	m_pParent=pParent;
}

CPatternSet::~CPatternSet()
{
}

void CPatternSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrlResult);
}


BEGIN_MESSAGE_MAP(CPatternSet, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CPatternSet::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDOK, &CPatternSet::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CPatternSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Global2Local();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// CPatternSet 메시지 처리기입니다.
void CPatternSet::SetInspectID(int nSet, int nID)
{
	m_nSet=nSet;
	m_nID =nID;
}

void CPatternSet::UpdateSet()
{
	if(g_Inspect[m_nSet].m_nROIShow[m_nID]) ((CKoWebView *)m_pParent)->GetTeachROI(m_nSet, m_nID);
	Local2Global();
}

void CPatternSet::Local2Global()
{
	TCHAR sData[20];

	g_Inspect[m_nSet].m_InspPatt[m_nID].m_nPatternID1  =GetDlgItemInt(IDC_EDIT_PAT_ID1);
	g_Inspect[m_nSet].m_InspPatt[m_nID].m_nPatternID2  =GetDlgItemInt(IDC_EDIT_PAT_ID2);
	
	((CButton *)GetDlgItem(IDC_EDIT_MINSCORE))->GetWindowTextW(sData, 20);		g_Inspect[m_nSet].m_InspPatt[m_nID].m_dMinScore=_tstof(sData);
	((CButton *)GetDlgItem(IDC_EDIT_CENTEROFFSET))->GetWindowTextW(sData, 20);	g_Inspect[m_nSet].m_InspPatt[m_nID].m_dMaxOffset=_tstof(sData);
}

void CPatternSet::Global2Local()
{
	CString str;

	str.Format(_T("%d"), g_Inspect[m_nSet].m_InspPatt[m_nID].m_nPatternID1);
	((CButton *)GetDlgItem(IDC_EDIT_PAT_ID1)) ->SetWindowText(str);
	str.Format(_T("%d"), g_Inspect[m_nSet].m_InspPatt[m_nID].m_nPatternID2);
	((CButton *)GetDlgItem(IDC_EDIT_PAT_ID2)) ->SetWindowText(str);
	
	str.Format(_T("%d"), (int)g_Inspect[m_nSet].m_InspPatt[m_nID].m_dMinScore);
	((CButton *)GetDlgItem(IDC_EDIT_MINSCORE)) ->SetWindowText(str);

	str.Format(_T("%.1f"), g_Inspect[m_nSet].m_InspPatt[m_nID].m_dMaxOffset);
	((CButton *)GetDlgItem(IDC_EDIT_CENTEROFFSET)) ->SetWindowText(str);
}

void CPatternSet::OnBnClickedButtonTest()
{
	CString str;

	UpdateSet();
	
	((CKoWebView *)m_pParent)->m_pImage->SetAddGraphic(true);
	((CKoWebView *)m_pParent)->m_pImage->UpdateImage();
	((CKoWebView *)m_pParent)->InspectPattern(g_fmGrab[g_ID], m_nSet, m_nID, true);

	m_ctrlResult.ResetContent();

	if(g_InspectResult[m_nID].m_nPatternScore==0)
	{
		str.Format(_T("Matching Failure"));				m_ctrlResult.AddString(str);
	}
	else
	{
		if(g_InspectResult[m_nID].m_nPatternResult[0]==0)
		{
			str.Format(_T("[OK] Score : %d"), g_InspectResult[m_nID].m_nPatternScore); m_ctrlResult.AddString(str);
		}
		else
		{
			str.Format(_T("[NG] Score : %d"), g_InspectResult[m_nID].m_nPatternScore); m_ctrlResult.AddString(str);
		}

		if(g_InspectResult[m_nID].m_nPatternResult[1]==0)
		{
			str.Format(_T("[OK] Offset=%.1lf  (%.1lf, %.1lf)"), g_InspectResult[m_nID].m_dPatternOffset, g_InspectResult[m_nID].m_dPatternRy, g_InspectResult[m_nID].m_dPatternRy); m_ctrlResult.AddString(str);
		}
		else
		{
			str.Format(_T("[NG] Offset=%.1lf  (%.1lf, %.1lf)"), g_InspectResult[m_nID].m_dPatternOffset, g_InspectResult[m_nID].m_dPatternRy, g_InspectResult[m_nID].m_dPatternRy); m_ctrlResult.AddString(str);
		}
	}
	
}


void CPatternSet::OnBnClickedOk()
{
	UpdateSet();

	CDialogEx::OnOK();
}
