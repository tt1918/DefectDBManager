// FindLineSet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "FindLineSet.h"
#include "afxdialogex.h"

#include "KoWebDoc.h"
#include "KoWebView.h"
// CFindLineSet 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFindLineSet, CDialogEx)

CFindLineSet::CFindLineSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFindLineSet::IDD, pParent)
{
	m_pParent=pParent;
	m_nDir = 0;
	m_nLineType = 0;
	m_nMethod = 0;
	m_nObject = 0;
	m_nGrayTh = 0;
	m_nSlopeTh = 0;
	m_nAvgLine = 0;
	m_dMinAngle = 0.0;
	m_dMaxAngle = 0.0;
}

CFindLineSet::~CFindLineSet()
{
}

void CFindLineSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_DIR, m_nDir);
	DDX_Radio(pDX, IDC_RADIO_LINETYPE, m_nLineType);
	DDX_Radio(pDX, IDC_RADIO_METHOD, m_nMethod);
	DDX_Radio(pDX, IDC_RADIO_OBJECT, m_nObject);
	DDX_Control(pDX, IDC_LIST2, m_ctrlResult);
	DDX_Text(pDX, IDC_EDIT_GRAYTH, m_nGrayTh);
	DDX_Text(pDX, IDC_EDIT_SLOPETH, m_nSlopeTh);
	DDX_Text(pDX, IDC_EDIT_AVERAGELINE, m_nAvgLine);
	DDX_Text(pDX, IDC_EDIT_ANGLEMIN, m_dMinAngle);
	DDX_Text(pDX, IDC_EDIT_ANGLEMAX, m_dMaxAngle);
	DDX_Check(pDX, IDC_CHECK_DIAGONAL, m_bDiagonal);
}


BEGIN_MESSAGE_MAP(CFindLineSet, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CFindLineSet::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDOK, &CFindLineSet::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_LINETYPE, &CFindLineSet::OnBnClickedRadioLinetype)
	ON_BN_CLICKED(IDC_RADIO_LINETYPE2, &CFindLineSet::OnBnClickedRadioLinetype2)
END_MESSAGE_MAP()


// CFindLineSet 메시지 처리기입니다.
void CFindLineSet::SetInspectID(int nSet, int nID)
{
	m_nSet=nSet;
	m_nID =nID;
}


BOOL CFindLineSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Global2Local();
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CFindLineSet::Global2Local()
{
	m_nLineType = g_Inspect[m_nSet].m_InspFindLine[m_nID].m_nLineType;
	m_nDir		= g_Inspect[m_nSet].m_InspFindLine[m_nID].m_nDir;
	m_bDiagonal = g_Inspect[m_nSet].m_InspFindLine[m_nID].m_ndiagonal;
	m_nMethod	= g_Inspect[m_nSet].m_InspFindLine[m_nID].m_nMethod;
	m_nObject	= g_Inspect[m_nSet].m_InspFindLine[m_nID].m_nObject;
	m_nGrayTh	= g_Inspect[m_nSet].m_InspFindLine[m_nID].m_nGrayTh;
	m_nSlopeTh	= g_Inspect[m_nSet].m_InspFindLine[m_nID].m_nSlopeTh;
	m_nAvgLine	= g_Inspect[m_nSet].m_InspFindLine[m_nID].m_nAvgLine;
	m_dMinAngle = g_Inspect[m_nSet].m_InspFindLine[m_nID].m_dMinAngle;
	m_dMaxAngle = g_Inspect[m_nSet].m_InspFindLine[m_nID].m_dMaxAngle;

	if(m_nLineType==0)
	{
		GetDlgItem(IDC_RADIO_DIR)->SetWindowTextW(_T("Top->Bottom"));
		GetDlgItem(IDC_RADIO_DIR2)->SetWindowTextW(_T("Bottom->Top"));
	}
	else
	{
		GetDlgItem(IDC_RADIO_DIR)->SetWindowTextW(_T("Left->Right"));
		GetDlgItem(IDC_RADIO_DIR2)->SetWindowTextW(_T("Right->Left"));
	}
}

void CFindLineSet::Local2Global()
{
	UpdateData(true);
	g_Inspect[m_nSet].m_InspFindLine[m_nID].m_nLineType = m_nLineType;
	g_Inspect[m_nSet].m_InspFindLine[m_nID].m_nDir		= m_nDir;
	g_Inspect[m_nSet].m_InspFindLine[m_nID].m_ndiagonal = m_bDiagonal;
	g_Inspect[m_nSet].m_InspFindLine[m_nID].m_nMethod	= m_nMethod;
	g_Inspect[m_nSet].m_InspFindLine[m_nID].m_nObject	= m_nObject;
	g_Inspect[m_nSet].m_InspFindLine[m_nID].m_nGrayTh	= m_nGrayTh;
	g_Inspect[m_nSet].m_InspFindLine[m_nID].m_nSlopeTh	= m_nSlopeTh;
	g_Inspect[m_nSet].m_InspFindLine[m_nID].m_nAvgLine	= m_nAvgLine;
	g_Inspect[m_nSet].m_InspFindLine[m_nID].m_dMinAngle = m_dMinAngle;
	g_Inspect[m_nSet].m_InspFindLine[m_nID].m_dMaxAngle = m_dMaxAngle;

}

void CFindLineSet::OnBnClickedButtonTest()
{
	if(g_Inspect[m_nSet].m_nROIShow[m_nID]) ((CKoWebView *)m_pParent)->GetTeachROI(m_nSet, m_nID);
	Local2Global();

	CString str;
	
	((CKoWebView *)m_pParent)->m_pImage->SetAddGraphic(true);
	((CKoWebView *)m_pParent)->m_pImage->UpdateImage();
	((CKoWebView *)m_pParent)->UpdateWindow();

	((CKoWebView *)m_pParent)->InspectFindLine(g_fmGrab[g_ID], m_nSet, m_nID, true);

	m_ctrlResult.ResetContent();

	if(g_InspectResult[m_nID].m_nFindLineResult[0])
	{
		str.Format(_T("[NG] Not Found Line"));	m_ctrlResult.AddString(str);
	}
	else
	{
		if(g_InspectResult[m_nID].m_nFindLineResult[1]==0)
		{
			str.Format(_T("[OK] Angle=%.3lf"),g_InspectResult[m_nID].m_dAngle);	m_ctrlResult.AddString(str);
		}
		else
		{
			str.Format(_T("[NG] Angle=%.3lf"),g_InspectResult[m_nID].m_dAngle);	m_ctrlResult.AddString(str);	
		}

		if(g_InspectResult[m_nID].m_t>0)
			str.Format(_T("Line Info: y=[%.3lf]x+[%.3lf]"), g_InspectResult[m_nID].m_a, g_InspectResult[m_nID].m_b);	
		else
			str.Format(_T("Line Info: x=[%.3lf]"), -1*g_InspectResult[m_nID].m_b);	

		m_ctrlResult.AddString(str);
	}
}


void CFindLineSet::OnBnClickedOk()
{
	if(g_Inspect[m_nSet].m_nROIShow[m_nID]) ((CKoWebView *)m_pParent)->GetTeachROI(m_nSet, m_nID);
	Local2Global();

	CDialogEx::OnOK();
}


void CFindLineSet::OnBnClickedRadioLinetype()
{
	GetDlgItem(IDC_RADIO_DIR)->SetWindowTextW(_T("Top->Bottom"));
	GetDlgItem(IDC_RADIO_DIR2)->SetWindowTextW(_T("Bottom->Top"));	
}


void CFindLineSet::OnBnClickedRadioLinetype2()
{
	GetDlgItem(IDC_RADIO_DIR)->SetWindowTextW(_T("Left->Right"));
	GetDlgItem(IDC_RADIO_DIR2)->SetWindowTextW(_T("Right->Left"));
}
