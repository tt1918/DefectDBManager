// Packet.cpp: implementation of the CPacket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Packet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPacket::CPacket()
{
	m_pBuf = NULL;
	m_pDefect_data = NULL;

#ifdef BARCODE_VISION
	m_pBcrDefect_Data = nullptr;
	m_pBcrAreaDel_Data = nullptr;
#endif
}

CPacket::~CPacket()
{
	if(m_pDefect_data)
		delete [] m_pDefect_data;

	if(m_pBuf)
		delete [] m_pBuf;

#ifdef BARCODE_VISION
	if (m_pBcrDefect_Data != nullptr)
		delete[] m_pBcrDefect_Data;
	if (m_pBcrAreaDel_Data != nullptr)
		delete[] m_pBcrAreaDel_Data;
#endif
}

int CPacket::GetCode(char* buf, int buf_len)
{
	int nCode=0;
	memcpy(&nCode, buf, 4);
	return nCode;
}

// defect_cnt=1045603의 경우  뒤의 03은 불량수, 10456은 Edge 1045.6mm 를 가리킨다.
void CPacket::MakePacketData()
{
	double dEdge;
	int  i, j, nEdgeDefectCombine;   //Edge와 Defect수를 합치다.

	CTime time = CTime::GetCurrentTime();
	CString sTime = time.Format("[%H-%M-%S]");
	int lotNo=0;
	TCHAR sFileName[MAX_BADIMAGE_FILENAME];
	TCHAR sFileName1[MAX_BADIMAGE_FILENAME];
	int nLotLen;
	char cLotName[20], cPcName[10], cFileName[MAX_BADIMAGE_FILENAME];

	if(g_Temp.m_nFoundEdge>0) dEdge=g_Temp.m_nFoundEdge*g_Param.m_dScaleFactorX+g_Param.m_dCamStartPosX;
	else					  dEdge=g_System.m_nImageW/2*g_Param.m_dScaleFactorX+g_Param.m_dCamStartPosX;
	nEdgeDefectCombine=((int)(dEdge*10))*100+g_DefectSend.m_nDefectCount;

	m_nDefect      = g_DefectSend.m_nDefectCount;
	m_nPacket_code = NM_DATA;
	m_nFrame_num   = g_DefectSend.m_nFrameNum;
	m_nFull_packet_length = m_nBuflen =  4 + 4 + 4 + 4 + 4 + sizeof(DEFECT)*g_DefectSend.m_nDefectCount;	

	if(m_pBuf)
		delete [] m_pBuf;
	m_pBuf = new char[m_nBuflen];
	memset(m_pBuf, 0, m_nBuflen);

	memcpy(m_pBuf,    &m_nBuflen, 4);
	memcpy(m_pBuf+4,  &m_nPacket_code, 4);

	memcpy(m_pBuf+8,  &lotNo, 4);
	
	memcpy(m_pBuf+12, &g_DefectSend.m_nFrameNum, 4);

	memcpy(m_pBuf+16, &nEdgeDefectCombine, 4);    //m_nDefect -> nEdgeDefectCombine 교체 

	if(g_DefectSend.m_nDefectCount>0)
	{
		if(m_pDefect_data) delete [] m_pDefect_data;
		m_pDefect_data = new DEFECT[g_DefectSend.m_nDefectCount];
		
		for(i=0; i<g_DefectSend.m_nDefectCount; i++)	
		{
			m_pDefect_data[i].defect_class	= g_DefectSend.m_Defect[i].defect_class;
			m_pDefect_data[i].x_pos			= g_DefectSend.m_Defect[i].x_pos;
			m_pDefect_data[i].y_pos			= g_DefectSend.m_Defect[i].y_pos;
			m_pDefect_data[i].size			= g_DefectSend.m_Defect[i].size;
			m_pDefect_data[i].value			= g_DefectSend.m_Defect[i].value;
		
			m_pDefect_data[i].dTemp[0]		= g_DefectSend.m_Defect[i].dTemp[0];		//마킹데이타 0 기존  1 마킹  2 미마킹  3 가변마킹
			m_pDefect_data[i].dTemp[1]		= g_DefectSend.m_Defect[i].dTemp[1];		//마킹데이타 0 기존  1 마킹  2 미마킹  3 가변마킹			
			m_pDefect_data[i].sizeX			= g_DefectSend.m_Defect[i].sizeX;
			m_pDefect_data[i].sizeY			= g_DefectSend.m_Defect[i].sizeY;

	#ifdef INTERFACE_NEXTEYE_SERVER		
			_stprintf(sFileName1, CA2W(g_DefectSend.m_Defect[i].filename));
			_stprintf(sFileName, _T("%s\\%s\\%s"), g_Temp.m_slotName, g_Temp.m_sMyComName,  sFileName1);
			sprintf(m_pDefect_data[i].filename, CW2A(sFileName));
	#else
		#ifdef USE_PACKET_ANSI
			_stprintf(sFileName1, CA2W(g_DefectSend.m_Defect[i].filename));
			_stprintf(sFileName, _T("%s\\%s\\%s"), g_Temp.m_slotName, g_Temp.m_sMyComName,  sFileName1);
			sprintf(m_pDefect_data[i].filename, CW2A(sFileName));
		#else
				#ifdef USE_COSS_SERVER_CLIENT_NEL
							_stprintf(sFileName, _T("%s\\%s\\%s"), g_Temp.m_slotName, g_Temp.m_sMyComName,  g_DefectSend.m_Defect[i].filename);
							_tcscpy(m_pDefect_data[i].filename, sFileName);	
				#else
							_tcscpy(m_pDefect_data[i].filename, g_DefectSend.m_Defect[i].filename);		
				#endif
		#endif
	#endif
			
		}	
	}

	if(g_DefectSend.m_nDefectCount) memcpy(m_pBuf+20, m_pDefect_data, sizeof(DEFECT)*g_DefectSend.m_nDefectCount);	
}

#ifdef BARCODE_VISION
void CPacket::MakeBcrPacketData()	// BCR Vision Data 전달용 Patcket만드는 함수
{
	double dEdge;
	int  i, j, nEdgeDefectCombine;   //Edge와 Defect수를 합치다.

	CTime time = CTime::GetCurrentTime();
	CString sTime = time.Format("[%H-%M-%S]");
	int lotNo = 0;
	TCHAR sFileName[MAX_BADIMAGE_FILENAME];
	TCHAR sFileName1[MAX_BADIMAGE_FILENAME];
	int nLotLen;
	char cLotName[20], cPcName[10], cFileName[MAX_BADIMAGE_FILENAME];
	int offset = 0;
	int dataSize;

	if (g_Temp.m_nFoundEdge > 0) dEdge = g_Temp.m_nFoundEdge * g_Param.m_dScaleFactorX + g_Param.m_dCamStartPosX;
	else					  dEdge = g_System.m_nImageW / 2 * g_Param.m_dScaleFactorX + g_Param.m_dCamStartPosX;
	
	m_nDefect = g_DefectSend.m_nDefectCount;
	m_nPacket_code = NM_NITTO_DATA_BCR;
	m_nFrame_num = g_DefectSend.m_nFrameNum;
	m_nFull_packet_length = m_nBuflen = 4 * 8 + (sizeof(MARK_DEFECT) * g_DefectSend.m_nBcrDefectCount) +
										(sizeof(AREA_MARK_DEFECT) * g_DefectSend.m_nBcrAreaDefectCount) +
										(sizeof(BCR_DEFECT) * g_DefectSend.m_nBcrCount);

	if (m_pBuf)
		delete[] m_pBuf;
	m_pBuf = new char[m_nBuflen];
	memset(m_pBuf, 0x00, m_nBuflen);

	memcpy(m_pBuf, &m_nBuflen, 4);									// 0
	memcpy(m_pBuf + 4, &m_nPacket_code, 4);							// 1
	memcpy(m_pBuf + 8, &lotNo, 4);									// 2
	memcpy(m_pBuf + 12, &g_DefectSend.m_nFrameNum, 4);				// 3
	memcpy(m_pBuf + 16, &dEdge, 4);									// 4 
	memcpy(m_pBuf + 20, &g_DefectSend.m_nBcrCount, 4);				// 5
	memcpy(m_pBuf + 24, &g_DefectSend.m_nBcrAreaDefectCount, 4);	// 6
	memcpy(m_pBuf + 28, &g_DefectSend.m_nBcrDefectCount, 4);		// 7
	
	if (g_DefectSend.m_nBcrCount > 0)
	{
		memcpy(&m_BcrInfo_Data, &g_DefectSend.m_BMarkDefect, sizeof(BCR_DEFECT));
#ifdef INTERFACE_NEXTEYE_SERVER		
		_stprintf(sFileName1, CA2W(g_DefectSend.m_BMarkDefect.fileName));
		_stprintf(sFileName, _T("%s\\%s\\%s"), g_Temp.m_slotName, g_Temp.m_sMyComName, sFileName1);
		sprintf(m_BcrInfo_Data.fileName, CW2A(sFileName));
#else
#ifdef USE_PACKET_ANSI
		_stprintf(sFileName1, CA2W(g_DefectSend.m_BMarkDefect.fileName));
		_stprintf(sFileName, _T("%s\\%s\\%s"), g_Temp.m_slotName, g_Temp.m_sMyComName, sFileName1);
		sprintf(m_BcrInfo_Data.fileName, CW2A(sFileName));
#else
#ifdef USE_COSS_SERVER_CLIENT_NEL
		_stprintf(sFileName, _T("%s\\%s\\%s"), g_Temp.m_slotName, g_Temp.m_sMyComName, g_DefectSend.m_BMarkDefect.fileName);
		_tcscpy(m_BcrInfo_Data.fileName, sFileName);
#else
		_tcscpy(m_BcrInfo_Data.fileName, g_DefectSend.m_BMarkDefect.fileName);
#endif
#endif
#endif
		dataSize = sizeof(BCR_DEFECT) * g_DefectSend.m_nBcrCount;
		memcpy(m_pBuf + 28, &m_BcrInfo_Data, dataSize);
		offset += dataSize;
	}

	if (g_DefectSend.m_nBcrAreaDefectCount > 0)
	{
		if (m_pBcrAreaDel_Data) delete[] m_pBcrAreaDel_Data;
		m_pBcrAreaDel_Data = new AREA_MARK_DEFECT[g_DefectSend.m_nBcrAreaDefectCount];
		memcpy(m_pBcrAreaDel_Data, g_DefectSend.m_BcrAreaDefect, sizeof(AREA_MARK_DEFECT) * g_DefectSend.m_nBcrAreaDefectCount);

		dataSize = sizeof(AREA_MARK_DEFECT) * g_DefectSend.m_nBcrAreaDefectCount;
		memcpy(m_pBuf + 28 + offset, m_pBcrAreaDel_Data, dataSize);
		offset += dataSize;
	}


	if (g_DefectSend.m_nBcrDefectCount > 0)
	{
		if (m_pBcrDefect_Data) delete[] m_pBcrDefect_Data;
		m_pBcrDefect_Data = new MARK_DEFECT[g_DefectSend.m_nBcrDefectCount];
		memcpy(m_pBcrDefect_Data, g_DefectSend.m_BcrDefect, sizeof(MARK_DEFECT) * g_DefectSend.m_nBcrDefectCount);

		dataSize = sizeof(MARK_DEFECT) * g_DefectSend.m_nBcrDefectCount;
		memcpy(m_pBuf + 28 + offset, m_pBcrDefect_Data, dataSize);
		offset += dataSize;
	}
}
#endif

#ifdef RESULT_DELAY
void CPacket::MakePacketDataDelay(int N)
{
	double dEdge;
	int  i, j, nEdgeDefectCombine;   //Edge와 Defect수를 합치다.

	CTime time = CTime::GetCurrentTime();
	CString sTime = time.Format("[%H-%M-%S]");
	int lotNo=0;
	TCHAR sFileName[MAX_BADIMAGE_FILENAME];
	TCHAR sFileName1[MAX_BADIMAGE_FILENAME];
	int nLotLen;
	char cLotName[20], cPcName[10], cFileName[MAX_BADIMAGE_FILENAME];

	if(g_Temp.m_nFoundEdge>0) dEdge=g_Temp.m_nFoundEdgeDelay[N]*g_Param.m_dScaleFactorX+g_Param.m_dCamStartPosX;
	else					  dEdge=g_System.m_nImageW/2*g_Param.m_dScaleFactorX+g_Param.m_dCamStartPosX;
	nEdgeDefectCombine=((int)(dEdge*10))*100+g_DefectSendDelay[N].m_nDefectCount;

	m_nDefect      = g_DefectSendDelay[N].m_nDefectCount;
	m_nPacket_code = NM_DATA;
	m_nFrame_num   = g_DefectSendDelay[N].m_nFrameNum;
	m_nFull_packet_length = m_nBuflen =  4 + 4 + 4 + 4 + 4 + sizeof(DEFECT)*g_DefectSendDelay[N].m_nDefectCount;	

	if(m_pBuf)
		delete [] m_pBuf;
	m_pBuf = new char[m_nBuflen];
	memset(m_pBuf, 0, m_nBuflen);

	memcpy(m_pBuf,    &m_nBuflen, 4);
	memcpy(m_pBuf+4,  &m_nPacket_code, 4);

	memcpy(m_pBuf+8,  &lotNo, 4);
	
	memcpy(m_pBuf+12, &g_DefectSendDelay[N].m_nFrameNum, 4);

	memcpy(m_pBuf+16, &nEdgeDefectCombine, 4);    //m_nDefect -> nEdgeDefectCombine 교체 

	if(g_DefectSendDelay[N].m_nDefectCount>0)
	{
		if(m_pDefect_data) delete [] m_pDefect_data;
		m_pDefect_data = new DEFECT[g_DefectSendDelay[N].m_nDefectCount];
		
		for(i=0; i<g_DefectSendDelay[N].m_nDefectCount; i++)	
		{
			m_pDefect_data[i].defect_class	= g_DefectSendDelay[N].m_Defect[i].defect_class;
			m_pDefect_data[i].x_pos			= g_DefectSendDelay[N].m_Defect[i].x_pos;
			m_pDefect_data[i].y_pos			= g_DefectSendDelay[N].m_Defect[i].y_pos;
			m_pDefect_data[i].size			= g_DefectSendDelay[N].m_Defect[i].size;
			m_pDefect_data[i].value			= g_DefectSendDelay[N].m_Defect[i].value;
		
			m_pDefect_data[i].dTemp[0]		= g_DefectSendDelay[N].m_Defect[i].dTemp[0];		//마킹데이타 0 기존  1 마킹  2 미마킹  3 가변마킹
			m_pDefect_data[i].dTemp[1]		= g_DefectSendDelay[N].m_Defect[i].dTemp[1];		//마킹데이타 0 기존  1 마킹  2 미마킹  3 가변마킹			
			m_pDefect_data[i].sizeX			= g_DefectSendDelay[N].m_Defect[i].sizeX;
			m_pDefect_data[i].sizeY			= g_DefectSendDelay[N].m_Defect[i].sizeY;

	#ifdef INTERFACE_NEXTEYE_SERVER		
			_stprintf(sFileName1, CA2W(g_DefectSendDelay[N].m_Defect[i].filename));
			_stprintf(sFileName, _T("%s\\%s\\%s"), g_Temp.m_slotName, g_Temp.m_sMyComName,  sFileName1);
			sprintf(m_pDefect_data[i].filename, CW2A(sFileName));
	#else

#ifdef USE_COSS_SERVER_CLIENT_NEL
			_stprintf(sFileName, _T("%s\\%s\\%s"), g_Temp.m_slotName, g_Temp.m_sMyComName,  g_DefectSendDelay[N].m_Defect[i].filename);
			_tcscpy(m_pDefect_data[i].filename, sFileName);	
#else

	#ifdef USE_PACKET_ANSI
			_stprintf(sFileName1, CA2W(g_DefectSendDelay[N].m_Defect[i].filename));
			_stprintf(sFileName, _T("%s"), sFileName1);
			sprintf(m_pDefect_data[i].filename, CW2A(sFileName));
	#else
			_tcscpy(m_pDefect_data[i].filename, g_DefectSendDelay[N].m_Defect[i].filename);		
	#endif
#endif

	#endif

#ifdef USE_NITTO_AI
			m_pDefect_data[i].ai_result = g_DefectSendDelay[N].m_Defect[i].ai_result;
			memcpy(m_pDefect_data[i].ai_score, g_DefectSendDelay[N].m_Defect[i].ai_score, sizeof(g_DefectSendDelay[N].m_Defect[i].ai_score));
#endif
		}	
	}

	if(g_DefectSendDelay[N].m_nDefectCount) memcpy(m_pBuf+20, m_pDefect_data, sizeof(DEFECT)*g_DefectSendDelay[N].m_nDefectCount);	
}
#endif

#ifdef USE_SK_BAT
void CPacket::MakeTwoEdgePacketData(double dOuterEdge, double dInnerEdge)
{
	float dOutEdge, dInEdge;

	dOutEdge=dOuterEdge;  
	dInEdge =dInnerEdge;  

	m_nPacket_code = NM_DATA_TWOEDGE;
	m_nFull_packet_length = m_nBuflen =  4 + 4 + 4 + 4 + 4;	  //길이, 코드, Frame, 바깥 Edge, 안 Edge

	if(m_pBuf)
		delete [] m_pBuf;
	m_pBuf = new char[m_nBuflen];
	memset(m_pBuf, 0, m_nBuflen);

	memcpy(m_pBuf,    &m_nBuflen, 4);
	memcpy(m_pBuf+4,  &m_nPacket_code, 4);	
	memcpy(m_pBuf+8,  &g_Temp.m_nGrabFrameID, 4);
	memcpy(m_pBuf+12, &dOutEdge, 4);
	memcpy(m_pBuf+16, &dInEdge, 4);
}
#endif

/*
//Defect_cnt를 쪼개서 사용한다.
// defect_cnt=1045603의 경우  뒤의 03은 불량수, 10456은 Edge 1045.6mm 를 가리킨다.
void CPacket::MakeDataPacket(int frame_num, double dEdge, int defect_cnt,int lotNo, int* defect_class, float* defect_x, float* defect_y, float* size_x, float* size_y, float *pTemp0, float *pTemp1, float *pTemp2, float *pTemp3, char** defect_image)
{
	int  nEdgeDefectCombine=((int)(dEdge*10))*100+defect_cnt;   //Edge와 Defect수를 합치다.
	//------------------------------------
	//	검사 데이터 전송용 패킷.
	m_nPacket_code = NM_DATA;
	m_nDefect = defect_cnt;
	m_nBuflen = m_nFull_packet_length =  4 + 4 + 4 + 4 + 4 + sizeof(DEFECT)*m_nDefect;	//chcho
	
	if(m_pBuf)
		delete [] m_pBuf;
	m_pBuf = new char[m_nBuflen];
	memset(m_pBuf, 0, m_nBuflen);

	memcpy(m_pBuf, &m_nFull_packet_length, 4);
	memcpy(m_pBuf+4, &m_nPacket_code, 4);

	memcpy(m_pBuf+8, &lotNo, 4);
	
	memcpy(m_pBuf+12, &frame_num, 4);

#ifdef A_FILM_ONLY
	memcpy(m_pBuf+16, &defect_cnt, 4);
#else
	memcpy(m_pBuf+16, &nEdgeDefectCombine, 4);    //m_nDefect -> nEdgeDefectCombine 교체 
#endif


	
	if(defect_cnt)
	{
		if(m_pDefect_data) delete [] m_pDefect_data;
		m_pDefect_data = new DEFECT[m_nDefect];
		
		for(int i=0; i<m_nDefect; i++)	
		{
			m_pDefect_data[i].defect_class	= defect_class[i];
			m_pDefect_data[i].x_pos			= defect_x[i];
			m_pDefect_data[i].y_pos			= defect_y[i];
			m_pDefect_data[i].size			= size_x[i];
			m_pDefect_data[i].value			= size_y[i];

			//Slit데이타--------------------------------------
			m_pDefect_data[i].dTemp[0]		= pTemp0[i];		//마킹데이타 0 기존  1 마킹  2 미마킹  3 가변마킹
			m_pDefect_data[i].dTemp[1]		= pTemp1[i];			
			//------------------------------------------------

			strcpy(m_pDefect_data[i].filename, defect_image[i]);		
		}		
		memcpy(m_pBuf+20, m_pDefect_data, sizeof(DEFECT)*m_nDefect);
	}
}
*/

//@TAPE : Packet만듦
void CPacket::MakeTapePacket(int frame_num, float dPosY)
{
	m_nPacket_code = NM_TAPEPOS;
	m_nBuflen =   4 + 4 + 4 + 4;	//chcho
	
	if(m_pBuf)
		delete [] m_pBuf;
	m_pBuf = new char[m_nBuflen];

	memset(m_pBuf, 0, m_nBuflen);

	memcpy(m_pBuf, &m_nBuflen, 4);
	memcpy(m_pBuf+4, &m_nPacket_code, 4);	
	memcpy(m_pBuf+8, &frame_num, 4);
	memcpy(m_pBuf+12, &dPosY, 4);
}

void CPacket::PullDataPacket(char* buf, int buf_len, int& code, int& frame_num, int& defect_cnt)
{
	if(m_pBuf)
		delete [] m_pBuf;
	m_nBuflen = buf_len;
	m_pBuf = new char[m_nBuflen];
	memcpy(m_pBuf, buf, m_nBuflen);
	//----------------------------------------------

	memcpy(&code, m_pBuf+4, 4);
	m_nPacket_code = code;
	if(code !=NM_DATA)	//데이터 패킷이 아닌 커맨드 패킷인 경우.
		return;
	
	memcpy(&frame_num, m_pBuf+8, 4);
	m_nFrame_num = frame_num;
	memcpy(&defect_cnt, m_pBuf+12, 4);
	m_nDefect = defect_cnt;

	if(defect_cnt>0)
	{
		m_pDefect_data = new DEFECT[defect_cnt];
		memcpy(m_pDefect_data, m_pBuf+16, sizeof(DEFECT)*defect_cnt);
	}
	else
		m_pDefect_data = NULL;

	//-----------------------------------------------

}

void CPacket::MakeInstantPacket(int code, LPCSTR param_name, double val)
{
	//----------------------------------------
	//	코드값과 더블 값 하나를 전송하는 패킷.
	int name_len = strlen(param_name);
	m_nPacket_code = code;
	m_nBuflen = m_nFull_packet_length = 4 + 4 + 4 + name_len + 8;
	
	if(m_pBuf)
		delete [] m_pBuf;
	m_pBuf = new char[m_nBuflen];
	memset(m_pBuf, 0, m_nBuflen);

	memcpy(m_pBuf, &m_nFull_packet_length, 4);
	memcpy(m_pBuf+4, &m_nPacket_code, 4);
	memcpy(m_pBuf+8, &name_len, 4);
	memcpy(m_pBuf+12, param_name, name_len);
	memcpy(m_pBuf+12+name_len, &val, 8);
}



void CPacket::MakeInstantPacket(int code, LPCSTR param_name, LPSTR str_val, int len)
{
	//----------------------------------------
	//	코드값과 문자열 값 하나를 전송하는 패킷.
	int name_len = strlen(param_name);
	m_nPacket_code = code;
	m_nBuflen = m_nFull_packet_length = 4 + 4 + 4 + name_len + 4 + len;
	
	if(m_pBuf)
		delete [] m_pBuf;
	m_pBuf = new char[m_nBuflen];
	memset(m_pBuf, 0, m_nBuflen);


	memcpy(m_pBuf, &m_nFull_packet_length, 4);
	memcpy(m_pBuf+4, &m_nPacket_code, 4);
	memcpy(m_pBuf+8, &name_len, 4);
	memcpy(m_pBuf+12, param_name, name_len);

	memcpy(m_pBuf+12+name_len, &len, 4);
	memcpy(m_pBuf+16+name_len, str_val, len);
}

void CPacket::PullInstantPacket(char* buf, int buf_len, int& code, LPSTR &param_name, double &val, LPSTR &str_val)
{
	//---------------------------------------------
	//	코드값 하나와 더블 값 하나를 수신하는 패킷.
	if(m_pBuf)
		delete [] m_pBuf;
	m_nBuflen = buf_len;
	m_pBuf = new char[m_nBuflen];
	memcpy(m_pBuf, buf, m_nBuflen);

	//----------------------------------------------
	memcpy(&code, m_pBuf+4, 4);
	
	int name_len=0;
	memcpy(&name_len, m_pBuf+8, 4);
//	if(param_name) delete [] param_name;

#ifdef INTERFACE_NEXTEYE_SERVER
	param_name = new char[name_len+1];
	memset(param_name, 0, name_len+1);
#else
	//YKS 20200723
	param_name = new char[name_len+2];
	memset(param_name, 0, name_len+2);
#endif	

	memcpy(param_name, m_pBuf+12, name_len);

	if( (!strcmp(param_name, PARAM_NAME_MODEL)) || 
		(!strcmp(param_name, PARAM_NAME_LOT_NUMBER)) || 
		(!strcmp(param_name, PARAM_NAME_UPDATE)) )
	{
		int val_len;
		memcpy(&val_len, m_pBuf+12+name_len, 4);
//		if(str_val) delete [] str_val;
		str_val = new char[val_len+1];
		memset(str_val, 0, val_len+1);

		memcpy(str_val, m_pBuf+16+name_len, val_len);
	}
	else
		memcpy(&val, m_pBuf+12+name_len, 8);
	//-----------------------------------------------
}

void CPacket::PullInstantPacket(char* buf, int buf_len, int& code, LPSTR &param_name, int *nFrame)
{
	//---------------------------------------------
	//	코드값 하나와 더블 값 하나를 수신하는 패킷.
	if(m_pBuf)
		delete [] m_pBuf;
	m_nBuflen = buf_len;
	m_pBuf = new char[m_nBuflen];
	memcpy(m_pBuf, buf, m_nBuflen);

	//----------------------------------------------
	memcpy(&code, m_pBuf+4, 4);
	
	int name_len=0;
	memcpy(&name_len, m_pBuf+8, 4);
//	if(param_name) delete [] param_name;

#ifdef INTERFACE_NEXTEYE_SERVER
	param_name = new char[name_len+1];
	memset(param_name, 0, name_len+1);
#else
	//YKS 20200723
	param_name = new char[name_len+2];
	memset(param_name, 0, name_len+2);
#endif

	memcpy(param_name, m_pBuf+12, name_len);
	memcpy(nFrame, m_pBuf+12+name_len, sizeof(int)*10);
	//-----------------------------------------------
}

//NEW_NG_IMAGE(7)===============================================================================
//nFileType : 서버에서 보내준 데이타를 그대로 다시 돌려보내준다.
void CPacket::MakeNGImagePacket(LPBYTE fm, int nFileType, int nImageSize)
{
	//------------------------------------
	//	검사 데이터 전송용 패킷.
	m_nPacket_code = NM_ASK_NG_IMAGE;
	m_nBuflen = m_nFull_packet_length = 4 + 4 + 4+ nImageSize;
	
	if(m_pBuf)
		delete [] m_pBuf;
	m_pBuf = new char[m_nBuflen];
	memset(m_pBuf, 0, m_nBuflen);

	memcpy(m_pBuf, &m_nFull_packet_length, 4);
	memcpy(m_pBuf+4, &m_nPacket_code, 4);
	memcpy(m_pBuf+8, &nFileType, 4);
	memcpy(m_pBuf+12, fm, nImageSize);
}

//Teach파일 변경후 OK결과 보내기 위해 만듦. [길이][코드][OK]  OK:1이면 정상 
void CPacket::MakeReturnPacket(int nCode, int nOK)
{
	//------------------------------------
	//	검사 데이터 전송용 패킷.
	m_nPacket_code = nCode;
	m_nBuflen = m_nFull_packet_length = 4 + 4 + 4;
	
	if(m_pBuf)
		delete [] m_pBuf;
	m_pBuf = new char[m_nBuflen];
	memset(m_pBuf, 0, m_nBuflen);

	memcpy(m_pBuf, &m_nFull_packet_length, 4);
	memcpy(m_pBuf+4, &m_nPacket_code, 4);
	memcpy(m_pBuf+8, &nOK, 4);
}

void CPacket::MakeKnullingPosPacket(int nCode, double dPos)
{
	//------------------------------------
	//	검사 데이터 전송용 패킷.
	m_nPacket_code = nCode;
	m_nBuflen = m_nFull_packet_length = 4 + 4 + 8;
	
	if(m_pBuf)
		delete [] m_pBuf;
	m_pBuf = new char[m_nBuflen];
	memset(m_pBuf, 0, m_nBuflen);
	
	memcpy(m_pBuf, &m_nFull_packet_length, 4);
	memcpy(m_pBuf+4, &m_nPacket_code, 4);
	memcpy(m_pBuf+8, &dPos, 8);
}



void CPacket::MakeInfoPacket(int code, TCHAR *param_name, double val)
{
	//	코드, char, double 값1개 를 전송
	int name_len;
	char sBuffer[256];
	
	WideCharToMultiByte(CP_ACP, 0, param_name, 256, sBuffer, 256, NULL, NULL); //char로 변경함
	name_len=strlen(sBuffer);

	m_nPacket_code = code;
	m_nBuflen = m_nFull_packet_length = 4 + 4 + 4 + name_len + 8;

	if(m_pBuf)
		delete [] m_pBuf;
	m_pBuf = new char[m_nBuflen];
	memset(m_pBuf, 0, m_nBuflen);

	memcpy(m_pBuf, &m_nFull_packet_length, 4);
	memcpy(m_pBuf+4, &m_nPacket_code, 4);
	memcpy(m_pBuf+8, &name_len, 4);
	memcpy(m_pBuf+12, sBuffer, name_len);
	memcpy(m_pBuf+12+name_len, &val, 8);
}

void CPacket::MakeSubLogDataPacket(CString strData)
{
	m_nPacket_code = 140;
	m_nBuflen = m_nFull_packet_length = 4 + 4 + strData.GetLength();

	if(m_pBuf)
		delete [] m_pBuf;
	m_pBuf = new char[m_nBuflen];
	memset(m_pBuf, 0, m_nBuflen);

	memcpy(m_pBuf, &m_nFull_packet_length, 4);
	memcpy(m_pBuf+4, &m_nPacket_code, 4);

	memcpy(m_pBuf+8, strData, strData.GetLength());
}

void CPacket::PullAiNgImagePacket(char* buf, int buf_len, int& frameNum, int& totalNg, int& aiClass, float* scores)
{
	//------------------------------------
	//	AI 데이터 수신용 패킷.
	if (m_pBuf)
		delete[] m_pBuf;
	m_nBuflen = buf_len;
	m_pBuf = new char[m_nBuflen];
	memcpy(m_pBuf, buf, m_nBuflen);

	//----------------------------------------------
	memcpy(&frameNum, m_pBuf + 8, 4);
	memcpy(&totalNg, m_pBuf + 12, 4);
	memcpy(&aiClass, m_pBuf + 16, 4);
	memcpy(scores, m_pBuf + 20, sizeof(float) * totalNg * aiClass);
	//-----------------------------------------------
}

#ifdef LR_SERVER_EXIST

void CPacket::MakeLRLinePacket(int nCode, int nFrame)
{
	//------------------------------------
	//	검사 데이터 전송용 패킷.
	int nCol, nRow;
	int pitch=4096;
	LPBYTE fm;

#ifdef WEB_DAKANO_MULTAE
	nRow=nFrame%2048;
	nCol=nFrame/2048;
#else
	nRow=nFrame%1000;
	nCol=nFrame/1000;
#endif

	fm=g_fmMAP+nRow*pitch+nCol*64;
	m_nPacket_code = nCode;
	m_nBuflen = m_nFull_packet_length = 4 + 4 + 4 + 128*2;
	
	if(m_pBuf)
		delete [] m_pBuf;
	m_pBuf = new char[m_nBuflen];
	memset(m_pBuf, 0, m_nBuflen);

	memcpy(m_pBuf, &m_nFull_packet_length, 4);
	memcpy(m_pBuf+4, &m_nPacket_code, 4);
	memcpy(m_pBuf+8, &nFrame, 4);
	memcpy(m_pBuf+12,  fm, 64);	//White

#ifdef WEB_DAKANO_MULTAE
	memcpy(m_pBuf+12+128, fm, 128);	//white  똑같은거 2번 보내줌
#else
	memcpy(m_pBuf+12+64, fm+1024*pitch , 64);	//Black
#endif
}

#endif

void CPacket::PullBcrSearchLotPacket(char* buf, int buf_len, char* lotName, bool& lotNext, int& vender, bool& useES, bool& useTG, bool& useETC, bool& useBMark)
{
	if (m_pBuf)
		delete[] m_pBuf;
	m_nBuflen = buf_len;
	m_pBuf = new char[m_nBuflen];
	memcpy(m_pBuf, buf, m_nBuflen);

	int dataLen;
	//----------------------------------------------
	memcpy(&dataLen, m_pBuf + 8, 4);
	memcpy(lotName, m_pBuf + 12, dataLen);
	memcpy(&lotNext, m_pBuf + 12 + dataLen, 1);
	memcpy(&vender, m_pBuf + 12 + dataLen + 1, 4);
	memcpy(&useES, m_pBuf + 12 + dataLen + 1 + 4, 1);
	memcpy(&useTG, m_pBuf + 12 + dataLen + 1 + 4 + 1, 1);
	memcpy(&useETC, m_pBuf + 12 + dataLen + 1 + 4 + 2, 1);
	memcpy(&useBMark, m_pBuf + 12 + dataLen + 1 + 4 + 3, 1);
	//-----------------------------------------------
}

void CPacket::MakeAckBcrSearchLotPacket(CString data, long progress)
{
	m_nPacket_code = NM_BCR_SEARCH_LOT_ACK;
	int dataLen = data.GetLength() * 2;	// UNICODE 처리
	m_nBuflen = m_nFull_packet_length = 4 + 4 + dataLen + 8;

	if (m_pBuf)
		delete[] m_pBuf;
	m_pBuf = new char[m_nBuflen];
	memset(m_pBuf, 0, m_nBuflen);

	memcpy(m_pBuf, &m_nFull_packet_length, 4);
	memcpy(m_pBuf + 4, &data, dataLen);
	memcpy(m_pBuf + 8 + dataLen, &progress, sizeof(long));
}

void CPacket::PullBcrSearchModelPatcket(char* buf, int buf_len, char* lotName)
{
	if (m_pBuf)
		delete[] m_pBuf;
	m_nBuflen = buf_len;
	m_pBuf = new char[m_nBuflen];
	memcpy(m_pBuf, buf, m_nBuflen);

	int dataLen;
	//----------------------------------------------
	memcpy(&dataLen, m_pBuf + 8, 4);
	memcpy(lotName, m_pBuf + 12, dataLen);
	//-----------------------------------------------
}

void CPacket::MakeAckBcrSearchModelPacket(CString data, long progress)
{
	m_nPacket_code = NM_BCR_SEARCH_MODEL_ACK;
	int dataLen = data.GetLength() * 2;	// UNICODE 처리
	m_nBuflen = m_nFull_packet_length = 4 + 4 + dataLen + 8;

	if (m_pBuf)
		delete[] m_pBuf;
	m_pBuf = new char[m_nBuflen];
	memset(m_pBuf, 0, m_nBuflen);

	memcpy(m_pBuf, &m_nFull_packet_length, 4);
	memcpy(m_pBuf + 4, &data, dataLen);
	memcpy(m_pBuf + 8 + dataLen, &progress, sizeof(long));
}