//#define FAST_FLTID

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
        public string UserID
        {
            get { return userID; }
            set { userID = value; }
        }
        private string userID;

        public string Password
        {
            get { return password; }
            set { password = value; }
        }
        private string password;

        public string IP
        {
            get { return dbIP; }
            set { dbIP = value; }
        }
        private string dbIP;

        public string DBName
        {
            get { return dbName; }
            set { dbName = value; }
        }
        private string dbName;

        public int DBPort
        {
            get { return dbPort; }
            set { dbPort = value; }
        }
        private int dbPort;

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
                // 이거 나중에 정리해야할듯....
                dbConn = String.Format("Data Source=(DESCRIPTION="
              + "(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(HOST=172.29.2.24)(PORT=1521)))"
              + "(CONNECT_DATA=(SERVER=DEDICATED)(SERVICE_NAME=INSP)));"
              + "User Id=INSP;Password=INSP");

                conn = new OracleConnection(dbConn);
            }

            try
            {
                if (!IsDBConnected)
                {
                    conn.Open();
                    if (conn.State == System.Data.ConnectionState.Open)
                    {
                        bDBConnCheck = true;
                        this.OnDbConnect(true);
                        Log.WriteLog("DB 연결에 성공하였습니다.");
                    }
                    else
                    {
                        bDBConnCheck = false;
                        Log.WriteLog("DB 연결에 실패하였습니다.");
                    }
                }
            }
            catch (Exception e)
            {
                string message = String.Format($"[Error] DB Login is Failed. Message : {e.Message}");
                Log.WriteLog(message);
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

            DBConnString = String.Format($"Data Source={dbName};" +
                    $"User ID={UserID};Password={password};Connection Timeout=30;");

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

        public List<PTRYLPdata> PTRLYP_Data;
        public List<XOFSMSTData> XOFSMST_Data;
        public List<AREADELData> AREADEL_Data;
        public List<PTRY0PData>[] PTRY0P_Data;

        public List<MRKCTLMSTData> MRKCTLMST_Data;
        public List<Dictionary<string, float>>[] dicSizeMRKCTLMST;
        public List<Dictionary<string, bool>>[] dicMRKF1MRKCTLMST;
        public List<MRKCTLMST_DE_Data>[] _MRKCTLMST_DE;

        public List<List<INSPDATData>>[] INSPDAT_Data;

        public Dictionary<string, float> dicSizeData;
        public Dictionary<string, bool> dicMRKF1Data;

        public List<string> LoadedBcNo;

        public List<DateTime> ProductEndTime;
        public List<string> ProductLotName;

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

        public ResultData ResultDefect
        {
            get { return resultDefect; }
            set { resultDefect = value; }
        }
        private ResultData resultDefect;

        private List<MarkingFaultDatum> markFaultData;
        private List<FaultDatum> faultData;

        public IRollDefectInfo _RollDefectInfo { get; set; }
        public CSV_DEFECT_HEADER _CsvDefectHeader = null;

        public NittoDBProgress DB_Progress { get; private set; }

        public string SearchLotName { get; set; }


        // 상위 객체
        private object owner;


        public NittoDB(object parent, OracleDbConnection dbconn)
        {
            owner = parent;

            conn = dbconn;

            PTRLYP_Data = new List<PTRYLPdata>();
            XOFSMST_Data = new List<XOFSMSTData>();
            AREADEL_Data = new List<AREADELData>();

            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            PTRY0P_Data = new List<PTRY0PData>[count];
            for (int i = 0; i < count; i++)
                PTRY0P_Data[i] = new List<PTRY0PData>();

            INSPDAT_Data = new List<List<INSPDATData>>[count];
            for (int i = 0; i < count; i++)
                INSPDAT_Data[i] = new List<List<INSPDATData>>();

            MRKCTLMST_Data = new List<MRKCTLMSTData>();
            dicSizeMRKCTLMST = new List<Dictionary<string, float>>[count];
            dicMRKF1MRKCTLMST = new List<Dictionary<string, bool>>[count];
            _MRKCTLMST_DE = new List<MRKCTLMST_DE_Data>[count];
            for (int i = 0; i < count; i++)
            {
                dicSizeMRKCTLMST[i] = new List<Dictionary<string, float>>();
                dicMRKF1MRKCTLMST[i] = new List<Dictionary<string, bool>>();
                _MRKCTLMST_DE[i] = new List<MRKCTLMST_DE_Data>();
            }

            dicSizeData = new Dictionary<string, float>();

            LoadedBcNo = new List<string>();

            ProductEndTime = new List<DateTime>();
            ProductLotName = new List<string>();

            DB_Progress = new NittoDBProgress();
        }

        ~NittoDB()
        {
            conn?.Dispose();
        }

        public void ResetDataAll()
        {
            LoadedBcNo.Clear();
            ProductEndTime.Clear();
            ProductLotName.Clear();

            AREADEL_Data.Clear();
            XOFSMST_Data.Clear();
            PTRLYP_Data.Clear();
            MRKCTLMST_Data.Clear();

            for (int i = 0; i < PTRY0P_Data.Length; i++) PTRY0P_Data[i].Clear();

            for (int i = 0; i < INSPDAT_Data.Length; i++)
            {
                for (int j = 0; j < INSPDAT_Data[i].Count; j++)
                    INSPDAT_Data[i][j].Clear();
                INSPDAT_Data[i].Clear();
            }

            // 각 광학별 불량 갯수 초기화
            CrtParam.ClearEachOpticDefectCnt();

            ResultDefect.ResetAll();

            CrtParam.isProductAvaliable = false;
            CrtParam.isXOffsetError = false;
        }

        /// <summary>
        /// DB 검색 시 dest unit split 사용에 따른 데이터 초기화 처리
        /// </summary>
        public void ResetDataSplit()
        {
            PTRLYP_Data.Clear(); 
            AREADEL_Data.Clear();
            XOFSMST_Data.Clear();
            MRKCTLMST_Data.Clear();

            for (int i = 0; i < PTRY0P_Data.Length; i++) PTRY0P_Data[i].Clear();

            for (int i = 0; i < INSPDAT_Data.Length; i++)
            {
                for (int j = 0; j < INSPDAT_Data[i].Count; j++)
                    INSPDAT_Data[i][j].Clear();
                INSPDAT_Data[i].Clear();
            }

            // 각 광학별 불량 갯수 초기화
            CrtParam.ClearEachOpticDefectCnt();

            // fault data 초기화. marking 데이터는 삭제하지 않음.
            resultDefect.Data.Clear();

            CrtParam.isProductAvaliable = false;
            CrtParam.isXOffsetError = false;
        }

        public void ResetData_DE()
        {
            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            for (int i = 0; i < count; i++)
            {
                for (int j = 0; j < dicSizeMRKCTLMST[i].Count; j++)
                    dicSizeMRKCTLMST[i][j].Clear();

                for(int j=0; j< dicMRKF1MRKCTLMST[i].Count; j++)
                    dicMRKF1MRKCTLMST[i][j].Clear();

                _MRKCTLMST_DE[i].Clear();
            }
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
                    if (newLotCnt > 0) Log.LotLog = $"{lotID}_{newLotCnt:D2}";
                    else Log.LotLog = lotID;
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

                if (SearchPTRYOP_Model(lotID) == true)
                {
                    int fcdCnt = System.Enum.GetValues(typeof(eFCD)).Length;
                    for (int i = 0; i < fcdCnt; i++)
                    {
                        for (int j = 0; j < PTRY0P_Data[i].Count; i++)
                        {
                            if (PTRY0P_Data[i][j].Y0ZKNM.Length > 0)
                            {
                                count++;
                                CrtParam.Model = PTRY0P_Data[i][j].Y0ZKNM;
                                strData = string.Format($"{count}\t-\t{PTRY0P_Data[i][j].Y0ZKNM}");
                                Log.WriteLoadData(strData, count, "MODEL", 0.0);
                                isRes = true;
                                SearchModelList.Add(PTRY0P_Data[i][j].Y0ZKNM);
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
                Log.WriteLog(strLog);
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
                    if (newLotCnt > 0) Log.LotLog = $"{lotID}_{newLotCnt:D2}";
                    else Log.LotLog = lotID;
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

                System.Threading.Thread.Sleep(100);
                DB_Progress.Reset(eNittoDBProgress.PTRYOP);
                DB_Progress.Set(eNittoDBProgress.PTRYOP);
                System.Threading.Thread.Sleep(500);
                SearchModelList.Add("12345678");
                SearchModelList.Add("87654321");
                DB_Progress.Complete(eNittoDBProgress.PTRYOP);

                if (count > 0)
                {

                }
                return isRes;
            }
            catch (Exception ex)
            {
                string strLog = $"[Error] DB Serach Model error message : [{ex.Message}]";
                Trace.WriteLine(strLog);
                Log.WriteLog(strLog);
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

                if (lotID.Substring(0, 2) == "TG" || lotID.Substring(0, 2) == "TS")
                    CrtParam.UseKT = 1;
                else
                    CrtParam.UseKT = 0;

                // 이전 랏데이터 확인해서 스플라이스 처리해야 함
                int newLotCnt = GetNextLotCnt(lotID);
                if (newLotCnt > 0) Log.LotLog = $"{lotID}_{newLotCnt:D2}";
                else Log.LotLog = lotID;
                DB_Progress.ResetAll();

                QueryMsg.PTRYLP_Query ptrylp = new QueryMsg.PTRYLP_Query(lotID);
                string query = ptrylp.GetQuery();
                long dbCnt = 0;
                Log.WriteLoadData(query.ToString(), 0, "PTRYLP", 0);

                if (query == "")
                {
                    Log.WriteLog($"[Error] DB Serach PTRYLP query is empty.");
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
                            PTRLYP_Data.Add(data);
                            string logData = string.Format($"{PTRLYP_Data.Count}\t-\t{data.ToString()}");
                            Log.WriteLoadData(logData, 0, "PTRYLP", 0);
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
                success = SearchAreaDel(lotID);
                if (success == false) return false;
                success = SearchPTRYOP(lotID);
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
                        CrtParam.isProductAvaliable = IsProductAvaliable(dbOption.prodAvaliableSpan);
                        CrtParam.isXOffsetError = CheckOffsetError();
                    }
                }
                return success;
            }
            catch (Exception ex)
            {
                Log.WriteLog($"[Error] DB Serach Lot error message : [{ex.Message}]");
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
                QueryMsg.XOFSMST_Query msg = new QueryMsg.XOFSMST_Query(lotID);
                string query = msg.GetQuery();
                Log.WriteLoadData(query, 0, "XOFSMST", 0.0);
                if (query == "")
                {
                    Log.WriteLog($"[Error] DB Serach XOFSMST query is empty.");
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
                            XOFSMST_Data.Add(data);
                            
                            logData = string.Format($"{XOFSMST_Data.Count}\t-\t{data.ToString()}");
                            Log.WriteLoadData(logData, XOFSMST_Data.Count, "XOFSMST", 0.0);
                        }
                    }
                }
                DB_Progress.Complete(eNittoDBProgress.XOFSMST);
                return true;
;
            }
            catch (Exception ex)
            {
                Log.WriteLog($"[Error] DB Serach XOFSMST error message : [{ex.Message}]");
                DB_Progress.SetError(eNittoDBProgress.XOFSMST);
                return false;
            }
        }

        public bool SearchAreaDel(string lotID)
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;
            
            try
            {
                QueryMsg.AREADEL_Query msg = new QueryMsg.AREADEL_Query(lotID);
                string query = msg.GetQuery();
                Log.WriteLoadData(query, 0, "AREADEL", 0.0);
                if (query == "")
                {
                    Log.WriteLog($"[Error] DB Serach AREADEL query is empty.");
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
                            AREADEL_Data.Add(data);

                            logData = string.Format($"{AREADEL_Data.Count}\t-\t{data.ToString()}");
                            Log.WriteLoadData(logData, AREADEL_Data.Count, "AREADEL", 0.0);
                        }
                    }
                }
                DB_Progress.Complete(eNittoDBProgress.AREADEL);
                return true;
            }
            catch (Exception ex)
            {
                Log.WriteLog($"[Error] DB Serach AREADEL error message : [{ex.Message}]");
                DB_Progress.SetError(eNittoDBProgress.AREADEL);
                return false;
            }
        }

        public bool SearchPTRYOP(string lotID)
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;

            try
            {
                QueryMsg.PTRYOP_Query msg = new QueryMsg.PTRYOP_Query(lotID);

                // PTRLYP에서 획득한 Lot Data  만큼 쿼리 탐색 구문 추가
                string query = msg.GetQuery(PTRLYP_Data);
                Log.WriteLoadData(query, 0, "PTRY0P", 0.0);

                if (query == "")
                {
                    Log.WriteLog($"[Error] DB Serach PTRYOP query is empty.");
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
                            
                            //아래 구문은 Int형 범위초과로 에러...
                            //if (Int32.Parse(data.StartTime) == 0 || Int32.Parse(data.EndTime) == 0)
                            //    continue;

                            if (dbOption.checkES == true && nY0PPCD == 100)
                            {
                                PTRY0P_Data[(int)eFCD.ES].Add(data);
                                logCnt = PTRY0P_Data[(int)eFCD.ES].Count;
                            }

                            if (dbOption.checkTG == true && nY0PPCD == 400)
                            {
                                PTRY0P_Data[(int)eFCD.TG].Add(data);
                                logCnt = PTRY0P_Data[(int)eFCD.TG].Count;
                            }

                            if (dbOption.checkETC == true && nY0PPCD != 100 && nY0PPCD != 400)
                            {
                                PTRY0P_Data[(int)eFCD.ETC].Add(data);
                                logCnt = PTRY0P_Data[(int)eFCD.ETC].Count;
                            }

                            logData = string.Format($"{logCnt}\t-\t{data.ToString()}");
                            Log.WriteLoadData(logData, logCnt, "PTRY0P", 0.0);
                        }
                    }
                }
                DB_Progress.Complete(eNittoDBProgress.PTRYOP);
                return true;
            }
            catch (Exception ex)
            {
                Log.WriteLog($"[Error] DB Serach PTRYOP error message : [{ex.Message}]");
                DB_Progress.SetError(eNittoDBProgress.PTRYOP);
                return false;
            }
        }

        public bool SearchPTRYOP_Model(string lotID)
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;

            try
            {
                QueryMsg.PTRYOP_Query msg = new QueryMsg.PTRYOP_Query(lotID);

                // PTRLYP에서 획득한 Lot Data  만큼 쿼리 탐색 구문 추가
                string query = msg.GetQuery(PTRLYP_Data, true);
                long dbCnt = 0;
                Log.WriteLoadData(query, 0, "PTRYOP_MODEL", 0.0);
                if (query == "")
                {
                    Log.WriteLog($"[Error] DB Serach PTRYOP query is empty.");
                    DB_Progress.SetError(eNittoDBProgress.PTRYOP);
                    return false;
                }

                DB_Progress.Reset(eNittoDBProgress.PTRYOP);

                using (var comm = new OracleCommand(query, conn.Connection))
                {
                    using (OracleDataReader reader = comm.ExecuteReader())
                    {
                        dbCnt = reader.RowSize;
                        DB_Progress.Set(eNittoDBProgress.PTRYOP);

                        while (reader.Read())
                        {
                            string strYOKLOT = reader[8].ToString();
                            string strY0LNSN = reader[10].ToString();
                            int nY0PPCD = Int32.Parse(reader[3].ToString());

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

                            if (Int32.Parse(data.StartTime) == 0 || Int32.Parse(data.EndTime) == 0)
                                continue;

                            if (nY0PPCD == 100)
                            {
                                PTRY0P_Data[(int)eFCD.ES].Add(data);
                                string logData = string.Format($"{(int)eFCD.ES}\t-\t{data.ToString()}");
                                Log.WriteLoadData(logData, 0, "PTRYOP_MODEL_ES", 0.0);
                            }

                            if (nY0PPCD == 400)
                            {
                                PTRY0P_Data[(int)eFCD.TG].Add(data);
                                string logData = string.Format($"{(int)eFCD.TG}\t-\t{data.ToString()}");
                                Log.WriteLoadData(logData, 0, "PTRYOP_MODEL_TG", 0.0);
                            }

                            if (nY0PPCD != 100 && nY0PPCD != 400)
                            {
                                PTRY0P_Data[(int)eFCD.ETC].Add(data);
                                string logData = string.Format($"{(int)eFCD.ETC}\t-\t{data.ToString()}");
                                Log.WriteLoadData(logData, 0, "PTRYOP_MODEL_ETC", 0.0);
                            }
                        }
                    }
                }
                DB_Progress.Complete(eNittoDBProgress.PTRYOP);
                return true;
            }
            catch (Exception ex)
            {
                Log.WriteLog($"[Error] DB Serach PTRYOP_MODEL error message : [{ex.Message}]");
                DB_Progress.SetError(eNittoDBProgress.PTRYOP);
                return false;
            }
        }

        private void checkDicMRKCTLMST(int targetCnt, int fcdIdx)
        {
            // Dic 부족한 인덱스 만큼 초기화 처리
            if (targetCnt > dicSizeMRKCTLMST[fcdIdx].Count)
                for (int dicIdx = dicSizeMRKCTLMST[fcdIdx].Count; dicIdx < targetCnt; dicIdx++)
                    dicSizeMRKCTLMST[fcdIdx].Add(new Dictionary<string, float>());

            if (targetCnt > dicMRKF1MRKCTLMST[fcdIdx].Count)
                for (int dicIdx = dicMRKF1MRKCTLMST[fcdIdx].Count; dicIdx < targetCnt; dicIdx++)
                    dicMRKF1MRKCTLMST[fcdIdx].Add(new Dictionary<string, bool>());
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
                DestConfigUnit destUnit = new DestConfigUnit();
                destConfig.GetData(dbOption.FWPlace, ref destUnit);

                int fcdTotal = System.Enum.GetValues(typeof(eFCD)).Length;
                int dataCnt = 0;
                int valMRKF1 = 0;
                bool boolMRKF1  = false;
                string logData = "";
                for (int fcdIdx = 0; fcdIdx < fcdTotal; fcdIdx++)
                {
                    procStep = fcdIdx;
                    int PTRY0Pcnt = PTRY0P_Data[fcdIdx].Count;
                    DB_Progress.Reset((eNittoDBProgress)((int)eNittoDBProgress.MRKCTLMST_ES + fcdIdx));
                    checkDicMRKCTLMST(PTRY0Pcnt, fcdIdx);
                    DB_Progress.Set((eNittoDBProgress)((int)eNittoDBProgress.MRKCTLMST_ES + fcdIdx));
                    for (int ptry0Idx = 0; ptry0Idx < PTRY0Pcnt; ptry0Idx++)
                    {
                        dicSizeMRKCTLMST[fcdIdx][ptry0Idx].Clear();
                        dicMRKF1MRKCTLMST[fcdIdx][ptry0Idx].Clear();

                        foreach (MRKCTLMSTData data in _MRKCTLMST_DE[fcdIdx][ptry0Idx].data)
                        {
                            if (dicSizeMRKCTLMST[fcdIdx][ptry0Idx].ContainsKey(data.FLTID) == true)
                                dicSizeMRKCTLMST[fcdIdx][ptry0Idx][data.FLTID] = data.SIZE;
                            else
                                dicSizeMRKCTLMST[fcdIdx][ptry0Idx].Add(data.FLTID, data.SIZE);

                            valMRKF1 = Int32.Parse(data.MRKF1);
                            boolMRKF1 = false;
                            if (valMRKF1 == 1) boolMRKF1  = true;
                            if (dicMRKF1MRKCTLMST[fcdIdx][ptry0Idx].ContainsKey(data.FLTID) == true)
                                dicMRKF1MRKCTLMST[fcdIdx][ptry0Idx][data.FLTID] = boolMRKF1 ;
                            else
                                dicMRKF1MRKCTLMST[fcdIdx][ptry0Idx].Add(data.FLTID, boolMRKF1 );

                            dataCnt++;

                            logData = string.Format($"{dataCnt}\t_\t{data.ToString()}");
                            Log.WriteLoadData(logData, dataCnt, "MRKCTLMST-BUFFER", 0.0);
                        }
                    }
                    DB_Progress.Complete((eNittoDBProgress)((int)eNittoDBProgress.MRKCTLMST_ES + fcdIdx));
                }
                return true;
            }
            catch(Exception ex)
            {
                DB_Progress.SetError((eNittoDBProgress)((int)eNittoDBProgress.MRKCTLMST_ES + procStep));
                Log.WriteLog($"[Error] MRKCTLMST_Buffer_{((eFCD)procStep).ToString()} error message : [{ex.Message}]");
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
                DestConfigUnit destUnit = new DestConfigUnit();
                destConfig.GetData(dbOption.FWPlace, ref destUnit);

                long dbCnt = 0;
                int count = System.Enum.GetValues(typeof(eFCD)).Length;
                string logData = "";

                for (int i = 0; i < count; i++)
                {
                    procStep = i;
                    int PTRY0Pcnt = PTRY0P_Data[i].Count;
                    DB_Progress.Reset((eNittoDBProgress)((int)eNittoDBProgress.MRKCTLMST_ES + i));
                    checkDicMRKCTLMST(PTRY0Pcnt, i);
                    DB_Progress.Set((eNittoDBProgress)((int)eNittoDBProgress.MRKCTLMST_ES + i));

                    for (int j = 0; j < PTRY0Pcnt; j++)
                    {
                        dicSizeMRKCTLMST[i][j].Clear();
                        dicMRKF1MRKCTLMST[i][j].Clear();

                        if ((dbOption.checkES == true && (eFCD)i == eFCD.ES) ||
                           (dbOption.checkTG == true && (eFCD)i == eFCD.TG) ||
                           (dbOption.checkETC == true && (eFCD)i == eFCD.ETC) && PTRY0P_Data[i][j].Y0KLOT.Length > 0)
                        {
                            QueryMsg.MRKCTLMST_Query msg = new QueryMsg.MRKCTLMST_Query();
                            msg.MKCD = dbOption.searchOP.MKCD;
                            msg.Y0KLOT = PTRY0P_Data[i][j].Y0KLOT;
                            string query = msg.GetQuery((eFCD)i);
                            Log.WriteLoadData(query, 0, "MRKCTLMST", 0.0);

                            if (query == "")
                            {
                                Log.WriteLog($"[Error] MRKCTLMST_{((eFCD)i).ToString()} Query message is empty.");
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
                                        MRKCTLMST_Data.Add(data);

                                        logData = string.Format($"{MRKCTLMST_Data.Count}\t-\t{data.ToString()}");
                                        Log.WriteLoadData(logData, MRKCTLMST_Data.Count, "MRKCTLMST", 0.0);
                                        // 조건문 추가해야 함
                                        CrtParam.MRKCTLMSTFLTID.Add(data.FLTID);
                                        if (dicSizeMRKCTLMST[i][j].ContainsKey(data.FLTID) == true)
                                            dicSizeMRKCTLMST[i][j][data.FLTID] = data.SIZE;
                                        else
                                            dicSizeMRKCTLMST[i][j].Add(data.FLTID, data.SIZE);

                                        int nVal = Int32.Parse(data.MRKF1);
                                        bool boolVal = false;
                                        if (nVal == 1) boolVal = true;
                                        if (dicMRKF1MRKCTLMST[i][j].ContainsKey(data.FLTID) == true)
                                            dicMRKF1MRKCTLMST[i][j][data.FLTID] = boolVal;
                                        else
                                            dicMRKF1MRKCTLMST[i][j].Add(data.FLTID, boolVal);

                                        for (int checkCnt = 0; checkCnt < destUnit.FLTIDCheck.Length; checkCnt++)
                                        {
                                            if (destUnit.FLTIDCheck[checkCnt].Length > 0 && destUnit.FLTIDCheck[checkCnt] == data.FLTID)
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
                DB_Progress.SetError((eNittoDBProgress)((int)eNittoDBProgress.MRKCTLMST_ES+ procStep));
                Log.WriteLog($"[Error] MRKCTLMST_{((eFCD)procStep).ToString()} error message : [{ex.Message}]");
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
                LoadedBcNo.Clear();
                ProductEndTime.Clear();
                ProductLotName.Clear();

                long dbCnt = 0;
                int dataCnt = 0;

                eCSV_TYPE csvType = destConfig.GetCsvType();

                int count = System.Enum.GetValues(typeof(eFCD)).Length;

                for (int idx = 0; idx < count; idx++)
                {
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

                    int PTRY0Pcnt = PTRY0P_Data[idx].Count;

                    for (int i = 0; i < PTRY0Pcnt; i++)
                    {
                        string query = "";
                        QueryMsg.INSPDATA_Query msg = new QueryMsg.INSPDATA_Query(lotID);
                        msg.LNCD = PTRY0P_Data[idx][i].LNCD;
                        msg.SetTime(PTRY0P_Data[idx][i].StartTime, QueryMsg.INSPDATA_Query.eTargetTime.TimeStart);
                        msg.SetTime(PTRY0P_Data[idx][i].EndTime, QueryMsg.INSPDATA_Query.eTargetTime.TimeEnd);

                        if (PTRY0P_Data[idx][i].Y0KLOT.Substring(0, 2).ToUpper() == "LL")
                        {
                            ProductEndTime.Add(msg.EndTime);
                            ProductLotName.Add(PTRY0P_Data[idx][i].Y0KLOT);
                        }

                        if (dbOption.useESTime == true &&
                            ((dbOption.checkES && idx == (int)eFCD.ES) || (dbOption.checkETC && idx == (int)eFCD.ETC)))
                        {
                            query = msg.GetQuery(0, dbOption);
                        }
                        else
                        {
                            query = msg.GetQuery(1, dbOption);
                        }
                        Log.WriteLoadData(query, 0, "INSPDAT", 0.0);

                        if (query == "")
                        {
                            Log.WriteLog($"[Error] INSPDAT{((eFCD)i).ToString()} Query message is empty.");
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
                                    data.Parse(reader);

                                    if (dbOption.useOffsetX)
                                    {
                                        int offsetDataCnt = XOFSMST_Data.Count;

                                        for (int offsetIdx = 0; offsetIdx < offsetDataCnt; offsetIdx++)
                                        {
                                            int ppcd = XOFSMST_Data[i].PPCD;
                                            if (data.KYCD == XOFSMST_Data[i].KYCD &&
                                                ((idx == (int)eFCD.ES && ppcd == 100) ||
                                                (idx == (int)eFCD.TG && ppcd == 400) ||
                                                (idx == (int)eFCD.ETC && ppcd != 100 && ppcd != 400)) &&
                                                PTRY0P_Data[idx][i].Y0ZKNM == XOFSMST_Data[i].YLSZKN &&
                                                PTRY0P_Data[idx][i].LNCD == XOFSMST_Data[i].LNCD)
                                            {
                                                data.OffsetX = XOFSMST_Data[i].X_OFFSET;
                                            }
                                        }
                                    }

                                    int bcnoCnt = LoadedBcNo.Count;
                                    isBcnoFind = false;
                                    if (bcnoCnt < 10) // 9개가 넘어가면 BCD Wrong Error
                                    {
                                        for (int bcnoIdx = 0; bcnoIdx < bcnoCnt; bcnoIdx++)
                                        {
                                            if (LoadedBcNo[bcnoIdx] == data.BCNO)
                                            {
                                                isBcnoFind = true;
                                                break;
                                            }
                                        }

                                        if (bcnoCnt == 0)
                                        {
                                            LoadedBcNo.Add(data.BCNO);
                                            LoadedBcNo.Add(data.LOTNO);
                                        }
                                        else
                                        {
                                            if (isBcnoFind == false) LoadedBcNo.Add(data.BCNO);
                                        }
                                    }

                                    // 리스트에 데이터 추가함
                                    inspDataList.Add(data);
                                    dataCnt++;
                                    Log.WriteLoadData(data.ToString(), dataCnt, "INSPDAT", 0.0);
                                }
                            }
                        }
                        // 최종 데이터 입력
                        INSPDAT_Data[idx].Add(inspDataList);
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
                Log.WriteLog($"[Error] INSPDAT_{((eFCD)procStep).ToString()} error message : [{ex.Message}]");
                return false;
            }
        }

        public bool SearchFLTDAT(string lotID)
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;

            faultData = resultDefect.Data;
            markFaultData = resultDefect.MarkFault.Data;

            DestConfigUnit destUnit = new DestConfigUnit();
            destConfig.GetData(dbOption.FWPlace, ref destUnit);

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
            bool bValid;
            float faultSize;
            float finalXPos;
            string tmpFaltID;

            bool validMark;

            INSPDATData inspdata;

            long dbCnt = 0;
            int fcdCnt = System.Enum.GetValues(typeof(eFCD)).Length;
            int dataCnt = 0;
            string logData = "";
            string query;
            int procStep = 0;

            try
            {
                for (int fcdIdx = 0; fcdIdx < fcdCnt; fcdIdx++)
                {
                    procStep = fcdIdx;
                    DB_Progress.Reset((eNittoDBProgress)((int)eNittoDBProgress.FAULTDAT_ES + fcdIdx));

                    if (dbOption.checkES == true && fcdIdx == (int)eFCD.ES)
                    {
                    }
                    else if (dbOption.checkTG == true && fcdIdx == (int)eFCD.TG)
                    {
                    }
                    else if (dbOption.checkETC == true && fcdIdx == (int)eFCD.ETC)
                    {
                    }
                    else
                    {
                        if (dbOption.checkES == false && fcdIdx == (int)eFCD.ES) DB_Progress.SetSkip(eNittoDBProgress.FAULTDAT_ES);
                        if (dbOption.checkTG == false && fcdIdx == (int)eFCD.TG) DB_Progress.SetSkip(eNittoDBProgress.FAULTDAT_TG);
                        if (dbOption.checkETC == false && fcdIdx == (int)eFCD.ETC) DB_Progress.SetSkip(eNittoDBProgress.FAULTDAT_ETC);

                        continue;
                    }

                    int nItemCnt = 0;
                    for (int iIdx = 0; iIdx < INSPDAT_Data[fcdIdx].Count; iIdx++)
                        nItemCnt += INSPDAT_Data[fcdIdx][iIdx].Count;

                    DB_Progress.Set((eNittoDBProgress)((int)eNittoDBProgress.FAULTDAT_ES + fcdIdx));
                    nItemCnt = 0;
                    for (int opIdx = 0; opIdx < PTRY0P_Data[fcdIdx].Count; opIdx++)
                    {
                        dicSizeData.Clear();
                        dicMRKF1Data.Clear();

                        foreach (KeyValuePair<string, float> pair in dicSizeMRKCTLMST[fcdIdx][opIdx])
                            dicSizeData.Add(pair.Key, pair.Value);

                        foreach (KeyValuePair<string, bool> pair in dicMRKF1MRKCTLMST[fcdIdx][opIdx])
                            dicMRKF1Data.Add(pair.Key, pair.Value);

                        int inspCnt = INSPDAT_Data[fcdIdx][opIdx].Count;
                        for (int inspIdx = 0; inspIdx < inspCnt; inspIdx++)
                        {
                            inspdata = INSPDAT_Data[fcdIdx][opIdx][inspIdx];
#if (FAST_FLTID)
                            QueryMsg.FLTDAT_FAST_Query fastMsg = new QueryMsg.FLTDAT_FAST_Query();
                            fastMsg.BCNO = inspdata.BCNO;
                            query = fastMsg.GetQuery();
#else
                            QueryMsg.FLTDAT_Query msg = new QueryMsg.FLTDAT_Query();
                            msg.CTLNO = inspdata.CTLNO;
                            query = msg.GetQuery();
#endif
                            Log.WriteLoadData(query, 0, "FAULTDAT", 0.0);

                            if (query == "")
                            {
                                Log.WriteLog($"[Error] FAULTDAT_{((eFCD)fcdIdx).ToString()} Query message is empty.");
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
                                        bValid = false;
                                        if (useAIFromDB == false)
                                        {
                                            tmpKey = data.MNTTAN.TrimStart();
                                            //mnttid = mnttid.TrimStart('\s');
                                            if (string.IsNullOrEmpty(tmpKey))
                                                tmpKey = data.FLTID;
                                        }
                                        else
                                            tmpKey = data.FLTID;

                                        if (dicSizeData.ContainsKey(tmpKey) == true && dicMRKF1Data.ContainsKey(tmpKey)==true)
                                        {
                                            validMark = dicMRKF1Data[data.FLTID];
                                            faultSize = dicSizeData[data.FLTID];
                                            if (faultSize <= (data.AREA_M + 0.00001f) && validMark==true) bValid = true;
                                            else bValid = false;
                                        }

                                        if (bValid == true)    // 소수점 오차 보정
                                        {
                                            if (finalXPos < 0.0f) continue;
                                            if (useMask == true && isMaskedDefect(finalXPos, data.OFFSET) == true) continue;
                                            if (useSplit == true && isSplitSkipDefect(finalXPos, splitStartX, splitEndX) == true) continue;

                                            // Fault Data 처리
                                            FaultDatum tmpFltData = new FaultDatum();

                                            tmpFltData.FLTNO = data.FLTNO;
                                            tmpFltData.OFFSET = data.OFFSET;
                                            tmpFltData.YPOS_M = data.YPOS_M;
                                            tmpFltData.XPOS_M = data.XPOS_M;

                                            // 코드 불량 카운트 증가
                                            if (inspdata.CTLNO == data.CTLNO)
                                                inspdata.RollCtlCnt++;

                                            // FLTID비교기능
                                            for (int checkCnt = 0; checkCnt < destUnit.FLTIDCheck.Length; checkCnt++)
                                            {
                                                if (destUnit.FLTIDCheck[checkCnt].Length > 0 && destUnit.FLTIDCheck[checkCnt] == data.FLTID)
                                                    CrtParam.FAULTDATFLTID.Add(data.FLTID);
                                            }

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
                                            else markData.XOFFSET_ALARM = float.MaxValue;

                                            if (csvType == eCSV_TYPE.NITTO)
                                            {
                                                if (fcdIdx == (int)eFCD.TG) markData.DefectLine = 9; // 점착
                                                else markData.DefectLine = 8; // 그외
                                            }
                                            else if (csvType == eCSV_TYPE.NITTO_RTS || csvType == eCSV_TYPE.NITTO_RK || csvType == eCSV_TYPE.KORENO_RK_IJP)
                                            {
                                                if (fcdIdx == (int)eFCD.TG) markData.DefectLine = 9; //점착 
                                                else if (fcdIdx == (int)eFCD.ES) markData.DefectLine = 8; // 연신 - 기타
                                                else markData.DefectLine = 7; // 그외
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
                                            {
                                                markData.DefectLine = defectLine;
                                            }

                                            //RK는 CAMNO별로 Defect Class 를 구분
                                            if (csvType == eCSV_TYPE.NITTO_RK || csvType == eCSV_TYPE.NITTO_RTS || csvType == eCSV_TYPE.KORENO_RK_IJP)
                                            {
                                                markData.DefectLine += Global.MaxDefectLine * data.CAMNO;
                                            }

                                            resultDefect.Data.Add(tmpFltData);
                                            resultDefect.MarkFault.Data.Add(markData);
                                            dataCnt++;
                                            logData = data.GetString(dataCnt, markData.DefectLine, markData.BCNO, markData.XOFFSET);
                                            Log.WriteLoadData(logData, dataCnt, "FAULTDAT", 0.0);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    DB_Progress.Set((eNittoDBProgress)((int)eNittoDBProgress.FAULTDAT_ES + fcdIdx));
                }

                //   Defect 사이즈 처리
                resultDefect.MarkFault.MinXPos = minXPos;
                resultDefect.MarkFault.MaxXPos = maxXPos;
                resultDefect.MarkFault.MinSize = minSize;

                return true;
            }
            catch (Exception ex)
            {
                if (procStep == (int)eFCD.ES) DB_Progress.SetError(eNittoDBProgress.INSPDAT_ES);
                else if (procStep == (int)eFCD.TG) DB_Progress.SetError(eNittoDBProgress.INSPDAT_TG);
                else if (procStep == (int)eFCD.ETC) DB_Progress.SetError(eNittoDBProgress.INSPDAT_ETC);
                Log.WriteLog($"[Error] FAULTDAT_{((eFCD)procStep).ToString()} error message : [{ex.Message}]");
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
                        Log.WriteLoadData(errData, 0, "XOFSMST", 0.0);
                        break;
                    }
                    count++;
                }
            }

            return bXOfSErr;
        }

        private bool isMaskedDefect(float xPos, float yPos)
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

        public bool IsProductAvaliable(int nAvaliableSpan)
        {
            DateTime dt = DateTime.Now;
            Debug.Assert(ProductEndTime.Count == ProductLotName.Count);
            string strE, strC, str;
            strC = dt.ToString("yyyyMMddHHmmdd");

            int count = 0;

            TimeSpan refTs = new TimeSpan(0, nAvaliableSpan, 0, 0);
            foreach (DateTime time in ProductEndTime)
            {
                TimeSpan sp = dt - time;
                strE = time.ToString("yyyyMMddHHmmdd");
                str = string.Format($"{dbOption.dbWhen.ToString()}, {ProductLotName[count]}, 현재시각:{strC}, " +
                    $"생산종료시각:{strE}, 설정시간:{nAvaliableSpan}, 차이시간:{sp.Hours}");
                Log.WriteLoadData(str, 0, "PRODUCTION_ABLE", 0);
                count++;

                if (sp < refTs)
                {
                    Log.WriteLoadData("생산가능시간 NG", 0, "PRODUCTION_ABLE", 0);
                    return false;
                }
            }
            return true;
        }

        #region Control CSV 
        public void OpenCsvFile(string path)
        {
            switch (destConfig.CSVType)
            {
                case eCSV_TYPE.NITTO:
                    openCsvKoh(path);
                    break;

                case eCSV_TYPE.NITTO_RTS:
                case eCSV_TYPE.NITTO_RK:
                    openCsvNitto(path);
                    break;

                case eCSV_TYPE.KORENO:
                case eCSV_TYPE.KORENO_RK:
                case eCSV_TYPE.KORENO_RK_IJP:
                    openCsvKor(path);
                    break;
            }
        }

        private void openCsvKoh(string path)
        {
            string text;
            bool useMask = dbOption.searchOP.useMask;
            // 우선 fault Data 초기화.. 
            // 나중에 CrtParam._FaultData 내에 다른 데이터 초기화 해야하는지 확인이 필요함.
            // CrtParam._FaultData는 인덱스가 0으로 바뀌는데 CrtParam._MarkFaultData는 초기화 안 함
            // 나중에 확인 필요
            resultDefect.Data.Clear();

            _RollDefectInfo = new RollDefectInfo();
            string tmpBCInfo = null;
            _RollDefectInfo.InitData("");

            using (var file = new StreamReader(path, Encoding.Default))
            {
                file.ReadLine(); // Title 

                while ((text = file.ReadLine()) != null)
                {
                    text = text.Replace("\"", "");
                    string[] items = text.Split(',');
                    FaultDatum tmpData = new FaultDatum();


                    // 데이터 인덱스 수정 필요함
                    tmpData.FLTNO = items[0].Trim();
                    tmpData.OFFSET = float.Parse(items[14]);
                    tmpData.XPOS_M = float.Parse(items[7]);
                    tmpData.YPOS_M = float.Parse(items[8]);
                    tmpData.SIZE_AREA = float.Parse(items[11]);
                    tmpData.MACNO = items[2].Trim();
                    tmpData.FLTID = items[5];
                    tmpData.CAM_NO = Int32.Parse(items[4]);

                    tmpBCInfo = items[12].Substring(0, items[12].IndexOf('_'));

                    resultDefect.Data.Add(tmpData);
                    if (useMask == true && isMaskedDefect(tmpData.XPOS_M, tmpData.YPOS_M) == true)
                        continue;

                    MarkingFaultDatum tmpMarkData = new MarkingFaultDatum();
                    tmpMarkData.FLTNO = tmpData.FLTNO;
                    tmpMarkData.OFFSET = tmpData.OFFSET;
                    tmpMarkData.YPOS_M = tmpData.YPOS_M;
                    tmpMarkData.XPOS_M = tmpData.XPOS_M;
                    tmpMarkData.SIZE = tmpData.SIZE_AREA;
                    tmpMarkData.FAULTID = tmpData.FLTID;
                    tmpMarkData.XOFFSET = 0.0f;
                    tmpMarkData.UseCSVResult = true;
                    tmpMarkData.CTLNO = "CSV";
                    tmpMarkData.MACNO = tmpData.MACNO;
                    tmpMarkData.CAM_NO = tmpData.CAM_NO;

                    if (dbOption.checkES == true && dbOption.checkTG == false && dbOption.checkETC == false) // 연신
                        tmpMarkData.DefectLine = 8;
                    else if (dbOption.checkES == false && dbOption.checkTG == true && dbOption.checkETC == false) // 점착    
                        tmpMarkData.DefectLine = 9;
                    else
                        tmpMarkData.DefectLine = 7;

                    resultDefect.MarkFault.Data.Add(tmpMarkData);
                    _RollDefectInfo.AddYPos(tmpMarkData.YPOS_M);

                }

                if (tmpBCInfo != null)
                    _RollDefectInfo.InitData(tmpBCInfo, false);
            }
        }

        private void openCsvKor(string path)
        {
            string text;
            bool useMask = dbOption.searchOP.useMask;
            // 우선 fault Data 초기화.. 
            // 나중에 CrtParam._FaultData 내에 다른 데이터 초기화 해야하는지 확인이 필요함.
            // CrtParam._FaultData는 인덱스가 0으로 바뀌는데 CrtParam._MarkFaultData는 초기화 안 함
            // 나중에 확인 필요
            resultDefect.Data.Clear();

            int Optic1Cnt = 0;
            int Optic1ScCount = 0;
            int Optic2Cnt = 0;
            int Optic3Cnt = 0;
            int Optic4DotCnt = 0;
            int Optic4LineCnt = 0;
            int OpticFreqCnt = 0;
            int OpticSameCnt = 0;

            int nThru, nDiff, nCross, nThru1, nThru2, nFreq, nSame;
            nDiff = nThru = nCross = nThru1 = nThru2 = nFreq = nSame = 0;

            string tmpDefect;
            int tmpDefectId = 0, defectKind = 0;

            bool bCSVOption = false;

            float tmpLeng1, tmpLeng2;

            bool isEsCSV = false;

            RollDefectInfo_Kor tmpRollInfo = new RollDefectInfo_Kor();

            using (var file = new StreamReader(path, Encoding.Default))
            {
                if (file == null) return;

                CSV_DEFECT_HEADER csvInspData = new CSV_DEFECT_HEADER();

                file.ReadLine(); // Title 
                text = file.ReadLine();
                text = text.Replace("\"", "");
                string[] items = text.Split(',');

                // INSPDATA 표시용 데이터 처리
                csvInspData.bcrInfo = items[2].Trim(' ');
                csvInspData.lotNo = csvInspData.bcrInfo.Substring(0, 7);
                csvInspData.rollNo = csvInspData.bcrInfo.Substring(csvInspData.bcrInfo.Length - 2, 2);
                csvInspData.startY = items[9];
                csvInspData.endY = items[10];
                csvInspData.rollY = items[13].Trim(' ');
                csvInspData.rollSY = items[14].Trim(' ');
                csvInspData.rollEY = items[15].Trim(' ');
                csvInspData.jig = items[3];
                csvInspData.machine = items[8];
                _CsvDefectHeader = csvInspData;

                tmpRollInfo.InitData(csvInspData.bcrInfo);

                file.ReadLine();
                file.ReadLine();

                while ((text = file.ReadLine()) != null)
                {
                    text = text.Replace("\"", " ");
                    items = text.Split(',');
                    FaultDatum tmpData = new FaultDatum();
                    MarkingFaultDatum tmpMarkData = new MarkingFaultDatum();

                    tmpDefect = items[1].Trim(' ');
                    if (Int32.TryParse(tmpDefect, out int intVal) == true) tmpDefectId = intVal;
                    else tmpDefectId = 0;

                    if (Int32.TryParse(items[4], out intVal) == true) defectKind = intVal;
                    else defectKind = 0;

                    if (float.TryParse(items[7], out float floatVal) == true) tmpData.XPOS_M = floatVal;
                    else tmpData.XPOS_M = 0;

                    if (float.TryParse(items[8], out floatVal) == true) tmpData.YPOS_M = floatVal;
                    else tmpData.YPOS_M = 0;

                    if (float.TryParse(items[9], out floatVal) == true) tmpData.SIZE_AREA = floatVal;
                    else tmpData.SIZE_AREA = 0;

                    if (float.TryParse(items[10], out floatVal) == true) tmpData.SIZE_X = floatVal;
                    else tmpData.SIZE_X = 0;

                    if (float.TryParse(items[11], out floatVal) == true) tmpData.SIZE_Y = floatVal;
                    else tmpData.SIZE_Y = 0;

                    if (float.TryParse(items[14], out floatVal) == true) tmpData.OFFSET = floatVal;
                    else tmpData.OFFSET = 0;

                    if (tmpDefectId < 256) // 점착은 Class가 256 미만
                    {
                        bCSVOption = false;
                        switch (defectKind)
                        {
                            case 1: if (dbOption.useCSVCross == true) bCSVOption = true; break;
                            case 2: if (dbOption.useCSVLimit == true) bCSVOption = true; break;
                            case 3: if (dbOption.useCSVSlit == true) bCSVOption = true; break;
                            case 4: if (dbOption.useCSVStraight == true) bCSVOption = true; break;
                            case 5: if (dbOption.useCSVSVJH == true) bCSVOption = true; break;
                            case 6: if (dbOption.useCSVSVMH == true) bCSVOption = true; break;
                        }

                        nDiff = nThru = nCross = nThru1 = nThru2 = nFreq = nSame = 0;

                        if (dbOption.useCSVAll == true || bCSVOption == true) // All or Check
                        {
                            if (tmpDefectId == 5)
                            {
                                if (destConfig.SkipData[(int)eOpticClass.OpticClass3].Check(tmpData.SIZE_X, tmpData.SIZE_Y, tmpData.SIZE_AREA))
                                    continue;
                                nFreq = 1;
                            }
                            else if (defectKind == 2)
                            {
                                if (destConfig.SkipData[(int)eOpticClass.OpticClass1].Check(tmpData.SIZE_X, tmpData.SIZE_Y, tmpData.SIZE_AREA))
                                    continue;
                                nDiff = 1;
                            }
                            else if (defectKind == 4)
                            {
                                tmpLeng1 = tmpData.SIZE_X / tmpData.SIZE_Y;
                                tmpLeng2 = tmpData.SIZE_Y / tmpData.SIZE_X;
                                if (tmpLeng1 > 2 || tmpLeng2 > 2) // 선
                                    nThru2 = 1;
                                else // 점
                                    nThru1 = 1;

                                if (tmpDefectId == 8)//동일 결점
                                {
                                    nSame = 1;
                                    nThru1 = nThru2 = 0;
                                }
                                else
                                {
                                    if (destConfig.SkipData[(int)eOpticClass.OpticClass2].Check(tmpData.SIZE_X, tmpData.SIZE_Y, tmpData.SIZE_AREA))
                                        continue;
                                }
                            }
                            else if (defectKind == 1)
                            {
                                if (tmpDefectId == 9)
                                    nCross = 9;
                                else
                                {
                                    nCross = 1;
                                    if (destConfig.SkipData[(int)eOpticClass.OpticClass4].Check(tmpData.SIZE_X, tmpData.SIZE_Y, tmpData.SIZE_AREA))
                                        continue;
                                }
                            }
                            else if (defectKind == 3)
                            {
                                nCross = 3;
                                if (destConfig.SkipData[(int)eOpticClass.OpticClass4].Check(tmpData.SIZE_X, tmpData.SIZE_Y, tmpData.SIZE_AREA))
                                    continue;
                            }
                            else if (defectKind == 5)
                            {
                                if (destConfig.SkipData[(int)eOpticClass.OpticClass5].Check(tmpData.SIZE_X, tmpData.SIZE_Y, tmpData.SIZE_AREA))
                                    continue;
                            }
                            else if (defectKind == 6)
                            {
                                if (destConfig.SkipData[(int)eOpticClass.OpticClass6].Check(tmpData.SIZE_X, tmpData.SIZE_Y, tmpData.SIZE_AREA))
                                    continue;
                            }
                            else
                            {
                                if (destConfig.SkipData[(int)eOpticClass.OpticClass2].Check(tmpData.SIZE_X, tmpData.SIZE_Y, tmpData.SIZE_AREA))
                                    continue;
                                nDiff = 1;
                            }

                            if (useMask && isMaskedDefect(tmpData.XPOS_M, tmpData.OFFSET) == true)
                                continue;

                            // thru 4 diff 2
                            if (nSame == 1)
                            {
                                if (dbOption.useSameDefect)
                                {
                                    tmpMarkData.DefectLine = 19; //CSV-정투과(동일결점)
                                    OpticSameCnt++;
                                }
                                else continue;
                            }
                            else if (nThru1 == 1)
                            {
                                tmpMarkData.DefectLine = 4;	//CSV-정투과(점)
                                Optic4DotCnt++;
                            }
                            else if (nThru2 == 1)
                            {
                                tmpMarkData.DefectLine = 5;	//CSV-정투과(실)
                                Optic4LineCnt++;
                            }
                            else if (nDiff == 1)
                            {
                                tmpMarkData.DefectLine = 2;	//CSV-경계
                                Optic2Cnt++;
                            }
                            else if (nCross == 1)
                            {
                                tmpMarkData.DefectLine = 1;	//CSV-크로스
                                Optic1Cnt++;
                            }
                            else if (nCross == 3)
                            {
                                tmpMarkData.DefectLine = 3;	//CSV-크로스 휘점
                                Optic1Cnt++;
                            }
                            else if (nCross == 9)
                            {
                                tmpMarkData.DefectLine = 10;	//CSV-크로스 스크래치
                                Optic1ScCount++;
                            }
                            else if (nFreq == 1)
                            {
                                tmpMarkData.DefectLine = 7;	//CSV-주기성 불량
                                OpticFreqCnt++;
                            }
                            else if (tmpDefectId >= 10 && tmpDefectId <= 16)
                            {
                                tmpMarkData.DefectLine = (int)eFLTID.JH_DOT_W_31 + (tmpDefectId - 10);
                                CrtParam.CSVFalutCount[(int)eFLTID.JH_DOT_W_31 + (tmpDefectId - 10)]++;
                            }
                            else if (tmpDefectId >= 17 && tmpDefectId <= 23)
                            {
                                tmpMarkData.DefectLine = (int)eFLTID.MH_DOT_W_51 + (tmpDefectId - 17);
                                CrtParam.CSVFalutCount[(int)eFLTID.MH_DOT_W_51 + (tmpDefectId - 17)]++;
                            }
                            else if (tmpDefectId >= 24 && tmpDefectId <= 26)
                            {
                                tmpMarkData.DefectLine = (int)eFLTID.SAME2_81 + (tmpDefectId - 24);
                                CrtParam.CSVFalutCount[(int)eFLTID.SAME2_81 + (tmpDefectId - 24)]++;
                            }
                            else if (tmpDefectId >= 28 && tmpDefectId <= 32)
                            {
                                tmpMarkData.DefectLine = (int)eFLTID.NEL7_HJK_5_61 + (tmpDefectId - 28);
                                CrtParam.CSVFalutCount[(int)eFLTID.NEL7_HJK_5_61 + (tmpDefectId - 28)]++;
                            }
                            else if (tmpDefectId >= 33 && tmpDefectId <= 35)
                            {
                                tmpMarkData.DefectLine = (int)eFLTID.NEL8_W_71 + (tmpDefectId - 33);
                                CrtParam.CSVFalutCount[(int)eFLTID.NEL8_W_71 + (tmpDefectId - 33)]++;
                            }
                            else if (tmpDefectId >= 36 && tmpDefectId <= 39)
                            {
                                tmpMarkData.DefectLine = (int)eFLTID.SAME5_84 + (tmpDefectId - 36);
                                CrtParam.CSVFalutCount[(int)eFLTID.SAME8_87 + (tmpDefectId - 36)]++;
                            }
                            else
                            {
                                tmpMarkData.DefectLine = 0;
                                Optic3Cnt++;
                            }

                        }
                    }
                    else
                    {
                        if (useMask == true && isMaskedDefect(tmpData.XPOS_M, tmpData.OFFSET) == true)
                            continue;

                        if (tmpDefectId == 2305 || tmpDefectId == 2321) CrtParam.ESClassDefectCnt[(int)eESDefectClass.CROSS1]++;
                        else if (tmpDefectId == 2306 || tmpDefectId == 2322) CrtParam.ESClassDefectCnt[(int)eESDefectClass.CROSS2]++;
                        else if (tmpDefectId == 2307 || tmpDefectId == 2323) CrtParam.ESClassDefectCnt[(int)eESDefectClass.CROSS3]++;
                        else if (tmpDefectId == 2308 || tmpDefectId == 2324) CrtParam.ESClassDefectCnt[(int)eESDefectClass.CROSS4]++;
                        else if (tmpDefectId == 2310 || tmpDefectId == 2326) CrtParam.ESClassDefectCnt[(int)eESDefectClass.CROSS5]++;
                        else if (tmpDefectId == 2311 || tmpDefectId == 2327) CrtParam.ESClassDefectCnt[(int)eESDefectClass.CROSS6]++;
                        else if (tmpDefectId == 2309 || tmpDefectId == 2325) CrtParam.ESClassDefectCnt[(int)eESDefectClass.CROSS7]++;
                        else if (tmpDefectId == 1025 || tmpDefectId == 1041) CrtParam.ESClassDefectCnt[(int)eESDefectClass.THRU1]++;
                        else if (tmpDefectId == 1026 || tmpDefectId == 1042) CrtParam.ESClassDefectCnt[(int)eESDefectClass.THRU2]++;
                        else if (tmpDefectId == 1027 || tmpDefectId == 1043) CrtParam.ESClassDefectCnt[(int)eESDefectClass.THRU3]++;
                        else if (tmpDefectId == 1030 || tmpDefectId == 1046) CrtParam.ESClassDefectCnt[(int)eESDefectClass.THRU4]++;
                        else if (tmpDefectId >= 1537 && tmpDefectId < 1567) CrtParam.ESClassDefectCnt[(int)eESDefectClass.REF1]++;
                        else if (tmpDefectId == 513) CrtParam.ESClassDefectCnt[(int)eESDefectClass.FREQ1]++;
                        else if (tmpDefectId == 514) CrtParam.ESClassDefectCnt[(int)eESDefectClass.FREQ2]++;
                        else if (tmpDefectId == 515) CrtParam.ESClassDefectCnt[(int)eESDefectClass.FREQ3]++;
                        else if (tmpDefectId == 516) CrtParam.ESClassDefectCnt[(int)eESDefectClass.FREQ4]++;
                        else if (tmpDefectId == 517) CrtParam.ESClassDefectCnt[(int)eESDefectClass.FREQ5]++;
                        else if (tmpDefectId == 518) CrtParam.ESClassDefectCnt[(int)eESDefectClass.FREQ6]++;
                        else if (tmpDefectId == 593) CrtParam.ESClassDefectCnt[(int)eESDefectClass.SAME1]++;
                        else if (tmpDefectId == 594) CrtParam.ESClassDefectCnt[(int)eESDefectClass.SAME2]++;
                        else if (tmpDefectId == 595) CrtParam.ESClassDefectCnt[(int)eESDefectClass.SAME3]++;
                        else if (tmpDefectId == 596) CrtParam.ESClassDefectCnt[(int)eESDefectClass.SAME4]++;
                        else if (tmpDefectId == 597) CrtParam.ESClassDefectCnt[(int)eESDefectClass.SAME5]++;
                        else CrtParam.ESClassDefectCnt[(int)eESDefectClass.ETC]++;

                        isEsCSV = true;
                        tmpMarkData.DefectLine = 0;
                    }

                    tmpData.MACNO = csvInspData.jig;

                    tmpMarkData.OFFSET = tmpData.OFFSET;
                    tmpMarkData.YPOS_M = tmpData.YPOS_M;
                    tmpMarkData.XPOS_M = tmpData.XPOS_M;
                    tmpMarkData.FAULTID = tmpDefectId.ToString();
                    tmpMarkData.MACNO = tmpData.MACNO;
                    tmpMarkData.UseCSVResult = true;
                    tmpMarkData.CTLNO = "CSV";
                    tmpMarkData.XOFFSET = 0.0f;
                    tmpMarkData.SIZE = tmpData.SIZE_AREA;
                    tmpMarkData.SIZE_X = tmpData.SIZE_X;
                    tmpMarkData.SIZE_Y = tmpData.SIZE_Y;

                    resultDefect.MarkFault.Data.Add(tmpMarkData);
                    resultDefect.Data.Add(tmpData);

                    _RollDefectInfo.AddYPos(tmpData.YPOS_M);
                }

                float defect2M = 0;
                float fEnd = 0.0f, fRoll = 0.0f;
                bool isParse = true;
                if (float.TryParse(csvInspData.endY, out float tmpVal) == true) fEnd = tmpVal;
                else isParse = false;
                if (float.TryParse(csvInspData.rollY, out tmpVal) == true) fRoll = tmpVal;
                else isParse = false;

                if (isParse == true && fEnd > 0.0 && fRoll > 0.0)
                    defect2M = (float)_RollDefectInfo.BadCnt / ((fEnd * fRoll) / 1000.0f / 1000.0f);
                tmpRollInfo.SetDefectPerM(defect2M);

                _RollDefectInfo = tmpRollInfo;

                CrtParam.UseKT = 1;
            }

            int ktTotal = 0;

            if (isEsCSV == true)
            {
                for (int i = 0; i < CrtParam.ESClassDefectCnt.Length; i++)
                    ktTotal += CrtParam.ESClassDefectCnt[i];
            }
            else
            {
                CrtParam.Optic1Cnt = Optic1Cnt;
                CrtParam.Optic1ScCount = Optic1ScCount;
                CrtParam.Optic2Cnt = Optic2Cnt;
                CrtParam.Optic3Cnt = Optic3Cnt;
                CrtParam.Optic4DotCnt = Optic4DotCnt;
                CrtParam.Optic4LineCnt = Optic4LineCnt;
                CrtParam.OpticFreqCnt = OpticFreqCnt;
                CrtParam.OpticSameCnt = OpticSameCnt;

                if (dbOption.useSameDefect == true)
                    ktTotal = Optic1Cnt + Optic2Cnt + Optic3Cnt + Optic4DotCnt + Optic4LineCnt + OpticFreqCnt + Optic1ScCount + OpticSameCnt;
                else
                    ktTotal = Optic1Cnt + Optic2Cnt + Optic3Cnt + Optic4DotCnt + Optic4LineCnt + OpticFreqCnt + Optic1ScCount;

                for (int i = 0; i < CrtParam.CSVFalutCount.Length; i++)
                    ktTotal += CrtParam.CSVFalutCount[i];
            }

            // 화면 표시는 상위 Form 단에서 처리

        }

        private void openCsvNitto(string path)
        {
            string text;
            bool useMask = dbOption.searchOP.useMask;


            // 에러 대비 초기화
            _RollDefectInfo = null;
            RollDefectInfo rollDefectInfo = new RollDefectInfo();
            bool isInit = false;

            // fault data만 초기화 함
            resultDefect.Data.Clear();
            faultData = resultDefect.Data;
            markFaultData = resultDefect.MarkFault.Data;

            int dataCnt = 0;
            string strMACNO, strFltID, strbcr, strCode, strCamNo;
            string[] items;
            bool bBcrExist = false;

            int totalLine = File.ReadAllLines(path).Length;

            using (var file = new StreamReader(path, Encoding.Default))
            {
                if (file == null)
                    return;

                text = file.ReadLine(); // Title 

                while ((text = file.ReadLine()) != null)
                {
                    string text1 = text.Replace("\"", "");
                    items = text1.Split(',');
                    FaultDatum tmpData = new FaultDatum();
                    MarkingFaultDatum tmpMarkData = new MarkingFaultDatum();

                    strMACNO = items[0];
                    strFltID = items[2];
                    strCode = items[3];
                    tmpData.YPOS_M = float.Parse(items[4]);
                    tmpData.XPOS_M = float.Parse(items[5]);
                    tmpData.SIZE_AREA = float.Parse(items[6]);
                    tmpData.OFFSET = float.Parse(items[9]);
                    strCamNo = items[10];

                    strbcr = items[16];
                    bBcrExist = false;
                    foreach (string bcr in LoadedBcNo)
                    {
                        if (bcr == strbcr)
                        {
                            bBcrExist = true;
                            break;
                        }
                    }
                    if (bBcrExist == false) LoadedBcNo.Add(strbcr);

                    if (useMask == true && isMaskedDefect(tmpData.XPOS_M, tmpData.OFFSET))
                        continue;



                    tmpData.MACNO = strMACNO;
                    tmpData.CAM_NO = Int32.Parse(strCamNo);

                    tmpMarkData.OFFSET = tmpData.OFFSET;
                    tmpMarkData.YPOS_M = tmpData.YPOS_M;
                    tmpMarkData.XPOS_M = tmpData.XPOS_M;
                    tmpMarkData.SIZE = tmpData.SIZE_AREA;
                    tmpMarkData.UseCSVResult = true;
                    tmpMarkData.CAM_NO = tmpData.CAM_NO;
                    tmpMarkData.XOFFSET = 0.0f;
                    tmpMarkData.CTLNO = "CSV";
                    tmpMarkData.FAULTID = strFltID;
                    tmpMarkData.MACNO = tmpData.MACNO;

                    int defectLine = 0;
                    if (destConfig.CSV_Ver == 1)
                    {
                        int code = Int32.Parse(strCode);
                        if (code == 100) defectLine = 8;// 연신
                        else if (code == 400) defectLine = 9;// 점착
                        else defectLine = 7;// 그 외
                        defectLine += Global.MaxDefectLine * Int32.Parse(strCamNo);

                        tmpMarkData.DefectLine = defectLine;

                        if (CrtParam._UserDefectClass.UpdateDefectLine(strFltID, ref defectLine) == true)
                            tmpMarkData.DefectLine = defectLine;
                    }
                    else
                    {
                        if (dbOption.checkES == true && dbOption.checkTG == false && dbOption.checkETC == false) defectLine = 8;// 연신
                        else if (dbOption.checkES == false && dbOption.checkTG == true && dbOption.checkETC == false) defectLine = 9;// 점착
                        else defectLine = 7;// 그 외
                    }

                    tmpMarkData.DefectLine = defectLine;

                    faultData.Add(tmpData);
                    markFaultData.Add(tmpMarkData);

                    if (isInit == false && strbcr != null)
                    {
                        rollDefectInfo.InitData(strbcr);
                        isInit = true;
                    }
                    rollDefectInfo.AddYPos((float)(tmpData.YPOS_M / 1000.0f));

                    dataCnt++;
                }
            }

            _RollDefectInfo = rollDefectInfo;
            // 데이터 후처리 추가 필요
        }
        #endregion

    }
}
