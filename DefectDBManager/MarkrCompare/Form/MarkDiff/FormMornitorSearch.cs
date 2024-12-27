#define USE_PRE_SETTING

using CustomControls;
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
    public partial class FormMornitorSearch : Form
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
        public event MarkrCompare.Delegate.UpdateEvent OnStartLotSearch;
        public event MarkrCompare.Delegate.UpdateEvent OnStopLotSearch;
        #endregion

        #region Create/Destroy
        public FormMornitorSearch()
        {
            InitializeComponent();
        }

        public FormMornitorSearch(DefectDBManager.PreprocLotManager lotManager)
        {
            InitializeComponent();
            _lotManager = lotManager;
        }
        private void FormMornitorSearch_Load(object sender, EventArgs e)
        {
            initCBSetting();
            //initLNCDCtrl();
            initLotSearchTimer();
        }

        private void FormMornitorSearch_FormClosing(object sender, FormClosingEventArgs e)
        {

        }

        private void FormMornitorSearch_VisibleChanged(object sender, EventArgs e)
        {
            if(this.Visible==true)
            {
                setLNCDCtrlData();
            }
        }

        #endregion

        #region Setting Combo Box 처리
        
        private void initCBSetting()
        {
            if (_lotManager == null) return;

            int size = _lotManager.ProcSetting.Count;

            cbJobList.Items.Clear();
            cbJobList.Sorted = false;
            cbJobList.DropDownStyle = ComboBoxStyle.DropDownList;
            foreach (var item in _lotManager.ProcSetting.Data)
                cbJobList.Items.Add(item.Name);
            
            if(size>0)  cbJobList.SelectedIndex = 0;
        }

        private void getSettingJob()
        {
            string name;
            name = cbJobList.SelectedItem as string;
            _lotManager.SetSelectedJob(name);
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
                checkBox.Checked = item.Use;
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
                    for (int i = 0; i < _lotManager.ProcLNCD.Info.Count; i++)
                    {
                        if (_lotManager.ProcLNCD.Info[i].Name == item.Text)
                        {
                            _lotManager.ProcLNCD.Info[i].Use = item.Checked;
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
                            chk.Checked = item.Use;
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
                if (_lotManager == null) return;

                _lotManager.SearchTime.SetTime(timePickerStart.Value, timePickerEnd.Value);

#if USE_PRE_SETTING
                getSettingJob();
#else
                getLNCDCtrlData();
                OnUpdatePrepLncdInfo?.Invoke();
#endif
                OnStartLotSearch?.Invoke();
                _timerLotSearchProcess.Start();
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
                OnStopLotSearch?.Invoke();

                string message = $"모니터링 정지";
                lblProcess.Text = message;
            }
            catch
            {

            }
        }

        private void btnMaterialFilter_Click(object sender, EventArgs e)
        {

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

            string message = $"데이터 처리 중... ({count} / {total})";
            lblProcess.Text = message;
        }
        #endregion
    }
}
