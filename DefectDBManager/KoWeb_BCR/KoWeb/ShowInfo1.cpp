// ShowInfo1.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "ShowInfo1.h"
#include "afxdialogex.h"
#include "KoWebDoc.h"
#include "KoWebView.h"

// CShowInfo1 대화 상자입니다.

IMPLEMENT_DYNAMIC(CShowInfo1, CDialogEx)

CShowInfo1::CShowInfo1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShowInfo1::IDD, pParent)
{
	m_pWnd=pParent;
	m_nExposure = 0;
	m_nNotInspectArea = 0;
	m_nEdgeOffset = 0;
	m_nLatestResult=0;
}

CShowInfo1::~CShowInfo1()
{
}

void CShowInfo1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_EXPOSURE, m_nExposure);
	DDX_Check(pDX, IDC_CHECK_SHOW_FLAT, m_bShowFlat);
	DDX_Check(pDX, IDC_CHECK_SHOW_AREA, m_bShowCandiArea);
	DDX_Check(pDX, IDC_CHECK_SHOW_ALLCANDI, m_bShowAllCandi);
	DDX_Check(pDX, IDC_CHECK_SHOW_VALIDCANDI, m_bShowValidCandi);
	DDX_Check(pDX, IDC_CHECK_BCR_MARK, m_bBcrMark);
	DDX_Text(pDX, IDC_EDIT_NOTINSPECT_AREA, m_nNotInspectArea);
	DDX_Text(pDX, IDC_EDIT_EDGE_OFFSET, m_nEdgeOffset);
	DDX_Control(pDX, IDC_BUTTON_SETEXPOSURE, m_btData1);
	DDX_Control(pDX, IDC_BUTTON_SETEXPOSURE2, m_btData2);
	DDX_Control(pDX, IDC_BUTTON_SETEXPOSURE3, m_btData3);
}


BEGIN_MESSAGE_MAP(CShowInfo1, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SETEXPOSURE, &CShowInfo1::OnBnClickedButtonSetexposure)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDCANCEL, &CCossImage::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_SHOW_FLAT, &CShowInfo1::OnBnClickedCheckShowFlat)
	ON_BN_CLICKED(IDC_CHECK_SHOW_AREA, &CShowInfo1::OnBnClickedCheckShowArea)
	ON_BN_CLICKED(IDC_CHECK_SHOW_ALLCANDI, &CShowInfo1::OnBnClickedCheckShowAllcandi)
	ON_BN_CLICKED(IDC_CHECK_SHOW_VALIDCANDI, &CShowInfo1::OnBnClickedCheckShowValidcandi)
	ON_BN_CLICKED(IDC_BUTTON_SETEXPOSURE2, &CShowInfo1::OnBnClickedButtonSetexposure2)
	ON_BN_CLICKED(IDC_BUTTON_SETEXPOSURE3, &CShowInfo1::OnBnClickedButtonSetexposure3)
	ON_BN_CLICKED(IDC_CHECK_BCR_MARK, &CShowInfo1::OnBnClickedCheckBcrMark)
END_MESSAGE_MAP()


// CShowInfo1 메시지 처리기입니다.


BOOL CShowInfo1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int i;
	int nYOffset=24;
	WINDOWPLACEMENT wp;

	m_ColorBK=RGB(40,40,40);
	m_Color[0]=RGB(0,0,0);
	m_Color[1]=RGB(40,40,40);

#ifndef BARCODE_VISION
	m_nYCnt=11;
#else
	m_nYCnt = 12;
#endif

	GetDlgItem(IDC_NGOK)->GetWindowPlacement(&wp);

	m_rcNG=wp.rcNormalPosition;
	m_nX	=(wp.rcNormalPosition.right-wp.rcNormalPosition.left)*0.45;
	m_nXEnd	=wp.rcNormalPosition.right+1;

	for(i=0;i<m_nYCnt;i++)
	{
		m_nY[i]=i*nYOffset;
	}
	m_sTitle[0]=_T("Version");
	m_sTitle[1]=_T("PC Name");
#if PROGRAM_TYPE==GENERAL_TEACH
	m_sTitle[2]=_T("Model Name");
	m_sTitle[3]=_T("Frame Cnt");
#else
	m_sTitle[2]=_T("LOT Name");
	m_sTitle[3]=_T("Insp Cnt/Frame Cnt");
#endif

	m_sTitle[4]=_T("NG Frame Cnt");
	m_sTitle[5]=_T("NG Cnt");
	m_sTitle[6]=_T("FPS");
	m_sTitle[7]=_T("Insp Time(msec)");
	m_sTitle[8]=_T("Bright/Sharp");
	m_sTitle[9]=_T("HDD(%)");
#ifdef BARCODE_VISION
	m_sTitle[10]=_T("BCNO");
#endif
	m_nExposure			=g_Param.m_nExposure;
	m_nNotInspectArea	=g_Param.m_nNotInspArea;
	m_nEdgeOffset		=g_Param.m_nEdgeOffset;

#ifdef BARCODE_VISION
	g_Param.m_bBcrMark=m_bBcrMark = true;
#endif

	UpdateData(false);

//OnCtlColor ----------------------------------------------------------------
	::SetWindowTheme(GetDlgItem(IDC_CHECK_SHOW_FLAT)			->GetSafeHwnd(), L"", L""); 
	::SetWindowTheme(GetDlgItem(IDC_CHECK_SHOW_AREA)			->GetSafeHwnd(), L"", L""); 
	::SetWindowTheme(GetDlgItem(IDC_CHECK_SHOW_ALLCANDI)		->GetSafeHwnd(), L"", L""); 
	::SetWindowTheme(GetDlgItem(IDC_CHECK_SHOW_VALIDCANDI)		->GetSafeHwnd(), L"", L"");
	::SetWindowTheme(GetDlgItem(IDC_CHECK_BCR_MARK)				->GetSafeHwnd(), L"", L"");
//	::SetWindowTheme(GetDlgItem(IDC_EDIT_EXPOSURE)		->GetSafeHwnd(), L"", L""); 


	m_BTFont.CreatePointFont(100, _T("Segoe UI Symbol"));
	initButtonST(0, &m_btData1);
	initButtonST(0, &m_btData2);
	initButtonST(0, &m_btData3);

#ifdef USE_3D_AREA
	GetDlgItem(IDC_CHECK_SHOW_AREA)			->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK_SHOW_ALLCANDI)		->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK_SHOW_VALIDCANDI)	->ShowWindow(SW_HIDE);
#endif

#ifdef BARCODE_VISION
	GetDlgItem(IDC_CHECK_SHOW_AREA)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK_SHOW_ALLCANDI)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK_SHOW_VALIDCANDI)-> ShowWindow(SW_HIDE);
#else
	GetDlgItem(IDC_CHECK_BCR_MARK)->ShowWindow(SW_HIDE);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CShowInfo1::initButtonST(int nType, CButtonST *cButton)
{
	if(nType==0)
	{
		cButton->SetUserFont(&m_BTFont);
		cButton->SetColor(CButtonST::BTNST_COLOR_BK_IN, COLOR_FOCUS);  //마우스 올라갔을때 보이는 것
		cButton->SetColor(CButtonST::BTNST_COLOR_FG_IN, COLOR_BLACK);
		cButton->SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(80,80,80)); //평소보이는 것
		cButton->SetColor(CButtonST::BTNST_COLOR_FG_OUT, RGB(225,225,225));
		cButton->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, COLOR_YELLOW);
		cButton->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, COLOR_BLACK);
	}
}


void CShowInfo1::SetData(int nRow, CString str, int nColor)
{
	CRect rc;
	if(nRow<1) return;

	rc.left=m_nX+2;
	rc.right=m_nXEnd;
	rc.top=m_nY[nRow-1]+2;
	rc.bottom=m_nY[nRow]-2;
	m_sData[nRow-1]=str;
	m_nColor[nRow-1]=nColor;
	InvalidateRect(rc,true);
}

void CShowInfo1::SetResult(int nResult)
{
	m_nLatestResult=nResult;		//NonZero : NG
	InvalidateRect(m_rcNG,true);
}

void CShowInfo1::ShowParamData(int nExposure, int nNotInspect, int nEdgeOffset)
{
	m_nExposure			=nExposure;
	m_nNotInspectArea	=nNotInspect;
	m_nEdgeOffset		=nEdgeOffset;
	UpdateData(false);
}



void CShowInfo1::DrawTitle(CPaintDC &dc)
{
	int i;
	CBrush colorBK(m_ColorBK);
	CBrush color0(m_Color[0]);
	CBrush color1(m_Color[1]);
	CPen *oldPen, newPen(PS_SOLID,1,RGB(128,128,128));
	CRect rc;

	GetClientRect(&rc);
	dc.FillRect(rc, &colorBK);

	rc.left=0;
	rc.top=0;
	rc.right=m_nX;
	rc.bottom=m_nY[m_nYCnt-1];
	dc.FillRect(rc, &color0);

	rc.left=m_nX;
	rc.top=0;
	rc.right=m_nXEnd;
	rc.bottom=m_nY[m_nYCnt-1];
	dc.FillRect(rc, &color1);

	oldPen=dc.SelectObject(&newPen);
	for(i=1;i<m_nYCnt;i++)
	{
		dc.MoveTo(0,m_nY[i]);
		dc.LineTo(m_nXEnd, m_nY[i]);
	}
	dc.MoveTo(m_nX,0);
	dc.LineTo(m_nX, m_nY[m_nYCnt-1]);


	for(i=0;i<m_nYCnt-1;i++)
	{
		rc.left=0;
		rc.right=m_nX;
		rc.top=m_nY[i];
		rc.bottom=m_nY[i+1];
		dc.DrawText(m_sTitle[i], m_sTitle[i].GetLength(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE	);
	}
	dc.SelectObject(oldPen);
}

void CShowInfo1::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(200,200,0));
	CFont *oldFont;
	CFont font, fontNG;
	font.CreatePointFont(110, _T("Segoe UI Symbol"));
	oldFont=dc.SelectObject(&font);

	DrawTitle(dc);

	int i;
	CRect rc;

//	dc.SetTextColor(RGB(0,0,0));
	dc.SetTextColor(RGB(210,200,200));
	//데이타 쓰는 부분------------------------------------------------------------------------------
	for(i=0;i<m_nYCnt-1;i++)
	{
		rc.left=m_nX;
		rc.right=m_nXEnd;
		rc.top=m_nY[i];
		rc.bottom=m_nY[i+1];
		if(m_nColor[i]) dc.SetTextColor(RGB(255,0,0));
		else            dc.SetTextColor(RGB(210,200,200));
		dc.DrawText(m_sData[i], m_sData[i].GetLength(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE	);
	}
	//------------------------------------------------------------------------------------------------

	fontNG.CreatePointFont(300, _T("Segoe UI Symbol"));
	dc.SelectObject(&fontNG);
	if(m_nLatestResult)	//가장 최근 결과
	{
		dc.SetTextColor(RGB(255,0,0));
		dc.DrawText(_T("N G"), 3, m_rcNG, DT_CENTER|DT_VCENTER|DT_SINGLELINE	);
	}
	else
	{
		dc.SetTextColor(RGB(0,255,0));
		dc.DrawText(_T("O K"), 3, m_rcNG, DT_CENTER|DT_VCENTER|DT_SINGLELINE	);
	}
	

	dc.SelectObject(oldFont);
}

//ESC눌러도 안없어지게...
void CShowInfo1::OnBnClickedCancel()
{
//	CDialog::OnCancel();
}


void CShowInfo1::OnBnClickedCheckShowFlat()
{
	m_bShowFlat=1-m_bShowFlat;
	((CKoWebView *)m_pWnd)->m_bShowFlat=m_bShowFlat;
}

void CShowInfo1::OnBnClickedCheckShowArea()
{
	m_bShowCandiArea=1-m_bShowCandiArea;
	((CKoWebView *)m_pWnd)->m_bShowCandiArea=m_bShowCandiArea;
	if(m_bShowCandiArea)
	{
		((CKoWebView *)m_pWnd)->SetShowImage(true);
	}
}
void CShowInfo1::OnBnClickedCheckShowAllcandi()
{
	m_bShowAllCandi=1-m_bShowAllCandi;
	((CKoWebView *)m_pWnd)->m_bShowAllCandi=m_bShowAllCandi;
	if(m_bShowAllCandi)
	{
		((CKoWebView *)m_pWnd)->SetShowImage(true);
	}
}

void CShowInfo1::OnBnClickedCheckShowValidcandi()
{
	m_bShowValidCandi=1-m_bShowValidCandi;
	((CKoWebView *)m_pWnd)->m_bShowValidCandi=m_bShowValidCandi;
	if(m_bShowValidCandi)
	{
		((CKoWebView *)m_pWnd)->SetShowImage(true);
	}
}

void CShowInfo1::OnBnClickedCheckBcrMark()
{
	m_bBcrMark = 1 - m_bBcrMark;
	g_Param.m_bBcrMark = (bool)m_bBcrMark;
}


HBRUSH CShowInfo1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	int nID=pWnd->GetDlgCtrlID();
	CBrush   Back(RGB(128,128,128));

	switch(nID)
	{
		case IDC_CHECK_SHOW_FLAT:
		case IDC_CHECK_SHOW_AREA:
		case IDC_CHECK_SHOW_ALLCANDI:
		case IDC_CHECK_SHOW_VALIDCANDI:	
		case IDC_CHECK_BCR_MARK:

			pDC->SetTextColor(RGB(255,255,255));
			pDC->SetBkMode(TRANSPARENT);

			return (HBRUSH)::GetStockObject(NULL_BRUSH);
			break;

		//case IDC_EDIT_EXPOSURE:
		//	pDC->SetBkColor(RGB(128,128,128));
		//	pDC->FillRect(CRect(0,0,100,100), &Back);
		//	pDC->SetTextColor(RGB(0,0,0));
		//	pDC->SetBkMode(TRANSPARENT);
		//
		//	return (HBRUSH)::GetStockObject(NULL_BRUSH);
		//	break;

		default: break;	
	}
	return hbr;
}




void CShowInfo1::OnBnClickedButtonSetexposure()
{
	bool flag=false;
	UpdateData(true);
	
	if(g_Param.m_nSetAutoExposure && g_Temp.m_nAutoExpoStart) 
		return;

	if(m_nExposure>g_System.m_nMaxExposure)		 { m_nExposure=g_System.m_nMaxExposure; flag=true;}
	else if(m_nExposure<g_System.m_nMinExposure) { m_nExposure=g_System.m_nMinExposure; flag=true;}

	g_Param.m_nExposure=m_nExposure;
	if(m_nExposure!=g_Temp.m_nExposureData)
	{
		g_Grabber.SetExposureTime(g_Param.m_nExposure, 1-g_Temp.m_bAuto);
//		g_Grabber.SetExposureTime(g_Param.m_nExposure, 1);		
	}

	if(flag) 
		UpdateData(false);
}


void CShowInfo1::OnBnClickedButtonSetexposure2()
{
	UpdateData(true);
	if(m_nNotInspectArea>g_System.m_nImageW || m_nNotInspectArea<-g_System.m_nImageW)
	{
		m_nNotInspectArea=g_System.m_nImageW;
		UpdateData(false);
	}
	g_Param.m_nNotInspArea=m_nNotInspectArea;
}


void CShowInfo1::OnBnClickedButtonSetexposure3()
{
	UpdateData(true);
	g_Param.m_nEdgeOffset=m_nEdgeOffset;
}
void CShowInfo1::CheckAllReset()
{
	m_bShowFlat=false;
	m_bShowCandiArea=false;
	m_bShowAllCandi=false;
	m_bShowValidCandi=false;;
	
	UpdateData(false);
}


BOOL CShowInfo1::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)
	{
		int nData[3];
		if(pMsg->wParam==VK_ESCAPE) return FALSE;
		if(pMsg->wParam==VK_RETURN)
		{
			nData[0]=m_nExposure;
			nData[1]=m_nNotInspectArea;
			nData[2]=m_nEdgeOffset;

			UpdateData(true);
			if(m_nExposure!=nData[0])		OnBnClickedButtonSetexposure();
			if(nData[1]!=m_nNotInspectArea) OnBnClickedButtonSetexposure2();
			if(nData[2]!=m_nEdgeOffset)     OnBnClickedButtonSetexposure3();

			return FALSE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

