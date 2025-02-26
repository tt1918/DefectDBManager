using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MarkrCompare
{
    public enum eSummaryMode { Mornitoring, LiveErrorCheck }
    public partial class FormLotSummaryData : Form
    {
        public DefectDBManager.PreprocLot LotSummery
        {
            get { return _lotSummery; }
            set
            {
                _lotSummery = value;
                displayLotSummery();
            }
        }
        /// <summary>
        /// 상위 검사 랏 서머리 정보 
        /// </summary>
        private DefectDBManager.PreprocLot _lotSummery = new DefectDBManager.PreprocLot();
        private eSummaryMode _mode = eSummaryMode.Mornitoring;

        public DefectDBManager.Preproc.PreprocItem ProcItem
        {
            get { return procItem; }
            set
            {
                procItem = value;
            }
        }
        //private DefectDBManager.PreprocLotManager _lotManager = new DefectDBManager.PreprocLotManager();
        private DefectDBManager.Preproc.PreprocItem procItem = new DefectDBManager.Preproc.PreprocItem();

        bool isError = false;
        public bool IsError 
        {
            get { return isError; }
            set { isError = value; }
        }

        #region Form
        public FormLotSummaryData()
        {
            InitializeComponent();
        }

        public FormLotSummaryData(DefectDBManager.PreprocLot lotSummery)
        {
            InitializeComponent();

            _lotSummery = lotSummery;
        }

        private void FormLotSummaryData_Load(object sender, EventArgs e)
        {

        }

        private void FormLotSummaryData_VisibleChanged(object sender, EventArgs e)
        {

        }
        private void FormLotSummaryData_FormClosing(object sender, FormClosingEventArgs e)
        {
            closeStatusTimer();
        }
        private void FormLotSummaryData_DoubleClick(object sender, EventArgs e)
        {
            ((FormLotList)this.ParentForm).DoubleClickSummaryData(this);
        }
        #endregion

        #region 데이터 표시
        private void displayLotSummery()
        {
            displayDetail();
            displayCompareResult();
            displayLotName();
        }

        private void displayCompareResult()
        {
            if (_lotSummery.MarkCompList == null || _lotSummery.MarkCompList.Data.Count <= 0)
            {
                lblStatus.Text = "Error";
                return;
            }

            if (isError)
            {
                lblStatus.BkColor = Color.Red;
                lblStatus.Text = "오차 발생";
            }
            else
            {
                lblStatus.BkColor = Color.MidnightBlue;
                lblStatus.Text = "정상";
            }
        }

        private void displayLotName()
        {
            string str = _lotSummery.LotName;
            lblLotName.Text = str;

            if (isError)
                lblLotName.BkColor = Color.Red;
            else
                lblLotName.BkColor = Color.MidnightBlue;
        }

        private void displayDetail()
        {
            if (_lotSummery.MarkCompList == null || _lotSummery.MarkCompList.Data.Count <= 0)
            {
                lblProcess.Text = "비교 데이터 없음";
                return;
            }

            string str = null;
            int[] compCnt = new int[_lotSummery.MarkCompList.Data[0].Comp.Length];

            foreach (var item in _lotSummery.MarkCompList.Data)
            {
                //baseCnt = 0;
                for (int i = 0; i < item.Comp.GetLength(0); i++)
                {
                    if (item.Comp[i, 0].Count > 0)
                        compCnt[0]++;

                    for (int j = 1; j < item.Comp.GetLength(1); j++)
                    {
                        if (item.Comp[i, j].Count > 0)
                            compCnt[j]++;
                    }
                }
            }

            double[] result = new double[compCnt.Length];
            for (int i = 0; i < compCnt.Length; i++)
            {
                result[i] = (double)compCnt[i] / _lotSummery.MarkCompList.Data.Count * 100;
                if (i == compCnt.Length - 1)
                {
                    str += $"Case {i + 1} : {result[i]:F2}%";
                }
                else
                {
                    str += $"Case {i + 1} : {result[i]:F2}%, ";
                }

                if (i > 0 && result[i - 1] - result[i] > procItem.CompRange[i - 1].Accuracy)
                {
                    isError = true;
                }
            }

            lblProcess.Text = str;
        }

        /// <summary>
        /// 지정 경로 Status.txt 파일의 상태 표시 
        /// </summary>
        public void SetStatusAlarm()
        {

        }
        #endregion

        #region Status.txt 상태 표시
        Timer _timerStatus = null;
        string _targetIP = string.Empty;
        int _duration = 60 * 1000;
        string _lineName = string.Empty;

        public void SetStatusCheck(string lineName, string ip, int min)
        {
            _mode = eSummaryMode.LiveErrorCheck;
            _targetIP = ip;
            _duration = 60 * min * 1000;
            _lineName = lineName;
            initStatusTimer();
            displayLineName();
        }

        private void initStatusTimer()
        {
            closeStatusTimer();

            _timerStatus = new System.Windows.Forms.Timer();
            _timerStatus.Interval = _duration;
            _timerStatus.Tick += new EventHandler(timerDisplayStatus);

            _timerStatus.Start();
        }

        private void closeStatusTimer()
        {
            if (_timerStatus != null)
            {
                _timerStatus.Stop();
                _timerStatus.Dispose();
                _timerStatus = null;
            }
        }

        private void timerDisplayStatus(object sender, EventArgs e)
        {
            string path = Path.Combine($"\\\\{_targetIP}", "COSS\\status.txt");
            if (File.Exists(path) == false)
            {
                // 세부 사항 업데이트
            }
        }
        private void displayLineName()
        {
            lblStatus.Text = _lineName;
        }

        #endregion

        private void tableLayoutPanel2_DoubleClick(object sender, EventArgs e)
        {
            ((FormLotList)this.ParentForm).DoubleClickSummaryData(this);
        }
    }
}
