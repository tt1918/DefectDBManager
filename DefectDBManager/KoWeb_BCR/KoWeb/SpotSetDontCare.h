#pragma once


// CSpotSetDontCare 대화 상자입니다.

class CSpotSetDontCare : public CDialogEx
{
	DECLARE_DYNAMIC(CSpotSetDontCare)

public:
	CSpotSetDontCare(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSpotSetDontCare();

	LPBYTE		     m_fm;
	LPBYTE		     m_fmMask;
	int              m_nWidth;
	int              m_nHeight;

	int              m_nSet;
	int              m_nID;
	int              m_nPtCount;
	POINT            m_pt[50];
	CCossImage		*m_pDisplay;
	bool             m_bRemakeMinMax;
	
	void SetImageInfo(int nSet, int nId, LPBYTE fm, int pitch);
	void OpenDisplay(LPBYTE fm, int nWidth, int nHeight, CRect rcDisp);

	RECT   m_rcGraph;       //그래프 그릴영역 
	CRect  m_rcTh1;			//Mouse로 Drag할 조그만 사각형	
	CRect  m_rcTh2;        
	int    m_MaxHisto;
	int    m_Histo[256]; //Histogram 데이타
	int    m_SelectThreshold; //어떤 Treshold가 선택이 됐는지?
	CPoint m_SelectPoint;
	int    GetHistogram(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, int *Histo);
	void   InvalidateUserDefine();
	void Binary(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nTh1, int nTh2, int nInverse);


// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SPOT_DONTCARE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL m_nROI;
	BOOL m_MultiLine;
	BOOL m_GrayValue;
	BOOL m_nInverse;
	int m_nThreshold1;
	int m_nThreshold2;
	CComboBox m_ctrlZoom;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedCheckRoi();
	afx_msg void OnBnClickedCheckRoi2();
	afx_msg void OnBnClickedCheckRoi3();
	afx_msg void OnBnClickedCheckInverse();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonAddAll();
	afx_msg void OnBnClickedButtonErase();
	afx_msg void OnBnClickedButtonEraseAll();
	afx_msg void OnBnClickedButtonDilate();
	afx_msg void OnBnClickedButtonErode();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};
