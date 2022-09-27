#include "stdafx.h"
#include "math.h"

void WEB_FindMarking()
{
	if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) return;

	int i,j,nc,nCount, nClass,  nX, nY;
	CRect rect;
	LPBYTE fm = g_fmFlat[g_ID];//pImage->GetImagePtr();

	double	dsum, dMarkMinValue;
	BYTE dProfileX[MAX_WIDTH];
	double 	dValue[128];
	int nPosX[128],nPosY[128], nExist, nIndex;
	int nOverKill, retValue=0;
	int nDefect=0;
	int nWidth, nHeight, nPitch, nInspectX1, nInspectX2, nMulti=1;

	g_Temp.m_nMarkingInspect=0;
	g_Temp.m_dMaxMark=0;

	//정코마킹 검사여부 체크-------------------------------------------------------------------------------------------
	if(g_Param.m_dMarkMinValueWithPoint<0.001) dMarkMinValue=g_Param.m_dMarkMinValue;
	else
	{
		if(g_Param.m_dMarkMinValue<g_Param.m_dMarkMinValueWithPoint)  dMarkMinValue=g_Param.m_dMarkMinValue;
		else														  dMarkMinValue=g_Param.m_dMarkMinValueWithPoint;
	}
	if(g_Param.m_nMarkPointValue==0) dMarkMinValue=g_Param.m_dMarkMinValue; //확인값없으면 마킹값만 사용함.
	if(dMarkMinValue<1) return;
	g_Temp.m_nMarkingInspect=1;


	//평활화 영상을 압축--------------------------------------------------------------------
	if(g_Temp.m_nMakePyramidDone==0)
	{
		ThreadMakePyramid(g_fmFlat[g_ID]);  //Thread로 변경
		g_Temp.m_nMakePyramidDone=1;
	}

	nWidth		=g_System.m_nImageW;
	nHeight		=g_System.m_nImageH;
	nPitch		=g_System.m_nPitch;
	nInspectX1	=g_Temp.m_nInspectX1;
	nInspectX2	=g_Temp.m_nInspectX2;

	if(g_System.m_nMakePyramid)  //8192이면 1/2영상에서 t
	{
		fm=l_fmPyramid[1];
		nWidth	/=2;
		nHeight	/=2;
		nPitch	/=2;
		nInspectX1/=2;
		nInspectX2/=2;
		nMulti=2;
	}
	rect.top=0;
	rect.bottom=nHeight-1;
	rect.left=nInspectX1;
	rect.right=nInspectX2;

	//평활화 후 라인 Profile 이므로 
	for(i=0;i<MAX_WIDTH;i++)
		dProfileX[i] = g_System.m_nFlatBright;

	//10:LowCut 
	nCount=MakePyramidImage_Lowcut(fm, nWidth, nHeight, nPitch, 10, dProfileX, rect.left, rect.right, dMarkMinValue, dValue, nPosX, nPosY);
	g_Temp.m_dMaxMark=dValue[0];

	for(i=0;i<nCount;i++)
	{
		nIndex=g_Defect.m_nDefectCount;
		if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect) continue;

		//중복 체크 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		nExist=0;
		for(j=0;j<g_Defect.m_nDefectCount;j++)
		{	
			if(abs(	nPosX[i]- g_Defect.m_Defect[j].x_pos ) <200 && abs(	nPosY[i]- g_Defect.m_Defect[j].y_pos ) <256 ) 
			{
				nExist=1;
				break;
			}
		}
		if(nExist) continue;


		nOverKill=IsOverKillForNEL4Mark(fm, 0, nPosX[i],nPosY[i], nPitch, nHeight-1, g_Param.m_nMarkPointValue, dMarkMinValue, &retValue);
		if(nOverKill==2) continue;

		if(retValue<dValue[i]) retValue=dValue[i];
		
		//마킹에서 g_Level.m_nMarkPointValue값으로 한번더 과검 체크-------------------------------------------------------------------------------------
		if(g_Param.m_nMarkPointValue==0)
		{
			if(dValue[i]<g_Param.m_dMarkMinValue) 
				continue;
		}
		else
		{
			if(dValue[i]>=g_Param.m_dMarkMinValue) nOverKill=0;
			
			if(nOverKill) continue;
		}


		nX=nPosX[i]*nMulti; 
		nY=nPosY[i]*nMulti;
		
		nClass=MILLION+g_Temp.m_nPCFirstNo*CLASSDEV+NG_MARKING;
		g_Defect.m_nFrameNum=g_Temp.m_nGrabFrameID;
		MakeDefectRect(g_Defect.m_nDefectCount, nX, nY, g_Temp.m_nInspectX1, g_Temp.m_nInspectX2);
		g_Defect.m_Defect[nIndex].x_pos=nX;
		g_Defect.m_Defect[nIndex].y_pos=nY;
		g_Defect.m_Defect[nIndex].defect_class=nClass;
		g_Defect.m_Defect[nIndex].value		  =retValue; //Value	
		g_Defect.m_Defect[nIndex].size        =dValue[i]; //Size
		g_Defect.m_Defect[nIndex].sizeX       =dValue[i]; //Size X
		g_Defect.m_Defect[nIndex].sizeY       =dValue[i]; //Size Y
		CopyNGImageFM(g_fmFlat[g_ID], g_System.m_nPitch);

		if(IsNGImage(g_Defect.m_pImage[nIndex], 0, 0, BAD_IMG_WIDTH-1, BAD_IMG_HEIGHT-1, BAD_IMG_WIDTH)==0) continue;


		
		if(g_Param.m_nMarkingLevelChange && g_Param.m_nMarkingLevelChange<=10) 
			g_Defect.m_Defect[nIndex].defect_class = g_Temp.m_nPCFirstNo*CLASSDEV+NG_MARKING+g_Param.m_nMarkingLevelChange*MILLION;
		
		g_Defect.m_nDefectCount++;
		if(g_Defect.m_nDefectCount>=g_System.m_nMaxDefect-2) break;
	}
}

//슬릿하고 관계없는 함수 (사용중)---------------------------------------------------------------------------------
int l_fm1[4096*4096];
int l_fm2[4096*4096/4];
int l_fm4[4096*4096/16];
int l_fm8[4096*4096/64];
int l_fm16[4096*4096/256];
int l_fm32[4096*4096/1024];

//return : dLimit 이하의 수 
int MakePyramidImage_Lowcut(LPBYTE fm, int width, int height, int pitch, int nLowCut, LPBYTE Profile, int left, int right, double dLimit, double *dValue, int *nPosX, int *nPosY)
{
	int k,i,j, ii, jj, nTmp, X1, X2, Y1, Y2;
	int nDevide=1, nWidthS, nHeightS, nWidthD,  nHeightD;
	int *fmS, *fmD, *fmT, nMinValue=0;
	int nLowCount=0;
	double dTmp, dTmpMin=0, dNormalMin=0;
	int  Px, Py;
	int  nDevideX=16, nDevideY=8; //최대 64개
	LPBYTE fmTemp;

	//영상평활화(2x2)씩  : 단 Lowcut적용해서 영상반전 >>>>>>>>>	
	for(i=0;i<height-1;i+=2)
	{
		fmTemp=fm+pitch*i;
		fmT   =l_fm1+pitch*i;
		for(j=0;j<width-1;j+=2)
		{
			nTmp=*(Profile+j)-*(fmTemp+j);
			if(nTmp<0)				nTmp=0;
			else if(nTmp>nLowCut)	nTmp=nLowCut;

			*(fmT+      j  )=nTmp;
			*(fmT+      j+1)=nTmp;
			*(fmT+pitch+j  )=nTmp;
			*(fmT+pitch+j+1)=nTmp;
		}
	}
	nDevideX=16;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


	//영상압축 1/2, 1/4, 1/8, 1/16, 1/32 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	nWidthS =nWidthD =width;
	nHeightS=nHeightD=height;
	nDevide =1;	

	for(k=0;k<5;k++)
	{
		if(k==0)		{ fmS=l_fm1;  fmD=l_fm2; }
		else if(k==1)	{ fmS=l_fm2;  fmD=l_fm4; }
		else if(k==2)	{ fmS=l_fm4;  fmD=l_fm8; }
		else if(k==3)	{ fmS=l_fm8;  fmD=l_fm16;}
		else if(k==4)	{ fmS=l_fm16; fmD=l_fm32;}
		nWidthS =width/nDevide;							//4096, 2048, 1024, 512, 256
		nHeightS=height/nDevide;						//4096, 2048, 1024, 512, 256
		nDevide*=2;										//1, 2, 4, 8, 16
		nWidthD =width/nDevide;							//2048, 1024, 512, 256, 128
		nHeightD=height/nDevide;						//2048, 1024, 512, 256, 128

		for(i=0, ii=0;i<nHeightS-1;i+=2, ii++)			
		{
			for(j=0, jj=0;j<nWidthS-1;j+=2, jj++)
			{
				fmT=fmS+nWidthS*i+j;
				nTmp=*(fmT)+*(fmT+1)+*(fmT+nWidthS)+*(fmT+nWidthS+1);
				*(fmD+nWidthD*ii+jj)=nTmp;
			}
		}
	}
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	//4096x2048 영상으로 시작했다면 nWidthD x nHeightD 는 128 x 64 가 된다. 
	//구간을 1024x1024로 8구간 나누어서 대표값 1개씩 구한다.>>>>>>>>>>>
	if(left){  left=left/32+2;   }
	if(right){ right=right/32-2; }
	for(k=0;k<nDevideX*nDevideY;k++)
	{										//X는 16분할 Y는 8분할 
		X1=nWidthD/nDevideX*(k%nDevideX);	//nWidthD=128 라면   k=0~31  0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128
		X2=X1+nWidthD/nDevideX;				//                           X1+8
		Y1=nHeightD/nDevideY*(k/nDevideX);	//nHeightD=64 라면           0,0,0,0,..16개,  32, 32, 32, 32...16개
		Y2=Y1+nHeightD/nDevideY;			//							 Y1+32
		if(X1<left) X1=left;
		if(X2>right)X2=right;

		dTmpMin=-1;
		for(i=Y1;i<Y2;i++)
			for(j=X1;j<X2;j++)
			{
				dTmp=*(fmD+nWidthD*i+j)/(32.0*32);	//원래는 (32x32)이나 
				if(dTmp>dTmpMin)
				{
					dTmpMin=dTmp;
					Px=j;
					Py=i;
				}
			}
		if(dTmpMin>=dLimit)
		{
			dValue[nLowCount]=dTmpMin;
			nPosX[nLowCount]=(Px*32+16);
			nPosY[nLowCount]=(Py*32+16);
			nLowCount++;
		}
		if(dTmpMin>dNormalMin) dNormalMin=dTmpMin;
	}
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	//제일 큰 순서로 소팅하기>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	for(i=0;i<nLowCount;i++)
	{
		for(j=i+1;j<nLowCount;j++)
		{
			if(dValue[j]>dValue[i])
			{
				dTmp=dValue[i]; dValue[i]=dValue[j]; dValue[j]=dTmp;
				nTmp=nPosX[i];  nPosX[i] =nPosX[j];	 nPosX[j] =nTmp;
				nTmp=nPosY[i];  nPosY[i] =nPosY[j];	 nPosY[j] =nTmp;
			}
		}
	}
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	//가까운것 없애기 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	for(i=0;i<nLowCount;i++)
	{
		for(j=i+1;j<nLowCount;j++)
		{
			if(abs(nPosX[i]-nPosX[j])<256 && abs(nPosY[i]-nPosY[j])<256)
			{
				for(k=j;k<nLowCount-1;k++)
				{
					dValue[k]=dValue[k+1];
					nPosX[k] =nPosX[k+1];
					nPosY[k] =nPosY[k+1];
				}
				i--;
				nLowCount--;
				break;
			}
		}
	}
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	if(nLowCount==0) dValue[0]=dNormalMin;

	return nLowCount;
}

int IsOverKillForNEL4Mark(LPBYTE fm, int nType, int nX, int nY, int pitch, int nGrabHeight, int nDnValue, double dMarkValue, int *pRetValue)
{
	int ret=1; //과검 
	int i, j;
	int left, top, right, bottom, nHCount=0;
	int nMinValue=80*9;
	int nMaxValue=0;
	int nTmp;
	

	left=nX-BAD_IMG_WIDTH/2; if(left<0) left=0; if(left+BAD_IMG_WIDTH>=pitch) left=pitch-BAD_IMG_WIDTH;
	right=left+BAD_IMG_WIDTH;
	top=nY-BAD_IMG_HEIGHT/2; if(top<0) top=0;  if((top+BAD_IMG_HEIGHT)>=nGrabHeight) top=nGrabHeight-BAD_IMG_HEIGHT;
	bottom=top+BAD_IMG_HEIGHT;	

	for(i=top+1;i<bottom-1;i++)
		for(j=left+1;j<right-1;j++)
		{
			nTmp=*(fm+pitch*(i-1)+j-1)+*(fm+pitch*(i-1)+j)+*(fm+pitch*(i-1)+j+1)+
				 *(fm+pitch*i+j-1)+*(fm+pitch*i+j)+*(fm+pitch*i+j+1)+
				 *(fm+pitch*(i+1)+j-1)+*(fm+pitch*(i+1)+j)+*(fm+pitch*(i+1)+j+1);

			if(nTmp<nMinValue) nMinValue=nTmp;
			if(nTmp>nMaxValue) nMaxValue=nTmp;
		}
	
	*pRetValue=g_System.m_nFlatBright-nMinValue/9.0;
	if((nMaxValue-nMinValue)/9.0<dMarkValue) return 2;

	if(g_System.m_nFlatBright-nMinValue/9.0<nDnValue) return 1; //과검
	else											  return 0; //불량 
}

//영상이 Defect이 맞으면 Return 1   Defect이 아니고 Noise에 의해서 발생된 것이면 0임 
//영상이 수평으로 한줄 튀는 경우 불량아님
int IsNGImage(LPBYTE fm, int left, int top, int right, int bottom, int pitch)
{
	int i,j, nCount=0;
	int nY[128];
	int nPos, nSlope, nMaxSlope;

	for(j=left;j<right;j++)
	{
		nMaxSlope=0;
		nPos=0;
		for(i=top;i<bottom-1;i++)
		{
			nSlope=*(fm+pitch*i+j)-*(fm+pitch*(i+1)+j);
			if(nSlope<0)nSlope*=-1;
			if(nSlope>nMaxSlope)
			{
				nMaxSlope=nSlope;
				nPos=i;
			}
		}
		if(nCount<128)
			nY[nCount++]=nPos;
	}
	for(i=1;i<nCount;i++)
	{
		if(abs(nY[0]-nY[i])>1)
			return 1;
	}
	return 0;
}