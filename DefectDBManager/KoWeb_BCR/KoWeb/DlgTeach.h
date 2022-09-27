#pragma once
#include "fpspread_data.h"


// CDlgTeach 대화 상자입니다.

class CDlgTeach : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTeach)

public:
	CDlgTeach(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgTeach();

	CWnd	*m_pParent;

	void UpdateInspectData();
	void FillInspectData();
	void AddInspectRow(int nSet, int nShowCount, int nNum);
	
	void HideROI();			//
	void BackUpROIData();	//다이얼로그 닫을때 ROI Backup
	void OpenSetDialog(int nID);
	void DeleteInspect(int nID);
	void UpdateResult();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TEACH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonInsert();
	DECLARE_EVENTSINK_MAP()
	void ButtonClickedFpspread1(long Col, long Row, short ButtonDown);
	CFpspread_data m_Spread;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonTest();
	CComboBox m_ctrlType;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonShowGrayimage();
};
