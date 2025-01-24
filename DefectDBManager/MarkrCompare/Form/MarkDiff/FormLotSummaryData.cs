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
    public enum eSummaryMode { Mornitoring, LiveErrorCheck}
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
            displayCompareResult();
            displayLotName();
            displayDetail();
        }

        private void displayCompareResult()
        {
            
        }

        private void displayLotName()
        {
            string str = _lotSummery.LotName;
            lblLotName.Text = str;
        }
        
        private void displayDetail()
        {
            string str = null;
            int idx = 1;
            int resultIdx = 0;
            foreach (var item in _lotSummery.MarkCompList.Data)
            {
                if (idx == 1)
                {
                    str += $"{idx++} : {item.Base}%";
                }
                else
                {
                    foreach (var item2 in item.Comp)
                    {
                        str += $"{idx++} : {item2[resultIdx++]}";
                    }
                }
            }

            lblProcess.Text = str;
        }

        /// <summary>
        /// 지정 경로 Status.txt 파일의 상태 표시 
        /// </summary>
        public void  SetStatusAlarm()
        {

        }
        #endregion

        #region Status.txt 상태 표시
        Timer   _timerStatus = null;
        string  _targetIP = string.Empty;
        int     _duration = 60 * 1000;
        string  _lineName = string.Empty;

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
            if(_timerStatus!=null)
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

        
    }
}
