using DefectDBManager;
using DefectDBManager.Preproc;
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

namespace MarkrCompare
{


    public partial class FormMarkDiff : Form
    {
        #region Param
        DefectDBManager.PreprocLotManager _lotManager = null;
        #endregion

        #region Event
        public event MarkrCompare.Delegate.UpdateEvent OnUpdateLiveLNCDInfo = null;
        public event MarkrCompare.Delegate.UpdateEvent OnUpdateSearchLNCDInfo = null;
        //public event MarkrCompare.Delegate.UpdatePrepLot OnUpdatePrepLot = null;
        #endregion

        public FormMarkDiff()
        {
            InitializeComponent();

        }

        public FormMarkDiff(DefectDBManager.PreprocLotManager lotManager)
        {
            InitializeComponent();
            _lotManager = lotManager;
        }

        #region Create/Destroy
        private void FormMarkDiff_Load(object sender, EventArgs e)
        {
            initTabSearchSetting();
            initRollMapForm();
            initLotListForms();
            initLotSummary();
            initLogTimer();

            switchRollmapAndLotHistroy(DefectDBManager.Preproc.eProc.Live);
        }

        private void FormMarkDiff_FormClosing(object sender, FormClosingEventArgs e)
        {
            closeTabSearchSetting();
            closeLotListForms();
            closeInOutTimer();
            closeLotSummary();
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
        public FormMornitorLive FormMorLive
        {
            get { return _formMorLive; }
            private set { _formMorLive = value; }
        }
        private FormMornitorLive _formMorLive;

        public FormMornitorSearch FormMorSearch
        {
            get { return _formMorSearch; }
            private set { _formMorSearch = value; }
        }
        private FormMornitorSearch _formMorSearch;

        private void initTabSearchSetting()
        {
            _formMorLive = new FormMornitorLive(_lotManager);
            _formMorLive.TopLevel = false;

            _formMorSearch = new FormMornitorSearch(_lotManager);
            _formMorSearch.TopLevel = false;

            // Live Tab
            tabSearchSet.TabPages[0].Text = "LIVE";
            tabSearchSet.TabPages[0].Controls.Add(_formMorLive.Controls[0]);
            _formMorLive.Dock = DockStyle.Fill;
            _formMorLive.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            _formMorLive.OnUpdatePrepLncdInfo += updateLiveMornitoringCtrl;
            OnUpdateLiveLNCDInfo += _formMorLive.DisplayLNCDCtrlData;
            _formMorLive.Show();

            // Search Tab
            tabSearchSet.TabPages[1].Text = "SEARCH";
            tabSearchSet.TabPages[1].Controls.Add(_formMorSearch.Controls[0]);
            _formMorSearch.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            _formMorSearch.OnUpdatePrepLncdInfo += showLotListForm;
            OnUpdateSearchLNCDInfo += _formMorSearch.DisplayLNCDCtrlData;
            _formMorSearch.OnOpenCsvForm += OpenFormCsv;
            _formMorSearch.Dock = DockStyle.Fill;
            _formMorSearch.Show();
        }

        private void closeTabSearchSetting()
        {
            _formMorLive.OnUpdatePrepLncdInfo -= showLotListForm;
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
                    _lotListForms[i] = new FormLotList();
                    _lotListForms[i].TopLevel = false;
                    _lotListForms[i].Show();
                }
                showLotListForm(DefectDBManager.Preproc.eProc.Live);
                updateLiveMornitoringCtrl(DefectDBManager.Preproc.eProc.Live);
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

        private void updateLiveMornitoringCtrl(DefectDBManager.Preproc.eProc proc)
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
                            line = _lotManager.ProcLNCD.Info[i].Name;
                            ip = _lotManager.ProcLNCD.Info[i].TargetIP;
                            checkDuration = _lotManager.ProcLNCD.Info[i].CheckDuration;

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

        }

        private void closeLotSummary()
        {
            if(_formLotSummary!=null)
            {
                _formLotSummary.Dispose();
                _formLotSummary = null;
            }
        }

        public void UpdateLotSummary()
        {
            foreach (var item in _lotManager.LiveProduct)
            {
                string[] keyData = item.Key.Split('_');
                string lncd = keyData[0];

                _formLotSummary.SetLotSummary(lncd, _lotManager.LiveLot[lncd], _lotManager, item.Key);
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
            //updateLiveMornitoringCtrl(DefectDBManager.Preproc.eProc.Search);
            BeginInvoke(new Action(delegate 
            {
                _lotListForms[(int)eProc.Search].OnClearSummaryData();
            }));
            foreach (var item in _lotManager.CrtProcFilter[(int)eProc.Search].Data)
            {
                PreprocItem procItem = new PreprocItem();
                foreach (var set in _lotManager.ProcSetting.Data)
                {
                    if (set.Name == item.Model)
                    {
                        procItem = set;
                        break;
                    }
                }

                //filter 로 구분하도록 수정 @ATW 250321
                //if (_lotManager.LOT.ContainsKey(item.Line))
                //{
                //    _lotListForms[(int)eProc.Search].AddSummaryData(_lotManager.LOT[item.Line], procItem, item.ToString());
                //}
                if (_lotManager.LOT.ContainsKey(item.ToString()))
                {
                    _lotListForms[(int)eProc.Search].AddSummaryData(_lotManager.LOT[item.ToString()], procItem, item.ToString());
                }
            }
        }

        public void UpdateRollmap(PreprocLot lot/*, PreprocItem procItem*/)
        {
            PreprocItem procItem = new PreprocItem();
            foreach (var item in _lotManager.CrtProcFilter[(int)eProc.Search].Data)
            {
                if (_lotManager.ProcSetting.Data.Find(x => x.Name == item.Model) != null)
                {
                    procItem = _lotManager.ProcSetting.Data.Find(x => x.Name == item.Model);
                    break;
                }
                //foreach (var set in _lotManager.ProcSetting.Data)
                //{
                //    if (set.Name == item.Model)
                //    {
                        
                //    }
                //}
            }

            PreprocLNCDInfo info = new PreprocLNCDInfo();
            //_rollMapForm.ClearMap();
            foreach (var item in lot.INSPDAT)
            {
                foreach (var item2 in item)
                {
                    foreach (var item3 in item2.Data)
                    {
                        info = _lotManager.ProcLNCD.Info.Find(x => x.LNCD == item3.LNCD);
                        if (info != null)
                        {
                            _rollMapForm.OnUpdateLotInfo(lot, info, procItem);
                            //break;
                        }
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
                SystemLog.DisplayFileServerLog("Csv 비교 시작");
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
                    _lotListForms[(int)eProc.Search].OnClearSummaryData();
                }));

                SystemLog.DisplayFileServerLog("Csv 비교 완료");
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
    }
}
