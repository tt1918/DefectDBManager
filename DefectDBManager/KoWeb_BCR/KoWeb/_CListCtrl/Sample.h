#pragma once
#include "fpspread_data.h"
#include "afxcmn.h"


// CDlgParam 대화 상자입니다.

class CDlgParam : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgParam)

public:
	CDlgParam(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgParam();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PARAM };
	
	void UpdateSpread(bool flag);

	
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:

	_CListCtrl<CListCtrl> m_ctrlParam1;
	_CListCtrl<CListCtrl> m_ctrlGrade;
	_CListCtrl<CListCtrl> m_ctrlParam2;

	afx_msg void OnBnClickedButtonSaveApply();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonReadparameter();

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
};
