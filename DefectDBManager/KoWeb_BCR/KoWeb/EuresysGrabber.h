#pragma once

#if GRABBER_TYPE==EURESYS_GRABBER

#include <EuresysSharedGenTL.h>
#include <EGrabber.h>

using namespace Euresys;

//Application buffer
#define MAX_BUFFER			3
//Board grab buffer
#define MAX_BOARD_BUFFER	3

class CCoaxlink;
class CGrabber  
{
	friend class CCoaxlink;
public:	
	CGrabber(CWnd* pwnd=NULL);
	virtual ~CGrabber();
	bool Open(bool flag, HWND hWnd);
	void Close(bool flag);

	void HookStart();
	void HookStop();

	void SetExposureTime(int nUserExpo, int nApply);
	void SetYOffset(int nOffset);
	LPBYTE GrabImagePtr();
	void SetTrigger(int flag);
	void SetTriggerMode(int nCam, bool bPeriod);

private:
	CCoaxlink* m_pCoaxlink;
	LPBYTE	m_buffer[MAX_BUFFER];
	int m_width;
	int m_height;
	int m_nImageInverse;
	int m_nGrabFrameSub;
};

typedef enum
{
	MODE_CONTINUOUS,
	MODE_HARDTRIGGER_FG,	//Encoder Signal into Frame Grabber
	MODE_HARDTRIGGER_CAM,	//Encoder Signal into Camera
	MODE_CONFIG_FILE,		//설정파일을 로드
} Grabmode_t;

class CCoaxlinkGrabber : public EGrabber<CallbackMultiThread>
{
public:
	CCoaxlinkGrabber(EGenTL &GenTL, int interfaceIndex, int deviceIndex, int dataStreamIndex, void* pParent);
	~CCoaxlinkGrabber(void);


private:
	unsigned char*		m_poImgPtr;
	void*				m_pParent;

public:

	static std::string formatTimestamp(uint64_t timestamp);

	virtual void onNewBufferEvent(const NewBufferData &data);
	virtual void onIoToolboxEvent(const IoToolboxData &data);
	virtual void onCicEvent(const CicData &data);
	virtual void onDataStreamEvent(const DataStreamData &data);
	virtual void onCxpInterfaceEvent(const CxpInterfaceData &data);
};

class CCoaxlink
{
public:
	CCoaxlink(void* parent);
	virtual ~CCoaxlink(void);
	int ListCards();

	/***************************************************************/
	/* Attribute                                                   */
private:
	void*								m_pParent;
	int									m_Pitch;
	int									m_Width;
	int									m_Height;
	bool								m_bOpen;
	bool								m_bGrabbing;
	int									m_nContext;
	CCoaxlinkGrabber*					m_pCoaxlinkGrabber;
	int									m_nBuffer_cnt;
	Grabmode_t							m_Grabmode;
	EGenTL								m_GenTL;
	int									m_NewExposure;
	int									m_Exposure;
	int									m_nGrabIndex;
	int									m_nSubCopyIndex;
	int									m_nSubGrabIndex;
	/* Attribute                                                   */
	/***************************************************************/

	/***************************************************************/
	/* Basic functions                                             */
public:
	bool								Open(int nGrabberID, int nCameraID);
	void								Close(void);
	void								Grab(uint64_t grab_cnt = GENTL_INFINITE);	
	void								GenerateSoftTrigger(void);
	void								Stop(void);
	void								SetAllEvent();
	void								SetConfigure(Grabmode_t mode);
	void								SetExposureTime(int nUserExpo);
	void								OnImageEvent(unsigned char* pBuffer);
	int									GetGrabIndex();

	//inline function
	bool								IsOpen(){return m_bOpen;}
	bool								IsGrabbing(){return m_bGrabbing;}
	Grabmode_t							GetGrabmode(void) { return m_Grabmode; }
	template<typename M> int			getInteger(const std::string &feature){return m_pCoaxlinkGrabber->getInteger<M>(feature);}
	template<typename M> double			getFloat(const std::string &feature){return m_pCoaxlinkGrabber->getFloat<M>(feature);}
	template<typename M> std::string	getString(const std::string &feature){return m_pCoaxlinkGrabber->getString<M>(feature);}
	template<typename M> void			setInteger(const std::string &feature, int value){m_pCoaxlinkGrabber->setInteger<M>(feature, value);}
	template<typename M> void			setFloat(const std::string &feature, double value){m_pCoaxlinkGrabber->setFloat<M>(feature, value);}
	template<typename M> void			setString(const std::string &feature, const std::string &value){m_pCoaxlinkGrabber->setString<M>(feature, value);}
	template<typename M> void			execute(const std::string &command){m_pCoaxlinkGrabber->execute<M>(command);}
	template<typename M> void			enableEvent(){return m_pCoaxlinkGrabber->enableEvent<M>();}
	/* Basic functions                                             */
	/***************************************************************/
};


extern CGrabber	g_Grabber;
#endif