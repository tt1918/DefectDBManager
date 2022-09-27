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
	m_pWndEdit=NULL;
	m_nOldClickY=-1;
}

CDlgParam::~CDlgParam()
{
	if(m_pWndEdit)
	{
		delete m_pWndEdit;
	}
}


BOOL CDlgParam::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int i;
	WINDOWPLACEMENT wp;
	GetDlgItem(IDC_FRAME)->GetWindowPlacement(&wp);
	m_rcShow=wp.rcNormalPosition;
	m_nOffset=0;

	m_Color[0]=RGB(228,236,247);
	m_Color[1]=RGB(190,190,190);
	m_Color[2]=RGB(255,255,255);
	m_nX[0]=40;
	m_nX[1]=200;
	m_nX[2]=wp.rcNormalPosition.right-wp.rcNormalPosition.left;
	for(i=0;i<SHOW_Y_AREA+1;i++)
		m_nY[i]=wp.rcNormalPosition.top+(wp.rcNormalPosition.bottom-wp.rcNormalPosition.top)*i/SHOW_Y_AREA;

	SetScrollRange(SB_VERT , 0, NEW_MAX_COL_ALG_ITEM-SHOW_Y_AREA,  TRUE );

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
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CDlgParam 메시지 처리기입니다.

//적용 및 저장
void CDlgParam::OnBnClickedButtonSaveApply()
{
	UpdateEdit(true);
	UpdateSpread(true);

	ApplyParameter();

	SaveParameter();

	CDialogEx::OnOK();
}


//Apply
void CDlgParam::OnBnClickedOk()
{
	UpdateEdit(true);
	UpdateSpread(true);

	ApplyParameter();

	CDialogEx::OnOK();
}

//Cancel
void CDlgParam::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}


void CDlgParam::OnBnClickedButtonReadparameter()
{
	UpdateEdit(true);

	if(ReadParameter(0)==0)
	{
		CString str=g_Temp.m_sMyComName;
		if(g_Temp.m_sMyComName.Find(_T("NEL"))>=0)
		{
			g_Temp.m_sMyComName.Format(_T("COS-%d%02d"), g_Temp.m_nPCFirstNo, g_Temp.m_nPCNum);
		}
		else if(g_Temp.m_sMyComName.Find(_T("COS"))>=0)
		{
			g_Temp.m_sMyComName.Format(_T("NEL-%d%02d"), g_Temp.m_nPCFirstNo, g_Temp.m_nPCNum);
		}
		ReadParameter(0);
		g_Temp.m_sMyComName=str;

	}


	UpdateSpread(false);
	GetDlgItem(IDCANCEL)->EnableWindow(false);
}






void CDlgParam::OnPaint()
{
	CPaintDC dcORG(this); // device context for painting
	
	//MEMDC(설정)---------------------------------------------------------------
	CDC dc;			  
	CBitmap *pOldbmp;
    CBitmap *pOldbmpBac;
	CBitmap bitmap;
	dc.CreateCompatibleDC(&dcORG);
	bitmap.CreateCompatibleBitmap(&dcORG,  m_rcShow.Width(), m_rcShow.Height());
	pOldbmp = dc.SelectObject(&bitmap);
	//--------------------------------------------------------------------------

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(10,10,10));
	CFont *oldFont;
	CFont font;
	font.CreatePointFont(100, _T("Segoe UI Symbol"));
	oldFont=dc.SelectObject(&font);

	DrawData(dc);

	dc.SelectObject(oldFont);
	font.DeleteObject();

	//MEMDC(적용)-------------------------------------------------------------------------------------
	dcORG.BitBlt(m_rcShow.left, m_rcShow.top, m_rcShow.Width(), m_rcShow.Height(),&dc, 0,0 , SRCCOPY);
	dc.SelectObject(pOldbmp);
    dc.DeleteDC();
	//------------------------------------------------------------------------------------------------
}

void CDlgParam::DrawData(CDC &dc)
{
	int i;
	CBrush color0(m_Color[0]);
	CBrush color1(m_Color[1]);
	CBrush color2(m_Color[2]);
	CRect rc;
	CString str;
	int strlen;
	CPen newPen(PS_SOLID, 2, RGB(0,0,255)), *oldPen;

	//색깔 채우기----------------
	rc.left=0;
	rc.top=0;
	rc.right=m_nX[0];
	rc.bottom=m_nY[SHOW_Y_AREA];
	dc.FillRect(rc, &color0);

	rc.left=m_nX[0];
	rc.top=0;
	rc.right=m_nX[1];
	rc.bottom=m_nY[SHOW_Y_AREA];
	dc.FillRect(rc, &color1);

	rc.left=m_nX[1];
	rc.top=0;
	rc.right=m_nX[2];
	rc.bottom=m_nY[SHOW_Y_AREA];
	dc.FillRect(rc, &color2);
	//---------------------------

	for(i=1;i<SHOW_Y_AREA;i++)
	{
		dc.MoveTo(0,m_nY[i]);
		dc.LineTo(m_nX[2], m_nY[i]);
	}

	dc.MoveTo(m_nX[0],0);
	dc.LineTo(m_nX[0], m_nY[SHOW_Y_AREA]);
	dc.MoveTo(m_nX[1],0);
	dc.LineTo(m_nX[1], m_nY[SHOW_Y_AREA]);

	//----------------------------------
	oldPen=dc.SelectObject(&newPen);
	for(i=0;i<SHOW_Y_AREA;i++)
	{
		if((i+m_nOffset)%10==0)
		{
			dc.MoveTo(0,m_nY[i]+1);
			dc.LineTo(m_nX[2], m_nY[i]+1);
		}		
	}
	dc.SelectObject(&oldPen);
	//----------------------------------


	for(i=0;i<SHOW_Y_AREA;i++)
	{
		if(i+1+m_nOffset> NEW_MAX_COL_ALG_ITEM) break;

		str.Format(_T("%d"),i+1+m_nOffset);
		rc.left=0;
		rc.right=m_nX[0];
		rc.top=m_nY[i]+1;
		rc.bottom=m_nY[i+1];
		dc.DrawText(str, str.GetLength(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

		rc.left=m_nX[0];
		rc.right=m_nX[1];
		rc.top=m_nY[i]+1;
		rc.bottom=m_nY[i+1];
		strlen=_tcsclen(g_ParamName[i+m_nOffset]);
		dc.DrawText(g_ParamName[i+m_nOffset], strlen, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

		rc.left=m_nX[1];
		rc.right=m_nX[2];
		rc.top=m_nY[i]+1;
		rc.bottom=m_nY[i+1];

		if((i+m_nOffset)==57 )								str.Format(_T("%.7lf"), m_ParamVer.param[i+m_nOffset]);
		else if((i+m_nOffset)==58 || (i+m_nOffset)==59)		str.Format(_T("%.5lf"), m_ParamVer.param[i+m_nOffset]);
		else												str.Format(_T("%.3lf"), m_ParamVer.param[i+m_nOffset]);
		dc.DrawText(str, str.GetLength(), rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	
}

void CDlgParam::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nStepY=20;
	int nOld=m_nOffset;

	UpdateEdit(true);

	switch(nSBCode)
	{
		case SB_LINELEFT: // 왼쪽 끝의 버튼을 누른경우
     			SetScrollPos(SB_VERT,(int)(GetScrollPos(SB_VERT)-1)); break;
		case SB_LINERIGHT: // 오른쪽 끝을 누른경우
     			SetScrollPos(SB_VERT,(int)(GetScrollPos(SB_VERT)+1)); break;
		case SB_PAGELEFT: // 왼쪽 몸통부분을 누른경우
     			SetScrollPos(SB_VERT,GetScrollPos(SB_VERT)-nStepY); break;
		case SB_PAGERIGHT: // 오른쪽 몸통부분을 누른경우
     			SetScrollPos(SB_VERT,GetScrollPos(SB_VERT)+nStepY); break;
		case SB_THUMBTRACK: // 절대적인 위치로 스크롤된경우
     			SetScrollPos(SB_VERT,nPos); break;

	}
	m_nOffset=GetScrollPos(SB_VERT);

	if(nOld!=m_nOffset)
		InvalidateRect(m_rcShow, false);

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CDlgParam::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	static int nOldOffset=-1;
	int nOffset;
	
	nOffset=m_nOffset+(zDelta/(-25));
	if(nOffset<0) nOffset=0;
	else if(nOffset>NEW_MAX_COL_ALG_ITEM-SHOW_Y_AREA) nOffset=NEW_MAX_COL_ALG_ITEM-SHOW_Y_AREA;
	
	if(nOffset!=nOldOffset)
	{		
		UpdateEdit(true);
		m_nOffset=nOldOffset=nOffset;

		InvalidateRect(m_rcShow, false);		
		SetScrollPos(SB_VERT, m_nOffset);
	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


UINT _GetNewControlID(CWnd *pWnd);
void CDlgParam::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	double dGap=(double)(m_rcShow.bottom-m_rcShow.top)/SHOW_Y_AREA;
	CString str;

	if(point.x>m_nX[1]  && point.y>m_rcShow.top && point.y<m_rcShow.bottom)
	{
		m_nClickY=(int)(point.y/dGap);

		if(m_nOldClickY!=m_nClickY) UpdateEdit(true);
		else					    return;

		UpdateEdit(false);

		m_nOldClickY=m_nClickY;
	}
	else
	{
		UpdateEdit(true);
	}


	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDlgParam::UpdateEdit(bool flag)
{
	CString str;
	int N;
	UINT uID;

	if(flag) //CEdit에 있는 데이타를 m_ParamVer.param에 넣고 CEdit 없앰.
	{		
		if(m_pWndEdit && m_nOldClickY>=0)
		{
			N=m_nOldClickY;
			((CEdit*)m_pWndEdit)->GetWindowText(str);
			m_ParamVer.param[N+m_nOffset]=_tstof(str);		

			delete m_pWndEdit;
			m_pWndEdit=NULL;

			rcControl.top-=2;    if(rcControl.top<m_rcShow.top) rcControl.top=m_rcShow.top;
			rcControl.bottom+=2; if(rcControl.bottom>m_rcShow.bottom)  rcControl.bottom=m_rcShow.bottom;

			InvalidateRect(rcControl, false);
		}
	}
	else   //m_ParamVer.param에 있는 데이타를 CEdit에 넣음
	{
		N=m_nClickY;
		m_pWndEdit = new CEdit();
		uID=_GetNewControlID(this);

		rcControl.left=m_nX[1]+1;
		rcControl.right=m_nX[2]-1;
		rcControl.top=m_nY[N]+2;
		rcControl.bottom=m_nY[N+1]-1;

		((CEdit*)m_pWndEdit)->Create(WS_VISIBLE, rcControl, this, uID);
		

		if((N+m_nOffset)==57 )								str.Format(_T("%.7lf"), m_ParamVer.param[N+m_nOffset]);
		else if((N+m_nOffset)==58 || (N+m_nOffset)==59)		str.Format(_T("%.5lf"), m_ParamVer.param[N+m_nOffset]);
		else												str.Format(_T("%.3lf"), m_ParamVer.param[N+m_nOffset]);
	
		((CEdit*)m_pWndEdit)->SetWindowText(str);
	}
}

void CDlgParam::UpdateSpread(bool flag)
{	
	//string unicode추가 YKS 20200807
	if(!flag)
	{
		memcpy(m_ParamVer.ComName,		g_ParamVer.ComName,		sizeof(g_ParamVer.ComName));
		memcpy(m_ParamVer.param,		g_ParamVer.param,		NEW_MAX_COL_ALG_ITEM*sizeof(float));
		memcpy(m_ParamVer.system,		g_ParamVer.system,		8*sizeof(float));
		memcpy(m_ParamVer.nCycle,		g_ParamVer.nCycle,		15*sizeof(int));
		m_ParamVer.m_sModelName		=	g_ParamVer.m_sModelName;
		m_ParamVer.m_strMachineName	=	g_ParamVer.m_strMachineName;
				
		Invalidate();	//화면에 씀
	}
	else
	{
		memcpy(g_ParamVer.ComName,		m_ParamVer.ComName,		sizeof(g_ParamVer.ComName));
		memcpy(g_ParamVer.param,		m_ParamVer.param,		NEW_MAX_COL_ALG_ITEM*sizeof(float));
		memcpy(g_ParamVer.system,		m_ParamVer.system,		8*sizeof(float));
		memcpy(g_ParamVer.nCycle,		m_ParamVer.nCycle,		15*sizeof(int));
		g_ParamVer.m_sModelName		=	m_ParamVer.m_sModelName;
		g_ParamVer.m_strMachineName	=	m_ParamVer.m_strMachineName;

		//Grid에 있는 데이타로 system데이타를 만듦(지우면 안됨)---
		g_ParamVer.system[0]=g_ParamVer.param[39];
		g_ParamVer.system[1]=g_ParamVer.param[55];
		g_ParamVer.system[2]=g_ParamVer.param[56];
		g_ParamVer.system[3]=g_ParamVer.param[57];
		g_ParamVer.system[4]=g_ParamVer.param[58];
		g_ParamVer.system[5]=g_ParamVer.param[59];
		//---------------------------------------------------------
	}
}


//Resource ID 얻는 함수
UINT _GetNewControlID(CWnd *pWnd)
{
	#define _MAXNUM5CHILDWINDOWS_INTHECLISTCTRL		0x1000
	#define _MAXID5CHILDWINDOW_INTHECLISTCTRL		0x7fff
	#define _MINID5CHILDWINDOW_INTHECLISTCTRL		0x7000

	static unsigned long ulHoldRand = 1;
	if (ulHoldRand == 1) ulHoldRand = (unsigned long)::time(NULL);
	UINT uID;
	do
	{
		uID = _MAXID5CHILDWINDOW_INTHECLISTCTRL - ((unsigned short)
			((((ulHoldRand = ulHoldRand * 0x343fd + 0x269ec3)	>> 16) * _MAXNUM5CHILDWINDOWS_INTHECLISTCTRL) >> 16));
	} while (NULL != pWnd->GetDlgItem(uID));
	return uID;
}


