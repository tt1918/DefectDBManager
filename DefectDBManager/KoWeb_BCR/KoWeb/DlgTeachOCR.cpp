// DlgTeachOCR.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "DlgTeachOCR.h"
#include "afxdialogex.h"

#include "KoWebDoc.h"
#include "KoWebView.h"


CROIControl g_OCRPattern;
// CDlgTeachOCR 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTeachOCR, CDialogEx)

CDlgTeachOCR::CDlgTeachOCR(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTeachOCR::IDD, pParent)
{
	m_pParent = pParent;
	m_pDisplay=NULL;
	m_nSizeX = 0;
	m_nSizeY = 0;
}

CDlgTeachOCR::~CDlgTeachOCR()
{
	if(m_pDisplay) delete m_pDisplay;
}

void CDlgTeachOCR::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_OCR, m_ctrlOCR);
	DDX_Control(pDX, IDC_SPIN1, m_spinOCR);
	DDX_Text(pDX, IDC_EDIT_SIZEX, m_nSizeX);
	DDX_Text(pDX, IDC_EDIT_SIZEY, m_nSizeY);
	DDX_Control(pDX, IDC_LIST_RESULT, m_ctrlResult);
	DDX_Check(pDX, IDC_CHECK_SMOOTH, m_bCheckSmooth);
	DDX_Check(pDX, IDC_CHECK_OVERLAP, m_bCheckOverlap);
	DDX_Check(pDX, IDC_CHECK_SHOW_TEACHROI, m_bTeachROI);
	DDX_Check(pDX, IDC_CHECK_SHOW_SEARCH_ROI, m_bSearchROI);
}


BEGIN_MESSAGE_MAP(CDlgTeachOCR, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_OCR, &CDlgTeachOCR::OnStnClickedStaticOcr)
	ON_BN_CLICKED(IDC_BUTTON_REGISTER, &CDlgTeachOCR::OnBnClickedButtonRegister)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CDlgTeachOCR::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_CHECK_SHOW_TEACHROI, &CDlgTeachOCR::OnBnClickedCheckShowTeachroi)
	ON_BN_CLICKED(IDC_CHECK_SHOW_SEARCH_ROI, &CDlgTeachOCR::OnBnClickedCheckShowSearchRoi)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDlgTeachOCR::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CDlgTeachOCR::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_PREV, &CDlgTeachOCR::OnBnClickedButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CDlgTeachOCR::OnBnClickedButtonNext)
	ON_CBN_SELCHANGE(IDC_COMBO_OCR, &CDlgTeachOCR::OnCbnSelchangeComboOcr)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDlgTeachOCR::OnDeltaposSpin1)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CDlgTeachOCR::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDOK, &CDlgTeachOCR::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgTeachOCR 메시지 처리기입니다.


void CDlgTeachOCR::OnStnClickedStaticOcr()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL CDlgTeachOCR::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int i;
	CString str;


	WINDOWPLACEMENT wp;
	GetDlgItem(IDC_STATIC_IMAGE)->GetWindowPlacement(&wp);

	OpenDisplay(OCR_MAX_WIDTH,OCR_MAX_HEIGHT, wp.rcNormalPosition);



	for(i=0;i<OCR_NUMBER;i++)
	{
		str.Format(_T("%c"), l_OCRCHAR[i]);
		m_ctrlOCR.AddString(str);
	}
	m_nSelect=0;
	m_ctrlOCR.SetCurSel(m_nSelect);
	SelectOCR(m_nSelect);
	


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgTeachOCR::SelectOCR(int N)
{
	m_nSizeX		=g_OCR[N].m_nSizeX;
	m_nSizeY		=g_OCR[N].m_nSizeY;
	m_bCheckSmooth	=g_OCR[N].m_nSmooth;
	m_bCheckOverlap	=g_OCR[N].m_nOverlap;
	
	m_nShowID=0;
	ShowOCR(N, m_nShowID);
	UpdateData(false);
}

void CDlgTeachOCR::OnBnClickedButtonRegister()
{
	int left, top, right, bottom;
	bool flag=false;

	UpdateData(true);

	((CKoWebView *)m_pParent)->m_pImage->m_pROI->GetROIRegion(_T("T"), &left, &top, &right, &bottom);
	
	if(right-left>OCR_MAX_WIDTH)   { flag=true; right=left+OCR_MAX_WIDTH;}
	if(bottom-top> OCR_MAX_HEIGHT) { flag=true; bottom=top+OCR_MAX_HEIGHT;}

	m_nSizeX=right-left;
	m_nSizeY=bottom-top;
	g_OCR[m_nSelect].m_nSizeX	=m_nSizeX;
	g_OCR[m_nSelect].m_nSizeY	=m_nSizeY;
	g_OCR[m_nSelect].m_nSmooth  =m_bCheckSmooth;
	g_OCR[m_nSelect].m_nOverlap =m_bCheckOverlap;
	m_nShowID=0;

	UpdateData(false);

	if(flag)
	{
		((CKoWebView *)m_pParent)->m_pImage->m_pROI->SetROIRect(_T("T"), left, top, right, bottom);
		((CKoWebView *)m_pParent)->m_pImage->UpdateImage();
	}

	RegisterOCR();

}
void CDlgTeachOCR::RegisterOCR()
{
	int left, top, right, bottom;
	int nSizeX, nSizeY;
	int i, j, k;
	int pitch, pitchM;
	int N=m_nSelect;
	int nMult;
	BYTE fmT[OCR_MAX_WIDTH*OCR_MAX_HEIGHT];
	CPoint pt[4], ptD[4];
	
	LPBYTE fm=((CKoWebView *)m_pParent)->m_pImage->GetImageDataDisplay(&pitch);
	LPBYTE fmShow=m_pDisplay->GetImageDataDisplay(&pitchM);


	((CKoWebView *)m_pParent)->m_pImage->m_pROI->GetROIRegion(_T("T"), &left, &top, &right, &bottom);
	
	nSizeX=right-left;
	nSizeY=bottom-top;

	ptD[0].x=0;				ptD[0].y=0;
	ptD[1].x=0;				ptD[1].y=nSizeY;
	ptD[2].x=nSizeX;		ptD[2].y=nSizeY;
	ptD[3].x=nSizeX;		ptD[3].y=0;

	for(k=0;k<7;k++)
	{
		if(k==0)
		{
			pt[0].x=left;	pt[0].y=top;
			pt[1].x=left;	pt[1].y=bottom;
			pt[2].x=right;	pt[2].y=bottom;
			pt[3].x=right;	pt[3].y=top;
		}
		else if(k==1) //X줄임
		{
			pt[0].x=left-OCR_ELONG_X;	pt[0].y=top;
			pt[1].x=left-OCR_ELONG_X;	pt[1].y=bottom;
			pt[2].x=right+OCR_ELONG_X;	pt[2].y=bottom;
			pt[3].x=right+OCR_ELONG_X;	pt[3].y=top;
		}
		else if(k==2) //X늘림
		{
			pt[0].x=left+OCR_ELONG_X;	pt[0].y=top;
			pt[1].x=left+OCR_ELONG_X;	pt[1].y=bottom;
			pt[2].x=right-OCR_ELONG_X;	pt[2].y=bottom;
			pt[3].x=right-OCR_ELONG_X;	pt[3].y=top;
		}
		else if(k==3) //Y줄임
		{
			pt[0].x=left;	pt[0].y=top-OCR_ELONG_Y;
			pt[1].x=left;	pt[1].y=bottom+OCR_ELONG_Y;
			pt[2].x=right;	pt[2].y=bottom+OCR_ELONG_Y;
			pt[3].x=right;	pt[3].y=top-OCR_ELONG_Y;
		}
		else if(k==4) //Y늘림
		{
			pt[0].x=left;	pt[0].y=top+OCR_ELONG_Y;
			pt[1].x=left;	pt[1].y=bottom-OCR_ELONG_Y;
			pt[2].x=right;	pt[2].y=bottom-OCR_ELONG_Y;
			pt[3].x=right;	pt[3].y=top+OCR_ELONG_Y;
		}
		else if(k==5) //전체줄임
		{
			pt[0].x=left-OCR_ELONG_X;	pt[0].y=top-OCR_ELONG_Y;
			pt[1].x=left-OCR_ELONG_X;	pt[1].y=bottom+OCR_ELONG_Y;
			pt[2].x=right+OCR_ELONG_X;	pt[2].y=bottom+OCR_ELONG_Y;
			pt[3].x=right+OCR_ELONG_X;	pt[3].y=top-OCR_ELONG_Y;
		}
		else if(k==6) //전체늘림
		{
			pt[0].x=left+OCR_ELONG_X;	pt[0].y=top-OCR_ELONG_Y;
			pt[1].x=left+OCR_ELONG_X;	pt[1].y=bottom+OCR_ELONG_Y;
			pt[2].x=right-OCR_ELONG_X;	pt[2].y=bottom+OCR_ELONG_Y;
			pt[3].x=right-OCR_ELONG_X;	pt[3].y=top-OCR_ELONG_Y;
		}

		g_OCR[N].m_nRegistered[k]=1;
		InterpolationTransform_SubPixel(pt, fm, g_System.m_nPitch, ptD, fmT, OCR_MAX_WIDTH);


		if(g_OCR[N].m_nSmooth)
		{
			memcpy(g_fmTemp, fmT, OCR_MAX_WIDTH*OCR_MAX_HEIGHT);

			for(i=1;i<nSizeY-1;i++)
				for(j=1;j<nSizeX-1;j++)
				{
					nMult=*(g_fmTemp+OCR_MAX_WIDTH*(i-1)+(j-1))+*(g_fmTemp+OCR_MAX_WIDTH*(i-1)+(j))+*(g_fmTemp+OCR_MAX_WIDTH*(i-1)+(j+1))+
						  *(g_fmTemp+OCR_MAX_WIDTH*(i)+(j-1))  +*(g_fmTemp+OCR_MAX_WIDTH*(i)+(j))  +*(g_fmTemp+OCR_MAX_WIDTH*(i)+(j+1))+
						  *(g_fmTemp+OCR_MAX_WIDTH*(i+1)+(j-1))+*(g_fmTemp+OCR_MAX_WIDTH*(i+1)+(j))+*(g_fmTemp+OCR_MAX_WIDTH*(i+1)+(j+1))+5;
					*(fmT+OCR_MAX_WIDTH*i+j)=nMult/9;					
				}

			for(i=0;i<nSizeY;i++)
			{
				*(fmT+OCR_MAX_WIDTH*i)=*(fmT+OCR_MAX_WIDTH*i+1);
				*(fmT+OCR_MAX_WIDTH*i+nSizeX-2)=*(fmT+OCR_MAX_WIDTH*i+nSizeX-3);
			}
			for(j=0;j<nSizeX;j++)
			{
				*(fmT+j)=*(fmT+OCR_MAX_WIDTH+j);
				*(fmT+OCR_MAX_WIDTH*(nSizeY-2)+j)=*(fmT+OCR_MAX_WIDTH*(nSizeY-3)+j);
			}
		}


		if(g_OCR[N].m_nOverlap && g_OCR[N].m_nOverLapCount>0)
		{
			nMult=g_OCR[N].m_nOverLapCount;
			for(i=0;i<nSizeY;i++)
				for(j=0;j<nSizeX;j++)
					*(g_OCR[N].m_fm[k]+OCR_MAX_WIDTH*i+j)=(*(g_OCR[N].m_fm[k]+OCR_MAX_WIDTH*i+j)*nMult+*(fmT+OCR_MAX_WIDTH*i+j))/(nMult+1);

		}
		else
		{
			for(i=0;i<nSizeY;i++)
				for(j=0;j<nSizeX;j++)
					*(g_OCR[N].m_fm[k]+OCR_MAX_WIDTH*i+j)=*(fmT+OCR_MAX_WIDTH*i+j);
		}


	}
	if(g_OCR[N].m_nOverlap) g_OCR[N].m_nOverLapCount++;

	memset(fmShow, 0, OCR_MAX_WIDTH*OCR_MAX_HEIGHT);
	for(i=0;i<m_nSizeY;i++)
		for(j=0;j<m_nSizeX;j++)
			*(fmShow+pitchM*i+j)=*(g_OCR[N].m_fm[0]+OCR_MAX_WIDTH*i+j);
	
	m_pDisplay->UpdateImage();

}

void CDlgTeachOCR::OpenDisplay(int nWidth, int nHeight, CRect rcDisp)
{
	CString str;
	
	g_OCRPattern.Initialize(OCR_MAX_WIDTH, OCR_MAX_HEIGHT);
	if(m_pDisplay==NULL)
	{
		m_pDisplay=new CCossImage(this);
		m_pDisplay->m_bShowGrayValue=false;
		m_pDisplay->IMAGE_COLOR=1;
		m_pDisplay->SetImageData(nWidth, nHeight);
		m_pDisplay->m_fmProcess=NULL;
		m_pDisplay->m_pROI=&g_OCRPattern;
		m_pDisplay->m_nDisplayID=1;
		str.Format(_T(""));
		m_pDisplay->SetImageName(str);
		m_pDisplay->Create(IDD_DIALOG_COSSIMAGE,this);
		m_pDisplay->MoveWindow(rcDisp);
	//	m_pDisplay->FitZoom();
		m_pDisplay->ShowWindow(SW_SHOWNORMAL);
	}
}

void CDlgTeachOCR::OnBnClickedCheckShowTeachroi()
{
	UpdateData(true);
	int nX, nY;
	if(m_bTeachROI)
	{
//		GetDlgItem(IDC_BUTTON_REGISTER)->EnableWindow(1);
//		GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(1);
		((CKoWebView *)m_pParent)->m_pImage->Local2Global(300, 300, &nX, &nY);	
		((CKoWebView *)m_pParent)->m_pImage->m_pROI->AddROI(_T("T"), nX-m_nSizeX/2, nY-m_nSizeY/2, nX+m_nSizeX/2, nY+m_nSizeY/2);
		((CKoWebView *)m_pParent)->m_pImage->m_pROI->EnableROI(true);
		((CKoWebView *)m_pParent)->m_pImage->m_pROI->ShowROI(true);
		((CKoWebView *)m_pParent)->m_pImage->UpdateImage();
	}
	else
	{
//		GetDlgItem(IDC_BUTTON_REGISTER)->EnableWindow(0);
//		GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(0);
		((CKoWebView *)m_pParent)->m_pImage->m_pROI->DeleteROI(_T("T"));
		((CKoWebView *)m_pParent)->m_pImage->UpdateImage();
	}
}


void CDlgTeachOCR::OnBnClickedCheckShowSearchRoi()
{
	UpdateData(true);
	int nX, nY;
	int left, top, right, bottom;
	if(m_bSearchROI)
	{
//		GetDlgItem(IDC_BUTTON_REGISTER)->EnableWindow(1);
//		GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(1);
		((CKoWebView *)m_pParent)->m_pImage->m_pROI->GetROIRegion(_T("T"), &left, &top, &right, &bottom);
		if(left>0)
		{
			nX=(left+right)/2;
			nY=(top+bottom)/2;
		}
		else
		{
			((CKoWebView *)m_pParent)->m_pImage->Local2Global(300, 300, &nX, &nY);
		}
		((CKoWebView *)m_pParent)->m_pImage->m_pROI->AddROI(_T("S"), nX-m_nSizeX, nY-m_nSizeY, nX+m_nSizeX, nY+m_nSizeY);
		((CKoWebView *)m_pParent)->m_pImage->m_pROI->EnableROI(true);
		((CKoWebView *)m_pParent)->m_pImage->m_pROI->ShowROI(true);
		((CKoWebView *)m_pParent)->m_pImage->UpdateImage();
	}
	else
	{
//		GetDlgItem(IDC_BUTTON_REGISTER)->EnableWindow(0);
//		GetDlgItem(IDC_BUTTON_SEARCH)->EnableWindow(0);
		((CKoWebView *)m_pParent)->m_pImage->m_pROI->DeleteROI(_T("S"));
		((CKoWebView *)m_pParent)->m_pImage->UpdateImage();
	}
}


void CDlgTeachOCR::OnBnClickedButtonSave()
{
	CString str;
	str.Format(_T("%sOCRData.dat"), MAIN_FOLDER);
	SaveOCRData(str);
}


void CDlgTeachOCR::OnBnClickedButtonLoad()
{
	CString str;
	str.Format(_T("%sOCRData.dat"), MAIN_FOLDER);
	LoadOCRData(str);
}


void CDlgTeachOCR::OnBnClickedButtonPrev()
{
	if(m_nShowID>0)
	{
		m_nShowID--;
		ShowOCR(m_nSelect, m_nShowID);
	}
}


void CDlgTeachOCR::OnBnClickedButtonNext()
{
	if(m_nShowID<OCR_MAX_PATTERN-1)
	{
		m_nShowID++;
		ShowOCR(m_nSelect, m_nShowID);
	}
}

void CDlgTeachOCR::ShowOCR(int N, int M)
{
	int i,j,pitchM;
	LPBYTE fmShow=m_pDisplay->GetImageDataDisplay(&pitchM);
		
	memset(fmShow, 0, OCR_MAX_WIDTH*OCR_MAX_HEIGHT);
	for(i=0;i<m_nSizeY;i++)
		for(j=0;j<m_nSizeX;j++)
			*(fmShow+OCR_MAX_WIDTH*i+j)=*(g_OCR[N].m_fm[M]+OCR_MAX_WIDTH*i+j);
	
	m_pDisplay->UpdateImage();
}

void CDlgTeachOCR::OnCbnSelchangeComboOcr()
{
	m_nSelect=m_ctrlOCR.GetCurSel();
	SelectOCR(m_nSelect);
}


void CDlgTeachOCR::OnBnClickedButtonTest()
{
	int i, j, N, M;
	int left, top, right, bottom, pitch;
	double dX, dY, dScore;
	double start, end;
	int nScore[5], nOCRID[5];
	int nX, nY;
	int ret;
	CString str;


	LPBYTE fm=((CKoWebView *)m_pParent)->m_pImage->GetImageDataDisplay(&pitch);

	((CKoWebView *)m_pParent)->m_pImage->m_pROI->GetROIRegion(_T("S"), &left, &top, &right, &bottom);

	start=Clock_set();


	ret=FindOCR(fm, left, top, right, bottom, pitch , nScore, nOCRID, 1, &nX, &nY);

	end=Clock_set();

	m_ctrlResult.ResetContent();
	((CKoWebView *)m_pParent)->m_pImage->ResetGraphic();
	((CKoWebView *)m_pParent)->m_pImage->UpdateImage();

	if(ret>0) 
	{
//		((CKoWebView *)m_pParent)->m_pImage->SetAddGraphic(true);
//		((CKoWebView *)m_pParent)->m_pImage->DrawCross(NULL,nX, nY,10,RGB(255,0,0));
		N=nOCRID[0];
		if(m_bTeachROI && N>=0 && N<OCR_NUMBER)
		{
			((CKoWebView *)m_pParent)->m_pImage->m_pROI->SetROIRect(_T("T"), nX, nY, nX+g_OCR[N].m_nSizeX, nY+g_OCR[N].m_nSizeY);
			((CKoWebView *)m_pParent)->m_pImage->UpdateImage();
		}


		str.Format(_T("Elapse Time: %.1lf msec"), end-start);	m_ctrlResult.AddString(str);	
		for(i=0;i<5;i++)
		{
			if( nScore[i]>=50)
			{
				str.Format(_T("(%c) Score: %d "), l_OCRCHAR[nOCRID[i]], nScore[i] );		
				m_ctrlResult.AddString(str);
			}
		}
	}
	else
	{
		str.Format(_T("Matching Failure")); m_ctrlResult.AddString(str);
		str.Format(_T("Elapse Time: %.1lf msec"), end-start);	m_ctrlResult.AddString(str);	
	}
	
	UpdateData(false);
}

void CDlgTeachOCR::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	if(pNMUpDown->iDelta<0)
	{
		if( m_nSelect+1<OCR_NUMBER) m_nSelect++;
	}
	else
	{
		if(m_nSelect-1>=0)  m_nSelect--;

	}
	m_ctrlOCR.SetCurSel(m_nSelect);
	SelectOCR(m_nSelect);
	
	*pResult = 0;
}


void CDlgTeachOCR::OnBnClickedButtonReset()
{
	int i,j,k;
	int N=m_nSelect;
	int pitchM;
	LPBYTE fmShow=m_pDisplay->GetImageDataDisplay(&pitchM);
	g_OCR[N].m_nOverLapCount=0;
	for(k=0;k<OCR_MAX_PATTERN;k++)
	{
		memset(g_OCR[N].m_fm[k], 0, OCR_MAX_WIDTH*OCR_MAX_HEIGHT);
	}

	memset(fmShow, 0, OCR_MAX_WIDTH*OCR_MAX_HEIGHT);
	
	m_pDisplay->UpdateImage();
}


void CDlgTeachOCR::OnBnClickedOk()
{
	bool flag=false;
	if(m_bTeachROI) {
		((CKoWebView *)m_pParent)->m_pImage->m_pROI->DeleteROI(_T("T")); flag=true;}
	if(m_bSearchROI){
		((CKoWebView *)m_pParent)->m_pImage->m_pROI->DeleteROI(_T("S")); flag=true;}

	if(flag)
	{
		((CKoWebView *)m_pParent)->m_pImage->UpdateImage();
	}

	CDialogEx::OnOK();
}
