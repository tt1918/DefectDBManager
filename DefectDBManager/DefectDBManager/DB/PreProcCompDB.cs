#define USE_MKCD_FROM_SERVER

using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace DefectDBManager
{
    public class PreProcCompDB
    {
        public event DelegateEvent OnUpdateMKCDModel = null;

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

        // 당일 생산할 PTRY0P 데이터
        public List<PTRY0PData> PTRY0P_Today_Data { get; private set; }

        public DbSearchResult _DbResult { get; set; }

        public PrePocResultData FaultData { get; set; }

        public IRollDefectInfo _RollDefectInfo { get; set; }
        public CSV_DEFECT_HEADER _CsvDefectHeader = null;
        public List<CSVLoadInfo> _CSVLoadInfo = null;

        public NittoDBProgress DB_Progress { get; private set; }

        public string SearchLotName { get; set; }

        public string SearchY0LNCD { get; set; }

        public LogDB _LOG;

        public MKCD_MODEL MKCD_Model { get; set; }

        public MkcdParam MKCD_Param { get; set; }

        // 상위 객체
        private object owner;
        private bool _doDiscon = true;

        public PreProcCompDB(object parent, OracleDbConnection dbconn, bool disconnDB = true)
        {
            owner = parent;
            conn = dbconn;
            _DbResult = new DbSearchResult();
            DB_Progress = new NittoDBProgress();
            _CSVLoadInfo = new List<CSVLoadInfo>();
            _LOG = new LogDB();

            PTRY0P_Today_Data = new List<PTRY0PData>();

            // Check MKCD Model Folder 
            if (Directory.Exists(Define.MKCDModelPath) == false)
                Directory.CreateDirectory(Define.MKCDModelPath);

            MKCD_Model = new MKCD_MODEL();
            MKCD_Param = new MkcdParam();

            _doDiscon = disconnDB;
        }

        ~PreProcCompDB()
        {
            if(_doDiscon == true)
                conn?.Dispose();
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

        public bool SearchTodayPTRY0PList(out int firstIdx)
        {
            firstIdx = -1;
            // 연결 확인
            if (conn?.IsConnected() == false)   return false;

            try
            {
                _LOG.Lot = "PTRY0P_Today_" + DateTime.Today.ToString("yyyyMMdd");

                // Daily Lot DATA 내용을 초기화 한다 
                PTRY0P_Today_Data.Clear();

                DB_Progress.ResetAll();
                DB_Progress._CurrentStep = eNittoDBProgress.PTRY0P_TODAY;

                QueryMsg.PTRY0P_Today_Query ptry0p = new QueryMsg.PTRY0P_Today_Query();
                
                // 해당 LNCD는 현재 공정 LINE CODE임
                ptry0p.Y0LNCD = destConfig.MainLNCD;

                string query = ptry0p.GetQuery();
                _LOG.WriteLoadData(query.ToString(), 0, "PTRY0P_Today", 0);
                
                if (query == "")
                {
                    Log.Write($"[Error] DB Serach PTRY0P_Today query is empty.");
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

                            // 우선 전체 데이터 넣는다.
                            PTRY0P_Today_Data.Add(data);

                            string logData = string.Format($"{PTRY0P_Today_Data.Count}\t-\t{data.ToString()}");
                            _LOG.WriteLoadData(logData, 0, "PTRY0P_Today", 0);
                        }
                        DB_Progress.Set(eNittoDBProgress.PTRY0P);
                    }
                }

                if (PTRY0P_Today_Data.Count == 0)
                    return false;

                // 이름으로 랏 정렬을 한다.
                PTRY0P_Today_Data = PTRY0P_Today_Data.OrderBy(p => p.Y0KLOT).ToList();

                // 데이터 초기화
                ResetDataAll();

                // 생산하지 않은 맨 처음 랏을 가지고 온다. 
                PTRY0PData firstItem=null;
                for (int i=-0; i< PTRY0P_Today_Data.Count; i++)
                {
                    if (PTRY0P_Today_Data[i].Y0KKOL.Substring(8) == "000000" && PTRY0P_Today_Data[i].Y0KSOL.Substring(8) == "000000")
                    {
                        firstItem = PTRY0P_Today_Data[i];
                        firstIdx = i;
                        break;
                    }
                }

                //원래 여기서 데이터 탐색만 해야 함. 
                // Test Code 나중에 삭제 처리.
                string tmpLotName = "";

                if (firstItem == null) return false;

                tmpLotName = firstItem.Y0KLOT;

                bool success;
                success = SearchPTRYLP(tmpLotName);
                if (success == false) return false;

                success = SearchXOFSMST(tmpLotName);
                if (success == false) return false;

                success = SearchPTRY0P(tmpLotName);
                if (success == false) return false;

                // MKCD Model 데이터를 읽어옴
                success = applyMKCD_Model();

                // 2-1. 모델 없음. 현재 랏 기준으로 모델 탐색 및 모델 업데이트 처리.
                if (success == false)
                {
                    //마킹 컨트롤 마스터 데이터 검색
                    success = SearchMRKCTLMST(tmpLotName);
                    if (success == false) { return false; }

                    MKCD_MODEL mKCD_MODEL = new MKCD_MODEL();
                    mKCD_MODEL.Name = MKCD_Param.Name;
                    foreach (MRKCTLMSTData data in _DbResult.MRKCTLMST_Data)
                        mKCD_MODEL.Add(data.LNCD, new MKCD_Data(data));

                    MKCD_Model = mKCD_MODEL;

                    //2-2. 모델 저장하는 기능 추가되어야 함. 
                    MKCD_Model.Save();
                }
                else
                {
                    //마킹 컨트롤 마스터 데이터 검색
                    success = SearchMRKCTLMST(tmpLotName);
                    if (success == true)
                    {
                        bool isUpdated = false;
                        foreach (MRKCTLMSTData data in _DbResult.MRKCTLMST_Data)
                        {
                            if (MKCD_Model.Param.ContainsKey(data.LNCD) == false)
                            {
                                MKCD_Model.Add(data.LNCD, new MKCD_Data(data));
                                isUpdated = true;
                            }
                            else if (MKCD_Model.Param[data.LNCD].Data.ContainsKey(data.FLTID) == false)
                            {
                                MKCD_Model.Add(data.LNCD, new MKCD_Data(data));
                                isUpdated = true;
                            }
                        }

                        if(isUpdated == true)
                            MKCD_Model.Save();
                    }
                }

                // inspData 불러옴.
                success = SearchINSPDAT(tmpLotName);
                if (success == false) return false;

                // 첫 검사 랏은 복사하여둔다
                CopyInspDatToMatchedInspData();

                success = SearchFLTDAT();
                if (success == false) return false;
            }
            catch ( Exception ex)
            {
                Log.Write($"[Error] DB Serach PTRY0P_TODAY Data error message : [{ex.Message}]");
                return false;
            }

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

            // 각 공정 별 INSPDAT 데이터의 갯수를 확인한다.
            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            isAvaliable = new bool[count];

            for (int i=0; i<count; i++)
            {
                isAvaliable[i] = false;
                if (_DbResult.Matched_INSPDAT_Data[i].Count>0)
                {
                    foreach (INSPDATData data in _DbResult.Matched_INSPDAT_Data[i])
                    {
                        if (data.BCNO == bcno && (data.XPosStart < dPosY && data.XPosEnd < dPosY))
                            isAvaliable[i] = true;
                    }
                }
            }

            bool isResult = false;
            for (int i = 0; i < count; i++)
                isResult |= isAvaliable[i];

            return isResult;
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
                _LOG.DeleteFolder(lotID);
                _LOG.Lot = lotID;
                DB_Progress.ResetAll();

                QueryMsg.PTRYLP_Query ptrylp = new QueryMsg.PTRYLP_Query(lotID);
                string query = ptrylp.GetQuery();
                long dbCnt = 0;
                _LOG.WriteLoadData(query.ToString(), 0, "PTRYLP", 0);

                if (query == "")
                {
                    errOut = 1;
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
                            _DbResult.PTRLYP_Data.Add(data);
                            string logData = string.Format($"{_DbResult.PTRLYP_Data.Count}\t-\t{data.ToString()}");
                            _LOG.WriteLoadData(logData, 0, "PTRYLP", 0);
                        }

                        DB_Progress.Complete(eNittoDBProgress.PTRYLP);
                        success = true;
                    }
                }

                if (success == false)
                {
                    DB_Progress.SetError(eNittoDBProgress.PTRYLP);
                    errOut = 2;
                    return false;
                }
                success = SearchXOFSMST(lotID);
                if (success == false) { errOut = 3; return false; }

                success = SearchPTRY0P(lotID);
                if (success == false) { errOut = 4; return false; }

#if (!USE_MKCD_FROM_SERVER)
                // MKCD 데이터를 모델에서 불러올 수 있도록 함
                success = applyMKCD_Model();
                if (success == false) { errOut = 5; return false; }
#else
                
                // 1. 마킹컨트롤 마스터 데이터 확인
                success = applyMKCD_Model();
                // 2-1. 모델 없음. 현재 랏 기준으로 모델 탐색 및 모델 업데이트 처리.
                if (success == false)
                {
                    //마킹 컨트롤 마스터 데이터 검색
                    success = SearchMRKCTLMST(lotID);
                    if (success == false) { errOut = 5; return false; }

                    MKCD_MODEL mKCD_MODEL = new MKCD_MODEL();
                    mKCD_MODEL.Name = MKCD_Param.Name;
                    foreach (MRKCTLMSTData data in _DbResult.MRKCTLMST_Data)
                        mKCD_MODEL.Add(data.LNCD, new MKCD_Data(data));

                    MKCD_Model = mKCD_MODEL;

                    //2-2. 모델 저장하는 기능 추가되어야 함. 
                    MKCD_Model.Save();
                }
                else
                {
                    //마킹 컨트롤 마스터 데이터 검색
                    success = SearchMRKCTLMST(lotID);
                    if (success == true)
                    {
                        bool isUpdated = false;
                        foreach (MRKCTLMSTData data in _DbResult.MRKCTLMST_Data)
                        {
                            if (MKCD_Model.Param.ContainsKey(data.LNCD) == false)
                            {
                                MKCD_Model.Add(data.LNCD, new MKCD_Data(data));
                                isUpdated = true;
                            }
                            else if (MKCD_Model.Param[data.LNCD].Data.ContainsKey(data.FLTID) == false)
                            {
                                MKCD_Model.Add(data.LNCD, new MKCD_Data(data));
                                isUpdated = true;
                            }
                        }

                        if (isUpdated == true)
                            MKCD_Model.Save();
                    }
                }
#endif
                success = SearchINSPDAT(lotID);
                if (success == false) { errOut = 6; return false; }

                // 첫 검사 랏은 복사하여둔다
                CopyInspDatToMatchedInspData();

                success = SearchFLTDAT();
                if (success == false) { errOut = -7; return false; }


                return success;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] DB Serach Lot error message : [{ex.Message}]");
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
                foreach (MarkingFaultDatum item in FaultData.MarkData.Data)
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
                            _DbResult.XOFSMST_Data.Add(data);

                            logData = string.Format($"{_DbResult.XOFSMST_Data.Count}\t-\t{data.ToString()}");
                            _LOG.WriteLoadData(logData, _DbResult.XOFSMST_Data.Count, "XOFSMST", 0.0);
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

        public bool SearchMRKCTLMST(string lotID)
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;

            int procStep = 0;

            try
            {
                //DestConfigUnit destUnit = destConfig.SelDestUnit;
                //if (destUnit == null)
                //{
                //    destConfig.SetSelDest(dbOption.FWPlace);
                //    destConfig.SelDestUnit = destUnit;
                //}

                long dbCnt = 0;
                int count = System.Enum.GetValues(typeof(eFCD)).Length;
                string logData = "";

                for (int i = 0; i < count; i++)
                {
                    DB_Progress._CurrentStep = ((eNittoDBProgress)((int)eNittoDBProgress.MRKCTLMST_ES + i));
                    procStep = i;
                    int PTRY0Pcnt = _DbResult.PTRY0P_Data[i].Count;
                    DB_Progress.Reset((eNittoDBProgress)((int)eNittoDBProgress.MRKCTLMST_ES + i));
                    _DbResult.CheckDicMRKCTLMSTSize(PTRY0Pcnt, i);
                    DB_Progress.Set((eNittoDBProgress)((int)eNittoDBProgress.MRKCTLMST_ES + i));

                    for (int j = 0; j < PTRY0Pcnt; j++)
                    {
                        _DbResult.ClearDicMRKCTLMST(i, j);

                        if (_DbResult.PTRY0P_Data[i][j].Y0KLOT.Length > 0)
                        {
                            QueryMsg.MRKCTLMST_Query msg = new QueryMsg.MRKCTLMST_Query();
                            msg.Y0KLOT = _DbResult.PTRY0P_Data[i][j].Y0KLOT;
                            msg.MKCD = DbDestConfig.FixedMKCD;
                            //string query = msg.GetQueryAll((eFCD)i);
                            // 코레노 요청으로 출하처 추가하여 사용하도록 수정함. 
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
                                        _DbResult.MRKCTLMST_Data.Add(data);

                                        logData = string.Format($"{_DbResult.MRKCTLMST_Data.Count}\t-\t{data.ToString()}");
                                        _LOG.WriteLoadData(logData, _DbResult.MRKCTLMST_Data.Count, "MRKCTLMST", 0.0);
                                        // 조건문 추가해야 함
                                        _DbResult.AddDicMRKCTLMST(i, j, data);

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

        public bool SearchPTRYLP(string lotID)
        {
            // 연결 확인
            if (conn?.IsConnected() == false)
                return false;

            bool success = false;
            try
            {

                this.SearchLotName = lotID;
                DB_Progress._CurrentStep = eNittoDBProgress.PTRYLP;
                _LOG.Lot = lotID;
                _LOG.DeleteFolder(lotID);
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
                            _DbResult.PTRLYP_Data.Add(data);
                            string logData = string.Format($"{_DbResult.PTRLYP_Data.Count}\t-\t{data.ToString()}");
                            _LOG.WriteLoadData(logData, 0, "PTRYLP", 0);
                        }

                        DB_Progress.Complete(eNittoDBProgress.PTRYLP);
                        success = true;
                    }
                }

                return success;
            }
            catch(Exception ex)
            {
                Log.Write($"[Error] DB Serach PTRY0P error message : [{ex.Message}]");
                DB_Progress.SetError(eNittoDBProgress.PTRY0P);
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
                string query = msg.GetQuery(_DbResult.PTRLYP_Data);
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
                                    data.LNCD = _DbResult.PTRY0P_Data[idx][i].LNCD;
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

            float inspStartY=0.0f;
            float inspEndY = 0.0f;

            bool bValid = false;

            FaultData = new PrePocResultData();

            try
            {
                for (int fcdIdx = 0; fcdIdx < fcdCnt; fcdIdx++)
                {
                    // 데이터 초기화
                    maxXPos = 0;
                    minXPos = float.MaxValue;
                    
                    // 
                    procStep = fcdIdx;
                    DB_Progress._CurrentStep = ((eNittoDBProgress)((int)eNittoDBProgress.FAULTDAT_ES + fcdIdx));
                    DB_Progress.Reset((eNittoDBProgress)((int)eNittoDBProgress.FAULTDAT_ES + fcdIdx));

                    if (fcdIdx == (int)eFCD.ES)         defectCnt[fcdIdx] = -1;// 확인 안 함
                    else if (fcdIdx == (int)eFCD.TG)    defectCnt[fcdIdx] = -1;// 확인 안 함
                    else if (fcdIdx == (int)eFCD.ETC)  defectCnt[fcdIdx] = -1;// 확인 안 함

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
                            if ((eFCD)fcdIdx == eFCD.ES) DB_Progress.SetError(eNittoDBProgress.FAULTDAT_ES);
                            if ((eFCD)fcdIdx == eFCD.TG) DB_Progress.SetError(eNittoDBProgress.FAULTDAT_TG);
                            if ((eFCD)fcdIdx == eFCD.ETC) DB_Progress.SetError(eNittoDBProgress.FAULTDAT_ETC);
                            return false;
                        }

                        MKCD_LNCD_Data mkcdLncdData = null;
                        using (var comm = new OracleCommand(query, conn.Connection))
                        {
                            using (var reader = comm.ExecuteReader())
                            {
                                dbCnt = reader.RowSize;
                                nItemCnt++;

                                PreProcDefect defectData = new PreProcDefect();

                                PreProcMarkingData preMarkData = new PreProcMarkingData();

                                // 
                                defectData.LNCD = inspdata.LNCD;
                                preMarkData.LNCD = inspdata.LNCD;

                                if (MKCD_Model.Param.ContainsKey(inspdata.LNCD) == true)
                                    mkcdLncdData = MKCD_Model.Param[inspdata.LNCD];
                                else
                                    mkcdLncdData = null;

                                while (reader.Read())
                                {
                                    FLTDATA_DailyData data = new FLTDATA_DailyData();
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


                                    // MKCD Model에서 데이터 가져와서 다시 탐색함. 
                                    bValid = false;
                                    if (mkcdLncdData != null)
                                    {
                                        if (mkcdLncdData.Data.ContainsKey(tmpKey) == true &&
                                            mkcdLncdData.Data[tmpKey].SIZE <= (data.AREA_M + 0.00001f) &&
                                            mkcdLncdData.Data[tmpKey].MRKF1 == true)
                                            bValid = true;
                                        else
                                            continue;
                                    }
                                    else continue;

                                    if (data.OFFSET < inspStartY || data.OFFSET > inspEndY) continue;
                                    if (finalXPos < 0.0f) continue;

                                    // 전체 데이터를 저장한다. 
                                    // Fault Data 처리
                                    FaultDatum tmpFltData = new FaultDatum();

                                    tmpFltData.TBCNO = inspdata.BCNO;
                                    tmpFltData.FLTNO = data.FLTNO;
                                    tmpFltData.OFFSET = data.OFFSET;
                                    tmpFltData.YPOS_M = data.YPOS_M;
                                    tmpFltData.XPOS_M = data.XPOS_M;

                                    if (minXPos > data.XPOS_M) minXPos = data.XPOS_M;
                                    if (maxXPos < data.XPOS_M) maxXPos = data.XPOS_M;

                                    // fault data 추가
                                    tmpFltData.RANK = data.RANK;
                                    tmpFltData.KND = data.KND;
                                    tmpFltData.JIGCD = data.JIGCD;
                                    tmpFltData.MACNO = data.MACNO;

                                    // 리스트에 데이터 삽입
                                    defectData.Data.Add(tmpFltData);


                                    // 마킹 데이터만 처리
                                    if (bValid == false) continue;
                                    
                                    // 코드 불량 카운트 증가
                                    if (inspdata.CTLNO == data.CTLNO)   inspdata.RollCtlCnt++;


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
                                        if (fcdIdx == (int)eFCD.TG && dbOption.useKT==true) // 점착
                                        {
                                            int fldID = Int32.Parse(data.FLTID.Substring(data.FLTID.Length - 2));
                                            markData.DefectLine = FalutFunction.GetLineFromFLTID(fldID);
                                        }
                                        else if ((fcdIdx == (int)eFCD.ES) || (fcdIdx == (int)eFCD.ETC))
                                        {
                                            markData.DefectLine = 0;
                                        }
                                    }

                                    // User Defect Class에 등록된 FLTID는 별도 클래스로 구분
                                    int defectLine = markData.DefectLine;

                                    //RK는 CAMNO별로 Defect Class 를 구분
                                    if (csvType == eCSV_TYPE.NITTO_RK || csvType == eCSV_TYPE.NITTO_RTS || csvType == eCSV_TYPE.KORENO_RK_IJP)
                                        markData.DefectLine += Global.MaxDefectLine * data.CAMNO;


                                    // 실시간 데이터용 데이터 삽입
                                    if (FaultData.IsPreProc == false)
                                        FaultData.Add(markData);
                                    else
                                        preMarkData.Data.Add(markData);

                                    dataCnt++;
                                    logData = data.GetString(dataCnt, tmpFltData.TBCNO);
                                    _LOG.WriteLoadData(logData, dataCnt, "FAULTDAT", 0.0);
                                    defectCnt[fcdIdx]++;
                                }

                                // 각 공정 별 불량 데이터를 입력한다. 
                                FaultData.FLTDAT[fcdIdx].Add(defectData);

                                // 이전 공정 결점 비교하는 경우에만 데이터를 추가한다.
                                if(FaultData.IsPreProc==true)
                                    FaultData.PreMarkData[fcdIdx].Add(preMarkData);
                            }
                        }
                    }
                    
                    DB_Progress.Complete((eNittoDBProgress)((int)eNittoDBProgress.FAULTDAT_ES + fcdIdx));
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


        /// <summary>
        /// 이전공정  Lot명으로 각가의 INSPDAT를 조회
        /// </summary>
        /// <param name="bcno">현재 생산중인 BCNO</param>
        /// <param name="dPosY">현재 생산중인 원단의 원단장 위치</param>
        /// <returns></returns>
        public bool SearchMatchedBCNOLot(string bcno, double dPosY, bool isUpdate=true)
        {
            bool success = true;
            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            List<INSPDATData>[] inspDat = new List<INSPDATData>[count];
            for (int i = 0; i < count; i++)
                inspDat[i] = new List<INSPDATData>();

            // 각 공정별로 탐색
            int added = 0;
            for (int i = 0; i < count; i++)
            {
                foreach (List<INSPDATData> data in _DbResult.INSPDAT_Data[i])
                {
                    foreach (INSPDATData datum in data)
                    {
                        // 매칭되면 데이터를 넣어준다. 
                        if (datum.BCNO == bcno && (datum.XPosStart <= dPosY && datum.XPosEnd >= dPosY))
                        {
                            inspDat[i].Add(datum);
                            added++;
                        }
                    }
                }
            }

            if(isUpdate==true)
                _DbResult.Matched_INSPDAT_Data = inspDat;

            if (added <= 0)
                success = false;

            return success;
        }

        /// <summary>
        ///  현재 랏 생산할 데이터가 확인이 되면 다음 예약랏 FAULTDAT 데이터 탐색 위해서 전체 복사한다.
        ///  
        /// </summary>
        public void CopyInspDatToMatchedInspData()
        {
            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            List<INSPDATData>[] inspDat = new List<INSPDATData>[count];
            for (int i = 0; i < count; i++)
                inspDat[i] = new List<INSPDATData>();

            // 각 공정별로 탐색
            for (int i = 0; i < count; i++)
            {
                foreach (List<INSPDATData> data in _DbResult.INSPDAT_Data[i])
                {
                    foreach (INSPDATData datum in data)
                    {
                        inspDat[i].Add(datum);
                    }
                }
            }

            _DbResult.Matched_INSPDAT_Data = inspDat;
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
            for(int i=0; i<size; i++)
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
            foreach(FaultDatum item in FaultData.FLTDAT[(int)fcd][index].Data)
            {
                System.Drawing.PointF pt = new System.Drawing.PointF();
                pt.X = item.XPOS_M;
                pt.Y = (float)item.OFFSET;
                pts.Add(pt);
            }
        }

        /// <summary>
        /// MKCD Model 적용
        /// Model 적용 후 Data 취합 처리 Lot Data Download 처리 해야함.
        /// </summary>
        /// <param name="name"></param>
        public void SetMKCDModel(string name)
        {
            MKCD_Param.Set(name);
            OnUpdateMKCDModel?.Invoke();
        }

        private bool applyMKCD_Model()
        {
            bool success = true;
            MKCD_MODEL model = new MKCD_MODEL();

            model.Name = MKCD_Param.Name;
            model.Load();
            if(model.Param.Count==0)   
                success = false;

            if(success == true)
                MKCD_Model = model;
            //else
            //{
            //    model.Name = "Default";
            //    model.Load();

            //    if (model.Param.Count == 0)
            //        success = false;
            //    else
            //        success = true;

            //    if (success == true)
            //        MKCD_Model = model;
            //}

            return success;
        }

        public bool SearchMKCD_Data(string lotName)
        {
            bool success = false;

            try
            {
                // 데이터 초기화
                ResetDataAll();
                success = SearchPTRYLP(lotName);
                if (success == false) return false;

                success = SearchXOFSMST(lotName);
                if (success == false) return false;

                success = SearchPTRY0P(lotName);
                if (success == false) return false;

                //마킹 컨트롤 마스터 데이터 검색
                success = SearchMRKCTLMST(lotName);
                if (success == false) return false;
            }
            catch(Exception ex)
            {
                Log.Write(ex.Message);
            }
            finally
            {

            }
            return success;
        }

    }
}
