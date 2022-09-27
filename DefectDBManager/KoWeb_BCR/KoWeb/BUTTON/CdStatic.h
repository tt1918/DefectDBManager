#pragma once

//#define	 CDSTATIC_ONPAINT	1
// CdStatic

class CdStatic : public CStatic
{
	DECLARE_DYNAMIC(CdStatic)

public:
	CdStatic();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CdStatic)
	//}}AFX_VIRTUAL
	virtual ~CdStatic();

	void SetTextColor(COLORREF crColor); // This Function is to set the Color for the Text.
	void SetBkColor(COLORREF crColor); // This Function is to set the BackGround Color for the Text.
	void SetColors(COLORREF _textColor, COLORREF _bgColor);
protected:
	//{{AFX_MSG(CdStatic)
	UINT	m_TextAlign;
	CBrush	m_brBkgnd; // Holds Brush Color for the Static Text
	COLORREF m_crBkColor; // Holds the Background Color for the Text
	COLORREF m_crTextColor; // Holds the Color for the Text

	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
#ifdef CDSTATIC_ONPAINT
	afx_msg void OnPaint();
#endif
};


