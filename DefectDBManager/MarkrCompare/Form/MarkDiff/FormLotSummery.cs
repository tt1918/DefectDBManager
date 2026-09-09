using Coss.Controls;
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

namespace MarkCompare
{
    public partial class FormLotSummery : Form
    {
        enum eShowGroup
        {
            Normal,
            NG,
            All,
            Total
        }

        private eShowGroup _showGroup = eShowGroup.All;

        #region Form
        public FormLotSummery()
        {
            InitializeComponent();
            initLotSummary();
            initDgvAiMonitor();
            // 전체 보기를 기본으로 처리함
            rbShowAll.Checked = true;
        }

        private void FormLotSummery_FormClosing(object sender, FormClosingEventArgs e)
        {
            closeLotSummary();
        }

        #endregion

        #region Lot Summary Flow Layout Panel
        /// <summary>
        /// 랏 정보에 대한 요약 데이터 처리
        /// key : 라인 정보로 입력함
        /// value : 각 랏의 정보를 입력함
        /// </summary>
        Dictionary<string, List<FormLotSummaryData>> _dicLotSummary = null;

        private void initLotSummary()
        {
            closeLotSummary();
            _dicLotSummary = new Dictionary<string, List<FormLotSummaryData>>();
        }

        private void closeLotSummary()
        {
            if(_dicLotSummary!=null)
            {
                foreach( var list in _dicLotSummary.Values )
                {
                    foreach(var form in list)
                        form.Dispose();
                    list.Clear();
                }
                _dicLotSummary.Clear();
            }
            
            _dicLotSummary = null;
        }

        private void clearLotSummery()
        {
            if (_dicLotSummary != null)
            {
                foreach (var list in _dicLotSummary.Values)
                {
                    foreach (var form in list)
                        form.Dispose();
                    list.Clear();
                }
                _dicLotSummary.Clear();
            }

        }

        public void SetLotSummary(string lncd, List<DefectDBManager.IPreprocLot> lotSummary, DefectDBManager.PreprocLotManager lotManager, string filter)
        {
            try
            {
                foreach (var lot in lotSummary)
                {
                    BeginInvoke(new Action(delegate 
                    {
                        bool isExist = false;
                        // 동일 랏이 처리되어 있는지 확인함
                        if (_dicLotSummary.ContainsKey(lncd))
                        {
                            foreach(var view in _dicLotSummary[lncd])
                                if (view.LotSummery.LotName == lot.LotName) isExist = true;
                        }

                        if(isExist==false)
                        {
                            DefectDBManager.Preproc.PreprocItem procItem = new DefectDBManager.Preproc.PreprocItem();
                          
                            var keyData = DefectDBManager.Helper.SplitKeyData(filter);
                            for (int i = 0; i < lotManager.ProcSetting.Count; i++)
                            {
                                if (lotManager.ProcSetting[i].Name == keyData[2])
                                {
                                    procItem = lotManager.ProcSetting[i];
                                    break;
                                }
                            }
                            
                            FormLotSummaryData form = new FormLotSummaryData();
                            form.LotManager = lotManager;
                            form.ProcItem = procItem;
                            form.Filter = filter;
                            form.LotSummery = lot;
                            form.MODE = eSummaryMode.LiveErrorCheck;
                            form.TopLevel = false;
                            form.Show();

                            if (_dicLotSummary == null) _dicLotSummary = new Dictionary<string, List<FormLotSummaryData>>();

                            if (_dicLotSummary.ContainsKey(lncd))
                            {
                                _dicLotSummary[lncd].Add(form);
                                flpLotSummary.Controls.Add(form);
                            }
                            else
                            {
                                _dicLotSummary.Add(lncd, new List<FormLotSummaryData>());
                                _dicLotSummary[lncd].Add(form);
                                flpLotSummary.Controls.Add(form);
                            }
                        }
                    }));
                }
            }
            catch
            {

            }
        }

        private void clearAllData()
        {
            try
            {
                clearLotSummery();
            }
            catch
            {

            }
        }

        private void clearNormalData()
        {
            try
            {
                if (_dicLotSummary != null) 
                {
                    foreach (var item in _dicLotSummary.Values)
                    {
                        foreach (var form in item)
                        {
                            if (!form.IsError)
                                form.Dispose();
                        }
                    }
                }
            }
            catch (Exception e)
            {
                SystemLog.DisplaySystemLog($"[ClearNormalData] - {e.Message}");
            }
        }

        private void clearNGData()
        {
            try
            {
                if (_dicLotSummary != null)
                {
                    foreach (var item in _dicLotSummary.Values)
                    {
                        foreach (var form in item)
                        {
                            if (form.IsError)   form.Dispose();
                        }
                    }
                }
            }
            catch (Exception e)
            {
                SystemLog.DisplaySystemLog($"[ClearNGData] - {e.Message}");
            }
        }

        private void clearData()
        {
            switch(_showGroup)
            {
                case eShowGroup.Normal:
                    clearNormalData();
                    break;

                case eShowGroup.NG:
                    clearNGData();
                    break;

                case eShowGroup.All:    
                    clearAllData(); 
                    break;
            }
        }

        private void showAllData()
        {
            try
            {
                foreach (var item in _dicLotSummary.Values)
                {
                    foreach (var form in item)
                        form.Visible = true;
                }
            }
            catch (Exception e)
            {
                SystemLog.DisplaySystemLog($"[ShowAllData] - {e.Message}");
            }
        }

        private void showNGData()
        {
            try
            {
                foreach (var item in _dicLotSummary.Values)
                {
                    foreach (var form in item)
                    {
                        if (form.IsError)   form.Visible = true;
                        else                form.Visible = false;
                    }
                }
            }
            catch (Exception e)
            {
                SystemLog.DisplaySystemLog($"[ShowNGData] - {e.Message}");
            }
        }

        private void showNormalData()
        {
            try 
            {
                foreach (var item in _dicLotSummary.Values)
                {
                    foreach (var form in item)
                    {
                        if (!form.IsError)
                            form.Visible = true;
                        else
                            form.Visible = false;
                    }
                }
            }
            catch (Exception e)
            {
                SystemLog.DisplaySystemLog($"[ShowNormalData] - {e.Message}");
            }
        }
        public void ShowData()
        {
            if (rbShowNormal.Checked == true)
            { 
                _showGroup = eShowGroup.Normal; 
                showNormalData(); 
            } 
            else if(rbShowNG.Checked == true)
            { 
                _showGroup = eShowGroup.NG; 
                showNGData(); 
            }  
            else if(rbShowAll.Checked==true)
            { 
                _showGroup = eShowGroup.All; 
                showAllData();
            } 
        }
        #endregion

        #region Control
        private void btnClearHistory_Click(object sender, EventArgs e)
        {
            clearData();
        }

        private void rbShow_Click(object sender, EventArgs e)
        {
            ShowData();
        }
        #endregion

        #region 언어 변경
        public void UpdateLanguage(string curtule)
        {
            this.BeginInvoke(new Action(() =>
            {
                string fontName = Functions.GetCultureFontName(curtule);
                Font newFont = new Font(fontName, 9, FontStyle.Bold);

                btnClearHistory.Font = newFont;
                btnClearHistory.Text = Lang.clearHistory;

            }));
        }
        #endregion

        #region AI Monitoring 
        enum eDgvTable
        {
            Index,
            LNCD,
            SJMode,
            CTRNO,
            Finish,
            ModeNo,
            FLTID,
            Rate,
        }

        class AiMonitorMergeRange
        {
            public int StartRow { get; set; }
            public int EndRow { get; set; }
        }

        readonly List<AiMonitorMergeRange> _aiMonitorMergeRanges = new List<AiMonitorMergeRange>();

        string[] aiTableName = new string[] { "#", "LNCD","MODE","CTRNO", "Finish", "ModeNo", "FLTID ", "Rate" };
        int[] dgvTableWidth = new int[] { 40, 50, 50,90, 45, 60, 70, 50};

        FormErrorLotDisp _sjmodeErrLotDispForm = new FormErrorLotDisp(DefectDBManager.Preproc.eProc.Live);

        int _aiDataCnt = 0;
        private void initDgvAiMonitor()
        {
            dgvAiMonitor.ColumnCount = aiTableName.Length;
            for (int i = 0; i < aiTableName.Length; i++)
            {
                dgvAiMonitor.Columns[i].Name = aiTableName[i];
                dgvAiMonitor.Columns[i].Width = dgvTableWidth[i];
                dgvAiMonitor.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
                dgvAiMonitor.Columns[i].Resizable = DataGridViewTriState.False;
            }

            dgvAiMonitor.CellFormatting -= dgvAiMonitor_CellFormatting;
            dgvAiMonitor.CellFormatting += dgvAiMonitor_CellFormatting;
            dgvAiMonitor.CellPainting -= dgvAiMonitor_CellPainting;
            dgvAiMonitor.CellPainting += dgvAiMonitor_CellPainting;

            dgvAiMonitor.AllowUserToAddRows = false;

            clearDgvAiMonitor();

            _sjmodeErrLotDispForm.IsHideMode = true;
        }

        private void clearDgvAiMonitor()
        {
            dgvAiMonitor.Rows.Clear();
            _aiMonitorMergeRanges.Clear();
            _aiDataCnt = 0;
        }

        bool isAiMonitorMergeColumn(int columnIndex)
        {
            return columnIndex == (int)eDgvTable.Index ||
                   columnIndex == (int)eDgvTable.LNCD ||
                   columnIndex == (int)eDgvTable.SJMode ||
                   columnIndex == (int)eDgvTable.CTRNO ||
                   columnIndex == (int)eDgvTable.ModeNo ||
                   columnIndex == (int)eDgvTable.Finish;
        }

        bool tryGetAiMonitorMergeRange(int rowIndex, out int startRow, out int endRow)
        {
            startRow = -1;
            endRow = -1;

            for (int i = _aiMonitorMergeRanges.Count - 1; i >= 0; i--)
            {
                var range = _aiMonitorMergeRanges[i];
                if (rowIndex >= range.StartRow && rowIndex <= range.EndRow)
                {
                    startRow = range.StartRow;
                    endRow = range.EndRow;
                    return true;
                }
            }

            return false;
        }

        void dgvAiMonitor_CellFormatting(object sender, DataGridViewCellFormattingEventArgs e)
        {
            if (e.RowIndex < 0 || e.ColumnIndex < 0) return;
            if (!isAiMonitorMergeColumn(e.ColumnIndex)) return;

            if (tryGetAiMonitorMergeRange(e.RowIndex, out int startRow, out int endRow) && endRow > startRow && e.RowIndex > startRow)
            {
                e.Value = string.Empty;
                e.FormattingApplied = true;
            }
        }

        void dgvAiMonitor_CellPainting(object sender, DataGridViewCellPaintingEventArgs e)
        {
            if (e.RowIndex < 0 || e.ColumnIndex < 0) return;
            if (!isAiMonitorMergeColumn(e.ColumnIndex)) return;

            if (!tryGetAiMonitorMergeRange(e.RowIndex, out int startRow, out int endRow) || endRow <= startRow)
                return;

            if (e.RowIndex > startRow)
            {
                e.Handled = true;

                using (Pen gridPen = new Pen(dgvAiMonitor.GridColor))
                {
                    e.Graphics.DrawLine(gridPen, e.CellBounds.Left, e.CellBounds.Top, e.CellBounds.Left, e.CellBounds.Bottom - 1);
                    e.Graphics.DrawLine(gridPen, e.CellBounds.Right - 1, e.CellBounds.Top, e.CellBounds.Right - 1, e.CellBounds.Bottom - 1);
                    if (e.RowIndex == endRow)
                    {
                        e.Graphics.DrawLine(gridPen, e.CellBounds.Left, e.CellBounds.Bottom - 1, e.CellBounds.Right - 1, e.CellBounds.Bottom - 1);
                    }
                }
                return;
            }

            Rectangle mergedBounds = e.CellBounds;
            for (int i = startRow + 1; i <= endRow; i++)
            {
                mergedBounds.Height += dgvAiMonitor.Rows[i].Height;
            }

            e.Handled = true;

            using (SolidBrush backBrush = new SolidBrush(e.CellStyle.BackColor))
            {
                e.Graphics.FillRectangle(backBrush, mergedBounds);
            }

            using (Pen gridPen = new Pen(dgvAiMonitor.GridColor))
            {
                e.Graphics.DrawRectangle(gridPen, mergedBounds.X, mergedBounds.Y, mergedBounds.Width - 1, mergedBounds.Height - 1);
            }

            string text = Convert.ToString(e.Value);
            Region oldClip = e.Graphics.Clip;
            e.Graphics.SetClip(mergedBounds);
            TextRenderer.DrawText(
                e.Graphics,
                text,
                e.CellStyle.Font,
                mergedBounds,
                e.CellStyle.ForeColor,
                System.Windows.Forms.TextFormatFlags.HorizontalCenter |
                System.Windows.Forms.TextFormatFlags.VerticalCenter |
                System.Windows.Forms.TextFormatFlags.EndEllipsis);
            e.Graphics.Clip = oldClip;
        }

        private int findAiMonitorRow(SjMonitorData monitorData)
        {
            for (int i = 0; i < dgvAiMonitor.Rows.Count; i++)
            {
                DataGridViewRow row = dgvAiMonitor.Rows[i];
                if (row.IsNewRow) continue;

                if (Convert.ToString(row.Cells[(int)eDgvTable.LNCD].Value) != Convert.ToString(monitorData.LNCD))
                    continue;
                if (Convert.ToString(row.Cells[(int)eDgvTable.CTRNO].Value) != Convert.ToString(monitorData.CTLNO))
                    continue;
                if (Convert.ToString(row.Cells[(int)eDgvTable.SJMode].Value) != Convert.ToString(monitorData.ModeNo))
                    continue;

                return i;
            }

            return -1;
        }

        private void setAiMonitorCommonCells(int rowIndex, object indexValue, SjMonitorData monitorData)
        {
            dgvAiMonitor.Rows[rowIndex].Cells[(int)eDgvTable.Index].Value = indexValue;
            dgvAiMonitor.Rows[rowIndex].Cells[(int)eDgvTable.LNCD].Value = monitorData.LNCD;
            dgvAiMonitor.Rows[rowIndex].Cells[(int)eDgvTable.CTRNO].Value = monitorData.CTLNO;
            dgvAiMonitor.Rows[rowIndex].Cells[(int)eDgvTable.SJMode].Value = monitorData.ModeNo;
            dgvAiMonitor.Rows[rowIndex].Cells[(int)eDgvTable.Finish].Value = monitorData.IsFinished ? "O" : "X";
        }

        private void setAiMonitorJudgeCells(int rowIndex, KeyValuePair<int, SjJudgement> judge)
        {
            string fltId = string.Join(",", judge.Value.FLTID);
            dgvAiMonitor.Rows[rowIndex].Cells[(int)eDgvTable.ModeNo].Value = judge.Key;
            dgvAiMonitor.Rows[rowIndex].Cells[(int)eDgvTable.FLTID].Value = fltId;
            dgvAiMonitor.Rows[rowIndex].Cells[(int)eDgvTable.Rate].Value = judge.Value.NoneConvertRate.ToString("0.00%");

            if (judge.Value.Judgement == true)
                dgvAiMonitor.Rows[rowIndex].Cells[(int)eDgvTable.Rate].Style.ForeColor = Color.Green;
            else
                dgvAiMonitor.Rows[rowIndex].Cells[(int)eDgvTable.Rate].Style.ForeColor = Color.Red;
        }

        private void setAiMonitorNonJudgeCells(int rowIndex, object indexValue, SjMonitorData monitorData)
        {
            string modeKey = string.Join(",", monitorData.DefectInfo.Keys);
            dgvAiMonitor.Rows[rowIndex].Cells[(int)eDgvTable.ModeNo].Value = modeKey;
            dgvAiMonitor.Rows[rowIndex].Cells[(int)eDgvTable.FLTID].Value = string.Empty;
            dgvAiMonitor.Rows[rowIndex].Cells[(int)eDgvTable.Rate].Value = string.Empty;
        }

        private void clearAiMonitorJudgeCells(int rowIndex)
        {
            dgvAiMonitor.Rows[rowIndex].Cells[(int)eDgvTable.FLTID].Value = string.Empty;
            dgvAiMonitor.Rows[rowIndex].Cells[(int)eDgvTable.Rate].Value = string.Empty;
            dgvAiMonitor.Rows[rowIndex].Cells[(int)eDgvTable.Rate].Style.ForeColor = dgvAiMonitor.DefaultCellStyle.ForeColor;
        }

        private void shiftAiMonitorMergeRanges(int startRowIndex, int delta)
        {
            if (delta == 0) return;

            foreach (var range in _aiMonitorMergeRanges)
            {
                if (range.StartRow >= startRowIndex)
                {
                    range.StartRow += delta;
                    range.EndRow += delta;
                }
                else if (range.EndRow >= startRowIndex)
                {
                    range.EndRow += delta;
                }
            }
        }

        private void removeAiMonitorMergeRangeContaining(int rowIndex)
        {
            for (int i = _aiMonitorMergeRanges.Count - 1; i >= 0; i--)
            {
                if (rowIndex >= _aiMonitorMergeRanges[i].StartRow && rowIndex <= _aiMonitorMergeRanges[i].EndRow)
                    _aiMonitorMergeRanges.RemoveAt(i);
            }
        }

        public void AddAiMonitorData(SjMonitorData monitorData)
        {
            if (InvokeRequired)
            {
                BeginInvoke(new Action(() =>
                {
                    AddAiMonitorData(monitorData);
                }));
                return;
            }

            if (monitorData == null)
                return;

            try
            {
                dgvAiMonitor.SuspendLayout();
                List<string> errLot = new List<string>();

                int existRowIndex = findAiMonitorRow(monitorData);
                bool hasJudgement = monitorData.Judgement != null && monitorData.Judgement.Count > 0;

                // 0. 에러 체크
                if (hasJudgement)
                {
                    foreach (var judge in monitorData.Judgement)
                    {
                        if(judge.Value.Judgement == false)
                        {
                            errLot.Add($"{monitorData.LNCD} / {monitorData.CTLNO} / {monitorData.ModeNo} / Mismatch {judge.Value.NoneConvertRate*100.0:F2}%");
                        }
                    }
                }
                else
                {
                    // 불량 데이터가 존재면 해당 SJMode를 추가한다
                    if(monitorData.DefectInfo.Count>0)
                    {
                        StringBuilder sb = new StringBuilder();
                        sb.Append($"{monitorData.LNCD} / {monitorData.CTLNO} / {monitorData.ModeNo} / Input SJMODE [");
                        foreach(var defectInfo in monitorData.DefectInfo)
                        {
                            sb.Append($" {defectInfo.Key}");
                        }
                        sb.Append(" ]");

                        sb.Append(" / No Judgement");
                        errLot.Add(sb.ToString());
                    }
                    else
                    {
                        errLot.Add($"{monitorData.LNCD} / {monitorData.CTLNO} / {monitorData.ModeNo} / No Judgement");
                    }
                }

                // 1. 기존 행이 있고, 이번에는 검색 결과가 생긴 경우
                if (existRowIndex >= 0 && hasJudgement)
                {
                    removeAiMonitorMergeRangeContaining(existRowIndex);

                    object indexValue = dgvAiMonitor.Rows[existRowIndex].Cells[(int)eDgvTable.Index].Value;
                    int startRow = existRowIndex;
                    int addedRowCount = 0;
                    bool isFirst = true;

                    foreach (var judge in monitorData.Judgement)
                    {
                        if (isFirst)
                        {
                            setAiMonitorCommonCells(existRowIndex, indexValue, monitorData);
                            setAiMonitorJudgeCells(existRowIndex, judge);
                            isFirst = false;
                        }
                        else
                        {
                            int insertRowIndex = existRowIndex + addedRowCount;
                            dgvAiMonitor.Rows.Insert(insertRowIndex, 1);
                            shiftAiMonitorMergeRanges(insertRowIndex, 1);

                            setAiMonitorCommonCells(insertRowIndex, indexValue, monitorData);
                            setAiMonitorJudgeCells(insertRowIndex, judge);
                        }

                        addedRowCount++;
                    }

                    if (addedRowCount > 1)
                    {
                        _aiMonitorMergeRanges.Add(new AiMonitorMergeRange
                        {
                            StartRow = startRow,
                            EndRow = startRow + addedRowCount - 1
                        });
                    }

                    dgvAiMonitor.Invalidate();
                    return;
                }

                // 2. 기존 행이 있지만 Judgement는 아직 없는 경우 -> 공통 정보만 갱신
                if (existRowIndex >= 0 && !hasJudgement)
                {
                    object indexValue = dgvAiMonitor.Rows[existRowIndex].Cells[(int)eDgvTable.Index].Value;
                    setAiMonitorCommonCells(existRowIndex, indexValue, monitorData);
                    clearAiMonitorJudgeCells(existRowIndex);

                    dgvAiMonitor.Invalidate();
                    return;
                }

                // 3. 신규 데이터 추가
                _aiDataCnt++;

                if (hasJudgement)
                {
                    int startRow = dgvAiMonitor.Rows.Count;
                    int addedRowCount = 0;

                    foreach (var judge in monitorData.Judgement)
                    {
                        int rowIndex = dgvAiMonitor.Rows.Add();
                        setAiMonitorCommonCells(rowIndex, _aiDataCnt, monitorData);
                        setAiMonitorJudgeCells(rowIndex, judge);
                        addedRowCount++;
                    }

                    if (addedRowCount > 1)
                    {
                        _aiMonitorMergeRanges.Add(new AiMonitorMergeRange
                        {
                            StartRow = startRow,
                            EndRow = startRow + addedRowCount - 1
                        });
                    }
                }
                else
                {
                    int rowIndex = dgvAiMonitor.Rows.Add();
                    setAiMonitorCommonCells(rowIndex, _aiDataCnt, monitorData);
                    setAiMonitorNonJudgeCells(rowIndex, _aiDataCnt, monitorData);
                }

                dgvAiMonitor.Invalidate();

                if(_sjmodeErrLotDispForm==null)
                    _sjmodeErrLotDispForm = new FormErrorLotDisp(DefectDBManager.Preproc.eProc.Live);
                _sjmodeErrLotDispForm.OnUpdateErrorLots(errLot.ToArray());
                _sjmodeErrLotDispForm.Show();
            }
            catch
            {
            }
            finally
            {
                dgvAiMonitor.ResumeLayout();
            }
        }

        public void UpdateAIResultMonitor(SjMonitorData monitorData)
        {
            if (monitorData == null) return;
            this.BeginInvoke(new Action(() =>
            {
                AddAiMonitorData(monitorData);
            }));
        }

        #endregion
    }
}
