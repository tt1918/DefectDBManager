
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
    public partial class FormMonitorSelectedLot : Form
    {
        #region Param
        DefectDBManager.PreprocLotManager _lotManager = null;
        DefectDBManager.Preproc.eProc _procIdx = DefectDBManager.Preproc.eProc.Selected;

        public DefectDBManager.CompPreprocDefect Process = null;

        private DefectDBManager.Preproc.LotSelProcParam _param;


        public bool IsRun
        {
            get { return _timerLotSearchProcess.Enabled; }
        }
        #endregion

        #region Event
        public event MarkCompare.Delegate.UpdatePrepLncdInfo OnUpdatePrepLncdInfo;
        public event MarkCompare.Delegate.UpdateEvent OnSearchSelectedLot;
        public event MarkCompare.Delegate.UpdateEvent OnStopLotSearch;
        public event Delegate.UpdateEvent OnOpenSelectLotForm;
        #endregion

        #region Create/Destroy
        public FormMonitorSelectedLot()
        {
            InitializeComponent();
        }

        public FormMonitorSelectedLot(DefectDBManager.PreprocLotManager lotManager, DefectDBManager.Preproc.LotSelProcParam param)
        {
            InitializeComponent();
            _lotManager = lotManager;
            _param = param;
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
                displayFilterData();
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
        private void displayFilterData()
        {
            int ctrlCount = _param.UserFilter.Count;
            bool isError = false;
            bool isSkip = false;
            List<string> strError = new List<string>();

            try
            {
                lvFilterList.BeginUpdate();
                lvFilterList.Items.Clear();
                if(_param.FilterType == DefectDBManager.Preproc.FilterType.UserFilter)
                {
                    foreach (var item in _param.UserFilter.Data)
                    {
                        bool isExistProd = false;
                        bool isExistModel = false;
                        isSkip = false;
                        foreach (var procInfo in _lotManager.ProcLNCD.Info)
                        {
                            if (procInfo.Name == item.Line && procInfo.Material.Items.Contains(item.Product))
                            {
                                if (procInfo.CheckStatus == true)
                                    isSkip = true;
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

                        if (isError == true || isSkip == true)
                            continue;

                        string format = $"{item.Line} - {Lang.product}:[{item.Product}], {Lang.filterDgvModel}:[{item.Model}]";
                        ListViewItem lvi = new ListViewItem(format);
                        lvFilterList.Items.Add(lvi);
                    }
                }
                else if(_param.FilterType == DefectDBManager.Preproc.FilterType.DbFilter)
                {
                    string format = $"DB Filter [{_param.DBFilter.Title}] [MKCD: {_param.DBFilter.MKCD}], " +
                        $"[ES: {_param.DBFilter.UseES.ToString()}], " +
                        $"[TG: {_param.DBFilter.UseTG.ToString()}]," +
                        $"[EtC: {_param.DBFilter.UseETC.ToString()}]";
                    ListViewItem listViewItem = new ListViewItem(format);
                    lvFilterList.Items.Add(listViewItem);
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

        private void displayLotList(List<string> lot)
        {
            try
            {
               lvLotList.BeginUpdate();
                lvLotList.Items.Clear();

                foreach (var item in lot)
                {
                    ListViewItem listViewItem = new ListViewItem(item);
                    lvLotList.Items.Add(item);
                }
            }
            catch
            {

            }
            finally
            {
                lvLotList.EndUpdate();
            }
        }

        public void DisplayLNCDCtrlData(List<string> listLot)
        {
            displayFilterData();
            displayLotList(listLot);
        }

        #endregion

        #region Control
        private void btnStart_Click(object sender, EventArgs e)
        {
            try
            {

                // 현재 검사 중이거나 랏메니저가 null 이면 검사 안 함.
                if (IsRun == true || _lotManager == null)
                {
                    SystemLog.DisplayFileServerLog(Lang.theSearingIsInProgress);
                    return;
                }

                // 모델에 에러가 있으면 실행 안 함.
                if (_isModelError==true)
                {
                    SystemLog.DisplayFileServerLog(Lang.noFilterData);
                    return;
                }

                if (Process.IsRunSearchingLotList)
                {
                    SystemLog.DisplayFileServerLog(Lang.theSearingIsInProgress);
                    Invoke(new Action(() =>
                    {
                        MessageBox.Show(this, Lang.theSearingIsInProgress);
                    }));
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

                OnUpdatePrepLncdInfo?.Invoke(DefectDBManager.Preproc.eProc.Selected);

                OnSearchSelectedLot?.Invoke();
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
            int total = _lotManager.Selected.TotalLot;
            int count = _lotManager.Selected.TotalProduct;

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
        
        #region 언어 변경
        public void UpdateLanguage(string culture)
        {
            this.BeginInvoke(new Action(() =>
            {
                string fontName = Functions.GetCultureFontName(culture);

                Font newFont = new Font(fontName, 9, FontStyle.Bold);
               

                newFont = new Font(fontName, 9, FontStyle.Regular);
                btnStart.Font = newFont;
                btnStop.Font = newFont;

                btnStart.Text = Lang.start;
                btnStop.Text = Lang.ProcStop;
            }));
        }
        #endregion

        private void btnSetLot_Click(object sender, EventArgs e)
        {
            OnOpenSelectLotForm?.Invoke();
        }
    }
}
