#pragma once
#include "afx.h"


class CdMemDC : public CDC {
private:    
    CBitmap  m_bitmap;       // Offscreen bitmap
    CBitmap* m_oldBitmap;    // bitmap originally found in CdMemDC
    CDC*     m_pDC;          // Saves CDC passed in constructor
    CRect    m_rect;         // Rectangle of drawing area.
    BOOL     m_bMemDC;       // TRUE if CDC really is a Memory DC.
public:
    
    CdMemDC(CDC* pDC, const CRect* pRect = NULL) : CDC()
    {
        ASSERT(pDC != NULL); 

        // Some initialization
        m_pDC = pDC;
        m_oldBitmap = NULL;
        m_bMemDC = !pDC->IsPrinting();

        // Get the rectangle to draw
        if (pRect == NULL) {
            pDC->GetClipBox(&m_rect);
        } else {
            m_rect = *pRect;
        }
        
        if (m_bMemDC) {
            // Create a Memory DC
            CreateCompatibleDC(pDC);
            pDC->LPtoDP(&m_rect);

            m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
            m_oldBitmap = SelectObject(&m_bitmap);
            
            SetMapMode(pDC->GetMapMode());
            pDC->DPtoLP(&m_rect);
            SetWindowOrg(m_rect.left, m_rect.top);
        } else {
            // Make a copy of the relevent parts of the current DC for printing
            m_bPrinting = pDC->m_bPrinting;
            m_hDC       = pDC->m_hDC;
            m_hAttribDC = pDC->m_hAttribDC;
        }

        // Fill background 
        FillSolidRect(m_rect, pDC->GetBkColor());
    }

    
    ~CdMemDC()    
    {        
        if (m_bMemDC) {
            // Copy the offscreen bitmap onto the screen.
            m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
                this, m_rect.left, m_rect.top, SRCCOPY);            
            
            //Swap back the original bitmap.
            SelectObject(m_oldBitmap);        
        } else {
            // All we need to do is replace the DC with an illegal value,
            // this keeps us from accidently deleting the handles associated with
            // the CDC that was passed to the constructor.            
            m_hDC = m_hAttribDC = NULL;
        }    
    }
    
    CdMemDC* operator->()	{	return this;   }    
    operator CdMemDC*()		{	return this;   }
};

enum DIB_RET {
	DIB_SUCCESS	= 0,
	DIB_FILE_NOTOPEN,
	DIB_READ_NOTBITMAP,
	DIB_READ_NOT8BIT,
	DIB_SIZE
};

#define BITMAP_MASK	0x4D42													//* BM : 0x4D42

class CdDib : public CObject  
{
   DECLARE_SERIAL(CdDib)
public:
	CdDib();
	virtual ~CdDib();

	void	Allocate(unsigned char* pImage, int width, int height, int bitCount = 8);
	void	Free();
	void	Clear();

	void	SetBitmapInfo(BITMAPINFOHEADER& bitmapInfoHeader);
	void	SetBitmapInfo(const int& width, const int& height, const int& bitCount = 8);
	BITMAPINFO*	GetBitmapInfo()	const { return m_pBitmapInfo; }
	BITMAPINFOHEADER*	GetBitmapInfoHeader()	{ return &m_BitmpaInfoHeader; }
	CPalette*	GetPalette()	const { return m_pPalette; }
	
	unsigned char*	GetImage()	const	{ return m_pImage;	}
	int		GetWidth()		const	{ return m_Width;	}
	int		GetHeight()		const	{ return m_Height;	}

	void	Draw(CDC* pDC, const CRect& rect);

	int		ReadFile(CString filename);
	int		WriteFile(CString filename, BOOL invserse = FALSE);
private:
//	int		GetPaletteSize();
//	BOOL	MakePalette();

	int					m_Width;
	int					m_Height;
	unsigned char*		m_pImage;

	HBITMAP				m_hBitmap;
	HDC					m_hMemoryDC;
	BITMAPINFO*			m_pBitmapInfo;
	BITMAPFILEHEADER	m_BitmapFileHeader;
	BITMAPINFOHEADER	m_BitmpaInfoHeader;
	RGBQUAD				m_Palette[256];
	CPalette*			m_pPalette;
};
int FindAutoThreshold(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch);

double	GetB2W(UCHAR* pLine, int length);
double	GetW2B(UCHAR* pLine, int length);