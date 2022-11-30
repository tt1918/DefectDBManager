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
    public partial class FormDbLoginData : Form
    {
        public OracleDbConnection Conn { get; set; }

        public FormDbLoginData(OracleDbConnection conn)
        {
            InitializeComponent();

            Conn = conn;
        }

        private void FormDbLoginData_Load(object sender, EventArgs e)
        {
            
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            if (rbConStringType1.Checked == true)
                Conn.ConStringType = 0;
            else if (rbConStringType2.Checked == true)
                Conn.ConStringType = 1;
            Conn.DBName = tbDbName.Text;
            Conn.UserID = tbUserID.Text;
            Conn.Password = tbPassword.Text;
            Conn.HostIP = tbHostIP.Text;
            Conn.DBPort = tbPort.Text;
            Conn.Connect();
        }

        private void btnDisconnect_Click(object sender, EventArgs e)
        {
            Conn.Disconnect();
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void FormDbLoginData_VisibleChanged(object sender, EventArgs e)
        {
            if(this.Visible==true)
            {
                UpdateLanguage();
                displayCtrl();
            }
        }

        private void displayCtrl()
        {
            if(Conn.ConStringType==0)
            {
                rbConStringType1.Checked = true;
                rbConStringType2.Checked = false;
                tbHostIP.Enabled = false;
                tbPort.Enabled = false;
            }
            else if(Conn.ConStringType == 1)
            {
                rbConStringType1.Checked = false;
                rbConStringType2.Checked = true;
                tbHostIP.Enabled = true;
                tbPort.Enabled = true;
            }
            tbHostIP.Text = Conn.HostIP;
            tbPort.Text = Conn.DBPort;
            tbDbName.Text = Conn.DBName;
            tbUserID.Text = Conn.UserID;
            tbPassword.Text = Conn.Password;
        }

        private void enableCtrl()
        {
            if (rbConStringType1.Checked == true)
            {
                tbHostIP.Enabled = false;
                tbPort.Enabled = false;
            }
            else if (rbConStringType2.Checked == true)
            {
                tbHostIP.Enabled = true;
                tbPort.Enabled = true;
            }
        }

        private void rbConStringType1_CheckedChanged(object sender, EventArgs e)
        {
            enableCtrl();
        }

        private void rbConStringType2_CheckedChanged(object sender, EventArgs e)
        {
            enableCtrl();
        }

        #region Language Update
        public void UpdateLanguage()
        {
            this.SuspendLayout();
            lblTitle.Text = Language.DBLoginTitle;
            lblType.Text = Language.DBLoginType;
            lblHostIP.Text = Language.DBLoginHostIP;
            lblPort.Text = Language.DBLoginPort;
            lblDomain.Text = Language.DBLoginDomain;
            lblUserID.Text = Language.DBLoginID;
            lblPassword.Text = Language.DBLoginPW;
            btnConnect.Text = Language.DBLoginConnect;
            btnDisconnect.Text = Language.DBLoginDisonnect;
            this.ResumeLayout();
        }
        #endregion Language Update
    }
}
