#include "stdafx.h"
#include "KoWeb.h"
#include "math.h"
#include "KoWebDoc.h"
#include "KoWebView.h"


#ifdef USE_SK_BAT
int l_Gray[MAX_WIDTH];
double l_ShowEdgeX[MAX_WIDTH];
double l_ShowEdgeY[MAX_WIDTH];

double l_dGray[MAX_WIDTH];
int FindHorEdgeGrayTh(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nThres, int nSkipPixel, double *pEdgeX, double *pEdgeY);
int FindVerEdgeGrayTh(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nThres, int nSkipPixel, double *pEdgeX, double *pEdgeY);

int FindHorEdgeSlope1st(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nSlope, int nOffset, int nSkipPixel, double *pEdgeX, double *pEdgeY);
int FindVerEdgeSlope1st(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nSlope, int nOffset, int nSkipPixel, double *pEdgeX, double *pEdgeY);

int FindHorEdgeSlopeMax(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nSlope, int nOffset, int nSkipPixel, double *pEdgeX, double *pEdgeY);
int FindVerEdgeSlopeMax(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nSlope, int nOffset, int nSkipPixel, double *pEdgeX, double *pEdgeY);


// nLineType [ 0: 수평선찾기, 1: 수직선 찾기        ]
// nObject	 [ 0: 흑  1: 백  2: 흑백상관없음		]
// nDir		 [ Edge서치방향 0: (왼쪽->오른쪽 or 위->아래)  1:(오른쪽->왼쪽 or 아래->위) ]
// nMethod	 [ 0: Gray값으로 1:1stEdge 2: Max Edge  ]
// nGrayTh	 [ 이 Gray값 을 넘거나 떨어질때 Edge 를 구함, nMethod=0일때만 사용함]
// nSlopeTh	 [ 기울기가 이 값을 넘으면 Edge, , nMethod=1,2 일때만 사용함 ]
// nAvgLine	 [몇 Line씩 평균해서 Edge 데이타를 구할 건지 설정 ]
// nSkipPixel[ 한번 Edge찾으면 Skip하는 Pixel수]
// nAllowGap [ 라인이 대략 수평이거나 수직일때 사용함. edge가 가장 많이 분포하는 범위, 이 범위 넘어가는 edge후보는 버린다, 0이면 사용하지 않는다.]


//return : 0 라인 못찾음   N 라인Fitting에 사용된 Edge수
//결과 라인함수:  (*pT)Y=(*pA)X+(*pB)
//(*pA)로 라인의 각도 구하는 법
// if((*pT)==0) dAngle=90;
// else	        dAngle=atan( (*pA) )*180/3.141592;

int FindFitLine(LPBYTE fm, int left, int top, int right, int bottom, int pitch, 
	int nLineType, int nObject, int nDir,  int nMethod,  int nGrayTh, int nSlopeTh, int nAvgLine, int nSkipPixel,int nAllowGap,
	double *pT, double *pA, double *pB)
{
	double dAngle;
	double *pEdgeX, *pEdgeY, t, a, b, dCx, dCy;							//구한 라인의 방정식, 및 중심
	int i, nIsHor=0, nCount=0, FoundEdge=0;
	int nEdgeNum;

	if(nAvgLine<1) nAvgLine=1;
	else if(nAvgLine>30) nAvgLine=30;
	if(nLineType==0) nIsHor=1; //수평

	if(nIsHor==1) //수평라인
	{
		nEdgeNum=(right-left)/(nAvgLine+nSkipPixel);
		pEdgeX=new double[nEdgeNum+10];
		pEdgeY=new double[nEdgeNum+10];

		if(nMethod==0) //밝기로 Edge구함
		{
			nCount=FindHorEdgeGrayTh(fm, left, top, right, bottom, pitch, nObject, nDir, nAvgLine, nGrayTh, nSkipPixel, pEdgeX, pEdgeY);
		}
		else if(nMethod==1) //1st기울기
		{
			nCount=FindHorEdgeSlope1st(fm, left, top, right, bottom, pitch, nObject, nDir, nAvgLine, nSlopeTh, 1, nSkipPixel, pEdgeX, pEdgeY);
		}
		else if(nMethod==2) //Max 기울기
		{
			nCount=FindHorEdgeSlopeMax(fm, left, top, right, bottom, pitch, nObject, nDir, nAvgLine, nSlopeTh, 1, nSkipPixel, pEdgeX, pEdgeY);
		}
	}
	else //수직라인
	{
		nEdgeNum=(bottom-top)/(nAvgLine+nSkipPixel);
		pEdgeX=new double[nEdgeNum+10];
		pEdgeY=new double[nEdgeNum+10];

		if(nMethod==0) //밝기로 Edge구함
		{
			nCount=FindVerEdgeGrayTh(fm, left, top, right, bottom, pitch, nObject, nDir, nAvgLine, nGrayTh, nSkipPixel, pEdgeX, pEdgeY);
		}
		else if(nMethod==1) //1st기울기
		{
			nCount=FindVerEdgeSlope1st(fm, left, top, right, bottom, pitch, nObject, nDir, nAvgLine, nSlopeTh, 1, nSkipPixel, pEdgeX, pEdgeY);
		}
		else if(nMethod==2) //Max 기울기
		{
			nCount=FindVerEdgeSlopeMax(fm, left, top, right, bottom, pitch, nObject, nDir, nAvgLine, nSlopeTh, 1, nSkipPixel, pEdgeX, pEdgeY);
		}
	}
	if(nCount*3<nEdgeNum) nCount=0; //Edge못구한것으로 간주함

	if(nAllowGap>0 && nCount>=2) //수평선 또는 수직선이면 수평, 수직이 아닌 데이타 버림. (이 부분 개별 설정하면 더 좋아짐. 현재는1/5영역 외는 버림.
	{
		if(nLineType==0)//수평선
		{
			nCount=ChooseGoodData_FirstBase(nCount, pEdgeY, pEdgeX, (bottom-top)/5);
		}
		else if(nLineType==1)//수직선
		{
			nCount=ChooseGoodData_FirstBase(nCount, pEdgeX, pEdgeY,  (right-left)/5);
		}
	}

	if(nCount>=2)
	{
		//Display용 --ㅡ-------------- 
		for(i=0;i<nCount;i++)
		{
			l_ShowEdgeX[i]=pEdgeX[i];
			l_ShowEdgeY[i]=pEdgeY[i];
		}
		//----------------------------

		if(nIsHor) //데이타 최대 1/3 버림.
		{
			if(LineFitting(nCount, pEdgeX, pEdgeY, nCount/3, 2, &t, &a, &b)>0)
			{
				FoundEdge=nCount;
			}
		}
		else
		{
			if(LineFitting1(nCount, pEdgeX, pEdgeY, nCount/3, 2, &t, &a, &b)>0)
			{
				FoundEdge=nCount;
			}
		}
	}

	if(FoundEdge)
	{
		*pT=t;
		*pA=a;
		*pB=b;
	}
	else
	{
		if(nIsHor==1)
		{
			*pT=1; *pA=0; *pB=(top+bottom)/2;
		}
		else
		{
			*pT=0; *pA=1; *pB=-(left+right)/2;
		}
	}
	delete 	pEdgeX;
	delete 	pEdgeY;
	return FoundEdge;
}



//수평라인 Threshold로 구하는 함수
int FindHorEdgeGrayTh(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nThres, int nSkipPixel, double *pEdgeX, double *pEdgeY)
{
	int i,j, jj;
	int nCount=0;
	int nTh=nThres*nAvgLine;
	bool found;


	for(j=left;j<right-nAvgLine;j+=(nAvgLine+nSkipPixel))
	{
		pEdgeX[nCount]=j+(nAvgLine-1)/2;
		pEdgeY[nCount]=-1;
		found=false;
		if(nAvgLine<=1)
		{
			for(i=top;i<bottom;i++)
			{
				l_Gray[i]=*(fm+nPitch*i+j);
			}
		}
		else
		{
			for(i=top;i<bottom;i++)
			{
				l_Gray[i]=0;
				for(jj=j;jj<j+nAvgLine;jj++)
					l_Gray[i]+=*(fm+nPitch*i+jj);
			}
		}

		if(nDir==0) //top->bottom
		{
			if(nObject==0) //Black
			{
				for(i=top+1;i<bottom;i++)
					if(l_Gray[i]<nTh && l_Gray[i-1]>=nTh)
					{
						pEdgeY[nCount]=i-1+(double)(l_Gray[i-1]-nTh)/(l_Gray[i-1]-l_Gray[i]);
						found=true;
						break;
					}
			}
			else if(nObject==1)	//White
			{
				for(i=top+1;i<bottom;i++)
					if(l_Gray[i]>nTh && l_Gray[i-1]<=nTh)
					{
						pEdgeY[nCount]=i-1+(double)(nTh-l_Gray[i-1])/(l_Gray[i]-l_Gray[i-1]);
						found=true;
						break;
					}
			}
			else
			{
				for(i=top+1;i<bottom;i++)
				{
					if((l_Gray[i]-nTh)*(l_Gray[i-1]-nTh)<0)
					{
						if(l_Gray[i]>nTh) //white
						{
							pEdgeY[nCount]=i-1+(double)(nTh-l_Gray[i-1])/(l_Gray[i]-l_Gray[i-1]);
							found=true;
							break;
						}
						else //black
						{
							pEdgeY[nCount]=i-1+(double)(l_Gray[i-1]-nTh)/(l_Gray[i-1]-l_Gray[i]);
							found=true;
							break;
						}
					}
				}
			}
		}
		else //bottom->top
		{
			if(nObject==0) //Black
			{
				for(i=bottom-2;i>top;i--)
					if(l_Gray[i]<nTh && l_Gray[i+1]>=nTh)
					{
						pEdgeY[nCount]=i+1-(double)(l_Gray[i+1]-nTh)/(l_Gray[i+1]-l_Gray[i]);
						found=true;
						break;
					}
			}
			else if(nObject==1)	//White
			{
				for(i=bottom-2;i>top;i--)
					if(l_Gray[i]>nTh && l_Gray[i+1]<=nTh)
					{
						pEdgeY[nCount]=i+1-(double)(nTh-l_Gray[i+1])/(l_Gray[i]-l_Gray[i+1]);
						found=true;
						break;
					}
			}
			else
			{
				for(i=bottom-2;i>top;i--)
				{

					if((l_Gray[i]-nTh)*(l_Gray[i+1]-nTh)<0)
					{
						if(l_Gray[i]>nTh) //white
						{
							pEdgeY[nCount]=i+1-(double)(nTh-l_Gray[i+1])/(l_Gray[i]-l_Gray[i+1]);
							found=true;
							break;
						}
						else //black
						{
							pEdgeY[nCount]=i+1-(double)(l_Gray[i+1]-nTh)/(l_Gray[i+1]-l_Gray[i]);
							found=true;
							break;
						}
					}
				}
			}
		}
		if(found) nCount++;
	}


	return nCount;
}

//수직라인 Threshold로 구하는 함수
int FindVerEdgeGrayTh(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nThres, int nSkipPixel, double *pEdgeX, double *pEdgeY)
{
	int i,j, ii;
	int nCount=0;
	int nTh=nThres*nAvgLine;
	bool found;


	for(i=top;i<bottom-nAvgLine;i+=(nAvgLine+nSkipPixel))
	{
		pEdgeY[nCount]=i+(nAvgLine-1)/2;
		pEdgeX[nCount]=-1;
		found=false;

		if(nAvgLine<=1)
		{
			for(j=left;j<right;j++)
			{
				l_Gray[j]=*(fm+nPitch*i+j);
			}
		}
		else
		{
			for(j=left;j<right;j++)
			{
				l_Gray[j]=0;
				for(ii=i;ii<i+nAvgLine;ii++)
					l_Gray[j]+=*(fm+nPitch*ii+j);
			}
		}

		if(nDir==0) //left->right
		{
			if(nObject==0) //Black
			{
				for(j=left+1;j<right;j++)
					if(l_Gray[j]<nTh && l_Gray[j-1]>=nTh)
					{
						pEdgeX[nCount]=j-1+(double)(l_Gray[j-1]-nTh)/(l_Gray[j-1]-l_Gray[j]);
						found=true;
						break;
					}
			}
			else if(nObject==1) 	//White
			{
				for(j=left+1;j<right;j++)
					if(l_Gray[j]>nTh && l_Gray[j-1]<=nTh)
					{
						pEdgeX[nCount]=j-1+(double)(nTh-l_Gray[j-1])/(l_Gray[j]-l_Gray[j-1]);
						found=true;
						break;
					}
			}
			else
			{
				for(j=left+1;j<right;j++)
					if((l_Gray[j]-nTh)*(l_Gray[j-1]-nTh)<0)
					{
						if(l_Gray[j]>nTh) //white
						{
							pEdgeX[nCount]=j-1+(double)(nTh-l_Gray[j-1])/(l_Gray[j]-l_Gray[j-1]);
							found=true;
							break;
						}
						else //black
						{
							pEdgeX[nCount]=j-1+(double)(l_Gray[j-1]-nTh)/(l_Gray[j-1]-l_Gray[j]);
							found=true;
							break;
						}
					}
			}
		}
		else //right->left
		{
			if(nObject==0) //Black
			{
				for(j=right-2;j>left;j--)
					if(l_Gray[j]<nTh && l_Gray[j+1]>=nTh)
					{
						pEdgeX[nCount]=j+1-(double)(l_Gray[j+1]-nTh)/(l_Gray[j+1]-l_Gray[j]);
						found=true;
						break;
					}
			}
			else if(nObject==1)	//White
			{
				for(j=right-2;j>left;j--)
					if(l_Gray[j]>nTh && l_Gray[j+1]<=nTh)
					{
						pEdgeX[nCount]=j+1-(double)(nTh-l_Gray[j+1])/(l_Gray[j]-l_Gray[j+1]);
						found=true;
						break;
					}
			}
			else
			{
				for(j=right-2;j>left;j--)
					if((l_Gray[j]-nTh)*(l_Gray[j+1]-nTh)<0)
					{
						if(l_Gray[j]>nTh) //white
						{
							pEdgeX[nCount]=j+1-(double)(nTh-l_Gray[j+1])/(l_Gray[j]-l_Gray[j+1]);
							found=true;
							break;
						}
						else //black
						{
							pEdgeX[nCount]=j+1-(double)(l_Gray[j+1]-nTh)/(l_Gray[j+1]-l_Gray[j]);
							found=true;
							break;
						}
					}

			}
		}
		if(found) nCount++;
	}


	return nCount;
}

//수평라인 Slope로 구하는 함수
int FindHorEdgeSlope1st(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nSlope, int nOffset, int nSkipPixel, double *pEdgeX, double *pEdgeY)
{
	int i,j,jj;
	int nCount=0;
	double edge;
	bool found;
	int dir;

	for(j=left;j<right-nAvgLine;j+=(nAvgLine+nSkipPixel))
	{
		pEdgeX[nCount]=j+(nAvgLine-1)/2;
		pEdgeY[nCount]=-1;
		found=false;

		if(nDir==0) //top->bottom
		{
			if(nAvgLine<=1)
			{
				for(i=top;i<bottom;i++)
				{
					l_dGray[i-top]=*(fm+nPitch*i+j);
				}
			}
			else
			{
				for(i=top;i<bottom;i++)
				{
					l_Gray[i]=0;
					for(jj=j;jj<j+nAvgLine;jj++)
						l_Gray[i]+=*(fm+nPitch*i+jj);

					l_dGray[i-top]=(double)l_Gray[i]/nAvgLine;
				}
			}

			if(nObject==0) //Black
			{
				if(LN_FindEdge1st_W2B((bottom-top), l_dGray, nSlope, &edge)==1)
				{
					pEdgeY[nCount]=top+edge;
					found=true;
				}
			}
			else if(nObject==1)	//White
			{
				if(LN_FindEdge1st_B2W((bottom-top), l_dGray, nSlope, &edge)==1)
				{
					pEdgeY[nCount]=top+edge;
					found=true;
				}
			}
			else //Any
			{
				if(LN_FindEdge1st_ABS((bottom-top), l_dGray, nSlope, &edge, &dir)==1)
				{
					pEdgeY[nCount]=top+edge;
					found=true;
				}
			}
		}
		else //bottom->top
		{
			if(nAvgLine<=1)
			{
				for(i=bottom;i>top;i--)
				{
					l_dGray[bottom-i]=*(fm+nPitch*i+j);
				}
			}
			else
			{
				for(i=bottom;i>top;i--)
				{
					l_Gray[i]=0;
					for(jj=j;jj<j+nAvgLine;jj++)
						l_Gray[i]+=*(fm+nPitch*i+jj);

					l_dGray[bottom-i]=(double)l_Gray[i]/nAvgLine;
				}
			}
			if(nObject==0) //Black
			{
				if(LN_FindEdge1st_W2B((bottom-top), l_dGray, nSlope, &edge)==1)
				{
					pEdgeY[nCount]=bottom-edge;
					found=true;
				}
			}
			else if(nObject==1)	//White
			{
				if(LN_FindEdge1st_B2W((bottom-top), l_dGray, nSlope, &edge)==1)
				{
					pEdgeY[nCount]=bottom-edge;
					found=true;
				}
			}
			else //Any
			{
				if(LN_FindEdge1st_ABS((bottom-top), l_dGray, nSlope, &edge, &dir)==1)
				{
					pEdgeY[nCount]=bottom-edge;
					found=true;
				}
			}
		}
		if(found) nCount++;
	}


	return nCount;
}

//수직라인 Slope로 구하는 함수
int FindVerEdgeSlope1st(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nSlope, int nOffset, int nSkipPixel, double *pEdgeX, double *pEdgeY)
{
	int i,j,ii;
	int nCount=0;
	double edge;
	bool found;
	int dir;

	for(i=top;i<bottom-nAvgLine;i+=(nAvgLine+nSkipPixel))
	{
		pEdgeY[nCount]=i+(nAvgLine-1)/2;
		pEdgeX[nCount]=-1;
		found=false;

		if(nDir==0) //top->bottom
		{
			if(nAvgLine<=1)
			{
				for(j=left;j<right;j++)
				{
					l_dGray[j-left]=*(fm+nPitch*i+j);
				}
			}
			else
			{
				for(j=left;j<right;j++)
				{
					l_Gray[j]=0;
					for(ii=i;ii<i+nAvgLine;ii++)
						l_Gray[j]+=*(fm+nPitch*ii+j);

					l_dGray[j-left]=(double)l_Gray[j]/nAvgLine;
				}
			}

			if(nObject==0) //Black
			{
				if(LN_FindEdge1st_W2B((right-left), l_dGray, nSlope, &edge)==1)
				{
					pEdgeX[nCount]=left+edge;
					found=true;
				}
			}
			else if(nObject==1)	//White
			{
				if(LN_FindEdge1st_B2W((right-left), l_dGray, nSlope, &edge)==1)
				{
					pEdgeX[nCount]=left+edge;
					found=true;
				}
			}
			else	//Any
			{
				if(LN_FindEdge1st_ABS((right-left), l_dGray, nSlope, &edge, &dir)==1)
				{
					pEdgeX[nCount]=left+edge;
					found=true;
				}
			}
		}
		else //bottom->top
		{
			if(nAvgLine<=1)
			{
				for(j=right;j>left;j--)
				{
					l_dGray[right-j]=*(fm+nPitch*i+j);
				}
			}
			else
			{
				for(j=right;j>left;j--)
				{
					l_Gray[j]=0;
					for(ii=i;ii<i+nAvgLine;ii++)
						l_Gray[j]+=*(fm+nPitch*ii+j);

					l_dGray[right-j]=(double)l_Gray[j]/nAvgLine;
				}
			}
			if(nObject==0) //Black
			{
				if(LN_FindEdge1st_W2B((right-left), l_dGray, nSlope, &edge)==1)
				{
					pEdgeX[nCount]=right-edge;
					found=true;
				}
			}
			else if(nObject==1) //White
			{
				if(LN_FindEdge1st_B2W((right-left), l_dGray, nSlope, &edge)==1)
				{
					pEdgeX[nCount]=right-edge;
					found=true;
				}
			}
			else	//ANY
			{
				if(LN_FindEdge1st_ABS((right-left), l_dGray, nSlope, &edge, &dir)==1)
				{
					pEdgeX[nCount]=right-edge;
					found=true;
				}
			}
		}
		if(found)nCount++;
	}


	return nCount;
}

//수평라인 Slope로 구하는 함수
int FindHorEdgeSlopeMax(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nSlope, int nOffset, int nSkipPixel, double *pEdgeX, double *pEdgeY)
{
	int i,j,jj;
	int nCount=0;
	double edge;
	bool found;
	double dSlope;
	int dir;

	for(j=left;j<right-nAvgLine;j+=(nAvgLine+nSkipPixel))
	{
		pEdgeX[nCount]=j+(nAvgLine-1)/2;
		pEdgeY[nCount]=-1;
		found=false;

		if(nDir==0) //top->bottom
		{
			if(nAvgLine<=1)
			{
				for(i=top;i<bottom;i++)
				{
					l_dGray[i-top]=*(fm+nPitch*i+j);
				}
			}
			else
			{
				for(i=top;i<bottom;i++)
				{
					l_Gray[i]=0;
					for(jj=j;jj<j+nAvgLine;jj++)
						l_Gray[i]+=*(fm+nPitch*i+jj);

					l_dGray[i-top]=(double)l_Gray[i]/nAvgLine;
				}
			}

			if(nObject==0) //Black
			{
				if(LN_FindEdge1_W2B((bottom-top), l_dGray, &edge, &dSlope)==1)
				{
					pEdgeY[nCount]=top+edge;
					found=true;
				}
			}
			else if(nObject==1) 	//White
			{
				if(LN_FindEdge1_B2W((bottom-top), l_dGray, &edge, &dSlope)==1)
				{
					pEdgeY[nCount]=top+edge;
					found=true;
				}
			}
			else	//ANY
			{
				if(LN_FindEdge1_ABS((bottom-top), l_dGray, &edge, &dSlope, &dir)==1)
				{
					pEdgeY[nCount]=top+edge;
					found=true;
				}
			}
		}
		else //bottom->top
		{
			if(nAvgLine<=1)
			{
				for(i=bottom;i>top;i--)
				{
					l_dGray[bottom-i]=*(fm+nPitch*i+j);
				}
			}
			else
			{
				for(i=bottom;i>top;i--)
				{
					l_Gray[i]=0;
					for(jj=j;jj<j+nAvgLine;jj++)
						l_Gray[i]+=*(fm+nPitch*i+jj);

					l_dGray[bottom-i]=(double)l_Gray[i]/nAvgLine;
				}
			}
			if(nObject==0) //Black
			{
				if(LN_FindEdge1_W2B((bottom-top), l_dGray, &edge, &dSlope)==1)
				{
					pEdgeY[nCount]=bottom-edge;
					found=true;
				}
			}
			else if(nObject==1) 	//White
			{
				if(LN_FindEdge1_B2W((bottom-top), l_dGray, &edge, &dSlope)==1)
				{
					pEdgeY[nCount]=bottom-edge;
					found=true;
				}
			}			
			else	//ANY
			{
				if(LN_FindEdge1_ABS((bottom-top), l_dGray, &edge, &dSlope, &dir)==1)
				{
					pEdgeY[nCount]=bottom-edge;
					found=true;
				}
			}
		}
		if(found) nCount++;
	}


	return nCount;
}

//수직라인 Slope로 구하는 함수
int FindVerEdgeSlopeMax(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nSlope, int nOffset, int nSkipPixel, double *pEdgeX, double *pEdgeY)
{
	int i,j,ii;
	int nCount=0;
	double edge;
	bool found;
	double dSlope;
	int dir;

	for(i=top;i<bottom-nAvgLine;i+=(nAvgLine+nSkipPixel))
	{
		pEdgeY[nCount]=i+(nAvgLine-1)/2;
		pEdgeX[nCount]=-1;
		found=false;

		if(nDir==0) //top->bottom
		{
			if(nAvgLine<=1)
			{
				for(j=left;j<right;j++)
				{
					l_dGray[j-left]=*(fm+nPitch*i+j);
				}
			}
			else
			{
				for(j=left;j<right;j++)
				{
					l_Gray[j]=0;
					for(ii=i;ii<i+nAvgLine;ii++)
						l_Gray[j]+=*(fm+nPitch*ii+j);

					l_dGray[j-left]=(double)l_Gray[j]/nAvgLine;
				}
			}

			if(nObject==0) //Black
			{
				if(LN_FindEdge1_W2B((right-left), l_dGray, &edge, &dSlope)==1)
				{
					if(dSlope>=nSlope)
					{
						pEdgeX[nCount]=left+edge;
						found=true;
					}
				}
			}
			else if(nObject==1)	//White
			{
				if(LN_FindEdge1_B2W((right-left), l_dGray, &edge, &dSlope)==1)
				{
					if(dSlope>=nSlope)
					{
						pEdgeX[nCount]=left+edge;
						found=true;
					}
				}
			}
			else	//ANY
			{
				if(LN_FindEdge1_ABS((right-left), l_dGray, &edge, &dSlope, &dir)==1)
				{
					if(dSlope>=nSlope)
					{
						pEdgeX[nCount]=left+edge;
						found=true;
					}
				}
			}
		}
		else //bottom->top
		{
			if(nAvgLine<=1)
			{
				for(j=right;j>left;j--)
				{
					l_dGray[right-j]=*(fm+nPitch*i+j);
				}
			}
			else
			{
				for(j=right;j>left;j--)
				{
					l_Gray[j]=0;
					for(ii=i;ii<i+nAvgLine;ii++)
						l_Gray[j]+=*(fm+nPitch*ii+j);

					l_dGray[right-j]=(double)l_Gray[j]/nAvgLine;
				}
			}
			if(nObject==0) //Black
			{
				if(LN_FindEdge1_W2B((right-left), l_dGray, &edge, &dSlope)==1)
				{
					if(dSlope>=nSlope)
					{
						pEdgeX[nCount]=right-edge;
						found=true;
					}
				}
			}
			else if(nObject==1) //White
			{
				if(LN_FindEdge1_B2W((right-left), l_dGray, &edge, &dSlope)==1)
				{
					if(dSlope>=nSlope)
					{
						pEdgeX[nCount]=right-edge;
						found=true;
					}
				}
			}
			else	//ANY
			{
				if(LN_FindEdge1_ABS((right-left), l_dGray, &edge, &dSlope, &dir)==1)
				{
					if(dSlope>=nSlope)
					{
						pEdgeX[nCount]=right-edge;
						found=true;
					}
				}
			}
		}
		if(found)nCount++;
	}


	return nCount;
}

void CKoWebView::InspectFindLine(LPBYTE fm, int nSet, int nId, BOOL bShowGraphic)
{

}
#else
int l_Gray[MAX_WIDTH];
double l_dGray[MAX_WIDTH];
int FindHorEdgeGrayTh(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nThres, double *pEdgeX, double *pEdgeY);
int FindVerEdgeGrayTh(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nThres, double *pEdgeX, double *pEdgeY);

int FindHorEdgeSlope1st(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nSlope, int nOffset, double *pEdgeX, double *pEdgeY);
int FindVerEdgeSlope1st(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nSlope, int nOffset, double *pEdgeX, double *pEdgeY);

int FindHorEdgeSlopeMax(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nSlope, int nOffset, double *pEdgeX, double *pEdgeY);
int FindVerEdgeSlopeMax(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nSlope, int nOffset, double *pEdgeX, double *pEdgeY);

void CKoWebView::InspectFindLine(LPBYTE fm, int nSet, int nId, BOOL bShowGraphic)
{
	int i, j;

	int left		 = g_Inspect[nSet].m_ROI1[nId].rc.left;
	int top			 = g_Inspect[nSet].m_ROI1[nId].rc.top;
	int right		 = g_Inspect[nSet].m_ROI1[nId].rc.right;
	int bottom		 = g_Inspect[nSet].m_ROI1[nId].rc.bottom;
	int nDir		 = g_Inspect[nSet].m_InspFindLine[nId].m_nDir;		//0:(왼쪽->오른쪽 or 위->아래)  1: 반대
	int nLineType	 = g_Inspect[nSet].m_InspFindLine[nId].m_nLineType; //0: 수평선찾기, 1: 수직선 찾기
	int nDiagonal    = g_Inspect[nSet].m_InspFindLine[nId].m_ndiagonal;	//각도 있는 라인
	int nMethod		 = g_Inspect[nSet].m_InspFindLine[nId].m_nMethod;   //0: Gray값으로 1:1stEdge 2: Max Edge
	int nObject		 = g_Inspect[nSet].m_InspFindLine[nId].m_nObject;   //0: 흑  1: 백  2: 임의 
	int nGrayTh		 = g_Inspect[nSet].m_InspFindLine[nId].m_nGrayTh;   //Gray값 을 넘거나 떨어질때 Edge값을 구함.
	int nSlopeTh	 = g_Inspect[nSet].m_InspFindLine[nId].m_nSlopeTh;	//기울기가 이 값을 넘으면 Edge
	int nAvgLine	 = g_Inspect[nSet].m_InspFindLine[nId].m_nAvgLine;  //몇 Line씩 평균해서 Edge 데이타를 구할 건지 설정
	
	double dMinAngle = g_Inspect[nSet].m_InspFindLine[nId].m_dMinAngle; //구한 라인의 각도 양불판정(0~90도)
	double dMaxAngle = g_Inspect[nSet].m_InspFindLine[nId].m_dMaxAngle; //구한 라인의 각도 양불판정(0~90도)
	int  pitch		 = g_System.m_nImageW;								//영상폭

	double dAngle;
	double *pEdgeX, *pEdgeY, t, a, b, dCx, dCy;							//구한 라인의 방정식, 및 중심
	int nIsHor=0, nCount=0, FoundLine=0;
	COLORREF  cColor[2]={RGB(0,255,0), RGB(255,0,0)};
	int nNG=0;

	if(nAvgLine<1) nAvgLine=1;
	else if(nAvgLine>20) nAvgLine=20;

	if(nLineType==0) nIsHor=1; //수평

	if(bShowGraphic) m_pImage->DrawRectangle(NULL, left, top, right, bottom, RGB(128,128,64));   //검사영역표시
	
	memset(&g_InspectResult[nId], 0, sizeof(INSPECTRESULT));									//결과 init
	g_InspectResult[nId].m_nInspectType=INSP_FINDLINE;	


	if(nIsHor==1) //수평라인
	{
		nCount=(right-left)/nAvgLine;
		pEdgeX=new double[nCount+1];
		pEdgeY=new double[nCount+1];

		if(nMethod==0) //밝기로 Edge구함
		{
			nCount=FindHorEdgeGrayTh(fm, left, top, right, bottom, pitch, nObject, nDir, nAvgLine, nGrayTh, pEdgeX, pEdgeY);
		}
		else if(nMethod==1) //1st기울기
		{
			nCount=FindHorEdgeSlope1st(fm, left, top, right, bottom, pitch, nObject, nDir, nAvgLine, nSlopeTh, 1, pEdgeX, pEdgeY);
		}
		else if(nMethod==2) //Max 기울기
		{
			nCount=FindHorEdgeSlopeMax(fm, left, top, right, bottom, pitch, nObject, nDir, nAvgLine, nSlopeTh, 1, pEdgeX, pEdgeY);
		}
	}
	else //수직라인
	{
		nCount=(bottom-top)/nAvgLine;
		pEdgeX=new double[nCount+1];
		pEdgeY=new double[nCount+1];

		if(nMethod==0) //밝기로 Edge구함
		{
			nCount=FindVerEdgeGrayTh(fm, left, top, right, bottom, pitch, nObject, nDir, nAvgLine, nGrayTh, pEdgeX, pEdgeY);
		}
		else if(nMethod==1) //1st기울기
		{
			nCount=FindVerEdgeSlope1st(fm, left, top, right, bottom, pitch, nObject, nDir, nAvgLine, nSlopeTh, 1, pEdgeX, pEdgeY);
		}
		else if(nMethod==2) //Max 기울기
		{
			nCount=FindVerEdgeSlopeMax(fm, left, top, right, bottom, pitch, nObject, nDir, nAvgLine, nSlopeTh, 1, pEdgeX, pEdgeY);
		}
	}

	if(nDiagonal==0 && nCount>=2) //수평선 또는 수직선이면 수평, 수직이 아닌 데이타 버림. (이 부분 개별 설정하면 더 좋아짐. 현재는1/5영역 외는 버림.
	{
		if(nLineType==0)//수평선
		{
			nCount=ChooseGoodData_FirstBase(nCount, pEdgeY, pEdgeX, (bottom-top)/5);
		}
		else if(nLineType==1)//수직선
		{
			nCount=ChooseGoodData_FirstBase(nCount, pEdgeX, pEdgeY,  (right-left)/5);
		}
	}

	if(nCount>=2)
	{
		if(nIsHor) //데이타 최대 1/3 버림.
		{
			if(LineFitting(nCount, pEdgeX, pEdgeY, nCount/3, 2, &t, &a, &b)>0)
			{
				FoundLine=1;
			}
		}
		else
		{
			if(LineFitting1(nCount, pEdgeX, pEdgeY, nCount/3, 2, &t, &a, &b)>0)
			{
				FoundLine=1;
			}
		}
	}


	if(FoundLine==0) g_InspectResult[nId].m_nFindLineResult[0]=nNG=1; //라인 못 찾음
	else
	{
		if(t==0) dAngle=90;
		else	 dAngle=atan(a)*180/3.141592;
		if(dAngle<0) dAngle*=-1;

		if(dAngle<dMinAngle || dAngle>dMaxAngle) g_InspectResult[nId].m_nFindLineResult[1]=nNG=1;
		g_InspectResult[nId].m_dAngle=dAngle;
		g_InspectResult[nId].m_t=t;
		g_InspectResult[nId].m_a=a;
		g_InspectResult[nId].m_b=b;

		if(nIsHor) FindCrossPoint(t,a,b, 0, 1, -1*(right+left)/2.0, &dCx, &dCy);
		else       FindCrossPoint(t,a,b, 1, 0,  (bottom+top)/2.0, &dCx, &dCy);

		g_InspectResult[nId].m_dCx=dCx;
		g_InspectResult[nId].m_dCy=dCy;
	}
		
//	CString str;
//	str.Format(_T("%.3lf"), dCx);
//	WriteLog(str);

	if(bShowGraphic)
	{
		for(j=0;j<nCount;j++)
		{
			m_pImage->SetPixel(NULL, (int)(pEdgeX[j]+1), (int)(pEdgeY[j]+1), RGB(0,255,0));
		}
		if(FoundLine)
			m_pImage->DrawCross(NULL, (int)(dCx+1), (int)(dCy+1), 10, cColor[nNG]);
	}


	delete 	pEdgeX;
	delete 	pEdgeY;
}

void CKoWebView::ShowFindLineInspectResult(int nId)
{


}

//수평라인 Threshold로 구하는 함수
int FindHorEdgeGrayTh(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nThres, double *pEdgeX, double *pEdgeY)
{
	int i,j, jj;
	int nCount=0;
	int nTh=nThres*nAvgLine;
	bool found;


	for(j=left;j<right-nAvgLine;j+=nAvgLine)
	{
		pEdgeX[nCount]=j+(nAvgLine-1)/2;
		pEdgeY[nCount]=-1;
		found=false;
		if(nAvgLine<=1)
		{
			for(i=top;i<bottom;i++)
			{
				l_Gray[i]=*(fm+nPitch*i+j);
			}
		}
		else
		{
			for(i=top;i<bottom;i++)
			{
				l_Gray[i]=0;
				for(jj=j;jj<j+nAvgLine;jj++)
					l_Gray[i]+=*(fm+nPitch*i+jj);
			}
		}

		if(nDir==0) //top->bottom
		{
			if(nObject==0) //Black
			{
				for(i=top+1;i<bottom;i++)
					if(l_Gray[i]<nTh && l_Gray[i-1]>=nTh)
					{
						pEdgeY[nCount]=i-1+(double)(l_Gray[i-1]-nTh)/(l_Gray[i-1]-l_Gray[i]);
						found=true;
						break;
					}
			}
			else if(nObject==1)	//White
			{
				for(i=top+1;i<bottom;i++)
					if(l_Gray[i]>nTh && l_Gray[i-1]<=nTh)
					{
						pEdgeY[nCount]=i-1+(double)(nTh-l_Gray[i-1])/(l_Gray[i]-l_Gray[i-1]);
						found=true;
						break;
					}
			}
			else
			{
				for(i=top+1;i<bottom;i++)
				{
					if((l_Gray[i]-nTh)*(l_Gray[i-1]-nTh)<0)
					{
						if(l_Gray[i]>nTh) //white
						{
							pEdgeY[nCount]=i-1+(double)(nTh-l_Gray[i-1])/(l_Gray[i]-l_Gray[i-1]);
							found=true;
							break;
						}
						else //black
						{
							pEdgeY[nCount]=i-1+(double)(l_Gray[i-1]-nTh)/(l_Gray[i-1]-l_Gray[i]);
							found=true;
							break;
						}
					}
				}
			}
		}
		else //bottom->top
		{
			if(nObject==0) //Black
			{
				for(i=bottom-2;i>top;i--)
					if(l_Gray[i]<nTh && l_Gray[i+1]>=nTh)
					{
						pEdgeY[nCount]=i+1-(double)(l_Gray[i+1]-nTh)/(l_Gray[i+1]-l_Gray[i]);
						found=true;
						break;
					}
			}
			else if(nObject==1)	//White
			{
				for(i=bottom-2;i>top;i--)
					if(l_Gray[i]>nTh && l_Gray[i+1]<=nTh)
					{
						pEdgeY[nCount]=i+1-(double)(nTh-l_Gray[i+1])/(l_Gray[i]-l_Gray[i+1]);
						found=true;
						break;
					}
			}
			else
			{
				for(i=bottom-2;i>top;i--)
				{

					if((l_Gray[i]-nTh)*(l_Gray[i+1]-nTh)<0)
					{
						if(l_Gray[i]>nTh) //white
						{
							pEdgeY[nCount]=i+1-(double)(nTh-l_Gray[i+1])/(l_Gray[i]-l_Gray[i+1]);
							found=true;
							break;
						}
						else //black
						{
							pEdgeY[nCount]=i+1-(double)(l_Gray[i+1]-nTh)/(l_Gray[i+1]-l_Gray[i]);
							found=true;
							break;
						}
					}
				}
			}
		}
		if(found) nCount++;
	}


	return nCount;
}

//수직라인 Threshold로 구하는 함수
int FindVerEdgeGrayTh(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nThres, double *pEdgeX, double *pEdgeY)
{
	int i,j, ii;
	int nCount=0;
	int nTh=nThres*nAvgLine;
	bool found;


	for(i=top;i<bottom-nAvgLine;i+=nAvgLine)
	{
		pEdgeY[nCount]=i+(nAvgLine-1)/2;
		pEdgeX[nCount]=-1;
		found=false;

		if(nAvgLine<=1)
		{
			for(j=left;j<right;j++)
			{
				l_Gray[j]=*(fm+nPitch*i+j);
			}
		}
		else
		{
			for(j=left;j<right;j++)
			{
				l_Gray[j]=0;
				for(ii=i;ii<i+nAvgLine;ii++)
					l_Gray[j]+=*(fm+nPitch*ii+j);
			}
		}

		if(nDir==0) //left->right
		{
			if(nObject==0) //Black
			{
				for(j=left+1;j<right;j++)
					if(l_Gray[j]<nTh && l_Gray[j-1]>=nTh)
					{
						pEdgeX[nCount]=j-1+(double)(l_Gray[j-1]-nTh)/(l_Gray[j-1]-l_Gray[j]);
						found=true;
						break;
					}
			}
			else if(nObject==1) 	//White
			{
				for(j=left+1;j<right;j++)
					if(l_Gray[j]>nTh && l_Gray[j-1]<=nTh)
					{
						pEdgeX[nCount]=j-1+(double)(nTh-l_Gray[j-1])/(l_Gray[j]-l_Gray[j-1]);
						found=true;
						break;
					}
			}
			else
			{
				for(j=left+1;j<right;j++)
					if((l_Gray[j]-nTh)*(l_Gray[j-1]-nTh)<0)
					{
						if(l_Gray[j]>nTh) //white
						{
							pEdgeX[nCount]=j-1+(double)(nTh-l_Gray[j-1])/(l_Gray[j]-l_Gray[j-1]);
							found=true;
							break;
						}
						else //black
						{
							pEdgeX[nCount]=j-1+(double)(l_Gray[j-1]-nTh)/(l_Gray[j-1]-l_Gray[j]);
							found=true;
							break;
						}
					}
			}
		}
		else //right->left
		{
			if(nObject==0) //Black
			{
				for(j=right-2;j>left;j--)
					if(l_Gray[j]<nTh && l_Gray[j+1]>=nTh)
					{
						pEdgeX[nCount]=j+1-(double)(l_Gray[j+1]-nTh)/(l_Gray[j+1]-l_Gray[j]);
						found=true;
						break;
					}
			}
			else if(nObject==1)	//White
			{
				for(j=right-2;j>left;j--)
					if(l_Gray[j]>nTh && l_Gray[j+1]<=nTh)
					{
						pEdgeX[nCount]=j+1-(double)(nTh-l_Gray[j+1])/(l_Gray[j]-l_Gray[j+1]);
						found=true;
						break;
					}
			}
			else
			{
				for(j=right-2;j>left;j--)
					if((l_Gray[j]-nTh)*(l_Gray[j+1]-nTh)<0)
					{
						if(l_Gray[j]>nTh) //white
						{
							pEdgeX[nCount]=j+1-(double)(nTh-l_Gray[j+1])/(l_Gray[j]-l_Gray[j+1]);
							found=true;
							break;
						}
						else //black
						{
							pEdgeX[nCount]=j+1-(double)(l_Gray[j+1]-nTh)/(l_Gray[j+1]-l_Gray[j]);
							found=true;
							break;
						}
					}

			}
		}
		if(found) nCount++;
	}


	return nCount;
}

//수평라인 Slope로 구하는 함수
int FindHorEdgeSlope1st(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nSlope, int nOffset, double *pEdgeX, double *pEdgeY)
{
	int i,j,jj;
	int nCount=0;
	double edge;
	bool found;
	int dir;

	for(j=left;j<right-nAvgLine;j+=nAvgLine)
	{
		pEdgeX[nCount]=j+(nAvgLine-1)/2;
		pEdgeY[nCount]=-1;
		found=false;

		if(nDir==0) //top->bottom
		{
			if(nAvgLine<=1)
			{
				for(i=top;i<bottom;i++)
				{
					l_dGray[i-top]=*(fm+nPitch*i+j);
				}
			}
			else
			{
				for(i=top;i<bottom;i++)
				{
					l_Gray[i]=0;
					for(jj=j;jj<j+nAvgLine;jj++)
						l_Gray[i]+=*(fm+nPitch*i+jj);

					l_dGray[i-top]=(double)l_Gray[i]/nAvgLine;
				}
			}

			if(nObject==0) //Black
			{
				if(LN_FindEdge1st_W2B((bottom-top), l_dGray, nSlope, &edge)==1)
				{
					pEdgeY[nCount]=top+edge;
					found=true;
				}
			}
			else if(nObject==1)	//White
			{
				if(LN_FindEdge1st_B2W((bottom-top), l_dGray, nSlope, &edge)==1)
				{
					pEdgeY[nCount]=top+edge;
					found=true;
				}
			}
			else //Any
			{
				if(LN_FindEdge1st_ABS((bottom-top), l_dGray, nSlope, &edge, &dir)==1)
				{
					pEdgeY[nCount]=top+edge;
					found=true;
				}
			}
		}
		else //bottom->top
		{
			if(nAvgLine<=1)
			{
				for(i=bottom;i>top;i--)
				{
					l_dGray[bottom-i]=*(fm+nPitch*i+j);
				}
			}
			else
			{
				for(i=bottom;i>top;i--)
				{
					l_Gray[i]=0;
					for(jj=j;jj<j+nAvgLine;jj++)
						l_Gray[i]+=*(fm+nPitch*i+jj);

					l_dGray[bottom-i]=(double)l_Gray[i]/nAvgLine;
				}
			}
			if(nObject==0) //Black
			{
				if(LN_FindEdge1st_W2B((bottom-top), l_dGray, nSlope, &edge)==1)
				{
					pEdgeY[nCount]=bottom-edge;
					found=true;
				}
			}
			else if(nObject==1)	//White
			{
				if(LN_FindEdge1st_B2W((bottom-top), l_dGray, nSlope, &edge)==1)
				{
					pEdgeY[nCount]=bottom-edge;
					found=true;
				}
			}
			else //Any
			{
				if(LN_FindEdge1st_ABS((bottom-top), l_dGray, nSlope, &edge, &dir)==1)
				{
					pEdgeY[nCount]=bottom-edge;
					found=true;
				}
			}
		}
		if(found) nCount++;
	}


	return nCount;
}

//수직라인 Slope로 구하는 함수
int FindVerEdgeSlope1st(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nSlope, int nOffset, double *pEdgeX, double *pEdgeY)
{
	int i,j,ii;
	int nCount=0;
	double edge;
	bool found;
	int dir;

	for(i=top;i<bottom-nAvgLine;i+=nAvgLine)
	{
		pEdgeY[nCount]=i+(nAvgLine-1)/2;
		pEdgeX[nCount]=-1;
		found=false;

		if(nDir==0) //top->bottom
		{
			if(nAvgLine<=1)
			{
				for(j=left;j<right;j++)
				{
					l_dGray[j-left]=*(fm+nPitch*i+j);
				}
			}
			else
			{
				for(j=left;j<right;j++)
				{
					l_Gray[j]=0;
					for(ii=i;ii<i+nAvgLine;ii++)
						l_Gray[j]+=*(fm+nPitch*ii+j);

					l_dGray[j-left]=(double)l_Gray[j]/nAvgLine;
				}
			}

			if(nObject==0) //Black
			{
				if(LN_FindEdge1st_W2B((right-left), l_dGray, nSlope, &edge)==1)
				{
					pEdgeX[nCount]=left+edge;
					found=true;
				}
			}
			else if(nObject==1)	//White
			{
				if(LN_FindEdge1st_B2W((right-left), l_dGray, nSlope, &edge)==1)
				{
					pEdgeX[nCount]=left+edge;
					found=true;
				}
			}
			else	//Any
			{
				if(LN_FindEdge1st_ABS((right-left), l_dGray, nSlope, &edge, &dir)==1)
				{
					pEdgeX[nCount]=left+edge;
					found=true;
				}
			}
		}
		else //bottom->top
		{
			if(nAvgLine<=1)
			{
				for(j=right;j>left;j--)
				{
					l_dGray[right-j]=*(fm+nPitch*i+j);
				}
			}
			else
			{
				for(j=right;j>left;j--)
				{
					l_Gray[j]=0;
					for(ii=i;ii<i+nAvgLine;ii++)
						l_Gray[j]+=*(fm+nPitch*ii+j);

					l_dGray[right-j]=(double)l_Gray[j]/nAvgLine;
				}
			}
			if(nObject==0) //Black
			{
				if(LN_FindEdge1st_W2B((right-left), l_dGray, nSlope, &edge)==1)
				{
					pEdgeX[nCount]=right-edge;
					found=true;
				}
			}
			else if(nObject==1) //White
			{
				if(LN_FindEdge1st_B2W((right-left), l_dGray, nSlope, &edge)==1)
				{
					pEdgeX[nCount]=right-edge;
					found=true;
				}
			}
			else	//ANY
			{
				if(LN_FindEdge1st_ABS((right-left), l_dGray, nSlope, &edge, &dir)==1)
				{
					pEdgeX[nCount]=right-edge;
					found=true;
				}
			}
		}
		if(found)nCount++;
	}


	return nCount;
}

//수평라인 Slope로 구하는 함수
int FindHorEdgeSlopeMax(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nSlope, int nOffset, double *pEdgeX, double *pEdgeY)
{
	int i,j,jj;
	int nCount=0;
	double edge;
	bool found;
	double dSlope;
	int dir;

	for(j=left;j<right-nAvgLine;j+=nAvgLine)
	{
		pEdgeX[nCount]=j+(nAvgLine-1)/2;
		pEdgeY[nCount]=-1;
		found=false;

		if(nDir==0) //top->bottom
		{
			if(nAvgLine<=1)
			{
				for(i=top;i<bottom;i++)
				{
					l_dGray[i-top]=*(fm+nPitch*i+j);
				}
			}
			else
			{
				for(i=top;i<bottom;i++)
				{
					l_Gray[i]=0;
					for(jj=j;jj<j+nAvgLine;jj++)
						l_Gray[i]+=*(fm+nPitch*i+jj);

					l_dGray[i-top]=(double)l_Gray[i]/nAvgLine;
				}
			}

			if(nObject==0) //Black
			{
				if(LN_FindEdge1_W2B((bottom-top), l_dGray, &edge, &dSlope)==1)
				{
					pEdgeY[nCount]=top+edge;
					found=true;
				}
			}
			else if(nObject==1) 	//White
			{
				if(LN_FindEdge1_B2W((bottom-top), l_dGray, &edge, &dSlope)==1)
				{
					pEdgeY[nCount]=top+edge;
					found=true;
				}
			}
			else	//ANY
			{
				if(LN_FindEdge1_ABS((bottom-top), l_dGray, &edge, &dSlope, &dir)==1)
				{
					pEdgeY[nCount]=top+edge;
					found=true;
				}
			}
		}
		else //bottom->top
		{
			if(nAvgLine<=1)
			{
				for(i=bottom;i>top;i--)
				{
					l_dGray[bottom-i]=*(fm+nPitch*i+j);
				}
			}
			else
			{
				for(i=bottom;i>top;i--)
				{
					l_Gray[i]=0;
					for(jj=j;jj<j+nAvgLine;jj++)
						l_Gray[i]+=*(fm+nPitch*i+jj);

					l_dGray[bottom-i]=(double)l_Gray[i]/nAvgLine;
				}
			}
			if(nObject==0) //Black
			{
				if(LN_FindEdge1_W2B((bottom-top), l_dGray, &edge, &dSlope)==1)
				{
					pEdgeY[nCount]=bottom-edge;
					found=true;
				}
			}
			else if(nObject==1) 	//White
			{
				if(LN_FindEdge1_B2W((bottom-top), l_dGray, &edge, &dSlope)==1)
				{
					pEdgeY[nCount]=bottom-edge;
					found=true;
				}
			}			
			else	//ANY
			{
				if(LN_FindEdge1_ABS((bottom-top), l_dGray, &edge, &dSlope, &dir)==1)
				{
					pEdgeY[nCount]=bottom-edge;
					found=true;
				}
			}
		}
		if(found) nCount++;
	}


	return nCount;
}

//수직라인 Slope로 구하는 함수
int FindVerEdgeSlopeMax(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nSlope, int nOffset, double *pEdgeX, double *pEdgeY)
{
	int i,j,ii;
	int nCount=0;
	double edge;
	bool found;
	double dSlope;
	int dir;

	for(i=top;i<bottom-nAvgLine;i+=nAvgLine)
	{
		pEdgeY[nCount]=i+(nAvgLine-1)/2;
		pEdgeX[nCount]=-1;
		found=false;

		if(nDir==0) //top->bottom
		{
			if(nAvgLine<=1)
			{
				for(j=left;j<right;j++)
				{
					l_dGray[j-left]=*(fm+nPitch*i+j);
				}
			}
			else
			{
				for(j=left;j<right;j++)
				{
					l_Gray[j]=0;
					for(ii=i;ii<i+nAvgLine;ii++)
						l_Gray[j]+=*(fm+nPitch*ii+j);

					l_dGray[j-left]=(double)l_Gray[j]/nAvgLine;
				}
			}

			if(nObject==0) //Black
			{
				if(LN_FindEdge1_W2B((right-left), l_dGray, &edge, &dSlope)==1)
				{
					if(dSlope>=nSlope)
					{
						pEdgeX[nCount]=left+edge;
						found=true;
					}
				}
			}
			else if(nObject==1)	//White
			{
				if(LN_FindEdge1_B2W((right-left), l_dGray, &edge, &dSlope)==1)
				{
					if(dSlope>=nSlope)
					{
						pEdgeX[nCount]=left+edge;
						found=true;
					}
				}
			}
			else	//ANY
			{
				if(LN_FindEdge1_ABS((right-left), l_dGray, &edge, &dSlope, &dir)==1)
				{
					if(dSlope>=nSlope)
					{
						pEdgeX[nCount]=left+edge;
						found=true;
					}
				}
			}
		}
		else //bottom->top
		{
			if(nAvgLine<=1)
			{
				for(j=right;j>left;j--)
				{
					l_dGray[right-j]=*(fm+nPitch*i+j);
				}
			}
			else
			{
				for(j=right;j>left;j--)
				{
					l_Gray[j]=0;
					for(ii=i;ii<i+nAvgLine;ii++)
						l_Gray[j]+=*(fm+nPitch*ii+j);

					l_dGray[right-j]=(double)l_Gray[j]/nAvgLine;
				}
			}
			if(nObject==0) //Black
			{
				if(LN_FindEdge1_W2B((right-left), l_dGray, &edge, &dSlope)==1)
				{
					if(dSlope>=nSlope)
					{
						pEdgeX[nCount]=right-edge;
						found=true;
					}
				}
			}
			else if(nObject==1) //White
			{
				if(LN_FindEdge1_B2W((right-left), l_dGray, &edge, &dSlope)==1)
				{
					if(dSlope>=nSlope)
					{
						pEdgeX[nCount]=right-edge;
						found=true;
					}
				}
			}
			else	//ANY
			{
				if(LN_FindEdge1_ABS((right-left), l_dGray, &edge, &dSlope, &dir)==1)
				{
					if(dSlope>=nSlope)
					{
						pEdgeX[nCount]=right-edge;
						found=true;
					}
				}
			}
		}
		if(found)nCount++;
	}


	return nCount;
}
#endif