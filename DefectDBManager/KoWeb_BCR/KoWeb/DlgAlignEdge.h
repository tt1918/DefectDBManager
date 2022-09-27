#pragma once


// CDlgAlignEdge 대화 상자입니다.

class CDlgAlignEdge : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAlignEdge)

public:
	CDlgAlignEdge(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgAlignEdge();

	int m_nSet;
	int m_nId;
	void SetAlignData(int nSet, int nId);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ALIGN_EDGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	int m_nDir;
	int m_nMethod;
	int m_nObject;
	int m_nSlope;
	int m_nGrayTh;
	int m_nAveLine;
};
