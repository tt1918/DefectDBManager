#include "Stdafx.h"


extern BYTE l_fmTemp1[128*128];
int CheckBondingSheet(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nOverkillLevel)
{
	int i, j;
	int nMax=-1, nTmp;
	int nMin=256;
	int nArray[128], nIsWhite[128];
	int nMaxArray, nAvgArray, nArrayWhite;
	double dMulti;
	int nSum=0, nAvg;
	int nMinDiff, nMinDiffW, nB, nB_;
	LPBYTE fmT;

	if(nOverkillLevel<1) return 0; //불량 (과검체크 안함)
	if(nOverkillLevel>3) nOverkillLevel=3;

	if(nOverkillLevel==1)		 {dMulti=2.5;   nMinDiff=12;  nMinDiffW=20; nB=20; nB_=15;}
	else if(nOverkillLevel==2)	 {dMulti=2.5;	nMinDiff=12;  nMinDiffW=25; nB=25; nB_=18;}
	else if(nOverkillLevel==3)	 {dMulti=3.0;   nMinDiff=20;  nMinDiffW=30; nB=30; nB_=22;}

	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
		{
			if(*(fm+pitch*i+j)>nMax)		nMax=*(fm+pitch*i+j);
			else if(*(fm+pitch*i+j)<nMin)	nMin=*(fm+pitch*i+j);
			nSum+=*(fm+pitch*i+j);
		}
	nAvg=nSum/((right-left)*(bottom-top));
	if(nMax-nMin>80) return 0;	//불량

	COSS_Mean(fm, l_fmTemp1, left, top, right, bottom, pitch, 5, 5);

	fmT=l_fmTemp1;

	for(j=left+2;j<right-2;j++)
	{
		nMax=-1;
		nMin=256;
		for(i=top+2;i<bottom-2;i++)
		{
			if( *(fmT+pitch*i+j)>nMax)		nMax=*(fmT+pitch*i+j);
			else if(*(fmT+pitch*i+j)<nMin)	nMin=*(fmT+pitch*i+j);
		}
		nArray[j]=nMax-nMin;
		if(nMax-nAvg>nAvg-nMin) nIsWhite[j]=1;
		else                    nIsWhite[j]=0;
	}
	nMaxArray=nAvgArray=0;
	for(j=left+2;j<right-2;j++)
	{
		if(nArray[j]>nMaxArray)
		{
			nMaxArray=nArray[j];
			nArrayWhite=nIsWhite[j];
		}
		nAvgArray+=nArray[j];
	}
	nAvgArray/=(right-left-4);

	if(nArrayWhite)	//흰점우선인 경우
	{
		if(nMaxArray>=nMinDiffW && nMaxArray>=nAvgArray*dMulti)
			return 0; //불량
	}
	else
	{
		if(nMaxArray>=nMinDiff && nMaxArray>=nAvgArray*dMulti)
			return 0; //불량
	}

	for(j=left+2;j<right-2;j++)
	{
		for(i=top+2;i<bottom-2-10;i++)
		{
			nTmp=*(fmT+pitch*i+j)-*(fmT+pitch*(i+10)+j);
			if(nTmp<0) nTmp*=-1;
			if(nTmp>=nB_)
			{		
				if((*(fmT+pitch*i+j)+*(fmT+pitch*(i+10)+j))/2 >nAvg) //백점
				{
					if(nTmp>=nB) 
						return 0;	//백점불량
				}
				else if(*(fmT+pitch*i+j)<= nAvg-nB_  || *(fmT+pitch*(i+10)+j)<= nAvg-nB_) //흑점
				{
					return 0;	//흑점불량
				}
			}
		}		
	}

	return 1;	//과검
}