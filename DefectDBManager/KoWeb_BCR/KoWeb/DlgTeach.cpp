// DlgTeach.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "DlgTeach.h"
#include "afxdialogex.h"

#include "KoWebDoc.h"
#include "KoWebView.h"

// CDlgTeach 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTeach, CDialogEx)

CDlgTeach::CDlgTeach(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTeach::IDD, pParent)
{
	m_pParent=pParent;
}

CDlgTeach::~CDlgTeach()
{
}

void CDlgTeach::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FPSPREAD1, m_Spread);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_ctrlType);
}


BEGIN_MESSAGE_MAP(CDlgTeach, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_INSERT, &CDlgTeach::OnBnClickedButtonInsert)
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CDlgTeach::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDOK, &CDlgTeach::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgTeach::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_GRAYIMAGE, &CDlgTeach::OnBnClickedButtonShowGrayimage)
END_MESSAGE_MAP()


// CDlgTeach 메시지 처리기입니다.




BEGIN_EVENTSINK_MAP(CDlgTeach, CDialogEx)
	ON_EVENT(CDlgTeach, IDC_FPSPREAD1, 3, CDlgTeach::ButtonClickedFpspread1, VTS_I4 VTS_I4 VTS_I2)
END_EVENTSINK_MAP()



BOOL CDlgTeach::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateInspectData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgTeach::UpdateInspectData()
{
	FillInspectData();
	g_ROI[g_InspSet].ShowROI(true);
	g_ROI[g_InspSet].EnableROI(true);
}

void CDlgTeach::FillInspectData()
{
	int i, k;
	int nNum=0;
	
	for(k=0;k<MAX_INSPECT_CLASS;k++) //같은불량 종류별로 묶기 위해서 꼭 사용함
	{
		for(i=0;i<MAX_INSPECT;i++)
		{
			if(g_Inspect[g_InspSet].m_nTeachDone[i]==0) continue;
	
			if(k==g_Inspect[g_InspSet].m_nInspectType[i])
				AddInspectRow(g_Inspect[g_InspSet].m_nInspectType[i], nNum++, g_Inspect[g_InspSet].m_nID[i]);		

			if(nNum==g_Inspect[g_InspSet].m_nInspectCount) break;
		}
	}

	for(i=nNum;i<MAX_INSPECT;i++)
	{
		m_Spread.SetRow(i+2);
		m_Spread.SetCol(1); m_Spread.SetValue(_T(""));
		m_Spread.SetCol(2); m_Spread.SetValue(_T(""));
		m_Spread.SetCol(3); m_Spread.SetValue(_T(""));
		m_Spread.SetCol(4); m_Spread.SetValue(_T(""));
		m_Spread.SetCol(5); m_Spread.SetValue(_T(""));
	}
}
void CDlgTeach::AddInspectRow(int nSet, int nShowCount, int nNum)
{
	int nRow=nShowCount+2;
	CString str;
	m_Spread.SetRow(nRow);

	//USE
	m_Spread.SetCol(1);
	str.Format(_T("%d"), g_Inspect[g_InspSet].m_nUSE[nNum]);
	m_Spread.SetValue(str);

	//Inspect Type
	m_Spread.SetCol(2);
	str.Format(_T("%s"), g_sInspectTypeName[g_Inspect[g_InspSet].m_nInspectType[nNum]]);
	m_Spread.SetValue(str);

	//ID
	m_Spread.SetCol(3);
	str.Format(_T("%d"), g_Inspect[g_InspSet].m_nID[nNum]);
	m_Spread.SetValue(str);

	//ROINum
	m_Spread.SetCol(4);
	str.Format(_T("%d"), g_Inspect[g_InspSet].m_nROINum[nNum]);
	m_Spread.SetValue(str);

	//SHOW ROI
	m_Spread.SetCol(5);
	str.Format(_T("%d"), g_Inspect[g_InspSet].m_nROIShow[nNum]);
	m_Spread.SetValue(str);
}

void CDlgTeach::OnBnClickedButtonInsert()
{
	if(g_Temp.m_bAuto) return;

	int nType=m_ctrlType.GetCurSel();
	int i, N=-1; 
	if(nType>=0)
	{
		//빈곳 사용함--------------------------------------
		for(i=0;i<g_Inspect[g_InspSet].m_nInspectCount;i++)
			if(g_Inspect[g_InspSet].m_nTeachDone[i]!=1)
			{
				N=i;
				break;
			}
		if(N<0) 
		{
			if(g_Inspect[g_InspSet].m_nInspectCount<MAX_INSPECT)
				N=g_Inspect[g_InspSet].m_nInspectCount;
			else
				return;
		}
		
		//-------------------------------------------------		
		g_Inspect[g_InspSet].m_nInspectCount++;
		g_Inspect[g_InspSet].m_nTeachDone[N]	=1;
		g_Inspect[g_InspSet].m_nInspectType[N]	=nType;
		g_Inspect[g_InspSet].m_nROINum[N]		=1;
		g_Inspect[g_InspSet].m_nROIShow[N]		=0;
		g_Inspect[g_InspSet].m_nUSE[N]			=1;
		g_Inspect[g_InspSet].m_nID[N]			=N;
		
		g_Inspect[g_InspSet].m_ROI1[N].dAngle	=0;
		g_Inspect[g_InspSet].m_ROI1[N].rc.left  =-1;
		g_Inspect[g_InspSet].m_ROI2[N].dAngle	=0;
		g_Inspect[g_InspSet].m_ROI2[N].rc.left  =-1;		
	}
	FillInspectData();
}
void CDlgTeach::DeleteInspect(int nID)
{
	CString ROIName;
	int N=g_Inspect[g_InspSet].m_nID[nID];
	if(N>=0)
	{
		g_Inspect[g_InspSet].m_nInspectCount--;
		g_Inspect[g_InspSet].m_nTeachDone[N]	=0;
		g_Inspect[g_InspSet].m_nROIShow[N]		=0;

		if(g_Inspect[g_InspSet].m_nROINum[N]==1)
		{
			ROIName.Format(_T("%d"),nID);
			((CKoWebView *)m_pParent)->TeachROI(0, true, ROIName, &g_Inspect[g_InspSet].m_ROI1[nID]);
			g_Inspect[g_InspSet].m_ROI1[N].dAngle	=0;
			g_Inspect[g_InspSet].m_ROI1[N].rc.left  =-1;
		}
		else if(g_Inspect[g_InspSet].m_nROINum[N]==2)
		{
			ROIName.Format(_T("%d_1"),N);
			((CKoWebView *)m_pParent)->TeachROI(0, true, ROIName, &g_Inspect[g_InspSet].m_ROI1[N]);
			ROIName.Format(_T("%d_2"),nID);
			((CKoWebView *)m_pParent)->TeachROI(0, true, ROIName, &g_Inspect[g_InspSet].m_ROI2[N]);
		
			g_Inspect[g_InspSet].m_ROI1[N].dAngle	=0;
			g_Inspect[g_InspSet].m_ROI1[N].rc.left  =-1;
			g_Inspect[g_InspSet].m_ROI2[N].dAngle	=0;
			g_Inspect[g_InspSet].m_ROI2[N].rc.left  =-1;
		}
	}
	FillInspectData();
}

void CDlgTeach::ButtonClickedFpspread1(long Col, long Row, short ButtonDown)
{
	CString str, ROIName;
	int nID, nUSE=0, nShowROI=0;
	int nType; //검사종류

	if(g_Temp.m_bAuto)
	{
		if(Col!=5) return;
	}

	//먼저 검사 ID를 체크한다. 검사 ID가 0미만이면 PASS-----------------------
	m_Spread.SetRow(Row);		
	m_Spread.SetCol(TEACH_ID);
	str=m_Spread.GetValue();
	if(str.GetLength()<1) return;
	else
	{
		nID=_ttoi(str);
		if(nID<0 || nID>=MAX_INSPECT)  return;
	}
	//-----------------------------------------------------------------------

	m_Spread.SetCol(Col);
	//USE-------------------------------------------------------------------------------------
	if(Col==1 && Row>1)
	{
		str=m_Spread.GetValue();
		nUSE=_ttoi(str);
		g_Inspect[g_InspSet].m_nUSE[nID]=nUSE;
	}
	//SHOW ROI -------------------------------------------------------------------------------
	if(Col==5 && Row>1)
	{
		str=m_Spread.GetValue();
		nShowROI=_ttoi(str);
		g_Inspect[g_InspSet].m_nROIShow[nID]=nShowROI;	

		if(g_Inspect[g_InspSet].m_nROINum[nID]==1)
		{
			ROIName.Format(_T("%d"),nID);
		 //SHOWROI =1 이면 AddROI  0 이면 DeleteROI
			((CKoWebView *)m_pParent)->TeachROI(nShowROI, true, ROIName, &g_Inspect[g_InspSet].m_ROI1[nID]);				
		}
		else if(g_Inspect[g_InspSet].m_nROINum[nID]==2)
		{
			ROIName.Format(_T("%d_1"),nID);
			((CKoWebView *)m_pParent)->TeachROI(nShowROI, true, ROIName, &g_Inspect[g_InspSet].m_ROI1[nID]);	
			ROIName.Format(_T("%d_2"),nID);
			((CKoWebView *)m_pParent)->TeachROI(nShowROI, true, ROIName, &g_Inspect[g_InspSet].m_ROI2[nID]);
		}			
	}
	//설정
	if(Col==6 && Row>1)
	{
		OpenSetDialog(nID); 
	}
	//Delete
	if(Col==7 && Row>1)
	{
		if(AfxMessageBox(_T("Delete?"), MB_YESNO)==IDYES)
			DeleteInspect(nID);
	}
	//------------------------------------------------------------------------------------------
}


//등록된 검사를 모두 수행한다.
void CDlgTeach::OnBnClickedButtonTest()
{
	if(g_Temp.m_bAuto) return;

	((CKoWebView *)m_pParent)->OneInspect(g_InspSet);
	UpdateResult();
}

void CDlgTeach::UpdateResult()
{
		int i, j, nId;
	CString str;
	bool NG;
	int nIdArray[MAX_INSPECT];
	int nZeroCount=0;
	int nMaxInspRow=0;
	
	//각 ROW 의 검사 Id를 읽어서 nIdArray에 넣는다.
	m_Spread.SetCol(3);
	for(j=0;j<MAX_INSPECT;j++)
	{
		m_Spread.SetRow(j+2);
		str=m_Spread.GetValue();
		if(str.IsEmpty())
			break;
		nIdArray[j]=_ttoi(str);
		nMaxInspRow++;
	}

	m_Spread.SetCol(8);
	for(j=0;j<nMaxInspRow;j++)
	{
		m_Spread.SetRow(j+2);
		m_Spread.SetValue(_T(""));
	}

	m_Spread.SetCol(8);
	for(i=0;i<nMaxInspRow;i++)
	{
		nId=g_Inspect[g_InspSet].m_nID[i];
		if(nId==0) nZeroCount++;
		if(nZeroCount>1) break;

		if(nId<0 || nId>=MAX_INSPECT) continue;

		if(!g_Inspect[g_InspSet].m_nUSE[nId]) continue;

		NG=IsInspectResultNG(nId);
		for(j=0;j<nMaxInspRow;j++)
			if(nId==nIdArray[j])
			{
				m_Spread.SetRow(j+2);
				if(NG) str=_T("NG");
				else   str=_T("OK");
				m_Spread.SetValue(str);
				break;
			}		
	}
}

void CDlgTeach::OnBnClickedOk()
{
	BackUpROIData();
	HideROI();

	CDialogEx::OnOK();
}


void CDlgTeach::OnBnClickedCancel()
{
	BackUpROIData();
	HideROI();	

	CDialogEx::OnCancel();
}

void CDlgTeach::HideROI()
{
	g_ROI[g_ID].ShowROI(false);
	g_ROI[g_ID].EnableROI(false);
	((CKoWebView *)m_pParent)->m_pImage->UpdateImage();
}

void CDlgTeach::BackUpROIData()
{
	int i;
	int nID;
	CString str;

	for(i=0;i<g_Inspect[g_InspSet].m_nInspectCount;i++)
	{
		if(g_Inspect[g_InspSet].m_nROIShow[i])
		{
			nID=g_Inspect[g_InspSet].m_nID[i];
			str.Format(_T("%d"), nID);
			((CKoWebView *)m_pParent)->TeachROI(0, false, str, &g_Inspect[g_InspSet].m_ROI1[nID]);	
		}		
	}
}

void CDlgTeach::OpenSetDialog(int nID)
{
	int nType;
	CKoWebView *pView=(CKoWebView*)m_pParent;		

	nType=g_Inspect[g_InspSet].m_nInspectType[nID];

	switch(nType)
	{
		case INSP_BLOB:		 pView->OpenBlobSet(g_InspSet, nID);		break;
		case INSP_PATTERN:	 pView->OpenPatternSet(g_InspSet, nID);		break;
		case INSP_FINDLINE:	 pView->OpenFindLineSet(g_InspSet, nID);	break;
		case INSP_GAP2LINE:	 pView->OpenMeasureSet(g_InspSet, nID);		break;
		case INSP_SPOT:		 pView->OpenSpotSet(g_InspSet, nID);		break;	//@항목추가3
		case INSP_COMPARE:	 pView->OpenCompareSet(g_InspSet, nID);		break;
		case INSP_OCR:		 pView->OpenOCRSet(g_InspSet, nID);			break;
		case INSP_USER:		 pView->OpenUserSet(g_InspSet, nID);		break;
		case INSP_GAP:									break;
		case INSP_STAIN:								break;
		
	}
}



void CDlgTeach::OnBnClickedButtonShowGrayimage()
{
	if(g_Temp.m_bAuto) return;
	CKoWebView *pView=(CKoWebView*)m_pParent;	
	pView->ShowGrayImage();
}
