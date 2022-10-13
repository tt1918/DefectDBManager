

// KoWebView.h : interface of the CKoWebView class
//

#pragma once

#include "resource.h"
#include "KoWebDoc.h"
#include "DlgConfig.h"
#include "DlgParam.h"
#include "DlgDebug.h"
#include "ShowInfo1.h"

#include "DlgPattern.h"
#include "DlgTeachOCR.h"
#include "DlgAlign.h"
#include "DlgTeach.h"
#include "DlgThreshold.h"
#include "fpspread_data.h"
#include "PatternSet.h"
#include "BlobSet.h"
#include "SpotSet.h"
#include "FindLineSet.h"
#include "MeasureSet.h"
#include "CompareSet.h"
#include "OCRSet.h"
#include "UserSet.h"
#include "DlgModel.h"

#include "BCR/CallClassWrapper.h"
#include "BCR/CallClassReadingEvents.h"
#include "BCR/MarkingDataManager.h"

#define SHOW_NG_IMAGE	1
class CKoWebView : public CFormView
{
protected: // create from serialization only
	CKoWebView();
	DECLARE_DYNCREATE(CKoWebView)

public:
	enum { IDD = IDD_KOWEB_FORM };

	// Attributes
public:
	CKoWebDoc* GetDocument() const;

#ifdef AI_APPLY
	AbyssClient* m_pAbyssClient;
#endif

#ifdef USE_NITTO_AI
	NittoAiClient* m_pNittoAiClient;
#endif

	CCossImage* m_pImageArray[MAX_IMAGE];
	CRect           m_rcImageMax;
	CRect           m_rcImage[MAX_IMAGE];
	CCossImage* m_pImage;						//여러개 사용하려면 CCossImage *m_pImageArray[3] 만들고 m_pImage=m_pImageArray[0] 이런식으로 사용
	CCossImage* m_pNGImage[SHOW_NG_IMAGE];

	CDlgConfig* m_pConfig;
	CDlgParam* m_pParam;
	CDlgDebug* m_pDebug;
	CShowInfo1* m_pShowInfo;

	//범용검사기>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	CDlgPattern* m_pPattern;
	CDlgTeachOCR* m_pOCR;
	CDlgAlign* m_pAlign;
	CDlgTeach* m_pTeach;

	CPatternSet* m_pPatternSet;
	CBlobSet* m_pBlobSet;
	CSpotSet* m_pSpotSet;
	CFindLineSet* m_pFindLineSet;
	CMeasureSet* m_pMeasureSet;
	CCompareSet* m_pCompareSet;
	COCRSet* m_pOCRSet;			//@항목추가15
	CUserSet* m_pUserSet;
	CDlgModel* m_pModel;

	MarkingDataManager m_MarkingDataMgr;

	//AI관련함수-----------------------------------------------------------
	void AI_ConnectServer();
	void AI_Close();
	int  AI_FindDefectName(LPBYTE fm, int width, int height, int nMaxTime, double* pData);
	//---------------------------------------------------------------------

	//Nitto AI관련함수-----------------------------------------------------------
	void NittoAI_ConnectServer();
	void NittoAI_Close();
	int NittoAI_SendData(DEFECTDATA* defectData);
	//---------------------------------------------------------------------

	void TeachROI(bool bShow, bool bImageUpdate, CString str, ROIDATA* pROI);
	void GetTeachROI(int nSet, int nID);
	void UpdateImageInfo();

	void OpenBlobSet(int nSet, int nID);
	void OpenPatternSet(int nSet, int nID);
	void OpenSpotSet(int nSet, int nID);	//5
	int	InspectSpot_Blob(int nSet, int nId, LPBYTE fm, LPBYTE fmGray, int nWidth, int nHeight, int nStartX, int nStartY, bool bShowGraphic);
	void OpenFindLineSet(int nSet, int nID);
	void OpenMeasureSet(int nSet, int nID);
	void OpenCompareSet(int nSet, int nID);
	void OpenOCRSet(int nSet, int nID); //@항목추가4-1
	void OpenUserSet(int nSet, int nID);

	//INSPECTOR
	int  PatternMatch(int nId, LPBYTE fmSearch, int left, int top, int right, int bottom, int pitch, double* dScore, double* dRx, double* dRy);

	//Blob
	void InspectBlob(LPBYTE fm, int nSet, int nId, BOOL bShowGraphic);
	void ShowBlobInspectResult(int nId);
	//Pattern Matching
	void InspectPattern(LPBYTE fm, int nSet, int nId, BOOL bShowGraphic);
	void ShowPatternInspectResult(int nId);
	//Find Line
	void InspectFindLine(LPBYTE fm, int nSet, int nId, BOOL bShowGraphic);
	void ShowFindLineInspectResult(int nId);
	//Gap From two Line
	int InspectMeasure(LPBYTE fm, int nSet, int nId, BOOL bShowGraphic);
	void ShowMeasureInspectResult(int nId);
	//Spot
	void InspectSpot(LPBYTE fm, int nSet, int nId, BOOL bShowGraphic); //@항목추가 13
	void ShowSpotInspectResult(int nId);
	//Compare
	void InspectCompare(LPBYTE fm, int nSet, int nId, BOOL bShowGraphic);
	void ShowCompareInspectResult(int nId);
	//OCR
	void InspectOCR(LPBYTE fm, int nSet, int nId, BOOL bShowGraphic);
	void ShowOCRInspectResult(int nId);
	//USER
	void InspectUser(LPBYTE fm, int nSet, int nId, BOOL bShowGraphic);
	void ShowUserInspectResult(int nId);


	void SelectImage(int nSet);
	//Align
	void ShowAlignROI(int nSet, BOOL bShow);
	void SetAlignROI(int nSet);
	int FindAlignPos(int nSet);

	void ShowGrayImage();
	void OneInspect(int nSet);

	void ShowMax(bool bShowMax);	//이미지를 한개만 보이게 한다.

	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	CDlgThreshold* m_pBinary;

	int         m_nShowNGID;
	int			m_nDisplayX;
	int			m_nDisplayY;

	int			m_nCheckNewLot;

	//WEB검사
	CTime m_tTimeCheckFrame;
	CTime m_tTimeCheckFrameOld;

	void InitData();
	void LotReset();
	int  CheckDefectOverlap();
	//	int  CopyNGImage(bool bFlat); //불량 128x128영상 메모리로 이동
	void ShowDefectInfo(int nID);
	void SendLotName(CString sLotName); //Cossclean에 Lot이름 넘김

	void ShowGrabImage(); //Grab한 영상을 화면에 보이게 함.
	void UpdateCount();
	void ViewInspect();
	void WebInspect();

	void ShowResults();
	void DrawResults();		//결과화면에 그리기

	void ShowResults_General();

	int CheckFrame(CString strpacket);

	void Inspect1();
	void Inspect2();
	void Inspect3();
	void Inspect4();
	void Inspect5();
	void Inspect6();
	void Inspect7();
	void Inspect8();
	void Inspect9();
	void InspectA();
	void InspectB();
	void InspectC();
	void InspectD();
	void InspectE();
	void InspectF();
	void InspectBCR();
	void InspectMarkingVision();

	void CheckPeriod();	//주기체크
	void AutoExposure(int nLineCAM, int nGrabFrame, double dBright); //자동밝기
	void CheckKnulling();	//TAC_LINE KNULLING유뮤체크
	// Operations
public:
	void SetShowImage(bool flag);	//Show Image Check
	void MakeMainDisplay();
	void MakeNGDisplay();
	void MakeGUI();
	void ButtonState(BOOL bStart);
	void DrawMainViewText(CDC* pDC);

	void DrawResultViewText(CDC* pDC);		//결과 화면에 쓰기
	void DrawResultViewText();				//영역 Invalidate

	void DrawDefectViewText(CDC* pDC);		//Defect정보 화면에 쓰기
	void DrawDefectViewText();				//Defect정보 Invalidate기

	void DrawMarkingVisionROI(CDC* pDC);		//결과 화면에 쓰기


	void SetSpreadData(int nRow, CString str, int nColor = 0);  //필요시점
	void SetSpreadCount();						//COUNT (매 Frame)
	void SetSpreadFPS();						//FPS 보여줌
	void UpdateInfo();							//검사시간,밝기, Sharpness 보여줌
	void ShowParameter();						//Parameter한번 읽은 후에 Call하는 함수

	void ShowProfile();							//화면에 Profile보이기
	void ShowCalibWidth();						//화면에 Calibration 표시
	void ShowNotInspectLine();					//Not Inspect Line

	void AutoStart();
	void AutoStop();
	void ResetTempData();

	//	void MakeDefectRect(int nDefect, int nX, int nY, int nLeft, int nRight);
	void DrawDefectRect(int nID);


	//  VIEWUDP
	void ConnectUDP();
	void CloseUDP();
	void Decoding(int nPort, unsigned char* pData);


	//  VIEWLOG
	void fnWriteSlice(int n, int nc);


	//BINARY Dialog
	void CloseBinary(bool bOriginal);
	void Binary(int nThreshold);
	void ROIFull(bool bFull);
	void ShowOriginal();
	void ShowApplyImage();
	void AppplyImage();
	void RotateImage(double  dAngle);
	void SmoothingImage();
	void ErodeImage();
	void EmphasizeImage(int nX, int nY, double dFactor);
	void RemoveLowHigh(int nCutLow, int nCutHigh, int nKernel);
	void FlipHor();
	void FlipVer();
	void ImageElongation(int nX, int nY);
	void InverseImage();
	void FlatImageROI(int nBaseGray);
	void FlatImageFull(int nType, int nBaseGray, int left, int width);


	void FindHBOSize(double dBase, int nThUp, int nCoreTh, double dCloudTh, int* pValue, int* CoreSize, double* dCloudValue, int* CloudSize);
	void CossFilter_MakeImageWithSobel(int nTh);


	void Binary(int nThreshold, int ll, int tt, int rr, int bb);
	void ShowGray(int ll, int tt, int rr, int bb);
	void SmoothAndBinary(int nThup, int nThDn, int wingsize);
	void FilterAverageMean(unsigned char* pSImg, int nImgXSize, int nImgYSize, int nWingSize, unsigned char* pDImg);
	void NearBrgDiffAll(int thup, int thdn, CRect insRect, unsigned char* pMask33, int nWidth, int nHeight, unsigned char* pAtt, int nWingSize);
	void RemoveHorNoise(int nTh);
	void MakePolaImage();
	void MakeSeparatemage();
	// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	void         ViewClose();
	// Implementation
public:
	virtual ~CKoWebView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void DrawBackGround(CDC* pDC);

	//#CButtonST  2  ----------------------------------
	CButtonST m_btConfig;   //[CONFIG] 버튼
	CButtonST m_btLoadIMG;  //[Load IMG] 버튼
	CButtonST m_btSaveIMG;  //[Save IMG] 버튼
	CButtonST m_btGrab;		//[Grab] 버튼
	CButtonST m_btParam;	//[Param] 버튼

	CFont m_BTFont;
	void initButtonST(int nType, CButtonST* cButton);

	//-------------------------------------------------

	//#OwnerDraw 2 -------------------------------------
	CCustomButton	m_btTool;

	/////////////////////////////////////////////////////////////////////////////
	// DB Defect Search - Start
public:
	CallClassWrapper* m_DefectCallClass;
	CallClassReadingEvents* m_DefectReadingEvent;

private:
	void CreateDefectCallCallss();
	void DestroyDefectCallClass();

	// DB Defect Search - End
	/////////////////////////////////////////////////////////////////////////////

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bShowFlat;
	BOOL m_bShowCandiArea;
	BOOL m_bShowAllCandi;
	BOOL m_bShowValidCandi;

	//Result 쓰기
	CRect	m_rcResult;
	CString m_sResult[20];
	int     m_nResultStringCnt;
	//Defect쓰기
	CRect	m_rcDefect;
	CString m_sDefectText[10];
	int     m_nDefecttextCnt;

	double m_dYLength;
	BOOL m_bShowInfo;
	BOOL m_bShowImage;
	CComboBox m_ctrlZoom;
	BOOL m_bShowMap;
	BOOL m_bShowMax;
	BOOL m_bShowOldMap;
	BOOL m_AllLength;
	//	CListBox m_ctrlDefectInfo;
	//	CListBox m_ctrlResult;

	DECLARE_EVENTSINK_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnDraw(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonLoadimage();
	afx_msg void OnBnClickedButtonSaveimage();
	afx_msg void OnBnClickedButtonInspect();
	afx_msg void OnBnClickedButtonConfig();
	afx_msg void OnBnClickedButtonParam();
	afx_msg void OnBnClickedButtonTeach();
	afx_msg void OnBnClickedCheckAutoStart();
	afx_msg void OnBnClickedCheckShowInfo();
	afx_msg void OnBnClickedCheckShowflat();
	afx_msg void OnBnClickedCheckShowimge();
	afx_msg void OnCbnSelchangeComboZoom();
	afx_msg void OnBnClickedButtonBinary();
	afx_msg void OnBnClickedButtonPattern();
	afx_msg void OnBnClickedButtonFuncTest();
	afx_msg void OnBnClickedButtonAlign();
	afx_msg void OnBnClickedButtonGrab();
	afx_msg void OnBnClickedCheckLive();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonPrev();
	afx_msg void OnBnClickedButtonOcr();
	afx_msg void OnBnClickedButtonDebug();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedCheckShowmap();
	afx_msg void OnBnClickedRadioCam1();
	afx_msg void OnBnClickedRadioCam2();
	afx_msg void OnBnClickedRadioCam3();
	afx_msg void OnBnClickedRadioCam4();
	afx_msg void OnBnClickedButtonModel();
	afx_msg void OnBnClickedCheckShowmax();
	afx_msg void OnBnClickedCheckShowoldmap();
	afx_msg void OnBnClickedButtonInspect3();
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnBnClickedBtnShowDefectNow();
	afx_msg void OnBnClickedBtnShowDefectNext();

	afx_msg LRESULT OnBCrComm(WPARAM wParam, LPARAM lParam);
};

#ifndef _DEBUG  // debug version in KoWebView.cpp
inline CKoWebDoc* CKoWebView::GetDocument() const
{
	return reinterpret_cast<CKoWebDoc*>(m_pDocument);
}
#endif

