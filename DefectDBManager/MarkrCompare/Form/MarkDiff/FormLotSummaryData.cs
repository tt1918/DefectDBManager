using CustomControls;
using DefectDBManager;
using log4net;
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

        public DefectDBManager.PreprocLotManager LotManager { get; set; } = null;

        public eSummaryMode MODE
        {
            get { return _mode; }
            set { _mode = value; }
        }
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

        bool _isError = false;
        public bool IsError 
        {
            get { return _isError; }
            set { _isError = value; }
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

        public bool IsCSV
        {
            get { return isCSV; }
            set { isCSV = value; }
        }
        private bool isCSV = false;

        #region Form
        public FormLotSummaryData()
        {
            InitializeComponent();
            lblProcess.Hide();
        }

        public FormLotSummaryData(DefectDBManager.PreprocLot lotSummery)
        {
            InitializeComponent();

            _lotSummery = lotSummery;
            lblProcess.Hide();
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
            if (_lotSummery.ProcName != procItem.Name)
            {
                StringBuilder sb1 = new StringBuilder();
                sb1.AppendLine($"Model Name : {procItem.Name}");
                SystemLog.DisplaySystemLog($"{Lang.LotSummaryShowDetail} :{sb1.ToString()}", Log.Level.Error);

                string procName = _lotSummery.ProcName;
                for (int i = 0; i < LotManager.ProcSetting.Count; i++)
                {
                    if (LotManager.ProcSetting[i].Name == procName)
                    {
                        procItem = LotManager.ProcSetting[i];
                        break;
                    }
                }

                StringBuilder sb2 = new StringBuilder();
                sb2.AppendLine($"Changed Model Name : {procItem.Name}");
                SystemLog.DisplaySystemLog($"{Lang.LotSummaryShowDetail} :{sb2.ToString()}", Log.Level.Error);
            }

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

                if (_isError)
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

                if (_isError)
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
            int maxLine = 0;
            int lineCnt = 0;
            try
            {
                _isError = false;
                // 컨트롤 리소스 삭제
                flpResult.Controls.Clear();

                if (_lotSummery == null)
                {
                    flpResult.Controls.Add(makeProcessInfoLabel("No Lot Info", true));
                    SystemLog.DisplaySystemLog($"Show Detail: {Lang.NoLotSummaryData}", Log.Level.Error);
                    
                    this.Height += 18;
                    foreach (Control ctrl in flpResult.Controls)
                        ctrl.Height = flpResult.ClientSize.Height - 20; // 여유 패딩 고려
                    return;
                }

                string str = null;
                int[,] compCnt = _lotSummery.CompCnt;

                List<int[,]> comp1Cnt = _lotSummery.Comp1Cnt;

                bool isEmpty = true;
                if (_lotSummery.MarkCompList != null && _lotSummery.MarkCompList.Data.Count > 0)
                {
                    foreach (var cnt in compCnt)
                        if (cnt > 0) isEmpty = false;
                }

                if (_lotSummery.MarkCompList == null || _lotSummery.MarkCompList.Data.Count <= 0 || isEmpty==true)
                {
                    
                    for (int idx = 0; idx < procItem.Compare.Count; idx++)
                    {
                        lineCnt = 0;
                        StringBuilder sb1 = new StringBuilder();
                        sb1.Append($"[{procItem.Reference.LNCD}-{procItem.Compare[idx].LNCD}]\n"); lineCnt++;

                        foreach (var data in _lotSummery.PTRY0P_Data)
                        {
                            foreach (var subData in data.Data)
                            {
                                if (subData.LNCD == procItem.Compare[idx].LNCD)
                                {
                                    // 생산 시간 입력
                                    sb1.Append($"[ {subData.Y0KKOL}-{subData.Y0KSOL}\n"); lineCnt++;
                                    // 품명 추가
                                    sb1.Append($"{Lang.product}: {subData.Y0ZKNM}\n"); lineCnt++;
                                    // 품명 추가
                                    sb1.Append($"LOT: {subData.Y0KLOT} ]\n"); lineCnt++;
                                }
                            }
                        }
                        sb1.Append($"{Lang.NoComparingData}"); lineCnt++;

                        if (lineCnt > maxLine) maxLine = lineCnt;
                        flpResult.Controls.Add(makeProcessInfoLabel(sb1.ToString(), false));
                    }

                    if (maxLine > 2)
                    {
                        this.Height += (maxLine - 2) * 18;
                        foreach (Control ctrl in flpResult.Controls)
                            ctrl.Height = flpResult.ClientSize.Height - 20; // 여유 패딩 고려
                    }
                    else
                    {
                        this.Height += 18;
                        foreach (Control ctrl in flpResult.Controls)
                            ctrl.Height = flpResult.ClientSize.Height - 20; // 여유 패딩 고려
                    }
                    return;
                }

                

                if (isEmpty)
                {
                    UIHelper.SetText(lblProcess, Lang.NoComparingData);
                    return;
                }

                double[] result = new double[procItem.CompRange.Count + 1];

                for (int idx = 0; idx < procItem.Compare.Count; idx++)
                {
                    
                    if (procItem.Compare[idx].IsSplitCTLNO==false)
                    {
                        bool isSubError = false;
                        StringBuilder sb1 = new StringBuilder();
                        lineCnt = 0;
                        isEmpty = true;
                        for (int i = 0; i < compCnt.GetLength(1); i++)
                            if (compCnt[idx, i] > 0) isEmpty = false;
                        if (isEmpty)
                        {
                            sb1.Append($"[{procItem.Reference.LNCD}-{procItem.Compare[idx].LNCD}]\n{Lang.NoComparingData}");
                            flpResult.Controls.Add(makeProcessInfoLabel(sb1.ToString(), false));
                            continue;
                        }

                        result[0] = 100.0;
                        // 데이터 입력
                        sb1.Append($"[{procItem.Reference.LNCD}-{procItem.Compare[idx].LNCD}]\n");
                        lineCnt++;

                        foreach (var data in _lotSummery.PTRY0P_Data)
                        {
                            foreach (var subData in data.Data)
                            {
                                if (subData.LNCD == procItem.Compare[idx].LNCD)
                                {
                                    // 생산 시간 입력
                                    sb1.Append($"[ {subData.Y0KKOL}-{subData.Y0KSOL}\n"); lineCnt++;
                                    // 품명 추가
                                    sb1.Append($"{Lang.product}: {subData.Y0ZKNM}\n"); lineCnt++;
                                    // 품명 추가
                                    sb1.Append($"LOT: {subData.Y0KLOT} ]\n"); lineCnt++;
                                }
                            }
                        }

                        if (compCnt[idx, 0] == 0) { sb1.Append($"REF :0%({compCnt[idx, 0]})"); lineCnt++; }
                        else { sb1.Append($"REF :{result[0]:F1}%({compCnt[idx, 0]})"); lineCnt++; }

                        if (procItem.CompRange.Count > 0)
                            sb1.Append("\n");

                        for (int i = 1; i < procItem.CompRange.Count + 1; i++)
                        {
                            if (compCnt[idx, 0] > 0)
                            {
                                result[i] = (double)((double)compCnt[idx, i] / (double)compCnt[idx, 0]) * 100.0;
                                sb1.Append($"Case {i} : {result[i]:F1}%({compCnt[idx, i]})");
                                lineCnt++;
                            }
                            else
                            {
                                if (compCnt[idx, i] > 0)
                                {
                                    result[i] = (double)compCnt[idx, i] * 100.0;
                                    sb1.Append($"Case {i} : {result[i]:F1}%({compCnt[idx, i]})");
                                }
                                else
                                {
                                    sb1.Append($"Case {i} : 0%({compCnt[idx, i]})");
                                }
                                lineCnt++;
                            }

                            if (Math.Abs(result[0] - result[i]) > procItem.CompRange[i - 1].Accuracy || isSubError == true)
                            {
                                sb1.Append($" *");
                                isSubError = true;
                            }

                            if (i < procItem.CompRange.Count) { sb1.Append("\n"); lineCnt++; }
                        }

                        if (lineCnt > maxLine) maxLine = lineCnt;

                        flpResult.Controls.Add(makeProcessInfoLabel(sb1.ToString(), isSubError));
                        if (isSubError == true) _isError = true;
                    }
                    else
                    {
                        if(_lotSummery.MarkCompList.CTLNO[idx].Count==0)
                        {
                            lineCnt = 0;
                            StringBuilder sb1 = new StringBuilder();
                            sb1.Append($"[{procItem.Reference.LNCD}-{procItem.Compare[idx].LNCD}]\n"); lineCnt++;

                            foreach (var data in _lotSummery.PTRY0P_Data)
                            {
                                foreach (var subData in data.Data)
                                {
                                    if (subData.LNCD == procItem.Compare[idx].LNCD)
                                    {
                                        // 생산 시간 입력
                                        sb1.Append($"[ {subData.Y0KKOL}-{subData.Y0KSOL}\n"); lineCnt++;
                                        // 품명 추가
                                        sb1.Append($"{Lang.product}: {subData.Y0ZKNM}\n"); lineCnt++;
                                        // 품명 추가
                                        sb1.Append($"LOT: {subData.Y0KLOT} ]\n"); lineCnt++;
                                    }
                                }
                            }
                            sb1.Append($"{Lang.NoComparingData}"); lineCnt++;

                            if (lineCnt > maxLine) maxLine = lineCnt;
                            flpResult.Controls.Add(makeProcessInfoLabel(sb1.ToString(), false));
                        }
                        else
                        {
                            for (int subIdx = 0; subIdx < _lotSummery.MarkCompList.CTLNO[idx].Count; subIdx++)
                            {
                                StringBuilder sb1 = new StringBuilder();
                                lineCnt = 0;
                                bool isSubError = false;
                                isEmpty = true;
                                for (int i = 0; i < compCnt.GetLength(1); i++)
                                    if (comp1Cnt[idx][subIdx, i] > 0) isEmpty = false;
                                if (isEmpty)
                                {
                                    sb1.AppendLine($"[{procItem.Reference.LNCD}-{procItem.Compare[idx].LNCD}]");
                                    sb1.AppendLine($"CTLNO : {_lotSummery.MarkCompList.CTLNO[idx][subIdx]}");
                                    sb1.AppendLine($"{Lang.NoComparingData}");
                                    flpResult.Controls.Add(makeProcessInfoLabel(sb1.ToString(), false));
                                    continue;
                                }

                                result[0] = 100.0;
                                // 데이터 입력
                                sb1.Append($"[{procItem.Reference.LNCD}-{procItem.Compare[idx].LNCD}]\n");
                                lineCnt++;

                                bool isFindCTLNO = false;
                                foreach (var data in _lotSummery.INSPDAT)
                                {
                                    foreach (var subData in data)
                                    {
                                        foreach (var subData2 in subData.Data)
                                        {

                                            if (subData2.LNCD == procItem.Compare[idx].LNCD && subData2.CTLNO == _lotSummery.MarkCompList.CTLNO[idx][subIdx]
                                                && isFindCTLNO == false)
                                            {
                                                // 생산 시간 입력
                                                sb1.AppendLine($"[ {subData2.STRDT}{subData2.STRTM}-{subData2.ENDDT}{subData2.ENDTM}"); lineCnt++;
                                                // 품명 추가
                                                sb1.AppendLine($"{Lang.product}: {subData2.HINMEI}"); lineCnt++;
                                                // LOT 추가
                                                sb1.AppendLine($"LOT: {subData2.LOTNO} ]"); lineCnt++;
                                                // CTLO 추가
                                                sb1.AppendLine($"CTLNO : {_lotSummery.MarkCompList.CTLNO[idx][subIdx]}"); lineCnt++;
                                                isFindCTLNO = true;
                                            }
                                        }
                                    }
                                }

                                if (comp1Cnt[idx][subIdx, 0] == 0) { sb1.Append($"REF :0%({comp1Cnt[idx][subIdx, 0]})"); lineCnt++; }
                                else { sb1.Append($"REF :{result[0]:F1}%({comp1Cnt[idx][subIdx, 0]})"); lineCnt++; }

                                if (procItem.CompRange.Count > 0)
                                    sb1.Append("\n");

                                for (int i = 1; i < procItem.CompRange.Count + 1; i++)
                                {
                                    if (comp1Cnt[idx][subIdx, 0] > 0)
                                    {
                                        result[i] = (double)((double)comp1Cnt[idx][subIdx, i] / (double)comp1Cnt[idx][subIdx, 0]) * 100.0;
                                        sb1.Append($"Case {i} : {result[i]:F1}%({comp1Cnt[idx][subIdx, i]})");
                                        lineCnt++;
                                    }
                                    else
                                    {
                                        if (comp1Cnt[idx][subIdx, i] > 0)
                                        {
                                            result[i] = (double)comp1Cnt[idx][subIdx, i] * 100.0;
                                            sb1.Append($"Case {i} : {result[i]:F1}%({comp1Cnt[idx][subIdx, i]})");
                                        }
                                        else
                                        {
                                            sb1.Append($"Case {i} : 0%({comp1Cnt[idx][subIdx, i]})");
                                        }
                                        lineCnt++;
                                    }

                                    if (Math.Abs(result[0] - result[i]) > procItem.CompRange[i - 1].Accuracy || isSubError == true)
                                    {
                                        sb1.Append($" *");
                                        isSubError = true;
                                    }

                                    if (i < procItem.CompRange.Count) { sb1.Append("\n"); lineCnt++; }
                                }

                                if (lineCnt > maxLine) maxLine = lineCnt;
                                if (isSubError == true) _isError = true;

                                flpResult.Controls.Add(makeProcessInfoLabel(sb1.ToString(), isSubError));
                            }
                        }
                    }
                }

                if (maxLine > 2)
                {
                    this.Height += (maxLine - 2) * 18;

                    foreach (Control ctrl in flpResult.Controls)
                    {
                        ctrl.Height = flpResult.ClientSize.Height - 20; // 여유 패딩 고려
                    }
                }
            }
            catch (Exception ex)
            {
                StringBuilder sb = new StringBuilder();
                sb.Append($"Proc Name: {procItem.Name}");
                UIHelper.SetText(lblProcess, sb.ToString());
                SystemLog.DisplaySystemLog($"{Lang.LotSummaryShowDetail} :[{procItem.Name}]{ex.Message}", Log.Level.Error);
            }
        }

        private RoundLabel makeProcessInfoLabel(string text, bool isError)
        {
            if (fontName == null)
                fontName = "Segoe UI";
            RoundLabel label = new RoundLabel();
            label.Width = 270;
            label.Height = 110;
            if(isError) label.BkColor = System.Drawing.Color.Crimson;
            else        label.BkColor = System.Drawing.Color.MidnightBlue;
            label.BorderColor = System.Drawing.Color.LightSteelBlue;
            label.CornerR = 10;
            label.Font = new System.Drawing.Font(fontName, 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            label.ForeColor = System.Drawing.Color.WhiteSmoke;
            label.IsFillLB = false;
            label.IsFillLT = false;
            label.IsFillRB = false;
            label.IsFillRT = false;
            label.Margin = new System.Windows.Forms.Padding(1);
            label.Name = "lblProcess";
            label.Padding = new System.Windows.Forms.Padding(5, 5, 1, 1);
            label.Text = text;
            label.TextAlign = System.Drawing.ContentAlignment.TopLeft;
            label.Thickness = 1; 
            label.Show();

            return label;
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

            // flpResult 숨기고 lblProcess 컨트롤 중심으로 옮김
            flpResult.Hide();
            tableLayoutPanel3.Controls.Remove(lblProcess);
            tableLayoutPanel3.Controls.Remove(flpResult);
            tableLayoutPanel3.Controls.Add(lblProcess, 1, 0);
            lblProcess.Dock = DockStyle.Fill;
            lblProcess.Show();
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
            lblProcess.Show();
            lblProcess.Text = "";
        }
        #endregion

        private void lblLotName_DoubleClick(object sender, EventArgs e)
        {
            if (_mode == eSummaryMode.LiveErrorCheck) return;

            if(this.ParentForm is FormLotList)
                ((FormLotList)this.ParentForm).DoubleClickSummaryData(this);
        }

        #region 언어 변경
        string fontName;
        public void UpdateLanguage()
        {
            CultureInfo culture = CultureInfo.CurrentCulture;

            fontName = Functions.GetCultureFontName(culture.Name);

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
