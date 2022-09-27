#pragma once


// CUserSet 대화 상자입니다.

class CUserSet : public CDialogEx
{
	DECLARE_DYNAMIC(CUserSet)

public:
	CUserSet(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CUserSet();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_USER };

	//-----------------------------------
	CWnd *m_pParent;
	void SetInspectID(int nSet, int nID);
	int  m_nSet;
	int  m_nID;
	//-----------------------------------
	void Local2Global();
	void Global2Local();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ctrlResult;
	int m_nData;
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
