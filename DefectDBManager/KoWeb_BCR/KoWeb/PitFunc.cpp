#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Ko, Young Woo
///////////////////////////////////////////////////////////////////////////////////////////////////////////

BYTE fmPitGray[128*128];
BYTE fmPitBin[128*128];
BYTE l_BoundData[200];
void quick_sort(BYTE list[], int left, int right);
double GetPitValue(LPBYTE fm, int startX, int startY, int width, int height, int pitch, int nBlackTh)
{
	double dValue=0;
	int i,j,loop, nSum=0;
	double nTh;
	int nBlob;
	int nMax, nLength, nMaxPos, nCnt;
	int *pixel, nBoundary;
	int nDataCount=10; //최대 20번
	double dMax, dAvg, dDate[20];
	CPoint pOrg[4], PMap[4];

	memset(dDate, 0, 20*sizeof(double));
	if(width>64) width=64;
	if(height>64) height=64;		
	
	for(i=startY;i<startY+height;i++)
		for(j=startX;j<startX+width;j++)
			nSum+=*(fm+pitch*i+j);

	nTh=nSum/(width*height)-nBlackTh;

	pOrg[0].x=startX;
	pOrg[0].y=startY;
	pOrg[1].x=startX;
	pOrg[1].y=startY+height-1;
	pOrg[2].x=startX+width-1;
	pOrg[2].y=startY+height-1;
	pOrg[3].x=startX+width-1;
	pOrg[3].y=startY;

	PMap[0].x=0;
	PMap[0].y=0;
	PMap[1].x=0;
	PMap[1].y=127;
	PMap[2].x=127;
	PMap[2].y=127;
	PMap[3].x=127;
	PMap[3].y=0;
		
	MakeSubPixel(pOrg, fm, pitch,  PMap, fmPitGray, 128);

	memset(fmPitBin, 0, 128*128);
	for(i=0;i<128;i++)
		for(j=0;j<128;j++)
			if(*(fmPitGray+128*i+j)<nTh) *(fmPitBin+128*i+j)=255;


	g_Chain.SetChainData(1,fmPitBin,1,1,4,500,128,128);
	nBlob=g_Chain.FastChain(0,0,127,127);
	nMax=0;
	for(i=0;i<nBlob;i++)
	{
		nLength=g_Chain.Chain_Length(i);
		if(nLength>nMax)
		{
			nMax=nLength;
			nMaxPos=i;
		}
	}
	if(nMax==0) goto done;

	for(loop=0;loop<nDataCount;loop++)
	{
		memset(fmPitBin, 0, 128*128);
		nBoundary=g_Chain.GetChainDataNumber(nMaxPos);
		pixel=g_Chain.GetChainData(nMaxPos);
		for(i=0;i<nBoundary;i++)
		{
			*(fmPitBin+128*(pixel[2*i+1]-1)+(pixel[2*i]-1))	=255;
			*(fmPitBin+128*(pixel[2*i+1]-1)+pixel[2*i])		=255;
			*(fmPitBin+128*(pixel[2*i+1]-1)+pixel[2*i]+1)	=255;
			*(fmPitBin+128*pixel[2*i+1]+pixel[2*i]-1)		=255;
			*(fmPitBin+128*pixel[2*i+1]+pixel[2*i])			=255;
			*(fmPitBin+128*pixel[2*i+1]+pixel[2*i]+1)		=255;
			*(fmPitBin+128*(pixel[2*i+1]+1)+pixel[2*i]-1)	=255;
			*(fmPitBin+128*(pixel[2*i+1]+1)+pixel[2*i])		=255;
			*(fmPitBin+128*(pixel[2*i+1]+1)+pixel[2*i]+1)	=255;
		}

		nBlob=g_Chain.FastChain(0,0,127,127);
		nMax=0;
		for(i=0;i<nBlob;i++)
		{
			nLength=g_Chain.Chain_Length(i);
			if(nLength>nMax)
			{
				nMax=nLength;
				nMaxPos=i;
			}
		}
		if(nMax==0) 
			break;

		nBoundary=g_Chain.GetChainDataNumber(nMaxPos);
		if(nBoundary>200) nBoundary=200;
		pixel=g_Chain.GetChainData(nMaxPos);
		for(i=0;i<nBoundary;i++)
		{
			l_BoundData[i]=*(fmPitGray+128*pixel[2*i+1]+pixel[2*i]);	
		}
		
		quick_sort(l_BoundData,  0,nBoundary-1);
		nSum=nCnt=0;
		for(i=nBoundary/2;i<nBoundary;i++)
		{
			nSum+=l_BoundData[i];
			nCnt++;
		}
		dDate[loop]=(double)nSum/nCnt;
	}
	dMax=0;
	for(i=0;i<nDataCount;i++)
		if(dDate[i]>dMax)
		{
			dMax=dDate[i];
			nMaxPos=i;
		}

	dAvg=nCnt=0;
	for(i=nMaxPos+3;i<nDataCount;i++)
	{
		dAvg+=dDate[i];
		nCnt++;
	}
	if(nCnt>0) dAvg/=nCnt;
	else       dAvg=dDate[nDataCount-1];

	dValue=dMax-dAvg;
	if(dValue<0) dValue=0;

done:
	return dValue;
}


// 영상메모리 시작번지가 fmOrg이고 피치는 OrgPitch이고, 임의의 사각형( 4점은 *POrg이고 반시계방향으로 코너번호가 매겨짐)
// 영역을 아래의 메모리로 매핑하는 함수 
// 영상의 시작번지가 pMap이고 피치는 MapPitch인 영상의 
//  PMap[0]=(left, top)주로 (0,0)
//  PMap[1]=(left, bottom)주로 (0,height-1)
//  PMap[2]=(right, bottom)주로 (width-1,height-1)
//  PMap[3]=(right, top)주로 (width-1,0)
//  으로 매핑됨.

int  MakeSubPixel(CPoint *POrg, LPBYTE fmOrg, int OrgPitch, CPoint *PMap, LPBYTE fmMap, int MapPitch)    
{
	// Affine Tranform의 6 Parameter 구하기 
	double a,b;
	double dWidth=(PMap[3].x-PMap[0].x)+1;
	double dHeight=(PMap[1].y-PMap[0].y)+1;

	int i,j;
	int x,y, ntmp;
	double dX, dY;
	int Value[4];
	for(i=0;i<(PMap[1].y-PMap[0].y)+1;i++)
		for(j=0;j<(PMap[3].x-PMap[0].x)+1;j++)
		{
			a=(double)(j-PMap[0].x)/dWidth;
			b=(double)(i-PMap[0].y)/dHeight;

			dX=	POrg[0].x*(1.0-a)*(1.0-b)+POrg[3].x*(a)*(1.0-b)+POrg[1].x*(1.0-a)*(b)+POrg[2].x*(a)*(b);
			dY=	POrg[0].y*(1.0-a)*(1.0-b)+POrg[3].y*(a)*(1.0-b)+POrg[1].y*(1.0-a)*(b)+POrg[2].y*(a)*(b);

			x=(int)dX;
			y=(int)dY;

			a=dX-x;
			b=dY-y;

			if(x+1>=POrg[2].x && y+1>=POrg[2].y)    
			{
				*(fmMap+i*MapPitch+j)=*(fmOrg+OrgPitch*y+x);								
			}
			else if(x+1>=POrg[2].x)
			{
				*(fmMap+i*MapPitch+j)=*(fmOrg+OrgPitch*y+x)*(1.0-b)+*(fmOrg+OrgPitch*(y+1)+x)*(b);									  		
			}
			else if(y+1>=POrg[2].y)
			{
				*(fmMap+i*MapPitch+j)=*(fmOrg+OrgPitch*y+x)*(1.0-a)+*(fmOrg+OrgPitch*y+x+1)*(a);
			}
			else
			{
				*(fmMap+i*MapPitch+j)=*(fmOrg+OrgPitch*y+x)*(1.0-a)*(1.0-b)+
									  *(fmOrg+OrgPitch*y+x+1)*(a)*(1.0-b)+
									  *(fmOrg+OrgPitch*(y+1)+x)*(1.0-a)*(b)+
									  *(fmOrg+OrgPitch*(y+1)+x+1)*(a)*(b);						
			}
		}

	return 0; //정상수행 
}



void SortBoundaryData(int nCount, BYTE *pData)
{
	int i,t;
	BYTE dTmp=0;

	for(i=0; i<nCount; i++	)
		for(t=i; t>0; t--)
		{
			if(pData[t]>pData[t-1])
			{
				dTmp = pData[t];
				pData[t] = pData[t-1];
				pData[t-1] = dTmp;
			}
		}	
}


# define MAX_SIZE 9
# define SWAP(x, y, temp) ( (temp)=(x), (x)=(y), (y)=(temp) )

// 1. 피벗을 기준으로 2개의 부분 리스트로 나눈다.
// 2. 피벗보다 작은 값은 모두 왼쪽 부분 리스트로, 큰 값은 오른쪽 부분 리스트로 옮긴다.
/* 2개의 비균등 배열 list[left...pivot-1]와 list[pivot+1...right]의 합병 과정 */
/* (실제로 숫자들이 정렬되는 과정) */
int partition(BYTE list[], int left, int right){
  int pivot, temp;
  int low, high;

  low = left;
  high = right + 1;
  pivot = list[left]; // 정렬할 리스트의 가장 왼쪽 데이터를 피벗으로 선택(임의의 값을 피벗으로 선택)

  /* low와 high가 교차할 때까지 반복(low<high) */
  do{
    /* list[low]가 피벗보다 작으면 계속 low를 증가 */
    do {
      low++; // low는 left+1 에서 시작
    } while (low<=right && list[low]<pivot);

    /* list[high]가 피벗보다 크면 계속 high를 감소 */
    do {
      high--; //high는 right 에서 시작
    } while (high>=left && list[high]>pivot);

    // 만약 low와 high가 교차하지 않았으면 list[low]를 list[high] 교환
    if(low<high){
      SWAP(list[low], list[high], temp);
    }
  } while (low<high);

  // low와 high가 교차했으면 반복문을 빠져나와 list[left]와 list[high]를 교환
  SWAP(list[left], list[high], temp);

  // 피벗의 위치인 high를 반환
  return high;
}

// 퀵 정렬
void quick_sort(BYTE list[], int left, int right)
{

  /* 정렬할 범위가 2개 이상의 데이터이면(리스트의 크기가 0이나 1이 아니면) */
  if(left<right){
    // partition 함수를 호출하여 피벗을 기준으로 리스트를 비균등 분할 -분할(Divide)
    int q = partition(list, left, right); // q: 피벗의 위치

    // 피벗은 제외한 2개의 부분 리스트를 대상으로 순환 호출
    quick_sort(list, left, q-1); // (left ~ 피벗 바로 앞) 앞쪽 부분 리스트 정렬 -정복(Conquer)
    quick_sort(list, q+1, right); // (피벗 바로 뒤 ~ right) 뒤쪽 부분 리스트 정렬 -정복(Conquer)
  }

}

