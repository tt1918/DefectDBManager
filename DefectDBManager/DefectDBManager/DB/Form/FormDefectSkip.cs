using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DefectDBManager
{
    public partial class FormDefectSkip : Form
    {
        public BindingList<DefectSizeTH> DefectSizeTHs
        {
            get { return _defectSizeThs; }
            set 
            { 
                _defectSizeThs = value;
                dgvDefectSize.DataSource = _defectSizeThs;
            }
        }

        private BindingList<DefectSizeTH> _defectSizeThs;

        public bool IsApply = false;

        public FormDefectSkip(BindingList<DefectSizeTH> data)
        {
            InitializeComponent();

            panelTitle.MouseDown += lblTitle_MouseDown;
            panelTitle.MouseMove += lblTitle_MouseMove;

            dgvDefectSize.Columns.Clear();

            _defectSizeThs = data;
            dgvDefectSize.DataSource = _defectSizeThs;

            UpdateLanguage();
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

        #region 언어 설정
        public void UpdateLanguage()
        {
            this.SuspendLayout();
           
            lblTitle.Text = Language.DefectSkipTitle;
            btnAdd.Text = Language.Add;
            btnDelete.Text = Language.Del;
            btnApply.Text = Language.Apply;
            btnClose.Text = Language.Close;

            if (dgvDefectSize.Columns.Count <= 4)
            {
                dgvDefectSize.Columns[0].HeaderText = Language.DefectSkip_Name;
                dgvDefectSize.Columns[1].HeaderText = Language.DefectSkip_LNCD;
                dgvDefectSize.Columns[2].HeaderText = Language.DefectSkip_Min;
                dgvDefectSize.Columns[3].HeaderText = Language.DefectSkip_Max;
            }

            this.ResumeLayout();

        }

        #endregion

        #region Contorl Button
        private void btnAdd_Click(object sender, EventArgs e)
        {
            DefectSizeTH data = new DefectSizeTH();
            _defectSizeThs.Add(data);
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            if (dgvDefectSize.SelectedCells.Count <= 0)
            {
                MessageBox.Show("선택된 열이 없습니다.");
                return;
            }
            int selected = dgvDefectSize.SelectedCells[0].RowIndex;

            string text = $"Index {selected}: {_defectSizeThs[selected].ToString()}를 삭제하시겠습니까?";
            if(MessageBox.Show(text, "DATA DELETE", MessageBoxButtons.YesNo)==DialogResult.Yes)
            {
                _defectSizeThs.RemoveAt(selected);
            }
        }

        private void btnApply_Click(object sender, EventArgs e)
        {
            IsApply = true;
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            Close();
        }
        #endregion

    }
}
