#include "stdafx.h"

OCRTEACH g_OCR[OCR_NUMBER];

TCHAR l_OCRCHAR[OCR_NUMBER]={'A', 'B','C', 'D','E', 'F','G', 'H','I', 'J', 
							 'K', 'L','M', 'N','O', 'P','Q', 'R','S', 'T',
							 'U', 'V','W', 'X','Y', 'Z','0', '1','2', '3',
							 '4', '5','6', '7','8', '9','-', '*'};

//무조건 제일먼저---
int InitOCRData()
{
	int ok=-1;
	int i,j;

	for(i=0;i<OCR_NUMBER;i++)
	{
		for(j=0;j<OCR_MAX_PATTERN;j++)
		{
			g_OCR[i].m_nRegistered[j]=0;
			g_OCR[i].m_fm[j]=new BYTE[OCR_MAX_WIDTH*OCR_MAX_HEIGHT];
			if(g_OCR[i].m_fm[j]==NULL) goto done;
		}
		g_OCR[i].m_nSmooth=1;
		g_OCR[i].m_nOverlap=1;
		g_OCR[i].m_nOverLapCount=0;
		g_OCR[i].m_nSizeX=30;
		g_OCR[i].m_nSizeY=60;
	}


	ok=0;

done:
	return ok;

}

void CloseOCRData()
{
	int i, j;
	for(i=0;i<OCR_NUMBER;i++)
	{
		for(j=0;j<OCR_MAX_PATTERN;j++)
		{
			if(g_OCR[i].m_fm[j]) delete g_OCR[i].m_fm[j];
		}
	}
}

int LoadOCRData(CString sFileName)
{
	int ok=-1;
	int i,j,ret;
	CFile f;

	ret=f.Open(sFileName, CFile::modeRead);
	if(ret)
	{
		CloseOCRData();
		f.Read(g_OCR, OCR_NUMBER*sizeof(OCRTEACH));

		for(i=0;i<OCR_NUMBER;i++)
			for(j=0;j<OCR_MAX_PATTERN;j++)
			{
				g_OCR[i].m_fm[j]=new BYTE[OCR_MAX_WIDTH*OCR_MAX_HEIGHT];
				f.Read(g_OCR[i].m_fm[j], OCR_MAX_WIDTH*OCR_MAX_HEIGHT);
			}

		f.Close();
	}


	ok=0;
	return ok;
}

int SaveOCRData(CString sFileName)
{
	int ok=-1;
	int i,j,ret;
	CFile f;

	ret=f.Open(sFileName, CFile::modeCreate|CFile::modeWrite);

	if(ret)
	{
		f.Write(g_OCR, OCR_NUMBER*sizeof(OCRTEACH));
		for(i=0;i<OCR_NUMBER;i++)
			for(j=0;j<OCR_MAX_PATTERN;j++)
				f.Write(g_OCR[i].m_fm[j], OCR_MAX_WIDTH*OCR_MAX_HEIGHT);

		f.Close();
	}


	ok=0;
	return ok;
}

int FindOCR(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int *pScore, int *pOCR, int nPosition, int *pX, int *pY)
{
	int i, j, N;
	double dX, dY, dScore;
	double start, end;
	int nScore, nMaxScore, nPos, nMaxScoreAll=0;
	int nScoreArray[OCR_NUMBER]={0,};
	int ret;
	CString str;
	pScore[0]=pScore[1]=pScore[2]=pScore[3]=pScore[4]=0;
	*pX= *pY=0;

	m_KOMatch.SetMatchingData(0.5, 0, 0, 1, 0.9);
	for(i=0;i<OCR_NUMBER;i++)
	{
		N=i;
		nMaxScore=0;
//		for(j=0;j<OCR_MAX_PATTERN;j++)
		for(j=0;j<3;j++)
		{
			if(g_OCR[N].m_nRegistered[j])
			{
				m_KOMatch.TeachTemplate(g_OCR[N].m_fm[j], 0, 0, g_OCR[N].m_nSizeX-2,g_OCR[N].m_nSizeY-2, OCR_MAX_WIDTH);
				ret=m_KOMatch.Matching(fm,left, top, right, bottom, pitch);

				nScore=100.0*m_KOMatch.GetValue();
				if(nScore>nMaxScore) nMaxScore=nScore;

				//보통때는 사용하지 않으나 수동 Search때만 사용함.
				if(nPosition)
				{
					if(nScore>nMaxScoreAll)
					{
						nMaxScoreAll=nScore;
						*pX=m_KOMatch.GetX();
						*pY=m_KOMatch.GetY();
					}
				}
			}
		}
		nScoreArray[i]=nMaxScore;
	}

	for(N=0;N<5;N++)
	{
		nMaxScore=0;
		nPos=-1;
		for(i=0;i<OCR_NUMBER;i++)
		{
			if(nScoreArray[i]>nMaxScore)
			{
				nMaxScore=nScoreArray[i];
				nPos=i;
			}
		}
		if(nMaxScore>0 && nPos>=0)
		{
			pScore[N]=nMaxScore;
			pOCR[N]=nPos;
			nScoreArray[nPos]=0;
		}
	}

	if(pScore[0]>=50) return 1;
	else              return 0;
}