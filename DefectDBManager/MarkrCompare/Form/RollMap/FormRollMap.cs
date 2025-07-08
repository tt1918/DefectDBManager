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
using static System.Windows.Forms.VisualStyles.VisualStyleElement.Button;

namespace MarkCompare
{
    public partial class FormRollMap : Form
    {
        #region Param
        private PreprocLot _crtLot = null;
        private PreprocLNCDInfo _info = null;
        private CSVProcParam _csvInfo = null;
        private PreprocItem _procItem = null;
        private bool _isCSV = false;
        private Font rollmapDefectFont = new System.Drawing.Font("굴림", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));

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

            var defects = Rollmap.GetNearestDefects(real, gdi, true);
            foreach (var d in defects)
                DefectClicked?.Invoke(real, gdi, d.Key, d.Value);

            defect = defects.FirstOrDefault(e => e.Value.Id != -1).Value;
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

            for (int i = 0; i < _procItem.Compare.Count; i++)
            {
                string itemName = $"{i + 1}:{_procItem.Reference.LNCD}_{_procItem.Compare[i].LNCD}";
                cbProcess.Items.Add(itemName);
            }
            cbProcess.SelectedIndex = 0;
        }
        private void UpdateCbProcessCSV()
        {
            cbProcess.Items.Clear();

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

        public void OnUpdateLotInfo(PreprocLot lot, PreprocLNCDInfo info, PreprocItem procItem)
        {
            if (lot.FaultData == null) return;
            _crtLot = lot;
            _info = info;
            _procItem = procItem;
            _isCSV = false;

            lblLotName.Text = _crtLot.LotName;

            if (cbProcess.InvokeRequired)   cbProcess.BeginInvoke(new Action(UpdateCbProcess));
            else                            UpdateCbProcess();

            // Rollmap update
            drawMap(_crtLot, _info, _procItem);
        }

        public void OnUpdateLotInfo(PreprocLot lot, CSVProcParam info)
        {
            if (lot.FaultData == null) return;
            _crtLot = lot;
            _csvInfo = info;
            _procItem = null;
            _isCSV = true;

            lblLotName.Text = _crtLot.LotName;

            if (cbProcess.InvokeRequired) cbProcess.BeginInvoke(new Action(UpdateCbProcessCSV));
            else UpdateCbProcessCSV();

            // Rollmap update
            drawMap(_crtLot, _csvInfo);
        }

        //private void drawMap(PreprocLot lot, PreprocLNCDInfo info, PreprocItem procItem)
        //{
        //    int defIdx = 0;

        //    double posX = 0, posY = 0;
        //    double oldX = 0, oldY = 0;

        //    double maxPosY = 0;

        //    foreach (var item in lot.MarkCompList.Data)
        //    {
        //        for (int i = 0; i < item.Comp.GetLength(0); i++)
        //        {
        //            int baseCnt = 0;
        //            int compCnt = 0;
                    
        //            if (i != cbProcess.SelectedIndex) continue;

        //            Rollmap.AddPrevDefect(new PrevCompareDefect(0, item.Base.XPOS_M, item.Base.OFFSET, item.Base.SIZE, 0, defIdx++, Color.Gray, info.Symbol));

        //            if (maxPosY < item.Base.OFFSET) maxPosY = item.Base.OFFSET;

        //            oldX = oldY = 0;

        //            for (int j = 0; j < item.Comp.GetLength(1); j++)
        //            {
        //                for (int k = 0; k < item.Comp[i, j].Count; k++)
        //                {
        //                    posX = item.Comp[i, j][k].XPOS_M;
        //                    posY = item.Comp[i, j][k].OFFSET;
        //                    if (j == 0)
        //                    {
        //                        oldX = posX;
        //                        oldY = posY;
        //                        Rollmap.AddPrevDefect(new PrevCompareDefect(0, posX, posY, item.Comp[i, j][k].SIZE, 0, defIdx++, Color.Yellow, "R"));
        //                        baseCnt++;
        //                    }
        //                    else
        //                    {
        //                        if(Math.Abs(oldX - posX)>5 || Math.Abs(oldX - posX) > 5)
        //                        {
        //                            string symbolE = $"C_{j}";
        //                            oldX = posX;
        //                            oldX = posX;
        //                            Rollmap.AddPrevDefect(new PrevCompareDefect(0, posX, posY, item.Comp[i, j][k].SIZE, 0, defIdx++, Color.Red, symbolE));
        //                        }
        //                        compCnt++;
        //                    }
        //                }

        //                if (((baseCnt > 0 && compCnt <= 0) || (baseCnt <= 0 && compCnt > 0)) && j > 0)
        //                {
        //                    int index = baseCnt > 0 ? 0 : j;
        //                    for (int l = 0; l < item.Comp[i, index].Count; l++)
        //                    {
        //                        posX = 0;
        //                        posY = 0;

        //                        if (baseCnt > 0)
        //                        {
        //                            posX = item.Comp[i, 0][l].XPOS_M;
        //                            posY = (float)(item.Comp[i, 0][l].OFFSET);
        //                        }
        //                        else
        //                        {
        //                            posX = item.Comp[i, j][l].XPOS_M;
        //                            posY = (float)(item.Comp[i, j][l].OFFSET);
        //                        }

        //                        int areaX = (int)(posX / procItem.Judge.X);
        //                        int areaY = (int)(posY / (procItem.Judge.Y * 1000));

        //                        double x = areaX * procItem.Judge.X;
        //                        double y = areaY * procItem.Judge.Y * 1000;
        //                        Rollmap.AddPrevErrorArea(new PrevErrorAreaPosition(x, y, x + procItem.Judge.X, y + (procItem.Judge.Y * 1000)));
        //                    }
        //                }
        //            }
        //        }
        //    }

        //    Rollmap.WholeHeight = (int)maxPosY+100000;

        //    Rollmap.RedrawAll();
        //}

        private void drawMap(PreprocLot lot, PreprocLNCDInfo info, PreprocItem procItem)
        {
            int defIdx = 0;
            double maxPosY = 0;
            double judgeY1000 = procItem.Judge.Y * 1000;
            var errorAreas = new List<PrevErrorAreaPosition>();
            var defects = new List<PrevCompareDefect>();
            object lockObj = new object();

            var selectedIndex = cbProcess.SelectedIndex;

            //Parallel.ForEach(lot.MarkCompList.Data, item =>
            foreach(var item in lot.MarkCompList.Data)
            {
                for (int i = 0; i < item.Comp.GetLength(0); i++)
                {
                    if (i != selectedIndex) continue;

                    var localDefects = new List<PrevCompareDefect>();
                    var localErrors = new List<PrevErrorAreaPosition>();

                    double localMaxY = item.Base.OFFSET;
                    int baseCnt = 0, compCnt = 0;
                    double oldX = 0, oldY = 0;

                    localDefects.Add(new PrevCompareDefect(
                        0, item.Base.XPOS_M, item.Base.OFFSET, item.Base.SIZE,
                        0, Interlocked.Increment(ref defIdx), Color.Gray, info.Symbol));

                    for (int j = 0; j < item.Comp.GetLength(1); j++)
                    {
                        var compList = item.Comp[i, j];
                        for (int k = 0; k < compList.Count; k++)
                        {
                            var comp = compList[k];
                            double posX = comp.XPOS_M;
                            double posY = comp.OFFSET;

                            if (j == 0)
                            {
                                oldX = posX;
                                oldY = posY;
                                localDefects.Add(new PrevCompareDefect(
                                    0, posX, posY, comp.SIZE,
                                    0, Interlocked.Increment(ref defIdx), Color.Yellow, "R"));
                                baseCnt++;
                            }
                            else
                            {
                                if (Math.Abs(oldX - posX) > 5 || Math.Abs(oldY - posY) > 5)
                                {
                                    oldX = posX;
                                    oldY = posY;
                                    string symbolE = $"C_{j}";
                                    localDefects.Add(new PrevCompareDefect(
                                        0, posX, posY, comp.SIZE,
                                        0, Interlocked.Increment(ref defIdx), Color.Red, symbolE));
                                }
                                compCnt++;
                            }

                            if (posY > localMaxY) localMaxY = posY;
                        }

                        // Error area 생성
                        if (((baseCnt > 0 && compCnt <= 0) || (baseCnt <= 0 && compCnt > 0)) && j > 0)
                        {
                            int index = baseCnt > 0 ? 0 : j;
                            foreach (var comp in item.Comp[i, index])
                            {
                                double x = (int)(comp.XPOS_M / procItem.Judge.X) * procItem.Judge.X;
                                double y = (int)(comp.OFFSET / judgeY1000) * judgeY1000;
                                localErrors.Add(new PrevErrorAreaPosition(x, y, x + procItem.Judge.X, y + judgeY1000));
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
            }

            foreach (var def in defects)
                Rollmap.AddPrevDefect(def);

            foreach (var area in errorAreas)
                Rollmap.AddPrevErrorArea(area);

            Rollmap.WholeHeight = (int)(maxPosY + 100000);
            Rollmap.RedrawAll();
        }

        private void drawMap(PreprocLot lot, CSVProcParam param)
        {
            int defIdx = 0;
            double maxPosY = 0;
            double judgeY1000 = param.Judge.Y * 1000;
            var errorAreas = new List<PrevErrorAreaPosition>();
            var defects = new List<PrevCompareDefect>();
            object lockObj = new object();

            var selectedIndex = cbProcess.SelectedIndex;

            Parallel.ForEach(lot.MarkCompList.Data, item =>
            {
                for (int i = 0; i < item.Comp.GetLength(0); i++)
                {
                    if (i != selectedIndex) continue;

                    var localDefects = new List<PrevCompareDefect>();
                    var localErrors = new List<PrevErrorAreaPosition>();

                    double localMaxY = item.Base.OFFSET;
                    int baseCnt = 0, compCnt = 0;
                    double oldX = 0, oldY = 0;

                    localDefects.Add(new PrevCompareDefect(
                        0, item.Base.XPOS_M, item.Base.OFFSET, item.Base.SIZE,
                        0, Interlocked.Increment(ref defIdx), Color.Gray, "C"));

                    for (int j = 0; j < item.Comp.GetLength(1); j++)
                    {
                        var compList = item.Comp[i, j];
                        for (int k = 0; k < compList.Count; k++)
                        {
                            var comp = compList[k];
                            double posX = comp.XPOS_M;
                            double posY = comp.OFFSET;

                            if (j == 0)
                            {
                                oldX = posX;
                                oldY = posY;
                                localDefects.Add(new PrevCompareDefect(
                                    0, posX, posY, comp.SIZE,
                                    0, Interlocked.Increment(ref defIdx), Color.Yellow, "R"));
                                baseCnt++;
                            }
                            else
                            {
                                if (Math.Abs(oldX - posX) > 5 || Math.Abs(oldY - posY) > 5)
                                {
                                    oldX = posX;
                                    oldY = posY;
                                    string symbolE = $"C_{j}";
                                    localDefects.Add(new PrevCompareDefect(
                                        0, posX, posY, comp.SIZE,
                                        0, Interlocked.Increment(ref defIdx), Color.Red, symbolE));
                                }
                                compCnt++;
                            }

                            if (posY > localMaxY) localMaxY = posY;
                        }

                        // Error area 생성
                        if (((baseCnt > 0 && compCnt <= 0) || (baseCnt <= 0 && compCnt > 0)) && j > 0)
                        {
                            int index = baseCnt > 0 ? 0 : j;
                            foreach (var comp in item.Comp[i, index])
                            {
                                double x = (int)(comp.XPOS_M / param.Judge.X) * param.Judge.X;
                                double y = (int)(comp.OFFSET / judgeY1000) * judgeY1000;
                                localErrors.Add(new PrevErrorAreaPosition(x, y, x + param.Judge.X, y + judgeY1000));
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
            });

            foreach (var def in defects)
                Rollmap.AddPrevDefect(def);

            foreach (var area in errorAreas)
                Rollmap.AddPrevErrorArea(area);

            Rollmap.WholeHeight = (int)(maxPosY + 100000);
            Rollmap.RedrawAll();
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
            if(!_isCSV) drawMap(_crtLot, _info, _procItem);
            else        drawMap(_crtLot, _csvInfo); 
        }
        #endregion

    }
}
