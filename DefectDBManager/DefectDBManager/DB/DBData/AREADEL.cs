using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager.DB
{

    #region AREADELData
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
        public float STR_WD;
        public float END_WD;
        public float STR_MD;
        public float END_MD;
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

    public class AREADELList : ItemList<AREADELData>
    {
        public AREADELList()
        {
            _data = new List<AREADELData>();
        }

        public void Copy(List<AREADELData> input)
        {
            _data.Clear();
            foreach (AREADELData lpData in input)
                _data.Add(lpData.Clone());
        }

        public void Copy(AREADELList input)
        {
            _data.Clear();
            foreach (AREADELData lpData in input.Data)
                _data.Add(lpData.Clone());
        }
    }
    #endregion AREADELData
}
