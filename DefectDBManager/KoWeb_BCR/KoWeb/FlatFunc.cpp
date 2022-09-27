#include "stdafx.h"

void MakeImageFlat( int left, int width)
{
#ifdef USE_3D_AREA

	MakeQWPFlatImage(g_fmGrab[g_ID], g_fmFlat[g_ID], left, 0, width-1, g_System.m_nImageH, g_System.m_nPitch);

#else

	if(g_System.m_nLineCAM==1) 
	{
		ThreadFunc_MakeFlat_LineImage();	//Line Scan Image 평활화
	}
	else
	{
		if(g_System.m_nImagePart==1) 
		{
			ThreadFunc_MakeFlat_LineImage();
		}
		else if(g_System.m_nImagePart==2) //CROSS Y=320x2 일떄 Thread사용 안함.
		{
			if(g_Temp.m_nNBOImage==0) Flat_Area_320_2(g_fmGrab[g_ID], g_fmFlat[g_ID], g_System.m_nFlatBright, left,width, g_System.m_nImageH, g_System.m_nPitch);
			else                      Flat_Area_320_2_NBO(g_fmGrab[g_ID], g_fmFlat[g_ID], g_System.m_nFlatBright, left,width, g_System.m_nImageH, g_System.m_nPitch);
		}
		else if(g_System.m_nImagePart==8) 
		{
			Flat_Area_320_8(g_fmGrab[g_ID], g_fmFlat[g_ID], g_System.m_nFlatBright, left,width, g_System.m_nImageH, g_System.m_nPitch);
		}
		else if(g_System.m_nImagePart==25) //투영, 투영반사 Y=128x25개 일 때
		{
			Flat_Area(25, g_fmGrab[g_ID], g_fmFlat[g_ID], g_System.m_nFlatBright, left, width, g_System.m_nImageH, g_System.m_nPitch);
		}
	}

#endif

	g_Temp.m_nFlatSuccess=1;
}

//g_Profile[전체 X] 의 Profile을 미리 구해야 한다.

//(left, top) 시작점
//width, height 수행할 범위
//pitch;
//nAvg: 영역평균밝기
//nBaseValue: 평활화 할 밝기

#if INSPECT_TYPE!=TAC_LINE
void MakeFlatLineScan(LPBYTE fmS, LPBYTE fmD, int nBaseValue, int left, int width, int height, int pitch, int nAvg)
{
	int i, j, nTmp;
	int right=left+width;
	int nFactor;
	int *pProjFactor, nProjFactor[2048];//최대 이미지 81942/8 이므로 
	int *pProfileTmp, *pProjFactorTmp;
	int nFlatBright=g_System.m_nFlatBright;
	LPBYTE fmT, fmTD;

	if(width>2048)
		pProjFactor=new int[width];
	else
		pProjFactor=nProjFactor;

	if(right<=g_Temp.m_nInspectX1 || left>=g_Temp.m_nInspectX2) //Flat할 범위가 검사영역에 속하지 않으면 Flat 하지 않는다.
	{
		for(i=0;i<height;i++)
		{
			for(j=left;j<right;j++) 
				*(fmD+pitch*i+j)=*(fmS+pitch*i+j);
		}

#ifdef USE_NEXT_GRAB
		for(i=0;i<SUB_FRAME_HEIGHT;i++)
		{
			for(j=left;j<right;j++) 
			{
				*(g_fmSubPrevFlat+pitch*i+j)=*(g_fmSubPrev+pitch*i+j);
				*(g_fmSubNextFlat+pitch*i+j)=*(g_fmSubNext+pitch*i+j);
			}
		}
#endif

	}
	else 
	{	
		//Flat Factor 만들기------------------------------------------
		for(j=left;j<right;j++)
		{
			if(g_Profile[j]>0)
			{
				nFactor=1000*nAvg/g_Profile[j];
				if(nFactor>1000) nFactor=1000+(nFactor-1000)*0.75;	// (75%) 어두우면 원래는 1.33배적용, 그러나 1 + 0.33의 75%만 적용한다. 즉 1.25적용됨.

				if(nFactor>1500)		pProjFactor[j-left]=1500;   //최대: 원본보다 1.5배 밝게
				else if(nFactor<800)	pProjFactor[j-left]=800;    //최소: 원본보다 0.8배 어둡게
				else                    pProjFactor[j-left]=nFactor;
			}
			else
			{
				pProjFactor[j-left]=1000;
			}
		}
		
		if(left<g_Temp.m_nInspectX1 && right>g_Temp.m_nInspectX1)  //왼쪽 부분이 검사영역에 속하지 않으면 원본으로 복사
		{
			for(i=0;i<height;i++)
			{
				for(j=left;j<g_Temp.m_nInspectX1;j++) 
					*(fmD+pitch*i+j)=*(fmS+pitch*i+j);
			}
			left=g_Temp.m_nInspectX1;
		}
		else if(left<g_Temp.m_nInspectX2 && right>g_Temp.m_nInspectX2)  //오른쪽 부분이 검사영역에 속하지 않으면 원본으로 복사
		{
			for(i=0;i<height;i++)
			{
				for(j=g_Temp.m_nInspectX2;j<right;j++) 
					*(fmD+pitch*i+j)=*(fmS+pitch*i+j);
			}
			right=g_Temp.m_nInspectX2;
		}

		//Flat 수행---------------------------------------------------------------------------------
		for(i=0;i<height;i++)
		{
			fmT =fmS+pitch*i+left;
			fmTD=fmD+pitch*i+left;

			pProfileTmp=g_Profile+left;
			pProjFactorTmp=pProjFactor;
			for(j=left;j<right;j++) 
			{
				nTmp=nFlatBright+( (*(fmT++)-*(pProfileTmp++))**(pProjFactorTmp++)+500)/1000;
				if(nTmp<0)        *(fmTD++)=0;
				else if(nTmp>255) *(fmTD++)=255;
				else			  *(fmTD++)=nTmp;
			}
		}

#ifdef USE_NEXT_GRAB
		for(i=0;i<SUB_FRAME_HEIGHT;i++)
		{
			//이전 Frame의 마지막 일부 영역을 현재 평활화 데이타를 사용하여 평활화 한다.-----------------
			fmT =g_fmSubPrev+pitch*i+left;
			fmTD=g_fmSubPrevFlat+pitch*i+left;

			pProfileTmp=g_Profile+left;
			pProjFactorTmp=pProjFactor;
			for(j=left;j<right;j++) 
			{
				nTmp=nFlatBright+( (*(fmT++)-*(pProfileTmp++))**(pProjFactorTmp++)+500)/1000;
				if(nTmp<0)        *(fmTD++)=0;
				else if(nTmp>255) *(fmTD++)=255;
				else			  *(fmTD++)=nTmp;
			}

			//다음 Frame의 처음 일부 영역을 현재 평활화 데이타를 사용하여 평활화 한다.------------------
			fmT =g_fmSubNext+pitch*i+left;
			fmTD=g_fmSubNextFlat+pitch*i+left;

			pProfileTmp=g_Profile+left;
			pProjFactorTmp=pProjFactor;
			for(j=left;j<right;j++) 
			{
				nTmp=nFlatBright+( (*(fmT++)-*(pProfileTmp++))**(pProjFactorTmp++)+500)/1000;
				if(nTmp<0)        *(fmTD++)=0;
				else if(nTmp>255) *(fmTD++)=255;
				else			  *(fmTD++)=nTmp;
			}
		}
#endif

		//------------------------------------------------------------------------------------------
	}

	if(width>2048) delete pProjFactor;
}	
#else
void MakeFlatLineScan(LPBYTE fmS, LPBYTE fmD, int nBaseValue, int left, int width, int height, int pitch, int nAvg)
{
	int i, j, nTmp;
	int right=left+width;
	int nFactor;
	int *pProjFactor, nProjFactor[2048];//최대 이미지 81942/8 이므로 
	int *pProfileTmp, *pProjFactorTmp;
	int nFlatBright=g_System.m_nFlatBright;
	double dAve80Ratio=1;
	LPBYTE fmT, fmTD;

	if(width>2048)
		pProjFactor=new int[width];
	else
		pProjFactor=nProjFactor;

	if(right<=g_Temp.m_nInspectX1 || left>=g_Temp.m_nInspectX2) //Flat할 범위가 검사영역에 속하지 않으면 Flat 하지 않는다.
	{
		for(i=0;i<height;i++)
		{
			for(j=left;j<right;j++) 
				*(fmD+pitch*i+j)=*(fmS+pitch*i+j);
		}
	}
	else 
	{	
		//Flat Factor 만들기------------------------------------------
		dAve80Ratio=75.0/nAvg;
		if(nAvg<70)
		{
			for(i=left;i<right;i++)
			{
				if(g_Profile[i]*dAve80Ratio>75)
					pProjFactor[i-left]=1000*75/(g_Profile[i]*dAve80Ratio);
				else
					pProjFactor[i-left]=1000*dAve80Ratio;
			}
		}
		else
		{
			for(i=left;i<right;i++)
			{
				if(g_Profile[i]*dAve80Ratio>75)
					pProjFactor[i-left]=1000*75/(g_Profile[i]*dAve80Ratio);
				else
					pProjFactor[i-left]=1000*dAve80Ratio;
			}
		}
		
		
		if(left<g_Temp.m_nInspectX1 && right>g_Temp.m_nInspectX1)  //왼쪽 부분이 검사영역에 속하지 않으면 원본으로 복사
		{
			for(i=0;i<height;i++)
			{
				for(j=left;j<g_Temp.m_nInspectX1;j++) 
					*(fmD+pitch*i+j)=*(fmS+pitch*i+j);
			}
			left=g_Temp.m_nInspectX1;
		}
		else if(left<g_Temp.m_nInspectX2 && right>g_Temp.m_nInspectX2)  //오른쪽 부분이 검사영역에 속하지 않으면 원본으로 복사
		{
			for(i=0;i<height;i++)
			{
				for(j=g_Temp.m_nInspectX2;j<right;j++) 
					*(fmD+pitch*i+j)=*(fmS+pitch*i+j);
			}
			right=g_Temp.m_nInspectX2;
		}

		//Flat 수행---------------------------------------------------------------------------------
		for(i=0;i<height;i++)
		{
			fmT =fmS+pitch*i+left;
			fmTD=fmD+pitch*i+left;

			pProfileTmp=g_Profile+left;
			pProjFactorTmp=pProjFactor;
			for(j=left;j<right;j++) 
			{
				nTmp=nFlatBright+( (*(fmT++)-*(pProfileTmp++))**(pProjFactorTmp++)+500)/1000;
				if(nTmp<0)        *(fmTD++)=0;
				else if(nTmp>255) *(fmTD++)=255;
				else			  *(fmTD++)=nTmp;
			}
		}
		//------------------------------------------------------------------------------------------
	}

	if(width>2048) delete pProjFactor;
}	
#endif


//ROI영역 평활화
void MakeFlatROI(LPBYTE fmS, LPBYTE fmD, int nBaseValue, int left, int top, int width, int height, int pitch)
{
	int i, j, nTmp;
	int nMax, nMaxPos;
	int nHisto[256];
	int *pProfile;

	pProfile=new int[width];
	for(j=left;j<left+width;j++)
	{
		memset(nHisto, 0, 256*sizeof(int));

		for(i=top;i<top+height;i++)
			nHisto[*(fmS+pitch*i+j)]++;

		nMax=0;
		for(i=3;i<256-3;i++)
		{
			nTmp=nHisto[i-3]+nHisto[i-2]+nHisto[i-1]+nHisto[i]+nHisto[i+1]+nHisto[i+2]+nHisto[i+3];
	//		nTmp=nHisto[i];
			if(nTmp>nMax)
			{
				nMax=nTmp;
				nMaxPos=(nHisto[i-3]*(i-3)+nHisto[i-2]*(i-2)+nHisto[i-1]*(i-1)+nHisto[i]*i+nHisto[i+1]*(i+1)+nHisto[i+2]*(i+2)+nHisto[i+3]*(i+3)+nTmp/2)/nTmp;
			}
		}
		pProfile[j-left]=nMaxPos;
	}
	for(i=top;i<top+height;i++)
		for(j=left;j<left+width;j++)
		{
			nTmp=nBaseValue+*(fmS+pitch*i+j)-pProfile[j-left];
			if(nTmp<0)			*(fmD+pitch*i+j)=0;
			else if(nTmp>255)   *(fmD+pitch*i+j)=255;
			else                *(fmD+pitch*i+j)=nTmp;
		}

	delete pProfile;
}


//투영 N개 Area영역 평활화 (X폴 최대 2048)
int g_Sum[MAX_CROSS_WIDTH*320];
int g_nProfileRatio[MAX_CROSS_WIDTH];
void Flat_Area(int nPart, LPBYTE fmS, LPBYTE fmD, int nBaseValue, int left, int width, int height, int pitch) 
{
	int i,j, k, nY, nOffset, nTmp;
	int nMaxAverage=0, nMaxProfile=0, nMaxProfilePos, nPosL, nPosR;
	int right		=left+width;
	int nPartHeight	=height/nPart;  //정수로 딱 떨어져야함. 
	int nWIDTH		=MAX_CROSS_WIDTH;
	int nProfile[MAX_CROSS_WIDTH]; 
	

	if(pitch>MAX_CROSS_WIDTH || nPartHeight>320) return; //범위 벗어남.

	if(width<nWIDTH) nWIDTH=width;

	//표준 이미지 만듦 (표준 SubFrame) -------------------
	memset(g_Sum, 0 , pitch*nPartHeight*sizeof(int));
	for(k=0;k<nPart;k++)
	{
		nOffset=k*nPartHeight;

		for(i=0;i<nPartHeight;i++)	
		{
			nY=nOffset+i;
			for(j=left;j<right;j++)
			{
				*(g_Sum+pitch*i+j)+=*(fmS+pitch*nY+j);
			}
		}
	}

	memset(nProfile, 0, nWIDTH*sizeof(int));
	for(i=0;i<nPartHeight;i++)
		for(j=left;j<right;j++)
		{
			nProfile[j]+=*(g_Sum+pitch*i+j);
			*(g_Sum+pitch*i+j)/=nPart;			
		}
	//------------------------------------------------------

	//밝기 비율구한다--------------------------------------------------------------
	for(j=left;j<left+width;j++)
		nProfile[j]/=(nPartHeight*nPart);
		
	nMaxProfilePos=(left+width/2); //중앙지점

	if(width!=pitch) //Boundary PC이면 
	{
		for(j=left;j<right;j++)
			if(nProfile[j]>nMaxProfile) 
			{
				nMaxProfile=nProfile[j];
				nMaxProfilePos=j;
			}
	}

	nPosL=nMaxProfilePos-50;if(nPosL<left+50)  nPosL=left +50;
	nPosR=nPosL+100;        if(nPosR>right-50) nPosR=right-50;

	for(j=nPosL;j<nPosR;j++)
		nMaxAverage+=nProfile[j];

	if(nPosR-nPosL>0) nMaxAverage/=(nPosR-nPosL);
	else              nMaxAverage=nBaseValue;

	for(j=left;j<right;j++)
	{
		if(nProfile[j]>0)
		{
			nTmp=1000*nMaxAverage/nProfile[j];
			if(nTmp>1000) g_nProfileRatio[j]=1000+(nTmp-1000)*0.75;  //Full 증폭 않고 75%만 적용한다.(경험수치) N사 0.8 사용
			else  		  g_nProfileRatio[j]=1000;
		}
		else  g_nProfileRatio[j]=1000;

		if(g_nProfileRatio[j]>1500)       g_nProfileRatio[j]=1500; //최대 1.5배 증폭
		else if(g_nProfileRatio[j]<1000)  g_nProfileRatio[j]=1000; //줄이는건 없음
	}
	//----------------------------------------------------------------------------
	ThreadFunc_MakeFlat_Too();



/*
	for(k=0;k<nPart;k++)
	{
		nOffset=k*nPartHeight;
		for(i=0;i<nPartHeight;i++)	
		{
			nY=nOffset+i;
			for(j=left;j<right;j++)
			{
				nTmp=nBaseValue+((*(fmS+pitch*nY+j)-*(g_Sum+pitch*i+j))*nProfileRatio[j]+500)/1000;
				if(nTmp<0)		  *(fmD+pitch*nY+j)=0;
				else if(nTmp>255) *(fmD+pitch*nY+j)=255;
				else              *(fmD+pitch*nY+j)=nTmp;
			}
		}
	}
*/
}


int l_Area[MAX_CROSS_WIDTH];
void Flat_Area_320_2(LPBYTE fmS, LPBYTE fmD, int nBaseValue, int left, int width, int height, int pitch) 
{
	int right=left+width;
	int i,j;
	int nTemp = 0;
	int nCount1 = 0;
	int    nImageProjection[MAX_CROSS_WIDTH];
	int    nProfileRatio[MAX_CROSS_WIDTH];
	int 	nSum, nCount, nGray, nTmp;
	int nRight;
	int nAVG=0, nMaxProfile=0, nMaxPos, nPosL, nPosR;;
	int l,m,k = 0;
	int nHeightH=height/2;

	if(nHeightH<=0) return;

	memset(nImageProjection, 0, pitch*sizeof(int));
	
	for(m=0;m<2;m++)
	{
		//한번 촬상영역의 전체 평균밝기를 구한다 -------------------------------------------------------
		for(j=left;j<right;j++) nImageProjection[j]=l_Area[j]=0;

		for(i=(m*nHeightH);i<((m+1)*nHeightH);i+=4)
			for(j=left;j<right;j++)
			{
				nImageProjection[j]+=*(fmS+i*pitch+j);
			}
		for(j=left;j<right;j++) nImageProjection[j]/=(nHeightH/4);

		nCount=nSum=0;
		for(j=left;j<right;j++)
		{
			l_Area[j]=nImageProjection[j]+15;
		}
		//------------------------------------------------------------------------------------------------

		//전체 평균보다 15이상 튄 값은 CUT하는 Projection을 구한다.----------------------------------------
		for(j=left;j<right;j++) nImageProjection[j]=0;

		for(i=(m*nHeightH);i<((m+1)*nHeightH);i++)
			for(j=left;j<right;j++)
			{
				nTemp=*(fmS+i*pitch+j);
				if(nTemp>l_Area[j]) nTemp=l_Area[j];
				nImageProjection[j]+=nTemp;
			}

		for(j=left;j<right;j++) nImageProjection[j]/=nHeightH;
		//--------------------------------------------------------------------------------------------------
		nMaxPos=pitch/2;

		if(width!=pitch)
		{
			for(j=left;j<right;j++)
				if(nImageProjection[j]>nMaxProfile)
				{
					nMaxProfile=nImageProjection[j];
					nMaxPos=j;
				}
		}
		nPosL=nMaxPos-50; if(nPosL<left+50) nPosL=left+50;   //가장 바깥쪽 +/- 50 Pixel은 사용안함.
		nPosR=nPosL+100;  if(nPosR>right-50) nPosR=right-50;

		for(j=nPosL;j<nPosR;j++) nAVG+=nImageProjection[j];
		if((nPosR-nPosL)>0) nAVG/=(nPosR-nPosL);
		else				nAVG=nBaseValue;

		for(j=left;j<right;j++)
		{
			if(nImageProjection[j]>0)
			{
				nTmp=1000*nAVG/nImageProjection[j];
				if(nTmp>1000) nProfileRatio[j]=1000+(nTmp-1000)*0.6; //60%만 증폭한다.
				else          nProfileRatio[j]=1000;
			}
			else	nProfileRatio[j]=1000;

			if(nProfileRatio[j]>1500)		 nProfileRatio[j]=1500;
			else if(nProfileRatio[j]<1000)   nProfileRatio[j]=1000;
		}

		for(i=(m*nHeightH) ;i<(m+1)*nHeightH;i++)
		{
			for(j=left;j<right;j++)
			{
				nGray=*(fmS+pitch*i+j);
				
				nTmp=nBaseValue+((nGray-nImageProjection[j])*nProfileRatio[j]+500)/1000;

				if(nTmp<0)        *(fmD+pitch*i+j)=0;
				else if(nTmp>255) *(fmD+pitch*i+j)=255;
				else              *(fmD+pitch*i+j)=nTmp;	
			}				
		}	
	}
}
void Flat_Area_320_8(LPBYTE fmS, LPBYTE fmD, int nBaseValue, int left, int width, int height, int pitch) 
{
	int right=left+width;
	int i,j;
	int nTemp = 0;
	int nCount1 = 0;
	int    nImageProjection[MAX_CROSS_WIDTH];
	int    nProfileRatio[MAX_CROSS_WIDTH];
	int 	nSum, nCount, nGray, nTmp;
	int nRight;
	int nAVG=0, nMaxProfile=0, nMaxPos, nPosL, nPosR;;
	int l,m,k = 0;
	int nHeightH=height/8;

	if(nHeightH<=0) return;

	memset(nImageProjection, 0, pitch*sizeof(int));
	
	for(m=0;m<8;m++)
	{
		//한번 촬상영역의 전체 평균밝기를 구한다 -------------------------------------------------------
		for(j=left;j<right;j++) nImageProjection[j]=l_Area[j]=0;

		for(i=(m*nHeightH);i<((m+1)*nHeightH);i+=4)
			for(j=left;j<right;j++)
			{
				nImageProjection[j]+=*(fmS+i*pitch+j);
			}
		for(j=left;j<right;j++) nImageProjection[j]/=(nHeightH/4);

		nCount=nSum=0;
		for(j=left;j<right;j++)
		{
			l_Area[j]=nImageProjection[j]+15;
		}
		//------------------------------------------------------------------------------------------------

		//전체 평균보다 15이상 튄 값은 CUT하는 Projection을 구한다.----------------------------------------
		for(j=left;j<right;j++) nImageProjection[j]=0;

		for(i=(m*nHeightH);i<((m+1)*nHeightH);i++)
			for(j=left;j<right;j++)
			{
				nTemp=*(fmS+i*pitch+j);
				if(nTemp>l_Area[j]) nTemp=l_Area[j];
				nImageProjection[j]+=nTemp;
			}

		for(j=left;j<right;j++) nImageProjection[j]/=nHeightH;
		//--------------------------------------------------------------------------------------------------
		nMaxPos=pitch/2;

		if(width!=pitch)
		{
			for(j=left;j<right;j++)
				if(nImageProjection[j]>nMaxProfile)
				{
					nMaxProfile=nImageProjection[j];
					nMaxPos=j;
				}
		}
		nPosL=nMaxPos-50; if(nPosL<left+50) nPosL=left+50;   //가장 바깥쪽 +/- 50 Pixel은 사용안함.
		nPosR=nPosL+100;  if(nPosR>right-50) nPosR=right-50;

		for(j=nPosL;j<nPosR;j++) nAVG+=nImageProjection[j];
		if((nPosR-nPosL)>0) nAVG/=(nPosR-nPosL);
		else				nAVG=nBaseValue;

		for(j=left;j<right;j++)
		{
			if(nImageProjection[j]>0)
			{
				nTmp=1000*nAVG/nImageProjection[j];
				if(nTmp>1000) nProfileRatio[j]=1000+(nTmp-1000)*0.6; //60%만 증폭한다.
				else          nProfileRatio[j]=1000;
			}
			else	nProfileRatio[j]=1000;

			if(nProfileRatio[j]>1500)		 nProfileRatio[j]=1500;
			else if(nProfileRatio[j]<1000)   nProfileRatio[j]=1000;
		}

		for(i=(m*nHeightH) ;i<(m+1)*nHeightH;i++)
		{
			for(j=left;j<right;j++)
			{
				nGray=*(fmS+pitch*i+j);
				
				nTmp=nBaseValue+((nGray-nImageProjection[j])*nProfileRatio[j]+500)/1000;

				if(nTmp<0)        *(fmD+pitch*i+j)=0;
				else if(nTmp>255) *(fmD+pitch*i+j)=255;
				else              *(fmD+pitch*i+j)=nTmp;	
			}				
		}	
	}
}

void Flat_Area_320_2_NBO(LPBYTE fmS, LPBYTE fmD, int nBaseValue, int left, int width, int height, int pitch) 
{
	int nheight= height;
	int right=left+width;
	int i,j;
	int nTemp = 0;
	int nCount1 = 0;
	int nImageProjection[2][MAX_CROSS_WIDTH], *pImageProjection, nProjSum[2];
	int nProfileRatio[MAX_CROSS_WIDTH];
	int nSum, nCount, nGray, nTmp;
	int nRight;
	int nAVG=0, nMaxProfile=0, nMaxPos, nPosL, nPosR;;
	int l,m,k = 0;
	int nHeightH=height/2;

	if(nHeightH<=0) return;

	memset(nImageProjection, 0, 2*MAX_CROSS_WIDTH*sizeof(int));

	
	nProjSum[0]=nProjSum[1]=0;
	for(m=0;m<2;m++)
	{
		pImageProjection=nImageProjection[m];
		//한번 촬상영역의 전체 평균밝기를 구한다 -------------------------------------------------------
		for(j=left;j<right;j++) pImageProjection[j]=l_Area[j]=0;

		for(i=(m*nHeightH);i<((m+1)*nHeightH);i+=4)
			for(j=left;j<right;j++)
			{
				pImageProjection[j]+=*(fmS+i*pitch+j);
			}
		for(j=left;j<right;j++)
		{
			pImageProjection[j]/=(nHeightH/4);
		}

		nCount=nSum=0;
		for(j=left;j<right;j++)
		{
			nProjSum[m]+=pImageProjection[j];
			l_Area[j]=pImageProjection[j]+15;
		}
		//------------------------------------------------------------------------------------------------

		//전체 평균보다 15이상 튄 값은 CUT하는 Projection을 구한다.----------------------------------------
		for(j=left;j<right;j++) pImageProjection[j]=0;

		for(i=(m*nHeightH);i<((m+1)*nHeightH);i++)
			for(j=left;j<right;j++)
			{
				nTemp=*(fmS+i*pitch+j);
				if(nTemp>l_Area[j]) nTemp=l_Area[j];
				pImageProjection[j]+=nTemp;
			}

		for(j=left;j<right;j++) pImageProjection[j]/=nHeightH;
		//--------------------------------------------------------------------------------------------------
	}

	if(nProjSum[0]<nProjSum[1]) pImageProjection=nImageProjection[0];
	else					    pImageProjection=nImageProjection[1];


	for(m=0;m<2;m++)
	{
		nMaxPos=width/2;
		if(width!=pitch)
		{
			for(j=left;j<right;j++)
				if(pImageProjection[j]>nMaxProfile)
				{
					nMaxProfile=pImageProjection[j];
					nMaxPos=j;
				}
		}
		nPosL=nMaxPos-50; if(nPosL<left+50)  nPosL=left+50;
		nPosR=nPosL+100;  if(nPosR>right-50) nPosR=right-50;

		for(j=nPosL;j<nPosR;j++) nAVG+=pImageProjection[j];
		if((nPosR-nPosL)>0) nAVG/=(nPosR-nPosL);
		else				nAVG=nBaseValue;

		for(j=left;j<right;j++)
		{
			if(nImageProjection[j]>0)
			{
				nTmp=1000*nAVG/pImageProjection[j];
				if(nTmp>1000) nProfileRatio[j]=1000+(nTmp-1000)*0.6;
				else          nProfileRatio[j]=1000;
			}
			else	nProfileRatio[j]=1000;

			if(nProfileRatio[j]>1500)		 nProfileRatio[j]=1500;
			else if(nProfileRatio[j]<1000)   nProfileRatio[j]=1000;
		}

		for(i=(m*nHeightH) ;i<(m+1)*nHeightH;i++)
		{
			if(i>= nheight)
				break;

			for(j=left;j<right;j++)
			{
				nGray=*(fmS+pitch*i+j);
				//if(nGray==255) 
				//{
				//	*(fmD+pitch*i+j)=255;
				//}
				//else
				{
					nTmp=nBaseValue+((nGray-pImageProjection[j])*nProfileRatio[j]+500)/1000;

					if(nTmp<0)        *(fmD+pitch*i+j)=0;
					else if(nTmp>255) *(fmD+pitch*i+j)=255;
					else			  *(fmD+pitch*i+j)=nTmp;
				}				
			}				
		}	
	}
}


int MakeQWPFlatImage(LPBYTE fm, LPBYTE fmR, int left, int top, int right, int bottom, int pitch)
{
	int i,j, k;
	int nMax, nMin;
	int nValue, nTmpMax;
	long nAvg;
	int dValue;
	int l_Proj[1392];
	

	if(right>=pitch) right=pitch-1;
	if((right-left)<=0) return 0;
	
	//Projection------------------------------------
	memset(l_Proj, 0, pitch*sizeof(long));

	for(i=top;i<bottom;i++)
		for(j=left;j<=right;j++)
			*(l_Proj+j)+=*(fm+pitch*i+j);		

	for(j=left;j<=right;j++)
		*(l_Proj+j)/=(bottom-top);

	nAvg=0;
	for(j=left;j<=right;j++)
		nAvg+=*(l_Proj+j);
	nAvg/=(right-left);

	if(nAvg==0) nAvg=1;
	dValue=1000*g_System.m_nGrabBright/nAvg;
	//----------------------------------------------

	//평균값 100을 기준으로 평활화 한다.(수정금지)--
	if(dValue<1000)
	{
		for(i=top;i<bottom;i++)
			for(j=left+1;j<right;j++)
			{
				nTmpMax=100+dValue*(*(fm+i*pitch+j)-*(l_Proj+j))/1000;
				if(nTmpMax<0) nTmpMax=0;
				else if(nTmpMax>255) nTmpMax=255;

				*(fmR+i*pitch+j)=nTmpMax;
			}
	}
	else
	{
		for(i=top;i<bottom;i++)
			for(j=left+1;j<right;j++)
			{
				nTmpMax=100+*(fm+i*pitch+j)-*(l_Proj+j);
				if(nTmpMax<0) nTmpMax=0;
				else if(nTmpMax>255) nTmpMax=255;

				*(fmR+i*pitch+j)=nTmpMax;
			}
	}

	for(i=top;i<bottom;i++)
	{
		*(fmR+i*pitch)=*(fmR+i*pitch+1);
		*(fmR+i*pitch+right)=*(fmR+i*pitch+right-1);
	}
	//----------------------------------------------

	return 0;
}