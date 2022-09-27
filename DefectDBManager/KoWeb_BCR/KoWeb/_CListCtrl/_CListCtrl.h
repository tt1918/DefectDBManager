#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "_C2DVector.h"
#include "_Debug.h"
#include "_GetDllVersion.h"
#include "shlwapi.h"

// *** _SetItemControl()를 호출할 때 아래의 값을 넣어 주면
		// 해당 control을 생성하고 subitem에 추가시켜 준다.
		// 보통 그렇듯이 이 enum형 상수를 class 내부에 포함시키면 인수의 데이터형이 UINT가 되겠지만
		// 외부에 정의하면 그 인수가 enum형임을 사용자가 명확히 인지할 수 있게  만들 수 있다.
enum _enumCONTROLTYPE
{
	_CT_CStatic, // Text for labeling other controls
	_CT_CButton, // Pushbuttons that cause an action; also used for check boxes, radio buttons, and group boxes
	_CT_CEdit, // Boxes for entering text
	_CT_CComboBox, // Combination of an edit box and a list box
	_CT_CListBox, // Box that contains a list of strings
	_CT_CAnimateCtrl, // Displays successive frames of an AVI video clip
	_CT_CDateTimeCtrl, // Allows the user to choose a specific date or time value
	_CT_CComboBoxEx, // A combo box control with the ability to display images
	_CT_CHeaderCtrl, // Button that appears above a column of text; controls width of text displayed
	_CT_CHotKeyCtrl, // Window that enables user to create a "hot key" to perform an action quickly
	_CT_CImageList, // Collection of images used to manage large sets of icons or bitmaps (image list isn't really a control; it supports lists used by other controls)
	_CT_CListCtrl, // Window that displays a list of text with icons
	_CT_CMonthCalCtrl, // Control that displays date information
	_CT_CProgressCtrl, // Window that indicates progress of a long operation
	_CT_CRebarCtrl, // Tool bar that can contain additional child windows in the form of controls
	_CT_CRichEditCtrl, // Window in which user can edit with character and paragraph formatting (see Classes Related to Rich Edit Controls)
	_CT_CScrollBar, // Scroll bar used as a control inside a dialog box (not on a window)
	_CT_CSliderCtrl, //Window containing a slider control with optional tick marks
	_CT_CSpinButtonCtrl, // Pair of arrow buttons user can click to increment or decrement a value
	_CT_CStatusBarCtrl, // Window for displaying status information, // similar to MFC class CStatusBar
	_CT_CTabCtrl, // Analogous to the dividers in a notebook; used in "tab dialog boxes" or property sheets
	_CT_CToolBarCtrl, // Window with command-generating buttons, // similar to MFC class CToolBar
	_CT_CToolTipCtrl, // Small pop-up window that describes purpose of a toolbar button or other tool
	_CT_CTreeCtrl, // Window that displays a hierarchical list of items
	_CT_NONE
};


template <class T>
class _CListCtrl : public T // CListCtrl(정상 작동), CMFCListCtrl(정상 작동), CMFCShellListCtrl(정적생성될 때와 종료할 때 runtime error) 
//class _CListCtrl : public CListCtrl // intellisense
{
	// 생성자, 파괴자
public:
	_CListCtrl() { this->_Init(); }
	virtual ~_CListCtrl() {}

protected:
	DECLARE_MESSAGE_MAP()

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// subitem의 contents info 다루기 - 시작
	//
	// subitem에 대한 정보가 저장되는 _SUBITEM_INFO를
	// SetItemData, GetItemData를 통해 저장, 호출하여 사용하는 것도 가능하다.
	// 하지만, 나중에 이 class를 사용하는 사용자가 SetItemData, GetItemData를 사용하여
	// LVITEM(lParam)를 변경할 수도 있어 위험하다.
	// 예를 들어 SortItems 를 하기 위해서는 lParam를 사용해야 하는데
	// lParam를 _CONTENS INFO를 저장하는 용도와 SortItems 용도로 동시에 사용하려면 번거롭다.
	// 사용자가 접근할 수 없는 별도의 저장 공간을 사용하도록 하자.
	// CHeaderCtrl* pHeaderCtrl = __super::GetHeaderCtrl();로 얻은 header control를 통해
	// column의 최소너비를 SetItem, GetItem을 통해 저장, 호출하여 사용하는 것도 가능하다.
	// 하지만, 마찬가지로 나중에 이 class를 사용하는 사용자가 SetItem, GetItem을 사용하여
	// HDITEM(lParam)를 변경할 수도 있어 위험하다.
	// 이 또한 사용자가 접근할 수 없는 별도의 저장 공간을 사용하도록 하자.
	// Header control이 아닌 column에 접근할 수 있는 CListCtrl member 함수 GetColumn, SetColumn도
	// Header control에 접근하는 SetItem, GetItem처럼 위험하다.
	// 2차원 vector 공간과 subitem을 일대일 대응시킬 수 있도록 _C2DVector를 이용하자.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*********************************************************************************************************************************
	* subitem의 contents info 구조와 2차원 vector
	*********************************************************************************************************************************/
private:
	// *** 행(row)과 열(coloumn)의 개수
	int _m_cRows;
	int _m_cCols;	

	// *** subitem 정보
	struct _SUBITEM_INFO
	{
		// _SetItemColor()로 설정
		COLORREF clrTextBk;	// subitem의 배경색
		COLORREF clrText;		// text의 색

		// subitem 안쪽 여백
		#define _PADDING_X 2
		#define _PADDING_Y 2
		#define _PADDING_X_DT_SINGLELINE 6

		// _SetItemFormat()로 설정
		UINT uDTFormatFlags; // DrawText() Format Flags를 조합한 값을 나타낸다.
									// 그 flag들 중에서 DT_TOP, DT_LEFT, DT_RIGHT, DT_BOTTOM, DT_CENTER, DT_VCENTER는
									// 그 사용 범위를 확장하여 control을 align하는데도 적용된다.

		// _SetItemControl()로 설정
		CWnd* pWnd; // handel to the added control in the subitem. _SetItemControl()에 의해 추가되는 control의 handle이다.

		_SUBITEM_INFO() { _Init(); }
		void _Init()
		{
			this->clrTextBk = ::GetSysColor(COLOR_WINDOW);
			this->clrText = ::GetSysColor(COLOR_WINDOWTEXT);
			this->uDTFormatFlags = DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS | DT_PATH_ELLIPSIS;
			this->pWnd = NULL;
		}
	};
	_C2DVector<_SUBITEM_INFO> _m_vSubItem;

	// *** column의 너비를 줄일 수 있는 한계
	#define _LV_DEFAULT_COLUMNWIDTH 10
	vector<LONG> _m_vColumnWidthLowerLimit;

	// *** row의 높이, list control은 row의 높이를 개별적으로 설정할 수 없도록 설계되어 있어 모든 row의 높이는 동일하다.
	#define _LV_DEFAULT_ROWHEIGHT 17
	LONG _m_nRowHeight;
	
	// *** _Init()
	void _Init(void)
	{
		this->_m_cRows = 0;
		this->_m_cCols = 0;

		this->_m_nRowHeight = _LV_DEFAULT_ROWHEIGHT;

		// control 보기 방식 설정
		this->_m_bAllControlsViewState = true;
	}

	/*********************************************************************************************************************************
	* row, column의 개수 및 index의 유효성 검사
	*********************************************************************************************************************************/
private:
	bool _IsSubItem(int nRow, int nCol)
	{
		if (nRow >= 0 && nRow < this->_m_cRows
			&& nCol >= 0 && nCol < this->_m_cCols) return true;
		return false;
	}
	bool _IsCol(int nCol)
	{
		if (nCol >= 0 && nCol < this->_m_cCols) return true;
		return false;
	}
	bool _IsRow(int nRow)
	{
		if (nRow >= 0 && nRow < this->_m_cRows) return true;
		return false;
	}
public:
	int _GetColumnCount() // CHeaderCtrl::GetItemCount() 과 동일
	{
		return this->_m_cCols;
	}
	int _GetRowCount() // __super::GetItemCount() 과 동일
	{
		return this->_m_cRows;
	}

	/*********************************************************************************************************************************
	* message별로 contents 정보 처리
	* LVM_INSERTCOLUMN,  LVM_INSERTITEM, LVM_DELETECOLUMN, LVM_DELETEITEM, LVM_DELETEALLITEMS, LVM_SORTITEMS
	* LVM_SETCOLUMN, LVM_SETITEMTEXT
	* 아래의 LVM_ message 를 처리하는 scope는 해당 함수들이 완료되기 이전에 호출된 것이므로
	* CListCtrl이나 CHeaderCtrl의 member함수를 사용하여 LVITEM, LVCOLUMN 정보를 얻으면 정상적이지 않다.
	* 변경하고자 하는 사항이 있으면 각 구조체의 mask를 통해 수정하도록 한다.
	*********************************************************************************************************************************/
protected:
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
		// __super::OnWndMsg 에서
		// message 처리 전에 column과 row의 개수를 구할 때
		// message 처리가 성공한다고 가정하고 그 개수를 +1 해준다.
		// message 처리가 실패하면 문제가 발생할 소지가 있어
		// message 처리 후에 그 개수를 구하는 것이 바람직해 보인다.
		// 하지만, message 처리 전이나 후나 그 개수를 구해보면
		// 동일하다. 
		if (message == LVM_INSERTCOLUMN // LVM_INSERTCOLUMN
			|| message == LVM_SETCOLUMN) // LVM_SETCOLUMN
		{
			int nCol = wParam;
			if (message == LVM_INSERTCOLUMN
				&& /*__FVERIFY*/(this->_IsCol(nCol) || nCol == this->_m_cCols))
			{
				// nCol이 0일 때, 0번과 1번 column 사이에 새 column을 삽입한다.
				// 그래서 nCol이 0일 때 nCol을 1로 변경한다.
				// 물론 column이 존재하지 않을 때 nCol을 1로 변경해도 아래 code에서 다시 0으로 변경되니 문제되지 않는다.
				// list control 자체 bug : 그런데, 이때 새 header item이 0번 앞에 삽입되는 문제가 발생한다.
				//								이럴 때 column의 이름과 너비를 user가 변경해 주어야 한다.
				if (nCol == 0) nCol = 1;
				if (!this->_IsCol(nCol)) nCol = this->_m_cCols;
				_SUBITEM_INFO sii;
				this->_m_vSubItem._InsertCol(nCol, sii);
				this->_m_vColumnWidthLowerLimit.insert(_m_vColumnWidthLowerLimit.begin() + nCol, _LV_DEFAULT_COLUMNWIDTH);
				this->_m_cCols++;
				__ASSERT(this->_m_cCols == this->_m_vSubItem._GetColSize());
			}
			if (this->_IsCol(nCol))
				this->_SetItemFormat(nCol, (LPLVCOLUMN)lParam);
		}
		else if (message == LVM_DELETECOLUMN) // LVM_DELETECOLUMN
		{
			int nCol = wParam;
			if (/*__FVERIFY*/(this->_IsCol(nCol)))
			{
				CWnd* pWnd;
				for (int i = 0; i < this->_m_cRows; i++)
				{
					pWnd = this->_m_vSubItem._GetAt(i, nCol).pWnd;
					if (pWnd != NULL)
					{
						pWnd->DestroyWindow();
						delete pWnd;
					}
				}
				this->_m_vSubItem._EraseCol(nCol);
				this->_m_vColumnWidthLowerLimit.erase(_m_vColumnWidthLowerLimit.begin() + nCol);
				this->_m_cCols--;
				__ASSERT(this->_m_cCols == this->_m_vSubItem._GetColSize());
			}
		}
		else if (message == LVM_INSERTITEM) // LVM_INSERTITEM
		{
			LVITEM* plvi = reinterpret_cast<LVITEM*>(lParam);
			int nRow = plvi->iItem;
			if (/*__FVERIFY*/(this->_IsRow(nRow) || nRow == this->_m_cRows))
			{
				if (!this->_IsRow(nRow)) nRow = this->_m_cRows;
				_SUBITEM_INFO sii;
				this->_m_vSubItem._InsertRow(nRow, sii);
				this->_m_cRows++;
				__ASSERT(this->_m_cRows == this->_m_vSubItem._GetRowSize());

				LVCOLUMN lvc;
				lvc.mask = LVCF_FMT;
				for (int i = 0; i < this->_m_cCols; i++)
				{
					__super::GetColumn(i, &lvc);
					this->_SetItemFormat(nRow, i, &lvc);
				}
			}
		}
		else if (message == LVM_DELETEITEM) // LVM_DELETEITEM
		{
			int nRow = wParam;
			if (/*__FVERIFY*/(this->_IsRow(nRow)))
			{
				CWnd* pWnd;
				for (int j = 0; j<this->_m_cCols; j++)
				{
					pWnd = this->_m_vSubItem._GetAt(nRow, j).pWnd;
					if (pWnd != NULL)
					{
						pWnd->DestroyWindow();
						delete pWnd;
					}
				}
				this->_m_vSubItem._EraseRow(nRow);
				this->_m_cRows--;
				__ASSERT(this->_m_cRows == this->_m_vSubItem._GetRowSize());
			}
		}
		else if (message == LVM_DELETEALLITEMS) // LVM_DELETEALLITEMS
		{
			CWnd* pWnd;
			for (int i = 0; i < this->_m_cRows; i++)
			{
				for (int j = 0; j<this->_m_cCols; j++)
				{
					pWnd = this->_m_vSubItem._GetAt(i, j).pWnd;
					if (pWnd != NULL)
					{
						pWnd->DestroyWindow();
						delete pWnd;
						pWnd = NULL;
					}
				}
			}
			// 아래 code에서 저장 공간의 모든 row만 삭제하고 column을 삭제하지 않는 이유 ?
			// 이 message 처리 이후, list control의 draw 상태를 확인해 보면 column이 삭제되지 않은 걸 볼 수 있다.
			// 따라서, list control을 완전히 비우려면 이 작업 이후, 추가적인 작업이 필요하다.
			// 그것은 모든 column에 대해서 LVM_DELETECOLUMN를 호출하는 작업이다.
			while (this->_m_vSubItem._GetRowSize() > 0) this->_m_vSubItem._EraseRow(0);
			this->_m_cRows = 0;
			__ASSERT(this->_m_cRows == this->_m_vSubItem._GetRowSize());
		}
		else if (message == LVM_SORTITEMS) // LVM_SORTITEMS runtime시에 제대로 작동하는지 확인 안함.
		{
			vector <LPARAM> lvilParam;
			for (int i = 0; i < this->_m_cRows; i++)
			{
				LVITEM lvi;
				lvi.mask = LVIF_PARAM;
				lvi.iItem = i;
				__super::GetItem(&lvi);
				lvilParam.push_back(lvi.lParam);
			}
			int n = 1;
			int nRange = this->_m_cRows - 2;
			while (1)
			{
				int cSwap = 0;
				for (int i = 0; i <= nRange; i++)
				{
					int iItem = (nRange - n*(nRange - i)) % (nRange + 1);
					if (((PFNLVCOMPARE)lParam)(lvilParam[iItem], lvilParam[iItem + 1], wParam)>0)
					{
						LPARAM lParamTemp = lvilParam[iItem];
						lvilParam[iItem] =lvilParam[iItem + 1];
						lvilParam[iItem + 1] =lParamTemp;
						this->_m_vSubItem[i].swap(this->_m_vSubItem[i + 1]);
						cSwap++;
					}
				}
				if (cSwap == 0) break;
				n *= -1;
			}
		}
		else if (message == LVM_SETTEXTBKCOLOR) // LVM_SETTEXTBKCOLOR
		{
			for (int i = 0; i < this->_m_cRows; i++)
				for (int j = 0; j<this->_m_cCols; j++)
					this->_m_vSubItem._GetAt(i, j).clrTextBk = (COLORREF)lParam;
		}
		else if (message == LVM_SETTEXTCOLOR) // LVM_SETTEXTCOLOR
		{
			for (int i = 0; i < this->_m_cRows; i++)
				for (int j = 0; j<this->_m_cCols; j++)
					this->_m_vSubItem._GetAt(i, j).clrText = (COLORREF)lParam;
		}
		else if (message == LVM_SETITEMTEXT) // LVM_SETITEMTEXT
		{
			LVITEM* plvi = reinterpret_cast<LVITEM*>(lParam);
			int nRow = wParam;
			int nCol = plvi->iSubItem;
			if (/*__FVERIFY*/(this->_IsSubItem(nRow, nCol)))
			{
				// gets previous info. of a subitem
				_SUBITEM_INFO sii = this->_m_vSubItem._GetAt(nRow, nCol);
				// delete previous control.
				if (sii.pWnd != NULL)
				{
					sii.pWnd->DestroyWindow();
					delete sii.pWnd;
					sii.pWnd = NULL;
				}
				this->_m_vSubItem._SetAt(nRow, nCol, sii);
			}
		}
		//lParam를 변경해도 LVSCW_AUTOSIZE인채로 수행됨. 그래서 함수 SetColumnWidth()로 대체함.
		//else if (message == LVM_SETCOLUMNWIDTH)
		//{
		//	int nCol = wParam;
		//	if (/*__FVERIFY*/(this->_IsCol(nCol)))
		//	{
		//		// cx가 LVSCW_AUTOSIZE일 때, 새로운 routine을 사용한다.
		//		if (LOWORD(lParam) == (WORD)LVSCW_AUTOSIZE)
		//		{
		//			int cx = LVSCW_AUTOSIZE;
		//			this->_AutoSize(nCol, &cx, NULL);	
		//			this->_SetColumnWidthLowerLimit(nCol, cx);
		//			lParam = MAKELPARAM(cx, 0);
		//		}
		//	}
		//}

		return __super::OnWndMsg(message, wParam, lParam, pResult);
	}

	/*********************************************************************************************************************************
	* subitem의 색상 설정
	*********************************************************************************************************************************/
public:
	BOOL _GetItemColor(int nRow, int nCol, LPCOLORREF lpclrTextBk, LPCOLORREF lpclrText)
	{
		if (/*__TVERIFY*/(!this->_IsSubItem(nRow, nCol))) return FALSE;
		// if NULL, 얻지 않는다.
		if (lpclrBk != NULL) *lpclrBk = this->_m_vSubItem._GetAt(nRow, nCol).clrBk;
		if (lpclrText != NULL) *lpclrText = this->_m_vSubItem._GetAt(nRow, nCol).clrText;
		return TRUE;
	}
	// *** _SetItemColor() : 0xffffffff(CLR_NONE), 0x00ffffff보다 크면(0x00000000)
	BOOL _SetItemColor(int nRow, int nCol,
		COLORREF clrTextBk = ::GetSysColor(COLOR_WINDOW),
		COLORREF clrText = ::GetSysColor(COLOR_WINDOWTEXT))
	{
		if (/*__TVERIFY*/(!this->_IsSubItem(nRow, nCol))) return FALSE;
		this->_m_vSubItem._GetAt(nRow, nCol).clrTextBk = clrTextBk;
		this->_m_vSubItem._GetAt(nRow, nCol).clrText = clrText;
		__super::RedrawItems(nRow, nRow);
		return TRUE;
	}

	/*********************************************************************************************************************************
	* subitem의 alignment 설정
	*********************************************************************************************************************************/
#define DT_HALIGN_JUSTIFYMASK	(DT_LEFT | DT_CENTER | DT_RIGHT)
#define DT_VALIGN_JUSTIFYMASK	(DT_TOP | DT_VCENTER | DT_BOTTOM)
public:
	UINT _GetItemFormat(int nRow, int nCol)
	{
		if (/*__TVERIFY*/(!this->_IsSubItem(nRow, nCol))) return -1;
		return this->_m_vSubItem._GetAt(nRow, nCol).uDTFormatFlags;
	}
	// *** __super::SetExtendedStyle() 과 동일한 방식으로 사용할 것.
	BOOL _SetItemFormat(int nRow, int nCol, UINT uNewDTFormatFlags)
	{
		if (/*__TVERIFY*/(!this->_IsSubItem(nRow, nCol))) return FALSE;
		this->_m_vSubItem._GetAt(nRow, nCol).uDTFormatFlags = uNewDTFormatFlags;
		__super::RedrawItems(nRow, nRow); 
		return TRUE;
	}
	BOOL _SetItemFormat(int nRow, UINT uNewDTFormatFlags)
	{
		if (/*__TVERIFY*/(!this->_IsRow(nRow))) return FALSE;
		for (int j = 0; j < this->_m_cCols; j++)
			this->_m_vSubItem._GetAt(nRow, j).uDTFormatFlags = uNewDTFormatFlags;
		__super::RedrawItems(nRow, nRow); 
		return TRUE;
	}
	BOOL _SetItemFormat(int nRow, int nCol, LPLVCOLUMN lpNewLVC)
	{
		if (/*__TVERIFY*/(!this->_IsSubItem(nRow, nCol))) return FALSE;
		if (!(lpNewLVC->mask & LVCF_FMT)) return FALSE; // column의 alignment를 변경하는 것이 아니면
		UINT uDTFormatFlags;
		switch (lpNewLVC->fmt & LVCFMT_JUSTIFYMASK) // sets new 
		{
		case LVCFMT_LEFT:
			uDTFormatFlags = DT_LEFT; 		
			break;
		case LVCFMT_CENTER:
			uDTFormatFlags = DT_CENTER; 		
			break;
		case LVCFMT_RIGHT:
			uDTFormatFlags = DT_RIGHT;
			break;
		}
		return this->_SetItemFormat(nRow, nCol, uDTFormatFlags | (this->_GetItemFormat(nRow, nCol) & ~(DT_LEFT | DT_CENTER | DT_RIGHT)));
	}
	BOOL _SetItemFormat(int nCol, LPLVCOLUMN lpNewLVC)
	{
		if (/*__TVERIFY*/(!this->_IsCol(nCol))) return FALSE;
		if (!(lpNewLVC->mask & LVCF_FMT)) return FALSE; // column의 alignment를 변경하는 것이 아니면
		UINT uDTFormatFlags;
		switch (lpNewLVC->fmt & LVCFMT_JUSTIFYMASK) // sets new 
		{
		case LVCFMT_LEFT:
			uDTFormatFlags = DT_LEFT; 		
			break;
		case LVCFMT_CENTER:
			uDTFormatFlags = DT_CENTER; 		
			break;
		case LVCFMT_RIGHT:
			uDTFormatFlags = DT_RIGHT;
			break;
		}
		for (int i = 0; i < this->_m_cRows; i++)
			this->_SetItemFormat(i, nCol, uDTFormatFlags | (this->_GetItemFormat(i, nCol) & ~(DT_LEFT | DT_CENTER | DT_RIGHT)));
		return TRUE;
	}

	/*********************************************************************************************************************************
	* subitem에 control 추가, 추가된 control 정보 얻기, 프로그램 종료시 추가된 모든 control들 제거,
	* control에서 발생한 message를 list control의 parent window로 전달 및
	* list control의 parent window로부터 받은 message들 중 control 관련 message 처리
	*********************************************************************************************************************************/
public:
	// *** _GetItemControl
	CWnd* _GetItemControl(int nRow, int nCol)
	{
		if (/*__TVERIFY*/(!this->_IsSubItem(nRow, nCol))) return NULL;
		return this->_m_vSubItem._GetAt(nRow, nCol).pWnd;
	}
	// *** _GetIndexControl
	BOOL _GetIndexControl(HWND hWnd, int *pnRow, int *pnCol)
	{
		CWnd* pWnd;
		for (int i = 0; i < this->_m_cRows; i++)
		{
			for (int j = 0; j<this->_m_cCols; j++)
			{
				pWnd = this->_m_vSubItem._GetAt(i, j).pWnd;
				if (pWnd != NULL && pWnd->GetSafeHwnd() == hWnd)
				{
					*pnRow = i;
					*pnCol = j;
					return TRUE;
				}
			}
		}
		*pnRow = -1;
		*pnCol = -1;
		return FALSE;
	}
private:
	// *** control를 정상적인 모양으로 그릴 수 있는 최소 높이를 구한다.
	int _GetControlMinHeight(CWnd *pWnd)
	{
		if (/*__TVERIFY*/(pWnd == NULL)) return 0;

		CDC *pDC = pWnd->GetDC();
		__ASSERT(pDC);
		CSize size = pDC->GetTextExtent(_T("한HAN"));
		pWnd->ReleaseDC(pDC);
		int nHeight = size.cy;
		WINDOWINFO wi;
		wi.cbSize = sizeof(WINDOWINFO);
		pWnd->GetWindowInfo(&wi);
		nHeight += wi.cyWindowBorders*2;
		return nHeight;
	}
public:
	// *** 이미 생성된 control을 추가시킨다.
	CWnd* _SetItemControl(int nRow,
		int nCol,
		CWnd* pWnd,
		int nWidth = CW_USEDEFAULT,	// CW_USEDEFAULT 의미
												// MSDN : CW_USEDEFAULT is valid only for overlapped windows;
												// if CW_USEDEFAULT is specified for a pop-up or child window,
												// the nWidth and nHeight parameter are set to zero.
												// 그래서 이 상수를 다른 의미로 사용하기로 한다.
												// _SetItemControl에서 사용하는 CW_USEDEFAULT는
												// subitem의 size를 control의 size로 사용하라는 의미이다.
												// 그런데, subitem의 height가 너무 작으면 문제가 발생할 수 있다.
												// 예를 들어 LISTBOX의 높이를 17이하로 MoveWindow하면 높이가 2로 변경된다.
												// control의 최소 height를 유지하는 방법을 찾아보자. 
		int nHeight = CW_USEDEFAULT,
		CFont* pFont = NULL)
	{
		if (/*__TVERIFY*/(!this->_IsSubItem(nRow, nCol))) return NULL;
		if (__TVERIFYM(!(__super::GetStyle() & LVS_REPORT), _T("Demands the LVS_REPORT style."))) return NULL;
		if (__TVERIFYM(pWnd == NULL, NULL)) return NULL;
		/* CLSID_WebBrowser처럼 WS_CHILD속성이 없는 window도 추가 가능하여
			아래 줄은 주석으로 나타낸다. 
		if (__TVERIFY(!(pWnd->GetStyle() & WS_CHILD))) return NULL;
		*/

		// deletes previous control and sets null text. SetItemText을 호출하면 OnWndMsg(...)에서 previous control을 제거할 수 있다.
		__super::SetItemText(nRow, nCol, _T(""));
		
		// sets the handle of window.
		this->_m_vSubItem._GetAt(nRow, nCol).pWnd = pWnd;

#ifndef _UNICODE
		// 대부분의 공용 control들은 project의 character set 설정에 맞춰 동일한 character set을 사용하지만
		// tree control이나 list control은 project의 character set설정에 상관없이
		// UNICODE character set을 사용하도록 초기화된다.
		// 그래서 notification code도 UNICODE용이 통지된다.
		::SendMessage(pWnd->GetSafeHwnd(), CCM_SETUNICODEFORMAT, FALSE/*the control will use ANSI characters.*/, 0);
#endif

		// sets same font as parent window
		// MoveWindow(...)가 함수 내부에서 control의 font를 참고하여 control의 height를 수정할 수도 있다.
		// 그래서 MoveWindow(...)수행되기 전에 control에서 사용할 font를 미리 변경해 주어야 한다.
		// 이런 작업이 일어나는 control들의 height는 nHeight로 미세하게 조정되지 않는다. 
		// 그리고 이 control들은 GetItemHeight(...)라는 member 함수를 가진다.
		if (pFont == NULL) pFont = __super::GetFont();
		pWnd->SetFont(pFont);

		// changes the dimensions.
		// control을 표시할 때마다 subitem좌표를 참고하여 position를 변경하기 때문에
		// 여기서는 position을 신경쓸 필요가 없고 size만 고려한다.
		CRect rcSubItemLabel(0, 0, 0, 0);
		CRect rcControl(0, 0, 0, 0);
		__super::GetSubItemRect(nRow, nCol, LVIR_LABEL, rcSubItemLabel);
		rcControl.CopyRect(&rcSubItemLabel);
		rcControl.bottom--; // this->_AlignObject(...) 참고
		rcControl.left++;
		rcControl.DeflateRect(_PADDING_X, _PADDING_Y);
		if (nWidth != CW_USEDEFAULT) rcControl.right = rcControl.left + nWidth;
		if (nHeight != CW_USEDEFAULT) rcControl.bottom = rcControl.top + nHeight;
		int nControlMinHeight = this->_GetControlMinHeight(pWnd);
		if (rcControl.Height() < nControlMinHeight) rcControl.bottom = rcControl.top + nControlMinHeight;
		pWnd->SetParent(this);
		// 상하 scroll이 발생할 때 추가된 control과 header control이 겹므로 header control이 항상 위에 있게 해야 한다.
		this->GetHeaderCtrl()->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		pWnd->ModifyStyle(0, WS_VISIBLE | WS_CLIPSIBLINGS);
		pWnd->MoveWindow(rcControl);
		// forces a list control to repaint a range of items.
		__super::RedrawItems(nRow, nRow);

		return pWnd;
	}
	// *** get the new window's ID through random number generator
			// UINT uID = 0x7fff - nRow * this->_m_cCols - nCol;
			// row번호, column번호를 사용하여 ID를 생성하는 윗 줄의 code는
			// row, column을 삭제/추가하면 row/column이 당겨지거나 밀리면서 동일한 id가 발생할 수 있어
			// 문제가 된다.
			// 그래서 이미 사용하는 id인지 아닌지 판단하여 할당하는 방식을 채택하자.
			// 생성할 id 구간은 0x7000 ~ 0x7fff로 정한다.
			// 구간을 순차적으로 1씩 증가/감소시켜 id 사용 여부를 확인하기 보단
			// 바로 새로운 id를 부여 받을 수 있는 random number generator를 활용해 보자.
	UINT _GetNewControlID()
	{
		#define _MAXNUM5CHILDWINDOWS_INTHECLISTCTRL 0x1000
		#define _MAXID5CHILDWINDOW_INTHECLISTCTRL 0x7fff
		#define _MINID5CHILDWINDOW_INTHECLISTCTRL 0x7000
		static unsigned long ulHoldRand = 1;
		if (ulHoldRand == 1) ulHoldRand = (unsigned long)::time(NULL);
		UINT uID;
		do
		{
			uID = _MAXID5CHILDWINDOW_INTHECLISTCTRL - ((unsigned short)
				((((ulHoldRand = ulHoldRand * 0x343fd + 0x269ec3)	>> 16) * _MAXNUM5CHILDWINDOWS_INTHECLISTCTRL) >> 16));
		} while (NULL != __super::GetDlgItem(uID));
		return uID;
	}

	 // *** enumControlType으로 control을 생성
			// _enumCONTROLTYPE 상수는 이 class보다 위에 외부에 정의 되어 있다.
	CWnd* _SetItemControl(int nRow,
		int nCol,
		_enumCONTROLTYPE enumControlType,
		DWORD dwStyle = 0,
		LPCTSTR lpszText = NULL,
		UINT uID = 0,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		CFont* pFont = NULL)
	{
		if (/*__TVERIFY*/(!this->_IsSubItem(nRow, nCol))) return NULL;
		if (__TVERIFYM(!(__super::GetStyle() & LVS_REPORT), _T("Demands the LVS_REPORT style."))) return NULL;

		// create new control.
		CWnd* pWnd;
		dwStyle |= WS_VISIBLE;
		CRect rcControl(0, 0, CW_USEDEFAULT, CW_USEDEFAULT);

		// id가 설정되어 있지 않으면 아직 사용하지 않은 id를 할당받는다.
		if (uID == 0) uID = this->_GetNewControlID();

		switch (enumControlType)
		{
		case _CT_CStatic:
			// SS_SIMPLE style은
			// 옮길 위치에 있는 그림을 배경으로 사용하기 때문에
			// 잔상으로 인식된다.
			dwStyle &= ~SS_SIMPLE;
			pWnd = new CStatic();
			((CStatic*)pWnd)->Create(_T(""), dwStyle, rcControl, this, uID);
			break;
		case _CT_CButton: // 클릭되어서 default button이 됐을 때, 이를 나타내는 rectangle이 button둘레에 그려진다.이 rectangel이 scroll할 때 잔상으로 남게 된다.
						  // 생성시에는 transparent 속성이 없음에도 불구하고
						  // redraw할 때 transparent 된다.
						  // 따라서, text와 겹칠 때 잔상이 남는다.
			pWnd = new CButton();
			((CButton*)pWnd)->Create(_T(""), dwStyle, rcControl, this, uID);
			break;
		case _CT_CEdit:
			pWnd = new CEdit();
			((CEdit*)pWnd)->Create(dwStyle, rcControl, this, uID);
			break;
		case _CT_CComboBox:
			pWnd = new CComboBox();
			((CComboBox*)pWnd)->Create(dwStyle, rcControl, this, uID);
			// 멀티바이트 문자셋일 때는 listbox의 크기가 사용자가 정해준 크기로만 생성된다.
			// 그런데 이 함수에서 combobox의 크기가 subitem의 rect 크기로 조정되기 때문에
			// 충분한 크기의 listbox를 확보하지 못하여 추가된 list항목이 보이지 않을 수도 있다.
			// 그래서 멀티바이트 문자셋일 때는 아래의 코드를 사용한다.
			#ifndef _UNICODE
			if ((dwStyle & CBS_DROPDOWNLIST) && nHeight == CW_USEDEFAULT) nHeight = ((CComboBox*)pWnd)->GetItemHeight(0) * 32;
			#endif
			break;
		case _CT_CListBox:
			// listbox의 height는 font의 높이를 한줄로 삼아
			// 그 만큼씩 증가한다.
			pWnd = new CListBox();
			((CListBox*)pWnd)->Create(dwStyle, rcControl, this, uID);
			break;

			//case _CT_CAnimateCtrl:
			//case _CT_CDateTimeCtrl:
			//case _CT_CComboBoxEx:
			//case _CT_CHeaderCtrl:
			//case _CT_CHotKeyCtrl:
			//case _CT_CImageList:

		case _CT_CListCtrl:
			pWnd = new CListCtrl();
			((CListCtrl*)pWnd)->Create(dwStyle, rcControl, this, uID);
			break;

			//case _CT_CMonthCalCtrl:
			//case _CT_CProgressCtrl:
			//case _CT_CRebarCtrl:
			//case _CT_CRichEditCtrl:
			//case _CT_CScrollBar:
			//case _CT_CSliderCtrl:
			//case _CT_CSpinButtonCtrl:
			//case _CT_CStatusBarCtrl:
			//case _CT_CTabCtrl:
			//case _CT_CToolBarCtrl:
			//case _CT_CToolTipCtrl:

		case _CT_CTreeCtrl:
			pWnd = new CTreeCtrl();
			((CTreeCtrl*)pWnd)->Create(dwStyle, rcControl, this, uID);
			break;
		}
		pWnd->SetWindowText(lpszText);

		if (NULL == this->_SetItemControl(nRow, nCol, pWnd, nWidth, nHeight, pFont))
		{
			if (pWnd != NULL)
			{
				pWnd->DestroyWindow();
				delete pWnd;
				pWnd = NULL;
			}
		}
		return pWnd;
	}
protected:
	// *** 종료시 추가된 모든 control 삭제
	afx_msg void OnDestroy()
	{
		__super::OnDestroy();

		// TODO: 여기에 메시지 처리기 코드를 추가합니다.
		// delete controls created as a subitem.
		_SUBITEM_INFO sii;
		for (int i = 0; i < this->_m_cRows; i++)
		{
			for (int j = 0; j < this->_m_cCols; j++)
			{
				sii = this->_m_vSubItem._GetAt(i, j);
				if (sii.pWnd != NULL) // is control
				{
					sii.pWnd->DestroyWindow();
					delete sii.pWnd;
					sii.pWnd = NULL;
					this->_m_vSubItem._SetAt(i, j, sii);
				}
			}
		}
	}
	// *** control에서 받은 message를 list control의 parent로 보내거나
			// list control의 parent에서 받은 message들 중 control에 대해 처리해야 할 것들을 적용한다.
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
	//#define _CListCtrl_OnWndMsg_TRACE
	#if defined(_DEBUG) && defined(_CListCtrl_OnWndMsg_TRACE)
		static unsigned int n = 0;
		CString strType = _T("");
		strType.GetBufferSetLength(128);
		if (message == WM_COMMAND)
		{
			if (lParam == 0) strType = _T("Menu");
			else if (lParam == 1) strType = _T("Accelerator");
			else  ::RealGetWindowClass((HWND)lParam, (LPTSTR)strType.GetString(), strType.GetAllocLength()); // Control(Handle to the control window)
			TRACE(_T("\n%u : _CListCtrl::OnWndMsg(WM_COMMAND, %p, %p, pResult) : %s(%p, %p, %p) =>\n"), n++, wParam, lParam, strType.GetString(), lParam, LOWORD(wParam), HIWORD(wParam));
		}
		else if (message == WM_NOTIFY)
		{
			LPNMHDR pNMHDR = reinterpret_cast<LPNMHDR>(lParam);
			// wParam와 pNMHDR->idFrom은 일치한다.
			// __super::GetDlgItem(wParam)->GetSafeHwnd()와 pNMHDR->hwndFrom는 일치한다.
			::RealGetWindowClass(pNMHDR->hwndFrom, (LPTSTR)strType.GetString(), strType.GetAllocLength());
			TRACE(_T("\n%u : _CListCtrl::OnWndMsg(WM_NOTIFY, %p, %p, pResult) : %s(%p, %p, %p) =>\n"), n++, wParam, lParam, strType.GetString(), pNMHDR->hwndFrom, pNMHDR->idFrom, pNMHDR->code);
		}
	#endif
		// OnWndMsg does most of the work, except for DefWindowProc call
		LRESULT lResult = 0;
		if (!OnWndMsg(message, wParam, lParam, &lResult))
		{
	#if defined(_DEBUG) && defined(_CListCtrl_OnWndMsg_TRACE)
			if (message == WM_COMMAND || message == WM_NOTIFY) // 이때 위의 OnWndMsg의 return값은 항상 FALSE이다.
				TRACE(_T("=> _CListCtrl::OnWndMsg(%p, %p, %p, pResult)\n"), message, wParam, lParam);
	#endif
			// list control의 기본 child control은 column header 뿐이기 때문에
			// child control 추가 작업을 하지 않으면 header에 대한 WM_NOTIFY만 발생한다.
			// 따라서 child control이 추가된 후 발생하는 모든 WM_COMMAND와 WM_NOTIFY는
			// 대부분 child control로부터 온 것이다. 이를 list control의 parent window로 보낸다.
			if (wParam != 0 /*colume header가 아니고*/ && (message == WM_COMMAND || message == WM_NOTIFY))
				::SendMessage(__super::GetParent()->GetSafeHwnd(), message, wParam, lParam);
			// list control의 parent window로부터 온 message들 중에서 control에 대해서 처리해야 할 것이 있으면...
			// WM_ENABLE message는 변화가 있을 때만 발생할 뿐 실질적으로 변화를 주는 message는 아니다.
			// 그래서 child control에 WM_ENABLE message를 SendMessage해도 아무런 변화를 주지 못한다.
			else if (message == WM_ENABLE)
			{
				CWnd* pWnd;
				for (int i = 0; i < this->_m_cRows; i++)
				{
					for (int j = 0; j<this->_m_cCols; j++)
					{
						pWnd = this->_m_vSubItem._GetAt(i, j).pWnd;
						if (pWnd != NULL) pWnd->EnableWindow(wParam);
					}
				}
			}
			// WM_ENABLE 처럼 처리해야 할 message가 더 있으면...
			//else if (message == )
			//{
			//}

			// column header의 WM_NOTIFY는 DefWindowProc내에서 parent로 보내진다.
			lResult = __super::DefWindowProc(message, wParam, lParam);
		}
		return lResult;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// contents info 다루기 - 끝
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// row의 높이와 column의 폭 다루기 - 시작
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*********************************************************************************************************************************
	* 주어진 column에 속한 subitem들의 최적 크기를 조사하여 그 중 가장 큰 크기로 일괄 조정한다.
	*********************************************************************************************************************************/
private:
	#define _RCOBJECT_INFLATE_SHIFT\
				/* this->_AlignObject(...) 참고*/\
				rcObject.InflateRect(_PADDING_X, _PADDING_Y);\
				rcObject.bottom++;\
				rcObject.left--;
	void _AutoSize(int nCol, int *pnNewColumnWidth, int *pnNewRowHeight)
	{
		if (pnNewColumnWidth != NULL && *pnNewColumnWidth < _LV_DEFAULT_COLUMNWIDTH) *pnNewColumnWidth = _LV_DEFAULT_COLUMNWIDTH;
		if (pnNewRowHeight  != NULL && *pnNewRowHeight < _LV_DEFAULT_ROWHEIGHT) *pnNewRowHeight = _LV_DEFAULT_ROWHEIGHT;
		CDC *pDC = __super::GetDC();
		CFont *pFont = __super::GetFont();
		CFont *pOldFont = pDC->SelectObject(pFont); // DrawItem()에서 사용하는 dc의 font는 list control의 font와 일치하는제
																// __super::GetDC()로 얻은 dc의 font는 list control으 font와 일치하지 않는다.
																// 그래서 DrawText()를 수행하기 전에 font를 일치시켜 준다.
		register int i;
		for (i = 0; i < this->_m_cRows; i++)
		{
			CRect rcObject(0, 0, 0, 0);
			_SUBITEM_INFO sii = this->_m_vSubItem._GetAt(i, nCol);
			if (sii.pWnd == NULL)
			{
				CString strT = __super::GetItemText(i, nCol);
				UINT uDTFormatFlags = sii.uDTFormatFlags & ~(DT_END_ELLIPSIS | DT_PATH_ELLIPSIS | DT_WORD_ELLIPSIS);
				if (!(__super::GetStyle() & LVS_OWNERDRAWFIXED)) uDTFormatFlags |= DT_SINGLELINE;
				pDC->DrawText(strT, rcObject, DT_CALCRECT | uDTFormatFlags);
				if (uDTFormatFlags & DT_SINGLELINE) rcObject.right += (_PADDING_X_DT_SINGLELINE - _PADDING_X) * 2 - 1;
										// 여기서 _PADDING_X_DT_SINGLELINE는
										// __super::SetColumnWidth(.., LVSCW_AUTOSIZE)와 DrawText(..., DT_CALCRECT | DT_SINGLELINE) 차이로서,
										// font 종류, font 크기, 그리고 Visual Studio의 version을 변경해도 항상 일정한 값을 유지하였다.
			}
			else
			{
				sii.pWnd->GetWindowRect(&rcObject);
			}
			_RCOBJECT_INFLATE_SHIFT;
			CRect rcSubItemLabel(0, 0, 0, 0);
			CRect rcSubItemBounds(0, 0, 0, 0);
			__super::GetSubItemRect(i, nCol, LVIR_LABEL, rcSubItemLabel);
			__super::GetSubItemRect(i, nCol, LVIR_BOUNDS, rcSubItemBounds);
			rcObject.left -= (rcSubItemLabel.left - rcSubItemBounds.left);
			if (pnNewColumnWidth != NULL && rcObject.Width() > *pnNewColumnWidth) *pnNewColumnWidth = rcObject.Width();
			if (pnNewRowHeight != NULL && rcObject.Height() > *pnNewRowHeight) *pnNewRowHeight = rcObject.Height();
		} 
		pDC->SelectObject(pOldFont);
		__super::ReleaseDC(pDC);
	}

	/*********************************************************************************************************************************
	* gets/sets the height of rows
	*********************************************************************************************************************************/
protected:
	// *** MeasureItem()
	bool _m_bGetSetRowHeightFlag;
	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
	{
		if (this->_m_bGetSetRowHeightFlag)
			lpMIS->itemHeight = this->_m_nRowHeight; // sets
		else
			this->_m_nRowHeight = lpMIS->itemHeight; // gets
	}
public:
	// *** _GetRowHeight()
	int _GetRowHeight(/*int nRow*/)
	{
		this->_m_bGetSetRowHeightFlag = false; // gets
		WINDOWPOS wp;
		wp.flags = SWP_NOACTIVATE
			| SWP_NOMOVE
			| SWP_NOOWNERZORDER
			| SWP_NOZORDER;
		// MeasureItem은 Owner Draw에서 호출된다.
		DWORD dwStyle = __super::GetStyle();
		if (!(dwStyle & LVS_OWNERDRAWFIXED)) __super::ModifyStyle(0, LVS_OWNERDRAWFIXED, 0);
		__super::SendMessage(WM_WINDOWPOSCHANGED, 0, reinterpret_cast<LPARAM> (&wp));
		if (!(dwStyle & LVS_OWNERDRAWFIXED)) __super::ModifyStyle(LVS_OWNERDRAWFIXED, 0, 0);
		return this->_m_nRowHeight;
	}
	#define	_LVSRH_AUTOSIZE	LVSCW_AUTOSIZE
	#define	_LVSRH_AUTOSIZE_USEHEADER	LVSCW_AUTOSIZE_USEHEADER
	// *** _SetRowHeight()
	BOOL _SetRowHeight(int cy)
	{
		if (cy == _LVSRH_AUTOSIZE)
		{
			for (int j = 0; j<this->_m_cCols; j++)
				this->_AutoSize(j, NULL, &cy);
		}

		if (cy < 0) cy = _LV_DEFAULT_ROWHEIGHT;

		// list control의 영역을 벗어난 높이로 설정하면
		// scroll 할 때 잔상이 남는다.
		CRect rcArea(0, 0, 0, 0);
		__super::GetClientRect(&rcArea);
		CRect rcWnd(0, 0, 0, 0);
		__super::GetWindowRect(&rcWnd);
		rcArea.bottom -= (rcWnd.Height() - rcArea.Height() - 2) / 2;
		CHeaderCtrl* pHeaderCtrl = CListCtrl::GetHeaderCtrl();
		if (pHeaderCtrl != NULL)
		{
			CRect rcHeader(0, 0, 0, 0);
			pHeaderCtrl->GetClientRect(&rcHeader);
			rcArea.top = rcHeader.Height();
		}
		if (/*__TVERIFY*/(cy > rcArea.Height())) return FALSE;

		__super::EnsureVisible(0, FALSE);

		// MeasureItem 호출할 때 사용할 값을 미리 sets
		this->_m_bGetSetRowHeightFlag = true; // sets
		this->_m_nRowHeight = cy;
		
		DWORD dwStyle = __super::GetStyle();
		// MeasureItem은 LVS_OWNERDRAWFIXED에서 호출된다.
		if (!(dwStyle & LVS_OWNERDRAWFIXED)) __super::ModifyStyle(0, LVS_OWNERDRAWFIXED, 0);
		// MeasureItem 호출
		WINDOWPOS wp;
		wp.flags = SWP_NOACTIVATE
			| SWP_NOMOVE
			| SWP_NOOWNERZORDER
			| SWP_NOZORDER;
		__super::SendMessage(WM_WINDOWPOSCHANGED, 0, reinterpret_cast<LPARAM> (&wp));
		//vs2005 : 두번째 호출에서 scrollbar의 이동량이 제대로 설정되고 scrollbar내 slider 길이도 변한다.
		__super::SendMessage(WM_WINDOWPOSCHANGED, 0, reinterpret_cast<LPARAM> (&wp));
		// LVS_OWNERDRAWFIXED가 없었으면 제거해 준다.
		if (!(dwStyle & LVS_OWNERDRAWFIXED)) __super::ModifyStyle(LVS_OWNERDRAWFIXED, 0, 0);
		
		return TRUE;
	}

	/*********************************************************************************************************************************
	* Sets the width of a column in a report-mode control.
	*********************************************************************************************************************************/
public:
	// *** OnWndMsg()에서 LVM_SETCOLUMNWIDTH message일 때
	// cx를 변경하여 lParam = MAKELPARAM(cx, 0);를 해 보았으나 그대로 LVSCW_AUTOSIZE로 처리됨.
	BOOL SetColumnWidth(int nCol, int cx)
	{
		if (!this->_IsCol(nCol)) return FALSE;
		
		// cx가 LVSCW_AUTOSIZE일 때, 새로운 routine을 사용한다.
		if (cx == LVSCW_AUTOSIZE)
		{
			this->_AutoSize(nCol, &cx, NULL);	
			this->_SetColumnWidthLowerLimit(nCol, cx);
		}
		return __super::SetColumnWidth(nCol, cx);
	}

	/*********************************************************************************************************************************
	* gets/set the lower limit of column's width
	*********************************************************************************************************************************/
public:
	// *** _GetColumnWidthLowerLimit
	int _GetColumnWidthLowerLimit(int nCol)
	{
		if (/*__TVERIFY*/(!this->_IsCol(nCol))) return -1;
		return this->_m_vColumnWidthLowerLimit[nCol];
	}
	// *** _SetColumnWidthLowerLimit
	BOOL _SetColumnWidthLowerLimit(int nCol, int cxLL)
	{
		if (/*__TVERIFY*/(!this->_IsCol(nCol))) return FALSE;
		if (cxLL < 0) return FALSE;
		this->_m_vColumnWidthLowerLimit[nCol] = cxLL;
		return TRUE;
	}
protected:
	// *** column의 너비를 제어한다.
		// !!! OnNotify는 child control로부터 오는 통지를 처리한다.
		// 이 통지를 message map에서 ON_NOTIFY를 통해 처리하면
		// parent control이 invalidate되어 깜박거림이 심하다.
		// OnNotify를 재정의하여 처리하면 이런 현상은 발생하지 않는다.
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	{
		// TODO: 여기에 특수화된 코드를 추가 및 또는 기본 클래스를 호출합니다.
	//#define _CListCtrl_OnNotify_TRACE
	#if defined(_DEBUG) && defined(_CListCtrl_OnNotify_TRACE)
		static unsigned int n = 0;
		CString strType;
		strType.GetBufferSetLength(128);
		LPNMHDR pNMHDR = reinterpret_cast<LPNMHDR>(lParam);
		// wParam와 pNMHDR->idFrom은 일치한다.
		// __super::GetDlgItem(wParam)->GetSafeHwnd()와 pNMHDR->hwndFrom는 일치한다.
		::RealGetWindowClass(pNMHDR->hwndFrom, (LPTSTR)strType.GetString(), strType.GetAllocLength());
		TRACE(_T("%u : _CListCtrl::OnNotify(%p, %p, pResult) : %s(%p, %p, %p) =>\n"), n++, wParam, lParam, strType.GetString(), pNMHDR->hwndFrom, pNMHDR->idFrom, pNMHDR->code);
	#endif
		if (wParam == 0) // 추가된 child control을 제외시킨다. header control의 id는 0이다.
		{

			// multibyte character set일때도 이 HDN_ITEMCHANGINGW이 통지된다.
			LPNMHEADER pNMHEADER = reinterpret_cast<LPNMHEADER>(lParam);
			if ((pNMHEADER->hdr.code == HDN_DIVIDERDBLCLICKW
				|| pNMHEADER->hdr.code == HDN_DIVIDERDBLCLICKA))
			{
				// header control의 divider를 double click하면 아래의 통지를 순서대로 받는다.
				// HDN_BEGINTRACKW, HDN_BEGINTRACKA,
				// HDN_ENDTRACKW, HDN_ENDTRACKA,
				// HDN_ITEMCHANGINGW, HDN_ITEMCHANGINGA
				// HDN_ITEMCHANGEDW, HDN_ITEMCHANGEDA
				// HDN_DIVIDERDBLCLICKW, HDN_DIVIDERDBLCLICKA
				// HDN_ITEMCHANGINGW, HDN_ITEMCHANGINGA
				// HDN_ITEMCHANGEDW, HDN_ITEMCHANGEDA
				this->SetColumnWidth(pNMHEADER->iItem, LVSCW_AUTOSIZE);
				*pResult = TRUE;
				return TRUE;
			}
			else if ((pNMHEADER->hdr.code == HDN_ITEMCHANGINGW	// multibyte character set일때도 이 HDN_ITEMCHANGINGW이 통지된다.
				|| pNMHEADER->hdr.code == HDN_ITEMCHANGINGA))		// 이유는 this->_SetItemControl(...)에서 CCM_SETUNICODEFORMAT 참고
			{
				// SetColumnWidth(LVM_SETCOLUMNWIDTH)가 호출되거나
				// header control의 column 폭을 mouse로 조정할 때
				// header control로부터 아래의 통지를 순서대로 받는다.
				// HDN_ITEMCHANGINGW, HDN_ITEMCHANGINGA
				// HDN_ITEMCHANGEDW, HDN_ITEMCHANGEDA
				int nCol = pNMHEADER->iItem;
				__ASSERT(nCol >= 0 && nCol < this->_m_cCols);
				if (pNMHEADER->pitem->cxy < this->_m_vColumnWidthLowerLimit[nCol])
				{
					// 1) mouse로 column의 너비를 조정할 때
					//    조정하려는 값이 주어진 최소 너비보다 작으면
					//    더 이상 조정을 할 수 없도록 해준다.
					// 2) 이 최소 너비를 응용하면 column을 고정시킬 수 있다.
					//    현재의 너비보다 최소 너비를 더 크게 설정해주면
					//    mouse로 조정할 수 없게 된다.
					//    하지만, mouse를 고속으로 움직이면 무용지물이 된다.
					// 3) Vista이후부터 LVCFMT_FIXED_WIDTH가 추가되어 2)와 같은 기능을 제공한다.

					*pResult = TRUE;
					return TRUE;
				}
			}
		}

		return __super::OnNotify(wParam, lParam, pResult);
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// row의 높이와 column의 폭 다루기 - 끝
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Draw - 시작
	//
	// Owner Draw, Custom Draw and Default Draw 비교
	//
	// Using Owner Draw :  
	//		1. Owner Draw는 LVS_REPORT style에서 대해서만 그릴 수 있다.
	//		2. list control이 LVS_REPORT | LVS_OWNERDRAWFIXED style 이어야 하고,
	//			_CListCtrl::DrawItem을 override 해야 한다.
	//		3. parent window에서 ModifyStyle(LVS_OWNERDRAWFIXED, 0, 0); 또는 ModifyStyle(LVS_REPORT, 0, 0);를 사용하면
	//			DrawItem을 사용하지 않게 되어, 원래의 그리기 작업으로 되돌릴 수 있다. 
	// Using Custom Draw :
	//		1. Owner Draw는 LVS_REPORT style에서 대해서만 그릴 수 있지만
	//			Custom Draw는 LVS_ICON, LVS_SMALLICON, LVS_LIST style에 대해서도 그릴 수 있다.
	//		2. ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &_CListCtrl::OnNMCustomdraw) 을 작성하는 것만으로
	//			Custom Draw를 할 수 있다.
	//			CDDS_PREPAINT(*pResult = CDRF_NOTIFYITEMDRAW;)
	//			=> CDDS_ITEMPREPAINT(*pResult = CDRF_NOTIFYSUBITEMDRAW;)
	//			=> (CDDS_SUBITEM|CDDS_ITEMPREPAINT)(*pResult = 0;) 순서로 처리된다.
	//		3. (CDDS_SUBITEM|CDDS_ITEMPREPAINT) 는
	//			Owner Draw의 DrawItem처럼 list control이 LVS_REPORT style일 때만 처리되는데
	//			DrawItem이 호출되는 상황(LVS_OWNERDRAWFIXED style)일 때는 처리되지 않는다.
	//			즉, (CDDS_SUBITEM|CDDS_ITEMPREPAINT)가 처리되려면 list control이
	//			LVS_REPORT style이어야 하고, LVS_OWNERDRAWFIXED style은 아니어야 한다.
	//		4. OnNMCustomdraw 호출될 때마다 NMHDR *pNMHDR의 meber들 중에서 color 값들은 초기화되지 않고 유지된다. 
	// Ownder Draw와 Custom Draw간에 호출 횟수 비교 :`
	//		1. Custom Draw(OnNMCustomdraw)가 Owner Draw(DrawItem)보다 먼저 호출된다.
	//		2. 두 호출 모두 list control에서 보이는 item에 대해서만 호출된다.
	//		3. 만약 보이는 item의 개수가 동일할 때 두 호출의 횟수를 비교하면 당연히 Custom Draw쪽이 훨씬 많다.
	//			그 이유는 Owner Draw는 item 단위로 호출되고 Custom Draw는 subitem 단위로 호출되기 때문이다. 
	//			그리고 CDDS_PREPAINT가 두번 먼저 호출된다.
	//			만약 CDDS_ITEMPREPAINT에서 *pResult = CDRF_NOTIFYSUBITEMDRAW; 를 return 하지 않고
	//			DrawItem처럼 모든 subitem에 대한 처리를 하면
	//			호출 횟수는 CDDS_PREPAINT 호출 횟수 만큼만 더 많아질 뿐이다. 
	// LVS_REPORT | LVS_OWNERDRAWFIXED style이 설정되어 있지 않으면 Owner Draw를 하지 않듯이
	// Custom Draw도 특정 style이 설정되어 있으면 작동하지 않도록 하면 일관성이 있어 보인다.
	// 아래와 같이 새로운 style인 _LVS_EX_CUSTOMDRAWFIXED을 define해 보았다.
	// 이로써 LVS_REPORT | _LVS_EX_CUSTOMDRAWFIXED style이 설정되어 있지 않으면 Custom Draw를 하지 않도록 할 수 있다.
	// list view 기본 style은 4바이트 모두 define되어 있지만
	// extended style에서는 아직까지 0x0x04000000, 0x20000000은 define되어 있지 않다.
	#define _LVS_EX_CUSTOMDRAWFIXED 0x20000000
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*********************************************************************************************************************************
	* init style
	*********************************************************************************************************************************/
protected:
	// *** PreCreateWindow
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs) // 동적 생성시 호출됨
	{
		// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
		// 정적 생성될 경우 PreCreateWindow가 호출되지 않기 때문에
		// PreSubclassWindow()를 통해 style 초기화를 해야하고,
		// 호출 쪽에서는 SubclassDlgItem()을 통해 이 클래스와 resource를 연결해주어야 한다.
		// 그런데, Create를 통해 동적 생성할 경우,
		// PreCreateWindow()와 PreSubclassWindow() 둘 다 호출된다.
		// 하지만, PreSubclassWindow()에 앞서 PreCreateWindow()가 먼저 수행되고
		// PreCreateWindow()에서 설정한 style이 우선시 된다.
		// 즉, 이 후 PreSubclassWindow()에서 변경된 style은 반영되지 않는다.

		// Control의 잔상이 생기지 않게 하려면
		cs.style |= WS_CLIPCHILDREN;

		return __super::PreCreateWindow(cs);
	}
	// *** PreSubclassWindow
	virtual void PreSubclassWindow() // 정적 생성시 호출됨
	{
		// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

		// Control의 잔상이 생기지 않게 하려면
		__super::ModifyStyle(0, WS_CLIPCHILDREN, 0);

		__super::PreSubclassWindow();
	}

	/*********************************************************************************************************************************
	* control view state
	*********************************************************************************************************************************/
private:
	bool _m_bAllControlsViewState;
public:
	bool _GetAllControlsViewState()
	{
		// true이면, 추가된 모든 control을 보여 주기
		// false이면, 선택된 row에 있는 control만 보여 주기
		return this->_m_bAllControlsViewState;
	}
	void _SetAllControlsViewState(bool bMode = true)
	{
		this->_m_bAllControlsViewState = bMode;
		__super::RedrawWindow();
		//__super::RedrawItems(0, this->_m_cRows-1); 
	}

	/*********************************************************************************************************************************
	* Owner Draw - checkbox
	*********************************************************************************************************************************/
public:
	// *** Sets the checked state of a particular item. Only useful on controls with the LVS_EX_CHECKBOXES style.
   BOOL SetCheck(int nItem, BOOL fCheck = TRUE)
   {
	   if (fCheck == -1) // checkbox를 제거하라.
	   {
			LVITEM lvi;
			lvi.mask = LVIF_STATE; // gets the state of a item
			lvi.iItem = nItem;
			lvi.iSubItem = 0;
			lvi.stateMask = LVIS_STATEIMAGEMASK; // The item's state image index is retrieved by a mask.
			lvi.state = 0;
			__super::SetItem(&lvi);
			return -1;
	   }
	   return __super::SetCheck(nItem, fCheck);
   }
protected:
	// *** LVS_OWNERDRAWFIXED 일 때 기존 __super::OnLButtonDown(nFlags, point);으로는 checkbox를 toggling 하지 않는다.
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point)
	{
		// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
		// check if hit was on a checkbox and then if yes, toggle a checkbox.
		if (((__super::GetStyle() & (LVS_OWNERDRAWFIXED | LVS_REPORT)) == (LVS_OWNERDRAWFIXED | LVS_REPORT))
			&& (__super::GetExtendedStyle() & LVS_EX_CHECKBOXES))
		{
			int nHitItem = __super::HitTest(point);
			if (nHitItem != -1)
			{
				BOOL bCheck = __super::GetCheck(nHitItem);
				if (bCheck != -1) // checkbox 를 그릴 때(bCheck == 0 or 1)
				{
					CRect rc(0, 0, 0, 0);
					__super::GetItemRect(nHitItem, &rc, LVIR_BOUNDS);
					rc.left += 4;
					rc.right = rc.left + 16;
					rc.top = (rc.bottom + rc.top) / 2 - 8;
					rc.bottom = rc.top + 16;
					if (rc.PtInRect(point)) __super::SetCheck(nHitItem, !bCheck);
				}
			}
		}

		__super::OnLButtonDown(nFlags, point);
	}
	// *** OnNMClick은 OnLButtonDown과 동일하다. 둘 중 하나를 선택하여 사용한다.
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
	{
		LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
		TRACE(_T("_CListCtrl::OnNMClick : %p, %p, %p\n"), pNMIA->uOldState, pNMIA->uNewState, pNMIA->uChanged); // 변화 없음
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		// check if hit was on a checkbox and then if yes, toggle a checkbox.
		if (((__super::GetStyle() & (LVS_OWNERDRAWFIXED | LVS_REPORT)) == (LVS_OWNERDRAWFIXED | LVS_REPORT))
			&& (__super::GetExtendedStyle() & LVS_EX_CHECKBOXES))
		{
			int nHitItem = pNMIA->iItem;
			if (nHitItem != -1)
			{
				BOOL bCheck = __super::GetCheck(nHitItem);
				if (bCheck != -1) // checkbox 를 그릴 때(bCheck == 0 or 1)
				{
					CRect rc(0, 0, 0, 0);
					__super::GetItemRect(nHitItem, &rc, LVIR_BOUNDS);
					rc.left += 4;
					rc.right = rc.left + 16;
					rc.top = (rc.bottom + rc.top) / 2 - 8;
					rc.bottom = rc.top + 16;
					if (rc.PtInRect(pNMIA->ptAction)) __super::SetCheck(nHitItem, !bCheck);
				}
			}
		}
		*pResult = 0;
	}
	// *** state 변화를 확인하기 위한 용도일 뿐 필요없는 member 함수
			// 1. OnLButtonDown 또는 OnNMClick에서 SetCheck를 수행하면
			//		-> OnNMItemChanged(0x00001000 => 0x00002000(checked) or 0x00002000 => 0x00001000(unchecked)) 이 발생한다.
			//		SetCheck 수행 여부에 상관없이 아래 두 줄은 발생한다.
			//		-> OnNMItemChanged(0x00000002(LVIS_SELECTED) => 0x00000000)
			//		-> OnNMItemChanged(0x00000001(LVIS_FOCUSED) => 0x00000003)
			// 2. 그래서 list control을 사용하는 parent window에서 checkbox를 확인하려면
			//		parent window에서 OnNMItemChanged(LVN_ITEMCHANGED)를 작성하면 된다.
	afx_msg void OnNMItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
	{ 
		LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
		TRACE(_T("_CListCtrl::OnNMItemChanged : %p, %p, %p\n"), pNMLV->uOldState, pNMLV->uNewState, pNMLV->uChanged/*LVIF_STATE 0x00000008*/);
		*pResult = 0;
	}

	/*********************************************************************************************************************************
	* Owner Draw - DrawItem
	*********************************************************************************************************************************/
protected:
	// *** DrawItem()
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS)
	{
		// TODO:  지정된 항목을 그리는 코드를 추가합니다.

		// 1. DrawItem 호출 같은 list control의 갱신이 발생하는 경우는 ...
		//		수직 scroll과 수평 scroll이 발생했을 때와
		//		child control이 있고 cursor가 child control 위에 올려지면서 child control 갱신될 때, 또 다른 방법으로 갱신될 때일 것이다.
		//
		// 2. scroll에 대한 이미지 이동 방식과 그에 따른 CPU 점유율 상승
		//		list control은 scroll에 대해 매번 item을 다시 그리는 것이 아니라
		//		scroll 양 만큼 이미지를 이동시키고 이동 후 비워지는 곳만 그리는 방식을 사용한다.
		//		이런 방식을 취해도 scroll을 고속으로 과다하게 조정하면 그리기 작업이 빈번하게 발생하여 CPU 점유율이 급격히 상승한다.
		//		CPU 점유율이 동영상 play의 그것보다 더 높게 올라간다.
		//		수평 scroll이 수직 scroll보다 더 높다.
		//		모든 응용프로그램에서 공통적으로 나타나는 현상이다.
		//
		// 3. child control 표시
		//		1) 일단 Owner Draw로 그려지는 화면이
		//			Custom Draw, Default Draw, 그리고 report view이외의 view일때의	화면과 다를 수 있기 때문에
		//			LVS_OWNERDRAWFIXED style을 설정할 때 Owner Draw를 해주어야 한다. and vice versa.
		//			그래서 ModifyStyle 등 style 수정 함수 다음에 RedrawWindow()가 수행되어야 한다.
		//			자세한 내용은 this->ModifyStyle(...) 의 설명 참고
		//
		//		2) control을 추가하는 이 class를 만들면서 항상 고려해야 할 것은 상하 방향 scroll에 의해 발생하는 문제들이다.
		//			checkbox, icon, 배경, text, 그리고 focus rect. 들을
		//			그냥 주어진 item에서만 draw를 하는데에 아무런 문제가 발생하지 않는다.
		//			control도 이들처럼 주어진 item에서만 MoveWindow(...)를 해 보자. 그러면
		//			가. 좌우 scroll에 대해서 MoveWindow(...)를 해도 아무런 문제가 없다.
		//				음의 수평 좌표값에도 잘 대응하고 client area를 일부 벗어나는 상태에도 잘 대응한다.
		//			나. 상하 scroll에 대해서
		//				list control은 기본적으로 상하 client area를 벗어난 item들에 대해서는 draw 함수를 호출하지 않는다.
		//				그래서 벗어난 item에 속한 control들을 MoveWindow(...)을 할 수 없게 된다.
		//				그리고 그 control들은 client area 경계선에 가까운 item에 쌓이게 되고,
		//				나아가 그 item이 더 이상 갱신 작업이 일어나지 않게 만든다.
		//				이런 현상이 점점 안쪽으로 진행되면서 다른 item에 속한 control들도 쌓이게 된다.
		//				* 나.항의 문제를 해결하는 방법으로
		//					모든 item들이 소유한 모든 control에 대해서 MoveWindow(...)를 일괄적으로 수행하는 것을 생각해 볼 수 있다.
		//					수행해 보면 상하 scroll에 대해서 이전 문제가 사라진다. 하지만 또 다른 문제가 발생한다.
		//					list control의 client area안에는 item에 추가한 control말고도 하나의 control이 더 있는데,
		//					바로 header control이다. 이 header control과 추가한 control들이 겹치면서 
		//					header control위에 추가한 control이 놓여 표시되는 문제를 발생시킨다.
		//				** 이 문제를 해결하기 위해  header control을 Z-order를 wndTop으로 유지시켜 보자.
		//					_SetItemControl(...)에서 새로운 control을 추가할 때마다 아래 code를 수행하면 된다.
		//					this->GetHeaderCtrl()->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		//
		//	3. list view contol의 수직 방향 미세 조정 구간
		//		상하 화살표를 눌렀을 떄 수직 방향으로 미세 조정 구간이 발생할 수 있다.
		//		이 미세 조정 구간은 늘 발생하는 것이 아니라 특정 조건일 때 만 발생한다.
		//		일단, 멀티바이트 문자 집합에서는 이 미세 조정 구간이 발생하지 않는다.
		//		유니코드 문자 집합일 때 LVS_EX_GRIDLINES 속성이 없을 때만 발생한다.
		//		(LVS_EX_GRIDLINES이 있어도 하위 control 버전에서 이 미세 조정 구간이 발생하는 bug는 일단 여기서는 접어두자)
		//		수직 방향 미세 조정이란? item이 한번에 행 높이 만큼 이동하는 것이 아니라 8pixels씩 나뉘어 이동되는 것을 말한다.
		//		lpDIS->rcItem의 top, bottom 정보를 확인해 보면 한줄의 item이 8pixels씩 나뉘어 scroll되는 것을 볼 수 있다.
		//		언뜻 드는 생각으로는, 미세 조정 구간에서 draw하지 않고 미세 조정이 끝날 때 한번만 draw하면
		//		draw 횟수를 감소시킬 수 있을 것처럼 보인다.
		//		그런데 마지막에 한번 draw해도 온전히 전체가 그려지지 않는다.
		//		앞의 미세 조정 구간 영역은 그려지지 않고 마지막 조정 구간 영역(8pixel높이)만 그려진다.
		//		그래서, list control의 기본 draw함수는 무조건 미세 조정구간을 그린다.
		//
		//		그런데 child control에 대해서는 이 미세 조정 구간에서 draw할 필요가 없다.
		//		child control은 나누어 그리는 작업이 필요한 것이 아니라 온전히 표시하느냐 하지 않느냐의 판단만 필요하기 때문이다. 
		//		미세 조정 구간이 완료 되었을 때 한번만 child control을 표시해도 될 것이다.
		//		if ((rcSubItem.top - rcAreaExceptHeader.top) % this->_m_nRowHeight != 0)
		//		{
		//			break;
		//		}
		//

		
		// device-context object
		CDC* pDC = CDC::FromHandle(lpDIS->hDC);

		// item 번호
		int nItem = lpDIS->itemID;

		// check whether or not a item is focused and selected
		__ASSERT(lpDIS->itemAction == ODA_DRAWENTIRE); // ODA_FOCUS, ODA_SELECT가 언제 전달되는지?
		// list control의 밖을 click했을 때 ODS_FOCUS와 ODS_SELECTED가 unflag되지 않는다.
		// 따라서 아래 두 줄 같이 lpDIS->itemState을 확인하는 것만으로는 충분하지 않다.
		//BOOL bFocused = lpDIS->itemState & ODS_FOCUS;
		//BOOL bSelected = lpDIS->itemState & ODS_SELECTED;
		LVITEM lvi;
		lvi.mask = LVIF_STATE; // gets the state of a item
		lvi.iItem = nItem;
		lvi.iSubItem = 0;
		lvi.stateMask = (UINT)-1; // gets all state flags
		__super::GetItem(&lvi);
		BOOL bFocused = (lvi.state & LVIS_FOCUSED);
		BOOL bSelected = ((lvi.state & LVIS_SELECTED) && ((__super::GetFocus() == this) || (__super::GetStyle() & LVS_SHOWSELALWAYS)));
		bSelected = (bSelected || (lvi.state & LVIS_DROPHILITED));
		BOOL bFullRowSelect = (__super::GetExtendedStyle() & LVS_EX_FULLROWSELECT); // 행 전체 선택 여부

									#define _CListCtrl_DrawItem_TRACE
									#if defined(_DEBUG) && defined(_CListCtrl_DrawItem_TRACE)
										static int n = 0;
										TRACE(_T("_CListCtrl::DrawItem : %dth, nItem: %d, bFocused: 0x%p, bSelected: 0x%p, lpDIS->rcItem(l,t,r,b): (%5d,%5d,%5d,%5d), row's height: %d\n"),
											++n, nItem, bFocused, bSelected,
											lpDIS->rcItem.left, lpDIS->rcItem.top, lpDIS->rcItem.right, lpDIS->rcItem.bottom,
											this->_m_nRowHeight);
									#endif
		
	// Owner Draw 시작
	#define _OWNERDRAW_CHECKBOX
	#define _OWNERDRAW_SMALLICON
	#define _OWNERDRAW_BKGND
	#define _OWNERDRAW_TEXTS
	#define _OWNERDRAW_CONTROLS
	#define _OWNERDRAW_FOCUSRECT

	#if defined(_OWNERDRAW_CHECKBOX) || defined(_OWNERDRAW_SMALLICON) || defined(_OWNERDRAW_BKGND)
		BOOL bEnabled = __super::IsWindowEnabled(); // disable 상태로 표시해야 하는지 확인한다.
	#endif

	#if defined(_OWNERDRAW_CHECKBOX) || defined(_OWNERDRAW_SMALLICON)
		// draws the bkgnd of a checkbox and an icon
		// this->SetCheck(-1)로 checkbox를 제거할 때 이미 그려져 있는 checkbox를 지울 필요가 있다.
		CRect rcCheckboxIcon(0, 0, 0, 0);
		__super::GetSubItemRect(nItem, 0, LVIR_LABEL, rcCheckboxIcon);
		rcCheckboxIcon.right = rcCheckboxIcon.left;
		rcCheckboxIcon.left = lpDIS->rcItem.left;
		pDC->FillSolidRect(&rcCheckboxIcon, bEnabled ?	::GetSysColor(COLOR_WINDOW) :
																	::GetSysColor(COLOR_BTNFACE));

		// sets color and mask for the checkbox and icon
		UINT uStyle = ILD_TRANSPARENT;
		if (bSelected) uStyle |= ILD_BLEND50;
		else if (lvi.state & LVIS_CUT) uStyle |= ILD_BLEND50;
		//else if ...

	#ifdef _OWNERDRAW_CHECKBOX
		// draws the checkbox
		BOOL bCheck = ((lvi.state & LVIS_STATEIMAGEMASK) >> 12) - 1;
		if (bCheck != -1)
		{
			CImageList* pImageList = __super::GetImageList(LVSIL_STATE);
			if (pImageList != NULL)
			{
				IMAGEINFO ImageInfo;
				pImageList->GetImageInfo(bCheck, &ImageInfo);
				pImageList->DrawEx(pDC, bCheck,
					CPoint(lpDIS->rcItem.left + 4, (lpDIS->rcItem.top + lpDIS->rcItem.bottom - ImageInfo.rcImage.bottom + ImageInfo.rcImage.top) / 2),
					CSize(ImageInfo.rcImage.right - ImageInfo.rcImage.left, ImageInfo.rcImage.bottom - ImageInfo.rcImage.top),
					CLR_DEFAULT, CLR_DEFAULT,
					uStyle);
			}
		}
	#endif

	#ifdef _OWNERDRAW_SMALLICON
		// draws the small icon
		CImageList* pImageList = __super::GetImageList(LVSIL_SMALL);
		if (pImageList != NULL)
		{
			LVITEM lvi;
			lvi.mask = LVIF_IMAGE; // gets the image of a item
			lvi.iItem = nItem;
			lvi.iSubItem = 0;
			__super::GetItem(&lvi);
			IMAGEINFO ImageInfo;  //ImageInfo.rcImage는 여러 image가 연결된 imagelist에서 해당 image의 위치를 나타낸다.
			pImageList->GetImageInfo(lvi.iImage, &ImageInfo);
			CRect rcIcon(0, 0, 0, 0);
			__super::GetItemRect(nItem, &rcIcon, LVIR_ICON);
			pImageList->DrawEx(pDC, lvi.iImage,
				CPoint(rcIcon.left, (rcIcon.top + rcIcon.bottom - ImageInfo.rcImage.bottom + ImageInfo.rcImage.top) / 2),
				CSize(ImageInfo.rcImage.right - ImageInfo.rcImage.left, ImageInfo.rcImage.bottom - ImageInfo.rcImage.top),
				CLR_DEFAULT, CLR_DEFAULT,
				uStyle | (lvi.state & LVIS_OVERLAYMASK));
		}
	#endif
	#endif

	#if defined(_OWNERDRAW_BKGND) || defined(_OWNERDRAW_TEXTS)
		for (int j = 0; j < this->_m_cCols; j++)
		{
			CRect rcSubItem(0, 0, 0, 0); // subitem의 좌표
			__super::GetSubItemRect(nItem, j, LVIR_LABEL, rcSubItem);
			_SUBITEM_INFO sii = this->_m_vSubItem._GetAt(nItem, j);
		#ifdef _OWNERDRAW_BKGND
			// draws the bkgnd of a subitem
			pDC->FillSolidRect(&rcSubItem,
				bEnabled ?	((bSelected && (bFullRowSelect || j == 0)) ? ::GetSysColor(COLOR_HIGHLIGHT)
																					: sii.clrTextBk)
								: ::GetSysColor(COLOR_BTNFACE));
		#endif

		#ifdef _OWNERDRAW_TEXTS
			// not control, just text
			if (sii.pWnd == NULL)
			{
				// gets text of subitem
				CString strT = __super::GetItemText(nItem, j);
				if (strT.GetLength() > 0)
				{
					// sets the color of text
					pDC->SetTextColor((bSelected && (bFullRowSelect || j == 0)) ?
							::GetSysColor(COLOR_HIGHLIGHTTEXT) : sii.clrText);

					// draws text of subitem
					// DrawText의  format 인수가 가지는 값 중에서 수직 정렬을 하려면 아래 두개의 값을 함께 사용해야 한다. 
					// DT_VCENTER  Centers text vertically. This value is used only with the DT_SINGLELINE value.
					// DT_SINGLELINE  Displays text on a single line only. Carriage returns and line feeds do not break the line.
					// 값의 설명을 따져 보면, 위의 두개의 조합은 필수라는 얘기이고,
					// 결국 DT_VCENTER는 작동하지만 CR,LF(\n\r)은 되지 않는다는 얘기이다.
					// 그래서 DrawText 함수는 개행을 포함한 문자열을 수직 정렬하여 표시하기에는 완전하지가 않다.
					// Workaround : control처럼 text도 CRect box를 정렬하는 방식을 채택해 보자.
					// 이로써 이 class에 사용된 정렬 방식은 text, control에 대해 통일되었다.
					CRect rcObject(0, 0, 0, 0);
					rcObject.CopyRect(&rcSubItem);
					rcObject.MoveToXY(0, 0);
					pDC->DrawText(strT, &rcObject, DT_CALCRECT | sii.uDTFormatFlags);
					this->_AlignObject(nItem, j, rcSubItem, rcObject);
					if (sii.uDTFormatFlags & DT_SINGLELINE)
					{
						if (!(sii.uDTFormatFlags & (DT_CENTER | DT_RIGHT)))
							rcObject.OffsetRect(_PADDING_X_DT_SINGLELINE - _PADDING_X - 1, 0);
						else if (sii.uDTFormatFlags & DT_RIGHT)
							rcObject.OffsetRect(_PADDING_X - _PADDING_X_DT_SINGLELINE, 0);
					}
					pDC->DrawText(strT, &rcObject, sii.uDTFormatFlags);
				}
			}
		#endif
		}
	#endif

	#ifdef _OWNERDRAW_CONTROLS
		// 미세 조정 구간이 존재할 때 MoveWindow(...)를 하지 않기 위해
		CRect rcAreaExceptHeader(0, 0, 0, 0);
		__super::GetClientRect(&rcAreaExceptHeader);
		static CHeaderCtrl* _s_pHeaderCtrl = NULL;
		static LONG _s_rcAreaTopExceptHeader = 0;
		CHeaderCtrl* pHeaderCtrl = CListCtrl::GetHeaderCtrl();
		if (pHeaderCtrl != _s_pHeaderCtrl)
		{
			// % row's height 나눈 나머지 값을 rcArea.top으로 정한다.
			// 그 이유는 DrawItem이 한번도 호출되지 않은 상황에서
			// 수직 scroll을 첫 조작부터 고속으로 아래로 당기면 처음 갱신할 nItem이 맨위 첫번째 row가 아닐 수 있기 때문이다.
			// 그런데 list control의 style을 수정할 때 무조건 RedrawWindow()하도록 이 class가 수정된 이후에는
			// % row's height를 할 필요가 없어졌다.
			_s_rcAreaTopExceptHeader = (lpDIS->rcItem.top + this->_m_nRowHeight * this->_m_cRows)
												% this->_m_nRowHeight;

									#if defined(_DEBUG) && defined(_CListCtrl_DrawItem_TRACE)
										TRACE(_T("\t\t\t\t\t\t_s_pHeaderCtrl: 0x%p, pHeaderCtrl: 0x%p, rcAreaExceptHeader(l,t,r,b): (%5d,%5d,%5d,%5d)\n"),
										_s_pHeaderCtrl, pHeaderCtrl, rcAreaExceptHeader.left, _s_rcAreaTopExceptHeader, rcAreaExceptHeader.right, rcAreaExceptHeader.bottom);
									#endif

			_s_pHeaderCtrl = pHeaderCtrl;
		}
		rcAreaExceptHeader.top = _s_rcAreaTopExceptHeader;

		// list control이 갱신될 때마다 list control에 보이는 모든 item들에 대해서 일괄적으로 갱신 작업이 발생한다.
		// 각 item이 갱신될 때 이 DrawItem(...)이 매번 호출되고,
		// DrawItem(...)내에서 모든 child control들을 이동시키는 작업 _MoveAllWindow(...) 이 수행된다.
		// 일괄적으로 갱신되는 item들의 이동량이 동일하기 때문에 각 item마다 수행된 _MoveAllWindow(...)의 수행결과는 동일하다.
		// 즉 각각의 item에 대해 수행되는 _MoveAllWindow(...) 작업은 중복 작업이라 할 수 있다.
		// 일괄 갱신 작업의 시작 또는 끝을 알아낼 수 있다면,
		// 그 시작 또는 끝에서 한번만 _MoveAllWindow(...)를 수행한다면 중복 작업을 피할 수 있다.
		// Custrom Draw에서는 draw state를 조사하여 그 시작을 확인할 수 있다.
		// Owner Draw에서는 아래 조건을 사용하여 그 시작을 알 수 있을 것이다.
			// 보완 필요! 이 조건문은 안전하지 않다는 것이 발견되었다.
			// 갱신되는 item들 중 맨 처음 item이 이 조건을 만족하지 않을 때가 있다.
				// 일단 item들을 모두 삭제하고
				// item 한 줄 삽입 -> button 한 개 추가
				// -> button이 list control을 벗어나 수직 scroll bar가 표시될 때까지 item삽입 반복
				// -> 수직 scroll bar가 사라질 때까지 item삭제 반복
				// -> 이후부터 item을 삭제할 때 button이 이동하지 않는(처음 item이 이 조건을 만족하지 않는) 문제 현상이 발생한다.
		static int _s_nPrevItem = 0x7ffff;
		if (nItem <= _s_nPrevItem)
		{
									#if defined(_DEBUG) && defined(_CListCtrl_DrawItem_TRACE)
									TRACE(_T("\t\t\t\t\t\tif (nItem: %d <= _s_nPrevItem: %d) this->_MoveAllWindow(rcAreaExceptHeader);\n"),
																	nItem, _s_nPrevItem);
									#endif
			this->_MoveAllWindow(rcAreaExceptHeader);
		}
		_s_nPrevItem = nItem;
	#endif

	#ifdef _OWNERDRAW_FOCUSRECT
		// draws focus rectangle
		//if (bFocused) // 이 조건문을 사용하면 list control 밖을 click했을 때 focus가 사라지지 않는다.
		if (bSelected)
		{
			// gets the area of label
			CRect rcItemLabel(0, 0, 0, 0);
			__super::GetItemRect(nItem, &rcItemLabel, LVIR_LABEL);
			if (bFullRowSelect) rcItemLabel.right = lpDIS->rcItem.right;
			pDC->DrawFocusRect(&rcItemLabel);
		}
	#endif

	}

	/*********************************************************************************************************************************
	* Custom Draw - OnNMCustomdraw
	*********************************************************************************************************************************/
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
	{
		LPNMLVCUSTOMDRAW pNMLVCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
									#ifdef _OWNERDRAW_CONTROLS
										static int _s_n = 0;
										if (pNMLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
											TRACE(_T("\n_CListCtrl::OnNMCustomdraw(CDDS_PREPAINT) : %d ******************************\n"), ++_s_n);
									#endif
		// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		*pResult = CDRF_DODEFAULT;

		if (!((__super::GetStyle() & LVS_REPORT) && (__super::GetExtendedStyle() & _LVS_EX_CUSTOMDRAWFIXED))) return;

		// 1. 우선 알아둘 점은
		//		dwDrawStage의 값은 PRE와 POST로 쌍을 이루는 값들인데
		//		자동으로 쌍을 이루며 수행되는 것은 아니다라는 것이다.
		//		pResult값으로 CDRF_NOTIFYPOSTPAINT를 넣어 주지 않는 한 POST는 절대 수행되지 않고 PRE만 수행된다.
		//		가. CDDS_PREPAINT에서 pResult로 CDRF_NOTIFYPOSTPAINT를 넘기면
		//			CDDS_POSTPAINT를 발생시킨다.
		//		나. CDDS_ITEMPREPAINT에서 pResult로 CDRF_NOTIFYPOSTPAINT를 넘기면
		//			CDDS_ITEMPOSTPAINT를 발생시킨다.
		//		CDDS_POSTPAINT, CDDS_ITEMPOSTPAINT가 발생되고 나면
		//		더 이상 하위 item에 대한 CDDS_ITEMPREPAINT를 발생시킬 수 없다.
		//		* (CDDS_SUBITEM | CDDS_ITEMPREPAINT)에서 pResult로 CDRF_NOTIFYPOSTPAINT를 넘겨도
		//			쌍을 이루는 POST가 없어, 다음 (CDDS_SUBITEM | CDDS_ITEMPREPAINT)를 발생시킨다.
		// 2. 일반적인 진행 순서를 보자.
		//		가. 갱신이 필요할 때 CDDS_PREPAINT가 제일 먼저 발생된다.
		//			CDRF_NOTIFYITEMDRAW를 pResult 값으로 넘긴다.
		//		나. CDRF_NOTIFYITEMDRAW는 하위 item 들에 대해 CDDS_ITEMPREPAINT를 발생시킨다.
		//			CDRF_NOTIFYSUBITEMDRAW를 pResult 값으로 넘긴다.
		//		다. CDRF_NOTIFYSUBITEMDRAW는 하위 subitem에 대해 (CDDS_SUBITEM | CDDS_ITEMPREPAINT)를 발생시킨다.
		//			마지막으로 CDRF_DODEFAULT를 pResult 값으로 넘긴다.
		//			그렇지 않으면 focus rect 등이 표시되지 않는다.
		//		* LVS_REPORT style이 설정되지 않으면 위의 다.항으로 진행되지 않는다.
		//		* LVS_OWNERDRAWFIXED style이 설정되면 위의 다.항으로 진행되지 않는다.
		// 3. Tips : 호출 횟수가 가장 많은 (CDDS_SUBITEM | CDDS_ITEMPREPAINT)에 대해 먼저 if문을 수행한다.
		
		if (pNMLVCD->nmcd.dwDrawStage == (CDDS_SUBITEM | CDDS_ITEMPREPAINT))
		{
			*pResult = CDRF_DODEFAULT;
			// TODO: 여기에 특수화된 코드를 추가
	#define _CUSTOMDRAW_BKGND
	#ifdef _CUSTOMDRAW_BKGND
			pNMLVCD->clrText = this->_m_vSubItem._GetAt(pNMLVCD->nmcd.dwItemSpec, pNMLVCD->iSubItem).clrText;
			pNMLVCD->clrTextBk = this->_m_vSubItem._GetAt(pNMLVCD->nmcd.dwItemSpec, pNMLVCD->iSubItem).clrTextBk;
	#endif
							#define _CListCtrl_OnNMCustomdraw_CDDS_SUBITEM_CDDS_ITEMPREPAINT_TRACE
							#if defined(_DEBUG) && defined(_CListCtrl_OnNMCustomdraw_CDDS_SUBITEM_CDDS_ITEMPREPAINT_TRACE)
								static int n = 0;
								CRect rcSubItem(0, 0, 0, 0);
								__super::GetSubItemRect(pNMLVCD->nmcd.dwItemSpec, pNMLVCD->iSubItem, LVIR_BOUNDS, rcSubItem);
								TRACE(_T("_CListCtrl::OnNMCustomdraw(CDDS_SUBITEM_CDDS_ITEMPREPAINT) : %dth, iSubItem: %d, (l,t,r,b): (%4d,%4d,%4d,%4d), (%4d,%4d,%4d,%4d)\n"),
									++n,
									pNMLVCD->iSubItem,
									pNMLVCD->nmcd.rc.left, pNMLVCD->nmcd.rc.top, pNMLVCD->nmcd.rc.right, pNMLVCD->nmcd.rc.bottom,
									rcSubItem.left, rcSubItem.top, rcSubItem.right, rcSubItem.bottom);
							#endif
		}
		else if (pNMLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
		{
			*pResult = CDRF_NOTIFYSUBITEMDRAW;
			// TODO: 여기에 특수화된 코드를 추가
							#define _CListCtrl_OnNMCustomdraw_CDDS_ITEMPREPAINT_TRACE
							#if defined(_DEBUG) && defined(_CListCtrl_OnNMCustomdraw_CDDS_ITEMPREPAINT_TRACE)
								static int n = 0;
								CRect rcItem(0, 0, 0, 0);
								__super::GetItemRect(pNMLVCD->nmcd.dwItemSpec, rcItem, LVIR_BOUNDS);
								TRACE(_T("_CListCtrl::OnNMCustomdraw(CDDS_ITEMPREPAINT) : %dth, dwItemSpec: %d, (l,t,r,b): (%4d,%4d,%4d,%4d), (%4d,%4d,%4d,%4d)\n"),
									++n,
									pNMLVCD->nmcd.dwItemSpec,
									pNMLVCD->nmcd.rc.left, pNMLVCD->nmcd.rc.top, pNMLVCD->nmcd.rc.right, pNMLVCD->nmcd.rc.bottom,
									rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);
							#endif
		}
		else if (pNMLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
		{
			*pResult = CDRF_NOTIFYITEMDRAW;
			// TODO: 여기에 특수화된 코드를 추가
	#define _CUSTOMDRAW_CONTROLS
	#ifdef _CUSTOMDRAW_CONTROLS
			// 미세 조정 구간이 존재할 때 MoveWindow(...)를 하지 않기 위해
			int nItem = 0;
			CRect rcAreaExceptHeader(0, 0, 0, 0);
			static CHeaderCtrl* _s_pHeaderCtrl = NULL;
			static LONG _s_rcAreaTopExceptHeader = 0;
			CHeaderCtrl* pHeaderCtrl = CListCtrl::GetHeaderCtrl();
			if (pHeaderCtrl != _s_pHeaderCtrl) // header가 변경되면
			{
				__super::GetClientRect(&rcAreaExceptHeader);
				if (pNMLVCD->nmcd.rc.left == rcAreaExceptHeader.left
					&& pNMLVCD->nmcd.rc.right == rcAreaExceptHeader.right
					&& pNMLVCD->nmcd.rc.bottom == rcAreaExceptHeader.bottom)
				{
					if (__super::GetItemRect(nItem, rcAreaExceptHeader, LVIR_BOUNDS)) // item이 존재하면
					{
						_s_rcAreaTopExceptHeader = (rcAreaExceptHeader.top + this->_m_nRowHeight * this->_m_cRows)
															% this->_m_nRowHeight;
						_s_pHeaderCtrl = pHeaderCtrl;
					}
				}
			}
			__super::GetClientRect(&rcAreaExceptHeader);
			rcAreaExceptHeader.top = _s_rcAreaTopExceptHeader;
	#endif
							#define _CListCtrl_OnNMCustomdraw_CDDS_PREPAINT_TRACE
							#if defined(_DEBUG) && defined(_CListCtrl_OnNMCustomdraw_CDDS_PREPAINT_TRACE)
										static int n = 0;
										CRect rcSubItem(0, 0, 0, 0);
								#ifndef _CUSTOMDRAW_CONTROLS
										CRect rcAreaExceptHeader(0, 0, 0, 0);
										CHeaderCtrl* _s_pHeaderCtrl = CListCtrl::GetHeaderCtrl();
										__super::GetClientRect(&rcAreaExceptHeader);
								#endif
								__super::GetItemRect(0, rcSubItem, LVIR_BOUNDS);
								TRACE(_T("_CListCtrl::OnNMCustomdraw(CDDS_PREPAINT) : %dth, (l,t,r,b): (%4d,%4d,%4d,%4d), (%4d,%4d,%4d,%4d), %p, (%4d,%4d,%4d,%4d), row's height: %d ******************************\n"),
										++n,
										pNMLVCD->nmcd.rc.left, pNMLVCD->nmcd.rc.top, pNMLVCD->nmcd.rc.right, pNMLVCD->nmcd.rc.bottom,
										rcSubItem.left, rcSubItem.top, rcSubItem.right, rcSubItem.bottom,
										_s_pHeaderCtrl,
										rcAreaExceptHeader.left, rcAreaExceptHeader.top, rcAreaExceptHeader.right, rcAreaExceptHeader.bottom,
										this->_m_nRowHeight);
							#endif
	#ifdef _CUSTOMDRAW_CONTROLS
			this->_MoveAllWindow(rcAreaExceptHeader);
	#endif
		}
	}

	/*********************************************************************************************************************************
	* Owner Draw, Custom Draw - 공통, _AlignObject, _MoveAllWindow
	*********************************************************************************************************************************/
private:	
	// *** text, control 두 object을 표시하는 데에 동일한 정렬 방식을 사용한다.
	void _AlignObject(int nRow, int nCol,
						CRect &rcSubItem, // nRow, nCol이 가리키는 subitem의 위치를 나타낸다.
						CRect &rcObject) // text나 control을 표시할 위치를 나타내고, 이 값에 대해 정렬 작업이 수행된다.
	{
		if (/*__TVERIFY*/(!this->_IsSubItem(nRow, nCol))) return;

		// rcSubItem과 rcObject의 기준 위치를 잡아 준다.
		rcSubItem.bottom--; // subitem의 좌표를 보면, bottom, left는 gridline을 물고 있고, top, right는 gridline을 물고 있지 않는다.
		rcSubItem.left++; // 대칭을 맞추기 위해 subitem의 bottom은 -1, left는 +1 해주어야 한다.
		rcSubItem.DeflateRect(_PADDING_X, _PADDING_Y); // 안쪽으로 _PADDING만큼 공간을 넣어 준다.
		rcObject.MoveToXY(rcSubItem.TopLeft());
		
		CPoint p; // subitem의 top, left 위치를 기준으로 정렬 작업을 통해 계산한 offset량을 나타낸다.
		UINT uDTFormatFlags = this->_GetItemFormat(nRow, nCol); // Retrieves the status of the text-alignment flags for the subitem.
		// 가로 정렬
		switch (uDTFormatFlags & (DT_LEFT | DT_CENTER | DT_RIGHT))
		{
		case DT_LEFT:
			p.x = 0;
			break;
		case DT_CENTER:
			p.x = (rcSubItem.Width() - rcObject.Width()) / 2;
			break;
		case DT_RIGHT:
			p.x = rcSubItem.Width() - rcObject.Width();
			break;
		}
		// 세로 정렬
		switch (uDTFormatFlags & (DT_TOP | DT_VCENTER | DT_BOTTOM))
		{
		case DT_TOP:
			p.y = 0;
			break;
		case DT_VCENTER:
			p.y = (rcSubItem.Height() - rcObject.Height()) / 2;	
			break;
		case DT_BOTTOM:
			p.y = (rcSubItem.Height() - rcObject.Height());
			break;
		}
		// 정렬 결과 적용
		rcObject.OffsetRect(p);
	}

private:
	void _MoveAllWindow(CRect &rcAreaExceptHeader)
	{
	//#define _OWNERDRAW_DEFERWINDOWMODE // DEFERWINDOWMODE가 MoveWindow보다 DrawItem 호출 횟수가 많아서 주석 처리한다.
	#ifdef _DEFERWINDOWMODE
		HDWP hdwp = ::BeginDeferWindowPos(this->_m_cCols);
	#endif
		for (int j = 0; j < this->_m_cCols; j++)
		{
			for (int i = 0; i < this->_m_cRows; i++)
			{
				CWnd *pWnd = this->_m_vSubItem._GetAt(i, j).pWnd;
				if (pWnd != NULL)
				{
					// 줄 단위 선택 모드이고 선택되지 않았을 때
					if (!this->_m_bAllControlsViewState && __super::GetItemState(i, LVIS_SELECTED) != LVIS_SELECTED)
					{
						// pWnd->ShowWindow 내부에서
						// control site가 있는 경우와 없는 경우에 따라
						// ShowWindow 처리 루틴이 다르다.
						// 그런데, control site가 있는 경우
						// SW_HIDE하면, m_hWnd가 0이 되어 버린다.
						::ShowWindow(pWnd->m_hWnd, SW_HIDE);
						continue;
					}

					// subitem 좌표 얻기
					CRect rcSubItem(0, 0, 0, 0);
					__super::GetSubItemRect(i, j, LVIR_LABEL, rcSubItem);

					// 미세 조정 구간이면 탈출
					if ((rcSubItem.top - rcAreaExceptHeader.top) % this->_m_nRowHeight != 0)
					{
						#if defined(_DEBUG) && (defined(_CListCtrl_OnNMCustomdraw_CDDS_PREPAINT_TRACE) \
														|| defined(_CListCtrl_DrawItem_TRACE))
									TRACE(_T("Skip the smooth scrolling feature : (%d - %d) %% %d = %d\n"),
										rcSubItem.top, rcAreaExceptHeader.top, this->_m_nRowHeight,
										(rcSubItem.top - rcAreaExceptHeader.top) % this->_m_nRowHeight);
						#endif
						break;
					}
					
					// subitem의 위치 및 alignment style을 고려하여 control이 표시되어야 할 위치 계산
						//		* 윈도우 크기 조정에 따른 DrawItem 호출 상황
						//				CStatic* pControl = (CStatic*) // not
						//				CButton* pControl = (CButton*) // not
						//				CEdit* pControl = (CEdit*) // not
						//				CComboBox* pControl = (CComboBox*) // 행높이 > 23일때 continueing
						//				CListBox* pControl = (CListBox*) // continueing
						//			따라서, CComboBox, CListBox 때문에
						//			윈도우를 옮길 때 위치 조정은 하되 크기 조정을 해서는 안된다.
					CRect rcObject(0, 0, 0, 0);
					pWnd->GetWindowRect(&rcObject);
					this->_AlignObject(i, j, rcSubItem, rcObject);

					// control의 위치 이동
				#ifdef _DEFERWINDOWMODE
					hdwp = ::DeferWindowPos(hdwp,
						pWnd->GetSafeHwnd(), 0,
						rcObject.left, rcObject.top,
						rcObject.Width(), rcObject.Height(),
						SWP_NOACTIVATE
						| SWP_NOOWNERZORDER
						| SWP_NOZORDER);
				#else
					pWnd->MoveWindow(&rcObject);
				#endif
					// 줄 단위 선택 모드이고 선택되어 있을 때나 줄 단위 선택 모드에서 해제되었을 때
					::ShowWindow(pWnd->m_hWnd, SW_SHOW);
				}
			}
		}

	#ifdef _DEFERWINDOWMODE
		::EndDeferWindowPos(hdwp);
	#endif
	}

	/*********************************************************************************************************************************
	* Owner Draw, Custom Draw, and Default Draw 설정
	*		*** ModifyStyle, SetExtendedStyle의 RedrawWindow 여부
	*		style												UNICODE		MULTIBYTE
	*		-----------------------------------------------------------------------------------------
	*		LVS_OWNERDRAWFIXED						O				X
	*		_LVS_EX_CUSTOMDRAWFIXED					O				X
	*		LVS_REPORT										O				O
	*		LVS_OWNERDRAWFIXED 반복 설정			X				X
	*		_LVS_EX_CUSTOMDRAWFIXED 반복 설정 	O				X => O되도록 SetExtendedStyle() 작성
	*		LVS_REPORT 반복 설정							X				X
	*********************************************************************************************************************************/
private:
	// *** 추가된 모든 control의 show state를 sets한다. 인수 nCmdShow의 값은 ShowWindow와 동일하다.
	void _ShowControls(int nCmdShow)
	{
		_SUBITEM_INFO sii;
		for (int i = 0; i < this->_m_cRows; i++)
		{
			for (int j = 0; j < this->_m_cCols; j++)
			{
				sii = this->_m_vSubItem._GetAt(i, j);
				if (sii.pWnd != NULL) // is control
				{
					::ShowWindow(sii.pWnd->m_hWnd, nCmdShow);
				}
			}
		}
	}

public:

	// *** Owner Draw 설정 : ModifyStyle()
	BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0)
	{
		// LVS_OWNERDRAWFIXED가 추가될 때 _LVS_EX_CUSTOMDRAWFIXED를 제거한다.
		DWORD dwOldStyle = __super::GetStyle();
		if (!(dwOldStyle & LVS_OWNERDRAWFIXED) && (dwAdd & LVS_OWNERDRAWFIXED))
		{
			DWORD dwNewExStyle = __super::GetExtendedStyle();
			dwNewExStyle &= ~_LVS_EX_CUSTOMDRAWFIXED;
			__super::SetExtendedStyle(dwNewExStyle);
		}

		int nRowHeight = this->_GetRowHeight(); 
		BOOL bRet = __super::ModifyStyle(dwRemove, dwAdd, nFlags);
		this->_SetRowHeight(nRowHeight); // 속성을 변경하면 row's height가 default(17)로 변경되는데 row's height를 유지시킬 필요가 있다.
	#ifndef _UNICODE
		// 위에서 설명한 '반복 설정'에 대한 설명 참고
		if (!(dwOldStyle & LVS_OWNERDRAWFIXED) && (dwAdd & LVS_OWNERDRAWFIXED))
			__super::RedrawWindow();
	#endif

		// LVS_REPORT가 제거될 때 추가된 모든 control들을 숨긴다.
		if ((dwOldStyle & LVS_REPORT) && (dwRemove & LVS_REPORT))
			this->_ShowControls(SW_HIDE);

		return bRet;
	}
	// *** Custom Draw 설정 : SetExtendedStyle()
	DWORD SetExtendedStyle(DWORD dwNewStyle)
	{
		// _LVS_EX_CUSTOMDRAWFIXED가 추가될 때 LVS_OWNERDRAWFIXED를 제거한다.
		DWORD dwOldStyle = __super::GetExtendedStyle();
		if (!(dwOldStyle & _LVS_EX_CUSTOMDRAWFIXED) && (dwNewStyle & _LVS_EX_CUSTOMDRAWFIXED))
		{
			__super::ModifyStyle(LVS_OWNERDRAWFIXED, 0, 0);
		}
		
		int nRowHeight = this->_GetRowHeight();
		DWORD  bRet = __super::SetExtendedStyle(dwNewStyle);
		this->_SetRowHeight(nRowHeight); // 속성을 변경하면 row's height가 default(17)로 변경되는데 row's height를 유지시킬 필요가 있다.
	#ifndef _UNICODE
		//위에서 설명한 '반복 설정'에 대한 설명 참고 if (!(dwOldStyle & _LVS_EX_CUSTOMDRAWFIXED) && (dwNewStyle & _LVS_EX_CUSTOMDRAWFIXED))
		__super::RedrawWindow();
	#endif

	#ifdef _UNICODE
		// LVS_EX_GRIDLINES 추가될 때 KB813791 - BUG를 확인한다.
		if (!(dwOldStyle & LVS_EX_GRIDLINES) && (dwNewStyle & LVS_EX_GRIDLINES))
		{
			BOOL bParam;
			if (::SystemParametersInfo(SPI_GETLISTBOXSMOOTHSCROLLING, 0, &bParam, 0)	&& bParam == TRUE)
			{
				// Common Control DLL Versions Numbers : https://msdn.microsoft.com/en-us/library/windows/desktop/hh298349(v=vs.85).aspx
				if (__TVERIFYM(::_GetDllVersion(_T("ComCtl32.dll")) <= MAKELONG(0, 6),
_T("\n\tKB813791 - BUG: Gridlines for list-view control are not drawn correctly using the LVS_EX_GRIDLINES style.\n\
SYMPTOMS :\n\
	When you use Extended List-View Style LVS_EX_GRIDLINES to display gridlines in a list-view control,\n\
	grid lines may be drawn over the items or may not appear while scrolling contents of the control.\n\
CAUSE : \n\
	The list-view control incorrectly handles the smooth scrolling feature\n\
	when scrolling the contents of the control.\n\
	This only affects the list-view control that is implemented in the Comctl32.dll file version 6.0.\n\
WORKAROUND :\n\
	To turn off the smooth scrolling feature of the list-view control and the list box control, follow these steps:\n\
	1. In Control Panel, double-click Display.\n\
	2. On the Appearance tab, click Effects.\n\
	3. Click to clear the Use the following transition effect for menus and tooltips check box.\n\
	Applications can use the SPI_SETLISTBOXSMOOTHSCROLLING parameter\n\
	when calling the SystemParametersInfo function to enable or to disable smooth scrolling\n\
	in list-view controls and in list box controls.")))
				{
					// MSDN에서는 아래처럼 programmatically 하게도 된다고 하나 막상 해 보면 효과가 없다.
					bParam = FALSE;
					::SystemParametersInfo(SPI_SETLISTBOXSMOOTHSCROLLING, 0, &bParam, 0);
				}
			}
		}
	#endif

		return bRet;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// Draw - 끝
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
};

BEGIN_TEMPLATE_MESSAGE_MAP(_CListCtrl, T, T)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &_CListCtrl::OnNMCustomdraw) //custom draw
	ON_WM_MEASUREITEM_REFLECT() // row height
	ON_WM_LBUTTONDOWN() // checkbox
	//ON_NOTIFY_REFLECT(NM_CLICK, &_CListCtrl::OnNMClick) // checkbox
	//ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &_CListCtrl::OnNMItemChanged) // checkbox
	ON_WM_DESTROY() // delete all controls
END_MESSAGE_MAP()

// BEGIN_TEMPLATE_MESSAGE_MAP을 class 내부에 작성할 때는 아래 코드 사용할 것.
// GetThisMessageMap을 static으로 지정하고, 두개의 template < typename T >을 제거해야 한다.
// 이를 위해 BEGIN_TEMPLATE_MESSAGE_MAP 매크로를 풀어서 다시 작성해야 한다.
//
//PTM_WARNING_DISABLE
////template < typename T >
//const AFX_MSGMAP* _CListCtrl< T >::GetMessageMap() const
//{
//	return GetThisMessageMap();
//}
//
////template < typename T >
//static/*추가*/ const AFX_MSGMAP* PASCAL _CListCtrl< T >::GetThisMessageMap()
//{
//	typedef _CListCtrl< T > ThisClass;
//	typedef T TheBaseClass;
//	static const AFX_MSGMAP_ENTRY _messageEntries[] =
//	{
//		ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &_CListCtrl::OnNMCustomdraw) //custom draw
//		ON_WM_MEASUREITEM_REFLECT() // row height
//		ON_WM_LBUTTONDOWN() // checkbox
//		ON_WM_DESTROY() // delete all controls
//END_MESSAGE_MAP()