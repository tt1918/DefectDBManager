#define TEST_MODE

using DefectDBManager.DB;
using DefectDBManager.Preproc;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Security;
using System.Security.Cryptography;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;
using System.Windows.Forms;
using static System.Net.WebRequestMethods;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.TaskbarClock;

namespace DefectDBManager
{
    public delegate void DelegateLotProgress(int percent);
    
    public sealed class CompPreprocDefect : IDisposable
    {
        #region Event
        // 상위 이벤트 보고 
        // 기간 설정 생산 PTRY0P 탐색
        public event DelegateEvent OnEndSearchingLotList = null;
        
        /// <summary>
        /// 실시간 검색
        /// </summary>
        public event DelegateEvent OnEndLiveSearchLot = null;

        public event DelegateEvent OnEndSelectedLot = null;

        // 프로세스 상에 발생하는 이벤트 보고용
        public event DelegateProcessEvent OnProcessEvent = null;
         // 랏 검색 진행 상황을 상위로 보고
        public event DelegateLotProgress OnLotProgress = null;

        public event DelegateEvent OnStartLiveDefectSearching = null;
        #endregion Event

        #region Param
        /// <summary>
        /// DB Query 및 탐색
        /// </summary>
        public MarkCompDB _DBProc;

        /// <summary>
        /// DB 접근
        /// </summary>
        public OracleDbConnection _DbConn;

        /// <summary>
        /// Destination configuration
        /// </summary>
        public DestConfig _DestConfig;

        /// <summary>
        /// Code configuration
        /// </summary>
        public CodeConfig _CodeConfig;

        /// <summary>
        /// Data 탐색 옵션
        /// </summary>
        public Option _Option;

        bool disposed = false;
        private object parent = null;

        private Thread threadDBConnect = null;

        public PreprocLotManager LotManager
        {
            get { return _lotManager; }
            set { _lotManager = value; }
        }
        private PreprocLotManager _lotManager = null;


        #region 기간 탐색 
        public bool IsRunSearchingLotList
        {
            get; private set;
        } = false;
        public bool StopSearchingLotList
        {
            get; set;
        } = false;
        #endregion

        #region 실시간 탐색
        public bool IsRunLiveTimer
        {
            get { return _timerCheckLiveLot.Enabled; }
        }
        public bool IsRunLiveSearch
        {
            get; private set;
        } = false;
        public bool StopLiveSearch
        {
            get; set;
        } = false;
        #endregion


        #region 선택 Lot 탐색
        public bool IsRunSelectedLotList
        {
            get; private set;
        } = false;
        public bool StopSelectedLotList
        {
            get; set;
        } = false;
        #endregion


        #endregion Param

        public CompPreprocDefect(object parent)
        {
            this.parent = parent;

            _DestConfig = new DestConfig();
            if (_DestConfig.Read() < 0) //Dest.Ini 파일 읽어들임
            {
                MessageBox.Show($"Failed to read {Define.DestPath}");
            }

            _DbConn = new OracleDbConnection();

            // 현재 랏 기준으로 데이터 생성
            _Option = new Option(0);
            _CodeConfig = new CodeConfig();

            _DBProc = new MarkCompDB(this, _DbConn);

            _DBProc.DbDestConfig = _DestConfig;
            _DBProc.DBCodeConfig = _CodeConfig;
            _DBProc.DbOption = _Option;
            
            if (this.threadDBConnect != null)
            {
                this.threadDBConnect.Join(100);
                this.threadDBConnect = null;
            }

            this.threadDBConnect = new Thread(this.DbConnect);
            this.threadDBConnect.Start();
            this.parent = parent;

            // 검색 타이머 초기화
            initCheckLotTimer();
        }

        ~CompPreprocDefect()
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
                _DbConn.Dispose();
            }
            
            closeCheckLotTimer();

            this.disposed = true;
        }

        private void DbConnect()
        {
            try
            {
                _DbConn.Connect();
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] : {ex.Message}");
            }
        }

        #region 실시간 데이터 오차 비교
        public void SearchLiveMarkDiff()
        {
            Task task = new Task(searchLiveDefect, null);
            task.Start();
        }

        private void searchLiveDefect(object obj)
        {
            if (IsRunLiveSearch == true) return;
            IsRunLiveSearch = true;

            // 동시에 Live와 Search가 구동되지 못 하도록 함.
            while (IsRunSearchingLotList)
                Thread.Sleep(1000);

            Log.Write(Language.startRealtimeSearch);
            OnStartLiveDefectSearching?.Invoke();

            // 기존 데이터 삭제
            LotManager.Live.ClearData();

            // 해당 공정에 대한 결점 정보 확인
            searchLiveLotList();

            // 데이터 검색
            int productIdx = 0;
            foreach (var list in LotManager.Live.Product)
            {
                string[] keyData = Helper.SplitKeyData(list.Key);

                string lncd = keyData[0];
                foreach (var item in list.Value.Data)
                {
                    if (StopLiveSearch == true) break;

                    string lotName = item.Y0KLOT;
                    PreprocItem preprocItem = null;
                    for (int i = 0; i < LotManager.ProcSetting.Count; i++)
                        if (LotManager.ProcSetting[i].Name == keyData[2]) { preprocItem = LotManager.ProcSetting[i]; break; }

                    ProcFilter filter = LotManager.CrtProcFilter[(int)eProc.Live][productIdx];

                    _DBProc.SetFilterParam(lncd, keyData[1], preprocItem);

                    SearchLiveDefectData(list.Key, lotName, preprocItem, filter);

                    // 검색 진행 상황을 
                    int rate = (int)((float)LotManager.Live.TotalLot / (float)LotManager.Live.TotalProduct);
                    OnLotProgress?.Invoke(rate);
                }

                productIdx++;
            }

            StopLiveSearch = false;
            IsRunLiveSearch = false;

            Log.Write(Language.finishRealtimeSearch);
            // 완료 보고
            OnEndLiveSearchLot?.Invoke();
        }

        /// <summary>
        /// 실시간 공정 별 생산 리스트 취합
        /// </summary>
        private void searchLiveLotList()
        {
            if (LotManager.Live != null)
                LotManager.Live.ClearLot();
            // 금일 날자로 설정
            DateTime stTime = DateTime.Today;
            DateTime edTime = DateTime.Now;

            ProcFilterList filter = LotManager.CrtProcFilter[(int)eProc.Live];
            string lncd = string.Empty;
            bool isSkip = false;
            foreach (var data in filter.Data)
            {
                // 검색 대상이 아니면 처리하지 않음.
                if (data.IsInTime() == false) continue;

                lncd = string.Empty;
                isSkip = false;

                for (int i = 0; i < LotManager.ProcLNCD.Info.Count; i++)
                {
                    if (LotManager.ProcLNCD.Info[i].Name == data.Line)
                    {
                        if (LotManager.ProcLNCD.Info[i].CheckStatus == true)
                            isSkip = true;
                        lncd = LotManager.ProcLNCD.Info[i].LNCD;
                        break;
                    }
                }

                if (isSkip == true)
                {
                    data.ResetTime();
                    continue;
                }

                string productName = data.Product;
                bool isWildCard = false;
                if (productName.ElementAt(0) == '*' && productName.ElementAt(productName.Length - 1) == '*')
                {
                    isWildCard = true;
                    productName = productName.Trim('*');    // * 은 지우고 필요한 내용만 남김
                }
                else isWildCard = false;

                if (LotManager.Live.LotHistory.Histroy == null)
                    LotManager.Live.CreateLotHistory();

#if TEST_MODE
                if (_DBProc.SearchPTRYOPList_TEST(lncd, data, stTime, edTime, LotManager.Live.LotHistory, LogDB.eDataType.Realtime) == true)
#else
                if (_DBProc.SearchPTRYOPList(lncd, data, stTime, edTime, LotManager.Live.LotHistory, LogDB.eDataType.Realtime) == true)
#endif
                {
                    PTRY0PList list = new PTRY0PList();
                    foreach (var ptry0p in _DBProc.PTRY0PList_Data.Data)
                    {
                        if (isWildCard == true && ptry0p.Y0ZKNM.Contains(productName) == false) continue;
                        else if (isWildCard == false && ptry0p.Y0ZKNM != productName) continue;

                        list.Add(ptry0p.Clone());
                    }

                    // 리스트 데이터 추가
                    LotManager.Live.AddProduct(data.ToString(), list);
                }

                data.ResetTime();
            }
        }

        /// <summary>
        /// 결과 데이터 처리
        /// </summary>
        /// <param name="lncd"></param>
        /// <param name="lotName"></param>
        /// <param name="preprocItem"></param>
        /// <param name="filter"></param>
        public void SearchLiveDefectData(string lncd, string lotName, PreprocItem preprocItem, ProcFilter filter)
        {
            eSearchError error = eSearchError.Normal;
            bool usemkcdModel = LotManager.UseMrkctlmstModel;
            try
            {
#if TEST_MODE
                IPreprocLot lot = _DBProc.SearchLot_TEST(lotName, false, false, LogDB.eDataType.Realtime, ref error);
#else
                IPreprocLot lot = _DBProc.SearchLot(lotName, false, false, LogDB.eDataType.Realtime, ref error);
#endif
                if (lot == null) return;

                // 입력 받은 데이터 기준으로 좌표 비교
                ((PreprocLotFilter)lot).ProcData = preprocItem;
                lot.ComparePosition();

                string logName = $"CompData";
                string subPath = _DBProc._SubPath;

                LogDB log = _DBProc._LOG;
                int idx1 = 0, idx2 = 0;

                int maxStep = preprocItem.Compare.Count; 

                // 이제 비교가 된 데이터에 대해서만 정보를 저장한다. 
                for (int i = 0; i < maxStep; i++)
                {
                    if (preprocItem.Compare[i].IsSplitCTLNO == false)
                    {
                        logName = $"CompData_{preprocItem.Reference.LNCD}_{preprocItem.Compare[i].LNCD}";
                        int nStep = preprocItem.CompRange.Count + 1; // 비교 거리 데이터 확인용
                        for (int j = 0; j < nStep; j++)
                        {
                            if (j == 0)
                            {
                                CompRange range = preprocItem.BasicRange;
                                log.WriteLoadData(subPath, $"[COMPARE BASIC]-[{range.MinXRange},{range.MinYRange}]~[{range.MaxXRange},{range.MaxYRange}]", j, logName, 0.0, true);
                            }
                            else
                            {
                                CompRange range = preprocItem.CompRange[j - 1];
                                log.WriteLoadData(subPath, $"[COMPARE Range {j}] - [{range.MinXRange},{range.MinYRange}]~[{range.MaxXRange},{range.MaxYRange}]", j, logName, 0.0);
                            }

                            idx1 = 0;
                            foreach (var item in lot.MarkCompList.Data)
                            {
                                if (!item.Comp1.Any(kv => kv.Key.Item1 == preprocItem.Compare[i].LNCD
                                                    && kv.Value[j].Count > 0)) continue;

                                var itemList = item.Comp1.
                                       Where(kv => kv.Key.Item1 == preprocItem.Compare[i].LNCD).ToList();

                                idx2 = 0;
                                string msg = String.Format($"{idx1},{idx2}\t-\t{item.Base.CTLNO}, {item.Base.FLTNO}, {item.Base.OFFSET:0.00}, {item.Base.YPOS_M:0.00}, {item.Base.XPOS_M:0.00}, " +
                                                            $"{item.Base.FAULTID}, {item.Base.SIZE:0.00}, {item.Base.CAM_NO}, {item.Base.MNTTID}, {item.Base.BCNO}");
                                log.WriteLoadData(subPath, msg, idx1, logName, 0.0);
                                idx2++;

                                foreach (var kv in itemList)
                                {
                                    foreach (var datum in kv.Value[j])
                                    {
                                        msg = String.Format($"{idx1},{idx2}\t-\t{datum.CTLNO}, {datum.FLTNO}, {datum.OFFSET:0.00}, {datum.YPOS_M:0.00}, {datum.XPOS_M:0.00}, " +
                                                                $"{datum.FAULTID}, {datum.SIZE:0.00}, {datum.CAM_NO}, {datum.MNTTID}, {datum.BCNO}");
                                        log.WriteLoadData(subPath, msg, idx1, logName, 0.0);
                                        idx2++;
                                    }
                                }
                                idx1++;
                            }
                        }
                    }
                    else
                    {
                        if (lot.MarkCompList.Data.Count != 0)
                        {
                            bool exists = lot.MarkCompList.Data[0].Comp1.Keys.Any(k => k.Item1 == preprocItem.Compare[i].LNCD);
                            if (exists == true)
                            {
                                foreach (var aaa in lot.MarkCompList.Data[0].Comp1)
                                {
                                    if (aaa.Key.Item1 == preprocItem.Compare[i].LNCD)
                                    {
                                        logName = $"CompData_{preprocItem.Reference.LNCD}_{preprocItem.Compare[i].LNCD}_{aaa.Key.Item2}";
                                        int nStep1 = lot.MarkCompList.Data[0].Comp1[aaa.Key].GetLength(0); // 비교 거리 데이터 확인용

                                        for (int j = 0; j < nStep1; j++)
                                        {
                                            if (j == 0)
                                            {
                                                CompRange range = preprocItem.BasicRange;
                                                log.WriteLoadData(subPath, $"[COMPARE BASIC]-[{range.MinXRange},{range.MinYRange}]~[{range.MaxXRange},{range.MaxYRange}]", j, logName, 0.0, true);
                                            }
                                            else
                                            {
                                                CompRange range = preprocItem.CompRange[j - 1];
                                                log.WriteLoadData(subPath, $"[COMPARE Range {j}] - [{range.MinXRange},{range.MinYRange}]~[{range.MaxXRange},{range.MaxYRange}]", j, logName, 0.0);
                                            }

                                            idx1 = 0;
                                            foreach (var item1 in lot.MarkCompList.Data)
                                            {
                                                if (item1.Comp1[aaa.Key][j].Count > 0)
                                                {
                                                    idx2 = 0;
                                                    string msg = String.Format($"{idx1},{idx2}\t-\t{item1.Base.CTLNO}, {item1.Base.FLTNO}, {item1.Base.OFFSET:0.00}, {item1.Base.YPOS_M:0.00}, {item1.Base.XPOS_M:0.00}, " +
                                                                                $"{item1.Base.FAULTID}, {item1.Base.SIZE:0.00}, {item1.Base.CAM_NO}, {item1.Base.MNTTID}, {item1.Base.BCNO}");
                                                    log.WriteLoadData(subPath, msg, idx1, logName, 0.0);
                                                    idx2++;

                                                    for (int k = 0; k < item1.Comp1[aaa.Key][j].Count; k++)
                                                    {
                                                        MarkingFaultDatum datum = item1.Comp1[aaa.Key][j][k];
                                                        msg = String.Format($"{idx1},{idx2}\t-\t{datum.CTLNO}, {datum.FLTNO}, {datum.OFFSET:0.00}, {datum.YPOS_M:0.00}, {datum.XPOS_M:0.00}, " +
                                                                                $"{datum.FAULTID}, {datum.SIZE:0.00}, {datum.CAM_NO}, {datum.MNTTID}, {datum.BCNO}");
                                                        log.WriteLoadData(subPath, msg, idx1, logName, 0.0);
                                                        idx2++;
                                                    }
                                                    idx1++;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                LotManager.Live.AddLot(lncd, lot);

                Thread.Sleep(200);
            }
            catch(Exception ex)
            {
                Log.Write($"[Error] SearchLiveDefectData : {ex.Message}");
            }
        }
        #endregion

        #region Live Search Timer 
        System.Timers.Timer _timerCheckLiveLot = null;

        private void initCheckLotTimer()
        {
            closeCheckLotTimer();

            _timerCheckLiveLot = new System.Timers.Timer();
            _timerCheckLiveLot.Interval = 10 * 1000;    // 10초에 1번씩 탐색하도록 함
            _timerCheckLiveLot.Elapsed += new ElapsedEventHandler(checkLiveLot);

        }

        private void closeCheckLotTimer()
        {
            try
            {
                if (_timerCheckLiveLot != null)
                {
                    _timerCheckLiveLot.Stop();
                    _timerCheckLiveLot.Elapsed -= checkLiveLot;
                    _timerCheckLiveLot.Dispose();
                    _timerCheckLiveLot = null;
                }
            }
            catch
            {

            }
        }

        public void CheckFilterExist()
        {

        }

        public void StartLiveLot()
        {
            bool isSync = LotManager.CrtProcFilter.UseLiveSync;
            // 전체 싱크 모드를 사용하면
            if (isSync == true)
            {
                int syncDuration = LotManager.CrtProcFilter.SyncDuration;
                foreach (var data in LotManager.CrtProcFilter[(int)eProc.Live].Data)
                    data.SetTime(syncDuration);
            }
            else
            {
                foreach (var data in LotManager.CrtProcFilter[(int)eProc.Live].Data)
                    data.SetTime();
            }
#if !TEST_MODE
            // 검색 데이터 폴더 정리
            foreach (var data in LotManager.CrtProcFilter[(int)eProc.Live].Data)
            {
                string filter = Helper.ReplaceInvalidPathChar(data.ToString());
                string path = $"{Define.RealtimeBCRPath}\\{filter}";
                if(Directory.Exists(path))
                    Directory.Delete(path, true);
            }

            LotManager.Live.LotHistory.Clear();
#endif
            _timerCheckLiveLot.Start();
        }
        public void StopLiveLot()
        {
            _timerCheckLiveLot.Stop();
        }

        public bool IsRunLiveCheck()
        {
            return _timerCheckLiveLot.Enabled || IsRunSearchingLotList;
        }

        private void checkLiveLot(object sender, ElapsedEventArgs e)
        {
            // 검색 중이면 스킵 처리
            if (IsRunLiveSearch == true) return;

            // 탐색 가능 확인
            bool doStart = false;
            int size = LotManager.CrtProcFilter[(int)eProc.Live].Count;
            for (int i = 0; i < size; i++)
            {
                if (LotManager.CrtProcFilter[(int)eProc.Live].Data[i].IsInTime() == true)
                    doStart = true;
            }

            // 탐색 시간이 안되었으면 스킵한다. 
            if (doStart == false) return;

            SearchLiveMarkDiff();
        }
        #endregion

        #region 기간 설정 데이터 오차 비교
        public void SearchLotMarkDiff()
        {
            // LotManager의 데이터는 업데이트되어있는 상황
            Task task = new Task(searchDefect, null);
            task.Start();
        }
        
        /// <summary>
        /// 기간 내의 Lot 정보 취합
        /// </summary>
        /// <param name="obj"></param>
        private void searchDefect(object obj)
        {
            if (IsRunSearchingLotList == true) return;
            IsRunSearchingLotList = true;

            // 동시에 Live와 Search가 구동되지 못 하도록 함.
            while (IsRunLiveSearch)
                Thread.Sleep(1000);

            Log.Write(Language.startSearchingbyPeriod);
            // 해당 공정에 대한 결점 정보 확인
            searchLotList();

            LotManager.Search.ClearLot();
            int productIdx = 0;
            foreach (var list in LotManager.Search.Product)
            {
                string[] keyData = Helper.SplitKeyData(list.Key);
                string lncd = keyData[0];

                ProcFilter filter = LotManager.CrtProcFilter[(int)eProc.Search][productIdx];
                if (filter.Use == true)
                {
                    PreprocItem preprocItem = null;
                    for (int i = 0; i < LotManager.ProcSetting.Count; i++)
                        if (LotManager.ProcSetting[i].Name == keyData[2]) preprocItem = LotManager.ProcSetting[i];

                    _DBProc.SetFilterParam(lncd, keyData[1], preprocItem);

                    foreach (var item in list.Value.Data)
                    {
                        if (StopSearchingLotList == true) break;

                        string lotName = item.Y0KLOT;
                        //filter 로 구분하도록 수정 @ATW 250321
                        SearchDefectData(list.Key, lotName, preprocItem, filter);

                        // 검색 진행 상황을 
                        int rate = (int)((float)LotManager.Search.TotalLot / (float)LotManager.Search.TotalProduct);
                        OnLotProgress?.Invoke(rate);
                    }
                }

                productIdx++;
            }

            StopSearchingLotList = false;
            IsRunSearchingLotList = false;

            Log.Write(Language.finishPeriodicSearch);
            // 완료 보고
            OnEndSearchingLotList?.Invoke();
        }

        /// <summary>
        /// 기간 공정 별 생산 리스트 취합.
        /// </summary>
        private void searchLotList()
        {
            LotManager.Search.Product.Clear();

            DateTime stTime = LotManager.SearchTime.StartTime;
            DateTime edTime = LotManager.SearchTime.EndTime;

            stTime = new DateTime(stTime.Year, stTime.Month, stTime.Day);

            ProcFilterList filter = LotManager.CrtProcFilter[(int)eProc.Search];
            string lncd = string.Empty;
            bool isSkip = false;

            foreach (var data in filter.Data)
            {
                if (data.Use == false)
                {
                    PTRY0PList list1 = new PTRY0PList();
                    LotManager.Search.Product.Add(data.ToString(), list1);
                    continue;
                }

                isSkip = false;
                data.IsSkip = false;
                lncd = string.Empty;
                for (int i = 0; i < LotManager.ProcLNCD.Info.Count; i++)
                {
                    if (LotManager.ProcLNCD.Info[i].Name == data.Line)
                    {
                        if (LotManager.ProcLNCD.Info[i].CheckStatus == true)
                            isSkip = true;
                        lncd = LotManager.ProcLNCD.Info[i].LNCD;
                        break;
                    }
                }

                data.IsSkip = isSkip;
                if (isSkip) continue;

                string productName = data.Product;
                bool isWildCard = false;
                if (productName.ElementAt(0) == '*' && productName.ElementAt(productName.Length - 1) == '*')
                {
                    isWildCard = true;

                    // * 은 지우고 필요한 내용만 남김
                    productName = productName.Trim('*');
                }
                else
                {
                    isWildCard = false;
                }

                PTRY0PList list = new PTRY0PList();
#if TEST_MODE
                if (_DBProc.SearchPTRYOPList_TEST(lncd, data, stTime, edTime, null, LogDB.eDataType.SearchLot) == true)
#else
                if (_DBProc.SearchPTRYOPList(lncd, data, stTime, edTime, null, LogDB.eDataType.SearchLot) == true)
#endif
                {
                    foreach (var ptry0p in _DBProc.PTRY0PList_Data.Data)
                    {
                        if (isWildCard == true && ptry0p.Y0ZKNM.Contains(productName) == false) continue;
                        else if (isWildCard == false && ptry0p.Y0ZKNM != productName) continue;

                        list.Add(ptry0p.Clone());
                    }
                }

                // 리스트 데이터 추가
                LotManager.Search.Product.Add(data.ToString(), list);
            }
        }
       
        /// <summary>
        /// 결과 데이터 처리
        /// </summary>
        /// <param name="lncd"></param>
        /// <param name="lotName"></param>
        /// <param name="preprocItem"></param>
        /// <param name="filter"></param>
        public void SearchDefectData(string lncd, string lotName, PreprocItem preprocItem, ProcFilter filter)
        {
            eSearchError error = eSearchError.Normal;
            try
            {
#if TEST_MODE
                IPreprocLot lot = _DBProc.SearchLot_TEST(lotName, false, false, LogDB.eDataType.SearchLot, ref error);
#else
                IPreprocLot lot = _DBProc.SearchLot(lotName, true, false, LogDB.eDataType.SearchLot, ref error);
#endif

                if (lot == null) return;

                // 입력 받은 데이터 기준으로 좌표 비교
                ((PreprocLotFilter)lot).ProcData = preprocItem;
                lot.ComparePosition();

                string logName = $"CompData";
                string subPath = _DBProc._SubPath;

                LogDB log = _DBProc._LOG;
                int idx1 = 0, idx2 = 0;

                int maxStep = preprocItem.Compare.Count;

                // 이제 비교가 된 데이터에 대해서만 정보를 저장한다. 
                for (int i = 0; i < maxStep; i++)
                {
                    if (preprocItem.Compare[i].IsSplitCTLNO == false)
                    {
                        logName = $"CompData_{preprocItem.Reference.LNCD}_{preprocItem.Compare[i].LNCD}";
                        int nStep = preprocItem.CompRange.Count + 1; // 비교 거리 데이터 확인용
                        for (int j = 0; j < nStep; j++)
                        {
                            if (j == 0)
                            {
                                CompRange range = preprocItem.BasicRange;
                                log.WriteLoadData(subPath, $"[COMPARE BASIC]-[{range.MinXRange},{range.MinYRange}]~[{range.MaxXRange},{range.MaxYRange}]", j, logName, 0.0, true);
                            }
                            else
                            {
                                CompRange range = preprocItem.CompRange[j - 1];
                                log.WriteLoadData(subPath, $"[COMPARE Range {j}] - [{range.MinXRange},{range.MinYRange}]~[{range.MaxXRange},{range.MaxYRange}]", j, logName, 0.0);
                            }

                            idx1 = 0;
                            foreach (var item in lot.MarkCompList.Data)
                            {
                                if (!item.Comp1.Any(kv => kv.Key.Item1 == preprocItem.Compare[i].LNCD
                                                    && kv.Value[j].Count > 0)) continue;

                                var itemList = item.Comp1.
                                       Where(kv => kv.Key.Item1 == preprocItem.Compare[i].LNCD).ToList();

                                idx2 = 0;
                                string msg = String.Format($"{idx1},{idx2}\t-\t{item.Base.CTLNO}, {item.Base.FLTNO}, {item.Base.OFFSET:0.00}, {item.Base.YPOS_M:0.00}, {item.Base.XPOS_M:0.00}, " +
                                                            $"{item.Base.FAULTID}, {item.Base.SIZE:0.00}, {item.Base.CAM_NO}, {item.Base.MNTTID}, {item.Base.BCNO}");
                                log.WriteLoadData(subPath, msg, idx1, logName, 0.0);
                                idx2++;

                                foreach (var kv in itemList)
                                {
                                    foreach (var datum in kv.Value[j])
                                    {
                                        msg = String.Format($"{idx1},{idx2}\t-\t{datum.CTLNO}, {datum.FLTNO}, {datum.OFFSET:0.00}, {datum.YPOS_M:0.00}, {datum.XPOS_M:0.00}, " +
                                                                $"{datum.FAULTID}, {datum.SIZE:0.00}, {datum.CAM_NO}, {datum.MNTTID}, {datum.BCNO}");
                                        log.WriteLoadData(subPath, msg, idx1, logName, 0.0);
                                        idx2++;
                                    }
                                }
                                idx1++;
                            }
                        }
                    }
                    else
                    {
                        if (lot.MarkCompList.Data.Count != 0)
                        {
                            bool exists = lot.MarkCompList.Data[0].Comp1.Keys.Any(k => k.Item1 == preprocItem.Compare[i].LNCD);
                            if (exists == true)
                            {
                                foreach (var aaa in lot.MarkCompList.Data[0].Comp1)
                                {
                                    if (aaa.Key.Item1 == preprocItem.Compare[i].LNCD)
                                    {
                                        logName = $"CompData_{preprocItem.Reference.LNCD}_{preprocItem.Compare[i].LNCD}_{aaa.Key.Item2}";
                                        int nStep1 = lot.MarkCompList.Data[0].Comp1[aaa.Key].GetLength(0); // 비교 거리 데이터 확인용

                                        for (int j = 0; j < nStep1; j++)
                                        {
                                            if (j == 0)
                                            {
                                                CompRange range = preprocItem.BasicRange;
                                                log.WriteLoadData(subPath, $"[COMPARE BASIC]-[{range.MinXRange},{range.MinYRange}]~[{range.MaxXRange},{range.MaxYRange}]", j, logName, 0.0, true);
                                            }
                                            else
                                            {
                                                CompRange range = preprocItem.CompRange[j - 1];
                                                log.WriteLoadData(subPath, $"[COMPARE Range {j}] - [{range.MinXRange},{range.MinYRange}]~[{range.MaxXRange},{range.MaxYRange}]", j, logName, 0.0);
                                            }

                                            idx1 = 0;
                                            foreach (var item1 in lot.MarkCompList.Data)
                                            {
                                                if (item1.Comp1[aaa.Key][j].Count > 0)
                                                {
                                                    idx2 = 0;
                                                    string msg = String.Format($"{idx1},{idx2}\t-\t{item1.Base.CTLNO}, {item1.Base.FLTNO}, {item1.Base.OFFSET:0.00}, {item1.Base.YPOS_M:0.00}, {item1.Base.XPOS_M:0.00}, " +
                                                                                $"{item1.Base.FAULTID}, {item1.Base.SIZE:0.00}, {item1.Base.CAM_NO}, {item1.Base.MNTTID}, {item1.Base.BCNO}");
                                                    log.WriteLoadData(subPath, msg, idx1, logName, 0.0);
                                                    idx2++;

                                                    for (int k = 0; k < item1.Comp1[aaa.Key][j].Count; k++)
                                                    {
                                                        MarkingFaultDatum datum = item1.Comp1[aaa.Key][j][k];
                                                        msg = String.Format($"{idx1},{idx2}\t-\t{datum.CTLNO}, {datum.FLTNO}, {datum.OFFSET:0.00}, {datum.YPOS_M:0.00}, {datum.XPOS_M:0.00}, " +
                                                                                $"{datum.FAULTID}, {datum.SIZE:0.00}, {datum.CAM_NO}, {datum.MNTTID}, {datum.BCNO}");
                                                        log.WriteLoadData(subPath, msg, idx1, logName, 0.0);
                                                        idx2++;
                                                    }
                                                    idx1++;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                LotManager.Search.AddLot(lncd, lot);

                Thread.Sleep(200);
            }
            catch (Exception e)
            {
                Log.Write($"[Error] SearchDefectData : {e.Message}");
            }
        }
        #endregion

        #region 선택 Lot 데이터 오차 비교
        List<string> _selLot = null;
        LotSelProcParam _selParam = null;
        /// <summary>
        /// task 실행
        /// </summary>
        public void SearchSelectedLotMarkDiff(List<string> lotList, LotSelProcParam param)
        {
            _selLot = lotList;
            _selParam = param;

            LotManager.Selected.ClearData();

            Task task = null;
            if (_selParam.FilterType == FilterType.UserFilter) task = new Task(searchSelectedLotDefectByUserFilter, null);
            else if (_selParam.FilterType == FilterType.DbFilter) task = new Task(searchSelectedLotDefectByDB, null);

            if (task == null) 
            {
                Log.Write("[Error] Filter type is not initialized.");
                return; 
            }

            task.Start();
        }

        private void searchSelectedLotDefectByUserFilter(object obj)
        {
            if (IsRunSelectedLotList == true) return;
            IsRunSelectedLotList = true;

            // 동시에 Live와 Search가 구동되지 못 하도록 함.
            while (IsRunLiveSearch || IsRunSearchingLotList)
                Thread.Sleep(1000);

            Log.Write(Language.startSelectedLotSearchByUserFilter);

            // Lot 정보는 획득 PTRYOP 데이터 생성해야 함. 
            foreach (var data in _selParam.UserFilter.Data)
            {
                bool isSkip = false;
                string lncd = string.Empty;
                for (int i = 0; i < LotManager.ProcLNCD.Info.Count; i++)
                {
                    if (LotManager.ProcLNCD.Info[i].Name == data.Line)
                    {
                        if (LotManager.ProcLNCD.Info[i].CheckStatus == true)    isSkip = true;
                        lncd = LotManager.ProcLNCD.Info[i].LNCD;
                        break;
                    }
                }

                data.IsSkip = isSkip;
                if (isSkip) continue;

                string productName = data.Product;                
                PTRY0PList list = new PTRY0PList();

                foreach (var lotName in _selLot)
                {
                    PTRY0PData opdata = new PTRY0PData();
                    opdata.Y0KLOT = lotName;
                    list.Add(opdata.Clone());
                }
                
                // 리스트 데이터 추가
                LotManager.Selected.Product.Add(data.ToString(), list);
            }

            LotManager.Selected.ClearLot();
            int productIdx = 0;
            foreach (var list in LotManager.Selected.Product)
            {
                string[] keyData = Helper.SplitKeyData(list.Key);
                string lncd = keyData[0];

                ProcFilter filter = _selParam.UserFilter[productIdx];
                if (filter.Use == true)
                {
                    PreprocItem preprocItem = null;

                    for (int i = 0; i < LotManager.ProcSetting.Count; i++)
                    {
                        if (LotManager.ProcSetting[i].Name == keyData[2])
                        {
                            preprocItem = LotManager.ProcSetting[i].Clone();
                            //preprocItem.CompRange = _selParam.CompRange;
                            //preprocItem.BasicRange = _selParam.BasicRange;
                            //preprocItem.Judge = _selParam.Judge;
                            //preprocItem.UseAiResult = _selParam.UseAiResult;
                        }
                    }

                    _DBProc.SetFilterParam(lncd, keyData[1], preprocItem);

                    foreach (var item in list.Value.Data)
                    {
                        if (StopSelectedLotList == true) break;

                        string lotName = item.Y0KLOT;
                        //filter 로 구분하도록 수정 @ATW 250321
                        SearchSelectedLotDefect(list.Key, lotName, preprocItem, filter);

                        // 검색 진행 상황을 
                        int rate = (int)((float)LotManager.Selected.TotalLot / (float)LotManager.Selected.TotalProduct);
                    }
                }

                productIdx++;
            }

            StopSelectedLotList = false;
            IsRunSelectedLotList = false;

            Log.Write(Language.finishSelectedLotSearch);
            // 완료 보고
            OnEndSelectedLot?.Invoke();
        }

        private void searchSelectedLotDefectByDB(object obj)
        {
            if (IsRunSelectedLotList == true) return;
            IsRunSelectedLotList = true;

            // 동시에 Live와 Search가 구동되지 못 하도록 함.
            while (IsRunLiveSearch || IsRunSearchingLotList)
                Thread.Sleep(1000);

            Log.Write(Language.startSelectedLotSearchByDB);

            // Lot 정보는 획득 PTRYOP 데이터 생성해야 함. 
            PTRY0PList oplist = new PTRY0PList();
            foreach (var item in _selLot)
            {
                PTRY0PData data = new PTRY0PData();
                data.Y0KLOT = item;
                oplist.Add(data);
            }
            LotManager.Selected.Product.Add("DB", oplist);

            DBFilter filter = _selParam.DBFilter;
            LotManager.Selected.ClearLot();
            int productIdx = 0;
            foreach (var list in LotManager.Selected.Product)
            {
                foreach (var item in list.Value.Data)
                {
                    if (StopSelectedLotList == true) break;

                    string lotName = item.Y0KLOT;
                    _DBProc.SetDBParam(filter.Title);

                    SearchSelectedDBLotDefect(lotName, _selParam);

                    // 검색 진행 상황을 
                    int rate = (int)((float)LotManager.Selected.TotalLot / (float)LotManager.Selected.TotalProduct);
                    OnLotProgress?.Invoke(rate);
                }

                productIdx++;
            }

            StopSelectedLotList = false;
            IsRunSelectedLotList = false;

            Log.Write(Language.finishSelectedLotSearch);
            // 완료 보고
            OnEndSelectedLot?.Invoke();
        }

        public void SearchSelectedLotDefect(string lncd, string lotName, PreprocItem preprocItem, ProcFilter filter)
        {
            eSearchError error = eSearchError.Normal;
            try
            {
#if TEST_MODE
                IPreprocLot lot = _DBProc.SearchLot_TEST(lotName, true, false, LogDB.eDataType.SelectedLot, ref error);
#else
                IPreprocLot lot = _DBProc.SearchLot(lotName, true, false, LogDB.eDataType.SelectedLot, ref error);
#endif

                if (lot == null) return;

                // 입력 받은 데이터 기준으로 좌표 비교
                ((PreprocLotFilter)lot).ProcData = preprocItem;
                lot.ComparePosition();

                string logName = $"CompData";
                string subPath = _DBProc._SubPath;

                LogDB log = _DBProc._LOG;
                int idx1 = 0, idx2 = 0;

                int maxStep = preprocItem.Compare.Count;

                // 이제 비교가 된 데이터에 대해서만 정보를 저장한다. 
                for (int i = 0; i < maxStep; i++)
                {
                    if (preprocItem.Compare[i].IsSplitCTLNO == false)
                    {
                        logName = $"CompData_{preprocItem.Reference.LNCD}_{preprocItem.Compare[i].LNCD}";
                        int nStep = preprocItem.CompRange.Count + 1; // 비교 거리 데이터 확인용
                        for (int j = 0; j < nStep; j++)
                        {
                            if (j == 0)
                            {
                                CompRange range = preprocItem.BasicRange;
                                log.WriteLoadData(subPath, $"[COMPARE BASIC]-[{range.MinXRange},{range.MinYRange}]~[{range.MaxXRange},{range.MaxYRange}]", j, logName, 0.0, true);
                            }
                            else
                            {
                                CompRange range = preprocItem.CompRange[j - 1];
                                log.WriteLoadData(subPath, $"[COMPARE Range {j}] - [{range.MinXRange},{range.MinYRange}]~[{range.MaxXRange},{range.MaxYRange}]", j, logName, 0.0);
                            }

                            idx1 = 0;
                            foreach (var item in lot.MarkCompList.Data)
                            {
                                if (!item.Comp1.Any(kv => kv.Key.Item1 == preprocItem.Compare[i].LNCD
                                                    && kv.Value[j].Count > 0)) continue;

                                var itemList = item.Comp1.
                                       Where(kv => kv.Key.Item1 == preprocItem.Compare[i].LNCD).ToList();

                                idx2 = 0;
                                string msg = String.Format($"{idx1},{idx2}\t-\t{item.Base.CTLNO}, {item.Base.FLTNO}, {item.Base.OFFSET:0.00}, {item.Base.YPOS_M:0.00}, {item.Base.XPOS_M:0.00}, " +
                                                            $"{item.Base.FAULTID}, {item.Base.SIZE:0.00}, {item.Base.CAM_NO}, {item.Base.MNTTID}, {item.Base.BCNO}");
                                log.WriteLoadData(subPath, msg, idx1, logName, 0.0);
                                idx2++;

                                foreach (var kv in itemList)
                                {
                                    foreach (var datum in kv.Value[j])
                                    {
                                        msg = String.Format($"{idx1},{idx2}\t-\t{datum.CTLNO}, {datum.FLTNO}, {datum.OFFSET:0.00}, {datum.YPOS_M:0.00}, {datum.XPOS_M:0.00}, " +
                                                                $"{datum.FAULTID}, {datum.SIZE:0.00}, {datum.CAM_NO}, {datum.MNTTID}, {datum.BCNO}");
                                        log.WriteLoadData(subPath, msg, idx1, logName, 0.0);
                                        idx2++;
                                    }
                                }
                                idx1++;
                            }
                        }
                    }
                    else
                    {
                        if (lot.MarkCompList.Data.Count != 0)
                        {
                            bool exists = lot.MarkCompList.Data[0].Comp1.Keys.Any(k => k.Item1 == preprocItem.Compare[i].LNCD);
                            if (exists == true)
                            {
                                foreach (var aaa in lot.MarkCompList.Data[0].Comp1)
                                {
                                    if (aaa.Key.Item1 == preprocItem.Compare[i].LNCD)
                                    {
                                        logName = $"CompData_{preprocItem.Reference.LNCD}_{preprocItem.Compare[i].LNCD}_{aaa.Key.Item2}";
                                        int nStep1 = lot.MarkCompList.Data[0].Comp1[aaa.Key].GetLength(0); // 비교 거리 데이터 확인용

                                        for (int j = 0; j < nStep1; j++)
                                        {
                                            if (j == 0)
                                            {
                                                CompRange range = preprocItem.BasicRange;
                                                log.WriteLoadData(subPath, $"[COMPARE BASIC]-[{range.MinXRange},{range.MinYRange}]~[{range.MaxXRange},{range.MaxYRange}]", j, logName, 0.0, true);
                                            }
                                            else
                                            {
                                                CompRange range = preprocItem.CompRange[j - 1];
                                                log.WriteLoadData(subPath, $"[COMPARE Range {j}] - [{range.MinXRange},{range.MinYRange}]~[{range.MaxXRange},{range.MaxYRange}]", j, logName, 0.0);
                                            }

                                            idx1 = 0;
                                            foreach (var item1 in lot.MarkCompList.Data)
                                            {
                                                if (item1.Comp1[aaa.Key][j].Count > 0)
                                                {
                                                    idx2 = 0;
                                                    string msg = String.Format($"{idx1},{idx2}\t-\t{item1.Base.CTLNO}, {item1.Base.FLTNO}, {item1.Base.OFFSET:0.00}, {item1.Base.YPOS_M:0.00}, {item1.Base.XPOS_M:0.00}, " +
                                                                                $"{item1.Base.FAULTID}, {item1.Base.SIZE:0.00}, {item1.Base.CAM_NO}, {item1.Base.MNTTID}, {item1.Base.BCNO}");
                                                    log.WriteLoadData(subPath, msg, idx1, logName, 0.0);
                                                    idx2++;

                                                    for (int k = 0; k < item1.Comp1[aaa.Key][j].Count; k++)
                                                    {
                                                        MarkingFaultDatum datum = item1.Comp1[aaa.Key][j][k];
                                                        msg = String.Format($"{idx1},{idx2}\t-\t{datum.CTLNO}, {datum.FLTNO}, {datum.OFFSET:0.00}, {datum.YPOS_M:0.00}, {datum.XPOS_M:0.00}, " +
                                                                                $"{datum.FAULTID}, {datum.SIZE:0.00}, {datum.CAM_NO}, {datum.MNTTID}, {datum.BCNO}");
                                                        log.WriteLoadData(subPath, msg, idx1, logName, 0.0);
                                                        idx2++;
                                                    }
                                                    idx1++;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                LotManager.Selected.AddLot(lncd, lot);

                Thread.Sleep(200);
            }
            catch (Exception e)
            {
                Log.Write($"[Error] SearchDefectData : {e.Message}");
            }
        }

        public void SearchSelectedDBLotDefect(string lotName, LotSelProcParam param)
        {
            eSearchError error = eSearchError.Normal;
            try
            {
#if TEST_MODE
                IPreprocLot lot = _DBProc.SearchDBLot_TEST(lotName, param, false, ref error);
#else
                IPreprocLot lot = _DBProc.SearchDBLot(lotName, param, false, ref error);
#endif
                if (lot == null) return;

                string refLNCD = lot.FaultData.MarkData.LNCD;
                List<string> listLNCD = new List<string>();
                foreach(var data in lot.FaultData.PreMarkData)
                {
                    foreach (var item in data)
                    {
                        if (listLNCD.Contains(item.LNCD) == false)
                            listLNCD.Add(item.LNCD);
                    }
                }

                // 입력 받은 데이터 기준으로 좌표 비교
                PreprocLotDB lotDB = (PreprocLotDB)lot;
                lotDB.SetInfo(param, refLNCD, listLNCD);
                lot.ComparePosition();

                string logName = $"CompData";
                string subPath = _DBProc._SubPath;

                LogDB log = _DBProc._LOG;
                int idx1 = 0, idx2 = 0;

                int maxIndex = listLNCD.Count;

                // 이제 비교가 된 데이터에 대해서만 정보를 저장한다. 
                for (int i = 0; i < maxIndex; i++)
                {
                    if(param.UseSplit==false)
                    {
                        logName = $"CompData_{refLNCD}_{listLNCD[i]}";
                        int nStep = param.CompRange.Count + 1; // 비교 거리 데이터 확인용
                        for (int j = 0; j < nStep; j++)
                        {
                            if (j == 0)
                            {
                                CompRange range = param.BasicRange;
                                log.WriteLoadData(subPath, $"[COMPARE BASIC]-[{range.MinXRange},{range.MinYRange}]~[{range.MaxXRange},{range.MaxYRange}]", j, logName, 0.0, true);
                            }
                            else
                            {
                                CompRange range = param.CompRange[j - 1];
                                log.WriteLoadData(subPath, $"[COMPARE Range {j}] - [{range.MinXRange},{range.MinYRange}]~[{range.MaxXRange},{range.MaxYRange}]", j, logName, 0.0);
                            }

                            idx1 = 0;
                            foreach (var item in lot.MarkCompList.Data)
                            {
                                if (!item.Comp1.Any(kv => kv.Key.Item1 == listLNCD[i]
                                                    && kv.Value[j].Count > 0)) continue;

                                var itemList = item.Comp1.
                                        Where(kv => kv.Key.Item1 == listLNCD[i]).ToList();

                                idx2 = 0;
                                string msg = String.Format($"{idx1},{idx2}\t-\t{item.Base.CTLNO}, {item.Base.FLTNO}, {item.Base.OFFSET:0.00}, {item.Base.YPOS_M:0.00}, {item.Base.XPOS_M:0.00}, " +
                                                            $"{item.Base.FAULTID}, {item.Base.SIZE:0.00}, {item.Base.CAM_NO}, {item.Base.MNTTID}, {item.Base.BCNO}");
                                log.WriteLoadData(subPath, msg, idx1, logName, 0.0);
                                idx2++;

                                foreach (var kv in itemList)
                                {
                                    foreach (var datum in kv.Value[j])
                                    {
                                        msg = String.Format($"{idx1},{idx2}\t-\t{datum.CTLNO}, {datum.FLTNO}, {datum.OFFSET:0.00}, {datum.YPOS_M:0.00}, {datum.XPOS_M:0.00}, " +
                                                                $"{datum.FAULTID}, {datum.SIZE:0.00}, {datum.CAM_NO}, {datum.MNTTID}, {datum.BCNO}");
                                        log.WriteLoadData(subPath, msg, idx1, logName, 0.0);
                                        idx2++;
                                    }
                                }
                                idx1++;
                            }
                        }
                    }
                    else
                    {
                        if (lot.MarkCompList.Data.Count != 0)
                        {
                            bool exists = lot.MarkCompList.Data[0].Comp1.Keys.Any(k => k.Item1 == listLNCD[i]);
                            if (exists == true)
                            {
                                foreach (var aaa in lot.MarkCompList.Data[0].Comp1)
                                {
                                    if (aaa.Key.Item1 == listLNCD[i])
                                    {
                                        logName = $"CompData_{refLNCD}_{listLNCD[i]}_{aaa.Key.Item2}";
                                        int nStep1 = lot.MarkCompList.Data[0].Comp1[aaa.Key].GetLength(0); // 비교 거리 데이터 확인용

                                        for (int j = 0; j < nStep1; j++)
                                        {
                                            if (j == 0)
                                            {
                                                CompRange range = param.BasicRange;
                                                log.WriteLoadData(subPath, $"[COMPARE BASIC]-[{range.MinXRange},{range.MinYRange}]~[{range.MaxXRange},{range.MaxYRange}]", j, logName, 0.0, true);
                                            }
                                            else
                                            {
                                                CompRange range = param.CompRange[j - 1];
                                                log.WriteLoadData(subPath, $"[COMPARE Range {j}] - [{range.MinXRange},{range.MinYRange}]~[{range.MaxXRange},{range.MaxYRange}]", j, logName, 0.0);
                                            }

                                            idx1 = 0;
                                            foreach (var item1 in lot.MarkCompList.Data)
                                            {
                                                if (item1.Comp1[aaa.Key][j].Count > 0)
                                                {
                                                    idx2 = 0;
                                                    string msg = String.Format($"{idx1},{idx2}\t-\t{item1.Base.CTLNO}, {item1.Base.FLTNO}, {item1.Base.OFFSET:0.00}, {item1.Base.YPOS_M:0.00}, {item1.Base.XPOS_M:0.00}, " +
                                                                                $"{item1.Base.FAULTID}, {item1.Base.SIZE:0.00}, {item1.Base.CAM_NO}, {item1.Base.MNTTID}, {item1.Base.BCNO}");
                                                    log.WriteLoadData(subPath, msg, idx1, logName, 0.0);
                                                    idx2++;

                                                    for (int k = 0; k < item1.Comp1[aaa.Key][j].Count; k++)
                                                    {
                                                        MarkingFaultDatum datum = item1.Comp1[aaa.Key][j][k];
                                                        msg = String.Format($"{idx1},{idx2}\t-\t{datum.CTLNO}, {datum.FLTNO}, {datum.OFFSET:0.00}, {datum.YPOS_M:0.00}, {datum.XPOS_M:0.00}, " +
                                                                                $"{datum.FAULTID}, {datum.SIZE:0.00}, {datum.CAM_NO}, {datum.MNTTID}, {datum.BCNO}");
                                                        log.WriteLoadData(subPath, msg, idx1, logName, 0.0);
                                                        idx2++;
                                                    }
                                                    idx1++;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                LotManager.Selected.AddLot("DB", lot);

                Thread.Sleep(200);
            }
            catch (Exception ex)
            {

            }
            finally
            {

            }
        }

        #endregion

        public void dbReconnect()
        {
            _DbConn?.Dispose();

            _DbConn = new OracleDbConnection();
        }

    }
}
