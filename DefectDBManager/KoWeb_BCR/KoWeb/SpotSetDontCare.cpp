// SpotSetDontCare.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "SpotSetDontCare.h"
#include "afxdialogex.h"

CROIControl g_ROISpot;

// CSpotSetDontCare 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSpotSetDontCare, CDialogEx)

CSpotSetDontCare::CSpotSetDontCare(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSpotSetDontCare::IDD, pParent)
{
	m_pDisplay=NULL;
	m_fm=NULL;
	m_nThreshold1 = 0;
	m_nThreshold2 = 0;
	m_nInverse=0;
}

CSpotSetDontCare::~CSpotSetDontCare()
{
	if(m_pDisplay) delete m_pDisplay;
	g_ROISpot.Close();
}

void CSpotSetDontCare::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_ROI, m_nROI);
	DDX_Check(pDX, IDC_CHECK_ROI2, m_MultiLine);
	DDX_Check(pDX, IDC_CHECK_ROI3, m_GrayValue);
	DDX_Text(pDX, IDC_EDIT_GRAYTH0, m_nThreshold1);
	DDX_Text(pDX, IDC_EDIT_GRAYTH1, m_nThreshold2);
	DDX_Control(pDX, IDC_COMBO1, m_ctrlZoom);
	DDX_Check(pDX, IDC_CHECK_INVERSE, m_nInverse);
}


BEGIN_MESSAGE_MAP(CSpotSetDontCare, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CSpotSetDontCare::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_CHECK_ROI, &CSpotSetDontCare::OnBnClickedCheckRoi)
	ON_BN_CLICKED(IDC_CHECK_ROI2, &CSpotSetDontCare::OnBnClickedCheckRoi2)
	ON_BN_CLICKED(IDC_CHECK_ROI3, &CSpotSetDontCare::OnBnClickedCheckRoi3)
	ON_BN_CLICKED(IDC_CHECK_INVERSE, &CSpotSetDontCare::OnBnClickedCheckInverse)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CSpotSetDontCare::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_ADD_ALL, &CSpotSetDontCare::OnBnClickedButtonAddAll)
	ON_BN_CLICKED(IDC_BUTTON_ERASE, &CSpotSetDontCare::OnBnClickedButtonErase)
	ON_BN_CLICKED(IDC_BUTTON_ERASE_ALL, &CSpotSetDontCare::OnBnClickedButtonEraseAll)
	ON_BN_CLICKED(IDC_BUTTON_DILATE, &CSpotSetDontCare::OnBnClickedButtonDilate)
	ON_BN_CLICKED(IDC_BUTTON_ERODE, &CSpotSetDontCare::OnBnClickedButtonErode)
	ON_BN_CLICKED(IDOK, &CSpotSetDontCare::OnBnClickedOk)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CSpotSetDontCare 메시지 처리기입니다.
void CSpotSetDontCare::SetImageInfo(int nSet, int nId, LPBYTE fm, int pitch)
{		
	int i,j;
	CRect rc=g_Inspect[nSet].m_ROI1[nId].rc;

	m_nSet=nSet;
	m_nID =nId;
	if(g_Inspect[nSet].m_InspSpot[nId].m_bUseMask==0)
	{
		m_nWidth=rc.right-rc.left;
		m_nHeight=rc.bottom-rc.top;	
		//g_Inspect.m_Dust[nId].m_UseMask=1;
		g_Inspect[nSet].m_InspSpot[nId].m_fmOrg=new BYTE[m_nWidth*m_nHeight];
		g_Inspect[nSet].m_InspSpot[nId].m_fmMask=new BYTE[m_nWidth*m_nHeight];
		if(g_Inspect[nSet].m_InspSpot[nId].m_fmOrg==NULL || g_Inspect[nSet].m_InspSpot[nId].m_fmMask==NULL)
		{
			AfxMessageBox(_T("Memory Alloc Error (at SetImageInfo)"));
		}
		g_Inspect[nSet].m_InspSpot[nId].m_nMaskWidth =m_nWidth;
		g_Inspect[nSet].m_InspSpot[nId].m_nMaskHeight=m_nHeight;

		m_fm=g_Inspect[nSet].m_InspSpot[nId].m_fmOrg;
		m_fmMask=g_Inspect[nSet].m_InspSpot[nId].m_fmMask;

		for(i=rc.top;i<rc.bottom;i++)
			for(j=rc.left;j<rc.right;j++)
				*(m_fm+m_nWidth*(i-rc.top)+j-rc.left)=*(fm+pitch*i+j);

		memset(m_fmMask, 0, m_nWidth*m_nHeight);
	}
	else
	{
		m_fm     =g_Inspect[nSet].m_InspSpot[nId].m_fmOrg;
		m_fmMask =g_Inspect[nSet].m_InspSpot[nId].m_fmMask;
		m_nWidth =g_Inspect[nSet].m_InspSpot[nId].m_nMaskWidth;
		m_nHeight=g_Inspect[nSet].m_InspSpot[nId].m_nMaskHeight;
	}
	g_ROISpot.Initialize(m_nWidth, m_nHeight);
}

BOOL CSpotSetDontCare::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int i,j;
	WINDOWPLACEMENT wp;
	GetDlgItem(IDC_GRAPH)->GetWindowPlacement(&wp);
	m_rcGraph=wp.rcNormalPosition;

	if(m_fm==NULL) return FALSE;

	OpenDisplay(m_fm, m_nWidth, m_nHeight, CRect(0,0,512,512));
	m_ctrlZoom.SetCurSel(0);
	m_pDisplay->PutImageData(m_fm, m_nWidth,m_nHeight, 1); 

	for(i=0;i<m_nHeight;i++)
		for(j=0;j<m_nWidth;j++)
			*(m_pDisplay->m_fmMask+m_nWidth*i+j)=*(m_fmMask+m_nWidth*i+j);

	m_pDisplay->SetShowMask(1);
	m_pDisplay->UpdateImage();
	m_pDisplay->FitZoomXY();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
void CSpotSetDontCare::OpenDisplay(LPBYTE fm, int nWidth, int nHeight, CRect rcDisp)
{
	int i;
	CString str;
	
	if(m_pDisplay==NULL)
	{
		m_pDisplay=new CCossImage(this);
		m_pDisplay->IMAGE_COLOR=1;
		m_pDisplay->m_UseMask=1;
		m_pDisplay->m_nShowMask=0;
		m_pDisplay->SetImageData(nWidth, nHeight);
		m_pDisplay->m_pROI=&g_ROISpot;
		m_pDisplay->m_fmProcess=fm;

		m_pDisplay->m_nDisplayID=0;
		str.Format(_T("Spot"));
		m_pDisplay->SetTitle(str);
		str.Format(_T(""));
		m_pDisplay->SetImageName(str);
		m_pDisplay->Create(IDD_DIALOG_COSSIMAGE,this);
		m_pDisplay->MoveWindow(rcDisp);
		m_pDisplay->FitZoomXY();
		m_pDisplay->ShowWindow(SW_SHOWNORMAL);
	}
}
int CSpotSetDontCare::GetHistogram(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, int *Histo)
{
	int i,j;
	int nHighThreshold;
	const int HISTOLEVEL=256;

	//초기화
	memset(Histo, 0, HISTOLEVEL*sizeof(int));

	//Histogram Data 구하기
	for(i=y1;i<y2;i++)
		for(j=x1;j<x2;j++)
			Histo[*(fm+i*pitch+j)]++;

	//제일 큰값을 찾는다.
    nHighThreshold=1;
	for(i=0;i<HISTOLEVEL;i++)
		if(Histo[i]>nHighThreshold) nHighThreshold=Histo[i];

	return nHighThreshold;
}
void CSpotSetDontCare::Binary(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nTh1, int nTh2, int nInverse)
{
	int i,j;

	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
		{
			if(*(fmS+pitch*i+j)>=nTh1 && *(fmS+pitch*i+j)<=nTh2) 
				*(fmD+pitch*i+j)=200-200*nInverse;
			else
				*(fmD+pitch*i+j)=200*nInverse;
		}
}


void CSpotSetDontCare::OnCbnSelchangeCombo1()
{
	int nIndex = m_ctrlZoom.GetCurSel();
	switch(nIndex)
	{
		case 0: m_pDisplay->FitZoomXY();	m_pDisplay->FitZoomXY();	break;
		case 1: m_pDisplay->Zoom(0.25, 0.25);	break;
		case 2: m_pDisplay->Zoom(0.5, 0.5);		break;
		case 3: m_pDisplay->Zoom(1, 1);			break;
		case 4: m_pDisplay->Zoom(2, 2);			break;
		case 5: m_pDisplay->Zoom(4, 4);		 break;
	}	
}


void CSpotSetDontCare::OnBnClickedCheckRoi()
{
	int nX, nY, nX1, nY1;

	UpdateData(true);

	m_MultiLine=0;
	((CButton*)GetDlgItem(IDC_CHECK_ROI2))->SetCheck(m_MultiLine);

	if(m_nROI)
	{
		m_pDisplay->Local2Global(512/2, 512/2, &nX, &nY);	
		m_pDisplay->Local2Global(512/2+128, 512/2+128, &nX1, &nY1);	
		m_pDisplay->m_pROI->AddROI(_T("R"), nX-(nX1-nX)/2, nY-(nY1-nY)/2, nX+(nX1-nX)/2, nY+(nY1-nY)/2);
		m_pDisplay->m_pROI->EnableROI(true);
		m_pDisplay->m_pROI->ShowROI(true);
		m_nPtCount=0;
		m_pDisplay->SetAddGraphic(true);
		m_pDisplay->UpdateImage();
	}
	else
	{
		m_pDisplay->m_pROI->DeleteROI(_T("R"));
		m_pDisplay->UpdateImage();
	}
}


void CSpotSetDontCare::OnBnClickedCheckRoi2()
{
	UpdateData(true);
	m_nROI=0;
	((CButton*)GetDlgItem(IDC_CHECK_ROI))->SetCheck(m_nROI);


	m_pDisplay->m_pROI->DeleteROI(_T("R"));
	m_nPtCount=0;	
	m_pDisplay->SetAddGraphic(true);
	m_pDisplay->UpdateImage();
}


void CSpotSetDontCare::OnBnClickedCheckRoi3()
{
	int i,j;
	int left,top, right, bottom;
	UpdateData(true);

	((CButton*)GetDlgItem(IDC_CHECK_INVERSE))->EnableWindow(m_GrayValue);
	
	if(m_nROI)
	{
		m_pDisplay->m_pROI->GetROIRegion(_T("R"), &left, &top, &right, &bottom);		
	}
	else
	{
		left=0; top=0;right=m_nWidth;bottom=m_nHeight;
	}

	m_MaxHisto=GetHistogram(m_fm, left,top, right, bottom, m_nWidth,m_Histo);

	InvalidateRect(&m_rcGraph);

	if(m_GrayValue)
	{
		Binary(m_fm, m_pDisplay->m_fmDisplay, left,top, right, bottom, m_nWidth, m_nThreshold1, m_nThreshold2, m_nInverse);
	}
	else
	{
		for(i=0;i<m_nHeight;i++)
			memcpy(m_pDisplay->m_fmDisplay+m_nWidth*i, m_fm+m_nWidth*i, m_nWidth);
	}
	m_pDisplay->SetAddGraphic(true);
	m_pDisplay->UpdateImage();
}


void CSpotSetDontCare::OnBnClickedCheckInverse()
{
	m_nInverse=1-m_nInverse;

	int left,top, right, bottom;
			
	if(m_nROI)
	{
		m_pDisplay->m_pROI->GetROIRegion(_T("R"), &left, &top, &right, &bottom);		
	}
	else
	{
		left=0; top=0;right=m_nWidth;bottom=m_nHeight;
	}

	Binary(m_fm, m_pDisplay->m_fmDisplay, left,top, right, bottom, m_nWidth, m_nThreshold1, m_nThreshold2, m_nInverse);	m_pDisplay->UpdateImage();
	InvalidateUserDefine();
}


void CSpotSetDontCare::OnBnClickedButtonAdd()
{
	int i,j, left,top, right, bottom;
	LPBYTE fm=m_pDisplay->m_fmMask;

	if(m_nROI || m_MultiLine)
	{
		 if(m_nROI)
		 {
			m_pDisplay->m_pROI->GetROIRegion(_T("R"), &left, &top, &right, &bottom);
	
			if(right>m_nWidth)   right =m_nWidth;
			if(bottom>m_nHeight) bottom=m_nHeight;

			
			for(i=top;i<bottom;i++)
				for(j=left;j<right;j++)
				{
					if(!m_GrayValue) *(fm+m_nWidth*i+j)=1;		
					else
					{
						if(*(m_pDisplay->m_fmDisplay+m_nWidth*i+j)==200) *(fm+m_nWidth*i+j)=1;
					}			
				}
		 }
		 else if(m_MultiLine)
		 {
			left=100000;
			top=100000;
			right=0;
			bottom=0;
			for(i=0;i<m_nPtCount;i++)
			{
				if(m_pt[i].x<left)	left	=m_pt[i].x;
				if(m_pt[i].y<top)	top		=m_pt[i].y;
				if(m_pt[i].x>right) right	=m_pt[i].x;
				if(m_pt[i].y>bottom)bottom	=m_pt[i].y;
			}
			for(i=top;i<bottom;i++)
				for(j=left;j<right;j++)
				{
					if(PtInPolygon(m_pt, m_nPtCount, j, i))
					{
						if(!m_GrayValue) *(fm+m_nWidth*i+j)=1;		
						else
						{
							if(*(m_pDisplay->m_fmDisplay+m_nWidth*i+j)==200) *(fm+m_nWidth*i+j)=1;
						}
					}
				}
		 }
	}
	else if(m_GrayValue)
	{
		for(i=0;i<m_nHeight;i++)
			for(j=0;j<m_nWidth;j++)
				if(*(m_pDisplay->m_fmDisplay+m_nWidth*i+j)==200)
				{
					*(fm+m_nWidth*i+j)=1;
				}
	}

	m_pDisplay->UpdateImage();
}


void CSpotSetDontCare::OnBnClickedButtonAddAll()
{
	int i,j;
	LPBYTE fm=m_pDisplay->m_fmMask;

	for(i=0;i<m_nHeight;i++)
		for(j=0;j<m_nWidth;j++)
		{
			*(fm+m_nWidth*i+j)=1;					
		}
	
	m_pDisplay->UpdateImage();
}


void CSpotSetDontCare::OnBnClickedButtonErase()
{
	int i,j, left,top, right, bottom;
	LPBYTE fm=m_pDisplay->m_fmMask;

	if(m_nROI || m_MultiLine)
	{
		 if(m_nROI)
		 {
			m_pDisplay->m_pROI->GetROIRegion(_T("R"), &left, &top, &right, &bottom);
	
			if(right>m_nWidth)   right =m_nWidth;
			if(bottom>m_nHeight) bottom=m_nHeight;

			
			for(i=top;i<bottom;i++)
				for(j=left;j<right;j++)
				{
					if(!m_GrayValue) *(fm+m_nWidth*i+j)=0;		
					else
					{
						if(*(m_pDisplay->m_fmDisplay+m_nWidth*i+j)==200) *(fm+m_nWidth*i+j)=0;
					}			
				}
		 }
		 else if(m_MultiLine)
		 {
			left=100000;
			top=100000;
			right=0;
			bottom=0;
			for(i=0;i<m_nPtCount;i++)
			{
				if(m_pt[i].x<left)	left	=m_pt[i].x;
				if(m_pt[i].y<top)	top		=m_pt[i].y;
				if(m_pt[i].x>right) right	=m_pt[i].x;
				if(m_pt[i].y>bottom)bottom	=m_pt[i].y;
			}
			for(i=top;i<bottom;i++)
				for(j=left;j<right;j++)
				{
					if(PtInPolygon(m_pt, m_nPtCount, j, i))
					{
						if(!m_GrayValue) *(fm+m_nWidth*i+j)=0;		
						else
						{
							if(*(m_pDisplay->m_fmDisplay+m_nWidth*i+j)==200) *(fm+m_nWidth*i+j)=0;
						}
					}
				}
		 }
	}
	else if(m_GrayValue)
	{
		for(i=0;i<m_nHeight;i++)
			for(j=0;j<m_nWidth;j++)
				if(*(m_pDisplay->m_fmDisplay+m_nWidth*i+j)==200)
				{
					*(fm+m_nWidth*i+j)=0;
				}
	}
	m_pDisplay->UpdateImage();
}


void CSpotSetDontCare::OnBnClickedButtonEraseAll()
{
	int i,j;
	LPBYTE fm=m_pDisplay->m_fmMask;

	for(i=0;i<m_nHeight;i++)
		for(j=0;j<m_nWidth;j++)
		{
			*(fm+m_nWidth*i+j)=0;					
		}
	
	m_pDisplay->UpdateImage();
}


void CSpotSetDontCare::OnBnClickedButtonDilate()
{
	LPBYTE fm=m_pDisplay->m_fmMask;

	int i, j;
	int left,top, right, bottom;
			
	if(m_nROI)
	{
		m_pDisplay->m_pROI->GetROIRegion(_T("R"), &left, &top, &right, &bottom);		
	}
	else
	{
		left=0; top=0;right=m_nWidth-1;bottom=m_nHeight-1;
	}
	Dilate_Gray(fm, g_fmTemp, left, top, right, bottom, m_nWidth,  NULL,  3,3);

	for(i=top+1;i<bottom-1;i++)
		for(j=left+1;j<right-1;j++)
			*(fm+m_nWidth*i+j)=*(g_fmTemp+m_nWidth*i+j);

	m_pDisplay->UpdateImage();
}


void CSpotSetDontCare::OnBnClickedButtonErode()
{
	LPBYTE fm=m_pDisplay->m_fmMask;

	int i, j;
	int left,top, right, bottom;
			
	if(m_nROI)
	{
		m_pDisplay->m_pROI->GetROIRegion(_T("R"), &left, &top, &right, &bottom);		
	}
	else
	{
		left=0; top=0;right=m_nWidth-1;bottom=m_nHeight-1;
	}
	Erode_Gray(fm, g_fmTemp, left, top, right, bottom, m_nWidth,  NULL,  3,3);
	for(i=top+1;i<bottom-1;i++)
		for(j=left+1;j<right-1;j++)
			*(fm+m_nWidth*i+j)=*(g_fmTemp+m_nWidth*i+j);

	m_pDisplay->UpdateImage();
}


void CSpotSetDontCare::OnBnClickedOk()
{
	memcpy(m_fmMask, m_pDisplay->m_fmMask, m_nWidth*m_nHeight); 
	int i, j, nMask=0;

	for(i=0;i<m_nHeight;i++)
		for(j=0;j<m_nWidth;j++)
			if(*(m_fmMask+m_nWidth*i+j)) 
			{
				nMask=1;
				break;
			}

	g_Inspect[m_nSet].m_InspSpot[m_nID].m_bUseMask=nMask;
	if(m_nROI) m_pDisplay->m_pROI->DeleteROI(_T("R"));
	if(nMask==0)
	{
		delete g_Inspect[m_nSet].m_InspSpot[m_nID].m_fmOrg;
		delete g_Inspect[m_nSet].m_InspSpot[m_nID].m_fmMask;
	}

	CDialogEx::OnOK();
}
void CSpotSetDontCare::InvalidateUserDefine()
{
	RECT rc;
	rc.left=m_rcGraph.left-10;
	rc.top=m_rcGraph.top-10;
	rc.right=m_rcGraph.right+10;
	rc.bottom=m_rcGraph.bottom+10;

	
	InvalidateRect(&rc,true);
}

void CSpotSetDontCare::OnLButtonDown(UINT nFlags, CPoint point)
{
	int nX, nY;
	if(m_MultiLine)
	{
		m_pDisplay->Local2Global(point.x, point.y, &nX, &nY);
		
		if(nX<m_nWidth && nY<m_nHeight)
		{
			m_pDisplay->Local2Global(point.x, point.y, &nX, &nY);
			m_pDisplay->DrawCross(NULL, nX, nY, 10, RGB(0,255,255));
			m_pt[m_nPtCount].x=nX;
			m_pt[m_nPtCount].y=nY;
			if(m_nPtCount<50) m_nPtCount++;
			if(m_nPtCount>1) 
				m_pDisplay->DrawLine(NULL, m_pt[m_nPtCount-2].x, m_pt[m_nPtCount-2].y, m_pt[m_nPtCount-1].x, m_pt[m_nPtCount-1].y, RGB(0,255,255));
		//m_nPtCount
		}
	}

	if(m_GrayValue)
	{
		if(m_rcTh1.PtInRect(point)) m_SelectThreshold=1;
		if(m_rcTh2.PtInRect(point)) m_SelectThreshold=2;
		m_SelectPoint=point;
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CSpotSetDontCare::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_SelectThreshold=0;

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CSpotSetDontCare::OnRButtonDown(UINT nFlags, CPoint point)
{
	int i, nX, nY;
	if(m_MultiLine && m_nPtCount>0)
	{
		m_pDisplay->SetAddGraphic(true);

		m_nPtCount--;
		for(i=0;i<m_nPtCount;i++)
		{
			m_pDisplay->DrawCross(NULL, m_pt[i].x, m_pt[i].y, 10, RGB(0,255,255));
			if(m_nPtCount>1 && i>0) 
				m_pDisplay->DrawLine(NULL, m_pt[i-1].x, m_pt[i-1].y, m_pt[i].x, m_pt[i].y, RGB(0,255,255));
		}
		m_pDisplay->UpdateImage();		
	}

	CDialogEx::OnRButtonDown(nFlags, point);
}


void CSpotSetDontCare::OnMouseMove(UINT nFlags, CPoint point)
{
	int dist;
	if(m_SelectThreshold==1 || m_SelectThreshold==2)
	{
		dist=(m_SelectPoint.x-point.x)*255/(m_rcGraph.right-m_rcGraph.left);
		if(m_SelectThreshold==1)
		{
			m_nThreshold1-=dist;
			point.x=m_rcGraph.left+m_nThreshold1*(m_rcGraph.right-m_rcGraph.left)/255;
		}
		else
		{
			m_nThreshold2-=dist;
			point.x=m_rcGraph.left+m_nThreshold2*(m_rcGraph.right-m_rcGraph.left)/255;
		}
		
		//Data Check : Thres1이 Thres2보다 클수 없다.		
		if(m_nThreshold1>m_nThreshold2)
		{
			if(m_SelectThreshold==1) m_nThreshold1+=dist;
			else				     m_nThreshold2+=dist;
		}

		// Data Check : Thres1,2는 0보다 작지않고 255보다 크지않다.
		if(m_nThreshold1<0) m_nThreshold1=0;
		else if(m_nThreshold1>255) m_nThreshold1=255;
		if(m_nThreshold2<0) m_nThreshold2=0;
		else if(m_nThreshold2>255) m_nThreshold2=255;


		int left,top, right, bottom;
			
		if(m_nROI)
		{
			m_pDisplay->m_pROI->GetROIRegion(_T("R"), &left, &top, &right, &bottom);		
		}
		else
		{
			left=0; top=0;right=m_nWidth;bottom=m_nHeight;
		}

		Binary(m_fm, m_pDisplay->m_fmDisplay, left,top, right, bottom, m_nWidth, m_nThreshold1, m_nThreshold2, m_nInverse);
		m_pDisplay->UpdateImage();


		m_SelectPoint=point;
		UpdateData(FALSE);
		InvalidateUserDefine();
	//	UpdateLUT();
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CSpotSetDontCare::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//조그만 사각형위치 설정.
	m_rcTh1.left=m_rcGraph.left+m_nThreshold1*(m_rcGraph.right-m_rcGraph.left)/255-5;
	m_rcTh1.right=m_rcTh1.left+10;
	m_rcTh1.top=(m_rcGraph.bottom+m_rcGraph.top)/2-5;
	m_rcTh1.bottom=m_rcTh1.top+10;
	//조그만 사각형위치 설정.
	m_rcTh2.left=m_rcGraph.left+m_nThreshold2*(m_rcGraph.right-m_rcGraph.left)/255-5;
	m_rcTh2.right=m_rcTh2.left+10;
	m_rcTh2.top=(m_rcGraph.bottom+m_rcGraph.top)/2-5;
	m_rcTh2.bottom=m_rcTh2.top+10;

	dc.Rectangle(&m_rcGraph);

	int nWidth =m_rcGraph.right-m_rcGraph.left;
	int nHeight=m_rcGraph.bottom-m_rcGraph.top;
	int i,x,y;

//{BEGIN: Histogram 그리기 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	if(m_MaxHisto>0)
	{
		for(i=0;i<256;i++)
		{
			x=m_rcGraph.left+1+i*nWidth/255;
			if(x>m_rcGraph.right-2) x=m_rcGraph.right-2;
			y=m_rcGraph.bottom-2-nHeight*m_Histo[i]/m_MaxHisto;
			if(y<m_rcGraph.top) y=m_rcGraph.top;
			dc.MoveTo(x,m_rcGraph.bottom-2);
			dc.LineTo(x,y);
		}
	}

//}END<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	CPen *pBluePen=new CPen(PS_SOLID,0,RGB(0,0,255));
	CPen *pRedPen=new CPen(PS_SOLID,0,RGB(255,0,0));
	CPen *pOldPen;
	pOldPen=dc.SelectObject(pBluePen);

	if(m_nInverse==0)
	{
		dc.MoveTo(m_rcGraph.left,m_rcGraph.bottom-2);
		dc.LineTo(m_rcGraph.left+m_nThreshold1*nWidth/255,m_rcGraph.bottom-2);
		dc.LineTo(m_rcGraph.left+m_nThreshold1*nWidth/255,m_rcGraph.top+1);
		dc.LineTo(m_rcGraph.left+m_nThreshold2*nWidth/255,m_rcGraph.top+1);
		dc.LineTo(m_rcGraph.left+m_nThreshold2*nWidth/255,m_rcGraph.bottom-2);
		dc.LineTo(m_rcGraph.right,m_rcGraph.bottom-2);
	}
	else
	{
		dc.MoveTo(m_rcGraph.left,m_rcGraph.top+1);
		dc.LineTo(m_rcGraph.left+m_nThreshold1*nWidth/255,m_rcGraph.top+1);
		dc.LineTo(m_rcGraph.left+m_nThreshold1*nWidth/255,m_rcGraph.bottom-2);
		dc.LineTo(m_rcGraph.left+m_nThreshold2*nWidth/255,m_rcGraph.bottom-2);
		dc.LineTo(m_rcGraph.left+m_nThreshold2*nWidth/255,m_rcGraph.top+1);
		dc.LineTo(m_rcGraph.right,m_rcGraph.top+1);
	}			

	dc.SelectObject(pRedPen);
	dc.Rectangle(m_rcTh1);
	dc.Rectangle(m_rcTh2);

	dc.SelectObject(pOldPen);
	delete pBluePen;
	delete pRedPen;
}