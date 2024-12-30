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
        public List<MarkingFaultDatum>[,] Comp { get; set; } = null;

        public CompareResult()
        {
            Base = new MarkingFaultDatum();
        }

        public void SetCompRange(int lineCont, int count)
        {
            Comp = new List<MarkingFaultDatum>[lineCont,count];
            for (int i = 0; i < lineCont; i++)
            {
                for (int j = 0; j < count; j++)
                {
                    Comp[i,j] = new List<MarkingFaultDatum>();
                }
            }
        }
        public void AddCompData(int lineIdx, int idx, MarkingFaultDatum data)
        {
            Comp[lineIdx, idx].Add(data);
        }
    }
    #endregion

    public class MarkCompData
    {
        public List<CompareResult> Data { get; set; }

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
    }
}
