#pragma once


// CCameraCalib 대화 상자입니다.

class CCameraCalib : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraCalib)

public:
	CCameraCalib(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCameraCalib();

	int m_nLDataOld;
	int m_nRDataOld;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CAMERA_CALIB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
//	int m_nLData;
//	int m_nRData;
	afx_msg void OnDeltaposSpinLeft(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinRight(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedSave();
	int m_nDataL;
	int m_nDataR;
};
