
//영상평활화
void MakeImageFlat( int left, int width);



//ROI용 Flat
void MakeFlatROI(LPBYTE fmS, LPBYTE fmD, int nBaseValue, int left, int top, int width, int height, int pitch);

//Line Scan용 Flat
void MakeFlatLineScan(LPBYTE fmS, LPBYTE fmD, int nBaseValue, int left, int width, int height, int pitch, int nAvg);

//Area Flat
void Flat_Area(int nPart, LPBYTE fmS, LPBYTE fmD, int nBaseValue, int left, int width, int height, int pitch);

//widthx320 2장 있는 이미지 평활화
void Flat_Area_320_2(LPBYTE fmS, LPBYTE fmD, int nBaseValue, int left, int width, int height, int pitch);

//widthx320 8장 있는 이미지 평활화
void Flat_Area_320_8(LPBYTE fmS, LPBYTE fmD, int nBaseValue, int left, int width, int height, int pitch);


//NBO이미지같이 2개 영역중에 한영역에서 큰 흰점이 발생되는 경우 사용
void Flat_Area_320_2_NBO(LPBYTE fmS, LPBYTE fmD, int nBaseValue, int left, int width, int height, int pitch);


//정코 COS1에서 하는 평활화
int MakeQWPFlatImage(LPBYTE fm, LPBYTE fmR, int left, int top, int right, int bottom, int pitch);