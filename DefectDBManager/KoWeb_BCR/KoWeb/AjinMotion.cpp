#include "stdafx.h"
#include "AjinMotion.h"

#ifdef COSS_AJIN_MOTION_BOARD
	#include "./AJIN/AXL.h"
	#pragma comment(lib, "./AJIN/AXL.lib")
#endif

#ifdef COSS_CONTEC_USB_IO
	#pragma comment(lib, "./CDIO/CDIO.lib")
	#include "./CDIO/cdio.h"
#endif

#ifdef COSS_AJIN_MOTION_BOARD

MOTIONDATA	 g_Motion;
double       g_dCurrentPos=0;
short		 g_IO_ID=-1;

int LoadMotionData()
{
	int i;
	CString str, filepath;
	TCHAR	cReturn[80];


	filepath.Format(_T("%sMotionConfig.ini"),MAIN_FOLDER);

	g_Motion.m_nHomeStart =GetPrivateProfileInt(_T("SETTING"),_T("HOMESTART"),	0,	filepath); 

	for(i=0;i<AXIS_COUNT;i++)
	{
		str.Format(_T("JOG_MAX_SPEED_AXIS_02d"), i);
		g_Motion.m_dJogMaxSpeed[i]=GetPrivateProfileInt(_T("SETTING"),str,	30,	filepath); 

		str.Format(_T("HOME_OFFSET_AXIS_02d"), i);
		::GetPrivateProfileString(_T("SETTING"),str, _T("0.0"),	cReturn, 20, filepath); 
		g_Motion.m_dHomeOffset[i]=_tstof(cReturn);

		str.Format(_T("MM2PULSE_AXIS_02d"), i);
		g_Motion.m_nMM2Pulse[i]=GetPrivateProfileInt(_T("SETTING"),str,	30,	filepath); 

		str.Format(_T("DONELIMIT_AXIS_02d"), i);
		g_Motion.m_dDoneLimit[i]=GetPrivateProfileInt(_T("SETTING"),str,	30,	filepath); 
		
		
	}
		
	::GetPrivateProfileString(_T("SETTING"),_T("ENCODE_SCALE"), _T("1"),	cReturn, 20, filepath); 
	g_Motion.m_dEncoderscale==_tstof(cReturn);  //한 Pulse가 몇 mm인지

	return 0;
}


int MotionOpen()
{
	int i, ret = 0;
	long lpBoardCount;
	DWORD dwStatus;
	
	g_Temp.m_nMotionError=0;
	if (AxlOpen(7) == AXT_RT_SUCCESS)
	{
		AxmInfoIsMotionModule(&dwStatus);
		if(dwStatus!=STATUS_EXIST)
		{
			AfxMessageBox(_T("모션보드가 없습니다. 확인 하시기 바랍니다"));
			g_Temp.m_nMotionError=1;
		}
//		else
//		{
//			AxlGetBoardCount(&lpBoardCount);
//			if(lpBoardCount!=2)
//			AfxMessageBox("IO보드가 없습니다. 확인 하시기 바랍니다");
//		}


		for(i=0;i<AXIS_COUNT;i++)
		{
			//Unit변경 
			AxmMotSetMoveUnitPerPulse(i, 1, g_Motion.m_nMM2Pulse[i]);  //1MM 에 몇 Pulse
			AxmMotSetMinVel(i,1);
			AxmMotSetPulseOutMethod(i,   OneHighLowHigh);
			AxmMotSetEncInputMethod(i,   ObverseSqr4Mode);
			AxmSignalSetLimit(i,   0, LOW, LOW);
			AxmSignalSetStop(i, 0, 2);
			AxmHomeSetVel(i, 30,10,5,5,300,300);
		}
	}
	else
	{
		AfxMessageBox(_T("모션보드 Open Error!"));
		ret =-1;
	}

	return ret;
}
void MotionClose()
{
	AxlClose();
}

unsigned long Read_USB_IO();
#define MAX_P_LIMIT	134217728			//134217728 pulse마다 1 Cycle
DWORD THREAD_IO_AND_MOTION(LPVOID lpData)
{
	
	static long nOldData;
	double pos, oldPos=0;
	double diff;
	unsigned long nIOInput;
	int nCount=0;
	

	while(g_Temp.m_nPGStart)
	{

#ifdef COSS_CONTEC_USB_IO
		nIOInput=Read_USB_IO();

		if(nIOInput&0x0000001<<g_Temp.m_nRWBitNumber && !(nOldData&0x0000001<<g_Temp.m_nRWBitNumber))		//RW신호 ON
		{			
			Sleep(50);
			nIOInput=Read_USB_IO();
			
			if(nIOInput&0x0000001<<g_Temp.m_nRWBitNumber)  g_Temp.m_nReWinder=1;
		}
		nOldData=nIOInput;
#endif


#ifdef COSS_AJIN_MOTION_BOARD
		AxmStatusGetActPos(0, &pos);  
		if(oldPos>10000 && pos<0)
		{
			pos=(pos+(double)MAX_P_LIMIT/g_Motion.m_nMM2Pulse[0]);
			diff=pos+(MAX_P_LIMIT/g_Motion.m_nMM2Pulse[0]-oldPos);	//0을기준으로 조금 더 간 값 + (Limit-직전위치)
			AxmStatusSetActPos(0, pos);								//0 이거나 0보다 조금 큰값으로 현재위치 재 세팅			
		}
		else
		{
			diff=pos-oldPos;
		}

		if(g_Temp.m_bAuto)// && g_TempData.m_nLeading==0)
		{
			g_dCurrentPos+=(diff*g_Motion.m_dEncoderscale)*0.001;		//M로 환산 , g_MotionConfig.m_dEncoderscale는 1Pulse의 실제길이
			if(g_dCurrentPos<0) g_dCurrentPos=0;
		}
		oldPos=pos;
#endif

		Sleep(10);		
	};
	
	return 0;
}

//Input IO입력 
unsigned long Read_USB_IO()
{
	unsigned long nIOData=0;
	int Ret;

#ifdef COSS_CONTEC_USB_IO
		BYTE	InpPortData;
		if(g_IO_ID>=0)
		{
			Ret = DioInpByte(g_IO_ID, 0, &InpPortData);
			if(Ret==0) nIOData=InpPortData;
		}
#endif

	nIOData=nIOData&0x00FF;
	
	return nIOData;
}

#else

int LoadMotionData() { return 0;}
int MotionOpen() {return 0;}
void MotionClose() { }

#endif