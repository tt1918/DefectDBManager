using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    // 이 부분 다시 정리해야함

    /// <summary>
    ///  쿼리 탐색 후 결과 데이터 저장
    /// </summary>
    public class FaultDatum
    {
        public string LOTNO;
        public string TLOTNO;
        public string TBCNO;

        public string TCTLNO;
        public string FLTNO;
        public double OFFSET;
        public float YPOS_M;
        public float XPOS_M;
        public float SIZE_X;
        public float SIZE_Y;
        public float SIZE_AREA;     // csv에서 읽어 온 fault area

        public string FLTID;
        public string RANK;
        public string KND;
        public string JIGCD;
        public string MACNO;
        public int CAM_NO;

        //public string CTLNO_FAULT;   <- 마킹 불량 데이터 쪽으로 가는게 맞아보임 
        //public float XOFFSET_ALARM;  <- 마킹 불량 데이터 쪽으로 가는게 맞아보임
        //public float SIZE_FAULT;    // DB에서 받은 fault area <- 마킹 불량 데이터 쪽으로 가는게 맞아보임
        //public string FAULTNO_FAULT; <- 마킹 불량 데이터 쪽으로 가는게 맞아보임
        //public string MNTTID_FAULT; <- 마킹 불량 데이터 쪽으로 가는게 맞아보임
    }


    public class MarkingFaultDatum
    {
        //////////////////////////////////////////////////////////
        // 데이터 전송용
        public string BCNO;
        public string FLTNO;        // DB 탐색에서 얻어오는 불량 순번
        public string FAULTID;      // 불량 항목 같음
        public double OFFSET;
        public float YPOS_M;
        public float XPOS_M;
        public float XOFFSET;
        public bool UseCSVResult;
        public int DefectLine;
        public int CAM_NO;
        // 데이터 전송용
        //////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////
        // 마킹 시 파라미터 확인용
        public float XOFFSET_ALARM;
        // 표시용 데이터
        //////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////
        // 표시용 데이터
        public string CTLNO;
        public string MACNO;
        public string MNTTID;
        public float SIZE;
        public float SIZE_X;
        public float SIZE_Y;
        // 표시용 데이터
        //////////////////////////////////////////////////////////
    }

    public class MarkingFaultData
    {
        /// <summary>
        /// 10M 기준으로 구분하여 데이터 입력함
        /// </summary>
        public List<MarkingFaultDatum> Data = null;
        public Dictionary<int, List<MarkingFaultDatum>> Dic = null;
        public float MinXPos;
        public float MaxXPos;
        public float MinSize;

        

        public MarkingFaultData()
        {
            Data = new List<MarkingFaultDatum>();
            Dic = new Dictionary<int, List<MarkingFaultDatum>>();
        }

        public void Reset()
        {
            Data.Clear();
            Dic.Clear();
            MinXPos = 0.0f;
            MaxXPos = 0.0f;
            MinSize = 0.0f;
        }

        public void Add(MarkingFaultDatum val)
        {
            Data.Add(val);
            int key = (int)(val.OFFSET / 10000.0);
            if (Dic.ContainsKey(key) == true)
                Dic[key].Add(val);
            else
                Dic[key] = new List<MarkingFaultDatum> { val };
        }

        public void GetData(string bcno, double start, double end, ref List<MarkingData> data)
        {
            int key1 = (int)(start / 10000.0);
            int key2 = (int)(end / 10000.0);

            for (int i = key1; i <= key2; i++)
            {
                if (Dic.ContainsKey(i) == true)
                {
                    foreach (MarkingFaultDatum datum in Dic[i])
                    {
                        if (datum.OFFSET >= start && datum.OFFSET <= end/* && datum.BCNO == bcno*/)
                        {
                            MarkingData item = new MarkingData();
                            item.DefectLine = datum.DefectLine;
                            item.BCNO = datum.BCNO;
                            item.CAM_NO = datum.CAM_NO;
                            item.FAULTID = datum.FAULTID;
                            item.FLTNO = datum.FLTNO;
                            item.OFFSET = datum.OFFSET;
                            item.UseCSVResult = datum.UseCSVResult;
                            item.XOFFSET = datum.XOFFSET;
                            item.XPOS_M = datum.XPOS_M;
                            item.YPOS_M = datum.YPOS_M;
                            item.SIZE_X = datum.SIZE_X;
                            item.SIZE_Y = datum.SIZE_Y;
                            data.Add(item);
                        }
                    }
                }
            }
        }
    }

    public class ResultData
    {
        public List<FaultDatum> Data;
        public MarkingFaultData MarkFault;

        public ResultData()
        {
            Data = new List<FaultDatum>();
            MarkFault = new MarkingFaultData();
        }

        ~ResultData()
        {
            ResetAll();
        }

        public void ResetAll()
        {
            Data.Clear();
            MarkFault.Reset();
        }

        
    }
}
