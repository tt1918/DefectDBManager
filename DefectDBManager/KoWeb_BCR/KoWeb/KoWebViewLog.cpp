#include "stdafx.h"
#include "KoWebDoc.h"
#include "KoWebView.h"

void CKoWebView::fnWriteSlice(int n,int nc)
{
	CTime ttime = CTime::GetCurrentTime();
	CString kk = ttime.Format("[%H-%M-%S]");

	CStdioFile   file;
	CString filename;
	CString strpath;
	CString str;
	int nDay,  nMonth, nChange=0;

	strpath.Format(MAIN_FOLDER);
	filename.Format(_T("%sSlice_COS%d%02d.txt"),strpath,g_Temp.m_nPCOptic, g_Temp.m_nPCNum);
	str.Format(_T("COS%d%02d\t%s\tSplice : %d, frame : %d"),g_Temp.m_nPCOptic, g_Temp.m_nPCNum,kk,nc,n);

	/*	
	nDay=ttime.GetDay();
	if(g_Temp.m_nDayChange && g_Temp.m_nDayChange!=nDay)
	{
		nChangeDay=1;
	}
	g_Temp.m_nDayChange=nDay;
	*/

	nMonth=ttime.GetMonth();
	if(g_Temp.m_nMonthChange && g_Temp.m_nMonthChange!=nMonth)
	{
		nChange=1;
	}
	g_Temp.m_nMonthChange=nMonth;

	if(nChange) //달이 바뀌었으면 새로 처음부터 쓴다.--------------------------------
	{
		if(file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
		{
			file.WriteString(str);	
			file.Close();
		}
	}
	else //같은 날짜이면 이어서 쓴다-------------------------------------------------------
	{
		if(!file.Open(filename, CFile::modeReadWrite|CFile::typeText)) 
		{		
			if(file.Open(filename, CFile::modeCreate|CFile::modeReadWrite|CFile::typeText))
			{
				file.WriteString(str);	
				file.Close();
			}
		}
		else
		{
			file.SeekToEnd();
			file.WriteString(_T("\n"));
			file.WriteString(str);
			file.Close(); 
		}	
	}
}


void CKoWebView::initButtonST(int nType, CButtonST *cButton)
{
	if(nType==0)
	{
		cButton->SetUserFont(&m_BTFont);
		cButton->SetColor(CButtonST::BTNST_COLOR_BK_IN, COLOR_FOCUS);  //마우스 올라갔을때 보이는 것
		cButton->SetColor(CButtonST::BTNST_COLOR_FG_IN, COLOR_BLACK);
		cButton->SetColor(CButtonST::BTNST_COLOR_BK_OUT, COLOR_BACK); //평소보이는 것
		cButton->SetColor(CButtonST::BTNST_COLOR_FG_OUT, COLOR_WHITE);
		cButton->SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, COLOR_YELLOW);
		cButton->SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, COLOR_BLACK);
	}
}
