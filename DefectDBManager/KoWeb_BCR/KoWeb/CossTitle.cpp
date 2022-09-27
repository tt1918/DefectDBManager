#include "stdafx.h"


#define MAIN_BACK_COLOR		RGB(112,146,190);

#ifdef USE_SK_BAT
TCHAR g_sDefectName[16][10][20]={ {_T(""),		_T("백점"),	_T("흑점"),			_T("찍힘"),			_T("흑라인"),			_T("Side Black"),				_T(""),			_T(""),		_T(""),			_T("")}, //COS0
								  {_T(" "),		_T("White"),_T("Black"),		_T("Dent"),			_T("B Line"),			_T("Side Black"),				_T(""),			_T(""),		_T(""),			_T("")}, //COS1
								  {_T(" "),		_T("White"),_T("Black"),		_T("Dent"),			_T("B Line"),			_T("Side Black"),				_T(""),			_T(""),		_T(""),			_T("")}, //COS2
								};
#else

#ifdef LANGUAGE_ENGLISH		     //사용안함          1			2			     3						4                5					6				7			8				9
TCHAR g_sDefectName[16][10][20]={ {_T(""),		_T("찍힘"),	_T(""),			_T("TD라인"),			_T("MD라인"),	_T("사선라인"),	    _T(""),		    _T("표면요철"),	_T("얼룩"),	_T("")}, //COSA

#ifdef USE_3D_AREA  //정코 COS1
								  {_T(" "),		_T("백점"),	_T("흑점"),			_T("FishEye"),		_T("얼룩"),		_T("액정라인"),		_T("마킹"),		_T(""),		_T(""),			_T("")}, //정코3D COS1 AREA
								  {_T(" "),		_T("백점"),	_T("흑점"),			_T("FishEye"),		_T("입술불량"),	_T("액정라인"),		_T("마킹"),		_T(""),		_T(""),			_T("")},     //정코3D COS2 AREA
#else
								  {_T(" "),		_T("휘점"),	_T("쿠닉"),			_T("스크라치"),		_T("꼽침"),		_T(""), 			_T(""),			_T("군집"),	_T(""),			_T("")}, //COS1
	#ifdef	WEB_DAKANO_MULTAE								
								  {_T(" "),		_T("물때얼룩"),	_T("군집"),		_T("원형얼룩"),		_T(""),			_T(""),				_T(""),			_T(""),		_T(""),			_T("")},		//COS2 물떄얼룩
	#else
		#if INSPECT_TYPE==TAC_LINE								
								  {_T(" "),		_T("(백)이물"),	_T("(흑)눌림"),	_T("(흑)오염"),		_T("(백)얼룩"),	_T("라인"),			_T("군집"),		_T("점핑"),	_T("연결"),		_T("")},  //COS2  정밀코팅	
		#else
								  {_T(" "),		_T("백점"),	 _T("흑점"),		_T("찍힘"),			_T(""),			_T(""),				_T(""),			_T(""),		_T(""),			_T("")}, //COS2	
		#endif								
	#endif
#endif

#ifdef USE_DAKANO_LINE
								  {_T(" "),		_T("액정라인"),	_T("휘점"),	   _T("쿠닉"),			_T("스크라치"),		_T("군집"),				_T(""),			_T(""),	_T(""),		_T("")}, //COS3
#else
								  {_T(" "),		_T("백점"),	_T("라미눌림"),		_T(""),				_T("Scratch"),	_T("L Scratch"),	_T(""),			_T(""),		_T(""),			_T("")}, //COS3
#endif
								  {_T(" "),		_T("백점"),	_T("흑점"),			_T("찍힘"),			_T("펜마킹"),	_T(""),				_T(""),			_T(""),		_T(""),			_T("")}, //COS4
								  {_T(" "),		_T("White"),	_T("Black"),	_T(""),				_T("MD Line"),	_T("TD Line"),		_T("Group"),	_T("Mura"),	_T(""),			_T("")}, //COS5								 

#if INSPECT_TYPE==TAC_LINE								
								  {_T(" "),		_T("(백)이물"),	_T("(흑)눌림"),	_T("(흑)오염"),		_T("(백)얼룩"),	_T("라인"),		_T("군집"),		_T("점핑"),	_T("연결"),		_T("")}, //COS6  정밀코팅	
#else
								  {_T(" "),		_T("백점"),	_T("흑점"),			_T("찍힘"),			_T("라인"),		_T(""),				_T(""),			_T("군집"),		_T(""),			_T("")}, //COS6	
#endif	



#if INSPECT_TYPE==USE_DSFR
								  {_T(" "),		_T("덴트I"),_T("흑점"),			_T("찍힘"),			_T("덴트II"),   _T("흑얼룩"),		_T("라인"),		_T("Group"),_T(""),			_T("")}, //COS7
#else
								  {_T(" "),		_T("White"),_T("Black"),		_T("Press"),		_T("얼룩"),		_T("LineKipo"),		_T("KipoPress"),_T("Group"),_T("Marking"),	_T("")}, //COS7
#endif

#ifdef MY_RECV
								  {_T(" "),		_T("White"),	_T("Black"),	_T("찍힘"),		_T("눌림"),		    _T("반짝눌림"),	    _T(""),			_T(""),		_T(""),			_T("")}, //COS8
#else
								  {_T(" "),		_T("White"),	_T("Black"),	_T("Press"),		_T("Group"),	_T(""),				_T("꺽임"),		_T(""),		_T(""),			_T("")}, //COS8
#endif


#ifdef USE_COS9_LR								
								  {_T(" "),		_T("백점"),	_T("흑점"),			_T("LR"),		_T("얼룩"),		_T("라인오염"),			_T("군집"),			_T(""),	_T(""),			_T("")}, //COS9
#else

		#ifdef NANJING_OLED
								  {_T(" "),		_T("WhiteSpot"),	_T("Cunic"),_T("Scratch"),		_T("Kkopchim"),_T("Group"),		_T("BloackSpot"),	_T("군집"),	_T(""),			_T("")}, //남경 OLED COS9  (우선순위 : 스크라치>백점>흑점>쿠닉>군집)
		#else
								  {_T(" "),		_T("휘점"),	_T("쿠닉"),			_T("스크라치"),		_T("꼽침"),		_T(""),				_T(""),			_T("군집"),	_T(""),			_T("")}, //COS9
		#endif
#endif

								  {_T(" "),		_T("찍힘"),	_T("흑점"),			_T("찍힘"),			_T(""),			_T(""),				_T(""),			_T(""),		_T(""),			_T("")}, //COSA
								  {_T(" "),		_T("백점"),	_T("쿠닉"),			_T("스크라치"),		_T("꼽침"),		_T(""),				_T(""),			_T(""),		_T(""),			_T("")}, //COSB
								  {_T(" "),		_T("백점"),	_T("흑점"),			_T("찍힘"),			_T(""),			_T(""),				_T(""),			_T(""),		_T(""),			_T("")}, //COSC
								  {_T(" "),		_T("백점"),	_T("흑점"),			_T("찍힘"),			_T(""),			_T(""),				_T(""),			_T(""),		_T(""),			_T("")}, //COSD
								  {_T(" "),		_T(""),	_T(""),			_T(""),			_T(""),			_T(""),				_T(""),			_T(""),		_T(""),			_T("")}, //BCR
								  {_T(" "),		_T(""),	_T(""),			_T(""),			_T(""),			_T(""),				_T(""),			_T(""),		_T(""),			_T("")}, //Marking Vision
								};
#else							//사용안함         첫번째     두번째         3             4                5          6            7          8            9
TCHAR g_sDefectName[16][10][20]={ {_T(""),		_T("백점"),	_T("흑점"),			_T("찍힘"),			_T(""),			_T(""),				_T(""),			_T(""),		_T(""),			_T("")}, //COS0

#ifdef USE_3D_AREA  //정코 COS1
								  {_T(" "),		_T("백점"),	_T("흑점"),			_T("FishEye"),		_T("얼룩"),		_T("액정라인"),		_T("마킹"),		_T(""),		_T(""),			_T("")}, //정코3D COS1 AREA
								  {_T(" "),		_T("백점"),	_T("흑점"),			_T("FishEye"),		_T("입술불량"),	_T("액정라인"),		_T("마킹"),		_T(""),		_T(""),			_T("")},     //정코3D COS2 AREA
#else
								  {_T(" "),		_T("휘점"),	_T("쿠닉"),			_T("스크라치"),		_T("꼽침"),		_T(""), 			_T(""),			_T("군집"),	_T(""),			_T("")}, //COS1
	#ifdef	WEB_DAKANO_MULTAE								
								  {_T(" "),		_T("물때얼룩"),	_T("군집"),		_T("원형얼룩"),		_T(""),			_T(""),				_T(""),			_T(""),		_T(""),			_T("")},		//COS2 물떄얼룩
	#else
		#if INSPECT_TYPE==TAC_LINE								
								  {_T(" "),		_T("(백)이물"),	_T("(흑)눌림"),	_T("(흑)오염"),		_T("(백)얼룩"),	_T("라인"),			_T("군집"),		_T("점핑"),	_T("연결"),		_T("")},  //COS2  정밀코팅	
		#else
								  {_T(" "),		_T("백점"),	_T("흑점"),			_T("찍힘"),			_T(""),			_T(""),				_T(""),			_T(""),		_T(""),			_T("")}, //COS2	
		#endif								
	#endif
#endif



								  {_T(" "),		_T("백점"),	_T("라미눌림"),		_T(""),				_T("스크라치"),	_T(""),				_T(""),			_T(""),		_T(""),			_T("")}, //COS3
								  {_T(" "),		_T("백점"),	_T("흑점"),			_T("찍힘"),			_T("펜마킹"),	_T(""),				_T(""),			_T(""),		_T(""),			_T("")}, //COS4
								  {_T(" "),		_T("White"),	_T("Black"),	_T(""),				_T("MD Line"),	_T("TD Line"),		_T("Group"),	_T("Mura"),	_T(""),			_T("")}, //COS5								 

#if INSPECT_TYPE==TAC_LINE								
								  {_T(" "),		_T("White"),	_T("B Mura"),	_T("Dirt"),		_T("(W Mura"),	_T("Line"),		_T("Group"),		_T("Jumpping"),	_T("Tape"),		_T("")}, //COS6  정밀코팅	
#else
								  {_T(" "),		_T("백점"),	_T("흑점"),			_T("찍힘"),			_T("라인"),		_T(""),				_T(""),			_T(""),		_T(""),			_T("")}, //COS6	
#endif	



#if INSPECT_TYPE==USE_DSFR
								  {_T(" "),		_T("덴트I"),_T("흑점"),			_T("찍힘"),			_T("덴트II"),   _T("흑얼룩"),		_T("라인"),		_T("Group"),_T(""),			_T("")}, //COS7
#else
								  {_T(" "),		_T("White"),_T("Black"),		_T("Press"),		_T("얼룩"),		_T("LineKipo"),		_T("KipoPress"),_T("Group"),_T("Marking"),	_T("")}, //COS7
#endif


								  {_T(" "),		_T("백점"),	_T("흑점"),			_T("찍힘"),			_T("군집"),			_T(""),				_T("꺽임"),			_T(""),		_T(""),			_T("")}, //COS8
								  {_T(" "),		_T("휘점"),	_T("쿠닉"),			_T("스크라치"),		_T("꼽침"),		_T(""),				_T(""),			_T("군집"),	_T(""),			_T("")}, //COS9
								  {_T(" "),		_T("찍힘"),	_T("흑점"),			_T("찍힘"),			_T(""),			_T(""),				_T(""),			_T(""),		_T(""),			_T("")}, //COSA
								  {_T(" "),		_T("백점"),	_T("쿠닉"),			_T("스크라치"),		_T("꼽침"),		_T(""),				_T(""),			_T("군집"),	_T(""),			_T("")}, //COSB
								  {_T(" "),		_T("백점"),	_T("흑점"),			_T("찍힘"),			_T(""),			_T(""),				_T(""),			_T(""),		_T(""),			_T("")}, //COSC
								  {_T(" "),		_T("백점"),	_T("흑점"),			_T("찍힘"),			_T(""),			_T(""),				_T(""),			_T(""),		_T(""),			_T("")}, //COSD
								  {_T(" "),		_T(""),	_T(""),			_T(""),			_T(""),			_T(""),				_T(""),			_T(""),		_T(""),			_T("")}, //BCR
								  {_T(" "),		_T(""),	_T(""),			_T(""),			_T(""),			_T(""),				_T(""),			_T(""),		_T(""),			_T("")}, //Marking Vision
								};
#endif
#endif



TCHAR	 g_sMainView[10][100]={_T("<Defect Info (size, x, y :mm)>"),};
COLORREF g_cMainView[10]={ RGB(255,255,255),  };
CRect	 g_rcMainView[10]={	CRect(605,725,785,745),};
int      g_nMainViewCnt=0;


void LoadNGID(int nVisionLine)
{
	NG_SPOT_WHITE	=1;		//백점, 휘점
	NG_LAMI_PRESS   =2;		//Lami Press
	NG_SPOT_BLACK	=2;		//흑점
	NG_SCRATCH		=4;		//스크라치
	NG_LONGSCRATCH  =5;		//Long스크라치
	NG_SPOT_PRESS	=3;		//찍힘
	NG_CUNIC		=3;		//쿠닉
	NG_SPOT_KIPO	=3;		//이물기포
	NG_KKOPCHIM		=4;		//꼽침
	NG_MURA			=7;		//얼룩
	NG_TAC_MURA     =4;		//TAC얼룩
	NG_TAC_MURA_B	=5;		//TAC(흑)얼룩
	NG_TAC_LINE		=6;		//TAC라인
	NG_LINE			=4;		//라인
	NG_LINE_ANGLE   =3;		//사선라인
	NG_LINE_TD		=4;		//수평라인
	NG_GROUP		=2;
	NG_LINEKIPO		=5;		//라인기포
	NG_MARKING		=3;		//정코마킹


	if(nVisionLine==COS1)
	{
		NG_SPOT_WHITE=1;
		NG_CUNIC	 =2;
		NG_SCRATCH	 =3;
		NG_KKOPCHIM  =4;
		NG_GROUP	 =3;

		//사용안함
		NG_SPOT_BLACK=0;
		NG_SPOT_PRESS=0;
		NG_SPOT_KIPO =0;
		NG_MURA		 =0;
		NG_LINE		 =0;
		NG_TAC_MURA	 =0;
		NG_TAC_MURA_B=0;
	}
	else if(nVisionLine==COS4)
	{
		NG_COS4_105INCH  =4;
	}
	else if(nVisionLine==COS5)
	{
#ifdef COS5_GUANGJOO
		NG_LINE_ANGLE   =3;		//사선라인
		NG_LINE			=4;		//라인
		NG_LINE_TD		=5;		//수평라인
		NG_BLACK_GROUP	=6;
		NG_BLACK_MURA	=7;		//흑얼룩
#else
		NG_LINE_ANGLE   =3;		//사선라인			Size 1
		NG_LINE			=4;		//MD 라인			Size 1
		NG_LINE_TD		=4;     //TD 라인			Size 2
		NG_BLACK_MURA	=5;		//흑얼룩(B.Stain)	//
		NG_BLACK_GROUP	=6;		//흑군집
	//	NG_WHITE_MURA	=7;		//백얼룩
#endif
	}
	else if(nVisionLine==COS6)
	{
		NG_GROUP=7;
	}
	else if(nVisionLine==COS7)
	{
		NG_KIPOPRESS=6;
		NG_GROUP	=7;
		NG_MARKING  =8;
	}
	else if(nVisionLine==COSA)
	{
		NG_LINE_TD		=3;		//수평라인
		NG_LINE			=4;		//라인
		NG_LINE_ANGLE   =5;		//사선라인
		NG_BLACK_MURA	=8;		//흑얼룩(B.Stain)	//
	}
	else if(nVisionLine==COS8)
	{
		NG_GROUP	 =4;
		NG_LINE_ANGLE=6;

#ifdef MY_RECV
		NG_CUNIC=4;
#endif
	}	

#ifdef USE_3D_AREA
	if(nVisionLine==COS1 || nVisionLine==COS2)
	{
		NG_SPOT_WHITE	=1;		//백점
		NG_SPOT_BLACK	=2;		//흑점
		NG_FISHEYE		=3;		//
		NG_MURA			=4;	    //HTO 얼룩형태 불량
		NG_LINE			=5;		//라인
		NG_MARKING		=6;		//정코마킹
		NG_LIPSHAPE		=4;
	}	
#endif

#ifdef WEB_DAKANO_MULTAE	
		NG_GROUP		=2;	//물때 그룹
		NG_MURA			=3;
#endif

#ifdef USE_COS9_LR
		NG_GROUP		=6;
		NG_KKOPCHIM		=3;
		NG_MURA			=4;
#endif

#if INSPECT_TYPE==TAC_LINE
		if(nVisionLine==COS2 || nVisionLine==COS6)
		{
			NG_SPOT_WHITE	=1;		//(백)이물 : 백점
			NG_SPOT_BLACK	=3;		//(흑)오염 : 흑점

			NG_TAC_MURA     =4;		//(백)얼룩
			NG_TAC_MURA_B	=2;		//(흑)눌림

			NG_SCRATCH		=5;		//스크라치
		}
#endif



#ifdef NANJING_OLED
		NG_SPOT_WHITE=1;		//휘점
		NG_CUNIC	 =2;		//쿠닉
		NG_SCRATCH	 =3;		//스크라치 (Y/X >4 이고 Y가 설정값보다 크면)
		NG_KKOPCHIM	 =4;		//꼽침
		NG_GROUP	 =5;		//군집
		NG_SPOT_BLACK=6;		//흑점		
#endif

#ifdef USE_DAKANO_LINE
		NG_SPOT_WHITE=2;		//휘점
		NG_CUNIC	 =3;		//쿠닉
		NG_SCRATCH	 =4;		//스크라치 (Y/X >4 이고 Y가 설정값보다 크면)		
		NG_GROUP	 =5;		//군집
#endif

}