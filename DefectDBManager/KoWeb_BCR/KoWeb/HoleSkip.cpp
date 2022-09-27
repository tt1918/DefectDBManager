
#include "stdafx.h"

#ifdef USE_HOLE_SKIP
#include "math.h"

//불량검출시 좌/우에 붙었는데 (세로)/(가로)>3 되는 불량은 검출하지 않는다. (코딩 넣어야 함 ) 
#define SIDE_WIDTH	128
int l_Index=0;
void HoleRemoveProcess()
{
	int i, j;
	int pitchC=g_System.m_nImageW/8;
	int pitch =g_System.m_nImageW;
	int nHeight= (g_System.m_nImageH/8+2*SUB_FRAME_HEIGHT/8);
	int nTh    =g_Param.m_nHoleTh+g_System.m_nFlatBright;
	int nHolePeriodPixel=0;
	double dErrorRatio=g_Param.m_dHoleErrorRatio, dCompactness;
	int nFlatValue=g_System.m_nFlatBright;
	int nHoleSizePixelX=50;
	int nHoleSizePixelY=50;
	int nHoleSizePixel;
	int nX, nY;
	int nLeft =g_Temp.m_nInspectX1/8;
	int nRight=g_Temp.m_nInspectX2/8;
	int nBlob;
	int ll,tt,rr,bb;
	int nHoleX, nHoleY, nDiff, nMinDiff=1000;
	int nHoleFound=0;
	int nHoleCandi=0, nPosX[10], nPosY[10], nSize[10], nRealSize[10],  nSideTouch[10]={0,};
	int nHeightOrg=g_System.m_nImageH+2*SUB_FRAME_HEIGHT;
	LPBYTE fmL, fmR;

	LPBYTE fmD=g_fmGrabComp1;
	LPBYTE fmS;

	//TEST DATA---------------------------
	//g_Param.m_nAreaThUp=10;
	//g_Param.m_dHoleSize=2.4;
	//nTh=g_System.m_nFlatBright+10; 
	//dErrorRatio=25;
	//------------------------------------

	if(g_Param.m_dScaleFactorX>0) nHoleSizePixelX=g_Param.m_dHoleSize/g_Param.m_dScaleFactorX/8;  //1/8압축
	if(g_Param.m_dScaleFactorY>0)
	{	
		nHoleSizePixelY=g_Param.m_dHoleSize/g_Param.m_dScaleFactorY/8;    //1/8압축
		nHolePeriodPixel=g_Param.m_dHolePeriod/g_Param.m_dScaleFactorY/8; //1/8압축된 Hole주기 Pixel
	}

	//영상을 1/8 로 압축해서 이진화 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	memset(g_fmGrabComp1, 0, nHeight*pitchC);
		

	//이전 Frame의 끝 부분을 이진화----------------------------------
	if(g_Temp.m_GrabNextDelay==2)
	{
		fmS=g_fmSubPrevFlat;
		nY=0;
		for(i=0;i<SUB_FRAME_HEIGHT;i+=8, nY++)
		{	
			nX=0;
			for(j=0;j<pitch;j+=8, nX++)
				if(*(fmS+pitch*i+j)>nTh)  *(fmD+pitchC*nY+nX)=255;
		}
	}

	//현재 Frame 이진화----------------------------------------------
	fmS=g_fmFlat[g_ID];
	nY=SUB_FRAME_HEIGHT/8;
	for(i=0;i<g_System.m_nImageH;i+=8, nY++)
	{	
		nX=0;
		for(j=0;j<pitch;j+=8, nX++)
			if(*(fmS+pitch*i+j)>nTh)  *(fmD+pitchC*nY+nX)=255;
	}		

	//다음 Frame 앞부분을 밑에 이진화 -------------------------------
	if(g_Temp.m_GrabNextDelay>0)
	{
		fmS=g_fmSubNextFlat;
		nY=g_System.m_nImageH/8+SUB_FRAME_HEIGHT/8;
		for(i=0;i<SUB_FRAME_HEIGHT;i+=8, nY++)
		{	
			nX=0;
			for(j=0;j<pitch;j+=8, nX++)
				if(*(fmS+pitch*i+j)>nTh)  *(fmD+pitchC*nY+nX)=255;
		}
	}
	//--------------------------------------------------------------------------------------------

	//Blob해서 Hole후보 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	g_Chain.SetChainData(1, fmD, 1, 1, 8, 1000, pitchC, nHeight);	
	nBlob=g_Chain.FastChain(nLeft, 0, nRight, nHeight-1);
	
	if(nBlob>0)
	{
		for(i=0;i<nBlob;i++)
		{
			if(nHoleCandi>=10) break;

			ll=g_Chain.FindMinX(i);
			tt=g_Chain.FindMinY(i);
			rr=g_Chain.FindMaxX(i);
			bb=g_Chain.FindMaxY(i);

			if(tt<=1 || bb>=nHeight-2) continue;   //위/아래 걸치면 제외한다.(위 128, 아래 128 포함해서 원을 구한거라 위/아래 걸치면 안됨)

			if(ll<=nLeft+1 || rr>=nRight-2)  //왼쪽 걸침, 오른쪽 걸침
			{
				if((bb-tt)>0.5*nHoleSizePixelY)  //Y길이가 Hole지름/2 보다 클때
				{
					nSideTouch[nHoleCandi]=1;
					nPosX[nHoleCandi]=8*(ll+rr)/2;
					nPosY[nHoleCandi]=8*(bb+tt)/2-SUB_FRAME_HEIGHT;    //SUB_FRAME_HEIGHT를 빼줘야 원래 Frame에서의 위치
					nRealSize[nHoleCandi]=(bb-tt+1)*8;
					if((rr-ll)>nHoleSizePixelX/2) nSize[nHoleCandi]=(bb-tt+1)*8;
					else                          nSize[nHoleCandi]=8*2*sqrt( (bb-tt+1)*(bb-tt+1)/4+(nHoleSizePixelX/2.0-(rr-ll+1))*(nHoleSizePixelX/2.0-(rr-ll+1)) );					
					nHoleCandi++;
				}
			}			
			else  //아무데도 안걸침
			{
				dCompactness=g_Chain.FindCompactness(i);
				if(dCompactness<0.75) continue;

				if( ((rr-ll)+(bb-tt))/2> 0.5*(nHoleSizePixelX+nHoleSizePixelY)/2 )
				{
					nPosX[nHoleCandi]=8*(ll+rr)/2;
					nPosY[nHoleCandi]=8*(bb+tt)/2-SUB_FRAME_HEIGHT;
					nSize[nHoleCandi]=((rr-ll+1)+(bb-tt+1))*4;
					nRealSize[nHoleCandi]=((rr-ll+1)+(bb-tt+1))*4;
					nHoleCandi++;
				}
			}
		}
	}

	//좌우 옆에서 Hole후보 찾기 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	if(g_Param.m_dScaleFactorX>0) nHoleSizePixelX=g_Param.m_dHoleSize/g_Param.m_dScaleFactorX;  //1/8압축
	if(g_Param.m_dScaleFactorY>0) nHoleSizePixelY=g_Param.m_dHoleSize/g_Param.m_dScaleFactorY;  //1/8압축

	nLeft =g_Temp.m_nInspectX1; if(nLeft<0) nLeft=0;
	nRight=g_Temp.m_nInspectX2; if(nRight>=g_System.m_nImageW) nRight=g_System.m_nImageW-1;
	                            if(nRight<SIDE_WIDTH)  nRight=SIDE_WIDTH;

	fmL=g_fmTemp;
	fmR=g_fmTemp+nHeightOrg*SIDE_WIDTH;

	memset(fmL, 0, nHeightOrg*SIDE_WIDTH);
	memset(fmR, 0, nHeightOrg*SIDE_WIDTH);	

	//이전 Frame의 끝을 맨먼저 이진화--------------------------------
	if(g_Temp.m_GrabNextDelay==2)
	{
		fmS=g_fmSubPrevFlat+nLeft;
		fmD=fmL;
		for(i=0;i<SUB_FRAME_HEIGHT;i++)
		{	
			for(j=0;j<pitch;j++)
				if(*(fmS+pitch*i+j)>nTh)  *(fmD+SIDE_WIDTH*i+j)=255;
		}

		fmS=g_fmSubPrevFlat+nRight-SIDE_WIDTH;
		fmD=fmR;
		for(i=0;i<SUB_FRAME_HEIGHT;i++)
		{	
			for(j=0;j<SIDE_WIDTH;j++)
				if(*(fmS+pitch*i+j)>nTh)  *(fmD+SIDE_WIDTH*i+j)=255;
		}
	}

	//현재 Frame 이진화------------------------------------------------
	fmS=g_fmFlat[g_ID]+nLeft;
	fmD=fmL+SUB_FRAME_HEIGHT*SIDE_WIDTH;
	for(i=0;i<g_System.m_nImageH;i++)
	{	
		for(j=0;j<SIDE_WIDTH;j++)
			if(*(fmS+pitch*i+j)>nTh)  *(fmD+SIDE_WIDTH*i+j)=255;
	}		

	fmS=g_fmFlat[g_ID]+nRight-SIDE_WIDTH;
	fmD=fmR+SUB_FRAME_HEIGHT*SIDE_WIDTH;
	for(i=0;i<g_System.m_nImageH;i++)
	{	
		for(j=0;j<SIDE_WIDTH;j++)
			if(*(fmS+pitch*i+j)>nTh)  *(fmD+SIDE_WIDTH*i+j)=255;
	}	

	//다음 Frame 이진화-----------------------------------------------
	if(g_Temp.m_GrabNextDelay>0)
	{
		fmS=g_fmSubNextFlat+nLeft;
		fmD=fmL+(nHeightOrg-SUB_FRAME_HEIGHT)*SIDE_WIDTH;
		for(i=0;i<SUB_FRAME_HEIGHT;i++)
		{	
			for(j=0;j<SIDE_WIDTH;j++)
				if(*(fmS+pitch*i+j)>nTh)  *(fmD+SIDE_WIDTH*i+j)=255;
		}

		fmS=g_fmSubNextFlat+nRight-SIDE_WIDTH;
		fmD=fmR+(nHeightOrg-SUB_FRAME_HEIGHT)*SIDE_WIDTH;
		for(i=0;i<SUB_FRAME_HEIGHT;i++)
		{	
			for(j=0;j<SIDE_WIDTH;j++)
				if(*(fmS+pitch*i+j)>nTh)  *(fmD+SIDE_WIDTH*i+j)=255;
		}
	}

	//왼쪽 Blob
	g_Chain.SetChainData(1, fmL, 1, 1, 10, 2000, SIDE_WIDTH, nHeightOrg);	
	nBlob=g_Chain.FastChain(0, 0, SIDE_WIDTH-1, nHeightOrg-1);
	
	if(nBlob>0)
	{
		for(i=0;i<nBlob;i++)
		{
			if(nHoleCandi>=10) break;

			ll=g_Chain.FindMinX(i);
			tt=g_Chain.FindMinY(i);
			rr=g_Chain.FindMaxX(i);
			bb=g_Chain.FindMaxY(i);

			if(tt<=1 || bb>=nHeightOrg-2) continue;  //위, 아래 걸치면 무시
			if(ll>1) continue;						 //왼쪽에 안걸치면 무시

			if((bb-tt+1)<=nHoleSizePixelY/2 || (bb-tt+1)/(rr-ll+1) < 3) continue;   //세로로 길지 않으면 무시

			nSideTouch[nHoleCandi]=1;
			nPosX[nHoleCandi]=nLeft+(ll+rr)/2;
			nPosY[nHoleCandi]=(bb+tt)/2-SUB_FRAME_HEIGHT;
			nRealSize[nHoleCandi]=bb-tt+1;
			nSize[nHoleCandi]=2*sqrt( (bb-tt+1)*(bb-tt+1)/4+(nHoleSizePixelX/2.0-(rr-ll+1))*(nHoleSizePixelX/2.0-(rr-ll+1)));					
			nHoleCandi++;
		}
	}

	//오른쪽 Blob
	g_Chain.SetChainData(1, fmR, 1, 1, 10, 2000, SIDE_WIDTH, nHeightOrg);	
	nBlob=g_Chain.FastChain(0, 0, SIDE_WIDTH-1, nHeightOrg-1);
	
	if(nBlob>0)
	{
		for(i=0;i<nBlob;i++)
		{
			if(nHoleCandi>=10) break;

			ll=g_Chain.FindMinX(i);
			tt=g_Chain.FindMinY(i);
			rr=g_Chain.FindMaxX(i);
			bb=g_Chain.FindMaxY(i);

			if(tt<=1 || bb>=nHeightOrg-2) continue;  //위, 아래 걸치면 무시
			if(rr<SIDE_WIDTH-2) continue;			 //오른쪽에 안걸치면 무시

			if((bb-tt+1)<=nHoleSizePixelY/2 || (bb-tt+1)/(rr-ll+1) < 3) continue;   //세로로 길지 않으면 무시

			nSideTouch[nHoleCandi]=1;
			nPosX[nHoleCandi]=nRight-SIDE_WIDTH+(ll+rr)/2;
			nPosY[nHoleCandi]=(bb+tt)/2-SUB_FRAME_HEIGHT;
			nRealSize[nHoleCandi]=bb-tt+1;
			nSize[nHoleCandi]=2*sqrt( (bb-tt+1)*(bb-tt+1)/4+(nHoleSizePixelX/2.0-(rr-ll+1))*(nHoleSizePixelX/2.0-(rr-ll+1)));					
			nHoleCandi++;
		}
	}


	if(nHoleCandi>0)
	{
		//Hole을 없앤다. 
		// - 좌/우 Touch 안한 것중에서 비슷한게 있으면 우선적으로)
		// - 좌/우 Touch 중에 큰 것
		nHoleSizePixel=(nHoleSizePixelX+nHoleSizePixelY)/2;
		nMinDiff=1000;
		for(i=0;i<nHoleCandi;i++)
		{
			if(nSideTouch[i]==0)
			{
				nDiff=nHoleSizePixel-nSize[i]; if(nDiff<0) nDiff*=-1;

				if(100.0*nDiff/nHoleSizePixel>dErrorRatio)  continue;

				//이전 Hole주기에서 Hole이 없으면 지금 Hole이라고 검출한것 SKIP하지 않는다.
				if(nHolePeriodPixel>1000)
				{
					if(CheckHoleExist(l_Index, nPosX[i], nPosY[i], nHolePeriodPixel, nSideTouch[i], nRealSize[i])==0) continue;
				}

				if(nDiff<nMinDiff)
				{
					nHoleFound=1;
					nMinDiff=nDiff;
					nHoleX=nPosX[i];
					nHoleY=nPosY[i];
				}
			}
		}

		if(nHoleFound==0)
		{	
			nMinDiff=1000;
			for(i=0;i<nHoleCandi;i++)
			{
				if(nSideTouch[i])
				{
					nDiff=nHoleSizePixel-nRealSize[i]; if(nDiff<0) nDiff*=-1;

					if(100.0*nDiff/nHoleSizePixel>dErrorRatio*2) continue;

					//이전 Hole주기에서 Hole이 없으면 지금 Hole이라고 검출한것 SKIP하지 않는다.
					if(nHolePeriodPixel>1000)
					{
						if(CheckHoleExist(l_Index, nPosX[i], nPosY[i], nHolePeriodPixel,  nSideTouch[i], nRealSize[i])==0) continue;
					}

					if(nDiff<nMinDiff)
					{
						nHoleFound=1;
						nMinDiff=nDiff;
						nHoleX=nPosX[i];
						nHoleY=nPosY[i];
					}
				}
			}
		}

		if(nHoleFound)
		{
			//Hole없앰
			RemoveHole(nHoleX, nHoleY, nLeft, nRight, nHoleSizePixelX*2, nHoleSizePixelY*2, g_Param.m_nAreaThUp*0.75, nFlatValue);

			goto Done;
		}
	}

Done:

	//Hole Binary이미지 Frame 전체 저장 ------------------------------------------------------------
	memcpy(g_fmHole[l_Index], g_fmGrabComp1+pitchC*(SUB_FRAME_HEIGHT/8), pitchC*g_System.m_nImageH/8);
	l_Index++;
	if(l_Index==3) l_Index=0;
	//----------------------------------------------------------------------------------------------

}

//실제로 Hole을 없애는 함수
void RemoveHole(int nCx, int nCy, int nX1, int nX2, int nSizeX, int nSizeY, int nTh, int nValue)
{
	//g_fmFlat[g_ID], g_fmSubPrevFlat, g_fmSubNextFlat 에서 모두 지운다.
	LPBYTE fmB;
	LPBYTE fm=g_fmFlat[g_ID];
	int pitch=g_System.m_nImageW;
	int nHeight=g_System.m_nImageH;
	int i, j, nY, nX;
	int left,top, right, bottom;
	int nBlob,  ll,tt,rr,bb;
	int nMax=0, ll_,tt_,rr_,bb_;


	if(nSizeY<SUB_FRAME_HEIGHT) nSizeY=SUB_FRAME_HEIGHT;
	if(nTh<5)                   nTh=5;

	nTh+=nValue;
	fmB=new BYTE[nSizeX*nSizeY];
	memset(fmB, 0, nSizeX*nSizeY);

	left=nCx-nSizeX/2; if(left<nX1) left=nX1;
	right=left+nSizeX;
	if(right>nX2)
	{
		right=nX2;
		left =right-nSizeX;
		if(left<nX1) left=nX1;
	}

	top=nCy-nSizeY/2;  if(top<(-1)*SUB_FRAME_HEIGHT) top=(-1)*SUB_FRAME_HEIGHT;
	bottom=top+nSizeY; 
	if(bottom>=nHeight+SUB_FRAME_HEIGHT)
	{
		bottom=nHeight+SUB_FRAME_HEIGHT-1;
		top   =bottom-SUB_FRAME_HEIGHT;
	}

	nY=0;
	if(top<0)  //홀이 이전Frame과 걸쳐 있는 경우 
	{
		//이전 Frame은 해당영역 전부 지움
		for(i=SUB_FRAME_HEIGHT+top;i<SUB_FRAME_HEIGHT; i++, nY++)
			for(j=left;j<right;j++)
				*(g_fmSubPrevFlat+pitch*i+j)=nValue;

		for(i=0;i<bottom; i++, nY++)
		{
			nX=0;
			for(j=left;j<right;j++, nX++)
			{
				if(*(fm+pitch*i+j)>nTh) *(fmB+nSizeX*nY+nX)=255;
			}
		}
	}
	else  if(bottom>nHeight)  //홀이 다음 Frame과 걸쳐 있는 경우 
	{
		for(i=top;i<nHeight; i++, nY++)
		{
			nX=0;
			for(j=left;j<right;j++, nX++)
			{
				if(*(fm+pitch*i+j)>nTh) *(fmB+nSizeX*nY+nX)=255;
			}
		}

		//이전 Frame은 해당영역은 전부 지움
		for(i=0;i<bottom-nHeight;i++)
			for(j=left;j<right;j++)
				*(g_fmSubNextFlat+pitch*i+j)=nValue;
	}
	else  //Hole이 이번 Frame에 온전히 있는 경우
	{
		for(i=top;i<bottom; i++, nY++)
		{
			nX=0;
			for(j=left;j<right;j++, nX++)
			{
				if(*(fm+pitch*i+j)>nTh) *(fmB+nSizeX*nY+nX)=255;
			}
		}
	}
	

	//제일 큰 Blob 평균값으로 채운다-----------------------------------------------
	g_Chain.SetChainData(1, fmB, 1, 1, 10, 1000, nSizeX, nSizeY);	
	nBlob=g_Chain.FastChain(0,0, nSizeX-1, nSizeY-1);
	
	if(nBlob>0)
	{
		for(i=0;i<nBlob;i++)
		{
			ll=g_Chain.FindMinX(i);
			tt=g_Chain.FindMinY(i);
			rr=g_Chain.FindMaxX(i);
			bb=g_Chain.FindMaxY(i);

			if((rr-ll)+(bb-tt) >nMax)
			{
				nMax=(rr-ll)+(bb-tt);
				ll_=ll-5; if(ll_<nX1)      ll_=nX1;
				tt_=tt-5; if(tt_<0)        tt_=0;
				rr_=rr+5; if(rr_>=nX2)     rr_=nX2-1;
				bb_=bb+5; if(bb_>=nHeight) bb_=nHeight-1;
			}
		}

		for(i=tt_;i<=bb_;i++)
			for(j=ll_;j<=rr_;j++)
			{
				if(*(fmB+nSizeX*i+j))
				{
					nY=top+i;
					nX=left+j;
					*(fm+pitch*nY+nX)=nValue;
				}
			}
	}
	//-------------------------------------------------------------------------------

	delete fmB;
}


//현재 Hole말고 이전에 주기 위치에 하나더 있었는지 확인: 당분간 return 1
int CheckHoleExist(int nIndex, int nCx, int nCy, int nHolePixelPeriod, int nSideTouch, int nSize)
{
	int nHoleExist=0;





	nHoleExist=1;
	return nHoleExist;
}


#endif