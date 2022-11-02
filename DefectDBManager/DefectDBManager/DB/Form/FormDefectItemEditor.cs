using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DefectDBManager
{
    public partial class FormDefectItemEditor : Form
    {
        public string EditData {get; set;}

        public FormDefectItemEditor()
        {
            InitializeComponent();

            panelTitle.MouseDown += lblTitle_MouseDown;
            panelTitle.MouseMove += lblTitle_MouseMove;
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

        private void btnOK_Click(object sender, EventArgs e)
        {
            EditData = tbEditData.Text;

            DialogResult = DialogResult.OK;
            this.Close();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void FormDefectItemEditor_VisibleChanged(object sender, EventArgs e)
        {
            if(Visible==true)
            {
                tbEditData.Text = EditData;
            }
        }
    }
}
