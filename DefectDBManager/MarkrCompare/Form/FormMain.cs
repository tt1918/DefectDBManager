using Coss.Controls;
using DefectDBManager;
using MarkCompare.Delegate;
using MarkCompare.Properties;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.StartPanel;

namespace MarkCompare
{
    public partial class FormMain : Form
    {
        
        #region Form 종료 못하게 막기
        private const int CP_NOCLOSE_BUTTON = 0x200;
        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                cp.ClassStyle = cp.ClassStyle | CP_NOCLOSE_BUTTON;
                return cp;
            }
        }

        public bool _FormCloseBtnType = false;
        #endregion

        #region Param
        DefectDBManager.PreprocLotManager _lotManager = null;
        DefectDBManager.CompPreprocDefect _dbManager = null;
        SystemParam _systemParam = new SystemParam();
        #endregion

        #region Event
        public event DeleUpdateLanguage OnUpdateLanguage;
        #endregion

        public FormMain()
        {
            InitializeComponent();

            lblTitle.MouseDown += lblTitle_MouseDown;
            lblTitle.MouseMove += lblTitle_MouseMove;
        }

        public FormMain(DefectDBManager.CompPreprocDefect manager)
        {
            InitializeComponent();

            lblTitle.MouseDown += lblTitle_MouseDown;
            lblTitle.MouseMove += lblTitle_MouseMove;

            _dbManager = manager;
            _lotManager = manager.LotManager;
        }

        ~FormMain()
        {
            stopClockTimer();
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            _systemParam.Load();
            SetCultureCode();

            initClockTimer();
            initMarkDiffForm();

            SystemLog.DisplaySystemLog = _markDiffForm.OnDisplaySystemLog;
            SystemLog.DisplayFileServerLog = _markDiffForm.OnDisplayFileServerLog;
            SystemLog.OnDisplayLogData = _markDiffForm.OnDisplayLog;
            SystemLog.DisplayNetworkLog = _markDiffForm.OnDisplayNetworkLog;
            SystemLog.DisplayAlarmLog = _markDiffForm.OnDisplayAlarmLog;
            DefectDBManager.Log.OnDispEventLog += _markDiffForm.OnDispDBLog;

            _dbManager.OnEndSearchingLotList += EndSearchLotList;
            _dbManager.OnEndLiveSearchLot += EndLiveSearch;

            ledOn = Properties.Resources.icons8_green_square_16;
            ledOff = Properties.Resources.icons8_black_medium_square_16;
            initTimerDBConn();

            // 하부 폼이 먼저 만들어져야 해서 마지막에 처리
            initLanguageFunc();

            ChangeLanguage();

            SystemLog.DisplaySystemLog("Program Start");
        }

        private void FormMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            DefectDBManager.Log.OnDispEventLog -= _markDiffForm.OnDispDBLog;
            _dbManager.OnEndSearchingLotList -= EndSearchLotList;
            _dbManager.OnEndLiveSearchLot -= EndLiveSearch;

            // 폼 삭제 전에 이벤트 연결 삭제
            closeLanguageFunc();

            ledOn.Dispose();
            ledOff.Dispose();

            destroyMarkDiffForm();
            closeTimerDBConn();
        }

        #region Marking Comparision Form
        private FormMarkDiff _markDiffForm;

        private void initMarkDiffForm()
        {
            _markDiffForm = new FormMarkDiff(_lotManager, _dbManager);
            _markDiffForm.TopLevel = false;

            tableLayoutPanel1.Controls.Add(_markDiffForm, 0, 1);
            _markDiffForm.Dock = DockStyle.Fill;
            _markDiffForm.Show();

            _markDiffForm.FormMorSearch.OnStartLotSearch += StartSearchLotList;
            _markDiffForm.FormMorSearch.OnStopLotSearch += StopSearchLotList;

            _markDiffForm.FormMorLive.OnStartLiveSearch += _dbManager.StartLiveLot;
            _markDiffForm.FormMorLive.OnStopLiveSearch += _dbManager.StopLiveLot;
            _dbManager.OnStartLiveDefectSearching += _markDiffForm.FormMorLive.StartLotSearch;

            _dbManager.OnEndSearchingLotList += _markDiffForm.FormMorSearch.EndLotSearch;
            _dbManager.OnEndLiveSearchLot += _markDiffForm.FormMorLive.EndLotSearch;

            _dbManager.OnEndLiveSearchLot += _markDiffForm.UpdateLotSummary;
            _dbManager.OnEndSearchingLotList += _markDiffForm.UpdateSearchLotList;
        }

        private void destroyMarkDiffForm()
        {
            _markDiffForm.FormMorSearch.OnStartLotSearch -= StartSearchLotList;
            _markDiffForm.FormMorSearch.OnStopLotSearch -= StopSearchLotList;

            _markDiffForm.FormMorLive.OnStartLiveSearch -= _dbManager.StartLiveLot;
            _markDiffForm.FormMorLive.OnStopLiveSearch -= _dbManager.StopLiveLot;
            _dbManager.OnStartLiveDefectSearching -= _markDiffForm.FormMorLive.StartLotSearch;

            _dbManager.OnEndSearchingLotList -= _markDiffForm.FormMorSearch.EndLotSearch;
            _dbManager.OnEndLiveSearchLot -= _markDiffForm.FormMorLive.EndLotSearch;

            _dbManager.OnEndLiveSearchLot -= _markDiffForm.UpdateLotSummary;

            _dbManager.OnEndSearchingLotList -= _markDiffForm.UpdateSearchLotList;
        }
        #endregion Marking Comparision Form

        #region 마우스로 폼 드래그
        private Point mouseDownLocation;
        private void lblTitle_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                this.mouseDownLocation = e.Location;
            }
        }
        private void lblTitle_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.WindowState == FormWindowState.Maximized) return;

            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                this.Left = e.X + this.Left - this.mouseDownLocation.X;
                this.Top = e.Y + this.Top - this.mouseDownLocation.Y;
            }
        }
        #endregion

        #region 시계
        Timer timerClock = null;
        private void initClockTimer()
        {
            timerClock = new Timer();
            timerClock.Interval = 1000;
            timerClock.Tick += new EventHandler(OnDisplayTime);
            timerClock.Start();
        }

        private void stopClockTimer()
        {
            timerClock.Stop();
        }

        private void OnDisplayTime(object sender, EventArgs e)
        {
            DateTime time = DateTime.Now;
            string strTime = time.ToString("yyyy-MM-dd HH:mm:ss");
            lblTime.Text = strTime;
        }
        #endregion

        #region CONTROL

        private void btnParam_Click(object sender, EventArgs e)
        {
            FormSetting form = new FormSetting(this._lotManager.ProcSetting);
            if (form.ShowDialog() == DialogResult.OK)
                this._lotManager.UpdatePreprocSet(form.PreprocSet);

        }

        private void btnSetting_Click(object sender, EventArgs e)
        {
            FormLNCD form = new FormLNCD();
            form.CultureCode = _cultureCode;
            if (form.ShowDialog() == DialogResult.OK)
                this._lotManager.SetLNCDData(form.MaterialDate);
        }

        private void btnSystem_Click(object sender, EventArgs e)
        {
            FormSystem form = new FormSystem(_systemParam);
            form.CultureCode = _cultureCode;
            if (form.ShowDialog() == DialogResult.OK)
            {
                _systemParam = form.SysParam;
                SetCultureCode();
                ChangeLanguage();
            }
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            string msg = Lang.closeProgram;
            
            if (MessageBox.Show(msg, Lang.btnClose, MessageBoxButtons.OKCancel) != DialogResult.OK)
                return;

            SystemLog.DisplaySystemLog("Program Close");
            this.Close();
        }

        #endregion CONTROL

        #region 최대/최소화
        FormWindowState _oldState = FormWindowState.Normal;
        private void btnMinimize_Click(object sender, EventArgs e)
        {
            this.WindowState = FormWindowState.Minimized;
        }

        private void btnMaximize_Click(object sender, EventArgs e)
        {
            switch (this.WindowState)
            {
                case FormWindowState.Minimized:
                    this.WindowState = FormWindowState.Maximized;
                    btnMaximize.Image?.Dispose();
                    btnMaximize.Image = Properties.Resources.min_24;
                    break;

                case FormWindowState.Maximized:
                    this.WindowState = FormWindowState.Normal;
                    btnMaximize.Image?.Dispose();
                    btnMaximize.Image = Properties.Resources.max_24;
                    break;

                case FormWindowState.Normal:
                    this.WindowState = FormWindowState.Maximized;
                    btnMaximize.Image?.Dispose();
                    btnMaximize.Image = Properties.Resources.min_24;
                    break;
            }
        }
        #endregion

        #region 기간 검색 시작
        public void StartSearchLotList()
        {
            _dbManager.SearchLotMarkDiff();
        }

        public void StopSearchLotList()
        {
            _dbManager.StopSearchingLotList = false;
        }

        public void EndSearchLotList()
        {
            SystemLog.DisplayFileServerLog(Lang.periodOperationIsComplete);
            Invoke(new Action(() => 
            {
                MessageBox.Show(this, Lang.periodOperationIsComplete);
            }));
        }
        #endregion

        #region 실시간 검색 시작
        public void EndLiveSearch()
        {
            bool isError = false;
            StringBuilder sb = new StringBuilder();
            if (_lotManager != null)
            {   
                foreach (var item in _lotManager.LiveProduct)
                {
                    string[] keyData = DefectDBManager.Helper.SplitKeyData(item.Key);
                    string lncd = keyData[0];

                    if (_lotManager.LiveLot.ContainsKey(item.Key) == false)
                        continue;

                    foreach(var lot in _lotManager.LiveLot[item.Key])
                    {
                        if(lot.CompResult==eCompResult.ProcNg)
                        {
                            sb.Append($"[{item.Key}-{lot.LotName}] : {Lang.ErrorOccurrence} \n");
                            isError = true;
                        }
                    }
                }
            }

            SystemLog.DisplayFileServerLog(Lang.realtimeSearchOperationIscomplete);
            Invoke(new Action(() =>
            {
                if(isError==false)  MessageBox.Show(this, Lang.realtimeSearchOperationIscomplete);
                //else                MessageBox.Show(this,sb.ToString());
            }));
        }
        #endregion

        #region DB Connection
        Timer _timerDBConn = null;
        private bool isDbConnOn = false;
        private Image ledOn = null;
        private Image ledOff = null;
        private void btnDBConnect_Click(object sender, EventArgs e)
        {
            using(DefectDBManager.FormDbLoginData form = new FormDbLoginData(_dbManager._DbConn))
            {
                form.ShowDialog();
            }
        }

        private void initTimerDBConn()
        {
            if (_timerDBConn != null)
                closeTimerDBConn();

            _timerDBConn = new System.Windows.Forms.Timer();
            _timerDBConn.Interval = 1000;
            _timerDBConn.Tick += timerCheckDBConn;
            _timerDBConn.Start();
        }

        private void closeTimerDBConn()
        {
            if (_timerDBConn == null) return;

            if (_timerDBConn.Enabled == true)
                _timerDBConn.Enabled = false;

            _timerDBConn.Dispose();
            _timerDBConn = null;
        }

        private void timerCheckDBConn(object sender, EventArgs e)
        {
            if (_dbManager._DbConn == null)
            {
                if(isDbConnOn==true)
                {
                    isDbConnOn = false;
                    btnDBConnect.Image = ledOff;
                }
                SystemLog.DisplaySystemLog(Lang.DbConnError, Log.Level.Error);
                return;
            }
            else
            {
                if(isDbConnOn==false && _dbManager._DbConn.IsDBConnected==true)
                {
                    btnDBConnect.Image = ledOn;
                    isDbConnOn = true;
                    SystemLog.DisplayFileServerLog(Lang.DBConnIsSuccessed);
                }
                else if(isDbConnOn == true && _dbManager._DbConn.IsDBConnected == false)
                {
                    btnDBConnect.Image = ledOff;
                    isDbConnOn = false;
                    SystemLog.DisplayFileServerLog(Lang.DBConnFailed);
                }
            }
        }
        #endregion

        #region 언어 변경
        string _cultureCode ="";
        private void initLanguageFunc()
        {
            OnUpdateLanguage += _markDiffForm.UpdateLanguage;
            OnUpdateLanguage += this.UpdateLanguage;
        }

        private void closeLanguageFunc()
        {
            OnUpdateLanguage -= _markDiffForm.UpdateLanguage;
            OnUpdateLanguage -= this.UpdateLanguage;
        }

        public void SetCultureCode()
        {
            string cultureCode = "";
            switch (_systemParam.Language)
            {
                case eLanguage.Korean:
                    cultureCode = "Ko-Kr";
                    break;

                case eLanguage.English:
                    cultureCode = "en-US";
                    break;

                case eLanguage.Japanese:
                    cultureCode = "ja-JP";
                    break;

                case eLanguage.Chinese:
                    cultureCode = "zh-CN";
                    break;

                case eLanguage.TaiwaneseHokkien:
                    cultureCode = "zh-TW";
                    break;
            }
            _cultureCode = cultureCode;
            var cultrue = new CultureInfo(cultureCode);
            System.Threading.Thread.CurrentThread.CurrentCulture = cultrue;
            System.Threading.Thread.CurrentThread.CurrentUICulture = cultrue;
        }

        public void ChangeLanguage()
        {
            OnUpdateLanguage?.Invoke(_cultureCode);
        }

        public void UpdateLanguage(string culture)
        {
            string fontName = Functions.GetCultureFontName(culture);

            Font newFont = new Font(fontName, 12, FontStyle.Bold);
            btnParam.Font = newFont;
            btnSetting.Font = newFont;
            btnSystem.Font = newFont;
            btnClose.Font = newFont;

            newFont = new Font(fontName, 10, FontStyle.Bold);
            lblTitle.Font = newFont;

            btnParam.Text = Lang.btnParam;
            btnSetting.Text = Lang.btnSetting;
            btnSystem.Text = Lang.btnSystem;
            btnClose.Text = Lang.btnClose;

            lblTitle.Text = Lang.mainFormTitle;
        }
        #endregion

       
    }
}
