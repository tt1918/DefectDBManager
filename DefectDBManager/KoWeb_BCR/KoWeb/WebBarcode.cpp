#include "Stdafx.h"

#include <memory.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include <dvec.h>
#include <math.h>

#include "KoWebDoc.h"
#include "KoWebView.h"
#include "BCR/CallClassWrapperCodeReader.h"

#include "opencv.hpp"
#include "imgproc/imgproc.hpp"
#include <list>

//BARCODE_VISION

#define BCR_CODE_LENGTH 17

enum eBCRPatRead
{
	eNone = -1,
	eFineRectOK,
	eReadDone,
	eForceReadDone,
};

CallClassWrapperCodeReader g_CodeReader;

extern CXManageSocket  l_Send_Server;

BYTE l_fmBCRBK[512 * 128];
BYTE l_fmBCR[512 * 2178];

int m_tmpNullDotPos;

int GetBCRData(LPBYTE fm, int left, int top, int w, int h, int pitch, int* pX, int* pY, TCHAR sBcr[][30]);

bool SearchBCR(LPVOID pParent);
double Distance2NearRect(CRect r1, CRect r2);
CRect GetBcrFineArea(LPBYTE fm, int left, int top, int w, int h, int pitch);
CPoint GetBcrCenter(unsigned char* fm, CRect rt, int nPitch);
CRect GetBarcodeRect2(LPBYTE fm, int w, int h, int pitch, int ndirect);
void GetBcrPosition(LPBYTE fm, int left, int top, int w, int h, int pitch);
bool CheckValidCode(CString str);
int CheckBcrOrder(CString NewBarcode, CString LastBarcode);
void SearchDefectData(LPVOID pParent, int crtFrameNum, int lastBcrFrameNum);
bool CheckDotOnPVA(LPBYTE fm, int w, int h, int pitch, int left, int right, int dir);

bool Read2DMatrix(cv::Mat roiImage);
std::list<cv::Rect> FindBarcodePosition(cv::Mat image);
cv::Rect SetBarcodeArea(cv::Rect rect, int width, int height);

void UpdateBcrImagePathToDefect(int frameNum);
void CvtBcrImage2SaveDefectImage(LPBYTE fm, int w, int h, CRect rect, LPBYTE out, int outW, int outH);

void WEB_Barcode(LPVOID pParent)
{
	int i, j, m, nOverlap;
	int nBcrPitch = 512;
	LPBYTE fm = g_fmGrab[g_ID];
	int pitch = g_System.m_nPitch;
	int width = g_System.m_nImageW;
	int height = g_System.m_nImageH;
	int nFrameNum = g_Temp.m_nGrabFrame;
	int nX, nBCRCount;
	int nBcrX[10], nBcrY[10];
	TCHAR sBCR[10][30];

	g_Temp.m_nMaxGray = 0;
	g_Temp.m_nMinGray = 255;
	g_Temp.m_BcrRect = CRect(0, 0, 0, 0);
	g_Temp.m_BcrRectFine = CRect(0, 0, 0, 0);
	g_Temp.m_BcrRectForMatch = CRect(0,0,0,0);
	g_Temp.m_BcrRectCodeRead = CRect(0, 0, 0, 0);

	if (g_Param.m_nNotInspArea == 0) return;

	if (g_Temp.m_nEdgeDir == 0) nX = g_Temp.m_nInspectX1;
	else                     nX = g_Temp.m_nInspectX2 - nBcrPitch;

	// 기존 방식 검사 
	bool bRes = SearchBCR(pParent);

	// 검사 실패한 경우 재검사
	if (bRes == false)
	{
#ifdef USE_CODEREADERDLL
		cv::Mat inImg = cv::Mat(height, width, CV_8UC1);
		memcpy(inImg.data, fm, sizeof(BYTE) * height * width);

		std::list<cv::Rect> codePosition = FindBarcodePosition(inImg);
		for (std::list<cv::Rect>::iterator iter = codePosition.begin(); iter != codePosition.end(); iter++)
		{
			bRes = Read2DMatrix(inImg(*iter));
			if (bRes == true)
			{
				g_Temp.m_BcrRectCodeRead = g_CodeReader.GetLastCodePosition();
				g_Temp.m_isBcrSuccessRead = true;

				break;
			}
		}

		inImg.release();
#endif
	}

	// 우선 방향을 못 확인했더라도 패턴 영역을 확인한 상태이면 이미지는 저장하도록함
	if (g_Param.m_bBcrSaveImage == true && (g_Temp.m_isBcrSuccessRead == true || g_Temp.m_nBcrPatFind == eForceReadDone))
	{
		CvtBcrImage2SaveDefectImage(fm, width, height, g_Temp.m_BcrRectFine, g_Defect.m_pImage[0], BAD_IMG_WIDTH, BAD_IMG_HEIGHT);
		UpdateBcrImagePathToDefect(nFrameNum);
	}

	//if (g_Temp.m_nBcrFirstRead==true
	//	|| (g_Temp.m_bBcrForceInsert ==true && g_Temp.m_isBcrForceReading==true)
	//	|| (g_Temp.m_nBcrDir != 0)) // 방향이 인식 되었으면 들어온다
	{
		SearchDefectData(pParent, g_Temp.m_nGrabFrame, g_Temp.m_nBcrPreInspFrame);
	}
	

	//for (i = 0; i < 128; i++)
	//	memcpy(l_fmBCR + nBcrPitch * i, l_fmBCRBK + nBcrPitch * i, nBcrPitch);
	//for (i = 0; i < g_System.m_nImageH; i++)
	//	memcpy(l_fmBCR + nBcrPitch * (i + 128), fm + pitch * i + nX, nBcrPitch);

	//nBCRCount = GetBCRData(l_fmBCR, 0, 0, nBcrPitch, 2178, nBcrPitch, nBcrX, nBcrY, sBCR);

	//for (i = 0; i < nBCRCount; i++)
	//{

	//}

	////가장 마지막 촬상한것 보관-----------------------------------
	//for (i = 0; i < 128; i++)
	//	memcpy(l_fmBCRBK + nBcrPitch * i, fm + pitch * i + nX, nBcrPitch);
	////------------------------------------------------------------
}

//최대 10개이나 현실적으로 3개만, 상, 하 붙은것은 제외.
//바코드 찾을때 필요한 파라미터 알려줘요
int GetBCRData(LPBYTE fm, int left, int top, int w, int h, int pitch, int* pX, int* pY, TCHAR sBcr[][30])
{
	int nBCRCount = 0;

	return nBCRCount;
}

bool SearchBCR(LPVOID pParent)
{
	int i, j;
	LPBYTE fm = g_fmGrab[g_ID];
	int pitch = g_System.m_nPitch;
	int width = g_System.m_nImageW;
	int height = g_System.m_nImageH;

	int nFrameNum = g_Temp.m_nGrabFrame;

	CString strLog;
	g_Temp.m_isBcrSuccessRead = false;
	g_Temp.m_nBcrPatFind = eNone;	// 패턴 검사로 찾은 경우 확인

	// 결과 데이터는 전부 g_Temp 데이터에 저장됨.
	GetBcrPosition(fm, 0, 0, width, height, pitch);
	// 기존 코드

	bool isBcrExsit = true;
	CString strBcrMsg = _T("none");
	CString strReadMsg = _T("");
	CString strMsg;


	if (g_Param.m_nBcrCsvType == eCSV_TYPE_NITTO_RTS || g_Param.m_nBcrCsvType == eCSV_TYPE_NITTO_RK)
	{
		if (g_Param.m_nBcrDotTh > 0 && g_Param.m_nBcrDotUpTh > 0)
		{
			if (g_Temp.m_BcrRectForMatch.Width() == 0 || g_Temp.m_BcrRectForMatch.Height() == 0)
				isBcrExsit = false;
		}
	}

	if (g_Param.m_nNotInspArea != width)
	{
		if (g_Temp.m_BcrRect.left < 0)		g_Temp.m_BcrRect.left = 0;
		if (g_Temp.m_BcrRect.right < 0)		g_Temp.m_BcrRect.right = 0;
		if (g_Temp.m_BcrRect.top < 0)		g_Temp.m_BcrRect.top = 0;
		if (g_Temp.m_BcrRect.bottom < 0)	g_Temp.m_BcrRect.bottom = 0;
		if (g_Temp.m_BcrRect.left > width)	g_Temp.m_BcrRect.left = width;
		if (g_Temp.m_BcrRect.right > width)	g_Temp.m_BcrRect.right = width;
		if (g_Temp.m_BcrRect.bottom > height) g_Temp.m_BcrRect.bottom = height;

		CRect tmpRect = g_Temp.m_BcrRect;

		if (isBcrExsit == true)
		{
			// BCR 검출 
#ifdef USE_CODEREADERDLL
			BYTE* pBcrImg = new BYTE[tmpRect.Width() * tmpRect.Height()];
			for (i = tmpRect.top, j = 0; i < tmpRect.bottom; i++, j++)
				memcpy(pBcrImg + j * tmpRect.Width(), fm + i * width + tmpRect.left, sizeof(BYTE) * tmpRect.Width());
			std::string strRead = g_CodeReader.CodeRead(pBcrImg, tmpRect.Width(), tmpRect.Height(), false);
			strReadMsg = strRead.c_str();
			if (strReadMsg == _T("") || CheckValidCode(strReadMsg) == false)
				strRead = g_CodeReader.CodeRead(pBcrImg, tmpRect.Width(), tmpRect.Height(), true);
			strReadMsg = strRead.c_str();
			if (strReadMsg == _T("") || CheckValidCode(strReadMsg) == false)
			{
				if (g_Temp.m_BcrRect.top > height)	g_Temp.m_BcrRect.top = height;
				strReadMsg = _T("no_barcode");
				g_Temp.m_isBcrSuccessRead = false;
			}
			else
			{
				g_Temp.m_BcrRectCodeRead = g_CodeReader.GetLastCodePosition();
				// 이미지 상의 실제 좌표 위치로 이동
				g_Temp.m_BcrRectCodeRead.OffsetRect(CPoint(tmpRect.left, tmpRect.top));
				g_Temp.m_isBcrSuccessRead = true;
	}

			delete[] pBcrImg;
#else
			strReadMsg = _T("no_barcode");
#endif
}
		else
		{
			g_Temp.m_isBcrSuccessRead = false;
		}

		if (g_Temp.m_isBcrSuccessRead == true)
		{
			if (g_Temp.m_isBcrFirstCode == false)
				g_Temp.m_isBcrFirstCode = true;

			if (g_Temp.m_bBcrForceInsert == true && g_Temp.m_isBcrSuccessRead == true)
			{
				CString strForcedBcr = g_Temp.m_strBcrForceData;
				CString strTemp = strReadMsg;

				if (strTemp.CompareNoCase(_T("")) == 0 || strTemp.CompareNoCase(_T("no_barcode")) == 0)
				{
					g_Temp.m_isBcrSuccessRead = false;
					g_Temp.m_isBcrFirstCode = false;
				}
				else
				{
					g_Temp.m_isBcrForceReading = true;
					g_Temp.m_dBCRForceREadingDist = 0.0;

					if (strForcedBcr.CompareNoCase(strTemp) != 0)
					{
						g_Temp.m_strBcrForceData = strTemp;

						// 강제 알람
						l_Send_Server.SendCommand_LocalHost(NM_FORCE_BCR_NOT_MATCHED_ALRAM);
						strLog.Format(_T("[FORCE_BCR] Matching Error BCR and OCR :%s,%s"), strForcedBcr, strTemp);
						WriteLog(strLog);
					}
				}
			}

			if (g_Temp.m_isBcrSuccessRead == true)
			{
				strBcrMsg = _T("Read");
				g_Temp.m_nBcrPatFind = eReadDone; // Reader로 읽어들인 경우
			}
		}

		if (strReadMsg == _T(""))
			strReadMsg = _T("no_barcode");

		int nMatchFrame = 0;
		const int nPreFrm = g_Temp.m_nBcrPreInspFrame;

		g_Temp.m_strBcrName = strReadMsg;

		// 강제 입력 시 방향을 설정해준다. 
		if (g_Temp.m_bBcrForceInsert == true)
		{
			// 증가
			if (g_Temp.m_bBcrForceDir == true)	g_Temp.m_nBcrDir = 1;
			else								g_Temp.m_nBcrDir = -1;
		}

		int nRet = 0;
		if (g_Temp.m_isBcrSuccessRead == false && g_Temp.m_nBcrDir != 0 && isBcrExsit == true &&
			(g_Param.m_useBcrMatSize == true && g_Temp.m_BcrRectFine.Width() > 0))
		{
			// 실제 BCR Length 처리 추가해야함.
			const double dBarcode_period_frame = (double)(1000, 0 / (g_Param.m_dBcrScaleFactorY * height));
			/*if (m_bUseTestMode && !m_bSim_Mode || m_bUseTestMode && m_bSim_Mode)
			{

				nMatchFrame = m_nFnPeriod;
			}
			else*/
			{
				if ((nFrameNum - nPreFrm >= (int)(dBarcode_period_frame * 1 - 1) && nFrameNum - nPreFrm <= (int)(dBarcode_period_frame * 1 + 1)))
					nMatchFrame = 1;
				else if ((nFrameNum - nPreFrm >= (int)(dBarcode_period_frame * 2 - 1) && nFrameNum - nPreFrm <= (int)(dBarcode_period_frame * 2 + 1)))
					nMatchFrame = 2;
				else if ((nFrameNum - nPreFrm >= (int)(dBarcode_period_frame * 3 - 1) && nFrameNum - nPreFrm <= (int)(dBarcode_period_frame * 3 + 1)))
					nMatchFrame = 3;
				else if ((nFrameNum - nPreFrm >= (int)(dBarcode_period_frame * 4 - 1) && nFrameNum - nPreFrm <= (int)(dBarcode_period_frame * 4 + 1)))
					nMatchFrame = 4;
				else if ((nFrameNum - nPreFrm >= (int)(dBarcode_period_frame * 5 - 1) && nFrameNum - nPreFrm <= (int)(dBarcode_period_frame * 5 + 1)))
					nMatchFrame = 5;
			}

			strBcrMsg.Format(_T("Matched"));
			nRet = 1;

			if (nRet > 0)
			{
				const int nLastBcrFrame = g_Temp.m_nBcrPreInspFrame;
				const CString strLastBCNO = g_Temp.m_strPreBcrName;
				int ntmp = 0;

				if (g_Temp.m_bBcrForceInsert == true)
				{
					strMsg = g_Temp.m_strBcrForceData.Left(11);
					ntmp = _ttoi(g_Temp.m_strBcrForceData.Right(6));
				}
				else
				{
					strMsg = g_Temp.m_strPreBcrName.Left(11);
					ntmp = _ttoi(g_Temp.m_strPreBcrName.Right(6));
				}

				CString strNewBCNO;
				if (g_Temp.m_bBcrForceInsert == true && g_Temp.m_isBcrForceReading == false)
				{
					if (g_Temp.m_nBcrDir == -1)  // 감소
					{
						int nBcd = int((nFrameNum - nLastBcrFrame) / dBarcode_period_frame);
						strNewBCNO.Format(_T("%s%06d"), strMsg, ntmp - nBcd);

						strLog.Format(_T("[FORCE_BCR]강제 Calulated nBcd after forced BCR inserting : %d / BCNO : %s / gFrame : %d , LastFrame : %d fBpF:%.6f / "),
							nBcd, strNewBCNO, nFrameNum, g_Temp.m_nBcrPreInspFrame, dBarcode_period_frame);
						WriteLog(strLog);
					}
					else // 증가
					{
						int nBcd = int((nFrameNum - nLastBcrFrame) / dBarcode_period_frame);
						strNewBCNO.Format(_T("%s%06d"), strMsg, ntmp + nBcd);

						strLog.Format(_T("[FORCE_BCR]Calulated nBcd after forced BCR inserting : %d / BCNO : %s / gFrame : %d , LastFrame : %d fBpF:%.6f / "),
							nBcd, strNewBCNO, nFrameNum, g_Temp.m_nBcrPreInspFrame, dBarcode_period_frame);
						WriteLog(strLog);
					}
				}
				else
				{
					if (g_Temp.m_nBcrDir == -1)  // 감소
						strNewBCNO.Format(_T("%s%06d"), strMsg, ntmp - nMatchFrame);
					else // 증가
						strNewBCNO.Format(_T("%s%06d"), strMsg, ntmp + nMatchFrame);
				}

				if (g_Temp.m_bBcrForceInsert == true)
				{
					if (strMsg.CompareNoCase(L"") == 0 ||
						strMsg.CompareNoCase(L"no_barcode") == 0)
					{
						g_Temp.m_isBcrForceReading = false;
						strNewBCNO = "";
						CString str;
						str.Format(L"[FORCE_BCR] Error is occured after pattern maching, because there is no measure or no_barcode signal :%s", strNewBCNO);
						WriteLog(str);
					}
					else
					{
						g_Temp.m_isBcrForceReading = true;
						g_Temp.m_dBCRForceREadingDist = 0.f;
						g_Temp.m_strBcrForceData = strNewBCNO;
						CString str;
						str.Format(L"[FORCE_BCR]BCNO is setted by force command after pattern matching:%s", strNewBCNO);
						WriteLog(str);
					}
				}

				if (strNewBCNO.GetLength() > 0)
				{
					g_Temp.m_strBcrName = strNewBCNO;
					g_Temp.m_isBcrSuccessRead = true;
					g_Temp.m_nBcrPatFind = eForceReadDone; // 강제 바코드 입력으로 데이터 입력
				}
				else
				{
					CString str;
					str.Format(L"This message should not be logged %d,%d", nFrameNum, g_Temp.m_nBcrPreInspFrame);
					WriteLog(str);
				}
			}
		}

#ifdef BCR_SIMUL_TEST
		if (g_Temp.m_strPreBcrName.GetLength() == 0)
		{
			g_Temp.m_strPreBcrName = g_Temp.m_strBcrName = _T("E850401-03_001400"); // 테스트 코드
		}
		if ((nFrameNum - g_Temp.m_nBcrPreInspFrame > 9) && g_Temp.m_isBcrSuccessRead == FALSE)
		{
			int nStrSize = g_Temp.m_strPreBcrName.GetLength();
			int nSpacePos = g_Temp.m_strPreBcrName.ReverseFind('_');

			CString strmsg = g_Temp.m_strPreBcrName.Left(nSpacePos + 1);
			int ntmp = _ttoi(g_Temp.m_strPreBcrName.Right(nStrSize - nSpacePos - 1));
			g_Temp.m_strBcrName.Format(_T("%s%06d"), strmsg, ntmp + 1);

			nStrSize = g_Temp.m_strBcrName.GetLength();
			nSpacePos = g_Temp.m_strBcrName.ReverseFind('_');

			/*pDoc->m_str_lot_Full = g_Temp.m_strBcrName;
			pDoc->m_str_lot_End = pDoc->m_str_lot_Full.Right(nStrSize - nSpacePos - 1);*/
			g_Temp.m_slotName = g_Temp.m_strBcrName.Left(nSpacePos);
			g_Temp.m_isBcrSuccessRead = TRUE;
			g_Defect.m_nBcrCount = 1;
			g_Defect.m_nBcrDefectCount = 0;
			g_Defect.m_nBcrAreaDefectCount = 0;
			g_Temp.m_nBcrPatFind = eReadDone;
			g_Temp.m_nBcrDir = 1;	//증가
			g_Temp.m_isBcrFirstCode = 1;
			strBcrMsg = _T("Read"); 
			g_Temp.m_isBcrFirstCheck = false;

			g_Temp.m_BcrRectFine = CRect(200, g_Temp.m_nFoundEdge, 800, g_Temp.m_nFoundEdge + 200);
			g_Temp.m_BcrRect = CRect(200, g_Temp.m_nFoundEdge, 800, g_Temp.m_nFoundEdge + 200);
		}
		else if (g_Temp.m_nBcrPatFind == eForceReadDone)
		{

		}
		else
		{
			g_Temp.m_isBcrSuccessRead = false;
			g_Defect.m_nBcrCount = 0;
			g_Defect.m_nBcrDefectCount = 0;
			g_Defect.m_nBcrAreaDefectCount = 0;
			g_Temp.m_nBcrPatFind = eNone;
		}
#endif	

		if (g_Temp.m_isBcrSuccessRead == true)
		{
			// 이위치 아닌거 같음.
			/*g_Temp.m_nBcrPreInspFrame = nFrameNum;
			g_Temp.m_strPreBcrName = g_Temp.m_strBcrName;*/

			g_Temp.m_nBcrReadOK++;
		}

		if (g_Param.m_nBcrCsvType == eCSV_TYPE_NITTO || g_Param.m_nBcrCsvType == eCSV_TYPE_KORENO || g_Param.m_nBcrCsvType == eCSV_TYPE_KORENO_RK)
		{
			// 10미터 마다 불량 데이터 전송?????
			// 구현 확인 해야함.
		}

		if (g_Param.m_nBcrCsvType == eCSV_TYPE_NITTO_RK || g_Param.m_nBcrCsvType == eCSV_TYPE_NITTO_RTS ||
			g_Param.m_nBcrCsvType == eCSV_TYPE_KORENO_RK || g_Param.m_nBcrCsvType == eCSV_TYPE_KORENO_RK_IJP)
		{
			if (g_Temp.m_isBcrSuccessRead == true)
			{
				g_Temp.m_dBcrNoReadWarning = 0;
				g_Temp.m_dBcrNoReadError = 0;

			}
			else
			{
				double calcFrameLeng = 0;

				if (g_Param.m_dBcrScaleFactorY > BCR_SCALE_MAX)
					g_Param.m_dBcrScaleFactorY = BCR_SCALE_REF;

				// 미터로 계산
				calcFrameLeng = (g_Param.m_dBcrScaleFactorY * height) / 1000.0;
				g_Temp.m_dBcrNoReadWarning += calcFrameLeng;
				g_Temp.m_dBcrNoReadError += calcFrameLeng;

				if (g_Temp.m_isBcrFirstCode == true)
				{
					strLog.Format(_T("ERROR Meter : %.3fM, Warning Meter : %.3fM"),
						g_Temp.m_dBcrNoReadWarning, g_Temp.m_dBcrNoReadError);
					WriteLog(strLog);
				}

				if (g_Temp.m_isBcrFirstCode == true && g_Temp.m_strBcrName.GetLength() > 9)
				{
					if ((int)g_Temp.m_dBcrNoReadError > g_Param.m_nBCRErrorM)
					{
						g_Temp.m_dBcrNoReadError = 0;
						l_Send_Server.SendCommand_LocalHost(NM_BCR_BCD_READING_ERROR);
						strLog.Format(_T("[Error] No Bcr Read Length: %.3fM [Error Std Meter:%d]"), g_Temp.m_dBcrNoReadError, g_Param.m_nBCRErrorM);
						WriteLog(strLog);
					}
					else if ((int)g_Temp.m_dBcrNoReadWarning > g_Param.m_nBCRWarningM)
					{
						g_Temp.m_dBcrNoReadWarning = 0;
						l_Send_Server.SendCommand_LocalHost(NM_BCR_BCD_READING_LOW);
						strLog.Format(_T("[Warning] No Bcr Read Length: %.3fM [Warning Std Meter:%d]"), g_Temp.m_dBcrNoReadError, g_Param.m_nBCRWarningM);
						WriteLog(strLog);
					}
				}

				// BCR 인식률 전송
				if (g_Temp.m_nInspectFrame > 0 && g_Temp.m_nInspectFrame % 100 == 0)
				{
					double dReadingRate;

					dReadingRate = (int)((double)g_Temp.m_nBcrReadOK * 100.0 / (g_Temp.m_nInspectFrame * g_Param.m_dFrameLen * 0.001));

					CPacket* packet = new CPacket;
					packet->MakeBcrReadingRatePacket(dReadingRate);
					l_Send_Server.SendInsData(packet);
					delete packet;
					strLog.Format(_T("[INFO] Barcode Reading Rate: %d"), (int)dReadingRate);
					WriteLog(strLog);
				}
			}
		}
	}

	if (strBcrMsg != L"none")
	{
		if (g_Temp.m_isBcrSuccessRead == true)
		{
			g_Temp.m_nBcrOrder = CheckBcrOrder(g_Temp.m_strBcrName, g_Temp.m_strPreBcrName);
			if (g_Temp.m_nBcrOrder > 0)
			{
				for (i = 0; i < g_Temp.m_nBcrOrder; i++)
					WriteBarcodeInfo(g_Temp.m_strPreBcrName, _T("none"), 10, 0);
				g_Temp.m_nBcrOrder = 0;
			}
			WriteBarcodeInfo(g_Temp.m_strPreBcrName, strBcrMsg, g_Temp.m_nBcrPatFind, g_Temp.m_nInspectFrame);
		}
	}

	if (nFrameNum > 0 && nFrameNum % 100 == 0)
		WriteBcrEdgeLog((float)(g_Temp.m_nFoundEdge * g_Param.m_dScaleFactorX + g_Param.m_dCamStartPosX));

	return g_Temp.m_isBcrSuccessRead;
}


#define DISTANCE(x1,y1,x2,y2) sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1))
double Distance2NearRect(CRect r1, CRect r2)
{
	double distance[16];
	distance[0] = DISTANCE((double)r1.left, (double)r1.top, (double)r2.left, (double)r2.top);
	distance[1] = DISTANCE((double)r1.left, (double)r1.top, (double)r2.right, (double)r2.top);
	distance[2] = DISTANCE((double)r1.left, (double)r1.top, (double)r2.right, (double)r2.bottom);
	distance[3] = DISTANCE((double)r1.left, (double)r1.top, (double)r2.left, (double)r2.bottom);

	distance[4] = DISTANCE((double)r1.right, (double)r1.top, (double)r2.left, (double)r2.top);
	distance[5] = DISTANCE((double)r1.right, (double)r1.top, (double)r2.right, (double)r2.top);
	distance[6] = DISTANCE((double)r1.right, (double)r1.top, (double)r2.right, (double)r2.bottom);
	distance[7] = DISTANCE((double)r1.right, (double)r1.top, (double)r2.left, (double)r2.bottom);

	distance[8] = DISTANCE((double)r1.right, (double)r1.bottom, (double)r2.left, (double)r2.top);
	distance[9] = DISTANCE((double)r1.right, (double)r1.bottom, (double)r2.right, (double)r2.top);
	distance[10] = DISTANCE((double)r1.right, (double)r1.bottom, (double)r2.right, (double)r2.bottom);
	distance[11] = DISTANCE((double)r1.right, (double)r1.bottom, (double)r2.left, (double)r2.bottom);

	distance[12] = DISTANCE((double)r1.left, (double)r1.bottom, (double)r2.left, (double)r2.top);
	distance[13] = DISTANCE((double)r1.left, (double)r1.bottom, (double)r2.right, (double)r2.top);
	distance[14] = DISTANCE((double)r1.left, (double)r1.bottom, (double)r2.right, (double)r2.bottom);
	distance[15] = DISTANCE((double)r1.left, (double)r1.bottom, (double)r2.left, (double)r2.bottom);

	double dMin = 100000;
	int nIndex;
	for (int i = 0; i < 16; i++)
	{
		if (distance[i] < dMin)
		{
			dMin = distance[i];
			nIndex = i;
		}
	}

	CPoint ptLT = CPoint(r2.left, r2.top);
	CPoint ptRT = CPoint(r2.right, r2.top);
	CPoint ptRB = CPoint(r2.right, r2.bottom);
	CPoint ptLB = CPoint(r2.left, r2.bottom);
	if (r1.PtInRect(ptLT) || r1.PtInRect(ptRT) || r1.PtInRect(ptRB) || r1.PtInRect(ptLB))
		dMin = 0;
	return dMin;
}

/// <summary>
/// Fine Area 탐색
/// 상하 기준 탐색 및 좌우는 기존 BCR 영역 기준 처리
/// </summary>
int l_proj[2048];
CRect GetBcrFineArea(LPBYTE fm, int left, int top, int w, int h, int pitch)
{
	int i, j, nDiff, nDiffMax, nMaxPos = 0;
	const int nOffset = 140;
	CRect  rect = g_Temp.m_BcrRect;
	CRect tmpRect;

	memset(l_proj, 0, 2048 * sizeof(int));
	for (i = rect.top; i < rect.bottom - 1; i++)
	{
		for (j = rect.left; j < rect.right; j++)
		{
			nDiff = *(fm + pitch * i + j) - *(fm + pitch * (i + 1) + j);
			if (nDiff < 0) nDiff *= -1;
			l_proj[i] += nDiff;
		}
	}
	nDiff = 0;
	nMaxPos = rect.top + nOffset / 2;
	for (i = rect.top; i < rect.top + nOffset; i++)
		nDiff += l_proj[i];

	nDiffMax = 0;
	for (i = rect.top + 1; i < rect.bottom - nOffset; i++)
	{
		nDiff = nDiff - l_proj[i - 1] + l_proj[i + nOffset];
		if (nDiff > nDiffMax)
		{
			nDiffMax = nDiff;
			nMaxPos = i + nOffset / 2;
		}
	}

	tmpRect.top = nMaxPos - nOffset / 2;
	tmpRect.bottom = nMaxPos + nOffset / 2;
	tmpRect.left = g_Temp.m_BcrRect.left;
	tmpRect.right = g_Temp.m_BcrRect.right;
	tmpRect.InflateRect(0, 0, 0, 0);

	return tmpRect;
}


CPoint GetBcrCenter(unsigned char* fm, CRect rt, int nPitch)
{
	int nWidth = rt.right - rt.left;
	int nHeight = rt.bottom - rt.top;
	CPoint Bar_Center = (0, 0);
	CPoint	Barcode_Center = (0, 0);

	// 1st ----------------------------------------------- 
	int i, j;
	int nSkipY = 1;
	int nSkipX = 1;
	int temp_low = 255;
	int value = 0;
	long sum = 0;
	float avg = 0;
	long temp_sum = 9200000;

	//for(j=rt.top; j<rt.bottom; j+=nSkipY)
	for (j = rt.top; j < rt.bottom; j++)	//코텍과 동일하게 바꿈
	{
		for (i = rt.left; i < rt.right; i += nSkipX)
		{
			sum = sum + fm[j * nPitch + i];

			if (temp_low > (fm[j * nPitch + i]))
			{
				Bar_Center = CPoint(i, j);
				value = fm[j * nPitch + i];
				temp_low = value;
			}
		}
	}
	avg = sum / (float)(nWidth * nHeight);

	if ((avg - temp_low) < 10)
		Bar_Center = CPoint(0, 0);

	return Bar_Center;
}

CRect GetBarcodeRect2(LPBYTE fm, int w, int h, int pitch, int ndirect)
{
	int i, j;
	CRect	rect;
	int		ntmp2 = g_Param.m_nBCRSearchPixel;
	const int ntmp1 = 30;

	int ntmp = 0;
	int ntop = 0, nbottom = 0, nleft = 0, nright = 0, nth = 0, nping = 0;
	nping = 0;
	//기존에는 누적 Gray값으로 비교했음. BCD Search ROI 크기가 변경되면, TH가 실질적으로 변경되는 문제가 있었음.
	//보통 BCD 높이가 140pixel정도 되고 폭을 220Pixel정도로 설정, 보통 중간값으로 평균 TH를 구함
	double dTh = g_Param.m_nBcrDotTh;
	rect.SetRect(0, 0, w, h);

	if (ndirect == 1)
	{	//102//202
		rect.left = g_Temp.m_BcrRect.left + ntmp1;
		rect.right = g_Temp.m_BcrRect.right - ntmp1;
		if (rect.right > 4095)	rect.right = 4095;
		if (rect.right < 0)		rect.right = 0;
		if (rect.left < 0)			rect.left = 0;
	}
	else
	{
		rect.right = g_Temp.m_BcrRect.right - ntmp1;
		rect.left = g_Temp.m_BcrRect.left + ntmp1;
		if (rect.left < 1)	rect.left = 0;
		if (rect.right < 0)	rect.right = 0;
		if (rect.left < 0)		rect.left = 0;
	}

	if (rect.Width() <= 0 || rect.Height() <= 0)
		return rect;


	//--------------------------------------------------------------------
	// 위아래 BCD 영역을 찾음
	int k = 0;
	int* pHisto = new int[rect.Height()];
	memset(pHisto, 0, sizeof(int) * rect.Height());

	int nCount = 0;
	for (j = rect.top; j < rect.bottom; j++)
	{
		for (i = rect.left; i < rect.right; i++)
			pHisto[j - rect.top] += fm[j * pitch + i];
	}

	int nHistoMin = INT_MAX;
	int nHistoMax = 0;
	int nPosMin = 0;
	int nPosMax = 0;
	int nHisto, nPos;
	//가장 낮은 위치를 찾는다.
	if (g_Param.m_bBcrObjW == false)
	{
		for (j = rect.top + 5; j < rect.bottom - 5; j++)
		{
			if (nHistoMin > pHisto[j])
			{
				nHistoMin = pHisto[j];
				nPosMin = j;
			}
		}
		nHisto = nHistoMin;
		nPos = nPosMin;
	}
	else
	{
		for (j = rect.top + 5; j < rect.bottom - 5; j++)
		{
			if (nHistoMax < pHisto[j])
			{
				nHistoMax = pHisto[j];
				nPosMax = j;
			}
		}
		nHisto = nHistoMax;
		nPos = nPosMax;
	}

	int ntopTmp = nPos - 150;
	if (ntopTmp < 0) ntopTmp = 0;
	int nbottomTmp = nPos + 150;
	if (nbottomTmp > rect.Height() - 1) nbottomTmp = rect.Height() - 1;

	int nSearchTop = nPos - 20;
	if (nSearchTop < 0) nSearchTop = 0;
	int nSearchbottom = nPos + 20;
	if (nSearchbottom > rect.Height() - 1) nSearchbottom = rect.Height() - 1;

	nCount = rect.right - rect.left;
	int nOffset = 10;
	for (j = ntopTmp + nOffset; j < nSearchbottom; j++)
	{
		if ((double)abs(pHisto[j] - pHisto[j - nOffset]) / nCount > dTh)
		{
			ntop = j - nOffset / 2;
			break;
		}
	}
	for (j = nbottomTmp - 1; j > nSearchTop + nOffset; j--)
	{
		if ((double)abs(pHisto[j] - pHisto[j - nOffset]) / nCount > dTh)
		{
			nbottom = j + nOffset / 2;
			break;
		}
	}
	//--------------------------------------------------------------------
	if (nbottom - ntop < 50)
	{
		nbottom = 0;
		ntop = 0;
	}

	// 좌우 BCD 영역을 찾음

	if (nbottom < 0 || ntop < 0 || nbottom >2048 || ntop >2048)
	{
	}
	else
	{
		//BCD를 포함하지 않는 주변영역의 Projection 을 구함
		int* pOuterHistoY = new int[rect.Width()];
		memset(pOuterHistoY, 0, sizeof(int) * rect.Width());
		const int nProjRange = 50;
		const int nProjSkip = 10;
		const int nProjY = nProjRange + nProjSkip;

		int nOuterCount = 0;
		if (ntop >= nProjY)
		{
			for (j = 0; j < ntop - nProjSkip; j += 2)
			{
				for (i = rect.left; i < rect.right; i++)
					pOuterHistoY[i - rect.left] += fm[j * pitch + i];
			}
			nOuterCount += (ntop - nProjSkip) / 2;
		}
		if (nbottom + nProjY < rect.bottom)
		{
			for (j = nbottom + nProjSkip; j < rect.Height(); j += 2)
			{
				for (i = rect.left; i < rect.right; i++)
					pOuterHistoY[i - rect.left] += fm[j * pitch + i];
			}
			nOuterCount += (rect.Height() - nbottom + nProjSkip) / 2;
		}

		//BCD를 포함하지 않는 영역과 BCD영역과의 밝기차이로 좌우 BCD영역을 찾음
		int* pHistoY = new int[rect.Width()];
		memset(pHistoY, 0, sizeof(int) * (rect.Width()));
		nCount = nbottom - ntop;

		double* pDiffY = new double[rect.Width()];
		memset(pDiffY, 0, sizeof(double) * (rect.Width()));

		for (j = ntop; j < nbottom; j++)
		{
			for (i = rect.left; i < rect.right; i++)
				pHistoY[i - rect.left] += fm[j * pitch + i];
		}

		double dMax = 0;
		int nMaxDiffIndex = 0;
		for (j = rect.left; j < rect.right; j++)
		{
			pDiffY[j - rect.left] = abs((double)pOuterHistoY[j - rect.left] / nOuterCount - (double)pHistoY[j - rect.left] / nCount);
			if (dMax < pDiffY[j - rect.left])
			{
				dMax = pDiffY[j - rect.left];
				nMaxDiffIndex = j;
			}
		}

		const int kBcdWidth = 75;
		if (nMaxDiffIndex > 0)
		{
			bool bFound = false;
			int nAvgRange = 5;
			for (j = nMaxDiffIndex; j < rect.right; j++)
			{
				double dMovingAvg = 0;
				if (j + nAvgRange > rect.right)
					nAvgRange = rect.right - j;

				for (int k = 0; k < nAvgRange; k++)
					dMovingAvg += pDiffY[j - rect.left + k];
				dMovingAvg /= nAvgRange;

				if (dMovingAvg < dTh / 2)
				{
					bFound = true;
					nright = j;
					nleft = j - 75;
					break;
				}
			}

			if (!bFound)
			{
				nright = nMaxDiffIndex + kBcdWidth / 2;
				nleft = nMaxDiffIndex - kBcdWidth / 2;
			}
		}
		else
		{
			nright = nMaxDiffIndex + kBcdWidth / 2;
			nleft = nMaxDiffIndex - kBcdWidth / 2;
		}

		delete[] pDiffY;
		delete[] pHistoY;
		delete[] pOuterHistoY;
	}

	//부적절한 Rect 사이즈 처리
	nping = 0;
	if (nleft < 0)		nleft = rect.left;
	if (nright < 0)		nright = rect.right;
	if (ntop < 0)		ntop = rect.top;
	if (nbottom < 0)	nbottom = rect.bottom;
	rect.SetRect(nleft, ntop, nright, nbottom);

	if (!rect.Width() || !rect.Height())
		rect = CRect(0, 0, 0, 0);

	delete[] pHisto;

	return rect;
}


// 2D 바코드 영역 획득
// Nitto 기준으로 
void GetBcrPosition(LPBYTE fm, int left, int top, int w, int h, int pitch)
{
	CRect rtBcrArea = CRect(0, 0, 0, 0);
	CRect rtBcrAreaCheck = CRect(0, 0, 0, 0); //테스트 검증용 

	int edgeX = 0;
	//-----------------------------------------------------------------------------------------
	//필름이 나오는 부위에 따라 기준선을 찾는다. 오른쪽에 나오는 경우 오른쪽부터 기준선을 찾음
	edgeX = g_Temp.m_nFoundEdge;
	if (g_Temp.m_nEdgeDir == 0)	//PVA가 영상에서 오른쪽에 있을 경우 
	{
		if (edgeX > w - 1 || edgeX < 20)	edgeX = g_Temp.m_nBcrPreEdge;
		else								g_Temp.m_nBcrPreEdge = edgeX;
	}
	else
	{
		if (edgeX > w - 1 || edgeX < 20)	edgeX = g_Temp.m_nBcrPreEdge;
		else								g_Temp.m_nBcrPreEdge = edgeX;
	}
	g_Temp.m_nFoundEdge = edgeX;
	//-----------------------------------------------------------------------------------------

	///////////////////////////////////////////////////////////////////////////////////////////////
	// 에지 기준으로 검출 영역 생성 - S
	if (g_Temp.m_nEdgeDir == 0)  // 에지 기준 오른쪽에 바코드가 있는 경우
	{
		rtBcrArea = CRect(edgeX + 2, 0, edgeX + g_Param.m_nBCRSearchPixel, h);
		rtBcrAreaCheck = CRect(edgeX - g_Param.m_nBCRSearchPixel, 0, edgeX - 2, h);
		g_Temp.m_BcrRect = CRect(edgeX + 2, 0, edgeX + g_Param.m_nBCRSearchPixel, h);
	}
	else	// 에지 기준 왼쪽에 바코드가 있는 경우
	{
		rtBcrArea = CRect(edgeX - g_Param.m_nBCRSearchPixel, 0, edgeX - 2, h);
		rtBcrAreaCheck = CRect(edgeX + 2, 0, edgeX + g_Param.m_nBCRSearchPixel, h);
		g_Temp.m_BcrRect = CRect(edgeX - g_Param.m_nBCRSearchPixel, 0, edgeX - 2, h);
	}

	if (g_Param.m_useBCRManualArea == true)
	{
		rtBcrArea.left = rtBcrAreaCheck.left = g_Temp.m_BcrRect.left = g_Param.m_nBCRAreaL;
		rtBcrArea.right = rtBcrAreaCheck.right = g_Temp.m_BcrRect.right = g_Param.m_nBCRAreaR;
	}
	// 에지 기준으로 검출 영역 생성 - S
	///////////////////////////////////////////////////////////////////////////////////////////////
	g_Temp.m_nTempNullPos = -1;
	if (CheckDotOnPVA(fm, w, h, w, rtBcrArea.left, rtBcrArea.right, g_Temp.m_nEdgeDir) == true)
	{
		// 검사 완료 후 널링부 피해서 에지 영역 이동
		edgeX = g_Temp.m_nTempNullPos = m_tmpNullDotPos;

		if (g_Temp.m_nEdgeDir == 0)
		{
			g_Temp.m_BcrRect.left = m_tmpNullDotPos;
			g_Temp.m_BcrRect.right = g_Temp.m_BcrRect.left + g_Param.m_nBCRSearchPixel;
			if (g_Temp.m_BcrRect.right >= w)
				g_Temp.m_BcrRect.right = w - 1;
		}
		else
		{
			g_Temp.m_BcrRect.right = m_tmpNullDotPos;
			g_Temp.m_BcrRect.left = g_Temp.m_BcrRect.right - g_Param.m_nBCRSearchPixel;
			if (g_Temp.m_BcrRect.left < 0)
				g_Temp.m_BcrRect.left = 0;
		}
	}
	else
	{
		g_Temp.m_nTempNullPos = g_Temp.m_nFoundEdge;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////
	//바코드 중심점 및 영역 만듬 - S   
	g_Temp.m_BcrRectFine = GetBcrFineArea(fm, left, top, w, h, pitch);
	CPoint BarCenter = GetBcrCenter(fm, rtBcrArea, w);

	BarCenter.x = (g_Temp.m_BcrRectFine.left + g_Temp.m_BcrRectFine.right) / 2;
	BarCenter.y = (g_Temp.m_BcrRectFine.top + g_Temp.m_BcrRectFine.bottom) / 2;
	g_Temp.m_BcrCenterPt = BarCenter;

	int bcrHalfW = (int)((g_Param.m_dBcrW / g_Param.m_dScaleFactorX) / 2.0);
	int bcrHalfH = (int)((g_Param.m_dBcrH / g_Param.m_dScaleFactorY) / 2.0);

	if (g_Temp.m_nEdgeDir == 0)		// 에지 오른쪽에 BCR 존재
		g_Temp.m_BcrRect.SetRect(edgeX + g_Param.m_nBcrOffset, g_Temp.m_BcrRectFine.top - bcrHalfH, BarCenter.x + (bcrHalfW + g_Param.m_nBcrOffset), g_Temp.m_BcrRectFine.bottom + bcrHalfH);
	else							// 에지 왼쪽에 BCR 존재	
		g_Temp.m_BcrRect.SetRect(BarCenter.x - (g_Param.m_nBcrOffset + bcrHalfW), g_Temp.m_BcrRectFine.top - bcrHalfH, edgeX - g_Param.m_nBcrOffset, g_Temp.m_BcrRectFine.bottom + bcrHalfH);
	//바코드 중심점 및 영역 만듬 - E  
	///////////////////////////////////////////////////////////////////////////////////////////////

	if (g_Param.m_useBcrMatSize == true)
	{
		int i, j;

		CRect rectBcd = g_Temp.m_BcrRect;
		if (rectBcd.left < 0)	rectBcd.left = 0;
		if (rectBcd.top < 0)	rectBcd.top = 0;
		if (rectBcd.right < 0)	rectBcd.right = 0;
		if (rectBcd.bottom < 0)	rectBcd.bottom = 0;
		if (rectBcd.right >= w - 1)	rectBcd.right = w - 1;
		if (rectBcd.bottom >= h - 1)	rectBcd.bottom = h - 1;
		if (rectBcd.Width() >= 30 && rectBcd.Height() >= 50)
		{
			rectBcd.right -= (rectBcd.Width() % 4);

			BYTE* fmErode = new BYTE[rectBcd.Width() * rectBcd.Height()];
			BYTE* fmTmp = new BYTE[rectBcd.Width() * rectBcd.Height()];
			BYTE* fmDilate = new BYTE[rectBcd.Width() * rectBcd.Height()];

			for (i = rectBcd.top; i < rectBcd.bottom; i++)
			{
				for (j = rectBcd.left; j < rectBcd.right; j++)
				{
					*(fmErode + (i - rectBcd.top) * rectBcd.Width() + (j - rectBcd.left)) = *(fm + i * pitch + j);
				}
			}

			double dAvg = 0.f;

			for (i = 0; i < rectBcd.Height(); i++)
				for (j = 0; j < rectBcd.Width(); j++)
					dAvg += *(fmErode + i * rectBcd.Width() + j);

			dAvg /= (rectBcd.Height() * rectBcd.Width());

			const int kSize = 5;
			long* pInt = new long[kSize * kSize];
			memset(pInt, 0, sizeof(long) * kSize * kSize);
			memcpy(fmDilate, fmErode, rectBcd.Width() * rectBcd.Height());

			for (int iter = 0; iter < 1; iter++)
			{
				Erode_Gray(fmErode, fmTmp, 0, 0, rectBcd.Width() - 1, rectBcd.Height() - 1, rectBcd.Width(), pInt, kSize, kSize);
				memcpy(fmErode, fmTmp, sizeof(BYTE) * rectBcd.Width() * rectBcd.Height());
			}

			/*for (int iter = 0; iter < 4; iter++)
			{
				Dilate_Gray(fmDilate, fmTmp, 0, 0, rectBcd.Width() - 1, rectBcd.Height() - 1, rectBcd.Width(), pInt, kSize, kSize);
				memcpy(fmDilate, fmTmp, sizeof(BYTE) * rectBcd.Width() * rectBcd.Height());
			}*/

			memset(fmTmp, 0xff, rectBcd.Width() * rectBcd.Height());

			int refTh = (int)dAvg - g_Param.m_nBcrDotTh;
			int refUpTh = (int)dAvg + g_Param.m_nBcrDotUpTh;

			for (i = 0; i < rectBcd.Height(); i++)
			{
				for (j = 0; j < rectBcd.Width(); j++)
				{
					if (*(fmErode + i * rectBcd.Width() + j) <= refTh)
						*(fmTmp + i * rectBcd.Width() + j) = 0;

					/*if (*(fmDilate + i * rectBcd.Width() + j) >= refUpTh)
						*(fmTmp + i * rectBcd.Width() + j) = 0;*/
				}
			}

			// 흑점을 Blob
			g_Chain.SetChainData(0, fmTmp, 1, 1, 2, 10000, rectBcd.Width(), rectBcd.Height());
			int nChainCnt = g_Chain.FastChain(0, 0, rectBcd.Width() - 1, rectBcd.Height() - 1);

			if (nChainCnt > 0)
			{
				CRect rect;
				int* minX, * minY, * maxX, * maxY;
				double* areaSize;
				double dWidth, dHeight;
				double dMaxMatchedW = 0;
				double dMaxMatchedH = 0;
				int nMaxMatchedIndex = -1;
				CRect rectMatched;
				CRect rectMostMatched;	// 매칭실패확인용
				int* blobType;

				areaSize = new double[nChainCnt];
				minX = new int[nChainCnt];
				minY = new int[nChainCnt];
				maxX = new int[nChainCnt];
				maxY = new int[nChainCnt];
				blobType = new int[nChainCnt];

				for (i = 0; i < nChainCnt; i++)
				{
					areaSize[i] = g_Chain.Chain_Area(i);
					minX[i] = g_Chain.FindMinX(i);
					minY[i] = g_Chain.FindMinY(i);
					maxX[i] = g_Chain.FindMaxX(i);
					maxY[i] = g_Chain.FindMaxY(i);
					blobType[i] = 1;
				}

				// Blob 갯수가 1개인 경우
				if (nChainCnt == 1)
				{
					CRect rectBlob;
					rectBlob.left = minX[0];
					rectBlob.right = maxX[0];
					rectBlob.top = minY[0];
					rectBlob.bottom = maxY[0];
					dWidth = rectBlob.Width() * g_Param.m_dScaleFactorX;
					dHeight = rectBlob.Height() * g_Param.m_dScaleFactorY;
					if (abs(g_Param.m_dBcrW - dWidth) <= g_Param.m_dBcrDiffW && abs(g_Param.m_dBcrH - dHeight) <= g_Param.m_dBcrDiffH)
					{
						dMaxMatchedW = dWidth;
						dMaxMatchedH = dHeight;
						nMaxMatchedIndex = i;
						rectMatched = rectBlob;
					}
					else
						rectMostMatched = rectBlob;
				}
				else
				{
					// 가까운 영역들은 병합 처리
					for (i = 0; i < nChainCnt; i++)
					{
						CRect rectBlob;
						rectBlob.left = minX[i];
						rectBlob.right = maxX[i];
						rectBlob.top = minY[i];
						rectBlob.bottom = maxY[i];
						const double mergeDistance = 5;

						if (mergeDistance > 0)
						{
							if (i < nChainCnt)
							{
								if (i < (nChainCnt - 1))
								{
									int nStartIndex = i + 1;
									do
									{
										CRect rectBlob2;
										rectBlob2.left = minX[nStartIndex];
										rectBlob2.right = maxX[nStartIndex];
										rectBlob2.top = minY[nStartIndex];
										rectBlob2.bottom = maxY[nStartIndex];

										if ((Distance2NearRect(rectBlob, rectBlob2) < mergeDistance) && blobType[nStartIndex])
										{
											if (rectBlob2.left < rectBlob.left)	rectBlob.left = rectBlob2.left;
											if (rectBlob2.top < rectBlob.top)	rectBlob.top = rectBlob2.top;
											if (rectBlob2.right > rectBlob.right)	rectBlob.right = rectBlob2.right;
											if (rectBlob2.bottom > rectBlob.bottom)	rectBlob.bottom = rectBlob2.bottom;
											blobType[nStartIndex] = 0;
										}
										nStartIndex++;

									} while (nStartIndex < nChainCnt);
								}

								dWidth = rectBlob.Width() * g_Param.m_dScaleFactorX;
								dHeight = rectBlob.Height() * g_Param.m_dScaleFactorY;
								if (abs(g_Param.m_dBcrW - dWidth) <= g_Param.m_dBcrDiffW && abs(g_Param.m_dBcrH - dHeight) <= g_Param.m_dBcrDiffH)
								{
									if (dMaxMatchedW > 0 && dMaxMatchedH > 0)
									{
										if ((pow(dWidth - g_Param.m_dBcrW, 2) + pow(dHeight - g_Param.m_dBcrH, 2)) < (pow(dMaxMatchedW - g_Param.m_dBcrW, 2) + pow(dMaxMatchedH - g_Param.m_dBcrH, 2)))
										{
											dMaxMatchedW = dWidth;
											dMaxMatchedH = dHeight;
											nMaxMatchedIndex = i;
											rectMatched = rectBlob;
										}
									}
									else
									{
										dMaxMatchedW = dWidth;
										dMaxMatchedH = dHeight;
										nMaxMatchedIndex = i;
										rectMatched = rectBlob;
									}
								}
								else
								{
									if (rectMostMatched.Width() == 0)
										rectMostMatched = rectBlob;
									else
									{
										double tmpW = rectMostMatched.Width() * g_Param.m_dScaleFactorX;
										double tmpH = rectMostMatched.Height() * g_Param.m_dScaleFactorY;
										if (pow(dWidth - g_Param.m_dBcrW, 2) + pow(dHeight - g_Param.m_dBcrH, 2) < pow(tmpW - g_Param.m_dBcrW, 2) + pow(tmpH - g_Param.m_dBcrH, 2))
											rectMostMatched = rectBlob;
									}
								}
							}
						}
					}
				}

				if (nMaxMatchedIndex >= 0)
				{
					g_Temp.m_BcrRectForMatch = CRect(
						rectBcd.left + rectMatched.left,
						rectBcd.top + rectMatched.top,
						rectBcd.left + rectMatched.right,
						rectBcd.top + rectMatched.bottom);

					// Blob 영역을 Fine Pat 영역으로 치환
					// GetBcrFineArea 에서 찾은 영역이 오차가 있음
					g_Temp.m_BcrRectFine = g_Temp.m_BcrRectForMatch;

					// 이진화로 패턴 영역 찾은 결과 
					g_Temp.m_nBcrPatFind = eFineRectOK;
				}
				else
				{
					g_Temp.m_BcrRectForMatch = CRect(0, 0, 0, 0);
					g_Temp.m_BcrRectFine = CRect(0, 0, 0, 0);
				}

				delete[] areaSize;
				delete[] minX;
				delete[] minY;
				delete[] maxX;
				delete[] maxY;
				delete[] blobType;
			}
			else
			{
				g_Temp.m_BcrRectForMatch = CRect(0, 0, 0, 0);
				g_Temp.m_BcrRectFine = CRect(0, 0, 0, 0);
			}

			delete[] pInt;
			delete[] fmDilate;
			delete[] fmErode;
			delete[] fmTmp;
		}
	}
	else
	{
		if (g_Param.m_nBcrDotTh > 0)	g_Temp.m_BcrRectForMatch = GetBarcodeRect2(fm, w, h, pitch, g_Temp.m_nEdgeDir == 1 ? -1 : 1);
		else							g_Temp.m_BcrRectForMatch = CRect(0, 0, 0, 0);
	}

	if (g_Param.m_useBCRManualArea == true)
	{
		g_Temp.m_BcrRect.left = g_Param.m_nBCRAreaL;
		g_Temp.m_BcrRect.right = g_Param.m_nBCRAreaR;
	}

	g_Temp.m_BcrRect.NormalizeRect();
}

bool CheckValidCode(CString str)
{
	bool bRet = true;
	int legnth = 0;

	legnth = str.GetLength();

	if (legnth != BCR_CODE_LENGTH)
		return (FALSE);

	for (int i = 0; i < legnth; i++)
	{
		TCHAR t = str.GetAt(i);
		if (!IsCharAlphaNumeric(t) && t != '_' && t != '-')
			bRet = FALSE;
	}
	return (bRet);
}

int CheckBcrOrder(CString NewBarcode, CString LastBarcode)
{
	int NewBarcodePos;
	int LastBarcodePos;
	int nRst = 0;

	NewBarcodePos = _ttoi(NewBarcode.Right(6));
	LastBarcodePos = _ttoi(LastBarcode.Right(6));

	if (NewBarcodePos == 0 || LastBarcodePos == 0)
	{
		nRst = 0;
		return nRst;
	}

	if (NewBarcodePos > LastBarcodePos)
	{
		nRst = NewBarcodePos - LastBarcodePos;
	}
	else
	{
		nRst = LastBarcodePos - NewBarcodePos;
	}

	return nRst;
}

int m_nBcrPreYPos = 0;

void SearchDefectData(LPVOID pParent, int crtFrameNum, int lastBcrFrameNum)
{
	CString strLot = g_Temp.m_slotName;

	CString strFileName, strTmp;

	if (strLot.GetLength() == 0)	strTmp.Format(_T(""));
	else							strTmp.Format(_T("%s%s"), BCR_PATH, strLot);

	if (DirectoryExist(strTmp.GetBuffer()) == false)
		CreateDirectory(strTmp, NULL);

	g_Temp.m_strBcrLog.Format(_T("%sBcrSearchDefectData"), strTmp);
	int csvType = g_Param.m_nBcrCsvType;

	int i;
	CString str, strBcrLotName, strLog;
	double temp1 = 0;
	double temp2 = 0;
	TCHAR* position;
	int nMaxCount = 0;
	int nLeftSkip = 0;
	int nRightSkip = 0;
	double dSum = 0;
	static double dBcdScaleBuf[3];

	if (g_Temp.m_isBcrSuccessRead == true)
	{
		// 바코드(BCNO)에서 바코드 위치정보를 뽑아내는 부분  --> 회사에서 테스트 시, 사용 안함 
		str = g_Temp.m_strBcrName.Right(BCR_POS_DATA_LENGTH);
		strBcrLotName = g_Temp.m_strBcrName.Left(LOT_NAME_LENGTH);
		g_Temp.m_dBcrCrtRealPos = _tcstod((LPCTSTR)(str), &position);
		g_Temp.m_dBcrCrtRealPos *= 1000.0;

		strLog.Format(_T("BCR NO: %s"), strBcrLotName);
		WriteBcrDefectLog(g_Temp.m_strBcrLog, strLog);

		g_Temp.m_dBcrOffsetY = g_Temp.m_BcrRectFine.top * g_Param.m_dBcrScaleFactorY;

		if (g_Temp.m_isBcrFirstCheck == true) // 처음 이면, scale factor default 값 입력 --> 파라미터 읽어올 때 입력되어 있음  
		{
			g_Temp.m_isBcrFirstCheck = false;
		}
		else	// 처음이 아닐 경우, 이전 바코드 위치와 좌표를 바탕으로 scale factor 계산 
		{
			g_Temp.m_dBcrScale[g_Temp.m_nBcrScaleIdx % 3] = (double)(fabs)(g_Temp.m_dBcrCrtRealPos - g_Temp.m_dBcrPreRealPos) /
				(double)((crtFrameNum - g_Temp.m_nBcrPreInspFrame) * g_System.m_nImageH + g_Temp.m_BcrRectFine.top - m_nBcrPreYPos);

			if (g_Temp.m_nBcrScaleIdx >= 3)
			{
				for (i = 0; i < 3; i++)
					dSum += g_Temp.m_dBcrScale[i];

				g_Param.m_dBcrScaleFactorY = dSum / 3.0;

				if (g_Param.m_dBcrScaleFactorY > g_Param.m_dScaleFactorY * 1.2 || g_Param.m_dBcrScaleFactorY > g_Param.m_dScaleFactorY * 0.8)
				{
					strLog.Format(_T("BarcodeScaleY: %f, m_dScaleFactorY: %f"), g_Param.m_dBcrScaleFactorY, g_Param.m_dScaleFactorY);
					WriteBcrDefectLog(g_Temp.m_strBcrLog, strLog);
					g_Param.m_dBcrScaleFactorY = g_Param.m_dScaleFactorY;
				}
				strLog.Format(_T("BarcodeScaleY: %f, m_dCurrentPos_Bar %f, m_dLastPos_Bar %f, frame_num %d, m_nLastBarcodeFrame %d, m_BarRectY.top %d, m_nLastBarcodeY %d m_nIndexBcdScale %d"),
					g_Param.m_dBcrScaleFactorY, g_Temp.m_dBcrCrtRealPos, g_Temp.m_dBcrPreRealPos, crtFrameNum, g_Temp.m_nBcrPreInspFrame, g_Temp.m_BcrRectFine.top, m_nBcrPreYPos, g_Temp.m_nBcrScaleIdx);
				WriteBcrDefectLog(g_Temp.m_strBcrLog, strLog);
			}

			g_Temp.m_nBcrScaleIdx++;

			//2번이상 읽었을 때 숫자 증감여부를 판단
			//2번 이상째부터 인식된 경우
			if (g_Temp.m_isBcrForceReading == true)
			{
				if ((g_Temp.m_dBcrCrtRealPos - g_Temp.m_dBcrPreRealPos) > 0) // 바코드 숫자가 증가
				{
					if (g_Temp.m_bBcrForceDir == false)
					{
						l_Send_Server.SendCommand_LocalHost(NM_FORCE_BCR_NOT_EQAUL_BCR_DIR);
						strLog.Format(_T("[FORCE_BCR]Not Eqaul BCR Direction - increase"));
						WriteLog(strLog);
					}
					g_Temp.m_nBcrDir = 1;	// 증가
				}
				else if ((g_Temp.m_dBcrCrtRealPos - g_Temp.m_dBcrPreRealPos) < 0) // 바코드 숫자가 감소
				{
					if (g_Temp.m_bBcrForceDir == true)
					{
						l_Send_Server.SendCommand_LocalHost(NM_FORCE_BCR_NOT_EQAUL_BCR_DIR);
						strLog.Format(_T("[FORCE_BCR]Not Eqaul BCR Direction - decrease"));
						WriteLog(strLog);
					}
					g_Temp.m_nBcrDir = -1;	// 감소
				}

				if (g_Temp.m_isBcrForceReading == true && g_Temp.m_isBcrFirstCode == true)
				{
					// Force 파라미터 초기화 추가
				}
			}
			else
			{
				if ((g_Temp.m_dBcrCrtRealPos - g_Temp.m_dBcrPreRealPos) > 0) // 바코드 숫자가 증가
					g_Temp.m_nBcrDir = 1;	// 증가
				else if ((g_Temp.m_dBcrCrtRealPos - g_Temp.m_dBcrPreRealPos) < 0) // 바코드 숫자가 감소
					g_Temp.m_nBcrDir = -1;	// 감소
			}

			g_Temp.m_isBcrInitRead = true;
			g_Temp.m_nBcrOrder = CheckBcrOrder(g_Temp.m_strBcrName, g_Temp.m_strPreBcrName);
		}

		// 마지막 바코드 값 입력 -->이전 바코드 프레임, 이전 바코드 영상내 위치, 이전 바코드 ROLL상 위치(mm 단위)  
		m_nBcrPreYPos = g_Temp.m_BcrRectFine.top;
		g_Temp.m_nBcrPreInspFrame = crtFrameNum;
		g_Temp.m_dBcrPreRealPos = g_Temp.m_dBcrCrtRealPos;
		g_Temp.m_strPreBcrName = g_Temp.m_strBcrName;
	}
	else
	{
		if (g_Temp.m_strPreBcrName.GetLength() > LOT_NAME_LENGTH)
			strBcrLotName = g_Temp.m_strPreBcrName.Left(LOT_NAME_LENGTH);
	}

	int nCrtMeter = 20;
	//-----------------------------------------------------------------------------
	//바코드 위치에서 위치 값 읽은 후 조건에 맞는 데이터 검색
	if (g_Temp.m_isBcrFirstCheck == false && (g_Temp.m_nBcrDir == -1 || g_Temp.m_nBcrDir == 1))
	{
		double bar_frame_length = 0; //바코드 상 거리로 인한 프레임 길이 값 
		bar_frame_length = g_Param.m_dBcrScaleFactorY * g_System.m_nImageH;

		int nDistMeter = g_Param.m_nMarkingDefectMarking;
		int nTenMeterFrame = (int)(1000.0 * nDistMeter / bar_frame_length); //몇Frame을 전의 Defect/frame 개수를 보낼지 Meter->Frame계산

		// 위치 정보 계산.
		double dNextFramePos, dCurFramePos;
		if (g_Temp.m_nBcrDir == 1)  //바코드 수가 증가할 경우 
		{
			dNextFramePos = g_Temp.m_dBcrCrtRealPos - g_Temp.m_dBcrOffsetY + (crtFrameNum - lastBcrFrameNum + 1) * bar_frame_length;
			dCurFramePos = g_Temp.m_dBcrCrtRealPos - g_Temp.m_dBcrOffsetY + (crtFrameNum - lastBcrFrameNum) * bar_frame_length;
		}
		else	//감소
		{
			dNextFramePos = g_Temp.m_dBcrCrtRealPos + g_Temp.m_dBcrOffsetY - (crtFrameNum - lastBcrFrameNum + 1) * bar_frame_length;
			dCurFramePos = g_Temp.m_dBcrCrtRealPos + g_Temp.m_dBcrOffsetY - (crtFrameNum - lastBcrFrameNum) * bar_frame_length;
		}

		if (g_Temp.m_dBcrPreFramePos == 0)	//처음 초기값
			g_Temp.m_dBcrPreFramePos = dCurFramePos;

		// 불량 데이터 검색
		if (g_Temp.m_nBcrDir == 1)  //바코드 수가 증가할 경우 
			((CKoWebView*)pParent)->m_DefectCallClass->GetMarkDefectData(strBcrLotName, dCurFramePos, dNextFramePos);
		else	//감소
			((CKoWebView*)pParent)->m_DefectCallClass->GetMarkDefectData(strBcrLotName, dNextFramePos, dCurFramePos);

		// Area Maring 데이터 검색
		// UseAreaDel은 함수 내부에서 확인함
		((CKoWebView*)pParent)->m_DefectCallClass->GetMarkAreaDefectData(dCurFramePos, dNextFramePos);
		
		// Bcr 영역 Marking 처리
		if (g_Temp.m_isBcrSuccessRead)
		{
			g_Defect.m_BMarkDefect.x = (float)(g_Temp.m_BcrRectFine.left * g_Param.m_dScaleFactorX + g_Param.m_dCamStartPosX);
			g_Defect.m_BMarkDefect.y = (float)(g_Temp.m_BcrRectFine.top * g_Param.m_dScaleFactorY + g_Param.m_dCamStartPosY);
			g_Defect.m_BMarkDefect.width = (float)(g_Temp.m_BcrRectFine.Width() * g_Param.m_dScaleFactorX);
			g_Defect.m_BMarkDefect.height = (float)(g_Temp.m_BcrRectFine.Width() * g_Param.m_dScaleFactorX);
			g_Defect.m_BMarkDefect.type = csvType;
			g_Defect.m_BMarkDefect.position = (int)g_Temp.m_dBcrCrtRealPos;

			if (csvType == eCSV_TYPE_NITTO || csvType == eCSV_TYPE_NITTO_RK
				|| csvType == eCSV_TYPE_NITTO_RTS || csvType == eCSV_TYPE_KORENO_RK_IJP) 
			{
				//g_Defect.m_BMarkDefect.defect_class = 6;
				g_Defect.m_BMarkDefect.defect_class = g_Temp.m_nPCNum *1000000 + 18; // 확인 필요
			}
			else
			{
				//g_Defect.m_BMarkDefect.defect_class = 9;
				g_Defect.m_BMarkDefect.defect_class = g_Temp.m_nPCNum * 1000000 + 18; // 확인 필요
			}
			g_Defect.m_BMarkDefect.mark = (int)g_Param.m_bBcrMark;
			strcpy_s(g_Defect.m_BMarkDefect.fileName, MAX_BADIMAGE_FILENAME, g_Temp.m_cBcrFileName);
			g_Defect.m_nBcrCount = 1;
		}

		// 이전 위치 데이터 업데이트
		g_Temp.m_dBcrPreFramePos = dCurFramePos;
	}
}

// PVA 내의 dot 확인하여 에지 경계 새로 추출함.
// top->bottom으로 검색하여 pixel 변화량 폭 계산과 주기율을 확인하여 
// nulling dot으로 인식하고 새로운 에지 영역 처리
// 원래라면 FFT 주파수 추출이 좋기는 한데.... 
bool CheckDotOnPVA(LPBYTE fm, int w, int h, int pitch, int left, int right, int dir)
{
	int diffVal = g_Param.m_nBcrNullDotDiff;
	int offset = g_Param.m_nBcrNullDotOffset;
	int checkMin = g_Param.m_nBcrNullDotCheckMin;
	int connMin = g_Param.m_nBcrNullDotConnMin;

	int range = right - left + 1;
	m_tmpNullDotPos = -1;
	// 나중에 
	int* freqCnt = new int[range];
	int* dirFlag = new int[range];
	int diff;

	memset(freqCnt, 0x00, sizeof(int) * range);
	memset(dirFlag, 0x00, sizeof(int) * range);

	int cont = 0;
	for (int j = left, k = 0; j <= right; j++, k++)
	{
		for (int i = 0; i < h - offset; i++)
		{
			diff = *(fm + (i + offset) * w + j) - *(fm + i * w + j);
			if (dirFlag[k] == 0)	// 시작
			{
				if (diff < -diffVal)
					dirFlag[k] = -1;
				else if (diff > diffVal)
					dirFlag[k] = 1;
			}
			else if (dirFlag[k] < 0)	// 감소
			{
				if (diff > diffVal)
				{
					if (cont >= 1)
					{
						dirFlag[k] = 1;
						i += (offset - 1); // 점프
					}
					else cont++;

				}
				else
					cont = 0;
			}
			else if (dirFlag[k] > 0) // 증가
			{
				if (diff < -diffVal)
				{
					if (cont >= 1)
					{
						dirFlag[k] = -1;
						freqCnt[k]++;
						i += (offset - 1); // 점프
					}
					else
						cont++;
				}
				else
					cont = 0;
			}
		}
	}

	if (dir == 0)	// 에지 오른쪽에 BCR 존재
	{
		for (int i = range; i >= 5; i--)
		{
			if (freqCnt[i] >= checkMin)
			{
				int k = i - 1;
				int subCnt = 1;
				bool isFault = false;
				while (k > i - 5)
				{
					if (freqCnt[k] >= checkMin)
						subCnt++;
					else
					{
						if (subCnt < connMin)
						{
							isFault = true;
							break;
						}
					}
					k--;
				}
				if (isFault == false)
				{
					m_tmpNullDotPos = i + left;
					break;
				}
			}
		}
	}
	else			// 에지 왼쪽에 BCR 존재
	{
		for (int i = 0; i <= range - 5; i++)
		{
			if (freqCnt[i] >= checkMin)
			{
				int k = i + 1;
				int subCnt = 1;
				bool isFault = false;
				while (k < i + 5)
				{
					if (freqCnt[k] >= checkMin)
						subCnt++;
					else
					{
						if (subCnt < connMin)
						{
							isFault = true;
							break;
						}

					}
					k++;
				}
				if (isFault == false)
				{
					m_tmpNullDotPos = i + left;
					break;
				}
			}
		}
	}

	bool bRes = true;
	if (m_tmpNullDotPos == -1)
		bRes = false;

	delete[] freqCnt;
	delete[] dirFlag;

	return bRes;
}

bool Read2DMatrix(cv::Mat roiImage)
{
	CString strCode;
	bool isFind = true;
	cv::Mat image;
	if (roiImage.cols % 4 != 0)
	{

		int newCols = (int)(roiImage.cols / 4.0);
		newCols *= 4;
		cv::resize(roiImage, image, cv::Size(newCols, roiImage.rows));
	}
	else
	{
		image = roiImage.clone();
	}
	double start, end;

	start = clock();
	std::string szCode[2] = { "", "" };
	szCode[0] = g_CodeReader.CodeRead(image.data, image.cols, image.rows, false);
	if (szCode[0].compare("") == 0)
	{
		szCode[1] = g_CodeReader.CodeRead(image.data, image.cols, image.rows, true);
		if (szCode[1].compare("") == 0)
			isFind = false;
		else
			strCode.Format(_T("%s"), szCode[1].c_str());
	}
	else
		strCode.Format(_T("%s"), szCode[0].c_str());
	image.release();

	if (isFind == true)
	{
		isFind = CheckValidCode(strCode);
		if (isFind == true) 
		{
			g_Temp.m_strBcrName = strCode;
			g_Temp.m_nBcrPatFind = eReadDone;
		}
	}
	end = clock();

	return isFind;
}

std::list<cv::Rect> FindBarcodePosition(cv::Mat image)
{
	cv::Mat bw, resizeImg, resizeBw;
	cv::resize(image, resizeImg, cv::Size(image.cols, image.rows / 2));
	cv::adaptiveThreshold(resizeImg, resizeBw, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 9, -10);
	cv::resize(resizeBw, bw, cv::Size(image.cols, image.rows));

	LPBYTE fmBinary = bw.data;
	double* sumValue = new double[bw.cols];
	ZeroMemory(sumValue, sizeof(sumValue) * bw.cols);

#pragma omp parallel for
	for (int i = 0; i < bw.cols; i++)
	{
		for (int j = 0; j < bw.rows; j++)
		{
			sumValue[i] += *(fmBinary + j * bw.cols + i);
		}
	}

	double maxValue = 0;
	int maxPosition = 0;

	for (int i = 0; i < bw.cols; i++)
	{
		if (sumValue[i] > maxValue)
		{
			maxValue = sumValue[i];
			maxPosition = i;
		}
	}

	delete[] sumValue;
	cv::Mat leftImg, rightImg;
	leftImg = image(cv::Rect(0, 0, maxPosition - 1, 256));
	rightImg = image(cv::Rect(maxPosition + 1, 0, image.cols - maxPosition - 1, 256));
	cv::Scalar meanL = cv::mean(leftImg);
	cv::Scalar meanR = cv::mean(rightImg);

	bool isBgLeft = false;
	int bcWidth = 500;
	cv::Mat cropedImg; // 에지영역에서 bcWidth 영역 만큼 잘라낸 이미지
	if (meanL[0] > meanR[0]) // 오른쪽에 필름
	{
		isBgLeft = true;
		if (maxPosition + bcWidth > image.cols)
			cropedImg = image(cv::Rect(maxPosition, 0, image.cols - maxPosition, image.rows));
		else
			cropedImg = image(cv::Rect(maxPosition, 0, bcWidth, image.rows));
	}
	else
	{
		if (maxPosition - bcWidth < 0)
			cropedImg = image(cv::Rect(0, 0, maxPosition, image.rows));
		else
		{
			cropedImg = image(cv::Rect(maxPosition - bcWidth, 0, bcWidth, image.rows));
		}
	}

	bw.release();
	resizeImg.release();
	resizeBw.release();
	cv::resize(cropedImg, resizeImg, cv::Size(cropedImg.cols / 2, cropedImg.rows / 2));
	cv::adaptiveThreshold(cropedImg, resizeBw, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 9, -5);
	cv::resize(resizeBw, bw, cv::Size(cropedImg.cols, cropedImg.rows));

	int offset = 256;
	cv::Mat vconcatImg;
	int loop = bw.rows / offset;
	for (int i = 0; i < loop; i++)
	{
		cv::Rect rt2;
		cv::Rect rt1(0, offset * i, bw.cols, offset);
		if (i != loop - 1)
		{
			rt2.x = 0;
			rt2.y = offset * (i + 1);
			rt2.width = bw.cols;
			rt2.height = offset;
		}
		else
		{
			rt2.x = 0;
			rt2.y = 0;
			rt2.width = bw.cols;
			rt2.height = offset;
		}
		cv::Mat source1 = bw(rt1);
		cv::Mat source2 = bw(rt2);
		cv::Mat diff;
		cv::subtract(source1, source2, diff);

		if (i == 0)
			vconcatImg = diff.clone();
		else
		{
			cv::Mat tempImg = vconcatImg.clone();
			cv::vconcat(tempImg, diff, vconcatImg);
		}
	}

	cv::Mat mask = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(1, 1));
	cv::dilate(vconcatImg, vconcatImg, mask, cv::Point(-1, -1), 3);
	cv::erode(vconcatImg, vconcatImg, mask, cv::Point(-1, -1), 7);

	cv::Mat img_label, stats, centroid;
	int numOfLabels = cv::connectedComponentsWithStats(vconcatImg, img_label, stats, centroid, 8, CV_32S);
	int num = 1;
	std::list<cv::Rect> listBarcode;
	for (int i = 1; i < numOfLabels; i++)
	{
		int area = stats.at<int>(i, cv::CC_STAT_AREA);
		int left = stats.at<int>(i, cv::CC_STAT_LEFT);
		int top = stats.at<int>(i, cv::CC_STAT_TOP);
		int width = stats.at<int>(i, cv::CC_STAT_WIDTH);
		int height = stats.at<int>(i, cv::CC_STAT_HEIGHT);

		double areaRatio = (double)area / (double)(width * height);

		if (isBgLeft)
		{
			if (area > 1000 && width > 45 && areaRatio > 0.5) {
				//cv::rectangle(colorImg, cv::Point(left + maxPosition, top), cv::Point(left + maxPosition + width, top + height), cv::Scalar(0, 0, 255), 3);
				cv::Rect rect(cv::Point(left + maxPosition, top), cv::Point(left + maxPosition + width, top + height));
				cv::Rect validRect = SetBarcodeArea(rect, image.cols, image.rows);
				listBarcode.push_back(validRect);
			}
		}
		else
		{
			if (area > 1000 && width > 45 && areaRatio > 0.5) {
				if (maxPosition - bcWidth < 0)
				{
					//cv::rectangle(colorImg, cv::Point(left, top), cv::Point(left + width, top + height), cv::Scalar(0, 0, 255), 3);
					cv::Rect rect(cv::Point(left, top), cv::Point(left + width, top + height));
					cv::Rect validRect = SetBarcodeArea(rect, image.cols, image.rows);
					listBarcode.push_back(validRect);
				}
				else
				{
					//cv::rectangle(colorImg, cv::Point(maxPosition - bcWidth + left, top), cv::Point(maxPosition - bcWidth + left + width, top + height), cv::Scalar(0, 0, 255), 3);
					cv::Rect rect(cv::Point(maxPosition - bcWidth + left, top), cv::Point(maxPosition - bcWidth + left + width, top + height));
					cv::Rect validRect = SetBarcodeArea(rect, image.cols, image.rows);
					listBarcode.push_back(validRect);
				}
			}
		}
	}

	bw.release();
	return listBarcode;
}

cv::Rect SetBarcodeArea(cv::Rect rect, int width, int height)
{
	cv::Rect modifyRect(rect);
	// X
	if (modifyRect.x > 100)
	{
		modifyRect.x -= 100;
		modifyRect.width += 100;
	}
	else
	{
		modifyRect.x = 0;
		modifyRect.width += modifyRect.x;
	}

	if (modifyRect.x + modifyRect.width < width - 100)
	{
		modifyRect.width += 100;
	}
	else
	{
		modifyRect.width = width - modifyRect.x - 1;
	}

	// Y
	if (modifyRect.y > 100)
	{
		modifyRect.y -= 100;
		modifyRect.height += 100;
	}
	else
	{
		modifyRect.y = 0;
		modifyRect.height += modifyRect.y;
	}

	if (modifyRect.y + modifyRect.height < height - 100)
	{
		modifyRect.height += 100;
	}
	else
	{
		modifyRect.height = height - modifyRect.y - 1;
	}
	return modifyRect;
}

void UpdateBcrImagePathToDefect(int frameNum)
{
	double dBcrX = 0.f, dBcrY = 0.f;
	dBcrX = (g_Temp.m_BcrRectFine.left + g_Temp.m_BcrRectFine.Width() / 2.0) * g_Param.m_dScaleFactorX;
	dBcrY = (g_Temp.m_BcrRectFine.top + g_Temp.m_BcrRectFine.Height() / 2.0) * g_Param.m_dScaleFactorY;

	CString sNGImageName, sNGImageFullName;
	sNGImageName.Format(_T("BCNO_MATCHED_[%05d]%.3f_%.3f.bmp"), frameNum, dBcrX, dBcrY);
	strcpy_s(g_Defect.m_Defect[0].filename, sNGImageName.GetLength()+1, CW2A(sNGImageName));

	sNGImageFullName.Format(_T("%s%s\\%s\\%s"), NG_IMAGE_PATH, g_Temp.m_slotName, g_Temp.m_sMyComName, sNGImageName);
	strcpy_s(g_Temp.m_cBcrFileName, sNGImageFullName.GetLength() + 1, CW2A(sNGImageFullName));
}

void CvtBcrImage2SaveDefectImage(LPBYTE fm, int w, int h, CRect rect, LPBYTE out, int outW, int outH)
{
	int i, j;
	int refW = rect.Width();
	int refH = rect.Height();
	int stX = rect.left;
	int stY = rect.top;

	double rateW = (double)refW / (double)outW;
	double rateH = (double)refH / (double)outH;

	double tarX, tarY;
	double diffX, diffY;
	int nTarX, nTarY;
	double w11, w12, w21, w22;
	byte uc11, uc12, uc21, uc22;
	byte ucOut;
	for (i = 0; i < outH; i++)
	{
		for (j = 0; j < outW; j++)
		{
			tarX = stX + rateW * j;
			tarY = stY + rateH * i;
			nTarX = (int)tarX;
			nTarY = (int)tarY;
			diffX = tarX - (double)(nTarX);
			diffY = tarY - (double)(nTarY);

			w11 = (1.0 - diffX) * (1.0 - diffY);
			w12 = diffX * (1.0 - diffY);
			w21 = (1.0 - diffX) * diffY;
			w22 = diffX * diffY;
			uc11 = *(fm + nTarY * w + nTarX);
			uc12 = *(fm + nTarY * w + nTarX + 1);
			uc21 = *(fm + (nTarY + 1) * w + nTarX);
			uc22 = *(fm + (nTarY + 1) * w + nTarX + 1);
			ucOut = (byte)(w11 * uc11 + w12 * uc12 + w21 * uc21 + w22 * uc22);
			if (ucOut > 255) ucOut = 255;

			*(out + i * outW + j) = ucOut;
		}
	}
}

void CheckBcrLotDataError(CString strBcno)
{
	int type = g_Param.m_nBcrCsvType;
	if (type == eCSV_TYPE_KORENO_RK_IJP || type == eCSV_TYPE_NITTO_RTS || type == eCSV_TYPE_NITTO_RK)
	{
		if (g_Param.m_nNoInspect == false)
		{
			int nBCNO_nowSize = (int)(g_Temp.m_arLoadedBCNO[0].GetSize());
			int nBCNO_nextSize = (int)(g_Temp.m_arLoadedBCNO[1].GetSize());

			CString strDataNow, strDataNext;
			CString strMsgNow, strMsgNext;
			for (int i = 0; i < nBCNO_nowSize; i++)
			{
				strDataNow += g_Temp.m_arLoadedBCNO[0][i];
				strDataNow += ",";
			}
			for (int i = 0; i < nBCNO_nextSize; i++)
			{
				strDataNext += g_Temp.m_arLoadedBCNO[1][i];
				strDataNext += ",";
			}

			strMsgNow.Format(_T("Now %s read : %s"), strDataNow, strBcno);
			strMsgNext.Format(_T("Now %s read : %s"), strDataNext, strBcno);

			bool bFind = false;
			for (int i = 0; i < nBCNO_nowSize; i++)
			{
				if (strBcno == g_Temp.m_arLoadedBCNO[0][i])
				{
					bFind = true;
					break;
				}
			}
			if (bFind == FALSE && strBcno != _T("no_barcode") && nBCNO_nowSize>0)
				WriteLog(strMsgNow);

			// bFind = false; ??? 확인 필요함
			for (int i = 0; i < nBCNO_nextSize; i++)
			{
				if (strBcno == g_Temp.m_arLoadedBCNO[1][i])
				{
					bFind = true;
					break;
				}
			}
			if (bFind == FALSE && strBcno != _T("no_barcode") && nBCNO_nextSize > 0)
				WriteLog(strMsgNext);

			if (g_Temp.m_isBcrFirstCode==true && strBcno != _T("no_barcode") && nBCNO_nowSize && bFind == FALSE)
			{
				if (g_Temp.m_bFirstCompare==false || g_Temp.m_nGrabFrame > 30)
				{
					if (strBcno.GetLength() > 9)
					{
						if ((type == eCSV_TYPE_NITTO_RTS) || (type == eCSV_TYPE_NITTO_RK))
						{
							if (g_Param.m_isUseES || g_Param.m_isUseTG)	//연신,그외 체크되었을 때만 비교
							{
								l_Send_Server.SendCommand_LocalHost(NM_BCR_LOT_DATA_ERROR);
								strMsgNow += _T(" Alarm");
								WriteLog(strMsgNow);
							}
						}
						else
						{
							if (g_Param.m_isUseES)	//연신 체크되었을 때만 비교
							{
								l_Send_Server.SendCommand_LocalHost(NM_BCR_LOT_DATA_ERROR);
								strMsgNow += _T(" Alarm");
								WriteLog(strMsgNow);
							}
						}
					}
				}
				if (g_Temp.m_bFirstCompare==true)
				{
					g_Temp.m_bFirstCompare = false;		//처음 읽은 바코드는 비교하여 에러 메세지를 보내지 않는다.
				}
			}
		} 
	}
}