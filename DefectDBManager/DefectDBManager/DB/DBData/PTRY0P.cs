using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager.DB
{
    #region PTRY0PData
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
        public string Y0PPCD;
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
            Y0PPCD = reader[2].ToString();
            Y0KLOT = reader[7].ToString();
            LNCD = reader[8].ToString();
            Y0LNSN = reader[9].ToString();
            Y0ZKNM = reader[10].ToString();
            Y0KKOL = reader[11].ToString();
            Y0KSOL = reader[12].ToString();
            Y0KASS = reader[13].ToString();
        }

        public void Parse(string reader)
        {
            string[] dummy = reader.Split('\t');
            string[] data = dummy[2].Split(',');
            LotData = data[0];
            StartTime = data[1].Trim(' ');
            EndTime = data[2].Trim(' ');
            Y0KYCD = data[3].Trim(' ');
            Y0KLOT = data[4].Trim(' ');
            Y0LNSN = data[5].Trim(' ');
            Y0KKOL = data[6].Trim(' ');
            Y0KSOL = data[7].Trim(' ');
            Y0ZKNM = data[8].Trim(' ');
            LNCD = data[9].Trim(' ');
            Y0KASS = data[10].Trim(' ');
        }

        //public override string ToString()
        //{
        //    string msg = $"{Y0KYCD}, {Y0KLOT}, {Y0LNSN}, {Y0KKOL} , {Y0KSOL}, {Y0ZKNM}, {LNCD}, {Y0KASS}";

        //    return msg;
        //}

        public override string ToString()
        {
            string msg = $"{LotData}, {StartTime}, {EndTime}, {Y0KYCD}, {Y0PPCD}, {Y0KLOT}, {Y0LNSN}, {Y0KKOL} , {Y0KSOL}, {Y0ZKNM}, {LNCD}, {Y0KASS}";

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

    public class PTRY0PList : ItemList<PTRY0PData>
    {

        public PTRY0PList()
        {
            _data = new List<PTRY0PData>();
        }

        public void Copy(List<PTRY0PData> input)
        {
            _data.Clear();
            foreach (PTRY0PData lpData in input)
                _data.Add(lpData.Clone());
        }

        public void Copy(PTRY0PList input)
        {
            _data.Clear();
            foreach (PTRY0PData lpData in input.Data)
                _data.Add(lpData.Clone());
        }
    }
    #endregion PTRY0PData
}
