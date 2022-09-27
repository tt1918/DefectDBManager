#include "stdafx.h"
#include "math.h"

PARAMETER_VER1	g_ParamVer1;
PARAMETER_VER2  g_ParamVer2;
TCHAR			g_ParamName[NEW_MAX_COL_ALG_ITEM][20];

PARAMETERDATA	g_Param;

#ifdef CHANGEABLE_MARKING
	PARAMETER_MARK	g_ParamMark; //파라미터 서버로 부터 받아서 저장할때만 사용함.
#endif

PARAMETER_MARKOPTIC		g_Mark;
PARAMETER_VER			g_ParamVer;

#ifdef USE_20_LEVEL_MARK
MARKING_SUB_NET			g_MarkSubNet;	//통신으로 받는것
MARKING_SUB_ALL			g_MarkSubAll;	//통신으로 받은 것을 전체로 모음
MARKING_SUB				g_MarkSub;		//이 클라이언트에서 사용하는 것
#endif

void ApplyOptic1();
void ApplyOptic2();
void ApplyOptic3();
void ApplyOptic4();
void ApplyOptic5();
void ApplyOptic6();
void ApplyOptic7();
void ApplyOptic8();
void ApplyOptic9();
void ApplyOpticA();
void ApplyOpticB();
void ApplyOpticC();
void ApplyOpticD();
void ApplyOpticE();
void ApplyOpticF();
void ApplyOpticBCR();
void ApplyOpticMarkingVision();

//[표준]데이타
void ApplyData_Press();			//찍힘
void ApplyData_Scratch();		//일반 스크라치
void ApplyData_LongScratch();		//일반 스크라치
void ApplyData_BlobScratch();	//Blob 스크라치
void ApplyData_Cunic();			//쿠닉
void ApplyData_Line();			//라인검출
void ApplyData_CrossGroup();	//Cross 군집
void ApplyData_CrossKkopChim();	//Cross 꼽침


void ApplyDakanoParameter();    //라인얼룩, 물때얼룩 파라미터 (라인 얼룩은 전용 프로그램 있음)
void ApplyMulTaeParameter();	//물때 파라미터

//Special Data
void LoadKipoPress();
void LoadParamName()
{
	int i, j;
	TCHAR sReturn[20];
	CString str;
	CString appName;
	CString strTmp;


#ifdef INTERFACE_NEXTEYE_SERVER
		appName.Format(_T("NEL%X"), g_Temp.m_nPCOptic);
#else
		if(g_Temp.m_nPCOptic<0x10)
			appName.Format(_T("COS%X"), g_Temp.m_nPCOptic);
		else if(g_Temp.m_nPCOptic==CBCR) //0x10
			appName.Format(_T("BCR"));
		else if(g_Temp.m_nPCOptic==CMAK) //0x11
			appName.Format(_T("MAK"));
#endif


	for(i=0;i<40;i++)
	{
		str.Format(_T("param%d"),i+1);
		::GetPrivateProfileString(appName,str, _T(""),	sReturn, 20, PARAMNAME_PATH); 
		_tcscpy(g_ParamName[i], sReturn);
	}
	for(i=0;i<20;i++)
	{
		str.Format(_T("param%d"),i+1);
		::GetPrivateProfileString(appName,str, _T(""),	sReturn, 20, PARAMGRADENAME_PATH); 
		_tcscpy(g_ParamName[40+i], sReturn);
	}
	for(i=60;i<NEW_MAX_COL_ALG_ITEM;i+=10)
	{
		str.Format(_T("param%d"),(1+i/10-6));
		::GetPrivateProfileString(appName,str, _T(""),	sReturn, 20, PARAMLEVELNAME_PATH); 
		if(_tcslen(sReturn)>0)
		{
			for(j=0;j<10;j++)
			{
				strTmp.Format(_T("%s [Lv%d]"), sReturn,  j+1);
				_tcscpy(g_ParamName[i+j], strTmp);
			}
		}
	}

#ifndef INTERFACE_NEXTEYE_SERVER
	if(g_Temp.m_nPCOptic>=0 && g_Temp.m_nPCOptic<CBCR)  //0x10
	{
		for(i=0;i<10;i++)
		{
			str.Format(_T("NG%d"), i+1);
			::GetPrivateProfileString(appName, str, _T(""),	sReturn, 20, NGNAME_PATH); 
			if(_tcslen(sReturn)>0)
			{
				_tcscpy(g_sDefectName[g_Temp.m_nPCOptic][i+1], sReturn);
			}
		}
	}
#endif

}

int CheckFileReceive20()
{
	//string unicode추가 YKS 20200807
	int i;
	int nIsMyComName=0;
	char    pcname[10];
	TCHAR   pcnameW[10]={0,};

	for(i=0; i<N_CONNECT; i++)
	{
#ifdef INTERFACE_NEXTEYE_SERVER
		sprintf(pcname, "%s", g_ParamVer1.ComName[i]);
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pcname, strlen(pcname), pcnameW, strlen(pcname)); 
#else
		memcpy(pcnameW, g_ParamVer1.ComName[i], sizeof(g_ParamVer1.ComName[i]));
#endif
		if(g_Temp.m_sMyComName.Compare(pcnameW)==0) nIsMyComName=1;
	}

	return nIsMyComName;
}


//10 Level 사용시 서버에서 데이타 날리면 받을때 20개씩 나눠서 받을때 사용함. ------------------------------------------
//g_ParamVer1데이타(최대 N_CONNECT개)를 g_ParamVer2에 복사한다.
void CopyParam20()
{
	//string unicode추가 YKS 20200807	
	int i,j;
	int nSame;
#ifdef INTERFACE_NEXTEYE_SERVER
	char pcname2[10], pcname1[10];
	char *sRet;
#else
	wchar_t pcname2[10], pcname1[10];
	wchar_t *sRet;
#endif
	int nChange=0;


	for(i=0;i<N_CONNECT;i++)
	{
#ifdef INTERFACE_NEXTEYE_SERVER
		sprintf(pcname1, "%s", g_ParamVer1.ComName[i]);	
		if(strlen(pcname1)<7) continue;

		//광학계이름에 "NE" 없으면 넘어감---------------
		sRet=strstr(pcname1, "NE");		
#else
		swprintf(pcname1, L"%s", g_ParamVer1.ComName[i]);
		if(wcslen(pcname1)<7) continue;

#ifdef USE_COSS_SERVER_CLIENT_NEL
		//광학계이름에 "NE" 없으면 넘어감---------------
		sRet=wcsstr(pcname1, L"NE");		
#else
		//광학계이름에 "CO" 없으면 넘어감---------------
		sRet=wcsstr(pcname1, L"CO");		
#endif
#endif
		if(sRet==NULL) continue;

		//방금 받은 광학계와 같은 이름을 찾는다------------
		nSame=-1;
		for(j=0;j<g_ParamVer2.nTotalPort;j++)
		{
#ifdef INTERFACE_NEXTEYE_SERVER
			sprintf(pcname2, "%s", g_ParamVer2.ComName[j]);		
			if(strcmp(pcname1, pcname2)==0) nSame=j;
#else
			swprintf(pcname2, L"%s", g_ParamVer2.ComName[j]);		
			if(wcscmp(pcname1, pcname2)==0) nSame=j;
#endif
		}
		//-------------------------------------------------

		if(nSame>=0) //같은이름이 있으면 방금 받은 데이타로 덮어쓴다.
		{
			memcpy(g_ParamVer2.param[nSame],  g_ParamVer1.param[i],  NEW_MAX_COL_ALG_ITEM*sizeof(float)); //220개 복사
			memcpy(g_ParamVer2.system[nSame], g_ParamVer1.system[i], 8*sizeof(float));
			
			nChange=1;
		}
		else  //같은 이름이 없으면 새로 추가한다.
		{
			if(g_ParamVer2.nTotalPort+1>=MAX_CONNECT) continue;

			memcpy(g_ParamVer2.ComName[g_ParamVer2.nTotalPort], g_ParamVer1.ComName[i], sizeof(g_ParamVer1.ComName[i]));
			memcpy(g_ParamVer2.param[g_ParamVer2.nTotalPort],	g_ParamVer1.param[i],  NEW_MAX_COL_ALG_ITEM*sizeof(float));
			memcpy(g_ParamVer2.system[g_ParamVer2.nTotalPort],  g_ParamVer1.system[i], 8*sizeof(float));

			g_ParamVer2.nTotalPort++;
			nChange=1;
		}
	}

	if(nChange)
	{
		memcpy(g_ParamVer2.MachineName, g_ParamVer1.MachineName, sizeof(g_ParamVer1.MachineName));
		memcpy(g_ParamVer2.ModelName,   g_ParamVer1.ModelName, sizeof(g_ParamVer1.ModelName));
		memcpy(g_ParamVer2.nCycle,      g_ParamVer1.nCycle, 15*sizeof(int));
	}
}
//-------------------------------------------------------------------------------------------------------------


int Check_Ini_Parameter(CString filename, CString str_name )
{
	int nRet=0;

	CString str, sParam;
	CString ModelName, strModel;
	CString sParameterPath;
	str.Format(_T("%s - Inspect Parameter"), str_name);
	int i, j, k, nTemp, nOptic;
	double dTemp;
	int nCnt, nTmp;
	double dParam[220];
	int nCycle[15];
	double  dSum=0;

#ifdef USE_MULTIBYTE
	char sReturn[20], sName[200]; 
#else
	TCHAR sReturn[20], sName[200]; 
#endif

	//SYSTEM DATA------------------------------------------------------------------------------------------------------------
	int shutter;
	double cal_x, cal_y, frame_len, scale_x, scale_y;
	
	sParameterPath.Format(_T("%s"), filename);
	str.Format(_T("%s - System Parameter"), str_name);
	
	::GetPrivateProfileString(str,_T("shutter"),	_T("3000"),	sReturn,10, sParameterPath); shutter	=(int)_tstof(sReturn);
	::GetPrivateProfileString(str,_T("cal_x"),		_T("0"),	sReturn,10, sParameterPath); cal_x		=_tstof(sReturn);
	::GetPrivateProfileString(str,_T("cal_y"),		_T("0"),	sReturn,10, sParameterPath); cal_y		=_tstof(sReturn);
	::GetPrivateProfileString(str,_T("frame_len"),	_T("100"),	sReturn,10, sParameterPath); frame_len	=_tstof(sReturn);
	::GetPrivateProfileString(str,_T("scale_x"),	_T("0.05"),	sReturn,10, sParameterPath); scale_x	=_tstof(sReturn);
	::GetPrivateProfileString(str,_T("scale_y"),	_T("0.05"),	sReturn,10, sParameterPath); scale_y	=_tstof(sReturn);


	if(shutter<10 || shutter>100000 || cal_x<-1000 || cal_x>3000 || frame_len<10 || frame_len>1000 ||
		scale_x<0.001 || scale_x>1.0 || scale_y<0.001 || scale_y>1.0) goto done;
	//------------------------------------------------------------------------------------------------------------------------

	//param1~220 까지 읽는다 ------------------------------------------------------------
	str.Format(_T("%s - Inspect Parameter"), str_name);
	for(i=0;i<220;i++)
	{
		sParam.Format(_T("param%d"), i+1);
		::GetPrivateProfileString(str,sParam,_T("0"),sReturn,10, sParameterPath);

#ifdef USE_MULTIBYTE
		dParam[i]=atof(sReturn);
#else
		dParam[i]=_tstof(sReturn);
#endif
		if(i==34 || i==35 || (i>=37 && i<40) || (i>=41 && i<45) || i==50 ) continue; //34~35:물리적마킹거리, 41~44: CamYPos, 미검영역, 자동Edge, TACEdge, 50:최소밝기

		dSum+=dParam[i];
	}

	
	//SERVER 데이타 읽는다 --------------------------------------------------------------
	str.Format(_T("SERVER"));

	::GetPrivateProfileString(str,_T("MachineName"),_T(""),sName,200, sParameterPath);	
	sParam.Format(_T("%s"), sName);
//	if(	sParam.GetLength()<1) goto done;

	::GetPrivateProfileString(str,_T("strModel"),_T(""),sName,200, sParameterPath);
	sParam.Format(_T("%s"), sName);
	if(	sParam.GetLength()<1)
	{
		::GetPrivateProfileString(str,_T("ModelName"),_T(""),sName,200, sParameterPath);
		ModelName.Format(_T("%s"), sName);
//		if(ModelName.GetLength()<1) goto done;
	}

	
	for(i=0;i<15;i++)
	{
		sParam.Format(_T("nCycle%d"), i+1);
		::GetPrivateProfileString(str,sParam,_T("0"),sReturn,10, sParameterPath);
#ifdef USE_MULTIBYTE
		nCycle[i]=(int)atof(sReturn);
#else
		nCycle[i]=(int)_tstof(sReturn);
#endif
		dSum+=nCycle[i];
	}


#ifdef CHANGEABLE_MARKING		
	nOptic=GetMarkOptic();
	for(i=nOptic*10;i<nOptic*10+10;i++)
	{
		for(j=0;j<10;j++)
		{
			sParam.Format(_T("nData[%d][%d]"), i,j);
			::GetPrivateProfileString(_T("MARKING"),sParam,_T("0"),sReturn,10, sParameterPath);
#ifdef USE_MULTIBYTE
			nTmp=atoi(sReturn);
#else
			nTmp=_tstoi(sReturn);
#endif
			dSum+=nTmp;
		}		
	}
#endif

	if(dSum<0) dSum*=-1.0;

	nRet=((int)dSum)%256;
	if(nRet==0) nRet=1;	

done:
	return nRet; //0 이면 파일이 엉터리데이타 임
}


#if PROGRAM_TYPE==WEB_10LEVEL
int Ini_file_MakingVer3(CString filename, int nMarkingData)
{
	//string unicode추가 YKS 20200807		
	int i,j, nLen;
	CString key;
	TCHAR sFile[256];
	char    pcname[MAX_PATH];
	TCHAR   pcnameT[MAX_PATH];
	char    charData[MAX_PATH];
	TCHAR   TcharData[MAX_PATH]={0,};
	CString parameter_path, str;
	CStdioFile f;

	parameter_path.Format(_T("%s"),filename);	

	if(!f.Open(parameter_path,  CFile::modeCreate|CFile::typeText|CFile::modeWrite)) return 0;

	for(i=0; i<g_ParamVer2.nTotalPort; i++)
	{
#ifdef INTERFACE_NEXTEYE_SERVER
		sprintf(pcname, "%s", g_ParamVer2.ComName[i]);
		nLen=strlen(pcname);
		if(nLen<7) continue;
		
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pcname, nLen, pcnameT, nLen);
		pcnameT[nLen]=NULL;
#else
		swprintf(pcnameT, L"%s", g_ParamVer2.ComName[i]);
		pcnameT[7]=NULL;
#endif

		//Inspect Parameter Write-------------------------------------------
		key.Format(_T("[%s - Inspect Parameter]\n"), pcnameT);
		f.WriteString(key);

		for(j=0;j<NEW_MAX_COL_ALG_ITEM;j++)   //param41~param60까지는 Grade Parameter임.
		{
			str.Format(_T("param%d=%.3lf\n"), j+1, g_ParamVer2.param[i][j]);
			f.WriteString(str);
		}
		//------------------------------------------------------------------
		
		//System Parameter Write--------------------------------------------
		key.Format(_T("[%s - System Parameter]\n"), pcnameT);
		f.WriteString(key);
		
		str.Format(_T("alram_maxgray=%d\n"), (int)g_ParamVer2.system[i][0]);
		f.WriteString(str);
		str.Format(_T("alram_mingray=%d\n"), (int)g_ParamVer2.system[i][1]);
		f.WriteString(str);
		str.Format(_T("shutter=%d\n"), (int)g_ParamVer2.system[i][2]);
		f.WriteString(str);
		str.Format(_T("cal_x=%.3lf\n"), g_ParamVer2.system[i][3]);
		f.WriteString(str);
		str.Format(_T("cal_y=%.3lf\n"), g_ParamVer2.system[i][4]);
		f.WriteString(str);
		str.Format(_T("frame_len=%.6lf\n"), g_ParamVer2.system[i][5]);
		f.WriteString(str);
		str.Format(_T("scale_x=%.6lf\n"), g_ParamVer2.system[i][6]);
		f.WriteString(str);
		str.Format(_T("scale_y=%.6lf\n"), g_ParamVer2.system[i][7]);
		f.WriteString(str);	
		//---------------------------------------------------------------------
	}
	
	str.Format(_T("[SERVER]\n"));
	f.WriteString(str);

#ifdef INTERFACE_NEXTEYE_SERVER
	memset(TcharData,0, MAX_PATH);
	sprintf(charData, "%s", g_ParamVer2.MachineName);
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, charData, strlen(charData), TcharData, strlen(charData)); 
	str.Format(_T("MachineName=%s\n"), TcharData);
	f.WriteString(str);
#else
	_stprintf(sFile, _T("MachineName=%s\n"), g_ParamVer2.MachineName);
	f.WriteString(sFile);
#endif
	
#ifdef INTERFACE_NEXTEYE_SERVER
	memset(TcharData,0, MAX_PATH);
	sprintf(charData, "%s", g_ParamVer2.ModelName);
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, charData, strlen(charData), TcharData, strlen(charData));
	str.Format(_T("ModelName=%s\n"), TcharData);
	f.WriteString(str);
#else
	_stprintf(sFile, _T("ModelName=%s\n"), g_ParamVer2.ModelName);
	f.WriteString(sFile);
#endif
	
#ifdef INTERFACE_NEXTEYE_SERVER
	str.Format(_T("strModel=%s\n"), TcharData);
	f.WriteString(str);
#else
	_stprintf(sFile, _T("strModel=%s\n"), g_ParamVer1.ModelName);
	f.WriteString(sFile);
#endif
		

	for(i=0;i<15;i++)
	{
		str.Format(_T("nCycle%d=%d\n"),i+1, g_ParamVer2.nCycle[i]);
		f.WriteString(str);
	}


//---------------------------------------------------------------------------------
#ifdef CHANGEABLE_MARKING
	//모든광학계 마킹데이타 저장---------------------------------------------------
	if(nMarkingData)
	{
		str.Format(_T("[MARKING]\n"));
		f.WriteString(str);		
		for(i=0;i<(MAX_OPTIC-1)*10;i++)
		{
			for(j=0;j<10;j++)
			{
				str.Format(_T("nData[%d][%d]=%d\n"),i,j,(int)g_ParamMark.nData[i][j]);
				f.WriteString(str);
			}
		}
		for(i=0;i<(MAX_OPTIC-1)*10;i++)
		{
			for(j=0;j<10;j++)
			{
				str.Format(_T("nLevelMCount[%d][%d]=%d\n"),i,j,g_ParamMark.nLevelMCount[i][j]);
				f.WriteString(str);
			}
		}
		for(i=0;i<MAX_OPTIC-1;i++)
		{
			for(j=0;j<10;j++)
			{
				str.Format(_T("nDefectMCount[%d][%d]=%d\n"),i,j,g_ParamMark.nDefectMCount[i][j]);
				f.WriteString(str);
			}
		}
		if(g_ParamMark.dBaseLength_mm<0) str.Format(_T("dBaseLength_mm=0\n"));
		else							 str.Format(_T("dBaseLength_mm=%.1lf\n"),g_ParamMark.dBaseLength_mm);
		f.WriteString(str);
		if(g_ParamMark.dBaseLength_M<0)  str.Format(_T("dBaseLength_M=0f\n"));
		else                             str.Format(_T("dBaseLength_M=%.3lf\n"),g_ParamMark.dBaseLength_M);
		f.WriteString(str);
		if(g_ParamMark.dBaseX_mm<0) str.Format(_T("dBaseX_mm=0\n"));
		else                        str.Format(_T("dBaseX_mm=%.1lf\n"),g_ParamMark.dBaseX_mm);
		f.WriteString(str);
		str.Format(_T("nUSE1=%d\n"),g_ParamMark.nUSE[0]);
		f.WriteString(str);
		str.Format(_T("nUSE2=%d\n"),g_ParamMark.nUSE[1]);
		f.WriteString(str);
	}
	//-------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------------

	f.Close();

	return 1;
}
int ReadParameter(int nReadFromReserve)
{
	int ret=-1;
	CString str, str_name, sParam;
	CString ModelName, strModel;
	CString sParameterPath;

	int i, j, k, nTemp, nOptic;
	double dTemp;
	int nSheetLength=0;
	int nCnt=0;
	int nNonZeroCount=0;
	
	int nCycle[15];
	TCHAR sReturn[20], sName[200]; 

	if(nReadFromReserve)
	{
		if(CopyFile(PARAMETER_PATH_RESERVE, PARAMETER_PATH, false))
		{
			DeleteFile(PARAMETER_PATH_RESERVE);
			WriteLog(_T("Copy Parameter Resever -> Main Folder"));
		}
	}
	sParameterPath.Format(_T("%s"), PARAMETER_PATH);

	//SERVER 데이타 읽는다 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	str.Format(_T("SERVER"));

	::GetPrivateProfileString(str,_T("MachineName"),_T("CC01"),sName,200, sParameterPath);
	g_ParamVer.m_strMachineName.Format(_T("%s"), sName);
	::GetPrivateProfileString(str,_T("ModelName"),_T(""),sName,200, sParameterPath);
	g_ParamVer.m_sModelName.Format(_T("%s"), sName);

	//@170112 NBO Model Check............................................................. 
	if(g_ParamVer.m_sModelName.Find(_T("W"))<0)
	{
		::GetPrivateProfileString(str,_T("strModel"),_T(""),sName,200, sParameterPath);
		g_ParamVer.m_sModelName.Format(_T("%s"), sName);
	}
	//....................................................................................

	for(i=0;i<15;i++)
	{
		sParam.Format(_T("nCycle%d"), i+1);
		::GetPrivateProfileString(str,sParam,_T("0"),sReturn,10, sParameterPath);
		g_ParamVer.nCycle[i]=(int)_tstof(sReturn);
	}

	//SYSTEM DATA 읽는다 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	int shutter;
	double cal_x, cal_y, frame_len, scale_x, scale_y;
	
	str.Format(_T("%s - System Parameter"),  g_Temp.m_sMyComName);
	
	::GetPrivateProfileString(str,_T("shutter"),	_T("3000"),	sReturn,10, sParameterPath); shutter	=(int)_tstof(sReturn);
	::GetPrivateProfileString(str,_T("cal_x"),		_T("0"),	sReturn,10, sParameterPath); cal_x		=_tstof(sReturn);
	::GetPrivateProfileString(str,_T("cal_y"),		_T("0"),	sReturn,10, sParameterPath); cal_y		=_tstof(sReturn);
	::GetPrivateProfileString(str,_T("frame_len"),	_T("100"),	sReturn,10, sParameterPath); frame_len	=_tstof(sReturn);
	::GetPrivateProfileString(str,_T("scale_x"),	_T("0.05"),	sReturn,10, sParameterPath); scale_x	=_tstof(sReturn);
	::GetPrivateProfileString(str,_T("scale_y"),	_T("0.05"),	sReturn,10, sParameterPath); scale_y	=_tstof(sReturn);

	g_ParamVer.system[0]=shutter;
	g_ParamVer.system[1]=cal_x;
	g_ParamVer.system[2]=cal_y;
	g_ParamVer.system[3]=frame_len;
	g_ParamVer.system[4]=scale_x;
	g_ParamVer.system[5]=scale_y;
	g_ParamVer.system[6]=0;
	g_ParamVer.system[7]=0;

	//서버에 불량 없어도 한번씩 데이타를 보내야 하는데 그 주기구함 (5의 배수로)>>>>>>>>>>>>>>>>
	//Frame Number보내는 주기--------------------------------------------------------------
	g_Temp.m_nFrameInfoSendCount=500/frame_len;
	g_Temp.m_nFrameInfoSendCount=5*(int)((g_Temp.m_nFrameInfoSendCount+3)/5);
	if(g_Temp.m_nFrameInfoSendCount<5) g_Temp.m_nFrameInfoSendCount=5;
	if(g_Temp.m_nFrameInfoSendCount>50) g_Temp.m_nFrameInfoSendCount=50; //최대 50 Frame

	//각종데이타 (평균검사시간, 평균밝기, 등을 보내는 주기---------------------------------
	g_Temp.m_nImageInfoSendCount=g_Temp.m_nFrameInfoSendCount*1.5;
	if(g_Temp.m_nImageInfoSendCount>50) g_Temp.m_nImageInfoSendCount=50; //최대 50 Frame
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


	//param1~220 까지 읽는다 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	str.Format(_T("%s - Inspect Parameter"), g_Temp.m_sMyComName);
	for(i=0;i<220;i++)
	{
		sParam.Format(_T("param%d"), i+1);
		::GetPrivateProfileString(str,sParam,_T("0"),sReturn,10, sParameterPath);
		g_ParamVer.param[i]=_tstof(sReturn);

		if(g_ParamVer.param[i]) nNonZeroCount++;
	}
	
	g_ParamVer.param[39]=g_ParamVer.system[0];
	g_ParamVer.param[55]=g_ParamVer.system[1];
	g_ParamVer.param[56]=g_ParamVer.system[2];
	g_ParamVer.param[57]=g_ParamVer.system[3];
	g_ParamVer.param[58]=g_ParamVer.system[4];
	g_ParamVer.param[59]=g_ParamVer.system[5];

#ifdef CHANGEABLE_MARKING	
	g_Mark.nCheckLevelMCount=0;
	g_Mark.nCheckDefectMCount=0;

	nCnt=0;
	nOptic=GetMarkOptic();
	for(i=nOptic*10;i<nOptic*10+10;i++)
	{
		for(j=0;j<10;j++)
		{
			sParam.Format(_T("nData[%d][%d]"), i,j);
			::GetPrivateProfileString(_T("MARKING"),sParam,_T("0"),sReturn,10, sParameterPath);
			g_Mark.nData[nCnt][j]=_tstoi(sReturn);
		}
		nCnt++;
	}

	nCnt=0;
	for(i=nOptic*10;i<nOptic*10+10;i++)
	{		
		for(j=0;j<10;j++)
		{			
			sParam.Format(_T("nLevelMCount[%d][%d]"), i,j);
			::GetPrivateProfileString(_T("MARKING"),sParam,_T("0"),sReturn,10, sParameterPath);
			g_Mark.nLevelMCount[nCnt][j]=_tstoi(sReturn);

			if(g_Mark.nLevelMCount[nCnt][j]>0 && g_Mark.nLevelMCount[nCnt][j]<9999) g_Mark.nCheckLevelMCount=1;
		}
		nCnt++;
	}
	//불량종류별 최대 데이타 수------------------------------------------------------------
	nCnt=0;
	for(i=0;i<10;i++) //불량종류
	{
		sParam.Format(_T("nDefectMCount[%d][%d]"), nOptic,i);
		::GetPrivateProfileString(_T("MARKING"),sParam,_T("0"),sReturn,10, sParameterPath);
		g_Mark.nDefectMCount[nCnt++]=_tstoi(sReturn);

		if(g_Mark.nDefectMCount[nCnt-1]) g_Mark.nCheckDefectMCount=1;
	}
	//-------------------------------------------------------------------------------------

	sParam.Format(_T("dBaseLength_mm"));
	::GetPrivateProfileString(_T("MARKING"),sParam,_T("1000"),sReturn,10, sParameterPath);
	g_Mark.dBaseLength_mm=_tstof(sReturn);
	if(g_Mark.dBaseLength_mm<100)  g_Mark.dBaseLength_mm=100;
	if(g_Mark.dBaseLength_mm>5000) g_Mark.dBaseLength_mm=5000;

	sParam.Format(_T("dBaseLength_M"));
	::GetPrivateProfileString(_T("MARKING"),sParam,_T("30"),sReturn,10, sParameterPath);
	g_Mark.dBaseLength_M=_tstof(sReturn);
	if(g_Mark.dBaseLength_M<1)   g_Mark.dBaseLength_M=1;
	if(g_Mark.dBaseLength_M>300) g_Mark.dBaseLength_M=300;

	sParam.Format(_T("dBaseX_mm"));
	::GetPrivateProfileString(_T("MARKING"),sParam,_T("10"),sReturn,10, sParameterPath);
	g_Mark.dBaseX_mm=_tstof(sReturn);
	if(g_Mark.dBaseX_mm<2) g_Mark.dBaseX_mm=2;

	sParam.Format(_T("nUSE1"));
	::GetPrivateProfileString(_T("MARKING"),sParam,_T("0"),sReturn,10, sParameterPath);
	nTemp=_tstoi(sReturn);
	if(nTemp&(0x01<<nOptic)) g_Mark.nUSE[0]=1;
	else			 	     g_Mark.nUSE[0]=0;

	sParam.Format(_T("nUSE2"));
	::GetPrivateProfileString(_T("MARKING"),sParam,_T("0"),sReturn,10, sParameterPath);
	nTemp=_tstoi(sReturn);
	if(nTemp&(0x01<<nOptic)) g_Mark.nUSE[1]=1;
	else			 	     g_Mark.nUSE[1]=0;

	//최종 g_Mark.nLevelMCount값 한개만 이용하면 된다.
	for(i=0;i<10;i++) 
		for(j=0;j<10;j++)
			g_Mark.nMarking[i][j]=g_Mark.nData[i][j];


	//NEL7   기포눌림관련설정----------------------
	for(j=1;j<10;j++)
		if(g_Mark.nData[5][j])
		{
			g_Param.m_nKipoPressLevelMarking=1;
			break;
		}
	//--------------------------------------------	
#else
	memset(&g_Mark, 0, sizeof(PARAMETER_MARKOPTIC));
#endif
	

	ApplyParameter();

#ifdef MY_SEND
	if(g_Temp.m_nPCOptic==SEND_OPTIC)
	{
		LoadKipoPress();
		ReadKipoPressRecvCalX();
	}
#endif

#ifdef MY_RECV
	if(g_Temp.m_nPCOptic==RECV_OPTIC)
	{
		LoadKipoPress();
	}
#endif

	ret=nNonZeroCount;
done:

#ifdef USE_20_LEVEL_MARK
	if(g_Temp.m_nPCOptic<CBCR)
	{
		SetSubMarkType(g_Temp.m_nPCOptic);
		ReadMarkParameter(nReadFromReserve);
	}
#endif

#ifdef USE_DAKANO_LINE
	LoadDKOffset();
#endif

	return ret;
}


void ReadKipoPressRecvCalX()
{
#ifdef MY_SEND
	int i;
	CString str, sParameterPath;
	TCHAR sReturn[20];
	double cal_x;

	sParameterPath.Format(_T("%s"), PARAMETER_PATH);
	for(i=1;i<=20;i++)		//RECV 최대 20개
	{
#ifdef INTERFACE_NEXTEYE_SERVER
		str.Format(_T("NEL-%d%02d - System Parameter"), RECV_OPTIC, i);		//받는게 200번대
#else
		str.Format(_T("COS-%d%02d - System Parameter"), RECV_OPTIC, i);		//받는게 200번대
#endif
		::GetPrivateProfileString(str,_T("cal_x"),		_T("0"),	sReturn,10, sParameterPath); cal_x		=_tstof(sReturn);

		g_Kipo.m_COSRecvCalX[i]=cal_x;
	}
#endif
}

#else
//DAKANO
int Ini_file_MakingVer3(CString filename, int nMarkingData)
{
	int i,j;
	CString key;
	TCHAR sFile[256];
	char    pcname[MAX_PATH];
	TCHAR   pcnameT[MAX_PATH];
	CString parameter_path, str;
	CStdioFile f;

	parameter_path.Format(_T("%s"),filename);	

	if(!f.Open(parameter_path,  CFile::modeCreate|CFile::typeText|CFile::modeWrite)) return 0;

	for(i=0; i<g_ParamVer2.nTotalPort; i++)
	{
		sprintf(pcname, "%s", g_ParamVer2.ComName[i]);		
		if(strlen(pcname)<7) continue;
		
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pcname, strlen(pcname), pcnameT, strlen(pcname));
		pcnameT[7]=NULL;
		//Inspect Parameter Write-------------------------------------------
		key.Format(_T("[%s - Inspect Parameter]\n"), pcnameT);
		f.WriteString(key);

		for(j=0;j<NEW_MAX_COL_ALG_ITEM;j++)   //param41~param60까지는 Grade Parameter임.
		{
			str.Format(_T("param%d=%.3lf\n"), j+1, g_ParamVer2.param[i][j]);
			f.WriteString(str);
		}
		//------------------------------------------------------------------
		
		//System Parameter Write--------------------------------------------
		key.Format(_T("[%s - System Parameter]\n"), pcnameT);
		f.WriteString(key);
		
		str.Format(_T("alram_maxgray=%d\n"), (int)g_ParamVer2.system[i][0]);
		f.WriteString(str);
		str.Format(_T("alram_mingray=%d\n"), (int)g_ParamVer2.system[i][1]);
		f.WriteString(str);
		str.Format(_T("shutter=%d\n"), (int)g_ParamVer2.system[i][2]);
		f.WriteString(str);
		str.Format(_T("cal_x=%.3lf\n"), g_ParamVer2.system[i][3]);
		f.WriteString(str);
		str.Format(_T("cal_y=%.3lf\n"), g_ParamVer2.system[i][4]);
		f.WriteString(str);
		str.Format(_T("frame_len=%.6lf\n"), g_ParamVer2.system[i][5]);
		f.WriteString(str);
		str.Format(_T("scale_x=%.6lf\n"), g_ParamVer2.system[i][6]);
		f.WriteString(str);
		str.Format(_T("scale_y=%.6lf\n"), g_ParamVer2.system[i][7]);
		f.WriteString(str);	
		//---------------------------------------------------------------------
	}
	
#ifdef INTERFACE_NEXTEYE_SERVER
	str.Format(_T("[SERVER]\n"));
	f.WriteString(str);		
	_stprintf(sFile, _T("MachineName=%s\n"), CA2W(g_ParamVer1.MachineName));
	f.WriteString(sFile);
	_stprintf(sFile, _T("ModelName=%s\n"), CA2W(g_ParamVer1.ModelName));
	f.WriteString(sFile);
	_stprintf(sFile, _T("strModel=%s\n"), CA2W(g_ParamVer1.ModelName));
	f.WriteString(sFile);	
#else
	str.Format(_T("[SERVER]\n"));
	f.WriteString(str);		
	_stprintf(sFile, _T("MachineName=%s\n"), g_ParamVer1.MachineName);
	f.WriteString(sFile);
	_stprintf(sFile, _T("ModelName=%s\n"), g_ParamVer1.ModelName);
	f.WriteString(sFile);
	_stprintf(sFile, _T("strModel=%s\n"), g_ParamVer1.ModelName);
	f.WriteString(sFile);	
#endif	

	for(i=0;i<15;i++)
	{
		str.Format(_T("nCycle%d=%d\n"),i+1, g_ParamVer2.nCycle[i]);
		f.WriteString(str);
	}


//---------------------------------------------------------------------------------
#ifdef CHANGEABLE_MARKING
	//모든광학계 마킹데이타 저장---------------------------------------------------
	if(nMarkingData)
	{
		str.Format(_T("[MARKING]\n"));
		f.WriteString(str);		
		for(i=0;i<(MAX_OPTIC-1)*10;i++)
		{
			for(j=0;j<10;j++)
			{
				str.Format(_T("nData[%d][%d]=%d\n"),i,j,(int)g_ParamMark.nData[i][j]);
				f.WriteString(str);
			}
		}
		for(i=0;i<(MAX_OPTIC-1)*10;i++)
		{
			for(j=0;j<10;j++)
			{
				str.Format(_T("nLevelMCount[%d][%d]=%d\n"),i,j,g_ParamMark.nLevelMCount[i][j]);
				f.WriteString(str);
			}
		}
		for(i=0;i<MAX_OPTIC-1;i++)
		{
			for(j=0;j<10;j++)
			{
				str.Format(_T("nDefectMCount[%d][%d]=%d\n"),i,j,g_ParamMark.nDefectMCount[i][j]);
				f.WriteString(str);
			}
		}
		if(g_ParamMark.dBaseLength_mm<0) str.Format(_T("dBaseLength_mm=0\n"));
		else							 str.Format(_T("dBaseLength_mm=%.1lf\n"),g_ParamMark.dBaseLength_mm);
		f.WriteString(str);
		if(g_ParamMark.dBaseLength_M<0)  str.Format(_T("dBaseLength_M=0f\n"));
		else                             str.Format(_T("dBaseLength_M=%.3lf\n"),g_ParamMark.dBaseLength_M);
		f.WriteString(str);
		if(g_ParamMark.dBaseX_mm<0) str.Format(_T("dBaseX_mm=0\n"));
		else                        str.Format(_T("dBaseX_mm=%.1lf\n"),g_ParamMark.dBaseX_mm);
		f.WriteString(str);
		str.Format(_T("nUSE1=%d\n"),g_ParamMark.nUSE[0]);
		f.WriteString(str);
		str.Format(_T("nUSE2=%d\n"),g_ParamMark.nUSE[1]);
		f.WriteString(str);
	}
	//-------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------------

	f.Close();

	return 1;
}
int ReadParameter(int nReadFromReserve)
{
	int ret=-1;
	CString str, str_name, sParam;
	CString ModelName, strModel;
	CString sParameterPath;

	int i, j, k, nTemp;
	double dTemp;
	int nSheetLength=0;
	int nCnt=0;
	int nNonZeroCount=0;
	
	int nCycle[15];
	TCHAR sReturn[20], sName[200]; 

	if(nReadFromReserve)
	{
		if(CopyFile(PARAMETER_PATH_RESERVE, PARAMETER_PATH, false))
		{
			DeleteFile(PARAMETER_PATH_RESERVE);
		}
	}
	sParameterPath.Format(_T("%s"), PARAMETER_PATH);


	//SERVER 데이타 읽는다 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	str.Format(_T("SERVER"));

	::GetPrivateProfileString(str,_T("MachineName"),_T("CC01"),sName,200, sParameterPath);
	g_ParamVer.m_strMachineName.Format(_T("%s"), sName);
	::GetPrivateProfileString(str,_T("ModelName"),_T(""),sName,200, sParameterPath);
	g_ParamVer.m_sModelName.Format(_T("%s"), sName);

	//@170112 NBO Model Check............................................................. 
	if(ModelName.Find(_T("W"))<0)
	{
		::GetPrivateProfileString(str,_T("strModel"),_T(""),sName,200, sParameterPath);
		g_ParamVer.m_sModelName.Format(_T("%s"), sName);
	}
	//....................................................................................

	for(i=0;i<15;i++)
	{
		sParam.Format(_T("nCycle%d"), i+1);
		::GetPrivateProfileString(str,sParam,_T("0"),sReturn,10, sParameterPath);
		g_ParamVer.nCycle[i]=(int)_tstof(sReturn);
	}

	//SYSTEM DATA 읽는다 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	int shutter;
	double cal_x, cal_y, frame_len, scale_x, scale_y;
	
	str.Format(_T("%s - System Parameter"),  g_Temp.m_sMyComName);
	
	::GetPrivateProfileString(str,_T("shutter"),	_T("3000"),	sReturn,10, sParameterPath); shutter	=(int)_tstof(sReturn);
	::GetPrivateProfileString(str,_T("cal_x"),		_T("0"),	sReturn,10, sParameterPath); cal_x		=_tstof(sReturn);
	::GetPrivateProfileString(str,_T("cal_y"),		_T("0"),	sReturn,10, sParameterPath); cal_y		=_tstof(sReturn);
	::GetPrivateProfileString(str,_T("frame_len"),	_T("100"),	sReturn,10, sParameterPath); frame_len	=_tstof(sReturn);
	::GetPrivateProfileString(str,_T("scale_x"),	_T("0.05"),	sReturn,10, sParameterPath); scale_x	=_tstof(sReturn);
	::GetPrivateProfileString(str,_T("scale_y"),	_T("0.05"),	sReturn,10, sParameterPath); scale_y	=_tstof(sReturn);

	g_ParamVer.system[0]=shutter;
	g_ParamVer.system[1]=cal_x;
	g_ParamVer.system[2]=cal_y;
	g_ParamVer.system[3]=frame_len;
	g_ParamVer.system[4]=scale_x;
	g_ParamVer.system[5]=scale_y;
	g_ParamVer.system[6]=0;
	g_ParamVer.system[7]=0;

	//서버에 불량 없어도 한번씩 데이타를 보내야 하는데 그 주기구함 (5의 배수로)>>>>>>>>>>>>>>>>
	//Frame Number보내는 주기--------------------------------------------------------------
	g_Temp.m_nFrameInfoSendCount=500/frame_len;
	g_Temp.m_nFrameInfoSendCount=5*(int)((g_Temp.m_nFrameInfoSendCount+3)/5);
	if(g_Temp.m_nFrameInfoSendCount<5) g_Temp.m_nFrameInfoSendCount=5;
	if(g_Temp.m_nFrameInfoSendCount>50) g_Temp.m_nFrameInfoSendCount=50; //최대 50 Frame

	//각종데이타 (평균검사시간, 평균밝기, 등을 보내는 주기---------------------------------
	g_Temp.m_nImageInfoSendCount=g_Temp.m_nFrameInfoSendCount*1.5;
	if(g_Temp.m_nImageInfoSendCount>50) g_Temp.m_nImageInfoSendCount=50; //최대 50 Frame
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	//param1~220 까지 읽는다 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	str.Format(_T("%s - Inspect Parameter"), g_Temp.m_sMyComName);
	for(i=0;i<NEW_MAX_COL_ALG_ITEM;i++)
	{
		sParam.Format(_T("param%d"), i+1);
		::GetPrivateProfileString(str,sParam,_T("0"),sReturn,10, sParameterPath);
		g_ParamVer.param[i]=_tstof(sReturn);

		if(g_ParamVer.param[i]) nNonZeroCount++;
	}
	
	ApplyParameter();
		
	ret=nNonZeroCount;
done:

	return ret;
}
#endif
int SaveParameter()
{
	int ret=-1, nOptic;
	CString str, str_name, sParam;
	CString sParameterPath;
	CStdioFile f;
	
	int i, j, k, nTemp;
	int nCnt=0;
	

	if(!f.Open(PARAMETER_PATH,  CFile::modeCreate|CFile::typeText|CFile::modeWrite)) return 0;

	//Parameter 저장>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	sParameterPath.Format(_T("[%s - Inspect Parameter]\n"), g_Temp.m_sMyComName);
	f.WriteString(sParameterPath);
	for(i=0;i<220;i++)
	{
		sParam.Format(_T("param%d=%.3lf\n"), i+1, g_ParamVer.param[i]);
		f.WriteString(sParam);
	}


	//Syetem 저장>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	str.Format(_T("[%s - System Parameter]\n"),  g_Temp.m_sMyComName);
	f.WriteString(str);

	str.Format(_T("shutter=%d\n"), (int)g_ParamVer.system[0]); f.WriteString(str);
	str.Format(_T("cal_x=%.6lf\n"),		g_ParamVer.system[1]); f.WriteString(str);
	str.Format(_T("cal_y=%.6lf\n"),		g_ParamVer.system[2]); f.WriteString(str);
	str.Format(_T("frame_len=%.6lf\n"), g_ParamVer.system[3]); f.WriteString(str);
	str.Format(_T("scale_x=%.6lf\n"),	g_ParamVer.system[4]); f.WriteString(str);
	str.Format(_T("scale_y=%.6lf\n"),	g_ParamVer.system[5]); f.WriteString(str);


	//SERVER 데이타 저장 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	str.Format(_T("[SERVER]\n"));
	f.WriteString(str);
	str.Format(_T("MachineName=%s\n"), g_ParamVer.m_strMachineName),	f.WriteString(str);
	str.Format(_T("ModelName=%s\n"), g_ParamVer.m_sModelName),	f.WriteString(str);
	for(i=0;i<15;i++)
	{
		sParam.Format(_T("nCycle%d=%d\n"), i+1, g_ParamVer.nCycle[i]);
		f.WriteString(sParam);
	}
	

	str.Format(_T("[MARKING]\n"));
	f.WriteString(str);

	nCnt=0;
	nOptic=GetMarkOptic();
	for(i=nOptic*10;i<nOptic*10+10;i++)
	{
		for(j=0;j<10;j++)
		{
			sParam.Format(_T("nData[%d][%d]=%d\n"), i,j, g_Mark.nData[nCnt][j]);
			f.WriteString(sParam);
		}
		nCnt++;
	}

	nCnt=0;
	for(i=nOptic*10;i<nOptic*10+10;i++)
	{		
		for(j=0;j<10;j++)
		{			
			sParam.Format(_T("nLevelMCount[%d][%d]=%d\n"), i,j, g_Mark.nLevelMCount[nCnt][j]);
			f.WriteString(sParam);
		}
		nCnt++;
	}
	//불량종류별 최대 데이타 수------------------------------------------------------------
	nCnt=0;
	for(i=0;i<10;i++) //불량종류
	{
		sParam.Format(_T("nDefectMCount[%d][%d]=%d\n"), nOptic,i,g_Mark.nDefectMCount[nCnt++]);
		f.WriteString(sParam);
	}
	//-------------------------------------------------------------------------------------

	sParam.Format(_T("dBaseLength_mm=%.3lf\n"),g_Mark.dBaseLength_mm);
	f.WriteString(sParam);

	sParam.Format(_T("dBaseLength_M=%.3lf\n"), g_Mark.dBaseLength_M);
	f.WriteString(sParam);

	sParam.Format(_T("dBaseX_mm=%.3lf\n"), g_Mark.dBaseX_mm);
	f.WriteString(sParam);

	sParam.Format(_T("nUSE1=%d\n"), g_Mark.nUSE[0]);
	f.WriteString(sParam);

	sParam.Format(_T("nUSE2=%d\n"), g_Mark.nUSE[1]);
	f.WriteString(sParam);

	f.Close();

	return 1;
}
		
//loadL ApplyParameter()
void ApplyParameter()
{
	int i;
	TCHAR sData[20];

//TPW HOLE 이 있으면 없앰.-------------------------
#ifdef USE_HOLE_SKIP
	g_Temp.m_nHoleCheck		=g_ParamVer.param[23];
	g_Temp.m_nHoleThres		=g_ParamVer.param[24];
	g_Temp.m_dHolePeriod    =g_ParamVer.param[25];
#else
	g_Temp.m_nHoleCheck     =0;
#endif
//-------------------------------------------------

	//GRADE PARAMETER-------------------------------------------------------------------------------------
	g_Param.m_nYOffset		=g_ParamVer.param[41];
	g_Param.m_nNotInspArea  =g_ParamVer.param[42];	//미검영역 (+ 면 왼쪽 미검, -면 오른쪽 미검)
	g_Param.m_nEdgeThreshold=g_ParamVer.param[43];  //자동검사 여부 및 Threadhold
	g_Param.m_nEdgeOffset	=g_ParamVer.param[44];  //TAC EDGE (자동검사후 Offset영역)
	

	if(g_Param.m_nEdgeThreshold>0) g_Param.m_nAutoEdgeFind=1;
	else						   g_Param.m_nAutoEdgeFind=0;

	g_Param.m_nTapeDist[0]	=g_ParamVer.param[45];	//테입거리
	g_Param.m_nTapeDist[1]	=g_ParamVer.param[46];	//이형테입거리
	g_Param.m_nTapeDist[2]	=g_ParamVer.param[47];	//보호테입거리
	g_Param.m_nTapeSkipDist =g_ParamVer.param[48];	//스킵거리

#ifdef USE_COSS_SERVER_CLIENT_NEL
	g_Param.m_nBrightMax	=g_ParamVer.param[29];	//최대밝기
	g_Param.m_nBrightMin	=g_ParamVer.param[30];	//최소밝기
#else
	g_Param.m_nBrightMax	=g_ParamVer.param[49];	//최대밝기
	g_Param.m_nBrightMin	=g_ParamVer.param[50];	//최소밝기
#endif

	g_Param.m_nOffsetTapeFrame=g_ParamVer.param[51];//CAMERASENSING : Unwinder신호후 이 Frame지나서부터 Tape 체크 (g_Param.m_nTapeDist[0] 범위까지) 

	g_Param.m_nAttatchPixel =2;						//Default (Dilation해서 불량 붙임. 2는 3x3 한번)
	g_Param.m_nSizeMethod	=0;						//Size구하는 방법 0 (가로, 세로, (가로+세로)/2), 1 (장축, 단축, 장축)   2 (장축, 단축, (장축+단축)/2) 
	g_Param.m_nExposure		=g_ParamVer.system[0];	//노출값
	g_Param.m_dCamStartPosX	=g_ParamVer.system[1];	//Cal X
	g_Param.m_dCamStartPosY	=g_ParamVer.system[2];	//Cal Y
	g_Param.m_dFrameLen		=g_ParamVer.system[3];	//Frame Length
	g_Param.m_dScaleFactorX	=g_ParamVer.system[4];	//Scale Factor X	
	g_Param.m_dScaleFactorY =g_ParamVer.system[5];  //Scale Factor Y

	if(g_Param.m_nExposure>g_System.m_nMaxExposure)		 g_Param.m_nExposure=g_System.m_nMaxExposure;
	else if(g_Param.m_nExposure<g_System.m_nMinExposure) g_Param.m_nExposure=g_System.m_nMinExposure;

	//CAMANGLE : 카메라가 TD방향으로 설치되어 있는데, 일부 
	if(g_System.m_nCAMAngle!=0) g_Param.m_dImageAngleFactorY=tan(g_System.m_nCAMAngle*3.142592/180)*(g_Param.m_dScaleFactorX*g_System.m_nImageW)/g_System.m_nImageW;
	else                        g_Param.m_dImageAngleFactorY=0;
	//-------------------------------------------------------------------------------------------------------
	if(g_Param.m_dScaleFactorX<=0) g_Param.m_dScaleFactorX=0.05;
	if(g_Param.m_dScaleFactorY<=0) g_Param.m_dScaleFactorY=0.05;

	//PARAMETER-------------------------------------
	g_Param.m_nValueOneUp	=(int)g_ParamVer.param[0];		//강백점(1Level) Value
	g_Param.m_dSizeOneUp	=g_ParamVer.param[1];			//강백점(1Level) Size

	if(g_Temp.m_nPCOptic!=COS3)
	{
		g_Param.m_nValueOneDn	=(int)g_ParamVer.param[2];		//강흑점(1Level) Value	
		g_Param.m_dSizeOneDn	=g_ParamVer.param[3];			//강흑점(1Level) Size
	}

	g_Param.m_nDotMarkRemove=0;								//Dot 마킹제거 사용하지 않음 (ApplyOptic1~B()에서 개별사용여부 결정)

	g_Param.m_nAreaThUp		=(int)g_ParamVer.param[31];	if(g_Param.m_nAreaThUp<5) g_Param.m_nAreaThUp=5;	//Area구하기 위한 최소값
	g_Param.m_nAreaThDn		=(int)g_ParamVer.param[32];	if(g_Param.m_nAreaThDn<5) g_Param.m_nAreaThDn=5;	//Area구하기 위한 최소값
	g_Param.m_nPeriodLevel	=(int)g_ParamVer.param[33];		//주기를 1Level로 변경
	if(g_Param.m_nPeriodLevel>10) g_Param.m_nPeriodLevel=10;


	g_Param.m_nNoInspect    =(int)g_ParamVer.param[36];     //No검사.
	//Spot검사는 Default
	for(i=0;i<10;i++)
	{
		g_Param.m_nUseLevel[i]	=(int)g_ParamVer.param[60+i];		//사용하는 Level
		g_Param.m_nValueUp[i]	=(int)g_ParamVer.param[70+i];		//White Value
		g_Param.m_dSizeUp[i]	=	  g_ParamVer.param[80+i];		//White Size

#ifdef NANJING_OLED
		g_Param.m_nValueDn[i]	=(int)g_ParamVer.param[140+i];		//Black Value
		g_Param.m_dSizeDn[i]	=	  g_ParamVer.param[150+i];		//Black Size
#else
		g_Param.m_nValueDn[i]	=(int)g_ParamVer.param[90+i];		//Black Value
		g_Param.m_dSizeDn[i]	=	  g_ParamVer.param[100+i];		//Black Size
#endif
	}

	//파라미터 Reset하는 것들
	g_Param.m_nLamiTh=0;	//NEL3에서만 사용함.
	g_Param.m_nBrightCompensation=0; //밝기 보상값 2~4까지 유효함.


	switch(g_Temp.m_nPCOptic)
	{
		case COS1:ApplyOptic1(); break;
		case COS2:ApplyOptic2(); break;
		case COS3:ApplyOptic3(); break;
		case COS4:ApplyOptic4(); break;
		case COS5:ApplyOptic5(); break;
		case COS6:ApplyOptic6(); break;
		case COS7:ApplyOptic7(); break;
		case COS8:ApplyOptic8(); break;
		case COS9:ApplyOptic9(); break;
		case COSA:ApplyOpticA(); break;
		case COSB:ApplyOpticB(); break;
		case COSC:ApplyOpticC(); break;
		case COSD:ApplyOpticD(); break;
		case CBCR:ApplyOpticBCR(); break;
		case CMAK:ApplyOpticMarkingVision(); break;
	}



	//셔터 바꾼다. 
	g_Temp.m_nExposureData=g_Param.m_nExposure;
#ifdef INTERFACE_NEXTEYE_SERVER
	if(g_System.m_nLineCAM) 
	{
		g_Grabber.SetExposureTime(g_Param.m_nExposure, 1-g_Temp.m_bAuto);  //Basler는 (설정값 / 100) usec   즉 1200 이면 12usec 
	}
	else
	{
		g_Grabber.SetExposureTime(g_Param.m_nExposure,    1-g_Temp.m_bAuto);  //Area는 그냥 보냄
	}
#else
	g_Grabber.SetExposureTime(g_Param.m_nExposure, 1-g_Temp.m_bAuto);
#endif

//	g_Temp.m_nAddExposure=g_Temp.m_nAddSumExposure=0;  //초기 설정시 이값은 0 이 된다.


#if GRABBER_TYPE==BASLER_GRABBER
	if(g_System.m_nLineCAM==0) g_Grabber.SetYOffset(g_Param.m_nYOffset);	//Area Camera
#else
	SetGain_Serial(g_Param.m_nGain); //Gain 조절 한다
#endif


	ResetEdgeData();	//Edge 데이타 Reset
	ChangeableMarkingReset(); //프로그램 시작시 또는 LOT 변경시 한번 Call 해준다.
}

//2개의 Level과 Class를 받아서 더 중요한 것을 Return 한다.
//조건이 같으면 앞에것 선택
int SelectDefect(int nLevel1, int nClass1, int nLevel2, int nClass2)
{
	int nClass=0;

	//데이타가 정상이 아니면 정상인 데이타를  Return 하든지 0을 Return한다------------------------------------------
	if(nLevel1<1 || nLevel1>10 || nClass1<1 || nClass1>9 || nLevel2<1 || nLevel2>10 ||  nClass2<1 || nClass2>9 )
	{
		if(	!(nLevel1<1 || nLevel1>10 || nClass1<1 || nClass1>9) )
		{
			nClass=MILLION*nLevel1+g_Temp.m_nPCFirstNo*CLASSDEV+nClass1;
		}
		if( !(nLevel2<1 || nLevel2>10 ||  nClass2<1 || nClass2>9) )
		{
			nClass=MILLION*nLevel2+g_Temp.m_nPCFirstNo*CLASSDEV+nClass2;
		}
		return nClass;
	}
	//--------------------------------------------------------------------------------------------------------------


	if(g_Mark.nMarking[nClass1-1][nLevel1-1]==g_Mark.nMarking[nClass2-1][nLevel2-1])  //마킹여부가 같다면
	{
		if(nLevel1==nLevel2) //같은 마킹, 같은 Level이면 앞에 있는 것으로 선택
		{
			nClass=MILLION*nLevel1+g_Temp.m_nPCFirstNo*CLASSDEV+nClass1;
		}
		else
		{
			if(nLevel1<nLevel2) nClass=MILLION*nLevel1+g_Temp.m_nPCFirstNo*CLASSDEV+nClass1;
			else				nClass=MILLION*nLevel2+g_Temp.m_nPCFirstNo*CLASSDEV+nClass2;
		}
	}
	else //하나는 마킹하고 하나는 마킹하지 않으면 마킹우선
	{
		if(g_Mark.nMarking[nClass1-1][nLevel1-1])
		{
			nClass=MILLION*nLevel1+g_Temp.m_nPCFirstNo*CLASSDEV+nClass1;
		}
		else
		{
			nClass=MILLION*nLevel2+g_Temp.m_nPCFirstNo*CLASSDEV+nClass2;
		}
	}

	return nClass;
}


#ifdef USE_3D_AREA
void ApplyOptic1() 
{
	int i;

//		pDoc->m_data.m_dMarkValue	= double(param9);	//Size구할때 세로 방향으로 이 기울기보다 큰 점이 시작점과 끝점이 된다.

	g_Param.m_nSizeMethod			=0;
	g_Param.m_nOverkillSlope		= g_ParamVer.param[5];  if(g_Param.m_nOverkillSlope>=255)  g_Param.m_nOverkillSlope=0;
	g_Param.m_nMarkThreholdValue	= g_ParamVer.param[6];  
	g_Param.m_nMarkLevel			= g_ParamVer.param[7];  if(g_Param.m_nMarkLevel<=0) g_Param.m_nMarkLevel=1;  //설정이 없으면 1Level로
	g_Param.m_nRemoveFishEye		= g_ParamVer.param[8];
	//자동밝기 설정-----------------------------------------------
	if(g_ParamVer.param[9]>0)
	{
		g_Param.m_nSetAutoExposure = 1;
		if(g_ParamVer.param[9]>=30)
			g_System.m_nGrabBright     = g_ParamVer.param[9];
		else
			g_System.m_nGrabBright     = g_System.m_nGrabBrightBK;
	}
	else
	{
		g_Param.m_nSetAutoExposure = 0;
		g_System.m_nGrabBright         = g_System.m_nGrabBrightBK;
	}
	//------------------------------------------------------------
	g_Param.m_dOffsetAngle= g_ParamVer.param[10];

	for(i=0;i<10;i++)
	{
		g_Param.m_dLineThreshold[i] =g_ParamVer.param[110+i];		//액정라인
		g_Param.m_dHTOCloudSize[i]	=g_ParamVer.param[120+i];		//HTO Size
	}

	//HTO--------------------------------------------------------------
	g_Param.m_nUSEHTO=g_Param.m_dHTOCloud=g_Param.m_dHTOCloudW=0;
	
	g_Param.m_dHTOCloudW	 =g_ParamVer.param[11];
	g_Param.m_dHTOCloud		 =g_ParamVer.param[12];

	g_Param.m_dGroupSize	 = g_ParamVer.param[13];
	g_Param.m_nGroupCount	 = g_ParamVer.param[14];

	g_Param.m_nBrightMax	=g_ParamVer.param[29];	//최대밝기
	g_Param.m_nBrightMin	=g_ParamVer.param[30];	//최소밝기

	
	if(g_Param.m_dHTOCloudW>0 || g_Param.m_dHTOCloud>0) 
		for(i=0;i<10;i++)
		{
			if(g_Param.m_nUseLevel[i] && g_Param.m_dHTOCloudSize[i]>0)
			{
				g_Param.m_nUSEHTO=1;
				break;
			}
		}
	//-----------------------------------------------------------------
}

void ApplyOptic2() 
{
	int i;

//		pDoc->m_data.m_dMarkValue	= double(param9);	//Size구할때 세로 방향으로 이 기울기보다 큰 점이 시작점과 끝점이 된다.

	g_Param.m_nSizeMethod			=0;
	g_Param.m_nOverkillSlope		= g_ParamVer.param[5];  if(g_Param.m_nOverkillSlope>=255)  g_Param.m_nOverkillSlope=0;
	g_Param.m_nMarkThreholdValue	= g_ParamVer.param[6]; // if(g_Param.m_nMarkThreholdValue<4) g_Param.m_nMarkThreholdValue=4;
	g_Param.m_nMarkLevel			= g_ParamVer.param[7];  if(g_Param.m_nMarkLevel<=0) g_Param.m_nMarkLevel=1;  //설정이 없으면 1Level로
	g_Param.m_nRemoveFishEye		= g_ParamVer.param[8];
	//자동밝기 설정-----------------------------------------------
	if(g_ParamVer.param[9]>0)
	{
		g_Param.m_nSetAutoExposure = 1;
		if(g_ParamVer.param[9]>=30)
			g_System.m_nGrabBright     = g_ParamVer.param[9];
		else
			g_System.m_nGrabBright     = g_System.m_nGrabBrightBK;
	}
	else
	{
		g_Param.m_nSetAutoExposure = 0;
		g_System.m_nGrabBright         = g_System.m_nGrabBrightBK;
	}
	//------------------------------------------------------------
	g_Param.m_dOffsetAngle= g_ParamVer.param[10];

	for(i=0;i<10;i++)
	{
		g_Param.m_dLipStdevLimit[i] =g_ParamVer.param[110+i];		//입술판단값
	}

	//HTO--------------------------------------------------------------
	g_Param.m_nUSEHTO=g_Param.m_dHTOCloud=g_Param.m_dHTOCloudW=0;
	
	g_Param.m_dHTOCloudW	 =g_ParamVer.param[11];
	g_Param.m_dHTOCloud		 =g_ParamVer.param[12];

	g_Param.m_dGroupSize	 = g_ParamVer.param[13];
	g_Param.m_nGroupCount	 = g_ParamVer.param[14];

//COS2 입술불량
	g_Param.m_nGrayLimit		= g_ParamVer.param[15];		//입술불량 (밝기차 )
	g_Param.m_dStdevSizeThres	= g_ParamVer.param[16];		//입술불량 
	g_Param.m_dLipMinSize       = g_ParamVer.param[17];		//입술최소Size


	g_Param.m_nBrightMax	=g_ParamVer.param[29];	//최대밝기
	g_Param.m_nBrightMin	=g_ParamVer.param[30];	//최소밝기
	

	g_Param.m_nRemoveFishEye=0;		
	g_Param.m_nUSEHTO=0;
	//-----------------------------------------------------------------
}
#else
//광학계별 데이타 (크로스)
void ApplyOptic1() 
{
	int i;

#ifdef INTERFACE_NEXTEYE_SERVER
	//미세이물 2동3호기 NEL1, NELB에서 사용함.
	#ifdef USE_MISE_IMUL
		g_Param.m_nValueOneUp	=(int)g_ParamVer.param[4];		//강휘점(1Level) Value
		g_Param.m_dSizeOneUp	=g_ParamVer.param[5];			//강휘점(1Level) Size
		g_Param.m_nValueOneDn	=0;								//강흑점(1Level) Value	
		g_Param.m_dSizeOneDn	=0;								//강흑점(1Level) Size
	#endif
#endif

	//자동밝기 설정--------------------------------------------
	if(g_ParamVer.param[9]>0)
	{
		g_Param.m_nSetAutoExposure = 1;
		if(g_ParamVer.param[9]>=30)
			g_System.m_nGrabBright     = g_ParamVer.param[9];
		else
			g_System.m_nGrabBright     = g_System.m_nGrabBrightBK;
	}
	else
	{
		g_Param.m_nSetAutoExposure = 0;
		g_System.m_nGrabBright         = g_System.m_nGrabBrightBK;
	}
	//---------------------------------------------------------

#ifdef USE_SK_BAT

	//검사영역 설정---------------------------------------------------------------------
	g_Param.m_nEdgeType=g_ParamVer.param[4];
	if(g_ParamVer.param[5]<g_ParamVer.param[6])
	{
		g_Param.m_nSearchX1=g_ParamVer.param[5];
		g_Param.m_nSearchX2=g_ParamVer.param[6];
	}
	else
	{
		g_Param.m_nSearchX1=g_ParamVer.param[6];
		g_Param.m_nSearchX2=g_ParamVer.param[5];
	}
	if(g_Param.m_nSearchX1<0) g_Param.m_nSearchX1=0;
	if(g_Param.m_nSearchX2>=g_System.m_nImageW) g_Param.m_nSearchX2=g_System.m_nImageW-1;

	g_Param.m_dOffset1 =g_ParamVer.param[7];
	g_Param.m_dOffset2 =g_ParamVer.param[8];
	g_Param.m_dOffset3 =g_ParamVer.param[9];
	g_Param.m_nEdgeSlope=g_ParamVer.param[10];
	if(g_Param.m_nEdgeSlope==0) g_Param.m_nEdgeSlope=5;

	g_Param.m_nCurrNoInspArea=g_ParamVer.param[14];			//Curr미검영역
	//-------------------------------------------------------------------------------------

	for(i=0;i<10;i++)
	{
		g_Param.m_dLineThreshold[i] =(int)g_ParamVer.param[110+i];		//흑라인값	

		g_Param.m_nValueDnWing[i]	=(int)g_ParamVer.param[150+i];		//사용하는 Level
		g_Param.m_dSizeDnWing[i]	=(int)g_ParamVer.param[160+i];		//White Value
	}

	g_Param.m_nAreaThDnWing=(int)g_ParamVer.param[33];
	if(g_Param.m_nAreaThDnWing<5) g_Param.m_nAreaThDnWing=15;
	
	if(g_Param.m_dOffset3>0)
		g_Param.m_nEdgeOffset=g_Param.m_dOffset3;

	ApplyData_Press();

#else

	g_Param.m_nAttatchPixel		=(int) g_ParamVer.param[13];	
	if(g_Param.m_nAttatchPixel%2==1)   g_Param.m_nAttatchPixel+=1;
	if(g_Param.m_nAttatchPixel<2)      g_Param.m_nAttatchPixel=2;
	g_Param.m_nSizeMethod		=      g_ParamVer.param[14];	 //Size구하는 방법 0 (가로, 세로, (가로+세로)/2), 1 (장축, 단축, 장축)   2 (장축, 단축, (장축+단축)/2) 
	
	for(i=0;i<10;i++)
		g_Param.m_nScratch[i]		=(int)g_ParamVer.param[90+i];	//크로스 스크라치

#ifdef 	USE_MISE_IMUL
	g_Param.m_nCunicInspArea		=(int) g_ParamVer.param[11];	//쿠닉검사Size(Pixel)
	if(g_Param.m_nCunicInspArea<24) g_Param.m_nCunicInspArea=24;
	if(g_Param.m_nCunicInspArea>80) g_Param.m_nCunicInspArea=80;
#else
	ApplyData_CrossKkopChim();	//크로스 꼽침
#endif

	ApplyData_Cunic();			//쿠닉
	ApplyData_CrossGroup();		//크로스 군집

/*
	//일정Level이상(즉 약한 불량은 한번더 검사해서 밀집인지 아닌지 체크, 사용하려고 만들었으나 실제는 사용하지 않음>>>>>>>>>>>>
	g_Param.m_nMILStartLevel	=(int) g_ParamVer.param[17];	//밀집 시작 Level
	g_Param.m_nMILTh			=(int) g_ParamVer.param[18];	//밀집 Th
	g_Param.m_dMILSize			= g_ParamVer.param[19];			//밀집 Size
	g_Param.m_nMILCount			=(int) g_ParamVer.param[20];	//밀집 Count
	g_Param.m_nMILChangeLevel	=(int) g_ParamVer.param[21];	//밀집 변경 Level
	if(g_Param.m_nMILTh<=0 || g_Param.m_nMILCount<=0 || g_Param.m_nMILChangeLevel<=0) g_Param.m_nMILStartLevel=0;
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
*/
#endif
}

#endif


void ApplyOptic3() 
{
	int i;

#ifdef USE_DAKANO_LINE   //액정기포라인

	ApplyDakanoParameter();		//라인얼룩데이타
	ApplyData_CrossKkopChim();	//크로스 꼽침
	ApplyData_Cunic();			


	g_Param.m_nBaseLine=(int) g_ParamVer.param[8];	//라인보상

	//쿠닉픽셀
	g_Param.m_nCunicInspArea		=(int) g_ParamVer.param[14];	
	if(g_Param.m_nCunicInspArea<24) g_Param.m_nCunicInspArea=24;
	if(g_Param.m_nCunicInspArea>64) g_Param.m_nCunicInspArea=64;
	
	g_Param.m_dCunicCanidValue	= 0;	//쿠닉후보값

	//영역확장
	g_Param.m_nAttatchPixel		=(int) g_ParamVer.param[15];	
	if(g_Param.m_nAttatchPixel%2==1)   g_Param.m_nAttatchPixel+=1;
	if(g_Param.m_nAttatchPixel<2)      g_Param.m_nAttatchPixel=2;
	 g_Param.m_nSizeMethod=0;
	
	for(i=0;i<10;i++)
		g_Param.m_dScratchLength[i]=g_ParamVer.param[90+i];		//크로스 스크라치


	ApplyData_CrossGroup();		//크로스 군집
	
#else

	//원래 COS3
	g_Param.m_nLamiTh	  =g_ParamVer.param[2];
	g_Param.m_dLamiMaxSize=g_ParamVer.param[3];

	//밝기증배추가--------------------------------------------------------
	g_Param.m_nBrightCompensation   = (int)g_ParamVer.param[4];
	if(g_Param.m_nBrightCompensation>4) g_Param.m_nBrightCompensation=0;
	//--------------------------------------------------------------------

	for(i=0;i<10;i++)
	{
		g_Param.m_dLamiSize[i]=g_ParamVer.param[100+i];


	}
	//기존 Scratch----------
	ApplyData_Scratch();
	ApplyData_LongScratch();

#endif

}

void ApplyOptic4() 
{
	int i;


	//마킹검출관련데이타 
	g_Param.m_nMarkingLevelChange	=(int)g_ParamVer.param[5];		//마킹불량을 검출하고 이 Level로 바꾼다.
	g_Param.m_dMarkMinValue         =     g_ParamVer.param[6];
	g_Param.m_nMarkPointValue		=(int)g_ParamVer.param[12];		//pDoc->m_data.m_nMarkValue와 이값을 동시에 만족해야 함.
	g_Param.m_dMarkMinValueWithPoint=     g_ParamVer.param[13];
	g_Param.m_nMarkNewValue			=(int)g_ParamVer.param[14];		//마킹검출 새로운 방법
	g_Param.m_nMarkNewTh			=5;								//Edge
	g_Param.m_nMuraLevelChange		=(int)g_ParamVer.param[10];		//얼룩검출후 설정된 Level로 표시

	//이물기포---------------------------------------------------------
	g_Param.m_dKipoBrightDiff=g_ParamVer.param[4];		//기포최소 밝기차
	g_Param.m_nKipoMinValue=256;
	for(i=0;i<10;i++)
	{
		g_Param.m_nKipoValue[i]	   =(int)g_ParamVer.param[110+i];	
		g_Param.m_dKipoSize[i]	   =g_ParamVer.param[120+i];	
		if(g_Param.m_nUseLevel[i]>0)
		{
			if(g_Param.m_nKipoValue[i]<g_Param.m_nKipoMinValue) 
				g_Param.m_nKipoMinValue=g_Param.m_nKipoValue[i];
		}
	}	
	if(g_Param.m_nKipoMinValue<10) g_Param.m_nKipoMinValue=10;
}

//투영
void ApplyOptic5() 
{
	int i;
	//과검체크
	g_Param.m_dOverKillSize =g_ParamVer.param[4];	//이 값보다 작으면 과검루틴 탐		
	g_Param.m_dOverKillSTD  =g_ParamVer.param[5];	//STD로 비교
	g_Param.m_dOverKillDiff =g_ParamVer.param[6];	//밝기차로 비교


	//흑군집 -----------------------------------------------------------------
	g_Param.m_nGroupArea		=(int)g_ParamVer.param[7];		//흑군집X범위
	g_Param.m_nGroupAreaTh		=(int)g_ParamVer.param[8];		//흑군집Area TH
	g_Param.m_nGroupValue		=(int)g_ParamVer.param[9];		//흑군집 Value
	g_Param.m_dGroupSize		=     g_ParamVer.param[10];		//흑군집 Size
	g_Param.m_nGroupCount		=(int)g_ParamVer.param[11];		//흑군집 갯수
	g_Param.m_nGruopLevel		=(int)g_ParamVer.param[12];		//군집을 변경할 Level
	if(g_Param.m_nGruopLevel<1) g_Param.m_nGruopLevel=1;
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------
	g_Param.m_nNEL5LevelChange	=(int)g_ParamVer.param[13];		//가변마킹으로 마킹이 된 불량은 다른 Lv로 마킹기에 전송함.(1~10까지 데이타가 있을때 유효함)
	
	g_Param.m_nOverKillValue    =(int)g_ParamVer.param[14];		//주름과검 산 Value (300)
	g_Param.m_nOverKillCount    =(int)g_ParamVer.param[15];     //주름과검 산 수 (3)


	//투영사선라인 검출시Blob
	g_Param.m_nLineThUp	    =(int)g_ParamVer.param[28]; if(g_Param.m_nLineThUp<15) g_Param.m_nLineThUp=30;
	g_Param.m_nLineThDn	    =(int)g_ParamVer.param[29]; if(g_Param.m_nLineThDn<10) g_Param.m_nLineThDn=20;


	for(i=0;i<10;i++)
	{			
		g_Param.m_nSTDThUp[i]=g_Param.m_nSTDThDn[i]=0;
		g_Param.m_dLineThresholdAngle[i]		   =(int)g_ParamVer.param[i+110];	//사선라인
		g_Param.m_dLineThreshold[i]				   =(int)g_ParamVer.param[i+120];	//MD Line
		g_Param.m_dLineThresholdTD[i]			   =(int)g_ParamVer.param[i+130];	//TD Line
		g_Param.m_nBMura[i]						   =(int)g_ParamVer.param[i+140];	//흑 얼룩
	}
}
//투영반사
void ApplyOpticA() 
{
	int i;

	g_Param.m_dOverKillSize =g_ParamVer.param[4];	//이 값보다 작으면 과검루틴 탐		
	g_Param.m_dOverKillSTD  =g_ParamVer.param[5];	//STD로 비교
	g_Param.m_dOverKillDiff =g_ParamVer.param[6];	//밝기차로 비교

	g_Param.m_nOverKillValue    =(int)g_ParamVer.param[14];		//주름과검 산 Value (300)
	g_Param.m_nOverKillCount    =(int)g_ParamVer.param[15];     //주름과검 산 수 (3)

	for(i=0;i<10;i++)
	{
		g_Param.m_nUseLevel[i]	=(int)g_ParamVer.param[60+i];		//사용하는 Level
		g_Param.m_nValueUp[i]	=(int)g_ParamVer.param[70+i];		//White Value
		g_Param.m_nValueDn[i]	=(int)g_ParamVer.param[80+i];		//Black Value
		g_Param.m_dSizeUp[i]	=	  g_ParamVer.param[90+i];		//Press Size

		g_Param.m_dLineThresholdAngle[i]=(int)g_ParamVer.param[110+i];		//사선라인
		g_Param.m_dLineThreshold[i]     =(int)g_ParamVer.param[120+i];		//MD라인
		g_Param.m_dLineThresholdTD[i]   =(int)g_ParamVer.param[120+i];		//TD라인

		g_Param.m_nBMura[i]				=(int)g_ParamVer.param[170+i];		//얼룩
	}
}


#if INSPECT_TYPE==USE_DSFR
void ApplyOptic7() 
{
	int i;

	//자동밝기 설정--------------------------------------------
	if(g_ParamVer.param[9]>0)
	{
		g_Param.m_nSetAutoExposure = 1;
		if(g_ParamVer.param[9]>=30)
			g_System.m_nGrabBright     = g_ParamVer.param[9];
		else
			g_System.m_nGrabBright     = g_System.m_nGrabBrightBK;
	}
	else
	{
		g_Param.m_nSetAutoExposure = 0;
		g_System.m_nGrabBright		   = g_System.m_nGrabBrightBK;
	}
	//---------------------------------------------------------

	g_Param.m_nRemoveBondingOverkill  = g_ParamVer.param[8];
	//찍힘/눌림 관련 파라미터------
//	ApplyData_Press();

	//군집데이타
	g_Param.m_nGroupAreaTh	=0;	//군집 Th
	if(g_Param.m_nGroupAreaTh==0)  g_Param.m_nGroupAreaTh=g_Param.m_nAreaThUp;

	//마킹검출관련데이타 
//	g_Param.m_nMarkingLevelChange	=(int)g_ParamVer.param[5];		//마킹불량을 검출하고 이 Level로 바꾼다.
	g_Param.m_nMarkingLevelChange   =0;
	g_Param.m_dMarkMinValue         =0;
	g_Param.m_nMarkPointValue		=0;		//pDoc->m_data.m_nMarkValue와 이값을 동시에 만족해야 함.
	g_Param.m_dMarkMinValueWithPoint=0;

	//기포라인
	g_Param.m_nLineKipoWidth		=0;	
	g_Param.m_nLineKipoTh			=0;		

	for(i=0;i<10;i++)
	{			
		g_Param.m_nValueLineKipo[i]		=(int)g_ParamVer.param[i+110]; //라인 
		g_Param.m_nSTDThUp[i]			=(int)g_ParamVer.param[i+120]; //백얼룩 
		g_Param.m_nSTDThDn[i]			=(int)g_ParamVer.param[i+130]; //흑얼룩 
	}
}
#elif INSPECT_TYPE==TAC_LINE
void ApplyOptic2() 
{
	ApplyOptic6();
}
void ApplyOptic6() 
{
	int i;

	g_Param.m_nStdSizeW				=40;
	g_Param.m_nStdSizeB				=30;

	g_Param.m_nBrightCompensation   = (int)g_ParamVer.param[22];
	g_Param.m_nBrightMax			=g_ParamVer.param[29];	//최대밝기
	g_Param.m_nBrightMin			=g_ParamVer.param[30];	//최소밝기


	g_Param.m_nAreaThUp		=(int)g_ParamVer.param[31];	if(g_Param.m_nAreaThUp<5) g_Param.m_nAreaThUp=5;	//Area구하기 위한 최소값
	g_Param.m_nAreaThDn		=(int)g_ParamVer.param[32];	if(g_Param.m_nAreaThDn<5) g_Param.m_nAreaThDn=5;	//Area구하기 위한 최소값


//KNULLING 체크관련 (정확히 확인후 적용) >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//필요한 데이타:
// (널링검사한다고 설정되어있고, No검사영역 있으면)  널링검사해서 자체적용하고 서버에 있다고 보내주면 서버에서 다른 PC들에게 날려줌
// 널링없는원단의 경우: 널링검사 안한다고 세팅되어 있으면 그냥 모두검사 (밝기범위내에 있으면검사)
	g_Param.m_nNoKnullingFilm   =(int)g_ParamVer.param[34]; //1이면 널링없는 원단임.
	g_Param.m_nKnullingValue =(int)g_ParamVer.param[28];	//KNULLING 값
	g_Param.m_dNullingRatioLimit=2.0;

	if(g_Temp.m_nPCOptic==COS6 && g_Param.m_nBrightMax>=255 && g_Param.m_nNotInspArea ) g_Param.m_nKnullingPC=1;  //널링검사
	else																				g_Param.m_nKnullingPC=0;  //널링검사하지 않음	

	if(g_Param.m_nNoKnullingFilm)  g_Param.m_nKnullingPC=0; //널링없는 원단이면 널링검사하지 않음
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


	for(i=0;i<10;i++)
	{
		g_Param.m_nSTDThUp[i]			=(int)g_ParamVer.param[i+110];	//백얼룩  
		if(g_Param.m_nSTDThUp[i]<50) g_Param.m_nSTDThUp[i]=50;			
		g_Param.m_dSTDSizeUp[i]			=g_ParamVer.param[i+120];		//백얼룩 Size

		g_Param.m_nSTDThDn[i]			=(int)g_ParamVer.param[i+130];	//흑얼룩 
		if(g_Param.m_nSTDThDn[i]<50) g_Param.m_nSTDThDn[i]=50;
		g_Param.m_dSTDSizeDn[i]			=g_ParamVer.param[i+140];		//흑얼룩 Size

		g_Param.m_nValueLineKipo[i]		=(int)g_ParamVer.param[150+i];	//라인
		g_Param.m_dLineThresholdTD[i]	=(int)g_ParamVer.param[160+i];	//TD라인
		g_Param.m_nScratch[i]			=(int)g_ParamVer.param[170+i];	//스크라치
	}
}


void ApplyOptic7() 
{
	g_Param.m_nBrightMax	=g_ParamVer.param[29];	//최대밝기
	g_Param.m_nBrightMin	=g_ParamVer.param[30];	//최소밝기

	//자동밝기 설정--------------------------------------------
	if(g_ParamVer.param[9]>0)
	{
		g_Param.m_nSetAutoExposure = 1;
		if(g_ParamVer.param[9]>=30)
			g_System.m_nGrabBright     = g_ParamVer.param[9];
		else
			g_System.m_nGrabBright     = g_System.m_nGrabBrightBK;
	}
	else
	{
		g_Param.m_nSetAutoExposure = 0;
		g_System.m_nGrabBright		   = g_System.m_nGrabBrightBK;
	}
	//---------------------------------------------------------


	//찍힘/눌림 관련 파라미터------
	ApplyData_Press();
}
#else
void ApplyOptic2() 
{
#ifdef USE_SK_BAT
	ApplyOptic1();
#else
	ApplyOptic8();
#endif
}

void ApplyOptic6() 
{
	int i;
	ApplyData_Line();


	//군집데이타
	g_Param.m_nGroupAreaTh	=(int) g_ParamVer.param[5];	//군집 Th
	if(g_Param.m_nGroupAreaTh==0)  g_Param.m_nGroupAreaTh=g_Param.m_nAreaThUp;
	for(i=0;i<10;i++)
	{			
		g_Param.m_nGroupAreaArray[i]	=(int)g_ParamVer.param[i+150]; //군집데이타 : 군집 영역
		g_Param.m_nGroupCountArray[i]	=(int)g_ParamVer.param[i+160]; //군집데이타 : 군집 갯수
		g_Param.m_nGroupValueArray[i]	=(int)g_ParamVer.param[i+170]; //군집데이타 : 군집 Value
		g_Param.m_nGroupSizeArray[i]	=g_ParamVer.param[i+180];	   //군집데이타 : 군집 Size

		g_Param.m_nMuitlLineValue[i]	=(int)g_ParamVer.param[i+190];	//세로선얼룩
	}
}

void ApplyOptic7() 
{
	int i;

	//자동밝기 설정(Param20=g_ParamVer.param[19]-------------------
	if(g_ParamVer.param[19]>0)
	{
		g_Param.m_nSetAutoExposure = 1;
		if(g_ParamVer.param[19]>=30)
			g_System.m_nGrabBright     = g_ParamVer.param[19];
		else
			g_System.m_nGrabBright     = g_System.m_nGrabBrightBK;
	}
	else
	{
		g_Param.m_nSetAutoExposure = 0;
		g_System.m_nGrabBright		   = g_System.m_nGrabBrightBK;
	}
	//--------------------------------------------------------------

	//군집데이타
	g_Param.m_nGroupAreaTh	=(int) g_ParamVer.param[5];	//군집 Th
	if(g_Param.m_nGroupAreaTh==0)  g_Param.m_nGroupAreaTh=g_Param.m_nAreaThUp;

	//마킹검출관련데이타 
//	g_Param.m_nMarkingLevelChange	=(int)g_ParamVer.param[5];		//마킹불량을 검출하고 이 Level로 바꾼다.
	g_Param.m_nMarkingLevelChange   =0;
	g_Param.m_dMarkMinValue         =     g_ParamVer.param[6];
	g_Param.m_nMarkPointValue		=(int)g_ParamVer.param[12];		//pDoc->m_data.m_nMarkValue와 이값을 동시에 만족해야 함.
	g_Param.m_dMarkMinValueWithPoint=     g_ParamVer.param[13];

	//기포라인
	g_Param.m_nLineKipoWidth		=(int)g_ParamVer.param[9];		if(g_Param.m_nLineKipoWidth<=0) g_Param.m_nLineKipoWidth=50;
	g_Param.m_nLineKipoTh			=(int)g_ParamVer.param[10];		
	if(g_Param.m_nLineKipoTh==0)		 g_Param.m_nLineKipoTh	=7;
	else if(g_Param.m_nLineKipoTh<3)     g_Param.m_nLineKipoTh	=3;

	for(i=0;i<10;i++)
	{			
		g_Param.m_nValueLineKipo[i]		=(int)g_ParamVer.param[i+110]; //라인기포 Value 

		g_Param.m_nGroupAreaArray[i]	=(int)g_ParamVer.param[i+130]; //군집데이타 : 군집 영역
		g_Param.m_nGroupCountArray[i]	=(int)g_ParamVer.param[i+140]; //군집데이타 : 군집 갯수
		g_Param.m_nGroupValueArray[i]	=(int)g_ParamVer.param[i+150]; //군집데이타 : 군집 Value
		g_Param.m_nGroupSizeArray[i]	=g_ParamVer.param[i+160];	   //군집데이타 : 군집 Size

		g_Param.m_nSTDThDn[i]			=(int)g_ParamVer.param[i+170]; //얼룩 당분간 사용안함. (검증은 되었음 한 Frame에서 최대 4개검출)
	}

}
#endif

void ApplyOptic8() 
{
	int i;

#if INSPECT_TYPE==TAC_LINE
	g_Param.m_nBrightMax	=g_ParamVer.param[29];	//최대밝기
	g_Param.m_nBrightMin	=g_ParamVer.param[30];	//최소밝기

	//KNULLING 체크관련
	g_Param.m_nInspectType   =(int)g_ParamVer.param[34]; //3D 경계투과에서 사용  1,2,3이면 KNULLING 검사
	g_Param.m_nKnullingValue =(int)g_ParamVer.param[35]; //KNULLING 값
	g_Param.m_dNullingRatioLimit=2.0;
	if(g_Param.m_nNotInspArea && (g_Param.m_nInspectType>0 && g_Param.m_nInspectType<4) ) g_Param.m_nKnullingPC=1;  //널링검사
	else																				  g_Param.m_nKnullingPC=0;  //널링검사하지 않음	
#endif

#ifdef MARKING_REMOVE
	//자동밝기 설정(Param20=g_ParamVer.param[19]-------------------
	if(g_ParamVer.param[20]>0)
	{
		g_Param.m_nSetAutoExposure = 1;
		if(g_ParamVer.param[20]>=30)
			g_System.m_nGrabBright     = g_ParamVer.param[20];
		else
			g_System.m_nGrabBright     = g_System.m_nGrabBrightBK;
	}
	else
	{
		g_Param.m_nSetAutoExposure = 0;
		g_System.m_nGrabBright		   = g_System.m_nGrabBrightBK;
	}
	//--------------------------------------------------------------
	//DOT마크제거를 위한 Parameter ------------------------------------------------------------------------------
	g_Param.m_nDotMarkRemove			=g_ParamVer.param[14];		//1이면 Dot마크 제거
	g_Param.m_nDotMarkTh				=g_ParamVer.param[15];		//마킹을 찾기 위한 Threshold
	g_Param.m_dDotMarkSize				=g_ParamVer.param[16];		//마킹Size
	g_Param.m_nDotLimitUp				=g_ParamVer.param[17];		//평활화후 Value Up이 이값보다 크면 마크아님
	g_Param.m_nDotLimitDn				=g_ParamVer.param[18];		//평활화후 Value Dn이 이값보다 크면 마크아님
	g_Param.m_nDotLevelChange			=g_ParamVer.param[19];		//마킹을 없애지 않고 Level만 변경
	if(g_Param.m_nDotMarkTh<5) g_Param.m_nDotMarkTh=5;
	if(g_Param.m_nDotLevelChange<0 || g_Param.m_nDotLevelChange>10) g_Param.m_nDotLevelChange=0;
	//-----------------------------------------------------------------------------------------------------------
#else
	//자동밝기 설정(Param20=g_ParamVer.param[19]-------------------
	if(g_ParamVer.param[19]>0)
	{
		g_Param.m_nSetAutoExposure = 1;
		if(g_ParamVer.param[19]>=30)
			g_System.m_nGrabBright     = g_ParamVer.param[19];
		else
			g_System.m_nGrabBright     = g_System.m_nGrabBrightBK;
	}
	else
	{
		g_Param.m_nSetAutoExposure = 0;
		g_System.m_nGrabBright		   = g_System.m_nGrabBrightBK;
	}
	//--------------------------------------------------------------
#endif





	//사선라인 꺽임 검사-----------------------------------------------------
	//for(i=0;i<10;i++)
	//{
	//	g_Param.m_dLineThresholdAngle[i]=(int)g_ParamVer.param[150+i];	//꺽임.
	//}


	//찍힘/눌림 관련 파라미터------
	ApplyData_Press();
	
	//라인스캔쿠닉검사파라미터-------------------------------------------------------------------
	g_Param.m_nCunicInspArea		=(int) g_ParamVer.param[11];	//쿠닉검사Size(Pixel)
	if(g_Param.m_nCunicInspArea<24) g_Param.m_nCunicInspArea=24;
	if(g_Param.m_nCunicInspArea>80) g_Param.m_nCunicInspArea=80;

	for(i=0;i<10;i++)
	{
		g_Param.m_nCunicValue1[i]	=255;	//이물성쿠닉Value	
		g_Param.m_dCunicStd1[i]		=0;		//이물성쿠닉 STD
		g_Param.m_dCunicStd[i]		=g_ParamVer.param[110+i];			//그냥 쿠닉 STD
		if(g_Param.m_dCunicStd[i]>100) g_Param.m_dCunicStd[i]/=1000.0;
	}
	//--------------------------------------------------------------------------------------------

	ApplyData_Line(); //표준라인검출


#ifndef MY_RECV
	//군집데이타 (군집사용하려면 살린다)--------------------------------------------------------------
	g_Param.m_nGroupAreaTh	=(int) g_ParamVer.param[5];	//군집 Th
	if(g_Param.m_nGroupAreaTh==0)  g_Param.m_nGroupAreaTh=g_Param.m_nAreaThUp;

	for(i=0;i<10;i++)
	{	
		g_Param.m_nGroupAreaArray[i]	=(int)g_ParamVer.param[i+130]; //군집데이타 : 군집 영역
		g_Param.m_nGroupCountArray[i]	=(int)g_ParamVer.param[i+140]; //군집데이타 : 군집 갯수
		g_Param.m_nGroupValueArray[i]	=(int)g_ParamVer.param[i+150]; //군집데이타 : 군집 Value
		g_Param.m_nGroupSizeArray[i]	=g_ParamVer.param[i+160];	   //군집데이타 : 군집 Size
	}
	//-----------------------------------------------------------------------------------------------
#endif


#if INSPECT_TYPE==USE_NORMAL
	//Blob Scratch-----------------
	ApplyData_BlobScratch();
#endif
}

#if INSPECT_TYPE==TAC_LINE
void ApplyOptic9() 
{
	int i;

	g_Param.m_nBrightMax	=g_ParamVer.param[29];	//최대밝기
	g_Param.m_nBrightMin	=g_ParamVer.param[30];	//최소밝기

	//KNULLING 체크관련
	g_Param.m_nInspectType   =(int)g_ParamVer.param[34]; //1,2,3이면 KNULLING 검사
	g_Param.m_nKnullingValue =(int)g_ParamVer.param[35]; //KNULLING 값
	g_Param.m_dNullingRatioLimit=2.0;
	if(g_Param.m_nNotInspArea && (g_Param.m_nInspectType>0 && g_Param.m_nInspectType<4) ) g_Param.m_nKnullingPC=1;  //널링검사
	else																				  g_Param.m_nKnullingPC=0;  //널링검사하지 않음	

	//찍힘/눌림 관련 파라미터------
	ApplyData_Press();
}
#else
//크로스
void ApplyOptic9() 
{
	int i;

#ifdef USE_COS9_LR

	g_Param.m_nBrightMax	=g_ParamVer.param[29];	//최대밝기
	g_Param.m_nBrightMin	=g_ParamVer.param[30];	//최소밝기


	for(i=0;i<10;i++)
	{
		g_Param.m_dLineThreshold[i]	=g_ParamVer.param[110+i];
		g_Param.m_nBMura[i]			=g_ParamVer.param[120+i];
		g_Param.m_dLineThresholdW[i]=g_ParamVer.param[130+i];
	}
#endif
	

#ifdef INTERFACE_NEXTEYE_SERVER
	g_Param.m_nValueOneUp	=(int)g_ParamVer.param[4];		//강휘점(1Level) Value
	g_Param.m_dSizeOneUp	=g_ParamVer.param[5];			//강휘점(1Level) Size
	g_Param.m_nValueOneDn	=0;								//강흑점(1Level) Value	
	g_Param.m_dSizeOneDn	=0;								//강흑점(1Level) Size
#endif

	//게인값 설정
#ifdef USE_GAIN_SET
	g_Param.m_nGain=g_ParamVer.param[8]; //미세이물 게인값 설정
	if(g_Param.m_nGain<0 && g_Param.m_nGain>2) g_Param.m_nGain=-1;
#endif

	//자동밝기 설정--------------------------------------------
	if(g_ParamVer.param[9]>0)
	{
		g_Param.m_nSetAutoExposure = 1;
		if(g_ParamVer.param[9]>=30)
			g_System.m_nGrabBright     = g_ParamVer.param[9];
		else
			g_System.m_nGrabBright     = g_System.m_nGrabBrightBK;
	}
	else
	{
		g_Param.m_nSetAutoExposure = 0;
		g_System.m_nGrabBright         = g_System.m_nGrabBrightBK;
	}
	//---------------------------------------------------------

	g_Param.m_nAttatchPixel		=(int) g_ParamVer.param[13];	
	if(g_Param.m_nAttatchPixel%2==1)   g_Param.m_nAttatchPixel+=1;
	if(g_Param.m_nAttatchPixel<2)      g_Param.m_nAttatchPixel=2;
	g_Param.m_nSizeMethod		=      g_ParamVer.param[14];	 //Size구하는 방법 0 (가로, 세로, (가로+세로)/2), 1 (장축, 단축, 장축)   2 (장축, 단축, (장축+단축)/2) 
	if(g_Param.m_nSizeMethod<0 || g_Param.m_nSizeMethod>2) g_Param.m_nSizeMethod=0;


	g_Param.m_nShadowTh=0;	//TOT외에는 그림자TH 사용하지 않는다.
#ifdef TOT_USE
	g_Param.m_nShadowOffset			=(int) g_ParamVer.param[26];	
	g_Param.m_nShadowTh				=(int) g_ParamVer.param[27];
	g_Param.m_dShowSkipSize			=	   g_ParamVer.param[28];
	g_Param.m_dShowdowNotSkipSize	=	   g_ParamVer.param[29];
	ApplyData_CrossGroup();		//크로스 군집
#endif


	ApplyData_CrossKkopChim();	//크로스 꼽침
	ApplyData_Cunic();			

	//쿠닉
#ifndef NANJING_OLED
	for(i=0;i<10;i++)
		g_Param.m_nScratch[i]=(int)g_ParamVer.param[90+i];		//크로스 스크라치


	ApplyData_CrossGroup();		//크로스 군집
	
#else

	//크로스 군집 (무조건 1Level)------------------------
	g_Param.m_nGroupAreaTh =(int) g_ParamVer.param[4];
	g_Param.m_nGroupCount  =(int) g_ParamVer.param[5];
	g_Param.m_nGroupArea   =(int) g_ParamVer.param[6];
	g_Param.m_dGroupSize   =g_ParamVer.param[7];
	//---------------------------------------------------

	for(i=0;i<10;i++)
	{
		g_Param.m_dScratchLength[i]=g_ParamVer.param[90+i];		//크로스 스크라치
	}
#endif


}
#endif




//미세이물검사
void ApplyOpticB() 
{
#ifdef USE_MISE_IMUL
	ApplyOptic1();
#endif
}

void ApplyOpticC() 
{
}
void ApplyOpticD() 
{
}
void ApplyOpticE() 
{
}
void ApplyOpticF() 
{
}
void ApplyOpticBCR()
{

}
void ApplyOpticMarkingVision() 
{
#ifdef MARKING_VISION
	g_MakringVision.m_nFrameOffset =g_ParamVer.param[46]/(g_System.m_nImagePartH*g_Param.m_dScaleFactorY); //강마킹거리 (mm단위)
	g_MakringVision.m_nFrameOffset1=g_ParamVer.param[47]/(g_System.m_nImagePartH*g_Param.m_dScaleFactorY); //약마킹거리 (mm단위)
	g_MakringVision.m_dTh          =g_ParamVer.param[9];

	g_MakringVision.m_nROIOffsetX1=100;																   //마킹Check ROI폭 왼쪽 Offset
	if(g_Param.m_dScaleFactorX) g_MakringVision.m_nROIOffsetX2=MARKING_GAP/g_Param.m_dScaleFactorX+100;//마킹Check ROI폭 오른쪽 Offset
	g_MakringVision.m_nROIHeight  =768; // 128짜리 6개
#endif

}


//[표준]찍힘 눌림 데이타
void ApplyData_Press()
{
	int i;

	g_Param.m_nPressMinUp	=(int)g_ParamVer.param[12];			//찍힘 백 최소값
	g_Param.m_nPressMinDn	=(int)g_ParamVer.param[13];			//찍힘 흑 최소값
	if(g_Param.m_nPressMinUp<5) g_Param.m_nPressMinUp=5;
	if(g_Param.m_nPressMinDn<5) g_Param.m_nPressMinDn=5;

	for(i=0;i<10;i++)
	{
#ifdef PRESS_FIRST_USE
		g_Param.m_nPressValue[i]=(int)g_ParamVer.param[110+i];	//찍힘 Value
		g_Param.m_nPressSize[i]	=     g_ParamVer.param[120+i];	//찍힘 Size
#else
		g_Param.m_nPressValue[i]=(int)g_ParamVer.param[130+i];	//찍힘 Value
		g_Param.m_nPressSize[i]	=     g_ParamVer.param[140+i];	//찍힘 Size
#endif
	}

	g_Param.m_nPressInspect=0;	
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i]>0 && (g_Param.m_nPressValue[i]>0 || g_Param.m_nPressSize[i]>0.001))
		{
			g_Param.m_nPressInspect=1; //<-------찍힘검사함.
			break;
		}
	}
}


//[표준]기존 Scratch
void ApplyData_Scratch()
{
	int i;

	for(i=0;i<10;i++)
	{
		g_Param.m_nScratch[i]=(int)g_ParamVer.param[90+i];
	}
}
//[표준]기존 Scratch
void ApplyData_LongScratch()
{
	int i;

	for(i=0;i<10;i++)
	{
		g_Param.m_nLongScratch[i]=(int)g_ParamVer.param[110+i];
	}
}

//[표준]Blob형 Scratch 
void ApplyData_BlobScratch()
{
	int i;
	//Blob Scratch-------------------------------------------------------
	g_Param.m_nScratchOffset=(int)g_ParamVer.param[15];	//스크라치 검사갭
	g_Param.m_nScratchDiff=(int)g_ParamVer.param[16];	//스크라치 밝기차	
	for(i=0;i<10;i++)
	{
		g_Param.m_nScratchValue[i] =(int)g_ParamVer.param[150+i];
		g_Param.m_dScratchLength[i]=(int)g_ParamVer.param[160+i];		
	}
}

//[표준]쿠닉파라미터
void ApplyData_Cunic()
{
	int i;

	g_Param.m_nCunicInspArea		=(int) g_ParamVer.param[11];	//쿠닉검사Size(Pixel)
	if(g_Param.m_nCunicInspArea<24 || g_Param.m_nCunicInspArea>64)
	{
		if(g_Param.m_dScaleFactorX>0.065)       g_Param.m_nCunicInspArea=32;  
		else								    g_Param.m_nCunicInspArea=48;  
	}

#ifdef USE_MISE_IMUL
	if(g_Param.m_nCunicInspArea<24) g_Param.m_nCunicInspArea=24;
	if(g_Param.m_nCunicInspArea>80) g_Param.m_nCunicInspArea=80;
#endif

	g_Param.m_dCunicCanidValue	= g_ParamVer.param[12];				//쿠닉후보값
	for(i=0;i<10;i++)
	{
		g_Param.m_nCunicValue1[i]	=(int)g_ParamVer.param[100+i];	//이물성쿠닉Value	
		g_Param.m_dCunicStd1[i]		=g_ParamVer.param[110+i];		//이물성쿠닉 STD
		if(g_Param.m_dCunicStd1[i]>100) g_Param.m_dCunicStd1[i]/=1000.0;
		g_Param.m_dCunicStd[i]		=g_ParamVer.param[120+i];		//그냥 쿠닉 STD
		if(g_Param.m_dCunicStd[i]>100)  g_Param.m_dCunicStd[i]/=1000.0;

		//쿠닉사용할때는 흑점 검출하지 않음.
//		g_Param.m_nValueDn[i]	=0;		//Black Value
//		g_Param.m_dSizeDn[i]	=0;		//Black Size
	}
}

//[표준]CROSS 군집
void ApplyData_CrossGroup()
{
	int i;

#ifdef USE_COS9_LR	
	g_Param.m_dGroupSize	=      g_ParamVer.param[3];	//군집 Size
	g_Param.m_nGroupArea	=(int) g_ParamVer.param[4];	//군집 영역
	g_Param.m_nGroupAreaTh	=(int) g_ParamVer.param[5];	//군집 Th
	g_Param.m_nGroupCount	=(int) g_ParamVer.param[6];	//군집 갯수	
#else
	#ifdef TOT_USE
		g_Param.m_nGroupAreaTh	=(int) g_ParamVer.param[4];	//군집 Th
		g_Param.m_nGroupCount	=(int) g_ParamVer.param[5];	//군집 갯수
		g_Param.m_nGroupArea	=(int) g_ParamVer.param[6];	//군집 영역
		g_Param.m_dGroupSize	=      g_ParamVer.param[7];	//군집 Size
		g_Param.m_nGroupSizeMethod	=(int) g_ParamVer.param[8];	//군집 Size 검출방법
	#else
		g_Param.m_nGroupAreaTh	=(int) g_ParamVer.param[2];	//군집 Th
		g_Param.m_nGroupCount	=(int) g_ParamVer.param[3];	//군집 갯수
		g_Param.m_nGroupArea	=(int) g_ParamVer.param[6];	//군집 영역
		g_Param.m_dGroupSize	=      g_ParamVer.param[7];	//군집 Size
	#endif
#endif

#ifndef TOT_USE
	if(g_Param.m_nGroupArea<64) g_Param.m_nGroupArea=128;
	if(g_Param.m_dGroupSize<=0) g_Param.m_dGroupSize=0.01;
#endif
}

//[표준]CROSS 꼽침
void ApplyData_CrossKkopChim()
{
	int i;

#ifdef USE_COS9_LR
	g_Param.m_nSlimPercent = 0;	//꼽침
	for(i=0;i<10;i++)
	{
		g_Param.m_dLineThreshold[i]	=(int)g_ParamVer.param[i+110];	//꼽침
	}

#else
	g_Param.m_nSlimPercent = (int)g_ParamVer.param[18];	//꼽침
	for(i=0;i<10;i++)
	{
		g_Param.m_dLineThreshold[i]	=(int)g_ParamVer.param[i+130];	//꼽침
	}
#endif
}

//[표준]Line 검출 (10 Frame중 5개이상일때)
void ApplyData_Line()
{
	int i;
	//Line-------------------------------------------------------
	for(i=0;i<10;i++)
	{
		g_Param.m_nLineUpValue[i] =(int)g_ParamVer.param[150+i]; 
		g_Param.m_nLineDnValue[i] =(int)g_ParamVer.param[160+i];	
		if(g_Param.m_nLineDnValue[i]<0) g_Param.m_nLineDnValue[i]*=-1;
	}
}



void ApplyDakanoParameter()
{
	int i;

	g_Param.m_nSetAutoExposure =g_ParamVer.param[9];	//1이면 자동밝기설정 
	g_Param.m_nDKTapeValue     =g_ParamVer.param[10];	//Tape Value
	g_Param.m_nDKJumpEdgeValue =g_ParamVer.param[11];	//Jummping Value
	g_Param.m_nDKSlopePixel    =g_ParamVer.param[12];   //기울기Pixel
	g_Param.m_nDKLineCnt	   =g_ParamVer.param[13];   //검사라인수
	g_Param.m_nDKNoUseJumpping =g_ParamVer.param[14];   //Tape점핑 으로 표현되는 부분은 검사결과를 사용안함.
	g_Param.m_nDKNoUseJumpping =0;


	if(g_Param.m_nDKLineCnt==0) g_Param.m_nDKLineCnt=50;
	for(i=0;i<10;i++)
	{
		g_Param.m_nLineUpValue[i]	=(int)g_ParamVer.param[130+i];		//라인값
		g_Param.m_nLineDnValue[i]	=	  g_ParamVer.param[140+i];		//상대라인값
	}


	if(g_Param.m_nDKTapeValue==0)	  g_Param.m_nDKTapeValue=500;
	if(g_Param.m_nDKJumpEdgeValue==0) g_Param.m_nDKJumpEdgeValue=500;

}


void	ApplyMulTaeParameter()
{
	int i;
	g_Param.m_nDKTapeValue=100000;
	g_Param.m_nDKJumpEdgeValue=100000;
	g_Param.m_nDKNoUseJumpping=1;
	g_Param.m_nDKMarkingTest=0;
	if(g_Param.m_nDKLineCnt==0) g_Param.m_nDKLineCnt=1;

	g_Param.m_nDivisionY	=(int) g_ParamVer.param[4];	//Y분할
	if(g_Param.m_nDivisionY<1) g_Param.m_nDivisionY=1;
	if(g_Param.m_nDivisionY>4) g_Param.m_nDivisionY=4;

	g_Param.m_nGroupAreaTh	=(int) g_ParamVer.param[5];	//군집 Th
	if(g_Param.m_nGroupAreaTh==0)  g_Param.m_nGroupAreaTh=g_Param.m_nAreaThUp;
	g_Param.m_nSaveN		   =g_ParamVer.param[8];

	//자동밝기 설정-----------------------------------------------
	if(g_ParamVer.param[9]>0)
	{
		g_Param.m_nSetAutoExposure = 1;
		if(g_ParamVer.param[9]>=20)
			g_System.m_nGrabBright     = g_ParamVer.param[9];
		else
			g_System.m_nGrabBright     = g_System.m_nGrabBrightBK;
	}
	else
	{
		g_Param.m_nSetAutoExposure = 0;
		g_System.m_nGrabBright         = g_System.m_nGrabBrightBK;
	}
	//------------------------------------------------------------
	
	for(i=0;i<10;i++)
	{
		g_Param.m_nUseLevel[i]		=(int)g_ParamVer.param[60+i];		//사용하는 Level
		g_Param.m_nLineUpValue[i]	=(int)g_ParamVer.param[70+i];		//라인값
		g_Param.m_nLineDnValue[i]	=	  g_ParamVer.param[80+i];		//상대라인값

		g_Param.m_nValueUp[i]		=(int)g_ParamVer.param[90+i];		//White Value
		g_Param.m_dSizeUp[i]		=	  g_ParamVer.param[100+i];		//White Size

		g_Param.m_nGroupAreaArray[i]	=(int)g_ParamVer.param[i+110]; //군집데이타 : 군집 영역
		g_Param.m_nGroupCountArray[i]	=(int)g_ParamVer.param[i+120]; //군집데이타 : 군집 갯수
		g_Param.m_nGroupValueArray[i]	=(int)g_ParamVer.param[i+130]; //군집데이타 : 군집 Value
		g_Param.m_nGroupSizeArray[i]	=g_ParamVer.param[i+140];	   //군집데이타 : 군집 Size
		g_Param.m_nSTDThUp[i]			=g_ParamVer.param[i+150];
	}
	g_Param.m_nBrightMax	=g_ParamVer.param[49];	//최대밝기
	g_Param.m_nBrightMin	=g_ParamVer.param[50];	//최소밝기
}


/*
kipopress.ini에 있어야 할 내용등
//----------------------------------
[NEL-201]  //보내는 것
send1=1    //NEL-801로 보냄.
offset1=0
[NEL-202]  //보내는 것
send1=2    //NEL-802로 보냄.
offset1=0

[RECV_IP]
IP[1]=71
IP[2]=72
..
iP[20]=90
//----------------------------------
[NEL-801]  //받는 것
recv1=1    //NEL-201에서 받음
[NEL-802]  //받는 것
recv1=2    //NEL-202에서 받음
//----------------------------------
*/


void LoadKipoPress()
{
#if defined MY_SEND || defined MY_RECV
	int i;
	int send[4], recv[4], IP[31];
	double offset[4];
	int nData;
	TCHAR sReturn[20];
	CString str, sParam;

	
	g_Kipo.m_nKipoPress			=(int)g_ParamVer.param[8];
	//if(g_Kipo.m_nKipoPress && g_Param.m_nKipoPressLevelMarking) g_Kipo.m_nKipoPress=2;	//기포눌림 사용하고, 기포눌림 2~9Level 도 마킹하면 g_Kipo.m_nKipoPress=2

	for(i=0;i<4;i++)
	{
#ifdef MY_SEND

	#ifdef INTERFACE_NEXTEYE_SERVER
		str.Format(_T("NEL-%d%02d"),SEND_OPTIC, g_Temp.m_nPCNum); 
	#else
		str.Format(_T("COS-%d%02d"),SEND_OPTIC, g_Temp.m_nPCNum); 
	#endif
		sParam.Format(_T("send%d"), i+1);			//보내는 PC (1,2,..)  (SEND 에서 사용)
		::GetPrivateProfileString(str,sParam,_T("0"),sReturn, 10, KIPOPRESS_PATH);
		send[i]=_tstoi(sReturn);

		sParam.Format(_T("offset%d"), i+1);			//(SEND 에서 사용)
		::GetPrivateProfileString(str,sParam,_T("0"),sReturn, 10, KIPOPRESS_PATH);
		offset[i]=_tstof(sReturn);

		if(i==0 && send[i]==0) send[i]=g_Temp.m_nPCNum%100;
#endif

#ifdef MY_RECV

	#ifdef INTERFACE_NEXTEYE_SERVER
		str.Format(_T("NEL-%d%02d"),RECV_OPTIC, g_Temp.m_nPCNum); 
	#else
		str.Format(_T("COS-%d%02d"),RECV_OPTIC, g_Temp.m_nPCNum); 
	#endif
		sParam.Format(_T("recv%d"), i+1);		    //받은 PC (1,2,..) (COS7에서 사용)
		::GetPrivateProfileString(str,sParam,_T("0"),sReturn, 10, KIPOPRESS_PATH);
		recv[i]=_tstoi(sReturn);

		if(i==0 && recv[i]==0) recv[i]=g_Temp.m_nPCNum%100;  //KIPOPRESS_PATH 파일이 없으면 1:1 임. 자기 PC번호로 받음 1,2,3
#endif
	}
#endif
	
#ifdef MY_RECV
	    //보내는 포트가 
		g_Kipo.m_nPort[0]=recv[0]%100;
		g_Kipo.m_nPort[1]=recv[1]%100;
		g_Kipo.m_nPort[2]=recv[2]%100;
		g_Kipo.m_nPort[3]=recv[3]%100;
#endif

#ifdef MY_SEND	
		//SEND PC는 RECV PC의 IP를 알아야 하므로 IP정보 있어야 함. ex)  IP[1]=71    (RECV IP(100.0.0.71) 마지막 번호는 71)
		for(i=0;i<20;i++)
		{
			sParam.Format(_T("IP[%d]"), i+1);
			::GetPrivateProfileString(_T("RECV_IP"),sParam,_T("0"),sReturn, 10, KIPOPRESS_PATH);
			IP[i+1]=_tstoi(sReturn);
		}

		//현재 PC가 SEND이면 X오버랩되는 RECV PC번호(1~N) 읽어온다. (FOV(SEND) 가 FOV(RECV)보다 작으면 2개, 많으면 3개)
		g_Kipo.m_nPort[0]=send[0]%100;
		g_Kipo.m_nPort[1]=send[1]%100;
		g_Kipo.m_nPort[2]=send[2]%100;
		g_Kipo.m_nPort[3]=0;
		g_Kipo.m_KIPODistance[0]=offset[0];
		g_Kipo.m_KIPODistance[1]=offset[1];	
		g_Kipo.m_KIPODistance[2]=offset[2];	
		g_Kipo.m_KIPODistance[3]=0;
		for(i=1;i<=20;i++)
			g_Kipo.m_COSRecvIP[i]=IP[i];	//RECV PC의 IP를 알아야 보낼 수 있음.
#endif
	
}

#ifdef USE_20_LEVEL_MARK
int l_nSubMarkType[10];
void SetSubMarkType(int nVisionLine)
{
	int i;
	for(i=0;i<10;i++) l_nSubMarkType[i]=2;

	if(nVisionLine==COSA)
	{
		l_nSubMarkType[0]=3;  //투영반사 A1불량만 해당됨
	}
}

void ApplySubMarking( int N)
{
	int j;
	int nDefectType;
	float dValue, dSize;
	int nOK[3];

	nDefectType=g_Defect.m_Defect[N].defect_class%CLASSDEV-1;  
	if(nDefectType<0 || g_MarkSub.nDefectTypeMarking[nDefectType]==0) return;

	g_DefectSend.m_Defect[N].dTemp[0]=g_DefectSend.m_Defect[N].dTemp[1]=0;
	dValue=g_Defect.m_Defect[N].value; if(dValue<0) dValue*=-1;
	dSize =g_Defect.m_Defect[N].size;


	for(j=0;j<20;j++)
	{
		nOK[0]=nOK[1]=nOK[2]=1;

		if(g_MarkSub.nMarking[nDefectType][j]) //마킹설정되어 있으면
		{
			if(l_nSubMarkType[nDefectType]==3) //투영반사는 여기없음 백/흑 데이타 새로 만들어야 함.
			{



			}
			else
			{
				if(g_MarkSub.dValueMin[nDefectType][j]>0 || g_MarkSub.dValueMax[nDefectType][j]>0)
				{
					if( dValue>=g_MarkSub.dValueMin[nDefectType][j] && dValue<=g_MarkSub.dValueMax[nDefectType][j]) nOK[0]=1;
					else																							nOK[0]=0;
				}
				if(g_MarkSub.dSizeMin[nDefectType][j]>0 || g_MarkSub.dSizeMax[nDefectType][j]>0)
				{
					if( dSize>=g_MarkSub.dSizeMin[nDefectType][j] && dSize<=g_MarkSub.dSizeMax[nDefectType][j]) nOK[1]=1;
					else																						nOK[1]=0;
				}
				
				if( nOK[0]*nOK[1])  g_DefectSend.m_Defect[N].dTemp[0]=g_DefectSend.m_Defect[N].dTemp[1]=5;
			}
		}
	}
}

int SaveSubParam()
{
	int ret=-1;
	int i, j;
	int nWriteDefect;

	CStdioFile f;
	CString str;

	if(f.Open(SUBMARKPARAM_PATH_RESERVE, CFile::modeCreate|CFile::typeText|CFile::modeWrite))
	{
		for(i=0;i<MAX_OPTIC*10;i++)
		{
			nWriteDefect=0;
			for(j=0;j<20;j++)
			{
				if(g_MarkSubAll.nMarking[i][j]) nWriteDefect=1;
			}
			if(nWriteDefect==0) continue;
			//g_MarkSubAll.nDefectType[i]=i+1;
			if(g_MarkSubAll.nDefectType[i]%10==0) continue;

			if(g_MarkSubAll.nDefectType[i]>110)
				str.Format(_T("[DEFECT_B%d]\n"), g_MarkSubAll.nDefectType[i]%10);  //A1, A2 ...A9   //현재는 A까지 밖에 없으므로
			else if(g_MarkSubAll.nDefectType[i]>100)
				str.Format(_T("[DEFECT_A%d]\n"), g_MarkSubAll.nDefectType[i]%10);  //A1, A2 ...A9   //현재는 A까지 밖에 없으므로
			else
				str.Format(_T("[DEFECT_%02d]\n"), g_MarkSubAll.nDefectType[i]);

			f.WriteString(str);
			for(j=0;j<20;j++)
			{
				if(g_MarkSubAll.nMarking[i][j])
				{
					str.Format(_T("nMarking[%d]=%d\n"),    j,g_MarkSubAll.nMarking[i][j]);
					f.WriteString(str);
					str.Format(_T("dValueMin[%d]=%.3lf\n"),j,g_MarkSubAll.dValueMin[i][j]);
					f.WriteString(str);
					str.Format(_T("dValueMax[%d]=%.3lf\n"),j,g_MarkSubAll.dValueMax[i][j]);
					f.WriteString(str);
					str.Format(_T("dSizeMin[%d]=%.3lf\n"), j,g_MarkSubAll.dSizeMin[i][j]);
					f.WriteString(str);
					str.Format(_T("dSizeMax[%d]=%.3lf\n"), j,g_MarkSubAll.dSizeMax[i][j]);
					f.WriteString(str);
				}
			}
		}
		f.Close();
		ret=0;
	}

	return ret;
}



int ReadMarkParameter(int nReadFromReserve)
{
	int ret=-1;
	CString str, str_name, sParam;
	CString ModelName, strModel;
	CString sParameterPath;

	int i, j, k, nTemp, nOptic;
	double dTemp;
	int nSheetLength=0;
	int nCnt=0;
	int nNonZeroCount=0;
	
	int nCycle[15];
	TCHAR sReturn[20], sName[200]; 

	if(nReadFromReserve)
	{
		if(CopyFile(SUBMARKPARAM_PATH_RESERVE, SUBMARKPARAM_PATH, false))
		{
			DeleteFile(SUBMARKPARAM_PATH_RESERVE);
			WriteLog(_T("Copy MarkParam Resever -> Main Folder"));
		}
	}
	ReadSubParam();

	return ret;
}

//사용시 유의사항: 투영반사 찍힘에서는 데이타를 3개 사용함.(다른건 1~2)
//군집은 사용안함.
int ReadSubParam()
{
	int ret=-1;

	int i, j;
	int nDefectType, nMarking;
	int nOptic;
	CString sDefect, sType;
	TCHAR sReturn[20];

#ifdef INTERFACE_NEXTEYE_SERVER
	if(g_Temp.m_nPCOptic==NELA)      nOptic=10;
	else if(g_Temp.m_nPCOptic==NELB) nOptic=11;
	else if(g_Temp.m_nPCOptic==NELC) nOptic=12;
	else							 nOptic=g_Temp.m_nPCOptic;
#else
	if(g_Temp.m_nPCOptic==COSA)      nOptic=10;			//COSA는 0 이라서 특별대우
	else							 nOptic=g_Temp.m_nPCOptic;
#endif

	for(i=0;i<9;i++) //9맞음 한 광학계 최대 DEFECT종류
	{
		nDefectType=nOptic*CLASSDEV+i+1;


#ifdef INTERFACE_NEXTEYE_SERVER
		if(g_Temp.m_nPCOptic==COSA)
			sDefect.Format(_T("DEFECT_A%d"), i+1);
		else
			sDefect.Format(_T("DEFECT_%02d"), nDefectType);
#else
		if(g_Temp.m_nPCOptic==COSA)
			sDefect.Format(_T("DEFECT_A%d"), i+1);
		else
		sDefect.Format(_T("DEFECT_%02X"), nDefectType);
#endif

		nMarking=0;
							
		for(j=0;j<20;j++)
		{
			sType.Format(_T("nMarking[%d]"),j);
			::GetPrivateProfileString(sDefect, sType, _T("0"),	sReturn, 20, SUBMARKPARAM_PATH); 
			g_MarkSub.nMarking[i][j]=(int)_tstoi(sReturn);

			if(g_MarkSub.nMarking[i][j])	//마킹Check없으면 읽을 필요없음
			{
				nMarking=1;
				sType.Format(_T("dValueMin[%d]"),j);
				::GetPrivateProfileString(sDefect, sType, _T("0"),	sReturn, 20, SUBMARKPARAM_PATH); 
				g_MarkSub.dValueMin[i][j]=_tstof(sReturn);

				sType.Format(_T("dValueMax[%d]"),j);
				::GetPrivateProfileString(sDefect, sType, _T("0"),	sReturn, 20, SUBMARKPARAM_PATH); 
				g_MarkSub.dValueMax[i][j]=_tstof(sReturn);

				sType.Format(_T("dSizeMin[%d]"),j);
				::GetPrivateProfileString(sDefect, sType, _T("0"),	sReturn, 20, SUBMARKPARAM_PATH); 
				g_MarkSub.dSizeMin[i][j]=_tstof(sReturn);

				sType.Format(_T("dSizeMax[%d]"),j);
				::GetPrivateProfileString(sDefect, sType, _T("0"),	sReturn, 20, SUBMARKPARAM_PATH); 
				g_MarkSub.dSizeMax[i][j]=_tstof(sReturn);

				if(l_nSubMarkType[i]==3)
				{
					g_MarkSub.dWValueMin[i][j]=(int)(g_MarkSub.dValueMin[i][j]/1000);
					g_MarkSub.dBValueMin[i][j]=(int)g_MarkSub.dValueMin[i][j]%1000;
					g_MarkSub.dWValueMax[i][j]=(int)(g_MarkSub.dValueMax[i][j]/1000);
					g_MarkSub.dBValueMax[i][j]=(int)g_MarkSub.dValueMax[i][j]%1000;
				}
			}
		}

		g_MarkSub.nDefectTypeMarking[i]=nMarking;
	}

	ret=0;
	return ret;
}
int CopySubMarkParam50(int N)
{
	int i,j;
	int nOffset=N*50;

	for(i=0;i<50;i++)
		for(j=0;j<20;j++)
		{
			g_MarkSubAll.nDefectType[N+i] =g_MarkSubNet.nDefectType[i];
			g_MarkSubAll.nMarking[N+i][j] =g_MarkSubNet.nMarking[i][j];
			g_MarkSubAll.dValueMin[N+i][j]=g_MarkSubNet.dValueMin[i][j];
			g_MarkSubAll.dValueMax[N+i][j]=g_MarkSubNet.dValueMax[i][j];
			g_MarkSubAll.dSizeMin[N+i][j] =g_MarkSubNet.dSizeMin[i][j];
			g_MarkSubAll.dSizeMax[N+i][j] =g_MarkSubNet.dSizeMax[i][j];
		}

	return 1;
}
#else
void SetSubMarkType(int nVisionLine){}
void ApplySubMarking( int N){}
int SaveSubParam() { return 0;}
int ReadMarkParameter(int nReadFromReserve){ return 0;}
int ReadSubParam() { return 0;}
int CopySubMarkParam50(int N) {return 0;}
#endif