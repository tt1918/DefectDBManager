using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager.DB
{
    #region INSPDATData
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

        //public override string ToString()
        //{
        //    string msg = String.Format($"{CTLNO}\t\t{HINMEI}\t\t{Y0KLOT}\t\t{LOTNO}\t\t{BCNO}\t\t{YPosStart:F2}\t\t{YPosEnd:F2}\t\t{Width:F2}\t\t{Length:F2}\t\t{STRDT}\t\t{STRTM}\t\t{ENDDT}\t\t{ENDTM}");
        //    return msg;
        //}

        public override string ToString()
        {
            string msg = String.Format($"{CTLNO}, {HINMEI}, {LOTNO}, {ROLLNO}, {BCNO}, {YPosStart}, {YPosEnd}, {Width}, {XPosStart}, {XPosEnd}, {Length}, {STRDT}, {STRTM}," +
                $" {ENDDT}, {ENDTM}, {USEFLG}, {CUSTCD}, {KYCD}, {KTCD}");
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
            data.LNCD = this.LNCD;
            data.YPosStart = this.YPosStart;
            data.YPosEnd = this.YPosEnd;

            return data;
        }
    }

    public class INSPDATList : ItemList<INSPDATData>
    {
        public INSPDATList()
        {
            _data = new List<INSPDATData>();
        }
        public void Copy(List<INSPDATData> input)
        {
            foreach (INSPDATData lpData in input)
                _data.Add(lpData.Clone());
        }

        public void Copy(INSPDATList input)
        {
            foreach (INSPDATData lpData in input.Data)
                _data.Add(lpData.Clone());
        }
    }
    #endregion INSPDATData
}
