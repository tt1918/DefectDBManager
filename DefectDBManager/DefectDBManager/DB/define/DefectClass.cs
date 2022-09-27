using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    public class DefectType
    {
        public const int Dot = 0x00000001; // 점불량           AG부츠,SENI
        public const int Line = 0x00000002; // 선불량          하쿠항스지,시로스지,요코레스지,쿠로스지,AG스지
        public const int Circle = 0x00000004; // 원형 불량     하쿠항,요코레,쿠로누케,AG누케
        public const int White = 0x00000008; // 백점 불량      미분조명 : AG부츠,SENI,하쿠항,요코레,하쿠항스지,시로스지,요코레스지 

        public const int Spv = 0x00000010; // SPV 불량
        public const int Scratch = 0x0000000D; // 스크래치 불량


        public const int BrcKoh = 0x00000003;
        public const int BrcKor = 0x00000001;

        //검반 결점 클래스
        //------------------------------------------------------
        //1:스리키즈 휘점, 3:쿠닉 , 4: 기포류 , 5: 스지, 6, 점: 7,선 8.주기.
        public const int KT_CROSS = 0x0101;
        public const int KT_CROSSKUNIC = 0x0103;
        public const int KT_SLIT = 0x0105;
        public const int KT_DIFF = 0x0104;
        public const int KT_LINE = 0x0107;
        public const int KT_DOT = 0x0106;
        public const int KT_PERIOD = 0x0108;

        public const int ES_DOT = 0x0200;
        public const int ES_CROSS = 0x0201;  //스리키즈
        public const int ES_DUMMY = 0x0202;  //Dummy
        public const int ES_CROSSKUNIC = 0x0203; //쿠닉
        public const int ES_DIFF = 0x0204;   //기포
        public const int ES_SLIT = 0x0205;       //스지
        public const int ES_DOT4 = 0x0206;   //점
        public const int ES_LINE4 = 0x0207;      //선

        public const int DBKT_DOT = 0x0300;
        public const int DBKT_CROSS = 0x0301;    //스리키즈
        public const int DBKT_DUMMY = 0x0302;    //Dummy
        public const int DBKT_CROSSKUNIC = 0x0303;   //쿠닉(찍힘)
        public const int DBKT_DIFF = 0x0304; //기포
        public const int DBKT_SLIT = 0x0305; //스지
        public const int DBKT_DOT4 = 0x0306; //점
        public const int DBKT_LINE4 = 0x0307;        //선
        public const int DBKT_PERIOD = 0x0308;   //주기성

        public const int BCR_MARK = 0x0401;//B-Mark

        public const int MURA = 0x00000010;  //불량이 무라(얼룩)인 경우
        public const int YOKODAN = 0x00000020;//불량이 요코단(위방향의 선이나 결점)인 경우
        //------------------------------------------------------
       
        //코텍 접합 결점 클래스
        //------------------------------------------------------
        public const int JH_KT_CROSS = 0x0111;   //CSV
        public const int JH_KT_CROSSKUNIC = 0x0113;  //CSV
        public const int JH_KT_DIFF = 0x0114;    //CSV
        public const int JH_KT_SLIT = 0x0115;    //CSV
        public const int JH_KT_DOT = 0x0116; //CSV
        public const int JH_KT_LINE = 0x0117;    //CSV

        public const int JH_ES = 0x0121; //연신
        public const int JH_TG = 0x0122;     //점착
        public const int JH_ETC = 0x0123;        //그외
        public const int JH_BARCODE = 0x01FF;
        //------------------------------------------------------


        //------------------------------------------------------
        //코텍 신규 RK  결점 클래스
        public const int RK_ES = 0x0110; //연신
        public const int RK_TG = 0x0120; //점착
        public const int RK_ETC = 0x0130;        //그외
        public const int RK_USER = 0x0160;
        public const int RK_USER_1 = 0x0161; //User Define
        public const int RK_USER_2 = 0x0162; //User Define
        public const int RK_USER_3 = 0x0163; //User Define
        public const int RK_USER_4 = 0x0164;     //User Define
        public const int RK_USER_5 = 0x0165;     //User Define
        public const int RK_USER_6 = 0x0166; //User Define
        public const int RK_USER_7 = 0x0167; //User Define
        public const int RK_USER_8 = 0x0168; //User Define
        public const int RK_USER_9 = 0x0169; //User Define
        public const int RK_USER_10 = 0x016A;    //User Define
        public const int RK_BARCODE = 0x01FF;
    }
}
