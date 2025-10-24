using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{

    #region Compare Result Data
    public class CompareResult
    {
        public MarkingFaultDatum Base { get; set; } = null;

        /// <summary>
        /// 각 공정에 대한 비교 데이터
        /// 0: 공정 인덱스
        /// 1: 불량 갯수
        /// </summary>

        public Dictionary<(string, string), List<MarkingFaultDatum>[]> Comp { get; set; } = null;

        public int IdxSize { get; set; }

        public CompareResult()
        {
            Base = new MarkingFaultDatum();

            Comp = new Dictionary<(string, string), List<MarkingFaultDatum>[]>();
        }

        public void AddCompData(string lncd, string ctrno, int idx, List<MarkingFaultDatum> data)
        {
            var key = (lncd, ctrno);

            // 키가 없으면 초기화
            if (!Comp.ContainsKey(key))
            {
                var listArray = new List<MarkingFaultDatum>[IdxSize];
                for (int i = 0; i < IdxSize; i++)
                    listArray[i] = new List<MarkingFaultDatum>();
                Comp[key] = listArray;
            }

            // 예외 방지: idx 유효성 확인
            if (idx < 0 || idx >= IdxSize)
                throw new IndexOutOfRangeException($"인덱스 {idx}는 유효하지 않습니다. 0 ~ {IdxSize - 1} 사이여야 합니다.");

            foreach( var item in data)
                Comp[key][idx].Add(item);
        }

    }
    #endregion

    public class MarkCompData
    {
        public List<CompareResult> Data { get; set; }

        public List<string>[] CTLNO { get; set;}

        public CompareResult this[int idx]
        {
            get { return Data[idx]; }
            set { Data[idx] = value; }  
        }

        public MarkCompData()
        {
            Data = new List<CompareResult>();
        }

        public void Add(CompareResult item)
        {
            Data.Add(item);
        }

        public void SetCTLNOArray(int size)
        {
            CTLNO = new List<string>[size];

            for(int i=0; i<size; i++)
            {
                CTLNO[i] = new List<string>();
            }
        }
    }
}
