#include "Stdafx.h"
#include "KoWebDoc.h"
#include "KoWebView.h"



long l_LineProfile[MAX_WIDTH];
double l_LineUpValue[MAX_WIDTH];
double l_LineDnValue[MAX_WIDTH];
BYTE l_ProfileUPLimitValue[MAX_WIDTH];
BYTE l_ProfileDnLimitValue[MAX_WIDTH];

BYTE g_LineExistUp[MAX_WIDTH/32][10];
BYTE g_LineExistDn[MAX_WIDTH/32][10];
int g_nLineCount=0;

int CheckIsRealLine(int nNewFrame, int nType, int nXpos, int nLevel, int nMinCount);

//중간 1/2 영역(Y)만으로 라인 찾음.(연속되는 라인)
//Value는 밝기대비 4배x100 정도임
//최근 10개중 5개이상 라인일떄 라인으로 분류
void WEB_VLine()
{
	int i, j, k, loop;
	LPBYTE fm=g_fmGrab[g_ID];
	int nX1=g_Temp.m_nInspectX1;
	int nX2=g_Temp.m_nInspectX2;
	int pitch=g_System.m_nPitch;
	int width=g_System.m_nImageW;
	int height=g_System.m_nImageH;
	long d1, d2, dc;
	double dTmp;
	int nMinLine=1;
	int nDiv=g_System.m_nImageH/2;
	int nOffset;
	int nMinUp=10000, nMinDn=100000;
	double dMaxValue, dMinValue;
	int  nMaxPos, nMinPos;
	int nLevelInspect=0;
	int nLevel, nClass;
	LPBYTE fmT;
	int nExistUp=0, nExistDn=0;
	int bNewLineUp=1, bNewLineDn=1;
	int LINE_LIMIT=8;
#ifdef TEST_MODE
	int nLineFrame=3;	//최근 10 Frame에서 3개 이상 라인일때 라인으로 분류
#else
	int nLineFrame=5;	//최근 10 Frame에서 5개 이상 라인일때 라인으로 분류
#endif

	if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) return;

	g_Temp.m_nMaxWLineValue=0;
	g_Temp.m_nMaxBLineValue=0;
	g_Temp.m_nMaxWLineValuePos=0;
	g_Temp.m_nMaxBLineValuePos=0;
//강조필터사용
	if(1)
	{
		for(j=nX1;j<nX2;j++)
		{
			if(g_Profile[j]+LINE_LIMIT>255) l_ProfileUPLimitValue[j]=255;
			else							l_ProfileUPLimitValue[j]=g_Profile[j]+LINE_LIMIT; 

			if(g_Profile[j]-LINE_LIMIT<0)   l_ProfileDnLimitValue[j]=0;
			else							l_ProfileDnLimitValue[j]=g_Profile[j]-LINE_LIMIT;
		}
		for(i=height/4-10;i<height*3/4+10;i++)
			for(j=nX1;j<nX2;j++)
			{
				if( *(fm+pitch*i+j)>l_ProfileUPLimitValue[j])		*(g_fmTemp+pitch*i+j)=l_ProfileUPLimitValue[j];
				else if(*(fm+pitch*i+j)<l_ProfileDnLimitValue[j])	*(g_fmTemp+pitch*i+j)=l_ProfileDnLimitValue[j];
				else												*(g_fmTemp+pitch*i+j)=*(fm+pitch*i+j);
			}
		Emphasize(0, g_fmTemp,g_fmTemp1, nX1,height/4-10,nX2,height*3/4+10, g_System.m_nPitch, 60, 20, 3);
		nX1+=30;
		nX2-=30;
		fmT=g_fmTemp1;
	}
	else
	{
		fmT=fm;
	}
//


	//검사 Check-------------------------------------------------------------------------------------------------------------------------
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])	
		{			
			if(g_Param.m_nLineUpValue[i]>0 && g_Param.m_nLineUpValue[i]<nMinUp) 
			{
				nMinUp=g_Param.m_nLineUpValue[i];
				nLevelInspect=1;
			}		
			if(g_Param.m_nLineDnValue[i]>0 && g_Param.m_nLineDnValue[i]<nMinDn) 
			{
				nMinDn=g_Param.m_nLineDnValue[i];
				nLevelInspect=1;
			}		
			
		}
	}
	if(nLevelInspect==0) return;
	//------------------------------------------------------------------------------------------------------------------------------------
	
	
	memset(l_LineProfile, 0, width*sizeof(long));
	memset(l_LineUpValue, 0, width*sizeof(double));
	memset(l_LineDnValue, 0, width*sizeof(double));

	for(i=height/4;i<height*3/4;i++)
		for(j=0;j<width;j++)
			*(l_LineProfile+j)+=*(fmT+pitch*i+j);


	for(k=10;k<50;k+=2)			//스지폭이 10~50 Pixel
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
			dTmp=(double)(dc-(d1+d2)/2)/(k*nDiv);
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

			if(j==nX2-(3+3*k)-1) break;

			d1=d1-*(l_LineProfile+j-1)    +*(l_LineProfile+j-1+k);
			dc=dc-*(l_LineProfile+j+k)    +*(l_LineProfile+j+2*k);
			d2=d2-*(l_LineProfile+j+1+2*k)+*(l_LineProfile+j+1+3*k);
		}
	}

	for(loop=0;loop<2;loop++)
	{
		dMaxValue=0; nMaxPos=-1;
		dMinValue=0; nMinPos=-1;
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
		if(loop==0)
		{
			g_Temp.m_nMaxWLineValue=dMaxValue*100;
			g_Temp.m_nMaxBLineValue=dMinValue*100;
			g_Temp.m_nMaxWLineValuePos=nMaxPos;
			g_Temp.m_nMaxBLineValuePos=nMinPos;
		}

		nLevel=GetLevel((int)(dMaxValue*100), g_Param.m_nLineUpValue);
		if(nLevel>0 && g_Defect.m_nDefectCount<g_System.m_nMaxDefect)
		{
			//흰색라인을 한번 검출했고, 기존흰색라인 한줄 밀고..
			nExistUp=1;
			if(!CheckIsRealLine(bNewLineUp, 0, nMaxPos, nLevel, nLineFrame)) 
			{
				bNewLineUp=0;

				if(nMaxPos>0)
				{
					for(j=nMaxPos-100;j<nMaxPos+100;j++)
					{
						if(j<0 || j>=width) continue;
						l_LineUpValue[j]=0;
					}
				}
				continue;
			}

			if(!IsInspectArea(nMaxPos)) continue; //In Position Check

			MakeDefectRect(g_Defect.m_nDefectCount, nMaxPos, height/2, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			CopyNGImageFMArea(fmT, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].left, height/4, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].right, height*3/4, pitch, -1);
			nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_LINE;
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nMaxPos;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=height/2;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =dMaxValue*100; //Value	
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0; //Size
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0; //Size X
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0; //Size Y
			if(CheckDefectOverlap()==0) 
				g_Defect.m_nDefectCount++;
		}

		nLevel=GetLevel((int)(dMinValue*(-100)), g_Param.m_nLineDnValue);
		if(nLevel>0 && g_Defect.m_nDefectCount<g_System.m_nMaxDefect)
		{
			//검정색라인을 한번 검출했고, 기존 검정색라인 한줄 밀고..
			nExistDn=1;
			if(!CheckIsRealLine(bNewLineDn, 1, nMinPos, nLevel, nLineFrame))
			{
				bNewLineDn=0;
				if(nMinPos>0)
				{
					for(j=nMinPos-100;j<nMinPos+100;j++)
					{
						if(j<0 || j>=width) continue;
						l_LineDnValue[j]=0;
					}
				}
				continue;
			}

			if(!IsInspectArea(nMinPos)) continue; //In Position Check

			MakeDefectRect(g_Defect.m_nDefectCount, nMinPos, height/2, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			CopyNGImageFMArea(fmT, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].left, height/4, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].right, height*3/4, pitch, -1);
			nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_LINE;
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=nMinPos;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=height/2;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =dMinValue*100; //Value	
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0; //Size
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0; //Size X
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0; //Size Y
			if(CheckDefectOverlap()==0) 
				g_Defect.m_nDefectCount++;
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
	if(nExistUp==0) CheckIsRealLine(bNewLineUp, 0, 0, 0, nLineFrame); //흰선
	if(nExistDn==0) CheckIsRealLine(bNewLineDn, 1, 0, 0, nLineFrame); //검정선

	return;
}

int CheckIsRealLine(int nNewFrame, int nType, int nXpos, int nLevel, int nMinCount)
{
	int i, j;
	int nMaxData=10;
	int nNGCount=0;
	int nX=nXpos/32;
	int nXAll=g_System.m_nImageW/32;
	int nCount=0;
	int nX1, nX2, nExist;

	nX1=nX-1; if(nX1<0) nX1=0;
	nX2=nX+1; if(nX2>=nXAll) nX2=nXAll-1;

	if(g_nLineCount<nMaxData)
	{
		if(nType==0)  g_LineExistUp[nX][g_nLineCount]=nLevel;
		else		  g_LineExistDn[nX][g_nLineCount]=nLevel;
		
		if(nNewFrame) g_nLineCount++;
		
		return 0;
	}
	else
	{
		if(nNewFrame)
		{
			for(j=0;j<nXAll;j++)
				for(i=0;i<nMaxData-1;i++)
				{
					if(nType==0) g_LineExistUp[j][i]=g_LineExistUp[j][i+1];
					else         g_LineExistDn[j][i]=g_LineExistDn[j][i+1];
				}
		}
		if(nType==0) g_LineExistUp[nX][nMaxData-1]=nLevel;
		else		 g_LineExistDn[nX][nMaxData-1]=nLevel;

		for(i=0;i<nMaxData;i++)
		{
			nExist=0;
			for(j=nX1;j<=nX2;j++)
			{
				if(nType==0)
				{
					if(g_LineExistUp[j][i]>0 && g_LineExistUp[j][i]<=nLevel) nExist++;
				}
				else
				{
					if(g_LineExistDn[j][i]>0 && g_LineExistDn[j][i]<=nLevel) nExist++;
				}
			}
			if(nExist) nNGCount++;
		}
		if(nNGCount>=nMinCount) return 1;  //Line NG
		else                    return 0;  //Line OK
	}
}
//WEB_VLine 와 같은 알고리즘 (수평선을 찾는다. X영역을 중심에서 W/2 만사용하고, Y=30~(H-30) 까지에서 (20x60) Emphasize filter적용후 검사
void WEB_HLine()
{
	int i, j, k, loop;
	LPBYTE fm=g_fmGrab[g_ID];
	int nX1=g_Temp.m_nInspectX1;
	int nX2=g_Temp.m_nInspectX2;
	int pitch=g_System.m_nPitch;
	int width=g_System.m_nImageW;
	int height=g_System.m_nImageH;
	long d1, d2, dc;
	double dTmp;
	int nMinLine=1;
	int nDiv;
	int nOffset;
	int nMinUp=10000, nMinDn=100000;
	double dMaxValue, dMinValue;
	int  nMaxPos, nMinPos;
	int nLevelInspect=0;
	int nLevel, nClass;
	LPBYTE fmT;
	int LINE_LIMIT=5;

	if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) return;
	if(nX2-nX1<width/8) return;
	
//강조필터사용
	if(nX2-nX1>width/2)
	{
		nX1=(g_Temp.m_nInspectX1+g_Temp.m_nInspectX2)/2-width/4;
		nX2=nX1+width/2;
	}
	nDiv=nX2-nX1;

	for(i=0;i<height;i++) l_LineProfile[i]=0;
	for(i=0;i<height;i++)
		for(j=nX1;j<nX2;j++)
			l_LineProfile[i]+=*(fm+pitch*i+j);

	for(i=0;i<height;i++)
		l_LineProfile[i]/=(nX2-nX1);

	if(1)
	{
		for(i=0;i<height;i++)
		{                  
			l_ProfileUPLimitValue[i]=l_LineProfile[i]+LINE_LIMIT; 
			if(l_ProfileUPLimitValue[i]>255) l_ProfileUPLimitValue[i]=255;

			l_ProfileDnLimitValue[i]=l_LineProfile[i]-LINE_LIMIT;
			if(l_ProfileDnLimitValue[i]<0) l_ProfileDnLimitValue[i]=0;
		}

		for(i=0;i<height;i++)
			for(j=nX1;j<nX2;j++)
			{
				if( *(fm+pitch*i+j)>l_ProfileUPLimitValue[i])		*(g_fmTemp+pitch*i+j)=l_ProfileUPLimitValue[i];
				else if(*(fm+pitch*i+j)<l_ProfileDnLimitValue[i])	*(g_fmTemp+pitch*i+j)=l_ProfileDnLimitValue[i];
				else												*(g_fmTemp+pitch*i+j)=*(fm+pitch*i+j);
			}
		Emphasize(0, g_fmTemp,g_fmTemp1, nX1,0,nX2,height, g_System.m_nPitch, 20, 60, 3);
		nX1+=10;
		nX2-=10;
		fmT=g_fmTemp1;
	}
	else
	{
		fmT=fm;
	}
//


	//검사 Check-------------------------------------------------------------------------------------------------------------------------
	for(i=0;i<10;i++)
	{
		if(g_Param.m_nUseLevel[i])	
		{			
			if(g_Param.m_nLineUpValue[i]>0 && g_Param.m_nLineUpValue[i]<nMinUp) 
			{
				nMinUp=g_Param.m_nLineUpValue[i];
				nLevelInspect=1;
			}		
			if(g_Param.m_nLineDnValue[i]>0 && g_Param.m_nLineDnValue[i]<nMinDn) 
			{
				nMinDn=g_Param.m_nLineDnValue[i];
				nLevelInspect=1;
			}		
			
		}
	}
	if(nLevelInspect==0) return;
	//------------------------------------------------------------------------------------------------------------------------------------
	
	
	memset(l_LineProfile, 0, width*sizeof(long));
	memset(l_LineUpValue, 0, width*sizeof(double));
	memset(l_LineDnValue, 0, width*sizeof(double));

	for(i=30;i<height-30;i++)
		for(j=nX1;j<nX2;j++)
			*(l_LineProfile+i)+=*(fmT+pitch*i+j);


	for(k=10;k<50;k+=2)			//스지폭이 10~50 Pixel
	{
		d1=d2=dc=0;
		for(i=30;i<30+k;i++)
			d1+=*(l_LineProfile+i);
		for(i=30+1+k;i<30+1+2*k;i++)
			dc+=*(l_LineProfile+i);
		for(i=30+2+2*k;i<30+2+3*k;i++)
			d2+=*(l_LineProfile+i);

		nOffset=k+k/2+1;

		for(i=30+1;i<height-30-(3+3*k);i++)
		{
			dTmp=(double)(dc-(d1+d2)/2)/(k*nDiv);
			if(dTmp>l_LineUpValue[i+nOffset])
			{
				if((dc-d1)*100/(k*nDiv)>=nMinUp && (dc-d2)*100/(k*nDiv)>=nMinUp)
					l_LineUpValue[i+nOffset]=dTmp;
			}
			if(dTmp<l_LineDnValue[i+nOffset])
			{
				if((d1-dc)*100/(k*nDiv)>=nMinUp && (d2-dc)*100/(k*nDiv)>=nMinUp)
					l_LineDnValue[i+nOffset]=dTmp;
			}

			if(i==height-30-(3+3*k)-1) break;

			d1=d1-*(l_LineProfile+i-1)    +*(l_LineProfile+i-1+k);
			dc=dc-*(l_LineProfile+i+k)    +*(l_LineProfile+i+2*k);
			d2=d2-*(l_LineProfile+i+1+2*k)+*(l_LineProfile+i+1+3*k);
		}
	}

	for(loop=0;loop<2;loop++)
	{
		dMaxValue=0; nMaxPos=-1;
		dMinValue=0; nMinPos=-1;
		for(i=30+10;i<height-30-10;i++)
		{
			if(l_LineUpValue[i]>dMaxValue)
			{
				dMaxValue=l_LineUpValue[i];
				nMaxPos=i;
			}
			if(l_LineDnValue[i]<dMinValue)
			{
				dMinValue=l_LineDnValue[i];
				nMinPos=i;
			}
		}
		nLevel=GetLevel((int)(dMaxValue*100), g_Param.m_nLineUpValue);
		if(nLevel>0 && g_Defect.m_nDefectCount<g_System.m_nMaxDefect)
		{
			if(!IsInspectArea(nMaxPos)) continue; //In Position Check

			MakeDefectRect(g_Defect.m_nDefectCount, width/2, nMaxPos, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			CopyNGImageFMArea(fmT, nX1,g_Defect.m_rcDefect[g_Defect.m_nDefectCount].top, nX2, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].bottom, pitch, -1);
			nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_LINE;
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=width/2;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nMaxPos;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =dMaxValue*100; //Value	
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0; //Size
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0; //Size X
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0; //Size Y
			if(CheckDefectOverlap()==0) 
				g_Defect.m_nDefectCount++;
		}

		nLevel=GetLevel((int)(dMinValue*(-100)), g_Param.m_nLineDnValue);
		if(nLevel>0 && g_Defect.m_nDefectCount<g_System.m_nMaxDefect)
		{
			if(!IsInspectArea(nMinPos)) continue; //In Position Check

			MakeDefectRect(g_Defect.m_nDefectCount, width/2, nMinPos, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
			CopyNGImageFMArea(fmT, nX1,g_Defect.m_rcDefect[g_Defect.m_nDefectCount].top, nX2, g_Defect.m_rcDefect[g_Defect.m_nDefectCount].bottom, pitch, -1);
			nClass=nLevel*MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_LINE;
			g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].x_pos=width/2;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].y_pos=nMinPos;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].defect_class=nClass;
			g_Defect.m_Defect[g_Defect.m_nDefectCount].value	   =dMinValue*100; //Value	
			g_Defect.m_Defect[g_Defect.m_nDefectCount].size        =0; //Size
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeX       =0; //Size X
			g_Defect.m_Defect[g_Defect.m_nDefectCount].sizeY       =0; //Size Y
			if(CheckDefectOverlap()==0) 
				g_Defect.m_nDefectCount++;
		}

		if(nMaxPos>0)
		{
			for(i=nMaxPos-100;i<nMaxPos+100;i++)
			{
				if(i<0 || i>=height) continue;
				l_LineUpValue[i]=0;
			}
		}
		if(nMinPos>0)
		{
			for(i=nMinPos-100;i<nMinPos+100;i++)
			{
				if(i<0 || i>=height) continue;
				l_LineDnValue[i]=0;
			}
		}
	}

	return;
}