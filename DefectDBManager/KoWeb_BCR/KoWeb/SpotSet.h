#pragma once
#include "fpspread_data.h"


// CSpotSet 대화 상자입니다.

class CSpotSet : public CDialogEx
{
	DECLARE_DYNAMIC(CSpotSet)

public:
	CSpotSet(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSpotSet();

	//@항목추가12 -------------------------
	CWnd *m_pParent;
	void SetInspectID(int nSet, int nID);
	int  m_nSet;
	int  m_nID;
	//-------------------------------------

	void UpdateSet(bool flag);

	void Global2Local();
	void Local2Global();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SPOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDontcare();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CFpspread_data m_Param;
	afx_msg void OnBnClickedCheckComp4();
	afx_msg void OnBnClickedCheckComp0();
	afx_msg void OnBnClickedCheckComp1();
	afx_msg void OnBnClickedCheckComp2();
	afx_msg void OnBnClickedCheckComp3();
	afx_msg void OnBnClickedRadioBump();
	afx_msg void OnBnClickedRadioInspType();
	afx_msg void OnBnClickedCancel();
};
