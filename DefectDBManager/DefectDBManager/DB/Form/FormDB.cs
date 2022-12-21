using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.Entity;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;
using System.Windows.Forms;
using System.Windows.Forms.PropertyGridInternal;
using System.Globalization;
using static DefectDBManager.QueryMsg;

namespace DefectDBManager
{

    public delegate void DelegateEndCsvReading(int eventID);

    public partial class FormDB : Form
    {
        #region Form 종료 못하게 막기
        private const int CP_NOCLOSE_BUTTON = 0x200;
        protected override CreateParams CreateParams
        {
            get
            {
                CreateParams cp = base.CreateParams;
                cp.ClassStyle = cp.ClassStyle | CP_NOCLOSE_BUTTON;
                return cp;
            }
        }

        public bool _FormCloseBtnType = false;
        #endregion
        #region const param
        readonly string[] BCnoHeader = { "No.", "LOTNO", "Bad Count", "M^2 불량수", "Use", "원단 BCNO" };
        readonly int[] listBCnoWidth = { 40, 100, 80, 80, 50, 80 };
        readonly string[] PTRYLPHeader = { "원단 Lot", "점착 Lot", "Insert M", "Complete M", "Width", "품종" };
        readonly int[] listPTRYLPWidth = { 100, 300, 100, 100, 100, 100 };
        readonly string[] PTRYOPHeader = { "품종", "연신 Lot", "라인코드", "개시 시간", "종료 시간" };
        readonly int[] listPTRYOPWidth = { 70, 70, 60, 140, 140 };
        readonly string[] MRKCTLMSTHeader = { "라인코드", "불량 ID", "품종", "사이즈" };
        readonly int[] listMRKCTLMSTWidth = { 70, 50, 180, 80 };
        readonly string[] INSPDATHeader = { "관리NO", "품종", "LOTNO", "STRDT", "STRTM", "ENDDT", "ENDTM", "WIDTH", "LENGTH" };
        readonly int[] listINSPDATWidth = { 130, 160, 90, 60, 70, 80, 80, 80, 90 };
        readonly string[] FAULTDATHeader = { "CNT", "관리 NO", "불량 CNT", "OFFSET", "XPOS_M", "YPOS_M", "종류", "불량번호",
                                             "SIZE", "CAM", "CLASS", "M CLASS", "X_OFFSET", "MNTTID"};
        readonly int[] listFAULTDATWidth = { 45, 130, 80, 90, 110, 110, 40, 75, 55, 50, 60, 90, 90, 70 };

        private const int maxFaultDat = 100;
        #endregion

        #region param
        private int crtFaultDatPage = 0;
        private int totalFaultDatPage = 0;

        object parent = null;

        public bool RedrawAll = false;

        private System.Windows.Forms.Timer dbCommTimer = null;
        private System.Windows.Forms.Timer dbSearchProgressTimer = null;
        private Stopwatch dbLoadingTime = null;

        public NittoDB DataBase
        {
            get { return dataBase; }
            set
            {
                dataBase = value;
                displayUI();
            }
        }
        private NittoDB dataBase = null;

        public OracleDbConnection DBConn
        {
            get { return dbConn; }
            set { dbConn = value; }
        }
        private OracleDbConnection dbConn = null;

        #endregion

        #region Form
        FormDbAddition formDbAddition = null;
        FormDbLoginData formLogin = null;
        FormDbProgress formProgress = null;
        #endregion

        #region Language
        public int _LangType
        {
            get
            {
                return langType;
            }
            set
            {
                langType = value;
                setLangType();
            }
        } // 0: 한국어 1: 영어 2: 중국어
        private int langType;
        #endregion

        #region ListViewData
        public ListViewData BCNO_LV_Data;
        public ListViewData PTRYLP_LV_Data;
        public ListViewData MRKCTLMST_LV_Data;
        public ListViewData PTRYOP_LV_Data;
        public ListViewData INSPDAT_LV_Data;

        public void CreateListViewData()
        {
            BCNO_LV_Data = new ListViewData();
            PTRYLP_LV_Data = new ListViewData();
            MRKCTLMST_LV_Data = new ListViewData();
            PTRYOP_LV_Data = new ListViewData();
            INSPDAT_LV_Data = new ListViewData();
        }

        public void ResetListViewData()
        {
            BCNO_LV_Data.Reset();
            PTRYLP_LV_Data.Reset();
            MRKCTLMST_LV_Data.Reset();
            PTRYOP_LV_Data.Reset();
            INSPDAT_LV_Data.Reset();
        }

        private void makeAllListViewData()
        {
            this.makeBCNOListData();
            this.makePTRYLPListViewData();
            this.makePTRYOPListViewData();
            this.makeMRKCTLMSTListViewData();
            this.makeINSPDATListView();
        }
        #endregion

        public event DelegateEndCsvReading OnEndCsvReading = null;
        public bool UpdateEndEvent = false;
        public eSearchProcessRes _SearchRes;
        public FormDB(object parent)
        {
            InitializeComponent();

            this.parent = parent;

            tbSearchStartTime.Enabled = false;
            tbSearchEndTime.Enabled = false;

            dbCommTimer = new System.Windows.Forms.Timer();
            dbCommTimer.Interval = 1000;
            dbCommTimer.Tick += new EventHandler(timer_DispDBConn);

            dbSearchProgressTimer = new System.Windows.Forms.Timer();
            dbSearchProgressTimer.Interval = 200;
            dbSearchProgressTimer.Tick += new EventHandler(timer_DbSearch);

            panelTitle.MouseDown += lblTitle_MouseDown;
            panelTitle.MouseMove += lblTitle_MouseMove;

            dbLoadingTime = new Stopwatch();

            this.FormClosing += Form_Closing;

            _LangType = 0;

            CreateListViewData();

            initBCNOListView();
            initPTRYLPListView();
            initPTRYOPListView();
            initMRKCTLMSTListView();
            initINSPDATListView();
            initFAULTDATListView();

        }

        private void Form_Closing(object sender, FormClosingEventArgs e)
        {
            dbLoadingTime.Stop();
            dbCommTimer.Stop();
            dbSearchProgressTimer.Stop();
            ledOff?.Dispose();
            ledOn?.Dispose();
        }

        private void FormDB_Load(object sender, EventArgs e)
        {

        }
        private void FormDB_VisibleChanged(object sender, EventArgs e)
        {
            if (this.Visible == true)
            {
                this.lblTitle.Text = $"DEFECT DATA VIEWER [{this.dataBase.DbOption.dbWhen.ToString()}]";

                if(DataBase.DbOption.isLoaded==true) displayUIOptionFromDBOption();
                else                                 displayMarkingOption();
                dbCommTimer.Start();
                dbConn.OnDbConnect += OnDbConnect;
            }
            else
            {
                dbCommTimer.Stop();
                dbConn.OnDbConnect -= OnDbConnect;
            }
        }

        private void displayUI()
        {
            displayCbcbDestination();
            displayCbLotcode();
        }

        private void displayCbcbDestination()
        {
            cbDestination.Items.Clear();

            DestConfigUnit u = new DestConfigUnit();
            for (int i = 0; i < DataBase.DbDestConfig.DicDest.Count; i++)
            {
                if (DataBase.DbDestConfig.GetData(i, ref u) == true)
                {
                    cbDestination.Items.Add(u.Title);
                }
            }

            if (DataBase.DbOption.FWPlace != null)
            {
                if (DataBase.DbDestConfig.DicDest.ContainsKey(DataBase.DbOption.FWPlace) == true)
                    cbDestination.SelectedText = DataBase.DbOption.FWPlace;
            }
            else
            {
                if (cbDestination.Items.Count > 0)
                {
                    cbDestination.SelectedIndex = 0;
                    DataBase.DbOption.FWPlace = cbDestination.SelectedItem.ToString();
                }
            }
        }

        private void displayCbLotcode()
        {
            //cbLotCode.Items.Clear();

            //if(DataBase.DBCodeConfig.DicCode.Count>0)
            //{
            //    foreach (var item in DataBase.DBCodeConfig.DicCode)
            //    {
            //        CodeUnit u = item.Value;
            //        cbLotCode.Items.Add(u.Title);
            //    }
            //}
            //else
            //{
            //    cbLotCode.Items.Add("사용안함");
            //}
        }

        private void displayMarkingOption()
        {
            if (DataBase.DbOption == null) return;
            if (DataBase.DbOption.FWPlace == null) return;
            if (DataBase.DbDestConfig.DicDest.ContainsKey(DataBase.DbOption.FWPlace) == true)
            {
                DestConfigUnit u = DataBase.DbDestConfig.DicDest[DataBase.DbOption.FWPlace];
                this.cbDestination.SelectedIndex = dataBase.DbOption.vendor;
                this.cbUseES.Checked = u.UseES;
                this.cbUseTG.Checked = u.UseTG;
                this.cbUseETC.Checked = u.UseETC;
            }
            this.tbLotName.Text = this.dataBase.DbOption.lotName;
        }

        private void displaySearchTime()
        {
            tbSearchStartTime.Text = $"{DataBase.DbOption.timeGabStMinute1}";
            tbSearchEndTime.Text = $"{DataBase.DbOption.timeGabEdMinute2}";
        }

        #region ListView
        private void clearAllListView()
        {
            if (this.InvokeRequired == true)
            {
                this.Invoke(new MethodInvoker(delegate ()
                {
                    listViewBCNO.Items.Clear();
                    listViewPTRYLP.Items.Clear();
                    listViewPTRYOP.Items.Clear();
                    listViewMRKCTLMST.Items.Clear();
                    listViewINSPDAT.Items.Clear();
                    listViewFAULTDAT.Items.Clear();
                }));
            }
            else
            {
                listViewBCNO.Items.Clear();
                listViewPTRYLP.Items.Clear();
                listViewPTRYOP.Items.Clear();
                listViewMRKCTLMST.Items.Clear();
                listViewINSPDAT.Items.Clear();
                listViewFAULTDAT.Items.Clear();
            }
        }

        private void displayAllListView()
        {
            if (this.InvokeRequired == true)
            {
                this.Invoke(new MethodInvoker(delegate ()
                {
                    displayBCNOListView();
                    displayPTRYLPListView();
                    displayPTRYOPListView();
                    displayMRKCTLMSTListView();
                    displayINSPDATListView();
                    displayFAULTDATListView();
                }));
            }
            else
            {
                displayBCNOListView();
                displayPTRYLPListView();
                displayPTRYOPListView();
                displayMRKCTLMSTListView();
                displayINSPDATListView();
                displayFAULTDATListView();
            }
        }

        #region Fault Page
        private void initFaultPage(int cnt = 0)
        {
            crtFaultDatPage = 0;
            if (cnt > 0) totalFaultDatPage = cnt / maxFaultDat;
            else totalFaultDatPage = 0;
            displayFaultPage();
        }

        private bool prevFaultPage()
        {
            if (totalFaultDatPage == 0) return false;

            crtFaultDatPage--;
            if (crtFaultDatPage < 0)
                crtFaultDatPage = 0;

            displayFAULTDATListView();
            return true;
        }

        private bool nextFaultPage()
        {
            if (totalFaultDatPage == 0) return false;

            crtFaultDatPage++;
            if (crtFaultDatPage > totalFaultDatPage)
                crtFaultDatPage = totalFaultDatPage;

            displayFAULTDATListView();

            return true;
        }

        private void displayFaultPage()
        {
            int total = dataBase.ResultDefect.MarkFault.Data.Count;
            if (this.InvokeRequired == true)
            {
                this.Invoke(new MethodInvoker(delegate ()
                {
                    tbFaultPage.Text = $"{crtFaultDatPage * 100}";
                    lblTotalFaultPage.Text = $"/{total}";
                }));
            }
            else
            {
                tbFaultPage.Text = $"{crtFaultDatPage * 100}";
                lblTotalFaultPage.Text = $"/{total}";
            }
        }
        #endregion


        #region BCNO List
        private void initBCNOListView()
        {
            listViewBCNO.View = View.Details;
            listViewBCNO.FullRowSelect = true;
            for (int i = 0; i < BCnoHeader.Length; i++)
                listViewBCNO.Columns.Add(BCnoHeader[i], listBCnoWidth[i]);
        }

        private void makeBCNOListDataByCSV()
        {
            int headCnt = listViewBCNO.Items.Count + 1;
            try
            {
                DBListViewBuf itemBuf = new DBListViewBuf(4);
                itemBuf.items[0] = headCnt.ToString();
                itemBuf.items[1] = dataBase._RollDefectInfo.LotNo + "(CSV)";
                itemBuf.items[2] = dataBase._RollDefectInfo.BadCnt.ToString();
                itemBuf.items[3] = $"{dataBase._RollDefectInfo.GetDefectPerM():F3}";
                BCNO_LV_Data.Data.Add(itemBuf);
            }
            finally
            {

            }
        }

        private void makeBCNOListData()
        {
            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            List<List<INSPDATData>>[] data = DataBase.INSPDAT_Data;

            // 리스트 초기화는 따로 불러서 처리

            bool useSplit = DataBase.DbOption.searchOP.useSplit;
            string title = DataBase.DbOption.searchOP.Title;
            eCSV_TYPE type = dataBase.DbDestConfig.CSVType;

            int total = 0;

            for (int i = 0; i < count; i++)
            {
                int subCnt = data[i].Count;
                for (int j = 0; j < subCnt; j++)
                {
                    int cnt = data[i][j].Count;
                    for (int k = 0; k < cnt; k++)
                    {
                        INSPDATData tmpData = data[i][j][k];

                        DBListViewBuf itemBuf = new DBListViewBuf(6);
                        // No.0
                        if (useSplit == true) itemBuf.items[0] = $"{total + 1}({title})";
                        else itemBuf.items[0] = $"{total + 1}";
                        total++;
                        // No.1
                        if (type == eCSV_TYPE.KORENO || type == eCSV_TYPE.KORENO_RK || type == eCSV_TYPE.KORENO_RK_IJP)
                        {
                            if (i == (int)eFCD.TG) itemBuf.items[1] = this.dataBase.DbOption.lotName;
                            else itemBuf.items[1] = tmpData.BCNO;
                        }
                        else
                        {
                            itemBuf.items[1] = tmpData.LOTNO;
                        }

                        // No.2
                        itemBuf.items[2] = $"{tmpData.RollCtlCnt}";
                        // No.3
                        if (tmpData.Width != 0 && tmpData.Length != 0)
                            itemBuf.items[3] = $"{(double)(tmpData.RollCtlCnt) / (double)((tmpData.Width / 1000) * (tmpData.Length / 1000)):F02}";
                        else
                            itemBuf.items[3] = "0.0";
                        // No.4
                        itemBuf.items[4] = "0";
                        // No.5
                        itemBuf.items[5] = tmpData.BCNO;
                        BCNO_LV_Data.Data.Add(itemBuf);
                    }
                }
            }
        }

        private void displayBCNOListView()
        {
            if (BCNO_LV_Data.Data == null) return;
            try
            {
                listViewBCNO.BeginUpdate();
                listViewBCNO.Items.Clear();
                foreach (DBListViewBuf data in BCNO_LV_Data.Data)
                {
                    ListViewItem item;
                    item = new ListViewItem(data.items[0]);
                    for (int i = 1; i < data.items.Length; i++)
                        item.SubItems.Add(data.items[i]);
                    listViewBCNO.Items.Add(item);
                }
            }
            finally
            {
                listViewBCNO.EndUpdate();
            }
        }

        private void updateBCNOListViewLanguage()
        {
            if (listViewBCNO.Columns.Count > 0)
            {
                listViewBCNO.Columns[3].Text = Language.listBCNO_3;
                listViewBCNO.Columns[5].Text = Language.listBCNO_5;
            }

        }
        #endregion

        #region PTRYLP List
        private void initPTRYLPListView()
        {
            listViewPTRYLP.View = View.Details;
            listViewPTRYLP.FullRowSelect = true;
            for (int i = 0; i < PTRYLPHeader.Length; i++)
                listViewPTRYLP.Columns.Add(PTRYLPHeader[i], listPTRYLPWidth[i]);
        }

        private void makePTRYLPListViewData()
        {
            foreach (PTRYLPdata data in DataBase.PTRLYP_Data)
            {
                DBListViewBuf bufData = new DBListViewBuf(6);
                bufData.items[0] = data.YLMLOT;
                bufData.items[1] = data.YLMZKY;
                bufData.items[2] = data.YLMTON.ToString();
                bufData.items[3] = data.YLMKAS.ToString();
                bufData.items[4] = data.YLMYKH;
                bufData.items[5] = data.YLSZKN;
                PTRYLP_LV_Data.Data.Add(bufData);
            }
        }

        private void displayPTRYLPListView()
        {
            if (PTRYLP_LV_Data.Data == null) return;
            try
            {
                listViewPTRYLP.BeginUpdate();
                listViewPTRYLP.Items.Clear();

                foreach (DBListViewBuf data in PTRYLP_LV_Data.Data)
                {
                    ListViewItem item = new ListViewItem(data.items[0]);
                    for (int i = 1; i < data.items.Length; i++)
                        item.SubItems.Add(data.items[i]);
                    listViewPTRYLP.Items.Add(item);
                }
            }
            finally
            {
                listViewPTRYLP.EndUpdate();
            }
        }

        private void updatePTRYLPListViewLanguage()
        {
            if (listViewPTRYLP.Columns.Count > 0)
            {
                listViewPTRYLP.Columns[0].Text = Language.listPTRYLP_0;
                listViewPTRYLP.Columns[1].Text = Language.listPTRYLP_1;
                listViewPTRYLP.Columns[5].Text = Language.listPTRYLP_5;
            }
        }
        #endregion

        #region PTRYOP List
        private void initPTRYOPListView()
        {
            listViewPTRYOP.View = View.Details;
            listViewPTRYOP.FullRowSelect = true;
            for (int i = 0; i < PTRYOPHeader.Length; i++)
                listViewPTRYOP.Columns.Add(PTRYOPHeader[i], listPTRYOPWidth[i]);
        }

        private void makePTRYOPListViewData()
        {
            List<PTRY0PData>[] tmpData = DataBase.PTRY0P_Data;
            int cnt = tmpData.Length;
            for (int i = 0; i < cnt; i++)
            {
                foreach (PTRY0PData data in tmpData[i])
                {
                    DBListViewBuf bufData = new DBListViewBuf(5);
                    bufData.items[0] = data.Y0ZKNM;
                    bufData.items[1] = data.Y0KLOT;
                    bufData.items[2] = data.Y0LNSN;
                    bufData.items[3] = data.Y0KKOL;
                    bufData.items[4] = data.Y0KSOL;
                    PTRYOP_LV_Data.Data.Add(bufData);
                }
            }
        }

        private void displayPTRYOPListView()
        {
            if (PTRYOP_LV_Data.Data == null) return;
            try
            {
                listViewPTRYOP.BeginUpdate();
                listViewPTRYOP.Items.Clear();

                foreach (DBListViewBuf data in PTRYOP_LV_Data.Data)
                {
                    ListViewItem item = new ListViewItem(data.items[0]);
                    item.SubItems.Add(data.items[1]);
                    item.SubItems.Add(data.items[2]);
                    item.SubItems.Add(data.items[3]);
                    item.SubItems.Add(data.items[4]);
                    listViewPTRYOP.Items.Add(item);
                }
            }
            finally
            {
                listViewPTRYOP.EndUpdate();
            }
        }

        private void updatePTRYOPListViewLanguage()
        {
            if (listViewPTRYOP.Columns.Count > 0)
            {
                listViewPTRYOP.Columns[0].Text = Language.listPTRYOP_0;
                listViewPTRYOP.Columns[1].Text = Language.listPTRYOP_1;
                listViewPTRYOP.Columns[2].Text = Language.listPTRYOP_2;
                listViewPTRYOP.Columns[3].Text = Language.listPTRYOP_3;
                listViewPTRYOP.Columns[4].Text = Language.listPTRYOP_4;
            }
        }
        #endregion

        #region MRKCTLMST List
        private void initMRKCTLMSTListView()
        {
            listViewMRKCTLMST.View = View.Details;
            listViewMRKCTLMST.FullRowSelect = true;
            for (int i = 0; i < MRKCTLMSTHeader.Length; i++)
                listViewMRKCTLMST.Columns.Add(MRKCTLMSTHeader[i], listMRKCTLMSTWidth[i]);
        }

        private void makeMRKCTLMSTListViewData()
        {
            // 리스트 초기화는 따로
            if (DataBase.DbOption.searchOP.useDefectEdit == false)
            {
                foreach (MRKCTLMSTData data in DataBase.MRKCTLMST_Data)
                {
                    DBListViewBuf bufData = new DBListViewBuf(4);
                    bufData.items[0] = data.LNCD;
                    bufData.items[1] = data.FLTID;
                    bufData.items[2] = data.ROLLNAME;
                    bufData.items[3] = $"{data.SIZE:F02}";
                    MRKCTLMST_LV_Data.Data.Add(bufData);
                }
            }
            else
            {
                for (int i = 0; i < DataBase._MRKCTLMST_DE.Length; i++)
                {
                    if (DataBase._MRKCTLMST_DE[i] == null)
                        continue;
                    for (int j = 0; j < DataBase._MRKCTLMST_DE[i].Count; j++)
                    {
                        foreach (MRKCTLMSTData data in DataBase._MRKCTLMST_DE[i][j].data)
                        {
                            DBListViewBuf bufData = new DBListViewBuf(4);
                            bufData.items[0] = data.LNCD;
                            bufData.items[1] = data.FLTID;
                            bufData.items[2] = data.ROLLNAME;
                            bufData.items[3] = $"{data.SIZE:F02}";
                            MRKCTLMST_LV_Data.Data.Add(bufData);
                        }
                    }
                }

            }
        }

        /// <summary>
        /// DB 탐색에서 얻어온 마킹컨트롤마스터 데이터 표시
        /// 데이터는 makeMRKCTLMSTListViewData()에서 생성하여 
        /// 현재 함수에서는 표시만 함
        /// </summary>
        private void displayMRKCTLMSTListView()
        {
            if (MRKCTLMST_LV_Data.Data == null) return;
            try
            {
                listViewMRKCTLMST.BeginUpdate();
                listViewMRKCTLMST.Items.Clear();

                foreach (DBListViewBuf data in MRKCTLMST_LV_Data.Data)
                {
                    ListViewItem item = new ListViewItem(data.items[0]);
                    item.SubItems.Add(data.items[1]);
                    item.SubItems.Add(data.items[2]);
                    item.SubItems.Add(data.items[3]);
                    listViewMRKCTLMST.Items.Add(item);
                }
            }
            finally
            {
                listViewMRKCTLMST.EndUpdate();
            }
        }

        private void updateMRKCTLMSTListViewLanguage()
        {
            if (listViewMRKCTLMST.Columns.Count > 0)
            {
                listViewMRKCTLMST.Columns[0].Text = Language.listMRKCTLMST_0;
                listViewMRKCTLMST.Columns[1].Text = Language.listMRKCTLMST_1;
                listViewMRKCTLMST.Columns[2].Text = Language.listMRKCTLMST_2;
                listViewMRKCTLMST.Columns[3].Text = Language.listMRKCTLMST_3;
            }
        }
        #endregion

        #region INSPDAT List
        private void initINSPDATListView()
        {
            listViewINSPDAT.View = View.Details;
            listViewINSPDAT.FullRowSelect = true;
            for (int i = 0; i < INSPDATHeader.Length; i++)
                listViewINSPDAT.Columns.Add(INSPDATHeader[i], listINSPDATWidth[i]);
        }

        private void makeINSPDATALiseViewByCSV()
        {
            try
            {
                DBListViewBuf bufData = new DBListViewBuf(11);
                bufData.items[0] = "";
                bufData.items[1] = "";
                bufData.items[2] = dataBase._CsvDefectHeader.lotNo;
                bufData.items[3] = dataBase._CsvDefectHeader.rollNo;
                bufData.items[4] = dataBase._CsvDefectHeader.bcrInfo;
                bufData.items[5] = dataBase._CsvDefectHeader.startY;
                bufData.items[6] = dataBase._CsvDefectHeader.endY;
                bufData.items[7] = "";
                bufData.items[8] = dataBase._CsvDefectHeader.rollSY;
                bufData.items[9] = dataBase._CsvDefectHeader.rollEY;
                bufData.items[10] = dataBase._CsvDefectHeader.rollY;
                INSPDAT_LV_Data.Data.Add(bufData);
            }
            finally
            {

            }
        }

        private void makeINSPDATListView()
        {
            if (DataBase.INSPDAT_Data == null) return;
            try
            {
                foreach (List<List<INSPDATData>> data in DataBase.INSPDAT_Data)
                {
                    if (data == null) continue;
                    foreach (List<INSPDATData> items in data)
                    {
                        foreach (INSPDATData item in items)
                        {
                            DBListViewBuf bufData = new DBListViewBuf(9);

                            bufData.items[0] = item.CTLNO;
                            bufData.items[1] = item.HINMEI;
                            bufData.items[2] = item.LOTNO;
                            bufData.items[3] = item.STRDT;
                            bufData.items[4] = item.STRTM;
                            bufData.items[5] = item.ENDDT;
                            bufData.items[6] = item.ENDTM;
                            bufData.items[7] = $"{item.Width:F3}";
                            bufData.items[8] = $"{item.Length:F3}";

                            INSPDAT_LV_Data.Data.Add(bufData);
                        }
                    }
                }
            }
            finally
            {
            }
        }
        private void displayINSPDATListView()
        {
            if (INSPDAT_LV_Data.Data == null) return;
            try
            {
                listViewINSPDAT.BeginUpdate();
                listViewINSPDAT.Items.Clear();
                // 리스트 초기화는 따로
                foreach(var data in INSPDAT_LV_Data.Data)
                {
                    ListViewItem listItem = new ListViewItem(data.items[0]);
                    listItem.SubItems.Add(data.items[1]);
                    listItem.SubItems.Add(data.items[2]);
                    listItem.SubItems.Add(data.items[3]);
                    listItem.SubItems.Add(data.items[4]);
                    listItem.SubItems.Add(data.items[5]);
                    listItem.SubItems.Add(data.items[6]);
                    listItem.SubItems.Add(data.items[7]);
                    listItem.SubItems.Add(data.items[8]);
                    listViewINSPDAT.Items.Add(listItem);
                }
            }
            finally
            {
                listViewINSPDAT.EndUpdate();
            }
        }

        private void updateINSPDATListViewLanguage()
        {
            if (listViewINSPDAT.Columns.Count > 0)
            {
                listViewINSPDAT.Columns[0].Text = Language.listINSPDAT_0;
                listViewINSPDAT.Columns[1].Text = Language.listINSPDAT_1;
            }
        }
        #endregion

        #region FAULTDAT List
        private void initFAULTDATListView()
        {
            listViewFAULTDAT.View = View.Details;
            listViewFAULTDAT.FullRowSelect = true;
            for (int i = 0; i < FAULTDATHeader.Length; i++)
                listViewFAULTDAT.Columns.Add(FAULTDATHeader[i], listFAULTDATWidth[i]);
        }

        private void displayFAULTDATListView()
        {
            try
            {
                listViewFAULTDAT.BeginUpdate();
                listViewFAULTDAT.Items.Clear();

                List<MarkingFaultDatum> tmpData = dataBase.ResultDefect.MarkFault.Data;
                MarkingFaultDatum data = null;

                Param tmpParam = DataBase.CrtParam;

                int mclass = 0;
                int dummyClass = 0;
                eCSV_TYPE tmpType = dataBase.DbDestConfig.CSVType;

                DestConfigUnit destUnit = dataBase.DbDestConfig.SelDestUnit;

                int enaCnt = tmpData.Count;

                if (enaCnt > maxFaultDat * crtFaultDatPage)
                {
                    if (enaCnt >= maxFaultDat * (crtFaultDatPage + 1)) enaCnt = maxFaultDat * (crtFaultDatPage + 1);
                }

                for (int i = maxFaultDat * crtFaultDatPage; i < enaCnt; i++)
                {
                    data = tmpData[i];
                    ListViewItem item = new ListViewItem(i.ToString());
                    item.SubItems.Add(data.CTLNO);
                    item.SubItems.Add(data.FLTNO);
                    item.SubItems.Add($"{data.OFFSET:0.00#}");
                    item.SubItems.Add(String.Format($"{data.XPOS_M:0.00#}"));
                    item.SubItems.Add(String.Format($"{data.YPOS_M:0.00#}"));
                    item.SubItems.Add(data.MACNO);
                    item.SubItems.Add(data.FAULTID);
                    item.SubItems.Add(String.Format($"{data.SIZE:0.00#}"));
                    item.SubItems.Add(data.CAM_NO.ToString());

                    item.SubItems.Add(String.Format($"{data.DefectLine}"));

                    if (tmpType == eCSV_TYPE.KORENO_RK) dummyClass = tmpParam._UserDefectClass.ConvetLine2DefectType(data.DefectLine, tmpType, destUnit.OPTIC1, destUnit.OPTIC2, destUnit.OPTIC4, ref mclass);
                    else mclass = tmpParam._UserDefectClass.ConvetLine2DefectType(data.DefectLine, tmpType, destUnit.OPTIC1, destUnit.OPTIC2, destUnit.OPTIC4, ref dummyClass);

                    item.SubItems.Add(String.Format($"0x{mclass:X}"));
                    item.SubItems.Add(String.Format($"{data.XOFFSET:0.00#}"));
                    item.SubItems.Add(data.MNTTID);
                    listViewFAULTDAT.Items.Add(item);
                }
            }
            finally
            {
                listViewFAULTDAT.EndUpdate();
            }
        }

        private void updateFAULTDATListViewLanguage()
        {
            if (listViewFAULTDAT.Columns.Count > 0)
            {
                listViewFAULTDAT.Columns[1].Text = Language.listFAULTDAT_1;
                listViewFAULTDAT.Columns[2].Text = Language.listFAULTDAT_2;
                listViewFAULTDAT.Columns[6].Text = Language.listFAULTDAT_6;
                listViewFAULTDAT.Columns[7].Text = Language.listFAULTDAT_7;
            }
        }
        #endregion

        #endregion ListView

        #region Defect Search Thread
        private Thread thread = null;
        private string destName;
        private string csvPath;

        private void threadFromDB()
        {
            bool isSuccess = true;
            try
            {
                this.dbLoadingTime.Reset();
                this.dbLoadingTime.Start();
                int errorOut = 0;
                DestConfigUnit unit = null;
                unit = DataBase.DbDestConfig.DicDest[this.destName];
                this.dbSearchProgressTimer.Start();
                // 검사 옵션 업데이트
                Option option = dataBase.DbOption;
                SearchOption searchOp = new SearchOption();
                option.searchOP = searchOp;

                searchOp.useDefectEdit = false;
                // 검색 시간 갭 설정

                if (cbSetSearchTime.Checked == true)
                {
                    if (Int32.TryParse(tbSearchStartTime.Text, out int value) == true || value != 0)
                        option.timeGabStMinute1 = value;
                    else
                        option.timeGabStMinute1 = DataBase.DbDestConfig.DbTime.start;

                    if (Int32.TryParse(tbSearchEndTime.Text, out value) == true || value != 0)
                        option.timeGabEdMinute2 = value;
                    else
                        option.timeGabEdMinute2 = DataBase.DbDestConfig.DbTime.end;
                }
                else
                {
                    option.timeGabStMinute1 = DataBase.DbDestConfig.DbTime.start;
                    option.timeGabEdMinute2 = DataBase.DbDestConfig.DbTime.end;
                }

                option.timeGabStMinute2 = DataBase.DbDestConfig.DbTime.start;
                option.timeGabEdMinute1 = DataBase.DbDestConfig.DbTime.end;

                option.timeGabEsStMinute1 = DataBase.DbDestConfig.ESDbTime.start;
                option.timeGabEsStMinute2 = DataBase.DbDestConfig.ESDbTime.start;
                option.timeGabEsEdMinute1 = DataBase.DbDestConfig.ESDbTime.end;
                option.timeGabEsEdMinute2 = DataBase.DbDestConfig.ESDbTime.end;
                option.useESTime = DataBase.DbDestConfig.ESDbTime.IsUse;

                if (formProgress != null) formProgress._Step = 0;

                searchOp.MKCD = unit.MKCD;
                dataBase.DbOption = option;
                dataBase.ResetDataSplit();
                isSuccess &= dataBase.SearchLot(this.dataBase.DbOption.lotName, false, ref errorOut);
                // 데이터 처리 필요
                if (dataBase.CrtParam.isProductAvaliable == false)
                {

                }

                if (dataBase.CrtParam.isXOffsetError == true)
                {

                }

                // List View 업데이트 데이터 생성
                this.makeAllListViewData();
                this.displayAllListView();

                // Fault Data 표시
                this.initFaultPage(this.dataBase.ResultDefect.MarkFault.Data.Count);

                DataBase.DbOption.isLoaded = true;
            }
            finally
            {
                this.dbSearchProgressTimer.Stop();
                this.dbLoadingTime.Stop();

                if (isSuccess == false)
                    this._SearchRes = eSearchProcessRes.DB_NoExistES;
                else
                    this._SearchRes = eSearchProcessRes.DB_SearchDone;

                if (this.UpdateEndEvent == true)
                {
                    OnEndCsvReading((int)eEventReport.eFinishedSearchLot);
                    this.UpdateEndEvent = false;
                }

                // FLTID 비교 발생 시 에러 알람
                if(dataBase.CrtParam.FLTIDCheckError==true)
                    OnEndCsvReading((int)eEventReport.eBCR_FLTID_CheckError);
                // ROLL MAP 거리 비교 에러 시 알람 처리
                if(dataBase.CrtParam.InspRollCheckError==true)
                    OnEndCsvReading((int)eEventReport.eBCR_INSPMETER_CheckError);

                this.updateSearchResult(isSuccess, 0);
            }
        }

        public void UpdateListViewFromLotChange()
        {
            if (this.IsHandleCreated == true)
            {
                this.BeginInvoke((Action)(() =>
                {
                    updateLotChangeResult();
                }));
            }
            else
            {
                updateLotChangeResult();
            }
        }

        private void updateLotChangeResult()
        {
            this.clearAllListView();
            this.initFaultPage(dataBase.ResultDefect.MarkFault.Data.Count);
            displayBCNOListView();
            displayPTRYLPListView();
            displayPTRYOPListView();
            displayMRKCTLMSTListView();
            displayINSPDATListView();
            displayFAULTDATListView();

            if (this.dataBase.DbOption.dbWhen == eDbIdWhen.Now)
            {
                lblDownloadResult.Text = "Lot Change is finished.";
            }   
            else
            {
                lblDownloadResult.Text = "ResultFault :";
            }
        }

        private void resetListView()
        {
            //리스트 클리어
            this.ResetListViewData();
            this.clearAllListView();
            this.initFaultPage();
        }

        private void updateSearchResult(bool isSuccess, int searchType)
        {
            string msg = "";
            double elapsedTime = (double)dbLoadingTime.ElapsedMilliseconds / 1000.0;
            if (searchType == 0)
                msg = "DB Searching Lot is ";
            else if (searchType == 1)
                msg = "DB Searching Model is ";
            if (isSuccess == false)
                msg += $"failed!! : {elapsedTime:F3}";
            else
                msg += $"complete!! : {elapsedTime:F3}";

            //Fault Data 표시
            if (this.InvokeRequired == true)
            {
                this.Invoke(new MethodInvoker(delegate ()
                {
                    lblDownloadResult.Text = msg;
                }));
            }
            else
            {
                lblDownloadResult.Text = msg;
            }
        }

        private void threadSearchModelFromDB()
        {
            bool isSuccess = true;

            try
            {
                this.resetListView();
                this.dbLoadingTime.Reset();
                this.dbLoadingTime.Start();
                this.dbSearchProgressTimer.Start();
                if (formProgress != null) formProgress._Step = 0;
                isSuccess &= dataBase.SearchModel(this.dataBase.DbOption.lotName, false);

                // List View 업데이트 데이터 생성
                this.makeAllListViewData();
                this.displayAllListView();
            }
            catch (Exception ex)
            {
                Trace.WriteLine($"threadSearchModelFromDB() Error Message : {ex.Message}");
            }
            finally
            {
                this._SearchRes = eSearchProcessRes.DB_SearchDone;
                this.dbSearchProgressTimer.Stop();
                this.dbLoadingTime.Stop();
                OnEndCsvReading((int)eEventReport.eFinishedSearchModel);
                this.updateSearchResult(isSuccess, 1);
            }
        }

        private void threadFromCSV()
        {
            this.dataBase.OpenCsvFile(this.csvPath);
            DataBase.DbOption.isLoaded = true;
            this.Invoke(new MethodInvoker(delegate ()
            {
                this.initFaultPage(dataBase.ResultDefect.MarkFault.Data.Count);
                // CSV 파일에서 BCNO Data 버퍼 생성
                this.makeBCNOListDataByCSV();
                this.displayBCNOListView();
                this.displayFAULTDATListView();

                if (this.dataBase.DbDestConfig.CSVType == eCSV_TYPE.KORENO || this.dataBase.DbDestConfig.CSVType == eCSV_TYPE.KORENO_RK ||
                this.dataBase.DbDestConfig.CSVType == eCSV_TYPE.KORENO_RK_IJP)
                {
                    this.makeINSPDATALiseViewByCSV();
                    this.displayINSPDATListView();
                }

                lblDownloadResult.Text = $"ResultFault : {dataBase._RollDefectInfo.BadCnt}";
            }));
        }
        #endregion

        #region Form
        private void btnSearchDB_Click(object sender, EventArgs e)
        {
            if (this.thread != null && this.thread.IsAlive == true)
            {
                MessageBox.Show($"DB 데이터 검색중입니다.");
                return;
            }

            if (dataBase.Conn.IsConnected() == false)
            {
                MessageBox.Show($"먼저 DB에 접속해주십시요.");
                return;
            }

            if (tbLotName.Text.Length == 0)
            {
                MessageBox.Show("Lot 번호를 입력하세요!");
                return;
            }
            if (tbLotName.Text.Length < Global.LotNameLength)
            {
                MessageBox.Show("10자리의 Lot 번호를 입력하세요!");
                return;
            }

            // 화면 데이터 적용
            updateUIOptionToDBOption();
            this.UpdateEndEvent = false;
            searchLotDefect();
        }

        public bool IsSearchDefect()
        {
            if (this.thread != null && this.thread.IsAlive == true)
                return true;

            return false;
        }


        /// <summary>
        ///  Defect Data 탐색
        ///  Client에서 받은 명령으로 수행
        /// </summary>
        public void SearchDefect()
        {
            if (IsSearchDefect() == true) return;

            // delete last defect list data
            ResetListViewData();

            DataBase.DbOption.isLoaded = false;
            // 화면에서 데이터 얻어온게 아니라서 화면에 현재 데이터 출력해줘야 함.
            displayUIOptionFromDBOption();
            DestConfigUnit u = new DestConfigUnit();
            this.dataBase.DbDestConfig.GetData(dataBase.DbOption.vendor, ref u);
            dataBase.DbDestConfig.SelDestUnit = u;
            dataBase.DbOption.FWPlace = u.Title;
            searchLotDefect();
        }

        /// <summary>
        /// Lot 이름으로 불량 데이터 탐색
        /// </summary>
        /// <param name="isHide"> 화면 숨김 상태 확인</param>
        private void searchLotDefect()
        {
            if (IsSearchDefect() == true) return;

            string destName = cbDestination.Items[this.dataBase.DbOption.vendor].ToString();
            if (DataBase.DbDestConfig.DicDest.ContainsKey(destName) == true)
            {
                this.destName = destName;
            }
            else
            {
                MessageBox.Show($"해당하는 출하처 정보가 존재하지 않습니다. : [{destName}]");
                return;
            }

            if (this.thread != null)
            {
                this.thread.Join(100);
                this.thread = null;
            }

            formProgress?.Dispose();
            formProgress = new FormDbProgress();
            formProgress._LotName = this.dataBase.DbOption.lotName;
            formProgress._DbProgress = DataBase.DB_Progress;
            formProgress._LastProgress = eNittoDBProgress.FAULTDAT_ETC;
            formProgress._DispType = 0;
            formProgress._Unit = DataBase.DbDestConfig.DicDest[this.destName];
            formProgress.Show();

            this.thread = new Thread(this.threadFromDB);
            this.thread.Start();
        }

        public void SearchModel()
        {
            if (IsSearchDefect() == true) return;
            tbLotName.Text = this.dataBase.DbOption.lotName;

            if (this.thread != null)
            {
                this.thread.Join(100);
                this.thread = null;
            }

            formProgress?.Dispose();
            formProgress = new FormDbProgress();
            formProgress._LotName = this.dataBase.DbOption.lotName;
            formProgress._DbProgress = DataBase.DB_Progress;
            formProgress._LastProgress = eNittoDBProgress.PTRYOP;
            formProgress._DispType = 1;
            formProgress._Unit = null;
            formProgress.Show();

            this.thread = new Thread(this.threadSearchModelFromDB);
            this.thread.Start();
        }

        private void btnOpenCSV_Click(object sender, EventArgs e)
        {
            if (this.thread != null && this.thread.IsAlive == true)
            {
                MessageBox.Show($"데이터 검색중입니다.");
                return;
            }

            using (OpenFileDialog browser = new OpenFileDialog())
            {
                //browser.InitialDirectory = Define.MainPath;
                browser.Filter = "CSV Files (*.csv)|*.csv|모든 파일 (*.*)|*.*";
                browser.FilterIndex = 1;
                browser.RestoreDirectory = true;

                if (browser.ShowDialog() == DialogResult.OK)
                {
                    if (File.Exists(browser.FileName) == false)
                    {
                        MessageBox.Show($"파일이 존재하지 않습니다. : [{browser.SafeFileName}]");
                        return;
                    }

                    DestConfig config = dataBase.DbDestConfig;
                    if (config.CSVType != eCSV_TYPE.NITTO && config.CSVType != eCSV_TYPE.NITTO_RTS &&
                        config.CSVType != eCSV_TYPE.NITTO_RK && config.CSVType != eCSV_TYPE.KORENO &&
                        config.CSVType != eCSV_TYPE.KORENO_RK && config.CSVType != eCSV_TYPE.KORENO_RK_IJP)
                    {
                        MessageBox.Show("선택된 CSV형식이 없습니다.Dest.ini파일을 확인하시길 바랍니다.");
                        return;
                    }

                    this.csvPath = browser.FileName;
                    updateUIOptionToDBOption();

                    if (this.thread != null)
                    {
                        this.thread.Join(100);
                        this.thread = null;
                    }

                    this.thread = new Thread(this.threadFromCSV);
                    this.thread.Start();
                }
            }
        }


        private void updateUIOptionToDBOption()
        {
            this.dataBase.DbOption.lotName = (string)tbLotName.Text.Clone();
            this.dataBase.DbOption.vendor = cbDestination.SelectedIndex;
            string dest = cbDestination.SelectedItem.ToString();
            dataBase.DbOption.FWPlace = dest;
            dataBase.DbDestConfig.SetSelDest(dest);
            dataBase.DbOption.checkES = cbUseES.Checked;
            dataBase.DbOption.checkTG = cbUseTG.Checked;
            dataBase.DbOption.checkETC = cbUseETC.Checked;
            dataBase.DbOption.searchOP.useMask = cbUseMask.Checked;
            if (Int32.TryParse(tbSearchEndTime.Text, out int val) == true)
                dataBase.DbOption.timeGabEdMinute2 = val;
            if (Int32.TryParse(tbSearchStartTime.Text, out val) == true)
                dataBase.DbOption.timeGabStMinute1 = val;
        }

        private void displayUIOptionFromDBOption()
        {
            tbLotName.Text = this.dataBase.DbOption.lotName;
            cbDestination.SelectedIndex = dataBase.DbOption.vendor;
            cbUseES.Checked = dataBase.DbOption.checkES;
            cbUseTG.Checked = dataBase.DbOption.checkTG;
            cbUseETC.Checked = dataBase.DbOption.checkETC;
        }

        private void btnReset_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("데이터를 초기화 하시겠습니까?", "Reset Fault Data", MessageBoxButtons.YesNo) == DialogResult.No)
            {
                return;
            }

            // ListView 초기화
            this.clearAllListView();
            this.initFaultPage();
            this.ResetListViewData();
            dataBase.ResetDataAll();
            dataBase.ResetData_DE();

            tbLotName.Text = "";

            if (dataBase.DbOption.dbWhen == eDbIdWhen.Now)
                OnEndCsvReading((int)eEventReport.eResetDataNow);
            else
                OnEndCsvReading((int)eEventReport.eResetDataNext);

            DataBase.DbOption.isLoaded = false;
        }
        private void btnPrevFaultPage_Click(object sender, EventArgs e)
        {
            if (prevFaultPage() == true)
            {
                displayFaultPage();
            }
        }

        private void btnNextFaultPage_Click(object sender, EventArgs e)
        {
            if (nextFaultPage() == true)
            {
                displayFaultPage();
            }
        }

        private void btnDBConn_Click(object sender, EventArgs e)
        {
            if (this.dbConn == null) return;

            this.dbConn.UserID = dataBase.DbDestConfig.dbLogin.DbID;
            this.dbConn.Password = dataBase.DbDestConfig.dbLogin.DbPW;
            this.dbConn.DBName = dataBase.DbDestConfig.dbLogin.DbName;
            this.dbConn.DBPort = dataBase.DbDestConfig.dbLogin.DBPort;
            this.dbConn.HostIP = dataBase.DbDestConfig.dbLogin.DBIP;
            this.dbConn.ConStringType = dataBase.DbDestConfig.dbLogin.DBConStringType;

            if (formLogin == null) formLogin = new FormDbLoginData(this.dbConn);

            formLogin.Conn = this.dbConn;
            formLogin.ShowDialog();
            if (this.dbConn.IsDBConnected == true)
            {
                dataBase.DbDestConfig.dbLogin.DbID = this.dbConn.UserID;
                dataBase.DbDestConfig.dbLogin.DbPW = this.dbConn.Password;
                dataBase.DbDestConfig.dbLogin.DbName = this.dbConn.DBName;
                dataBase.DbDestConfig.dbLogin.DBConStringType = this.dbConn.ConStringType;
                dataBase.DbDestConfig.dbLogin.DBIP = this.dbConn.HostIP;
                dataBase.DbDestConfig.dbLogin.DBPort = this.dbConn.DBPort;
            }
        }

        private void btnXOFSMST_Click(object sender, EventArgs e)
        {
            if (formDbAddition == null)
                formDbAddition = new FormDbAddition();

            if (formDbAddition.Visible == true)
                return;

            formDbAddition._XOFSMSTData = DataBase.XOFSMST_Data;
            formDbAddition._AREADELData = DataBase.AREADEL_Data;

            formDbAddition.Show();
        }

        private void btnExportCSV_Click(object sender, EventArgs e)
        {
            bool isES = dataBase.DbOption.checkES;
            bool isTG = dataBase.DbOption.checkTG;

            using (SaveFileDialog browser = new SaveFileDialog())
            {
                browser.InitialDirectory = Define.MainPath;
                browser.Filter = "CSV Files (*.csv)|*.csv|모든 파일 (*.*)|*.*";
                browser.FilterIndex = 1;
                browser.RestoreDirectory = true;

                if (browser.ShowDialog() == DialogResult.OK)
                {
                    string path = browser.FileName;

                    StreamWriter wr = new StreamWriter(path);

                    wr.WriteLine("Header line");
                    int idx = 0;
                    foreach (MarkingFaultDatum data in DataBase.ResultDefect.MarkFault.Data)
                    {
                        StringBuilder sb = new StringBuilder();

                        sb.Append($"{idx},");           // 0
                        sb.Append($",");                // 1
                        sb.Append($"{data.FLTNO},");    // 2

                        //3
                        if (isES) sb.Append("100,");
                        else if (isTG) sb.Append("400,");
                        else sb.Append("200,");

                        sb.Append($"{data.YPOS_M:F3},"); // 4
                        sb.Append($"{data.XPOS_M:F3},"); // 5
                        sb.Append($"{data.SIZE:F3},");   // 6
                        sb.Append($"{data.SIZE_Y:F3},"); // 7
                        sb.Append($"{data.SIZE_X:F3},"); // 8
                        sb.Append($"{data.OFFSET:F3},"); // 9
                        sb.Append($"{data.CAM_NO},");    // 10
                        sb.Append($",");                 // 11
                        sb.Append($",");                 // 12
                        sb.Append($",");                 // 13
                        sb.Append($",");                 // 14
                        sb.Append($"0,");                // 15
                        sb.Append($"{data.BCNO},");      // 16
                        sb.Append("0");                  // 17
                        wr.WriteLine(sb.ToString());

                        idx++;
                    }

                    wr.Close();
                }
            }
        }

        private void cbDestination_SelectedIndexChanged(object sender, EventArgs e)
        {
            DataBase.DbOption.FWPlace = cbDestination.SelectedItem.ToString();

            displayMarkingOption();
            displaySearchTime();
        }

        private void btnApplySearchTime_Click(object sender, EventArgs e)
        {
            if (Int32.TryParse(tbSearchStartTime.Text, out int value) == true)
            {
                DataBase.DbOption.timeGabStMinute1 = value;
            }
            else
            {
                MessageBox.Show("숫자를 입력해주십시요.");
                return;
            }

            if (Int32.TryParse(tbSearchEndTime.Text, out value) == true)
            {
                DataBase.DbOption.timeGabEdMinute2 = value;
            }
            else
            {
                MessageBox.Show("숫자를 입력해주십시요.");
                return;
            }
        }

        private void cbSetSearchTime_CheckedChanged(object sender, EventArgs e)
        {
            bool check = cbSetSearchTime.Checked;
            tbSearchStartTime.Enabled = check;
            tbSearchEndTime.Enabled = check;
            btnApplySearchTime.Enabled = check;
        }

        private void btnShowSkipParam_Click(object sender, EventArgs e)
        {
            using (FormSkipParam form = new FormSkipParam())
            {
                form._Param = dataBase.CrtParam;
                form.Config = dataBase.DbDestConfig;
                form.FWPlace = dataBase.DbOption.FWPlace;

                form.ShowDialog();
            }
        }

        private void btnEditDefectClass_Click(object sender, EventArgs e)
        {
            using (FormDefectClassEdit form = new FormDefectClassEdit())
            {
                form._UserDefectClass = dataBase.CrtParam._UserDefectClass;
                form.ShowDialog();
            }
        }

        private void tbFaultPage_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                if (Int32.TryParse(tbFaultPage.Text, out int intput) == true)
                {
                    int total = dataBase.ResultDefect.MarkFault.Data.Count;
                    if (intput < total)
                    {
                        crtFaultDatPage = intput / maxFaultDat;
                        tbFaultPage.Text = $"{crtFaultDatPage * 100}";
                        displayFAULTDATListView();
                    }
                    else
                        tbFaultPage.Text = $"{crtFaultDatPage * 100}";
                }
                else
                {
                    tbFaultPage.Text = $"{crtFaultDatPage * 100}";
                }
            }
        }

        private void btnEditDefect_Click(object sender, EventArgs e)
        {
            RunDefectEdit();
        }

        private void btnUpdateMarkingData_Click(object sender, EventArgs e)
        {
            if (dataBase.DbOption.dbWhen == eDbIdWhen.Now)
                OnEndCsvReading((int)eEventReport.eUpdateDataNow);
            else
                OnEndCsvReading((int)eEventReport.eUpdateDataNext);

            // 임시 데이터 생성

            //if (eDbIdWhen.Now == dataBase.DbOption.dbWhen)
            //{
            //    DataBase.AREADEL_Data.Clear();

            //    for(int i=0; i<100; i++)
            //    {
            //        AREADELData data = new AREADELData();

            //        data.ENTRY_NUM = i.ToString();
            //        data.STR_WD = 100;
            //        data.END_WD = 300;
            //        data.STR_MD = 1406500.0f + (float)i * 10000.0f;
            //        data.END_MD = 1407000.0f + (float)i * 10000.0f;

            //        DataBase.AREADEL_Data.Add(data);
            //    }

            //    Trace.WriteLine($"Area Defect Count : {DataBase.AREADEL_Data.Count}");
            //}
        }

        private void btnFormMaximize_Click(object sender, EventArgs e)
        {
            if (this.WindowState == FormWindowState.Maximized)
                this.WindowState = FormWindowState.Normal;
            else if (this.WindowState == FormWindowState.Normal)
                this.WindowState = FormWindowState.Maximized;
        }

        private void btnFormHide_Click(object sender, EventArgs e)
        {
            if (this._FormCloseBtnType == true)
                this.Close();
            else
                this.Hide();
        }

        private void OnDbConnect(bool state)
        {
            if (state)
            {
                if (this.formLogin?.Visible == true)
                {
                    this.formLogin.Close();
                }
            }
        }

        private void tbLotName_TextChanged(object sender, EventArgs e)
        {
            this.dataBase.DbOption.lotName = this.tbLotName.Text;
        }
        #endregion

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

        #region Timer

        bool isOldConn = false;
        Image ledOn = Properties.Resources.icons8_green_square_16;
        Image ledOff = Properties.Resources.icons8_black_medium_square_16;
        private void timer_DispDBConn(object sender, EventArgs e)
        {
            if (this.dbConn == null) return;

            if (this.dbConn.IsDBConnected == true)
            {
                if (isOldConn == false)
                {
                    lblDbConnState.Text = Language.Connected;
                    lblDbConnStateIcon.Image = ledOn;
                    isOldConn = true;
                }
            }
            else
            {
                if (isOldConn == true)
                {
                    lblDbConnState.Text = Language.Disconnected;
                    lblDbConnStateIcon.Image = ledOff;
                    isOldConn = false;
                }
            }
        }

        private void timer_DbSearch(object sender, EventArgs e)
        {
            if (this.dataBase == null) return;

            StringBuilder sb = new StringBuilder();

            if (this.dataBase.DB_Progress.IsError == true)
            {
                sb.Append($"Error is occured : Error Step [{((eNittoDBProgress)dataBase.DB_Progress.ErrorStep).ToString()}]");
                lblDownloadResult.Text = sb.ToString();
                return;
            }

            int maxIdx = System.Enum.GetValues(typeof(eNittoDBProgress)).Length;

            // 현재 진항하고 있는 마지막 스텝을 확인한다. 
            int finalStep = 0;
            for (int i = 0; i < maxIdx; i++)
            {
                if (this.dataBase.DB_Progress._Progress[i].IsComplete() == true || this.dataBase.DB_Progress._Progress[i].IsSkip == true)
                    finalStep = i;
                else
                    break;
            }

            // 화면 출력 고려하여 이전 3스텝만 표시함
            finalStep -= 3;
            if (finalStep < 0) finalStep = 0;


            for (int i = finalStep; i < maxIdx; i++)
            {

                switch ((eNittoDBProgress)i)
                {
                    case eNittoDBProgress.PTRYLP:
                        if (this.dataBase.DB_Progress._Progress[i].IsComplete() == true)
                            sb.Append("PTRYLP Complete => ");
                        else
                        {
                            sb.Append($"PTRYLP is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.XOFSMST:
                        if (this.dataBase.DB_Progress._Progress[i].IsComplete() == true)
                            sb.Append("XOFSMST Complete => ");
                        else
                        {
                            sb.Append($"XOFSMST is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;


                    case eNittoDBProgress.AREADEL:
                        if (this.dataBase.DB_Progress._Progress[i].IsComplete() == true)
                            sb.Append("AREADEL Complete => ");
                        else
                        {
                            sb.Append($"AREADEL is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.PTRYOP:
                        if (this.dataBase.DB_Progress._Progress[i].IsComplete() == true)
                            sb.Append("PTRYOP Complete => ");
                        else
                        {
                            sb.Append($"PTRYOP is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.MRKCTLMST_ES:
                        if (this.dataBase.DB_Progress._Progress[i].IsSkip == true)
                            sb.Append("MRKCTLMST_ES Skip => ");
                        else if (this.dataBase.DB_Progress._Progress[i].IsComplete() == true)
                            sb.Append("MRKCTLMST_ES Complete => ");
                        else
                        {
                            sb.Append($"MRKCTLMST_ES is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.MRKCTLMST_TG:
                        if (this.dataBase.DB_Progress._Progress[i].IsSkip == true)
                            sb.Append("MRKCTLMST_TG Skip => ");
                        else if (this.dataBase.DB_Progress._Progress[i].IsComplete() == true)
                            sb.Append("MRKCTLMST_TG Complete => ");
                        else
                        {
                            sb.Append($"MRKCTLMST_TG is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.MRKCTLMST_ETC:
                        if (this.dataBase.DB_Progress._Progress[i].IsSkip == true)
                            sb.Append("MRKCTLMST_ETC Skip => ");
                        else if (this.dataBase.DB_Progress._Progress[i].IsComplete() == true)
                            sb.Append("MRKCTLMST_ETC Complete => ");
                        else
                        {
                            sb.Append($"MRKCTLMST_ETC is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.INSPDAT_ES:
                        if (this.dataBase.DB_Progress._Progress[i].IsSkip == true)
                            sb.Append("INSPDAT_ES Skip => ");
                        else if (this.dataBase.DB_Progress._Progress[i].IsComplete() == true)
                            sb.Append("INSPDAT_ES Complete => ");
                        else
                        {
                            sb.Append($"INSPDAT_ESis processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.INSPDAT_TG:
                        if (this.dataBase.DB_Progress._Progress[i].IsSkip == true)
                            sb.Append("INSPDAT_TG Skip => ");
                        else if (this.dataBase.DB_Progress._Progress[i].IsComplete() == true)
                            sb.Append("INSPDAT_TG Complete => ");
                        else
                        {
                            sb.Append($"INSPDAT_TG is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.INSPDAT_ETC:
                        if (this.dataBase.DB_Progress._Progress[i].IsSkip == true)
                            sb.Append("INSPDAT_ETC Skip => ");
                        else if (this.dataBase.DB_Progress._Progress[i].IsComplete() == true)
                            sb.Append("INSPDAT_ETC Complete => ");
                        else
                        {
                            sb.Append($"INSPDAT_ETC is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.FAULTDAT_ES:
                        if (this.dataBase.DB_Progress._Progress[i].IsSkip == true)
                            sb.Append("FAULTDAT_ES Skip => ");
                        else if (this.dataBase.DB_Progress._Progress[i].IsComplete() == true)
                            sb.Append("FAULTDAT_ES Complete => ");
                        else
                        {
                            sb.Append($"FAULTDAT_ES is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.FAULTDAT_TG:
                        if (this.dataBase.DB_Progress._Progress[i].IsSkip == true)
                            sb.Append("FAULTDAT_TG Skip => ");
                        else if (this.dataBase.DB_Progress._Progress[i].IsComplete() == true)
                            sb.Append("FAULTDAT_TG Complete => ");
                        else
                        {
                            sb.Append($"FAULTDAT_TG is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.FAULTDAT_ETC:
                        if (this.dataBase.DB_Progress._Progress[i].IsSkip == true)
                            sb.Append("FAULTDAT_ETC Skip => ");
                        else if (this.dataBase.DB_Progress._Progress[i].IsComplete() == true)
                            sb.Append("FAULTDAT_ETC Complete => ");
                        else
                        {
                            sb.Append($"FAULTDAT_ETC is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;
                }
            }
        }

        #endregion Timer

        #region Defect Edit
        public void RunDefectEdit()
        {
            int errorIdx = -1;
            DestConfigUnit unit = new DestConfigUnit();
            int vendorIdx = this.cbDestination.SelectedIndex;
            dataBase.DbDestConfig.GetData(vendorIdx, ref unit);

            DataBase.ResetData_DE();

            for (int i = 0; i < 10; i++)
            {
                PTRY0PData data = new PTRY0PData();
                data.Y0KLOT = $"{i}";
                DataBase.PTRY0P_Data[0].Add(data);
            }

            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            int queryCount = 0;
            for (int i = 0; i < count; i++)
            {
                for (int j = 0; j < DataBase.PTRY0P_Data[i].Count; j++)
                {
                    QueryMsg.MRKCTLMST_DE_Query msg = new QueryMsg.MRKCTLMST_DE_Query();
                    msg.Y0KLOT = DataBase.PTRY0P_Data[i][j].Y0KLOT;
                    msg.MKCD = unit.MKCD;
                    MRKCTLMST_DE_Data de_data = new MRKCTLMST_DE_Data();
                    if (cbUseES.Checked == true && i == (int)eFCD.ES)
                    {
                        de_data.query = msg.GetQuery(eFCD.ES);
                        DataBase._MRKCTLMST_DE[i].Add(de_data);
                        queryCount++;
                    }
                    else if (cbUseETC.Checked == true && i == (int)eFCD.ETC)
                    {
                        de_data.query = msg.GetQuery(eFCD.ETC);
                        DataBase._MRKCTLMST_DE[i].Add(de_data);
                        queryCount++;
                    }
                    else if (cbUseTG.Checked == true && i == (int)eFCD.TG)
                    {
                        de_data.query = msg.GetQuery(eFCD.TG);
                        DataBase._MRKCTLMST_DE[i].Add(de_data);
                        queryCount++;
                    }
                    else
                    {
                        de_data.query = "";
                        DataBase._MRKCTLMST_DE[i].Add(de_data);
                    }
                }
            }

            if (queryCount > 0)
            {
                using (FormEditDefect form = new FormEditDefect())
                {
                    form._DataBase = DataBase;
                    if (form.ShowDialog() == DialogResult.OK)
                    {
                        if (MessageBox.Show("선택된 결점정보를 적용하시겠습니까?", "Defect Editor",
                            MessageBoxButtons.YesNo) == DialogResult.Yes)
                        {
                            DataBase.ResetDataAll();
                            Option option = DataBase.DbOption;
                            SearchOption searchOP = new SearchOption();
                            option.searchOP = searchOP;
                            searchOP.useMask = cbUseMask.Checked;
                            searchOP.useDefectEdit = true;
                            DataBase.SearchLot(this.dataBase.DbOption.lotName, false, ref errorIdx);

                            clearAllListView();
                            ResetListViewData();
                            displayAllListView();
                            // Fault Data 표시
                            this.initFaultPage(this.dataBase.ResultDefect.MarkFault.Data.Count);
                        }
                    }
                }
            }
            else
            {
                MessageBox.Show("결점 정보가 존재하지 않습니다.");
            }
        }
        #endregion Defect Edit

        #region Language Update
        public void setLangType()
        {
            switch (_LangType)
            {
                case 0: // 한국어
                    {
                        Thread.CurrentThread.CurrentUICulture = CultureInfo.GetCultureInfo("ko-KR");
                    }
                    break;
                case 1: // 영어
                    {
                        Thread.CurrentThread.CurrentUICulture = CultureInfo.GetCultureInfo("en-US");
                    }
                    break;

                case 2: // 중국어
                    {
                        Thread.CurrentThread.CurrentUICulture = CultureInfo.GetCultureInfo("zh-CN");
                    }
                    break;
            }
            UpdateLanguage();
        }
        public void UpdateLanguage()
        {
            this.SuspendLayout();
            btnDBConn.Text = Language.Connection;
            if (this.dbConn?.IsDBConnected == true)
                lblDbConnState.Text = Language.Connected;
            else
                lblDbConnState.Text = Language.Disconnected;

            lblLotName.Text = Language.LotName;
            lblDestination.Text = Language.Destination;
            cbSetSearchTime.Text = Language.SetSearchTime;
            lblSearchStartTime.Text = Language.SearchStartTime;
            lblSearchEndTime.Text = Language.SearchEndTime;
            btnApplySearchTime.Text = Language.Apply;
            btnSearchDB.Text = Language.Search;
            btnOpenCSV.Text = Language.OpenCSV;
            btnUpdateMarkingData.Text = Language.UpdateData;
            btnExportCSV.Text = Language.ExportCSV;
            btnReset.Text = Language.Reset;
            gbMarkingGroup.Text = Language.MarkingGroup;
            cbUseES.Text = Language.ES;
            cbUseTG.Text = Language.TG;
            cbUseETC.Text = Language.ETC;
            gbOption.Text = Language.Option;
            cbUseMask.Text = Language.UseMask;
            btnShowSkipParam.Text = Language.ShowSkipParam;
            btnEditDefectClass.Text = Language.EditDefectClass;
            btnXOFSMST.Text = Language.ShowMore;
            btnEditDefect.Text = Language.EditDefect;
            btnPrevFaultPage.Text = Language.PrevFaultPage;
            btnNextFaultPage.Text = Language.NextFaultPage;

            updateBCNOListViewLanguage();
            updatePTRYLPListViewLanguage();
            updatePTRYOPListViewLanguage();
            updateMRKCTLMSTListViewLanguage();
            updateINSPDATListViewLanguage();
            updateFAULTDATListViewLanguage();
            this.ResumeLayout();
        }
        #endregion Language Update

    }
}
