// CossImage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KoWeb.h"
#include "CossImage.h"
#include "afxdialogex.h"
#include "math.h"

// CCossImage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCossImage, CDialog)

CCossImage::CCossImage(CWnd* pParent /*=NULL*/)
	: CDialog(CCossImage::IDD, pParent)
{
	m_pParent=pParent;

	m_fmProcess=NULL;
	m_pBMIH=NULL;

	m_nCurrentPosX=0;
	m_nCurrentPosY=0;
	m_nCurrentPixel[0]=m_nCurrentPixel[1]=m_nCurrentPixel[2]=0;
	m_MainColor=RGB(0,255,0);
	m_CenterColor=RGB(0,255,0);
	m_sTitle=_T("IMAGE");
	m_sImageName="";
	
	m_fmDisplay=NULL;
	m_fmMask   =NULL;
	m_fmDisplayGray=NULL;
	m_nOrgX=0;
	m_nOrgY=0;
	m_dSx=1.0;
	m_dSy=1.0;
	m_dOldZoomX=m_dOldZoomY=0;

	m_bDrawGraphic			=false;		//내부변수:	이 값이 true인 동안은 Draw 를 Call해도 저장되지 않는다.

	m_ROIAngleDialogEnable	=true;		//ROIANGLE Disable
	m_bShowGrayValue		=true;		//마우스 움직일때 Gray값 보임 
	m_bShowImageName		=true;		//이미지이름 보임 
	m_SizeX=m_SizeY			=0;			//Display Client초기화 	
	m_bflagPAN				=false;		//이 Flag가 셋팅되어 있으면 왼쪽 마우스로 영상이 이동 시킬 수 있음

	m_bShowCenterCross		=false;		//화면 중심에 Center Line 그리기 
	m_nDisplayWidth	=m_nDisplayHeight=0;
	m_dibImage				=NULL;
	m_bZoomCenter			=false;		//true면 Zoom이 바뀔때 이미지가 중심을 기준으로 바뀜 
	m_bAddGraphic			=false;		//true면 Graphic이 쓰고 지워지지 않는다.
	m_bSetFont				=false;		//Font를 Set하면 이후는 Setting된 Font를 사용한다.
	m_bSetMouseZoom			=false;		//True면 마우스 왼쪽은 Zoom IN 마우스 오른쪽은 ZoomOUT
	m_bMouseMoveROI			=false;		//이 Flag가 세팅 되어 있으면 마우스로 Image를 찍으면 ROI가 그 위치로 이동한다.
	m_bUseDCZoom			=true;		//이값이 True면 마우스 Double Click시 1:1줌이 됨.
	m_bUseMousePixel		=false;		//마우스값을 Pixel로 보냄 
	m_bUseRButtonRotation	=false;		//RButton을 Rotation에 사용함 


	fmOutput=NULL;
}

CCossImage::~CCossImage()
{
	if(m_pBMIH)				delete m_pBMIH;
	if(m_fmDisplay)			delete m_fmDisplay;
	if(m_fmMask)            delete m_fmMask;
	if(m_fmDisplayGray)		delete m_fmDisplayGray;
	if(m_dibImage)			delete m_dibImage;
	if(fmOutput)			delete fmOutput;
}

void CCossImage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCossImage, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDCANCEL, &CCossImage::OnBnClickedCancel)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CCossImage 메시지 처리기입니다.
BOOL CCossImage::OnInitDialog()
{
	CDialog::OnInitDialog();

	MakeBitMapHead(m_WIDTH, m_HEIGHT);
	SetWindowText(m_sTitle);

	//PNG color table=======================================================================
	 for (int i=0; i < 256; i++) 
	 {
        m_bmiColors[i].rgbRed = m_bmiColors[i].rgbGreen = m_bmiColors[i].rgbBlue = (BYTE)i;
        m_bmiColors[i].rgbReserved = 0;
	 }
	 m_Saveimage.Create(m_WIDTH,m_HEIGHT,8);
	 m_Saveimage.SetColorTable(0, 256, m_bmiColors);

#ifdef WEB_DAKANO_MULTAE
	 m_SaveMapimage.Create(4096,2048,8);
	 m_SaveMapimage.SetColorTable(0, 256, m_bmiColors);
#endif	
	 //======================================================================================
	 m_bUseRButtonXWheel=false;


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCossImage::DisplayClose()
{
	m_WIDTH=m_HEIGHT=0;

	if(m_pBMIH)			delete m_pBMIH;
	if(m_fmMask)        delete m_fmMask;
	if(m_fmDisplay)		delete m_fmDisplay;
	if(m_fmDisplayGray)	delete m_fmDisplayGray;
	if(m_dibImage)		delete m_dibImage;

	m_pBMIH=NULL;
	m_fmDisplay=NULL;
	m_fmDisplayGray=NULL;
	m_dibImage=NULL;
}

void CCossImage::SetTextFont(bool flag, int fontSize, TCHAR *fontName)
{
	m_bSetFont=flag;
	m_Font.DeleteObject();
	if(flag)
		m_Font.CreatePointFont(fontSize,fontName);

}

long CCossImage::SetImageData(int nWidth, int nHeight)
{
	if(nWidth<=0 || nHeight<=0) return -1;
		
	if(m_WIDTH!=nWidth && m_HEIGHT!=nHeight)
	{
		if(m_fmDisplay) delete m_fmDisplay;
		m_fmDisplay=new BYTE[nWidth*nHeight*IMAGE_COLOR];

		if(m_UseMask)
		{
			if(m_fmMask) delete m_fmMask;
			m_fmMask=new BYTE[nWidth*nHeight];
		}

		if(IMAGE_COLOR==3)
		{
			if(m_fmDisplayGray) delete m_fmDisplayGray;
			m_fmDisplayGray=new BYTE[nWidth*nHeight];
		}
	}

	memset (m_fmDisplay, 0, nWidth*nHeight*IMAGE_COLOR);
	if(m_UseMask) memset (m_fmMask, 0, nWidth*nHeight);

	if(IMAGE_COLOR==3) memset (m_fmDisplayGray, 0, nWidth*nHeight);


	m_WIDTH=nWidth;
	m_HEIGHT=nHeight;

	UpdateScroll();

	return 0;
}


LPBYTE CCossImage::GetImageDataDisplay(int *pitch)	
{
	*pitch=m_WIDTH; 
	if(IMAGE_COLOR==1) return m_fmDisplay;
	else			   return m_fmDisplayGray;
}

void CCossImage::ClearImage(int Value)
{
	memset(m_fmDisplay, Value, m_WIDTH*m_HEIGHT);
	Invalidate(false);
}

long CCossImage::PutImageData(LPBYTE fm, int nWidth, int nHeight, int nColor)
{
	int pitch1, pitch2;
	if(nWidth<=0 || nHeight<=0) return -1;
		
	if(nWidth>m_WIDTH || nHeight>m_HEIGHT)
	{
		AfxMessageBox(_T("Image is over Display Size."));
		return -1;
	}
	int i,j;

	if(nColor==1 || nColor==3) //흑백이미지를 흑백이미지로 칼라는 칼라로 
	{
		if(nWidth==m_WIDTH && nHeight==m_HEIGHT)
		{
			memcpy(m_fmDisplay, fm, m_WIDTH*m_HEIGHT*nColor);
		}
		else if(nWidth>m_WIDTH || nHeight>m_HEIGHT)
		{
			if(m_fmDisplay) delete m_fmDisplay;
			m_fmDisplay=new BYTE[nWidth*nHeight*nColor];
			memset(m_fmDisplay, 0, nWidth*nHeight*nColor);

			if(IMAGE_COLOR==3)
			{
				if(m_fmDisplayGray) delete m_fmDisplayGray;
				m_fmDisplayGray=new BYTE[nWidth*nHeight];
				memset(m_fmDisplayGray, 0, nWidth*nHeight);
			}

			m_WIDTH =nWidth;
			m_HEIGHT=nHeight;

			pitch1=m_WIDTH*IMAGE_COLOR;
			pitch2=nWidth*nColor;
			for(i=0;i<nHeight;i++)
				memcpy(m_fmDisplay+pitch1*i, fm+pitch2*i, pitch2);
		}
		else
		{
			memset(m_fmDisplay, 0, m_HEIGHT*m_WIDTH*nColor);

			pitch1=m_WIDTH*IMAGE_COLOR;
			pitch2=nWidth*nColor;
			for(i=0;i<nHeight;i++)
				memcpy(m_fmDisplay+pitch1*i, fm+pitch2*i, pitch2);
		}
	}
	else	//흑백이미지를 칼라이미지로 
	{
		if(nWidth==m_WIDTH && nHeight==m_HEIGHT)
		{
			for(i=0;i<nHeight;i++)
				for(j=0;j<nWidth;j++)
				{
					*(m_fmDisplay+(nWidth*i+j)*3  )=*(fm+nWidth*i+j);
					*(m_fmDisplay+(nWidth*i+j)*3+1)=*(fm+nWidth*i+j);
					*(m_fmDisplay+(nWidth*i+j)*3+2)=*(fm+nWidth*i+j);
					*(m_fmDisplayGray+nWidth*i+j)  =*(fm+nWidth*i+j);	//GRAY IMAGE
				}
		}
		else if(nWidth>m_WIDTH || nHeight>m_HEIGHT)
		{
			if(m_fmDisplay) delete m_fmDisplay;
			m_fmDisplay=new BYTE[nWidth*nHeight*3];
			memset(m_fmDisplay, 0, nWidth*nHeight*3);

			if(IMAGE_COLOR==3)
			{
				if(m_fmDisplayGray) delete m_fmDisplayGray;
				m_fmDisplayGray=new BYTE[nWidth*nHeight];
				memset(m_fmDisplayGray, 0, nWidth*nHeight);
			}

			m_WIDTH =nWidth;
			m_HEIGHT=nHeight;

			for(i=0;i<nHeight;i++)
				for(j=0;j<nWidth;j++)
				{
					*(m_fmDisplay+(nWidth*i+j)*3  )=*(fm+nWidth*i+j);
					*(m_fmDisplay+(nWidth*i+j)*3+1)=*(fm+nWidth*i+j);
					*(m_fmDisplay+(nWidth*i+j)*3+2)=*(fm+nWidth*i+j);
					*(m_fmDisplayGray+nWidth*i+j)  =*(fm+nWidth*i+j);	//GRAY IMAGE 
				}
		}
		else
		{
			memset(m_fmDisplay, 0, m_HEIGHT*m_WIDTH*3);

			for(i=0;i<nHeight;i++)
				for(j=0;j<nWidth;j++)
				{
					*(m_fmDisplay+(m_WIDTH*i+j)*3  )=*(fm+nWidth*i+j);
					*(m_fmDisplay+(m_WIDTH*i+j)*3+1)=*(fm+nWidth*i+j);
					*(m_fmDisplay+(m_WIDTH*i+j)*3+2)=*(fm+nWidth*i+j);
					*(m_fmDisplayGray+m_WIDTH*i+j)  =*(fm+nWidth*i+j);	//GRAY IMAGE
				}
		}
	}

	Invalidate(false);
	return 0;
}


long CCossImage::PutImageData(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nColor)
{
	int pitch1;
		
	if(left<0 || right>=m_WIDTH || top<0 || bottom>m_HEIGHT)
	{
		AfxMessageBox(_T("Image is over Display Size."));
		return -1;
	}
	int i,j;

	if(nColor==1)		//흑백이미지는 흑백으로
	{
		pitch1=m_WIDTH;
		for(i=top;i<bottom;i++)
			for(j=left;j<right;j++)
				*(m_fmDisplay+pitch1*i+j)=*(fm+pitch*i+j);
	}
	else if(nColor==3)	//칼라는 칼라로 
	{
		for(i=top;i<bottom;i++)
			for(j=left;j<right;j++)
			{
				*(m_fmDisplay+(m_WIDTH*i+j)*3)  =*(fm+(pitch*i+j)*3);
				*(m_fmDisplay+(m_WIDTH*i+j)*3+1)=*(fm+(pitch*i+j)*3+1);
				*(m_fmDisplay+(m_WIDTH*i+j)*3+2)=*(fm+(pitch*i+j)*3+2);
			}
	}
	else			   //흑백이미지를 칼라로  
	{
		pitch1=m_WIDTH*3;
		for(i=top;i<bottom;i++)
			for(j=left;j<right;j++)
			{
				*(m_fmDisplay+(m_WIDTH*i+j)*3)  =*(fm+pitch*i+j);
				*(m_fmDisplay+(m_WIDTH*i+j)*3+1)=*(fm+pitch*i+j);
				*(m_fmDisplay+(m_WIDTH*i+j)*3+2)=*(fm+pitch*i+j);
				*(m_fmDisplayGray+m_WIDTH*i+j)  =*(fm+pitch*i+j);
			}
	}
	

	Invalidate(false);

	return 0;
}



void CCossImage::SetTitle(CString str)
{
	m_sTitle=str;
}

void CCossImage::SetImageName(CString str)
{
	m_sImageName=str;
}

void CCossImage::MakeBitMapHead(int nWidth, int nHeight)
{
	//Bitmap Head 만들기 ------------------------------------------------------------------------
	if(m_pBMIH==NULL)
	{
		m_pBMIH = (LPBITMAPINFOHEADER) new BYTE[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];

		m_pBMIH->biSize = sizeof(BITMAPINFOHEADER);
		m_pBMIH->biWidth=nWidth;   
		m_pBMIH->biHeight=nHeight;
		m_pBMIH->biPlanes = 1;
		if(IMAGE_COLOR==1)
			m_pBMIH->biBitCount = 8;
		else if(IMAGE_COLOR==3)
			m_pBMIH->biBitCount = 32;
		m_pBMIH->biCompression = BI_RGB;
		m_pBMIH->biSizeImage = 0;
		m_pBMIH->biXPelsPerMeter = 0;
		m_pBMIH->biYPelsPerMeter = 0;

		m_pBMIH->biClrUsed = 0;
		m_pBMIH->biClrImportant = 0;

		//-----Color Table작성 ----------------------------------------------------------------------
		m_lpvColorTable = (LPBYTE) m_pBMIH + sizeof(BITMAPINFOHEADER);
		LPBYTE  tmp=(LPBYTE)m_lpvColorTable;

		for(int i=0;i<256;i++)
		{
			*(tmp++)=i;
			*(tmp++)=i;
			*(tmp++)=i;
			*(tmp++)=0;
		}
	}
	//-------------------------------------------------------------------------------------------
}

void CCossImage::OnPaint()
{

	//추가되는 곳------------------------------
	CPaintDC dcORG(this); // device context for painting

	CRect  Rect;
	GetClientRect(&Rect);

	//MEMDC(설정)-------------------------------------------------------
	CDC dc;			   
	CBitmap *pOldbmp;
    	CBitmap *pOldbmpBac;
	CBitmap bitmap;
	dc.CreateCompatibleDC(&dcORG);
	bitmap.CreateCompatibleBitmap(&dcORG,  Rect.Width(), Rect.Height());
	pOldbmp = dc.SelectObject(&bitmap);
	//------------------------------------------------------------------
 
	int nPrevMode;
	int i,left,top,right,bottom;
	CString str;

	m_nShowPosX=4;
	m_nShowPosY=4;


	// IMAGE DISPLAY=================================================================
	ImageDisplay(&dc);



	dc.SetBkMode(TRANSPARENT);
	if(m_bShowCenterCross)
		DrawCenterCross((CDC*)&dc, m_CenterColor, m_nCenterCrossType);
	

	if(m_bShowGrayValue || m_bShowImageName)
	{
		dc.SetTextColor(m_MainColor);

		CString str;
		if(IMAGE_COLOR==1)
		{
			if(m_nCurrentPixel[0]>=0)
			{
				if(m_bShowGrayValue && m_bShowImageName)
					str.Format(_T("%s  (%4d, %4d) %3d"), m_sImageName, m_nCurrentPosX, m_nCurrentPosY, m_nCurrentPixel[0]);
				else if(m_bShowGrayValue)
					str.Format(_T("(%4d, %4d) %3d"), m_nCurrentPosX, m_nCurrentPosY, m_nCurrentPixel[0]);
				else if(m_bShowImageName)
					str.Format(_T("%s"), m_sImageName);
			}
			else
			{
				if(m_bShowGrayValue && m_bShowImageName)
					str.Format(_T("%s  (%4d, %4d) %3d"), m_sImageName, 0,0,0);
				else if(m_bShowGrayValue)
					str.Format(_T("(%4d, %4d) %3d"), 0,0,0);
				else if(m_bShowImageName)
					str.Format(_T("%s"), m_sImageName);
			}
		}
		else
		{
			if(m_nCurrentPixel[0]>=0)
			{
				if(m_bShowGrayValue && m_bShowImageName)
					str.Format(_T("%s  (%4d, %4d) %3d %3d %3d"), m_sImageName, m_nCurrentPosX, m_nCurrentPosY, m_nCurrentPixel[2],m_nCurrentPixel[1],m_nCurrentPixel[0]);
				else if(m_bShowGrayValue)
					str.Format(_T("(%4d, %4d) %3d %3d %3d"), m_nCurrentPosX, m_nCurrentPosY, m_nCurrentPixel[2],m_nCurrentPixel[1],m_nCurrentPixel[0]);
				else if(m_bShowImageName)
					str.Format(_T("%s"), m_sImageName);
			}
			else
			{
				if(m_bShowGrayValue && m_bShowImageName)
					str.Format(_T("%s  (%4d, %4d) %3d %3d %3d"), m_sImageName, 0,0,0,0,0);
				else if(m_bShowGrayValue)
					str.Format(_T("(%4d, %4d) %3d %3d %3d"), 0,0,0,0,0);
				else if(m_bShowImageName)
					str.Format(_T("%s"), m_sImageName);
			}
		}
			
		dc.TextOut(m_nShowPosX, m_nShowPosY, str);
		
		if(m_bShowZoom)
		{
			if(m_dSx==m_dSy)
				str.Format(_T("Zoom : %5.2lf"), m_dSx);
			else
				str.Format(_T("Zoom : %5.2lf, %5.2lf"), m_dSx, m_dSy);

			dc.TextOut(m_nShowPosX+160+(IMAGE_COLOR-1)*30, m_nShowPosY, str);
		}
#if PROGRAM_TYPE==WEB_DAKANO
		if(g_Temp.m_bShowMap)
		{
			int nCol;
			double dX, dY;
			nCol=m_nCurrentPosX/128;
			dX=g_Param.m_dCamStartPosX+32*(m_nCurrentPosX%128)*g_Param.m_dScaleFactorX;
			dY=nCol*2048*g_Param.m_dFrameLen+(m_nCurrentPosY%2048)*g_Param.m_dFrameLen;
			str.Format(_T("X:%d mm  Y:%1.lf M"), (int)dX, dY/1000.);
			dc.TextOut(m_nShowPosX+300+(IMAGE_COLOR-1)*30, m_nShowPosY, str);
		}
#endif
	}

	//Add된 Graphic 그리기 ================================================================================
	if(m_nGraphicCount>0 || m_nPixelCnt>0)
	{
		m_bDrawGraphic=true;
		for(i=0;i<m_nPixelCnt;i++)
			dc.SetPixel(m_nPixelX[i], m_nPixelY[i], m_PixelColor);

		for(i=0;i<m_nGraphicCount;i++)
		{
			if(m_Graphic[i].nType==GRA_TEXT)	//TEXT
			{

				if(m_Graphic[i].fontSize)
				{
					if(m_Graphic[i].nABS==0)
						DrawTextFont(&dc, m_Graphic[i].x1, m_Graphic[i].y1, m_Graphic[i].color, m_Graphic[i].str, m_Graphic[i].fontSize, m_Graphic[i].fontName);
					else
						DrawTextABSFont(&dc, m_Graphic[i].x1, m_Graphic[i].y1, m_Graphic[i].color, m_Graphic[i].str, m_Graphic[i].fontSize, m_Graphic[i].fontName);
				}
				else
				{
					if(m_bSetFont)
					{
						if(m_Graphic[i].nABS==0)
							DrawText(&dc, m_Graphic[i].x1, m_Graphic[i].y1, m_Graphic[i].color, m_Graphic[i].str, &m_Font);
						else
							DrawTextABS(&dc, m_Graphic[i].x1, m_Graphic[i].y1, m_Graphic[i].color, m_Graphic[i].str, &m_Font);
					}
					else
					{
						if(m_Graphic[i].nABS==0)
							DrawText(&dc, m_Graphic[i].x1, m_Graphic[i].y1, m_Graphic[i].color, m_Graphic[i].str);
						else
							DrawTextABS(&dc, m_Graphic[i].x1, m_Graphic[i].y1, m_Graphic[i].color, m_Graphic[i].str);
					}
				}
			}
			else if(m_Graphic[i].nType==GRA_CROSS) //CROSS
				DrawCross(&dc, m_Graphic[i].x1, m_Graphic[i].y1, m_Graphic[i].nWing, m_Graphic[i].color);
			else if(m_Graphic[i].nType==GRA_RECT) //Rectangle
				DrawRectangle(&dc, m_Graphic[i].x1, m_Graphic[i].y1, m_Graphic[i].x2, m_Graphic[i].y2, m_Graphic[i].color);
			else if(m_Graphic[i].nType==GRA_LINE) //LINE
				DrawLine(&dc, m_Graphic[i].x1, m_Graphic[i].y1, m_Graphic[i].x2, m_Graphic[i].y2, m_Graphic[i].color);
			else if(m_Graphic[i].nType==GRA_LINEABS) //LINE
				DrawLineABS(&dc, m_Graphic[i].x1, m_Graphic[i].y1, m_Graphic[i].x2, m_Graphic[i].y2, m_Graphic[i].color);
			else if(m_Graphic[i].nType==GRA_CIRCLE) //CIRCLE
				DrawCircle(&dc, m_Graphic[i].x1, m_Graphic[i].y1, m_Graphic[i].radius, m_Graphic[i].color,m_Graphic[i].style);
			else if(m_Graphic[i].nType==GRA_ANGLERECT) //Rect shape
				DrawRectShape(&dc, m_Graphic[i].pt, m_Graphic[i].color);
 			else if(m_Graphic[i].nType==GRA_ELLIPSE) //Ellipse
				DrawEllipse(&dc, m_Graphic[i].x1, m_Graphic[i].y1, m_Graphic[i].x2, m_Graphic[i].y2, m_Graphic[i].color,m_Graphic[i].style);

		}
		m_bDrawGraphic=false;
	}
	if(m_UseMask && m_nShowMask && m_nShowMask!=254)
	{
		DrawMask(&dc);
	}
	//=====================================================================================================

	//MEMDC(적용)----------------------------------------------------------------------
	dcORG.BitBlt(Rect.left, Rect.top, Rect.Width(), Rect.Height(),&dc, 0,0 , SRCCOPY);
	dc.SelectObject(pOldbmp);
    dc.DeleteDC();
	//---------------------------------------------------------------------------------

	//ROI만 기존대로 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	// 좌표축 변환 ----------------
	PrepareDC((CDC*)&dcORG);		//dc를 사용하는 경우는 Global2Local함수를 사용하면 안된다.

	//ROI 그리기 ------------------
	m_pROI->SetScale(m_dSx, m_dSy);
	m_pROI->DrawROI((CDC*)&dcORG);

	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}

void CCossImage::ImageDisplay(CDC *pDC)
{
	int i,j;
	int left,top,right,bottom;
	int left_,top_,right_,bottom_;
	int nImageWidth, nImageHeight;
	int nGUIWidth, nGUIHeight;
	int width, height;          //화면에 Display하려고하는 폭 
	double dMultix, dMultiy;
	RECT rc;

	if(m_dSx==0) {m_dSx=1; m_dSy=1;}

	int nJump, nX, nY;
	CSize cz;
	LPBYTE fmT, fmS;

	nImageWidth =m_WIDTH*m_dSx;		//Zoom 영상의 최대 크기 X
	nImageHeight=m_HEIGHT*m_dSy;    //Zoom 영상의 최대 크기 Y
	GetClientRect(&rc);
	nGUIWidth =rc.right-rc.left;	//GUI의 최대 크기 X
	nGUIHeight=rc.bottom-rc.top;	//GUI의 최대 크기 Y

	if(nGUIWidth>=nImageWidth && nGUIHeight>=nImageHeight)  //GUI가 충분히 크면 영상을 모두 Display한다 
	{
		left=0; top=0;
		right=nImageWidth;
		bottom=nImageHeight;

		width	=right-left;
		height	=bottom-top;
	}
	else
	{
		left=0;top=0;	
		Local2Global(left, top, &left_, &top_);
		if(left_<0) left_=0;
		if(top_<0) top_=0;

		right=nGUIWidth-1;	
		bottom=nGUIHeight-1;
		Local2Global(right, bottom, &right_, &bottom_);
		if(right_>=m_WIDTH) right_=m_WIDTH-1;
		if(bottom_>=m_HEIGHT) bottom_=m_HEIGHT-1;

		width	=right-left;
		height	=bottom-top;

		left=left_;
		top =top_;
		right=right_;
		bottom=bottom_;
	}

	if(left+width/m_dSx > m_WIDTH)	width=(m_WIDTH-left)*m_dSx;
	if(top+height/m_dSy > m_HEIGHT)  height=(m_HEIGHT-top)*m_dSy;

	if(m_dibImage==NULL || m_nDisplayWidth!=width || m_nDisplayHeight!=height)
	{
		if(width<=0 || height<=0) return;

		m_nDisplayWidth=width;
		if(width%4)
			cz.cx=width+4-width%4;
		else
			cz.cx=width;
		cz.cy=m_nDisplayHeight=height;
	
		m_nDisplayPitch = ((DWORD) cz.cx * 8) / 32;
		if(((DWORD) cz.cx * 8) % 32) 	m_nDisplayPitch++;
		m_nDisplayPitch *= 4;

		if(m_nDisplayPitch>width)
		{
			m_nDisplayPitch-=4;
			m_nDisplayWidth=cz.cx=width=m_nDisplayPitch;
		}

		if(m_dibImage) delete m_dibImage;
		m_dibImage=new CDib(cz,8*IMAGE_COLOR);
	}

	
	dMultix=1/m_dSx;
	dMultiy=1/m_dSy;
	if(IMAGE_COLOR==1)
	{
		fmT=m_dibImage->m_lpImage;
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				*(fmT+m_nDisplayPitch*(m_nDisplayHeight-1-i)+j)=*(m_fmDisplay+m_WIDTH*(int)(top+i*dMultiy)+(int)(left+j*dMultix));
	}
	else if(IMAGE_COLOR==3)
	{
		for(i=0;i<height;i++)
		{
			fmT=m_dibImage->m_lpImage+m_nDisplayPitch*3*(m_nDisplayHeight-1-i);
			fmS=m_fmDisplay+m_WIDTH*3*(int)(top+i*dMultiy);
			for(j=0;j<width;j++)
			{
				*(fmT+3*j  )=*(fmS+(int)(left+j*dMultix)*3);
				*(fmT+3*j+1)=*(fmS+(int)(left+j*dMultix)*3+1);
				*(fmT+3*j+2)=*(fmS+(int)(left+j*dMultix)*3+2);
			}
		}
	}

	if(m_dibImage)
	{
		m_dibImage->UsePalette(pDC);
		m_dibImage->Draw(pDC, CPoint(0,0),CSize(m_nDisplayWidth, m_nDisplayHeight));
	}
}

void CCossImage::DrawText(CDC *pDC, int x, int y, COLORREF color, CString str, CFont *pFont)
{
	CFont *oldFont;

	if(pDC==NULL) pDC=GetDC();

	if(pFont)
	{
		oldFont=pDC->SelectObject(pFont);
	}
	else 
	{
		if(m_bSetFont)
			oldFont=pDC->SelectObject(&m_Font);
	}
	pDC->SetTextColor(color);
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(x,y,str);
	if(pFont || m_bSetFont)
	{
		pDC->SelectObject(oldFont);
	}
	ReleaseDC(pDC);

	if(!m_bDrawGraphic && m_bAddGraphic && m_nGraphicCount<MAX_GRAPHIC)
	{
		m_Graphic[m_nGraphicCount].nType=GRA_TEXT;
		m_Graphic[m_nGraphicCount].nABS=0;
		m_Graphic[m_nGraphicCount].x1=x;
		m_Graphic[m_nGraphicCount].y1=y;
		m_Graphic[m_nGraphicCount].str=str;
		m_Graphic[m_nGraphicCount].color=color;
		m_Graphic[m_nGraphicCount].fontSize=0;
		m_nGraphicCount++;
	}
}

void CCossImage::DrawTextABS(CDC *pDC, int x, int y, COLORREF color, CString str, CFont *pFont)
{
	int x_, y_;
	CFont *oldFont;
	if(pDC==NULL) pDC=GetDC();

	pDC->SetTextColor(color);
	pDC->SetBkMode(TRANSPARENT);

	if(pFont)
	{
		oldFont=pDC->SelectObject(pFont);
	}
	else 
	{
		if(m_bSetFont)
			oldFont=pDC->SelectObject(&m_Font);
	}

	Global2Local(x, y, &x_, &y_);

	pDC->TextOut(x_, y_, str);

	if(pFont || m_bSetFont)
	{
		pDC->SelectObject(oldFont);
	}
	ReleaseDC(pDC);


	if(!m_bDrawGraphic && m_bAddGraphic && m_nGraphicCount<MAX_GRAPHIC)
	{
		m_Graphic[m_nGraphicCount].nType=GRA_TEXT;
		m_Graphic[m_nGraphicCount].nABS=1;
		m_Graphic[m_nGraphicCount].x1=x;
		m_Graphic[m_nGraphicCount].y1=y;
		m_Graphic[m_nGraphicCount].str=str;
		m_Graphic[m_nGraphicCount].color=color;
		m_Graphic[m_nGraphicCount].fontSize=0;
		m_nGraphicCount++;
	}
}

void CCossImage::DrawTextFont(CDC *pDC, int x, int y, COLORREF color, CString str, int fontSize, TCHAR *fontname)
{
	CFont *oldFont;
	CFont font;
	if(pDC==NULL) pDC=GetDC();
	

	if(fontSize)
	{
		font.CreatePointFont(fontSize, fontname);
		oldFont=pDC->SelectObject(&font);
	}
	else 
	{
		if(m_bSetFont)
			oldFont=pDC->SelectObject(&m_Font);
	}

	pDC->SetTextColor(color);
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(x,y,str);
	if(fontSize || m_bSetFont)
	{
		pDC->SelectObject(oldFont);
	}
	ReleaseDC(pDC);

	if(!m_bDrawGraphic && m_bAddGraphic && m_nGraphicCount<MAX_GRAPHIC)
	{
		m_Graphic[m_nGraphicCount].nType=GRA_TEXT;
		m_Graphic[m_nGraphicCount].nABS=0;
		m_Graphic[m_nGraphicCount].x1=x;
		m_Graphic[m_nGraphicCount].y1=y;
		m_Graphic[m_nGraphicCount].str=str;
		m_Graphic[m_nGraphicCount].color=color;
		m_Graphic[m_nGraphicCount].fontSize=fontSize;
		if(_tcslen( fontname)<20)
			wsprintf(m_Graphic[m_nGraphicCount].fontName,_T("%s"), fontname);
		else
			wsprintf(m_Graphic[m_nGraphicCount].fontName, _T("Arial Black"));
		m_nGraphicCount++;
	}
}

void CCossImage::DrawTextABSFont(CDC *pDC, int x, int y, COLORREF color, CString str, int fontSize, TCHAR *fontname)
{
	int x_, y_;
	CFont *oldFont;
	CFont font;
	if(pDC==NULL) pDC=GetDC();
	pDC->SetTextColor(color);
	pDC->SetBkMode(TRANSPARENT);

	if(fontSize)
	{
		font.CreatePointFont(fontSize, fontname);
		oldFont=pDC->SelectObject(&font);
	}
	else 
	{
		if(m_bSetFont)
			oldFont=pDC->SelectObject(&m_Font);
	}

	Global2Local(x, y, &x_, &y_);

	pDC->TextOut(x_, y_, str);

	if(fontSize || m_bSetFont)
	{
		pDC->SelectObject(oldFont);
	}
	ReleaseDC(pDC);


	if(!m_bDrawGraphic && m_bAddGraphic && m_nGraphicCount<MAX_GRAPHIC)
	{
		m_Graphic[m_nGraphicCount].nType=GRA_TEXT;
		m_Graphic[m_nGraphicCount].nABS=1;
		m_Graphic[m_nGraphicCount].x1=x;
		m_Graphic[m_nGraphicCount].y1=y;
		m_Graphic[m_nGraphicCount].str=str;
		m_Graphic[m_nGraphicCount].color=color;
		m_Graphic[m_nGraphicCount].fontSize=fontSize;
		if(_tcslen( fontname)<20)
			wsprintf(m_Graphic[m_nGraphicCount].fontName, _T("%s"), fontname);
		else
			wsprintf(m_Graphic[m_nGraphicCount].fontName, _T("Arial Black"));
		m_nGraphicCount++;
	}

}


void CCossImage::DrawCenterCross(CDC *pDC, COLORREF color, int nTYPE)
{
	int x1,x2,y1,y2;
	int x1_,x2_,y1_,y2_;
	int nCx, nCy;
	CPen newPen(nTYPE, 1, color), *oldPen;


	oldPen=pDC->SelectObject(&newPen);

	nCx=m_WIDTH/2;
	nCy=m_HEIGHT/2;

	x1=0;		y1=nCy*m_dSy;
	x2=m_WIDTH*m_dSx;	y2=nCy*m_dSy;
	pDC->MoveTo(x1, y1);
	pDC->LineTo(x2, y2);

	x1=nCx*m_dSx;		y1=0;
	x2=nCx*m_dSx;		y2=m_HEIGHT*m_dSy;
	pDC->MoveTo(x1, y1);
	pDC->LineTo(x2, y2);

	pDC->SelectObject(oldPen);
}

void CCossImage::MoveImagePosition(int nX, int nY)
{
	if(m_dSx==0 || m_dSy==0) return;
	if(nX<0) nX=0;
	if(nY<0) nY=0;


	if(m_dSx>0.0125 && m_dSy>0.0125)  // (1/80)
	{
		if(m_UseScrollX)
		{
			m_nOrgX=m_SizeX/2-nX*m_dSx;
			if(m_nOrgX>0) m_nOrgX=0;
			SetScrollPos(SB_HORZ, -m_nOrgX);
		}

		if(m_UseScrollY)
		{
			m_nOrgY=m_SizeY/2-nY*m_dSy;
			if(m_nOrgY>0) m_nOrgY=0;
			SetScrollPos( SB_VERT, -m_nOrgY);
		}
	}

	Invalidate(true);
}

//실제이미지의 위치를 Display되는 영역에서의 위치로 환산 
void CCossImage::Global2Local(int x, int y, int *x_, int *y_)
{
	*x_=(int)(x*m_dSx+m_nOrgX+m_dSx+0.5);
	*y_=(int)(y*m_dSy+m_nOrgY+m_dSy+0.5);
}

//Dialog Client영역이 실제 이미지에서 어디인지 구하는 함수 
void CCossImage::Local2Global(int x, int y, int *x_, int *y_)
{
	*x_=(int)((x-m_nOrgX-m_dSx)/m_dSx+0.5);
	*y_=(int)((y-m_nOrgY-m_dSy)/m_dSy+0.5);
}

//--------------------------------------------------------------------------------------------
//
//	Special 한 함수임 (Global 데이타를 사용한다.
//
//--------------------------------------------------------------------------------------------
//새로 한번 그리는 함수 
void  CCossImage::UpdateImage(bool flag)
{
	Invalidate(flag);
}
void  CCossImage::UpdateImageWindow()
{
	UpdateWindow();
}


int CCossImage::PrepareDC(CDC* pDC)
{	
	int nPrevMode=0;
	CSize WinExt,ViewExt;

	WinExt.cx=(int)(m_WIDTH/m_dSx);
	WinExt.cy=(int)(m_HEIGHT/m_dSy);
	ViewExt.cx=m_WIDTH;
	ViewExt.cy=m_HEIGHT;

	pDC->SetWindowExt(WinExt);
	pDC->SetViewportExt(ViewExt);
	pDC->SetViewportOrg(m_nOrgX,m_nOrgY);

	return nPrevMode;
}

void CCossImage::SetPixel(CDC *pDC, int x, int y, COLORREF color)
{
	DrawPoint(pDC, x,y,color);
}


void CCossImage::DrawRectangle(CDC *pDC, int left, int top, int right, int bottom, COLORREF  color)
{
	int left_, top_, right_, bottom_;

	Global2Local(left, top, &left_, &top_);
	Global2Local(right, bottom, &right_, &bottom_);


	if(pDC==NULL) pDC=GetDC();

	CPen *oldPen, newPen(PS_SOLID,1,color);

	oldPen=pDC->SelectObject(&newPen);

	pDC->MoveTo(left_, top_);
	pDC->LineTo(left_, bottom_-1);
	pDC->LineTo(right_-1, bottom_-1);
	pDC->LineTo(right_-1, top_);
	pDC->LineTo(left_, top_);

	pDC->SelectObject(oldPen);
	ReleaseDC(pDC);

	if(!m_bDrawGraphic && m_bAddGraphic && m_nGraphicCount<MAX_GRAPHIC)
	{
		m_Graphic[m_nGraphicCount].nType=GRA_RECT;
		m_Graphic[m_nGraphicCount].x1=left;
		m_Graphic[m_nGraphicCount].y1=top;
		m_Graphic[m_nGraphicCount].x2=right;
		m_Graphic[m_nGraphicCount].y2=bottom;
		m_Graphic[m_nGraphicCount].color=color;
		m_nGraphicCount++;
	}
}

void CCossImage::DrawRectShape(CDC *pDC, POINT *pt, COLORREF  color)
{
	int i, x1_, y1_;

	if(pDC==NULL) pDC=GetDC();
	CPen *oldPen, newPen(PS_SOLID,1,color);

	oldPen=pDC->SelectObject(&newPen);

	Global2Local(pt[0].x, pt[0].y, &x1_, &y1_);
	pDC->MoveTo(x1_, y1_);
	for(i=1;i<4;i++)
	{
		Global2Local(pt[i].x, pt[i].y, &x1_, &y1_);
		pDC->LineTo(x1_, y1_);
	}
	Global2Local(pt[0].x, pt[0].y, &x1_, &y1_);
	pDC->LineTo(x1_, y1_);

	pDC->SelectObject(oldPen);
	ReleaseDC(pDC);

	if(!m_bDrawGraphic && m_bAddGraphic && m_nGraphicCount<MAX_GRAPHIC)
	{
		m_Graphic[m_nGraphicCount].nType=GRA_ANGLERECT;
		m_Graphic[m_nGraphicCount].pt[0]=pt[0];
		m_Graphic[m_nGraphicCount].pt[1]=pt[1];
		m_Graphic[m_nGraphicCount].pt[2]=pt[2];
		m_Graphic[m_nGraphicCount].pt[3]=pt[3];
		m_Graphic[m_nGraphicCount].color=color;
		m_nGraphicCount++;
	}
}

void CCossImage::DrawRectShape(CDC *pDC, TPoint *pt, COLORREF  color)
{
	int i, x1_, y1_;

	if(pDC==NULL) pDC=GetDC();
	CPen *oldPen, newPen(PS_SOLID,1,color);

	oldPen=pDC->SelectObject(&newPen);

	Global2Local(pt[0].x, pt[0].y, &x1_, &y1_);
	pDC->MoveTo(x1_, y1_);
	for(i=1;i<4;i++)
	{
		Global2Local(pt[i].x, pt[i].y, &x1_, &y1_);
		pDC->LineTo(x1_, y1_);
	}
	Global2Local(pt[0].x, pt[0].y, &x1_, &y1_);
	pDC->LineTo(x1_, y1_);

	pDC->SelectObject(oldPen);
	ReleaseDC(pDC);

	if(!m_bDrawGraphic && m_bAddGraphic && m_nGraphicCount<MAX_GRAPHIC)
	{
		m_Graphic[m_nGraphicCount].nType=GRA_ANGLERECT;
		for(i=0;i<4;i++)
		{
			m_Graphic[m_nGraphicCount].pt[i].x=pt[i].x;
			m_Graphic[m_nGraphicCount].pt[i].y=pt[i].y;
		}
		m_Graphic[m_nGraphicCount].color=color;
		m_nGraphicCount++;
	}
}
void CCossImage::DrawLine(CDC *pDC, int x1, int y1, int x2, int y2, COLORREF  color)
{
	int x1_, y1_, x2_, y2_;

	Global2Local(x1, y1, &x1_, &y1_);
	Global2Local(x2, y2, &x2_, &y2_);


	if(pDC==NULL) pDC=GetDC();

	CPen *oldPen, newPen(PS_SOLID,1,color);

	oldPen=pDC->SelectObject(&newPen);

	pDC->MoveTo(x1_, y1_);
	pDC->LineTo(x2_, y2_);
	

	pDC->SelectObject(oldPen);
	ReleaseDC(pDC);

	if(!m_bDrawGraphic && m_bAddGraphic && m_nGraphicCount<MAX_GRAPHIC)
	{
		m_Graphic[m_nGraphicCount].nType=GRA_LINE;
		m_Graphic[m_nGraphicCount].x1=x1;
		m_Graphic[m_nGraphicCount].y1=y1;
		m_Graphic[m_nGraphicCount].x2=x2;
		m_Graphic[m_nGraphicCount].y2=y2;
		m_Graphic[m_nGraphicCount].color=color;
		m_nGraphicCount++;
	}
}
void CCossImage::DrawLineABS(CDC *pDC, int x1, int y1, int x2, int y2, COLORREF  color)
{
	if(pDC==NULL) pDC=GetDC();

	CPen *oldPen, newPen(PS_SOLID,1,color);

	oldPen=pDC->SelectObject(&newPen);

	pDC->MoveTo(x1, y1);
	pDC->LineTo(x2, y2);
	
	pDC->SelectObject(oldPen);
	ReleaseDC(pDC);

	if(!m_bDrawGraphic && m_bAddGraphic && m_nGraphicCount<MAX_GRAPHIC)
	{
		m_Graphic[m_nGraphicCount].nType=GRA_LINEABS;
		m_Graphic[m_nGraphicCount].x1=x1;
		m_Graphic[m_nGraphicCount].y1=y1;
		m_Graphic[m_nGraphicCount].x2=x2;
		m_Graphic[m_nGraphicCount].y2=y2;
		m_Graphic[m_nGraphicCount].color=color;
		m_nGraphicCount++;
	}
}

void CCossImage::DrawCircle(CDC *pDC, int cx, int cy, int radius, COLORREF  color, int style)
{
	int left, top, right, bottom;
	int left_, top_, right_, bottom_;

	left	=cx-radius;
	right	=cx+radius;
	top		=cy-radius;
	bottom	=cy+radius;

	Global2Local(left, top, &left_, &top_);
	Global2Local(right, bottom, &right_, &bottom_);


	if(pDC==NULL) pDC=GetDC();
	CPen *oldPen, newPen(style,1,color);

	oldPen=pDC->SelectObject(&newPen);
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->SetBkMode(TRANSPARENT);

	pDC->Ellipse(CRect(left_, top_, right_, bottom_));

	pDC->SelectObject(oldPen);
	ReleaseDC(pDC);

	if(!m_bDrawGraphic && m_bAddGraphic && m_nGraphicCount<MAX_GRAPHIC)
	{
		m_Graphic[m_nGraphicCount].nType=GRA_CIRCLE;
		m_Graphic[m_nGraphicCount].x1=cx;
		m_Graphic[m_nGraphicCount].y1=cy;
		m_Graphic[m_nGraphicCount].radius=radius;
		m_Graphic[m_nGraphicCount].color=color;
		m_Graphic[m_nGraphicCount].style=style;
		m_nGraphicCount++;
	}
}

void CCossImage::DrawMask(CDC *pDC)
{
	int i,j;
	int x, y, x1, y1, x2, y2;
	int endX, endY;
	int nOffsetX, nOffsetY, N=0;
	COLORREF color[8];//=RGB(255,0,0);

	color[1]=RGB(255,0,0);		//빨강
	color[2]=RGB(255,127,39);	//주황
	color[3]=RGB(255,255,0);	//노랑
	color[4]=RGB(0,255,0);		//초
	color[5]=RGB(0,128,255);	//파
	color[6]=RGB(0,0,255);		//남
	color[7]=RGB(0,255,255);	//DUST	


	Local2Global(0, 0, &x1, &y1);
	Local2Global(1024, 1024, &endX, &endY);
	Local2Global(3, 3, &x2, &y2);
	nOffsetX=x2-x1;
	nOffsetY=y2-y1;
	if(nOffsetX<=0) nOffsetX=1;
	if(nOffsetY<=0) nOffsetY=1;


	if(y1<0)            y1=0;
	if(endY>m_HEIGHT) endY=m_HEIGHT;
	if(x1<0)            x1=0;
	if(endX>m_WIDTH)  endX=m_WIDTH;

	for(i=y1;i<endY;i+=nOffsetY)
	{
		y=i*m_dSy+m_nOrgY;
		for(j=x1;j<endX;j+=nOffsetX)
		{
			N=*(m_fmMask+m_WIDTH*i+j);
			if(N>7) continue;
	
			if(m_nShowMask==255)
			{				
				if(N)
				{
					x=j*m_dSx+m_nOrgX;				
					pDC->SetPixel(x,y,color[N]);
								//if(N>6) pDC->SetPixel(x+1,y,color[N-6]);
				}
			}
			else
			{
				if(N>0 && N==m_nShowMask)
				{
					x=j*m_dSx+m_nOrgX;
					pDC->SetPixel(x,y,color[N]);
								//if(N>6) pDC->SetPixel(x+1,y,color[N-6]);
				}
			}
		}
	}
}

void CCossImage::DrawEllipse(CDC *pDC, int left, int top, int right, int bottom, COLORREF  color, int style)
{
	int left_, top_, right_, bottom_;


	Global2Local(left, top, &left_, &top_);
	Global2Local(right, bottom, &right_, &bottom_);


	if(pDC==NULL) pDC=GetDC();

	CPen *oldPen, newPen(style,1,color);

	oldPen=pDC->SelectObject(&newPen);
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->SetBkMode(TRANSPARENT);

	pDC->Ellipse(CRect(left_, top_, right_, bottom_));

	pDC->SelectObject(oldPen);
	ReleaseDC(pDC);

	if(!m_bDrawGraphic && m_bAddGraphic && m_nGraphicCount<MAX_GRAPHIC)
	{
		m_Graphic[m_nGraphicCount].nType=GRA_ELLIPSE;
		m_Graphic[m_nGraphicCount].x1=left;
		m_Graphic[m_nGraphicCount].y1=top;
		m_Graphic[m_nGraphicCount].x2=right;
		m_Graphic[m_nGraphicCount].y2=bottom;
		m_Graphic[m_nGraphicCount].color=color;
		m_Graphic[m_nGraphicCount].style=style;
		m_nGraphicCount++;
	}
}

void CCossImage::DrawCross(CDC *pDC, int x, int y, int nWing, COLORREF  color)
{
	int x_,y_;

	Global2Local(x, y, &x_, &y_);
//	m_pROI->ChangePointPosition(x, y, &x_, &y_);

	if(pDC==NULL) pDC=GetDC();
	CPen *oldPen, newPen(PS_SOLID,1,color);

	oldPen=pDC->SelectObject(&newPen);

	pDC->MoveTo(x_-nWing, y_);
	pDC->LineTo(x_+nWing+1, y_);
	pDC->MoveTo(x_, y_-nWing);
	pDC->LineTo(x_, y_+nWing+1);

	pDC->SelectObject(oldPen);
	ReleaseDC(pDC);

	if(!m_bDrawGraphic && m_bAddGraphic && m_nGraphicCount<MAX_GRAPHIC)
	{
		m_Graphic[m_nGraphicCount].nType=GRA_CROSS;
		m_Graphic[m_nGraphicCount].x1=x;
		m_Graphic[m_nGraphicCount].y1=y;
		m_Graphic[m_nGraphicCount].nWing=nWing;
		m_Graphic[m_nGraphicCount].color=color;
		m_nGraphicCount++;
	}
}



void CCossImage::DrawPixelArray(CDC *pDC, int nCount, long *x, long *y, COLORREF color)
{
	int i;
	int x_,y_;
	if(pDC==NULL) pDC=GetDC();


	for(i=0;i<nCount;i++)
	{
		Global2Local(x[i], y[i], &x_, &y_);
		pDC->SetPixel((int)(x_+0.5),(int)(y_+0.5),color);
	}

	ReleaseDC(pDC);
}

void CCossImage::DrawPixelArray_DisplayCoodinate(CDC *pDC, int nCount, long *x, long *y, COLORREF color)
{
	int i;
	if(pDC==NULL) pDC=GetDC();

	m_nPixelCnt =nCount;
	m_PixelColor=color;
	for(i=0;i<nCount;i++)
	{
		pDC->SetPixel(x[i],y[i],color);
		m_nPixelX[i]=x[i];
		m_nPixelY[i]=y[i];
	}

	ReleaseDC(pDC);
}

void CCossImage::DrawPixelArray(CDC *pDC, int nCount, double *x, double *y, COLORREF color)
{
	int i;
	int x_,y_;
	if(pDC==NULL) pDC=GetDC();


	for(i=0;i<nCount;i++)
	{
		Global2Local(x[i], y[i], &x_, &y_);
		pDC->SetPixel((int)(x_+0.5),(int)(y_+0.5),color);
	}

	ReleaseDC(pDC);
}

void CCossImage::DrawPoint(CDC *pDC, int x, int y, COLORREF  color)
{
	int x_,y_;
	if(pDC==NULL) pDC=GetDC();


	Global2Local(x, y, &x_, &y_);
	
	pDC->SetPixel(x_,y_,color);
	
	ReleaseDC(pDC);
}


void CCossImage::Pan()
{
	int nOrgX, nOrgY;
	nOrgX=m_nOrgX;
	nOrgY=m_nOrgY;

//	if(-GetScrollPos(SB_VERT)-m_nOrgY<0)	m_bRefresh=true;
	m_nOrgY=-GetScrollPos(SB_VERT);
//	if(-GetScrollPos(SB_HORZ)-m_nOrgX<0 )	m_bRefresh=true;
	m_nOrgX=-GetScrollPos(SB_HORZ);

	if(nOrgX!=m_nOrgX || nOrgY!=m_nOrgY)
	{
		Invalidate(false);
	}
}
long CCossImage::FitZoom()
{
	double dSH, dSV;

	m_bFitZoom=true;

	dSH=(double)m_SizeX/m_WIDTH;
	dSV=(double)m_SizeY/m_HEIGHT;
	if(dSH<dSV) Zoom(dSH, dSH);
	else		Zoom(dSV, dSV);

	m_dOldZoomX=m_dOldZoomY=0;

	return 0;
}
long CCossImage::FitZoomXY()
{
	double dSH, dSV;

	m_bFitZoom=true;

	dSH=(double)m_SizeX/m_WIDTH;
	dSV=(double)m_SizeY/m_HEIGHT;
	Zoom(dSH, dSV);

	m_dOldZoomX=m_dOldZoomY=0;

	return 0;
}
long CCossImage::Zoom(double dSx, double dSy)
{
	CRect rc;
	int nHor_Data;
	int nVer_Data;
	int nPrevPosX, nPrevPosY;


	if(!m_bZoomCenter)
	{   //Zoom 변경전 화면의 중심의 Image 위치를 기억하고 있다가 Zoom 변경후 다시 찾아간다.
		Local2Global(m_nDisplayWidth/2,m_nDisplayHeight/2,&nPrevPosX,&nPrevPosY);
	}

	if((m_dSx!=dSx || m_dSy!=dSy) && m_dibImage)
	{
		delete m_dibImage;
		m_dibImage=NULL;
	}
	m_dSx=dSx;
	m_dSy=dSy;


	//-----------Scroll Setting하는 정확한 방법임--------------
	UpdateScroll();	
	//---------------------------------------------------------

	if(m_bZoomCenter)
	{
		MoveImagePosition(m_WIDTH/2, m_HEIGHT/2);
	}
	else
	{
		MoveImagePosition(nPrevPosX, nPrevPosY);
	}
	Pan(); //없애면 안됨.

	Invalidate(false);

	return 0;
}
double CCossImage::GetZoom()
{
	return m_dSx;
}
double CCossImage::GetZoomX()
{
	return m_dSx;
}
double CCossImage::GetZoomY()
{
	return m_dSy;
}

void CCossImage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnTimer(nIDEvent);
}


void CCossImage::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnClose();
}





BOOL CCossImage::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	     return TRUE;
//	return CDialog::OnEraseBkgnd(pDC);
}






void CCossImage::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int nStepY=250;

	switch(nSBCode)
	{
		case SB_LINELEFT: // 왼쪽 끝의 버튼을 누른경우
     			SetScrollPos(SB_VERT,(int)(GetScrollPos(SB_VERT)-1)); break;
		case SB_LINERIGHT: // 오른쪽 끝을 누른경우
     			SetScrollPos(SB_VERT,(int)(GetScrollPos(SB_VERT)+1)); break;
		case SB_PAGELEFT: // 왼쪽 몸통부분을 누른경우
     			SetScrollPos(SB_VERT,GetScrollPos(SB_VERT)-nStepY); break;
		case SB_PAGERIGHT: // 오른쪽 몸통부분을 누른경우
     			SetScrollPos(SB_VERT,GetScrollPos(SB_VERT)+nStepY); break;
		case SB_THUMBTRACK: // 절대적인 위치로 스크롤된경우
     			SetScrollPos(SB_VERT,nPos); break;

	}
	Pan();

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CCossImage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nStepX=100;

	switch(nSBCode)
	{
		case SB_LINELEFT: // 왼쪽 끝의 버튼을 누른경우
     			SetScrollPos(SB_HORZ,(int)(GetScrollPos(SB_HORZ)-1)); break;
		case SB_LINERIGHT: // 오른쪽 끝을 누른경우
     			SetScrollPos(SB_HORZ,(int)(GetScrollPos(SB_HORZ)+1)); break;
		case SB_PAGELEFT: // 왼쪽 몸통부분을 누른경우
     			SetScrollPos(SB_HORZ,GetScrollPos(SB_HORZ)-nStepX); break;
		case SB_PAGERIGHT: // 오른쪽 몸통부분을 누른경우
     			SetScrollPos(SB_HORZ,GetScrollPos(SB_HORZ)+nStepX); break;
		case SB_THUMBTRACK: // 절대적인 위치로 스크롤된경우
     			SetScrollPos(SB_HORZ,nPos); break;

	}
	
	Pan();

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CCossImage::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPoint pt;
	int x, y, newX, newY;
	double dZoomX, dZoomY;

	if(m_bflagPAN)							//영상을 이동시킴 
	{
		m_nPanX1=point.x;
		m_nPanY1=point.y;
		m_bLButtonDown=true;
	}
	else if(m_bMouseMoveROI)				//이 Flag가 세팅 되어 있으면 마우스로 Image를 찍으면 ROI가 그 위치로 이동한다.
	{
		int left,top,right,bottom;
		int nBaseX, nBaseY;
		Local2Global(point.x, point.y, &newX, &newY);
		m_pROI->GetROIRegion(m_MouseTrackerName, &left, &top, &right, &bottom);
		nBaseX=(left+right)/2;
		nBaseY=(top+bottom)/2;
		left	+=(newX-nBaseX);	if(left<0)			left=0;
		right	+=(newX-nBaseX);	if(right>=m_WIDTH)	right=m_WIDTH-1;
		top		+=(newY-nBaseY);	if(top<0)			top=0;
		bottom	+=(newY-nBaseY);	if(bottom>=m_HEIGHT)bottom=m_HEIGHT-1;
		m_pROI->SetROIRect(m_MouseTrackerName, left, top, right, bottom);
	}
	else if(m_bSetMouseZoom)
	{	
		dZoomX=GetZoomX();
		dZoomY=GetZoomY();
		Local2Global(point.x, point.y, &newX, &newY);
		Zoom(dZoomX+1./8., dZoomY+1./8.);
		MoveImagePosition(newX, newY);
	}
	else
	{
		//==============KROI=============================
		pt.x=(point.x-m_nOrgX)/m_dSx;
		pt.y=(point.y-m_nOrgY)/m_dSy;

		if(m_pROI->LButtonDown(nFlags, pt, this)>=0)
			Invalidate(false);
		//===============================================
	}


	//MOUSE POINT 메인에 전달 ------------------------------------------------------------
	DWORD pos;
	if(m_bUseMousePixel)
	{
		POINT m_pt;	
		BYTE  m_Gray;
		Local2Global(point.x, point.y, &newX, &newY);

		pos=newY;
		pos=pos<<16;
		pos+=newX;
	}
	else
	{
		pos=point.y;
		pos=pos<<16;
		pos+=point.x;
	}
	::SendMessage(m_pParent->m_hWnd, WM_LBUTTONDOWN,  (DWORD)(100+m_nDisplayID), pos);
	//------------------------------------------------------------------------------------

	CDialog::OnLButtonDown(nFlags, point);
}


void CCossImage::OnLButtonUp(UINT nFlags, CPoint point)
{
	CPoint pt;
	int newX, newY;

	m_bLButtonDown=false;

	if(m_pROI->m_IsROIClicked)
	{
		//==============KROI=============================
		pt.x=(point.x-m_nOrgX)/m_dSx;
		pt.y=(point.y-m_nOrgY)/m_dSy;

		m_pROI->OnLButtonUp(nFlags, pt, this);
		//===============================================
		Invalidate(false);
	}

	//MOUSE POINT 메인에 전달 ------------------------------------------------------------
	DWORD pos;
	if(m_bUseMousePixel)
	{
		POINT m_pt;	
		BYTE  m_Gray;
		Local2Global(point.x, point.y, &newX, &newY);

		pos=newY;
		pos=pos<<16;
		pos+=newX;
	}
	else
	{
		pos=point.y;
		pos=pos<<16;
		pos+=point.x;
	}
	::SendMessage(m_pParent->m_hWnd, WM_LBUTTONUP,  (DWORD)(100+m_nDisplayID), pos);
	//------------------------------------------------------------------------------------

	CDialog::OnLButtonUp(nFlags, point);
}


void CCossImage::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int newX, newY;
	double dZoomX,dZoomY;

	if(m_bUseDCZoom)
	{
		dZoomX=GetZoomX();
		dZoomY=GetZoomY();
		if(dZoomX!=1)
		{
			m_dOldZoomX=dZoomX;
			m_dOldZoomY=dZoomY;
			Local2Global(point.x, point.y, &newX, &newY);
			Zoom(1, 1);
			MoveImagePosition(newX, newY);

			Global2Local(m_WIDTH, m_HEIGHT, &newX, &newY);
			if(newX<m_SizeX) m_nOrgX+=m_SizeX/2;
			if(newY<m_SizeY) m_nOrgY+=m_SizeY/2;
			Invalidate(false);
			Pan();
		}
		else if(m_dOldZoomX>0.001)
		{
			Local2Global(point.x, point.y, &newX, &newY);

			Zoom(m_dOldZoomX, m_dOldZoomY);
			MoveImagePosition(newX, newY);

			Global2Local(m_WIDTH, m_HEIGHT, &newX, &newY);
			if(newX<m_SizeX) m_nOrgX=0;
			if(newY<m_SizeY) m_nOrgY=0;
			Invalidate(false);
		}
	}

	//MOUSE POINT 메인에 전달 ------------------------------------------------------------
	DWORD pos;
	if(m_bUseMousePixel)
	{
		POINT m_pt;	
		BYTE  m_Gray;
		Local2Global(point.x, point.y, &newX, &newY);

		pos=newY;
		pos=pos<<16;
		pos+=newX;
	}
	else
	{
		pos=point.y;
		pos=pos<<16;
		pos+=point.x;
	}
	::SendMessage(m_pParent->m_hWnd, WM_LBUTTONDBLCLK,  (DWORD)(100+m_nDisplayID), pos);
	//------------------------------------------------------------------------------------	

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CCossImage::OnMouseMove(UINT nFlags, CPoint point)
{
	CString str;
	CPoint pt;
	int newX, newY;
	int nCurrentPosX, nCurrentPosY;

// Title Bar에 정보 Display
	m_nCurrentPosX=(point.x-m_nOrgX)/m_dSx;
	m_nCurrentPosY=(point.y-m_nOrgY)/m_dSy;
	m_nCurrentPixel[0]=-1;
	nCurrentPosX=m_nCurrentPosX; if(nCurrentPosX<0) nCurrentPosX=0;
	nCurrentPosY=m_nCurrentPosY; if(nCurrentPosY<0) nCurrentPosY=0;
	if(m_nCurrentPosX<0 || m_nCurrentPosY<0) return;
	if(m_nCurrentPosX<m_WIDTH && m_nCurrentPosY<m_HEIGHT)
	{
		m_nCurrentPixel[0]=*(m_fmDisplay+nCurrentPosY*m_WIDTH*IMAGE_COLOR+nCurrentPosX*IMAGE_COLOR);
		if(IMAGE_COLOR==3)
		{
			m_nCurrentPixel[1]=*(m_fmDisplay+nCurrentPosY*m_WIDTH*IMAGE_COLOR+nCurrentPosX*IMAGE_COLOR+1);
			m_nCurrentPixel[2]=*(m_fmDisplay+nCurrentPosY*m_WIDTH*IMAGE_COLOR+nCurrentPosX*IMAGE_COLOR+2);
		}
	}


	if(m_bflagPAN && m_bLButtonDown)							//영상을 이동시킴 
	{
		if( (point.x-m_nPanX1)>0 || (m_SizeX-(m_nOrgX+(point.x-m_nPanX1)))/m_dSx<m_WIDTH )
		{
			m_nOrgX+=(point.x-m_nPanX1); 
			if(m_nOrgX>0)			 m_nOrgX=0; 
			SetScrollPos(SB_HORZ, -m_nOrgX);
		}
		if( (point.y-m_nPanY1)>0 || (m_SizeY-(m_nOrgY+(point.y-m_nPanY1)))/m_dSy<m_HEIGHT)
		{
			m_nOrgY+=(point.y-m_nPanY1); 
			if(m_nOrgY>0)			  m_nOrgY=0; 
			SetScrollPos(SB_VERT, -m_nOrgY);
		}
		Invalidate(false);
		m_nPanX1=point.x;
		m_nPanY1=point.y;
	}


	//==============KROI=============================

	pt.x=(point.x-m_nOrgX)/m_dSx;
	pt.y=(point.y-m_nOrgY)/m_dSy;

	if(m_pROI->OnMouseMove(nFlags, pt, this))
	{
		Invalidate(false);
	}
	else
	{
		if(m_nCurrentPixel[0]>=0) InvalidateRect(CRect(m_nShowPosX,m_nShowPosY,m_nShowPosX+650,m_nShowPosY+20),false);
	}
	//==========================================


	//MOUSE POINT 메인에 전달 ------------------------------------------------------------
	DWORD pos;
	if(m_bUseMousePixel)
	{
		POINT m_pt;	
		BYTE  m_Gray;
		Local2Global(point.x, point.y, &newX, &newY);

		pos=newY;
		pos=pos<<16;
		pos+=newX;
	}
	else
	{
		pos=point.y;
		pos=pos<<16;
		pos+=point.x;
	}
	::SendMessage(m_pParent->m_hWnd, WM_MOUSEMOVE,  (DWORD)(100+m_nDisplayID), pos);
	//------------------------------------------------------------------------------------

	CDialog::OnMouseMove(nFlags, point);
}


void CCossImage::OnRButtonDown(UINT nFlags, CPoint point)
{
	CPoint pt;
	int nROINumber;
	int newX, newY;
	double dZoomX, dZoomY;
	m_bUseRButtonXWheel = true;
	if(m_bUseRButtonRotation)
	{
/*		ROIAngle.cpp, h를 사용할때만 살린다.
		//==============KROI=============================
		pt.x=(point.x-m_nOrgX)/m_dSx;
		pt.y=(point.y-m_nOrgY)/m_dSy;

		nROINumber=m_pROI->GetActiveNumber(pt);
		if(nROINumber>=0)
		{	
			CROIAngle dlg(this);

			dlg.m_pROI=m_pROI;

			dlg.m_nROINumber=nROINumber;
			dlg.DoModal();
		}	
		
		Invalidate(false);
		//===============================================
*/
	}
	else if(m_bSetMouseZoom)
	{
		dZoomX=GetZoomX();
		dZoomY=GetZoomY();
		if(dZoomX-1.0/8.0>1.0/8.0-0.0001 && dZoomY-1.0/8.0>1.0/8.0-0.0001)
			Zoom(dZoomX-1./8., dZoomY-1./8.);
	}


	//MOUSE POINT 메인에 전달 ------------------------------------------------------------
	DWORD pos;
	if(m_bUseMousePixel)
	{
		POINT m_pt;	
		BYTE  m_Gray;
		Local2Global(point.x, point.y, &newX, &newY);

		pos=newY;
		pos=pos<<16;
		pos+=newX;
	}
	else
	{
		pos=point.y;
		pos=pos<<16;
		pos+=point.x;
	}
	::SendMessage(m_pParent->m_hWnd, WM_RBUTTONDOWN,  (DWORD)(100+m_nDisplayID), pos);
	//------------------------------------------------------------------------------------
	

	CDialog::OnRButtonDown(nFlags, point);
}


void CCossImage::OnRButtonUp(UINT nFlags, CPoint point)
{
	int newX, newY;
	m_bUseRButtonXWheel = false;
	//MOUSE POINT 메인에 전달 ------------------------------------------------------------
	DWORD pos;
	if(m_bUseMousePixel)
	{
		POINT m_pt;	
		BYTE  m_Gray;
		Local2Global(point.x, point.y, &newX, &newY);

		pos=newY;
		pos=pos<<16;
		pos+=newX;
	}
	else
	{
		pos=point.y;
		pos=pos<<16;
		pos+=point.x;
	}
	::SendMessage(m_pParent->m_hWnd, WM_RBUTTONUP,  (DWORD)(100+m_nDisplayID), pos);
	//------------------------------------------------------------------------------------

	CDialog::OnRButtonUp(nFlags, point);
}





void CCossImage::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	m_SizeX=cx;
	m_SizeY=cy;		
	Zoom(m_dSx, m_dSy);
}

#define SCROLL_WIDTH	17
void CCossImage::UpdateScroll()
{
	if(m_WIDTH*m_dSx-m_SizeX>SCROLL_WIDTH)
	{
		SetScrollRange( SB_HORZ   , 0, m_WIDTH*m_dSx-m_SizeX,  TRUE );
		m_UseScrollX=true;
	}
	else
	{
		SetScrollRange( SB_HORZ   , 0, 0,  FALSE );
		m_UseScrollX=false;
		m_nOrgX=0;
	}

	if(m_HEIGHT*m_dSy-m_SizeY>SCROLL_WIDTH)
	{
		SetScrollRange( SB_VERT   , 0, m_HEIGHT*m_dSy-m_SizeY,  TRUE );
		m_UseScrollY=true;
	}
	else
	{
		SetScrollRange( SB_VERT   , 0, 0,  FALSE );
		m_UseScrollY=false;
		m_nOrgY=0;
	}
}

int CCossImage::MakeColor2GrayAll()
{
	int i,j, ret;
	if(m_fmDisplay==NULL || m_fmDisplayGray==NULL) return -1;

	for(i=0;i<m_HEIGHT;i++)
		for(j=0;j<m_WIDTH;j++)
		{
			*(m_fmDisplayGray+m_WIDTH*i+j)=(*(m_fmDisplay+3*(m_WIDTH*i+j))+*(m_fmDisplay+3*(m_WIDTH*i+j)+1)+*(m_fmDisplay+3*(m_WIDTH*i+j+2)))/3;
		}

	return 0;	
}
int CCossImage::MakeColor2Gray(LPBYTE fmGray, LPBYTE fmColor, int left, int top, int right, int bottom, int pitch)
{
	int i,j, ret;
	if(fmGray==NULL || fmColor==NULL) return -1;

	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
		{
			*(fmGray+pitch*i+j)=(*(fmColor+3*(pitch*i+j))+*(fmColor+3*(pitch*i+j)+1)+*(fmColor+3*(pitch*i+j+2)))/3;
		}

	return 0;	
}

int CCossImage::SaveROIImage(CString sFileName, LPBYTE fm, int left, int top, int right, int bottom, int pitch, int BMP_JPG)
{
	int i,j, ret;
	CFile f;
	LPBYTE fmD;
	CDib *pdib;
	int nWidth=right-left+1;
	int nHeight=bottom-top+1;
	int pitchJpeg, nByte;
	int pitch_;


	if(BMP_JPG==0) //BMP이면 
	{
		CSize cz(nWidth, nHeight);	
		pdib=new CDib(cz,8);	

		nByte=pdib->m_lpBMIH->biBitCount/8;
		pitch_ = ((DWORD) pdib->m_lpBMIH->biWidth*nByte * 8) / 32;
		if(((DWORD) pdib->m_lpBMIH->biWidth*nByte * 8) % 32) 	pitch_++;
		pitch_ *= 4;

		for(i=top;i<top+nHeight;i++)
			for(j=left;j<left+nWidth*IMAGE_COLOR;j++)
				*(pdib->m_lpImage+pitch_*IMAGE_COLOR*(nHeight-1-(i-top))+j-(left))=*(fm+pitch*IMAGE_COLOR*i+j);

		ret=f.Open(sFileName, CFile::modeCreate|CFile::modeWrite);
		if(ret)
		{
			pdib->Write(&f);	
			f.Close();	
		}

		delete pdib;
	}
	else
	{
		pitchJpeg=nWidth*3+ nWidth%4;
		fmD=new BYTE[pitchJpeg*nHeight];
		if(IMAGE_COLOR==1)
		{
			for(i=top;i<bottom;i++)
				for(j=left*3;j<(left+nWidth)*3;j++)
					*(fmD+pitchJpeg*(i-top)+j-left*3)=*(fm+pitch*i+j/3);
		}
		else if(IMAGE_COLOR==3)
		{
			for(i=top;i<bottom;i++)
				for(j=left*3;j<(left+nWidth)*3;j++)
					*(fmD+pitchJpeg*(i-top)+j-left*3)=*(fm+3*pitch*i+j);
		}

		delete fmD;
	}

	return !ret;
}


void CCossImage::BGRA_to_RGBA(BYTE* data,int width,int height)
{
/*  int   i;
  int   j;
  int   pad;
  int   line_width;
  BYTE  r, g, b, a;
  BYTE* ptr;

  ptr = data;
  pad = IJL_DIB_PAD_BYTES(width,4);
  line_width = width * 4 + pad;

  for(i = 0; i < height; i++)
  {
    ptr = data + line_width*i;
    for(j = 0; j < width; j++)
    {
      b = ptr[0];
      g = ptr[1];
      r = ptr[2];
      a = ptr[3];
      ptr[0] = r;
      ptr[1] = g;
      ptr[2] = b;
      ptr += 4;
    }
  }
*/
}
void  CCossImage::GetImagePart(LPBYTE fmD, int nX, int nY, int nSize)
{
	int i,j;
	int left,top,right,bottom;

	left=nX-nSize/2;  if(left<0) left=0;
	right=left+nSize; if(right>=m_WIDTH) { right=m_WIDTH-1; left=right-nSize;}
	top=nY-nSize/2;  if(top<0) top=0;
	bottom=top+nSize; if(bottom>=m_HEIGHT) { bottom=m_HEIGHT-1; top=bottom-nSize;}

	for(i=top;i<bottom;i++)
		memcpy(fmD+nSize*(i-top), m_fmDisplay+m_WIDTH*i+j, nSize);
}
void CCossImage::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CDialog::OnCancel();
}

int CCossImage::OpenIMG(CString filename) 
{
	int i,j, ret=1;
	CFile file;
	CString str;

	if(m_Loadimage.Load(filename)) goto done;

	g_Temp.m_LoadSizeX=m_Loadimage.GetWidth();
	g_Temp.m_LoadSizeY=m_Loadimage.GetHeight();

	if(m_Loadimage.GetHeight() > m_HEIGHT || m_Loadimage.GetWidth() > m_WIDTH)
	{
		ret = 0;
		str.Format(_T("Image Size must be X<%d Y<%d (X=%d, Y=%d)"), m_WIDTH, m_HEIGHT, m_Loadimage.GetWidth(), m_Loadimage.GetHeight());
			AfxMessageBox(str);
			goto done;
	}
	else if(m_Loadimage.GetHeight()==0 || m_Loadimage.GetWidth()==0)
	{
		ret = 0;
		str.Format(_T("Image Size must be X>0 Y>0 "));
			AfxMessageBox(str);
			goto done;
	}
	else
	{
	
		BYTE *src = (BYTE *)m_Loadimage.GetBits();
		BYTE *dst = (BYTE *)m_fmDisplay;
		int pitch=m_Loadimage.GetPitch();
		int y;
		
		if(abs(pitch)>m_WIDTH*2)
		{
			for (y=0; y < g_Temp.m_LoadSizeY; y++,src+=pitch,dst+=m_WIDTH)
			{
				for(j=0;j<g_Temp.m_LoadSizeX;j++)
				{
					*(dst+j)=*(src+3*j);
				}
			}
		}
		else
		{
			for (y=0; y < g_Temp.m_LoadSizeY; y++,src+=pitch,dst+=m_WIDTH)
				CopyMemory(dst, src, g_Temp.m_LoadSizeX);
		}
	 
		if(m_fmProcess)
			memcpy(m_fmProcess, m_fmDisplay, m_WIDTH*m_HEIGHT);
	}
	UpdateImage();
done:
	m_Loadimage.Destroy();
	return ret;

}

int CCossImage::SaveIMG(CString filename) 
{
    BYTE *src = m_fmDisplay;
    BYTE *dst = (BYTE *)m_Saveimage.GetBits();
	int pitch = m_Saveimage.GetPitch();
	for (int y=0; y < m_HEIGHT; y++,src+=m_WIDTH,dst+=pitch)
        CopyMemory(dst, src, m_WIDTH);
	
	if(filename.Find(_T(".bmp"))>=0 || filename.Find(_T(".BMP"))>=0)
		m_Saveimage.Save(filename, Gdiplus::ImageFormatBMP);
	else if(filename.Find(_T(".png"))>=0 || filename.Find(_T(".PNG"))>=0)
		m_Saveimage.Save(filename, Gdiplus::ImageFormatPNG);
	else if(filename.Find(_T(".jpg"))>=0 || filename.Find(_T(".JPG"))>=0 || filename.Find(_T(".jpeg"))>=0 || filename.Find(_T(".JPEG"))>=0)
		m_Saveimage.Save(filename, Gdiplus::ImageFormatJPEG);

	return 0;
}

int CCossImage::SaveMAP(CString filename, LPBYTE fmSrc) 
{
#ifdef WEB_DAKANO_MULTAE
    BYTE *src = fmSrc;
    BYTE *dst = (BYTE *)m_SaveMapimage.GetBits();
	int pitch = m_SaveMapimage.GetPitch();
	for (int y=0; y < 2048; y++,src+=4096,dst+=pitch)
        CopyMemory(dst, src, 4096);
	
	if(filename.Find(_T(".bmp"))>=0 || filename.Find(_T(".BMP"))>=0)
		m_SaveMapimage.Save(filename, Gdiplus::ImageFormatBMP);
	else if(filename.Find(_T(".png"))>=0 || filename.Find(_T(".PNG"))>=0)
		m_SaveMapimage.Save(filename, Gdiplus::ImageFormatPNG);
	else if(filename.Find(_T(".jpg"))>=0 || filename.Find(_T(".JPG"))>=0 || filename.Find(_T(".jpeg"))>=0 || filename.Find(_T(".JPEG"))>=0)
		m_SaveMapimage.Save(filename, Gdiplus::ImageFormatJPEG);

#else
    BYTE *src = fmSrc;
    BYTE *dst = (BYTE *)m_Saveimage.GetBits();
	int pitch = m_Saveimage.GetPitch();

#if DAKANO_WIDTH==64
	for (int y=0; y < 2048; y++,src+=4096,dst+=pitch)
        CopyMemory(dst, src, 4096);
#else
	for (int y=0; y < m_HEIGHT; y++,src+=m_WIDTH,dst+=pitch)
        CopyMemory(dst, src, m_WIDTH);
#endif
	
	if(filename.Find(_T(".bmp"))>=0 || filename.Find(_T(".BMP"))>=0)
		m_Saveimage.Save(filename, Gdiplus::ImageFormatBMP);
	else if(filename.Find(_T(".png"))>=0 || filename.Find(_T(".PNG"))>=0)
		m_Saveimage.Save(filename, Gdiplus::ImageFormatPNG);
	else if(filename.Find(_T(".jpg"))>=0 || filename.Find(_T(".JPG"))>=0 || filename.Find(_T(".jpeg"))>=0 || filename.Find(_T(".JPEG"))>=0)
		m_Saveimage.Save(filename, Gdiplus::ImageFormatJPEG);
#endif

	return 0;
}


BOOL CCossImage::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(m_bUseRButtonXWheel)
	{
		m_nPanX1=pt.x;
		
		if((m_SizeX-(m_nOrgX+pt.x+zDelta-m_nPanX1))/m_dSx>=m_WIDTH)
			zDelta=m_SizeX-(m_WIDTH-1)*m_dSx-m_nOrgX-pt.x+m_nPanX1;

		pt.x+=zDelta;
		if( (pt.x-m_nPanX1)>0 || (m_SizeX-(m_nOrgX+(pt.x-m_nPanX1)))/m_dSx<m_WIDTH)
		{
			m_nOrgX+=(pt.x-m_nPanX1); 
			if(m_nOrgX>0)	      m_nOrgX=0; 
			SetScrollPos(SB_HORZ, -m_nOrgX);
		}
		Invalidate(false);
		m_nPanX1=pt.x;
	}
	else
	{
		m_nPanY1=pt.y;
		
		if((m_SizeY-(m_nOrgY+pt.y+zDelta-m_nPanY1))/m_dSy>=m_HEIGHT)
			zDelta=m_SizeY-(m_HEIGHT-1)*m_dSy-m_nOrgY-pt.y+m_nPanY1;

		pt.y+=zDelta;
		if( (pt.y-m_nPanY1)>0 || (m_SizeY-(m_nOrgY+(pt.y-m_nPanY1)))/m_dSy<m_HEIGHT)
		{
			m_nOrgY+=(pt.y-m_nPanY1); 
			if(m_nOrgY>0)	      m_nOrgY=0; 
			SetScrollPos(SB_VERT, -m_nOrgY);
		}
		Invalidate(false);
		m_nPanY1=pt.y;
	}
	
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

