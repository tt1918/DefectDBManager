// Packet.h: interface for the CPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PACKET_H__0086D72B_3BA5_46C1_A01B_A01D89AD6CDC__INCLUDED_)
#define AFX_PACKET_H__0086D72B_3BA5_46C1_A01B_A01D89AD6CDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_BADIMAGE_FILENAME			100


#define NM_DATA							1
#define NM_TAPEPOS						101			//@TAPE 
#define NM_INS_START					2
#define NM_INS_STOP						3



#define NM_CALIB_START					9
#define NM_CALIB_RESULT					10
#define NM_INS_PRESET					11

#define NM_LOTDATA_CLEAR				12	// 클라이언트의 LotData폴더의 내용을 모두 삭제 한다. 
#define NM_PARAM_INSTANT				14


#define NM_SPLICE						19	// 클라이언트가 동작 상태에서 시작신호를 받은 경우
#define MN_DATA_LOT_NAME				40	// 서버에서 lot name 보내주는 데이터
#define MN_DATA_NEB						41	// 401에서 BROADCASTING
#define MN_DATA_LOT_NAME2				44	// 서버에서 lot name 보내주는 데이터
#define NM_UNWIND						45	//UNWINDER 신호 @200824

#define NM_ACK_ALIVE					49
#define NM_DATA_FRAME					50	//Frame수 Data Receive Packet
#define COS_MARKING_DATA				51	//마킹서버에서 마킹비전으로
#define NM_TAPE2						53	//이형필름
#define NM_TAPE3						60	//보호필름 
#define NM_ACK_CAM_AVERAGE				54
#define NM_ACK_LIGHT_DATA				56	//Double : AVE	STR : Edge (mm)
#define NM_MOTION_CHECK					58	//Double : AVE	STR : Edge (mm)
#define NM_MOTION_CHECK_ONE				59	//해당하는 PC만 


#define NM_CROSS2BRIGHT_ADJUST			70
#define NM_CROSSBRIGHT_ADJUS			71
#define NM_CROSSBRIGHT_AUTO				73
#define NM_CROSSBRIGHT_MANUAL			74

//#define NM_ASK_IMAGE					90      //임기선사용하던 것 

#define NM_EDGE_SEND					90		//3D서버에서 리딩 여부 보내줌 


//NEW_NG_IMAGE(8)=======================================================================
#define NM_ASK_NG_IMAGE					35		//KYW 추가함 Client에 불량영상 요청		
#define NM_SEND_TEACH_FILE				36		//KYW 티칭파일을 송부한다.
#define NM_CLIENT_CHECK					37		//Client가 살았는지 체크한다.
#define NM_FRAME_ADJUST					38		//Frame수를 맞춘다
//=================================================================================


#define NM_TXTFILE_ASK					39		//서버로부터 TXT파일 보내달라는 신호 받음 

#define NM_NULLING_MARK					82		//NULLING보내는 데이타 
#define NM_KIPOPRESS					91		//COS7번에서 기포관련 정보를 보내줌 
#define NM_HOLE_INFO					94		//HOLE정보 


#define NM_CHECK_LOT_CHANGE				102

#define NM_SEND_HDD_ERROR				103		//HDD 15%미만을 알림 (서버에 추가해야 함)
#define NM_TEST_SYNC_FRAME				113

#define PARAM_NAME_UPDATE				"Update"
#define PARAM_NAME_HDD_REMAINING		"HDD_remaining"

#define PARAM_NAME_MODEL				"Model"
#define PARAM_NAME_LOT_NUMBER			"Lot"
#define PARAM_DUMMY						"NULL"
#define PARAM_RETRY_TIME				"RETRY_TIME"
#define PARAM_CLIENT_MODE				"CLIENT_MODE"

#define NM_TESTFRAME				149
#define NM_FOUNDTAPE				150

#define LR_GET_DATA					100			//LR 서버에서 라인데이타를 달라고 함 (128x2)
#define LR_CLIENT_CHECK				101			//LR 서버에서 클라이언트 살아 있는지 체크
#define LR_PORT_RESET				104			//LR 서버에서 통신포트 다시 열라고 명령함.

#define NM_SEND_MARK_FILE			153			//SubMarkParam.ini 보냄. //USE_20_LEVEL_MARK

#define NM_DATA_TWOEDGE				180			//에지 두개 보냄
#define NM_LOT_RESET				120

#define NM_FULL_IMG_SAVE_100		301
#define NM_FULL_IMG_SAVE_ALL		302
#define NM_LINE_SETTING				303

#define NM_BCR_SEARCH_LOT			401
#define NM_BCR_SEARCH_LOT_ACK		402
#define NM_BCR_SEARCH_MODEL			403
#define NM_BCR_SEARCH_MODEL_ACK		404
#define NM_BCR_SEND_PARAMETER_FILE	405
#define NM_BCR_SEND_PARAMETER_FILE_DEST_ACK	406

#define NM_BCR_BCD_READING_ERROR	410
#define NM_BCR_BCD_READING_LOW		411
#define NM_BCR_BCD_READING_RATE		412
#define NM_BCR_FILM_WIDTH			413
#define NM_BCR_LOT_DATA_ERROR		414

#define NM_FORCE_BCR_NOT_MATCHED_ALRAM 430
#define NM_FORCE_BCR_NOT_EQAUL_BCR_DIR 431

#define NM_NITTO_AI_IMAGE			500
#define NM_NITTO_AI_RESULT			501
#define NM_NITTO_DATA_BCR			502


//순서가 NEXTEYE와 같음 (value->sizey, sizeX->dTemp[0], sizeY->dTemp[1]
//Size는 136 Byte
typedef struct tagDEFECT
{
	int			defect_class;
	float		x_pos;
	float		y_pos;
	float		size;
	float		value;


#ifdef USE_PACKET_ANSI
	char		filename[MAX_BADIMAGE_FILENAME];	//서버상태에 따라서 1BYTE 또는 2BYTE
#else
	TCHAR		filename[MAX_BADIMAGE_FILENAME];
#endif

	float       dTemp[2];							//dTemp[4]를 dTemp[2]로 줄이고, sizeX,sizeY로 구분함. dTemp[0], dTemp[1]는 마킹여부로 사용함
	float       sizeX;
	float       sizeY;

#ifdef USE_NITTO_AI
	int			ai_result;			//ai판정 결과, 0: 미적용, 1 ~ 10: Ai 판정 NG, 11 ~ 20: Ai 판정 SKIP
	float		ai_score[10];		//AI판정 스코어
#endif
#ifdef SEND_14_DATA					//현재는 2동 코팅 3호기만 사용
	float       dTemp1[14];
#endif

}DEFECT;

typedef struct tagMarkDefect
{
	int			defect_class;	// 결점 class
	float		x_pos;			// 불량 위치 X mm
	float		y_pos;			// 불량 위치 Y mm
	float		size;			//  불량 크기
	float		offset;			// 불량 원단장 y 위치 mm
	char		fltid[4];		// FLTID(ANSI)
	int			mark;			// 0:미마킹, 1:마킹
}MARK_DEFECT;

typedef struct tagAreaMarkDefect
{
	int			defect_class;	// 결점 class
	float		x;				// 구간 마킹 시작 위치 X mm
	float		y;				// 구간 마킹 시작 위치 Y mm
	float		width;			// 구간 마킹 넓이 mm
	float		height;			// 구간 마킹 높이 mm
	float		offset;			// 원단장 시작 위치 y mm
	int			mark;			// 0:미마킹, 1:마킹
}AREA_MARK_DEFECT;

typedef struct tagBcrDefect
{
	int			defect_class;						// 결점 class
	float		x;									// 불량 위치 X mm
	float		y;									// 불량 위치 Y mm
	float		width;								// BCD 넓이
	float		height;								// BCD 높이
	int			type;								// BCD 리딩 옵션
	int			position;							// BCD 위치 (M)
	char		fileName[MAX_BADIMAGE_FILENAME];	// 파일명(ANSI)
	int			mark;								// 0:미마킹, 1:마킹
}BCR_DEFECT;


class CPacket  
{
public:
	CPacket();
	virtual ~CPacket();

	// 수신시에 코드값만 일단 분해.
	int		GetCode(char* buf, int buf_len);
	//----------------------------------------------------------------------------------------------------------------------------------------------

	void MakePacketData();		//NG데이타 전달용 Packet만드는 함수
#ifdef BARCODE_VISION
	void MakeBcrPacketData();	// BCR Vision Data 전달용 Patcket만드는 함수
#endif
#ifdef RESULT_DELAY
	void MakePacketDataDelay(int N); //NG데이타 전달용 Packet만드는 함수 (한 Frame뒤에 보냄)
#endif
	void MakeInfoPacket(int code, TCHAR *param_name, double val);

#ifdef USE_SK_BAT
	void MakeTwoEdgePacketData(double dOuterEdge, double dInnerEdge);
#endif

	//NEW_NG_IMAGE(9)====================================================
	void MakeNGImagePacket(LPBYTE fm, int nFileType, int nImageSize);
	void MakeReturnPacket(int nCode, int nOK);
	//================================================================
	//slitter
	void MakeKnullingPosPacket(int nCode, double dPos);

	// Client -> Server
	void	MakeDataPacket(int frame_num, double dEdge, int defect_cnt,int lotNo, int* defect_class, float* defect_x, float* defect_y, float* size_x, float* size_y, float *pTemp0, float *pTemp1, float *pTemp2, float *pTemp3, char** defect_image);
	// Server에서 받은 데이터 분해.
	void	PullDataPacket(char* buf, int buf_len, int& code, int& frame_num, int& defect_cnt);
	//----------------------------------------------------------------------------------------------------------------------------------------------

	//Tape위치 
	void CPacket::MakeTapePacket(int frame_num, float dPosY);

	//	Server -> Client
	void	MakeInstantPacket(int code, LPCSTR param_name, double val);
	void	MakeInstantPacket(int code, LPCSTR param_name, LPSTR str_val, int len);
	//  Client에서 데이터 분해.
	void	PullInstantPacket(char* buf, int buf_len, int& code, LPSTR &param_name, double &val, LPSTR &str_val);
	void	PullInstantPacket(char* buf, int buf_len, int& code, LPSTR &param_name, int *nFrame);
	//----------------------------------------------------------------------------------------------------------------------------------------------
	
	void    MakeSubLogDataPacket(CString strData);
	void	PullAiNgImagePacket(char* buf, int buf_len, int& frameNum, int& totalNg, int& aiClass, float* scores);

	void	PullBcrSearchLotPacket(char* buf, int buf_len, char* lotName, bool& lotNext, int& vendor, bool& useES, bool& useTG, bool& useETC, bool& useBMark);
	void	MakeAckBcrSearchLotPacket(CString data, double progress);
	
	void	PullBcrSearchModelPatcket(char* buf, int buf_len, char* lotName);
	void	MakeAckBcrSearchModelPacket(CString data, double progress);

	void	PullBcrParamPacket(char* buf, int buf_len);

	void	MakeBcrReadingRatePacket(double val);

	void	MakeLRLinePacket(int nCode, int nFrame);


	char*	m_pBuf;
	int		m_nBuflen;

	int		m_nFull_packet_length;
	int		m_nPacket_code;
	int		m_nFrame_num;

	int		m_nDefect;
	DEFECT	*m_pDefect_data;

#ifdef BARCODE_VISION
	MARK_DEFECT*		m_pBcrDefect_Data;
	AREA_MARK_DEFECT*	m_pBcrAreaDel_Data;
	BCR_DEFECT			m_BcrInfo_Data;
#endif

protected:

};

#endif // !defined(AFX_PACKET_H__0086D72B_3BA5_46C1_A01B_A01D89AD6CDC__INCLUDED_)
