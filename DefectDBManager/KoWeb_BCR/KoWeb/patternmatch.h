#ifndef PATTERNMATCH_H
#define PATTERNMATCH_H

//  NOTE FOR COPYRIGHT
//	이 소스코드는 고영우에 의해 만들어진 것이므로 다른 사람의 사용을 금합니다.
//
//	1998.6.5
//

#define PM_MIN_SIZE		24
#define PM_MAX_SIZEX	512
#define PM_MAX_SIZEY	512

class CPatternMatch
{
public:
   CPatternMatch();   // 패턴과 서치영역의 시작번지를 초기화 한다.
   ~CPatternMatch();  // 메모리 할당받은것을 Free한다.
   // Template를 티칭한다.
   // fms : 전체영상의 시작번지 
   // x1,y1,x2,y2 : 윈도우 양 끝점
   // WIDTH_MEM : 전체영상의 x축 폭 
   int TeachTemplate(LPBYTE fmS, int x1, int y1, int x2, int y2, int WIDTH_MEM);
   void SetMatchingData(double limitCorrelation, int skip, BOOL SetSubPixel,int nCandiNumber,	double dAcceptCorrValue);
   int Matching(LPBYTE fmS, int x1, int y1, int x2, int y2, int WIDTH_MEM);
   int MatchingMulti(LPBYTE fmS, int x1, int y1, int x2, int y2, int WIDTH_MEM);//, double *corrx, double *corry, double *corr);


   // Template를 저장한다. "*.tmp" 형태로
   void SaveTemplate(CFile *f);
   // Template를 읽어온다. 반드시 SaveTemplate로 저장한것만 읽어올수 있다.
   void LoadTemplate(CFile *f);

   // Limit Value와 skip픽셀수를 지정한다.
   void SetMatchingData(double limitCorrelation, int skip);
   // 4단계 피라밋 매칭을 수행한다.

   // 매칭 x좌표를 받아온다.
   double GetX()     {return m_dCorrX; }; 
   double GetCenterX() {return (m_dCorrX+(m_TempWidth[0]/2.0));};
   // 매칭 y좌표를 받아온다.
   double GetY()     {return m_dCorrY; }; 
   double GetCenterY() {return (m_dCorrY+(m_TempHeight[0]/2.0));};

   // Correlation값을 받아온다.
   double GetValue() {return m_dCorrValue; };  
   // Template데이타를 읽어올때 사용
   LPBYTE GetTempData(int Level, int *Width, int *Height);
   // Search Area 데이타를 읽어올때 사용 
   LPBYTE GetSearchData(int Level, int *Width, int *Height);


protected:
   // 내부에서만 사용되는 함수들 
   void SetSearchArea(LPBYTE fmS, int x1, int y1, int x2, int y2, int WIDTH_MEM);
   int MatchingSubForTop(int N, double CandiData[][3]);
   int MatchingSub(int N, double *nCorrX, double *nCorrY, double *CorrValue);
   int MatchingForBottom(int N, int WIDTH_MEM, double *nCorrX, double *nCorrY, double *CorrValue);
   int MatchingForSubPixel(int WIDTH_MEM);
   int MatchingRotationSub(int N, int WIDTH_MEM);
   int MakePyramid(LPBYTE fmS, LPBYTE fmD, int nWidth, int nHeight);
   int MakePyramidForSearchArea(LPBYTE fmS,LPBYTE fmD,int nWidth, int nHeight,int WIDTH_MEM);
   int FindSecondPolynomialPeak(int N, double Data[][3],double *rx, double *ry);
   int MatchSort(int count, double Value[][3]);
   int MatrixTranspose(int n, int m, double A[][9], double Ai[][9]);
   int MatrixMultiplicant(int n1, int m1, double A[][9], int n2, int m2, double B[][9], double C[][9]);
   int MatrixMultiplicant2(int n, int m, double A[][9], double B[], double C[]);
   int MatrixInverse(int n, double A[][9], double Ai[][9]);

public:

//protected:
	long m_LUT[512];
// Template관련 데이타
	LPBYTE m_fmTemp[4];
	int m_TempWidth[4];
	int m_TempHeight[4];
// Search Area관련 데이타 
	LPBYTE m_fmSearch[4];
	int m_SearchWidth[4];
	int m_SearchHeight[4];

	BOOL   m_SetSubPixel;
	int    m_nCandiNumber;
	int    m_nFoundCandiNumber;
	double m_dAcceptCorrValue;
    double m_dThreshold;
    int    m_nSkip;
	double m_dCorrValue;
	double m_dCorrX;
	double m_dCorrY;

	double m_dCorrValueM[50];
	double m_dCorrXM[50];
	double m_dCorrYM[50];
};

extern CPatternMatch m_KOMatch;


int SavePatternData(CString sFileName);
int LoadPatternData(CString sFileName);

#endif