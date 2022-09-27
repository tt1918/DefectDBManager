
#define OCR_NUMBER		38
#define OCR_MAX_WIDTH	60
#define OCR_MAX_HEIGHT	90
#define OCR_MAX_PATTERN	10     //현재는 7개    (정상, 옆확장/축소  위확장/축소  모두확장/축소   

#define OCR_ELONG_X	2
#define OCR_ELONG_Y	3

typedef struct OCRTEACH_
{
	int m_nRegistered[OCR_MAX_PATTERN];
	LPBYTE m_fm[OCR_MAX_PATTERN];
	int m_nSmooth;
	int m_nOverlap;
	int m_nOverLapCount;
	int m_nSizeX;
	int m_nSizeY;




}OCRTEACH;


extern TCHAR l_OCRCHAR[OCR_NUMBER];
extern OCRTEACH g_OCR[OCR_NUMBER];

int InitOCRData();
void CloseOCRData();

int LoadOCRData(CString sFileName);
int SaveOCRData(CString sFileName);

int FindOCR(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int *Score, int *pOCR, int nPosition, int *pX, int *pY);
