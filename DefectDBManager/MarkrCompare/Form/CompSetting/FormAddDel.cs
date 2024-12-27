using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MarkrCompare
{
    public partial class FormAddDel : Form
    {
        public string Title { get; set; }
        public string ScriptName { get; set; }
        public string DataName { get; set; }
        public FormAddDel(string title, string scriptName, string btnOkName, string btnCancelName, string dataName = "")
        {
            InitializeComponent();

            Title = title;
            ScriptName = scriptName;
            DataName = dataName;

            lblTitle.Text = Title;
            lblName.Text = ScriptName;
            tbName.Texts = dataName;
            btnOK.Text = btnOkName;
            btnCancel.Text = btnCancelName;

            lblTitle.MouseDown += lblTitle_MouseDown;
            lblTitle.MouseMove += lblTitle_MouseMove;
        }

        private void FormAdd_Load(object sender, EventArgs e)
        {

        }

        #region 마우스로 폼 드래그
        private Point mouseDnLoc;
        private void lblTitle_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                this.mouseDnLoc = e.Location;
            }
        }
        private void lblTitle_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.WindowState == FormWindowState.Maximized) return;

            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                this.Left = e.X + this.Left - this.mouseDnLoc.X;
                this.Top = e.Y + this.Top - this.mouseDnLoc.Y;
            }
        }
        #endregion

        private void btnCancel_Click(object sender, EventArgs e)
        {

            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            DataName = tbName.Texts;
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void FormAddDel_VisibleChanged(object sender, EventArgs e)
        {
            if(Visible==true)
            {
                lblTitle.Text = Title;
                lblName.Text = ScriptName;
                tbName.Texts = DataName;
            }
        }
    }
}
