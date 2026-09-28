using MarkCompare.Summery;
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
        DefectDBManager.Preproc.eProc _viewType;

        public bool IsHideMode { get; set; } = false;
        #endregion

        #region Form
        public FormErrorLotDisp(DefectDBManager.Preproc.eProc type)
        {
            InitializeComponent();

            lblTitle.MouseDown += lblTitle_MouseDown;
            lblTitle.MouseMove += lblTitle_MouseMove;

            initErrList();

            _viewType = type;
        }

        private void FormErrorLotDisp_VisibleChanged(object sender, EventArgs e)
        {
            if(this.Visible)
            {
                UpdateLanguage();
            }
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

        #region Display Error List
        private void initErrList()
        {
            rtbErrList.ReadOnly = true;
            rtbErrList.BorderStyle = BorderStyle.None;
            rtbErrList.DetectUrls = false;
            rtbErrList.HideSelection = true;
            rtbErrList.TabStop = false;
        }

        private void displayErrList(SummeryInfoList infos)
        {
            if (infos == null) return;

            try
            {
                rtbErrList.SuspendLayout();

                foreach (var info in infos.Infos)
                {
                    Color color = Color.Black;

                    if (info.IsMarkingError && !info.IsAiProcError)
                        color = Color.Black;
                    else if (!info.IsMarkingError && info.IsAiProcError)
                        color = Color.Blue;
                    else if (info.IsMarkingError && info.IsAiProcError)
                        color = Color.Red;
                    else
                        color = Color.Black;

                    rtbErrList.SelectionStart = rtbErrList.TextLength;
                    rtbErrList.SelectionLength = 0;

                    rtbErrList.SelectionColor = color;
                    rtbErrList.AppendText(info.Info + Environment.NewLine);
                }

                // 커서/선택 제거
                rtbErrList.SelectionStart = 0;
                rtbErrList.SelectionLength = 0;
            }
            finally
            {
                rtbErrList.ResumeLayout();
            }
        }
        #endregion

        #region Control
        private void btnOK_Click(object sender, EventArgs e)
        {
            if(IsHideMode)
            {
                this.Hide();
            }
            else
            {
                this.DialogResult = DialogResult.OK;
                this.Close();
            }
        }
        #endregion

        #region Lot Update
        public void OnUpdateErrorLots(SummeryInfoList infos)
        {
            try
            {
                displayErrList(infos);
            }
            catch
            {

            }
            finally
            {
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
            {
                if(!IsHideMode)
                    lblTitle.Text = Lang.formErrorLotDispTitleLive;
                else
                    lblTitle.Text = Lang.SJMODE_Mornitor_Error;
            }
                
            else
                lblTitle.Text = Lang.formErrorLotDispTitleSearch;

            btnOK.Text = Lang.btnOK1;
        }
        #endregion

    }
}
