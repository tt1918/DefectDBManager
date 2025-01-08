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
        DefectDBManager.Preproc.eProc _procIdx = DefectDBManager.Preproc.eProc.Live;
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
        
        private void initLNCDCtrl()
        {
            if (_lotManager == null) return;

        }

        /// <summary>
        /// _lotManager.ProcLNCD.Info -> tlLncd
        /// </summary>
        private void setLNCDCtrlData()
        {

            int ctrlCount = _lotManager.CrtProcFilter[(int)_procIdx].Count;

            try
            {
                lvFilterList.BeginUpdate();
                lvFilterList.Items.Clear();
                foreach (var item in _lotManager.CrtProcFilter[(int)_procIdx].Data)
                {
                    string format = $"{item.Line} - Product:[{item.Product}], Model:[{item.Model}]";
                    ListViewItem lvi = new ListViewItem(format);
                    lvFilterList.Items.Add(lvi);
                }
            }
            catch
            {

            }
            finally
            {
                lvFilterList.EndUpdate();
            }
        }

        public void DisplayLNCDCtrlData()
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
                OnUpdatePrepLncdInfo?.Invoke(_procIdx);
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
                using (FormProductFilter form = new FormProductFilter(_lotManager, _procIdx))
                {
                    if(form.ShowDialog()==DialogResult.OK)
                    {
                        setLNCDCtrlData();
                        OnUpdatePrepLncdInfo?.Invoke(eProc.Live);
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
