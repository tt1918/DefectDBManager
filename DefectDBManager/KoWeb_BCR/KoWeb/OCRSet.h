#pragma once


// COCRSet 대화 상자입니다.

class COCRSet : public CDialogEx
{
	DECLARE_DYNAMIC(COCRSet)

public:
	COCRSet(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~COCRSet();

	//@항목추가12 -------------------------
	CWnd *m_pParent;
	void SetInspectID(int nSet, int nID);
	int  m_nSet;
	int  m_nID;
	//-------------------------------------

	void Global2Local();
	void Local2Global();
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_INSPECTOCR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int m_nSizeX;
	int m_nSizeY;
	CListBox m_ctrlResult;
	int m_nCharNum;
	afx_msg void OnBnClickedButtonTest();
	virtual BOOL OnInitDialog();
	int m_nCharPitch;
	afx_msg void OnBnClickedOk();
	BOOL m_nEqualPitch;
};
