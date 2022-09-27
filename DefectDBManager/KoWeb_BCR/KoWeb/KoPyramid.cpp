#include "stdafx.h"
#include "math.h"

#define COMP_LEVEL		5			//압축레벨: 5Level의 데이타를 갖고 있다는 뜻 (4번 압축)
LPBYTE l_fmPyramid[COMP_LEVEL];		//원본, (1/2), (1/4), (1/8), (1/16)
int    l_nPyramidWidth[COMP_LEVEL];
int    l_nPyramidHeight[COMP_LEVEL];

//압축된 영상이 저장될 Memory확보
bool OpenPyramid(int nWidth, int nHeight)
{
	int i;

	l_nPyramidWidth[0]=nWidth;
	l_nPyramidHeight[0]=nHeight;

	for(i=1;i<COMP_LEVEL;i++)
	{
		l_nPyramidWidth[i] =l_nPyramidWidth[i-1]/2;
		l_nPyramidHeight[i]=l_nPyramidHeight[i-1]/2;
	}

	for(i=1;i<COMP_LEVEL;i++)
	{
		l_fmPyramid[i]= new BYTE[l_nPyramidWidth[i]*l_nPyramidHeight[i]];
		if(l_fmPyramid[i]==NULL) 
			return false;
	}

	return true;
}

void ClosePyramid()
{
	int i;
	for(i=1;i<COMP_LEVEL;i++)
		delete l_fmPyramid[i];
}

//일정영역을 압축함.(세로는 전체, X만 나눔)
//nStartX, nWidth는 16의 배수로 설정해야 한다.(가능하면)
void MakePyramid(LPBYTE fmSrc, int nStartX, int nWidth)
{
	int i, j, k;
	LPBYTE fmS=fmSrc;
	int pitchS=l_nPyramidWidth[0];
	LPBYTE fmD;
	int pitchD;
	int nSx=nStartX;
	int nW=nWidth;
	int ni, nj;

	for(k=1;k<COMP_LEVEL;k++)
	{
		//압축되면 아래 크기로 됨-------
		nSx    = nSx/2;
		nW     = nW /2;
		fmD	   = l_fmPyramid[k];
		pitchD = l_nPyramidWidth[k];
		//-----------------------------

		for(i=0;i<l_nPyramidHeight[k];i++)
		{
			ni=i*2;
			for(j=nSx;j<nW;j++)
			{
				nj=j*2;
				*(fmD+pitchD*i+j)=(*(fmS+pitchS*ni+nj)+*(fmS+pitchS*ni+nj+1)+*(fmS+pitchS*(ni+1)+nj)+*(fmS+pitchS*(ni+1)+nj+1)+2)/4;
			}
		}
		fmS   = l_fmPyramid[k];
		pitchS= l_nPyramidWidth[k];
	}
}

extern LPBYTE g_fm[3];
//깊은 Level로 압축
void MakePyramidSub(int N)
{
	int i, j, k;
	LPBYTE fmS;
	int pitchS=l_nPyramidWidth[0];
	LPBYTE fmD, fmDD, fmSS;
	int pitchD;
	int nSx=N*g_System.m_nImageW/PYRAMID_PART;
	int nW=g_System.m_nImageW/PYRAMID_PART;
	int nSy=N*g_System.m_nImageH/PYRAMID_PART;
	int nH=g_System.m_nImageH/PYRAMID_PART;
	int ni, nj;

	fmS=g_fm[0];
	for(k=1;k<COMP_LEVEL;k++)
	{

#ifdef SPECIAL_FUNC
		//압축되면 아래 크기로 됨-------
		nSx    = nSx/2;
		nW     = nW /2;
		fmD	   = l_fmPyramid[k];
		pitchD = l_nPyramidWidth[k];
		//-----------------------------
		ImageCompression(fmS, fmD, l_nPyramidHeight[k]*2, pitchS, nSx*2, nSx*2+ nW*2);
#else
		//압축되면 아래 크기로 됨-------
		nSy    = nSy/2;
		nH     = nH /2;
		fmD	   = l_fmPyramid[k];
		pitchD = l_nPyramidWidth[k];
		//-----------------------------
		for(i=nSy;i<nSy+nH;i++)
		{
			fmDD=fmD+pitchD*i;
			fmSS=fmS+pitchS*i*2;
			for(j=0;j<l_nPyramidWidth[k];j++)
			{
				nj=2*j;
				*(fmDD++)=(*(fmSS+nj)+*(fmSS+nj+1)+*(fmSS+nj+pitchS)+*(fmSS+pitchS+nj+1)+2)/4;
			}
		}
#endif

		fmS   = l_fmPyramid[k];
		pitchS= l_nPyramidWidth[k];
	}
}


//보틍은 nWidth은 nGap의 배수가 되게 설정한다.
// 흑/백 후보는(1/2)  압축한 영상에서 구함.
// Area후보는  (1/16) 압축한 영상에서 구함.

#if INSPECT_TYPE==TAC_LINE
int FindCandiFromPyramid(int nStartX, int nWidth, int nGap)
{
	int i,j, n,m;

	if(nWidth>4096)
	{
		if(nGap<256) nGap=256;  //영역이 1024개가 Max이므로 (8192x8192)
	}

	int nN=nStartX/nGap;
	int nEndX=nStartX+nWidth;
	int nNW;  
	int nM, nDiff;
	int nNPitch=l_nPyramidWidth[0]/nGap;
	int nCandiCount=0;
	int nX1, nX2, nY1, nY2, pitch;
	int nLastGap=1;
	LPBYTE fm, fmT, fmT_;
	int nID;
	int nFlatBase=g_System.m_nFlatBright;
	int nTmp, nTmp_, nSum,nFlag, nMax, nMaxDiff, nMaxPosX, nMaxPosY, nMin, nMinPosX, nMinPosY;
	int nLimitX1=g_Temp.m_nInspectX1/2;
	int nLimitX2=g_Temp.m_nInspectX2/2;
	int nLimitLastX1;
	int nLimitLastX2;
	int nTinyMaxB;
	

	for(i=1;i<COMP_LEVEL;i++)
		nLastGap=nLastGap*2;

	nLimitLastX1=g_Temp.m_nInspectX1/nLastGap;
	nLimitLastX2=g_Temp.m_nInspectX2/nLastGap;

	//범위CHECK--------------------------------------------------------
	if(nEndX>l_nPyramidWidth[0]) nEndX=l_nPyramidWidth[0];

	//(nStartX+nWidth)가 nGap의 배수가 아니면 아래와 같이처리----------
	nNW=(nStartX+nWidth)/nGap;
	if((nStartX+nWidth)-nNW*nGap>0)      nNW++;
	
	//l_nPyramidHeight[0]가 nGap의 배수가 아니면 아래와 같이처리-------
	nM=l_nPyramidHeight[0]/nGap;
	if(l_nPyramidHeight[0]-nM*nGap>0)    nM++;

	//전체영역이 nGap의 배수가 아니면 
	nNPitch=l_nPyramidWidth[0]/nGap;
	if(l_nPyramidWidth[0]>nNPitch*nGap)  nNPitch++;
	//-----------------------------------------------------------------

	memset(&g_CandiW,		0, sizeof(CANDIPOINTS));
	memset(&g_CandiB,		0, sizeof(CANDIPOINTS));
	memset(&g_CandiArea,	0, sizeof(CANDIPOINTS));

	for(n=0;n<nM;n++) //Y
	{
		for(m=nN;m<nNW;m++)  //X
		{
			nID=n*nNPitch+m;

			//(1/2)영역에서 후보 구함 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			nX1=m*nGap/2;			 if(nX1<nStartX/2)   nX1=nStartX/2;
			nX2=(m+1)*nGap/2;		 if(nX2>nEndX/2)     nX2=nEndX/2;
			nY1=n*nGap/2; 
			nY2=(n+1)*nGap/2;		 if(nY2>l_nPyramidHeight[1]) nY2=l_nPyramidHeight[1];
			pitch=l_nPyramidWidth[1];

			//미검영역 적용-------------------
			if(nX1<nLimitX1) nX1=nLimitX1;
			if(nX2>nLimitX2) nX2=nLimitX2;
			if(nX2-nX1<10) continue;
			//--------------------------------

			nSum=0;
			nMax=-1;
			nMin=256;
			nTinyMaxB=0;
			for(i=nY1;i<nY2;i++)
			{
				fmT=l_fmPyramid[1]+i*pitch+nX1;
				for(j=nX1;j<nX2;j++)
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
			g_CandiW.m_nCandiValue[nID] =nMax;
			g_CandiW.m_nCandiX[nID]		=nMaxPosX*2+1;
			g_CandiW.m_nCandiY[nID]		=nMaxPosY*2+1;
			g_CandiW.m_nCandiAvg[nID]	=nSum/((nY2-nY1)*(nX2-nX1));

			g_CandiB.m_nCandiValue[nID] =nMin;
			g_CandiB.m_nCandiX[nID]		=nMinPosX*2+1;
			g_CandiB.m_nCandiY[nID]		=nMinPosY*2+1;
			g_CandiB.m_nCandiAvg[nID]	=nSum/((nY2-nY1)*(nX2-nX1));



			//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


			//(1/nLastGap)영역에서 후보 구함 >>>>>>>>>>>>>>>>>>>>
			nX1=m*nGap/nLastGap;				if(nX1<nStartX/nLastGap)  nX1=nStartX/nLastGap;
			nX2=(m+1)*nGap/nLastGap;			if(nX2>nEndX/nLastGap)	  nX2=nEndX/nLastGap;
			nY1=n*nGap/nLastGap;
			nY2=(n+1)*nGap/nLastGap;			if(nY2>l_nPyramidHeight[COMP_LEVEL-1]) nY2=l_nPyramidHeight[COMP_LEVEL-1];
			pitch=l_nPyramidWidth[COMP_LEVEL-1];

			//미검영역 적용-----------------------
			if(nX1<nLimitLastX1) nX1=nLimitLastX1;
			if(nX2>nLimitLastX2) nX2=nLimitLastX2;
			if(nX2-nX1<1) continue;
			//------------------------------------

			nSum=0;
			nMax=0;
			nMin=256;
			for(i=nY1;i<nY2-1;i++)
			{
				fmT =l_fmPyramid[COMP_LEVEL-1]+i*pitch+nX1;
				for(j=nX1;j<nX2;j++)
				{
					nTmp =*(fmT++);										
					if(nTmp>nMax)
					{
						nMax=nTmp;
						nMaxPosX=j;
						nMaxPosY=i;
					}
					else if(nTmp<nMin)
					{
						nMin=nTmp;
						nMinPosX=j;
						nMinPosY=i;
					}					
				}
			}

			if(nFlatBase-nMin>=nMax-nFlatBase)
			{
				g_CandiArea.m_nCandiValue[nID]	=nFlatBase-nMin;
				g_CandiArea.m_nCandiX[nID]		=nMinPosX*nLastGap+nLastGap/2;
				g_CandiArea.m_nCandiY[nID]		=nMinPosY*nLastGap+nLastGap;
				g_CandiArea.m_nCandiAvg[nID]	=-1;
			}
			else
			{
				g_CandiArea.m_nCandiValue[nID]	=nMax-nFlatBase;
				g_CandiArea.m_nCandiX[nID]		=nMaxPosX*nLastGap+nLastGap/2;
				g_CandiArea.m_nCandiY[nID]		=nMaxPosY*nLastGap+nLastGap;
				g_CandiArea.m_nCandiAvg[nID]	=1;

			}					
			//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

			nCandiCount++;
		}
	}

	return nCandiCount;
}
#else
int FindCandiFromPyramid(int nStartX, int nWidth, int nGap)
{
	int i,j, n,m;

	if(nWidth>4096)
	{
		if(nGap<256) nGap=256;  //영역이 1024개가 Max이므로 (8192x8192)
	}

	int nN=nStartX/nGap;
	int nEndX=nStartX+nWidth;
	int nNW;  
	int nM, nDiff;
	int nNPitch=l_nPyramidWidth[0]/nGap;
	int nCandiCount=0;
	int nX1, nX2, nY1, nY2, pitch;
	int nLastGap=1;
	LPBYTE fm, fmT, fmT_;
	int nID;
	int nFlatBase=g_System.m_nFlatBright;
	int nTmp, nTmp_, nSum,nFlag, nMax, nMaxDiff, nMaxPosX, nMaxPosY, nMin, nMinPosX, nMinPosY;
	int nLimitX1=g_Temp.m_nInspectX1/2;
	int nLimitX2=g_Temp.m_nInspectX2/2;
	int nLimitLastX1;
	int nLimitLastX2;
	int nTinyMaxB;

#ifdef FIND_TINYDEFECT
	LPBYTE fmT1, fmT2;
	int  nTinyMaxBPosX, nTinyMaxBPosY;
	int nTinyOff=7;
#endif
	

	for(i=1;i<COMP_LEVEL;i++)
		nLastGap=nLastGap*2;

	nLimitLastX1=g_Temp.m_nInspectX1/nLastGap;
	nLimitLastX2=g_Temp.m_nInspectX2/nLastGap;

	//범위CHECK--------------------------------------------------------
	if(nEndX>l_nPyramidWidth[0]) nEndX=l_nPyramidWidth[0];

	//(nStartX+nWidth)가 nGap의 배수가 아니면 아래와 같이처리----------
	nNW=(nStartX+nWidth)/nGap;
	if((nStartX+nWidth)-nNW*nGap>0)      nNW++;
	
	//l_nPyramidHeight[0]가 nGap의 배수가 아니면 아래와 같이처리-------
	nM=l_nPyramidHeight[0]/nGap;
	if(l_nPyramidHeight[0]-nM*nGap>0)    nM++;

	//전체영역이 nGap의 배수가 아니면 
	nNPitch=l_nPyramidWidth[0]/nGap;
	if(l_nPyramidWidth[0]>nNPitch*nGap)  nNPitch++;
	//-----------------------------------------------------------------

	memset(&g_CandiW,		0, sizeof(CANDIPOINTS));
	memset(&g_CandiB,		0, sizeof(CANDIPOINTS));
	memset(&g_CandiArea,	0, sizeof(CANDIPOINTS));

	for(n=0;n<nM;n++) //Y
	{
		for(m=nN;m<nNW;m++)  //X
		{
			nID=n*nNPitch+m;

			//(1/2)영역에서 후보 구함 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			nX1=m*nGap/2;			 if(nX1<nStartX/2)   nX1=nStartX/2;
			nX2=(m+1)*nGap/2;		 if(nX2>nEndX/2)     nX2=nEndX/2;
			nY1=n*nGap/2; 
			nY2=(n+1)*nGap/2;		 if(nY2>l_nPyramidHeight[1]) nY2=l_nPyramidHeight[1];
			pitch=l_nPyramidWidth[1];

			//미검영역 적용-------------------
			if(nX1<nLimitX1) nX1=nLimitX1;
			if(nX2>nLimitX2) nX2=nLimitX2;
			if(nX2-nX1<10) continue;
			//--------------------------------

			nSum=0;
			nMax=-1;
			nMin=256;
			nTinyMaxB=0;
			for(i=nY1;i<nY2;i++)
			{
				fmT=l_fmPyramid[1]+i*pitch+nX1;
				for(j=nX1;j<nX2;j++)
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
#ifdef FIND_TINYDEFECT
					if(j<nX1+nTinyOff || j>=nX2-nTinyOff) continue;

					fmT1=fmT-nTinyOff-1;
					fmT2=fmT+nTinyOff-1;
					if(*(fmT1)-nTmp>=g_Param.m_nAreaThDn && *(fmT2)-nTmp>=g_Param.m_nAreaThDn)
					{
						nTmp=(*(fmT1)+*(fmT2))/2-nTmp;
						if(nTmp>nTinyMaxB)
						{
							nTinyMaxB=nTmp;
							nTinyMaxBPosX=j;
							nTinyMaxBPosY=i;
						}
					}
#endif
				}					
			}
			g_CandiW.m_nCandiValue[nID] =nMax;
			g_CandiW.m_nCandiX[nID]		=nMaxPosX*2+1;
			g_CandiW.m_nCandiY[nID]		=nMaxPosY*2+1;
			g_CandiW.m_nCandiAvg[nID]	=nSum/((nY2-nY1)*(nX2-nX1));

			g_CandiB.m_nCandiValue[nID] =nMin;
			g_CandiB.m_nCandiX[nID]		=nMinPosX*2+1;
			g_CandiB.m_nCandiY[nID]		=nMinPosY*2+1;
			g_CandiB.m_nCandiAvg[nID]	=nSum/((nY2-nY1)*(nX2-nX1));

#ifdef FIND_TINYDEFECT
			g_CandiTinyB.m_nCandiValue[nID] =nTinyMaxB;
			g_CandiTinyB.m_nCandiAvg[nID]	=0;
			if(nTinyMaxB>0)
			{				
				g_CandiTinyB.m_nCandiX[nID]		=nTinyMaxBPosX*2+1;
				g_CandiTinyB.m_nCandiY[nID]		=nTinyMaxBPosY*2+1;				
			}
#endif

			//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


			//(1/nLastGap)영역에서 후보 구함 >>>>>>>>>>>>>>>>>>>>
			nX1=m*nGap/nLastGap;				if(nX1<nStartX/nLastGap)  nX1=nStartX/nLastGap;
			nX2=(m+1)*nGap/nLastGap;			if(nX2>nEndX/nLastGap)	  nX2=nEndX/nLastGap;
			nY1=n*nGap/nLastGap;
			nY2=(n+1)*nGap/nLastGap;			if(nY2>l_nPyramidHeight[COMP_LEVEL-1]) nY2=l_nPyramidHeight[COMP_LEVEL-1];
			pitch=l_nPyramidWidth[COMP_LEVEL-1];

			//미검영역 적용-----------------------
			if(nX1<nLimitLastX1) nX1=nLimitLastX1;
			if(nX2>nLimitLastX2) nX2=nLimitLastX2;
			if(nX2-nX1<1) continue;
			//------------------------------------

			nSum=0;
			nMaxDiff=-1;
			nMax=0;
			nMin=256;
			for(i=nY1;i<nY2-1;i++)
			{
				fmT =l_fmPyramid[COMP_LEVEL-1]+i*pitch+nX1;
				fmT_=l_fmPyramid[COMP_LEVEL-1]+(i+1)*pitch+nX1;
				for(j=nX1;j<nX2;j++)
				{
					nTmp =*(fmT++);
					nTmp_=*(fmT_++);

					nDiff=nTmp-nTmp_; if(nDiff<0) nDiff*=-1;
					
					if(nDiff>nMaxDiff)
					{
						nMaxDiff=nDiff;
						nMaxPosX=j;
						nMaxPosY=i;
						if(nTmp>nTmp_)
						{
							if(nTmp-nFlatBase>=nFlatBase-nTmp_) nFlag=1;
							else                                nFlag=-1;
						}
						else
						{
							if(nTmp_-nFlatBase>=nFlatBase-nTmp) nFlag=1;
							else                                nFlag=-1;
						}
					}
				}
			}
			g_CandiArea.m_nCandiValue[nID]	=nMaxDiff;
			g_CandiArea.m_nCandiX[nID]		=nMaxPosX*nLastGap+nLastGap/2;
			g_CandiArea.m_nCandiY[nID]		=nMaxPosY*nLastGap+nLastGap;
			g_CandiArea.m_nCandiAvg[nID]	=nFlag;//nSum/((nY2-nY1)*(nX2-nX1));					
			//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

			nCandiCount++;
		}
	}

	return nCandiCount;
}
#endif

CANDIPOINTS l_CandiTemp;
int FindCandiSorting(int nStartX, int nWidth, int nGap)
{
	int i, j, k, m;
	int nCol, nRow, nNPitch;
	int nTmp, nMax, nPos, nOverlap;
	int nDx, nDy, nMinDx, nMinDy;
	int nLimitY=128;
	int nPosX1, nPosX2;
	int nMaxLoop;
	int MARKING_X_OFFSET=g_System.m_nOverlapX;
	int MARKING_Y_OFFSET=g_System.m_nOverlapY;
	if(MARKING_X_OFFSET<=0) MARKING_X_OFFSET=1;
	if(MARKING_Y_OFFSET<=0) MARKING_Y_OFFSET=1;

	CANDIPOINTS *pCandi;

	if(nWidth>4096)
	{
		if(nGap<256) nGap=256;
	}

	nCol=(nWidth-nStartX)/nGap;			if( (nWidth-nStartX)-nCol*nGap)		 nCol++;
	nRow=l_nPyramidHeight[0]/nGap;		if(l_nPyramidHeight[0]-nRow*nGap)	 nRow++;
	nNPitch=l_nPyramidWidth[0]/nGap;	if(l_nPyramidWidth[0]>nNPitch*nGap)  nNPitch++;
	g_CandiW.m_nCandiCount=nCol*nRow;
	g_CandiB.m_nCandiCount=nCol*nRow;
	g_CandiArea.m_nCandiCount=nCol*nRow;

	if(g_Param.m_dScaleFactorY>0) nLimitY=MARKING_Y_OFFSET/(g_Param.m_dScaleFactorY);

#ifdef FIND_TINYDEFECT
	for(m=0;m<4;m++)
#else
	for(m=0;m<3;m++)
#endif
	{
		if(m==0)
		{
			memcpy(&l_CandiTemp, &g_CandiW, sizeof(CANDIPOINTS));
			memset(&g_CandiW, 0, sizeof(CANDIPOINTS));
			pCandi=&g_CandiW;
		}
		else if(m==1)
		{
			memcpy(&l_CandiTemp, &g_CandiB, sizeof(CANDIPOINTS));
			memset(&g_CandiB, 0, sizeof(CANDIPOINTS));
			pCandi=&g_CandiB;
		}		
		else if(m==2)
		{
			memcpy(&l_CandiTemp, &g_CandiArea, sizeof(CANDIPOINTS));
			memset(&g_CandiArea, 0, sizeof(CANDIPOINTS));
			pCandi=&g_CandiArea;
		}		
		else if(m==3) //흑 Tiny
		{
			memcpy(&l_CandiTemp, &g_CandiTinyB, sizeof(CANDIPOINTS));
			memset(&g_CandiTinyB, 0, sizeof(CANDIPOINTS));
			pCandi=&g_CandiTinyB;
		}		

		 
		nMaxLoop= 1024;   //기존 MAX_DEFECT(10)*20

		for(k=0;k<nMaxLoop;k++)
		{
			
			nMax=0;
			for(i=0;i<nRow;i++)
				for(j=0;j<nCol;j++)
				{
					if(m==2)  //면적으로 하는 것은  l_CandiTemp.m_nCandiAvg[nNPitch*i+j] 사용하지 않음.
					{	
							nTmp=l_CandiTemp.m_nCandiValue[nNPitch*i+j];
					}
					else
					{
							nTmp=l_CandiTemp.m_nCandiValue[nNPitch*i+j]-l_CandiTemp.m_nCandiAvg[nNPitch*i+j];
							if(nTmp<0) nTmp*=-1;
					}

					if(nTmp>nMax)
					{
						nMax=nTmp;
						nPos=nNPitch*i+j;
					}
				}
			
			//중복 제외----------------------------------------------------------------------
			if(nMax>0)
			{
				nOverlap=0;

				nPosX1=(int)((g_Param.m_dCamStartPosX+l_CandiTemp.m_nCandiX[nPos]*g_Param.m_dScaleFactorX)/MARKING_X_OFFSET);
				for(i=0;i<k;i++)
				{
					nPosX2=(int)((g_Param.m_dCamStartPosX+pCandi->m_nCandiX[i]*g_Param.m_dScaleFactorX)/MARKING_X_OFFSET);
					if(nPosX1==nPosX2 && (int)(pCandi->m_nCandiY[i]/nLimitY)==(int)(l_CandiTemp.m_nCandiY[nPos]/nLimitY) )
					{
						nOverlap=1;
						break;
					}
				}
				if(nOverlap)
				{
					l_CandiTemp.m_nCandiValue[nPos]=l_CandiTemp.m_nCandiAvg[nPos];
					continue;
				}
			}
			//--------------------------------------------------------------------------------


			if(nMax>0)
			{
				pCandi->m_nCandiValue[pCandi->m_nCandiCount]	=l_CandiTemp.m_nCandiValue[nPos];
				pCandi->m_nCandiX[pCandi->m_nCandiCount]		=l_CandiTemp.m_nCandiX[nPos];
				pCandi->m_nCandiY[pCandi->m_nCandiCount]		=l_CandiTemp.m_nCandiY[nPos];
				pCandi->m_nCandiAvg[pCandi->m_nCandiCount]		=l_CandiTemp.m_nCandiAvg[nPos];
				pCandi->m_nCandiCount++;
				if(pCandi->m_nCandiCount>=g_System.m_nMaxCandiDefect) break;

				l_CandiTemp.m_nCandiValue[nPos]=l_CandiTemp.m_nCandiAvg[nPos];
			}
		}		
	}

	return 0;
}

//m_nShadowTh
//nType=0 : White만
//nType=1 : Black만
//nType=2 : WHite Black포함

//nAveValue: 평균밝기가 주어지면 이값을 이용하고 0 이면 영역의 중간값을 이용한다.(Histo 중간값)
//nAttachPixel: 붙일 Pixel수(반드시 짝수)
//nSizeMethod: 0 (가로, 세로, (가로+세로)/2), 1 (장축, 단축, 장축)   2 (장축, 단축, (장축+단축)/2) 

//return : *pValueUp : (최대값-평균)
//         *pValueDn : (평균값-최소)
//         *pSize    : [0]: x 또는 장축   [1]:Y 또는 단축   [2]: Size  [3]: 군집갯수 [4]:군집Size


void GetSizeNValue(int nType, LPBYTE fm, int nDefectY, int left, int top, int width, int height, int pitch, int nAveValue, int nThUp, int nThDn, int nAttachPixel, int nSizeMethod, 
					double dScaleX, double dScaleY, int *pValueUp, int *pValueDn, double *pSize, double *pImulKipoSize, int *pCx, int *pCy)
{
	int i, j;
	int nAve=nAveValue;
	int nTh,  nMaxValue, nMinValue;
	BYTE fmBin[DEFECT_IMAGE_SIZE*DEFECT_IMAGE_SIZE];
	BYTE fmDilate[DEFECT_IMAGE_SIZE*DEFECT_IMAGE_SIZE];
	int nBlob, nBlobID;
	int ll,tt,rr,bb, nCenterX, nCenterY=-1, nCenterXB, nCenterYB=-1;
	int nDilation=nAttachPixel/2;
	int nSumT=0, nSumB=0, nUsePrev=0;
	double dArea, dMaxArea, cx, cy, angle, pos[4][2], dXDist, dYDist, dist1, dist2;
	double dShadowX, dShadowY, dShadowSize;
	double dSizeX, dSizeY, dSize, dGroupSize;
	int nGroupCount;


	g_Temp.m_nUsePrevImage=0;
	*pValueUp=*pValueUp=pSize[0]=pSize[1]=pSize[2]=*pImulKipoSize=0;	
	*pCx=left+width/2;
	*pCy=top+height/2;

	if(nAttachPixel==1) nDilation=1; //한Pixel붙이는 걸로 되어 있으면 Dilation한번한다.

	if(width!=DEFECT_IMAGE_SIZE)  width=DEFECT_IMAGE_SIZE;
	if(height!=DEFECT_IMAGE_SIZE) height=DEFECT_IMAGE_SIZE;
	

	//평균밝기 다시구하기--------------------------------------------------------------
	if(g_Temp.m_nPCOptic==COS4)	//정투과는 평활화 값을 그냥사용한다.
	{
		nAve=g_System.m_nFlatBright;
	}
	else
	{
		if(nAveValue==0)  
		{
			nAve=GetAveValueByHistogram(fm, left, top, width, height, pitch);  

			if(nAve-g_System.m_nFlatBright>=50)			//Big White의 경우  영역평균값이 평활화 값보다 50이상 밝으면 평활화 값을 사용한다.
				nAve=g_System.m_nFlatBright;			
			else if(g_System.m_nFlatBright-nAve>=30)    //Big Black의 경우  영역평균값이 평활화 값보다 30이상 어두우면 평활화 값을 사용한다.
				nAve=g_System.m_nFlatBright;
		}
	}

	//---------------------------------------------------------------------------------

#if INSPECT_TYPE==TAC_LINE
	if(g_System.m_nLineCAM==1 && nDefectY<32 && top==0) //라인스캔이고, 불량이 <64에 있으면 직전 Frame 끝 32Pixel데이타를 이용한다.
	{
		bb=height-32;
		for(j=left;j<left+width;j++)
		{
			nSumT+=*(g_fmPrev+j);
			nSumB+=*(fm+pitch*bb+j);
		}
		if(abs(nSumT-nSumB)<DEFECT_IMAGE_SIZE*3)	 //맨위와 맨 아래 평균밝기가 3미만이면 직전 Frame 32 Pixel사용한다.	
			nUsePrev=1;
		
		if(nUsePrev)
		{
			nMaxValue=0;
			nMinValue=255;
			for(i=0;i<32-1;i++)
				for(j=left;j<left+width-1;j++)
				{
					nTh=(*(g_fmPrev+pitch*i+j)+*(g_fmPrev+pitch*i+j+1)+*(g_fmPrev+pitch*i+j+pitch)+*(g_fmPrev+pitch*i+j+pitch+1))/4;
					if(nTh>nMaxValue) nMaxValue=nTh;				
					else if(nTh<nMinValue) nMinValue=nTh;
				}
			for(i=0;i<height-31;i++)
				for(j=left;j<left+width-1;j++)
				{
					nTh=(*(fm+pitch*i+j)+*(fm+pitch*i+j+1)+*(fm+pitch*i+j+pitch)+*(fm+pitch*i+j+1+pitch))/4;
					if(nTh>nMaxValue) nMaxValue=nTh;				
					else if(nTh<nMinValue) nMinValue=nTh;	
				}
		}
	}	

	if(nUsePrev==0)
	{
		//최대 최소 구함------------------------------------------------------------------
		nMaxValue=0;
		nMinValue=255;
		for(i=top;i<top+height-1;i++)
			for(j=left;j<left+width-1;j++)
			{
				nTh=(*(fm+pitch*i+j)+*(fm+pitch*i+j+1)+*(fm+pitch*i+j+pitch)+*(fm+pitch*i+j+1+pitch))/4;
				if(nTh>nMaxValue) nMaxValue=nTh;				
				else if(nTh<nMinValue) nMinValue=nTh;				
			}
			
	}

	*pValueUp=nMaxValue-nAve;  if(*pValueUp>127) *pValueUp=127;   //평활화하면서 밝기가 비율로 떨어지므로
	*pValueDn=nAve-nMinValue;
#else
	if(g_System.m_nLineCAM==1 && nDefectY<32 && top==0) //라인스캔이고, 불량이
	{
		bb=height-32;
		for(j=left;j<left+width;j++)
		{
			nSumT+=*(g_fmPrev+j);
			nSumB+=*(fm+pitch*bb+j);
		}
		if(abs(nSumT-nSumB)<DEFECT_IMAGE_SIZE*3)	 //맨위와 맨 아래 평균밝기가 3미만이면 직전 Frame 32 Pixel사용한다.	
			nUsePrev=1;

		//
		if(nUsePrev)
		{
			nSumT=*(g_fmPrev+left)+*(g_fmPrev+pitch+left)+*(g_fmPrev+pitch*2+left);
			nSumB=*(g_fmPrev+left+width-1)+*(g_fmPrev+pitch+left+width-1)+*(g_fmPrev+pitch*2+left+width-1);
			if(abs(nSumT-nSumB)>=15) nUsePrev=0;
		}
		
		if(nUsePrev)
		{
			nMaxValue=0;
			nMinValue=255;
			for(i=0;i<32;i++)
				for(j=left;j<left+width;j++)
				{
					nTh=*(g_fmPrev+pitch*i+j);
					if(nTh>nMaxValue)
					{
						nMaxValue=nTh;
						nCenterX=j-left;
						nCenterY=i-32;
					}
					else if(nTh<nMinValue)
					{
						nMinValue=nTh;
						nCenterXB=j-left;
						nCenterYB=i-top;
					}
				}
			for(i=0;i<height-32;i++)
				for(j=left;j<left+width;j++)
				{
					nTh=*(fm+pitch*i+j);
					if(nTh>nMaxValue)
					{
						nMaxValue=nTh;
						nCenterX=j-left;
						nCenterY=i-top;
					}
					else if(nTh<nMinValue)
					{
						nMinValue=nTh;
						nCenterXB=j-left;
						nCenterYB=i-top;
					}
				}
		}
	}
	if(nUsePrev==0)
	{
		//최대 최소 구함------------------------------------------------------------------
		nMaxValue=0;
		nMinValue=255;
		for(i=top;i<top+height;i++)
			for(j=left;j<left+width;j++)
			{
				nTh=*(fm+pitch*i+j);
				if(nTh>nMaxValue)
				{
					nMaxValue=nTh;
					nCenterX=j-left;
					nCenterY=i-top;
				}
				else if(nTh<nMinValue)
				{
					nMinValue=nTh;
					nCenterXB=j-left;
					nCenterYB=i-top;
				}
			}
	}

	*pValueUp=nMaxValue-nAve;
	*pValueDn=nAve-nMinValue;
#endif

	//Binary----------------------------------------------------------------------------
	memset(fmBin, 0, DEFECT_IMAGE_SIZE*DEFECT_IMAGE_SIZE);
	if(nType==0 || nType==2)  //백점+찍힘
	{
		if(nType==0)
		{
			if(nUsePrev)
			{
				nTh =nAve+nThUp-1; //
				for(i=0;i<32;i++)
					for(j=0;j<DEFECT_IMAGE_SIZE;j++)
						if(*(g_fmPrev+pitch*i+(j+left))>nTh) *(fmBin+DEFECT_IMAGE_SIZE*i+j)=255;
				for(i=32;i<DEFECT_IMAGE_SIZE;i++)
					for(j=0;j<DEFECT_IMAGE_SIZE;j++)
						if(*(fm+pitch*(i-32)+(j+left))>nTh) *(fmBin+DEFECT_IMAGE_SIZE*i+j)=255;

			}
			else
			{
				nTh=nAve+nThUp-1; //
				for(i=0;i<DEFECT_IMAGE_SIZE;i++)
					for(j=0;j<DEFECT_IMAGE_SIZE;j++)
						if(*(fm+pitch*(i+top)+(j+left))>nTh) *(fmBin+DEFECT_IMAGE_SIZE*i+j)=255;
			}
		}
		else
		{
			if(nUsePrev)
			{
				nTh =nAve+nThUp-1; //
				for(i=0;i<32;i++)
					for(j=0;j<DEFECT_IMAGE_SIZE-1;j++)
						if((*(g_fmPrev+pitch*i+(j+left))+*(g_fmPrev+pitch*i+(j+left)))/2>nTh) *(fmBin+DEFECT_IMAGE_SIZE*i+j)=255;
				for(i=32;i<DEFECT_IMAGE_SIZE-1;i++)
					for(j=0;j<DEFECT_IMAGE_SIZE-1;j++)
						if((*(fm+pitch*(i-32)+(j+left))+*(fm+pitch*(i-32)+(j+left+1))+*(fm+pitch*(i-32+1)+(j+left))+*(fm+pitch*(i-32+1)+(j+left+1)))/4>nTh) *(fmBin+DEFECT_IMAGE_SIZE*i+j)=255;

			}
			else
			{
				nTh=nAve+nThUp-1; //
				for(i=0;i<DEFECT_IMAGE_SIZE-1;i++)
					for(j=0;j<DEFECT_IMAGE_SIZE-1;j++)
						if((*(fm+pitch*(i+top)+(j+left))+*(fm+pitch*(i+top)+(j+left+1))+*(fm+pitch*(i+top+1)+(j+left))+*(fm+pitch*(i+top+1)+(j+left+1)))/4>nTh) *(fmBin+DEFECT_IMAGE_SIZE*i+j)=255;
			}
			for(i=0;i<DEFECT_IMAGE_SIZE-1;i++)
				*(fmBin+DEFECT_IMAGE_SIZE*i+DEFECT_IMAGE_SIZE-1)=*(fmBin+DEFECT_IMAGE_SIZE*i+DEFECT_IMAGE_SIZE-2);
			for(j=0;j<DEFECT_IMAGE_SIZE;j++)
				*(fmBin+DEFECT_IMAGE_SIZE*(DEFECT_IMAGE_SIZE-1)+j)=*(fmBin+DEFECT_IMAGE_SIZE*(DEFECT_IMAGE_SIZE-2)+j);
		}
	}
	if(nType==1 || nType==2) //흑점+찍힘
	{
		if(nType==1)
		{
			if(nUsePrev)
			{
				nTh=nAve-nThDn+1; //
				for(i=0;i<32;i++)
					for(j=0;j<DEFECT_IMAGE_SIZE;j++)
						if(*(g_fmPrev+pitch*i+(j+left))<nTh) 
							*(fmBin+DEFECT_IMAGE_SIZE*i+j)=255;
				for(i=32;i<DEFECT_IMAGE_SIZE;i++)
					for(j=0;j<DEFECT_IMAGE_SIZE;j++)
						if(*(fm+pitch*(i-32)+(j+left))<nTh) 
							*(fmBin+DEFECT_IMAGE_SIZE*i+j)=255;
			}
			else
			{
				nTh=nAve-nThDn+1; //
				for(i=0;i<DEFECT_IMAGE_SIZE;i++)
					for(j=0;j<DEFECT_IMAGE_SIZE;j++)
						if(*(fm+pitch*(i+top)+(j+left))<nTh) 
							*(fmBin+DEFECT_IMAGE_SIZE*i+j)=255;
			}
		}
		else
		{
			if(nUsePrev)
			{
				nTh=nAve-nThDn+1; //
				for(i=0;i<32;i++)
					for(j=0;j<DEFECT_IMAGE_SIZE-1;j++)
						if((*(g_fmPrev+pitch*i+(j+left))+*(g_fmPrev+pitch*i+(j+left)))/2<nTh) 
							*(fmBin+DEFECT_IMAGE_SIZE*i+j)=255;
				for(i=32;i<DEFECT_IMAGE_SIZE-1;i++)
					for(j=0;j<DEFECT_IMAGE_SIZE-1;j++)
						if((*(fm+pitch*(i-32)+(j+left))+*(fm+pitch*(i-32)+(j+left+1))+*(fm+pitch*(i-32+1)+(j+left))+*(fm+pitch*(i-32+1)+(j+left+1)))/4<nTh) 
							*(fmBin+DEFECT_IMAGE_SIZE*i+j)=255;
			}
			else
			{
				nTh=nAve-nThDn+1; //
				for(i=0;i<DEFECT_IMAGE_SIZE-1;i++)
					for(j=0;j<DEFECT_IMAGE_SIZE-1;j++)
						if((*(fm+pitch*(i+top)+(j+left))+*(fm+pitch*(i+top)+(j+left+1))+*(fm+pitch*(i+top+1)+(j+left))+*(fm+pitch*(i+top+1)+(j+left+1)))/4<nTh) 
							*(fmBin+DEFECT_IMAGE_SIZE*i+j)=255;
			}
			for(i=0;i<DEFECT_IMAGE_SIZE-1;i++)
				*(fmBin+DEFECT_IMAGE_SIZE*i+DEFECT_IMAGE_SIZE-1)=*(fmBin+DEFECT_IMAGE_SIZE*i+DEFECT_IMAGE_SIZE-2);
			for(j=0;j<DEFECT_IMAGE_SIZE;j++)
				*(fmBin+DEFECT_IMAGE_SIZE*(DEFECT_IMAGE_SIZE-1)+j)=*(fmBin+DEFECT_IMAGE_SIZE*(DEFECT_IMAGE_SIZE-2)+j);
		}
	}
	g_Temp.m_nUsePrevImage=nUsePrev;
	//-----------------------------------------------------------------------------------

	//이웃한 Pixel붙이기------------------------------------------------------------------------------------------------------------	
	if(nDilation) 
	{
		if(nType==2) BinaryDilate(fmBin, fmDilate, 0,0,DEFECT_IMAGE_SIZE-1,DEFECT_IMAGE_SIZE-1,DEFECT_IMAGE_SIZE, 1, nDilation*2+1);
		else		 BinaryDilate(fmBin, fmDilate, 0,0,DEFECT_IMAGE_SIZE-1,DEFECT_IMAGE_SIZE-1,DEFECT_IMAGE_SIZE, nDilation*2+1, nDilation*2+1);
	}
	else
		memcpy(fmDilate, fmBin, DEFECT_IMAGE_SIZE*DEFECT_IMAGE_SIZE);
	

	//Blob----------------------------------------------------------------------------------------------------
	g_Chain.SetChainData(1, fmDilate, 1, 1, 1, 2000, DEFECT_IMAGE_SIZE, DEFECT_IMAGE_SIZE);
	nBlob=g_Chain.FastChain(nDilation,nDilation,DEFECT_IMAGE_SIZE-1-nDilation, DEFECT_IMAGE_SIZE-1-nDilation);			     


	if(nBlob>0)
	{
		dMaxArea=0;
		for(i=0;i<nBlob;i++)
		{
			dArea=g_Chain.Chain_Area(i);
			if(dArea>=dMaxArea)//최대 불량 사이즈.
			{
				nBlobID = i;
				dMaxArea = dArea;
			}
		}
		if(dMaxArea<0.1) return;
	
		if(nType==2)
		{
			ll	= g_Chain.FindMinX(nBlobID);
			rr	= g_Chain.FindMaxX(nBlobID);
		}
		else
		{
			ll	= g_Chain.FindMinX(nBlobID)+nDilation;
			rr	= g_Chain.FindMaxX(nBlobID)-nDilation;
		}
		tt	= g_Chain.FindMinY(nBlobID)+nDilation;
		bb	= g_Chain.FindMaxY(nBlobID)-nDilation;

		pSize[0]=(rr-ll+1)*dScaleX;
		pSize[1]=(bb-tt+1)*dScaleY;
		pSize[2]=(pSize[0]+pSize[1])*0.5;
		if(nSizeMethod)
			pSize[2]=sqrt(  pSize[0]*pSize[0] + pSize[1]*pSize[1] );

		nCenterX=(ll+rr)/2;
		nCenterY=(tt+bb)/2;	

#ifdef TOT_USE

		//SHADOW 크기체크
		if(g_Param.m_nShadowTh>0)  //그림자 TH가 있으면 그림자 찾는다.
		{

			if(nType==0 && fm==g_fmFlat[g_ID])
			{
				nTh =nAve+g_Param.m_nShadowTh-1; //
				memset(fmBin, 0, DEFECT_IMAGE_SIZE*DEFECT_IMAGE_SIZE);			
				
				if(nUsePrev)
				{					
					for(i=0;i<32;i++)
						for(j=0;j<DEFECT_IMAGE_SIZE;j++)
							if(*(g_fmPrev+pitch*i+(j+left))>nTh) *(fmBin+DEFECT_IMAGE_SIZE*i+j)=255;
					for(i=32;i<DEFECT_IMAGE_SIZE;i++)
						for(j=0;j<DEFECT_IMAGE_SIZE;j++)
							if(*(fm+pitch*(i-32)+(j+left))>nTh) *(fmBin+DEFECT_IMAGE_SIZE*i+j)=255;

				}
				else
				{
					for(i=0;i<DEFECT_IMAGE_SIZE;i++)
						for(j=0;j<DEFECT_IMAGE_SIZE;j++)
							if(*(fm+pitch*(i+top)+(j+left))>nTh) *(fmBin+DEFECT_IMAGE_SIZE*i+j)=255;
				}

				g_Chain.SetChainData(1, fmBin, 1, 1, 1, 2000, DEFECT_IMAGE_SIZE, DEFECT_IMAGE_SIZE);


				ll=ll-g_Param.m_nShadowOffset-1;  if(ll<0) ll=0;
				tt=tt-g_Param.m_nShadowOffset-1;  if(tt<0) tt=0;
				rr=rr+g_Param.m_nShadowOffset+1;  if(rr>=DEFECT_IMAGE_SIZE-1) rr=DEFECT_IMAGE_SIZE-1;
				bb=bb+g_Param.m_nShadowOffset+1;  if(bb>=DEFECT_IMAGE_SIZE-1) bb=DEFECT_IMAGE_SIZE-1;

				nBlob=g_Chain.FastChain(ll,tt,rr, bb);			     

				if(nBlob>0)
				{
					dMaxArea=0;
					nBlobID=-1;
					for(i=0;i<nBlob;i++)
					{
						dArea=g_Chain.Chain_Area(i);
						if(dArea>=dMaxArea)//최대 불량 사이즈.
						{
							nBlobID = i;
							dMaxArea = dArea;
						}
					}
					if(dMaxArea<0.1) return;
	
					if(nBlobID>=0)
					{
						ll	= g_Chain.FindMinX(nBlobID);
						rr	= g_Chain.FindMaxX(nBlobID);					
						tt	= g_Chain.FindMinY(nBlobID);
						bb	= g_Chain.FindMaxY(nBlobID);
					}

				}

				dShadowX=(rr-ll+1)*dScaleX;
				dShadowY=(bb-tt+1)*dScaleY;
				dShadowSize=(dShadowX+dShadowY)/2-pSize[2];
				if(nSizeMethod)
					dShadowSize=sqrt(  dShadowX*dShadowX + dShadowY*dShadowY )-pSize[2];

				if(dShadowSize>=g_Param.m_dShowSkipSize && pSize[2]<g_Param.m_dShowdowNotSkipSize) //그림자 SKIP 조건 충족
				{
					//군집조건 체크

				}
			}
		}
		if(g_Param.m_nGroupAreaTh>0 && g_Param.m_nGroupCount>0  && g_Param.m_dGroupSize>0)
		{
			nTh =nAve+g_Param.m_nGroupAreaTh-1; //
			memset(fmBin, 0, DEFECT_IMAGE_SIZE*DEFECT_IMAGE_SIZE);			
				
			if(nUsePrev)
			{					
				for(i=0;i<32;i++)
					for(j=0;j<DEFECT_IMAGE_SIZE;j++)
						if(*(g_fmPrev+pitch*i+(j+left))>nTh) *(fmBin+DEFECT_IMAGE_SIZE*i+j)=255;
				for(i=32;i<DEFECT_IMAGE_SIZE;i++)
					for(j=0;j<DEFECT_IMAGE_SIZE;j++)
						if(*(fm+pitch*(i-32)+(j+left))>nTh) *(fmBin+DEFECT_IMAGE_SIZE*i+j)=255;

			}
			else
			{
				for(i=0;i<DEFECT_IMAGE_SIZE;i++)
					for(j=0;j<DEFECT_IMAGE_SIZE;j++)
						if(*(fm+pitch*(i+top)+(j+left))>nTh) *(fmBin+DEFECT_IMAGE_SIZE*i+j)=255;
			}

			g_Chain.SetChainData(1, fmBin, 1, 1, 1, 2000, DEFECT_IMAGE_SIZE, DEFECT_IMAGE_SIZE);
			nBlob=g_Chain.FastChain(0,0,DEFECT_IMAGE_SIZE-1, DEFECT_IMAGE_SIZE-1);

			nGroupCount=0;
			dGroupSize=0;
			for(i=0;i<nBlob;i++)
			{
				dArea=g_Chain.Chain_Area(i);
				if(dArea<0) continue;
	
				ll	= g_Chain.FindMinX(nBlobID);
				rr	= g_Chain.FindMaxX(nBlobID);					
				tt	= g_Chain.FindMinY(nBlobID);
				bb	= g_Chain.FindMaxY(nBlobID);
					
				dSizeX=(rr-ll+1)*dScaleX;
				dSizeY=(bb-tt+1)*dScaleY;
				dSize =(dSizeX+dSizeY)/2;
				if(g_Param.m_nGroupSizeMethod)
					dSize=sqrt(  dSizeX*dSizeX + dSizeY*dSizeY );

				if(dSize<g_Param.m_dGroupSize) continue;

				dist1=nCenterX-(ll+rr)/2; if(dist1<0) dist1*=-1;
				dist2=nCenterY-(tt+bb)/2; if(dist2<0) dist2*=-1;

				if(dist1<=g_Param.m_nGroupArea && dist2<=g_Param.m_nGroupArea)
				{
					nGroupCount++;
					dGroupSize+=dSize;
				}
			}
			if(nGroupCount>=g_Param.m_nGroupCount)
			{
				pSize[3]=nGroupCount;
				pSize[4]=dGroupSize;
			}
		}
#else
		if(nSizeMethod) //0: X, Y구해서 (X+Y)/2   1: 장축, 단축 구해서 장축사용  2: 장축, 단축 구해서 (장축+단축)/2
		{               //장축, 단축 구할때만 여기 들어옴
			if(rr-ll>=2 && bb-tt>=2)
			{
				g_Chain.Chain_Center(nBlobID, &cx, &cy);
				angle=g_Chain.FindAngle(nBlobID);
				if(fabs(angle)<1)
				{
					if(nSizeMethod==2) pSize[2]=(pSize[0]+pSize[1])*0.5;
					else			   pSize[2] =pSize[0];
				}
				else if( fabs(fabs(angle)-90) <1)
				{
					if(nSizeMethod==2) pSize[2]=(pSize[0]+pSize[1])*0.5;
					else			   pSize[2] =pSize[1];
				}
				else
				{
					g_Chain.FindMinRect(nBlobID, cx, cy, angle,  pos);
					dXDist=(fabs(pos[0][0]-pos[1][0])-nDilation*2)*dScaleX; if(dXDist<0) dXDist=dScaleX;
					dYDist=(fabs(pos[0][1]-pos[1][1])-nDilation*2)*dScaleY; if(dYDist<0) dYDist=dScaleY;
					dist1=sqrt(dXDist*dXDist+dYDist*dYDist);
					dXDist=(fabs(pos[0][0]-pos[3][0])-nDilation*2)*dScaleX; if(dXDist<0) dXDist=dScaleX;
					dYDist=(fabs(pos[0][1]-pos[3][1])-nDilation*2)*dScaleY; if(dYDist<0) dYDist=dScaleY;
					dist2=sqrt(dXDist*dXDist+dYDist*dYDist);
							 

					if(dist1>dist2)
					{
						pSize[0] =dist1;
						pSize[1] =dist2;
						if(nSizeMethod==2) pSize[2]=(dist1+dist2)*0.5;
						else			   pSize[2] =dist1;
					}
					else
					{
						pSize[0] =dist2;
						pSize[1] =dist1;
						if(nSizeMethod==2) pSize[2]=(dist1+dist2)*0.5;
						else               pSize[2] =dist2;
					}
				}
			}
			else
			{
				if(pSize[1]>pSize[0])
				{
					dist1=pSize[1];
					pSize[1]=pSize[0];
					pSize[0]=dist1;
				}
				if(nSizeMethod==2)  pSize[2]=(pSize[0]+pSize[1])*0.5;
				else				pSize[2]=pSize[0];
			}
		}
#endif

		//찍힘이면 Value 다시 구함------------------------------------------------------------------------
		if(nType==2)
		{
			ll-=2; if(ll<0) ll=0;
			tt-=2; if(tt<0) tt=0;
			rr+=2; if(rr>=DEFECT_IMAGE_SIZE-1) rr=DEFECT_IMAGE_SIZE-2;
			bb+=2; if(bb>=DEFECT_IMAGE_SIZE-1) bb=DEFECT_IMAGE_SIZE-2;
			

			nMaxValue=0;
			nMinValue=255;
			for(i=top+tt;i<top+bb;i++)
				for(j=left+ll;j<left+rr;j++)
				{
					nTh=(*(fm+pitch*i+j)+*(fm+pitch*i+j+1)+*(fm+pitch*(i+1)+j)+*(fm+pitch*(i+1)+j+1))/4;
					if(nTh>nMaxValue)		nMaxValue=nTh;
					else if(nTh<nMinValue)  nMinValue=nTh;
				}
			*pValueUp=nMaxValue-nAve;
			*pValueDn=nAve-nMinValue;
		}
		//------------------------------------------------------------------------------------------------	
		*pCx=left+(ll+rr)/2;
		*pCy=top+(tt+bb)/2;
	}
	//-------------------------------------------------------------------------------------------------------


	//COS4에서 사용하는 이물기포 찾기 -----------------------------------------------------------------------------------------------------------------
/*	if(g_Param.m_dKipoBrightDiff>=1 && nType==1 && *pValueDn>=g_Param.m_nKipoMinValue)
	{
		int nX, nY1, nY2, nLowCount, nSum, nBlobCount, nMaxPos, nKipoY1, nKipoY2, nKipoLowCount, nY1Min, nY2Max, nFoundImulKipo, nCenterOffset=1;
		double dAvg1, dAvg2, area, dMaxArea=0;

		nY1=nCenterY-60; if(nY1<0)				nY1=0;
		nY2=nCenterY+60; if(nY2>=height)		nY2=height-1;
		nX =nCenterX-6;  if(nX<0)				nX =0;  
						 else if(nX+12>=width) nX =width-12;
		
		nLowCount=0;
		nSum=0;
		for(j=left+nX;j<left+nX+12;j++)
			nSum+=*(fm+pitch*nY1+j);
		dAvg1=nSum/12.0;

		nSum=0;
		for(j=left+nX;j<left+nX+12;j++)
			nSum+=*(fm+pitch*nY2+j);
		dAvg2=nSum/12.0;
		if(dAvg2<dAvg1) dAvg1=dAvg2;
		
		nCenterY=(nY2-nY1)/2;
		for(i=top+nY1;i<top+nY2;i++)
		{
			for(j=left+nX;j<left+nX+12;j++)
			{
				if( (dAvg1-*(fm+pitch*i+j)) > g_Param.m_dKipoBrightDiff)
					*(fmBin+14*(i-nY1-top)+(j-nX-left)+1)=255;
				else
					*(fmBin+14*(i-nY1-top)+(j-nX-left)+1)=0;									 
			}
		}
		g_Chain.SetChainData(1, fmBin, 2,2,10,10000,14,(nY2-nY1));
		nBlobCount = g_Chain.FastChain(0, 0, 14-1,(nY2-nY1)-1);
		nMaxPos=-1;
		for(i=0;i<nBlobCount;i++)
		{
			//X폭이 6Pixel미만이면 버림----
			nY1=g_Chain.FindMinX(i);
			nY2=g_Chain.FindMaxX(i);
			if(nY2-nY1+1<6) continue;
			//-----------------------------

			nY1=g_Chain.FindMinY(i);
			nY2=g_Chain.FindMaxY(i);
			if(nCenterY>=nY1 && nCenterY<=nY2)
			{
				area=g_Chain.Chain_Area(i);
				if(area>dMaxArea)
				{
					dMaxArea=area;
					nMaxPos=i;
				}
			}
		}

		if(nMaxPos>=0)
		{		
			nY1=g_Chain.FindMinY(nMaxPos);
			nY2=g_Chain.FindMaxY(nMaxPos);
			
			nKipoY1=nKipoY2=-1;				
			for(i=nY1;i<nY2;i++)
			{
				nKipoLowCount=0;
				for(j=1;j<14-1;j++)
					if(*(fmBin+14*i+j)>0) nKipoLowCount++;
				
				if(nKipoLowCount>=6)
				{
					nKipoY1=i;
					break;
				}
			}

			for(i=nY2-1;i>=nY1;i--)
			{
				nKipoLowCount=0;
				for(j=1;j<14-1;j++)
					if(*(fmBin+14*i+j)>0) nKipoLowCount++;
				
				if(nKipoLowCount>=6)
				{
					nKipoY2=i;
					break;				
				}
			}

			nLowCount=(nKipoY2-nKipoY1);
			if(nLowCount<0) nLowCount=0;			
		}
		else //중심이 Blob사이에 없는 경우, 위/아래 Blob을 구한다.
		{
			nY1Min=10000;
			nY2Max=0;
			for(i=0;i<nBlobCount;i++)
			{
				//X폭이 8Pixel미만이면 버림----
				nY1=g_Chain.FindMinX(i);
				nY2=g_Chain.FindMaxX(i);
				if(nY2-nY1+1<8) continue;
				//-----------------------------

				nY1=g_Chain.FindMinY(i);
				nY2=g_Chain.FindMaxY(i);
				if(nY1<nY1Min) nY1Min=nY1;
				if(nY2>nY2Max) nY2Max=nY2;				
			}
			if(nCenterY>=nY1Min && nCenterY<=nY2Max)
			{
				if(abs(nCenterY-(nY1Min+nY2Max)/2)<=7+nCenterOffset)
					nLowCount=nY2Max-nY1Min;
			}
		}
		

		nFoundImulKipo=0;

		*pImulKipoSize=nLowCount*g_Param.m_dScaleFactorX;
		for(i=0;i<10;i++)
		{
			if(g_Param.m_nUseLevel[i] && (g_Param.m_nKipoValue[i]>0 || g_Param.m_dKipoSize[i]>0) && *pImulKipoSize>=g_Param.m_dKipoSize[i] && *pValueDn>=g_Param.m_nKipoValue[i])
			{
				nFoundImulKipo=1;
			}
		}
		if(nFoundImulKipo==0) *pImulKipoSize=0;
	}
*/
}


void GetSizeNValueTinyB(LPBYTE fm, int left, int top, int width, int height, int pitch, int nAveValue, int nThDn, int nAttachPixel, int nSizeMethod, double dScaleX, double dScaleY, int *pValueDn, double *pSize)
{
	int i, j;
	int nAve=nAveValue;
	int nTh, nMaxValue, nMinValue;
	BYTE fmBin[DEFECT_IMAGE_SIZE*DEFECT_IMAGE_SIZE];
	BYTE fmDilate[DEFECT_IMAGE_SIZE*DEFECT_IMAGE_SIZE];
	int nBlob, nBlobID;
	int ll,tt,rr,bb, nCenterX, nCenterY;
	int nDilation=nAttachPixel/2;
	int nSumT=0, nSumB=0, nUsePrev=0;
	int nTinyOff=10;
	double dArea, dMaxArea, cx, cy, angle, pos[4][2], dXDist, dYDist, dist1, dist2;

	g_Temp.m_nUsePrevImage=0;
	*pValueDn=pSize[0]=pSize[1]=pSize[2]=0;	

	if(nAttachPixel==1) nDilation=1; //한Pixel붙이는 걸로 되어 있으면 Dilation한번한다.

	if(width!=DEFECT_IMAGE_SIZE/2)  width=DEFECT_IMAGE_SIZE/2;
	if(height!=DEFECT_IMAGE_SIZE/2) height=DEFECT_IMAGE_SIZE/2;
	
	//최소 구함------------------------------------------------------------------
	nMaxValue=0;
	nMinValue=255;
	for(i=top;i<top+height;i++)
		for(j=left;j<left+width;j++)
		{
			nTh=*(fm+pitch*i+j);
			
			if(nTh<nMinValue)
			{
				nMinValue=nTh;
				nCenterX=j-left;
				nCenterY=i-top;
			}
		}
	

	//평균밝기 다시구하기--------------------------------------------------------------
	if(g_Temp.m_nPCOptic==COS4)
	{
		nAve=g_System.m_nFlatBright;
	}
	else
	{
		if(nAveValue==0)  
		{
			nAve=GetAveValueByHistogram(fm, left, top, width, height, pitch);  

			if(abs(nAve-g_System.m_nFlatBright)>=60)
				nAve=g_System.m_nFlatBright;			
		}
	}

	*pValueDn=nAve-nMinValue;

	//Binary----------------------------------------------------------------------------
	memset(fmBin, 0, DEFECT_IMAGE_SIZE*DEFECT_IMAGE_SIZE);
			
	for(i=0;i<DEFECT_IMAGE_SIZE/2;i++)
		for(j=0;j<DEFECT_IMAGE_SIZE/2;j++)
			if(*(fm+pitch*(i+top)+(j+left)+nTinyOff)-*(fm+pitch*(i+top)+(j+left))>=nThDn && *(fm+pitch*(i+top)+(j+left)-nTinyOff)-*(fm+pitch*(i+top)+(j+left))>=nThDn)
				*(fmBin+DEFECT_IMAGE_SIZE*i+j)=255;
	
	//-----------------------------------------------------------------------------------

	//이웃한 Pixel붙이기------------------------------------------------------------------------------------------------------------
	if(nDilation) 
		BinaryDilate(fmBin, fmDilate, 0,0,DEFECT_IMAGE_SIZE-1,DEFECT_IMAGE_SIZE-1,DEFECT_IMAGE_SIZE, nDilation*2+1, nDilation*2+1);
	else
		memcpy(fmDilate, fmBin, DEFECT_IMAGE_SIZE*DEFECT_IMAGE_SIZE);
	
	//Blob----------------------------------------------------------------------------------------------------
	g_Chain.SetChainData(1, fmDilate, 1, 1, 1, 2000, DEFECT_IMAGE_SIZE, DEFECT_IMAGE_SIZE);
	nBlob=g_Chain.FastChain(nDilation,nDilation,DEFECT_IMAGE_SIZE-1-nDilation, DEFECT_IMAGE_SIZE-1-nDilation);			     

	if(nBlob>0)
	{
		dMaxArea=0;
		for(i=0;i<nBlob;i++)
		{
			dArea=g_Chain.Chain_Area(i);
			if(dArea>=dMaxArea)//최대 불량 사이즈.
			{
				nBlobID = i;
				dMaxArea = dArea;
			}
		}
		if(dMaxArea<0.1) return;
	
		ll	= g_Chain.FindMinX(nBlobID)+nDilation;
		rr	= g_Chain.FindMaxX(nBlobID)-nDilation;
		tt	= g_Chain.FindMinY(nBlobID)+nDilation;
		bb	= g_Chain.FindMaxY(nBlobID)-nDilation;
	
		pSize[0]=(rr-ll+1)*dScaleX;
		pSize[1]=(bb-tt+1)*dScaleY;
		pSize[2]=(pSize[0]+pSize[1])*0.5;
		
		nCenterX=(ll+rr)/2;
		nCenterY=(tt+bb)/2;

		if(nSizeMethod) //0: X, Y구해서 (X+Y)/2   1: 장축, 단축 구해서 장축사용  2: 장축, 단축 구해서 (장축+단축)/2
		{               //장축, 단축 구할때만 여기 들어옴
			if(rr-ll>=2 && bb-tt>=2)
			{
				g_Chain.Chain_Center(nBlobID, &cx, &cy);
				angle=g_Chain.FindAngle(nBlobID);
				if(fabs(angle)<1)
				{
					if(nSizeMethod==2) pSize[2]=(pSize[0]+pSize[1])*0.5;
					else			   pSize[2] =pSize[0];
				}
				else if( fabs(fabs(angle)-90) <1)
				{
					if(nSizeMethod==2) pSize[2]=(pSize[0]+pSize[1])*0.5;
					else			   pSize[2] =pSize[1];
				}
				else
				{
					g_Chain.FindMinRect(nBlobID, cx, cy, angle,  pos);
					dXDist=(fabs(pos[0][0]-pos[1][0])-nDilation*2)*dScaleX; if(dXDist<0) dXDist=dScaleX;
					dYDist=(fabs(pos[0][1]-pos[1][1])-nDilation*2)*dScaleY; if(dYDist<0) dYDist=dScaleY;
					dist1=sqrt(dXDist*dXDist+dYDist*dYDist);
					dXDist=(fabs(pos[0][0]-pos[3][0])-nDilation*2)*dScaleX; if(dXDist<0) dXDist=dScaleX;
					dYDist=(fabs(pos[0][1]-pos[3][1])-nDilation*2)*dScaleY; if(dYDist<0) dYDist=dScaleY;
					dist2=sqrt(dXDist*dXDist+dYDist*dYDist);
							 

					if(dist1>dist2)
					{
						pSize[0] =dist1;
						pSize[1] =dist2;
						if(nSizeMethod==2) pSize[2]=(dist1+dist2)*0.5;
						else			   pSize[2] =dist1;
					}
					else
					{
						pSize[0] =dist2;
						pSize[1] =dist1;
						if(nSizeMethod==2) pSize[2]=(dist1+dist2)*0.5;
						else               pSize[2] =dist2;
					}
				}
			}
			else
			{
				if(pSize[1]>pSize[0])
				{
					dist1=pSize[1];
					pSize[1]=pSize[0];
					pSize[0]=dist1;
				}
				if(nSizeMethod==2)  pSize[2]=(pSize[0]+pSize[1])*0.5;
				else				pSize[2]=pSize[0];
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------



}

//TPW Hole여부 판정
int HoleCheck(LPBYTE fm, int nCx, int nCy, int pitch, int imageH, int nAveValue, int nThUp, double dScaleX, double dScaleY, int nFrameID)
{
	int i, j;
	int nHole=0;
	int nAve=nAveValue;
	int nTh, nMaxValue, nMinValue;
	BYTE fmBin[DEFECT_IMAGE_SIZE*DEFECT_IMAGE_SIZE];
	int nBlob, nBlobID;
	int left,top,right,bottom;
	int ll,tt,rr,bb;
	double dArea, dMaxArea;
	double dSizeX, dSizeY;
	int diffX, diffY=0,  diffY1=0;
	int nTouchSum, nTouchL=0, nTouchT=0, nTouchR=0, nTouchB=0;

	if(dScaleX<=0 || dScaleY<=0) return 0;
	
	left=nCx-DEFECT_IMAGE_SIZE;		if(left<=0) { nTouchL=1; left=0;}
	right=left+DEFECT_IMAGE_SIZE*2; if(right>=pitch-1) {nTouchR=1; right=pitch-1; left=right-DEFECT_IMAGE_SIZE*2;}
	top=nCy-DEFECT_IMAGE_SIZE;		if(top<=0) { nTouchT=1; top=0;}
	bottom=top+DEFECT_IMAGE_SIZE*2; if(bottom>=imageH-1) {nTouchB=1; bottom=imageH-1; top=bottom-DEFECT_IMAGE_SIZE*2;}
	nTouchSum=nTouchL+nTouchT+nTouchR+nTouchB;

	
	diffX=g_Temp.m_nHoleXpos-nCx;																	 if(diffX<0) diffX*=-1;
	diffY=(nFrameID-g_Temp.m_nHoleFrame)*imageH+nCy-g_Temp.m_nHoleYpos-g_Temp.m_dHolePeriod/dScaleY; if(diffY<0) diffY*=-1;
	diffY1=(nFrameID-g_Temp.m_nHoleFrame)*imageH+nCy-g_Temp.m_nHoleYpos;

	if(g_Temp.m_nHoleFrame>0 && diffX<(4/g_Param.m_dScaleFactorX) && (diffY<(5/g_Param.m_dScaleFactorY) || diffY1<(5/g_Param.m_dScaleFactorY)) ) nHole=1; //Hole과 주기 위치가 같음.


	//Binary----------------------------------------------------------------------------
	memset(fmBin, 0, DEFECT_IMAGE_SIZE*DEFECT_IMAGE_SIZE);
	
	nTh=nAve+nThUp-1; //
	for(i=0;i<DEFECT_IMAGE_SIZE;i++)
		for(j=0;j<DEFECT_IMAGE_SIZE;j++)
			if(*(fm+pitch*(i*2+top)+(j*2+left))>nTh) *(fmBin+DEFECT_IMAGE_SIZE*i+j)=255;	
	//-----------------------------------------------------------------------------------
		
	//Blob----------------------------------------------------------------------------------------------------
	g_Chain.SetChainData(1, fmBin, 1, 1, 1, 2000, DEFECT_IMAGE_SIZE, DEFECT_IMAGE_SIZE);
	nBlob=g_Chain.FastChain(1,1,DEFECT_IMAGE_SIZE-1, DEFECT_IMAGE_SIZE-1);			     

	if(nBlob>0)
	{
		dMaxArea=0;
		for(i=0;i<nBlob;i++)
		{
			dArea=g_Chain.Chain_Area(i);
			if(dArea>=dMaxArea)//최대 불량 사이즈.
			{
				nBlobID = i;
				dMaxArea = dArea;
			}
		}
		if(dMaxArea<0.1) return 0;
	
		ll	= g_Chain.FindMinX(nBlobID);
		rr	= g_Chain.FindMaxX(nBlobID);
		tt	= g_Chain.FindMinY(nBlobID);
		bb	= g_Chain.FindMaxY(nBlobID);
	

		dSizeX=(rr-ll+1)*dScaleX;
		dSizeY=(bb-tt+1)*dScaleY;				

		
		if( nTouchSum>=2 && (ll<=2 && tt <=2)  || (rr>=DEFECT_IMAGE_SIZE-2 && tt<=2) || (ll<=2 && bb>=DEFECT_IMAGE_SIZE-2) || ( rr>=DEFECT_IMAGE_SIZE-2 && bb>=DEFECT_IMAGE_SIZE-2)) //코너에 있는 경우
		{
			g_Temp.m_nHoleXpos=nCx;
			g_Temp.m_nHoleYpos=nCy;
			g_Temp.m_nHoleFrame=nFrameID;
			nHole=1;		
		}
		else if(nTouchSum==1 && (ll<=2 || rr>=DEFECT_IMAGE_SIZE-2))  //좌우에 붙은 경우
		{
			if(dSizeY>2.0 || dSizeY/dSizeX>3.0)   //Y가 2보다 크거나 (세로/가로) 가 3보다 크면 Hole인정
			{
				g_Temp.m_nHoleXpos=nCx;
				g_Temp.m_nHoleYpos=nCy;
				g_Temp.m_nHoleFrame=nFrameID;
				nHole=1;
			}
		}
		else if(nTouchSum==1 && (tt <=2 || bb>=DEFECT_IMAGE_SIZE-2)) //상하에 붙은 경우
		{
			if(dSizeX>2.0 || dSizeX/dSizeY>3.0)   //Y가 2보다 크거나 (세로/가로) 가 3보다 크면 Hole인정
			{
				g_Temp.m_nHoleXpos=nCx;
				g_Temp.m_nHoleYpos=nCy;
				g_Temp.m_nHoleFrame=nFrameID;
				nHole=1;
			}
		}
		else //경계에 붙지 않은 경우 
		{
			if(dSizeX>2 || dSizeY>2)
			{
				g_Temp.m_nHoleXpos=nCx;
				g_Temp.m_nHoleYpos=nCy;
				g_Temp.m_nHoleFrame=nFrameID;
				nHole=1;
			}
		}
		return nHole;
	}	
	else
		return 0;  //TPW 아님
}



//Histogram으로 평균밝기를 구한다.--------------------------------------------------------
int GetAveValueByHistogram(LPBYTE fm, int left, int top, int width, int height, int pitch)
{
	int i,j,nHisto[256];
	int nSum, nMax, nMaxPos;

	memset(nHisto, 0, 256*sizeof(int));
	for(i=top;i<top+height;i++)
		for(j=left;j<left+width;j++)
			nHisto[*(fm+pitch*i+j)]++;

	nSum=nMax=nHisto[0]+nHisto[1]+nHisto[2]+nHisto[3]+nHisto[4];
	nMaxPos=0;
	for(i=5;i<256;i++)
	{
		nSum=nSum+nHisto[i]-nHisto[i-5];
		if(nSum>nMax)
		{
			nMax=nSum;
			nMaxPos=i-4;
		}
	}
	nMax=0;
	for(i=nMaxPos;i<nMaxPos+5;i++)
	{
		if(nHisto[i]>nMax)
		{
			nMax=nHisto[i];
			nMaxPos=i;
		}
	}

	return nMaxPos;
}




//단순한 Gradient인지 검증--------------------------------------------------------------
//실제영역 64x64에서 밝기 변화가 일정값 만큼 없으면 과검(단순 Gradient변화) 으로 간주함
//(nCenterX, nCenterY) Defect 위치
//dLimitvalue : 8x8이웃하는 Pixel간의 최대 밝기 차
//X1, X2 : 영상 사용가능 최대 범위(X)
//nBottom: 영상 사용가능 최대 범위(Y)

#define USE_CHECK_SIZE	8
int CheckSimpleGradient( int nCenterX, int nCenterY,  int X1, int X2, int nBottom, double dLimitvalue)
{
	int nHEIGHT		=g_System.m_nImageH;
	int i, j, k,  ii, jj;
	int nData[USE_CHECK_SIZE*USE_CHECK_SIZE];
	int *pData1, *pData2;
	int ll, tt, rr, bb;
	int ll_, tt_, rr_, bb_;
	int nSum, nTmp, nMax=0;
	int nCol=0, nRow=0;
	LPBYTE fm=l_fmPyramid[2];	//(1/4) 압축영상에서...
	int pitch=l_nPyramidWidth[2];
	int X1Limit=X1/4;
	int X2Limit=X2/4;
	int Y2Limit=nBottom/4;
	int nCx=nCenterX/4;
	int nCy=nCenterY/4;
	int Va,Vb,Vc,Vd;

	if(X2-X1<USE_CHECK_SIZE*8) return 0;

	
	ll=nCx-USE_CHECK_SIZE;    if(ll<X1Limit)   ll=X1Limit;
	rr=ll +USE_CHECK_SIZE*2;  if(rr>X2Limit) { rr=X2Limit; ll=rr-USE_CHECK_SIZE*2;}

	tt=nCy-USE_CHECK_SIZE;    if(tt<0) tt=0;
	bb=tt +USE_CHECK_SIZE*2;  if(bb>Y2Limit) { bb=Y2Limit; tt=bb-USE_CHECK_SIZE*2;}


	if(g_System.m_nImagePart==2)
	{
		if(tt<nHEIGHT/8 && bb>nHEIGHT/8)
		{
			if(nCy<nHEIGHT/8)
			{
				bb=nHEIGHT/8;
				tt=bb-16;
			}
			else
			{
				tt=nHEIGHT/8;
				bb=tt+16;
			}
		}
	}

	ll_=ll;
	tt_=tt;
	rr_=rr;
	bb_=bb;
	for(k=0;k<4;k++)
	{
		ll=ll_;
		tt=tt_;
		rr=rr_;
		bb=bb_;
		if(k/2==1)
		{
			if(nCx<=(X1+X2)/2)
			{	ll=ll_+1;	rr=rr_+1;  }
			else
			{	ll=ll_-1;	rr=rr_+1;  }
		}
		if(k%2==1)
		{
			if(tt_<nHEIGHT/8 || (tt_>=nHEIGHT/4 && tt_<nHEIGHT*3/8))
			{	tt=tt_+1;	bb=bb_+1;  }
			else
			{	tt=tt_-1;	bb=bb_-1;  }
		}

		nRow=0;
		for(i=tt;i<bb-1;i+=2)
		{
			nCol=0;
			for(j=ll;j<rr-1;j+=2)
			{
				nSum=0;
				for(ii=i;ii<i+2;ii++)
					for(jj=j;jj<j+2;jj++)
						nSum+=*(fm+ii*pitch+jj);

				*(nData+nRow*USE_CHECK_SIZE+nCol)=nSum;

				nCol++;
			}
			nRow++;
		}

		//옆에비교
		for(i=0;i<USE_CHECK_SIZE-1;i++)
		{
			pData1=nData+i*USE_CHECK_SIZE;
			pData2=nData+(i+1)*USE_CHECK_SIZE;

			Va=*(pData1++);
			Vb=*(pData2++);

			for(j=0;j<USE_CHECK_SIZE-1;j++)
			{
				Vc=*(pData1++);
				Vd=*(pData2++);
				nTmp=Va-Vb; if(nTmp<0) nTmp*=-1; if(nTmp>nMax) nMax=nTmp;
				nTmp=Va-Vc; if(nTmp<0) nTmp*=-1; if(nTmp>nMax) nMax=nTmp;
				nTmp=Va-Vd; if(nTmp<0) nTmp*=-1; if(nTmp>nMax) nMax=nTmp;
				nTmp=Vb-Vc; if(nTmp<0) nTmp*=-1; if(nTmp>nMax) nMax=nTmp;
				Va=Vc;
				Vb=Vd;
			}
		}
		//맨 아랫줄
		pData1=nData+(USE_CHECK_SIZE-1)*USE_CHECK_SIZE;
		Va=*(pData1++);
		for(j=0;j<USE_CHECK_SIZE-1;j++)
		{
			Vc=*(pData1++);
			nTmp=Va-Vc; if(nTmp<0) nTmp*=-1; 
			if(nTmp>nMax) nMax=nTmp;
			Va=Vc;
		}

		//맨 오른쪽 줄
		Va=*(nData+i*USE_CHECK_SIZE+USE_CHECK_SIZE-1);
		for(i=0;i<USE_CHECK_SIZE-1;i++)
		{
			Vb=*(nData+(i+1)*USE_CHECK_SIZE+USE_CHECK_SIZE-1);
			nTmp=Va-Vb; if(nTmp<0) nTmp*=-1; 
			if(nTmp>nMax) nMax=nTmp;
			Va=Vb;
		}

		if(nMax/4.0>=dLimitvalue) return 0;
	}

	return 1;
}
