//#define TEST_MODE

using DefectDBManager.DB;
using DefectDBManager.Preproc;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Security;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;
using System.Windows.Forms;

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

        // 프로세스 상에 발생하는 이벤트 보고용
        public event DelegateProcessEvent OnProcessEvent = null;
         // 랏 검색 진행 상황을 상위로 보고
        public event DelegateLotProgress OnLotProgress = null;
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
        public bool IsRunLiveSearch
        {
            get; private set;
        } = false;
        public bool StopLiveSearch
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

        public void SearchLiveMarkDiff()
        {
            Task task = new Task(searchLiveDefect, null);
            task.Start();
        }

        public void SearchLotMarkDiff()
        {
            // LotManager의 데이터는 업데이트되어있는 상황
            Task task = new Task(searchDefect, null);
            task.Start();
        }

        private void searchLiveDefect(object obj)
        {
            if (IsRunLiveSearch == true) return;
            IsRunLiveSearch = true;

            // 동시에 Live와 Search가 구동되지 못 하도록 함.
            while(IsRunSearchingLotList)
            {
                Thread.Sleep(1000);
            }

            // 해당 공정에 대한 결점 정보 확인
            searchLiveLotList();

            int productIdx = 0;
            foreach (var list in LotManager.LiveProduct)
            {
                string[] keyData = list.Key.Split('_');
                string lncd = keyData[0];
                foreach (var item in list.Value.Data)
                {
                    if (StopLiveSearch == true) break;

                    string lotName = item.Y0KLOT;
                    PreprocItem preprocItem = null;
                    for (int i = 0; i < LotManager.ProcSetting.Count; i++)
                        if (LotManager.ProcSetting[i].Name == keyData[2]) preprocItem = LotManager.ProcSetting[i];

                    ProcFilter filter = LotManager.CrtProcFilter[(int)eProc.Search][productIdx];

                    _DBProc.SetFilterParam(lncd, keyData[1], preprocItem);

                    SearchLiveDefectData(lncd, lotName, preprocItem, filter);

                    // 검색 진행 상황을 
                    int rate = (int)((float)LotManager.TotalLiveLot / (float)LotManager.TotalLiveProduct);
                    OnLotProgress?.Invoke(rate);
                }

                productIdx++;
            }

            StopLiveSearch = false;
            IsRunLiveSearch = false;

            // 완료 보고
            OnEndLiveSearchLot?.Invoke();
        }

        private void searchDefect(object obj)
        {
            if (IsRunSearchingLotList == true) return;
            IsRunSearchingLotList = true;

            // 동시에 Live와 Search가 구동되지 못 하도록 함.
            while (IsRunLiveSearch)
            {
                Thread.Sleep(1000);
            }

            // 해당 공정에 대한 결점 정보 확인
            searchLotList();

            LotManager.ClearLot();
            int productIdx = 0;
            foreach(var list in LotManager.Product)
            {
                string[] keyData = list.Key.Split('_');
                string lncd = keyData[0];

                ProcFilter filter = LotManager.CrtProcFilter[(int)eProc.Search][productIdx];

                PreprocItem preprocItem = null;
                for (int i = 0; i < LotManager.ProcSetting.Count; i++)
                    if (LotManager.ProcSetting[i].Name == keyData[2]) preprocItem = LotManager.ProcSetting[i];

                _DBProc.SetFilterParam(lncd, keyData[1], preprocItem);

                foreach (var item in list.Value.Data)
                {
                    if (StopSearchingLotList == true) break;

                    string lotName = item.Y0KLOT;
                    //filter 로 구분하도록 수정 @ATW 250321
                    //SearchDefectData(lncd, lotName, preprocItem);
                    SearchDefectData(list.Key, lotName, preprocItem, filter);

                    // 검색 진행 상황을 
                    int rate = (int)((float)LotManager.TotalLot / (float)LotManager.TotalProduct);
                    OnLotProgress?.Invoke(rate);
                }

                productIdx++;
            }
            
            StopSearchingLotList = false;
            IsRunSearchingLotList = false;

            // 완료 보고
            OnEndSearchingLotList?.Invoke();
        }

        #region 실시간 공정 별 생산 리스트 취합
        private void searchLiveLotList()
        {
            if (LotManager.LiveProduct != null)
                LotManager.LiveProduct.Clear();
            // 금일 날자로 설정
            DateTime stTime = DateTime.Today;
            DateTime edTime = DateTime.Now;

            ProcFilterList filter = LotManager.CrtProcFilter[(int)eProc.Live];
            string lncd = string.Empty;
            foreach (var data in filter.Data)
            {
                // 검색 대상이 아니면 처리하지 않음.
                if (data.IsInTime() == false) continue;

                lncd = string.Empty;
                for (int i=0; i< LotManager.ProcLNCD.Info.Count; i++)
                {
                    if (LotManager.ProcLNCD.Info[i].Name == data.Line)
                    {
                        lncd = LotManager.ProcLNCD.Info[i].LNCD;
                        break;
                    }    
                }
#if TEST_MODE
                if (_DBProc.SearchPTRYOPList_TEST(lncd, data, stTime, edTime) == true)
#else
                if (_DBProc.SearchPTRYOPList(lncd, data, stTime, edTime) == true)
#endif
                {
                    PTRY0PList list = new PTRY0PList();

                    foreach (var ptry0p in _DBProc.PTRY0PList_Data.Data)
                        list.Add(ptry0p.Clone());

                    // 리스트 데이터 추가
                    LotManager.LiveProduct.Add(data.ToString(), list);
                }

                data.ResetTime();
            }
        }
#endregion

        #region 기간 공정 별 생산 리스트 취합.
        private void searchLotList()
        {
            LotManager.Product.Clear();

            DateTime stTime = LotManager.SearchTime.StartTime;
            DateTime edTime = LotManager.SearchTime.EndTime;

            ProcFilterList filter = LotManager.CrtProcFilter[(int)eProc.Search];
            string lncd = string.Empty;

            foreach (var data in filter.Data)
            {
                lncd = string.Empty;
                for (int i = 0; i < LotManager.ProcLNCD.Info.Count; i++)
                {
                    if (LotManager.ProcLNCD.Info[i].Name == data.Line)
                    {
                        lncd = LotManager.ProcLNCD.Info[i].LNCD;
                        break;
                    }
                }
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
                if (_DBProc.SearchPTRYOPList_TEST(lncd, data, stTime, edTime) == true)
#else
                if (_DBProc.SearchPTRYOPList(lncd, data, stTime, edTime) == true)
#endif
                {
                    foreach (var ptry0p in _DBProc.PTRY0PList_Data.Data)
                    {
                        if (/*isWildCard == true && */ptry0p.Y0ZKNM.Contains(productName) == false) continue;
                        //else if (isWildCard == false && ptry0p.Y0ZKNM != productName) continue;
                        
                        list.Add(ptry0p.Clone());
                    }
                }

                // 리스트 데이터 추가
                LotManager.Product.Add(data.ToString(), list);
            }
        }
#endregion

#region 결점 데이터 검색
        public void SearchLiveDefectData(string lncd, string lotName, PreprocItem preprocItem, ProcFilter filter)
        {
            int error = -1;
            bool usemkcdModel = LotManager.UseMrkctlmstModel;
            try
            {
#if TEST_MODE
                PreprocLot lot = _DBProc.SearchLot_TEST(lotName, false, false, ref error);
#else
                PreprocLot lot = _DBProc.SearchLot(lotName, false, false, ref error);
#endif
                if (lot == null) return;

                // 입력 받은 데이터 기준으로 좌표 비교
                lot.ComparePosition(preprocItem);

                string logName = $"CompData";
                string subPath = _DBProc._SubPath;

                LogDB log = _DBProc._LOG;
                int idx1 = 0, idx2 = 0;

                int maxStep = 0;
                if (lot.MarkCompList.Data.Count > 0)
                    maxStep = lot.MarkCompList.Data[0].Comp.GetLength(1);

                // 이제 비교가 된 데이터에 대해서만 정보를 저장한다. 
                for (int i = 0; i < maxStep; i++)
                {
                    logName = $"CompData_{preprocItem.Compare[i].LineID}";
                    int nStep = lot.MarkCompList.Data[0].Comp.GetLength(0); // 비교 거리 데이터 확인용
                    for(int j=0; j<nStep; j++)
                    {
                        if (j == 0)
                        {
                            CompRange range = preprocItem.BasicRange;
                            log.WriteLoadData(subPath, $"[COMPARE BASIC]-[{range.MinXRange},{range.MinYRange}]~[{range.MaxXRange},{range.MaxYRange}]", j, logName, 0.0, true);
                        }
                        else
                        {
                            CompRange range = preprocItem.CompRange[j-1];
                            log.WriteLoadData(subPath, $"[COMPARE Range {j}] - [{range.MinXRange},{range.MinYRange}]~[{range.MaxXRange},{range.MaxYRange}]", j, logName, 0.0);
                        }
                        idx1 = 0;
                        foreach (var item in lot.MarkCompList.Data)
                        {
                            if (item.Comp[i, j].Count>0)
                            {
                                idx2 = 0;
                                for (int k = 0; k < item.Comp[i, j].Count; k++)
                                {
                                    MarkingFaultDatum datum = item.Comp[i, j][k];
                                    string msg = String.Format($"{idx1},{idx2}\t-\t{datum.CTLNO}, {datum.FLTNO}, {datum.OFFSET:0.00}, {datum.YPOS_M:0.00}, {datum.XPOS_M:0.00}, " +
                                                            $"{datum.FAULTID}, {datum.SIZE:0.00}, {datum.CAM_NO}, {datum.FAULTID}, {datum.BCNO}");
                                    log.WriteLoadData(subPath, msg, idx1, logName, 0.0);
                                    idx2++;
                                }
                            }
                        }
                    }
                }
                
                LotManager.AddLiveLot(lncd, lot);
            }
            catch
            {

            }
        }

        public void SearchDefectData(string lncd, string lotName , PreprocItem preprocItem, ProcFilter filter)
        {
            int error = -1;
            try
            {
#if TEST_MODE
                PreprocLot lot = _DBProc.SearchLot_TEST(lotName, false, false, ref error);
#else
                PreprocLot lot = _DBProc.SearchLot(lotName, true, false, ref error);
#endif

                if (lot == null) return;

                string logName = $"CompData";

                // 입력 받은 데이터 기준으로 좌표 비교
                lot.ComparePosition(preprocItem);

                LogDB log = _DBProc._LOG;
                string subPath = _DBProc._SubPath;
                int idx1 = 0, idx2 = 0;

                int maxStep = 0;
                if(lot.MarkCompList.Data.Count>0)
                    maxStep = lot.MarkCompList.Data[0].Comp.GetLength(1);

                log.WriteLoadData(subPath, "COMPARE BASIC", idx1, logName, 0.0, true);

                foreach (var item in lot.MarkCompList.Data)
                {
                    idx2 = 0;
                    string msg = String.Format($"{idx1},{idx2}\t-\t{item.Base.CTLNO}, {item.Base.FLTNO}, {item.Base.OFFSET:0.00}, {item.Base.YPOS_M:0.00}, {item.Base.XPOS_M:0.00}, " +
                                                        $"{item.Base.FAULTID}, {item.Base.SIZE:0.00}, {item.Base.CAM_NO}, {item.Base.FAULTID}, {item.Base.BCNO}");
                    log.WriteLoadData(subPath, msg, idx1, logName, 0.0);
                    idx2++;
                    for (int i = 0; i < item.Comp.GetLength(0); i++)
                    {
                        if (item.Comp[i, 0].Count > 0)
                        {
                            for(int j=0; j< item.Comp[i, 0].Count; j++)
                            {
                                MarkingFaultDatum datum = item.Comp[i, 0][j];
                                msg = String.Format($"{idx1},{idx2}\t-\t{datum.CTLNO}, {datum.FLTNO}, {datum.OFFSET:0.00}, {datum.YPOS_M:0.00}, {datum.XPOS_M:0.00}, " +
                                                        $"{datum.FAULTID}, {datum.SIZE:0.00}, {datum.CAM_NO}, {datum.FAULTID}, {datum.BCNO}");
                                log.WriteLoadData(subPath, msg, idx1, logName, 0.0);
                                idx2++;
                            }
                        }
                    }
                    idx1++;
                }

                for(int idx = 1; idx< maxStep; idx++)
                {
                    idx1 = 0;
                    log.WriteLoadData(subPath, $"COMPARE Range {idx}", idx1, logName, 0.0);

                    foreach (var item in lot.MarkCompList.Data)
                    {
                        idx2 = 0;
                        string msg = String.Format($"{idx1},{idx2}\t-\t{item.Base.CTLNO}, {item.Base.FLTNO}, {item.Base.OFFSET:0.00}, {item.Base.YPOS_M:0.00}, {item.Base.XPOS_M:0.00}, " +
                                                            $"{item.Base.FAULTID}, {item.Base.SIZE:0.00}, {item.Base.CAM_NO}, {item.Base.FAULTID}, {item.Base.BCNO}");
                        log.WriteLoadData(subPath, msg, idx1, logName, 0.0);
                        idx2++;
                        for (int i = 0; i < item.Comp.GetLength(0); i++)
                        {
                            if (item.Comp[i, idx].Count > 0)
                            {
                                for (int j = 0; j < item.Comp[i, idx].Count; j++)
                                {
                                    MarkingFaultDatum datum = item.Comp[i, idx][j];
                                    msg = String.Format($"{idx1},{idx2}\t-\t{datum.CTLNO}, {datum.FLTNO}, {datum.OFFSET:0.00}, {datum.YPOS_M:0.00}, {datum.XPOS_M:0.00}, " +
                                                            $"{datum.FAULTID}, {datum.SIZE:0.00}, {datum.CAM_NO}, {datum.FAULTID}, {datum.BCNO}");
                                    log.WriteLoadData(subPath, msg, idx1, logName, 0.0);
                                    idx2++;
                                }
                            }
                        }
                        idx1++;
                    }
                }

                LotManager.AddLot(lncd, lot);
            }
            catch(Exception e)
            {
                Log.Write($"[Error] SearchDefectData : {e.Message}");
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
                if(_timerCheckLiveLot!=null)
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
            _timerCheckLiveLot.Start();
            int size = LotManager.CrtProcFilter[(int)eProc.Live].Count;
            bool isSync = LotManager.CrtProcFilter.UseLiveSync;
            int syncDuration = LotManager.CrtProcFilter.SyncDuration;
            
            // 전체 싱크 모드를 사용하면
            if(isSync==true)
            {
                for (int i = 0; i < size; i++)
                {
                    LotManager.CrtProcFilter[(int)eProc.Live].Data[i].SetTime(syncDuration);
                }
            }
            else
            {
                for (int i = 0; i < size; i++)
                {
                    LotManager.CrtProcFilter[(int)eProc.Live].Data[i].SetTime();
                }
            }
        }
        public void StopLiveLot()       
        {   
            _timerCheckLiveLot.Stop();
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
    }
}
