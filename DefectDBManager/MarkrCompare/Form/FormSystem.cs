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

namespace MarkCompare
{
    public partial class FormSystem : Form
    {

        #region Param
        public string CultureCode = "";
        public SystemParam SysParam
        {
            get { return _systemParam; }
            private set { _systemParam = value; }
        }
        SystemParam _systemParam = new SystemParam();
        #endregion

        #region Form Control
        public FormSystem(SystemParam param)
        {
            InitializeComponent();

            lblTitle.MouseDown += lblTitle_MouseDown;
            lblTitle.MouseMove += lblTitle_MouseMove;

            _systemParam = param.Clone();

            UpdateLanguage(CultureCode);
        }

        private void FormSystem_Load(object sender, EventArgs e)
        {
            if (this.Visible == true)
            {
                this.ppgSystem.SelectedObject = _systemParam;
            }
        }

        private void FormSystem_FormClosing(object sender, FormClosingEventArgs e)
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

        #region Control
        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            Close();
        }
        private void btnOK_Click(object sender, EventArgs e)
        {
            _systemParam.Save();
            DialogResult = DialogResult.OK;
            Close();
        }
        #endregion

        #region 언어 변경
        public void UpdateLanguage(string culture)
        {
            string fontName = Functions.GetCultureFontName(culture);
            Font newFont = new Font(fontName, 10, FontStyle.Bold);
            btnCancel.Font = newFont;
            btnOK.Font = newFont;
            lblTitle.Font = newFont;

            btnCancel.Text = Lang.btnCancel;
            btnOK.Text = Lang.btnOK;
            lblTitle.Text = Lang.formSystemTitle;
        }
        #endregion
    }
}
