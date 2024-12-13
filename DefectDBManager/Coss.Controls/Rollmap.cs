using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.ComponentModel;
using System.ComponentModel.Design;
#if USE_SHARP_DX
using SharpDX.WIC;
using SharpDX.Direct2D1;
using SharpDX.Mathematics.Interop;
#else
using JeremyAnsel.DirectX.D2D1;
using JeremyAnsel.DirectX.DWrite;
#endif
using System.Threading.Tasks;
using System.Diagnostics;

namespace Coss.Controls
{
    public enum tagShowGridLine
    {
        None = 0,
        Horizontal,
        All,
        AllDash,
        AllSimple,
        AllRuller,
    }

    /// <summary>
    /// 롤그래프 컨트롤
    /// View: GDI 좌표
    /// Real: mm 좌표
    /// </summary>
    public partial class Rollmap : UserControl
    {
        public const int kMaxTapeKind = 3;
        public const int kMaxTapePositions = 5;
        public const int kMaxOptic = 16;

        [Category("Rollmap")]
        public RollmapViewAreaChangedHandler ViewAreaChanged;

        [Category("Rollmap")]
        public RollmapMouseClickedHandler MouseClicked;

        [Category("Rollmap")]
        public RollmapMouseClickedHandler MouseDoubleClicked;

        [Category("Rollmap")]
        public RollmapUpDnHandler RollmapUpDownChanged;

        [Category("Rollmap")]
        public RollmapMouseUpHandler CustomMouseUp;

        public SyncRollmapEtcHandler SyncRollmapEtc; 


        [Category("Rollmap")]
        [DefaultValue(3000 * 1000)]
        public int WholeHeight { get; set; } = 3000 * 1000;

        [Category("Rollmap")]
        [DefaultValue(2000)]
        public int WholeWidth { get; set; } = 2000;

        [Category("Rollmap")]
        [DefaultValue(200 * 1000)]
        public int MapAutoRange { get; set; } = 200 * 1000;

        private int mapAutoPosion = 0;
        /// <summary>
        /// 검사위치(mm)
        /// </summary> 
        [Category("Rollmap")]
        [DefaultValue(0)]
        public int MapAutoPosion
        {
            get { return mapAutoPosion; }
            set
            {
                mapAutoPosion = value;
            }
        }

        [Category("Rollmap")]
        [DefaultValue(200 * 1000)]
        public int MapManualRange { get; set; } = 200 * 1000;

        /// <summary>
        /// Load Map 시작 위치
        /// </summary>
        public int MapManualStartPos { get; set; } = 0;

        public Rectangle ViewRealRect { get; set; }

        private int offsetX = 0;
        /// <summary>
        /// Map X축 Margin (좌우 적용)
        /// </summary>
        [Category("Rollmap")]
        [DefaultValue(0)]
        public int OffsetX
        {
            get { return offsetX; }
            set { offsetX = value; }
        }
        private int offsetY = 0;
        /// <summary>
        /// Map Y축 Margin (상하 적용)
        /// </summary>
        [Category("Rollmap")]
        [DefaultValue(0)]
        public int OffsetY
        {
            get { return offsetY; }
            set { offsetY = value; }
        }

        private int maxZoneX = 20;
        [Category("Rollmap")]
        [DefaultValue(20)]
        public int MaxZoneX
        {
            get { return maxZoneX; }
            set { maxZoneX = value; }
        }
        private int maxZoneY = 30;
        [Category("Rollmap")]
        [DefaultValue(30)]
        public int MaxZoneY
        {
            get { return maxZoneY; }
            set { maxZoneY = value; }
        }

        [Category("Rollmap")]
        [DefaultValue(tagShowGridLine.All)]
        public tagShowGridLine ShowGridLine { get; set; } = tagShowGridLine.All;


        [Category("Rollmap")]
        [DefaultValue(true)]
        public bool ShowGridLineMeter { get; set; } = true;

        public static readonly String kOptionGraphTypeKey = "graph.type";
        public static readonly String kOptionGuiThema = "gui.thema";


        [Category("Rollmap")]
        public GridFrame Frames { get { return frames; }}
        private GridFrame frames;

        [Category("Rollmap")]
        [DefaultValue(true)]
        public bool UseGradation { get; set; } = false;

        private bool disableDraw = false;
        public bool DisableDraw { get { return disableDraw; } set { disableDraw = value; if (this.defects != null) this.defects.DisableDraw = value; } }

        [Category("Rollmap")]
        [DefaultValue(true)]
        private bool showDefect = true;
        public bool ShowDefect { get { return showDefect; } set { showDefect = value; RedrawAll(); } }

        [Category("Rollmap")]
        [DefaultValue(false)]
        private bool relativeCoordinate = false;
        public bool RelativeCoordinate { get { return relativeCoordinate; } set { relativeCoordinate = value; RedrawAll(); } }

        [Category("Rollmap")]
        [DefaultValue(false)]
        private bool showEdge = false;
        public bool ShowEdge { get { return showEdge; } set { showEdge = value; RedrawAll(); } }

        private int automapViewOffsetY = 0;
        /// <summary>
        /// 자동맵 표시 Y축 시작위치 (Pixel)
        /// </summary>
        [Category("Rollmap")]
        [DefaultValue(0)]
        public int AutomapViewOffsetY
        {
            get { return automapViewOffsetY; }
            set { automapViewOffsetY = value; }
        }

        private float osPosX = 0;
        /// <summary>
        /// OS 시작위치  (mm)
        /// </summary>
        [Category("Rollmap")]
        [DefaultValue(0)]
        public float OsPosX
        {
            get { return osPosX; }
            set { osPosX = value; }
        }

        private float dsPosX = 0;
        /// <summary>
        /// DS 시작위치  (mm)
        /// </summary>
        [Category("Rollmap")]
        [DefaultValue(0)]
        public float DsPosX
        {
            get { return dsPosX; }
            set { dsPosX = value; }
        }

        private bool osDsPosXbyEdge = false;
        /// <summary>
        /// Edge위치를 받아 OS,DS 위치를 맵에 표시함
        /// true면 받은엣지로 자체 계산
        /// false면 외부에서 받은 OsPosX,dsPosX값을 그대로 표시
        /// </summary>
        [Category("Rollmap")]
        [DefaultValue(false)]
        public bool OsDsPosXbyEdge
        {
            get { return osDsPosXbyEdge; }
            set { osDsPosXbyEdge = value; }
        }
        

        private bool showOsDsPosX = false;
        [Category("Rollmap")]
        [DefaultValue(false)]
        public bool ShowOsDsPosX
        {
            get { return showOsDsPosX; }
            set { showOsDsPosX = value; }
        }

        private bool showOpticDistance = false;
        [Category("Rollmap")]
        [DefaultValue(false)]
        public bool ShowOpticDistance
        {
            get { return showOpticDistance; }
            set { showOpticDistance = value; }
        }
        double[] opticDistance = new double[kMaxOptic];
        public double[] OpticDistance
        {
            get { return opticDistance; }
            private set { opticDistance = value;}
        }
        Color[] opticColor = new Color[kMaxOptic];
        public Color[] OpticColor
        {
            get { return opticColor; }
            private set { opticColor = value;}
        }
        public int StdOptic { get; set; }

        private float osViewPosX = 0;
        /// <summary>
        /// OS View 시작위치, 모니터링모드에서 임시로 사용 (mm)
        /// </summary>
        [Category("Rollmap")]
        [DefaultValue(0)]
        public float OsViewPosX
        {
            get { return osViewPosX; }
            set { osViewPosX = value; }
        }

        public bool ShowDefectColor()
        {
            return this.defects.ShowDefectColor;
        }
        public void ShowDefectColor(bool value)
        {
            this.defects.ShowDefectColor = value;
            RedrawAll();
        }

        private int markingSerieIndex = -1;
        public int MarkingSerieIndex {
            get { return markingSerieIndex; }
            set { markingSerieIndex = value; if(defects!=null) defects.MarkingSerieIndex = MarkingSerieIndex;}
        }

        [Category("Rollmap")]
        [DefaultValue(true)]
        private bool showOS = true;
        public bool ShowOS { get { return showOS; } set { showOS = value; RedrawAll(); } }

        public enum OptionGraphType
        {
            kVerticalScroll,
            kHorizontalScroll,
        }
        private OptionGraphType graphType = OptionGraphType.kVerticalScroll;
        public OptionGraphType GraphType { get { return graphType;  } set { graphType = value; RedrawAll(); } }

        [Category("Rollmap")]
        [DefaultValue(20)]
        private int defectSearchRange = 20;
        /// <summary>
        /// 클릭된 결점을 찾는 범위(pixel)
        /// </summary>
        public int DefectSearchRange { get { return defectSearchRange; } set { defectSearchRange = value; } }

        [Category("Rollmap")]
        private Color clickedColor = Color.White;
        /// <summary>
        /// 클릭된 결점 색상
        /// </summary>
        public Color ClickedColor { get { return clickedColor; } set { clickedColor = value; if(defects != null) defects.ClickedColor = value; } }

        [Category("Rollmap")]
        [DefaultValue(false)]
        private bool showGraphics = false;
        public bool ShowGraphics { get { return showGraphics; } set { showGraphics = value; RedrawAll(); } }

        private object graphicsLock = new object();
        private List<RollmapGraphics> graphics = new List<RollmapGraphics>();

        public void ResetGraphics() { lock (graphicsLock) { graphics.Clear(); UpdateDraw();/*Invalidate();*/ } }

        [Category("Rollmap")]
        private bool useVscroll = false;
        public bool UseVscroll { get { return useVscroll; } set { useVscroll = value; RedrawAll(); } }

        [Category("Rollmap")]
        [DefaultValue(false)]
        private bool useColor2 = false;
        /// <summary>
        /// 약불량 색상 사용시 true, false면 Marking유무에 따라 마킹 = 원래색상, 미마킹 = Gray
        /// </summary>
        public bool UseColor2 { get { return useColor2; } set { useColor2 = value; if (defects != null) defects.UseColor2 = value; RedrawAll(); } }

        [Category("Rollmap")]
        private Font defectFont;
        public Font DefectFont
        {
            get { return defectFont; }
            set { defectFont?.Dispose(); defectFont = value; UpdateTextFormat();  RedrawAll(); }
        }

        /// <summary>
        /// 주기결점 Symbol을 @와 같은 지정된 문자가 아닌, 200과 같은 실주기로 표시
        /// </summary>
        [Category("Rollmap")]
        private bool showPeriod;
        public bool ShowPeriod
        {
            get { return showPeriod; }
            set { showPeriod = value; if (defects != null) defects.ShowPeriod = value; RedrawAll(); }
        }

        /// <summary>
        /// 주기불량 분류별 맵 표시
        /// </summary>
        [Category("Rollmap")]
        bool usePeriodClassification;
        public bool UsePeriodClassification
        {
            get { return usePeriodClassification; }
            set { usePeriodClassification = value; if (defects != null) defects.UsePeriodClassification = value; RedrawAll(); }
        }

        /// <summary>
        /// 동일결점 on/off 기능
        /// </summary>
        [Category("Rollmap")]
        bool useSameDefect;
        public bool UseSameDefect
        {
            get { return useSameDefect; }
            set { useSameDefect = value; if (defects != null) defects.UseSameDefect = value; RedrawAll(); }
        }
        List<int> cycleParam;
        public List<int> CycleParam
        {
            get { return cycleParam; }
            set { cycleParam = value; if (defects != null) defects.CycleParam = value; RedrawAll(); }
        }

        /// <summary>
        /// 주기불량 ShowPeriodDefect
        /// </summary>
        [Category("Rollmap")]
        bool[,] showPeriodDefect;
        public bool[,] ShowPeriodDefect
        {
            get { return showPeriodDefect; }
            set { showPeriodDefect = value; if (defects != null) defects.ShowPeriodDefect = value; RedrawAll(); }
        }

        /// <summary>
        /// 동일결점 표시
        /// </summary>
        [Category("Rollmap")]
        bool[] showSameDefect;
        public bool[] ShowSameDefect
        {
            get { return showSameDefect; }
            set { showSameDefect = value; if (defects != null) defects.ShowSameDefect = value; RedrawAll(); }
        }
        /// <summary>
        /// N사 좌표비교 사용
        /// </summary>
        bool useComparePostion;
        public bool UseComparePosition
        {
            get { return useComparePostion; }
            set { useComparePostion = value; if (defects != null) defects.ComparePostion = value; RedrawAll(); }
        }
        bool[] compareShowDefect;
        public bool[] CompareShowDefect
        {
            get { return compareShowDefect; }
            set { compareShowDefect = value; if (defects != null) defects.CompareShowDefect = value; }
        }
        /// <summary>
        /// 이전공정 결점비교 On/Off
        /// </summary>
        bool usePrevCompare;
        public bool UsePrevCompare
        {
            get { return usePrevCompare; }
            set { usePrevCompare = value; if (defects != null) defects.UsePrevCompare = value; RedrawAll(); }
        }
        /// <summary>
        /// 이전공정 동일결점 범위
        /// </summary>
        Point prevCompareSameRange;
        public Point PrevCompareSameRange
        {
            get { return  prevCompareSameRange; }
            set { prevCompareSameRange = value; if(defects != null) defects.PrevCompareSameRange = value; }
        }

        bool isAiMode;
        public bool IsAiMode 
        {
            get { return isAiMode; }
            set { isAiMode = value; if (defects != null) defects.IsAiMode = value; }
        }

        public void SetPeriodParam(List<int> cycle, bool[,] show)
        {
            this.CycleParam = cycle;
            this.ShowPeriodDefect = show;
        }

        public void SetSameDefectParam(bool[] show)
        {
            this.ShowSameDefect = show;
        }

        public void SetCaptureRender()
        {
            captureRender = true;
            Captured = false;
            MapViewOption(MapViewOptionArgs.tagMapViewRange.Fit);

            RedrawAll();
        }
        private bool captureRender = false;
        public bool Captured { get; set; } = false;

        private System.Drawing.Bitmap bmpCaptured;
        public System.Drawing.Bitmap BmpCaptured { get { return bmpCaptured; } }
        /// <summary>
        /// 결점제거창 on/off 체크
        /// </summary>
        public bool UseDefectDelete { get; set; } = false;

        private bool useQueue = false;
        private Timer timer100msec;
        private Queue<RollmapDefect> inputBuffer = new Queue<RollmapDefect>();
        private Queue<RollmapDefect> inputBufferMarked = new Queue<RollmapDefect>();
        private Queue<RollmapDefect> inputBufferAi = new Queue<RollmapDefect>();
        private Queue<PrevCompareDefect> inputBufferPrev = new Queue<PrevCompareDefect>(); //이전공정 추가 @ATW 240318
        private bool drawDefect = false;
        private bool exit = false;
        private RectangleF viewRect;
        private int defectOffsetX = 0;
        public int DefectOffsetX
        {
            get { return defectOffsetX; }
            set { defectOffsetX = value; }
        }

        public Rollmap()
        {
        	//D2D 사용으로 삭제
            //this.SetStyle(ControlStyles.DoubleBuffer, true);
            //this.SetStyle(ControlStyles.UserPaint, true);
            //this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            InitializeComponent();
        }

        // D2D1
#if USE_SHARP_DX
        private SharpDX.Direct2D1.Factory _factory;
        private SharpDX.Direct2D1.RenderTarget _hwndRenderTarget;
        private SharpDX.WIC.ImagingFactory _wicFactory;
        private SharpDX.DirectWrite.Factory _dWriteFactory;
        private SharpDX.DirectWrite.TextFormat _textFormat;
        private SharpDX.WIC.Bitmap _wicBitmap;
        private SharpDX.Direct2D1.RenderTarget _wicRenderTarget;
        private SharpDX.Direct2D1.SolidColorBrush _wicColorBrush;
        private SharpDX.Direct2D1.SolidColorBrush _colorBrush;
#else
        private D2D1Factory _factory;
        private D2D1RenderTarget _hwndRenderTarget;
        private D2D1RenderTarget _wicfdRenderTarget;
        private D2D1SolidColorBrush _colorBrush;
        DWriteFactory _dWriteFactory;
        DWriteTextFormat _textFormat;
#endif
        object lockRenderTarget = new object();


        protected override void OnHandleCreated(EventArgs e)
        {
#if USE_SHARP_DX
            _factory = new SharpDX.Direct2D1.Factory();
            InitD2d1RenderTarget();

            _dWriteFactory = new SharpDX.DirectWrite.Factory(SharpDX.DirectWrite.FactoryType.Isolated);
            UpdateTextFormat();
#else
            _factory = D2D1Factory.Create(D2D1FactoryType.SingleThreaded);
            InitD2d1RenderTarget();
           
            _dWriteFactory = DWriteFactory.Create(DWriteFactoryType.Isolated);
            UpdateTextFormat();
#endif

            base.OnHandleCreated(e);
        }

        private void UpdateTextFormat()
        {
#if USE_SHARP_DX
            _textFormat?.Dispose();

            var weight = SharpDX.DirectWrite.FontWeight.Normal;
            if (defectFont != null)
            {
                if (defectFont.Bold) weight = SharpDX.DirectWrite.FontWeight.Bold;

                _textFormat = new SharpDX.DirectWrite.TextFormat(_dWriteFactory, defectFont.Name, null, weight, SharpDX.DirectWrite.FontStyle.Normal, SharpDX.DirectWrite.FontStretch.Normal, defectFont.Size, "");
            }
#else
            _textFormat?.Release();
            _textFormat?.Dispose();

            DWriteFontWeight weight = DWriteFontWeight.Normal;
            if(defectFont != null)
            {
                if (defectFont.Bold) weight = DWriteFontWeight.Bold;

                _textFormat = _dWriteFactory.CreateTextFormat(defectFont.Name, null, weight, DWriteFontStyle.Normal, DWriteFontStretch.Normal, defectFont.Size, "");
            }
#endif
        }

        private void InitD2d1RenderTarget()
		{
#if USE_SHARP_DX
            HwndRenderTargetProperties hwndRenderTargetProperties = new HwndRenderTargetProperties
            {
                Hwnd = this.Handle,
                PixelSize = new SharpDX.Size2(ClientSize.Width, ClientSize.Height),
                PresentOptions = PresentOptions.Immediately
            };
            RenderTargetProperties renderTargetProperties = new RenderTargetProperties
            {
                Type = RenderTargetType.Default,
                Usage = RenderTargetUsage.None,
                PixelFormat = new SharpDX.Direct2D1.PixelFormat(SharpDX.DXGI.Format.B8G8R8A8_UNorm, SharpDX.Direct2D1.AlphaMode.Premultiplied)
            };

            _hwndRenderTarget = new WindowRenderTarget(_factory, renderTargetProperties, hwndRenderTargetProperties);
            _hwndRenderTarget.AntialiasMode = AntialiasMode.PerPrimitive;
            _colorBrush = new SolidColorBrush(_hwndRenderTarget, ColorConverter.ColorToRawColor(Color.Yellow));

            _wicFactory = new SharpDX.WIC.ImagingFactory();
            _wicBitmap = new SharpDX.WIC.Bitmap(_wicFactory, (int)ClientSize.Width, (int)ClientSize.Height, SharpDX.WIC.PixelFormat.Format32bppBGR, BitmapCreateCacheOption.CacheOnLoad);

            var wicRenderTargetProperties = new RenderTargetProperties(RenderTargetType.Default,
                new SharpDX.Direct2D1.PixelFormat(SharpDX.DXGI.Format.Unknown,
                    SharpDX.Direct2D1.AlphaMode.Unknown), 0, 0, RenderTargetUsage.None,
                    SharpDX.Direct2D1.FeatureLevel.Level_DEFAULT);

            _wicRenderTarget = new WicRenderTarget(_factory, _wicBitmap, wicRenderTargetProperties);
            _wicRenderTarget.AntialiasMode = AntialiasMode.PerPrimitive;
            _wicColorBrush = new SolidColorBrush(_wicRenderTarget, ColorConverter.ColorToRawColor(Color.Yellow));

#else
            D2D1RenderTargetProperties renderTargetProperties = new D2D1RenderTargetProperties
            {
                RenderTargetType = D2D1RenderTargetType.Default,
                Usage = D2D1RenderTargetUsages.None,
                PixelFormat = new D2D1PixelFormat() { AlphaMode = D2D1AlphaMode.Premultiplied, Format = JeremyAnsel.DirectX.Dxgi.DxgiFormat.B8G8R8A8UNorm }
            };
            D2D1HwndRenderTargetProperties hwndRenderTargetProperties = new D2D1HwndRenderTargetProperties
            {
                Hwnd = this.Handle,
                PixelSize = new D2D1SizeU((uint)ClientSize.Width, (uint)ClientSize.Height),
                PresentOptions = D2D1PresentOptions.Immediately
            };
            _hwndRenderTarget = _factory.CreateHwndRenderTarget(renderTargetProperties, hwndRenderTargetProperties);
            _hwndRenderTarget.AntialiasMode = D2D1AntialiasMode.PerPrimitive;
            _colorBrush = _hwndRenderTarget.CreateSolidColorBrush(new D2D1ColorF(D2D1KnownColor.Yellow));
#endif
        }


        /// <summary>
        /// 롤맵초기화
        /// </summary>
        /// <param name="showZoomArea"></param>
        /// <param name="showDeleteArea"></param>
        /// <param name="fixedGridLines">true=정해진라인수만큼그림, false=정해진거리마다그림</param>
        /// <param name="useQueue"></param>
        public void Init(bool showZoomArea, bool showDeleteArea, bool fixedGridLines, bool useQueue)
        {
            if(defects == null)
            {
                defects = new GridDefects(useQueue);
            }

            if(sheets == null)
            {
                sheets = new GridSheet();
            }

            zoomArea.Init(showZoomArea, 2, Color.Transparent, Color.Red);
            deleteArea.Init(showDeleteArea, 2, Color.Transparent, Color.LightYellow);

            ViewRealRect = new Rectangle(0, 0, WholeWidth, WholeHeight);
            //deleteArea.SetRect(ConvertRealToView(new Rectangle(
            //                    (int)ViewRealRect.Left, (int)ViewRealRect.Top,
            //                    (int)ViewRealRect.Right, 200 * 1000)));
            if (fixedGridLines)
            {
                frames = new GridFrame();
                frames.GridXLines = maxZoneX;
                frames.GridYLines = maxZoneY;
            }
            else
            {
                frames = new GridFrame();
                frames.GridXRange = 500;
                frames.GridYRange = 1000 * 1000;
            }
            frames.GridLinePen.Dispose();
            frames.GridLinePen = new Pen(frames.GridMeterBrush, 1);

            frames.GridDashLinePen.Dispose();
            frames.GridDashLinePen = new Pen(frames.GridMeterBrush, 1);
            frames.GridDashLinePen.DashStyle = System.Drawing.Drawing2D.DashStyle.Dash;

            frames.GridDashLinePen2.Dispose();
            frames.GridDashLinePen2 = new Pen(frames.GridMeterBrush, 1);
            frames.GridDashLinePen2.DashStyle = System.Drawing.Drawing2D.DashStyle.DashDotDot;

            frames.SimpleGridLinePen.Dispose();
            frames.SimpleGridLinePen = new Pen(frames.GridMeterBrush, 2);

            frames.RullerGridLinePen1.Dispose();
            frames.RullerGridLinePen1 = new Pen(frames.GridMeterBrush, 2);

            frames.RullerGridLinePen2.Dispose();
            frames.RullerGridLinePen2 = new Pen(frames.GridMeterBrush, 1);

            if(useQueue)
            {
                if (this.timer100msec == null)
                {
                    this.timer100msec = new System.Windows.Forms.Timer();
                    this.timer100msec.Interval = 100;
                    this.timer100msec.Start();
                    this.timer100msec.Tick += new EventHandler(OnTimer100msec);
                }
            }
            this.useQueue = useQueue;

            //종스크롤 한번클릭시, 작게는 1M, 크게는 10M씩 이동
            vScrollBar1.LargeChange = 10000;
            vScrollBar1.SmallChange = 1000;
        }
        public void Term ( )
		{
			defects.Term();
            
            sheets.Term();

			DisableDraw = true;
			exit = true;
			if ( this.timer100msec != null )
			{
				this.timer100msec.Stop();
			}

#if USE_SHARP_DX
			_factory?.Dispose();
			_hwndRenderTarget?.Dispose();
			_colorBrush?.Dispose();
			_dWriteFactory?.Dispose();
			_textFormat?.Dispose();
            _wicFactory?.Dispose();
            _wicBitmap?.Dispose();
            _wicColorBrush?.Dispose();
            _wicRenderTarget?.Dispose();
#else
            _factory?.Release();
			_factory?.Dispose();
			_hwndRenderTarget?.Release();
			_hwndRenderTarget?.Dispose();
            _colorBrush?.Release();
			_colorBrush?.Dispose();
			_dWriteFactory?.Release();
			_dWriteFactory?.Dispose();
			_textFormat?.Release();
			_textFormat?.Dispose();
#endif
        }

        /// <summary>
        /// 롤맵 속성 설정
        /// <param name="key">Option Key 값</param>
        /// <param name="value">Option value 값</param>
        /// </summary>
        public bool SetOption(String key, String value)
        {
            if (options.ContainsKey(key))
            {
                options[key] = value;

                if (key == kOptionGuiThema)
                {
                    SetGuiThema(value);
                }
                return true;
            }
            return false;
        }

        private void OnTimer100msec(object sender, EventArgs e)
        {
            if (exit) return;
            lock(inputBuffer)
            {
                //Trace.WriteLine($"defect={inputBuffer.Count}");
                while(inputBuffer.Count > 0)
                {
                    if (exit) return;
                    var defect = inputBuffer.Dequeue();
                    AddDefectInner(defect, drawDefect);
                }
            }
            lock (inputBufferMarked)
            {
                //Trace.WriteLine($"marked={inputBufferMarked.Count}");
                while (inputBufferMarked.Count > 0)
                {
                    if (exit) return;
                    var marked = inputBufferMarked.Dequeue();
                    AddMarkedDefectInner(marked, drawDefect);

                    if(marked.Id >= 0)
                        defects.SetMarked(marked.Id);
                }
            }
            lock (inputBufferAi)
            {
                while (inputBufferAi.Count > 0)
                {
                    if (exit) return;
                    var ai = inputBufferAi.Dequeue();
                    AddAiDefectInner(ai, drawDefect);
                }
            }
            //이전공정 추가 @ATW 240318
            lock (inputBufferPrev)
            {
                while (inputBufferPrev.Count > 0)
                {
                    if (exit) return;
                    var prev = inputBufferPrev.Dequeue();
                    AddPrevDefectInner(prev, drawDefect);
                }
            }
        }

        public void AddSerie(int serieIndex, SerieParameter param)
        {
            this.defects.AddSerie(serieIndex, param);
        }
        /// <summary>
        /// useQueue = true경우 UI를 직접접근하지 않음. false인 경우, UI를 직접접근
        /// useQueue = true경우 Thread에서 직접 접근하지 않도록 주의
        /// </summary>
        /// <param name="defect">결점</param>
        /// <param name="draw">즉시 draw</param>
        public void AddDefect(RollmapDefect defect, bool draw = true)
        {
            if(useQueue)
            {
                lock (inputBuffer)
                {
                    inputBuffer.Enqueue(defect);
                    drawDefect = draw;
                }
            }
            else
            {
                AddDefectInner(defect, draw);
            }
        }
        public void AddDefectInner(RollmapDefect defect, bool draw)
        {
            defects.AddPoint(defect);

            if (drawDefect)
            {
                var param = defects.GetParameter(defect.Serie);
                if (param != null && defects.GetViewType(defect.Serie))
                {
                    if (defects.DefectViewMode == tagDefectViewMode.TypeMarking)
                    {
                        if (!defect.Marking) return;
                    }
                    else if (defects.DefectViewMode == tagDefectViewMode.Level)
                    {
                        if (!defects.GetViewLevel(defect.Level)) return;
                    }
                    else if (defects.DefectViewMode == tagDefectViewMode.LevelMarking)
                    {
                        if (!defects.GetViewLevel(defect.Level)) return;
                        if (!defect.Marking) return;
                    }
                    else if (defects.DefectViewMode == tagDefectViewMode.LevelMarked)
                    {
                        return;
                    }
                    else if (defects.DefectViewMode == tagDefectViewMode.LevelMissed)
                    {
                        if (!defects.GetViewLevel(defect.Level)) return;
                        if (defect.Marked > 0) return;
                    }
                    else if (defects.DefectViewMode == tagDefectViewMode.AiClass)
                    {
                        if (!defects.GetViewLevel(defect.Level)) return;
                        if (!defects.GetViewAiClass(defect.AiClass)) return;
                    }
                    else if (defects.DefectViewMode == tagDefectViewMode.AiClassMarking)
                    {
                        if (!defects.GetViewLevel(defect.Level)) return;
                        if (!defects.GetViewAiClass(defect.AiClass)) return;
                        if (!defect.Marking) return;
                    }

                    _hwndRenderTarget.BeginDraw();
                    if (param.Symbol == "Rectangle")
                    {
#if USE_SHARP_DX
                        SharpDX.DirectWrite.TextLayout txtLayout = null;
#else
                        DWriteTextLayout txtLayout = null;
#endif
                        var color = (useColor2 == true) ? (defect.Level > 0 ? param.Color2 : param.Color) :                     //강불량은 Color, 약불량은 Color2
                        ((defects.ShowDefectColor == false && defect.Marking == false) ? defects.NonMarkColor : param.Color);   //마킹불량은 Color, 미마킹은 nonMarkColor
                        if (defect.ClickDisable) color = Color.Gray;
                        
                        //Text정보 있으면 표시
                        if (!string.IsNullOrWhiteSpace(defect.Info))
                            txtLayout = defects.GetTextLayout(_dWriteFactory, _textFormat, defect.Info);

                        defect.DrawRectangle(_hwndRenderTarget, txtLayout, _colorBrush, this.ViewRealRect, this.viewRect, defect.Info, param.Font, color);
                    }
                    else
                    {
                        var color = (useColor2 == true) ? (defect.Level > 0 ? param.Color2 : param.Color) :                     //강불량은 Color, 약불량은 Color2
                        ((defects.ShowDefectColor == false && defect.Marking == false) ? defects.NonMarkColor : param.Color);   //마킹불량은 Color, 미마킹은 nonMarkColor
                        if (defect.ClickDisable) color = Color.Gray;
                        var symbol = param.Symbol;

                        //주기결점 처리
                        if (showPeriod)
                        {
                            var period = defect.DefectToPeriod10Level(defect.Dclass);
                            if (period > 0)
                                symbol = period.ToString();

                        }
                        if (usePeriodClassification)
                        {
                            var period = defect.DefectToPeriod10Level(defect.Dclass);
                            var optic = param.Optic;
                            var cycleindex = this.cycleParam.IndexOf(period);
                            if (period > 0 && optic >= 0 && optic < showPeriodDefect.GetLength(0))
                            {
                                if (cycleindex == -1)
                                {
                                    if (!showPeriodDefect[optic, this.cycleParam.Count - 1]) return;
                                }
                                else
                                {
                                    if (!showPeriodDefect[optic, cycleindex]) return;
                                }
                            }
                        }
                        
                        var txtLayout = defects.GetTextLayout(_dWriteFactory, _textFormat, symbol);
                        defect.Draw(_hwndRenderTarget, txtLayout, _colorBrush, this.ViewRealRect, this.viewRect, symbol, param.Font, color);
                    }
                    _hwndRenderTarget.EndDraw();
                }
            }
        }
        public void AddMarkedDefect(RollmapDefect defect, bool draw = true)
        {
            if (useQueue)
            {
                lock (inputBufferMarked)
                {
                    inputBufferMarked.Enqueue(defect);
                }
            }
            else
            {
                AddMarkedDefectInner(defect, draw);
            }
        }
        private void AddMarkedDefectInner(RollmapDefect defect, bool draw = true)
        {
            defects.AddPoint(defect);

            if (draw)
            {
				var param = defects.GetParameter(defect.Serie);
                if (param != null && defects.GetViewType(defect.Serie))
                {
                    if (defects.DefectViewMode == tagDefectViewMode.LevelMarked)
                    {
                        if (!defects.GetViewLevel(defect.Level)) return;
                    }
                    else
                    {
                        return;
                    }
					_hwndRenderTarget.BeginDraw();
                    var symbol = param.Symbol;

                    //주기결점 처리
                    if (showPeriod)
                    {
                        var period = defect.DefectToPeriod10Level(defect.Dclass);
                        if (period > 0)
                            symbol = period.ToString();
                    }
                    if (usePeriodClassification)
                    {
                        var period = defect.DefectToPeriod10Level(defect.Dclass);
                        var optic = param.Optic;
                        var cycleindex = this.cycleParam.IndexOf(period);
                        if (period > 0 && optic >= 0 && optic < showPeriodDefect.GetLength(0))
                        {
                            if (cycleindex == -1)
                            {
                                if (!showPeriodDefect[optic, this.cycleParam.Count - 1]) return;
                            }
                            else
                            {
                                if (!showPeriodDefect[optic, cycleindex]) return;
                            }
                        }
                    }
                    var txtLayout = defects.GetTextLayout(_dWriteFactory, _textFormat, symbol);
                    defect.Draw(_hwndRenderTarget, txtLayout, _colorBrush, this.ViewRealRect, this.viewRect,
						param.Symbol, param.Font, param.Color);
					_hwndRenderTarget.EndDraw();
				}
            }
        }
        //ai id 중복으로 인해 확인용 로그 추가 @ATW 241107
        public bool AddAiDefect(RollmapDefect defect, bool draw = true)
        {
            if (useQueue)
            {
                lock (inputBufferAi)
                {
                    if (inputBufferAi.Contains(defect)) return false;
                    inputBufferAi.Enqueue(defect);
                }
            }
            else
            {
                AddAiDefectInner(defect, draw);
            }
            return true;
        }
        private void AddAiDefectInner(RollmapDefect defect, bool draw = true)
        {
            if (defect.Id >= 0 && defect.AiClass > 0)
                defects.SetAiClass(defect.Id, defect.AiClass - 1);

            if (draw)
            {
                var param = defects.GetParameter(defect.Serie);
                if (param != null && defects.GetViewType(defect.Serie))
                {
                    if (defects.DefectViewMode == tagDefectViewMode.AiClass)
                    {
                        if (!defects.GetViewLevel(defect.Level)) return;
                        if (!defects.GetViewAiClass(defect.AiClass)) return;
                    }
                    else if (defects.DefectViewMode == tagDefectViewMode.AiClassMarking)
                    {
                        if (!defects.GetViewLevel(defect.Level)) return;
                        if (!defects.GetViewAiClass(defect.AiClass)) return;
                        if (!defect.Marking) return;
                    }
                    else
                    {
                        return;
                    }

                    _hwndRenderTarget.BeginDraw();
                    var symbol = param.Symbol;

                    var txtLayout = defects.GetTextLayout(_dWriteFactory, _textFormat, symbol);
                    defect.Draw(_hwndRenderTarget, txtLayout, _colorBrush, this.ViewRealRect, this.viewRect,
                        param.Symbol, param.Font, param.Color);
                    _hwndRenderTarget.EndDraw();
                }
            }
        }
        public void AddEdge(Edge edge, bool draw = true)
        {
            edges.Add(edge);
        }

        public void AddEdge(Edge[] edge)
        {
            edges.Add(edge);
        }

        public static int MakeType(int level, bool mark)
        {
            return level * 10 + (mark==true ? 1 : 0);
        }

        public void SetDefectViewType(int serie, bool value)
        {
            defects.SetViewType(serie, value);
            RedrawAll();
        }
        public void SetDefectViewLevel(int level, bool value)
        {
            defects.SetViewLevel(level, value);
            RedrawAll();
        }
        public void SetDefectView(tagDefectViewMode viewMode)
        {
            defects.DefectViewMode = viewMode;
            RedrawAll();
        }
        public void SetDefectViewAiClass(int aiClass, bool value)
        {
            defects.SetViewAiClass(aiClass, value);
            RedrawAll();
        }


        public void SetSheetXRange(int colCount, float stX, float edX)
        {
            sheets.SetSheetXRange(colCount, stX, edX);
        }

        public void ResetSheetSummery()
        {
            sheets.ResetSummery();
        }

        public void AddSheet(int id, int colIdx, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
        {
            sheets.Add(id, colIdx, x1, y1, x2, y2, x3, y3, x4, y4);
        }

        public void SetSheetState(int idx, bool isNg)
        {
            sheets.SetStatus(idx, isNg);
        }

        public void RemoveAllSheet()
        {
            sheets.RemoveAll();
        }

        public void DisableDrawSheets(bool isDisable)
        {
            sheets.DisableDraw = isDisable;
        }

        public MapViewOptionArgs MapViewOption()
        {
            return new MapViewOptionArgs(mapViewOption.AutoScroll, mapViewOption.mapViewRange);
        }
        public void MapViewOption(bool autoScroll)
        {
            SetMapViewOption(autoScroll, mapViewOption.mapViewRange);
        }
        public void MapViewOption(MapViewOptionArgs.tagMapViewRange value)
        {
            int x = 0;
            int y = 0;
            if (!lastClicked.IsEmpty)
            {
                x = lastClicked.X;
                y = lastClicked.Y;
            }
            SetMapViewOption(mapViewOption.AutoScroll, value, x, y);
        }
        public void MapViewOption(MapViewOptionArgs value, Rectangle viewRealRect, bool update)
        {
            if (viewRealRect != Rectangle.Empty)
            {
                ViewRealRect = viewRealRect;
                //Trace.WriteLine(String.Format("2 {0},{1},{2},{3}", viewImageRect.Left, viewImageRect.Top, viewImageRect.Right, viewImageRect.Bottom));
            }
            SetMapViewOption(value.AutoScroll, value.mapViewRange, forceUpdate: update);
        }
        public void MapViewOption(MapViewOptionArgs.tagMapViewRange value, Point gdi)
        {
            SetMapViewOption(mapViewOption.AutoScroll, value, gdi.X, gdi.Y);
        }
        private void MapViewOption(MapViewOptionArgs value)
        {
            SetMapViewOption(value.AutoScroll, value.mapViewRange);
        }
        public void MapViewOption(float zoom)
        {
            int x = 0;
            int y = 0;
            if (!lastClicked.IsEmpty)
            {
                x = lastClicked.X;
                y = lastClicked.Y;
            }

            SetMapViewOption(mapViewOption.AutoScroll, MapViewOptionArgs.tagMapViewRange.Zoom, x, y, zoom);
        }

        public void OnViewAreaChanged(object sender, MapViewOptionArgs viewOption, Rectangle rect)
        {
            var viewRect = ConvertRealToView(rect);

            if (mapViewOption.mapViewRange == viewOption.mapViewRange)
                return;

            var automap = mapViewOption.AutoScroll;
            mapViewOption = new MapViewOptionArgs(automap, viewOption.mapViewRange); ;
            if(mapViewOption.mapViewRange == MapViewOptionArgs.tagMapViewRange.Fit)
                ViewRealRect = rect;
            else
                ViewRealRect = new Rectangle(0, 0, WholeWidth, WholeHeight);
            RedrawAll();
        }
        public void RedrawAll(int y = 0)
        {
            UpdateViewRect(false, 0, y);
        }

        public void RemoveDefectRange(int sttIndex, int endIndex, int sttX, int endX, int sttY, int endY)
        {
            defects.RemoveRange(sttIndex, endIndex, sttX, endX, sttY, endY);
            RedrawAll();
        }
        public void RemainDefectRange(int sttIndex, int endIndex, int sttX, int endX, int sttY, int endY)
        {
            defects.RemainRange(sttIndex, endIndex, sttX, endX, sttY, endY);
            RedrawAll();
        }

        public void RemoveAll()
        {
            DisableDraw = true;
            defects.RemoveAll();
            sheets.ResetSummery();
            lock (inputBuffer) { inputBuffer.Clear(); }
            lock (inputBufferMarked) { inputBufferMarked.Clear(); }
            lock (inputBufferAi) { inputBufferAi.Clear(); }
            lock (inputBufferPrev) { inputBufferPrev.Clear(); } //이전공정 추가 @ATW 240318
            edges.Clear();
            ResetGraphics();
            DisableDraw = false;
            UpdateDraw();
            //Invalidate();
        }

        /// <summary>
        /// 마우스 클릭 좌표기준으로 허용범위내(defectSearchRange) 최근접 결점을 찾음
        /// </summary>
        /// <param name="real">mm단위</param>
        /// <param name="gdi">pixel단위</param>
        /// <param name="mark">찾은 최근접결점 표시를 바꿈</param>
        /// <returns>최근접결점. id==-1은 못찾음</returns>
        public RollmapDefect GetNearestDefect(Point real, Point gdi, bool mark)
        {
            return defects.GetNearestDefect(_hwndRenderTarget, _dWriteFactory, _textFormat, _colorBrush, real, gdi, ViewRealRect, viewRect, defectSearchRange, mark);
        }
        //결점영상창에서 좌표로만 롤맵에 표시하기위해 추가 @ATW 231011
        public RollmapDefect GetNearestDefect(Point real, bool mark)
        {
            return defects.GetNearestDefect(_hwndRenderTarget, _dWriteFactory, _textFormat, _colorBrush, real, ConvertRealToView(real), ViewRealRect, viewRect, defectSearchRange, mark);
        }
        /// <summary>
        /// 마우스 클릭 좌표기준으로 허용범위내(defectSearchRange) optic별로 최근접 결점들을 찾음
        /// </summary>
        /// <param name="real">mm단위</param>
        /// <param name="gdi">pixel단위</param>
        /// <param name="mark">찾은 최근접결점 표시를 바꿈</param>
        /// <returns>최근접결점. id==-1은 못찾음</returns>
        public Dictionary<int, RollmapDefect> GetNearestDefects(Point real, Point gdi, bool mark)
        {
            return defects.GetNearestDefects(_hwndRenderTarget, _dWriteFactory, _textFormat, _colorBrush, real, gdi, ViewRealRect, viewRect, defectSearchRange, mark);
        }

        public void DrawRectangle(bool constant, bool draw, int x1, int y1, int width, int height, Pen pen)
        {
            var graphic = new RollmapGraphics(RollmapGraphics.GraphicType.Rect, x1, y1, width, height, pen);
            lock (graphicsLock)
            {
                graphics.Clear();
                if (constant) graphics.Add(graphic);

                if (draw && ShowGraphics)
                {
                    //using (var g = CreateGraphics())
                    {
                        _hwndRenderTarget.BeginDraw();
#if USE_SHARP_DX
                        //graphic.Draw(_hwndRenderTarget, _colorBrush, ViewRealRect, viewRect, this.Width, this.Height);
                        graphic.Draw(_hwndRenderTarget, _colorBrush, ViewRealRect, viewRect, this.Width, height);
#else
                    graphic.Draw(_hwndRenderTarget, _colorBrush, ViewRealRect, viewRect, this.Width, this.Height);
#endif
                        _hwndRenderTarget.EndDraw();
                    }
                }
            }
            RedrawAll();
        }
        public void DrawLine(bool constant, bool draw, int x1, int y1, int x2, int y2, Pen pen)
        {
            var graphic = new RollmapGraphics(RollmapGraphics.GraphicType.Line, x1, y1, x2, y2, pen);
            lock (graphicsLock)
            {
                if(constant) graphics.Add(graphic);
            }
            if(draw && ShowGraphics)
            {
                //using (var g = CreateGraphics())
                {
					_hwndRenderTarget.BeginDraw();
                    graphic.Draw(_hwndRenderTarget, _colorBrush, ViewRealRect, viewRect, this.Width, this.Height);
					_hwndRenderTarget.EndDraw();
                }
            }
        }
        public void DrawText(bool constant, bool draw, int x, int y, string txt, Color color)
        {
            var graphic = new RollmapGraphics(RollmapGraphics.GraphicType.Text, x, y, txt, new Font("굴림", 12.0f), color);
            lock (graphicsLock)
            {
                if (constant)
                {
                    graphics.Add(graphic);
                }
            }
            if (draw && ShowGraphics)
            {
                //using (var g = CreateGraphics())
                {
                    _hwndRenderTarget.BeginDraw();
                    graphic.Draw(_hwndRenderTarget, _colorBrush, ViewRealRect, viewRect, this.Width, this.Height);
                    _hwndRenderTarget.EndDraw();
                }
            }
        }

        private Dictionary<String, String> options = new Dictionary<String, String>();
        private GridDefects defects;
        private MovingRect zoomArea = new MovingRect();
        private GridEdges edges = new GridEdges();
        private bool isDoubleClicked;
        private MapViewOptionArgs mapViewOption = new MapViewOptionArgs(false, MapViewOptionArgs.tagMapViewRange.Whole);
        private int mapViewRange = 4000 * 1000;
        private Point lastClicked = Point.Empty;
        //결점 드래그로 지우는 영역 설정하기 위한 변수 추가 @ATW 231012
        private Point firstClicked = Point.Empty; 
        private MovingRect deleteArea = new MovingRect();

        private GridSheet sheets = null;
        
#region ConvertCoordinate
        private Point ConvertViewToReal(Point view)
        {
            return new Point((int)(ViewRealRect.X + (double)view.X / viewRect.Width * ViewRealRect.Width), 
                (int)(ViewRealRect.Y + (double)view.Y / viewRect.Height * ViewRealRect.Height));
        }

        private Rectangle ConvertViewToReal(Rectangle view)
        {
            return new Rectangle(
                (int)((double)view.X / viewRect.Width * ViewRealRect.Width), (int)((double)view.Y / viewRect.Height * ViewRealRect.Height),
                (int)((double)view.Width / viewRect.Width * ViewRealRect.Width), (int)((double)view.Height / viewRect.Height * ViewRealRect.Height));
        }
        private Point ConvertRealToView(Point real)
        {
            if (ViewRealRect.Width == 0 || ViewRealRect.Height == 0) return new Point(0, 0);

            return new Point((int)((double)real.X / ViewRealRect.Width * viewRect.Width),
                (int)((double)real.Y / ViewRealRect.Height * viewRect.Height));
        }
        private Rectangle ConvertRealToView(Rectangle real)
        {
            if (ViewRealRect.Width == 0 || ViewRealRect.Height == 0) return new Rectangle(0, 0, 0, 0);

            return new Rectangle(
                (int)((double)real.X / ViewRealRect.Width * viewRect.Width), (int)((double)real.Y / ViewRealRect.Height * viewRect.Height),
                (int)((double)real.Width / ViewRealRect.Width * viewRect.Width), (int)((double)real.Height / ViewRealRect.Height * viewRect.Height));
        }
#endregion


        private void SetGuiThema(string value)
        {
			if ( value == Color.Black.ToString() )
			{
				if ( UseGradation )
				{
					frames.BgBottomColor = Color.Black;
					frames.BgTopColor = Color.FromArgb(unchecked((int)0xFF303030));
				}
				else
				{
					frames.BgBottomColor = Color.Black;
					frames.BgTopColor = Color.Black;
				}
			}
			else if ( value == Color.White.ToString() )
			{
				frames.BgBottomColor = Color.White;
				frames.BgTopColor = Color.White;
			}
			UpdateDraw();
			//Invalidate();
		}

        
        private void UpdateDraw()
        {
            if (this.InvokeRequired)
			{
                this.BeginInvoke(new Action(() =>
                {
                    UpdateWindowViewRect();
                    DrawMap();
                }
                ));
			}
			else
			{
                UpdateWindowViewRect();
                DrawMap();
			}
		}

        static float[] dashes = { 1.0f, 1.0f, 1.0f };
        private void DrawMap()
		{
            if (DisableDraw) return;
			try
			{
				if (_hwndRenderTarget == null) return;

				if (_hwndRenderTarget.Size.Width != this.Width || _hwndRenderTarget.Size.Height != this.Height)
				{
#if USE_SHARP_DX
                    if (_hwndRenderTarget is WindowRenderTarget)
    					(_hwndRenderTarget as WindowRenderTarget).Resize(new SharpDX.Size2(this.Width, this.Height));

#else
                    if (_hwndRenderTarget is D2D1HwndRenderTarget)
    					(_hwndRenderTarget as D2D1HwndRenderTarget).Resize(new D2D1SizeU((uint)this.Width, (uint)this.Height));
#endif
                }

#if USE_SHARP_DX
                if (captureRender)
                {
                    if (_wicBitmap.Size.Width != this.Width || _wicBitmap.Size.Height != this.Height)
                    {
                        _wicBitmap.Dispose();
                        _wicRenderTarget.Dispose();

                        _wicBitmap = new SharpDX.WIC.Bitmap(_wicFactory, this.Width, this.Height, SharpDX.WIC.PixelFormat.Format32bppBGR, BitmapCreateCacheOption.CacheOnLoad);
                        //_wicBitmap = new SharpDX.WIC.Bitmap(_wicFactory, 787, 693, SharpDX.WIC.PixelFormat.Format32bppBGR, BitmapCreateCacheOption.CacheOnLoad);

                        var wicRenderTargetProperties = new RenderTargetProperties(RenderTargetType.Default,
                            new SharpDX.Direct2D1.PixelFormat(SharpDX.DXGI.Format.Unknown,
                                SharpDX.Direct2D1.AlphaMode.Unknown), 0, 0, RenderTargetUsage.None,
                                SharpDX.Direct2D1.FeatureLevel.Level_DEFAULT);

                        _wicRenderTarget = new WicRenderTarget(_factory, _wicBitmap, wicRenderTargetProperties);
                        _wicRenderTarget.AntialiasMode = AntialiasMode.PerPrimitive;
                    }
                }

                SharpDX.Direct2D1.RenderTarget renderTarget = _hwndRenderTarget;
                SharpDX.Direct2D1.SolidColorBrush colorBrush = _colorBrush;
                
                if (captureRender)
                {
                    renderTarget = _wicRenderTarget;
                    colorBrush = _wicColorBrush;
                }
#else
                D2D1RenderTarget renderTarget = _hwndRenderTarget;
#endif
                renderTarget.BeginDraw();
#if USE_SHARP_DX
                if (captureRender)
                    renderTarget.Clear(ColorConverter.ColorToRawColor(Color.White));
                else
                    renderTarget.Clear(ColorConverter.ColorToRawColor(Color.Black));
#else
                renderTarget.Clear(new D2D1ColorF(D2D1KnownColor.Black));
#endif

                frames.Draw(_factory, renderTarget, ViewRealRect, viewRect, this.Width, this.Height, ShowGridLine, ShowGridLineMeter, showOS);
                if (ShowDefect) defects.Draw(renderTarget, _dWriteFactory, _textFormat, colorBrush, ViewRealRect, viewRect, this.ParentForm.Name, defectOffsetX);
                sheets.Draw(renderTarget, _factory, ViewRealRect, viewRect);

                if (ShowOpticDistance)
                {
                    //ShowOpticDistance 적용시
                    if (mapViewRange >= 20 * 1000 && mapViewRange <= 100 * 1000)
                    {
                        for (int i = 0; i < this.OpticDistance.Length; i++)
                        {
                            if (OpticDistance[i] > 0)
                            {
                                using (var brush = new SolidColorBrush(_hwndRenderTarget, ColorConverter.ColorToRawColor(this.OpticColor[i])))
                                {
                                    //var dist = this.OpticDistance[i] - this.OpticDistance[StdOptic];
                                    //frames.DrawOpticDistance(renderTarget, _textFormat, brush, ViewRealRect, viewRect, this.Width, this.Height, MapAutoPosion - dist);
                                    if (ShowOsDsPosX && i == StdOptic)
                                        frames.DrawOpticDistance(renderTarget, _textFormat, brush, ViewRealRect, viewRect, this.Width, this.Height, MapAutoPosion - this.OpticDistance[i], this.OsPosX, this.DsPosX);
                                    else
                                        frames.DrawOpticDistance(renderTarget, _textFormat, brush, ViewRealRect, viewRect, this.Width, this.Height, MapAutoPosion - this.OpticDistance[i]);
                                }
                            }
                        }
                    }
                }
                else if (ShowOsDsPosX)
                {
                    frames.DrawOsDsPosX(renderTarget, _textFormat, colorBrush, ViewRealRect, viewRect, this.Width, this.Height, this.OsPosX, this.DsPosX, MapAutoPosion);
                }

                if ( ShowEdge ) edges.Draw(renderTarget, _factory, colorBrush, ViewRealRect, viewRect);
                zoomArea.Draw(renderTarget);
                //결점제거창 on 상태에서만 그리도록 수정 @ATW 231013
                if (UseDefectDelete)
                {
                    var rPt = ConvertViewToReal(deleteArea.GetRect());
                    if (rPt.Width > 50 && rPt.Height > 50)
                        deleteArea.Draw(renderTarget);
                }
                if ( ShowGraphics )
				{
					lock ( graphicsLock )
					{
						graphics.ForEach(each => each.Draw(renderTarget, colorBrush, ViewRealRect, viewRect, this.Width, this.Height, this.Name != "rollmap" ? true : false));
					}
				}
                renderTarget.EndDraw();
#if USE_SHARP_DX
                if (captureRender)
                {
                    int width = _wicBitmap.Size.Width;
                    int height = _wicBitmap.Size.Height;
                    System.Drawing.Bitmap gdiBitmap = new System.Drawing.Bitmap(width, height, System.Drawing.Imaging.PixelFormat.Format32bppPArgb);

                    System.Drawing.Imaging.BitmapData gdiBitmapData = gdiBitmap.LockBits(
                        new System.Drawing.Rectangle(0, 0, gdiBitmap.Width, gdiBitmap.Height),
                        System.Drawing.Imaging.ImageLockMode.ReadWrite,
                        System.Drawing.Imaging.PixelFormat.Format32bppPArgb);

                    int[] buffer = new int[width * height];
                    _wicBitmap.CopyPixels(buffer);
                    System.Runtime.InteropServices.Marshal.Copy(buffer, 0, gdiBitmapData.Scan0, buffer.Length);

                    gdiBitmap.UnlockBits(gdiBitmapData);

                    if (bmpCaptured != null)
                        bmpCaptured.Dispose();

                    bmpCaptured = gdiBitmap;

                    captureRender = false;
                    Captured = true;
                }
#endif
            }
            catch (Exception ex)
			{
                Trace.WriteLine($"Error : " +  ex.ToString());
			}
        }

		private void SetMapViewOption(bool autoScroll, MapViewOptionArgs.tagMapViewRange viewRange,
                                       int x = 0, int y = 0, float zoom = 1.0f, bool forceUpdate = false)
        {
            if (mapViewOption.AutoScroll != autoScroll || mapViewOption.mapViewRange != viewRange || zoom != 1.0f || forceUpdate)
            {
                int range = 0;
                switch (viewRange)
                {
                    case MapViewOptionArgs.tagMapViewRange.View200mm:
                        range = 200;
                        break;
                    case MapViewOptionArgs.tagMapViewRange.View1M:
                        range = 1 * 1000;
                        break;
                    case MapViewOptionArgs.tagMapViewRange.View10M:
                        range = 10 * 1000;
                        break;
                    case MapViewOptionArgs.tagMapViewRange.View20M:
                        range = 20 * 1000;
                        break;
                    case MapViewOptionArgs.tagMapViewRange.View100M:
                        range = 100 * 1000;
                        break;
                    case MapViewOptionArgs.tagMapViewRange.View200M:
                        range = 200 * 1000;
                        break;
                    case MapViewOptionArgs.tagMapViewRange.View500M:
                        range = 500 * 1000;
                        break;
                    case MapViewOptionArgs.tagMapViewRange.View1000M:
                        range = 1000 * 1000;
                        break;
                    case MapViewOptionArgs.tagMapViewRange.Whole:
                        range = WholeHeight;
                        break;
                    case MapViewOptionArgs.tagMapViewRange.Zoom:
                        range = (int)(mapViewRange * zoom);
                        if (range > WholeHeight)
                            range = WholeHeight;
                        else if (range < 200)
                            return;
                        break;
                    case MapViewOptionArgs.tagMapViewRange.Fit:
                        range = MapManualRange;
                        break;
                    default:
                        range = WholeHeight;
                        break;
                }
                mapViewOption = new MapViewOptionArgs(autoScroll, viewRange);
                mapViewRange = range;
                UpdateViewRect(true, x, y, zoom: zoom);
            }
        }
        MapViewOptionArgs.tagMapViewRange prevRange = MapViewOptionArgs.tagMapViewRange.View10M;
        private void UpdateViewRect(bool changed = false, int x = 0, int y = 0, bool vscrollUpdating = false, float zoom = 1.0f)
        {
            if (defects == null) return;

            int range = mapViewRange;
            
            if (mapViewOption.mapViewRange == MapViewOptionArgs.tagMapViewRange.Whole)
            {
                ViewRealRect = new Rectangle(0, y, WholeWidth, WholeHeight);
            }
            else if(mapViewOption.mapViewRange == MapViewOptionArgs.tagMapViewRange.Fit)
            {
                ViewRealRect = new Rectangle(0, MapManualStartPos, WholeWidth, MapManualRange);
            }
            else
            {
                if (mapViewOption.AutoScroll)
                {
                    var destRealRect = new Rectangle(0, 0, WholeWidth, WholeHeight);
                    var scaleY = (double)viewRect.Height / mapViewRange;
                    int viewOffset = (int)(automapViewOffsetY / scaleY + 0.5);

                    destRealRect.Y = MapAutoPosion - mapViewRange + viewOffset;
                    destRealRect.Height = mapViewRange;
                    ViewRealRect = destRealRect;
                }
                else
                {
                    if (x > 0 && y > 0 || changed == true)
                    {
                        var clicked = ConvertViewToReal(new Point(x, y));
                        var destRealRect = new Rectangle(0, 0, WholeWidth, WholeHeight);
                        destRealRect.Y = clicked.Y - (int)((double)y / this.Size.Height * range);
                        destRealRect.Height = mapViewRange;
                        ViewRealRect = destRealRect;
                    }
                }
            }
            //< LOT > 버튼 클릭스 크로스 스레드 예외 발생 수정
            //@230917 YKS
            if (vScrollBar1.InvokeRequired)
            {
                vScrollBar1.BeginInvoke(new Action(() => {
                    UpdateVScroll(vscrollUpdating);
                }));
            }
            else
            {
                UpdateVScroll(vscrollUpdating);
            }

            //Invalidate();
            UpdateDraw();
            SyncRollmapEtc?.Invoke(false, mapViewOption.mapViewRange, x, y, zoom);
		}

        private void UpdateVScroll(bool vscrollUpdating)
        {
            //Scroll 처리
            if (useVscroll == true)
            {
                if(vscrollUpdating == false)
                {
                    if (prevRange != mapViewOption.mapViewRange)
                    {
                        if (mapViewOption.mapViewRange != MapViewOptionArgs.tagMapViewRange.Whole && mapViewOption.mapViewRange != MapViewOptionArgs.tagMapViewRange.Fit)
                        {
                            vScrollBar1.Minimum = 0;
                            vScrollBar1.Maximum = WholeHeight;
                            vScrollBar1.Show();
                        }
                        else
                        {
                            vScrollBar1.Hide();
                        }
                        prevRange = mapViewOption.mapViewRange;
                    }
                    //스크롤바 막대는 ViewRealRect의 Bottom을 기준으로 한다.
                    var value = ViewRealRect.Bottom;
                    if (value < vScrollBar1.Minimum) value = vScrollBar1.Minimum;
                    if (value > vScrollBar1.Maximum) value = vScrollBar1.Maximum;
                    vScrollBar1.Value = value;
                }
            }
            else
            {
                if (vScrollBar1.Visible) vScrollBar1.Hide();
            }
            //Scroll 처리 끝
        }

        private void Rollmap_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            var rPt = ConvertViewToReal(e.Location);
            if (MouseDoubleClicked != null) MouseDoubleClicked(rPt, e.Location, e.Button);

            lastClicked = e.Location;
        }


        private const int WM_KEYDOWN = 0x100;
        private const int WM_SYSKEYDOWN = 0x104;
        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            try
            {
                if (msg.Msg == WM_KEYDOWN || msg.Msg == WM_SYSKEYDOWN)
                {
                    switch (keyData)
                    {
                        case Keys.PageUp:
                        case Keys.Up:
                            RollmapUpDown(true);
                            break;
                        case Keys.PageDown:
                        case Keys.Down:
                            RollmapUpDown(false);
                            break;
                    }
                }
            }
            catch (Exception ex)
            {
                Console.Write("exception : " + ex.Message);
            }
            return base.ProcessCmdKey(ref msg, keyData);
        }

        private void RollmapUpDown(bool up)
        {
            //위아래 이동 (스크롤과 동일)
            if (mapViewOption.mapViewRange != MapViewOptionArgs.tagMapViewRange.Whole)
            {
                var rect = ViewRealRect;
                rect.Offset(0, up? -1 * ViewRealRect.Height / 2 : ViewRealRect.Height / 2);

                int oldH = (int)rect.Height;
                if (rect.Top< 0)
                {
                    rect.Y = 0;
                    rect.Height = oldH;
                }
                if (rect.Bottom > WholeHeight)
                {
                    rect.Y = WholeHeight - oldH;
                }
                ViewRealRect = rect;
                UpdateViewRect();

                if (ViewAreaChanged != null)
                {
                    ViewAreaChanged(this, mapViewOption, ViewRealRect);
                }

                RollmapUpDownChanged(up);
            }
        }


        private void Rollmap_MouseScroll(object sender, MouseEventArgs e)
        {
            //롤맵 양쪽 바코드 및 특이사항, 결점다발구간 표시 추가 @ATW 231220
            if (this.Name != "rollmap") return;
            if ((Control.ModifierKeys & Keys.Control) == Keys.Control)
            {
                //확대 축소
                int x = 0;
                int y = 0;
                float zoom = 1.0f;
                if (e.Delta > 0)
                {
                    zoom *= 0.5f; //확대
                }
                else
                {
                    zoom *= 2.0f; //축소
                }
                if (!lastClicked.IsEmpty)
                {
                    x = lastClicked.X;
                    y = lastClicked.Y;
                }
                SetMapViewOption(mapViewOption.AutoScroll, MapViewOptionArgs.tagMapViewRange.Zoom, x, y, zoom);
            }
            else
            {
                //위아래 이동
                RollmapUpDown(e.Delta > 0 ? true : false);
            }
        }

        private void Rollmap_MouseDown(object sender, MouseEventArgs e)
        {
            isDoubleClicked = false;

            if (e.Button == MouseButtons.Left)
            {
                if (ModifierKeys == Keys.ControlKey)
                {
                    zoomArea.Show = true;
                    zoomArea.Moving = true;
                    var rect = zoomArea.GetRect();
                    zoomArea.SetRect(e.Location.X, e.Location.Y, rect.Right, rect.Bottom);
                }
                else if(UseDefectDelete)
                {
                    deleteArea.Show = true;
                    deleteArea.Moving = true;
                    //deleteArea.SetRect(e.Location.X, e.Location.Y, 0, 0);
                    firstClicked = e.Location;
                }
            }
        }

        private void Rollmap_MouseUp(object sender, MouseEventArgs e)
        {
            if (zoomArea.Moving && ModifierKeys == Keys.ControlKey && e.Button == MouseButtons.Left && !isDoubleClicked 
                && Math.Abs(zoomArea.GetRect().Height) > 10)
            {
                zoomArea.Moving = false;
                var rect = ConvertViewToReal(zoomArea.GetRect());

                //최소 표시 크기 실좌표 폭 100mm, 높이 1000mm
                if (rect.Width < 100)
                    rect.Width = 100;

                if (rect.Height < 1000)
                    return;
  
                ViewRealRect = rect;

                RedrawAll();
            }
            else if (deleteArea.Moving && e.Button == MouseButtons.Left && !isDoubleClicked)
            {
                var rPt = ConvertViewToReal(deleteArea.GetRect());
                if (CustomMouseUp != null && (rPt.Width > 50 && rPt.Height > 50)) CustomMouseUp(rPt);
                deleteArea.Moving = false;
                RedrawAll();
                deleteArea.SetRect(0, 0, 0, 0);
            }
            else if(e.Button == MouseButtons.Left)
            {
                //롤맵 X축 Offset 추가 @ATW 240409
                var location = new Point(e.Location.X - defectOffsetX, e.Location.Y);
                var rPt = ConvertViewToReal(location);
                this.defects.DefectOffsetX = defectOffsetX;
                if (MouseClicked != null) MouseClicked(rPt, location, e.Button);
            }

            lastClicked = e.Location;
        }

        private void Rollmap_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left && zoomArea.Moving)
            {
                var rect = zoomArea.GetRect();
                zoomArea.SetRect(rect.Left, rect.Top, e.Location.X, e.Location.Y);
            }
            else if(e.Button == MouseButtons.Left && deleteArea.Moving)
            {
                int x = e.Location.X, y = e.Location.Y;
                if (e.Location.X < 0) x = 0;
                else if (e.Location.X > Width) x = Width;

                if (e.Location.Y < 0) y = 0;
                else if (e.Location.Y > Height) y = Height;
                deleteArea.SetRect(firstClicked.X, firstClicked.Y, x, y);
            }
        }

        private void Rollmap_Resize(object sender, EventArgs e)
        {
            UpdateDraw();   //Invalidate();
		}

        protected override void OnPaintBackground(PaintEventArgs e)
        {
            UpdateDraw();
        }

		private void vScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            var diff = (e.NewValue - e.OldValue);
            if (diff == 0) return;

            //스크롤바 막대는 ViewRealRect의 Bottom을 기준으로 한다.
            var rect = new Rectangle(ViewRealRect.Left, e.NewValue - ViewRealRect.Height, ViewRealRect.Width, ViewRealRect.Height);

            int oldH = (int)rect.Height;
            if (rect.Top < 0)
            {
                rect.Y = 0;
                rect.Height = oldH;
            }
            if (rect.Bottom > WholeHeight)
            {
                rect.Y = WholeHeight - oldH;
            }
            ViewRealRect = rect;
            UpdateViewRect(vscrollUpdating: true);
        }
        /// <summary>
        /// 종스크롤바 표시 여부에 따라 롤맵을 그려주는 영역을 재설정
        /// </summary>
        private void UpdateWindowViewRect()
        {
            var temp = new RectangleF(OffsetX, OffsetY, this.Width - OffsetX * 2, this.Height - OffsetY * 2);
            if (vScrollBar1.Visible)
            {
                temp.Width = temp.Width - vScrollBar1.Width;
            }
            viewRect = temp;
        }

        public void SetComparePos(double posX, double posY, int optic, Color color, int cam, double size, string path, double calcX, double calcY, double sttPos, string date)
        {
            if (defects != null)
            {
                defects.SetNelPos(posX, posY, optic, color, cam, size, path, calcX, calcY, sttPos, date);
            }
        }

        public void ResetComparePos()
        {
            if (defects != null)
            {
                defects.ResetNelPos();
            }
        }

        public List<NelCompareDefect> GetComparePos()
        {
            if (defects != null)
                return defects.GetNelPos();
            else
                return null;
        }

        public void SetCompareShowDefect(bool[] value)
        {
            this.CompareShowDefect = value;
        }

        public string[] GetViewDefect()
        {
            if (defects != null)
                return defects.GetNelViewDefect();
            else
                return null;
        }

        public void SetSyncEtcMap(Rectangle rect)
        {
            ViewRealRect = rect;
            UpdateViewRect();
        }

        public void SetEtcMapViewOption(bool autoScroll, MapViewOptionArgs.tagMapViewRange viewRange, int x = 0, int y = 0, float zoom = 1.0f, bool forceUpdate = false)
        {
            SetMapViewOption(autoScroll, viewRange, x, y, zoom, forceUpdate);
        }

        public void SetPrevCompareDefectPos(double posX, double posY)
        {
            if (defects != null)
            {
                defects.SetPrevCompareDefectPos(posX, posY);
            }
        }

        public void ResetPrevCompareDefectPos()
        {
            if (defects != null)
            {
                defects.ResetPrevCompareDefectPos();
            }
        }

        public List<PointF> GetPrevCompareDefectPos()
        {
            if (defects != null)
            {
                return defects.GetPrevCompareDefectPos();
            }
            else
            {
                return null;
            }
        }

        //이전공정 추가 @ATW 240318
        private void AddPrevDefectInner(PrevCompareDefect defect, bool draw = true)
        {
            defects.AddPrevPoint(defect);

            //if (draw)
            //{
            //    //var param = defects.GetParameter(defect.Serie);
            //    //if (defect != null && defects.GetViewType(defect.Serie))
            //    {
            //        _hwndRenderTarget.BeginDraw();
            //        var symbol = "%";

            //        var txtLayout = defects.GetTextLayout(_dWriteFactory, _textFormat, symbol);
            //        defect.Draw(_hwndRenderTarget, txtLayout, _colorBrush, this.ViewRealRect, this.viewRect,
            //            symbol, this.Font, Color.AliceBlue);
            //        _hwndRenderTarget.EndDraw();
            //    }
            //}
        }

        public void AddPrevDefect(PrevCompareDefect defect, bool draw = true)
        {
            if (useQueue)
            {
                lock (inputBufferPrev)
                {
                    inputBufferPrev.Enqueue(defect);
                }
            }
            else
            {
                //AddPrevDefectInner(new RollmapDefect(-1, (float)defect.posx, (float)defect.posy, 0, 0, 0, true, 1), draw);
                AddPrevDefectInner(defect, draw);
            }
        }
    }
}
