#pragma once
#include "fpspread_data.h"
#include "afxcmn.h"


// CDlgParam 대화 상자입니다.
#define SHOW_Y_AREA 40
class CDlgParam : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgParam)

public:
	CDlgParam(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgParam();
	CRect m_rcShow;
	COLORREF m_Color[3];
	int  m_nX[3];
	int  m_nY[SHOW_Y_AREA+1];
	int  m_nOffset;



	CWnd *m_pWndEdit;
	int  m_nClickY;
	int  m_nOldClickY;
	CRect rcControl;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PARAM };
	
	void UpdateSpread(bool flag);
	void UpdateEdit(bool flag);

	PARAMETER_VER	m_ParamVer;
	void DrawData(CDC &dc);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSaveApply();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonReadparameter();

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
