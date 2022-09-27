// DlgPattern.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "DlgPattern.h"
#include "afxdialogex.h"

#include "KoWebDoc.h"
#include "KoWebView.h"
// CDlgPattern 대화 상자입니다.

CROIControl g_ROIPattern;
int l_nReturnX=0;
int l_nReturnY=0;

IMPLEMENT_DYNAMIC(CDlgPattern, CDialogEx)

CDlgPattern::CDlgPattern(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPattern::IDD, pParent)
{
	m_pParent = pParent;
	m_nMinScore = 0;
	m_pDisplay=NULL;
	m_sComment = _T("");
	m_sComment = _T("");
}

CDlgPattern::~CDlgPattern()
{
	g_ROIPattern.Close();
	if(m_pDisplay) delete m_pDisplay;
}

void CDlgPattern::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PATTERNID, m_ctrlPatternID);
	DDX_Control(pDX, IDC_LIST_RESULT, m_ctrlResult);
	DDX_Text(pDX, IDC_EDIT_MINSCORE, m_nMinScore);
	DDX_Check(pDX, IDC_CHECK_SHOWROI, m_nROI);
	DDX_Text(pDX, IDC_EDIT_COMMENT, m_sComment);
	DDX_Check(pDX, IDC_CHECK_CHANGE_RETURN, m_bChangePos);
}


BEGIN_MESSAGE_MAP(CDlgPattern, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_PATTERNID, &CDlgPattern::OnCbnSelchangeComboPatternid)
	ON_BN_CLICKED(IDC_CHECK_CHANGE_RETURN, &CDlgPattern::OnBnClickedCheckChangeReturn)
	ON_BN_CLICKED(IDC_BUTTON_LEFT, &CDlgPattern::OnBnClickedButtonLeft)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT, &CDlgPattern::OnBnClickedButtonRight)
	ON_BN_CLICKED(IDC_BUTTON_UP, &CDlgPattern::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, &CDlgPattern::OnBnClickedButtonDown)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDlgPattern::OnDeltaposSpin1)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, &CDlgPattern::OnBnClickedButtonRegister)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CDlgPattern::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_CHECK_SHOWROI, &CDlgPattern::OnBnClickedCheckShowroi)
	ON_BN_CLICKED(IDC_BUTTON_APPLYALL, &CDlgPattern::OnBnClickedButtonApplyall)
	ON_BN_CLICKED(IDOK, &CDlgPattern::OnBnClickedOk)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CDlgPattern 메시지 처리기입니다.



BOOL CDlgPattern::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	WINDOWPLACEMENT wp;
	GetDlgItem(IDC_STATIC_IMAGE)->GetWindowPlacement(&wp);

	OpenDisplay(PM_MAX_SIZEX,PM_MAX_SIZEY, wp.rcNormalPosition);

	int i;
	CString str;
	for(i=0;i<MAX_PATTERN;i++)
	{
		str.Format(_T("%d"), i);
		m_ctrlPatternID.AddString(str);
	}

	m_nId=g_Temp.m_nLastPatternID;
	m_ctrlPatternID.SetCurSel(m_nId);
	SelectPattern(m_nId);
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
void CDlgPattern::OpenDisplay(int nWidth, int nHeight, CRect rcDisp)
{
	CString str;
	
	if(m_pDisplay==NULL)
	{
		g_ROIPattern.Initialize(nWidth, nHeight);
		m_pDisplay=new CCossImage(this);
		m_pDisplay->m_bShowGrayValue=false;
		m_pDisplay->IMAGE_COLOR=1;
		m_pDisplay->SetImageData(nWidth, nHeight);
		m_pDisplay->m_pROI=&g_ROIPattern;
		m_pDisplay->m_fmProcess=NULL;
		m_pDisplay->m_nDisplayID=1;
		str.Format(_T(""));
		m_pDisplay->SetImageName(str);
		m_pDisplay->Create(IDD_DIALOG_COSSIMAGE,this);
		m_pDisplay->MoveWindow(rcDisp);
		m_pDisplay->FitZoom();
		m_pDisplay->ShowWindow(SW_SHOWNORMAL);
	}
}
void CDlgPattern::OnCbnSelchangeComboPatternid()
{
	m_nId=m_ctrlPatternID.GetCurSel();
	SelectPattern(m_nId);
	UpdateData(false);
}
void CDlgPattern::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	UpdateData(true);

	wsprintf(g_Pattern[m_nId].m_sName, _T("%s"),m_sComment);
	g_Pattern[m_nId].m_nMinScore=m_nMinScore;


	if(pNMUpDown->iDelta<0)
	{
		if( m_nId+1<MAX_PATTERN) m_nId++;
	}
	else
	{
		if(m_nId-1>=0)  m_nId--;

	}
	m_ctrlPatternID.SetCurSel(m_nId);
	SelectPattern(m_nId);
	UpdateData(false);


	*pResult = 0;
}




void CDlgPattern::OnBnClickedCheckChangeReturn()
{
	UpdateData(true);
	GetDlgItem(IDC_BUTTON_LEFT)	->EnableWindow(m_bChangePos);
	GetDlgItem(IDC_BUTTON_RIGHT)->EnableWindow(m_bChangePos);
	GetDlgItem(IDC_BUTTON_UP)	->EnableWindow(m_bChangePos);
	GetDlgItem(IDC_BUTTON_DOWN)	->EnableWindow(m_bChangePos);
}


void CDlgPattern::OnBnClickedButtonLeft()
{
	if(g_Pattern[m_nId].m_nAddCount==0) return;
	if(g_Pattern[m_nId].m_dReturnPosX-1>=0) g_Pattern[m_nId].m_dReturnPosX-=1;
	g_Pattern[m_nId].m_dReturnPosX=(int)g_Pattern[m_nId].m_dReturnPosX;

	m_pDisplay->ResetGraphic();
	m_pDisplay->UpdateImage();
	m_pDisplay->SetAddGraphic(true);
	m_pDisplay->DrawCross(NULL, g_Pattern[m_nId].m_dReturnPosX, g_Pattern[m_nId].m_dReturnPosY,  20, RGB(255,0,0));

}


void CDlgPattern::OnBnClickedButtonRight()
{
	if(g_Pattern[m_nId].m_nAddCount==0) return;
	if(g_Pattern[m_nId].m_dReturnPosX+1<=g_Pattern[m_nId].m_nWidth-1) g_Pattern[m_nId].m_dReturnPosX+=1;
	g_Pattern[m_nId].m_dReturnPosX=(int)g_Pattern[m_nId].m_dReturnPosX;

	m_pDisplay->ResetGraphic();
	m_pDisplay->UpdateImage();
	m_pDisplay->SetAddGraphic(true);
	m_pDisplay->DrawCross(NULL, g_Pattern[m_nId].m_dReturnPosX, g_Pattern[m_nId].m_dReturnPosY,  20, RGB(255,0,0));
}


void CDlgPattern::OnBnClickedButtonUp()
{
	if(g_Pattern[m_nId].m_nAddCount==0) return;
	if(g_Pattern[m_nId].m_dReturnPosY-1>=0) g_Pattern[m_nId].m_dReturnPosY-=1;
	g_Pattern[m_nId].m_dReturnPosY=(int)g_Pattern[m_nId].m_dReturnPosY;

	m_pDisplay->ResetGraphic();
	m_pDisplay->UpdateImage();
	m_pDisplay->SetAddGraphic(true);
	m_pDisplay->DrawCross(NULL, g_Pattern[m_nId].m_dReturnPosX, g_Pattern[m_nId].m_dReturnPosY,  20, RGB(255,0,0));
}


void CDlgPattern::OnBnClickedButtonDown()
{
	if(g_Pattern[m_nId].m_nAddCount==0) return;
	if(g_Pattern[m_nId].m_dReturnPosY+1<=g_Pattern[m_nId].m_nHeight-1) g_Pattern[m_nId].m_dReturnPosY+=1;
	g_Pattern[m_nId].m_dReturnPosY=(int)g_Pattern[m_nId].m_dReturnPosY;

	m_pDisplay->ResetGraphic();
	m_pDisplay->UpdateImage();
	m_pDisplay->SetAddGraphic(true);
	m_pDisplay->DrawCross(NULL, g_Pattern[m_nId].m_dReturnPosX, g_Pattern[m_nId].m_dReturnPosY,  20, RGB(255,0,0));
}


void CDlgPattern::OnBnClickedButtonRegister()
{
	int left, top, right, bottom, pitch;

	if(g_Pattern[m_nId].m_nAddCount)
	{
		if(g_Pattern[m_nId].m_fm[0]) delete g_Pattern[m_nId].m_fm[0];
		if(g_Pattern[m_nId].m_fm[1]) delete g_Pattern[m_nId].m_fm[1];
		g_Pattern[m_nId].m_nAddCount=0;
		m_pDisplay->m_nGraphicCount=0;
		m_pDisplay->ClearImage(0);
	}

	LPBYTE fm=((CKoWebView *)m_pParent)->m_pImage->GetImageDataDisplay(&pitch);
	((CKoWebView *)m_pParent)->m_pImage->m_pROI->GetROIRegion(_T("P"), &left, &top, &right, &bottom);
	
	UpdateData(true);
	AddPattern(m_nId, fm, left, top, right, bottom, pitch, m_sComment);
}

void CDlgPattern::AddPattern(int nID, LPBYTE fm, int left, int top, int right, int bottom, int pitch, CString sComment)
{
	int i,j;
	CString str;
	int nWidth, nHeight;

	nWidth =right-left+1;
	nHeight=bottom-top+1;

	if(nID<0 || nID>=MAX_PATTERN)
	{
		str.Format(_T("Pattern Id Range is 0~%d"), MAX_PATTERN-1);
		AfxMessageBox(str);
		return;
	}
	if(nWidth<PM_MIN_SIZE || nHeight<PM_MIN_SIZE || nWidth>PM_MAX_SIZEX || nHeight>PM_MAX_SIZEY)
	{
		str.Format(_T("The range of pattern size is 24x24~%dx%d (%d, %d)"), PM_MAX_SIZEX, PM_MAX_SIZEY, nWidth, nHeight );
		AfxMessageBox(str);
		return;
	}

	if(g_Pattern[nID].m_nAddCount==0)
	{
		g_Pattern[nID].m_fm[0]=new BYTE[nWidth*nHeight];
		g_Pattern[nID].m_fm[1]=new BYTE[nWidth*nHeight];
		g_Pattern[nID].m_nWidth	=nWidth;
		g_Pattern[nID].m_nHeight=nHeight;
		g_Pattern[nID].m_dReturnPosX=nWidth/2.0;
		g_Pattern[nID].m_dReturnPosY=nHeight/2.0;
	}
	else
	{
		if(g_Pattern[nID].m_nWidth!=nWidth || g_Pattern[nID].m_nHeight!=nHeight)
		{
			str.Format(_T("먼저 등록된 패턴과 크기다 다릅니다. 기존것 지우고 다시 등록하세요"));
			AfxMessageBox(str);
			return;
		}
	}
	str.Format(_T("W:%d H:%d Return Pt:(%.1lf, %.1lf)"), g_Pattern[nID].m_nWidth, g_Pattern[nID].m_nHeight, g_Pattern[nID].m_dReturnPosX, g_Pattern[nID].m_dReturnPosY);
	GetDlgItem(IDC_EDIT_PATTERNINFO)->SetWindowTextW(str);

	wsprintf(g_Pattern[nID].m_sName, _T("%s"),sComment);

	for(i=top;i<=bottom;i++)
		for(j=left;j<=right;j++)
			*(g_Pattern[nID].m_fm[0]+nWidth*(i-top)+(j-left))=(*(g_Pattern[nID].m_fm[0]+nWidth*(i-top)+(j-left))*g_Pattern[nID].m_nAddCount+*(fm+pitch*i+j))/(g_Pattern[nID].m_nAddCount+1);

	g_Pattern[nID].m_nAddCount++;

	m_pDisplay->PutImageData(g_Pattern[nID].m_fm[0], 0,0, nWidth, nHeight, nWidth, 1);

	m_pDisplay->SetAddGraphic(true);
	m_pDisplay->DrawCross(NULL, g_Pattern[nID].m_dReturnPosX, g_Pattern[nID].m_dReturnPosY,  20, RGB(255,0,0));
}


void CDlgPattern::SelectPattern(int nID)
{
	CString str;
	m_sComment.Format(_T("%s"), g_Pattern[nID].m_sName);
	m_nMinScore=g_Pattern[nID].m_nMinScore;
	if(m_nMinScore<50) m_nMinScore=50;


	if(g_Pattern[nID].m_nAddCount)
	{
		m_pDisplay->m_nGraphicCount=0;
		m_pDisplay->ClearImage(0);
		m_pDisplay->SetAddGraphic(true);
		m_pDisplay->PutImageData(g_Pattern[nID].m_fm[0], 0,0, g_Pattern[nID].m_nWidth, g_Pattern[nID].m_nHeight, g_Pattern[nID].m_nWidth, 1);
		m_pDisplay->DrawCross(NULL, g_Pattern[nID].m_dReturnPosX, g_Pattern[nID].m_dReturnPosY,  20, RGB(255,0,0));

		str.Format(_T("W:%d H:%d Return Pt:(%.1lf, %.1lf)"), g_Pattern[nID].m_nWidth, g_Pattern[nID].m_nHeight, g_Pattern[nID].m_dReturnPosX, g_Pattern[nID].m_dReturnPosY);
		GetDlgItem(IDC_EDIT_PATTERNINFO)->SetWindowTextW(str);
	}
	else
	{
		m_pDisplay->m_nGraphicCount=0;
		m_pDisplay->ClearImage(0);

		GetDlgItem(IDC_EDIT_PATTERNINFO)->SetWindowTextW(_T(""));
	}
	g_Temp.m_nLastPatternID=nID;
	l_nReturnX=0;
	UpdateData(false);
}

void CDlgPattern::OnBnClickedButtonSearch()
{
	int left, top, right, bottom, pitch;
	double dX, dY, dScore;
	double start, end;
	int ret;
	CString str;
	UpdateData(true);
	if(m_nMinScore<50) m_nMinScore=50;

	start=Clock_set();

	((CKoWebView *)m_pParent)->m_pImage->m_pROI->GetROIRegion(_T("P"), &left, &top, &right, &bottom);

	ret=((CKoWebView *)m_pParent)->PatternMatch(m_nId, g_fmGrab[g_ID], left, top, right, bottom, g_System.m_nImageW, &dScore, &dX, &dY);

	end=Clock_set();

	m_ctrlResult.ResetContent();
	((CKoWebView *)m_pParent)->m_pImage->ResetGraphic();
	((CKoWebView *)m_pParent)->m_pImage->UpdateImage();

	if(ret==0 && dScore>=m_nMinScore) 
	{
		((CKoWebView *)m_pParent)->m_pImage->SetAddGraphic(true);
		((CKoWebView *)m_pParent)->m_pImage->DrawCross(NULL,(dX+0.5),(dY+0.5),10,RGB(255,0,0));

		str.Format(_T("Elapse Time: %.1lf msec"), end-start);	m_ctrlResult.AddString(str);	
		str.Format(_T("Score: %d"), (int)dScore);				m_ctrlResult.AddString(str);
		str.Format(_T("Pos:(%.2lf, %.2lf)"), dX, dY);			m_ctrlResult.AddString(str);
	}
	else
	{
		str.Format(_T("Matching Failure")); m_ctrlResult.AddString(str);
	}
	
	UpdateData(false);

}


void CDlgPattern::OnBnClickedCheckShowroi()
{
	UpdateData(true);
	int nX, nY;
	if(m_nROI)
	{
		GetDlgItem(IDC_BUTTON_REGISTER)->EnableWindow(1);
		GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(1);
		((CKoWebView *)m_pParent)->m_pImage->Local2Global(300, 300, &nX, &nY);	
		((CKoWebView *)m_pParent)->m_pImage->m_pROI->AddROI(_T("P"), nX-64, nY-64, nX+64, nY+64);
		((CKoWebView *)m_pParent)->m_pImage->m_pROI->EnableROI(true);
		((CKoWebView *)m_pParent)->m_pImage->m_pROI->ShowROI(true);
		((CKoWebView *)m_pParent)->m_pImage->UpdateImage();
	}
	else
	{
		GetDlgItem(IDC_BUTTON_REGISTER)->EnableWindow(0);
		GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(0);
		((CKoWebView *)m_pParent)->m_pImage->m_pROI->DeleteROI(_T("P"));
		((CKoWebView *)m_pParent)->m_pImage->UpdateImage();
	}
}


void CDlgPattern::OnBnClickedButtonApplyall()
{
	int i;

	if(AfxMessageBox(_T("All Pattern Apply?"), MB_YESNO)==IDYES)
	{
		UpdateData(true);
		for(i=0;i<MAX_PATTERN;i++)
			g_Pattern[i].m_nMinScore=m_nMinScore;
	}
}


void CDlgPattern::OnBnClickedOk()
{
	if(m_nROI)
	{
		((CKoWebView *)m_pParent)->m_pImage->m_pROI->DeleteROI(_T("P"));
		((CKoWebView *)m_pParent)->m_pImage->UpdateImage();
	}
	CDialogEx::OnOK();
}

