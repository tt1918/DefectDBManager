#include "stdafx.h"
#include "math.h"
#include <afxmt.h>
#include "KoWebDoc.h"
#include "KoWebView.h"

#define MAX_CANDI_POINT				1024
#define CANDIPOINT_AREA				256

#define THREAD_FLAT					1
#define THREAD_FINDCANDISPOT		2
#define THREAD_FINDCANDIAREA		3
#define THREAD_IMAGEROTATE			4
#define THREAD_PERSPECTIVE			5
#define THREAD_FLAT_TOO				6
#define THREAD_FINDCANDITOO			7
#define THREAD_MAKE_COMPIMG			8		//원본을 1/2압축
#define THREAD_MAKE_PYRAMID			9		//평활화된 영상을 1/2, 1/4, 1/8, 1/16 로 압축함
#define THREAD_EMPHASIZE			10
#define THREAD_BINARY				11		//Binray
#define THREAD_MDLINE				12		//투영 MD라인 
#define THREAD_TDLINE				13		//투영 TD라인 
#define THREAD_SCRATCH				14
#define THREAD_FILTER_VEREDGE		15		//Vertical Edge

CCriticalSection  g_CS;
int  g_ThreadPart[MAX_CANDI_POINT];
int  g_ThreadPartEnd[MAX_CANDI_POINT];

LPBYTE g_fm[3];				  //처리할 영상과 결과 담을 영상
long *g_pDataLong;
int  *g_pDataInt;

void SetThreadStart(int nThreadCount, int nType);
int GetThreadPart(int N);

void MakeFlatImageMulti(int N); //
void MakeFlatImageToo(int N);	//투영 한줄 평활화

void FindCandiPoints(int N);	//Point성 후보 구함
void FindCandiArea(int N);		//Area성 후보 구함
	int CO_Compress(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY);
void FindCandiPointsToo(int N);	//투영 Point성 후보 구함

//이미지 Rotation-----------------------------------------------------------
void ImageRotateSub(int ith);
int g_nImageRotateSubPixel=1;	//시간이 없으면 0으로 한다.  (4096x2048 I7에서 33msec, 0으로 설정하면 22msec)
RECT   g_ThreadRect[8];			//처리할 영역 
int    g_Data[10];				//보낼데이타 (int)
double g_dData[10];				//보낼데이타 (double)
//--------------------------------------------------------------------------

void MakePerspectiveTR(int N); 

void MakeCompImage(int N);			//1/2 압축하는 부분함수

int MakeBinarySub(int N);			//Binary Sub

void FindMDLineSub(int N);			//투영라인 MD 부분함수
void FindTDLineSub(int N);			//투영라인 TD 부분함수
void FindScratchSub(int N);			//스크라치 부분함수
void FindFilterVerEdgeSub(int N);	//Vertical Edge만드는 부분함수
//이미지 저장하는 THREAD
UINT THREAD_Save(LPVOID lParam)
{
	CString sFileName;
	CString sPCID;

	while(g_Temp.m_nPGStart)  //프로그램 시작하면서 1, 끝날때 0
	{				
		if(g_Temp.m_nSaveNumberID)
		{
#ifdef USE_HEX_FORMAT
			sPCID.Format(_T("%X"), g_Temp.m_nPCID);
#else
			sPCID.Format(_T("%d"), g_Temp.m_nPCID);
#endif
			if(g_Temp.m_dHDDspace==0 || g_Temp.m_dHDDspace>15.0)	
			{
				if(g_Config.m_nImageType==0)
				{
					sFileName.Format(_T("%s%s\\%s-%d.bmp"), IMAGE_SAVE_PATH, g_Temp.m_slotName, sPCID, g_Temp.m_nSaveNumberID);
					SaveBMP(sFileName, g_fmSave);
				}
				else if(g_Config.m_nImageType==1)
				{
					sFileName.Format(_T("%s%s\\%s-%d.png"), IMAGE_SAVE_PATH, g_Temp.m_slotName, sPCID, g_Temp.m_nSaveNumberID);
					SavePNG(sFileName, g_fmSave);
				}
				else if(g_Config.m_nImageType==2)
				{
					sFileName.Format(_T("%s%s\\%s-%d.jpg"), IMAGE_SAVE_PATH, g_Temp.m_slotName, sPCID, g_Temp.m_nSaveNumberID);
					SaveJPG(sFileName, g_fmSave);
				}
			}
			g_Temp.m_nSaveNumberID=0;
		}
		Sleep(10);	
	}
	return 1;
}


UINT THREAD_SaveCompImage(LPVOID lParam)
{
	CString sFileName;
	CString sPCID;
	int nImageType=1;

	while(g_Temp.m_nPGStart)  //프로그램 시작하면서 1, 끝날때 0
	{				
		if(g_Temp.m_nSaveNumberID_CompImage)
		{
#ifdef USE_HEX_FORMAT
			sPCID.Format(_T("%X"), g_Temp.m_nPCID);
#else
			sPCID.Format(_T("%d"), g_Temp.m_nPCID);
#endif
			if(g_Temp.m_dHDDspace==0 || g_Temp.m_dHDDspace>15.0)	
			{
				if(nImageType==0)
				{
					sFileName.Format(_T("%s%s\\NEL-%s\\NEL-%s_06d.bmp"), NG_IMAGE_PATH, g_Temp.m_slotName, sPCID,sPCID, g_Temp.m_nSaveNumberID_CompImage);
					SaveBMP(sFileName, g_fmSaveComp);
				}
				else if(nImageType==1)
				{
					sFileName.Format(_T("%s%s\\NEL-%s\\NEL-%s_%06d.png"), NG_IMAGE_PATH, g_Temp.m_slotName, sPCID, sPCID, g_Temp.m_nSaveNumberID_CompImage);
					SavePNGComp(sFileName, g_fmSaveComp, g_System.m_nImageW/8, g_System.m_nImageH/8);
				}
				else if(nImageType==2)
				{
					sFileName.Format(_T("%s%s\\NEL-%s\\NEL-%s.jpg"), NG_IMAGE_PATH, g_Temp.m_slotName, sPCID, sPCID, g_Temp.m_nSaveNumberID_CompImage);
					SaveJPG(sFileName, g_fmSaveComp);
				}
			}
			g_Temp.m_nSaveNumberID_CompImage=0;
		}
		Sleep(10);	
	}
	return 1;
}



UINT THREAD_ShowImage(LPVOID lParam)
{
	CString sFileName;
	CString sPCID;
	CKoWebView *pView=(CKoWebView*)lParam;

	while(g_Temp.m_nPGStart)  //프로그램 시작하면서 1, 끝날때 0
	{				
		if(g_Temp.m_nImageCopy==1)
		{
			g_Temp.m_nImageCopy=0;
			pView->m_pImage->UpdateImage();
			memcpy(g_Temp.m_fmImageDst, g_Temp.m_fmImageScr, g_Temp.m_nImageSize);	
			pView->m_pImage->UpdateImage();
		}
		Sleep(1);	
	}
	return 1;
}

UINT THREAD_MultiProcess(LPVOID lParam)
{
	int nID=(int)lParam;
	int i, nPart, nWork;

	//초기화------------------------------------
	memset(g_ThreadPart, 0, MAX_CANDI_POINT*sizeof(int));
	memset(g_ThreadPartEnd, 0, MAX_CANDI_POINT*sizeof(int));
	//------------------------------------------

	while(g_Temp.m_nPGStart)
	{		
		nWork=nPart=-1;
		for(i=0;i<MAX_CANDI_POINT;i++)
		{
			nWork=GetThreadPart(i);
			if(nWork>0)
			{
				nPart=i; //해야할 영역이 남았으면 그 영역
				break;
			}
		}

		if(nPart>=0)
		{
			switch(nWork)
			{
				case THREAD_FLAT:			MakeFlatImageMulti(nPart);   break;  //Line Scan Flat
				case THREAD_FINDCANDISPOT:	FindCandiPoints(nPart);      break;  //후보위치 찾기(Point)
				case THREAD_FINDCANDIAREA:	FindCandiArea(nPart);        break;  //후보위치 찾기(Area)
				case THREAD_IMAGEROTATE:	ImageRotateSub(nPart);	     break;  //이미지회전	
				case THREAD_PERSPECTIVE:	MakePerspectiveTR(nPart);    break;  //이미지 Perspective Transform
				case THREAD_FLAT_TOO:		MakeFlatImageToo(nPart);     break;  //투영이미지 Flat
				case THREAD_FINDCANDITOO:	FindCandiPointsToo(nPart);   break;  //투영이미지 후보찾기(point)
				case THREAD_MAKE_COMPIMG:	MakeCompImage(nPart);	     break;  //압축영상 만들기(1/2만듦)
				case THREAD_MAKE_PYRAMID:	MakePyramidSub(nPart);	     break;  //압축영상 만들기(여러단계)
				case THREAD_EMPHASIZE:      MakeEmphasize(nPart);	     break;  //Emphasize이미지 만들기
				case THREAD_BINARY:			MakeBinarySub(nPart);	     break;  //Binary 이미지 만들기
				case THREAD_MDLINE:         FindMDLineSub(nPart);	     break;  //투영 MD라인 5등분
				case THREAD_TDLINE:         FindTDLineSub(nPart);	     break;  //투영 MD라인 5등분
				case THREAD_SCRATCH:		FindScratchSub(nPart);	     break;	//스크라치
				case THREAD_FILTER_VEREDGE: FindFilterVerEdgeSub(nPart); break;	//Vertical Edge	
			}

			g_ThreadPartEnd[nPart]=0;
	
			Sleep(0);
		}
		else
		{
			Sleep(1);
		}
	}

	return 1;
}

//Local 함수-------------------------------------
void SetThreadStart(int nThreadCount, int nType)
{
	int i;
	g_CS.Lock();

	for(i=0;i<nThreadCount;i++)
		g_ThreadPart[i]=g_ThreadPartEnd[i]=nType;

	g_CS.Unlock();
}

int GetThreadPart(int N)
{
	int ret=-1;
	
	g_CS.Lock();
	if(g_ThreadPart[N])
	{
		ret=g_ThreadPart[N];
		g_ThreadPart[N]=0;
	}
	g_CS.Unlock();

	return ret;
}
//-----------------------------------------------


//평활화>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void ThreadFunc_MakeFlat_LineImage()
{
	int loop, i;

	SetThreadStart(IMAGE_FLAT_PART, THREAD_FLAT);
	
	for(loop=0;loop<200;loop++)	//200msec 이내 완료
	{
		Sleep(1);
		for(i=0;i<IMAGE_FLAT_PART;i++)
		{
			if(g_ThreadPartEnd[i]) break;
		}
		if(i==IMAGE_FLAT_PART) 
			break; //완료
	}
}

void ThreadFunc_MakeFlat_Too()
{
	int loop, i;

	SetThreadStart(g_System.m_nImagePart, THREAD_FLAT_TOO);
	
	for(loop=0;loop<200;loop++)	//200msec 이내 완료
	{
		Sleep(1);
		for(i=0;i<g_System.m_nImagePart;i++)
		{
			if(g_ThreadPartEnd[i]) break;
		}
		if(i==g_System.m_nImagePart) 
			break; //완료
	}
}


//(1/8)이미지만 평활화 하는 함수 (Thread에서만 사용되는 함수) 
//nMethod==1 : 평활화 하는 함수 
void MakeFlatImageMulti(int N)
{
	int height=g_System.m_nImageH;
	int width =g_System.m_nImageW;
	int nFlatBright=g_System.m_nFlatBright;
	int nAvg=(int)g_Temp.m_dAverageBright;
	int left, right;
	
	left =width*N/IMAGE_FLAT_PART;
	right=width*(N+1)/IMAGE_FLAT_PART;

	//8부분으로 나눠서 검사
	MakeFlatLineScan(g_fmGrab[g_ID], g_fmFlat[g_ID], nFlatBright, left, (right-left), height, width, nAvg);
}



extern int g_nProfileRatio[MAX_CROSS_WIDTH];
extern int g_Sum[MAX_CROSS_WIDTH*320];
void MakeFlatImageToo(int N)
{
	int i, j;
	int height=g_System.m_nImageH;
	int width =g_System.m_nImageW;
	int pitch =g_System.m_nPitch;
	int nFlatBright=g_System.m_nFlatBright;
	int nAvg=g_Temp.m_dAverageBright;
	int nPartHeight=g_System.m_nImagePartH;
	int left, right, nOffset, nY, nTmp;
	int nHighLimit, nLowLimit;
	LPBYTE fmToo=g_fmTemp1;
	LPBYTE fmS=g_fmGrab[g_ID];
	LPBYTE fmD=g_fmFlat[g_ID];
	
	left =g_Temp.m_nInspectX1;
	right=g_Temp.m_nInspectX2;

	nHighLimit=nFlatBright+10;
	nLowLimit =nFlatBright-20;
	
	nOffset=N*g_System.m_nImagePartH;
		for(i=0;i<nPartHeight;i++)	
		{
			nY=nOffset+i;
			for(j=left;j<right;j++)
			{
				nTmp=nFlatBright+((*(fmS+pitch*nY+j)-*(g_Sum+pitch*i+j))*g_nProfileRatio[j]+500)/1000;
				if(nTmp<0)		  *(fmD+pitch*nY+j)=0;
				else if(nTmp>255) *(fmD+pitch*nY+j)=255;
				else              *(fmD+pitch*nY+j)=nTmp;

#ifdef USE_TOO_MURA
				if(nTmp>nHighLimit)		 *(fmToo+pitch*nY+j)=nHighLimit;
				else if(nTmp<nLowLimit)  *(fmToo+pitch*nY+j)=nLowLimit;
				else					 *(fmToo+pitch*nY+j)=nTmp;
#endif
			}
		}
}


//후보구하기>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void ThreadFunc_FindCandiPoints()  //Point 성
{
	int loop, i;
	int nPart;
	int nXPart, nYPart;

	nXPart=g_System.m_nImageW/CANDIPOINT_AREA;
	nYPart=g_System.m_nImageH/CANDIPOINT_AREA;

	if(nXPart>32) nXPart=32;
	if(nYPart>32) nYPart=32;


	nPart=nXPart*nYPart;

	g_CandiB.m_nCandiCount =nPart;
	g_CandiB.m_nCandiCountX=nXPart;
	g_CandiB.m_nCandiCountY=nYPart;
	g_CandiW.m_nCandiCount =nPart;
	g_CandiW.m_nCandiCountX=nXPart;
	g_CandiW.m_nCandiCountY=nYPart;

	SetThreadStart(nPart,THREAD_FINDCANDISPOT);
	
	for(loop=0;loop<200;loop++) //200msec이내처리
	{
		Sleep(1);
		for(i=0;i<nPart;i++)
		{
			if(g_ThreadPartEnd[i]) break;
		}
		if(i==nPart) 
			break; //완료
	}
}

//사용안하고...int FindCandiFromPyramid(int nStartX, int nWidth, int nGap) 함수를 대신 사용함
void ThreadFunc_FindCandiArea()  //Area 성
{
	int loop, i;
	int nPart;
	int nXPart, nYPart;

	nXPart=g_System.m_nImageW/CANDIPOINT_AREA;
	nYPart=g_System.m_nImageH/CANDIPOINT_AREA;

	if(nXPart>32) nXPart=32;
	if(nYPart>32) nYPart=32;

	nPart=nXPart*nYPart;

	g_CandiArea.m_nCandiCount =nPart;
	g_CandiArea.m_nCandiCountX=nXPart;
	g_CandiArea.m_nCandiCountY=nYPart;	

	SetThreadStart(nPart,THREAD_FINDCANDIAREA);
	
	for(loop=0;loop<200;loop++) //200msec이내처리
	{
		Sleep(1);
		for(i=0;i<nPart;i++)
		{
			if(g_ThreadPartEnd[i]) break;
		}
		if(i==nPart) 
			break; //완료
	}
}

void ThreadFunc_FindCandiPointsToo()  //Point 성
{
	int loop, i;
	int nPart;
	int nXPart, nYPart;


	nXPart=g_System.m_nImageW/CANDIPOINT_AREA;
	nYPart=g_System.m_nImagePart;

	nPart=nXPart*nYPart;

	SetThreadStart(nPart,THREAD_FINDCANDITOO);
	
	for(loop=0;loop<200;loop++) //200msec이내처리
	{
		Sleep(1);
		for(i=0;i<nPart;i++)
		{
			if(g_ThreadPartEnd[i]) break;
		}
		if(i==nPart) 
			break; //완료
	}
}


#define TINY_OFFSET 10 
void FindCandiPoints(int N)
{
	int i, j, nTmp;
	int height=g_System.m_nImageH;
	int width =g_System.m_nImageW;
	int pitch =g_System.m_nPitch;
	int left, top, right, bottom;
	int nX, nY, nXPart, nYPart;
	int nMax=-1, nMaxPosX, nMaxPosY;
	int nMin=256, nMinPosX, nMinPosY;
	int nSum=0;
	LPBYTE fm, fmT;

#ifdef FIND_TINYDEFECT
	int nTinyMin=256, nTinyMinPosX, nTinyMinPosY;
	LPBYTE fmT1, fmT2;
#endif

	if(g_System.m_nImageW>4096) nXPart=32;
	else                        nXPart=g_System.m_nImageW/CANDIPOINT_AREA;
	if(g_System.m_nImageH>4096) nYPart=32;
	else                        nYPart=g_System.m_nImageH/CANDIPOINT_AREA;

	nX = N%nXPart;
	nY = N/nXPart;

	fm		=g_fmFlat[g_ID];
	left	=width*nX/nXPart;
	right	=width*(nX+1)/nXPart;
	top		=height*nY/nYPart;
	bottom	=height*(nY+1)/nYPart;

#ifdef FIND_TINYDEFECT
		for(i=top;i<bottom;i++)
		{
			fmT=fm+pitch*i+left;
			for(j=left;j<right;j++)
			{
				nTmp=*(fmT++);
				nSum+=nTmp;
				if(nTmp>nMax)
				{
					nMax=nTmp;
					nMaxPosX=j;
					nMaxPosY=i;
				}
				if(nTmp<nMin)
				{
					nMin=nTmp;
					nMinPosX=j;
					nMinPosY=i;
				}

				if(j<TINY_OFFSET || j>width-TINY_OFFSET) continue;
				
				fmT1=fmT-TINY_OFFSET;
				fmT2=fmT+TINY_OFFSET;
				if(nTmp-(*(fmT1)+*(fmT2))/2<nTinyMin)
				{
					nTinyMin=nTmp-(*(fmT1)+*(fmT2))/2;
					nTinyMinPosX=j;
					nTinyMinPosY=i;
				}
				fmT1++;
				fmT2++;
			}
		}
#else
	for(i=top;i<bottom;i++)
	{
		fmT=fm+pitch*i+left;
		for(j=left;j<right;j++)
		{
			nTmp=*(fmT++);
			nSum+=nTmp;
			if(nTmp>nMax)
			{
				nMax=nTmp;
				nMaxPosX=j;
				nMaxPosY=i;
			}
			if(nTmp<nMin)
			{
				nMin=nTmp;
				nMinPosX=j;
				nMinPosY=i;
			}
		}
	}
#endif

	g_CandiW.m_nCandiValue[N] =nMax;
	g_CandiW.m_nCandiX[N]=nMaxPosX;
	g_CandiW.m_nCandiY[N]=nMaxPosY;
	g_CandiW.m_nCandiAvg[N]=nSum/((bottom-top)*(right-left));

	g_CandiB.m_nCandiValue[N] =nMin;
	g_CandiB.m_nCandiX[N]=nMinPosX;
	g_CandiB.m_nCandiY[N]=nMinPosY;
	g_CandiB.m_nCandiAvg[N]=nSum/((bottom-top)*(right-left));

#ifdef FIND_TINYDEFECT
	if(nTinyMin<0)
	{
		g_CandiTinyB.m_nCandiValue[N] =nTinyMin*(-1);
		g_CandiTinyB.m_nCandiX[N]=nTinyMinPosX;
		g_CandiTinyB.m_nCandiY[N]=nTinyMinPosY;
		g_CandiTinyB.m_nCandiAvg[N]=0;
	}
#endif
}

void FindCandiArea(int N)
{
	int i, j, nTmp;
	int height=g_System.m_nImageH;
	int width =g_System.m_nImageW;
	int pitch =g_System.m_nPitch;
	int pitchL;
	int left, top, right, bottom;
	int nX, nY, nXPart, nYPart;
	int nMax=-1, nMaxPosX, nMaxPosY;
	int nMin=256, nMinPosX, nMinPosY;
	int ll,tt,rr,bb;
	int nSum=0;
	int COMPAREA=8;
	LPBYTE fm, fmT;

	if(g_System.m_nImageW>4096) nXPart=16;
	else                        nXPart=g_System.m_nImageW/CANDIPOINT_AREA;
	if(g_System.m_nImageH>4096) nYPart=16;
	else                        nYPart=g_System.m_nImageH/CANDIPOINT_AREA;

	nX = N%nXPart;
	nY = N/nXPart;

	fm		=g_fmFlat[g_ID];
	left	=width*nX/nXPart;
	right	=width*(nX+1)/nXPart;
	top		=height*nY/nYPart;
	bottom	=height*(nY+1)/nYPart;

	CO_Compress(fm, g_fmTemp,  left,  top,  right,  bottom, pitch, COMPAREA, COMPAREA);
	ll=left/COMPAREA;
	rr=right/COMPAREA;
	tt=top/COMPAREA;
	bb=bottom/COMPAREA;
	pitchL=pitch/COMPAREA;

	for(i=tt;i<bb;i++)
	{
		fmT=g_fmTemp+pitchL*i+ll;
		for(j=ll;j<rr;j++)
		{
			nTmp=*(fmT++);
			nSum+=nTmp;
			if(nTmp>nMax)
			{
				nMax=nTmp;
				nMaxPosX=j;
				nMaxPosY=i;
			}
			if(nTmp<nMin)
			{
				nMin=nTmp;
				nMinPosX=j;
				nMinPosY=i;
			}
		}
	}

	g_CandiArea.m_nCandiValue[N] =nMax;
	g_CandiArea.m_nCandiX[N]	 =nMaxPosX*COMPAREA+COMPAREA/2;
	g_CandiArea.m_nCandiY[N]	 =nMaxPosY*COMPAREA+COMPAREA/2;
	g_CandiArea.m_nCandiAvg[N]	 =nSum/((bb-tt)*(rr-ll));

}

void FindCandiPointsToo(int N)
{
	int i, j, nTmp;
	int height=g_System.m_nImageH;
	int width =g_System.m_nImageW;
	int pitch =g_System.m_nPitch;
	int left, top, right, bottom;
	int nX, nY, nXPart, nYPart;
	int nMax=-1, nMaxPosX, nMaxPosY;
	int nMin=256, nMinPosX, nMinPosY;
	int nSum=0;
	LPBYTE fm, fmT;

	nXPart=width/256;
	nYPart=g_System.m_nImagePart;

	nX = N%nXPart;
	nY = N/nXPart;

	fm		=g_fmFlat[g_ID];
	left	=width*nX/nXPart;
	right	=width*(nX+1)/nXPart;
	top		=height*nY/nYPart;
	bottom	=height*(nY+1)/nYPart;
	if(left<g_Temp.m_nInspectX1)  left =g_Temp.m_nInspectX1;
	if(right>g_Temp.m_nInspectX2) right=g_Temp.m_nInspectX2;
	if(right-left<50 )
	{
		g_CanidData[N].dAreaMaxValue =0;
		g_CanidData[N].nAreaMaxX=0;
		g_CanidData[N].nAreaMaxY=0;

		g_CanidData[N].dAreaMinValue =0;
		g_CanidData[N].nAreaMinX=0;
		g_CanidData[N].nAreaMinY=0;

		return;
	}


	for(i=top+1;i<bottom-1;i++)
	{
		fmT=fm+pitch*i+left+1;
		for(j=left+1;j<right-1;j++)
		{
			nTmp=(*(fmT-pitch-1)+*(fmT-pitch)+*(fmT-pitch+1)+*(fmT-1)+*(fmT)+*(fmT+1)+*(fmT+pitch-1)+*(fmT+pitch)+*(fmT+pitch+1)+4)/9;
			nSum+=*(fmT++);
			if(nTmp>nMax)
			{
				nMax=nTmp;
				nMaxPosX=j;
				nMaxPosY=i;
			}
			if(nTmp<nMin)
			{
				nMin=nTmp;
				nMinPosX=j;
				nMinPosY=i;
			}
		}
	}


	g_CanidData[N].dAreaMaxValue =nMax;
	g_CanidData[N].nAreaMaxX	 =nMaxPosX;
	g_CanidData[N].nAreaMaxY	 =nMaxPosY;

	g_CanidData[N].dAreaMinValue =nMin;
	g_CanidData[N].nAreaMinX	 =nMinPosX;
	g_CanidData[N].nAreaMinY	 =nMinPosY;

}

//최대 16x16으로 압축
int CO_Compress(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY)
{
	int i, j;
	LPBYTE fmT;
	int ii,jj;
	int nSum;
	int pitchL=pitch/nSizeX;
	int nDivide=nSizeX*nSizeY;
	int N=0, M=top/nSizeY;

	for(i=top;i<bottom;i+=nSizeY)
	{
		N=left/nSizeX;
		for(j=left;j<right;j+=nSizeX)
		{
			nSum=0;
			for(ii=i;ii<i+nSizeY;ii++)
			{
				fmT=fmS+pitch*ii+j;
				for(jj=j;jj<j+nSizeY;jj++)
					nSum+=*(fmT++);
			}
			*(fmD+M*pitchL+N++)=nSum/nDivide;
		}
		M++;
	}

	return 0;
}


//return 0: SUCCESS
//      <0: Failure
//fmS : Source Image 시작번지
// (left,top)-(right-bottom) : Rotation Area
// pitch: 
// xLimit, yLimit : 영상의 최대 범위 

//(Cx, Cy) : 회전 중심 
//xOff, yOff : 영상의 이동  Offset
//angle : 영상의 회전 각도 
//16k x 16 k : (Release : 2471 msec, Debug: 5119 msec)
//4Kx2K : (Release : 노트북 I5 7200U 2.7G  89msec)
int ThreadFunc_MakeRotateImage(LPBYTE fmS,  LPBYTE fmD, int left, int top, int width, int height, int pitch, int xLimit, int yLimit, double Cx, double Cy, double xOff,double yOff, double angle)
{
	int i, j, loop, ret=-1, nThreadSum;	
	int right=left+width;
	int bottom=top+height;
	int nPart=8;			//8등분

	left   += (int)(xOff);
	top    += (int)(yOff);
	right  += (int)(xOff);
	bottom += (int)yOff;

	if(left<0)              {left=0;		right=left+width; }
	else if(right>xLimit)   {right=xLimit;	left=right-width; }
	if(top<0)               {top = 0;		bottom=top+height;}
	else if(bottom>yLimit)  {bottom=yLimit; top=bottom-height;}	

 	g_Data[0]=xLimit;
	g_Data[1]=yLimit;
	g_Data[2]=pitch;
	g_dData[0]=Cx;
	g_dData[1]=Cy;
	g_dData[2]=xOff;
	g_dData[3]=yOff;
	g_dData[4]=angle;
	g_fm[0]=fmS;
	g_fm[1]=fmD;

	for(i=0;i<nPart;i++)
	{
		g_ThreadRect[i].left=left;
		g_ThreadRect[i].right=right;
		g_ThreadRect[i].top=top+(bottom-top)*i/nPart;
		g_ThreadRect[i].bottom=top+(bottom-top)*(i+1)/nPart;
	}
	SetThreadStart(nPart,THREAD_IMAGEROTATE);

	for(loop=0;loop<500;loop++) //500msec이내처리
	{
		Sleep(1);
		for(i=0;i<nPart;i++)
		{
			if(g_ThreadPartEnd[i]) break;
		}
		if(i==nPart) 
		{
			ret=0;
			break; //완료
		}
	}

    return ret;
}

void ImageRotateSub(int ith)
{
	int i, j, Dum, nOffsetAddr;
	int N, M, xLimit, yLimit, nOffX, nOffY;
	int left,top, right, bottom, pitch;
	double Cx, Cy, xOff, yOff;
	double a,b,pX,pY;
	double cosAngle, sinAngle, dAngle;
	double dTemp1, dTemp2, dTemp3, dTemp4;
	LPBYTE fmS, fmD;

	fmS	   =g_fm[0];
	fmD	   =g_fm[1];
	left   =g_ThreadRect[ith].left;
	top    =g_ThreadRect[ith].top;
	right  =g_ThreadRect[ith].right;
	bottom =g_ThreadRect[ith].bottom;

	xLimit=g_Data[0];
	yLimit=g_Data[1];
	pitch =g_Data[2];
	Cx	  =g_dData[0];
	Cy	  =g_dData[1];
	xOff  =g_dData[2];
	yOff  =g_dData[3];
	dAngle=g_dData[4];
	cosAngle=cos(dAngle*3.141592653589/180.0);
    sinAngle=sin(dAngle*3.141592653589/180.0);

	if(fabs(dAngle)<0.00001)
	{
		nOffX=(int)(xOff+0.5);
		nOffY=(int)(yOff+0.5);
		for(i=top;i<bottom;i++)
		   for(j=left;j<right;j++)
		   {
			   N=j-nOffX;
			   M=i-nOffY;
			   if(N<0 || N>= xLimit || M<0 || M>= yLimit)
				   *(fmD+pitch*i+j)=0;
			   else
					*(fmD+pitch*i+j)=*(fmS+pitch*M+N);
		   }
	}
	else
	{
		if(g_nImageRotateSubPixel)
		{
			for(i=top;i<bottom;i++)
			{
				dTemp1=-cosAngle*Cx+sinAngle*(i-Cy)+Cx-xOff;  //이것 빼서 20%줌
				dTemp2= sinAngle*Cx+cosAngle*(i-Cy)+Cy-yOff;
				for(j=left;j<right;j++)
			   {  		    
					pX= dTemp1+cosAngle*j;
					pY= dTemp2-sinAngle*j;

					N=(int)pX;
					M=(int)pY;
					a=pX-N;
					b=pY-M;

					if(N<0 || N>= xLimit-1 || M<0 || M>= yLimit-1)
					{
						*(fmD+pitch*i+j)=0;
					}
					else
					{
						Dum=M*pitch+N;	
						*(fmD+pitch*i+j)=*(fmS+Dum)*(1.0-a)*(1.0-b)+*(fmS+Dum+1)*(a)*(1.0-b)+*(fmS+Dum+pitch)*(1.0-a)*(b)+*(fmS+Dum+pitch+1)*(a)*(b);						
					}
			   }
			}
		}
		else
		{
			for(i=top;i<bottom;i++)
			{
				dTemp1=-cosAngle*Cx+sinAngle*(i-Cy)+Cx-xOff;  //이것 2둘 빼서 20%줌
				dTemp2= sinAngle*Cx+cosAngle*(i-Cy)+Cy-yOff;
				for(j=left;j<right;j++)
			    {  		    
					pX= dTemp1+cosAngle*j;
					pY= dTemp2-sinAngle*j;

					N=(int)(pX+0.5);
					M=(int)(pY+0.5);				

					if(N<0 || N>= xLimit || M<0 || M>= yLimit)
						*(fmD+pitch*i+j)=0;
					else
						*(fmD+pitch*i+j)=*(fmS+pitch*M+N);
			   }
			}
		}
	}
}

//PERSPECTIVE TRANSFORM 수행하는 THREAD 함수>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int ThreadPerspectiveTR()
{
	int loop, i, ret;
	int nThreadSum;
	int nPART=g_System.m_nImagePart;
	
	g_fm[0]=g_fmGrab[g_ID];
	g_fm[1]=g_fmTemp;
	
	SetThreadStart(nPART,THREAD_PERSPECTIVE);

	for(loop=0;loop<200;loop++) //200msec이내처리
	{
		Sleep(1);
		for(i=0;i<nPART;i++)
		{
			if(g_ThreadPartEnd[i]) break;
		}
		if(i==nPART) 
		{
			ret=0;
			break; //완료
		}
	}
	memcpy(g_fmGrab[g_ID], g_fmTemp, g_System.m_nImageW*g_System.m_nImageH);

	return 0;
}
//PERSPECTIVE TRANSFORM 수행하는 THREAD SUB 함수
void MakePerspectiveTR(int N)
{
	int i, j;
	int pitch=g_System.m_nPitch;
	int nTmp;
	int dTmp;
	LPBYTE fmS, fmD;

	if(g_Temp.m_nMakeLUT!=1) return;

	fmS=g_fm[0]+N*g_System.m_nImagePartH*g_System.m_nPitch;
	fmD=g_fm[1]+N*g_System.m_nImagePartH*g_System.m_nPitch;
	
	for(i=0;i<g_System.m_nImagePartH;i++)
	{
		for(j=0;j<g_System.m_nImageW;j++)
		{
			nTmp=*(g_LUT+pitch*i+j)/1000;
			dTmp=*(g_LUT+pitch*i+j)-nTmp*1000;
			*(fmD+pitch*i+j)=(*(fmS+pitch*i+nTmp)*(1000-dTmp)+*(fmS+pitch*i+nTmp+1)*dTmp+500)/1000;				
		}
	}	
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//원본 영상을 (1/2)  압축영상 만드는  THREAD 함수 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

int ThreadMakeCompression(LPBYTE fmS, LPBYTE fmD)
{
	int loop, i, ret;
	int nThreadSum;
	int nPART=COMPRESSION_PART;
	
	g_fm[0]=fmS;
	g_fm[1]=fmD;

#ifdef USE_DAKANO_LINE
	g_fm[2]=g_fmGrabComp1;
#endif
	
	SetThreadStart(nPART,THREAD_MAKE_COMPIMG);

	for(loop=0;loop<200;loop++) //200msec이내처리
	{
		Sleep(1);
		for(i=0;i<nPART;i++)
		{
			if(g_ThreadPartEnd[i]) break;
		}
		if(i==nPART) 
		{
			ret=0;
			break; //완료
		}
	}

	return 0;
}

//원본 영상을 (1/2)  압축영상 만드는  THREAD SUB함수
void MakeCompImage(int N)
{
	int i, j, k;
	LPBYTE fmS, fmD, fmD1;
	LPBYTE fmDD, fmSS;
	int pitchS=g_System.m_nPitch;
	int nHeightD=g_System.m_nImageH/2;
	int nWidthD =g_System.m_nImageW/2;
	int pitchD, nSx, nW, nSy, nH;
	int ni, nj;

	fmS	   = g_fm[0];
	fmD	   = g_fm[1];
	pitchD = nWidthD;
		
#ifdef SPECIAL_FUNC
	nSx    = N*g_System.m_nImageW/COMPRESSION_PART;
	nW     = g_System.m_nImageW/COMPRESSION_PART;
	ImageCompression(fmS, fmD, g_System.m_nImageH, pitchS, nSx, nSx+ nW);
#else
	nSy		= N*g_System.m_nImageH/(COMPRESSION_PART*2);
	nH      = g_System.m_nImageH/(COMPRESSION_PART*2);

	for(i=nSy;i<nSy+nH;i++)
	{
		fmDD=fmD+pitchD*i;
		fmSS=fmS+pitchS*i*2;
		for(j=0;j<nWidthD;j++)
		{
			nj=j*2;
			*(fmDD++)=(*(fmSS+nj)+*(fmSS+nj+1)+*(fmSS+nj+pitchS)+*(fmSS+pitchS+nj+1)+2)/4;		
		}
	}
#endif

#ifdef USE_DAKANO_LINE
	fmD1	= g_fm[2];
	nSy		= N*g_System.m_nImageH/(COMPRESSION_PART*4);
	nH      = g_System.m_nImageH/(COMPRESSION_PART*4);
	for(i=nSy;i<nSy+nH;i++)
	{
		memcpy(fmD1+nWidthD*i, fmD+nWidthD*i*2, nWidthD);
	}
#endif
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//평활화된 영상을 1/2 1/4 1/8 1/16 1/32 압축하는  THREAD  함수 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int ThreadMakePyramid(LPBYTE fmS)
{
	int loop, i, ret;
	int nThreadSum;
	int nPART=PYRAMID_PART;
	
	g_fm[0]=fmS;
	
	SetThreadStart(nPART,THREAD_MAKE_PYRAMID);

	for(loop=0;loop<200;loop++) //200msec이내처리
	{
		Sleep(1);
		for(i=0;i<nPART;i++)
		{
			if(g_ThreadPartEnd[i]) break;
		}
		if(i==nPART) 
		{
			ret=0;
			break; //완료
		}
	}

	return 0;
}

//강조영상 만드는 THREAD 함수 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int ThreadEmphasize(LPBYTE fmS, LPBYTE fmD, int width, int top, int bottom, int pitch, int nSizeX, int nSizeY, double dFactor)
{
	int loop, i, ret;
	int nThreadSum;
	int nPART=8;
	
	g_fm[0]=fmS;
	g_fm[1]=fmD;
	g_Data[0]=width;
	g_Data[1]=top;
	g_Data[2]=bottom;
	g_Data[3]=pitch;
	g_Data[4]=nSizeX;
	g_Data[5]=nSizeY;
	g_dData[0]=dFactor;
	
	SetThreadStart(nPART,THREAD_EMPHASIZE);

	for(loop=0;loop<200;loop++) //200msec이내처리
	{
		Sleep(1);
		for(i=0;i<nPART;i++)
		{
			if(g_ThreadPartEnd[i]) break;
		}
		if(i==nPART) 
		{
			ret=0;
			break; //완료
		}
	}

	return 0;
}

//Vertical Edge 만드는 THREAD 함수 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int ThreadFilterVerEdge(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch)
{
	int loop, i, ret;
	int nThreadSum;
	int nPART=8;
	
	g_fm[0]=fmS;
	g_fm[1]=fmD;
	g_Data[0]=left;
	g_Data[1]=right;
	g_Data[2]=top;
	g_Data[3]=bottom;
	g_Data[4]=pitch;
	
	SetThreadStart(nPART,THREAD_FILTER_VEREDGE);

	for(loop=0;loop<200;loop++) //200msec이내처리
	{
		Sleep(1);
		for(i=0;i<nPART;i++)
		{
			if(g_ThreadPartEnd[i]) break;
		}
		if(i==nPART) 
		{
			ret=0;
			break; //완료
		}
	}

	return 0;
}
void FindFilterVerEdgeSub(int N)
{
	int i, j;
	int nTmp;
	LPBYTE fmS=g_fm[0];
	LPBYTE fmD=g_fm[1];
	int nLeft, nRight, left,top,right,bottom,pitch;
	
	nLeft	=g_Data[0];
	nRight	=g_Data[1];
	top		=g_Data[2];
	bottom	=g_Data[3];
	pitch	=g_Data[4];

	left =nLeft+(nRight-nLeft)*N/8-4;     if(left<0) left=0;
	right=nLeft+(nRight-nLeft)*(N+1)/8+4; if(right>=pitch) right=pitch-1;
	

	for(i=top;i<bottom;i++)
		for(j=left+4;j<right-4;j++)
		{
			nTmp=*(fmS+pitch*i+j-4)+*(fmS+pitch*i+j+4)-*(fmS+pitch*i+j)*2;
			if(nTmp<0) nTmp=0;
			else if(nTmp>20) nTmp=20;

			*(fmD+pitch*i+j)=nTmp;						 
		}
}
/*
void FindFilterVerEdgeSub(int N)
{
	int i, j;
	int nTmp;
	LPBYTE fmS=g_fm[0];
	LPBYTE fmD=g_fm[1];
	int nLeft, nRight, left,top,right,bottom,pitch, nTh;
	
	nLeft	=g_Data[0];
	nRight	=g_Data[1];
	top		=g_Data[2]+1;
	bottom	=g_Data[3]-1;
	pitch	=g_Data[4];
	nTh		=g_Data[5];	

	left =nLeft+(nRight-nLeft)*N/8-1;     if(left<0) left=0;
	right=nLeft+(nRight-nLeft)*(N+1)/8+1; if(right>=pitch) right=pitch-1;
	

	for(i=top+1;i<bottom-1;i++)
		for(j=left+1;j<right-1;j++)
		{
			nTmp=*(fmS+pitch*(i-1)+j-1)+*(fmS+pitch*(i)+j-1)+*(fmS+pitch*(i+1)+j-1)-*(fmS+pitch*(i-1)+j+1)-*(fmS+pitch*(i)+j+1)-*(fmS+pitch*(i+1)+j+1);
			if(nTmp<0) nTmp*=-1;
			else if(nTmp>255) nTmp=255;

			*(fmD+pitch*i+j)=nTmp;						 
		}
}*/

//Binary영상 만드는 THREAD 함수 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
int ThreadBinary(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nTh)
{
	int loop, i, ret;
	int nThreadSum;
	int nPART=8;
	
	g_fm[0]=fmS;
	g_fm[1]=fmD;
	g_Data[0]=left;
	g_Data[1]=right;
	g_Data[2]=top;
	g_Data[3]=bottom;
	g_Data[4]=pitch;
	g_Data[5]=nTh;
	
	SetThreadStart(nPART,THREAD_BINARY);

	for(loop=0;loop<200;loop++) //200msec이내처리
	{
		Sleep(1);
		for(i=0;i<nPART;i++)
		{
			if(g_ThreadPartEnd[i]) break;
		}
		if(i==nPART) 
		{
			ret=0;
			break; //완료
		}
	}

	return 0;
}

int MakeBinarySub(int N)
{
	int i, j;
	LPBYTE fmS=g_fm[0];
	LPBYTE fmD=g_fm[1];
	int nLeft, nRight, left,top,right,bottom,pitch, nTh;
	
	nLeft	=g_Data[0];
	nRight	=g_Data[1];
	top		=g_Data[2];
	bottom	=g_Data[3];
	pitch	=g_Data[4];
	nTh		=g_Data[5];	

	left =nLeft+(nRight-nLeft)*N/8;
	right=nLeft+(nRight-nLeft)*(N+1)/8;
	
	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
		{
			if(*(fmS+pitch*i+j)>nTh)  *(fmD+pitch*i+j)=255;
			else					  *(fmD+pitch*i+j)=0;
		}

	 return 0;
}

//투영 MD라인
void ThreadFunc_FindMDLine(LPBYTE fm, int nX1, int nX2, int nHeightSub, int pitch)  
{
	int loop, i;
	int nPart=5;

	g_fm[0]=fm;
	g_Data[0]=nX1;
	g_Data[1]=nX2;
	g_Data[2]=pitch;
	g_Data[3]=nHeightSub;


	SetThreadStart(nPart,THREAD_MDLINE);
	
	for(loop=0;loop<50;loop++) //200msec이내처리
	{
		Sleep(1);
		for(i=0;i<nPart;i++)
		{
			if(g_ThreadPartEnd[i]) break;
		}
		if(i==nPart) 
			break; //완료
	}
}

//투영 TD라인
void ThreadFunc_FindTDLine(LPBYTE fm, int nX1, int nX2, int nHeightSub, int pitch)  
{
	int loop, i;
	int nPart=25;

	g_fm[0]=fm;
	g_Data[0]=nX1;
	g_Data[1]=nX2;
	g_Data[2]=pitch;
	g_Data[3]=nHeightSub;


	SetThreadStart(nPart,THREAD_TDLINE);
	
	for(loop=0;loop<50;loop++) //200msec이내처리
	{
		Sleep(1);
		for(i=0;i<nPart;i++)
		{
			if(g_ThreadPartEnd[i]) break;
		}
		if(i==nPart) 
			break; //완료
	}
}

void FindMDLineSub(int N)
{
	LPBYTE fm;
	int nY1, nY2;
	int nX[2], nValue[2], nAngle[2], nOverKill[2];
	int nX1, nX2, nHeightSub, pitch;

	fm		= g_fm[0];
	nX1		=g_Data[0];
	nX2		=g_Data[1];
	pitch	=g_Data[2];
	nHeightSub =g_Data[3];
	
	FindLineBW_TOO_A(N, 1, fm, nX1, 5*N*nHeightSub, nX2, 5*(N+1)*nHeightSub, pitch, nX, nValue, nAngle, nOverKill);

	g_Temp.m_TooX[2*N]			=nX[0];
	g_Temp.m_TooValue[2*N]		=nValue[0];
	g_Temp.m_ToonAngle[2*N]		=nAngle[0];
	g_Temp.m_ToonOverKill[2*N]	=nOverKill[0];
	
	g_Temp.m_TooX[2*N+1]		=nX[1];
	g_Temp.m_TooValue[2*N+1]	=nValue[1];
	g_Temp.m_ToonAngle[2*N+1]	=nAngle[1];
	g_Temp.m_ToonOverKill[2*N+1]=nOverKill[1];
}

extern long l_ProjProfileTD[MAX_WIDTH];
void FindTDLineSub(int N)
{
	LPBYTE fm;
	int i, j;
	int nY1, nY2;
	int nX[2], nValue[2], nAngle[2], nOverKill[2];
	int nX1, nX2, nHeightSub, pitch;
	int nStart, nEnd;
	int nX_, nValue_;

	fm		= g_fm[0];
	nX1		=g_Data[0];
	nX2		=g_Data[1];
	pitch	=g_Data[2];
	nHeightSub =g_Data[3];
	
	i=N;
	for(j=0;j<8;j++)
	{
		nStart=j*256;
		nEnd  =(j+1)*256;

		if(nStart<nX1)   nStart=nX1;
		if(nEnd  >nX2)   nEnd  =nX2;
		if(nEnd-nStart<256) continue;

		FindLineBW_TOO_TD(i, fm, nStart, nHeightSub*i, nEnd, nHeightSub*(i+1), pitch, l_ProjProfileTD, &nX_, &nValue_);

		g_Temp.m_TooTDValue[i][j]=nValue_;
		g_Temp.m_TooTDX[i][j]=nX_;
	}
}


//스크라치
void ThreadFunc_FindScratch(LPBYTE fm, int nX1, int nX2, int nHeight, int pitch, long *pKProj, int  nInspY, int nJumpY, int  nUpperCut, int *pKProjLocalArray)  
{
	int loop, i;
	int nPart=8;  //이것 바꿀려면 nN1=nTotalCount*N/8; 도 같이 바꿔야 함.

	g_fm[0]    =fm;
	g_pDataLong=pKProj;
	g_pDataInt =pKProjLocalArray;

	g_Data[0]=nX1;
	g_Data[1]=nX2;
	g_Data[2]=pitch;
	g_Data[3]=nHeight;
	g_Data[4]=nInspY;
	g_Data[5]=nJumpY;
	g_Data[6]=nUpperCut;

	memset(pKProjLocalArray, 0, 512*MAX_WIDTH*sizeof(int));
	SetThreadStart(nPart,THREAD_SCRATCH);
	
	for(loop=0;loop<200;loop++) //200msec이내처리
	{
		Sleep(1);
		for(i=0;i<nPart;i++)
		{
			if(g_ThreadPartEnd[i]) break;
		}
		if(i==nPart) 
			break; //완료
	}

//	CString str;
//	str.Format(_T("%d"), loop);
//	AfxMessageBox(str);
	
	return;
}

//Y0~Y1, Y1~Y2, ...  8개로 쪼개서 검사함.
void FindScratchSub(int N)
{
	LPBYTE fm;
	int i, j, k, nX1, nX2, pitch, nHeight, nInspY, nJumpY, nUpperCut;
	long *pKProj;
	int *pKProjLocalArray;
	int nY1, nY2, nAvg, nTmp;
	int nN1, nN2;
	int nTotalCount;
	int nUpperCutCount, nTmpSum, nMax;

	fm				 = g_fm[0];
	pKProj			 = g_pDataLong;
	pKProjLocalArray = g_pDataInt;

	nX1			=g_Data[0];
	nX2			=g_Data[1];
	pitch		=g_Data[2];
	nHeight		=g_Data[3];
	nInspY		=g_Data[4];
	nJumpY		=g_Data[5];
	nUpperCut	=g_Data[6];
	
	nTotalCount=nHeight/nJumpY;
	nN1=nTotalCount*N/8;
	nN2=nTotalCount*(N+1)/8;
	if(nN2>=nTotalCount) nN2=nTotalCount-1;
	
	for(k=nN1;k<nN2;k++)
	{
		nY1=k*nJumpY;
		nY2=nY1+nInspY;
		if(nY2>nHeight) { nY2=nHeight; nY1=nY2-nInspY; }

		for(j=nX1;j<nX2;j++)
		{
			nAvg=*(pKProj+j);
			nUpperCutCount=0;
			nTmpSum=0;
			nMax=0;

			for(i=nY1;i<nY2;i++)
			{
				nTmp=*(fm+pitch*i+j)-nAvg;
				if(nTmp>nMax) nMax=nTmp;

				if(nTmp>nUpperCut)
				{
					if(nUpperCutCount<4) nTmp=nAvg;          //Uppercut초과하는 4개까지 없앰(평균값넣음. 휘점이 스크라치로 분류되는 것 막음)
					else                 nTmp=nAvg+nUpperCut;
					nUpperCutCount++;
				}
				else
					nTmp+=nAvg;

				nTmpSum+=nTmp;
			}	

			//일단 최대 4개의 튄값은 뺀다.(nUpperCut을 넘어간건 이미 뻇으므로 할 필요없다------------------
			if(nUpperCutCount<4)
			{
				if(nMax>nUpperCut) nMax=nUpperCut;
				nTmpSum-=(nMax*(4-nUpperCutCount));
			}
			//---------------------------------------------------------------------------------------------


			*(pKProjLocalArray+k*pitch+j)=nTmpSum*100/nInspY;
		}
	}
}