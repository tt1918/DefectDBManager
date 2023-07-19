using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using static DefectDBManager.UserDefectClass;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.Window;

namespace DefectDBManager
{
    public delegate void DelegateProcessEvent(int eventID);
    public sealed class DbManager : IDisposable
    {
        /// <summary>
        /// DB Query 및 탐색
        /// </summary>
        public NittoDB[] _DbProc;
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

        /// <summary>
        /// DB 검색 후 최종 불량 데이터
        /// </summary>
 //       public ResultData[] _ResultData;

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

        public DbManager(object parent)
        {
            this.parent = parent;

            _DestConfig = new DestConfig();
            if (_DestConfig.Read() < 0) //Dest.Ini 파일 읽어들임
            {
                MessageBox.Show($"Failed to read {Define.DestPath}");
            }

            _DbConn = new OracleDbConnection();

            int cnt = System.Enum.GetValues(typeof(eDbIdWhen)).Length + 1;
            _DbProc = new NittoDB[cnt];
            _Param = new Param[cnt];
            _Option = new Option[cnt];
            _CodeConfig = new CodeConfig[cnt];

            for (int i = 0; i < cnt; i++)
            {
                _Option[i] = new Option(i);
                _Param[i] = new Param();
                _Param[i]._UserDefectClass.Load();
                _CodeConfig[i] = new CodeConfig();

                _DbProc[i] = new NittoDB(this, _DbConn);

                _DbProc[i].DbDestConfig = _DestConfig;
                _DbProc[i].DBCodeConfig = _CodeConfig[i];
                _DbProc[i].DbOption = _Option[i];
                _DbProc[i].CrtParam = _Param[i];
                _DbProc[i].ResultDefect = new ResultData();
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
                formDB[i].DataBase = _DbProc[i];
            }

            this.threadDBConnect = new Thread(this.DbConnect);
            this.threadDBConnect.Start();
            this.parent = parent;
        }

        ~DbManager()
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

            formDB[idx].DataBase = _DbProc[idx];
            formDB[idx].Show();
        }

        public void HideDBViewer()
        {
            formDB[0].Hide();
            formDB[1].Hide();
        }

        public void SearchLot(string lotName, bool isNext, int vendor, bool useES, bool useTG, bool useETC)
        {
            int idx = 0;
            if (isNext == false) idx = 0;
            else idx = 1;

            formDB[idx]._SearchRes = eSearchProcessRes.Process_None;
            if (_DbConn.IsDBConnected == false)
            {
                formDB[idx]._SearchRes = eSearchProcessRes.DB_Disconnected;
                OnProcessEvent((int)eEventReport.eFinishedSearchLot);
                return;
            }

            if (formDB[idx].IsSearchDefect() == true)
            {
                formDB[idx]._SearchRes = eSearchProcessRes.DB_SearchIsBusy;
                OnProcessEvent((int)eEventReport.eFinishedSearchLot);
                return;
            }

            _Option[idx].dbWhen = (eDbIdWhen)idx;
            _Option[idx].vendor = vendor;
            _Option[idx].checkES = useES;
            _Option[idx].checkTG = useTG;
            _Option[idx].checkETC = useETC;
            _Option[idx].lotName = lotName;
            formDB[idx].DataBase = _DbProc[idx];
            formDB[idx].DataBase.ResetDataAll();
            formDB[idx].UpdateEndEvent = true;
            formDB[idx].SearchDefect();
        }

        public void SearchCSVFile(string lotName, string filePath, bool isNext, int vendor, bool useES, bool useTG, bool useETC)
        {
            int idx = 0;
            if (isNext == false) idx = 0;
            else idx = 1;

            _Option[idx].dbWhen = (eDbIdWhen)idx;
            _Option[idx].vendor = vendor;
            _Option[idx].checkES = useES;
            _Option[idx].checkTG = useTG;
            _Option[idx].checkETC = useETC;
            _Option[idx].lotName = lotName;
            formDB[idx].DataBase = _DbProc[idx];
            formDB[idx].DataBase.ResetDataAll();
            formDB[idx].UpdateEndEvent = true;
            formDB[idx].SearchCSVFile(filePath);
        }

        public void GetSearchLotResultSummery(bool isNext, ref List<LotSearchResult> results)
        {
            int idx = 0;
            if (isNext == false) idx = 0;
            else idx = 1;

            int size = formDB[idx].DataBase._DbResult.INSPDAT_Data.Length;
            foreach (List<List<INSPDATData>> data in formDB[idx].DataBase._DbResult.INSPDAT_Data)
            {
                if (data == null) continue;
                foreach (List<INSPDATData> items in data)
                {
                    foreach (INSPDATData item in items)
                    {
                        LotSearchResult result = new LotSearchResult();
                        result.LotNo = item.LOTNO;
                        result.DefectCnt = item.RollCtlCnt;
                        result.Line = item.LOTNO.Substring(0, 2); // 확인 필요
                        result.TimeST = item.STRTM;
                        result.DateST = item.STRDT;
                        result.TimeED = item.ENDTM;
                        result.DateED = item.ENDDT;
                        result.Length = (item.Length/1000.0f);
                        result.DefectPerM = ((float)(item.RollCtlCnt) / (float)((item.Width / 1000.0f) * (item.Length / 1000.0f)));
                        results.Add(result);
                    }
                }
            }
        }

        public void GetSearchLotResultSummery_TOT(bool isNext, ref List<LotSearchResult> results)
        {
            int idx = 0;
            if (isNext == false) idx = 0;
            else idx = 1;

            int size = formDB[idx].DataBase._DbResult.INSPDAT_Data.Length;
            foreach (List<List<INSPDATData>> data in formDB[idx].DataBase._DbResult.INSPDAT_Data)
            {
                if (data == null) continue;
                foreach (List<INSPDATData> items in data)
                {
                    foreach (INSPDATData item in items)
                    {
                        LotSearchResult result = new LotSearchResult();
                        result.LotNo = item.Y0KLOT;
                        result.BCNO = item.BCNO;
                        result.DefectCnt = item.RollCtlCnt;
                        result.Line = item.LOTNO.Substring(0, 2); // 확인 필요
                        result.TimeST = item.STRTM;
                        result.DateST = item.STRDT;
                        result.TimeED = item.ENDTM;
                        result.DateED = item.ENDDT;
                        result.Length = (item.Length / 1000.0f);
                        result.DefectPerM = ((float)(item.RollCtlCnt) / (float)((item.Width / 1000.0f) * (item.Length / 1000.0f)));
                        results.Add(result);
                    }
                }
            }
        }

        public void SearchModel(string lotName)
        {
            formDB[0]._SearchRes = eSearchProcessRes.Process_None;
            if (_DbConn.IsDBConnected == false)
            {
                formDB[0]._SearchRes = eSearchProcessRes.DB_Disconnected;
                OnProcessEvent((int)eEventReport.eFinishedSearchModel);
                return;
            }

            if (formDB[0].IsSearchDefect() == true)
            {
                formDB[0]._SearchRes = eSearchProcessRes.DB_SearchIsBusy;
                OnProcessEvent((int)eEventReport.eFinishedSearchModel);
                return;
            }
            _Option[2].dbWhen = (eDbIdWhen)0;
            _Option[2].lotName = lotName;

            _DbProc[2].ResetDataAll();
            formDB[0].IsDataBaseChanged = true;
            formDB[0].DataBase = _DbProc[2];
            formDB[0].SearchModel();
        }
    }
}
