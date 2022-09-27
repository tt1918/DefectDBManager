#pragma once


// CROIAngle 대화 상자입니다.

class CROIAngle : public CDialogEx
{
	DECLARE_DYNAMIC(CROIAngle)
 
public:
	CWnd *m_pParent;
	CROIAngle(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CROIAngle();

	//{{AFX_DATA(CROIAngle)
	enum { IDD = IDD_DLG_ANGLE };
	CSliderCtrl	m_ctrlThresholdL;
	CSliderCtrl	m_ctrlThreshold;
	CSliderCtrl	m_ctrlAngle;
	int		m_nAngle;
	BOOL	m_bAutoThreshold;
	int		m_nThreshold;
	int		m_nThresholdL;
	BOOL	m_bInverse;
	//}}AFX_DATA

	CROIControl	*m_pROI;
	int m_nCam;

	int m_nThresholdOld;
	int m_nThresholdLOld;

	int m_nROINumber;
	void Binary_OnlyDisplay();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	//{{AFX_MSG(CROIAngle)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonZero();
	afx_msg void OnCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSlider2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckAuto();
	afx_msg void OnReleasedcaptureSlider2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSlider3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSlider3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckReverse();
	afx_msg void OnButtonGray();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonZero();
	afx_msg void OnBnClickedButtonGray();
	afx_msg void OnBnClickedCheckAuto();
	afx_msg void OnBnClickedCheckReverse();
	afx_msg void OnEnChangeEditAngle();
};


/*
class CROIAngle : public CDialog
{
// Construction
public:
	CROIAngle(CWnd* pParent = NULL);   // standard constructor

	CWnd *m_pParent;
// Dialog Data
	//{{AFX_DATA(CROIAngle)
	enum { IDD = IDD_DIALOG_ROIANGLE };
	CSliderCtrl	m_ctrlThresholdL;
	CSliderCtrl	m_ctrlThreshold;
	CSliderCtrl	m_ctrlAngle;
	int		m_nAngle;
	BOOL	m_bAutoThreshold;
	int		m_nThreshold;
	int		m_nThresholdL;
	BOOL	m_bInverse;
	//}}AFX_DATA

	CROIControl	*m_pROI;
	int m_nCam;

	int m_nThresholdOld;
	int m_nThresholdLOld;

	int m_nROINumber;
	void Binary_OnlyDisplay();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CROIAngle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CROIAngle)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonZero();
	afx_msg void OnCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSlider2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckAuto();
	afx_msg void OnReleasedcaptureSlider2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSlider3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSlider3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCheckReverse();
	afx_msg void OnButtonGray();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
*/