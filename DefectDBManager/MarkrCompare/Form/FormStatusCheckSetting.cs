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
                MessageBox.Show("IP 주소 입력이 잘못 되었습니다.");
                return false;
            }

            for (int i = 0; i < 4; i++)
            {
                ips[i] = Convert.ToInt32(strings[i]);
                if (ips[i] < 0 || ips[i] > 255)
                {
                    MessageBox.Show($"{i + 1} 번째 주소가 0~255 사의 값이 아닙니다.");
                    return false;
                }
            }

            int duration;
            if(int.TryParse(tbDuration.Texts, out duration)==false)
            {
                MessageBox.Show($"검색 주기에 대한 상수를 입력해주십시요.");
                return false;
            }

            IP = tbIP.Texts;
            Duration = duration;

            return true;
        }
        #endregion
    }
}
