#include "Stdafx.h"
#include "math.h"
#include "KoWebDoc.h"
#include "KoWebView.h"


void MakeCunicMap(LPBYTE fm, int x1,int x2, int pitch, int nHeight, int *pStdev, int *nSW, int *nSH);
double GetCandiValue_HighCut(LPBYTE fm, int nPitch, CRect rect); //쿠닉후보값 구하기(평균보다 큰값은 Cut)
double FindCunicStd(LPBYTE fmPrev, LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nLimitX1, int nLimitX2, int nSubHeight, int *nCunicValue);
double FindCunicStd_LineScan(LPBYTE fmPrev, LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nLimitX1, int nLimitX2, int nSubHeight, int *nCunicValue);

//쿠닉과검체크----
int IsOverKillCross( LPBYTE fm, int nUseProfile, double *pProfile,  int nCx, int nCy, int nLimitX1, int nLimitX2, int pitch, int nGrabHeight);
int CheckCunicAgain(LPBYTE fm, int nCx, int nCy,  int X1, int X2, int pitch,double dLimitvalue);
	double SearchStdevData(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch);

int l_nMap[(MAX_WIDTH/12+1)*(640/12+1)];
BYTE l_fmCunic[MAX_WIDTH*640];
long l_cProfile[MAX_WIDTH];
long l_nProfileSum[MAX_WIDTH];
long l_nProfileAvg[MAX_WIDTH];
long l_nProfileSumMoveinAvg[MAX_WIDTH];
//Y 640만 됨.
void WEB_CunicInspect()
{
	CRect rect, rc;
	unsigned char* fm = g_fmFlat[g_ID]; //평활이미지	
	int i, j, k;
	int nMapX, nMapY;
	int nWidth=g_System.m_nImageW;
	int nHeight=g_System.m_nImageH;
	int pitch=g_System.m_nPitch;
	int nLevelInspect=0, nOffset;
	int left, top, right, bottom;
	double dMinStd=100;
	int nMax, ii, jj, nX, nY, ni, nj, nOverKill;
	int    nCandiX[5], nCandiY[5], nCunicValue;
	double dCandiValue[5];
	double dStdev, dBoundaryCunicOffset;
	int nLevelArray[2], nLevel, nFoundCunic;

	g_Temp.m_dCunicCanicMax=0;

	nOffset=g_Param.m_nCunicInspArea/2;
	if(nOffset<12) nOffset=12; 
	
	if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) return;

	//검사 Check-----------------------------------------------------------------------
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])	
		{			
			if(g_Param.m_dCunicStd[i]>0)
			{
				nLevelInspect=1;
				if(g_Param.m_dCunicStd[i]<dMinStd) dMinStd=g_Param.m_dCunicStd[i];
			}
			if(	g_Param.m_dCunicStd1[i]>0 && g_Param.m_nCunicValue1[i]>0)
			{
				nLevelInspect=1;
				if(g_Param.m_dCunicStd1[i]<dMinStd) dMinStd=g_Param.m_dCunicStd1[i];
			}
		}
	}
	if(nLevelInspect==0) return;
	//----------------------------------------------------------------------------------


	rect.left	=g_Temp.m_nInspectX1;
	rect.top	=0;
	rect.right	=g_Temp.m_nInspectX2;
	rect.bottom	=g_System.m_nImageH;
	MakeCunicMap(fm, rect.left,rect.right, pitch, nHeight, l_nMap, &nMapX, &nMapY);


	for(k=0;k<5;k++) //쿠닉후보 제일 큰 것 5개로 검사함.
	{
		nMax=-1;
		for(i=0;i<nMapY;i++)
			for(j=0;j<nMapX;j++)
				if(*(l_nMap+nMapX*i+j)>nMax)
				{
					nMax=*(l_nMap+nMapX*i+j);
					ii=i;
					jj=j;
				}
		if(nMax<0) continue;
		// 가장 큰 후보 근처데이타 Reset (바로 옆에 있는게 또 후보로 걸리지 않도록)-
		for(i=ii-10;i<=ii+10;i++)
		{
			if(i<0 || i>=nMapY) continue;
			for(j=jj-10;j<=jj+10;j++)
			{
				if(j<0 || j>=nMapX) continue;
				*(l_nMap+nMapX*i+j)=0;
			}
		}	

		//후보영역과 후보영역값을 다시 구한다.-----------
		nCandiX[k]=jj*12+12+rect.left;
		
		if(ii<25) //288~312
		{
			nCandiY[k]	=ii*12+12;
			rc.top		=ii*12;
			rc.bottom	=rc.top+24;
		}
		else if(ii==25)
		{
			nCandiY[k]	=nHeight/2-12;
			rc.top		=nHeight/2-24;
			rc.bottom	=rc.top+24;
		}
		else if(ii<51)
		{
			nCandiY[k]	=ii*12+8+12;
			rc.top		=ii*12+8;
			rc.bottom	=rc.top+24;
		}
		else if(ii==51)
		{
			nCandiY[k]	=nHeight-12;
			rc.top		=nHeight-24;
			rc.bottom	=rc.top+24;
		}
		

		rc.left		=jj*12+rect.left;
		rc.right	=rc.left+24; 
		if(rc.right>g_System.m_nImageW) 
		{
			rc.right=g_System.m_nImageW;
			rc.left =rc.right-24;
		}

		dCandiValue[k]=GetCandiValue_HighCut(fm, pitch, rc); //HIGH 값 CUT하고 후보 유사 STDEV 값 계산 
		if(g_Temp.m_dCunicCanicMax<dCandiValue[k]) g_Temp.m_dCunicCanicMax=dCandiValue[k];
	}
	COSS_Mean(fm, l_fmCunic, 0,0, nWidth, nHeight, pitch, 3,3);

	for(k=0;k<5;k++)
	{		
		dStdev=dCandiValue[k];
		nX	=nCandiX[k];
		nY	=nCandiY[k];
	

		if(dStdev<g_Param.m_dCunicCanidValue) continue; 
		if(!IsInspectArea(nX)) continue; //In Position Check

		rect.SetRect(nX,nY,nX,nY);
		rect.InflateRect(50, 50); //쿠닉은 조금 작게 Display한다.

		 dBoundaryCunicOffset=0;
		if(g_Temp.m_nInspectX1>10 || g_Temp.m_nInspectX2<pitch-10) dBoundaryCunicOffset=0.01;
		

		ni = nX;
		nj = nY;
		if(ni<(nOffset+5))			 ni = nOffset+5;
		if(ni>nWidth-nOffset-5)		 ni = nWidth-nOffset-5;
		if(nj<nOffset)				 nj = nOffset;
		if(nj>nHeight-5-nOffset)	 nj = nHeight-5-nOffset;

		left	=ni-nOffset;
		right	=ni+nOffset;
		top		=nj-nOffset;
		bottom	=nj+nOffset;
		if(left  < rect.left)	 { left  = rect.left;  right=left+2*nOffset;}
		if(right > rect.right)	 { right = rect.right; left =left-2*nOffset;}
		if(top < 0)				 { top = 0; bottom=top+2*nOffset ;}
		if(bottom > nHeight-1)	 { bottom = nHeight-1; top=bottom-2*nOffset;}

		//Level을 먼저 뽑고...
		nFoundCunic=0;
		if(g_System.m_nLineCAM==0)
			dStdev=FindCunicStd(fm, l_fmCunic, left, top, right, bottom, pitch,g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, g_System.m_nImagePartH, &nCunicValue);			//Area
		else
			dStdev=FindCunicStd_LineScan(fm, l_fmCunic, left, top, right, bottom, pitch,g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, g_System.m_nImagePartH, &nCunicValue);//Line Scan

		nLevelArray[0]=GetLevel(dStdev, g_Param.m_dCunicStd);
		nLevelArray[1]=GetLevel(nCunicValue, dStdev, g_Param.m_nCunicValue1, g_Param.m_dCunicStd1);
		
		if(nLevelArray[0]>0 || nLevelArray[1]>0)
		{			
			if(nLevelArray[0]>0 && nLevelArray[1]>0)
			{
				if(nLevelArray[0]<nLevelArray[1]) 
				{
					if(nLevelArray[0]>0)
					{
						g_Defect.m_Defect[g_Defect.m_nDefectCount].value = int(dStdev*1000);					
						nFoundCunic=1;
						nLevel=nLevelArray[0];
					}
				}
				else //이물성쿠닉
				{
					if(nLevelArray[1]>0)
					{
						g_Defect.m_Defect[g_Defect.m_nDefectCount].value = int(dStdev*1000)*100+nCunicValue;					
						nFoundCunic=1;
						nLevel=nLevelArray[1];
					}
				}
			}
			else if(nLevelArray[0]>0)
			{
					g_Defect.m_Defect[g_Defect.m_nDefectCount].value = int(dStdev*1000);
					nFoundCunic=1;
					nLevel=nLevelArray[0];
			}
			else if(nLevelArray[1]>0) //이물성쿠닉
			{
					g_Defect.m_Defect[g_Defect.m_nDefectCount].value = int(dStdev*1000)*100+nCunicValue;
					nFoundCunic=1;
					nLevel=nLevelArray[1];
			}
	}
	else 
		continue;



		nOverKill=IsOverKillCross( g_fmGrab[g_ID], 0, NULL, nX,nY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, pitch, g_System.m_nImagePartH);   //과검체크 해서 과검이 아니면 불량 Count증가 
		if(nOverKill==2)  continue;

		if(nOverKill==0)
			nOverKill = CheckCunicAgain(fm, nX,nY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2, pitch, 4.5); //4.5부터 쿠닉임.

		if(nOverKill==1)
		{		
	#ifdef OVERKILL_CUNIC_LEVEL10
			nLevel=10;
	#else
			continue; //버림
	#endif
		}

		if(nFoundCunic)
		{
			MakeDefectRect(g_Defect.m_nDefectCount, nX, nY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			CopyNGImage(true);
			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_CUNIC;
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nX;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nY;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0; //Size
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0; //Size X
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0; //Size Y

			if(CheckDefectOverlap()==0)   							
				g_Defect.m_nDefectCount++;
		}
	}

}

//1392(2048) x 640 (640은 바뀌면 안됨)
void MakeCunicMap(LPBYTE fm, int x1,int x2, int pitch, int nHeight, int *pStdev, int *nSW, int *nSH)
{
	int i,j,k,ii, jj;
	double stdev=0; 
	int nArea, tmp, nSum, nSum_, nSum2, nSum2_;
	int nX, nY, nSPitch=0, nMapCount=0;
	int nW=24, nH=24;
	int nHalfHeight=nHeight/2;

	*nSW=*nSH=0;
	if(nHeight!=640) return;

	for(j=x1;j<=x2-nW;j+=nW/2) nSPitch++;

	nArea=nW*nH;
	
	nY=0;
	for(k=0;k<52;k++, nY++)
	{	
		if(k<25) i=k*nH/2;
		else if(k==25) i=nHalfHeight-24;
		else if(k<51)  i=nHalfHeight+(k-26)*nH/2;
		else if(k==51) i=2*nHalfHeight-24;       

		//nW/2, nH 영역만 먼저구함.
		nSum=nSum2=0;
		for(ii=i;ii<i+nH;ii++)
			for(jj=x1;jj<x1+nW/2;jj++)
			{
				tmp=*(fm+pitch*ii+jj);
				nSum+=tmp;
				nSum2+=(tmp*tmp);
			}

		nX=0;
		for(j=x1;j<=x2-nW;j+=nW/2, nX++)
		{
			nSum_=nSum2_=0;
			for(ii=i;ii<i+nH;ii++)
				for(jj=j+nW/2;jj<j+nW;jj++)
				{
					tmp=*(fm+pitch*ii+jj);
					nSum_+=tmp;
					nSum2_+=(tmp*tmp);
				}			

			if(nSum+nSum_>0)
			{
				*(pStdev+nSPitch*nY+nX)=(int)(100*sqrt(((double)nArea*(nSum2+nSum2_)-(double)(nSum+nSum_)*(double)(nSum+nSum_))/((double)nArea*((double)nArea-1.))));	
			}
			else
			{
				*(pStdev+nSPitch*nY+nX)=0;
			}

			nMapCount++;

			nSum=nSum_;
			nSum2=nSum2_;
		}
	}

	*nSW=nX;
	*nSH=nY;
}

double GetCandiValue_HighCut(LPBYTE fm, int nPitch, CRect rect)
{
	int i,j, nTmp;
	long nSum=0;
	double dSum, dTmp, dMean;
	int nCount = (rect.bottom-rect.top)*(rect.right-rect.left);

	
	for(i=rect.top; i<rect.bottom; i++)
		for(j=rect.left; j<rect.right; j++)
			nSum += *(fm+nPitch*i+j);			
	
	if(nCount>0)
		dMean = (double)nSum/nCount;
	
	dSum=0;
	for(i=rect.top; i<rect.bottom; i++)
		for(j=rect.left; j<rect.right; j++)
		{
			nTmp    =*(fm+nPitch*i+j);
			dTmp    = nTmp - dMean;
			if(dTmp>0) dTmp=0;    
			dSum   += dTmp*dTmp;
		}
	
	return sqrt(dSum);
}


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

double FindCunicStd_LineScan(LPBYTE fmPrev, LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nLimitX1, int nLimitX2, int nSubHeight, int *nCunicValue)
{
	int i,j,k;
	int nY1, nY2;
	int nX1, nX2;
	int nSize=right-left;
	int tt,rr,ll,bb;
	int nHisto[256],nCount, nHCount, nSum, nAveMean;
	int nTemp;
	double dSum2, dStd[3], dRetStd;
	int nPos=0;
	int nOffset=64;
	int nMaxValue=0;

	*nCunicValue=0;
	if(nLimitX2-nLimitX1<64) return 0;
	if(left<nLimitX1 || right>nLimitX2) return 0;	

	nY1=top;
	nY2=bottom;

	dStd[0]=dStd[1]=dStd[2]=-1;
	for(k=0;k<3;k++)
	{

		if(k==0)
		{
			nX2=left-nOffset;
			nX1=nX2-nSize;
			if(nX1<nLimitX1) continue;
		}
		else if(k==1)
		{
			nX1=left;
			nX2=right;
		}
		else if(k==2)
		{
			nX1=right+nOffset;
			nX2=nX1+nSize;
			if(nX2>nLimitX2) continue;
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

		if(k==1) *nCunicValue=nMaxValue-nAveMean;
	}

	if(dStd[0]>0 && dStd[2]>0) dRetStd=dStd[1]-(dStd[0]+dStd[2])*0.5;
	else if(dStd[0]>0)		   dRetStd=dStd[1]-dStd[0];
	else if(dStd[2]>0)		   dRetStd=dStd[1]-dStd[2];
	else return 0;
	

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

int IsOverKillCross( LPBYTE fm, int nUseProfile, double *pProfile,  int nCx, int nCy, int nLimitX1, int nLimitX2, int pitch, int nGrabHeight)
{
	int ret=1; //과검 
	int i, j, k, m, n;
	int nTmp, nMax=0, nMaxPos;
	int left, top, right, bottom, nYGrabPos, nHCount=0;
	int nAvg=0, nLCount, nRCount, nY1, nY2;
	int nProfile[BAD_IMG_WIDTH];
	LPBYTE fmT;

	if(nLimitX2-nLimitX1<BAD_IMG_WIDTH) return 2;//과검

	left=nCx-BAD_IMG_WIDTH/2;  if(left <nLimitX1)   left =nLimitX1;
	right=left+BAD_IMG_WIDTH;  if(right>nLimitX2) { right=nLimitX2; left=right-BAD_IMG_WIDTH;}

	top   =nCy-BAD_IMG_WIDTH/2;  if(top<0) top=0;
	bottom=top+BAD_IMG_WIDTH;  if(bottom>g_System.m_nImageH) {bottom=g_System.m_nImageH; top=bottom-BAD_IMG_WIDTH;}
	if(top<g_System.m_nImagePartH && bottom>g_System.m_nImagePartH)
	{
		if(nCy<g_System.m_nImagePartH)
		{
			bottom=g_System.m_nImagePartH;
			top=bottom-BAD_IMG_WIDTH;
		}
		else
		{
			top=g_System.m_nImagePartH;
			bottom=top+BAD_IMG_WIDTH;
		}
	}

	//중간 세로 한줄이 모두 200넘으면 과검
	nLCount=0;
	for(j=left;j<right;j+=4)
	{
		nLCount=0;
		for(i=top;i<bottom;i++)
		{
			if(*(fm+pitch*i+j)>200)    nLCount++;
		}
		if(nLCount>=(bottom-top)) return 1; //과검
	}

	nLCount=nRCount=0;
	for(k=0;k<4;k++)
	{
		nY1=top+k*BAD_IMG_HEIGHT/4;
		nY2=nY1+BAD_IMG_HEIGHT/4;
		memset(nProfile, 0, BAD_IMG_HEIGHT*sizeof(int));
		for(i=nY1;i<nY2;i++)
			for(j=left;j<right;j++)
				nProfile[j-left]+=*(fm+pitch*i+j);

		nMax=nMaxPos=nAvg=0;
		for(j=0;j<BAD_IMG_WIDTH;j++)
		{
			nAvg+=nProfile[j];
			if(nProfile[j]>=nMax) 
			{
				nMax=nProfile[j];
				nMaxPos=j;
			}
		}
		nAvg/=BAD_IMG_WIDTH;
		if(nMaxPos<10 && nMax>(nAvg+30*32))					    nLCount++;
		else if(nMaxPos>BAD_IMG_HEIGHT-10 && nMax>(nAvg+30*32)) nRCount++;
	}

	if(nLCount==4 || nRCount==4) return 1;  //과검
	else                         return 0;  //과검아님
}

int CheckCunicAgain(LPBYTE fm, int nCx, int nCy,  int X1, int X2, int pitch,double dLimitvalue)
{
	int i, j, k,  ii, jj;
	LPBYTE fm1;
	int nData[64];
	int ll, tt, rr, bb;
	int ll_, tt_, rr_, bb_;
	int nSum, nTmp, nMaxRect=0, nMaxRectV=0;
	int nCol=0, nRow=0;
	int nMin, nMinX, nMinY;
	int nMax, nMaxX, nMaxY;
	int nDiff[4];
	int MINDIFF=64;
	double dMax=0,dStd, dStdSide[4]={100,100,100,100};
	int nStdX, nStdY;
	double dEdgeStd=1.7;
	double dNotEdgeStd=1.1;
	double dLimitStd;
	double dAddOffset=0;
	double dAddDiff=0;
	int HEIGHT=g_System.m_nImagePartH;
	int NUM_BUFFERS=g_System.m_nImagePart;

	if(X2-X1>pitch-5) dLimitStd=dNotEdgeStd;
	else
	{
		dLimitStd=dEdgeStd; //1.7
		dAddOffset=1.5;     //Edge가 있으면 최고 강화(1.5+4.5=6)
		dAddDiff  =1;
	}

	//왼쪽 PC에 Edge가 있으면 영향을 받을 수 있으므로 왼쪽 1/6영역은 강화된 값을 적용한다.
	if(g_Temp.m_nInspectX1>10)
	{
		if(nCx<pitch/6)
		{
			dLimitStd=dEdgeStd-0.2; //1.5
			dAddOffset=1;			//Edge옆 PC는 조금 강화(1+4.5=5.5)
			dAddDiff=0.5;
		}
	}
	else if(g_Temp.m_nInspectX2<pitch-10) //오른쪽 PC에 Edge가 있으면 영향을 받을 수 있으므로 오른쪽 1/6영역은 강화된 값을 적용한다.
	{
		if(nCx>pitch*5/6)
		{
			dLimitStd=dEdgeStd-0.2; //1.5
			dAddOffset=1;			//Edge옆 PC는 조금 강화(1+4.5=5.5)
			dAddDiff=0.5;
		}
	}

	////강화된 버전(2동코팅2호기 131만 적용? DC02 121번 적용으로 알고 있는데...)------------------
	//dLimitStd=1.7; 
	//dAddOffset=1.5;     //Edge가 있으면 최고 강화(1.5+4.5=6)
	//dAddDiff  =1.5;
	////------------------------------------------------------------------------------------------


	if(X2-X1<72) return 1; //영역이 작으면 과검
	

	ll=nCx-32; if(ll<X1) ll=X1;
	rr=ll+64;  if(rr>X2) { rr=X2; ll=rr-64;}

	tt=nCy-32; if(tt<0) tt=0;
	bb=tt+64;  if(bb>HEIGHT*NUM_BUFFERS) {bb=HEIGHT*NUM_BUFFERS; tt=bb-64;}
	if(tt<HEIGHT && bb>HEIGHT)
	{
		if(nCy<HEIGHT)
		{
			bb=HEIGHT;
			tt=bb-64;
		}
		else
		{
			tt=HEIGHT;
			bb=tt+64;
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
			{	ll=ll_+4;	rr=rr_+4;  }
			else
			{	ll=ll_-4;	rr=rr_-4;  }
		}
		if(k%2==1)
		{
			if(tt_<HEIGHT/2 || (tt_>=HEIGHT && tt_<HEIGHT*1.5))
			{	tt=tt_+4;	bb=bb_+4;  }
			else
			{	tt=tt_-4;	bb=bb_-4;  }
		}
		if(ll<X1 || rr>X2 || tt<0 || bb>HEIGHT*NUM_BUFFERS) continue;

		nRow=0;
		for(i=tt;i<bb-7;i+=8)
		{
			nCol=0;
			for(j=ll;j<rr-7;j+=8)
			{
				nSum=0;
				for(ii=i;ii<i+8;ii++)
					for(jj=j;jj<j+8;jj++)
						nSum+=*(fm+ii*pitch+jj);

				*(nData+nRow*8+nCol)=nSum;

				nCol++;
			}
			nRow++;
		}

		//최소값 주위 4개가 더 밝으면 쿠닉----------------------
		if(tt==0 || tt==HEIGHT)
		{
			nMin=255*64+1;
			for(i=0;i<6;i++)
				for(j=2;j<6;j++)
				{
					if(*(nData+i*8+j)<nMin)
					{
						nMin=*(nData+i*8+j);
						nMinX=j;
						nMinY=i;
					}				
				}

			nDiff[0]=*(nData+nMinY*8+nMinX-2)-nMin;		//좌
			nDiff[1]=*(nData+nMinY*8+nMinX+2)-nMin;		//우
			nDiff[3]=*(nData+(nMinY+2)*8+nMinX)-nMin;	//하
			if(nDiff[0]>=MINDIFF && nDiff[1]>=MINDIFF &&  nDiff[3]>=MINDIFF*2)
			{
				if((nDiff[0]+nDiff[1]+nDiff[3])/64.0>=(8.5+dAddDiff))
					return 0;
			}
		}
		else if(bb==HEIGHT || bb==HEIGHT*NUM_BUFFERS)
		{
			nMin=255*64+1;
			for(i=2;i<8;i++)
				for(j=2;j<6;j++)
				{
					if(*(nData+i*8+j)<nMin)
					{
						nMin=*(nData+i*8+j);
						nMinX=j;
						nMinY=i;
					}				
				}

			nDiff[0]=*(nData+nMinY*8+nMinX-2)-nMin;		//좌
			nDiff[1]=*(nData+nMinY*8+nMinX+2)-nMin;		//우
			nDiff[2]=*(nData+(nMinY-2)*8+nMinX)-nMin;	//상			

			if(nDiff[0]>=MINDIFF && nDiff[1]>=MINDIFF && nDiff[2]>=MINDIFF*2)
			{
				if((nDiff[0]+nDiff[1]+nDiff[2])/64.0>=(8.5+dAddDiff))
					return 0;
			}
		}
		else if(ll==X1)
		{
			nMin=255*64+1;
			for(i=2;i<6;i++)
				for(j=0;j<6;j++)
				{
					if(*(nData+i*8+j)<nMin)
					{
						nMin=*(nData+i*8+j);
						nMinX=j;
						nMinY=i;
					}				
				}
			nDiff[1]=*(nData+nMinY*8+nMinX+2)-nMin;		//우
			nDiff[2]=*(nData+(nMinY-2)*8+nMinX)-nMin;	//상
			nDiff[3]=*(nData+(nMinY+2)*8+nMinX)-nMin;	//하

			if( nDiff[1]>=MINDIFF && nDiff[2]>=MINDIFF && nDiff[3]>=MINDIFF)
			{
				if((nDiff[1]+nDiff[2]+nDiff[3])/64.0>=(8.5+dAddDiff))
					return 0;
			}
		}
		else if(rr==X2)
		{
			nMin=255*64+1;
			for(i=2;i<6;i++)
				for(j=2;j<8;j++)
				{
					if(*(nData+i*8+j)<nMin)
					{
						nMin=*(nData+i*8+j);
						nMinX=j;
						nMinY=i;
					}				
				}
			nDiff[0]=*(nData+nMinY*8+nMinX-2)-nMin;		//좌
			nDiff[2]=*(nData+(nMinY-2)*8+nMinX)-nMin;	//상
			nDiff[3]=*(nData+(nMinY+2)*8+nMinX)-nMin;	//하

			if(nDiff[0]>MINDIFF && nDiff[2]>=MINDIFF && nDiff[3]>=MINDIFF)
			{
				if((nDiff[0]+nDiff[2]+nDiff[3])/64.0>=(8.5+dAddDiff))
					return 0;
			}
		}
		else
		{
			nMin=255*64+1;
			for(i=2;i<6;i++)
				for(j=2;j<6;j++)
				{
					if(*(nData+i*8+j)<nMin)
					{
						nMin=*(nData+i*8+j);
						nMinX=j;
						nMinY=i;
					}				
				}
			nDiff[0]=*(nData+nMinY*8+nMinX-2)-nMin;		//좌
			nDiff[1]=*(nData+nMinY*8+nMinX+2)-nMin;		//우
			nDiff[2]=*(nData+(nMinY-2)*8+nMinX)-nMin;	//상
			nDiff[3]=*(nData+(nMinY+2)*8+nMinX)-nMin;	//하

			if(nDiff[0]>32 && nDiff[1]>32 && nDiff[2]>32 && nDiff[3]>32)
			{
				if((nDiff[0]+nDiff[1]+nDiff[2]+nDiff[3])/64.0>=(10+dAddDiff))
					return 0;
			}
			if(nDiff[0]>64*(1+dAddDiff) && nDiff[1]>64*(1+dAddDiff) && nDiff[2]>64*(1+dAddDiff) && nDiff[3]>64*(1+dAddDiff))
				return 0;
		}
		//----------------------------------------------------

		//제일밝은것과 제일 어두운게 4Rect 이하면 쿠닉--------------
		nMax=0;
		nMin=255*64+1;
		for(i=0;i<8;i++)
			for(j=0;j<8;j++)
			{				
				if(*(nData+i*8+j)>nMax)
				{
					nMax=*(nData+i*8+j);
					nMaxX=j;
					nMaxY=i;
				}
				if(*(nData+i*8+j)<nMin)
				{
					nMin=*(nData+i*8+j);
					nMinX=j;
					nMinY=i;
				}			
			}
		if(abs(nMinX-nMaxX)<=1 && abs(nMinY-nMaxY)<=3 && (nMax-nMin)/64.0>=5)
			return 0;
		//----------------------------------------------------------

		//옆에비교
		for(i=0;i<8;i++)
		{
			for(j=0;j<7;j++)
			{
				nTmp=*(nData+i*8+j)-*(nData+i*8+j+1); if(nTmp<0) nTmp*=-1;
				if(nTmp>nMaxRect) nMaxRect=nTmp;
			}
		}
		//위아래 비교
		for(i=0;i<7;i++)
		{
			for(j=0;j<8;j++)
			{
				nTmp=*(nData+i*8+j)-*(nData+(i+1)*8+j); if(nTmp<0) nTmp*=-1;
				if(nTmp>nMaxRectV) nMaxRectV=nTmp;
				if(nTmp>nMaxRect)  nMaxRect =nTmp;
			}
		}
		//대각선
		for(i=0;i<7;i++)
		{
			for(j=0;j<7;j++)
			{
				nTmp=*(nData+i*8+j)-*(nData+(i+1)*8+j+1); if(nTmp<0) nTmp*=-1;
				if(nTmp>nMaxRect) nMaxRect=nTmp;
			}
		}
		//대각선
		for(i=0;i<7;i++)
		{
			for(j=0;j<7;j++)
			{
				nTmp=*(nData+i*8+j+1)-*(nData+(i+1)*8+j); if(nTmp<0) nTmp*=-1;
				if(nTmp>nMaxRect) nMaxRect=nTmp;
			}
		}
		if(nMaxRect/64.0>=dLimitvalue+dAddOffset ) break;
	}


	if(X2-X1>=128) //STDEV
	{
		ll=nCx-64;  if(ll<X1)   ll=X1;
		rr=ll+128;  if(rr>X2) { rr=X2; ll=rr-128;}

		tt=nCy-64;  if(tt<0) tt=0;
		bb=tt+128;  if(bb>HEIGHT*NUM_BUFFERS) {bb=HEIGHT*NUM_BUFFERS; tt=bb-128;}
		if(tt<HEIGHT && bb>HEIGHT)
		{
			if(nCy<HEIGHT)
			{
				bb=HEIGHT;
				tt=bb-128;
			}
			else
			{
				tt=HEIGHT;
				bb=tt+128;
			}
		}
		fm1=fm+pitch*tt+ll;

		for(i=0;i<15;i++)
		{
			for(j=0;j<15;j++)
			{
				dStd=SearchStdevData(fm1, j*8, i*8, j*8+16, i*8+16, pitch);
				if(dStd>=dMax)
				{
					dMax=dStd;
					nStdX=j;
					nStdY=i;
				}
			}
		}

		if(nStdX>2 && nStdX<12)
		{
			if(nStdX<13)dStdSide[0]=SearchStdevData(fm1, (nStdX+2)*8, nStdY*8,   (nStdX+2)*8+16, nStdY*8+16,    pitch);
			if(nStdX>1) dStdSide[1]=SearchStdevData(fm1, (nStdX-2)*8, nStdY*8,   (nStdX-2)*8+16, nStdY*8+16,    pitch);

			if(nStdY<13) dStdSide[2]=SearchStdevData(fm1, nStdX*8,	  (nStdY+2)*8, nStdX*8+16,   (nStdY+2)*8+16, pitch);
			if(nStdY>1)  dStdSide[3]=SearchStdevData(fm1, nStdX*8,    (nStdY-2)*8, nStdX*8+16,   (nStdY-2)*8+16, pitch);
//			if(dMax-dStdSide[0]>dLimitStd || dMax-dStdSide[1]>dLimitStd || dMax-dStdSide[2]>dLimitStd || dMax-dStdSide[3]>dLimitStd || dMax*4-(dStdSide[0]+dStdSide[1]+dStdSide[2]+dStdSide[3])>4) 
			if(dMax-dStdSide[2]>dLimitStd || dMax-dStdSide[3]>dLimitStd) 
				return 0;
		}
	}


	if(nMaxRect/64.0<dLimitvalue+dAddOffset || nMaxRectV/64<dLimitvalue*0.5) 
		return 1;
	else
		return 0;
}
double SearchStdevData(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch)
{
	int i,j;
	long nSum=0;
	int tmp;
	double dSum2=0, stdev=0; 
	long   nArea;
//	if(x1<0 || x2>pitch || y1<0 ) return 0;

	nArea=(x2-x1)*(y2-y1);
    for(i=y1;i<y2;i++)
		for(j=x1;j<x2;j++)
		{
			tmp=*(fm+pitch*i+j);
			nSum+=tmp;
			dSum2+=(tmp*tmp);
		}

    stdev=sqrt((nArea*dSum2-(double)nSum*(double)nSum)/((double)nArea*((double)nArea-1.)));	
	return stdev;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	CROSS 군집
//
////////////////////////////////////////////////////////////////////////////////////////////////////
void WEB_CrossGroup(int nCompresion)
{
	CRect rect, rc;
	unsigned char* fm = g_fmFlat[g_ID]; //평활이미지	
	int i, j, ii,jj, k;
	int ll,tt,rr,bb;
	int nMax, nSum, nSumCount;
	int nWidth=g_System.m_nImageW;
	int nHeight=g_System.m_nImageH;
	int pitch=g_System.m_nPitch;
	int nX1=g_Temp.m_nInspectX1;
	int nX2=g_Temp.m_nInspectX2;
	int nLevelInspect=0, nOffset;
	int left, top, right, bottom;
	int nGroupTh	=g_Param.m_nGroupAreaTh;	
	int nGroupCount =g_Param.m_nGroupCount;
	int nGroupArea	=g_Param.m_nGroupArea;
	double dGroupSize=g_Param.m_dGroupSize;
	int nThreshold=g_System.m_nFlatBright+g_Param.m_nGroupAreaTh;
	int nSizeMethod=g_Param.m_nSizeMethod;
	int N, nCount=0, nCandiCnt=0;
	int nX[100], nY[100], nValue[100];
	double cx, cy, angle, dSize, pSize[2], pos[4][2];
	double dXDist, dYDist, dist1, dist2;
	int dX, dY, Cx, Cy;
	int nMulti=1, nClass, nGroupCnt=0;
	double dScaleX=g_Param.m_dScaleFactorX;
	double dScaleY=g_Param.m_dScaleFactorY;
	int nMaxValue, nMaxI;

	//검사 Check-----------------------------------------------------------------------
	if(nGroupTh>0 && nGroupCount>0) nLevelInspect=1;
	else                            return;
	//----------------------------------------------------------------------------------
	
	if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) return;

	if(g_System.m_nMakePyramid && nCompresion)  //큰영상이라 압축하고, 압축한 것을 사용하도록 되어 있으면
	{
		fm=g_fmGrabComp;
		nWidth	/=2;
		nHeight	/=2;
		pitch	/=2;
		nX1/=2;
		nX2/=2;
		nMulti=2;
		dScaleX*=2;
		dScaleY*=2;
		nGroupArea/=2;
	}

	//평범한 Binary
	ThreadBinary(fm, g_fmTemp, nX1, 0, nX2, nHeight, pitch, nThreshold);	//4096x4096   약 1~2msec  (Thread안하면 5msec)
	

	g_Chain.SetChainData(1, g_fmTemp, 0, 0, 1, 500, nWidth, nHeight);
	N=g_Chain.FastChain(nX1, 0, nX2, nHeight-1);

	if(N<nGroupCount) return;

	for(i=0;i<N;i++)
	{
		if(g_Chain.Chain_Area(i)<0) continue;  //Hole Skip

		ll=g_Chain.FindMinX(i);
		tt=g_Chain.FindMinY(i);
		rr=g_Chain.FindMaxX(i);
		bb=g_Chain.FindMaxY(i);
		pSize[0]=(rr-ll+1)*dScaleX;
		pSize[1]=(bb-tt+1)*dScaleY;
		dSize=(pSize[0]+pSize[1])/2;
		cx=(ll+rr)/2;
		cy=(tt+bb)/2;
		if(nSizeMethod && rr-ll>=2 &&  bb-tt>=2)
		{
			g_Chain.Chain_Center(i, &cx, &cy);
			angle=g_Chain.FindAngle(i);
			if(fabs(angle)<1)
			{
				if(nSizeMethod==2) dSize=(pSize[0]+pSize[1])*0.5;
				else			   dSize=pSize[0];
			}
			else if( fabs(fabs(angle)-90) <1)
			{
				if(nSizeMethod==2) dSize=(pSize[0]+pSize[1])*0.5;
				else			   dSize =pSize[1];
			}
			else
			{
				g_Chain.FindMinRect(i, cx, cy, angle,  pos);
				dXDist=(fabs(pos[0][0]-pos[1][0]))*dScaleX; if(dXDist<0) dXDist=1;
				dYDist=(fabs(pos[0][1]-pos[1][1]))*dScaleY; if(dYDist<0) dYDist=1;
				dist1=sqrt(dXDist*dXDist+dYDist*dYDist);
				dXDist=(fabs(pos[0][0]-pos[3][0]))*dScaleX; if(dXDist<0) dXDist=1;
				dYDist=(fabs(pos[0][1]-pos[3][1]))*dScaleY; if(dYDist<0) dYDist=1;
				dist2=sqrt(dXDist*dXDist+dYDist*dYDist);
				if(nSizeMethod==1) dSize=(dist1+dist2)/2;
				else
				{
					if(dist1>dist2) dSize=dist1;
					else            dSize=dist2;
				}
			}
		}
		if(dSize>=dGroupSize && dSize<1.0)
		{
			nMax=0;
			for(ii=tt;ii<=bb;ii++)
				for(jj=ll;jj<=rr;jj++)
				{
					if(*(fm+pitch*ii+jj)>nMax) nMax=*(fm+pitch*ii+jj);
				}
			nSum=nSumCount=0;
			if(tt>10)			 {nSum+=*(fm+pitch*((tt+bb)/2-10)+(ll+rr)/2); nSumCount++;}
			if(bb<nHeight-10)    {nSum+=*(fm+pitch*((tt+bb)/2+10)+(ll+rr)/2); nSumCount++;}
			if(ll>nX1+10)		 {nSum+=*(fm+pitch*((tt+bb)/2)+(ll+rr)/2-10); nSumCount++;}
			if(rr<nX2-10)		 {nSum+=*(fm+pitch*((tt+bb)/2)+(ll+rr)/2+10); nSumCount++;}
			if(nSumCount>0 && nMax-nSum/nSumCount >=nGroupTh)
			{
				nX[nCandiCnt]=(int)cx;
				nY[nCandiCnt]=(int)cy;
				nValue[nCandiCnt]=nMax-g_System.m_nFlatBright;
				nCandiCnt++;
			}
		}
		if(nCandiCnt>=100) break;
	}

	for(i=0;i<nCandiCnt;i++)
	{
		 nMaxValue=0;
		 nMaxI=-1;
		for(j=0;j<nCandiCnt;j++)
		{
			if(nValue[j]>nMaxValue)
			{
				nMaxValue=nValue[j];
				nMaxI=j;
			}
		}
		if(nMaxI<0) continue;
		nValue[nMaxI]=0;
		 
		if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) continue;
		if(nX[nMaxI]==0 && nY[nMaxI]==0) continue;

		nCount=1;
		Cx=nX[nMaxI];
		Cy=nY[nMaxI];
		for(j=0;j<nCandiCnt;j++)
		{
			if(j==nMaxI) continue;
			if(nX[j]==0 && nY[j]==0) continue;
			dX=Cx-nX[j]; if(dX<0) dX*=-1;
			dY=Cy-nY[j]; if(dY<0) dY*=-1;
			if(dX<=nGroupArea/2 && dY <=nGroupArea/2)
			{
				nCount++;
			}
		}

		if(nCount>g_Temp.m_nGroupMaxCount) g_Temp.m_nGroupMaxCount=nCount;

		if(nCount>=nGroupCount)
		{
			//한군집이 검출되면 그 영역 지움.------------
			for(j=0;j<nCandiCnt;j++)
			{
				if(nX[j]==0 && nY[j]==0) continue;
				dX=Cx-nX[j]; if(dX<0) dX*=-1;
				dY=Cy-nY[j]; if(dY<0) dY*=-1;
				if(dX<=nGroupArea/2 && dY <=nGroupArea/2)
				{
					nX[j]=nY[j]=0;;
				}
			}
			//--------------------------------------------
						
			Cx*=nMulti;
			Cy*=nMulti;
			if(!IsInspectArea(Cx)) continue; //In Position Check

			MakeDefectRect(g_Defect.m_nDefectCount, Cx, Cy, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			CopyNGImageFM(g_fmFlat[g_ID], g_System.m_nPitch);
			nClass=(1)*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_GROUP;
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=Cx;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=Cy;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nMaxValue; //Value	
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =nCount*0.001+9; 
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =nCount*0.001+9; 
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =nCount*0.001+9;; 
			if(CheckDefectOverlap()==0) 
			{
				g_Defect.m_nDefectCount++;
				if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) return;
//				nGroupCnt++;
//				if(nGroupCnt>=2) return;
			}
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////
//
//  크로스 꼽침
//
///////////////////////////////////////////////////////////////////////////////////
int FindCrossLRBlack(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nMinValue, long *pProfile, int *pX, int *pY, int *pValue, int *pValueAdjust);
int FindCrossLRWhite(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nMinValue, long *pProfile, int *pX, int *pY, int *pValue, int *pValueAdjust);
void WEB_CrossKkopChim()
{
	int i,j,nX, nY, nValue, nValueAdjust=0, nMaxValue=0, nMaxValueW=0, nMaxX=0, nMaxY=0;
	CRect rect;;
	LPBYTE fm = g_fmGrab[g_ID];
	int nWidth=g_System.m_nImageW;
	int nHeight=g_System.m_nImageH;
	int pitch=g_System.m_nPitch;
	int nX1=g_Temp.m_nInspectX1;
	int nX2=g_Temp.m_nInspectX2;
	int nCount=0;
	int nLevel=10, nLevelW,  dLineTh=0,nLevelInspect=0;

	
	if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) return;

	//검사 Check-----------------------------------------------------------------------
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])	
		{			
			if(	g_Param.m_dLineThreshold[i]>0)
			{
				nLevelInspect=1;
				break;
			}
		}
	}
	if(nLevelInspect==0) return;
	g_Temp.m_nLRInspect=1;
	//----------------------------------------------------------------------------------


	rect.left	=g_Temp.m_nInspectX1;
	rect.top	=0;
	rect.right	=g_Temp.m_nInspectX2;
	rect.bottom	=g_System.m_nImageH;


	//전체 영역의 반만으로 꼽침 찾느다.
	Emphasize(0, fm, g_fmTemp, nX1, 0, nX2, nHeight/2-1, nWidth, 50, 5, 10); //m_nX:50 m_nY:5

	memset(	l_cProfile, 0, nWidth*sizeof(long));
	for(i=10;i<nHeight/2-10;i+=2)
	{
		nCount++;
		for(j=nX1;j<nX2;j++)
			l_cProfile[j]+=*(g_fmTemp+pitch*i+j);
	}
	for(j=nX1;j<nX2;j++)
		l_cProfile[j]/=nCount;
	//----------------------------------------------
	
	FindCrossLRBlack(g_fmTemp, nX1+25, 25, nX2-25, nHeight/2-25, nWidth, 100, l_cProfile, &nX, &nY, &nValue, &nValueAdjust);
	nValueAdjust/=4;
	nValue/=4;

	if(	nValue>nMaxValue)
	{
		  nMaxValue=nValue;
		  nMaxX=nX;
		  nMaxY=nY;
	}

	g_Temp.m_nMaxLRValue=  nMaxValue;
	g_Temp.m_nMaxLRPosX=nMaxX;
	nLevel=GetLevel(nMaxValue, g_Param.m_dLineThreshold);

	if(nLevel>0)
	{
		MakeDefectRect(g_Defect.m_nDefectCount, nX, nY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
		CopyNGImageFM(g_fmTemp, g_System.m_nPitch);
		g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_KKOPCHIM;
		g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nX;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nY;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nMaxValue; //Value	
		g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0; //Size
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0; //Size X
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0; //Size Y

		if(g_Param.m_nSlimPercent>=10 && nMaxValue>0)
		{
			if(100*(nMaxValue-nValueAdjust)/nMaxValue >= g_Param.m_nSlimPercent)
			{
				g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =(int)nValueAdjust; //Value
				g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=10*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_KKOPCHIM;
			}
		}
		if(CheckDefectOverlap()==0)   							
			g_Defect.m_nDefectCount++;
	}
	if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) return;
	
	FindCrossLRWhite(g_fmTemp, nX1+25, 25, nX2-25, nHeight/2-25, nWidth, 100, l_cProfile, &nX, &nY, &nValue, &nValueAdjust);
	nValueAdjust/=4;
	nValue/=4;
	if(	nValue>nMaxValue)
	{
		  nMaxValueW=nValue;
		  nMaxX=nX;
		  nMaxY=nY;
	}

	if(nMaxValueW>g_Temp.m_nMaxLRValue)
	{
		g_Temp.m_nMaxLRValue=  nMaxValueW;
		g_Temp.m_nMaxLRPosX =  nMaxX;
	}

	nLevelW=GetLevel(nMaxValueW, g_Param.m_dLineThreshold);
	if(nLevelW<0) return;

	if(nLevel>0 && g_Defect.m_nDefectCount>0 && abs(nMaxX-g_Defect.m_Defect[g_Defect.m_nDefectCount-1].x_pos)<128)
	{
		if(nLevelW<nLevel || ( nLevelW==nLevel && nMaxValueW>nMaxValue))
		{
			g_Defect.m_nDefectCount--;
		}
		else
		{
			return;
		}
	}
	
	nLevel=nLevelW;
	if(nLevel>0)
	{
		MakeDefectRect(g_Defect.m_nDefectCount, nX, nY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
		CopyNGImageFM(g_fmTemp, g_System.m_nPitch);
		g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_KKOPCHIM;
		g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nX;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nY;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nMaxValueW; //Value	
		g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0; //Size
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0; //Size X
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0; //Size Y

		if(g_Param.m_nSlimPercent>=10 && nMaxValueW>0)
		{
			if(100*(nMaxValueW-nValueAdjust)/nMaxValueW >= g_Param.m_nSlimPercent)
			{
				g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =(int)nValueAdjust; //Value
				g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=10*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_KKOPCHIM;
			}
		}
		if(CheckDefectOverlap()==0)   							
			g_Defect.m_nDefectCount++;
	}
}

//InspectB() 꼽침검사
void WEB_LineKkopChim()
{
	int i,j,nX, nY, nValue, nValueAdjust=0, nMaxValue=0, nMaxValueW=0, nMaxX=0, nMaxY=0;
	CRect rect;;
	LPBYTE fm = g_fmGrab[g_ID];
	int nWidth, nHeight, nPitch, nX1, nX2;
	int nCount=0;
	int nLevel=10, nLevelW,  dLineTh=0,nLevelInspect=0;
	int nMulti=1;
	int nOffset=256;

	if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) return;

	//검사 Check-----------------------------------------------------------------------
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])	
		{			
			if(	g_Param.m_dLineThreshold[i]>0)
			{
				nLevelInspect=1;
				break;
			}
		}
	}
	if(nLevelInspect==0) return;
	g_Temp.m_nLRInspect=1;
	//----------------------------------------------------------------------------------

	nWidth		=g_System.m_nImageW;
	nHeight		=g_System.m_nImageH;
	nPitch		=g_System.m_nPitch;
	nX1			=g_Temp.m_nInspectX1;
	nX2			=g_Temp.m_nInspectX2;

	if(g_System.m_nMakePyramid)
	{
		fm=g_fmGrabComp;
		nWidth	/=2;
		nHeight	/=2;
		nPitch	/=2;
		nX1/=2;
		nX2/=2;
		nMulti=2;
	}


	rect.left	=nX1;
	rect.top	=0;
	rect.right	=nX2;
	rect.bottom	=1024;


	//중앙 1024Pixel (압축한것은 512Pixel) 0.015면 15mm
	ThreadEmphasize(fm, g_fmTemp, nWidth, nHeight/2-nOffset, nHeight/2+nOffset, nPitch, 50,5,10);
//	for(i=0;i<1024;i++)
//		for(j=0;j<4096;j++)
//			*(g_fmFlat[0]+8192*i+j)=*(g_fmTemp+nPitch*i+j);

	memset(	l_cProfile, 0, nWidth*sizeof(long));
	for(i=nHeight/2-nOffset+10;i<nHeight/2+nOffset-10;i+=2)
	{
		nCount++;
		for(j=nX1;j<nX2;j++)
			l_cProfile[j]+=*(g_fmTemp+nPitch*i+j);
	}
	for(j=nX1;j<nX2;j++)
		l_cProfile[j]/=nCount;
	//----------------------------------------------
	
	FindCrossLRBlack(g_fmTemp, nX1+25, nHeight/2-nOffset+25, nX2-25, nHeight/2+nOffset-25, nPitch, 100, l_cProfile, &nX, &nY, &nValue, &nValueAdjust);

	nValueAdjust/=4;
	nValue/=4;

	if(	nValue>nMaxValue)
	{
		  nMaxValue=nValue;
		  nMaxX=nX;
		  nMaxY=nY;
	}

	g_Temp.m_nMaxLRValue=  nMaxValue;
	g_Temp.m_nMaxLRPosX=nMaxX;
	nLevel=GetLevel(nMaxValue, g_Param.m_dLineThreshold);

	if(nLevel>0)
	{
		MakeDefectRect(g_Defect.m_nDefectCount, nX*nMulti, nY*nMulti, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
		CopyNGImageFM(g_fmTemp, nPitch);
		g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_KKOPCHIM;
		g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nX*nMulti;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nY*nMulti;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nMaxValue; //Value	
		g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0; //Size
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0; //Size X
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0; //Size Y

		if(g_Param.m_nSlimPercent>=10 && nMaxValue>0)
		{
			if(100*(nMaxValue-nValueAdjust)/nMaxValue >= g_Param.m_nSlimPercent)
			{
				g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =(int)nValueAdjust; //Value
				g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=10*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_KKOPCHIM;
			}
		}
		if(CheckDefectOverlap()==0)   							
			g_Defect.m_nDefectCount++;
	}
	if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) return;

/*	흰선 꼽침에 대해서는 당분간 사용하지 않음
	FindCrossLRWhite(g_fmTemp, nX1+25, nHeight/2-nOffset+25, nX2-25, nHeight/2+nOffset-25, nPitch, 100, l_cProfile, &nX, &nY, &nValue, &nValueAdjust);
					
	nValueAdjust/=4;
	nValue/=4;
	if(	nValue>nMaxValue)
	{
		  nMaxValueW=nValue;
		  nMaxX=nX;
		  nMaxY=nY;
	}

	if(nMaxValueW>g_Temp.m_nMaxLRValue)
	{
		g_Temp.m_nMaxLRValue=  nMaxValueW;
		g_Temp.m_nMaxLRPosX =  nMaxX;
	}

	nLevelW=GetLevel(nMaxValueW, g_Param.m_dLineThreshold);
	if(nLevelW<0) return;

	if(nLevel>0 && g_Defect.m_nDefectCount>0 && abs(nMaxX-g_Defect.m_Defect[g_Defect.m_nDefectCount-1].x_pos)<128)
	{
		if(nLevelW<nLevel || ( nLevelW==nLevel && nMaxValueW>nMaxValue))
		{
			g_Defect.m_nDefectCount--;
		}
		else
		{
			return;
		}
	}
	
	nLevel=nLevelW;
	if(nLevel>0)
	{
		MakeDefectRect(g_Defect.m_nDefectCount, nX*nMulti, nY*nMulti, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
		CopyNGImageFM(g_fmTemp, nPitch);
		g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_KKOPCHIM;
		g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nX*nMulti;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nY*nMulti;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nMaxValueW; //Value	
		g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0; //Size
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0; //Size X
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0; //Size Y

		if(g_Param.m_nSlimPercent>=10 && nMaxValueW>0)
		{
			if(100*(nMaxValueW-nValueAdjust)/nMaxValueW >= g_Param.m_nSlimPercent)
			{
				g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =(int)nValueAdjust; //Value
				g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=10*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_KKOPCHIM;
			}
		}
		if(CheckDefectOverlap()==0)   							
			g_Defect.m_nDefectCount++;
	}
*/
}

int l_CurrLine[4][496];
void WEB_BATCurr()
{
#ifdef USE_SK_BAT
	int i,j,k,nX[5], nY[5], nValue[5], nValueAdjust[5];
	LPBYTE fm = g_fmGrab[g_ID];
	int nWidth, nHeight, nPitch, nX1, nX2, nY1, nY2;
	int nLevel, nLevelInspect=0;
	int nMulti=1;
	long nSum=0, nTmp, nMax, nMaxPos=0, nMaxPosK;
	double dAverage;

	if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) return;

	//검사 Check-----------------------------------------------------------------------
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])	
		{			
			if(	g_Param.m_dLineThreshold[i]>0)
			{
				nLevelInspect=1;
				break;
			}
		}
	}
	if(nLevelInspect==0) return;
	g_Temp.m_nLRInspect=1;
	//----------------------------------------------------------------------------------

	nWidth		=g_System.m_nImageW;
	nHeight		=g_System.m_nImageH;
	nPitch		=g_System.m_nPitch;
	nX1			=g_Temp.m_nInspectX1;
	nX2			=g_Temp.m_nInspectX2;

	if(nX1>10 || (nPitch-nX2)>10)
	{
		if(nX1>(nPitch-nX2))  nX1+=g_Param.m_nCurrNoInspArea;
		else                  nX2-=g_Param.m_nCurrNoInspArea;
	}

	if(nX2-nX1<30) return;

	if(g_System.m_nMakePyramid)
	{
		fm=g_fmGrabComp;
		nWidth	/=2;
		nHeight	/=2;
		nPitch	/=2;
		nX1/=2;
		nX2/=2;
		nMulti=2;
	}

	//Vertical Edge ------------------------------------------------------------

	/*for(i=10;i<nHeight-10;i++)
		for(j=nX1+4;j<nX2-4;j++)
		{
			nTmp=*(fm+nPitch*i+j-4)+*(fm+nPitch*i+j+4)-*(fm+nPitch*i+j)*2;
			if(nTmp<0) nTmp=0;
			else if(nTmp>255) nTmp=255;

			*(g_fmTemp+nPitch*i+j)=nTmp;						 
		}*/
	ThreadFilterVerEdge(fm, g_fmTemp, nX1, 10, nX2, nHeight-10, nPitch);
	//-------------------------------------------------------------------------


	nSum=0;
	memset(	l_CurrLine, 0, 4*496*sizeof(int));
	for(k=0;k<4;k++)
	{
		if(k==0)      {nY1=10;         nY2=nHeight/4;}
		else if(k==1) {nY1=nHeight/4;  nY2=nHeight/2;}
		else if(k==2) {nY1=nHeight/2;  nY2=nHeight*3/4;}
		else if(k==3) {nY1=nHeight*3/4;nY2=nHeight-10;}

		for(i=nY1;i<nY2;i++)
		{
			for(j=nX1+4;j<nX2-4;j++)
				l_CurrLine[k][j/10]+=*(g_fmTemp+nPitch*i+j);
		}

		for(j=nX1/10+1;j<nX2/10-1;j++)
		{
			l_CurrLine[k][j]/=(nY2-nY1);
			nSum+=l_CurrLine[k][j];
		}
	}
	
	if(nX2/10-nX1/10-2>0) dAverage=(double)nSum/((nX2/10-nX1/10-2)*4);
	else  return;
	
	//----------------------------------------------
	
	for(i=0;i<5;i++)
	{
		nValue[i]=0;

		nMax=0;
		nMaxPos=-1;
		for(k=0;k<4;k++)
			for(j=nX1/10+2;j<nX2/10-2;j++)
			{
				nTmp=l_CurrLine[k][j-1]+l_CurrLine[k][j]+l_CurrLine[k][j+1];
				if(nTmp>nMax)
				{
					nMax=nTmp;
					nMaxPos=j;
					nMaxPosK=k;
				}
			}

		if(nMaxPos>0)
		{
			nValue[i]=2*(nMax*10-dAverage*3*10);if(nValue[i]<0) nValue[i]=0;
			nX[i]=nMaxPos*10*nMulti;
			nY[i]=nMulti*(nMaxPosK*(nHeight/4)+nHeight/8);

			l_CurrLine[nMaxPosK][nMaxPos-1]=l_CurrLine[nMaxPosK][nMaxPos]=l_CurrLine[nMaxPosK][nMaxPos+1]=0;
		}

		if(i==0)
		{
			g_Temp.m_nMaxLRValue=nValue[i];
			g_Temp.m_nMaxLRPosX=nX[i];
		}

		nLevel=GetLevel(nValue[i], g_Param.m_dLineThreshold);

		if(nLevel>0)
		{
			MakeDefectRect(g_Defect.m_nDefectCount, nX[i], nY[i], g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			//CopyNGImageFM(fm, nPitch);
			CopyNGImageFMArea(fm, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].left/nMulti, nY[i]/nMulti-256, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].right/nMulti, nY[i]/nMulti+256, nPitch, 0);
			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_KKOPCHIM;
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nX[i];
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nY[i];
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nValue[i]; //Value	
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0; //Size
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0; //Size X
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0; //Size Y

			if(CheckDefectOverlap()==0)   							
				g_Defect.m_nDefectCount++;
		}
		else
		{
			break;
		}		
	}
	if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) return;
#endif
}


//좀 두꺼운 라인을 검출한다.
int BlackThickLineSub(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nMinValue, long *pProfile, int *pX, int *pY, int *pValue, int *pValueAdjust);
void WEB_BlackThickLine()
{
	int i,j,nX[5], nY[5], nValue[5], nValueAdjust[5], nLineCnt;
	CRect rect;;
	LPBYTE fm = g_fmGrab[g_ID];
	int nWidth, nHeight, nPitch, nX1, nX2;
	int nCount=0;
	int nLevel=10, nLevelW,  dLineTh=0,nLevelInspect=0;
	int nMulti=1;
	int nOffset=256;

	if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) return;

	//검사 Check-----------------------------------------------------------------------
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])	
		{			
			if(	g_Param.m_dLineThreshold[i]>0)
			{
				nLevelInspect=1;
				break;
			}
		}
	}
	if(nLevelInspect==0) return;
	g_Temp.m_nLRInspect=1;
	//----------------------------------------------------------------------------------

	nWidth		=g_System.m_nImageW;
	nHeight		=g_System.m_nImageH;
	nPitch		=g_System.m_nPitch;
	nX1			=g_Temp.m_nInspectX1;
	nX2			=g_Temp.m_nInspectX2;

	if(g_System.m_nMakePyramid)
	{
		fm=g_fmGrabComp;
		nWidth	/=2;
		nHeight	/=2;
		nPitch	/=2;
		nX1/=2;
		nX2/=2;
		nMulti=2;
	}


	rect.left	=nX1;
	rect.top	=0;
	rect.right	=nX2;
	rect.bottom	=1024;



	memset(	l_cProfile, 0, nWidth*sizeof(long));
	for(i=10;i<nHeight-10;i+=2)
	{
		nCount++;
		for(j=nX1;j<nX2;j++)
			l_cProfile[j]+=*(fm+nPitch*i+j);
	}
	for(j=nX1;j<nX2;j++)
		l_cProfile[j]/=nCount;
	//----------------------------------------------
	
	nLineCnt=BlackThickLineSub(fm, nX1+10, 25, nX2-10, nHeight-25, nPitch, 100, l_cProfile, nX, nY, nValue, nValueAdjust);


	for(i=0;i<nLineCnt;i++)
	{
		if(nValue[i]>g_Temp.m_nMaxLRValue)
		{
			g_Temp.m_nMaxLRValue=nValue[i];
			g_Temp.m_nMaxLRPosX=nX[i]*nMulti;
		}
		
	
		nLevel=GetLevel(nValue[i], g_Param.m_dLineThreshold);

		if(nLevel>0)
		{
			MakeDefectRect(g_Defect.m_nDefectCount, nX[i]*nMulti, nY[i]*nMulti, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			//CopyNGImageFM(fm, nPitch);
			CopyNGImageFMArea(fm, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].left/nMulti, 0, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].right/nMulti, nHeight, nPitch, 0);
			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_KKOPCHIM;
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nX[i]*nMulti;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nY[i]*nMulti;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nValue[i]; //Value	
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0; //Size
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0; //Size X
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0; //Size Y

			if(CheckDefectOverlap()==0)   							
				g_Defect.m_nDefectCount++;
		}
	}
	if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) return;
}
//*pValue : 꼽침 값
//*pValueAdjust: 주변과 비교해서 보정한 값(꼽침값 보다 조금 작음. 많이 작으면 쓸림이 될 수 있다)
//최대 3개 찾음
void LineSort(int count, long *Value, int *Pos);
int BlackThickLineSub(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nMinValue, long *pProfile, int *pX, int *pY, int *pValue, int *pValueAdjust)
{
	int i,ii, j, jj, loop=0;
	int nLRFind=0;
	long nSum, nMaxSum, nMaxPos=0;
	long nMaxSumMax, nMaxPos_;
	long nMaxSumOrg, nMaxSumOrgMax;
	long nAvgSum=0, nCount;
	int nDevide=1;
	double dDistMax, dDist;
	long nLocalMax[32], nLocalMaxAvg, nLocalMaxCount;
	int  nLocalMaxPos[32];
	int nDevide16=pitch/32;
	int nX[5], nY[5], nValue[5], nValueAdjust[5];
	int nUpperCut=5;
	int nUnderCut=30;
	int nMoveOffset=20;
	int nValueOffset=10;


	*pX=*pY=*pValue=0;
	if(right-left<150) return 0; //원래는 최소 120 [ Emphasize (25,25), Moveing Avg (35, 35) ]

	//평균 및 사용할 Profile정리-------------------------
	nAvgSum=0;
	for(j=left;j<right;j++)
	{
		l_nProfileAvg[j]=pProfile[j];
		nAvgSum+=pProfile[j];
	}
	nAvgSum/=(right-left);

	//상 하  CUT------------------------------------------
	//엉청밝은 휘점때문에 튀는 것 방지
	for(j=left;j<right;j++) l_nProfileSum[j]=0;

	for(i=top;i<bottom;i++)
	{
		for(j=left;j<right;j++)
		{
			if(*(fm+pitch*i+j)<l_nProfileAvg[j]-nUnderCut)
			{
				l_nProfileSum[j]+=(l_nProfileAvg[j]-nUnderCut);
			}
			else if(*(fm+pitch*i+j)>l_nProfileAvg[j]+nUpperCut)
			{
				l_nProfileSum[j]+=(l_nProfileAvg[j]+nUpperCut);
			}
			else
				l_nProfileSum[j]+=*(fm+pitch*i+j);
		}
	}
	//----------------------------------------------------
	nMaxSumMax=nMaxSumOrgMax=nMaxPos_=0;
	memset(nLocalMax, 0, 32*sizeof(long));
	memset(nLocalMaxPos, 0, 32*sizeof(int));

	//Moving Average (중간 2*nValueOffset빼고) 구함----------------------------
	nDevide=(nMoveOffset*2-nValueOffset*2);
	for(j=left+nMoveOffset;j<right-nMoveOffset;j++)
	{
		nSum=0;
		for(jj=j-nMoveOffset;jj<j-nValueOffset;jj++)
			nSum+=l_nProfileSum[jj];
		for(jj=j+nValueOffset+1;jj<=j+nMoveOffset;jj++)
			nSum+=l_nProfileSum[jj];
		
		l_nProfileSumMoveinAvg[j]=nSum/nDevide;
	}
	for(j=left;j<left+nMoveOffset;j++)
		l_nProfileSumMoveinAvg[j]=l_nProfileSumMoveinAvg[left+nMoveOffset];
	for(j=right-nMoveOffset+1;j<right;j++)
		l_nProfileSumMoveinAvg[j]=l_nProfileSumMoveinAvg[right-nMoveOffset-1];
	//------------------------------------------------------------------------

	//(nValueOffset*2)개 합------------------------------------------------------------------
	nSum=0;


	for(jj=left;jj<=left+2*nValueOffset;jj++)
		nSum+=(l_nProfileSumMoveinAvg[jj]-l_nProfileSum[jj]);

	for(j=left+nValueOffset;j<right-nValueOffset;j++)
	{		
		if(nSum>nLocalMax[j/nDevide16])
		{
			nLocalMax[j/nDevide16]=nSum;
			nLocalMaxPos[j/nDevide16]=j;
		}

		if(j!=(right-nValueOffset-1))
			nSum+=((l_nProfileSumMoveinAvg[j+nValueOffset+1]-l_nProfileSum[j+nValueOffset+1])-(l_nProfileSumMoveinAvg[j-nValueOffset]-l_nProfileSum[j-nValueOffset]));
	}

	//제일 큰 것 2개 없앰, 나머지로 평균값 구함---------
	LineSort(32, nLocalMax, nLocalMaxPos);
	nLocalMaxAvg=nLocalMaxCount=0;
	for(j=3;j<16-3;j++)
	{
		if(nLocalMax[j]>0)
		{
			nLocalMaxAvg+=nLocalMax[j];
			nLocalMaxCount++;
		}
	}

	if(nLocalMaxCount) nLocalMaxAvg=nLocalMaxAvg/nLocalMaxCount;
	else 
	{
		for(j=2;j>0;j--)
		{
			if(nLocalMax[j]>0)
			{
				nLocalMaxAvg=nLocalMax[j];
				break;
			}
		}
	}

	for(i=0;i<32;i++)
		nLocalMax[i]=(int)(100.0*100*nLocalMax[i]/((bottom-top)*(nValueOffset*2)*nAvgSum));

	nLRFind=0;
	for(i=0;i<32;i++)
	{
		dDistMax=4096;
		for(j=0;j<nLRFind;j++)
		{
			dDist=nLocalMaxPos[i]-pX[j];
			if(dDist<0) dDist*=-1;
			if(dDist<dDistMax) dDistMax=dDist;
		}
		if(dDistMax<64) continue;

		pValue[nLRFind]=nLocalMax[i];
		pValueAdjust[nLRFind]=nLocalMax[i]-nLocalMaxAvg;
		pX[nLRFind]=nLocalMaxPos[i];
		pY[nLRFind]=(top+bottom)/2;
		nLRFind++;
		if(nLRFind>=3) break;
	}
	
	return nLRFind;
}


void LineSort(int count, long *Value, int *Pos)
{
	int i,j;
	long MaxValue;
	int Number;
	long StoreValue;
	int  StoreValuePos;

	for(i=0;i<count-1;i++)
	{
		Number=i;
		MaxValue=StoreValue=Value[i];
		StoreValuePos=Pos[i];
	      
		for(j=i+1;j<count;j++)
			if(Value[j] > MaxValue)
			{
				MaxValue=Value[j];
				Number=j; 
			}  
		  
		
		Value[i]=Value[Number]; //제일 큰 것
		Pos[i]=Pos[Number]; //제일 큰 것
		
		Value[Number]=StoreValue;
		Pos[Number]=StoreValuePos;
	}
}


//*pValue : 꼽침 값
//*pValueAdjust: 주변과 비교해서 보정한 값(꼽침값 보다 조금 작음. 많이 작으면 쓸림이 될 수 있다)
int FindCrossLRBlack(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nMinValue, long *pProfile, int *pX, int *pY, int *pValue, int *pValueAdjust)
{
	int i,ii, j, jj, loop=0;
	int nLRFind=0;
	long nSum, nMaxSum, nMaxPos=0;
	long nMaxSumMax, nMaxPos_;
	long nMaxSumOrg, nMaxSumOrgMax;
	int nMoveOffset=25;
	int nValueOffset=10;
	long nAvgSum=0, nCount;
	int nDevide=1;
	long nLocalMax[16], nLocalMaxAvg, nLocalMaxCount;
	int nDevide16=pitch/16;

	if(g_Param.m_dScaleFactorX<0.045) 
	{
		nMoveOffset=21;
		nValueOffset=8;
	}
	else if(g_Param.m_dScaleFactorX<0.06)
	{
		nMoveOffset=15;
		nValueOffset=6;
	}
	else
	{
		nMoveOffset=13;
		nValueOffset=5;
	}


	*pX=*pY=*pValue=0;
	if(right-left<150) return 0; //원래는 최소 120 [ Emphasize (25,25), Moveing Avg (35, 35) ]

	//Profile 구하기---------------------------------
	nAvgSum=0;
	if(pProfile==NULL)
	{
		for(j=left;j<right;j++) l_nProfileSum[j]=0;

		for(i=top;i<bottom;i++)
			for(j=left;j<right;j++)
				l_nProfileSum[j]+=*(fm+pitch*i+j);

		for(j=left;j<right;j++)
		{
			l_nProfileAvg[j]=l_nProfileSum[j]/(bottom-top);
			nAvgSum+=l_nProfileAvg[j];
		}
	}
	else
	{
		for(j=left;j<right;j++)
		{
			l_nProfileAvg[j]=pProfile[j];
			nAvgSum+=pProfile[j];
		}
	}
	nAvgSum/=(right-left);

	//상 하 +/- 5 CUT-----------------------------------
	//엉청밝은 휘점때문에 튀는 것 방지
	for(j=left;j<right;j++) l_nProfileSum[j]=0;

	for(i=top;i<bottom;i++)
	{
		for(j=left;j<right;j++)
		{
			if(*(fm+pitch*i+j)<l_nProfileAvg[j]-5)
			{
				l_nProfileSum[j]+=(l_nProfileAvg[j]-5);
			}
			else if(*(fm+pitch*i+j)>l_nProfileAvg[j]+5)
			{
				l_nProfileSum[j]+=(l_nProfileAvg[j]+5);
			}
			else
				l_nProfileSum[j]+=*(fm+pitch*i+j);
		}
	}
	//----------------------------------------------------
	nMaxSumMax=nMaxSumOrgMax=nMaxPos_=0;
	for(loop=0;loop<3;loop++)
	{
		memset(nLocalMax, 0, 16*sizeof(long));
		if(loop) nMoveOffset+=2;	

		//Moving Average (중간 2*nValueOffset빼고) 구함----------------------------
		nDevide=(nMoveOffset*2-nValueOffset*2);
		for(j=left+nMoveOffset;j<right-nMoveOffset;j++)
		{
			nSum=0;
			for(jj=j-nMoveOffset;jj<j-nValueOffset;jj++)
				nSum+=l_nProfileSum[jj];
			for(jj=j+nValueOffset+1;jj<=j+nMoveOffset;jj++)
				nSum+=l_nProfileSum[jj];
		
			l_nProfileSumMoveinAvg[j]=nSum/nDevide;
		}
		for(j=left;j<left+nMoveOffset;j++)
			l_nProfileSumMoveinAvg[j]=l_nProfileSumMoveinAvg[left+nMoveOffset];
		for(j=right-nMoveOffset+1;j<right;j++)
			l_nProfileSumMoveinAvg[j]=l_nProfileSumMoveinAvg[right-nMoveOffset-1];
		//------------------------------------------------------------------------

		//(nValueOffset*2)개 합------------------------------------------------------------------
		nSum=0;
		for(jj=left;jj<=left+2*nValueOffset;jj++)
			nSum+=(l_nProfileSumMoveinAvg[jj]-l_nProfileSum[jj]);

		nMaxSum=0;
		for(j=left+nValueOffset;j<right-nValueOffset;j++)
		{		
			if(nSum>nMaxSum)
			{
				nMaxSum=nSum;
				nMaxPos=j;
			}
			if(nSum>nLocalMax[j/nDevide16])
				nLocalMax[j/nDevide16]=nSum;

			if(j!=(right-nValueOffset-1))
				nSum+=((l_nProfileSumMoveinAvg[j+nValueOffset+1]-l_nProfileSum[j+nValueOffset+1])-(l_nProfileSumMoveinAvg[j-nValueOffset]-l_nProfileSum[j-nValueOffset]));
		}

		//제일큰것 2개 없앰, 나머지로 평균값 구함-----------------------------
		EdgeSort(16, nLocalMax);
		nLocalMaxAvg=nLocalMaxCount=0;
		for(j=3;j<16-3;j++)
		{
			if(nLocalMax[j]>0)
			{
				nLocalMaxAvg+=nLocalMax[j];
				nLocalMaxCount++;
			}
		}
		if(nLocalMaxCount) nLocalMaxAvg=nLocalMaxAvg/nLocalMaxCount;
		else 
		{
			for(j=2;j>0;j--)
			{
				if(nLocalMax[j]>0)
				{
					nLocalMaxAvg=nLocalMax[j];
					break;
				}
			}
		}
		nMaxSumOrg=nMaxSum;           //, nMaxSumOrgMax
		nMaxSum-=nLocalMaxAvg;
		//-------------------------------------------------------------------


		if((bottom-top)==0 || nAvgSum==0) nMaxSum=nMaxSumOrg=0;
		else  
		{
			if(nAvgSum<g_System.m_nFlatBright) nAvgSum=g_System.m_nFlatBright;

			nMaxSum=(int)(100.0*100*nMaxSum/((bottom-top)*(nValueOffset*2)*nAvgSum));
			nMaxSumOrg=(int)(100.0*100*nMaxSumOrg/((bottom-top)*(nValueOffset*2)*nAvgSum));
		}

		if(nMaxSumOrg> nMaxSumOrgMax)
		{
			nMaxSumMax=nMaxSum;
			nMaxSumOrgMax=nMaxSumOrg;
			nMaxPos_=nMaxPos;
		}
	}

	*pValue=nMaxSumOrgMax;
	*pValueAdjust=nMaxSumMax;
	*pX=nMaxPos_;
	*pY=(top+bottom)/2;

	if(nMaxSumOrgMax>=nMinValue) nLRFind=1;

	return nLRFind;
}

//*pValue : 꼽침 값
//*pValueAdjust: 주변과 비교해서 보정한 값(꼽침값 보다 조금 작음. 많이 작으면 쓸림이 될 수 있다)
int FindCrossLRWhite(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nMinValue, long *pProfile, int *pX, int *pY, int *pValue, int *pValueAdjust)
{
	int i,ii, j, jj, loop=0;
	int nLRFind=0;
	long nSum, nMaxSum, nMaxPos=0;
	long nMaxSumMax, nMaxPos_;
	long nMaxSumOrg, nMaxSumOrgMax;
	int nMoveOffset=25;
	int nValueOffset=10;
	long nAvgSum=0, nCount;
	int nDevide=1;
	long nLocalMax[16], nLocalMaxAvg, nLocalMaxCount;
	int nDevide16=pitch/16;

	if(g_Param.m_dScaleFactorX<0.045) 
	{
		nMoveOffset=21;
		nValueOffset=8;
	}
	else if(g_Param.m_dScaleFactorX<0.06)
	{
		nMoveOffset=15;
		nValueOffset=6;
	}
	else
	{
		nMoveOffset=13;
		nValueOffset=5;
	}


	*pX=*pY=*pValue=0;
	if(right-left<150) return 0; //원래는 최소 120 [ Emphasize (25,25), Moveing Avg (35, 35) ]

	//Profile 구하기---------------------------------
	nAvgSum=0;
	if(pProfile==NULL)
	{
		for(j=left;j<right;j++) l_nProfileSum[j]=0;

		for(i=top;i<bottom;i++)
			for(j=left;j<right;j++)
				l_nProfileSum[j]+=*(fm+pitch*i+j);

		for(j=left;j<right;j++)
		{
			l_nProfileAvg[j]=l_nProfileSum[j]/(bottom-top);
			nAvgSum+=l_nProfileAvg[j];
		}
	}
	else
	{
		for(j=left;j<right;j++)
		{
			l_nProfileAvg[j]=pProfile[j];
			nAvgSum+=pProfile[j];
		}
	}
	nAvgSum/=(right-left);

	//상 하 +/- 5 CUT-----------------------------------
	//엉청밝은 휘점때문에 튀는 것 방지
	for(j=left;j<right;j++) l_nProfileSum[j]=0;

	for(i=top;i<bottom;i++)
	{
		for(j=left;j<right;j++)
		{
			if(*(fm+pitch*i+j)<l_nProfileAvg[j]-35)
			{
				l_nProfileSum[j]+=(l_nProfileAvg[j]-35);
			}
			else if(*(fm+pitch*i+j)>l_nProfileAvg[j]+55)
			{
				l_nProfileSum[j]+=(l_nProfileAvg[j]+55);
			}
			else
				l_nProfileSum[j]+=*(fm+pitch*i+j);
		}
	}
	//----------------------------------------------------
	nMaxSumMax=nMaxSumOrgMax=nMaxPos_=0;
	for(loop=0;loop<3;loop++)
	{
		memset(nLocalMax, 0, 16*sizeof(long));
		if(loop) nMoveOffset+=2;	

		//Moving Average (중간 2*nValueOffset빼고) 구함----------------------------
		nDevide=(nMoveOffset*2-nValueOffset*2);
		for(j=left+nMoveOffset;j<right-nMoveOffset;j++)
		{
			nSum=0;
			for(jj=j-nMoveOffset;jj<j-nValueOffset;jj++)
				nSum+=l_nProfileSum[jj];
			for(jj=j+nValueOffset+1;jj<=j+nMoveOffset;jj++)
				nSum+=l_nProfileSum[jj];
		
			l_nProfileSumMoveinAvg[j]=nSum/nDevide;
		}
		for(j=left;j<left+nMoveOffset;j++)
			l_nProfileSumMoveinAvg[j]=l_nProfileSumMoveinAvg[left+nMoveOffset];
		for(j=right-nMoveOffset+1;j<right;j++)
			l_nProfileSumMoveinAvg[j]=l_nProfileSumMoveinAvg[right-nMoveOffset-1];
		//------------------------------------------------------------------------

		//(nValueOffset*2)개 합------------------------------------------------------------------
		nSum=0;
		for(jj=left;jj<=left+2*nValueOffset;jj++)
			nSum+=(l_nProfileSum[jj]-l_nProfileSumMoveinAvg[jj]);

		nMaxSum=0;
		for(j=left+nValueOffset;j<right-nValueOffset;j++)
		{		
			if(nSum>nMaxSum)
			{
				nMaxSum=nSum;
				nMaxPos=j;
			}
			if(nSum>nLocalMax[j/nDevide16])
				nLocalMax[j/nDevide16]=nSum;

			if(j!=(right-nValueOffset-1))
				nSum+=((l_nProfileSum[j+nValueOffset+1]-l_nProfileSumMoveinAvg[j+nValueOffset+1])-(l_nProfileSum[j-nValueOffset]-l_nProfileSumMoveinAvg[j-nValueOffset]));
		}

		//제일큰것 2개 없앰, 나머지로 평균값 구함---------
		EdgeSort(16, nLocalMax);
		nLocalMaxAvg=nLocalMaxCount=0;
		for(j=3;j<16-3;j++)
		{
			if(nLocalMax[j]>0)
			{
				nLocalMaxAvg+=nLocalMax[j];
				nLocalMaxCount++;
			}
		}
		if(nLocalMaxCount) nLocalMaxAvg=nLocalMaxAvg/nLocalMaxCount;
		else 
		{
			for(j=2;j>0;j--)
			{
				if(nLocalMax[j]>0)
				{
					nLocalMaxAvg=nLocalMax[j];
					break;
				}
			}
		}
		nMaxSumOrg=nMaxSum;           //, nMaxSumOrgMax
		nMaxSum-=nLocalMaxAvg;
		//------------------------------------------------

		if((bottom-top)==0 || nAvgSum==0) nMaxSum=nMaxSumOrg=0;
		else  
		{
			if(nAvgSum<80) nAvgSum=80;

			nMaxSum=100*100*nMaxSum/((bottom-top)*(nValueOffset*2)*nAvgSum);
			nMaxSumOrg=100*100*nMaxSumOrg/((bottom-top)*(nValueOffset*2)*nAvgSum);
		}

		if(nMaxSumOrg> nMaxSumOrgMax)
		{
			nMaxSumMax=nMaxSum;
			nMaxSumOrgMax=nMaxSumOrg;
			nMaxPos_=nMaxPos;
		}
	}

	*pValue=nMaxSumOrgMax;
	*pValueAdjust=nMaxSumMax;
	*pX=nMaxPos_;
	*pY=(top+bottom)/2;

	if(nMaxSumOrgMax>=nMinValue) nLRFind=1;

	return nLRFind;
}
