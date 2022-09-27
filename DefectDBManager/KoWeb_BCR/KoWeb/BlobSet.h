#pragma once
#include "fpspread_data.h"


// CBlobSet 대화 상자입니다.

class CBlobSet : public CDialogEx
{
	DECLARE_DYNAMIC(CBlobSet)

public:
	CBlobSet(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBlobSet();

	//--------------------------------------
	CWnd *m_pParent;
	void SetInspectID(int nSet, int nID);
	int  m_nSet;
	int  m_nID;
	//--------------------------------------
	
	void UpdateSet(bool flag);
	void Global2Local();
	void Local2Global();
	void ShowReault();
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_BLOB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_ctrlThreshold;
	int m_nObject;
	afx_msg void OnBnClickedButtonGray();
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedOk();
	CFpspread_data m_Param;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMCustomdrawSliderBinary(NMHDR *pNMHDR, LRESULT *pResult);
};
