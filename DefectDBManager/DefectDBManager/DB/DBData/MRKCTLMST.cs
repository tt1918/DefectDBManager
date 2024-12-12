using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager.DB
{
    #region MRKCTLMSTData
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
        public float SIZE;

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
    public class MRKCTLMSTList : ItemList<MRKCTLMSTData>
    {
        public MRKCTLMSTList()
        {
            _data = new List<MRKCTLMSTData>();
        }

        public void Copy(List<MRKCTLMSTData> input)
        {
            foreach (MRKCTLMSTData lpData in input)
                _data.Add(lpData.Clone());
        }

        public void Copy(MRKCTLMSTList input)
        {
            foreach (MRKCTLMSTData lpData in input.Data)
                _data.Add(lpData.Clone());
        }
    }
    #endregion MRKCTLMSTData

    #region mrkctlmst 변수 (Defect Edit에서 사용)
    public class MRKCTLMST_DE_Data
    {
        public MRKCTLMSTList data;
        public string query { get; set; }

        public MRKCTLMST_DE_Data()
        {
            data = new MRKCTLMSTList();
        }
    }
    #endregion
}
