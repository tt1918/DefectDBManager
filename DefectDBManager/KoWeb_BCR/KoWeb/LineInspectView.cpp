#include "Stdafx.h"
#include "KoWebDoc.h"
#include "KoWebView.h"

#ifdef USE_DAKANO_LINE 

void FindDKLine(int N, int nData, LPBYTE fmMap, int nFrame, int nX1, int nX2, int *pValue, int *pValueRel, int *pPos);
void FindDKMaxProfile(int *pProfile, int  nX1, int nX2,  int *value, int *valueRel, int *pos);
int CheckJumpEdge(LPBYTE fm, int nFrame, int  nJumpEdgeValue);

int l_nOneLineSum[2];
int l_nOneLineCnt[2];
int l_nOneLineAvg[2];
int l_nOneLineMax[2];
int l_nOneLineMaxPos[2];
int l_nOneLineMin[2];
int l_nOneLineMinPos[2];
int l_nOneLineLowCnt[2];

int l_nDKMaxLineUp[2];
int l_nDKMaxLineUpPosX[2];
int l_nDKMaxLineUpPosY[2];
int l_nDKMaxLineUpRel[2];
int l_nDKMaxLineUpRelPosX[2];
int l_nDKMaxLineUpRelPosY[2];
int l_nDKMaxLineDn[2];
int l_nDKMaxLineDnPosX[2];
int l_nDKMaxLineDnPosY[2];
int l_nDKMaxLineDnRel[2];
int l_nDKMaxLineDnRelPosX[2];
int l_nDKMaxLineDnRelPosY[2];
int l_nTapePos[2];			//Tape이 있는위치
int l_nJumpEnd[2];
int l_nJumpEndFlag[2];

CString l_String[20];
int l_nStringCnt=0;

void DakanoDataReset()
{
	int i;
	for(i=0;i<2;i++)
	{
		l_nOneLineSum[i]=0;
		l_nOneLineCnt[i]=0;
		l_nOneLineAvg[i]=0;
		l_nOneLineMax[i]=0;
		l_nOneLineMaxPos[i]=0;
		l_nOneLineMin[i]=2560;
		l_nOneLineMinPos[i]=0;
		l_nOneLineLowCnt[i]=0;

		l_nDKMaxLineUp[i]=0;
		l_nDKMaxLineDn[i]=0;
		l_nDKMaxLineUpRel[i]=0;
		l_nDKMaxLineDnRel[i]=0;
		l_nTapePos[i]=0;
		l_nJumpEnd[i]=0;
		l_nJumpEndFlag[i]=0;
	}
	//MAP빠진것
	g_Temp.m_nMapLineCnt=0;
	g_Temp.m_nMapLineCntDiff=0;

	//Send빠진것
	g_Temp.m_nLRSendFrame=0;
	g_Temp.m_nLRSendFrameDiff=0;

	g_Temp.m_nPortOpenCount=0;
}
/*
void CKoWebView::OnBnClickedButtonInspect3()
{	
	int nFrame,  i,j;
	int nCol, nRow, nTmp, nYEnd;
	double dStart, dEnd;
	int nValueUp, nPosXUp, nValueDn, nPosXDn;
	int nValueUpRel, nPosXUpRel, nValueDnRel, nPosDnRel;
	CString str;
	BYTE fmMapUp[128];
	BYTE fmMapDn[128];

	UpdateData(true);

	//끝 찾기------------------------------------------------------------------	
	for(i=32000-1;i>=0;i--)
	{
		nCol=i/1000;
		nRow=i%1000;

		nTmp=0;
		for(j=0;j<128;j++)
			nTmp+=*(m_pImage->m_fmDisplay+nRow*g_System.m_nPitch+nCol*128+j);

		if(nTmp>0)
		{
			nYEnd=i;
			break;
		}
	}
	//--------------------------------------------------------------------------
	DakanoDataReset();

	if(m_AllLength==0 && g_Param.m_dFrameLen>0.01)
	{
		if(m_dYLength*1000/g_Param.m_dFrameLen<nYEnd) 
			nYEnd=(int)(1000*m_dYLength/g_Param.m_dFrameLen);
	}

	//한 Frame촬상마다 Call하는 영역---------------------------------------------
	dStart=Clock_set();
	for(nFrame=0;nFrame<nYEnd;nFrame++)
	{
		 nCol=nFrame/1000;
		 nRow=nFrame%1000;
		for(j=0;j<128;j++)
		{
			*(fmMapUp+j)=*(m_pImage->m_fmDisplay+nRow*g_System.m_nPitch+nCol*128+j);
			*(fmMapDn+j)=*(m_pImage->m_fmDisplay+g_System.m_nPitch*1024+nRow*g_System.m_nPitch+nCol*128+j);
		}

		//실제 1 Frame검사하면 아래 함수를 Call한다.
		OneLineMapUpdate(1, nFrame, fmMapUp, fmMapDn, m_pImage->m_fmDisplay, &nValueUp, &nPosXUp, &nValueDn, &nPosXDn, &nValueUpRel,&nValueDnRel);
	}
	//---------------------------------------------------------------------------

	//TEST 결과 보여줌 ----------------------------------------------------------
	m_nResultStringCnt=0;
	dEnd=Clock_set();
	str.Format(_T("Elapse Time(sec) :%.1lf"), (dEnd-dStart)/1000);
	m_sResult[m_nResultStringCnt++]=str;
	if(l_nStringCnt)
	{
		for(i=0;i<l_nStringCnt;i++)
			m_sResult[m_nResultStringCnt++]=l_String[i];
	}
	DrawResultViewText();
	//---------------------------------------------------------------------------
}
*/

#if DAKANO_WIDTH==64
	#define DK_DEVIDE	128
#else
	#define DK_DEVIDE	64
#endif
void OneLineMapUpdate(int N, int nFrame, LPBYTE fmUp, LPBYTE fmDn, LPBYTE fmMap, int *pValueUp, int *pPosXUp, int *pValueDn, int *pPosXDn, int *pValueUpRel, int *pValueDnRel)
{
	int i,j, k;
	int nX1=g_Temp.m_nInspectX1/DK_DEVIDE;
	int nX2=(g_Temp.m_nInspectX2+1)/DK_DEVIDE;
	int nCheckX1, nCheckX2, nCheckCnt;
	int nTmp=0;
	int nMax, nMin, nMaxXPos, nMinXPos;
	int nJumpEdgeValue=g_Param.m_nDKJumpEdgeValue;
	int nTapeValue    =g_Param.m_nDKTapeValue;
	int nInspLineCnt=g_Param.m_nDKLineCnt;
	int nLeftOff=g_System.m_nDKLeftOff;
	int nRightOff=g_System.m_nDKRightOff;
	int dX1, dX2;
	int nValueUp, nPosXUp,nValueDn, nPosXDn;
	int nValueUpRel, nPosXUpRel,nValueDnRel, nPosXDnRel;
	int nFlagJumpping=0, nCheckJumpEdge;
	int nColCnt;
#if DAKANO_WIDTH==64
	int pitchMap=4096;
#else
	int pitchMap=8192;
#endif
	CString str;
	double dX, dY;

	if(nX1>=nLeftOff)      nLeftOff=0;
	if(nX2<=127-nRightOff) nRightOff=0;

	nValueUp=nPosXUp=nValueDn=nPosXDn=0;
	nValueUpRel=nPosXUpRel=nValueDnRel=nPosXDnRel=0;
	*pValueUp=*pValueUpRel=*pPosXUp =*pValueDn=*pValueDnRel =*pPosXDn= 0;

	l_nStringCnt=0;

	if(nInspLineCnt<10) nInspLineCnt=10;

#if DAKANO_WIDTH==64
	if(nX1<0)nX1=0;
	if(nX2==0) nX2=64;
	nCheckX1=nX1; if(nCheckX1<14)	  nCheckX1=14;
	nCheckX2=nX2; if(nCheckX2>64-14) nCheckX2=64-14;
	nCheckCnt=nCheckX2-nCheckX1;
	if(nCheckCnt<1) return;
#else
if(nX1<0)nX1=0;
	if(nX2==0) nX2=128;
	nCheckX1=nX1; if(nCheckX1<14)	  nCheckX1=14;
	nCheckX2=nX2; if(nCheckX2>128-14) nCheckX2=128-14;
	nCheckCnt=nCheckX2-nCheckX1;
	if(nCheckCnt<1) return;
#endif

	nTmp=0;
	nMax=-1;
	nMin=-1;
	nColCnt=0;
	for(j=nCheckX1;j<nCheckX2;j++)
	{
		nTmp+=*(fmUp+j);
		if(*(fmUp+j)>nMax) {nMax=*(fmUp+j); nMaxXPos=j;}

		if(*(fmDn+j)>nMin) {nMin=*(fmDn+j); nMinXPos=j;}

		if(*(fmUp+j)>0) nColCnt++;
	}
	if(nColCnt) nTmp=10*nTmp/nColCnt;
	else
	{
		nTmp=0;
		j=nFrame/1000;
		i=nFrame%1000;
		for(k=0;k<64;k++)
			nTmp+=*(fmMap+pitchMap*i+DAKANO_WIDTH*j+k);
		if(nTmp==0) l_nOneLineLowCnt[N]++;
		nTmp=0;
	}	
		
	if(nTmp>0)
	{
		l_nOneLineSum[N]+=nTmp;
		l_nOneLineCnt[N]++;
		l_nOneLineAvg[N]=l_nOneLineSum[N]/l_nOneLineCnt[N];
		if(nTmp>l_nOneLineMax[N])
		{
			l_nOneLineMax[N]=nTmp;
			l_nOneLineMaxPos[N]=nFrame;
		}
		if(nTmp<l_nOneLineMin[N])
		{
			l_nOneLineMin[N]=nTmp;
			l_nOneLineMinPos[N]=nFrame;
		}

		//점핑구간 Check-------------------------------------------------------------
		//양끝 Edge의 3X3이 점핑Value보다 크면 점핑으로 판정함. 

	
		if(nTmp>=nTapeValue)
		{
			l_nTapePos[N]=nFrame;
			l_nJumpEnd[N]=nFrame;
			nFlagJumpping=1;
			l_nOneLineLowCnt[N]++;
		}
		else
		{
			nCheckJumpEdge=CheckJumpEdge(fmMap, nFrame, nJumpEdgeValue);
			if(nCheckJumpEdge)			
			{
				l_nJumpEndFlag[N]=0;
			}
			else
			{
				//점핑구간을 10 Frame벗어나면 Tape은 Reset
				l_nJumpEndFlag[N]++;
				if(l_nJumpEndFlag[N]>=10) l_nTapePos[N]=0;
			}

			//nCheckJumpEdge 1이고 한줄 점핑이 나왔으면 계속 점핑구간임.
			if(l_nTapePos[N]>0)
			{
				l_nJumpEnd[N]=nFrame;
				l_nOneLineLowCnt[N]++;
				nFlagJumpping=1;
			}
		}
		//---------------------------------------------------------------------------

		if(nFrame>=l_nJumpEnd[N]+nInspLineCnt) 
		{
			FindDKLine(N, nInspLineCnt, fmMap, nFrame, nX1+nLeftOff, nX2-nRightOff, &nValueUp, &nValueUpRel,  &nPosXUp);
			FindDKLine(N, nInspLineCnt, fmMap+1024*pitchMap, nFrame, nX1+nLeftOff, nX2-nRightOff, &nValueDn, &nValueDnRel, &nPosXDn);

			
			//전체 검사한 것 중에서 최대값 최소값 구함------------
			if(nFlagJumpping==0) //점핑이 아닐떄 계산.
			{				
				if(nValueUp>l_nDKMaxLineUp[N])
				{
					l_nDKMaxLineUp[N]=nValueUp;
					l_nDKMaxLineUpPosX[N]=nPosXUp;
					l_nDKMaxLineUpPosY[N]=nFrame;
				}
				if(nValueUpRel>l_nDKMaxLineUpRel[N])
				{
					l_nDKMaxLineUpRel[N]=nValueUpRel;
					l_nDKMaxLineUpRelPosX[N]=nPosXUp;
					l_nDKMaxLineUpRelPosY[N]=nFrame;
				}
				if(nValueDn>l_nDKMaxLineDn[N])
				{
					l_nDKMaxLineDn[N]=nValueDn;
					l_nDKMaxLineDnPosX[N]=nPosXDn;
					l_nDKMaxLineDnPosY[N]=nFrame;
				}		
				if(nValueDnRel>l_nDKMaxLineDnRel[N])
				{
					l_nDKMaxLineDnRel[N]=nValueDnRel;
					l_nDKMaxLineDnRelPosX[N]=nPosXDn;
					l_nDKMaxLineDnRelPosY[N]=nFrame;
				}
			}
			//----------------------------------------------------
		}
		
		//데이타표시 ----------------------------
		if(N==0)
		{
			dX1=g_Param.m_dCamStartPosX+nMaxXPos*DK_DEVIDE*g_Param.m_dScaleFactorX;
			dX2=g_Param.m_dCamStartPosX+nMinXPos*DK_DEVIDE*g_Param.m_dScaleFactorX;
			if(nFlagJumpping) str.Format(_T("1 Frame(점핑): Avg= %d  white=%d (%d mm)  black:%d (%d mm)  현재Y위치: %d M 셔터:%d"), nTmp, nMax*10, dX1, nMin*10, dX2, (int)(nFrame*g_Param.m_dFrameLen/1000), g_Temp.m_nExposureData);
			else              str.Format(_T("1 Frame: Avg= %d  white=%d (%d mm)  black:%d (%d mm) 현재Y위치: %d M 셔터:%d"), nTmp, nMax*10, dX1, nMin*10, dX2, (int)(nFrame*g_Param.m_dFrameLen/1000), g_Temp.m_nExposureData);
			l_String[l_nStringCnt++]=str;
		}

		str.Format(_T("  ( 전체Avg=%d   Avg최대값: %d (%dM)  Avg최소값:%d (%dM), 점핑라인:%d )"), l_nOneLineAvg[N], l_nOneLineMax[N], (int)(l_nOneLineMaxPos[N]*g_Param.m_dFrameLen/1000), l_nOneLineMin[N], (int)(l_nOneLineMinPos[N]*g_Param.m_dFrameLen/1000), l_nOneLineLowCnt[N]);
		l_String[l_nStringCnt++]=str;
//		str.Format(_T(""));
//		l_String[l_nStringCnt++]=str;
		if(N==0)
		{
			str.Format(_T("최근 (Line %d 기준):"), nInspLineCnt);
			l_String[l_nStringCnt++]=str;
			dX1=g_Param.m_dCamStartPosX+nPosXUp*DK_DEVIDE*g_Param.m_dScaleFactorX;
			dX2=g_Param.m_dCamStartPosX+nPosXDn*DK_DEVIDE*g_Param.m_dScaleFactorX;
			str.Format(_T("- 절대값: Line(백): %d (X: %dmm Y: %d M)  Line(흑):%d (X: %dmm Y: %d M)"), nValueUp,dX1, (int)(nFrame*g_Param.m_dFrameLen/1000), nValueDn, dX2, (int)(nFrame*g_Param.m_dFrameLen/1000));
			l_String[l_nStringCnt++]=str;
			dX1=g_Param.m_dCamStartPosX+nPosXUp*DK_DEVIDE*g_Param.m_dScaleFactorX;
			dX2=g_Param.m_dCamStartPosX+nPosXDn*DK_DEVIDE*g_Param.m_dScaleFactorX;
			str.Format(_T("- 상대값: Line(백): %d (X: %dmm Y: %d M)  Line(흑):%d (X: %dmm Y: %d M)"), nValueUpRel,dX1, (int)(nFrame*g_Param.m_dFrameLen/1000), nValueDnRel, dX2, (int)(nFrame*g_Param.m_dFrameLen/1000));
			l_String[l_nStringCnt++]=str;
		}
		str.Format(_T("Lot전체 최대값(Line %d 기준):"), nInspLineCnt);
		l_String[l_nStringCnt++]=str;

		dX1=g_Param.m_dCamStartPosX+l_nDKMaxLineUpPosX[N]*DK_DEVIDE*g_Param.m_dScaleFactorX;
		dX2=g_Param.m_dCamStartPosX+l_nDKMaxLineDnPosX[N]*DK_DEVIDE*g_Param.m_dScaleFactorX;
		str.Format(_T("- 절대값: Line(백): %d (X: %dmm Y: %d M)  Line(흑):%d (X: %dmm Y: %d M)"), l_nDKMaxLineUp[N],dX1, (int)(l_nDKMaxLineUpPosY[N]*g_Param.m_dFrameLen/1000), l_nDKMaxLineDn[N], dX2, (int)(l_nDKMaxLineDnPosY[N]*g_Param.m_dFrameLen/1000));
		l_String[l_nStringCnt++]=str;

		dX1=g_Param.m_dCamStartPosX+l_nDKMaxLineUpRelPosX[N]*DK_DEVIDE*g_Param.m_dScaleFactorX;
		dX2=g_Param.m_dCamStartPosX+l_nDKMaxLineDnRelPosX[N]*DK_DEVIDE*g_Param.m_dScaleFactorX;
		str.Format(_T("- 상대값: Line(백): %d (X: %dmm Y: %d M)  Line(흑):%d (X: %dmm Y: %d M)"), l_nDKMaxLineUpRel[N],dX1, (int)(l_nDKMaxLineUpRelPosY[N]*g_Param.m_dFrameLen/1000), l_nDKMaxLineDnRel[N], dX2, (int)(l_nDKMaxLineDnRelPosY[N]*g_Param.m_dFrameLen/1000));
		l_String[l_nStringCnt++]=str;

		if(g_Temp.m_nMaxTDValue>0 && g_Temp.m_dMuraMaxValue>0)
		{
			str.Format(_T("TD Max: %d  (%d, %d)  오염(백) Value : %d  (%d,%d)"), g_Temp.m_nMaxTDValue, g_Temp.m_nMaxTDPosX, g_Temp.m_nMaxTDPosY, (int)g_Temp.m_dMuraMaxValue, g_Temp.m_nMuraMaxPosX, g_Temp.m_nMuraMaxPosY);
			l_String[l_nStringCnt++]=str;
		}
		else if(g_Temp.m_nMaxTDValue>0)
		{
			str.Format(_T("TD Max: %d  (%d, %d)"), g_Temp.m_nMaxTDValue, g_Temp.m_nMaxTDPosX, g_Temp.m_nMaxTDPosY);
			l_String[l_nStringCnt++]=str;
		}
		else if(g_Temp.m_dMuraMaxValue>0)
		{
			str.Format(_T("오염(백) Value : %d  (%d,%d)"), (int)g_Temp.m_dMuraMaxValue, g_Temp.m_nMuraMaxPosX, g_Temp.m_nMuraMaxPosY);
			l_String[l_nStringCnt++]=str;
		}

		//점핑구간 사용안하게 세팅되었고, 점핑으로 간주되는 구간이면 구한결과는 0으로 return
		if(g_Param.m_nDKNoUseJumpping && nFlagJumpping)
		{
			nValueUp=nValueUpRel=nPosXUp=nValueDn=nPosXDn=0;
			*pValueUp=*pValueUpRel=*pPosXUp =*pValueDn=*pValueDnRel=*pPosXDn =0;
		}
		else
		{
			*pValueUp=nValueUp;
			*pValueUpRel=nValueUpRel;
			*pPosXUp =nPosXUp;
			*pValueDn=nValueDn;
			*pValueDnRel=nValueDnRel;
			*pPosXDn =nPosXDn;
		}
	}	
}


//return 1이면 점핑Edge임
int CheckJumpEdge(LPBYTE fm, int nFrame, int  nJumpEdgeValue)
{
	int i, j, k;
	int nTmp[4], nSum=0;
#if DAKANO_WIDTH==64
	int pitchMap=4096;
#else
	int pitchMap=8192;
#endif
	LPBYTE fmUp=fm;
	LPBYTE fmDn=fm+1024*pitchMap;

	
	if(nFrame<1 || nFrame>=64000-1)  return 0;

	j=nFrame/1000;
	i=nFrame%1000;
	if(i<2) i=2;

	nTmp[0]=*(fmUp+(i-2)*pitchMap+DAKANO_WIDTH*j+1)+*(fmUp+(i-2)*pitchMap+DAKANO_WIDTH*j+2)+
		 *(fmUp+(i-1)*pitchMap+DAKANO_WIDTH*j+1)+*(fmUp+(i-1)*pitchMap+DAKANO_WIDTH*j+2)+
		 *(fmUp+(i)*pitchMap+DAKANO_WIDTH*j+1)+*(fmUp+(i)*pitchMap+DAKANO_WIDTH*j+2);
	if(nTmp[0]*10/6>=nJumpEdgeValue) return 1;

	nTmp[1]=*(fmUp+(i-2)*pitchMap+DAKANO_WIDTH*j+DAKANO_WIDTH-3)+*(fmUp+(i-2)*pitchMap+DAKANO_WIDTH*j+DAKANO_WIDTH-2)+
		 *(fmUp+(i-1)*pitchMap+DAKANO_WIDTH*j+DAKANO_WIDTH-3)+*(fmUp+(i-1)*pitchMap+DAKANO_WIDTH*j+DAKANO_WIDTH-2)+
		 *(fmUp+(i)*pitchMap+DAKANO_WIDTH*j+DAKANO_WIDTH-3)+*(fmUp+(i)*pitchMap+DAKANO_WIDTH*j+DAKANO_WIDTH-2);
	if(nTmp[1]*10/6>=nJumpEdgeValue ) return 1;

	nTmp[2]=*(fmDn+(i-2)*pitchMap+DAKANO_WIDTH*j+1)+*(fmDn+(i-2)*pitchMap+DAKANO_WIDTH*j+2)+
		 *(fmDn+(i-1)*pitchMap+DAKANO_WIDTH*j+1)+*(fmDn+(i-1)*pitchMap+DAKANO_WIDTH*j+2)+
		 *(fmDn+(i)*pitchMap+DAKANO_WIDTH*j+1)+*(fmDn+(i)*pitchMap+DAKANO_WIDTH*j+2);
	if(nTmp[2]*10/6>=nJumpEdgeValue ) return 1;

	nTmp[3]=*(fmDn+(i-2)*pitchMap+DAKANO_WIDTH*j+DAKANO_WIDTH-3)+*(fmDn+(i-2)*pitchMap+DAKANO_WIDTH*j+DAKANO_WIDTH-2)+
		 *(fmDn+(i-1)*pitchMap+DAKANO_WIDTH*j+DAKANO_WIDTH-3)+*(fmDn+(i-1)*pitchMap+DAKANO_WIDTH*j+DAKANO_WIDTH-2)+
		 *(fmDn+(i)*pitchMap+DAKANO_WIDTH*j+DAKANO_WIDTH-3)+*(fmDn+(i)*pitchMap+DAKANO_WIDTH*j+DAKANO_WIDTH-2);
	if(nTmp[3]*10/6>=nJumpEdgeValue ) return 1;

/*
	nTmp=*(fmUp+(i-1)*pitch+128*j)+*(fmUp+(i-1)*pitch+128*j+1)+*(fmUp+(i-1)*pitch+128*j+2)+
		 *(fmUp+(i)*pitch+128*j)+*(fmUp+(i)*pitch+128*j+1)+*(fmUp+(i)*pitch+128*j+2)+
		 *(fmUp+(i+1)*pitch+128*j)+*(fmUp+(i+1)*pitch+128*j+1)+*(fmUp+(i+1)*pitch+128*j+2);
	if(nTmp*10/9>=nJumpEdgeValue) return 1;

	nTmp=*(fmUp+(i-1)*pitch+128*j+128-3)+*(fmUp+(i-1)*pitch+128*j+128-2)+*(fmUp+(i-1)*pitch+128*j+128-1)+
		 *(fmUp+(i)*pitch+128*j+128-3)+*(fmUp+(i)*pitch+128*j+128-2)+*(fmUp+(i)*pitch+128*j+128-1)+
		 *(fmUp+(i+1)*pitch+128*j+128-3)+*(fmUp+(i+1)*pitch+128*j+128-2)+*(fmUp+(i+1)*pitch+128*j+128-1);
	if(nTmp*10/9>=nJumpEdgeValue) return 1;

	nTmp=*(fmDn+(i-1)*pitch+128*j)+*(fmDn+(i-1)*pitch+128*j+1)+*(fmDn+(i-1)*pitch+128*j+2)+
		 *(fmDn+(i)*pitch+128*j)+*(fmDn+(i)*pitch+128*j+1)+*(fmDn+(i)*pitch+128*j+2)+
		 *(fmDn+(i+1)*pitch+128*j)+*(fmDn+(i+1)*pitch+128*j+1)+*(fmDn+(i+1)*pitch+128*j+2);
	if(nTmp*10/9>=nJumpEdgeValue) return 1;

	nTmp=*(fmDn+(i-1)*pitch+128*j+128-3)+*(fmDn+(i-1)*pitch+128*j+128-2)+*(fmDn+(i-1)*pitch+128*j+128-1)+
		 *(fmDn+(i)*pitch+128*j+128-3)+*(fmDn+(i)*pitch+128*j+128-2)+*(fmDn+(i)*pitch+128*j+128-1)+
		 *(fmDn+(i+1)*pitch+128*j+128-3)+*(fmDn+(i+1)*pitch+128*j+128-2)+*(fmDn+(i+1)*pitch+128*j+128-1);
	if(nTmp*10/9>=nJumpEdgeValue) return 1;
*/

	return 0;
}

BYTE l_fmDKMap[128*100];
BYTE l_fmDKMapDilate[128*100];
//MAP에서 최근 데이타 nData개를 읽어와서 Projection을 구해서 가장 큰 값을 찾는다.
//설정된 값에 따라 Slope 줘가며 Projection한다.
void FindDKLine(int N, int nData, LPBYTE fmMap, int nFrame, int nX1, int nX2, int *pValue, int *pValueRel, int *pPos)
{	
	int i, j, k;
	int nCol, nRow;
#if DAKANO_WIDTH==64
	int pitchMap=4096;
#else
	int pitchMap=8192;
#endif
	int nSlope=g_Param.m_nDKSlopePixel;
	int nProfile[128];
	int nX, nY;
	int value, valueRel, pos;
	LPBYTE fm=l_fmDKMap;
	if(nSlope<0) nSlope=0;

	*pValue=*pValueRel=*pPos=0;
	
	if(nData>100) nData=100;
	if(nFrame<nData) return;
	if(nX2-nX1<=0)   return;

	memset(l_fmDKMap, 0, 128*nData);
	nY=0;
	for(i=nFrame-nData+1;i<=nFrame;i++)
	{
		nCol=i/1000;
		nRow=i%1000;

		for(j=nX1;j<nX2;j++)
		{			
			*(fm+DAKANO_WIDTH*nY+j)=*(fmMap+nRow*pitchMap+nCol*DAKANO_WIDTH+j);
		}
		nY++;
	}

	if(N==0) //Real Time일때만 
	{
//		Dilate_Gray(fm, l_fmDKMapDilate, 0,0, 127, nData, 128,  NULL,  3,3);
//		fm=l_fmDKMapDilate;
	}

	//기울기 없이 수직 Projection --------------------------------
	memset(nProfile, 0, DAKANO_WIDTH*sizeof(int));
	for(i=0;i<nData;i++)
		for(j=nX1;j<nX2;j++)
			nProfile[j]+=*(fm+DAKANO_WIDTH*i+j);
	for(j=nX1;j<nX2;j++)
		nProfile[j]=nProfile[j]*10/nData;

	FindDKMaxProfile(nProfile, nX1, nX2, &value, &valueRel, &pos);
	if(value>*pValue)
	{
		*pValue=value;
		*pValueRel=valueRel;
		*pPos=pos;
	}
	//---------------------------------------------------
	
	//좌상에서 우하로 잇는 선 구함. ---------------------
	for(k=1;k<nSlope;k++)
	{
		memset(nProfile, 0, DAKANO_WIDTH*sizeof(int));
		for(i=0;i<nData;i++)
		{	
			nX=(int)((double)k*i/nData+0.5);
			for(j=nX1;j<nX2-k;j++)
			{
				nProfile[j]+=*(fm+DAKANO_WIDTH*i+j+nX);
			}
		}
		for(j=nX1;j<nX2-k;j++)
			nProfile[j]=nProfile[j]*10/nData;
		
		FindDKMaxProfile(nProfile, nX1, nX2-k, &value, &valueRel, &pos);
		if(value>*pValue)
		{
			*pValue=value;
			*pValueRel=valueRel;
			*pPos=pos+k;
		}
	}

	//우상에서 좌하로 잇는 선 구함.----------------------
	for(k=1;k<nSlope;k++)
	{
		memset(nProfile, 0, DAKANO_WIDTH*sizeof(int));
		for(i=0;i<nData;i++)
		{	
			nX=(int)((double)k*i/nData+0.5);
			for(j=nX1+k;j<nX2;j++)
			{
				nProfile[j]+=*(fm+DAKANO_WIDTH*i+j-nX);
			}
		}
		for(j=nX1+k;j<nX2;j++)
			nProfile[j]=nProfile[j]*10/nData;

		FindDKMaxProfile(nProfile, nX1+k, nX2, &value, &valueRel, &pos);
		if(value>*pValue)
		{
			*pValue=value;
			*pValueRel=valueRel;
			*pPos=pos-k;
		}
	}
	if(*pPos<0)		*pPos=0;
#if DAKANO_WIDTH==64
	if(*pPos>63)	*pPos=63;
#else
	if(*pPos>127)	*pPos=127;
#endif
}


void FindDKMaxProfile(int *pProfile, int  nX1, int nX2,  int *value, int *valueRel, int *pos)
{
	int i, nPos;
	int nSum=0;
	int nCount=0;
	int nAvg;
	int nNonCnt=0;
	*value=*valueRel=*pos=0;

	for(i=nX1;i<nX2;i++)
	{
		if(pProfile[i]>*value)
		{
			*value=pProfile[i];
			*pos=nPos=i;
		}
		if(pProfile[i]) nNonCnt++;
	}
	nNonCnt-=5;
	if(nNonCnt<0) nNonCnt=1;

	if(nNonCnt>=20)
	{
		for(i=nX1;i<nPos-2;i++)
		{
			nSum+=pProfile[i];
		}
		for(i=nPos+3;i<nX2;i++)
		{
			nSum+=pProfile[i];
		}
		
		nAvg=nSum/nNonCnt;			
		//if(nAvg<20) nAvg=20;
		*valueRel=*value-nAvg;		
	}
}


#endif