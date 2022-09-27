#pragma once

#include "CameraCalib.h"
// CDlgConfig 대화 상자입니다.

class CDlgConfig : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgConfig)

public:
	CDlgConfig(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgConfig();

	CCameraCalib *m_pCameraCalib;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_nImageType;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	int m_nSaveMethod;
	afx_msg void OnBnClickedButtonSetFrameNumber();
	int m_nFrameNumber;
	BOOL m_bShowProfile;
	double m_dCalibWidth;
	BOOL m_bCalibWidth;
	afx_msg void OnBnClickedButtonSetScale();
	double m_dScaleFactorX;
	afx_msg void OnBnClickedCheckCalibwidth();
	BOOL m_bImageInverse;
	BOOL m_nModePeriod;
	afx_msg void OnBnClickedCheckPeriod();
	afx_msg void OnBnClickedButtonComportset();
	int m_nComPort;
	int m_nSaveNumber;
	afx_msg void OnBnClickedRadioSaveMethod();
	afx_msg void OnBnClickedRadioSaveMethod2();
	afx_msg void OnBnClickedRadioSaveMethod3();
	afx_msg void OnBnClickedButtonTooset();
	afx_msg void OnDeltaposSpinMotion(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonRotate();
	afx_msg void OnBnClickedCheckShowprofile();
	double m_dCalibStartX;
};
