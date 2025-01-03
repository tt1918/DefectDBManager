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
            initFilterCtrl();

            lblTitle.MouseDown += lblTitle_MouseDown;
            lblTitle.MouseMove += lblTitle_MouseMove;

            _lotManager = manager;
            _procIdx = (int)proc;
        }

        private void FormProductFilter_Load(object sender, EventArgs e)
        {
            if(this.Visible==true)
            {
                displayFilterCtrl();
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

        #region Data Grid View Filter Info
        static string[] _strDgvMaterial = { "No", "LINE", "PRODUCT NAME", "MODEL"};
        static int[] _DgvMaterialLength = { 60, 100, 250, 100 };
        enum eDgvFilter { No, Line, Product, Model, Total};

        private DataGridViewComboBoxCell makeComboBoxCell(string[] names, string trgName)
        {
            DataGridViewComboBoxCell cbCell = new DataGridViewComboBoxCell();
            cbCell.DisplayStyle = DataGridViewComboBoxDisplayStyle.ComboBox;
            foreach (string name in names)
                cbCell.Items.Add(name);
            cbCell.Value = trgName;
            return cbCell;
        }

        private void initFilterCtrl()
        {
            try
            {
                dgvFilter.SelectionMode = DataGridViewSelectionMode.FullRowSelect;
                dgvFilter.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dgvFilter.AllowUserToAddRows = false;
                dgvFilter.RowHeadersVisible = false;
                dgvFilter.ColumnCount = (int)eDgvFilter.Total;
                for (int i = 0; i < dgvFilter.ColumnCount; i++)
                {
                    dgvFilter.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
                    dgvFilter.Columns[i].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;

                    dgvFilter.Columns[i].Name = _strDgvMaterial[i];
                    dgvFilter.Columns[i].Width = _DgvMaterialLength[i];
                }

                dgvFilter.Columns[(int)eDgvFilter.No].ReadOnly = true;
            }
            catch
            {

            }
        }

        private void displayFilterCtrl()
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

                        if(info.Name == item.Line)
                        {
                            lncd = info.LNCD;

                            foreach (var product in info.Material.Items)
                                material.Add(product);
                        }
                    }

                    foreach(var data in _lotManager.ProcSetting.Data)
                    {
                        // 해당 라인 코드에 맞는 모델만 추가한다.
                        if(data.Reference.LNCD == lncd)
                            model.Add(data.Name);
                    }
                    
                    object[] s = new object[(int)eDgvFilter.Total];
                    s[(int)eDgvFilter.No] = idx.ToString();
                    s[(int)eDgvFilter.Line] = makeComboBoxCell(lineName.ToArray(), item.Line);
                    s[(int)eDgvFilter.Product] = makeComboBoxCell(material.ToArray(), item.Product);
                    s[(int)eDgvFilter.Model] = makeComboBoxCell(model.ToArray(), item.Model);
                    dgvFilter.Rows.Add(s);
                    idx++;
                }
            }
            catch
            {

            }
        }

        private void updateFilterCtrl()
        {
            try
            {
                DefectDBManager.Preproc.ProcFilterList list = _lotManager.CrtProcFilter[_procIdx];

                list.Clear();

                foreach (DataGridViewRow item in dgvFilter.Rows)
                {
                    DefectDBManager.Preproc.ProcFilter filter = new ProcFilter();
                    filter.Line = item.Cells[(int)eDgvFilter.Line].FormattedValue as string;
                    filter.Product = item.Cells[(int)eDgvFilter.Product].FormattedValue as string;
                    filter.Model = item.Cells[(int)eDgvFilter.Model].FormattedValue as string;
                    list.Add(filter);
                }
                _lotManager.CrtProcFilter[_procIdx] = list;
            }
            catch
            {

            }
        }

        private void addFilter()
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

                object[] s = new object[(int)eDgvFilter.Total];
                s[(int)eDgvFilter.Line] = makeComboBoxCell(lineName.ToArray(), "");
                s[(int)eDgvFilter.Product] = makeComboBoxCell(material.ToArray(), "");
                s[(int)eDgvFilter.Model] = makeComboBoxCell(model.ToArray(), "");

                dgvFilter.Rows.Add(s);
                int index = dgvFilter.Rows.Count - 1;
                dgvFilter.Rows[index].Cells[(int)eDgvFilter.No].Value = index.ToString();
                dgvFilter.Rows[index].Cells[(int)eDgvFilter.Line] = s[(int)eDgvFilter.Line] as DataGridViewComboBoxCell;
                dgvFilter.Rows[index].Cells[(int)eDgvFilter.Product] = s[(int)eDgvFilter.Product] as DataGridViewComboBoxCell;
                dgvFilter.Rows[index].Cells[(int)eDgvFilter.Model] = s[(int)eDgvFilter.Model] as DataGridViewComboBoxCell;
            }
            catch
            {

            }
            finally
            {
                dgvFilter.ResumeLayout();
            }
        }

        private void deleteFilter()
        {
            dgvFilter.SuspendLayout();
            try
            {
                int idx = dgvFilter.SelectedRows[0].Index;
                if (idx != 0)
                {
                    dgvFilter.Rows.RemoveAt(idx);

                    for (int i = 0; i < dgvFilter.Rows.Count; i++)
                        dgvFilter.Rows[i].Cells[(int)eDgvFilter.No].Value = i.ToString();
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

            return  makeComboBoxCell(material.ToArray(), "");
        }

        private object makeModelCombobox(int row)
        {
            string line = dgvFilter.Rows[row].Cells[(int)eDgvFilter.Line].EditedFormattedValue as string;
            string product = dgvFilter.Rows[row].Cells[(int)eDgvFilter.Product].EditedFormattedValue as string;
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

            foreach(var data in _lotManager.ProcSetting.Data)
            {
                if(data.Reference.LNCD == lncd)
                    strings.Add(data.Name);
            }
            return makeComboBoxCell(strings.ToArray(), "");
        }

        private void dgvFilter_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            int colIdx = dgvFilter.SelectedCells[0].ColumnIndex;
            int rowIdx = dgvFilter.SelectedCells[0].RowIndex;
            string name = dgvFilter.SelectedCells[0].EditedFormattedValue as string;

            switch ((eDgvFilter)colIdx)
            {
                case eDgvFilter.Line:
                    {
                        dgvFilter.Rows[rowIdx].Cells[(int)eDgvFilter.Product] = makeProductComboBox(name, rowIdx, colIdx) as DataGridViewComboBoxCell;
                        dgvFilter.Rows[rowIdx].Cells[(int)eDgvFilter.Model] = makeModelCombobox(rowIdx) as DataGridViewComboBoxCell;
                        break;
                    }

                case eDgvFilter.Product:
                    {                        
                        break;
                    }

                case eDgvFilter.Model:
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

        private void btnAddMaterial_Click(object sender, EventArgs e)
        {
            addFilter();
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
            DialogResult = DialogResult.Cancel;
            Close();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {

            updateFilterCtrl();

            DialogResult = DialogResult.OK;
            Close();
        }
        #endregion

    }
}
