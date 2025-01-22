using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager.DB
{
    #region PTRYLPdata
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
        public float YLMTON;    //g3 투입M
        public float YLMKAS;    //g4 완성M
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

        public void Parse(string reader)
        {
            string[] dummy = reader.Split('\t');
            string[] data = dummy[2].Split(',');

            YLMLOT = data[0].Trim(' ');

            YLSZKY = data[1].Trim(' ');
            YLMZKN = data[2].Trim(' ');

            float valF = 0.0f;
            if (float.TryParse(data[3].Trim(' '), out valF) == true)
                YLMTON = valF;
            if (float.TryParse(data[4].Trim(' '), out valF) == true)
                YLMKAS = valF;

            YLMYKH = data[5].Trim(' ');
            YLSLOT = data[6].Trim(' ');
            
            YLSZKN = data[8].Trim(' ');
        }

        //public override string ToString()
        //{
        //    string msg = $"{YLMLOT}, {YLSZKY}, {YLMTON:F3}, {YLMKAS:F3}, {YLMYKH}, {YLSLOT}, {YLSZKN}";
        //    return msg;
        //}

        public override string ToString()
        {
            string msg = $"{YLMLOT}, {YLSZKY}, {YLMZKN}, {YLMTON:F3}, {YLMKAS:F3}, {YLMYKH}, {YLSLOT}, {YLSZKY}, {YLSZKN}";
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

    public class PTRYLPList : ItemList<PTRYLPdata>
    {
        public PTRYLPList()
        {
            this.Data = new List<PTRYLPdata>();
        }

        public void Copy(List<PTRYLPdata> input)
        {
            _data.Clear();
            foreach (PTRYLPdata lpData in input)
                _data.Add(lpData.Clone());
        }

        public void Copy(PTRYLPList input)
        {
            _data.Clear();
            foreach (PTRYLPdata lpData in input.Data)
                _data.Add(lpData.Clone());
        }
    }
    #endregion PTRYLPdata
}
