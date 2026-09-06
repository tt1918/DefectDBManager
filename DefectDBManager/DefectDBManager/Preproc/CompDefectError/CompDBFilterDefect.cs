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
    public class CompDBFilterDefect : CompDefectErrorDefault, ICompareDefectError
    {
        #region Lot Search Param
        public LotSelProcParam ProcParam { get; private set; } = new LotSelProcParam();

        public AiMonitorItem _AiMonitorItem { get; private set; } = new AiMonitorItem();
        #endregion

        public CompDBFilterDefect(object parent, OracleDbConnection dbconn, bool disconnDB = true)
            : base(parent, dbconn, disconnDB)
        {
        }

        public void SetDBParam(string dbMkcd, AiMonitorItem aiMonitorItem)
        {
            _SubPath = $"DB_{dbMkcd}";
            _isWildCard = false;
            _AiMonitorItem = aiMonitorItem;
        }

        public void SetLotSearchingParam(string lotID, LotSelProcParam procParam, AiMonitorItem aiMonitorItem, bool bMsgOut)
        {
            this.LotID = lotID.ToUpper();
            this.ProcParam = procParam;
            this._AiMonitorItem = aiMonitorItem;
            this.IsMsgOut = bMsgOut;
        }
        private bool SearchMRKCTLMST()
        {
            return searchMRKCTLMSTfromDB();
        }

        private bool searchMRKCTLMSTfromDB()
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;

            int procStep = 0;
            string lotID = LotID;
            LotSelProcParam procParam = ProcParam;

            try
            {
                DestConfigUnit destUnit = destConfig.SelDestUnit;
                if (destUnit == null)
                {
                    destConfig.SetSelDest(procParam.DBFilter.Title);
                }

                long dbCnt = 0;
                int count = System.Enum.GetValues(typeof(eFCD)).Length;
                string logData = "";

                for (int i = 0; i < count; i++)
                {
                    procStep = i;
                    int PTRY0Pcnt = _DbResult.PTRY0P[i].Count;
                    _DbResult.CheckDicMRKCTLMSTSize(PTRY0Pcnt, i);

                    for (int j = 0; j < PTRY0Pcnt; j++)
                    {
                        _DbResult.ClearDicMRKCTLMST(i, j);

                        if ((procParam.DBFilter.UseES == true && (eFCD)i == eFCD.ES) ||
                           (procParam.DBFilter.UseTG == true && (eFCD)i == eFCD.TG) ||
                           (procParam.DBFilter.UseETC == true && (eFCD)i == eFCD.ETC) && _DbResult.PTRY0P[i][j].Y0KLOT.Length > 0)
                        {
                            QueryMsg.MRKCTLMST_Query msg = new QueryMsg.MRKCTLMST_Query();
                            msg.MKCD = procParam.DBFilter.MKCD;
                            msg.Y0KLOT = _DbResult.PTRY0P[i][j].Y0KLOT;
                            string query = msg.GetQuery((eFCD)i);
                            _LOG.WriteLoadData(_SubPath, query, 0, "MRKCTLMST", 0.0);

                            if (query == "")
                            {
                                Log.Write($"[Error] MRKCTLMST_{((eFCD)i).ToString()} Query message is empty.");
                                return false;
                            }

                            using (var comm = new OracleCommand(query, conn.Connection))
                            {
                                using (var reader = comm.ExecuteReader())
                                {
                                    dbCnt = reader.RowSize;

                                    while (reader.Read())
                                    {
                                        MRKCTLMSTData data = new MRKCTLMSTData();
                                        data.Parse(reader);
                                        _DbResult.MRKCTLMST.Add(data);

                                        logData = string.Format($"{_DbResult.MRKCTLMST.Count}\t-\t{data.ToString()}");
                                        _LOG.WriteLoadData(_SubPath, logData, _DbResult.MRKCTLMST.Count, "MRKCTLMST", 0.0);
                                        // 조건문 추가해야 함

                                        _DbResult.AddDicMRKCTLMST(i, j, data);
                                    }
                                }
                            }
                        }
                    }
                }

                return true;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] MRKCTLMST_{((eFCD)procStep).ToString()} error message : [{ex.Message}]");
                return false;
            }
        }

        private bool SearchFLTDAT()
        {
            LotSelProcParam procParam = ProcParam;

            float maxXPos = 0;
            float minXPos = float.MaxValue;

            bool useXOffset = false;
            bool useAIFromDB = procParam.UseAiResult;
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

                    if (procParam.DBFilter.UseES == true && fcdIdx == (int)eFCD.ES) defectCnt[fcdIdx] = 0;// 확인 안 함
                    else if (procParam.DBFilter.UseTG == true && fcdIdx == (int)eFCD.TG) defectCnt[fcdIdx] = 0;// 확인 안 함
                    else if (procParam.DBFilter.UseETC == true && fcdIdx == (int)eFCD.ETC) defectCnt[fcdIdx] = 0;// 확인 안 함

                    if (_DbResult.INSPDATArray[fcdIdx] == null) continue;

                    int inspCnt = _DbResult.INSPDATArray[fcdIdx].Count;

                    for (int inspIdx = 0; inspIdx < inspCnt; inspIdx++)
                    {
                        if (_DbResult.INSPDATArray[fcdIdx][inspIdx] == null) continue;
                        string inspLNCD = _DbResult.INSPDATArray[fcdIdx][inspIdx].LNCD;
                        int mkcdIdx = -1;
                        for (int mkcdI = 0; mkcdI < _DbResult.INSPDAT[fcdIdx].Count; mkcdI++)
                        {
                            for (int idx = 0; idx < _DbResult.INSPDAT[fcdIdx][mkcdI].Count; idx++)
                            {
                                if (_DbResult.INSPDAT[fcdIdx][mkcdI][idx].LNCD == inspLNCD)
                                {
                                    mkcdIdx = mkcdI;
                                    break;
                                }
                            }
                            if (mkcdIdx != -1) break;
                        }

                        if (mkcdIdx == -1)
                        {
                            continue;
                        }

                        _DbResult.UpdateDicMRKF1Data(fcdIdx, mkcdIdx);
                        _DbResult.UpdateDicSizeData(fcdIdx, mkcdIdx);

                        if (_DbResult.dicMRKF1Data.Count == 0 || _DbResult.dicSizeData.Count == 0)
                            continue;

                        inspdata = _DbResult.INSPDATArray[fcdIdx][inspIdx];

                        inspStartY = inspdata.YPosStart;
                        inspEndY = inspdata.YPosEnd;

                        // 매칭 불량 갯수 초기화
                        inspdata.RollCtlCnt = 0;

                        QueryMsg.FLTDAT_FAST_Query fastMsg = new QueryMsg.FLTDAT_FAST_Query();
                        fastMsg.CTLNO = inspdata.CTLNO;
                        query = fastMsg.GetQuery(_DbResult.dicSizeData, _DbResult.dicMRKF1Data);

                        _LOG.WriteLoadData(_SubPath, query, 0, "FAULTDAT", 0.0);

                        if (query == "")
                        {
                            Log.Write($"[Error] FAULTDAT_{((eFCD)fcdIdx).ToString()} Query message is empty.");
                            return false;
                        }

                        eProcDataType dataTarget = eProcDataType.None;
                        AiMonitorItem aiItem = null;

                        // LNCD 데이터를 기준으로 Reference/Compare 중에서 선택함. 
                        if (inspdata.LNCD == _DbResult.SelectedDbLNCD)
                        {
                            FaultData.MarkData.LNCD = inspdata.LNCD;
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
                            dataTarget = eProcDataType.Compare;
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

                                        FLTDATAData data = new FLTDATAData();
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

                                        if (data.OFFSET < inspStartY || data.OFFSET > inspEndY) continue;
                                        if (finalXPos < 0.0f) continue;

                                        // 전체 데이터를 저장한다. 
                                        // Fault Data 처리
                                        FaultDatum tmpFltData = new FaultDatum();
                                        tmpFltData.SetData(inspdata.BCNO, data);
                                        if (minXPos > data.XPOS_M) minXPos = data.XPOS_M;
                                        if (maxXPos < data.XPOS_M) maxXPos = data.XPOS_M;

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

        private bool SearchFLTDAT_TEST()
        {
            LotSelProcParam procParam = ProcParam;
            string lotID = LotID;

            float maxXPos = 0;
            float minXPos = float.MaxValue;

            bool useXOffset = false;
            bool useAIFromDB = procParam.UseAiResult;
            eCSV_TYPE csvType = eCSV_TYPE.NITTO;

            string tmpKey;
            float finalXPos;
            string tmpFaltID;

            INSPDATData inspdata;

            int fcdCnt = System.Enum.GetValues(typeof(eFCD)).Length;
            int procStep = 0;
            
            int[] defectCnt = new int[fcdCnt];
            defectCnt.Initialize();

            float inspStartY = 0.0f;
            float inspEndY = 0.0f;

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
                        if (inspdata.LNCD == _DbResult.SelectedDbLNCD)
                        {
                            FaultData.MarkData.LNCD = inspdata.LNCD;
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
                            dataTarget = eProcDataType.Compare;


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
                                        FLTDATAData data = new FLTDATAData();
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

                                        if (dataTarget == eProcDataType.Reference && aiItem != null)
                                        {
                                            FaultData.AIMonResult.AddDefectCnt(data.MNTTAN, data.FLTID);
                                        }

                                        // MKCD Model에서 데이터 가져와서 다시 탐색함. 

                                        if (data.OFFSET < inspStartY || data.OFFSET > inspEndY) continue;
                                        if (finalXPos < 0.0f) continue;

                                        // 전체 데이터를 저장한다. 
                                        // Fault Data 처리
                                        FaultDatum tmpFltData = new FaultDatum();
                                        tmpFltData.SetData(inspdata.BCNO, data);
                                        if (minXPos > data.XPOS_M) minXPos = data.XPOS_M;
                                        if (maxXPos < data.XPOS_M) maxXPos = data.XPOS_M;

                                        // 코드 불량 카운트 증가
                                        if (inspdata.CTLNO == data.CTLNO) inspdata.RollCtlCnt++;

                                        // Marking fault data 추가
                                        MarkingFaultDatum markData = new MarkingFaultDatum();
                                        markData.SetFaultData((eFCD)fcdIdx, csvType, inspdata.LNCD, inspdata.BCNO, (float)finalXPos, false, tmpFltData, data, dbOption.useKT);

                                        int yIdx = markData.OFFSET>0 ? (int)(markData.OFFSET / 1000.0f) : -1;
                                        if (dataTarget == eProcDataType.Reference)
                                        {
                                            FaultData.MarkData.Add(markData);
                                            if(yIdx>=0)FaultData.DefectCnt1M[yIdx]++;
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
            // 연결 확인
            if (conn?.IsConnected() == false)
                return null;
            bool success = false;

            ResetDataAll();

            try
            {
                this.SearchLotName = LotID;
                if (LotID.Substring(0, 2) == "TG" || LotID.Substring(0, 2) == "TS")
                    dbOption.useKT = true;
                else
                    dbOption.useKT = false;

                _LOG.LogMode = LogDB.eDataType.SelectedLot;
                _LOG.Lot = LotID;

                // 이전 랏데이터 확인해서 스플라이스 처리해야 함
                // 이전 랏데이터 확인해서 데이터가 있으면 넘김
                int newLotCnt = GetLotSpliceCnt(LotID);
                if (newLotCnt > 0)
                {
                    Log.Write(Language.spliceExists);
                    errOut = eSearchError.SpliceExistErr;
                    _LOG.DeleteFolder(_SubPath, LotID);
                }

                Log.Write($"[{LotID}] {Language.searchPTRYLP}");
                QueryMsg.PTRYLP_Query ptrylp = new QueryMsg.PTRYLP_Query(LotID);
                string query = ptrylp.GetQuery();
                long dbCnt = 0;
                _LOG.WriteLoadData(_SubPath, query.ToString(), 0, "PTRYLP", 0);

                if (query == "")
                    Log.Write($"[Error] DB Serach PTRYLP query is empty.");

                using (var comm = new OracleCommand(query, conn.Connection))
                {
                    using (var reader = comm.ExecuteReader())
                    {
                        dbCnt = reader.RowSize;
                        while (reader.Read())
                        {
                            PTRYLPdata data = new PTRYLPdata();
                            data.Parse(reader);
                            _DbResult.PTRLYP.Add(data);
                            string logData = string.Format($"{_DbResult.PTRLYP.Count}\t-\t{data.ToString()}");
                            _LOG.WriteLoadData(_SubPath, logData, 0, "PTRYLP", 0);
                        }

                        success = true;
                    }
                }

                if (success == false)
                {
                    return null;
                }

                Log.Write($"[{LotID}] {Language.searchXOFSMST}");
                success = SearchXOFSMST();
                if (success == false) return null;

                if (DbDestConfig.UseAREADEL == true)
                {
                    Log.Write($"[{LotID}] {Language.searchAREADEL}");
                    success = SearchAreaDel(ref _DbResult.AREADEL);
                    if (success == false) return null;
                }

                Log.Write($"[{LotID}] {Language.searchPTRY0P}");
                success = SearchPTRY0P();
                if (success == false) return null;

                Log.Write($"[{LotID}] {Language.searchMRKCTLMST}");
                success = SearchMRKCTLMST();
                if (success == false) return null;

                Log.Write($"[{LotID}] {Language.searchINSPDAT}");
                success = SearchINSPDAT();
                if (success == false) return null;

                // PTRYOP랑 매칭해서 선택 랏 이름 처리해야 함
                _DbResult.SelectedDbLNCD = string.Empty;
                foreach (var opList in _DbResult.PTRY0P)
                {
                    PTRY0PData result = opList.Data.FirstOrDefault(x => x.Y0KLOT == LotID);
                    if (result != null)
                    {
                        _DbResult.SelectedDbLNCD = result.LNCD;
                        break;
                    }
                }
                if (_DbResult.SelectedDbLNCD.Length == 0)
                    return null;

                // 첫 검사 랏은 복사하여둔다
                InspDatToFCDArray();

                Log.Write($"[{LotID}] {Language.searchFLTDAT}");
                success = SearchFLTDAT();

                // 처리 완료되면 데이터 정리
                return new PreprocLotDB(LotID, _DbResult, FaultData);
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach Lot error message : [{ex.Message}]");
                return null;
            }
        }

        public override IPreprocLot SearchLot_TEST(ref eSearchError errOut)
        {
            // 연결 확인
            bool success = false;

            ResetDataAll();

            try
            {
                ResetDataAll();

                this.SearchLotName = LotID;
                if (LotID.Substring(0, 2) == "TG" || LotID.Substring(0, 2) == "TS")
                    dbOption.useKT = true;
                else
                    dbOption.useKT = false;

                _LOG.LogMode = LogDB.eDataType.SelectedLot;
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

                Log.Write($"[{LotID}] {Language.searchPTRYLP}");
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

                _DbResult.SelectedDbLNCD = string.Empty;
                foreach (var opList in _DbResult.PTRY0P)
                {
                    PTRY0PData result = opList.Data.FirstOrDefault(x => x.Y0KLOT == LotID);
                    if (result != null)
                    {
                        _DbResult.SelectedDbLNCD = result.LNCD;
                        break;
                    }
                }
                if (_DbResult.SelectedDbLNCD.Length == 0)
                    return null;

                // 첫 검사 랏은 복사하여둔다
                InspDatToFCDArray();

                Log.Write($"[{LotID}] {Language.searchFLTDAT}");
                success = SearchFLTDAT_TEST();
                if (success == false)
                {
                    errOut = eSearchError.FLTDATSearchErr;
                    Log.Write($"[{LotID}] {Language.errorSearchFLTDAT}");
                    return null;
                }

                // 처리 완료되면 데이터 정리
                return new PreprocLotDB(LotID, _DbResult, FaultData);
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach Lot error message : [{ex.Message}]");
                return null;
            }
        }

    }
}
