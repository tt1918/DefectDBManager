#include "Stdafx.h"
#include "math.h"
#include "KoWebDoc.h"
#include "KoWebView.h"

#define MAX_SCRATCH 10


void WEB_Scratch()
{
	int i, j, nOverlap;
	int nLevel;
	int nClass;
	int posx[MAX_SCRATCH], posy[MAX_SCRATCH], value[MAX_SCRATCH];
	int nInspect=0, nScratchThres=10000;
	int AverageScratch=0, MaxScratch=0;
	int nScratch, nOverkill;
	int nWidth, nHeight, nPitch, nInspectX1, nInspectX2, nMulti=1;
	LPBYTE fmORG=g_fmGrab[g_ID];
	LPBYTE fm=g_fmGrab[g_ID];

	
	//검사하기위한 데이타가 들어 있는지 확인---------------
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])		
			if(g_Param.m_nScratch[i]<nScratchThres && g_Param.m_nScratch[i]>=100)
			{
				nScratchThres=g_Param.m_nScratch[i];
				nInspect=1;
			}
	}
	if(nInspect==0) return;
	g_Temp.m_nScratchInspect=1;
	//-----------------------------------------------------

	nWidth		=g_System.m_nImageW;
	nHeight		=g_System.m_nImageH;
	nPitch		=g_System.m_nPitch;
	nInspectX1	=g_Temp.m_nInspectX1;
	nInspectX2	=g_Temp.m_nInspectX2;

	if(g_System.m_nMakePyramid)  //8192이면 1/2영상에서 t
	{
		fm=g_fmGrabComp;
		nWidth	/=2;
		nHeight	/=2;
		nPitch	/=2;
		nInspectX1/=2;
		nInspectX2/=2;
		nMulti=2;
	}

	if(g_System.m_nLineCAM)
	{
#ifdef USE_DECODING_RESET
		nScratch=FindScratch(fm, nInspectX1, 0, nInspectX2, nHeight, nPitch, 1, 60, 0, 32, 16, 10, nScratchThres,nScratchThres, NULL, posx, posy, value, &AverageScratch, &MaxScratch);
		for(i=0;i<nScratch;i++)
		{
			value[i]=value[i]*0.8;
		}
		AverageScratch*=0.8;
		MaxScratch*-0.8;
#else
		nScratch=FindScratch(fm, nInspectX1, 0, nInspectX2, nHeight, nPitch, 1, 35, 0, 32, 16, 10, nScratchThres,nScratchThres, NULL, posx, posy, value, &AverageScratch, &MaxScratch);
#endif
	}
	else
	{
		nScratch=FindScratch(fm, nInspectX1, 0, nInspectX2, nHeight, nPitch, 1, 35, 0, 32, 16, 10, nScratchThres,nScratchThres, NULL, posx, posy, value, &AverageScratch, &MaxScratch);
	}

	g_Temp.m_MaxScratch=MaxScratch;
	g_Temp.m_AveScratch=AverageScratch;





	for(i=0;i<nScratch;i++)
	{

#ifdef LONG_SCRATCH_EMPHASIZE
		nOverlap=0;
		for(j=0;j<g_Defect.m_nDefectCount;j++)
		{
			if( abs(posx[i] - g_Defect.m_Defect[j].x_pos)<64) nOverlap=1;;
		}
		if(nOverlap) continue;
#endif

//		if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) break;

		nLevel=GetLevel(value[i], g_Param.m_nScratch);
		if(nLevel<1) continue;


		//과검 체크-----------------------------------------------------------------------------------------------------------
		nOverkill=IsOverKillForScratch(fm, posx[i], posy[i], nPitch, nHeight, nScratchThres);
		if(nOverkill) continue;

		if(g_System.m_nLineCAM)
			nOverkill=CheckRealScratch(fm, posx[i], posy[i], nPitch, nWidth, nHeight, 24);
		else
			nOverkill=CheckRealScratch(fm, posx[i], posy[i], nPitch, nWidth, nHeight, 24);
		if(nOverkill) continue;
		//--------------------------------------------------------------------------------------------------------------------

		if(!IsInspectArea(posx[i])) continue; //In Position Check


		MakeDefectRect(g_Defect.m_nDefectCount, posx[i]*nMulti, posy[i]*nMulti, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
		CopyNGImageFM(fmORG, g_System.m_nPitch);

		nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_SCRATCH;
		g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=posx[i]*nMulti;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=posy[i]*nMulti;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =value[i]; //Value	
		g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0; //Size
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0; //Size X
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0; //Size Y

		//겹치는 불량 점검-----------------------------------------------------------------------------------------------
		nOverlap=CheckDefectOverlap();
		if(nOverlap==0 && g_Defect.m_nDefectCount<g_System.m_nMaxDefect) //안겹치고 불량갯수도 최대치보다 작으면 추가함.
		{
			g_Defect.m_nDefectCount++;
		}
		else
		{
			if(nOverlap==3) CheckPriorLevel(1); //겹치는것 없고, 불량갯수가 이미 최대치에 도달했으면				
		}
		//--------------------------------------------------------------------------------------------------------------
	}
}

//Y를 128씩 나눔, X는 16씩 나눔 => 각각의 라인값 구함.
//약간 사선으로 기울어진것도 검출 됨.
int FindLongScratch(LPBYTE fm, int left, int top, int right, int bottom, int nHeight, int pitch, int *pMaxScratch, int *pMaxScratchPos);
void WEB_LongScratch()
{
	int i, j, nOverlap;
	int nLevel;
	int nClass;
	int posx[MAX_SCRATCH], posy[MAX_SCRATCH], value[MAX_SCRATCH];
	int nInspect=0, nScratchThres=10000;
	int AverageScratch=0, MaxScratch[3], MaxScratchPos[3];
	int nScratch, nOverkill;
	int nWidth, nHeight, nPitch, nInspectX1, nInspectX2, nMulti=1;
	LPBYTE fmORG=g_fmGrab[g_ID];
	LPBYTE fm=g_fmGrab[g_ID];

	
	//검사하기위한 데이타가 들어 있는지 확인---------------
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])		
			if(g_Param.m_nLongScratch[i]>=100)
			{
				nScratchThres=g_Param.m_nLongScratch[i];
				nInspect=1;
			}
	}
	if(nInspect==0) return;
	g_Temp.m_nLongScratchInspect=1;
	//-----------------------------------------------------

	nWidth		=g_System.m_nImageW;
	nHeight		=g_System.m_nImageH;
	nPitch		=g_System.m_nPitch;
	nInspectX1	=g_Temp.m_nInspectX1;
	nInspectX2	=g_Temp.m_nInspectX2;

	if(g_System.m_nMakePyramid)  //8192이면 1/2영상에서 t
	{
		fm=g_fmGrabComp;
		nWidth	/=2;
		nHeight	/=2;
		nPitch	/=2;
		nInspectX1/=2;
		nInspectX2/=2;
		nMulti=2;
	}

	nScratch=FindLongScratch(fm, nInspectX1, 0, nInspectX2, nHeight, nHeight, nPitch, MaxScratch, MaxScratchPos);
	g_Temp.m_MaxLongScratch=MaxScratch[0];
	g_Temp.m_MaxLongScratchPos=MaxScratchPos[0]*nMulti;

	for(i=0;i<nScratch;i++)
	{
//		if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) break;

		nLevel=GetLevel(MaxScratch[i], g_Param.m_nLongScratch);
		if(nLevel<1) continue;


/*		//과검 체크-----------------------------------------------------------------------------------------------------------
		nOverkill=IsOverKillForScratch(fm, posx[i], posy[i], nPitch, nHeight, nScratchThres);
		if(nOverkill) continue;

		if(g_System.m_nLineCAM)
			nOverkill=CheckRealScratch(fm, posx[i], posy[i], nPitch, nWidth, nHeight, 24);
		else
			nOverkill=CheckRealScratch(fm, posx[i], posy[i], nPitch, nWidth, nHeight, 24);
		if(nOverkill) continue;
		//--------------------------------------------------------------------------------------------------------------------
*/
		posx[i]=MaxScratchPos[i];
		posy[i]=nHeight/2;
		if(!IsInspectArea(posx[i])) continue; //In Position Check


		MakeDefectRect(g_Defect.m_nDefectCount, posx[i]*nMulti, posy[i]*nMulti, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
//		CopyNGImageFM(fmORG, g_System.m_nPitch);
		CopyNGImageFMArea(fmORG, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].left, 0, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].right, g_System.m_nImageH, g_System.m_nPitch, 0);

		nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_LONGSCRATCH;
		g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=posx[i]*nMulti;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=posy[i];
		g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =MaxScratch[i]; //Value	
		g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0; //Size
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0; //Size X
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0; //Size Y

		//겹치는 불량 점검-----------------------------------------------------------------------------------------------
		nOverlap=CheckDefectOverlap();
		if(nOverlap==0 && g_Defect.m_nDefectCount<g_System.m_nMaxDefect) //안겹치고 불량갯수도 최대치보다 작으면 추가함.
		{
			g_Defect.m_nDefectCount++;
		}
		else
		{
			if(nOverlap==3) CheckPriorLevel(1); //겹치는것 없고, 불량갯수가 이미 최대치에 도달했으면				
		}
		//--------------------------------------------------------------------------------------------------------------
	}
}


//평활화 할때 구했던 Profile로 양 옆(Offset3)보다 밝은곳 제일 밝은곳 1개 구해서 100곱한 값으로 긴 라인 구함.
//오직 수직라인 만 검출 (사선으로 된것도 검출하려면 void WEB_LongScratch() 사용해야 함.
void WEB_LongScratchSimple()
{
	int i, j, nOverlap;
	int nLevel;
	int nClass;
	int posx[MAX_SCRATCH], posy[MAX_SCRATCH], value[MAX_SCRATCH];
	int nInspect=0, nScratchThres=10000;
	int AverageScratch=0, MaxScratch[3], MaxScratchPos[3];
	int nScratch, nOverkill, nTmp;
	int nWidth, nHeight, nPitch, nInspectX1, nInspectX2, nMulti=1;
	LPBYTE fmORG=g_fmGrab[g_ID];
	LPBYTE fm=g_fmGrab[g_ID];

	
	//검사하기위한 데이타가 들어 있는지 확인---------------
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])		
			if(g_Param.m_nLongScratch[i]>=100)
			{
				nScratchThres=g_Param.m_nLongScratch[i];
				nInspect=1;
			}
	}
	if(nInspect==0) return;
	g_Temp.m_nLongScratchInspect=1;
	//-----------------------------------------------------

	nWidth		=g_System.m_nImageW;
	nHeight		=g_System.m_nImageH;
	nPitch		=g_System.m_nPitch;
	nInspectX1	=g_Temp.m_nInspectX1;
	nInspectX2	=g_Temp.m_nInspectX2;

	MaxScratch[0]=0;
	for(j=nInspectX1+3;j<nInspectX2-3;j++)
	{
		nTmp=*(g_Profile100+j)-(*(g_Profile100+j-3)+*(g_Profile100+j+3))/2;
		if(nTmp>MaxScratch[0])
		{
			MaxScratch[0]=nTmp;
			MaxScratchPos[0]=j;
		}
	}

	nScratch=1;
	g_Temp.m_MaxLongScratch=MaxScratch[0];
	g_Temp.m_MaxLongScratchPos=MaxScratchPos[0]*nMulti;

	for(i=0;i<nScratch;i++)
	{
//		if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) break;

		nLevel=GetLevel(MaxScratch[i], g_Param.m_nLongScratch);
		if(nLevel<1) continue;


/*		//과검 체크-----------------------------------------------------------------------------------------------------------
		nOverkill=IsOverKillForScratch(fm, posx[i], posy[i], nPitch, nHeight, nScratchThres);
		if(nOverkill) continue;

		if(g_System.m_nLineCAM)
			nOverkill=CheckRealScratch(fm, posx[i], posy[i], nPitch, nWidth, nHeight, 24);
		else
			nOverkill=CheckRealScratch(fm, posx[i], posy[i], nPitch, nWidth, nHeight, 24);
		if(nOverkill) continue;
		//--------------------------------------------------------------------------------------------------------------------
*/
		posx[i]=MaxScratchPos[i];
		posy[i]=nHeight/2;
		if(!IsInspectArea(posx[i])) continue; //In Position Check


		MakeDefectRect(g_Defect.m_nDefectCount, posx[i]*nMulti, posy[i]*nMulti, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
		CopyNGImageFM(fmORG, g_System.m_nPitch);

		nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_LONGSCRATCH;
		g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=posx[i]*nMulti;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=posy[i];
		g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =MaxScratch[i]; //Value	
		g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0; //Size
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0; //Size X
		g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0; //Size Y

		//겹치는 불량 점검-----------------------------------------------------------------------------------------------
		nOverlap=CheckDefectOverlap();
		if(nOverlap==0 && g_Defect.m_nDefectCount<g_System.m_nMaxDefect) //안겹치고 불량갯수도 최대치보다 작으면 추가함.
		{
			g_Defect.m_nDefectCount++;
		}
		else
		{
			if(nOverlap==3) CheckPriorLevel(1); //겹치는것 없고, 불량갯수가 이미 최대치에 도달했으면				
		}
		//--------------------------------------------------------------------------------------------------------------
	}
}


int   l_KProjLocalArray[512*MAX_WIDTH];							//Scratch Thread용
long  l_KProjFlatAvg[MAX_WIDTH];								//최대
long  l_KProj[MAX_WIDTH];										//전체 Projection
long  l_KProjLocal[MAX_WIDTH];									//매 라인 검사시 사용되는 Projection
long  l_KProjErode[MAX_WIDTH];									//l_KProjLocal를 Erode함 
long  l_KProjDilate[MAX_WIDTH];									//l_KProjErode를 Dilate함 
long  l_KProjFlat[MAX_WIDTH];									//l_KProjLocal- l_KProjErode
long  l_KProjFlat1[MAX_WIDTH];									//l_KProjLocal- l_KProjErode
long  l_KProjFlatMax[MAX_WIDTH];								//l_KProjLocal- l_KProjErode
int l_nMax[256][64], l_nMaxBK[256][64], l_nMaxPos[256][64];		//세로 최대 128, 가로는 128씩 체크 
int l_nMax1[256][64];
int l_nUpperCutCount[MAX_WIDTH];
//평균밝기 80  스크라치 90   길면 2037    짧으면(32Pixel)   604    
//                      100  길면 4100                     1209
//                      110  길면 6136                     1814   +35까지
//                      120  길면 7591                     2116
//						130  길면 7591                     2116
//						150  길면 8055                     2116
//                      255  길면 9450                     2116
int FindScratch(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nType, int nUpperCut, int nLowerCut, int nInspY, int nJumpY, int nErode, int nThUp, int nThDn, int *pProj, int *posx, int *posy, int *value, int *AverageScratch, int *MaxScratch)
{
	int i, j, k, m, nAvg, nUpperCutCount;
	int nScratchCount =0;
	int nProjCount=256, nSkipY;
	int nDilateMax, nErodeMin;
	int nInspCount, nInspXCount;
	int nY1, nY2, nX1, nX2, nTmp, nTmpSum, Maxj, Maxi, addValue;
	int nLL, nRR;
	double dRatio;
	long nProjMaxSum=0, nProjMaxCount=0, nProjMax=0, nProjMaxPos[2];
	double dAveProj=0, dAvgRatio=1;
	long nAveValue=0;
	int  nCountTT=0, SValue[10], SValue1[10], Sx[10], Sy[10];
	int  nBASE_VALUE=52;					 //기준밝기는 40
	int nOffsetX, nOffsetY, SValueSum, SValueSum1, nOverlap;
	int nInclude;
	int nMaxSValue, nMaxSPos;
	int nOverLapLimitY=128, nOverLapLimitX=128;
	int nRowNum, nColNum;
	int nLeftSum, nRightSum;
	int nOffset;

	nRowNum=bottom/nJumpY;  if(bottom%nJumpY)  nRowNum++;
	nColNum=right/128;      if(right%128)      nColNum++;
	if(nRowNum>256 || nColNum>64) goto done; //배열 잡은 게 [256][64] 임.

	if(nThUp<200) nThUp=200;				//200이하는 노이즈
	
	if(nErode<2) nErode=2;					 //1 이면  3   
	else if(nErode>20)  nErode=20;			 //10이면  21   

	if(nJumpY<16)  nJumpY=16;								//최소 JUMP 값은 16이다 		
	if((bottom-top)<=nInspY || right-left<=20) goto done;
	memset(l_nMax,    0, nRowNum*64*sizeof(int));
	memset(l_nMaxPos, 0, nRowNum*64*sizeof(int));
	memset(l_KProjFlatAvg,0, pitch*sizeof(long));

	//Projection 데이타가 주어지면 그것을 이용하고 그렇지 않으면 전체 Projection을 구한다.---
	//0.87 msec
	if(pProj==NULL)
	{
		nSkipY=(bottom-top+1)/nProjCount;
		if(nSkipY<1) nSkipY=1;
		nProjCount=0;
		for(i=top;i<bottom;i+=nSkipY) nProjCount++;
		if(nProjCount==0) goto done;
		memset(l_KProj, 0, right*sizeof(long));

		for(i=top;i<bottom;i+=nSkipY)
			for(j=left;j<right;j++)
				*(l_KProj+j)+=*(fm+pitch*i+j);

		for(j=left;j<right;j++)       *(l_KProj+j)/=nProjCount;
	}
	else
	{
		for(j=left;j<right;j++)
			*(l_KProj+j)=*(pProj+j);
	}

	//전체 평균밝기 구해서 전체 평균밝기가 기준값보다 크면 그 만큼 결과를 작게한다.------------
	for(j=left;j<right;j++)
		nAveValue+=*(l_KProj+j);


	nAveValue/=(right-left);
	if(nAveValue>nBASE_VALUE)
	{
		dAvgRatio=(double)nBASE_VALUE/(double)nAveValue;
		dAvgRatio=sqrt(dAvgRatio);
	}
	//-----------------------------------------------------------------------------------------

	nInspCount=nInspXCount=0;
	for(i=top;i<bottom;i+=nJumpY) nInspCount++;
	for(i=left;i<right;i+=128)    nInspXCount++;
	

	//스크라치 아래 루틴일부를 THREAD로 구함.(기존 알고리즘보다 1/3 걸림)
	ThreadFunc_FindScratch(fm, left, right, bottom, pitch, l_KProj, nInspY, nJumpY, nUpperCut, l_KProjLocalArray);

	for(k=0;k<nInspCount;k++)
	{
		nY1=top+k*nJumpY;
		nY2=nY1+nInspY;
		if(nY2>bottom) { nY2=bottom; nY1=nY2-nInspY; }
		
		memset(l_KProjLocal, 0, right*sizeof(long));
		memset(l_nUpperCutCount, 0, right*sizeof(int));

		nLL=nRR=0;
		

		//nY1-nY2구간에서 Projection을 구한다.------------------------------------------
		//nUpperCut자르고 평균보다 어두우면 평균값으로 채운다.
		//보기 가장 쉬운 코딩(시간이 제일 많이 걸림)
/*		for(j=left;j<right;j++)
		{
			nAvg=*(l_KProj+j);
			nUpperCutCount=0;
			nTmpSum=0;
			for(i=nY1;i<nY2;i++)
			{
				nTmp=*(fm+pitch*i+j)-nAvg;
				if(nTmp>nUpperCut)
				{
					if(nUpperCutCount<4) nTmp=nAvg;          //Uppercut초과하는 4개까지 없앰(평균값넣음. 휘점이 스크라치로 분류되는 것 막음)
					else                 nTmp=nAvg+nUpperCut;
					nUpperCutCount++;
				}
				else
					nTmp=*(fm+pitch*i+j);

				nTmpSum+=nTmp;
			}	
			*(l_KProjLocal+j)=nTmpSum*100/nInspY;
		}
*/
		//------------------------------------------------------------------------------
		//위의 코드에 비해서 좀 복잡한데 시간은 10%정도 덜 걸림
		//Thread로 하면 CPU:I7 일때 1/3로 줄어듦.
/*
		for(i=nY1;i<nY2;i++)
		{
			for(j=left;j<right;j++)
			{
				nAvg=*(l_KProj+j);								
				nTmp=*(fm+pitch*i+j)-nAvg;

				if(nTmp>nUpperCut)
				{
					if(l_nUpperCutCount[j]<4) nTmp=nAvg;          //Uppercut초과하는 4개까지 없앰(평균값넣음. 휘점이 스크라치로 분류되는 것 막음)
					else                      nTmp=nAvg+nUpperCut;
					l_nUpperCutCount[j]++;
				}
				else
					nTmp=*(fm+pitch*i+j);
					
				*(l_KProjLocal+j)+=nTmp;;
			}
		}
		for(j=left;j<right;j++)
			*(l_KProjLocal+j)=*(l_KProjLocal+j)*100/nInspY;
*/
		for(j=left;j<right;j++)
			*(l_KProjLocal+j)=*(l_KProjLocalArray+pitch*k+j);
		//-----------------------------------------------------------------------------



//튀는 것만-----------------------------------------------------------------
		//ERODE------------------------------------------------------
		nOffset=2;
		for(j=left+nOffset;j<right-nOffset;j++)
		{
			nTmp=*(l_KProjLocal+j-nOffset);
			for(m=j-nOffset+1;m<=j+nOffset;m++)
				if(*(l_KProjLocal+m)<nTmp) nTmp=*(l_KProjLocal+m);

			l_KProjErode[j]=nTmp;
		}
		for(j=left;j<left+nOffset;j++)   l_KProjErode[j] =l_KProjLocal[j];			
		for(j=right-nOffset;j<right;j++) l_KProjErode[j] =l_KProjLocal[j];
	
		//DILATE------------------------------------------------------
		for(j=left+nOffset;j<right-nOffset;j++)
		{
			nTmp=*(l_KProjErode+j-nOffset);
			for(m=j-nOffset+1;m<=j+nOffset;m++)
				if(*(l_KProjErode+m)>nTmp) nTmp=*(l_KProjErode+m);

			l_KProjDilate[j]=nTmp;
		}
		for(j=left;j<left+nOffset;j++)   l_KProjDilate[j] =l_KProjErode[j];			
		for(j=right-nOffset;j<right;j++) l_KProjDilate[j] =l_KProjErode[j];
//------------------------------------------------------------------------------

//주변밝기 대비 밝은 것 검출----------------------------------------------------
		nLeftSum=nRightSum=0;
		for(j=left;j<left+nErode-1;j++)
			nLeftSum+=*(l_KProjLocal+j);
		for(j=left+nErode+2;j<=left+nErode*2;j++)
			nRightSum+=*(l_KProjLocal+j);
		
		for(j=left+nErode;j<right-nErode;j++)
		{
			l_KProjErode[j]=(nLeftSum+nRightSum)/(nErode*2-2);

			nLeftSum=nLeftSum-*(l_KProjLocal+j-nErode)+*(l_KProjLocal+j-1);
			nRightSum=nRightSum-*(l_KProjLocal+j+2)+*(l_KProjLocal+j+nErode+1);
		}
//-------------------------------------------------------------------------------


		//원래 Profile과 ERODE(5x1)-DILATE(5x1)한것과의 차이(Profile에서 없어진 부분만 구함)
		for(j=left;j<right;j++)
		{
			l_KProjFlat[j]=(l_KProjLocal[j]-l_KProjDilate[j]);    //원본 - (ERODE(5x1)-DILATE(5x1))
			l_KProjFlat1[j] =(l_KProjLocal[j]-l_KProjErode[j]);   //원본 - Average
			l_KProjFlatAvg[j]+=l_KProjFlat[j];
		}


		//범위를 세로로는 nJumpY간격으로 가로로는 128간격으로 가장 높은 l_KProjFlat값들을 찾는다.
		for(m=0;m<nInspXCount;m++)
		{
			if(m==0) nX1=left+nErode;
			else     nX1=left+128*m;
		
			nX2=nX1+128;
			if(nX2>right-nErode) 
				nX2=right-nErode;

			for(j=nX1;j<nX2;j++)
			{
				if(l_KProjFlat[j]>l_nMax[k][m])
				{
					if(j>nLL && j<nRR) //HOLE영역 무시 
					{

					}
					else
					{
						l_nMax[k][m] =l_KProjFlat[j];
						l_nMax1[k][m]=l_KProjFlat1[j];
						l_nMaxPos[k][m]=j;
					}
				}
			}

			//Average구하기 위한값---------------------------
			nProjMaxSum+=l_nMax[k][m];
			nProjMaxCount++;
			

			//제일 높은값 찾기 (Display용)-------------------
			if(l_nMax[k][m]>nProjMax)
			{
				nProjMax=l_nMax[k][m];
				nProjMaxPos[0]=(nX1+nX2)/2;  //제일 쎈 스크라치 위치 
				nProjMaxPos[1]=(nY1+nY2)/2;
			}
			//----------------------------------------------
		}
	}
	
	//Scratch평균값 -------------------------------
	for(j=left;j<right;j++)
		l_KProjFlatAvg[j]/=nInspCount;
	//---------------------------------------------

	//전체 128영역에서 MAX의 평균값 ---------------------
	if(nProjMaxCount) dAveProj=nProjMaxSum/nProjMaxCount;

	*AverageScratch=dAveProj*dAvgRatio;
	*MaxScratch    =nProjMax*dAvgRatio;

//	if(nThUp<dAveProj*2) nThUp=dAveProj*2;		//주어진 Scratch구하는 Threshold 값이 평균 Scratch값의 2배보다 작으면  과검이 발생할 확률이 커지므로 이값으로 대체 

	//스크라치를 찾는다(최대 10개)-----------------------------------------
	memcpy(l_nMaxBK, l_nMax, nInspCount*64*sizeof(int));  //l_nMax를  l_nMaxBK에 백업받는다 (백업데이타를 이용해서 큰 순서대로 찾는다)
	for(k=0;k<MAX_SCRATCH;k++)
	{
		nProjMax=0;
		for(i=0;i<nInspCount;i++)
			for(j=0;j<nInspXCount;j++)
			{
				if(l_nMaxBK[i][j]>nProjMax)
				{
					nProjMax=l_nMaxBK[i][j];
					Maxi=i;
					Maxj=j;
				}
			}

		nInclude=0;
		if(nProjMax>100) //라인 스캔을 48x3영역에서 더함 (Area는 32x5(중복포함))
		{
			SValueSum	=nProjMax;
			SValueSum1  =l_nMax1[Maxi][Maxj];

	//----------------------------------------------------------------------------------------
			//긴 Scratch에 대한 가중치 Local Max가 발생한 x좌표의 +2,-2위치에서 
			if(Maxi>1)
			{
				if(	abs(l_nMaxPos[Maxi][Maxj]-l_nMaxPos[Maxi-2][Maxj])<=2 ) 
				{
					addValue=l_nMax[Maxi-2][Maxj]-l_KProjFlatAvg[l_nMaxPos[Maxi][Maxj]];
					if(addValue>75) SValueSum+=addValue;

					addValue=l_nMax1[Maxi-2][Maxj]-l_KProjFlatAvg[l_nMaxPos[Maxi][Maxj]];
					if(addValue>75) SValueSum1+=addValue;
				}
				nInclude++;
			}
			if(Maxi<nInspCount-2)
			{
				if(	abs(l_nMaxPos[Maxi][Maxj]-l_nMaxPos[Maxi+2][Maxj])<=2 ) 
				{
					addValue=l_nMax[Maxi+2][Maxj]-l_KProjFlatAvg[l_nMaxPos[Maxi][Maxj]];
					if(addValue>75) SValueSum+=addValue;

					addValue=l_nMax1[Maxi+2][Maxj]-l_KProjFlatAvg[l_nMaxPos[Maxi][Maxj]];
					if(addValue>75) SValueSum1+=addValue;
				}
				nInclude++;
			}
			if(Maxi>0 && nInclude<2)
			{
				if(	abs(l_nMaxPos[Maxi][Maxj]-l_nMaxPos[Maxi-1][Maxj])<=2 ) 
				{
					addValue=l_nMax[Maxi-1][Maxj]-l_KProjFlatAvg[l_nMaxPos[Maxi][Maxj]];
					if(addValue>75) SValueSum+=addValue;

					addValue=l_nMax1[Maxi-1][Maxj]-l_KProjFlatAvg[l_nMaxPos[Maxi][Maxj]];
					if(addValue>75) SValueSum1+=addValue;
				}
				nInclude++;
			}
	
			if(Maxi<nInspCount-1 && nInclude<2)
			{
				if(	abs(l_nMaxPos[Maxi][Maxj]-l_nMaxPos[Maxi+1][Maxj])<=2 ) 
				{
					addValue=l_nMax[Maxi+1][Maxj]-l_KProjFlatAvg[l_nMaxPos[Maxi][Maxj]];
					if(addValue>75) SValueSum+=addValue;

					addValue=l_nMax1[Maxi+1][Maxj]-l_KProjFlatAvg[l_nMaxPos[Maxi][Maxj]];
					if(addValue>75) SValueSum1+=addValue;
				}
			}
//----------------------------------------------------------------------------------------
			nOverlap=0;
			for(i=0;i<nScratchCount;i++)
			{
				nOffsetY=top+nInspY/2+nJumpY*Maxi-Sy[i];
				if(nOffsetY<0) nOffsetY*=-1;
				nOffsetX=l_nMaxPos[Maxi][Maxj]-Sx[i];
				if(nOffsetX<0) nOffsetX*=-1;
				if(nOffsetY<nOverLapLimitY && nOffsetX<nOverLapLimitX) //같은 위치이면OVERLAP
				{
					nOverlap	=1;
					if(SValueSum>SValue[i]) //현재 Value가 크면 대체한다.
					{
						SValue[i]	=SValueSum;
						SValue1[i]	=SValueSum1;
						Sx[i]		=l_nMaxPos[Maxi][Maxj];
						Sy[i]		=top+nInspY/2+nJumpY*Maxi;	
					}
					break;
				}
			}

			if(nOverlap==0)
			{
				SValue[nScratchCount]	=SValueSum;
				SValue1[nScratchCount]	=SValueSum1;
				Sx[nScratchCount]		=l_nMaxPos[Maxi][Maxj];
				Sy[nScratchCount]		=top+nInspY/2+nJumpY*Maxi;
				nScratchCount++;
			}
			l_nMaxBK[Maxi][Maxj]=0; //1
			if(Maxi>0)			  l_nMaxBK[Maxi-1][Maxj]=0;
			if(Maxi<nInspCount-1) l_nMaxBK[Maxi+1][Maxj]=0;
		}
		else
		{
			break;
		}
	}

	//제일큰 스크라치 찾는다 (Display용)--
	nMaxSPos=0;
	for(i=0;i<nScratchCount;i++)
	{
		SValue[i]*=dAvgRatio;
		if(SValue[i]>*MaxScratch)
		{
			*MaxScratch=SValue[i];
			nMaxSPos=i;
		}
	}
	if(SValue1[nMaxSPos]<*MaxScratch) 
		*MaxScratch=SValue1[nMaxSPos];  //두가지 방법중 작은 것을 사용한다.
	//------------------------------------

	//제일 큰 순서대로 소팅해서 RETURN-------------------------------------------------
	nCountTT=nScratchCount;
	nScratchCount=0;
	for(i=0;i<nCountTT;i++)
	{
		//제일 큰 것 찾는다-------------
		nMaxSValue=0;
		for(j=0;j<nCountTT;j++)
		{
			if(SValue[j]>nMaxSValue)
			{
				nMaxSValue=SValue[j];
				nMaxSPos=j;
			}
		}
		//------------------------------

		if(SValue[nMaxSPos]>=nThUp && SValue1[nMaxSPos]>=nThUp)
		{
			//겹치는 것이 있는지 체크한다.----------------------------
			nOverlap=0;
			for(k=0;k<nScratchCount;k++)
			{
				nOffsetY=posy[k]-Sy[nMaxSPos];
				if(nOffsetY<0) nOffsetY*=-1;
				nOffsetX=posx[k]-Sx[nMaxSPos];
				if(nOffsetX<0) nOffsetX*=-1;
				if(nOffsetY<nOverLapLimitY && nOffsetX<nOverLapLimitX)
					nOverlap=1;
			}
			//--------------------------------------------------------

			//겹치는 것이 었으면 데이타에 넣는다. (최대 3개)
			if(nOverlap==0 && nScratchCount<3)
			{
				if(SValue[nMaxSPos]<SValue1[nMaxSPos]) value[nScratchCount]=SValue[nMaxSPos];
				else                                   value[nScratchCount]=SValue1[nMaxSPos];

				posx[nScratchCount]=Sx[nMaxSPos];
				posy[nScratchCount]=Sy[nMaxSPos];

				nScratchCount++;
				SValue[nMaxSPos]=0;
			}
		}
	}
	//------------------------------------------------------------
//------------------------------------------------------------


done:


	return nScratchCount;
}

//긴것 두줄 있는 경우
int IsOverKillForScratch(LPBYTE fm, int nX, int nY, int pitch, int nHeight, int nValue)
{
	int ret=1; //과검 
	int i, j, k, y1, y2;
	int left, top, right, bottom, nHCount=0;
	int nAveProj, nMaxProj, nMaxProj1, nPos;
	int nProfile[BAD_IMG_WIDTH];
	int nSumL, nSumR;

	left=nX-BAD_IMG_WIDTH/2; if(left<0) left=0; if(left+BAD_IMG_WIDTH>=pitch) left=pitch-BAD_IMG_WIDTH-1;
	right=left+BAD_IMG_WIDTH; 

	
	top=nY-BAD_IMG_HEIGHT/2; if(top<0) top=0;  if((top+BAD_IMG_HEIGHT)>=nHeight) top=nHeight-BAD_IMG_HEIGHT-1;
	bottom=top+BAD_IMG_HEIGHT;	
	

	nSumL=nSumR=0;
	for(i=top;i<bottom;i++)
	{
		nSumL+=*(fm+i*pitch+left);
		nSumR+=*(fm+i*pitch+right-1);
	}
	if(nSumL/(bottom-top)>150 || nSumR/(bottom-top)>150) return 1;

	for(k=0;k<7;k++)
	{
		y1=top+BAD_IMG_WIDTH*k/7;
		y2=y1+32;
		if(y2>=nHeight) {y2=nHeight; y1=y2-32;}

		memset(nProfile, 0, BAD_IMG_WIDTH*sizeof(int));
		for(i=y1;i<y2;i++)
		{
			for(j=left;j<right;j++)
				nProfile[j-left]+=*(fm+pitch*i+j);
		}

		nAveProj=nMaxProj=nMaxProj1=0;
		for(j=0;j<BAD_IMG_WIDTH;j++)
		{
			if(nProfile[j]>nMaxProj)
			{
				nMaxProj=nProfile[j];
				nPos=j;
			}
			nAveProj+=nProfile[j];
		}
		nAveProj/=BAD_IMG_WIDTH;
		
		if((nMaxProj-nAveProj)*100/32>1500) 
			return 0;

		if((nMaxProj-nAveProj)*100/32<nValue) 
			continue;

		ret=0;
		break;
	}
	
	return ret;
}

void BlobElongation(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nMinLength);
int CheckRealScratch(LPBYTE fm, int nX, int nY, int pitch, int nMaxWidth, int nMaxHeight, int nMinLength)
{
	int nOverKill=1;
	int i,j;
	int left, top, right, bottom;
	int nBlob, nY1, nY2;
	int nSum, nThres, nTmp;
	int nBlobCount=0;
	int nXOffset=64; //나중 32
	int nBinPitch=40;
	int nMaxY[2]={0,0}, nMaxT[2], nMaxB[2], nMaxPos[2]={-1,-1}, nMaxPosX[2];
	int nProfileY[128], nProfileCount=0;

	nBinPitch=nXOffset*2;
	left=nX-nXOffset;
	right=nX+nXOffset;
	top=nY-64;
	bottom=nY+64;
	if(left<0)				left=0;
	if(right>=nMaxWidth)	right=nMaxWidth-1;
	if(top<0)			   {top=0;bottom=top+128;}
	if(bottom>=nMaxHeight) {bottom=nMaxHeight-1; top=bottom-128;}


	nThres=10;
	memset(nProfileY, 0, 128*sizeof(int));

	for(i=top+1;i<bottom-1;i++)
		for(j=left+1;j<right-1;j++)
		{
			nTmp=*(fm+pitch*(i-1)+j-1)+*(fm+pitch*i+j-1)+*(fm+pitch*(i+1)+j-1)-
				(*(fm+pitch*(i-1)+j+1)+*(fm+pitch*i+j+1)+*(fm+pitch*(i+1)+j+1));

			if(nTmp>=nThres) *(g_fmTemp+nBinPitch*(i-top)+(j-left))=255;
			else             *(g_fmTemp+nBinPitch*(i-top)+(j-left))=0;
		}

	BlobElongation(g_fmTemp, 1,1, (right-left)-2, (bottom-top)-2, 128, 4);

	g_Chain.SetChainData(1, g_fmTemp, 1, 1, 4, 1000, nBinPitch, 128);	
	nBlob=g_Chain.FastChain(1, 1, (right-left)-2, (bottom-top)-2);
	if(nBlob)
	{
		for(i=0;i<nBlob;i++)
		{		
			nY1=g_Chain.FindMinY(i);
			nY2=g_Chain.FindMaxY(i);
			if((nY2-nY1)>nMaxY[0])
			{
				nMaxY[0]	=(nY2-nY1);
				nMaxPos[0]	=i;
				nMaxT[0]	=nY1;
				nMaxB[0]	=nY2;
			}
		}
		if(nMaxY[0]>=nMinLength) nOverKill=0;		
	}	
	return nOverKill;
}

void BlobElongation(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nMinLength)
{
	int i, j;
	int nW, nH, nX, nY;
	int nAdd;
	int nBlob, *pixel, nBoundary, nMaxPos;
	double cx, cy, dist, distMax;
	int nPx[2], nPy[2];

	if(nMinLength<2) nMinLength=2;//최소 2
	g_Chain.SetChainData(1, fm, 1, 1, 4, 1000, pitch, bottom);	
	nBlob=g_Chain.FastChain(left, top, right, bottom);

	for(i=0;i<nBlob;i++)
	{
		nW=g_Chain.FindMaxX(i)-g_Chain.FindMinX(i);
		nH=g_Chain.FindMaxY(i)-g_Chain.FindMinY(i);
		if(nW<nMinLength && nH<nMinLength) continue;
		if(nW>nH) nAdd=nW/2;
		else      nAdd=nH/2;

		g_Chain.Chain_Center(i,&cx, &cy);
		nBoundary=g_Chain.GetChainDataNumber(i); 
		pixel=g_Chain.GetChainData(i);
		
		//중심에서 제일 먼위치 찾기
		distMax=0;
		for(j=0;j<nBoundary;j++)
		{
			dist=(pixel[j*2]-cx)*(pixel[j*2]-cx)+(pixel[j*2+1]-cy)*(pixel[j*2+1]-cy);
			if(dist>distMax)
			{
				distMax=dist;
				nPx[0]=pixel[j*2];
				nPy[0]=pixel[j*2+1];
			}
		}
		//(nPx[0], nPy[0])에서 제일 먼위치 찾기
		distMax=0;
		for(j=0;j<nBoundary;j++)
		{
			dist=(pixel[j*2]-nPx[0])*(pixel[j*2]-nPx[0])+(pixel[j*2+1]-nPy[0])*(pixel[j*2+1]-nPy[0]);
			if(dist>distMax)
			{
				distMax=dist;
				nPx[1]=pixel[j*2];
				nPy[1]=pixel[j*2+1];
			}
		}
		dist=sqrt((double)(nPx[1]-nPx[0])*(nPx[1]-nPx[0])+(nPy[1]-nPy[0])*(nPy[1]-nPy[0]))*0.5;
		cx=(nPx[0]+nPx[1])/2;
		cy=(nPy[0]+nPy[1])/2;

		for(j=1;j<=nAdd;j++)
		{
			nX=((dist+j)*nPx[0]-j*cx)/dist;
			nY=((dist+j)*nPy[0]-j*cy)/dist;
			if(nX>left && nX<right && nY>top && nY<bottom)
				*(fm+pitch*nY+nX)=255;

			nX=((dist+j)*nPx[1]-j*cx)/dist;
			nY=((dist+j)*nPy[1]-j*cy)/dist;
			if(nX>left && nX<right && nY>top && nY<bottom)
				*(fm+pitch*nY+nX)=255;
		}
	}
	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
			if(*(fm+pitch*i+j)>0) *(fm+pitch*i+j)=255;
}


void WEB_ScratchBlob_White()
{
	int i, j, k;
	int nLevel;
	int nClass;
	int posx[MAX_SCRATCH], posy[MAX_SCRATCH], value[MAX_SCRATCH];
	int nInspect=0, nScratchThres=10000;
	int AverageScratch, MaxScratch;
	int nScratch, nOverkill;
	int nHeight=g_System.m_nImageH;
	int nWidth=g_System.m_nImageW;
	int pitch=g_System.m_nPitch;
	int left=g_Temp.m_nInspectX1;
	int right=g_Temp.m_nInspectX2;
	int nOffset=g_Param.m_nScratchOffset;
	int nDiff=g_Param.m_nScratchDiff*3-1;
	int nX1, nX2, nY1, nY2, nC;
	int nBlob;
	int ll,tt,rr,bb;
	double dCx, dCy, dAngle, dLong, dShort;
	double pos[4][2], dRatio;
	double dLength[10], dMinLength, dMaxLength;
	int    nLengthPos[10], nMinPos, nMaxPos, nLoop, nMaxI;
	int    nCount=0;
	int    nBoundaryLength, *Pixel, nSum,nInCount, nBoundaryValue, nInValue;
	int   nDilation=1;
	LPBYTE fm=g_fmGrab[g_ID]; //평활화전 이미지

	if(nOffset<=0 || nDiff<=0) return;
	if(nDiff<5) nDiff=5;
	
	//검사하기위한 데이타가 들어 있는지 확인---------------
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])		
			if(g_Param.m_nScratchValue[i]>0 || g_Param.m_dScratchLength[i]>0.01)
				nInspect=1;
	}
	if(nInspect==0) return;
	//-----------------------------------------------------

	memset(g_fmTemp, 0, nHeight*nWidth);
	memset(g_fmTemp1, 0, nHeight*nWidth);
	for(i=nOffset;i<nHeight-nOffset;i++)
	{
		for(j=left+nOffset;j<right-nOffset;j++)
		{
			nC=*(fm+pitch*i+j)*3;
			nX1=*(fm+pitch*i+j-nOffset)+*(fm+pitch*(i-1)+j-nOffset)+*(fm+pitch*(i+1)+j-nOffset);
			nX2=*(fm+pitch*i+j+nOffset)+*(fm+pitch*(i-1)+j+nOffset)+*(fm+pitch*(i+1)+j+nOffset);
			nY1=*(fm+pitch*(i-nOffset)+j-1)+*(fm+pitch*(i-nOffset)+j)+*(fm+pitch*(i-nOffset)+j+1);
			nY2=*(fm+pitch*(i+nOffset)+j-1)+*(fm+pitch*(i+nOffset)+j)+*(fm+pitch*(i+nOffset)+j+1);
			if( (nC-nX1>nDiff && nC-nX2>nDiff) || (nC-nY1>nDiff && nC-nY2>nDiff) )
			{
				*(g_fmTemp1+pitch*(i-1)+j-1)		=255;
				*(g_fmTemp1+pitch*(i-1)+j)			=255;
				*(g_fmTemp1+pitch*(i-1)+j+1)		=255;
				*(g_fmTemp1+pitch*i+j-1)			=255;
				*(g_fmTemp1+pitch*i+j)				=255;
				*(g_fmTemp1+pitch*i+j+1)			=255;
				*(g_fmTemp1+pitch*(i+1)+j-1)		=255;
				*(g_fmTemp1+pitch*(i+1)+j)			=255;
				*(g_fmTemp1+pitch*(i+1)+j+1)		=255;
				*(g_fmTemp+pitch*i+j)				=255;
			}
		}
	}
	g_Chain.SetChainData(1, g_fmTemp1, nDilation,nDilation, 20, 10000, nWidth, nHeight);

	nBlob=g_Chain.FastChain(left+nOffset+nDilation,nOffset+nDilation,right-nOffset-nDilation-1, nHeight-nOffset-nDilation-1);

	for(i=0;i<nBlob;i++)
	{
		g_Chain.Chain_Center(i, &dCx, &dCy);
		dAngle=g_Chain.FindAngle(i);
		if(dAngle==0) dAngle=0.1;
		g_Chain.FindMinRect(i, dCx, dCy, dAngle, pos);
		dLong=sqrt((pos[0][0]-pos[1][0])*(pos[0][0]-pos[1][0])+(pos[0][1]-pos[1][1])*(pos[0][1]-pos[1][1]));
		dShort=sqrt((pos[1][0]-pos[2][0])*(pos[1][0]-pos[2][0])+(pos[1][1]-pos[2][1])*(pos[1][1]-pos[2][1]));
		if(dLong<1) dLong=1;
		if(dShort<1) dShort=1;
		if(dShort<dLong)
		{
			dRatio=dLong/dShort;
		}
		else
		{
			dRatio=dShort/dLong;
			dLong=dShort;
		}
		if(dRatio<3) continue;

		if(nCount<10)
		{
			dLength[nCount]=dLong;
			nLengthPos[nCount]=i;
			nCount++;
		}
		else
		{
			dMinLength=dLength[0];
			nMinPos=0;
			for(j=1;j<nCount;j++)
			{
				if(dLength[j]<dMinLength)
				{
					dMinLength=dLength[j];
					nMinPos=j;
				}
			}
			if(dLong>dMinLength)
			{
				dLength[nMinPos]=dLong;
				nLengthPos[nMinPos]=i;
			}
		}
	}

	nLoop=nCount;
	for(k=0;k<nLoop;k++)
	{
		dMaxLength=10;
		nMaxPos=-1;
		for(i=0;i<nCount;i++)
		{
			if(dLength[i]>dMaxLength)
			{
				dMaxLength=dLength[i];
				nMaxPos=nLengthPos[i];
				nMaxI=i;
			}
		}
		if(nMaxPos<0) break;
		dLength[nMaxI]=0;

		
		nBoundaryLength=g_Chain.GetChainDataNumber(nMaxPos);
		Pixel=g_Chain.GetChainData(nMaxPos);
		nSum=0;
		for(i=0;i<nBoundaryLength;i++)
		{
			nSum+=*(fm+pitch*Pixel[2*i+1]+Pixel[2*i]);
		}
		nBoundaryValue=nSum/nBoundaryLength;

		ll=g_Chain.FindMinX(nMaxPos);
		tt=g_Chain.FindMinY(nMaxPos);
		rr=g_Chain.FindMaxX(nMaxPos);
		bb=g_Chain.FindMaxY(nMaxPos);
		g_Chain.Chain_Center(nMaxPos, &dCx, &dCy);

		nSum=nInCount=0;
		for(i=tt;i<=bb;i++)
			for(j=ll;j<=rr;j++)
			{
				if(*(g_fmTemp+pitch*i+j)==255)
				{
					nSum+=*(fm+pitch*i+j);
					nInCount++;
				}
			}
		if(nInCount) nInValue=nSum/nInCount;
		nScratch=nInValue-nBoundaryValue;

		dMaxLength=(dMaxLength-nDilation*2)*g_Param.m_dScaleFactorY;
		nLevel=GetLevel((double)nScratch, dMaxLength, g_Param.m_nScratchValue, g_Param.m_dScratchLength);
		if(nLevel>0)
		{
			if(!IsInspectArea(dCx)) continue; //In Position Check

			MakeDefectRect(g_Defect.m_nDefectCount, dCx, dCy, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			CopyNGImage(true);
			nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_SCRATCH;
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos	   =dCx;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos	   =dCy;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nScratch; //Value	
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =dMaxLength; //Size
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =dMaxLength; //Size X
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =dMaxLength; //Size Y
			if(CheckDefectOverlap()==0) 
				g_Defect.m_nDefectCount++;

			if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) break;
		}
	}
}

int GetScratchData(int *fm, int x1, int y1, int x2, int y2, int nPitch);
//Y를 128씩 나눔, X는 16씩 나눔 => 각각의 라인값 구함.
//
//Value=꽉찼을때 평균차의 250배
//int l_nLScratch[64][512];	//최대 이미지 8192x8192
int l_nLScratch[64*512];	//최대 이미지 8192x8192
#define LSCT_OFF	3
#define LSCT_XAREA	16
#define LSCT_H		128


int FindLongScratch(LPBYTE fm, int left, int top, int right, int bottom, int nHeight, int pitch, int *pMaxScratch, int *pMaxScratchPos)
{
	int nLScarchCount=0;
	int i, ii, j, jj, ntmp, ntmpMax;
	int nLoop=nHeight/LSCT_H;
	int nXArea=pitch/LSCT_XAREA;
	int nMax, nMaxPos, nX1, nX2;
	int nMaxSkip=0;
	int *pLocalMaxScratch;
	LPBYTE fmT;
	
	//맨가에 몇 줄은 사용하지 않는다.
	if(left<20) left=20;
	if(right>pitch-20) right=pitch-20;

	memset(l_nLScratch, 0, 64*512*sizeof(int));
	for(i=0;i<nLoop;i++)
	{
		memset(l_KProjLocal, 0, right*sizeof(int));
		fmT=fm+i*LSCT_H*pitch;

		for(ii=0;ii<LSCT_H-1;ii+=2)
			for(j=left;j<right;j++)
			   *(l_KProjLocal+j)+=(*(fmT+pitch*ii+j)+*(fmT+pitch*(ii+1)+j))/2;


		pLocalMaxScratch=l_nLScratch+nXArea+nXArea*i;
		for(j=left+LSCT_OFF+1;j<right-LSCT_OFF-1;j++)
		{
			ntmp=*(l_KProjLocal+j)-(*(l_KProjLocal+j-LSCT_OFF)+*(l_KProjLocal+j-LSCT_OFF+1)+*(l_KProjLocal+j+LSCT_OFF-1)+*(l_KProjLocal+j+LSCT_OFF))/4;
			if(ntmp> *(pLocalMaxScratch+j/LSCT_XAREA)) *(pLocalMaxScratch+j/LSCT_XAREA)=ntmp;
		}

#ifdef LONG_SCRATCH_EMPHASIZE
		for(j=0;j<nXArea;j++)
		{
			*(pLocalMaxScratch+j)=*(pLocalMaxScratch+j)*133/(LSCT_H/2);   //1.33배 증폭함. 대신에 16개 영역중 가장큰값 2, 가장작은값 2개 없앰.
			if(*(pLocalMaxScratch+j)<250) *(pLocalMaxScratch+j)*=0.5;     
		}
#else
		for(j=0;j<nXArea;j++)
			*(pLocalMaxScratch+j)=*(pLocalMaxScratch+j)*100/(LSCT_H/2);
#endif
	}
		
	memset(l_KProjLocal, 0, nXArea*sizeof(int));

	for(i=0;i<nLoop;i++)
	{
		for(j=0;j<nXArea;j++)
		{
			l_KProjLocal[j]+=*(l_nLScratch+nXArea*i+j);
		}
	}

	for(i=0;i<3;i++)
	{
		pMaxScratch[i]=0;
		nMax=0;
		nMaxPos=-1;
		for(j=0;j<nXArea;j++)
		{
			if(l_KProjLocal[j]>nMax)
			{
				nMax=l_KProjLocal[j];
				nMaxPos=j;
			}
		}
		if(nMaxPos>=0)
		{
			//제일 높은점 -2~2 중에서 가장 높은 것으로 loop개 만큼 더함.
			nX1=nMaxPos-2; if(nX1<0) nX1=0;
			nX2=nMaxPos+2; if(nX2>=nXArea) nX2=nXArea-1;
			
			
			nMax=0;
			for(j=nX1;j<=nX2;j++)
			{
				for(jj=nX1;jj<=nX2;jj++)
				{
					ntmp=GetScratchData(l_nLScratch, j,0,jj, nLoop, nXArea);
					if(ntmp>nMax)
					{
						nMax=ntmp;
					}
				}
			}
			
			pMaxScratch[nLScarchCount]=nMax;
			pMaxScratchPos[nLScarchCount]=nMaxPos*LSCT_XAREA+LSCT_XAREA/2;
		

			//한번 구한영역 +/- 64는 또 검출하지 않는다.---------
			nX1=nMaxPos-4; if(nX1<0) nX1=0;
			nX2=nMaxPos+4; if(nX2>nXArea) nX2=nXArea;
			for(j=nX1;j<nX2;j++)
				l_KProjLocal[j]=0;
			//----------------------------------------------------

			nLScarchCount++;
		}
	}


	return nLScarchCount;
}

int GetScratchData(int *fm, int x1, int y1, int x2, int y2, int nPitch)
{
	int i, nCount=0;
    int fLineLength;
	double  nAbsX1_X2, nAbsY1_Y2;
    double slope;
	int  N ; // Interpolation Function에서 사용하는 변수들 
	double a,b;  // f(a,b)=P(N,M)(1-a)(1-b)+P(N+1,M)(a)(1-b)+p(N,M+1)(1-a)(b)+P(N+1,M+1)(a)(b)
	double pX[LSCT_H+10], pY[LSCT_H+10], pGrayValue[LSCT_H+10];
	int nTmp, nMax=0, nSum=0;
	int nScratchValue[64];

    nAbsX1_X2 = abs(x1-x2);
	nAbsY1_Y2 = abs(y1-y2);

	// 라인의 길이 
    fLineLength=y2-y1;
	if(fLineLength<10) return 0;

	if(nAbsX1_X2>0 && nAbsY1_Y2>0)
    {	
		slope = (double)(x1-x2)/fLineLength;
        for(i=y1;i<y2;i++)
        {
			pY[i]=i;
			pX[i]=slope*(pY[i]-y1)+x1; //직선의 방정식으로 부터 각 좌표를 구한다.
        }           
        

        // 각 좌표에서의 픽셀값을 구한다.
		for(i=y1;i<y2;i++)
        {
			N=(int)pX[i];
			if(N>nPitch) break;
			if(*(fm+i*nPitch+N) > *(fm+i*nPitch+N+1) ) nTmp=*(fm+i*nPitch+N);
			else									   nTmp=*(fm+i*nPitch+N+1);

			nScratchValue[nCount]=nTmp;
			nCount++;
        }
    }
	else if(nAbsX1_X2==0)
	{
        for(i=y1;i<y2;i++)
        {
			pY[i]=i;
			pX[i]=(double) x1; 
			nTmp=*(fm+nPitch*(int)pY[i]+(int)x1);
			nScratchValue[nCount]=nTmp;
			nCount++;
        }
	}
	SortData(nCount, nScratchValue);  //작은게 0번

	for(i=3;i<nCount-2;i++)
		nSum+=nScratchValue[i];

	return  (nSum/(nCount-4));
}

/*
int FindLongScratch(LPBYTE fm, int left, int top, int right, int bottom, int nHeight, int pitch, int *pMaxScratch, int *pMaxScratchPos)
{
	int nLScarchCount=0;
	int i, ii, j, ntmp, ntmpMax;
	int nLoop=nHeight/LSCT_H;
	int nXArea=pitch/LSCT_XAREA;
	int nMax, nMaxPos, nX1, nX2;
	int nMaxSkip=0;
	int *pLocalMaxScratch;
	LPBYTE fmT;
	
	memset(l_nLScratch, 0, nLoop*512*sizeof(int));
	for(i=0;i<nLoop;i++)
	{
		memset(l_KProjLocal, 0, right*sizeof(int));
		fmT=fm+i*LSCT_H*pitch;

		for(ii=0;ii<LSCT_H-1;ii+=2)
			for(j=left;j<right;j++)
			   *(l_KProjLocal+j)+=(*(fmT+pitch*ii+j)+*(fmT+pitch*(ii+1)+j))/2;

		//for(j=left+1;j<right-1;j++)
		//{
		//	if(*(l_KProjLocal+j-1)>*(l_KProjLocal+j)) ntmp=*(l_KProjLocal+j-1);
		//	else									  ntmp=*(l_KProjLocal+j);

		//	if(*(l_KProjLocal+j+1)>ntmp) ntmp=*(l_KProjLocal+j+1);
		//	*(l_KProjDilate+j)=ntmp;
		//}

		//for(j=left+1;j<right-1;j++)
		//	*(l_KProjLocal+j)=*(l_KProjDilate+j);

		pLocalMaxScratch=l_nLScratch[i];
		for(j=left+LSCT_OFF+1;j<right-LSCT_OFF-1;j++)
		{
			ntmp=*(l_KProjLocal+j)-(*(l_KProjLocal+j-LSCT_OFF)+*(l_KProjLocal+j-LSCT_OFF+1)+*(l_KProjLocal+j+LSCT_OFF-1)+*(l_KProjLocal+j+LSCT_OFF))/4;
			if(ntmp> *(pLocalMaxScratch+j/LSCT_XAREA)) *(pLocalMaxScratch+j/LSCT_XAREA)=ntmp;
		}

#ifdef LONG_SCRATCH_EMPHASIZE
		for(j=0;j<nXArea;j++)
		{
			*(pLocalMaxScratch+j)=*(pLocalMaxScratch+j)*150/(LSCT_H/2);   //1.5배 증폭 대신에 16개 영역중 가장큰값 1개 없앰.
			if(*(pLocalMaxScratch+j)<300) *(pLocalMaxScratch+j)*=0.5;     
		}
#else
		for(j=0;j<nXArea;j++)
			*(pLocalMaxScratch+j)=*(pLocalMaxScratch+j)*100/(LSCT_H/2);
#endif
	}
		
	memset(l_KProjLocal, 0, nXArea*sizeof(int));

	for(i=0;i<nLoop;i++)
	{
		for(j=0;j<nXArea;j++)
		{
			l_KProjLocal[j]+=l_nLScratch[i][j];
		}
	}

	for(i=0;i<3;i++)
	{
		pMaxScratch[i]=0;
		nMax=0;
		nMaxPos=-1;
		for(j=0;j<nXArea;j++)
		{
			if(l_KProjLocal[j]>nMax)
			{
				nMax=l_KProjLocal[j];
				nMaxPos=j;
			}
		}
		if(nMaxPos>=0)
		{
			//제일 높은점 -1,0,1 셋중에서 가장 높은 것으로 loop개 만큼 더함.
			nX1=nMaxPos-1; if(nX1<0) nX1=0;
			nX2=nMaxPos+1; if(nX2>=nXArea) nX2=nXArea-1;
			
			
			for(ii=0;ii<nLoop;ii++)
			{
				if(l_nLScratch[ii][nX1]>l_nLScratch[ii][nX2]) nMax=l_nLScratch[ii][nX1];
				else										  nMax=l_nLScratch[ii][nX2];
				
				if(l_nLScratch[ii][nMaxPos]>nMax) nMax=l_nLScratch[ii][nMaxPos];
				pMaxScratch[i]+=nMax;

				if(nMax>nMaxSkip) nMaxSkip=nMax;
			}
			pMaxScratch[nLScarchCount]=(pMaxScratch[i]-nMaxSkip)/(nLoop-1);
			pMaxScratchPos[nLScarchCount]=nMaxPos*LSCT_XAREA+LSCT_XAREA/2;
		

			//한번 구한영역 +/- 64는 또 검출하지 않는다.---------
			nX1=nMaxPos-4; if(nX1<0) nX1=0;
			nX2=nMaxPos+4; if(nX2>nXArea) nX2=nXArea;
			for(j=nX1;j<nX2;j++)
				l_KProjLocal[j]=0;
			//----------------------------------------------------

			nLScarchCount++;
		}
	}


	return nLScarchCount;
}
*/
