// DlgModel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "DlgModel.h"
#include "afxdialogex.h"
#include "DlgModelName.h"
#include "KoWebDoc.h"
#include "KoWebView.h"

// CDlgModel 대화 상자입니다.
CString g_ModelName[256];
int     g_ModelCount=0;

IMPLEMENT_DYNAMIC(CDlgModel, CDialogEx)

CDlgModel::CDlgModel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgModel::IDD, pParent)
{
	m_pParent=pParent;
}

CDlgModel::~CDlgModel()
{
}

void CDlgModel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrlModelList);
}


BEGIN_MESSAGE_MAP(CDlgModel, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDlgModel::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CDlgModel::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CDlgModel::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, &CDlgModel::OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDOK, &CDlgModel::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SAVEAS, &CDlgModel::OnBnClickedButtonSaveas)
END_MESSAGE_MAP()


// CDlgModel 메시지 처리기입니다.




BOOL CDlgModel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	FillModelList();
	GetDlgItem(IDC_CURRENT)->SetWindowText(g_Temp.m_slotName);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgModel::OnBnClickedButtonSave()
{
	if(SaveModel()<0)
	{
		AfxMessageBox(_T("Model Save Error. Check Model Folder!"));
	}
}




void CDlgModel::FillModelList()
{
	BOOL bRval=FALSE;
	int nRval=0;
	CString sFilePath;
	CString szNextDirPath=_T("");
	CString szRoot=_T("");
	CFileFind find;

	sFilePath.Format(_T("%s*.*"), MODEL_PATH);
	//폴더가 있는지
	bRval=find.FindFile(sFilePath);
	if(!bRval) return;

	m_ctrlModelList.ResetContent();
	g_ModelCount=0;
	while(bRval)
	{
		bRval=find.FindNextFileW();
		if(find.IsDots()) continue;
		if(find.IsDirectory())
		{
			m_ctrlModelList.AddString(find.GetFileName());
			g_ModelName[g_ModelCount++]=find.GetFileName();
			if(g_ModelCount>=256) break;
		}
	}	
	find.Close();

}

void CDlgModel::OnBnClickedButtonNew()
{
	int i;
	CDlgModelName dlg;
	CString sFolderName;

	if(dlg.DoModal()==IDOK)
	{
		g_Temp.m_slotName=dlg.m_sModelName;
		sFolderName.Format(_T("%s%s"), MODEL_PATH,g_Temp.m_slotName);
		CreateDirectory(sFolderName,NULL);

		FillModelList();
		NewModel();
		GetDlgItem(IDC_CURRENT)->SetWindowText(g_Temp.m_slotName);
		SaveCurrentLotName();
		((CKoWebView*) m_pParent)->SetSpreadData(3,g_Temp.m_slotName);
	}
}


void CDlgModel::OnBnClickedButtonDelete()
{
	CString sModelName, sCheck, sFullPath;
	int N=m_ctrlModelList.GetCurSel();
	if(N<0) return;

	m_ctrlModelList.GetText(N, sModelName);
	if(sModelName.Compare(g_Temp.m_slotName)==0)
	{
		AfxMessageBox(_T("It can't delete the current Model!"));
		return;
	}

	sCheck.Format(_T("Do you want to remove this Model(%s)?"), sModelName);
	if(AfxMessageBox(sCheck, MB_OKCANCEL) == IDCANCEL)	return;
	

	sFullPath.Format(_T("%s%s\\*.*"), MODEL_PATH,sModelName);
	DeleteDirectory(sFullPath);
	FillModelList();

	return;
}


void CDlgModel::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


void CDlgModel::OnBnClickedButtonSaveas()
{
	int i;
	CDlgModelName dlg;
	CString sModelName, sFolderName;

	if(dlg.DoModal()==IDOK)
	{
		sModelName=dlg.m_sModelName;
		sFolderName.Format(_T("%s%s"), MODEL_PATH,sModelName);
		CreateDirectory(sFolderName,NULL);

		//현재것 저장


		//현재모든폴더를 Save As 폴더로 복사
		CopyDirectory(g_Temp.m_slotName, sModelName);

		g_Temp.m_slotName=sModelName;
		FillModelList();
		GetDlgItem(IDC_CURRENT)->SetWindowText(g_Temp.m_slotName);
		SaveCurrentLotName();
		((CKoWebView*) m_pParent)->SetSpreadData(3,g_Temp.m_slotName);
	}
}

void CDlgModel::OnBnClickedButtonSelect()
{
	int N=m_ctrlModelList.GetCurSel();
	CString sModelName;
	if(N<0) return;

	m_ctrlModelList.GetText(N, sModelName);
	
	if(LoadModel(sModelName)==0)
	{
		g_Temp.m_slotName=sModelName;
		SaveCurrentLotName();
		((CKoWebView*) m_pParent)->SetSpreadData(3,g_Temp.m_slotName);
	}
	CDialogEx::OnOK();
}