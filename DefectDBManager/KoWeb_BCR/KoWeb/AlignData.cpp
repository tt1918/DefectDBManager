#include "stdafx.h"
#include "math.h"

ALIGNDATA  g_AlignData[MAX_ALIGN_MODULE];



TCHAR g_sType[EDGE_TYPE_NUM][50]={	_T("No Align"),						//0
								   _T("[Edge] X1"),						//1
								   _T("[Edge] Y1"),						//2
  								   _T("[Edge] X1, Y1"),					//3
								   _T("[Edge] X1, Y1, Y2"),				//4
								   _T("[Edge] Y1, X1, X2"),				//5
								   _T("[Edge] X1,Y1, X2, Y2"),			//6
								   _T("[Match] M1(X,Y)"),				//7
								   _T("[Match] M1(X,Y), M2(X,Y)"),		//8
								   _T("[Match] M1,M2,M3,M4"),			//9
								   _T("[Match] M1(X,Y),[Edge] X1, Y1"),	//10
								   _T("[Match] M1(X,Y),[Edge] X1, X2"),	//11
								   _T("[Match] M1(X,Y),[Edge] Y1, Y2")	//12
};
int g_ROINum[EDGE_TYPE_NUM]={0,1,1,2,3,3,4,1,2,4,3,3,3};
TCHAR g_ROIName[EDGE_TYPE_NUM][4][3]={{_T(""),},								//0
									  {_T("X1"),},								//1
									  {_T("Y1"),},								//2
									  {_T("X1"),_T("Y1"),},						//3
									  {_T("X1"),_T("Y1"),_T("Y2"),},			//4
									  {_T("Y1"),_T("X1"),_T("X2"),},			//5
									  {_T("X1"),_T("Y1"),_T("X2"),_T("Y2")},	//6
									  {_T("M1"),},								//7
									  {_T("M1"),_T("M2"),},						//8
									  {_T("M1"),_T("M2"),_T("M3"),_T("M4")},	//9
									  {_T("M1"),_T("X1"),_T("Y1"),},			//10
									  {_T("M1"),_T("X1"),_T("X2"),},			//11
									  {_T("M1"),_T("Y1"),_T("Y2"),}				//12
};

//InspectFindLine.cpp에서 사용하는 함수들----------------------------------------------------------------------------------------------------------------------------------------------
int FindHorEdgeGrayTh(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nThres, double *pEdgeX, double *pEdgeY);
int FindVerEdgeGrayTh(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nThres, double *pEdgeX, double *pEdgeY);

int FindHorEdgeSlope1st(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nSlope, int nOffset, double *pEdgeX, double *pEdgeY);
int FindVerEdgeSlope1st(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nSlope, int nOffset, double *pEdgeX, double *pEdgeY);

int FindHorEdgeSlopeMax(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nSlope, int nOffset, double *pEdgeX, double *pEdgeY);
int FindVerEdgeSlopeMax(LPBYTE fm, int left, int top, int right, int bottom, int nPitch, int nObject, int nDir, int nAvgLine, int nSlope, int nOffset, double *pEdgeX, double *pEdgeY);
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// fm			이미지 번지
// left		 
// top		
// right	
// bottom	
// nDir			0:(왼쪽->오른쪽 or 위->아래)  1: 반대
// nLineType	0: 수평선찾기, 1: 수직선 찾기
// nMethod		0: Gray값으로 1:1stEdge 2: Max Edge
// nObject		0: 흑  1: 백  2: 임의 
// nGrayTh		Gray값 을 넘거나 떨어질때 Edge값을 구함.
// nSlopeTh		기울기가 이 값을 넘으면 Edge
// nAvgLine		몇 Line씩 평균해서 Edge 데이타를 구할 건지 설정
int FindLine(LPBYTE fm, int left, int top, int right, int bottom, int pitch, int nDir, int nLineType, int nMethod, int nObject, int nGrayTh, int nSlopeTh, int nAvgLine,  BOOL bShowGraphic, CCossImage *m_pImage,
			  double *lt, double *la, double *lb, double *pX, double *pY)
{
	int i, j;
	double dAngle;
	double *pEdgeX, *pEdgeY, t, a, b, dCx, dCy;							//구한 라인의 방정식, 및 중심
	int nIsHor=0, nCount=0, FoundLine=0;

	if(nAvgLine<1) nAvgLine=1;
	else if(nAvgLine>20) nAvgLine=20;

	if(nLineType==0) nIsHor=1; //수평

#ifndef USE_SK_BAT
	if(nIsHor==1) //수평라인
	{
		nCount=(right-left)/nAvgLine;
		pEdgeX=new double[nCount+1];
		pEdgeY=new double[nCount+1];

		if(nMethod==0) //밝기로 Edge구함
		{
			nCount=FindHorEdgeGrayTh(fm, left, top, right, bottom, pitch, nObject, nDir, nAvgLine, nGrayTh, pEdgeX, pEdgeY);
		}
		else if(nMethod==1) //1st기울기
		{
			nCount=FindHorEdgeSlope1st(fm, left, top, right, bottom, pitch, nObject, nDir, nAvgLine, nSlopeTh, 1, pEdgeX, pEdgeY);
		}
		else if(nMethod==2) //Max 기울기
		{
			nCount=FindHorEdgeSlopeMax(fm, left, top, right, bottom, pitch, nObject, nDir, nAvgLine, nSlopeTh, 1, pEdgeX, pEdgeY);
		}
	}
	else //수직라인
	{
		nCount=(bottom-top)/nAvgLine;
		pEdgeX=new double[nCount+1];
		pEdgeY=new double[nCount+1];

		if(nMethod==0) //밝기로 Edge구함
		{
			nCount=FindVerEdgeGrayTh(fm, left, top, right, bottom, pitch, nObject, nDir, nAvgLine, nGrayTh, pEdgeX, pEdgeY);
		}
		else if(nMethod==1) //1st기울기
		{
			nCount=FindVerEdgeSlope1st(fm, left, top, right, bottom, pitch, nObject, nDir, nAvgLine, nSlopeTh, 1, pEdgeX, pEdgeY);
		}
		else if(nMethod==2) //Max 기울기
		{
			nCount=FindVerEdgeSlopeMax(fm, left, top, right, bottom, pitch, nObject, nDir, nAvgLine, nSlopeTh, 1, pEdgeX, pEdgeY);
		}
	}
#endif

	if(nCount>=2) //수평선 또는 수직선이면 수평, 수직이 아닌 데이타 버림. (이 부분 개별 설정하면 더 좋아짐. 현재는1/5영역 외는 버림.
	{
		if(nLineType==0)//수평선
		{
			nCount=ChooseGoodData_FirstBase(nCount, pEdgeY, pEdgeX, (bottom-top)/5);
		}
		else if(nLineType==1)//수직선
		{
			nCount=ChooseGoodData_FirstBase(nCount, pEdgeX, pEdgeY,  (right-left)/5);
		}
	}

	if(nCount>=2)
	{
		if(nIsHor) //데이타 최대 1/3 버림.
		{
			if(LineFitting(nCount, pEdgeX, pEdgeY, nCount/3, 2, &t, &a, &b)>0)
			{
				FoundLine=1;
			}
		}
		else
		{
			if(LineFitting1(nCount, pEdgeX, pEdgeY, nCount/3, 2, &t, &a, &b)>0)
			{
				FoundLine=1;
			}
		}
	}


	if(FoundLine==0) //라인 못 찾음
	{
		
	}
	else	//라인찾음
	{		
		*lt=t;
		*la=a;
		*lb=b;

		if(nIsHor) FindCrossPoint(t,a,b, 0, 1, -1*(right+left)/2.0, &dCx, &dCy);
		else       FindCrossPoint(t,a,b, 1, 0,  (bottom+top)/2.0, &dCx, &dCy);

		*pX=dCx;
		*pY=dCy;
	}
			
	//필요하면 화면에 표시--------------------------------------------------------------------
	if(bShowGraphic && m_pImage)
	{
		for(j=0;j<nCount;j++)
		{
			m_pImage->SetPixel(NULL, (int)(pEdgeX[j]+0.5), (int)(pEdgeY[j]+0.5), RGB(0,255,0));
		}
		if(FoundLine)
			m_pImage->DrawCross(NULL, (int)(dCx+0.5), (int)(dCy+0.5), 10, RGB(0,255,0));
	}
	//-----------------------------------------------------------------------------------------

	delete 	pEdgeX;
	delete 	pEdgeY;

	return FoundLine;
}


int SaveAlignData(CString sFileName)
{
	CFile f;
	int ret;
	
	ret=f.Open(sFileName, CFile::modeCreate|CFile::modeWrite);
	if(ret)
	{
		f.Write(g_AlignData, MAX_ALIGN_MODULE*sizeof(ALIGNDATA));
		f.Close();
	}
	return ret;
}

int LoadAlignData(CString sFileName)
{
	CFile f;
	int ret;
	
	ret=f.Open(sFileName, CFile::modeRead);
	if(ret)
	{
		f.Read(g_AlignData, MAX_ALIGN_MODULE*sizeof(ALIGNDATA));
		f.Close();
	}

	return ret;
}

int CalAlignData(ALIGNDATA *pAlign)
{
	int i;
	double dCx, dCy, dCx1, dCy1;
	double dAngleBase, dAngle, dAngleSum;
	double bt,ba,bb, ct,ca,cb;

	pAlign->m_dOffsetX=pAlign->m_dOffsetY=pAlign->m_dAngleDeg=0;
	pAlign->m_dCx=pAlign->m_dCy=0;
	pAlign->m_nFoundAlign=1;


	switch(pAlign->m_nAlignType)
	{
	case 1: //X
			pAlign->m_dOffsetX=pAlign->m_dX[0]-pAlign->m_dBaseX[0];	
			break;
	case 2: //Y
			pAlign->m_dOffsetY=pAlign->m_dY[0]-pAlign->m_dBaseY[0]; 
			break;
	case 3: //X, Y
			pAlign->m_dOffsetX=pAlign->m_dX[0]-pAlign->m_dBaseX[0]; 
			pAlign->m_dOffsetY=pAlign->m_dY[1]-pAlign->m_dBaseY[1]; 
			break;
	case 4:	//X, Y1, Y2 
	case 5: //Y, X1, X2
			FindLineFunction(pAlign->m_dBaseX[1], pAlign->m_dBaseY[1], pAlign->m_dBaseX[2], pAlign->m_dBaseY[2], &bt, &ba, &bb);
			FindCrossPoint(pAlign->bt[0], pAlign->ba[0], pAlign->bb[0],bt,ba,bb,&dCx, &dCy);
			dAngleBase=atan2((pAlign->m_dBaseY[1]-pAlign->m_dBaseY[2]), (pAlign->m_dBaseX[1]-pAlign->m_dBaseX[2]));

			FindLineFunction(pAlign->m_dX[1], pAlign->m_dY[1], pAlign->m_dX[2], pAlign->m_dY[2], &ct, &ca, &cb);
			FindCrossPoint(pAlign->t[0], pAlign->a[0], pAlign->b[0],ct,ca,cb,&dCx1, &dCy1);
			dAngle=atan2((pAlign->m_dY[1]-pAlign->m_dY[2]), (pAlign->m_dX[1]-pAlign->m_dX[2]));

			pAlign->m_dCx=dCx;
			pAlign->m_dCy=dCy;
			pAlign->m_dOffsetX=dCx1-dCx;
			pAlign->m_dOffsetY=dCy1-dCy;
			pAlign->m_dAngleDeg=(dAngle-dAngleBase)*180.0/3.14159265358;
			break;
	case 6: //X1, Y1, X2, Y2
			pAlign->m_dCx=(pAlign->m_dBaseX[0]+pAlign->m_dBaseX[2])*0.5;	
			pAlign->m_dCy=(pAlign->m_dBaseY[1]+pAlign->m_dBaseY[3])*0.5;

			dCx=(pAlign->m_dX[0]+pAlign->m_dX[2])*0.5;
			dCy=(pAlign->m_dY[1]+pAlign->m_dY[3])*0.5;

			pAlign->m_dOffsetX=dCx-pAlign->m_dCx;
			pAlign->m_dOffsetY=dCy-pAlign->m_dCy;

			dAngleBase=atan2((pAlign->m_dBaseY[1]-pAlign->m_dBaseY[3]), (pAlign->m_dBaseX[0]-pAlign->m_dBaseX[2])); 
			dAngle    =atan2((pAlign->m_dY[1]    -pAlign->m_dY[3]    ), (pAlign->m_dX[0]    -pAlign->m_dX[2]    ));
			pAlign->m_dAngleDeg=(dAngle-dAngleBase)*180.0/3.14159265358;  
			break;
	case 7: //M1 (X, Y)
			pAlign->m_dOffsetX=pAlign->m_dX[0]-pAlign->m_dBaseX[0]; //X
			pAlign->m_dOffsetY=pAlign->m_dY[0]-pAlign->m_dBaseY[0]; //Y  
			break;
	case 8: //M1,M2
			pAlign->m_dCx=(pAlign->m_dBaseX[0]+pAlign->m_dBaseX[1])*0.5;
			pAlign->m_dCy=(pAlign->m_dBaseY[0]+pAlign->m_dBaseY[1])*0.5;

			dCx=(pAlign->m_dX[0]+pAlign->m_dX[1])*0.5;
			dCy=(pAlign->m_dY[0]+pAlign->m_dY[1])*0.5;

			pAlign->m_dOffsetX=dCx-pAlign->m_dCx;
			pAlign->m_dOffsetY=dCy-pAlign->m_dCy;

			dAngleBase=atan2((pAlign->m_dBaseY[0]-pAlign->m_dBaseY[1]), (pAlign->m_dBaseX[0]-pAlign->m_dBaseX[1])); 
			dAngle    =atan2((pAlign->m_dY[0]    -pAlign->m_dY[1]    ), (pAlign->m_dX[0]    -pAlign->m_dX[1]    ));
			pAlign->m_dAngleDeg=(dAngle-dAngleBase)*180.0/3.14159265358;  
			break;
	case 9: //M1, M2, M3, M4
			pAlign->m_dCx=(pAlign->m_dBaseX[0]+pAlign->m_dBaseX[1]+pAlign->m_dBaseX[2]+pAlign->m_dBaseX[3])*0.25;
			pAlign->m_dCy=(pAlign->m_dBaseY[0]+pAlign->m_dBaseY[1]+pAlign->m_dBaseY[2]+pAlign->m_dBaseY[3])*0.25;

			dCx=(pAlign->m_dX[0]+pAlign->m_dX[1]+pAlign->m_dX[2]+pAlign->m_dX[3])*0.25;
			dCy=(pAlign->m_dY[0]+pAlign->m_dY[1]+pAlign->m_dY[2]+pAlign->m_dY[3])*0.25;

			pAlign->m_dOffsetX=dCx-pAlign->m_dCx;
			pAlign->m_dOffsetY=dCy-pAlign->m_dCy;

			dAngleSum=0;
			dAngleBase=atan2((pAlign->m_dBaseY[0]-pAlign->m_dBaseY[2]), (pAlign->m_dBaseX[0]-pAlign->m_dBaseX[2])); 
			dAngle    =atan2((pAlign->m_dY[0]    -pAlign->m_dY[2]    ), (pAlign->m_dX[0]    -pAlign->m_dX[2]    ));
			dAngleSum=(dAngle-dAngleBase);

			dAngleBase=atan2((pAlign->m_dBaseY[1]-pAlign->m_dBaseY[3]), (pAlign->m_dBaseX[1]-pAlign->m_dBaseX[3])); 
			dAngle    =atan2((pAlign->m_dY[1]    -pAlign->m_dY[3]    ), (pAlign->m_dX[1]    -pAlign->m_dX[3]    ));
			dAngleSum+=(dAngle-dAngleBase);

			pAlign->m_dAngleDeg=dAngleSum*0.5*180.0/3.14159265358;  
			break;
	case 10://M1, X1, Y1
			pAlign->m_dCx=(pAlign->m_dBaseX[0]+pAlign->m_dBaseX[1])*0.5;
			pAlign->m_dCy=(pAlign->m_dBaseY[0]+pAlign->m_dBaseY[2])*0.5;

			dCx=(pAlign->m_dX[0]+pAlign->m_dX[1])*0.5;
			dCy=(pAlign->m_dY[0]+pAlign->m_dY[2])*0.5;

			pAlign->m_dOffsetX=dCx-pAlign->m_dCx;
			pAlign->m_dOffsetY=dCy-pAlign->m_dCy;

			dAngleBase=atan2((pAlign->m_dBaseY[0]-pAlign->m_dBaseY[2]), (pAlign->m_dBaseX[0]-pAlign->m_dBaseX[1])); 
			dAngle    =atan2((pAlign->m_dY[0]    -pAlign->m_dY[2]    ), (pAlign->m_dX[0]    -pAlign->m_dX[1]    ));
			pAlign->m_dAngleDeg=(dAngle-dAngleBase)*180.0/3.14159265358;  
			break;
	case 11: //M1, X1, X2 
	case 12: //M1, Y1, Y2
			dAngleBase=atan2((pAlign->m_dBaseY[1]-pAlign->m_dBaseY[2]), (pAlign->m_dBaseX[1]-pAlign->m_dBaseX[2]));
			dAngle    =atan2((pAlign->m_dY[1]    -pAlign->m_dY[2]    ), (pAlign->m_dX[1]    -pAlign->m_dX[2]    ));

			pAlign->m_dCx=pAlign->m_dBaseX[0];
			pAlign->m_dCy=pAlign->m_dBaseY[0];
			pAlign->m_dOffsetX=pAlign->m_dX[0]-pAlign->m_dBaseX[0];
			pAlign->m_dOffsetY=pAlign->m_dY[0]-pAlign->m_dBaseY[0];
			pAlign->m_dAngleDeg=(dAngle-dAngleBase)*180.0/3.14159265358;
			break;
	}


	return 0;
}
	
int FindNewPoint(ALIGNDATA *pAlign,double Px, double Py,  double *Px_, double *Py_)
{
	if(pAlign->m_nNoApply)
	{
		*Px_=Px;
		*Py_=Py;
		return 0;
	}

	if(pAlign->m_dAngleDeg==0 || pAlign->m_nNoApplyAngle)
	{
		*Px_=Px+pAlign->m_dOffsetX;
		*Py_=Py+pAlign->m_dOffsetY;
	}
	else
	{
		PointRotation(pAlign->m_dCx, pAlign->m_dCy, pAlign->m_dAngleDeg, pAlign->m_dOffsetX, pAlign->m_dOffsetY, Px, Py,  Px_, Py_);
	}
	return 0;
}