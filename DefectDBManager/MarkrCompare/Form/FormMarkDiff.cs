using DefectDBManager;
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
        DefectDBManager.PreprocLotManager _lotManager = null;
        DefectDBManager.CompPreprocDefect _dbProcess = null;
        #endregion

        #region Event
        public event MarkCompare.Delegate.UpdateEvent OnUpdateLiveLNCDInfo = null;
        public event MarkCompare.Delegate.UpdateEvent OnUpdateSearchLNCDInfo = null;
        //public event MarkrCompare.Delegate.UpdatePrepLot OnUpdatePrepLot = null;
        public event DeleUpdateLanguage OnUpdateLanguage = null;
        #endregion

        public FormMarkDiff()
        {
            InitializeComponent();

        }

        public FormMarkDiff(DefectDBManager.PreprocLotManager lotManager, DefectDBManager.CompPreprocDefect proc)
        {
            InitializeComponent();
            _lotManager = lotManager;
            _dbProcess = proc;
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
                while (lbLog.Items.Count > 100)
                {
                    lbLog.Items.RemoveAt(0);
                }

                while (queueSize > 0)
                {
                    if (lbLog.Items.Count > 100)
                        lbLog.Items.RemoveAt(0);

                    string strData;
                    if (_queueLog.TryDequeue(out strData) == true)
                        lbLog.Items.Add(strData);
                    queueSize--;
                }
                lbLog.SetSelected(lbLog.Items.Count - 1, true);
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
            AddLog(text, level);
            if (write) SystemLog.FileServer.Write(level, text);
        }
        public void OnDisplayNetworkLog(string text, Log.Level level = Log.Level.Info, bool write = true, bool duplicate = false)
        {
            AddLog(text, level);
            if (write) SystemLog.Network.Write(level, text);
        }
        public void OnDisplaySystemLog(string text, Log.Level level = Log.Level.Info, bool write = true, bool duplicate = false)
        {
            AddLog(text, level);
            if (write) SystemLog.System.Write(level, text);
        }
        public void OnDisplayAlarmLog(string text, Log.Level level = Log.Level.Info, bool write = true, bool duplicate = false)
        {
            AddLog(text, level);
            if (write) SystemLog.Alarm.Write(level, text);
        }
        public void OnDisplayLog(string text, Log.Level level = Log.Level.Info, bool write = true, bool duplicate = false)
        {
            AddLog(text, level);
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

        private void initTabSearchSetting()
        {
            _formMorLive = new FormMonitorLive(_lotManager);
            _formMorLive.Process = _dbProcess;
            _formMorLive.TopLevel = false;

            _formMorSearch = new FormMonitorSearch(_lotManager);
            _formMorSearch.Process = _dbProcess;
            _formMorSearch.TopLevel = false;

            // Live Tab
            tabSearchSet.TabPages[0].Text = Lang.LiveSearch;
            tabSearchSet.TabPages[0].Controls.Add(_formMorLive.Controls[0]);
            _formMorLive.Dock = DockStyle.Fill;
            _formMorLive.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            _formMorLive.OnUpdatePrepLncdInfo += updateLiveMonitoringCtrl;
            OnUpdateLiveLNCDInfo += _formMorLive.DisplayLNCDCtrlData;

            // Search Tab
            tabSearchSet.TabPages[1].Text = Lang.PeridoSearch;
            tabSearchSet.TabPages[1].Controls.Add(_formMorSearch.Controls[0]);
            _formMorSearch.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            _formMorSearch.OnUpdatePrepLncdInfo += showLotListForm;
            OnUpdateSearchLNCDInfo += _formMorSearch.DisplayLNCDCtrlData;
            _formMorSearch.OnOpenCsvForm += OpenFormCsv;
            _formMorSearch.Dock = DockStyle.Fill;
            
            _formMorSearch.Show();
            _formMorLive.Show();
        }

        private void closeTabSearchSetting()
        {
            _formMorLive.OnUpdatePrepLncdInfo -= updateLiveMonitoringCtrl;
            _formMorSearch.OnUpdatePrepLncdInfo -= showLotListForm;
            OnUpdateLiveLNCDInfo -= _formMorLive.DisplayLNCDCtrlData;
            OnUpdateSearchLNCDInfo -= _formMorSearch.DisplayLNCDCtrlData;
            _formMorSearch.OnOpenCsvForm -= OpenFormCsv;

            _formMorLive?.Close();
            _formMorSearch?.Close();
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
                            line = $"{_lotManager.ProcLNCD.Info[i].Name}_{_lotManager.ProcLNCD.Info[i].LNCD}";
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

        private void showLotListForm(DefectDBManager.Preproc.eProc proc)
        {
            switch (proc)
            {
                case DefectDBManager.Preproc.eProc.Live:
                    tableLayoutPanel3.Controls.Remove(_lotListForms[(int)DefectDBManager.Preproc.eProc.Search]);
                    tableLayoutPanel3.Controls.Add(_lotListForms[(int)DefectDBManager.Preproc.eProc.Live], 0, 0);
                    _lotListForms[(int)DefectDBManager.Preproc.eProc.Live].Dock = DockStyle.Fill;
                    _lotListForms[(int)DefectDBManager.Preproc.eProc.Live].ShowPanel(false);
                    break;

                case DefectDBManager.Preproc.eProc.Search:
                    tableLayoutPanel3.Controls.Remove(_lotListForms[(int)DefectDBManager.Preproc.eProc.Live]);
                    tableLayoutPanel3.Controls.Add(_lotListForms[(int)DefectDBManager.Preproc.eProc.Search], 0, 0);
                    _lotListForms[(int)DefectDBManager.Preproc.eProc.Search].Dock = DockStyle.Fill;
                    _lotListForms[(int)DefectDBManager.Preproc.eProc.Search].ShowPanel(true);
                    break;
            }
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

            foreach (var item in _lotManager.LiveProduct)
            {
                string[] keyData = item.Key.Split('_');
                string lncd = keyData[0];
                if (_lotManager.LiveLot.ContainsKey(item.Key) == false) continue;

                _formLotSummary.SetLotSummary(item.Key, _lotManager.LiveLot[item.Key], _lotManager, item.Key);

                foreach(var lot in _lotManager.LiveLot[item.Key])
                {
                    if (lot.CompResult == eCompResult.ProcNg)
                    {
                        string strTemp = $"{item.ToString()} : {lot.LotName}";
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

                if (_lotManager.LOT.ContainsKey(item.ToString()))
                {
                    _lotListForms[(int)DefectDBManager.Preproc.eProc.Search].AddSummaryData(_lotManager.LOT[item.ToString()], procItem, item.ToString());

                    foreach (var lot in _lotManager.LOT[item.ToString()])
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

        public void UpdateRollmap(DefectDBManager.PreprocLot lot, string name)
        {
            _rollMapForm.ClearMap();

            string[] filterInfo = name.Split('_');
            if (filterInfo == null) return;

            DefectDBManager.Preproc.PreprocItem procItem = new DefectDBManager.Preproc.PreprocItem();
            //foreach (var item in _lotManager.CrtProcFilter[(int)DefectDBManager.Preproc.eProc.Search].Data)
            {
                //if (_lotManager.ProcSetting.Data.Find(x => x.Name == item.Model) != null)
                {
                    procItem = _lotManager.ProcSetting.Data.Find(x => x.Name == filterInfo[2]);
                   // break;
                }
            }

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
        #endregion

        List<string> csvList = new List<string>();
        public void OpenFormCsv()
        {
            FormCsv form = new FormCsv();
            if (form.ShowDialog() == DialogResult.OK)
            {
                csvList = form.Csv;
                CompareCsv();
            }
        }

        public void CompareCsv()
        {
            try
            {
                SystemLog.DisplayFileServerLog(Lang.startComparingCSV);
                Dictionary<int, List<PointF>> defPos = new Dictionary<int, List<PointF>>();
                int cnt = 0;
                int headerCnt = 0;
                foreach (var item in csvList)
                {
                    if (!string.IsNullOrWhiteSpace(item) && File.Exists(item))
                    {
                        string[] txt = File.ReadAllLines(item);
                        defPos.Add(cnt, new List<PointF>());

                        foreach (string str in txt)
                        {
                            if (headerCnt++ < 4) continue;
                            string[] data = str.Replace("\"", "").Split(',');
                            double posX = Convert.ToDouble(data[7]);
                            double posY = Convert.ToDouble(data[14]);
                            defPos[cnt].Add(new PointF((float)posX, (float)posY));
                        }
                        cnt++;
                    }
                }

                BeginInvoke(new Action(delegate
                {
                    _lotListForms[(int)DefectDBManager.Preproc.eProc.Search].OnClearSummaryData();
                }));

                SystemLog.DisplayFileServerLog(Lang.finishedComparingCSV);
            }
            catch (Exception e)
            {
                SystemLog.DisplaySystemLog($"CompareCsv Error, {e.Message}", Log.Level.Error);
            }
        }

        public void RemoveAll()
        {
            _rollMapForm.ClearMap();
        }

        #region 언어 변경
        private void initLanguageFunc()
        {
            OnUpdateLanguage += _formMorLive.UpdateLanguage;
            OnUpdateLanguage += _formMorSearch.UpdateLanguage;

            int size = (int)DefectDBManager.Preproc.eProc.Total;
            for(int i=0; i<size; i++)
                OnUpdateLanguage += _lotListForms[i].UpdateLanguage;
        }

        private void closeLanguageFunc()
        {
            OnUpdateLanguage -= _formMorLive.UpdateLanguage;
            OnUpdateLanguage -= _formMorSearch.UpdateLanguage;

            int size = (int)DefectDBManager.Preproc.eProc.Total;
            for (int i = 0; i < size; i++)
                OnUpdateLanguage -= _lotListForms[i].UpdateLanguage;
        }

        public async void UpdateLanguage(string culture)
        {
            string fontName = Functions.GetCultureFontName(culture);

            // 
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
