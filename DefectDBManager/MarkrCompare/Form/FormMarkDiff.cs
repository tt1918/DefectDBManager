using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
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
            initCrtLotForm();
            initDgvLotSumInfo();
            initRollMapForm();
            initLotListForms(DefectDBManager.Preproc.eProc.Live);

            initLogTimer();

        }

        private void FormMarkDiff_FormClosing(object sender, FormClosingEventArgs e)
        {
            CloseTabSearchSetting();
            CloseLotListForms();
            CloseCrtLotForm();

            closeInOutTimer();
        }
        #endregion

        #region Log Control
        ConcurrentQueue<string> _queueLog = new ConcurrentQueue<string>();
        System.Windows.Forms.Timer _timerLog = new System.Windows.Forms.Timer();

        private void initLogTimer()
        {
            _timerLog.Interval = 200;
            _timerLog.Tick += timerLogProc;

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
            _formMorLive.OnUpdatePrepLncdInfo += initLotListForms;
            OnUpdateLiveLNCDInfo += _formMorLive.UpdateLNCDCtrlData;
            _formMorLive.Show();

            // Search Tab
            tabSearchSet.TabPages[1].Text = "SEARCH";
            tabSearchSet.TabPages[1].Controls.Add(_formMorSearch.Controls[0]);
            _formMorSearch.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            _formMorSearch.OnUpdatePrepLncdInfo += initLotListForms;
            OnUpdateSearchLNCDInfo += _formMorSearch.DisplayLNCDCtrlData;
            _formMorSearch.Dock = DockStyle.Fill;
            _formMorSearch.Show();
        }

        private void CloseTabSearchSetting()
        {
            _formMorLive.OnUpdatePrepLncdInfo -= initLotListForms;
            _formMorSearch.OnUpdatePrepLncdInfo -= initLotListForms;
            OnUpdateLiveLNCDInfo -= _formMorLive.UpdateLNCDCtrlData;
            OnUpdateSearchLNCDInfo -= _formMorSearch.DisplayLNCDCtrlData;
            _formMorLive?.Close();
            _formMorSearch?.Close(); 
        }

        private void tabSearchSet_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch((sender as TabControl).SelectedIndex)
            {
                case 0: // Live Form
                    OnUpdateLiveLNCDInfo?.Invoke();
                    break;

                case 1: // Search Form
                    OnUpdateSearchLNCDInfo?.Invoke();
                    break;
            }
        }
        #endregion

        #region Lot Summery Information
        private DataGridView _dgvLotSumInfo = null;
        static string[] _strDgvLotSumInfoHeader = { "No", "Info1", "Info2", "Info3" };
        static int[] _dgvLotSumInfoLength = { 50, 80, 80, 80 };
        enum eLotSumInfo { No, Info1, Info2, Info3, Total };


        /// <summary>
        /// 데이터 표시
        /// 직접 생성할지 바인딩할지는 추후 변경 필요
        /// </summary>
        private void initDgvLotSumInfo()
        {
            _dgvLotSumInfo = new DataGridView();

            _dgvLotSumInfo.SelectionMode = DataGridViewSelectionMode.CellSelect;
            _dgvLotSumInfo.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
            _dgvLotSumInfo.AllowUserToAddRows = false;
            _dgvLotSumInfo.RowHeadersVisible = false;
            _dgvLotSumInfo.ColumnCount = (int)eLotSumInfo.Total;
            for (int i = 0; i < _dgvLotSumInfo.ColumnCount; i++)
            {
                _dgvLotSumInfo.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
                _dgvLotSumInfo.Columns[i].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;

                _dgvLotSumInfo.Columns[i].Name = _strDgvLotSumInfoHeader[i];
                _dgvLotSumInfo.Columns[i].Width = _dgvLotSumInfoLength[i];
                _dgvLotSumInfo.Columns[i].ReadOnly = true;
            }

            tlpInfoMap.Controls.Add(_dgvLotSumInfo, 0, 0);
            _dgvLotSumInfo.Dock = DockStyle.Fill;
            _dgvLotSumInfo.Show();

        }

        private void displayDgvLotSumInfo()
        {

        }

        #endregion

        #region Roll Map Form
        private FormRollMap _rollMapForm;

        private void initRollMapForm()
        {
            _rollMapForm = new FormRollMap();
            _rollMapForm.TopLevel = false;
            _rollMapForm.InitRollMap();

            tlpInfoMap.Controls.Add(_rollMapForm.Controls[0], 1, 0);
            _rollMapForm.Dock = DockStyle.Fill;
            _rollMapForm.Show();
        }

        #endregion Roll Map Form


        #region Lot List of Product Line
        private List<FormLotList> _lotListForms = null;

        private void initLotListForms(DefectDBManager.Preproc.eProc proc)
        {
            try
            {
                if (_lotListForms != null)
                    CloseLotListForms();

                if (_lotListForms == null)
                    _lotListForms = new List<FormLotList>();

                tabLineList.TabPages.Clear();

                foreach (var item in _lotManager.ProcLNCD.Info)
                {
                    FormLotList form = new FormLotList(this, item.Name);
                    form.TopLevel = false;
                    _lotListForms.Add(form);
                    TabPage page = new TabPage();

                    page.Font = new Font(tabLineList.Font, FontStyle.Regular);
                    page.Text = item.Name;
                    page.Controls.Add(form.Controls[0]);
                    tabLineList.TabPages.Add(page);
                    form.WindowState = System.Windows.Forms.FormWindowState.Maximized;
                    form.Dock = DockStyle.Fill;
                    form.OnUpdatePrepLot += _crtLotForm.OnUpdateLot;
                    form.OnUpdatePrepLot += _rollMapForm.OnUpdateLotInfo;
                    form.Show();
                }
            }
            catch
            {

            }
        }

        private void CloseLotListForms()
        {
            for (int i = 0; i < _lotListForms.Count; i++)
            {
                if (_lotListForms[i] == null) continue;
                
                _lotListForms[i].Close();
                _lotListForms[i].OnUpdatePrepLot -= _crtLotForm.OnUpdateLot;
                _lotListForms[i].OnUpdatePrepLot -= _rollMapForm.OnUpdateLotInfo;
            }
            

            _lotListForms.Clear();
        }
        #endregion

        #region Current Lot Info
        private FormCrtLot _crtLotForm = null;

        private void initCrtLotForm()
        {
            try
            {
                CloseCrtLotForm();
                _crtLotForm = new FormCrtLot(this);
                _crtLotForm.TopLevel = false;

                tlpLineData.Controls.Add(_crtLotForm.Controls[0], 0, 1);
                _crtLotForm.Dock = DockStyle.Fill;
                _crtLotForm.WindowState = System.Windows.Forms.FormWindowState.Maximized;
                _crtLotForm.Show();
            }
            catch
            {

            }
        }

        private void CloseCrtLotForm()
        {
            _crtLotForm?.Close();
        }

        #endregion

    }
}
