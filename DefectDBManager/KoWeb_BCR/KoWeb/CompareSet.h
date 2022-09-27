#pragma once


// CCompareSet 대화 상자입니다.

class CCompareSet : public CDialogEx
{
	DECLARE_DYNAMIC(CCompareSet)

public:
	CCompareSet(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCompareSet();

	//@항목추가12 -------------------------
	CWnd *m_pParent;
	void SetInspectID(int nSet, int nID);
	int  m_nSet;
	int  m_nID;
	//-------------------------------------

	void Local2Global();
	void Global2Local();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_COMPARE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int m_nData;
	virtual BOOL OnInitDialog();
};
