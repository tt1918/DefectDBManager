// BlobSet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "BlobSet.h"
#include "afxdialogex.h"

#include "KoWebDoc.h"
#include "KoWebView.h"
// CBlobSet 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBlobSet, CDialogEx)

CBlobSet::CBlobSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBlobSet::IDD, pParent)
{
	m_pParent=pParent;
	m_nObject = 0;
}

CBlobSet::~CBlobSet()
{
}

void CBlobSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_BINARY, m_ctrlThreshold);
	DDX_Radio(pDX, IDC_RADIO_BLACK, m_nObject);
	DDX_Control(pDX, IDC_FPSPREAD1, m_Param);
}


BEGIN_MESSAGE_MAP(CBlobSet, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_GRAY, &CBlobSet::OnBnClickedButtonGray)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CBlobSet::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDOK, &CBlobSet::OnBnClickedOk)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_BINARY, &CBlobSet::OnNMCustomdrawSliderBinary)
END_MESSAGE_MAP()

BOOL CBlobSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ctrlThreshold.SetRange(0,255);

	Global2Local();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// CBlobSet 메시지 처리기입니다.

void CBlobSet::SetInspectID(int nSet, int nID)
{
	m_nSet=nSet;
	m_nID =nID;
}

void CBlobSet::OnBnClickedButtonGray()
{	
	((CKoWebView *)m_pParent)->ShowGray(g_Inspect[m_nSet].m_ROI1[m_nID].rc.left, g_Inspect[m_nSet].m_ROI1[m_nID].rc.top, g_Inspect[m_nSet].m_ROI1[m_nID].rc.right, g_Inspect[m_nSet].m_ROI1[m_nID].rc.bottom );
}

void CBlobSet::UpdateSet(bool flag)
{
	if(g_Inspect[m_nSet].m_nROIShow[m_nID]) ((CKoWebView *)m_pParent)->GetTeachROI(m_nSet, m_nID);
	Local2Global();
}
void CBlobSet::OnBnClickedButtonTest()
{
	UpdateSet(true);

	((CKoWebView *)m_pParent)->m_pImage->SetAddGraphic(true);
	((CKoWebView *)m_pParent)->m_pImage->UpdateImage();
	((CKoWebView *)m_pParent)->InspectBlob(g_fmGrab[g_ID], m_nSet, m_nID, true);

	ShowReault();
}
void CBlobSet::ShowReault()
{
	int i;
	CString str;

	for(i=0;i<BLOB_INPECT_ITEM;i++)
	{
		m_Param.SetRow(i+2);

		if(!g_Inspect[m_nSet].m_InspBlob[m_nID].m_nUSE[i])
		{
			m_Param.SetCol(4); m_Param.SetValue(_T(""));
			m_Param.SetCol(5); m_Param.SetValue(_T(""));
			continue;
		}

		m_Param.SetCol(4);  str.Format(_T("%.1lf"), g_InspectResult[m_nID].m_dBlobData[i]); m_Param.SetValue(str);

		m_Param.SetCol(5);
		if(g_InspectResult[m_nID].m_nBlobResult[i]==0) {str.Format(_T("OK")); m_Param.SetValue(str);}
		else									       {str.Format(_T("NG")); m_Param.SetValue(str);}
	}
}


void CBlobSet::OnBnClickedOk()
{
	Local2Global();
	CDialogEx::OnOK();
}


void CBlobSet::Global2Local()
{
	int i, j;
	CString str;
	SetDlgItemInt(IDC_EDIT_THRESHOLD, g_Inspect[m_nSet].m_InspBlob[m_nID].m_nThreshold);
	m_ctrlThreshold.SetPos(g_Inspect[m_nSet].m_InspBlob[m_nID].m_nThreshold);

	if(g_Inspect[m_nSet].m_InspBlob[m_nID].m_nObject==0)
	{
		((CButton *)GetDlgItem(IDC_RADIO_BLACK)) ->SetCheck(1);
		((CButton *)GetDlgItem(IDC_RADIO_WHITE)) ->SetCheck(0);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_RADIO_BLACK)) ->SetCheck(0);
		((CButton *)GetDlgItem(IDC_RADIO_WHITE)) ->SetCheck(1);
	}
	SetDlgItemInt(IDC_EDIT_MINAREA, g_Inspect[m_nSet].m_InspBlob[m_nID].m_nMinArea);

	for(i=0;i<BLOB_INPECT_ITEM;i++)
	{
		m_Param.SetRow(i+2);
		m_Param.SetCol(1); str.Format(_T("%d"),g_Inspect[m_nSet].m_InspBlob[m_nID].m_nUSE[i]); m_Param.SetValue(str);
		m_Param.SetCol(2); str.Format(_T("%.1f"),g_Inspect[m_nSet].m_InspBlob[m_nID].m_dMin[i]); m_Param.SetValue(str);
		m_Param.SetCol(3); str.Format(_T("%.1f"),g_Inspect[m_nSet].m_InspBlob[m_nID].m_dMax[i]); m_Param.SetValue(str);
	}
	
}
void CBlobSet::Local2Global()
{
	int i, j;
	CString str;
	TCHAR sData[20];

	g_Inspect[m_nSet].m_InspBlob[m_nID].m_nThreshold=GetDlgItemInt(IDC_EDIT_THRESHOLD);
	g_Inspect[m_nSet].m_InspBlob[m_nID].m_nObject = ((CButton *)GetDlgItem(IDC_RADIO_WHITE))->GetCheck();
	g_Inspect[m_nSet].m_InspBlob[m_nID].m_nMinArea= GetDlgItemInt(IDC_EDIT_MINAREA);
	
	for(i=0;i<BLOB_INPECT_ITEM;i++)
	{
		m_Param.SetRow(i+2);
		m_Param.SetCol(1); str=m_Param.GetValue(); g_Inspect[m_nSet].m_InspBlob[m_nID].m_nUSE[i]=_ttoi(str);
		m_Param.SetCol(2); str=m_Param.GetValue(); g_Inspect[m_nSet].m_InspBlob[m_nID].m_dMin[i]=_tstof(str);
		m_Param.SetCol(3); str=m_Param.GetValue(); g_Inspect[m_nSet].m_InspBlob[m_nID].m_dMax[i]=_tstof(str);
	}
}



void CBlobSet::OnNMCustomdrawSliderBinary(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;
	int nThreshold=m_ctrlThreshold.GetPos();
	str.Format(_T("%d"), nThreshold);

	GetDlgItem(IDC_EDIT_THRESHOLD)->SetWindowTextW(str);

	((CKoWebView *)m_pParent)->Binary(nThreshold, g_Inspect[m_nSet].m_ROI1[m_nID].rc.left, g_Inspect[m_nSet].m_ROI1[m_nID].rc.top, g_Inspect[m_nSet].m_ROI1[m_nID].rc.right, g_Inspect[m_nSet].m_ROI1[m_nID].rc.bottom);

	*pResult = 0;
}
