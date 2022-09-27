// DlgModelName.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "DlgModelName.h"
#include "afxdialogex.h"

extern CString g_ModelName[256];
extern int     g_ModelCount;

// CDlgModelName 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgModelName, CDialogEx)

CDlgModelName::CDlgModelName(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgModelName::IDD, pParent)
{

	m_sModelName = _T("");
}

CDlgModelName::~CDlgModelName()
{
}

void CDlgModelName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MODELNAME, m_sModelName);
}


BEGIN_MESSAGE_MAP(CDlgModelName, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgModelName::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgModelName 메시지 처리기입니다.


void CDlgModelName::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int i;
	UpdateData(true);

	for(i=0;i<g_ModelCount;i++)
	{
		if(m_sModelName.Compare(g_ModelName[i])==0)
		{
			AfxMessageBox(_T("Input Other Name (Same Model Exist!!!)"));

			return;
		}
	}

	CDialogEx::OnOK();
}
