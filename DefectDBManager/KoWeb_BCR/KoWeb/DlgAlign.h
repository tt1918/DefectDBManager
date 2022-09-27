#pragma once


// CDlgAlign 대화 상자입니다.

class CDlgAlign : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAlign)

public:
	CDlgAlign(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgAlign();

	//--------------------------------------
	CWnd *m_pParent;
	void SetID(int nSet);
	int  m_nSet;
	//--------------------------------------
	
	void ShowROI();
	void SetSelectAlignType(int N);
	void SetData(int N, CString str);

	void Local2Global();
	void Global2Local();

	void UpdateGUI();  //메인상황이 바뀌었을떄, 즉, 카메라 변경시 달라져야 하는 GUI

	int  m_bShowROIBK[4];	//최대 4개 MAX_IMAGE
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ALIGN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_ctrlType;
	afx_msg void OnBnClickedCheckShowroi();
	BOOL m_bShowROI;
	afx_msg void OnCbnSelchangeComboType();
	afx_msg void OnBnClickedButtonDataset0();
	afx_msg void OnBnClickedButtonDataset1();
	afx_msg void OnBnClickedButtonDataset2();
	afx_msg void OnBnClickedButtonDataset3();
	BOOL m_nNoApply;
	afx_msg void OnBnClickedButtonFindpos();
	afx_msg void OnBnClickedButtonSetalign();
	afx_msg void OnBnClickedOk();
	CListBox m_ctrlResult;
	double m_dMaxAngle;
	double m_dMaxOffsetX;
	double m_dMaxOffsetY;
	afx_msg void OnBnClickedButtonLoadmaster();
	afx_msg void OnBnClickedButtonSetalign2();
};
