#include "stdafx.h"

#ifdef SAVE_NG_THREAD 
	LPBYTE  g_SaveImage[SAVE_NG_NUM];
	CString g_SaveStr[SAVE_NG_NUM];
	int     g_SaveIn=0;
	int     g_SaveOut=0;
	int     g_SaveOver=0;

	CString g_SaveNGStr[MAX_DEFECT+1];
	int     g_SaveNGCnt;

//불량 조각 이미지 저장하는 THREAD
UINT ThreadSaveNGImage(LPVOID lParam)
{
	while(g_Temp.m_nPGStart)
	{		
#ifdef TEST_MODE
		if(1)
#else
		if(g_Temp.m_bAuto)
#endif
		{
			OutputNGImage(); //NG BMP 저장			
			Sleep(1);
		}
		else
		{			
			Sleep(10);
		}
	}
	return 1;
}


//불량작은 영상 Save하기위한 준비
void InitNGSaveThread()
{
	int i;
	for(i=0;i<SAVE_NG_NUM;i++)
	{
		g_SaveImage[i] = new BYTE[BAD_IMG_WIDTH*BAD_IMG_HEIGHT];	//m_pLastImage
	}	

	RGBQUAD   m_bmiColors[256];
	for (i=0; i < 256; i++) 
	{
		m_bmiColors[i].rgbRed = m_bmiColors[i].rgbGreen = m_bmiColors[i].rgbBlue = (BYTE)i;
		m_bmiColors[i].rgbReserved = 0;
	}
	g_SaveNG.Create(BAD_IMG_WIDTH, BAD_IMG_HEIGHT,8);
	g_SaveNG.SetColorTable(0, 256, m_bmiColors);
}

//Close할때 사용
void CloseNGSaveThread()
{
	int i;
	for(i=0;i<SAVE_NG_NUM;i++)
	{
		if(g_SaveImage[i]) delete g_SaveImage[i];
	}
}

//불량발생하면 여기 넣으면 자동으로 저장됨.
void InputNGImage(LPBYTE pNGImage, CString str)
{
	if(g_SaveIn+g_SaveOver*SAVE_NG_NUM>=g_SaveOut && g_SaveIn+g_SaveOver*SAVE_NG_NUM-g_SaveOut<SAVE_NG_NUM)
	{
		memcpy(g_SaveImage[g_SaveIn], pNGImage, BAD_IMG_WIDTH*BAD_IMG_HEIGHT);
		g_SaveStr[g_SaveIn]=str;
		g_SaveIn++;
		if(g_SaveIn>=SAVE_NG_NUM)
		{
			g_SaveIn=0;
			g_SaveOver=1;
		}
	}
}

//불량 작은영상 저장하는 코드
void OutputNGImage()
{
	if(g_SaveIn+g_SaveOver*SAVE_NG_NUM>g_SaveOut)
	{
		LPBYTE dst = (LPBYTE)g_SaveNG.GetBits();
		LPBYTE src = g_SaveImage[g_SaveOut];
		int y, pitch=g_SaveNG.GetPitch();
		for (y=0; y < BAD_IMG_HEIGHT; y++,src+=BAD_IMG_WIDTH,dst+=pitch)
			CopyMemory(dst, src, BAD_IMG_WIDTH);

		g_SaveNG.Save(g_SaveStr[g_SaveOut], Gdiplus::ImageFormatBMP);
		g_SaveOut++;
		if(g_SaveOut>=SAVE_NG_NUM)
		{
			g_SaveOut=0;
			g_SaveOver=0;
		}
	}
}

//불량 String 한줄 쓰는것--------------------------------------------------------
UINT ThreadSaveNGString(LPVOID lParam)
{
	while(g_Temp.m_nPGStart)
	{		
#ifdef TEST_MODE
		if(1)
#else
		if(g_Temp.m_bAuto)
#endif
		{
			if(g_SaveNGCnt)  //NG String있으면 저장
			{
				SaveNGString(g_SaveNGCnt);
				g_SaveNGCnt=0;
			}
		}
		Sleep(1);
	}
	return 1;
}

extern CXManageSocket  l_Send_PVAServer;
void SaveNGString(int nCnt)
{
	int i;
	CString sFileName;
	bool bFileOpen=false;
	double dTemp=0;
	TCHAR sBMPFile[100], sNGFile[100];
	CStdioFile   f;
	ULONGLONG nLine;
	CString strtmp=_T(""),str,strpath,strpc;
	CString txtscr,txtdst;


	CString data[6];
	CTime timer=CTime::GetCurrentTime();
	
	data[0].Format(_T("%d"), timer.GetYear());
	data[1].Format(_T("%02d"), timer.GetMonth());
	data[2].Format(_T("%02d"), timer.GetDay());
	data[3].Format(_T("%02d"), timer.GetHour());
	data[4].Format(_T("%02d"), timer.GetMinute());
	data[5].Format(_T("%02d"), timer.GetSecond());
	
	strpc = g_Temp.m_sMyComName.Right(3);

	strpath.Format(_T("%s%s\\"),LOT_PATH, g_Temp.m_slotName);
	CreateDirectory(strpath,NULL);

	sFileName.Format(_T("%s%s.txt"),strpath,g_Temp.m_sMyComName);
	if(!f.Open(sFileName, CFile::modeWrite|CFile::typeText)) 
	{		
		CFileFind finder;
		if(!finder.FindFile(sFileName))
		{
			if(f.Open(sFileName, CFile::modeCreate|CFile::modeWrite|CFile::typeText))
			{
				bFileOpen=true;
			}
		}
	}
	else
	{
		bFileOpen=true;
	}
	if(!bFileOpen) return;
	nLine=f.SeekToEnd();

	for(i=0;i<nCnt;i++)
	{	
#ifdef INTERFACE_NEXTEYE_SERVER
//		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, g_DefectSend.m_Defect[i].filename, MAX_BADIMAGE_FILENAME, sNGFile, MAX_BADIMAGE_FILENAME); 
		_stprintf(sBMPFile, CA2W(g_DefectSend.m_Defect[i].filename));
		_stprintf(sNGFile, _T("%s\\%s\\%s"), g_Temp.m_slotName, g_Temp.m_sMyComName, sBMPFile);
#else
	#ifdef USE_PACKET_ANSI
		_stprintf(sBMPFile, CA2W(g_DefectSend.m_Defect[i].filename));
		_stprintf(sNGFile, _T("%s\\%s\\%s"), g_Temp.m_slotName, g_Temp.m_sMyComName, sBMPFile);
	#else
		_stprintf(sNGFile, _T("%s\\%s\\%s"), g_Temp.m_slotName, g_Temp.m_sMyComName, g_DefectSend.m_Defect[i].filename);
	#endif
#endif
		

#ifdef SEND_14_DATA

	#ifdef USE_HEX_FORMAT
		strtmp.Format(_T("%s%s%s%s%s%s\t%s\t%d\t%s\t%2.2f\t%d\t%X\t%2.2f\t%2.2f\t%2.3f\t%2.3f\t%s\t%2.2f\t%2.2f\t%2.2f\t%2.2f\t%2.2f\t0.0\t0.0\t0.0\t0.0\t0.0\t0.0\t0.0\t0.0\t0.0\t0.0\t0.0\t0.0\t0.0\t0.0"),
	#else
		strtmp.Format(_T("%s%s%s%s%s%s\t%s\t%d\t%s\t%2.2f\t%d\t%d\t%2.2f\t%2.2f\t%2.3f\t%2.3f\t%s\t%2.2f\t%2.2f\t%2.2f\t%2.2f\t%2.2f\t0.0\t0.0\t0.0\t0.0\t0.0\t0.0\t0.0\t0.0\t0.0\t0.0\t0.0\t0.0\t0.0\t0.0"),
	#endif
					data[0],data[1],data[2],data[3],data[4],data[5],
					g_Temp.m_slotName,g_Temp.m_nGrabFrameID,strpc,0.0,
					i,g_DefectSend.m_Defect[i].defect_class,g_DefectSend.m_Defect[i].x_pos,g_DefectSend.m_Defect[i].y_pos,g_DefectSend.m_Defect[i].sizeX,g_DefectSend.m_Defect[i].sizeY,sNGFile,
					g_DefectSend.m_Defect[i].dTemp[0],	//마킹데이타 0 마킹관련없음  1 마킹함  2 마킹안함 3 가변마킹
					dTemp,g_DefectSend.m_Defect[i].value,g_DefectSend.m_Defect[i].value,g_DefectSend.m_Defect[i].value);
#else
	#ifdef USE_HEX_FORMAT
		strtmp.Format(_T("%s%s%s%s%s%s\t%s\t%d\t%s\t%2.2f\t%d\t%X\t%2.2f\t%2.2f\t%2.3f\t%2.3f\t%s\t%2.2f\t%2.2f\t%2.2f\t%2.2f\t%2.2f"),
	#else
		strtmp.Format(_T("%s%s%s%s%s%s\t%s\t%d\t%s\t%2.2f\t%d\t%d\t%2.2f\t%2.2f\t%2.3f\t%2.3f\t%s\t%2.2f\t%2.2f\t%2.2f\t%2.2f\t%2.2f"),
	#endif
					data[0],data[1],data[2],data[3],data[4],data[5],
					g_Temp.m_slotName,g_Temp.m_nGrabFrameID,strpc,0.0,
					i,g_DefectSend.m_Defect[i].defect_class,g_DefectSend.m_Defect[i].x_pos,g_DefectSend.m_Defect[i].y_pos,g_DefectSend.m_Defect[i].sizeX,g_DefectSend.m_Defect[i].sizeY,sNGFile,
					g_DefectSend.m_Defect[i].dTemp[0],	//마킹데이타 0 마킹관련없음  1 마킹함  2 마킹안함 3 가변마킹
					dTemp,g_DefectSend.m_Defect[i].value,g_DefectSend.m_Defect[i].value,g_DefectSend.m_Defect[i].value);
#endif

#ifdef WEB_TYPE_PVA
		CPacket* packet = new CPacket;
		packet->MakeSubLogDataPacket(strtmp);
		l_Send_PVAServer.SendInsData(packet);
#endif

		if(nLine) 
		{			
			f.WriteString(_T("\n")+strtmp);	//맨처음라인이 아니면 한칸 띄우고 씀					
		}
		else
		{
			f.WriteString(strtmp);	//맨처음이면 바로 쓰고
			nLine=1;
		}
	}
	f.Close();
}
#else
UINT ThreadSaveNGImage(LPVOID lParam) {}
UNIT ThreadSaveNGString(LPVOID lParam) {}
void InitNGSaveThread(){}
void CloseNGSaveThread(){}
void InputNGImage(LPBYTE pImage, CString str){}
void OutputNGImage(){}
void SaveNGString(){}
#endif