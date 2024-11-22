using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    /// <summary>
    /// 이전 결점 정보 취합 
    /// 서버에서 데이터 참조할 타겟
    /// </summary>
    public class PreDefectInfo
    {
        /// <summary>
        /// Lot 이름
        /// </summary>
        public string LotName { get; set; }

        public List<PTRYLPdata> PTRLYP_Data { get; private set; }
        public List<PTRY0PData>[] PTRY0P_Data { get; private set; }
        public List<List<INSPDATData>>[] INSPDAT_Data { get; private set; }

        /// <summary>
        /// 결점 정보 데이터
        /// </summary>
        public PrePocResultData FaultData { get; private set; }

        /// <summary>
        /// 초기화
        /// </summary>
        public PreDefectInfo()
        {
            Init();
        }

        /// <summary>
        /// 데이터 초기화
        /// </summary>
        /// <param name="lotName"></param>
        /// <param name="dbResult"></param>
        /// <param name="fault"></param>
        public PreDefectInfo(string lotName, DbSearchResult dbResult, PrePocResultData fault)
        {
            Init();

            LotName = lotName;
            SetQueryResult(dbResult);
            SetFaultData(fault);
        }

        public void Init()
        {
            PTRLYP_Data = new List<PTRYLPdata>();

            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            PTRY0P_Data = new List<PTRY0PData>[count];
            for (int i = 0; i < count; i++)
                PTRY0P_Data[i] = new List<PTRY0PData>();

            INSPDAT_Data = new List<List<INSPDATData>>[count];
            for (int i = 0; i < count; i++)
                INSPDAT_Data[i] = new List<List<INSPDATData>>();
        }

        /// <summary>
        /// 쿼리 데이터 카피
        /// </summary>
        /// <param name="result"></param>
        public void SetQueryResult(DbSearchResult result)
        {
            foreach(PTRYLPdata lpData in result.PTRLYP_Data)
                PTRLYP_Data.Add(lpData.Clone());

            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            for (int i = 0; i < count; i++)
            {
                foreach (var data in result.PTRY0P_Data[i])
                    PTRY0P_Data[i].Add(data.Clone());
            }

            for(int i=0; i<count; i++)
            {
                foreach(var data in result.INSPDAT_Data)
                {
                    List<List<INSPDATData>> inspList = new List<List<INSPDATData>>(); 
                    foreach(var data1 in data)
                    {
                        List<INSPDATData> inspList1 = new List<INSPDATData>();
                        foreach(var data2 in data1)
                        {
                            inspList1.Add(data2.Clone());
                        }
                        inspList.Add(inspList1);
                    }
                    INSPDAT_Data[i] = inspList;
                }
            }

        }

        public void SetFaultData(PrePocResultData data)
        {

        }
    }
}
