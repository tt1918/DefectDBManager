using DefectDBManager.Preproc;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace MarkCompare
{
    public partial class FormLNCD : Form
    {

        #region Param
        public string CultureCode="";
        public DefectDBManager.Preproc.PreprocLNCD MaterialDate
        {
            get { return _materialData; }
            set { _materialData = value; }
        }
        DefectDBManager.Preproc.PreprocLNCD _materialData = null;

        #endregion

        #region Form Control
        public FormLNCD()
        {
            InitializeComponent();
            initDataList();
            initMaterialCtrl();

            lblTitle.MouseDown += lblTitle_MouseDown;
            lblTitle.MouseMove += lblTitle_MouseMove;

            _materialData = new PreprocLNCD();
            _materialData.Load();

            tbSymbol._TextChanged += this.tbSymbolTextChanged;

            UpdateLanguage(CultureCode);
        }

        private void FormLNCD_Load(object sender, EventArgs e)
        {
            if(this.Visible==true)
            {
                displayDataList();
                displayMaterialCtrl();
                displaySymbolData();
                displayIpData();
            }
        }

        private void FormLNCD_FormClosing(object sender, FormClosingEventArgs e)
        {
            tbSymbol._TextChanged -= this.tbSymbolTextChanged;
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

        #region 공통 사용 함수
        private int getValidTaskIdx(string name)
        {
            int selIdx = -1;
            for (int i = 0; i < _materialData.Info.Count; i++)
            {
                if (_materialData[i].Name == name) selIdx = i;
            }

            return selIdx;
        }
        #endregion

        #region Model Name List
        readonly string[] ListModelHeader = { "No.", "Name" };
        readonly int[] ListModelWidth = { 40, 170 };

        private string _selSetName = "";
        private void initDataList()
        {
            try
            {
                lvLNCDList.View = View.Details;
                lvLNCDList.FullRowSelect = true;
                for (int i = 0; i < ListModelHeader.Length; i++)
                    lvLNCDList.Columns.Add(ListModelHeader[i], ListModelWidth[i], HorizontalAlignment.Center);
            }
            catch
            {

            }
        }

        private void displayDataList()
        {
            try
            {
                lvLNCDList.BeginUpdate();
                lvLNCDList.Items.Clear();
                lblSelLNCD.Text = _selSetName;
                int count = 0;
                if (_materialData == null) return;
                foreach (var data in _materialData.Info)
                {
                    ListViewItem item = new ListViewItem($"{++count}");
                    item.SubItems.Add(data.Name);
                    lvLNCDList.Items.Add(item);
                }
            }
            finally
            {
                lvLNCDList.EndUpdate();
            }
        }

        private void lvLNCDList_DoubleClick(object sender, EventArgs e)
        {
            try
            {
                _selSetName = "";

                if (lvLNCDList.SelectedItems.Count == 0) return;

                int index = lvLNCDList.SelectedItems[0].Index;
                _selSetName = lvLNCDList.Items[index].SubItems[1].Text;
                lblSelLNCD.Text = _selSetName;

                // 영상 표시
                displayMaterialCtrl();
                displaySymbolData();
                displayIpData();
            }
            catch
            {

            }
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            FormAddDel form = new FormAddDel(Lang.formAddDelTitle, Lang.formAddDelName, Lang.formAddDelOK, Lang.formAddDelCancel);
            form.CultureCode = CultureCode;
            if (form.ShowDialog() != DialogResult.OK) return;

            if (form.DataName == "")
            {
                MessageBox.Show(Lang.formAddEmptyProcessName, Lang.warning);
                return;
            }

            if (form.DataName.Contains('_'))
            {
                MessageBox.Show(Lang.NamesCannotContain, Lang.warning);
                return;
            }

            PreprocLNCDInfo item = new PreprocLNCDInfo();
            item.Name = form.DataName;
            _materialData.Info.Add(item);

            displayDataList();
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            int index = lvLNCDList.SelectedItems[0].Index;
            string name = lvLNCDList.SelectedItems[0].SubItems[1].Text;
            if (MessageBox.Show($"{name} {Lang.deleteData1}", Lang.deleteData2, MessageBoxButtons.YesNo) == DialogResult.No)
                return;

            _materialData.Remove(name);

            displayDataList();
        }

        private void btnLoad_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show(Lang.loadData1, Lang.loadData2, MessageBoxButtons.YesNo) == DialogResult.No)
                return;

            _materialData.Load();

            displayDataList();
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show(Lang.saveCurrentData1, Lang.saveCurrentData2, MessageBoxButtons.YesNo) == DialogResult.No)
                return;

            _materialData.Save();
            SystemLog.DisplayFileServerLog(Lang.doneSavingData);
        }
        #endregion

        #region Data Grid View Material Info
        static string[] _strDgvMaterial = { "No", "Name"};
        static int[] _DgvMaterialLength = { 50, 200 };
        enum eDgvMaterial { No, Name, Total};

        private void initMaterialCtrl()
        {
            try
            {
                dgvMaterial.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
                dgvMaterial.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dgvMaterial.AllowUserToAddRows = false;
                dgvMaterial.RowHeadersVisible = false;
                dgvMaterial.ColumnCount = (int)eDgvMaterial.Total;
                for (int i = 0; i < dgvMaterial.ColumnCount; i++)
                {
                    dgvMaterial.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
                    dgvMaterial.Columns[i].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;

                    dgvMaterial.Columns[i].Name = _strDgvMaterial[i];
                    dgvMaterial.Columns[i].Width = _DgvMaterialLength[i];
                }

                dgvMaterial.Columns[(int)eDgvMaterial.No].ReadOnly = true;
            }
            catch
            {

            }
        }

        private void displayMaterialCtrl()
        {
            try
            {
                int selProcIdx = getValidTaskIdx(_selSetName);
                if (selProcIdx == -1 || _selSetName == "") return;

                PreprocLNCDInfo info = MaterialDate[selProcIdx];
                tbLNCD.Texts = info.LNCD;

                dgvMaterial.Rows.Clear();
                int idx = 0;
                foreach (var item in info.Material.Items)
                {
                    string[] s = new string[(int)eDgvMaterial.Total];
                    s[(int)eDgvMaterial.No] = idx.ToString();
                    s[(int)eDgvMaterial.Name] = item;
                    dgvMaterial.Rows.Add(s);
                    idx++;
                }
            }
            catch
            {

            }
        }

        private void updateMaterialCtrl()
        {
            int selProcIdx = getValidTaskIdx(_selSetName);

            PreprocLNCDInfo material = _materialData[selProcIdx];

            material.LNCD = tbLNCD.Texts;
            material.Material.Name = material.LNCD;

            List<string> listInfo = new List<string>();
            foreach (DataGridViewRow item in dgvMaterial.Rows)
            {
                listInfo.Add(item.Cells[(int)eDgvMaterial.Name].Value as string);
            }

            material.Material.Items = listInfo;
        }

        private void addMaterial()
        {
            dgvMaterial.SuspendLayout();
            try
            {
                int idx = dgvMaterial.Rows.Count;
                string[] data = new string[(int)eDgvMaterial.Total];
                data[(int)eDgvMaterial.No] = Convert.ToString(idx);
                data[(int)eDgvMaterial.Name] = "";
                dgvMaterial.Rows.Add(data);
            }
            catch
            {

            }
            finally
            {
                dgvMaterial.ResumeLayout();
            }
        }

        private void deleteMaterial()
        {
            dgvMaterial.SuspendLayout();
            try
            {
                int idx = dgvMaterial.SelectedRows[0].Index;
                if (idx != 0)
                {
                    dgvMaterial.Rows.RemoveAt(idx);

                    for (int i = 1; i < dgvMaterial.Rows.Count; i++)
                        dgvMaterial.Rows[i].Cells[(int)eDgvMaterial.No].Value = Convert.ToString(i);
                }
            }
            catch
            {

            }
            finally
            {
                dgvMaterial.ResumeLayout();
            }
        }


        private void btnAddMaterial_Click(object sender, EventArgs e)
        {
            int selProcIdx = getValidTaskIdx(_selSetName);
            if (selProcIdx == -1) return;

            addMaterial();
        }

        private void btnDelMaterial_Click(object sender, EventArgs e)
        {
            int selProcIdx = getValidTaskIdx(_selSetName);
            if (selProcIdx == -1) return;

            deleteMaterial();
        }
        #endregion

        #region Control
        private void btnApply_Click(object sender, EventArgs e)
        {
            try
            {
                int selProcIdx = getValidTaskIdx(_selSetName);
                if (selProcIdx == -1) return;

                updateMaterialCtrl();
                updateSymbolData();

                MessageBox.Show(Lang.applied1);
                SystemLog.DisplayFileServerLog(Lang.applied1);
            }
            catch (Exception ex)
            {
                SystemLog.System.Write(Log.Level.Error, ex.Message);
            }
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            Close();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
            Close();
        }
        #endregion

        #region IP Data
        private void displayIpData()
        {
            int selProcIdx = getValidTaskIdx(_selSetName);
            if (selProcIdx == -1 || _selSetName == "") return;
            
            PreprocLNCDInfo info = MaterialDate[selProcIdx];
            lblCheckIPData.Text = info.TargetIP;
            lblCheckDurationData.Text = info.CheckDuration.ToString();
            chbCheckState.Checked = info.CheckStatus;
        }

        private void btnSetStatusCheck_Click(object sender, EventArgs e)
        {
            int selProcIdx = getValidTaskIdx(_selSetName);
            if (selProcIdx == -1 || _selSetName == "") return;
            PreprocLNCDInfo info = MaterialDate[selProcIdx];
            string ip = info.TargetIP;
            int duration = info.CheckDuration;

            using (FormStatusCheckSetting form = new FormStatusCheckSetting(ip, duration))
            {
                form.CultureCode = CultureCode;
                if (form.ShowDialog()==DialogResult.OK)
                {
                    MaterialDate[selProcIdx].TargetIP = form.IP;
                    MaterialDate[selProcIdx].CheckDuration = form.Duration;
                    displayIpData();
                }
            }
        }

        private void chbCheckState_CheckedChanged(object sender, EventArgs e)
        {
            int selProcIdx = getValidTaskIdx(_selSetName);
            if (selProcIdx == -1 || _selSetName == "") return;
            PreprocLNCDInfo info = MaterialDate[selProcIdx];
            MaterialDate[selProcIdx].CheckStatus = chbCheckState.Checked;
        }

        #endregion

        #region Symbol Control
        private Color _symbolColor;
        private void displaySymbolData()
        {
            int selProcIdx = getValidTaskIdx(_selSetName);
            if (selProcIdx == -1 || _selSetName == "") return;

            PreprocLNCDInfo info = MaterialDate[selProcIdx];

            tbSymbol.Texts = info.Symbol;

            _symbolColor = info.SymbolColor;
            Color txtColor = Color.FromArgb(255 - _symbolColor.R, 255 - _symbolColor.G, 255 - _symbolColor.B);
            btnSelectColor.BackColor = _symbolColor;

            string text = $"RGB[{_symbolColor.R},{_symbolColor.G},{_symbolColor.B}]";
            btnSelectColor.Text = text;
            btnSelectColor.TextColor = txtColor;
        }
        private void updateSymbolData()
        {
            int selProcIdx = getValidTaskIdx(_selSetName);
            if (selProcIdx == -1 || _selSetName == "") return;

            MaterialDate[selProcIdx].Symbol = tbSymbol.Texts;
            MaterialDate[selProcIdx].SymbolColor = _symbolColor;
        }

        private void btnSelectColor_Click(object sender, EventArgs e)
        {
            try
            {
                using (ColorDialog dlg = new ColorDialog())
                {
                    if (dlg.ShowDialog() == DialogResult.OK)
                    {
                        _symbolColor = dlg.Color;
                        Color txtColor = Color.FromArgb(255 - _symbolColor.R, 255 - _symbolColor.G, 255 - _symbolColor.B);
                        btnSelectColor.BackColor = _symbolColor;

                        string text = $"RGB[{_symbolColor.R},{_symbolColor.G},{_symbolColor.B}]";
                        btnSelectColor.Text = text;
                        btnSelectColor.TextColor = txtColor;
                    }
                }
            }
            catch
            {

            }
        }

        private void tbSymbolTextChanged(object s, EventArgs e)
        {
            try
            {
                string text = tbSymbol.Texts;
                if (text.Length > 1)
                {
                    tbSymbol.Texts = text.Substring(0, 1);
                }
            }
            catch
            {

            }
        }
        #endregion

        #region 언어
        public void UpdateLanguage(string cultrue)
        {
            string fontName = Functions.GetCultureFontName(cultrue);
            Font newFont = new Font(fontName, 10, FontStyle.Bold);

            lblTitle.Font = newFont;
            lblLNCD.Font = newFont;
            lblSelLNCD.Font = newFont;
            lblSymbol.Font = newFont;
            
            lblCheckIP.Font = newFont;
            lblDuration.Font = newFont;
            btnSelectColor.Font = newFont;

            newFont = new Font(fontName, 9);
            chbCheckState.Font = newFont;
            btnSetStatusCheck.Font = newFont;
            btnAdd.Font = newFont;
            btnDelete.Font = newFont;
            btnLoad.Font = newFont;
            btnSave.Font = newFont;
            btnAddMaterial.Font = newFont;
            btnDelMaterial.Font = newFont;
            btnApply.Font = newFont;
            btnCancel.Font = newFont;
            btnOK.Font = newFont;
            lvLNCDList.Font = newFont;
            dgvMaterial.Font = newFont;

            lblTitle.Text = Lang.formLNCDTitle;
            lblSymbol.Text = Lang.formLNCDSymbol;
            lblDuration.Text = Lang.formLNCDCycle;
            chbCheckState.Text = Lang.formLNCDLineCheck;

            btnSetStatusCheck.Text = Lang.setting;
            btnAdd.Text = Lang.btnAdd;
            btnDelete.Text = Lang.btnDel1;
            btnLoad.Text = Lang.btnLoad1;
            btnSave.Text = Lang.btnSave1;

            btnAddMaterial.Text = Lang.btnAdd;
            btnDelMaterial.Text = Lang.btnDel1;

            btnApply.Text = Lang.btnApply;
            btnCancel.Text = Lang.btnCancel;
            btnOK.Text = Lang.btnOK1;

            
            dgvMaterial.Columns[0].Name = Lang.dgvMeterialNo;
            dgvMaterial.Columns[1].Name = Lang.dgvMeterialModel;
        }
        #endregion

    }
}
