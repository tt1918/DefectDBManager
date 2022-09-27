#pragma once


// CMeasureSet 대화 상자입니다.

class CMeasureSet : public CDialogEx
{
	DECLARE_DYNAMIC(CMeasureSet)

public:
	CMeasureSet(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMeasureSet();

	//-------------------------------------
	CWnd *m_pParent;
	void SetInspectID(int nSet, int nID);
	int  m_nSet;
	int  m_nID;
	//-------------------------------------
	void Local2Global();
	void Global2Local();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MEASURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedOk();
	double m_dMinDist;
	double m_dMaxDist;
	CListBox m_ctrlResult;
	int m_nBaseId;
	int m_nPointId;
	virtual BOOL OnInitDialog();
};
