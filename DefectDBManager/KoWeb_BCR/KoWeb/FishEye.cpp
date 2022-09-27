#include "stdafx.h"
#include "math.h"
#include "FishEye.h"

BYTE l_fmFlat1[128*128];
BYTE l_fmTemp1[128*128];
BYTE l_fmNGBinary1[128*128];
BYTE l_fmNGBinary2[128*128];



int CheckFishEye(LPBYTE fm, int left, int top, int right, int bottom, int pitch)
{
	int ret=1, nType;
	double dMaxDist=20, dist, dCorr=0;
	int nCorr, Cx, Cy, nArea=0, nThres, nCompactness;
	double dWhite, dBlack, dDiff;
	double Cx1, Cy1, Ratio, dStd;
	int nWValue,nBValue;
	CString str;

	
	FishEyeFlat(fm, l_fmFlat1, left, top, right, bottom, pitch, &nWValue, &nBValue, &dist, &dStd);	//1)거리
	if(dStd>=7 || nWValue<40 || nBValue<4 || (int)dist>10)  return 0;  //흑점-백점의 거리가 20 Pixel이상 떨어져 있거나, 백점 Value가 일정값 이하거나, 흑점 Value가 일정값이하거나, 영상의 STD가 일정값이상이면 Fisheye아님
			
	FindBlob(fm, l_fmFlat1, left, top, right, bottom, pitch, 15 , &nArea, &nCompactness, &Ratio);
	if(nArea>100 || nCompactness>70 || Ratio<1.5) return 0;



	return ret;
}


//return 0 : OK
//return -1: NG
int FishEyeFlat(LPBYTE fm, LPBYTE fmD, int left, int top, int right, int bottom, int pitch,  int *pWValue, int *pBValue, double *pDist, double *pSTD)
{
	int i,j,k;
	int nX1, nX2, nY1, nY2;
	int nProj[3][128];
	int nProjR[128];
	int nY[3], nDIff[3], nMinDIff, nTmp, tmp, nPos=0, nSum=0, nArea;
	int nMax, nMaxPos[2], nMin, nMinPos[2];
	double dtemp1, dtemp2, dTmp, dAvg;

	memset(nProj, 0, 3*128*sizeof(int));
	nY[0]=top; nY[1]=(top+bottom)/2-5; nY[2]=bottom-10;
	for(k=0;k<3;k++)
	{
		for(i=nY[k];i<nY[k]+10;i++)
			for(j=left;j<right;j++)		
				nProj[k][j]+=*(fm+pitch*i+j);

		nDIff[k]=0;
	}

	for(j=left;j<right;j++)
	{
		if(nProj[0][j]>nDIff[0]) nDIff[0]=nProj[0][j];
		if(nProj[1][j]>nDIff[1]) nDIff[1]=nProj[1][j];
		if(nProj[2][j]>nDIff[2]) nDIff[2]=nProj[2][j];
		
	}

	if(nDIff[0]>=nDIff[1] && nDIff[0]>=nDIff[2])
	{
		for(j=left;j<right;j++)
			nProjR[j]=(nProj[1][j]+nProj[2][j])/20;
	}
	else if(nDIff[1]>=nDIff[0] && nDIff[1]>=nDIff[2])
	{
		for(j=left;j<right;j++)
			nProjR[j]=(nProj[0][j]+nProj[2][j])/20;
	}
	else if(nDIff[2]>=nDIff[1] && nDIff[2]>=nDIff[0])
	{
		for(j=left;j<right;j++)
			nProjR[j]=(nProj[0][j]+nProj[1][j])/20;
	}
	
	//백위치-----------------------------------------
	nMax=0;
	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
		{
			tmp=*(fm+pitch*i+j);
			nTmp=128+tmp-nProjR[j];
			nSum+=tmp;
			if(nTmp<0) nTmp=0;
			else if(nTmp>255) nTmp=255;
			*(fmD+pitch*i+j)=nTmp;

			if(nTmp>nMax)
			{
				nMax=nTmp;
				nMaxPos[0]=j;
				nMaxPos[1]=i;
			}
		}
	//흑위치(백중심에서 +/- 32 영역에서 찾음)----------
	nX1=nMaxPos[0]-32; if(nX1<left)  nX1=left;
	nX2=nMaxPos[0]+32; if(nX2>right) nX2=right;
	nY1=nMaxPos[1]-32; if(nY1<top)	 nY1=top;
	nY2=nMaxPos[1]+32; if(nY2>bottom)nY2=bottom;

	nMin=255*9;
	for(i=nY1+1;i<nY2-1;i++)
		for(j=nX1+1;j<nX2-1;j++)
		{
			tmp=*(fmD+pitch*(i-1)+j-1)+*(fmD+pitch*(i-1)+j)+*(fmD+pitch*(i-1)+j+1)+
					 *(fmD+pitch*i+j-1)+*(fmD+pitch*i+j)+*(fmD+pitch*i+j+1)+
					 *(fmD+pitch*(i+1)+j-1)+*(fmD+pitch*(i+1)+j)+*(fmD+pitch*(i+1)+j+1);
			if(tmp<nMin)
			{
				nMin=tmp;
				nMinPos[0]=j;
				nMinPos[1]=i;
			}
		}
	nMin/=9;
	//STD찾기 ---------------------------------------------
	nArea=(bottom-top)*(right-left);
	dAvg=(double)nSum/nArea;

	dtemp1=dtemp2=0;
	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
		{
			nTmp    =*(fm+pitch*i+j);
			dTmp    = nTmp - dAvg;
			dtemp1 += dTmp;
			dtemp2 += dTmp * dTmp;
		}
	
	if(nArea*dtemp2-dtemp1*dtemp1>0 && nArea*(nArea-1.)>0)
		*pSTD = sqrt((nArea*dtemp2-dtemp1*dtemp1)/(nArea*(nArea-1.)));
	else
		*pSTD =100;
	//----------------------------------------------------------------

	*pWValue=nMax-128;
	*pBValue=128-nMin;
	*pDist=sqrt((double)(nMaxPos[0]-nMinPos[0])*(nMaxPos[0]-nMinPos[0])+(nMaxPos[1]-nMinPos[1])*(nMaxPos[1]-nMinPos[1]));

	return 0;
}




#define	K_WIDTH	 1392
#define K_HEIGHT 640
extern int l_fm1[4096*4096];
extern int l_fm2[4096*4096/4];
extern int l_fm4[4096*4096/16];
extern int l_fm8[4096*4096/64];
extern int l_fm16[4096*4096/256];
extern int l_fm32[4096*4096/1024];
void MakePyramidImage_Lowcut1(LPBYTE fm, int width, int height, int pitch, int nLowCut);
void ImageCompress1(LPBYTE fm, LPBYTE fmD, int width, int height, int pitch, int nLevel, int lowcut, int *pWidth, int *pHeight) 
{
	int i,j;

	MakePyramidImage_Lowcut1(fm, width, height, pitch, lowcut);

	if(nLevel==5)
	{
		*pWidth=width/32;
		*pHeight=height/32;

		for(i=0;i<*pHeight;i++)
			for(j=0;j<*pWidth;j++)
				*(fmD+*pWidth*i+j)=*(l_fm32+*pWidth*i+j);
	}
}

//return : dLimit 이하의 수 
void MakePyramidImage_Lowcut1(LPBYTE fm, int width, int height, int pitch, int nLowCut)
{
	int k,i,j, ii, jj, nTmp, X1, X2, Y1, Y2;
	int nDevide=1, nWidthS, nHeightS, nWidthD,  nHeightD;
	int *fmS, *fmD, *fmT, nMinValue=0;
	int nLowCount=0, nBase=100;
	double dTmp, dTmpMin=0, dNormalMin=0;
	int  Px, Py;
	LPBYTE fmTemp;

	//111020추가함 --------------------------
	//평균밝기 구한다.
	nMinValue=nLowCount=0;
	for(i=0;i<height;i+=10)
		for(j=10;j<width-10;j++)
		{
			nMinValue+=*(fm+pitch*i+j);
			nLowCount++;
		}
	if(nLowCount)
		nBase=nMinValue/nLowCount;
	//---------------------------------------


	//영상평활화(2x2)씩  : 단 Lowcut적용해서 영상반전 >>>>>>>>>
	for(i=0;i<height-1;i+=2)
	{
		fmTemp=fm+pitch*i;
		fmT=l_fm1+K_WIDTH*i;
		for(j=0;j<width-1;j+=2)
		{
			nTmp=nBase-*(fmTemp+j);
			if(nTmp<0) nTmp*=-1;
			if(nTmp>nLowCut)	nTmp=nLowCut;
			*(fmT+j)=nTmp;
			*(fmT+j+1)=nTmp;
			*(fmT+K_WIDTH+j)=nTmp;
			*(fmT+K_WIDTH+j+1)=nTmp;
		}
	}
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


	//영상압축 1/2, 1/4, 1/8, 1/16, 1/32 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	nWidthS=nWidthD=width;
	nHeightS=nHeightD=height;

	for(k=0;k<5;k++)
	{
		if(k==0)		{ fmS=l_fm1;  fmD=l_fm2; }
		else if(k==1)	{ fmS=l_fm2;  fmD=l_fm4; }
		else if(k==2)	{ fmS=l_fm4;  fmD=l_fm8; }
		else if(k==3)	{ fmS=l_fm8;  fmD=l_fm16;}
		else if(k==4)	{ fmS=l_fm16; fmD=l_fm32;}
		nWidthS=width/nDevide;
		nHeightS=height/nDevide;
		nDevide*=2;
		nWidthD=width/nDevide;
		nHeightD=height/nDevide;

		for(i=0, ii=0;i<nHeightS-1;i+=2, ii++)
		{
			for(j=0, jj=0;j<nWidthS-1;j+=2, jj++)
			{
				fmT=fmS+nWidthS*i+j;
				nTmp=*(fmT)+*(fmT+1)+*(fmT+nWidthS)+*(fmT+nWidthS+1);
				*(fmD+nWidthD*ii+jj)=nTmp/4;
			}
		}
	}
}

int CheckVerticalEdge(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nThreshold, double Cx, double Cy, double *Ratio)
{
	int nSize=30;
	int nOffset=9;
	int x1, y1, x2, y2;
	int i, j;
	int nTmp, nSumValue, nLimitCount=900;
	LPBYTE fm1, fm2;
	int nHisto[256];

	int nSumX=0, nSumY=0, nCount=0;
	*Ratio=0;

	memset(nHisto, 0, 256*sizeof(int));
	for(i=top;i<bottom-nOffset;i++)
	{
		fm1=fm+pitch*i+left;
		fm2=fm+pitch*(i+nOffset)+left;
		for(j=left;j<right;j++)
		{
			nTmp=*(fm1++)-*(fm2++);
			if(nTmp<0) nTmp*=-1;
			if(nTmp>=5) 
			{
				*(l_fmNGBinary1+pitch*i+j)=nTmp;
				nHisto[nTmp]++;
	//			nSumX+=j*nTmp;
	//			nSumY+=(i+5)*nTmp;
	//			nCount+=nTmp;
			}
			else
				*(l_fmNGBinary1+pitch*i+j)=0;
		}
	}
	nSumValue=0;
	for(i=255;i>=0;i--)
	{
		nSumValue+=nHisto[i];
		if(nSumValue>nLimitCount)
		{
			nThreshold=i;
			break;
		}
	}



	x1=(int)Cx-nSize/2; if(x1<left) x1=left ; if(x1+nSize>right) x1=right-nSize;
	x2=x1+nSize;

	y1=(int)Cy-nSize/2; if(y1<top) y1=top ; if(y1+nSize>bottom-nOffset) y1=bottom-nOffset-nSize;
	y2=y1+nSize;

	nCount=0;
	for(i=y1;i<y2;i++)
		for(j=x1;j<x2;j++)
		{
			if(*(l_fmNGBinary1+pitch*i+j)>=nThreshold)
				nCount++;
		}

	*Ratio=nCount*100.0/(nSize*nSize);		

	return 0;
}

int FindBlob(LPBYTE fmGray, LPBYTE fm, int left, int top, int right, int bottom, int pitch, int ThUp, int *pArea,  int *pCompactness, double *dDiff)
{
	int i,j, ret=0;
	int nTmp, nX1, nX2, nY1, nY2;
	int nBX1, nBX2, nBY1, nBY2;
	int nUpThreshold=128+ThUp;
	int N1=0, N2=0, nMAX[2]={0,0}, nValue[2]={128,128};
	int nBlackSum, nWhiteSum, nBlackCount, nWhiteCount;
	int nMinL, nMinR;
	double angle, area, areaMaxB, areaMax, cx, cy, cx1, cy1, distTT;
	double pos[4][2], dist1, dist2, dCompactness=0;
	if(left<0) { left=0; right=60;}
	if(top<0)  { top=0;  bottom=60;}
 	if(right>=pitch) {right=pitch; left=right-60;}
	if(bottom>128)   {bottom=128-1; top=128-60;}
	
	*dDiff=1;
	*pArea=1000;
	*pCompactness=100;
	nBlackSum=nWhiteSum=nBlackCount=nWhiteCount=0;

	//흰것 Binary--------------------------------------
	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
		{
			nTmp=*(fm+pitch*i+j);
			
			if(nTmp>nUpThreshold)
				*(l_fmNGBinary2+pitch*i+j)=255;
			else				  
				*(l_fmNGBinary2+pitch*i+j)=0;
		}
	//-------------------------------------------------

	BinaryDilate(l_fmNGBinary2,l_fmTemp1, left,top,right,bottom, pitch, 3, 3);
	BinaryErode(l_fmTemp1, l_fmNGBinary2,left+1,top+1,right-1,bottom-1, pitch, 3, 3);
	g_Chain.SetChainData(1, l_fmNGBinary2, 1, 1, 4, 1000, pitch, bottom);	
	N2=g_Chain.FastChain(left+2, top+2, right-2, bottom-2);

	if(N2>0)
	{
		areaMax=0;

		for(i=0;i<N2;i++)
		{
			area=g_Chain.Chain_Area(i);
			if(area>areaMax) 
			{
				nMAX[1]=i;
				areaMax=area;
			}
		}
		if(nMAX[1]>=0)
		{
			g_Chain.Chain_Center(nMAX[1], &cx, &cy);
			nValue[1]=*(fmGray+pitch*(int)cy+(int)cx);
			nX1=g_Chain.FindMinX(nMAX[1]);
			nY1=g_Chain.FindMinY(nMAX[1]);
			nX2=g_Chain.FindMaxX(nMAX[1]);
			nY2=g_Chain.FindMaxY(nMAX[1]);
			if(nY2-nY1>=40 || nX2-nX1>=40 || nX1<=left+3 || nX2>=right-3 || nY1<=top+3 || nY2>=bottom-3)
				return 0;

			angle=g_Chain.FindAngle(nMAX[1]);
			if(nX2-nX1>=2 && nY2-nY1>=2 && angle!=0)
			{		
				g_Chain.FindMinRect(nMAX[1], cx, cy, angle,  pos);
				dist1=sqrt((pos[0][0]-pos[1][0])*(pos[0][0]-pos[1][0])+(pos[0][1]-pos[1][1])*(pos[0][1]-pos[1][1]));
				dist2=sqrt((pos[0][0]-pos[3][0])*(pos[0][0]-pos[3][0])+(pos[0][1]-pos[3][1])*(pos[0][1]-pos[3][1]));
			}
			if(dist1>dist2)
			{
				*dDiff=dist1/dist2;
			}
			else
			{
				*dDiff=dist2/dist1;
			}
			dCompactness=g_Chain.FindCompactness(nMAX[1]);
			if(fabs(angle)<10 || fabs(angle)>83) 
			{
				*dDiff=1;
			}
		}

		//Blob 2개 이상있을떄 첫번째-두번쨰 거리가 15보다 크면 FishEye아님
		if(N2>1)
		{
			areaMaxB=0;
			nMAX[0]=-1;
			for(i=0;i<N2;i++)
			{
				if(i==nMAX[1]) continue;
				area=g_Chain.Chain_Area(i);
				if(area>areaMaxB) 
				{
					nMAX[0]=i;
					areaMaxB=area;
				}
			}
			if(nMAX[0]>0)
			{
				g_Chain.Chain_Center(nMAX[0], &cx1, &cy1);				
				distTT=sqrt((cx-cx1)*(cx-cx1)+(cy-cy1)*(cy-cy1));
				if(distTT>15) *dDiff=1;
			}
		}
	}
	else
		return 0;

	*pArea=	areaMax;
	*pCompactness=(int)(100*dCompactness);

	return ret;
}

/*
//흰점기준(흰점이 커지게) : 기존보다 적게 걸림.
int BinaryDilate(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY)
{
	int i, j, ii, jj;
	int nSum;
	int nHalfX, nHalfY;

	if(nSizeX%2==0) nSizeX++;
	if(nSizeY%2==0) nSizeY++;

	nHalfX=nSizeX/2;
	nHalfY=nSizeY/2;

	for(i=top;i<bottom;i++)
		memset(fmD+pitch*i+left, 0, (right-left));
	
	for(i=top;i<bottom-nSizeY;i++)
		for(j=left;j<right-nSizeX;j++)
		{
			if(j==left)
			{
				nSum=0;
				for(ii=i;ii<i+nSizeY;ii++)
					for(jj=j;jj<j+nSizeX;jj++)
						nSum+=*(fmS+pitch*ii+jj);
			}
			else
			{
				for(ii=i;ii<i+nSizeY;ii++)
					nSum+=(*(fmS+pitch*ii+j+nSizeX-1)-*(fmS+pitch*ii+j-1));
			}

			if(nSum) *(fmD+pitch*(i+nHalfY)+j+nHalfX)=255;
		}
	
	return 0;
}


//흰점기준(흰점이 작아지게)
int BinaryErode(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY)
{
	int i, j;
	int nSum;
	int nHalfX, nHalfY;
//	int nTmp;
	int nStdValue;

	if(nSizeX%2==0) nSizeX++;
	if(nSizeY%2==0) nSizeY++;

	nHalfX=nSizeX/2;
	nHalfY=nSizeY/2;


	nStdValue=255*nSizeX*nSizeY;

	for(i=top;i<bottom;i++)
		memset(fmD+pitch*i+left, 255, (right-left));

	//맨 왼쪽 Y에 따른 nSizeX만큼 합 ------------
	for(i=top;i<bottom;i++)
	{	
		nSum=0;
		for(j=left;j<left+nSizeX;j++)
			nSum+=*(fmS+pitch*i+j);

		l_SumVer[i]=nSum;
	}
	//--------------------------------------------

	for(j=left+nHalfX;j<right-nHalfX;j++)
	{
		if(j!=left+nHalfX) //한칸씩 오른쪽으로 감에 따라 이 값도 변경
		{
			for(i=top;i<bottom;i++)
				l_SumVer[i]=l_SumVer[i]+*(fmS+pitch*i+j-nHalfX+nSizeX-1)-*(fmS+pitch*i+j-nHalfX-1);
		}

		nSum=0;
		for(i=top;i<top+nSizeY;i++)
			nSum+=l_SumVer[i];

		for(i=top+nHalfY;i<bottom-nHalfY;i++)
		{
			if(nSum!=nStdValue) *(fmD+pitch*i+j)=0;

			nSum=nSum+l_SumVer[i-nHalfY+nSizeY]-l_SumVer[i-nHalfY];
		}
	}

	return 0;
}
*/

