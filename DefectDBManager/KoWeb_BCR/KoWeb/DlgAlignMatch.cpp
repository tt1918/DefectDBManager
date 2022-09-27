// DlgAlignMatch.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "DlgAlignMatch.h"
#include "afxdialogex.h"


// CDlgAlignMatch 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgAlignMatch, CDialogEx)

CDlgAlignMatch::CDlgAlignMatch(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAlignMatch::IDD, pParent)
{

	m_nMinScore = 0;
	m_nPatternId = 0;
}

CDlgAlignMatch::~CDlgAlignMatch()
{
}

void CDlgAlignMatch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MINSCORE, m_nMinScore);
	DDX_Text(pDX, IDC_EDIT_PATTERNID, m_nPatternId);
}


BEGIN_MESSAGE_MAP(CDlgAlignMatch, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgAlignMatch::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAlignMatch 메시지 처리기입니다.
void CDlgAlignMatch::SetAlignData(int nSet, int nId)
{
	m_nSet=nSet;
	m_nId=nId;
}

BOOL CDlgAlignMatch::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_nPatternId=g_AlignData[m_nSet].m_nPatternID[m_nId];
	m_nMinScore =g_AlignData[m_nSet].m_nPatternMinScore[m_nId];
	if(m_nMinScore==0) m_nMinScore=50;
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgAlignMatch::OnBnClickedOk()
{
	UpdateData(true);
	g_AlignData[m_nSet].m_nPatternID[m_nId]	    =m_nPatternId;
	g_AlignData[m_nSet].m_nPatternMinScore[m_nId]=m_nMinScore;


	CDialogEx::OnOK();
}
