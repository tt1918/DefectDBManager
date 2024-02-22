using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace DefectDBManager
{
    public delegate void DelegateEndSearchingTodayProduct();
    public delegate void DelegateEndSearchingAvailableLot();
    public delegate void DelegatePopupError(string errString);
    public class PreProcCompDB
    {
        // 
        public event DelegateEndSearchingTodayProduct OnEndTodayProductSearching;
        public event DelegateEndSearchingAvailableLot OnEndSearchingAvailableLot;
        public event DelegatePopupError OnPopupError;
        public OracleDbConnection Conn { get { return conn; } }
        private OracleDbConnection conn = null;

        public List<string> SearchModelList = null;

        public DestConfig DbDestConfig
        {
            get { return destConfig; }
            set { destConfig = value; }
        }
        private DestConfig destConfig;

        public CodeConfig DBCodeConfig
        {
            get { return codeConfig; }
            set { codeConfig = value; }
        }
        private CodeConfig codeConfig;

        public Option DbOption
        {
            get { return dbOption; }
            set { dbOption = value; }
        }
        private Option dbOption;

        public Param CrtParam
        {
            get { return currentParam; }
            set { currentParam = value; }
        }
        private Param currentParam;

        public PreProcCompDBResult _DbResult
        {
            get;
            set;
        }

        public ResultData[] ResultDefect
        {
            get { return resultDefect; }
            set { resultDefect = value; }
        }
        private ResultData[] resultDefect;

        public IRollDefectInfo _RollDefectInfo { get; set; }
        public CSV_DEFECT_HEADER _CsvDefectHeader = null;
        public List<CSVLoadInfo> _CSVLoadInfo = null;

        public NittoDBProgress DB_Progress { get; private set; }

        public string SearchLotName { get; set; }

        public string SearchY0LNCD { get; set; }

        public UInt16 CrtY0KLOTIdx { get; set; }
        public UInt16 NextY0KLOTIdx { get; set; }

        public LogDB _LOG;

        // 상위 객체
        private object owner;

        public PreProcCompDB(object parent, OracleDbConnection dbconn)
        {
            owner = parent;
            conn = dbconn;
            _DbResult = new PreProcCompDBResult();
            DB_Progress = new NittoDBProgress();
            _CSVLoadInfo = new List<CSVLoadInfo>();
            _LOG = new LogDB();

            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            for(int i= 0; i < count; i++)
                ResultDefect[i] = new ResultData();
        }

        ~PreProcCompDB()
        {
            conn?.Dispose();
        }

        public void ResetDataAll()
        {
            _DbResult.ClearAll();

            // 각 광학별 불량 갯수 초기화
            CrtParam.ClearEachOpticDefectCnt();

            for(int i=0; i<resultDefect.Length; i++)
                ResultDefect[i].ResetAll();
            _CSVLoadInfo.Clear();

            CrtParam.isProductAvaliable = false;
            CrtParam.isXOffsetError = false;
        }

        private int GetNextLotCnt(string lotID)
        {
            int nNewCnt = 0;

            int i, j, nLotCnt = -1;
            DateTime t = DateTime.Today;
            TimeSpan ts = new TimeSpan(1, 0, 0, 0);

            string strLowPath;
            for (i = 0; i < 10; i++)
            {
                for (j = 30 - 1; j >= 0; j--)
                {
                    if (j == 0) strLowPath = Path.Combine(Define.BCRPath, lotID);
                    else strLowPath = Path.Combine(Define.BCRPath, $"{lotID}_{j:D2}");

                    if (Directory.Exists(strLowPath)) nLotCnt = j;

                    //폴더가 존재함
                    if (nLotCnt < 30 && nLotCnt >= 0) return nLotCnt + 1;
                    else nNewCnt = 0;
                }
                t -= ts;
                nLotCnt = -1;
            }

            return nNewCnt;
        }

        public bool SearchLot(string Y0LNCD, string lotID)
        {
            // 연결 확인
            if (conn?.IsConnected() == false)   return false;
            bool success = false;

            // Lot 확인하는 스레드 중지
            StopCheckAvaliableINSPDAT();

            try
            {
                DB_Progress.ResetAll();
                DB_Progress._CurrentStep = eNittoDBProgress.PTRYLP;

                QueryMsg.PTRYOP_Today_Query ptryop = new QueryMsg.PTRYOP_Today_Query();
                ptryop.Y0LNCD = Y0LNCD;

                string query = ptryop.GetQuery();
                _LOG.WriteLoadData(query.ToString(), 0, "PTRYLP", 0);
                
                if (query == "")
                {
                    Log.Write($"[Error] DB Serach PTRYLP query is empty.");
                    DB_Progress.SetError(eNittoDBProgress.PTRYLP);
                }

                using (var comm = new OracleCommand(query, conn.Connection))
                {
                    using (var reader = comm.ExecuteReader())
                    {
                        while(reader.Read())
                        {
                            PTRY0PData data = new PTRY0PData();
                            data.Parse(reader);
                            
                            // 비어있는 데이터만 탐색한다. 
                            if(data.Y0KKOL.Substring(8)=="000000" && data.Y0KSOL.Substring(8) == "000000")
                            {
                                _DbResult.PTRY0P_Today_Data.Add(data);
                            }
                        }
                        DB_Progress.Set(eNittoDBProgress.PTRYLP);
                    }
                }

                success = SearchPTRYOP(lotID);
                if (success == false) return false;
                success = SearchINSPDAT(lotID);
                if (success == false) return false;
            }
            catch ( Exception ex)
            {
                Log.Write($"[Error] DB Serach Lot error message : [{ex.Message}]");
                return false;
            }

            // BCNO와 yPos 이용해서 검사하는 Lot Check 스레드 재시작
            StartCheckAvaliableINSPDAT();

            return true;
        }

        /// <summary>
        /// 현재 생산하는 제품의 BCR과 위치를 확인하여 생산하고 있는 랏이 유효한지
        /// 확인하는 함수
        /// </summary>
        /// <param name="bcno"> 현재 생산하는 제품의 BCNO </param>
        /// <param name="dPosY"> 현재 생산하는 제품의 위치 </param>
        /// <returns></returns>
        public bool IsCrtDataAvaliable(string bcno, double dPosY)
        {
            bool[] isAvaliable = null;
            int count = System.Enum.GetValues(typeof(eFCD)).Length;

            isAvaliable = new bool[count];

            for (int i=0; i<count; i++)
            {
                isAvaliable[i] = true;
                if (_DbResult.Matched_INSPDAT_Data[i].Count==0)

                foreach(INSPDATData data in _DbResult.Matched_INSPDAT_Data[i])
                {
                    if (data.BCNO != bcno || (data.YPosStart > dPosY || data.YPosEnd < dPosY))
                        isAvaliable[i] = false;
                }
            }

            bool isResult = true;
            for (int i = 0; i < count; i++)
                isResult &= isAvaliable[i];

            return isResult;
        }


        /// <summary>
        /// 이전공정  Lot명으로 각가의 INSPDAT를 조회
        /// </summary>
        /// <param name="bcno">현재 생산중인 BCNO</param>
        /// <param name="dPosY">현재 생산중인 원단의 원단장 위치</param>
        /// <returns></returns>
        public bool SearchMatchedBCNOLot(string bcno, double dPosY)
        {
            bool success = true;
            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            List<INSPDATData>[] inspDat = new List<INSPDATData>[count];
            for (int i = 0; i < count; i++)
                inspDat[i] = new List<INSPDATData>();

            // 각 공정별로 탐색
            for (int i = 0; i < count; i++)
            {
                foreach(List<INSPDATData> data in _DbResult.INSPDAT_Data[i])
                {
                    foreach(INSPDATData datum in data)
                    {
                        // 매칭되면 데이터를 넣어준다. 
                        if(datum.BCNO == bcno && (datum.YPosStart <= dPosY && datum.YPosEnd >= dPosY))
                            inspDat[i].Add(datum);
                    }
                }
            }
            
            _DbResult.Matched_INSPDAT_Data = inspDat;

            return success;
        }

        public bool SearchPTRYOP(string lotID)
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;

            try
            {
                DB_Progress._CurrentStep = eNittoDBProgress.PTRYOP;
                QueryMsg.PTRYOP_Query msg = new QueryMsg.PTRYOP_Query(lotID);

                // PTRLYP에서 획득한 Lot Data  만큼 쿼리 탐색 구문 추가
                string query = msg.GetQuery(null);
                _LOG.WriteLoadData(query, 0, "PTRY0P", 0.0);

                if (query == "")
                {
                    Log.Write($"[Error] DB Serach PTRYOP query is empty.");
                    DB_Progress.SetError(eNittoDBProgress.PTRYOP);
                    return false;
                }

                long dbCnt = 0;
                string logData = "";
                int logCnt = 0;
                DB_Progress.Reset(eNittoDBProgress.PTRYOP);
                using (var comm = new OracleCommand(query, conn.Connection))
                {
                    using (OracleDataReader reader = comm.ExecuteReader())
                    {
                        dbCnt = reader.RowSize;
                        DB_Progress.Set(eNittoDBProgress.PTRYOP);

                        while (reader.Read())
                        {
                            string strYOKLOT = reader[7].ToString();
                            string strY0LNSN = reader[9].ToString();
                            int nY0PPCD = Int32.Parse(reader[2].ToString());

                            if (Char.IsLetter(strYOKLOT, 0) == true)
                            {
                                strYOKLOT = strYOKLOT.Substring(0, 10); // 나중에 사이즈는 설정해야함.
                            }
                            else
                            {
                                int pos = strYOKLOT.IndexOf(' ');
                                if (pos > 0)
                                    strYOKLOT = strYOKLOT.Substring(0, pos);
                            }

                            PTRY0PData data = new PTRY0PData();
                            data.Parse(reader);

                            // 연신
                            if (nY0PPCD == 100)
                            {
                                _DbResult.PTRY0P_Data[(int)eFCD.ES].Add(data);
                                logCnt = _DbResult.PTRY0P_Data[(int)eFCD.ES].Count;
                            }

                            // 도공
                            if (nY0PPCD == 400)
                            {
                                _DbResult.PTRY0P_Data[(int)eFCD.TG].Add(data);
                                logCnt = _DbResult.PTRY0P_Data[(int)eFCD.TG].Count;
                            }

                            // 그외
                            if (nY0PPCD != 100 && nY0PPCD != 400)
                            {
                                _DbResult.PTRY0P_Data[(int)eFCD.ETC].Add(data);
                                logCnt = _DbResult.PTRY0P_Data[(int)eFCD.ETC].Count;
                            }

                            logData = string.Format($"{logCnt}\t-\t{data.ToString()}");
                            _LOG.WriteLoadData(logData, logCnt, "PTRY0P", 0.0);
                        }
                    }
                }
                DB_Progress.Complete(eNittoDBProgress.PTRYOP);
                return true;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach PTRYOP error message : [{ex.Message}]");
                DB_Progress.SetError(eNittoDBProgress.PTRYOP);
                return false;
            }
        }

        public bool SearchINSPDAT(string lotID)
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;

            int procStep = 0;
            try
            {
                long dbCnt = 0;
                int dataCnt = 0;
                bool useXOffset = destConfig.UseXOffset;

                eCSV_TYPE csvType = destConfig.GetCsvType();

                int count = System.Enum.GetValues(typeof(eFCD)).Length;

                for (int idx = 0; idx < count; idx++)
                {
                    DB_Progress._CurrentStep = ((eNittoDBProgress)((int)eNittoDBProgress.INSPDAT_ES + idx));
                    procStep = idx;
                    DB_Progress.Reset((eNittoDBProgress)((int)eNittoDBProgress.INSPDAT_ES + idx));

                    DB_Progress.Set((eNittoDBProgress)((int)eNittoDBProgress.INSPDAT_ES + idx));

                    int PTRY0Pcnt = _DbResult.PTRY0P_Data[idx].Count;

                    for (int i = 0; i < PTRY0Pcnt; i++)
                    {
                        string query = "";
                        QueryMsg.INSPDATA_Query msg = new QueryMsg.INSPDATA_Query(lotID);
                        msg.LNCD = _DbResult.PTRY0P_Data[idx][i].LNCD;
                        msg.SetTime(_DbResult.PTRY0P_Data[idx][i].StartTime, QueryMsg.INSPDATA_Query.eTargetTime.TimeStart);
                        msg.SetTime(_DbResult.PTRY0P_Data[idx][i].EndTime, QueryMsg.INSPDATA_Query.eTargetTime.TimeEnd);

                        query = msg.GetQuery(1, dbOption);
                        
                        _LOG.WriteLoadData(query, 0, "INSPDAT", 0.0);

                        if (query == "")
                        {
                            Log.Write($"[Error] INSPDAT{((eFCD)i).ToString()} Query message is empty.");
                            if ((eFCD)i == eFCD.ES) DB_Progress.SetError(eNittoDBProgress.INSPDAT_ES);
                            if ((eFCD)i == eFCD.TG) DB_Progress.SetError(eNittoDBProgress.INSPDAT_TG);
                            if ((eFCD)i == eFCD.ETC) DB_Progress.SetError(eNittoDBProgress.INSPDAT_ETC);
                            return false;
                        }

                        List<INSPDATData> inspDataList = new List<INSPDATData>();

                        using (var comm = new OracleCommand(query, conn.Connection))
                        {
                            using (var reader = comm.ExecuteReader())
                            {
                                dbCnt = reader.RowSize;

                                while (reader.Read())
                                {

                                    INSPDATData data = new INSPDATData();
                                    data.Y0KLOT = _DbResult.PTRY0P_Data[idx][i].Y0KLOT;
                                    data.Parse(reader);

                                    // 리스트에 데이터 추가함
                                    inspDataList.Add(data);
                                    dataCnt++;
                                    _LOG.WriteLoadData(data.ToString(), dataCnt, "INSPDAT", 0.0);
                                }
                            }
                        }
                        // 최종 데이터 입력
                        _DbResult.INSPDAT_Data[idx].Add(inspDataList);
                    }

                    DB_Progress.Complete((eNittoDBProgress)((int)eNittoDBProgress.INSPDAT_ES + idx));
                }

                return true;
            }
            catch (Exception ex)
            {
                if (procStep == (int)eFCD.ES) DB_Progress.SetError(eNittoDBProgress.INSPDAT_ES);
                else if (procStep == (int)eFCD.TG) DB_Progress.SetError(eNittoDBProgress.INSPDAT_TG);
                else if (procStep == (int)eFCD.ETC) DB_Progress.SetError(eNittoDBProgress.INSPDAT_ETC);
                Log.Write($"[Error] INSPDAT_{((eFCD)procStep).ToString()} error message : [{ex.Message}]");
                return false;
            }
        }

        public bool SearchFLTDAT()
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;

            DestConfigUnit destUnit = destConfig.SelDestUnit;
            if (destUnit == null)
            {
                destConfig.SetSelDest(dbOption.FWPlace);
                destConfig.SelDestUnit = destUnit;
            }

            bool useXOffset = destConfig.UseXOffset;
            bool useAIFromDB = DbOption.useAIfromDB;

            float maxXPos = 0;
            float minXPos = float.MaxValue;
            float minSize = float.MaxValue;

            eCSV_TYPE csvType = destConfig.GetCsvType();

            int defectLine;
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

            try
            {
                for (int fcdIdx = 0; fcdIdx < fcdCnt; fcdIdx++)
                {
                    // 데이터 초기화
                    maxXPos = 0;
                    minXPos = float.MaxValue;
                    minSize = float.MaxValue;

                    // 
                    procStep = fcdIdx;
                    DB_Progress._CurrentStep = ((eNittoDBProgress)((int)eNittoDBProgress.FAULTDAT_ES + fcdIdx));
                    DB_Progress.Reset((eNittoDBProgress)((int)eNittoDBProgress.FAULTDAT_ES + fcdIdx));

                    if (dbOption.checkES == true && fcdIdx == (int)eFCD.ES)         defectCnt[fcdIdx] = -1;// 확인 안 함
                    else if (dbOption.checkTG == true && fcdIdx == (int)eFCD.TG)    defectCnt[fcdIdx] = -1;// 확인 안 함
                    else if (dbOption.checkETC == true && fcdIdx == (int)eFCD.ETC)  defectCnt[fcdIdx] = -1;// 확인 안 함

                    int nItemCnt = 0;

                    if (_DbResult.Matched_INSPDAT_Data[fcdIdx] == null) continue;
                    
                    DB_Progress.Set((eNittoDBProgress)((int)eNittoDBProgress.FAULTDAT_ES + fcdIdx));
                    
                    nItemCnt = 0;

                    if (_DbResult.Matched_INSPDAT_Data[fcdIdx] == null) continue;

                    int inspCnt = _DbResult.Matched_INSPDAT_Data[fcdIdx].Count;
                    for (int inspIdx = 0; inspIdx < inspCnt; inspIdx++)
                    {
                        if (_DbResult.Matched_INSPDAT_Data[fcdIdx][inspIdx] == null) continue;

                        inspdata = _DbResult.Matched_INSPDAT_Data[fcdIdx][inspIdx];
                        
                        // 매칭 불량 갯수 초기화
                        inspdata.RollCtlCnt = 0;

                        QueryMsg.FLTDAT_Daily_Query msg = new QueryMsg.FLTDAT_Daily_Query();
                        msg.CTLNO = inspdata.CTLNO;
                        query = msg.GetQuery();

                        _LOG.WriteLoadData(query, 0, "FAULTDAT", 0.0);

                        if (query == "")
                        {
                            Log.Write($"[Error] FAULTDAT_{((eFCD)fcdIdx).ToString()} Query message is empty.");
                            if ((eFCD)fcdIdx == eFCD.ES) DB_Progress.SetError(eNittoDBProgress.FAULTDAT_ES);
                            if ((eFCD)fcdIdx == eFCD.TG) DB_Progress.SetError(eNittoDBProgress.FAULTDAT_TG);
                            if ((eFCD)fcdIdx == eFCD.ETC) DB_Progress.SetError(eNittoDBProgress.FAULTDAT_ETC);
                            return false;
                        }

                        using (var comm = new OracleCommand(query, conn.Connection))
                        {
                            using (var reader = comm.ExecuteReader())
                            {
                                dbCnt = reader.RowSize;

                                nItemCnt++;

                                while (reader.Read())
                                {
                                    FLTDATAData data = new FLTDATAData();
                                    data.Parse(reader);

                                    tmpFaltID = data.FLTID.ToUpper();

                                    finalXPos = data.XPOS_M;
                                    if (useXOffset == true)     finalXPos += inspdata.OffsetX;
                                        
                                    if (useAIFromDB == false)
                                    {
                                        tmpKey = data.MNTTAN.TrimStart();
                                        if (string.IsNullOrEmpty(tmpKey))   tmpKey = data.FLTID;
                                    }
                                    else    tmpKey = data.FLTID;
                                        
                                    if (finalXPos < 0.0f) continue;

                                    // Fault Data 처리
                                    FaultDatum tmpFltData = new FaultDatum();

                                    tmpFltData.FLTNO = data.FLTNO;
                                    tmpFltData.OFFSET = data.OFFSET;
                                    tmpFltData.YPOS_M = data.YPOS_M;
                                    tmpFltData.XPOS_M = data.XPOS_M;

                                    // 코드 불량 카운트 증가
                                    if (inspdata.CTLNO == data.CTLNO)   inspdata.RollCtlCnt++;

                                    if (minXPos > data.XPOS_M) minXPos = data.XPOS_M;
                                    if (maxXPos < data.XPOS_M) maxXPos = data.XPOS_M;
                                    if (csvType == eCSV_TYPE.NITTO_RK || csvType == eCSV_TYPE.NITTO_RTS)
                                    {
                                        if (tmpFaltID == "610" || tmpFaltID == "611" || tmpFaltID == "612")
                                            if (minSize > data.AREA_M) minSize = data.AREA_M;
                                    }

                                    // fault data 추가
                                    tmpFltData.RANK = data.RANK;
                                    tmpFltData.KND = data.KND;
                                    tmpFltData.JIGCD = data.JIGCD;
                                    tmpFltData.MACNO = data.MACNO;

                                    // Marking fault data 추가
                                    MarkingFaultDatum markData = new MarkingFaultDatum();

                                    markData.BCNO = inspdata.BCNO;
                                    markData.FLTNO = data.FLTNO;
                                    markData.FAULTID = data.FLTID;
                                    markData.OFFSET = tmpFltData.OFFSET;
                                    markData.YPOS_M = tmpFltData.YPOS_M;
                                    markData.XPOS_M = tmpFltData.XPOS_M;
                                    markData.XOFFSET = inspdata.OffsetX;
                                    markData.UseCSVResult = false;
                                    markData.CAM_NO = data.CAMNO;
                                    markData.CTLNO = data.CTLNO;
                                    markData.SIZE = data.AREA_M;
                                    markData.MNTTID = data.MNTTAN;
                                    markData.MACNO = data.MACNO;

                                    if (data.CAMNO != 9) markData.XOFFSET_ALARM = inspdata.OffsetX;
                                    else                 markData.XOFFSET_ALARM = float.MaxValue;

                                    if (csvType == eCSV_TYPE.NITTO)
                                    {
                                        if (fcdIdx == (int)eFCD.TG) markData.DefectLine = 9; // 점착
                                        else                        markData.DefectLine = 8; // 그외
                                    }
                                    else if (csvType == eCSV_TYPE.NITTO_RTS || csvType == eCSV_TYPE.NITTO_RK || csvType == eCSV_TYPE.KORENO_RK_IJP)
                                    {
                                        if (fcdIdx == (int)eFCD.TG)         markData.DefectLine = 9; //점착 
                                        else if (fcdIdx == (int)eFCD.ES)    markData.DefectLine = 8; // 연신 - 기타
                                        else                                markData.DefectLine = 7; // 그외
                                    }
                                    else
                                    {
                                        if (fcdIdx == (int)eFCD.TG && dbOption.useKT == true) // 점착
                                        {
                                            int fldID = Int32.Parse(data.FLTID.Substring(data.FLTID.Length - 2));
                                            markData.DefectLine = getDefectFromFLTID(fldID);
                                            if (markData.DefectLine != 13) CrtParam.DBFaultCount[fldID]++;
                                        }
                                        else if ((fcdIdx == (int)eFCD.ES && dbOption.checkES == true) ||
                                            (fcdIdx == (int)eFCD.ETC && dbOption.checkETC == true))
                                        {
                                            markData.DefectLine = 0;
                                            CrtParam.ESFalutCount++; // 연신 결점 데이터 카운트 처리
                                        }
                                    }

                                    // User Defect Class에 등록된 FLTID는 별도 클래스로 구분
                                    defectLine = markData.DefectLine;

                                    if (CrtParam._UserDefectClass.UpdateDefectLine(tmpFaltID, ref defectLine) == true)
                                        markData.DefectLine = defectLine;

                                    //RK는 CAMNO별로 Defect Class 를 구분
                                    if (csvType == eCSV_TYPE.NITTO_RK || csvType == eCSV_TYPE.NITTO_RTS || csvType == eCSV_TYPE.KORENO_RK_IJP)
                                        markData.DefectLine += Global.MaxDefectLine * data.CAMNO;

                                    resultDefect[fcdIdx].Data.Add(tmpFltData);
                                    resultDefect[fcdIdx].MarkFault.Add(markData);

                                    dataCnt++;
                                    logData = data.GetString(dataCnt, markData.DefectLine, markData.BCNO, markData.XOFFSET);
                                    _LOG.WriteLoadData(logData, dataCnt, "FAULTDAT", 0.0);
                                    defectCnt[fcdIdx]++;
                                }
                            }
                        }
                    }
                    DB_Progress.Complete((eNittoDBProgress)((int)eNittoDBProgress.FAULTDAT_ES + fcdIdx));

                    if (minSize == 999.0)   minSize = 0;

                    //   Defect 사이즈 처리
                    resultDefect[fcdIdx].MarkFault.MinXPos = minXPos;
                    resultDefect[fcdIdx].MarkFault.MaxXPos = maxXPos;
                    resultDefect[fcdIdx].MarkFault.MinSize = minSize;
                }


                // 불량 체크
                bool isSuccess = true;
                if (DbDestConfig.CSVType == eCSV_TYPE.KORENO || DbDestConfig.CSVType == eCSV_TYPE.KORENO_RK || DbDestConfig.CSVType == eCSV_TYPE.KORENO_RK_IJP)
                {
                    // 하나라도 검색이 되었으면 OK
                    isSuccess = false;
                    for (int i = 0; i < fcdCnt; i++)
                    {
                        if (defectCnt[i] != -1 && defectCnt[i] == 0)// 갯수 확인 못했으면
                            isSuccess = true;
                    }
                }
                else
                {
                    // 전체가 다 불량이 있어야 OK
                    for (int i = 0; i < fcdCnt; i++)
                    {
                        if (defectCnt[i] != -1 && defectCnt[i] == 0)// 갯수 확인 못했으면
                            isSuccess &= false;
                    }
                }
                return true;
            }
            catch (Exception ex)
            {
                if (procStep == (int)eFCD.ES) DB_Progress.SetError(eNittoDBProgress.INSPDAT_ES);
                else if (procStep == (int)eFCD.TG) DB_Progress.SetError(eNittoDBProgress.INSPDAT_TG);
                else if (procStep == (int)eFCD.ETC) DB_Progress.SetError(eNittoDBProgress.INSPDAT_ETC);
                Log.Write($"[Error] FAULTDAT_{((eFCD)procStep).ToString()} error message : [{ex.Message}]");
                return false;
            }
        }

        private int getDefectFromFLTID(int id)
        {
            int defectLine = 13;

            switch ((eFLTID)id)
            {
                case eFLTID.JT_DOT_01: defectLine = 14; break;
                case eFLTID.GB_GIPPO_02: defectLine = 12; break;
                case eFLTID.PERIOD_GIPPO_03: defectLine = 18; break;
                case eFLTID.SAME_04: defectLine = 19; break;
                case eFLTID.SRKZ_05: defectLine = 17; break;
                case eFLTID.JT_LINE_06: defectLine = 15; break;
                case eFLTID.CROSS_07: defectLine = 11; break;
                case eFLTID.SCRATCH_09: defectLine = 10; break;

                default:
                    if ((id >= (int)eFLTID.JH_DOT_W_31 && id <= (int)eFLTID.JH_GROUP_37) ||
                        (id >= (int)eFLTID.MH_DOT_W_51 && id <= (int)eFLTID.MH_GROUP_57) ||
                        (id >= (int)eFLTID.NEL7_HJK_5_61 && id <= (int)eFLTID.NEL7_HJK_1_65) ||
                        (id >= (int)eFLTID.NEL8_W_71 && id <= (int)eFLTID.NEL8_WB_73) ||
                        (id >= (int)eFLTID.SAME2_81 && id <= (int)eFLTID.SAME8_87))
                    {
                        defectLine = id;
                    }
                    break;
            }

            return defectLine;
        }

        #region Daily Lot 탐색 후 생산 데이터 정보 확인하는 Thread
        private string crtBCNO = "";
        private double crtRollPosY = 0.0;
        private bool enableCheckINSPDAT=false; 
        private Thread CheckAvailableLotthread = null;

        public void StartCheckAvaliableINSPDAT()
        {
            StopCheckAvaliableINSPDAT();

            this.CheckAvailableLotthread = new Thread(this.threadCheckAvaliableINSPDAT);
            this.CheckAvailableLotthread.Start();
        }

        public void StopCheckAvaliableINSPDAT()
        {
            if (this.CheckAvailableLotthread != null)
            {
                this.CheckAvailableLotthread.Abort();
                this.CheckAvailableLotthread.Join(100);
                this.CheckAvailableLotthread = null;
            }
        }

        private void threadCheckAvaliableINSPDAT()
        {

            while(true)
            {
                // 검색
                if(enableCheckINSPDAT==false)
                {
                    Thread.Sleep(500);
                    continue;
                }

                //현재 생산하고 있는 랏이 데이터에 없으면 다음 Lot을 탐색한다. 
                if(IsCrtDataAvaliable(crtBCNO, crtRollPosY)==false)
                {
                    // 다음 랏을 기준으로 탐색한다.
                    string strLotID;
                    bool success;

                    if (_DbResult.PTRY0P_Today_Data.Count >= NextY0KLOTIdx)
                    {
                        OnPopupError("탐색 인덱스가 현재 존재하는 데이터 범위를 넘어섰습니다.");
                        return;
                    }

                    strLotID = _DbResult.PTRY0P_Today_Data[NextY0KLOTIdx].Y0KLOT;
                    success = SearchPTRYOP(strLotID);
                    if(success==false)
                    {
                        NextY0KLOTIdx++;
                        continue;
                    }
                    success = SearchINSPDAT(strLotID);
                    
                    if(SearchMatchedBCNOLot(crtBCNO, crtRollPosY)==true)
                    {
                        success = SearchFLTDAT();
                        OnEndSearchingAvailableLot();
                    }
                }

                // 
                Thread.Sleep(500);
            }
        }


        #endregion
    }
}
