#include "stdafx.h"

#include "stdafx.h"
#include <immintrin.h>

#include "math.h"

#ifdef USE_DAKANO_LINE
#include <cstdint>

int  l_ProfileSum[MAX_WIDTH];
int l_nLineFactor[MAX_WIDTH];
BYTE  g_fmTempDakano[4096*2048];
BYTE  g_fmTempDakano1[4096*2048];
double l_LineUpValueMap[4096];
double l_LineDnValueMap[4096];


extern long l_LineProfile[MAX_WIDTH];
extern double l_LineUpValue[MAX_WIDTH];
extern double l_LineDnValue[MAX_WIDTH];
extern BYTE l_ProfileUPLimitValue[MAX_WIDTH];
extern BYTE l_ProfileDnLimitValue[MAX_WIDTH];
extern BYTE g_LineExistUp[MAX_WIDTH/32][10];
extern BYTE g_LineExistDn[MAX_WIDTH/32][10];
extern int g_nLineCount;


void CopyDKLine(int nSrcLine,int nTargetLine, int pitch);
void WEB_FindTACLine_Level_NEW()
{
	int nKipoMarking=0;
	int LINE_LIMIT=5;
	int i,j, k, loop, nY=0;
	CRect rect;            // = pData->m_rectIns;
//	unsigned char* fm = g_fmOriginal;//pImage->GetImagePtr();	
	int nLevelInapect=0, nKipoMinValue=9999;
	long nSumAll=0, nSum;
	int nOffset=2;
	int ii,jj,ll,tt,rr,bb, nYY, nYi, nXi;
	int nX1, nX2, nPitchDest;
	int pitch=g_System.m_nPitch;
#if DAKANO_WIDTH==64
	int pitchMap=4096;
#else
	int pitchMap=8192;
#endif
	int width=4096;
	int height=2048;
	long d1, d2, dc;
	double dTmp, dSize;
	int nMinLine=1;
	int nDiv=2048/2;
	int nMinUp=10000, nMinDn=100000;
	double dMaxValue, dMinValue;
	int  nMaxPos, nMinPos, nMaxYPos, nMinYPos;
	int nLevelInspect=0;
	int nLevel, nLevel_, nLevelW, nClass;
	int nExistUp=0, nExistDn=0;
	int bNewLineUp=1, bNewLineDn=1;
	int nS1, nS2, nRow, nCol, nTmp;
	int nValueUp, nPosXUp, nValueDn, nPosXDn;
	int nValueUpRel, nPosXUpRel, nValueDnRel, nPosDnRel;
	BYTE fmMapUp[128];
	BYTE fmMapDn[128];

	LPBYTE fm;

	LPBYTE fmT, fmDest;
	
	if(g_System.m_nImageW>4096)
	{
		fm=g_fmGrabComp1;
		rect.top	=0;
		rect.bottom	=2048;
		rect.left	=g_Temp.m_nInspectX1/2;
		rect.right	=g_Temp.m_nInspectX2/2;
		pitch       =pitch/2;



		if(g_Param.m_nBaseLine)
		{
			if(g_Temp.m_nSetBaseLine)
			{
				memset(l_LineProfile, 0, width*sizeof(long));
				memset(l_ProfileSum, 0, width*sizeof(int));
				
				for(i=0;i<2048;i++)
				{
					for(j=0;j<pitch;j++)
						l_LineProfile[j]+=*(fm+pitch*i+j);
				}
				for(j=0;j<pitch;j++)
					g_BaseLine[j]=l_LineProfile[j]/2048;

				//Profile +/- 5 Cut--------------------------------------------------------------------------
				for(i=0;i<2048;i++)
				{
					for(j=0;j<pitch;j++)
					{
						if(*(fm+pitch*i+j)>g_BaseLine[j]+5)		 l_ProfileSum[j]+=(g_BaseLine[j]+5);
						else if(*(fm+pitch*i+j)<g_BaseLine[j]-5) l_ProfileSum[j]+=(g_BaseLine[j]-5);
						else									 l_ProfileSum[j]+=*(fm+pitch*i+j);
					}
				}
				for(j=0;j<pitch;j++)
					g_BaseLine[j]=l_ProfileSum[j]/2048;
				//-------------------------------------------------------------------------------------------

				g_Temp.m_nSetBaseLine=0;
				g_Temp.m_nReadyBaseLine=1;
				FuncBaseLineSave(_T("C:\\COSS\\BaseLine.dat"));
			}
			else
			{
				if(g_Temp.m_nReadyBaseLine)
				{
					//g_fmTemp1
					for(i=0;i<2048;i++)
					{
						for(j=0;j<pitch;j++)
						{
							nTmp=128+*(fm+pitch*i+j)-g_BaseLine[j];
							if(nTmp<0) nTmp=0;
							else if(nTmp>255) nTmp=255;
							*(g_fmTemp1+pitch*i+j)=nTmp;
						}
					}
					memcpy(g_fmGrabComp1, g_fmTemp1, 4096*2048);
				}
			}
		}
		else
		{
			g_Temp.m_nSetBaseLine=0;
		}
	}
	else
	{
		fm=g_fmGrab[g_ID];
		rect.top	=0;
		rect.bottom	=g_System.m_nImageH;
		rect.left	=g_Temp.m_nInspectX1;
		rect.right	=g_Temp.m_nInspectX2;
	}

	nX1=rect.left;
	nX2=rect.right;

	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i] && (g_Param.m_nLineUpValue[i] || g_Param.m_nLineDnValue[i])) nLevelInspect=1;
	}

	if(nLevelInspect==0) return;

	if(nX2-nX1<256) return;
//강조필터사용
	memset(l_ProfileSum, 0, nX2*sizeof(int));
	if(1)
	{
		nSum=0;
		for(i=rect.top;i<rect.bottom;i+=2)nSum++;

		for(i=rect.top;i<rect.bottom;i+=2)
			for(j=nX1;j<nX2;j++)
				*(l_ProfileSum+j)+=*(fm+pitch*i+j);
			
		for(j=nX1;j<nX2;j++)
		{
			*(l_ProfileSum+j)/=nSum;
			if(*(l_ProfileSum+j)) l_nLineFactor[j]=1000*g_System.m_nGrabBright/(*(l_ProfileSum+j));
			else				  l_nLineFactor[j]=1000;
		}

		for(j=nX1;j<nX2;j++)
		{
			if(l_ProfileSum[j]+LINE_LIMIT>255)	l_ProfileUPLimitValue[j]=255;
			else								l_ProfileUPLimitValue[j]=l_ProfileSum[j]+LINE_LIMIT; 

			if(l_ProfileSum[j]-LINE_LIMIT<0)	l_ProfileDnLimitValue[j]=0;
			else								l_ProfileDnLimitValue[j]=l_ProfileSum[j]-LINE_LIMIT;
		}
		for(i=height/4-10;i<height*3/4+10;i++)
			for(j=nX1;j<nX2;j++)
			{
				if( *(fm+pitch*i+j)>l_ProfileUPLimitValue[j])		*(g_fmTempDakano+pitch*i+j)=l_ProfileUPLimitValue[j];
				else if(*(fm+pitch*i+j)<l_ProfileDnLimitValue[j])	*(g_fmTempDakano+pitch*i+j)=l_ProfileDnLimitValue[j];
				else												*(g_fmTempDakano+pitch*i+j)=*(fm+pitch*i+j);
			}
		ThreadEmphasize(g_fmTempDakano, g_fmTempDakano1, pitch,height/4-10,height*3/4+10, pitch, 60, 20, 3);

		nX1+=30;
		nX2-=30;
		fmT=g_fmTempDakano1;
//		memcpy(fm1+width*height/4,g_fmTemp1+width*height/4, height*width/2);
	}

//	nMinUp=g_ParamVer.param[2];
	nMinUp=200;
	g_Temp.m_nMaxLRValue=0;

		
	
	memset(l_LineProfile, 0, width*sizeof(long));
	memset(l_LineUpValue, 0, width*sizeof(double));
	memset(l_LineDnValue, 0, width*sizeof(double));

//DAKANO_MAP---------------------------------------------------------------------
	memset(l_LineUpValueMap, 0, width*sizeof(double));
	memset(l_LineDnValueMap, 0, width*sizeof(double));
//-------------------------------------------------------------------------------	

	for(i=height/4;i<height*3/4;i++)
		for(j=0;j<width;j++)
			*(l_LineProfile+j)+=*(fmT+pitch*i+j);


	for(k=10;k<40;k+=2)			//스지폭이 10~50 Pixel
	{
		d1=d2=dc=0;
		for(j=nX1;j<nX1+k;j++)
			d1+=*(l_LineProfile+j);
		for(j=nX1+1+k;j<nX1+1+2*k;j++)
			dc+=*(l_LineProfile+j);
		for(j=nX1+2+2*k;j<nX1+2+3*k;j++)
			d2+=*(l_LineProfile+j);

		nOffset=k+k/2+1;

		for(j=nX1+1;j<nX2-(3+3*k);j++)
		{
			dTmp=(double)(dc-(d1+d2)/2)/(k*nDiv)*l_nLineFactor[j+nOffset]/1000;
			if(dTmp>l_LineUpValue[j+nOffset])
			{
				if((dc-d1)*100/(k*nDiv)>=nMinUp && (dc-d2)*100/(k*nDiv)>=nMinUp)
					l_LineUpValue[j+nOffset]=dTmp;
			}
			if(dTmp<l_LineDnValue[j+nOffset])
			{
				if((d1-dc)*100/(k*nDiv)>=nMinUp && (d2-dc)*100/(k*nDiv)>=nMinUp)
					l_LineDnValue[j+nOffset]=dTmp;
			}

//DAKANO_MAP---------------------------------------------------------------------
			if(dTmp>l_LineUpValueMap[j+nOffset]) l_LineUpValueMap[j+nOffset]=dTmp;
			if(dTmp<l_LineDnValueMap[j+nOffset]) l_LineDnValueMap[j+nOffset]=dTmp;
//-------------------------------------------------------------------------------


			if(j==nX2-(3+3*k)-1) break;

			d1=d1-*(l_LineProfile+j-1)    +*(l_LineProfile+j-1+k);
			dc=dc-*(l_LineProfile+j+k)    +*(l_LineProfile+j+2*k);
			d2=d2-*(l_LineProfile+j+1+2*k)+*(l_LineProfile+j+1+3*k);
		}
	}

	////DAKANO_MAP---------------------------------
	if(g_Temp.m_nGrabFrame<64000)
	{
		nRow=g_Temp.m_nGrabFrame%1000;
		nCol=g_Temp.m_nGrabFrame/1000;
		for(loop=0;loop<DAKANO_WIDTH;loop++)
		{
#if DAKANO_WIDTH==64
			nS1=loop*64;
			nS2=(loop+1)*64;
#else
			nS1=loop*32;
			nS2=(loop+1)*32;
#endif
			dMaxValue=0;
			dMinValue=0;
			for(j=nS1;j<nS2;j++)
			{
				if(l_LineUpValueMap[j]>dMaxValue) dMaxValue=l_LineUpValueMap[j];
				if(l_LineDnValueMap[j]<dMinValue) dMinValue=l_LineDnValueMap[j];
			}
			
			nTmp=(int)(dMaxValue*10)-g_System.m_nDKOffsetW[loop];

			if(nTmp>255) nTmp=255;
			else if(nTmp<0) nTmp=0;
			*(g_fmMAP+nRow*pitchMap+nCol*DAKANO_WIDTH+loop)=nTmp;
			*(fmMapUp+loop)=nTmp;

			if(dMinValue>0)	nTmp=(int)(dMinValue*10);
			else            nTmp=(int)((-1)*dMinValue*10);
			nTmp=nTmp-g_System.m_nDKOffsetB[loop];

			if(nTmp>255) nTmp=255;
			else if(nTmp<0) nTmp=0;
			*(g_fmMAP+(nRow+1024)*pitchMap+nCol*DAKANO_WIDTH+loop)=nTmp;	
			*(fmMapDn+loop)=nTmp;

//			if(loop%10==0) *(g_fmMAP+nRow*pitch+nCol*128+loop)=*(fmMapUp+loop)=*(g_fmMAP+(nRow+1024)*pitch+nCol*128+loop)=*(fmMapDn+loop)=0;
		}

		//중간에 검은 선 생기지 않도록 수정함------------------------------------------------------------------
		for(loop=2;loop<DAKANO_WIDTH-2;loop++)
		{
			if(g_System.m_nDKOffsetW[loop]>3)
			{
				if(*(fmMapUp+loop) < (*(fmMapUp+loop-2)+*(fmMapUp+loop-1)+*(fmMapUp+loop+1)+*(fmMapUp+loop+2))/4)
				{
					*(fmMapUp+loop)= (*(fmMapUp+loop-2)+*(fmMapUp+loop-1)+*(fmMapUp+loop+1)+*(fmMapUp+loop+2))/4;
					*(g_fmMAP+nRow*pitchMap+nCol*DAKANO_WIDTH+loop)=*(fmMapUp+loop);
				}
			}
			if(g_System.m_nDKOffsetB[loop]>3)
			{
				if(*(fmMapDn+loop) < (*(fmMapDn+loop-2)+*(fmMapDn+loop-1)+*(fmMapDn+loop+1)+*(fmMapDn+loop+2))/4 )
				{
					*(fmMapUp+loop)= (*(fmMapDn+loop-2)+*(fmMapDn+loop-1)+*(fmMapDn+loop+1)+*(fmMapDn+loop+2))/4;
					*(g_fmMAP+(nRow+1024)*pitchMap+nCol*DAKANO_WIDTH+loop)=*(fmMapDn+loop);
				}
			}
		}
		//------------------------------------------------------------------------------------------------------


		if(g_Temp.m_nMapLineCnt!=(g_Temp.m_nGrabFrame-1))
		{
			if(abs(g_Temp.m_nGrabFrame-g_Temp.m_nMapLineCnt)<=5)
			{
				for(j=g_Temp.m_nMapLineCnt;j<g_Temp.m_nGrabFrame;j++)
					CopyDKLine(g_Temp.m_nGrabFrame, j, pitchMap);
			}
			g_Temp.m_nMapLineCntDiff=g_Temp.m_nGrabFrame-g_Temp.m_nMapLineCnt;
		}


		//실제 1 Frame검사하면 아래 함수를 Call한다.
		OneLineMapUpdate(0, g_Temp.m_nGrabFrame, fmMapUp, fmMapDn, g_fmMAP, &nValueUp, &nPosXUp, &nValueDn, &nPosXDn, &nValueUpRel, &nValueDnRel);	

		g_Temp.m_nMapLineCnt=g_Temp.m_nGrabFrame;
	}
	//-------------------------------------------------------

	for(loop=0;loop<1;loop++)
	{
		dMaxValue=0; nMaxPos=-1;
		dMinValue=100000; nMinPos=-1;
		for(j=nX1+10;j<nX2-10;j++)
		{
			if(l_LineUpValue[j]>dMaxValue)
			{
				dMaxValue=l_LineUpValue[j];
				nMaxPos=j;
			}
			if(l_LineDnValue[j]<dMinValue)
			{
				dMinValue=l_LineDnValue[j];
				nMinPos=j;
			}
		}

		nLevel=GetLevel(nValueUp, g_Param.m_nLineUpValue);		//절대비교
		nLevel_=GetLevel(nValueUpRel, g_Param.m_nLineDnValue);  //상대비교
		dSize=1;
		if(nLevel>0 && nLevel_>0)
		{
			if(nLevel_<nLevel)
			{
				nLevel=nLevel_;
				dSize=2;
			}
		}
		else if(nLevel_>0)
		{
			nLevel=nLevel_;
			dSize=2;
		}
						
		if(nLevel>0 && g_Defect.m_nDefectCount<g_System.m_nMaxDefect && abs(nPosXUp*64-nMaxPos)<=64)
		{
			nLevelW=nLevel;
			nMaxYPos=height/2;

			MakeDefectRect(g_Defect.m_nDefectCount, nMaxPos*2, nMaxYPos*4, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nMaxPos*2;							//불량위치 X (Pixel)
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nMaxYPos*4;							//불량위치 Y (Pixel)
			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+1;				//액정라인
			if(dSize<1.5)		g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nValueUp;	
			else				g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nValueUpRel;	
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size=g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX=g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY=dSize;
			

			//이미지--------------------------------------------------------
			fmDest	    = g_Defect.m_pImage[g_Defect.m_nDefectCount];
			nPitchDest	= BAD_IMG_HEIGHT;

			//Y범위
			tt=nMaxYPos-256;
			bb=tt+512;
			//X범위
			ll=nMaxPos-128; if(ll<rect.left) ll=rect.left;
			rr=ll+256;      
			if(rr>rect.right)
			{
				rr=rect.right;
				ll=rr-256;
				if(ll<rect.left) ll=rect.left;
			}

			for(ii=0;ii<BAD_IMG_HEIGHT;ii++)
			{
				nYi=tt+(bb-tt)*ii/BAD_IMG_HEIGHT;

				for(jj=0;jj<BAD_IMG_HEIGHT;jj++)
				{
					nXi=ll+(jj)*(rr-ll)/BAD_IMG_HEIGHT;
					*(fmDest+nPitchDest*ii+jj)=*(fm+pitch*nYi+nXi);
				}
			}
			//----------------------------------------------------------------

//			if(CheckDefectOverlap()==0)   							
				g_Defect.m_nDefectCount++;

			if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) break;

//------------------------------------------------------------------------------------------------------			
		}

		
		nLevel=GetLevel(nValueDn, g_Param.m_nLineUpValue);     //절대비교
		nLevel_=GetLevel(nValueDnRel, g_Param.m_nLineDnValue); //상대비교
		dSize=1;
		if(nLevel>0 && nLevel_>0)
		{
			if(nLevel_<nLevel)
			{
				nLevel=nLevel_;
				dSize=2;

			}
		}
		else if(nLevel_>0)
		{
			nLevel=nLevel_;
			dSize=2;
		}


		if(nLevel>0 && g_Defect.m_nDefectCount<g_System.m_nMaxDefect && abs(nPosXDn*32-nMinPos)<=64)
		{
			//같은자리 중복되면
			if(g_Defect.m_nDefectCount>0 && fabs((double)g_Defect.m_Defect[g_Defect.m_nDefectCount-1].x_pos-nMinPos)<100)
			{
				if(nLevelW>0 && nLevelW>nLevel) g_Defect.m_nDefectCount=0;		//백 Level이 약하면 백을 안쓰고 흑을 사용한다.

				if(g_Defect.m_nDefectCount>0 && nLevelW>0 && nLevelW==nLevel)	//백 흑 Level이 같으면 값이 큰 것을 사용한다.
				{			
					if(dSize<1.5)
					{
						if(g_Defect.m_Defect[g_Defect.m_nDefectCount-1].size<1.5)
						{
							if(nValueDn>nValueUp) g_Defect.m_nDefectCount=0;
							else				  nLevel=-1;
						}
						else  g_Defect.m_nDefectCount=0;						 
					}
					else
					{
						if(g_Defect.m_Defect[g_Defect.m_nDefectCount-1].size<1.5) nLevel=-1;
						else
						{
							if(nValueDnRel>nValueUpRel) g_Defect.m_nDefectCount=0;
							else						nLevel=-1;
						}
					}				
				}
			}

			if(nLevel>0)
			{
				nMaxYPos=height/2;

				MakeDefectRect(g_Defect.m_nDefectCount, nMinPos, nMaxYPos, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
				g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
				g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nMinPos;							//불량위치 X (Pixel)
				g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nMaxYPos;							//불량위치 Y (Pixel)
				g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+1;			
				if(dSize<1.5)		g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nValueDn;	
				else				g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =nValueDnRel;	
				g_Defect.m_Defect[g_Defect.m_nDefectCount].size=g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX=g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY=dSize;


				//이미지--------------------------------------------------------
				fmDest	    = g_Defect.m_pImage[g_Defect.m_nDefectCount];
				nPitchDest	= BAD_IMG_HEIGHT;

				//Y범위
				tt=nMaxYPos-256;
				bb=tt+512;
				//X범위
				ll=nMinPos-128; if(ll<rect.left) ll=rect.left;
				rr=ll+256;      
				if(rr>rect.right)
				{
					rr=rect.right;
					ll=rr-256;
					if(ll<rect.left) ll=rect.left;
				}

				for(ii=0;ii<BAD_IMG_HEIGHT;ii++)
				{
					nYi=tt+(bb-tt)*ii/BAD_IMG_HEIGHT;

					for(jj=0;jj<BAD_IMG_HEIGHT;jj++)
					{
						nXi=ll+(jj)*(rr-ll)/BAD_IMG_HEIGHT;
						*(fmDest+nPitchDest*ii+jj)=*(fm+pitch*nYi+nXi);
					}
				}
				//----------------------------------------------------------------
			
	//			if(CheckDefectOverlap()==0)   							
					g_Defect.m_nDefectCount++;

				if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) break;
			}
		}

		if(nMaxPos>0)
		{
			for(j=nMaxPos-100;j<nMaxPos+100;j++)
			{
				if(j<0 || j>=width) continue;
				l_LineUpValue[j]=0;
			}
		}
		if(nMinPos>0)
		{
			for(j=nMinPos-100;j<nMinPos+100;j++)
			{
				if(j<0 || j>=width) continue;
				l_LineDnValue[j]=0;
			}
		}
	}

//	if(nExistUp==0) CheckIsRealLineDakano(bNewLineUp, 0, 0, 0, 5); //흰선
//	if(nExistDn==0) CheckIsRealLineDakano(bNewLineDn, 1, 0, 0, 5); //검정선

	return;
}
void CopyDKLine(int nSrcLine, int nTargetLine, int pitch)
{
	int loop;
	int nRow, nCol;
	int nRowS, nColS;

	if(nSrcLine<10) return;

	nRowS=nSrcLine%1000;
	nColS=nSrcLine/1000;
	nRow=nTargetLine%1000;
	nCol=nTargetLine/1000;

	for(loop=0;loop<DAKANO_WIDTH;loop++)
	{			
		*(g_fmMAP+nRow*pitch+nCol*DAKANO_WIDTH+loop)=*(g_fmMAP+nRowS*pitch+nColS*DAKANO_WIDTH+loop);			
		*(g_fmMAP+(nRow+1024)*pitch+nCol*DAKANO_WIDTH+loop)=*(g_fmMAP+(nRowS+1024)*pitch+nColS*DAKANO_WIDTH+loop);				
	}
}




void FuncBaseLineSetting()
{
	g_Temp.m_nSetBaseLine=1;
	g_Temp.m_nReadyBaseLine=0;
}


int FuncBaseLineSave(CString sFileName)
{
	CFile f;
	int ret=0;
	
	ret=f.Open(sFileName, CFile::modeCreate|CFile::modeWrite);
	if(ret)
	{
		f.Write(g_BaseLine, 4096*sizeof(int));
		f.Close();
	}
	return ret;

}

int FuncBaseLineLoad(CString sFileName)
{
	CFile f;
	int ret=0;
	
	ret=f.Open(sFileName, CFile::modeRead);
	if(ret)
	{
		f.Read(g_BaseLine, 4096*sizeof(int));
		f.Close();
		g_Temp.m_nReadyBaseLine=1;
	}
	
	return ret;
}

/*
#define TD_LINE_MULTI	4	//Line값을 4배 증폭
#define TD_UP_LIMIT		6
#define TD_DN_LIMIT		6
#define TD_LINE_DATA	5
#define TD_LINE_OFFSET	15
double l_LineTDValue[32][2048];	//TD라인 검출에 사용함.  4096을 128씩 나눔=32
long    l_LineTD_UpLimit[2048];
long    l_LineTD_DnLimit[2048];

void WEB_FindTAC_TDLine_Level()
{
	int i,j, k, N;
	CRect rect;            // = pData->m_rectIns;
	int nLevelInapect=0;
	long nSumAll=0, nSum, nTmpT, nTmpB, nTmpDiff, nTmpDiffMax=0;
	int nPosX, nPosY;
	int ii,jj,ll,tt,rr,bb, nYY, nYi, nXi;
	int nX1, nX2, nPitchDest;
	int pitch=g_System.m_nPitch;
	int width=4096;
	int height=2048;

	int nLevelInspect=0;
	int nLevel;
	int nProfile[2048], nCount;


	LPBYTE fm=g_fmGrab[g_ID];

	LPBYTE fmT, fmDest;
	
	rect.top	=0;
	rect.bottom	=g_System.m_nImageH;
	rect.left	=g_Temp.m_nInspectX1;
	rect.right	=g_Temp.m_nInspectX2;

	nX1=rect.left;
	nX2=rect.right;

	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i] && g_Param.m_nLineTDValue[i]) nLevelInspect=1;
	}

	if(nLevelInspect==0) return;

	if(nX2-nX1<256) return;

	nCount=0;
	for(j=nX1;j<nX2;j+=10) nCount++;
	if(nCount==0) return;

	//수평줄의 Gray Value Limit건다.
	for(i=0;i<height;i++)
	{
		nSum=0;
		for(j=nX1;j<nX2;j+=10)
			nSum+=*(fm+pitch*i+j);

		l_LineTD_UpLimit[i]=nSum/nCount+TD_UP_LIMIT;
		l_LineTD_DnLimit[i]=nSum/nCount-TD_DN_LIMIT;
	}

	g_Temp.m_nMaxTDValue=0;  //m_nMaxTDPosX, m_nMaxTDPosY

	N=0;
	for(k=0;k<4096;k+=128, N++)
	{
		for(i=0;i<height;i++)
		{	
			nSum=0;
			for(j=k;j<k+128;j++)
			{
				if(*(fm+pitch*i+j)>l_LineTD_UpLimit[i])			nSum+=l_LineTD_UpLimit[i];
				else if(*(fm+pitch*i+j)<l_LineTD_DnLimit[i])	nSum+=l_LineTD_DnLimit[i];
				else											nSum+=*(fm+pitch*i+j);
			}
			l_LineTDValue[N][i]=nSum*TD_LINE_MULTI*100/128;
		}
	}

	for(N=0;N<32-8;N++)
	{
		if(N*128<nX1) continue;
		else if((N+8)*128>nX2) continue;

		for(i=0;i<height;i++)
		{
			nProfile[i]=0;
			for(j=N;j<N+8;j++)
				nProfile[i]+=l_LineTDValue[j][i];

			nProfile[i]/=8;
		}

		for(i=0;i<height-2*TD_LINE_DATA-TD_LINE_OFFSET;i++)
		{
			if(nProfile[i]==0) continue;

			nTmpT=0;
			for(j=i;j<i+TD_LINE_DATA;j++)
				nTmpT+=nProfile[j];

			nTmpB=0;
			for(j=i+TD_LINE_DATA+TD_LINE_OFFSET;j<i+TD_LINE_DATA*2+TD_LINE_OFFSET;j++)
				nTmpB+=nProfile[j];

			if(nTmpT>0 && nTmpB>0)
			{
				nTmpT/=TD_LINE_DATA;
				nTmpB/=TD_LINE_DATA;
				nTmpDiff=nTmpT-nTmpB; if(nTmpDiff<0) nTmpDiff*=-1;

				if(nTmpDiff>nTmpDiffMax)
				{
					nTmpDiffMax=nTmpDiff;
					nPosX=(N+4)*128;
					nPosY=i+TD_LINE_DATA+TD_LINE_OFFSET/2;	
					if(nPosY<64) nPosY=64;
					else if(nPosY>height-64) nPosY=height-64;
				}
			}
		}
	}

	nTmpDiffMax*=0.4;
	g_Temp.m_nMaxTDValue=nTmpDiffMax; 
	g_Temp.m_nMaxTDPosX=nPosX;
	g_Temp.m_nMaxTDPosY=nPosY;

	nLevel=GetLevel(nTmpDiffMax, g_Param.m_nLineTDValue);     //절대비교

	if(nLevel>0 && g_Defect.m_nDefectCount<g_System.m_nMaxDefect )
	{			
		MakeDefectRect(g_Defect.m_nDefectCount, nPosX, nPosY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);

		g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
		g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos		=nPosX;													//불량위치 X (Pixel)
		g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos		=nPosY;													//불량위치 Y (Pixel)
		g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class	=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+3;			//3: TD라인불량
		g_Defect.m_Defect[g_Defect.m_nDefectCount].value		=nTmpDiffMax;	
		g_Defect.m_Defect[g_Defect.m_nDefectCount].size=g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX=g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY=0;


		//이미지--------------------------------------------------------
		fmDest	    = g_Defect.m_pImage[g_Defect.m_nDefectCount];
		nPitchDest	= BAD_IMG_HEIGHT;

		//Y범위
		tt=nPosY-256; if(tt<0) tt=0;
		bb=tt+512;   
		if(bb>height)
		{
			bb=height;
			tt=bb-512;
		}
		//X범위
		ll=nPosX-512; if(ll<rect.left) ll=rect.left;
		rr=ll+1024;      
		if(rr>rect.right)
		{
			rr=rect.right;
			ll=rr-1024;
			if(ll<rect.left) ll=rect.left;
		}

		for(ii=0;ii<BAD_IMG_HEIGHT;ii++)
		{
			nYi=tt+(bb-tt)*ii/BAD_IMG_HEIGHT;

			for(jj=0;jj<BAD_IMG_HEIGHT;jj++)
			{
				nXi=ll+(jj)*(rr-ll)/BAD_IMG_HEIGHT;
				*(fmDest+nPitchDest*ii+jj)=*(fm+pitch*nYi+nXi);
			}
		}
		//----------------------------------------------------------------
			
		g_Defect.m_nDefectCount++;
	}

	return;
}
*/


#endif
