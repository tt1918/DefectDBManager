#include "stdafx.h"
#include "math.h"

int l_SumVer[MAX_WIDTH];
long FIndROIInfo(LPBYTE fm, int left, int top, int right, int bottom, int pitch)
{
	int ret=0;





	return ret;
}


BOOL PtInPolygon(POINT poly[], int nCount, int nX, int nY) 
{
	int i, j;
	BOOL c = FALSE;
	for (i = 0, j = nCount-1; i < nCount; j = i++) {
		if ( (((poly[i].y<=nY) && (nY<poly[j].y)) ||
		     ((poly[j].y<=nY) && (nY<poly[i].y))) &&
		     (nX < (poly[j].x - poly[i].x) * (nY - poly[i].y) /
		     (poly[j].y - poly[i].y) + poly[i].x))
		     c = !c;
	}
	return c;
}


//회전중심 (Cx, Cy)
//Point의 Translation값 (Dx, Dy) , Translation이없으면 (0, 0)
//dAngleDeg 회전각(degree)
int PointRotation(double Cx, double Cy, double dAngleDeg, double Dx, double Dy, double Px, double Py,  double *Px_, double *Py_)
{
	double sinT=sin(dAngleDeg*3.141592/180.0);
	double cosT=cos(dAngleDeg*3.141592/180.0);

	*Px_=(Px-Cx)*cosT-(Py-Cy)*sinT+Dx+Cx;
	*Py_=(Px-Cx)*sinT+(Py-Cy)*cosT+Dy+Cy;

	return 0;
}
//return 0: SUCCESS
//       -1: FAILURE
int FindLineFunction(double x1, double y1, double x2, double y2, double *t, double *a, double *b)
{
	
	if(x1==x2)
	{
		if(y1==y2) return -1;

		*t=0;
		*a=1;
		*b=-x1;
	}
	else
	{
		*t=1;
		*a=(y1-y2)/(x1-x2);
		*b=y1-*a*x1;
	}

	return 0;
}
//FindStdevArea 함수와 결과가 완전히 같음. FindStdevArea 함수 이용바람.
double GetStdevArea(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch)
{
	int i,j;
	double dSum=0,dTmp;
	int nSkip = 1, nTmp;
	double dAve, dStdev=0;
	long nArea = (x2-x1)*(y2-y1);

	//평균값 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	for(i=y1; i<y2; i++)
		for(j=x1; j<x2; j++)
			dSum += *(fm+pitch*i+j);			
	
	if(nArea>0)
		dAve = (double)dSum/nArea;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	
	dSum=0;
	double dtemp1 = 0;
	double dtemp2 = 0;
	for(i=y1; i<y2; i++)
		for(j=x1; j<x2; j++)
		{
			nTmp    =*(fm+pitch*i+j);
			dTmp    = nTmp - dAve;
			dtemp1 += nTmp;
			dtemp2 += nTmp * nTmp;
			dSum   += dTmp*dTmp;
		}
	
	if(nArea*dtemp2-dtemp1*dtemp1>0 && nArea*(nArea-1.)>0)
		dStdev = sqrt((nArea*dtemp2-dtemp1*dtemp1)/(nArea*(nArea-1.)));

	return dStdev;
}

int GetStdevNBrightArea(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, double *pStdev, double *pAverage)
{
	int i,j;
	double dSum=0,dTmp;
	int nSkip = 1, nTmp;
	double dAve, dStdev=0;
	long nArea = (x2-x1)*(y2-y1);

	//평균값 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	for(i=y1; i<y2; i++)
		for(j=x1; j<x2; j++)
			dSum += *(fm+pitch*i+j);			
	
	if(nArea>0)
		dAve = (double)dSum/nArea;
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	
	dSum=0;
	double dtemp1 = 0;
	double dtemp2 = 0;
	for(i=y1; i<y2; i++)
		for(j=x1; j<x2; j++)
		{
			nTmp    =*(fm+pitch*i+j);
			dTmp    = nTmp - dAve;
			dtemp1 += nTmp;
			dtemp2 += nTmp * nTmp;
			dSum   += dTmp*dTmp;
		}
	
	if(nArea*dtemp2-dtemp1*dtemp1>0 && nArea*(nArea-1.)>0)
		dStdev = sqrt((nArea*dtemp2-dtemp1*dtemp1)/(nArea*(nArea-1.)));

	*pStdev=dStdev;
	*pAverage=dAve;

	return 0;
}


//일정영역 STDEV값 구하는 함수 (이 함수 이용하면 넓은 영역 짧은 시간 스캔하는 방법이 가능함)
double FindStdevArea(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch)
{
	int i,j;
	long nSum=0;
	double dSum2=0, stdev=0; 
	BYTE tmp;
	long   nArea=(x2-x1)*(y2-y1);

	for(i=y1;i<y2;i++)
		for(j=x1;j<x2;j++)
		{
			tmp=*(fm+pitch*i+j);
			nSum+=tmp;
			dSum2+=(tmp*tmp);
		}

	if((nArea*dSum2-(double)nSum*(double)nSum)>0 && ((double)nArea*((double)nArea-1.))>0)
		stdev=sqrt((nArea*dSum2-(double)nSum*(double)nSum)/((double)nArea*((double)nArea-1.)));	

	return stdev;
}
double FindAveArea(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, int nSkip)
{
	int i,j;
	long nSum=0;

	BYTE tmp;
	int nArea=1;

	for(i=y1;i<y2;i=i+nSkip)
		for(j=x1;j<x2;j=j+nSkip)
		{
			tmp=*(fm+pitch*i+j);
			nSum+=tmp;
			nArea++;
		}

	return nSum/nArea;
}
void Binary(LPBYTE fmS, LPBYTE fmD,int left, int top, int right, int bottom, int pitch, int nThreshold)
{
	int i,j;

	for(i=top;i<bottom;i++)
	{
		for(j=left;j<right;j++)
		{
			if(*(fmS+pitch*i+j)>nThreshold)  *(fmD+pitch*i+j)=255;
			else                             *(fmD+pitch*i+j)=0;
		}
	}
}

//압축하는데 시간을 줄이기 위해서 Sampling해서 압축 (Binary 압축)
//nSkip=2면  1/2 압축   3이면 1/3압축
void CompressSampling(LPBYTE fmS, int left, int top, int right, int bottom, int pitch, int nSkip, LPBYTE fmD, int nThreshold, int *pWidth, int *pHeight)
{
	int i, j;
	int nLocalW=0, nLocalH=0;
	int nX, nY;

	for(i=top;i<bottom;i+=nSkip) nLocalH++;
	for(j=left;j<right;j+=nSkip) nLocalW++;

	nY=0;
	for(i=top;i<bottom;i+=nSkip)
	{
		nX=0;
		for(j=left;j<right;j+=nSkip)
		{
			if(*(fmS+pitch*i+j)>nThreshold) *(fmD+nLocalW*nY+nX)=255;
			else                            *(fmD+nLocalW*nY+nX)=0;
	
			nX++;
		}
		nY++;
	}

	*pWidth =nLocalW;
	*pHeight=nLocalH;
}

//
// 영상메모리 시작번지가 fmOrg이고 피치는 OrgPitch이고, 임의의 사각형( 4점은 *POrg이고 반시계방향으로 코너번호가 매겨짐)
// 영역을 아래의 메모리로 매핑하는 함수 
// 영상의 시작번지가 pMap이고 피치는 MapPitch인 영상으로 매핑 
//  PMap[0]=(left, top)주로 (0,0)
//  PMap[1]=(left, bottom)주로 (0,height-1)
//  PMap[2]=(right, bottom)주로 (width-1,height-1)
//  PMap[3]=(right, top)주로 (width-1,0)
//  
int  InterpolationTransform_SubPixel(CPoint *POrg, LPBYTE fmOrg, int OrgPitch, CPoint *PMap, LPBYTE fmMap, int MapPitch)    
{
	// Affine Tranform의 6 Parameter 구하기 
	double a,b;
	double dWidth=(PMap[3].x-PMap[0].x);
	double dHeight=(PMap[1].y-PMap[0].y);

	int i,j;
	int x,y;
	double dX, dY;
	for(i=0;i<(PMap[1].y-PMap[0].y);i++)
		for(j=0;j<(PMap[3].x-PMap[0].x);j++)
		{
			a=(double)(j-PMap[0].x)/dWidth;
			b=(double)(i-PMap[0].y)/dHeight;

			dX=POrg[0].x*(1.0-a)*(1.0-b)+POrg[3].x*(a)*(1.0-b)+POrg[1].x*(1.0-a)*(b)+POrg[2].x*(a)*(b);
			dY=POrg[0].y*(1.0-a)*(1.0-b)+POrg[3].y*(a)*(1.0-b)+POrg[1].y*(1.0-a)*(b)+POrg[2].y*(a)*(b);

			dX=((int)((dX+0.00001)*100))/100.0;
			dY=((int)((dY+0.00001)*100))/100.0;
			
			x=(int)dX;
			y=(int)dY;

			*(fmMap+i*MapPitch+j)=0;
			if(x+1>=POrg[2].x || y+1>=POrg[2].y) continue;;       

			a=dX-x;
			b=dY-y;
			*(fmMap+i*MapPitch+j)=(BYTE)(*(fmOrg+OrgPitch*y+x)*(1.0-a)*(1.0-b)+
								  *(fmOrg+OrgPitch*y+x+1)*(a)*(1.0-b)+
								  *(fmOrg+OrgPitch*(y+1)+x)*(1.0-a)*(b)+
								  *(fmOrg+OrgPitch*(y+1)+x+1)*(a)*(b));						
		}

	return 0; //정상수행 
}




void Registermaster(LPBYTE fmS,int nID, int nSmooth, int nAllowPixel) 
{
/*	LPBYTE fm=0;
	int i, j;
	long width, height;
	CString roiname=g_pTeach->m_UD[nID].m_sROIName;

	if(nID<0 || nID>=MAX_UDINSPECT_NUM) return;

	if(g_pTeach->m_UD[nID].m_nRegister)
	{
		if(g_pTeach->m_UD[nID].m_fm)		delete g_pTeach->m_UD[nID] .m_fm;
		if(g_pTeach->m_UD[nID].m_fmMax)		delete g_pTeach->m_UD[nID].m_fmMax;
		if(g_pTeach->m_UD[nID].m_fmMin)		delete g_pTeach->m_UD[nID].m_fmMin;
		if(g_pTeach->m_UD[nID].m_fmMask)	delete g_pTeach->m_UD[nID].m_fmMask; 
	}
	g_pDisplay->GetROIImage((long)fmS,roiname, (long*)&fm, &width, &height);

	g_pTeach->m_UD[nID].m_nRegister=1;
	g_pTeach->m_UD[nID].m_fm=new BYTE[width*height];
	g_pTeach->m_UD[nID].m_fmMax=new BYTE[width*height];
	g_pTeach->m_UD[nID].m_fmMin=new BYTE[width*height];
	g_pTeach->m_UD[nID].m_fmMask=new BYTE[width*height];
	g_pTeach->m_UD[nID].m_nWidth=width;
	g_pTeach->m_UD[nID].m_nHeight=height;
	g_pTeach->m_UD[nID].m_nMaskUSE=0;
	g_pTeach->m_UD[nID].m_nSmooth    =nSmooth;
	g_pTeach->m_UD[nID].m_nAllowPixel=nAllowPixel;

	if(nSmooth>0)
	{
		COSS_Mean(fm, g_pTeach->m_UD[nID].m_fm, 0,0, width-1, height-1, width, nSmooth*2+1, nSmooth*2+1);
	}
	else
	{
		for(i=0;i<height;i++)
			for(j=0;j<width;j++)
				*(g_pTeach->m_UD[nID].m_fm+width*i+j)=*(fm+width*i+j);
	}

	memset(g_pTeach->m_UD[nID].m_fmMask, 0, width*height);

	MakeMaxMinImage(g_pTeach->m_UD[nID].m_fm,g_pTeach->m_UD[nID].m_fmMax, g_pTeach->m_UD[nID].m_fmMin, 0, 0, width-1, height-1, width, nAllowPixel); 
*/
}


//영상차이 구하기 ======================================================================================
//nColor==0  White 만 구한다.
//      ==1  Black 만 구한다.
//      ==2  White Black모두 구한다.
//fmThUp 백 Threshold MAP
//fmThDn 흑 Threshold MAP
//fmBinary; Binary MAP
long FindDiffImage(int nSmooth, int nColor, LPBYTE fmSrc, LPBYTE fmMax, LPBYTE fmMin, LPBYTE fmThUp, LPBYTE fmThDn, LPBYTE fmDiff, LPBYTE fmBinary,
				   int nMakeDiffImage, long left, long top, long right, long bottom, long pitch) 
{
	// TODO: Add your dispatch handler code here
	int i,j;
	int nTmp, nTemp1, nTemp2;
	LPBYTE fm;
	

	if(nSmooth)
	{
		COSS_Mean(fmSrc, g_fmTemp, left, top, right, bottom, pitch, nSmooth*2+1, nSmooth*2+1);
		fm=g_fmTemp;
	}
	else
	{
		fm=fmSrc;
	}

	if(nColor==0)
	{
		for(i=top;i<=bottom;i++)
			for(j=left;j<=right;j++)
			{
				nTemp1=*(fm+pitch*i+j) -*(fmMax+pitch*i+j);	if(nTemp1<0)	nTemp1=0; 
				*(fmDiff+pitch*i+j)=nTemp1;
			}
	}
	else if(nColor==1)
	{
		for(i=top;i<=bottom;i++)
			for(j=left;j<=right;j++)
			{
				nTemp2=*(fmMin+pitch*i+j) -*(fm+pitch*i+j);	if(nTemp2<0)	nTemp2=0;
				*(fmDiff+pitch*i+j)=nTemp2;
			}
	}
	else
	{
		memset(fmBinary, 0, pitch*bottom);
		for(i=top;i<=bottom;i++)
			for(j=left;j<=right;j++)
			{
				nTemp1=*(fm+pitch*i+j)	  -*(fmMax+pitch*i+j);	
				if(nTemp1>*(fmThUp+pitch*i+j))
				{
					*(fmBinary+pitch*i+j)=255;					
					continue;
				}

				nTemp2=*(fmMin+pitch*i+j) -*(fm+pitch*i+j);	
				if(nTemp2>*(fmThDn+pitch*i+j))
				{
					*(fmBinary+pitch*i+j)=255;
				}
			}

		if(nMakeDiffImage) //검사에 필요없으나 영상차이값을 보려면 필요함.
		{
			for(i=top;i<=bottom;i++)
				for(j=left;j<=right;j++)
				{
					nTemp1=*(fm+pitch*i+j)	  -*(fmMax+pitch*i+j);	
					nTemp2=*(fmMin+pitch*i+j) -*(fm+pitch*i+j);	
					if(nTemp1>0)
					{
						if(128+nTemp1>255) *(fmDiff+pitch*i+j)=255;
						else               *(fmDiff+pitch*i+j)=128+nTemp1;
					}
					else if(nTemp2>0)
					{
						if(128-nTemp2<0) *(fmDiff+pitch*i+j)=0;
						else   			 *(fmDiff+pitch*i+j)=128-nTemp2;
					}
					else
						*(fmDiff+pitch*i+j)=128;
				}
		}
	}

	return 0;
}


long MakeMaxMinImage(LPBYTE fm, LPBYTE fmMax, LPBYTE fmMin,  long left, long top, long right, long bottom, int pitch, int nAllowLimit) 
{
	if(nAllowLimit<1 ) nAllowLimit=1;
	else if(nAllowLimit>10) nAllowLimit=10;

	long kernel[500];
	int nKernelSize=1+2*nAllowLimit;

	memset(kernel, 0, 500*sizeof(long));

	Dilate_Gray(fm, fmMax, left, top, right, bottom, pitch,  kernel,  nKernelSize, nKernelSize);
	Erode_Gray(fm, fmMin,  left, top, right, bottom, pitch,  kernel,  nKernelSize, nKernelSize);
	
	return 0;
}
void Dilate_Gray(LPBYTE fmSour, LPBYTE fmDest, int left, int top, int right, int bottom, int nPitch,long* pKernel, int nKernelSizeX, int nKernelSizeY)
{
// 변수 체크 
   if(right>=nPitch) right=nPitch-1;
   
   int i,j,k,l;
   int nOrgX, nOrgY;
   int Kernel_ALLZero=0;
   int MaxValue;
   bool flag=false;
   

   if(pKernel==NULL)
   {
		pKernel=new long[nKernelSizeX*nKernelSizeY];
		memset(pKernel, 0, nKernelSizeX*nKernelSizeY*sizeof(long));
		flag=true;
   }

   nOrgX=(int)(nKernelSizeX/2.0-0.5);
   nOrgY=(int)(nKernelSizeY/2.0-0.5);

   //BEGIN-0: 테두리는 원래 영상과 동일하게 만든다. >>>>>>>>>>>>>>>>>>
   for(i=0;i<=nOrgY;i++) 
      for(j=left;j<=right;j++)
      {
		  *(fmDest+nPitch*(top+i)+j)=*(fmSour+nPitch*(top+i)+j);
		  *(fmDest+nPitch*(bottom-i)+j)=*(fmSour+nPitch*(bottom-i)+j);
      }

   for(j=0;j<=nOrgX;j++)
      for(i=top;i<=bottom;i++) 
	  {
		  *(fmDest+nPitch*i+(left+j))=*(fmSour+nPitch*i+(left+j));
		  *(fmDest+nPitch*i+(right-j))=*(fmSour+nPitch*i+(right-j));
      }
    //END-0: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    for(k=0;k<nKernelSizeY;k++)
		for(l=0;l<nKernelSizeX;l++)
           if( *(pKernel+nKernelSizeX*k+l) ) Kernel_ALLZero=1;
  
   if(Kernel_ALLZero==0) // 모든 kernel의 값이 ZERO이면 
   {
	  for(i=top;i<=bottom-nKernelSizeY;i++)         
 	     for(j=left;j<=right-nKernelSizeX;j++)                           
		 {
            MaxValue=0;
		    for(k=0;k<nKernelSizeY;k++)
			   for(l=0;l<nKernelSizeX;l++)
				   if( *(fmSour+(i+k)*nPitch+j+l) > MaxValue)
                   {
						MaxValue=*(fmSour+(i+k)*nPitch+j+l);
                   }
					   
		    *(fmDest+(i+nOrgY)*nPitch+j+nOrgX)=MaxValue;
       }
   }
   else  //kernel이 어떤 값을 가지고 있을때
   {
	  for(i=top;i<=bottom-nKernelSizeY;i++)         
 	     for(j=left;j<=right-nKernelSizeX;j++)                           
		 {
            MaxValue=0;
		    for(k=0;k<nKernelSizeY;k++)
			   for(l=0;l<nKernelSizeX;l++)
				   if( *(fmSour+(i+k)*nPitch+j+l)+*(pKernel+nKernelSizeX*k+l) > MaxValue)
                   {
						MaxValue=*(fmSour+(i+k)*nPitch+j+l)+*(pKernel+nKernelSizeX*k+l);
                   }
			if(	MaxValue>255) MaxValue=255;	   
		    *(fmDest+(i+nOrgY)*nPitch+j+nOrgX)=MaxValue;
       }

   }

   if(flag)
   {
		delete pKernel;
		pKernel=NULL;
   }
}
void Erode_Gray(LPBYTE fmSour, LPBYTE fmDest, int left, int top, int right, int bottom, int nPitch,long* pKernel, int nKernelSizeX, int nKernelSizeY)
{
   // 변수 체크 
   if(right>=nPitch) right=nPitch-1;
   
   int i,j,k,l;
   int nOrgX, nOrgY;
   int Kernel_ALLZero=0;
   int MinValue;
   bool flag=false;
   

   if(pKernel==NULL)
   {
		pKernel=new long[nKernelSizeX*nKernelSizeY];
		memset(pKernel, 0, nKernelSizeX*nKernelSizeY*sizeof(long));
		flag=true;
   }


   nOrgX=(int)(nKernelSizeX/2.0-0.5);
   nOrgY=(int)(nKernelSizeY/2.0-0.5);

   //BEGIN-0: 테두리는 원래 영상과 동일하게 만든다. >>>>>>>>>>>>>>>>>>
   for(i=0;i<=nOrgY;i++) 
      for(j=left;j<=right;j++)
      {
		  *(fmDest+nPitch*(top+i)+j)=*(fmSour+nPitch*(top+i)+j);
		  *(fmDest+nPitch*(bottom-i)+j)=*(fmSour+nPitch*(bottom-i)+j);
      }

   for(j=0;j<=nOrgX;j++)
      for(i=top;i<=bottom;i++) 
	  {
		  *(fmDest+nPitch*i+(left+j))=*(fmSour+nPitch*i+(left+j));
		  *(fmDest+nPitch*i+(right-j))=*(fmSour+nPitch*i+(right-j));
      }
    //END-0: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    for(k=0;k<nKernelSizeY;k++)
		for(l=0;l<nKernelSizeX;l++)
           if( *(pKernel+nKernelSizeX*k+l) ) Kernel_ALLZero=1;
  
   if(Kernel_ALLZero==0) // 모든 kernel의 값이 ZERO이면 
   {
	  for(i=top;i<=bottom-nKernelSizeY;i++)         
 	     for(j=left;j<=right-nKernelSizeX;j++)                           
		 {
            MinValue=255;
		    for(k=0;k<nKernelSizeY;k++)
			   for(l=0;l<nKernelSizeX;l++)
				   if( *(fmSour+(i+k)*nPitch+j+l) < MinValue)
                   {
						MinValue=*(fmSour+(i+k)*nPitch+j+l);
                   }
					   
		    *(fmDest+(i+nOrgY)*nPitch+j+nOrgX)=MinValue;
       }
   }
   else  //kernel이 어떤 값을 가지고 있을때
   {
	  for(i=top;i<=bottom-nKernelSizeY;i++)         
 	     for(j=left;j<right-nKernelSizeX;j++)                           
		 {
            MinValue=255;
		    for(k=0;k<nKernelSizeY;k++)
			   for(l=0;l<nKernelSizeX;l++)
				   if( *(fmSour+(i+k)*nPitch+j+l)-*(pKernel+nKernelSizeX*k+l) < MinValue)
                   {
						MinValue=*(fmSour+(i+k)*nPitch+j+l)-*(pKernel+nKernelSizeX*k+l);
                   }
			if(	MinValue<0) MinValue=0;	   
		    *(fmDest+(i+nOrgY)*nPitch+j+nOrgX)=MinValue;
       }

   }
   if(flag)
   {
		delete pKernel;
		pKernel=NULL;
   }
}



extern LPBYTE g_fm[3];
extern int    g_Data[10];
extern double g_dData[10];
int MakeEmphasize(int N)
{
	LPBYTE fmS=g_fm[0];
	LPBYTE fmD=g_fm[1];
	int width, left,top,right,bottom,pitch;
	int nSizeX, nSizeY;
	double dFactor;

	width	=g_Data[0];
	top		=g_Data[1];
	bottom	=g_Data[2];
	pitch	=g_Data[3];
	nSizeX	=g_Data[4];
	nSizeY	=g_Data[5];
	dFactor	=g_dData[0];

	left=width*N/8;
	right=width*(N+1)/8;
	if(left<nSizeX/2)		  left=nSizeX/2;
	if(right>width-nSizeX/2)  right=width-nSizeX/2;

	 Emphasize(N, fmS, fmD, left-nSizeX/2, top, right+nSizeX/2, bottom, pitch, nSizeX, nSizeY, dFactor);

	 return 0;
}

int l_EmSumVer[8][8192];
int Emphasize(int N, LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY, double dFactor)
{
	int i, j;
	int nSum;
	int nHalfX, nHalfY;
	int nTmp;
	int nFactor=(int)(dFactor*100);
	int *pSumVer;

	if(nSizeX%2==0) nSizeX++;
	if(nSizeY%2==0) nSizeY++;
	pSumVer=l_EmSumVer[N];

	nHalfX=nSizeX/2;
	nHalfY=nSizeY/2;

//	for(i=top;i<bottom;i++)
//		memcpy(fmD+pitch*i+left, fmS+pitch*i+left, (right-left));

	//맨 왼쪽 Y에 따른 nSizeX만큼 합 ------------
	for(i=top;i<bottom;i++)
	{	
		nSum=0;
		for(j=left;j<left+nSizeX;j++)
			nSum+=*(fmS+pitch*i+j);

		pSumVer[i]=nSum;
	}
	//--------------------------------------------

	for(j=left+nHalfX;j<right-nHalfX;j++)
	{
		if(j!=left+nHalfX) //한칸씩 오른쪽으로 감에 따라 이 값도 변경
		{
			for(i=top;i<bottom;i++)
				pSumVer[i]=pSumVer[i]+*(fmS+pitch*i+j-nHalfX+nSizeX-1)-*(fmS+pitch*i+j-nHalfX-1);
		}

		nSum=0;
		for(i=top;i<top+nSizeY;i++)
			nSum+=pSumVer[i];

		for(i=top+nHalfY;i<bottom-nHalfY;i++)
		{
			nTmp=*(fmS+pitch*i+j)+(*(fmS+pitch*i+j)-nSum/(nSizeX*nSizeY))*nFactor/100;
			if(nTmp<0)		  nTmp=0;
			else if(nTmp>255) nTmp=255;
			*(fmD+pitch*i+j)=nTmp;

			nSum=nSum+pSumVer[i-nHalfY+nSizeY]-pSumVer[i-nHalfY];
		}
	}

	return 0;
}

int HighPass(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY)
{
	int i, j;
	int nSum;
	int nHalfX, nHalfY;
	int nTmp;

	if(nSizeX%2==0) nSizeX++;
	if(nSizeY%2==0) nSizeY++;

	nHalfX=nSizeX/2;
	nHalfY=nSizeY/2;


	for(i=top;i<bottom;i++)
		memset(fmD+pitch*i+left, 128, (right-left));

	//맨 왼쪽 Y에 따른 nSizeX만큼 합 ------------
	for(i=top;i<bottom;i++)
	{	
		nSum=0;
		for(j=left;j<left+nSizeX;j++)
			nSum+=*(fmS+pitch*i+j);

		l_SumVer[i]=nSum;
	}
	//--------------------------------------------

	for(j=left+nHalfX;j<right-nHalfX;j++)
	{
		if(j!=left+nHalfX) //한칸씩 오른쪽으로 감에 따라 이 값도 변경
		{
			for(i=top;i<bottom;i++)
				l_SumVer[i]=l_SumVer[i]+*(fmS+pitch*i+j-nHalfX+nSizeX-1)-*(fmS+pitch*i+j-nHalfX-1);
		}

		nSum=0;
		for(i=top;i<top+nSizeY;i++)
			nSum+=l_SumVer[i];

		for(i=top+nHalfY;i<bottom-nHalfY;i++)
		{
			nTmp=128-(nSum-*(fmS+pitch*i+j)*(nSizeX*nSizeY+1))/(nSizeX*nSizeY);
			if(nTmp<0)		  nTmp=0;
			else if(nTmp>255) nTmp=255;
			*(fmD+pitch*i+j)=nTmp;

			nSum=nSum+l_SumVer[i-nHalfY+nSizeY]-l_SumVer[i-nHalfY];
		}
	}

	return 0;
}

int COSS_Mean(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY)
{
	int i, j;
	int nSum;
	int nHalfX, nHalfY;
	int nTmp;

	if(nSizeX%2==0) nSizeX++;
	if(nSizeY%2==0) nSizeY++;

	nHalfX=nSizeX/2;
	nHalfY=nSizeY/2;


	for(i=0;i<nSizeY/2;i++)
	{
		memcpy(fmD+pitch*(i+top)+left, fmS+pitch*(i+top)+left, (right-left));
		memcpy(fmD+pitch*(bottom-i-1)+left, fmS+pitch*(bottom-1-i)+left, (right-left));
	}
	for(j=0;j<nSizeX/2;j++)
	{
		for(i=top;i<bottom;i++)
		{
			*(fmD+pitch*i+left+j)=*(fmS+pitch*i+left+j);
			*(fmD+pitch*i+right-1-j)=*(fmS+pitch*i+right-1-j);
		}
	}

	//맨 왼쪽 Y에 따른 nSizeX만큼 합 ------------
	for(i=top;i<bottom;i++)
	{	
		nSum=0;
		for(j=left;j<left+nSizeX;j++)
			nSum+=*(fmS+pitch*i+j);

		l_SumVer[i]=nSum;
	}
	//--------------------------------------------

	for(j=left+nHalfX;j<right-nHalfX;j++)
	{
		if(j!=left+nHalfX) //한칸씩 오른쪽으로 감에 따라 이 값도 변경
		{
			for(i=top;i<bottom;i++)
				l_SumVer[i]=l_SumVer[i]+*(fmS+pitch*i+j-nHalfX+nSizeX-1)-*(fmS+pitch*i+j-nHalfX-1);
		}

		nSum=0;
		for(i=top;i<top+nSizeY;i++)
			nSum+=l_SumVer[i];

		for(i=top+nHalfY;i<bottom-nHalfY;i++)
		{
			nTmp=nSum/(nSizeX*nSizeY);
			if(nTmp<0)		  nTmp=0;
			else if(nTmp>255) nTmp=255;
			*(fmD+pitch*i+j)=nTmp;

			nSum=nSum+l_SumVer[i-nHalfY+nSizeY]-l_SumVer[i-nHalfY];
		}
	}

	return 0;
}

//Smoothing된걸로 채움
int Mean_1(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY)
{
	int i, j;
	int nSum;
	int nHalfX, nHalfY;
	int nTmp;

	if(nSizeX%2==0) nSizeX++;
	if(nSizeY%2==0) nSizeY++;

	nHalfX=nSizeX/2;
	nHalfY=nSizeY/2;




	//맨 왼쪽 Y에 따른 nSizeX만큼 합 ------------
	for(i=top;i<bottom;i++)
	{	
		nSum=0;
		for(j=left;j<left+nSizeX;j++)
			nSum+=*(fmS+pitch*i+j);

		l_SumVer[i]=nSum;
	}
	//--------------------------------------------

	for(j=left+nHalfX;j<right-nHalfX;j++)
	{
		if(j!=left+nHalfX) //한칸씩 오른쪽으로 감에 따라 이 값도 변경
		{
			for(i=top;i<bottom;i++)
				l_SumVer[i]=l_SumVer[i]+*(fmS+pitch*i+j-nHalfX+nSizeX-1)-*(fmS+pitch*i+j-nHalfX-1);
		}

		nSum=0;
		for(i=top;i<top+nSizeY;i++)
			nSum+=l_SumVer[i];

		for(i=top+nHalfY;i<bottom-nHalfY;i++)
		{
			nTmp=nSum/(nSizeX*nSizeY);
			if(nTmp<0)		  nTmp=0;
			else if(nTmp>255) nTmp=255;
			*(fmD+pitch*i+j)=nTmp;

			nSum=nSum+l_SumVer[i-nHalfY+nSizeY]-l_SumVer[i-nHalfY];
		}
	}

	for(i=0;i<nSizeY/2;i++)
	{
		memcpy(fmD+pitch*(i+top)+left, fmD+pitch*(i+nSizeY/2)+left, (right-left));
		memcpy(fmD+pitch*(bottom-i-1)+left, fmD+pitch*(bottom-1-nSizeY/2)+left, (right-left));
	}
	for(j=0;j<nSizeX/2;j++)
	{
		for(i=top;i<bottom;i++)
		{
			*(fmD+pitch*i+left+j)=*(fmD+pitch*i+left+nSizeX/2);
			*(fmD+pitch*i+right-1-j)=*(fmD+pitch*i+right-1-nSizeX/2);
		}
	}

	return 0;
}

void Filter2DFVer(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY, int nAbsolute, int nDivide)
{
	int i, j;
	int nSum, nSum1, nTmp;
	int nHalfX, nHalfY;

	if(nSizeX%2) nSizeX++;
	if(nSizeY%2) nSizeY++;

	nHalfX=nSizeX/2;
	nHalfY=nSizeY/2;


	//맨 왼쪽 Y에 따른 nSizeX만큼 합 ------------
	for(i=top;i<bottom;i++)
	{	
		nSum=0;
		for(j=left;j<left+nSizeX;j++)
			nSum+=*(fmS+pitch*i+j);

		l_SumVer[i]=nSum;
	}
	//--------------------------------------------


	for(j=left+nHalfX;j<right-nHalfX;j+=4)
	{
		if(j!=left+nHalfX) //한칸씩 오른쪽으로 감에 따라 이 값도 변경
		{
			for(i=top;i<bottom;i++)
				l_SumVer[i]=l_SumVer[i]+*(fmS+pitch*i+j-nHalfX+nSizeX-1)+*(fmS+pitch*i+j-nHalfX+nSizeX)+*(fmS+pitch*i+j-nHalfX+nSizeX+1)+*(fmS+pitch*i+j-nHalfX+nSizeX+2)
				-*(fmS+pitch*i+j-nHalfX-1)-*(fmS+pitch*i+j-nHalfX)-*(fmS+pitch*i+j-nHalfX+1)-*(fmS+pitch*i+j-nHalfX+2);
		}

		nSum=0;
		for(i=top;i<top+nHalfY;i++)
			nSum+=l_SumVer[i];

		nSum1=0;
		for(i=top+nHalfY;i<top+nSizeY;i++)
			nSum1+=l_SumVer[i];		


		for(i=top+nHalfY;i<bottom-nHalfY;i++)
		{
			nTmp=(nSum-nSum1)/nDivide;
			if(nAbsolute)
			{
				if(nTmp<0) nTmp*=-1;
				if(nTmp>255) nTmp=255;
			}
			else
			{
				if(nTmp<0)		  nTmp=0;
				else if(nTmp>255) nTmp=255;
			}

			*(fmD+pitch*i+j)  =nTmp;
			*(fmD+pitch*i+j+1)=nTmp;
			*(fmD+pitch*i+j+2)=nTmp;
			*(fmD+pitch*i+j+3)=nTmp;

			nSum =nSum+l_SumVer[i]-l_SumVer[i-nHalfY];
			nSum1=nSum1+l_SumVer[i+nHalfY]-l_SumVer[i];
		}
	}
}

/*
int BinaryDilate(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY)
{
	int i, j, ii, jj;
	int nSum;
	int nHalfX, nHalfY;

	if(nSizeX%2==0) nSizeX++;
	if(nSizeY%2==0) nSizeY++;

	nHalfX=nSizeX/2;
	nHalfY=nSizeY/2;

	for(i=top;i<bottom;i++)
		memset(fmD+pitch*i+left, 0, (right-left));
	
	for(j=left;j<right-nSizeX;j++)
		for(i=top;i<bottom-nSizeY;i++)
		{
			if(i==top)
			{
				nSum=0;
				for(ii=i;ii<i+nSizeY;ii++)
					for(jj=j;jj<j+nSizeX;jj++)
						nSum+=*(fmS+pitch*ii+jj);
			}
			else
			{
				for(jj=j;jj<j+nSizeX;jj++)
					nSum+=(*(fmS+pitch*(i+nSizeY-1)+jj)-*(fmS+pitch*(i-1)+jj));
			}

			if(nSum) *(fmD+pitch*(i+nHalfY)+j+nHalfX)=255;
		}
	
	return 0;
}

//흰점기준(흰점이 커지게) : 기존보다 적게 걸림.
int BinaryDilate(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY)
{
	int i, j, ii, jj;
	int nSum;
	int nHalfX, nHalfY;

	if(nSizeX%2==0) nSizeX++;
	if(nSizeY%2==0) nSizeY++;

	nHalfX=nSizeX/2;
	nHalfY=nSizeY/2;

	for(i=top;i<bottom;i++)
		memset(fmD+pitch*i+left, 0, (right-left));
	
	for(i=top;i<bottom-nSizeY;i++)
		for(j=left;j<right-nSizeX;j++)
		{
			if(j==left)
			{
				nSum=0;
				for(ii=i;ii<i+nSizeY;ii++)
					for(jj=j;jj<j+nSizeX;jj++)
						nSum+=*(fmS+pitch*ii+jj);
			}
			else
			{
				for(ii=i;ii<i+nSizeY;ii++)
					nSum+=(*(fmS+pitch*ii+j+nSizeX-1)-*(fmS+pitch*ii+j-1));
			}

			if(nSum) *(fmD+pitch*(i+nHalfY)+j+nHalfX)=255;
		}
	
	return 0;
}
*/


// 이함수가 위보다 0.15msec 덜걸림(128x128)기준
int BinaryDilate(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY)
{
	int i, j;
	int nSum;
	int nHalfX, nHalfY;
//	int nTmp;

	if(nSizeX%2==0) nSizeX++;
	if(nSizeY%2==0) nSizeY++;

	nHalfX=nSizeX/2;
	nHalfY=nSizeY/2;


	for(i=top;i<bottom;i++)
		memset(fmD+pitch*i+left, 0, (right-left));

	//맨 왼쪽 Y에 따른 nSizeX만큼 합 ------------
	for(i=top;i<bottom;i++)
	{	
		nSum=0;
		for(j=left;j<left+nSizeX;j++)
			nSum+=*(fmS+pitch*i+j);

		l_SumVer[i]=nSum;
	}
	//--------------------------------------------

	for(j=left+nHalfX;j<right-nHalfX;j++)
	{
		if(j!=left+nHalfX) //한칸씩 오른쪽으로 감에 따라 이 값도 변경
		{
			for(i=top;i<bottom;i++)
				l_SumVer[i]=l_SumVer[i]+*(fmS+pitch*i+j-nHalfX+nSizeX-1)-*(fmS+pitch*i+j-nHalfX-1);
		}

		nSum=0;
		for(i=top;i<top+nSizeY;i++)
			nSum+=l_SumVer[i];

		for(i=top+nHalfY;i<bottom-nHalfY;i++)
		{
			if(nSum) *(fmD+pitch*i+j)=255;

			nSum=nSum+l_SumVer[i-nHalfY+nSizeY]-l_SumVer[i-nHalfY];
		}
	}

	return 0;
}


//흰점기준(흰점이 작아지게)
int BinaryErode(LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nSizeX, int nSizeY)
{
	int i, j;
	int nSum;
	int nHalfX, nHalfY;
//	int nTmp;
	int nStdValue;

	if(nSizeX%2==0) nSizeX++;
	if(nSizeY%2==0) nSizeY++;

	nHalfX=nSizeX/2;
	nHalfY=nSizeY/2;


	nStdValue=255*nSizeX*nSizeY;

	for(i=top;i<bottom;i++)
		memset(fmD+pitch*i+left, 255, (right-left));

	//맨 왼쪽 Y에 따른 nSizeX만큼 합 ------------
	for(i=top;i<bottom;i++)
	{	
		nSum=0;
		for(j=left;j<left+nSizeX;j++)
			nSum+=*(fmS+pitch*i+j);

		l_SumVer[i]=nSum;
	}
	//--------------------------------------------

	for(j=left+nHalfX;j<right-nHalfX;j++)
	{
		if(j!=left+nHalfX) //한칸씩 오른쪽으로 감에 따라 이 값도 변경
		{
			for(i=top;i<bottom;i++)
				l_SumVer[i]=l_SumVer[i]+*(fmS+pitch*i+j-nHalfX+nSizeX-1)-*(fmS+pitch*i+j-nHalfX-1);
		}

		nSum=0;
		for(i=top;i<top+nSizeY;i++)
			nSum+=l_SumVer[i];

		for(i=top+nHalfY;i<bottom-nHalfY;i++)
		{
			if(nSum!=nStdValue) *(fmD+pitch*i+j)=0;

			nSum=nSum+l_SumVer[i-nHalfY+nSizeY]-l_SumVer[i-nHalfY];
		}
	}

	return 0;
}


int FindImageAngle(LPBYTE fm,  int left, int top, int right, int bottom, int pitch, double *angle)
{
	int ok=1;
	int i,j;
	int sum, nData;
	int nThreshold;
	double sx,sy, angtmp, ang, norm;
	int px,py;
	double sumx=0,sumy=0;

	for(i=top+1;i<bottom-1;i++)
		for(j=left+1;j<right-1;j++)	{
			sx=*(fm+pitch*(i-1)+j+1)-*(fm+pitch*(i-1)+j-1)+	
				2*(*(fm+pitch*(i)+j+1)-*(fm+pitch*(i)+j-1))+	
				*(fm+pitch*(i+1)+j+1)-*(fm+pitch*(i+1)+j-1);
			sy=*(fm+pitch*(i+1)+j-1)-*(fm+pitch*(i-1)+j-1)+	
				2*(*(fm+pitch*(i+1)+j)-*(fm+pitch*(i-1)+j))+	
				*(fm+pitch*(i+1)+j+1)-*(fm+pitch*(i-1)+j+1);

			norm=sqrt(sx*sx+sy*sy);
			angtmp=atan2(sy,sx);
			angtmp=angtmp*180/3.141592;
			if(angtmp>5) continue;
			if(angtmp<-5) continue;

			ang=4*angtmp;

			sx=norm*cos(ang);
			sy=norm*sin(ang);

			sumx+=sx;
			sumy+=sy;
		}

	*angle=(float)((atan(sumy/sumx)/4.0)*180/3.141592);
	ok=0;

done:
	return ok;
} 


void GetImageLUT(int nMasterTH, int nMasterMaxL, int nMasterMaxR,  int *HistoMaster, int *Histo, LPBYTE LUT)
{
	int i,nImageTH;
	int nProfile[30];
	int nX1, nX2, nOffSet;
	int nMax, nMaxL, nMaxR;
	int nLUT[256];
	double dCorr, dMaxCorr;
	double dMasterGap, dGap, dRatio;

	nImageTH=AutoThresholdFromHisto(Histo);
	for(i=0;i<256;i++) nLUT[i]=i;
	if(nMasterMaxR-nMasterMaxL>50)
	{
		nMax=0;
		for(i=0;i<nImageTH;i++)						
			if(Histo[i]>nMax) {nMax=Histo[i]; nMaxL=i;} 
		nMax=0;
		for(i=nImageTH+1;i<256;i++)						
			if(Histo[i]>nMax) {nMax=Histo[i]; nMaxR=i;} 

		if(nMaxR>nMaxL>30)
		{
			dMasterGap=nMasterTH-nMasterMaxL;
			dGap	  =nImageTH -nMaxL;
			dRatio    =dMasterGap/dGap;
			for(i=0;i<nImageTH;i++)
			{
				nLUT[i]=nImageTH-(nImageTH-i)*dRatio;
			}

			dMasterGap=nMasterMaxR-nMasterTH;
			dGap	  =nMaxR	  -nImageTH;
			dRatio    =dMasterGap/dGap;
			for(i=nImageTH;i<256;i++)
			{
				nLUT[i]=nImageTH+(i-nImageTH)*dRatio;
			}
		}
	}
	

	for(i=0;i<256;i++)
	{
		nLUT[i]=nLUT[i]+(nMasterTH-nImageTH);
		if(nLUT[i]<0) nLUT[i]=0;
		else if(nLUT[i]>255) nLUT[i]=255;
	}

	for(i=0;i<256;i++)
		LUT[i]=nLUT[i];
}


int AutoThresholdFromHisto(int m_nHistoData[256])
{
	int i,j;
	const int GRAYLEVEL=256;
	int  m_nArea=0,nSum=0;
	float m_dU;     /* Average */
	int nCount=0, Pos[3];
	int nThreshold;
	float pt, qt, U1, U2, qtPrev;
	float SigmaB2, MaxSigmaB2=0;
	int leftX, rightX;
	float x1,x2;
	float a[4],b[4], minY, maxY;
 
	for(i=0;i<GRAYLEVEL;i++)
	{
		nSum+=i*m_nHistoData[i];
		m_nArea+=m_nHistoData[i];
	}

	/* 면적과 평균을 구한다. */
	m_dU   =(float)nSum/(float)m_nArea;

	/* Histogram의 데이타가 NONZERO인 곳이 1,2있을때>>>>>>>>*/
	for(i=0;i<GRAYLEVEL;i++)
		if(m_nHistoData[i])
		{
			Pos[nCount]=i;
			nCount++;
			if(nCount>2) break;
		}
//	if(nCount==1) return -2;
	if(nCount==1) return 128;
	else if(nCount==2) return (Pos[0]+Pos[1])/2;
	/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/


	qt=(float)m_nHistoData[0]/(float)m_nArea;
	if(qt==1.0) return 0;

	U1   = 0;
	U2   = (m_dU-qt*U1)/(1.0-qt);

	for(i=0;i<GRAYLEVEL-1;i++)
	{
		SigmaB2=qt*(1-qt)*(U2-U1)*(U2-U1);
		if(SigmaB2 > MaxSigmaB2 )
		{
			nThreshold=i;
			MaxSigmaB2=SigmaB2;
        }
		qtPrev=qt;
		pt=(float)m_nHistoData[i+1]/(float)m_nArea;
		qt=qtPrev+pt;
		if(qt==0)         continue;
		else if(qt>0.99999)  break;    /*qt가 1 이면 더이상은 의미가 없으므로 Break함*/
		U1=(qtPrev*U1+(i+1)*pt)/qt;
		U2=(m_dU-qt*U1)/(1-qt);		 
	}

	return nThreshold;
}

//return 1: OK

//fmS : Source Image 시작번지
// (left,top)-(right-bottom) : 회전할 영역 
// nPitch: Source Image의 영상폭 
// xLimit, yLimit : 영상의 최대 범위 

//(Cx, Cy) : 회전 중심 
//xOff, yOff : 영상의 이동  Offset
//angle : 영상의 회전 각도 
//16k x 16 k : (Release : 2471 msec, Debug: 5119 msec)
int MakeRotateImage(LPBYTE fmS,  LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int xLimit, int yLimit, double Cx, double Cy, double xOff,double yOff, double angle)
{
	int i,j,N,M, count=0;
	int Dum, nOffsetAddr;
	double a,b,pX,pY;
	double cosAngle, sinAngle;	
	LPBYTE  fmT;
	int width=right-left;
	int height=bottom-top;

	right += (int)(xOff);
	left += (int)(xOff);

	bottom += (int)yOff;
	top += (int)(yOff);

	if(left<0)              {left=0; right=left+width;}
	else if(right>xLimit)   {right=xLimit; left=right-width;}
	if(top<0)               {top = 0; bottom=top+height;}
	else if(bottom>yLimit)  {bottom=yLimit; top=bottom-height;}

    cosAngle=cos(angle*3.141592653589/180.0);
    sinAngle=sin(angle*3.141592653589/180.0);

	for(i=top;i<bottom;i++)
	{
	   for(j=left;j<right;j++)
       {  		    
            pX= cosAngle*(j-Cx)+sinAngle*(i-Cy)+Cx-xOff;
			pY=-sinAngle*(j-Cx)+cosAngle*(i-Cy)+Cy-yOff;

			N=(int)pX;
			M=(int)pY;
			a=pX-N;
			b=pY-M;
			if(N==right)  N=right-1; 
			if(M==bottom) M=bottom-1;

			if(N<0 || N>= xLimit-1 || M<0 || M>= yLimit-1)
			{
				*(fmD+pitch*i+j)=0;
			}
			else
			{
				Dum=M*pitch+N;
				*(fmD+pitch*i+j)=*(fmS+Dum)*(1.0-a)*(1.0-b)+
											*(fmS+Dum+1)*(a)*(1.0-b)+
											*(fmS+Dum+pitch)*(1.0-a)*(b)+
											*(fmS+Dum+pitch+1)*(a)*(b);
			}
       }
	}

    return 1;
}

//16k x 16 k : (Release : 1946 msec, Debug: 4014 msec)
void Rotate(LPBYTE dest, LPBYTE src, int width, int height, float x0,float y0,float angle)
{	
	int i,j;
	double costheta,sintheta,x,y;
	double cxf,cyf,cxsf,cysf;
	double yc,ys;
	int ix2, iy2;
	double x2, y2;	/* (x2,y2) and (ix2,iy2) are coordinates of source */
	double dx1,dy1,dx2,dy2;
	LPBYTE d=dest, s;
	
	costheta=cos(angle*3.141592653589/180.0);
	sintheta=sin(angle*3.141592653589/180.0);
	
	cxf=width/2;			/* center of destination image */
	cyf=height/2;
	cxsf=x0;
	cysf=y0;

	/* (x,y) and (i,j) are coordinates of destination */
	for (y=-cyf,i=0; i<height; i++,y++,d+=width)	
	{	
		
		ys=cxsf-y*sintheta;
		yc=cysf+y*costheta;
		for (x=-cxf,j=0; j<width; j++,x++)
		{	
			iy2=(int)(y2=x*sintheta+yc);
			if ((iy2>0)&&(iy2<height-1))			/* check out-of-range */
			{					
				ix2=(int)(x2=x*costheta+ys);
				if ((ix2>0)&&(ix2<width-1))		/* check out-of-range */
				{						
					s=src+iy2*width+ix2;
					dx1=x2-ix2;					/* distances to near pixels */
					dy1=y2-iy2;
					dx2=1.-dx1;
					dy2=1.-dy1;
					d[j]=((s[0]*dx2+s[1]*dx1)*dy2+(s[width]*dx2+s[width+1]*dx1)*dy1);
				}
				else d[j]=0;
			}
			else d[j]=0;
		}
	}
}


//-------------------------------------------------------------------------
//서치할때 nW의 반을 겹쳐서 하고 겹친부분은 한번만 계산함으로써 계산시간을 1/2로 줄임.
//(nW, nH)의 Stdev MAP을 구하는 함수
int GetStdevMap(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, int nW, int nH, double *pStdev, int *nSW, int *nSH)
{
	int i,j, ii, jj;
	double stdev=0, dSumValue=0; 
	long nArea, tmp, nSum, nSum_;
	double nSum2, nSum2_;
	int nX, nY, nSPitch=0, nMapCount=0;

	if(nW%2) nW--;
	if(nH%2) nH--;
	for(j=x1;j<=x2-nW;j+=nW/2) nSPitch++;

	nArea=nW*nH;
	
	nY=0;
	for(i=y1;i<=y2-nH;i+=nH/2, nY++)
	{	
		//nW/2, nH 영역만 먼저구함.
		nSum=nSum2=0;
		for(ii=i;ii<i+nH;ii++)
			for(jj=x1;jj<x1+nW/2;jj++)
			{
				tmp=*(fm+pitch*ii+jj);
				nSum+=tmp;
				nSum2+=(tmp*tmp);
			}


		nX=0;
		for(j=x1;j<=x2-nW;j+=nW/2, nX++)
		{
			nSum_=nSum2_=0;
			for(ii=i;ii<i+nH;ii++)
				for(jj=j+nW/2;jj<j+nW;jj++)
				{
					tmp=*(fm+pitch*ii+jj);
					nSum_+=tmp;
					nSum2_+=(tmp*tmp);
				}					

			if(nSum+nSum_>0)
			{
				*(pStdev+nSPitch*nY+nX)=sqrt(((double)nArea*(nSum2+nSum2_)-((double)nSum+(double)nSum_)*((double)nSum+(double)nSum_))/((double)nArea*((double)nArea-1.)));	
			}
			else
			{
				*(pStdev+nSPitch*nY+nX)=0;
			}

			dSumValue+=(nSum+nSum_)/(nW*nH);
			nMapCount++;

			nSum=nSum_;
			nSum2=nSum2_;
		}
	}

	*nSW=nX;
	*nSH=nY;
	
	if(nMapCount) return (int)(dSumValue/nMapCount);
	else		  return 0;
}


int GetAvgMap(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, int nW, int nH, double *pStdev, int *nSW, int *nSH)
{
	int i,j, ii, jj;
	double stdev=0, dSumValue=0; 
	long nArea, tmp, nSum, nSum_;
	double nSum2, nSum2_;
	int nX, nY, nSPitch=0, nMapCount=0;

	if(nW%2) nW--;
	if(nH%2) nH--;
	for(j=x1;j<=x2-nW;j+=nW/2) nSPitch++;

	nArea=nW*nH;
	
	nY=0;
	for(i=y1;i<=y2-nH;i+=nH/2, nY++)
	{	
		//nW/2, nH 영역만 먼저구함.
		nSum=nSum2=0;
		for(ii=i;ii<i+nH;ii++)
			for(jj=x1;jj<x1+nW/2;jj++)
			{				
				tmp=*(fm+pitch*ii+jj);
				nSum+=tmp;				
			}


		nX=0;
		for(j=x1;j<=x2-nW;j+=nW/2, nX++)
		{
			nSum_=nSum2_=0;
			for(ii=i;ii<i+nH;ii++)
				for(jj=j+nW/2;jj<j+nW;jj++)
				{
					tmp=*(fm+pitch*ii+jj);				
					nSum_+=tmp;					
				}					

			if(nSum+nSum_>0)
			{
				*(pStdev+nSPitch*nY+nX)=(double)(nSum+nSum_)/(double)nArea;	
			}
			else
			{
				*(pStdev+nSPitch*nY+nX)=0;
			}

			dSumValue+=(nSum+nSum_)/(nW*nH);
			nMapCount++;

			nSum=nSum_;			
		}
	}

	*nSW=nX;
	*nSH=nY;
	
	if(nMapCount) return (int)(dSumValue/nMapCount);
	else		  return 0;
}

void ImageFlipHor(LPBYTE fm, int left, int top, int right, int bottom, int pitch)
{
	int i, j;
	BYTE nTmp;
	int nHalf=(bottom-top+1)/2;

	for(i=top;i<top+nHalf;i++)
		for(j=left;j<right;j++)
		{
			nTmp=*(fm+pitch*i+j);
			*(fm+pitch*i+j)=*(fm+pitch*(bottom-(i-top))+j);
			*(fm+pitch*(bottom-(i-top))+j)=nTmp;
		}
}
void ImageFlipVer(LPBYTE fm, int left, int top, int right, int bottom, int pitch)
{
	int i, j;
	BYTE nTmp;
	int nHalf=(right-left+1)/2;

	for(i=top;i<bottom;i++)
		for(j=left;j<left+nHalf;j++)
		{
			nTmp=*(fm+pitch*i+j);
			*(fm+pitch*i+j)=*(fm+pitch*i+(right-(j-left)));
			*(fm+pitch*i+(right-(j-left)))=nTmp;
		}
}
void quick_sort(BYTE list[], int left, int right);

int Get2Size(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nCoreColor, int nCloudColor, double dAvg, int nCoreThW, int nCoreThB, double dCloudThW, double dCloudThB, int *Cx, int *Cy, int *pValue, int *CoreSize,  double *dCloudValue, int *CloudSize)
{
	int ret=0;
	int i,j, loop, nTmp, nSum, nCnt, flagWhiteTry;
	int nULimit;	
	int nDLimit;
	int nCloudUpTh, nCloudDnTh;
	int ll,tt,rr,bb;
	int ll_,tt_,rr_,bb_;
	int nBlob, nMaxPos, nMaxPos1, nContour, nMaxContour, nMinValue=255, nMaxValue=0;
	int nBoundary;
	int nDataCount, *pixel;
	int nMax, nMinPos, nMinPosEnd, nMaxPosEnd, nLength, nPerimeter;
	double dMin, dMinEnd=0, dMax, dMax1, dMaxEnd, dBlackSize=0, dBlackValue=0, dWhiteSize=0, dWhiteValue=0;
	int nMaxDataCount=35;
	double dData[50];
	double dEdgeX[50],  dEdgeY[50];
	int nWidth=128, nHeight=128;
	BYTE nBoundData[1024];
	
	if(dCloudThB<1.5) dCloudThB=1.5;
	if(dCloudThW<3)   dCloudThW=3;
	if(nCoreThW<20)   nCoreThW=20;
	if(nCoreThB<20)   nCoreThB=20;

	*Cx=*Cy=*pValue=*CoreSize=*dCloudValue=*CloudSize=0;
	
	nULimit=dAvg+nCoreThW;
	nDLimit=dAvg-nCoreThB;

	//Core 구하기(3x3 Smoothing)-----------------------------------------------------------------------------
	for(i=top+1;i<bottom-1;i++)
		for(j=left+1;j<right-1;j++)
			*(g_fmDefectGray+pitch*(i-top)+(j-left))=(*(fm+pitch*(i-1)+j-1)+*(fm+pitch*(i-1)+j)+*(fm+pitch*(i-1)+j+1)+*(fm+pitch*i+j-1)+*(fm+pitch*i+j)+*(fm+pitch*i+j+1)+*(fm+pitch*(i+1)+j-1)+*(fm+pitch*(i+1)+j)+*(fm+pitch*(i+1)+j+1)+4)/9;


	if(nCoreColor==1) //코어 백
	{
		for(i=1;i<nHeight-1;i++)
			for(j=1;j<nWidth-1;j++)
				if(*(g_fmDefectGray+pitch*i+j)>nULimit)	*(g_fmDefectBinary+pitch*i+j)=255;
				else								    *(g_fmDefectBinary+pitch*i+j)=0;
	}
	else if(nCoreColor==0) //코어 흑
	{
		for(i=1;i<nHeight-1;i++)
			for(j=1;j<nWidth-1;j++)
				if(*(g_fmDefectGray+pitch*i+j)<nDLimit)	*(g_fmDefectBinary+pitch*i+j)=255;
				else								    *(g_fmDefectBinary+pitch*i+j)=0;
	}
	else if(nCoreColor==2) //코어 흑 또는 백
	{
		for(i=1;i<nHeight-1;i++)
			for(j=1;j<nWidth-1;j++)
				if(*(g_fmDefectGray+pitch*i+j)>nULimit || *(g_fmDefectGray+pitch*i+j)<nDLimit)	*(g_fmDefectBinary+pitch*i+j)=255;
				else																			*(g_fmDefectBinary+pitch*i+j)=0;
	}

	g_Chain.SetChainData(1, g_fmDefectBinary, 1, 1, 1, 2000, nWidth, nHeight);
	nBlob=g_Chain.FastChain(0,0,nWidth-1,nHeight-1);
	nMaxPos=-1;
	nMaxContour=0;
	for(i=0;i<nBlob;i++)
	{
		nContour=g_Chain.Chain_Area(i);
		if(nContour>nMaxContour)
		{
			nMaxContour=nContour;
			nMaxPos=i;
		}
	}
	if(nMaxPos>=0)
	{
		ll	= g_Chain.FindMinX(nMaxPos);
		rr	= g_Chain.FindMaxX(nMaxPos);
		tt	= g_Chain.FindMinY(nMaxPos);
		bb	= g_Chain.FindMaxY(nMaxPos);

		if(nCoreColor==1) //코어 백
		{
			nMaxValue=0;
			for(i=tt;i<bb;i++)
				for(j=ll;j<rr;j++)
				{
					if(*(g_fmDefectGray+BAD_IMG_WIDTH*i+j)>nMaxValue) nMaxValue=*(g_fmDefectGray+pitch*i+j);
				}		
				*pValue=nMaxValue-dAvg;
		}
		else if(nCoreColor==0) //코어 흑
		{
			nMinValue=255;
			for(i=tt;i<bb;i++)
				for(j=ll;j<rr;j++)
				{
					if(*(g_fmDefectGray+BAD_IMG_WIDTH*i+j)<nMinValue) nMinValue=*(g_fmDefectGray+pitch*i+j);
				}		
				*pValue=dAvg-nMinValue;
		}
		else if(nCoreColor==2) //코어 흑 또는 백
		{
			nMaxValue=0;
			nMinValue=255;
			for(i=tt;i<bb;i++)
				for(j=ll;j<rr;j++)
				{
					if(*(g_fmDefectGray+BAD_IMG_WIDTH*i+j)>nMaxValue) nMaxValue=*(g_fmDefectGray+pitch*i+j);
					if(*(g_fmDefectGray+BAD_IMG_WIDTH*i+j)<nMinValue) nMinValue=*(g_fmDefectGray+pitch*i+j);
				}	
		
			if(nMaxValue-dAvg>dAvg-nMinValue)  *pValue=nMaxValue-dAvg;
			else							   *pValue=nMinValue-dAvg;
		}

		*Cx=(ll+rr)/2+1;
		*Cy=(tt+bb)/2+1;
		if(rr-ll>bb-tt) *CoreSize=(rr-ll+1);
		else			*CoreSize=(bb-tt+1);
	}
	else
	{
		return -1;
	}

	//----------------------------------------------------------------------------------------------------
	nDataCount=0;
	for(loop=0;loop<nMaxDataCount;loop++)
	{
		memset(g_fmDefectBinary, 0, 128*128);
		nBoundary=g_Chain.GetChainDataNumber(nMaxPos);
		pixel=g_Chain.GetChainData(nMaxPos);
		for(i=0;i<nBoundary;i++)
		{
			*(g_fmDefectBinary+128*(pixel[2*i+1]-1)+(pixel[2*i]-1))	=255;
			*(g_fmDefectBinary+128*(pixel[2*i+1]-1)+pixel[2*i])		=255;
			*(g_fmDefectBinary+128*(pixel[2*i+1]-1)+pixel[2*i]+1)	=255;
			*(g_fmDefectBinary+128*pixel[2*i+1]+pixel[2*i]-1)		=255;
			*(g_fmDefectBinary+128*pixel[2*i+1]+pixel[2*i])			=255;
			*(g_fmDefectBinary+128*pixel[2*i+1]+pixel[2*i]+1)		=255;
			*(g_fmDefectBinary+128*(pixel[2*i+1]+1)+pixel[2*i]-1)	=255;
			*(g_fmDefectBinary+128*(pixel[2*i+1]+1)+pixel[2*i])		=255;
			*(g_fmDefectBinary+128*(pixel[2*i+1]+1)+pixel[2*i]+1)	=255;
		}

		ll_=ll-2-loop; if(ll_<2)   ll_=2;
		tt_=tt-2-loop; if(tt_<2)   tt_=2;
		rr_=rr+2+loop; if(rr_>125) rr_=125;
		bb_=bb+2+loop; if(bb_>125) bb_=125;

		nBlob=g_Chain.FastChain(ll_,tt_,rr_,bb_);
		nMax=0;
		for(i=0;i<nBlob;i++)
		{
			nLength=g_Chain.Chain_Area(i);
			if(nLength>nMax)
			{
				nMax=nLength;
				nMaxPos=i;
			}
		}
		if(nMax==0) 
			break;
		

		nPerimeter=g_Chain.GetChainDataNumber(nMaxPos);
		if(nPerimeter>400) nPerimeter=400;
		pixel=g_Chain.GetChainData(nMaxPos);
		nBoundary=0;
		for(i=0;i<nPerimeter;i++)
		{
			if(pixel[2*i]<2 || pixel[2*i]>125 || pixel[2*i+1]<2 || pixel[2*i+1]>125) continue;

			nTmp=*(g_fmDefectGray+128*pixel[2*i+1]+pixel[2*i]);
			if(nTmp>=nDLimit && nTmp<=nULimit)
				nBoundData[nBoundary++]=*(g_fmDefectGray+128*pixel[2*i+1]+pixel[2*i]);	
		}
		quick_sort(nBoundData, 0, nBoundary-1);
		nSum=nCnt=0;
		for(i=nBoundary/4;i<nBoundary*3/4;i++)
		{
			nSum+=nBoundData[i];
			nCnt++;
		}
		if(nCnt==0) break;
		dData[nDataCount++]=(double)nSum/nCnt;
	}	

	//얼룩 검출----------------------------------------
	dMinEnd=0;
	for(i=nDataCount-10;i<nDataCount;i++)
		dMinEnd+=dData[i];
	dMinEnd*=0.1;

	if(nCloudColor==1 || nCloudColor==2) //구름띠 WHITE
	{
		nMaxPosEnd=0;
		for(i=nDataCount-1;i>3;i--)
		{
			if(dData[i]>dMinEnd+dCloudThW)
			{
				nMaxPosEnd=i;
				break;
			}
		}
		if(nMaxPosEnd>3)
		{
			dMaxEnd=nCnt=0;
			dMin=255;
			dMax=dMax1=0;
			for(i=1;i<=nMaxPosEnd;i++)
			{
				if(dData[i]<=dMin)
					dMin=dData[i];

				if(dData[i]>dMin+1)
				{
					if(dData[i]>dMax)
						dMax=dData[i];
				}
			}
			if(dMax<1)
			{
				for(i=4;i<=nMaxPosEnd;i++)
					if(dData[i]>dMax)
						dMax=dData[i];
			}	

			if(dMax>0)
			{
				dWhiteValue=dMax-(dMinEnd+1);
				dWhiteSize=*CoreSize+2*(nMaxPosEnd+1);	
			}
			if(dWhiteValue>=dCloudThW)
			{
				*dCloudValue=dWhiteValue;
				*CloudSize  =dWhiteSize;
			}
		}
	}
	if(nCloudColor==0 || nCloudColor==2) //흑 구름띠
	{
		nMaxPosEnd=0;
		for(i=nDataCount-1;i>3;i--)
		{
			if(dData[i]<dMinEnd-dCloudThB)
			{
				nMaxPosEnd=i;
				break;
			}
		}
		if(nMaxPosEnd>3)
		{
			dMaxEnd=nCnt=0;
			dMin=0;
			dMax=dMax1=255;
			for(i=1;i<=nMaxPosEnd;i++)
			{
				if(dData[i]>=dMin)
					dMin=dData[i];

				if(dData[i]<dMin+1)
				{
					if(dData[i]<dMax)
						dMax=dData[i];
				}
			}
			if(dMax<1)
			{
				for(i=4;i<=nMaxPosEnd;i++)
					if(dData[i]<dMax)
						dMax=dData[i];
			}	

			if(dMax>0)
			{
				dBlackValue=(dMinEnd+1)-dMax;
				dBlackSize=*CoreSize+2*(nMaxPosEnd+1);	
			}
			if(dBlackValue>=dCloudThB)
			{
				*dCloudValue=dBlackValue;
				*CloudSize  =dBlackSize;
			}
		}
	}

	if(nCloudColor==2)
	{
		if(dWhiteSize>dBlackSize)
		{
			*dCloudValue=dWhiteValue;
			*CloudSize  =dWhiteSize;
		}
		else
		{
			*dCloudValue=-dBlackValue;
			*CloudSize  =dBlackSize;
		}
	}


	return ret;
}

int GetHBOSize(LPBYTE fm, int left, int top, int right, int bottom, int pitch, double dAvg, int nCoreTh, double dCloudUpTh, int *IsWhiteSpot, int *Cx, int *Cy, int *pValue, int *CoreSize,  double *dCloudValue, int *CloudSize)
{
	int ret=0;
	int i,j, loop, nTmp, nSum, nCnt, flagWhiteTry;
	int nULimit;	
	int nDLimit;
	int nUpTh, nCloudUpTh, nCloudDnTh;
	int ll,tt,rr,bb;
	int ll_,tt_,rr_,bb_;
	int nBlob, nMaxPos, nMaxPos1, nContour, nMaxContour, nMinValue=255, nMaxValue=0;
	int nBoundary;
	int nDataCount, *pixel;
	int nMax, nMinPos, nMinPosEnd, nMaxPosEnd, nLength, nPerimeter;
	double dMin, dMinEnd=0, dMax, dMax1, dMaxEnd, dBlackSize=0, dBlackValue=0, dWhiteSize=0, dWhiteValue=0;
	int nMaxDataCount=35;
	double dDataDn[50],dDataUp[50];
	double dEdgeX[50],  dEdgeY[50];
	BYTE nBoundData[1024];
	if(dCloudUpTh<3) dCloudUpTh=3;
	*Cx=*Cy=*pValue=*CoreSize=*dCloudValue=*CloudSize=*IsWhiteSpot=0;
	
	nUpTh=dAvg+nCoreTh;
	nULimit=dAvg+nCoreTh;
	nDLimit=dAvg-20;

	//Core 구하기-----------------------------------------------------------------------------
	for(i=1;i<128-1;i++)
		for(j=1;j<128-1;j++)
			*(g_fmDefectGray+pitch*i+j)=(*(fm+pitch*(i-1)+j-1)+*(fm+pitch*(i-1)+j)+*(fm+pitch*(i-1)+j+1)+*(fm+pitch*i+j-1)+*(fm+pitch*i+j)+*(fm+pitch*i+j+1)+*(fm+pitch*(i+1)+j-1)+*(fm+pitch*(i+1)+j)+*(fm+pitch*(i+1)+j+1)+4)/9;

	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
			if(*(g_fmDefectGray+pitch*i+j)>nUpTh)	*(g_fmDefectBinary+pitch*i+j)=255;
			else								    *(g_fmDefectBinary+pitch*i+j)=0;

	g_Chain.SetChainData(1, g_fmDefectBinary, 1, 1, 1, 2000, BAD_IMG_WIDTH, BAD_IMG_HEIGHT);
	nBlob=g_Chain.FastChain(left,top,right-1,bottom-1);
	nMaxPos=-1;
	nMaxContour=0;
	for(i=0;i<nBlob;i++)
	{
		nContour=g_Chain.GetChainDataNumber(i);
		if(nContour>nMaxContour)
		{
			nMaxContour=nContour;
			nMaxPos=i;
		}
	}
	if(nMaxPos>=0)
	{
		ll	= g_Chain.FindMinX(nMaxPos);
		rr	= g_Chain.FindMaxX(nMaxPos);
		tt	= g_Chain.FindMinY(nMaxPos);
		bb	= g_Chain.FindMaxY(nMaxPos);

		nMaxValue=0;
		for(i=tt;i<bb;i++)
			for(j=ll;j<rr;j++)
			{
				if(*(g_fmDefectGray+BAD_IMG_WIDTH*i+j)>nMaxValue) nMaxValue=*(g_fmDefectGray+pitch*i+j);
			}
		*pValue=nMaxValue-dAvg;

		*Cx=(ll+rr)/2+1;
		*Cy=(tt+bb)/2+1;
		if(rr-ll>bb-tt) *CoreSize=(rr-ll+1);
		else			*CoreSize=(bb-tt+1);
	}
	else
	{
		return -1;
	}

	//----------------------------------------------------------------------------------------------------
	nDataCount=0;
	for(loop=0;loop<nMaxDataCount;loop++)
	{
		memset(g_fmDefectBinary, 0, 128*128);
		nBoundary=g_Chain.GetChainDataNumber(nMaxPos);
		pixel=g_Chain.GetChainData(nMaxPos);
		for(i=0;i<nBoundary;i++)
		{
			*(g_fmDefectBinary+128*(pixel[2*i+1]-1)+(pixel[2*i]-1))	=255;
			*(g_fmDefectBinary+128*(pixel[2*i+1]-1)+pixel[2*i])		=255;
			*(g_fmDefectBinary+128*(pixel[2*i+1]-1)+pixel[2*i]+1)	=255;
			*(g_fmDefectBinary+128*pixel[2*i+1]+pixel[2*i]-1)		=255;
			*(g_fmDefectBinary+128*pixel[2*i+1]+pixel[2*i])			=255;
			*(g_fmDefectBinary+128*pixel[2*i+1]+pixel[2*i]+1)		=255;
			*(g_fmDefectBinary+128*(pixel[2*i+1]+1)+pixel[2*i]-1)	=255;
			*(g_fmDefectBinary+128*(pixel[2*i+1]+1)+pixel[2*i])		=255;
			*(g_fmDefectBinary+128*(pixel[2*i+1]+1)+pixel[2*i]+1)	=255;
		}

		ll_=ll-2-loop; if(ll_<1)   ll_=2;
		tt_=tt-2-loop; if(tt_<1)   tt_=1;
		rr_=rr+2+loop; if(rr_>125) rr_=125;
		bb_=bb+2+loop; if(bb_>125) bb_=125;

		nBlob=g_Chain.FastChain(ll_,tt_,rr_,bb_);
		nMax=0;
		for(i=0;i<nBlob;i++)
		{
			nLength=g_Chain.Chain_Area(i);
			if(nLength>nMax)
			{
				nMax=nLength;
				nMaxPos=i;
			}
		}
		if(nMax==0) 
			break;
		

		nPerimeter=g_Chain.GetChainDataNumber(nMaxPos);
		if(nPerimeter>400) nPerimeter=400;
		pixel=g_Chain.GetChainData(nMaxPos);
		nBoundary=0;
		for(i=0;i<nPerimeter;i++)
		{
			if(pixel[2*i]<2 || pixel[2*i]>125 || pixel[2*i+1]<2 || pixel[2*i+1]>125) continue;

			nTmp=*(g_fmDefectGray+128*pixel[2*i+1]+pixel[2*i]);
			if(nTmp>=nDLimit && nTmp<=nULimit)
				nBoundData[nBoundary++]=*(g_fmDefectGray+128*pixel[2*i+1]+pixel[2*i]);	
		}
		quick_sort(nBoundData, 0, nBoundary-1);
		nSum=nCnt=0;
		for(i=0;i<nBoundary/2;i++)
		{
			nSum+=nBoundData[i];
			nCnt++;
		}
		if(nCnt==0) break;
		dDataDn[nDataCount]=(double)nSum/nCnt;

		nSum=nCnt=0;
		for(i=nBoundary/2;i<nBoundary;i++)
		{
			nSum+=nBoundData[i];
			nCnt++;
		}
		if(nCnt==0) break;
		dDataUp[nDataCount++]=(double)nSum/nCnt;
	}	

	//검은 얼룩 검출----------------------------------------
	//제일 낮은곳 
	dMin=255;
	for(i=0;i<nDataCount;i++)
		if(dDataDn[i]<dMin)
		{
			dMin=dDataDn[i];
			nMinPos=i;
		}
	if(nMinPos<nDataCount-15)
	{
		dBlackSize=*CoreSize+2*nMinPos;	

		dMinEnd=nCnt=nMinPosEnd=0;
		for(i=nMinPos+10;i<nMinPos+20;i++)
		{
			if(i>=nDataCount) continue;
			dMinEnd+=dDataDn[i];
			nCnt++;
		}
		if(nCnt>=3)
		{
			dMinEnd/=nCnt;
			dBlackValue=dMinEnd-dMin;
		}

		for(i=nMinPos+1;i<nDataCount;i++)
			if(dDataDn[i]>(dMinEnd-1)) 
			{
				nMinPosEnd=i;
				break;
			}

		if(nMinPosEnd>0) 
			dBlackSize=*CoreSize+2*nMinPosEnd;	
	}

	//흰얼룩검출--------------------------------------------
	//흰얼룩검출--------------------------------------------
	nMaxPosEnd=0;
	if(1)
	{
		dMinEnd=0;
		for(i=nDataCount-10;i<nDataCount;i++)
			dMinEnd+=dDataUp[i];
		dMinEnd*=0.1;
	}
	for(i=nDataCount-1;i>3;i--)
	{
		if(dDataUp[i]>dMinEnd+dCloudUpTh)
		{
			nMaxPosEnd=i;
			break;
		}
	}
	if(nMaxPosEnd>3)
	{
		dMaxEnd=nCnt=0;
		dMin=255;
		dMax=dMax1=0;
		for(i=1;i<=nMaxPosEnd;i++)
		{
			if(dDataUp[i]<=dMin)
				dMin=dDataUp[i];

			if(dDataUp[i]>dMin+1)
			{
				if(dDataUp[i]>dMax)
					dMax=dDataUp[i];
			}
		}
		if(dMax<1)
		{
			for(i=4;i<=nMaxPosEnd;i++)
				if(dDataUp[i]>dMax)
					dMax=dDataUp[i];
		}	

		if(dMax>0)
		{
			dWhiteValue=dMax-(dMinEnd+1);
			dWhiteSize=*CoreSize+2*(nMaxPosEnd+1);	
		}
	}

	//흰얼룩인지 검은 얼룩인지 체크------------------------
	if( (dBlackValue>=2.5 && dBlackSize>=dWhiteSize) || nMaxPosEnd<=3)
	{
		if(dBlackValue>=1.0) //아무리 작아도 1보다는 커야 함.
		{
			*dCloudValue=dBlackValue;
			*CloudSize  =dBlackSize;
		}
	}
	else 
	{
		if(dWhiteValue>=3)
		{
			*IsWhiteSpot=1;
			*dCloudValue=dWhiteValue;
			*CloudSize  =dWhiteSize;
		}
	}
/*	nMaxPosEnd=0;
	if(dMinEnd<1)
	{
		dMinEnd=0;
		for(i=nDataCount-10;i<nDataCount;i++)
			dMinEnd+=dDataDn[i];
		dMinEnd*=0.1;
	}
	for(i=nDataCount-1;i>3;i--)
	{
		if(dDataDn[i]>dMinEnd+dCloudUpTh)
		{
			nMaxPosEnd=i;
			break;
		}
	}
	if(nMaxPosEnd>3)
	{
		dMaxEnd=nCnt=0;
		dMin=255;
		dMax=dMax1=0;
		for(i=1;i<=nMaxPosEnd;i++)
		{
			if(dDataDn[i]<dMin)
				dMin=dDataDn[i];

			if(dDataDn[i]>dMin+1)
			{
				if(dDataDn[i]>dMax)
					dMax=dDataDn[i];
			}
		}
		if(dMax<1)
		{
			for(i=4;i<=nMaxPosEnd;i++)
				if(dDataDn[i]>dMax)
					dMax=dDataDn[i];
		}	

		if(dMax>0)
		{
			dWhiteValue=dMax-(dMinEnd+1);
			dWhiteSize=*CoreSize+2*(nMaxPosEnd+1);	
		}
	}

	//흰얼룩인지 검은 얼룩인지 체크------------------------
	if( (dBlackValue>=1.5 && dBlackSize>=dWhiteSize) || nMaxPosEnd<=3)
	{
		if(dBlackValue>=1.0) //아무리 작아도 1보다는 커야 함.
		{
			*dCloudValue=dBlackValue;
			*CloudSize  =dBlackSize;
		}
	}
	else 
	{
		*IsWhiteSpot=1;
		*dCloudValue=dWhiteValue;
		*CloudSize  =dWhiteSize;
	}
*/




	//흑처럼 하는 루틴 --(잘 안됨)----------------------
	//제일 높은곳
	//dMin=255;
	//dMax=dMax1=0;
	//for(i=0;i<nDataCount;i++)
	//{
	//	if(dDataUp[i]<dMin)
	//	{
	//		dMin=dDataUp[i];
	//	}
	//	if(dDataUp[i]>dMin+1)
	//	{
	//		if(dDataUp[i]>dMax)
	//		{
	//			dMax=dDataUp[i];
	//			nMaxPos=i;
	//		}
	//	}
	//}
	//
	//if(nMaxPos<nDataCount-15)
	//{
	//	flagWhiteTry=0;
	//	dWhiteSize=*CoreSize+2*nMaxPos;	

	//	dMaxEnd=nCnt=nMaxPosEnd=0;
	//	for(i=nMaxPos+10;i<nMaxPos+20;i++)
	//	{
	//		if(i>=nDataCount) continue;
	//		dMaxEnd+=dDataUp[i];
	//		nCnt++;
	//	}
	//	if(nCnt>=3)
	//	{
	//		dMaxEnd/=nCnt;
	//		dWhiteValue=dMax-dMaxEnd;
	//	}

	//	for(i=nMaxPos+1;i<nDataCount;i++)
	//		if(dDataUp[i]<(dMaxEnd+1)) 
	//		{
	//			nMaxPosEnd=i;
	//			break;
	//		}

	//	if(nMaxPosEnd>0) 
	//		dWhiteSize=*CoreSize+2*nMaxPosEnd;	
	//}
	//else
	//{
	//	flagWhiteTry=1;
	//	//처음부터 밝다가 어두워지면서 끝남.
	//	for(i=0;i<nDataCount;i++)
	//	{
	//		if(dDataUp[i]>dAvg+3)
	//		{
	//			if(i>3) dMax1+=dDataUp[i];
	//			nMaxPos=i;
	//		}
	//		else
	//		{
	//			break;
	//		}
	//	}	
	//	dWhiteSize=*CoreSize+2*nMaxPos;
	//	if(nMaxPos>3) dWhiteValue=dMax1/(nMaxPos-3)-dAvg;
	//	else		  dWhiteValue=0;
	//}

	//if((flagWhiteTry==1 && dBlackValue>1) || dBlackValue>=dWhiteValue)
	//{
	//	*dCloudValue=dBlackValue;
	//	*CloudSize  =dBlackSize;
	//}
	//else
	//{
	//	*IsWhiteSpot=1;
	//	*dCloudValue=dWhiteValue;
	//	*CloudSize  =dWhiteSize;
	//}



	return ret;
}

/*  Full Cross Full개방
int GetHBOSize(LPBYTE fm, int left, int top, int right, int bottom, int pitch, double dAvg, int nCoreTh, int n2ndTh, double dCloudTh, int *IsWhiteSpot, int *Cx, int *Cy, int *pValue, int *CoreSize, int *MiddleSize, int *CloudSize)
{
	int ret=0;
	int i,j, nTmp, nSum;
	int nBase=4;		//새로 만드는 영상의 Offset이라고 생각하면 됨.
	int nKernelSize=11;	//
	int nULimit=25;	
	int nDLimit=25;
	int nOffset=35;		//Cloud구하기 위한 영역
	int nDnTh, nCloudUpTh, nCloudDnTh;
	int ll,tt,rr,bb, line1, line2;
	int ll_,tt_,rr_,bb_;
	int nBlob, nMaxPos, nContour, nMaxContour, nMinValue=255, nMaxValue=0;
	int nH1, nH2;
	int nHisto[256];
	LPBYTE fmT;

	memset(nHisto, 0, 256*sizeof(int));
	*Cx=*Cy=*pValue=*CoreSize=*MiddleSize=*CloudSize=*IsWhiteSpot=0;

	fmT=fm;
	
	nDnTh=dAvg+nCoreTh;
	nCloudDnTh=(dAvg-dCloudTh)*10;
	nCloudUpTh=(dAvg+dCloudTh)*10;

	//Core 구하기-----------------------------------------------------------------------------
	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
			if(*(fmT+pitch*i+j)>nDnTh)	*(g_fmDefectBinary+pitch*i+j)=255;
			else						*(g_fmDefectBinary+pitch*i+j)=0;

	g_Chain.SetChainData(1, g_fmDefectBinary, 1, 1, 1, 2000, BAD_IMG_WIDTH, BAD_IMG_HEIGHT);
	nBlob=g_Chain.FastChain(left,top,right-1,bottom-1);
	nMaxPos=-1;
	nMaxContour=0;
	for(i=0;i<nBlob;i++)
	{
		nContour=g_Chain.GetChainDataNumber(i);
		if(nContour>nMaxContour)
		{
			nMaxContour=nContour;
			nMaxPos=i;
		}
	}
	if(nMaxPos>=0)
	{
		ll	= g_Chain.FindMinX(nMaxPos);
		rr	= g_Chain.FindMaxX(nMaxPos);
		tt	= g_Chain.FindMinY(nMaxPos);
		bb	= g_Chain.FindMaxY(nMaxPos);

		nMaxValue=0;
		for(i=tt;i<bb;i++)
			for(j=ll;j<rr;j++)
			{
				if(*(fmT+BAD_IMG_WIDTH*i+j)>nMaxValue) nMaxValue=*(fmT+pitch*i+j);
			}
		*pValue=nMaxValue-dAvg;

		*Cx=(ll+rr)/2+1;
		*Cy=(tt+bb)/2+1;
		if(rr-ll>bb-tt) *CoreSize=(rr-ll+1);
		else			*CoreSize=(bb-tt+1);
	}
	//----------------------------------------------------------------------------------------------------
	


	//얼룩영역 구하기------------------------------------------------------------------------------
	{
		//3rd 영역(Cloud) 찾는것----------------------------------------------------------------
		ll_=left;
		rr_=right-1;
		tt_=top;
		bb_=bottom-1;
		
		//기준밝기 구하기----------------
		for(i=tt_;i<bb_;i++)
		{
			nHisto[*(fmT+pitch*i+ll_)]++;
			nHisto[*(fmT+pitch*i+rr_)]++;
		}
		for(j=ll_;j<rr_;j++)
		{
			nHisto[*(fmT+pitch*tt_+j)]++;
			nHisto[*(fmT+pitch*bb_+j)]++;
		}
		
		nSum=0;
		for(i=0;i<255;i++)
		{
			nSum+=nHisto[i];
			if(nSum>20) {nH1=i; break;}
		}
		nSum=0;
		for(i=255;i>=0;i--)
		{
			nSum+=nHisto[i];
			if(nSum>20) {nH2=i; break;}
		}
		nSum=nTmp=0;
		for(i=nH1;i<=nH2;i++)
		{
			nSum+=nHisto[i]*i;
			nTmp+=nHisto[i];
		}
		if(nTmp>0)
		{
			nCloudUpTh=( (double)nSum/nTmp+dCloudTh)*10;
			nCloudDnTh=( (double)nSum/nTmp-dCloudTh)*10;
		}

		//-------------------------------

		for(i=tt_+1;i<bb_-1;i++)
			for(j=ll_+1;j<rr_-1;j++)
			{
				nTmp=*(fmT+pitch*(i-1)+j-1)+*(fmT+pitch*(i-1)+j)+*(fmT+pitch*(i-1)+j+1)+
					 *(fmT+pitch*i+j-1)+*(fmT+pitch*i+j)*2+*(fmT+pitch*i+j+1)+
					 *(fmT+pitch*(i+1)+j-1)+*(fmT+pitch*(i+1)+j)+*(fmT+pitch*(i+1)+j+1);
			
				if(nTmp>nCloudUpTh || nTmp<nCloudDnTh)	*(g_fmDefectTemp1+pitch*i+j)=255;
				else									*(g_fmDefectTemp1+pitch*i+j)=0;
			}

		//Erode Binary----------------------------------------------------------------------------------------------------------
		//White Pixel중에 주변 8개중 5개이상 Black이면 Black
		for(i=tt_+2;i<bb_-2;i++)
			for(j=ll_+2;j<rr_-2;j++)
			{
				if((g_fmDefectTemp1+pitch*i+j)==0) 
				{
					*(g_fmDefectGray+pitch*i+j)=0;
					continue;
				}

				nTmp=*(g_fmDefectTemp1+pitch*(i-1)+j-1)+*(g_fmDefectTemp1+pitch*(i-1)+j)+*(g_fmDefectTemp1+pitch*(i-1)+j+1)+
					 *(g_fmDefectTemp1+pitch*i+j-1)+*(g_fmDefectTemp1+pitch*i+j+1)+
					 *(g_fmDefectTemp1+pitch*(i+1)+j-1)+*(g_fmDefectTemp1+pitch*(i+1)+j)+*(g_fmDefectTemp1+pitch*(i+1)+j+1);

				if(nTmp<=255*3) *(g_fmDefectGray+pitch*i+j)=0;
				else		    *(g_fmDefectGray+pitch*i+j)=255;
			}
		//----------------------------------------------------------------------------------------------------------------------
	
		g_Chain.SetChainData(1, g_fmDefectGray, 2, 2, 20, 2000, 128, 128);
		nBlob=g_Chain.FastChain(ll_+1,tt_+1,rr_-1,bb_-1);	
		nMaxPos=-1;
		nMaxContour=0;
		for(i=0;i<nBlob;i++)
		{
			ll_	= g_Chain.FindMinX(i);
			rr_	= g_Chain.FindMaxX(i);
			tt_	= g_Chain.FindMinY(i);
			bb_	= g_Chain.FindMaxY(i);
			if( !((ll_<=ll && rr_>=rr) || (tt_<tt && bb_>bb)) ) continue;

			nContour=g_Chain.GetChainDataNumber(i);
			if(nContour>nMaxContour)
			{
				nMaxContour=nContour;
				nMaxPos=i;
			}
		}

		if(nMaxPos>=0)
		{
			ll_	= g_Chain.FindMinX(nMaxPos);
			rr_	= g_Chain.FindMaxX(nMaxPos);
			tt_	= g_Chain.FindMinY(nMaxPos);
			bb_	= g_Chain.FindMaxY(nMaxPos);
			if(rr_-ll_>bb_-tt_) *CloudSize=(rr_-ll_+1)-3;
			else				*CloudSize=(bb_-tt_+1)-3;
		}
		//-------------------------------------------------------------------------------
	}


	return ret;
}
*/


/*  93도 필토  내부
int GetHBOSize(LPBYTE fm, int left, int top, int right, int bottom, int pitch, double dAvg, int nCoreTh, int n2ndTh, double dCloudTh, int *IsWhiteSpot, int *Cx, int *Cy, int *pValue, int *CoreSize, int *MiddleSize, int *CloudSize)
{
	int ret=0;
	int i,j, nTmp, nSum;
	int nBase=4;		//새로 만드는 영상의 Offset이라고 생각하면 됨.
	int nKernelSize=11;	//
	int nULimit=25;	
	int nDLimit=25;
	int nOffset=35;		//Cloud구하기 위한 영역
	int nDnTh, nCloudTh;
	int ll,tt,rr,bb, line1, line2;
	int ll_,tt_,rr_,bb_;
	int nBlob, nMaxPos, nContour, nMaxContour, nMinValue=255, nMaxValue=0;
	int nH1, nH2;
	int nHisto[256];
	LPBYTE fmT;

	memset(nHisto, 0, 256*sizeof(int));
	*Cx=*Cy=*pValue=*CoreSize=*MiddleSize=*CloudSize=*IsWhiteSpot=0;

	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
			if(*(fm+pitch*i+j)<nMinValue)		nMinValue=*(fm+pitch*i+j);
			else if(*(fm+pitch*i+j)>nMaxValue) 	nMaxValue=*(fm+pitch*i+j);

	if(nMaxValue-dAvg > dAvg-nMinValue)
	{
		for(i=top;i<bottom;i++)
			for(j=left;j<right;j++)
				*(g_fmDefectGray+pitch*i+j)=255-*(fm+pitch*i+j);

		fmT=g_fmDefectGray;
		dAvg=255-dAvg;
		dCloudTh+=1;
		*IsWhiteSpot=1;
	}
	else
	{
		fmT=fm;
	}

	nDnTh=dAvg-nCoreTh;
	nCloudTh=(dAvg+dCloudTh)*10;

	//Std Filter
	ImageFilter_withStd(fmT, g_fmDefectTemp1, left,top,right,bottom, pitch, dAvg, nBase, nKernelSize, nULimit, nDLimit);

	//Core 구하기-----------------------------------------------------------------------------
	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
			if(*(fmT+pitch*i+j)<nDnTh)	*(g_fmDefectBinary+pitch*i+j)=255;
			else						*(g_fmDefectBinary+pitch*i+j)=0;

	g_Chain.SetChainData(1, g_fmDefectBinary, 1, 1, 1, 2000, BAD_IMG_WIDTH, BAD_IMG_HEIGHT);
	nBlob=g_Chain.FastChain(left,top,right-1,bottom-1);
	nMaxPos=-1;
	nMaxContour=0;
	for(i=0;i<nBlob;i++)
	{
		nContour=g_Chain.GetChainDataNumber(i);
		if(nContour>nMaxContour)
		{
			nMaxContour=nContour;
			nMaxPos=i;
		}
	}
	if(nMaxPos>=0)
	{
		ll	= g_Chain.FindMinX(nMaxPos);
		rr	= g_Chain.FindMaxX(nMaxPos);
		tt	= g_Chain.FindMinY(nMaxPos);
		bb	= g_Chain.FindMaxY(nMaxPos);

		nMinValue=255;
		for(i=tt;i<bb;i++)
			for(j=ll;j<rr;j++)
			{
				if(*(fmT+BAD_IMG_WIDTH*i+j)<nMinValue) nMinValue=*(fmT+pitch*i+j);
			}
		*pValue=dAvg-nMinValue;

		*Cx=(ll+rr)/2+1;
		*Cy=(tt+bb)/2+1;
		if(rr-ll>bb-tt) *CoreSize=(rr-ll+1);
		else			*CoreSize=(bb-tt+1);
	}
	//----------------------------------------------------------------------------------------------------
	

	//얼룩영역 구하기------------------------------------------------------------------------------
	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
			if(*(g_fmDefectTemp1+pitch*i+j)>n2ndTh)	*(g_fmDefectBinary+pitch*i+j)=255;
			else									*(g_fmDefectBinary+pitch*i+j)=0;


	g_Chain.SetChainData(1, g_fmDefectBinary, 2, 2, 10, 2000, BAD_IMG_WIDTH, BAD_IMG_HEIGHT);
	nBlob=g_Chain.FastChain(left,top,right-1,bottom-1);		     
	nMaxPos=-1;
	nMaxContour=0;
	for(i=0;i<nBlob;i++)
	{
		ll_	= g_Chain.FindMinX(i);
		rr_	= g_Chain.FindMaxX(i);
		tt_	= g_Chain.FindMinY(i);
		bb_	= g_Chain.FindMaxY(i);
		if( !((ll_<=ll && rr_>=rr) && (tt_<=tt && bb_>=bb)) ) continue;

		nContour=g_Chain.GetChainDataNumber(i);
		if(nContour>nMaxContour)
		{
			nMaxContour=nContour;
			nMaxPos=i;
		}
	}
	if(nMaxPos>=0)
	{
		ll_	= g_Chain.FindMinX(nMaxPos);
		rr_	= g_Chain.FindMaxX(nMaxPos);
		tt_	= g_Chain.FindMinY(nMaxPos);
		bb_	= g_Chain.FindMaxY(nMaxPos);
		if(rr_-ll_>bb_-tt_) *MiddleSize=(rr_-ll_+1);
		else				*MiddleSize=(bb_-tt_+1);
		
		*MiddleSize-=(nKernelSize/2);
		if(*MiddleSize<*CoreSize) *MiddleSize=*CoreSize;

		//3rd 영역(Cloud) 찾는것----------------------------------------------------------------
		ll_=ll_-nOffset; if(ll_<left)	ll_=left;
		rr_=rr_+nOffset; if(rr_>right)	rr_=right;
		tt_=tt_-nOffset; if(tt_<top)	tt_=top;
		bb_=bb_+nOffset; if(bb_>bottom) bb_=bottom;
		
		//기준밝기 구하기----------------
		for(i=tt_;i<bb_;i++)
		{
			nHisto[*(fmT+pitch*i+ll_)]++;
			nHisto[*(fmT+pitch*i+rr_)]++;
		}
		for(j=ll_;j<rr_;j++)
		{
			nHisto[*(fmT+pitch*tt_+j)]++;
			nHisto[*(fmT+pitch*bb_+j)]++;
		}
		
		nSum=0;
		for(i=0;i<255;i++)
		{
			nSum+=nHisto[i];
			if(nSum>20) {nH1=i; break;}
		}
		nSum=0;
		for(i=255;i>=0;i--)
		{
			nSum+=nHisto[i];
			if(nSum>20) {nH2=i; break;}
		}
		nSum=nTmp=0;
		for(i=nH1;i<=nH2;i++)
		{
			nSum+=nHisto[i]*i;
			nTmp+=nHisto[i];
		}
		if(nTmp>0)
			nCloudTh=( (double)nSum/nTmp+dCloudTh)*10;

		//-------------------------------

		for(i=tt_+1;i<bb_-1;i++)
			for(j=ll_+1;j<rr_-1;j++)
			{
				nTmp=*(fmT+pitch*(i-1)+j-1)+*(fmT+pitch*(i-1)+j)+*(fmT+pitch*(i-1)+j+1)+
					 *(fmT+pitch*i+j-1)+*(fmT+pitch*i+j)*2+*(fmT+pitch*i+j+1)+
					 *(fmT+pitch*(i+1)+j-1)+*(fmT+pitch*(i+1)+j)+*(fmT+pitch*(i+1)+j+1);
			
				if(nTmp>nCloudTh)	*(g_fmDefectBinary+pitch*i+j)=255;
				else				*(g_fmDefectBinary+pitch*i+j)=0;
			}

		//Erode Binary----------------------------------------------------------------------------------------------------------
		//White Pixel중에 주변 8개중 5개이상 Black이면 Black
		for(i=tt_+2;i<bb_-2;i++)
			for(j=ll_+2;j<rr_-2;j++)
			{
				if((g_fmDefectBinary+pitch*i+j)==0) 
				{
					*(g_fmDefectGray+pitch*i+j)=0;
					continue;
				}

				nTmp=*(g_fmDefectBinary+pitch*(i-1)+j-1)+*(g_fmDefectBinary+pitch*(i-1)+j)+*(g_fmDefectBinary+pitch*(i-1)+j+1)+
					*(g_fmDefectBinary+pitch*i+j-1)+*(g_fmDefectBinary+pitch*i+j+1)+
					*(g_fmDefectBinary+pitch*(i+1)+j-1)+*(g_fmDefectBinary+pitch*(i+1)+j)+*(g_fmDefectBinary+pitch*(i+1)+j+1);

				if(nTmp<=255*3) *(g_fmDefectGray+pitch*i+j)=0;
				else		    *(g_fmDefectGray+pitch*i+j)=255;
			}
		//----------------------------------------------------------------------------------------------------------------------
	
		g_Chain.SetChainData(1, g_fmDefectGray, 2, 2, 20, 2000, 128, 128);
		nBlob=g_Chain.FastChain(ll_+1,tt_+1,rr_-1,bb_-1);	
		nMaxPos=-1;
		nMaxContour=0;
		for(i=0;i<nBlob;i++)
		{
			nContour=g_Chain.GetChainDataNumber(i);
			if(nContour>nMaxContour)
			{
				nMaxContour=nContour;
				nMaxPos=i;
			}
		}

		if(nMaxPos>=0)
		{
			ll_	= g_Chain.FindMinX(nMaxPos);
			rr_	= g_Chain.FindMaxX(nMaxPos);
			tt_	= g_Chain.FindMinY(nMaxPos);
			bb_	= g_Chain.FindMaxY(nMaxPos);
			if(rr_-ll_>bb_-tt_) *CloudSize=(rr_-ll_+1)-3;
			else				*CloudSize=(bb_-tt_+1)-3;
		
			if(*CloudSize<*MiddleSize) *CloudSize=*MiddleSize;
		}
		//-------------------------------------------------------------------------------
	}


	return ret;
}
*/
void ImageFilter_withStd(LPBYTE fm, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, double dAvg, double dBase, int nKerSize, int nUpperLimtit, int nDownLimit)
{
	int i, j, ii, jj;
	BYTE nTmp;
	int nSum=0;
	int nOffset;
	int nCount;
	int nULimit, nDLimit;
	int nHArea, nArea;
	int nJimpJ;
	double dStdev, dStdevMax, dTmp, dtemp1, dtemp2, dSum, dData1;
	int nSum_=0;
	double dtemp1_, dtemp2_, dTmp_, dSum_;

	nArea =nKerSize*nKerSize;
	nHArea=nArea/2;
	nOffset=nKerSize/2;
	dStdevMax=100;
	nULimit=dAvg+nUpperLimtit;
	nDLimit=dAvg-nDownLimit;
	

	for(i=top;i<bottom;i++)
		for(j=left;j<right;j++)
		{
			*(fmD+pitch*i+j)=0;
		}

	//새로운 이미지 만듦
	for(i=top;i<bottom-nKerSize;i++)
		for(j=left;j<right-nKerSize;j++)
		{
			if(j==left)
			{
				nSum=0;
				dtemp1=0;
				dtemp2=0;
				dSum=0;
				for(ii=i;ii<i+nKerSize;ii++)
					for(jj=j;jj<j+nKerSize;jj++)
					{
						nTmp    =*(fm+pitch*ii+jj);
						if(nTmp>nULimit || nTmp<nDLimit) {nSum++; continue;}
					
						dtemp1 += nTmp;
						dtemp2 += nTmp * nTmp;
						dTmp    = nTmp - dAvg;
						dSum   += dTmp*dTmp;
					}

				nSum_  =nSum;
				dtemp1_=dtemp1;
				dtemp2_=dtemp2;
				dSum_  =dSum;
			}
			else //계산시간 절약을 위한 라인 1/3 으로 줌----------------------------
			{
				nSum=0;
				dtemp1=0;
				dtemp2=0;
				dSum=0;

				jj=j-1;
				for(ii=i;ii<i+nKerSize;ii++)
				{
					nTmp    =*(fm+pitch*ii+jj);
					if(nTmp>nULimit || nTmp<nDLimit) {nSum++; continue;}
					
					dtemp1 += nTmp;
					dtemp2 += nTmp * nTmp;
					dTmp    = nTmp - dAvg;
					dSum   += dTmp*dTmp;
				}
				nSum_  -=nSum;
				dtemp1_-=dtemp1;
				dtemp2_-=dtemp2;
				dSum_  -=dSum;


				nSum=0;
				dtemp1=0;
				dtemp2=0;
				dSum=0;
				jj=j+nKerSize-1;
				for(ii=i;ii<i+nKerSize;ii++)
				{
					nTmp    =*(fm+pitch*ii+jj);
					if(nTmp>nULimit || nTmp<nDLimit) {nSum++; continue;}
					
					dtemp1 += nTmp;
					dtemp2 += nTmp * nTmp;
					dTmp    = nTmp - dAvg;
					dSum   += dTmp*dTmp;
				}
				nSum_  +=nSum;
				dtemp1_+=dtemp1;
				dtemp2_+=dtemp2;
				dSum_  +=dSum;


				nSum  =nSum_;	
				dtemp1=dtemp1_;
				dtemp2=dtemp2_;
				dSum  =dSum_;
			}//---------------------------------------------------------------------

			nTmp=*(fm+pitch*(i+nOffset)+(j+nOffset));
			if(nTmp>nULimit || nTmp<nDLimit) 
			{
				*(fmD+pitch*(i+nOffset)+(j+nOffset))=255;
				continue;
			}

			nCount=	nArea-	nSum;

			if(nSum>=nHArea) 
			{
				*(fmD+pitch*(i+nOffset)+(j+nOffset))=255;
			}
			else
			{

				dData1=nCount*dtemp2-dtemp1*dtemp1;
				if(dData1>0 && nCount*(nCount-1.)>0)
					dStdev = (dData1)/(nCount*(nCount-1.))  - dBase;
				else 
					dStdev=0;
				if(dStdev<0) dStdev=0;

				if(dStdev>dStdevMax)
					*(fmD+pitch*(i+nOffset)+(j+nOffset))=255;
				else
				{
					nTmp=(int)(dStdev/dStdevMax*255);
					*(fmD+pitch*(i+nOffset)+(j+nOffset))=nTmp;
				}
			}
		}
}



int ImageElongate(LPBYTE fm, LPBYTE fmD, int left, int top, int right, int bottom, int pitch, int nX, int nY)
{
	int ok=-1;
	int i,j, width, height;
	int nXEnd, nYEnd;
	CPoint ptOrg[4];
	CPoint ptDst[4];
	LPBYTE fmT;

	width=(right-left+nX);
	height=(bottom-top+nY);
	if(width<10 || right+nX>=pitch || (bottom-top)+nY<10 || bottom+nY>=g_System.m_nImageH) return ok;

	ptOrg[0].x=left; ptOrg[0].y=top;
	ptOrg[1].x=left; ptOrg[1].y=bottom;
	ptOrg[2].x=right; ptOrg[2].y=bottom;
	ptOrg[3].x=right; ptOrg[3].y=top;

	ptDst[0].x=0;		 ptDst[0].y=0;
	ptDst[1].x=0;		 ptDst[1].y=height;
	ptDst[2].x=width;	 ptDst[2].y=height;
	ptDst[3].x=width;	 ptDst[3].y=0;

	fmT=new BYTE[width*(bottom-top+nY)];
	InterpolationTransform_SubPixel(ptOrg, fm, pitch, ptDst, fmT, width); 

	if((right-left)<width) nXEnd=(right-left);
	else                   nXEnd=width;

	if((bottom-top)<height) nYEnd=(bottom-top);
	else                    nYEnd=height;


	for(i=0;i<nYEnd;i++)
		for(j=0;j<nXEnd;j++)
			*(fmD+pitch*(top+i)+(left+j))=*(fmT+width*i+j);

	ok=0;
	return ok;
}


//int g_ProfileHor[4096];
//수평밝은 노이즈 라인(2개 없애는 함수
void RemoveHorlineNoise(LPBYTE fm, int nX1, int nX2, int height, int pitch, int nThres)
{
	int i, j, nTmp;
	long nSum=0;
	int nSkip;
	int nWCount=0;
	int nAve;
	int nMaxPos, nMaxDiff=0;
	int nMax, nPos, nMin, nMinPos;

	if(nX2-nX1<1) return;

	nSkip=(nX2-nX1+1)/512; if(nSkip<1) nSkip=1;
	for(j=nX1;j<nX2;j+=nSkip) nWCount++;

	for(i=0;i<height;i++)
	{
		nTmp=0;
		for(j=nX1;j<nX2;j+=nSkip)
			nTmp+=*(fm+pitch*i+j);

		g_ProfileHor[i]=(nTmp/nWCount);
		nSum+=(nTmp/nWCount);
	}
	nAve=nSum/height;

	for(i=1;i<height;i++)
	{
		nTmp=g_ProfileHor[i]-g_ProfileHor[i-1];
		if(nTmp<0) nTmp*=-1;
		if(nTmp>nMaxDiff)
		{
			nMaxDiff=nTmp;
			nMaxPos=i;
		}
	}
	if(nMaxDiff>nThres)
	{
		nMax=0;
		nMin=256;
		nPos=-1;
		nMinPos=-1;
		for(i=nMaxPos-2;i<=nMaxPos+1;i++)
		{
			if(i<0 || i>=height) continue;
			if(g_ProfileHor[i]>nMax)
			{
				nMax=g_ProfileHor[i];
				nPos=i;
			}
			if(g_ProfileHor[i]<nMin)
			{
				nMin=g_ProfileHor[i];
				nMinPos=i;
			}
		}
		if(nPos>=0)
		{
			for(j=nX1;j<nX2;j++)
				*(fm+pitch*nPos+j)=*(fm+pitch*nMinPos+j);

		}
		if(nPos>0 && nPos<height-1)
		{
			if(g_ProfileHor[nPos-1]>g_ProfileHor[nPos+1])
			{
				for(j=nX1;j<nX2;j++)
					*(fm+pitch*(nPos-1)+j)=*(fm+pitch*nMinPos+j);
			}
			else
			{
				for(j=nX1;j<nX2;j++)
					*(fm+pitch*(nPos+1)+j)=*(fm+pitch*nMinPos+j);
			}
		}
	}
}

double CheckDiffTopBottom(LPBYTE fm, int pitch, int nX1, int nX2, int height)
{
	int j;
	long nSumT=0;
	long nSumB=0;
	double dDiff=0;
	LPBYTE fmT, fmB;

	if(nX1<0) nX1=0;
	if(nX2>=pitch) nX2=pitch-1;

	fmT=fm+nX1;
	fmB=fm+(height-1)*pitch+nX1;
	for(j=nX1;j<nX2;j++)
	{
		nSumT+=*(fmT++);
		nSumB+=*(fmB++);
	}
	if(nX2-nX1>0) dDiff=(double)(nSumT-nSumB)/(nX2-nX1);
	if(dDiff<0) dDiff*=-1;

	return dDiff;
}

double CheckDiffTopBottom(LPBYTE fm, int pitch, int nX1, int nX2, int height, int *pYPos)
{
	int i,j;
	long nSumT=0;
	long nSumB=0;
	double dDiff=0, dDiffMax=0;
	int nCnt, nPos=-1;
	LPBYTE fmT, fmB;
	int nDIff;
	*pYPos=0;
	if(nX1<0) nX1=0;
	if(nX2>=pitch) nX2=pitch-1;

	nCnt=(nX2-nX1)/4;
	if(nX2-nX1<=0) return 0.0;

	if(nCnt<200)
	{
		nCnt=0;
		for(j=nX1;j<nX2;j++) nCnt++;

		fmT=fm+nX1;
		for(j=nX1;j<nX2;j++)
			nSumT+=*(fmT++);
		

		for(i=1;i<height/2;i++)
		{
			nSumB=0;
			fmT=fm+pitch*i+nX1;
			for(j=nX1;j<nX2;j++)
			   nSumB+=*(fmT++);

			dDiff=(double)(nSumB-nSumT)/nCnt;
			if(dDiff<0) dDiff*=-1;
			if(dDiff>dDiffMax)
			{
				dDiffMax=dDiff;
				nPos=i;
			}
			nSumT=nSumB;
		}
	}
	else
	{
		nCnt=0;
		for(j=nX1;j<nX2;j+=4) nCnt++;

		fmT=fm;
		for(j=nX1;j<nX2;j+=4)
			nSumT+=*(fmT+j);

		for(i=1;i<height/2;i++)
		{
			nSumB=0;
			fmT=fm+pitch*i;
			for(j=nX1;j<nX2;j+=4)
				nSumB+=*(fmT+j);

			dDiff=(double)(nSumB-nSumT)/nCnt;
			if(dDiff<0) dDiff*=-1;
			if(dDiff>dDiffMax)
			{
				dDiffMax=dDiff;
				nPos=i;
			}
			nSumT=nSumB;
		}
	}

	*pYPos=nPos;
	return dDiffMax;
}


double GetStdevData(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch)
{
	int i,j;
	long nSum=0;
	int tmp;
	double dSum2=0, stdev=0; 
	long   nArea;
//	if(x1<0 || x2>pitch || y1<0 ) return 0;

	nArea=(x2-x1)*(y2-y1);
    for(i=y1;i<y2;i++)
		for(j=x1;j<x2;j++)
		{
			tmp=*(fm+pitch*i+j);
			nSum+=tmp;
			dSum2+=(tmp*tmp);
		}

    stdev=sqrt((nArea*dSum2-(double)nSum*(double)nSum)/((double)nArea*((double)nArea-1.)));	
	return stdev;
}
/*
//Filter부분추가
void Func_Filter(int nFilter, LPBYTE fmS, LPBYTE fmD, int left, int top, int right, int bottom, int pitch)
{
	int i,j;
	int nSame=0;
	LPBYTE fmDD;

	if(fmS==fmD)
	{
		nSame=1;
		fmDD=g_fmFilter;
	}
	else
		fmDD=fmD;


	switch(nFilter)
	{  
		case	FILTER_2DF: Filter2DFVer(fmS, fmDD, left, top, right, bottom, pitch, 3,6, 1, 10); break;
		case	FILTER_HIGHPASS: HighPass(fmS, fmDD, left, top, right, bottom, pitch, 5,5); break;
		case	FILTER_EMPH: Emphasize(fmS, fmDD, left, top, right, bottom, pitch,10,10,2); break;
	}

	if(nSame==1)
	{
		for(i=top;i<bottom;i++)
		{
			memcpy(fmS+pitch*i+left, fmDD+pitch*i+left, (right-left)); 
		}
	}
}
*/


double SearchMaxStd_Cut(LPBYTE fm, int nX, int nY, int pitch, int MURA_RECT, int nBaseValue, int nHighCut, int nLowCut, int nLimitX1, int nLimitX2, int nHeight, int nSearchRange, int *pRx, int *pRy)
{
	double dStdev, dMaxStdev=0;
	int i, j, nX1, nX2, nY1, nY2;
	int nCx, nCy;
	int nCutH=nBaseValue+nHighCut;
	int nCutL=nBaseValue-nLowCut;

	nX1=nX-nSearchRange/2; if(nX1<nLimitX1) nX1=nLimitX1;
	nX2=nX+nSearchRange/2; if(nX2>nLimitX2) nX2=nLimitX2;
	nY1=nY-nSearchRange/2; if(nY1<0)        nY1=0;
	nY2=nY+nSearchRange/2; if(nY2>nHeight)  nY2=nHeight;


	for(i=nY1;i<nY2-MURA_RECT;i+=4)
		for(j=nX1;j<nX2-MURA_RECT;j+=4)
		{
			dStdev=FindStd_Cut(fm, j,i, j+MURA_RECT, i+MURA_RECT, pitch, nCutH, nCutL);
			if(dStdev>dMaxStdev)
			{
				nCx=j;
				nCy=i;
				dMaxStdev=dStdev;
			}
		}
	if(dMaxStdev<=0) return 0;

	nX1=nCx-2; if(nX1<nLimitX1)  nX1=nLimitX1;
	nX2=nCx+2; if(nX2>=nLimitX2) nX2=nLimitX2;
	nY1=nCy-2; if(nY1<0)         nY1=0;
	nY2=nCy+2; if(nY2>=nHeight)  nY2=nHeight;

	for(i=nY1;i<=nY2-MURA_RECT;i++)
		for(j=nX1;j<=nX2-MURA_RECT;j++)
		{
			dStdev=FindStd_Cut(fm, j,i, j+MURA_RECT, i+MURA_RECT, pitch, nCutH, nCutL);
			if(dStdev>dMaxStdev)
			{
				nCx=j;
				nCy=i;
			}
		}
	
	*pRx=nCx+MURA_RECT/2;
	*pRy=nCy+MURA_RECT/2;

	return dMaxStdev;

}
double FindStd_Cut(LPBYTE fm, int x1, int y1, int x2, int y2, int pitch, int nCutH, int nCutL)
{
	int i,j;
	long nSum=0;
	int tmp;
	double dSum2=0, stdev=0; 
	long   nArea;

	nArea=(x2-x1)*(y2-y1);
    for(i=y1;i<y2;i++)
		for(j=x1;j<x2;j++)
		{
			tmp=*(fm+pitch*i+j);
			if(tmp>nCutH)		tmp=nCutH;
			else if(tmp<nCutL)	tmp=nCutL;

			nSum+=tmp;
			dSum2+=(tmp*tmp);
		}

    stdev=sqrt((nArea*dSum2-(double)nSum*(double)nSum)/((double)nArea*((double)nArea-1.)));	
	return stdev;

}

void BrightCompensation(LPBYTE fm, LPBYTE fmT, int nWidth, int nHeight, int pitch, int nType)
{

	int i, j;
	long nSum=0, nCount=0, nAvg;
	int nOffset, nTmp;

	if(nType>4) nType=4;

	switch(nType)
	{
		case 2: 
				if(g_Temp.m_nPCOptic==COS3)
				{
					for(i=0;i<nHeight-1;i++)
						for(j=0;j<nWidth;j++)
						{
							nOffset=i*pitch+j;
							nTmp=*(fm+nOffset)+*(fm+nOffset+pitch);
							if(nTmp>255) nTmp=255;
							*(fmT+nOffset)=nTmp;
						}

					for(j=0;j<nWidth;j++)
					{
						nTmp=*(fm+pitch*(nHeight-1)+j)*2;
						if(nTmp>255) nTmp=255;
						*(fmT+pitch*(nHeight-1)+j)=nTmp;
					}
				}
				else
				{
					for(i=0;i<nHeight;i++)
						for(j=0;j<nWidth-1;j++)
						{
							nOffset=i*pitch+j;
							nTmp=*(fm+nOffset)+*(fm+nOffset+1);
							if(nTmp>255) nTmp=255;
							*(fmT+nOffset)=nTmp;
						}

					for(i=0;i<nHeight;i++)
						*(fmT+i*pitch+nWidth-1)=*(fmT+i*pitch+nWidth-2);
				}

				memcpy(fm, fmT, nHeight*nWidth);
				break;
		case 3: for(i=0;i<nHeight-1;i++)
					for(j=0;j<nWidth-1;j++)
					{
						nOffset=i*pitch+j;
						nTmp=*(fm+nOffset)+*(fm+nOffset+1)+*(fm+nOffset+pitch);
						if(nTmp>255) 
							nTmp=255;
						*(fmT+nOffset)=nTmp;
					}
				for(i=0;i<nHeight;i++)
					*(fmT+i*pitch+nWidth-1)=*(fmT+i*pitch+nWidth-2);
		
				for(j=0;j<nWidth;j++)
					*(fmT+pitch*(nHeight-1)+j)=*(fmT+pitch*(nHeight-2)+j);

				memcpy(fm, fmT, nHeight*nWidth);
				break;
		case 4: for(i=0;i<nHeight-1;i++)
					for(j=0;j<nWidth-1;j++)
					{
						nOffset=i*pitch+j;
						nTmp=*(fm+nOffset)+*(fm+nOffset+1)+*(fm+nOffset+pitch)+*(fm+nOffset+pitch+1);
						if(nTmp>255) 
							nTmp=255;
						*(fmT+nOffset)=nTmp;
					}
				for(i=0;i<nHeight;i++)
					*(fmT+i*pitch+nWidth-1)=*(fmT+i*pitch+nWidth-2);
		
				for(j=0;j<nWidth;j++)
					*(fmT+pitch*(nHeight-1)+j)=*(fmT+pitch*(nHeight-2)+j);

				memcpy(fm, fmT, nHeight*nWidth);
				break;
	}
}

//검출한 불량이 수평 노이즈 인지 Check
//nNoizeType =0 흑선, 1 백선, 2 흑백모두
//nDiffValue : 수평라인이 평균값 대비 얼마나 어두우면 수평라인이라고 판정할 것인지
int l_ProfileHNoize[4][128];
int IsHorNoize(int nNoizeType, LPBYTE fm, int nW, int nH, int pitch, int nDiffValue)
{
	int i, j, nTmp;
	int nSum=0, nAvg;
	int nMin;
	int nPos[4]={-1,-1,-1,-1}, nMaxPos, nMinPos;

	if(nH>128 || nW>128) return 0; //노이즈 아님

	memset(l_ProfileHNoize, 0, 4*128*sizeof(int));
	for(i=0;i<nH;i++)
	{
		for(j=0;j<nW;j++)
		{
			nTmp=*(fm+pitch*i+j);
			nSum+=nTmp;
			l_ProfileHNoize[j/32][i]+=nTmp;
		}
	}
	nAvg=nSum/(nW*nH);
	for(j=0;j<4;j++)
	{
		nMin=255*nW;
		for(i=0;i<nH;i++)
		{
			if(l_ProfileHNoize[j][i]<nMin)
			{
				nMin=l_ProfileHNoize[j][i];
				nPos[j]=i;
			}
		}
		if(nNoizeType==0 && nAvg-nMin/(nH*nW/4) <=nDiffValue)	  return 0;
		if(nNoizeType==1 && nMin/(nH*nW/4)-nAvg <=nDiffValue)	  return 0;
		if(nNoizeType==2 && abs(nAvg-nMin/(nH*nW/4))<=nDiffValue) return 0;
	}
	nMaxPos=0;
	nMinPos=nH;
	for(i=0;i<4;i++)
	{
		if(nPos[i]>nMaxPos) nMaxPos=nPos[i];
		if(nPos[i]<nMinPos) nMinPos=nPos[i];
	}
	if(nMaxPos-nMinPos<=1) return 1; //Noize
	else                   
		return 0;

}
