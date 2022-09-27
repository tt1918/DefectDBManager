// DlgAlignEdge.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "DlgAlignEdge.h"
#include "afxdialogex.h"


// CDlgAlignEdge 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgAlignEdge, CDialogEx)

CDlgAlignEdge::CDlgAlignEdge(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgAlignEdge::IDD, pParent)
{

	m_nDir = 0;
	m_nMethod = 0;
	m_nObject = 0;
	m_nSlope = 0;
	m_nGrayTh = 0;
	m_nAveLine = 0;
}

CDlgAlignEdge::~CDlgAlignEdge()
{
}

void CDlgAlignEdge::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_DIR0, m_nDir);
	DDX_Radio(pDX, IDC_RADIO_METHOD0, m_nMethod);
	DDX_Radio(pDX, IDC_RADIO_OBJECT0, m_nObject);
	DDX_Text(pDX, IDC_EDIT_SLOPE, m_nSlope);
	DDX_Text(pDX, IDC_EDIT_GRAYTH, m_nGrayTh);
	DDX_Text(pDX, IDC_EDIT_AVERAGE_LINE, m_nAveLine);
}


BEGIN_MESSAGE_MAP(CDlgAlignEdge, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgAlignEdge::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgAlignEdge 메시지 처리기입니다.
void CDlgAlignEdge::SetAlignData(int nSet, int nId)
{
	m_nSet=nSet;
	m_nId=nId;
}


BOOL CDlgAlignEdge::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if(g_AlignData[m_nSet].m_nROIType[m_nId]==0) //수평선찾기(Y 값)
	{
		GetDlgItem(IDC_RADIO_DIR0)->SetWindowTextW(_T("Top->Bottom"));
		GetDlgItem(IDC_RADIO_DIR1)->SetWindowTextW(_T("Bottom->Top"));
	}
	else //수직선 찾기(X 값)
	{
		GetDlgItem(IDC_RADIO_DIR0)->SetWindowTextW(_T("Left->Right"));
		GetDlgItem(IDC_RADIO_DIR1)->SetWindowTextW(_T("Right->Left"));
	}

	m_nDir		= g_AlignData[m_nSet].m_nDir[m_nId];
	m_nMethod	= g_AlignData[m_nSet].m_nMethod[m_nId];
	m_nObject	= g_AlignData[m_nSet].m_nObject[m_nId];
	m_nSlope	= g_AlignData[m_nSet].m_nSlope[m_nId];
	m_nGrayTh	= g_AlignData[m_nSet].m_nGrayTh[m_nId];
	m_nAveLine	= g_AlignData[m_nSet].m_nAvgLine[m_nId];
	
	if(m_nGrayTh==0)  m_nGrayTh =80;
	if(m_nSlope==0)   m_nSlope  =15;
	if(m_nAveLine==0) m_nAveLine=1;

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgAlignEdge::OnBnClickedOk()
{
	UpdateData(true);

	g_AlignData[m_nSet].m_nDir[m_nId]		= m_nDir	;
	g_AlignData[m_nSet].m_nMethod[m_nId]	= m_nMethod	;
	g_AlignData[m_nSet].m_nObject[m_nId]	= m_nObject	;
	g_AlignData[m_nSet].m_nSlope[m_nId]		= m_nSlope	;
	g_AlignData[m_nSet].m_nGrayTh[m_nId]	= m_nGrayTh	;
	g_AlignData[m_nSet].m_nAvgLine[m_nId]	= m_nAveLine;

	CDialogEx::OnOK();
}
