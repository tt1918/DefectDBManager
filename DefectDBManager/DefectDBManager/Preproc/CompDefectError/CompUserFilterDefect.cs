using DefectDBManager.DB;
using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager.Preproc
{
    public class CompUserFilterDefect : CompDefectErrorDefault, ICompareDefectError
    {
        public PreprocItem _PreprocItem { get; private set; } = null;
        public AiMonitorItem _AiMonitorItem { get; private set; } = null;
        #region Lot List Search Param
        public string LNCD { get; private set; } = string.Empty;
        public ProcFilter Filter { get; private set; } = null;
        public DateTime StartTime { get; private set; } = DateTime.Now;
        public DateTime EndTime { get; private set; } = DateTime.Now;
        public LotHistory LotHistory { get; private set; } = null;
        public LogDB.eDataType LogMode { get; private set; } = LogDB.eDataType.SearchLot;
        #endregion

        #region Lot Search Param
        public bool Renewal { get; private set; } = false;
        #endregion

        public CompUserFilterDefect(object parent, OracleDbConnection dbconn, bool disconnDB = true)
            : base(parent, dbconn, disconnDB)
        {
            _PreprocItem = new PreprocItem();
            _AiMonitorItem = null;
        }

        public void SetFilterParam(string lncd, string productName, PreprocItem item, AiMonitorItem aiMonitorItem, bool skipWildCard = false)
        {
            _SearchY0LNCD = lncd;
            _ProductName = productName;
            _PreprocItem = item;
            _AiMonitorItem = aiMonitorItem; 

            _SubPath = $"{lncd}_{productName}_{item.Name}";
            _SubPath = Helper.ReplaceInvalidPathChar(_SubPath);

            if (skipWildCard)
            {
                _isWildCard = false;
                return;
            }

            // 품종 wild card 확인
            if (_ProductName.Length < 2)
            {
                _isWildCard = false;
            }
            else
            {
                if (_ProductName.ElementAt(0) == '*' && _ProductName.ElementAt(_ProductName.Length - 1) == '*')
                {
                    _isWildCard = true;

                    // * 은 지우고 필요한 내용만 남김
                    _ProductName = _ProductName.Trim('*');
                }
                else
                {
                    _isWildCard = false;
                }
            }
        }

        public void SetListSearchingParam(string lncd, ProcFilter filter, DateTime startTime, DateTime endTime, LotHistory history, LogDB.eDataType logMode)
        {
            LNCD = lncd;
            Filter = filter;
            StartTime = startTime;
            EndTime = endTime;
            LotHistory = history;
            LogMode = logMode;
        }

        public void SetLotSearchingParam(string lotID, bool renewal, bool bMsgOut, LogDB.eDataType logMode)
        {
            this.LotID = lotID.ToUpper();
            this.Renewal = renewal;
            this.IsMsgOut = bMsgOut;
            this.LogMode = logMode;
        }

        private bool SearchFLTDAT()
        {
            // 연결 확인
            //if (conn?.IsConnected() == false)
            //    return false;

            float maxXPos = 0;
            float minXPos = float.MaxValue;

            bool useXOffset = false;
            bool useAIFromDB = _PreprocItem.UseAiResult;
            bool useAiMonitoring = _PreprocItem.UseAiMonitoring;
            eCSV_TYPE csvType = eCSV_TYPE.NITTO;

            string tmpKey;
            float finalXPos;
            string tmpFaltID;

            INSPDATData inspdata;

            int fcdCnt = System.Enum.GetValues(typeof(eFCD)).Length;
            int dataCnt = 0;
            string query;
            int procStep = 0;
            long dbCnt;
            string logData;

            int[] defectCnt = new int[fcdCnt];
            defectCnt.Initialize();

            float inspStartY = 0.0f;
            float inspEndY = 0.0f;

            bool bValid = false;

            FaultData = new PreProcResultData();

            // 현재 데이터는 마킹 비교 결점 데이터라는 것을 표시함.
            FaultData.IsPreProc = true;

            // 체크 카운트 초기화
            conn.ResetDisconCheck();

            try
            {
                for (int fcdIdx = 0; fcdIdx < fcdCnt; fcdIdx++)
                {
                    // 데이터 초기화
                    maxXPos = 0;
                    minXPos = float.MaxValue;

                    // 
                    procStep = fcdIdx;

                    if (fcdIdx == (int)eFCD.ES) defectCnt[fcdIdx] = 0;// 확인 안 함
                    else if (fcdIdx == (int)eFCD.TG) defectCnt[fcdIdx] = 0;// 확인 안 함
                    else if (fcdIdx == (int)eFCD.ETC) defectCnt[fcdIdx] = 0;// 확인 안 함

                    if (_DbResult.INSPDATArray[fcdIdx] == null) continue;

                    int inspCnt = _DbResult.INSPDATArray[fcdIdx].Count;
                    for (int inspIdx = 0; inspIdx < inspCnt; inspIdx++)
                    {
                        if (_DbResult.INSPDATArray[fcdIdx][inspIdx] == null) continue;

                        inspdata = _DbResult.INSPDATArray[fcdIdx][inspIdx];

                        inspStartY = inspdata.YPosStart;
                        inspEndY = inspdata.YPosEnd;

                        // 매칭 불량 갯수 초기화
                        inspdata.RollCtlCnt = 0;

                        QueryMsg.FLTDAT_Daily_Query msg = new QueryMsg.FLTDAT_Daily_Query();
                        query = msg.GetQuery(inspdata.CTLNO);

                        _LOG.WriteLoadData(_SubPath, query, 0, "FAULTDAT", 0.0);

                        if (query == "")
                        {
                            Log.Write($"[Error] FAULTDAT_{((eFCD)fcdIdx).ToString()} Query message is empty.");
                            return false;
                        }

                        ProcessData mkcdLncdData = null;
                        eProcDataType dataTarget = eProcDataType.None;
                        AiMonitorItem aiItem = null;

                        // LNCD 데이터를 기준으로 Reference/Compare 중에서 선택함. 
                        if (inspdata.LNCD == _PreprocItem.Reference.LNCD)
                        {
                            FaultData.MarkData.LNCD = inspdata.LNCD;
                            mkcdLncdData = _PreprocItem.Reference;
                            dataTarget = eProcDataType.Reference;

                            if (_AiMonitorItem != null)
                            {
                                string filter = _AiMonitorItem.ModelName.Trim('*');
                                if (inspdata.HINMEI.Contains(filter))
                                    aiItem = _AiMonitorItem;
                            }

                            if (aiItem != null)
                            {
                                FaultData.AIMonResult.IsModelExsit = true;
                                foreach (var aimonitor in aiItem.DefectInfo)
                                {
                                    bool isExist = false;
                                    for (int aiIdx = 0; aiIdx < FaultData.AIMonResult.Items.Count; aiIdx++)
                                    {
                                        if (FaultData.AIMonResult[aiIdx].Model != null &&
                                            FaultData.AIMonResult[aiIdx].Model.SECFLTID == aimonitor.SECFLTID)
                                        {
                                            var existFltIds = FaultData.AIMonResult[aiIdx].Model.FLTID ?? new List<string>();
                                            var targetFltIds = aimonitor.FLTID ?? new List<string>();

                                            if (existFltIds.Count == targetFltIds.Count &&
                                                !existFltIds.Except(targetFltIds).Any() &&
                                                !targetFltIds.Except(existFltIds).Any())
                                            {
                                                isExist = true;
                                                break;
                                            }
                                        }
                                    }

                                    if (isExist) continue;
                                    FaultData.AIMonResult.Items.Add(new AiMonitorResultItem()
                                    {
                                        Model = aimonitor,
                                        Total = 0,
                                        Match = 0
                                    });
                                }
                            }
                        }
                        else
                        {
                            bool isFindComp = false;
                            foreach (var compItem in _PreprocItem.Compare)
                            {
                                if (inspdata.LNCD == compItem.LNCD)
                                {
                                    isFindComp = true;
                                    mkcdLncdData = compItem;
                                    dataTarget = eProcDataType.Compare;
                                    break;
                                }
                            }
                            if (isFindComp == false)
                            {
                                mkcdLncdData = null;
                                continue; // 데이터 탐색 안함.
                            }
                        }

                        while (true)
                        {
                            bool isDbError = false;

                            if (conn.Connection.State != ConnectionState.Open)
                                conn.Connect();

                            using (var comm = new OracleCommand(query, conn.Connection))
                            {
                                using (var reader = comm.ExecuteReader(CommandBehavior.SequentialAccess))
                                {
                                    dbCnt = reader.RowSize;

                                    PreprocMrkDat preMarkData = new PreprocMrkDat();
                                    preMarkData.LNCD = inspdata.LNCD;
                                    preMarkData.CTLNO = inspdata.CTLNO;

                                    while (true)
                                    {
                                        if (conn.Connection.State != ConnectionState.Open)
                                        {
                                            isDbError = true;
                                            break;
                                        }
                                        if (reader.Read() == false) break;

                                        FLTDATA_DailyData data = new FLTDATA_DailyData();
                                        data.Parse(reader);

                                        tmpFaltID = data.FLTID.ToUpper();


                                        finalXPos = data.XPOS_M;
                                        if (useXOffset == true) finalXPos += inspdata.OffsetX;
                                        if (useAIFromDB == false) // AI 미사용시
                                        {
                                            tmpKey = data.MNTTAN.TrimStart();
                                            if (string.IsNullOrEmpty(tmpKey))
                                                tmpKey = data.FLTID;
                                        }
                                        else tmpKey = data.FLTID;
                                        
                                        if (dataTarget == eProcDataType.Reference && aiItem != null)
                                        {
                                            FaultData.AIMonResult.AddDefectCnt(data.MNTTAN, data.FLTID);
                                        }

                                        // Log는 무조건 데이터 다 남기도록 수정
                                        dataCnt++;
                                        logData = data.GetString(dataCnt, inspdata.BCNO);
                                        _LOG.WriteLoadData(_SubPath, logData, dataCnt, "FAULTDAT", 0.0);

                                        // MKCD Model에서 데이터 가져와서 다시 탐색함. 
                                        bValid = false;
                                        if (mkcdLncdData != null)
                                        {
                                            bValid = mkcdLncdData.IsValidFLTID(tmpKey, data.AREA_M + 0.00001f);
                                            if (bValid == false) continue;
                                        }
                                        else continue;

                                        if (data.OFFSET < inspStartY || data.OFFSET > inspEndY) continue;
                                        if (finalXPos < 0.0f) continue;

                                        // 전체 데이터를 저장한다. 
                                        // Fault Data 처리
                                        FaultDatum tmpFltData = new FaultDatum();
                                        tmpFltData.SetData(inspdata.BCNO, data);
                                        if (minXPos > data.XPOS_M) minXPos = data.XPOS_M;
                                        if (maxXPos < data.XPOS_M) maxXPos = data.XPOS_M;

                                        // 마킹 데이터만 처리
                                        if (bValid == false) continue;

                                        // 코드 불량 카운트 증가
                                        if (inspdata.CTLNO == data.CTLNO) inspdata.RollCtlCnt++;

                                        // Marking fault data 추가
                                        MarkingFaultDatum markData = new MarkingFaultDatum();
                                        markData.SetFaultData((eFCD)fcdIdx, csvType, inspdata.LNCD, inspdata.BCNO, (float)finalXPos, false, tmpFltData, data, dbOption.useKT);
                                        preMarkData.Data.Add(markData); // 이전 비교 공정 데이터

                                        int yIdx = markData.OFFSET > 0 ? (int)(markData.OFFSET / 1000.0f) : -1;
                                        if (yIdx >= 0) preMarkData.DefectCnt1M[yIdx]++;

                                        defectCnt[fcdIdx]++;
                                    }

                                    if (isDbError == false)
                                    {
                                        // 그렇지 않고 Compare Data이면 PreMarkData에 입력
                                        if (dataTarget == eProcDataType.Compare)
                                            FaultData.PreMarkData[fcdIdx].Add(preMarkData);
                                        else if (dataTarget == eProcDataType.Reference)
                                        {
                                            foreach (var datum in preMarkData.Data)
                                                FaultData.MarkData.Add(datum);
                                        }
                                    }
                                }
                            }

                            if (isDbError == false)
                                break;

                            if (conn.CheckDisconn() == true)
                            {
                                while (!conn.Connect())
                                {
                                    System.Threading.Thread.Sleep(500);
                                    conn.CheckDisconn();
                                    if (conn.IsDisconnCheckout() == true)
                                        break;
                                }
                            }
                        }

                        System.Threading.Thread.Sleep(1000);
                    }
                }

                // 불량 체크
                bool isSuccess = FalutFunction.IsDefectExist(DbDestConfig.CSVType, defectCnt);
                FaultData.AIMonResult.Judgement();
                return true;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] FAULTDAT_{((eFCD)procStep).ToString()} error message : [{ex.Message}]");
                return false;
            }
        }
        private bool SearchFLTDAT_TEST(string lotID)
        {
            float maxXPos = 0;
            float minXPos = float.MaxValue;

            bool useXOffset = false;
            bool useAIFromDB = _PreprocItem.UseAiResult;
            bool useAiMonitoring = _PreprocItem.UseAiMonitoring;
            eCSV_TYPE csvType = eCSV_TYPE.NITTO;

            string tmpKey;
            float finalXPos;
            string tmpFaltID;

            INSPDATData inspdata;

            int fcdCnt = System.Enum.GetValues(typeof(eFCD)).Length;
            int dataCnt = 0;
            string query;
            int procStep = 0;
            long dbCnt;
            string logData;

            int[] defectCnt = new int[fcdCnt];
            defectCnt.Initialize();

            float inspStartY = 0.0f;
            float inspEndY = 0.0f;

            bool bValid = false;

            FaultData = new PreProcResultData();

            // 현재 데이터는 마킹 비교 결점 데이터라는 것을 표시함.
            FaultData.IsPreProc = true;

            try
            {
                for (int fcdIdx = 0; fcdIdx < fcdCnt; fcdIdx++)
                {
                    // 데이터 초기화
                    maxXPos = 0;
                    minXPos = float.MaxValue;

                    // 
                    procStep = fcdIdx;

                    if (fcdIdx == (int)eFCD.ES) defectCnt[fcdIdx] = 0;// 확인 안 함
                    else if (fcdIdx == (int)eFCD.TG) defectCnt[fcdIdx] = 0;// 확인 안 함
                    else if (fcdIdx == (int)eFCD.ETC) defectCnt[fcdIdx] = 0;// 확인 안 함

                    if (_DbResult.INSPDATArray[fcdIdx] == null) continue;

                    int inspCnt = _DbResult.INSPDATArray[fcdIdx].Count;
                    for (int inspIdx = 0; inspIdx < inspCnt; inspIdx++)
                    {
                        if (_DbResult.INSPDATArray[fcdIdx][inspIdx] == null) continue;

                        inspdata = _DbResult.INSPDATArray[fcdIdx][inspIdx];

                        inspStartY = inspdata.YPosStart;
                        inspEndY = inspdata.YPosEnd;

                        string ctlno = inspdata.CTLNO;

                        ProcessData mkcdLncdData = null;
                        eProcDataType dataTarget = eProcDataType.None;
                        PreprocMrkDat preMarkData = new PreprocMrkDat();
                        preMarkData.LNCD = inspdata.LNCD;
                        preMarkData.CTLNO = inspdata.CTLNO;

                        AiMonitorItem aiItem = null;

                        // LNCD 데이터를 기준으로 Reference/Compare 중에서 선택함. 
                        if (inspdata.LNCD == _PreprocItem.Reference.LNCD)
                        {
                            FaultData.MarkData.LNCD = inspdata.LNCD;
                            mkcdLncdData = _PreprocItem.Reference;
                            dataTarget = eProcDataType.Reference;

                            if(_AiMonitorItem!=null)
                            {
                                string filter = _AiMonitorItem.ModelName.Trim('*');
                                if (inspdata.HINMEI.Contains(filter))
                                    aiItem = _AiMonitorItem;
                            }

                            if (aiItem != null)
                            {
                                FaultData.AIMonResult.IsModelExsit = true;
                                foreach(var aimonitor in aiItem.DefectInfo)
                                {
                                    bool isExist = false;
                                    for(int aiIdx=0; aiIdx< FaultData.AIMonResult.Items.Count; aiIdx++)
                                    {
                                        if (FaultData.AIMonResult[aiIdx].Model!=null && 
                                            FaultData.AIMonResult[aiIdx].Model.SECFLTID == aimonitor.SECFLTID)
                                        {
                                            var existFltIds = FaultData.AIMonResult[aiIdx].Model.FLTID ?? new List<string>();
                                            var targetFltIds = aimonitor.FLTID ?? new List<string>();

                                            if (existFltIds.Count == targetFltIds.Count &&
                                                !existFltIds.Except(targetFltIds).Any() &&
                                                !targetFltIds.Except(existFltIds).Any())
                                            {
                                                isExist = true;
                                                break;
                                            }
                                        }
                                    }

                                    if (isExist)                continue;
                                    FaultData.AIMonResult.Items.Add(new AiMonitorResultItem()
                                    {
                                        Model = aimonitor,
                                        Total = 0,
                                        Match = 0
                                    });
                                }
                            }
                        }
                        else
                        {
                            bool isFindComp = false;
                            foreach (var compItem in _PreprocItem.Compare)
                            {
                                if (inspdata.LNCD == compItem.LNCD)
                                {
                                    isFindComp = true;
                                    mkcdLncdData = compItem;
                                    dataTarget = eProcDataType.Compare;
                                    break;
                                }
                            }
                            if (isFindComp == false)
                            {
                                mkcdLncdData = null;
                                continue; // 데이터 탐색 안함.
                            }
                        }

                        // 매칭 불량 갯수 초기화
                        inspdata.RollCtlCnt = 0;

                        bool isTextEnd = false;
                        string path = Path.Combine(_LOG.GetBcrPath(), _SubPath, lotID, "FAULTDAT_DBResult.txt");
                        using (var reader = new StreamReader(path, Encoding.UTF8))
                        {
                            string text;
                            while ((text = reader.ReadLine()) != null)
                            {
                                // 데이터 맞는지 확인
                                if (text.Contains("SELECT") == true && text.Contains(inspdata.CTLNO) == true)
                                {
                                    while ((text = reader.ReadLine()) != null)
                                    {
                                        if (text.Contains("SELECT") == true)
                                        {
                                            isTextEnd = true;
                                            break;
                                        }
                                        FLTDATA_DailyData data = new FLTDATA_DailyData();
                                        data.Parse(text);
                                        tmpFaltID = data.FLTID.ToUpper();


                                        finalXPos = data.XPOS_M;
                                        if (useXOffset == true) finalXPos += inspdata.OffsetX;
                                        if (useAIFromDB == false) // AI 미사용시
                                        {
                                            tmpKey = data.MNTTAN.TrimStart();
                                            if (string.IsNullOrEmpty(tmpKey))
                                                tmpKey = data.FLTID;
                                        }
                                        else tmpKey = data.FLTID;

                                        if (dataTarget == eProcDataType.Reference && aiItem!=null)
                                        {
                                            FaultData.AIMonResult.AddDefectCnt(data.MNTTAN, data.FLTID);
                                        }

                                        // MKCD Model에서 데이터 가져와서 다시 탐색함. 
                                        bValid = false;
                                        if (mkcdLncdData != null)
                                        {
                                            bValid = mkcdLncdData.IsValidFLTID(tmpKey, data.AREA_M + 0.00001f);
                                            if (bValid == false) continue;
                                        }
                                        else continue;

                                        if (data.OFFSET < inspStartY || data.OFFSET > inspEndY) continue;
                                        if (finalXPos < 0.0f) continue;

                                        // 전체 데이터를 저장한다. 
                                        // Fault Data 처리
                                        FaultDatum tmpFltData = new FaultDatum();
                                        tmpFltData.SetData(inspdata.BCNO, data);
                                        if (minXPos > data.XPOS_M) minXPos = data.XPOS_M;
                                        if (maxXPos < data.XPOS_M) maxXPos = data.XPOS_M;

                                        // 마킹 데이터만 처리
                                        if (bValid == false) continue;

                                        // 코드 불량 카운트 증가
                                        if (inspdata.CTLNO == data.CTLNO) inspdata.RollCtlCnt++;

                                        // Marking fault data 추가
                                        MarkingFaultDatum markData = new MarkingFaultDatum();
                                        markData.SetFaultData((eFCD)fcdIdx, csvType, inspdata.LNCD, inspdata.BCNO, (float)finalXPos, false, tmpFltData, data, dbOption.useKT);

                                        int yIdx = markData.OFFSET > 0 ? (int)(markData.OFFSET / 1000.0f) : -1;
                                        if (dataTarget == eProcDataType.Reference)
                                        {
                                            FaultData.MarkData.Add(markData);
                                            if (yIdx >= 0) FaultData.DefectCnt1M[yIdx]++;
                                        }
                                        else// 마킹 대상 결점
                                        {
                                            preMarkData.Data.Add(markData); // 이전 비교 공정 데이터
                                            if (yIdx >= 0) preMarkData.DefectCnt1M[yIdx]++;
                                        }

                                        defectCnt[fcdIdx]++;
                                    }
                                }

                                if (isTextEnd) break;
                            }
                        }

                        // 그렇지 않고 Compare Data이면 PreMarkData에 입력
                        if (dataTarget == eProcDataType.Compare)
                            FaultData.PreMarkData[fcdIdx].Add(preMarkData);
                    }
                }

                // 불량 체크
                bool isSuccess = FalutFunction.IsDefectExist(DbDestConfig.CSVType, defectCnt);
                FaultData.AIMonResult.Judgement();
                return true;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] FAULTDAT_{((eFCD)procStep).ToString()} error message : [{ex.Message}]");
                return false;
            }
        }

        public override IPreprocLot SearchLot(ref eSearchError errOut)
        {
            bool success = false;
            try
            {
                ResetDataAll();

                this.SearchLotName = LotID;
                if (LotID.Substring(0, 2) == "TG" || LotID.Substring(0, 2) == "TS")
                    dbOption.useKT = true;
                else
                    dbOption.useKT = false;

                // 이전 랏데이터 확인해서 데이터가 있으면 넘김
                int newLotCnt = GetLotSpliceCnt(LotID);
                if (newLotCnt > 0)
                {
                    Log.Write(Language.spliceExists);
                    errOut = eSearchError.SpliceExistErr;
                    // 재갱신 데이터가 아니면 업데이트 안하고 스킵함.
                    if (Renewal == false)
                        return null;

                    _LOG.DeleteFolder(_SubPath, LotID);
                }

                _LOG.LogMode = LogMode;
                _LOG.Lot = LotID;

                QueryMsg.PTRYLP_Query ptrylp = new QueryMsg.PTRYLP_Query(LotID);
                string query = ptrylp.GetQuery();
                long dbCnt = 0;
                _LOG.WriteLoadData(_SubPath, query.ToString(), 0, "PTRYLP", 0);

                if (query == "")
                {
                    errOut = eSearchError.PTRYLPEmpty;
                    Log.Write($"[Error] DB Serach PTRYLP query is empty.");
                }

                Log.Write($"[{LotID}] {Language.searchPTRYLP}");

                bool isDbError = false;
                // 끊어짐에 대한 재연결 처리
                conn.ResetDisconCheck();

                while (true)
                {
                    isDbError = false;

                    // 검색하다가 튕겨나갈 수 있으니 데이터 삭제 처리 후 다시 검색함
                    _DbResult.PTRLYP.Clear();

                    if (conn.Connection.State != ConnectionState.Open)
                        conn.Connect();

                    using (var comm = new OracleCommand(query, conn.Connection))
                    {
                        using (var reader = comm.ExecuteReader(CommandBehavior.SequentialAccess))
                        {
                            while (true)
                            {
                                if (conn.Connection.State != ConnectionState.Open)
                                {
                                    isDbError = true;
                                    break;
                                }
                                if (reader.Read() == false) break;

                                PTRYLPdata data = new PTRYLPdata();
                                data.Parse(reader);
                                _DbResult.PTRLYP.Add(data);
                                string logData = string.Format($"{_DbResult.PTRLYP.Count}\t-\t{data.ToString()}");
                                _LOG.WriteLoadData(_SubPath, logData, 0, "PTRYLP", 0);
                            }

                            success = true;
                        }
                    }

                    if (isDbError == false)
                        break;

                    if (conn.CheckDisconn() == true)
                    {
                        while (!conn.Connect())
                        {
                            System.Threading.Thread.Sleep(500);

                            if (conn.IsDisconnCheckout() == true)
                                break;
                        }
                    }
                }

                if (success == false)
                {
                    Log.Write($"[{LotID}] {Language.errorSearchPTRYLP}");
                    errOut = eSearchError.PTRYLPSearchErr;
                    return null;
                }

                Log.Write($"[{LotID}] {Language.searchXOFSMST}");
                success = SearchXOFSMST();
                if (success == false)
                {
                    errOut = eSearchError.XOFSMSTSearchErr;
                    Log.Write($"[{LotID}] {Language.errorSearchXOFSMST}");
                    return null;
                }

                Log.Write($"[{LotID}] {Language.searchPTRY0P}");
                success = SearchPTRY0P();
                if (success == false)
                {
                    errOut = eSearchError.PTRY0PSearchErr;
                    Log.Write($"[{LotID}] {Language.errorSearchPTRY0P}");
                    return null;
                }

                Log.Write($"[{LotID}] {Language.searchINSPDAT}");
                success = SearchINSPDAT();
                if (success == false)
                {
                    errOut = eSearchError.INSPDATSearchErr;
                    Log.Write($"[{LotID}] {Language.errorSearchINSPDAT}");
                    return null;
                }

                // 첫 검사 랏은 복사하여둔다
                InspDatToFCDArray();

                Log.Write($"[{LotID}] {Language.searchFLTDAT}");
                success = SearchFLTDAT();
                if (success == false)
                {
                    errOut = eSearchError.FLTDATSearchErr;
                    Log.Write($"[{LotID}] {Language.errorSearchFLTDAT}");
                    return null;
                }
                Log.Write($"[{LotID}] {Language.finishLotSearch}");
                // 처리 완료되면 데이터 정리

                return new PreprocLotFilter(LotID, _DbResult, FaultData);
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach Lot error message : [{ex.Message}]");
                return null;
            }
        }

        public override IPreprocLot SearchLot_TEST(ref eSearchError errOut)
        {
            bool success = false;
            try
            {
                ResetDataAll();

                this.SearchLotName = LotID;
                if (LotID.Substring(0, 2) == "TG" || LotID.Substring(0, 2) == "TS")
                    dbOption.useKT = true;
                else
                    dbOption.useKT = false;

                _LOG.LogMode = LogMode;
                _LOG.Lot = LotID;

                Log.Write($"[{LotID}] {Language.searchPTRYLP}");
                string path = Path.Combine(_LOG.GetBcrPath(), _SubPath, LotID, "PTRYLP_DBResult.txt");

                using (var reader = new StreamReader(path, Encoding.UTF8))
                {
                    string text;
                    while ((text = reader.ReadLine()) != null)
                    {
                        if (text.Contains("SELECT") == true) continue;
                        PTRYLPdata data = new PTRYLPdata();
                        data.Parse(text);
                        _DbResult.PTRLYP.Add(data);
                    }

                    success = true;
                }

                if (success == false)
                {
                    errOut = eSearchError.PTRYLPSearchErr;
                    Log.Write($"[{LotID}] {Language.errorSearchPTRYLP}");
                    return null;
                }

                Log.Write($"[{LotID}] {Language.searchXOFSMST}");
                success = SearchXOFSMST_TEST();
                if (success == false)
                {
                    errOut = eSearchError.XOFSMSTSearchErr;
                    Log.Write($"[{LotID}] {Language.errorSearchXOFSMST}");
                    return null;
                }

                Log.Write($"[{LotID}] {Language.searchPTRY0P}");
                success = SearchPTRY0P_TEST();
                if (success == false)
                {
                    errOut = eSearchError.PTRY0PSearchErr;
                    Log.Write($"[{LotID}] {Language.errorSearchPTRY0P}");
                    return null;
                }

                Log.Write($"[{LotID}] {Language.searchINSPDAT}");
                success = SearchINSPDAT_TEST();
                if (success == false)
                {
                    errOut = eSearchError.INSPDATSearchErr;
                    Log.Write($"[{LotID}] {Language.errorSearchINSPDAT}");
                    return null;
                }

                // 첫 검사 랏은 복사하여둔다
                InspDatToFCDArray();

                Log.Write($"[{LotID}] {Language.searchFLTDAT}");
                success = SearchFLTDAT_TEST(LotID);
                if (success == false)
                {
                    errOut = eSearchError.FLTDATSearchErr;
                    Log.Write($"[{LotID}] {Language.errorSearchFLTDAT}");
                    return null;
                }

                // 처리 완료되면 데이터 정리
                return new PreprocLotFilter(LotID, _DbResult, FaultData);
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach Lot error message : [{ex.Message}]");
                return null;
            }
        }

        public override bool SearchPTRYOPList()
        {
            // 연결 확인
            if (conn?.IsConnected() == false) return false;

            try
            {
                Log.Write(Language.searchProductedLot);
                string strFilter = Helper.ReplaceInvalidPathChar($"{Filter.Line}_{Filter.Product}_{Filter.Model}");
                string strLine = $"[{strFilter}]";

                _LOG.LogMode = LogMode;
                _LOG.Lot = $"{strLine} PTRY0PList" + StartTime.ToString("yyyyMMdd");

                // 임시로 패스 경로를 설정한다.
                _SubPath = strFilter;

                // Daily Lot DATA 내용을 초기화 한다 
                PTRY0PList_Data.Clear();

                // 검색은 하루 단위로 해야함
                DateTime currentTime = StartTime;
                while (currentTime <= EndTime)
                {
                    QueryMsg.PTRY0PList_Query ptry0p = new QueryMsg.PTRY0PList_Query();

                    //////////////////////////////////////////////////////
                    // 해당 LNCD는 상위에서 입력받은 LNCD임
                    // 검색 시간은 찾고자 하는 검색 시간대를 입력함.
                    ptry0p.Y0LNCD = LNCD;
                    ptry0p.DateCurrent = currentTime;
                    ptry0p.DateNext = currentTime;
                    ///////////////////////////////////////////////////////

                    string query = ptry0p.GetQuery();
                    string listFileName = $"[{LNCD}] PTRY0PList";
                    _LOG.WriteLoadData(query.ToString(), 0, listFileName, 0, true);

                    if (query == "")
                    {
                        Log.Write($"[Error] DB Serach {listFileName} query is empty.");
                    }

                    using (var comm = new OracleCommand(query, conn.Connection))
                    {
                        using (var reader = comm.ExecuteReader(CommandBehavior.SequentialAccess))
                        {
                            while (reader.Read())
                            {
                                PTRY0PData data = new PTRY0PData();
                                data.Parse(reader);

                                if (data.Y0KKOL.Substring(8) == "000000" && data.Y0KSOL.Substring(8) == "000000")
                                    continue;

                                // LotHistory에 존재하면 스킵함
                                if (LotHistory != null)
                                {
                                    if (LotHistory.IsLotExist(Filter.ToString(), data.Y0KLOT))
                                        continue;
                                }

                                // 우선 전체 데이터 넣는다.
                                PTRY0PList_Data.Add(data);

                                string logData = string.Format($"{PTRY0PList_Data.Count}\t-\t{data.ToString()}");
                                _LOG.WriteLoadData(logData, 0, listFileName, 0);
                            }
                        }
                    }
                    currentTime = currentTime.AddDays(1);
                }

                if (PTRY0PList_Data.Count == 0)
                    return false;

                // 이름으로 랏 정렬을 한다.
                PTRY0PList_Data.Copy(PTRY0PList_Data.Data.OrderBy(p => p.Y0KLOT).ToList());

                Log.Write($"{Language.lotSearchCount} : {PTRY0PList_Data.Count}");
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach PTRY0P_TODAY Data error message : [{ex.Message}]");
                return false;
            }

            return true;
        }

        public override bool SearchPTRYOPList_TEST()
        {
            try
            {
                string strFilter = Helper.ReplaceInvalidPathChar($"{Filter.Line}_{Filter.Product}_{Filter.Model}");
                string strLine = $"[{strFilter}]";
                string path = $"{strLine} PTRY0PList" + StartTime.ToString("yyyyMMdd");

                // 실시간 감시 모드를 업데이트한다. 
                _LOG.LogMode = LogMode;

                path = Path.Combine(_LOG.GetBcrPath(), path, $"[{LNCD}] PTRY0PList_DBResult.txt");

                _SubPath = strFilter;
                // Daily Lot DATA 내용을 초기화 한다 
                PTRY0PList_Data.Clear();

                if (System.IO.File.Exists(path) == false)
                    return false;

                using (var file = new StreamReader(path, Encoding.Default))
                {
                    string text;
                    while ((text = file.ReadLine()) != null)
                    {
                        if (text.Contains("SELECT") == true) continue;

                        PTRY0PData data = new PTRY0PData();
                        data.Parse(text);

                        if (data.Y0KKOL.Substring(8) == "000000" && data.Y0KSOL.Substring(8) == "000000")
                            continue;

                        // 동일 경로가 존재하는 경우 패스
                        if (LotHistory != null)
                        {
                            if (LotHistory.IsLotExist(Filter.ToString(), data.Y0KLOT))
                                continue;
                        }

                        // 우선 전체 데이터 넣는다.
                        PTRY0PList_Data.Add(data);
                    }
                }
                if (PTRY0PList_Data.Count == 0)
                    return false;

                // 이름으로 랏 정렬을 한다.
                PTRY0PList_Data.Copy(PTRY0PList_Data.Data.OrderBy(p => p.Y0KLOT).ToList());
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach PTRY0P_TODAY Data error message : [{ex.Message}]");
                return false;
            }

            return true;
        }

    }
}
