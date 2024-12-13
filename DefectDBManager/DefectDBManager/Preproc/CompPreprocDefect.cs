using DefectDBManager.DB;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Security;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DefectDBManager
{
    public delegate void DelegateLotProgress(int percent);

    public sealed class CompPreprocDefect : IDisposable
    {
        #region Event
        // 상위 이벤트 보고 
        // 오늘자 생산 예정 PTRY0P 탐색
        public event DelegateEvent OnEndTodayProductSearching = null;
        // 프로세스 상에 발생하는 이벤트 보고용
        public event DelegateProcessEvent OnProcessEvent = null;
         // 랏 검색 진행 상황을 상위로 보고
        public event DelegateLotProgress OnLotProgress = null;
        #endregion Event

        #region Param
        /// <summary>
        /// DB Query 및 탐색
        /// </summary>
        public PreProcCompDB _DBProc;

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

        public MrkctlmstMaterial MRKCTLMST_Material
        {
            get { return _mrkctlmstMaterial; }
            set { _mrkctlmstMaterial = value; }
        }
        private MrkctlmstMaterial _mrkctlmstMaterial = null;

        public PrepocLotManager LotManager
        {
            get { return _lotManager; }
            set { _lotManager = value; }
        }
        private PrepocLotManager _lotManager = null;
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

            _DBProc = new PreProcCompDB(this, _DbConn);

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

        public void SearchLotMarkDiff()
        {
            // LotManager의 데이터는 업데이트되어있는 상황
            Task task = new Task(search, null);
            task.Start();
        }

        private void search(object obj)
        {
            // 해당 공정에 대한 결점 정보 확인
            searchLotList();

            foreach(var list in LotManager.Product)
            {
                string lncd = list.Key;
                foreach (var item in list.Value.Data)
                {
                    string lotName = item.Y0KLOT;
                    SearchDefectData(lncd, lotName);

                    // 검색 진행 상황을 
                    int rate = (int)((float)LotManager.TotalLot / (float)LotManager.TotalProduct);
                    OnLotProgress?.Invoke(rate);
                }
            }
        }

        #region 공정 별 생산 리스트 취합.
        private void searchLotList()
        {
            LotManager.Product.Clear();

            DateTime stTime = LotManager.StartTime ;
            DateTime edTime = LotManager.EndTime ;
            foreach (var data in LotManager.ProcLNCD.Info)
            {
                if (data.Use == false) continue;

                if (_DBProc.SearchPTRYOPList(data.LNCD, stTime, edTime) == true)
                {
                    PTRY0PList list = new PTRY0PList();

                    foreach(var ptry0p in _DBProc.PTRY0PList_Data.Data)
                        list.Add(ptry0p.Clone());

                    // 리스트 데이터 추가
                    LotManager.Product.Add(data.Name, list);
                }
            }
        }
        #endregion


        #region 결점 데이터 검색
        public void SearchDefectData(string lncd, string lotName)
        {
            int error=-1;
            bool usemkcdModel = LotManager.UseMrkctlmstModel;
            try
            {
                PreprocLot lot = _DBProc.SearchLot(lotName, usemkcdModel, false, ref error);
                if (lot == null) return;
                LotManager.AddLot(lncd, lot);
            }
            catch
            {

            }
        }

        #endregion
    }
}
