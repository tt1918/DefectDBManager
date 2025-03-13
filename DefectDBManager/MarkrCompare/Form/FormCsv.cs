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

namespace MarkrCompare
{
    public partial class FormCsv : Form
    {
        public List<string> Csv = new List<string>();
        const string CsvListPath = @"C:\COSS\Csv List\";
        public FormCsv()
        {
            InitializeComponent();
            InitGridView();
        }

        void InitGridView()
        {
            dataGridView1.ColumnHeadersVisible = true;
            dataGridView1.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;

            dataGridView1.RowHeadersVisible = true;

            dataGridView1.Columns.Add("Path", "Csv Path");
            dataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void btnOk_Click(object sender, EventArgs e)
        {
            if (dataGridView1.Rows.Count < 2 && MessageBox.Show("2개 이상의 Csv 부터 비교 가능합니다.") == DialogResult.OK)
            {
                return;
            }

            if (!Directory.Exists(CsvListPath))
                Directory.CreateDirectory(CsvListPath);

            foreach (DataGridViewRow row in dataGridView1.Rows)
            {
                Csv.Add(row.Cells[1].Value.ToString());
            }
            DialogResult = DialogResult.OK;
            this.Close();
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            if (dataGridView1.Rows.Count >= 100)
            {
                MessageBox.Show("Csv 개수가 100 개를 초과할 수 없습니다.");
                return;
            }
            dataGridView1.Rows.Add();
        }

        private void btnDel_Click(object sender, EventArgs e)
        {
            if (dataGridView1.Rows.Count == 0 || dataGridView1.SelectedRows.Count == 0) 
            {
                MessageBox.Show("선택된 행이 없습니다.");
                return; 
            }

            dataGridView1.CommitEdit(DataGridViewDataErrorContexts.Commit);

            foreach (DataGridViewRow item in dataGridView1.SelectedRows)
            {
                dataGridView1.Rows.Remove(item);
            }
        }

        private void dataGridView1_RowPostPaint(object sender, DataGridViewRowPostPaintEventArgs e)
        {
            // 현재 행의 인덱스를 가져옴 (1부터 시작하려면 +1)
            string rowIndex = (e.RowIndex + 1).ToString();

            // 인덱스를 표시할 위치 계산
            System.Drawing.Font rowFont = dataGridView1.Font;
            var textSize = e.Graphics.MeasureString(rowIndex, rowFont);
            var location = new System.Drawing.PointF(
                e.RowBounds.Left + 15,  // 왼쪽 여백 조정
                e.RowBounds.Top + (e.RowBounds.Height - textSize.Height) / 2 // 중앙 정렬
            );

            // 인덱스 그리기
            e.Graphics.DrawString(rowIndex, rowFont, System.Drawing.Brushes.Black, location);
        }
    }
}
