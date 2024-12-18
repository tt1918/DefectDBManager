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
    delegate void DelegateShowLineData(string lncd, string lotNum, bool show);
    public partial class FormCrtLot : Form
    {
        #region Param
        private object _owner;

        private PreprocLot _crtLot = null;
        #endregion

        #region Event
        event DelegateShowLineData OnShowLineData;
        #endregion


        #region Create/Close
        public FormCrtLot()
        {
            InitializeComponent();
        }

        public FormCrtLot(object owner)
        {
            InitializeComponent();
            this._owner = owner;
        }

        private void FormCrtLot_Load(object sender, EventArgs e)
        {
            initDgvLotInfo();
        }
        private void FormCrtLot_FormClosing(object sender, FormClosingEventArgs e)
        {
        }
        #endregion

        #region Lot Data Grid View
        static string[] _strdgvListHeader = { "보기", "공정", "심볼", "LOT NUM", "시작 시간 ~ 종료 시간", "생산 M", "판정" };
        static int[] _dgvListLength = { 40, 60, 60, 150, 310, 90, 100 };
        enum _eDgvList{Use, LNCD, Symbol, LotNum, Time, Meter, Judge, Total };

        private void initDgvLotInfo()
        {
            // 자동 줄 추가 방지
            dgvLotInfo.AllowUserToAddRows = false;

            dgvLotInfo.Rows.Clear();
            dgvLotInfo.Columns.Clear();
            for (int i = 0; i < (int)_eDgvList.Total; i++)
            {
                switch((_eDgvList)i)
                {
                    case _eDgvList.Use:
                        {
                            DataGridViewCheckBoxColumn column = new DataGridViewCheckBoxColumn();
                            {
                                column.HeaderText = _strdgvListHeader[i];
                                column.Name = _strdgvListHeader[i];
                                column.FlatStyle = FlatStyle.Standard;
                                column.ThreeState = false;
                                column.CellTemplate = new DataGridViewCheckBoxCell();
                            }
                            dgvLotInfo.Columns.Add(column);
                        }
                        break;

                    default:
                        dgvLotInfo.Columns.Add(_strdgvListHeader[i], _strdgvListHeader[i]);
                        break;

                }

                dgvLotInfo.Columns[i].Width = _dgvListLength[i];
            }
        }

        private void setLotInfoToDgvLot(PreprocLot lot)
        {
            try
            {
                dgvLotInfo.SuspendLayout();
                dgvLotInfo.Rows.Clear();
                string time, stTime, edTime;
                foreach (var items in lot.PTRY0P_Data)
                {
                    foreach (var item in items.Data)
                    {
                        string[] strValue = new string[(int)_eDgvList.Total];
                        strValue[(int)_eDgvList.Use] = "true";
                        strValue[(int)_eDgvList.LNCD] = item.LNCD;
                        strValue[(int)_eDgvList.Symbol] = "";
                        strValue[(int)_eDgvList.LotNum] = item.Y0KLOT;
                        time = item.Y0KKOL;
                        stTime = $"{time.Substring(0, 4)}-{time.Substring(4, 2)}-{time.Substring(6, 2)} {time.Substring(8, 2)}:{time.Substring(10, 2)}:{time.Substring(12, 2)}";
                        time = item.Y0KSOL;
                        edTime = $"{time.Substring(0, 4)}-{time.Substring(4, 2)}-{time.Substring(6, 2)} {time.Substring(8, 2)}:{time.Substring(10, 2)}:{time.Substring(12, 2)}";
                        strValue[(int)_eDgvList.Time] = $"{stTime}~{edTime}";
                        strValue[(int)_eDgvList.Meter] = $"{item.Y0KASS}M";
                        strValue[(int)_eDgvList.Judge] = "";
                        dgvLotInfo.Rows.Add(strValue);
                    }
                }
            }
            catch
            {

            }
            finally
            {
                dgvLotInfo.ResumeLayout();
            }
        }

        private void setJudge(string lncd, bool judge, float score)
        {
            string data;
            string result;
            result = judge == true ? "OK" : "NG";
            result += $"({score:F1}%)";

            for(int i=0; i< dgvLotInfo.Rows.Count; i++)
            {
                data = dgvLotInfo.Rows[i].Cells[(int)_eDgvList.LNCD].Value as string;
                if (data == lncd)
                {
                    dgvLotInfo.Rows[i].Cells[(int)_eDgvList.Judge].Value = result;

                    if (judge == false) dgvLotInfo.Rows[i].DefaultCellStyle.BackColor = Color.Red;
                    else                dgvLotInfo.Rows[i].DefaultCellStyle.BackColor = Color.Tan;

                }
            }
        }

       private void dgvLotInfo_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            dgvLotInfo.CommitEdit(DataGridViewDataErrorContexts.Commit);
        }

        private void dgvLotInfo_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            int idx = e.RowIndex;
            if (e.ColumnIndex != (int)_eDgvList.Use) return;

            bool use = Convert.ToBoolean(dgvLotInfo.Rows[idx].Cells[(int)_eDgvList.Use].Value);
            string lncd = dgvLotInfo.Rows[idx].Cells[(int)_eDgvList.LNCD].Value as string;
            string lotNum = dgvLotInfo.Rows[idx].Cells[(int)_eDgvList.LotNum].Value as string;

            // 상위로 이벤트 전송
            OnShowLineData?.Invoke(lncd, lotNum, use);
        }
        #endregion

        #region Data Update Event
        public void OnUpdateLot(PreprocLot lot)
        {
            _crtLot = lot;

            if(InvokeRequired==true)
            {
                lblLotName.Text = $"LOT: {_crtLot.LotName}";
                setLotInfoToDgvLot(_crtLot);
            }
            else
            {
                lblLotName.Text = $"LOT: {_crtLot.LotName}";
                setLotInfoToDgvLot(_crtLot);
            }
        }

        public void OnUpdateJudge(string lncd, bool judge, float score)
        {
            if (InvokeRequired == true) setJudge(lncd, judge, score);
            else                        setJudge(lncd, judge, score);
        }

        #endregion

    }
}
