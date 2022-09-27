#include "stdafx.h"
#include "math.h"
#include "FishEye.h"

#ifdef USE_3D_AREA

#define WIDTH	1392

 int FindDefectBlob(LPBYTE fmGray, LPBYTE fmBinary, double dSizeValue, int left, int top, int right, int bottom, int pitch, double *pCx, double *pCy, double *pSizeX, double *pSizeY, double *pSize, int *Gray, int *nType, int *nStrongCount, double dCrowdSize, int nMaxDefect, int nMaxXDist, int nStrongValue);
 int CheckDefect_QWP(LPBYTE fm, int x, int y, int  nHHeight);
 int CheckVerticalSlope(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nSlope, int nMinCount);
 int CheckVerticalSlope1(LPBYTE fm, int Cx, int Cy, int SizeX, int SizeY,  int pitch, int nSlope, int nMinCount);
 int GetHTOSize(LPBYTE fm, int left, int top, int right, int bottom, int pitch, double dAvg, int nCoreTh, double dCloudUpTh, int *IsWhiteSpot, int *Cx, int *Cy, int *pValue, int *CoreSize,  double *dCloudValue, int *CloudSize);
 
 BYTE l_fm[128*128];
 BYTE l_fmT[128*128];

 int FindIsWhite(LPBYTE fm, int left, int top, int right, int bottom, int Y1, int Y2, int pitch);
 int FindGrayDiff(int nFindMax, LPBYTE fm, int left, int top, int right, int bottom, int pitch);
 int FindStrongCount(int nStrongValue, LPBYTE fm, int left, int top, int right, int bottom, int pitch);
 int CheckIsBlack(LPBYTE fm, int nCx, int nCy, int nX1, int nX2, int *pValue);
 int CheckIsBlackQWP(LPBYTE fm, int nCx, int nCy, int nX1, int nX2);
 void Binary3DFlatImage(LPBYTE fm, LPBYTE fmBinary, int nThUp, int nThDn, int left, int top, int right, int bottom, int pitch);
 void Close3DFlatImage(LPBYTE fmBinary, LPBYTE fmT, int left, int top, int right, int bottom, int pitch);
 int IsRealDefect(LPBYTE fm, int nX, int nY, double dSizeX, double dSizeY, int nOverkillSlope);
 int	FindLargeDefect(LPBYTE fm, int x1, int x2, int nHeight, int pitch, int nThreshold, int nSizeX, int nSizeY, int *MarkValue, int *nCx, int *nCy);

 //1)마크를 먼저 찾는다.
 //
void WEB_RotCrossInspect()
{
	int i,j, k,m, ii, jj,  nX, nY;
//roi2 문제로 수정.
	CRect rect;
	int nSkipX = 2;
	int nSkipY = 2;
	double dSize, dThSize;
	int nOffsetY = 10,nCheckX =0,nCheckY = 0;
	int nLevel, nClass, nLevel_, nClass_, nNGGroup=0;
	LPBYTE fm = g_fmFlat[g_ID];
	LPBYTE fmTT;

	int nVisionLine =  g_Temp.m_nPCOptic;;
	int nSkipAve=1;//32;
	int nLineTmp = 0,nPosTemp = 0;	
	int nl = rect.left+1, IsDefect, nTmp;
	
    //고영우 만든 알고즘으로 대체 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	double pCx[10], pCy[10], pSize[10], pSizeX[10], pSizeY[10];
	int nBlob, nType[10], nGray[10], nStrongCount[10];
	int nBlobB[2], nTypeB[2][10], nGrayB[2][10];
	int nMAXDEFECT=g_System.m_nMaxDefect;
	double dDist, dMinDist, dLimitDist;
	int nKCount, nBlockSize, nBlockCount, nCandiW[10][3], nCandiB[10][3];
	int nCandiW1[10][3], nCandiB1[10][3], posW1, posB1;
	int nX1, nX2, gray, nCx, nCy, left, top, right, bottom, nDefect, nLowDefect;
	int nMaxXDist;
	int nOverlap; //불량이 겹치면 없앤다.
	int MarkW, MarkH, MarkX1, MarkX2, MarkLevel, CompressSize, MarkValue,MarkValueSum, MarkValueThreshold, MarkOffset, MarkSlope, Slope, MarkPosX1, MarkPosX, MarkPosY, nMaxPosX, nMaxPosY, nOverKill, nMinPoint; //마킹전용 검사 
	int nCheckCount=1;
	int nOverkillSlope=g_Param.m_nOverkillSlope, nSlope;
	int nStrongValue=g_Param.m_nValueOneUp;
	int nIsBlack, nGrayValue;
	double dCloudValue, dNBOCloudValue, dNBOCloudSize;
	int IsNBOCloud;	
	int nMinThUp=255, nMinThDn=255;

	//HTO----------------------------
	int IsWhiteSpot;
	int Cx, Cy;
	int pValue, CoreSize, CloudSize;
	//-------------------------------

	//밝기조절하는 동안 마킹관련 데이타 Reset-----------------------------------------
	if(g_Temp.m_nMotionCheck || g_Temp.m_nAutoAdjustStart)  g_Temp.m_nMarkValueCount=0; 
	//--------------------------------------------------------------------------------

	rect.left=g_Temp.m_nInspectX1;
	rect.right=g_Temp.m_nInspectX2;
	rect.top=0;
	rect.bottom=g_System.m_nImageH;
	g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;

	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i] && g_Param.m_nValueUp[i]>0)
		{
			if(g_Param.m_nValueUp[i]<nMinThUp) nMinThUp=g_Param.m_nValueUp[i];
		}
		if(g_Param.m_nUseLevel[i] && g_Param.m_nValueDn[i]>0)
		{
			if(g_Param.m_nValueDn[i]<nMinThDn) nMinThDn=g_Param.m_nValueDn[i];
		}
	}
	if(nMinThUp!=255) g_Temp.m_nInspectWhite=1;
	if(nMinThDn!=255) g_Temp.m_nInspectBlack=1;


	if(nVisionLine==COS1)
	{
		nMAXDEFECT=2;   //불량최대 수 
		dLimitDist=300; //불량간 최대 붙을 수 있는 거리 
		nBlockSize=500; //Block에서 후보 검출 
		nMaxXDist=5;	//0.175mm Blob에서 이 거리만큼 떨어져 있으면 합친다.
		nMinPoint=1;

		MarkOffset=10;
	}
	else if(nVisionLine==COS2)
	{
		nMAXDEFECT=2;
		dLimitDist=100;
		nBlockSize=WIDTH/8;		
		nMaxXDist=10; //1mm
		nMinPoint=1;
		MarkOffset=5;
	}

	//{MARKING불량 처럼 큰 불량 검출 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	if(nVisionLine==COS1)
	{
		MarkX1=rect.left/32+1;
		MarkX2=rect.right/32-1;
		MarkLevel=5;
		CompressSize=32;
		g_Temp.m_nNEL2NotInspAreaX1=0;
		g_Temp.m_nNEL2NotInspAreaX2=0;
	}
	else if(nVisionLine==COS2)
	{
		MarkX1=rect.left/16+1;
		MarkX2=rect.right/16-1;
		MarkLevel=4;
		CompressSize=16;
	}

	if(g_Param.m_nMarkThreholdValue>0)
	{
		if(nVisionLine==COS1)
		{
			MarkPosX1=0;		  //l_fmBinary: 압축영상 
			ImageCompress1(fm, g_fmTemp, WIDTH, g_System.m_nImageH, WIDTH, MarkLevel, 15, &MarkW, &MarkH);   //영상을 압축한다.
		
			for(k=0;k<2;k++)
			{
				if(k==1 && MarkPosX1==0) continue; //k==0에서 불량이 없으면 끝 

				MarkValue=0;
				if(k==0)
				{
					for(i=0;i<MarkH;i++)
						for(j=MarkX1;j<MarkX2;j++)
						{
							if(*(g_fmTemp+MarkW*i+j)>MarkValue)
							{
								MarkValue=*(g_fmTemp+MarkW*i+j);
								MarkPosX=j;
								MarkPosY=i;
							}
						}
				}
				else
				{
					for(i=0;i<MarkH;i++)
					{
						for(j=MarkX1;j<MarkPosX1-MarkOffset;j++)
						{
							if(*(g_fmTemp+MarkW*i+j)>MarkValue)
							{
								MarkValue=*(g_fmTemp+MarkW*i+j);
								MarkPosX=j;
								MarkPosY=i;
							}
						}
						for(j=MarkPosX1+MarkOffset;j<MarkX2;j++)
						{
							if(*(g_fmTemp+MarkW*i+j)>MarkValue)
							{
								MarkValue=*(g_fmTemp+MarkW*i+j);
								MarkPosX=j;
								MarkPosY=i;
							}
						}
					}
				}

				//수평 Slope 로 한번더 체크한다--------------------------------------------
				if(MarkValue>0 && MarkPosX>=0 && MarkPosY>=0)
				{
					MarkSlope=0;
					for(j=MarkPosX-5;j<MarkPosX+5;j++)
					{
						if( j < MarkX1 || j > MarkX2+1) continue;
						Slope=*(g_fmTemp+MarkW*MarkPosY+j)-*(g_fmTemp+MarkW*MarkPosY+j+1);
						if(Slope<0) Slope*=-1;
						if(Slope>MarkSlope) MarkSlope=Slope;
					}
				}
				//-------------------------------------------------------------------------

				if(k==0)
				{
					g_Temp.m_nMarkValueMax=MarkValue;
					if(g_Temp.m_nMarkValueCount<100)
					{
						g_Temp.m_dMarkValueArray[g_Temp.m_nMarkValueCount]=MarkValue;
						g_Temp.m_nMarkValueCount++;
					}
					else
					{
						for(i=0;i<99;i++)
							g_Temp.m_dMarkValueArray[i]=g_Temp.m_dMarkValueArray[i+1];
						g_Temp.m_dMarkValueArray[99]=MarkValue;
					}
					MarkValueSum=0;
					for(i=0;i<g_Temp.m_nMarkValueCount;i++)
						MarkValueSum+=g_Temp.m_dMarkValueArray[i];

					//최근 100개의 평균값에 티칭 값을 더한 값이 기준값이다.
					MarkValueThreshold=MarkValueSum/g_Temp.m_nMarkValueCount+g_Param.m_nMarkThreholdValue;
					g_Temp.m_MarkValueThreshold=MarkValueThreshold;
				}
			}
		}
		else if(nVisionLine==COS2)
		{
			FindLargeDefect(fm, rect.left, rect.right, g_System.m_nImageH, WIDTH, g_Param.m_nMarkThreholdValue, 24, 16, &MarkValue, &nCx, &nCy);   //영상을 압축한다.

			//노이즈데이타를 최근 100 Frame저장한다. ----------------------------------------
			if(g_Temp.m_nMarkValueCount<100)
			{
				if(g_Temp.m_nMarkValueMax<=2) g_Temp.m_nMarkValueMax--;
				g_Temp.m_dMarkValueArray[g_Temp.m_nMarkValueCount]=g_Temp.m_nMarkValueMax;
				g_Temp.m_nMarkValueCount++;
			}
			else
			{
				for(i=0;i<99;i++)
					g_Temp.m_dMarkValueArray[i]=g_Temp.m_dMarkValueArray[i+1];

				if(g_Temp.m_nMarkValueMax<=2) g_Temp.m_nMarkValueMax--;
				g_Temp.m_dMarkValueArray[99]=g_Temp.m_nMarkValueMax;
			}

			MarkValueSum=0;
			for(i=0;i<g_Temp.m_nMarkValueCount;i++)
				MarkValueSum+=g_Temp.m_dMarkValueArray[i];
			//-------------------------------------------------------------------------------

			//최근 100개의 평균값에 티칭 값을 더한 값이 기준값이다. (원단의 좋지 않은 것은 기본적으로 높은 값이 나오므로)
			MarkValueThreshold=MarkValueSum/g_Temp.m_nMarkValueCount+g_Param.m_nMarkThreholdValue;
			g_Temp.m_MarkValueThreshold=MarkValueThreshold;
			MarkSlope=3;
			MarkPosX=nCx;
			MarkPosY=nCy;
		}


		if(MarkValue>=MarkValueThreshold && MarkSlope>=3 && g_Param.m_nMarkLevel<=10)
		{
			if(nVisionLine==COS1)
			{
				nX=MarkPosX*CompressSize;
				nY=MarkPosY*CompressSize;
			}
			else
			{
				nX=MarkPosX;
				nY=MarkPosY;
			}

			nLevel=g_Param.m_nMarkLevel;
			nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_MARKING;
		

			left=nX-BAD_IMG_WIDTH/2;
			right=left+BAD_IMG_WIDTH;
			if(left<0)		{ left=0;		right=left+BAD_IMG_WIDTH;}
			if(right>=WIDTH){ right=WIDTH-1; left=right-BAD_IMG_WIDTH;}
			top=nY-BAD_IMG_HEIGHT/2;   if(top<0) {top=0; bottom=top+BAD_IMG_HEIGHT;}
			bottom=nY+BAD_IMG_HEIGHT/2;if(bottom>=g_System.m_nImageH) {bottom=g_System.m_nImageH-1; top=bottom-BAD_IMG_HEIGHT;}
		

			nOverKill=1;
			//-------------------------------------------------------------------------------------------------------------
			// Vertical Slope로 과검 체크 
			if(g_Param.m_nOverkillSlope>12)
				nOverKill=CheckVerticalSlope(fm, left, top, right, bottom, WIDTH, nOverkillSlope, nMinPoint); //과점 체크 
			else
				nOverKill=CheckVerticalSlope(fm, left, top, right, bottom, WIDTH, 10, nMinPoint);			  //주어진 값으로 과점 체크 
			//------------------------------------------------------------------------------------------------------------

			if(nOverKill==1)
			{
				MakeDefectRect(g_Defect.m_nDefectCount, nX, nY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
				CopyNGImageFM(fm, WIDTH);

				g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nX;
				g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nY;
				g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;
				g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =-MarkValue; //Value	
				g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =2; //Size
				g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =2; //Size X
				g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =2; //Size Y

				if(CheckDefectOverlap()==0) 
						g_Defect.m_nDefectCount++;
			}	
	//		nIsBlack=CheckIsBlackQWP(l_fmFlat, nMaxPosX, nMaxPosY,rect.left, rect.right);
	//		if(nIsBlack==0)  //흑
	//		else             //백
		}
	}
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	
	  
	nBlockCount=(rect.right-rect.left)/nBlockSize+1;
	if(nBlockCount>10) nBlockCount=10;
	else if(nBlockCount<0)  nBlockCount=0;

	//각각의 Block에서 밝은것 2개,어두운것 2개 구한다.------------------------------------------------------
	g_Temp.m_nMinGray=255;
	g_Temp.m_nMaxGray=0;
	for(k=0;k<nBlockCount;k++)
	{
		nX1=rect.left+k*nBlockSize;	   if(nX1>=rect.right) break;
		nX2=nX1+nBlockSize;            if(nX2>rect.right) nX2=rect.right;
		if(nX1<0) nX1=0;  if(nX1>=WIDTH) nX1=WIDTH-1;
		if(nX2<0) nX2=0;  if(nX2>=WIDTH) nX2=WIDTH-1;
		
		nCandiW[k][2]=0;
		nCandiB[k][2]=600;
		for(i=0;i<g_System.m_nImageH;i++)
		{
			for(j=nX1;j<nX2-1;j++)
			{
				nTmp=*(fm+WIDTH*i+j)+*(fm+WIDTH*i+j+1);  //Edge에서 튀는 것을 방지하기 위해서 
				if(nTmp>nCandiW[k][2])
				{
					nCandiW[k][2]=nTmp;		//Max
					nCandiW[k][0]=j;		//X
					nCandiW[k][1]=i;		//Y	
				}
				else if(nTmp<nCandiB[k][2])
				{
					nCandiB[k][2]=nTmp;		//Min
					nCandiB[k][0]=j;		//X
					nCandiB[k][1]=i;		//Y	
				}
			}
		}
		if(nCandiB[k][2]/2<g_Temp.m_nMinGray) g_Temp.m_nMinGray=nCandiB[k][2]/2;
		if(nCandiW[k][2]/2>g_Temp.m_nMaxGray) g_Temp.m_nMaxGray=nCandiW[k][2]/2;
	}
	g_Temp.m_nMaxGray-=g_System.m_nFlatBright; if(g_Temp.m_nMaxGray<0) g_Temp.m_nMaxGray=0;
	g_Temp.m_nMinGray-=g_System.m_nFlatBright; if(g_Temp.m_nMinGray>0) g_Temp.m_nMinGray=0;
	//-------------------------------------------------------------------------------------------------------


	//첫번째 검정색 후보1개, 흰색후보 1개를 구한다.--------------------------
	nKCount=1;
	nNGGroup=0;
	nCandiB1[0][2]=200;
	nCandiW1[0][2]=200;
	for(k=0;k<nBlockCount;k++)
	{
		if(nCandiW[k][2]>=nCandiW1[0][2])
		{
			nCandiW1[0][2]=nCandiW[k][2];
			nCandiW1[0][0]=nCandiW[k][0];
			nCandiW1[0][1]=nCandiW[k][1];
			posW1=k;
		}
		if(nCandiB[k][2]<=nCandiB1[0][2])
		{
			nCandiB1[0][2]=nCandiB[k][2];
			nCandiB1[0][0]=nCandiB[k][0];
			nCandiB1[0][1]=nCandiB[k][1];
			posB1=k;
		}
	}
	//-----------------------------------------------------------------------
	//두번째 검정색 후보1개, 흰색후보 1개를 구한다.--------------------------
	if(nBlockCount>1)
	{
		nKCount=2;
		nCandiB1[1][2]=200;
		nCandiW1[1][2]=200;
		for(k=0;k<nBlockCount;k++)
		{
			if(posW1!=k && nCandiW[k][2]>=nCandiW1[1][2])
			{
				nCandiW1[1][2]=nCandiW[k][2];
				nCandiW1[1][0]=nCandiW[k][0];
				nCandiW1[1][1]=nCandiW[k][1];
			}
			if(posB1!=k && nCandiB[k][2]<=nCandiB1[1][2])
			{
				nCandiB1[1][2]=nCandiB[k][2];
				nCandiB1[1][0]=nCandiB[k][0];
				nCandiB1[1][1]=nCandiB[k][1];
			}
		}
	}
	//---------------------------------------------------------------------
	for(k=0;k<2*nKCount;k++)
	{	
		if(k==0)
		{
			gray=100-nCandiB1[0][2]/2;
			if(gray<nMinThDn) continue;
			nCx=nCandiB1[0][0];
			nCy=nCandiB1[0][1];
		}
		else if(k==1)
		{
			gray=100-nCandiB1[1][2]/2;
			if(gray<nMinThDn) continue;
			nCx=nCandiB1[1][0];
			nCy=nCandiB1[1][1];
		}
		else if(k==2)
		{
			gray=nCandiW1[0][2]/2-100;
			if(gray<nMinThUp) continue;
			nCx=nCandiW1[0][0];
			nCy=nCandiW1[0][1];
		}
		else if(k==3)
		{
			gray=nCandiW1[1][2]/2-100;
			if(gray<nMinThUp) continue;
			nCx=nCandiW1[1][0];
			nCy=nCandiW1[1][1];
		}
		
		//강불량과 겹치는 불량이 있으면 무시한다.-----------------------------------------------------------
		nOverlap=0;
		for(i=0;i<g_Defect.m_nDefectCount;i++)
		{
			if(g_Defect.m_Defect[i].defect_class%10==1 || g_Defect.m_Defect[i].defect_class%10==2)	//백, 흑
			{
				if(abs(nCx-g_Defect.m_Defect[i].x_pos) < dLimitDist) nOverlap=1;
			}
		}
		if(nOverlap) 
			continue;
		//---------------------------------------------------------------------------------------------------

		
		//세부 검사영역 범위를 다시 구한다.-----------------------------------------
		left=nCx-BAD_IMG_WIDTH/2;
		right=left+BAD_IMG_WIDTH;
		if(left<0)		{ left=0;		right=left+BAD_IMG_WIDTH;}
		if(right>=WIDTH){ right=WIDTH-1; left=right-BAD_IMG_WIDTH;}

		top = nCy-BAD_IMG_WIDTH/2;
		bottom=top+BAD_IMG_WIDTH;
		if( top<0)			{top=0;bottom=top+BAD_IMG_WIDTH;}
		if(bottom>=g_System.m_nImageH) {bottom=g_System.m_nImageH-1; top=bottom-BAD_IMG_WIDTH;}
		if(top<g_System.m_nImagePartH && bottom>g_System.m_nImagePartH)
		{
			if(g_System.m_nImagePartH-top>bottom-g_System.m_nImagePartH)
			{
				bottom=g_System.m_nImagePartH-1; top=bottom-BAD_IMG_WIDTH;
			}
			else
			{
				top=g_System.m_nImagePartH; bottom=top+BAD_IMG_WIDTH;
			}
		}
		//----------------------------------------------------------------------------

		if( CheckVerticalSlope(fm, left, top, right, bottom, WIDTH,nOverkillSlope,1)==0) continue; //(i)-(i+4)에서 10이상 차이나는게 3개 이상있으면 TRUE 아니면 Defect아님 	

		//Binary----------------------------------------------------------------------
		Binary3DFlatImage(fm,g_fmTemp, g_Param.m_nAreaThUp, g_Param.m_nAreaThDn, left, top, right, bottom, WIDTH);

		//Close (g_fmTemp1는 Temperary로 사용)
		Close3DFlatImage(g_fmTemp, g_fmTemp1, left, top, right, bottom, WIDTH);

		nBlob=FindDefectBlob(fm, g_fmTemp, 0, left, top, right, bottom, WIDTH, pCx, pCy, pSizeX, pSizeY, pSize, nGray, nType, nStrongCount, g_Param.m_dGroupSize/g_Param.m_dScaleFactorX, 10, nMaxXDist,nStrongValue);

		if(nBlob<=0) continue;
		//강불량 먼저 체크 하고 -----------------------------------------------------------------------------------
		for(i=0;i<1;i++)
		{
			pSizeX[i] *= g_Param.m_dScaleFactorX;
			pSizeY[i] *= g_Param.m_dScaleFactorX;
			dSize=(pSizeX[i]+pSizeY[i])/2;

			if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) break;

			if(pSizeX[i]<=0 || pSizeY[i]<=0) continue;

			nIsBlack=CheckIsBlack(fm, nCx, nCy, rect.left, rect.right, &nGrayValue);			
					
			if(nIsBlack)
			{
				nLevel=GetLevel(nGrayValue, dSize, g_Param.m_nValueDn, g_Param.m_dSizeDn);
				nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_SPOT_BLACK;
			}
			else
			{
				nLevel=GetLevel(nGrayValue, dSize, g_Param.m_nValueUp, g_Param.m_dSizeUp);	
				nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_SPOT_WHITE;
			}

			//군집성불량이면 무조건 강불량이다---------------------
			if(nLevel>1 && nBlob>=g_Param.m_nGroupCount && g_Param.m_nGroupCount>0)
			{
				dSize=pSizeX[i]=pSizeY[i]=1.90+0.001*nBlob;
				nNGGroup=1;
				nLevel=1;
			}

			if(nLevel<0 || nLevel>10) continue;
			//-----------------------------------------------------
			MakeDefectRect(g_Defect.m_nDefectCount, nCx, nCy, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			CopyNGImageFM(fm, WIDTH);

			//FISHEYE>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			//광학계 1군에서만, Fisheye제거가 세팅되어 있을때 Size는 1.5보다 작고, 강불량일때 
			if(g_Param.m_nRemoveFishEye && dSize<1.5 && nNGGroup==0 )
			{
				fmTT=g_Defect.m_pImage[g_Defect.m_nDefectCount];
				for(ii=0;ii<BAD_IMG_HEIGHT;ii++)
					for(jj=0;jj<BAD_IMG_WIDTH;jj++)
						*(l_fmT+BAD_IMG_WIDTH*ii+jj)=*(fmTT+BAD_IMG_WIDTH*(BAD_IMG_HEIGHT-ii-1)+jj);
				if(CheckFishEye(l_fmT, 0,0,127,127,128)) 
				{
					nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_FISHEYE;
				}
			}
			//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

			//HTO SIZE>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			IsNBOCloud=0;				
			if(g_Param.m_nUSEHTO && nNGGroup==0 )
			{															
				IsNBOCloud=0;
				fmTT=g_Defect.m_pImage[g_Defect.m_nDefectCount];
				GetHTOSize(fmTT, 0,0,127,127, 128, g_System.m_nFlatBright, 30,  5.0, &IsWhiteSpot, &Cx, &Cy, &pValue, &CoreSize, &dCloudValue, &CloudSize);

				if(IsWhiteSpot)
				{
					if(dCloudValue>=g_Param.m_dHTOCloudW)
					{
						IsNBOCloud=1;
						dNBOCloudValue=dCloudValue;
						dNBOCloudSize=CloudSize*g_Param.m_dScaleFactorX;									
					}
				}
				else 
				{
					if(dCloudValue>=g_Param.m_dHTOCloud)
					{
						IsNBOCloud=1;
						dNBOCloudValue=dCloudValue*-1;
						dNBOCloudSize=CloudSize*g_Param.m_dScaleFactorX;							
					}
				}								
				
				if(IsNBOCloud)
				{
					nLevel_=GetLevel(dNBOCloudSize, g_Param.m_dHTOCloudSize);
					nClass_=nLevel_*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_MURA;

					IsNBOCloud=0;
					if(g_Mark.nMarking[NG_LINE-1][nLevel_-1]==g_Mark.nMarking[nClass_%10-1][nLevel-1])
					{
						if(nLevel_<nLevel) IsNBOCloud=1; //같은 마킹, 같은 미마킹의 경우 Level이 높은 것 선택
					}
					else if(g_Mark.nMarking[NG_LINE-1][nLevel_-1])
					{
						IsNBOCloud=1;	//HTO만 마킹이면 마킹되는 것 선택
					}
					if(IsNBOCloud)
					{
						g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nCx;
						g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nCy;
						g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass_;
						g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =(int)dNBOCloudValue; //Value	
						g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =dNBOCloudSize; //Size
						g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =dNBOCloudSize; //Size X
						g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =dNBOCloudSize; //Size Y

						if(CheckDefectOverlap()==0 && IsRealDefect(fm, nCx, nCy, pSizeX[i], pSizeY[i], nOverkillSlope)) 
							g_Defect.m_nDefectCount++;
					}
				}
			}
			//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			
			if(IsNBOCloud==0)
			{
				g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nCx;
				g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nCy;
				g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;
				g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nGrayValue; //Value	
				g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =dSize; //Size
				g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =pSizeX[i]; //Size X
				g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =pSizeY[i]; //Size Y

				if(CheckDefectOverlap()==0 && IsRealDefect(fm, nCx, nCy, pSizeX[i], pSizeY[i], nOverkillSlope)) 
					g_Defect.m_nDefectCount++;
			}					
		}
		//---------------------------------------------------------------------------------------------------------------------------------
	}

	if(g_Temp.m_nLeading)  g_Defect.m_nDefectCount=0; //20161017	
}

//0이면 불량아님
int IsRealDefect(LPBYTE fm, int nX, int nY, double dSizeX, double dSizeY, int nOverkillSlope)
{
	int IsDefect;
	int nSkipX, nSkipY;
	int nCheckCount;

	IsDefect=CheckDefect_QWP(fm, nX, nY, g_System.m_nImagePartH);
	if(IsDefect)
	{
		nSkipX=dSizeX/g_Param.m_dScaleFactorX;
		nSkipY=dSizeY/g_Param.m_dScaleFactorY;

			
		if(dSizeX>1.9)
		{
			nCheckCount=3;
			nSkipX=50;
			nSkipY=50;
		}
		else                       
		{
			nCheckCount=1;
		}

		if(nSkipX<10) nSkipX=10;
		if(nSkipY<10) nSkipY=10;
		if(CheckVerticalSlope1(fm, nX, nY, nSkipX, nSkipY, WIDTH, nOverkillSlope,1)==0) IsDefect=0; //(i)-(i+4)에서 10이상 차이나는게 3개 이상있으면 TRUE 아니면 Defect아님 
	}

	return IsDefect;
}


//Binary영상을 3x1 CLOSE(DILATE+ERODE) 한다.
void Close3DFlatImage(LPBYTE fmBinary, LPBYTE fmT, int left, int top, int right, int bottom, int pitch)
{
	int i,j,k;
	int nTmp;
	int n3X255=3*255;

	//X 확장
	for(i=top;i<bottom;i++)
		for(j=left+1;j<right-1;j++)
		{
			nTmp=*(fmBinary+pitch*i+j-1)+*(fmBinary+pitch*i+j)+*(fmBinary+pitch*i+j+1);
			if(nTmp>0) *(fmT+i*pitch+j)=255;
			else       *(fmT+i*pitch+j)=0;
		}

	//X 축소 
	for(i=top;i<bottom;i++)
		for(j=left+1;j<right-1;j++)
		{
			nTmp=*(fmT+pitch*i+j-1)+*(fmT+pitch*i+j)+*(fmT+pitch*i+j+1);
			if(nTmp<n3X255) *(fmBinary+i*pitch+j)=0;
			else			*(fmBinary+i*pitch+j)=255;
		}
}
//평균밝기 100인 이미지를 nThUp, nThDn값으로 Binary한다. 
void Binary3DFlatImage(LPBYTE fm, LPBYTE fmBinary, int nThUp, int nThDn, int left, int top, int right, int bottom, int pitch)
{
	int i,j;
	int nTmp;

	nThUp=100+nThUp;
	nThDn=100-nThDn;
	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
		{
			nTmp=*(fm+pitch*i+j);
			if(nTmp>nThUp || nTmp<nThDn) 
				*(fmBinary+i*pitch+j)=255;
			else						 
				*(fmBinary+i*pitch+j)=0;
		}
}

int CheckIsBlack(LPBYTE fm, int nCx, int nCy, int nX1, int nX2, int *pValue)
{
	int i, j, left, top, right, bottom, nTmp;
	int nBlack, nMax=0, nMin=255;
		int HEIGHT=g_System.m_nImagePartH;
	int NUM_BUFFERS=g_System.m_nImagePart;

	left=nCx-64; if(left<nX1) left=nX1;
	right=nCx+64; if(right>nX2) right=nX2;
	top=nCy-64;   if(top<0)     top=0;
	bottom=nCy+64; if(bottom>NUM_BUFFERS*HEIGHT) bottom=NUM_BUFFERS*HEIGHT;
	
	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
		{
			nTmp=*(fm+WIDTH*i+j);
			if(nTmp>nMax) nMax=nTmp;
			if(nTmp<nMin) nMin=nTmp;
		}

	if( (nMax-100)>(100-nMin)) 
	{
		nBlack=0;
		*pValue=(nMax-100);
	}
	else
	{
		nBlack=1;
		*pValue=(100-nMin);
	}

	return nBlack;
}
int CheckIsBlackQWP(LPBYTE fm, int nCx, int nCy, int nX1, int nX2)
{
	int i, j, left, top, right, bottom, nTmp;
	int nBlack, nCount=0;
	long nAvg=0;
	int HEIGHT=g_System.m_nImagePartH;
	int NUM_BUFFERS=g_System.m_nImagePart;

	left=nCx-16; if(left<nX1) left=nX1;
	right=nCx+16; if(right>nX2) right=nX2;
	top=nCy-16;   if(top<0)     top=0;
	bottom=nCy+16; if(bottom>NUM_BUFFERS*HEIGHT) bottom=NUM_BUFFERS*HEIGHT;
	
	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
		{
			nAvg+=*(fm+WIDTH*i+j);
			nCount++;
		}
	
	if(nCount) nAvg/=nCount;
	
	if(nAvg>100) nBlack=0;
	else         nBlack=1;

	return nBlack;
}

//dSizeValue: Size보강 (위 - 아래 이값보다 크면 Size로 구함)
//Binary영상을 Blob해서 합칠건 합치고, 크기순으로 소팅한다.
//nType : 0(흑점) 1(백점)  2(흑선) 3(백선) 
//int CCalibration::FindDefectBlob(LPBYTE fmGray, LPBYTE fmBinary, int left, int top, int right, int bottom, int pitch, double *pCx, double *pCy, double *pSize, int *Gray, int *nType, int nMaxDefect)
int FindDefectBlob(LPBYTE fmGray, LPBYTE fmBinary, double dSizeValue, int left, int top, int right, int bottom, int pitch, double *pCx, double *pCy, double *pSizeX, double *pSizeY, double *pSize, int *Gray, int *nType, int *nStrongCount, double dCrowdSize, int nMaxDefect, int nMaxXDist, int nStrongValue)
{
	int i, j, k, nBlob=0;
	int N, nYS1, nYS2, nXS1, nXS2, nSum;
	double dAvg, dDiff;
	int nX1[50], nX2[50], nY1[50], nY2[50], nFlag[50], temp;
	int nNotUSE[50];
	int nSize, nSizeX, nSizeY,  nPos, nIsWhite, nXDist, nYDist;
	double dSizeMax, dWHRatio;
	int nMax[2]={0,0}, nMaxPos[2]={-1,-1};
	int nCrowdCount=0;


	
	g_Chain.SetChainData(1, fmBinary, 1, 1, 1, 5000, pitch, bottom);	
	N=g_Chain.FastChain(left, top, right, bottom);

	if(N>50) N=50; //최대 50개만 가지고 작업한다.
	else if(N<1) return 0;

	//제일 큰 OBJECT찾는다------------------------
	for(i=0;i<N;i++)
	{
		nNotUSE[i]=0;

		nX1[i]=g_Chain.FindMinX(i);
		nX2[i]=g_Chain.FindMaxX(i);
		nY1[i]=g_Chain.FindMinY(i);
		nY2[i]=g_Chain.FindMaxY(i);

		nSize=(nX2[i]-nX1[i])+(nY2[i]-nY1[i]);

		if(nSize>=nMax[0])
		{
			nMax[0]=nSize;
			nMaxPos[0]=i;
		}
		if(nSize*0.5>dCrowdSize) nCrowdCount++;
	}

	//두번째 큰 OBJECT찾는다------------------------
	for(i=0;i<N;i++)
	{
		if(nNotUSE[i] || i==nMaxPos[0]) continue;
		nSize=(nX2[i]-nX1[i])+(nY2[i]-nY1[i]);
		if(nSize>nMax[1])
		{
			nMax[1]=nSize;
			nMaxPos[1]=i;
		}
	}

	//일정 조건이 맞으면 2개를 합친다.--------------------------------------------------------------------------------------------------------------
	if(nMaxPos[0]>=0 && nMaxPos[1]>=0)
	{
		nSizeX=(nX2[nMaxPos[0]]-nX1[nMaxPos[0]]);
		nSizeY=(nY2[nMaxPos[0]]-nY1[nMaxPos[0]]);
		
		if(abs(nX2[nMaxPos[0]]-nX1[nMaxPos[1]])<abs(nX1[nMaxPos[0]]-nX2[nMaxPos[1]]))
			nXDist=abs(nX2[nMaxPos[0]]-nX1[nMaxPos[1]]);
		else
			nXDist=abs(nX1[nMaxPos[0]]-nX2[nMaxPos[1]]);

		if( (nY2[nMaxPos[1]]+nY1[nMaxPos[1]])*0.5 >=nY1[nMaxPos[0]] && (nY2[nMaxPos[1]]+nY1[nMaxPos[1]])*0.5 <= nY2[nMaxPos[0]] && nXDist<nMaxXDist )
		{
			if(nX1[nMaxPos[1]]<nX1[nMaxPos[0]]) nX1[nMaxPos[0]]=nX1[nMaxPos[1]];
			if(nX2[nMaxPos[1]]>nX2[nMaxPos[0]]) nX2[nMaxPos[0]]=nX2[nMaxPos[1]];
		}

		if(abs(nY2[nMaxPos[0]]-nY1[nMaxPos[1]])<abs(nY1[nMaxPos[0]]-nY2[nMaxPos[1]]))
			nYDist=abs(nY2[nMaxPos[0]]-nY1[nMaxPos[1]]);
		else
			nYDist=abs(nY1[nMaxPos[0]]-nY2[nMaxPos[1]]);

		if( (nX2[nMaxPos[1]]+nX1[nMaxPos[1]])*0.5 >=nX1[nMaxPos[0]]-5 && (nX2[nMaxPos[1]]+nX1[nMaxPos[1]])*0.5 <= nX2[nMaxPos[0]]+5 && nYDist<nMaxXDist )
		{
			if(nY1[nMaxPos[1]]<nY1[nMaxPos[0]]) nY1[nMaxPos[0]]=nY1[nMaxPos[1]];
			if(nY2[nMaxPos[1]]>nY2[nMaxPos[0]]) nY2[nMaxPos[0]]=nY2[nMaxPos[1]];
		}
	}
	//------------------------------------------------------------------------------------------------------------------------------------------------

	//불량 Type구하기 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	nBlob=0;
	for(k=nMaxPos[0];k<=nMaxPos[0];k++) //1개만 가지고 구한다.
	{ 
		nType[nBlob]=0;
		nPos=k;
		pCx[nBlob]=(nX1[nPos]+nX2[nPos])*0.5;
		pCy[nBlob]=(nY1[nPos]+nY2[nPos])*0.5;

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		if(dSizeValue>=3)
		{
			nYS1=nY1[nPos]-20; if(nYS1<top)		nYS1=top;
			nYS2=nY2[nPos]+20; if(nYS2>bottom)	nYS2=bottom;
			nXS1=nX1[nPos];
			nXS2=nX2[nPos];
			if(nXS2-nXS1<4)
			{
				nXS1--;	if(nXS1<left)  nXS1=left;
				nXS2++; if(nXS2>=right) nXS2=right-1;
			}
	

			//상단 Edge검출 -------------------------
			nSum=0;
			for(j=nXS1;j<=nXS2;j++)
				nSum+=*(fmGray+pitch*nYS1+j);
			dAvg=(double)nSum/(nXS2-nXS1+1);

			for(i=nYS1;i<nYS2;i++)
			{
				nSum=0;
				for(j=nXS1;j<=nXS2;j++)
					nSum+=*(fmGray+pitch*i+j);
				dDiff=(double)nSum/(nXS2-nXS1+1)-dAvg;
				if(dDiff<0) dDiff*=-1;
				if(dDiff>dSizeValue)
				{
					if(i<nY1[nPos]) nY1[nPos]=i;
					break;
				}
			}
			//----------------------------------------

			//하단 Edge검출 -------------------------
			nSum=0;
			for(j=nXS1;j<=nXS2;j++)
				nSum+=*(fmGray+pitch*nYS2+j);
			dAvg=(double)nSum/(nXS2-nXS1+1);

			for(i=nYS2;i>=nYS1;i--)
			{
				nSum=0;
				for(j=nXS1;j<=nXS2;j++)
					nSum+=*(fmGray+pitch*i+j);
				dDiff=(double)nSum/(nXS2-nXS1+1)-dAvg;
				if(dDiff<0) dDiff*=-1;
				if(dDiff>dSizeValue)
				{
					if(i>nY2[nPos]) nY2[nPos]=i;
					break;
				}
			}
			//----------------------------------------
		}
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


		//Type구하기 --------------------------------------------------------------------------------
		nSizeX=nX2[nPos]-nX1[nPos]+1; if(nSizeX<1) nSizeX=1;  //nType : 0(흑점) 1(백점)  2(흑선) 3(백선) 
		nSizeY=nY2[nPos]-nY1[nPos]+1; if(nSizeY<1) nSizeY=1;
		if(nSizeX<nSizeY) dWHRatio=(double)nSizeY/(double)nSizeX;
		else			  dWHRatio=(double)nSizeX/(double)nSizeY;
		if(dWHRatio>4)    nType[nBlob]+=2;
		
		nIsWhite=FindIsWhite(fmGray, nX1[nPos], nY1[nPos], nX2[nPos], nY2[nPos], top, bottom, pitch);
		nType[nBlob]+=nIsWhite;
		pSizeX[nBlob]=nSizeX;
		pSizeY[nBlob]=nSizeY;
		pSize[nBlob] =(nSizeX+nSizeY)/2.0;
		//--------------------------------------------------------------------------------------------

		Gray[nBlob]=FindGrayDiff(nIsWhite, fmGray, nX1[nPos], nY1[nPos], nX2[nPos], nY2[nPos], pitch);
		nStrongCount[nBlob]=FindStrongCount(nStrongValue, fmGray, nX1[nPos], nY1[nPos], nX2[nPos], nY2[nPos], pitch);

		nFlag[nPos]=1;
		nBlob++;
	}
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	if(nBlob>nCrowdCount) nCrowdCount=nBlob;

	return nCrowdCount;
}

 int CheckDefect_QWP(LPBYTE fm, int x, int y, int  nHHeight)
 {
	int nIsDefect=0;
	int nBlob;
	int nTmp;
	int nThreshold=15;

	int i,j,top, bottom;
	int left=x-64;
	int right=x+64;
	int XD, YD, nMAX=0;

	if(left<0)		 { left=0;        right=128;}
	if(right>=WIDTH) { right=WIDTH-1; left=right-128;}
	
	if(y>=nHHeight) y-=nHHeight;

	
	top=y-64; 
	if(top<0) { top=0; bottom=128;}
	else      
	{ 
		bottom=y+64;
		if(bottom>=nHHeight) { bottom=nHHeight-1; top=bottom-128; }
	}
	
	memset(l_fm,0,128*128);
	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
		{
			nTmp=*(fm+WIDTH*i+j)-*(fm+WIDTH*(i+nHHeight)+j);
			if(nTmp<-nThreshold || nTmp>nThreshold)
				*(l_fm+128*(i-top)+j-left)=255;
		}
	for(j=left;j<right;j++)
	{
		*(l_fm+128*40+j)=0;
		*(l_fm+128*80+j)=0;
	}

	g_Chain.SetChainData(1,l_fm,1,1,1,1000,128,128);
	nBlob=g_Chain.FastChain(0,0,127,127);

	if(nBlob) nIsDefect=1;
	
	return nIsDefect;
 }


 //수직선을 따라 내려가면서 튀는값이 nMinCount만큼 있으면 1을 Return 없으면 0을 Return 
int CheckVerticalSlope(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nSlope, int nMinCount)
{
	int nCount=0;
	int i, j, nTmp;
	int HEIGHT=g_System.m_nImagePartH;
	int NUM_BUFFERS=g_System.m_nImagePart;

	if(left<0) left=0;
	if(top<0)  top=0;
	if(right>=WIDTH-1) right=WIDTH-1;
	if(bottom>=NUM_BUFFERS*HEIGHT) bottom=NUM_BUFFERS*HEIGHT-1;

	for(j=left;j<right-1;j++)
	{
		for(i=top;i<bottom-4;i++)
		{
			nTmp=*(fm+pitch*i+j)-*(fm+pitch*(i+4)+j);
			if(nTmp<0) nTmp*=-1;

			if(nTmp>nSlope) 
			{
				nCount++;
				break;
			}
		}
		if(nCount>=nMinCount) 
			return 1;
	}

	return 0;	
}

 //수직선을 따라 내려가면서 튀는값이 nMinCount만큼 있으면 1을 Return 없으면 0을 Return 
int CheckVerticalSlope1(LPBYTE fm, int Cx, int Cy, int SizeX, int SizeY,  int pitch, int nSlope, int nMinCount)
{
	return 1;

	int nCount=0;
	int i, j, nTmp;
	int left, top, right, bottom;
	int HEIGHT=g_System.m_nImagePartH;
	int NUM_BUFFERS=g_System.m_nImagePart;

	left	=Cx-SizeX;
	right	=Cx+SizeX;

	if(Cy<HEIGHT)
	{
		top		=Cy-SizeY-5;
		bottom  =Cy+SizeY+5;
		if(bottom>=HEIGHT) bottom=HEIGHT-1;
	}
	else
	{
		top		=Cy-SizeY-5;
		if(top<HEIGHT) top=HEIGHT;
		bottom  =Cy+SizeY+5;
	}


	if(left<0) left=0;
	if(top<0)  top=0;
	if(right>=WIDTH-1) right=WIDTH-1;
	if(bottom>=NUM_BUFFERS*HEIGHT) bottom=NUM_BUFFERS*HEIGHT-1;

	for(j=left;j<right-1;j++)
	{
		for(i=top;i<bottom-4;i++)
		{
			nTmp=*(fm+pitch*i+j)-*(fm+pitch*(i+4)+j);
			if(nTmp<0) nTmp*=-1;

			if(nTmp>nSlope) 
			{
				nCount++;
				break;
			}
		}
	}
	if(nCount>=nMinCount) 
		return 1;
	else
		return 0;	
}

BYTE g_fmEmph[2048*640];
BYTE g_fmEmph1[2048*640];
long l_LRProfile[2048];
int FindLRLineNew_CROSS(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nMinValue, long *pProfile, int *pX, int *pY, int *pValue, int *pValueAdjust);
void WEB_FindDefectCrossLR()
{
	int i,j,nX, nY, nValue, nValueAdjust=0, nMaxValue=0, nMaxX=0, nMaxY=0;
	CRect rect;
	int nHeight;
	int nCount=0;
	int nLevel, nLevelInspect=0;
	int nThreshold=10000;
	int nClass;
	int pitch=g_System.m_nPitch;
	LPBYTE fm=g_fmGrab[g_ID];
	int nX1=g_Temp.m_nInspectX1;
	int nX2=g_Temp.m_nInspectX2;

	rect.left=nX1;
	rect.right=nX2;

	if(rect.right<rect.left) return; 
	if(rect.right-rect.left<BAD_IMG_WIDTH) return;

	if(rect.left<0) rect.left=0;
	else if(rect.left>=g_System.m_nImageW) rect.left=g_System.m_nImageW-1;
	if(rect.right<0) rect.right=0;
	else if(rect.right>=g_System.m_nImageW) rect.right=g_System.m_nImageW-1;

	if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) return;

	//검사 있는지 체크--------------
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i] && g_Param.m_dLineThreshold[i]>=100)
		{
			if(g_Param.m_dLineThreshold[i]<nThreshold) nThreshold=g_Param.m_dLineThreshold[i];
			nLevelInspect=1;
		}
	}
	if(nLevelInspect==0) return;
	//------------------------------

	//전체 영역의 반만으로 꼽침 찾느다.
	ThreadEmphasize(fm, g_fmEmph, g_System.m_nImageW, 0, g_System.m_nImagePartH, pitch, 50,5,10);

	nHeight=300;	
	memcpy(g_fmEmph1, g_fmEmph+pitch*10, pitch*(g_System.m_nImagePartH-20));
	
	//강조된 영상에서 영상 전체 프로파일 구함------
	//
	memset(	l_LRProfile, 0, g_System.m_nImageW*sizeof(long));
	for(i=10;i<nHeight;i+=2)
	{
		nCount++;
		for(j=0;j<g_System.m_nImageW;j++)
			l_LRProfile[j]+=*(g_fmEmph1+pitch*i+j);
	}
	for(j=0;j<g_System.m_nImageW;j++)
		l_LRProfile[j]/=nCount;
	//----------------------------------------------
	
	FindLRLineNew_CROSS(g_fmEmph1, rect.left+25, 0, rect.right-25, nHeight, pitch, 100, l_LRProfile, &nX, &nY, &nValue, &nValueAdjust);
	nValueAdjust/=4;


	if(	nValue>nMaxValue)
	{
	  nMaxValue=nValue;
	  nMaxX=nX;
	  nMaxY=g_System.m_nImagePartH;
	}
	nMaxValue/=4;

	g_Temp.m_nMaxLineValue	=  nMaxValue;
	g_Temp.m_nMaxLinePos	=  nMaxX;

	if(nMaxValue>nThreshold)
	{
		nX=nMaxX; 
		nY=g_System.m_nImagePartH/2;
		nLevel=GetLevel(nMaxValue, g_Param.m_dLineThreshold);
		nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_LINE;
		g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;

		if(nX-BAD_IMG_WIDTH/2-25<0) nX=BAD_IMG_WIDTH/2+25;
		if(nX+BAD_IMG_WIDTH/2+25>=g_System.m_nImageW) nX=g_System.m_nImageW-BAD_IMG_WIDTH/2-25;
		MakeDefectRect(g_Defect.m_nDefectCount, nX, nY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
		CopyNGImageFM(g_fmEmph1, pitch);

		g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nMaxX;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nY;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nMaxValue; //Value	
		g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0; //Size
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0; //Size X
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0; //Size Y

		if(CheckDefectOverlap()==0) 
				g_Defect.m_nDefectCount++;
	}
}


extern long l_nProfileSum[MAX_WIDTH];
extern long l_nProfileAvg[MAX_WIDTH];
extern long l_nProfileSumMoveinAvg[MAX_WIDTH];

int FindLRLineNew_CROSS(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nMinValue, long *pProfile, int *pX, int *pY, int *pValue, int *pValueAdjust)
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


	nMoveOffset=25;
	nValueOffset=6;


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

		//제일큰것 2개 없앰, 나머지로 평균값 구함---------
		SortData(16, nLocalMax);
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

			nMaxSum=100*100*((double)nMaxSum/((bottom-top)*(nValueOffset*2)*nAvgSum));
			nMaxSumOrg=100*100*((double)nMaxSumOrg/((bottom-top)*(nValueOffset*2)*nAvgSum));
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

//범위의 영역이 배경보다 밝으면 1, 어두우면 0 return
int FindIsWhite(LPBYTE fm, int left, int top, int right, int bottom, int Y1, int Y2, int pitch)
{
	int i, j;
	int nSum=0, nY1, nY2;
	double dAvg, dAvgBase;

	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
		{
			nSum+=*(fm+pitch*i+j);
		}
	dAvg=(double)nSum/((right-left)*(bottom-top));

	nSum=0;

	//비교할 영역 범위 구함 --------------------------
	//1) 바로 위 20 픽셀 
	//2) 바로 아래 20 픽셀
	//3) 전체 영역 
	nY1=top-20; if(nY1<Y1) nY1=Y1;
	nY2=top;
	if(nY2-nY1<10)
	{
		nY1=bottom;
		nY2=bottom+20; if(nY2>bottom) nY2=bottom;
		if(nY2-nY1<10)
		{
			nY1=Y1;
			nY2=Y2;
		}
	}
	//------------------------------------------------

	for(i=nY1;i<nY2;i++)
		for(j=left;j<right;j++)
		{
			nSum+=*(fm+pitch*i+j);
		}
	dAvgBase=(double)nSum/((right-left)*(nY2-nY1));

	if(dAvg>dAvgBase) return 1;
	else              return 0;
}
int FindGrayDiff(int nFindMax, LPBYTE fm, int left, int top, int right, int bottom, int pitch)
{
	int i, j;
	int nMax,nMin;
	double dAvg, dAvgBase;

	if(nFindMax)
	{
		nMax=100;
		for(i=top;i<=bottom;i++)
			for(j=left;j<=right;j++)
			{
				if(*(fm+pitch*i+j)>nMax) nMax=*(fm+pitch*i+j);
			}

		return (nMax-100);
	}
	else
	{
		nMin=100;
		for(i=top;i<=bottom;i++)
			for(j=left;j<=right;j++)
			{
				if(*(fm+pitch*i+j)<nMin) nMin=*(fm+pitch*i+j);
			}
		return (100-nMin);
	}
}
int FindStrongCount(int nStrongValue, LPBYTE fm, int left, int top, int right, int bottom, int pitch)
{
	int i, j;
	int nThUpStrong=100+nStrongValue;
	int nThDnStrong=100-nStrongValue;
	int nCount=0;

	for(i=top;i<=bottom;i++)
		for(j=left;j<=right;j++)
		{
			if(*(fm+pitch*i+j)>=nThUpStrong) nCount++;
			if(*(fm+pitch*i+j)<=nThDnStrong) nCount++;
		}
	return nCount;
}

//HTO관련

void quick_sort(BYTE list[], int left, int right);
int GetHTOSize(LPBYTE fm, int left, int top, int right, int bottom, int pitch, double dAvg, int nCoreTh, double dCloudUpTh, int *IsWhiteSpot, int *Cx, int *Cy, int *pValue, int *CoreSize,  double *dCloudValue, int *CloudSize)
{
	int ret=0;
	int i,j, loop, nTmp, nSum, nCnt, flagWhiteTry;
	int nULimit;	
	int nDLimit;
	int nUpTh, nCloudUpTh, nCloudDnTh;
	int ll,tt,rr,bb;
	int ll_,tt_,rr_,bb_;
	int nBlob, nMaxPos, nMaxPos1, nContour, nMaxContour, nMinValue=255, nMaxValue=0;
	int nBoundary;
	int nDataCount, *pixel;
	int nMax, nMinPos, nMinPosEnd, nMaxPosEnd, nLength, nPerimeter;
	double dArea, dMin, dMinEnd=0, dMax, dMax1, dMaxEnd, dBlackSize=0, dBlackValue=0, dWhiteSize=0, dWhiteValue=0;
	int nMaxDataCount=35;
	double dDataDn[50],dDataUp[50];
	double dEdgeX[50],  dEdgeY[50];
	BYTE nBoundData[1024];

	if(g_Param.m_dHTOCloudW<dCloudUpTh) dCloudUpTh=g_Param.m_dHTOCloudW;  //5보다작으면 그값을 사용하고 그렇지 않으면 5를 사용함.
	if(dCloudUpTh<3) dCloudUpTh=3;

	*Cx=*Cy=*pValue=*CoreSize=*dCloudValue=*CloudSize=*IsWhiteSpot=0;
	
	nUpTh=dAvg+nCoreTh;
	nULimit=dAvg+nCoreTh;
	nDLimit=dAvg-20;
	if(nCoreTh<45) nCoreTh=45;

	//Core 구하기-----------------------------------------------------------------------------
	for(i=1;i<128-1;i++)
		for(j=1;j<128-1;j++)
			*(g_fmDefectGray+pitch*i+j)=(*(fm+pitch*(i-1)+j-1)+*(fm+pitch*(i-1)+j)+*(fm+pitch*(i-1)+j+1)+*(fm+pitch*i+j-1)+*(fm+pitch*i+j)+*(fm+pitch*i+j+1)+*(fm+pitch*(i+1)+j-1)+*(fm+pitch*(i+1)+j)+*(fm+pitch*(i+1)+j+1)+4)/9;

	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
			if(*(g_fmDefectGray+pitch*i+j)>nUpTh)	*(g_fmDefectBinary+pitch*i+j)=255;
			else								    *(g_fmDefectBinary+pitch*i+j)=0;

	g_Chain.SetChainData(1, g_fmDefectBinary, 1, 1, 1, 2000, BAD_IMG_WIDTH, BAD_IMG_HEIGHT);
	nBlob=g_Chain.FastChain(left,top,right-1,bottom-1);
	nMaxPos=-1;
	nMaxContour=0;
	for(i=0;i<nBlob;i++)
	{
		dArea=g_Chain.Chain_Area(i);		
		if(dArea>nMaxContour)
		{
			nMaxContour=dArea;
			nMaxPos=i;
		}
	}
	if(nMaxPos>=0)
	{
		ll	= g_Chain.FindMinX(nMaxPos);
		rr	= g_Chain.FindMaxX(nMaxPos);
		tt	= g_Chain.FindMinY(nMaxPos);
		bb	= g_Chain.FindMaxY(nMaxPos);

		nMaxValue=0;
		for(i=tt;i<bb;i++)
			for(j=ll;j<rr;j++)
			{
				if(*(g_fmDefectGray+BAD_IMG_WIDTH*i+j)>nMaxValue) nMaxValue=*(g_fmDefectGray+pitch*i+j);
			}
		*pValue=nMaxValue-dAvg;

		*Cx=(ll+rr)/2+1;
		*Cy=(tt+bb)/2+1;
		if(rr-ll>bb-tt) *CoreSize=(rr-ll+1);
		else			*CoreSize=(bb-tt+1);

		if((rr-ll+1)<4 && (bb-tt+1)<4) return -2;
	}
	else
	{
		return -1;
	}

	//----------------------------------------------------------------------------------------------------
	nDataCount=0;
	for(loop=0;loop<nMaxDataCount;loop++)
	{
		memset(g_fmDefectBinary, 0, 128*128);
		nBoundary=g_Chain.GetChainDataNumber(nMaxPos);
		pixel=g_Chain.GetChainData(nMaxPos);
		
		for(i=0;i<nBoundary;i++)
		{
			*(g_fmDefectBinary+128*(pixel[2*i+1]-1)+(pixel[2*i]-1))	=255;
			*(g_fmDefectBinary+128*(pixel[2*i+1]-1)+pixel[2*i])		=255;
			*(g_fmDefectBinary+128*(pixel[2*i+1]-1)+pixel[2*i]+1)	=255;
			*(g_fmDefectBinary+128*pixel[2*i+1]+pixel[2*i]-1)		=255;
			*(g_fmDefectBinary+128*pixel[2*i+1]+pixel[2*i])			=255;
			*(g_fmDefectBinary+128*pixel[2*i+1]+pixel[2*i]+1)		=255;
			*(g_fmDefectBinary+128*(pixel[2*i+1]+1)+pixel[2*i]-1)	=255;
			*(g_fmDefectBinary+128*(pixel[2*i+1]+1)+pixel[2*i])		=255;
			*(g_fmDefectBinary+128*(pixel[2*i+1]+1)+pixel[2*i]+1)	=255;
		}

		ll_=ll-2-loop; if(ll_<1)   ll_=1;
		tt_=tt-2-loop; if(tt_<1)   tt_=1;
		rr_=rr+2+loop; if(rr_>125) rr_=125;
		bb_=bb+2+loop; if(bb_>125) bb_=125;

		nBlob=g_Chain.FastChain(ll_,tt_,rr_,bb_);
		nMax=0;
		for(i=0;i<nBlob;i++)
		{
			dArea=g_Chain.Chain_Area(i);
			if(dArea>nMax)
			{
				nMax=dArea;
				nMaxPos=i;
			}
		}
		if(nMax==0) 
			break;
		
		nPerimeter=g_Chain.GetChainDataNumber(nMaxPos);
		pixel=g_Chain.GetChainData(nMaxPos);
		if(nPerimeter>400) nPerimeter=400;

		nBoundary=0;
		for(i=0;i<nPerimeter;i++)
		{
			if(pixel[2*i]<2 || pixel[2*i]>125 || pixel[2*i+1]<2 || pixel[2*i+1]>125) continue;

			nTmp=*(g_fmDefectGray+128*pixel[2*i+1]+pixel[2*i]);
			if(nTmp<=nULimit)
				nBoundData[nBoundary++]=*(g_fmDefectGray+128*pixel[2*i+1]+pixel[2*i]);	
		}
		quick_sort(nBoundData, 0, nBoundary-1);

		//낮은것으로 구함---------------------
		nSum=nCnt=0;
		for(i=0;i<nBoundary/2;i++)
		{
			nSum+=nBoundData[i];
			nCnt++;
		}
		if(nCnt==0) break;
		dDataDn[nDataCount]=(double)nSum/nCnt;

		//높은것으로 구함---------------------
		nSum=nCnt=0;
		for(i=nBoundary/4;i<nBoundary*3/4;i++)
		{
			nSum+=nBoundData[i];
			nCnt++;
		}
		if(nCnt==0) break;
		dDataUp[nDataCount]=(double)nSum/nCnt;

		nDataCount++;
	}	
	if(nDataCount<10) return ret;
	//검은 얼룩 검출----------------------------------------
	//제일 낮은곳 
	dMin=255; nMinPos=-1;
	for(i=0;i<nDataCount;i++)
		if(dDataDn[i]<=dMin)
		{
			dMin=dDataDn[i];
			nMinPos=i;
		}

	if(nMinPos>0 && nMinPos<nDataCount-15)
	{
		dBlackSize=*CoreSize+2*nMinPos;	

		dMinEnd=nCnt=nMinPosEnd=0;
		for(i=nMinPos+10;i<nMinPos+20;i++)
		{
			if(i>=nDataCount) continue;
			dMinEnd+=dDataDn[i];
			nCnt++;
		}
		if(nCnt>=3)
		{
			dMinEnd/=nCnt;
			dBlackValue=dMinEnd-dMin;
		}
		else
		{
			nCnt=dMinEnd=0;
			for(i=nDataCount-10;i<nDataCount;i++)
				dMinEnd+=dDataDn[i];
			dMinEnd=dMinEnd*0.1;
		}

		for(i=nMinPos+1;i<nDataCount;i++)
			if(dDataDn[i]>(dMinEnd-1)) 
			{
				nMinPosEnd=i;
				break;
			}

		if(nMinPosEnd>0) 
			dBlackSize=*CoreSize+2*nMinPosEnd;	
	}

	//흰얼룩검출--------------------------------------------
	// 처음은 dDataUp를 dDataDn으로 사용했음.
	nMaxPosEnd=0;
	if(1)
	{
		dMinEnd=0;
		for(i=nDataCount-10;i<nDataCount;i++)  
			dMinEnd+=dDataUp[i];
		dMinEnd*=0.1;							//끝에 10개로 평균값 구함
	}
	for(i=nDataCount-1;i>3;i--)
	{
		if(dDataUp[i]>dMinEnd+dCloudUpTh)       //끝에서 부터 스캔해서 평균값+백설정값보다 커지는데가 있으면 STOP
		{
			nMaxPosEnd=i;
			break;
		}
	}
	if(nMaxPosEnd>3)	
	{
		dMaxEnd=nCnt=0;
		dMin=255;
		dMax=dMax1=0;
		for(i=1;i<=nMaxPosEnd;i++)
		{
			if(dDataUp[i]<=dMin)
				dMin=dDataUp[i];

			if(dDataUp[i]>dMin+1)
			{
				if(dDataUp[i]>dMax)			//백 Value구함
					dMax=dDataUp[i];
			}
		}
		if(dMax<1)  //못구했으면 4부터 가장 높은값을 선택
		{
			for(i=4;i<=nMaxPosEnd;i++)
				if(dDataUp[i]>dMax)
					dMax=dDataUp[i];
		}	

		if(dMax>0)
		{
			dWhiteValue=dMax-(dMinEnd+1);
			dWhiteSize=*CoreSize+2*(nMaxPosEnd+1);	
		}
	}

	if( (dBlackValue>=g_Param.m_dHTOCloud && dBlackSize>=dWhiteSize) || nMaxPosEnd<=3)
	{
		if(dBlackValue>=1.0 && dBlackValue>=g_Param.m_dHTOCloud && dBlackSize>0) 
		{
			*dCloudValue=dBlackValue;
			*CloudSize  =dBlackSize;
		}
	}
	else 
	{
		if(dWhiteValue>=1.0 && dWhiteValue>=g_Param.m_dHTOCloudW && dWhiteSize>0)
		{
			*IsWhiteSpot=1;
			*dCloudValue=dWhiteValue;
			*CloudSize  =dWhiteSize;
		}
	}


	return ret;
}




//입술불량 검사원리
//4개의 파라미터 사용
//판단값
//Size
//과검값(밝기차 Value가 이 값보다 크면 입술얼룩이 아니고 그냥 백점)
//Size구하기 위한 Stdev Th

int nStdev[200*40];
int GetStdevMapLIP(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, int nW, int nH, int *pStdev, int *nSW, int *nSH);
double GetStdevMapLIP(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, int nAvg, int nLimit);
int GetStdevMapLIP(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, int nW, int nH, int *pCount, int *pMap);

int GetStdevMapH(LPBYTE fm, int x1, int x2, int y1, int y1Offset, int pitch, int nW, int nH, double dThres, int *pX1, int *pX2);
int GetStdevMapV(LPBYTE fm, int y1, int y2, int x1, int y1Offset, int pitch, int nW, int nH, double dThres, int *pY1, int *pY2);
int FindLipOverkill(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, double dStdev);
int CheckOverKill3D(LPBYTE fm, int left, int top, int right, int bottom, int pitch, double dThreshold, double dThresholdH, int *pMax, int *pMaxH);
//입술불량 		 파라미터 받는것과 불량유형 구분하면됨.
void WEB_FindDefectSTDEV()
{
	int i,j, k;
	int nSizeX=24, nSizeY=20, nOffsetY=16;

	CRect rect;;
	LPBYTE fm = g_fmFlat[g_ID];

	int nVisionLine = g_Temp.m_nPCOptic;
	int nClass, nLevel, nX, nY;
	int nSW, nSH, nTmp;
	int nPosX[2], nPosY[2], nMaxValue[2];
	int x1,y1,x2,y2,MarkX1, MarkX2;
	double dStdveUp, dStdveDn, dStdev[2];
	double dSizeX, dSizeY, dSize;
	int nOverKill, y1Offset=0, nX1, nX2, nY1, nY2;
	int left,top, right, bottom, nCount,nSum, nMaxGray, nMinGray, nGrayDiff;
	int nMax, nMaxH;
	int nLipInspect=0;
	double dMinLipValue=100;

	rect.left=g_Temp.m_nInspectX1;
	rect.right=g_Temp.m_nInspectX2;
	rect.top=0;
	rect.bottom=g_System.m_nImageH;
	g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;

	if(rect.right-rect.left<100) goto done;

	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i] && g_Param.m_dLipStdevLimit[i]>0.001)
		{
			if(g_Param.m_dLipStdevLimit[i]<dMinLipValue) dMinLipValue=g_Param.m_dLipStdevLimit[i];
			nLipInspect=1;
		}
	}
	if(nLipInspect==0) goto done;

	memset(nStdev, 0, 200*40*sizeof(int));
	GetStdevMapLIP(fm, 0, 0, WIDTH, g_System.m_nImageH, WIDTH, nSizeX, nSizeY, nStdev, &nSW, &nSH);

	//영역을 크게 좌우 2개로 나누어서 입술불량 검사 
	for(k=0;k<2;k++)
	{
		if(k==0)
		{
			MarkX1=rect.left/(nSizeX/2);
			MarkX2=WIDTH/nSizeX;
		}
		else
		{
 			MarkX1=WIDTH/nSizeX;
			MarkX2=rect.right/(nSizeX/2)-1;
		}

		nMaxValue[k]=0;
		for(i=0;i<(nOffsetY-1);i++)
			for(j=MarkX1;j<MarkX2;j++)
			{

				  nTmp=	*(nStdev+ nSW*i+j) - *(nStdev+ nSW*(i+nOffsetY)+j);
				  if(nTmp<0) nTmp*=-1;
				  if(  nTmp> nMaxValue[k])
				  {
					  nMaxValue[k]=nTmp;
					  nPosX[k]=j;
					  nPosY[k]=i;
				  }
			}

		x1=	nPosX[k]*(nSizeX/2)-3;  if(x1< rect.left) x1=rect.left;
		x2= x1+30;					if(x2> rect.right) {x2=rect.right; x1=x2-30;}
		y1=	nPosY[k]*(nSizeY/2)-5;  if(y1<0) y1=0;
		y2= y1+30;					if(y2>g_System.m_nImagePartH) {y2=g_System.m_nImagePartH; x1=y2-30;}

		
		dStdveUp=GetStdevMapLIP(fm, 	x1,y1,x2,y2,WIDTH,100,20);
		dStdveDn=GetStdevMapLIP(fm, 	x1,y1+g_System.m_nImagePartH,x2,y2+g_System.m_nImagePartH,WIDTH,100,20);
		dStdev[k]= dStdveUp- dStdveDn;
		if(	dStdveUp>dStdveDn)
		{
			 dStdev[k]=	dStdveUp-dStdveDn;
			 nPosX[k]=nPosX[k]*(nSizeX/2)+(nSizeX/2);
			 nPosY[k]=nPosY[k]*(nSizeY/2)+5;
		}
		else
		{
			 dStdev[k]=	dStdveDn-dStdveUp;
			 nPosX[k]=nPosX[k]*(nSizeX/2)+(nSizeX/2);
			 nPosY[k]=nPosY[k]*(nSizeY/2)+5+g_System.m_nImagePartH;
		}

		if(	dStdev[k]> g_Temp.m_dMaxStdev) g_Temp.m_dMaxStdev=	dStdev[k];
	}

	for(k=0;k<2;k++)
	{
		if(dStdev[k]>=dMinLipValue)
		{
			if(g_Defect.m_nDefectCount<2)
			{									//m_nNewNotInspLeft~g_Temp.m_nNewNotInspRight	검사하지 않음
				if( nPosY[k]<g_System.m_nImagePartH  ) y1Offset=g_System.m_nImagePartH;
				else								   y1Offset=-g_System.m_nImagePartH;

				GetStdevMapH(fm, nPosX[k]-64, nPosX[k]+64, nPosY[k]-10, y1Offset, WIDTH, nSizeX, nSizeY, g_Param.m_dStdevSizeThres, &nX1, &nX2);
				GetStdevMapV(fm, nPosY[k]-64, nPosY[k]+64, nPosX[k]-10, y1Offset, WIDTH, nSizeX, nSizeY, g_Param.m_dStdevSizeThres, &nY1, &nY2);
				dSizeX=(nX2-nX1)*g_Param.m_dScaleFactorX;
				dSizeY=(nY2-nY1)*g_Param.m_dScaleFactorX;
				dSize=(dSizeX+dSizeY)/2;

				//-------------------------------------------------------------------------------------------------------------------
				//입술불량으로 잡은 영역에서 밝기값이 어둡거나 밝은게 있으면 과검 
				 nCount=nSum=0;
				 nMaxGray=0;
				 nMinGray=255;
				left=nPosX[k]-64; if(left<rect.left) left=rect.left;
				right=nPosX[k]+64; if(right>rect.right) right=rect.right;

				top	  =	 nPosY[k]-64;
				bottom=	 nPosY[k]+64;

				if(top<0) { top=0; bottom=top+128; }
				if(bottom>=g_System.m_nImageH) { bottom=g_System.m_nImageH-1; top=bottom-128; }
				if(top<g_System.m_nImagePartH && bottom>g_System.m_nImagePartH)
				{
					if((g_System.m_nImagePartH-top)>(bottom-g_System.m_nImagePartH))
					{
						bottom=g_System.m_nImagePartH;
						top=bottom-128;
					}
					else
					{
						top=g_System.m_nImagePartH;
						bottom=top+128;
					}
				}


				for(i=top;i<bottom;i++)
					for(j=left;j<right;j++)
					{
						nCount++;
						nSum+= *(fm+WIDTH*i+j);

						if(*(fm+WIDTH*i+j)>nMaxGray)   nMaxGray = *(fm+WIDTH*i+j);
						if(*(fm+WIDTH*i+j)<nMinGray)   nMinGray = *(fm+WIDTH*i+j);
					}

				 nOverKill=0;
				 if(nCount>0)
				 {

					if( nMaxGray-nSum/nCount > nSum/nCount-nMinGray)  nGrayDiff = nMaxGray-nSum/nCount ;
					else											  nGrayDiff = nSum/nCount-nMinGray ;

					if(nGrayDiff>99) nGrayDiff=99;
					
					if(nGrayDiff>g_Param.m_nGrayLimit)	 nOverKill=1;
				 }
				//------------------------------------------------------------------------------------------------------------------------

				if(g_Param.m_nGrayLimit<100)														//밝기차를 100이상하면 과검루틴 작동안함.
				{
					if(	dStdev[k]<1 && nGrayDiff>=15) 	 nOverKill=1;							//Stdev가 낮은데 15이상 차이나는 것은 입술불량이 아님.

					if(	dStdev[k]<4 && nOverKill==0 && rect.right-rect.left>BAD_IMG_WIDTH) //과검루틴 추가 
					{
						if(right-left<BAD_IMG_WIDTH) 
						{
							if(left ==rect.left) { right=left + BAD_IMG_WIDTH;}
							if(right==rect.right) { left=right - BAD_IMG_WIDTH;}				
						}
						if(bottom-top<BAD_IMG_WIDTH)
						{
							if(top==0)				{bottom=top + BAD_IMG_WIDTH;}
							if(bottom==	g_System.m_nImagePartH*2-1)	{top=bottom - BAD_IMG_WIDTH;}
						}
						nOverKill = FindLipOverkill(fm, left, top, right, bottom, WIDTH, dStdev[k]);
					}
				}

				if(!nOverKill &&  dSize>=g_Param.m_dLipMinSize)
					nOverKill=CheckOverKill3D(fm, left, top, right, bottom, WIDTH,0.7, 0.5, &nMax, &nMaxH);


				if(!nOverKill &&  dSize>=g_Param.m_dLipMinSize)
				{
					nX=nPosX[k];
					nY=nPosY[k];
					nLevel=GetLevel(dStdev[k], g_Param.m_dLipStdevLimit);
					if(nLevel<0 || nLevel>10) continue;

					nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_LIPSHAPE;
					MakeDefectRect(g_Defect.m_nDefectCount, nX, nY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
					CopyNGImageFM(fm, WIDTH);

					g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nX;
					g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nY;
					g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;
//					g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =((int)(dStdev[k]*1000))*100+nGrayDiff; //Value	
					g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =((int)(dStdev[k]*1000)); //Value	
					g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =dSize; //Size
					g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =dSizeX; //Size X
					g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =dSizeY; //Size Y

					if(CheckDefectOverlap()==0) 
							g_Defect.m_nDefectCount++;
				}
			}
		 }
	}		
		
	//---------------------------------------------------------------------------------------
	
	  

done:


	return;
}



//서치할때 nW의 반을 겹쳐서 하고 겹친부분은 한번만 계산함으로써 계산시간을 1/2로 줄임.
//(nW, nH)의 Stdev MAP을 구하는 함수
int GetStdevMapLIP(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, int nW, int nH, int *pStdev, int *nSW, int *nSH)
{
	int i,j, ii, jj;
	double stdev=0; 
	int nArea, tmp, nSum, nSum_, nSum2, nSum2_;
	int nX, nY, nSPitch=0;

	if(nW%2) nW--;
	if(nH%2) nH--;
	for(j=x1;j<=x2-nW;j+=nW/2) nSPitch++;

	nArea=nW*nH;
	
	nY=0;
	for(i=y1;i<=y2-nH;i+=nH/2, nY++)
	{	
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
				*(pStdev+nSPitch*nY+nX)=(int)(100*sqrt(((double)nArea*(nSum2+nSum2_)-(double)(nSum+nSum_)*(double)(nSum+nSum_))/((double)nArea*((double)nArea-1.))));	
			else
				*(pStdev+nSPitch*nY+nX)=0;

			nSum=nSum_;
			nSum2=nSum2_;
		}
	}

	*nSW=nX;
	*nSH=nY;
	
	return 0;
}


int GetStdevMapH(LPBYTE fm, int x1, int x2, int y1, int y1Offset, int pitch, int nW, int nH, double dThres, int *pX1, int *pX2)
{
	int i,j, ii, jj;
	double stdev=0; 
	int nArea, tmp, nSum, nSum_, nSum2, nSum2_;
	int nX, nY, nSPitch=0;
	double dStdev, dStdevBase;

	if(nW%2) nW--;
	if(nH%2) nH--;


	*pX1=*pX2=0;

	if(x1<0) x1=0;
	if(y1<0) y1=0;

	if(y1Offset>0)
	{
		if(y1<0)         y1=0;
		if(y1+nH>g_System.m_nImagePartH) y1=g_System.m_nImagePartH-nH;
	}
	else
	{
 		if(y1<g_System.m_nImagePartH) y1=g_System.m_nImagePartH;
		if(y1+nH>  2*g_System.m_nImagePartH) y1=2*g_System.m_nImagePartH-nH;
	}

	nArea=nW*nH;


	for(j=x1;j<x2-nW;j++)
	{
		nSum=nSum2=0;
		for(ii=y1;ii<y1+nH;ii++)
			for(jj=j;jj<j+nW;jj++)
			{
				tmp=*(fm+pitch*ii+jj);
				nSum+=tmp;
				nSum2+=(tmp*tmp);
			}


		nSum_=nSum2_=0;
		for(ii=y1+y1Offset;ii<y1+y1Offset+nH;ii++)
			for(jj=j;jj<j+nW;jj++)
			{
				tmp=*(fm+pitch*ii+jj);
				nSum_+=tmp;
				nSum2_+=(tmp*tmp);
			}

//		 dStdev=(int)(100*sqrt(((double)nArea*(nSum2)-(double)(nSum)*(double)(nSum))/((double)nArea*((double)nArea-1.))));	
//		 dStdevBase=(int)(100*sqrt(((double)nArea*(nSum2_)-(double)(nSum_)*(double)(nSum_))/((double)nArea*((double)nArea-1.))));	
		 dStdev=sqrt(((double)nArea*(nSum2)-(double)(nSum)*(double)(nSum))/((double)nArea*((double)nArea-1.)));	
		 dStdevBase=sqrt(((double)nArea*(nSum2_)-(double)(nSum_)*(double)(nSum_))/((double)nArea*((double)nArea-1.)));	

		 if(   dStdev-dStdevBase >	dThres)
		 {
			 if(*pX1==0) *pX1=j;
			 *pX2=j;
		 }
	}
				
	
	return 0;
}

int GetStdevMapV(LPBYTE fm, int y1, int y2, int x1, int y1Offset, int pitch, int nW, int nH, double dThres, int *pY1, int *pY2)
{
	int i,j, ii, jj;
	double stdev=0; 
	int nArea, tmp, nSum, nSum_, nSum2, nSum2_;
	int nX, nY, nSPitch=0;
	double dStdev, dStdevBase;

	if(nW%2) nW--;
	if(nH%2) nH--;


	*pY1=*pY2=0; 

	if(x1<0) x1=0;
	else if(x1+nW>WIDTH) x1=WIDTH-nW;

	if(y1Offset>0)
	{
		if(y1<0) y1=0;
		if(y2>g_System.m_nImagePartH)y2= g_System.m_nImagePartH;
	}
	else
	{
 		if(y1<g_System.m_nImagePartH)   y1=g_System.m_nImagePartH;
		if(y2>2*g_System.m_nImagePartH) y2=2*g_System.m_nImagePartH;

	}

	nArea=nW*nH;


	for(i=y1;i<y2-nH;i++)
	{
		nSum=nSum2=0;
		for(ii=i;ii<i+nH;ii++)
			for(jj=x1;jj<x1+nW;jj++)
			{
				tmp=*(fm+pitch*ii+jj);
				nSum+=tmp;
				nSum2+=(tmp*tmp);
			}


		nSum_=nSum2_=0;
		for(ii=i+y1Offset;ii<i+nH+y1Offset;ii++)
			for(jj=x1;jj<x1+nW;jj++)
			{
				tmp=*(fm+pitch*ii+jj);
				nSum_+=tmp;
				nSum2_+=(tmp*tmp);
			}

//		 dStdev=(int)(100*sqrt(((double)nArea*(nSum2)-(double)(nSum)*(double)(nSum))/((double)nArea*((double)nArea-1.))));	
//		 dStdevBase=(int)(100*sqrt(((double)nArea*(nSum2_)-(double)(nSum_)*(double)(nSum_))/((double)nArea*((double)nArea-1.))));	
		 dStdev=sqrt(((double)nArea*(nSum2)-(double)(nSum)*(double)(nSum))/((double)nArea*((double)nArea-1.)));	
		 dStdevBase=sqrt(((double)nArea*(nSum2_)-(double)(nSum_)*(double)(nSum_))/((double)nArea*((double)nArea-1.)));	

		 if(   dStdev-dStdevBase >	dThres)
		 {
			 if(*pY1==0) *pY1=i;
			 *pY2=i;
		 }
	}
				
	
	return 0;
}


double GetStdevMapLIP(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, int nAvg, int nLimit)
{
	int i,j;
	double stdev=0; 
	int nArea, tmp, nSum=0, nSum2=0;
	int nX, nY, nSPitch=0;
	int nCount=0;

	nArea=(x2-x1)*(y2-y1);
	
	nY=0;
	for(i=y1;i<y2;i++)
		for(j=x1;j<x2;j++)
		{
			tmp=nAvg-*(fm+pitch*i+j);
			if(tmp<	-nLimit)    
			{
				if(nCount<25)   tmp=0;
				else			tmp=-nLimit;
				nCount++;
			}
			else if(tmp>nLimit)
			{
				if(nCount<25)	tmp=0;
				else			tmp=nLimit;	
				nCount++;
			}
			
			nSum+=tmp;
			nSum2+=(tmp*tmp);
		}


	stdev=sqrt(((double)nArea*(nSum2)-(double)(nSum)*(double)(nSum))/((double)nArea*((double)nArea-1.)));	
		
	
	return stdev;
}


BYTE l_fmVerEdge[BAD_IMG_WIDTH*BAD_IMG_WIDTH];
int FindLipOverkill(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, double dStdev)
{
	int i, j;
	int nTemp;
	int nMaxPosI, nMaxPosJ, nMax=0;
	int left, top, right, bottom, nCount, nSum;
	int nThrehold=30;
	int nLimitCount=10;
	
	memset(l_fmVerEdge, 0, 	BAD_IMG_WIDTH*BAD_IMG_WIDTH);

	nSum=nCount=0;
	for(i=y1+1;i<y2-1;i++)
		for(j=x1+1;j<x2-1;j++)
		{
			nSum=*(fm+pitch*i+j); nCount++;
			nTemp=2*(*(fm+pitch*(i-1)+j-1)-*(fm+pitch*(i-1)+j+1)+
					 *(fm+pitch*(i)+j-1)  -*(fm+pitch*(i)+j+1)+
				     *(fm+pitch*(i+1)+j-1)-*(fm+pitch*(i+1)+j+1));
			if(nTemp<0) nTemp*=-1;
			if(nTemp>255) nTemp=255;

		   *(l_fmVerEdge+(i-y1-1)*BAD_IMG_WIDTH+(j-x1-1))=nTemp;
		}


	for(i=0;i<BAD_IMG_WIDTH-2;i++)
		for(j=0;j<BAD_IMG_WIDTH-2;j++)
		{
			if(*(l_fmVerEdge+BAD_IMG_WIDTH*i+j)>nMax)
			{
				nMax= *(l_fmVerEdge+BAD_IMG_WIDTH*i+j);
				nMaxPosI=i;
				nMaxPosJ=j;
			}
		}

	left=nMaxPosJ-15; if(left<0) left=0;
	right=left+30;  if(right>BAD_IMG_WIDTH-2) {right=BAD_IMG_WIDTH-2; left=right-30;}
	top=nMaxPosI-15;  if(top<0) top=0;
	bottom=top+30;  if(bottom>BAD_IMG_WIDTH-2) {bottom=BAD_IMG_WIDTH-2; top=bottom-30;}


	if(nCount && (nSum/nCount)>75)
		nThrehold+=	((nSum/nCount)-70)/5;

	nCount=0;
	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
		{
			if(*(l_fmVerEdge+BAD_IMG_WIDTH*i+j)>nThrehold)
			{
				nCount++;
			}
		}
		
	if(dStdev<1.0)		nLimitCount=10;
	else if(dStdev<2.0) nLimitCount=12;
	else if(dStdev<3.0) nLimitCount=14;
	else                nLimitCount=16;


	if(	nCount>=nLimitCount) return 1; //과검 
	else					 return 0; //과검아님 

}

int LineFitting3D(int dataN, double *x, double *y, int removeN, double ErrorLimit,double *t, double *a, double *b);
double FindStdevData(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch);
BYTE l_Dilate[128*128];
int CheckOverKill3D(LPBYTE fm, int left, int top, int right, int bottom, int pitch, double dThreshold, double dThresholdH, int *pMax, int *pMaxH)
{
	int i, j;
	int nDiff;
	int nCount, nMap[20];
	int nCountH, nMapH[20];
	double dX[20], dY[20], a, b, t;
	double dH, dHMax=0, dStdev;
	int nAvg, nSum=0;

	*pMax=*pMaxH=0;

	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
			nSum+=*(fm+pitch*i+j);
	nAvg=nSum/((right-left)*(bottom-top));

	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
		{
			if(nAvg-*(fm+pitch*i+j)>10)
				*(l_Dilate+128*(i-top)+(j-left))=nAvg-10;
			else 
				*(l_Dilate+128*(i-top)+(j-left))=*(fm+pitch*i+j);
		}

	dStdev=FindStdevData(l_Dilate, 0, 0, (right-left), (bottom-top), 128);
//	if(dStdev>5) return 1;


	for(i=top;i<bottom-32;i+=8)
	{
		nDiff=GetStdevMapLIP(fm, left, i, right, i+32, pitch, 24, 32, &nCount, nMap);
		if(nDiff>*pMax)
		{
			*pMax=nDiff;
			nCountH=nCount;
			for(j=0;j<nCountH;j++) nMapH[j]=nMap[j];
		}
	}

	if(*pMax>dThreshold*100)  //일단 1차 통과해야 하고
	{
		for(i=0;i<nCountH;i++)
		{
			dX[i]=i;
			dY[i]=nMapH[i];
		}
		LineFitting3D(nCountH, dX, dY, 0, 10, &t, &a, &b);
			
		for(i=0;i<nCountH;i++)
		{
			dH=a*i+b-dY[i]; if(dH<0) dH*=-1;
			if(dH>dHMax) dHMax=dH;
		}
		*pMaxH=dHMax;

		if(dHMax>dThresholdH*100)
			return 0; //불량
		else 
			return 1; //OverKill
	}
	else
		    return 1; //OverKill
}

int LineFitting3D(int dataN, double *x, double *y, int removeN, double ErrorLimit,double *t, double *a, double *b)
{
	int k,i,j, nResult=0;
	int nCount, MaxPos,*nValid;  
	double xvalue,yvalue;
	double Sxx, Sx, Sy,Sxy, SxSy, dd, MaxDist=0,dist;
	double ZeroData=0.00001;
	if(dataN<2) return -1;

	if(removeN>(dataN-2)) removeN=(dataN-2);

	nValid=(int*)malloc(dataN*sizeof(int));
	if(nValid==0) return -1;

	for(i=0;i<dataN;i++) nValid[i]=0;

	for(k=0;k<removeN+1;k++)
	{
		if(k==0)
		{
			Sx=Sy=Sxx=Sxy=SxSy=0.0;
			for(i=0;i<dataN;i++)
			{
				xvalue=*(x+i);
				yvalue=*(y+i);
				Sx+=xvalue;
				Sy+=yvalue;
				Sxx+=xvalue*xvalue;
				Sxy+=xvalue*yvalue;
			}
		}
		else
		{
			xvalue=*(x+MaxPos);
			yvalue=*(y+MaxPos);
			Sx-=xvalue;
			Sy-=yvalue;
			Sxx-=xvalue*xvalue;
			Sxy-=xvalue*yvalue;		
		}

		nCount=dataN-k;
		SxSy=Sx*Sy;

		dd=nCount*Sxx-Sx*Sx;
		if(fabs(dd)<ZeroData)
		{
			*t=0;
			*a=1;
			*b=-Sx/nCount;
			goto done;
		}
		else
		{
			*t=1;
			*a=(nCount*Sxy-Sx*Sy)/dd;
			*b=(-Sxy*Sx+Sy*Sxx)/dd;
		}

		dd=sqrt(*a**a+1);
		MaxDist=0;
		for(j=0;j<dataN;j++)
		{
			if(nValid[j]) continue;
			dist=fabs(-*(y+j)+*a**(x+j)+*b);
			if(dist>MaxDist)
			{
				MaxDist=dist;
				MaxPos=j;
			}
		}
		if(MaxDist/dd<ErrorLimit) goto done;
		else nValid[MaxPos]=1;
	}

done:
	if(nValid) free(nValid);
	return nResult;
}
//서치할때 nW의 반을 겹쳐서 하고 겹친부분은 한번만 계산함으로써 계산시간을 1/2로 줄임.
//(nW, nH)의 Stdev MAP을 구하는 함수
int GetStdevMapLIP(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, int nW, int nH, int *pCount, int *pMap)
{
	int i,j, ii, jj;
	double stdev=0; 
	int nArea, tmp, nSum, nSum_, nSum2, nSum2_;
	int nX, nY, nSPitch=0;
	int nMin=10000, nMax=0;

	if(nW%2) nW--;
	if(nH%2) nH--;
	for(j=x1;j<=x2-nW;j+=nW/2) nSPitch++;

	nArea=nW*nH;
	i=y1;

	nY=0;
	
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
	for(j=x1;j<=x2-nW;j+=nW/2)
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
			tmp=(int)(100*sqrt(((double)nArea*(nSum2+nSum2_)-(double)(nSum+nSum_)*(double)(nSum+nSum_))/((double)nArea*((double)nArea-1.))));	
			if(tmp>nMax) nMax=tmp;
			if(tmp<nMin) nMin=tmp;
			pMap[nX++]=tmp;
		}

		nSum=nSum_;
		nSum2=nSum2_;
	}
	*pCount=nX;
	
	return nMax-nMin;
}
double FindStdevData(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch)
{
	int i,j;
	long nSum=0;
	int tmp, nTop, nBottom;
	double dSum2=0, stdev=0; 
	long   nArea;

	nArea=(x2-x1)*(y2-y1);

	for(i=y1;i<y2;i++)
		for(j=x1;j<x2;j++)
		{
			tmp=*(fm+pitch*i+j);
			nSum+=tmp;
			dSum2+=(tmp*tmp);
		}

	if((nArea*dSum2-(double)nSum*(double)nSum)>0 && ((double)nArea*((double)nArea-1.))>0)
		stdev=sqrt((nArea*dSum2-(double)nSum*(double)nSum)/((double)nArea*((double)nArea-1.)));	

	return stdev;
}

//세로로 주어진 크기만큼 24x18영역 평균값을 40Pixel거리를 두고 비교함.
// fm : Flat 이미지 
BYTE l_fmL[WIDTH*640];	//Low, High Cut 이미지 +/- 20
BYTE l_fmMap[640/8][WIDTH/8];
int	FindLargeDefect(LPBYTE fm, int x1, int x2, int nHeight, int pitch, int nThreshold, int nSizeX, int nSizeY, int *MarkValue, int *nCx, int *nCy)
{
	int nOK=-1;
	int i, j, k, ii,jj;
	int nTmp, nInc=8;
	int nHighCut=120;
	int nLowCut=80;
	int nPosY;
	int nDivide=nSizeX*nSizeY;
	int nNx, nNy;
	int nT1, nT2;
	int nCompOffset=4;
	int nYS[2], nYE[2];

	nYS[0] = 0;
	nYE[0] = (g_System.m_nImagePartH-nSizeY)/nInc-nCompOffset;
	nYS[1] = g_System.m_nImagePartH/nInc;
	nYE[1] = (2*g_System.m_nImagePartH-nSizeY)/nInc-nCompOffset;

	*MarkValue=0;
	g_Temp.m_nMarkValueMax=0;

	for(i=0;i<nHeight; i++)
		for(j=0;j<WIDTH;j++)
		{
			nTmp=*(fm+pitch*i+j);
			if(nTmp>nHighCut)		*(l_fmL+pitch*i+j)=nHighCut;
			else if(nTmp<nLowCut)	*(l_fmL+pitch*i+j)=nLowCut;
			else					*(l_fmL+pitch*i+j)=nTmp;
		}

	nNy=0;
	for(i=0;i<=nHeight-nSizeY; i+=nInc)
	{
		nNx=0;
		for(j=x1;j<x2-nSizeX;j+=nInc)
		{
			nTmp=0;
			for(ii=i;ii<i+nSizeY;ii++)
				for(jj=j;jj<j+nSizeX;jj++)
					nTmp+=*(l_fmL+pitch*ii+jj);

			nTmp/=nDivide;		
			l_fmMap[nNy][nNx++]=nTmp;
		}
		nNy++;
	}

	if(nYE[1]+nCompOffset>nNy) nYE[1]=nNy-nCompOffset-1;

	for(k=0;k<2;k++)
	{
		for(j=0;j<nNx;j++)
		{
			for(i=nYS[k];i<=nYE[k];i++)
			{
				nTmp=l_fmMap[i][j]-l_fmMap[i+nCompOffset][j];
				if(nTmp<0) nTmp*=-1;

				if(nTmp>g_Temp.m_nMarkValueMax) 
					g_Temp.m_nMarkValueMax=nTmp;

				if(nTmp>=nThreshold && nTmp>*MarkValue)
				{
					nT1=100-l_fmMap[i][j];	if(nT1<0) nT1*=-1;
					nT2=100-l_fmMap[i+nCompOffset][j];if(nT2<0) nT2*=-1;
					if(	nT1>nT2) nPosY=i;
					else         nPosY=i+nCompOffset;
					
					//옆이랑 비교해서 밝기차가 nThreshold이상 나야 함 ----------------------
					nT1=nT2=0;
					if(j>=nCompOffset)
					{
						nT1=l_fmMap[nPosY][j]-l_fmMap[nPosY][j-nCompOffset];	if(nT1<0) nT1*=-1;
					}
					if(j<nNx-nCompOffset)
					{
						nT2=l_fmMap[nPosY][j]-l_fmMap[nPosY][j+nCompOffset];	if(nT2<0) nT2*=-1;
					}
					if(	nT1>=nThreshold*0.8 || nT2>=nThreshold*0.8)
					{
						*MarkValue=nTmp;
						*nCx=x1+j*nInc+nSizeX/2;
						*nCy=nPosY*nInc+nSizeY/2;
					}
					//----------------------------------------------------------------------
				}
			}
		}
	}

	return nOK;
}

#endif