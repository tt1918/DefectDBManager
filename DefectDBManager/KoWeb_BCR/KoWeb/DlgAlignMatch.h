#pragma once


// CDlgAlignMatch 대화 상자입니다.

class CDlgAlignMatch : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAlignMatch)

public:
	CDlgAlignMatch(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgAlignMatch();

	int m_nSet;
	int m_nId;
	void SetAlignData(int nSet, int nId);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ALIGN_PATTERN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nMinScore;
	int m_nPatternId;
	afx_msg void OnBnClickedOk();
};
