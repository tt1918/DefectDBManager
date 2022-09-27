#include "stdafx.h"
#include "KoWebDoc.h"
#include "KoWebView.h"


//서버로부터 받는 것
CXManageSocket  l_Recv_Server;		
CXManageSocket  l_Recv_MarkServer[2];	//마킹서버 1

//서버에 주는것 
CXManageSocket  l_Send_Server;
CXManageSocket  l_Send_PVAServer;
CXManageSocket  l_Send_MarkServer[2];

#ifdef LR_SERVER_EXIST
	CXManageSocket  l_Recv_LRServer;
	CXManageSocket  l_Send_LRServer;
#endif

#ifdef SERVER_TEST
	CXManageSocket  l_Send_ServerTest;
	CXManageSocket  l_Send_MarkServerTest[2];
#endif

CXManageSocket  l_Send_Client[4];
CXManageSocket  l_Recv_Client[4];

#ifdef USE_NITTO_AI
extern void UpdateAiResultData(int nFrame, int ngCount, int aiClass, float* scores);
#endif


void CKoWebView::ConnectUDP()
{
	int i;
	int nIP;
	CString strSrc;
	BOOL bReturn[3]={FALSE, FALSE, FALSE};
	BOOL bReturnLR=FALSE;

	//받는것-------------------------------------------------------------------------------------------------------------
#ifdef WEB_TYPE_PVA
	bReturn[0]=l_Recv_Server.Create((HWND)this->m_hWnd,5130);							//PVA 서버로 부터 받는 것 연결
#else
	bReturn[0]=l_Recv_Server.Create((HWND)this->m_hWnd,DEFAULT_PORT);					//서버로 부터 받는 것 연결
#endif


	if(g_System.m_nMarkServerCnt>0)
		bReturn[1]=l_Recv_MarkServer[0].Create((HWND)this->m_hWnd,MARKING_PORT_RED);	//마킹서버(RED)로 부터 받는 것 연결
	if(g_System.m_nMarkServerCnt>1)
		bReturn[2]=l_Recv_MarkServer[1].Create((HWND)this->m_hWnd,MARKING_PORT_BLUE);	//마킹서버(BLUE)로 부터 받는 것 연결
	
	//주는것-------------------------------------------------------------------------------------------------------------
	nIP=GetIpAddress();
	g_Temp.m_nSelfPort=nIP;

#ifdef WEB_TYPE_PVA
	l_Send_Server.Create((HWND)this->m_hWnd, 5130+nIP,		 _T("130.0.0.254"));	//서버
	l_Send_PVAServer.Create((HWND)this->m_hWnd,9000+nIP,		 _T("130.0.0.250"));	//PAV 서버
#else

	if(g_System.m_nServerType==0)
		l_Send_Server.Create((HWND)this->m_hWnd,DEFAULT_PORT+nIP,			 _T("100.0.0.254"));	//서버
	else
		l_Send_Server.Create((HWND)this->m_hWnd,DEFAULT_PORT+nIP,			 _T("100.0.0.244"));	//서버

#endif


#ifdef SERVER_TEST
	if(g_System.m_nServerTest) l_Send_ServerTest.Create((HWND)this->m_hWnd,DEFAULT_PORT+nIP,		 _T("100.0.0.244"));	//TEST서버
	if(g_System.m_nMarkServerTest[0]) l_Send_MarkServerTest[0].Create((HWND)this->m_hWnd,MARKING_PORT_RED+nIP, _T("100.0.0.243"));	//TEST서버
	if(g_System.m_nMarkServerTest[1]) l_Send_MarkServerTest[1].Create((HWND)this->m_hWnd,MARKING_PORT_BLUE+nIP, _T("100.0.0.242"));	//TEST서버
#endif


#ifdef LR_SERVER_EXIST
	#ifdef TEST_MODE
		g_Temp.m_nSelfPort=2;
	#endif

	bReturnLR=l_Recv_LRServer.Create((HWND)this->m_hWnd,RECV_LR_PORT);											//LR서버에 받을때 PORT 7800    (LR서버는 받는것 1개)
	l_Send_LRServer.Create((HWND)this->m_hWnd,RECV_LR_PORT+g_Temp.m_nSelfPort%30,		 _T("100.0.0.248"));	//LR서버에 줄때 PORT 7801~7  (LR서버 보내는 것 7개)   물때얼룩은 카메라 8개
#endif


	if(g_System.m_nServerType==0)
	{
		if(g_System.m_nMarkServerCnt>0)
			l_Send_MarkServer[0].Create((HWND)this->m_hWnd,MARKING_PORT_RED+nIP,  _T("100.0.0.253"));	///마킹서버(RED)
		if(g_System.m_nMarkServerCnt>1)
			l_Send_MarkServer[1].Create((HWND)this->m_hWnd,MARKING_PORT_BLUE+nIP, _T("100.0.0.252"));	///마킹서버(BLUE)
	}
	else
	{
		if(g_System.m_nMarkServerCnt>0)
			l_Send_MarkServer[0].Create((HWND)this->m_hWnd,MARKING_PORT_RED+nIP,  _T("100.0.0.243"));	///마킹서버(RED)
		if(g_System.m_nMarkServerCnt>1)
			l_Send_MarkServer[1].Create((HWND)this->m_hWnd,MARKING_PORT_BLUE+nIP, _T("100.0.0.242"));	///마킹서버(BLUE)
	}



	//데이타 오는지 계속 CHECK
	if(bReturn[0]) l_Recv_Server.WatchComm();
	if(bReturn[1]) l_Recv_MarkServer[0].WatchComm();
	if(bReturn[2]) l_Recv_MarkServer[1].WatchComm();

#ifdef USE_DAKANO_LINE
	if(bReturnLR)  l_Recv_LRServer.WatchComm();
#endif

#ifdef MY_SEND
	if(g_Temp.m_nPCOptic == SEND_OPTIC)	//보내는 부분 
	{
		for(i=0;i<4;i++)
		{
			if(g_Kipo.m_nPort[i]<=0) continue;
			strSrc.Format(_T("100.0.0.%d"), g_Kipo.m_COSRecvIP[g_Kipo.m_nPort[i]]);						//받을 COS7의 IP (i==0, g_Kipo.m_nPort[0]=1, g_Kipo.m_COSRecvIP[1]=71)
			l_Send_Client[i].Create((HWND)this->m_hWnd,KIPO_PORT+g_Kipo.m_nPort[i], strSrc);		//SEND->RECV로 보냄  (COS7 PC번호에 따라 7301, 7302...7311..)
		}
	}
#endif
#ifdef MY_RECV
	if(g_Temp.m_nPCOptic ==RECV_OPTIC) //내가 COS7이면 다음과 같이 Port를 오픈해 둔다.
	{
		for(i=0;i<4;i++)
		{
			if(g_Kipo.m_nPort[i]<=0) continue;
			if(l_Recv_Client[i].Create((HWND)this->m_hWnd,KIPO_PORT+g_Kipo.m_nPort[i]))			//7301,7302,..7311..  
				l_Recv_Client[i].WatchComm();
		}
	}
#endif

}

void CKoWebView::CloseUDP()
{
	int i;


#ifdef LR_SERVER_EXIST
	l_Recv_LRServer.StopComm();
	l_Recv_LRServer.Close();
	l_Send_LRServer.Close();
#endif

	if(g_System.m_nMarkServerCnt>0) l_Recv_MarkServer[0].StopComm();
	if(g_System.m_nMarkServerCnt>1) l_Recv_MarkServer[1].StopComm();

	l_Recv_Server.Close();
	if(g_System.m_nMarkServerCnt>0) l_Recv_MarkServer[0].Close();
	if(g_System.m_nMarkServerCnt>1) l_Recv_MarkServer[1].Close();

	l_Send_Server.Close();
	l_Send_MarkServer[0].Close();
	l_Send_MarkServer[1].Close();

#ifdef SERVER_TEST
	if(g_System.m_nServerTest) l_Send_ServerTest.Close();	//TEST서버
	if(g_System.m_nMarkServerTest[0]) l_Send_MarkServerTest[0].Close();	//TEST서버
	if(g_System.m_nMarkServerTest[1]) l_Send_MarkServerTest[1].Close();	//TEST서버
#endif

#ifdef WEB_TYPE_PVA
	l_Send_PVAServer.Close();
#endif


#ifdef MY_SEND
	if(g_Temp.m_nPCOptic == SEND_OPTIC)	//보내는 부분 
	{
		for(i=0;i<4;i++)
		{
			if(g_Kipo.m_nPort[i]<=0) continue;
			l_Send_Client[i].Close();
		}
	}
#endif
#ifdef MY_RECV
	if(g_Temp.m_nPCOptic == RECV_OPTIC)
	{
		for(i=0;i<4;i++)
		{
			if(g_Kipo.m_nPort[i]<=0) continue;
			l_Recv_Client[i].StopComm();
			l_Recv_Client[i].Close();
		}
	}
#endif
}


LRESULT CKoWebView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int i;

	switch(message)
	{
		case WM_XDATARRECIEVED: Decoding((int)wParam,(unsigned char*)lParam);
								break;
	}

	return CFormView::WindowProc(message, wParam, lParam);
}


double l_SpliceTime;
int   l_ReceiveMyParam=0;
int   l_ReceiveEndParam=0;
int   l_ReceiveMarkingParam=0;	//마킹데이타 받았으면 1
float l_AiScores[MAX_NITTO_AI_CLASS * MAX_DEFECT];

void CKoWebView::Decoding(int nPort, unsigned char* pData)
{
	CPacket packet;
	int		i, j;
	LPSTR	param_name = NULL;
	double	val = -1;  //TP데이타 받을    Array	
	LPSTR	str_val = NULL;
	CString str, strFRMData;
	int nPortLocal;

	int code = packet.GetCode((char*)pData+4,4);
	int nDataLen;
	int nCOSRecvCount, nMyParam=0;
	double dOffset=0;

	memcpy(&nDataLen, pData, 4);

	CTime t=CTime::GetCurrentTime();
	str.Format(_T("C:%d F:%d [%02d:%02d:%02d]"),code, g_Temp.m_nGrabFrame, t.GetHour(), t.GetMinute(), t.GetSecond());
	GetDlgItem(IDC_STATIC_RECV)->SetWindowText(str);


#ifdef LR_SERVER_EXIST
	if((DEFAULT_PORT+nPort)==RECV_LR_PORT)
	{
		switch(code){
			case LR_GET_DATA:	//특정 Frame 데이타 달라고 하면 준다.
				{
					int nFrame=packet.GetCode((char*)pData+8,4);	//길이+ CODE + nFrame + PORT  (1~7)
					nPortLocal=packet.GetCode((char*)pData+12,4);
					if(nPortLocal!=(g_Temp.m_nSelfPort%10)) return;

					CPacket* packet = new CPacket;
					packet->MakeLRLinePacket(LR_GET_DATA, nFrame);
					l_Send_LRServer.SendInsData(packet);
					delete packet;
				}
				break;
			case LR_CLIENT_CHECK:		
				{
					l_Send_LRServer.SendCommand_LocalHost(LR_CLIENT_CHECK);
					break;
				}
			case LR_PORT_RESET:
				{
					nPortLocal=packet.GetCode((char*)pData+12,4);
					if(nPortLocal!=(g_Temp.m_nSelfPort%10)) return;

					if(g_Temp.m_nPortOpenCount<3)
					{
						l_Send_LRServer.Close();
						l_Send_LRServer.Create((HWND)this->m_hWnd,RECV_LR_PORT+g_Temp.m_nSelfPort%10,		 _T("100.0.0.248"));
						g_Temp.m_nPortOpenCount++;
					}
					break;
				}
		}

		return;
	}
#endif

#ifdef USE_NITTO_AI
	if ((DEFAULT_PORT + nPort) == NITTO_AI_PORT + 1)
	{
		switch (code) {
		case NM_NITTO_AI_RESULT:	
		{
			int frameNum, ngCount, aiClassCount, ai;
			packet.PullAiNgImagePacket((char*)pData, nDataLen, frameNum, ngCount, aiClassCount, l_AiScores);
			UpdateAiResultData(frameNum, ngCount, aiClassCount, l_AiScores);
		}
		break;
		}
		return;
	}
#endif

	switch(code)
	{
	case NM_ACK_ALIVE:	//살았는지 물어보는 것 
		{
			if(nPort==(MARKING_PORT_RED-DEFAULT_PORT))							//살았는지 마킹기(RED)에서 보내오면 마킹기가 대답하고
				l_Send_MarkServer[0].SendCommand_LocalHost(NM_ACK_ALIVE);
			else if(nPort==(MARKING_PORT_BLUE-DEFAULT_PORT))					//살았는지 마킹기(BLUE)에서 보내오면 마킹기가 대답하고
				l_Send_MarkServer[1].SendCommand_LocalHost(NM_ACK_ALIVE);
			else																//살았는지 서버에서 보내오면 서버가 대답한다.
			{
				l_Send_Server.SendCommand_LocalHost(NM_ACK_ALIVE);
#ifdef SERVER_TEST
				if(g_System.m_nServerTest) l_Send_ServerTest.SendCommand_LocalHost(NM_ACK_ALIVE);
				if(g_System.m_nMarkServerTest[0]) l_Send_MarkServerTest[0].SendCommand_LocalHost(NM_ACK_ALIVE);
				if(g_System.m_nMarkServerTest[1]) l_Send_MarkServerTest[1].SendCommand_LocalHost(NM_ACK_ALIVE);
#endif
			}
		}
		break;
	case NM_FULL_IMG_SAVE_100:	//(301) 100장저장 
		{
			//전체영상 100장 촬상
			CossMakeImageDirectoryLot(g_Temp.m_slotName);
			g_Config.m_nImageSaveMethod=2;
			g_Config.m_nImageSaveCount=100;
			g_Temp.m_nSaveCount	=0;
		}
		break;
	case NM_FULL_IMG_SAVE_ALL:	//(302) LOT끝날때 까지 저장(최대 10000)
		{
			//전체영상 10000장 촬상
			CossMakeImageDirectoryLot(g_Temp.m_slotName);
			g_Config.m_nImageSaveMethod=2;
			g_Config.m_nImageSaveCount=10000;
			g_Temp.m_nSaveCountLot=1;
			g_Temp.m_nSaveCount	=0;
		}
		break;
	case NM_LINE_SETTING:
		{
#ifdef USE_DAKANO_LINE
			FuncBaseLineSetting(); //기준이미지(기준라인)설정 및 저장
#endif
		}
		break;
	case NM_KIPOPRESS:
		{
			memcpy(&nCOSRecvCount, (char*)pData+8,  4);  //X좌표 (mm)
	
			//-----------------------------------------------
			//g_Kipo.m_nCOSRecvTotal+=nCOSRecvCount;
			//str.Format(_T("N=%d"), g_Kipo.m_nCOSRecvTotal);
			//GetDlgItem(IDC_STATIC_KIPO)->SetWindowText(str);
			//-----------------------------------------------
		}
		break;

#ifdef TEST_MODE			
	case NM_TESTFRAME:	//서버에서 NextFrame넘어가라고 신호가 옴.
		{
			g_Temp.m_nReceiveServerFrame=1;
			break;
		}
#endif

	case NM_HOLE_INFO:	//서버에서 HOLE정보 보내옴
		{

		}
		break;	
#ifdef USE_DECODING_RESET
	case NM_LOT_RESET:	//이 신호 받으면 Reset
		{
			int flagRest=0;
			CString sFileName;
			sFileName.Format(_T("%s%s\\%s.txt"), LOT_PATH, g_Temp.m_slotName,g_Temp.m_sMyComName);
			for(i=0;i<100;i++)  //1초기다림.
			{
				if(g_SaveNGCnt==0)
				{
					DeleteFile(sFileName);
					InitData();
					ResetCount();
					flagRest=1;
					break;
				}
				Sleep(10);
			}

			if(flagRest==0) //1초동안 못지우면 그냥 강제 지움.
			{
				DeleteFile(sFileName);
				InitData();
				ResetCount();
			}
		}
		break;
#endif
		
	case NM_INS_PRESET:		//서버에서 읽으라는 Message보내면 읽는다. 
		{	
			//받은 확인 보내기 --------------------------------------------------------
			int nOK=1;
			CPacket* packet = new CPacket;
			packet->MakeReturnPacket(NM_SEND_TEACH_FILE, nOK);
			l_Send_Server.SendInsData(packet);
			delete packet;
			//------------------------------------------------------------------------

			ReadParameter(1);		//C:\nexteye\reserve/parameter.ini 에서 복사해서 C:\nexteye\parameter.ini에서 읽기 
		}
		break;
	case NM_LOTDATA_CLEAR:
		{
			str.Format(_T("%sLotData"), MAIN_FOLDER);
			Delete_All_in_Folder(str);
		}
		break;
	case NM_INS_START:
		{
			m_nCheckNewLot = 1; //새로 시작했으니 LOT이름도 받으라는?
			AutoStart();			
		}
		break;
	case NM_INS_STOP:
		{
			AutoStop();
			//ResetTempData();
			g_Temp.m_nFrameAfterTape=10000;			
		}
		break;
	case NM_SPLICE:		//메인 Tape 신호
		{
			if(!g_Temp.m_nTapeSignal[0])
			{
				g_Temp.m_nTapeSignal[0] = TRUE;
				g_Temp.m_nTapeInFrame[0]= g_Temp.m_nGrabFrame;
			}
			if(g_Param.m_nTapeDist[0] >= 1.0)							
				fnWriteSlice(g_Temp.m_nGrabFrame,10);			
		}
		break;
	case NM_TAPE2:	    //보호필름 Tape 신호
		{
			if(!g_Temp.m_nTapeSignal[1])
			{
				g_Temp.m_nTapeSignal[1] = TRUE;
				g_Temp.m_nTapeInFrame[1]= g_Temp.m_nGrabFrame;
			}
			if(g_Param.m_nTapeDist[1] >= 1.0)
				fnWriteSlice(g_Temp.m_nGrabFrame,20);
		}
		break;
	case NM_TAPE3:	    //이형필름 Tape 신호
		{
			if(!g_Temp.m_nTapeSignal[2])
			{
				g_Temp.m_nTapeSignal[2] = TRUE;
				g_Temp.m_nTapeInFrame[2]= g_Temp.m_nGrabFrame;
			}
			if(g_Param.m_nTapeDist[2] >= 1.0)
				fnWriteSlice(g_Temp.m_nGrabFrame,30);
		}
		break;
	
	case MN_DATA_LOT_NAME:	//서버에서 lot name 보내주는 데이터 정상적으로 테이프 검지해서 보내줌 
		 if(g_System.m_nUseCamTapeSensor) break;  //@200824   카메라를 센서로 사용하는 경우  MN_DATA_LOT_NAME 신호 무시함.

	case NM_UNWIND:								  //@200824	  Unwind신호 오면 기존 MN_DATA_LOT_NAME 신호처럼 사용
		{
			if(g_Temp.m_nLotChangeSignal>0) break;
			g_Temp.m_nLotChangeSignal=1;
			g_Temp.m_nLotChangeFrame =g_Temp.m_nGrabFrame;
			g_Temp.m_nLotChangeFMCount=0;


			g_Temp.m_nCkSplice = 2;
			l_SpliceTime=Clock_set();
			packet.PullInstantPacket((char*)pData,nDataLen,code,param_name,val,str_val);

#ifdef INTERFACE_NEXTEYE_SERVER
			CString strNebData(param_name);
#else
			//YKS 20200723
			CString strNebData((TCHAR*)param_name);
#endif

			if(m_nCheckNewLot==1 && strNebData.GetLength()>0)
			{
				g_Temp.m_slotName  = strNebData;
				m_nCheckNewLot    = 0;
				SetSpreadData(3,g_Temp.m_slotName);
				SaveCurrentLotName();
			}
			g_Temp.m_sNewlotName = strNebData;	
			

			if(param_name) delete param_name;
			if(str_val)    delete str_val;
		
			CPacket* packet = new CPacket;
			packet->MakeReturnPacket(NM_CHECK_LOT_CHANGE, g_Temp.m_nGrabFrame+g_Param.m_nTapeDist[0]+g_Param.m_nTapeSkipDist); //서버에 Reset할 Frame수 보내줌
			l_Send_Server.SendInsData(packet);
#ifdef SERVER_TEST
			if(g_System.m_nServerTest) l_Send_ServerTest.SendInsData(packet);
#endif
			delete packet;
			if(code==MN_DATA_LOT_NAME) WriteLog(_T("Lot Change Signal (MN_DATA_LOT_NAME:40)"));
			else					   WriteLog(_T("Lot Change Signal (NM_UNWIND:45)"));
		}	
		break;
	case MN_DATA_LOT_NAME2:	//서버에서 lot name 보내주는 데이터 (3동만 적용)
		{
			if(g_Temp.m_nLotChangeSignal>0) break;
			g_Temp.m_nLotChangeSignal=1;
			g_Temp.m_nLotChangeFrame =g_Temp.m_nGrabFrame;
			g_Temp.m_nLotChangeFMCount=0;
			
			g_Temp.m_nCkSpliceCount = 3;
			l_SpliceTime=Clock_set();
			packet.PullInstantPacket((char*)pData,nDataLen,code,param_name,val,str_val);

#ifdef INTERFACE_NEXTEYE_SERVER
			CString strNebData(param_name);
#else
			//YKS 20200723
			CString strNebData((TCHAR*)param_name);
#endif			

			if(m_nCheckNewLot==1  && strNebData.GetLength()>0)
			{
				g_Temp.m_slotName =  strNebData;
				m_nCheckNewLot = 0;
				SetSpreadData(3,g_Temp.m_slotName);
				SaveCurrentLotName();
			}
			g_Temp.m_sNewlotName =strNebData;	
			
			
			if(param_name) delete param_name;
			if(str_val)    delete str_val;
			
		
			CPacket* packet = new CPacket;
			packet->MakeReturnPacket(NM_CHECK_LOT_CHANGE, g_Temp.m_nGrabFrame+g_Param.m_nTapeDist[0]+g_Param.m_nTapeSkipDist); //서버에 Reset할 Frame수 보내줌
			l_Send_Server.SendInsData(packet);
#ifdef SERVER_TEST
			if(g_System.m_nServerTest) l_Send_ServerTest.SendInsData(packet);
#endif
			delete packet;
			WriteLog(_T("Lot Change Signal (MN_DATA_LOT_NAME2:44)"));
		}
		break;
#ifdef CHECK_LOT_CHANGE_SIGNAL
	case NM_CHECK_LOT_CHANGE:
		{
			//m_nCkSplice이 아니면 이미 LOT 신호를 받았으므로, 이 신호를 받을 필요가 없음.
			if(g_Temp.m_nLotChangeSignal>0) break;

			CPacket* packet = new CPacket;
			packet->MakeReturnPacket(NM_CHECK_LOT_CHANGE, g_Temp.m_nGrabFrame+g_Param.m_nTapeDist[0]+g_Param.m_nTapeSkipDist); //서버에 Reset할 Frame수 보내줌
			l_Send_Server.SendInsData(packet);
#ifdef SERVER_TEST
			if(g_System.m_nServerTest) l_Send_ServerTest.SendInsData(packet);
#endif
			delete packet;
		/*	int nFrame[10];
			
			packet.PullInstantPacket((char*)pData, nDataLen, code, param_name, nFrame);
			CString strNebData(param_name);
			pDoc->m_str_NEWlot = pDoc->m_str_NEWlot_Full = strNebData;
			m_nResetFrameForSplice=nFrame[g_Temp.m_nOptic];

			if(param_name) delete param_name;

			if(m_nResetFrameForSplice>g_frame_num)
			{
				m_nFrameSplice=1;
			}*/
		}
		break;
#endif
	//>>>>>>>>>>>>>밝기조정>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	case NM_CROSS2BRIGHT_ADJUST:
		{
#ifdef USE_3D_AREA
			if(g_Temp.m_nPCOptic == COS1 || g_Temp.m_nPCOptic == COS2)
				g_Temp.m_nAutoAdjustStart=1;
#endif
		}
		break;
	case NM_CROSSBRIGHT_ADJUS:
		{
			if(g_Temp.m_nPCOptic == COS9)
				g_Temp.m_nAutoAdjustStart=1;
		}
		break;
	case NM_CROSSBRIGHT_AUTO: //UNWINDER 신호 받으면 밝기 조정함.
		{
#ifdef USE_3D_AREA
			if(g_Temp.m_nPCOptic == COS1 || g_Temp.m_nPCOptic == COS9)
			{
				g_Temp.m_nAutoAdjustStart=1;
			}
#endif
		}
		break;
	case NM_CROSSBRIGHT_MANUAL:
		{
			if(g_Temp.m_nPCOptic == COS1 || g_Temp.m_nPCOptic == COS9)
			{
			}
		}
		break;

	//정코 COS1에서 사용하는 항목들(NM_MOTION_CHECK, NM_MOTION_CHECK_ONE, NM_EDGE_SEND)----------------
	case NM_MOTION_CHECK:	//Motion 강제 회전.
		{
			g_Temp.m_nMotionCheck = 1;	//자동밝기 조정 들어감
		}
		break;
	case NM_MOTION_CHECK_ONE: //개별 한 PC만 회전
		{
			int port;						

			port = packet.GetCode((char*)pData+8,4);
			if((port%100)!=(g_Temp.m_nSelfPort%100)) break;

			if(g_Temp.m_nPCOptic == COS1)
			{
				g_Temp.m_nMotionCheck = 1;
				g_Temp.m_nManualROT=0;				
			}						
		}
		break;
	case NM_EDGE_SEND: //@@@
		{
			memcpy(&g_Temp.m_nLeading, (char*)pData+8, 4);
	
#ifdef USE_ROTATION
			//리딩에서 검사중으로 바뀌면 자동밝기 실행. 
			if(g_Temp.m_nOldLeading!=g_Temp.m_nLeading)
			{
				if(g_Temp.m_nOldLeading==1 && g_Temp.m_nLeading==0)
					g_Temp.m_nMotionCheck = 1;	
				
				g_Temp.m_nOldLeading=g_Temp.m_nLeading;
			}
#endif

		}
		break;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



//NEW_NG_IMAGE(5)===============================================================================
	case NM_ASK_NG_IMAGE:	//서버로부터 불량영상 보내달라고 명령을 받는다.
		{
			int nFileType;
			int port; 

			port = packet.GetCode((char*)pData+8,4);
			if(port!=g_Temp.m_nSelfPort) break;

			nFileType=packet.GetCode((char*)pData+12,4);
			if(nFileType>0) //파일에서 읽어서 보낸다.  (만약 nFileType==0 이면 마지막 저장된 불량영상 g_Temp.fmNGImage를 보낸다ㅣ)
			{
#ifdef USE_MULTIBYTE

				char sFileName[200]={0,};
				sprintf(sFileName, "%s", (char*)(pData+16));
				LoadBMP(sFileName, g_Temp.fmNGImageLoad, BAD_IMG_WIDTH, BAD_IMG_HEIGHT);

#else

	#ifdef INTERFACE_NEXTEYE_SERVER
				char filename[256]={0,};
				TCHAR filenameW[256]={0,};
				int nLen=strlen((char*)(pData+16));
				sprintf(filename, "%s", (char*)(pData+16));
				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, filename, strlen(filename), filenameW, strlen(filename)); 
				for(i=0;i<nLen-4;i++)
				{
					if(filenameW[i]==_T('.'))
					{
						filenameW[i+4]=NULL;
						break;
					}
				}
				LoadBMP(filenameW, g_Temp.fmNGImageLoad, BAD_IMG_WIDTH, BAD_IMG_HEIGHT);
	#else
				//YKS 20200723
				int strLen = nDataLen - 16;
				char* filename = new char[strLen+2];
				memset(filename, 0, strLen+2);
				memcpy(filename, pData+16, strLen);
				CString s((TCHAR*)filename);
				delete filename;
				LoadBMP(s, g_Temp.fmNGImageLoad, BAD_IMG_WIDTH, BAD_IMG_HEIGHT);
	#endif
#endif		
				//불량영상 보내기 --------------------------------------------------------
				CPacket* packet = new CPacket;
				packet->MakeNGImagePacket(g_Temp.fmNGImageLoad, nFileType, BAD_IMG_WIDTH*BAD_IMG_HEIGHT);
				l_Send_Server.SendInsData(packet);
#ifdef SERVER_TEST
				if(g_System.m_nServerTest) l_Send_ServerTest.SendInsData(packet);
#endif
				delete packet;
				//------------------------------------------------------------------------
			}
			else if(nFileType==0)
			{
				for(i=0;i<10;i++) if(g_Temp.m_FlagNGSave) Sleep(1);

				//불량영상 보내기 --------------------------------------------------------
				CPacket* packet = new CPacket;
					g_Temp.m_FlagNGSave=1;
					packet->MakeNGImagePacket(g_Temp.fmNGImage, nFileType, BAD_IMG_WIDTH*BAD_IMG_HEIGHT);
					g_Temp.m_FlagNGSave=0;
				l_Send_Server.SendInsData(packet);
#ifdef SERVER_TEST
				if(g_System.m_nServerTest) l_Send_ServerTest.SendInsData(packet);
#endif
				delete packet;
				//------------------------------------------------------------------------
			}
		}
		break;
	case NM_SEND_TEACH_FILE: //36
		{
			int nFileType, nOK=0;
			int nReceiveOK=0;
			char sFileName[200];

			nFileType = packet.GetCode((char*)pData+8,4);
					
			if(nFileType==9)
			{
				memcpy(&g_ParamVer1, pData+12, sizeof(PARAMETER_VER1));   //
			
				if(g_ParamVer1.nCurrentSend==1)		// 처음 데이타 받았으면 	
				{				
					l_ReceiveMyParam=l_ReceiveEndParam=0;
					g_ParamVer2.nTotalPort=0;
				}
				if(g_ParamVer1.nTotalSend!=0 && g_ParamVer1.nCurrentSend==g_ParamVer1.nTotalSend) //마지막 부분 받았으면
				{
					l_ReceiveEndParam=1;					
				}
				nMyParam=CheckFileReceive20();		//방금 받은 g_ParamVer1에 해당 카메라 데이타가 있는지 체크			
				if(nMyParam)
					l_ReceiveMyParam=1;

				if(g_ParamVer1.nCurrentSend==2)	
				{
					l_ReceiveMyParam=1;
				}

				CopyParam20();						//g_ParamVer1데이타를 g_ParamVer2에 복사한다.

				//마킹도 받고 자기데이타도 받고 마지막 데이타도 받으면 저장한다(서버에 받았다는 신호 준다)-------
				if((g_System.m_nMarkDataUSE==0 || l_ReceiveMarkingParam) && l_ReceiveMyParam && l_ReceiveEndParam)
				{
					Ini_file_MakingVer3(PARAMETER_PATH_RESERVE, 1);	//g_ParamVer2 저장 (1:마킹데이타 저장)
					nOK=Check_Ini_Parameter(PARAMETER_PATH_RESERVE, g_Temp.m_sMyComName);
					l_ReceiveMarkingParam=l_ReceiveMyParam=l_ReceiveEndParam=0;

					//LOG-----------------------------------------------------------------------------------------------
					int nLen;
					char charData[256];
					TCHAR TcharData[256]={0,};
					sprintf(charData, "%s", g_ParamVer2.ModelName);
					nLen=strlen(charData);
					MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, charData, nLen, TcharData, nLen); 
					if(nLen>0 && nLen<256) 
					{
						str.Format(_T("Received Parameter (%s)"), TcharData);
					}
					else
					{
						str.Format(_T("Received Parameter"));
					}
					WriteLog(str);
					//---------------------------------------------------------------------------------------------------
				}				
				//-----------------------------------------------------------------------------------------------
				
				if(nOK==0) break; //서버에 아무것도 return하지 않는다.
			}			
			else if(nFileType==10)
			{
				memcpy(&g_ParamMark, pData+12, sizeof(PARAMETER_MARK));
				l_ReceiveMarkingParam=1;
				break;
			}
		

			//받은 확인 보내기 --------------------------------------------------------
			CPacket* packet = new CPacket;
			packet->MakeReturnPacket(NM_SEND_TEACH_FILE, nOK);
			l_Send_Server.SendInsData(packet);
			delete packet;
			//------------------------------------------------------------------------
		}
		break;

#ifdef USE_20_LEVEL_MARK
	case NM_SEND_MARK_FILE:
		{
			int nFileType, nOK=0;
			int nReceiveOK=0;
			char sFileName[200];

			nFileType = packet.GetCode((char*)pData+8,4);
					
			if(nFileType==9)
			{
				memcpy(&g_MarkSubNet, pData+12, sizeof(MARKING_SUB_NET));   //
			
				nOK=CopySubMarkParam50(0);
				if(nOK) SaveSubParam();

				//받은 확인 보내기 --------------------------------------------------------
				CPacket* packet = new CPacket;
				packet->MakeReturnPacket(NM_SEND_MARK_FILE, nOK);
				l_Send_Server.SendInsData(packet);
				delete packet;
				//------------------------------------------------------------------------
			}
		}
		break;
#endif

//===========================================================================================
//	case NM_TXTFILE_ASK:
//		{
//			int  nStrSize;
//			char sFileCommand[200];
//			CStdioFile file;
//
//			nStrSize = packet.GetCode((char*)pData+12,4);
//			if(nStrSize>200) break;
//			
//			memcpy(sFileCommand, (char*)(pData+16), nStrSize);
//			
//			if(file.Open("C:\\nexteye\\servercommand.cmd",CFile::modeCreate|CFile::modeWrite|CFile::typeText))
//			{			
//				file.WriteString(sFileCommand);
//				file.Close();
//				Sleep(10);
//				g_Temp.m_Saveflag=1;
//			}			
//		}
//		break;
//
	case NM_DATA_FRAME:	//Server->Frame수 Receive
		{
#ifdef INTERFACE_NEXTEYE_SERVER
			TCHAR sNGFile[160];
			packet.PullInstantPacket((char*)pData,nDataLen,code,param_name,val,str_val);
	
			_stprintf(sNGFile, CA2W(param_name));
			CheckFrame(sNGFile);

#else
			packet.PullInstantPacket((char*)pData,nDataLen,code,param_name,val,str_val);

			//YKS 20200723
			CString s((TCHAR*)(param_name));
			strFRMData = s;
//			strFRMData.Format(_T("%s"), param_name);
	
			CheckFrame(strFRMData);
#endif

			m_tTimeCheckFrameOld = m_tTimeCheckFrame;

			if(param_name) delete param_name;
			if(str_val)    delete str_val;
/*
			packet.PullInstantPacket((char*)pData,nDataLen,code,param_name,val,str_val);
			CString strFRMData(param_name);
			CheckFrame(strFRMData);
			m_tTimeCheckFrameOld = m_tTimeCheckFrame;

			if(param_name) delete param_name;
			if(str_val)    delete str_val;
*/
		}
		break;

	case NM_TEST_SYNC_FRAME:
		{
			g_Temp.m_bTestSyncRecieved = true;
		}
		break;

#if INSPECT_TYPE==TAC_LINE
	case MN_DATA_NEB:	
		{
			//널링 Check PC는 이 데이타를 무시한다.
			if(g_Param.m_nKnullingPC) break; 

			//double값이 1:nulling 유, 0 널릴 무
			packet.PullInstantPacket((char*)pData,nDataLen,code,param_name,val,str_val);
		
			if(val==0)	
			{
				g_Temp.m_nFoundKnulling=0; //리딩
			}
			else if(val==1)
			{
				g_Temp.m_nFoundKnulling=1; //검사원단(양끝에 Knulling있음)
			}
		}
		break;
#endif
	
	default:
		break;
	}
}


int CKoWebView::CheckFrame(CString strpacket)  //@@@NEL-A사용시 수정해야 함.
{
	
	int nPc = g_Temp.m_nPCOptic;
	
	int nMargin=10;
	int nMarginFront=10;
	int nMinMargin=100;
	int nN = 0;
	int nSpiceNoTrace=0;
	double dTime;
	CString sReceiveLotName, str;
				
	dTime=Clock_set();
	m_tTimeCheckFrame  = CTime::GetCurrentTime();


	if(nPc==COS1 || nPc==COS9) nMarginFront*=3;

	if(g_System.m_nUseCamTapeSensor)
	{
		if(g_Temp.m_nGrabFrame>0 && g_Temp.m_nGrabFrame< 100) 
			nSpiceNoTrace=1;	
	}
	else
	{
		if(dTime-l_SpliceTime<120*1000 || (g_Temp.m_nGrabFrame>0 && g_Temp.m_nGrabFrame< 100) ) //Splice신호 후 120초 동안은 Frame 쫒아가지 않는다.  또한 Reset후 100Frame 까지는 Frame 쫒아가지 않는다
			nSpiceNoTrace=1;		
	}

#ifndef USE_ROTATION
	if(g_Temp.m_nResetFlag)
	{
		nSpiceNoTrace=1;
		if(g_Temp.m_nGrabFrame>=20)  g_Temp.m_nResetFlag=0;
	}
#endif

	sReceiveLotName=strpacket.Right(15);
	
	if(g_Temp.m_nLotChangeSignal==0 && g_Temp.m_slotName.Find(sReceiveLotName)<0)	//200722수정  g_Temp.m_nLotChangeSignal==0 추가함. Lot바꾸는 신호이후는 여기서 임의로 바꾸지 않음
	{
		g_Temp.m_slotName=sReceiveLotName;
		SetSpreadData(3,g_Temp.m_slotName);

		//폴더생성
		CossMakeDirectoryLot(g_Temp.m_slotName);
	}

//그대로 사용>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#ifdef INTERFACE_NEXTEYE_SERVER
	#if PROGRAM_TYPE==WEB_10LEVEL
		if(nPc == COS9)			 nN = _ttoi(strpacket.Left(8));		//크로스					0 
		else if(nPc == COS3)	 nN = _ttoi(strpacket.Mid(8,8));	//미분투과					1 
		else if(nPc == COS7)	 nN = _ttoi(strpacket.Mid(8*2,8));	//정반사A					2
		else if(nPc == COS6)	 nN = _ttoi(strpacket.Mid(8*3,8));	//정반사B					4
		else if(nPc == COS4)	 nN = _ttoi(strpacket.Mid(8*4,8));	//정투과					5
		else if(nPc == COS1)	 nN = _ttoi(strpacket.Mid(8*5,8));	//이형크로스				6
		else if(nPc == COS2)	 nN = _ttoi(strpacket.Mid(8*6,8));	//슬릿반사					7
		else if(nPc == COS5)	 nN = _ttoi(strpacket.Mid(8*7,8));	//투영						8
		else if(nPc == COS8)	 nN = _ttoi(strpacket.Mid(8*8,8));	//경계						9
		else if(nPc == COSA)	 nN = _ttoi(strpacket.Mid(8*9,8));	//투영반사					10
		else if(nPc == COSB)	 nN = _ttoi(strpacket.Mid(8*10,8));	//투영반사					10
		else 					 nN = _ttoi(strpacket.Mid(8*4,8));	//정투과						 //@@@
	#elif PROGRAM_TYPE==WEB_DAKANO
		if(nPc == COS9)			 nN = _ttoi(strpacket.Left(8));		//크로스					0 
		else if(nPc == COS3)	 nN = _ttoi(strpacket.Mid(8,8));	//미분투과					1 
		else if(nPc == COS7)	 nN = _ttoi(strpacket.Mid(8*2,8));	//정반사A					2
		else if(nPc == COS8)	 nN = _ttoi(strpacket.Mid(8*3,8));	//경계투과					3  정밀코팅 3D 경계는 이값을 사용한다.(3번째)
		else if(nPc == COS6)	 nN = _ttoi(strpacket.Mid(8,8));	//정반사B					4
		else if(nPc == COS4)	 nN = _ttoi(strpacket.Mid(8*4,8));	//정투과					5
		else if(nPc == COS1)	 nN = _ttoi(strpacket.Mid(8*5,8));	//이형크로스				6
		else if(nPc == COS2)	 nN = _ttoi(strpacket.Mid(8*6,8));	//물때얼룩					7
		else 					 nN = _ttoi(strpacket.Mid(8*4,8));	//정투과						
	#endif
#else
	if(nPc == COS1)			 nN = _ttoi(strpacket.Left(8));	

#if PROGRAM_TYPE==WEB_DAKANO	
	else if(nPc == COS3)	 nN = _ttoi(strpacket.Mid(8,8));	
	else if(nPc == COS2)	 nN = _ttoi(strpacket.Mid(8*2,8));	
#else
	else if(nPc == COS2)	 nN = _ttoi(strpacket.Mid(8,8));	
	else if(nPc == COS3)	 nN = _ttoi(strpacket.Mid(8*2,8));	
#endif

	else if(nPc == COS4)	 nN = _ttoi(strpacket.Mid(8*3,8));	
	else if(nPc == COS5)	 nN = _ttoi(strpacket.Mid(8*4,8));	
	else if(nPc == COS6)	 nN = _ttoi(strpacket.Mid(8*5,8));	
	else if(nPc == COS7)	 nN = _ttoi(strpacket.Mid(8*6,8));	
	else if(nPc == COS8)	 nN = _ttoi(strpacket.Mid(8*7,8));	
	else if(nPc == COS9)	 nN = _ttoi(strpacket.Mid(8*8,8));	
	else if(nPc == COSA)	 nN = _ttoi(strpacket.Mid(8*9,8));	
	else if(nPc == COSB)	 nN = _ttoi(strpacket.Mid(8*10,8));	
	else if(nPc == COSC)	 nN = _ttoi(strpacket.Mid(8*11,8));	
	else if(nPc == COSD)	 nN = _ttoi(strpacket.Mid(8*12,8));		
	else if(nPc == CBCR)	 nN = _ttoi(strpacket.Mid(8*13,8));	
	else if(nPc == CMAK)	 nN = _ttoi(strpacket.Mid(8*14,8));	
	else 					 nN = _ttoi(strpacket.Mid(8*4,8));	
#endif

	if(nN<10)  return 0;


	CTimeSpan ts    = m_tTimeCheckFrame-m_tTimeCheckFrameOld;	//fnLGCheckFrame 함수 한번 들어올때 까지의 시간 (30초정도..)
	CTimeSpan tsend = m_tTimeCheckFrame-g_Temp.m_tLastSendTime; //마지막으로 데이타 보내고 fnLGCheckFrame 들어올때까지 대기 시간 (1~2초)

	if(nN<nMinMargin) return 0;

//	str.Format(_T("g_Temp.m_bAuto=%d g_Temp.m_nGrabFrame=%d  ts:%d  <  tsend:%d"), g_Temp.m_bAuto, g_Temp.m_nGrabFrame,  ts, tsend);
//	AfxMessageBox(str);

	//(현재 STOP상태이고 Frame number 이 0)  또는 (두번 CheckFrame들어 올동안 한번도 Frame number를 서버에 보낸적이 없으면) Frame 맞추고 클라이언트 START시킴.
	if((!g_Temp.m_bAuto && g_Temp.m_nGrabFrame<1) || ts<tsend) 
	{
		AutoStart();
		g_Temp.m_nGrabFrame=nN+1;

		return 0;
	}

	if(nSpiceNoTrace==0) 
	{
		if( (nN-g_Temp.m_nGrabFrame) > nMargin)			 //느리게 가면 바로 맞춘다.
		{
			g_Temp.m_nGrabFrame=nN+1;
		}
		else if(  (g_Temp.m_nGrabFrame-nN)>nMarginFront) //혼자 빨리 가면 좀 천천히 맞춘다.
		{
			g_Temp.m_nGrabFrame=nN+1;
		}
	}
	
	return 1;
}