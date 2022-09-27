#include "stdafx.h"
#include "patternmatch.h"
#include "math.h"

CPatternMatch m_KOMatch;

//생성자 
CPatternMatch::CPatternMatch()
{
	int i;
	for(i=0;i<4;i++)
	{
		m_fmTemp[i]=NULL;
		m_fmSearch[i]=NULL;
    }
	// Look Up Table 생성 
	for(i=0;i<512;i++)
	      m_LUT[i]=i*i;
 

	m_dThreshold=0.5;
	m_nSkip=2;
	m_nCandiNumber=5;
	m_SetSubPixel=true;
	m_dAcceptCorrValue=0.99;
}
//소멸자 : 메모리를 잡은게 있으면 여기서 풀어준다.
CPatternMatch::~CPatternMatch()
{
	int i;
	if(m_fmTemp[0]!=NULL)
	   for(i=0;i<4;i++)
		  delete m_fmTemp[i];
	if(m_fmSearch[1]!=NULL)
	   for(i=1;i<4;i++)
		  delete m_fmSearch[i];
}

  
//패턴매칭데이타 
void CPatternMatch::SetMatchingData(double limitCorrelation, int skip)
{
	 const double MINCORVALUE=0.3;

	 if( limitCorrelation <0 || limitCorrelation >1 ||
		 skip <0 || skip >3 )
     {
	     m_dThreshold=MINCORVALUE;
		 skip        =0 ;
     }
	 else
     {
	    m_dThreshold = limitCorrelation;
		m_nSkip      = skip+1;
     }
	 m_SetSubPixel=FALSE;

}
//패턴매칭데이타 : SubPixel하려면 여기서 SetSubPixel를 TRUE로 둔다. 
void CPatternMatch::SetMatchingData(double limitCorrelation, int skip, BOOL SetSubPixel,	
					int nCandiNumber,	double dAcceptCorrValue)
{
	 // 사용자가 입력을 제대로 못했을때 아래와 같은 Default값으로 대체한다.
     const int nMAXCANDIDATENUMBER=50; // 최대 후보의 수 
	 const int MAXSKIPPIXEL=3;
	 const double MINCORVALUE=0.3;
	 const double ACCEPTCORVALUE=0.9;

	 // 찾을 Minimum Corelation 설정
	 if( limitCorrelation <0 || limitCorrelation >1 ) m_dThreshold=MINCORVALUE;
	 else 											  m_dThreshold=limitCorrelation;

	 // Skip할 Pixel의 수 설정
     if(skip <0 || skip >MAXSKIPPIXEL )  m_nSkip  = 1 ;
	 else                                m_nSkip  = skip+1;

	 // 찾을 후보의 수 설정 
	 if(nCandiNumber>nMAXCANDIDATENUMBER) m_nCandiNumber=nMAXCANDIDATENUMBER;
	 else if(nCandiNumber<0)             m_nCandiNumber=1;
	 else                                m_nCandiNumber=nCandiNumber;

	 // SubPixel 검사할건지를 세팅 TRUE:검사 FALSE:검사안함.
	 m_SetSubPixel=SetSubPixel;

	 // 후보가운데 가장 높은 Corelation부터 차례로 검사해서 Accept값 이상이 나오면 여기서 
	 // 검사를 Stop한다.
	 if(dAcceptCorrValue >MINCORVALUE && dAcceptCorrValue<=1 ) 
		       m_dAcceptCorrValue = dAcceptCorrValue;
	 else 	   m_dAcceptCorrValue = ACCEPTCORVALUE;

}
// fmS         : 전체 영상의 시작번지
// x1,y1,x2,y2 : 티칭할  Area (x1,y1)-(x2,y2)
// WIDTH_MEM   : 티칭할 영역이 있는 전체영상의 폭( 만약 전체 영역을 모두 티칭하면 
//               WIDTH_MEM은 (x2-x1+1)이 될것이다.
// return 0: 정상 1:불량
int CPatternMatch::TeachTemplate(LPBYTE fmS, int x1, int y1, int x2, int y2, int WIDTH_MEM)
{
	int i,j,dum;
	LPBYTE fm;
        
	if(x1>x2) { dum=x1; x1=x2; x2=dum; }
	if(y1>y2) { dum=y1; y1=y2; y2=dum; }

    if( (x2-x1+1)<PM_MIN_SIZE || (y2-y1+1) <PM_MIN_SIZE ) return 1;

    // Template의 Width 와 Height
    m_TempWidth[0] =x2-x1+1;                        // 1     예) 64     66    
    m_TempWidth[1] =(int)(m_TempWidth[0]/2.0-0.5);  // 1/2       31     32    
    m_TempWidth[2] =(int)(m_TempWidth[1]/2.0-0.5);  // 1/4       15     15   
    m_TempWidth[3] =(int)(m_TempWidth[2]/2.0-0.5);  // 1/8        7      7

    m_TempHeight[0]=y2-y1+1;						// 1         65     67
    m_TempHeight[1]=(int)(m_TempHeight[0]/2.0-0.5); // 1/2       32     33
    m_TempHeight[2]=(int)(m_TempHeight[1]/2.0-0.5); // 1/4       15     16
    m_TempHeight[3]=(int)(m_TempHeight[2]/2.0-0.5); // 1/8        7      7 
 

	if(m_fmTemp[0]!=NULL)
		for(i=0;i<4;i++)
		   delete m_fmTemp[i];
    
    for(i=0;i<4;i++)
		m_fmTemp[i]=new BYTE[m_TempWidth[i]*m_TempHeight[i]];
		          
	// Template와 같은 영상을 m_fmTemp[0]에 저장
	fm = m_fmTemp[0];
	for(i=y1;i<=y2;i++)
	    for(j=x1;j<=x2;j++)
		    *fm++=*(fmS+i*WIDTH_MEM+j);

	// Template를 1/2, 1/4, 1/8로 축소한다
	for(i=1;i<4;i++)
	   MakePyramid(m_fmTemp[i-1],m_fmTemp[i],m_TempWidth[i-1],m_TempHeight[i-1]);

	return 0;
}

// fmS : 전체 영상의 시작번지
// x1,y1,x2,y2 : 검사 Area
// WIDTH_MEM : 전체영상의 폭 
void CPatternMatch::SetSearchArea(LPBYTE fmS, int x1, int y1, int x2, int y2, int WIDTH_MEM)
{
	int i,dum;
        
	if(x1>x2) { dum=x1; x1=x2; x2=dum; }
	if(y1>y2) { dum=y1; y1=y2; y2=dum; }

    // SearchArea의 Width 와 Height 설정 
    // [0]는 따로 메모리를 잡는게 아니라 원래 이미지를 그대로 쓴다.(즉 Width=WIDTHMEM, Height=y2+1)
    m_SearchWidth[0] =x2-x1+1;                          // 1     예) 64     66    
    m_SearchWidth[1] =(int)(m_SearchWidth[0]/2.0-0.5);  // 1/2       31     32    
    m_SearchWidth[2] =(int)(m_SearchWidth[1]/2.0-0.5);  // 1/4       15     15   
    m_SearchWidth[3] =(int)(m_SearchWidth[2]/2.0-0.5);  // 1/8        7      7

    m_SearchHeight[0]=y2-y1+1;							// 1         65     67
    m_SearchHeight[1]=(int)(m_SearchHeight[0]/2.0-0.5); // 1/2       32     33
    m_SearchHeight[2]=(int)(m_SearchHeight[1]/2.0-0.5); // 1/4       15     16
    m_SearchHeight[3]=(int)(m_SearchHeight[2]/2.0-0.5); // 1/8        7      7 
 

	// 만약 기존에 메모리 할당받았으면 여기서 해제한다.
	if(m_fmSearch[1]!=NULL)
		for(i=1;i<4;i++)
		   delete m_fmSearch[i];
    
	// 새로운 메모리를 할당 받는다.
    for(i=1;i<4;i++)
		m_fmSearch[i]=new BYTE[m_SearchWidth[i]*m_SearchHeight[i]];
		          
	// 원 영상의 서치영역시작번지를 m_fmSearch[0]에 저장
	m_fmSearch[0]=fmS+WIDTH_MEM*y1+x1;

	// Searching Area를 1/2, 1/4, 1/8로 축소한다	
	MakePyramidForSearchArea(m_fmSearch[0],m_fmSearch[1],m_SearchWidth[0],m_SearchHeight[0],WIDTH_MEM);
	MakePyramid(m_fmSearch[1],m_fmSearch[2],m_SearchWidth[1],m_SearchHeight[1]);
	MakePyramid(m_fmSearch[2],m_fmSearch[3],m_SearchWidth[2],m_SearchHeight[2]);

}

// 실제로 매칭을 수행하는 함수 이다.
// fmS : 전체 영상의 시작번지
// x1,y1,x2,y2 : 검사 Area
// WIDTH_MEM : 전체영상의 폭 
// return 0: 정상 1,2:불량
// 계산이 끝나고 나서 m_dCorrX,m_dCorrY,m_dCorrValue 를 읽으면 된다.
int CPatternMatch::Matching(LPBYTE fmS, int x1, int y1, int x2, int y2, int WIDTH_MEM)
{
    if(m_fmTemp[0]==NULL) return 1;
	const int SIZEFORLEVEL4 =60;			// Template의 크기가 60초과면 피라밋 레벨을 4(압축3회)로한다 
    const int SIZEFORLEVEL3 =30;			// Template의 크기가 30초과면 피라밋 레벨을 3(압축2회)로한다 
    const int SIZEFORLEVEL2 =PM_MIN_SIZE-1; // Template의 크기가 15초과면 피라밋 레벨을 2(압축1회)로한다 


    int  i,dum;
	int  SearchLevel;
	double dMaxCorrValue, dPrevCorrValue;
	int nMaxCorrValuePos;
	
	if(x1>x2) { dum=x1; x1=x2; x2=dum; }
	if(y1>y2) { dum=y1; y1=y2; y2=dum; }
	
    SetSearchArea(fmS, x1, y1, x2, y2, WIDTH_MEM);

	if( m_TempWidth[0]> SIZEFORLEVEL4 && m_TempHeight[0]> SIZEFORLEVEL4)      SearchLevel=4;
	else if( m_TempWidth[0]> SIZEFORLEVEL3 && m_TempHeight[0]> SIZEFORLEVEL3) SearchLevel=3;
	else if( m_TempWidth[0]> SIZEFORLEVEL2 && m_TempHeight[0]> SIZEFORLEVEL2) SearchLevel=2;
	else  												return 1;

	double (*CandiData)[3]=new double[m_nCandiNumber][3];

	//단계별 매칭
	switch(SearchLevel)
	{
	case 4:  if(MatchingSubForTop(3,CandiData))  goto END;
		     for(i=0;i<m_nFoundCandiNumber;i++)
			 {
				dPrevCorrValue=CandiData[i][2];
				CandiData[i][2]=0;
                if(MatchingSub(2,&CandiData[i][0],&CandiData[i][1],&CandiData[i][2]))continue;
				CandiData[i][2]=0;
                if(MatchingSub(1,&CandiData[i][0],&CandiData[i][1],&CandiData[i][2]))continue;                  
				CandiData[i][2]=0;
                if(MatchingForBottom(0, WIDTH_MEM,&CandiData[i][0],&CandiData[i][1],&CandiData[i][2]))continue;
                // 구한 Correlation 값이 m_dAcceptCorrValue보다 크면 여기서 STOP한다 
                if(CandiData[i][2]>m_dAcceptCorrValue)
				{   
					m_nFoundCandiNumber=i+1;
					break;
				}
			 }
			 break;
	case 3:  if(MatchingSubForTop(2,CandiData))  goto END;
		     for(i=0;i<m_nFoundCandiNumber;i++)
			 {
				dPrevCorrValue=CandiData[i][2];
				CandiData[i][2]=0;
                if(MatchingSub(1,&CandiData[i][0],&CandiData[i][1],&CandiData[i][2]))continue;
				CandiData[i][2]=0;
                if(MatchingForBottom(0, WIDTH_MEM,&CandiData[i][0],&CandiData[i][1],&CandiData[i][2]))continue;
                // 구한 Correlation 값이 m_dAcceptCorrValue보다 크면 여기서 STOP한다 
                if(CandiData[i][2]>m_dAcceptCorrValue)
				{   
					m_nFoundCandiNumber=i+1;
					break;
				}
			 }
			 break;
	case 2:  if(MatchingSubForTop(1,CandiData))  goto END;
		     for(i=0;i<m_nFoundCandiNumber;i++)
			 {
				CandiData[i][2]=0;
                if(MatchingForBottom(0, WIDTH_MEM,&CandiData[i][0],&CandiData[i][1],&CandiData[i][2]))continue;
                // 구한 Correlation 값이 m_dAcceptCorrValue보다 크면 여기서 STOP한다 
                if(CandiData[i][2]>m_dAcceptCorrValue)
				{   
					m_nFoundCandiNumber=i+1;
					break;
				}
			 }
			 break;
    }

	// BEGIN-0 : 모든 후보에 대한 Corelation 값이 구해졌으면 이중에 가장 높은 값을 
	//           구하고자 하는 값으로 결정짓는다. >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	dMaxCorrValue=CandiData[0][2];
	nMaxCorrValuePos=0;
	for(i=1;i<m_nFoundCandiNumber;i++)
	   if(CandiData[i][2]>dMaxCorrValue)
	   {
		   dMaxCorrValue=CandiData[i][2];
		   nMaxCorrValuePos=i;
	   }

	m_dCorrValue=dMaxCorrValue;
	m_dCorrX=CandiData[nMaxCorrValuePos][0];
	m_dCorrY=CandiData[nMaxCorrValuePos][1];

	if(m_dCorrValue <=0 ) goto END;
	//END-0 : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	// SubPixel 패턴매칭 처리를 한다(아래 한줄 )//////////////////////////////////
	if(m_SetSubPixel && m_SearchWidth[0]>m_TempWidth[0]+2 && m_SearchHeight[0]>m_TempHeight[0]+2)
	{
		MatchingForSubPixel(WIDTH_MEM);
	}

    m_dCorrX += x1;
    m_dCorrY += y1;

	delete [] CandiData;
	return 0;

END:

	m_dCorrValue=0;
	delete [] CandiData;
	return 2;
}

// 여러개를 구하는 함수 이다. 
// fmS : 전체 영상의 시작번지
// x1,y1,x2,y2 : 검사 Area
// WIDTH_MEM : 전체영상의 폭 
// return   >0:구한 패턴 수 
//          -1:불량
int CPatternMatch::MatchingMulti(LPBYTE fmS, int x1, int y1, int x2, int y2, int WIDTH_MEM)//, double *corrx, double *corry, double *corr)
{
    if(m_fmTemp[0]==NULL) return -1;
	const int SIZEFORLEVEL4 =60;			// Template의 크기가 60초과면 피라밋 레벨을 4(압축3회)로한다 
    const int SIZEFORLEVEL3 =30;			// Template의 크기가 30초과면 피라밋 레벨을 3(압축2회)로한다 
    const int SIZEFORLEVEL2 =PM_MIN_SIZE-1;   // Template의 크기가 15초과면 피라밋 레벨을 2(압축1회)로한다 


    int  i,dum;
	int  SearchLevel;
	double dPrevCorrValue;
	int corrcount=0;
	
	if(x1>x2) { dum=x1; x1=x2; x2=dum; }
	if(y1>y2) { dum=y1; y1=y2; y2=dum; }
	
    SetSearchArea(fmS, x1, y1, x2, y2, WIDTH_MEM);

	if( m_TempWidth[0]> SIZEFORLEVEL4 && m_TempHeight[0]> SIZEFORLEVEL4)      SearchLevel=4;
	else if( m_TempWidth[0]> SIZEFORLEVEL3 && m_TempHeight[0]> SIZEFORLEVEL3) SearchLevel=3;
	else if( m_TempWidth[0]> SIZEFORLEVEL2 && m_TempHeight[0]> SIZEFORLEVEL2) SearchLevel=2;
	else  												return -1;

	double (*CandiData)[3]=new double[m_nCandiNumber][3];

	//단계별 매칭
	switch(SearchLevel)
	{
	case 4:  if(MatchingSubForTop(3,CandiData))  goto END;
		     for(i=0;i<m_nFoundCandiNumber;i++)
			 {
				dPrevCorrValue=CandiData[i][2];
				CandiData[i][2]=0;
                if(MatchingSub(2,&CandiData[i][0],&CandiData[i][1],&CandiData[i][2]))continue;
				CandiData[i][2]=0;
                if(MatchingSub(1,&CandiData[i][0],&CandiData[i][1],&CandiData[i][2]))continue;                  
				CandiData[i][2]=0;
                if(MatchingForBottom(0, WIDTH_MEM,&CandiData[i][0],&CandiData[i][1],&CandiData[i][2]))continue;
                // 구한 Correlation 값이 m_dThreshold 크면 저장한다. 
				if(CandiData[i][2]>m_dThreshold)
                {
					m_dCorrXM[corrcount]=CandiData[i][0];
					m_dCorrYM[corrcount]=CandiData[i][1];
					m_dCorrValueM[corrcount] =CandiData[i][2];
					corrcount++;					
				}
			 }
			 break;
	case 3:  if(MatchingSubForTop(2,CandiData))  goto END;
		     for(i=0;i<m_nFoundCandiNumber;i++)
			 {
				dPrevCorrValue=CandiData[i][2];
				CandiData[i][2]=0;
                if(MatchingSub(1,&CandiData[i][0],&CandiData[i][1],&CandiData[i][2]))continue;
				CandiData[i][2]=0;
                if(MatchingForBottom(0, WIDTH_MEM,&CandiData[i][0],&CandiData[i][1],&CandiData[i][2]))continue;
                // 구한 Correlation 값이 m_dThreshold 크면 저장한다. 
				if(CandiData[i][2]>m_dThreshold)
                {
					m_dCorrXM[corrcount]=CandiData[i][0];
					m_dCorrYM[corrcount]=CandiData[i][1];
					m_dCorrValueM[corrcount] =CandiData[i][2];
					corrcount++;					
				}
			 }
			 break;
	case 2:  if(MatchingSubForTop(1,CandiData))  goto END;
		     for(i=0;i<m_nFoundCandiNumber;i++)
			 {
				CandiData[i][2]=0;
                if(MatchingForBottom(0, WIDTH_MEM,&CandiData[i][0],&CandiData[i][1],&CandiData[i][2]))continue;
                // 구한 Correlation 값이 m_dThreshold 크면 저장한다. 
				if(CandiData[i][2]>m_dThreshold)
                {
					m_dCorrXM[corrcount]=CandiData[i][0];
					m_dCorrYM[corrcount]=CandiData[i][1];
					m_dCorrValueM[corrcount] =CandiData[i][2];
					corrcount++;					
				}
			 }
			 break;
    }

	if(m_SetSubPixel)
       for(i=0;i<corrcount;i++)
       {
			m_dCorrValue=m_dCorrValueM[i];
			m_dCorrX=m_dCorrXM[i];
			m_dCorrY=m_dCorrYM[i];
            MatchingForSubPixel(WIDTH_MEM);
            m_dCorrXM[i]=m_dCorrX+x1;
			m_dCorrYM[i]=m_dCorrY+y1;
       }
    else
       for(i=0;i<corrcount;i++)
       {
            m_dCorrXM[i]+=x1;
			m_dCorrYM[i]+=y1;
       }

   
	delete [] CandiData;
	return corrcount;

END:

	m_dCorrValue=0;
	delete [] CandiData;
	return -2;
}

//피라밋의 맨 위쪽에서 매칭을 수행한다.
int CPatternMatch::MatchingSubForTop(int N, double CandiData[][3])
{	
	int i,j,k,l,ii, dum;
	int FindLocalPeak;
	LPBYTE fmT, fmS;
	int nTempWidth,nTempHeight;
	int nSearchWidth,nSearchHeight;

    int tempT, tempS;
	LPBYTE fmT_, fmS_, fmS__;
	/////////////////// Correration Data ///////////////////////////////
	double Area, Sig_Ti, Sig_Ti2;
	double       Sig_Si, Sig_Si2;
	double       Sig_TiSi;
	double dDiv;
	double dCorrData;
    int Iter_Number=0;

	double dThreshold=m_dThreshold;
	double dMinCorrValue;
	int    nMinCorrPos;

	//KO 속도 증가를 위해 사용했던 데이타 또 사용하기 위해서 추가 하는 변수들
	double Sig_Si_Org, Sig_Si2_Org, Sig_Si_New, Sig_Si2_New;
	double Sig_Si_Left, Sig_Si2_Left, Sig_Si_Right, Sig_Si2_Right;
	double Sig_Si_Top, Sig_Si2_Top, Sig_Si_Bottom, Sig_Si2_Bottom;
	int Y_Data,End_X;
	


    fmT        = m_fmTemp[N];
	nTempWidth =m_TempWidth[N];
	nTempHeight=m_TempHeight[N];

	fmS          = m_fmSearch[N];
	nSearchWidth =m_SearchWidth[N];
	nSearchHeight=m_SearchHeight[N];

    //BEGIN-0: Memory Allocation>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	int  Corr_N=nSearchWidth-nTempWidth+5, Corr_M=nSearchHeight-nTempHeight+5;
	double **pCorrMap;
	pCorrMap= (double **)malloc( Corr_N*sizeof(double *));
	if(!pCorrMap) return 1;
	for(i=0;i<Corr_N;i++)
		pCorrMap[i]=(double *)malloc(Corr_M*sizeof(double));
	//END-0:<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			
	//BEGIN-0 : Template데이타 구하기 >>>>>>>
    Sig_Ti = Sig_Ti2  =0;
	Area = nTempWidth*nTempHeight;
	for(i=0;i<nTempHeight;i++) 
	 {
		fmT_ = fmT + i*nTempWidth;
		for(j=0;j<nTempWidth ;j++) 
		{
		   tempT    = *fmT_++;
		   Sig_Ti  += tempT;
		   Sig_Ti2 += m_LUT[tempT];
		}
	 }
	//END-0 : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    //BEGIN-0: 이전 데이타를 이용하기 위해서 이렇게 했음>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


	//BEGIN-0: 새로운 방법 시작 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	for(k=0; k <=nSearchHeight-nTempHeight ; k++) 
	{
	   for(l=0; l<=nSearchWidth-nTempWidth ; l++) 
	   {
          Sig_Si = Sig_Si2  = Sig_TiSi =0;
		  fmS_ = fmS + nSearchWidth*k+l;

		  if(k==0 && l==0) // 맨 처음인 경우
		  {									//
			 Sig_Si_Left=Sig_Si2_Left=0;	//
			 Sig_Si_Top=Sig_Si2_Top=0;		//
			 
			 for(i=0;i< nTempHeight ;i++) 
			 {
			    fmT_  = fmT + nTempWidth*i;
			    fmS__ = fmS_+ nSearchWidth*i;

				Sig_Si_Left+=*(fmS__);					//
				Sig_Si2_Left+=m_LUT[*(fmS__)];			//
				if(i==0)								//
				{										//
			        for(j=0 ;j< nTempWidth ;j++)		//
					{									//
						tempS   = *(fmS__+j);			//
						Sig_Si_Top +=tempS;				//
						Sig_Si2_Top+=m_LUT[tempS];		//
					}									//
				}										//
				for(j=0 ;j< nTempWidth ;j++) 	
				{
					tempS   = *(fmS__+j);
					Sig_Si  += tempS;
					Sig_Si2 += m_LUT[tempS];
					Sig_TiSi+= m_LUT[tempS+*(fmT_+j)];
				}
		     }											//
			 Sig_Si_New=Sig_Si_Org= Sig_Si;				//
			 Sig_Si2_New=Sig_Si2_Org=Sig_Si2;			//
			 Y_Data     =k;								//
		  }												//
		  else if(k==Y_Data)  // 같은 라인에 대해서 반복되는 루틴
		  {					  // Right를 더해주고 Left를 빼준다.

			 Sig_Si =Sig_Si_New -Sig_Si_Left;  Sig_Si2=Sig_Si2_New-Sig_Si2_Left;

			 Sig_Si_Right=Sig_Si2_Right=0;	  // Right초기화
			 Sig_Si_Left =Sig_Si2_Left =0;	  // Left 초기화 

			 End_X=	nTempWidth-1;
			 for(i=0;i< nTempHeight ;i++) 
			 {
			    fmT_  = fmT + nTempWidth*i;
			    fmS__ = fmS_+ nSearchWidth*i;

				Sig_Si_Left+=*(fmS__);          // Left 구한다.   
				Sig_Si2_Left+=m_LUT[*(fmS__)];	//
				Sig_Si_Right+=*(fmS__+End_X); 		 // Right 구한다.
				Sig_Si2_Right+=m_LUT[*(fmS__+End_X)];//

				for(j=0 ;j< nTempWidth ;j++) 
			    {
				   Sig_TiSi+= m_LUT[*(fmS__+j)+*(fmT_+j)];
			    }
		     }

			 Sig_Si_New = Sig_Si = Sig_Si + Sig_Si_Right;
			 Sig_Si2_New= Sig_Si2= Sig_Si2+ Sig_Si2_Right;
			 Y_Data     =k;					  //
		  }
		  else 	 // 새로운 라인이 시작될때 Top을 빼주고 Bottom을 더해준다. 
		  {		 // 다음을 위해서 새로운 Top과 Left를 구해둔다.
			 Sig_Si =Sig_Si_Org -Sig_Si_Top;  Sig_Si2=Sig_Si2_Org-Sig_Si2_Top;

			 Sig_Si_Left  =Sig_Si2_Left  =0;	  //  Left를 초기화
			 Sig_Si_Top   =Sig_Si2_Top   =0;      //  Top을 초기화 
			 Sig_Si_Bottom=Sig_Si2_Bottom=0;      //  Bottom을 초기화 


			 for(i=0;i< nTempHeight ;i++) 
			 {
			    fmT_  = fmT + nTempWidth*i;
			    fmS__ = fmS_+ nSearchWidth*i;

				Sig_Si_Left+=*(fmS__);  		// Left구한다.
				Sig_Si2_Left+=m_LUT[*(fmS__)];	//

				if(i==0)								//
				{										//
			        for(j=0 ;j< nTempWidth ;j++)		//
					{									//
						tempS   = *(fmS__+j);			//	   Top 구함
						Sig_Si_Top +=tempS;				//
						Sig_Si2_Top+=m_LUT[tempS];		//
					}									//
				}										//
				else if((i+1)>=nTempHeight)				//
				{										//
			        for(j=0 ;j< nTempWidth ;j++)		//
					{									//
						tempS   = *(fmS__+j);			//	   Bottom 구함 
						Sig_Si_Bottom +=tempS;			//
						Sig_Si2_Bottom+=m_LUT[tempS];	//
					}									//
			    }
				for(j=0 ;j< nTempWidth ;j++) 
			    {
				   Sig_TiSi+= m_LUT[*(fmS__+j)+*(fmT_+j)];
			    }
		     }

			 Sig_Si_Org=Sig_Si_New = Sig_Si = Sig_Si + Sig_Si_Bottom;   //
			 Sig_Si2_Org=Sig_Si2_New= Sig_Si2= Sig_Si2+ Sig_Si2_Bottom;	//
			 Y_Data     =k;												//
		  }
		  //END-0:새로운 방법 여기까지  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



		  dDiv = sqrt( ( Area*Sig_Si2 - Sig_Si*Sig_Si)*(Area*Sig_Ti2-Sig_Ti*Sig_Ti) );		  
		  if (dDiv > 0.0001 ) dCorrData = ( Area*(Sig_TiSi-Sig_Ti2-Sig_Si2)/2 - Sig_Ti*Sig_Si)/dDiv;
		  else                dCorrData = 0.0;

		  pCorrMap[l+2][k+2]=dCorrData;	 //각각 2를 더한 이유는 local max구할때 알고리즘을 
		                                 //간단하게 하기 위해서 
	   }
	}
    // END-0:<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	//BEGIN-0: Correlation Map으로 부터 후보점들을 찾는다. >>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	//BEGIN-1: Correlaton Map 주위 각 2픽셀씩을 0으로 채운다. >>>>>>>>>>>>>>
	dum=nSearchHeight-nTempHeight+5-2;
	if(dum<0) return 1;
	for(l=0; l<nSearchWidth-nTempWidth+5 ; l++) 
		pCorrMap[l][0]=pCorrMap[l][1]=pCorrMap[l][dum]=pCorrMap[l][dum+1]=0;

	dum=nSearchWidth-nTempWidth+5-2;
	if(dum<0) return 1;
	for(k=0; k <nSearchHeight-nTempHeight+5 ; k++) 
		pCorrMap[0][k]=pCorrMap[1][k]=pCorrMap[dum][k]=pCorrMap[dum+1][k]=0;
	//END-1: <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	// BEGIN-1: Local Max를 찾아서 후보데이타를 구한다. >>>>>>>>>>>>>>>>>>>>
	for(k=2; k <=nSearchHeight-nTempHeight+2 ; k++) 
	{
	   for(l=2; l<=nSearchWidth-nTempWidth+2 ; l++) 
	   {
		  if(pCorrMap[l][k] >= dThreshold)	//일단 이점이 기본값 이상이어야 한다.		 
		  {
			 FindLocalPeak=1;
			 for(ii=k-2; ii<=k+2; ii++)			    
				if( pCorrMap[l-2][ii] > pCorrMap[l][k] || pCorrMap[l-1][ii] > pCorrMap[l][k] ||
				    pCorrMap[l  ][ii] > pCorrMap[l][k] || pCorrMap[l+1][ii] > pCorrMap[l][k] ||
				    pCorrMap[l+2][ii] > pCorrMap[l][k] )  { FindLocalPeak =0; break; }

			 //BEGIN-2 : 이점이 Local Max이라면 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			 if(FindLocalPeak)
			 {	 
			    if( Iter_Number < m_nCandiNumber) //찾는 데이타가 구하고자하는 수보다 작으면 저장
			    {
			       CandiData[Iter_Number][0]=(double)(l-2);
			       CandiData[Iter_Number][1]=(double)(k-2);
			       CandiData[Iter_Number++][2]=pCorrMap[l][k];
			    }
			    else // 이미 후보데이타보다 많이 찾았다면 후보중 가장 낮은값을 구해서 이를 없애소 
			    {	 // 새로 찾은 값을 저장한다.
				   dMinCorrValue=CandiData[0][2];
				   nMinCorrPos=0;
				   for(ii=1; ii<m_nCandiNumber; ii++)
					  if(	CandiData[ii][2]< dMinCorrValue) 
				      { 
					     dMinCorrValue=CandiData[ii][2];
					     nMinCorrPos=ii;
				      }
				   dThreshold= dMinCorrValue;
				   if( pCorrMap[l][k] >	dMinCorrValue) // 구한값이 기존의 가장작은것 보다 크면 저장
				   {
				      CandiData[nMinCorrPos][0]=(double)(l-2);
			          CandiData[nMinCorrPos][1]=(double)(k-2);
			          CandiData[nMinCorrPos][2]=pCorrMap[l][k];
				   }
			    }
		     } 
			 //END-2<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	      }
       }  
	}
	//END-1: Local Max를 찾아서 후보데이타를 구한다.<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	// 구한값을 소팅한다 >>>>>>>>>>>>>>
     MatchSort(Iter_Number, CandiData);
	 m_nFoundCandiNumber= Iter_Number;


	 for(i=0;i<Corr_N;i++) free(pCorrMap[i]);
 	 free(pCorrMap);
     
	 //END-0 :<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	 if(Iter_Number==0) return 1;
	 else               return 0;
}
// 소팅함수 
int CPatternMatch::MatchSort(int count, double Value[][3])
{
	int i,j;
	double MaxValue;
	int Number;
	double StoreValue, StoreXData, StoreYData;

	   for(i=0;i<count-1;i++)
	   {
		  Number=i;
		  StoreXData=Value[i][0];
		  StoreYData=Value[i][1];
		  MaxValue=StoreValue=Value[i][2];
	      
		  for(j=i+1;j<count;j++)
			 if(Value[j][2] > MaxValue) { MaxValue=Value[j][2]; Number=j; }  
		  
		  Value[i][0]=Value[Number][0];
		  Value[i][1]=Value[Number][1];
		  Value[i][2]=Value[Number][2];;

		  Value[Number][0]=StoreXData;
		  Value[Number][1]=StoreYData;
		  Value[Number][2]=StoreValue;
	   }
	   return 0;
}

// 피라밋의 중간단계에서 매칭을 수행한다.
int CPatternMatch::MatchingSub(int N, double *nCorrX, double *nCorrY, double *CorrValue)
{	
	int i,j,k,l;
	LPBYTE fmT, fmS;
	int nTempWidth,nTempHeight;
	int nSearchWidth,nSearchHeight;

    int tempT, tempS;
	LPBYTE fmT_, fmS_, fmS__;
	/////////////////// Correlation Data ///////////////////////////////
	double Area, Sig_Ti, Sig_Ti2;
	double       Sig_Si, Sig_Si2;
	double       Sig_TiSi;
	double dDiv;
	double dCorrData,dMaxCorrData=0;
    int Iter_Number=0;
    int nLeft, nTop;    //새로 검사할 영역의 좌상귀의 좌표 
	int StartX,EndX, StartY,EndY;

	nLeft = 2*(int)(*nCorrX);
	nTop  = 2*(int)(*nCorrY);

	nTempWidth =m_TempWidth[N];
	nTempHeight=m_TempHeight[N];
    fmT        = m_fmTemp[N];

	nSearchWidth =m_SearchWidth[N];
	nSearchHeight=m_SearchHeight[N];
	fmS          = m_fmSearch[N]+nSearchWidth*nTop+nLeft;

    //BEGIN-0 : 피라밋의 한단계 아래로 내려왔을때 서치 영역을 5x5로 잡기위해서
	//          아래와 같이 한다. 특히 서치영역을 넘어가는것을 방지하기 위해서
	//          If문을 사용했음 
	if(nLeft>0) StartX=-1;
    else        StartX=0;
	if(nTop>0)  StartY=-1;
    else        StartY=0;
    if( (nLeft+nTempWidth+2)<nSearchWidth ) EndX=3;
	else									EndX=nSearchWidth-(nLeft+nTempWidth);
    if( (nTop+nTempHeight+2)<nSearchHeight ) EndY=3;
	else									 EndY=nSearchHeight-(nTop+nTempHeight);
    //END -0 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 

    //BEGIN-0 : Template데이타 구하기 >>>>>>>
    Sig_Ti = Sig_Ti2  =0;
	Area = 0;
	for(i=0;i<nTempHeight;i+=m_nSkip) 
	{
		fmT_ = fmT + i*nTempWidth;
		for(j=0;j<nTempWidth ;j+=m_nSkip) 
		{
		   tempT    = *(fmT_+j);
		   Sig_Ti  += tempT;
		   Sig_Ti2 += m_LUT[tempT];
		   Area++;
		}
	 }
	//END-0 : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    //BEGIN-0: 이전 데이타를 이용하기 위해서 이렇게 했음>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	for(k=StartY; k <= EndY ; k++) 
	{
	   for(l=StartX; l<= EndX ; l++) 
	   {
          Sig_Si = Sig_Si2  = Sig_TiSi =0;
		  fmS_ = fmS + nSearchWidth*k+l;
		  for(i=0;i< nTempHeight ;i+=m_nSkip) 
		  {
			 fmT_  = fmT + nTempWidth*i;
			 fmS__ = fmS_+ nSearchWidth*i;
			 for(j=0 ;j< nTempWidth ;j+=m_nSkip) 
			 {
				tempS   = *(fmS__+j);
				Sig_Si  += tempS;
				Sig_Si2 += m_LUT[tempS];
				Sig_TiSi+= m_LUT[tempS+*(fmT_+j)];
			 }
		  }

		  dDiv = sqrt( ( Area*Sig_Si2 - Sig_Si*Sig_Si)*(Area*Sig_Ti2-Sig_Ti*Sig_Ti) );
		  if (dDiv > 0.0001 ) dCorrData = ( Area*(Sig_TiSi-Sig_Ti2-Sig_Si2)/2 - Sig_Ti*Sig_Si)/dDiv;
		  else                dCorrData = 0.0;


		  // Classification of Match Degree 
		  if(dCorrData >= m_dThreshold) 
		  {
			 Iter_Number++;
			 if(dCorrData > dMaxCorrData) 
			 {
				*nCorrX=(double)(nLeft+l);
				*nCorrY=(double)(nTop +k);
			    *CorrValue=m_dCorrValue=dMaxCorrData=dCorrData;
			 }
		  } // end of if 
	    }
	 }  // end of loop 
	 if(Iter_Number==0) return 1;
	 else               return 0;
}

// 피라밋의 가장 아랫단계에서 매칭을 수행한다.
int CPatternMatch::MatchingForBottom(int N, int WIDTH_MEM, double *nCorrX, double *nCorrY, double *CorrValue)
{	
	int i,j,k,l;
	LPBYTE fmT, fmS;
	int nTempWidth,nTempHeight;
	int nSearchWidth,nSearchHeight;

    int tempT, tempS;
	LPBYTE fmT_, fmS_, fmS__;
	/////////////////// Correlation Data ///////////////////////////////
	double Area, Sig_Ti, Sig_Ti2;
	double       Sig_Si, Sig_Si2;
	double       Sig_TiSi;
	double dDiv;
	double dCorrData,dMaxCorrData=0;
    int Iter_Number=0;
    int nLeft, nTop;    //새로 검사할 영역의 좌상귀의 좌표 
	int StartX,EndX, StartY,EndY;



	nLeft = 2*(int)(*nCorrX);
	nTop  = 2*(int)(*nCorrY);

	nTempWidth =m_TempWidth[N];
	nTempHeight=m_TempHeight[N];
    fmT        =m_fmTemp[N];

	nSearchWidth =m_SearchWidth[N];
	nSearchHeight=m_SearchHeight[N];
	fmS          =m_fmSearch[N]+WIDTH_MEM*nTop+nLeft;

    //BEGIN-0 : 피라밋의 한단계 아래로 내려왔을때 서치 영역을 5x5로 잡기위해서
	//          아래와 같이 한다. 특히 서치영역을 넘어가는것을 방지하기 위해서
	//          If문을 사용했음 
	if(nLeft>0) StartX=-1;
    else        StartX=0;
	if(nTop>0)  StartY=-1;
    else        StartY=0;
    if( (nLeft+nTempWidth+2)<nSearchWidth ) EndX=3;
	else									EndX=nSearchWidth-(nLeft+nTempWidth);
    if( (nTop+nTempHeight+2)<nSearchHeight ) EndY=3;
	else									 EndY=nSearchHeight-(nTop+nTempHeight);
    //END -0 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 

    //매칭 데이타를 100개만을 사용하기 위해서 m_nSkip을 여기서 재 설정한다.
    if(m_nSkip==4) m_nSkip = nTempWidth/10+1;

    //BEGIN-0 : Template데이타 구하기 >>>>>>>
    Sig_Ti = Sig_Ti2  =0;
	Area = 0;
	for(i=0;i<nTempHeight;i+=m_nSkip) 
	{
		fmT_ = fmT + i*nTempWidth;
		for(j=0;j<nTempWidth ;j+=m_nSkip) 
		{
		   tempT    = *(fmT_+j);
		   Sig_Ti  += tempT;
		   Sig_Ti2 += m_LUT[tempT];
		   Area++;
		}
	 }
	//END-0 : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	for(k=StartY; k <= EndY ; k++) 
	{
	   for(l=StartX; l<= EndX ; l++) 
	   {
          Sig_Si = Sig_Si2  = Sig_TiSi =0;
		  fmS_ = fmS + WIDTH_MEM*k+l;
		  for(i=0;i< nTempHeight ;i+=m_nSkip) 
		  {
			 fmT_  = fmT + nTempWidth*i;
			 fmS__ = fmS_+ WIDTH_MEM*i;
			 for(j=0 ;j< nTempWidth ;j+=m_nSkip) 
			 {
				tempS   = *(fmS__+j);
				Sig_Si  += tempS;
				Sig_Si2 += m_LUT[tempS];
				Sig_TiSi+= m_LUT[tempS+*(fmT_+j)];
			 }
		  }

		  dDiv = sqrt( ( Area*Sig_Si2 - Sig_Si*Sig_Si)*(Area*Sig_Ti2-Sig_Ti*Sig_Ti) );
		  if (dDiv > 0.0001 ) dCorrData = ( Area*(Sig_TiSi-Sig_Ti2-Sig_Si2)/2 - Sig_Ti*Sig_Si)/dDiv;
		  else                dCorrData = 0.0;

		  // Classification of Match Degree 
		  if(dCorrData >= m_dThreshold) 
		  {
			 Iter_Number++;
			 if(dCorrData > dMaxCorrData) 
			 {
				*nCorrX=(double)(nLeft+l);
				*nCorrY=(double)(nTop +k);
			    *CorrValue=m_dCorrValue=dMaxCorrData=dCorrData;
			 }
		  } // end of if 
	    }
	 }  // end of loop 

	if(Iter_Number==0) return 1;
	else               return 0;
}

int CPatternMatch::MatchingForSubPixel(int WIDTH_MEM)
{	
	int i,j,k,l;
	LPBYTE fmT, fmS;
	int nTempWidth,nTempHeight;
	int nSearchWidth,nSearchHeight;

    int tempT, tempS;
	LPBYTE fmT_, fmS_, fmS__;
	/////////////////// Correlation Data ///////////////////////////////
	double Area, Sig_Ti, Sig_Ti2;
	double       Sig_Si, Sig_Si2;
	double       Sig_TiSi;
	double dDiv;
	double dCorrData,dMaxCorrData=0;
    int nLeft, nTop;    //새로 검사할 영역의 좌상귀의 좌표 
	// SubPixel을 위해 사용하는 변수들 
	double dCorrX, dCorrY;
	double dCorData[9][3];


	nLeft = (int)m_dCorrX;
	nTop  = (int)m_dCorrY;

	nTempWidth =m_TempWidth[0];
	nTempHeight=m_TempHeight[0];
    fmT        = m_fmTemp[0];

	nSearchWidth =m_SearchWidth[0];
	nSearchHeight=m_SearchHeight[0];
	fmS          = m_fmSearch[0]+WIDTH_MEM*nTop+nLeft;


    //매칭 데이타를 100개만을 사용하기 위해서 m_nSkip을 여기서 재 설정한다.
    if(m_nSkip==4) m_nSkip = nTempWidth/10+1;

    //BEGIN-0 : Template데이타 구하기 >>>>>>>
    Sig_Ti = Sig_Ti2  =0;
	Area = 0;
	for(i=0;i<nTempHeight;i+=m_nSkip) 
	{
		fmT_ = fmT + i*nTempWidth;
		for(j=0;j<nTempWidth ;j+=m_nSkip) 
		{
		   tempT    = *(fmT_+j);
		   Sig_Ti  += tempT;
		   Sig_Ti2 += m_LUT[tempT];
		   Area++;
		}
	 }
	//END-0 : <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//BEGIN-0: SubPixel처리하는 부분  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  int nCount=0;
  {
	if(  m_dCorrX>0 && (m_dCorrX+nTempWidth) < nSearchWidth  && 
		 m_dCorrY>0  && (m_dCorrY+nTempHeight) < nSearchHeight   )
	{
	  for(k=-1;k<=1;k++)
		for(l=-1;l<=1;l++)
	    {
          Sig_Si = Sig_Si2  = Sig_TiSi =0;
		  fmS_ = fmS + WIDTH_MEM*k+l;
		   for(i=0;i< nTempHeight ;i+=m_nSkip) 
		   {
			  fmT_  = fmT + nTempWidth*i;
			  fmS__ = fmS_+ WIDTH_MEM*i;
			  for(j=0 ;j< nTempWidth ;j+=m_nSkip) 
			  {
				 tempS   = *(fmS__+j);
				 Sig_Si  += tempS;
				Sig_Si2 += m_LUT[tempS];
				Sig_TiSi+= m_LUT[tempS+*(fmT_+j)];
			 }
		  }
		  dDiv = sqrt( ( Area*Sig_Si2 - Sig_Si*Sig_Si)*(Area*Sig_Ti2-Sig_Ti*Sig_Ti) );
		  if (dDiv > 0.0001 ) dCorrData = ( Area*(Sig_TiSi-Sig_Ti2-Sig_Si2)/2 - Sig_Ti*Sig_Si)/dDiv;
		  else                dCorrData = 0.0;

		   dCorData[nCount][0] = m_dCorrX+l;
		   dCorData[nCount][1] = m_dCorrY+k;
		   dCorData[nCount++][2] = dCorrData;

	    }
		
	    if(!FindSecondPolynomialPeak(9, dCorData,&dCorrX,&dCorrY))
			if( fabs( m_dCorrX - dCorrX) <1 && fabs( m_dCorrY - dCorrY) <1)
		    {
			   m_dCorrX= dCorrX;
	           m_dCorrY= dCorrY;
		    }
	  }
  }
  //END-0:<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	
  for(i=0;i<9;i++) if(dCorData[i][2]==0) return 1;
  return 0;
}

int CPatternMatch::FindSecondPolynomialPeak(int N, double Data[][3], double *rx, double *ry)
{
	int i;
    double A[9][9];
	double AT[9][9];
	double ATA[9][9];
	double ATA_Inverse[9][9];
	double B[9],ATB[9],R[9];

	for(i=0;i<N;i++)
	{ 
	   A[i][0]=Data[i][0]*Data[i][0];
	   A[i][1]=Data[i][1]*Data[i][1];
	   A[i][2]=Data[i][0]*Data[i][1];
	   A[i][3]=Data[i][0];
	   A[i][4]=Data[i][1];
	   A[i][5]=1;
	   B[i]   =Data[i][2];
	}

	MatrixTranspose(9, 6, A, AT);
	MatrixMultiplicant(6, 9,AT, 9, 6, A, ATA);
	MatrixInverse(6, ATA, ATA_Inverse);
	MatrixMultiplicant2(6, 9, AT, B, ATB);
	MatrixMultiplicant2(6, 6, ATA_Inverse, ATB, R);

	if( (R[2]*R[2]-4*R[0]*R[1]) !=0)
	{
		*rx=(2*R[3]*R[1]-R[2]*R[4])/(R[2]*R[2]-4*R[0]*R[1]);
		*ry=(2*R[0]*R[4]-R[3]*R[2])/(R[2]*R[2]-4*R[0]*R[1]);
	}
	else 
		return 1;

	return 0;
}



//------------------------------------------------------------------------------
// 한칸식 건너뛰며 선택된 Pixel에 대해서 그 Pixel과 이를 둘러싼 8개의 Pixel 즉
// 3x3의 Pixel의 평균값을 구해서 압축된 이미지의 Pixel값을 만든다.
int CPatternMatch::MakePyramid(LPBYTE fmS,LPBYTE fmD,int nWidth, int nHeight)
{
	int i,j;  
	LPBYTE fm1,fm2,fm3;
	const float W025=0.25;
	const float W050=0.50;
	LPBYTE fmD_=fmD;

	  
	  for(i=1; i < nHeight-1 ; i+=2) 
	     for(j=1; j < nWidth-1; j+=2) 
		 {

		     fm1= fmS+j+(i-1)*nWidth;
		     fm2= fm1+nWidth;
		     fm3= fm2+nWidth;

		     *(fmD++)= (unsigned char)(W025*(W025**(fm1-1)+W050**fm1+W025**(fm1+1))+
									   W050*(W025**(fm2-1)+W050**fm2+W025**(fm2+1))+
									   W025*(W025**(fm3-1)+W050**fm3+W025**(fm3+1)));
	        		    
		 }
	  
	  return 0;
}

int CPatternMatch::MakePyramidForSearchArea(LPBYTE fmS,LPBYTE fmD,int nWidth, int nHeight,int WIDTH_MEM)
{
	int i,j, count;  
	const double W025=0.25;
	const double W050=0.50;
	LPBYTE fm1,fm2,fm3;

	  count=0;
	  for(i=1; i < nHeight-1 ; i+=2) 
	     for(j=1; j < nWidth-1; j+=2) 
		 {

		     fm1= fmS+j+(i-1)*WIDTH_MEM;
		     fm2= fm1+WIDTH_MEM;
		     fm3= fm2+WIDTH_MEM;

		     *(fmD++)= (unsigned char)(W025*(W025**(fm1-1)+W050**fm1+W025**(fm1+1))+
									   W050*(W025**(fm2-1)+W050**fm2+W025**(fm2+1))+
									   W025*(W025**(fm3-1)+W050**fm3+W025**(fm3+1)));

		 }
	  
	  return 0;
}

void CPatternMatch::SaveTemplate(CFile *f)
{
	 if(m_TempWidth[0]>0)
     {
	    f->Write(&m_TempWidth[0],sizeof(int));
	    f->Write(&m_TempHeight[0],sizeof(int));
        f->Write(m_fmTemp[0],m_TempWidth[0]*m_TempHeight[0]);
     }
}
void CPatternMatch::LoadTemplate(CFile *f)
{
	 int i;

	 f->Read(&m_TempWidth[0],sizeof(int));
	 f->Read(&m_TempHeight[0],sizeof(int));

     m_TempWidth[1] =(int)(m_TempWidth[0]/2.0-0.5);     
     m_TempWidth[2] =(int)(m_TempWidth[1]/2.0-0.5);     
     m_TempWidth[3] =(int)(m_TempWidth[2]/2.0-0.5);  

     m_TempHeight[1]=(int)(m_TempHeight[0]/2.0-0.5); 
     m_TempHeight[2]=(int)(m_TempHeight[1]/2.0-0.5); 
     m_TempHeight[3]=(int)(m_TempHeight[2]/2.0-0.5); 
 

	if(m_fmTemp[0]!=NULL)
		for(i=0;i<4;i++)
		   delete m_fmTemp[i];
    
    for(i=0;i<4;i++)
		m_fmTemp[i]=new BYTE[m_TempWidth[i]*m_TempHeight[i]];

    f->Read(m_fmTemp[0],m_TempWidth[0]*m_TempHeight[0]);
	
	// Template를 1/2, 1/4, 1/8로 축소한다
	for(i=1;i<4;i++)
	   MakePyramid(m_fmTemp[i-1],m_fmTemp[i],m_TempWidth[i-1],m_TempHeight[i-1]);

}

LPBYTE CPatternMatch::GetTempData(int Level, int *Width, int *Height)
{
	if(Level>3) return NULL;

	if(m_fmTemp[Level])
	{
		*Width = m_TempWidth[Level];
		*Height= m_TempHeight[Level];
		return m_fmTemp[Level];
	}
	else
		return NULL;
}
LPBYTE CPatternMatch::GetSearchData(int Level, int *Width, int *Height)
{
	if(Level>3) return NULL;

	if(m_fmSearch[Level])
	{
		*Width = m_SearchWidth[Level];
		*Height= m_SearchHeight[Level];
		return m_fmSearch[Level];
	}
	else
		return NULL;

}





// PC용(C++) >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

// Matrix Transpose
// n x m ==> m x n
int CPatternMatch::MatrixTranspose(int n, int m, double A[][9], double Ai[][9])
{
	int i,j;
	for(i=0;i<n;i++)
	   for(j=0;j<m;j++)
          Ai[j][i]=A[i][j];	      

	return 0;
}
// Matrix Mulpiplicant
// Size [n1 x m1] [n2 x m2] => [n1 x m2]
// [A][B]=[C]
int CPatternMatch::MatrixMultiplicant(int n1, int m1, double A[][9], int n2, int m2, double B[][9], double C[][9])
{
	int i,j,k;

	if(	n1 > 9 || m1 >9 || n2 >9 || m2 >9 || m1!=n2) return 1;
	
	for(i=0 ; i<n1; i++)
		for(j=0; j<m2; j++)
		   C[i][j]=0;


	for(i=0 ; i<n1; i++)
		for(j=0; j<m1; j++)
		    for(k=0; k<m1; k++)
			   C[i][j]+=A[i][k]*B[k][j];

	return 0;
}

// Matrix Mulpiplicant
// Size [n x m] [m x 1] => [n x 1]
// [A][B]=[C]
int CPatternMatch::MatrixMultiplicant2(int n, int m, double A[][9], double B[], double C[])
{
	int i,j;

	if(	n > 9 || m >9 ) return 1;
	
	for(i=0 ; i<n; i++)
		   C[i]=0;


	for(i=0 ; i<n; i++)
		for(j=0; j<m; j++)
			   C[i]+=A[i][j]*B[j];

	return 0;
}

// MAtrix Inverse
// Size n x n
// Inverse [A] => [Ai]
// return 0: 정상 NONZERO 불량 
int CPatternMatch::MatrixInverse(int n, double A[][9], double Ai[][9])
{
   double big, pivot_inverse, temp, abs_element;
   int pivot_flag[9], swap_col[9], swap_row[9];
   int i,j, row,col,swap,irow,icol;

   if(	n > 9) return 1;
   
   for(i=0;i<n;i++)   
	   for(j=0;j<n;j++)
   			Ai[i][j]=A[i][j];

   for(i=0;i<9;i++)
     pivot_flag[i]=swap_row[i]=swap_col[i]=0;


   for(i=0;i<n;i++)  //iterations of pivots
   {
	   big=0.0;
       for(row=0; row <n ;row++)
	   {
		   if(!pivot_flag[row]) // only unused pivotd
		   {
			   for(col=0; col<n ;col++)
	           {
                  if(!pivot_flag[col])
			      {
				     abs_element=fabs(Ai[row][col]);
				     if(abs_element >= big)
				     {
					    big=abs_element;
					    irow=row;
					    icol=col;
				     }
			      }
		       }
	        }
       }
       pivot_flag[icol]++;
   
   // swap rows to make this diagonal the bggest absolute pivot 
       if(irow!=icol)
       {
	      for(col=0;col <n;col++)
	      {
		     temp=Ai[irow][col];
		     Ai[irow][col]=Ai[icol][col];
		     Ai[icol][col]=temp;
	      }
       }

   //  store what we swaped
       swap_row[i]=irow;
       swap_col[i]=icol;

   // Bad News if the pivot is zero
       if(Ai[icol][icol]==0.0)  return 1;
	   

   // divide the row by the pivot
       pivot_inverse = 1.0/Ai[icol][icol];
       Ai[icol][icol]=1.0;
       for(col=0;col<n;col++)
	      Ai[icol][col]=Ai[icol][col]*pivot_inverse;

   //Fix the other rows by substraction
       for(row=0; row <n; row++)
	      if(row!=icol)
          {
	         temp=Ai[row][icol];
		     Ai[row][icol]=0.0;
		     for(col=0; col<n ; col++)
			    Ai[row][col] = Ai[row][col]-Ai[icol][col]*temp;
          }
   }
   for(swap=n-1;swap>=0;swap--)
   {
	   if(swap_row[swap] !=swap_col[swap])
	   {
		   for(row=0;row<n;row++)
		   {
			   temp=Ai[row][swap_row[swap]];
			   Ai[row][swap_row[swap]]=Ai[row][swap_col[swap]];
			   Ai[row][swap_col[swap]]=temp;
		   }
	   }
   }
 
   return 0;

}



int SavePatternData(CString sFileName)
{
	CFile f;
	int i, ret;
	
	ret=f.Open(sFileName, CFile::modeCreate|CFile::modeWrite);
	if(ret)
	{
		f.Write(g_Pattern, MAX_PATTERN*sizeof(PATTERNDATA));
		for(i=0;i<MAX_PATTERN;i++)
		{
			if(g_Pattern[i].m_nAddCount>0)
			{
				f.Write(g_Pattern[i].m_fm[0],  g_Pattern[i].m_nWidth*g_Pattern[i].m_nHeight);
				f.Write(g_Pattern[i].m_fm[1],  g_Pattern[i].m_nWidth*g_Pattern[i].m_nHeight);
			}
		}
		f.Close();
	}
	return ret;
}


int LoadPatternData(CString sFileName)
{
	CFile f;
	int i, ret;
	
	for(i=0;i<MAX_PATTERN;i++)
	{
		if(g_Pattern[i].m_nAddCount>0)
		{
			if(g_Pattern[i].m_fm[0]) delete g_Pattern[i].m_fm[0];
			if(g_Pattern[i].m_fm[1]) delete g_Pattern[i].m_fm[1];
		}
	}

	ret=f.Open(sFileName, CFile::modeRead);
	if(ret)
	{
		f.Read(g_Pattern, MAX_PATTERN*sizeof(PATTERNDATA));
		for(i=0;i<MAX_PATTERN;i++)
		{
			if(g_Pattern[i].m_nAddCount>0)
			{
				g_Pattern[i].m_fm[0]=new BYTE[g_Pattern[i].m_nWidth*g_Pattern[i].m_nHeight];
				g_Pattern[i].m_fm[1]=new BYTE[g_Pattern[i].m_nWidth*g_Pattern[i].m_nHeight];

				f.Read(g_Pattern[i].m_fm[0],  g_Pattern[i].m_nWidth*g_Pattern[i].m_nHeight);
				f.Read(g_Pattern[i].m_fm[1],  g_Pattern[i].m_nWidth*g_Pattern[i].m_nHeight);
			}
		}
		f.Close();
	}
	return ret;
}