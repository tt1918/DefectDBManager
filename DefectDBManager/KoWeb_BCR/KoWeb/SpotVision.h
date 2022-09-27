class CSpotVision
{
public:

	//Comp Filter사용 검사(최대 Size)------------------------------------
	LPBYTE m_fmComp[7];		//압축 이미지 		   
	LPBYTE m_fmCompMask[7]; //압축 마스크 이미지  사용안하면 1 사용하면 0
	LPBYTE m_fmBinary;		//Temp로 사용, Binary(White)로 사용
	LPBYTE m_fmBinaryB;		//Black
	//-------------------------------------------------------------------

	int    m_width[7];
	int    m_height[7];
	int    m_nAllocFailure;

	int SpotInit(int nUseMask, int width, int height);
	void SpotClose();
	int SpotInspect(LPBYTE fm, int pitch,  int nSet, int nId);
	    int MakeBinarySub				(LPBYTE fmS,                int nWidthS, int nHeightS, LPBYTE fmBinary, int nComp, int *pDirection, BOOL bBlack, BOOL bWhite, int nThDn, int nThUp, int nOffset );
		int MakeBinarySubWithMask		(LPBYTE fmS, LPBYTE fmMask, int nWidthS, int nHeightS, LPBYTE fmBinary, int nComp, int *pDirection, BOOL bBlack, BOOL bWhite, int nThDn, int nThUp, int nOffset );
		int MakeBinarySubForBump		(LPBYTE fmS,                int nWidthS, int nHeightS, LPBYTE fmBinary, int nComp, int nThDn, int nThUp, int nOffset );
		int MakeBinarySubForBumpWithMask(LPBYTE fmS, LPBYTE fmMask, int nWidthS, int nHeightS, LPBYTE fmBinary, int nComp, int nThDn, int nThUp, int nOffset );
		int SpotBlob(int nSet, int nId, CChain *pChain, int nComp,  LPBYTE fmGray, LPBYTE fmBinary, int left, int top, int nWidth, int nHeight, int bBlack, int bWhite, int bBumpOnly);
			int GetBumpValue(LPBYTE fmBinary, LPBYTE fmGray, int left, int top, int right, int bottom, int pitch);
			int GetInValue  (LPBYTE fmBinary, LPBYTE fmGray, int left, int top, int right, int bottom, int pitch, int nBlack, int nWhite);
};