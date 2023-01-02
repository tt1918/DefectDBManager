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

        readonly string[] AREADELHeader = { "No", "KYCD", "PPCD", "LNCD", "LOTNO", "STR_WD", "END_WD", "STR_MD", "END_MD" };
        readonly int[] listAREADELWidth = { 30, 50, 50, 50, 100, 80, 80, 80, 80 };

        public NittoDB _DBData = null;

        public FormDbAddition()
        {
            InitializeComponent();
        }

        private void FormDbAddition_Load(object sender, EventArgs e)
        {
            initXOFSMSTList();
            initAREADELList();
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
            if (_DBData._DbResult.XOFSMST_Data == null) return;

            listViewXOFSMST.BeginUpdate();
            listViewXOFSMST.Items.Clear();
            foreach (XOFSMSTData data in _DBData._DbResult.XOFSMST_Data)
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
            if (_DBData._DbResult.AREADEL_Data == null) return;

            listViewAREADEL.BeginUpdate();
            listViewAREADEL.Items.Clear();
            int index = 0;
            foreach (AREADELData data in _DBData._DbResult.AREADEL_Data)
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
        }
        #endregion Language Update

        private void btnLoadtAreaDel_Click(object sender, EventArgs e)
        {
            using (OpenFileDialog browser = new OpenFileDialog())
            {
                //browser.InitialDirectory = Define.MainPath;
                browser.Filter = "CSV Files (*.csv)|*.csv|모든 파일 (*.*)|*.*";
                browser.FilterIndex = 1;
                browser.RestoreDirectory = true;

                if (browser.ShowDialog() == DialogResult.OK)
                {
                    if (System.IO.File.Exists(browser.FileName) == false)
                    {
                        MessageBox.Show($"파일이 존재하지 않습니다. : [{browser.SafeFileName}]");
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
                browser.Filter = "CSV Files (*.csv)|*.csv|모든 파일 (*.*)|*.*";
                browser.FilterIndex = 1;
                browser.RestoreDirectory = true;

                if (browser.ShowDialog() == DialogResult.OK)
                {
                    string path = browser.FileName;
                    AreaDelCSV.Save(path, _DBData);
                }
            }
        }
    }
}
