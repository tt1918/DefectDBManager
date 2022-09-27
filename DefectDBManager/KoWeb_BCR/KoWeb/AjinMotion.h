
#define AXIS_COUNT		4
typedef struct MOTIONCONFIG_
{
	int     m_nHomeStart;				//1 이면 프로그램 시작시 Home을 잡는다.
	double  m_dJogMaxSpeed[AXIS_COUNT];	//JOG 최대속도
	double  m_dHomeOffset[AXIS_COUNT];  //Home 잡은후 이동하여 그 위치를 Home이라함
	long	m_nMM2Pulse[AXIS_COUNT];	//1MM 이동시 모션 Pulse 수
	double  m_dEncoderscale;

	double m_dStageMLimit[AXIS_COUNT];
	double m_dStagePLimit[AXIS_COUNT];

	double m_dDoneLimit[AXIS_COUNT];;		//모션이 가려고 하는 위치에서 이 값 이하로 차이가 나면 도착한 것으로 간주함 


}MOTIONDATA;

int LoadMotionData();
int MotionOpen();
void MotionClose();