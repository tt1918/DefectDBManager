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
using System.Security.Cryptography.X509Certificates;
using DefectDBManager.DB;

namespace DefectDBManager
{
    public partial class FormPreCompDB : Form
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
        readonly int[] listPTRYLPWidth = { 120, 300, 100, 100, 100, 300 };
        readonly string[] PTRY0PHeader = { "품종", "연신 Lot", "라인코드", "라인식별", "개시 시간", "종료 시간" };
        readonly int[] listPTRY0PWidth = { 70, 80, 60, 60, 130, 130 };
        readonly string[] MRKCTLMSTHeader = { "라인코드", "불량 ID", "품종", "사이즈" };
        readonly int[] listMRKCTLMSTWidth = { 80, 80, 180, 80 };
        readonly string[] INSPDATHeader = { "관리NO", "품종", "LOTNO", "BCNO", "STRDT", "STRTM", "ENDDT", "ENDTM", "WIDTH", "LENGTH", "ST_Y", "ED_Y" };
        readonly int[] listINSPDATWidth = { 130, 160, 90, 90, 65, 65, 65, 65, 80, 90, 80, 80 };
        readonly string[] FAULTDATHeader = { "CNT", "관리 NO", "불량 CNT", "OFFSET", "XPOS_M", "YPOS_M", "종류", "불량번호",
                                             "SIZE", "CAM", "CLASS", "M CLASS", "X_OFFSET", "MNTTID"};
        readonly int[] listFAULTDATWidth = { 45, 130, 80, 90, 110, 110, 40, 75, 55, 50, 60, 90, 90, 70 };
        private const int maxFaultDat = 100;


        #region 이전 결점 비교 처리 => List View는 Marking Control Master를 사용.
        readonly string[] PTLYOP_TodayHeader = { "LOT", "LNCD", "품종", "개시 시간", "종료 시간" };
        readonly int[] listPTLYOP_TodayWidth = { 80, 70, 160, 120, 120 };
        #endregion

        #endregion

        #region param
        private int crtFaultDatPage = 0;
        private int totalFaultDatPage = 0;

        object parent = null;

        public bool RedrawAll = false;

        private System.Windows.Forms.Timer dbCommTimer = null;
        private System.Windows.Forms.Timer dbSearchProgressTimer = null;
        private Stopwatch dbLoadingTime = null;

        public PreProcCompDB PreCompDB
        {
            get { return _preCompDB; }
            set
            {
                _preCompDB = value;
                displayUI();
                initPTRY0P_TODAYListView();
            }
        }
        private PreProcCompDB _preCompDB = null;

        public PreProcCompProcess Process
        {
            get; set;
        } = null;

        public OracleDbConnection DBConn
        {
            get { return dbConn; }
            set { dbConn = value; }
        }
        private OracleDbConnection dbConn = null;
        #endregion

        #region Form
        FormDbLoginData formLogin = null;
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
        public ListViewData PTRY0P_TODAY_LV;
        public ListViewData PTRY0P_LV_Data;
        public ListViewData INSPDAT_LV_Data;

        public void CreateListViewData()
        {
            BCNO_LV_Data = new ListViewData();
            PTRYLP_LV_Data = new ListViewData();
            PTRY0P_TODAY_LV = new ListViewData();
            PTRY0P_LV_Data = new ListViewData();
            INSPDAT_LV_Data = new ListViewData();
        }

        public void ResetListViewData()
        {
            BCNO_LV_Data.Reset();
            PTRYLP_LV_Data.Reset();
            PTRY0P_TODAY_LV.Reset();
            PTRY0P_LV_Data.Reset();
            INSPDAT_LV_Data.Reset();
        }

        private void makeAllListViewData()
        {
            this.makeBCNOListData();
            this.makePTRYLPListViewData();
            this.makePTRY0PListViewData();
            this.makeINSPDATListView();
        }
        #endregion

        public event DelegateEndJob OnEndJob = null;
        public bool UpdateEndEvent = false;
        public eSearchProcessRes _SearchRes;
        public bool IsDataBaseChanged = false;

        public bool IsHoldFW { get { return isHoldFW; } set { isHoldFW = value; } }
        /// <summary>
        /// 상위에서 랏 검색 명령 받았을 때 화면 갱신 예외 처리 
        /// cbDestination에서는 실제 클릭이 된 상황이 아니면 파라미터 변경하지 않음
        /// </summary>
        private bool isHoldFW = false;

        public FormPreCompDB(object parent)
        {
            InitializeComponent();

            this.parent = parent;

            dbCommTimer = new System.Windows.Forms.Timer();
            dbCommTimer.Interval = 1000;
            dbCommTimer.Tick += new EventHandler(timer_DispDBConn);

            dbSearchProgressTimer = new System.Windows.Forms.Timer();
            dbSearchProgressTimer.Interval = 200;
            dbSearchProgressTimer.Tick += new EventHandler(timer_DbSearch);

            dbLoadingTime = new Stopwatch();

            this.FormClosing += Form_Closing;

            _LangType = 0;

            CreateListViewData();

            initBCNOListView();
            initPTRYLPListView();
            initPTRY0PListView();
            initPTRY0P_TODAYListView();
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
                DestConfig config = null;
                config = this._preCompDB.DbDestConfig;

                dbCommTimer.Start();
                if (dbConn != null) dbConn.OnDbConnect += OnDbConnect;
                if (this.IsDataBaseChanged == true)
                {
                    tbMKCDModelName.Text = PreCompDB?.MKCD_Param.Name;
                    tbLotName.Text = PreCompDB?.SearchLotName;
                    makeAllListViewData();
                    updateLotChangeResult();
                }
            }
            else
            {
                dbCommTimer.Stop();
                if (dbConn != null) dbConn.OnDbConnect -= OnDbConnect;
            }
        }

        private void displayUI()
        {
            
        }

        private void displayMarkingOption()
        {
            Option option = null;
            DestConfig config = null;
            option = PreCompDB?.DbOption;
            config = PreCompDB?.DbDestConfig;

            if (option == null) return;

            this.tbLotName.Text = option.lotName;
        }

        private void displaySearchTime()
        {
            Option dbOption = null;
            dbOption = PreCompDB?.DbOption;
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
                    listViewPTRY0P.Items.Clear();
                    listViewPTRY0P_TODAY.Items.Clear();
                    listViewINSPDAT.Items.Clear();
                    listViewFAULTDAT.Items.Clear();
                }));
            }
            else
            {
                listViewBCNO.Items.Clear();
                listViewPTRYLP.Items.Clear();
                listViewPTRY0P.Items.Clear();
                listViewPTRY0P_TODAY.Items.Clear();
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
                    displayPTRY0PListView();
                    displayPTRY0P_TodayListView();
                    displayINSPDATListView();
                    displayFAULTDATListView();

                    tbMKCDModelName.Text = _preCompDB.MKCD_Model.Name;
                }));
            }
            else
            {
                displayBCNOListView();
                displayPTRYLPListView();
                displayPTRY0PListView();
                displayPTRY0P_TodayListView();
                displayINSPDATListView();
                displayFAULTDATListView();

                tbMKCDModelName.Text = _preCompDB.MKCD_Model.Name;
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
            MkFltDatumList fltdat = PreCompDB.FaultData?.MarkData.Data;

            if (fltdat == null) return;

            int total = fltdat.Count;
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


        private void makeBCNOListData()
        {
            try
            {
                if (PreCompDB.DbOption.isLoadCSV == false)
                {
                    int count = System.Enum.GetValues(typeof(eFCD)).Length;
                    List<INSPDATList>[] data = PreCompDB._DbResult.INSPDAT;

                    // 리스트 초기화는 따로 불러서 처리

                    bool useSplit = PreCompDB.DbOption.searchOP.useSplit;
                    string title = PreCompDB.DbOption.searchOP.Title;
                    eCSV_TYPE type = PreCompDB.DbDestConfig.CSVType;

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
                                    if (i == (int)eFCD.TG) itemBuf.items[1] = this.PreCompDB.DbOption.lotName;
                                    else itemBuf.items[1] = tmpData.BCNO;
                                }
                                else
                                {
                                    //itemBuf.items[1] = tmpData.LOTNO;
                                    itemBuf.items[1] = tmpData.Y0KLOT;
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
                else
                {
                    BCNO_LV_Data.Data.Clear();
                    int index = 0;
                    foreach (var item in PreCompDB._CSVLoadInfo)
                    {
                        DBListViewBuf itemBuf = new DBListViewBuf(4);
                        itemBuf.items[0] = index.ToString();
                        itemBuf.items[1] = item.LotNo;
                        itemBuf.items[2] = item.BadCnt.ToString();
                        itemBuf.items[3] = $"{item.DefectPerM:F3}";
                        BCNO_LV_Data.Data.Add(itemBuf);
                        index++;
                    }
                }
            }
            finally
            {

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

        /// <summary>
        /// 확인 필요함
        /// </summary>
        private void makePTRYLPListViewData()
        {
            PTRYLP_LV_Data.Data.Clear();
            foreach (PTRYLPdata data in PreCompDB._DbResult.PTRLYP.Data)
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

        #region PTRY0P List
        private void initPTRY0PListView()
        {
            listViewPTRY0P.View = View.Details;
            listViewPTRY0P.FullRowSelect = true;
            for (int i = 0; i < PTRY0PHeader.Length; i++)
                listViewPTRY0P.Columns.Add(PTRY0PHeader[i], listPTRY0PWidth[i]);
        }

        private void makePTRY0PListViewData()
        {
            PTRY0P_LV_Data.Data.Clear();
            PTRY0PList[] tmpData = null;
            tmpData = PreCompDB._DbResult.PTRY0P;

            int cnt = tmpData.Length;
            for (int i = 0; i < cnt; i++)
            {
                foreach (PTRY0PData data in tmpData[i].Data)
                {
                    DBListViewBuf bufData = new DBListViewBuf(PTRY0PHeader.Length);
                    bufData.items[0] = data.Y0ZKNM;
                    bufData.items[1] = data.Y0KLOT;
                    bufData.items[2] = data.LNCD;
                    bufData.items[3] = data.Y0LNSN;
                    bufData.items[4] = data.Y0KKOL;
                    bufData.items[5] = data.Y0KSOL;
                    PTRY0P_LV_Data.Data.Add(bufData);
                }
            }
        }

        private void displayPTRY0PListView()
        {
            if (PTRY0P_LV_Data.Data == null) return;
            try
            {
                listViewPTRY0P.BeginUpdate();
                listViewPTRY0P.Items.Clear();

                foreach (DBListViewBuf data in PTRY0P_LV_Data.Data)
                {
                    ListViewItem item = new ListViewItem(data.items[0]);
                    for(int i=1; i< data.items.Length; i++)
                        item.SubItems.Add(data.items[i]);
                    listViewPTRY0P.Items.Add(item);
                }
            }
            finally
            {
                listViewPTRY0P.EndUpdate();
            }
        }

        private void updatePTRY0PListViewLanguage()
        {
            if (listViewPTRY0P.Columns.Count > 0)
            {
                listViewPTRY0P.Columns[0].Text = Language.listPTRY0P_0;
                listViewPTRY0P.Columns[1].Text = Language.listPTRY0P_1;
                listViewPTRY0P.Columns[2].Text = Language.listPTRY0P_2;
                listViewPTRY0P.Columns[3].Text = Language.listPTRY0P_3;
                listViewPTRY0P.Columns[4].Text = Language.listPTRY0P_4;
            }
        }
        #endregion

        #region MRKCTLMST List
        private void initPTRY0P_TODAYListView()
        {
            listViewPTRY0P_TODAY.View = View.Details;
            listViewPTRY0P_TODAY.FullRowSelect = true;
            listViewPTRY0P_TODAY.Columns.Clear();
            for (int i = 0; i < PTLYOP_TodayHeader.Length; i++)
                listViewPTRY0P_TODAY.Columns.Add(PTLYOP_TodayHeader[i], listPTLYOP_TodayWidth[i]);
        }

        private void makePTRY0P_TODAYListViewData()
        {
            PTRY0P_TODAY_LV.Data.Clear();

            if (PreCompDB == null)
                return;
            
            foreach (PTRY0PData data in PreCompDB.PTRY0PList_Data.Data)
            {
                DBListViewBuf bufData = new DBListViewBuf(5);
                bufData.items[0] = data.Y0KLOT;
                bufData.items[1] = data.LNCD;
                bufData.items[2] = data.Y0ZKNM;
                bufData.items[3] = data.Y0KKOL;
                bufData.items[4] = data.Y0KSOL;
                PTRY0P_TODAY_LV.Data.Add(bufData);
            }
        }

        /// <summary>
        /// DB 탐색에서 얻어온 마킹컨트롤마스터 데이터 표시
        /// 데이터는 makeMRKCTLMSTListViewData()에서 생성하여 
        /// 현재 함수에서는 표시만 함
        /// </summary>
        private void displayPTRY0P_TodayListView()
        {
            if (PTRY0P_TODAY_LV.Data == null) return;
            try
            {
                listViewPTRY0P_TODAY.BeginUpdate();
                listViewPTRY0P_TODAY.Items.Clear();

                int dataSize = 0;
                dataSize = 5;

                if (dataSize > 0)
                {
                    foreach (DBListViewBuf data in PTRY0P_TODAY_LV.Data)
                    {
                        ListViewItem item = new ListViewItem(data.items[0]);
                        for (int i = 1; i < dataSize; i++)
                            item.SubItems.Add(data.items[i]);
                        listViewPTRY0P_TODAY.Items.Add(item);
                    }
                }
            }
            finally
            {
                listViewPTRY0P_TODAY.EndUpdate();
            }
        }

        private void updatePTRY0P_TODAYListViewLanguage()
        {
            if (listViewPTRY0P_TODAY.Columns.Count > 0)
            {
                listViewPTRY0P_TODAY.Columns[0].Text = Language.listTodayPTLY0P_0;
                listViewPTRY0P_TODAY.Columns[1].Text = Language.listTodayPTLY0P_1;
                listViewPTRY0P_TODAY.Columns[2].Text = Language.listTodayPTLY0P_2;
                listViewPTRY0P_TODAY.Columns[3].Text = Language.listTodayPTLY0P_3;
                listViewPTRY0P_TODAY.Columns[4].Text = Language.listTodayPTLY0P_4;
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
            INSPDAT_LV_Data.Data.Clear();
            try
            {

                CSV_DEFECT_HEADER header = null;
                header = _preCompDB._CsvDefectHeader;

                if (header == null) return;

                DBListViewBuf bufData = new DBListViewBuf(INSPDATHeader.Length);
                bufData.items[0] = "";
                bufData.items[1] = "";
                bufData.items[2] = header.lotNo;
                bufData.items[3] = header.rollNo;
                bufData.items[4] = header.bcrInfo;
                bufData.items[5] = header.startY;
                bufData.items[6] = header.endY;
                bufData.items[7] = "";
                bufData.items[8] = header.rollSY;
                bufData.items[9] = header.rollEY;
                bufData.items[10] = header.rollY;
                INSPDAT_LV_Data.Data.Add(bufData);
            }
            finally
            {

            }
        }

        private void makeINSPDATListView()
        {
            List<INSPDATList>[] inspData = PreCompDB._DbResult.INSPDAT; 

            if (inspData == null) return;

            try
            {
                INSPDAT_LV_Data.Data.Clear();
                foreach (List<INSPDATList> data in inspData)
                {
                    if (data == null) continue;
                    foreach (INSPDATList items in data)
                    {
                        foreach (INSPDATData item in items.Data)
                        {
                            DBListViewBuf bufData = new DBListViewBuf(INSPDATHeader.Length);

                            bufData.items[0] = item.CTLNO;
                            bufData.items[1] = item.HINMEI;
                            bufData.items[2] = item.LOTNO;
                            bufData.items[3] = item.BCNO;
                            bufData.items[4] = item.STRDT;
                            bufData.items[5] = item.STRTM;
                            bufData.items[6] = item.ENDDT;
                            bufData.items[7] = item.ENDTM;
                            bufData.items[8] = $"{item.Width:F3}";
                            bufData.items[9] = $"{item.Length:F3}";
                            bufData.items[10] = $"{item.YPosStart:F3}";
                            bufData.items[11] = $"{item.YPosEnd:F3}";

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
                foreach (var data in INSPDAT_LV_Data.Data)
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
                    listItem.SubItems.Add(data.items[9]);
                    listItem.SubItems.Add(data.items[10]);
                    listItem.SubItems.Add(data.items[11]);
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

                MkFltDatumList tmpData = _preCompDB.FaultData.MarkData.Data;
                DestConfig config = _preCompDB.DbDestConfig;

                MarkingFaultDatum data = null;

                int mclass = 0;
                eCSV_TYPE tmpType = config.CSVType;

                DestConfigUnit destUnit = config.SelDestUnit;

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
            ResetListViewData();
            this.clearAllListView();
            this.initFaultPage(PreCompDB.FaultData.MarkData.Data.Count);
            makeBCNOListData();
            makePTRYLPListViewData();
            makePTRY0PListViewData();
            makePTRY0P_TODAYListViewData();
            makeINSPDATListView();
            displayBCNOListView();
            displayPTRYLPListView();
            displayPTRY0PListView();
            displayPTRY0P_TodayListView();
            displayINSPDATListView();
            displayFAULTDATListView();

            if (this.PreCompDB.DbOption.dbWhen == eDbIdWhen.Now)
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
        #endregion

        #region Form
        private void btnSearchDB_Click(object sender, EventArgs e)
        {
            if (this.thread != null && this.thread.IsAlive == true)
            {
                MessageBox.Show(Language.ProgramIsSearchingDB);
                return;
            }

            OracleDbConnection conn = null;
            conn = PreCompDB.Conn;

            if (conn.IsConnected() == false)
            {
                MessageBox.Show(Language.PleaseLoginToTheDB);
                return;
            }

            if (tbLotName.Text.Length == 0)
            {
                MessageBox.Show(Language.PleaseInsertLotNumber);
                return;
            }
            if (tbLotName.Text.Length < Global.LotNameLength)
            {
                MessageBox.Show(Language.PleaseInsertTenDigitsOfLotNumber);
                return;
            }

            // 화면 데이터 적용
            updateUIOptionToDBOption();
            this.UpdateEndEvent = false;

            // Thread 처리 필요
            Process.SearchLotData(PreCompDB.DbOption.dbWhen, tbLotName.Text, tbMKCDModelName.Text);
        }

        public bool IsSearchDefect()
        {
            if (this.thread != null && this.thread.IsAlive == true)
                return true;

            return false;
        }

        public void ResetDBData()
        {
            if (InvokeRequired == true)
            {
                this.Invoke(new MethodInvoker(delegate ()
                {
                    // 출하처 옵션 표시 Flag 리셋
                    isHoldFW = false;

                    // ListView 초기화
                    this.clearAllListView();
                    this.initFaultPage();
                    this.ResetListViewData();
                    tbLotName.Text = "";

                    PreCompDB.ResetDataAll();

                    if (PreCompDB.DbOption.dbWhen == eDbIdWhen.Now)
                    {
                        if (OnEndJob != null) OnEndJob((int)eEventReport.eResetDataNow);
                    }
                    else
                    {
                        if (OnEndJob != null) OnEndJob((int)eEventReport.eResetDataNext);
                    }

                    PreCompDB.DbOption.isLoadCSV = false;

                    // Update dbconfig from FWPlace
                    displayMarkingOption();
                    displaySearchTime();
                }));
            }
            else
            {
                // 출하처 옵션 표시 Flag 리셋
                isHoldFW = false;

                // ListView 초기화
                this.clearAllListView();
                this.initFaultPage();
                this.ResetListViewData();

                tbLotName.Text = "";

                if (PreCompDB != null)
                {
                    PreCompDB.ResetDataAll();

                    if (PreCompDB.DbOption.dbWhen == eDbIdWhen.Now)
                        if (OnEndJob != null) OnEndJob((int)eEventReport.eResetDataNow);
                        else
                        if (OnEndJob != null) OnEndJob((int)eEventReport.eResetDataNext);

                    PreCompDB.DbOption.isLoadCSV = false;
                }

                // Update dbconfig from FWPlace
                displayMarkingOption();
                displaySearchTime();
            }
        }

        private void updateUIOptionToDBOption()
        {
            this.PreCompDB.DbOption.lotName = (string)tbLotName.Text.Clone();
            this.PreCompDB.MKCD_Param.Set(tbMKCDModelName.Text);
        }

        private void btnReset_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show(Language.ResetAllData, "Reset Fault Data", MessageBoxButtons.YesNo) == DialogResult.No)
                return;

            ResetDBData();
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

            this.dbConn.LoginInfo = _preCompDB.Conn.LoginInfo;

            if (formLogin == null) formLogin = new FormDbLoginData(this.dbConn);

            formLogin.Conn = this.dbConn;
            formLogin.ShowDialog();

            if (this.dbConn.IsDBConnected == true)
            {
                _preCompDB.Conn.LoginInfo = this.dbConn.LoginInfo;
            }
        }

        private void btnShowSkipParam_Click(object sender, EventArgs e)
        {
            if (IsSearchDefect() == true)
            {
                MessageBox.Show("Now program is searching DB");
                return;
            }

        }

        /// <summary>
        /// 에디트 창에 데이터 입력 후 화면 표시
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void tbFaultPage_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {

                MkFltDatumList data = _preCompDB.FaultData.MarkData.Data;

                if (Int32.TryParse(tbFaultPage.Text, out int intput) == true)
                {
                    int total = data.Count;
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

        private void btnUpdateMarkingData_Click(object sender, EventArgs e)
        {
            eDbIdWhen when = eDbIdWhen.Now;
            when = PreCompDB.DbOption.dbWhen;


            if (OnEndJob == null) return;

            if (when == eDbIdWhen.Now)
                OnEndJob((int)eEventReport.eUpdateDataNow);
            else
                OnEndJob((int)eEventReport.eUpdateDataNext);
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
            Option dbOption = null;
            dbOption = PreCompDB.DbOption;
            dbOption.lotName = this.tbLotName.Text;
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
            NittoDBProgress progress = null;
            if (this._preCompDB == null) return;
            progress = _preCompDB.DB_Progress;

            if (progress == null) return;

            StringBuilder sb = new StringBuilder();

            if (progress.IsError == true)
            {
                sb.Append($"Error is occured : Error Step [{((eNittoDBProgress)progress.ErrorStep).ToString()}]");
                lblDownloadResult.Text = sb.ToString();
                return;
            }

            int maxIdx = System.Enum.GetValues(typeof(eNittoDBProgress)).Length;

            // 현재 진항하고 있는 마지막 스텝을 확인한다. 
            int finalStep = 0;
            for (int i = 0; i < maxIdx; i++)
            {
                if (progress._Progress[i].IsComplete() == true || progress._Progress[i].IsSkip == true)
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
                        if (progress._Progress[i].IsComplete() == true)
                            sb.Append("PTRYLP Complete => ");
                        else
                        {
                            sb.Append($"PTRYLP is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.XOFSMST:
                        if (progress._Progress[i].IsComplete() == true)
                            sb.Append("XOFSMST Complete => ");
                        else
                        {
                            sb.Append($"XOFSMST is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;


                    case eNittoDBProgress.AREADEL:
                        if (progress._Progress[i].IsComplete() == true)
                            sb.Append("AREADEL Complete => ");
                        else
                        {
                            sb.Append($"AREADEL is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.PTRY0P:
                        if (progress._Progress[i].IsComplete() == true)
                            sb.Append("PTRY0P Complete => ");
                        else
                        {
                            sb.Append($"PTRY0P is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.MRKCTLMST_ES:
                        if (progress._Progress[i].IsSkip == true)
                            sb.Append("MRKCTLMST_ES Skip => ");
                        else if (progress._Progress[i].IsComplete() == true)
                            sb.Append("MRKCTLMST_ES Complete => ");
                        else
                        {
                            sb.Append($"MRKCTLMST_ES is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.MRKCTLMST_TG:
                        if (progress._Progress[i].IsSkip == true)
                            sb.Append("MRKCTLMST_TG Skip => ");
                        else if (progress._Progress[i].IsComplete() == true)
                            sb.Append("MRKCTLMST_TG Complete => ");
                        else
                        {
                            sb.Append($"MRKCTLMST_TG is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.MRKCTLMST_ETC:
                        if (progress._Progress[i].IsSkip == true)
                            sb.Append("MRKCTLMST_ETC Skip => ");
                        else if (progress._Progress[i].IsComplete() == true)
                            sb.Append("MRKCTLMST_ETC Complete => ");
                        else
                        {
                            sb.Append($"MRKCTLMST_ETC is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.INSPDAT_ES:
                        if (progress._Progress[i].IsSkip == true)
                            sb.Append("INSPDAT_ES Skip => ");
                        else if (progress._Progress[i].IsComplete() == true)
                            sb.Append("INSPDAT_ES Complete => ");
                        else
                        {
                            sb.Append($"INSPDAT_ESis processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.INSPDAT_TG:
                        if (progress._Progress[i].IsSkip == true)
                            sb.Append("INSPDAT_TG Skip => ");
                        else if (progress._Progress[i].IsComplete() == true)
                            sb.Append("INSPDAT_TG Complete => ");
                        else
                        {
                            sb.Append($"INSPDAT_TG is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.INSPDAT_ETC:
                        if (progress._Progress[i].IsSkip == true)
                            sb.Append("INSPDAT_ETC Skip => ");
                        else if (progress._Progress[i].IsComplete() == true)
                            sb.Append("INSPDAT_ETC Complete => ");
                        else
                        {
                            sb.Append($"INSPDAT_ETC is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.FAULTDAT_ES:
                        if (progress._Progress[i].IsSkip == true)
                            sb.Append("FAULTDAT_ES Skip => ");
                        else if (progress._Progress[i].IsComplete() == true)
                            sb.Append("FAULTDAT_ES Complete => ");
                        else
                        {
                            sb.Append($"FAULTDAT_ES is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.FAULTDAT_TG:
                        if (progress._Progress[i].IsSkip == true)
                            sb.Append("FAULTDAT_TG Skip => ");
                        else if (progress._Progress[i].IsComplete() == true)
                            sb.Append("FAULTDAT_TG Complete => ");
                        else
                        {
                            sb.Append($"FAULTDAT_TG is processing");
                            lblDownloadResult.Text = sb.ToString();
                            return;
                        }
                        break;

                    case eNittoDBProgress.FAULTDAT_ETC:
                        if (progress._Progress[i].IsSkip == true)
                            sb.Append("FAULTDAT_ETC Skip => ");
                        else if (progress._Progress[i].IsComplete() == true)
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


        #region Language Update
        public void setLangType()
        {
            switch (_LangType)
            {
                case 0: // 한국어
                    Thread.CurrentThread.CurrentUICulture = CultureInfo.GetCultureInfo("ko-KR");
                    break;
                case 1: // 영어
                    Thread.CurrentThread.CurrentUICulture = CultureInfo.GetCultureInfo("en-US");
                    break;

                case 2: // 중국어 번체
                    Thread.CurrentThread.CurrentUICulture = CultureInfo.GetCultureInfo("zh-TW");
                    break;
                case 3: // 중국어 간체
                    Thread.CurrentThread.CurrentUICulture = CultureInfo.GetCultureInfo("zh-CN");
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
            btnSearchDB.Text = Language.Search;
            btnUpdateMarkingData.Text = Language.UpdateData;
            btnReset.Text = Language.Reset;
            btnShowSkipParam.Text = Language.ShowSkipParam;
            btnPrevFaultPage.Text = Language.PrevFaultPage;
            btnNextFaultPage.Text = Language.NextFaultPage;

            updateBCNOListViewLanguage();
            updatePTRYLPListViewLanguage();
            updatePTRY0PListViewLanguage();
            updatePTRY0P_TODAYListViewLanguage();
            updateINSPDATListViewLanguage();
            updateFAULTDATListViewLanguage();
            this.ResumeLayout();
        }
        #endregion Language Update

        #region 검색 결과 업데이트
        public void OnUpdateAvailableLot()
        {
            if (PreCompDB == null)
            {
                // 화면이 생성되면 보이게 한다.
                this.IsDataBaseChanged = true;
                return;
            }
            // 현재 선택된 Lot Name을 업데이트 한다. 
            if (this.tbLotName.InvokeRequired == true)
                this.Invoke(new MethodInvoker(delegate ()
                {
                    tbLotName.Text = PreCompDB.SearchLotName;
                }));
            else
                tbLotName.Text = PreCompDB.SearchLotName;

            // List View 업데이트 데이터 생성
            initFaultPage(PreCompDB.FaultData.MarkData.Data.Count);
            this.makeAllListViewData();
            this.displayAllListView();
        }

        public void OnUpdateDailyLotInfo()
        {
            this.makePTRY0P_TODAYListViewData();
            if (this.InvokeRequired == true)
                this.Invoke(new MethodInvoker(delegate ()
                {
                    displayPTRY0P_TodayListView();
                }));
            else
                displayPTRY0P_TodayListView();

        }
        #endregion

        private void btnSearchPTRY0P_Today_Click(object sender, EventArgs e)
        {
            OracleDbConnection conn = null;
            conn = PreCompDB.Conn;

            if (conn.IsConnected() == false)
            {
                MessageBox.Show(Language.PleaseLoginToTheDB);
                return;
            }

            // 화면 데이터 적용
            updateUIOptionToDBOption();
            this.UpdateEndEvent = false;

            // 검색 데이터 처리
            Process.SearchDailyLot();
        }
       
        private void btnMKCDModel_Click(object sender, EventArgs e)
        {
            using(FormMKCDModel form = new FormMKCDModel())
            {
                form.DbConn = this.DBConn;
                form.ShowDialog();
            }
        }

        public void OnUpdateMKCD_ModelName()
        {
            string name = PreCompDB?.MKCD_Param.Name;
            if (this.InvokeRequired == true)
            {
                this.Invoke(new MethodInvoker(delegate ()
                {
                    tbMKCDModelName.Text = name;
                }));
            }
            else
            {
                tbMKCDModelName.Text = name;
            }
        }
    }
}
