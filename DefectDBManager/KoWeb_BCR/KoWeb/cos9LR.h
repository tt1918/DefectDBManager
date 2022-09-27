

void WEB_CrossLR();  //LR라인검출
int FindLRLineNew(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nMinValue, long *pProfile, int *pX, int *pY, int *pValue);

void WEB_CrossLR_White(); //LR(White)라인 검출

void WEB_LR_Mura();		//Mura 검출 4 Frame으로 다시 평활화 하여 검출

extern int  l_fmMuraCount;