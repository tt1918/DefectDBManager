#pragma once


// CDlgModelName 대화 상자입니다.

class CDlgModelName : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgModelName)

public:
	CDlgModelName(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgModelName();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MODELNAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_sModelName;
	afx_msg void OnBnClickedOk();
};
