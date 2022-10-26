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
            tbDbName.Text = Conn.DBName;
            tbUserID.Text = Conn.UserID;
            tbPassword.Text = Conn.Password;
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            Conn.DBName = tbDbName.Text;
            Conn.UserID = tbUserID.Text;
            Conn.Password = tbPassword.Text;

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
    }
}
