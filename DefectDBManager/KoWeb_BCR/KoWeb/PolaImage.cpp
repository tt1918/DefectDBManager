#include "stdafx.h"
#include "math.h"

//----------------------------------------------------------
// pixel offset for each polarization angles in raw image 
// 90   45   90   45   90   45 .......
// 135  0   135   0   135    0 .......
//----------------------------------------------------------
// --------------------------------------
// S0 = ( I0  + I90 ) or ( I45 + I135 )
// S1 = ( I0  - I90 )/S0 
// S2 = ( I45 - I135 )/S0 
// DoLP = sqr( S1 * S1 + S2 * S2 )/S0 
// AoP = arctan( S2/S1 )/2 
//--------------------------------------
void MakeMultiImage(LPBYTE fm, LPBYTE fm0, LPBYTE fm1, LPBYTE fm2, LPBYTE fm3, LPBYTE fmS0, LPBYTE fmS1,LPBYTE fmS2, LPBYTE fmAoP, LPBYTE fmDoLP, int width, int height)
{
	int i, j;
	int nTmp, nTmp1;
	int pitch=g_System.m_nPitch;
	double S1, S2, fAoP;

	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
		{
			*(fm2+width*i+j)=*(fm+pitch*2*i+2*j);		//90
			*(fm1+width*i+j)=*(fm+pitch*2*i+2*j+1);		//45
			*(fm3+width*i+j)=*(fm+pitch*(2*i+1)+2*j);	//135
			*(fm0+width*i+j)=*(fm+pitch*(2*i+1)+2*j+1); //0

			nTmp=(*(fm0+width*i+j)+*(fm1+width*i+j)+*(fm2+width*i+j)+*(fm3+width*i+j))/2; if(nTmp>255) nTmp=255;  //0도 90도 더함.
			*(fmS0+width*i+j)=nTmp;
			if(nTmp==0) nTmp=1;

			S1=(*(fm0+width*i+j)-*(fm2+width*i+j));
			S2=(*(fm1+width*i+j)-*(fm3+width*i+j));

			nTmp1=255*sqrt(S1*S1+S2*S2)/nTmp; if(nTmp1>255) nTmp1=255;
			*(fmDoLP+width*i+j)=nTmp1;

			if(S1!=0)
				fAoP = atan(S2 / S1) / 2.0;
			else
				fAoP=45;

			if(fAoP>255) fAoP=255;
			*(fmAoP+width*i+j)=(int)fAoP;

			S1=S1/2+128;
			if(S1<0) S1*=-1;
			if(S1>255) S1=255;
			*(fmS1+width*i+j)=(int)S1;

			S2=S2/2+128;
			if(S2<0) S2*=-1;
			if(S2>255) S2=255;
			*(fmS2+width*i+j)=(int)S2;
		}
}

