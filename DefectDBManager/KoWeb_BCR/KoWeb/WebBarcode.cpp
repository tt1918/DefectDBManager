#include "Stdafx.h"

#include <memory.h>
#include <emmintrin.h>
#include <smmintrin.h>
#include <dvec.h>
#include <math.h>

#include "KoWebDoc.h"
#include "KoWebView.h"

//BARCODE_VISION

BYTE l_fmBCRBK[512 * 128];
BYTE l_fmBCR[512 * 2178];

int GetBCRData(LPBYTE fm, int left, int top, int w, int h, int pitch, int* pX, int* pY, TCHAR sBcr[][30]);
double Distance2NearRect(CRect r1, CRect r2);
CRect GetBcrFineArea(LPBYTE fm, int left, int top, int w, int h, int pitch);
CPoint GetBcrCenter(unsigned char* fm, CRect rt, int nPitch);
CRect GetBarcodeRect2(LPBYTE fm, int w, int h, int pitch, int ndirect);
void GetBcrPosition(LPBYTE fm, int left, int top, int w, int h, int pitch);

void WEB_Barcode()
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

	int nFrameNum = g_Temp.m_nGrabFrame;

	g_Temp.m_nMaxGray = 0;
	g_Temp.m_nMinGray = 255;

	if (g_Param.m_nNotInspArea == 0) return;

	if (g_Temp.m_nEdgeDir == 0) nX = g_Temp.m_nInspectX1;
	else                     nX = g_Temp.m_nInspectX2 - nBcrPitch;

	g_Temp.m_isBcrSuccessRead = false;
	g_Temp.m_nBcrPatFind = 0;
	// 결과 데이터는 전부 g_Temp 데이터에 저장됨.
	GetBcrPosition(fm, 0, 0, width, height, pitch);
	// 기존 코드
	bool isBcrExsit = true;
	CString strBcrMsg = _T("none");
	CString strReadMsg = _T("");
	CString strMsg;
	if (g_Param.m_nBcrDotTh > 0 && g_Param.m_nBcrDotUpTh > 0)
	{
		if (g_Temp.m_BcrRectForMatch.Width() == 0 || g_Temp.m_BcrRectForMatch.Height() == 0)
			isBcrExsit = false;

		if (g_Param.m_nNotInspArea != width)
		{
			if (g_Temp.m_BcrRect.left < 0)		g_Temp.m_BcrRect.left = 0;
			if (g_Temp.m_BcrRect.right < 0)		g_Temp.m_BcrRect.right = 0;
			if (g_Temp.m_BcrRect.top < 0)		g_Temp.m_BcrRect.top = 0;
			if (g_Temp.m_BcrRect.bottom < 0)	g_Temp.m_BcrRect.bottom = 0;
			if (g_Temp.m_BcrRect.left > width)	g_Temp.m_BcrRect.left = width;
			if (g_Temp.m_BcrRect.right > width)	g_Temp.m_BcrRect.right = width;
			if (g_Temp.m_BcrRect.top > height)	g_Temp.m_BcrRect.top = height;
			if (g_Temp.m_BcrRect.bottom > height) g_Temp.m_BcrRect.bottom = height;

			CRect tmpRect = g_Temp.m_BcrRect;

			if (isBcrExsit == true)
			{
				// 검출 처리
				//strReadMsg = ;

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

							// 강제 알람....
							//m_SendtoServerSock.SendCommand_LocalHost(NM_FORCE_BCR_NOT_EQAUL_OCR_ALRAM);
							/*CString str;
							str.Format("[FORCE_BCR] Not Eqaul BCR & OCR :%s,%s", strForcedBcr, strTemp);
							WriteLog(str);*/
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
			const int nPreFrm = g_Temp.m_nPreBcrInspFrame;
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
				(g_Param.m_useBcrMatSize == true && g_Temp.m_BcrFineRect.Width() > 0))
			{
				// 실제 BCR Length 처리 추가해야함.
				const double dBarcode_period_frame = 4.5;// (float)(1000 / (pDoc->m_data.m_dBarcodeScaleY * pDoc->m_pImage->GetHeight()));
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
					const int nLastBcrFrame = g_Temp.m_nPreBcrInspFrame;
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

							/*strT.Format("[FORCE_BCR]강제 BCR 입력 후 계산 nBcd : %d / BCNO : %s / gFrame : %d , LastFrame : %d fBpF:%.6f / ",
								nBcd, strNewBcno, m_nFrameNum, nLastBcdFrame, dBarcode_period_frame);
							WriteLog(strT);*/
						}
						else // 증가
						{
							int nBcd = int((nFrameNum - nLastBcrFrame) / dBarcode_period_frame);
							strNewBCNO.Format(_T("%s%06d"), strMsg, ntmp + nBcd);

							/*strT.Format("[FORCE_BCR]강제 BCR 입력 후 계산 nBcd : %d / BCNO : %s / gFrame : %d , LastFrame : %d fBpF:%.6f / ",
								nBcd, strNewBcno, m_nFrameNum, nLastBcdFrame, dBarcode_period_frame);
							WriteLog(strT);*/
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
							str.Format(L"[FORCE_BCR]패턴매칭후 값이 없거나 no_barcode 발생 :%s", strNewBCNO);
							WriteLog(str);
						}
						else
						{
							g_Temp.m_isBcrForceReading = true;
							g_Temp.m_dBCRForceREadingDist = 0.f;
							g_Temp.m_strBcrForceData = strNewBCNO;
							CString str;
							str.Format(L"[FORCE_BCR]패턴매칭후 값 강제 셋팅 :%s", strNewBCNO);
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
						str.Format(L"This message should not be logged %d,%d", nFrameNum, g_Temp.m_nPreBcrInspFrame);
						WriteLog(str);
					}
				}
			}

			if (g_Temp.m_isBcrSuccessRead == true)
			{
				g_Temp.m_nPreBcrInspFrame = nFrameNum;
				g_Temp.m_strPreBcrName  = g_Temp.m_strBcrName;

				g_Temp.m_nBcrReadOK++;
			}

			if (g_Temp.m_isBcrFirstCode == true ||
				(g_Temp.m_bBcrForceInsert == true && g_Temp.m_isBcrForceReading == true) ||
				(g_Temp.m_nBcrDir != 0))
			{
				// 전공정 데이터 검색해야함
			}

			if (g_Param.m_nBcrType == eCSV_TYPE_KOTECH || g_Param.m_nBcrType == eCSV_TYPE_KORENO || g_Param.m_nBcrType == eCSV_TYPE_KORENO_RK)
			{
				if (g_Temp.m_nBcrFirstRead == 2)
				{

				}
			}

			if (g_Param.m_nBcrType == eCSV_TYPE_NITTO_RK || g_Param.m_nBcrType == eCSV_TYPE_NITTO_RTS || 
				g_Param.m_nBcrType == eCSV_TYPE_KORENO_RK || g_Param.m_nBcrType == eCSV_TYPE_KORENO_RK_IJP)
			{
				if (g_Temp.m_isBcrSuccessRead == true)
				{
					g_Temp.m_nBcrNoReadWarning = 0;
					g_Temp.m_nBcrNoReadError = 0;

				}
				else
				{

				}
			}

		}
	}

	if (strBcrMsg != L"none")
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

	if (g_Param.m_bBCRSaveImage)
	{
		/*CNeImage image;
		if (image.Create(rectBcd.Width(), rectBcd.Height(), 8))
		{
			unsigned char* ptDest = (unsigned char*)image.GetImagePtr();

			for (i = 0; i < rectBcd.Height(); i++)
				for (j = 0; j < rectBcd.Width(); j++)
					*(ptDest + i * rectBcd.Width() + j) = *(fmTmp + i * rectBcd.Width() + j);

			double dBcrX = 0.f, dBcrY = 0.f;
			dBcrX = rectMatched.Width() * g_Param.m_dScaleFactorX;
			dBcrY = rectMatched.Height() * g_Param.m_dScaleFactorY;

			CString str;
			str.Format(L"%s%s_%s_MATCHED_[%05d]%.3f_%.3f.bmp", "C:\\COSS\\Image\\",
				pDoc->m_str_lot,
				pData->m_strComName,
				pView->m_nFrameNum,
				dBcrX,
				dBcrY);
			image.Save(str);
		}*/
	}

}

//최대 10개이나 현실적으로 3개만, 상, 하 붙은것은 제외.
//바코드 찾을때 필요한 파라미터 알려줘요
int GetBCRData(LPBYTE fm, int left, int top, int w, int h, int pitch, int* pX, int* pY, TCHAR sBcr[][30])
{
	int nBCRCount = 0;




	return nBCRCount;
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

	if (g_Param.m_bBCRSaveImage == true)
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

	g_Temp.m_BcrFineRect = GetBcrFineArea(fm, left, top, w, h, pitch);
	CPoint BarCenter = GetBcrCenter(fm, rtBcrArea, w);

	BarCenter.x = (g_Temp.m_BcrFineRect.left + g_Temp.m_BcrFineRect.right) / 2;
	BarCenter.y = (g_Temp.m_BcrFineRect.top + g_Temp.m_BcrFineRect.bottom) / 2;
	g_Temp.m_BcrCenterPt = BarCenter;

	int bcrHalfW = g_Param.m_nBcrW / 2;
	int bcrHalfH = g_Param.m_nBcrH / 2;

	if (g_Temp.m_nEdgeDir == 0)  // 에지 왼쪽에 BCR 존재
		g_Temp.m_BcrRect.SetRect(BarCenter.x - (g_Param.m_nBcrOffset + bcrHalfW), g_Temp.m_BcrFineRect.top - bcrHalfH, edgeX - g_Param.m_nBcrOffset, g_Temp.m_BcrFineRect.bottom + bcrHalfH);
	else					  // 에지 오른쪽에 BCR 존재
		g_Temp.m_BcrRect.SetRect(edgeX + g_Param.m_nBcrOffset, g_Temp.m_BcrFineRect.top - bcrHalfH, BarCenter.x + (bcrHalfW + g_Param.m_nBcrOffset), g_Temp.m_BcrFineRect.bottom + bcrHalfH);
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
					g_Temp.m_BcrFineRect = CRect(
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

