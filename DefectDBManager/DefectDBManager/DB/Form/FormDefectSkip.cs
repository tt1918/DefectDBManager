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

        public FormDefectSkip(BindingList<DefectSizeTH> data)
        {
            InitializeComponent();

            panelTitle.MouseDown += lblTitle_MouseDown;
            panelTitle.MouseMove += lblTitle_MouseMove;

            dgvDefectSize.Columns.Clear();

            _defectSizeThs = data;
            dgvDefectSize.DataSource = _defectSizeThs;
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

        #region

        #endregion

        #region
        private void btnAdd_Click(object sender, EventArgs e)
        {
            DefectSizeTH data = new DefectSizeTH();
            _defectSizeThs.Add(data);
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {

        }

        private void btnApply_Click(object sender, EventArgs e)
        {

        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            Close();
        }
        #endregion

    }
}
