#include "StdAfx.h"
#include "CdDib.h"

#define	CD_USE_4WIDTH	1
#define	CD_USE_REVERSE	0

#define WIDTHBYTES(bytes)		( (bytes+3) & ~3 )													//* 영상 폭.

IMPLEMENT_SERIAL(CdDib, CObject, 0);

CdDib::CdDib()
{
	m_pImage = NULL;
	for ( int i = 0; i < 256; ++i ) {
		m_Palette[i].rgbBlue = 
		m_Palette[i].rgbGreen =
		m_Palette[i].rgbRed = i;
		m_Palette[i].rgbReserved = 0;
	}
	m_pPalette = NULL;
}

CdDib::~CdDib()
{
	Free();
}

#define	PI				3.141592												//* 원주율.
#define	RADIAN(A)		(((A)*PI)/180.f)											//* 각도 : Radian 표기.
#define ANGLE(R)		(((R)*180.F)/PI)
#define	SQUARE(A)		((A)*(A))
#include "math.h"
void	Spread(UCHAR* pDst, UCHAR* pSrc, const int& width, const int& height, const double& roiAngle)
{
	int radius = (int)(0.5f*width);
	int roi = (int)(radius*(1.f - cos(RADIAN(roiAngle))));
	double unitAngle = (180.f - 2.f*roiAngle)/(width - 2*roi);
	double offset;
	for ( int h = height/2; h < height; ++h ) {
		for ( int w = 0; w < width; ++w ) {
			if ( w < roi || w > ( width - roi ) ) {
				*(pDst + h*width + w) = *(pSrc + h*width + w);
			} else {
				offset = radius - radius*cos(RADIAN(roiAngle + (w-roi)*unitAngle)) - roi;
				int iOffset = (int)offset;
				*(pDst + h*width + w) = (UCHAR)((1.f - ( offset - iOffset) )*(*(pSrc + h*width + roi + iOffset)) + ( offset - iOffset)*(*(pSrc + h*width + roi + iOffset + 1)));
			}
		}
	}
}

int		GetSpreadWidth(const int& width) 
{
	return (int)(PI*(0.5f*width));
}

void	SpreadWhole(UCHAR* pDst, const int& dstWidth, UCHAR* pSrc, const int& srcWidth, const int& srcHeight)
{
	int radius = (int)(0.5f*srcWidth);
	double unitAngle = 160/dstWidth;
	double offset;
	for ( int h = srcHeight/2; h < srcHeight; ++h ) {
		for ( int w = 0; w < dstWidth; ++w ) {
			offset = radius - radius*cos(RADIAN((w)*unitAngle));
			int iOffset = (int)offset;
			*(pDst + h*srcWidth + w) = (UCHAR)((1.f - ( offset - iOffset) )*(*(pSrc + h*srcWidth + iOffset)) + ( offset - iOffset)*(*(pSrc + h*srcWidth + iOffset + 1)));
		}
	}
}

void	Spread1(UCHAR* pDst, UCHAR* pSrc, const int& width, const int& height, const double& roiAngle)
{
	int radius = (int)(0.5f*width);
	int roi = (int)(radius*(1.f - sin(RADIAN(roiAngle))));
	double unitAngle = 180.f/width;
	double offset;
	for ( int h = height/2; h < height; ++h ) {
		for ( int w = 0; w < width; ++w ) {
			if ( w < (int)radius ) {
				offset = radius - radius*cos(RADIAN(w*unitAngle));
			} else {
				offset = radius - radius*cos(RADIAN(w*unitAngle));
			}
			int iOffset = (int)offset;
			*(pDst + h*width + w) = (UCHAR)((1.f - ( offset - iOffset) )*(*(pSrc + h*width + iOffset)) + ( offset - iOffset)*(*(pSrc + h*width + iOffset + 1)));
		}
	}
}


void	Spread2(UCHAR* pDst, UCHAR* pSrc, const int& width, const int& height, const double& roiAngle)
{
	int radius = (int)(0.5f*width);
	int roi = (int)(radius*(1.f - cos(RADIAN(roiAngle))));
	double unitAngle = (180.f - 2.f*roiAngle)/(width - 2*roi);
	double offset;
	for ( int h = height/2; h < height; ++h ) {
		for ( int w = 0; w < width; ++w ) {
			if ( w < roi || w > ( width - roi ) ) {
				*(pDst + h*width + w) = *(pSrc + h*width + w);
			} else {
				if ( w < (int)radius ) {
					offset = radius - radius*cos(RADIAN(roiAngle + (w-roi)*unitAngle)) - roi;
				} else {
					offset = radius - radius*cos(RADIAN(roiAngle + (w-roi)*unitAngle)) - roi;
				}
				int iOffset = (int)offset;
				*(pDst + h*width + w) = (UCHAR)((1.f - ( offset - iOffset) )*(*(pSrc + h*width + roi + iOffset)) + ( offset - iOffset)*(*(pSrc + h*width + roi + iOffset + 1)));
			}
		}
	}
}
int	CdDib::ReadFile(CString fileName)
{
	BOOL ret = DIB_SUCCESS;
	BITMAPINFOHEADER BitmpaInfoHeader;
	fileName.MakeUpper();
	if ( -1 == fileName.Find(_T(".BMP")) ) {
		ret = DIB_READ_NOTBITMAP;
	} else {
		CFile file;
		if ( file.Open(fileName, CFile::modeRead | CFile::typeBinary) ) 
		{
			file.Read(&m_BitmapFileHeader, sizeof(BITMAPFILEHEADER));		//* Bitmap파일의 Header.
			if ( BITMAP_MASK == m_BitmapFileHeader.bfType ) {				//* File type check.
				file.Read(&BitmpaInfoHeader, sizeof(BITMAPINFOHEADER));	//* 영상정보의 Header.
				if ( 8 == BitmpaInfoHeader.biBitCount) {
					int imageSize = (int)file.GetLength() - 
									sizeof(BITMAPFILEHEADER) -
									sizeof(BITMAPINFOHEADER) -
									256*sizeof(RGBQUAD);
#if CD_USE_4WIDTH
					m_Width = WIDTHBYTES(BitmpaInfoHeader.biWidth);
#else
					m_Width		= BitmpaInfoHeader.biWidth;
#endif
					m_Height	= BitmpaInfoHeader.biHeight;

					file.Read(&m_Palette, 256*sizeof(RGBQUAD));								//* pallete정보.

					SetBitmapInfo(BitmpaInfoHeader);
#if CD_USE_4WIDTH
//					for (int h = m_Height - 1; h >= 0; --h) {										//* 역순 저장.
					for (int h = 0; h < m_Height; ++h) {										//* 역순 저장.
						file.Read(m_pImage + h*WIDTHBYTES(m_Width), m_Width);
					}
#else
					file.Read(m_pImage, imageSize);
#endif
//					MakePalette();
				} else if ( 24 == BitmpaInfoHeader.biBitCount) {
					int imageSize = (int)file.GetLength() - 
									sizeof(BITMAPFILEHEADER) -
									sizeof(BITMAPINFOHEADER);
					m_Width		= BitmpaInfoHeader.biWidth;
					m_Height	= BitmpaInfoHeader.biHeight;
					SetBitmapInfo(m_Width, m_Height, 24);
					file.Read(m_pImage, imageSize);
				}		
			} else {
				ret = DIB_READ_NOTBITMAP;
			}
			file.Close();
		} else {
			ret = DIB_FILE_NOTOPEN;
		}
//		file.Close();
	}
	return	ret;
}


void makeBMPHeader(BITMAPINFOHEADER *pbmiHdr, BITMAPFILEHEADER *pbmfHdr, int width, int height)
{
	memset(pbmfHdr, 0, sizeof(BITMAPFILEHEADER));
	pbmfHdr->bfType = ((WORD)'M'<<8 | 'B');
	pbmfHdr->bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD) + (width*height);
	pbmfHdr->bfReserved1 = 0;
	pbmfHdr->bfReserved2 = 0;
	pbmfHdr->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD);

	memset(pbmiHdr, 0, sizeof(BITMAPINFOHEADER));
	pbmiHdr->biSize = sizeof(BITMAPINFOHEADER);
	pbmiHdr->biWidth = width;
	pbmiHdr->biHeight = height;
	pbmiHdr->biPlanes = 1;
	pbmiHdr->biBitCount = 8;
	pbmiHdr->biCompression = BI_RGB;
	pbmiHdr->biSizeImage = width*height;
}


int CdDib::WriteFile(CString fileName, BOOL invserse )
{
	BOOL ret = DIB_SUCCESS;

	BITMAPFILEHEADER* bmpfile;
	BITMAPINFOHEADER* bmpinfo;
	unsigned char *pSrcImage;
	unsigned char *pDestImage;
	int width = GetWidth();
	int height = GetHeight();
	unsigned char* pImage = GetImage();
	unsigned char *pBmpImage = (unsigned char *)calloc(width*height, sizeof(unsigned char));
	RGBQUAD bmiColor[256];
	for(int i = 0; i < 256; i++){
		bmiColor[i].rgbBlue = i;
		bmiColor[i].rgbGreen = i;
		bmiColor[i].rgbRed = i;
		bmiColor[i].rgbReserved = 0;
	}

	bmpfile = (BITMAPFILEHEADER *)calloc(sizeof(BITMAPFILEHEADER), sizeof(unsigned char));
	bmpinfo = (BITMAPINFOHEADER *)calloc(sizeof(BITMAPINFOHEADER), sizeof(unsigned char));

	makeBMPHeader(bmpinfo, bmpfile, width, height);

	CString strPath, strFileName;
	strFileName = fileName.Mid(fileName.ReverseFind('\\')+1);
	strPath = fileName.Left(fileName.ReverseFind('\\')+1);

	CFileFind fileFind;
	CString strFile = _T("*.*");
	BOOL bResult = fileFind.FindFile(strPath + strFile);

	if(!bResult)
	{
		CreateDirectory(strPath, NULL);
	}

	CFile file;
	if ( !file.Open(fileName, CFile::modeWrite | CFile::modeCreate) ) return false;

	file.Write(bmpfile, sizeof(BITMAPFILEHEADER));
	file.Write(bmpinfo, sizeof(BITMAPINFOHEADER));
	file.Write(bmiColor, sizeof(bmiColor));

	for(int h = 0 ; h < height; h++){
		if ( invserse ) 	pSrcImage = pImage + ( height - 1 - h )*width;
		else				pSrcImage = pImage + ( h )*width;
		pDestImage = pBmpImage + h*width;
		memcpy(pDestImage, pSrcImage, width);
	}

	file.Write(pBmpImage, width * height);
	file.Close();

	free(bmpfile);
	free(bmpinfo);
	free(pBmpImage);

	return	ret;
}

void CdDib::Allocate(unsigned char* pImage, int width, int height, int bitCount)
{
	m_Width = width;
	m_Height = height;
	SetBitmapInfo(m_Width, m_Height, bitCount);

	if ( NULL != pImage ) memcpy(m_pImage, pImage, width*height);
}

void CdDib::Clear()
{
	//if ( NULL != m_pImage ) 
	//	memset( m_pImage, 0x0, m_Width*m_Height*(m_BitmpaInfoHeader.biBitCount/3) );	
}

void CdDib::Free()
{
	if ( m_pImage )	{
		delete[] m_pBitmapInfo;
		DeleteDC(m_hMemoryDC);
		DeleteObject(m_hBitmap);		
		m_pPalette = NULL;
		m_pImage = NULL;
	}
}

void CdDib::SetBitmapInfo(BITMAPINFOHEADER& bitmapInfoHeader)
{
	Free();

	m_pBitmapInfo = (BITMAPINFO *) new char[sizeof(BITMAPINFO)+sizeof(RGBQUAD)*255];
	memcpy((void*)(m_pBitmapInfo), (void*)(&bitmapInfoHeader), sizeof(BITMAPINFOHEADER));
	for ( int i = 0; i < 256; ++i ) {
		m_pBitmapInfo->bmiColors[i].rgbBlue		= m_Palette[i].rgbBlue;
		m_pBitmapInfo->bmiColors[i].rgbRed		= m_Palette[i].rgbRed;
		m_pBitmapInfo->bmiColors[i].rgbGreen	= m_Palette[i].rgbGreen;
		m_pBitmapInfo->bmiColors[i].rgbReserved = 0;
	}

	m_hBitmap = CreateDIBSection(NULL, m_pBitmapInfo, DIB_RGB_COLORS, (void**)(&m_pImage), NULL, 0);
	ZeroMemory(m_pImage, 0);
	m_hMemoryDC = CreateCompatibleDC(GetWindowDC(NULL));
	SelectObject(m_hMemoryDC, m_hBitmap);
}

void CdDib::SetBitmapInfo(const int& width, const int& height, const int& bitCount)
{
	Free();

	m_pBitmapInfo = (BITMAPINFO *) new char[sizeof(BITMAPINFO)+sizeof(RGBQUAD)*255];
	m_pBitmapInfo->bmiHeader.biSize				= sizeof(BITMAPINFOHEADER);
	m_pBitmapInfo->bmiHeader.biWidth			= width;
#if CD_USE_REVERSE
	m_pBitmapInfo->bmiHeader.biHeight			= -1*height;
#else
	m_pBitmapInfo->bmiHeader.biHeight			= height;
#endif
	m_pBitmapInfo->bmiHeader.biPlanes			= 1;
	m_pBitmapInfo->bmiHeader.biBitCount			= bitCount;
	m_pBitmapInfo->bmiHeader.biCompression		= BI_RGB;
#if CD_USE_4WIDTH
	m_pBitmapInfo->bmiHeader.biSizeImage		= WIDTHBYTES(width)*height;
#else
	m_pBitmapInfo->bmiHeader.biSizeImage		= width*height;
#endif
	m_pBitmapInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pBitmapInfo->bmiHeader.biYPelsPerMeter	= 0;
	m_pBitmapInfo->bmiHeader.biClrUsed			= 0;
	m_pBitmapInfo->bmiHeader.biClrImportant		= 0;

	for ( int i = 0; i < 256; ++i ) {
		m_pBitmapInfo->bmiColors[i].rgbBlue		= m_Palette[i].rgbBlue;
		m_pBitmapInfo->bmiColors[i].rgbRed		= m_Palette[i].rgbRed;
		m_pBitmapInfo->bmiColors[i].rgbGreen	= m_Palette[i].rgbGreen;
		m_pBitmapInfo->bmiColors[i].rgbReserved = 0;
	}

	m_hBitmap = CreateDIBSection(NULL, m_pBitmapInfo, DIB_RGB_COLORS, (void**)(&m_pImage), NULL, 0);
	ZeroMemory(m_pImage, 0);
	m_hMemoryDC = CreateCompatibleDC(GetWindowDC(NULL));
	SelectObject(m_hMemoryDC, m_hBitmap);
}
/*
int	 CdDib::GetPaletteSize()
{
	int paletteSize = m_pBitmapInfo->bmiHeader.biClrUsed;
	if ( 0 == paletteSize ) {
		switch ( m_pBitmapInfo->bmiHeader.biBitCount ) {
		case 1:		paletteSize = 2;		break;
		case 4:		paletteSize = 16;		break;
		case 8:		paletteSize = 256;		break;
		case 16:	
		case 24:	
		case 32:	paletteSize = 0;		break;
		default:	ASSERT(FALSE);			break;
		}
	}
	return paletteSize;
}

BOOL CdDib::MakePalette()
{
	int paletteSize = GetPaletteSize();
	if ( 0 == paletteSize )	return FALSE;
	if ( m_pPalette )	{
		m_pPalette->DeleteObject();
		delete m_pPalette;
	}
	m_pPalette = new CPalette();

	LPLOGPALETTE pLogPalette = (LPLOGPALETTE) (new char[2*sizeof(WORD) + paletteSize*sizeof(PALETTEENTRY)]);
	pLogPalette->palVersion = 0x300;
	pLogPalette->palNumEntries = paletteSize;
	for(int i = 0; i < paletteSize; i++) {
		pLogPalette->palPalEntry[i].peRed	= m_Palette[i].rgbRed;
		pLogPalette->palPalEntry[i].peGreen = m_Palette[i].rgbGreen;
		pLogPalette->palPalEntry[i].peBlue	= m_Palette[i].rgbBlue;
		pLogPalette->palPalEntry[i].peFlags	= 0;
	}
	m_pPalette->CreatePalette(pLogPalette);
	delete pLogPalette;
	return TRUE;
}	
*/
void CdDib::Draw(CDC* pDC, const CRect& rect)
{
	if ( m_pImage ) {
		CPalette* pOldPalette = pDC->SelectPalette(m_pPalette, TRUE);
		pDC->RealizePalette();
		int oldMode = SetStretchBltMode(pDC->GetSafeHdc(), COLORONCOLOR);		//* 4 Zooming (삭제).

	#if CD_USE_REVERSE
		StretchBlt(pDC->GetSafeHdc(), rect.left, rect.top, rect.Width(), rect.Height(),
				  m_hMemoryDC, 0, 0, m_pBitmapInfo->bmiHeader.biWidth, abs(m_pBitmapInfo->bmiHeader.biHeight), SRCCOPY);
	#else
		StretchBlt(pDC->GetSafeHdc(), rect.left, rect.top, rect.Width(), rect.Height(),
				  m_hMemoryDC, 0, 0, m_pBitmapInfo->bmiHeader.biWidth, m_pBitmapInfo->bmiHeader.biHeight, SRCCOPY);
	#endif

	/*
		::StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top, rect.Width(), rect.Height(),
			0, 0, m_pBitmapInfo->bmiHeader.biWidth, m_pBitmapInfo->bmiHeader.biHeight,
			m_pImage, (LPBITMAPINFO)&m_pBitmapInfo->bmiHeader, DIB_RGB_COLORS, SRCCOPY);
	*/
		SetStretchBltMode(pDC->GetSafeHdc(), oldMode);	
		pDC->SelectPalette(pOldPalette, TRUE);
	}
}

int ReadLineValue(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, LPBYTE array)
{
	int x,y,dx,dy, cnt=0, d, inc1, inc2, xend, yend, Number=0;

	dx = abs(x2 - x1); 
	dy = abs(y2 - y1);
	if(dx > dy)
	{									// slope between 0 and 1 
		d = (dy<<1) -dx;
		inc1 = dy<<1;
		inc2 = 2 * (dy - dx);
										// draw from smallest x to largest 
		if(x1 >x2)
		{
			x = x2;	y = y2;	xend = x1;
		}
		else
		{
			x = x1;	y = y1;	xend = x2;
		}
		*array++ = *(fm+x+y*pitch);
		cnt++;
		Number++;
		if( (y1 < y2 && xend == x2) || (y1>y2 && xend == x1))
			while(x++ < xend)
			{
				if(d<0) d += inc1;
				else
				{
					y++;	d += inc2;
				}
				*array++ = *(fm+x+y*pitch);
				cnt++;
				Number++;
			}
		else while(x++ < xend)
		{
			if(d<0) d += inc1;
			else
			{
				y--;
				d += inc2;
			}
			*array++ = *(fm+x+y*pitch);
			cnt++;
			Number++;
		}
	}
	else
	{
		d = (dx <<1) - dy;
		inc1 = dx <<1;
		inc2 = 2 * (dx - dy);
		if(y1 > y2)
		{
			y = y2;
			x = x2;
			yend = y1;
		}
		else
		{
			y = y1;
			x = x1;
			yend = y2;
		}
		*array++ = *(fm+x+y*pitch);
		cnt++;
		Number++;
		if( (x1 < x2 && yend == y2) || (x1 > x2 && yend == y1) )
			while(y++ < yend)
			{
				if( d<0) d += inc1;
				else
				{
					x++;
					d += inc2;
				}
				*array++ = *(fm+x+y*pitch);
				cnt++;
				Number++;
			}
		else
			while(y++ < yend)
			{
				if( d<0) d += inc1;
				else
				{
					x--;
					d += inc2;
				}
				*array++ = *(fm+x+y*pitch);
				cnt++;
				Number++;
			}
	 }
	 return cnt;
}
int FindAutoThreshold(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch)
{
	int i,j;
	const int GRAYLEVEL=256;
	DWORD  m_nHistoData[256];
	DWORD  m_nArea,nSum=0;
	double m_dU;    
    int nCount=0, Pos[3];
	int nThreshold;
	double pt, qt, U1, U2, qtPrev;
	double SigmaB2=0.f, MaxSigmaB2=0.f;

	if(x1 > x2 || y1 > y2 ) return -1;
    

	// 초기화 
	for(i=0;i<GRAYLEVEL;i++) m_nHistoData[i]=0;

	// Histogram Data 구하기 
	for(i=y1;i<=y2;i++)
		for(j=x1;j<=x2;j++)
			m_nHistoData[*(fm+i*pitch+j)]++;

    for(i=0;i<GRAYLEVEL;i++)
    {
		nSum+=i*m_nHistoData[i];
    }

	// 면적과 평균을 구한다. 
	m_nArea=(x2-x1+1)*(y2-y1+1);
    m_dU   =(double)nSum/(double)m_nArea;

	 // Histogram의 데이타가 NONZERO인 곳이 1,2있을때>>>>>>>>
	 for(i=0;i<GRAYLEVEL;i++)
		 if(m_nHistoData[i])
		 {
             Pos[nCount]=i;
			 nCount++;
			 if(nCount>2) break;
         }
     if(nCount==1) return -2;
	 else if(nCount==2) return (Pos[0]+Pos[1])/2;
	 //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


	 qt=(double)m_nHistoData[0]/(double)m_nArea;
	 if(qt==1) return 0;

	 U1   = 0;
	 U2   = (m_dU-qt*U1)/(1-qt);

	 for(i=0;i<GRAYLEVEL-1;i++)
     {
		 SigmaB2=qt*(1-qt)*(U2-U1)*(U2-U1);
		 if(SigmaB2 > MaxSigmaB2 )
		 {
			nThreshold=i;
			MaxSigmaB2=SigmaB2;
         }
		 qtPrev=qt;
		 pt=(double)m_nHistoData[i+1]/(double)m_nArea;
		 qt=qtPrev+pt;
		 if(qt==0)       continue;
		 else if(qt==1)  break;    //qt가 1 이면 더이상은 의미가 없으므로 Break함
		 U1=(qtPrev*U1+(i+1)*pt)/qt;
		 U2=(m_dU-qt*U1)/(1-qt);		 
     }

	 return nThreshold;
}

double	GetB2W(UCHAR* pLine, int length)			//* 0 ~ 255.
{
	int maxId = 0;
	double maxSlope = -1.f;
	for ( int i = 2; i < length-2; ++i ) {
		if ( *(pLine + i + 1) - *(pLine + i - 1) >= maxSlope ) {
			maxId = i;
			maxSlope = *(pLine + i + 1) - *(pLine + i - 1);
		}
	}
	if ( -1.f != maxSlope ) return maxId;
	else					return -1;
}

double	GetW2B(UCHAR* pLine, int length)
{
	int maxId = 0;
	double maxSlope = -1.f;
	for ( int i = 2; i < length-2; ++i ) {
		if ( *(pLine + i - 1) - *(pLine + i + 1)  >= maxSlope ) {
			maxId = i;
			maxSlope = *(pLine + i - 1) - *(pLine + i + 1);
		}
	}
	if ( -1.f != maxSlope ) return maxId;
	else					return -1;
}