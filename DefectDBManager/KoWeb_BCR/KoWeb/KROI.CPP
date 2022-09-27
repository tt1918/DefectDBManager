///////////////////////////////////////////////////////////////////////////////
//
//  KO YOUNG WOO 만듦.  1998년 어느날...
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "kroi.h"
#include "math.h"

//#define ROI_ROTATE

CROIControl g_ROI[MAX_IMAGE];

void CROIControl::Initialize(int nWidth, int nHeight)
{
	m_WIDTH=nWidth;
	m_HEIGHT=nHeight;
	LoadDefaultROI();

	m_fmD=NULL;

#ifdef ROI_ROTATE
	m_fmD=new BYTE[m_WIDTH*m_HEIGHT];
#endif
}
void CROIControl::Close()
{
#ifdef ROI_ROTATE
	if(m_fmD)
	{
		delete m_fmD;
		m_fmD=NULL;
	}
#endif
}
void CROIControl::LoadDefaultROI()
{
	int i,j;

	m_TrackerNumber=0;
	for(j=0;j<MAXTRACKERNUMBER;j++)
	{
		m_Tracker[j].rect.left	=m_WIDTH/2	-50;
		m_Tracker[j].rect.top	=m_HEIGHT/2	-50;
		m_Tracker[j].rect.right	=m_WIDTH/2	+50;
		m_Tracker[j].rect.bottom=m_HEIGHT/2	+50;

		wsprintf(m_TrackerName[j],_T("%d"), j);
	}
	g_CurWindowNum=-1;

	m_dOffsetX=m_dOffsetY=m_dOffsetR=0;
	

	m_FlagShowROI	=true;
	m_FlagEnableROI	=false;
	m_FlagEnableOneROI=false;
	m_FlagShowText	=true;

	m_IsROIClicked	=false;
	m_TrackerON		=false;
	m_bTrackerHandleIN=false;

	m_ROIColor=RGB(0,100,255);
	m_TextColor=RGB(128,0,128);
	if(m_dScaleX<0.01) m_dScaleX=1.0;
	if(m_dScaleY<0.01) m_dScaleY=1.0;
}

int CROIControl::SaveROI(CString filename)
{
	int ret=-1;

	CFile f;

	if(!f.Open(filename, CFile::modeCreate|CFile::modeWrite))
	{
		goto done;
	}
	else
	{
		f.Write( &m_FlagShowROI, sizeof(BOOL));
		f.Write( &m_FlagEnableROI, sizeof(BOOL));
		f.Write( &m_FlagEnableROIOld, sizeof(BOOL));
		f.Write( &m_FlagEnableOneROI, sizeof(BOOL));
		f.Write( &m_FlagShowText, sizeof(BOOL));
		f.Write( &m_FlagShowCenterLine, sizeof(BOOL));
		f.Write( &m_TrackerNumber, sizeof(int));
		f.Write( &m_ROIColor, sizeof(COLORREF));
		f.Write( &m_TextColor, sizeof(COLORREF));
		f.Write( m_Tracker, sizeof(KROI)*MAXTRACKERNUMBER);
		f.Write( m_TrackerName, sizeof(TCHAR)*MAXTRACKERNUMBER*MAXNAMELENGTH);
		f.Write( m_OneTrackerName, sizeof(TCHAR)*MAXNAMELENGTH);

			
		f.Close();
	}


	ret=0;
done:
	return ret;
}

void CROIControl::SetScale(double dScaleX, double dScaleY)//{m_dScale=dScale;};
{
	m_dScaleX=dScaleX;
	m_dScaleY=dScaleY;
}
int CROIControl::LoadROI(CString filename)
{
	int i, ret=-1;

	CFile f;

	if(!f.Open(filename, CFile::modeRead))
	{
		LoadDefaultROI();
		goto done;
	}
	else
	{
		f.Read( &m_FlagShowROI, sizeof(BOOL));
		f.Read( &m_FlagEnableROI, sizeof(BOOL));
		f.Read( &m_FlagEnableROIOld, sizeof(BOOL));
		f.Read( &m_FlagEnableOneROI, sizeof(BOOL));
		f.Read( &m_FlagShowText, sizeof(BOOL));
		f.Read( &m_FlagShowCenterLine, sizeof(BOOL));
		f.Read( &m_TrackerNumber, sizeof(int));
		f.Read( &m_ROIColor, sizeof(COLORREF));
		f.Read( &m_TextColor, sizeof(COLORREF));
		f.Read( m_Tracker, sizeof(KROI)*MAXTRACKERNUMBER);
		f.Read( m_TrackerName, sizeof(TCHAR)*MAXTRACKERNUMBER*MAXNAMELENGTH);
		f.Read( m_OneTrackerName, sizeof(TCHAR)*MAXNAMELENGTH);

		g_CurWindowNum=-1;

		f.Close();
	}
	
	ret=0;
done:
	return ret;
}

//false면 안보임.
void CROIControl::ShowROI(bool flag)
{
	m_FlagShowROI=flag;
}
bool CROIControl::GetShowROI()
{
	return m_FlagShowROI;
}

void CROIControl::ShowCenterLine(bool flag)
{
	m_FlagShowCenterLine=flag;
}

void CROIControl::ShowTextROI(bool flag)
{
	m_FlagShowText=flag;
}

//false면 보이지만 변경시킬수는 없다. 
void CROIControl::EnableROI(bool flag)
{
	int i;
	m_FlagEnableROIOld=m_FlagEnableROI=flag;
	if(!flag)
	{
		g_CurWindowNum=-1;
		m_FlagEnableOneROI=false;
	}
}
bool CROIControl::GetEnableROI()
{
	return m_FlagEnableROI;
}

void CROIControl::EnableROIOne(bool flag, TCHAR * RoiName)
{
	m_FlagEnableOneROI=flag;
	if(m_FlagEnableOneROI)
	{
		m_FlagEnableROI=flag;
		_tcscpy(m_OneTrackerName, RoiName);
	}
	else
	{
		m_FlagEnableROI=m_FlagEnableROIOld;		//m_FlagEnableOneROI=false가 될때는 EnableROI시 세팅된 값으로 돌아간다.
	}
	g_CurWindowNum=-1;
}
void CROIControl::SetROIMaxSize(TCHAR *RoiName, bool flag)
{
	int nNumber=-1, j;

	nNumber=GetROINumber(RoiName);

	if(nNumber>=0) m_Tracker[nNumber].full=flag;
}

int CROIControl::GetROINumber(TCHAR * RoiName)
{
	int nNumber=-1, j;

	for(j=0;j<m_TrackerNumber;j++)
	{
		if(_tcscmp(m_TrackerName[j],RoiName)==0)
		{
			nNumber=j;
			break;
		}
	}

	return nNumber;
}

//return	 0 :  OK
//			-1 :  ROI 영역불량 
//			-2 : 최대 ROI 수 초과 
//			-3 : 같은 이름이 있음
//			-4 : ROI 이름 길이초과 
int CROIControl::AddROI(TCHAR * RoiName, int left, int top, int right, int bottom)
{
	int i, j;
	int nNewROI;

	if(left>right || top>bottom) return -1;
	if(m_TrackerNumber+1>=MAXTRACKERNUMBER) return -2;
	if(_tcslen(RoiName)>=MAXNAMELENGTH) return -4;


	for(j=0;j<m_TrackerNumber;j++)
	{
		if(_tcscmp(m_TrackerName[j], RoiName)==0)
		{
			return -3;
		}
	}

	nNewROI=m_TrackerNumber++;
	_tcscpy(m_TrackerName[nNewROI], RoiName);
	
	m_Tracker[nNewROI].nAngle=0;
	m_Tracker[nNewROI].bAutoThreshold=true;
	m_Tracker[nNewROI].nThreshold=128;
	m_Tracker[nNewROI].bShow=true;

	if(left<0) left=0;
	else if(left>=m_WIDTH) left=m_WIDTH-1;
    m_Tracker[nNewROI].rect.left=left;
	
	if(right<0) right=0;
	else if(right>=m_WIDTH) right=m_WIDTH-1;
    m_Tracker[nNewROI].rect.right=right;
	
	if(top<0) top=0;
	else if(top>=m_HEIGHT) top=m_HEIGHT-1;
    m_Tracker[nNewROI].rect.top=top;

	if(bottom<0) bottom=0;
	else if(bottom>=m_HEIGHT) bottom=m_HEIGHT-1;
    m_Tracker[nNewROI].rect.bottom=bottom;

	m_Tracker[nNewROI].full=false;

	m_Tracker[nNewROI].pt[0].x=m_Tracker[nNewROI].rect.left;
	m_Tracker[nNewROI].pt[0].y=m_Tracker[nNewROI].rect.top;
	m_Tracker[nNewROI].pt[1].x=m_Tracker[nNewROI].rect.left;
	m_Tracker[nNewROI].pt[1].y=m_Tracker[nNewROI].rect.bottom;
	m_Tracker[nNewROI].pt[2].x=m_Tracker[nNewROI].rect.right;
	m_Tracker[nNewROI].pt[2].y=m_Tracker[nNewROI].rect.bottom;
	m_Tracker[nNewROI].pt[3].x=m_Tracker[nNewROI].rect.right;
	m_Tracker[nNewROI].pt[3].y=m_Tracker[nNewROI].rect.top;

	for(i=0;i<m_TrackerNumber;i++)
	{
		m_Tracker[i].isResize=false;
	}
	return 0;
}
int CROIControl::SetROIRect(TCHAR * RoiName, int left, int top, int right, int bottom)
{
	int nNumber=-1, j;

	nNumber=GetROINumber(RoiName);
	
	if(nNumber>=0)
	{
		m_Tracker[nNumber].rect.left=left;
		m_Tracker[nNumber].rect.top=top;
		m_Tracker[nNumber].rect.right=right;
		m_Tracker[nNumber].rect.bottom=bottom;
		m_Tracker[nNumber].nAngle=0;

		m_Tracker[nNumber].pt[0].x=m_Tracker[nNumber].rect.left;
		m_Tracker[nNumber].pt[0].y=m_Tracker[nNumber].rect.top;
		m_Tracker[nNumber].pt[1].x=m_Tracker[nNumber].rect.left;
		m_Tracker[nNumber].pt[1].y=m_Tracker[nNumber].rect.bottom;
		m_Tracker[nNumber].pt[2].x=m_Tracker[nNumber].rect.right;
		m_Tracker[nNumber].pt[2].y=m_Tracker[nNumber].rect.bottom;
		m_Tracker[nNumber].pt[3].x=m_Tracker[nNumber].rect.right;
		m_Tracker[nNumber].pt[3].y=m_Tracker[nNumber].rect.top;
	}

	return nNumber;
}
//해당이름의 ROI를 삭제한다.
//return	 0 :  OK
//			 -1: 해당이름의 ROI없음 
int CROIControl::DeleteROI(TCHAR * RoiName)
{
	int nNumber=-1, j;

	nNumber=GetROINumber(RoiName);

	if(nNumber>=0)
	{
		m_Tracker[nNumber].nNGCount=0;
		for(j=nNumber;j<m_TrackerNumber;j++)
		{
			m_Tracker[j]=m_Tracker[j+1];
			_tcscpy(m_TrackerName[j],m_TrackerName[j+1]);
		}
		m_TrackerNumber--;
		return 0;
	}
	else return -1;
}
void  CROIControl::DeleteAllROI() 
{
	m_TrackerNumber=0;
	g_CurWindowNum=-1;
}

//해당이름의 ROI정보를 읽어오는 함수 (맨 처음이 @또는 /인경우는 Offset을 주지 않는다.)
//return	 >=0 :  OK
//			 -1: 해당이름의 ROI없음 
//           -2: ROI가 영역을 벗어남.
int CROIControl::GetROIData(TCHAR * RoiName, KROI *pROI)
{
	int nNumber=-1, i, j;
	double dOffsetX, dOffsetY;
	double cosT, sinT, x, y, CRx, CRy;

	nNumber=GetROINumber(RoiName);

	if(nNumber>=0)
	{
		*pROI=m_Tracker[nNumber];

		if(!m_Tracker[nNumber].full)
		{
			if(m_TrackerName[nNumber][0]=='@' || m_TrackerName[nNumber][0]=='/')
			{
				dOffsetX=dOffsetY=0;
			}
			else
			{
				dOffsetX=m_dOffsetX;
				dOffsetY=m_dOffsetY;
			}

			pROI->rect.left+=(int)dOffsetX;
			pROI->rect.right+=(int)dOffsetX;
			pROI->rect.top+=(int)dOffsetY;
			pROI->rect.bottom+=(int)dOffsetY;

			pROI->nAngle+=m_dOffsetR;
//			for(i=0;i<4;i++)
//			{
//				pROI->pt[i].x+=dOffsetX;
//				pROI->pt[i].y+=dOffsetY;
//			}
			pROI->pt[0].x=pROI->rect.left;
			pROI->pt[0].y=pROI->rect.top;
			pROI->pt[1].x=pROI->rect.left;
			pROI->pt[1].y=pROI->rect.bottom;
			pROI->pt[2].x=pROI->rect.right;
			pROI->pt[2].y=pROI->rect.bottom;
			pROI->pt[3].x=pROI->rect.right;
			pROI->pt[3].y=pROI->rect.top;

			cosT=cos(pROI->nAngle*ROI_PI/180.0);
			sinT=sin(pROI->nAngle*ROI_PI/180.0);
			CRx=(pROI->rect.right+pROI->rect.left)*0.5;
			CRy=(pROI->rect.bottom+pROI->rect.top)*0.5;
			for(i=0;i<4;i++)
			{
				x=cosT*(pROI->pt[i].x-CRx)-sinT*(pROI->pt[i].y-CRy)+CRx;
				y=sinT*(pROI->pt[i].x-CRx)+cosT*(pROI->pt[i].y-CRy)+CRy;	

				pROI->pt[i].x=x;
				pROI->pt[i].y=y;
			}

			if(pROI->nAngle)
			{
				for(i=0;i<4;i++)
					if(pROI->pt[i].x<0 || pROI->pt[i].x>=m_WIDTH || pROI->pt[i].y<0 || pROI->pt[i].y>=m_HEIGHT)
						nNumber=-2;
			}
			else
			{
				if(pROI->rect.left<0 || pROI->rect.right>=m_WIDTH || pROI->rect.top<0 || pROI->rect.bottom>=m_HEIGHT)
					nNumber=-2;
			}		

			return nNumber;
		}
		else
		{
			pROI->rect.left=0;
			pROI->rect.right=m_WIDTH-1;
			pROI->rect.top=0;
			pROI->rect.bottom=m_HEIGHT-1;
			pROI->nAngle=0;
			return nNumber;
		}
	}
	else return -1;
}
//해당이름의 ROI정보를 읽어오는 함수 
//return	 >=0 :  OK
//			 -1: 해당이름의 ROI없음 
//           -2: ROI가 영역을 벗어남.
int CROIControl::GetROIData_(TCHAR * RoiName, KROI *pROI)
{
	int nNumber=-1, i, j;
	double dOffsetX, dOffsetY;

	nNumber=GetROINumber(RoiName);

	if(nNumber>=0)
	{
		*pROI=m_Tracker[nNumber];

		if(!m_Tracker[nNumber].full)
		{
			dOffsetX=m_dOffsetX;
			dOffsetY=m_dOffsetY;

			pROI->rect.left+=(int)dOffsetX;
			pROI->rect.right+=(int)dOffsetX;
			pROI->rect.top+=(int)dOffsetY;
			pROI->rect.bottom+=(int)dOffsetY;

			for(i=0;i<4;i++)
			{
				pROI->pt[i].x+=dOffsetX;
				pROI->pt[i].y+=dOffsetY;
			}

			if(pROI->nAngle)
			{
				for(i=0;i<4;i++)
					if(pROI->pt[i].x<0 || pROI->pt[i].x>=m_WIDTH || pROI->pt[i].y<0 || pROI->pt[i].y>=m_HEIGHT)
						nNumber=-2;
			}
			else
			{
				if(pROI->rect.left<0 || pROI->rect.right>=m_WIDTH || pROI->rect.top<0 || pROI->rect.bottom>=m_HEIGHT)
					nNumber=-2;
			}		

			return nNumber;
		}
		else
		{
			pROI->rect.left=0;
			pROI->rect.right=m_WIDTH-1;
			pROI->rect.top=0;
			pROI->rect.bottom=m_HEIGHT-1;
			pROI->nAngle=0;
			return nNumber;
		}
	}
	else return -1;
}
//해당이름의 ROI 영역을 읽어오는 함수 
//return	 0 :  OK
//			 -1: 해당이름의 ROI없음 
int CROIControl::GetROIRegion(TCHAR * RoiName, int *left, int *top, int *right, int *bottom)
{
	int nNumber=-1, j;
	double dOffsetX, dOffsetY;

	nNumber=GetROINumber(RoiName);
	*left=*right=*top=*bottom=0;
	if(nNumber>=0)
	{
		if(m_Tracker[nNumber].full)
		{
			*left=*top=0;
			*right=m_WIDTH-1;
			*bottom=m_HEIGHT-1;
		}
		else
		{
			if(m_TrackerName[nNumber][0]=='@' || m_TrackerName[nNumber][0]=='/')
			{
				dOffsetX=dOffsetY=0;
			}
			else
			{
				dOffsetX=m_dOffsetX;
				dOffsetY=m_dOffsetY;
			}

			*left=m_Tracker[nNumber].rect.left+(int)dOffsetX;
			*top=m_Tracker[nNumber].rect.top+(int)dOffsetY;
			*right=m_Tracker[nNumber].rect.right+(int)dOffsetX;
			*bottom=m_Tracker[nNumber].rect.bottom+(int)dOffsetY;
		}
		return 0;
	}
	else return -1;
}
int  CROIControl::GetROIRegionPt(TCHAR * RoiName, TPoint *pt)
{
	int nNumber=-1, j;
	double dOffsetX, dOffsetY;

	nNumber=GetROINumber(RoiName);	
	if(nNumber>=0)
	{
		if(m_TrackerName[nNumber][0]=='@' || m_TrackerName[nNumber][0]=='/')
		{
			dOffsetX=dOffsetY=0;
		}
		else
		{
			dOffsetX=m_dOffsetX;
			dOffsetY=m_dOffsetY;
		}

		for(j=0;j<4;j++)
		{
			pt[j].x=m_Tracker[nNumber].pt[j].x+(int)dOffsetX;
			pt[j].y=m_Tracker[nNumber].pt[j].y+(int)dOffsetY;
		}
		return 0;
	}
	else return -1;
}

int CROIControl::SetShowROI_ONE(TCHAR * RoiName, bool flag)
{
	int j;

	if(flag)
	{
		for(j=0;j<m_TrackerNumber;j++)
		{
			if(m_TrackerName[j][0]=='/') continue;
			if(_tcscmp(m_TrackerName[j],RoiName)==0)
			{
				m_Tracker[j].bShow=flag;
			}
			else
			{
				m_Tracker[j].bShow=false;
			}
		}
	}
	else
	{
		for(j=0;j<m_TrackerNumber;j++)
		{
			if(m_TrackerName[j][0]=='/') continue;
				m_Tracker[j].bShow=true;
		}
	}

	return  0;
}
int CROIControl::SetShowROI(TCHAR * RoiName, bool flag)
{
	int j;

	for(j=0;j<m_TrackerNumber;j++)
	{
		if(_tcscmp(m_TrackerName[j],RoiName)==0)
		{
			m_Tracker[j].bShow=flag;
			if(!flag)
			{
				m_Tracker[j].isResize=false;
				if(j==g_CurWindowNum)  g_CurWindowNum=-1;
			}
			break;
		}
	}

	return  0;
}

//해당이름의 ROI를 Active 시킨다. 
void CROIControl::SetActiveROI( TCHAR * RoiName)
{
	int i;

	g_CurWindowNum=GetROINumber(RoiName);
	if(g_CurWindowNum==-1)
	{
		//아무것도 선택된것이 없으며 Resize된 ROI를 모두 원상복귀한다.----------
		m_nResizeCount=0;
		for(i=0;i<(int)m_TrackerNumber;i++)
		{
			m_Tracker[i].isResize=false;
		}			
		//----------------------------------------------------------------------
	}
}

int CROIControl::GetROIName(int nRoiNumber, TCHAR *RoiName)
{
	if(nRoiNumber<0 || m_TrackerNumber>=0) return -1;

	_tcscpy(RoiName, m_TrackerName[nRoiNumber]);
	return 0;
}

int CROIControl::GetActiveROI(TCHAR *RoiName)		//현재 Active된 ROI이름을 읽어온다.
{
	if(g_CurWindowNum<0) return -1;

	_tcscpy(RoiName, m_TrackerName[g_CurWindowNum]);

	return 0;
}
void CROIControl::ResetResize()
{
	for(int i=0;i<m_TrackerNumber;i++)
	{
		m_Tracker[i].isResize=false;
	}
}

//{마우스 관련 함수 ====================================================================
int CROIControl::LButtonDown(UINT nFlags, CPoint point, CWnd *pWnd)
{
	int i, nReturn=-1, Length, MinLength=1000, nMinPos=-1;
	CRectTracker tracker;

	//초기화 
	m_IsROIClicked=false;
	if(m_dScaleY<m_dScaleX)
        tracker.m_nHandleSize=15/m_dScaleY;
	else
        tracker.m_nHandleSize=15/m_dScaleX;

	if(m_bTrackerHandleIN)
		tracker.m_nStyle=CRectTracker::resizeInside;
	else
		tracker.m_nStyle=CRectTracker::resizeOutside;

	m_TrackerON=false;


	if(m_FlagShowROI && m_FlagEnableROI)
	{
		if(g_CurWindowNum>=0 && !m_Tracker[g_CurWindowNum].full) //Active된게 있으면여기서 Size변경가능 
		{
			tracker.m_rect=m_Tracker[g_CurWindowNum].rect;

			if((i=tracker.HitTest(point))>=0 && i!=8) 
			{
				m_nHitTest=i;
				m_TrackerON=true;
				m_cOldPnt=point;
				m_IsROIClicked=true;
				nReturn=g_CurWindowNum;
				goto done;
			}

			if(PtInROI(g_CurWindowNum, point)==0)
			{
				m_Tracker[g_CurWindowNum].isResize=true;
				m_nResizeCount=1;
				nMinPos=g_CurWindowNum;
			}
		}

//ROI중에서 point와 가장 가까운 ROI 구함 --------------------------------
//여기서는 움직일수 있도록만 설정됨.
		if(nMinPos<0)
		{
			for(i=0;i<(int)m_TrackerNumber;i++)
			{
				if(m_FlagEnableOneROI) //이 flag가 설정되어 있으면 해당 ROI만 Enable된다.
				{
					if(_tcscmp(m_TrackerName[i],m_OneTrackerName) && m_TrackerName[i][0]!='/')
						continue;
				}
				if(m_Tracker[i].full) continue;
				if(!m_Tracker[i].bShow) continue;
				if(m_nResizeCount<=1) m_Tracker[i].isResize=false;
				if(PtInROI(i, point)==0) //보이는 사각형 안에 있는지 체크 
				{
					Length=GetMinLength(i, point);
					if(Length<MinLength)
					{
						nMinPos=i;
						MinLength=Length;
					}
				}
			}
		}
//----------------------------------------------------------------------

		if(nMinPos>=0)
		{
			m_Tracker[nMinPos].isResize=true;
			m_nResizeCount=1;
			g_CurWindowNum=nMinPos;

			//한개가 선택이 되면 
			m_cOldPnt=point;
			m_IsROIClicked=true;
			nReturn=0;
			goto done;
		}

		nReturn=g_CurWindowNum;
		g_CurWindowNum=-1;

		//아무것도 선택된것이 없으며 Resize된 ROI를 모두 원상복귀한다.----------
		m_nResizeCount=0;
		for(i=0;i<(int)m_TrackerNumber;i++)
		{
			m_Tracker[i].isResize=false;
		}			
		//----------------------------------------------------------------------
    }
done:
//	((CKoFormView*)((CImageDisplay*)pWnd)->m_pView)->ReDrawImage(false);
	return nReturn;
}
/*
int CROIControl::LButtonDown(UINT nFlags, CPoint point, CWnd *pWnd, int nCam)
{
	int i, nReturn=0, Length, MinLength=1000, nMinPos=-1;
	CRectTracker tracker;

	//초기화 
	m_IsROIClicked=false;
	tracker.m_nHandleSize=10;
	tracker.m_nStyle=CRectTracker::resizeInside;


	if(m_FlagShowROI && m_FlagEnableROI)
	{
		if(g_CurWindowNum>=0 && !m_Tracker[g_CurWindowNum].full) //Active된게 있으면여기서 Size변경가능 
		{
			tracker.m_rect=m_Tracker[g_CurWindowNum].rect;
//			tracker.m_rect.left=m_Tracker[g_CurWindowNum].rect.left*m_dScale;
//			tracker.m_rect.top=m_Tracker[g_CurWindowNum].rect.top*m_dScale;
//			tracker.m_rect.right=m_Tracker[g_CurWindowNum].rect.right*m_dScale;
//			tracker.m_rect.bottom=m_Tracker[g_CurWindowNum].rect.bottom*m_dScale;
//			point.x*=m_dScale;
//			point.y*=m_dScale;
			if((i=tracker.HitTest(point))>=0 && i!=8) //=없앰.(0613)
			{
				tracker.Track(pWnd, point, FALSE, NULL);
				//데이타 체크>>>>>>>>>>>>
				if(tracker.m_rect.left<0 || tracker.m_rect.top<0 ||
				   tracker.m_rect.right>=m_WIDTH*m_dScale || tracker.m_rect.bottom>=m_HEIGHT*m_dScale )
				{
					nReturn= -1;
					goto done;
				}
				
				m_Tracker[g_CurWindowNum].rect.left=(tracker.m_rect.left  < 0)?  
													0: tracker.m_rect.left/m_dScale;
				m_Tracker[g_CurWindowNum].rect.right=(tracker.m_rect.right >= m_WIDTH*m_dScale)?  
													m_WIDTH-1: tracker.m_rect.right/m_dScale;
				m_Tracker[g_CurWindowNum].rect.top  =(tracker.m_rect.top   < 0)?  
													0: tracker.m_rect.top/m_dScale;
				m_Tracker[g_CurWindowNum].rect.bottom=(tracker.m_rect.bottom>= m_HEIGHT*m_dScale)?  
													m_HEIGHT-1: tracker.m_rect.bottom/m_dScale;
				nReturn=g_CurWindowNum;
				goto done;
			}

			if(PtInROI(g_CurWindowNum, point)==0)
			{
				m_Tracker[g_CurWindowNum].isResize=true;
				m_nResizeCount=1;
				nMinPos=g_CurWindowNum;
			}
		}

//ROI중에서 point와 가장 가까운 ROI 구함 --------------------------------
//여기서는 움직일수 있도록만 설정됨.
		if(nMinPos<0)
		{
			for(i=0;i<(int)m_TrackerNumber;i++)
			{
				if(m_Tracker[i].full) continue;
				if(!m_Tracker[i].bShow) continue;
				if(m_nResizeCount<=1) m_Tracker[i].isResize=false;
				if(PtInROI(i, point)==0) //보이는 사각형 안에 있는지 체크 
				{
					Length=GetMinLength(i, point);
					if(Length<MinLength)
					{
						nMinPos=i;
						MinLength=Length;
					}
				}
			}
		}
//----------------------------------------------------------------------

		if(nMinPos>=0)
		{
			m_Tracker[nMinPos].isResize=true;
			m_nResizeCount=1;
			g_CurWindowNum=nMinPos;

			//한개가 선택이 되면 
			m_cOldPnt=point;
			m_IsROIClicked=true;
			nReturn=0;
			goto done;
		}

		nReturn=g_CurWindowNum=-1;

		//아무것도 선택된것이 없으며 Resize된 ROI를 모두 원상복귀한다.----------
		m_nResizeCount=0;
		for(i=0;i<(int)m_TrackerNumber;i++)
		{
			m_Tracker[i].isResize=false;
		}			
		//----------------------------------------------------------------------
    }
done:
//	((CKoFormView*)((CImageDisplay*)pWnd)->m_pView)->ReDrawImage(false);
	return nReturn;
}
*/
bool CROIControl::OnMouseMove(UINT nFlags, CPoint point, CWnd *pWnd)
{
	int i;
	CRect rc;
	int nNum=g_CurWindowNum;

	if(m_IsROIClicked)
	{
		//Size 변경에 사용됨 ==========================================================================================================
		if(m_TrackerON && nNum>=0) 
		{
			rc=m_Tracker[nNum].rect;

			switch(m_nHitTest)
			{
				case CRectTracker::hitTopLeft:	
								rc.top+=(point.y-m_cOldPnt.y);		
								rc.left+=(point.x-m_cOldPnt.x);		
								break;
				case CRectTracker::hitTopRight:		
								rc.top+=(point.y-m_cOldPnt.y);
								rc.right+=(point.x-m_cOldPnt.x);	
								break;
				case CRectTracker::hitBottomRight:	
								rc.bottom+=(point.y-m_cOldPnt.y);
								rc.right+=(point.x-m_cOldPnt.x);
								break;
				case CRectTracker::hitBottomLeft:	
								rc.bottom+=(point.y-m_cOldPnt.y);
								rc.left+=(point.x-m_cOldPnt.x);	
								break;
				case CRectTracker::hitTop:			
								rc.top+=(point.y-m_cOldPnt.y);		
								break;
				case CRectTracker::hitRight:	
								rc.right+=(point.x-m_cOldPnt.x);	
								break;
				case CRectTracker::hitBottom:		
								rc.bottom+=(point.y-m_cOldPnt.y);
								break; 
				case CRectTracker::hitLeft:			
								rc.left+=(point.x-m_cOldPnt.x);		
								break;			
	
			}
			if(rc.left<0 || rc.top<0 || rc.right>=m_WIDTH || rc.bottom>=m_HEIGHT ||
				rc.right<rc.left+3 || rc.bottom <rc.top+3)
			{
				goto done;
			}
			
			m_Tracker[nNum].rect.left=(rc.left  < 0)?  0: rc.left;
			m_Tracker[nNum].rect.right=(rc.right >= m_WIDTH)?  m_WIDTH-1: rc.right;
			m_Tracker[nNum].rect.top  =(rc.top   < 0)?  0: rc.top;
			m_Tracker[nNum].rect.bottom=(rc.bottom>= m_HEIGHT)?  m_HEIGHT-1: rc.bottom;
		}
		//================================================================================================================================
		else
		{
			for(i=0;i<m_TrackerNumber;i++)
			{
				if(m_Tracker[i].nAngle==0) //ROI가 Angle=0일때 사용(보통)
				{
					if(m_Tracker[i].isResize)
					{
						if(m_Tracker[i].rect.left+(point.x-m_cOldPnt.x)<0) goto done;				//@20161114  10을 0으로 변경
						if(m_Tracker[i].rect.right+(point.x-m_cOldPnt.x)>=m_WIDTH) goto done;
						if(m_Tracker[i].rect.top+(point.y-m_cOldPnt.y)<0) goto done;				//@20161114  10을 0으로 변경
						if(m_Tracker[i].rect.bottom+(point.y-m_cOldPnt.y)>=m_HEIGHT) goto done;
					}
				}
				else //ROI가 기울어졌을때 사용함)
				{
					if(m_Tracker[i].isResize)
					{
						if(m_Tracker[i].pt[0].x+(point.x-m_cOldPnt.x)<10) goto done;
						if(m_Tracker[i].pt[1].x+(point.x-m_cOldPnt.x)<10) goto done;
						if(m_Tracker[i].pt[2].x+(point.x-m_cOldPnt.x)>=m_WIDTH) goto done;
						if(m_Tracker[i].pt[3].x+(point.x-m_cOldPnt.x)>=m_WIDTH) goto done;
						if(m_Tracker[i].pt[0].y+(point.y-m_cOldPnt.y)<10) goto done;
						if(m_Tracker[i].pt[3].y+(point.y-m_cOldPnt.y)<10) goto done;
						if(m_Tracker[i].pt[1].y+(point.y-m_cOldPnt.y)>=m_HEIGHT) goto done;
						if(m_Tracker[i].pt[2].y+(point.y-m_cOldPnt.y)>=m_HEIGHT) goto done;
					}
				}
			}

			for(i=0;i<m_TrackerNumber;i++)
			{
				if(m_Tracker[i].isResize)
				{
					m_Tracker[i].rect.left+=(point.x-m_cOldPnt.x);
					m_Tracker[i].rect.right+=(point.x-m_cOldPnt.x);
					m_Tracker[i].rect.top+=(point.y-m_cOldPnt.y);
					m_Tracker[i].rect.bottom+=(point.y-m_cOldPnt.y);
				}
			}
		}
//	((CKoFormView*)((CImageDisplay*)pWnd)->m_pView)->ReDrawImage(false);
done:
		m_cOldPnt=point;
	}

	return m_IsROIClicked;
}
void CROIControl::OnLButtonUp(UINT nFlags, CPoint point, CWnd *pWnd)
{
	m_IsROIClicked=false;
	m_TrackerON=false;		//Size 변경 상태에서 풀림 

//	((CKoFormView*)((CImageDisplay*)pWnd)->m_pView)->ReDrawImage(false);
}

int CROIControl::PtInROI(int nNum, CPoint point)
{
	int nResult=-1;
	CRectTracker tracker;

	if(m_Tracker[nNum].nAngle==0)
	{
		tracker.m_rect=m_Tracker[nNum].rect;
		if(tracker.HitTest(point)>=0)
		{
			nResult=0;
		}
	}
	else if(PtInPolygon(m_Tracker[nNum].pt,4,point))
	{
		nResult=0;
	}

	return nResult;
}

//ROI를 Active된것처럼 보이게 한다.
int CROIControl::SetResizeShapeROI(CWnd *pWnd, int left, int top, int right, int bottom)
{
	int i,j;
	bool IsPtOut;

	m_nResizeCount=0;
	for(i=0;i<m_TrackerNumber;i++)
	{
		if(m_Tracker[i].nAngle==0)
		{
			if(m_Tracker[i].rect.left>left && m_Tracker[i].rect.right<right &&
			   m_Tracker[i].rect.top > top && m_Tracker[i].rect.bottom<bottom)
			{
				m_Tracker[i].isResize=true;
				m_nResizeCount++;
				g_CurWindowNum=i;
			}
		}
		else
		{
			IsPtOut=false;
			for(j=0;j<4;j++)
			{
				if(!(m_Tracker[i].pt[j].x>left && m_Tracker[i].pt[j].x<right &&
				    m_Tracker[i].pt[j].y > top && m_Tracker[i].pt[j].y<bottom)) 
					IsPtOut=true;

			}
			if(!IsPtOut)
			{
				m_Tracker[i].isResize=true;
				m_nResizeCount++;
				g_CurWindowNum=i;
			}
		}
	}
	if(m_nResizeCount>=1) g_CurWindowNum=-1;
//	((CKoFormView*)((CImageDisplay*)pWnd)->m_pView)->ReDrawImage(false);

	return 0;
}
//ROI를 Active된것처럼 보이게 한다. =>원래대로 한다.
int CROIControl::ResetResizeShapeROI()
{
	int i;

	m_nResizeCount=0;
	for(i=0;i<m_TrackerNumber;i++)
	{
		m_Tracker[i].isResize=false;
	}
	return 0;
}

//이영역에 해당하는 ROI의 내부 번호를 구한다.
int CROIControl::GetActiveNumber(POINT point)
{
	int i;
	CRectTracker tracker;

	if(g_CurWindowNum>=0)
	{
		if(m_Tracker[g_CurWindowNum].full)
		{
			tracker.m_rect.left=0;
			tracker.m_rect.top=0;
			tracker.m_rect.right=m_WIDTH-1;
			tracker.m_rect.bottom=m_HEIGHT-1;
			return g_CurWindowNum;
		}
		else
		{
			tracker.m_rect=m_Tracker[g_CurWindowNum].rect;

			tracker.m_rect.left+=m_dOffsetX;
			tracker.m_rect.top+=m_dOffsetY;
			tracker.m_rect.right+=m_dOffsetX;
			tracker.m_rect.bottom+=m_dOffsetY;

			if(tracker.HitTest(point)>=0)
			{
				return g_CurWindowNum;
			}
		}
	}
	
	for(i=(int)m_TrackerNumber-1;i>=0;i--)
	{
		if(!m_Tracker[i].bShow) continue;
		if(m_Tracker[i].nAngle==0)
		{
			tracker.m_rect.left+=m_dOffsetX;
			tracker.m_rect.top+=m_dOffsetY;
			tracker.m_rect.right+=m_dOffsetX;
			tracker.m_rect.bottom+=m_dOffsetY;

			tracker.m_rect=m_Tracker[i].rect;
			if(tracker.HitTest(point)>=0)	return i;
		}
		else
		{
			if(PtInPolygon( m_Tracker[i].pt,4,point))
			{
				g_CurWindowNum=i;
				return i;
			}
		}
	}
	
	return -1;
}

//ROI에 각도를 넣는다.(-90~90)
int CROIControl::SetROIAngle(TCHAR * RoiName, int nDegree)
{
	double dCx, dCy;
	int j, nROINumber=-1;


	nROINumber=GetROINumber(RoiName);

	if(nROINumber<0) return -1;

	if(nDegree<-90) nDegree=-90;
	else if(nDegree>90) nDegree=90;

	dCx=(m_Tracker[nROINumber].rect.left+m_Tracker[nROINumber].rect.right)*0.5;
	dCy=(m_Tracker[nROINumber].rect.top+m_Tracker[nROINumber].rect.bottom)*0.5;

	PointRotation(dCx, dCy, nDegree, m_Tracker[nROINumber].rect.left, m_Tracker[nROINumber].rect.top, 
									 &m_Tracker[nROINumber].pt[0].x, &m_Tracker[nROINumber].pt[0].y);
	PointRotation(dCx, dCy, nDegree, m_Tracker[nROINumber].rect.left, m_Tracker[nROINumber].rect.bottom, 
									 &m_Tracker[nROINumber].pt[1].x, &m_Tracker[nROINumber].pt[1].y);
	PointRotation(dCx, dCy, nDegree, m_Tracker[nROINumber].rect.right, m_Tracker[nROINumber].rect.bottom, 
									 &m_Tracker[nROINumber].pt[2].x, &m_Tracker[nROINumber].pt[2].y);
	PointRotation(dCx, dCy, nDegree, m_Tracker[nROINumber].rect.right, m_Tracker[nROINumber].rect.top, 
									 &m_Tracker[nROINumber].pt[3].x, &m_Tracker[nROINumber].pt[3].y);

	if(m_Tracker[nROINumber].pt[0].x<0 || m_Tracker[nROINumber].pt[1].x<0 || 
	   m_Tracker[nROINumber].pt[2].x>=m_WIDTH || m_Tracker[nROINumber].pt[3].x>=m_WIDTH ||
	   m_Tracker[nROINumber].pt[0].y<0 || m_Tracker[nROINumber].pt[3].y<0 ||
	   m_Tracker[nROINumber].pt[1].y>=m_HEIGHT || m_Tracker[nROINumber].pt[3].y>=m_HEIGHT)
	{
		m_Tracker[nROINumber].nAngle=0;
		return -1;
	}
	m_Tracker[nROINumber].nAngle=nDegree;

	return 0;
}	

//ROI에 각도를 넣는다.(-90~90)
int CROIControl::SetROIAngle(int nROINumber, int nDegree)
{
	double dCx, dCy;

	dCx=(m_Tracker[nROINumber].rect.left+m_Tracker[nROINumber].rect.right)*0.5;
	dCy=(m_Tracker[nROINumber].rect.top+m_Tracker[nROINumber].rect.bottom)*0.5;

	PointRotation(dCx, dCy, nDegree, m_Tracker[nROINumber].rect.left, m_Tracker[nROINumber].rect.top, 
									 &m_Tracker[nROINumber].pt[0].x, &m_Tracker[nROINumber].pt[0].y);
	PointRotation(dCx, dCy, nDegree, m_Tracker[nROINumber].rect.left, m_Tracker[nROINumber].rect.bottom, 
									 &m_Tracker[nROINumber].pt[1].x, &m_Tracker[nROINumber].pt[1].y);
	PointRotation(dCx, dCy, nDegree, m_Tracker[nROINumber].rect.right, m_Tracker[nROINumber].rect.bottom, 
									 &m_Tracker[nROINumber].pt[2].x, &m_Tracker[nROINumber].pt[2].y);
	PointRotation(dCx, dCy, nDegree, m_Tracker[nROINumber].rect.right, m_Tracker[nROINumber].rect.top, 
									 &m_Tracker[nROINumber].pt[3].x, &m_Tracker[nROINumber].pt[3].y);

	if(m_Tracker[nROINumber].pt[0].x<0 || m_Tracker[nROINumber].pt[1].x<0 || 
	   m_Tracker[nROINumber].pt[2].x>=m_WIDTH || m_Tracker[nROINumber].pt[3].x>=m_WIDTH ||
	   m_Tracker[nROINumber].pt[0].y<0 || m_Tracker[nROINumber].pt[3].y<0 ||
	   m_Tracker[nROINumber].pt[1].y>=m_HEIGHT || m_Tracker[nROINumber].pt[3].y>=m_HEIGHT)
	{
//		m_Tracker[nROINumber].nAngle=0;
		return -1;
	}
	m_Tracker[nROINumber].nAngle=nDegree;

	return 0;
}	

int  CROIControl::SetROIOffset(double dOffsetX, double dOffsetY, double dOffsetR)	//ROI에 Offset을 설정한다.
{
	m_dOffsetX=dOffsetX;
	m_dOffsetY=dOffsetY;
	m_dOffsetR=dOffsetR;

	return 0;
}

int  CROIControl::GetROIAngle(int nROINumber)
{
	return m_Tracker[nROINumber].nAngle;
}

//@A
int  CROIControl::GetROIThreshold(int nROINumber,  BOOL *bAutoThreshold, BOOL *bInveerse, int *nThreshold, int *nThresholdL)
{
	*bAutoThreshold=m_Tracker[nROINumber].bAutoThreshold;
	*bInveerse=m_Tracker[nROINumber].bInverse;
	*nThreshold=m_Tracker[nROINumber].nThreshold;
	*nThresholdL=m_Tracker[nROINumber].nThresholdL;

	return 0;
}
//@A
int  CROIControl::SetROIThreshold(int nROINumber, BOOL bAutoThreshold, BOOL bInverse, int nThreshold, int nThresholdL)
{
	m_Tracker[nROINumber].bAutoThreshold=bAutoThreshold;
	m_Tracker[nROINumber].bInverse		=bInverse;
	m_Tracker[nROINumber].nThreshold	  =nThreshold;
	m_Tracker[nROINumber].nThresholdL	  =nThresholdL;

	return 0;
}
//Active된 ROI의 영역을 읽어오는 함수 
//return	 0 :  OK
//			 -1: 해당이름의 ROI없음 
int CROIControl::GetActiveROIRegion(int *left, int *top, int *right, int *bottom)
{
	if(g_CurWindowNum>=0)
	{
		*left=m_Tracker[g_CurWindowNum].rect.left;
		*top=m_Tracker[g_CurWindowNum].rect.top;
		*right=m_Tracker[g_CurWindowNum].rect.right;
		*bottom=m_Tracker[g_CurWindowNum].rect.bottom;
		return 0;
	}
	else return -1;
}

//회전중심 (Cx, Cy)
//Point의 Translation값 (Dx, Dy) , Translation이없으면 (0, 0)
//dAngleDeg 회전각(degree)
int CROIControl::PointRotation(double Cx, double Cy, double dAngleDeg, double Px, double Py,  double *Px_, double *Py_)
{
	double sinT=sin(dAngleDeg*ROI_PI/180.0);
	double cosT=cos(dAngleDeg*ROI_PI/180.0);

	*Px_=(Px-Cx)*cosT-(Py-Cy)*sinT+Cx;
	*Py_=(Px-Cx)*sinT+(Py-Cy)*cosT+Cy;

	return 0;
}

bool CROIControl::PtInPolygon(TPoint poly[], int nSize, POINT pt) 
{
	int i, j;
	BOOL c = FALSE;

	for(i=0;i<4;i++)
	{
		poly[i].x+=m_dOffsetX;
		poly[i].y+=m_dOffsetY;
	}

	for (i = 0, j = nSize-1; i < nSize; j = i++) {
		if ( (((poly[i].y<=pt.y) && (pt.y<poly[j].y)) ||
		     ((poly[j].y<=pt.y) && (pt.y<poly[i].y))) &&
		     (pt.x < (poly[j].x - poly[i].x) * (pt.y - poly[i].y) /
		     (poly[j].y - poly[i].y) + poly[i].x))
		     c = !c;
	}
	return c;
}

//}===============================================================================================================
//{Draw ROI KoForm 전용함수 ======================================================================================
void CROIControl::DrawROI(CDC *pDC)
{
	//{BEGIN-TRACKER : 트랙커 그리기 >>>>>>>>>>>>>>>>>
	if(m_FlagShowROI)
    {
		int i, j;
		CString str;
		CBrush *pOldBrush, *pNewBrush= new CBrush(m_ROIColor);
		CPen *oldPen, *pNewPen=new CPen(PS_SOLID, 1, m_ROIColor);
		CRect	rc;
		int nSizeI=1, nSizeO=7, nSize=5;
		RECT  nrc;
		double dOffsetX, dOffsetY;

		if(m_bTrackerHandleIN)
		{
			nSizeI=7;
			nSizeO=0;
		}

		pOldBrush = pDC->SelectObject(pNewBrush);
		oldPen = pDC->SelectObject(pNewPen);
		pDC->SetTextAlign(TA_LEFT|TA_TOP);
		pDC->SetTextColor(m_TextColor);
		pDC->SetBkMode(TRANSPARENT);

		for(i=0;i<(int)m_TrackerNumber;i++)
		{
			if(!m_Tracker[i].bShow) continue;  //전체 화면이면 그리지 않는다.
			if(m_Tracker[i].full)
			{
				DrawRect(pDC, pNewBrush, i);
				continue;
			}	
			if(m_Tracker[i].isResize && g_CurWindowNum!=i)
			{
				if(m_TrackerName[i][0]=='@' || m_TrackerName[i][0]=='/')
				{
					dOffsetX=dOffsetY=0;
				}
				else
				{
					dOffsetX=m_dOffsetX;
					dOffsetY=m_dOffsetY;
				}
				rc.left		=m_Tracker[i].rect.left+	(int)dOffsetX;
				rc.top		=m_Tracker[i].rect.top+		(int)dOffsetY;
				rc.right	=m_Tracker[i].rect.right+	(int)dOffsetX;
				rc.bottom	=m_Tracker[i].rect.bottom+	(int)dOffsetY;
				
				ChangePointPosition(rc.left, rc.top, &nrc.left, &nrc.top);
				ChangePointPosition(rc.right, rc.bottom, &nrc.right, &nrc.bottom);
				rc=nrc;

				//모서리 (좌상, 우상, 좌하, 우하 )
				pDC->FrameRect(CRect(rc.left-nSizeO, rc.top-nSizeO,	rc.left+nSizeI,	rc.top+nSizeI),pNewBrush);
				pDC->FrameRect(CRect(rc.right-nSizeI,rc.top-nSizeO,	rc.right+nSizeO,rc.top+nSizeI),pNewBrush);
				pDC->FrameRect(CRect(rc.left-nSizeO, rc.bottom-nSizeI,  rc.left+nSizeI,  rc.bottom+nSizeO),pNewBrush);
				pDC->FrameRect(CRect(rc.right-nSizeI,rc.bottom-nSizeI,  rc.right+nSizeO, rc.bottom+nSizeO),pNewBrush);
				//변 (위, 아래, 좌, 우)
				if(	(rc.right-rc.left)>=48)
				{
					pDC->FrameRect(CRect((rc.left+rc.right)/2-nSize,	rc.top-nSizeO,
								   (rc.left+rc.right)/2+nSize,	rc.top+nSizeI),pNewBrush);
					pDC->FrameRect(CRect((rc.left+rc.right)/2-nSize,	rc.bottom-nSizeI,
								   (rc.left+rc.right)/2+nSize,	rc.bottom+nSizeO),pNewBrush);
				}
				if(	(rc.bottom-rc.top)>=48)
				{
					pDC->FrameRect(CRect(rc.left-nSizeO, (rc.top+rc.bottom)/2-nSize,
								   rc.left+nSizeI, (rc.top+rc.bottom)/2+nSize),pNewBrush);
					pDC->FrameRect(CRect(rc.right-nSizeI  ,(rc.top+rc.bottom)/2-nSize,
									rc.right+nSizeO  ,(rc.top+rc.bottom)/2+nSize),pNewBrush);	
				}
			}
			DrawRect(pDC, pNewBrush, i);
		}
		if(g_CurWindowNum>=0 && m_Tracker[g_CurWindowNum].bShow && !m_Tracker[g_CurWindowNum].full)
		{
				
				if(m_TrackerName[g_CurWindowNum][0]=='@' || m_TrackerName[g_CurWindowNum][0]=='/')
				{
					dOffsetX=dOffsetY=0;
				}
				else
				{
					dOffsetX=m_dOffsetX;
					dOffsetY=m_dOffsetY;
				}
				rc.left		=m_Tracker[g_CurWindowNum].rect.left+	(int)dOffsetX;
				rc.top		=m_Tracker[g_CurWindowNum].rect.top+	(int)dOffsetY;
				rc.right	=m_Tracker[g_CurWindowNum].rect.right+	(int)dOffsetX;
				rc.bottom	=m_Tracker[g_CurWindowNum].rect.bottom+	(int)dOffsetY;

				ChangePointPosition(rc.left, rc.top, &nrc.left, &nrc.top);
				ChangePointPosition(rc.right, rc.bottom, &nrc.right, &nrc.bottom);
				rc	=nrc;

				//모서리 (좌상, 우상, 좌하, 우하 )
				pDC->FrameRect(CRect(rc.left-nSizeO, rc.top-nSizeO,	rc.left+nSizeI,	rc.top+nSizeI),pNewBrush);
				pDC->FrameRect(CRect(rc.right-nSizeI,rc.top-nSizeO,	rc.right+nSizeO,rc.top+nSizeI),pNewBrush);
				pDC->FrameRect(CRect(rc.left-nSizeO, rc.bottom-nSizeI,  rc.left+nSizeI,  rc.bottom+nSizeO),pNewBrush);
				pDC->FrameRect(CRect(rc.right-nSizeI,rc.bottom-nSizeI,  rc.right+nSizeO, rc.bottom+nSizeO),pNewBrush);
				//변 (위, 아래, 좌, 우)
				if(	(rc.right-rc.left)>=48)
				{
					pDC->FrameRect(CRect((rc.left+rc.right)/2-nSize,	rc.top-nSizeO,
								   (rc.left+rc.right)/2+nSize,	rc.top+nSizeI),pNewBrush);
					pDC->FrameRect(CRect((rc.left+rc.right)/2-nSize,	rc.bottom-nSizeI,
								   (rc.left+rc.right)/2+nSize,	rc.bottom+nSizeO),pNewBrush);
				}
				if(	(rc.bottom-rc.top)>=48)
				{
					pDC->FrameRect(CRect(rc.left-nSizeO, (rc.top+rc.bottom)/2-nSize,
								   rc.left+nSizeI, (rc.top+rc.bottom)/2+nSize),pNewBrush);
					pDC->FrameRect(CRect(rc.right-nSizeI  ,(rc.top+rc.bottom)/2-nSize,
								   rc.right+nSizeO  ,(rc.top+rc.bottom)/2+nSize),pNewBrush);
				}
		}


		pDC->SelectObject(oldPen);
		pDC->SelectObject(pOldBrush);
		delete pNewPen;
		delete pNewBrush;    
	}    
    //}END-TRACKER <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<		 	
}
int CROIControl::ROIMove(int x, int y)
{
	int i;
	bool flag=false;
	KROI  m_TrackerBackup[MAXTRACKERNUMBER];
	memcpy(m_TrackerBackup, m_Tracker,  MAXTRACKERNUMBER*sizeof(KROI));

	for(i=0;i<m_TrackerNumber;i++)
	{
		if(m_Tracker[i].full) continue;
		if(m_TrackerName[i][0]=='@' || m_TrackerName[i][0]=='/') continue;

		m_Tracker[i].rect.left+=x;	if(m_Tracker[i].rect.left<0) flag=true;
		m_Tracker[i].rect.right+=x;	if(m_Tracker[i].rect.right>=m_WIDTH) flag=true;
		m_Tracker[i].rect.top+=y;		if(m_Tracker[i].rect.top<0) flag=true;
		m_Tracker[i].rect.bottom+=y;	if(m_Tracker[i].rect.bottom>=m_HEIGHT) flag=true;

		if(m_Tracker[i].nAngle)
		{
			m_Tracker[i].pt[0].x+=x;		if(m_Tracker[i].pt[0].x<0 || m_Tracker[i].pt[0].x>=m_WIDTH-1) flag=true;
			m_Tracker[i].pt[1].x+=x;		if(m_Tracker[i].pt[1].x<0 || m_Tracker[i].pt[1].x>=m_WIDTH-1) flag=true;
			m_Tracker[i].pt[2].x+=x;		if(m_Tracker[i].pt[2].x<0 || m_Tracker[i].pt[2].x>=m_WIDTH-1) flag=true;
			m_Tracker[i].pt[3].x+=x;		if(m_Tracker[i].pt[3].x<0 || m_Tracker[i].pt[3].x>=m_WIDTH-1) flag=true;
			m_Tracker[i].pt[0].y+=y;		if(m_Tracker[i].pt[0].y<0 || m_Tracker[i].pt[0].y>=m_HEIGHT-1) flag=true;
			m_Tracker[i].pt[1].y+=y;		if(m_Tracker[i].pt[1].y<0 || m_Tracker[i].pt[1].y>=m_HEIGHT-1) flag=true;
			m_Tracker[i].pt[2].y+=y;		if(m_Tracker[i].pt[2].y<0 || m_Tracker[i].pt[2].y>=m_HEIGHT-1) flag=true;
			m_Tracker[i].pt[3].y+=y;		if(m_Tracker[i].pt[3].y<0 || m_Tracker[i].pt[3].y>=m_HEIGHT-1) flag=true;
		}
	}

	if(	flag)
	{
		memcpy(m_Tracker, m_TrackerBackup, MAXTRACKERNUMBER*sizeof(KROI));
		return -1;
	}
	else
		return 0;
}
void CROIControl::ROISize(int x, int y)
{
	int i;
	bool flag=false;
	KROI  m_TrackerBackup[MAXTRACKERNUMBER];
	memcpy(m_TrackerBackup, m_Tracker,  MAXTRACKERNUMBER*sizeof(KROI));

	for(i=0;i<m_TrackerNumber;i++)
	{
		if(m_Tracker[i].full) continue;
		if(m_TrackerName[i][0]=='@' || m_TrackerName[i][0]=='/') continue;

//		if(m_Tracker[i].nAngle==0)
		{
			if(m_Tracker[i].rect.right+x>m_Tracker[i].rect.left+10)
			{
				m_Tracker[i].rect.right+=x;	if(m_Tracker[i].rect.right>=m_WIDTH) flag=true;
			}
			if(m_Tracker[i].rect.bottom+y>m_Tracker[i].rect.top+10)
			{
				m_Tracker[i].rect.bottom+=y;	if(m_Tracker[i].rect.bottom>=m_HEIGHT) flag=true;
			}
		}
	}

	if(	flag) memcpy(m_Tracker, m_TrackerBackup, MAXTRACKERNUMBER*sizeof(KROI));
}

void CROIControl::DrawRect(CDC *pDC, CBrush *pBrush, int nROI)
{
	int i;
	RECT rc;
	RECT  nrc;
	POINT pt[4];
	bool showflag=true;
	double dOffsetX, dOffsetY;
	CPen *oldPen, newPen(PS_DOT, 1, m_ROIColor);
	
	if(m_Tracker[nROI].full)
	{
		rc.left=rc.top=1;
		rc.right=m_WIDTH-1;
		rc.bottom=m_HEIGHT-1;

		ChangePointPosition(rc.left, rc.top, &nrc.left, &nrc.top);
		ChangePointPosition(rc.right, rc.bottom, &nrc.right, &nrc.bottom);

		pDC->FrameRect(&nrc,pBrush);
		
		return;
	}

	if(m_Tracker[nROI].nAngle==0)
	{
		if(m_TrackerName[nROI][0]=='@' || m_TrackerName[nROI][0]=='/')
		{
			dOffsetX=dOffsetY=0;
		}
		else
		{
			dOffsetX=m_dOffsetX;
			dOffsetY=m_dOffsetY;
		}

		//Offset 적용 ---
		rc.left=m_Tracker[nROI].rect.left+(int)dOffsetX;
		rc.top=m_Tracker[nROI].rect.top+(int)dOffsetY;
		rc.right=m_Tracker[nROI].rect.right+(int)dOffsetX;
		rc.bottom=m_Tracker[nROI].rect.bottom+(int)dOffsetY;
		//---------------

		if(rc.left>=0 &&  rc.right<m_WIDTH && rc.top>=0 && rc.bottom <m_HEIGHT)
		{		
			ChangePointPosition(rc.left, rc.top, &nrc.left, &nrc.top);
			ChangePointPosition(rc.right, rc.bottom, &nrc.right, &nrc.bottom);

			pDC->FrameRect(&nrc,pBrush);

/*			oldPen=pDC->SelectObject(&newPen);
			pDC->SelectStockObject(NULL_BRUSH);
			pDC->SetBkMode(TRANSPARENT);
			pDC->Ellipse(&nrc);
*/
			if(m_FlagShowText)
				pDC->TextOut(nrc.left+1,nrc.top+1,m_TrackerName[nROI]);

			if(m_FlagShowCenterLine)
			{
				oldPen=pDC->SelectObject(&newPen);
				pDC->MoveTo(nrc.left,(nrc.top+nrc.bottom)*0.5);
				pDC->LineTo(nrc.right,(nrc.top+nrc.bottom)*0.5);
				pDC->MoveTo((nrc.left+nrc.right)*0.5,nrc.top);
				pDC->LineTo((nrc.left+nrc.right)*0.5,nrc.bottom);
				pDC->SelectObject(oldPen);
			}
		}
	}
	else
	{
		SetROIAngle(nROI, m_Tracker[nROI].nAngle);

		for(i=0;i<4;i++)
		{
			if(m_TrackerName[nROI][0]=='@' || m_TrackerName[nROI][0]=='/')
			{
				dOffsetX=dOffsetY=0;
			}
			else
			{
				dOffsetX=m_dOffsetX;
				dOffsetY=m_dOffsetY;
			}
			ChangePointPosition((long)(m_Tracker[nROI].pt[i].x+dOffsetX+0.5), (long)(m_Tracker[nROI].pt[i].y+dOffsetY+0.5),
									   &pt[i].x, &pt[i].y);	
			if((long)(m_Tracker[nROI].pt[i].x+dOffsetX+0.5)<0 || (long)(m_Tracker[nROI].pt[i].x+dOffsetX+0.5)>=m_WIDTH ||
			   (long)(m_Tracker[nROI].pt[i].y+dOffsetY+0.5)<0 || (long)(m_Tracker[nROI].pt[i].y+dOffsetY+0.5)>=m_HEIGHT   )
				showflag=false;
		}

		if(showflag)
		{
			pDC->MoveTo(pt[0].x, pt[0].y);
			for(i=1;i<4;i++)
				pDC->LineTo(pt[i].x, pt[i].y);
			pDC->LineTo(pt[0].x, pt[0].y);
			if(m_FlagShowText)
				pDC->TextOut(pt[0].x+1,pt[0].y+1,m_TrackerName[nROI]);

			if(m_FlagShowCenterLine)
			{
				oldPen=pDC->SelectObject(&newPen);
				pDC->MoveTo((pt[0].x+pt[3].x)*0.5, (pt[0].y+pt[3].y)*0.5 );
				pDC->LineTo((pt[1].x+pt[2].x)*0.5, (pt[1].y+pt[2].y)*0.5 );
				pDC->MoveTo((pt[0].x+pt[1].x)*0.5, (pt[0].y+pt[1].y)*0.5 );
				pDC->LineTo((pt[2].x+pt[3].x)*0.5, (pt[2].y+pt[3].y)*0.5 );
				pDC->SelectObject(oldPen);
			}
		}
	}
}

int CROIControl::SetScaleNOffset(int nZoom, int nPanX, int nPanY)
{
	if(nZoom==1)	m_dScaleX=1.0;
	else if(nZoom==2) m_dScaleX=2.0;
	else if(nZoom==4) m_dScaleX=4.0;
	else if(nZoom==-2) m_dScaleX=0.5;
	else if(nZoom==-4) m_dScaleX=0.25;
	else if(nZoom==-8) m_dScaleX=0.125;
	else if(nZoom==-16) m_dScaleX=0.0625;
	else if(nZoom==-32) m_dScaleX=0.03125;

	m_dScaleY=m_dScaleX;
	m_nPanX=nPanX;
	m_nPanY=nPanY;

	return 0;
}

// KoForm에서만 사용함.
int CROIControl::ChangePointPosition(long Px, long Py, long *Px_, long *Py_)
{
	int nPos, nDir;
	double Cx, Cy;
		
//	*Px_=Px*m_dScale;
//	*Py_=Py*m_dScale;
	*Px_=Px*m_dScaleX-m_nPanX;
	*Py_=Py*m_dScaleY-m_nPanY;
//	*Px_=(Px-m_nPanX)*m_dScale;	
//	*Py_=(Py-m_nPanY)*m_dScale;


	return 0;
}
/*
		   0		왼쪽과 같은 모양으로 번호를 메긴다.
           *
          *  *
         *     * 1
		*     *
     3 *     *	 
         *  *
		   * 
		   2
*/
void CROIControl::NV_Binary(LPBYTE fm, KROI cROI)
{
	int  i, j;
	int leftX, rightX;
	BYTE tmp;
	double a[4],b[4], x1, x2, minY, maxY;
	TPoint p1, p2, p3, p4;

	int threshold=cROI.nThreshold;
	int thresholdL=cROI.nThresholdL;

	if(cROI.nAngle==0)
	{
		for(i=cROI.rect.top;i<cROI.rect.bottom;i++)
			for(j=cROI.rect.left;j<cROI.rect.right;j++)
			{
				tmp=*(fm+i*m_WIDTH+j);
				if(tmp>=thresholdL && tmp<threshold) *(fm+i*m_WIDTH+j)=0;
				else								 *(fm+i*m_WIDTH+j)=255;
			}
	}
	else
	{
		p1=cROI.pt[0];
		p2=cROI.pt[3];
		p3=cROI.pt[2];
		p4=cROI.pt[1];

		NV_GetAreaInRect_SubRoutine(p1, p2, p3, p4, a,b,&minY, &maxY);

		for(i=(int)(minY+0.5)+1; i<(int)(maxY+0.5); i++)
		{
			x1=a[0]*i+b[0];
			x2=a[1]*i+b[1];
			leftX=(x1>x2)? (int)(x1+0.5) : (int)(x2+0.5);

			x1=a[2]*i+b[2];
			x2=a[3]*i+b[3];
			rightX=(x1>x2)? (int)(x2+0.5) : (int)(x1+0.5);
			for(j=leftX+1;j<rightX;j++)
			{
				tmp=*(fm+i*m_WIDTH+j);
				if(tmp>=thresholdL && tmp<threshold) *(fm+i*m_WIDTH+j)=0;
				else								 *(fm+i*m_WIDTH+j)=255;
			}
		}
	}
}
void CROIControl::NV_BinaryDisplay(LPBYTE fmSrc, LPBYTE fmDst, KROI cROI)
{
	int  i, j;
	int leftX, rightX;
	BYTE tmp;
	double a[4],b[4], x1, x2, minY, maxY;
	TPoint p1, p2, p3, p4;
	int nInverse=cROI.bInverse;
	int threshold=cROI.nThreshold;
	int thresholdL=cROI.nThresholdL;

	if(cROI.full)
	{
		cROI.nAngle=0;
		cROI.rect.left=0;
		cROI.rect.top=0;
		cROI.rect.right=m_WIDTH-1;
		cROI.rect.bottom=m_HEIGHT-1;
	}
	else
	{
		cROI.rect.left+=m_dOffsetX;
		cROI.rect.top+=m_dOffsetY;
		cROI.rect.right+=m_dOffsetX;
		cROI.rect.bottom+=m_dOffsetY;
		if(cROI.rect.left>=m_WIDTH) cROI.rect.left=m_WIDTH-1;
		if(cROI.rect.top>=m_HEIGHT) cROI.rect.top=m_HEIGHT-1;
		if(cROI.rect.right>=m_WIDTH) cROI.rect.right=m_WIDTH-1;
		if(cROI.rect.bottom>=m_HEIGHT) cROI.rect.bottom=m_HEIGHT-1;
	}

	if(cROI.nAngle==0)
	{
		if(nInverse)
		{
			for(i=cROI.rect.top;i<cROI.rect.bottom;i++)
				for(j=cROI.rect.left;j<cROI.rect.right;j++)
				{
					tmp=*(fmSrc+i*m_WIDTH+j);
					if(tmp<threshold && tmp>=thresholdL) *(fmDst+m_WIDTH*i+j)=255;
					else							   *(fmDst+m_WIDTH*i+j)=0;
				}
		}
		else
		{
			for(i=cROI.rect.top;i<cROI.rect.bottom;i++)
				for(j=cROI.rect.left;j<cROI.rect.right;j++)
				{
					tmp=*(fmSrc+i*m_WIDTH+j);
					if(tmp<threshold && tmp>=thresholdL) *(fmDst+m_WIDTH*i+j)=0;
					else							     *(fmDst+m_WIDTH*i+j)=255;
				}
		}
	}
	else
	{
		for(i=0;i<4;i++)
		{
			cROI.pt[i].x+=m_dOffsetX;
			if(cROI.pt[i].x>=m_WIDTH) cROI.pt[i].x=m_WIDTH-1;
			cROI.pt[i].y+=m_dOffsetY;
			if(cROI.pt[i].y>=m_HEIGHT) cROI.pt[i].y=m_HEIGHT-1;
		}
		p1=cROI.pt[0];
		p2=cROI.pt[3];
		p3=cROI.pt[2];
		p4=cROI.pt[1];

		NV_GetAreaInRect_SubRoutine(p1, p2, p3, p4, a,b,&minY, &maxY);

		if(nInverse)
		{
			for(i=(int)(minY+0.5)+1; i<(int)(maxY+0.5); i++)
			{
				x1=a[0]*i+b[0];
				x2=a[1]*i+b[1];
				leftX=(x1>x2)? (int)(x1+0.5) : (int)(x2+0.5);

				x1=a[2]*i+b[2];
				x2=a[3]*i+b[3];
				rightX=(x1>x2)? (int)(x2+0.5) : (int)(x1+0.5);
				for(j=leftX+1;j<rightX;j++)
				{
					tmp=*(fmSrc+i*m_WIDTH+j);
					if(tmp<threshold && tmp>=thresholdL) *(fmDst+m_WIDTH*i+j)=255;
					else							   *(fmDst+m_WIDTH*i+j)=0;
				}
			}
		}
		else
		{
			for(i=(int)(minY+0.5)+1; i<(int)(maxY+0.5); i++)
			{
				x1=a[0]*i+b[0];
				x2=a[1]*i+b[1];
				leftX=(x1>x2)? (int)(x1+0.5) : (int)(x2+0.5);

				x1=a[2]*i+b[2];
				x2=a[3]*i+b[3];
				rightX=(x1>x2)? (int)(x2+0.5) : (int)(x1+0.5);
				for(j=leftX+1;j<rightX;j++)
				{
					tmp=*(fmSrc+i*m_WIDTH+j);
					if(tmp<threshold && tmp>=thresholdL) *(fmDst+m_WIDTH*i+j)=0;
					else							   *(fmDst+m_WIDTH*i+j)=255;
				}
			}
		}
	}
}
void CROIControl::NV_GrayDisplay(LPBYTE fmSrc, LPBYTE fmDst, KROI cROI)
{
	int  i, j;
	int leftX, rightX;
	BYTE tmp;
	double a[4],b[4], x1, x2, minY, maxY;
	TPoint p1, p2, p3, p4;
	int nInverse=cROI.bInverse;
	int threshold=cROI.nThreshold;
	int thresholdL=cROI.nThresholdL;

	if(cROI.full)
	{
		cROI.nAngle=0;
		cROI.rect.left=0;
		cROI.rect.top=0;
		cROI.rect.right=m_WIDTH-1;
		cROI.rect.bottom=m_HEIGHT-1;
	}
	else
	{
		cROI.rect.left+=m_dOffsetX;
		cROI.rect.top+=m_dOffsetY;
		cROI.rect.right+=m_dOffsetX;
		cROI.rect.bottom+=m_dOffsetY;
		if(cROI.rect.left>=m_WIDTH) cROI.rect.left=m_WIDTH-1;
		if(cROI.rect.top>=m_HEIGHT) cROI.rect.top=m_HEIGHT-1;
		if(cROI.rect.right>=m_WIDTH) cROI.rect.right=m_WIDTH-1;
		if(cROI.rect.bottom>=m_HEIGHT) cROI.rect.bottom=m_HEIGHT-1;
	}

	if(cROI.nAngle==0)
	{
		if(nInverse)
		{
			for(i=cROI.rect.top;i<cROI.rect.bottom;i++)
				for(j=cROI.rect.left;j<cROI.rect.right;j++)
				{
					*(fmDst+m_WIDTH*i+j)=*(fmSrc+i*m_WIDTH+j);
				}
		}
		else
		{
			for(i=cROI.rect.top;i<cROI.rect.bottom;i++)
				for(j=cROI.rect.left;j<cROI.rect.right;j++)
				{
					*(fmDst+m_WIDTH*i+j)=*(fmSrc+i*m_WIDTH+j);
				}
		}
	}
	else
	{
		for(i=0;i<4;i++)
		{
			cROI.pt[i].x+=m_dOffsetX;
			if(cROI.pt[i].x>=m_WIDTH) cROI.pt[i].x=m_WIDTH-1;
			cROI.pt[i].y+=m_dOffsetY;
			if(cROI.pt[i].y>=m_HEIGHT) cROI.pt[i].y=m_HEIGHT-1;
		}
		p1=cROI.pt[0];
		p2=cROI.pt[3];
		p3=cROI.pt[2];
		p4=cROI.pt[1];

		NV_GetAreaInRect_SubRoutine(p1, p2, p3, p4, a,b,&minY, &maxY);

		if(nInverse)
		{
			for(i=(int)(minY+0.5)+1; i<(int)(maxY+0.5); i++)
			{
				x1=a[0]*i+b[0];
				x2=a[1]*i+b[1];
				leftX=(x1>x2)? (int)(x1+0.5) : (int)(x2+0.5);

				x1=a[2]*i+b[2];
				x2=a[3]*i+b[3];
				rightX=(x1>x2)? (int)(x2+0.5) : (int)(x1+0.5);
				for(j=leftX+1;j<rightX;j++)
				{
					*(fmDst+m_WIDTH*i+j)=*(fmSrc+i*m_WIDTH+j);
				}
			}
		}
		else
		{
			for(i=(int)(minY+0.5)+1; i<(int)(maxY+0.5); i++)
			{
				x1=a[0]*i+b[0];
				x2=a[1]*i+b[1];
				leftX=(x1>x2)? (int)(x1+0.5) : (int)(x2+0.5);

				x1=a[2]*i+b[2];
				x2=a[3]*i+b[3];
				rightX=(x1>x2)? (int)(x2+0.5) : (int)(x1+0.5);
				for(j=leftX+1;j<rightX;j++)
				{
					*(fmDst+m_WIDTH*i+j)=*(fmSrc+i*m_WIDTH+j);
				}
			}
		}
	}
}
int CROIControl::NV_AutoThreshold(int nInverse, LPBYTE fm, KROI cROI, int *threshold, int *thresholdL)
{
	int i,j;
	const int GRAYLEVEL=256;
	DWORD  m_nHistoData[256];
	DWORD  m_nArea=0,nSum=0;
	double m_dU;     /* Average */
	int nCount=0, Pos[3];
	int nThreshold;
	double pt, qt, U1, U2, qtPrev;
	double SigmaB2, MaxSigmaB2=0;
	int leftX, rightX;
	double a[4],b[4], x1, x2, minY, maxY;
	TPoint p1, p2, p3, p4;

   
	/* 초기화 */
	for(i=0;i<GRAYLEVEL;i++) m_nHistoData[i]=0;

	/* Histogram Data 구하기 */
//	for(i=y1;i<=y2;i++)
//		for(j=x1;j<=x2;j++)
//			m_nHistoData[*(fm+i*pitch+j)]++;

	if(cROI.nAngle==0)
	{
		for(i=cROI.rect.top;i<cROI.rect.bottom;i++)
			for(j=cROI.rect.left;j<cROI.rect.right;j++)
				m_nHistoData[*(fm+i*m_WIDTH+j)]++;
	}
	else
	{
		p1=cROI.pt[0];
		p2=cROI.pt[3];
		p3=cROI.pt[2];
		p4=cROI.pt[1];

		NV_GetAreaInRect_SubRoutine(p1, p2, p3, p4, a,b,&minY, &maxY);

		for(i=(int)(minY+0.5)+1; i<(int)(maxY+0.5); i++)
		{
			x1=a[0]*i+b[0];
			x2=a[1]*i+b[1];
			leftX=(x1>x2)? (int)(x1+0.5) : (int)(x2+0.5);

			x1=a[2]*i+b[2];
			x2=a[3]*i+b[3];
			rightX=(x1>x2)? (int)(x2+0.5) : (int)(x1+0.5);
			for(j=leftX+1;j<rightX;j++)
			{
				m_nHistoData[*(fm+i*m_WIDTH+j)]++;
			}
		}
	}


	for(i=0;i<GRAYLEVEL;i++)
	{
		nSum+=i*m_nHistoData[i];
		m_nArea+=m_nHistoData[i];
	}

	/* 면적과 평균을 구한다. */
	m_dU   =(double)nSum/(double)m_nArea;

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


	qt=(double)m_nHistoData[0]/(double)m_nArea;
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
		pt=(double)m_nHistoData[i+1]/(double)m_nArea;
		qt=qtPrev+pt;
		if(qt==0)         continue;
		else if(qt>0.99999)  break;    /*qt가 1 이면 더이상은 의미가 없으므로 Break함*/
		U1=(qtPrev*U1+(i+1)*pt)/qt;
		U2=(m_dU-qt*U1)/(1-qt);		 
	}

	if(nInverse==0)
	{
		*threshold=nThreshold;
		*thresholdL=0;
	}
	else
	{
		*threshold=255;
		*thresholdL=nThreshold;
	}

	return nThreshold;
}
void CROIControl::NV_GetAreaNBinary(LPBYTE fm, KROI cROI, int threshold, int *BlackArea, int *WhiteArea, bool flag)
{
	int  i, j;
	int leftX, rightX;
	double a[4],b[4], x1, x2, minY, maxY;
	TPoint p1, p2, p3, p4;
	*BlackArea=0;
	*WhiteArea=0;

	if(cROI.nAngle==0)
	{
		for(i=cROI.rect.top;i<cROI.rect.bottom;i++)
			for(j=cROI.rect.left;j<cROI.rect.right;j++)
				if(*(fm+i*m_WIDTH+j)<threshold) 
				{
					*BlackArea++;
					if(flag) *(fm+i*m_WIDTH+j)=0;
				}
				else
				{
					*WhiteArea++;
					if(flag) *(fm+i*m_WIDTH+j)=255;
				}

	}
	else
	{
		p1=cROI.pt[0];
		p2=cROI.pt[3];
		p3=cROI.pt[2];
		p4=cROI.pt[1];

		NV_GetAreaInRect_SubRoutine(p1, p2, p3, p4, a,b,&minY, &maxY);

		for(i=(int)(minY+0.5)+1; i<(int)(maxY+0.5); i++)
		{
			x1=a[0]*i+b[0];
			x2=a[1]*i+b[1];
			leftX=(x1>x2)? (int)(x1+0.5) : (int)(x2+0.5);

			x1=a[2]*i+b[2];
			x2=a[3]*i+b[3];
			rightX=(x1>x2)? (int)(x2+0.5) : (int)(x1+0.5);
			for(j=leftX+1;j<rightX;j++)
			{
				if(*(fm+i*m_WIDTH+j)<threshold) 
				{
					*BlackArea++;
					if(flag) *(fm+i*m_WIDTH+j)=0;
				}
				else
				{
					*WhiteArea++;
					if(flag) *(fm+i*m_WIDTH+j)=255;
				}
			}
		}
	}
}
void CROIControl::NV_GetAreaCenterNBinary(LPBYTE fm, KROI cROI, int threshold, int IsBlackCenter,
										  int *BlackArea, int *WhiteArea, double *Cx, double *Cy, bool flag)
{
	int  i, j;
	int leftX, rightX;
	double a[4],b[4], x1, x2, minY, maxY;
	int nCxSumB=0;
	int nCySumB=0;
	int nCxSumW=0;
	int nCySumW=0;
	TPoint p1, p2, p3, p4;

	*BlackArea=0;
	*WhiteArea=0;

	if(cROI.nAngle==0)
	{
		for(i=cROI.rect.top;i<cROI.rect.bottom;i++)
			for(j=cROI.rect.left;j<cROI.rect.right;j++)
				if(*(fm+i*m_WIDTH+j)<threshold) 
				{
					(*BlackArea)++;
					nCxSumB+=j;
					nCySumB+=i;
					if(flag) *(fm+i*m_WIDTH+j)=0;
				}
				else
				{
					(*WhiteArea)++;
					nCxSumW+=j;
					nCySumW+=i;
					if(flag) *(fm+i*m_WIDTH+j)=255;
				}
	}
	else
	{
		p1=cROI.pt[0];
		p2=cROI.pt[3];
		p3=cROI.pt[2];
		p4=cROI.pt[1];

		NV_GetAreaInRect_SubRoutine(p1, p2, p3, p4, a,b,&minY, &maxY);

		for(i=(int)(minY+0.5)+1; i<(int)(maxY+0.5); i++)
		{
			x1=a[0]*i+b[0];
			x2=a[1]*i+b[1];
			leftX=(x1>x2)? (int)(x1+0.5) : (int)(x2+0.5);

			x1=a[2]*i+b[2];
			x2=a[3]*i+b[3];
			rightX=(x1>x2)? (int)(x2+0.5) : (int)(x1+0.5);
			for(j=leftX+1;j<rightX;j++)
			{
				if(*(fm+i*m_WIDTH+j)<threshold) 
				{
					(*BlackArea)++;
					nCxSumB+=j;
					nCySumB+=i;
					if(flag) *(fm+i*m_WIDTH+j)=0;
				}
				else
				{
					(*WhiteArea)++;
					nCxSumW+=j;
					nCySumW+=i;
					if(flag) *(fm+i*m_WIDTH+j)=255;
				}
			}
		}
	}
	if(IsBlackCenter)
	{
		if(*BlackArea==0) *Cx=*Cy=0;
		else
		{
			*Cx=nCxSumB/(*BlackArea);
			*Cy=nCySumB/(*BlackArea);
		}
	}
	else
	{
		if(*WhiteArea==0) *Cx=*Cy=0;
		else
		{
			*Cx=nCxSumW/(*WhiteArea);
			*Cy=nCySumW/(*WhiteArea);
		}
	}
}

void CROIControl::NV_GetAreaInRect_SubRoutine(TPoint p1, TPoint p2, TPoint p3, TPoint p4, 
												double *a, double *b, double *minY, double *maxY)
{
	int i, tmpN;
	double p[4][2];
	double pp[4][2];
	double tmpP;

	p[0][0]=p1.x; p[0][1]=p1.y;
	p[1][0]=p2.x; p[1][1]=p2.y;
	p[2][0]=p3.x; p[2][1]=p3.y;
	p[3][0]=p4.x; p[3][1]=p4.y;

	tmpP=p[0][1];
	tmpN=0;
	for(i=1;i<4;i++)
		if(p[i][1]<tmpP) { tmpP=p[i][1]; tmpN=i; }
		
	pp[0][0]=p[tmpN][0];
	pp[0][1]=p[tmpN][1];

	if(tmpN==0 || tmpN==2)
	{
		if(tmpN==0) { pp[2][0]=p[2][0]; pp[2][1]=p[2][1]; }
		if(tmpN==2) { pp[2][0]=p[0][0]; pp[2][1]=p[0][1]; }
		
		if(p[1][0]<p[3][0]) {
			pp[3][0]=p[1][0]; pp[3][1]=p[1][1];
			pp[1][0]=p[3][0]; pp[1][1]=p[3][1];
		}
		else{
			pp[3][0]=p[3][0]; pp[3][1]=p[3][1];
			pp[1][0]=p[1][0]; pp[1][1]=p[1][1];
		}
	}
	else
	{			
		if(tmpN==1) { pp[2][0]=p[3][0]; pp[2][1]=p[3][1]; }
		if(tmpN==3) { pp[2][0]=p[1][0]; pp[2][1]=p[1][1]; }
				
		if(p[0][0]<p[2][0]) {
			pp[3][0]=p[0][0]; pp[3][1]=p[0][1];
			pp[1][0]=p[2][0]; pp[1][1]=p[2][1];
		}
		else{
			pp[3][0]=p[2][0]; pp[3][1]=p[2][1];
			pp[1][0]=p[0][0]; pp[1][1]=p[0][1];
		}				
	}				

	/* Line 0-3 */
	if( fabs(pp[0][1]-pp[3][1])<0.1) a[0]=-10000.0;
	else	a[0]=(pp[0][0]-pp[3][0])/(pp[0][1]-pp[3][1]);
	b[0]=pp[0][0]-a[0]*pp[0][1];
	/* Line 3-2 */
	if( fabs(pp[3][1]-pp[2][1])<0.1) a[1]=10000.0;
	else	a[1]=(pp[3][0]-pp[2][0])/(pp[3][1]-pp[2][1]);		
	b[1]=pp[3][0]-a[1]*pp[3][1];
	/* Line 0-1 */
	if( fabs(pp[0][1]-pp[1][1])<0.1) a[2]=10000.0;
	else	a[2]=(pp[0][0]-pp[1][0])/(pp[0][1]-pp[1][1]);	
	b[2]=pp[0][0]-a[2]*pp[0][1];
	/* Line 1-2 */
	if( fabs(pp[1][1]-pp[2][1])<0.1) a[3]=-10000.0;
	else	a[3]=(pp[1][0]-pp[2][0])/(pp[1][1]-pp[2][1]);		
	b[3]=pp[1][0]-a[3]*pp[1][1];

	*minY=pp[0][1];
	*maxY=pp[2][1];
}

int CROIControl::NV_MAPImagePixel(LPBYTE fm, KROI cROI, LPBYTE *fmD, int *Width, int *Height)
{
	int i, j;
	int M, N, nPitch;
	int nOffsetX, nOffsetY;
	double i_, j_, a, b;
	double cosT, sinT;
	double cx, cy, CRx, CRy;
	LPBYTE Dum;

	*Width=cROI.rect.right-cROI.rect.left+1;
	*Height=cROI.rect.bottom-cROI.rect.top+1;
	nPitch=*Width;
	if(m_fmD==NULL) return -1;
	*fmD=m_fmD;

	if(cROI.nAngle==0)
	{
		nOffsetX=cROI.rect.left;
		nOffsetY=cROI.rect.top;
		for(i=0;i<*Height;i++)
			for(j=0;j<*Width;j++)
				*(*fmD+nPitch*i+j)=*(fm+(i+nOffsetY)*m_WIDTH+j+nOffsetX);
	}
	else
	{
		cosT=cos(cROI.nAngle*ROI_PI/180.0);
		sinT=sin(cROI.nAngle*ROI_PI/180.0);
		cx=(*Width-1)*0.5;
		cy=(*Height-1)*0.5;
		CRx=(cROI.rect.right+cROI.rect.left)*0.5;
		CRy=(cROI.rect.bottom+cROI.rect.top)*0.5;

		for(i=0;i<*Height;i++)
			for(j=0;j<*Width;j++)
			{
				j_=cosT*(j-cx)-sinT*(i-cy)+CRx;	
				i_=sinT*(j-cx)+cosT*(i-cy)+CRy;	
				N=(int)j_;
				M=(int)i_;

				if(N<=0 || N>=m_WIDTH || M<=0 ||M>=m_HEIGHT)
					*(*fmD+nPitch*i+j)=0;
				else
				{
					Dum=fm+M*m_WIDTH+N;
					*(*fmD+nPitch*i+j)=(BYTE)*(Dum);
				}
			}
	}

	return 0;
}

int CROIControl::NV_MAPImage(LPBYTE fm, KROI cROI, LPBYTE *fmD, int *Width, int *Height)
{
	int i, j;
	int M, N, nPitch;
	int nOffsetX, nOffsetY;
	double i_, j_, a, b;
	double cosT, sinT;
	double cx, cy, CRx, CRy;
	LPBYTE Dum;

	*Width=cROI.rect.right-cROI.rect.left+1;
	*Height=cROI.rect.bottom-cROI.rect.top+1;
	nPitch=*Width;
	if(m_fmD==NULL) return -1;
	*fmD=m_fmD;

	if(cROI.nAngle==0)
	{
		nOffsetX=cROI.rect.left;
		nOffsetY=cROI.rect.top;
		for(i=0;i<*Height;i++)
			for(j=0;j<*Width;j++)
				*(*fmD+nPitch*i+j)=*(fm+(i+nOffsetY)*m_WIDTH+j+nOffsetX);
	}
	else
	{
		cosT=cos(cROI.nAngle*ROI_PI/180.0);
		sinT=sin(cROI.nAngle*ROI_PI/180.0);
		cx=(*Width-1)*0.5;
		cy=(*Height-1)*0.5;
		CRx=(cROI.rect.right+cROI.rect.left)*0.5;
		CRy=(cROI.rect.bottom+cROI.rect.top)*0.5;

		for(i=0;i<*Height;i++)
			for(j=0;j<*Width;j++)
			{
				j_=cosT*(j-cx)-sinT*(i-cy)+CRx;	
				i_=sinT*(j-cx)+cosT*(i-cy)+CRy;	
				N=(int)j_;
				M=(int)i_;
				a=j_-N;
				b=i_-M;

				if(N<=0 || N>=m_WIDTH || M<=0 ||M>=m_HEIGHT)
					*(*fmD+nPitch*i+j)=0;
				else
				{
					Dum=fm+M*m_WIDTH+N;
					*(*fmD+nPitch*i+j)=(BYTE)(*(Dum)*(1.0-a)*(1.0-b)+
											  *(Dum+1)*(a)*(1.0-b)+
											  *(Dum+m_WIDTH)*(1.0-a)*(b)+
											  *(Dum+m_WIDTH+1)*(a)*(b));
				}
			}
	}

	return 0;
}

int CROIControl::NV_MAPImage_USER(LPBYTE fm, int pitch, TPoint *pt, int nAngle, LPBYTE fmD, int nWidth, int nHeight, int nLimitX, int nLimitY, int nTrans)
{
	int i, j;
	int M, N;
	BYTE nTmp;
	int nOffsetX, nOffsetY;
	double i_, j_, a, b;
	double cosT, sinT;
	double cx, cy, CRx, CRy;
	LPBYTE Dum;

	if(nTrans==1)
	{
		i=nWidth;
		nWidth=nHeight;
		nHeight=i;
	}

	cosT=cos(nAngle*ROI_PI/180.0);
	sinT=sin(nAngle*ROI_PI/180.0);
	cx=nWidth*0.5;
	cy=nHeight*0.5;
	CRx=(pt[0].x+pt[1].x+pt[2].x+pt[3].x)*0.25;
	CRy=(pt[0].y+pt[1].y+pt[2].y+pt[3].y)*0.25;

	for(i=0;i<nHeight;i++)
		for(j=0;j<nWidth;j++)
		{
			j_=cosT*(j-cx)-sinT*(i-cy)+CRx;	
			i_=sinT*(j-cx)+cosT*(i-cy)+CRy;	
			N=(int)j_;
			M=(int)i_;
			a=j_-N;
			b=i_-M;

			if(N<=0 || N>=m_WIDTH || M<=0 ||M>=m_HEIGHT)
				*(fmD+nWidth*i+j)=0;
			else
			{
				Dum=fm+M*m_WIDTH+N;
				
				nTmp=(BYTE)(*(Dum)*(1.0-a)*(1.0-b)+
											*(Dum+1)*(a)*(1.0-b)+
											*(Dum+pitch)*(1.0-a)*(b)+
											*(Dum+pitch+1)*(a)*(b));
				if(nTrans==1) *(fmD+nHeight*j+nHeight-1-i)=nTmp;
				else		  *(fmD+nWidth*i+j)=nTmp;
			}
		}

	return 0;
}
/*
int CROIControl::NV_MAPImage_USER(LPBYTE fm, int pitch, TPoint *pt, int nAngle, LPBYTE fmD, int nWidth, int nHeight, int nLimitX, int nLimitY, int nTrans)
{
	int i, j;
	int M, N;
	int nOffsetX, nOffsetY;
	double i_, j_, a, b;
	double cosT, sinT;
	double cx, cy, CRx, CRy;
	LPBYTE Dum;
	TPoint ptNew[4];
	
	if(nTrans==1)
	{
		ptNew[0].x=pt[3].x;
		ptNew[0].y=pt[3].y;
		ptNew[1].x=pt[0].x;
		ptNew[1].y=pt[0].y;
		ptNew[2].x=pt[1].x;
		ptNew[2].y=pt[1].y;
		ptNew[3].x=pt[2].x;
		ptNew[3].y=pt[2].y;
	}
	else
	{
		for(i=0;i<4;i++)
		{
			ptNew[i].x=pt[i].x;
			ptNew[i].y=pt[i].y;
		}
	}

	cosT=cos(nAngle*ROI_PI/180.0);
	sinT=sin(nAngle*ROI_PI/180.0);
	cx=nWidth*0.5;
	cy=nHeight*0.5;
	CRx=(pt[0].x+pt[1].x+pt[2].x+pt[3].x)*0.25;
	CRy=(pt[0].y+pt[1].y+pt[2].y+pt[3].y)*0.25;

	for(i=0;i<nHeight;i++)
		for(j=0;j<nWidth;j++)
		{
			j_=cosT*(j-cx)-sinT*(i-cy)+CRx;	
			i_=sinT*(j-cx)+cosT*(i-cy)+CRy;	
			N=(int)j_;
			M=(int)i_;
			a=j_-N;
			b=i_-M;

			if(N<=0 || N>=m_WIDTH || M<=0 ||M>=m_HEIGHT)
				*(fmD+nWidth*i+j)=0;
			else
			{
				Dum=fm+M*m_WIDTH+N;
				*(fmD+nWidth*i+j)=(BYTE)(*(Dum)*(1.0-a)*(1.0-b)+
											*(Dum+1)*(a)*(1.0-b)+
											*(Dum+pitch)*(1.0-a)*(b)+
											*(Dum+pitch+1)*(a)*(b));
			}
		}
	

	return 0;
}
*/

void CROIControl::NV_GetOriginalPos(KROI cROI, double x, double y, double *newx, double *newy)
{
	double cosT, sinT;
	double cx, cy, CRx, CRy;

	cosT=cos(cROI.nAngle*ROI_PI/180.0);
	sinT=sin(cROI.nAngle*ROI_PI/180.0);
	cx=(cROI.rect.right-cROI.rect.left)*0.5;
	cy=(cROI.rect.bottom-cROI.rect.top)*0.5;
	CRx=(cROI.rect.right+cROI.rect.left)*0.5;
	CRy=(cROI.rect.bottom+cROI.rect.top)*0.5;

	*newx=cosT*(x-cx)-sinT*(y-cy)+CRx;	
	*newy=sinT*(x-cx)+cosT*(y-cy)+CRy;	
}
void CROIControl::NV_GetOriginalPos_USER(TPoint *pt, int X0, int Y0, int nWidth, int nHeight,  int nAngle, int nTrans, double x, double y, double *newx, double *newy)
{
	double cosT, sinT;
	double cx, cy, CRx, CRy;
	double dNx, dNy;

	if(nTrans==1)
	{
		dNx=nWidth;
		nWidth=nHeight;
		nHeight=dNx;
	}

	if(nTrans)
	{
		dNx=y;
		dNy=nHeight-1-x;
	}
	else
	{
		dNx=x;
		dNy=y;
	}

	if(nAngle==0)
	{
		*newx=X0+dNx;
		*newy=Y0+dNy;
	}
	else
	{
		cosT=cos(nAngle*ROI_PI/180.0);
		sinT=sin(nAngle*ROI_PI/180.0);
		cx=nWidth*0.5;
		cy=nHeight*0.5;
		CRx=(pt[0].x+pt[1].x+pt[2].x+pt[3].x)*0.25;
		CRy=(pt[0].y+pt[1].y+pt[2].y+pt[3].y)*0.25;

		*newx=cosT*(dNx-cx)-sinT*(dNy-cy)+CRx;	
		*newy=sinT*(dNx-cx)+cosT*(dNy-cy)+CRy;	
	}
	
}
// 사각형안의 한 점과 사각형과의 가장가까운 길이를 구한다.
//y=ax+b
int CROIControl::GetMinLength(int nNum, CPoint pt)
{
	RECT rc;
	int i;
	int dx, dy, MinLength=1000;
	int dist;
	double a,b;

	if(m_Tracker[nNum].nAngle==0)
	{
		rc=m_Tracker[nNum].rect;
		dx=( (rc.right-pt.x) < (pt.x-rc.left) )? (rc.right-pt.x): (pt.x-rc.left);
		dy=( (rc.bottom-pt.y) < (pt.y-rc.top) )? (rc.bottom-pt.y):(pt.y-rc.top);
		MinLength=(dx<dy)? dx: dy;
	}
	else if(m_Tracker[nNum].nAngle==90 || m_Tracker[nNum].nAngle==-90)
	{
		dx=( (fabs(m_Tracker[nNum].pt[0].x-pt.x)) < (fabs(m_Tracker[nNum].pt[2].x-pt.x)) )? 
			 (fabs(m_Tracker[nNum].pt[0].x-pt.x)) : (fabs(m_Tracker[nNum].pt[2].x-pt.x));
		dy=( (fabs(m_Tracker[nNum].pt[0].y-pt.y)) < (fabs(m_Tracker[nNum].pt[2].y-pt.y)) )? 
			 (fabs(m_Tracker[nNum].pt[0].y-pt.y)) : (fabs(m_Tracker[nNum].pt[2].y-pt.y));
		MinLength=(dx<dy)? dx: dy;
	}
	else
	{	
		for(i=0;i<3;i++)
		{
			a=(m_Tracker[nNum].pt[i+1].y-m_Tracker[nNum].pt[i].y)/
			  (m_Tracker[nNum].pt[i+1].x-m_Tracker[nNum].pt[i].x);
			b=m_Tracker[nNum].pt[i].y-a*m_Tracker[nNum].pt[i].x;
			dist=abs(a*pt.x+b-pt.y)/sqrt(a*a+1);
			if(dist<MinLength)MinLength=dist;
		}			
		a=(m_Tracker[nNum].pt[0].y-m_Tracker[nNum].pt[3].y)/
		  (m_Tracker[nNum].pt[0].x-m_Tracker[nNum].pt[3].x);
		b=m_Tracker[nNum].pt[3].y-a*m_Tracker[nNum].pt[3].x;
		dist=abs(a*pt.x+b-pt.y)/sqrt(a*a+1);
		if(dist<MinLength)MinLength=dist;
	}
	return MinLength;
}
