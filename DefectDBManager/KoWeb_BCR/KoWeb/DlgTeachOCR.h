#pragma once


// CDlgTeachOCR 대화 상자입니다.

class CDlgTeachOCR : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTeachOCR)

public:
	CDlgTeachOCR(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTeachOCR();

	CWnd			*m_pParent;
	CCossImage		*m_pDisplay;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TEACHOCR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedStaticOcr();
	virtual BOOL OnInitDialog();
	CComboBox m_ctrlOCR;
	CSpinButtonCtrl m_spinOCR;
	int m_nSizeX;
	int m_nSizeY;
	int m_nShowID;
	CListBox m_ctrlResult;
	BOOL m_bCheckSmooth;
	BOOL m_bCheckOverlap;
	BOOL m_bTeachROI;
	BOOL m_bSearchROI;
	int m_nSelect;

	void SelectOCR(int N);
	void ShowOCR(int N, int M);
	void OpenDisplay(int nWidth, int nHeight, CRect rcDisp);
	void RegisterOCR();

	afx_msg void OnBnClickedButtonRegister();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedCheckShowTeachroi();
	afx_msg void OnBnClickedCheckShowSearchRoi();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonPrev();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnCbnSelchangeComboOcr();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedOk();
};
