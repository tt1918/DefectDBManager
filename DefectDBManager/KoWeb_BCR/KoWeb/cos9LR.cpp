#include "stdafx.h"
#include "math.h"

#ifdef USE_COS9_LR

long l_LRProfile[MAX_WIDTH];
extern long l_nProfileSum[MAX_WIDTH];
extern long l_nProfileAvg[MAX_WIDTH];
extern long l_nProfileSumMoveinAvg[MAX_WIDTH];

void WEB_CrossLR()
{
	int i,j,nX, nY, nValue, nValueAdjust=0, nMaxValue=0, nMaxValue1, nMaxValueW=0, nMaxX=0, nMaxY=0;
	CRect rect;
	LPBYTE fm = g_fmGrab[g_ID];
	int nWidth=g_System.m_nImageW;
	int nHeight=g_System.m_nImageH;
	int pitch=g_System.m_nPitch;
	int nCount=0;
	int nLevel=10, nLevelW,  dLineTh=0,nLevelInspect=0;

	g_Temp.m_nMaxLRValue=0;
	
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


	//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//검사범위 다시 구함.
	int nThreshold=50, leftV=0, rightV=0;
	for(i=0;i<nHeight;i++)
	{
		leftV+=*(fm+pitch*i+1);
		rightV+=*(fm+pitch*i+(pitch-2));
	}
	leftV/=nHeight;
	rightV/=nHeight;
	if(abs(leftV-rightV)>=30)
	{
		nThreshold=(leftV+rightV)/2;
		memset(	l_LRProfile, 0, pitch*sizeof(long));
		for(i=0;i<320;i++)
			for(j=0;j<pitch;j++)
				l_LRProfile[j]+=*(fm+pitch*i+j);

		for(j=0;j<pitch;j++)
			l_LRProfile[j]/=320;

		if(leftV>rightV)
		{
			for(j=0;j<pitch;j+=2)
				if(l_LRProfile[j]<nThreshold)
				{
					if(j-100<rect.right)
					{
						if(j-100>0)
						{
							rect.right=j-100;
						}
						else
						{
							rect.right=0;
						}
					}
					break;
				}
		}
		else
		{
			for(j=pitch-2;j>=0;j--)
				if(l_LRProfile[j]<nThreshold)
				{
					if(j+100>rect.left)
					{
						if(j+100<pitch)
						{
							rect.left=j+100;
						}
						else
						{
							rect.left=pitch-1;
						}
					}
					break;
				}
		}
	}
	if(rect.left>rect.right) rect.left=rect.right;

	memset(g_fmTemp, 0, 1392*320*8);
	ThreadEmphasize(fm, g_fmTemp, nWidth, 0, nHeight-1, pitch, 50,50,20);

	memset(	l_LRProfile, 0, pitch*sizeof(long));
	for(i=25;i<nHeight-25;i+=4)
	{
		nCount++;
		for(j=0;j<pitch;j++)
			l_LRProfile[j]+=*(g_fmTemp+pitch*i+j);
	}
	for(j=0;j<pitch;j++)
		l_LRProfile[j]/=nCount;
	//----------------------------------------------
	
	nMaxValue=0;
	for(i=0;i<2;i++)
	{		
		FindLRLineNew(g_fmTemp, rect.left+25, i*nHeight/2+25, rect.right-25, (i+1)*nHeight/2-25, pitch, 100, l_LRProfile, &nX, &nY, &nValue);
		if(	nValue>nMaxValue)
		{
			  nMaxValue=nValue;
			  nMaxX=nX;
			  nMaxY=nY;
		}
	}
	g_Temp.m_nMaxLRValue=  nMaxValue;
	g_Temp.m_nMaxLRPosX =  nMaxX;

	nLevel=GetLevel(nMaxValue, g_Param.m_dLineThreshold);

	nMaxValue1=0;
	if(nLevel>0)
	{
		for(i=0;i<8;i++)
		{					
			//8등분중에 가장쎈놈 찾음 
			FindLRLineNew(g_fmTemp, rect.left+25, i*g_System.m_nImagePartH+25, rect.right-25, (i+1)*g_System.m_nImagePartH-25, pitch, 100, l_LRProfile, &nX, &nY, &nValue);
			if(	nValue>nMaxValue1)
			{
				nMaxValue1=nValue;
				nMaxX=nX;
				nMaxY=nY;
			}
		}

		MakeDefectRect(g_Defect.m_nDefectCount, nMaxX, nMaxY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
		CopyNGImageFM(g_fmTemp, g_System.m_nPitch);
		g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_KKOPCHIM;
		g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nMaxX;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nMaxY;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nMaxValue; //Value	
		g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0; //Size
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0; //Size X
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0; //Size Y

		
		if(CheckDefectOverlap()==0)   							
			g_Defect.m_nDefectCount++;
	}
	if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) return;
	

}

int FindWhiteLine(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nWhiteWidth, int *pRx, int *pValue);
void WEB_CrossLR_White()
{
	int i,j,nX, nY, nValue,nMaxX, nMaxY;
	int nLevel, nLevelInspect=0;
	CRect rect;
	LPBYTE fm = g_fmGrab[g_ID];
	int NUM_BUFFERS=g_System.m_nImagePart;
	int WIDTH=g_System.m_nImageW;
	int HEIGHT=g_System.m_nImagePartH;
	int pitch=g_System.m_nPitch;
	int nMaxValue[2]={0,}, nMaxPosX[2]={0,},nMaxPosY[2]={0,};

	g_Temp.m_nMaxLRWValue=  0;


	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])	
		{			
			if(	g_Param.m_dLineThresholdW[i]>0)
			{
				nLevelInspect=1;
				break;
			}
		}
	}
	if(nLevelInspect==0) return;


	rect.left	=g_Temp.m_nInspectX1;
	rect.top	=0;
	rect.right	=g_Temp.m_nInspectX2;
	rect.bottom	=g_System.m_nImageH;

	if(rect.left<0) rect.left=0;
	else if(rect.left>=WIDTH) rect.left=WIDTH-1;
	if(rect.right<0) rect.right=0;
	else if(rect.right>=WIDTH) rect.right=WIDTH-1;

	//최대 2개만
	for(j=0;j<NUM_BUFFERS;j++)
	{
		i=4*(j%2)+(j/2)%4;

		FindWhiteLine(fm, rect.left, i*HEIGHT, rect.right, (i+1)*HEIGHT , pitch, 15, &nMaxX, &nValue);

		if(nValue>nMaxValue[0])
		{
			nMaxValue[1]=nMaxValue[0];
			nMaxPosX[1] =nMaxPosX[0];
			nMaxPosY[1] =nMaxPosY[0];

			nMaxValue[0]=nValue;
			nMaxPosX[0] =nMaxX;
			nMaxPosY[0] =i*HEIGHT+HEIGHT/2;
		}
		else if(nValue>nMaxValue[1])
		{
			nMaxValue[1]=nValue;
			nMaxPosX[1]=nMaxX;
			nMaxPosY[1]=i*HEIGHT+HEIGHT/2;
		}
	}

	g_Temp.m_nMaxLRWValue=  nMaxValue[0];;
	g_Temp.m_nMaxLRWPosX = nMaxPosX[0];


	for(i=0;i<2;i++)
	{
		if(nMaxValue[i]<10) continue;

		nValue=nMaxValue[i];
		nMaxX=nMaxPosX[i];
		nMaxY=nMaxPosY[i];

		nLevel=GetLevel(nValue, g_Param.m_dLineThresholdW);
		if(nLevel>0)
		{

			MakeDefectRect(g_Defect.m_nDefectCount, nMaxX, nMaxY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			CopyNGImageFM(fm, g_System.m_nPitch);
			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+5;
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nMaxX;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nMaxY;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nValue; //Value	
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0; //Size
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0; //Size X
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0; //Size Y

		
			if(CheckDefectOverlap()==0)   							
				g_Defect.m_nDefectCount++;
		}
	}
}


int FindLRLineNew(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nMinValue, long *pProfile, int *pX, int *pY, int *pValue)
{
	int i,ii, j, jj;
	int nLRFind=0;
	long nSum, nMaxSum, nMaxPos=0;
	int nMoveOffset=25;
	int nValueOffset=10;
	long nAvgSum=0;

	*pX=*pY=*pValue=0;
	if(right-left<150) return 0; //원래는 최소 120 [ Emphasize (25,25), Moveing Avg (35, 35) ]

	//Profile 구하기---------------------------------
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
	for(j=left;j<right;j++) l_nProfileSum[j]=0;

	for(i=top;i<bottom;i++)
	{
		for(j=left;j<right;j++)
		{
			if(*(fm+pitch*i+j)<l_nProfileAvg[j]-25)
			{
				l_nProfileSum[j]+=(l_nProfileAvg[j]-25);
			}
			else if(*(fm+pitch*i+j)>l_nProfileAvg[j]+25)
			{
				l_nProfileSum[j]+=(l_nProfileAvg[j]+25);
			}
			else
				l_nProfileSum[j]+=*(fm+pitch*i+j);
		}
	}
	//----------------------------------------------------

	//Moving Average (50) 구함----------------------------
	for(j=left+nMoveOffset;j<right-nMoveOffset;j++)
	{
		nSum=0;
		for(jj=j-nMoveOffset;jj<j-nValueOffset;jj++)
			nSum+=l_nProfileSum[jj];
		for(jj=j+nValueOffset+1;jj<=j+nMoveOffset;jj++)
			nSum+=l_nProfileSum[jj];
		
		l_nProfileSumMoveinAvg[j]=nSum/(nMoveOffset*2-nValueOffset*2);
	}
	//----------------------------------------------------


	//(nValueOffset*2=20)개 합------------------------------------------------------------------
	nMaxSum=0;
	for(j=left+nMoveOffset+nValueOffset;j<right-nMoveOffset-nValueOffset;j++)
	{
		nSum=0;
		for(jj=j-nValueOffset;jj<=j+nValueOffset;jj++)
			nSum+=(l_nProfileSumMoveinAvg[jj]-l_nProfileSum[jj]);
		
		if(nSum>nMaxSum)
		{
			nMaxSum=nSum;
			nMaxPos=j;
		}
	}
	//-------------------------------------------------------------------------

	if((bottom-top)==0 || nAvgSum==0) nMaxSum=0;
	else  
	{
		if(nAvgSum<80) nAvgSum=80;
		nMaxSum=100*nMaxSum/(bottom-top)*100/nAvgSum/20;
	}

	*pValue=nMaxSum;
	*pX=nMaxPos;
	*pY=(top+bottom)/2;

	if(nMaxSum>=nMinValue) nLRFind=1;

	return nLRFind;
}
int  l_ProfileLR[8][2048/4];
int  l_ProfileLR1[8][2048/4];
int FindWhiteLine(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nWhiteWidth, int *pRx, int *pValue)
{
	int i, j, nTmp, N=0, M=0;
	int pitch_=pitch/4;
	int *pPrifile,*pPrifile1;
	int nOffset=(nWhiteWidth/4)/2+2, nMax, nPosi=-1, nPosj=-1;
	int nX1, nX2, nMaxSum=0;
	int nProfData;
	int nAngleOffset=1;
	LPBYTE fmT;

	memset(l_ProfileLR,  0, 8*pitch_*sizeof(int));
	memset(l_ProfileLR1, 0, 8*pitch_*sizeof(int));

	// 1/4 압축-----------------------------------------------------------------------
	for(i=top;i<bottom-3;i+=4, N++)
	{
		fmT=fm+i*pitch;
		M=0;
		pPrifile=l_ProfileLR[N/10];
		for(j=left;j<right-3;j+=4, M++)
		{
			nTmp=*(fmT+j)+*(fmT+j+2)+*(fmT+pitch+pitch+j)+*(fmT+pitch+pitch+j+2);
			*(pPrifile+M)+=nTmp;
		}
	}	
	//--------------------------------------------------------------------------------

	for(i=0;i<8;i++)
	{
		pPrifile =l_ProfileLR[i];
		pPrifile1=l_ProfileLR1[i];
		for(j=nOffset;j<pitch_-nOffset-2;j++)
		{
			if( *(pPrifile+j)>*(pPrifile+j-nOffset)  && *(pPrifile+j)>*(pPrifile+j+nOffset) )
			{
				if(*(pPrifile+j-nOffset)>*(pPrifile+j+nOffset))
				{
					nTmp = *(pPrifile+j)-*(pPrifile+j-nOffset);
				}
				else
				{
					nTmp = *(pPrifile+j)-*(pPrifile+j+nOffset);
				}
				if(nTmp>100*4) nTmp=100*4;
				*(pPrifile1+j)= nTmp;
			}
		}
	}

	nMax=-1;
	for(i=0;i<8;i++)
	{
		pPrifile1=l_ProfileLR1[i];
		for(j=nOffset;j<pitch_-nOffset;j++)
		{
			if(*(pPrifile1+j)>nMax)
			{
				nMax=*(pPrifile1+j);
				nPosi=i;
				nPosj=j;
			}
		}
	}

	if(nMax>=0)
	{
		nX1=nPosj-nAngleOffset; if(nX1<0) nX1=0;
		nX2=nPosj+nAngleOffset; if(nX2>=pitch_) nX2=pitch_-1;
		for(i=0;i<8;i++)
		{
			nMax=0;
			pPrifile1=l_ProfileLR1[i];
			for(j=nX1;j<=nX2;j++)
			{
				if(*(pPrifile1+j)>nMax) nMax=*(pPrifile1+j);
			}
			nMaxSum+=nMax;
		}
		*pValue=nMaxSum*100/(8*4*10); //4는 Pixel 4개 더한것, 10은 10개로 한줄 만듦, 8은 8개 합쳤으므로
		*pRx=left+nPosj*4;
	}
	else
	{
		*pValue=*pRx=0;
	}

	return 0;
}


BYTE l_fmMuraImage[4*8*2048*320];
int  l_fmMuraCount=-1;
double LR_FindStdWithCutOff(LPBYTE fm, CRect rc, int x, int y, int pitch, int nSize);
void ImageFlatArea_MURA(LPBYTE fmS, LPBYTE fm, LPBYTE fmD, int nBase, int left, int right, int UpperCut, int nLowCut);

void WEB_LR_Mura()
{
	int i,j,ii,jj,k,m, nTmp, loop;
	CRect rect;
	LPBYTE fm = g_fmFlat[g_ID];
	LPBYTE fmDest;
	int ll,tt,rr,bb, nPitchDest;
	int nSize=32;

	double dTmp, dSizeMax;
	int  nExist, nLowExist;  //512x512영역안에 이미 불량이 있으면 뺀다.
	double dThUp, dThDn;
	int nMaxX[8], nMaxY[8], nMaxValue[8];
	int nMinX[8], nMinY[8], nMinValue[8];
	double dMinStd[5]={0,0,0,0,0}, dMaxStd[5]={0,0,0,0,0}, dHor=0, dVer=0;
	int nJump=0, nX, nY;
	int nLevelInspect=0;
	int nLevel, nExistNum;
	int NUM_BUFFERS=g_System.m_nImagePart;
	int WIDTH=g_System.m_nImageW;
	int HEIGHT=g_System.m_nImagePartH;
	int pitch=g_System.m_nPitch;
		

	g_Temp.m_nMuraMax=0;
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])	
		{			
			if(	g_Param.m_nBMura[i]>0)
			{
				nLevelInspect=1;
				break;
			}
		}
	}
	if(nLevelInspect==0) return;

	rect.left	=g_Temp.m_nInspectX1+1;
	rect.top	=0;
	rect.right	=g_Temp.m_nInspectX2;
	rect.bottom	=g_System.m_nImageH;

	//평활화-------------------------------------------------
	l_fmMuraCount++; if(l_fmMuraCount<0) return ;

	memcpy(l_fmMuraImage+(l_fmMuraCount%4)*NUM_BUFFERS*WIDTH*HEIGHT, fm, NUM_BUFFERS*WIDTH*HEIGHT);
	if(l_fmMuraCount<4) return;

	ImageFlatArea_MURA(l_fmMuraImage, fm, g_fmTemp, 100, rect.left, rect.right, 105, 95); 
				

	for(i=0;i<NUM_BUFFERS;i++)
	{
		FindMinMaxPos(g_fmTemp, rect.left, i*HEIGHT, rect.right,(i+1)*HEIGHT,pitch, nSize, nSize, &nMinX[i], &nMinY[i], &nMinValue[i], &nMaxX[i], &nMaxY[i], &nMaxValue[i] );
		
		if(i>0)
		{
			if(nMinValue[i]<nMinValue[0])
			{
				nMinValue[0]=nMinValue[i];
				nMinX[0]=nMinX[i];
				nMinY[0]=nMinY[i];
			}
			if(nMaxValue[i]>nMaxValue[0])
			{
				nMaxValue[0]=nMaxValue[i];
				nMaxX[0]=nMaxX[i];
				nMaxY[0]=nMaxY[i];
			}
		}
	}
	
	//왼쪽---------------------------------------------------------------------------------------------------------------------
	for(loop=0;loop<2;loop++)
	{
		if(loop==1)
		{
			nMinX[0]=nMaxX[0];
			nMinY[0]=nMaxY[0];
		}
		dMinStd[0]=-1;
		nY=nMinY[0]-20;
		nX=nMinX[0]-20;

		for(i=0;i<5;i++)
		{
			for(j=0;j<5;j++)
			{
				if((int)(nY+i*10)/HEIGHT==(int)((nY+20)/HEIGHT) && (int)(nY+i*10+48)/HEIGHT==(int)((nY+20)/HEIGHT) && nX+j*10>rect.left && nX+j*10+48<rect.right )
				{
					dTmp=10*LR_FindStdWithCutOff(g_fmTemp, rect, nX+j*10, nY+i*10, WIDTH, 48);
					if(dTmp>dMinStd[0])
					{
						dMinStd[0]=dTmp;
						nMinX[0]=nX+j*10;
						nMinY[0]=nY+i*10;
					}
				}
			}
		}
		nLevel=0;
		if(dMinStd[0]>100)
		{
			if(nMinY[0]-HEIGHT>0)					dMinStd[1]=10*LR_FindStdWithCutOff(g_fmTemp, rect, nMinX[0], nMinY[0]-HEIGHT, WIDTH, 48);
			if(nMinY[0]+HEIGHT<HEIGHT*NUM_BUFFERS)  dMinStd[2]=10*LR_FindStdWithCutOff(g_fmTemp, rect, nMinX[0], nMinY[0]+HEIGHT, WIDTH, 48);
			if(dMinStd[1]>0 && dMinStd[2]>0) dMinStd[0]=dMinStd[0]-(dMinStd[1]+dMinStd[2])*0.5;
			else if(dMinStd[1]>0)			 dMinStd[0]=dMinStd[0]-dMinStd[1];
			else if(dMinStd[2]>0)			 dMinStd[0]=dMinStd[0]-dMinStd[2];


			//g_Temp.m_nWhiteStdMax=dMaxStd[0];	//왼쪽
			if(dMinStd[0]<0) dMinStd[0]=0;
			g_Temp.m_nMuraMax=dMinStd[0];   //오른쪽 
	
			nLevel=GetLevel(dMinStd[0], g_Param.m_nBMura);
		}
		if(nLevel>0)
		{

			MakeDefectRect(g_Defect.m_nDefectCount, nMinX[0], nMinY[0], g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			CopyNGImageFM(fm, g_System.m_nPitch);
			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+4;
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nMinX[0];
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nMinY[0];
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   = (int)dMinStd[0];//Value	
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0; //Size
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0; //Size X
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0; //Size Y

		
			if(CheckDefectOverlap()==0)   							
				g_Defect.m_nDefectCount++;
		}
	}
}

int l_ProfileArea[2048];
int l_ProfileAreaRatio[2048];
int l_sumArea[2048*320];

void ImageFlatArea_MURA(LPBYTE fmS, LPBYTE fm, LPBYTE fmD, int nBase, int left, int right, int UpperCut, int nLowCut) 
{	
	unsigned char* fmImg = fm;// = pDoc->m_pImage->GetImagePtr();
	int nwidth = g_System.m_nImageW;	
	int i,j, k, nY, nOffset, nTmp;
	int nBrightSum, nMaxBright=0, nMaxPos;
	int nAverage=0;
	int nMaxProfile=0, nMaxProfilePos, nPosL, nPosR;
	int nCount=0;
	int NUM_BUFFERS=g_System.m_nImagePart;
	int WIDTH=g_System.m_nImageW;
	int HEIGHT=g_System.m_nImagePartH;

	memset(l_ProfileArea, 0, nwidth*sizeof(int));

	for(k=0;k<4*NUM_BUFFERS;k++)
	{
		nBrightSum=0;
		nOffset=k*HEIGHT;
		for(i=0;i<HEIGHT;i+=2)	
		{
			nY=nOffset+i;
			for(j=0;j<WIDTH;j++)
			{
				nBrightSum+=*(fmS+WIDTH*nY+j);
				l_ProfileArea[j]+=*(fmS+WIDTH*nY+j);
			}
		}

		if(nBrightSum>nMaxBright)
		{
			nMaxBright=nBrightSum;
			nMaxPos=k;
		}
	}

	//밝기 비율구한다--------------------------------------------------------------
	for(j=0;j<WIDTH;j++)
	{
		l_ProfileArea[j]/=(4*NUM_BUFFERS*HEIGHT/2);
	}
		
	nMaxProfilePos=(left+right)/2;

	if(left>50 || right<1392-50) nMaxProfilePos=WIDTH/2;
	else  
	{
		for(j=left;j<right;j++)
			if(l_ProfileArea[j]>nMaxProfile) 
			{
				nMaxProfile=l_ProfileArea[j];
				nMaxProfilePos=j;
			}
	}

	nPosL=nMaxProfilePos-50;if(nPosL<left) nPosL=left;
	nPosR=nPosL+100;        if(nPosR>right) nPosR=right;

	for(j=nPosL;j<nPosR;j++)
		nAverage+=l_ProfileArea[j];

	if(nPosR-nPosL>0) nAverage/=(nPosR-nPosL);
	else              nAverage=nBase;
	if(nAverage>110) nAverage=110;


	for(j=0;j<WIDTH;j++)
	{
		if(l_ProfileArea[j]>0)
		{
			nTmp=1000*nAverage/l_ProfileArea[j];
			if(nTmp>1000) l_ProfileAreaRatio[j]=1000+(nTmp-1000)*0.8;
			else  		  l_ProfileAreaRatio[j]=1000;
		}
		else  l_ProfileAreaRatio[j]=1000;

		if(l_ProfileAreaRatio[j]>1500)       l_ProfileAreaRatio[j]=1500; //최대 1.5배 증폭
		else if(l_ProfileAreaRatio[j]<1000)  l_ProfileAreaRatio[j]=1000; //줄이는건 없음
	}
	//----------------------------------------------------------------------------

	memset(l_sumArea, 0 , WIDTH*HEIGHT*sizeof(int));

	for(k=0;k<4*NUM_BUFFERS;k++)
	{
		if(abs(nMaxPos-k)<=1) continue; //제일밝은 Grab +/-1 줄 제외하고 평균구함.

		nCount++;
		nOffset=k*HEIGHT;

		for(i=0;i<HEIGHT;i++)	
		{
			nY=nOffset+i;
			for(j=0;j<WIDTH;j++)
				*(l_sumArea+WIDTH*i+j)+=*(fmS+WIDTH*nY+j);
		}
	}

	for(i=0;i<HEIGHT;i++)
		for(j=0;j<WIDTH;j++)
			*(l_sumArea+WIDTH*i+j)/=nCount;

	for(k=0;k<NUM_BUFFERS;k++)
	{
		nOffset=k*HEIGHT;
		for(i=0;i<HEIGHT;i++)	
		{
			nY=nOffset+i;
			for(j=0;j<WIDTH;j++)
			{
				nTmp=nBase+((*(fm+WIDTH*nY+j)-*(l_sumArea+WIDTH*i+j))*l_ProfileAreaRatio[j]+500)/1000;
				if(nTmp<nLowCut) nTmp=nLowCut;
				else if(nTmp>UpperCut) nTmp=UpperCut;
				*(fmD+WIDTH*nY+j)=nTmp;
			}
		}
	}
}
double LR_FindStdWithCutOff(LPBYTE fm, CRect rc, int x, int y, int pitch, int nSize)
{
	int i,j;
	long nSum=0;
	int tmp, nHighLimit, nLowLimit;
	double dSum2=0, stdev=0; 
	long   nArea;
	int x1, y1, x2, y2;
	int NUM_BUFFERS=g_System.m_nImagePart;
	int HEIGHT=g_System.m_nImagePartH;

	if(rc.right-rc.left<64) return 0;

	x1=x-nSize/2; if(x1<rc.left) x1=rc.left;
	x2=x1+nSize;  if(x2>rc.right) { x2=rc.right;x1=x2-nSize;}
	y1=y-nSize/2; if(y1<0) y1=0;
	y2=y1+nSize;  if(y2>NUM_BUFFERS*HEIGHT) { y2=NUM_BUFFERS*HEIGHT; y1=y2-nSize;}

	for(i=0;i<NUM_BUFFERS-1;i++)
	{
		if( y1>(i+1)*HEIGHT-nSize && y1<=(i+1)*HEIGHT-nSize/2)
		{
			y2=(i+1)*HEIGHT;
			y1=y2-nSize;
		}
		else if(y1>(i+1)*HEIGHT-nSize/2 && y1<=(i+1)*HEIGHT)
		{
			y1=(i+1)*HEIGHT;
			y2=y1+nSize;
		}
	}
	nSum=0;
	for(i=y1;i<y2;i++)
		for(j=x1;j<x2;j++)
		{
			nSum+=*(fm+pitch*i+j);
		}
	nArea=(x2-x1)*(y2-y1);

	//+/- 25이상 차이 제외하고 STD구함.
	nHighLimit=nSum/nArea+25;
	nLowLimit =nSum/nArea-25;

	nSum=0;
	for(i=y1;i<y2;i++)
		for(j=x1;j<x2;j++)
		{
			tmp=*(fm+pitch*i+j);
			if(tmp>nHighLimit)   
				tmp=nHighLimit;
			else if(tmp<nLowLimit) 
				tmp=nLowLimit;

			nSum+=tmp;
			dSum2+=(tmp*tmp);
		}

	if((nArea*dSum2-(double)nSum*(double)nSum)>0 && ((double)nArea*((double)nArea-1.))>0)
		stdev=100*sqrt((nArea*dSum2-(double)nSum*(double)nSum)/((double)nArea*((double)nArea-1.)));	

	return stdev;
}


#endif