#pragma once


// CFindLineSet 대화 상자입니다.

class CFindLineSet : public CDialogEx
{
	DECLARE_DYNAMIC(CFindLineSet)

public:
	CFindLineSet(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFindLineSet();

	//--------------------------------------
	CWnd *m_pParent;
	void SetInspectID(int nSet, int nID);
	int  m_nSet;
	int  m_nID;
	//--------------------------------------

	void Global2Local();
	void Local2Global();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_FINDLINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nDir;
	int m_nLineType;
	int m_nMethod;
	int m_nObject;
	CListBox m_ctrlResult;
	int m_nGrayTh;
	int m_nSlopeTh;
	int m_nAvgLine;
	double m_dMinAngle;
	double m_dMaxAngle;
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedOk();
	BOOL m_bDiagonal;
	afx_msg void OnBnClickedRadioLinetype();
	afx_msg void OnBnClickedRadioLinetype2();
};
