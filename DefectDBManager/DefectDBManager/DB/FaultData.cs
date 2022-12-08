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
        public List<MarkingFaultDatum> Data = new List<MarkingFaultDatum>();

        public float MinXPos;
        public float MaxXPos;
        public float MinSize;

        public void Reset()
        {
            Data.Clear();
            MinXPos = 0.0f;
            MaxXPos = 0.0f;
            MinSize = 0.0f;
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
