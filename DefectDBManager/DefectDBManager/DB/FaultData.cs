using System;
using System.Collections.Generic;
using System.Drawing;
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

    public class PrePocResultData
    {
        /// <summary>
        /// 상위 보고용 데이터
        /// </summary>
        public List<PointF> DefectPt = null;
        public Dictionary<int, List<PointF>> DicPt = null;

        /// <summary>
        /// FAULTData 저장
        /// 데이터는 각 공정 및 LNCD 기준으로 처리하도록 한다. 
        /// </summary>
        public List<PreProcDefect>[] FLTDAT
        {
            get { return _fltdat; }
            private set { _fltdat = value; }
        }
        private List<PreProcDefect>[] _fltdat;


        public List<MarkingFaultDatum> DispData
        {
            get { return _dispData; }
            private set { _dispData = value; }
        }
        private List<MarkingFaultDatum> _dispData;

        // 현재 생산하고 있는 BCNO
        public string BCNO
        {
            get { return _bcno; }
            set { _bcno = value; }
        }
        private string _bcno;

        public PrePocResultData()
        {
            DefectPt = new List<PointF>();
            DicPt = new Dictionary<int, List<PointF>>();

            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            _fltdat = new List<PreProcDefect>[count];
            for (int i = 0; i < count; i++)
                _fltdat[i] = new List<PreProcDefect>();

            _dispData = new List<MarkingFaultDatum>();
        }

        ~PrePocResultData()
        {
            ResetAll();
        }

        public void ResetAll()
        {
            // 문자열 초기화
            _bcno = "";
            
            // 실시간 불량 전송용 데이터 
            DefectPt.Clear();
            DicPt.Clear();


            for (int i = 0; i < _fltdat.Length; i++)
            {
                // 내부 데이터 삭제
                for (int j = 0; j < _fltdat[i].Count; j++)
                    _fltdat[i][j].ResetAll();

                // 공정 별 데이터 리스트 삭제
                _fltdat[i].Clear();
            }

            _dispData.Clear();
        }

        public void Add(MarkingFaultDatum data)
        {
            // 10M 단위로 데이터 자름
            int key = (int)(data.OFFSET / 10000.0);
            
            PointF pt = new PointF();
            pt.X = data.XPOS_M;
            pt.Y = (float)data.OFFSET;

            if (DicPt.ContainsKey(key) == true)
                DicPt[key].Add(pt);
            else
                DicPt[key] = new List<PointF> { pt };

            _dispData.Add(data);
        }

        public List<PointF> GetDefectPts(string bcno, float startY, float endY)
        {
            // BCNO가 다르면 다시 탐색해야 함. 
            if (bcno != _bcno) return null;

            List<PointF> pts = new List<PointF>();
            int key1 = (int)(startY / 10000.0);
            int key2 = (int)(endY / 10000.0);

            for (int i = key1; i <= key2; i++)
            {
                if (DicPt.ContainsKey(i) == true)
                {
                    foreach (PointF pt in DicPt[i])
                    {
                        if (pt.Y >= startY && pt.Y <= endY)
                            pts.Add(pt);
                    }
                }
            }
            return pts;
        }
    }


    public class PreProcDefect
    {
        // 이전 공정 라인 코드
        public string LNCD;
        public List<FaultDatum> Data;

        public PreProcDefect()
        {
            Data = new List<FaultDatum>();
        }

        ~PreProcDefect()
        {
            ResetAll();
        }

        public void ResetAll()
        {
            Data.Clear();
        }
    }
}
