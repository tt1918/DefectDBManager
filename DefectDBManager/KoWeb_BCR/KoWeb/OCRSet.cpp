// OCRSet.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "OCRSet.h"
#include "afxdialogex.h"

#include "KoWebDoc.h"   //@항목추가 16
#include "KoWebView.h"
// COCRSet 대화 상자입니다.

IMPLEMENT_DYNAMIC(COCRSet, CDialogEx)

COCRSet::COCRSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(COCRSet::IDD, pParent)
{
	m_pParent=pParent;		//@항목추가 15-1
	m_nSizeX = 0;
	m_nSizeY = 0;
	m_nCharNum = 0;
	m_nCharPitch = 0;
}

COCRSet::~COCRSet()
{
}

void COCRSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_XSIZE, m_nSizeX);
	DDX_Text(pDX, IDC_EDIT_YSIZE, m_nSizeY);
	DDX_Control(pDX, IDC_LIST1, m_ctrlResult);
	DDX_Text(pDX, IDC_EDIT_CHAR_NUM, m_nCharNum);
	DDX_Text(pDX, IDC_EDIT_PITCH, m_nCharPitch);
	DDX_Check(pDX, IDC_CHECK_EQUALPITCH, m_nEqualPitch);
}


BEGIN_MESSAGE_MAP(COCRSet, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &COCRSet::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDOK, &COCRSet::OnBnClickedOk)
END_MESSAGE_MAP()


// COCRSet 메시지 처리기입니다.




// CCompareSet 메시지 처리기입니다.
void COCRSet::SetInspectID(int nSet, int nID)  //@항목추가 15-2
{
	m_nSet=nSet;
	m_nID =nID;
}

BOOL COCRSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Global2Local();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void COCRSet::OnBnClickedButtonTest()
{
	int i;
	CString str;
	if(g_Inspect[m_nSet].m_nROIShow[m_nID]) ((CKoWebView *)m_pParent)->GetTeachROI(m_nSet, m_nID);
	Local2Global();

//	((CKoWebView *)m_pParent)->m_pImage->SetAddGraphic(true);
//	((CKoWebView *)m_pParent)->m_pImage->UpdateImage();
//	((CKoWebView *)m_pParent)->UpdateWindow();

	((CKoWebView *)m_pParent)->InspectOCR(g_fmGrab[g_ID], m_nSet, m_nID, true);

	m_ctrlResult.ResetContent();	
	for(i=0;i<g_InspectResult[m_nID].m_nOCRNum;i++)
	{		
		if(g_InspectResult[m_nID].m_sOCRScore[i]>=50)
			str.Format(_T("%c -- Score=%d"), g_InspectResult[m_nID].m_cOCR[i],g_InspectResult[m_nID].m_sOCRScore[i] );
		else
			str.Format(_T("XXX") );

		m_ctrlResult.AddString(str);
	}

}

void COCRSet::Global2Local()
{
	m_nSizeX	= g_Inspect[m_nSet].m_InspOCR[m_nID].m_nSizeX;
	m_nSizeY	= g_Inspect[m_nSet].m_InspOCR[m_nID].m_nSizeY;
	m_nCharNum  = g_Inspect[m_nSet].m_InspOCR[m_nID].m_nCharNum;
	m_nCharPitch=g_Inspect[m_nSet].m_InspOCR[m_nID].m_nCharPitch;
	m_nEqualPitch=g_Inspect[m_nSet].m_InspOCR[m_nID].m_nEqualPitch;

	UpdateData(false);
}

void COCRSet::Local2Global()
{
	UpdateData(true);

	g_Inspect[m_nSet].m_InspOCR[m_nID].m_nSizeX		=m_nSizeX;
	g_Inspect[m_nSet].m_InspOCR[m_nID].m_nSizeY		=m_nSizeY;
	g_Inspect[m_nSet].m_InspOCR[m_nID].m_nCharNum	=m_nCharNum;
	g_Inspect[m_nSet].m_InspOCR[m_nID].m_nCharPitch =m_nCharPitch;
	g_Inspect[m_nSet].m_InspOCR[m_nID].m_nEqualPitch=m_nEqualPitch;
}

void COCRSet::OnBnClickedOk()
{
	Local2Global();
	CDialogEx::OnOK();
}
