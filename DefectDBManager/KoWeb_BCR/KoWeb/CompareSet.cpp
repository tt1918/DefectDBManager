// CompareSet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "CompareSet.h"
#include "afxdialogex.h"

#include "KoWebDoc.h"
#include "KoWebView.h"
// CCompareSet 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCompareSet, CDialogEx)

CCompareSet::CCompareSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCompareSet::IDD, pParent)
{
	m_pParent=pParent;
	m_nData = 0;
}

CCompareSet::~CCompareSet()
{
}

void CCompareSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DATA, m_nData);
}


BEGIN_MESSAGE_MAP(CCompareSet, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCompareSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CCompareSet 메시지 처리기입니다.
void CCompareSet::SetInspectID(int nSet, int nID)
{
	m_nSet=nSet;
	m_nID =nID;
}

BOOL CCompareSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Global2Local();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CCompareSet::OnBnClickedOk()
{
	if(g_Inspect[m_nSet].m_nROIShow[m_nID]) ((CKoWebView *)m_pParent)->GetTeachROI(m_nSet, m_nID);
	Local2Global();

	CDialogEx::OnOK();
}

void CCompareSet::Global2Local()
{
	m_nData = g_Inspect[m_nSet].m_InspCompare[m_nID].m_nData;
	
	UpdateData(false);
}
void CCompareSet::Local2Global()
{
	UpdateData(true);
	g_Inspect[m_nSet].m_InspCompare[m_nID].m_nData=m_nData;
}


