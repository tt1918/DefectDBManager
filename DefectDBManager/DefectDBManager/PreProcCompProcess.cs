using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DefectDBManager
{
    public sealed class PreProcCompProcess : IDisposable
    {
        /// <summary>
        /// DB Query 및 탐색
        /// </summary>
        public PreProcCompDB[] _DBProc;
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
        public CodeConfig[] _CodeConfig;
        /// <summary>
        /// Data 탐색 옵션
        /// </summary>
        public Option[] _Option;

        /// <summary>
        /// 데이터 탐색 파라미터
        /// </summary>
        public Param[] _Param;

        public FormDB _FormDB_Now { get { return formDB[0]; } }
        public FormDB _FormDB_Next { get { return formDB[1]; } }
        /// <summary>
        /// DB에서 받은 데이터 표시 및 컨트롤 
        /// </summary>
        private FormDB[] formDB = new FormDB[2];
        
        bool disposed = false;

        private object parent = null;

        private Thread threadDBConnect = null;
        public event DelegateProcessEvent OnProcessEvent = null;

        public PreProcCompProcess(object parent)
        {
            this.parent = parent;

            _DestConfig = new DestConfig();
            if (_DestConfig.Read() < 0) //Dest.Ini 파일 읽어들임
            {
                MessageBox.Show($"Failed to read {Define.DestPath}");
            }

            _DbConn = new OracleDbConnection();

            int cnt = System.Enum.GetValues(typeof(eDbIdWhen)).Length + 1;
            _DBProc = new PreProcCompDB[cnt];
            _Param = new Param[cnt];
            _Option = new Option[cnt];
            _CodeConfig = new CodeConfig[cnt];

            for (int i = 0; i < cnt; i++)
            {
                _Option[i] = new Option(i);
                _Param[i] = new Param();
                _Param[i]._UserDefectClass.Load();
                _CodeConfig[i] = new CodeConfig();

                _DBProc[i] = new PreProcCompDB(this, _DbConn);

                _DBProc[i].DbDestConfig = _DestConfig;
                _DBProc[i].DBCodeConfig = _CodeConfig[i];
                _DBProc[i].DbOption = _Option[i];
                _DBProc[i].CrtParam = _Param[i];

                int count = System.Enum.GetValues(typeof(eFCD)).Length;
                _DBProc[i].ResultDefect = new ResultData[count];
                for (int idx = 0; idx < count; i++)
                    _DBProc[i].ResultDefect[i] = new ResultData();
            }

            if (this.threadDBConnect != null)
            {
                this.threadDBConnect.Join(100);
                this.threadDBConnect = null;
            }


            for (int i = 0; i < 2; i++)
            {
                formDB[i] = new FormDB(this);
                formDB[i].DBConn = _DbConn;
                formDB[i].TodayDataBase = _DBProc[i];
            }

            this.threadDBConnect = new Thread(this.DbConnect);
            this.threadDBConnect.Start();
            this.parent = parent;
        }

        ~PreProcCompProcess()
        {
            if (this.disposed)
                return;
            this.Dispose(false);
        }
        public void Dispose()
        {
            this.Dispose(true);
            for (int i = 0; i < 2; i++)
                formDB[i].Dispose();
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

        public void ShowDBViewer(bool isNext)
        {
            int idx = 0;
            if (isNext == false) idx = 0;
            else idx = 1;

            if (formDB[idx] == null) return;

            formDB[idx].TodayDataBase= _DBProc[idx];
            formDB[idx].Show();
        }

        public void HideDBViewer()
        {
            formDB[0].Hide();
            formDB[1].Hide();
        }

        static void searchDailyLot(object obj)
        {
            PreProcCompDB dbProc = (PreProcCompDB)obj;
            string lotID = dbProc.SearchLotName;
            string Y0LNCD = dbProc.SearchY0LNCD;
            dbProc.SearchLot(Y0LNCD, lotID);
            
            // 
        }

        public void SearchDailyLot(bool isNext, string lotID, string Y0LNCD)
        {
            PreProcCompDB proc = null;

            if (isNext == false)    proc = _DBProc[(int)eDbIdWhen.Now];
            else                    proc = _DBProc[(int)eDbIdWhen.Next];

            proc.SearchLotName = lotID;
            proc.SearchY0LNCD = Y0LNCD;

            Task task = new Task(searchDailyLot, proc);
            task.Start();
        }

    }
}
