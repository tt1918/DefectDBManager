#pragma once


// CDlgDebug 대화 상자입니다.

class CDlgDebug : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDebug)

public:
	CDlgDebug(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgDebug();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_DEBUG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ctrlData;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonReset();
};
