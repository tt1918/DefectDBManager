using DefectDBManager.DB;
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
    public interface IPreprocLot
    {
        string LotName { get; set; }

        string ProcName { get; set; }

        PTRYLPList PTRLYP_Data { get; }
        XOFSMSTList XOFSMST_Data { get; }
        PTRY0PList[] PTRY0P_Data { get; }
        List<INSPDATList>[] INSPDAT { get; }

        /// <summary>
        /// 결점 정보 데이터
        /// </summary>
        PreProcResultData FaultData { get; }

        /// <summary>
        /// 마킹 오차 비교 데이터
        /// </summary>
        MarkCompData MarkCompList { get; set; }

        eCompResult CompResult { get; }

        List<int[,]> CompCnt { get; }

        Preproc.LOT.CompResult LotSummary { get; }

        void Init();

        void SetQueryResult(DBLotInfo.LotData result);

        void SetFaultData(PreProcResultData data);

        void ComparePosition();
    }
}
