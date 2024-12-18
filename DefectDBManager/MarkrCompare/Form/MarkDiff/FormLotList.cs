using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MarkrCompare
{
    public partial class FormLotList : Form
    {
        object _owner = null;

        #region Create/Destroy
        public FormLotList()
        {
            InitializeComponent();
        }

        public FormLotList(object owner, string name)
        {
            InitializeComponent();
            _owner = owner;
            Name = name;
        }
        private void FormLotList_Load(object sender, EventArgs e)
        {
            initDgvLot();
        }

        private void FormLotList_FormClosing(object sender, FormClosingEventArgs e)
        {

        }
        #endregion

        #region Lot Data Grid View
        static string[] _strdgvListHeader = { "LOT NUM", "시작 시간 ~ 종료 시간", "생산 M", "판정" };
        static int[] _dgvListLength = { 200, 350, 100, 100 };

        private void initDgvLot()
        {
            dgvList.ColumnCount = 4;
            for(int i=0; i< dgvList.ColumnCount; i++)
            {
                dgvList.Columns[i].Name = _strdgvListHeader[i];
                dgvList.Columns[i].Width = _dgvListLength[i];
            }
        }



        #endregion

        
    }
}
