#pragma once


// CCossImage 대화 상자입니다.

#include "resource.h"


#define    MAX_GRAPHIC		10000

#define GRA_TEXT		1
#define GRA_CROSS		2
#define GRA_RECT		3
#define GRA_LINE		4
#define GRA_CIRCLE		5
#define GRA_ANGLERECT	6
#define GRA_ELLIPSE		7
#define GRA_LINEABS		8


#include "CDIB.h"
#include "atlImage.h"
typedef struct GRAPHICSET_
{
	int nType;		//1:TEXT   2: CROSS   3:RECTANGLE   4:Line  5:Circle  6:각도있는 사각형  7: Ellipse
	int nABS;		//1: 이미지 기준 0:다이얼로그 기준 
	int x1, y1;		//
	int x2, y2;
	int nWing;
	int radius;
	int style;		//Pen Style
	POINT pt[4];    //오른쪽으로 돈다 
	CString str;
	COLORREF color;

	int fontSize;
	TCHAR fontName[20];
}GRAPHICSET;

class CCossImage : public CDialog
{
	DECLARE_DYNAMIC(CCossImage)

public:
	CCossImage(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCossImage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_COSSIMAGE };


	int PrepareDC(CDC* pDC);		   // ROI와 Size를 맞추기 위해서 OnPaint에서 Call

	CROIControl	*m_pROI;				//KROI의 Object번지 
	CWnd	*m_pParent;					//이 Class의 부모 Window   이렇게 데이타 전달 => ::SendMessage(m_pParent->m_hWnd, WM_IMAGE_INFO, (DWORD)&m_pt, (DWORD)&m_Gray);
	CDib	*m_dibImage;				//Display를 위한 Class

	int        IMAGE_COLOR;
	int        m_nDisplayID;
	LPBYTE     m_fmDisplay;				//영상 Display 용 버퍼 	
	LPBYTE     m_fmMask;				//영상 마스크 
	LPBYTE     m_fmDisplayGray;			//영상 Display 용 버퍼 	

	LPBYTE	   m_fmProcess;				//Process용 버퍼, 꼭 사용할 필요는 없다. BMP Load시 그 영상을 복사해 준다.
	COLORREF   m_MainColor;				//화면에 글씨 쓸때 사용하는 색 
	CString    m_sTitle;				//Dialog Title

	bool       m_bSetFont;
	CFont      m_Font;

	bool        m_UseMask;
	int         m_nShowMask;			//해당 마스크 보여줌
	bool        m_UseScrollX;
	bool        m_UseScrollY;
	bool        m_bMouseMoveROI;		//이 Flag가 세팅 되어 있으면 마우스로 Image를 찍으면 ROI가 그 위치로 이동한다.
	TCHAR        m_MouseTrackerName[MAXNAMELENGTH];

	GRAPHICSET m_Graphic[MAX_GRAPHIC];	//지워지지 않는 Graphic 
	int        m_nPixelCnt;				//지워지지 않은 Pixel수
	int        m_nPixelX[2000];
	int        m_nPixelY[2000];
	COLORREF   m_PixelColor;

	int        m_nGraphicCount;			//Add된 Graphic 수 <=  MAX_GRAPHIC
	bool	   m_bAddGraphic;			//이 값이 true면 Draw...을 사용한 Graphic데이타를 저장한다.  
	bool       m_bDrawGraphic;			//이 값이 true인 동안은 Draw 를 Call해도 저장되지 않는다.
	bool       m_bFitZoom;
	double		m_dOldZoomX;
	double		m_dOldZoomY;

	bool        m_bSetMouseZoom;		//True면 마우스 왼쪽은 Zoom IN 마우스 오른쪽은 ZoomOUT
	bool		m_bShowCenterCross;		//True면 Center Cross그린다.
	COLORREF    m_CenterColor;			//Center Cross Color
	int         m_nCenterCrossType;		//Center Cross TYPE  PS_DOT, PS_SOLID
	bool   m_ROIAngleDialogEnable;		//ROI Angle Dialog띄울수 있는 조건 
	bool   m_bShowGrayValue;			// 픽셀값 표시(true)
	bool   m_bShowImageName;			// 이미지이름 표시 
	bool   m_bShowZoom;					//Zoom 표시
	bool   m_bZoomCenter;				//true면 Zoom이 바뀔때 이미지가 중심을 기준으로 바뀜 
	int    m_nShowPosX, m_nShowPosY;	// 픽셀값 표시 위치 

	CString  m_sImageName;				//Channel
	double m_dSx, m_dSy;				//배율	1[1:1]  0.5 [2:1]   0.25[4:1] 0.125[8:1] 0.0625 [16:1]
	int  m_nOrgX;						//시작점 ( PAN 때문에 발생한 값)
	int  m_nOrgY;						//
	bool m_bflagPAN;					//이 Flag가 셋팅되어 있으면 왼쪽 마우스로 영상이 이동 시킬 수 있음 
	bool m_bLButtonDown;				//마우스 왼쪽 버튼이 눌린동안 True
	int  m_nPanX1, m_nPanY1;			//PAN시작 마우스 L버튼 눌렀을때 실제 영상값 

	int   m_SizeX, m_SizeY;				//Display할 GUI Size 
	int   m_WIDTH, m_HEIGHT;			//Display할 Real Image Size
	int   m_nDisplayWidth, m_nDisplayHeight, m_nDisplayPitch; //실제 BMP영상을 뿌릴 Size

	int  m_nCurrentPosX;				//마우스 위치의 Pixel값을 표시하기위한 데이타.
	int  m_nCurrentPosY;			    //
	int  m_nCurrentPixel[3];			//
	bool m_bRefresh; //당분간 사용안함(앞으로도 안할 확률이 높음)
	bool m_bUseDCZoom;
	bool m_bUseMousePixel;				//마우스값을 Pixel로 보냄 
	bool m_bUseRButtonRotation;			//RButton을 Rotation에 사용함 
	bool m_bUseRButtonXWheel;			//RButton 클릭 후 휠 조정 시 좌우로 이미지 이동
	CImage m_Loadimage;					//PNG 이미지
	CImage m_Saveimage;					//PNG 이미지
#ifdef WEB_DAKANO_MULTAE
	CImage m_SaveMapimage;					//PNG 이미지
#endif	
	//
	LPBYTE fmOutput;

	void ImageDisplay(CDC *pDC);		//전체 Image에서 특정 부분만 뽑아서 Dialog에 Display한다.


	void SetTitle(CString str);					//다이얼로그 Title설정 
	void SetImageName(CString str);				//화면에 CH0 같은 이름 설정 
	long SetImageData(int nWidth, int nHeight); //이미지 크기 설정 
	void DisplayClose();
	void SetUseMask(bool flag) {m_UseMask=flag;}
	void SetShowMask(int nShowMask) {m_nShowMask=nShowMask;}
	void SetShowZoom(int ShowZoom) { m_bShowZoom=ShowZoom;}
	void SetZoomCenter(bool flag){m_bZoomCenter=flag;} 
	long    FitZoom();							//현재 Dialog크기에 영상을 맞춘다.
	long FitZoomXY();
	long	Zoom(double dSx, double dSy);		//Image Zoom  1[1:1]  0.5 [2:1]   0.25[4:1] 0.125[8:1] 0.0625 [16:1]
	double	GetZoom();							//Zoom Data return
	double	GetZoomX();							//Zoom Data return
	double	GetZoomY();							//Zoom Data return
	void	UpdateImage(bool flag=false);
	void	UpdateImageWindow();
	void    GetImageInfo(int *width, int *height) { *width=m_WIDTH;*height=m_HEIGHT;}//현재 Image 폭, 높이 
	void    GetImagePart(LPBYTE fmD, int nX, int nY, int nSize);
	LPBYTE GetImageDataProcess(int *pitch)	{ *pitch=m_WIDTH; return m_fmProcess;}
	LPBYTE GetImageDataDisplay(int *pitch);												//현재 Display되는 이미지정보 Return
	LPBYTE GetImageDataDisplayRGB(int *pitch)	{ *pitch=m_WIDTH; return m_fmDisplay;}	//현재 Display되는 이미지정보 Return

	int MakeColor2GrayAll();
	int MakeColor2Gray(LPBYTE fmGray, LPBYTE fmColor, int left, int top, int right, int bottom, int pitch); //칼라영상을 흑백영상으로 만듦 

	//-------------------------------------------------------------------------------------------------------
	// 아래 6개 함수는 m_bAddGraphic값이 TRUE인 동안 사용되면 저장되어 계속 그려진다. (ResetGraphic() 하면 지워짐)
	void DrawRectShape(CDC *pDC, POINT *pt, COLORREF  color);		//사각형 그리는 함수 
	void DrawRectShape(CDC *pDC, TPoint *pt, COLORREF  color);		//사각형 그리는 함수 
	void DrawRectangle(CDC *pDC, int left, int top, int right, int bottom, COLORREF  color);							//사각형 그리는 함수 
	void DrawCircle(CDC *pDC, int cx, int cy, int radius, COLORREF  color, int nType=PS_SOLID);							//Circle 그리는 함수 
	void DrawEllipse(CDC *pDC, int left, int top, int right, int bottom, COLORREF  color, int nType);					//Ellipse 그리는 함수 
	void DrawLine(CDC *pDC, int x1, int y1, int x2, int y2, COLORREF  color);											//Line 그리는 함수
	void DrawLineABS(CDC *pDC, int x1, int y1, int x2, int y2, COLORREF  color);
	void DrawCross(CDC *pDC, int x, int y, int nWing, COLORREF  color);													// + 
	void DrawText(CDC *pDC, int x, int y, COLORREF color, CString str, CFont *pFont=NULL);								//다이얼로그 왼쪽끝 기준 
	void DrawTextABS(CDC *pDC, int x, int y, COLORREF color, CString str, CFont *pFont=NULL);							//이미지 왼쪽 끝 기준 
	void DrawTextFont(CDC *pDC, int x, int y, COLORREF color, CString str, int fontSize=0, TCHAR *fontname=NULL);		//다이얼로그 왼쪽끝 기준 
	void DrawTextABSFont(CDC *pDC, int x, int y, COLORREF color, CString str, int fontSize=0, TCHAR *fontname=NULL);	//이미지 왼쪽 끝 기준 
	void DrawPoint(CDC *pDC, int x, int y, COLORREF  color);															//SetPixel 와 동일한 함수 
	void DrawMask(CDC *pDC);
	void SetTextFont(bool flag, int fontSize, TCHAR *fontName);
	void SetPixel(CDC *pDC, int x, int y, COLORREF color);																// Pixel 한점 찍는 함수 
	void DrawPixelArray(CDC *pDC, int nCount, long *x, long *y, COLORREF color);										//SetPixel을 연속으로 수행 
	void DrawPixelArray(CDC *pDC, int nCount, double *x, double *y, COLORREF color);									//
	void DrawPixelArray_DisplayCoodinate(CDC *pDC, int nCount, long *x, long *y, COLORREF color);						//Display좌표에서 그리기
	void SetCenterCross(bool bShowCenterCross, COLORREF color=RGB(0,255,0), int nTYPE=PS_DOT) { m_bShowCenterCross=bShowCenterCross; m_CenterColor=color; m_nCenterCrossType=nTYPE;}
	void ShowGrayValue(int PosX, int PosY, bool bShow){m_nShowPosX=PosX;m_nShowPosY=PosY;m_bShowGrayValue=bShow;};
	void ShowGrayInfo(bool bShow){m_bShowGrayValue=bShow;};
	void ShowImageName(bool bShow) {m_bShowImageName=bShow;};							//이미지 이름 보임 
	void SetROIAngleDialogEnable(bool bEnable) {m_ROIAngleDialogEnable=bEnable;};		//사용안함. 
	void SetAddGraphic(bool flag){m_bAddGraphic=flag; m_nGraphicCount=0; m_nPixelCnt=0;}
	void ResetGraphic() {m_bAddGraphic=false; m_nGraphicCount=0; m_nPixelCnt=0;}
	//------------------------------------------------------------------


	//Image Control------------------------------------------------------------------------------------------------- 
	void BGRA_to_RGBA(BYTE* data,int width,int height);

	void ClearImage(int Value);
	long PutImageData(LPBYTE fm, int nWidth, int nHeight, int nColor);		//흑백 이미지를 Color 메모리에 넣는 함수 
	long PutImageData(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nColor);
	int SaveROIImage(CString sFileName,  LPBYTE fm, int left, int top, int right, int bottom, int pitch, int BMP_JPG=0);
	
	void UpdateScroll();
	void MoveImagePosition(int nX, int nY);						//(nX, nY)의 위치가 화면의 중심에 오도록 함. 

	int OpenIMG(CString filename);						//BMP,PNG, JPG 파일 Load 
	int SaveIMG(CString filename);						//BMP,PNG, JPG 파일 Save
	int SaveMAP(CString filename, LPBYTE fmSrc);		//BMP,PNG, JPG 파일 Save

	void Global2Local(int x, int y, int *x_, int *y_);			//이미지의 좌표가 Dialog의 좌표로 변환
	void Local2Global(int x, int y, int *x_, int *y_);			//Dialog Client영역의 좌표를 이미지 좌표로 변환  
	void SetColor(COLORREF color) {m_MainColor=color;};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


	void DrawCenterCross(CDC *pDC, COLORREF color, int nTYPE);
	void MakeBitMapHead(int nWidth, int nHeight);					//Display용 Bitmap Head 생성 
	void Pan();

	
	LPBITMAPINFOHEADER m_pBMIH;

    LPVOID  m_lpvColorTable;

	RGBQUAD   m_bmiColors[256];


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedCancel();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
