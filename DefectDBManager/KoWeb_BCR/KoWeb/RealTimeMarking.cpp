#include "stdafx.h"

//----------------------------------------------------------------------------------------------------------------------
//  
//  다발구간 불량설정
//
//	설정구간(유저설정 300~5000mm) 에서 각 DEFECT별 동일종류, 해당 LEVEL이 N개 이상이면 해당 LEVEL 모두 MARKING 한다.
//                                      
//  RING BUFFER
//
//---------------------------------------------------------------------------------------------------------------------------

#ifdef CHANGEABLE_MARKING


//USE0: Y방향으로 최대 8000mm 사용가능
int l_nYDefectCnt[MAX_DEFECT_SORT][10][1000];	//[Defect수][10 Level][10mm간격으로 1000개]   2000은 여유공간
int l_nBunchCntY=30;								//1M


//USE1: FOV 최대 400mm폭, Y방향으로 최대 300M 
int l_nYPeriodData[MAX_DEFECT_SORT][MAX_FOV/MIN_DEVIDE_FOV][1000];   //[MAX_DEFECT_SORT]Defect종류 0~8 까지 [9]사용안함
int l_nPeriodMarkingX[MAX_DEFECT_SORT][MAX_FOV/MIN_DEVIDE_FOV];	   //[MAX_DEFECT_SORT]Defect종류 0~8 => 1이면 마킹
int l_nMaxY=0, l_nMaxX=0;


//파라미터 읽을때 마다(LOT변경시 포함)--------------------------------------------
void ChangeableMarkingReset()
{
	memset(l_nYDefectCnt,     0, MAX_DEFECT_SORT*10*1000*sizeof(int));
	memset(l_nYPeriodData,	  0, MAX_DEFECT_SORT*MAX_FOV/MIN_DEVIDE_FOV*1000*sizeof(int));
	memset(l_nPeriodMarkingX, 0, MAX_DEFECT_SORT*MAX_FOV/MIN_DEVIDE_FOV*sizeof(int));

	//USE0에 사용-------------------------------------------------------------------
	//최대 5 M 까지만 사용함.
	l_nBunchCntY=(int)((g_Mark.dBaseLength_mm+RING_BUFFER_MM)/MINY_INTERVAL);
	if(l_nBunchCntY>1000) 
	{
		g_Mark.dBaseLength_mm=5000;
		l_nBunchCntY=(int)((g_Mark.dBaseLength_mm+RING_BUFFER_MM)/MINY_INTERVAL);
	}
	
	//USE1에사용--------------------------------------------------------
	l_nMaxY   =(int)((g_Mark.dBaseLength_M+RING_BUFFER_M)/MIN_PERIOD);
	l_nMaxX   =(g_System.m_nImageW*g_Param.m_dScaleFactorX)/MIN_DEVIDE_FOV;
	if(l_nMaxX>250) l_nMaxX=250;
}
//----------------------------------------------------------------------------------


void InsertDefectData(int N, int *pType, float dLength, float *pY)
{
	int i, j, nY;
	float dY;
	int nClass;
	int nLevel;
	double dBaseLength_mm=(g_Mark.dBaseLength_mm+RING_BUFFER_MM); //최대 7000

	for(i=0;i<N;i++)
	{
		nClass=pType[i]%CLASSDEV;
		if(nClass<=0 || nClass>MAX_DEFECT_SORT) continue;

		nLevel=pType[i]/MILLION;
		if(nLevel<1 || nLevel>10) continue;

		if(g_Mark.nLevelMCount[nClass-1][nLevel-1]==0) continue; //가변마킹 사용하지 않는 불량이면 버린다.
	
		dY=(pY[i]+dLength)-dBaseLength_mm*(int)((pY[i]+dLength)/dBaseLength_mm);
		nY=(int)(dY/MINY_INTERVAL);
		if(nY>=1000) continue; //dY가 최대 7000 이므로 nY는 최대 700

		for(j=0;j<10;j++)
		{
			if(g_Mark.nLevelMCount[nClass-1][j])
				l_nYDefectCnt[nClass-1][j][nY]++;
		}
	}
}

void UpdateMarking(double dCurrentY_mm) //현재 Y좌표 (M)
{
	int i,j,k;
	double dY;
	int nY;
	int nYY;
	double dBaseLength_mm=(g_Mark.dBaseLength_mm+RING_BUFFER_MM);
	int nCount=(int)(g_Mark.dBaseLength_mm/MINY_INTERVAL);
	int nDefectSum[MAX_DEFECT_SORT][10];

	dY=dCurrentY_mm-dBaseLength_mm*(int)(dCurrentY_mm/dBaseLength_mm);
	nY=(int)(dY/10)+1;

	for(i=0;i<MAX_DEFECT_SORT;i++) //Defect종류
		for(j=0;j<10;j++) //Level
		{
			nDefectSum[i][j]=0;
			g_Mark.nMarking[i][j]=0;
			for(k=0;k<nCount;k++)
			{
				nYY=nY-k;
				if(nYY<0) nYY+=l_nBunchCntY;
				nDefectSum[i][j]+=l_nYDefectCnt[i][j][nYY];
			}

			//불량합이 실행갯수 합을 넘어가면 실행Check된 모든것은 마킹한다.--------------
			if(nDefectSum[i][j]>=g_Mark.nLevelMCount[i][j] && g_Mark.nLevelMCount[i][j]>0)
			{
				for(k=0;k<10;k++)
				{
					if(g_Mark.nLevelMCount[i][j]>0) g_Mark.nMarking[i][k]=1;
				}
			}
		}

	//유저가 설정한 마킹데이타는 마킹해야 한다. --------------
	for(i=0;i<10;i++) 
		for(j=0;j<10;j++)
		{
			if(g_Mark.nData[i][j])
				g_Mark.nMarking[i][j]=1;
		}
	//-------------------------------------------------------

	//
	//	l_nYDefectCnt 지우는 부분 추가해야 함-----------------------
	nY=(int)(dY/10)+50; //현재위치 +500mm에서 500mm 데이타를 지운다.
	for(i=0;i<MAX_DEFECT_SORT;i++) //불량별
		for(j=0;j<10;j++) //Level별
		{
			for(k=0;k<50;k++) //50개만 지운다.
			{
				nYY=nY+k;
				if(nYY>=l_nBunchCntY) nYY-=l_nBunchCntY;

				l_nYDefectCnt[i][j][nYY]=0;
			}
		}
	//--------------------------------------------------------------
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//
//	주기성 불량 가변마킹 사용
//
//	30M간격(유저설정 1 ~ 300M) 으로 +/- 10mm 영역에서  동일종류의 불량이 N개 이상이면 모든 Level마킹한다.
//
//  RING BUFFER 사용함. (여유공간 2M)
//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//불량발생 마다 CALL
//X는 전체좌표(0~2350 mm)
//Y는 전체좌표(300 ~ 5000M)
void InsertDefectDataForPeriod(int N, int *pType, float *pX, float dLength, float *pY)
{
	int i,j,k ; 
	int nX, nY;
	int nClass;
	double dBaseLength_M=g_Mark.dBaseLength_M+RING_BUFFER_M;
	float dY;
	
	for(i=0;i<N;i++)
	{
		nClass=pType[i]%CLASSDEV; //불량종류 10
		if(nClass<1 || nClass>MAX_DEFECT_SORT) continue;		

		nX=(int)((pX[i]-g_Param.m_dCamStartPosX)/2.0);  //X위치  0~199  커버할폭 (0~400mm)
		if(nX<0 || nX>=l_nMaxX) continue;		 
		
		dY= (dLength+pY[i]/1000.)-dBaseLength_M*(int)((dLength+pY[i]/1000.)/dBaseLength_M);   //0~dBaseLength_M
		nY=dY/MIN_PERIOD;																  //0~l_nMaxY
		if(nY<0 || nY>=1000) continue; //버퍼 최대 1000개

		l_nYPeriodData[nClass-1][nX][nY]=1;  //0.33영역에서 1개만 인정, 전부 인정하려면 ++하면됨.
	}
}

//한Frame에 한번 CALL 최소단위는 100mm (0.1M)
void UpdateMarkingPeriod(double dCurrentY_M) //현재 Y좌표 (M)
{
	int i,j, nClass;
	int nSum[MAX_FOV/MIN_DEVIDE_FOV+1];
	int nOffset=(g_Mark.dBaseX_mm*2)/MIN_DEVIDE_FOV;
	int nDefectSum;
	int nDeleteNum=(int)(1.0/MIN_PERIOD); //가장 마지막 1M 지운다.
	double dBaseLength_M=g_Mark.dBaseLength_M+RING_BUFFER_M;
	double dCurrY, dY;
	int nCurrY,nY;
	int nStartY, nEndY;

	if(l_nMaxX>250) l_nMaxX=250;
	if(dCurrentY_M-g_Mark.dBaseLength_M<0)
	{
		nStartY=0;
		nEndY=g_Mark.dBaseLength_M/MIN_PERIOD;
	}
	else
	{
		dY= (dCurrentY_M-g_Mark.dBaseLength_M)-dBaseLength_M*(int)((dCurrentY_M-g_Mark.dBaseLength_M)/dBaseLength_M);   //0~dBaseLength_M
		nStartY=dY/MIN_PERIOD;	if(nStartY>=l_nMaxY) nStartY=nStartY-l_nMaxY;							
		nEndY = nStartY+g_Mark.dBaseLength_M/MIN_PERIOD+1;
	}

	for(nClass=0;nClass<MAX_DEFECT_SORT;nClass++)  //DEFECT별로
	{
		for(j=0;j<l_nMaxX;j++)  //해당 X위치에 2mm 간격으로 몇개씩 DEFECT이 있는지 계산
		{
			nSum[j]=0;
			for(i=nStartY;i<=nEndY;i++)     
			{
				nY=i;
				if(nY>=l_nMaxY) nY=nY-l_nMaxY;
				nSum[j]+=l_nYPeriodData[nClass][j][nY];
			}
		}

		//해당 DEFECT이 유저설정구간에 몇개인지 체크-------------------------------------
		nDefectSum=0;
		for(j=0;j<nOffset;j++)
			nDefectSum+=nSum[j];


		for(j=0;j<l_nMaxX-nOffset;j++)
		{
			if(nDefectSum>=g_Mark.nDefectMCount[nClass] && g_Mark.nDefectMCount[nClass]>0)
				l_nPeriodMarkingX[nClass][j+nOffset/2]=1;
			else
				l_nPeriodMarkingX[nClass][j+nOffset/2]=0;

			nDefectSum=nDefectSum+nSum[j+nOffset]-nSum[j];
		}
		
		//왼쪽 가
		for(j=0;j<nOffset/2;j++)
			l_nPeriodMarkingX[nClass][j]=l_nPeriodMarkingX[nClass][nOffset/2];

		//오른쪽 가
		for(j=l_nMaxX-nOffset/2;j<l_nMaxX;j++)
			l_nPeriodMarkingX[nClass][j]=l_nPeriodMarkingX[nClass][l_nMaxX-nOffset/2-1];
	}

	//가장 마지막 1M 데이타는 미리 지운다
	for(nClass=0;nClass<9;nClass++)
	{
		for(i=nEndY+1;i<nEndY+1+nDeleteNum;i++)
		{
			nY=i;
			if(nY>=l_nMaxY) nY=nY-l_nMaxY;

			for(j=0;j<l_nMaxX;j++)
				l_nYPeriodData[nClass][j][nY]=0;
		}
	}
}

#endif