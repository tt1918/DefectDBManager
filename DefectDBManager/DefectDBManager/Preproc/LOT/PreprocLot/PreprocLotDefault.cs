using DefectDBManager.DB;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    public class PreprocLotDefault : IPreprocLot
    {
        public string LotName { get; set; } = string.Empty;
        public string ProcName { get; set; } = string.Empty;
        public PTRYLPList PTRLYP_Data { get; protected set; } = new PTRYLPList();
        public XOFSMSTList XOFSMST_Data { get; protected set; } = new XOFSMSTList();
        public PTRY0PList[] PTRY0P_Data { get; protected set; } = new PTRY0PList[0];
        public List<INSPDATList>[] INSPDAT { get; protected set; } = new List<INSPDATList>[0];
        public PreProcResultData FaultData { get; protected set; } = null;
        public MarkCompData MarkCompList { get; set; } = null;
        public eCompResult CompResult { get; protected set; } = eCompResult.None;
        public List<int[,]> CompCnt { get; protected set; } = new List<int[,]>();
        public Preproc.LOT.CompResult LotSummary { get; protected set; } = new Preproc.LOT.CompResult();

        public void Init()
        {
            PTRLYP_Data = new PTRYLPList();

            XOFSMST_Data = new XOFSMSTList();

            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            PTRY0P_Data = new PTRY0PList[count];
            for (int i = 0; i < count; i++)
                PTRY0P_Data[i] = new PTRY0PList();

            INSPDAT = new List<INSPDATList>[count];
            for (int i = 0; i < count; i++)
                INSPDAT[i] = new List<INSPDATList>();

            CompResult = eCompResult.None;

            LotSummary = new Preproc.LOT.CompResult();
        }

        public void SetQueryResult(DBLotInfo.LotData result)
        {
            if (result == null) return;
            PTRLYP_Data.Copy(result.PTRLYP);

            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            for (int i = 0; i < count; i++)
            {
                foreach (var data in result.PTRY0P[i].Data)
                    PTRY0P_Data[i].Add(data.Clone());
            }

            int idx = 0;
            foreach (var data in result.INSPDAT)
            {
                List<INSPDATList> inspList = new List<INSPDATList>();
                foreach (var data1 in data)
                {
                    INSPDATList inspList1 = new INSPDATList();
                    foreach (var data2 in data1.Data)
                        inspList1.Add(data2.Clone());
                    inspList.Add(inspList1);
                }
                INSPDAT[idx] = inspList;
                idx++;
            }
            
        }

        public void SetFaultData(PreProcResultData data)
        {
            FaultData = data.Copy();
        }

        public virtual void ComparePosition()
        {
            // Implementation here  
        }
    }
}
