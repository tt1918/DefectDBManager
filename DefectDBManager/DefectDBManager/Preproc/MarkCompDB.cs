using DefectDBManager.DB;
using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.Window;

namespace DefectDBManager.Preproc
{
    public class MarkCompDB
    {
        public OracleDbConnection Conn { get { return conn; } }
        private OracleDbConnection conn = null;

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

        // 생산 정보 데이터
        public PTRY0PList PTRY0PList_Data { get; private set; }

        public DBLotInfo.LotData _DbResult { get; set; }

        public PreProcResultData FaultData { get; set; }

        public IRollDefectInfo _RollDefectInfo { get; set; }
        public CSV_DEFECT_HEADER _CsvDefectHeader = null;
        public List<CSVLoadInfo> _CSVLoadInfo = null;

        public string SearchLotName { get; set; }

        #region Filter Param
        /// <summary>
        /// 탐색할 LineCode
        /// </summary>
        public string _SearchY0LNCD { get; private set; } = string.Empty;
        /// <summary>
        /// 품종 정보
        /// </summary>
        public string _ProductName { get; private set; } = string.Empty;
        private bool _isWildCard = false;
        /// <summary>
        /// 모델 정보
        /// </summary>
        public PreprocItem _PreprocItem { get; private set; } = null;
        #endregion


        public LogDB _LOG;

        // 상위 객체
        private object owner;
        private bool _doDiscon = true;

        public MarkCompDB(object parent, OracleDbConnection dbconn, bool disconnDB = true)
        {
            owner = parent;
            conn = dbconn;
            _DbResult = new DBLotInfo.LotData();
            _CSVLoadInfo = new List<CSVLoadInfo>();
            _LOG = new LogDB();

            PTRY0PList_Data = new PTRY0PList();
            _PreprocItem = new PreprocItem();

            _doDiscon = disconnDB;
        }

        ~MarkCompDB()
        {
            if (_doDiscon == true)
                conn?.Dispose();
        }

        public void SetFilterParam(string lncd, string productName, PreprocItem item)
        {
            _SearchY0LNCD = lncd;
            _ProductName = productName;
            _PreprocItem = item;

            // 품종 wild card 확인
            if (_ProductName.Length < 2)
            { 
                _isWildCard = false; 
            }
            else
            {
                if(_ProductName.ElementAt(0)=='*' && _ProductName.ElementAt(_ProductName.Length-1) == '*')
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

        public void ResetDataAll()
        {
            // DB 데이터 초기화
            _DbResult.ClearAll();
            _DbResult.ResetData_DE();

            // 각 광학별 불량 갯수 초기화
            _CSVLoadInfo.Clear();

            // 결점 데이터 초기화
            FaultData?.ResetAll();
        }

        private int GetLotSpliceCnt(string lotID)
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

        public bool SearchPTRYOPList(string lncd, DateTime startTime, DateTime endTime)
        {
            // 연결 확인
            if (conn?.IsConnected() == false) return false;

            try
            {
                _LOG.Lot = $"[{lncd}] PTRY0PList" + startTime.ToString("yyyyMMdd");

                // Daily Lot DATA 내용을 초기화 한다 
                PTRY0PList_Data.Clear();

                QueryMsg.PTRY0PList_Query ptry0p = new QueryMsg.PTRY0PList_Query();

                //////////////////////////////////////////////////////
                // 해당 LNCD는 상위에서 입력받은 LNCD임
                // 검색 시간은 찾고자 하는 검색 시간대를 입력함.
                ptry0p.Y0LNCD = lncd;
                ptry0p.DateCurrent = startTime;
                ptry0p.DateNext = endTime;
                ///////////////////////////////////////////////////////

                string query = ptry0p.GetQuery();
                string listFileName = $"[{lncd}] PTRY0PList";
                _LOG.WriteLoadData(query.ToString(), 0, listFileName, 0);

                if (query == "")
                {
                    Log.Write($"[Error] DB Serach {listFileName} query is empty.");
                }

                using (var comm = new OracleCommand(query, conn.Connection))
                {
                    using (var reader = comm.ExecuteReader())
                    {
                        while (reader.Read())
                        {
                            PTRY0PData data = new PTRY0PData();
                            data.Parse(reader);

                            if (data.Y0KKOL.Substring(8) == "000000" && data.Y0KSOL.Substring(8) == "000000")
                                continue;

                            // 우선 전체 데이터 넣는다.
                            PTRY0PList_Data.Add(data);

                            string logData = string.Format($"{PTRY0PList_Data.Count}\t-\t{data.ToString()}");
                            _LOG.WriteLoadData(logData, 0, listFileName, 0);
                        }
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

        public bool SearchPTRYOPList_TEST(string lncd, DateTime startTime, DateTime endTime)
        {
            try
            {
                string path = $"[{lncd}] PTRY0PList" + startTime.ToString("yyyyMMdd");
                path = Path.Combine(Define.BCRPath, path, $"[{lncd}] PTRY0PList_DBResult.txt");

                // Daily Lot DATA 내용을 초기화 한다 
                PTRY0PList_Data.Clear();

                using (var file = new StreamReader(path, Encoding.Default))
                {
                    string text;
                    while ((text = file.ReadLine())!=null)
                    {
                        if (text.Contains("SELECT") == true) continue;

                        PTRY0PData data = new PTRY0PData();
                        data.Parse(text);

                        if (data.Y0KKOL.Substring(8) == "000000" && data.Y0KSOL.Substring(8) == "000000")
                            continue;

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


        public PreprocLot SearchLot(string lotID, bool renewal, bool bMsgOut, ref int errOut)
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return null;
            bool success = false;
            try
            {
                ResetDataAll();

                lotID = lotID.ToUpper();

                this.SearchLotName = lotID;
                if (lotID.Substring(0, 2) == "TG" || lotID.Substring(0, 2) == "TS")
                    dbOption.useKT = true;
                else
                    dbOption.useKT = false;

                // 이전 랏데이터 확인해서 데이터가 있으면 넘김
                int newLotCnt = GetLotSpliceCnt(lotID);
                if (newLotCnt > 0)
                {
                    // 재갱신 데이터가 아니면 업데이트 안하고 스킵함.
                    if (renewal == false)
                        return null;

                    _LOG.DeleteFolder(lotID);
                }

                _LOG.Lot = lotID;

                QueryMsg.PTRYLP_Query ptrylp = new QueryMsg.PTRYLP_Query(lotID);
                string query = ptrylp.GetQuery();
                long dbCnt = 0;
                _LOG.WriteLoadData(query.ToString(), 0, "PTRYLP", 0);

                if (query == "")
                {
                    errOut = 1;
                    Log.Write($"[Error] DB Serach PTRYLP query is empty.");
                }

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
                            _LOG.WriteLoadData(logData, 0, "PTRYLP", 0);
                        }

                        success = true;
                    }
                }

                if (success == false)
                {
                    errOut = 2;
                    return null;
                }
                success = SearchXOFSMST(lotID);
                if (success == false) { errOut = 3; return null; }

                success = SearchPTRY0P(lotID);
                if (success == false) { errOut = 4; return null; }

                success = SearchINSPDAT(lotID);
                if (success == false) { errOut = 6; return null; }

                // 첫 검사 랏은 복사하여둔다
                InspDatToFCDArray();

                success = SearchFLTDAT();
                if (success == false) { errOut = -7; return null; }

                // 처리 완료되면 데이터 정리

                return new PreprocLot(lotID, _DbResult, FaultData);
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach Lot error message : [{ex.Message}]");
                return null;
            }
        }

        public PreprocLot SearchLot_TEST(string lotID, bool renewal, bool bMsgOut, ref int errOut)
        {
            bool success = false;
            try
            {
                ResetDataAll();

                lotID = lotID.ToUpper();

                this.SearchLotName = lotID;
                if (lotID.Substring(0, 2) == "TG" || lotID.Substring(0, 2) == "TS")
                    dbOption.useKT = true;
                else
                    dbOption.useKT = false;
                _LOG.Lot = lotID;

                string path = Path.Combine(Define.BCRPath, lotID, "PTRYLP_DBResult.txt");

                using (var reader = new StreamReader(path, Encoding.UTF8))
                {
                    string text;
                    while ((text = reader.ReadLine())!=null)
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
                    errOut = 2;
                    return null;
                }
                success = SearchXOFSMST_TEST(lotID);
                if (success == false) { errOut = 3; return null; }

                success = SearchPTRY0P_TEST(lotID);
                if (success == false) { errOut = 4; return null; }

                success = SearchINSPDAT_TEST(lotID);
                if (success == false) { errOut = 6; return null; }

                // 첫 검사 랏은 복사하여둔다
                InspDatToFCDArray();

                success = SearchFLTDAT_TEST(lotID);
                if (success == false) { errOut = -7; return null; }

                // 처리 완료되면 데이터 정리
                return new PreprocLot(lotID, _DbResult, FaultData);
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach Lot error message : [{ex.Message}]");
                return null;
            }
        }

        public bool SearchXOFSMST(string lotID)
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;

            try
            {
                QueryMsg.XOFSMST_Query msg = new QueryMsg.XOFSMST_Query(lotID);
                string query = msg.GetQuery();
                _LOG.WriteLoadData(query, 0, "XOFSMST", 0.0);
                if (query == "")
                {
                    Log.Write($"[Error] DB Serach XOFSMST query is empty.");
                    return false;
                }

                long dbCnt = 0;
                string logData;
                using (var comm = new OracleCommand(query, conn.Connection))
                {
                    using (var reader = comm.ExecuteReader())
                    {
                        dbCnt = reader.RowSize;
                        while (reader.Read())
                        {
                            XOFSMSTData data = new XOFSMSTData();
                            data.Parse(reader);
                            _DbResult.XOFSMST.Add(data);

                            logData = string.Format($"{_DbResult.XOFSMST.Count}\t-\t{data.ToString()}");
                            _LOG.WriteLoadData(logData, _DbResult.XOFSMST.Count, "XOFSMST", 0.0);
                        }
                    }
                }
                return true;
                ;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach XOFSMST error message : [{ex.Message}]");
                return false;
            }
        }
        public bool SearchXOFSMST_TEST(string lotID)
        {
            try
            {
                string path = Path.Combine(Define.BCRPath, lotID, "XOFSMST_DBResult.txt");
                using (var reader = new StreamReader(path, Encoding.UTF8))
                {
                    string text;
                    while ((text = reader.ReadLine())!=null)
                    {
                        if (text.Contains("SELECT") == true) continue;

                        XOFSMSTData data = new XOFSMSTData();
                        data.Parse(text);
                        _DbResult.XOFSMST.Add(data);
                    }
                }
                return true;
                ;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach XOFSMST error message : [{ex.Message}]");
                return false;
            }
        }


        public bool SearchPTRYLP(string lotID)
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;

            bool success = false;
            try
            {

                this.SearchLotName = lotID;
                _LOG.Lot = lotID;
                _LOG.DeleteFolder(lotID);

                QueryMsg.PTRYLP_Query ptrylp = new QueryMsg.PTRYLP_Query(lotID);
                string query = ptrylp.GetQuery();
                long dbCnt = 0;
                _LOG.WriteLoadData(query.ToString(), 0, "PTRYLP", 0);

                if (query == "")
                {
                    Log.Write($"[Error] DB Serach PTRYLP query is empty.");
                }

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
                            _LOG.WriteLoadData(logData, 0, "PTRYLP", 0);
                        }
                        success = true;
                    }
                }

                return success;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach PTRY0P error message : [{ex.Message}]");
                return false;
            }
        }

        public bool SearchPTRY0P(string lotID)
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;

            try
            {
                QueryMsg.PTRY0P_Query msg = new QueryMsg.PTRY0P_Query(lotID);

                // PTRLYP에서 획득한 Lot Data  만큼 쿼리 탐색 구문 추가
                string query = msg.GetQuery(_DbResult.PTRLYP);
                _LOG.WriteLoadData(query, 0, "PTRY0P", 0.0);

                if (query == "")
                {
                    Log.Write($"[Error] DB Serach PTRY0P query is empty.");
                    return false;
                }

                long dbCnt = 0;
                string logData = "";
                int logCnt = 0;
                using (var comm = new OracleCommand(query, conn.Connection))
                {
                    using (OracleDataReader reader = comm.ExecuteReader())
                    {
                        dbCnt = reader.RowSize;

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
                                _DbResult.PTRY0P[(int)eFCD.ES].Add(data);
                                logCnt = _DbResult.PTRY0P[(int)eFCD.ES].Count;
                            }

                            // 도공
                            if (nY0PPCD == 400)
                            {
                                _DbResult.PTRY0P[(int)eFCD.TG].Add(data);
                                logCnt = _DbResult.PTRY0P[(int)eFCD.TG].Count;
                            }

                            // 그외
                            if (nY0PPCD != 100 && nY0PPCD != 400)
                            {
                                _DbResult.PTRY0P[(int)eFCD.ETC].Add(data);
                                logCnt = _DbResult.PTRY0P[(int)eFCD.ETC].Count;
                            }

                            logData = string.Format($"{logCnt}\t-\t{data.ToString()}");
                            _LOG.WriteLoadData(logData, logCnt, "PTRY0P", 0.0);
                        }
                    }
                }
                return true;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach PTRY0P error message : [{ex.Message}]");
                return false;
            }
        }

        public bool SearchPTRY0P_TEST(string lotID)
        {

            try
            {
                string path = Path.Combine(Define.BCRPath, lotID, "PTRY0P_DBResult.txt");
                string inspPath = Path.Combine(Define.BCRPath, lotID, "INSPDAT_DBResult.txt");
                using (var reader = new StreamReader(path, Encoding.UTF8))
                {
                    string text;
                    while ((text=reader.ReadLine())!=null)
                    {
                        if (text.Contains("SELECT") == true) continue;

                        PTRY0PData data = new PTRY0PData();
                        data.Parse(text);

                        string strYOKLOT = data.Y0KLOT;
                        string strY0LNSN = data.Y0LNSN;

                        // DATA 갖고 오지 않아서 INSPDATA에서 찾아야 함
                        int nY0PPCD = 420;
                        using (var inspRD = new StreamReader(inspPath, Encoding.UTF8))
                        {
                            string text1;
                            while((text1 = inspRD.ReadLine())!=null)
                            {
                                if (text1.Contains("SELECT") == true) continue;

                                INSPDATData inspData = new INSPDATData();
                                inspData.Parse(text1);

                                if(data.LNCD == inspData.USEFLG)
                                {
                                    nY0PPCD = Int32.Parse(inspData.KTCD.ToString());
                                    break;
                                }

                            }
                        }

                        if (Char.IsLetter(strYOKLOT, 0) == true)
                            strYOKLOT = strYOKLOT.Substring(0, 10); // 나중에 사이즈는 설정해야함.
                        else
                        {
                            int pos = strYOKLOT.IndexOf(' ');
                            if (pos > 0)
                                strYOKLOT = strYOKLOT.Substring(0, pos);
                        }

                        // 연신
                        if (nY0PPCD == 100)
                            _DbResult.PTRY0P[(int)eFCD.ES].Add(data);

                        // 도공
                        if (nY0PPCD == 400)
                            _DbResult.PTRY0P[(int)eFCD.TG].Add(data);

                        // 그외
                        if (nY0PPCD != 100 && nY0PPCD != 400)
                            _DbResult.PTRY0P[(int)eFCD.ETC].Add(data);
                    }
                }
                return true;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach PTRY0P error message : [{ex.Message}]");
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
                    procStep = idx;
                    int PTRY0Pcnt = _DbResult.PTRY0P[idx].Count;

                    for (int i = 0; i < PTRY0Pcnt; i++)
                    {
                        string query = "";
                        QueryMsg.INSPDATA_Query msg = new QueryMsg.INSPDATA_Query(lotID);
                        msg.LNCD = _DbResult.PTRY0P[idx][i].LNCD;
                        msg.SetTime(_DbResult.PTRY0P[idx][i].StartTime, QueryMsg.INSPDATA_Query.eTargetTime.TimeStart);
                        msg.SetTime(_DbResult.PTRY0P[idx][i].EndTime, QueryMsg.INSPDATA_Query.eTargetTime.TimeEnd);

                        query = msg.GetQuery(1, dbOption);

                        _LOG.WriteLoadData(query, 0, "INSPDAT", 0.0);

                        if (query == "")
                        {
                            Log.Write($"[Error] INSPDAT{((eFCD)i).ToString()} Query message is empty.");
                            return false;
                        }

                        INSPDATList inspDataList = new INSPDATList();

                        using (var comm = new OracleCommand(query, conn.Connection))
                        {
                            using (var reader = comm.ExecuteReader())
                            {
                                dbCnt = reader.RowSize;

                                while (reader.Read())
                                {

                                    INSPDATData data = new INSPDATData();
                                    data.Y0KLOT = _DbResult.PTRY0P[idx][i].Y0KLOT;
                                    data.LNCD = _DbResult.PTRY0P[idx][i].LNCD;
                                    data.Parse(reader);

                                    // 리스트에 데이터 추가함
                                    inspDataList.Add(data);
                                    dataCnt++;
                                    _LOG.WriteLoadData(data.ToString(), dataCnt, "INSPDAT", 0.0);
                                }
                            }
                        }
                        // 최종 데이터 입력
                        _DbResult.INSPDAT[idx].Add(inspDataList);
                    }
                }

                return true;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] INSPDAT_{((eFCD)procStep).ToString()} error message : [{ex.Message}]");
                return false;
            }
        }

        public bool SearchINSPDAT_TEST(string lotID)
        {
            int procStep = 0;
            try
            {
                long dbCnt = 0;
                int dataCnt = 0;
                bool useXOffset = destConfig.UseXOffset;

                eCSV_TYPE csvType = destConfig.GetCsvType();

                Dictionary<string, INSPDATList> dicList = new Dictionary<string, INSPDATList>();

                string path = Path.Combine(Define.BCRPath, lotID, "INSPDAT_DBResult.txt");
                using (var reader = new StreamReader(path, Encoding.UTF8))
                {
                    string text;
                    while ((text = reader.ReadLine()) != null)
                    {
                        if (text.Contains("SELECT") == true) continue;

                        INSPDATData data = new INSPDATData();
                        data.Parse(text);

                        foreach(var opList in _DbResult.PTRY0P)
                        {
                            foreach(var op in opList.Data)
                            {
                                if(op.LNCD == data.USEFLG)
                                {
                                    data.Y0KLOT = op.Y0KLOT;
                                    data.LNCD = op.LNCD;

                                    if(dicList.ContainsKey(op.LNCD)==true)
                                        dicList[op.LNCD].Add(data);
                                    else
                                    {
                                        dicList.Add(op.LNCD, new INSPDATList());
                                        dicList[op.LNCD].Add(data);
                                    }
                                }
                            }
                        }
                    }
                }

                foreach(var item in dicList)
                {
                    if(item.Value.Count>0)
                    {
                        if (item.Value[0].KTCD == "100")
                            _DbResult.INSPDAT[0].Add(item.Value);
                        else if(item.Value[0].KTCD == "400")
                            _DbResult.INSPDAT[1].Add(item.Value);
                        else
                            _DbResult.INSPDAT[2].Add(item.Value);
                    }
                }
                return true;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] INSPDAT_{((eFCD)procStep).ToString()} error message : [{ex.Message}]");
                return false;
            }
        }


        public bool SearchFLTDAT()
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;

            float maxXPos = 0;
            float minXPos = float.MaxValue;

            bool useXOffset = false;
            bool useAIFromDB = false;
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

                        // 매칭 불량 갯수 초기화
                        inspdata.RollCtlCnt = 0;

                        QueryMsg.FLTDAT_Daily_Query msg = new QueryMsg.FLTDAT_Daily_Query();
                        query = msg.GetQuery(inspdata.CTLNO);

                        _LOG.WriteLoadData(query, 0, "FAULTDAT", 0.0);

                        if (query == "")
                        {
                            Log.Write($"[Error] FAULTDAT_{((eFCD)fcdIdx).ToString()} Query message is empty.");
                            return false;
                        }
                        
                        ProcessData mkcdLncdData = null;
                        eProcDataType dataTarget = eProcDataType.None;
                        using (var comm = new OracleCommand(query, conn.Connection))
                        {
                            using (var reader = comm.ExecuteReader())
                            {
                                dbCnt = reader.RowSize;

                                PreprocMrkDat preMarkData = new PreprocMrkDat();
                                preMarkData.LNCD = inspdata.LNCD;


                                // LNCD 데이터를 기준으로 Reference/Compare 중에서 선택함. 
                                if (inspdata.LNCD == _PreprocItem.Reference.LNCD)
                                {
                                    mkcdLncdData = _PreprocItem.Reference;
                                    dataTarget = eProcDataType.Reference;
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

                                while (reader.Read())
                                {
                                    FLTDATA_DailyData data = new FLTDATA_DailyData();
                                    data.Parse(reader);

                                    tmpFaltID = data.FLTID.ToUpper();


                                    finalXPos = data.XPOS_M;
                                    if (useXOffset == true) finalXPos += inspdata.OffsetX;
                                    if (useAIFromDB == false)
                                    {
                                        tmpKey = data.MNTTAN.TrimStart();
                                        if (string.IsNullOrEmpty(tmpKey)) tmpKey = data.FLTID;
                                    }
                                    else tmpKey = data.FLTID;

                                    // MKCD Model에서 데이터 가져와서 다시 탐색함. 
                                    bValid = false;
                                    if (mkcdLncdData != null)
                                    {
                                        bValid = mkcdLncdData.IsValidFLTID(tmpKey, data.AREA_M + 0.00001f);
                                        if(bValid==false)   continue;
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

                                    if (dataTarget == eProcDataType.Reference)
                                        FaultData.MarkData.Add(markData);
                                    else// 마킹 대상 결점
                                        preMarkData.Data.Add(markData); // 이전 비교 공정 데이터

                                    dataCnt++;
                                    logData = data.GetString(dataCnt, tmpFltData.TBCNO);
                                    _LOG.WriteLoadData(logData, dataCnt, "FAULTDAT", 0.0);
                                    defectCnt[fcdIdx]++;
                                }

                                // 그렇지 않고 Compare Data이면 PreMarkData에 입력
                                if (dataTarget == eProcDataType.Compare)
                                    FaultData.PreMarkData[fcdIdx].Add(preMarkData);
                            }
                        }
                    }
                }

                // 불량 체크
                bool isSuccess = FalutFunction.IsDefectExist(DbDestConfig.CSVType, defectCnt);
                return true;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] FAULTDAT_{((eFCD)procStep).ToString()} error message : [{ex.Message}]");
                return false;
            }
        }

        public bool SearchFLTDAT_TEST(string lotID)
        {
            float maxXPos = 0;
            float minXPos = float.MaxValue;

            bool useXOffset = false;
            bool useAIFromDB = false;
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
                        // LNCD 데이터를 기준으로 Reference/Compare 중에서 선택함. 
                        if (inspdata.LNCD == _PreprocItem.Reference.LNCD)
                        {
                            mkcdLncdData = _PreprocItem.Reference;
                            dataTarget = eProcDataType.Reference;
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
                        string path = Path.Combine(Define.BCRPath, lotID, "FAULTDAT_DBResult.txt");
                        using (var reader = new StreamReader(path, Encoding.UTF8))
                        {
                            string text;
                            while((text=reader.ReadLine())!=null)
                            {
                                // 데이터 맞는지 확인
                                if(text.Contains("SELECT")==true && text.Contains(inspdata.CTLNO)==true)
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
                                        if (useAIFromDB == false)
                                        {
                                            tmpKey = data.MNTTAN.TrimStart();
                                            if (string.IsNullOrEmpty(tmpKey)) tmpKey = data.FLTID;
                                        }
                                        else tmpKey = data.FLTID;

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

                                        if (dataTarget == eProcDataType.Reference)
                                            FaultData.MarkData.Add(markData);
                                        else// 마킹 대상 결점
                                            preMarkData.Data.Add(markData); // 이전 비교 공정 데이터

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
                return true;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] FAULTDAT_{((eFCD)procStep).ToString()} error message : [{ex.Message}]");
                return false;
            }
        }

        /// <summary>
        ///  현재 랏 생산할 데이터가 확인이 되면 다음 예약랏 FAULTDAT 데이터 탐색 위해서 전체 복사한다.
        ///  
        /// </summary>
        public void InspDatToFCDArray()
        {
            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            INSPDATList[] inspDat = new INSPDATList[count];
            for (int i = 0; i < count; i++)
                inspDat[i] = new INSPDATList();

            // 각 공정별로 탐색
            for (int i = 0; i < count; i++)
            {
                foreach (INSPDATList data in _DbResult.INSPDAT[i])
                    inspDat[i].Copy(data);
            }

            _DbResult.INSPDATArray = inspDat;
        }

        /// <summary>
        /// 현재 검사 중인 Lot의 INSPDAT 공정 별 갯수
        /// </summary>
        /// <returns></returns>
        public int[] GetCurrentInspDatCnt()
        {
            int size = System.Enum.GetValues(typeof(eFCD)).Length;

            int[] count = new int[size];

            // FLTDAT에 정보 담겨 있어서 INSPDAT 대신에 FLTDAT 검색 결과로 대신 처리함.
            for (int i = 0; i < size; i++)
                count[i] = FaultData.FLTDAT[i].Count;

            return count;
        }

        /// <summary>
        /// 각 해당 공정의 불량 데이터를 얻어온다. 
        /// </summary>
        /// <param name="fcd"> 공정 유형 0: 연신, 1: 점착, 2: 그외 </param>
        /// <param name="index"> 검색하고자 하는 전공정 인덱스 </param>
        /// <param name="LNCD"> 출력할 라인 코드 </param>
        /// <param name="pts"> 결점 정보 </param>
        public void GetSelectedPreprocDefects(eFCD fcd, int index, out string LNCD, out List<System.Drawing.PointF> pts)
        {
            LNCD = "";
            pts = new List<System.Drawing.PointF>();

            // 리스트 크기 얻어옴
            int size = FaultData.FLTDAT[(int)fcd].Count;

            // 입력 인덱스랑 크기 비교
            if (index >= size) return;

            // 해당 공정 코드 얻어옴.
            LNCD = FaultData.FLTDAT[(int)fcd][index].LNCD;

            // FltData 크기 얻어옴
            int fltSize = FaultData.FLTDAT[(int)fcd][index].Data.Count;

            // Data 검색해서 추가
            foreach (FaultDatum item in FaultData.FLTDAT[(int)fcd][index].Data.Data)
            {
                System.Drawing.PointF pt = new System.Drawing.PointF();
                pt.X = item.XPOS_M;
                pt.Y = (float)item.OFFSET;
                pts.Add(pt);
            }
        }
    }
}
