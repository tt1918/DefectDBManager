using DefectDBManager;
using DefectDBManager.Preproc;
using MarkCompare.Delegate;
using MarkCompare.Helper;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MarkCompare
{
    public partial class FormMarkDiff : Form
    {
        #region Param
        string _cultureCode = "";

        DefectDBManager.PreprocLotManager _lotManager = null;
        DefectDBManager.CompPreprocDefect _dbProcess = null;

        CSVProcParam _csvCompParam = new CSVProcParam();
        PreProcResultData _csvCompData = new PreProcResultData();
        #endregion

        #region Event
        public event MarkCompare.Delegate.UpdateEvent OnUpdateLiveLNCDInfo = null;
        public event MarkCompare.Delegate.UpdateEvent OnUpdateSearchLNCDInfo = null;
        public event MarkCompare.Delegate.UpdateSelectedLotInfo OnUpdateSelLotInfo = null;
        //public event MarkrCompare.Delegate.UpdatePrepLot OnUpdatePrepLot = null;
        public event DeleUpdateLanguage OnUpdateLanguage = null;
        #endregion

        public FormMarkDiff()
        {
            InitializeComponent();
            _csvCompParam.Load();
        }

        public FormMarkDiff(DefectDBManager.PreprocLotManager lotManager, DefectDBManager.CompPreprocDefect proc)
        {
            InitializeComponent();
            _lotManager = lotManager;
            _dbProcess = proc;
            _csvCompParam.Load();
        }

        #region Create/Destroy
        private void FormMarkDiff_Load(object sender, EventArgs e)
        {
            initTabSearchSetting();
            initRollMapForm();
            initLotListForms();
            initLotSummary();
            initLogTimer();
            initStateViewTimer();

            switchRollmapAndLotHistroy(DefectDBManager.Preproc.eProc.Live);

            // 언어 변경 함수 연결
            initLanguageFunc();
        }

        private void FormMarkDiff_FormClosing(object sender, FormClosingEventArgs e)
        {
            // 언어 변경 함수 연결 해제
            closeLanguageFunc();

            closeTabSearchSetting();
            closeLotListForms();
            closeInOutTimer();
            closeLotSummary();
            closeStateViewTimer();
        }
        #endregion

        #region 검사 상태 표시
        System.Windows.Forms.Timer _timerStateView = new System.Windows.Forms.Timer();
        private void initStateViewTimer()
        {
            _timerStateView.Interval = 1000;
            _timerStateView.Tick += timerStateView;
            _timerStateView.Start();

        }
        private void closeStateViewTimer()
        {
            _timerStateView.Stop();
            _timerStateView.Tick -= timerStateView;
        }
        private void timerStateView(object sender, EventArgs e)
        {
            lblRunState.BeginInvoke(new Action(() =>
            {
                lblRunState.SuspendLayout();
                if (_dbProcess.IsRunLiveTimer)
                {
                    lblRunState.BkColor = Color.Yellow;
                    lblRunState.ForeColor = Color.ForestGreen;
                    lblRunState.Text = Lang.ProcLiveSearch;
                }
                else if (_dbProcess.IsRunSearchingLotList)
                {
                    lblRunState.BkColor = Color.White;
                    lblRunState.ForeColor = Color.DarkBlue;
                    lblRunState.Text = Lang.ProcSearch;
                }
                else if(_dbProcess.IsRunSelectedLotList)
                {
                    lblRunState.BkColor = Color.White;
                    lblRunState.ForeColor = Color.DarkBlue;
                    lblRunState.Text = Lang.ProcSelectedLot;
                }
                else
                {
                    lblRunState.BkColor = Color.Black;
                    lblRunState.ForeColor = Color.White;
                    lblRunState.Text = Lang.ProcStop;
                }
                lblRunState.ResumeLayout();
            }));
            
        }
        #endregion

        #region Log Control
        ConcurrentQueue<string> _queueLog = new ConcurrentQueue<string>();
        System.Windows.Forms.Timer _timerLog = new System.Windows.Forms.Timer();

        private void initLogTimer()
        {
            _timerLog.Interval = 200;
            _timerLog.Tick += timerLogProc;
            _timerLog.Start();
        }
        private void closeInOutTimer()
        {
            _timerLog.Stop();
            _timerLog.Tick -= timerLogProc;
        }
        private void timerLogProc(object sender, EventArgs e)
        {
            _timerLog.Stop();
            int maxCnt = 200;
            try
            {
                int queueSize = _queueLog.Count;
                if (queueSize == 0)
                {
                    _timerLog.Start();
                    return;
                }

                lbLog.BeginUpdate();

                // 100개 까지만 데이터 표시함
                while (lbLog.Items.Count > maxCnt)
                {
                    lbLog.Items.RemoveAt(maxCnt-1);
                }

                while (queueSize > 0)
                {
                    if (lbLog.Items.Count > maxCnt)
                        lbLog.Items.RemoveAt(maxCnt - 1);

                    string strData;
                    if (_queueLog.TryDequeue(out strData) == true)
                        lbLog.Items.Insert(0, strData);
                    queueSize--;
                }
            }
            catch
            {

            }
            finally
            {
                lbLog.EndUpdate();
                _timerLog.Start();
            }
        }


        public void AddLog(string comment, Log.Level level)
        {
            _queueLog.Enqueue(comment);
        }
        public void ClearLog()
        {
            string cmt;
            try
            {
                while (!_queueLog.IsEmpty) { _queueLog.TryDequeue(out cmt); }
            }
            catch
            {

            }
        }

        public void OnDisplayFileServerLog(string text, Log.Level level = Log.Level.Info, bool write = true, bool duplicate = false)
        {
            DateTime time = DateTime.Now;
            string data;
            data = $"{time.Hour}:{time.Minute}:{time.Second} {text}";
            AddLog(data, level);
            if (write) SystemLog.FileServer.Write(level, text);
        }
        public void OnDisplayNetworkLog(string text, Log.Level level = Log.Level.Info, bool write = true, bool duplicate = false)
        {
            DateTime time = DateTime.Now;
            string data;
            data = $"{time.Hour}:{time.Minute}:{time.Second} {text}";
            AddLog(data, level);
            if (write) SystemLog.Network.Write(level, text);
        }
        public void OnDisplaySystemLog(string text, Log.Level level = Log.Level.Info, bool write = true, bool duplicate = false)
        {
            DateTime time = DateTime.Now;
            string data;
            data = $"{time.Hour}:{time.Minute}:{time.Second} {text}";
            AddLog(data, level);
            if (write) SystemLog.System.Write(level, text);
        }
        public void OnDisplayAlarmLog(string text, Log.Level level = Log.Level.Info, bool write = true, bool duplicate = false)
        {
            DateTime time = DateTime.Now;
            string data;
            data = $"{time.Hour}:{time.Minute}:{time.Second} {text}";
            AddLog(data, level);
            if (write) SystemLog.Alarm.Write(level, text);
        }
        public void OnDisplayLog(string text, Log.Level level = Log.Level.Info, bool write = true, bool duplicate = false)
        {
            DateTime time = DateTime.Now;
            string data;
            data = $"{time.Hour}:{time.Minute}:{time.Second} {text}";
            AddLog(data, level);
        }
        public void OnDispDBLog(string text)
        {
            DateTime time = DateTime.Now;
            string data;
            data = $"{time.Hour}:{time.Minute}:{time.Second} {text}";
            AddLog(data, Log.Level.Info);
        }
        #endregion

        #region Tab Serach Setting
        public FormMonitorLive FormMorLive
        {
            get { return _formMorLive; }
            private set { _formMorLive = value; }
        }
        private FormMonitorLive _formMorLive;

        public FormMonitorSearch FormMorSearch
        {
            get { return _formMorSearch; }
            private set { _formMorSearch = value; }
        }
        private FormMonitorSearch _formMorSearch;

        public FormMonitorSelectedLot FormMorSelectedLot
        {
            get { return _formMorSelectedLot; }
            private set { _formMorSelectedLot = value; }
        }
        private FormMonitorSelectedLot _formMorSelectedLot;

        private void initTabSearchSetting()
        {
            _formMorLive = new FormMonitorLive(_lotManager);
            _formMorLive.Process = _dbProcess;
            _formMorLive.TopLevel = false;

            _formMorSearch = new FormMonitorSearch(_lotManager);
            _formMorSearch.Process = _dbProcess;
            _formMorSearch.TopLevel = false;

            _selLotParam.Load();
            _formMorSelectedLot = new FormMonitorSelectedLot(_lotManager, _selLotParam);
            _formMorSelectedLot.Process = _dbProcess;
            _formMorSelectedLot.TopLevel = false;

            tabSearchSet.TabPages.Clear();

            // Live Tab
            TabPage tabPage = new TabPage(Lang.LiveSearch);
            tabSearchSet.TabPages.Add(tabPage);
            tabSearchSet.TabPages[0].Controls.Add(_formMorLive.Controls[0]);
            _formMorLive.Dock = DockStyle.Fill;
            _formMorLive.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            _formMorLive.OnUpdatePrepLncdInfo += updateLiveMonitoringCtrl;
            OnUpdateLiveLNCDInfo += _formMorLive.DisplayLNCDCtrlData;

            // Search Tab
            TabPage tabPage1 = new TabPage(Lang.PeridoSearch);
            tabSearchSet.TabPages.Add(tabPage1);
            tabSearchSet.TabPages[1].Controls.Add(_formMorSearch.Controls[0]);
            _formMorSearch.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            _formMorSearch.OnUpdatePrepLncdInfo += showLotListForm;
            OnUpdateSearchLNCDInfo += _formMorSearch.DisplayLNCDCtrlData;
            _formMorSearch.OnOpenCsvForm += OpenFormCsv;
            _formMorSearch.Dock = DockStyle.Fill;

            // Lot 지정 검사
            TabPage tabPage2 = new TabPage(Lang.SelectedLotSearch);
            tabSearchSet.TabPages.Add(tabPage2);
            tabSearchSet.TabPages[2].Controls.Add(_formMorSelectedLot.Controls[0]);
            _formMorSelectedLot.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            _formMorSelectedLot.OnUpdatePrepLncdInfo += showLotListForm;
            OnUpdateSelLotInfo += _formMorSelectedLot.DisplayLNCDCtrlData;
            _formMorSelectedLot.OnOpenSelectLotForm += OpenFormSelectedLot;
            _formMorSelectedLot.Dock = DockStyle.Fill;

            _formMorSearch.Show();
            _formMorLive.Show();
            _formMorSelectedLot.Show();
        }

        private void closeTabSearchSetting()
        {
            _formMorLive.OnUpdatePrepLncdInfo -= updateLiveMonitoringCtrl;
            _formMorSearch.OnUpdatePrepLncdInfo -= showLotListForm;
            OnUpdateLiveLNCDInfo -= _formMorLive.DisplayLNCDCtrlData;
            OnUpdateSearchLNCDInfo -= _formMorSearch.DisplayLNCDCtrlData;
            _formMorSearch.OnOpenCsvForm -= OpenFormCsv;

            _formMorSelectedLot.OnOpenSelectLotForm -= OpenFormSelectedLot;
            OnUpdateSelLotInfo -= _formMorSelectedLot.DisplayLNCDCtrlData;

            _formMorLive?.Close();
            _formMorSearch?.Close();
            _formMorSelectedLot?.Close();

            _formMorLive.Dispose();
            _formMorSearch.Dispose();
            _formMorSelectedLot.Dispose();
        }

        private void tabSearchSet_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch ((sender as TabControl).SelectedIndex)
            {
                case 0: // Live Form
                    OnUpdateLiveLNCDInfo?.Invoke();
                    showLotListForm(DefectDBManager.Preproc.eProc.Live);
                    switchRollmapAndLotHistroy(DefectDBManager.Preproc.eProc.Live);
                    break;

                case 1: // Search Form
                    OnUpdateSearchLNCDInfo?.Invoke();
                    showLotListForm(DefectDBManager.Preproc.eProc.Search);
                    switchRollmapAndLotHistroy(DefectDBManager.Preproc.eProc.Search);
                    break;

                case 2: // Selected Form
                    OnUpdateSelLotInfo?.Invoke(_selLotList);
                    showLotListForm(DefectDBManager.Preproc.eProc.Selected);
                    switchRollmapAndLotHistroy(DefectDBManager.Preproc.eProc.Selected);
                    break;
            }
        }
        #endregion

        #region Roll Map Form
        private FormRollMap _rollMapForm;

        private void initRollMapForm()
        {
            _rollMapForm = new FormRollMap();
            _rollMapForm.TopLevel = false;
            _rollMapForm.InitRollMapAndSummary();
            _rollMapForm.Show();

        }

        public void RemoveAll()
        {
            _rollMapForm.ClearMap();
        }

        #endregion Roll Map Form

        #region Lot List of Product Line
        private FormLotList[] _lotListForms = null;

        private void initLotListForms()
        {
            try
            {
                closeLotListForms();
                int size = (int)DefectDBManager.Preproc.eProc.Total;
                _lotListForms = new FormLotList[size];
                for (int i = 0; i < size; i++)
                {
                    _lotListForms[i] = new FormLotList((DefectDBManager.Preproc.eProc)i);
                    _lotListForms[i].TopLevel = false;
                    _lotListForms[i].Show();
                }
                showLotListForm(DefectDBManager.Preproc.eProc.Live);
                updateLiveMonitoringCtrl(DefectDBManager.Preproc.eProc.Live);
            }
            catch
            {

            }
        }

        private void closeLotListForms()
        {
            if (_lotListForms != null)
            {
                for (int i = 0; i < (int)DefectDBManager.Preproc.eProc.Total; i++)
                    _lotListForms[i].Dispose();

                _lotListForms = null;
            }
        }

        private void updateLiveMonitoringCtrl(DefectDBManager.Preproc.eProc proc)
        {
            int idx = (int)proc;
            _lotListForms[idx].OnClearSummaryData();
            string ip = "";
            int checkDuration = 5;
            string line = string.Empty;

            for (int i = 0; i < _lotManager.ProcLNCD.Info.Count; i++)
            {
                switch (proc)
                {
                    case DefectDBManager.Preproc.eProc.Live:
                        if (_lotManager.ProcLNCD.Info[i].CheckStatus == true)
                        {
                            line = $"{_lotManager.ProcLNCD.Info[i].Name}";// _{_lotManager.ProcLNCD.Info[i].LNCD}";
                            ip = _lotManager.ProcLNCD.Info[i].TargetIP;
                            checkDuration = _lotManager.ProcLNCD.Info[i].CheckDuration;

                            bool isFormExist = false;
                            if(_lotListForms[idx].DicFormSummary.ContainsKey("ErrorCheck"))
                            {
                                foreach(var form in _lotListForms[idx].DicFormSummary["ErrorCheck"])
                                    if (form.TargetIP == ip)    isFormExist = true;
                            }

                            if (isFormExist == false)
                                _lotListForms[idx].AddErrorCheckMode(line, ip, checkDuration);
                        }
                        break;
                    case DefectDBManager.Preproc.eProc.Search:
                        //_lotListForms[idx].AddSummaryData(_lotManager.LOT[]);
                        break;
                    case DefectDBManager.Preproc.eProc.Total:
                        break;
                    default:
                        break;
                }
            }
        }

        Form _selForm=null;

        private void showLotListForm(DefectDBManager.Preproc.eProc proc)
        {
            if(_selForm!=null)
                tableLayoutPanel3.Controls.Remove(_selForm);
            _selForm = _lotListForms[(int)proc];
            tableLayoutPanel3.Controls.Add(_selForm, 0, 0);
            _lotListForms[(int)proc].Dock = DockStyle.Fill;
            _lotListForms[(int)proc].ShowPanel(true);
        }
        #endregion

        #region Live Lot History
        public FormLotSummery FormLotSummery 
        {
            get { return _formLotSummary; }
            private set { _formLotSummary = value; }
        }
        private FormLotSummery _formLotSummary=null;
        private void initLotSummary()
        {
            _formLotSummary = new FormLotSummery();
            _formLotSummary.TopLevel = false;
            _formLotSummary.Show();

            OnUpdateLanguage += _formLotSummary.UpdateLanguage;

        }

        private void closeLotSummary()
        {
            if(_formLotSummary!=null)
            {
                _formLotSummary.Dispose();
                _formLotSummary = null;

                OnUpdateLanguage -= _formLotSummary.UpdateLanguage;
            }
        }

        public void UpdateLotSummary()
        {
            List<string> errLot = new List<string>();

            foreach (var item in _lotManager.Live.Product)
            {
                string[] keyData = DefectDBManager.Helper.SplitKeyData(item.Key);
                string lncd = keyData[0];
                if (_lotManager.Live.LOT.ContainsKey(item.Key) == false) continue;

                _formLotSummary.SetLotSummary(item.Key, _lotManager.Live.LOT[item.Key], _lotManager, item.Key);

                foreach(var lot in _lotManager.Live.LOT[item.Key])
                {
                    if (lot.CompResult == eCompResult.ProcNg)
                    {
                        string strTemp = $"{item.Key} : {lot.LotName}";
                        errLot.Add(strTemp);
                    }
                }
            }

            if (errLot.Count > 0)
            {
                BeginInvoke(new Action(delegate
                {
                    FormErrorLotDisp form = new FormErrorLotDisp(DefectDBManager.Preproc.eProc.Live);
                    form.OnUpdateErrorLots(errLot.ToArray());
                    form.Show();
                }));
            }
        }
        #endregion

        #region tableLayoutPanel3 RollMap/LotHistory 표시
        private void switchRollmapAndLotHistroy(DefectDBManager.Preproc.eProc index)
        {
            switch(index)
            {
                case DefectDBManager.Preproc.eProc.Live: // FormLotSummery
                    tableLayoutPanel3.Controls.Remove(_rollMapForm);
                    tableLayoutPanel3.Controls.Add(_formLotSummary, 1, 0);
                    if (_formLotSummary.Dock != DockStyle.Fill) _formLotSummary.Dock = DockStyle.Fill;
                    break;

                case DefectDBManager.Preproc.eProc.Search: // Roll Map
                case DefectDBManager.Preproc.eProc.Selected:
                    tableLayoutPanel3.Controls.Remove(_formLotSummary);
                    tableLayoutPanel3.Controls.Add(_rollMapForm, 1, 0);
                    if(_rollMapForm.Dock!=DockStyle.Fill) _rollMapForm.Dock = DockStyle.Fill;
                    break;

            }
        }
        #endregion

        #region Search 완료
        public void UpdateSearchLotList()
        {
            BeginInvoke(new Action(delegate 
            {
                _lotListForms[(int)DefectDBManager.Preproc.eProc.Search].OnClearSummaryData();
                _lotListForms[(int)DefectDBManager.Preproc.eProc.Search].SetTapControl(_lotManager.CrtProcFilter[(int)DefectDBManager.Preproc.eProc.Search]);
            }));

            List<string> errLot = new List<string>();

            foreach (var item in _lotManager.CrtProcFilter[(int)DefectDBManager.Preproc.eProc.Search].Data)
            {
                DefectDBManager.Preproc.PreprocItem procItem = new DefectDBManager.Preproc.PreprocItem();
                foreach (var set in _lotManager.ProcSetting.Data)
                {
                    if (set.Name == item.Model)
                    {
                        procItem = set;
                        break;
                    }
                }

                if (_lotManager.Search.LOT.ContainsKey(item.ToString()))
                {
                    _lotListForms[(int)DefectDBManager.Preproc.eProc.Search].AddSummaryData(_lotManager.Search.LOT[item.ToString()], procItem, item.ToString(), _lotManager);

                    foreach (var lot in _lotManager.Search.LOT[item.ToString()])
                    {
                        if(lot.CompResult== eCompResult.ProcNg)
                        {
                            string strTemp = $"{item.ToString()} : {lot.LotName}";
                            errLot.Add(strTemp);
                        }
                    }
                }
            }

            if(errLot.Count>0)
            {
                BeginInvoke(new Action(delegate
                {
                    FormErrorLotDisp form = new FormErrorLotDisp(DefectDBManager.Preproc.eProc.Search);
                    form.OnUpdateErrorLots(errLot.ToArray());
                    form.Show();
                }));
            }

        }

        public void UpdateRollmap(DefectDBManager.IPreprocLot lot, string name)
        {
            _rollMapForm.ClearMap();
            if(name!=null)
            {
                if(name!="LOT INSP")
                {
                    string[] filterInfo = DefectDBManager.Helper.SplitKeyData(name);
                    if (filterInfo == null) return;

                    DefectDBManager.Preproc.PreprocItem procItem = new DefectDBManager.Preproc.PreprocItem();
                    procItem = _lotManager.ProcSetting.Data.Find(x => x.Name == filterInfo[2]);

                    DefectDBManager.Preproc.PreprocLNCDInfo info = new DefectDBManager.Preproc.PreprocLNCDInfo();

                    foreach (var item in lot.INSPDAT)
                    {
                        foreach (var item2 in item)
                        {
                            foreach (var item3 in item2.Data)
                            {
                                info = _lotManager.ProcLNCD.Info.Find(x => x.LNCD == item3.LNCD && x.Name == filterInfo[0]);
                                if (info != null)
                                    _rollMapForm.OnUpdateLotInfo(lot, info, procItem);
                            }
                        }
                    }
                }
                else
                {
                    _rollMapForm.OnUpdateLotInfo(lot, SelLotParam);
                }
            }
            else
            {
               
            }
           
        }

        public void UpdateRollmapCSV(DefectDBManager.IPreprocLot lot)
        {
            _rollMapForm.ClearMap();
            // CSV 파일이 업데이트 됨
            _rollMapForm.OnUpdateLotInfo(lot, _csvCompParam);
        }

        public void UpdateRollmapDB(DefectDBManager.IPreprocLot lot)
        {
            _rollMapForm.ClearMap();

            _rollMapForm.OnUpdateLotInfo(lot, SelLotParam);
        }

        #endregion

        #region CSV 데이터 비교
        List<string> csvList = new List<string>();
        public void OpenFormCsv()
        {
            FormCsv form = new FormCsv(_csvCompParam);
            if (form.ShowDialog() == DialogResult.OK)
            {
                _csvCompParam = form._procItem;
                csvList = form.Csv;
                CompareCsv();
            }
        }
        public void CompareCsv()
        {
            try
            {
                string strLot = "";
                // 데이터 초기화
                _csvCompData.ResetAll();
                PreprocItem preprocItem = new PreprocItem();
                SystemLog.DisplayFileServerLog(Lang.startComparingCSV);
                Dictionary<int, List<PointF>> defPos = new Dictionary<int, List<PointF>>();
                int cnt = 0;
                int headerCnt = 0;

                NittoDB dataBase = new NittoDB(null, null);
                dataBase.ResultDefect = new ResultData();
                dataBase.CrtParam = new Param();
                dataBase.DbDestConfig = new DestConfig();
                dataBase.DbOption = new DefectDBManager.Option(0);
                dataBase.DbDestConfig.CSVType = eCSV_TYPE.NITTO;
                dataBase.DbDestConfig.CSV_Ver = 1;

                int idxCnt = 0;
                foreach (var item in csvList)
                {
                    dataBase.ResultDefect.ResetAll();
                    DefectCSV.OpenCompareCsV(item, dataBase);

                    // 여기서 데이터 후처리

                    strLot = dataBase._CSVLoadInfo[0].LotNo;
                    if (_csvCompParam.CompType == 0)
                    {
                        if (idxCnt == 0)
                        {
                            string pathL = Path.GetFileNameWithoutExtension(item);
                            _csvCompData.MarkData.LNCD = pathL;
                            ProcessData tmpProc = new ProcessData(pathL, pathL);
                            preprocItem.Reference = tmpProc;
                            foreach (var item1 in dataBase.ResultDefect.MarkFault.Data.Data)
                            {
                                // 데이터는 처리가 필요함. 
                                _csvCompData.MarkData.Add(item1);
                            }
                        }
                        else
                        {
                            string pathL = Path.GetFileNameWithoutExtension(item);
                            PreprocMrkDat preMarkData = new PreprocMrkDat();
                            preMarkData.LNCD = pathL;
                            foreach (var item1 in dataBase.ResultDefect.MarkFault.Data.Data)
                            {
                                // 데이터는 처리가 필요함. 
                                preMarkData.Data.Add(item1);
                            }
                            ProcessData tmpProc = new ProcessData(pathL, pathL);
                            preprocItem.Compare.Add(tmpProc);
                            _csvCompData.PreMarkData[0].Add(preMarkData);
                        }
                    }
                    else
                    {
                        int refIdx = 100;
                        int[] lut = new int[10];

                        List<PreprocMrkDat> preMarkData = new List<PreprocMrkDat>();
                        for (int i = 0; i < 2; i++)
                            preMarkData.Add(new PreprocMrkDat());

                        if (_csvCompParam.CompType == 1)
                        {
                            refIdx = 8;
                            lut[8] = 2;
                            lut[9] = 0;
                            lut[7] = 1;

                            ProcessData tmpProc = new ProcessData("ES", "ES");
                            preprocItem.Reference = tmpProc;
                            ProcessData tmpProc1 = new ProcessData("TG", "TG");
                            preprocItem.Compare.Add(tmpProc1);
                            ProcessData tmpProc2 = new ProcessData("ETC", "ETC");
                            preprocItem.Compare.Add(tmpProc2);

                            _csvCompData.MarkData.LNCD = "ES";
                            preMarkData[0].LNCD = preMarkData[0].CTLNO = "TG";
                            preMarkData[1].LNCD = preMarkData[1].CTLNO = "ETC";
                        }
                        else if (_csvCompParam.CompType == 2)
                        {
                            refIdx = 9;
                            lut[9] = 2;
                            lut[8] = 0;
                            lut[7] = 1;

                            ProcessData tmpProc = new ProcessData("TG", "TG");
                            preprocItem.Reference = tmpProc;
                            ProcessData tmpProc1 = new ProcessData("ES", "ES");
                            preprocItem.Compare.Add(tmpProc1);
                            ProcessData tmpProc2 = new ProcessData("ETC", "ETC");
                            preprocItem.Compare.Add(tmpProc2);

                            _csvCompData.MarkData.LNCD = "TG";
                            preMarkData[0].LNCD = preMarkData[0].CTLNO = "ES";
                            preMarkData[1].LNCD = preMarkData[1].CTLNO = "ETC";
                        }
                        else
                        {
                            refIdx = 7;
                            lut[7] = 2;
                            lut[8] = 0;
                            lut[9] = 1;

                            ProcessData tmpProc = new ProcessData("ETC", "ETC");
                            preprocItem.Reference = tmpProc;
                            ProcessData tmpProc1 = new ProcessData("ES", "ES");
                            preprocItem.Compare.Add(tmpProc1);
                            ProcessData tmpProc2 = new ProcessData("TG", "TG");
                            preprocItem.Compare.Add(tmpProc2);

                            _csvCompData.MarkData.LNCD = "ETC";
                            preMarkData[0].LNCD = preMarkData[0].CTLNO = "ES";
                            preMarkData[1].LNCD = preMarkData[1].CTLNO = "TG";
                        }


                        foreach (var item1 in dataBase.ResultDefect.MarkFault.Data.Data)
                        {
                            // 데이터는 처리가 필요함. 
                            if (item1.DefectLine % 10 == refIdx)
                                _csvCompData.MarkData.Add(item1);
                            else
                                preMarkData[lut[item1.DefectLine % 10]].Data.Add(item1);
                        }
                        _csvCompData.PreMarkData[0].Add(preMarkData[0]);
                        _csvCompData.PreMarkData[0].Add(preMarkData[1]);
                    }
                    idxCnt++;
                }

                // Data 비교 처리
                preprocItem.BasicRange = _csvCompParam.BasicRange;
                preprocItem.CompRange = _csvCompParam.CompRange;
                preprocItem.UseAiResult = _csvCompParam.UseAiResult;

                PreprocLotFilter tmpLot = new PreprocLotFilter(strLot, null, _csvCompData);
                tmpLot.IsCSVMode = true;
                tmpLot.ProcData = preprocItem;

                tmpLot.ComparePosition();

                // 데이터 정리
                _lotManager.Search.ClearLot();
                _lotManager.Search.AddLot("CSV", tmpLot);

                BeginInvoke(new Action(delegate
                {
                    _lotListForms[(int)DefectDBManager.Preproc.eProc.Search].OnClearSummaryData();
                    _lotListForms[(int)DefectDBManager.Preproc.eProc.Search].SetTapControlCsv();
                }));

                _lotListForms[(int)DefectDBManager.Preproc.eProc.Search].AddSummaryData(_lotManager.Search.LOT["CSV"], preprocItem, _lotManager);

                SystemLog.DisplayFileServerLog(Lang.finishedComparingCSV);
            }
            catch (Exception e)
            {
                SystemLog.DisplaySystemLog($"CompareCsv Error, {e.Message}", Log.Level.Error);
            }
        }
        #endregion

        #region Lot 선택 검사 처리
       
        public LotSelProcParam SelLotParam { get { return _selLotParam; } }
        LotSelProcParam _selLotParam = new LotSelProcParam();

        // 검사할 Lot 배열
        public List<string> SelLotList  {   get { return _selLotList; } }
        List<string> _selLotList = new List<string>();

        public void OpenFormSelectedLot()
        {
            using (FormSelectedLot form = new FormSelectedLot(_selLotParam, _lotManager))
            {
                form.CultureCode = _cultureCode;
                // 검사 시작하지 않으면 저장된 랏 정보를 Form에 넣어준다.
                form._LotList.AddRange(_selLotList);
                if (form.ShowDialog() == DialogResult.OK)
                {
                    _selLotParam = form.ProcItem;
                    _selLotList = form._LotList;
                    OnUpdateSelLotInfo?.Invoke(_selLotList);
                }
            }
        }

        public void UpdateSelectedLotList()
        {
            BeginInvoke(new Action(delegate
            {
                _lotListForms[(int)DefectDBManager.Preproc.eProc.Selected].OnClearSummaryData();
                if (_selLotParam.FilterType == FilterType.UserFilter)
                    _lotListForms[(int)DefectDBManager.Preproc.eProc.Selected].SetTapControl(_selLotParam.UserFilter);
                else if (_selLotParam.FilterType == FilterType.DbFilter)
                    _lotListForms[(int)DefectDBManager.Preproc.eProc.Selected].SetTapControlDB();
            }));

            List<string> errLot = new List<string>();

            if(_selLotParam.FilterType==FilterType.UserFilter)
            {
                foreach (var item in _selLotParam.UserFilter.Data)
                {
                    DefectDBManager.Preproc.PreprocItem procItem = new DefectDBManager.Preproc.PreprocItem();
                    foreach (var set in _lotManager.ProcSetting.Data)
                    {
                        if (set.Name == item.Model)
                        {
                            procItem = set;
                            break;
                        }
                    }

                    if (_lotManager.Selected.LOT.ContainsKey(item.ToString()))
                    {
                        _lotListForms[(int)DefectDBManager.Preproc.eProc.Selected].AddSummaryData(_lotManager.Selected.LOT[item.ToString()], procItem, item.ToString(), _lotManager);

                        foreach (var lot in _lotManager.Selected.LOT[item.ToString()])
                        {
                            if (lot.CompResult == eCompResult.ProcNg)
                            {
                                string strTemp = $"{item.ToString()} : {lot.LotName}";
                                errLot.Add(strTemp);
                            }
                        }
                    }
                }
            }
            else
            {
                foreach(var lots in _lotManager.Selected.LOT)
                {
                    if(lots.Key== "DB")
                        _lotListForms[(int)DefectDBManager.Preproc.eProc.Selected].AddSummaryData(lots.Value, _selLotParam, "LOT INSP", _lotManager);
             
                    foreach (var lot in lots.Value)
                    {
                        if (lot.CompResult == eCompResult.ProcNg)
                        {
                            string strTemp = $"DB : {lot.LotName}";
                            errLot.Add(strTemp);
                        }
                    }
                }
            }

            if (errLot.Count > 0)
            {
                BeginInvoke(new Action(delegate
                {
                    FormErrorLotDisp form = new FormErrorLotDisp(DefectDBManager.Preproc.eProc.Selected);
                    form.OnUpdateErrorLots(errLot.ToArray());
                    form.Show();
                }));
            }
        }

        #endregion



        #region 언어 변경
        private void initLanguageFunc()
        {
            OnUpdateLanguage += _formMorLive.UpdateLanguage;
            OnUpdateLanguage += _formMorSearch.UpdateLanguage;
            OnUpdateLanguage += _formMorSelectedLot.UpdateLanguage;

            int size = (int)DefectDBManager.Preproc.eProc.Total;
            for(int i=0; i<size; i++)
                OnUpdateLanguage += _lotListForms[i].UpdateLanguage;
        }

        private void closeLanguageFunc()
        {
            OnUpdateLanguage -= _formMorLive.UpdateLanguage;
            OnUpdateLanguage -= _formMorSearch.UpdateLanguage;
            OnUpdateLanguage -= _formMorSelectedLot.UpdateLanguage;

            int size = (int)DefectDBManager.Preproc.eProc.Total;
            for (int i = 0; i < size; i++)
                OnUpdateLanguage -= _lotListForms[i].UpdateLanguage;
        }


        public async void UpdateLanguage(string culture)
        {
            _cultureCode = culture;
            
            string fontName = Functions.GetCultureFontName(culture);
            await Task.Run(() => OnUpdateLanguage?.Invoke(culture));
            
            // 변경할 언어 표시 추가
            await Task.Run(() =>
            {
                Font newFont = new Font(fontName, 9);
                string text = "";
                if (_dbProcess.IsRunLiveTimer) text = Lang.ProcLiveSearch;
                else if (_dbProcess.IsRunSearchingLotList) text = Lang.ProcSearch;
                else text = Lang.ProcStop;

                UIHelper.SetText(lblRunState, text);
                tabSearchSet.BeginInvoke(new Action(() =>
                {
                    tabSearchSet.Font = newFont;
                    tabSearchSet.TabPages[0].Text = Lang.LiveSearch;
                    tabSearchSet.TabPages[1].Text = Lang.PeridoSearch;
                }));
            });
        }
        #endregion
    }
}
