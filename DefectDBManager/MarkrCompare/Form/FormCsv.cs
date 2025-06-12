using DefectDBManager.Preproc;
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

namespace MarkCompare
{
    public partial class FormCsv : Form
    {
        public List<string> Csv = new List<string>();
        const string CsvListPath = @"C:\COSS\Csv List\";

        public CSVProcParam _procItem = null;

        public FormCsv(CSVProcParam csvParam)
        {
            _procItem = csvParam;

            InitializeComponent();
            InitGridView();
            initDgvCompRange();

            displayDgvCompRange();
            displayJudgeRange();
            displayOtherParam();
            rdTypeCheckedChaged();

            UpdateLanguage();

            lblTitle.MouseDown += lblTitle_MouseDown;
            lblTitle.MouseMove += lblTitle_MouseMove;
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

        void InitGridView()
        {
            dgvCSVList.ColumnHeadersVisible = true;
            dgvCSVList.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;

            dgvCSVList.RowHeadersVisible = true;

            dgvCSVList.Columns.Add("Path", "Csv Path");
            dgvCSVList.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            updateOtherParam();

            Csv.Clear();

            foreach (DataGridViewRow row in dgvCSVList.Rows)
            {
                Csv.Add(row.Cells[0].Value.ToString());
            }
            DialogResult = DialogResult.OK;
            this.Close();
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            if (dgvCSVList.Rows.Count >= 100)
            {
                MessageBox.Show(Lang.maximumCsvFiles);
                return;
            }

            OpenFileDialog open = new OpenFileDialog();
            if (open.ShowDialog() == DialogResult.OK)
            {
                dgvCSVList.Rows.Add();
                dgvCSVList[0, dgvCSVList.RowCount - 1].Value = open.FileName;
                dgvCSVList.FirstDisplayedScrollingRowIndex = dgvCSVList.RowCount - 1;
            }
            rdTypeCheckedChaged();
        }

        private void btnDel_Click(object sender, EventArgs e)
        {
            if (dgvCSVList.Rows.Count == 0 || dgvCSVList.SelectedRows.Count == 0) 
            {
                MessageBox.Show(Lang.noSelectedRow);
                return; 
            }

            dgvCSVList.CommitEdit(DataGridViewDataErrorContexts.Commit);

            foreach (DataGridViewRow item in dgvCSVList.SelectedRows)
            {
                dgvCSVList.Rows.Remove(item);
            }
            rdTypeCheckedChaged();
        }

        private void dataGridView1_RowPostPaint(object sender, DataGridViewRowPostPaintEventArgs e)
        {
            // 현재 행의 인덱스를 가져옴 (1부터 시작하려면 +1)
            string rowIndex = (e.RowIndex + 1).ToString();

            // 인덱스를 표시할 위치 계산
            System.Drawing.Font rowFont = dgvCSVList.Font;
            var textSize = e.Graphics.MeasureString(rowIndex, rowFont);
            var location = new System.Drawing.PointF(
                e.RowBounds.Left + 15,  // 왼쪽 여백 조정
                e.RowBounds.Top + (e.RowBounds.Height - textSize.Height) / 2 // 중앙 정렬
            );

            // 인덱스 그리기
            e.Graphics.DrawString(rowIndex, rowFont, System.Drawing.Brushes.Black, location);
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            dgvCSVList.Rows.Clear();
            rdTypeCheckedChaged();
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            FormAddDel form = new FormAddDel(Lang.btnSave1, Lang.fileName, Lang.btnOK1, Lang.btnCancel1);
            if(form.ShowDialog() == DialogResult.Cancel) return;

            if (string.IsNullOrWhiteSpace(form.DataName))
            {
                MessageBox.Show(Lang.InsertFileName);
                return;
            }

            if (!Directory.Exists(CsvListPath))
                Directory.CreateDirectory(CsvListPath);

            string path = Path.Combine(CsvListPath, form.DataName + ".txt");

            using (StreamWriter sw = new StreamWriter(path)) 
            {
                foreach (DataGridViewRow row in dgvCSVList.Rows)
                {
                    if (string.IsNullOrWhiteSpace(row.Cells[0].Value.ToString())) continue;
                    sw.WriteLine(row.Cells[0].Value.ToString());
                }
            }
        }

        private void btnLoad_Click(object sender, EventArgs e)
        {
            OpenFileDialog open = new OpenFileDialog();
            open.InitialDirectory = CsvListPath;
            if (open.ShowDialog() == DialogResult.OK)
            {
                dgvCSVList.Rows.Clear();
                string[] csv = File.ReadAllLines(open.FileName);
                foreach (string line in csv) 
                {
                    dgvCSVList.Rows.Add();
                    dgvCSVList[0, dgvCSVList.RowCount - 1].Value = line;
                }
            }
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
            List<CompRange> range = _procItem.CompRange;

            string[] sR = new string[(int)eDgvCompRange.Total];
            sR[(int)eDgvCompRange.No] = "R";

            sR[(int)eDgvCompRange.MinX] = _procItem.BasicRange.MinXRange.ToString();
            sR[(int)eDgvCompRange.MaxX] = _procItem.BasicRange.MaxXRange.ToString();
            sR[(int)eDgvCompRange.MinY] = _procItem.BasicRange.MinYRange.ToString();
            sR[(int)eDgvCompRange.MaxY] = _procItem.BasicRange.MaxYRange.ToString();
            sR[(int)eDgvCompRange.Rate] = _procItem.BasicRange.Accuracy.ToString();
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
                    CompRange rangeData = _procItem.BasicRange;
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

            _procItem.CompRange = rangeList;
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
            _procItem.Judge.X = (float)Convert.ToDouble(tbJudgeRangeX.Texts);
            _procItem.Judge.Y = (float)Convert.ToDouble(tbJudgeRangeY.Texts);
        }

        /// <summary>
        /// Ctrl에 현재 정보를 업데이트 한다.
        /// </summary>
        private void displayJudgeRange()
        {
            try
            {
                tbJudgeRangeX.Texts = _procItem.Judge.X.ToString();
                tbJudgeRangeY.Texts = _procItem.Judge.Y.ToString();
            }
            catch
            {

            }
        }
        #endregion

        #region Other Param
        private void updateOtherParam()
        {
            _procItem.UseAiResult = cbUseMNTTAN.Checked;
            if (rbType1.Checked) _procItem.CompType = 0;
            else if (rbType2.Checked) _procItem.CompType = 1;
            else if (rbType3.Checked) _procItem.CompType = 2;
            else if (rbType4.Checked) _procItem.CompType = 3;
        }

        private void displayOtherParam()
        {
            cbUseMNTTAN.Checked = _procItem.UseAiResult;
        }

        private void rdTypeCheckedChaged()
        {
            if(dgvCSVList.Rows.Count==1)
            {
                rbType1.Enabled = false;
                rbType2.Enabled = true;
                rbType3.Enabled = true;
                rbType4.Enabled = true;
                rbType2.Checked = true;
            }
            else if(dgvCSVList.Rows.Count > 1)
            {
                rbType1.Enabled = true;
                rbType2.Enabled = false;
                rbType3.Enabled = false;
                rbType4.Enabled = false;
                rbType1.Checked = true;
            }
            else
            {
                rbType1.Enabled = rbType1.Checked = false;
                rbType2.Enabled = rbType2.Checked = false;
                rbType3.Enabled = rbType3.Checked = false;
                rbType4.Enabled = rbType4.Checked = false;
            }
        }
        #endregion

        private void btnLoadParam_Click(object sender, EventArgs e)
        {
            _procItem.Load();

            displayDgvCompRange();
            displayJudgeRange();
            displayOtherParam();
            rdTypeCheckedChaged();
        }

        private void btnSaveParam_Click(object sender, EventArgs e)
        {
            updateDgvCompRange();
            updateJudgeRange();
            updateOtherParam();
            _procItem.Save();
        }
    }
}
