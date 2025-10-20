using DefectDBManager;
using DefectDBManager.Preproc;
using log4net;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Net.Mime.MediaTypeNames;

namespace MarkCompare
{
    public partial class FormSelectedLot : Form
    {
        static string[] _strDgvLotList = { "No", "Name" };
        static int[] _DgvLotListLength = { 50, 235 };
        enum eDgvLotName { No, Name, Total };

        public List<string> _LotList = new List<string>();

        public LotSelProcParam ProcItem { get; private set; } = null;

        /// <summary>
        /// DB 탐색용 데이터 코드
        /// </summary>
        private DefectDBManager.DestConfig _destConfig = null;

        DefectDBManager.PreprocLotManager _lotManager = null;
        DefectDBManager.Preproc.eProc _procIdx = DefectDBManager.Preproc.eProc.Selected;

        public bool ModelError { get; private set; }


        public FormSelectedLot(LotSelProcParam procItem, PreprocLotManager manager)
        {
            ProcItem = procItem;
            _lotManager = manager;

            _selDestName = ProcItem.DBFilter.Title;

            _destConfig = new DefectDBManager.DestConfig();
            _destConfig.Read();

            InitializeComponent();
            InitGridView();
            initDgvCompRange();

            initUserFilterCtrl();
            initDbFilterCtrl();

            cbDest.SelectedText = ProcItem.DBFilter.Title;

            UpdateLanguage();

            lblTitle.MouseDown += lblTitle_MouseDown;
            lblTitle.MouseMove += lblTitle_MouseMove;

            btnAdd.Click += btnAdd_Click;
            btnDel.Click += btnDel_Click;
            btnClear.Click += btnClear_Click;

            if (ProcItem.FilterType==FilterType.UserFilter)
                rbFilterType1.Checked = true;
            else
                rbFilterType2.Checked = true;

            ModelError = false;
        }


        #region 마우스로 폼 드래그
        private Point mouseDownLocation;
        private void lblTitle_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                this.mouseDownLocation = e.Location;
            }
        }
        private void lblTitle_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.WindowState == FormWindowState.Maximized) return;

            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                this.Left = e.X + this.Left - this.mouseDownLocation.X;
                this.Top = e.Y + this.Top - this.mouseDownLocation.Y;
            }
        }
        #endregion

        private void FormSelectedLot_VisibleChanged(object sender, EventArgs e)
        {
            if (Visible)
            {
                displayDgvCompRange();
                displayJudgeRange();
                displayOtherParam();

                displayLNCDCtrlData();
                displayDbFilterOption();
                displayMaterial();
            }
        }


        void InitGridView()
        {
            try
            {
                dgvLotList.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
                dgvLotList.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dgvLotList.AllowUserToAddRows = false;
                dgvLotList.RowHeadersVisible = false;
                dgvLotList.ColumnCount = (int)eDgvLotName.Total;
                for (int i = 0; i < dgvLotList.ColumnCount; i++)
                {
                    dgvLotList.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
                    dgvLotList.Columns[i].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;

                    dgvLotList.Columns[i].Name = _strDgvLotList[i];
                    dgvLotList.Columns[i].Width = _DgvLotListLength[i];
                }

                dgvLotList.Columns[(int)eDgvLotName.No].ReadOnly = true;

                
            }
            catch
            {

            }
        }

        private void displayMaterial()
        {
            dgvLotList.SuspendLayout();
            try 
            {
                dgvLotList.Rows.Clear();

                int idx = 0;
                foreach(var item in _LotList)
                {
                    string[] data = new string[(int)eDgvLotName.Total];
                    data[(int)eDgvLotName.No] = Convert.ToString(idx);
                    data[(int)eDgvLotName.Name] = item;
                    dgvLotList.Rows.Add(data);
                    idx++;
                }

            }
            catch
            {

            }
            finally
            {
                dgvLotList.ResumeLayout();
            }
        }

        private void addMaterial(string name="")
        {
            dgvLotList.SuspendLayout();
            try
            {
                int idx = dgvLotList.Rows.Count;
                string[] data = new string[(int)eDgvLotName.Total];
                data[(int)eDgvLotName.No] = Convert.ToString(idx);
                data[(int)eDgvLotName.Name] = name;
                dgvLotList.Rows.Add(data);
            }
            catch
            {

            }
            finally
            {
                dgvLotList.ResumeLayout();
            }
        }

        private void deleteMaterial()
        {
            dgvLotList.SuspendLayout();
            try
            {
                int idx = dgvLotList.SelectedRows[0].Index;
                if (idx >= 0)
                {
                    dgvLotList.Rows.RemoveAt(idx);

                    for (int i = 1; i < dgvLotList.Rows.Count; i++)
                        dgvLotList.Rows[i].Cells[(int)eDgvLotName.No].Value = Convert.ToString(i);
                }
            }
            catch
            {

            }
            finally
            {
                dgvLotList.ResumeLayout();
            }
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            _LotList.Clear();

            foreach (DataGridViewRow row in dgvLotList.Rows)
            {
                _LotList.Add(row.Cells[1].Value.ToString());
            }

            DialogResult = DialogResult.OK;
            this.Close();
        }


        private void dgvLotList_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            try
            {
                int index = (int)e.RowIndex;

                var name = dgvLotList.Rows[index].Cells[(int)eDgvLotName.Name].Value.ToString();
                if (name.Length > 12)
                    dgvLotList.Rows[index].Cells[(int)eDgvLotName.Name].Value = name.Substring(0, 12);
            }
            catch
            {

            }
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            if (dgvLotList.Rows.Count >= 100)
            {
                MessageBox.Show(Lang.maximumCsvFiles);
                return;
            }

            addMaterial();
        }

        private void btnDel_Click(object sender, EventArgs e)
        {
            if (dgvLotList.Rows.Count == 0 || dgvLotList.SelectedRows.Count == 0)
            {
                MessageBox.Show(Lang.noSelectedRow);
                return;
            }

            dgvLotList.CommitEdit(DataGridViewDataErrorContexts.Commit);

            deleteMaterial();
        }
        private void btnClear_Click(object sender, EventArgs e)
        {
            dgvLotList.Rows.Clear();
        }

        #region 언어 변경
        public void UpdateLanguage()
        { 
        }
        #endregion

        #region Compare Range
        static string[] _strDgvCompRangeHeader = { "No", "MinX", "MaxX", "MinY", "MaxY", "Rate" };
        static int[] _DgvCompRangeLength = { 30, 50, 50, 50, 50, 60 };
        enum eDgvCompRange { No, MinX, MaxX, MinY, MaxY, Rate, Total };

        private void initDgvCompRange()
        {
            dgvCompRange.SelectionMode = DataGridViewSelectionMode.CellSelect;
            dgvCompRange.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
            dgvCompRange.AllowUserToAddRows = false;
            dgvCompRange.RowHeadersVisible = false;
            dgvCompRange.ColumnCount = (int)eDgvCompRange.Total;
            for (int i = 0; i < dgvCompRange.ColumnCount; i++)
            {
                dgvCompRange.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
                dgvCompRange.Columns[i].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;

                dgvCompRange.Columns[i].Name = _strDgvCompRangeHeader[i];
                dgvCompRange.Columns[i].Width = _DgvCompRangeLength[i];
            }

            dgvCompRange.Columns[(int)eDgvCompRange.No].ReadOnly = true;
        }

        private void displayDgvCompRange()
        {
            dgvCompRange.Rows.Clear();
            List<CompRange> range = ProcItem.CompRange;

            string[] sR = new string[(int)eDgvCompRange.Total];
            sR[(int)eDgvCompRange.No] = "R";

            sR[(int)eDgvCompRange.MinX] = ProcItem.BasicRange.MinXRange.ToString();
            sR[(int)eDgvCompRange.MaxX] = ProcItem.BasicRange.MaxXRange.ToString();
            sR[(int)eDgvCompRange.MinY] = ProcItem.BasicRange.MinYRange.ToString();
            sR[(int)eDgvCompRange.MaxY] = ProcItem.BasicRange.MaxYRange.ToString();
            sR[(int)eDgvCompRange.Rate] = ProcItem.BasicRange.Accuracy.ToString();
            dgvCompRange.Rows.Add(sR);

            int idx = 1;
            foreach (CompRange compRange in range)
            {
                string[] s = new string[(int)eDgvCompRange.Total];
                s[(int)eDgvCompRange.No] = idx.ToString();

                s[(int)eDgvCompRange.MinX] = compRange.MinXRange.ToString();
                s[(int)eDgvCompRange.MaxX] = compRange.MaxXRange.ToString();
                s[(int)eDgvCompRange.MinY] = compRange.MinYRange.ToString();
                s[(int)eDgvCompRange.MaxY] = compRange.MaxYRange.ToString();
                s[(int)eDgvCompRange.Rate] = compRange.Accuracy.ToString();
                dgvCompRange.Rows.Add(s);

                idx++;
            }
        }

        private void updateDgvCompRange()
        {
            List<CompRange> rangeList = new List<CompRange>();
            for (int i = 0; i < dgvCompRange.Rows.Count; i++)
            {
                if (i == 0)
                {
                    CompRange rangeData = ProcItem.BasicRange;
                    rangeData.MinXRange = (float)Convert.ToDouble(dgvCompRange.Rows[i].Cells[(int)eDgvCompRange.MinX].Value);
                    rangeData.MaxXRange = (float)Convert.ToDouble(dgvCompRange.Rows[i].Cells[(int)eDgvCompRange.MaxX].Value);
                    rangeData.MinYRange = (float)Convert.ToDouble(dgvCompRange.Rows[i].Cells[(int)eDgvCompRange.MinY].Value);
                    rangeData.MaxYRange = (float)Convert.ToDouble(dgvCompRange.Rows[i].Cells[(int)eDgvCompRange.MaxY].Value);
                    rangeData.Accuracy = (float)Convert.ToDouble(dgvCompRange.Rows[i].Cells[(int)eDgvCompRange.Rate].Value);
                }
                else
                {
                    CompRange rangeData = new CompRange();
                    rangeData.MinXRange = (float)Convert.ToDouble(dgvCompRange.Rows[i].Cells[(int)eDgvCompRange.MinX].Value);
                    rangeData.MaxXRange = (float)Convert.ToDouble(dgvCompRange.Rows[i].Cells[(int)eDgvCompRange.MaxX].Value);
                    rangeData.MinYRange = (float)Convert.ToDouble(dgvCompRange.Rows[i].Cells[(int)eDgvCompRange.MinY].Value);
                    rangeData.MaxYRange = (float)Convert.ToDouble(dgvCompRange.Rows[i].Cells[(int)eDgvCompRange.MaxY].Value);
                    rangeData.Accuracy = (float)Convert.ToDouble(dgvCompRange.Rows[i].Cells[(int)eDgvCompRange.Rate].Value);
                    rangeList.Add(rangeData);
                }
            }

            ProcItem.CompRange = rangeList;
        }

        private void addDgvCompRange()
        {
            dgvCompRange.SuspendLayout();
            try
            {
                int idx = dgvCompRange.Rows.Count;
                string[] data = new string[(int)eDgvCompRange.Total];
                data[(int)eDgvCompRange.No] = Convert.ToString(idx);
                data[(int)eDgvCompRange.MinX] = "0.0";
                data[(int)eDgvCompRange.MaxX] = "0.0";
                data[(int)eDgvCompRange.MinY] = "0.0";
                data[(int)eDgvCompRange.MaxY] = "0.0";
                data[(int)eDgvCompRange.Rate] = "0.0";
                dgvCompRange.Rows.Add(data);
            }
            catch
            {

            }
            finally
            {
                dgvCompRange.ResumeLayout();
            }
        }

        private void deleteDgvCompRange()
        {
            dgvCompRange.SuspendLayout();
            try
            {
                int idx = dgvCompRange.SelectedCells[0].RowIndex;
                if (idx != 0)
                {
                    dgvCompRange.Rows.RemoveAt(idx);
                    dgvCompRange.Rows[0].Cells[(int)eDgvCompRange.No].Value = "R";

                    for (int i = 1; i < dgvCompRange.Rows.Count; i++)
                        dgvCompRange.Rows[i].Cells[(int)eDgvCompRange.No].Value = Convert.ToString(i);
                }
            }
            catch
            {

            }
            finally
            {
                dgvCompRange.ResumeLayout();
            }
        }

        private void btnAddCompRange_Click(object sender, EventArgs e)
        {
            addDgvCompRange();
        }

        private void btnDelCompRange_Click(object sender, EventArgs e)
        {
            deleteDgvCompRange();
        }

        #endregion

        #region Judge Range

        /// <summary>
        /// Ctrl에 입력되어 있는 정보를 얻어온다.
        /// </summary>
        private void updateJudgeRange()
        {
            ProcItem.Judge.X = (float)Convert.ToDouble(tbJudgeRangeX.Texts);
            ProcItem.Judge.Y = (float)Convert.ToDouble(tbJudgeRangeY.Texts);
        }

        /// <summary>
        /// Ctrl에 현재 정보를 업데이트 한다.
        /// </summary>
        private void displayJudgeRange()
        {
            try
            {
                tbJudgeRangeX.Texts = ProcItem.Judge.X.ToString();
                tbJudgeRangeY.Texts = ProcItem.Judge.Y.ToString();
            }
            catch
            {

            }
        }
        #endregion

        #region Other Param
        private void displayOtherParam()
        {
            cbUseMNTTAN.Checked = ProcItem.UseAiResult;
            cbUseSplit.Checked = ProcItem.UseSplit;
        }

        #endregion

        private void btnLoadParam_Click(object sender, EventArgs e)
        {
            ProcItem.Load();

            displayDgvCompRange();
            displayJudgeRange();
            displayOtherParam();
        }

        private void btnSaveParam_Click(object sender, EventArgs e)
        {
            updateDbFilterOption();
            updateDgvCompRange();
            updateJudgeRange();
            ProcItem.UseAiResult = cbUseMNTTAN.Checked;
            ProcItem.UseSplit = cbUseSplit.Checked;
            ProcItem.Save();
        }

        #region User Filter
        private void initUserFilterCtrl()
        {
            rbFilterType1.CheckedChanged += rbFilterType_CheckedChanged;
        }
        private void enableUserFilterCtrl(bool enable)
        {
            btnAddUserFilter.Enabled = enable;
            lvFilterList.Enabled = enable;
        }

        private void btnAddUserFilter_Click(object sender, EventArgs e)
        {
            try
            {
                using (FormProductFilter form = new FormProductFilter(_lotManager, _procIdx, ProcItem.UserFilter))
                {

                    if (form.ShowDialog() == DialogResult.OK)
                    {
                        ProcItem.UserFilter = form.Filter;
                        displayLNCDCtrlData();
                    }
                }
            }
            catch
            {

            }
        }

        private void displayLNCDCtrlData()
        {
            int ctrlCount = ProcItem.UserFilter.Count;
            bool isError = false;
            bool isSkip = false;
            List<string> strError = new List<string>();

            try
            {
                lvFilterList.BeginUpdate();
                lvFilterList.Items.Clear();
                foreach (var item in ProcItem.UserFilter.Data)
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

                ModelError = isError;

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
        #endregion

        #region DB Filter
        string _selDestName = string.Empty;
        private void initDbFilterCtrl()
        {
            rbFilterType2.CheckedChanged += rbFilterType_CheckedChanged;

            cbDest.Items.Clear();
            foreach (var item in _destConfig.DicDest)
                cbDest.Items.Add(item.Key);
        }
        private void cbDest_SelectedIndexChanged(object sender, EventArgs e)
        {
            _selDestName = cbDest.SelectedItem.ToString();
            if (_selDestName == "") return;
            
            displayDbFilterOption();
        }

        private void displayDbFilterOption()
        {
            try
            {
                var item = _destConfig.DicDest[_selDestName];
                cbDest.SelectedItem = _selDestName;
                if (_selDestName == ProcItem.DBFilter.Title)
                {
                    ckbES.Checked = ProcItem.DBFilter.UseES;
                    ckbTG.Checked = ProcItem.DBFilter.UseTG;
                    ckbETC.Checked = ProcItem.DBFilter.UseETC;
                }
                else
                {
                    ckbES.Checked = item.UseES;
                    ckbTG.Checked = item.UseTG;
                    ckbETC.Checked = item.UseETC;
                }
            }
            catch (Exception ex)
            {

            }
        }

        private void updateDbFilterOption()
        {
            try
            {
                string selName = cbDest.SelectedItem.ToString();
                if (selName == "")
                {
                    ProcItem.DBFilter.Title = "";
                    return;
                }

                ProcItem.DBFilter.Title = selName;
                ProcItem.DBFilter.MKCD = _destConfig.DicDest[selName].MKCD;
                ProcItem.DBFilter.UseES = ckbES.Checked;
                ProcItem.DBFilter.UseTG = ckbTG.Checked;
                ProcItem.DBFilter.UseETC = ckbETC.Checked;
            }
            catch
            {

            }
        }

        private void enableDbFilterCtrl(bool enable)
        {
            cbDest.Enabled = enable;
            ckbES.Enabled = enable;
            ckbTG.Enabled = enable;
            ckbETC.Enabled = enable;
        }
        #endregion


        #region rbFilterType 변환
        private void enableFilterCtrl()
        {
            if (ProcItem.FilterType==FilterType.UserFilter)
            {
                enableUserFilterCtrl(true);
                enableDbFilterCtrl(false);
            }
            else if (ProcItem.FilterType == FilterType.DbFilter)
            {
                enableUserFilterCtrl(false);
                enableDbFilterCtrl(true);
            }
        }

        private void rbFilterType_CheckedChanged(object sender, EventArgs e)
        {
            RadioButton changed = sender as RadioButton;
            if(changed.Checked)
            {
                if(changed == rbFilterType1)
                {
                    rbFilterType2.Checked = false;
                    ProcItem.FilterType = FilterType.UserFilter;
                }
                else if(changed == rbFilterType2)
                {
                    rbFilterType1.Checked = false;
                    ProcItem.FilterType = FilterType.DbFilter;
                }
                enableFilterCtrl();
            }
        }

        #endregion

        private void btnAddList_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog browser = new OpenFileDialog())
            {
                browser.RestoreDirectory = true;

                if (browser.ShowDialog() == DialogResult.OK)
                {
                    using (StreamReader reader = new StreamReader(browser.FileName))
                    {
                        string text;
                        while ((text = reader.ReadLine()) != null)
                        {
                            addMaterial(text);
                        }
                    }
                }
            }
        }
    }
}
