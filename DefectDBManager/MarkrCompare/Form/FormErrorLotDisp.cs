using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web.Configuration;
using System.Windows.Forms;
using System.Xml.Linq;

namespace MarkCompare
{
    public partial class FormErrorLotDisp : Form
    {
        #region Param
        private BindingList<string> _errLots = new BindingList<string>();
        DefectDBManager.Preproc.eProc _viewType;
        #endregion

        #region Form
        public FormErrorLotDisp(DefectDBManager.Preproc.eProc type)
        {
            InitializeComponent();

            lblTitle.MouseDown += lblTitle_MouseDown;
            lblTitle.MouseMove += lblTitle_MouseMove;

            // listBox에 바인딩
            lbLot.DataSource = _errLots;

            _viewType = type;

            UpdateLanguage();
        }

        private void FormErrorLotDisp_VisibleChanged(object sender, EventArgs e)
        {
        }
        #endregion

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

        #region Control
        private void btnOK_Click(object sender, EventArgs e)
        {
           
            this.DialogResult = DialogResult.OK;
            this.Close();
        }
        #endregion

        #region Lot Update
        public void OnUpdateErrorLots(string[] lots)
        {
            try
            {
                _errLots.RaiseListChangedEvents = false;// UI 갱신 중지
                foreach (var lot in lots)
                    _errLots.Add(lot);
            }
            catch
            {

            }
            finally
            {
                _errLots.RaiseListChangedEvents = true;// UI 갱신 중지
                _errLots.ResetBindings();
            }
        }
        #endregion

        #region 언어 변경
        public void UpdateLanguage()
        {
            CultureInfo culture = CultureInfo.CurrentCulture;

            string fontName = Functions.GetCultureFontName(culture.Name);
            Font newFont = new Font(fontName, 10, FontStyle.Bold);

            lblTitle.Font = newFont;
            btnOK.Font = newFont;

            if (_viewType == DefectDBManager.Preproc.eProc.Live)
                lblTitle.Text = Lang.formErrorLotDispTitleLive;
            else
                lblTitle.Text = Lang.formErrorLotDispTitleSearch;

            btnOK.Text = Lang.btnOK1;
        }
        #endregion

    }
}
