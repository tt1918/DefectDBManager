//#define TEST_MODE

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
using System.Security.Policy;
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
    public delegate void DelegateSjMonitorEvent(SjMonitorData data);

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

        public event DelegateSjMonitorEvent OnSjMonitorEvent = null;
        #endregion Event

        #region Comp Data 저장용 구조체
        private struct CompareTarget
        {
            public string LNCD;
            public bool IsSplitCTLNO;

            public CompareTarget(string lncd, bool isSplitCTLNO)
            {
                LNCD = lncd;
                IsSplitCTLNO = isSplitCTLNO;
            }
        }
        #endregion

        #region Param
        /// <summary>
        /// DB Query 및 탐색
        /// </summary>
        public ICompareDefectError _CompUserFD;
        public ICompareDefectError _CompDBFD;

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

            _CompUserFD = new CompUserFilterDefect(this, _DbConn);
            _CompUserFD.DbDestConfig = _DestConfig;
            _CompUserFD.DBCodeConfig = _CodeConfig;
            _CompUserFD.DbOption = _Option;

            _CompDBFD = new CompDBFilterDefect(this, _DbConn);
            _CompDBFD.DbDestConfig = _DestConfig;
            _CompDBFD.DBCodeConfig = _CodeConfig;
            _CompDBFD.DbOption = _Option;

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
            initSjModeMonitorTimer();
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
                    AiMonitorItem aiMonitorItem = null;

                    for (int i = 0; i < LotManager.ProcSetting.Count; i++)
                        if (LotManager.ProcSetting[i].Name == keyData[2]) { preprocItem = LotManager.ProcSetting[i]; break; }

                    for (int i = 0; i < LotManager.AiMonitorParam.ModeItems.Count; i++)
                    {
                        if (LotManager.AiMonitorParam.ModeItems[i].LNCD == preprocItem.Reference.LNCD &&
                            LotManager.AiMonitorParam.ModeItems[i].ModelName == keyData[1])
                            aiMonitorItem = LotManager.AiMonitorParam.ModeItems[i];
                    }

                    ProcFilter filter = LotManager.CrtProcFilter[(int)eProc.Live][productIdx];

                    (_CompUserFD as CompUserFilterDefect)?.SetFilterParam(lncd, keyData[1], preprocItem, aiMonitorItem);

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

            CompUserFilterDefect comp = _CompUserFD as CompUserFilterDefect;

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

                // 데이터 설정 처리
                comp.SetListSearchingParam(lncd, data, stTime, edTime, LotManager.Live.LotHistory, LogDB.eDataType.Realtime);
#if TEST_MODE
                if (comp.SearchPTRYOPList_TEST() == true)
#else
                if (comp.SearchPTRYOPList() == true)
#endif
                {
                    PTRY0PList list = new PTRY0PList();
                    foreach (var ptry0p in comp.PTRY0PList_Data.Data)
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
                CompUserFilterDefect comp = _CompUserFD as CompUserFilterDefect;
                comp.SetLotSearchingParam(lotName, false, false, LogDB.eDataType.Realtime);
#if TEST_MODE
                IPreprocLot lot = comp.SearchLot_TEST(ref error);
#else
                IPreprocLot lot = comp.SearchLot(ref error);
#endif
                if (lot == null) return;

                // 입력 받은 데이터 기준으로 좌표 비교
                ((PreprocLotFilter)lot).ProcData = preprocItem;
                lot.ComparePosition();

                string subPath = comp._SubPath;

                LogDB log = comp._LOG;
                var targets = preprocItem.Compare
                    .Select(x => new CompareTarget(x.LNCD, x.IsSplitCTLNO))
                    .ToList();

                var compRangeLogs = preprocItem.CompRange
                    .Select(x => x.LogString())
                    .ToList();

                WriteCompareDataLog(
                    lot,
                    comp._LOG,
                    comp._SubPath,
                    preprocItem.Reference.LNCD,
                    targets,
                    preprocItem.BasicRange.LogString(),
                    compRangeLogs);

                // Ai Monitoring Result
                logAIMonitorResult(subPath, log, lot);
                
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

            // SJ MODE DATA 모니터링 시작
            runSjModeData();
            _timerSjModeMonitor.Interval = LotManager.ProcSetting.SJModeCycleTime * 1000*60;
            _timerSjModeMonitor.Start();
        }
        public void StopLiveLot()
        {
            _timerCheckLiveLot.Stop();
            _timerSjModeMonitor.Stop();
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
                    AiMonitorItem aiMonitorItem = null;

                    for (int i = 0; i < LotManager.ProcSetting.Count; i++)
                        if (LotManager.ProcSetting[i].Name == keyData[2]) preprocItem = LotManager.ProcSetting[i];

                    for (int i = 0; i < LotManager.AiMonitorParam.ModeItems.Count; i++)
                    {
                        if (LotManager.AiMonitorParam.ModeItems[i].LNCD == preprocItem.Reference.LNCD &&
                            LotManager.AiMonitorParam.ModeItems[i].ModelName == keyData[1])
                            aiMonitorItem = LotManager.AiMonitorParam.ModeItems[i];
                    }

                    (_CompUserFD as CompUserFilterDefect)?.SetFilterParam(lncd, keyData[1], preprocItem, aiMonitorItem);

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

            CompUserFilterDefect comp = _CompUserFD as CompUserFilterDefect;

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

                // 데이터 설정 처리
                comp.SetListSearchingParam(lncd, data, stTime, edTime, null, LogDB.eDataType.SearchLot);
#if TEST_MODE
                if (comp.SearchPTRYOPList_TEST() == true)
#else
                if (comp.SearchPTRYOPList() == true)
#endif
                {
                    foreach (var ptry0p in comp.PTRY0PList_Data.Data)
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
                CompUserFilterDefect comp = _CompUserFD as CompUserFilterDefect;
                comp.SetLotSearchingParam(lotName, true, false, LogDB.eDataType.SearchLot);
#if TEST_MODE
                IPreprocLot lot = comp.SearchLot_TEST(ref error);
#else
                IPreprocLot lot = comp.SearchLot(ref error);
#endif

                if (lot == null) return;

                // 입력 받은 데이터 기준으로 좌표 비교
                ((PreprocLotFilter)lot).ProcData = preprocItem;
                lot.ComparePosition();

                string subPath = comp._SubPath;

                LogDB log = comp._LOG;

                var targets = preprocItem.Compare
                    .Select(x => new CompareTarget(x.LNCD, x.IsSplitCTLNO))
                    .ToList();

                var compRangeLogs = preprocItem.CompRange
                    .Select(x => x.LogString())
                    .ToList();

                WriteCompareDataLog(
                    lot,
                    comp._LOG,
                    comp._SubPath,
                    preprocItem.Reference.LNCD,
                    targets,
                    preprocItem.BasicRange.LogString(),
                    compRangeLogs);

                // Ai Monitoring Result
                logAIMonitorResult(subPath, log, lot);

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
                    AiMonitorItem aiMonitorItem = null;

                    for (int i = 0; i < LotManager.ProcSetting.Count; i++)
                    {
                        if (LotManager.ProcSetting[i].Name == keyData[2])
                        {
                            preprocItem = LotManager.ProcSetting[i].Clone();
                            // 선택랏 설정에 대하여 따로 설정되는 파라미터 적용
                            preprocItem.UseAiMonitoring = _selParam.UseAiMonitoring;
                            preprocItem.UseAiResult = _selParam.UseAiResult;
                        }
                    }

                    for(int i=0; i< LotManager.AiMonitorParam.ModeItems.Count; i++)
                    {
                        if (LotManager.AiMonitorParam.ModeItems[i].LNCD == preprocItem.Reference.LNCD &&
                            LotManager.AiMonitorParam.ModeItems[i].ModelName == keyData[1])
                            aiMonitorItem = LotManager.AiMonitorParam.ModeItems[i];
                    }
                    
                    (_CompUserFD as CompUserFilterDefect)?.SetFilterParam(lncd, keyData[1], preprocItem, aiMonitorItem);

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
                    AiMonitorItem aiMonitorItem = null;

                    string lotName = item.Y0KLOT;

                    aiMonitorItem = LotManager.AiMonitorParam.ModeItems.FirstOrDefault(x=>x.Name==_selParam.DBFilterAiMonitorName);

                    (_CompDBFD as CompDBFilterDefect)?.SetDBParam(filter.Title, aiMonitorItem);

                    SearchSelectedDBLotDefect(lotName, _selParam, aiMonitorItem);

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
                CompUserFilterDefect comp = _CompUserFD as CompUserFilterDefect;
                comp.SetLotSearchingParam(lotName, true, false, LogDB.eDataType.SelectedLot);
#if TEST_MODE
                IPreprocLot lot = comp.SearchLot_TEST(ref error);
#else
                IPreprocLot lot = comp.SearchLot(ref error);
#endif

                if (lot == null) return;

                // 입력 받은 데이터 기준으로 좌표 비교
                ((PreprocLotFilter)lot).ProcData = preprocItem;
                lot.ComparePosition();

                string subPath = comp._SubPath;

                LogDB log = comp._LOG;

                var targets = preprocItem.Compare
                    .Select(x => new CompareTarget(x.LNCD, x.IsSplitCTLNO))
                    .ToList();

                var compRangeLogs = preprocItem.CompRange
                    .Select(x => x.LogString())
                    .ToList();

                WriteCompareDataLog(
                    lot,
                    comp._LOG,
                    comp._SubPath,
                    preprocItem.Reference.LNCD,
                    targets,
                    preprocItem.BasicRange.LogString(),
                    compRangeLogs);

                // Ai Monitoring Result
                logAIMonitorResult(subPath, log, lot);

                LotManager.Selected.AddLot(lncd, lot);

                Thread.Sleep(200);
            }
            catch (Exception e)
            {
                Log.Write($"[Error] SearchDefectData : {e.Message}");
            }
        }

        public void SearchSelectedDBLotDefect(string lotName, LotSelProcParam param, AiMonitorItem aiMonitorItem)
        {
            eSearchError error = eSearchError.Normal;
            try
            {
                CompDBFilterDefect comp = _CompDBFD as CompDBFilterDefect;
                comp.SetLotSearchingParam(lotName, param, aiMonitorItem, false);
#if TEST_MODE
                IPreprocLot lot = comp.SearchLot_TEST(ref error);
#else
                IPreprocLot lot = comp.SearchLot(ref error);
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

                string subPath = comp._SubPath;

                LogDB log = comp._LOG;
                var targets = listLNCD
                    .Select(x => new CompareTarget(x, param.UseSplit))
                    .ToList();

                var compRangeLogs = param.CompRange
                    .Select(x => x.LogString())
                    .ToList();

                WriteCompareDataLog(
                    lot,
                    comp._LOG,
                    comp._SubPath,
                    refLNCD,
                    targets,
                    param.BasicRange.LogString(),
                    compRangeLogs);

                // Ai Monitoring Result
                logAIMonitorResult(subPath, log, lot);

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

        #region Log 저장 함수
        private void WriteCompareDataLog(IPreprocLot lot, LogDB log, string subPath, string refLncd, IList<CompareTarget> targets,
                                        string basicRangeLog, IList<string> compRangeLogs)
        {
            if (lot == null || log == null || targets == null) return;

            foreach (var target in targets)
            {
                if (!target.IsSplitCTLNO)
                {
                    string logName = $"CompData_{refLncd}_{target.LNCD}";
                    int nStep = compRangeLogs.Count + 1;

                    for (int j = 0; j < nStep; j++)
                    {
                        if (j == 0) log.WriteLoadData(subPath, $"[COMPARE BASIC]-{basicRangeLog}", j, logName, 0.0, true);
                        else log.WriteLoadData(subPath, $"[COMPARE Range {j}] - {compRangeLogs[j - 1]}", j, logName, 0.0);

                        int idx1 = 0;
                        foreach (var item in lot.MarkCompList.Data)
                        {
                            if (!item.Comp.Any(kv => kv.Key.Item1 == target.LNCD && kv.Value[j].Count > 0)) continue;

                            var itemList = item.Comp.Where(kv => kv.Key.Item1 == target.LNCD).ToList();
                            int idx2 = 0;

                            log.WriteLoadData(subPath, $"{idx1},{idx2}\t-\t{item.Base.LogString()}", idx1, logName, 0.0);
                            idx2++;

                            foreach (var kv in itemList)
                            {
                                foreach (var datum in kv.Value[j])
                                {
                                    log.WriteLoadData(subPath, $"{idx1},{idx2}\t-\t{datum.LogString()}", idx1, logName, 0.0);
                                    idx2++;
                                }
                            }

                            idx1++;
                        }
                    }
                }
                else
                {
                    if (lot.MarkCompList.Data.Count == 0) continue;

                    bool exists = lot.MarkCompList.Data[0].Comp.Keys.Any(k => k.Item1 == target.LNCD);
                    if (!exists) continue;

                    foreach (var compItem in lot.MarkCompList.Data[0].Comp)
                    {
                        if (compItem.Key.Item1 != target.LNCD) continue;

                        string logName = $"CompData_{refLncd}_{target.LNCD}_{compItem.Key.Item2}";
                        int nStep = lot.MarkCompList.Data[0].Comp[compItem.Key].GetLength(0);

                        for (int j = 0; j < nStep; j++)
                        {
                            if (j == 0) log.WriteLoadData(subPath, $"[COMPARE BASIC]-{basicRangeLog}", j, logName, 0.0, true);
                            else if (j - 1 < compRangeLogs.Count)
                                log.WriteLoadData(subPath, $"[COMPARE Range {j}] - {compRangeLogs[j - 1]}", j, logName, 0.0);

                            int idx1 = 0;
                            foreach (var item1 in lot.MarkCompList.Data)
                            {
                                if (item1.Comp[compItem.Key][j].Count <= 0) continue;

                                log.WriteLoadData(subPath, $"{idx1},0\t-\t{item1.Base.LogString()}", idx1, logName, 0.0);
                                for (int k = 0, id = 1; k < item1.Comp[compItem.Key][j].Count; k++, id++)
                                {
                                    log.WriteLoadData(subPath, $"{idx1},{id}\t-\t{item1.Comp[compItem.Key][j][k].LogString()}", idx1, logName, 0.0);
                                }
                                idx1++;
                            }
                        }
                    }
                }
            }
        }

        private void logAIMonitorResult(string subPath, LogDB log, IPreprocLot lot)
        {
            if (lot.FaultData.AIMonResult == null) return;
            string logName = $"Ai_Monitoring";

            int idx = 0;
            AIMonitorResult aiRes = lot.FaultData.AIMonResult;
            log.WriteLoadData(subPath, $"Model Exist: {aiRes.IsModelExsit}", idx, logName, 0.0, true);
            foreach (var item in lot.FaultData.AIMonResult.Items)
            {
                log.WriteLoadData(subPath, $"[{item.Model.SECFLTID} - {string.Join(",", item.Model.FLTID)}] Judgement Rate : {item.Model.Rate}", ++idx, logName, 0.0);

                string logString = $"Total : {item.Total}";
                log.WriteLoadData(subPath, logString, ++idx, logName, 0.0);
                logString = $"Match : {item.Match}";
                log.WriteLoadData(subPath, logString, ++idx, logName, 0.0);
                if (item.Model.Use)
                {
                    logString = $"Judgement : {item.Judge}";
                    log.WriteLoadData(subPath, logString, ++idx, logName, 0.0);
                }
                else
                {
                    string strExist = item.Total > 0 ? "OK" : "NG";
                    logString = $"Defect Exsit : {strExist}";
                    log.WriteLoadData(subPath, logString, ++idx, logName, 0.0);
                }

            }
        }
        #endregion


        #region SJMode 실시간 모니터링 처리
        System.Timers.Timer _timerSjModeMonitor = null;

        bool _isRunSjModeMonitor = false;
        bool _stopSjModeMonitor = false;

        private void initSjModeMonitorTimer()
        {
            closeSjModeMonitorTimer();
            _timerSjModeMonitor = new System.Timers.Timer();
            _timerSjModeMonitor.Interval = 1000*60;
            _timerSjModeMonitor.Elapsed += checkSjModeMonitor;
        }

        private void closeSjModeMonitorTimer()
        {
            if(_timerSjModeMonitor!=null)
            {
                _timerSjModeMonitor.Stop();
                _timerSjModeMonitor.Elapsed -= checkSjModeMonitor;
                _timerSjModeMonitor.Dispose();
                _timerSjModeMonitor = null;
            }
        }

        private void checkSjModeMonitor(object sender, ElapsedEventArgs e)
        {
            // 검색 중이면 스킵 처리
            if (_isRunSjModeMonitor == true) return;

            // 탐색 가능 확인

            runSjModeData();
        }

        private void runSjModeData()
        {
            Task task = new Task(searchSjModeData);
            task.Start();
        }

        private void searchSjModeData()
        {
            try
            {
                if (_isRunSjModeMonitor == true) return;
                _isRunSjModeMonitor = true;

                var param = LotManager.SjMonitorParam;
                foreach (var item in param.ModeItems)
                {
                    string mainPath = item.MainPath;

                    // 내부에 있는 폴더를 확인하고, 해당 폴더에 대한 데이터를 취합한다.
                    var subFolderList = Directory.GetDirectories(mainPath);
                    foreach (var subFolder in subFolderList)
                    {
                        string subFolderName = Path.GetFileName(subFolder);
                        string ctlno = subFolderName;

                        // 해당 ctlno에 대한 데이터가 이미 존재하면 스킵
                        if (LotManager.SjMonitorDataList.Exist(ctlno) == true &&
                            LotManager.SjMonitorDataList.DataList.First(x => x.CTLNO == ctlno).IsFinished == true)
                            continue;

                        string path = item.NetPathSummery(ctlno);

                        // 해당 경로에 있는 파일을 확인하고, 데이터를 취합한다.
                        int key = Convert.ToInt32(item.ModelName);
                        var data = ReadSjData(ctlno, path);
                        data.ModeNo = key;

                        // 판정 처리함
                        foreach (var infoItem in item.DefectInfo)
                        {
                            if(infoItem.Use==false) continue;
                            if (!data.DefectInfo.ContainsKey(key)) continue;
                            var rate = data.SetDefectJudgement(key, infoItem);
                        }

                        data.LNCD = item.LNCD;

                        if (LotManager.SjMonitorDataList.Exist(ctlno) == false)
                            LotManager.SjMonitorDataList.Add(data);
                        else
                        {
                            LotManager.SjMonitorDataList.Remove(ctlno);
                            LotManager.SjMonitorDataList.Add(data);
                        }

                        // 데이터가 존재하면 리스트에 추가
                        OnSjMonitorEvent?.Invoke(data);

                        // 검색 결과 데이터를 파일에 저장한다.
                        _ = Task.Run(() => saveSjMonitorData(data));
                    }
                }
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] searchSjModeData : {ex.Message}");
            }
            finally
            {
                _isRunSjModeMonitor = false;
            }
        }

        private SjMonitorData ReadSjData(string ctrno, string path)
        {
            SjMonitorData data = new SjMonitorData();
            data.CTLNO = ctrno;
            data.IsFinished = false;

            Dictionary<int, int> modeNo = new Dictionary<int, int>();
            Dictionary<string, string> tmpCtrlNo = new Dictionary<string, string>();
            // 해당 경로에 있는 파일을 확인하고, 데이터를 취합한다.
            var filePath = Path.Combine(path, "DecisionResult.csv");
            try
            {
                // 파일이 존재하지 않으면 없음을 표시함
                if (System.IO.File.Exists(filePath) == false)            
                    return data;

                // 파일이 존재하면 완료 플레그는 true로 설정함
                data.IsFinished = true;
                // 파일이 존재하면 데이터를 읽어옴
                var lines = System.IO.File.ReadAllLines(filePath);
                foreach (var line in lines)
                {
                    if(line.Contains("SJMODE") ||
                        line.Contains("CTLNO") || 
                        line.Contains("FLTNO") ||
                        line.Contains("PICFNAME") || 
                        line.Contains("FLTID") ||
                        line.Contains("SECFLTID"))
                        continue;

                    string[] items = line.Split(',');
                    if (items.Length <= 0) continue;

                    SjData item = new SjData
                    {
                        FltNo = int.Parse(items[2]),
                        PicName = items[3],
                        FLTID = items[4],
                        SECFLTID = items[5]
                    };
                    int no = int.Parse(items[0]);
                    data.AddDefect(no, item);

                    if (!modeNo.ContainsKey(no)) 
                        modeNo.Add(no, no);

                    
                    if(!tmpCtrlNo.ContainsKey(items[1]))
                        tmpCtrlNo.Add(items[1], items[1]);
                }

               

                if (tmpCtrlNo.Count==1)
                {
                    data.CTLNO = tmpCtrlNo.First().Key;
                }
                else if(tmpCtrlNo.Count>1)
                {
                    Log.Write($"[Error] ReadSjData : {ctrno} - CTLNO Count is more than 1");
                    data.CTLNO = tmpCtrlNo.First().Key;
                }

                return data;
            }
            catch(Exception ex)
            {
                Log.Write($"[Error] ReadSjData : {ex.Message}");
                return null;
            }
        }

        private void saveSjMonitorData(SjMonitorData data)
        {
            string path = Path.Combine(Define.SjMonitorDataFolder, $"{data.CTLNO}.csv");
            if (Directory.Exists(Define.SjMonitorDataFolder) == false)
                Directory.CreateDirectory(Define.SjMonitorDataFolder);
            
            using (StreamWriter sw = new StreamWriter(path))
            {
                // Summery 정보 저장
                sw.WriteLine("CTLNO,MODE,Finish");
                sw.WriteLine($"{data.CTLNO},{data.ModeNo},{data.IsFinished}");

                StringBuilder header = new StringBuilder();
                StringBuilder dataLine = new StringBuilder();
                header.Append("FLTID,Total,Converted,None Convert Rate,Judgement");
                sw.WriteLine(header.ToString());
                foreach (var item in data.Judgement)
                {
                    dataLine.Clear();
                    string fltids = string.Join(";", item.Value.FLTID);
                    dataLine.Append(fltids);
                    dataLine.Append($",{item.Value.Total}");
                    dataLine.Append($",{item.Value.Converted}");
                    dataLine.Append($",{item.Value.NoneConvertRate*100.0}");
                    dataLine.Append($",{item.Value.Judgement}");
                    sw.WriteLine(dataLine.ToString());
                }

                sw.WriteLine("SJMODE,CTLNO,FLTNO,PICFNAME,FLTID,SECFLTID");
                foreach (var item in data.DefectInfo)
                {
                    foreach (var defect in item.Value)
                    {
                        sw.WriteLine($"{item.Key},{data.CTLNO},{defect.FltNo},{defect.PicName},{defect.FLTID},{defect.SECFLTID}");
                    }
                }
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
