#include "stdafx.h"

//고영우 수정 
int    l_nDataCount;
double l_dMotionData[1000];
double l_dBrightData[1000];
double l_LastSuccess=0;

double Find50Pos(int N, double *pPos, double *pValue, double *pMinValue);
UINT MotorControlThread(LPVOID pParam)
{ 	
	int	   i,j, k, loop;
    double nTargetPos = 0, nCurPos = 0;
	double nBrightness, nBrightnessOld;
	double nMax = 0;
	double nStartPos, nEndPos,nPos, nOldPos, dMinValue;
	int nAverage[2], nBrightSum=0, nBrightCount=0;
	int nIsLeading=0;
	int nOldFrameNumber=0;
	int nBrightMin, nBrightMax;

	nAverage[0]=nAverage[1]=0;

#ifdef USE_ROTATION   
	int nRoungPulse=41250;
	int nStepPulse=500;
	int nStepCount=44;
	int nPrevFrame=0;
	int nMinValuePos;
	int nOffsetMotion=0;
	int loop0Exposure=0;
	char sMoveData[100];
	while (g_Temp.m_nPGStart)
	{
		if(g_Temp.m_nMotionExist==0) //Serial Port를 열지 못하면 더이상 진행하지 않는다.
		{
			Sleep(1000);
			continue;
		}

		// 프로그램이 실행된 상태에서.. Tape 신호가 들어왔을때.. 또는 서버에서 테스트 버튼을 눌렀을때 
		if (g_Temp.m_bAuto && g_Temp.m_nMotionCheck==1)//&& g_Temp.m_nManualROT==0)
		{							
			nBrightMax=0;
			nBrightMin=255;
			for(loop=0;loop<2;loop++)
			{				
				if(loop==0)	//처음 돌릴때
				{					
					//현재 밝기가 128보다 크면 셔터를 조정해서 다시 찾고, 그렇지 않으면 기존 셔터로 찾는다.
					if(g_Temp.m_dAverageBright>=128)							
					{
						if(g_Temp.m_nExposureData>500) g_Grabber.SetExposureTime(500,0);
						else
						{
							//현재보다 셔터를 반으로 낮춘다.
							if(g_Temp.m_nExposureData/2>40)
								g_Grabber.SetExposureTime(g_Temp.m_nExposureData/2, 0);
							else
								g_Grabber.SetExposureTime(40, 0);
						}
						loop0Exposure=g_Temp.m_nExposureData;
					}
					else
					{
						loop0Exposure=g_Temp.m_nExposureData*2;
						if(loop0Exposure>1000) loop0Exposure=1000;
					}
				}
				else        
				{
					if(nBrightMin>128)		   g_Grabber.SetExposureTime(40,   0);			 //180도 최소값이 128보다 크면 셔터를 최대로 낮춘후에 다시 시도
					else if(nBrightMax<128)	   g_Grabber.SetExposureTime(loop0Exposure, 0);	 //180도 최대값이 128보다 작으면 셔터를 2배로 올린후 다시 시도
					else					   g_Grabber.SetExposureTime(500,  0);	
				}
				


				//현재위치에서 180도 뒤로 물림. 0~190도 정도 돌림.
				sprintf(sMoveData,"1FL%6d\r", -21000);
				g_SioMotion.Write((LPBYTE)sMoveData, strlen(sMoveData)); //
				Sleep(1200);
					
				l_dMotionData[0]  =0;
				l_dBrightData[0]=g_Temp.m_dAverageBright;

				for(i=1;i<=nStepCount;i++) //180 도를 21번에 걸쳐서 돌린다.
				{
					//(180/21) 도 이동후 Frame바뀔떄까지 대기----------------------------
					sprintf(sMoveData,"1FL%6d\r", nStepPulse);
					g_SioMotion.Write((LPBYTE)sMoveData, strlen(sMoveData)); // 1000 pulse이동 						
					Sleep(200);
					nPrevFrame=g_Temp.m_nGrabFrame;
					for(j=0;j<30;j++)
					{
						if(g_Temp.m_nGrabFrame!=nPrevFrame) break;
						else Sleep(10);
					} //총 빠르면 0.2초 길면 0.4초 걸림.
					//------------------------------------------------------------------

					l_dMotionData[i]  =i*nStepPulse;
					l_dBrightData[i]=g_Temp.m_dAverageBright;
						
					if(g_Temp.m_dAverageBright>nBrightMax) nBrightMax=g_Temp.m_dAverageBright;
					if(g_Temp.m_dAverageBright<nBrightMin) nBrightMin=g_Temp.m_dAverageBright;
					
					g_Temp.m_dMotionPos=i*nStepPulse;
					g_Temp.m_dMotionBrightness=g_Temp.m_dAverageBright;
					g_Temp.m_nMotionCount=i;
				}
			
				//가장 어두운곳 찾는 상황에서 가장 밝은 값이 128이 안되거나, 가장 어두운것이 128보다 크면 셔터 조정해서 다시한다.---
				if(loop==0)
				{
					if( nBrightMin>128) 
						continue;
				}
				//------------------------------------------------------------------------------------------------------------------

				 g_Temp.m_dMotionTarget=Find50Pos(nStepCount, l_dMotionData, l_dBrightData, &dMinValue);
					
				g_Temp.m_dMotionPos=g_Temp.m_dMotionTarget;
				g_Temp.m_dMotionBrightness=nBrightMax; //Display용으로 사용 (최종Sharpness)
					
				//Target위치로 이동
				sprintf(sMoveData,"1FL%6d\r", (int)g_Temp.m_dMotionTarget-nStepPulse*nStepCount-200);
				g_SioMotion.Write((LPBYTE)sMoveData, strlen(sMoveData));	
				Sleep(1200);

//추가-------------------------------------------------------------------------------------				
				nPrevFrame=g_Temp.m_nGrabFrame;
				for(j=0;j<30;j++)
				{
					if(g_Temp.m_nGrabFrame!=nPrevFrame) break;
					else Sleep(10);
				} 
				dMinValue=g_Temp.m_dAverageBright;
				nMinValuePos=0;
				for(i=1;i<5;i++)
				{
					g_Temp.m_dMotionPos+=100;
					sprintf(sMoveData,"1FL%6d\r", 100);
					g_SioMotion.Write((LPBYTE)sMoveData, strlen(sMoveData));
					Sleep(200);
					nPrevFrame=g_Temp.m_nGrabFrame;
					for(j=0;j<30;j++)
					{
						if(g_Temp.m_nGrabFrame!=nPrevFrame) break;
						else Sleep(10);
					} 
					if(g_Temp.m_dAverageBright<dMinValue)
					{
						dMinValue=g_Temp.m_dAverageBright;
						nMinValuePos=i;
					}
				}
				if(nMinValuePos!=4)
				{
							
					nOffsetMotion=-100*(4-nMinValuePos)+(int)(g_Param.m_dOffsetAngle*nRoungPulse/360);
					g_Temp.m_dMotionPos+=nOffsetMotion;
							
					sprintf(sMoveData,"1FL%6d\r", nOffsetMotion);
					g_SioMotion.Write((LPBYTE)sMoveData, strlen(sMoveData));
					Sleep(200);
				}
				else
				{
					if(g_Param.m_dOffsetAngle!=0)
					{
						nOffsetMotion=(int)(g_Param.m_dOffsetAngle*nRoungPulse/360);
						g_Temp.m_dMotionPos+=nOffsetMotion;

						sprintf(sMoveData,"1FL%6d\r", nOffsetMotion);
						g_SioMotion.Write((LPBYTE)sMoveData, strlen(sMoveData));
						Sleep(200);
					}
				}

				g_Temp.m_dMotionTarget=g_Temp.m_dMotionPos;		
				g_Temp.m_nMotionCheck = 0;
				g_Temp.m_nAutoAdjustStart=1;				//가장 어두운곳 찾은 후 자동 밝기 시작 								
				break;
				//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<		
			}
			
		}
		Sleep(50);
	}
#endif
	
	return 0;

}

// N : Step 수 (데이타 수)
// *pPos: 각 Step에서의 Z축 높이 
// *pValue: 각 Step에서의 Focusing Value
// return : Focusing이 가장 잘 된 높이(Z)
double Find50Pos(int N, double *pPos, double *pValue, double *pMinValue)
{
	int i;
	int nFound=0, minPos=0;
	double minValue=1000;
	double focusPos=0;

	for(i=0;i<N;i++)
		if(pValue[i]<minValue)
		{
			minValue=pValue[i];
			minPos=i;
		}

	focusPos=pPos[minPos];
	*pMinValue=pValue[minPos];
	

	return focusPos;
}