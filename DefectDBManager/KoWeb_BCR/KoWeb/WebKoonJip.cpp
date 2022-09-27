#include "stdafx.h"

void WebKoonJip()
{
	int i, j, k, ii,jj, ll, tt, rr, bb;
	int ll_, tt_, rr_, bb_, nXSize, nYSize;
	int nX, nY, nLevel, nLevel1;
	int nMaxSize=64;	//X 또는 Y크기가 64Pixel이상이면 군집에서 제외한다. 자체로 불량이 되므로
	int nMinSize, nMax;
	double dMinSize=1;
	int nBlobCount, nBlobRealCount, nKoonJipValue[200], nBlobX[200], nBlobY[200];
	int nRangeX, nRangeY, nKoonJipCount, nKoonJipCountAll, nDiffX, nDiffY;
	double dSize[200];
	bool nExistX[50], nExistY[50];
	int nGroupInspect=0;
	int nOverkill, nOverlap;
	CRect rect;
	int nLRange=0, nRRange=0;
	int nWidth, nHeight, nPitch, pitch, nInspectX1, nInspectX2, nClass, nMulti=1;
	int nXOff=2, nYOff=2;
	int nTh=g_Param.m_nGroupAreaTh;
	int nFlatValue=g_System.m_nFlatBright;

	g_Temp.m_nGroupInspect=0;
	g_Temp.m_nKoonJipCountInArea=0;
	g_Temp.m_nKoonJinCountInImage=0;

	if(g_Param.m_nGroupAreaTh<=0) return;

	LPBYTE fm=g_fmFlat[g_ID];
	LPBYTE fmB;

	nWidth		=g_System.m_nImageW;
	nHeight		=g_System.m_nImageH;
	nPitch		=g_System.m_nPitch;
	nInspectX1	=g_Temp.m_nInspectX1+10;
	nInspectX2	=g_Temp.m_nInspectX2-10;	
	if(g_System.m_nMakePyramid)
	{
		fm=l_fmPyramid[1];
		nWidth	/=2;
		nHeight	/=2;
		nPitch	/=2;
		nInspectX1/=2;
		nInspectX2/=2;
		nMulti=2;
	}
	pitch       =nPitch/2;

	rect.left	=nInspectX1;
	rect.top	=0;
	rect.right	=nInspectX2;
	rect.bottom	=nHeight;
	
	nLRange=rect.left;
	nRRange=rect.right;

	//맨가쪽이면 512 Pixel은 군집에서 제외한다. 
	if(rect.left>20)		  nLRange=rect.left +512; 
	if(rect.right<nWidth-20)  nRRange=rect.right-512;

	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])	
		{
			if(g_Param.m_nGroupAreaArray[i]>0 && g_Param.m_nGroupCountArray[i]>0 &&  g_Param.m_nGroupSizeArray[i]>0.001)
			{
				nGroupInspect=1;
				if(g_Param.m_nGroupSizeArray[i]<dMinSize) dMinSize=g_Param.m_nGroupSizeArray[i];
			}
		}
	}
	if(nGroupInspect==0) 
		return;

	g_Temp.m_nGroupInspect=1;
	nY=0;
	for(i=rect.top;i<rect.bottom-1;i+=nYOff)
	{
		nX=0;
		fmB=g_fmTemp+pitch*nY;
		for(j=rect.left;j<rect.right;j+=nXOff)
		{
			if(*(fm+nPitch*i+j)-nFlatValue>=nTh)
			{
				if(j<nLRange || j>nRRange) *(fmB++)=0;	
				else                       *(fmB++)=255;
			}
			else
				*(fmB++)=0;			   
		}
		nY++;
	}

	// 1/2압축하므로... 
	nMinSize=0.5*dMinSize*2/(g_Param.m_dScaleFactorX+g_Param.m_dScaleFactorY)/nMulti;
	if(nMinSize<1) nMinSize=1;


	g_Chain.SetChainData(1, g_fmTemp, 1, 1, nMinSize, 60/nMulti, pitch, nY);	//너무 크면 군집 제외함.
	nBlobCount=g_Chain.FastChain(0, 0, pitch-1, nY-1);
	if(nBlobCount>200) nBlobCount=200;

	for(i=0;i<nBlobCount;i++)
	{		
		ll	= g_Chain.FindMinX(i)-1;
		rr	= g_Chain.FindMaxX(i)+1;
		tt	= g_Chain.FindMinY(i)-1;
		bb	= g_Chain.FindMaxY(i)+1;

		nMax=0;
		memset(nExistX, 0, 50*sizeof(bool));
		memset(nExistY, 0, 50*sizeof(bool));

		ll_=rect.left+ll*nXOff;
		tt_=tt*nYOff;
		rr_=rect.left+rr*nXOff;
		bb_=bb*nYOff;
		if(rr_-ll_>50) rr_=ll_+50;
		if(bb_-tt_>50) bb_=tt_+50;

		for(ii=tt_;ii<bb_;ii++)
			for(jj=ll_;jj<rr_;jj++)
			{
				if(*(fm+ii*nPitch+jj)>nMax)  nMax=*(fm+ii*nPitch+jj);
				if(*(fm+ii*nPitch+jj)-nFlatValue>=nTh)
				{
					nExistX[jj-ll_]=1;
					nExistY[ii-tt_]=1;
				}
			}

		nXSize=nYSize=0;
		for(ii=0;ii<50;ii++)
		{
			if(nExistX[ii]) nXSize++;
			if(nExistY[ii]) nYSize++;
		}

		nBlobX[i]=(ll+rr)/2;
		nBlobY[i]=(tt+bb)/2;
		nKoonJipValue[i]=nMax-nFlatValue;
		dSize[i]=nMulti*(nXSize*g_Param.m_dScaleFactorX+nYSize*g_Param.m_dScaleFactorY)/2;
	}

	for(k=0;k<10;k++) //10 Level에 대해서 적용
	{
		if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) break;
		if(g_Param.m_nUseLevel[k]==0 || g_Param.m_nGroupAreaArray[k]<10 || g_Param.m_nGroupCountArray[k]<2 || g_Param.m_nGroupSizeArray[k]<0.001) continue;

		nRangeX=g_Param.m_nGroupAreaArray[k]/(2*nXOff*nMulti);
		nRangeY=g_Param.m_nGroupAreaArray[k]/(2*nYOff*nMulti);

		for(i=0;i<nBlobCount;i++)
		{
			if(dSize[i]<g_Param.m_nGroupSizeArray[k] || nKoonJipValue[i]<g_Param.m_nGroupValueArray[k]) continue;
			nKoonJipCount=nKoonJipCountAll=0;
			for(j=0;j<nBlobCount;j++)
			{
				if(dSize[j]<g_Param.m_nGroupSizeArray[k] || nKoonJipValue[j]<g_Param.m_nGroupValueArray[k]) continue;

				nDiffX=nBlobX[i]-nBlobX[j]; if(nDiffX<0) nDiffX*=-1;
				nDiffY=nBlobY[i]-nBlobY[j]; if(nDiffY<0) nDiffY*=-1;
				if(nDiffX<nRangeX && nDiffY<nRangeY)
				{
					nKoonJipCount++;
				}
				nKoonJipCountAll++;
			}
			if(nKoonJipCount>g_Temp.m_nKoonJipCountInArea)     g_Temp.m_nKoonJipCountInArea=nKoonJipCount;
			if(nKoonJipCountAll>g_Temp.m_nKoonJinCountInImage) g_Temp.m_nKoonJinCountInImage=nKoonJipCountAll;
			
			if(nKoonJipCount>=g_Param.m_nGroupCountArray[k])
			{
				//한번검출한 불량 제거------------------------------------
				nMax=0;
				for(j=0;j<nBlobCount;j++)
				{
					if(dSize[j]<g_Param.m_nGroupSizeArray[k] || nKoonJipValue[j]<g_Param.m_nGroupValueArray[k]) continue;

					nDiffX=nBlobX[i]-nBlobX[j]; if(nDiffX<0) nDiffX*=-1;
					nDiffY=nBlobY[i]-nBlobY[j]; if(nDiffY<0) nDiffY*=-1;
					if(nDiffX<nRangeX && nDiffY<nRangeY)
					{
						if(nKoonJipValue[j]>nMax) nMax=nKoonJipValue[j];
						dSize[j]=0;
					}
				}
				//--------------------------------------------------------
				nLevel=k+1;
				nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_GROUP;
				g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
				g_Defect.m_Defect[g_Defect.m_nDefectCount].value = nKoonJipValue[i];
				g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos = nMulti*(rect.left+nBlobX[i]*nXOff);
				g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos = nMulti*(nBlobY[i]*nYOff);
				g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;

				nOverkill=IsOverKillForNEL7(fm, 1, rect.left+nBlobX[i]*nXOff, nBlobY[i]*nYOff, nPitch, rect.bottom, rect.right, nTh, 0);
				if(nOverkill==0)
				{
					MakeDefectRect(g_Defect.m_nDefectCount, g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos, g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
					g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =9.0+nKoonJipCount/1000.0;; //Size
					g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =9.0+nKoonJipCount/1000.0;; //Size
					g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =9.0+nKoonJipCount/1000.0;; //Size
					CopyNGImageFM(g_fmFlat[g_ID], g_System.m_nPitch);



		//겹치는 불량 점검-----------------------------------------------------------------------------------------------
		//군집이 Size가 9.00x 로 나오므로 Size가 커서 우선이 됨.
		nOverlap=CheckDefectOverlap();
		if(nOverlap==0 && g_Defect.m_nDefectCount<g_System.m_nMaxDefect) //안겹치고 불량갯수도 최대치보다 작으면 추가함.
		{
			g_Defect.m_nDefectCount++;
		}
		else
		{
			if(nOverlap==3) CheckPriorLevel(0); //겹치는것 없고, 불량갯수가 이미 최대치에 도달했으면				
		}
		//--------------------------------------------------------------------------------------------------------------

	/*				if(g_Defect.m_nDefectCount<g_System.m_nMaxDefect) 
					{
						g_Defect.m_nDefectCount++;
					}
					else
						break;
	*/
				}
			}
		}
	}

	return;
}

// nType : 0 흑점  1 백점 
int IsOverKillForNEL7(LPBYTE fm, int nType, int nX, int nY, int pitch, int nGrabHeight, int nMinCount, int nUpValue, int nDnValue)
{
	int ret=1; //과검 
	int i, j;
	int left, top, right, bottom, nHCount=0;
	int nAvg=0;
	int nHisto[256], nHistoMax=0, nHistoSum;
	LPBYTE fmT;

	if(nMinCount<=1) nMinCount=1;
	else			 nMinCount=3;

	left=nX-BAD_IMG_WIDTH/2; if(left<0) left=0; if(left+BAD_IMG_WIDTH>=pitch) left=pitch-BAD_IMG_WIDTH;
	right=left+BAD_IMG_WIDTH;
	top=nY-BAD_IMG_HEIGHT/2; if(top<0) top=0;  if((top+BAD_IMG_HEIGHT)>=nGrabHeight) top=nGrabHeight-BAD_IMG_HEIGHT;
	bottom=top+BAD_IMG_HEIGHT;	

	memset(nHisto, 0, 256*sizeof(int));

	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
		{
			nHisto[*(fm+pitch*i+j)]++;	
		}


	for(i=0;i<256;i++)
		if(nHisto[i]>nHistoMax)
		{
			nHistoMax=nHisto[i];
			nAvg=i;
		}

	if(nType==0) //흑
	{
		nHistoSum=0;
		for(i=0;i<nAvg;i++)
		{	
			nHistoSum+=nHisto[i];
			if(nHistoSum>=nMinCount)
			{
				nHistoSum=i;
				break;
			}
		}
		if(nAvg-nHistoSum>=nDnValue) ret=0;
	}
	else 
	{
		nHistoSum=0;
		for(i=255;i>nAvg;i--)
		{	
			nHistoSum+=nHisto[i];
			if(nHistoSum>=nMinCount)
			{
				nHistoSum=i;
				break;
			}
		}
		if(nHistoSum-nAvg>=nUpValue) ret=0;
	}

	return ret;
}

extern BYTE fmPitGray[128*128];
extern BYTE fmPitBin[128*128];
int CheckMiseKoonJip(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nTh, double dSize, double dSFx, double dSFy, int nCount)
{
	int nRet=0;
	int nSum=0,nCnt=0;
	int nThres, nBlob;
	int i, j,ll,rr,tt,bb;
	double area, dSizeData;

	for(i=top;i<bottom;i+=2)
		for(j=left;j<right;j+=2)
		{
			nSum+=*(fm+pitch*i+j);
			nCnt++;
		}
	nThres=(nSum+nCnt/2)/nCnt+nTh-1;
	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
		{
			if(*(fm+pitch*i+j)>nThres) *(fmPitGray+128*(i-top)+j-left)=255;
			else					   *(fmPitGray+128*(i-top)+j-left)=0;
		}

	for(i=1;i<(bottom-top)-1;i++)
		for(j=1;j<(right-left)-1;j++)
		{
			ll=*(fmPitGray+128*(i-1)+j-1)+*(fmPitGray+128*(i-1)+j)+*(fmPitGray+128*(i-1)+j+1)+
				*(fmPitGray+128*i+j-1)+*(fmPitGray+128*i+j)+*(fmPitGray+128*i+j+1)+
				*(fmPitGray+128*(i+1)+j-1)+*(fmPitGray+128*(i+1)+j)+*(fmPitGray+128*(i+1)+j+1);

			if(ll>0) *(fmPitBin+128*i+j)=255;
			else     *(fmPitBin+128*i+j)=0;
		}

	g_Chain.SetChainData(1, fmPitBin, 1, 1, 1, 1000, 128, 128);	
	nBlob=g_Chain.FastChain(1, 1, (right-left)-2, (bottom-top)-2);
	
	nCnt=0;
	for(i=0;i<nBlob;i++)
	{
		area=g_Chain.Chain_Area(i);
		
		if(area<0) continue;
		ll=g_Chain.FindMinX(i);
		rr=g_Chain.FindMaxX(i);
		tt=g_Chain.FindMinY(i);
		bb=g_Chain.FindMaxY(i);

		dSizeData=((rr-ll-1)*dSFx+(bb-tt-1)*dSFy)*0.5;
		if(dSizeData>=dSize) nCnt++;
	}

	if(nCnt>=nCount) nRet=1;

	return nRet;
}