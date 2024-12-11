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
    public class PreprocLot
    {
        /// <summary>
        /// Lot 이름
        /// </summary>
        public string LotName { get; set; }

        public PTRYLPList PTRLYP_Data { get; private set; }
        public XOFSMSTList XOFSMST_Data { get; private set; }
        public PTRY0PList[] PTRY0P_Data { get; private set; }
        public List<List<INSPDATData>>[] INSPDAT_Data { get; private set; }

        /// <summary>
        /// 결점 정보 데이터
        /// </summary>
        public PreProcResultData FaultData { get; private set; }

        /// <summary>
        /// 초기화
        /// </summary>
        public PreprocLot()
        {
            Init();
        }

        /// <summary>
        /// 데이터 초기화
        /// </summary>
        /// <param name="lotName"></param>
        /// <param name="dbResult"></param>
        /// <param name="fault"></param>
        public PreprocLot(string lotName, DbSearchResult dbResult, PreProcResultData fault)
        {
            Init();

            LotName = lotName;
            SetQueryResult(dbResult);
            SetFaultData(fault);
        }

        public void Init()
        {
            PTRLYP_Data = new PTRYLPList();

            XOFSMST_Data = new XOFSMSTList();

            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            PTRY0P_Data = new PTRY0PList[count];
            for (int i = 0; i < count; i++)
                PTRY0P_Data[i] = new PTRY0PList();

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
            PTRLYP_Data.Copy(result.PTRLYP);

            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            for (int i = 0; i < count; i++)
            {
                foreach (var data in result.PTRY0P[i].Data)
                    PTRY0P_Data[i].Add(data.Clone());
            }

            for(int i=0; i<count; i++)
            {
                foreach(var data in result.INSPDAT)
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

        public void SetFaultData(PreProcResultData data)
        {
            FaultData = data;
        }
    }
}
