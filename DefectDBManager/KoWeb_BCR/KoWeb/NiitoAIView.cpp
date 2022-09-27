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

void CKoWebView::NittoAI_ConnectServer()
{
#ifdef USE_NITTO_AI	
	m_pNittoAiClient = new NittoAiClient(this->m_hWnd);
	m_pNittoAiClient->Init();
#endif
}
void CKoWebView::NittoAI_Close()
{
#ifdef USE_NITTO_AI	
	if (m_pNittoAiClient) delete  m_pNittoAiClient;
#endif
}
//return  0: 오류 없음
//        -1: 오류
int CKoWebView::NittoAI_SendData(DEFECTDATA* defectData)
{
#ifdef USE_NITTO_AI	
	if (m_pNittoAiClient && m_pNittoAiClient->Inited())
	{
		if (m_pNittoAiClient->Send(defectData))
			return 0;
	}
#endif
	return -1;
}