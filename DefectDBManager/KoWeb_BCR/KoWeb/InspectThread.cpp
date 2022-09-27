#include "stdafx.h"
#include "KoWebDoc.h"
#include "KoWebView.h"
#include <afxmt.h>


bool l_InspectFlag=false;
void CopyGrabImage();					 //Grab한 영상을 복사만 함
void ResultDataSend();					 //직전검사한 데이타를 마킹서버/서버에 전송
int CheckLotChange(CView *pView);		 //Lot Change하는지, 이번 Grab한 이미지 검사하는지 체크
int CheckTapeSignal(int nTape);          //Tape Skip 범위내인지 확인
int CheckUseCamTape(CView *pView);		 //Tape 센서를 사용하지 않는 경우, 카메라로 Tape을 확인해서 LotReset 및 서버에 Tape알림

double l_dInspectStart, l_dInspectEnd;


//서버에 주는것 
extern CXManageSocket  l_Send_Server;
extern CXManageSocket  l_Send_PVAServer;
extern CXManageSocket  l_Send_MarkServer[2];

#ifdef SERVER_TEST
	extern CXManageSocket  l_Send_ServerTest;
	extern CXManageSocket  l_Send_MarkServerTest[2];
#endif

#ifdef LR_SERVER_EXIST
	extern CXManageSocket  l_Send_LRServer;
#endif

#ifdef USE_NITTO_AI
	CCriticalSection  g_csAi;	//AI 데이터 동기화용
	void UpdateAiResultData(int nFrame, int ngCount, int aiClass, float* scores);
#endif


//프로그램 시작과 함께 실행되며 Grab완료되면 검사(또는 Live(화면표시)) 반복----------
UINT THREAD_Auto(LPVOID lParam)
{
	CString sFileName;
	CKoWebView *pView=(CKoWebView*)lParam;
	int i, nNoInspect=0;

	while(g_Temp.m_nPGStart)  
	{			
		if(g_Temp.m_bAuto==0 && g_Temp.m_bLive==0) { Sleep(10);	continue; }  //AUTO도 아니고 Live도 아니면 반복대기
		
		if(g_Temp.m_nGrabDone==0) 
		{
			Sleep(1);
			continue;
		}						 

//TEST		g_Temp.m_nTestCnt[0]++;

		//Grab완료되면 들어오는 구간 (g_Temp.m_nGrabDone==1) >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		g_Temp.m_nGrabDone=0;												 //g_Temp.m_nGrabDone 이 1이면 바로 0을 만들고 아래로 진행


		//Live면 영상만 화면에 표시하고 다시 돌아가서 Grab 대기 -------
		if(g_Temp.m_bLive)			
		{
			pView->ShowGrabImage(); //화면에 표시
			continue; 
		}	
		//-------------------------------------------------------------


		//Lot 변경신호 들어오면 아래 루틴을 탄다------------------------------------------------
		//Lot 변경신호받고 Tape거리지나면 검사안함, Tape거리+Skip거리 지나면 Reset
		nNoInspect=0;
		if(g_Temp.m_nLotChangeSignal)   //LotReset()함수에서 g_Temp.m_nLotChangeSignal는 0이 됨.
		{
			if(g_System.m_nUseCamTapeSensor)		//CAMERASENSING : Tape을 카메라로 센싱
			{
				nNoInspect=CheckUseCamTape(pView);	//1이면 검사안함 : 내부에 Tape찾는 루틴 포함. LotReset
			}
			else									//Tape신호를 받는 경우
			{
				nNoInspect=CheckLotChange(pView);	//1이면 검사안함 : 이 함수는 단지 Frame만 계산해서 LotReset
			}
		}

#ifdef USE_ROTATION
		if(g_Temp.m_nResetFlag && g_Temp.m_nGrabFrame>=20)
		{
			g_Temp.m_nMotionCheck=1;
			g_Temp.m_nResetFlag=0;
		}
#endif

		for(i=0;i<3;i++)
		{
			if(g_Temp.m_nTapeSignal[i])				 //Tape, 보호, 이형
			{
				if(CheckTapeSignal(i)) nNoInspect=1; //검사안함
			}		
		}
//		if(g_Param.m_nNoInspect) nNoInspect=1;
		//--------------------------------------------------------------------------------------
		
		if(nNoInspect==0) //Skip거리 아닐때만 검사한다.
		{		
			if(g_Temp.m_nGrabFrameID>0 && g_System.m_nResultSendMethod==0)
				ResultDataSend();		 //막 Grab이 완료되었으므로 직전 검사한 Frame데이타를 마킹서버/서버로 전송

		#ifdef MY_SEND 
			if( g_Temp.m_nPCOptic==SEND_OPTIC && g_DefectSend.m_nDefectCount>0)
				KO_SendDefect();
		#endif

			SetInspectFlag(true);		//THREAD_Inspect에서 1회 검사 시작을 알림. (l_InspectFlag=true 임)
		}

		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
		Sleep(10); //적어도 10msec는 while loop 탈 이유없음. 즉 초당 100 Frame이상 촬상하지 않으면 Sleep(10)한다고 달라질것 없음
	}
	return 1;
}

//THREAD_Auto에서 검사하라고 하면 검사함. Auto 모드일때만 사용.
UINT THREAD_Inspect(LPVOID lParam)
{
	CString sFileName;
	CKoWebView *pView=(CKoWebView*)lParam;

//	SetThreadAffinityMask(GetCurrentThread(), 1);

	while(g_Temp.m_nPGStart)  
	{			
		if(g_Temp.m_bAuto )
		{
			if(l_InspectFlag)
			{
//TEST				g_Temp.m_nTestCnt[1]++;

				l_dTime[0]=l_dInspectStart=Clock_set();		//Grab후(마킹데이타 있으면 보내고) 검사시작--------------------------------------------------

				if(g_Temp.m_nGrabFrame==0)					//처음 시작하면 1부터 시작함(왜냐하면 Grab 하면 1이 되므로) 그런데 0인 경우는? 
					g_Temp.m_nGrabFrame=1;

				g_Temp.m_nGrabFrameID=g_Temp.m_nGrabFrame;	//g_Temp.m_nGrabFrame은 검사가 길어지면 바뀔 수 있으므로 검사내에서는 g_Temp.m_nGrabFrameID사용
				CopyGrabImage();							//Grab한 영상을 g_fmGrab[g_ID]에 복사함.


				pView->ViewInspect();						//검사 (THREAD_Inspect)
				//좌하단에 보이는 검사시간은 여기까지-----------------------------------------------------------------------------------------------------

			
#if PROGRAM_TYPE==GENERAL_TEACH
				pView->ShowResults_General();
#else
				MakeResultData();							//보낼데이타 만듦.(가변마킹, 불량 한 줄씩 쓰는 것도 여기서 진행 by Thread)
				pView->ShowResults();						//결과화면에 보여주고, 영상 Update (6~7msec) ListBox에 쓰는데 시간이 많이 걸림 (나중에 List Box없애야 함)
#endif

				l_dInspectEnd=Clock_set();

				//결과표시까지포함 검사완료까지 걸리는 시간 (+Alpha)--------------------------------------------------------------------------------------
				//한 Frame검사시간을 최대 50개 까지 보관...g_Temp.m_nImageInfoSendCount 이상 되는 순간 Reset됨. 
				if(g_Temp.m_nImageInfoCount<50)
					g_FrameInfo[g_Temp.m_nImageInfoCount].m_dInspectTime=l_dInspectEnd-l_dInspectStart;
				g_Temp.m_nImageInfoCount++;


				g_Temp.m_dInspTime=l_dInspectEnd-l_dInspectStart;
				
				//아래 5줄은 디버깅용---------------------------------------------------------------------
				if(g_Temp.m_nInspectFrame>10)
				{
					if(g_Temp.m_dInspTime>g_Temp.m_dMaxInspTime) g_Temp.m_dMaxInspTime=g_Temp.m_dInspTime;
					if(g_Temp.m_dInspTime<g_Temp.m_dMinInspTime) g_Temp.m_dMinInspTime=g_Temp.m_dInspTime;
				}
				//----------------------------------------------------------------------------------------


#ifdef USE_NITTO_AI
				//Nitto AI에 결점영상 읽으라는 신호를 보낸다.
				pView->NittoAI_SendData(&g_DefectSend);
#endif


				if(g_System.m_nResultSendMethod==1)				
					ResultDataSend();    //검사결과 바로 보냄.(일반검사는 이렇게, WEB검사는 THREAD_Auto 에서 결과 보냄)
								
				pView->UpdateCount();	 //검사 결과 Update
				pView->UpdateInfo();	 //검사시간, 밝기, Sharpness 등
				
				if(g_Temp.m_nSaveOneFrame==1)  //한 Frame저장 버튼이 눌리면 한장 저장한다.
				{
					g_Temp.m_nSaveOneFrame=0;
					memcpy(g_fmSave, g_fmGrab[g_ID], g_System.m_nImageW*g_System.m_nImageH);
					g_Temp.m_nSaveNumberID=g_Temp.m_nGrabFrameID;					
				}							   //모두저장 또는 불량만 저장이 선택되면 설정된 Frame수만큼만 저장한다.	
				else if(g_Temp.m_nSaveCount<g_Config.m_nImageSaveCount && (g_Config.m_nImageSaveMethod==2 || (g_Config.m_nImageSaveMethod==1 && g_Defect.m_nDefectCount>0)) )
				{					
					memcpy(g_fmSave, g_fmGrab[g_ID], g_System.m_nImageW*g_System.m_nImageH);
					g_Temp.m_nSaveNumberID=g_Temp.m_nGrabFrameID;
					g_Temp.m_nSaveCount++;
				}

#ifdef WEB_DAKANO_MULTAE
				if(g_Param.m_nSaveN<0 || g_Param.m_nSaveN>100)
				{
					g_Param.m_nSaveN=g_ParamVer.param[8];
				}

				if(g_Param.m_nSaveN>0 && g_Temp.m_nGrabFrameID%g_Param.m_nSaveN==0 && g_Temp.m_nNG_MulTae)
				{					
					CompNCopy(g_fmGrab[g_ID], g_fmSaveComp, g_System.m_nImageW, g_System.m_nImageH);
					g_Temp.m_nSaveNumberID_CompImage=g_Temp.m_nGrabFrameID;
				}
#endif

//				if(g_Temp.m_nGrabFrameID<g_Temp.m_nInspectFrame)
//					pView->UpdateInfo();

				SetInspectFlag(false);		//1회 검사 완료를 알림.
			}
		}
		Sleep(1);
	}
	return 1;
}

CCriticalSection  g_CSInsp;
void SetInspectFlag(bool flag)
{
	g_CSInsp.Lock();
	l_InspectFlag=flag;
	g_Temp.m_nONInspecting=flag;
	g_CSInsp.Unlock();
}

//Lot Change하는지, 이번 Grab한 이미지 검사하는지 체크
int CheckLotChange(CView *pView)
{
	int nNoInspect=0;

	g_Temp.m_nLotChangeFMCount++;

	if(g_Temp.m_nGrabFrame-g_Temp.m_nLotChangeFrame >= g_Param.m_nTapeDist[0]+g_Param.m_nTapeSkipDist || g_Temp.m_nLotChangeFMCount>=g_Param.m_nTapeDist[0]+g_Param.m_nTapeSkipDist)
	{
		//Reset한다.
		((CKoWebView *)pView)->LotReset();
		nNoInspect=1;
	}
	else if(g_Temp.m_nGrabFrame-g_Temp.m_nLotChangeFrame >= g_Param.m_nTapeDist[0] || g_Temp.m_nLotChangeFMCount>g_Param.m_nTapeDist[0])
	{
		nNoInspect=1;
	}
	
	return nNoInspect;
}


int CheckTapeSignal(int nTape)
{
	int nNoInspect=0;

	if(g_Temp.m_nGrabFrame-g_Temp.m_nTapeInFrame[nTape] >= g_Param.m_nTapeDist[nTape] &&
	   g_Temp.m_nGrabFrame-g_Temp.m_nTapeInFrame[nTape] <= g_Param.m_nTapeDist[nTape]+g_Param.m_nTapeSkipDist)
	{
		nNoInspect=1;		
	}

	if(g_Temp.m_nGrabFrame-g_Temp.m_nTapeInFrame[nTape] > g_Param.m_nTapeDist[nTape]+g_Param.m_nTapeSkipDist)
	{
		g_Temp.m_nTapeSignal[nTape]=0;
	}
	
	return nNoInspect;
}

//CAMERASENSING  ---------------------------------------------------------------------------------------------------
int IsTapeExist();
int CheckUseCamTape(CView *pView)
{
	int nNoInspect=0;
	static int nFoundFrame=0;

	g_Temp.m_nLotChangeFMCount++;
	if(g_Temp.m_nLotChangeSignal<3)
	{
		if( (g_Temp.m_nGrabFrame-g_Temp.m_nLotChangeFrame >= g_Param.m_nOffsetTapeFrame && g_Temp.m_nGrabFrame-g_Temp.m_nLotChangeFrame<= g_Param.m_nOffsetTapeFrame+g_Param.m_nTapeDist[0]) ||
			(g_Temp.m_nLotChangeFMCount>= g_Param.m_nOffsetTapeFrame && g_Temp.m_nLotChangeFMCount < g_Param.m_nOffsetTapeFrame+g_Param.m_nTapeDist[0]) )            
		{
			g_Temp.m_nLotChangeSignal=2; //Tape검사 범위에 진입.

			if(IsTapeExist() ) //Tape이 찾았으면
			{
				g_Temp.m_nLotChangeSignal=3;		//Tape을 찾았으면 이값이 3이 된다.
				nFoundFrame=g_Temp.m_nGrabFrame;	
				l_Send_Server.SendCommand_LocalHost(NM_FOUNDTAPE);
			}
		}
		else if( (g_Temp.m_nGrabFrame-g_Temp.m_nLotChangeFrame > g_Param.m_nOffsetTapeFrame+g_Param.m_nTapeDist[0]) || 
			     (g_Temp.m_nLotChangeFMCount > g_Param.m_nOffsetTapeFrame+g_Param.m_nTapeDist[0])  )                   //Unwinder신호후 Param52+Param46 보다 크면 Tape찾은 것으로 간주하고 바로 Reset한다.
		{
			g_Temp.m_nLotChangeSignal=3;
			nFoundFrame=g_Temp.m_nGrabFrame-g_Param.m_nTapeSkipDist;
			l_Send_Server.SendCommand_LocalHost(NM_FOUNDTAPE);
		}
	}

	//Tape을 찾았으면 Skip거리지나서 Reset (Lot변경) ---------------------
	if(g_Temp.m_nLotChangeSignal==3)
	{
		nNoInspect=1;
		if(g_Temp.m_nGrabFrame>=nFoundFrame+g_Param.m_nTapeSkipDist)
		{
			((CKoWebView *)pView)->LotReset(); //Reset한다.
		}
	}

	//Unwinder신호후,  다른 클라이언트가 이미 Reset되어서 Frame수가 갑자기 낮아지는 경우, 난 아직 Reset안되어 있으면 Reset한다.
	if(g_Temp.m_nLotChangeSignal>0)
	{
		if(g_Temp.m_nGrabFrame<1000 && g_Temp.m_nGrabFrame-g_Temp.m_nLotChangeFrame<500)   //50M이상 차이남
		{
			l_Send_Server.SendCommand_LocalHost(NM_FOUNDTAPE); //나도 이제 Reset했다고 서버에 알림.
			((CKoWebView *)pView)->LotReset(); //Reset한다.
		}
	}

	return nNoInspect;
}
//----------------------------------------------------------------------------------------------


//정투과 같은데만 사용가능함. (상측 10Pixel 평균값과 하측 10Pixel 평균값의 차이가 g_System.m_nTapeThreshold 이상이면 Tape으로 간주
int IsTapeExist()
{
	int i, j;
	int pitch=g_System.m_nPitch;
	int nWidth=g_System.m_nImageW;
	int nHeight=g_System.m_nImageH;
	long nSumTop=0, nSumBottom=0, nSumMiddle=0;
	double dDiff, dMaxDiff=0;
	int left, right;
	LPBYTE fm=g_Grabber.GrabImagePtr();
	
	//검사범위---------------------------------
	if(g_System.m_nTapeNotArea>0)
	{
		left=g_System.m_nTapeNotArea;
		right=nWidth;
	}
	else if(g_System.m_nTapeNotArea<0)
	{
		left=0;
		right=nWidth+g_System.m_nTapeNotArea;
	}
	else
	{
		left=0;
		right=nWidth;
	}
	if(right-left<=100) return 0;
	//-----------------------------------------

	for(i=0;i<10;i++)
		for(j=left;j<right;j++)
		{
			nSumTop		+=*(fm+pitch*i+j);
			nSumBottom	+=*(fm+pitch*(nHeight-1-i)+j);
			nSumMiddle  +=*(fm+pitch*(i+nHeight/2)+j);
		}

	//맨위-아래 비교
	dMaxDiff=(nSumTop-nSumMiddle)/(10.0*(right-left));
	if(dMaxDiff<0) dMaxDiff*=-1;
	

	//중간-아래 비교
	dDiff=(nSumBottom-nSumMiddle)/(10.0*(right-left));
	if(dDiff<0) dDiff*=-1;
	if(dDiff>dMaxDiff) dMaxDiff=dDiff;

	//중간-위 비교
	dDiff=(nSumTop-nSumBottom)/(10.0*(right-left));
	if(dDiff<0) dDiff*=-1;
	if(dDiff>dMaxDiff) dMaxDiff=dDiff;
	
	if(dMaxDiff>=g_System.m_nTapeThreshold) return 1; //Tape
	else                                    return 0; //Tape아님 
}


void CKoWebView::LotReset()
{	
	InitData();
	ResetCount();


#ifdef SERVER_TEST  //파일 .txt 를 .ini로 복사한다.
	if(g_System.m_nServerTest)
	{
		CString sFileName,sFileName1;
		sFileName.Format(_T("%s%s\\%s.txt"), LOT_PATH, g_Temp.m_slotName,g_Temp.m_sMyComName);


		CreateDirectory(_T("C:\\COSS\\LOG\\"), NULL); 
		CreateDirectory(_T("C:\\COSS\\LOG\\")+g_Temp.m_slotName, NULL);	
		sFileName1.Format(_T("C:\\COSS\\LOG\\%s\\%s.txt"), g_Temp.m_slotName,g_Temp.m_sMyComName);
		CopyFile(sFileName, sFileName1, false);
	}
#endif


//다카노맵 저장하고 Reset함.
#ifdef USE_DAKANO_LINE
	CString sFileName;
	sFileName.Format(_T("%s%s_%d.bmp"),IMAGE_SAVE_PATH, g_Temp.m_slotName, g_Temp.m_nPCID);
	m_pImage->SaveMAP(sFileName, g_fmMAP);

	//"C:\Nexteye\LOTDATA\LotID\NEL-803\LotID_803.bmp"
	sFileName.Format(_T("%s%s\\%s\\%s_%d.bmp"), NG_IMAGE_PATH, g_Temp.m_slotName, g_Temp.m_sMyComName, g_Temp.m_slotName, g_Temp.m_nPCID);
	m_pImage->SaveMAP(sFileName, g_fmMAP);

//	sFileName.Format(_T("\\\\100.0.0.230\\INSP_LOW\\%s%s\\%s\\s_%d.bmp"), NG_IMAGE_PATH, g_Temp.m_slotName, g_Temp.m_sMyComName, g_Temp.m_slotName, g_Temp.m_nPCID);
//	m_pImage->SaveMAP(sFileName, g_fmMAP);

	#if DAKANO_WIDTH==64
		memset(g_fmMAP, 0, 4096*2048);
	#else
		memset(g_fmMAP, 0, 8192*2048);
	#endif

#endif
	WriteLog(_T("LotReset:")+g_Temp.m_slotName);

	//Lot이름 변경 및 각종 폴더 생성-------------
	if(g_Temp.m_sNewlotName.GetLength()>0) 
		  g_Temp.m_slotName=g_Temp.m_sNewlotName;
	SaveCurrentLotName();
	SetSpreadData(3,g_Temp.m_slotName);
	CossMakeDirectoryLot(g_Temp.m_slotName);
	//-------------------------------------------
	ReadParameter(1);		//파라미터 읽기

	SendLotName(g_Temp.m_slotName);  //Cossclean에 Lot정보 보냄.

	g_Temp.m_nResetFlag=1;
}


//촬상한 영상을 화면에 표시
void CKoWebView::ShowGrabImage()
{
	LPBYTE fm=g_Grabber.GrabImagePtr();
	if(m_pImage->m_hWnd)
	{
		memcpy(m_pImage->m_fmDisplay, fm, g_System.m_nImageW*g_System.m_nImageH);
		memcpy(g_fmGrab[g_ID],		  fm, g_System.m_nImageW*g_System.m_nImageH);
		m_pImage->UpdateImage();
	}
}

//Grab한 이미지를 g_fmGrab[g_ID]에 복사함.
void CopyGrabImage()
{
#if GRABBER_TYPE==NO_GRABBER
	if(m_nSimulRun)
	{
		LPBYTE fm=g_Grabber.GrabImagePtr();
		memcpy(g_fmGrab[g_ID],  fm, g_System.m_nImageW*g_System.m_nImageH); 
	}
#else
	LPBYTE fm=g_Grabber.GrabImagePtr();


#ifdef MARKING_VISION 
	if(g_Temp.m_nGrabFrame==1)
	{
		memcpy(g_fmGrab[g_ID],  fm, g_System.m_nImageW*g_System.m_nImageH/2);
		memcpy(g_fmGrab[g_ID]+g_System.m_nImageW*g_System.m_nImageH/2,  fm, g_System.m_nImageW*g_System.m_nImageH/2);
	}
	else
	{
		memcpy(g_fmGrab[g_ID],  g_fmGrab[g_ID]+g_System.m_nImageW*g_System.m_nImageH/2, g_System.m_nImageW*g_System.m_nImageH/2); //나중 1/2 영상을 처음 1/2로 옮기고
		memcpy(g_fmGrab[g_ID]+g_System.m_nImageW*g_System.m_nImageH/2,  fm, g_System.m_nImageW*g_System.m_nImageH/2);			  //방금찍은 1/2을 나중 1/2로 복사한다.	
	}
#else

	#ifdef	USE_NEXT_GRAB		

		memcpy(g_fmSubPrev, g_fmGrab[g_ID]+g_System.m_nImageW*(g_System.m_nImageH-SUB_FRAME_HEIGHT), g_System.m_nImageW*SUB_FRAME_HEIGHT);  //직전 검사에 사용한 이미지의 마지막 128 영역

		memcpy(g_fmGrab[g_ID],  g_fmNext, g_System.m_nImageW*g_System.m_nImageH);	//현재 검사에 사용할 이미지
		memcpy(g_fmNext,  fm, g_System.m_nImageW*g_System.m_nImageH);				//방금 촬상한 이미지를 다음 검사를 위해서 보관

		memcpy(g_fmSubNext, g_fmNext,  g_System.m_nImageW*SUB_FRAME_HEIGHT);		//다음 Frame의 처음 128영역

		if(g_Temp.m_GrabNextDelay==0)		g_Temp.m_GrabNextDelay=1;				//  g_fmSubNext만 사용가능
		else if(g_Temp.m_GrabNextDelay==1)  g_Temp.m_GrabNextDelay=2;				//  g_fmSubNext, g_fmSubPrev 모두 사용가능 

	#else
		memcpy(g_fmGrab[g_ID],  fm, g_System.m_nImageW*g_System.m_nImageH); 
	#endif

#endif


#endif
}

void CKoWebView::UpdateCount()
{
	g_Temp.m_nInspectFrame++;	//검사수 증가
	if(g_Defect.m_nDefectCount>0)
	{
		g_Temp.m_nNGCount+=g_Defect.m_nDefectCount;
		g_Temp.m_nNGFrame++;
	}

	SetSpreadCount();


}


//검사함수
void CKoWebView::ViewInspect()
{
#if PROGRAM_TYPE==GENERAL_TEACH
	OneInspect(g_InspSet);
#else
	WebInspect();
#endif

	if(m_pShowInfo)
		m_pShowInfo->SetResult(g_Temp.m_nLatestResult);
}

//서버에 보낼데이타 만듦 불량 
//가변마킹 체크
//한 줄씩 쓰는 것도 여기서 진행 (g_SaveNGCnt 이 양수가 되면 Thread에서 자동 쓰짐)
void MakeResultData()
{
	int i,j, nDefect, nLevel;
	CString sNGImageName, sNGImageFullName;
	int nDefectCnt=g_Defect.m_nDefectCount;
	CTime t = CTime::GetCurrentTime();
	CString sTime = t.Format("[%H-%M-%S]");

	memcpy(&g_DefectSend, &g_Defect, sizeof(DEFECTDATA));

	//방향에 따라서 x_pos를 mm좌표로 만듦.------------------------------------------------------------------------
	for(i=0;i<nDefectCnt;i++)
	{
		g_DefectSend.m_Defect[i].x_pos=g_Param.m_dCamStartPosX+g_Defect.m_Defect[i].x_pos*g_Param.m_dScaleFactorX;
		g_DefectSend.m_Defect[i].y_pos=g_Param.m_dCamStartPosY+g_Defect.m_Defect[i].y_pos*g_Param.m_dScaleFactorY+g_Param.m_dImageAngleFactorY*g_Defect.m_Defect[i].x_pos; //CAMANGLE  "+g_Param.m_dImageAngleFactorY*g_Defect.m_Defect[i].x_pos" 부분추가함

		g_DefectSend.m_Defect[i].dTemp[0]=g_DefectSend.m_Defect[i].dTemp[1]=2;		

		if(g_Temp.m_nPCOptic==COS4 && g_Param.m_nMarkingLevelChange>0 )
		{
			if(g_Defect.m_Defect[i].size>=2.0)				// 강불량중 Size가 2.0보다 큰것은 마킹설정 Level로 변경해서 마킹기에 보낸다.
			{
				g_DefectSend.m_Defect[i].defect_class=g_Defect.m_Defect[i].defect_class%OPTICDEV+g_Param.m_nMarkingLevelChange*MILLION;  
			}
		}
	}


	//가변마킹 추가 ------------------------------------------
//1msec 이내 처리완료됨.
#ifdef CHANGEABLE_MARKING
	double dCurrentY_M, dCurrentY_mm, dDiffPos, dDefectMaxPos;
	int nType[50], nPeriodDefect, nX;
	float dPosX[50], dPosY[50];
	static double dPrevPos=0;

	if(g_Mark.nUSE[1] && g_Mark.nCheckDefectMCount)
	{
		dCurrentY_M=g_Param.m_dFrameLen*g_Temp.m_nGrabFrame/1000.0; //현재위치 : M단위로 만들어서 보냄.
		if(dCurrentY_M>0)
		{
			if(nDefectCnt)
			{				
				for(i=0;i<nDefectCnt;i++)
				{
					nType[i]=g_DefectSend.m_Defect[i].defect_class;
					dPosX[i]=g_DefectSend.m_Defect[i].x_pos;
					dPosY[i]=g_DefectSend.m_Defect[i].y_pos;
				}
				InsertDefectDataForPeriod(nDefectCnt, nType, dPosX, dCurrentY_M, dPosY);				
			}

			dDiffPos=dCurrentY_M-dPrevPos;
			if(dDiffPos<0) dDiffPos*=-1;

			if(dDiffPos>0.02 || nDefectCnt)
			{		
				dDefectMaxPos=0;
				for(i=0;i<nDefectCnt;i++)
					if(g_DefectSend.m_Defect[i].y_pos>dDefectMaxPos) dDefectMaxPos=g_DefectSend.m_Defect[i].y_pos;

				UpdateMarkingPeriod(dCurrentY_M+dDefectMaxPos/1000.0); //0.1M 이상에 한번씩만 CALL (최대 6msec, 투영경우 2msec이하)
				
				dPrevPos=dCurrentY_M;
			}
		}
	}
	if(g_Mark.nUSE[0] && g_Mark.nCheckLevelMCount)
	{
		dCurrentY_mm=g_Param.m_dFrameLen*g_Temp.m_nGrabFrame;
		if(dCurrentY_mm>0)
		{
			if(nDefectCnt)
			{
				for(i=0;i<nDefectCnt;i++)
				{
					nType[i]=g_DefectSend.m_Defect[i].defect_class;
					dPosY[i]=g_DefectSend.m_Defect[i].y_pos;
				}

				InsertDefectData(nDefectCnt,  nType, dCurrentY_mm, dPosY);
			}							 
			UpdateMarking(dCurrentY_mm);
		}
	}
#endif

	
	for(i=0;i<g_Defect.m_nDefectCount;i++)
	{
		//1차조건: 유저 설정---------------------------------------------------------
		nLevel =g_Defect.m_Defect[i].defect_class/MILLION;
		nDefect=g_Defect.m_Defect[i].defect_class%CLASSDEV;
		if(nLevel>=1 && nLevel<=10 && nDefect>=1 && nDefect<=9) 
		{
			if(g_Mark.nData[nDefect-1][nLevel-1])
				g_DefectSend.m_Defect[i].dTemp[0]=g_DefectSend.m_Defect[i].dTemp[1]=1;
			else if(g_Mark.nMarking[nDefect-1][nLevel-1])
				g_DefectSend.m_Defect[i].dTemp[0]=g_DefectSend.m_Defect[i].dTemp[1]=3;   //가변마킹(수정 1->3)


#ifdef USE_20_LEVEL_MARK
			if(nDefect-1>=0 && nDefect-1<=9 && g_MarkSub.nDefectTypeMarking[nDefect-1])  //20Level 세팅값이 있으면 일단 No Marking으로 설정
			{
				if(g_DefectSend.m_Defect[i].dTemp[0]==1)
				{
					g_DefectSend.m_Defect[i].dTemp[0]=g_DefectSend.m_Defect[i].dTemp[1]=2;
				}
			}

			ApplySubMarking(i);
#endif
		}



		//가변마킹 적용--------------------------------------------------------------
#ifdef CHANGEABLE_MARKING	
		//10Level 이면서 쿠닉, 경계반사는 마킹안함.
		if(nLevel==10 && (g_Defect.m_Defect[i].defect_class%OPTICDEV==12 || g_Defect.m_Defect[i].defect_class%OPTICDEV==92 || ( g_Defect.m_Defect[i].defect_class%OPTICDEV>80 &&  g_Defect.m_Defect[i].defect_class%OPTICDEV<90) || ( g_Defect.m_Defect[i].defect_class%OPTICDEV>60 &&  g_Defect.m_Defect[i].defect_class%OPTICDEV<70)))  //쿠닉과검 10Level일때 마킹하지 않게...
		{
			g_DefectSend.m_Defect[i].dTemp[0]=g_DefectSend.m_Defect[i].dTemp[1]=2;
		}
		else
		{
			//2차조건--------------------------------------
			//주기성 불량과 비슷하게 일정구간(X, Y)에 불량수 체크해서 마킹이면 마킹
			nX=(int)((g_DefectSend.m_Defect[i].x_pos-g_Param.m_dCamStartPosX)/2.0);

			if(g_Mark.nUSE[1] && nX>=0 && nX<l_nMaxX && nDefect>=1 && nDefect<=9)
			{
				if(l_nPeriodMarkingX[nDefect-1][nX])
				{
					if((int)g_DefectSend.m_Defect[i].dTemp[0]%2 != 1)  //마킹이 아니면 가변마킹으로 바꾼다.
						g_DefectSend.m_Defect[i].dTemp[0]=g_DefectSend.m_Defect[i].dTemp[1]=3;  //가변마킹(수정 1->3)
				}
			}			

			//3차조건(주기성불량인지 체크)------------------------
			nPeriodDefect=((int)(g_Defect.m_Defect[i].defect_class/OPTICDEV))%PERIODDEV;
#ifdef USE_20_LEVEL_MARK
			if(nPeriodDefect)
			{
				if(g_Mark.nMarking[9][0] && g_DefectSend.m_Defect[i].dTemp[0]!=5) //[주기:9][Level 1]
					g_DefectSend.m_Defect[i].dTemp[0]=g_DefectSend.m_Defect[i].dTemp[1]=1;
			}
#else
			if(nPeriodDefect)
			{
				if(g_Mark.nMarking[9][0]) //[주기:9][Level 1]
					g_DefectSend.m_Defect[i].dTemp[0]=g_DefectSend.m_Defect[i].dTemp[1]=1;
			}
#endif
		}
#endif



		//-----------------------------------------------------------------------------
	}


	//불량이미지 저장-----------------------------------------------------------------------------------------------------------------------------------------------------
	for(i=0;i<nDefectCnt;i++)
	{
#ifdef USE_HEX_FORMAT
		sNGImageName.Format(_T("%s%s[%05d_%d_%X_%d_%d].bmp"), g_Temp.m_sMyComName, sTime, g_Temp.m_nGrabFrameID, i, g_DefectSend.m_Defect[i].defect_class, (int)g_DefectSend.m_Defect[i].value, (int)((g_DefectSend.m_Defect[i].size+0.0005)*1000));
#else
		sNGImageName.Format(_T("%s%s[%05d_%d_%d_%d_%d].bmp"), g_Temp.m_sMyComName, sTime, g_Temp.m_nGrabFrameID, i, g_DefectSend.m_Defect[i].defect_class, (int)g_DefectSend.m_Defect[i].value, (int)((g_DefectSend.m_Defect[i].size+0.0005)*1000));
#endif



	//서버에 보내기위한 Format으로 변경>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#ifdef INTERFACE_NEXTEYE_SERVER  //받는쪽이 char로 받을때...
//		WideCharToMultiByte(CP_ACP, 0, sNGImageName, MAX_BADIMAGE_FILENAME, g_DefectSend.m_Defect[i].filename, MAX_BADIMAGE_FILENAME, NULL, NULL);
		sprintf(g_DefectSend.m_Defect[i].filename, CW2A(sNGImageName));
#else  //받는쪽이 TCHAR로 받을때는 심플(32bit, 64bit동일)
	#ifdef USE_PACKET_ANSI
		sprintf(g_DefectSend.m_Defect[i].filename, CW2A(sNGImageName));
	#else
		_stprintf(g_DefectSend.m_Defect[i].filename, _T("%s"), sNGImageName);
	#endif
#endif
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#ifndef DONT_SAVE_IMAGE			//불량이미지 저장하는 것 막음(테스트에서 저장하지 못하게), 현장적용에서는 DONT_SAVE_IMAGE 없앰.
		if(g_Temp.m_dHDDspace==0 || g_Temp.m_dHDDspace>15.0)
		{
			sNGImageFullName.Format(_T("%s%s\\%s\\%s"), NG_IMAGE_PATH, g_Temp.m_slotName, g_Temp.m_sMyComName, sNGImageName);
			InputNGImage(g_Defect.m_pImage[i], sNGImageFullName);
		}
#endif
	}


	//서버에 실시간으로 보낼 128x128화면 갖고 있기 (서버가 달라고 하면 준다)-------------------------------------------------
	for(i=0;i<10;i++) if(g_Temp.m_FlagNGSave) Sleep(1);
	g_Temp.m_FlagNGSave=1;
	if(nDefectCnt)
	{
	//	memcpy(g_Temp.fmNGImage, g_Defect.m_pImage[0], BAD_IMG_WIDTH*BAD_IMG_HEIGHT);
		for(i=0;i<BAD_IMG_HEIGHT;i++)
			memcpy(g_Temp.fmNGImage+i*BAD_IMG_WIDTH, g_Defect.m_pImage[0]+(BAD_IMG_HEIGHT-1-i)*BAD_IMG_WIDTH, BAD_IMG_WIDTH);
	}
	g_Temp.m_FlagNGSave=0;
	//-----------------------------------------------------------------------------------------------------------------------


#ifndef DONT_SAVE_DATA			//결과 한줄 쓰는것 막음
	g_SaveNGCnt=nDefectCnt;
#endif
}

#ifdef USE_NITTO_AI
void UpdateAiResultData(int nFrame, int ngCount, int aiClass, float* scores)
{
	int delayIndex = -1;
	int max_index = 0;
	float max_score = 0;
	double score = 0;

	g_csAi.Lock();

	for (int i = 0; i < g_System.m_nResultDaley; i++)
	{
		if (nFrame == g_DefectSendDelay[i].m_nFrameNum &&
			ngCount == g_DefectSendDelay[i].m_nDefectCount)
		{
			delayIndex = i;
			break;
		}
	}
	if (delayIndex != -1)
	{
		DEFECTDATA& defect = g_DefectSendDelay[delayIndex];
		for (int i = 0; i < ngCount; i++)
		{
			max_index = 0;
			max_score = 0;
			for (int j = 0; j < aiClass; j++)
			{
				score = scores[j * ngCount + i];
				if (score > max_score)
				{
					max_score = score;
					max_index = j;
				}
				defect.m_Defect[i].ai_score[j] = score;
			}

			if (max_score > 0.5) //TODO. 여기서 score parameter처리
			{
				if (defect.m_Defect[i].size > 0.6) //TODO. 여기서 ai class별 size조건 처리
				{
					//Ai처리 결과 NG
					defect.m_Defect[i].ai_result = max_index + 1;
				}
				else
				{
					//Ai처리 결과 SKIP
					defect.m_Defect[i].ai_result = max_index + 11;
				}
				//TODO. 기존 마킹유무 변경필요
			}
			else
			{
				//AI처리 결과 스코어 미달
				//기존 마킹유무 유지
				defect.m_Defect[i].ai_result = max_index + 21;
			}
		}
	}
	g_csAi.Unlock();
}
#endif

//직전 Frame검출한 데이타를 마킹서버/서버로 보냄.
double EdgeSort(int count, double *Value);
void ResultDataSend()	
{
	int i, nCount, loop;
	static double start=0;
	static int nGrabFrame=0;
	double end, dElapseTime;
	double dEdge[100];
	int nLevel=0;
	TCHAR  sSendData[256];
	FRAMEINFO frameInfo;


#ifdef RESULT_DELAY

#ifdef USE_NITTO_AI
	g_csAi.Lock();
#endif

	for(i=g_DelayIndex-1;i>=0;i--)
	{
		g_Temp.m_nFoundEdgeDelay[i+1]=g_Temp.m_nFoundEdgeDelay[i];
		memcpy(&g_DefectSendDelay[i+1], &g_DefectSendDelay[i], sizeof(DEFECTDATA));			
	}

	g_Temp.m_nFoundEdgeDelay[0]=g_Temp.m_nFoundEdge;
	memcpy(&g_DefectSendDelay[0], &g_DefectSend, sizeof(DEFECTDATA));	

	if(g_DelayIndex<g_System.m_nResultDaley) g_DelayIndex++;  // g_DelayIndex는 최대 5

#ifdef USE_NITTO_AI
	g_csAi.Unlock();
#endif

	if(g_DelayIndex>=g_System.m_nResultDaley) 
	{
		if(g_DefectSendDelay[g_DelayIndex].m_nDefectCount>0)
		{		
			CPacket* packet = new CPacket;

#ifdef USE_NITTO_AI
			g_csAi.Lock();
#endif
			//전전 Frame불량 데이타로 Packet만듦.			
			packet->MakePacketDataDelay(g_DelayIndex);

#ifdef USE_NITTO_AI
			g_csAi.Unlock();
#endif

			for(loop=0;loop<2;loop++)
			{
				//마킹데이타먼저 보내고
				for(i=0;i<g_System.m_nMarkServerCnt;i++)
				{
					l_Send_MarkServer[i].SendInsData(packet); 
				}
			}

			//서버데이타 보내고
			l_Send_Server.SendInsData(packet); 

	#ifdef SERVER_TEST
			if(g_System.m_nServerTest) l_Send_ServerTest.SendInsData(packet); 
			if(g_System.m_nMarkServerTest[0]) l_Send_MarkServerTest[0].SendInsData(packet); 
			if(g_System.m_nMarkServerTest[1]) l_Send_MarkServerTest[1].SendInsData(packet); 
	#endif
		
	#ifdef LR_SERVER_EXIST
			l_Send_LRServer.SendInsData(packet);
	#endif

	#ifdef WEB_TYPE_PVA
			for(i=0;i<g_DefectSend.m_nDefectCount;i++)
				if(g_DefectSend.m_Defect[i].defect_class/MILLION==1) nLevel=1;
			if(nLevel)
			{
				for(i=0;i<g_DefectSend.m_nDefectCount;i++)
					g_DefectSend.m_Defect[i].dTemp[0]=g_Param.m_dFrameLen;
				l_Send_PVAServer.SendInsData(packet);
			}
	#endif

			delete packet;
		}
		else
		{
			//불량이 없어도 g_Temp.m_nFrameInfoSendCount 마다 한번씩 Frame Number보냄.----------
			if(g_Temp.m_nGrabFrameID%g_Temp.m_nFrameInfoSendCount==0 && g_Temp.m_nGrabFrameID>0)
			{
				CPacket* packet = new CPacket;
				packet->MakePacketDataDelay(g_DelayIndex);

				//서버데이타 보내고
				l_Send_Server.SendInsData(packet); 

	#ifdef SERVER_TEST
				if(g_System.m_nServerTest) l_Send_ServerTest.SendInsData(packet); 
	#endif

	#ifdef LR_SERVER_EXIST
				l_Send_LRServer.SendInsData(packet);
	#endif

				delete packet;
			}
			//----------------------------------------------------------------------------------
		}
	}
#else

	//불량이 있으면 불량데이타를 마킹서버/서버에 보냄 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	if(g_DefectSend.m_nDefectCount>0)
	{				
		CPacket* packet = new CPacket;

		g_Temp.m_nFrameDiff=g_Temp.m_nGrabFrame-g_Temp.m_nGrabFrameID;

		//정상적인 경우는 1 임. 1 초과는 검사시간이 길어져서 다다음 Frame에 데이타를 보내는 것이므로 Frame Length를 빼준다.
		if(g_Temp.m_nFrameDiff>1) 
		{
			//y_pos를 차이나는 Frame만큼 빼줌. (서버는 Y가 -값이 들어와도 사용해야 한다)-----
			for(i=0;i<g_DefectSend.m_nDefectCount;i++)
				g_DefectSend.m_Defect[i].y_pos-=g_Param.m_dFrameLen*(g_Temp.m_nFrameDiff-1);
			//-------------------------------------------------------------------------------
		}


#ifdef USE_20_LEVEL_MARK   
		//투영에 한해서 가변마킹으로 검출된 불량은 마킹할때 LEVEL을 바꿔서 마킹기에 보냄.--------------------------------------------------
		//그냥 마킹은 1, 가변마킹으로 마킹되는 불량은 3, 20Level마킹으로 마킹되는 불량은 5
		if(g_Temp.m_nPCOptic==COS5 && g_Param.m_nNEL5LevelChange>0 && g_Param.m_nNEL5LevelChange<=10)
		{
			for(i=0;i<g_DefectSend.m_nDefectCount;i++)
				if(g_DefectSend.m_Defect[i].dTemp[0]==3)				
					g_DefectSend.m_Defect[i].defect_class=g_DefectSend.m_Defect[i].defect_class%OPTICDEV+g_Param.m_nNEL5LevelChange*MILLION;  
				
		}
#endif


		//불량 데이타로 Packet만듦.
		packet->MakePacketData();

		for(loop=0;loop<2;loop++)
		{
			//마킹데이타먼저 보내고
			for(i=0;i<g_System.m_nMarkServerCnt;i++)
			{
				l_Send_MarkServer[i].SendInsData(packet); 
			}
		}

		//서버데이타 보내고
		l_Send_Server.SendInsData(packet); 

	#ifdef SERVER_TEST
		if(g_System.m_nServerTest) l_Send_ServerTest.SendInsData(packet); 
		if(g_System.m_nMarkServerTest[0]) l_Send_MarkServerTest[0].SendInsData(packet); 
		if(g_System.m_nMarkServerTest[1]) l_Send_MarkServerTest[1].SendInsData(packet); 
	#endif
		
	#ifdef LR_SERVER_EXIST
		l_Send_LRServer.SendInsData(packet);
	#endif

	#ifdef WEB_TYPE_PVA
		for(i=0;i<g_DefectSend.m_nDefectCount;i++)
			if(g_DefectSend.m_Defect[i].defect_class/MILLION==1) nLevel=1;
		if(nLevel)
		{
			for(i=0;i<g_DefectSend.m_nDefectCount;i++)
				g_DefectSend.m_Defect[i].dTemp[0]=g_Param.m_dFrameLen;
			l_Send_PVAServer.SendInsData(packet);
		}
	#endif

		delete packet;		
	}
	else
	{
		//불량이 없어도 g_Temp.m_nFrameInfoSendCount 마다 한번씩 Frame Number보냄.----------
		if(g_Temp.m_nGrabFrameID%g_Temp.m_nFrameInfoSendCount==0 && g_Temp.m_nGrabFrameID>0)
		{
			CPacket* packet = new CPacket;
			g_DefectSend.m_nFrameNum=g_Temp.m_nGrabFrameID;
			packet->MakePacketData();
			l_Send_Server.SendInsData(packet); 

#ifdef SERVER_TEST
			if(g_System.m_nServerTest) l_Send_ServerTest.SendInsData(packet); 
#endif

#ifdef LR_SERVER_EXIST
			l_Send_LRServer.SendInsData(packet);
#endif

			delete packet;
		}
		//----------------------------------------------------------------------------------
	}
#endif





#ifdef USE_SK_BAT
	if(g_Temp.m_dOuterEdge>0 || g_Temp.m_dInnerEdge>0)
	{
		CPacket* packet = new CPacket;
		g_DefectSend.m_nFrameNum=g_Temp.m_nGrabFrameID;
		packet->MakeTwoEdgePacketData(g_Param.m_dCamStartPosX+g_Temp.m_dOuterEdge*g_Param.m_dScaleFactorX, g_Param.m_dCamStartPosX+g_Temp.m_dInnerEdge*g_Param.m_dScaleFactorX);
		l_Send_Server.SendInsData(packet); 
		delete packet;
	}	
#endif

#ifdef LR_SERVER_EXIST
	//프레임 끝나면 데이타 보내기
	CPacket* packetLR = new CPacket;

	//빠진것 있으면 보냄.
	if(g_Temp.m_nGrabFrameID-g_Temp.m_nLRSendFrame-1>g_Temp.m_nLRSendFrameDiff)g_Temp.m_nLRSendFrameDiff=g_Temp.m_nGrabFrameID-g_Temp.m_nLRSendFrame-1;
	if(g_Temp.m_nGrabFrameID>10 && g_Temp.m_nGrabFrameID-g_Temp.m_nLRSendFrame<=5 && g_Temp.m_nGrabFrameID-g_Temp.m_nLRSendFrame>1)
	{
		for(i=g_Temp.m_nLRSendFrame+1;i<g_Temp.m_nGrabFrameID;i++)
		{
			packetLR->MakeLRLinePacket(LR_GET_DATA, i);
			l_Send_LRServer.SendInsData(packetLR);
		}
	}
	
	packetLR->MakeLRLinePacket(LR_GET_DATA, g_Temp.m_nGrabFrameID);
	l_Send_LRServer.SendInsData(packetLR);
	
	g_Temp.m_nLRSendFrame=g_Temp.m_nGrabFrameID;
	delete packetLR;
#endif

	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


	//매 N Frame마다 한번 씩 데이타를 보내기 위한 데이타 준비--------------------------------
	if(g_Temp.m_nImageInfoCount>=g_Temp.m_nImageInfoSendCount && g_Temp.m_nImageInfoCount>0)
	{
		nCount=g_Temp.m_nImageInfoCount;
		if(nCount>50) nCount=50;
		g_Temp.m_nImageInfoCount=0;

		end=Clock_set();
		if(start>0) dElapseTime=end-start;
		else        dElapseTime=0;
		start=end;

		memset(&frameInfo, 0, sizeof(FRAMEINFO));

		for(i=0;i<nCount;i++)
		{
			dEdge[i]=g_FrameInfo[i].m_dEdge;
			frameInfo.m_nMax			+=g_FrameInfo[i].m_nMax;
			frameInfo.m_nMin			+=g_FrameInfo[i].m_nMin;
			frameInfo.m_dAveAllBright	+=g_FrameInfo[i].m_dAveAllBright;
			frameInfo.m_dAvgROIBright	+=g_FrameInfo[i].m_dAvgROIBright;
			frameInfo.m_dSharpness		+=g_FrameInfo[i].m_dSharpness;
			frameInfo.m_dInspectTime	+=g_FrameInfo[i].m_dInspectTime;
		}
		
		frameInfo.m_dEdge				 =EdgeSort(nCount, dEdge);
		frameInfo.m_nMax				/=nCount;
		frameInfo.m_nMin				/=nCount;
		frameInfo.m_dAveAllBright		/=nCount;
		frameInfo.m_dAvgROIBright		/=nCount;
		frameInfo.m_dSharpness			/=nCount;
		frameInfo.m_dInspectTime		/=nCount;

		frameInfo.m_dFPS				 =g_Temp.m_dFPS;
		frameInfo.m_dHDD				 =g_Temp.m_dHDDspace;    

		nGrabFrame=g_Temp.m_nGrabFrameID;

		//서버에만 데이타 보낸다-------------		
		_stprintf(sSendData, _T("%.1f_%.0f_%.0f_%d_%d_%.3f_%.2f_%.2f_%.2f_%.2f_%d"),
			frameInfo.m_dEdge,  //EDGE
			frameInfo.m_dAveAllBright,frameInfo.m_dAvgROIBright,    //Real(전체밝기), ROI영역 밝기 
			frameInfo.m_nMax,frameInfo.m_nMin,
			frameInfo.m_dSharpness,
			frameInfo.m_dInspectTime,
			frameInfo.m_dFPS,
			frameInfo.m_dHDD,
			frameInfo.m_dSharpness,
			g_Temp.m_nExposureData);	

		CPacket* packet = new CPacket;
		packet->MakeInfoPacket(NM_ACK_CAM_AVERAGE, sSendData, frameInfo.m_dAvgROIBright);
		l_Send_Server.SendInsData(packet); 

#ifdef SERVER_TEST
		if(g_System.m_nServerTest) l_Send_ServerTest.SendInsData(packet); 
#endif

#ifdef LR_SERVER_EXIST
		l_Send_LRServer.SendInsData(packet);
#endif

		delete packet;

		g_Temp.m_tLastSendTime = CTime::GetCurrentTime();
	}
}

void CompNCopy(LPBYTE fmS, LPBYTE fmD, int width, int height)
{
	int i, j;
	int N=0;
	int pitch=width;
	int pitchD=width/8;
	LPBYTE fmT;

	for(i=0;i<height;i+=8)
	{
		fmT=fmD+(N++)*pitchD;
		for(j=0;j<width;j+=8)
			*(fmT++)=*(fmS+pitch*i+j);
	}			
}