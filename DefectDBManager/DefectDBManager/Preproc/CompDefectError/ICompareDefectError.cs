using DefectDBManager.DB;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager.Preproc
{
    public interface ICompareDefectError
    {
        OracleDbConnection Conn { get; }

        DestConfig DbDestConfig
        {
            get;
            set;
        }

        CodeConfig DBCodeConfig
        {
            get;
            set;
        }

        Option DbOption
        {
            get;
            set;
        }

        // 생산 정보 데이터
        PTRY0PList PTRY0PList_Data { get; }

        DBLotInfo.LotData _DbResult { get; set; }

        PreProcResultData FaultData { get; set; }

        IRollDefectInfo _RollDefectInfo { get; set; }
        CSV_DEFECT_HEADER _CsvDefectHeader { get; }
        List<CSVLoadInfo> _CSVLoadInfo { get; }

        string SearchLotName { get; set; }

        #region Filter Param
        /// <summary>
        /// 탐색할 LineCode
        /// </summary>
        string _SearchY0LNCD { get; }
        /// <summary>
        /// 품종 정보
        /// </summary>
        string _ProductName { get; }

        /// <summary>
        /// 랏 탐색 서브 위치 추가
        /// </summary>
        string _SubPath { get; set; }
        #endregion

        LogDB _LOG { get; }

        void ResetDataAll();

        IPreprocLot SearchLot(ref eSearchError errOut);
        IPreprocLot SearchLot_TEST(ref eSearchError errOut);

        bool SearchPTRYOPList();

        bool SearchPTRYOPList_TEST();
    }
}
