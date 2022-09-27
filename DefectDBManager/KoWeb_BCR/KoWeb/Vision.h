// Vision.h: interface for the CVision class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VISION_H__BCDEE2C5_BA6F_475A_A4ED_BD3C30038CDC__INCLUDED_)
#define AFX_VISION_H__BCDEE2C5_BA6F_475A_A4ED_BD3C30038CDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MIL_EXIST
#include "mil.h"
#endif
#include "LogData.h"	// Added by ClassView

typedef struct
{
#ifdef MIL_EXIST
	MIL_ID MilApplication;
	MIL_ID MilSystem;
	MIL_ID MilDigitizer;
	MIL_ID MilBuffer[MAX_BUFFER];
	MIL_ID MilImage;
	MIL_ID MilDisplay;
	MIL_ID MilClipImage;
#endif
	int		i;
}UserDataStruct;
#ifdef MIL_EXIST
long MFTYPE GrabStart(long HookType, MIL_ID EventId,void *vMilData);
long MFTYPE GrabEnd(long HookType, MIL_ID EventId,void *vMilData);
#endif
class CVision  
{
public:
//	CLogData *m_pLogProcess;
	void GetExposureTime(double* pNanoSec);
	void SetExposureTime(int nNanoSec);
	unsigned char* GetProcessBuf();
	void SingleGrab(int nBufferNum);
//	BOOL m_bBufIndex;
	void ChangeVerticalLine(int Line);
	void MakeImg();
	int m_nImgVCount;
	void InitOriImage();
	void SetTrigMode();
	void SetLiveMode();
	void HookStop();
	void HookStart();
	unsigned char* GetImgPtr(int nCam);
	unsigned char* GetOriImgPtr(int nCam,int nFrameNo);
	BOOL Open(HWND hWnd);
	CVision(CWnd* pwnd=NULL);
	virtual ~CVision();
#ifdef MIL_EXIST
	MIL_ID MilApplication;
	MIL_ID MilSystem;
	MIL_ID MilDigitizer;
	MIL_ID MilBuffer[MAX_BUFFER];
	MIL_ID MilImage;
	MIL_ID MilClipImage;
	MIL_ID MilDisplay;
	MIL_ID MilSaveBuf;
#endif
	long m_nBand;
	long m_nSizeX;
	long m_nSizeY;
	CWnd*	m_pParent;	
	void ImageClip(int nBufferNum);
	void SaveImg(CString strpath);
	int m_nCrossCount;
	int m_nFrameCount;
/////////////////////////////////////////////////////@pwj
	unsigned short* GetProcessBufShort();
	unsigned short* GetImgPtrShort(int nIndex);
////////////////////////////////////////////////////	

	int		m_nBufIndexGrabStt;
	int		m_nBufIndexGrabEnd;
	int		m_nBufIndexCopy;
//	int		m_nGrabCount;
};

#endif // !defined(AFX_VISION_H__BCDEE2C5_BA6F_475A_A4ED_BD3C30038CDC__INCLUDED_)
