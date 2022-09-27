// CameraCalib.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "CameraCalib.h"
#include "afxdialogex.h"


// CCameraCalib 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCameraCalib, CDialogEx)

CCameraCalib::CCameraCalib(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCameraCalib::IDD, pParent)
{
	m_nDataL = 0;
	m_nDataR = 0;
}

CCameraCalib::~CCameraCalib()
{
}

void CCameraCalib::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LEFT, m_nDataL);
	DDX_Text(pDX, IDC_EDIT_RIGHT, m_nDataR);
}


BEGIN_MESSAGE_MAP(CCameraCalib, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LEFT, &CCameraCalib::OnDeltaposSpinLeft)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RIGHT, &CCameraCalib::OnDeltaposSpinRight)
	ON_BN_CLICKED(IDCANCEL, &CCameraCalib::OnBnClickedCancel)
	ON_BN_CLICKED(ID_SAVE, &CCameraCalib::OnBnClickedSave)
END_MESSAGE_MAP()


// CCameraCalib 메시지 처리기입니다.
BOOL CCameraCalib::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_nLDataOld=m_nDataL=g_System.m_nLUTL;
	m_nRDataOld=m_nDataR=g_System.m_nLUTR;


	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CCameraCalib::OnDeltaposSpinLeft(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(true);
	if(pNMUpDown->iDelta<0)
	{
		if( m_nDataL+1<=50)   m_nDataL++;
	}
	else
	{
		if(m_nDataL-1>=-50)  m_nDataL--;

	}
	g_System.m_nLUTL=m_nDataL;
	UpdateData(false);

	MakeLUT();

	*pResult = 0;
}


void CCameraCalib::OnDeltaposSpinRight(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(true);
	if(pNMUpDown->iDelta<0)
	{
		if( m_nDataR+1<=50)   m_nDataR++;
	}
	else
	{
		if(m_nDataR-1>=-50)  m_nDataR--;

	}
	g_System.m_nLUTR=m_nDataR;
	UpdateData(false);

	MakeLUT();

	*pResult = 0;
}




void CCameraCalib::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	g_System.m_nLUTL=m_nLDataOld;
	g_System.m_nLUTR=m_nRDataOld;
	MakeLUT();
	CDialogEx::OnCancel();
}


void CCameraCalib::OnBnClickedSave()
{
	SavePerspective();
}
