// CustomButton.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "CustomButton.h"


// CCustomButton

IMPLEMENT_DYNAMIC(CCustomButton, CButton)

CCustomButton::CCustomButton()
{
	m_bSelectFlag = 0;
}

CCustomButton::~CCustomButton()
{
}


BEGIN_MESSAGE_MAP(CCustomButton, CButton)
END_MESSAGE_MAP()



// CCustomButton 메시지 처리기입니다.




void CCustomButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  지정된 항목을 그리는 코드를 추가합니다.
	CDC dc;
	RECT rect;

	dc.Attach(lpDrawItemStruct->hDC);	//Get the Button DC to CDC

	rect = lpDrawItemStruct->rcItem	;	//Store the Button rect to local rect

	if(m_bSelectFlag)
	{
		dc.Draw3dRect(&rect, RGB(200,200,200), RGB(20,20,20));
		dc.FillSolidRect(&rect, RGB(100,150,100));

		dc.SetBkColor(RGB(100,150,100));		//Setting the Text Background Color
		dc.SetTextColor(RGB(255,255,255));		//Setting the Text Color
	}
	else
	{
		dc.Draw3dRect(&rect, RGB(100,100,100), RGB(20,20,20)); //???
		dc.FillSolidRect(&rect, RGB(10,15,10));	//버튼전체 칠하기
	
		dc.SetBkColor(RGB(10,15,10));			//Setting the Text Background Color
		dc.SetTextColor(RGB(255,255,255));		//Setting the Text Color	
	}

	//Show the Effect of Click Event
	UINT state = lpDrawItemStruct->itemState;
	if((state & ODS_SELECTED))
	{
		dc.DrawEdge(&rect, EDGE_SUNKEN, BF_RECT);
	}
	else
	{
		dc.DrawEdge(&rect, EDGE_RAISED, BF_RECT);
	}

	TCHAR buffer[MAX_PATH];
	ZeroMemory(buffer, MAX_PATH);
	::GetWindowText(lpDrawItemStruct->hwndItem, buffer, MAX_PATH);

	dc.DrawText(buffer, &rect, DT_CENTER|DT_VCENTER |DT_SINGLELINE);
	dc.Detach();							//Detach the Button DC

}
