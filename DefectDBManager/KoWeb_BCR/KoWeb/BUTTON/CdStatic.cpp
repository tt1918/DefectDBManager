// CdStatic.cpp : implementation file
//

#include "stdafx.h"
#include "CdStatic.h"


// CdStatic

IMPLEMENT_DYNAMIC(CdStatic, CStatic)

CdStatic::CdStatic() : m_TextAlign(DT_VCENTER)
{
	m_crTextColor = RGB(250, 255, 255);					// Initializing the text to Black
	m_brBkgnd.CreateSolidBrush(RGB(0, 0, 0)); // Create the Brush Color for the Background.
}

CdStatic::~CdStatic()
{
}

BEGIN_MESSAGE_MAP(CdStatic, CStatic)
	ON_WM_CTLCOLOR_REFLECT()
#ifdef CDSTATIC_ONPAINT
	ON_WM_PAINT()
#endif
END_MESSAGE_MAP()

// CdStatic message handlers

HBRUSH CdStatic::CtlColor(CDC* pDC, UINT nCtlColor)
{
	HBRUSH hbr;
	hbr = (HBRUSH)m_brBkgnd; // Passing a Handle to the Brush
	pDC->SetBkColor(m_crBkColor); // Setting the Color of the Text Background to the one passed by the Dialog
	pDC->SetTextColor(m_crTextColor); // Setting the Text Color to the one Passed by the Dialog

	if (nCtlColor)       // To get rid of compiler warning
		nCtlColor += 0;

	return hbr;

}

void CdStatic::SetBkColor(COLORREF crColor)
{
	m_crBkColor = crColor; // Passing the value passed by the dialog to the member varaible for Backgound Color
	m_brBkgnd.DeleteObject(); // Deleting any Previous Brush Colors if any existed.
	m_brBkgnd.CreateSolidBrush(crColor); // Creating the Brush Color For the Static Text Background
	RedrawWindow();
}

void CdStatic::SetTextColor(COLORREF crColor)
{
	m_crTextColor = crColor; // Passing the value passed by the dialog to the member varaible for Text Color
	RedrawWindow();
}

void	CdStatic::SetColors(COLORREF _textColor, COLORREF _bgColor)
{
	SetTextColor(_textColor);
	SetBkColor(_bgColor);
}

#ifdef CDSTATIC_ONPAINT
void CdStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CStatic::OnPaint() for painting messages
	CRect wndRect;
	GetWindowRect( &wndRect );
	GetClientRect( &wndRect );

	CRect rect( wndRect );

	CString strText;
	GetWindowText( strText );

	CFont* pOldFont = dc.SelectObject( GetFont() );
	int nOldMode = dc.SetBkMode( TRANSPARENT );

	dc.DrawText( strText, &rect, m_TextAlign | DT_WORDBREAK | DT_CALCRECT );

	rect.OffsetRect( wndRect.CenterPoint().x - rect.CenterPoint().x, wndRect.CenterPoint().y - rect.CenterPoint().y );

	dc.DrawText( strText, &rect, m_TextAlign | DT_WORDBREAK );

	dc.SetBkMode( nOldMode );
	dc.SelectObject( pOldFont );
}
#endif
