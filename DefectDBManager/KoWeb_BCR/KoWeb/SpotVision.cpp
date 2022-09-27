#include "stdafx.h"
#include "KoWeb.h"

//#define SHOW_PROCESS_IMAGE


//메모리 Alloc
int CSpotVision::SpotInit(int nUseMask, int width, int height)
{
	int i;
	m_nAllocFailure=0;

	for(i=0;i<7;i++)
		m_fmComp[i]=m_fmCompMask[i]=NULL;

	m_width[0]		=width;
	m_height[0]		=height;
	m_fmComp[0]		=new BYTE[width*height];
	if(nUseMask) 
		m_fmCompMask[0]	=new BYTE[width*height];

	if(m_fmComp[0]==NULL || (nUseMask && m_fmCompMask[0]==NULL))	
		m_nAllocFailure=1;

	m_fmBinary = new BYTE[width*height];
	if(m_fmBinary==NULL) m_nAllocFailure=1;

	m_fmBinaryB = new BYTE[width*height];
	if(m_fmBinaryB==NULL) m_nAllocFailure=1;
	

	if(m_nAllocFailure==0)
	{
		for(i=1;i<7;i++)
		{
			m_width[i]		=m_width[i-1]/2;
			m_height[i]		=m_height[i-1]/2;
			m_fmComp[i]		=new BYTE[m_width[i]*m_height[i]];
			m_fmCompMask[i] =new BYTE[m_width[i]*m_height[i]];

			if(m_fmComp[i]==NULL || (nUseMask && m_fmCompMask[i]==NULL) )
			{
				m_nAllocFailure=1;
				break;
			}
		}
	}

	return m_nAllocFailure;
}

void CSpotVision::SpotClose()
{
	int i;

	if(m_fmBinary) delete m_fmBinary;
	if(m_fmBinaryB) delete m_fmBinaryB;

	for(i=0;i<7;i++)
	{
		if(m_fmComp[i])		delete m_fmComp[i];
		if(m_fmCompMask[i]) delete m_fmCompMask[i];
	}
}

int CSpotVision::SpotInspect(LPBYTE fm, int pitch, int nSet, int nId)
{
	int i, j, k, ret=0;
	int nWidth[7], nHeight[7];	     //원본, 압축 6단계 각 이미지 크기
	int nMaxComp=0, nTmp;
	LPBYTE fmS, fmD, fmMask=g_Inspect[nSet].m_InspSpot[nId].m_fmMask;
	int nDefect;
	int pitchS, pitchD, nWidthS, nHeightS;
	int nOffsetY=0;
	int nWingSize=1;
	int nX, nY;

	//-------------------------------------------------------------
	int left			= g_Inspect[nSet].m_ROI1[nId].rc.left;
	int top				= g_Inspect[nSet].m_ROI1[nId].rc.top;
	int right			= g_Inspect[nSet].m_ROI1[nId].rc.right;
	int bottom			= g_Inspect[nSet].m_ROI1[nId].rc.bottom;
	int bBumpOnly		= g_Inspect[nSet].m_InspSpot[nId].m_bBumpOnly;
	int nUseMask		= g_Inspect[nSet].m_InspSpot[nId].m_bUseMask;
	int nMean			= g_Inspect[nSet].m_InspSpot[nId].m_nMeanKernel;
	int nImageCut		= g_Inspect[nSet].m_InspSpot[nId].m_bUseGrayRange;
	int nLowCut			= g_Inspect[nSet].m_InspSpot[nId].m_nGrayMin;
	int nHighCut		= g_Inspect[nSet].m_InspSpot[nId].m_nGrayMax;
	int bBlack, bWhite, nDirection[4];	
	CChain chain(2000, 50000);

	for(i=0;i<4;i++)
		nDirection[i]=g_Inspect[nSet].m_InspSpot[nId].m_bDirection[i];

	if(nLowCut<0)	 nLowCut=0;
	if(nHighCut>255) nHighCut=255;

	if(left<0) left=0;
	if(top<0)  top=0;
	if(right >=g_System.m_nImageW) right =g_System.m_nImageW-1;
	if(bottom>=g_System.m_nImageH) bottom=g_System.m_nImageH-1;	
	//-------------------------------------------------------------

	for(i=0;i<7;i++)
		if(g_Inspect[nSet].m_InspSpot[nId].m_bUseLevel[i]==1) nMaxComp=i+1;

	//압축----------------------------------------------------------------------------------------------------
	nWidth[0]=right-left;
	nHeight[0]=bottom-top;
	
	if(g_Inspect[nSet].m_InspSpot[nId].m_bUseMeanFilter) //
	{	//원본영상을 m_fmBinary에 복사후 Mean Filter 사용해서 다시 =>m_fmComp[0] 로
		if(nImageCut)
		{
			for(i=top;i<bottom;i++)
				for(j=left;j<right;j++)
				{
					nTmp=*(fm+pitch*i+j);
					if(nTmp<nLowCut)		nTmp=nLowCut;
					else if(nTmp>nHighCut)  nTmp=nHighCut;

					*(m_fmBinary+nWidth[0]*(i-top)+j-left)=nTmp;
				}
		}
		else
		{
			for(i=top;i<bottom;i++)
				memcpy(m_fmBinary+nWidth[0]*(i-top), fm+pitch*i+left, nWidth[0]);
		}

		COSS_Mean(m_fmBinary, m_fmComp[0], 0,0,nWidth[0],nHeight[0], nWidth[0], nMean*2+1, nMean*2+1);
	}
	else
	{	//원본영상 복사 =>m_fmComp[0]
		if(nImageCut)
		{
			for(i=top;i<bottom;i++)
				for(j=left;j<right;j++)
				{
					nTmp=*(fm+pitch*i+j);
					if(nTmp<nLowCut)		nTmp=nLowCut;
					else if(nTmp>nHighCut)  nTmp=nHighCut;

					*(m_fmComp[0]+nWidth[0]*(i-top)+j-left)=nTmp;
				}
		}
		else
		{
			for(i=top;i<bottom;i++)
				memcpy(m_fmComp[0]+nWidth[0]*(i-top), fm+pitch*i+left, nWidth[0]);
		}
	}

	//Filter부분추가
//	if(nFilter)
//	{
//		Func_Filter(nFilter, m_fmComp[0], m_fmBinary, 0,0,nWidth[0],nHeight[0], nWidth[0]);
//	}

//원본영상 복사 확인--------------------------------------------------
#ifdef SHOW_PROCESS_IMAGE
		for(i=0;i<nHeight[0];i++)
			for(j=0;j<nWidth[0];j++)
				*(fm+pitch*(i+nOffsetY)+j)=*(m_fmComp[0]+nWidth[0]*i+j);

		nOffsetY+=nHeight[0];
#endif
//--------------------------------------------------------------------

	

	//영상압축(x2, x4, x8, x16, x32, x64)----------------------
	for(k=1;k<nMaxComp;k++)
	{
		nWidth[k]=nWidth[k-1]/2;
		nHeight[k]=nHeight[k-1]/2;
		pitchD=nWidth[k];
		pitchS=nWidth[k-1];
		fmD=m_fmComp[k];
		fmS=m_fmComp[k-1];					

		for(i=0;i<nHeight[k];i++)
			for(j=0;j<nWidth[k];j++)
			{
				*(fmD+pitchD*i+j)=(*(fmS+pitchS*2*i+2*j)+*(fmS+pitchS*2*i+2*j+1)+*(fmS+pitchS*(2*i+1)+2*j)+*(fmS+pitchS*(2*i+1)+2*j+1)+2)/4;
			}

		if(nUseMask)
		{	
			fmD=m_fmCompMask[k];
			if(k==1) fmS=fmMask;		
			else	 fmS=m_fmCompMask[k-1];	

			for(i=0;i<nHeight[k];i++)
				for(j=0;j<nWidth[k];j++)
				{
					nTmp=*(fmS+pitchS*2*i+2*j)+*(fmS+pitchS*2*i+2*j+1)+*(fmS+pitchS*(2*i+1)+2*j)+*(fmS+pitchS*(2*i+1)+2*j+1);
					if(nTmp) *(fmD+pitchD*i+j)=1; //사용안하는 영역
					else     *(fmD+pitchD*i+j)=0; //사용하는 영역
				}	
		}

//영상압축 확인 fm의 왼쪽 끝에 그린다. (압축잘됨 확인)-----------
#ifdef SHOW_PROCESS_IMAGE
		for(i=0;i<nHeight[k];i++)
			for(j=0;j<nWidth[k];j++)
			{
				if(i+nOffsetY<g_System.m_nImageH)
					*(fm+pitch*(i+nOffsetY)+j)=*(fmD+pitchD*i+j);
			}

		nOffsetY+=nHeight[k];
#endif
//---------------------------------------------------------------
	}
	//--------------------------------------------------------------------------------------------------------------

	//Binary MAKE>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	memset(&g_InspectResult[nId],0,sizeof(INSPECTRESULT));
	g_InspectResult[nId].m_nInspectType=INSP_SPOT;

	memset(m_fmBinary, 0, nWidth[0]*nHeight[0]);
	for(k=0;k<nMaxComp;k++)
	{
		if(g_Inspect[nSet].m_InspSpot[nId].m_bUseLevel[k]==0) continue;

		fmS=m_fmComp[k];
		if(k==0) fmD=fmMask;
		else	 fmD=m_fmCompMask[k];

		nWidthS  =nWidth[k];
		nHeightS =nHeight[k];
		//-----------------------------------------------------------------

		if(bBumpOnly)
		{
			if(nUseMask) MakeBinarySubForBumpWithMask(fmS, fmD, nWidthS, nHeightS, m_fmBinary, k, g_Inspect[nSet].m_InspSpot[nId].m_nThDn[k], g_Inspect[nSet].m_InspSpot[nId].m_nThUp[k], g_Inspect[nSet].m_InspSpot[nId].m_nOffset[k]);
			else         MakeBinarySubForBump        (fmS,      nWidthS, nHeightS, m_fmBinary, k, g_Inspect[nSet].m_InspSpot[nId].m_nThDn[k], g_Inspect[nSet].m_InspSpot[nId].m_nThUp[k], g_Inspect[nSet].m_InspSpot[nId].m_nOffset[k]);
		}
		else
		{
			if(g_Inspect[nSet].m_InspSpot[nId].m_nBValue[k]>0 && g_Inspect[nSet].m_InspSpot[nId].m_dBSizeMin[k]>0 && g_Inspect[nSet].m_InspSpot[nId].m_dBSizeMax[k]-g_Inspect[nSet].m_InspSpot[nId].m_dBSizeMin[k]>0) bBlack=true;
			if(g_Inspect[nSet].m_InspSpot[nId].m_nWValue[k]>0 && g_Inspect[nSet].m_InspSpot[nId].m_dWSizeMin[k]>0 && g_Inspect[nSet].m_InspSpot[nId].m_dWSizeMax[k]-g_Inspect[nSet].m_InspSpot[nId].m_dWSizeMin[k]>0) bWhite=true;

			if(nUseMask) MakeBinarySubWithMask       (fmS, fmD, nWidthS, nHeightS, m_fmBinary, k, nDirection, bBlack, bWhite, g_Inspect[nSet].m_InspSpot[nId].m_nThDn[k], g_Inspect[nSet].m_InspSpot[nId].m_nThUp[k], g_Inspect[nSet].m_InspSpot[nId].m_nOffset[k]);
			else         MakeBinarySub               (fmS,      nWidthS, nHeightS, m_fmBinary, k, nDirection, bBlack, bWhite, g_Inspect[nSet].m_InspSpot[nId].m_nThDn[k], g_Inspect[nSet].m_InspSpot[nId].m_nThUp[k], g_Inspect[nSet].m_InspSpot[nId].m_nOffset[k]);		
		}	

		SpotBlob(nSet, nId, &chain, k, fmS, m_fmBinary, left,top, nWidthS, nHeightS, bBlack, bWhite, bBumpOnly);
	}
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	//Binary 보여주기 위해서 압축된 영상을 원본 크기로 만든다.-----------------------------------
	if(g_Inspect[nSet].m_InspSpot[nId].m_bShowBinary && nMaxComp>1)
	{
		nWingSize=1;
		for(i=1;i<nMaxComp;i++)
			nWingSize*=2;

		nWidthS  =nWidth[nMaxComp-1];
		nHeightS =nHeight[nMaxComp-1];
		for(i=0;i<nHeightS;i++)
			for(j=0;j<nWidthS;j++)
				*(m_fmBinaryB+nWidthS*i+j)=*(m_fmBinary+nWidthS*i+j);

		for(i=0;i<nHeight[0];i++)
			for(j=0;j<nWidth[0];j++)
			{
				nX=j/nWingSize;
				nY=i/nWingSize;
				if(nX< nWidthS && nY<nHeightS)
					*(m_fmBinary+nWidth[0]*i+j)=*(m_fmBinaryB+nWidthS*nY+nX);
			}
	}
	//------------------------------------------------------------------------------------------

	return ret;
}
int CSpotVision::SpotBlob(int nSet, int nId, CChain *pChain, int nComp, LPBYTE fmGray,  LPBYTE fmBinary, int left, int top, int nWidth, int nHeight, int bBlack, int bWhite, int bBumpOnly)
{
	int nNGCount=0;
	int bump   =g_Inspect[nSet].m_InspSpot[nId].m_bBumpOnly;
	int nBValue=g_Inspect[nSet].m_InspSpot[nId].m_nBValue[nComp];
	int nWValue=g_Inspect[nSet].m_InspSpot[nId].m_nWValue[nComp];
	double dBSizeMin=g_Inspect[nSet].m_InspSpot[nId].m_dBSizeMin[nComp];
	double dBSizeMax=g_Inspect[nSet].m_InspSpot[nId].m_dBSizeMax[nComp];
	double dWSizeMin=g_Inspect[nSet].m_InspSpot[nId].m_dWSizeMin[nComp];
	double dWSizeMax=g_Inspect[nSet].m_InspSpot[nId].m_dWSizeMax[nComp];
	double dScaleX=g_InspectSys[nSet].m_dScaleFactorX;
	double dScaleY=g_InspectSys[nSet].m_dScaleFactorY;
	int i, j, nBlob, ll, tt,rr, bb, nValue;
	double dSize, dMinSize, dArea;
	int nWing=1;
	int nPos, nMinSizePos;

	for(i=0;i<nComp;i++) nWing*=2;
	
	pChain->SetChainData(1, fmBinary, 1, 1, 1, 20000, nWidth, nHeight); 
	nBlob=pChain->FastChain(1,1,nWidth-2, nHeight-2);

	for(i=0;i<nBlob;i++)
	{
		//Size 범위외 버림--------------------------------------------------------------------------------------------
		dArea=pChain->Chain_Area(i);
		if(dArea<0) continue;

		ll	=pChain->FindMinX(i);
		tt	=pChain->FindMinY(i);
		rr	=pChain->FindMaxX(i);
		bb	=pChain->FindMaxY(i);
		
		dSize=((rr-ll+1)*nWing*dScaleX+(bb-tt+1)*nWing*dScaleY)/2.0;		

		//Value, Size 기준미달 버림--------------------------------------------------------------------------------------------
		if(bump)
		{
			nValue=GetBumpValue(fmBinary, fmGray, ll,tt,rr,bb, nWidth);
			if(nValue<(nBValue+nWValue))				continue;   //검정색을 사용한다.
			if(dSize<dBSizeMin || dSize>dBSizeMax || dSize<dWSizeMin || dSize>dWSizeMax)  continue; 
		}
		else
		{
			nValue=GetInValue(fmBinary, fmGray, ll,tt,rr,bb, nWidth, nBValue, nWValue);
			if(nValue>0)
			{
				if(!bWhite || nValue<nWValue)  continue;
				if(dSize<dWSizeMin || dSize>dWSizeMax) continue; 
			}
			else
			{				
				if(!bBlack || nValue*-1<nBValue) continue;
				if(dSize<dBSizeMin || dSize>dBSizeMax) continue; 
			}
		}

		nPos=nNGCount;
		if(nNGCount>=MAX_INSPECT_SPOT_LEVEL_NG_COUNT) //구한 불량이 MAX_INSPECT_SPOT_LEVEL_NG_COUNT(10)개가 찼으면 제일 작은 것을 대체한다.
		{
			dMinSize=10000;
			nMinSizePos=-1;
			for(j=0;j<MAX_INSPECT_SPOT_LEVEL_NG_COUNT;j++)
			{
				if(g_InspectResult[nId].m_dSpotNGSize[nComp][j]<dMinSize)
				{
					dMinSize=g_InspectResult[nId].m_dSpotNGSize[nComp][j];
					nMinSizePos=j;
				}
			}
			if(dSize<dMinSize) continue;
			else			   nPos=nMinSizePos;
		}
		else
		{
			nNGCount++;
		}
		g_InspectResult[nId].m_nLevelCount[nComp]=nNGCount;
		g_InspectResult[nId].m_rcSpot[nComp][nPos].left  =left+ll*nWing;
		g_InspectResult[nId].m_rcSpot[nComp][nPos].top   =top +tt*nWing;
		g_InspectResult[nId].m_rcSpot[nComp][nPos].right =left+(rr+1)*nWing;
		g_InspectResult[nId].m_rcSpot[nComp][nPos].bottom=top +(bb+1)*nWing;
		if(bump) 
		{
			g_InspectResult[nId].m_nSpotNGType[nComp][nPos]  = 2;				//BUMP
		}
		else
		{
			if(nValue>0) g_InspectResult[nId].m_nSpotNGType[nComp][nPos]  = 0;	//WHITE
			else	
			{
				nValue*=-1;
				g_InspectResult[nId].m_nSpotNGType[nComp][nPos]  = 1;	//BLACK
			}
		}
		g_InspectResult[nId].m_nSpotNGValue[nComp][nPos]=nValue;
		g_InspectResult[nId].m_dSpotNGSize[nComp][nPos]=dSize;
	}

	return nNGCount;
}

int CSpotVision::GetBumpValue(LPBYTE fmBinary, LPBYTE fmGray, int left, int top, int right, int bottom, int pitch)
{
	int i,j;
	int nBValue=255;
	int nWValue=0;
	

	for(i=top;i<=bottom;i++)
		for(j=left;j<right;j++)
		{
			if(*(fmBinary+pitch*i+j)>0)
			{
				if(*(fmGray+pitch*i+j)>nWValue) nWValue=*(fmGray+pitch*i+j);
				if(*(fmGray+pitch*i+j)<nBValue) nBValue=*(fmGray+pitch*i+j);
			}
		}


	return nWValue-nBValue;
}
int CSpotVision::GetInValue(LPBYTE fmBinary, LPBYTE fmGray, int left, int top, int right, int bottom, int pitch, int nBlack, int nWhite)
{
	int i,j, nTmp;
	int nOutCount=0;
	int nOutValue=0;
	int nOutAvg;
	int nMax=0;
	int nMin=256;
	int nWValue, nBValue;
	int nValue;

	for(i=top-2;i<=bottom+2;i++)
		for(j=left-2;j<=right+2;j++)
		{
			if(*(fmBinary+pitch*i+j)>0)
			{
				nTmp=*(fmGray+pitch*i+j);
				if(nTmp>nMax) nMax=nTmp;
				else if(nTmp<nMin) nMin=nTmp;
			}
			else
			{
				nTmp=*(fmGray+pitch*i+j);
				nOutCount++;
				nOutValue+=nTmp;
			}
		}
	if(nOutCount) nOutAvg=nOutValue/nOutCount;
	nWValue=nMax-nOutAvg;
	nBValue=nOutAvg-nMin;

	if(nBlack && nWhite)
	{
		if(nWValue>nBValue) nValue=nWValue;
		else                nValue=nBValue*-1;
	}
	else if(nWhite)
	{
		nValue=nWValue;
	}
	else if(nBlack)
	{
		 nValue=nBValue*-1;
	}
	else
	{
		nValue=0;
	}

	return nValue;
}

// 영상에서 Spot Binary 구하기-------------------------------------------------------------------------------------------
// 검사기준 위치 0 을 기준으로 방향 
//   1   2   3
//   4   0   5
//   6   7   8
//   fmS, nWidthS, nHeightS //검사할 압축 또는 원본이미지
//   fmBinary, pitch        //만들 Binary이미지 
//  nWingSize: 압축없으면 1, 압축1이면 2  압축 2이면 4 .. 압축 6이면 64
//  nDirection: 비교방향
//BYTE fmTTT[K_WIDTH*K_HEIGHT];
int CSpotVision::MakeBinarySub(LPBYTE fmS, int nWidthS, int nHeightS, LPBYTE fmBinary, int nComp, int *pDirection, BOOL bBlack, BOOL bWhite, int nThDn, int nThUp, int nOffset )
{
	int i, j, ii,jj;
	int nDefect;
	int N0, N1, N2, N3, N4, N5, N6, N7, N8;
	int nOff=4;
	int nSum, nAvg;
	int nDirection=0;

	if(nOffset==0)
	{
		switch(nComp)
		{
			case 0: nOff=6; break;
			case 1: nOff=5; break;
			case 2: nOff=4; break;
			case 3: nOff=4; break;
			case 4: nOff=2; break;
			case 5: nOff=2; break;
			case 6: nOff=1; break;
		}
	}
	else
	{
		nOff=nOffset;
	}
	if(pDirection[0] && pDirection[1] && pDirection[2] && pDirection[3]) nDirection=1;
	if(pDirection[0]+pDirection[1]+pDirection[2]+pDirection[3]==0) nDirection=2;

	if(nDirection==1) //4방향 비교해서 한개라도 걸리면 후보
	{
		for(i=nOff;i<nHeightS-nOff;i++)
			for(j=nOff;j<nWidthS-nOff;j++)
			{
				N1=*(fmS+nWidthS*(i-nOff)+j-nOff);
				N2=*(fmS+nWidthS*(i-nOff)+j  );
				N3=*(fmS+nWidthS*(i-nOff)+j+nOff);
				N4=*(fmS+nWidthS*(i  )+j-nOff);
				N0=*(fmS+nWidthS*(i  )+j  );
				N5=*(fmS+nWidthS*(i  )+j+nOff);
				N6=*(fmS+nWidthS*(i+nOff)+j-nOff);
				N7=*(fmS+nWidthS*(i+nOff)+j  );
				N8=*(fmS+nWidthS*(i+nOff)+j+nOff);

				nDefect=0;
				if(bBlack)
				{
					if((N4-N0>=nThDn && N5-N0>=nThDn) ||
						(N2-N0>=nThDn && N7-N0>=nThDn) ||
						(N1-N0>=nThDn && N8-N0>=nThDn) ||
						(N3-N0>=nThDn && N6-N0>=nThDn) ) nDefect=1;
				}
				if(nDefect==0 && bWhite)
				{
					if((N0-N4>=nThUp && N0-N5>=nThUp) ||
						(N0-N2>=nThUp && N0-N7>=nThUp) ||
						(N0-N1>=nThUp && N0-N8>=nThUp) ||
						(N0-N3>=nThUp && N0-N6>=nThUp) ) nDefect=1;
				}
				if(nDefect)
				{
					*(fmBinary+nWidthS*i+j)=255;					
				}
			}
		//수평추가 -------------------------------------------------------------------------			
		for(j=nOff;j<nWidthS-nOff;j++)
		{				
			for(i=0;i<nOff;i++)
			{
				N1=*(fmS+nWidthS*(i  )+j-nOff);
				N0=*(fmS+nWidthS*(i  )+j  );
				N2=*(fmS+nWidthS*(i  )+j+nOff);
					
				if((bBlack && N1-N0>=nThDn && N2-N0>=nThDn) ||(bWhite && N0-N1>=nThUp && N0-N2>=nThUp) ) 
				{
					*(fmBinary+nWidthS*i+j)=255;					
				}
			}
			for(i=nHeightS-nOff;i<nHeightS;i++)
			{
				N1=*(fmS+nWidthS*(i  )+j-nOff);
				N0=*(fmS+nWidthS*(i  )+j  );
				N2=*(fmS+nWidthS*(i  )+j+nOff);
					
				if((bBlack && N1-N0>=nThDn && N2-N0>=nThDn) ||(bWhite && N0-N1>=nThUp && N0-N2>=nThUp) ) 
				{
					*(fmBinary+nWidthS*i+j)=255;					
				}
			}

		}
		//수직추가
		for(i=nOff;i<nHeightS-nOff;i++)
		{
			for(j=0;j<nOff;j++)
			{					
				N1=*(fmS+nWidthS*(i-nOff)+j  );
				N0=*(fmS+nWidthS*(i  )+j  );
				N2=*(fmS+nWidthS*(i+nOff)+j  );
					
				if((bBlack && N1-N0>=nThDn && N2-N0>=nThDn) ||(bWhite && N0-N1>=nThUp && N0-N2>=nThUp) ) 
				{
					*(fmBinary+nWidthS*i+j)=255;					
				}
			}
			for(j=nWidthS-nOff;j<nWidthS;j++)
			{					
				N1=*(fmS+nWidthS*(i-nOff)+j  );
				N0=*(fmS+nWidthS*(i  )+j  );
				N2=*(fmS+nWidthS*(i+nOff)+j  );
					
				if((bBlack && N1-N0>=nThDn && N2-N0>=nThDn) ||(bWhite && N0-N1>=nThUp && N0-N2>=nThUp) ) 
				{
					*(fmBinary+nWidthS*i+j)=255;					
				}
			}
		}
	}
	else if(nDirection==2)
	{
		nSum=0;
		for(i=0;i<nHeightS;i+=2)
			for(j=0;j<nWidthS;j+=2)
				nSum+=*(fmS+nWidthS*i+j);
		nAvg=nSum*4/(nHeightS*nWidthS);

		for(i=0;i<nHeightS;i++)
			for(j=0;j<nWidthS;j++)
			{									
				N0=*(fmS+nWidthS*(i  )+j  );
								
				if((bBlack && nAvg-N0>=nThDn ) || (bWhite && N0-nAvg>=nThUp) ) 
				{
					*(fmBinary+nWidthS*i+j)=255;					
				}
			}
	}
	else
	{
		if(pDirection[0]) //수평비교 
		{
			for(i=0;i<nHeightS;i++)
				for(j=nOff;j<nWidthS-nOff;j++)
				{					
					N1=*(fmS+nWidthS*(i  )+j-nOff);
					N0=*(fmS+nWidthS*(i  )+j  );
					N2=*(fmS+nWidthS*(i  )+j+nOff);
					
					if((bBlack && N1-N0>=nThDn && N2-N0>=nThDn) ||(bWhite && N0-N1>=nThUp && N0-N2>=nThUp) ) 
					{
						*(fmBinary+nWidthS*i+j)=255;					
					}
				}
		}
		if(pDirection[1])  //수직비교
		{
			for(i=nOff;i<nHeightS-nOff;i++)
				for(j=0;j<nWidthS;j++)
				{					
					N1=*(fmS+nWidthS*(i-nOff)+j  );
					N0=*(fmS+nWidthS*(i  )+j  );
					N2=*(fmS+nWidthS*(i+nOff)+j  );
					
					if((bBlack && N1-N0>=nThDn && N2-N0>=nThDn) ||(bWhite && N0-N1>=nThUp && N0-N2>=nThUp) ) 
					{
						*(fmBinary+nWidthS*i+j)=255;					
					}
				}
		}
		if(pDirection[2])  //45 방향 비교
		{
			for(i=nOff;i<nHeightS-nOff;i++)
				for(j=nOff;j<nWidthS-nOff;j++)
				{					
					N1=*(fmS+nWidthS*(i-nOff)+j-nOff);
					N0=*(fmS+nWidthS*(i  )+j  );
					N2=*(fmS+nWidthS*(i+nOff)+j+nOff);
					
					if((bBlack && N1-N0>=nThDn && N2-N0>=nThDn) ||(bWhite && N0-N1>=nThUp && N0-N2>=nThUp) ) 
					{
						*(fmBinary+nWidthS*i+j)=255;					
					}
			}
		}
		if(pDirection[3])  //-45 방향 비교
		{
			for(i=nOff;i<nHeightS-nOff;i++)
				for(j=nOff;j<nWidthS-nOff;j++)
				{					
					N1=*(fmS+nWidthS*(i-nOff)+j+nOff);
					N0=*(fmS+nWidthS*(i  )+j  );
					N2=*(fmS+nWidthS*(i+nOff)+j-nOff);
					
					if((bBlack && N1-N0>=nThDn && N2-N0>=nThDn) ||(bWhite && N0-N1>=nThUp && N0-N2>=nThUp) ) 
					{
						*(fmBinary+nWidthS*i+j)=255;					
					}
				}
		}
	}
	
	return 0;
}
int CSpotVision::MakeBinarySubWithMask(LPBYTE fmS, LPBYTE fmMask, int nWidthS, int nHeightS, LPBYTE fmBinary, int nComp, int *pDirection, BOOL bBlack, BOOL bWhite, int nThDn, int nThUp, int nOffset )
{
	int i, j, ii,jj;
	int nDefect;
	int N0, N1, N2, N3, N4, N5, N6, N7, N8;
	int nOff=4;
	int nSum, nAvg;
	int flag45, flag27, flag18, flag36;
	int nDirection=0;

	if(nOffset==0)
	{
		switch(nComp)
		{
			case 0: nOff=6; break;
			case 1: nOff=5; break;
			case 2: nOff=4; break;
			case 3: nOff=4; break;
			case 4: nOff=2; break;
			case 5: nOff=2; break;
			case 6: nOff=1; break;
		}
	}
	else
	{
		nOff=nOffset;
	}
	if(pDirection[0] && pDirection[1] && pDirection[2] && pDirection[3]) nDirection=1;
	if(pDirection[0]+pDirection[1]+pDirection[2]+pDirection[3]==0) nDirection=2;

	switch(nComp)
	{
		case 0: nOff=6; break;
		case 1: nOff=5; break;
		case 2: nOff=4; break;
		case 3: nOff=4; break;
		case 4: nOff=2; break;
		case 5: nOff=2; break;
		case 6: nOff=1; break;
	}

	if(nDirection==1) //4방향 비교해서 한개라도 걸리면 후보
	{
		for(i=nOff;i<nHeightS-nOff;i++)
			for(j=nOff;j<nWidthS-nOff;j++)
			{
				//Mask 추가----------------------------
				N1=*(fmMask+nWidthS*(i-nOff)+j-nOff);
				N2=*(fmMask+nWidthS*(i-nOff)+j  );
				N3=*(fmMask+nWidthS*(i-nOff)+j+nOff);
				N4=*(fmMask+nWidthS*(i  )+j-nOff);
				N0=*(fmMask+nWidthS*(i  )+j  );
				N5=*(fmMask+nWidthS*(i  )+j+nOff);
				N6=*(fmMask+nWidthS*(i+nOff)+j-nOff);
				N7=*(fmMask+nWidthS*(i+nOff)+j  );
				N8=*(fmMask+nWidthS*(i+nOff)+j+nOff);
				
				flag45=flag27=flag18=flag36=1;
				if(N4+N5) flag45=0;
				if(N2+N7) flag27=0;
				if(N1+N8) flag18=0;
				if(N3+N6) flag36=0;
			//	if(N0+N1+N2+N3+N4+N5+N6+N7+N8) continue;
				//--------------------------------------


				N1=*(fmS+nWidthS*(i-nOff)+j-nOff);
				N2=*(fmS+nWidthS*(i-nOff)+j  );
				N3=*(fmS+nWidthS*(i-nOff)+j+nOff);
				N4=*(fmS+nWidthS*(i  )+j-nOff);
				N0=*(fmS+nWidthS*(i  )+j  );
				N5=*(fmS+nWidthS*(i  )+j+nOff);
				N6=*(fmS+nWidthS*(i+nOff)+j-nOff);
				N7=*(fmS+nWidthS*(i+nOff)+j  );
				N8=*(fmS+nWidthS*(i+nOff)+j+nOff);

				nDefect=0;
				if(bBlack)
				{
					if( (flag45 && N4-N0>=nThDn && N5-N0>=nThDn) ||
						(flag27 && N2-N0>=nThDn && N7-N0>=nThDn) ||
						(flag18 && N1-N0>=nThDn && N8-N0>=nThDn) ||
						(flag36 && N3-N0>=nThDn && N6-N0>=nThDn) ) nDefect=1;
				}
				if(nDefect==0 && bWhite)
				{
					if( (flag45 && N0-N4>=nThUp && N0-N5>=nThUp) ||
						(flag27 && N0-N2>=nThUp && N0-N7>=nThUp) ||
						(flag18 && N0-N1>=nThUp && N0-N8>=nThUp) ||
						(flag36 && N0-N3>=nThUp && N0-N6>=nThUp) ) nDefect=1;
				}
				if(nDefect)
				{
					*(fmBinary+nWidthS*i+j)=255;					
				}
			}
		//수평추가 -------------------------------------------------------------------------			
		for(j=nOff;j<nWidthS-nOff;j++)
		{				
			for(i=0;i<nOff;i++)
			{
				//Mask 추가-------------------------
				N1=*(fmMask+nWidthS*(i  )+j-nOff);
				N0=*(fmMask+nWidthS*(i  )+j  );
				N2=*(fmMask+nWidthS*(i  )+j+nOff);
				if(N0+N1+N2) continue;
				//----------------------------------

				N1=*(fmS+nWidthS*(i  )+j-nOff);
				N0=*(fmS+nWidthS*(i  )+j  );
				N2=*(fmS+nWidthS*(i  )+j+nOff);
					
				if((bBlack && N1-N0>=nThDn && N2-N0>=nThDn) ||(bWhite && N0-N1>=nThUp && N0-N2>=nThUp) ) 
				{
					*(fmBinary+nWidthS*i+j)=255;					
				}
			}
			for(i=nHeightS-nOff;i<nHeightS;i++)
			{
				//Mask 추가-------------------------
				N1=*(fmMask+nWidthS*(i  )+j-nOff);
				N0=*(fmMask+nWidthS*(i  )+j  );
				N2=*(fmMask+nWidthS*(i  )+j+nOff);
				if(N0+N1+N2) continue;
				//----------------------------------

				N1=*(fmS+nWidthS*(i  )+j-nOff);
				N0=*(fmS+nWidthS*(i  )+j  );
				N2=*(fmS+nWidthS*(i  )+j+nOff);
					
				if((bBlack && N1-N0>=nThDn && N2-N0>=nThDn) ||(bWhite && N0-N1>=nThUp && N0-N2>=nThUp) ) 
				{
					*(fmBinary+nWidthS*i+j)=255;					
				}
			}

		}
		//수직추가
		for(i=nOff;i<nHeightS-nOff;i++)
		{
			for(j=0;j<nOff;j++)
			{
				//Mask 추가-------------------------
				N1=*(fmMask+nWidthS*(i-nOff)+j  );
				N0=*(fmMask+nWidthS*(i  )+j  );
				N2=*(fmMask+nWidthS*(i+nOff)+j  );
				if(N0+N1+N2) continue;
				//----------------------------------

				N1=*(fmS+nWidthS*(i-nOff)+j  );
				N0=*(fmS+nWidthS*(i  )+j  );
				N2=*(fmS+nWidthS*(i+nOff)+j  );
					
				if((bBlack && N1-N0>=nThDn && N2-N0>=nThDn) ||(bWhite && N0-N1>=nThUp && N0-N2>=nThUp) ) 
				{
					*(fmBinary+nWidthS*i+j)=255;					
				}
			}
			for(j=nWidthS-nOff;j<nWidthS;j++)
			{					
				//Mask 추가-------------------------
				N1=*(fmMask+nWidthS*(i-nOff)+j  );
				N0=*(fmMask+nWidthS*(i  )+j  );
				N2=*(fmMask+nWidthS*(i+nOff)+j  );
				if(N0+N1+N2) continue;
				//----------------------------------

				N1=*(fmS+nWidthS*(i-nOff)+j  );
				N0=*(fmS+nWidthS*(i  )+j  );
				N2=*(fmS+nWidthS*(i+nOff)+j  );
					
				if((bBlack && N1-N0>=nThDn && N2-N0>=nThDn) ||(bWhite && N0-N1>=nThUp && N0-N2>=nThUp) ) 
				{
					*(fmBinary+nWidthS*i+j)=255;					
				}
			}
		}
	}
	else if(nDirection==2)
	{
		nSum=0;
		for(i=0;i<nHeightS;i+=2)
			for(j=0;j<nWidthS;j+=2)
				nSum+=*(fmS+nWidthS*i+j);
		nAvg=nSum*4/(nHeightS*nWidthS);

		for(i=0;i<nHeightS;i++)
			for(j=0;j<nWidthS;j++)
			{			
				if(*(fmMask+nWidthS*i+j)) continue;

				N0=*(fmS+nWidthS*i+j  );				
					
				if((bBlack && nAvg-N0>=nThDn ) || (bWhite && N0-nAvg>=nThUp) ) 
				{
					*(fmBinary+nWidthS*i+j)=255;					
				}
			}
	}
	else
	{
		if(pDirection[0]) //수평비교 
		{
			for(i=0;i<nHeightS;i++)
				for(j=nOff;j<nWidthS-nOff;j++)
				{					
					//Mask 추가-------------------------
					N1=*(fmMask+nWidthS*(i  )+j-nOff);
					N0=*(fmMask+nWidthS*(i  )+j  );
					N2=*(fmMask+nWidthS*(i  )+j+nOff);
					if(N0+N1+N2) continue;
					//----------------------------------	

					N1=*(fmS+nWidthS*(i  )+j-nOff);
					N0=*(fmS+nWidthS*(i  )+j  );
					N2=*(fmS+nWidthS*(i  )+j+nOff);
					
					if((bBlack && N1-N0>=nThDn && N2-N0>=nThDn) ||(bWhite && N0-N1>=nThUp && N0-N2>=nThUp) ) 
					{
						*(fmBinary+nWidthS*i+j)=255;					
					}
				}
		}
		if(pDirection[1]) //수직비교
		{
			for(i=nOff;i<nHeightS-nOff;i++)
				for(j=0;j<nWidthS;j++)
				{					
					//Mask 추가-------------------------
					N1=*(fmMask+nWidthS*(i-nOff)+j  );
					N0=*(fmMask+nWidthS*(i  )+j  );
					N2=*(fmMask+nWidthS*(i+nOff)+j  );
					if(N0+N1+N2) continue;
					//----------------------------------

					N1=*(fmS+nWidthS*(i-nOff)+j  );
					N0=*(fmS+nWidthS*(i  )+j  );
					N2=*(fmS+nWidthS*(i+nOff)+j  );
					
					if((bBlack && N1-N0>=nThDn && N2-N0>=nThDn) ||(bWhite && N0-N1>=nThUp && N0-N2>=nThUp) ) 
					{
						*(fmBinary+nWidthS*i+j)=255;					
					}
				}
		}
		if(pDirection[2]) //45 방향 비교
		{
			for(i=nOff;i<nHeightS-nOff;i++)
				for(j=nOff;j<nWidthS-nOff;j++)
				{					
					//Mask 추가-------------------------
					N1=*(fmMask+nWidthS*(i-nOff)+j-nOff);
					N0=*(fmMask+nWidthS*(i  )+j  );
					N2=*(fmMask+nWidthS*(i+nOff)+j+nOff);
					if(N0+N1+N2) continue;
					//-----------------------------------

					N1=*(fmS+nWidthS*(i-nOff)+j-nOff);
					N0=*(fmS+nWidthS*(i  )+j  );
					N2=*(fmS+nWidthS*(i+nOff)+j+nOff);
					
					if((bBlack && N1-N0>=nThDn && N2-N0>=nThDn) ||(bWhite && N0-N1>=nThUp && N0-N2>=nThUp) ) 
					{
						*(fmBinary+nWidthS*i+j)=255;					
					}
				}
		}
		if(pDirection[3]) //-45 방향 비교
		{
			for(i=nOff;i<nHeightS-nOff;i++)
				for(j=nOff;j<nWidthS-nOff;j++)
				{					
					//Mask 추가-------------------------
					N1=*(fmMask+nWidthS*(i-nOff)+j+nOff);
					N0=*(fmMask+nWidthS*(i  )+j  );
					N2=*(fmMask+nWidthS*(i+nOff)+j-nOff);
					if(N0+N1+N2) continue;
					//-------------------------

					N1=*(fmS+nWidthS*(i-nOff)+j+nOff);
					N0=*(fmS+nWidthS*(i  )+j  );
					N2=*(fmS+nWidthS*(i+nOff)+j-nOff);
					
					if((bBlack && N1-N0>=nThDn && N2-N0>=nThDn) ||(bWhite && N0-N1>=nThUp && N0-N2>=nThUp) ) 
					{
						*(fmBinary+nWidthS*i+j)=255;					
					}
				}
		}		
	}
	
	return 0;
}
// 영상에서 Spot Binary 구하기-------------------------------------------------------------------------------------------
// 검사기준 위치 0 을 기준으로 방향 
//   1   2   3
//   4   0   5
//   6   7   8
//   fmS, nWidthS, nHeightS //검사할 압축 또는 원본이미지
//   fmBinary, pitch        //만들 Binary이미지 
//  nWingSize: 압축없으면 1, 압축1이면 2  압축 2이면 4 .. 압축 6이면 64
//  nDirection: 비교방향
int CSpotVision::MakeBinarySubForBump(LPBYTE fmS, int nWidthS, int nHeightS, LPBYTE fmBinary,int nComp, int nThDn, int nThUp, int nOffset )
{
	int i, j, ii,jj;
	int nDefect;
	int N0, N1, N2, N3, N4, N5, N6, N7, N8;
	int nOff=4;

	if(nOffset==0)
	{
		switch(nComp)
		{
			case 0: nOff=6; break;
			case 1: nOff=5; break;
			case 2: nOff=4; break;
			case 3: nOff=4; break;
			case 4: nOff=2; break;
			case 5: nOff=2; break;
			case 6: nOff=1; break;
		}
	}
	else
	{
		nOff=nOffset;
	}

	for(i=nOff;i<nHeightS-nOff;i++)
		for(j=0;j<nWidthS;j++)
		{	
			N1=*(fmS+nWidthS*(i-nOff)+j  );
			N0=*(fmS+nWidthS*(i  )+j  );
			N2=*(fmS+nWidthS*(i+nOff)+j  );
					
			if(( N1-N0>=nThUp && N0-N2>=nThDn) || ( N2-N0>=nThUp && N0-N1>=nThDn) || 
			   ( N2-N1>=nThUp && N1-N0>=nThDn) || ( N0-N1>=nThUp && N1-N2>=nThDn) ||
			   ( N0-N2>=nThUp && N2-N1>=nThDn) || ( N1-N2>=nThUp && N2-N0>=nThDn)    )
				{
					*(fmBinary+nWidthS*i+j)=255;					
				}
		}		
	
	return 0;
}

int CSpotVision::MakeBinarySubForBumpWithMask(LPBYTE fmS, LPBYTE fmMask, int nWidthS, int nHeightS, LPBYTE fmBinary,  int nComp, int nThDn, int nThUp, int nOffset )
{
	int i, j, ii,jj;
	int nDefect;
	int N0, N1, N2, N3, N4, N5, N6, N7, N8;
	int nOff=4;

	if(nOffset==0)
	{
		switch(nComp)
		{
			case 0: nOff=6; break;
			case 1: nOff=5; break;
			case 2: nOff=4; break;
			case 3: nOff=4; break;
			case 4: nOff=2; break;
			case 5: nOff=2; break;
			case 6: nOff=1; break;
		}
	}
	else
	{
		nOff=nOffset;
	}

	for(i=nOff;i<nHeightS-nOff;i++)
		for(j=0;j<nWidthS;j++)
		{	
			//Mask 추가-------------------------
			N1=*(fmMask+nWidthS*(i-nOff)+j  );
			N0=*(fmMask+nWidthS*(i  )+j  );
			N2=*(fmMask+nWidthS*(i+nOff)+j  );
			if(N0+N1+N2) continue;
			//-----------------------------------

			N1=*(fmS+nWidthS*(i-nOff)+j  );
			N0=*(fmS+nWidthS*(i  )+j  );
			N2=*(fmS+nWidthS*(i+nOff)+j  );					

			if(( N1-N0>=nThUp && N0-N2>=nThDn) || ( N2-N0>=nThUp && N0-N1>=nThDn) || 
			   ( N2-N1>=nThUp && N1-N0>=nThDn) || ( N0-N1>=nThUp && N1-N2>=nThDn) ||
			   ( N0-N2>=nThUp && N2-N1>=nThDn) || ( N1-N2>=nThUp && N2-N0>=nThDn)    )
			{
				*(fmBinary+nWidthS*i+j)=255;					
			}
		}
	
	return 0;
}




