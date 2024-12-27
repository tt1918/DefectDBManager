using Coss.Controls;
using DefectDBManager;
using System;
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
            initClockTimer();
            initMarkDiffForm();

            initLiveSearchTimer();

            SystemLog.DisplaySystemLog = _markDiffForm.OnDisplaySystemLog;
            SystemLog.DisplayFileServerLog = _markDiffForm.OnDisplayFileServerLog;
            SystemLog.OnDisplayLogData = _markDiffForm.OnDisplayLog;
            SystemLog.DisplayNetworkLog = _markDiffForm.OnDisplayNetworkLog;
            SystemLog.DisplayAlarmLog = _markDiffForm.OnDisplayAlarmLog;

            _dbManager.OnEndSearchingLotList += EndSearchLotList;
            _dbManager.OnEndLiveSearchLot += EndLiveSearch;


            SystemLog.DisplaySystemLog("Program Start");
        }

        private void FormMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            _dbManager.OnEndSearchingLotList -= EndSearchLotList;
            _dbManager.OnEndLiveSearchLot -= EndLiveSearch;

            destroyMarkDiffForm();
        }

        #region Marking Comparision Form
        private FormMarkDiff _markDiffForm;

        private void initMarkDiffForm()
        {
            _markDiffForm = new FormMarkDiff(_lotManager);
            _markDiffForm.TopLevel = false;

            tableLayoutPanel1.Controls.Add(_markDiffForm, 0, 1);
            _markDiffForm.Dock = DockStyle.Fill;
            _markDiffForm.Show();

            _markDiffForm.FormMorSearch.OnStartLotSearch += StartSearchLotList;
            _markDiffForm.FormMorSearch.OnStopLotSearch += StopSearchLotList;

            _markDiffForm.FormMorLive.OnStartLiveSearch += StartLiveSearch;
            _markDiffForm.FormMorLive.OnStopLiveSearch += StopLiveSearch;

            _dbManager.OnEndSearchingLotList += _markDiffForm.FormMorSearch.EndLotSearch;
            _dbManager.OnEndLiveSearchLot += _markDiffForm.FormMorLive.EndLotSearch;

        }

        private void destroyMarkDiffForm()
        {
            _markDiffForm.FormMorSearch.OnStartLotSearch -= StartSearchLotList;
            _markDiffForm.FormMorSearch.OnStopLotSearch -= StopSearchLotList;

            _markDiffForm.FormMorLive.OnStartLiveSearch -= StartLiveSearch;
            _markDiffForm.FormMorLive.OnStopLiveSearch -= StopLiveSearch;

            _dbManager.OnEndSearchingLotList -= _markDiffForm.FormMorSearch.EndLotSearch;
            _dbManager.OnEndLiveSearchLot -= _markDiffForm.FormMorLive.EndLotSearch;
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
        private void btnClose_Click(object sender, EventArgs e)
        {
            string msg = "프로그램을 종료하시겠습니까?";
            
            if (MessageBox.Show(msg, "CLOSE", MessageBoxButtons.OKCancel) != DialogResult.OK)
                return;

            this.Close();
        }

        private void btnMinimize_Click(object sender, EventArgs e)
        {
            this.WindowState = FormWindowState.Minimized;
        }
        #endregion CONTROL

        private void btnSitting_Click(object sender, EventArgs e)
        {
            FormSetting form = new FormSetting(this._lotManager.ProcSetting);
            if(form.ShowDialog()==DialogResult.OK)
                this._lotManager.UpdatePreprocSet(form.PreprocSet);

        }

        #region MyRegion

        #endregion

        #region 기간 검색 시작
        public void StartSearchLotList()
        {
            _dbManager.SearchLotMarkDiffFromSetting();
        }

        public void StopSearchLotList()
        {
            _dbManager.StopSearchingLotList = false;
        }

        public void EndSearchLotList()
        {
            SystemLog.DisplaySystemLog("기간 검색 작업이 완료되었습니다.");
        }
        #endregion

        #region 실시간 검색 시작

        Timer _timerLiveSearch = null;

        private void initLiveSearchTimer()
        {
            _timerLiveSearch = new Timer();
            _timerLiveSearch.Interval = 60*60*1000;
            _timerLiveSearch.Tick += new EventHandler(timerLiveSearch);
        }

        private void timerLiveSearch(object sender, EventArgs e)
        {
            if (_dbManager.IsRunSearchingLotList == true || _dbManager.IsRunLiveSearch == true)
                _timerLiveSearch.Interval = 5 * 60 * 1000;  // 5분 후에 다시 검색 한다. 
            
            _dbManager.SearchLotMarkDiff();

            // 검사가 시작되었으면 검색 시간을 1시간으로 변경한다. 
            _timerLiveSearch.Interval = 60 * 60 * 1000;
        }

        public void StartLiveSearch()
        {
            _timerLiveSearch.Start();
        }

        public void StopLiveSearch()
        {
            _dbManager.StopLiveSearch = true;
            _timerLiveSearch.Stop();
        }

        public void EndLiveSearch()
        {
            SystemLog.DisplaySystemLog("실시간 검사가 완료되었습니다.");
        }
        #endregion

    }
}
