using DefectDBManager.Preproc;
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
using static System.Windows.Forms.VisualStyles.VisualStyleElement.ProgressBar;

namespace MarkCompare
{
    public partial class FormMonitorLive : Form
    {
        #region Param
        DefectDBManager.PreprocLotManager _lotManager = null;
        DefectDBManager.Preproc.eProc _procIdx = DefectDBManager.Preproc.eProc.Live;

        public DefectDBManager.CompPreprocDefect Process = null;

        public bool IsRun 
        {
            get { return _timerLotSearchProcess.Enabled; }
        }

        private bool _isError = false;

        #endregion

        #region Event
        public event MarkCompare.Delegate.UpdatePrepLncdInfo OnUpdatePrepLncdInfo;
        public event MarkCompare.Delegate.UpdateEvent OnStartLiveSearch;
        public event MarkCompare.Delegate.UpdateEvent OnStopLiveSearch;
        #endregion

        #region Create/Destroy
        public FormMonitorLive()
        {
            InitializeComponent();
        }

        public FormMonitorLive(DefectDBManager.PreprocLotManager lotManager)
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
            bool isError = false;
            bool isSkip = false;
            List<string> strError=new List<string>();
            try
            {
                lvFilterList.BeginUpdate();
                lvFilterList.Items.Clear();
                foreach (var item in _lotManager.CrtProcFilter[(int)_procIdx].Data)
                {
                    bool isExistProd=false;
                    bool isExistModel = false;
                    isSkip = false;
                    foreach (var procInfo in _lotManager.ProcLNCD.Info)
                    {
                        if(procInfo.Name == item.Line && procInfo.Material.Items.Contains(item.Product))
                        {
                            if (procInfo.CheckStatus == true)
                                isSkip = true;
                            isExistProd = true;
                            break;
                        }    
                    }

                    foreach(var prodModel in _lotManager.ProcSetting.Data)
                    {
                        if(prodModel.Name == item.Model)
                        {
                            isExistModel = true;
                            break;
                        }
                    }

                    if (isExistProd == false && isSkip == false)
                    {
                        strError.Add($"Line : {item.Line}, {Lang.product} : {item.Product} {Lang.InformationDoesNotExist}");
                        isError = true;
                    }
                    if (isExistModel == false && isSkip == false)
                    {
                        strError.Add($"{Lang.dgvMeterialModel} : {item.Model} {Lang.InformationDoesNotExist}");
                        isError = true;
                    }

                    if(isError==true || isSkip == true)
                        continue;

                    string format = $"{item.Line} - {Lang.filterDgvProdName}:[{item.Product}], {Lang.dgvMeterialModel}:[{item.Model}]";
                    ListViewItem lvi = new ListViewItem(format);
                    lvFilterList.Items.Add(lvi);
                }

                _isError = isError;

                if (isError ==true)
                {
                    if (strError.Count > 0)
                    {
                        strError.Insert(0, Lang.ProcLiveSearch);
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
                if(_isError==true)
                {
                    SystemLog.DisplayFileServerLog(Lang.noFilterData);
                    return;
                }

                if (IsRun == true)
                {
                    SystemLog.DisplayFileServerLog(Lang.theSearingIsInProgress);
                    return;
                }

                if(Process.IsRunSearchingLotList)
                {
                    SystemLog.DisplayFileServerLog(Lang.ConditionalLotSearchIsRunning);
                    Invoke(new Action(() =>
                    {
                        MessageBox.Show(this, Lang.ConditionalLotSearchIsRunning);
                    }));
                    return;
                }

                OnUpdatePrepLncdInfo?.Invoke(_procIdx);
                _timerLotSearchProcess.Start();
                OnStartLiveSearch?.Invoke();
                SystemLog.DisplayFileServerLog(Lang.startRealtimeExploration);
            }
            catch
            {

            }
        }

        private void btnStop_Click(object sender, EventArgs e)
        {
            try
            {
                if (IsRun == false)
                {
                    SystemLog.DisplayFileServerLog(Lang.RealtimeExplorationIsAlreadyStopped);
                    return;
                }
                SystemLog.DisplayFileServerLog(Lang.StopRealtimeExploration);
                _timerLotSearchProcess.Stop();

                // 검사 정지
                OnStopLiveSearch?.Invoke();

                _compProc = CompProc.Stop;
                updateLotSearchRes(_compProc);
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
                    form.CultureCode = _cultureCode;
                    if (form.ShowDialog()==DialogResult.OK)
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
        public void StartLotSearch()
        {
            // 타이머에서 처음 데이터 업데이트
            _compProc = CompProc.Proc;
            updateLotSearchRes(_compProc);
        }

        public void EndLotSearch()
        {
            // 여기서 timer_LotSearch 종료하면 안됨.
            // 마지막 데이터 업데이트
            _compProc = CompProc.End;
            updateLotSearchRes(_compProc);
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
            updateLotSearchRes(_compProc);
        }

        CompProc _compProc = CompProc.None;
        private void updateLotSearchRes(CompProc eProc)
        {
            int total = _lotManager.Live.TotalProduct;
            int count = _lotManager.Live.TotalLot;

            string message = "";
            switch (eProc)
            {
                case CompProc.Proc: message = $"{Lang.processingData} [{count} / {total}]"; break;
                case CompProc.Stop: message = $"{Lang.searchingDataCancel} [{count} / {total}]"; break;
                case CompProc.End: message = $"{Lang.dataProcessingComplete} [{count} / {total}]"; break;
                case CompProc.None: message = Lang.waiting; break;
            }

            UIHelper.SetText(lblProcess, message);
        }
        #endregion

        #region 언어 변경
        string _cultureCode = "";
        public void UpdateLanguage(string culture)
        {
            _cultureCode = culture;

            this.BeginInvoke(new Action(() =>
            {
                string fontName = Functions.GetCultureFontName(culture);
                Font newFont = new Font(fontName, 9, FontStyle.Regular);
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
