using DefectDBManager;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.TaskbarClock;

namespace MarkrCompare
{
    public partial class FormLotList : Form
    {
        #region Param
        object _owner = null;
        
        private List<PreprocLot> _prepLotList = null;

        #endregion

        #region Event
        public event MarkrCompare.Delegate.UpdatePrepLot OnUpdatePrepLot;
        #endregion

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

            _prepLotList = new List<PreprocLot>();
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
        static int[] _dgvListLength = { 150, 280, 100, 100 };
        enum eDgvLot{LotNum, Time, Meter, Judge, Total};

        private void initDgvLot()
        {
            dgvList.AllowUserToAddRows = false;
            dgvList.ColumnCount = (int)eDgvLot.Total;
            dgvList.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;

            for (int i=0; i< dgvList.ColumnCount; i++)
            {
                dgvList.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
                dgvList.Columns[i].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dgvList.Columns[i].Name = _strdgvListHeader[i];
                dgvList.Columns[i].Width = _dgvListLength[i];
            }
        }

        private void dgvList_CellDoubleClick(object sender, DataGridViewCellEventArgs e)
        {
            try
            {
                int selRow = e.RowIndex;
                string selLotName = dgvList.Rows[selRow].Cells[0].Value as string;

                if (_prepLotList == null) return;

                foreach (var prepLot in _prepLotList)
                {
                    if (prepLot.LotName == selLotName)
                    {
                        //OnUpdatePrepLot?.BeginInvoke(prepLot, null, this);
                        OnUpdatePrepLot?.Invoke(prepLot);
                        break;
                    }
                }
            }
            catch
            {

            }
        }

        private void clearDgvLot()
        {
            dgvList.Rows.Clear();
        }

        private void addDateToDgvLot(PreprocLot lot)
        {
            string time = "", stTime="", edTime="";
            string[] data = new string[4];

            data[(int)eDgvLot.LotNum] = lot.LotName;

            foreach (var items in lot.PTRY0P_Data)
            {
                foreach (var item in items.Data)
                {
                    if(item.Y0KLOT== lot.LotName)
                    {
                        time = item.Y0KKOL;
                        stTime = $"{time.Substring(0, 4)}-{time.Substring(4, 2)}-{time.Substring(6, 2)} {time.Substring(8, 2)}:{time.Substring(10, 2)}:{time.Substring(12, 2)}";
                        time = item.Y0KSOL;
                        edTime = $"{time.Substring(0, 4)}-{time.Substring(4, 2)}-{time.Substring(6, 2)} {time.Substring(8, 2)}:{time.Substring(10, 2)}:{time.Substring(12, 2)}";
                        data[(int)eDgvLot.Time] = $"{stTime}~{edTime}";
                        data[(int)eDgvLot.Meter] = $"{item.Y0KASS}M";
                        break;
                    }
                }
            }
            data[(int)eDgvLot.Judge] = "";

            dgvList.Rows.Add(data);
        }
        #endregion

        #region Data Control
        public void UpdateLotInfo(PreprocLot[] lots)
        {
            _prepLotList?.Clear();
            if (_prepLotList == null) _prepLotList = new List<PreprocLot>();
            
            clearDgvLot();

            foreach(PreprocLot lot in lots)
            {
                addDateToDgvLot(lot);
                _prepLotList.Add(lot);
            }
        }
        #endregion
    }
}
