using DefectDBManager;
using MarkCompare.Delegate;
using MarkCompare.Helper;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Net.Mime.MediaTypeNames;

namespace MarkCompare
{
    public enum eSummaryMode { Monitoring, LiveErrorCheck }
    public partial class FormLotSummaryData : Form
    {
        public event UpdateSummaryCheck OnClickSummaryCheck;

        public DefectDBManager.PreprocLot LotSummery
        {
            get { return _lotSummery; }
            set
            {
                _lotSummery = value;
                if (this.InvokeRequired)
                {
                    this.BeginInvoke(new Action(()=>
                    {
                        if (this.Visible == false) this.Visible = true;
                        displayLotSummery();
                    }));
                }
                else
                {
                    if (this.Visible == false) this.Visible = true;
                    displayLotSummery();
                }
            }
        }
        /// <summary>
        /// 상위 검사 랏 서머리 정보 
        /// </summary>
        private DefectDBManager.PreprocLot _lotSummery = new DefectDBManager.PreprocLot();
        private eSummaryMode _mode = eSummaryMode.Monitoring;

        public DefectDBManager.Preproc.PreprocItem ProcItem
        {
            get { return procItem; }
            set
            {
                procItem = value;
            }
        }
        //private DefectDBManager.PreprocLotManager _lotManager = new DefectDBManager.PreprocLotManager();
        private DefectDBManager.Preproc.PreprocItem procItem = new DefectDBManager.Preproc.PreprocItem();

        bool isError = false;
        public bool IsError 
        {
            get { return isError; }
            set { isError = value; }
        }

        string filter = null;
        public string Filter 
        {
            get { return filter; }
            set { filter = value; }
        }

        public bool ShowCheckbox
        {
            get { return this.cbViewSelect.Visible; }
            set { this.cbViewSelect.Visible = value; }
        }

        public bool StateCheckbox
        {
            get { return this.cbViewSelect.Checked; }
            set { this.cbViewSelect.Checked = value; }
        }

        #region Form
        public FormLotSummaryData()
        {
            InitializeComponent();
        }

        public FormLotSummaryData(DefectDBManager.PreprocLot lotSummery)
        {
            InitializeComponent();

            _lotSummery = lotSummery;
        }

        private void FormLotSummaryData_Load(object sender, EventArgs e)
        {
            UpdateLanguage();
        }

        private void FormLotSummaryData_VisibleChanged(object sender, EventArgs e)
        {

        }
        private void FormLotSummaryData_FormClosing(object sender, FormClosingEventArgs e)
        {
            closeStatusTimer();
        }
        #endregion

        #region 데이터 표시
        private void displayLotSummery()
        {
            displayLotName();
            displayDetail();
            displayCompareResult();
        }

        private void displayCompareResult()
        {
            try
            {
                if(_lotSummery==null)
                {
                    UIHelper.SetText(lblStatus, Lang.error);
                    UIHelper.SetBackColor(lblStatus, Color.Black);
                    UIHelper.SetForeColor(lblStatus, Color.White);

                    this.BeginInvoke(new Action(() =>
                    {
                        this.BackColor = Color.WhiteSmoke;
                        SystemLog.DisplaySystemLog(Lang.LotSummaryIsEmpty, Log.Level.Error);
                    }));
                    
                    return;
                }

                if (_lotSummery.MarkCompList == null || _lotSummery.MarkCompList.Data.Count <= 0)
                {
                    UIHelper.SetText(lblStatus, Lang.error);
                    UIHelper.SetBackColor(lblStatus, Color.Black);
                    UIHelper.SetForeColor(lblStatus, Color.White);

                    this.BeginInvoke(new Action(() =>
                    {
                        this.BackColor = Color.WhiteSmoke;
                    }));
                    return;
                }

                if (isError)
                {
                    this.BeginInvoke(new Action(() =>
                    {
                        lblStatus.BkColor = Color.Red;
                        this.BackColor = Color.Pink;
                        lblStatus.Text = Lang.ErrorOccurrence;
                    }));
                }
                else
                {
                    this.BeginInvoke(new Action(() =>
                    {
                        lblStatus.BkColor = Color.MidnightBlue;
                        this.BackColor = Color.WhiteSmoke;
                        lblStatus.Text = Lang.Normal;
                    }));
                }
            }
            catch (Exception ex)
            {
                SystemLog.DisplaySystemLog($"{Lang.lotSummaryCompResult}:{ex.Message}", Log.Level.Error);
            }
        }

        private void displayLotName()
        {
            try
            {
                string str = _lotSummery.LotName;
                lblLotName.Text = str + $" - {filter}";

                if (isError)
                    lblLotName.BkColor = Color.Red;
                else
                    lblLotName.BkColor = Color.MidnightBlue;
            }
            catch(Exception ex)
            {
                SystemLog.DisplaySystemLog($"{Lang.lotSummaryLotName} :{ex.Message}", Log.Level.Error);
            }
            
        }

        private void displayDetail()
        {
            try
            {
                if (_lotSummery == null)
                {
                    UIHelper.SetText(lblProcess, Lang.NoLotSummaryData);
                    SystemLog.DisplaySystemLog($"Show Detail: {Lang.NoLotSummaryData}", Log.Level.Error);
                    return;
                }

                if (_lotSummery.MarkCompList == null || _lotSummery.MarkCompList.Data.Count <= 0)
                {
                    UIHelper.SetText(lblProcess, Lang.NoComparingData);
                    return;
                }

                string str = null;
                int[,] compCnt = _lotSummery.CompCnt;

                bool isEmpty = true;
                foreach (var cnt in compCnt)
                    if (cnt > 0) isEmpty = false;

                if (isEmpty)
                {
                    UIHelper.SetText(lblProcess, Lang.NoComparingData);
                    return;
                }

                double[] result = new double[procItem.CompRange.Count + 1];

                StringBuilder sb = new StringBuilder();

                for (int idx = 0; idx < procItem.Compare.Count; idx++)
                {
                    if (idx > 0) sb.Append("\n");
                    isEmpty = true;
                    for (int i = 0; i < compCnt.GetLength(1); i++)
                        if (compCnt[idx, i] > 0) isEmpty = false;
                    if (isEmpty)
                    {
                        sb.Append($"[{procItem.Reference.LNCD}-{procItem.Compare[idx].LNCD}] {Lang.NoComparingData}");
                        continue;
                    }

                    result[0] = 100.0;
                    if (compCnt[idx, 0] == 0) sb.Append($"[{procItem.Reference.LNCD}-{procItem.Compare[idx].LNCD}] REF :0%({compCnt[idx, 0]}), ");
                    else sb.Append($"[{procItem.Reference.LNCD}-{procItem.Compare[idx].LNCD}] REF :{result[0]:F1}%({compCnt[idx, 0]}), ");

                    for (int i = 1; i < procItem.CompRange.Count + 1; i++)
                    {
                        if (compCnt[idx, 0] > 0)
                        {
                            result[i] = (double)((double)compCnt[idx, i] / (double)compCnt[idx, 0]) * 100.0;
                            sb.Append($"Case {i} : {result[i]:F1}%({compCnt[idx, i]})");
                        }
                        else
                        {
                            if (compCnt[idx, i] > 0)
                            {
                                result[i] = (double)compCnt[idx, i] * 100.0;
                                sb.Append($"Case {i} : {result[i]:F1}%({compCnt[idx, i]})");
                            }
                            else
                                sb.Append($"Case {i} : 0%({compCnt[idx, i]})");
                        }

                        if (i < procItem.CompRange.Count) sb.Append(", ");

                        if (Math.Abs(result[i] - result[i - 1]) > procItem.CompRange[i - 1].Accuracy)
                            isError = true;
                    }
                }

                UIHelper.SetText(lblProcess, sb.ToString());
            }
            catch (Exception ex)
            {
                SystemLog.DisplaySystemLog($"{Lang.LotSummaryShowDetail} :{ex.Message}", Log.Level.Error);
            }
        }

        /// <summary>
        /// 지정 경로 Status.txt 파일의 상태 표시 
        /// </summary>
        public void SetStatusAlarm()
        {

        }
        #endregion

        #region Status.txt 상태 표시
        System.Windows.Forms.Timer _timerStatus = null;
        private readonly SemaphoreSlim _statusLock = new SemaphoreSlim(1, 1);
        public string TargetIP
        {
            get { return _targetIP; }
            private set { _targetIP = value; }
        }
        string _targetIP = string.Empty;
        int _duration = 60 * 1000;
        string _lineName = string.Empty;

        bool _isCheckStatus = false;

        public void SetStatusCheck(string lineName, string ip, int min)
        {
            _mode = eSummaryMode.LiveErrorCheck;
            _targetIP = ip;
            _duration = 60 * min * 1000;
            _lineName = lineName;
            displayLineName();
            initStatusTimer();
        }

        private void initStatusTimer()
        {
            closeStatusTimer();

            _timerStatus = new System.Windows.Forms.Timer();
            _timerStatus.Interval = _duration;
            _timerStatus.Tick += new EventHandler(timerDisplayStatus);
            
            _timerStatus.Start();

            Task.Factory.StartNew(state => timerDisplayStatus(state, EventArgs.Empty), null);
        }

        private void closeStatusTimer()
        {
            if (_timerStatus != null)
            {
                _timerStatus.Stop();
                _timerStatus.Dispose();
                _timerStatus = null;
            }
        }

        

        private async void timerDisplayStatus(object sender, EventArgs e)
        {
            if (!await _statusLock.WaitAsync(0)) return;

            try
            {
                string result = await Task.Run(() =>
                {
                    var sb = new StringBuilder();
                    var sbFiles = new StringBuilder();
                    //var paths = new List<string>
                    //{
                    //    //Path.Combine($"\\\\{_targetIP}", "COSS\\Status"),
                    //    //Path.Combine($"\\\\{_targetIP}", "nexteye\\Status")
                    //    Path.Combine(_targetIP, "Status")
                    //};

                    //foreach (var path in paths)
                    {
                        if (Directory.Exists(_targetIP))
                        {
                            var files = Directory.GetFiles(_targetIP, "*Status.txt", System.IO.SearchOption.TopDirectoryOnly);
                            foreach (var file in files)
                            {
                                try
                                {
                                    sbFiles.Append(file+" ");
                                    foreach (var line in File.ReadLines(file, Encoding.Default))
                                    {
                                        char[] separators = new char[] { ',' };
                                        var texts = line.Split(separators, StringSplitOptions.RemoveEmptyEntries);
                                        for (int i = 0; i < texts.Length; i += 2)
                                        {
                                            sb.Append(texts[i]);
                                            if (i + 1 < texts.Length)
                                                sb.Append(", ").Append(texts[i + 1]);
                                            sb.AppendLine();
                                        }
                                    }
                                }
                                catch (Exception ex)
                                {
                                    sb.AppendLine($"파일 읽기 실패: {file}, 이유: {ex.Message}");
                                }
                            }
                        }
                    }

                    if (sb.Length == 0)
                        sb.Append(Lang.CanNotFindStatusTxtFile);


                    // UI 갱신은 UI 스레드에서
                    UIHelper.SetText(lblProcess, sb.ToString());
                    if(sbFiles.Length>0)    UIHelper.SetText(lblLotName, sbFiles.ToString());
                   
                    return sb.ToString();
                });

            }
            catch (Exception ex)
            {
                SystemLog.DisplaySystemLog($"{Lang.StatusCheck}: {ex.Message}", Log.Level.Error);
            }
            finally
            {
                _statusLock.Release(); // 작업 종료 후 잠금 해제
            }
        }

        private void displayLineName()
        {
            lblStatus.Text = _lineName;
            lblLotName.Text = _targetIP;
            lblProcess.Text = "";
        }
        #endregion

        private void lblLotName_DoubleClick(object sender, EventArgs e)
        {
            if (_mode == eSummaryMode.LiveErrorCheck) return;

            ((FormLotList)this.ParentForm).DoubleClickSummaryData(this);
        }

        #region 언어 변경
        public void UpdateLanguage()
        {
            CultureInfo culture = CultureInfo.CurrentCulture;

            string fontName = Functions.GetCultureFontName(culture.Name);

            Font newFont = new Font(fontName, 10, FontStyle.Bold);
            lblStatus.Font = newFont;
            lblLotName.Font = newFont;

            newFont = new Font(fontName, 9, FontStyle.Bold);
            lblProcess.Font = newFont;
        }
        #endregion

        private void cbViewSelect_CheckedChanged(object sender, EventArgs e)
        {
            OnClickSummaryCheck?.Invoke(this);
        }
    }
}
