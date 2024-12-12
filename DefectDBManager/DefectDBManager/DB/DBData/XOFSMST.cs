using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager.DB
{

    #region XOFSMSTData
    public class XOFSMSTData
    {
        //1			2		3		4			5			6			7		8		9			10
        //KYCD		PPCD	LNCD	YLMZKN2		YLSZKN		X_OFFSET	CRDT	CRTM	UPDT		UPTM
        //거점코드	공정코드	라인코드	최종공정품종명	재료로트품종명	폭좌표보정값	작성일	작성시간	업데이트날짜	업데이트시간
        //3			3		4		100			100			3.1			8		6		8			6

        public string KYCD;
        public int PPCD;
        public string LNCD;
        public string YLMZKN2;
        public string YLSZKN;
        public float X_OFFSET;


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

    public class XOFSMSTList : ItemList<XOFSMSTData>
    {
        public XOFSMSTList()
        {
            Data = new List<XOFSMSTData>();
        }

        public void Copy(List<XOFSMSTData> input)
        {
            _data.Clear();
            foreach (XOFSMSTData lpData in input)
                _data.Add(lpData.Clone());
        }

        public void Copy(XOFSMSTList input)
        {
            _data.Clear();
            foreach (XOFSMSTData lpData in input.Data)
                _data.Add(lpData.Clone());
        }
    }
    #endregion XOFSMSTData
}
