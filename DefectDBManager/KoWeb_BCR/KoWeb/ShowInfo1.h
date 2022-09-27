#pragma once
#include "afxwin.h"


// CShowInfo1 대화 상자입니다.

class CShowInfo1 : public CDialogEx
{
	DECLARE_DYNAMIC(CShowInfo1)

public:
	CShowInfo1(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CShowInfo1();
	CWnd *m_pWnd;

	CRect m_rcNG;
	int m_nX;
	int m_nXEnd;

	int m_nY[20];
	int m_nYCnt;
	int m_nLatestResult;

	COLORREF m_ColorBK;
	COLORREF m_Color[2];
	CString  m_sTitle[20];
	CString  m_sData[20];
	int      m_nColor[20];

	
	void ShowParamData(int nExposure, int nNotInspect, int nEdgeOffset);
	void SetData(int nRow,CString str, int nColor);
	void DrawTitle(CPaintDC &dc);
	void CheckAllReset();
	void SetResult(int nResult);

	CFont m_BTFont;
	void initButtonST(int nType, CButtonST *cButton);
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SHOWINFO1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSetexposure();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedCancel();
	int m_nExposure;
	afx_msg void OnBnClickedCheckShowFlat();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedCheckShowArea();
	afx_msg void OnBnClickedCheckShowAllcandi();
	afx_msg void OnBnClickedCheckShowValidcandi();
	BOOL m_bShowFlat;
	BOOL m_bShowCandiArea;
	BOOL m_bShowAllCandi;
	BOOL m_bShowValidCandi;
	afx_msg void OnBnClickedButtonSetexposure2();
	afx_msg void OnBnClickedButtonSetexposure3();
	int m_nNotInspectArea;
	int m_nEdgeOffset;
	CButtonST m_btData1;
	CButtonST m_btData2;
	CButtonST m_btData3;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
