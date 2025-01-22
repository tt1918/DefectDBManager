using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager.DB
{
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

        public void Parse(string reader)
        {
            string[] dummy = reader.Split('\t');
            string[] data = dummy[2].Split(',');

            float fVal;
            CTLNO = data[0].Trim(' ');
            FLTNO = data[1].Trim(' ');
            if (float.TryParse(data[2].Trim(' '), out fVal) == true)
                OFFSET = fVal;
            if (float.TryParse(data[3].Trim(' '), out fVal) == true)
                YPOS_M = fVal;
            if (float.TryParse(data[4].Trim(' '), out fVal) == true)
                XPOS_M = fVal;

            RANK = data[5].Trim(' ');
            KND = data[6].Trim(' ');
            JIGCD = data[7].Trim(' ');
            MACNO = data[8].Trim(' ');
            FLTID = data[9].Trim(' ');

            if (float.TryParse(data[10].Trim(' '), out fVal) == true)
                AREA_M = fVal;

            if (Int32.TryParse(data[11].Trim(' '), out int iVal) == true)
                CAMNO = iVal;

            MNTTAN = data[12].Trim(' ');
            
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
}
