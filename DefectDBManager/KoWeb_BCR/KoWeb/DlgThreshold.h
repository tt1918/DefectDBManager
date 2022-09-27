#pragma once


// CDlgThreshold 대화 상자입니다.

class CDlgThreshold : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgThreshold)

public:
	CDlgThreshold(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgThreshold();
	CWnd *m_pWnd;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_THRESHOLD };
	void SetImageInfo(double dAve, double dStdev, int nMax, int nMin, double dSharp, int nSizeX, int nSizeY, int nWaterPrintValue, double dWaterValue);
	int m_nAve;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CSliderCtrl m_ctrlBinary;
	int m_nThreshold;
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	int m_nThSub;
	afx_msg void OnBnClickedButtonShowGray();
	afx_msg void OnBnClickedButtonRotate();
	double m_dAngle;
	BOOL m_bCheckBinary;
	afx_msg void OnBnClickedCheckBinary();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonFlipHor();
	afx_msg void OnBnClickedButtonFlipVer();
	int m_nElongX;
	afx_msg void OnBnClickedButtonElong();
	afx_msg void OnBnClickedButtonInverse();
	int m_nElongY;
//	int m_nSmooth;
	afx_msg void OnBnClickedButtonSmoothing();
	afx_msg void OnBnClickedButtonEmph();
	int m_nEmpX;
	int m_nEmpY;
	CListBox m_ctrlImageInfo;
	BOOL m_bFullImage;
	afx_msg void OnBnClickedCheckFullimage();
	CComboBox m_ctrlFlat;
	BYTE m_nBaseGray;
	afx_msg void OnBnClickedButtonFlat();
	afx_msg void OnBnClickedButtonSave();
	double m_dData1;
	double m_dData2;
	afx_msg void OnBnClickedButtonTest();
	double m_dData3;
	double m_dData4;
	afx_msg void OnBnClickedButtonRemovenoise();
	int m_nNoiseTh;
	afx_msg void OnBnClickedButtonTest2();
	afx_msg void OnBnClickedButtonTest3();
	afx_msg void OnBnClickedButtonCutLowhigh();
	int m_nCutHigh;
	int m_nCutLow;
	int m_nOffset;
	int m_nOffsetValue;
	BOOL m_bWaterPrint;
	afx_msg void OnBnClickedCheckWaterprint();
	int m_nRemoveKernel;
	int m_nMinYLength;
	afx_msg void OnBnClickedButtonSmoothing2();
};
