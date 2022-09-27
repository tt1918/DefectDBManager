#include "stdafx.h"

KIPOPRESS		g_Kipo;
MAKRINGVISION   g_MakringVision;

BYTE	l_SendBuffer[100];
extern CXManageSocket  l_Send_Client[4];
extern CXManageSocket  l_Recv_Client[4];

#ifdef MY_SEND

//보내는 함수(최대 4군데 PC에 보낸다)
void KO_SendDefect()
{
	int i, k;
	int nCode=NM_KIPOPRESS;
	int nLength;
	int nNGCount[4]={0,0,0,0}, nX[4][10], nY[4][10];
	double dX1=10000, dX2=10000, dX3=10000;
	int pType[50];
	float pX[50];
	float pY[50];
	int nCount=g_DefectSend.m_nDefectCount;

	if(g_Kipo.m_nPort[1]>0) dX1=g_Kipo.m_COSRecvCalX[g_Kipo.m_nPort[1]];  //두번째 카메라 CalX
	if(g_Kipo.m_nPort[2]>0) dX2=g_Kipo.m_COSRecvCalX[g_Kipo.m_nPort[2]];  //세번째 카메라 CalX
	if(g_Kipo.m_nPort[3]>0) dX3=g_Kipo.m_COSRecvCalX[g_Kipo.m_nPort[3]];  //네번째 카메라 CalX

	for(i=0;i<nCount;i++)
	{
		pType[i]=g_DefectSend.m_Defect[i].defect_class;
		pX[i]	=g_DefectSend.m_Defect[i].x_pos;
		pY[i]	=g_DefectSend.m_Defect[i].y_pos;
	}

	//X의 좌표에 따라서 불량을 분류한다.----------------------------------
	for(i=0;i<nCount;i++)
	{
		//RECV에 보낸다.
//		if(pType[i]/MILLION==1) continue; //LEVEL1은 보내지 않음.

		if(pType[i]%CLASSDEV==SEND_OPTIC_NG_TYPE1 || pType[i]%CLASSDEV==SEND_OPTIC_NG_TYPE2)  //해당하는 불량을 보낸다.(투영은 백/흑 만 보낸다) 
		{																					  //다른 광학계는 거기에 맞게 설정
			if(pX[i]<dX1) 
			{
				if(nNGCount[0]<9)
				{
					nX[0][nNGCount[0]]=pX[i];
					nY[0][nNGCount[0]]=pY[i]+g_Kipo.m_KIPODistance[0];
					nNGCount[0]++;
				}
			}
			else if(pX[i]<dX2)
			{
				if(nNGCount[1]<9)
				{
					nX[1][nNGCount[1]]=pX[i];
					nY[1][nNGCount[1]]=pY[i]+g_Kipo.m_KIPODistance[1];
					nNGCount[1]++;
				}
			}
			else if(pX[i]<dX3)
			{
				if(nNGCount[2]<9)
				{
					nX[2][nNGCount[2]]=pX[i];
					nY[2][nNGCount[2]]=pY[i]+g_Kipo.m_KIPODistance[2];
					nNGCount[2]++;
				}
			}
			else
			{
				if(nNGCount[3]<9)
				{
					nX[3][nNGCount[3]]=pX[i];
					nY[3][nNGCount[3]]=pY[i]+g_Kipo.m_KIPODistance[3];
					nNGCount[3]++;
				}
			}
		}
	}
	//--------------------------------------------------------------------

	//최대 4개의 COS7에 결과를 보낸다.--------------------------------------
	for(k=0;k<4;k++)
	{
		if(g_Kipo.m_nPort[k]<=0) continue;

		if(nNGCount[k]>0)
		{
			nLength=12+nNGCount[k]*8;

			memcpy(l_SendBuffer, &nLength, 4);
			memcpy(l_SendBuffer+4, &nCode, 4);
			memcpy(l_SendBuffer+8, &(nNGCount[k]), 4);

			for(i=0;i<nNGCount[k];i++)
			{
				memcpy(l_SendBuffer+12+i*8, &(nX[k][i]),4);	
				memcpy(l_SendBuffer+16+i*8, &(nY[k][i]),4);	
			}

			l_Send_Client[k].SendInsData_Buffer(l_SendBuffer,nLength);
		}
	}
	//----------------------------------------------------------------------
}
#endif

#if defined MY_SEND || defined MY_RECV
//받은 데이타를 임시로 저장한다. (decoding에서 사용)
//RECV의 한 Sub Frame의 촬상이 완료되면 넣는다. (일단 SEND 에서 주는 데이타 바로 저장해둠)
//SEND에서 보내주는 순간 바로 받아서 처리한다.
void KO_SetKipoPress(int nFrame, int nCount, int *pX, int *pY)
{
	int i;
	double dCalX, dFrameLength, dFOV_D;
	int nX, nY;
	int LINE_NUM_BUFFERS=g_System.m_nImagePart;

	//RECV의 기본정보 ----------------------------------------------------------------
	dCalX=g_Param.m_dCamStartPosX;							// 이카메라의 CalX
	dFrameLength=g_Param.m_dFrameLen;
	dFOV_D=g_System.m_nImageW*g_Param.m_dScaleFactorX/20;   //X는 FOV를 20등분함. 그 한등분의 폭 (mm)
	//--------------------------------------------------------------------------------

	if(g_Kipo.m_nCOSSendCount[0]==0)					//RECV는 SEND 1~3개와 연관되므로 3번중 먼저 받으면 이쪽으로 들어옴.
	{
		g_Kipo.m_nCOSSendCount[0]		= nCount;			
		g_Kipo.m_SlitFrameAtReceive[0]	= nFrame;

		for(i=0;i<nCount;i++)
		{
			g_Kipo.m_nCOSSendX[i]=(int)((pX[i]-dCalX)/dFOV_D); 
			g_Kipo.m_nIsHole[i]=0;
			g_Kipo.m_nCOSSendY[i]=(int)(pY[i]/(dFrameLength/LINE_NUM_BUFFERS));
		}
	}
	else if(g_Kipo.m_nCOSSendCount[1]==0)		//두번째 NEL5로 부터 받으면 이쪽으로 들어옴.
	{
		if(g_Kipo.m_nCOSSendCount[0]+nCount>10) nCount=10-g_Kipo.m_nCOSSendCount[0];  //최대 처리 수는 10개 
		g_Kipo.m_nCOSSendCount[1]			=nCount;			
		g_Kipo.m_SlitFrameAtReceive[1]		=nFrame;

		for(i=0;i<nCount;i++)
		{
			g_Kipo.m_nCOSSendX[i+g_Kipo.m_nCOSSendCount[0]]=(int)((pX[i]-dCalX)/dFOV_D); 
			g_Kipo.m_nIsHole[i+g_Kipo.m_nCOSSendCount[0]]=0;
			g_Kipo.m_nCOSSendY[i+g_Kipo.m_nCOSSendCount[0]]=(int)(pY[i]/(dFrameLength/LINE_NUM_BUFFERS));
		}
	}
	else if(g_Kipo.m_nCOSSendCount[2]==0)		//세번째 NEL5로 부터 받으면 이쪽으로 들어옴.
	{
		if(g_Kipo.m_nCOSSendCount[0]+g_Kipo.m_nCOSSendCount[1]+nCount>10) nCount=10-g_Kipo.m_nCOSSendCount[0]-g_Kipo.m_nCOSSendCount[1];  //최대 처리 수는 10개 
		g_Kipo.m_nCOSSendCount[2]			=nCount;			
		g_Kipo.m_SlitFrameAtReceive[2]		=nFrame;

		for(i=0;i<nCount;i++)
		{
			g_Kipo.m_nCOSSendX[i+g_Kipo.m_nCOSSendCount[0]+g_Kipo.m_nCOSSendCount[1]]=(int)((pX[i]-dCalX)/dFOV_D); 
			g_Kipo.m_nIsHole[i+g_Kipo.m_nCOSSendCount[0]+g_Kipo.m_nCOSSendCount[1]]=0;
			g_Kipo.m_nCOSSendY[i+g_Kipo.m_nCOSSendCount[0]+g_Kipo.m_nCOSSendCount[1]]=(int)(pY[i]/(dFrameLength/LINE_NUM_BUFFERS));
		}
	}
}

//RECV의 한 Frame이 Grab완료된 후 Update
void KO_UpdateKipoPress(int nFrame)
{
	int i,j;
	int nOffset;
	int nX, nY;
	int nYOffset=2;  //(Y 매칭이 정확하면 이값을 더 작게 할 수 있다.) 1은 한 Frame의 1/16 
	double dFrameLength;
	int LINE_NUM_BUFFERS=g_System.m_nImagePart;
	int LINE_BUFFER_HEIGHT=g_System.m_nImagePartH;

	//한 Frame의 촬상이 완료된 상태에서 SEND PC로부터 받은 데이타가 있으면 g_NELRecvPreNG에 넣는다.-------
	if(g_Kipo.m_nCOSSendCount[0]>0)
	{
		nOffset=LINE_NUM_BUFFERS+(LINE_NUM_BUFFERS-(nFrame-g_Kipo.m_SlitFrameAtReceive[0]));     //현재 SubFrame에서 NEL5에서 정보 받을때의 Frame : 한 Frame(16개)를 더해서 데이타를 저장한다.
		for(i=0;i<g_Kipo.m_nCOSSendCount[0];i++)
		{
			nX=g_Kipo.m_nCOSSendX[i];
			nY=g_Kipo.m_nCOSSendY[i]+nOffset;
			if(nX>=0 && nX<24 && nY>=0 && nY<MAX_COS_RECV_PRE_NG) //MAX_NEL2_PRE_NG (400)
			{
				g_Kipo.m_COSRecvPreNG[nY][nX]=1; 
			}
		}
		if(g_Kipo.m_nCOSSendCount[1]>0)
		{
			nOffset=LINE_NUM_BUFFERS+(LINE_NUM_BUFFERS-(nFrame-g_Kipo.m_SlitFrameAtReceive[1]));  //현재 SubFrame에서 NEL5에서 정보 받을때의 Frame : 한 Frame(16개)를 더해서 데이타를 저장한다.
			for(i=g_Kipo.m_nCOSSendCount[0];i<g_Kipo.m_nCOSSendCount[0]+g_Kipo.m_nCOSSendCount[1];i++)
			{
				nX=g_Kipo.m_nCOSSendX[i];
				nY=g_Kipo.m_nCOSSendY[i]+nOffset;
				if(nX>=0 && nX<24 && nY>=0 && nY<MAX_COS_RECV_PRE_NG)
				{
					g_Kipo.m_COSRecvPreNG[nY][nX]=1; 
				}
			}
			if(g_Kipo.m_nCOSSendCount[2]>0)
			{
				nOffset=LINE_NUM_BUFFERS+(LINE_NUM_BUFFERS-(nFrame-g_Kipo.m_SlitFrameAtReceive[2]));  //현재 SlitFrame에서 NEL7에서 정보 받을때의 Frame : 한 Frame(20개)를 더해서 데이타를 저장한다.
				for(i=g_Kipo.m_nCOSSendCount[0]+g_Kipo.m_nCOSSendCount[1];i<g_Kipo.m_nCOSSendCount[0]+g_Kipo.m_nCOSSendCount[1]+g_Kipo.m_nCOSSendCount[2];i++)
				{
					nX=g_Kipo.m_nCOSSendX[i];
					nY=g_Kipo.m_nCOSSendY[i]+nOffset;
					if(nX>=0 && nX<24 && nY>=0 && nY<MAX_COS_RECV_PRE_NG)
					{
						g_Kipo.m_COSRecvPreNG[nY][nX]=1; 
					}
				}
			}
		}
		g_Kipo.m_nCOSSendCount[0]=g_Kipo.m_nCOSSendCount[1]=g_Kipo.m_nCOSSendCount[2]=0;
	}
	//---------------------------------------------------------------------------------------------


	//g_Kipo.m_NEL2PreNG 를 분석해서 현재 검사할 영상에서 NEL7로부터 받은 기포 위치가 있는지 체크한다.----------------------------------------------------------------------
	//아직 한 Frame밀기전 검사해서 체크한 후 한 Frame민다.
	g_Kipo.m_nCurRectCount=0;


	//기포가 있는지 체크한다.---------------------------------------------------------------------------------------------------------------
	for(i=LINE_NUM_BUFFERS-nYOffset;i<LINE_NUM_BUFFERS*2+nYOffset;i++)     //직전 Frame의 마지막 nYOffset 칸과 다음 Frame의 nYOffset 칸까지 서치해서 Kipo가 있으면 처리한다.  
	{
		for(j=0;j<24;j++)
		{
			if(g_Kipo.m_COSRecvPreNG[i][j]==1 && g_Kipo.m_nCurRectCount<4)
			{
				g_Kipo.m_nCurRect[g_Kipo.m_nCurRectCount].left  =g_System.m_nImageW*(j-1)/20;  if(g_Kipo.m_nCurRect[g_Kipo.m_nCurRectCount].left<0)					  g_Kipo.m_nCurRect[g_Kipo.m_nCurRectCount].left=0;
				g_Kipo.m_nCurRect[g_Kipo.m_nCurRectCount].right =g_System.m_nImageW*(j+2)/20;  if(g_Kipo.m_nCurRect[g_Kipo.m_nCurRectCount].right>g_System.m_nImageW) g_Kipo.m_nCurRect[g_Kipo.m_nCurRectCount].right=g_System.m_nImageH-1;
				g_Kipo.m_nCurRect[g_Kipo.m_nCurRectCount].top   =LINE_BUFFER_HEIGHT*(i-nYOffset-1);
				g_Kipo.m_nCurRect[g_Kipo.m_nCurRectCount].bottom=LINE_BUFFER_HEIGHT*(i+nYOffset  );
				g_Kipo.m_nCurRectHole[g_Kipo.m_nCurRectCount]   =0; //눌림사각형
				g_Kipo.m_nCurRectCount++;
				if(g_Kipo.m_nCurRectCount>=4) break;
			}
		}
		if(g_Kipo.m_nCurRectCount>=4) break;
	}

	//SEND PC로부터 받은 데이타가 있으면 한 Frame민다.
	if(g_Kipo.m_nCurRectCount>0)
	{
		for(i=0;i<g_Kipo.m_nCurRectCount;i++)
		{
			g_Kipo.m_nCurRect[i].top   -=g_System.m_nImageH;  if(g_Kipo.m_nCurRect[i].top<0)					 g_Kipo.m_nCurRect[i].top=0;
			g_Kipo.m_nCurRect[i].bottom-=g_System.m_nImageH;  if(g_Kipo.m_nCurRect[i].bottom>g_System.m_nImageW) g_Kipo.m_nCurRect[i].bottom=g_System.m_nImageW-1;
		}
	}
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------


	//g_NELRecvPreNG Array를 한 Frame(20개)만큼 민다. 밀고난 나머지는 0으로 채운다.----------
	for(i=0;i<MAX_COS_RECV_PRE_NG-LINE_NUM_BUFFERS;i++)
	{
		for(j=0;j<24;j++)
			g_Kipo.m_COSRecvPreNG[i][j]=g_Kipo.m_COSRecvPreNG[i+LINE_NUM_BUFFERS][j];
	}
	for(i=MAX_COS_RECV_PRE_NG-LINE_NUM_BUFFERS;i<MAX_COS_RECV_PRE_NG;i++)
		for(j=0;j<24;j++)
			g_Kipo.m_COSRecvPreNG[i][j]=0;
	//-------------------------------------------------------------------------------------
}
#endif

#ifdef MARKING_VISION

void COSS_MoveMarkingData(); //1024씩 촬상할때마다 Call해서 데이타를 8개 Shift시킨다.  8은=>128*8=>1024
int CheckMarkingExist(LPBYTE fm, int N,  double dTh, double *pValue);


void WEB_CheckMarkingData()
{
	LPBYTE fm=g_fmFlat[g_ID];
	int pitch=g_System.m_nPitch;
	int width=g_System.m_nImageW;
	int height=g_System.m_nImageH;
	int i, N, nCount, nEdgeDefect;
	double dValue[2];
	int nX1=g_Temp.m_nInspectEdgeX1;
	int nX2=g_Temp.m_nInspectEdgeX2;
	int nUSEIndex=0;

	  COSS_MoveMarkingData();  //마킹데이타 g_MarkingVision.m_nMarkingData 를 (g_System.m_nImagePart/2) 만큼 Shift시킴
	N=COSS_CheckMarkingROI();  //현재 Frame에서 최대 10개의 ROI를 찾는다.
	
	g_MakringVision.m_rcCnt=N; //DrawMarkingVisionROI 에서 사용하는 데이타 (사각형 영역 그림)
	for(i=0;i<N;i++)
	{
		nUSEIndex=g_MakringVision.m_nInspIndex[i];

		nCount=CheckMarkingExist(fm, i, g_MakringVision.m_dTh, dValue);  //해당 ROI에 마킹이 있는 지 체크

		//영역이 Edge에 걸친 경우-------------------------------------
		nEdgeDefect=0;
		if(nX1>0)
		{
			if(g_MakringVision.m_rcInsp[i].left<nX1) nEdgeDefect=1;
		}
		else if(nX2<width-1)
		{
			if(g_MakringVision.m_rcInsp[i].right>nX2) nEdgeDefect=1;
		}
		//------------------------------------------------------------

		//선이 2개 없으면 불량으로 잡고, 서버에 보낸다.
		if(nCount==0)
		{
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			MakeDefectRect(g_Defect.m_nDefectCount, 0, 0, 0, 1);
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=g_MakringVision.m_MKData[nUSEIndex].m_dX;			//불량위치 X (Pixel)
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=g_MakringVision.m_MKData[nUSEIndex].m_dY;			//불량위치 Y (Pixel)
			if(g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos<0) g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=0;
	
			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=1; //마킹이 2열 모두 없음
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size=0;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX=0;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY=0;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value=dValue[0];
			g_Defect.m_nDefectCount++;
		}
		else if(nCount==1 && nEdgeDefect==0) //ROI가 Edge에 걸친 경우는 1개 검출해도 NG아님
		{
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			MakeDefectRect(g_Defect.m_nDefectCount, 0, 0, 0, 1);
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=g_MakringVision.m_MKData[nUSEIndex].m_dX;		//불량위치 X (Pixel)
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=g_MakringVision.m_MKData[nUSEIndex].m_dY;		//불량위치 Y (Pixel)
			if(g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos<0) g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=0;
	
			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=2; //마킹이 1열만 없음
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size=0;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX=0;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY=0;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value=dValue[0];
			g_Defect.m_nDefectCount++;
		}		

		g_MakringVision.m_nResult[i]=nCount;		
		g_MakringVision.m_nResultData[i][0]=dValue[0];
		g_MakringVision.m_nResultData[i][1]=dValue[1];
		g_MakringVision.m_nResultIndex[i]=g_MakringVision.m_nInspIndex[i];
	}



	//이미 사용한 Index는 비운다---------------------------------------------------------------------
	//.m_nPC 값으로 사용하는 버퍼인지 아직 비어있는 버퍼인지 확인
	if(nUSEIndex>0)
	{
		if(nUSEIndex>g_MakringVision.m_nMKIndex)
		{
			for(i=g_MakringVision.m_nMKIndex;i<nUSEIndex;i++)
				g_MakringVision.m_MKData[i].m_nPC=0;			
		}
		else
		{
			for(i=g_MakringVision.m_nMKIndex;i<MAX_INDEX;i++) g_MakringVision.m_MKData[i].m_nPC=0;
			for(i=1;i<nUSEIndex;i++)						  g_MakringVision.m_MKData[i].m_nPC=0;
		}
	}
	//----------------------------------------------------------------------------------------------

}

//마킹데이타를 16개의 반인 8개씩 Shift 시킨다.
bool   l_nMarkingData[MAX_MKDATA_STORE][MAX_MKDATA_PORT];
void COSS_MoveMarkingData()
{
	memcpy(l_nMarkingData, g_MakringVision.m_nMarkingData, MAX_MKDATA_STORE*MAX_MKDATA_PORT*sizeof(int));
	memcpy(g_MakringVision.m_nMarkingData, l_nMarkingData[g_System.m_nImagePart/2], (MAX_MKDATA_STORE-g_System.m_nImagePart/2)*MAX_MKDATA_PORT*sizeof(int));
}

//현재 Frame에 있는 ROI를 최대 10개 찾음
int COSS_CheckMarkingROI()
{
	int i, j;
	int nROICount=0, nROINextCount=0;
	int N=g_System.m_nImagePart;		//16개
	int nY1, nY2;
	int nJump;
	double dStep=100;
	
	if(g_Param.m_dScaleFactorX) dStep=MARKING_GAP/g_Param.m_dScaleFactorX; //마킹포트 1칸의 Pixel수
	
	

	//200x16(16이 아니고 실제로는 3~13 이정도 g_MakringVision.m_nROIHeight값에 따라 달라짐) 영역에서 마킹 ROI 구함. 최대 10개까지 
	nY1=(g_MakringVision.m_nROIHeight/2)/g_System.m_nImagePartH;
	nY2=N-(g_MakringVision.m_nROIHeight/2)/g_System.m_nImagePartH;

	for(j=0;j<MAX_MKDATA_PORT;j++)
		for(i=nY1;i<=nY2;i++)
		{
			if(g_MakringVision.m_nMarkingData[i][j])  //해당 MAP에 서버로 부터 받은 마킹데이타 있으면 ROI영역 구한다. ROI영역이 Frame범위내에 있으면 마킹체크할 ROI가 됨.
			{		
				g_MakringVision.m_rcInsp[nROICount].left=j*dStep-g_MakringVision.m_nROIOffsetX1; 
				if(g_MakringVision.m_rcInsp[nROICount].left<0) g_MakringVision.m_rcInsp[nROICount].left=0;

				g_MakringVision.m_rcInsp[nROICount].right=g_MakringVision.m_rcInsp[nROICount].left+(g_MakringVision.m_nROIOffsetX1+g_MakringVision.m_nROIOffsetX2);
				if(g_MakringVision.m_rcInsp[nROICount].right>=g_System.m_nImageW) g_MakringVision.m_rcInsp[nROICount].right=g_System.m_nImageW-1;
								
				g_MakringVision.m_rcInsp[nROICount].top   =i*g_System.m_nImagePartH-g_MakringVision.m_nROIHeight/2;
				g_MakringVision.m_rcInsp[nROICount].bottom=g_MakringVision.m_rcInsp[nROICount].top+g_MakringVision.m_nROIHeight;

				g_MakringVision.m_nInspIndex[nROICount]   =g_MakringVision.m_nMarkingData[i][j];

				//ROI가 Frame 상/하 범위를 벗어나지 않는 것만 사용함.
				if(g_MakringVision.m_rcInsp[nROICount].top>=0 && g_MakringVision.m_rcInsp[nROICount].bottom<g_System.m_nImageH)
				{
					if(nROICount<10)
						nROICount++;					
				}

				nJump=(int)((g_MakringVision.m_rcInsp[nROICount].right-(j-1)*dStep)/dStep);
				if(nJump<1) nJump=1;

				j+=nJump;
				break;								
			}
		}

	return nROICount;
}

//해당영역(N)에서 마킹 2열 찾는 함수
#define MK_OFF	4
int CheckMarkingExist(LPBYTE fm, int N, double dTh, double *pValue)
{
	int i,j,k;
	int left, top, right, bottom;
	int width, height;
	int ii, jj;
	int nX, nY, nSum;
	int pitch=g_System.m_nPitch;
	int nTmp,X1, X2;
	int nMin[2], nMinPos[2], nMax[2];
	int OffsetX=10;
	int nMarkCount=0;

	left  =g_MakringVision.m_rcInsp[N].left;
	top   =g_MakringVision.m_rcInsp[N].top;
	right =g_MakringVision.m_rcInsp[N].right;
	bottom=g_MakringVision.m_rcInsp[N].bottom;
	width=right-left;
	height=bottom-top;

	pValue[0]=pValue[1]=0;
	if(g_Param.m_dScaleFactorX>0) OffsetX=5/(MK_OFF*g_Param.m_dScaleFactorX);

	nY=0;
	if(top<0)
	{
		for(i=g_System.m_nImageH+top;i<g_System.m_nImageH-MK_OFF;i+=MK_OFF)
		{
			nX=0;
			for(j=left;j<right-MK_OFF;j+=MK_OFF)
			{
				nSum=0;
				for(ii=i;ii<i+MK_OFF;ii++)
					for(jj=j;jj<j+MK_OFF;jj++)
						nSum+=*(g_fmTemp+pitch*ii+jj);

				*(g_fmTemp1+width*nY+nX)=nSum/(MK_OFF*MK_OFF);
				nX++;
			}
			nY++;
		}
		top=0;
	}

	for(i=top;i<bottom;i+=MK_OFF)
	{
		nX=0;
		for(j=left;j<right;j+=MK_OFF)
		{
			nSum=0;
				for(ii=i;ii<i+MK_OFF;ii++)
					for(jj=j;jj<j+MK_OFF;jj++)
						nSum+=*(fm+pitch*ii+jj);
			*(g_fmTemp1+width*nY+nX)=nSum/(MK_OFF*MK_OFF);
			nX++;
		}
		nY++;
	}

	//(1/4) 압축영상에서 체크
	nMinPos[0]=nMinPos[1]=-1;
	for(k=0;k<2;k++)
	{
		nMin[k]=255*4+1;
		for(i=0;i<nY-1;i++)
		{
			for(j=0;j<nX-1;j++)
			{
				nTmp=*(g_fmTemp1+width*i+j)+*(g_fmTemp1+width*i+j+1)+*(g_fmTemp1+width*(i+1)+j)+*(g_fmTemp1+width*(i+1)+j+1);

				if(nTmp<nMin[k])
				{
					nMin[k]=nTmp;
					nMinPos[k]=j;
				}
			}
		}

		nMax[k]=0;
		j=nMinPos[k];
		for(i=0;i<nY-1;i++)
		{
			nTmp=*(g_fmTemp1+width*i+j)+*(g_fmTemp1+width*i+j+1)+*(g_fmTemp1+width*(i+1)+j)+*(g_fmTemp1+width*(i+1)+j+1);
			if(nTmp>nMax[k]) nMax[k]=nTmp;
		}

		if(k==0)
		{
			X1=nMinPos[k]-OffsetX; if(X1<0)   X1=0;
			X2=nMinPos[k]+OffsetX; if(X2>nX) X2=nX;
			for(i=0;i<nY-1;i++)
				for(j=X1;j<X2;j++)
					*(g_fmTemp1+width*i+j)=255;
		}
	}
	for(k=0;k<2;k++)
	{		
		if(nMinPos[k]>0)
		{			
			pValue[k]=0.25*(nMax[k]-nMin[k]);
			if(pValue[k]>dTh) nMarkCount++;
		}
	}

	return nMarkCount;
}

#endif