// DlgDebug.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "DlgDebug.h"
#include "afxdialogex.h"


// CDlgDebug 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgDebug, CDialogEx)

CDlgDebug::CDlgDebug(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgDebug::IDD, pParent)
{

}

CDlgDebug::~CDlgDebug()
{
}

void CDlgDebug::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrlData);
}


BEGIN_MESSAGE_MAP(CDlgDebug, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CDlgDebug::OnBnClickedButtonReset)
END_MESSAGE_MAP()


// CDlgDebug 메시지 처리기입니다.


BOOL CDlgDebug::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString str;

	str.Format(_T("Max Inspect Time:%.1lf (msec)"), g_Temp.m_dMaxInspTime); m_ctrlData.AddString(str);
	str.Format(_T("Min Inspect Time:%.1lf (msec)"), g_Temp.m_dMinInspTime); m_ctrlData.AddString(str);
	str.Format(_T("Send Frame Diff: %d (2=>%d 3=>%d)"), g_Temp.m_nFrameDiff, g_Temp.m_nFrameDiffSum[0], g_Temp.m_nFrameDiffSum[1]); m_ctrlData.AddString(str);

#if PROGRAM_TYPE==WEB_DAKANO
	str.Format(_T("DK Frame Diff: %d Send Diff: %d"), g_Temp.m_nMapLineCntDiff, g_Temp.m_nLRSendFrameDiff); m_ctrlData.AddString(str);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgDebug::OnBnClickedButtonReset()
{
	g_Temp.m_dMaxInspTime=0;
	g_Temp.m_dMinInspTime=10000;

#if PROGRAM_TYPE==WEB_DAKANO
	LoadDKOffset();
#endif

}
