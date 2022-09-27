// KoWebView.cpp : implementation of the CKoWebView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "KoWeb.h"
#endif

#include "KoWebDoc.h"
#include "KoWebView.h"


LPBYTE l_fmAI;
int    l_ReceiveAIData;

//0.2msec정도 걸림.
void CKoWebView::AI_ConnectServer()
{
#ifdef AI_APPLY	
	m_pAbyssClient = new AbyssClient();
	m_pAbyssClient->ConnectServer(g_System.m_sAIIP, g_System.m_nAIPort);
	g_Temp.m_nConnectAIServer =m_pAbyssClient->m_bConnect; 
#endif
}
void CKoWebView::AI_Close()
{
#ifdef AI_APPLY	
	if(m_pAbyssClient) delete  m_pAbyssClient;
#endif
}
//Size 는 128x128
//nMaxTime : msec
//return  
//        -1: 접속불량
//        -2: Time Out
//		  -3: 영역안맞음
//        -4: 결과없음
int CKoWebView::AI_FindDefectName(LPBYTE fm, int width, int height, int nMaxTime, double *pData)
{
#ifdef AI_APPLY	
	int i, nMaxPos=0;
	double dMax, *pConfidenceArray;
	double startProcess, endProcess;
	int nRet, nAIFound;
	long nCount=0;

	if(width!=128 || height!=128) return -3; //영역안맞음

	startProcess=Clock_set();

	l_ReceiveAIData=0;
	nRet=m_pAbyssClient->Predict(fm, 128,128);	
	for(i=0;i<nMaxTime;i++)
	{
		Sleep(1);
		HandleAllMessage();
		endProcess=Clock_set();
		if(l_ReceiveAIData>0 || endProcess-startProcess>nMaxTime) break;
	}
	
	if(l_ReceiveAIData==2)				////결과없음(보낸결과와 받은 결과가 다름)
	{		 
		return -4; 
	}
	else if(l_ReceiveAIData==0)			//100msec초과 Time OUT
	{
		return -2;  
	}
	

	//정상이면 가장 Socre가 높은 것 구함.--------------------
	if(nRet>0)
	{
		pConfidenceArray=m_pAbyssClient->GetPredictResult();
		if(pConfidenceArray==NULL) return -4;				//데이타가 없는 경우

		dMax=pConfidenceArray[0];
		nMaxPos=1;
		for(i=1;i<5;i++)
			if(pConfidenceArray[i]>dMax)
			{
				dMax=pConfidenceArray[i];
				nMaxPos=i+1;
			}

		if(pData)
		{
			for(i=0;i<5;i++)
				pData[i]=pConfidenceArray[i];
		}
	}

	return nMaxPos;
#else
	return -2;
#endif
}
