using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.Entity;
using System.Data.Entity.Core.Common.CommandTrees.ExpressionBuilder;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;
using static System.Windows.Forms.AxHost;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;
using ListView = System.Windows.Forms.ListView;

namespace DefectDBManager
{
    public partial class FormEditDefect : Form
    {
        #region const param
        readonly string[] DE_Header = { " ", "마킹", "FALTID", "SIZE", "PPCD", "ROLLNAME" };
        readonly int[] listDE_Width = { 30, 60, 60, 60, 60, 100};
        #endregion

        public NittoDB _DataBase { get; set; }

        private Dictionary<int, MRKCTLMSTData> _mrk_de;

        public FormEditDefect()
        {
            InitializeComponent();


            panelTitle.MouseDown += lblTitle_MouseDown;
            panelTitle.MouseMove += lblTitle_MouseMove;

            _mrk_de = new Dictionary<int, MRKCTLMSTData>();

            initDataGridView();
        }

        private void FormEditDefect_VisibleChanged(object sender, EventArgs e)
        {
            if (this.Visible == true)
            {
                UpdateLanguage();
                GetItems();
            }
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

        private void btnClearAll_Click(object sender, EventArgs e)
        {
            resetAllCheck();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            UpdateDefectEditedData();
            DialogResult = DialogResult.OK;
            this.Close();
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void initDataGridView()
        {
            dgvDefect.RowCount = 0;
            dgvDefect.SelectionMode = DataGridViewSelectionMode.CellSelect;

            foreach (DataGridViewColumn column in dgvDefect.Columns)
            {
                column.SortMode = DataGridViewColumnSortMode.NotSortable;
            }

            int index = 1;
            foreach (DataGridViewRow rowHeader in dgvDefect.Rows)
            {
                rowHeader.HeaderCell.Value = String.Format("{0}", index);
                index++;
            }

            dgvDefect.ClearSelection();
        }

        private void addItem(MRKCTLMSTData data)
        {
            string[] strValue = new string[6];
            if (data.MRKF1 == "1") strValue[0] = "True";
            else strValue[0] = "False";
            strValue[1] = $"{data.LNCD}";
            strValue[2] = $"{data.FLTID}";
            strValue[3] = $"{data.SIZE}";
            strValue[4] = $"{data.PPCD}";
            strValue[5] = $"{data.ROLLNAME}";
            dgvDefect.Rows.Add(strValue);
        }

        #region DB
        private bool GetItems()
        {
            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            int resCnt = 0;
            string logData;
            try
            {
                this.dgvDefect.Rows.Clear();
                _mrk_de.Clear();

                dgvDefect.SuspendLayout();

                for (int i = 0; i < count; i++)
                {
                    for (int j = 0; j < _DataBase._DbResult._MRKCTLMST_DE[i].Count; j++)
                    {
                        if (_DataBase._DbResult._MRKCTLMST_DE[i][j].query == "")
                            continue;

                        using (var comm = new OracleCommand(_DataBase._DbResult._MRKCTLMST_DE[i][j].query, _DataBase.Conn.Connection))
                        {
                            using (var reader = comm.ExecuteReader())
                            {
                                while (reader.Read())
                                {
                                    MRKCTLMSTData data = new MRKCTLMSTData();
                                    data.Parse(reader);
                                    addItem(data);
                                    _DataBase._DbResult._MRKCTLMST_DE[i][j].data.Add(data);

                                    _mrk_de.Add(resCnt, data);

                                    logData = string.Format($"{resCnt}\t-\t{data.ToString()}");
                                    this._DataBase._LOG.WriteLoadData(logData, resCnt, "MRKCTLMST-EDIT", 0.0);
                                    resCnt++;
                                }
                            }
                        }
                    }
                }

                dgvDefect.ResumeLayout();
                return true;
            }
            catch(Exception ex)
            {
                string log = $"[ERROR] MRKCTLMST-EDIT : {ex.Message}";
                Log.Write(log);
                dgvDefect.ResumeLayout();
                return false;
            }
        }

        private void UpdateDefectEditedData()
        {
            int count = this.dgvDefect.Rows.Count;
            for (int i = 0; i < count; i++)
            {
                DataGridViewRow dataGridViewRow = this.dgvDefect.Rows[i];
                MRKCTLMSTData data = _mrk_de[i];

                if (dataGridViewRow.Cells[0].Value.ToString().ToUpper() == "FALSE") data.MRKF1 = "0";
                else data.MRKF1 = "1";

                data.LNCD = dataGridViewRow.Cells[1].Value.ToString();
                data.FLTID = dataGridViewRow.Cells[2].Value.ToString();
                if (float.TryParse(dataGridViewRow.Cells[3].Value.ToString(), out float val) == true)
                    data.SIZE = val;

                data.PPCD = dataGridViewRow.Cells[4].Value.ToString();
                data.ROLLNAME = dataGridViewRow.Cells[5].Value.ToString();

                _mrk_de[i] = data;
            }
        }
        #endregion

        private void dgvDefect_CellDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            int row = e.RowIndex;
            int col = e.ColumnIndex;

            if (col <= 0 || row < 0) return;

            string text = this.dgvDefect[col, row].FormattedValue.ToString();
            using(FormDefectItemEditor form = new FormDefectItemEditor())
            {
                form.EditData = text;
                if(form.ShowDialog()==DialogResult.OK)
                {
                    this.dgvDefect.Rows[row].Cells[col].Value = form.EditData;
                }
            }
        }

        private void resetAllCheck()
        {
            this.dgvDefect.SuspendLayout();
            int count = this.dgvDefect.Rows.Count;
            for (int i = 0; i < count; i++)
            {
                this.dgvDefect.Rows[i].Cells[0].Value = "False";
            }
            this.dgvDefect.ResumeLayout();
        }

        #region Language Update
        public void UpdateLanguage()
        {
            this.SuspendLayout();
            lblTitle.Text = Language.DefectEditorTitle;
            btnClearAll.Text = Language.ClearAll;
            btnOK.Text = Language.OK;
            btnClose.Text = Language.Close;
            this.ResumeLayout();
        }
        #endregion Language Update
    }
}
