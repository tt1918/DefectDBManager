using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.Linq;
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
        //1			2		3		4		5		6		7		8		9		10		11		12		13		14		15		16		17		18		19		20		21		22		23		24		25		26		27		28		29		30		31
        //YLMLOT	YLMKYC	YLMMAC	YLMPPC	YLMKNC	YLMKSB	YLMZKY	YLMZKN	YLMSAG	YLMTON	YLMKAS	YLMYKH	YLMSOK	YLYIEL	YLLEVE	YLSLOT	YLSSEQ	YLSGEB	YLSZKB	YLSZKY	YLSZKN	YLSTON	YLYSKH	YLCRDT	YLCRTM	YLOPDT	YLRPTM	YLNMID	YLLGID	YLPCID	YLPGID
        //점착LOT	x		x		x		x		x		품종	    x		x		투입M	완성M	원단폭	X		X		X		연신LOT	X		X		X		X-----------------------------------------
        //20												X						8		5		5								42				42				40		42

        public string YLMLOT;   //g1 점착lot
        public string YLMZKY;   //g8 품종
        public string YLMZKN;   //g9 이전 프로그램에서는 사용 안 함
        public float YLMTON;    //g3 투입M
        public float YLMKAS;    //g4 완성M
        public string YLMYKH;   //g5 원단폭
        public string YLSLOT;   //g6 점착LOT <--연신Lot
        public string YLSZKY;   //g2 품명
        public string YLSZKN;   //g7 원래 프로그램에서 데이터 넣는 부분 이름이 이상함 데이터. 데이터 읽어오는것 외에 사용 안 함 

        public void Parse(OracleDataReader reader)
        {
            YLMLOT = reader[1].ToString();

            YLMZKY = reader[7].ToString();
            YLMZKN = reader[8].ToString();

            float valF = 0.0f;
            if (float.TryParse(reader[10].ToString(), out valF) == true)
                YLMTON = valF;
            if (float.TryParse(reader[11].ToString(), out valF) == true)
                YLMKAS = valF;

            YLMYKH = reader[12].ToString();
            YLSLOT = reader[16].ToString();
            YLSZKY = reader[20].ToString();
            YLSZKN = reader[21].ToString();
        }

        public override string ToString()
        {
            string msg = $"{YLMLOT}, {YLSZKY}, {YLMTON:F3}, {YLMKAS:F3}, {YLMYKH}, {YLSLOT}, {YLSZKN}";
            return msg;
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
            KYCD = reader[1].ToString();
            PPCD = Int32.Parse(reader[2].ToString());
            LNCD = reader[3].ToString();
            YLMZKN2 = reader[4].ToString();
            YLSZKN = reader[5].ToString();
            X_OFFSET = float.Parse(reader[6].ToString());
        }

        public override string ToString()
        {
            string msg = $"{KYCD}, {PPCD}, {LNCD}, {YLMZKN2}, {YLSZKN}, {X_OFFSET:F3}";
            return msg;
        }
    }

    public class AREADELData
    {
        //1			2		3		4		5			6			7			8				9				10			11
        //KYCD		PPCD	LNCD	LOTNO	ENTRY_NUM	STR_WD		END_WD		STR_MD			END_MD			DELFLG		TAKNDTM	
        //거점		공정	라인	롯		일련번호	시작폭좌표	종료폭좌표	시작흐름좌표	종료흐름좌표	삭제플러그	읽기일시		
        //3			3		4		30		2			7.2			7.2			11.2			11.2			1			14

        public string KYCD;
        public string PPCD;
        public string LNCD;
        public string LOTNO;
        public string ENTRY_NUM;
        public float STR_WD;
        public float END_WD;
        public float STR_MD;
        public float END_MD;

        public void Parse(OracleDataReader reader)
        {
            KYCD = reader[1].ToString();
            PPCD = reader[2].ToString();
            LNCD = reader[3].ToString();
            LOTNO = reader[4].ToString();
            ENTRY_NUM = reader[5].ToString();

            float ret = 0.0f;
            if (float.TryParse(reader[6].ToString(), out ret)) STR_WD = ret;
            else STR_WD = 0.0f;

            if (float.TryParse(reader[7].ToString(), out ret)) END_WD = ret;
            else END_WD = 0.0f
                    ;
            if (float.TryParse(reader[8].ToString(), out ret)) STR_MD = ret;
            else STR_MD = 0.0f;

            if (float.TryParse(reader[9].ToString(), out ret)) END_MD = ret;
            else END_MD = 0.0f;
        }

        public override string ToString()
        {
            string msg = $"{KYCD}, {PPCD}, {LNCD}, {LOTNO}, {ENTRY_NUM}, {STR_WD:F3}, {END_WD:F3}, {STR_MD:F3}, {END_MD:F3}";
            return msg;
        }
    }

    public class PTRY0PData
    {
        //	1		2		3		4		5		6		7		8		9		10		11		12		13		14		15
        //	거점		제부		공정		작업구분	품종		기계		색코드	원단LOT	라인		라인식별	원단명칭	개시		종료		완성M	랏품종		
        //	Y0KYCD	Y0MACD	Y0PPCD	Y0SAGK	Y0HINC	Y0KIKC	Y0IROC	Y0KLOT	Y0LNCD	Y0LNSN	Y0ZKNM	Y0KKOL	Y0KSOL	Y0KASS	Y0ZKNM2
        //	3		3		3		2		5		5		2		20		4		4		80		14		14		5		100
        //	x		x		x		x		o		x		x		o		x		o		x		o		o		x		o

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
            LotData = reader[10].ToString();

            StartTime = reader[12].ToString();
            int pos = StartTime.IndexOf(' ');
            if (pos > 0)
                StartTime = StartTime.Substring(0, pos);

            EndTime = reader[13].ToString();
            pos = EndTime.IndexOf(' ');
            if (pos > 0)
                EndTime = EndTime.Substring(0, pos);

            Y0KYCD = reader[1].ToString();
            Y0KLOT = reader[8].ToString();
            LNCD = reader[9].ToString();
            Y0LNSN = reader[10].ToString();
            Y0ZKNM = reader[11].ToString();
            Y0KKOL = reader[12].ToString();
            Y0KSOL = reader[13].ToString();
            Y0KASS = reader[14].ToString();
        }

        public override string ToString()
        {
            string msg = $"{Y0KYCD}, {Y0KLOT}, {LNCD}, {Y0LNSN}, {Y0KKOL} , {Y0KSOL}, {Y0ZKNM}, {Y0KASS}";

            return msg;
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
        public float SIZE;

        // Dic 구현해야함
        // FLTID -> SIZE

        public void Parse(OracleDataReader reader)
        {
            LNCD = reader[4].ToString();
            FLTID = reader[5].ToString();
            ROLLNAME = reader[6].ToString();
            MRKF1 = reader[7].ToString();
            MRKF2 = reader[8].ToString();
            SIZE = float.Parse(reader[9].ToString());
        }

        public override string ToString()
        {
            string msg = $"{LNCD}, {FLTID}, {ROLLNAME}, {MRKF1}, {MRKF2}";
            return msg;
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
        public float TimeInspStart = 0.0f;
        public float TimeInspEnd = 0.0f;
        public float Width = 0.0f;
        public float Length = 0.0f;
        public float YPosStart = 0.0f;
        public float YPosEnd = 0.0f;

        // 후처리 FaltData 연산 갯수
        // DB 데이터와는 상관 없음
        public int RollCtlCnt = 0;

        public void Parse(OracleDataReader reader)
        {
            CTLNO = reader[1].ToString();
            HINMEI = reader[3].ToString();
            LOTNO = reader[4].ToString();
            ROLLNO = reader[5].ToString();
            BCNO = reader[7].ToString();
            TimeInspStart = float.Parse(reader[8].ToString());
            TimeInspEnd = float.Parse(reader[9].ToString());
            Width = float.Parse(reader[14].ToString());
            YPosStart = float.Parse(reader[15].ToString());
            YPosEnd = float.Parse(reader[16].ToString());
            Length = float.Parse(reader[19].ToString());
            STRDT = reader[22].ToString();
            STRTM = reader[23].ToString();
            ENDDT = reader[24].ToString();
            ENDTM = reader[25].ToString();
            USEFLG = reader[35].ToString();
            CUSTCD = reader[43].ToString();
            KYCD = reader[47].ToString();
            KTCD = reader[48].ToString();
        }

        public override string ToString()
        {
            string msg = String.Format($"{CTLNO}\t\t{HINMEI}\t\t{LOTNO}\t\t{BCNO}\t\t{TimeInspStart:F3}\t\t{TimeInspEnd:F3}\t\t{Width:F3}\t\t{YPosStart:F3}\t\t{YPosEnd:F3}\t\t{Length:F3}\t\t{STRDT}\t\t{STRTM}\t\t{ENDDT}\t\t{ENDTM}");
            return msg;
        }
    }

    public class FLTDATAData
    {
        // Search Item = YLMLOT(점착LOT)
        //	1		2		3		4		5		6		7		8		9		10      11		12		13		14		15		16		17		18		    19		    20		
        //	CTLNO	FLTNO	DT	    TM  	OFFSET	YPOS_M	YPOS_P	XPOS_M	XPOS_P	AREA_M	AREA_P	LEN_M	LEN_P	WID_M	WID_P	RANK	KND 	S_SLIT	    E_SLIT	    CMT	
        //	관리No	결점Ｎｏ	발생일   발생시각 원단장위치 흐르는 방향    폭방향          면적mm   면적pxl 길이mm   길이pxl 길이mm  길이pxl  랭크    종류수   개시슬리터   완료 슬리터  코멘트
        //  21		22		23		24		25		26	    27	    28	    29	    30	        31	        32	      33	    34	    35	    36	        
        //  CAMNO	MAGNIF	PICFLG	PICSIZE	PICFNAME MNTINF	MNTDT	MNTTM	MNTTAN	JIGCD	    MACNO	    GRANK	  PICTYPE	PICFSIZE FLTID  FLTORG		
        //  카메라No Y배율   화상유무 화상크기 화상필름명 보수정보 보수일 보수시각 보수담당 사무실코드   검사장치No   외관 랭크  화상 타임  화상사이즈 결점ID 결점ID(검사장치)

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

        public void Parse(OracleDataReader reader)
        {
            CTLNO = reader[1].ToString();
            FLTNO = reader[2].ToString();
            OFFSET = float.Parse(reader[5].ToString());
            YPOS_M = float.Parse(reader[6].ToString());
            XPOS_M = float.Parse(reader[8].ToString());
            AREA_M = float.Parse(reader[10].ToString());
            RANK = reader[16].ToString();
            KND = reader[17].ToString();
            CAMNO = Int32.Parse(reader[21].ToString());
            MNTTAN = reader[29].ToString();
            JIGCD = reader[30].ToString();
            MACNO = reader[31].ToString();
            FLTID = reader[35].ToString();
        }

        public string GetString(int index, int defectLine, string bcno, float xOffset)
        {
            string msg = String.Format($"{index}\t-\t{CTLNO}, {FLTNO}, {OFFSET:0.000}, {YPOS_M:0.000}, {XPOS_M:0.000}, {RANK}, {KND}, {JIGCD}, {MACNO} , " +
                $"{FLTID}, {AREA_M:0.000}, {CAMNO}, {MNTTAN}, {defectLine}, {bcno}, {xOffset}");
            return msg;
        }
    }
    #endregion DB Query

}
