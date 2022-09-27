#include "stdafx.h"
#include "math.h"

//////////////////////////////////////////////////////////////////////////////////////////

#define MAXEDGENUMBER   200    //한 Line에서 구할수 있는 최대 Edge의 수 (임의로 증가가능)
#define K_MAX_PIXEL		MAX_WIDTH	
#define MAX_LINE_LENGTH	K_MAX_PIXEL
double PixelX[K_MAX_PIXEL], PixelY[K_MAX_PIXEL];
double PixelV[K_MAX_PIXEL];
double EdgeM[MAXEDGENUMBER];
//////////////////////////////////////////////////////////////////////////////////////////

double dHorX[MAX_LINE_LENGTH], dHorY[MAX_LINE_LENGTH];
double dVerX[MAX_LINE_LENGTH], dVerY[MAX_LINE_LENGTH];


int LN_FindEdge1_B2W(int n, double *data, double *edge, double *slope)
{
	int i, maxpos;
	double maxslope;
	double Temp_A, Temp_B;
	
	if(n<2)	return -1;

	maxslope=maxpos=0;
	for(i=1;i<n-1;i++)
		if( (data[i+1]-data[i-1])> maxslope)
		{
			maxpos=i;
			maxslope=(data[i+1]-data[i-1]);
		}
		
	if(maxslope==0) return -2;

	*slope=fabs(maxslope);
	if(maxpos<=2 || maxpos>=(n-3))
	{
		*edge=(double)maxpos;
	}
	else
	{
		if(fabs(data[maxpos+2]-data[maxpos]) < fabs(data[maxpos]-data[maxpos-2]))
		{
			maxpos-=1;
		}

		Temp_A=(double)fabs(data[maxpos+2]+data[maxpos-2]-2*data[maxpos]);
		Temp_B=(double)fabs(data[maxpos+3]+data[maxpos-1]-2*data[maxpos+1]);

		if((Temp_A+Temp_B)!=0)
			*edge=(double)maxpos+Temp_A/(Temp_A+Temp_B);	
		else
			*edge=(double)maxpos;
	}

	return 1;
}

int LN_FindEdge1_W2B(int n, double *data, double *edge, double *slope)
{
	int i,maxpos;
	double maxslope;
	double Temp_A, Temp_B;
	
	if(n<2)	return -1;

	maxslope=maxpos=0;
	for(i=1;i<n-1;i++)
		if( (data[i+1]-data[i-1])< maxslope)
		{
			maxpos=i;
			maxslope=(data[i+1]-data[i-1]);
		}
		
	if(maxslope==0) return -2;

	*slope=fabs(maxslope);
	if(maxpos<=2 || maxpos>=(n-3))
	{
		*edge=(double)maxpos;
	}
	else
	{
		if(fabs(data[maxpos+2]-data[maxpos]) < fabs(data[maxpos]-data[maxpos-2]))
		{
			maxpos-=1;
		}

		Temp_A=(double)fabs(data[maxpos+2]+data[maxpos-2]-2*data[maxpos]);
		Temp_B=(double)fabs(data[maxpos+3]+data[maxpos-1]-2*data[maxpos+1]);

		if((Temp_A+Temp_B)!=0)
			*edge=(double)maxpos+Temp_A/(Temp_A+Temp_B);	
		else
			*edge=(double)maxpos;
	}

	return 1;
}

int LN_FindEdge1_ABS(int n, double *data, double *edge, double *slope, int *dir)
{
	int i,maxpos;
	double maxslope;
	double Temp_A, Temp_B;
	
	if(n<2)	return -1;

	maxslope=maxpos=0;
	for(i=1;i<n-1;i++)
		if( fabs(data[i+1]-data[i-1])> maxslope)
		{
			maxpos=i;
			maxslope=fabs(data[i+1]-data[i-1]);
		}
		
	if(maxslope==0) return -2;

	*slope=fabs(maxslope);
	
	if(data[maxpos+1]>data[maxpos]) *dir=1; //Black->White
	else                            *dir=0;	//White->Black

	if(maxpos<=2 || maxpos>=(n-3))
	{
		*edge=(double)maxpos;
	}
	else
	{
		if(fabs(data[maxpos+2]-data[maxpos]) < fabs(data[maxpos]-data[maxpos-2]))
		{
			maxpos-=1;
		}

		Temp_A=(double)fabs(data[maxpos+2]+data[maxpos-2]-2*data[maxpos]);
		Temp_B=(double)fabs(data[maxpos+3]+data[maxpos-1]-2*data[maxpos+1]);

		if((Temp_A+Temp_B)!=0)
			*edge=(double)maxpos+Temp_A/(Temp_A+Temp_B);	
		else
			*edge=(double)maxpos;
	}

	return 1;
}

int LN_FindEdgePos1_B2W(LPBYTE fm, double x1, double y1, double x2, double y2, int nPitch,
						double *edgeX, double *edgeY, double *slope)
{
	int nData;
	double edge, fLineLength;

    fLineLength=(double)sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));

	nData=GetLineData(fm, x1, y1, x2, y2, nPitch, PixelX, PixelY, PixelV);
	if(nData<0) return -1;

	if(LN_FindEdge1_B2W(nData, PixelV, &edge, slope)<0) return -2;

	*edgeX=(x1*(fLineLength-edge)+x2*edge)/fLineLength;
	*edgeY=(y1*(fLineLength-edge)+y2*edge)/fLineLength;

	return 1;
}

int LN_FindEdgePos1_W2B(LPBYTE fm, double x1, double y1, double x2, double y2, int nPitch,
						double *edgeX, double *edgeY, double *slope)
{
	int nData;
	double edge, fLineLength;

    fLineLength=(double)sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));

	nData=GetLineData(fm, x1, y1, x2, y2, nPitch, PixelX, PixelY, PixelV);
	if(nData<0) return -1;

	if(LN_FindEdge1_W2B(nData, PixelV, &edge, slope)<0) return -2;

	*edgeX=(x1*(fLineLength-edge)+x2*edge)/fLineLength;
	*edgeY=(y1*(fLineLength-edge)+y2*edge)/fLineLength;

	return 1;
}

int LN_FindEdgePos1_ABS(LPBYTE fm, double x1, double y1, double x2, double y2, int nPitch,
						double *edgeX, double *edgeY, double *slope, int *dir)
{
	int nData;
	double edge, fLineLength;

    fLineLength=(double)sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));

	nData=GetLineData(fm, x1, y1, x2, y2, nPitch, PixelX, PixelY, PixelV);
	if(nData<0) return -1;

	if(LN_FindEdge1_ABS(nData, PixelV, &edge, slope, dir)<0) return -2;

	*edgeX=(x1*(fLineLength-edge)+x2*edge)/fLineLength;
	*edgeY=(y1*(fLineLength-edge)+y2*edge)/fLineLength;

	return 1;
}

int LN_FindEdge1st_B2W(int n, double *data, double slope, double *edge)
{
	int i, maxpos;
	double maxslope;
	double Temp_A, Temp_B;
	
	if(n<2)	return -1;

	maxpos=-1;
	for(i=1;i<n-1;i++)
		if( (data[i+1]-data[i-1]) >= slope)
		{
			maxslope=(data[i+1]-data[i-1]);
			maxpos=i;
			break;
		}
	
	if(maxpos<0) return -2;

	for(i=maxpos+1;i<n-1;i++)
	{
		if( (data[i+1]-data[i-1]) >= maxslope)
		{
			maxslope=(data[i+1]-data[i-1]);
			maxpos=i;
		}
		else 
			break;
	}		

	if(maxpos<=2 || maxpos>=(n-3))
	{
		*edge=(double)maxpos;
	}
	else
	{
		if(fabs(data[maxpos+2]-data[maxpos]) < fabs(data[maxpos]-data[maxpos-2]))
		{
			maxpos-=1;
		}

		Temp_A=(double)fabs(data[maxpos+2]+data[maxpos-2]-2*data[maxpos]);
		Temp_B=(double)fabs(data[maxpos+3]+data[maxpos-1]-2*data[maxpos+1]);

		if((Temp_A+Temp_B)!=0)
			*edge=(double)maxpos+Temp_A/(Temp_A+Temp_B);	
		else
			*edge=(double)maxpos;
	}

	return 1;
}

int LN_FindEdge1st_W2B(int n, double *data, double slope, double *edge)
{
	int i;
	int maxpos;
	double maxslope, Temp_A, Temp_B;
	
	if(n<2)	return -1;

	if(slope>0) slope*=-1;

	maxpos=-1;
	for(i=1;i<n-1;i++)
		if( (data[i+1]-data[i-1]) <= slope)
		{
			maxslope=(data[i+1]-data[i-1]);
			maxpos=i;
			break;
		}

	if(maxpos<0) return -2;

	for(i=maxpos+1;i<n-1;i++)
	{
		if( (data[i+1]-data[i-1]) <= maxslope)
		{
			maxslope=(data[i+1]-data[i-1]);
			maxpos=i;
		}
		else 
			break;
	}
		
	if(maxpos<=2 || maxpos>=(n-3))
	{
		*edge=(double)maxpos;
	}
	else
	{
		if(fabs(data[maxpos+2]-data[maxpos]) < fabs(data[maxpos]-data[maxpos-2]))
		{
			maxpos-=1;
		}

		Temp_A=(double)fabs(data[maxpos+2]+data[maxpos-2]-2*data[maxpos]);
		Temp_B=(double)fabs(data[maxpos+3]+data[maxpos-1]-2*data[maxpos+1]);


		if((Temp_A+Temp_B)!=0)
			*edge=(double)maxpos+Temp_A/(Temp_A+Temp_B);	
		else
			*edge=(double)maxpos;
	}

	return 1;
}

int LN_FindEdge1st_ABS(int n, double *data, double slope, double *edge, int *dir)
{
	int i;
	int maxpos;
	double Temp_A, Temp_B, maxslope;
	
	if(n<2)	return -1;

	maxpos=-1;
	for(i=1;i<n-1;i++)
		if( fabs(data[i+1]-data[i-1]) >= slope)
		{
			maxslope=fabs(data[i+1]-data[i-1]);
			maxpos=i;
			break;
		}

	if(maxpos<0) return -2;

	for(i=maxpos+1;i<n-1;i++)
	{
		if( fabs(data[i+1]-data[i-1]) >= maxslope)
		{
			maxslope=fabs(data[i+1]-data[i-1]);
			maxpos=i;
		}
		else 
			break;
	}
		
	if(data[maxpos+1]>data[maxpos]) *dir=1; //Black->White
	else                            *dir=0;	//White->Black

	if(maxpos<=2 || maxpos>=(n-3))
	{
		*edge=(double)maxpos;
	}
	else
	{
		if(fabs(data[maxpos+2]-data[maxpos]) < fabs(data[maxpos]-data[maxpos-2]))
		{
			maxpos-=1;
		}

		Temp_A=(double)fabs(data[maxpos+2]+data[maxpos-2]-2*data[maxpos]);
		Temp_B=(double)fabs(data[maxpos+3]+data[maxpos-1]-2*data[maxpos+1]);

		if((Temp_A+Temp_B)!=0)
			*edge=(double)maxpos+Temp_A/(Temp_A+Temp_B);	
		else
			*edge=(double)maxpos;
	}

	return 1;
}

int LN_FindEdgePos1st_B2W(LPBYTE fm, double x1, double y1, double x2, double y2, int nPitch, double slope,
						  double *edgeX, double *edgeY)
{
	int nData;
	double edge, fLineLength;

    fLineLength=(double)sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));

	nData=GetLineData(fm, x1, y1, x2, y2, nPitch, PixelX, PixelY, PixelV);
	if(nData<0) return -1;

	if(LN_FindEdge1st_B2W(nData, PixelV, slope, &edge)<0) return -2;

	*edgeX=(x1*(fLineLength-edge)+x2*edge)/fLineLength;
	*edgeY=(y1*(fLineLength-edge)+y2*edge)/fLineLength;

	return 1;
}

int LN_FindEdgePos1st_W2B(LPBYTE fm, double x1, double y1, double x2, double y2, int nPitch, double slope,
						  double *edgeX, double *edgeY)
{
	int nData;
	double edge, fLineLength;

    fLineLength=(double)sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));

	nData=GetLineData(fm, x1, y1, x2, y2, nPitch, PixelX, PixelY, PixelV);
	if(nData<0) return -1;

	if(LN_FindEdge1st_W2B(nData, PixelV, slope, &edge)<0) return -2;

	*edgeX=(x1*(fLineLength-edge)+x2*edge)/fLineLength;
	*edgeY=(y1*(fLineLength-edge)+y2*edge)/fLineLength;

	return 1;
}

int LN_FindEdgePos1st_ABS(LPBYTE fm, double x1, double y1, double x2, double y2, int nPitch, double slope,
						  double *edgeX, double *edgeY, int *dir)
{
	int nData;
	double edge, fLineLength;

    fLineLength=(double)sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));

	nData=GetLineData(fm, x1, y1, x2, y2, nPitch, PixelX, PixelY, PixelV);
	if(nData<0) return -1;

	if(LN_FindEdge1st_ABS(nData, PixelV, slope, &edge, dir)<0) return -2;

	*edgeX=(x1*(fLineLength-edge)+x2*edge)/fLineLength;
	*edgeY=(y1*(fLineLength-edge)+y2*edge)/fLineLength;

	return 1;
}


int LN_FindEdgeM_B2W(int n, double *data, double slope, double *edge)
{
	int i, nCount=0;
	int offset=0, flag;
	double  edge1;

	while(1)
	{
		if(LN_FindEdge1st_B2W(n-offset, &data[offset], slope, &edge1)<0) break;
		edge[nCount++]=(double)offset+edge1;
		flag=0;
		for(i=offset+(int)edge1+1;i<n-1;i++)
			if((data[i+1]-data[i-1]) <=0)
			{
				offset=i;
				flag=1;
				break;
			}
		if(!flag) break;
		if(nCount==MAXEDGENUMBER) break;
	}	
	
	return nCount;
}	
	
int LN_FindEdgeM_W2B(int n, double *data, double slope, double *edge)
{
	int i, nCount=0;
	int offset=0, flag;
	double  edge1;

	while(1)
	{
		if(LN_FindEdge1st_W2B(n-offset, &data[offset], slope, &edge1)<0) break;
		edge[nCount++]=(double)offset+edge1;
		flag=0;
		for(i=offset+(int)edge1+1;i<n-1;i++)
			if((data[i+1]-data[i+1]) >=0)
			{
				offset=i;
				flag=1;
				break;
			}
		if(!flag) break;
		if(nCount==MAXEDGENUMBER) break;
	}	
	
	return nCount;
}	

int LN_FindEdgeM_ABS(int n, double *data, double slope, double *edge, int *dir)
{
	int i, nCount=0;
	int offset=0, flag, dir1;
	double  edge1;

	while(1)
	{
		if(LN_FindEdge1st_ABS(n-offset, &data[offset], slope, &edge1, &dir1)<0) break;
		dir[nCount]=dir1;
		edge[nCount++]=(double)offset+edge1;
		flag=0;
		for(i=offset+(int)edge1+1;i<n-1;i++)
			if((data[i+1]-data[i])*(data[i]-data[i-1]) <= 0)
			{
				offset=i;
				flag=1;
				break;
			}
		if(!flag) break;
		if(nCount==MAXEDGENUMBER) break;
	}	
	
	return nCount;
}	

int LN_FindEdgePosM_B2W(LPBYTE fm, double x1, double y1, double x2, double y2, int nPitch, double slope,
						double *edgeX, double *edgeY)
{
	int i, nData;
	double fLineLength;
	int nEdge;

    fLineLength=(double)sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));

	nData=GetLineData(fm, x1, y1, x2, y2, nPitch, PixelX, PixelY, PixelV);
	if(nData<0) return -1;

	nEdge=LN_FindEdgeM_B2W(nData, PixelV, slope, EdgeM);

	for(i=0;i<nEdge;i++)
	{
		edgeX[i]=(x1*(fLineLength-EdgeM[i])+x2*EdgeM[i])/fLineLength;
		edgeY[i]=(y1*(fLineLength-EdgeM[i])+y2*EdgeM[i])/fLineLength;
	}
	return nEdge;
}

int LN_FindEdgePosM_W2B(LPBYTE fm, double x1, double y1, double x2, double y2, int nPitch, double slope,
						double *edgeX, double *edgeY)
{
	int i, nData;
	double fLineLength;
	int nEdge;

    fLineLength=(double)sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));

	nData=GetLineData(fm, x1, y1, x2, y2, nPitch, PixelX, PixelY, PixelV);
	if(nData<0) return -1;

	nEdge=LN_FindEdgeM_W2B(nData, PixelV, slope, EdgeM);

	for(i=0;i<nEdge;i++)
	{
		edgeX[i]=(x1*(fLineLength-EdgeM[i])+x2*EdgeM[i])/fLineLength;
		edgeY[i]=(y1*(fLineLength-EdgeM[i])+y2*EdgeM[i])/fLineLength;
	}
	return nEdge;
}

int LN_FindEdgePosM_ABS(LPBYTE fm, double x1, double y1, double x2, double y2, int nPitch, double slope,
						double *edgeX, double *edgeY, int *dir)
{
	int i, nData;
	double fLineLength;
	int nEdge;

    fLineLength=(double)sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));

	nData=GetLineData(fm, x1, y1, x2, y2, nPitch, PixelX, PixelY, PixelV);
	if(nData<0) return -1;

	nEdge=LN_FindEdgeM_ABS(nData, PixelV, slope, EdgeM, dir);

	for(i=0;i<nEdge;i++)
	{
		edgeX[i]=(x1*(fLineLength-EdgeM[i])+x2*EdgeM[i])/fLineLength;
		edgeY[i]=(y1*(fLineLength-EdgeM[i])+y2*EdgeM[i])/fLineLength;
	}
	return nEdge;
}

//return -1: (Left,top)-(right,bottom) Error
//		 -2: Memory Allocation Error
//       -3: Edge못찾음 
//		 
//        1: SUCCESS
int AR_FindEdge_T2B_B2W(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,
						double *edgeX, double *edgeY)
{
	int i, nCount=0;
	int nData=right-left+1;
	double slope;
	double *edgeX1, *edgeY1;
	double l_t, l_a, l_b;  //Line함수의 Parameter (t*y=a*x+b)
	
	if(nData<2) return -1;
	edgeX1 = (double*)malloc(nData*sizeof(double));
	edgeY1 = (double*)malloc(nData*sizeof(double));
	if(edgeX1==0 || edgeY1==0) return -2;

	for(i=left;i<=right;i++)
	{
		if(LN_FindEdgePos1_B2W(fm, i, top, i, bottom, nPitch,&edgeX1[nCount], &edgeY1[nCount], &slope)>0)
			nCount++;
	}

	if(nCount>=2)
	{
		if(LineFitting(nCount, edgeX1, edgeY1, (int)(nCount*0.5), 0.1, &l_t, &l_a, &l_b)<0)
			return -2;

		if(FindCrossPoint(l_t, l_a, l_b, 0, -1, (double)(left+right)/2.0,edgeX,edgeY)<0) return -3;
		if(*edgeX<left || *edgeX>right || *edgeY<top && *edgeY>bottom) return -3;

	}
	else return -3;

	if(edgeX1) free(edgeX1);
	if(edgeY1) free(edgeY1);
	return 1;
}

//return -1: (Left,top)-(right,bottom) Error
//		 -2: Memory Allocation Error
//       -3: Edge못찾음 
//		 
//        1: SUCCESS
int AR_FindEdge_T2B_W2B(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,
						double *edgeX, double *edgeY)
{
	int i, nCount=0;
	int nData=right-left+1;
	double slope;
	double *edgeX1, *edgeY1;
	double l_t, l_a, l_b;  //Line함수의 Parameter (t*y=a*x+b)
	
	if(nData<2) return -1;
	edgeX1 = (double*)malloc(nData*sizeof(double));
	edgeY1 = (double*)malloc(nData*sizeof(double));
	if(edgeX1==0 || edgeY1==0) return -2;

	for(i=left;i<=right;i++)
	{
		if(LN_FindEdgePos1_W2B(fm, i, top, i, bottom, nPitch,&edgeX1[nCount], &edgeY1[nCount], &slope)>0)
			nCount++;
	}

	if(nCount>=2)
	{
		if(LineFitting(nCount, edgeX1, edgeY1, (int)(nCount*0.5), 0.1, &l_t, &l_a, &l_b)<0)
			return -2;

		if(FindCrossPoint(l_t, l_a, l_b, 0, -1, (double)(left+right)/2.0,edgeX,edgeY)<0) return -3;
		if(*edgeX<left || *edgeX>right || *edgeY<top && *edgeY>bottom) return -3;
	}
	else return -3;

	if(edgeX1) free(edgeX1);
	if(edgeY1) free(edgeY1);
	return 1;
}

//return -1: (Left,top)-(right,bottom) Error
//		 -2: Memory Allocation Error
//       -3: Edge못찾음 
//		 
//        1: SUCCESS
int AR_FindEdge_L2R_B2W(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,
						double *edgeX, double *edgeY)
{
	int i, nCount=0;
	int nData=bottom-top+1;
	double slope;
	double *edgeX1, *edgeY1;
	double l_t, l_a, l_b;  //Line함수의 Parameter (t*y=a*x+b)
	
	if(nData<2) return -1;
	edgeX1 = (double*)malloc(nData*sizeof(double));
	edgeY1 = (double*)malloc(nData*sizeof(double));
	if(edgeX1==0 || edgeY1==0) return -2;

	for(i=top;i<=bottom;i++)
	{
		if(LN_FindEdgePos1_B2W(fm, left, i, right, i, nPitch,&edgeX1[nCount], &edgeY1[nCount], &slope)>0)
			nCount++;
	}

	if(nCount>=2)
	{
		if(LineFitting1(nCount, edgeX1, edgeY1, (int)(nCount*0.5), 0.1, &l_t, &l_a, &l_b)<0)
			return -2;

		if(FindCrossPoint(l_t, l_a, l_b, 1, 0, (double)(top+bottom)/2.0,edgeX,edgeY)<0) return -3;
		if(*edgeX<left || *edgeX>right || *edgeY<top && *edgeY>bottom) return -3;
	}
	else return -3;

	if(edgeX1) free(edgeX1);
	if(edgeY1) free(edgeY1);
	return 1;
}

//return -1: (Left,top)-(right,bottom) Error
//		 -2: Memory Allocation Error
//       -3: Edge못찾음 
//		 
//        1: SUCCESS
int AR_FindEdge_L2R_W2B(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,
						double *edgeX, double *edgeY)
{
	int i, nCount=0;
	int nData=bottom-top+1;
	double slope;
	double *edgeX1, *edgeY1;
	double l_t, l_a, l_b;  //Line함수의 Parameter (t*y=a*x+b)
	
	if(nData<2) return -1;
	edgeX1 = (double*)malloc(nData*sizeof(double));
	edgeY1 = (double*)malloc(nData*sizeof(double));
	if(edgeX1==0 || edgeY1==0) return -2;

	for(i=top;i<=bottom;i++)
	{
		if(LN_FindEdgePos1_W2B(fm, left, i, right, i, nPitch,&edgeX1[nCount], &edgeY1[nCount], &slope)>0)
			nCount++;
	}

	if(nCount>=2)
	{
		if(LineFitting1(nCount, edgeX1, edgeY1, (int)(nCount*0.5), 0.1, &l_t, &l_a, &l_b)<0)
			return -2;

		if(FindCrossPoint(l_t, l_a, l_b, 1, 0, (double)(top+bottom)/2.0,edgeX,edgeY)<0) return -3;
		if(*edgeX<left || *edgeX>right || *edgeY<top && *edgeY>bottom) return -3;
	}
	else return -3;

	if(edgeX1) free(edgeX1);
	if(edgeY1) free(edgeY1);
	return 1;
}
int AR_FindEdge_B2T_B2W(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,
						double *edgeX, double *edgeY)
{
	return AR_FindEdge_T2B_W2B(fm, left, top, right, bottom, nPitch,edgeX, edgeY);
}
int AR_FindEdge_B2T_W2B(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,
						double *edgeX, double *edgeY)
{
	return AR_FindEdge_T2B_B2W(fm, left, top, right, bottom, nPitch,edgeX, edgeY);
}
int AR_FindEdge_R2L_B2W(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,
						double *edgeX, double *edgeY)
{
	return AR_FindEdge_L2R_W2B(fm, left, top, right, bottom, nPitch,edgeX, edgeY);
}
int AR_FindEdge_R2L_W2B(LPBYTE fm, int left, int top, int right, int bottom, int nPitch,
						double *edgeX, double *edgeY)
{
	return AR_FindEdge_L2R_B2W(fm, left, top, right, bottom, nPitch,edgeX, edgeY);
}




/////////////////////////////////////////////////////////////////////////////////////
//
//	각종 LOCAL 함수들 
//
/////////////////////////////////////////////////////////////////////////////////////


// 함수설명 : 임의의 라인을 1Pixel 간격으로 ReSampling해주는 함수 
// return Value : 구한 좌표의 개수       
//               <0 FAILURE
int GetLineData(LPBYTE fm, double x1, double y1, double x2, double y2, int nPitch,
				double *pX, double *pY, double *pGrayValue)
{
	int i, nCount=0;
    double fLineLength;
	double  nAbsX1_X2, nAbsY1_Y2;
	double t=1.0;
    double slope;
	int  N, M ; // Interpolation Function에서 사용하는 변수들 
	double a,b;  // f(a,b)=P(N,M)(1-a)(1-b)+P(N+1,M)(a)(1-b)+p(N,M+1)(1-a)(b)+P(N+1,M+1)(a)(b)

    nAbsX1_X2 = fabs(x1-x2);
	nAbsY1_Y2 = fabs(y1-y2);

	// 라인의 길이 
    fLineLength=(double)sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
	if(fLineLength<2) return -1;

	if(nAbsX1_X2>0 && nAbsY1_Y2>0)
    {
		if(nAbsX1_X2 >= nAbsY1_Y2)  // 선이 x축으로 누운경우 
        {
            t=(double)nAbsX1_X2/fLineLength;
			if(x1>x2) t*=-1;
			slope = (double)(y1-y2)/(double)(x1-x2);
            for(i=0;i<=(int)fLineLength;i++)
            {
				pX[i]=(double)(x1+i*t);
				pY[i]=slope*(pX[i]-x1)+y1;  //직선의 방정식으로 부터 각 좌표를 구한다. 
            }
		}	
		else                     // 선이 y축으로 누운경우 
        {
            t=(double)nAbsY1_Y2/fLineLength;
			if(y1>y2) t*=-1;
			slope = (double)(x1-x2)/(double)(y1-y2);
            for(i=0;i<=(int)fLineLength;i++)
            {
				pY[i]=(double)(y1+i*t);
				pX[i]=slope*(pY[i]-y1)+x1; //직선의 방정식으로 부터 각 좌표를 구한다.
            }           
        }

        // 각 좌표에서의 픽셀값을 구한다.
		for(i=0;i<=(int)fLineLength;i++)
        {
			N=(int)pX[i];
			M=(int)pY[i];
			if(N>nPitch || M<=0 || M>g_System.m_nImageH) break;
			a=pX[i]-N;
			b=pY[i]-M;
			pGrayValue[i]=(int)(*(fm+M*nPitch+N)*(1.0-a)*(1.0-b)+*(fm+M*nPitch+N+1)*(a)*(1.0-b)+
								*(fm+(M+1)*nPitch+N)*(1.0-a)*(b)+*(fm+(M+1)*nPitch+N+1)*(a)*(b));
			nCount++;
        }

    }
	else if(nAbsY1_Y2==0)
	{
		if(x1>x2) t=-1.0;
        for(i=0;i<=(int)fLineLength;i++)
        {
			pX[i]=(double)(x1+i*t);
			pY[i]=(double)y1;  
			pGrayValue[i]=(double)*(fm+nPitch*(int)y1+(int)pX[i]);
			nCount++;
        }
	}
	else if(nAbsX1_X2==0)
	{
		if(y1>y2) t=-1.0;
        for(i=0;i<=(int)fLineLength;i++)
        {
			pY[i]=(double)(y1+i*t);
			pX[i]=(double) x1; 
			pGrayValue[i]=(double)*(fm+nPitch*(int)pY[i]+(int)x1);
			nCount++;
        }
	}

    return (nCount);
}

//Line Fitting
//ty=ax+b
//dataN : 주어진 (x,y)수
//*x, *y; (x,y)좌표 
//removeN : LineFitting후 Line에서 가장 멀리있는 점을 없애고 다시 fitting울 반복한다.
//          이 반복횟수를 나타낸다.
//ErrorLimit: LineFitting후 Line에서 가장 멀리있는 점이 이값 이하이면 반복을 중지한다.
//결과값 return: 1: 정상수행
//				 <0 : FAILURE
//              *t, *a, *b (ty=ax+b)

int LineFitting(int dataN, double *x, double *y, int removeN, double ErrorLimit,
				 double *t, double *a, double *b)
{
	int k,i,j;
	int nCount=0, MaxPos=0,*nValid, nStartN=0;  
	double xvalue,yvalue;
	double Sxx, Sx, Sy,Sxy, SxSy, dd, MaxDist=0,dist=0;
	double ZeroData=0.00001;

	Sx=Sy=Sxx=Sxy=SxSy=0.0;

	if(dataN<2) return -1;

	if(removeN>(dataN-2)) removeN=(dataN-2);

	nValid=(int*)malloc(dataN*sizeof(int));
	if(nValid==0) return -1;

	for(i=0;i<dataN;i++) nValid[i]=0;

	for(i=0;i<dataN;i++)
	{
		if(*(x+i)<0 || *(y+i)<0) 
		{
			nValid[i]=1;
			nStartN++;
		}
	}
	if(removeN<nStartN) removeN=nStartN+1;


	for(k=nStartN;k<removeN+1;k++)
	{
		if(k==nStartN)
		{
//			Sx=Sy=Sxx=Sxy=SxSy=0.0;
			for(i=0;i<dataN;i++)
			{
				if(nValid[i]==0) //x, y 0보다 작지 않은 것만 사용한다.
				{
					xvalue=*(x+i);
					yvalue=*(y+i);
					Sx+=xvalue;
					Sy+=yvalue;
					Sxx+=xvalue*xvalue;
					Sxy+=xvalue*yvalue;
				}
			}
		}
		else
		{
			xvalue=*(x+MaxPos);
			yvalue=*(y+MaxPos);
			Sx-=xvalue;
			Sy-=yvalue;
			Sxx-=xvalue*xvalue;
			Sxy-=xvalue*yvalue;		
		}

		nCount=dataN-k;
		SxSy=Sx*Sy;

		dd=nCount*Sxx-Sx*Sx;
		if(fabs(dd)<ZeroData)
		{
			*t=0;
			*a=1;
			*b=-Sx/nCount;
			goto done;
		}
		else
		{
			*t=1;
			*a=(nCount*Sxy-Sx*Sy)/dd;
			*b=(-Sxy*Sx+Sy*Sxx)/dd;
		}

		dd=sqrt(*a**a+1);
		MaxDist=0;
		for(j=0;j<dataN;j++)
		{
			if(nValid[j]) continue;
			dist=fabs(-*(y+j)+*a**(x+j)+*b);
			if(dist>MaxDist)
			{
				MaxDist=dist;
				MaxPos=j;
			}
		}
		if(MaxDist/dd<ErrorLimit) goto done;
		else nValid[MaxPos]=1;
	}
done:
	if(nValid) free(nValid);
	return 1;
}


int LineFitting_original(int dataN, double *x, double *y, int removeN, double ErrorLimit,
				 double *t, double *a, double *b)
{
	int k,i,j;
	int nCount, MaxPos,*nValid, nStartN=0;  
	double xvalue,yvalue;
	double Sxx, Sx, Sy,Sxy, SxSy, dd, MaxDist=0,dist;
	double ZeroData=0.00001;
	if(dataN<2) return -1;

	if(removeN>(dataN-2)) removeN=(dataN-2);

	nValid=(int*)malloc(dataN*sizeof(int));
	if(nValid==0) return -1;

	for(i=0;i<dataN;i++) nValid[i]=0;	

	for(k=0;k<removeN+1;k++)
	{
		if(k==0)
		{
			Sx=Sy=Sxx=Sxy=SxSy=0.0;
			for(i=0;i<dataN;i++)
			{			
				xvalue=*(x+i);
				yvalue=*(y+i);
				Sx+=xvalue;
				Sy+=yvalue;
				Sxx+=xvalue*xvalue;
				Sxy+=xvalue*yvalue;
			}
		}
		else
		{
			xvalue=*(x+MaxPos);
			yvalue=*(y+MaxPos);
			Sx-=xvalue;
			Sy-=yvalue;
			Sxx-=xvalue*xvalue;
			Sxy-=xvalue*yvalue;		
		}

		nCount=dataN-k;
		SxSy=Sx*Sy;

		dd=nCount*Sxx-Sx*Sx;
		if(fabs(dd)<ZeroData)
		{
			*t=0;
			*a=1;
			*b=-Sx/nCount;
			goto done;
		}
		else
		{
			*t=1;
			*a=(nCount*Sxy-Sx*Sy)/dd;
			*b=(-Sxy*Sx+Sy*Sxx)/dd;
		}

		dd=sqrt(*a**a+1);
		MaxDist=0;
		for(j=0;j<dataN;j++)
		{
			if(nValid[j]) continue;
			dist=fabs(-*(y+j)+*a**(x+j)+*b);
			if(dist>MaxDist)
			{
				MaxDist=dist;
				MaxPos=j;
			}
		}
		if(MaxDist/dd<ErrorLimit) goto done;
		else nValid[MaxPos]=1;
	}
done:
	if(nValid) free(nValid);
	return 1;
}

//1st Line Fitting(Line이 거의 수직일때)
// return 1: SUCCESS
//        -1: FAILURE
int LineFitting1(int dataN, double *x, double *y, int removeN, double ErrorLimit,
				 double *t, double *a, double *b)
{
	int returnValue;
	double la,lb,lt;

	returnValue=LineFitting(dataN,y, x, removeN, ErrorLimit, &lt,&la,&lb);

	if(returnValue==1)
	{
		if(fabs(la)<0.0001)  //기울기가 10000이상은 y축에 평행한것으로 간주한다.
		{
			*t=0;
			*a=lt;
			*b=-lb;
		}
		else
		{
			*t=1;
			*a=lt/la;
			*b=-lb/la;
		}	
		return 1;
	}
	else return returnValue;
}

// 두개의 직선으로부터 Cross하는 점 구하기
//t1y=a1x+b1
//t2y=a2x+b2
//return 0: SUCCESS
//       -1: FAILURE
int FindCrossPoint(double t1, double a1, double b1, double t2, double a2, double b2, double *cx, double *cy)
{
	double local_LIMIT=1e-10;
	double dd;

	dd=-a1*t2+a2*t1;

	if(fabs(dd)<local_LIMIT) return -1;

	*cx=(t2*b1-t1*b2)/dd;
	*cy=(a2*b1-a1*b2)/dd;

	return 0;
}


//
//왼쪽에서 오른쪽으로 검정색을 검출
//bottom은 포함이 안됨. 따라서 0~255까지 하려면 Top=0, bottom=256을 넣어야 함.
int FindEdge1st_L2R_W2B(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nGap, int nDiffValue,int nDiffValueL, double *pEdge)
{
	int i, j;
	int n3Value=nDiffValue*3;
	int nTmp,nMaxTmp,maxpos, nTmp3,  nEdge=0;
	int nEnd;
	double Temp_A, Temp_B;

	for(i=top;i<bottom;i++)
	{
		pEdge[i-top]=-1;
		nEdge=-1;
		for(j=left;j<right-nGap;j++)
		{
			nTmp=*(fm+pitch*i+j)-*(fm+pitch*i+j+nGap);
			if(nTmp>nDiffValueL)
			{

				nEdge=j;
				break;
			}
			else if(nTmp>=nDiffValue)
			{
				if(i==top)				nTmp3=nTmp+2*(*(fm+pitch*(i+1)+j)-*(fm+pitch*(i+1)+j+nGap));
				else if(i==(bottom-1))  nTmp3=nTmp+2*(*(fm+pitch*(i-1)+j)-*(fm+pitch*(i-1)+j+nGap));
				else				    nTmp3=nTmp+*(fm+pitch*(i+1)+j)-*(fm+pitch*(i+1)+j+nGap)+*(fm+pitch*(i-1)+j)-*(fm+pitch*(i-1)+j+nGap);

				if(nTmp3>=n3Value)
				{
					nEdge=j;
					break;
				}
			}			
		}
		if(nEdge>0)
		{
			maxpos=-1;
			nMaxTmp=0;
			nEnd=nEdge+5; if(nEnd>right-2) nEnd=right-2;
			for(j=nEdge;j<nEnd;j++)
			{
				nTmp=*(fm+pitch*i+j)-*(fm+pitch*i+j+2);
				if(nTmp>nMaxTmp)
				{
					nMaxTmp=nTmp;
					maxpos=j+1;
				}
			}
			if(maxpos<=left+2 || maxpos>=(right-3))
			{
				pEdge[i-top]=(double)maxpos;
			}
			else if(maxpos>0)
			{

				if(abs(*(fm+pitch*i+maxpos+2)-*(fm+pitch*i+maxpos)) < abs(*(fm+pitch*i+maxpos)-*(fm+pitch*i+maxpos-2)))
				{
					maxpos-=1;
				}
				Temp_A=(double)abs(*(fm+pitch*i+maxpos+2)+*(fm+pitch*i+maxpos-2)-2**(fm+pitch*i+maxpos));
				Temp_B=(double)abs(*(fm+pitch*i+maxpos+3)+*(fm+pitch*i+maxpos-1)-2**(fm+pitch*i+maxpos+1));

				if((Temp_A+Temp_B)!=0)
					pEdge[i-top]=(double)maxpos+Temp_A/(Temp_A+Temp_B);	
				else
					pEdge[i-top]=(double)maxpos;
			}
		}
	}

	return (bottom-top);
}
int FindEdge1st_L2R_B2W(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nGap, int nDiffValue,int nDiffValueL, double *pEdge)
{
	int i, j;
	int n3Value=nDiffValue*3;
	int nTmp,nMaxTmp,maxpos, nTmp3,  nEdge=0;
	int nEnd;
	double Temp_A, Temp_B;

	for(i=top;i<bottom;i++)
	{
		pEdge[i-top]=-1;
		nEdge=-1;
		for(j=left;j<right-nGap;j++)
		{
			nTmp=*(fm+pitch*i+j+nGap)-*(fm+pitch*i+j);
			if(nTmp>nDiffValueL) //
			{

				nEdge=j;
				break;
			}
			else if(nTmp>=nDiffValue)
			{
				if(i==top)				nTmp3=nTmp+2*(*(fm+pitch*(i+1)+j+nGap)-*(fm+pitch*(i+1)+j));
				else if(i==(bottom-1))  nTmp3=nTmp+2*(*(fm+pitch*(i-1)+j+nGap)-*(fm+pitch*(i-1)+j));
				else				    nTmp3=nTmp-*(fm+pitch*(i+1)+j)+*(fm+pitch*(i+1)+j+nGap)-*(fm+pitch*(i-1)+j)+*(fm+pitch*(i-1)+j+nGap);

				if(nTmp3>=n3Value)
				{
					nEdge=j;
					break;
				}
			}			
		}
		if(nEdge>0)
		{
			maxpos=-1;
			nMaxTmp=0;
			nEnd=nEdge+5; if(nEnd>right-2) nEnd=right-2;
			for(j=nEdge;j<nEnd;j++)
			{
				nTmp=*(fm+pitch*i+j+2)-*(fm+pitch*i+j);
				if(nTmp>nMaxTmp)
				{
					nMaxTmp=nTmp;
					maxpos=j+1;
				}
			}
			if(maxpos<=left+2 || maxpos>=(right-3))
			{
				pEdge[i-top]=(double)maxpos;
			}
			else if(maxpos>0)
			{

				if(abs(*(fm+pitch*i+maxpos+2)-*(fm+pitch*i+maxpos)) < abs(*(fm+pitch*i+maxpos)-*(fm+pitch*i+maxpos-2)))
				{
					maxpos-=1;
				}
				Temp_A=(double)abs(*(fm+pitch*i+maxpos+2)+*(fm+pitch*i+maxpos-2)-2**(fm+pitch*i+maxpos));
				Temp_B=(double)abs(*(fm+pitch*i+maxpos+3)+*(fm+pitch*i+maxpos-1)-2**(fm+pitch*i+maxpos+1));

				if((Temp_A+Temp_B)!=0)
					pEdge[i-top]=(double)maxpos+Temp_A/(Temp_A+Temp_B);	
				else
					pEdge[i-top]=(double)maxpos;
			}
		}
	}

	return (bottom-top);
}

//오른쪽에서 왼쪽으로 검정색을 검출
//bottom은 포함이 안됨. 따라서 0~255까지 하려면 Top=0, bottom=256을 넣어야 함.
int FindEdge1st_R2L_W2B(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nGap, int nDiffValue,int nDiffValueL, double *pEdge)
{
	int i, j;
	int n3Value=nDiffValue*3;
	int nTmp,nMaxTmp,maxpos, nTmp3,  nEdge=0;
	int nEnd;
	double Temp_A, Temp_B;

	for(i=top;i<bottom;i++)
	{
		pEdge[i-top]=-1;
		nEdge=-1;
		for(j=right-1;j>=left+nGap;j--)
		{
			nTmp=*(fm+pitch*i+j)-*(fm+pitch*i+j-nGap);
			if(nTmp>nDiffValueL)
			{
				nEdge=j;
				break;
			}
			else if(nTmp>=nDiffValue)
			{
				if(i==top)				nTmp3=nTmp+2*(*(fm+pitch*(i+1)+j)-*(fm+pitch*(i+1)+j-nGap));
				else if(i==(bottom-1))  nTmp3=nTmp+2*(*(fm+pitch*(i-1)+j)-*(fm+pitch*(i-1)+j-nGap));
				else				    nTmp3=nTmp+*(fm+pitch*(i+1)+j)-*(fm+pitch*(i+1)+j-nGap)+*(fm+pitch*(i-1)+j)-*(fm+pitch*(i-1)+j-nGap);

				if(nTmp3>=n3Value)
				{
					nEdge=j;
					break;
				}
			}
		}
		if(nEdge>0)
		{			
			maxpos=-1;
			nMaxTmp=0;
			nEnd=nEdge-5; if(nEnd<left+2) nEnd=left+2;
			for(j=nEdge;j>=nEnd;j--)
			{
				nTmp=*(fm+pitch*i+j)-*(fm+pitch*i+j-2);
				if(nTmp>nMaxTmp)
				{
					nMaxTmp=nTmp;
					maxpos=j-1;
				}
			}
		
			if(maxpos>=right-2 || maxpos<=left+3)
			{
				pEdge[i-top]=(double)maxpos;
			}
			else if(maxpos>0)
			{
				if(abs(*(fm+pitch*i+maxpos-2)-*(fm+pitch*i+maxpos)) < abs(*(fm+pitch*i+maxpos)-*(fm+pitch*i+maxpos+2)))
				{
					maxpos+=1;
				}
				Temp_A=(double)abs(*(fm+pitch*i+maxpos-2)+*(fm+pitch*i+maxpos+2)-2**(fm+pitch*i+maxpos));
				Temp_B=(double)abs(*(fm+pitch*i+maxpos-3)+*(fm+pitch*i+maxpos+1)-2**(fm+pitch*i+maxpos-1));

				if((Temp_A+Temp_B)!=0)
					pEdge[i-top]=(double)maxpos-Temp_A/(Temp_A+Temp_B);	
				else
					pEdge[i-top]=(double)maxpos;
			}
		}
	}

	return (bottom-top);
}
int FindEdge1st_R2L_B2W(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nGap, int nDiffValue,int nDiffValueL, double *pEdge)
{
	int i, j;
	int n3Value=nDiffValue*3;
	int nTmp,nMaxTmp,maxpos, nTmp3,  nEdge=0;
	int nEnd;
	double Temp_A, Temp_B;

	for(i=top;i<bottom;i++)
	{
		pEdge[i-top]=-1;
		nEdge=-1;
		for(j=right-1;j>=left+nGap;j--)
		{
			nTmp=*(fm+pitch*i+j-nGap)-*(fm+pitch*i+j);
			if(nTmp>nDiffValueL)
			{
				nEdge=j;
				break;
			}
			else if(nTmp>=nDiffValue)
			{
				if(i==top)				nTmp3=nTmp+2*(*(fm+pitch*(i+1)+j-nGap)-*(fm+pitch*(i+1)+j));
				else if(i==(bottom-1))  nTmp3=nTmp+2*(*(fm+pitch*(i-1)+j-nGap)-*(fm+pitch*(i-1)+j));
				else				    nTmp3=nTmp-*(fm+pitch*(i+1)+j)+*(fm+pitch*(i+1)+j-nGap)-*(fm+pitch*(i-1)+j)+*(fm+pitch*(i-1)+j-nGap);

				if(nTmp3>=n3Value)
				{
					nEdge=j;
					break;
				}
			}
		}
		if(nEdge>0)
		{			
			maxpos=-1;
			nMaxTmp=0;
			nEnd=nEdge-5; if(nEnd<left+2) nEnd=left+2;
			for(j=nEdge;j>=nEnd;j--)
			{
				nTmp=*(fm+pitch*i+j-2)-*(fm+pitch*i+j);
				if(nTmp>nMaxTmp)
				{
					nMaxTmp=nTmp;
					maxpos=j-1;
				}
			}
		
			if(maxpos>=right-2 || maxpos<=left+3)
			{
				pEdge[i-top]=(double)maxpos;
			}
			else if(maxpos>0)
			{
				if(abs(*(fm+pitch*i+maxpos-2)-*(fm+pitch*i+maxpos)) < abs(*(fm+pitch*i+maxpos)-*(fm+pitch*i+maxpos+2)))
				{
					maxpos+=1;
				}
				Temp_A=(double)abs(*(fm+pitch*i+maxpos-2)+*(fm+pitch*i+maxpos+2)-2**(fm+pitch*i+maxpos));
				Temp_B=(double)abs(*(fm+pitch*i+maxpos-3)+*(fm+pitch*i+maxpos+1)-2**(fm+pitch*i+maxpos-1));

				if((Temp_A+Temp_B)!=0)
					pEdge[i-top]=(double)maxpos-Temp_A/(Temp_A+Temp_B);	
				else
					pEdge[i-top]=(double)maxpos;
			}
		}
	}

	return (bottom-top);
}


//위쪽에서 아래쪽으로 검정색을 검출
//right은 포함이 안됨. 따라서 0~255까지 하려면 left=0, right=256을 넣어야 함.
int FindEdge1st_T2B_W2B(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nGap, int nDiffValue,int nDiffValueL, double *pEdge)
{
	int i, j;
	int n3Value=nDiffValue*3;
	int nTmp,nMaxTmp,maxpos, nTmp3,  nEdge=0;
	int nEnd;
	double Temp_A, Temp_B;

	for(j=left;j<right;j++)
	{
		pEdge[j-left]=-1;
		nEdge=-1;
		for(i=top;i<bottom-nGap;i++)
		{
			nTmp=*(fm+pitch*i+j)-*(fm+pitch*(i+nGap)+j);
			if(nTmp>nDiffValueL)
			{

				nEdge=i;
				break;
			}
			else if(nTmp>=nDiffValue)
			{
				if(j==left)				nTmp3=nTmp+2*(*(fm+pitch*i+j+1)-*(fm+pitch*(i+nGap)+j+1));
				else if(j==(right-1))   nTmp3=nTmp+2*(*(fm+pitch*i+j-1)-*(fm+pitch*(i+nGap)+j-1));
				else				    nTmp3=nTmp+*(fm+pitch*i+j+1)-*(fm+pitch*(i+nGap)+j+1)+*(fm+pitch*i+j-1)-*(fm+pitch*(i+nGap)+j-1);

				if(nTmp3>=n3Value)
				{
					nEdge=i;
					break;
				}
			}
		}
		if(nEdge>0)
		{
			maxpos=-1;
			nMaxTmp=0;
			nEnd=nEdge+5; if(nEnd>bottom-2) nEnd=bottom-2;
			for(i=nEdge;i<nEnd;i++)
			{
				nTmp=*(fm+pitch*i+j)-*(fm+pitch*(i+2)+j);
				if(nTmp>nMaxTmp)
				{
					nMaxTmp=nTmp;
					maxpos=i+1;
				}
			}
			if(maxpos<=top+2 || maxpos>=(bottom-3))
			{
				pEdge[j-left]=(double)maxpos;
			}
			else if(maxpos>0)
			{
				if(abs(*(fm+pitch*(maxpos+2)+j)-*(fm+pitch*maxpos+j)) < abs(*(fm+pitch*maxpos+j)-*(fm+pitch*(maxpos-2)+j)))
				{
					maxpos-=1;
				}

				Temp_A=(double)abs(*(fm+pitch*(maxpos+2)+j)+*(fm+pitch*(maxpos-2)+j)-2**(fm+pitch*maxpos+j));
				Temp_B=(double)abs(*(fm+pitch*(maxpos+3)+j)+*(fm+pitch*(maxpos-1)+j)-2**(fm+pitch*(maxpos+1)+j));

				if((Temp_A+Temp_B)!=0)
					pEdge[j-left]=(double)maxpos+Temp_A/(Temp_A+Temp_B);	
				else
					pEdge[j-left]=(double)maxpos;
			}
		}
	}

	return (right-left);
}
int FindEdge1st_T2B_B2W(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nGap, int nDiffValue,int nDiffValueL, double *pEdge)
{
	int i, j;
	int n3Value=nDiffValue*3;
	int nTmp,nMaxTmp,maxpos, nTmp3,  nEdge=0;
	int nEnd;
	double Temp_A, Temp_B;

	for(j=left;j<right;j++)
	{
		pEdge[j-left]=-1;
		nEdge=-1;
		for(i=top;i<bottom-nGap;i++)
		{
			nTmp=*(fm+pitch*(i+nGap)+j)-*(fm+pitch*i+j);
			if(nTmp>nDiffValueL)
			{

				nEdge=i;
				break;
			}
			else if(nTmp>=nDiffValue)
			{
				if(j==left)				nTmp3=nTmp+2*(*(fm+pitch*(i+nGap)+j+1)-*(fm+pitch*i+j+1));
				else if(j==(right-1))   nTmp3=nTmp+2*(*(fm+pitch*(i+nGap)+j-1)-*(fm+pitch*i+j-1));
				else				    nTmp3=nTmp-*(fm+pitch*i+j+1)+*(fm+pitch*(i+nGap)+j+1)-*(fm+pitch*i+j-1)+*(fm+pitch*(i+nGap)+j-1);

				if(nTmp3>=n3Value)
				{
					nEdge=i;
					break;
				}
			}
		}
		if(nEdge>0)
		{
			maxpos=-1;
			nMaxTmp=0;
			nEnd=nEdge+5; if(nEnd>bottom-2) nEnd=bottom-2;
			for(i=nEdge;i<nEnd;i++)
			{
				nTmp=*(fm+pitch*(i+2)+j)-*(fm+pitch*i+j);
				if(nTmp>nMaxTmp)
				{
					nMaxTmp=nTmp;
					maxpos=i+1;
				}
			}
			if(maxpos<=top+2 || maxpos>=(bottom-3))
			{
				pEdge[j-left]=(double)maxpos;
			}
			else if(maxpos>0)
			{
				if(abs(*(fm+pitch*(maxpos+2)+j)-*(fm+pitch*maxpos+j)) < abs(*(fm+pitch*maxpos+j)-*(fm+pitch*(maxpos-2)+j)))
				{
					maxpos-=1;
				}

				Temp_A=(double)abs(*(fm+pitch*(maxpos+2)+j)+*(fm+pitch*(maxpos-2)+j)-2**(fm+pitch*maxpos+j));
				Temp_B=(double)abs(*(fm+pitch*(maxpos+3)+j)+*(fm+pitch*(maxpos-1)+j)-2**(fm+pitch*(maxpos+1)+j));

				if((Temp_A+Temp_B)!=0)
					pEdge[j-left]=(double)maxpos+Temp_A/(Temp_A+Temp_B);	
				else
					pEdge[j-left]=(double)maxpos;
			}
		}
	}

	return (right-left);
}
//아래쪽에서 위쪽으로 검정색을 검출
//right은 포함이 안됨. 따라서 0~255까지 하려면 left=0, right=256을 넣어야 함.
int FindEdge1st_B2T_W2B(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nGap, int nDiffValue,int nDiffValueL, double *pEdge)
{
	int i, j;
	int n3Value=nDiffValue*3;
	int nTmp,nMaxTmp,maxpos, nTmp3,  nEdge=0;
	int nEnd;
	double Temp_A, Temp_B;

	for(j=left;j<right;j++)
	{
		pEdge[j-left]=-1;
		nEdge=-1;
		for(i=bottom-1;i>=top+nGap;i--)
		{
			nTmp=*(fm+pitch*i+j)-*(fm+pitch*(i-nGap)+j);
			if(nTmp>nDiffValueL)
			{

				nEdge=i;
				break;
			}
			else if(nTmp>=nDiffValue)
			{
				if(j==left)				nTmp3=nTmp+2*(*(fm+pitch*i+j+1)-*(fm+pitch*(i-nGap)+j+1));
				else if(j==(right-1))   nTmp3=nTmp+2*(*(fm+pitch*i+j-1)-*(fm+pitch*(i-nGap)+j-1));
				else				    nTmp3=nTmp+*(fm+pitch*i+j+1)-*(fm+pitch*(i-nGap)+j+1)+*(fm+pitch*i+j-1)-*(fm+pitch*(i-nGap)+j-1);

				if(nTmp3>=n3Value)
				{
					nEdge=i;
					break;
				}
			}
		}
		if(nEdge>0)
		{
			maxpos=-1;
			nMaxTmp=0;
			nEnd=nEdge-5; if(nEnd<top+2) nEnd=top+2;
			for(i=nEdge;i>=nEnd;i--)
			{
				nTmp=*(fm+pitch*i+j)-*(fm+pitch*(i-2)+j);
				if(nTmp>nMaxTmp)
				{
					nMaxTmp=nTmp;
					maxpos=i-1;
				}
			}
			if(maxpos>=bottom+2 || maxpos<=(top-3))
			{
				pEdge[j-left]=(double)maxpos;
			}
			else if(maxpos>0)
			{
				if(abs(*(fm+pitch*(maxpos-2)+j)-*(fm+pitch*maxpos+j)) < abs(*(fm+pitch*maxpos+j)-*(fm+pitch*(maxpos+2)+j)))
				{
					maxpos+=1;
				}

				Temp_A=(double)abs(*(fm+pitch*(maxpos-2)+j)+*(fm+pitch*(maxpos+2)+j)-2**(fm+pitch*maxpos+j));
				Temp_B=(double)abs(*(fm+pitch*(maxpos-3)+j)+*(fm+pitch*(maxpos+1)+j)-2**(fm+pitch*(maxpos-1)+j));

				if((Temp_A+Temp_B)!=0)
					pEdge[j-left]=(double)maxpos-Temp_A/(Temp_A+Temp_B);	
				else
					pEdge[j-left]=(double)maxpos;
			}
		}
	}

	return (right-left);
}

int FindEdge1st_B2T_B2W(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nGap, int nDiffValue,int nDiffValueL, double *pEdge)
{
	int i, j;
	int n3Value=nDiffValue*3;
	int nTmp,nMaxTmp,maxpos, nTmp3,  nEdge=0;
	int nEnd;
	double Temp_A, Temp_B;

	for(j=left;j<right;j++)
	{
		pEdge[j-left]=-1;
		nEdge=-1;
		for(i=bottom-1;i>=top+nGap;i--)
		{
			nTmp=*(fm+pitch*(i-nGap)+j)-*(fm+pitch*i+j);
			if(nTmp>nDiffValueL)
			{
				nEdge=i;
				break;
			}
			else if(nTmp>=nDiffValue)
			{
				if(j==left)				nTmp3=nTmp+2*(*(fm+pitch*(i-nGap)+j+1)-*(fm+pitch*i+j+1));
				else if(j==(right-1))   nTmp3=nTmp+2*(*(fm+pitch*(i-nGap)+j-1)-*(fm+pitch*i+j-1));
				else				    nTmp3=nTmp-*(fm+pitch*i+j+1)+*(fm+pitch*(i-nGap)+j+1)-*(fm+pitch*i+j-1)+*(fm+pitch*(i-nGap)+j-1);

				if(nTmp3>=n3Value)
				{
					nEdge=i;
					break;
				}
			}
		}
		if(nEdge>0)
		{
			maxpos=-1;
			nMaxTmp=0;
			nEnd=nEdge-5; if(nEnd<top+2) nEnd=top+2;
			for(i=nEdge;i>=nEnd;i--)
			{
				
				nTmp=*(fm+pitch*(i-2)+j)-*(fm+pitch*i+j);
				if(nTmp>nMaxTmp)
				{
					nMaxTmp=nTmp;
					maxpos=i-1;
				}
			}
			if(maxpos>=bottom+2 || maxpos<=(top-3))
			{
				pEdge[j-left]=(double)maxpos;
			}
			else if(maxpos>0)
			{
				if(abs(*(fm+pitch*(maxpos-2)+j)-*(fm+pitch*maxpos+j)) < abs(*(fm+pitch*maxpos+j)-*(fm+pitch*(maxpos+2)+j)))
				{
					maxpos+=1;
				}

				Temp_A=(double)abs(*(fm+pitch*(maxpos-2)+j)+*(fm+pitch*(maxpos+2)+j)-2**(fm+pitch*maxpos+j));
				Temp_B=(double)abs(*(fm+pitch*(maxpos-3)+j)+*(fm+pitch*(maxpos+1)+j)-2**(fm+pitch*(maxpos-1)+j));

				if((Temp_A+Temp_B)!=0)
					pEdge[j-left]=(double)maxpos-Temp_A/(Temp_A+Temp_B);	
				else
					pEdge[j-left]=(double)maxpos;
			}
		}
	}

	return (right-left);
}


int FindVerLine(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch, int nSlope, int nCorner_Pos,
				int nObject_Color, int nSkip, int nRotate_Range, double *x, double *x_use_ratio, double *tt, double *aa, double *bb)
{
	int i, j;
	int nResult=-1;
	int nXEdgeTotal=0;
	int nXEdgeCnt=0;
	int nReturn, tmp, color;
	double slope;
	double dEdge, t1,a1,b1,t2,a2,b2;
	double y;


	if(nSkip<=0) nSkip=1;

//리턴value 초기화
	*x=*x_use_ratio=*tt=*bb=*aa=0;

	for(i=y1;i<y2;i+=nSkip)
		nXEdgeTotal++;
	
	if(nXEdgeTotal<2) goto done;

//옆면의 Edge를 구한다.
	for(i=y1;i<y2;i+=nSkip)
	{
		if(nCorner_Pos==0)
		{
			for(j=x1;j<x2;j++)
				PixelV[j-x1]=*(fm+nPitch*i+j);
		}
		else
		{
			for(j=x2-1;j>=x1;j--)
				PixelV[x2-1-j]=*(fm+nPitch*i+j);
		}

		if( nObject_Color>0) //흰색 	   
		{
			if(nSlope==0)
				nReturn=LN_FindEdge1_B2W((x2-x1), PixelV, &dEdge, &slope);
			else
				nReturn=LN_FindEdge1st_B2W((x2-x1), PixelV, nSlope, &dEdge);
		}
		else
		{
			if(nSlope==0)
				nReturn=LN_FindEdge1_W2B((x2-x1), PixelV, &dEdge, &slope);
			else
				nReturn=LN_FindEdge1st_W2B((x2-x1), PixelV, nSlope, &dEdge);
		}

		if(nReturn==1)
		{
			if(nCorner_Pos==0)
				dVerX[nXEdgeCnt]=(double)x1+dEdge;
			else
				dVerX[nXEdgeCnt]=(double)x2-1-dEdge;

			dVerY[nXEdgeCnt++]=(double)i;
		}
	}


	nXEdgeCnt=ChooseGoodData_FirstBase(nXEdgeCnt, dVerX, dVerY, nRotate_Range);

	if(nXEdgeCnt<2 ) goto done;

	if(LineFitting1(nXEdgeCnt, dVerX, dVerY, (int)(nXEdgeCnt*0.5), 2, &t1, &a1, &b1)<=0) goto done;

	if(FindCrossPoint(t1, a1, b1, 1, 0, (y1+y2)/2, x, &y)<0) goto done;
	if(*x<x1 || *x>x2)  goto done;

	*tt=t1;
	*aa=a1;
	*bb=b1;
	*x_use_ratio=(double)nXEdgeCnt/(double)nXEdgeTotal;


	nResult=0;
done:
	return nResult;
}

int FindVerLineByGrayValue(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch, int nThres, int nCorner_Pos,
				int nObject_Color, int nSkip, int nRotate_Range, double *x, double *x_use_ratio, double *tt, double *aa, double *bb)
{
	int i, j;
	int nResult=-1;
	int nXEdgeTotal=0;
	int nXEdgeCnt=0;
	int nReturn, tmp, color;
	double slope;
	double dEdge, t1,a1,b1;
	double y;


	if(nSkip<=0) nSkip=1;

//리턴value 초기화
	*x=*x_use_ratio=*tt=*bb=*aa=0;

	for(i=y1;i<y2;i+=nSkip)
		nXEdgeTotal++;
	
	if(nXEdgeTotal<2) goto done;

//옆면의 Edge를 구한다.
	for(i=y1;i<y2;i+=nSkip)
	{
		if(nCorner_Pos==0)
		{
			dEdge=x1;
			if( nObject_Color>0)
			{
				for(j=x1;j<x2;j++)
					if(*(fm+nPitch*i+j)>=nThres)
					{
						dEdge=j;
						break;
					}
			}
			else
			{
				for(j=x1;j<x2;j++)
					if(*(fm+nPitch*i+j)<=nThres)
					{
						dEdge=j;
						break;
					}

			}
		}
		else
		{
			dEdge=y1;
			if( nObject_Color>0)
			{
				for(j=x2-1;j>=x1;j--)
					if(*(fm+nPitch*i+j)>=nThres)
					{
						dEdge=j;
						break;
					}
			}
			else
			{
				for(j=x2-1;j>=x1;j--)
					if(*(fm+nPitch*i+j)<=nThres)
					{
						dEdge=j;
						break;
					}

			}
		}

		dVerX[nXEdgeCnt]=(double)dEdge;
		dVerY[nXEdgeCnt++]=(double)i;
	}


	nXEdgeCnt=ChooseGoodData_FirstBase(nXEdgeCnt, dVerX, dVerY, nRotate_Range);

	if(nXEdgeCnt<2 ) goto done;

	if(LineFitting1(nXEdgeCnt, dVerX, dVerY, (int)(nXEdgeCnt*0.5), 2, &t1, &a1, &b1)<=0) goto done;

	if(FindCrossPoint(t1, a1, b1, 1, 0, (y1+y2)/2, x, &y)<0) goto done;
	if(*x<x1 || *x>x2)  goto done;

	*tt=t1;
	*aa=a1;
	*bb=b1;
	*x_use_ratio=(double)nXEdgeCnt/(double)nXEdgeTotal;


	nResult=0;
done:
	return nResult;
}

int FindHorLine(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch, int nSlope, int nCorner_Pos,
				int nObject_Color, int nSkip, int nRotate_Range, double *y, double *y_use_ratio, double *tt, double *aa, double *bb)
{
	int i, j;
	int nResult=-1;
	int nYEdgeTotal=0;
	int nYEdgeCnt=0;
	int nReturn, tmp, color;
	double slope;
	double dEdge, t1,a1,b1,t2,a2,b2, x;

	if(nSkip<=0) nSkip=1;


//리턴value 초기화
	*y=*y_use_ratio=*tt=*aa=*bb=0;

	for(i=x1;i<x2;i+=nSkip)
		nYEdgeTotal++;

	if(nYEdgeTotal<2) goto done;


//윗면/아랫면 Edge를 구한다.
	for(j=x1;j<x2;j+=nSkip)
	{
		if(nCorner_Pos==0)
		{
			for(i=y1;i<y2;i++)
				PixelV[i-y1]=(double)*(fm+nPitch*i+j);
		}
		else
		{
			for(i=y2-1;i>=y1;i--)
				PixelV[y2-1-i]=(double)*(fm+nPitch*i+j);
		}

		if( nObject_Color>0 )  //흰색 
		{
			if(nSlope==0)
				nReturn=LN_FindEdge1_B2W((y2-y1), PixelV, &dEdge, &slope);
			else
				nReturn=LN_FindEdge1st_B2W((y2-y1), PixelV, nSlope, &dEdge);
		}
		else
		{
			if(nSlope==0)
				nReturn=LN_FindEdge1_W2B((y2-y1), PixelV, &dEdge, &slope);
			else
				nReturn=LN_FindEdge1st_W2B((y2-y1), PixelV, nSlope, &dEdge);
		}

		if(nReturn==1)
		{
			dHorX[nYEdgeCnt]=(double)j;
			
			if(nCorner_Pos==0)
				dHorY[nYEdgeCnt++]=(double)y1+dEdge;
			else
				dHorY[nYEdgeCnt++]=(double)y2-1-dEdge;
		}
	}

	nYEdgeCnt=ChooseGoodData_FirstBase(nYEdgeCnt, dHorY, dHorX, nRotate_Range);

	if(nYEdgeCnt<2 ) goto done;

	if(LineFitting(nYEdgeCnt, dHorX, dHorY, (int)(nYEdgeCnt*0.5), 2, &t2, &a2, &b2)<=0) goto done;

	if(FindCrossPoint(0, -1, (x1+x2)/2, t2, a2, b2, &x, y)<0) goto done;
	if(*y<y1 || *y>y2)  goto done;

	*tt=t2;
	*aa=a2;
	*bb=b2;
	*y_use_ratio=(double)nYEdgeCnt/(double)nYEdgeTotal;

	nResult=0;
done:
	return nResult;
}

int FindHorLineByGrayValue(LPBYTE fm, int x1, int y1, int x2, int y2, int nPitch, int nThres, int nCorner_Pos,
				int nObject_Color, int nSkip, int nRotate_Range, double *y, double *y_use_ratio, double *tt, double *aa, double *bb)
{
	int i, j;
	int nResult=-1;
	int nYEdgeTotal=0;
	int nYEdgeCnt=0;
	int nReturn, tmp, color;
	double slope;
	double dEdge, t2,a2,b2,x;

	if(nSkip<=0) nSkip=1;


//리턴value 초기화
	*y=*y_use_ratio=*tt=*aa=*bb=0;

	for(i=x1;i<x2;i+=nSkip)
		nYEdgeTotal++;

	if(nYEdgeTotal<2) goto done;


//윗면/아랫면 Edge를 구한다.
	for(j=x1;j<x2;j+=nSkip)
	{
		if(nCorner_Pos==0)
		{
			dEdge=y1;
			if( nObject_Color>0 ) 
			{
				for(i=y1;i<y2;i++)
					if(*(fm+nPitch*i+j)>=nThres)
					{
						dEdge=i;
						break;
					}

			}
			else
			{
				for(i=y1;i<y2;i++)
					if(*(fm+nPitch*i+j)<=nThres)
					{
						dEdge=i;
						break;
					}
			}
		}
		else
		{
			dEdge=y2;
			if( nObject_Color>0 ) 
			{
				for(i=y2-1;i>=y1;i--)
					if(*(fm+nPitch*i+j)>=nThres)
					{
						dEdge=i;
						break;
					}
			}
			else
			{
				for(i=y2-1;i>=y1;i--)
					if(*(fm+nPitch*i+j)<=nThres)
					{
						dEdge=i;
						break;
					}

			}
		}


		
		dHorX[nYEdgeCnt]=(double)j;
		dHorY[nYEdgeCnt++]=(double)dEdge;		
	}

	nYEdgeCnt=ChooseGoodData_FirstBase(nYEdgeCnt, dHorY, dHorX, nRotate_Range);

	if(nYEdgeCnt<2 ) goto done;

	if(LineFitting(nYEdgeCnt, dHorX, dHorY, (int)(nYEdgeCnt*0.5), 2, &t2, &a2, &b2)<=0) goto done;

	if(FindCrossPoint(0, -1, (x1+x2)/2, t2, a2, b2, &x, y)<0) goto done;
	if((int)*y<=y1 || (int)*y>=y2)  goto done;

	*tt=t2;
	*aa=a2;
	*bb=b2;
	*y_use_ratio=(double)nYEdgeCnt/(double)nYEdgeTotal;

	nResult=0;
done:
	return nResult;
}

int l_histo[MAX_LINE_LENGTH/3+10];
int ChooseGoodData_FirstBase(int N, double *x, double *y, int space)
{
	int i;
	int nMax=0, iMax;		//nMax : Max histo Data, iMax: Max histo 발생지점
	int nTemp;				
	int nCount=0;
	int HISTO_SPACE=space/3;
	int MAX_HISTO;
	
	if(HISTO_SPACE<3) HISTO_SPACE=3;
	MAX_HISTO=MAX_LINE_LENGTH/HISTO_SPACE+HISTO_SPACE;

	if(N<=0) return N;

	for(i=0;i<MAX_HISTO;i++)
		l_histo[i]=0;

	for(i=0;i<N;i++)
	{
		nTemp=(int)(x[i]/HISTO_SPACE);
		if(nTemp<MAX_HISTO)
			l_histo[nTemp]++;
	}

	for(i=0;i<MAX_HISTO;i++)
		if(l_histo[i]>nMax)
		{
			nMax=l_histo[i];
			iMax=i;
		}

	for(i=0;i<N;i++)
	{
		if((int)(x[i]/HISTO_SPACE)>=iMax-1 && (int)(x[i]/HISTO_SPACE)<=iMax+1)
		{
			x[nCount]=x[i];
			y[nCount]=y[i];
			nCount++;
		}
	}	
	return nCount;
}


void MedianFilter(int N, double *pEdge, int nMedian)
{
	int i, j;
	int nHalf=nMedian;
	int nNNN=nMedian*2+1;
	double dEdgeSort[31];
	double *pData;

	if(nNNN>31) nNNN=31;
	nHalf=nNNN/2;

	pData=new double[N];

	for(i=0;i<N-nMedian;i++)
	{
		for(j=i;j<i+nNNN;j++)
			dEdgeSort[j-i]=pEdge[j];

		pData[i+nHalf]=EdgeSort(nNNN, dEdgeSort);
	}
	for(i=0;i<nHalf;i++)
		pEdge[i]=pData[nHalf];
	for(i=N-nHalf;i<N;i++)
		pEdge[i]=pData[N-nHalf-1];
	for(i=nHalf;i<N-nHalf;i++)
		pEdge[i]=pData[i];

	delete pData;
}



double EdgeSort(int count, double *Value)
{
	int i,j;
	double MaxValue;
	int Number;
	double StoreValue;

	for(i=0;i<count-1;i++)
	{
		Number=i;
		MaxValue=StoreValue=Value[i];
	      
		for(j=i+1;j<count;j++)
			if(Value[j] > MaxValue) { MaxValue=Value[j]; Number=j; }  
		  
		
		Value[i]=Value[Number]; //제일 큰 것
		
		Value[Number]=StoreValue;
	}

	return Value[count/2-1];
}
long EdgeSort(int count, long *Value)
{
	int i,j;
	long MaxValue;
	int Number;
	long StoreValue;

	for(i=0;i<count-1;i++)
	{
		Number=i;
		MaxValue=StoreValue=Value[i];
	      
		for(j=i+1;j<count;j++)
			if(Value[j] > MaxValue) { MaxValue=Value[j]; Number=j; }  
		  
		
		Value[i]=Value[Number]; //제일 큰 것
		
		Value[Number]=StoreValue;
	}

	return Value[count/2-1];
}


int FindPointLineCross(double dCx, double dCy, double t, double a, double b, double *dRx, double *dRy)
{
	if(t==0)
	{
		*dRx=-b;
		*dRy=dCy;
	}
	else if(a==0)
	{
		*dRx=dCx;
		*dRy=b;
	}
	else
	{
		FindCrossPoint(t, a, b, 1, -1/a, dCy+dCx/a, dRx, dRy);
	}
	return 0;
}
