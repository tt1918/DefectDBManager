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
    public partial class FormDbAddition : Form
    {
        readonly string[] XOFSMSTHeader = { "KYCD", "PPCD", "LNCD", "YLMZKN2", "YLSZKN", "X_OFFSET" };
        readonly int[] listXOFSMSTWidth = { 50, 50, 50, 180, 180, 70 };

        readonly string[] AREADELHeader = { "No", "KYCD", "PPCD", "LNCD", "LOTNO", "STR_WD", "END_WD", "STR_MD", "END_MD" };
        readonly int[] listAREADELWidth = { 30, 50, 50, 50, 180, 100, 100, 100, 100 };

        public List<XOFSMSTData> _XOFSMSTData = null;
        public List<AREADELData> _AREADELData = null;

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
            listViewXOFSMST.Items.Clear();

            if (_XOFSMSTData == null) return;

            listViewXOFSMST.BeginUpdate();
            foreach (XOFSMSTData data in _XOFSMSTData)
            {
                ListViewItem item = new ListViewItem(data.KYCD);
                item.SubItems.Add(data.PPCD.ToString());
                item.SubItems.Add(data.LNCD);
                item.SubItems.Add(data.YLMZKN2);
                item.SubItems.Add(data.YLSZKN);
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
            listViewAREADEL.Items.Clear();

            if (_AREADELData == null) return;

            listViewAREADEL.BeginUpdate();
            int index = 0;
            foreach (AREADELData data in _AREADELData)
            {
                ListViewItem item = new ListViewItem(index.ToString());
                item.SubItems.Add(data.KYCD);
                item.SubItems.Add(data.PPCD.ToString());
                item.SubItems.Add(data.LNCD);
                item.SubItems.Add(data.LOTNO);
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
                updateXOFSMSTList();
                updateAREADELList();
            }
        }
    }
}
