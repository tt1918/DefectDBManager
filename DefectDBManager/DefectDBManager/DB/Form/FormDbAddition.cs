using DefectDBManager.DB;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.Entity;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DefectDBManager
{
    public partial class FormDbAddition : Form
    {
        readonly string[] XOFSMSTHeader = { "KYCD", "PPCD", "LNCD", "YLMZKN2", "YLSZKN", "X_OFFSET" };
        readonly int[] listXOFSMSTWidth = { 50, 50, 50, 180, 180, 70 };

        readonly string[] AREADELHeader = { "No", "KYCD", "PPCD", "LNCD", "LOTNO", "STR_WD", "END_WD", "STR_MD", "END_MD", "BCNO" };
        readonly int[] listAREADELWidth = { 30, 50, 50, 50, 100, 80, 80, 80, 80, 110 };

        public NittoDB _DBData = null;

        public FormDbAddition()
        {
            InitializeComponent();
        }

        private void FormDbAddition_Load(object sender, EventArgs e)
        {
            initXOFSMSTList();
            initAREADELList();

            panelTitle.MouseDown += lblTitle_MouseDown;
            panelTitle.MouseMove += lblTitle_MouseMove;
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        private void initXOFSMSTList()
        {
            listViewXOFSMST.View = View.Details;
            listViewXOFSMST.FullRowSelect = true;

            int cnt = System.Enum.GetValues(typeof(eXOFSMSTList)).Length;
            for (int i = 0; i < cnt; i++)
                listViewXOFSMST.Columns.Add(XOFSMSTHeader[i], listXOFSMSTWidth[i]);
        }

        private void updateXOFSMSTList()
        {
            if(_DBData==null)
            {
                MessageBox.Show("현재 모드에서는 지원하지 않습니다.");
                return;
            }

            if (_DBData._DbResult.XOFSMST == null) return;

            listViewXOFSMST.BeginUpdate();
            listViewXOFSMST.Items.Clear();
            foreach (XOFSMSTData data in _DBData._DbResult.XOFSMST.Data)
            {
                ListViewItem item;
                if (data.KYCD != null) item = new ListViewItem(data.KYCD);
                else item = new ListViewItem("");
                item.SubItems.Add(data.PPCD.ToString());
                if (data.LNCD != null) item.SubItems.Add(data.LNCD);
                else item.SubItems.Add("");
                if (data.YLMZKN2 != null) item.SubItems.Add(data.YLMZKN2);
                else item.SubItems.Add("");
                if (data.YLSZKN != null) item.SubItems.Add(data.YLSZKN);
                else item.SubItems.Add("");
                item.SubItems.Add(data.X_OFFSET.ToString());
                listViewXOFSMST.Items.Add(item);
            }
            listViewXOFSMST.EndUpdate();
        }

        private void initAREADELList()
        {
            listViewAREADEL.View = View.Details;
            listViewAREADEL.FullRowSelect = true;

            int cnt = System.Enum.GetValues(typeof(eAREADELList)).Length;
            for (int i = 0; i < cnt; i++)
                listViewAREADEL.Columns.Add(AREADELHeader[i], listAREADELWidth[i]);
        }

        private void updateAREADELList()
        {
            if (_DBData == null) return;
            if (_DBData._DbResult.AREADEL == null) return;

            listViewAREADEL.BeginUpdate();
            listViewAREADEL.Items.Clear();
            int index = 0;
            foreach (AREADELData data in _DBData._DbResult.AREADEL.Data)
            {
                ListViewItem item = new ListViewItem(index.ToString());
                if (data.KYCD != null) item.SubItems.Add(data.KYCD);
                else item.SubItems.Add("");
                if(data.PPCD!=null) item.SubItems.Add(data.PPCD.ToString());
                else item.SubItems.Add("");
                if (data.LNCD != null) item.SubItems.Add(data.LNCD);
                else item.SubItems.Add("");
                if (data.LOTNO != null) item.SubItems.Add(data.LOTNO);
                else item.SubItems.Add("");
                item.SubItems.Add(data.STR_WD.ToString());
                item.SubItems.Add(data.END_WD.ToString());
                item.SubItems.Add(data.STR_MD.ToString());
                item.SubItems.Add(data.END_MD.ToString());
                item.SubItems.Add(data.BCNO);
                listViewAREADEL.Items.Add(item);
                index++;
            }
            listViewAREADEL.EndUpdate();
        }

        private void FormDbAddition_VisibleChanged(object sender, EventArgs e)
        {
            if(this.Visible==true)
            {
                UpdateLanguage();
                updateXOFSMSTList();
                updateAREADELList();
            }
        }

        #region Language Update
        public void UpdateLanguage()
        {
            btnClose.Text = Language.Close;
            btnLoadtAreaDel.Text = Language.btnLoadtAreaDel;
            btnSaveAreaDel.Text = Language.btnSaveAreaDel;
        }
        #endregion Language Update

        private void btnLoadtAreaDel_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog browser = new OpenFileDialog())
            {
                //browser.InitialDirectory = Define.MainPath;
                browser.Filter = "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*";
                browser.FilterIndex = 1;
                browser.RestoreDirectory = true;

                if (browser.ShowDialog() == DialogResult.OK)
                {
                    if (System.IO.File.Exists(browser.FileName) == false)
                    {
                        MessageBox.Show($"{Language.FileDoesNotExist} : [{browser.SafeFileName}]");
                        return;
                    }

                    if(AreaDelCSV.Load(browser.FileName, _DBData)==true)
                        updateAREADELList();
                }
            }
        }

        private void btnSaveAreaDel_Click(object sender, EventArgs e)
        {
            using (SaveFileDialog browser = new SaveFileDialog())
            {
                browser.InitialDirectory = Define.MainPath;
                browser.Filter = "CSV Files (*.csv)|*.csv|All Files (*.*)|*.*";
                browser.FilterIndex = 1;
                browser.RestoreDirectory = true;

                if (browser.ShowDialog() == DialogResult.OK)
                {
                    string path = browser.FileName;
                    AreaDelCSV.Save(path, _DBData);
                }
            }
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
    }
}
