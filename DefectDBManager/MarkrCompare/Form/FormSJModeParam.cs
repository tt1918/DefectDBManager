using DefectDBManager.Preproc;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace MarkCompare
{
    public partial class FormSJModeParam : Form
    {

        #region Param
        public string CultureCode="";
        public DefectDBManager.Preproc.SjModeParam Param
        {
            get { return _param; }
            set { _param = value; }
        }
        DefectDBManager.Preproc.SjModeParam _param = null;

        private DefectDBManager.Preproc.SjModeIPath _selectedModel = null;

        #endregion

        #region Form Control
        public FormSJModeParam()
        {
            InitializeComponent();

            lblTitle.MouseDown += lblTitle_MouseDown;
            lblTitle.MouseMove += lblTitle_MouseMove;

            _param = new DefectDBManager.Preproc.SjModeParam();
            _param.Load();

            UpdateLanguage(CultureCode);
        }

        private void FormSJModeParam_Load(object sender, EventArgs e)
        {
            if(this.Visible==true)
            {
                initDgvModel();
                initDgvParam();
            }
        }

        private void FormSJModeParam_FormClosing(object sender, FormClosingEventArgs e)
        {
        }
        #endregion

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


        #region 언어
        public void UpdateLanguage(string cultrue)
        {
            string fontName = Functions.GetCultureFontName(cultrue);
            Font newFont = new Font(fontName, 10, FontStyle.Bold);

            lblTitle.Font = newFont;
            
            lblSelLNCD.Font = newFont;
            

            newFont = new Font(fontName, 9);
            btnAddModel.Font = newFont;
            btnDeleteModel.Font = newFont;
            btnLoad.Font = newFont;
            btnSave.Font = newFont;
            btnCancel.Font = newFont;
            btnOK.Font = newFont;
            dgvModel.Font = newFont;

            lblTitle.Text = Lang.formLNCDTitle;

            btnAddModel.Text = Lang.btnAdd;
            btnDeleteModel.Text = Lang.btnDel1;
            btnLoad.Text = Lang.btnLoad1;
            btnSave.Text = Lang.btnSave1;

            btnCancel.Text = Lang.btnCancel;
            btnOK.Text = Lang.btnOK1;
        }
        #endregion

        #region Model Control

        #region Data Grid View 
        private void initDgvModel()
        {
            dgvModel.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;

            dgvModel.Rows.Clear();
            dgvModel.Columns.Clear();

            dgvModel.Columns.Add("Name", "NAME");
            dgvModel.Columns.Add("LNCD", "LNCD");
            dgvModel.Columns.Add("ModelName", "SJMODE");

            dgvModel.Columns[0].FillWeight = 100;
            dgvModel.Columns[1].FillWeight = 100;
            dgvModel.Columns[2].FillWeight = 200;

            // 컬럼 정렬 기능 비활성화
            foreach (DataGridViewColumn column in dgvModel.Columns)
                column.SortMode = DataGridViewColumnSortMode.NotSortable;

            dgvModel.AllowUserToAddRows = false;
            dgvModel.AllowUserToDeleteRows = false;
            dgvModel.AllowUserToResizeColumns = false;
            dgvModel.AllowUserToResizeRows = false;
            dgvModel.RowHeadersVisible = false;
            dgvModel.SelectionMode = DataGridViewSelectionMode.FullRowSelect;

            foreach (var model in _param.ModeItems)
                addModelToDgv(model);
        }

        private void addModelToDgv(DefectDBManager.Preproc.SjModeIPath model)
        {
            dgvModel.Rows.Add(model.Name, model.LNCD, model.ModelName);
        }
        private void dgvModel_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            _selectedModel = null;
            if (dgvModel.SelectedRows.Count > 0)
            {
                string name = dgvModel.SelectedRows[0].Cells[0].Value?.ToString() ?? string.Empty;
                string lncd = dgvModel.SelectedRows[0].Cells[1].Value?.ToString() ?? string.Empty;
                string modelName = dgvModel.SelectedRows[0].Cells[2].Value?.ToString() ?? string.Empty;

                _selectedModel = _param.ModeItems.FirstOrDefault(item => item.Name == name && item.LNCD == lncd && item.ModelName == modelName);
                displayParamForSelectedModel();
            }
        }

        private void dgvModel_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            if(dgvModel.SelectedRows.Count > 0)
            {
                
                DataGridViewRow row = dgvModel.SelectedRows[0];
                int index = row.Index;
                string name = row.Cells[0].Value?.ToString() ?? string.Empty;
                string lncd = row.Cells[1].Value?.ToString() ?? string.Empty;
                string modelName = row.Cells[2].Value?.ToString() ?? string.Empty;
                _param.ModeItems[index].Name = name;
                _param.ModeItems[index].LNCD = lncd;
                _param.ModeItems[index].ModelName = modelName;
            }
        }

        #endregion
        private void btnAddModel_Click(object sender, EventArgs e)
        {
            DefectDBManager.Preproc.SjModeIPath item = new DefectDBManager.Preproc.SjModeIPath();
            _param.ModeItems.Add(item);
            
            addModelToDgv(item);
            _selectedModel = item;
            displayParamForSelectedModel();
        }

        private void btnDeleteModel_Click(object sender, EventArgs e)
        {
            if (dgvModel.SelectedRows.Count > 0)
            {
                string name = dgvModel.SelectedRows[0].Cells[0].Value?.ToString() ?? string.Empty;
                string lncd = dgvModel.SelectedRows[0].Cells[1].Value?.ToString() ?? string.Empty;
                string modelName = dgvModel.SelectedRows[0].Cells[2].Value?.ToString() ?? string.Empty;

                for (int i=0; i<_param.ModeItems.Count; i++)
                {
                    var item = _param.ModeItems[i];
                    if (item.Name == name && 
                        item.LNCD == lncd && 
                        item.ModelName == modelName)
                    {
                        _param.ModeItems.RemoveAt(i);
                        break;
                    }
                }

                dgvModel.Rows.Remove(dgvModel.SelectedRows[0]);
            }
        }

        #endregion

        #region Parameter Control

        #region Data Grid View
      

        private void initDgvParam()
        {
            dgvDetailParam.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;

            dgvDetailParam.Rows.Clear();
            dgvDetailParam.Columns.Clear();

            // 1. 텍스트 컬럼
            dgvDetailParam.Columns.Add("SECFLTID", "대상 결점");

            // 2. 체크박스 컬럼
            DataGridViewCheckBoxColumn useColumn = new DataGridViewCheckBoxColumn();
            useColumn.Name = "Use";
            useColumn.HeaderText = "감시 여부";
            useColumn.TrueValue = true;
            useColumn.FalseValue = false;
            useColumn.DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
            dgvDetailParam.Columns.Add(useColumn);

            // 3. 텍스트 컬럼
            dgvDetailParam.Columns.Add("ErrorRate", "이상 비율");

            // 4. 결점 이름
            dgvDetailParam.Columns.Add("FLTID", "결점");

            dgvDetailParam.Columns[0].FillWeight = 90;
            dgvDetailParam.Columns[1].FillWeight = 90;
            dgvDetailParam.Columns[2].FillWeight = 90;
            dgvDetailParam.Columns[3].FillWeight = 200;

            // 컬럼 정렬 기능 비활성화
            foreach (DataGridViewColumn column in dgvDetailParam.Columns)
                column.SortMode = DataGridViewColumnSortMode.NotSortable;

            dgvDetailParam.AllowUserToAddRows = false;
            dgvDetailParam.AllowUserToDeleteRows = false;
            dgvDetailParam.AllowUserToResizeColumns = false;
            dgvDetailParam.AllowUserToResizeRows = false;
            dgvDetailParam.RowHeadersVisible = false;
            dgvDetailParam.SelectionMode = DataGridViewSelectionMode.CellSelect;
        }

        private void displayParamForSelectedModel()
        {
            dgvDetailParam.Rows.Clear();
            if (_selectedModel != null)
            {
                foreach (var defect in _selectedModel.DefectInfo)
                {
                    if (defect.FLTID == null || defect.FLTID.Count == 0)
                    {
                        dgvDetailParam.Rows.Add(defect.SECFLTID, defect.Use, defect.Rate, string.Empty);
                        continue;
                    }
                    string fltIds = string.Join(", ", defect.FLTID);
                    dgvDetailParam.Rows.Add(defect.SECFLTID, defect.Use, defect.Rate, fltIds ?? string.Empty);
                }

                tbxIP.Text = _selectedModel.MainIP;
                tbxMainFolderName.Text = _selectedModel.MainFolderName;
                tbxSubFolderName.Text = _selectedModel.SubFolderName;
                lblPathView.Text = _selectedModel.NetPathSummery("CTLNO");
            }

        }

        #endregion

        private void btnAddSecFltId_Click(object sender, EventArgs e)
        {
            dgvDetailParam.Rows.Add("", false, "", "");
        }

        private void btnDeleteParam_Click(object sender, EventArgs e)
        {
            if (dgvDetailParam.CurrentCell != null)
            {
                int rowIndex = dgvDetailParam.CurrentCell.RowIndex;
                if (rowIndex >= 0 && rowIndex < dgvDetailParam.Rows.Count)
                {
                    dgvDetailParam.Rows.RemoveAt(rowIndex);
                }
            }
        }

        private void btnApplyParam_Click(object sender, EventArgs e)
        {
            List<SjModelItem> info = new List<SjModelItem>();

            foreach (DataGridViewRow row in dgvDetailParam.Rows)
            {
                string secfltId = row.Cells[0].Value?.ToString().Trim() ?? string.Empty;
                bool use = row.Cells[1].Value is bool boolValue && boolValue;

                double rate;
                if (!double.TryParse(row.Cells[2].Value?.ToString().Trim(), out rate))
                    rate = 90.0;

                string ids = row.Cells[3].Value?.ToString() ?? string.Empty;

                string[] fltid = ids.Split(',')
                    .Select(x => x.Trim())
                    .Where(x => !string.IsNullOrEmpty(x))
                    .ToArray();

                SjModelItem item = new SjModelItem
                {
                    SECFLTID = secfltId,
                    Use = use,
                    Rate = rate,
                    FLTID = fltid.ToList()
                };
                info.Add(item);
            }

            _selectedModel.MainIP = tbxIP.Text.Trim();
            _selectedModel.MainFolderName = tbxMainFolderName.Text.Trim();
            _selectedModel.SubFolderName = tbxSubFolderName.Text.Trim();

            if (_selectedModel != null)
                _selectedModel.DefectInfo = info;

            lblPathView.Text = _selectedModel.NetPathSummery("CTLNO");
        }
        #endregion

        #region Control
        private void btnLoad_Click(object sender, EventArgs e)
        {
            _param.Load();
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            _param.Save();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;

            this.Close();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;

            this.Close();
        }
        #endregion

    }
}
