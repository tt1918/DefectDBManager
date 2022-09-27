double GetPitValue(LPBYTE fm, int startX, int startY, int width, int height, int pitch, int nBlackTh);
int  MakeSubPixel(CPoint *POrg, LPBYTE fmOrg, int OrgPitch, CPoint *PMap, LPBYTE fmMap, int MapPitch) ;
void SortBoundaryData(int nCount, BYTE *pData);