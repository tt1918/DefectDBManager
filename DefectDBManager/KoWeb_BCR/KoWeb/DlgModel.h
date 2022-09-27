#pragma once


// CDlgModel 대화 상자입니다.

class CDlgModel : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgModel)

public:
	CWnd   *m_pParent;
	CDlgModel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgModel();


	void FillModelList();
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MODEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSave();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSelect();
	CListBox m_ctrlModelList;
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonSaveas();
};
