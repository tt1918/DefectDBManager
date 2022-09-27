// DlgParam.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "DlgParam.h"
#include "afxdialogex.h"


// CDlgParam 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgParam, CDialogEx)

CDlgParam::CDlgParam(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgParam::IDD, pParent)
{

}

CDlgParam::~CDlgParam()
{
}


BOOL CDlgParam::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int i;
	int nNoWidth=40;
	int nWidth=200;
	CString str;
	CRect rc;

	m_ctrlParam1.SubclassDlgItem(IDC_LIST_PARAM1, this);
	m_ctrlGrade.SubclassDlgItem(IDC_LIST_GRADE,   this);
	m_ctrlParam2.SubclassDlgItem(IDC_LIST_PARAM2, this);

	m_ctrlParam2.GetClientRect(&rc);
	m_ctrlParam2.InsertColumn(0,_T("No"), LVCFMT_CENTER, nNoWidth);
	m_ctrlParam2.InsertColumn(1,_T("Level Parameter"), LVCFMT_CENTER, nWidth-20);
	m_ctrlParam2.InsertColumn(2,_T("Data"), LVCFMT_CENTER, rc.Width()-(nWidth-20)-nNoWidth-20);

	m_ctrlGrade.GetClientRect(&rc);
	m_ctrlGrade.InsertColumn(0,_T("No"), LVCFMT_CENTER, nNoWidth);
	m_ctrlGrade.InsertColumn(1,_T("Grade Parameter"), LVCFMT_CENTER, nWidth);
	m_ctrlGrade.InsertColumn(2,_T("Data"), LVCFMT_CENTER, rc.Width()-nWidth-nNoWidth);

	m_ctrlParam1.GetClientRect(&rc);
	m_ctrlParam1.InsertColumn(0,_T("No"), LVCFMT_CENTER, nNoWidth);
	m_ctrlParam1.InsertColumn(1,_T("Parameter"), LVCFMT_CENTER, nWidth);
	m_ctrlParam1.InsertColumn(2,_T("Data"), LVCFMT_CENTER, rc.Width()-nWidth-nNoWidth);



	for(i=0;i<NEW_MAX_COL_ALG_ITEM;i++)
	{
		str.Format(_T("%d"), i+1);
		if(i<120)	
		{			
			m_ctrlParam1.InsertItem(i, str);
			m_ctrlParam1.SetItem(i, 1, LVIF_TEXT, g_ParamName[i], 0, 0, 0, 0);
		}
		else if(i<160)
		{
			m_ctrlGrade.InsertItem(i, str);
			m_ctrlGrade.SetItem(i-120, 1, LVIF_TEXT, g_ParamName[i], 0, 0, 0, 0);
		}
		else
		{
			m_ctrlParam2.InsertItem(i, str);
			m_ctrlParam2.SetItem(i-160, 1, LVIF_TEXT, g_ParamName[i], 0, 0, 0, 0);				
		}
	}	


	UpdateSpread(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlgParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgParam, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_APPLY, &CDlgParam::OnBnClickedButtonSaveApply)
	ON_BN_CLICKED(IDOK, &CDlgParam::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_READPARAMETER, &CDlgParam::OnBnClickedButtonReadparameter)
	ON_BN_CLICKED(IDCANCEL, &CDlgParam::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgParam 메시지 처리기입니다.

//읽어서 저장 및 적용
void CDlgParam::OnBnClickedButtonSaveApply()
{
	UpdateSpread(true);
	ApplyParameter();
	SaveParameter();

	CDialogEx::OnOK();
}


//읽어서 적용
void CDlgParam::OnBnClickedOk()
{
	UpdateSpread(true);
	ApplyParameter();

	CDialogEx::OnOK();
}


void CDlgParam::OnBnClickedButtonReadparameter()
{
	ReadParameter(0);
	UpdateSpread(false);
}

void CDlgParam::UpdateSpread(bool flag)
{
	int i;
	CString str;

	if(!flag)
	{
		for(i=0;i<NEW_MAX_COL_ALG_ITEM;i++)
		{
			if(i==57 )					str.Format(_T("%.7lf"), g_ParamVer.param[i]);
			else if(i==58 || i==59)		str.Format(_T("%.5lf"), g_ParamVer.param[i]);
			else						str.Format(_T("%.3lf"), g_ParamVer.param[i]);

						
			if(i<40)		m_ctrlParam1._SetItemControl(i,    2, _CT_CEdit, 0, str);			
			else if(i<60)   m_ctrlGrade._SetItemControl(i-40,  2, _CT_CEdit, 0, str);				
			else			m_ctrlParam2._SetItemControl(i-60, 2, _CT_CEdit, 0, str);						
		}	
	}
	else
	{

		g_ParamVer.system[0]=g_ParamVer.param[39];
		g_ParamVer.system[1]=g_ParamVer.param[55];
		g_ParamVer.system[2]=g_ParamVer.param[56];
		g_ParamVer.system[3]=g_ParamVer.param[57];
		g_ParamVer.system[4]=g_ParamVer.param[58];
		g_ParamVer.system[5]=g_ParamVer.param[59];
	}
}


void CDlgParam::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


