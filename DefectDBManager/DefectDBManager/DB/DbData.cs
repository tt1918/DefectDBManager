using Microsoft.SqlServer.Server;
using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    #region DB Query 
    public class PTRYLPdata
    {
        //1단계 potylp 접속 //  품종 : VEGQ1723NTBCAG330-H1-AC(G)  , 연신 LOT : EC80302-02  return;
        // Search Item = YLMLOT(점착LOT) 
        // 18번 RollNo 일때 20번 혹은 21번 추출 "YLSGEB"
        //  
        //1			2		3		4		5		6		7		8		9		10		11		12		13		14		
        //YLMLOT	YLMKYC	YLMMAC	YLMPPC	YLMKNC	YLMKSB	YLMZKY	YLMZKN	YLMSAG	YLMTON	YLMKAS	YLMYKH	YLMSOK	YLYIEL	
        //점착LOT	x		x		x		x		x		품종	    x		x		투입M	완성M	원단폭	X		X		
        //20												X						8		5		5								
        //
        //15		16		17		18		19		20		21		22		23		24		25		26		27		28		29		30		31
        //YLLEVE	YLSLOT	YLSSEQ	YLSGEB	YLSZKB	YLSZKY	YLSZKN	YLSTON	YLYSKH	YLCRDT	YLCRTM	YLOPDT	YLRPTM	YLNMID	YLLGID	YLPCID	YLPGID
        //X		    연신LOT	X		X		X		X-----------------------------------------
        //42				42				40		42

        public string YLMLOT;   //g1 점착lot
        public string YLMZKY;   //g8 품종
        public string YLMZKN;   //g9 이전 프로그램에서는 사용 안 함
        public float  YLMTON;    //g3 투입M
        public float  YLMKAS;    //g4 완성M
        public string YLMYKH;   //g5 원단폭
        public string YLSLOT;   //g6 점착LOT <--연신Lot
        public string YLSZKY;   //g2 품명
        public string YLSZKN;   //g7 원래 프로그램에서 데이터 넣는 부분 이름이 이상함 데이터. 데이터 읽어오는것 외에 사용 안 함 

        public void Parse(OracleDataReader reader)
        {
            YLMLOT = reader[0].ToString();

            YLMZKY = reader[6].ToString();
            YLMZKN = reader[7].ToString();

            float valF = 0.0f;
            if (float.TryParse(reader[9].ToString(), out valF) == true)
                YLMTON = valF;
            if (float.TryParse(reader[10].ToString(), out valF) == true)
                YLMKAS = valF;

            YLMYKH = reader[11].ToString();
            YLSLOT = reader[15].ToString();
            YLSZKY = reader[19].ToString();
            YLSZKN = reader[20].ToString();
        }

        public override string ToString()
        {
            string msg = $"{YLMLOT}, {YLSZKY}, {YLMTON:F3}, {YLMKAS:F3}, {YLMYKH}, {YLSLOT}, {YLSZKN}";
            return msg;
        }

        public PTRYLPdata Clone()
        {
            PTRYLPdata data = new PTRYLPdata();

            data.YLMLOT = this.YLMLOT;
            data.YLMZKY = this.YLMZKY;
            data.YLMZKN = this.YLMZKN;
            data.YLMTON = this.YLMTON;
            data.YLMKAS = this.YLMKAS;
            data.YLMYKH = this.YLMYKH;
            data.YLSLOT = this.YLSLOT;
            data.YLSZKY = this.YLSZKY;
            data.YLSZKN = this.YLSZKN;

            return data;
        }
    }

    public class XOFSMSTData
    {
        //1			2		3		4			5			6			7		8		9			10
        //KYCD		PPCD	LNCD	YLMZKN2		YLSZKN		X_OFFSET	CRDT	CRTM	UPDT		UPTM
        //거점코드	공정코드	라인코드	최종공정품종명	재료로트품종명	폭좌표보정값	작성일	작성시간	업데이트날짜	업데이트시간
        //3			3		4		100			100			3.1			8		6		8			6

        public string   KYCD;
        public int      PPCD;
        public string   LNCD;
        public string   YLMZKN2;
        public string   YLSZKN;
        public float    X_OFFSET;


        public void Parse(OracleDataReader reader)
        {
            KYCD = reader[0].ToString();
            PPCD = Int32.Parse(reader[1].ToString());
            LNCD = reader[2].ToString();
            YLMZKN2 = reader[3].ToString();
            YLSZKN = reader[4].ToString();
            X_OFFSET = float.Parse(reader[5].ToString());
        }

        public override string ToString()
        {
            string msg = $"{KYCD}, {PPCD}, {LNCD}, {YLMZKN2}, {YLSZKN}, {X_OFFSET:F3}";
            return msg;
        }

        public XOFSMSTData Clone()
        {
            XOFSMSTData data = new XOFSMSTData();

            data.KYCD = this.KYCD;
            data.PPCD = this.PPCD;
            data.LNCD = this.LNCD;
            data.YLMZKN2 = this.YLMZKN2;
            data.YLSZKN = this.YLSZKN;
            data.X_OFFSET = this.X_OFFSET;

            return data;
        }

    }

    public class AREADELData
    {
        //1			2		3		4		5			6			7			8				9				10			11          12          13
        //KYCD		PPCD	LNCD	LOTNO	ENTRY_NUM	STR_WD		END_WD		STR_MD			END_MD			DELFLG		TAKNDTM	    REMOVEKBN   BCNO
        //거점		공정	라인	롯		일련번호	시작폭좌표	종료폭좌표	시작흐름좌표	종료흐름좌표	삭제플러그	읽기일시	?           	
        //3			3		4		30		2			7.2			7.2			11.2			11.2			1			14          1           10

        public string KYCD;
        public string PPCD;
        public string LNCD;
        public string LOTNO;
        public string ENTRY_NUM;
        public float  STR_WD;
        public float  END_WD;
        public float  STR_MD;
        public float  END_MD;
        public string BCNO;

        public void Parse(OracleDataReader reader)
        {
            KYCD = reader[0].ToString();
            PPCD = reader[1].ToString();
            LNCD = reader[2].ToString();
            LOTNO = reader[3].ToString();
            ENTRY_NUM = reader[4].ToString();

            float ret = 0.0f;
            if (float.TryParse(reader[5].ToString(), out ret)) STR_WD = ret;
            else STR_WD = 0.0f;

            if (float.TryParse(reader[6].ToString(), out ret)) END_WD = ret;
            else END_WD = 0.0f
                    ;
            if (float.TryParse(reader[7].ToString(), out ret)) STR_MD = ret;
            else STR_MD = 0.0f;

            if (float.TryParse(reader[8].ToString(), out ret)) END_MD = ret;
            else END_MD = 0.0f;

            BCNO = reader[12].ToString();
        }

        public override string ToString()
        {
            string msg = $"{KYCD}, {PPCD}, {LNCD}, {LOTNO}, {ENTRY_NUM}, {STR_WD:F3}, {END_WD:F3}, {STR_MD:F3}, {END_MD:F3}, {BCNO}";
            return msg;
        }

        public AREADELData Clone()
        {
            AREADELData data = new AREADELData();

            data.KYCD = this.KYCD;
            data.PPCD = this.PPCD;
            data.LNCD = this.LNCD;
            data.LOTNO = this.LOTNO;
            data.ENTRY_NUM = this.ENTRY_NUM;
            data.STR_WD = this.STR_WD;
            data.END_WD = this.END_WD;
            data.STR_MD = this.STR_MD;
            data.END_MD = this.END_MD;
            data.BCNO = this.BCNO;


            return data;
        }
    }

    public class PTRY0PData
    {
        //	1		2		3		4		    5		6		7		8		9		10		    11		    12		13		14		15        
        //	거점	제부	공정	작업구분    품종	기계	색코드	원단LOT	라인	라인식별    원단명칭	개시	종료	완성M	랏품종		
        //	Y0KYCD	Y0MACD	Y0PPCD	Y0SAGK	    Y0HINC	Y0KIKC	Y0IROC	Y0KLOT	Y0LNCD	Y0LNSN	    Y0ZKNM	    Y0KKOL	Y0KSOL	Y0KASS	Y0ZKNM2
        //	3		3		3		2		    5		5		2		20		4		4		    80		    14		14		5		100
        //	x		x		x		x		    o		x		x		o		x		o		    x		    o		o		x		o

        public string LotData;
        public string StartTime;
        public string EndTime;
        public string Y0KYCD;
        public string Y0KLOT;
        public string LNCD;
        public string Y0LNSN;
        public string Y0ZKNM;
        public string Y0KKOL;
        public string Y0KSOL;
        public string Y0KASS;

        public void Parse(OracleDataReader reader)
        {
            LotData = reader[9].ToString();

            StartTime = reader[11].ToString();
            int pos = StartTime.IndexOf(' ');
            if (pos > 0)
                StartTime = StartTime.Substring(0, pos);

            EndTime = reader[12].ToString();
            pos = EndTime.IndexOf(' ');
            if (pos > 0)
                EndTime = EndTime.Substring(0, pos);

            Y0KYCD = reader[0].ToString();
            Y0KLOT = reader[7].ToString();
            LNCD = reader[8].ToString();
            Y0LNSN = reader[9].ToString();
            Y0ZKNM = reader[10].ToString();
            Y0KKOL = reader[11].ToString();
            Y0KSOL = reader[12].ToString();
            Y0KASS = reader[13].ToString();
        }

        public override string ToString()
        {
            string msg = $"{Y0KYCD}, {Y0KLOT}, {Y0LNSN}, {Y0KKOL} , {Y0KSOL}, {Y0ZKNM}, {LNCD}, {Y0KASS}";

            return msg;
        }

        public PTRY0PData Clone()
        {
            PTRY0PData data = new PTRY0PData();

            data.LotData = this.LotData;
            data.StartTime = this.StartTime;
            data.EndTime = this.EndTime;
            data.Y0KYCD = this.Y0KYCD;
            data.Y0KLOT = this.Y0KLOT;
            data.LNCD = this.LNCD;
            data.Y0LNSN = this.Y0LNSN;
            data.Y0ZKNM = this.Y0ZKNM;
            data.Y0KKOL = this.Y0KKOL;
            data.Y0KSOL = this.Y0KSOL;
            data.Y0KASS = this.Y0KASS;

            return data;
        }
    }

    public class MRKCTLMSTData
    {
        //3단계 MRKCTLMST //  라인코드 & fault ID 별 불량 마킹 여부   return;
        // Search Item = YLMLOT(점착LOT)
        //1		2		3		4		5		6			7		8		9
        //MKCD	KYCD	PPCD	LNCD	FLTID	ROLLNAME	MRKF1	MRKF2   SIZE
        //발주처거점C	공정C	라인C	불량ID	품종		통상	결점다발	- 통상과 결점다발은 0일시 마킹안함 1일때 마킹함.
        //3		3		3		4		3		40			1		1
        //x		x		x		o		o		o			o		o

        public string LNCD;
        public string FLTID;
        public string ROLLNAME;
        public string MRKF1;
        public string MRKF2;
        public string PPCD;
        public float  SIZE;

        // Dic 구현해야함
        // FLTID -> SIZE

        public void Parse(OracleDataReader reader)
        {
            PPCD = reader[2].ToString();
            LNCD = reader[3].ToString();
            FLTID = reader[4].ToString();
            ROLLNAME = reader[5].ToString();
            MRKF1 = reader[6].ToString();
            MRKF2 = reader[7].ToString();
            SIZE = float.Parse(reader[8].ToString());
        }

        public override string ToString()
        {
            string msg = $"{LNCD}, {FLTID}, {ROLLNAME}, {MRKF1}, {MRKF2}, {SIZE:F3}";
            return msg;
        }

        public MRKCTLMSTData Clone()
        {
            MRKCTLMSTData data = new MRKCTLMSTData();

            data.LNCD = this.LNCD;
            data.FLTID = this.FLTID;
            data.ROLLNAME = this.ROLLNAME;
            data.MRKF1 = this.MRKF1;
            data.MRKF2 = this.MRKF2;
            data.PPCD = this.PPCD;
            data.SIZE = this.SIZE;

            return data;
        }
    }

    public class INSPDATData
    {
        //5단계 inspdat //  ctlno   return;
        // Search Item = YLMLOT(점착LOT)
        //	1		2		3		4		5		6		7		8		9		10		11		12		13		14		15		16		17		18		19		20		
        //	CTLNO	JIGCD	HINMEI	LOTNO	ROLLNO	STRPOS	BCNO	S_INSP	E_INSP	MACNO	INSPID	VENDER	DATVER	WIDTH	S_YPOS	E_YPOS	S_MASK	E_MASK	LENGTH	X_BASE	
        //	발주처	거점C	품종	LotC
        //  21		22		23		24		25		26	27	    28	    29	    30	    31	    32	    33	    34	        35	    36	    37	    38	    39	    40	    
        //  CONDNAM	STRDT	STRTM	ENDDT	ENDTM	CMT	RESERVE	COND1	COND2	COND3	COND4	COND5	AMPGAIN	OFFSET		USEFLG	PRCDT	PRCTM	TRNSNO	FLTCNT	PICCNT	
        //
        //  41	    42	 43	    44	    45	        46	    47	    48	    49	    50	    51	    52	    53	        54	    55	    56	    57
        //  INSPLVL	GLVL CUSTCD	BKUPFLG	PICDELFLG	MEDIANO	KYCD	KTCD	KSCD	HINBAN	KIKAKU	COLOR	KIKAKUMEI	FIL1	FIL2	GVER	SIMFLG
        //  

        //취득 데이터 
        public string CTLNO;
        public string HINMEI;
        public string LOTNO;
        public string ROLLNO;
        public string BCNO;
        public string STRDT;
        public string STRTM;
        public string ENDDT;
        public string ENDTM;
        public string USEFLG;
        public string CUSTCD;
        public string KYCD;
        public string KTCD;

        // 실데이터
        public float OffsetX = 0.0f;
        public float YPosStart = 0.0f;
        public float YPosEnd = 0.0f;
        public float Width = 0.0f;
        public float Length = 0.0f;
        public float XPosStart = 0.0f;
        public float XPosEnd = 0.0f;

        // 후처리 FaltData 연산 갯수
        // DB 데이터와는 상관 없음
        public int RollCtlCnt = 0;

        // 화면 표시용 Y0KLOT 추가
        public string Y0KLOT { get; set; }

        // 검사 라인 확인용 LNCD 추가
        public string LNCD { get; set; }

        public void Parse(OracleDataReader reader)
        {
            CTLNO = reader[0].ToString();
            HINMEI = reader[2].ToString();
            LOTNO = reader[3].ToString();
            ROLLNO = reader[4].ToString();
            BCNO = reader[6].ToString();
            YPosStart = float.Parse(reader[7].ToString());
            YPosEnd = float.Parse(reader[8].ToString());
            Width = float.Parse(reader[13].ToString());
            XPosStart = float.Parse(reader[14].ToString());
            XPosEnd = float.Parse(reader[15].ToString());
            Length = float.Parse(reader[18].ToString());
            STRDT = reader[21].ToString();
            STRTM = reader[22].ToString();
            ENDDT = reader[23].ToString();
            ENDTM = reader[24].ToString();
            USEFLG = reader[34].ToString();
            CUSTCD = reader[42].ToString();
            KYCD = reader[46].ToString();
            KTCD = reader[47].ToString();
        }

        public override string ToString()
        {
            string msg = String.Format($"{CTLNO}\t\t{HINMEI}\t\t{Y0KLOT}\t\t{LOTNO}\t\t{BCNO}\t\t{YPosStart:F2}\t\t{YPosEnd:F2}\t\t{Width:F2}\t\t{Length:F2}\t\t{STRDT}\t\t{STRTM}\t\t{ENDDT}\t\t{ENDTM}");
            return msg;
        }

        public INSPDATData Clone()
        {
            INSPDATData data = new INSPDATData();

            data.CTLNO = this.CTLNO;
            data.HINMEI = this.HINMEI;
            data.LOTNO = this.LOTNO;
            data.ROLLNO = this.ROLLNO;
            data.BCNO = this.BCNO;
            data.STRDT = this.STRDT;
            data.STRTM = this.STRTM;
            data.ENDDT = this.ENDDT;
            data.ENDTM = this.ENDTM;
            data.USEFLG = this.USEFLG;
            data.CUSTCD = this.CUSTCD;
            data.KYCD = this.KYCD;
            data.KTCD = this.KTCD;

            return data;
        }
    }

    public class FLTDATAData
    {
        // Search Item = YLMLOT(점착LOT)
        //	1		2		3           4           5       6       7		8       9		 10         11	      12        13
        //	CTLNO	FLTNO	OFFSET      YPOS_M	    XPOS_M	AREA_M  RANK	KND     CAMNO	 MNTTAN		JIGCD	  MACNO     FLTID
        //	관리No	결점No	원단장위치   흐르는 방향   폭방향   면적mm  랭크    종류수    카메라No  보수담당    사무실코드 검사장치No  결점ID

        public string CTLNO;
        public string FLTNO;
        public float OFFSET;
        public float YPOS_M;
        public float XPOS_M;
        public float AREA_M;
        public string RANK;
        public string KND;
        public int CAMNO;
        public string MNTTAN;
        public string JIGCD;
        public string MACNO;
        public string FLTID;

        // 속도 개선 후 데이터 검색 처리
        public void Parse(OracleDataReader reader)
        {
            float fVal;
            CTLNO = reader[0].ToString();
            FLTNO = reader[1].ToString();
            if (float.TryParse(reader[2].ToString(), out fVal) == true)
                OFFSET = fVal;
            if (float.TryParse(reader[3].ToString(), out fVal) == true)
                YPOS_M = fVal;
            if (float.TryParse(reader[4].ToString(), out fVal) == true)
                XPOS_M = fVal;
            if (float.TryParse(reader[5].ToString(), out fVal) == true)
                AREA_M = fVal;
            RANK = reader[6].ToString();
            KND = reader[7].ToString();
            if (Int32.TryParse(reader[8].ToString(), out int iVal) == true)
                CAMNO = iVal;
            MNTTAN = reader[9].ToString();
            JIGCD = reader[10].ToString();
            MACNO = reader[11].ToString();
            FLTID = reader[12].ToString();
        }

        public string GetString(int index, int defectLine, string bcno, float xOffset)
        {
            string msg = String.Format($"{index}\t-\t{CTLNO}, {FLTNO}, {OFFSET:0.00}, {YPOS_M:0.00}, {XPOS_M:0.00}, {RANK}, {KND}, {JIGCD}, {MACNO} , " +
                $"{FLTID}, {AREA_M:0.00}, {CAMNO}, {MNTTAN}, {defectLine}, {bcno}, {xOffset}");
            return msg;
        }

        public string GetString(int index, string bcno)
        {
            string msg = String.Format($"{index}\t-\t{CTLNO}, {FLTNO}, {OFFSET:0.00}, {YPOS_M:0.00}, {XPOS_M:0.00}, {RANK}, {KND}, {JIGCD}, {MACNO} , " +
                $"{FLTID}, {AREA_M:0.00}, {CAMNO}, {MNTTAN}, {bcno}");
            return msg;
        }
    }

    public class FLTDATA_DailyData
    {
        // Search Item = YLMLOT(점착LOT)
        //	1	   2		3		4           5		    6		7       8		9		10      11      12		13		14		15		16		17		18		    19		    20		
        //	CTLNO  FLTNO	DT	    TM  	    OFFSET      YPOS_M	YPOS_P	XPOS_M	XPOS_P	AREA_M	AREA_P  LEN_M	LEN_P	WID_M	WID_P	RANK	KND 	S_SLIT	    E_SLIT	    CMT	
        //	관리NO  결점No	발생일   발생시각     원단장위치    흐르는 방향      폭방향           면적mm  면적pxl  길이mm  길이pxl  길이mm   길이pxl 랭크     종류수   개시슬리터   완료 슬리터   코멘트
        //================================================================================================================================================================================
        //  21		22		23		   24		  25		 26	     27	    28	      29	     30	        31	        32	      33	    34	        35	   36	        
        //  CAMNO	MAGNIF	PICFLG	   PICSIZE	  PICFNAME   MNTINF	 MNTDT	MNTTM	  MNTTAN	 JIGCD	    MACNO	    GRANK	  PICTYPE	PICFSIZE    FLTID  FLTORG		
        //  카메라No Y배율   화상유무     화상크기    화상필름명  보수정보 보수일  보수시각    보수담당    사무실코드  검사장치No   외관 랭크   화상 타입  화상사이즈   결점ID  결점ID(검사장치)

        public string CTLNO;
        public string FLTNO;
        public float OFFSET;
        public float YPOS_M;
        public float XPOS_M;
        public float AREA_M;
        public string RANK;
        public string KND;
        public int CAMNO;
        public string MNTTAN;
        public string JIGCD;
        public string MACNO;
        public string FLTID;

        //속도 올리기 전 버전 DATA 처리
        public void Parse(OracleDataReader reader)
        {
            float fVal;
            CTLNO = reader[0].ToString();
            FLTNO = reader[1].ToString();
            if (float.TryParse(reader[4].ToString(), out fVal) == true)
                OFFSET = fVal;
            if (float.TryParse(reader[5].ToString(), out fVal) == true)
                YPOS_M = fVal;
            if (float.TryParse(reader[7].ToString(), out fVal) == true)
                XPOS_M = fVal;
            if (float.TryParse(reader[9].ToString(), out fVal) == true)
                AREA_M = fVal;
            RANK = reader[15].ToString();
            KND = reader[16].ToString();
            if (Int32.TryParse(reader[20].ToString(), out int iVal) == true)
                CAMNO = iVal;
            MNTTAN = reader[28].ToString();
            JIGCD = reader[29].ToString();
            MACNO = reader[30].ToString();
            FLTID = reader[34].ToString();
        }

        public string GetString(int index, int defectLine, string bcno, float xOffset)
        {
            string msg = String.Format($"{index}\t-\t{CTLNO}, {FLTNO}, {OFFSET:0.00}, {YPOS_M:0.00}, {XPOS_M:0.00}, {RANK}, {KND}, {JIGCD}, {MACNO} , " +
                $"{FLTID}, {AREA_M:0.00}, {CAMNO}, {MNTTAN}, {defectLine}, {bcno}, {xOffset}");
            return msg;
        }

        public string GetString(int index, string bcno)
        {
            string msg = String.Format($"{index}\t-\t{CTLNO}, {FLTNO}, {OFFSET:0.00}, {YPOS_M:0.00}, {XPOS_M:0.00}, {RANK}, {KND}, {JIGCD}, {MACNO} , " +
                $"{FLTID}, {AREA_M:0.00}, {CAMNO}, {MNTTAN}, {bcno}");
            return msg;
        }
    }

    #endregion DB Query

    #region DB Search Result
    public class DbSearchResult
    {
        public List<PTRYLPdata> PTRLYP_Data;
        public List<PTRY0PData>[] PTRY0P_Data;
        public List<MRKCTLMSTData> MRKCTLMST_Data;
        public List<List<INSPDATData>>[] INSPDAT_Data;
        public List<XOFSMSTData> XOFSMST_Data;
        public List<AREADELData> AREADEL_Data;

        public List<Dictionary<string, float>>[] dicSizeMRKCTLMST;
        public List<Dictionary<string, bool>>[] dicMRKF1MRKCTLMST;
        public List<MRKCTLMST_DE_Data>[] _MRKCTLMST_DE;

        public Dictionary<string, float> dicSizeData;
        public Dictionary<string, bool> dicMRKF1Data;

        public Dictionary<string, int> DicCSVDefectCnt;

        public List<string> LoadedBcNo;

        public List<DateTime> ProductEndTime;
        public List<string> ProductLotName;


        // 현재 생산중인 Lot의 이전 공정 데이터
        public List<INSPDATData>[] Matched_INSPDAT_Data;

        public DbSearchResult()
        {
            Init();
        }

        public void Init()
        {
            PTRLYP_Data = new List<PTRYLPdata>();
            XOFSMST_Data = new List<XOFSMSTData>();
            AREADEL_Data = new List<AREADELData>();

            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            PTRY0P_Data = new List<PTRY0PData>[count];
            for (int i = 0; i < count; i++)
                PTRY0P_Data[i] = new List<PTRY0PData>();

            INSPDAT_Data = new List<List<INSPDATData>>[count];
            for (int i = 0; i < count; i++)
                INSPDAT_Data[i] = new List<List<INSPDATData>>();

            MRKCTLMST_Data = new List<MRKCTLMSTData>();
            dicSizeMRKCTLMST = new List<Dictionary<string, float>>[count];
            dicMRKF1MRKCTLMST = new List<Dictionary<string, bool>>[count];
            _MRKCTLMST_DE = new List<MRKCTLMST_DE_Data>[count];
            for (int i = 0; i < count; i++)
            {
                dicSizeMRKCTLMST[i] = new List<Dictionary<string, float>>();
                dicMRKF1MRKCTLMST[i] = new List<Dictionary<string, bool>>();
                _MRKCTLMST_DE[i] = new List<MRKCTLMST_DE_Data>();
            }

            dicSizeData = new Dictionary<string, float>();
            dicMRKF1Data = new Dictionary<string, bool>();

            LoadedBcNo = new List<string>();

            ProductEndTime = new List<DateTime>();
            ProductLotName = new List<string>();

            DicCSVDefectCnt = new Dictionary<string, int>();

            Matched_INSPDAT_Data = new List<INSPDATData>[count];
            for (int i = 0; i < count; i++)
                Matched_INSPDAT_Data[i] = new List<INSPDATData>();
        }

        public void ClearAll ()
        {
            LoadedBcNo.Clear();
            AREADEL_Data.Clear();
            XOFSMST_Data.Clear();
            PTRLYP_Data.Clear();
            MRKCTLMST_Data.Clear();

            LoadedBcNo.Clear();

            ProductEndTime.Clear();
            ProductLotName.Clear();

            for (int i = 0; i < PTRY0P_Data.Length; i++) PTRY0P_Data[i].Clear();

            for (int i = 0; i < INSPDAT_Data.Length; i++)
            {
                for (int j = 0; j < INSPDAT_Data[i].Count; j++)
                    INSPDAT_Data[i][j].Clear();
                INSPDAT_Data[i].Clear();
            }

            DicCSVDefectCnt.Clear();


            for (int i = 0; i < Matched_INSPDAT_Data.Length; i++)
            {
                for (int j = 0; j < Matched_INSPDAT_Data[i].Count; j++)
                    Matched_INSPDAT_Data[i].Clear();
            }
        }

        public void ClearProductInfo()
        {
            LoadedBcNo.Clear();
            ProductEndTime.Clear();
            ProductLotName.Clear();
        }

        public void CheckAndUpdateProductInfo(int i, int j, DateTime edTime)
        {
            if (PTRY0P_Data[i][j].Y0KLOT.Substring(0, 2).ToUpper() == "LL")
            {
                ProductEndTime.Add(edTime);
                ProductLotName.Add(PTRY0P_Data[i][j].Y0KLOT);
            }
        }
        public bool IsProductAvaliable(Option opt, LogDB log)
        {
            DateTime dt = DateTime.Now;
            Debug.Assert(ProductEndTime.Count == ProductLotName.Count);
            string strE, strC, str;
            strC = dt.ToString("yyyyMMddHHmmdd");

            int count = 0;

            TimeSpan refTs = new TimeSpan(0, opt.prodAvaliableSpan, 0, 0);
            foreach (DateTime time in ProductEndTime)
            {
                TimeSpan sp = dt - time;
                strE = time.ToString("yyyyMMddHHmmdd");
                str = string.Format($"{opt.dbWhen.ToString()}, {ProductLotName[count]}, 현재시각:{strC}, " +
                    $"생산종료시각:{strE}, 설정시간:{opt.prodAvaliableSpan}, 차이시간:{sp.Hours}");
                log.WriteLoadData(str, 0, "PRODUCTION_ABLE", 0);
                count++;

                if (sp < refTs)
                {
                    log.WriteLoadData("생산가능시간 NG", 0, "PRODUCTION_ABLE", 0);
                    return false;
                }
            }
            return true;
        }

        public void ClearSplit()
        {
            PTRLYP_Data.Clear();
            AREADEL_Data.Clear();
            XOFSMST_Data.Clear();
            MRKCTLMST_Data.Clear();

            for (int i = 0; i < PTRY0P_Data.Length; i++) PTRY0P_Data[i].Clear();

            for (int i = 0; i < INSPDAT_Data.Length; i++)
            {
                for (int j = 0; j < INSPDAT_Data[i].Count; j++)
                    INSPDAT_Data[i][j].Clear();
                INSPDAT_Data[i].Clear();
            }
        }
        public void ResetData_DE()
        {
            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            for (int i = 0; i < count; i++)
            {
                for (int j = 0; j < dicSizeMRKCTLMST[i].Count; j++)
                    dicSizeMRKCTLMST[i][j].Clear();

                for (int j = 0; j < dicMRKF1MRKCTLMST[i].Count; j++)
                    dicMRKF1MRKCTLMST[i][j].Clear();

                _MRKCTLMST_DE[i].Clear();
            }
        }

        public void ClearDicMRKCTLMST(int fdIdx, int ptryoIdx)
        {
            dicSizeMRKCTLMST[fdIdx][ptryoIdx].Clear();
            dicMRKF1MRKCTLMST[fdIdx][ptryoIdx].Clear();
        }
        public void CheckDicMRKCTLMSTSize(int targetCnt, int fcdIdx)
        {
            // Dic 부족한 인덱스 만큼 초기화 처리
            if (targetCnt > dicSizeMRKCTLMST[fcdIdx].Count)
                for (int dicIdx = dicSizeMRKCTLMST[fcdIdx].Count; dicIdx < targetCnt; dicIdx++)
                    dicSizeMRKCTLMST[fcdIdx].Add(new Dictionary<string, float>());

            if (targetCnt > dicMRKF1MRKCTLMST[fcdIdx].Count)
                for (int dicIdx = dicMRKF1MRKCTLMST[fcdIdx].Count; dicIdx < targetCnt; dicIdx++)
                    dicMRKF1MRKCTLMST[fcdIdx].Add(new Dictionary<string, bool>());
        }

        public void AddDicMRKCTLMST(int fdIdx, int ptryoIdx, MRKCTLMSTData data)
        {
            if (dicSizeMRKCTLMST[fdIdx][ptryoIdx].ContainsKey(data.FLTID) == true)
                dicSizeMRKCTLMST[fdIdx][ptryoIdx][data.FLTID] = data.SIZE;
            else
                dicSizeMRKCTLMST[fdIdx][ptryoIdx].Add(data.FLTID, data.SIZE);

            int valMRKF1 = Int32.Parse(data.MRKF1);
            bool boolMRKF1 = false;
            if (valMRKF1 == 1) boolMRKF1 = true;
            if (dicMRKF1MRKCTLMST[fdIdx][ptryoIdx].ContainsKey(data.FLTID) == true)
                dicMRKF1MRKCTLMST[fdIdx][ptryoIdx][data.FLTID] = boolMRKF1;
            else
                dicMRKF1MRKCTLMST[fdIdx][ptryoIdx].Add(data.FLTID, boolMRKF1);
        }

        public void UpdateDicSizeData(int fcd, int op)
        {
            dicSizeData.Clear();
            foreach (KeyValuePair<string, float> pair in dicSizeMRKCTLMST[fcd][op])
                dicSizeData.Add(pair.Key, pair.Value);
        }

        public void UpdateDicMRKF1Data(int fcd, int op)
        {
            dicMRKF1Data.Clear();
            foreach (KeyValuePair<string, bool> pair in dicMRKF1MRKCTLMST[fcd][op])
                dicMRKF1Data.Add(pair.Key, pair.Value);
        }

        public bool CheckValidSize(string key, float size)
        {
            bool bValid = false;
            if (dicSizeData.ContainsKey(key) == true && dicMRKF1Data.ContainsKey(key) == true)
            {
                // 원래는 사이즈 비교만 하였으나 Defect Edit 시 버퍼 내용에서 마킹 체크가 변경이 가능하기 때문에 
                // MRKF1도 확인해서 비교하도록 해야함. 
                if (dicSizeData[key] <= (size + 0.00001f) && dicMRKF1Data[key] == true) 
                    bValid = true;
            }
            
            return bValid;
        }
    }
    #endregion
}
