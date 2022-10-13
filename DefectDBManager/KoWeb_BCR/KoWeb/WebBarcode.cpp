#include "Stdafx.h"

#include <memory.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include <dvec.h>
#include <math.h>

#include "KoWebDoc.h"
#include "KoWebView.h"
#include "BCR/CallClassWrapperCodeReader.h"

//BARCODE_VISION

#define BCR_CODE_LENGTH 17

CallClassWrapperCodeReader g_CodeReader;

extern CXManageSocket  l_Send_Server;

BYTE l_fmBCRBK[512 * 128];
BYTE l_fmBCR[512 * 2178];

int GetBCRData(LPBYTE fm, int left, int top, int w, int h, int pitch, int* pX, int* pY, TCHAR sBcr[][30]);

bool SearchBCR(LPVOID pParent);
double Distance2NearRect(CRect r1, CRect r2);
CRect GetBcrFineArea(LPBYTE fm, int left, int top, int w, int h, int pitch);
CPoint GetBcrCenter(unsigned char* fm, CRect rt, int nPitch);
CRect GetBarcodeRect2(LPBYTE fm, int w, int h, int pitch, int ndirect);
void GetBcrPosition(LPBYTE fm, int left, int top, int w, int h, int pitch);
BOOL CheckValidCode(CString str);
int CheckBcrOrder(CString NewBarcode, CString LastBarcode);
void SearchDefectData(LPVOID pParent, int crtFrameNum, int lastBcrFrameNum);
void WriteBcrDefectLog(CString path, CString str);

void WEB_Barcode(LPVOID pParent)
{
	int i, j, m, nOverlap;
	int nBcrPitch = 512;
	LPBYTE fm = g_fmGrab[g_ID];
	int pitch = g_System.m_nPitch;
	int width = g_System.m_nImageW;
	int height = g_System.m_nImageH;
	int nX, nBCRCount;
	int nBcrX[10], nBcrY[10];
	TCHAR sBCR[10][30];

	g_Temp.m_nMaxGray = 0;
	g_Temp.m_nMinGray = 255;

	if (g_Param.m_nNotInspArea == 0) return;

	if (g_Temp.m_nEdgeDir == 0) nX = g_Temp.m_nInspectX1;
	else                     nX = g_Temp.m_nInspectX2 - nBcrPitch;

	// 기존 방식 검사 
	if (SearchBCR(pParent) == false) // 검사 실패한 경우 재검사
	{

	}
	else // 완료하면 후처리???
	{

	}

	for (i = 0; i < 128; i++)
		memcpy(l_fmBCR + nBcrPitch * i, l_fmBCRBK + nBcrPitch * i, nBcrPitch);
	for (i = 0; i < g_System.m_nImageH; i++)
		memcpy(l_fmBCR + nBcrPitch * (i + 128), fm + pitch * i + nX, nBcrPitch);

	nBCRCount = GetBCRData(l_fmBCR, 0, 0, nBcrPitch, 2178, nBcrPitch, nBcrX, nBcrY, sBCR);


	for (i = 0; i < nBCRCount; i++)
	{

	}

	//가장 마지막 촬상한것 보관-----------------------------------
	for (i = 0; i < 128; i++)
		memcpy(l_fmBCRBK + nBcrPitch * i, fm + pitch * i + nX, nBcrPitch);
	//------------------------------------------------------------

	

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
	int i, j, m, nOverlap;
	LPBYTE fm = g_fmGrab[g_ID];
	int pitch = g_System.m_nPitch;
	int width = g_System.m_nImageW;
	int height = g_System.m_nImageH;

	int nFrameNum = g_Temp.m_nGrabFrame;

	CString strLog;
	g_Temp.m_isBcrSuccessRead = false;
	g_Temp.m_nBcrPatFind = 0;
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
				g_Temp.m_nBcrPatFind = 1;
			}
			else
				g_Temp.m_nBcrPatFind = 0;
		}
		else
			g_Temp.m_nBcrPatFind = 0;

		if (strReadMsg == _T(""))
			strReadMsg = _T("no_barcode");

		int nMatchFrame = 0;
		const int nPreFrm = g_Temp.m_nBcrPreInspFrame;

		g_Temp.m_strBcrName = strReadMsg;

		// 강제 입력 시 방향을 설정해준다. 
		if (g_Temp.m_bBcrForceInsert == true)
		{
			if (g_Temp.m_bBcrForceDir == true) // 증가
				g_Temp.m_nBcrDir = 1;
			else
				g_Temp.m_nBcrDir = 0;
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

			CRect rectBCD;
			rectBCD = g_Temp.m_BcrRectForMatch;
			strBcrMsg.Format(_T("Matched"));
			nRet = 1;


			if (nRet > 0)
			{
				g_Temp.m_BcrRectMatched = rectBCD;
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
					g_Temp.m_nBcrPatFind = 2;
				}
				else
				{
					CString str;
					str.Format(L"This message should not be logged %d,%d", nFrameNum, g_Temp.m_nBcrPreInspFrame);
					WriteLog(str);
				}
			}
		}

		if (g_Temp.m_isBcrSuccessRead == true)
		{
			g_Temp.m_nBcrPreInspFrame = nFrameNum;
			g_Temp.m_strPreBcrName = g_Temp.m_strBcrName;

			g_Temp.m_nBcrReadOK++;
		}

		if (g_Temp.m_isBcrFirstCode == true ||
			(g_Temp.m_bBcrForceInsert == true && g_Temp.m_isBcrForceReading == true) ||
			(g_Temp.m_nBcrDir != 0))
		{
			// 전공정 결점 데이터 검색해야함
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
				g_Temp.m_nBcrNoReadWarning = 0;
				g_Temp.m_nBcrNoReadError = 0;

			}
			else
			{
				double calcFrameLeng = 0;

				if (g_Param.m_dBcrScaleFactorY > BCR_SCALE_MAX)
					g_Param.m_dBcrScaleFactorY = BCR_SCALE_REF;

				// 미터로 계산
				calcFrameLeng = (g_Param.m_dBcrScaleFactorY * height) / 1000.0;
				g_Temp.m_nBcrNoReadWarning += calcFrameLeng;
				g_Temp.m_nBcrNoReadError += calcFrameLeng;

				if (g_Temp.m_isBcrFirstCode == true)
				{
					strLog.Format(_T("ERROR Meter : %.3fM, Warning Meter : %.3fM"),
						g_Temp.m_nBcrNoReadWarning, g_Temp.m_nBcrNoReadError);
					WriteLog(strLog);
				}

				if (g_Temp.m_isBcrFirstCode == true && g_Temp.m_strBcrName.GetLength() > 9)
				{
					if ((int)g_Temp.m_nBcrNoReadError > g_Param.m_nBCRErrorM)
					{
						g_Temp.m_nBcrNoReadError = 0;
						l_Send_Server.SendCommand_LocalHost(NM_BCR_BCD_READING_ERROR);
						strLog.Format(_T("[Error] No Bcr Read Length: %.3fM [Error Std Meter:%d]"), g_Temp.m_nBcrNoReadError, g_Param.m_nBCRErrorM);
						WriteLog(strLog);
					}
					else if ((int)g_Temp.m_nBcrNoReadWarning > g_Param.m_nBCRWarningM)
					{
						g_Temp.m_nBcrNoReadWarning = 0;
						l_Send_Server.SendCommand_LocalHost(NM_BCR_BCD_READING_LOW);
						strLog.Format(_T("[Warning] No Bcr Read Length: %.3fM [Warning Std Meter:%d]"), g_Temp.m_nBcrNoReadError, g_Param.m_nBCRWarningM);
						WriteLog(strLog);
					}
				}

				// BCR 인식률 전송
				if (g_Temp.m_nInspectFrame > 0 && g_Temp.m_nInspectFrame%100==0)
				{
					double dReadingRate;

					dReadingRate = (int)((double)g_Temp.m_nBcrReadOK * 100.0 / (g_Temp.m_nInspectFrame * g_Param.m_dFrameLen * 0.001));

					CPacket* packet = new CPacket;
					packet->MakeReturnPacket(NM_BCR_BCD_READING_RATE, 1);
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

	if (g_Param.m_bBcrSaveImage==true && g_Temp.m_isBcrSuccessRead==true)
	{
		int stX, stY;
		stX = g_Temp.m_BcrRect.left + g_Temp.m_BcrRect.Width() / 2 - BAD_IMG_WIDTH / 2;
		stY = g_Temp.m_BcrRect.top + g_Temp.m_BcrRect.Height() / 2 - BAD_IMG_HEIGHT / 2;
		for (i = 0; i < BAD_IMG_HEIGHT; i++)
		{
			memcpy(g_Defect.m_pImage[0] + i * BAD_IMG_WIDTH, fm + (stY + i) * width + stX, sizeof(BYTE)* BAD_IMG_WIDTH);
		}

		double dBcrX = 0.f, dBcrY = 0.f;
		dBcrX = (g_Temp.m_BcrRectFine.left + g_Temp.m_BcrRectFine.Width()/2.0) * g_Param.m_dScaleFactorX;
		dBcrY = (g_Temp.m_BcrRectFine.top + g_Temp.m_BcrRectFine.Height()/2.0) * g_Param.m_dScaleFactorY;

		CString sNGImageName, sNGImageFullName;
		sNGImageName.Format(_T("MATCHED_[%05d]%.3f_%.3f.bmp"), nFrameNum, dBcrX, dBcrY);

		//불량이미지 저장하는 것 막음(테스트에서 저장하지 못하게), 현장적용에서는 DONT_SAVE_IMAGE 없앰.
#ifndef DONT_SAVE_IMAGE			
		if (g_Temp.m_dHDDspace == 0 || g_Temp.m_dHDDspace > 15.0)
		{
			sNGImageFullName.Format(_T("%s%s\\%s\\%s"), NG_IMAGE_PATH, g_Temp.m_slotName, g_Temp.m_sMyComName, sNGImageName);
			InputNGImage(g_Defect.m_pImage[0], sNGImageFullName);
		}
#endif
	}

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

	if (g_Param.m_bBcrSaveImage == true)
	{
		g_Temp.m_BcrSavingRect.SetRect(0, 0, 256, 256);

		CRect rectSave;
		rectSave.SetRect(0, 0, 0, 0);
		rectSave = rect;

		CPoint nCenter;
		nCenter = rect.CenterPoint();

		rectSave.top = nCenter.y - (g_Temp.m_BcrSavingRect.Height() / 2);
		rectSave.bottom = nCenter.y + (g_Temp.m_BcrSavingRect.Height() / 2);
		rectSave.left = nCenter.x - (g_Temp.m_BcrSavingRect.Width() / 2);
		rectSave.right = nCenter.x + (g_Temp.m_BcrSavingRect.Width() / 2);
		if (rectSave.top < 0)
		{
			rectSave.top = 0;
			rectSave.bottom = g_Temp.m_BcrSavingRect.Height();
		}
		if (rectSave.bottom > h)
		{
			rectSave.bottom = h;
			rectSave.top = rectSave.bottom - g_Temp.m_BcrSavingRect.Height();
		}
		if (rectSave.left < 0)
		{
			rectSave.left = 0;
			rectSave.right = g_Temp.m_BcrSavingRect.Width();
		}
		if (rectSave.right > w)
		{
			rectSave.right = w;
			rectSave.left = rectSave.right - g_Temp.m_BcrSavingRect.Width();
		}
		g_Temp.m_BcrSavingRect = rectSave;


	}
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
	CRect rt = CRect(0, 0, w, h);
	int ntmp = 0;


	//-----------------------------------------------------------------------------------------
	//필름이 나오는 부위에 따라 기준선을 찾는다. 오른쪽에 나오는 경우 오른쪽부터 기준선을 찾음

	if (g_Temp.m_nEdgeDir == 1)	//PVA가 영상에서 오른쪽에 있을 경우 
	{
		edgeX = g_Temp.m_nFoundEdge;

		if (edgeX > w - 1 || edgeX < 20)	edgeX = g_Temp.m_nBcrPreEdge;
		else								g_Temp.m_nBcrPreEdge = edgeX;

		rt.left = edgeX;

		if (ntmp == 0) ntmp = edgeX;
		else		edgeX = ntmp + g_Param.m_nEdgeOffset;
	}
	else
	{
		if (edgeX > w - 1 || edgeX < 20)	edgeX = g_Temp.m_nBcrPreEdge;
		else								g_Temp.m_nBcrPreEdge = edgeX;

		rt.right = edgeX;

		if (ntmp == 0) ntmp = edgeX;
		else		edgeX = ntmp + g_Param.m_nEdgeOffset;
	}

	g_Temp.m_nFoundEdge = edgeX;

	///////////////////////////////////////////////////////////////////////////////////////////////
	// 에지 기준으로 검출 영역 생성 - S
	if (g_Temp.m_nEdgeDir == 1)  // 에지 기준 오른쪽에 바코드가 있는 경우
	{
		rtBcrArea = CRect(edgeX + 2, 0, edgeX + g_Param.m_nBCRSearchPixel, h);
		rtBcrAreaCheck = CRect(edgeX - g_Param.m_nBCRSearchPixel, 0, edgeX - 2, h);
		g_Temp.m_BcrRect = CRect(edgeX + 2, 0, edgeX + g_Param.m_nBCRSearchPixel, h);


	}
	else	// 에지 기준 오른쪽에 바코드가 있는 경우
	{
		rtBcrArea = CRect(edgeX - g_Param.m_nBCRSearchPixel, 0, edgeX - 2, h);
		rtBcrAreaCheck = CRect(edgeX + 2, 0, edgeX + g_Param.m_nBCRSearchPixel, h);
		g_Temp.m_BcrRect = CRect(edgeX - g_Param.m_nBCRSearchPixel, 0, edgeX - 2, h);
	}

	if (g_Param.m_nBCRManualArea)
	{
		rtBcrArea.left = rtBcrAreaCheck.left = g_Temp.m_BcrRect.left = g_Param.m_nBCRAreaL;
		rtBcrArea.right = rtBcrAreaCheck.right = g_Temp.m_BcrRect.right = g_Param.m_nBCRAreaR;
	}
	// 에지 기준으로 검출 영역 생성 - S
	///////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////
	//바코드 중심점 및 영역 만듬 - S   
	g_Temp.m_BcrRectFine = GetBcrFineArea(fm, left, top, w, h, pitch);
	CPoint BarCenter = GetBcrCenter(fm, rtBcrArea, w);

	BarCenter.x = (g_Temp.m_BcrRectFine.left + g_Temp.m_BcrRectFine.right) / 2;
	BarCenter.y = (g_Temp.m_BcrRectFine.top + g_Temp.m_BcrRectFine.bottom) / 2;
	g_Temp.m_BcrCenterPt = BarCenter;

	int bcrHalfW = g_Param.m_nBcrW / 2;
	int bcrHalfH = g_Param.m_nBcrH / 2;

	if (g_Temp.m_nEdgeDir == 0)  // 에지 왼쪽에 BCR 존재
		g_Temp.m_BcrRect.SetRect(BarCenter.x - (g_Param.m_nBcrOffset + bcrHalfW), g_Temp.m_BcrRectFine.top - bcrHalfH, edgeX - g_Param.m_nBcrOffset, g_Temp.m_BcrRectFine.bottom + bcrHalfH);
	else					  // 에지 오른쪽에 BCR 존재
		g_Temp.m_BcrRect.SetRect(edgeX + g_Param.m_nBcrOffset, g_Temp.m_BcrRectFine.top - bcrHalfH, BarCenter.x + (bcrHalfW + g_Param.m_nBcrOffset), g_Temp.m_BcrRectFine.bottom + bcrHalfH);
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

			for (int iter = 0; i < 4; i++)
			{
				Erode_Gray(fmErode, fmTmp, 0, 0, rectBcd.Width(), rectBcd.Height(), rectBcd.Width(), pInt, kSize, kSize);
				memcpy(fmErode, fmTmp, sizeof(BYTE) * rectBcd.Width() * rectBcd.Height());
			}

			for (int iter = 0; i < 4; i++)
			{
				Dilate_Gray(fmDilate, fmTmp, 0, 0, rectBcd.Width(), rectBcd.Height(), rectBcd.Width(), pInt, kSize, kSize);
				memcpy(fmDilate, fmTmp, sizeof(BYTE) * rectBcd.Width() * rectBcd.Height());
			}

			memset(fmTmp, 0xff, rectBcd.Width() * rectBcd.Height());

			int refTh = (int)dAvg - g_Param.m_nBcrDotTh;
			int refUpTh = (int)dAvg - g_Param.m_nBcrDotUpTh;

			for (i = 0; i < rectBcd.Height(); i++)
			{
				for (j = 0; j < rectBcd.Width(); j++)
				{
					if (*(fmErode + i * rectBcd.Width() + j) <= refTh)
						*(fmTmp + i * rectBcd.Width() + j) = 0;

					if (*(fmDilate + i * rectBcd.Width() + j) >= refUpTh)
						*(fmTmp + i * rectBcd.Width() + j) = 0;
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
					if (abs(g_Param.m_nBcrW - dWidth) <= g_Param.m_nBcrDiffW && abs(g_Param.m_nBcrH - dHeight) <= g_Param.m_nBcrDiffH)
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
								if (abs(g_Param.m_nBcrW - dWidth) <= g_Param.m_nBcrDiffW && abs(g_Param.m_nBcrH - dHeight) <= g_Param.m_nBcrDiffH)
								{
									if (dMaxMatchedW > 0 && dMaxMatchedH > 0)
									{
										if ((pow(dWidth - g_Param.m_nBcrW, 2) + pow(dHeight - g_Param.m_nBcrH, 2)) < (pow(dMaxMatchedW - g_Param.m_nBcrW, 2) + pow(dMaxMatchedH - g_Param.m_nBcrH, 2)))
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
										if (pow(dWidth - g_Param.m_nBcrW, 2) + pow(dHeight - g_Param.m_nBcrW, 2) < pow(tmpW - g_Param.m_nBcrW, 2) + pow(tmpH - g_Param.m_nBcrW, 2))
											rectMostMatched = rectBlob;
									}
								}
							}
						}
					}
				}
				if (nMaxMatchedIndex >= 0)
				{
					g_Temp.m_BcrRectFine = CRect(
						rectBcd.left + rectMatched.left,
						rectBcd.top + rectMatched.top,
						rectBcd.left + rectMatched.right,
						rectBcd.top + rectMatched.bottom);
				}
				else
				{
					g_Temp.m_BcrRectForMatch = CRect(0, 0, 0, 0);
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

	if (g_Param.m_nBCRManualArea)
	{
		g_Temp.m_BcrRect.left = g_Param.m_nBCRAreaL;
		g_Temp.m_BcrRect.right = g_Param.m_nBCRAreaR;
	}

	g_Temp.m_BcrRect.NormalizeRect();
}

BOOL CheckValidCode(CString str)
{
	BOOL bRet = TRUE;
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

			// 마지막 바코드 값 입력 -->이전 바코드 프레임, 이전 바코드 영상내 위치, 이전 바코드 ROLL상 위치(mm 단위)  
			m_nBcrPreYPos = g_Temp.m_BcrRectFine.top;
			g_Temp.m_nBcrPreInspFrame = crtFrameNum;
			g_Temp.m_dBcrPreRealPos = g_Temp.m_dBcrCrtRealPos;
			g_Temp.m_nBcrOrder = CheckBcrOrder(g_Temp.m_strBcrName, g_Temp.m_strPreBcrName);
			g_Temp.m_strPreBcrName = g_Temp.m_strBcrName;
		}
	}
	else
	{
		if (g_Temp.m_strPreBcrName.GetLength() > LOT_NAME_LENGTH)
			strBcrLotName = g_Temp.m_strPreBcrName.Left(LOT_NAME_LENGTH);
	}

	int nCrtMeter = 20;
	//-----------------------------------------------------------------------------
	//바코드 위치에서 위치 값 읽은 후 조건에 맞는 데이터 검색
	if (g_Temp.m_isBcrFirstCheck == false && (g_Temp.m_nBcrDir==-1 || g_Temp.m_nBcrDir == 1))
	{
		double bar_frame_length = 0; //바코드 상 거리로 인한 프레임 길이 값 
		bar_frame_length = g_Param.m_dBcrScaleFactorY * g_System.m_nImageH;

		int nDistMeter = g_Param.m_nMarkingDefectMarking;
		int nTenMeterFrame = (int)(1000.0 * nDistMeter / bar_frame_length); //몇Frame을 전의 Defect/frame 개수를 보낼지 Meter->Frame계산

		// 위치 정보 계산.
		double dNextFramePos, dCurFramePos;
		if (g_Temp.m_nBcrDir == 1)  //바코드 수가 증가할 경우 
		{
			dNextFramePos = g_Temp.m_dBcrCrtRealPos - g_Temp.m_dBcrOffsetY + (lastBcrFrameNum - g_Temp.m_nBcrPreInspFrame + 1) * bar_frame_length;
			dCurFramePos = g_Temp.m_dBcrCrtRealPos - g_Temp.m_dBcrOffsetY + (lastBcrFrameNum - g_Temp.m_nBcrPreInspFrame) * bar_frame_length;
		}
		else	//감소
		{
			dNextFramePos = g_Temp.m_dBcrCrtRealPos + g_Temp.m_dBcrOffsetY - (lastBcrFrameNum - g_Temp.m_nBcrPreInspFrame + 1) * bar_frame_length;
			dCurFramePos = g_Temp.m_dBcrCrtRealPos + g_Temp.m_dBcrOffsetY - (lastBcrFrameNum - g_Temp.m_nBcrPreInspFrame) * bar_frame_length;
		}

		if (g_Temp.m_dBcrPreFramePos == 0)	//처음 초기값
			g_Temp.m_dBcrPreFramePos = dCurFramePos;


		
		int defectCnt;
		// 불량 데이터 검색
		if (g_Temp.m_nBcrDir == 1)  //바코드 수가 증가할 경우 
			((CKoWebView*)pParent)->m_DefectCallClass->GetMarkDefectData(strBcrLotName, dCurFramePos, dNextFramePos);
		else	//감소
			((CKoWebView*)pParent)->m_DefectCallClass->GetMarkDefectData(strBcrLotName, dNextFramePos, dCurFramePos);

		// Area Maring 데이터 검색
		((CKoWebView*)pParent)->m_DefectCallClass->GetMarkAreaDefectData(dCurFramePos, dNextFramePos);

		// Bcr 영역 Marking 처리
		if (g_Temp.m_isBcrSuccessRead)
		{
			
			g_Defect.m_BMarkDefect.x = (float)(g_Temp.m_BcrRectFine.left * g_Param.m_dScaleFactorX) + g_Param.m_dCamStartPosX;
			g_Defect.m_BMarkDefect.y = (float)(g_Temp.m_BcrRectFine.top * g_Param.m_dScaleFactorY) + g_Param.m_dCamStartPosY;
			g_Defect.m_BMarkDefect.width = g_Temp.m_BcrRectFine.Width() * g_Param.m_dScaleFactorX;
			g_Defect.m_BMarkDefect.height = g_Temp.m_BcrRectFine.Width() * g_Param.m_dScaleFactorX;
			g_Defect.m_BMarkDefect.type = csvType;
			g_Defect.m_BMarkDefect.position = (int)g_Temp.m_dBcrCrtRealPos;

			if (csvType == eCSV_TYPE_NITTO || csvType == eCSV_TYPE_NITTO_RK 
				|| csvType == eCSV_TYPE_NITTO_RTS || csvType == eCSV_TYPE_KORENO_RK_IJP)
				g_Defect.m_BMarkDefect.defect_class = 6;
			else
				g_Defect.m_BMarkDefect.defect_class = 9;
			g_Defect.m_BMarkDefect.mark = (int)g_Param.m_bBcrMark;
			strcpy_s(g_Defect.m_BMarkDefect.fileName, MAX_BADIMAGE_FILENAME, g_Temp.m_cBcrFileName);
			g_Defect.m_nBcrCount = 1;
		}
		
		//if (((CVisWebApp*)AfxGetApp())->m_DestConfig.GetAREADEL())
		//{
		//	double y_stt, y_end;
		//	//에어리어마킹
		//	for (int i = 0; i < m_nAreadelCnt; i++)
		//	{
		//		if ((m_STR_MD_AREADEL[i] <= dCurFramePos && dNextFramePos < m_END_MD_AREADEL[i]) ||
		//			(m_END_MD_AREADEL[i] <= dCurFramePos && dNextFramePos < m_STR_MD_AREADEL[i]) ||
		//			(dCurFramePos <= m_STR_MD_AREADEL[i] && m_STR_MD_AREADEL[i] < dNextFramePos) ||
		//			(dCurFramePos <= m_END_MD_AREADEL[i] && m_END_MD_AREADEL[i] < dNextFramePos))
		//		{
		//			//현재위치에 AreaDel마킹영역이 존재시
		//			if (pDoc->m_data.m_nOddEven == 1)
		//			{
		//				pDoc->m_data.m_AreaDelDefect[pDoc->m_data.m_nAreadelDefects].x_stt =
		//					m_STR_WD_AREADEL[i] + pDoc->m_data.m_nEdgePoint * pDoc->m_data.m_dScaleFactorX + pDoc->m_data.m_dCalibSttX;
		//				pDoc->m_data.m_AreaDelDefect[pDoc->m_data.m_nAreadelDefects].x_end =
		//					m_END_WD_AREADEL[i] + pDoc->m_data.m_nEdgePoint * pDoc->m_data.m_dScaleFactorX + pDoc->m_data.m_dCalibSttX;
		//			}
		//			else if (pDoc->m_data.m_nOddEven == 0)
		//			{
		//				pDoc->m_data.m_AreaDelDefect[pDoc->m_data.m_nAreadelDefects].x_stt =
		//					pDoc->m_data.m_nEdgePoint * pDoc->m_data.m_dScaleFactorX + pDoc->m_data.m_dCalibSttX - m_END_WD_AREADEL[i];
		//				pDoc->m_data.m_AreaDelDefect[pDoc->m_data.m_nAreadelDefects].x_end =
		//					pDoc->m_data.m_nEdgePoint * pDoc->m_data.m_dScaleFactorX + pDoc->m_data.m_dCalibSttX - m_STR_WD_AREADEL[i];
		//			}
		//			if (m_nBarDirection == 1)	//증가
		//			{
		//				y_stt = max(dCurFramePos, m_STR_MD_AREADEL[i]);
		//				y_end = min(dNextFramePos, m_END_MD_AREADEL[i]);
		//				pDoc->m_data.m_AreaDelDefect[pDoc->m_data.m_nAreadelDefects].y_stt = y_stt - dCurFramePos;	//1frame 기준으로 변환
		//				pDoc->m_data.m_AreaDelDefect[pDoc->m_data.m_nAreadelDefects].y_end = y_end - dCurFramePos;	//1frame 기준으로 변환
		//			}
		//			else	//감소
		//			{
		//				y_stt = min(dCurFramePos, m_END_MD_AREADEL[i]);
		//				y_end = max(dNextFramePos, m_STR_MD_AREADEL[i]);
		//				pDoc->m_data.m_AreaDelDefect[pDoc->m_data.m_nAreadelDefects].y_stt = dCurFramePos - y_stt;	//1frame 기준으로 변환
		//				pDoc->m_data.m_AreaDelDefect[pDoc->m_data.m_nAreadelDefects].y_end = dCurFramePos - y_end;	//1frame 기준으로 변환
		//			}

		//			str.Format("Areadel Mark Pos, %.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f", m_STR_WD_AREADEL[i], m_END_WD_AREADEL[i], y_stt, y_end,
		//				pDoc->m_data.m_AreaDelDefect[pDoc->m_data.m_nAreadelDefects].x_stt, pDoc->m_data.m_AreaDelDefect[pDoc->m_data.m_nAreadelDefects].x_end,
		//				pDoc->m_data.m_AreaDelDefect[pDoc->m_data.m_nAreadelDefects].y_stt, pDoc->m_data.m_AreaDelDefect[pDoc->m_data.m_nAreadelDefects].y_end);
		//			logData.WriteLog(str);

		//			pDoc->m_data.m_nAreadelDefects++;
		//		}
		//	}
		//}
	}
}