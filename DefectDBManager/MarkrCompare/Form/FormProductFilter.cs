using DefectDBManager;
using DefectDBManager.Preproc;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Linq;

namespace MarkrCompare
{
    public partial class FormProductFilter : Form
    {

        #region Param
        DefectDBManager.PreprocLotManager _lotManager = null;
        int _procIdx = -1;
        #endregion

        #region Form Control
        public FormProductFilter(DefectDBManager.PreprocLotManager manager, DefectDBManager.Preproc.eProc proc)
        {
            InitializeComponent();


            lblTitle.MouseDown += lblTitle_MouseDown;
            lblTitle.MouseMove += lblTitle_MouseMove;

            _lotManager = manager;
            _procIdx = (int)proc;

            switch ((DefectDBManager.Preproc.eProc)proc)
            {
                case eProc.Live: initLiveFilterCtrl(); break;
                case eProc.Search: initSearchFilterCtrl(); break;
            }
            initSyncDurationCtrl();
        }

        private void FormProductFilter_Load(object sender, EventArgs e)
        {
            if (this.Visible == true)
            {
                switch ((DefectDBManager.Preproc.eProc)_procIdx)
                {
                    case eProc.Live:  displayLiveFilterCtrl();  break;
                    case eProc.Search: displaySearchFilterCtrl(); break;
                }
            }
        }

        private void FormProductFilter_FormClosing(object sender, FormClosingEventArgs e)
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

        #region Data Grid View Reference
        static string[] _strDgvMaterial = { "No", "LINE", "PRODUCT NAME", "MODEL", "DURATION" };
        static int[] _DgvMaterialLength = { 60, 100, 250, 100, 50 };
        enum eDgvLiveFilter { No, Line, Product, Model, Duration, Total };
        enum eDgvSearchFilter { No, Line, Product, Model, Total };

        private void deleteFilter()
        {
            dgvFilter.SuspendLayout();
            try
            {
                int idx = dgvFilter.SelectedCells[0].RowIndex;
                if (idx >= 0)
                {
                    dgvFilter.Rows.RemoveAt(idx);

                    for (int i = 0; i < dgvFilter.Rows.Count; i++)
                        dgvFilter.Rows[i].Cells[(int)eDgvSearchFilter.No].Value = i.ToString();
                }
            }
            catch
            {

            }
            finally
            {
                dgvFilter.ResumeLayout();
            }
        }

        private void clearFilter()
        {
            dgvFilter.SuspendLayout();
            try
            {
                dgvFilter.Rows.Clear();
            }
            catch
            {

            }
            finally
            {
                dgvFilter.ResumeLayout();
            }
        }
        private DataGridViewComboBoxCell makeComboBoxCell(string[] names, string trgName)
        {
            DataGridViewComboBoxCell cbCell = new DataGridViewComboBoxCell();
            cbCell.DisplayStyle = DataGridViewComboBoxDisplayStyle.ComboBox;
            foreach (string name in names)
                cbCell.Items.Add(name);
            cbCell.Value = trgName;
            return cbCell;
        }
        private object makeProductComboBox(string name, int row, int col)
        {
            List<string> material = new List<string>();

            foreach (var info in _lotManager.ProcLNCD.Info)
            {
                if (info.Name == name)
                {
                    foreach (var product in info.Material.Items)
                        material.Add(product);
                    break;
                }
            }

            return makeComboBoxCell(material.ToArray(), "");
        }

        private object makeModelCombobox(int row)
        {
            string line = dgvFilter.Rows[row].Cells[(int)eDgvSearchFilter.Line].EditedFormattedValue as string;
            string product = dgvFilter.Rows[row].Cells[(int)eDgvSearchFilter.Product].EditedFormattedValue as string;
            string lncd = string.Empty;
            List<string> strings = new List<string>();
            foreach (var info in _lotManager.ProcLNCD.Info)
            {
                if (info.Name == line)
                {
                    lncd = info.LNCD;
                    break;
                }
            }

            string firstName="";
            int cnt = 0;
            foreach (var data in _lotManager.ProcSetting.Data)
            {
                if (cnt == 0) firstName = data.Name;
                strings.Add(data.Name);
            }
            return makeComboBoxCell(strings.ToArray(), firstName);
        }

        private object makeDurationCombobox()
        {
            List<string> duration = new List<string>();
            for (int i = 1; i <= 24; i++)
                duration.Add(i.ToString());

            return makeComboBoxCell(duration.ToArray(), "1");
        }
        private void dgvFilter_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            int colIdx = dgvFilter.SelectedCells[0].ColumnIndex;
            int rowIdx = dgvFilter.SelectedCells[0].RowIndex;
            string name = dgvFilter.SelectedCells[0].EditedFormattedValue as string;

            switch ((eDgvSearchFilter)colIdx)
            {
                case eDgvSearchFilter.Line:
                    {
                        dgvFilter.Rows[rowIdx].Cells[(int)eDgvSearchFilter.Product] = makeProductComboBox(name, rowIdx, colIdx) as DataGridViewComboBoxCell;
                        dgvFilter.Rows[rowIdx].Cells[(int)eDgvSearchFilter.Model] = makeModelCombobox(rowIdx) as DataGridViewComboBoxCell;
                        break;
                    }

                case eDgvSearchFilter.Product:
                    {
                        break;
                    }

                case eDgvSearchFilter.Model:
                    {
                        break;
                    }
            }
        }

        void dgvFilter_CurrentCellDirtyStateChanged(object sender, EventArgs e)
        {
            if (dgvFilter.IsCurrentCellDirty)
            {
                // This fires the cell value changed handler below
                dgvFilter.CommitEdit(DataGridViewDataErrorContexts.Commit);
            }
        }
        #endregion

        #region Data Grid View Search Filter Info
        private void initSearchFilterCtrl()
        {
            try
            {
                dgvFilter.SelectionMode = DataGridViewSelectionMode.CellSelect;
                dgvFilter.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dgvFilter.AllowUserToAddRows = false;
                dgvFilter.RowHeadersVisible = false;
                dgvFilter.ColumnCount = (int)eDgvSearchFilter.Total;
                for (int i = 0; i < dgvFilter.ColumnCount; i++)
                {
                    dgvFilter.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
                    dgvFilter.Columns[i].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;

                    dgvFilter.Columns[i].Name = _strDgvMaterial[i];
                    dgvFilter.Columns[i].Width = _DgvMaterialLength[i];
                }

                dgvFilter.Columns[(int)eDgvSearchFilter.No].ReadOnly = true;
            }
            catch
            {

            }
        }
        private void displaySearchFilterCtrl()
        {
            try
            {
                int size;
                string lncd = string.Empty;
                List<string> lineName = new List<string>();
                List<string> material = new List<string>();
                List<string> model = new List<string>();

                DefectDBManager.Preproc.ProcFilterList list = _lotManager.CrtProcFilter[_procIdx];

                size = list.Count;

                dgvFilter.Rows.Clear();
                int idx = 0;
                foreach (var item in list.Data)
                {
                    lineName.Clear();
                    material.Clear();
                    model.Clear();

                    // 라인 코드 및 라인 코드에 해당하는 품종 데이터 업데이트
                    foreach (var info in _lotManager.ProcLNCD.Info)
                    {
                        lineName.Add(info.Name);

                        if (info.Name == item.Line)
                        {
                            lncd = info.LNCD;

                            foreach (var product in info.Material.Items)
                                material.Add(product);
                        }
                    }

                    foreach (var data in _lotManager.ProcSetting.Data)
                    {
                        // 해당 라인 코드에 맞는 모델만 추가한다.
                        model.Add(data.Name);
                    }

                    object[] s = new object[(int)eDgvSearchFilter.Total];
                    dgvFilter.Rows.Add(s);

                    dgvFilter.Rows[idx].Cells[(int)eDgvSearchFilter.No].Value = idx.ToString();
                    dgvFilter.Rows[idx].Cells[(int)eDgvSearchFilter.Line] = makeComboBoxCell(lineName.ToArray(), item.Line) as DataGridViewComboBoxCell;
                    dgvFilter.Rows[idx].Cells[(int)eDgvSearchFilter.Product] = makeComboBoxCell(material.ToArray(), item.Product) as DataGridViewComboBoxCell;
                    dgvFilter.Rows[idx].Cells[(int)eDgvSearchFilter.Model] = makeComboBoxCell(model.ToArray(), item.Model) as DataGridViewComboBoxCell;

                    idx++;
                }
            }
            catch
            {

            }
        }
        private void updateSearchFilterCtrl()
        {
            try
            {
                DefectDBManager.Preproc.ProcFilterList list = _lotManager.CrtProcFilter[_procIdx];

                list.Clear();

                foreach (DataGridViewRow item in dgvFilter.Rows)
                {
                    DefectDBManager.Preproc.ProcFilter filter = new ProcFilter();
                    filter.Line = item.Cells[(int)eDgvSearchFilter.Line].FormattedValue as string;
                    filter.Product = item.Cells[(int)eDgvSearchFilter.Product].FormattedValue as string;
                    filter.Model = item.Cells[(int)eDgvSearchFilter.Model].FormattedValue as string;
                    list.Add(filter);
                }
                _lotManager.CrtProcFilter[_procIdx] = list;
            }
            catch
            {

            }
        }
        private void addSearchFilter()
        {
            dgvFilter.SuspendLayout();
            try
            {
                List<string> lineName = new List<string>();
                List<string> material = new List<string>();
                List<string> model = new List<string>();

                // 라인 코드 및 라인 코드에 해당하는 품종 데이터 업데이트
                foreach (var info in _lotManager.ProcLNCD.Info)
                {
                    lineName.Add(info.Name);
                }

                object[] s = new object[(int)eDgvSearchFilter.Total];
                dgvFilter.Rows.Add(s);

                int index = dgvFilter.Rows.Count - 1;

                dgvFilter.Rows[index].Cells[(int)eDgvSearchFilter.No].Value = index.ToString();
                dgvFilter.Rows[index].Cells[(int)eDgvSearchFilter.Line] = makeComboBoxCell(lineName.ToArray(), "") as DataGridViewComboBoxCell;
                dgvFilter.Rows[index].Cells[(int)eDgvSearchFilter.Product] = makeComboBoxCell(material.ToArray(), "") as DataGridViewComboBoxCell;
                dgvFilter.Rows[index].Cells[(int)eDgvSearchFilter.Model] = makeComboBoxCell(model.ToArray(), "") as DataGridViewComboBoxCell;
            }
            catch
            {

            }
            finally
            {
                dgvFilter.ResumeLayout();
            }
        }
        #endregion

        #region Data Grid View Live Filter Info
        private void initLiveFilterCtrl()
        {
            try
            {
                dgvFilter.SelectionMode = DataGridViewSelectionMode.CellSelect;
                dgvFilter.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dgvFilter.AllowUserToAddRows = false;
                dgvFilter.RowHeadersVisible = false;
                dgvFilter.ColumnCount = (int)eDgvLiveFilter.Total;
                for (int i = 0; i < dgvFilter.ColumnCount; i++)
                {
                    dgvFilter.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
                    dgvFilter.Columns[i].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;

                    dgvFilter.Columns[i].Name = _strDgvMaterial[i];
                    dgvFilter.Columns[i].Width = _DgvMaterialLength[i];
                }

                dgvFilter.Columns[(int)eDgvLiveFilter.No].ReadOnly = true;
            }
            catch
            {

            }
        }
        private void displayLiveFilterCtrl()
        {
            try
            {
                int size;
                string lncd = string.Empty;
                List<string> lineName = new List<string>();
                List<string> material = new List<string>();
                List<string> model = new List<string>();

                DefectDBManager.Preproc.ProcFilterList list = _lotManager.CrtProcFilter[_procIdx];

                size = list.Count;

                dgvFilter.Rows.Clear();
                int idx = 0;
                foreach (var item in list.Data)
                {
                    lineName.Clear();
                    material.Clear();
                    model.Clear();

                    // 라인 코드 및 라인 코드에 해당하는 품종 데이터 업데이트
                    foreach (var info in _lotManager.ProcLNCD.Info)
                    {
                        lineName.Add(info.Name);

                        if (info.Name == item.Line)
                        {
                            lncd = info.LNCD;

                            foreach (var product in info.Material.Items)
                                material.Add(product);
                        }
                    }

                    foreach (var data in _lotManager.ProcSetting.Data)
                    {
                        // 해당 라인 코드에 맞는 모델만 추가한다.
                        model.Add(data.Name);
                    }

                    object[] s = new object[(int)eDgvLiveFilter.Total];
                    dgvFilter.Rows.Add(s);

                    dgvFilter.Rows[idx].Cells[(int)eDgvLiveFilter.No].Value = idx.ToString();
                    dgvFilter.Rows[idx].Cells[(int)eDgvLiveFilter.Line] = makeComboBoxCell(lineName.ToArray(), item.Line) as DataGridViewComboBoxCell;
                    dgvFilter.Rows[idx].Cells[(int)eDgvLiveFilter.Product] = makeComboBoxCell(material.ToArray(), item.Product) as DataGridViewComboBoxCell;
                    dgvFilter.Rows[idx].Cells[(int)eDgvLiveFilter.Model] = makeComboBoxCell(model.ToArray(), item.Model) as DataGridViewComboBoxCell;
                    dgvFilter.Rows[idx].Cells[(int)eDgvLiveFilter.Duration] = makeDurationCombobox() as DataGridViewComboBoxCell;

                    idx++;
                }
            }
            catch
            {

            }
        }
        private void updateLiveFilterCtrl()
        {
            try
            {
                DefectDBManager.Preproc.ProcFilterList list = _lotManager.CrtProcFilter[_procIdx];

                list.Clear();

                foreach (DataGridViewRow item in dgvFilter.Rows)
                {
                    DefectDBManager.Preproc.ProcFilter filter = new ProcFilter();
                    filter.Line = item.Cells[(int)eDgvLiveFilter.Line].FormattedValue as string;
                    filter.Product = item.Cells[(int)eDgvLiveFilter.Product].FormattedValue as string;
                    filter.Model = item.Cells[(int)eDgvLiveFilter.Model].FormattedValue as string;
                    filter.Duration = Convert.ToInt32(item.Cells[(int)eDgvLiveFilter.Duration].FormattedValue as string);
                    list.Add(filter);
                }
                _lotManager.CrtProcFilter[_procIdx] = list;
            }
            catch
            {

            }
        }
        private void addLiveFilter()
        {
            dgvFilter.SuspendLayout();
            try
            {
                List<string> lineName = new List<string>();
                List<string> material = new List<string>();
                List<string> model = new List<string>();

                // 라인 코드 및 라인 코드에 해당하는 품종 데이터 업데이트
                foreach (var info in _lotManager.ProcLNCD.Info)
                {
                    lineName.Add(info.Name);
                }

                object[] s = new object[(int)eDgvLiveFilter.Total];
                dgvFilter.Rows.Add(s);

                int index = dgvFilter.Rows.Count - 1;

                dgvFilter.Rows[index].Cells[(int)eDgvLiveFilter.No].Value = index.ToString();
                dgvFilter.Rows[index].Cells[(int)eDgvLiveFilter.Line] = makeComboBoxCell(lineName.ToArray(), "") as DataGridViewComboBoxCell;
                dgvFilter.Rows[index].Cells[(int)eDgvLiveFilter.Product] = makeComboBoxCell(material.ToArray(), "") as DataGridViewComboBoxCell;
                dgvFilter.Rows[index].Cells[(int)eDgvLiveFilter.Model] = makeComboBoxCell(model.ToArray(), "") as DataGridViewComboBoxCell;
                dgvFilter.Rows[index].Cells[(int)eDgvLiveFilter.Duration] = makeDurationCombobox() as DataGridViewComboBoxCell;
            }
            catch
            {

            }
            finally
            {
                dgvFilter.ResumeLayout();
            }
        }
        #endregion

        #region Sync Duration 
        private void initSyncDurationCtrl()
        {
            if((DefectDBManager.Preproc.eProc)_procIdx== DefectDBManager.Preproc.eProc.Search)
            {
                lblSyncDuration.Hide();
                cbSyncDuration.Hide();
                comboSyncDuration.Hide();
                return;
            }

            for (int i = 1; i <= 24; i++)
                comboSyncDuration.Items.Add(i.ToString());
            comboSyncDuration.SelectedItem = _lotManager.CrtProcFilter.SyncDuration.ToString();
            cbSyncDuration.Checked = _lotManager.CrtProcFilter.UseLiveSync;
        }
        private void updateSyncDurationCtrl()
        {
            _lotManager.CrtProcFilter.SyncDuration = Convert.ToInt32(comboSyncDuration.SelectedItem.ToString());
            _lotManager.CrtProcFilter.UseLiveSync = cbSyncDuration.Checked;
        }
        #endregion

        #region Control
        private void btnAddMaterial_Click(object sender, EventArgs e)
        {
            switch ((DefectDBManager.Preproc.eProc)_procIdx)
            {
                case eProc.Live:    addLiveFilter(); break;
                case eProc.Search:  addSearchFilter(); break;
            }
        }
        private void btnDelMaterial_Click(object sender, EventArgs e)
        {
            deleteFilter();
        }
        private void btnClear_Click(object sender, EventArgs e)
        {
            clearFilter();
        }
        private void btnCancel_Click(object sender, EventArgs e)
        {
            //_lotManager.LoadFilterSet();
            DialogResult = DialogResult.Cancel;
            Close();
        }
        private void btnOK_Click(object sender, EventArgs e)
        {
            switch ((DefectDBManager.Preproc.eProc)_procIdx)
            {
                case eProc.Live:
                    {
                        updateSyncDurationCtrl();
                        updateLiveFilterCtrl();
                        break;
                    }
                case eProc.Search:  updateSearchFilterCtrl(); break;
            }
            
            _lotManager.CrtProcFilter.Save();
            DialogResult = DialogResult.OK;
            Close();
        }
        #endregion

    }
}
