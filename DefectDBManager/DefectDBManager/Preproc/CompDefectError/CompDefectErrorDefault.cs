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
    public class CompDefectErrorDefault : ICompareDefectError
    {
        public OracleDbConnection Conn
        {
            get { return conn; }
        }
        protected OracleDbConnection conn = null;

        public DestConfig DbDestConfig
        {
            get { return destConfig; }
            set { destConfig = value; }
        }
        protected DestConfig destConfig;

        public CodeConfig DBCodeConfig
        {
            get { return codeConfig; }
            set { codeConfig = value; }
        }
        protected CodeConfig codeConfig;

        public Option DbOption
        {
            get { return dbOption; }
            set { dbOption = value; }
        }
        protected Option dbOption;

        public PTRY0PList PTRY0PList_Data { get; protected set; }

        public DBLotInfo.LotData _DbResult { get; set; }

        public PreProcResultData FaultData { get; set; }

        public IRollDefectInfo _RollDefectInfo { get; set; }
        public CSV_DEFECT_HEADER _CsvDefectHeader { get; protected set; }
        public List<CSVLoadInfo> _CSVLoadInfo { get; protected set; }

        public string SearchLotName { get; set; }

        #region Filter Param
        public string _SearchY0LNCD { get; protected set; } = string.Empty;
        public string _ProductName { get; protected set; } = string.Empty;

        protected bool _isWildCard = false;

        public string _SubPath { get; set; }
        #endregion

        public LogDB _LOG { get; protected set; }

        #region Lot Search Param
        public string LotID { get; protected set; } = string.Empty;
        public bool IsMsgOut { get; protected set; } = false;
        #endregion

        // 상위 객체
        protected object owner;
        protected bool _doDiscon = true;

        public CompDefectErrorDefault(object parent, OracleDbConnection dbconn, bool disconnDB = true)
        {
            owner = parent;
            conn = dbconn;
            _DbResult = new DBLotInfo.LotData();
            _CSVLoadInfo = new List<CSVLoadInfo>();
            _LOG = new LogDB();

            PTRY0PList_Data = new PTRY0PList();

            _doDiscon = disconnDB;
        }

        public void ResetDataAll()
        {
            _DbResult.ClearAll();
            _DbResult.ResetData_DE();
            _CSVLoadInfo.Clear();
            FaultData?.ResetAll();
        }

        protected int GetLotSpliceCnt(string lotID)
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
                    if (j == 0) strLowPath = Path.Combine(_LOG.GetBcrPath(), _SubPath, lotID);
                    else strLowPath = Path.Combine(_LOG.GetBcrPath(), _SubPath, $"{lotID}_{j:D2}");

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

        public virtual IPreprocLot SearchLot(ref eSearchError errOut)
        {
            return null;
        }

        public virtual IPreprocLot SearchLot_TEST(ref eSearchError errOut)
        {
            return null;
        }

        public virtual bool SearchPTRYOPList()
        {
            return false;
        }

        public virtual bool SearchPTRYOPList_TEST()
        {
            return false;
        }

        protected void InspDatToFCDArray()
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

        protected bool SearchXOFSMST()
        {
            try
            {
                string lotID = LotID;
                // 연결 횟수 확인 초기화
                conn.ResetDisconCheck();
                bool isDbError = false;
                while (true)
                {
                    isDbError = false;
                    QueryMsg.XOFSMST_Query msg = new QueryMsg.XOFSMST_Query(lotID);
                    string query = msg.GetQuery();
                    _LOG.WriteLoadData(_SubPath, query, 0, "XOFSMST", 0.0);
                    if (query == "")
                    {
                        Log.Write($"[Error] DB Serach XOFSMST query is empty.");
                        return false;
                    }

                    string logData;
                    _DbResult.XOFSMST.Clear();

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
                                XOFSMSTData data = new XOFSMSTData();
                                data.Parse(reader);
                                _DbResult.XOFSMST.Add(data);

                                logData = string.Format($"{_DbResult.XOFSMST.Count}\t-\t{data.ToString()}");
                                _LOG.WriteLoadData(_SubPath, logData, _DbResult.XOFSMST.Count, "XOFSMST", 0.0);
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

                            if (conn.IsDisconnCheckout() == true)
                                break;
                        }
                    }
                }


                return true;

            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach XOFSMST error message : [{ex.Message}]");
                return false;
            }
        }
        protected bool SearchXOFSMST_TEST()
        {
            try
            {
                string lotID = LotID;
                string path = string.Empty;
                path = Path.Combine(_LOG.GetBcrPath(), _SubPath, lotID, "XOFSMST_DBResult.txt");

                using (var reader = new StreamReader(path, Encoding.UTF8))
                {
                    string text;
                    while ((text = reader.ReadLine()) != null)
                    {
                        if (text.Contains("SELECT") == true) continue;

                        XOFSMSTData data = new XOFSMSTData();
                        data.Parse(text);
                        _DbResult.XOFSMST.Add(data);
                    }
                }
                return true;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach XOFSMST error message : [{ex.Message}]");
                return false;
            }
        }

        protected bool SearchAreaDel(ref AREADELList listAreaDel)
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;

            try
            {
                string lotID = LotID;
                QueryMsg.AREADEL_Query msg = new QueryMsg.AREADEL_Query(lotID);
                string query = msg.GetQuery();
                _LOG.WriteLoadData(_SubPath, query, 0, "AREADEL", 0.0);
                if (query == "")
                {
                    Log.Write($"[Error] DB Serach AREADEL query is empty.");
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
                            AREADELData data = new AREADELData();
                            data.Parse(reader);
                            listAreaDel.Add(data);

                            logData = string.Format($"{listAreaDel.Count}\t-\t{data.ToString()}");
                            _LOG.WriteLoadData(_SubPath, logData, listAreaDel.Count, "AREADEL", 0.0);
                        }
                    }
                }
                return true;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach AREADEL error message : [{ex.Message}]");
                return false;
            }
        }

        protected bool SearchPTRYLP()
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;

            bool success = false;
            try
            {
                string lotID = LotID;
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
                    using (var reader = comm.ExecuteReader(CommandBehavior.SequentialAccess))
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
                Log.Write($"[Error] DB Serach PTRYLP error message : [{ex.Message}]");
                return false;
            }
        }

        protected bool SearchPTRY0P()
        {
            try
            {
                string lotID = LotID;
                QueryMsg.PTRY0P_Query msg = new QueryMsg.PTRY0P_Query(lotID);

                // PTRLYP에서 획득한 Lot Data  만큼 쿼리 탐색 구문 추가
                string query = msg.GetQuery(_DbResult.PTRLYP);
                _LOG.WriteLoadData(_SubPath, query, 0, "PTRY0P", 0.0);

                if (query == "")
                {
                    Log.Write($"[Error] DB Serach PTRY0P query is empty.");
                    return false;
                }

                conn.ResetDisconCheck();
                bool isDbError = false;
                int logCnt = 0;
                while (true)
                {
                    isDbError = false;

                    for (int i = 0; i < _DbResult.PTRY0P.Length; i++)
                        _DbResult.PTRY0P[i].Clear();

                    string logData = "";

                    if (conn.Connection.State != ConnectionState.Open)
                        conn.Connect();

                    using (var comm = new OracleCommand(query, conn.Connection))
                    {
                        using (OracleDataReader reader = comm.ExecuteReader(CommandBehavior.SequentialAccess))
                        {
                            while (true)
                            {
                                if (conn.Connection.State != ConnectionState.Open)
                                {
                                    isDbError = true;
                                    break;
                                }
                                if (reader.Read() == false) break;
                                string strYOKLOT = reader[7].ToString();
                                string strY0LNSN = reader[9].ToString();

                                // 파싱이 잘못되었을 경우, 999로 처리함.
                                if (Int32.TryParse(reader[2].ToString(), out int nY0PPCD) == false)
                                    nY0PPCD = 999;

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

                                switch (nY0PPCD)
                                {
                                    case 100:   // 연신
                                        {
                                            _DbResult.PTRY0P[(int)eFCD.ES].Add(data);
                                            logCnt = _DbResult.PTRY0P[(int)eFCD.ES].Count;
                                            break;
                                        }

                                    case 400:   // 도공
                                        {
                                            _DbResult.PTRY0P[(int)eFCD.TG].Add(data);
                                            logCnt = _DbResult.PTRY0P[(int)eFCD.TG].Count;
                                            break;
                                        }

                                    default: // 그 외
                                        {
                                            _DbResult.PTRY0P[(int)eFCD.ETC].Add(data);
                                            logCnt = _DbResult.PTRY0P[(int)eFCD.ETC].Count;
                                            break;
                                        }
                                }

                                logData = string.Format($"{logCnt}\t-\t{data.ToString()}");
                                _LOG.WriteLoadData(_SubPath, logData, logCnt, "PTRY0P", 0.0);
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

                            if (conn.IsDisconnCheckout() == true)
                                break;
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

        protected bool SearchPTRY0P_TEST()
        {

            try
            {
                string lotID = LotID;
                string path = string.Empty;
                path = Path.Combine(_LOG.GetBcrPath(), _SubPath, lotID, "PTRY0P_DBResult.txt");

                using (var reader = new StreamReader(path, Encoding.UTF8))
                {
                    string text;
                    while ((text = reader.ReadLine()) != null)
                    {
                        if (text.Contains("SELECT") == true) continue;

                        PTRY0PData data = new PTRY0PData();
                        data.Parse(text);

                        string strYOKLOT = data.Y0KLOT;
                        string strY0LNSN = data.Y0LNSN;

                        // DATA 갖고 오지 않아서 INSPDATA에서 찾아야 함
                        int nY0PPCD = System.Convert.ToInt32(data.Y0PPCD);

                        if (Char.IsLetter(strYOKLOT, 0) == true)
                            strYOKLOT = strYOKLOT.Substring(0, 10); // 나중에 사이즈는 설정해야함.
                        else
                        {
                            int pos = strYOKLOT.IndexOf(' ');
                            if (pos > 0)
                                strYOKLOT = strYOKLOT.Substring(0, pos);
                        }

                        switch (nY0PPCD)
                        {
                            case 100: _DbResult.PTRY0P[(int)eFCD.ES].Add(data); break; // 연신
                            case 400: _DbResult.PTRY0P[(int)eFCD.TG].Add(data); break; // 도공
                            default: _DbResult.PTRY0P[(int)eFCD.ETC].Add(data); break; // 그외

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

        protected bool SearchINSPDAT()
        {
            // 연결 확인
            //if (conn?.IsConnected() == false)
            //    return false;

            // 끊어짐에 대한 재연결 처리
            conn.ResetDisconCheck();

            int procStep = 0;
            try
            {
                string lotID = LotID;
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

                        _LOG.WriteLoadData(_SubPath, query, 0, "INSPDAT", 0.0);

                        if (query == "")
                        {
                            Log.Write($"[Error] INSPDAT{((eFCD)i).ToString()} Query message is empty.");
                            return false;
                        }


                        INSPDATList inspDataList = new INSPDATList();
                        bool isDbError = false;
                        while (true)
                        {
                            isDbError = false;
                            inspDataList.Clear();

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

                                        INSPDATData data = new INSPDATData();
                                        data.Y0KLOT = _DbResult.PTRY0P[idx][i].Y0KLOT;
                                        data.LNCD = _DbResult.PTRY0P[idx][i].LNCD;
                                        data.Parse(reader);

                                        // 리스트에 데이터 추가함
                                        inspDataList.Add(data);
                                        dataCnt++;
                                        _LOG.WriteLoadData(_SubPath, data.ToString(), dataCnt, "INSPDAT", 0.0);
                                    }
                                }
                            }

                            if (isDbError == false)
                            {
                                // 최종 데이터 입력
                                _DbResult.INSPDAT[idx].Add(inspDataList);
                                break;
                            }

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
        protected bool SearchINSPDAT_TEST()
        {
            int procStep = 0;
            string lotID = LotID;
            try
            {
                long dbCnt = 0;
                int dataCnt = 0;
                bool useXOffset = destConfig.UseXOffset;

                eCSV_TYPE csvType = destConfig.GetCsvType();

                Dictionary<string, INSPDATList> dicList = new Dictionary<string, INSPDATList>();

                string path = string.Empty;
                path = Path.Combine(_LOG.GetBcrPath(), _SubPath, lotID, "INSPDAT_DBResult.txt");

                using (var reader = new StreamReader(path, Encoding.UTF8))
                {
                    string text;
                    while ((text = reader.ReadLine()) != null)
                    {
                        if (text.Contains("SELECT") == true) continue;

                        INSPDATData data = new INSPDATData();
                        data.Parse(text);

                        foreach (var opList in _DbResult.PTRY0P)
                        {
                            foreach (var op in opList.Data)
                            {
                                if (op.LNCD == data.USEFLG)
                                {
                                    data.Y0KLOT = op.Y0KLOT;
                                    data.LNCD = op.LNCD;

                                    if (dicList.ContainsKey(op.LNCD) == true)
                                    {
                                        bool isExist = false;
                                        foreach (var dicItem in dicList[op.LNCD].Data)
                                            if (data.CTLNO == dicItem.CTLNO) isExist = true;

                                        if (isExist == false) dicList[op.LNCD].Add(data);
                                    }
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

                foreach (var item in dicList)
                {
                    if (item.Value.Count > 0)
                    {
                        if (item.Value[0].KTCD == "100")
                            _DbResult.INSPDAT[0].Add(item.Value);
                        else if (item.Value[0].KTCD == "400")
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
    }

}
