
using CustomControls;
using MarkCompare.Helper;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Net.Mime.MediaTypeNames;



namespace MarkCompare
{
    public partial class FormMonitorSearch : Form
    {
        #region Param
        DefectDBManager.PreprocLotManager _lotManager = null;
        DefectDBManager.Preproc.eProc _procIdx = DefectDBManager.Preproc.eProc.Search;

        public DefectDBManager.CompPreprocDefect Process = null;

        public bool IsRun
        {
            get { return _timerLotSearchProcess.Enabled; }
        }
        #endregion

        #region Event
        public event MarkCompare.Delegate.UpdatePrepLncdInfo OnUpdatePrepLncdInfo;
        public event MarkCompare.Delegate.UpdateEvent OnStartLotSearch;
        public event MarkCompare.Delegate.UpdateEvent OnStopLotSearch;
        public event Delegate.UpdateEvent OnOpenCsvForm;
        #endregion

        #region Create/Destroy
        public FormMonitorSearch()
        {
            InitializeComponent();
        }

        public FormMonitorSearch(DefectDBManager.PreprocLotManager lotManager)
        {
            InitializeComponent();
            _lotManager = lotManager;
        }
        private void FormMornitorSearch_Load(object sender, EventArgs e)
        {
            initLNCDCtrl();
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
        
        #endregion

        #region 체크 버튼 인식
        List<CheckBox> _lncdCheckBox = null;

        bool _isModelError = false;

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
            bool isError = false;
            List<string> strError = new List<string>();

            try
            {
                lvFilterList.BeginUpdate();
                lvFilterList.Items.Clear();
                foreach (var item in _lotManager.CrtProcFilter[(int)_procIdx].Data)
                {
                    bool isExistProd = false;
                    bool isExistModel = false;
                    foreach (var procInfo in _lotManager.ProcLNCD.Info)
                    {
                        if (procInfo.Name == item.Line && procInfo.Material.Items.Contains(item.Product))
                        {
                            isExistProd = true;
                            break;
                        }
                    }

                    foreach (var prodModel in _lotManager.ProcSetting.Data)
                    {
                        if (prodModel.Name == item.Model)
                        {
                            isExistModel = true;
                            break;
                        }
                    }

                    if (isExistProd == false)
                    {
                        strError.Add($"Line : {item.Line}, {Lang.product} : {item.Product} {Lang.InformationDoesNotExist}");
                        isError = true;
                    }
                    if (isExistModel == false)
                    {
                        strError.Add($"{Lang.dgvMeterialModel} : {item.Model} {Lang.InformationDoesNotExist}");
                        isError = true;
                    }

                    if (isError == true)
                        continue;

                    string format = $"{item.Line} - {Lang.product}:[{item.Product}], {Lang.filterDgvModel}:[{item.Model}]";
                    ListViewItem lvi = new ListViewItem(format);
                    lvFilterList.Items.Add(lvi);
                }

                _isModelError = isError;

                if (isError == true)
                {
                    if (strError.Count > 0)
                    {
                        strError.Insert(0, Lang.SelectiveMonitoring);
                        var errorMessage = string.Join("\n", strError.Select((error, index) => $"{index + 1}. {error}"));
                        MessageBox.Show(errorMessage, "Error List", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    }
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
                if(_isModelError==true)
                {
                    SystemLog.DisplayFileServerLog(Lang.noFilterData);
                    return;
                }

                if (IsRun == true || _lotManager == null)
                {
                    SystemLog.DisplayFileServerLog(Lang.theSearingIsInProgress);
                    return;
                }

                if (Process.IsRunLiveTimer)
                {
                    SystemLog.DisplayFileServerLog(Lang.realtimeExplorationInProgress);
                    Invoke(new Action(() =>
                    {
                        MessageBox.Show(this, Lang.realtimeExplorationInProgress);
                    }));
                    return;
                }

                _lotManager.SearchTime.SetTime(timePickerStart.Value, timePickerEnd.Value);

                OnUpdatePrepLncdInfo?.Invoke(DefectDBManager.Preproc.eProc.Search);

                OnStartLotSearch?.Invoke();
                _timerLotSearchProcess.Start();
            }
            catch (Exception ex)
            {
                SystemLog.DisplaySystemLog(ex.Message, Log.Level.Error);
            }
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            try
            {
                if (IsRun == false) return;
                
                _timerLotSearchProcess.Stop();
                OnStopLotSearch?.Invoke();

                updateLotSearchRes(CompProc.Stop);
            }
            catch (Exception ex)
            {
                SystemLog.DisplaySystemLog(ex.Message, Log.Level.Error);
            }
        }

        private void btnMaterialFilter_Click(object sender, EventArgs e)
        {
            try
            {
                using (FormProductFilter form = new FormProductFilter(_lotManager, _procIdx))
                {
                    if (form.ShowDialog() == DialogResult.OK)
                    {
                        setLNCDCtrlData();
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
            // 마지막 데이터 업데이트
            updateLotSearchRes(CompProc.End);
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
            updateLotSearchRes();
        }

        private void updateLotSearchRes(CompProc eProc = CompProc.Proc)
        {
            int total = _lotManager.TotalLot;
            int count = _lotManager.TotalProduct;

            string message ="";
            switch(eProc)
            {
                case CompProc.Proc: message = $"{Lang.processingData} [{count} / {total}]"; break;
                case CompProc.Stop: message = $"{Lang.searchingDataCancel} [{count} / {total}]"; break;
                case CompProc.End: message = $"{Lang.dataProcessingComplete} [{count} / {total}]"; break;
                case CompProc.None: message = Lang.waiting; break;
            }

            UIHelper.SetText(lblProcess, message);
        }
        #endregion

        private void btnCsv_Click(object sender, EventArgs e)
        {
            OnOpenCsvForm?.Invoke();
        }

        #region 언어 변경
        public void UpdateLanguage(string culture)
        {
            this.BeginInvoke(new Action(() =>
            {
                string fontName = Functions.GetCultureFontName(culture);

                Font newFont = new Font(fontName, 9, FontStyle.Bold);
                lblTime.Font = newFont;
                lblTime.Text = Lang.time;

                newFont = new Font(fontName, 9, FontStyle.Regular);
                btnStart.Font = newFont;
                btnStop.Font = newFont;
                btnMaterialFilter.Font = newFont;

                btnStart.Text = Lang.start;
                btnStop.Text = Lang.ProcStop;
                btnMaterialFilter.Text = Lang.filterSet;
            }));
        }
        #endregion
    }
}
