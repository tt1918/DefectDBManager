
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
using static System.Net.Mime.MediaTypeNames;



namespace MarkrCompare
{
    public partial class FormMornitorSearch : Form
    {
        #region Param
        DefectDBManager.PreprocLotManager _lotManager = null;
        DefectDBManager.Preproc.eProc _procIdx = DefectDBManager.Preproc.eProc.Search;
        public bool IsRun
        {
            get { return _timerLotSearchProcess.Enabled; }
        }
        #endregion

        #region Event
        public event MarkrCompare.Delegate.UpdatePrepLncdInfo OnUpdatePrepLncdInfo;
        public event MarkrCompare.Delegate.UpdateEvent OnStartLotSearch;
        public event MarkrCompare.Delegate.UpdateEvent OnStopLotSearch;
        public event Delegate.UpdateEvent OnOpenCsvForm;
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
                        strError.Add($"라인 : {item.Line}, 품종 : {item.Product} 정보가 존재하지 않습니다.");
                        isError = true;
                    }
                    if (isExistModel == false)
                    {
                        strError.Add($"모델 : {item.Model} 정보가 존재하지 않습니다.");
                        isError = true;
                    }

                    if (isError == true)
                        continue;

                    string format = $"{item.Line} - Product:[{item.Product}], Model:[{item.Model}]";
                    ListViewItem lvi = new ListViewItem(format);
                    lvFilterList.Items.Add(lvi);
                }

                _isModelError = isError;

                if (isError == true)
                {
                    if (strError.Count > 0)
                    {
                        strError.Insert(0, "수동 감시");
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
                    SystemLog.DisplayFileServerLog("검사 모델 데이터가 존재하지 않습니다.");
                    return;
                }

                if (IsRun == true || _lotManager == null)
                {
                    SystemLog.DisplayFileServerLog("검사 진행중 입니다.");
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
                case CompProc.Proc: message = $"데이터 처리 중... [{count} / {total}]";   break;
                case CompProc.Stop: message = $"검사 중지 [{count} / {total}]";           break;
                case CompProc.End:  message = $"데이터 처리 완료 [{count} / {total}]";    break;
                case CompProc.None: message = $"대기";                                    break;
            }

            if (lblProcess.InvokeRequired)
            {
                lblProcess.BeginInvoke(new Action(() =>
                {
                    lblProcess.Text = message;
                }));
            }
            else
                lblProcess.Text = message;
        }
        #endregion

        private void btnCsv_Click(object sender, EventArgs e)
        {
            OnOpenCsvForm?.Invoke();
        }
    }
}
