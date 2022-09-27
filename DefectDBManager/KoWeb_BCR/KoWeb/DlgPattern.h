#pragma once


// CDlgPattern 대화 상자입니다.

class CDlgPattern : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPattern)

public:
	CDlgPattern(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgPattern();

	int              m_nId;
	CWnd			*m_pParent;
	CCossImage		*m_pDisplay;

	void OpenDisplay(int nWidth, int nHeight, CRect rcDisp);
	void AddPattern(int nID, LPBYTE fm, int left, int top, int right, int bottom, int pitch, CString sComment);
	void SelectPattern(int nID);
	

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PATTERN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboPatternid();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckChangeReturn();
	afx_msg void OnBnClickedButtonLeft();
	afx_msg void OnBnClickedButtonRight();
	afx_msg void OnBnClickedButtonUp();
	afx_msg void OnBnClickedButtonDown();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonRegister();
	afx_msg void OnBnClickedButtonSearch();
	CComboBox m_ctrlPatternID;
	CListBox m_ctrlResult;
	int m_nMinScore;
	afx_msg void OnBnClickedCheckShowroi();
	afx_msg void OnBnClickedButtonApplyall();
	afx_msg void OnBnClickedOk();
	BOOL m_nROI;
	CString m_sComment;
	BOOL m_bChangePos;
};
