#define FAST_FLTID

using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.RegularExpressions;
using System.IO;
using System.Diagnostics;
using System.Security.Cryptography;
using static System.Net.WebRequestMethods;
using System.Data.Entity;
using static System.Net.Mime.MediaTypeNames;
using static System.Runtime.CompilerServices.RuntimeHelpers;
using System.Data;
using System.Threading;

namespace DefectDBManager
{
    public delegate void DelegateDBConnect(bool state);
    public class OracleDbConnection : IDisposable
    {
        public string DBConnString
        {
            get;
            private set;
        }

        public DBLoginInfo LoginInfo { get; set; }

        public OracleConnection Connection
        {
            get { return conn; }
        }
        private OracleConnection conn = null;

        bool disposed = false;

        public bool bDBConnCheck = false;

        public event DelegateDBConnect OnDbConnect = null;

        public OracleDbConnection()
        {
            LoginInfo = new DBLoginInfo();
        }

        ~OracleDbConnection()
        {
            if (this.disposed)
                return;
            this.Dispose(false);
        }

        public void Dispose()
        {
            this.Dispose(true);
            GC.SuppressFinalize(this);
        }

        private void Dispose(bool disposing)
        {
            if (this.disposed)
                return;
            if (disposing)
            {
                Close();
            }
            this.disposed = true;
        }

        private bool connectToDB(string dbConn)
        {
            if (conn == null)
            {
                conn = new OracleConnection(dbConn);
            }
            else
            {
                if (conn.State == System.Data.ConnectionState.Open)
                    conn.Close();

                conn.ConnectionString = dbConn;
            }

            try
            {
                if (!IsDBConnected)
                {
                    conn.Open();
                    if (conn.State == System.Data.ConnectionState.Open)
                    {
                        bDBConnCheck = true;
                        if(this.OnDbConnect!=null)
                            this.OnDbConnect(true);
                        Log.Write("DB 연결에 성공하였습니다.");
                    }
                    else
                    {
                        bDBConnCheck = false;
                        Log.Write("DB 연결에 실패하였습니다.");
                    }
                }
            }
            catch (Exception e)
            {
                string message = String.Format($"[Error] DB Login is Failed. Message : {e.Message}");
                Log.Write(message);
            }

            return true;
        }

        public bool IsDBConnected
        {
            get
            {
                if (conn == null) return false;

                if (conn.State != System.Data.ConnectionState.Open)
                    return false;

                return true;
            }
        }

        public void Close()
        {
            if (IsDBConnected)
            {
                this.bDBConnCheck = false;
                Connection?.Close();
                Connection?.Dispose();
            }
        }

        public bool IsConnected()
        {
            if (conn == null) return false;

            return IsDBConnected;
        }

        public bool Connect()
        {
            if (IsDBConnected == true) return true;

            if (LoginInfo.StringType == 0)
            {
                DBConnString = String.Format($"Data Source={LoginInfo.Name};" +
                             $"User Id={LoginInfo.ID};Password={LoginInfo.PW};Connection Timeout=30;");
            }
            else if (LoginInfo.StringType == 1)
            {
                DBConnString = String.Format("Data Source=(DESCRIPTION="
                            + $"(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST={LoginInfo.IP})(PORT={LoginInfo.Port})))"
                            + $"(CONNECT_DATA=(SERVER=DEDICATED)(SERVICE_NAME={LoginInfo.Name})));"
                            + $"User Id={LoginInfo.ID};Password={LoginInfo.PW}");
            }

            connectToDB(DBConnString);

            return true;
        }

        public void Disconnect()
        {
            if (conn == null)
                return;

            conn.Close();
            conn = null;
        }
    }

    public class NittoDB
    {
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

        public DbSearchResult _DbResult
        {
            get;
            set;
        }

        public ResultData ResultDefect
        {
            get { return resultDefect; }
            set { resultDefect = value; }
        }
        private ResultData resultDefect;

        public IRollDefectInfo _RollDefectInfo { get; set; }
        public CSV_DEFECT_HEADER _CsvDefectHeader = null;
        public List<CSVLoadInfo> _CSVLoadInfo = null;

        public NittoDBProgress DB_Progress { get; private set; }

        public string SearchLotName { get; set; }

        public LogDB _LOG;
        // 상위 객체
        private object owner;

        public NittoDB(object parent, OracleDbConnection dbconn)
        {
            owner = parent;
            conn = dbconn;
            _DbResult = new DbSearchResult();
            DB_Progress = new NittoDBProgress();
            _CSVLoadInfo = new List<CSVLoadInfo>();
            _LOG = new LogDB();
        }

        ~NittoDB()
        {
            conn?.Dispose();
        }

        public void ResetDataAll()
        {
            _DbResult.ClearAll();

            // 각 광학별 불량 갯수 초기화
            CrtParam.ClearEachOpticDefectCnt();

            ResultDefect.ResetAll();
            _CSVLoadInfo.Clear();

            CrtParam.isProductAvaliable = false;
            CrtParam.isXOffsetError = false;
        }

        /// <summary>
        /// DB 검색 시 dest unit split 사용에 따른 데이터 초기화 처리
        /// </summary>
        public void ResetDataSplit()
        {
            _DbResult.ClearSplit();

            // 각 광학별 불량 갯수 초기화
            CrtParam.ClearEachOpticDefectCnt();

            // fault data 초기화. marking 데이터는 삭제하지 않음.
            resultDefect.Data.Clear();

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

        public bool SearchModel(string lotID, bool bMsgOut = true)
        {
            bool isRes = true;
            CrtParam.Model = "";
            try
            {
                lotID = lotID.ToUpper();

                eCSV_TYPE type = DbDestConfig.CSVType;
                if (type == eCSV_TYPE.NITTO || type == eCSV_TYPE.NITTO_RTS || type == eCSV_TYPE.NITTO_RK)
                {
                    // 이전 랏데이터 확인해서 스플라이스 처리해야 함
                    int newLotCnt = GetNextLotCnt(lotID);
                    if (newLotCnt > 0)_LOG.Lot = $"{lotID}_{newLotCnt:D2}";
                    else _LOG.Lot = lotID;
                }

                int count = 0;
                string strData;

                if (SearchModelList != null)
                    SearchModelList.Clear();
                else
                    SearchModelList = new List<string>();

                DB_Progress.SetSkip(eNittoDBProgress.PTRYLP);
                DB_Progress.SetSkip(eNittoDBProgress.XOFSMST);
                DB_Progress.SetSkip(eNittoDBProgress.AREADEL);

                if (SearchPTRY0P_Model(lotID) == true)
                {
                    int fcdCnt = System.Enum.GetValues(typeof(eFCD)).Length;
                    for (int i = 0; i < fcdCnt; i++)
                    {
                        for (int j = 0; j < _DbResult.PTRY0P[i].Count; i++)
                        {
                            if (_DbResult.PTRY0P[i][j].Y0ZKNM.Length > 0)
                            {
                                count++;
                                CrtParam.Model = _DbResult.PTRY0P[i][j].Y0ZKNM;
                                strData = string.Format($"{count}\t-\t{_DbResult.PTRY0P[i][j].Y0ZKNM}");
                                _LOG.WriteLoadData(strData, count, "MODEL", 0.0);
                                isRes = true;
                                SearchModelList.Add(_DbResult.PTRY0P[i][j].Y0ZKNM);
                                break;
                            }
                        }
                    }
                }

                if (count > 0)
                {

                }
                return isRes;
            }
            catch (Exception ex)
            {
                string strLog = $"[Error] DB Serach Model error message : [{ex.Message}]";
                Trace.WriteLine(strLog);
                Log.Write(strLog);
                return false;
            }
        }

        public bool SearchModelDummy(string lotID, bool bMsgOut = true)
        {
            bool isRes = true;
            CrtParam.Model = "";
            try
            {
                lotID = lotID.ToUpper();

                eCSV_TYPE type = DbDestConfig.CSVType;
                if (type == eCSV_TYPE.NITTO || type == eCSV_TYPE.NITTO_RTS || type == eCSV_TYPE.NITTO_RK)
                {
                    // 이전 랏데이터 확인해서 스플라이스 처리해야 함
                    int newLotCnt = GetNextLotCnt(lotID);
                    if (newLotCnt > 0) _LOG.Lot = $"{lotID}_{newLotCnt:D2}";
                    else _LOG.Lot = lotID;
                }

                int count = 0;
                if (SearchModelList != null)
                    SearchModelList.Clear();
                else
                    SearchModelList = new List<string>();

                DB_Progress.SetSkip(eNittoDBProgress.PTRYLP);
                DB_Progress.SetSkip(eNittoDBProgress.XOFSMST);
                DB_Progress.SetSkip(eNittoDBProgress.AREADEL);

                System.Threading.Thread.Sleep(100);
                DB_Progress.Reset(eNittoDBProgress.PTRY0P);
                DB_Progress.Set(eNittoDBProgress.PTRY0P);
                System.Threading.Thread.Sleep(500);
                SearchModelList.Add("12345678");
                SearchModelList.Add("87654321");
                DB_Progress.Complete(eNittoDBProgress.PTRY0P);
                if (count > 0)
                {

                }
                return isRes;
            }
            catch (Exception ex)
            {
                string strLog = $"[Error] DB Serach Model error message : [{ex.Message}]";
                Trace.WriteLine(strLog);
                Log.Write(strLog);
                return false;
            }
        }

        public bool SearchLot(string lotID, bool bMsgOut, ref int errOut)
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;
            bool success = false;
            try
            {
                lotID = lotID.ToUpper();

                this.SearchLotName = lotID;
                DB_Progress._CurrentStep = eNittoDBProgress.PTRYLP;
                if (lotID.Substring(0, 2) == "TG" || lotID.Substring(0, 2) == "TS")
                    dbOption.useKT = true;
                else
                    dbOption.useKT = false;

                // 이전 랏데이터 확인해서 스플라이스 처리해야 함
                int newLotCnt = GetNextLotCnt(lotID);
                if (newLotCnt > 0) _LOG.Lot = $"{lotID}_{newLotCnt:D2}";
                else _LOG.Lot = lotID;
                DB_Progress.ResetAll();

                QueryMsg.PTRYLP_Query ptrylp = new QueryMsg.PTRYLP_Query(lotID);
                string query = ptrylp.GetQuery();
                long dbCnt = 0;
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
                        dbCnt = reader.RowSize;
                        DB_Progress.Set(eNittoDBProgress.PTRYLP);
                        while (reader.Read())
                        {
                            PTRYLPdata data = new PTRYLPdata();
                            data.Parse(reader);
                            _DbResult.PTRLYP.Add(data);
                            string logData = string.Format($"{_DbResult.PTRLYP.Count}\t-\t{data.ToString()}");
                            _LOG.WriteLoadData(logData, 0, "PTRYLP", 0);
                        }

                        DB_Progress.Complete(eNittoDBProgress.PTRYLP);
                        success = true;
                    }
                }

                if (success == false)
                {
                    DB_Progress.SetError(eNittoDBProgress.PTRYLP);
                    return false;
                }
                success = SearchXOFSMST(lotID);
                if (success == false) return false;

                if (DbDestConfig.UseAREADEL == true)
                {
                    success = SearchAreaDel(lotID, ref _DbResult.AREADEL);
                    if (success == false) return false;
                }
                success = SearchPTRY0P(lotID);
                if (success == false) return false;
                success = SearchMRKCTLMST(lotID);
                if (success == false) return false;
                success = SearchINSPDAT(lotID);
                if (success == false) return false;
                success = SearchFLTDAT(lotID);
                if (success == false) return false;
                if (DbDestConfig.CSVType == eCSV_TYPE.NITTO || DbDestConfig.CSVType == eCSV_TYPE.NITTO_RK || DbDestConfig.CSVType == eCSV_TYPE.NITTO_RTS)
                {
                    if (dbOption.dbWhen == eDbIdWhen.Now && dbOption.prodAvaliableSpan > 0)
                    {
                        CrtParam.isProductAvaliable = _DbResult.IsProductAvaliable(dbOption, _LOG);
                        CrtParam.isXOffsetError = CheckOffsetError();
                    }
                }
                return success;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach Lot error message : [{ex.Message}]");
                return false;
            }
        }

        public bool SearchAreaDelFromServer(string lotID, bool bMsgOut, ref int errOut)
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;
            bool success = false;
            try
            {
                AREADELList listAreaDel = new AREADELList();
                lotID = lotID.ToUpper();
                // 이전 랏데이터 확인해서 스플라이스 처리해야 함
                int newLotCnt = GetNextLotCnt(lotID);
                if (newLotCnt > 0) _LOG.Lot = $"{lotID}_{newLotCnt:D2}";
                else _LOG.Lot = lotID;

                success = SearchAreaDel(lotID, ref listAreaDel);
                _DbResult.AREADEL = listAreaDel;
                return success;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach Lot error message : [{ex.Message}]");
                return false;
            }
        }
        public bool SearchXOFSMST(string lotID)
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;

            try
            {
                DB_Progress._CurrentStep = eNittoDBProgress.XOFSMST;
                QueryMsg.XOFSMST_Query msg = new QueryMsg.XOFSMST_Query(lotID);
                string query = msg.GetQuery();
                _LOG.WriteLoadData(query, 0, "XOFSMST", 0.0);
                if (query == "")
                {
                    Log.Write($"[Error] DB Serach XOFSMST query is empty.");
                    DB_Progress.SetError(eNittoDBProgress.XOFSMST);
                    return false;
                }

                long dbCnt = 0;
                string logData;
                using (var comm = new OracleCommand(query, conn.Connection))
                {
                    using (var reader = comm.ExecuteReader())
                    {
                        DB_Progress.Reset(eNittoDBProgress.XOFSMST);
                        dbCnt = reader.RowSize;
                        DB_Progress.Set(eNittoDBProgress.XOFSMST);
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
                DB_Progress.Complete(eNittoDBProgress.XOFSMST);
                return true;
                ;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach XOFSMST error message : [{ex.Message}]");
                DB_Progress.SetError(eNittoDBProgress.XOFSMST);
                return false;
            }
        }

        public bool SearchAreaDel(string lotID, ref AREADELList listAreaDel)
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;

            try
            {
                DB_Progress._CurrentStep = eNittoDBProgress.AREADEL;
                QueryMsg.AREADEL_Query msg = new QueryMsg.AREADEL_Query(lotID);
                string query = msg.GetQuery();
                _LOG.WriteLoadData(query, 0, "AREADEL", 0.0);
                if (query == "")
                {
                    Log.Write($"[Error] DB Serach AREADEL query is empty.");
                    DB_Progress.SetError(eNittoDBProgress.AREADEL);
                    return false;
                }

                long dbCnt = 0;
                string logData;
                DB_Progress.Reset(eNittoDBProgress.AREADEL);
                using (var comm = new OracleCommand(query, conn.Connection))
                {
                    using (var reader = comm.ExecuteReader())
                    {
                        dbCnt = reader.RowSize;
                        DB_Progress.Set(eNittoDBProgress.AREADEL);

                        while (reader.Read())
                        {
                            AREADELData data = new AREADELData();
                            data.Parse(reader);
                            listAreaDel.Add(data);

                            logData = string.Format($"{listAreaDel.Count}\t-\t{data.ToString()}");
                            _LOG.WriteLoadData(logData, listAreaDel.Count, "AREADEL", 0.0);
                        }
                    }
                }
                DB_Progress.Complete(eNittoDBProgress.AREADEL);
                return true;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach AREADEL error message : [{ex.Message}]");
                DB_Progress.SetError(eNittoDBProgress.AREADEL);
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
                DB_Progress._CurrentStep = eNittoDBProgress.PTRY0P;
                QueryMsg.PTRY0P_Query msg = new QueryMsg.PTRY0P_Query(lotID);

                // PTRLYP에서 획득한 Lot Data  만큼 쿼리 탐색 구문 추가
                string query = msg.GetQuery(_DbResult.PTRLYP);
                _LOG.WriteLoadData(query, 0, "PTRY0P", 0.0);

                if (query == "")
                {
                    Log.Write($"[Error] DB Serach PTRY0P query is empty.");
                    DB_Progress.SetError(eNittoDBProgress.PTRY0P);
                    return false;
                }

                long dbCnt = 0;
                string logData = "";
                int logCnt = 0;
                DB_Progress.Reset(eNittoDBProgress.PTRY0P);
                using (var comm = new OracleCommand(query, conn.Connection))
                {
                    using (OracleDataReader reader = comm.ExecuteReader())
                    {
                        dbCnt = reader.RowSize;
                        DB_Progress.Set(eNittoDBProgress.PTRY0P);

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

                            //아래 구문은 Int형 범위초과로 에러...
                            //if (Int32.Parse(data.StartTime) == 0 || Int32.Parse(data.EndTime) == 0)
                            //    continue;

                            if (dbOption.checkES == true && nY0PPCD == 100)
                            {
                                _DbResult.PTRY0P[(int)eFCD.ES].Add(data);
                                logCnt = _DbResult.PTRY0P[(int)eFCD.ES].Count;
                            }

                            if (dbOption.checkTG == true && nY0PPCD == 400)
                            {
                                _DbResult.PTRY0P[(int)eFCD.TG].Add(data);
                                logCnt = _DbResult.PTRY0P[(int)eFCD.TG].Count;
                            }

                            if (dbOption.checkETC == true && nY0PPCD != 100 && nY0PPCD != 400)
                            {
                                _DbResult.PTRY0P[(int)eFCD.ETC].Add(data);
                                logCnt = _DbResult.PTRY0P[(int)eFCD.ETC].Count;
                            }

                            logData = string.Format($"{logCnt}\t-\t{data.ToString()}");
                            _LOG.WriteLoadData(logData, logCnt, "PTRY0P", 0.0);
                        }
                    }
                }
                DB_Progress.Complete(eNittoDBProgress.PTRY0P);
                return true;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach PTRY0P error message : [{ex.Message}]");
                DB_Progress.SetError(eNittoDBProgress.PTRY0P);
                return false;
            }
        }

        public bool SearchPTRY0P_Model(string lotID)
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;

            try
            {
                QueryMsg.PTRY0P_Query msg = new QueryMsg.PTRY0P_Query(lotID);

                // PTRLYP에서 획득한 Lot Data  만큼 쿼리 탐색 구문 추가
                string query = msg.GetQuery(_DbResult.PTRLYP, true);
                long dbCnt = 0;
                _LOG.WriteLoadData(query, 0, "PTRY0P_MODEL", 0.0);
                if (query == "")
                {
                    Log.Write($"[Error] DB Serach PTRY0P query is empty.");
                    DB_Progress.SetError(eNittoDBProgress.PTRY0P);
                    return false;
                }

                DB_Progress.Reset(eNittoDBProgress.PTRY0P);

                using (var comm = new OracleCommand(query, conn.Connection))
                {
                    using (OracleDataReader reader = comm.ExecuteReader())
                    {
                        dbCnt = reader.RowSize;
                        DB_Progress.Set(eNittoDBProgress.PTRY0P);

                        while (reader.Read())
                        {
                            string strYOKLOT = reader[7].ToString();
                            string strY0LNSN = reader[9].ToString();
                            int nY0PPCD = 0;
                            if (Int32.TryParse(reader[2].ToString(), out nY0PPCD) ==false )
                            {
                                continue;
                            }

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

                            if (long.Parse(data.StartTime) == 0 || long.Parse(data.EndTime) == 0)
                                continue;

                            if (nY0PPCD == 100)
                            {
                                _DbResult.PTRY0P[(int)eFCD.ES].Add(data);
                                string logData = string.Format($"{(int)eFCD.ES}\t-\t{data.ToString()}");
                                _LOG.WriteLoadData(logData, 0, "PTRY0P_MODEL_ES", 0.0);
                            }

                            if (nY0PPCD == 400)
                            {
                                _DbResult.PTRY0P[(int)eFCD.TG].Add(data);
                                string logData = string.Format($"{(int)eFCD.TG}\t-\t{data.ToString()}");
                                _LOG.WriteLoadData(logData, 0, "PTRY0P_MODEL_TG", 0.0);
                            }

                            if (nY0PPCD != 100 && nY0PPCD != 400)
                            {
                                _DbResult.PTRY0P[(int)eFCD.ETC].Add(data);
                                string logData = string.Format($"{(int)eFCD.ETC}\t-\t{data.ToString()}");
                                _LOG.WriteLoadData(logData, 0, "PTRY0P_MODEL_ETC", 0.0);
                            }
                        }
                    }
                }
                DB_Progress.Complete(eNittoDBProgress.PTRY0P);
                return true;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach PTRY0P_MODEL error message : [{ex.Message}]");
                DB_Progress.SetError(eNittoDBProgress.PTRY0P);
                return false;
            }
        }

        public bool SearchMRKCTLMST(string logID)
        {
            if (dbOption.searchOP.useDefectEdit)
                return searchMRKCTLMSTfromBuffer(logID);
            else
                return searchMRKCTLMSTfromDB(logID);
        }

        private bool searchMRKCTLMSTfromBuffer(string logID)
        {
            int procStep = 0;

            try
            {
                DestConfigUnit destUnit = destConfig.SelDestUnit;
                if(destUnit==null)
                {
                    destConfig.SetSelDest(dbOption.FWPlace);
                    destConfig.SelDestUnit = destUnit;
                }

                int fcdTotal = System.Enum.GetValues(typeof(eFCD)).Length;
                int dataCnt = 0;
                string logData = "";
                for (int fcdIdx = 0; fcdIdx < fcdTotal; fcdIdx++)
                {
                    procStep = fcdIdx;
                    int PTRY0Pcnt = _DbResult.PTRY0P[fcdIdx].Count;
                    DB_Progress.Reset((eNittoDBProgress)((int)eNittoDBProgress.MRKCTLMST_ES + fcdIdx));
                    _DbResult.CheckDicMRKCTLMSTSize(PTRY0Pcnt, fcdIdx);
                    DB_Progress.Set((eNittoDBProgress)((int)eNittoDBProgress.MRKCTLMST_ES + fcdIdx));
                    for (int ptry0Idx = 0; ptry0Idx < PTRY0Pcnt; ptry0Idx++)
                    {
                        _DbResult.ClearDicMRKCTLMST(fcdIdx, ptry0Idx);

                        // Defect Edit에 의해서 수정한 데이터를 실제 FLTID 검색 시 사용하도록 함.
                        foreach (MRKCTLMSTData data in _DbResult._MRKCTLMST_DE[fcdIdx][ptry0Idx].data)
                        {
                            _DbResult.AddDicMRKCTLMST(fcdIdx, ptry0Idx, data);
                            dataCnt++;

                            logData = string.Format($"{dataCnt}\t_\t{data.ToString()}");
                            _LOG.WriteLoadData(logData, dataCnt, "MRKCTLMST-BUFFER", 0.0);
                        }
                    }
                    DB_Progress.Complete((eNittoDBProgress)((int)eNittoDBProgress.MRKCTLMST_ES + fcdIdx));
                }
                return true;
            }
            catch (Exception ex)
            {
                DB_Progress.SetError((eNittoDBProgress)((int)eNittoDBProgress.MRKCTLMST_ES + procStep));
                Log.Write($"[Error] MRKCTLMST_Buffer_{((eFCD)procStep).ToString()} error message : [{ex.Message}]");
                return false;
            }
        }

        private bool searchMRKCTLMSTfromDB(string lotID)
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;

            int procStep = 0;

            try
            {
                DestConfigUnit destUnit = destConfig.SelDestUnit;
                if (destUnit == null)
                {
                    destConfig.SetSelDest(dbOption.FWPlace);
                    destConfig.SelDestUnit = destUnit;
                }

                long dbCnt = 0;
                int count = System.Enum.GetValues(typeof(eFCD)).Length;
                string logData = "";

                for (int i = 0; i < count; i++)
                {
                    DB_Progress._CurrentStep = ((eNittoDBProgress)((int)eNittoDBProgress.MRKCTLMST_ES + i));
                    procStep = i;
                    int PTRY0Pcnt = _DbResult.PTRY0P[i].Count;
                    DB_Progress.Reset((eNittoDBProgress)((int)eNittoDBProgress.MRKCTLMST_ES + i));
                    _DbResult.CheckDicMRKCTLMSTSize(PTRY0Pcnt, i);
                    DB_Progress.Set((eNittoDBProgress)((int)eNittoDBProgress.MRKCTLMST_ES + i));

                    for (int j = 0; j < PTRY0Pcnt; j++)
                    {
                        _DbResult.ClearDicMRKCTLMST(i, j);

                        if ((dbOption.checkES == true && (eFCD)i == eFCD.ES) ||
                           (dbOption.checkTG == true && (eFCD)i == eFCD.TG) ||
                           (dbOption.checkETC == true && (eFCD)i == eFCD.ETC) && _DbResult.PTRY0P[i][j].Y0KLOT.Length > 0)
                        {
                            QueryMsg.MRKCTLMST_Query msg = new QueryMsg.MRKCTLMST_Query();
                            msg.MKCD = dbOption.searchOP.MKCD;
                            msg.Y0KLOT = _DbResult.PTRY0P[i][j].Y0KLOT;
                            string query = msg.GetQuery((eFCD)i);
                            _LOG.WriteLoadData(query, 0, "MRKCTLMST", 0.0);

                            if (query == "")
                            {
                                Log.Write($"[Error] MRKCTLMST_{((eFCD)i).ToString()} Query message is empty.");
                                DB_Progress.SetError((eNittoDBProgress)((int)eNittoDBProgress.MRKCTLMST_ES + i));
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
                                        _LOG.WriteLoadData(logData, _DbResult.MRKCTLMST.Count, "MRKCTLMST", 0.0);
                                        // 조건문 추가해야 함
                                        CrtParam.MRKCTLMSTFLTID.Add(data.FLTID);
                                        _DbResult.AddDicMRKCTLMST(i, j, data);

                                        for (int checkCnt = 0; checkCnt < destUnit.FLTIDCheck.Length; checkCnt++)
                                        {
                                            if (destUnit.FLTIDCheck[checkCnt].Length > 0)
                                                if(destUnit.FLTIDCheck[checkCnt] == data.FLTID)
                                                CrtParam.MRKCTLMSTFLTID.Add(data.FLTID);
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            DB_Progress.SetSkip((eNittoDBProgress)((int)eNittoDBProgress.MRKCTLMST_ES + i));
                        }
                    }
                    DB_Progress.Complete((eNittoDBProgress)((int)eNittoDBProgress.MRKCTLMST_ES + i));
                }

                return true;
            }
            catch (Exception ex)
            {
                DB_Progress.SetError((eNittoDBProgress)((int)eNittoDBProgress.MRKCTLMST_ES + procStep));
                Log.Write($"[Error] MRKCTLMST_{((eFCD)procStep).ToString()} error message : [{ex.Message}]");
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
                _DbResult.ClearProductInfo();

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

                    if (dbOption.checkES == true && idx == (int)eFCD.ES)
                    {
                    }
                    else if (dbOption.checkTG == true && idx == (int)eFCD.TG)
                    {
                    }
                    else if (dbOption.checkETC == true && idx == (int)eFCD.ETC)
                    {
                    }
                    else
                    {
                        if (dbOption.checkES == false && idx == (int)eFCD.ES) DB_Progress.SetSkip(eNittoDBProgress.INSPDAT_ES);
                        if (dbOption.checkTG == false && idx == (int)eFCD.TG) DB_Progress.SetSkip(eNittoDBProgress.INSPDAT_TG);
                        if (dbOption.checkETC == false && idx == (int)eFCD.ETC) DB_Progress.SetSkip(eNittoDBProgress.INSPDAT_ETC);
                        continue;
                    }
                    DB_Progress.Set((eNittoDBProgress)((int)eNittoDBProgress.INSPDAT_ES + idx));

                    int PTRY0Pcnt = _DbResult.PTRY0P[idx].Count;

                    for (int i = 0; i < PTRY0Pcnt; i++)
                    {
                        string query = "";
                        QueryMsg.INSPDATA_Query msg = new QueryMsg.INSPDATA_Query(lotID);
                        msg.LNCD = _DbResult.PTRY0P[idx][i].LNCD;
                        msg.SetTime(_DbResult.PTRY0P[idx][i].StartTime, QueryMsg.INSPDATA_Query.eTargetTime.TimeStart);
                        msg.SetTime(_DbResult.PTRY0P[idx][i].EndTime, QueryMsg.INSPDATA_Query.eTargetTime.TimeEnd);

                        _DbResult.CheckAndUpdateProductInfo(idx, i, msg.EndTime);

                        if (dbOption.useESTime == true &&
                            ((dbOption.checkES && idx == (int)eFCD.ES) || (dbOption.checkETC && idx == (int)eFCD.ETC)))
                        {
                            query = msg.GetQuery(0, dbOption);
                        }
                        else
                        {
                            query = msg.GetQuery(1, dbOption);
                        }
                        _LOG.WriteLoadData(query, 0, "INSPDAT", 0.0);

                        if (query == "")
                        {
                            Log.Write($"[Error] INSPDAT{((eFCD)i).ToString()} Query message is empty.");
                            if ((eFCD)i == eFCD.ES) DB_Progress.SetError(eNittoDBProgress.INSPDAT_ES);
                            if ((eFCD)i == eFCD.TG) DB_Progress.SetError(eNittoDBProgress.INSPDAT_TG);
                            if ((eFCD)i == eFCD.ETC) DB_Progress.SetError(eNittoDBProgress.INSPDAT_ETC);
                            return false;
                        }

                        bool isBcnoFind = false;

                        List<INSPDATData> inspDataList = new List<INSPDATData>();

                        using (var comm = new OracleCommand(query, conn.Connection))
                        {
                            using (var reader = comm.ExecuteReader())
                            {
                                dbCnt = reader.RowSize;

                                while (reader.Read())
                                {

                                    INSPDATData data = new INSPDATData();
                                    data.Y0KLOT = _DbResult.PTRY0P[idx][i].Y0KLOT;
                                    data.Parse(reader);

                                    if (useXOffset)
                                    {
                                        int offsetDataCnt = _DbResult.XOFSMST.Count;

                                        for (int offsetIdx = 0; offsetIdx < offsetDataCnt; offsetIdx++)
                                        {
                                            int ppcd = _DbResult.XOFSMST[offsetIdx].PPCD;
                                            if (data.KYCD == _DbResult.XOFSMST[offsetIdx].KYCD &&
                                                ((idx == (int)eFCD.ES && ppcd == 100) ||
                                                (idx == (int)eFCD.TG && ppcd == 400) ||
                                                (idx == (int)eFCD.ETC && ppcd != 100 && ppcd != 400)) &&
                                                _DbResult.PTRY0P[idx][i].Y0ZKNM == _DbResult.XOFSMST[offsetIdx].YLSZKN &&
                                                _DbResult.PTRY0P[idx][i].LNCD == _DbResult.XOFSMST[offsetIdx].LNCD)
                                            {
                                                data.OffsetX = _DbResult.XOFSMST[offsetIdx].X_OFFSET;
                                            }
                                        }
                                    }

                                    int bcnoCnt = _DbResult.LoadedBcNo.Count;
                                    isBcnoFind = false;
                                    if (bcnoCnt < 10) // 9개가 넘어가면 BCD Wrong Error
                                    {
                                        for (int bcnoIdx = 0; bcnoIdx < bcnoCnt; bcnoIdx++)
                                        {
                                            if (_DbResult.LoadedBcNo[bcnoIdx] == data.BCNO)
                                            {
                                                isBcnoFind = true;
                                                break;
                                            }
                                        }

                                        if (bcnoCnt == 0)
                                        {
                                            _DbResult.LoadedBcNo.Add(data.BCNO);
                                            _DbResult.LoadedBcNo.Add(data.LOTNO);
                                        }
                                        else
                                        {
                                            if (isBcnoFind == false) _DbResult.LoadedBcNo.Add(data.BCNO);
                                        }
                                    }

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

        public bool SearchFLTDAT(string lotID)
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
            bool useXOffsetAlarm = destConfig.UseXOffsetAlarm;
            bool useMask = DbOption.searchOP.useMask;
            //bool bXOffsetError = false;
            bool useAIFromDB = DbOption.useAIfromDB;

            bool useSplit = DbOption.searchOP.useSplit;
            float splitStartX = DbOption.searchOP.splitStartX;
            float splitEndX = DbOption.searchOP.splitEndX;

            float maxXPos = 0;
            float minXPos = float.MaxValue;
            float minSize = float.MaxValue;

            eCSV_TYPE csvType = destConfig.GetCsvType();

            int defectLine;
            string tmpKey;
            float finalXPos;
            string tmpFaltID;

            INSPDATData inspdata;

            long dbCnt = 0;
            int fcdCnt = System.Enum.GetValues(typeof(eFCD)).Length;
            int dataCnt = 0;
            string logData = "";
            string query;
            int procStep = 0;

            bool bValid = false;

            int[] defectCnt = new int[fcdCnt];
            defectCnt.Initialize();

            try
            {
                for (int fcdIdx = 0; fcdIdx < fcdCnt; fcdIdx++)
                {
                    procStep = fcdIdx;
                    DB_Progress._CurrentStep = ((eNittoDBProgress)((int)eNittoDBProgress.FAULTDAT_ES + fcdIdx));
                    DB_Progress.Reset((eNittoDBProgress)((int)eNittoDBProgress.FAULTDAT_ES + fcdIdx));

                    if (dbOption.checkES == true && fcdIdx == (int)eFCD.ES)         defectCnt[fcdIdx] = -1;// 확인 안 함
                    else if (dbOption.checkTG == true && fcdIdx == (int)eFCD.TG)    defectCnt[fcdIdx] = -1;// 확인 안 함
                    else if (dbOption.checkETC == true && fcdIdx == (int)eFCD.ETC)  defectCnt[fcdIdx] = -1;// 확인 안 함
                    else
                    {
                        if (dbOption.checkES == false && fcdIdx == (int)eFCD.ES) DB_Progress.SetSkip(eNittoDBProgress.FAULTDAT_ES);
                        if (dbOption.checkTG == false && fcdIdx == (int)eFCD.TG) DB_Progress.SetSkip(eNittoDBProgress.FAULTDAT_TG);
                        if (dbOption.checkETC == false && fcdIdx == (int)eFCD.ETC) DB_Progress.SetSkip(eNittoDBProgress.FAULTDAT_ETC);

                        continue;
                    }

                    int nItemCnt = 0;
                    for (int iIdx = 0; iIdx < _DbResult.INSPDAT[fcdIdx].Count; iIdx++)
                    {
                        if (_DbResult.INSPDAT[fcdIdx][iIdx] == null) continue;
                        nItemCnt += _DbResult.INSPDAT[fcdIdx][iIdx].Count;
                    }
                        

                    DB_Progress.Set((eNittoDBProgress)((int)eNittoDBProgress.FAULTDAT_ES + fcdIdx));
                    nItemCnt = 0;
                    for (int opIdx = 0; opIdx < _DbResult.PTRY0P[fcdIdx].Count; opIdx++)
                    {
                        if (_DbResult.INSPDAT[fcdIdx][opIdx] == null) continue;

                        _DbResult.UpdateDicSizeData(fcdIdx, opIdx);
                        _DbResult.UpdateDicMRKF1Data(fcdIdx, opIdx);

                        int inspCnt = _DbResult.INSPDAT[fcdIdx][opIdx].Count;
                        for (int inspIdx = 0; inspIdx < inspCnt; inspIdx++)
                        {
                            if (_DbResult.INSPDAT[fcdIdx][opIdx][inspIdx] == null) continue;

                            inspdata = _DbResult.INSPDAT[fcdIdx][opIdx][inspIdx];

                            if (_DbResult.dicMRKF1Data.Count == 0 && _DbResult.dicSizeData.Count == 0)  continue;
#if (FAST_FLTID)
                            QueryMsg.FLTDAT_FAST_Query fastMsg = new QueryMsg.FLTDAT_FAST_Query();
                            fastMsg.CTLNO = inspdata.CTLNO;
                            query = fastMsg.GetQuery(_DbResult.dicSizeData, _DbResult.dicMRKF1Data);
#else
                            QueryMsg.FLTDAT_Query msg = new QueryMsg.FLTDAT_Query();
                            msg.CTLNO = inspdata.CTLNO;
                            query = msg.GetQuery();
#endif
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
                                        if (useXOffset == true)
                                            finalXPos += inspdata.OffsetX;
                                        
                                        if (useAIFromDB == false)
                                        {
                                            tmpKey = data.MNTTAN.TrimStart();
                                            //mnttid = mnttid.TrimStart('\s');
                                            if (string.IsNullOrEmpty(tmpKey))
                                                tmpKey = data.FLTID;
                                        }
                                        else
                                            tmpKey = data.FLTID;

                                        if (finalXPos < 0.0f)       continue;
                                        if (useMask == true && IsMaskedDefect(finalXPos, data.OFFSET) == true) continue;
                                        if (useSplit == true && isSplitSkipDefect(finalXPos, splitStartX, splitEndX) == true) continue;

                                        // Fault Data 처리
                                        FaultDatum tmpFltData = new FaultDatum();
                                        tmpFltData.SetData(inspdata.BCNO, data);

                                        // 코드 불량 카운트 증가
                                        if (inspdata.CTLNO == data.CTLNO)   inspdata.RollCtlCnt++;

                                        // FLTID비교기능
                                        if(destUnit.FLTIDCheck!=null)
                                        {
                                            for (int checkCnt = 0; checkCnt < destUnit.FLTIDCheck.Length; checkCnt++)
                                            {
                                                if (destUnit.FLTIDCheck[checkCnt].Length > 0 && destUnit.FLTIDCheck[checkCnt] == data.FLTID)
                                                    CrtParam.FAULTDATFLTID.Add(data.FLTID);
                                            }
                                        }
                                            
                                        if (minXPos > data.XPOS_M) minXPos = data.XPOS_M;
                                        if (maxXPos < data.XPOS_M) maxXPos = data.XPOS_M;
                                        if (csvType == eCSV_TYPE.NITTO_RK || csvType == eCSV_TYPE.NITTO_RTS)
                                        {
                                            if (tmpFaltID == "610" || tmpFaltID == "611" || tmpFaltID == "612")
                                                if (minSize > data.AREA_M) minSize = data.AREA_M;
                                        }

                                        // Marking fault data 추가
                                        MarkingFaultDatum markData = new MarkingFaultDatum();
                                        markData.SetFaultData((eFCD)fcdIdx, csvType, inspdata.BCNO, (float)tmpFltData.OFFSET, false, tmpFltData, data, dbOption, ref currentParam);
                                       
                                        resultDefect.Data.Add(tmpFltData);
                                        resultDefect.MarkFault.Add(markData);

                                        dataCnt++;
                                        logData = data.GetString(dataCnt, markData.DefectLine, markData.BCNO, markData.XOFFSET);
                                        _LOG.WriteLoadData(logData, dataCnt, "FAULTDAT", 0.0);
                                        defectCnt[fcdIdx]++;
                                    }
                                }
                            }
                        }
                    }
                    DB_Progress.Complete((eNittoDBProgress)((int)eNittoDBProgress.FAULTDAT_ES + fcdIdx));
                }

                if (DbDestConfig.CSVType == eCSV_TYPE.NITTO || DbDestConfig.CSVType == eCSV_TYPE.NITTO_RTS || DbDestConfig.CSVType == eCSV_TYPE.NITTO_RK)
                {
                    if(DbDestConfig.SelDestUnit.useFaltIDCheck==true)
                    {
                        CrtParam.VerifyFLTID();

                        if (checkInspectionRollMeter()==false)
                            CrtParam.InspRollCheckError = true;
                    }
                    
                }

                if (minSize == 999.0)   minSize = 0;

                //   Defect 사이즈 처리
                resultDefect.MarkFault.MinXPos = minXPos;
                resultDefect.MarkFault.MaxXPos = maxXPos;
                resultDefect.MarkFault.MinSize = minSize;

                // 불량 체크
                bool isSuccess = FalutFunction.IsDefectExist(DbDestConfig.CSVType, defectCnt);
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

        public bool CheckOffsetError()
        {
            bool bXOfSErr = false;
            float refXOffset = dbOption.xOffset;
            int count = 0;
            if (DbDestConfig.UseXOffset == true && DbDestConfig.UseXOffsetAlarm == true)
            {
                foreach (MarkingFaultDatum item in resultDefect.MarkFault.Data)
                {
                    if (item.XOFFSET_ALARM != float.MaxValue && (Math.Abs(item.XOFFSET_ALARM + 1.0f) > +0.000001f) && Math.Abs(refXOffset - item.XOFFSET_ALARM) >= 0.1f)
                    {
                        bXOfSErr = true;
                        string errData = string.Format($"ERROR X-Offset:{dbOption.dbWhen.ToString()}, {item.XOFFSET_ALARM:0.0}, Ref:{refXOffset:0.0}, index:{count}");
                        _LOG.WriteLoadData(errData, 0, "XOFSMST", 0.0);
                        break;
                    }
                    count++;
                }
            }

            return bXOfSErr;
        }

        private bool checkInspectionRollMeter()
        {
            bool bMatch = false;
            bool bFindSepa2 = false, bFindSepa3 = false;
            int i, j, k;
            int nFindSepa1 = 0, nFindSepa2 = 0, nFindSepa3 = 0;
            int nCompleteMCnt = 0, nINSPLengthCnt = 0, nINSPLengthCnt2 = 0;
            int nTmp = 0, nLength1, nLength2;
            float fLength = 0.0f, fStLength = 0.0f, fTmp;
            string strTmp;
            string strLotMatch;
            List<int> nIndex1 = new List<int>();
            List<int>[] nIndex2 = new List<int>[3];
            for(i=0; i<3; i++) nIndex2[i] = new List<int>();
            List<float> fINSPLength = new List<float>();

            nFindSepa1 = DbOption.lotName.IndexOf("LL");
            if (nFindSepa1 >= 0)
            {
                strLotMatch = DbOption.lotName.Substring(10);
                for (i = 0; i < _DbResult.PTRLYP.Count; i++)
                {
                    nFindSepa2 = _DbResult.PTRLYP[i].YLMLOT.IndexOf(strLotMatch);
                    if (nFindSepa2 >= 0)
                    {
                        if (_DbResult.PTRLYP[i].YLMKAS == 0.0f)
                            continue;
                        nIndex1.Add(i);
                        bFindSepa2 = true;
                        Log.Write($"PTRYLP LOT Match : {_DbResult.PTRLYP[i].YLMKAS}");
                    }
                }

                int fcdSize = System.Enum.GetValues(typeof(eFCD)).Length;
                for (i = 0; i < fcdSize; i++)
                {
                    for (j = 0; j < _DbResult.PTRLYP.Count; j++)
                    {
                        for (k = 0; k < _DbResult.INSPDAT[i][j].Count; k++)
                        {
                            nFindSepa3 = _DbResult.INSPDAT[i][j][k].LOTNO.IndexOf(strLotMatch);
                            if (nFindSepa3 >= 0)
                            {
                                if (_DbResult.INSPDAT[i][j][k].Length == 0.0f || nTmp == (int)_DbResult.INSPDAT[i][j][k].Length)
                                    continue;
                                nTmp = (int)_DbResult.INSPDAT[i][j][k].Length;
                                nIndex2[0].Add(i);
                                nIndex2[1].Add(j);
                                nIndex2[2].Add(k);
                                bFindSepa3 = false;
                                Log.Write($"INSP LOT Match : {_DbResult.INSPDAT[i][j][k].Length}");
                            }
                        }
                    }
                }
                if (bFindSepa2 && bFindSepa3)
                {
                    nCompleteMCnt = (int)nIndex1.Count;
                    nINSPLengthCnt = (int)nIndex2[0].Count;
                    fLength = 0.0f;
                    fTmp = 0.0f;
                    strTmp = "";
                    for (i = 0; i < nINSPLengthCnt; i++)
                    {
                        if (fStLength != _DbResult.INSPDAT[nIndex2[0][i]][nIndex2[1][i]][nIndex2[2][i]].YPosStart)
                        {
                            fStLength = _DbResult.INSPDAT[nIndex2[0][i]][nIndex2[1][i]][nIndex2[2][i]].YPosStart;
                            fTmp = _DbResult.INSPDAT[nIndex2[0][i]][nIndex2[1][i]][nIndex2[2][i]].Length;
                            fTmp = fTmp / 1000.0f;
                            fLength += fTmp;
                        }
                        if (strTmp != _DbResult.INSPDAT[nIndex2[0][i]][nIndex2[1][i]][nIndex2[2][i]].LOTNO)
                        {
                            if (strTmp == "")
                                strTmp = _DbResult.INSPDAT[nIndex2[0][i]][nIndex2[1][i]][nIndex2[2][i]].LOTNO;
                            else
                            {
                                strTmp = _DbResult.INSPDAT[nIndex2[0][i]][nIndex2[1][i]][nIndex2[2][i]].LOTNO;
                                fINSPLength.Add(fLength);
                                fLength = 0.0f;
                            }
                        }
                    }
                    fINSPLength.Add(fLength);
                    nINSPLengthCnt2 = (int)fINSPLength.Count;
                    for (i = 0; i < nINSPLengthCnt2; i++)
                    {
                        for (j = 0; j < nCompleteMCnt; j++)
                        {
                            nLength1 = (int)_DbResult.PTRLYP[nIndex1[i]].YLMKAS;
                            nLength2 = (int)fINSPLength[i];
                            if (nLength1 - DbDestConfig.SelDestUnit.LengErrorRangeMinus <= nLength2 && nLength1 + DbDestConfig.SelDestUnit.LengErrorRangePlus >= nLength2)
                                bMatch = true;
                            else
                                bMatch = false;
                            Log.Write($"CompleteMeter={nLength1} InspectionMeter={nLength2}");
                        }
                    }

                    Log.Write($"InspLengthCount={nINSPLengthCnt2} CompleteMeterCount={nCompleteMCnt}");
                }
                if (nINSPLengthCnt==0)
                    bMatch = false;
            }
            else
            {
                bMatch = false;
            }
            nIndex1.Clear();
            nIndex2[0].Clear();
            nIndex2[1].Clear();
            nIndex2[2].Clear();
            fINSPLength.Clear();
            return bMatch;
        }

        public bool IsMaskedDefect(float xPos, double yPos)
        {
            List<SkipOffsetParam> skip = CrtParam.OffsetSkip;
            int count = skip.Count;

            foreach (SkipOffsetParam param in skip)
            {
                if (param.CheckSkip(xPos, yPos) == true)
                    return true;
            }

            return false;
        }

        private bool isSplitSkipDefect(float xPos, float splitS, float splitE)
        {
            bool bSkip = false;
            if (xPos < splitS || xPos >= splitE)
                bSkip = true;
            return bSkip;
        }
    }
}
