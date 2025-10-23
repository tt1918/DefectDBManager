using Coss.Controls;
using DefectDBManager;
using DefectDBManager.Preproc;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using static DefectDBManager.LogDB;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.Button;

namespace MarkCompare
{
    public partial class FormRollMap : Form
    {
        public enum eViewType
        { 
            Normal =0,
            CSV,
            DB
        }


        #region Param
        private IPreprocLot _crtLot = null;
        private PreprocLNCDInfo _info = null;
        private CSVProcParam _csvParam = null;
        private LotSelProcParam _selParam = null;
        private PreprocItem _procItem = null;
        private eViewType _dataType = eViewType.Normal;
        private Font rollmapDefectFont = new System.Drawing.Font("Segoe UI", 14F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));

        private Dictionary<int, string> dicProcInfo = new Dictionary<int, string>();


        string[] _key = null;
        bool _isSplit = false;
        #endregion

        #region RollMap
        public RollmapMouseClickedHandler MouseClicked;
        public RollmapDefectClickedHandler DefectClicked;
        public RollmapViewAreaChangedHandler ViewAreaChanged;

        public FormRollMap()
        {
            InitializeComponent();
        }

        public void InitRollMapAndSummary()
        {
            initRollMap();
        }

        private void FormRollMap_FormClosing(object sender, FormClosingEventArgs e)
        {
            Rollmap.Dispose();
        }
        #endregion

        #region RollMap Control
        private void initRollMap()
        {
            Rollmap.OffsetX = 0;
            Rollmap.OffsetY = 0;
            Rollmap.WholeWidth = 1800;
            Rollmap.WholeHeight = 2000000;
            Rollmap.Init(false, false, false, false);
            Rollmap.UseVscroll = true;

            Rollmap.AutomapViewOffsetY = 0;

            Rollmap.ShowOsDsPosX = false;
            //표시안해서 상관없음
            Rollmap.OsDsPosXbyEdge = false;

            Rollmap.Frames.OsdsOpacity = 0.5f;

            Rollmap.UseColor2 = false;
            Rollmap.ShowPeriod = true;

            this.Rollmap.MouseDoubleClicked += OnRollMapMouseDoubleClicked;
            this.Rollmap.MouseClicked += OnRollMapMouseClicked;
            this.Rollmap.ViewAreaChanged += OnViewAreaChanged;
        }

        private void OnRollMapMouseDoubleClicked(Point real, Point gdi, MouseButtons btn)
        {

        }

        private void OnRollMapMouseClicked(Point real, Point gdi, MouseButtons btn)
        {
            MouseClicked?.Invoke(real, gdi, btn);
            RollmapDefect defect;

            double minDist = double.MaxValue;
            double dist;
            int idx = 0, minIdx=-1;
            if (_crtLot == null) return;
            if(_crtLot.MarkCompList.Data==null) return;
            foreach (var item in _crtLot.MarkCompList.Data)
            {
                dist = Math.Sqrt(Math.Pow(item.Base.XPOS_M - real.X, 2.0)+ Math.Pow(item.Base.OFFSET - real.Y, 2.0));
                if(dist<minDist)
                {
                    minDist = dist;
                    minIdx = idx;
                }
                idx++;
            }

            var defect1 = _crtLot.MarkCompList.Data.Select((item, index) => new
            {
                Index = index,
                Dist = Math.Sqrt(
                    Math.Pow(item.Base.XPOS_M - real.X, 2.0) +
                    Math.Pow(item.Base.OFFSET - real.Y, 2.0)),
                Defect = item
            })
            .OrderBy(x => x.Dist).First().Defect;

            if (minIdx >= 0 && defect1 != null)
            {
                lbxRefInfo.BeginUpdate();
                lbxRefInfo.Items.Clear();
                lbxRefInfo.Items.Add($"X:{defect1.Base.XPOS_M:F1}");
                lbxRefInfo.Items.Add($"Y:{defect1.Base.OFFSET:F1}");
                lbxRefInfo.Items.Add($"Size:{defect1.Base.SIZE:F3}");
                lbxRefInfo.Items.Add($"FAULTID:{defect1.Base.FAULTID:F3}");

                lbxRefInfo.EndUpdate();

                if (_isSplit)
                {
                    foreach (var def in defect1.Comp1)
                    {
                        if (def.Key.Item1 == _key[0] && def.Key.Item2 == _key[1])
                        {
                            lbxCompItem.BeginUpdate();
                            lbxCompItem.Items.Clear();
                            int cnt = 0;
                            foreach (var d in def.Value)
                            {
                                if(cnt==0) lbxCompItem.Items.Add("== Reference ==");
                                else lbxCompItem.Items.Add($"== Compare {cnt} ==");
                                int subCnt = 0;
                                foreach (var item in d)
                                {
                                    lbxCompItem.Items.Add($"Index:{subCnt}");
                                    lbxCompItem.Items.Add($"X:{item.XPOS_M:F1}");
                                    lbxCompItem.Items.Add($"Y:{item.OFFSET:F1}");
                                    lbxCompItem.Items.Add($"Size:{item.SIZE:F3}");
                                    lbxCompItem.Items.Add($"FAULTID:{item.FAULTID:F3}");
                                    lbxCompItem.Items.Add("");
                                    subCnt++;
                                }
                                cnt++;
                            }
                            lbxCompItem.EndUpdate();
                        }
                    }
                }
                else
                {
                    foreach (var def in defect1.Comp1)
                    {
                        if (def.Key.Item1 == _key[0])
                        {
                            lbxCompItem.BeginUpdate();
                            lbxCompItem.Items.Clear();
                            int cnt = 0;
                            foreach (var d in def.Value)
                            {
                                if (cnt == 0) lbxCompItem.Items.Add("== Reference ==");
                                else lbxCompItem.Items.Add($"== Compare {cnt} ==");
                                int subCnt = 0;
                                foreach (var item in d)
                                {
                                    lbxCompItem.Items.Add($"Index:{subCnt}");
                                    lbxCompItem.Items.Add($"X:{item.XPOS_M:F1}");
                                    lbxCompItem.Items.Add($"Y:{item.OFFSET:F1}");
                                    lbxCompItem.Items.Add($"Size:{item.SIZE:F3}");
                                    lbxCompItem.Items.Add($"FAULTID:{item.FAULTID:F3}");
                                    lbxCompItem.Items.Add("");
                                    subCnt++;
                                }
                                cnt++;
                            }
                            lbxCompItem.EndUpdate();
                        }
                    }
                }
            }

            //var defects = Rollmap.GetNearestDefects(real, gdi);
            //foreach (var d in defects)
            //    DefectClicked?.Invoke(real, gdi, d.Key, d.Value);

                //defect = defects.FirstOrDefault(e => e.Value.Id != -1).Value;
        }

        private void OnViewAreaChanged(object sender, MapViewOptionArgs viewOption, Rectangle rect)
        {
            ViewAreaChanged?.Invoke(this, viewOption, rect);
        }

        #endregion RollMap

        #region Radio Control 
        private void radioButton_CheckedChanged(object sender, EventArgs e)
        {
            System.Windows.Forms.RadioButton rb = sender as System.Windows.Forms.RadioButton;
            if (rb == null) return; //중복호출 방지

            if (rbScale1.Checked)       this.Rollmap.MapViewOption(MapViewOptionArgs.tagMapViewRange.View10M);
            else if (rbScale2.Checked)  this.Rollmap.MapViewOption(MapViewOptionArgs.tagMapViewRange.View100M);
            else if (rbScale3.Checked)  this.Rollmap.MapViewOption(MapViewOptionArgs.tagMapViewRange.View500M);
            else if (rbScale4.Checked)  this.Rollmap.MapViewOption(MapViewOptionArgs.tagMapViewRange.View1000M);
        }
        #endregion

        #region Event
        private void UpdateCbProcess()
        {
            cbProcess.Items.Clear();
            dicProcInfo.Clear();

            for (int i = 0; i < _procItem.Compare.Count; i++)
            {
                if (_procItem.Compare[i].IsSplitCTLNO==false)
                {
                    string itemName = $"{i + 1}:{_procItem.Reference.LNCD}_{_procItem.Compare[i].LNCD}";
                    cbProcess.Items.Add(itemName);
                }
                else
                {
                    int size = _crtLot.MarkCompList.CTLNO[i].Count;
                    if(size==0)
                    {
                        string itemName = $"{i + 1}:{_procItem.Reference.LNCD}_{_procItem.Compare[i].LNCD}";
                        cbProcess.Items.Add(itemName);
                    }
                    else
                    {
                        for (int j = 0; j < size; j++)
                        {
                            string itemName = $"{i + 1}-{j + 1}:{_procItem.Reference.LNCD}_{_procItem.Compare[i].LNCD}_{_crtLot.MarkCompList.CTLNO[i][j]}";
                            cbProcess.Items.Add(itemName);
                        }
                    }
                }
            }
            cbProcess.SelectedIndex = 0;
        }
        private void UpdateCbProcessCSV()
        {
            cbProcess.Items.Clear();
            dicProcInfo.Clear();
            foreach (var item in _crtLot.FaultData.PreMarkData) 
            {
                for(int i=0; i<item.Count; i++)
                {
                    string itemName = $"{i + 1}:{_crtLot.FaultData.MarkData.LNCD}_{item[i].LNCD}";
                    cbProcess.Items.Add(itemName);
                }
            }
            cbProcess.SelectedIndex = 0;
        }

        private void UpdateCbProcessDB()
        {
            cbProcess.Items.Clear();
            dicProcInfo.Clear();

            List<string> listLNCD = new List<string>();
            foreach (var item in _crtLot.FaultData.PreMarkData)
            {
                for (int i = 0; i < item.Count; i++)
                {
                    if(listLNCD.Contains( item[i].LNCD)==false)
                        listLNCD.Add(item[i].LNCD);
                }
            }

            string refLNCD = _crtLot.FaultData.MarkData.LNCD;

            for (int i=0; i<listLNCD.Count; i++)
            {
                if(_selParam.UseSplit==false)
                {
                    string itemName = $"{i + 1}:{refLNCD}_{listLNCD[i]}";
                    cbProcess.Items.Add(itemName);
                }
                else
                {
                    int size = _crtLot.MarkCompList.CTLNO[i].Count;
                    if (size == 0)
                    {
                        string itemName = $"{i + 1}:{refLNCD}_{listLNCD[i]}";
                        cbProcess.Items.Add(itemName);
                    }
                    else
                    {
                        for (int j = 0; j < size; j++)
                        {
                            string itemName = $"{i + 1}-{j + 1}:{refLNCD}_{listLNCD[i]}_{_crtLot.MarkCompList.CTLNO[i][j]}";
                            cbProcess.Items.Add(itemName);
                        }
                    }
                }
            }

            cbProcess.SelectedIndex = 0;
        }

        public void OnUpdateLotInfo(IPreprocLot lot, PreprocLNCDInfo info, PreprocItem procItem)
        {
            if (lot.FaultData == null) return;
            _crtLot = lot;
            _info = info;
            _procItem = procItem;
            _csvParam = null;
            _selParam = null;
            _dataType = eViewType.Normal;

            lblLotName.Text = _crtLot.LotName;

            if (cbProcess.InvokeRequired)   cbProcess.BeginInvoke(new Action(UpdateCbProcess));
            else                            UpdateCbProcess();

            // Rollmap update
            drawMap(_crtLot, _info, _procItem);
        }

        public void OnUpdateLotInfo(IPreprocLot lot, CSVProcParam info)
        {
            if (lot.FaultData == null) return;
            _crtLot = lot;
            _csvParam = info;
            _procItem = null;
            _selParam = null;
            _dataType = eViewType.CSV;

            lblLotName.Text = _crtLot.LotName;

            if (cbProcess.InvokeRequired) cbProcess.BeginInvoke(new Action(UpdateCbProcessCSV));
            else UpdateCbProcessCSV();

            // Rollmap update
            drawMap(_crtLot, _csvParam);
        }

        public void OnUpdateLotInfo(IPreprocLot lot, LotSelProcParam info)
        {
            if (lot.FaultData == null) return;
            _crtLot = lot;
            _selParam = info;
            _procItem = null;
            _csvParam = null;
            _dataType = eViewType.DB;

            lblLotName.Text = _crtLot.LotName;

            if (cbProcess.InvokeRequired) cbProcess.BeginInvoke(new Action(UpdateCbProcessDB));
            else UpdateCbProcessDB();

            // Rollmap update
            drawMap(_crtLot, _selParam);
        }

        private void drawMap(IPreprocLot lot, PreprocLNCDInfo info, PreprocItem procItem)
        {
            int defIdx = 0;
            double maxPosY = 0;
            double judgeY1000 = procItem.Judge.Y * 1000;
            var errorAreas = new List<PrevErrorAreaPosition>();
            var defects = new List<PrevCompareDefect>();
            object lockObj = new object();

            var selectedIndex = cbProcess.SelectedIndex;
            var selectedText = cbProcess.Text;

            string[] string1 = selectedText.Split(':');
            string[] strSel = string1[0].Split('-');
            selectedIndex = int.Parse(strSel[0])-1;

            var isSplit = procItem.Compare[selectedIndex].IsSplitCTLNO;
            string[] key = {"",""};
            if(isSplit==true)
            {
                string[] string2 = string1[1].Split('_');

                if (string2.Length ==3)
                {
                    key[0] = string2[1];
                    key[1] = string2[2];
                }
                else
                    isSplit = false;
            }
            else
            {
                key[0] = procItem.Compare[selectedIndex].LNCD;
            }

            _key = key;
            _isSplit = isSplit;

            double compRangeX = procItem.CompRange[0].MaxXRange;
            double compRangeY = procItem.CompRange[0].MaxYRange;

            int compStep = procItem.CompRange.Count+1;
            int compSize = procItem.Compare.Count;

            foreach (var item in lot.MarkCompList.Data)
            {
                for (int i = 0; i < compStep; i++)
                {
                    if (i != selectedIndex) continue;

                    var localDefects = new List<PrevCompareDefect>();
                    var localErrors = new List<PrevErrorAreaPosition>();

                    double localMaxY = item.Base.OFFSET;
                    int baseCnt = 0, compCnt = 0;
                    double oldX = 0, oldY = 0;

                    int errIdx = -1;

                    localDefects.Add(new PrevCompareDefect(
                        0, item.Base.XPOS_M, item.Base.OFFSET, item.Base.SIZE,
                        0, Interlocked.Increment(ref defIdx), Color.Gray, info.Symbol));

                    oldX = item.Base.XPOS_M;
                    oldY = item.Base.OFFSET;

                    if (isSplit==false)
                    {
                        var compList1 = item.Comp1.Where(kv => kv.Key.Item1 == key[0]).ToList();
                        if (compList1.Count >0)
                        {
                            errIdx = -1;
                            for (int j = 0; j < compStep; j++)
                            {
                                foreach (var cList in compList1)
                                {
                                    bool isSet = false, isSetC = false;
                                    PrevCompareDefect tmpItem = new PrevCompareDefect();
                                    var compList = cList.Value[j];
                                    for (int k = 0; k < cList.Value[j].Count; k++)
                                    {
                                        var comp = cList.Value[j][k];
                                        double posX = comp.XPOS_M;
                                        double posY = comp.OFFSET;

                                        if (j == 0)
                                        {
                                            tmpItem = new PrevCompareDefect(
                                                0, posX, posY, comp.SIZE,
                                                0, Interlocked.Increment(ref defIdx), Color.Yellow, "R");
                                            isSet = true;
                                            baseCnt++;
                                        }
                                        else
                                        {
                                            if (Math.Abs(oldX - posX) > compRangeX || Math.Abs(oldY - posY) > compRangeY)
                                            {
                                                if (isSetC == false)
                                                {
                                                    string symbolE = $"C_{j}";
                                                    tmpItem = new PrevCompareDefect(
                                                    0, posX, posY, comp.SIZE,
                                                    0, Interlocked.Increment(ref defIdx), Color.Red, symbolE);
                                                    isSet = isSetC = true;
                                                }
                                            }
                                            compCnt++;
                                        }

                                        if (isSet) localDefects.Add(tmpItem);

                                        if (posY > localMaxY) localMaxY = posY;
                                    }
                                }

                                // Error area 생성
                                if (((baseCnt > 0 && compCnt <= 0) || (baseCnt <= 0 && compCnt > 0)) && j > 0)
                                    errIdx = baseCnt > 0 ? 0 : j;
                            }

                            // Error area 생성
                            if (errIdx>=0)
                            {
                                var firstItem = compList1
                                                .Select(kv => kv.Value)                               // 배열
                                                .Where(arr => arr != null && arr.Length > errIdx)     // 인덱스 범위 체크
                                                .Select(arr => arr[errIdx])                           // 리스트
                                                .Where(list => list != null && list.Count > 0)        // 비어있지 않은 리스트만
                                                .Select(list => list[0])                              // 리스트의 첫 요소
                                                .FirstOrDefault();                                    // 전체 중 첫 요소

                                if (firstItem != null)
                                {
                                    double x = (int)(firstItem.XPOS_M / procItem.Judge.X) * procItem.Judge.X;
                                    double y = (int)(firstItem.OFFSET / judgeY1000) * judgeY1000;
                                   
                                    var newError = new PrevErrorAreaPosition(x, y, x + procItem.Judge.X, y + judgeY1000);
                                    bool exists = localErrors.Any(e =>
                                        Math.Abs(e.startX - newError.startX) < 0.001 &&
                                        Math.Abs(e.startY - newError.startY) < 0.001 &&
                                        Math.Abs(e.endX - newError.endX) < 0.001 &&
                                        Math.Abs(e.endY - newError.endY) < 0.001
                                    );

                                    if (!exists)
                                        localErrors.Add(newError);
                                }
                            }
                        }

                        lock (lockObj)
                        {
                            defects.AddRange(localDefects);
                            errorAreas.AddRange(localErrors);
                            if (localMaxY > maxPosY) maxPosY = localMaxY;
                        }
                    }
                    else
                    {
                        for (int j = 0; j < item.Comp1[(key[0], key[1])].GetLength(0); j++)
                        {
                            var compList = item.Comp1[(key[0], key[1])][j];
                            PrevCompareDefect tmpItem=new PrevCompareDefect();
                            bool isSet = false, isSetC = false;
                            for (int k = 0; k < compList.Count; k++)
                            {
                                var comp = compList[k];
                                double posX = comp.XPOS_M;
                                double posY = comp.OFFSET;

                                if (j == 0)
                                {
                                    tmpItem = new PrevCompareDefect(
                                        0, posX, posY, comp.SIZE,
                                        0, Interlocked.Increment(ref defIdx), Color.Yellow, "R");
                                    isSet = true;
                                    baseCnt++;
                                }
                                else
                                {
                                    if (Math.Abs(oldX - posX) > compRangeX || Math.Abs(oldY - posY) > compRangeY)
                                    {
                                        if (isSetC == false)
                                        {
                                            string symbolE = $"C_{j}";
                                            tmpItem = new PrevCompareDefect(
                                            0, posX, posY, comp.SIZE,
                                            0, Interlocked.Increment(ref defIdx), Color.Red, symbolE);
                                            isSet = isSetC = true;
                                        }
                                    }
                                    compCnt++;
                                }

                                if (isSet) localDefects.Add(tmpItem);

                                if (posY > localMaxY) localMaxY = posY;
                            }

                            // Error area 생성
                            if (((baseCnt > 0 && compCnt <= 0) || (baseCnt <= 0 && compCnt > 0)) && j > 0)
                                errIdx = baseCnt > 0 ? 0 : j;
                        }


                        // Error area 생성
                        if (errIdx>=0)
                        {
                            foreach (var comp in item.Comp1[(key[0], key[1])][errIdx])
                            {
                                double x = (int)(comp.XPOS_M / procItem.Judge.X) * procItem.Judge.X;
                                double y = (int)(comp.OFFSET / judgeY1000) * judgeY1000;
                                localErrors.Add(new PrevErrorAreaPosition(x, y, x + procItem.Judge.X, y + judgeY1000));
                                break;
                            }
                        }

                        lock (lockObj)
                        {
                            defects.AddRange(localDefects);
                            errorAreas.AddRange(localErrors);
                            if (localMaxY > maxPosY) maxPosY = localMaxY;
                        }
                    }
                }
            }

            foreach (var def in defects)
                Rollmap.AddPrevDefect(def);

            foreach (var area in errorAreas)
                Rollmap.AddPrevErrorArea(area);

            Rollmap.WholeHeight = (int)(maxPosY + 100000);
            Rollmap.RedrawAll();
        }

        private void drawMap(IPreprocLot lot, CSVProcParam param)
        {
            int defIdx = 0;
            double maxPosY = 0;
            double judgeY1000 = param.Judge.Y * 1000;
            var errorAreas = new List<PrevErrorAreaPosition>();
            var defects = new List<PrevCompareDefect>();
            object lockObj = new object();
            
            var selectedIndex = cbProcess.SelectedIndex;
            var selectedText = cbProcess.Text;

            string[] string1 = selectedText.Split(':');
            string[] strSel = string1[0].Split('-');
            string[] strLNCD = string1[1].Split('_');
            selectedIndex = int.Parse(strSel[0]) - 1;

            string[] key = { "", "" };
            key[0] = strLNCD[1];

            _key = key;
            _isSplit = false;

            double compRangeX = param.CompRange[0].MaxXRange;
            double compRangeY = param.CompRange[0].MaxYRange;

            int compStep = param.CompRange.Count + 1;
            int compSize = lot.MarkCompList.CTLNO.Length;

            //            Parallel.ForEach(lot.MarkCompList.Data, item =>
            foreach (var item in lot.MarkCompList.Data)
            {
                var localDefects = new List<PrevCompareDefect>();
                var localErrors = new List<PrevErrorAreaPosition>();

                double localMaxY = item.Base.OFFSET;
                int baseCnt = 0, compCnt = 0;
                double oldX = 0, oldY = 0;

                int errIdx = -1;

                localDefects.Add(new PrevCompareDefect(
                    0, item.Base.XPOS_M, item.Base.OFFSET, item.Base.SIZE,
                    0, Interlocked.Increment(ref defIdx), Color.Gray, "C"));

                oldX = item.Base.XPOS_M;
                oldY = item.Base.OFFSET;

                var compList1 = item.Comp1.Where(kv => kv.Key.Item1 == key[0]).ToList();
                if (compList1.Count > 0)
                {
                    errIdx = -1;
                        
                    foreach (var cList in compList1)
                    {
                        for (int j = 0; j < cList.Value.Length; j++)
                        {
                            if(j<=1)
                            {
                                compRangeX = param.BasicRange.MaxXRange;
                                compRangeY = param.BasicRange.MaxYRange;
                            }
                            else
                            {
                                compRangeX = param.CompRange[j-2].MaxXRange;
                                compRangeY = param.CompRange[j-2].MaxYRange;
                            }

                            bool isSet = false;
                            PrevCompareDefect tmpItem = new PrevCompareDefect();
                            var compList = cList.Value[j];
                            for (int k = 0; k < cList.Value[j].Count; k++)
                            {
                                var comp = cList.Value[j][k];
                                double posX = comp.XPOS_M;
                                double posY = comp.OFFSET;

                                if (j == 0)
                                {
                                    tmpItem = new PrevCompareDefect(
                                        0, posX, posY, comp.SIZE,
                                        0, Interlocked.Increment(ref defIdx), Color.Yellow, "R");
                                    isSet = true;
                                    baseCnt++;
                                }
                                else
                                {
                                    if (Math.Abs(oldX - posX) > compRangeX || Math.Abs(oldY - posY) > compRangeY)
                                    {
                                        string symbolE = $"C_{j}";
                                        tmpItem = new PrevCompareDefect(
                                        0, posX, posY, comp.SIZE,
                                        0, Interlocked.Increment(ref defIdx), Color.Red, symbolE);
                                        isSet = true;
                                        
                                        compCnt++;
                                    }
                                }

                                if (posY > localMaxY) localMaxY = posY;
                            }

                            if (isSet) localDefects.Add(tmpItem);

                            // Error area 생성
                            if (tmpItem.symbol != null && tmpItem.symbol.Contains("C_"))
                            {
                                if (((baseCnt > 0 && compCnt <= 0) || (baseCnt <= 0 && compCnt > 0)) && j > 0)
                                    errIdx = baseCnt > 0 ? 0 : j;
                                else if (baseCnt > 0 && compCnt > 0 && j > 0)
                                    errIdx = j;
                            }
                        }

                        // Error area 생성
                        if (errIdx > 0)
                        {
                            var firstItem = compList1
                                            .Select(kv => kv.Value)                               // 배열
                                            .Where(arr => arr != null && arr.Length > errIdx)     // 인덱스 범위 체크
                                            .Select(arr => arr[errIdx])                           // 리스트
                                            .Where(list => list != null && list.Count > 0)        // 비어있지 않은 리스트만
                                            .Select(list => list[0])                              // 리스트의 첫 요소
                                            .FirstOrDefault();                                    // 전체 중 첫 요소

                            if (firstItem != null)
                            {
                                double x = (int)(firstItem.XPOS_M / param.Judge.X) * param.Judge.X;
                                double y = (int)(firstItem.OFFSET / judgeY1000) * judgeY1000;

                                var newError = new PrevErrorAreaPosition(x, y, x + param.Judge.X, y + judgeY1000);
                                bool exists = localErrors.Any(e =>
                                    Math.Abs(e.startX - newError.startX) < 0.001 &&
                                    Math.Abs(e.startY - newError.startY) < 0.001 &&
                                    Math.Abs(e.endX - newError.endX) < 0.001 &&
                                    Math.Abs(e.endY - newError.endY) < 0.001
                                );

                                if (!exists)
                                    localErrors.Add(newError);
                            }
                        }
                    }

                    
                }

                lock (lockObj)
                {
                    defects.AddRange(localDefects);
                    errorAreas.AddRange(localErrors);
                    if (localMaxY > maxPosY) maxPosY = localMaxY;
                }
            }

            foreach (var def in defects)
                Rollmap.AddPrevDefect(def);

            foreach (var area in errorAreas)
                Rollmap.AddPrevErrorArea(area);

            Rollmap.WholeHeight = (int)(maxPosY + 100000);
            Rollmap.RedrawAll();
        }

        private void drawMap(IPreprocLot lot, LotSelProcParam param)
        {
            try
            {
                int defIdx = 0;
                double maxPosY = 0;
                double judgeY1000 = param.Judge.Y * 1000;
                var errorAreas = new List<PrevErrorAreaPosition>();
                var defects = new List<PrevCompareDefect>();
                object lockObj = new object();

                var selectedIndex = cbProcess.SelectedIndex;
                var selectedText = cbProcess.Text;

                string[] string1 = selectedText.Split(':');
                string[] strSel = string1[0].Split('-');
                selectedIndex = int.Parse(strSel[0]) - 1;

                List<string> listLNCD = new List<string>();
                foreach (var item in _crtLot.FaultData.PreMarkData)
                {
                    for (int i = 0; i < item.Count; i++)
                    {
                        if (listLNCD.Contains(item[i].LNCD) == false)
                            listLNCD.Add(item[i].LNCD);
                    }
                }

                string[] key = { "", "" };
                _isSplit = _selParam.UseSplit;
                if (_isSplit == true)
                {
                    string[] string2 = string1[1].Split('_');
                    if (string2.Length == 3)
                    {
                        key[0] = string2[1];
                        key[1] = string2[2];
                    }
                    else
                        _isSplit = false;
                }
                else
                {
                    key[0] = listLNCD[selectedIndex];
                }

                _key = key;

                double compRangeX = param.CompRange[0].MaxXRange;
                double compRangeY = param.CompRange[0].MaxYRange;

                int compStep = param.CompRange.Count + 1;
                int compSize = listLNCD.Count;

                foreach (var item in lot.MarkCompList.Data)
                {
                    for (int i = 0; i < compStep; i++)
                    {
                        var localDefects = new List<PrevCompareDefect>();
                        var localErrors = new List<PrevErrorAreaPosition>();

                        double localMaxY = item.Base.OFFSET;
                        int baseCnt = 0, compCnt = 0;
                        double oldX = 0, oldY = 0;

                        int errIdx = -1;

                        localDefects.Add(new PrevCompareDefect(
                            0, item.Base.XPOS_M, item.Base.OFFSET, item.Base.SIZE,
                            0, Interlocked.Increment(ref defIdx), Color.Gray, "B"));

                        oldX = item.Base.XPOS_M;
                        oldY = item.Base.OFFSET;
                        if (_isSplit == false)
                        {
                            var compList1 = item.Comp1.Where(kv => kv.Key.Item1 == key[0]).ToList();
                            if (compList1.Count > 0)
                            {
                                errIdx = -1;

                                foreach (var cList in compList1)
                                {
                                    for (int j = 0; j < cList.Value.Length; j++)
                                    {
                                        if (j <= 1)
                                        {
                                            compRangeX = param.BasicRange.MaxXRange;
                                            compRangeY = param.BasicRange.MaxYRange;
                                        }
                                        else
                                        {
                                            compRangeX = param.CompRange[j - 2].MaxXRange;
                                            compRangeY = param.CompRange[j - 2].MaxYRange;
                                        }

                                        bool isSet = false;
                                        PrevCompareDefect tmpItem = new PrevCompareDefect();
                                        var compList = cList.Value[j];
                                        for (int k = 0; k < cList.Value[j].Count; k++)
                                        {
                                            var comp = cList.Value[j][k];
                                            double posX = comp.XPOS_M;
                                            double posY = comp.OFFSET;

                                            if (j == 0)
                                            {
                                                tmpItem = new PrevCompareDefect(
                                                    0, posX, posY, comp.SIZE,
                                                    0, Interlocked.Increment(ref defIdx), Color.Yellow, "R");
                                                isSet = true;
                                                baseCnt++;
                                            }
                                            else
                                            {
                                                if (Math.Abs(oldX - posX) > compRangeX || Math.Abs(oldY - posY) > compRangeY)
                                                {
                                                    string symbolE = $"C_{j}";
                                                    tmpItem = new PrevCompareDefect(
                                                    0, posX, posY, comp.SIZE,
                                                    0, Interlocked.Increment(ref defIdx), Color.Red, symbolE);
                                                    isSet = true;

                                                    compCnt++;
                                                }
                                            }
                                            if (posY > localMaxY) localMaxY = posY;
                                        }

                                        if (isSet) localDefects.Add(tmpItem);


                                        // Error area 생성
                                        if (tmpItem.symbol != null && tmpItem.symbol.Contains("C_"))
                                        {
                                            if (((baseCnt > 0 && compCnt <= 0) || (baseCnt <= 0 && compCnt > 0)) && j > 0)
                                                errIdx = baseCnt > 0 ? 0 : j;
                                            else if (baseCnt > 0 && compCnt > 0 && j > 0)
                                                errIdx = j;
                                        }
                                    }
                                }

                                // Error area 생성
                                if (errIdx >= 0)
                                {
                                    var firstItem = compList1
                                                    .Select(kv => kv.Value)                               // 배열
                                                    .Where(arr => arr != null && arr.Length > errIdx)     // 인덱스 범위 체크
                                                    .Select(arr => arr[errIdx])                           // 리스트
                                                    .Where(list => list != null && list.Count > 0)        // 비어있지 않은 리스트만
                                                    .Select(list => list[0])                              // 리스트의 첫 요소
                                                    .FirstOrDefault();                                    // 전체 중 첫 요소

                                    if (firstItem != null)
                                    {
                                        double x = (int)(firstItem.XPOS_M / param.Judge.X) * param.Judge.X;
                                        double y = (int)(firstItem.OFFSET / judgeY1000) * judgeY1000;

                                        var newError = new PrevErrorAreaPosition(x, y, x + param.Judge.X, y + judgeY1000);
                                        bool exists = localErrors.Any(e =>
                                            Math.Abs(e.startX - newError.startX) < 0.001 &&
                                            Math.Abs(e.startY - newError.startY) < 0.001 &&
                                            Math.Abs(e.endX - newError.endX) < 0.001 &&
                                            Math.Abs(e.endY - newError.endY) < 0.001
                                        );

                                        if (!exists)
                                            localErrors.Add(newError);
                                    }
                                }
                            }

                            lock (lockObj)
                            {
                                defects.AddRange(localDefects);
                                errorAreas.AddRange(localErrors);
                                if (localMaxY > maxPosY) maxPosY = localMaxY;
                            }
                        }
                        else
                        {
                            for (int j = 0; j < item.Comp1[(key[0], key[1])].GetLength(0); j++)
                            {
                                var compList = item.Comp1[(key[0], key[1])][j];
                                PrevCompareDefect tmpItem = new PrevCompareDefect();
                                bool isSet = false;
                                for (int k = 0; k < compList.Count; k++)
                                {
                                    var comp = compList[k];
                                    double posX = comp.XPOS_M;
                                    double posY = comp.OFFSET;
                                    if (j == 0)
                                    {
                                        tmpItem = new PrevCompareDefect(
                                            0, posX, posY, comp.SIZE,
                                            0, Interlocked.Increment(ref defIdx), Color.Yellow, "R");
                                        isSet = true;
                                        baseCnt++;
                                    }
                                    else
                                    {
                                        if (Math.Abs(oldX - posX) > compRangeX || Math.Abs(oldY - posY) > compRangeY)
                                        {
                                            string symbolE = $"C_{j}";
                                            tmpItem = new PrevCompareDefect(
                                            0, posX, posY, comp.SIZE,
                                            0, Interlocked.Increment(ref defIdx), Color.Red, symbolE);
                                            isSet = true;
                                            compCnt++;
                                        }
                                    }
                                    if (posY > localMaxY) localMaxY = posY;
                                }
                                if (isSet) localDefects.Add(tmpItem);
                                // Error area 생성
                                if (tmpItem.symbol != null && tmpItem.symbol.Contains("C_"))
                                {
                                    if (((baseCnt > 0 && compCnt <= 0) || (baseCnt <= 0 && compCnt > 0)) && j > 0)
                                        errIdx = baseCnt > 0 ? 0 : j;
                                    else if (baseCnt > 0 && compCnt > 0 && j > 0)
                                        errIdx = j;
                                }
                            }
                            // Error area 생성
                            if (errIdx > 0)
                            {
                                foreach (var comp in item.Comp1[(key[0], key[1])][errIdx])
                                {
                                    double x = (int)(comp.XPOS_M / param.Judge.X) * param.Judge.X;
                                    double y = (int)(comp.OFFSET / judgeY1000) * judgeY1000;
                                    localErrors.Add(new PrevErrorAreaPosition(x, y, x + param.Judge.X, y + judgeY1000));
                                    break;
                                }
                            }

                            lock (lockObj)
                            {
                                defects.AddRange(localDefects);
                                errorAreas.AddRange(localErrors);
                                if (localMaxY > maxPosY) maxPosY = localMaxY;
                            }
                        }
                    }
                }

                foreach (var def in defects)
                    Rollmap.AddPrevDefect(def);

                foreach (var area in errorAreas)
                    Rollmap.AddPrevErrorArea(area);

                Rollmap.WholeHeight = (int)(maxPosY + 100000);
                Rollmap.RedrawAll();
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Rollmap Draw Error:{ex.Message}");
            }
        }
        #endregion

        private void FormRollMap_Load(object sender, EventArgs e)
        {
            Rollmap.DefectFont = this.rollmapDefectFont;
        }

        public void ClearMap()
        {
            Rollmap.RemoveAll();
        }

        public void PaintCsvData()
        {

        }

        #region Combo Box 업데이트
        private void cbProcess_SelectedIndexChanged(object sender, EventArgs e)
        {
            ClearMap();
            // Rollmap update
            switch (_dataType)
            {
                case eViewType.Normal:
                    drawMap(_crtLot, _info, _procItem);
                    break;

                case eViewType.CSV:
                    drawMap(_crtLot, _csvParam);
                    break;

                case eViewType.DB:
                    drawMap(_crtLot, _selParam);
                    break;
            }
        }
        #endregion
    }
}
