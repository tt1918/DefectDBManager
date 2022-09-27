#include "Stdafx.h"
#include "math.h"
#include "KoWebDoc.h"
#include "KoWebView.h"


int *l_fmT;
int *l_fmMuraINT;
int *l_fmMuraPrevArray;
int  l_fmMuraPrev=0;

double StdevDataForMura(int * fm, int x1, int y1, int x2, int y2, int pitch);
int COSS_Sum(int * fmS, int * fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY);
int Erode_Fast(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch);


#define CUT_HIGH	81
#define CUT_LOW		73

void WEB_MuraMalloc()
{
	if(g_System.m_nImageW>=MAX_WIDTH)
	{
		l_fmT		=new int [g_System.m_nImageW*g_System.m_nImageH/16];
		l_fmMuraINT	=new int [g_System.m_nImageW*g_System.m_nImageH/16];
	}
	else
	{
		l_fmT		=new int [g_System.m_nImageW*g_System.m_nImageH/4];
		l_fmMuraINT	=new int [g_System.m_nImageW*g_System.m_nImageH/4];
	}
	l_fmMuraPrevArray=new int[g_System.m_nImageW/2*50];
}
void WEB_MuraFree()
{
	if(l_fmT)				 delete l_fmT;
	if(l_fmMuraINT)			 delete l_fmMuraINT;
	if(l_fmMuraPrevArray)	 delete l_fmMuraPrevArray;
}
void WEB_MuraSTD()
{
	CRect rect;
	int MURA_RECT=40;
	unsigned char* fm = g_fmFlat[g_ID]; //평활화 이미지	
	int i,j, ii,jj,k,nTmp;
	int nLevelInspect=0, nClass;
	int width, height;
	int nX, nY;
	int nX1, nX2, nY1, nY2;
	int ll_, tt_,rr_,bb_;
	double dBlackThresStd=100000, dWhiteThresStd=100000, dStdev, dMaxStdev, dMaxStdevSide, dStdevDiff;
	int nCx[4], nCy[4], nMin[4], nPos, nLevel;
	int nCxMax, nCyMax;
	int *pFM;
	int nPitchDest, nXi, nYi;
	int nAVG=14;  //8192면 64x64,  4096이면 32x32 에서 후보 구함.
	int nDiv;
	int pitch=g_System.m_nPitch;
	LPBYTE fmDest;

	rect.left	=g_Temp.m_nInspectX1;
	rect.top	=0;
	rect.right	=g_Temp.m_nInspectX2;
	rect.bottom	=g_System.m_nImageH;

	if(g_System.m_nImageW>=MAX_WIDTH && g_System.m_nImageH>=MAX_WIDTH) { nDiv=8; nAVG=7; MURA_RECT=32;}
	else if(g_System.m_nImageW>=MAX_WIDTH)							   { nDiv=4; nAVG=14;MURA_RECT=40;}
	else															   { nDiv=2; nAVG=14;MURA_RECT=40;}

	if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) return;
	if(rect.right-rect.left<128*nDiv)				   return;
	if(g_Temp.m_nGrabFrameID<10) l_fmMuraPrev=0;	//Lot바뀌고 초기는 자신의 이미지 만으로 처리한다.

	g_Temp.m_dMuraMaxValue=0;

	//검사 Check-------------------------------------------------------------------------------------------------------------------------
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])	
		{			
			if(g_Param.m_nSTDThDn[i]>0 && g_Param.m_nSTDThDn[i]<dBlackThresStd) { dBlackThresStd=g_Param.m_nSTDThDn[i];nLevelInspect=1;}			
			if(g_Param.m_nSTDThUp[i]>0 && g_Param.m_nSTDThUp[i]<dWhiteThresStd) { dWhiteThresStd=g_Param.m_nSTDThUp[i];nLevelInspect=1;}			
		}
	}
	if(nLevelInspect==0) return;
	//------------------------------------------------------------------------------------------------------------------------------------

	if(g_System.m_nImageW>=MAX_WIDTH && g_System.m_nImageH>=MAX_WIDTH) 
	{
/*		width=g_System.m_nImageW/4;
		height=g_System.m_nImageH/4;

		nY=0;
		for(i=0;i<g_System.m_nImageH-3;i+=4)
		{
			nX=0;
			for(j=0;j<g_System.m_nImageW-3;j+=4)
			{
				nTmp=(*(fm+pitch*i+j)+*(fm+pitch*i+(j+2))+*(fm+pitch*(i+2)+j)+*(fm+pitch*(i+2)+(j+2)) + 2)/4;
				if(nTmp>CUT_HIGH) nTmp=CUT_HIGH;
				else if(nTmp<CUT_LOW) nTmp=CUT_LOW;
				*(l_fmMuraINT+width*nY+nX)=nTmp;
				nX++;
			}
			nY++;
		}
		COSS_Sum(l_fmMuraINT, l_fmT, 0, 0, width, height,width, nAVG, nAVG);
		pFM=l_fmMuraINT;
*/
		width=g_System.m_nImageW/nDiv;
		height=g_System.m_nImageH/nDiv;

		nY=0;
		for(i=0;i<g_System.m_nImageH-7;i+=nDiv)
		{
			nX=0;
			for(j=0;j<g_System.m_nImageW-7;j+=nDiv)
			{
				nTmp=(*(fm+pitch*i+j)+*(fm+pitch*i+(j+4))+*(fm+pitch*(i+4)+j)+*(fm+pitch*(i+4)+(j+4)) + 2)/4;
				if(nTmp>CUT_HIGH) nTmp=CUT_HIGH;
				else if(nTmp<CUT_LOW) nTmp=CUT_LOW;
				*(l_fmMuraINT+width*nY+nX)=nTmp;
				nX++;
			}
			nY++;
		}
		COSS_Sum(l_fmMuraINT, l_fmT, 0, 0, width, height,width, nAVG, nAVG);
		pFM=l_fmMuraINT;
	}
	else if(g_System.m_nImageW>=MAX_WIDTH) 
	{
		width=g_System.m_nImageW/nDiv;
		height=g_System.m_nImageH/nDiv;

		nY=0;
		for(i=0;i<g_System.m_nImageH-3;i+=nDiv)
		{
			nX=0;
			for(j=0;j<g_System.m_nImageW-3;j+=nDiv)
			{
				nTmp=(*(fm+pitch*i+j)+*(fm+pitch*i+(j+2))+*(fm+pitch*(i+2)+j)+*(fm+pitch*(i+2)+(j+2)) + 2)/4;
				if(nTmp>CUT_HIGH) nTmp=CUT_HIGH;
				else if(nTmp<CUT_LOW) nTmp=CUT_LOW;
				*(l_fmMuraINT+width*nY+nX)=nTmp;
				nX++;
			}
			nY++;
		}
		COSS_Sum(l_fmMuraINT, l_fmT, 0, 0, width, height,width, nAVG, nAVG);
		pFM=l_fmMuraINT;
	}
	else// if(g_System.m_nImageW==4096) 
	{
		width=g_System.m_nImageW/nDiv;
		height=g_System.m_nImageH/nDiv;

		nY=0;
		for(i=0;i<g_System.m_nImageH-1;i+=nDiv)
		{
			nX=0;
			for(j=0;j<g_System.m_nImageW-1;j+=nDiv)
			{
				nTmp=(*(fm+pitch*i+j)+*(fm+pitch*i+(j+1))+*(fm+pitch*(i+1)+j)+*(fm+pitch*(i+1)+(j+1)) + 2)/4;
				if(nTmp>CUT_HIGH) nTmp=CUT_HIGH;
				else if(nTmp<CUT_LOW) nTmp=CUT_LOW;
				*(l_fmMuraINT+width*nY+nX)=nTmp;
				nX++;
			}
			nY++;
		}
		COSS_Sum(l_fmMuraINT, l_fmT, 0, 0, width, height,width, nAVG, nAVG);	
		pFM=l_fmMuraINT;
	}

	//압축영상 보여주기-------------------------------
//	for(i=0;i<g_System.m_nImageH/4;i++)
//		for(j=0;j<g_System.m_nImageW/4;j++)
//			*(fm+pitch*i+j)=*(l_fmT+width*i+j);
	//------------------------------------------------
		
	//4분할 해서 한개씩 후보 구함.
	for(k=0;k<4;k++)
	{
		if(k==0)		{nX1=nAVG/2;	nX2=width/2;		nY1=nAVG/2;		nY2=height/2; }
		else if(k==1)   {nX1=nAVG/2;	nX2=width/2;		nY1=height/2;	nY2=height-nAVG/2;}
		else if(k==2)	{nX1=width/2;	nX2=width-nAVG/2; 	nY1=nAVG/2;		nY2=height/2;}
		else if(k==3)   {nX1=width/2;	nX2=width-nAVG/2; 	nY1=height/2;	nY2=height-nAVG/2;}
		
		nMin[k]=255*nAVG*nAVG;

		if(nX2<rect.left/nDiv+MURA_RECT/2)   continue;
		if(nX1>rect.right/nDiv-MURA_RECT/2)  continue;

		if(nX1<rect.left /nDiv+MURA_RECT/2)  nX1=rect.left /nDiv+MURA_RECT/2;
		if(nX2>rect.right/nDiv-MURA_RECT/2)  nX2=rect.right/nDiv-MURA_RECT/2;

		if((nX2-nX1)<MURA_RECT*nDiv) continue;
		
		for(i=nY1;i<nY2;i++)
			for(j=nX1;j<nX2;j++)
			{
				if(*(l_fmT+width*i+j)<nMin[k])
				{
					nMin[k]=*(l_fmT+width*i+j);
					nCx[k]=j;
					nCy[k]=i;
				}
			}
	}

	for(k=0;k<4;k++)
	{
		//제일 작은 값을 찾는다---------------------------
		nTmp=255*nAVG*nAVG;
		nPos=-1;
		for(i=0;i<4;i++)
			if(nMin[i]<nTmp)
			{
				nTmp=nMin[i];
				nPos=i;
			}
		if(nPos<0) 		break;	//255보다 작은게 없으면 끝	
		//------------------------------------------------

		nMin[nPos]=255*nAVG*nAVG;
		if(nCx[nPos]<rect.left/nDiv+MURA_RECT/2 || nCx[nPos]>rect.right/nDiv-MURA_RECT/2) continue;		

		nY1=nCy[nPos]-MURA_RECT/2-30; 
		if(l_fmMuraPrev==0)
		{
			if(nY1<0)			 nY1=0;
		}
		nY2=nY1+MURA_RECT;	      if(nY2+60>height)  {nY1=height-60-MURA_RECT; nY2=nY1+MURA_RECT;}
		dMaxStdev=0;

		for(i=0;i<7;i++) //7바꾸면 안됨. 위와 연결되어 있음.
		{
			nX1=nCx[nPos]-MURA_RECT; if(nX1<rect.left/nDiv) nX1=rect.left/nDiv;
			nX2=nX1+MURA_RECT;       		

			for(j=0;j<5;j++)
			{
				if(nX2<=rect.right/nDiv)
				{
					dStdev=StdevDataForMura(pFM, nX1, nY1, nX2, nY2, width);
					if(dStdev>dMaxStdev)
					{
						dMaxStdev=dStdev;
						nCxMax=(nX1+nX2)/2;
						nCyMax=(nY1+nY2)/2;
					}
				}
				nX1+=10;  //바꾸면 안됨
				nX2+=10;
			}
			nY1+=10;	 //바꾸면 안됨
			nY2+=10;
		}
		if(dMaxStdev<=0.1) continue;

		nCx[nPos]=nCxMax;

		//옆에 한번더-----------------------------------------------------------
		nX1=nCx[nPos]-MURA_RECT/2; if(nX1<rect.left/nDiv)   nX1=rect.left/nDiv;
		nX2=nX1+MURA_RECT;         if(nX2>rect.right/nDiv) {nX2=rect.right/nDiv; nX1=nX2-MURA_RECT;}	
		nY1=nCy[nPos]-MURA_RECT/2-30; 
		if(l_fmMuraPrev==0)
		{
			if(nY1<0)			 nY1=0;
		}
		nY2=nY1+MURA_RECT;	      if(nY2+60>height)  {nY1=height-60-MURA_RECT; nY2=nY1+MURA_RECT;}
		dMaxStdevSide=0;
		for(i=0;i<7;i++)
		{
			if(nX1-MURA_RECT*2>=rect.left/nDiv)
			{
				dStdev=StdevDataForMura(pFM, nX1-MURA_RECT*2, nY1, nX2-MURA_RECT*2, nY2, width);
				if(dStdev>dMaxStdevSide) dMaxStdevSide=dStdev;
			}
			if(nX2+MURA_RECT*2<=rect.right/nDiv)
			{
				dStdev=StdevDataForMura(pFM, nX1+MURA_RECT*2, nY1, nX2+MURA_RECT*2, nY2, width);
				if(dStdev>dMaxStdevSide) dMaxStdevSide=dStdev;
			}
			nY1+=10;
			nY2+=10;			
		}
		//-----------------------------------------------------------------------
		nCy[nPos]=nCyMax;

		dStdevDiff=0;
		nY1=nCy[nPos]-MURA_RECT*2.5;
		if(nY1>=0)
		{
			nY2=nY1+MURA_RECT;
			dStdev=StdevDataForMura(pFM, nX1, nY1, nX2, nY2, width);
			dStdevDiff=(dMaxStdev-dStdev)*1000;
		}
		else
		{
			nY1=nCy[nPos]+MURA_RECT*2.5;
			nY2=nY1+MURA_RECT;
			dStdev=StdevDataForMura(pFM, nX1, nY1, nX2, nY2, width);
			if((dMaxStdev-dStdev)*1000 > dStdevDiff)
			{
				dStdevDiff=(dMaxStdev-dStdev)*1000;
			}
		}
		
		nY1=nCy[nPos]+MURA_RECT*1.5;
		nY2=nY1+MURA_RECT;
		if(nY2<=height)
		{
			dStdev=StdevDataForMura(pFM, nX1, nY1, nX2, nY2, width);
			if((dMaxStdev-dStdev)*1000 > dStdevDiff)
			{
				dStdevDiff=(dMaxStdev-dStdev)*1000;
			}
		}
		else
		{
			nY1=nCy[nPos]-MURA_RECT*3.5;
			nY2=nY1+MURA_RECT;
			dStdev=StdevDataForMura(pFM, nX1, nY1, nX2, nY2, width);
			dStdevDiff=(dMaxStdev-dStdev)*1000;
		}

		//옆에것과 비교해서 차이가 작으면 그 작은값이 Value임
		if((dMaxStdev-dMaxStdevSide)*1000 < dStdevDiff)
		{
			dStdevDiff=(dMaxStdev-dMaxStdevSide)*1000;
			if(dStdevDiff<0) dStdevDiff=0;
		}
		if(dStdevDiff<0) dStdevDiff=0;

		if(dStdevDiff>g_Temp.m_dMuraMaxValue)
		{
			g_Temp.m_dMuraMaxValue=dStdevDiff;
			g_Temp.m_nMuraMaxPosX=nCx[nPos]*g_System.m_nImageW/2048;
			g_Temp.m_nMuraMaxPosY=nCy[nPos]*g_System.m_nImageH/1024;
		}

		nLevel=GetLevel((int)dStdevDiff, g_Param.m_nSTDThDn);

		if(nLevel>0)
		{
			nX=nCx[nPos]*nDiv; //8192는 x4  4096은 x2
			nY=nCy[nPos]*nDiv;
		
			nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_MURA;
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			MakeDefectRect(g_Defect.m_nDefectCount, nX, nY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nX;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nY;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =(int)dStdevDiff; //Value	
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0; //Size
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0; //Size X
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0; //Size Y

			
//			UpdateNGImage(pImage, pData);
//불량영상저장(1/4 압축영상)------------------------------------------------------------
			fmDest	    = g_Defect.m_pImage[g_Defect.m_nDefectCount];
			nPitchDest	= BAD_IMG_HEIGHT;
	
			//Y범위
			tt_=nCy[nPos]-BAD_IMG_HEIGHT/2;
			if(l_fmMuraPrev==0)
			{
				if(tt_<0) tt_=0;
			}
			else
			{
				if(tt_<-50) tt_=-50;
			}

			bb_=tt_+BAD_IMG_HEIGHT;
			if(bb_>g_System.m_nImageH/nDiv)
			{
				bb_=g_System.m_nImageH/nDiv;
				tt_=bb_-BAD_IMG_HEIGHT;
			}
			//X범위
			ll_=nCx[nPos]-BAD_IMG_HEIGHT/2; if(ll_<rect.left/nDiv) ll_=rect.left/nDiv;
			rr_=ll_+BAD_IMG_HEIGHT;
			if(rr_>rect.right/nDiv)
			{
				rr_=rect.right/nDiv;
				ll_=rr_-BAD_IMG_HEIGHT;
				if(ll_<rect.left/nDiv) ll_=rect.left/nDiv;
			}

			for(ii=0;ii<BAD_IMG_HEIGHT;ii++)
			{						
				nYi=tt_+ii;
				if(nYi<0)
				{
					for(jj=0;jj<BAD_IMG_HEIGHT;jj++)
					{
						nXi=ll_+jj;
						*(fmDest+nPitchDest*ii+jj)=*(l_fmMuraPrevArray+width*(50+nYi)+nXi);	
					}
				}
				else
				{
					for(jj=0;jj<BAD_IMG_HEIGHT;jj++)
					{
						nXi=ll_+jj;
						*(fmDest+nPitchDest*ii+jj)=*(pFM+width*nYi+nXi);	
					}
				}
			}
			if(CheckDefectOverlap()==0) 
				g_Defect.m_nDefectCount++;

			if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) break;
		}
		//--------------------------------------------------------------------------------------		
	}

	//최근 검사한 압축이미지의 50Line을 저장한다.------------------------
	memcpy(l_fmMuraPrevArray, pFM+width*(height-50), width*50*sizeof(int));
	l_fmMuraPrev=1;
	//-------------------------------------------------------------------
}
void WEB_MuraSTD_White()
{
	CRect rect;
	int MURA_RECT=40;
	unsigned char* fm = g_fmFlat[g_ID]; //평활화 이미지	
	int i,j, ii,jj,k,nTmp;
	int nHighCut=g_System.m_nFlatBright+10;
	int nLowCut =g_System.m_nFlatBright-2;
	int nLevelInspect=0, nClass;
	int width, height;
	int nX, nY;
	int nX1, nX2, nY1, nY2;
	int ll_, tt_,rr_,bb_;
	double dBlackThresStd=100000, dWhiteThresStd=100000, dStdev, dMaxStdev, dMaxStdevSide, dStdevDiff;
	int nCx[4], nCy[4], nMax[4], nPos, nLevel;
	int nCxMax, nCyMax;
	int *pFM;
	int nPitchDest, nXi, nYi;
	int nAVG=14;  //8192면 64x64,  4096이면 32x32 에서 후보 구함.
	int nDiv;
	int pitch=g_System.m_nPitch;
	LPBYTE fmDest;

	rect.left	=g_Temp.m_nInspectX1;
	rect.top	=0;
	rect.right	=g_Temp.m_nInspectX2;
	rect.bottom	=g_System.m_nImageH;

	if(g_System.m_nImageW>=MAX_WIDTH && g_System.m_nImageH>=MAX_WIDTH) { nDiv=8; nAVG=7; MURA_RECT=32;}
	else if(g_System.m_nImageW>=MAX_WIDTH)							   { nDiv=4; nAVG=14;MURA_RECT=40;}
	else															   { nDiv=2; nAVG=14;MURA_RECT=40;}

	if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) return;
	if(rect.right-rect.left<128*nDiv)				   return;
	if(g_Temp.m_nGrabFrameID<10) l_fmMuraPrev=0;	//Lot바뀌고 초기는 자신의 이미지 만으로 처리한다.

	g_Temp.m_dMuraMaxValue=0;

	//검사 Check-------------------------------------------------------------------------------------------------------------------------
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])	
		{			
			if(g_Param.m_nSTDThUp[i]>0 && g_Param.m_nSTDThUp[i]<dBlackThresStd) { dBlackThresStd=g_Param.m_nSTDThUp[i];nLevelInspect=1;}					
		}
	}
	if(nLevelInspect==0) return;
	//------------------------------------------------------------------------------------------------------------------------------------

	if(g_System.m_nImageW>=MAX_WIDTH && g_System.m_nImageH>=MAX_WIDTH) 
	{
		width=g_System.m_nImageW/nDiv;
		height=g_System.m_nImageH/nDiv;

		nY=0;
		for(i=0;i<g_System.m_nImageH-7;i+=nDiv)
		{
			nX=0;
			for(j=0;j<g_System.m_nImageW-7;j+=nDiv)
			{
				nTmp=(*(fm+pitch*i+j)+*(fm+pitch*i+(j+4))+*(fm+pitch*(i+4)+j)+*(fm+pitch*(i+4)+(j+4)) + 2)/4;
				if(nTmp>nHighCut) nTmp=nHighCut;
				else if(nTmp<nLowCut) nTmp=nLowCut;
				*(l_fmMuraINT+width*nY+nX)=nTmp;
				nX++;
			}
			nY++;
		}
		COSS_Sum(l_fmMuraINT, l_fmT, 0, 0, width, height,width, nAVG, nAVG);
		pFM=l_fmMuraINT;
	}
	else if(g_System.m_nImageW>=MAX_WIDTH) 
	{
		width=g_System.m_nImageW/nDiv;
		height=g_System.m_nImageH/nDiv;

		nY=0;
		for(i=0;i<g_System.m_nImageH-3;i+=nDiv)
		{
			nX=0;
			for(j=0;j<g_System.m_nImageW-3;j+=nDiv)
			{
				nTmp=(*(fm+pitch*i+j)+*(fm+pitch*i+(j+2))+*(fm+pitch*(i+2)+j)+*(fm+pitch*(i+2)+(j+2)) + 2)/4;
				if(nTmp>nHighCut) nTmp=nHighCut;
				else if(nTmp<nLowCut) nTmp=nLowCut;
				*(l_fmMuraINT+width*nY+nX)=nTmp;
				nX++;
			}
			nY++;
		}
		COSS_Sum(l_fmMuraINT, l_fmT, 0, 0, width, height,width, nAVG, nAVG);
		pFM=l_fmMuraINT;
	}
	else// if(g_System.m_nImageW==4096) 
	{
		width=g_System.m_nImageW/nDiv;
		height=g_System.m_nImageH/nDiv;

		nY=0;
		for(i=0;i<g_System.m_nImageH-1;i+=nDiv)
		{
			nX=0;
			for(j=0;j<g_System.m_nImageW-1;j+=nDiv)
			{
				nTmp=(*(fm+pitch*i+j)+*(fm+pitch*i+(j+1))+*(fm+pitch*(i+1)+j)+*(fm+pitch*(i+1)+(j+1)) + 2)/4;
				if(nTmp>nHighCut) nTmp=nHighCut;
				else if(nTmp<nLowCut) nTmp=nLowCut;
				*(l_fmMuraINT+width*nY+nX)=nTmp;
				nX++;
			}
			nY++;
		}
		COSS_Sum(l_fmMuraINT, l_fmT, 0, 0, width, height,width, nAVG, nAVG);	
		pFM=l_fmMuraINT;
	}

	//압축영상 보여주기-------------------------------
//	for(i=0;i<g_System.m_nImageH/4;i++)
//		for(j=0;j<g_System.m_nImageW/4;j++)
//			*(fm+pitch*i+j)=*(l_fmT+width*i+j);
	//------------------------------------------------
		
	//4분할 해서 한개씩 후보 구함.
	for(k=0;k<4;k++)
	{
		if(k==0)		{nX1=nAVG/2;	nX2=width/2;		nY1=nAVG/2;		nY2=height/2; }
		else if(k==1)   {nX1=nAVG/2;	nX2=width/2;		nY1=height/2;	nY2=height-nAVG/2;}
		else if(k==2)	{nX1=width/2;	nX2=width-nAVG/2; 	nY1=nAVG/2;		nY2=height/2;}
		else if(k==3)   {nX1=width/2;	nX2=width-nAVG/2; 	nY1=height/2;	nY2=height-nAVG/2;}
		
		nMax[k]=0;

		if(nX2<rect.left/nDiv+MURA_RECT/2)   continue;
		if(nX1>rect.right/nDiv-MURA_RECT/2)  continue;

		if(nX1<rect.left /nDiv+MURA_RECT/2)  nX1=rect.left /nDiv+MURA_RECT/2;
		if(nX2>rect.right/nDiv-MURA_RECT/2)  nX2=rect.right/nDiv-MURA_RECT/2;

		if((nX2-nX1)<MURA_RECT*nDiv) continue;
		
		for(i=nY1;i<nY2;i++)
			for(j=nX1;j<nX2;j++)
			{
				if(*(l_fmT+width*i+j)>nMax[k])
				{
					nMax[k]=*(l_fmT+width*i+j);
					nCx[k]=j;
					nCy[k]=i;
				}
			}
	}

	for(k=0;k<4;k++)
	{
		//제일 큰 값을 찾는다---------------------------
		nTmp=0;
		nPos=-1;
		for(i=0;i<4;i++)
			if(nMax[i]>nTmp)
			{
				nTmp=nMax[i];
				nPos=i;
			}
		if(nPos<0) 		break;	//255보다 작은게 없으면 끝	
		//------------------------------------------------

		nMax[nPos]=0;
		if(nCx[nPos]<rect.left/nDiv+MURA_RECT/2 || nCx[nPos]>rect.right/nDiv-MURA_RECT/2) continue;		

		nY1=nCy[nPos]-MURA_RECT/2-30; 
		if(l_fmMuraPrev==0)
		{
			if(nY1<0)			 nY1=0;
		}
		nY2=nY1+MURA_RECT;	      if(nY2+60>height)  {nY1=height-60-MURA_RECT; nY2=nY1+MURA_RECT;}
		dMaxStdev=0;

		for(i=0;i<7;i++) //7바꾸면 안됨. 위와 연결되어 있음.
		{
			nX1=nCx[nPos]-MURA_RECT; if(nX1<rect.left/nDiv) nX1=rect.left/nDiv;
			nX2=nX1+MURA_RECT;       		

			for(j=0;j<5;j++)
			{
				if(nX2<=rect.right/nDiv)
				{
					dStdev=StdevDataForMura(pFM, nX1, nY1, nX2, nY2, width);
					if(dStdev>dMaxStdev)
					{
						dMaxStdev=dStdev;
						nCxMax=(nX1+nX2)/2;
						nCyMax=(nY1+nY2)/2;
					}
				}
				nX1+=10;  //바꾸면 안됨
				nX2+=10;
			}
			nY1+=10;	 //바꾸면 안됨
			nY2+=10;
		}
		if(dMaxStdev<=0.1) continue;

		nCx[nPos]=nCxMax;

		//옆에 한번더-----------------------------------------------------------
		nX1=nCx[nPos]-MURA_RECT/2; if(nX1<rect.left/nDiv)   nX1=rect.left/nDiv;
		nX2=nX1+MURA_RECT;         if(nX2>rect.right/nDiv) {nX2=rect.right/nDiv; nX1=nX2-MURA_RECT;}	
		nY1=nCy[nPos]-MURA_RECT/2-30; 
		if(l_fmMuraPrev==0)
		{
			if(nY1<0)			 nY1=0;
		}
		nY2=nY1+MURA_RECT;	      if(nY2+60>height)  {nY1=height-60-MURA_RECT; nY2=nY1+MURA_RECT;}
		dMaxStdevSide=0;
		for(i=0;i<7;i++)
		{
			if(nX1-MURA_RECT*2>=rect.left/nDiv)
			{
				dStdev=StdevDataForMura(pFM, nX1-MURA_RECT*2, nY1, nX2-MURA_RECT*2, nY2, width);
				if(dStdev>dMaxStdevSide) dMaxStdevSide=dStdev;
			}
			if(nX2+MURA_RECT*2<=rect.right/nDiv)
			{
				dStdev=StdevDataForMura(pFM, nX1+MURA_RECT*2, nY1, nX2+MURA_RECT*2, nY2, width);
				if(dStdev>dMaxStdevSide) dMaxStdevSide=dStdev;
			}
			nY1+=10;
			nY2+=10;			
		}
		//-----------------------------------------------------------------------
		nCy[nPos]=nCyMax;

		dStdevDiff=0;
		nY1=nCy[nPos]-MURA_RECT*2.5;
		if(nY1>=0)
		{
			nY2=nY1+MURA_RECT;
			dStdev=StdevDataForMura(pFM, nX1, nY1, nX2, nY2, width);
			dStdevDiff=(dMaxStdev-dStdev)*1000;
		}
		else
		{
			nY1=nCy[nPos]+MURA_RECT*2.5;
			nY2=nY1+MURA_RECT;
			dStdev=StdevDataForMura(pFM, nX1, nY1, nX2, nY2, width);
			if((dMaxStdev-dStdev)*1000 > dStdevDiff)
			{
				dStdevDiff=(dMaxStdev-dStdev)*1000;
			}
		}
		
		nY1=nCy[nPos]+MURA_RECT*1.5;
		nY2=nY1+MURA_RECT;
		if(nY2<=height)
		{
			dStdev=StdevDataForMura(pFM, nX1, nY1, nX2, nY2, width);
			if((dMaxStdev-dStdev)*1000 > dStdevDiff)
			{
				dStdevDiff=(dMaxStdev-dStdev)*1000;
			}
		}
		else
		{
			nY1=nCy[nPos]-MURA_RECT*3.5;
			nY2=nY1+MURA_RECT;
			dStdev=StdevDataForMura(pFM, nX1, nY1, nX2, nY2, width);
			dStdevDiff=(dMaxStdev-dStdev)*1000;
		}

		//옆에것과 비교해서 차이가 작으면 그 작은값이 Value임
		if((dMaxStdev-dMaxStdevSide)*1000 < dStdevDiff)
		{
			dStdevDiff=(dMaxStdev-dMaxStdevSide)*1000;
			if(dStdevDiff<0) dStdevDiff=0;
		}
		if(dStdevDiff<0) dStdevDiff=0;

		if(dStdevDiff>g_Temp.m_dMuraMaxValue)
		{
			g_Temp.m_dMuraMaxValue=dStdevDiff;
			g_Temp.m_nMuraMaxPosX=nCx[nPos]*g_System.m_nImageW/2048;
			g_Temp.m_nMuraMaxPosY=nCy[nPos]*g_System.m_nImageH/1024;
		}

		nLevel=GetLevel((int)dStdevDiff, g_Param.m_nSTDThUp);

		if(nLevel>0)
		{
			nX=nCx[nPos]*nDiv; //8192는 x4  4096은 x2
			nY=nCy[nPos]*nDiv;
		
			nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_MURA;
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			MakeDefectRect(g_Defect.m_nDefectCount, nX, nY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nX;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nY;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =(int)dStdevDiff; //Value	
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0; //Size
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0; //Size X
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0; //Size Y

			
//			UpdateNGImage(pImage, pData);
//불량영상저장(1/4 압축영상)------------------------------------------------------------
			fmDest	    = g_Defect.m_pImage[g_Defect.m_nDefectCount];
			nPitchDest	= BAD_IMG_HEIGHT;
	
			//Y범위
			tt_=nCy[nPos]-BAD_IMG_HEIGHT/2;
			if(l_fmMuraPrev==0)
			{
				if(tt_<0) tt_=0;
			}
			else
			{
				if(tt_<-50) tt_=-50;
			}

			bb_=tt_+BAD_IMG_HEIGHT;
			if(bb_>g_System.m_nImageH/nDiv)
			{
				bb_=g_System.m_nImageH/nDiv;
				tt_=bb_-BAD_IMG_HEIGHT;
			}
			//X범위
			ll_=nCx[nPos]-BAD_IMG_HEIGHT/2; if(ll_<rect.left/nDiv) ll_=rect.left/nDiv;
			rr_=ll_+BAD_IMG_HEIGHT;
			if(rr_>rect.right/nDiv)
			{
				rr_=rect.right/nDiv;
				ll_=rr_-BAD_IMG_HEIGHT;
				if(ll_<rect.left/nDiv) ll_=rect.left/nDiv;
			}

			for(ii=0;ii<BAD_IMG_HEIGHT;ii++)
			{						
				nYi=tt_+ii;
				if(nYi<0)
				{
					for(jj=0;jj<BAD_IMG_HEIGHT;jj++)
					{
						nXi=ll_+jj;
						*(fmDest+nPitchDest*ii+jj)=*(l_fmMuraPrevArray+width*(50+nYi)+nXi);	
					}
				}
				else
				{
					for(jj=0;jj<BAD_IMG_HEIGHT;jj++)
					{
						nXi=ll_+jj;
						*(fmDest+nPitchDest*ii+jj)=*(pFM+width*nYi+nXi);	
					}
				}
			}
			if(CheckDefectOverlap()==0) 
				g_Defect.m_nDefectCount++;

			if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) break;
		}
		//--------------------------------------------------------------------------------------		
	}

	//최근 검사한 압축이미지의 50Line을 저장한다.------------------------
	memcpy(l_fmMuraPrevArray, pFM+width*(height-50), width*50*sizeof(int));
	l_fmMuraPrev=1;
	//-------------------------------------------------------------------
}

#if INSPECT_TYPE==TAC_LINE
	#define MURA_HIGH_CUT 20
	#define MURA_LOW_CUT  15
#else
	#define MURA_HIGH_CUT 12
	#define MURA_LOW_CUT  12
#endif

void WEB_MuraTAC()
{
	CRect rect;
#ifdef MURA_TAC_STYPE
	int MURA_RECT=40;
	int nCount;
#else
	int MURA_RECT=64;
#endif
	unsigned char* fm = g_fmFlat[g_ID]; //평활화 이미지	
	int i,j, ii,jj,k,nTmp;
	int nLevelInspect=0, nClass, nType=0;
	int width, height;
	int nX, nY;
	int nX1, nX2, nY1, nY2;
	int ll_, tt_,rr_,bb_;
	double dStdev, dMaxStdev, dStdevDiff;
	double dStdevH[2], dStdevW[2];
	int nCx, nCy, nMin[4], nPos, nLevel;
	int nCxMax, nCyMax;
	int *pFM;
	int nPitchDest, nXi, nYi;
	int nAVG=14;  //8192면 64x64,  4096이면 32x32 에서 후보 구함.
	int nDiv;
	int pitch=g_System.m_nPitch;
	int nValueUp, nValueDn;
	double dSize[3], dImulKipoSize;
	LPBYTE fmDest;

	rect.left	=g_Temp.m_nInspectX1;
	rect.top	=0;
	rect.right	=g_Temp.m_nInspectX2;
	rect.bottom	=g_System.m_nImageH;

	if(g_Temp.m_nGrabFrameID<10) l_fmMuraPrev=0;	//Lot바뀌고 초기는 자신의 이미지 만으로 처리한다.

	g_Temp.m_dMuraMaxValue=0;
	g_Temp.m_dMuraMaxValueB=0;

	//검사 Check-------------------------------------------------------------------------------------------------------------------------
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])	
		{			
			if(g_Param.m_nSTDThUp[i]>0  || g_Param.m_nSTDThDn[i]>0) { nLevelInspect=1;}			
		}
	}
	if(nLevelInspect==0) return;
	//------------------------------------------------------------------------------------------------------------------------------------


#ifdef MURA_TAC_STYPE
//	Erode_Fast(fm, g_fmTemp, g_System.m_nImageW, g_System.m_nImageH, g_System.m_nPitch); //시간여유가 많으면 사용가능
	for(i=0;i<30;i++)
	{
		if(g_CandiArea.m_nCandiAvg[i]>0)
		{
			if(g_CandiArea.m_nCandiValue[i]<3) continue;
			MURA_RECT=40;
		}
		else  
		{
			if(g_CandiArea.m_nCandiValue[i]<2) continue;
			MURA_RECT=30;
		}

		nX=g_CandiArea.m_nCandiX[i];
		nY=g_CandiArea.m_nCandiY[i];

		dMaxStdev=SearchMaxStd_Cut(fm, nX, nY, pitch, MURA_RECT, g_System.m_nFlatBright, MURA_HIGH_CUT, MURA_LOW_CUT, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, g_System.m_nImageH, MURA_RECT*2, &nCx, &nCy);	
		if(dMaxStdev<1) continue;

		dStdevH[0]=dStdevH[1]=dStdevW[0]=dStdevW[1]=0;
		//상
		if(nCy-MURA_RECT/2-MURA_RECT>0)
			dStdevH[0]=FindStd_Cut(fm, nCx-MURA_RECT/2, nCy-MURA_RECT/2-MURA_RECT, nCx+MURA_RECT/2, nCy+MURA_RECT/2-MURA_RECT, pitch, g_System.m_nFlatBright+MURA_HIGH_CUT, g_System.m_nFlatBright-MURA_HIGH_CUT);
		else
			dStdevH[0]=0;
		//하
		if(nCy+MURA_RECT/2+MURA_RECT<g_System.m_nImageH)
			dStdevH[1]=FindStd_Cut(fm, nCx-MURA_RECT/2, nCy-MURA_RECT/2+MURA_RECT, nCx+MURA_RECT/2, nCy+MURA_RECT/2+MURA_RECT, pitch, g_System.m_nFlatBright+MURA_HIGH_CUT, g_System.m_nFlatBright-MURA_HIGH_CUT);
		else
			dStdevH[1]=0;

		//좌
		if(nCx-MURA_RECT/2-MURA_RECT>g_Temp.m_nInspectX1)
			dStdevW[0]=FindStd_Cut(fm, nCx-MURA_RECT/2-MURA_RECT, nCy-MURA_RECT/2, nCx+MURA_RECT/2-MURA_RECT, nCy+MURA_RECT/2, pitch, g_System.m_nFlatBright+MURA_HIGH_CUT, g_System.m_nFlatBright-MURA_HIGH_CUT);
		else
			dStdevW[0]=0;
		//우
		if(nCx+MURA_RECT/2+MURA_RECT<g_Temp.m_nInspectX2)
			dStdevW[1]=FindStd_Cut(fm, nCx-MURA_RECT/2+MURA_RECT, nCy-MURA_RECT/2, nCx+MURA_RECT/2+MURA_RECT, nCy+MURA_RECT/2, pitch, g_System.m_nFlatBright+MURA_HIGH_CUT, g_System.m_nFlatBright-MURA_HIGH_CUT);
		else
			dStdevW[1]=0;

		//TAC은 4개 평균을 나눔-----------------------------------------
		dStdevDiff=nCount=0;
		if(dStdevH[0]>0) { dStdevDiff+=dStdevH[0]; nCount++;}
		if(dStdevH[1]>0) { dStdevDiff+=dStdevH[1]; nCount++;}
		if(dStdevW[0]>0) { dStdevDiff+=dStdevW[0]; nCount++;}
		if(dStdevW[1]>0) { dStdevDiff+=dStdevW[1]; nCount++;}

		if(nCount>0)	dStdevDiff=100*(dMaxStdev-dStdevDiff/nCount);
		else			dStdevDiff=0;
#else

	for(i=0;i<30;i++)
	{
		if(g_CandiArea.m_nCandiValue[i]<3) break;

		nX=g_CandiArea.m_nCandiX[i];
		nY=g_CandiArea.m_nCandiY[i];

		dMaxStdev=SearchMaxStd_Cut(fm, nX, nY, pitch, MURA_RECT, g_System.m_nFlatBright, MURA_HIGH_CUT, MURA_LOW_CUT, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, g_System.m_nImageH, MURA_RECT*2, &nCx, &nCy);	
		if(dMaxStdev<1) continue;

		dStdevH[0]=dStdevH[1]=dStdevW[0]=dStdevW[1]=0;
		//상
		if(nCy-MURA_RECT/2-MURA_RECT>0)
			dStdevH[0]=FindStd_Cut(fm, nCx-MURA_RECT/2, nCy-MURA_RECT/2-MURA_RECT, nCx+MURA_RECT/2, nCy+MURA_RECT/2-MURA_RECT, pitch, g_System.m_nFlatBright+MURA_HIGH_CUT, g_System.m_nFlatBright-MURA_HIGH_CUT);
		else
			dStdevH[0]=0;
		//하
		if(nCy+MURA_RECT/2+MURA_RECT<g_System.m_nImageH)
			dStdevH[1]=FindStd_Cut(fm, nCx-MURA_RECT/2, nCy-MURA_RECT/2+MURA_RECT, nCx+MURA_RECT/2, nCy+MURA_RECT/2+MURA_RECT, pitch, g_System.m_nFlatBright+MURA_HIGH_CUT, g_System.m_nFlatBright-MURA_HIGH_CUT);
		else
			dStdevH[1]=0;

		//좌
		if(nCx-MURA_RECT/2-MURA_RECT>g_Temp.m_nInspectX1)
			dStdevW[0]=FindStd_Cut(fm, nCx-MURA_RECT/2-MURA_RECT, nCy-MURA_RECT/2, nCx+MURA_RECT/2-MURA_RECT, nCy+MURA_RECT/2, pitch, g_System.m_nFlatBright+MURA_HIGH_CUT, g_System.m_nFlatBright-MURA_HIGH_CUT);
		else
			dStdevW[0]=0;
		//우
		if(nCx+MURA_RECT/2+MURA_RECT<g_Temp.m_nInspectX2)
			dStdevW[1]=FindStd_Cut(fm, nCx-MURA_RECT/2+MURA_RECT, nCy-MURA_RECT/2, nCx+MURA_RECT/2+MURA_RECT, nCy+MURA_RECT/2, pitch, g_System.m_nFlatBright+MURA_HIGH_CUT, g_System.m_nFlatBright-MURA_HIGH_CUT);
		else
			dStdevW[1]=0;

		//상 하 좌 우 4개보다 커야함. 그중 제일 작은 값을 선택함.
		dStdevDiff=100;
		if(dMaxStdev-dStdevH[0]<dStdevDiff) dStdevDiff=dMaxStdev-dStdevH[0];
		if(dMaxStdev-dStdevH[1]<dStdevDiff) dStdevDiff=dMaxStdev-dStdevH[1];
		if(dMaxStdev-dStdevW[0]<dStdevDiff) dStdevDiff=dMaxStdev-dStdevW[0];
		if(dMaxStdev-dStdevW[1]<dStdevDiff) dStdevDiff=dMaxStdev-dStdevW[1];
		dStdevDiff*=100;
#endif
		

		if(g_CandiArea.m_nCandiAvg[i]>=0)
		{
			nLevel=GetLevel((int)dStdevDiff, g_Param.m_nSTDThUp);
			if(dStdevDiff>g_Temp.m_dMuraMaxValue)
			{
				g_Temp.m_dMuraMaxValue=dStdevDiff;
				g_Temp.m_nMuraMaxPosX=nCx;
				g_Temp.m_nMuraMaxPosY=nCy;
			}
		}
		else
		{
			nLevel=GetLevel((int)dStdevDiff, g_Param.m_nSTDThDn);
			if(dStdevDiff>g_Temp.m_dMuraMaxValueB)
			{
				g_Temp.m_dMuraMaxValueB=dStdevDiff;
				g_Temp.m_nMuraMaxPosXB=nCx;
				g_Temp.m_nMuraMaxPosYB=nCy;
			}
		}

		//일단 Size구하기 전에 Value로 먼저 Level을 본다.
		if(nLevel>0)
		{
			if(g_CandiArea.m_nCandiAvg[i]>=0) GetDefectInfoTAC(fm, 0, nX, nY, pitch, dSize);
			else							  GetDefectInfoTAC(fm, 1, nX, nY, pitch, dSize);			

			//Value Size를 가지고 다시 한번 Level을 구한다.------------------------------------------------------------------------
			if(g_CandiArea.m_nCandiAvg[i]>=0) nLevel=GetLevel((int)dStdevDiff, dSize[2], g_Param.m_nSTDThUp, g_Param.m_dSTDSizeUp);
			else							  nLevel=GetLevel((int)dStdevDiff, dSize[2], g_Param.m_nSTDThDn, g_Param.m_dSTDSizeDn);
			if(nLevel<1) continue;
			//---------------------------------------------------------------------------------------------------------------------


			if(g_CandiArea.m_nCandiAvg[i]>=0)
			{
				nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_TAC_MURA;
				nType=0;	//WHITE
			}
			else	
			{
				nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_TAC_MURA_B;
				nType=1;   //Black
			}
			g_DefectTemp.m_nFrameNum=g_Temp.m_nGrabFrameID;
			MakeDefectTempRect(nX, nY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);

			g_DefectTemp.m_Defect.x_pos=nX;
			g_DefectTemp.m_Defect.y_pos=nY;
			g_DefectTemp.m_Defect.defect_class=nClass;
			g_DefectTemp.m_Defect.value		  =(int)dStdevDiff; //Value				
			g_DefectTemp.m_Defect.sizeX       =dSize[0]; //Size X
			g_DefectTemp.m_Defect.sizeY       =dSize[1]; //Size Y
			g_DefectTemp.m_Defect.size        =dSize[2]; //Size

//불량영상저장------------------------------------------------------------
			//1:1로 복사하려면 아래와 같이 사용.
			CopyNGImageTemp(fm, g_DefectTemp.m_pImage, g_DefectTemp.m_rcDefect.left,g_DefectTemp.m_rcDefect.top, g_DefectTemp.m_rcDefect.right, g_DefectTemp.m_rcDefect.bottom ,pitch);

/*   //압축하려면 아래 프로그램 수정해서 사용
			fmDest	    = g_DefectTemp.m_pImage;
			nPitchDest	= BAD_IMG_HEIGHT;
	
			//Y범위
			tt_=nCy-BAD_IMG_HEIGHT/2;			
			if(tt_<0) tt_=0;
			bb_=tt_+BAD_IMG_HEIGHT;
			if(bb_>g_System.m_nImageH)
			{
				bb_=g_System.m_nImageH;
				tt_=bb_-BAD_IMG_HEIGHT;
			}
			//X범위
			ll_=nCx-BAD_IMG_HEIGHT/2; if(ll_<rect.left) ll_=rect.left;
			rr_=ll_+BAD_IMG_HEIGHT;
			if(rr_>rect.right)
			{
				rr_=rect.right;
				ll_=rr_-BAD_IMG_HEIGHT;
				if(ll_<rect.left) ll_=rect.left;
			}

			for(ii=0;ii<BAD_IMG_HEIGHT;ii++)
			{						
				nYi=tt_+ii;
				
				for(jj=0;jj<BAD_IMG_HEIGHT;jj++)
				{
					nXi=ll_+jj;
					*(fmDest+nPitchDest*ii+jj)=*(fm+pitch*nYi+nXi);					
				}
			}
*/
//------------------------------------------------------------------------------------

			//불량 정리해 주는 함수(방금 구한 것을 버릴지, 겹치는 이전 것을 버릴지, 안겹치면 불량수 증가, 이미 곽찾으면 알아서 정리)
			CheckDefectOverlapAdjust();			
		}
	}
}



double StdevDataForMura(int * fm, int x1, int y1, int x2, int y2, int pitch)
{
	int i,j;
	long nSum=0;
	int tmp;
	long nArea;
	double dSum2=0, stdev=0; 

	nArea=(x2-x1)*(y2-y1);
	if(y1<0 && y2<0)
	{
		for(i=50+y1;i<50+y2;i++)
			for(j=x1;j<x2;j++)
			{
				tmp=*(l_fmMuraPrevArray+pitch*i+j);
				nSum+=tmp;
				dSum2+=(tmp*tmp);
			}
	}
	else if(y1<0 && y2>=0)
	{
		for(i=50+y1;i<50;i++)
			for(j=x1;j<x2;j++)
			{
				tmp=*(l_fmMuraPrevArray+pitch*i+j);
				nSum+=tmp;
				dSum2+=(tmp*tmp);
			}

		for(i=0;i<y2;i++)
			for(j=x1;j<x2;j++)
			{
				tmp=*(fm+pitch*i+j);
				nSum+=tmp;
				dSum2+=(tmp*tmp);
			}
	}
	else
	{
		for(i=y1;i<y2;i++)
			for(j=x1;j<x2;j++)
			{
				tmp=*(fm+pitch*i+j);
				nSum+=tmp;
				dSum2+=(tmp*tmp);
			}
	}

    stdev=sqrt((nArea*dSum2-(double)nSum*(double)nSum)/((double)nArea*((double)nArea-1.)));	
	return stdev;
}

void WEB_MuraTACLine()
{
	CRect rect;
	unsigned char* fm = g_fmGrab[g_ID]; //평활화전 이미지
	int i,j, ii,jj,k,nTmp;
	int nOff=20;
	int nLevelInspect=0, nClass;
	int width, height;
	int nX, nY;
	int nX1, nX2, nMaxLineValue, nLineValue, nPos;
	int ll_, tt_,rr_,bb_;
	int  nLevel;
	int *pFM;
	int nPitchDest, nXi, nYi;
	int nYCount;
	int pitch=g_System.m_nPitch;
	long nLineProfile[2048];
	bool nLineProfileUSE[2048];
	LPBYTE fmDest;

	rect.left	=4*(g_Temp.m_nInspectX1/4);
	rect.top	=0;
	rect.right	=4*(g_Temp.m_nInspectX2/4);
	rect.bottom	=g_System.m_nImageH;

	if(g_Temp.m_nGrabFrameID<10) l_fmMuraPrev=0;	//Lot바뀌고 초기는 자신의 이미지 만으로 처리한다.

	g_Temp.m_nMaxLRValue=0;

	//검사 Check-------------------------------------------------------------------------------------------------------------------------
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])	
		{			
			if(g_Param.m_nValueLineKipo[i]>0 ) { nLevelInspect=1;}			
		}
	}
	if(nLevelInspect==0) return;
	//------------------------------------------------------------------------------------------------------------------------------------

	nYCount=0;
	for(i=g_System.m_nImageH/4;i<g_System.m_nImageH*3/4;i+=2) nYCount++;


	memset(nLineProfile, 0, 2048*sizeof(long));
	memset(nLineProfileUSE, 1, 2048*sizeof(bool));
	for(i=g_System.m_nImageH/4;i<g_System.m_nImageH*3/4;i+=2)
		for(j=rect.left;j<rect.right;j++)
		{
			nLineProfile[j/4]+=*(fm+pitch*i+j);
		}
	nX1=rect.left/4;
	nX2=rect.right/4;
	for(j=nX1;j<nX2;j++)
		nLineProfile[j]=nLineProfile[j]*100/(4*nYCount);

	for(i=0;i<2;i++)
	{
		nMaxLineValue=0;
		for(j=nX1+nOff;j<nX2-nOff;j++)
		{
			if(nLineProfileUSE[j]==0) continue;

			nLineValue=nLineProfile[j]-(nLineProfile[j-nOff]+nLineProfile[j+nOff])/2;
			if(nLineValue>nMaxLineValue)
			{
				nMaxLineValue=nLineValue;
				nPos=j;
			}
		}
		if(nMaxLineValue>g_Temp.m_nMaxLRValue)
		{
			g_Temp.m_nMaxLRValue=nMaxLineValue;
			g_Temp.m_nMaxLRPosX =nPos*4;
		}
		nLevel=GetLevel(nMaxLineValue, g_Param.m_nValueLineKipo);
		if(nLevel>0)
		{
			for(j=nPos-nOff;j<nPos+nOff;j++)
				nLineProfileUSE[j]=0;

			nX=nPos*4; 
			nY=g_System.m_nImageH/2;
		
			nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_TAC_LINE;
			g_DefectTemp.m_nFrameNum=g_Temp.m_nGrabFrameID;
			MakeDefectTempRect(nX, nY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			g_DefectTemp.m_Defect.x_pos=nX;
			g_DefectTemp.m_Defect.y_pos=nY;
			g_DefectTemp.m_Defect.defect_class=nClass;
			g_DefectTemp.m_Defect.value		  =nMaxLineValue; //Value	
			g_DefectTemp.m_Defect.size        =0; //Size
			g_DefectTemp.m_Defect.sizeX       =0; //Size X
			g_DefectTemp.m_Defect.sizeY       =0; //Size Y

//불량영상저장(1/4 압축영상)------------------------------------------------------------
			fmDest	    = g_DefectTemp.m_pImage;
			nPitchDest	= BAD_IMG_HEIGHT;
	
			//Y범위
			tt_=nY-BAD_IMG_HEIGHT;			
			if(tt_<0) tt_=0;
			bb_=tt_+BAD_IMG_HEIGHT*2;
			if(bb_>g_System.m_nImageH)
			{
				bb_=g_System.m_nImageH;
				tt_=bb_-BAD_IMG_HEIGHT*2;
			}
			//X범위
			ll_=nX-BAD_IMG_HEIGHT*1.5; if(ll_<rect.left) ll_=rect.left;
			rr_=ll_+BAD_IMG_HEIGHT*3;
			if(rr_>rect.right)
			{
				rr_=rect.right;
				ll_=rr_-BAD_IMG_HEIGHT*3;
				if(ll_<rect.left) ll_=rect.left;
			}

			for(ii=0;ii<BAD_IMG_HEIGHT;ii++)
			{						
				nYi=tt_+ii*2;
				
				for(jj=0;jj<BAD_IMG_HEIGHT;jj++)
				{
					nXi=ll_+(rr_-ll_)*jj/BAD_IMG_HEIGHT;
					*(fmDest+nPitchDest*ii+jj)=*(fm+pitch*nYi+nXi);					
				}
			}

			//겹치는 것 알아서 정리해주는 함수
			CheckDefectOverlapAdjust();			
		}

	}
}

void WEB_MuraTACLine_COS6()
{
	CRect rect;
	unsigned char* fm = g_fmGrab[g_ID]; //평활화전 이미지
	int i,j, ii,jj,k,nTmp;
	int nOff=7;
	int nLevelInspect=0, nClass;
	int width, height;
	int nX, nY;
	int nX1, nX2, nMaxLineValue, nLineValue, nPos;
	int ll_, tt_,rr_,bb_;
	int  nLevel;
	int *pFM;
	int nPitchDest, nXi, nYi;
	int nYCount;
	int pitch=g_System.m_nPitch;
	long nLineProfile[2048];		//8192까지 커버됨.
	bool nLineProfileUSE[2048];		//8192까지 커버됨.
	LPBYTE fmDest;

	rect.left	=4*(g_Temp.m_nInspectX1/4);
	rect.top	=0;
	rect.right	=4*(g_Temp.m_nInspectX2/4);
	rect.bottom	=g_System.m_nImageH;

	if(g_Temp.m_nGrabFrameID<10) l_fmMuraPrev=0;	//Lot바뀌고 초기는 자신의 이미지 만으로 처리한다.

	g_Temp.m_nMaxLRValue=0;

	//검사 Check-------------------------------------------------------------------------------------------------------------------------
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])	
		{			
			if(g_Param.m_nValueLineKipo[i]>0 ) { nLevelInspect=1;}			
		}
	}
	if(nLevelInspect==0) return;
	//------------------------------------------------------------------------------------------------------------------------------------

	nYCount=0;
	for(i=g_System.m_nImageH/4;i<g_System.m_nImageH*3/4;i+=2) nYCount++;


	memset(nLineProfile, 0, 2048*sizeof(long));
	memset(nLineProfileUSE, 1, 2048*sizeof(bool));
	for(i=g_System.m_nImageH/4;i<g_System.m_nImageH*3/4;i+=2)
		for(j=rect.left;j<rect.right;j++)
		{
			nLineProfile[j/4]+=*(fm+pitch*i+j);
		}
	nX1=rect.left/4;
	nX2=rect.right/4;
	for(j=nX1;j<nX2;j++)
		nLineProfile[j]=nLineProfile[j]*100/(nYCount*4);

	for(i=0;i<2;i++)
	{
		nMaxLineValue=0;
		for(j=nX1+nOff;j<nX2-nOff;j++)
		{
			if(nLineProfileUSE[j]==0) continue;

			nLineValue=nLineProfile[j]-(nLineProfile[j-nOff]+nLineProfile[j+nOff])/2;
			if(nLineValue>nMaxLineValue)
			{
				nMaxLineValue=nLineValue;
				nPos=j;
			}
		}
		if(nMaxLineValue>g_Temp.m_nMaxLRValue)
		{
			g_Temp.m_nMaxLRValue=nMaxLineValue;
			g_Temp.m_nMaxLRPosX =nPos*4;
		}
		nLevel=GetLevel(nMaxLineValue, g_Param.m_nValueLineKipo);
		if(nLevel>0)
		{
			for(j=nPos-nOff;j<nPos+nOff;j++)
				nLineProfileUSE[j]=0;

			nX=nPos*4; 
			nY=g_System.m_nImageH/2;
		
			nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+5;		//라인
			g_DefectTemp.m_nFrameNum=g_Temp.m_nGrabFrameID;
			MakeDefectTempRect(nX, nY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			g_DefectTemp.m_Defect.x_pos=nX;
			g_DefectTemp.m_Defect.y_pos=nY;
			g_DefectTemp.m_Defect.defect_class=nClass;
			g_DefectTemp.m_Defect.value		  =nMaxLineValue; //Value	
			g_DefectTemp.m_Defect.size        =1; //Size
			g_DefectTemp.m_Defect.sizeX       =1; //Size X
			g_DefectTemp.m_Defect.sizeY       =1; //Size Y

//불량영상저장(1/4 압축영상)------------------------------------------------------------
			fmDest	    = g_DefectTemp.m_pImage;
			nPitchDest	= BAD_IMG_HEIGHT;
	
			//Y범위
			tt_=nY-BAD_IMG_HEIGHT;			
			if(tt_<0) tt_=0;
			bb_=tt_+BAD_IMG_HEIGHT*2;
			if(bb_>g_System.m_nImageH)
			{
				bb_=g_System.m_nImageH;
				tt_=bb_-BAD_IMG_HEIGHT*2;
			}
			//X범위
			ll_=nX-BAD_IMG_HEIGHT*1.5; if(ll_<rect.left) ll_=rect.left;
			rr_=ll_+BAD_IMG_HEIGHT*3;
			if(rr_>rect.right)
			{
				rr_=rect.right;
				ll_=rr_-BAD_IMG_HEIGHT*3;
				if(ll_<rect.left) ll_=rect.left;
			}

			for(ii=0;ii<BAD_IMG_HEIGHT;ii++)
			{						
				nYi=tt_+ii*2;
				
				for(jj=0;jj<BAD_IMG_HEIGHT;jj++)
				{
					nXi=ll_+(rr_-ll_)*jj/BAD_IMG_HEIGHT;
					*(fmDest+nPitchDest*ii+jj)=*(fm+pitch*nYi+nXi);					
				}
			}

			//겹치는 것 알아서 정리해주는 함수
			CheckDefectOverlapAdjust();			
		}

	}
}


#define TD_LINE_MULTI	4	//Line값을 4배 증폭
#define TD_UP_LIMIT		5
#define TD_DN_LIMIT		5
#define TD_LINE_DATA	5
#define TD_LINE_OFFSET	15
double l_LineTDValue[32][4096];	//TD라인 검출에 사용함.  4096을 128씩 나눔=32
long    l_LineTD_UpLimit[4096];
long    l_LineTD_DnLimit[4096];

void WEB_MuraTACLine_TD()
{
	int i,j, k, N;
	unsigned char* fm = g_fmFlat[g_ID]; //평활화 이미지
	int nLevelInapect=0;
	long nSumAll=0, nSum, nTmpT, nTmpB, nTmpDiff, nTmpDiffMax=0;
	int nPosX, nPosY, nClass;
	int ii,jj,ll,tt,rr,bb, nYY, nYi, nXi;
	int nX1, nX2, nPitchDest;
	int pitch=g_System.m_nPitch;
	int width=g_System.m_nImageW;
	int height=g_System.m_nImageH;
	CRect rect;			

	int nLevelInspect=0;
	int nLevel;
	int nProfile[4096], nCount;
	LPBYTE fmT, fmDest;
	

	nX1=g_Temp.m_nInspectX1;
	nX2=g_Temp.m_nInspectX2;

	g_Temp.m_nMaxTDValue=0;  //m_nMaxTDPosX, m_nMaxTDPosY
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])	
		{			
			if(g_Param.m_dLineThresholdTD[i]>0 ) { nLevelInspect=1;}			
		}
	}

	if(nLevelInspect==0) return;

	if(nX2-nX1<256) return;

	nCount=0;
	for(j=nX1;j<nX2;j+=10) nCount++;
	if(nCount==0) return;

	//수평줄의 Gray Value Limit건다.
	for(i=0;i<height;i++)
	{
		nSum=0;
		for(j=nX1;j<nX2;j+=10)
			nSum+=*(fm+pitch*i+j);

		l_LineTD_UpLimit[i]=nSum/nCount+TD_UP_LIMIT;
		l_LineTD_DnLimit[i]=nSum/nCount-TD_DN_LIMIT;
	}

	

	N=0;
	for(k=0;k<4096;k+=128, N++)
	{
		for(i=0;i<height;i++)
		{	
			nSum=0;
			for(j=k;j<k+128;j++)
			{
				if(*(fm+pitch*i+j)>l_LineTD_UpLimit[i])			nSum+=l_LineTD_UpLimit[i];
				else if(*(fm+pitch*i+j)<l_LineTD_DnLimit[i])	nSum+=l_LineTD_DnLimit[i];
				else											nSum+=*(fm+pitch*i+j);
			}
			l_LineTDValue[N][i]=nSum*TD_LINE_MULTI*100/128;
		}
	}

	for(N=0;N<32-8;N++)
	{
		if(N*128<nX1) continue;
		else if((N+8)*128>nX2) continue;

		for(i=0;i<height;i++)
		{
			nProfile[i]=0;
			for(j=N;j<N+8;j++)
				nProfile[i]+=l_LineTDValue[j][i];

			nProfile[i]/=8;
		}

		for(i=0;i<height-2*TD_LINE_DATA-TD_LINE_OFFSET;i++)
		{
			if(nProfile[i]==0) continue;

			nTmpT=0;
			for(j=i;j<i+TD_LINE_DATA;j++)
				nTmpT+=nProfile[j];

			nTmpB=0;
			for(j=i+TD_LINE_DATA+TD_LINE_OFFSET;j<i+TD_LINE_DATA*2+TD_LINE_OFFSET;j++)
				nTmpB+=nProfile[j];

			if(nTmpT>0 && nTmpB>0)
			{
				nTmpT/=TD_LINE_DATA;
				nTmpB/=TD_LINE_DATA;
				nTmpDiff=nTmpT-nTmpB; if(nTmpDiff<0) nTmpDiff*=-1;

				if(nTmpDiff>nTmpDiffMax)
				{
					nTmpDiffMax=nTmpDiff;
					nPosX=(N+4)*128;
					nPosY=i+TD_LINE_DATA+TD_LINE_OFFSET/2;	
					if(nPosY<64) nPosY=64;
					else if(nPosY>height-64) nPosY=height-64;
				}
			}
		}
	}
	g_Temp.m_nMaxTDValue=nTmpDiffMax; 
	g_Temp.m_nMaxTDPosX=nPosX;
	g_Temp.m_nMaxTDPosY=nPosY;

	nLevel=GetLevel(nTmpDiffMax, g_Param.m_dLineThresholdTD);     //절대비교

	if(nLevel>0 && g_Defect.m_nDefectCount<g_System.m_nMaxDefect )
	{					
		nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+5;		//라인
		g_DefectTemp.m_nFrameNum=g_Temp.m_nGrabFrameID;
		MakeDefectTempRect(nPosX, nPosY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
		g_DefectTemp.m_Defect.x_pos=nPosX;
		g_DefectTemp.m_Defect.y_pos=nPosY;
		g_DefectTemp.m_Defect.defect_class=nClass;
		g_DefectTemp.m_Defect.value		  =nTmpDiffMax; //Value	
		g_DefectTemp.m_Defect.size        =2; //Size
		g_DefectTemp.m_Defect.sizeX       =2; //Size X
		g_DefectTemp.m_Defect.sizeY       =2; //Size Y

		//이미지--------------------------------------------------------
		fmDest	    = g_DefectTemp.m_pImage;
		nPitchDest	= BAD_IMG_HEIGHT;

		//Y범위
		tt=nPosY-256; if(tt<0) tt=0;
		bb=tt+512;   
		if(bb>height)
		{
			bb=height;
			tt=bb-512;
		}
		//X범위
		ll=nPosX-512; if(ll<rect.left) ll=rect.left;
		rr=ll+1024;      
		if(rr>nX2)
		{
			rr=nX2;
			ll=rr-1024;
			if(ll<nX1) ll=nX1;
		}

		for(ii=0;ii<BAD_IMG_HEIGHT;ii++)
		{
			nYi=tt+(bb-tt)*ii/BAD_IMG_HEIGHT;

			for(jj=0;jj<BAD_IMG_HEIGHT;jj++)
			{
				nXi=ll+(jj)*(rr-ll)/BAD_IMG_HEIGHT;
				*(fmDest+nPitchDest*ii+jj)=*(fm+pitch*nYi+nXi);
			}
		}
		//----------------------------------------------------------------
		CheckDefectOverlapAdjust();		
//		if(CheckDefectOverlap()==0) 
//				g_Defect.m_nDefectCount++;
	}

	return;
}



//(SizeX, SIzeY)크기의 Sum Map을 구함.
int COSS_Sum(int * fmS, int * fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY)
{
	int ok;
	int i, j;
	int jj;
	int nSum, nAvg;
	int nHalfX, nHalfY;
	int nTmp;

	if(nSizeX%2==0) nSizeX++;
	if(nSizeY%2==0) nSizeY++;

	nHalfX=nSizeX/2;
	nHalfY=nSizeY/2;


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
			nTmp=nSum;
			*(fmD+pitch*i+j)=nTmp;

			nSum=nSum+l_SumVer[i-nHalfY+nSizeY]-l_SumVer[i-nHalfY];
		}
	}

	return 0;
}

/*
BYTE fm2[MAX_WIDTH/2*MAX_WIDTH/2];
BYTE fm3[MAX_WIDTH/2*MAX_WIDTH/2];
int Erode_Fast(LPBYTE fmSour, LPBYTE fmDest, int nWidth, int nHeight, int nPitch)
{
	int i, j, nHPitch=nPitch/2;
	int nHWidth=nWidth/2;
	int nHHeight=nHeight/2;
	int nTmp;
	LPBYTE fmT;

	for(i=0;i<nHHeight;i++)
		for(j=0;j<nHWidth;j++)
		{
			fmT=fmSour+nPitch*i*2+j*2;
			*(fm2+nHPitch*i+j)=(*(fmT)+*(fmT+1)+*(fmT+nPitch)+*(fmT+nPitch+1))/4;
		}

	for(i=0;i<nHHeight-1;i++)
		for(j=0;j<nHWidth-1;j++)
		{
			fmT=fm2+nHPitch*i+j;
			nTmp=*(fmT);
			if(*(fmT+1) <nTmp)				nTmp=*(fmT+1);
			else if(*(fmT+nHPitch) <nTmp)	nTmp=*(fmT+nHPitch);
			else if(*(fmT+nHPitch+1) <nTmp) nTmp=*(fmT+nHPitch+1);
			*(fm3+nHPitch*i+j)=nTmp;
		}
	for(i=0;i<nHHeight;i++)
		*(fm3+nHPitch*i+nHWidth-1)=*(fm3+nHPitch*i+nHWidth-2);
	for(j=0;j<nHWidth;j++)
		*(fm3+nHPitch*(nHHeight-1)+j)=*(fm3+nHPitch*(nHHeight-2)+j);
	

	for(i=0;i<nHHeight;i++)
		for(j=0;j<nHWidth;j++)
		{
			fmT=fmDest+nPitch*i*2+j*2;
			*(fmT)=*(fmT+1)=*(fmT+nPitch)=*(fmT+nPitch+1)=*(fm3+nHPitch*i+j);
		}
	
	return 0;
}
*/

int l_Projection[256*2];
int l_ProjectionSmooth[256*2];
double l_GrayDiffH, l_GrayDiffW;
void GetDefectSizeTAC_ForWhite(int N, LPBYTE fm, int nX, int nY, int pitch, int nMaxHeight, double *dWidth, double *dHeight, double *dGrayDiff);
void GetDefectSizeTAC_ForBlack(int N, LPBYTE fm, int nX, int nY, int pitch, int nMaxHeight, double *dWidth, double *dHeight, double *dGrayDiff);

int GetDefectInfoTAC(LPBYTE fm, int nIsBlack, int nX, int nY, int pitch,  double *pSize)
{
	int nReturn=0, nOffset;
	double dWidth, dHeight, dGrayDiff;
	double dSizeX, dSizeY;

	if(nX<0 || nX>=pitch || nY<0 || nY>=g_System.m_nImageH) return -1;

	if(nIsBlack==0)
	{
		GetDefectSizeTAC_ForWhite(15, fm, nX, nY, pitch, g_System.m_nImageH, &dWidth, &dHeight, &dGrayDiff);
		pSize[0] =dWidth* g_Param.m_dScaleFactorX;
		pSize[1] =dHeight* g_Param.m_dScaleFactorY;
		pSize[2]  =(pSize[0]+pSize[1])/2;

		if(pSize[0]>pSize[1]) pSize[1]=pSize[2]=pSize[0];
		else				  pSize[0]=pSize[2]=pSize[1];

//		nOffset=(int)(dGrayDiff*10);
//		if(nOffset>99) nOffset=99;		
//		pSize[2]=(((int)(pSize[2]*10))*100+nOffset)/1000.0;

		if(dGrayDiff<3) pSize[0]=pSize[1]=pSize[2]=0;
	}
	else 
	{
		GetDefectSizeTAC_ForBlack(7, fm, nX, nY, pitch, g_System.m_nImageH, &dWidth, &dHeight, &dGrayDiff);
		pSize[0] =dWidth* g_Param.m_dScaleFactorX;
		pSize[1] =dHeight* g_Param.m_dScaleFactorY;
		pSize[2]  =(pSize[0]+pSize[1])/2;
		//dGrayDiff-=g_PyramidTemp.m_nBlackOffset;
		//if(dGrayDiff<0) dGrayDiff=0;

		//흑으로 못잡으면 백으로 한번더 한다. 왜냐하면 미코팅 같은게 가끔은 후보는 흑인데, 백으로 보이는 경우가 있기 때문에 
		if(dGrayDiff<3)
		{
			GetDefectSizeTAC_ForWhite(15, fm,nX, nY, pitch, g_System.m_nImageH, &dWidth, &dHeight, &dGrayDiff);
			
			pSize[0] =dWidth* g_Param.m_dScaleFactorX;
			pSize[1] =dHeight* g_Param.m_dScaleFactorY;
			pSize[2]  =(pSize[0]+pSize[1])/2;
		}

		if(pSize[0]>pSize[1]) pSize[1]=pSize[2]=pSize[0];
		else				  pSize[0]=pSize[2]=pSize[1];

//		nOffset=(int)(dGrayDiff*10);
//		if(nOffset>99) nOffset=99;		
//		pSize[2]=(((int)(pSize[2]*10))*100+nOffset)/1000.0;

		if(dGrayDiff<3) pSize[0]=pSize[1]=pSize[2]=0;
	}
	return nReturn;
}



void GetDefectSizeTAC_ForWhite(int N, LPBYTE fm, int nX, int nY, int pitch, int nMaxHeight, double *dWidth, double *dHeight, double *dGrayDiff)
{
	int i, j, left, top, right, bottom;
	int leftT, rightT, bottomT, topT;
	int nWidth, nHeight;
	int nTmp, nSum, nSumAvg, nCount, nPos, nSumX, nSumY;
	double dAvg, dMax, dThreshold;
	int  M_l, M_t, M_r, M_b;
	LPBYTE fmT;
	int nTop;

	//정확한 중심위치를 다시 구한다 --------------------------
	left=nX-25;  if(left<0) left=0;
 	top=nY-25;   if(top<0)  top=0;
	right=nX+25; if(right>=pitch) right=pitch-1;
	bottom=nY+25;if(bottom>=nMaxHeight) bottom=nMaxHeight-1;
	nSumX=nSumY=nSum=0;

	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
		{
			nTmp=*(fm+pitch*i+j)-130;
			if(nTmp>0)
			{
				nSumX+=nTmp*j;
				nSumY+=nTmp*i;
				nSum+=nTmp;
			}
		}
	if(nSum>0)
	{
		nX=nSumX/nSum;
		nY=nSumY/nSum;
	}
	//---------------------------------------------------------

	left=nX-100;  if(left<0) left=0;
 	top=nY-100;   if(top<0)  top=0;
	right=nX+100; if(right>=pitch) right=pitch-1;
	bottom=nY+100;if(bottom>=nMaxHeight) bottom=nMaxHeight-1;

	leftT=nX-7; if(leftT<0) leftT=0;
	rightT=nX+7; if(rightT>=pitch) rightT=pitch-1;
	nWidth=rightT-leftT+1;  if(nWidth<=0) nWidth=1;
	//수직성분 --------------------------------
	if(g_Temp.m_nPCOptic==COS2)
	{
		M_l=leftT-1;	if(M_l<0) M_l=0;
		M_t=top-1;		if(M_t<0) M_t=0;
		M_r=rightT+1;	if(M_r>=pitch) M_r=pitch-1;
		M_b=bottom+1;	if(M_b>=nMaxHeight) M_b=nMaxHeight-1;
		Erode_Gray(fm, g_fmTemp, M_l, M_t, M_r, M_b, pitch, NULL, 3,3);
		fmT=g_fmTemp;
	}
	else
		fmT=fm;

	nHeight=0;
	
	for(i=top;i<bottom;i++)
	{
		nSum=0;
		for(j=leftT;j<=rightT;j++)
			nSum+=*(fmT+pitch*i+j);
		l_Projection[nHeight++]=nSum/nWidth;
	}	

	if(nHeight<4 || nHeight>=512) return;

	for(i=2;i<nHeight-2;i++)
		l_ProjectionSmooth[i]=(l_Projection[i-2]+l_Projection[i-1]+l_Projection[i]+l_Projection[i+1]+l_Projection[i+2])*0.2;
	l_ProjectionSmooth[0]=(l_Projection[0]+l_Projection[1]+l_Projection[2])/3;
	l_ProjectionSmooth[1]=(l_Projection[1]+l_Projection[2]+l_Projection[3])/3;
	l_ProjectionSmooth[nHeight-1]=(l_Projection[nHeight-1]+l_Projection[nHeight-2]+l_Projection[nHeight-3])/3;
	l_ProjectionSmooth[nHeight-2]=(l_Projection[nHeight-2]+l_Projection[nHeight-3]+l_Projection[nHeight-4])/3;

	//양쪽 가장자리 1/4씩 뽑아서 평균값 구하기 ---------------------
	nSumAvg=nCount=0;
	if(top!=0) //불량이 TOP에 치우쳐 있으면 TOP은 사용하지 않는다.
	{
		for(i=0;i<nHeight/4;i++)
		{
			nSumAvg+=l_ProjectionSmooth[i];
			nCount++;
		}
	}
	if(bottom!=(nMaxHeight-1)) //불량이 Bottom에 치우쳐 있으면 Bottom은 사용하지 않는다.
	{
		for(i=nHeight*3/4;i<nHeight;i++)
		{
			nSumAvg+=l_ProjectionSmooth[i];
			nCount++;
		}
	}
	if(nCount) dAvg=nSumAvg/nCount;
	else	   dAvg=0;
	//-----------------------------------

	//최대값 및 그 위치 구하기 (알고리즘 맞음)----------
	nTmp=0;
	for(i=0;i<N;i++)
		nTmp+=l_ProjectionSmooth[i];
	dMax=nTmp;

	nPos=N/2;
	for(i=N;i<nHeight;i++)
	{
		nTmp+=(l_ProjectionSmooth[i]-l_ProjectionSmooth[i-N]);
		if(nTmp>dMax)
		{
			dMax=nTmp;
			nPos=i-N/2;
		}
	}
	dMax/=N;
	dThreshold=(dAvg+dMax)*0.5;
	l_GrayDiffH=dMax-dAvg;
	//----------------------------------------------------------------------------
	//아래-위 위치를 찾는다.
	topT=0;
	bottomT=nHeight-1;
	for(i=nPos+1;i<nHeight;i++)
		if(l_ProjectionSmooth[i]<dThreshold)
		{
			bottomT=i;
			break;
		}
	for(i=nPos-1;i>=0;i--)
		if(l_ProjectionSmooth[i]<dThreshold)
		{
			topT=i;
			break;
		}
	*dHeight=(bottomT-topT+1);
	//-----------------------------------------------------------------------------
	


	topT=nY-7; if(topT<0) topT=0;
	bottomT=nY+7; if(bottomT>=nMaxHeight) bottomT=nMaxHeight-1;
	nHeight=bottomT-topT+1; if(nHeight<=0) nHeight=1;
	//수평성분 --------------------------------
	if(g_Temp.m_nPCOptic==COS2)
	{
		M_l=left-1;	  if(M_l<0) M_l=0;
		M_t=topT-1;	  if(M_t<0) M_t=0;
		M_r=right+1;  if(M_r>=pitch) M_r=pitch-1;
		M_b=bottomT+1;if(M_b>=nMaxHeight) M_b=nMaxHeight-1;
		Erode_Gray(fm, g_fmTemp, M_l, M_t, M_r, M_b, pitch, NULL, 3,3);
	}
	
	nWidth=0;
	for(j=left;j<right;j++)
	{
		nSum=0;
		for(i=topT;i<=bottomT;i++)
			nSum+=*(fm+pitch*i+j);
		l_Projection[nWidth++]=nSum/nHeight;
	}
	

	if(nWidth<4 || nWidth>=512) return;

	for(i=2;i<nWidth-2;i++)
		l_ProjectionSmooth[i]=(l_Projection[i-2]+l_Projection[i-1]+l_Projection[i]+l_Projection[i+1]+l_Projection[i+2])*0.2;
	l_ProjectionSmooth[0]=(l_Projection[0]+l_Projection[1]+l_Projection[2])/3;
	l_ProjectionSmooth[1]=(l_Projection[1]+l_Projection[2]+l_Projection[3])/3;
	l_ProjectionSmooth[nWidth-1]=(l_Projection[nWidth-1]+l_Projection[nWidth-2]+l_Projection[nWidth-3])/3;
	l_ProjectionSmooth[nWidth-2]=(l_Projection[nWidth-2]+l_Projection[nWidth-3]+l_Projection[nWidth-4])/3;

	//평균값 구하기 ---------------------
	nSumAvg=nCount=0;
	if(left!=0)
	{
		for(i=0;i<nWidth/4;i++)
		{
			nSumAvg+=l_ProjectionSmooth[i];
			nCount++;
		}
	}
	if(right!=(pitch-1))
	{
		for(i=nWidth*3/4;i<nWidth;i++)
		{
			nSumAvg+=l_ProjectionSmooth[i];
			nCount++;
		}
	}
	if(nCount) dAvg=nSumAvg/nCount;
	else       dAvg=0;
	//-----------------------------------

	//최대값 및 그 위치 구하기 ----------
	nTmp=0;
	for(i=0;i<N;i++)
		nTmp+=l_ProjectionSmooth[i];
	dMax=nTmp;

	nPos=N/2;
	for(i=N;i<nWidth;i++)
	{
		nTmp+=(l_ProjectionSmooth[i]-l_ProjectionSmooth[i-N]);
		if(nTmp>dMax)
		{
			dMax=nTmp;
			nPos=i-N/2;
		}
	}
	dMax/=N;
	dThreshold=(dAvg+dMax)*0.5;
	l_GrayDiffW=dMax-dAvg;
	//----------------------------------------------------------------------------
	//아래-위 위치를 찾는다.
	leftT=0;
	rightT=nWidth-1;
	for(i=nPos+1;i<nWidth;i++)
		if(l_ProjectionSmooth[i]<dThreshold)
		{
			rightT=i;
			break;
		}
	for(i=nPos-1;i>=0;i--)
		if(l_ProjectionSmooth[i]<dThreshold)
		{
			leftT=i;
			break;
		}
	*dWidth=(rightT-leftT+1);
	//-----------------------------------------------------------------------------
	
	*dGrayDiff=	(l_GrayDiffH+l_GrayDiffW)*0.5;
}

//2009.12.9일 얼룩의 크기 구하는 방법 변경 
void GetDefectSizeTAC_ForBlack(int N, LPBYTE fm, int nX, int nY, int pitch, int nMaxHeight, double *dWidth, double *dHeight, double *dGrayDiff)
{
	int i, j, left, top, right, bottom;
	int leftT, rightT, bottomT, topT;
	int nWidth, nHeight;
	int nTmp, nSum, nSumAvg, nCount, nPos, nSumX, nSumY;
	double dAvg, dMax, dThreshold;
	int nBase=g_System.m_nFlatBright;

	//정확한 중심위치를 다시 구한다 --------------------------
	left=nX-25;  if(left<0) left=0;
 	top=nY-25;   if(top<0)  top=0;
	right=nX+25; if(right>=pitch) right=pitch-1;
	bottom=nY+25;if(bottom>=nMaxHeight) bottom=nMaxHeight-1;
	nSumX=nSumY=nSum=0;
	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
		{
			nTmp=(nBase-2)-*(fm+pitch*i+j);
			if(nTmp>0)
			{
				nSumX+=nTmp*j;
				nSumY+=nTmp*i;
				nSum+=nTmp;
			}
		}
	if(nSum>0)
	{
		nX=nSumX/nSum;
		nY=nSumY/nSum;
	}
	//---------------------------------------------------------

	left=nX-100;  if(left<0) left=0;
 	top=nY-100;   if(top<0)  top=0;
	right=nX+100; if(right>=pitch) right=pitch-1;
	bottom=nY+100;if(bottom>=nMaxHeight) bottom=nMaxHeight-1;

	leftT=nX-7; if(leftT<0) leftT=0;
	rightT=nX+7; if(rightT>=pitch) rightT=pitch-1;
	nWidth=rightT-leftT+1; if(nWidth<=0) nWidth=1;
	//수직성분 --------------------------------
	nHeight=0;
	for(i=top;i<bottom;i++)
	{
		nSum=0;
		for(j=leftT;j<=rightT;j++)
			nSum+=*(fm+pitch*i+j);
		l_Projection[nHeight++]=nSum/nWidth;
	}

	if(nHeight<4 || nHeight>=512) return;

	for(i=2;i<nHeight-2;i++)
		l_ProjectionSmooth[i]=(l_Projection[i-2]+l_Projection[i-1]+l_Projection[i]+l_Projection[i+1]+l_Projection[i+2])*0.2;
	l_ProjectionSmooth[0]=(l_Projection[0]+l_Projection[1]+l_Projection[2])/3;
	l_ProjectionSmooth[1]=(l_Projection[1]+l_Projection[2]+l_Projection[3])/3;
	l_ProjectionSmooth[nHeight-1]=(l_Projection[nHeight-1]+l_Projection[nHeight-2]+l_Projection[nHeight-3])/3;
	l_ProjectionSmooth[nHeight-2]=(l_Projection[nHeight-2]+l_Projection[nHeight-3]+l_Projection[nHeight-4])/3;

	//양쪽 가장자리 1/4씩 뽑아서 평균값 구하기 ---------------------
	nSumAvg=nCount=0;
	if(top!=0) //불량이 TOP에 치우쳐 있으면 TOP은 사용하지 않는다.
	{
		for(i=0;i<nHeight/4;i++)
		{
			nSumAvg+=l_ProjectionSmooth[i];
			nCount++;
		}
	}
	if(bottom!=(nMaxHeight-1)) //불량이 Bottom에 치우쳐 있으면 Bottom은 사용하지 않는다.
	{
		for(i=nHeight*3/4;i<nHeight;i++)
		{
			nSumAvg+=l_ProjectionSmooth[i];
			nCount++;
		}
	}
	if(nCount) dAvg=nSumAvg/nCount;
	else       dAvg=0;
	//-----------------------------------

	//최대값 및 그 위치 구하기 ----------
	nTmp=0;
	for(i=0;i<N;i++)
		nTmp+=l_ProjectionSmooth[i];
	dMax=nTmp;

	nPos=N/2;
	for(i=N;i<nHeight;i++)
	{
		nTmp+=(l_ProjectionSmooth[i]-l_ProjectionSmooth[i-N]);
		if(nTmp<dMax)
		{
			dMax=nTmp;
			nPos=i-N/2;
		}
	}
	dMax/=N;
	dThreshold=(dAvg+dMax)*0.5;
	l_GrayDiffH=dAvg-dMax;
	//----------------------------------------------------------------------------
	//아래-위 위치를 찾는다.
	topT=0;
	bottomT=nHeight-1;
	for(i=nPos+1;i<nHeight;i++)
		if(l_ProjectionSmooth[i]>dThreshold)
		{
			bottomT=i;
			break;
		}
	for(i=nPos-1;i>=0;i--)
		if(l_ProjectionSmooth[i]>dThreshold)
		{
			topT=i;
			break;
		}
	*dHeight=(bottomT-topT+1);
	//-----------------------------------------------------------------------------
	


	topT=nY-7; if(topT<0) topT=0;
	bottomT=nY+7; if(bottomT>=nMaxHeight) bottomT=nMaxHeight-1;
	nHeight=bottomT-topT+1; if(nHeight<=0) nHeight=1;
	//수평성분 --------------------------------
	nWidth=0;
	for(j=left;j<right;j++)
	{
		nSum=0;
		for(i=topT;i<=bottomT;i++)
			nSum+=*(fm+pitch*i+j);
		l_Projection[nWidth++]=nSum/nHeight;
	}

	if(nWidth<4 || nWidth>=512) return;

	for(i=2;i<nWidth-2;i++)
		l_ProjectionSmooth[i]=(l_Projection[i-2]+l_Projection[i-1]+l_Projection[i]+l_Projection[i+1]+l_Projection[i+2])*0.2;
	l_ProjectionSmooth[0]=(l_Projection[0]+l_Projection[1]+l_Projection[2])/3;
	l_ProjectionSmooth[1]=(l_Projection[1]+l_Projection[2]+l_Projection[3])/3;
	l_ProjectionSmooth[nWidth-1]=(l_Projection[nWidth-1]+l_Projection[nWidth-2]+l_Projection[nWidth-3])/3;
	l_ProjectionSmooth[nWidth-2]=(l_Projection[nWidth-2]+l_Projection[nWidth-3]+l_Projection[nWidth-4])/3;

	//평균값 구하기 ---------------------
	nSumAvg=nCount=0;
	if(left!=0)
	{
		for(i=0;i<nWidth/4;i++)
		{
			nSumAvg+=l_ProjectionSmooth[i];
			nCount++;
		}
	}
	if(right!=(pitch-1))
	{
		for(i=nWidth*3/4;i<nWidth;i++)
		{
			nSumAvg+=l_ProjectionSmooth[i];
			nCount++;
		}
	}
	if(nCount) dAvg=nSumAvg/nCount;
	else       dAvg=0;
	//-----------------------------------

	//최대값 및 그 위치 구하기 ----------
	nTmp=0;
	for(i=0;i<N;i++)
		nTmp+=l_ProjectionSmooth[i];
	dMax=nTmp;

	nPos=N/2;
	for(i=N;i<nWidth;i++)
	{
		nTmp+=(l_ProjectionSmooth[i]-l_ProjectionSmooth[i-N]);
		if(nTmp<dMax)
		{
			dMax=nTmp;
			nPos=i-N/2;
		}
	}
	dMax/=N;
	dThreshold=(dAvg+dMax)*0.5;
	l_GrayDiffW=dAvg-dMax;
	//----------------------------------------------------------------------------
	//아래-위 위치를 찾는다.
	leftT=0;
	rightT=nWidth-1;
	for(i=nPos+1;i<nWidth;i++)
		if(l_ProjectionSmooth[i]>dThreshold)
		{
			rightT=i;
			break;
		}
	for(i=nPos-1;i>=0;i--)
		if(l_ProjectionSmooth[i]>dThreshold)
		{
			leftT=i;
			break;
		}
	*dWidth=(rightT-leftT+1);
	//-----------------------------------------------------------------------------
	*dGrayDiff=	(l_GrayDiffH+l_GrayDiffW)*0.5;
}

int FindSEPALine(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int *nPosY, int *Tape);
int CheckTACJumpping()
{
	int i, ii,jj, nX, nY, ret=0;
	int nJumpping=0;
	int nPosY[5], nTape[5];
	int left=g_Temp.m_nInspectX1;
	int right=g_Temp.m_nInspectX2;
	LPBYTE fmDest;
	int nPitchDest;
	int ll_,rr_,tt_,bb_;
	int nXi, nYi;

	ret=FindSEPALine(g_fmGrab[g_ID], g_Temp.m_nInspectX1, 0, g_Temp.m_nInspectX2, g_System.m_nImageH, g_System.m_nPitch, nPosY, nTape);

	if(ret>2)
	{
		for(i=0;i<5;i++)
		{
			nX=left+(right-left)*i/5+(right-left)/10;
			nY=nPosY[i];
		
			if(nTape[i]==1) g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class = MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+8;	//SEPA LINE불량 (TAPE같이 평균밝기 대비 20보다 작으면 연결)
			else            g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class = MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+7;	//JUMPPING 

			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			MakeDefectRect(g_Defect.m_nDefectCount, nX, nY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nX;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nY;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =999+(1-nTape[i]); //연결 999, 점핑 1000
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =5; //Size
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =5; //Size X
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =5; //Size Y
			CopyNGImageFM(g_fmGrab[g_ID], g_System.m_nPitch);
						 
			g_Defect.m_nDefectCount++;
			nJumpping=1;
			if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) break;
		}
	}
	//--------------------------------------------------------------------------------------		

	return nJumpping;
}


//X를 5등분
//Y프로젝션
//nLimitValue=NEL2면 20
// Tape[loop]=0 이면 점핑
int g_nImageProjectionT[MAX_WIDTH];
int FindSEPALine(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int *nPosY, int *Tape)
{
	int i, j, loop;
	int nCount;
	int nXCount, left1, right1;
	int nMax, nMin, nPos=0, nSlopeMax=0, nSlope;
	int nFoundCount=0, nMinProjection;
	if(right>=pitch) right=pitch-1;
	int nAvgSum,nAvgCount;
	int nLimitValue=10;
	int nJump=5; 

	if(g_Temp.m_nPCOptic==COS2) nLimitValue=20;   //각도 30도

	for(i=0;i<5;i++)
	{
		nPosY[i]=(top+bottom)/2;
		Tape[i]=0;
	}

	for(loop=0;loop<5;loop++)
	{
		nPosY[loop]=(top+bottom)/2;
		left1=left+(right-left)*loop/5;
		right1=left+(right-left)*(loop+1)/5;

		nXCount=nCount=0;	
		for(j=left1;j<=right1;j+=5) nXCount++;
		if(nXCount==0) continue;

		nAvgSum=nAvgCount=0;
		for(i=top;i<bottom;i+=nJump)
		{
			g_nImageProjectionT[nCount]=0;
			for(j=left1;j<=right1;j+=5)
				g_nImageProjectionT[nCount]+=*(fm+pitch*i+j);

			nAvgSum+=g_nImageProjectionT[nCount]/nXCount;
			g_nImageProjectionT[nCount++]/=nXCount;
			nAvgCount++;
		}
		nAvgSum/=nAvgCount;

		nMax=nSlopeMax=0;
		nMin=1000;
		nPos=-1;
		for(i=0;i<nCount;i++)
		{
			if(g_nImageProjectionT[i]>nMax) nMax=g_nImageProjectionT[i];
			if(g_nImageProjectionT[i]<nMin) nMin=g_nImageProjectionT[i];
		}

		if((nMax-nMin)>nLimitValue)
		{
			for(i=0;i<nCount-3;i++)
			{
				nSlope=	g_nImageProjectionT[i+3]-g_nImageProjectionT[i];
				if(nSlope<0) nSlope*=-1;
				if(nSlope>nSlopeMax)
				{
					nSlopeMax=nSlope;
					nPos=i*nJump;
				}
			}
		}
		nPosY[loop]=nPos;
		if(nAvgSum>80) nAvgSum=80;
		if(nMin<nAvgSum*0.7) Tape[loop]=1;
		else                 Tape[loop]=0;
		if(nPos>0) nFoundCount++;
	}
	return nFoundCount;
}