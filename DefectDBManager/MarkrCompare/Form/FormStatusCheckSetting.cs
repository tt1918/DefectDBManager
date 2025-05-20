using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Linq;

namespace MarkrCompare
{
    public partial class FormStatusCheckSetting : Form
    {
        #region Param
        public string CultureCode = "";
        public string IP { get; set; }
        public int Duration { get; set; }
        #endregion

        #region Form
        public FormStatusCheckSetting()
        {
            InitializeComponent();

            lblTitle.MouseDown += lblTitle_MouseDown;
            lblTitle.MouseMove += lblTitle_MouseMove;

            
        }

        public FormStatusCheckSetting(string ip, int duration)
        {
            InitializeComponent();

            lblTitle.MouseDown += lblTitle_MouseDown;
            lblTitle.MouseMove += lblTitle_MouseMove;

            UpdateLanguage();

            this.IP = ip;
            this.Duration = duration;
        }
        private void FormStatusCheckSetting_VisibleChanged(object sender, EventArgs e)
        {
            if (Visible == true)
            {
                tbIP.Texts = IP;
                tbDuration.Texts = Duration.ToString();
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

        #region Control
        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (updateIpData() == false)
                return;

            this.DialogResult = DialogResult.OK;
            this.Close();
        }


        private bool updateIpData()
        {
            string[] strings = tbIP.Texts.Split('.');
            int[] ips = new int[4];
            if (strings.Length != 4)
            {
                MessageBox.Show(Lang.IpAddressIsWrong);
                return false;
            }

            for (int i = 0; i < 4; i++)
            {
                ips[i] = Convert.ToInt32(strings[i]);
                if (ips[i] < 0 || ips[i] > 255)
                {
                    MessageBox.Show($"[{i + 1}, {ips[i]}] : {Lang.InsertCorrectNumber}");
                    return false;
                }
            }

            int duration;
            if(int.TryParse(tbDuration.Texts, out duration)==false)
            {
                MessageBox.Show(Lang.enterSearchInterval);
                return false;
            }

            IP = tbIP.Texts;
            Duration = duration;

            return true;
        }
        #endregion

        #region 언어 변경
        public void UpdateLanguage()
        {
            string fontName = Functions.GetCultureFontName(CultureCode);
            Font newFont = new Font(fontName, 10, FontStyle.Bold);

            lblTitle.Font = newFont;
            lblDuration.Font = newFont;
            lblIP.Font = newFont;
            btnCancel.Font = newFont;
            btnOK.Font = newFont;

            lblTitle.Text = Lang.formStatCheckSetTitle;
            lblDuration.Text = Lang.formStatCheckSetDuration;
            btnCancel.Text = Lang.btnCancel1;
            btnOK.Text = Lang.btnOK;
        }
        #endregion
    }
}
