// UserSet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "UserSet.h"
#include "afxdialogex.h"

#include "KoWebDoc.h"
#include "KoWebView.h"
// CUserSet 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUserSet, CDialogEx)

CUserSet::CUserSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserSet::IDD, pParent)
{
	m_pParent=pParent;
	m_nData = 0;
}

CUserSet::~CUserSet()
{
}

void CUserSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrlResult);
	DDX_Text(pDX, IDC_EDIT_NUM, m_nData);
}


BEGIN_MESSAGE_MAP(CUserSet, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CUserSet::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDOK, &CUserSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CUserSet 메시지 처리기입니다.
void CUserSet::SetInspectID(int nSet, int nID)
{
	m_nSet=nSet;
	m_nID =nID;
}


BOOL CUserSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Global2Local();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CUserSet::OnBnClickedButtonTest()
{
	CString str;
	if(g_Inspect[m_nSet].m_nROIShow[m_nID]) ((CKoWebView *)m_pParent)->GetTeachROI(m_nSet, m_nID);
	Local2Global();

	((CKoWebView *)m_pParent)->m_pImage->SetAddGraphic(true);
	((CKoWebView *)m_pParent)->m_pImage->UpdateImage();
	((CKoWebView *)m_pParent)->UpdateWindow();

	((CKoWebView *)m_pParent)->InspectUser(g_fmGrab[g_ID], m_nSet, m_nID, true);

	m_ctrlResult.ResetContent();

	if(g_InspectResult[m_nID].m_nUserResult[0])
	{
		str.Format(_T("[NG] "));	m_ctrlResult.AddString(str);
	}
	else
	{
		str.Format(_T("[OK] "));	m_ctrlResult.AddString(str);
	}
}


void CUserSet::OnBnClickedOk()
{
	if(g_Inspect[m_nSet].m_nROIShow[m_nID]) ((CKoWebView *)m_pParent)->GetTeachROI(m_nSet, m_nID);
	Local2Global();

	CDialogEx::OnOK();
}

void CUserSet::Global2Local()
{
	m_nData = g_Inspect[m_nSet].m_InspUser[m_nID].m_nData;
	
	UpdateData(false);
}
void CUserSet::Local2Global()
{
	UpdateData(true);
	g_Inspect[m_nSet].m_InspUser[m_nID].m_nData=m_nData;
}



