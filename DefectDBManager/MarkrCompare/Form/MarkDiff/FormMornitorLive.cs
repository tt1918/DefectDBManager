using DefectDBManager.Preproc;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.ProgressBar;

namespace MarkrCompare
{
    public partial class FormMornitorLive : Form
    {
        #region Param
        DefectDBManager.PreprocLotManager _lotManager = null;
        public bool IsRun 
        {
            get { return _timerLotSearchProcess.Enabled; }
        }


        #endregion

        #region Event
        public event MarkrCompare.Delegate.UpdatePrepLncdInfo OnUpdatePrepLncdInfo;
        public event MarkrCompare.Delegate.UpdateEvent OnStartLiveSearch;
        public event MarkrCompare.Delegate.UpdateEvent OnStopLiveSearch;
        #endregion

        #region Create/Destroy
        public FormMornitorLive()
        {
            InitializeComponent();
        }

        public FormMornitorLive(DefectDBManager.PreprocLotManager lotManager)
        {
            InitializeComponent();
            _lotManager = lotManager;
        }

        private void FormMornitorLive_Load(object sender, EventArgs e)
        {
            initLNCDCtrl();
            initLotSearchTimer();


        }

        private void FormMornitorLive_FormClosing(object sender, FormClosingEventArgs e)
        {

        }

        private void FormMornitorLive_VisibleChanged(object sender, EventArgs e)
        {
            if(this.Visible==true)
            {
                setLNCDCtrlData();
            }
        }
        #endregion

        #region 체크 버튼 인식
        List<CheckBox> _lncdCheckBox = null;
        private void initLNCDCtrl()
        {
            if (_lotManager == null) return;

            if (_lncdCheckBox != null)
                _lncdCheckBox.Clear();
            if (_lncdCheckBox == null)
                _lncdCheckBox = new List<CheckBox>();

            int size = _lotManager.ProcLNCD.Info.Count;

            tlLncd.Controls.Clear();
            tlLncd.ColumnStyles.Clear();
            tlLncd.RowStyles.Clear();

            tlLncd.Dock = DockStyle.Fill;

            tlLncd.ColumnCount = size;
            tlLncd.RowCount = 1;

            int index = 0;
            tlLncd.RowStyles.Add(new RowStyle(SizeType.AutoSize));

            foreach (var item in _lotManager.ProcLNCD.Info)
            {
                CheckBox checkBox = new CheckBox();
                checkBox.Text = item.Name;
                checkBox.UseVisualStyleBackColor = true;
                checkBox.Checked = item.Use[(int)DefectDBManager.Preproc.eProc.Live];
                checkBox.AutoSize = true;

                tlLncd.ColumnStyles.Add(new ColumnStyle(SizeType.AutoSize));
                tlLncd.Controls.Add(checkBox, index, 0);

                checkBox.Dock = DockStyle.Fill;

                _lncdCheckBox.Add(checkBox);
                index++;
            }
        }

        /// <summary>
        /// tlLncd -> _lotManager.ProcLNCD.Info
        /// </summary>
        private void getLNCDCtrlData()
        {
            try
            {
                tlLncd.SuspendLayout();
                foreach (var item in _lncdCheckBox)
                {
                    _lotManager.SetUse(DefectDBManager.Preproc.eProc.Live, item.Text, item.Checked);
                }
            }
            catch
            {

            }
            finally
            {
                tlLncd.ResumeLayout();
            }

        }

        /// <summary>
        /// _lotManager.ProcLNCD.Info -> tlLncd
        /// </summary>
        private void setLNCDCtrlData()
        {
            if (_lncdCheckBox == null) return;
            int ctrlCount = _lncdCheckBox.Count;

            try
            {
                tlLncd.SuspendLayout();
                foreach (var item in _lotManager.ProcLNCD.Info)
                {
                    for (int i = 0; i < ctrlCount; i++)
                    {
                        CheckBox chk = _lncdCheckBox[i];
                        if (chk.Text == item.Name)
                        {
                            chk.Checked = item.Use[(int)DefectDBManager.Preproc.eProc.Live];
                            break;
                        }
                    }
                }
            }
            catch
            {

            }
            finally
            {
                tlLncd.ResumeLayout();
            }
            

        }

        public void UpdateLNCDCtrlData()
        {
            setLNCDCtrlData();
        }
        #endregion

        #region Control
        private void btnStart_Click(object sender, EventArgs e)
        {
            try
            {
                if (IsRun == true) return;
                getLNCDCtrlData();
                OnUpdatePrepLncdInfo?.Invoke(DefectDBManager.Preproc.eProc.Live);
                _timerLotSearchProcess.Start();
                OnStartLiveSearch?.Invoke();
            }
            catch
            {

            }
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            try
            {
                if (IsRun == false) return;
                _timerLotSearchProcess.Stop();

                // 검사 정지
                OnStopLiveSearch?.Invoke();

                string message = $"모니터링 정지";
                lblProcess.Text = message;
            }
            catch
            {

            }
        }

        private void btnMaterialFilter_Click(object sender, EventArgs e)
        {
            try
            {
                using (FormProductFilter form = new FormProductFilter(_lotManager, DefectDBManager.Preproc.eProc.Live))
                {
                    if(form.ShowDialog()==DialogResult.OK)
                    {

                    }
                }
            }
            catch
            {

            }
        }
        #endregion

        #region 검색 완료
        public void EndLotSearch()
        {
            // 타이머 종료
            _timerLotSearchProcess.Stop();

        }
        #endregion

        #region Lot 탐색 Timer 
        private Timer _timerLotSearchProcess;

        private void initLotSearchTimer()
        {
            _timerLotSearchProcess = new Timer();
            _timerLotSearchProcess.Interval = 500;
            _timerLotSearchProcess.Tick += new EventHandler(timer_LotSearch);
        }

        private void timer_LotSearch(object sender, EventArgs e)
        {
            int total = _lotManager.TotalLot;
            int count = _lotManager.TotalProduct;

            string message = $"실시간 모니터링 중... ({count} / {total})";
            lblProcess.Text = message;
        }
        #endregion

    }
}
