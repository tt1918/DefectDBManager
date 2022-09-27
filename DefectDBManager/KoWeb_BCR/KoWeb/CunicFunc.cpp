#include "Stdafx.h"
#include "math.h"

#define REMOVE_W_LIMIT	10 
#define REMOVE_B_LIMIT	10 

double FinStd_WithLimitCut(LPBYTE fm,  int nAveMean, int left, int top, int right, int bottom, int pitch);

void GetCunicValue_SameFrame(LPBYTE fm, int nX, int nY,  int nMinX, int nMaxX, int pitch, int nMaxHeight, int nCunicSize, double *pCunic, int *pCunicValue)
{
	int i, j;
	int ll,tt,rr,bb;
	double dMaxCunic=0, dCunic;
	int nMaxPosX=-1, nMaxPosY=-1;
	int nSum=0, nAvg;
	int nMaxValue=0;
	double  dBaseStd[2];

	//영상양끝이 일반적으로 이상하기 때문에 양끝 10Pixel은 사용하지 않는다.---
	if(nMinX<10)	   nMinX=10;
	if(nMaxX>pitch-10) nMaxX=pitch-10;
	//------------------------------------------------------------------------

	*pCunic=*pCunicValue=0;
	if(nMaxX-nMinX-2<2*nCunicSize) return;

	ll=nX-nCunicSize;   if(ll<nMinX+1)   ll=nMinX+1;
	rr=ll+nCunicSize*2; if(rr>nMaxX-1) { rr=nMaxX-1;ll=rr-nCunicSize*2;}

	tt=nY-nCunicSize;   if(tt<1) tt=1;
	bb=tt+nCunicSize*2; if(bb>nMaxHeight-1) { bb=nMaxHeight-1;tt=bb-nCunicSize*2;}

	//평균값, 최대값 구함--------------------------------------------------
	for(i=tt;i<bb;i++)
		for(j=ll;j<rr;j++)
		{
			nSum+=*(fm+pitch*i+j);
			if(*(fm+pitch*i+j)>nMaxValue) nMaxValue=*(fm+pitch*i+j);
		}
	nAvg=nSum/((rr-ll)*(bb-tt));
	*pCunicValue=nMaxValue-nAvg;
	//--------------------------------------------------------------------
	
	tt=nY-nCunicSize/2-20; if(tt<0) tt=0;
	bb=tt+nCunicSize;

	for(i=0;i<5;i++)
	{
		ll=nX-nCunicSize/2-20; if(ll<nMinX) ll=nMinX;
		rr=ll+nCunicSize;

		for(j=0;j<5;j++)
		{	
			dCunic=FinStd_WithLimitCut(fm, nAvg, ll,tt,rr,bb,pitch);

			if(dCunic>dMaxCunic)
			{
				dMaxCunic=dCunic;
				nMaxPosX=ll;
				nMaxPosY=tt;
			}

			ll+=10;
			rr+=10;
			if(rr>=nMaxX) 
				break;
		}
		tt+=10;
		bb+=10;
		if(bb>=nMaxHeight) 
			break;
	}
	if(nMaxPosY<0) return;
	//최대값 찾기---------------------------------------------------
	nMaxValue=0;
	for(i=nMaxPosY;i<nMaxPosY+nCunicSize;i++)
		for(j=nMaxPosX;j<nMaxPosX+nCunicSize;j++)
		{
			if(*(fm+pitch*i+j)>nMaxValue) nMaxValue=*(fm+pitch*i+j);
		}
	//--------------------------------------------------------------	

	if(dMaxCunic>0)
	{
		if(nMaxPosX-nCunicSize*2<nMinX)			dBaseStd[0]=-101;
		else									dBaseStd[0]=FinStd_WithLimitCut(fm, nAvg, nMaxPosX-nCunicSize*2,nMaxPosY,nMaxPosX-nCunicSize,  nMaxPosY+nCunicSize,pitch);

		if(nMaxPosX+nCunicSize*3>=nMaxX)		dBaseStd[1]=-101;
		else									dBaseStd[1]=FinStd_WithLimitCut(fm, nAvg, nMaxPosX+nCunicSize*2,nMaxPosY,nMaxPosX+nCunicSize*3,nMaxPosY+nCunicSize,pitch);

		if(dBaseStd[0]>-100 && dBaseStd[1]>-100) *pCunic=dMaxCunic-(dBaseStd[0]+dBaseStd[1])*0.5;
		else if(dBaseStd[0]>-100)				 *pCunic=dMaxCunic-dBaseStd[0];
		else if(dBaseStd[1]>-100)				 *pCunic=dMaxCunic-dBaseStd[1];
		else									 *pCunic=0;

		if(*pCunic<0) *pCunic=0;
	}
}


void GetCunicValue_OtherFrame(LPBYTE fm, int nX, int nY,  int nMinX, int nMaxX, int pitch, int nMaxHeight, int nCunicSize, double *pCunic, int *pCunicValue)
{
	int i, j;
	int ll,tt,rr,bb;
	double dMaxCunic=0, dCunic;
	int nMaxPosX, nMaxPosY;
	int nSum=0, nAvg;
	int nMaxValue=0;
	int nYPart,nYPart1 ;
	double  dBaseStd[2];

	*pCunic=*pCunicValue=0;
	if(nMaxX-nMinX-2<2*nCunicSize) return;

	ll=nX-nCunicSize;   if(ll<nMinX+1) ll=nMinX+1;
	rr=ll+nCunicSize*2; if(rr>nMaxX-1) { rr=nMaxX-1;ll=rr-nCunicSize*2;}

	nYPart=nY/g_System.m_nImagePartH;
	tt=nY-nCunicSize;   if(tt<nYPart*g_System.m_nImagePartH+1) tt=nYPart*g_System.m_nImagePartH+1;
	bb=tt+nCunicSize*2; if(bb>=(nYPart+1)*g_System.m_nImagePartH-1) { bb=(nYPart+1)*g_System.m_nImagePartH-1;tt=bb-nCunicSize*2;}

	//평균값, 최대값 구함--------------------------------------------------
	for(i=tt;i<bb;i++)
		for(j=ll;j<rr;j++)
		{
			nSum+=*(fm+pitch*i+j);
			if(*(fm+pitch*i+j)>nMaxValue) nMaxValue=*(fm+pitch*i+j);
		}
	nAvg=nSum/((rr-ll)*(bb-tt));
	*pCunicValue=nMaxValue-nAvg;
	//--------------------------------------------------------------------
	
	tt=nY-nCunicSize/2-20; if(tt<0) tt=0;
	bb=tt+nCunicSize;

	for(i=0;i<5;i++)
	{
		ll=nX-nCunicSize/2-20; if(ll<nMinX+1) ll=nMinX+1;
		rr=ll+nCunicSize;

		for(j=0;j<5;j++)
		{	
			dCunic=FinStd_WithLimitCut(fm, nAvg, ll,tt,rr,bb,pitch);

			if(dCunic>dMaxCunic)
			{
				dMaxCunic=dCunic;
				nMaxPosX=ll;
				nMaxPosY=tt;
			}

			ll+=10;
			rr+=10;
			if(rr>=nMaxX-1) 
				break;
		}
		tt+=10;
		bb+=10;
		if(bb>=(nYPart+1)*g_System.m_nImagePartH-1) 
			break;
	}

	if(dMaxCunic>0)
	{
		nYPart1=nYPart+1;
		if(nYPart1>=g_System.m_nImagePart) nYPart1=0;

		if(nYPart1>nYPart)	dBaseStd[0]=FinStd_WithLimitCut(fm, nAvg, nMaxPosX,nMaxPosY+g_System.m_nImagePartH,nMaxPosX+nCunicSize,  nMaxPosY+nCunicSize+g_System.m_nImagePartH,pitch);			
		else				dBaseStd[0]=FinStd_WithLimitCut(fm, nAvg, nMaxPosX,nMaxPosY-g_System.m_nImagePartH,nMaxPosX+nCunicSize,  nMaxPosY+nCunicSize-g_System.m_nImagePartH,pitch);					

		*pCunic=dMaxCunic-dBaseStd[0];
		if(*pCunic<0) *pCunic=0;
	}
}


double FinStd_WithLimitCut(LPBYTE fm, int nAveMean, int left, int top, int right, int bottom, int pitch)
{
	int i, j, nTemp;
	long nSum,nCount;
	double dSum2, dStd;
	int nArea=(right-left)*(bottom-top);
	
	if(left<=0) left=1;
	if(top<=0)  top=1;
	if(right>=pitch) right=pitch-1;
	if(bottom>=g_System.m_nImageH) bottom=g_System.m_nImageH-1;

	nArea=(right-left)*(bottom-top);

	nSum=0;
	dSum2=0;
	nCount=0;
	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
		{
			nTemp=(*(fm+pitch*(i-1)+j-1)+*(fm+pitch*(i-1)+j)+*(fm+pitch*(i-1)+j+1)+
				  *(fm+pitch*i+j-1)+*(fm+pitch*i+j)+*(fm+pitch*i+j+1)+
				  *(fm+pitch*(i+1)+j-1)+*(fm+pitch*(i+1)+j)+*(fm+pitch*(i+1)+j+1))/9;

			if(	nTemp > nAveMean+REMOVE_W_LIMIT)	 { nTemp = nAveMean; nCount++;}
			else if( nTemp < nAveMean-REMOVE_B_LIMIT){ nTemp = nAveMean; nCount++;}

			if(nTemp > nAveMean+3)  nTemp = nAveMean+3;

			nSum+=nTemp;
			dSum2+=(nTemp*nTemp);
		}
		
	dStd=sqrt((nArea*dSum2-(double)nSum*(double)nSum)/((double)nArea*((double)nArea-1.)));	

	return dStd;
}


/*
#define REMOVE_W_LIMIT	10 
#define REMOVE_B_LIMIT	10 
double FindCunicStd(LPBYTE fmPrev, LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nLimitX1, int nLimitX2, int nSubHeight, int *nCunicValue)
{
	int i,j,k;
	int nY1, nY2;
	int nX1, nX2;
	int nSize=right-left;
	int tt,rr,ll,bb;
	int nHisto[256],nCount, nHCount, nSum, nAveMean;
	int nTemp;
	double dSum2, dStd[2], dRetStd;
	int nPos=0;
	int nMaxValue=0;

	*nCunicValue=0;
	if(nLimitX2-nLimitX1<64) return 0;

	nX1=left;
	nX2=right;
	if(nX1<nLimitX1) { nX1=nLimitX1;  nX2=nX1+nSize;}
	if(nX2>nLimitX2) { nX2=nLimitX2;  nX1=nX2-nSize;}

	if((top+bottom)/2<nSubHeight) nPos=0;
	else                          nPos=1;

	for(k=0;k<2;k++)
	{
		if(nPos==0) // 불량후보가 첫번째 있으면 
		{
			if(k==0)	//첫번째에서 구한다.
			{
				nY1=top;
				nY2=nY1+nSize;
				if(nY2>=nSubHeight)
				{
					nY2 = nSubHeight-1;
					nY1 = nY2 - nSize;
				}
			}
			else if(k==1) //두번째에서 구한다.
			{
				nY1=nY1+nSubHeight;
				nY2=nY2+nSubHeight;
			}
		}
		else //불량후보가 2번째 있음 
		{
			if(k==0) //첫번째에서 구한다.
			{
				nY1=top;
				if(nY1 < nSubHeight)	nY1=nSubHeight;
				nY2 = nY1+nSize;
				if(nY2>=g_System.m_nImageH)
				{
					nY2=g_System.m_nImageH-1;
					nY1=nY2-nSize;
				}
			}
			if(k==1) //두번째에서 구한다.
			{
				nY1=nY1-nSubHeight;
				nY2=nY2-nSubHeight;
			}
		}


			
		//48x48영역에서 중간밝기 구하기 ---------------------------------------		
   		if(nSize!=48)  //Size가 48x48이 아니면 중심값 구할때는 이걸 사용함.
		{
			ll=(nX1+nX2)/2-24; 
			rr=ll+48; 
			if(rr<nLimitX1 || rr>=nLimitX2) { ll=nX1;rr=nX2; }

			tt=(nY1+nY2)/2-24;
			bb=tt+48;
			if(tt<0 || bb>=g_System.m_nImageH || (tt<nSubHeight && bb>nSubHeight)) {tt=nY1; bb=nY2;}
		}
		else
		{
			ll=nX1; rr=nX2; tt=nY1; bb=nY2;
		}

		for(i=0;i<256;i++) nHisto[i]=0;

		nCount=0;
		for(i=tt;i<bb;i++){
			for(j=ll;j<rr;j++)
			{	
				nTemp=*(fmPrev+pitch*i+j);
				if(nTemp>nMaxValue) nMaxValue=nTemp;
				nTemp=*(fm+pitch*i+j);
				nHisto[nTemp]++;
				nCount++;
			}
		}

		nSum=0;
		nHCount=nCount/2;
		for(i=0;i<256;i++)
		{
			nSum+=nHisto[i];
			if(nSum>=nHCount)
			{
				nAveMean=i;
				break;
			}
		}
		//----------------------------------------------------------------------------


		nSum=0;
		dSum2=0;
		nCount=0;
		for(i=nY1;i<nY2;i++)
			for(j=left;j<right;j++)
			{
				nTemp=*(fm+pitch*i+j);

				if(	nTemp > nAveMean+REMOVE_W_LIMIT)	 { nTemp = nAveMean; nCount++;}
				else if( nTemp < nAveMean-REMOVE_B_LIMIT){ nTemp = nAveMean; nCount++;}


				if(nTemp > nAveMean+3)  nTemp = nAveMean+3;

				nSum+=nTemp;
				dSum2+=(nTemp*nTemp);
			}
		
		dStd[k]=sqrt((nSize*nSize*dSum2-(double)nSum*(double)nSum)/((double)nSize*nSize*((double)nSize*nSize-1.)));			
	}

	
	
	dRetStd=dStd[0]-dStd[1];	
	if(dRetStd<0) dRetStd=0;

	*nCunicValue=nMaxValue-nAveMean;


#ifdef CUNIC_NANJING	
	int nSumBright=0;
	double dAveBright;

	for(j=nX1;j<nX2;j++)
		nSumBright+=g_Profile[j];
		
	dAveBright=nSumBright/(nX2-nX1);
	if(dAveBright<45) dAveBright=45;

	dRetStd=dRetStd*60.0/dAveBright;
#endif


	return dRetStd;
}
*/