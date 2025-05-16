using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Xml.Xsl;

#if USE_SHARP_DX
using SharpDX.WIC;
using SharpDX.Direct2D1;
using SharpDX.DirectWrite;
using SharpDX.Mathematics.Interop;
using SharpDX.IO;
#else
using JeremyAnsel.DirectX.D2D1;
using JeremyAnsel.DirectX.DWrite;
#endif

namespace Coss.Controls
{
    public enum tagDefectViewMode
    {
        Type = 0,
        TypeMarking,
        Level,
        LevelMarking,
        LevelMarked,
        LevelMissed,
        AiClass,
        AiClassMarking,
    }

    public class ColorConverter
    {
        public static uint ColorToUint(Color c)
        {
            uint u = (UInt32)c.A << 24;
            u += (UInt32)c.R << 16;
            u += (UInt32)c.G << 8;
            u += c.B;
            return u;
        }
#if USE_SHARP_DX
        public static RawColor4 ColorToRawColor(Color c)
        {
            return new RawColor4((float)c.R / 255, (float)c.G / 255, (float)c.B / 255, (float)c.A / 255);
        }
#endif
    }

    public class GridDefects
    {
        /// <summary>
        /// 각 serie별 최대 queue 크기
        /// </summary>
        const int kMaxRollmapQueueSize = 5000;
        /// <summary>
        /// Max queue size
        /// </summary>
        const int kMaxTrackingQueueSize = 30000;
        /// <summary>
        /// max optic size
        /// </summary>
        const int kMaxOpticSize = 16;
        /// <summary>
        /// max cycleparam size
        /// </summary>
        const int kMaxCycle = 15;
        /// <summary>
        /// 동일결점 최대 크기
        /// </summary>
        const int kMaxSameDefect = 5;

        const int k10LevelDelimiterNgType = 100;

        /// <summary>
        /// 최대  AI 클래스
        /// </summary>
        const int kMaxAiClasss = 30;

        public bool DisableDraw { get; set; } = false;
        public bool DrawReverse = false;
        private bool useQueue = false;
        public int MarkingSerieIndex { get; set; } = -1;
        private tagDefectViewMode defectViewMode = tagDefectViewMode.LevelMarking;
        public tagDefectViewMode DefectViewMode { get { return defectViewMode; } set { defectViewMode = value; } }
        private bool showDefectColor = false;
        /// <summary>
        /// true면 마킹하는 결점은 원래색상으로, 미마킹결점은 nonMarkColor으로 표시
        /// false면 모두 원래색상으로 표시
        /// </summary>
        public bool ShowDefectColor { get { return showDefectColor; } set { showDefectColor = value; } }

        private System.Drawing.Brush nonMarkBrush;
        private Color nonMarkColor = Color.Gray;
        public Color NonMarkColor { get { return nonMarkColor; } set { nonMarkColor = value; nonMarkBrush = new SolidBrush(nonMarkColor); } }

        private Color clickedColor = Color.White;
        public Color ClickedColor { get { return clickedColor; } set { clickedColor = value; } }

        private bool useColor2 = false;
        public bool UseColor2 { get { return useColor2; } set { useColor2 = value; } }
        /// <summary>
        /// 주기결점 Symbol을 @와 같은 지정된 문자가 아닌, 200과 같은 실주기로 표시
        /// </summary>
        public bool ShowPeriod { get; set; } = false;

        /// <summary>
        /// 주기불량 분류 on/off
        /// </summary>
        public bool UsePeriodClassification { get; set; } = false;
        /// <summary>
        /// 주기불량 분류 구분하기 위한 cycleparam list
        /// </summary>
        public List<int> CycleParam = new List<int>();
        /// <summary>
        /// 주기불량 분류 구분하기 위한 showperioddefect 배열
        /// </summary>
        public bool[,] ShowPeriodDefect = new bool[kMaxOpticSize, kMaxCycle];

        /// <summary>
        /// 동일결점 on/off
        /// </summary>
        public bool UseSameDefect { get; set; } = false;
        /// <summary>
        /// 동일결점 표시
        /// </summary>
        public bool[] ShowSameDefect = new bool[kMaxSameDefect];
        /// <summary>
        /// N사와 좌표비교 on/off
        /// </summary>
        public bool ComparePostion { get; set; } = false;
        public bool[] CompareShowDefect = new bool[kMaxOpticSize];
        /// <summary>
        /// 이전공정 결점비교 On/Off
        /// </summary>
        public bool UsePrevCompare { get; set; } = false;
        /// <summary>
        /// 이전공정 동일결점 범위
        /// </summary>
        public Point PrevCompareSameRange { get; set; }

        private object locker = new object();
        private Dictionary<int, PointsSerie> _series = new Dictionary<int, PointsSerie>();
        private Dictionary<int, PointsSerieQueue> _seriesQueue = new Dictionary<int, PointsSerieQueue>();
        private bool[] _viewLevel = new bool[10];   //10Level
        private bool[] _viewAiClass = new bool[kMaxAiClasss]; //ai클래스
        private List<int> _markedIdQueue = new List<int>();
        private List<int> _optics = new List<int>();
        private List<int> _aiClassIdQueue = new List<int>();
        private Dictionary<int, int> _aiClassIdToClassQueue = new Dictionary<int, int>();
        //N사 좌표비교 데이터
        List<NelCompareDefect> compareDefect = new List<NelCompareDefect>();
        //이전공정 결점비교 데이터
        List<PointF> prevCompareRollmapDefect = new List<PointF>();
        List<PrevCompareDefect> prevDrawPoint = new List<PrevCompareDefect>();
        List<PrevErrorAreaPosition> prevErrorArea = new List<PrevErrorAreaPosition>();

        //롤맵 X축 Offset 추가 @ATW 240409
        public int DefectOffsetX { get; set; } = 0;

        public bool IsAiMode { get; set; } = false;

        public GridDefects(bool useQueue)
        {
            this.useQueue = useQueue;
            this.nonMarkBrush = new SolidBrush(nonMarkColor);
        }

        public void Term()
        {
            foreach (var item in dicTextLayout)
            {
#if USE_SHARP_DX
                item.Value.Dispose();
#else
                item.Value.Release();
				item.Value.Dispose();
#endif
            }
        }

        public void SetViewType(int serie, bool value)
        {
            if (useQueue)
            {
                if (_seriesQueue.ContainsKey(serie))
                {
                    _seriesQueue[serie].IsVisable = value;
                }
            }
            else
            {
                if (_series.ContainsKey(serie))
                {
                    _series[serie].IsVisable = value;
                }
            }
        }
        public bool GetViewType(int serie)
        {
            if (useQueue)
            {
                if (_seriesQueue.ContainsKey(serie))
                {
                    return _seriesQueue[serie].IsVisable;
                }
            }
            else
            {
                if (_series.ContainsKey(serie))
                {
                    return _series[serie].IsVisable;
                }
            }
            return false;
        }
        public void SetViewLevel(int level, bool values)
        {
            _viewLevel[level] = values;
        }
        public bool GetViewLevel(int level)
        {
            return _viewLevel[level];
        }
        public void SetViewAiClass(int aiClass, bool values)
        {
            _viewAiClass[aiClass] = values;
        }
        public bool GetViewAiClass(int aiClass)
        {
            if (aiClass < 0)
                return false;
            return _viewAiClass[aiClass];
        }
        /// <summary>
        /// 설정범위에 포함된 결점을 제거
        /// </summary>
        /// <param name="sttIndex">제거할 _series의 Index, -1이면 모든광학계, 0이상이면 선택된 Seire의 시작 Index</param>
        /// <param name="endIndex">제거할 _series의 Index, -1이면 모든광학계, 0이상이면 선택된 Seire의 마지막 Index</param>
        /// <param name="sttX">mm</param>
        /// <param name="endX">mm</param>
        /// <param name="sttY">mm</param>
        /// <param name="endY">mm</param>
        public void RemoveRange(int sttIndex, int endIndex, int sttX, int endX, int sttY, int endY)
        {
            lock (locker)
            {
                if (useQueue)
                {
                    throw new NotImplementedException();
                }
                else
                {
                    if (sttIndex == -1 || endIndex == -1)
                    {
                        foreach (var serie in _series)
                        {
                            serie.Value.Defects.RemoveAll(e => e.Y >= sttY && e.Y < endY && e.X >= sttX && e.X < endX);
                        }
                    }
                    else
                    {
                        foreach (var serie in _series)
                        {
                            if (sttIndex <= serie.Key && serie.Key <= endIndex)
                            {
                                serie.Value.Defects.RemoveAll(e => e.Y >= sttY && e.Y < endY && e.X >= sttX && e.X < endX);
                            }
                        }
                    }
                }
            }
        }
        /// <summary>
        /// 설정범위에 포함되지 않은 결점을 제거
        /// </summary>
        /// <param name="sttIndex">제거할 _series의 Index, -1이면 모든광학계, 0이상이면 선택된 Seire의 시작 Index</param>
        /// <param name="endIndex">제거할 _series의 Index, -1이면 모든광학계, 0이상이면 선택된 Seire의 마지막 Index</param>
        /// <param name="sttX">mm</param>
        /// <param name="endX">mm</param>
        /// <param name="sttY">mm</param>
        /// <param name="endY">mm</param>
        public void RemainRange(int sttIndex, int endIndex, int sttX, int endX, int sttY, int endY)
        {
            lock (locker)
            {
                if (useQueue)
                {
                    throw new NotImplementedException();
                }
                else
                {
                    if (sttIndex == -1 || endIndex == -1)
                    {
                        foreach (var serie in _series)
                        {
                            serie.Value.Defects.RemoveAll(e => e.X < sttX);
                            serie.Value.Defects.RemoveAll(e => e.X >= endX);
                            serie.Value.Defects.RemoveAll(e => e.Y < sttY);
                            serie.Value.Defects.RemoveAll(e => e.Y >= endY);
                        }
                    }
                    else
                    {
                        foreach (var serie in _series)
                        {
                            if (sttIndex <= serie.Key && serie.Key <= endIndex)
                            {
                                serie.Value.Defects.RemoveAll(e => e.X < sttX);
                                serie.Value.Defects.RemoveAll(e => e.X >= endX);
                                serie.Value.Defects.RemoveAll(e => e.Y < sttY);
                                serie.Value.Defects.RemoveAll(e => e.Y >= endY);
                            }
                        }
                    }
                }
            }
        }

        public void RemoveAll()
        {
            lock (locker)
            {
                if (useQueue)
                {
                    foreach (var serie in _seriesQueue)
                    {
                        serie.Value.Defects.Clear();
                    }
                    _markedIdQueue.Clear();
                    _aiClassIdQueue.Clear();
                    _aiClassIdToClassQueue.Clear();
                }
                else
                {
                    foreach (var serie in _series)
                    {
                        serie.Value.Defects.Clear();
                    }
                    prevDrawPoint.Clear();
                    prevErrorArea.Clear();
                }
            }
            oldClicked.Id = -1;
        }

        public void AddSerie(int serieIndex, SerieParameter param)
        {
            if (useQueue)
            {
                var serie = new PointsSerieQueue(param);
                if (!_seriesQueue.ContainsKey(serieIndex))
                {
                    _seriesQueue.Add(serieIndex, serie);
                }
            }
            else
            {
                var serie = new PointsSerie(param);
                if (!_series.ContainsKey(serieIndex))
                {
                    _series.Add(serieIndex, serie);
                }
            }
            //마킹기 optic = -1는 제외
            if (param.Optic >= 0 && !_optics.Contains(param.Optic))
                _optics.Add(param.Optic);
        }

        public bool AddPoint(RollmapDefect point)
        {
            if (useQueue)
            {
                if (point.Serie < _seriesQueue.Count)
                {
                    lock (locker)
                    {
                        if (kMaxRollmapQueueSize < _seriesQueue[point.Serie].Defects.Count)
                        {
                            _seriesQueue[point.Serie].Defects.Dequeue();
                        }
                        _seriesQueue[point.Serie].Defects.Enqueue(point);
                    }

                    return true;
                }
            }
            else
            {
                if (point.Serie < _series.Count)
                {
                    lock (locker)
                    {
                        _series[point.Serie].Defects.Add(point);
                    }
                    _series[point.Serie].IsDirty = true;

                    return true;
                }
            }
            return false;
        }

        //이전공정 추가 @ATW 240320
        public void AddPrevPoint(PrevCompareDefect defect)
        {
            prevDrawPoint.Add(defect);
        }

        public void AddPrevErrorPoint(PrevErrorAreaPosition point)
        {
            if (prevErrorArea.Contains(point)) return;
            prevErrorArea.Add(point);
        }

        public SerieParameter GetParameter(int serieIndex)
        {
            if (useQueue)
            {
                if (_seriesQueue.ContainsKey(serieIndex))
                {
                    return _seriesQueue[serieIndex].Param;
                }
            }
            else
            {
                if (_series.ContainsKey(serieIndex))
                {
                    return _series[serieIndex].Param;
                }
            }
            return null;
        }

        public void SetMarked(int id)
        {
            if (useQueue)
            {
                if (_markedIdQueue.Count > kMaxTrackingQueueSize)
                    _markedIdQueue.RemoveAt(0);

                var index = _markedIdQueue.BinarySearch(id);
                if (index < 0) index = ~index;

                _markedIdQueue.Insert(index, id);
            }
        }

        public void SetAiClass(int id, int aiClass)
        {
            if (useQueue)
            {
                if (_aiClassIdQueue.Count > kMaxTrackingQueueSize)
                    _aiClassIdQueue.RemoveAt(0);

                var index = _aiClassIdQueue.BinarySearch(id);
                if (index < 0) index = ~index;

                _aiClassIdQueue.Insert(index, id);
                _aiClassIdToClassQueue.Add(id, aiClass);
            }
        }

        private bool ShowDefect(RollmapDefect defect)
        {
            if (MarkingSerieIndex < 0 || (defect.Serie != MarkingSerieIndex && defect.Serie != MarkingSerieIndex + 1))
            {
                //일반적인결점
                if (DefectViewMode == tagDefectViewMode.TypeMarking)
                {
                    if (!defect.Marking) return false;
                }
                else if (DefectViewMode == tagDefectViewMode.Level)
                {
                    if (!GetViewLevel(defect.Level)) return false;
                }
                else if (DefectViewMode == tagDefectViewMode.LevelMarking)
                {
                    if (!GetViewLevel(defect.Level)) return false;
                    if (!defect.Marking) return false;
                }
                else if (DefectViewMode == tagDefectViewMode.LevelMarked)
                {
                    return false;
                }
                else if (DefectViewMode == tagDefectViewMode.LevelMissed)
                {
                    if (!GetViewLevel(defect.Level)) return false;
                    if (!defect.Marking) return false;
                    if (defect.Marked > 0) return false;
                    if (useQueue && _markedIdQueue.Contains(defect.Id)) return false;
                }
                else if (DefectViewMode == tagDefectViewMode.AiClass)
                {
                    if (!GetViewLevel(defect.Level)) return false;
                    if (useQueue)
                    {
                        if(!_aiClassIdQueue.Contains(defect.Id)) return false;
                        if (!_aiClassIdToClassQueue.ContainsKey(defect.Id) || !GetViewAiClass(_aiClassIdToClassQueue[defect.Id])) return false;
                    }
                    else
                    {
                        if (defect.AiClass >= 0 && !GetViewAiClass(defect.AiClass)) return false;
                    }
                }
                else if (DefectViewMode == tagDefectViewMode.AiClassMarking)
                {
                    if (!GetViewLevel(defect.Level)) return false;
                    if (!defect.Marking) return false;
                    if (useQueue)
                    {
                        if (!_aiClassIdQueue.Contains(defect.Id)) return false;
                        if (!_aiClassIdToClassQueue.ContainsKey(defect.Id) || !GetViewAiClass(_aiClassIdToClassQueue[defect.Id])) return false;
                    }
                    else
                    {
                        if (!GetViewAiClass(defect.AiClass)) return false;
                    }
                }
            }
            else
            {
                //마킹기에서 가져온 결점
                if (DefectViewMode == tagDefectViewMode.LevelMarked)
                {
                    if (!GetViewLevel(defect.Level)) return false;
                }
                else
                {
                    return false;
                }
            }
            return true;
        }
#if USE_SHARP_DX
        Dictionary<string, TextLayout> dicTextLayout = new Dictionary<string, TextLayout>();
#else
        Dictionary<string, DWriteTextLayout> dicTextLayout = new Dictionary<string, DWriteTextLayout>();
#endif

#if USE_SHARP_DX
        public TextLayout GetTextLayout(SharpDX.DirectWrite.Factory writeFactory, TextFormat txtFormat, string symbol)
        {
            if (!dicTextLayout.ContainsKey(symbol))
            {
                if (symbol == "Circle") // 마킹비전 심볼
                {
                    var newSymbol = "○";
                    using (var format = new TextFormat(writeFactory, "Segoe UI", null, FontWeight.Normal, SharpDX.DirectWrite.FontStyle.Normal, FontStretch.Normal, 25, ""))
                    {
                        var textLayout = new TextLayout(writeFactory, newSymbol, format, 50, 50);
                        dicTextLayout.Add(symbol, textLayout);
                    }
                }
                else
                {
                    var textLayout = new TextLayout(writeFactory, symbol, txtFormat, 50, 50);
                    dicTextLayout.Add(symbol, textLayout);
                }
            }

            return dicTextLayout[symbol];
        }
#else
        public DWriteTextLayout GetTextLayout(DWriteFactory writeFactory, DWriteTextFormat txtFormat, string symbol)
        {
            if ( !dicTextLayout.ContainsKey(symbol) )
			{
                if (symbol == "Circle") // 마킹비전 심볼
                {
                    var newSymbol = "○";
                    using (var format = writeFactory.CreateTextFormat("Segoe UI", null, DWriteFontWeight.Normal, DWriteFontStyle.Normal, DWriteFontStretch.Normal, 25, ""))
                    {
                        var textLayout = writeFactory.CreateTextLayout(newSymbol, format, 50, 50);
                        dicTextLayout.Add(symbol, textLayout);
                    }
                }
                else
                {
                    var textLayout = writeFactory.CreateTextLayout(symbol, txtFormat, 50, 50);
                    dicTextLayout.Add(symbol, textLayout);
                }
			}

			return dicTextLayout[symbol];
		}
#endif

#if USE_SHARP_DX
        public void Draw(RenderTarget renderTarget, SharpDX.DirectWrite.Factory writeFactory, TextFormat txtFormat, SolidColorBrush brush, RectangleF real, RectangleF view, string text, int mapOffsetX = 0)
#else
        public void Draw(D2D1RenderTarget renderTarget, DWriteFactory writeFactory, DWriteTextFormat txtFormat, D2D1SolidColorBrush brush, RectangleF real, RectangleF view)
#endif
        {
            if (DisableDraw) return;
            try
            {
                //using (NativeTextRenderer textRenderer = new NativeTextRenderer(g))
                {
                    var ScaleRealX = (double)view.Width / real.Width;
                    var ScaleRealY = (double)view.Height / real.Height;
                    float x = 0;
                    float y = 0;
                    float w = 0;
                    float h = 0;

                    Action<RollmapDefect, SerieParameter, Color> drawDefect = (defect, param, color) =>
                    {
#if USE_SHARP_DX
                        brush.Color = ColorConverter.ColorToRawColor(color);
#else
                    brush.Color = new D2D1ColorF(ColorConverter.ColorToUint(color));
#endif
                        var offsetX = -(param.Font.Size / 3);
                        if (param.Symbol == "Circle") offsetX = -(param.Font.Size);
                        var offsetY = -(param.Font.Size + 1);
                        if (param.Symbol == "Circle") offsetY = -(param.Font.Size + 9);

                        if (param.Symbol == "Rectangle")
                        {
                            //Text정보 표시
                            if (!string.IsNullOrWhiteSpace(defect.Info))
                            {
                                brush.Opacity = 1f;
                                x = (float)(((defect.X - real.X) * ScaleRealX + view.Left) + offsetX);
                                y = (float)(((defect.Y - real.Y) * ScaleRealY + view.Top) + offsetY);
                                var txtLayout = GetTextLayout(writeFactory, txtFormat, defect.Info);
#if USE_SHARP_DX
                                renderTarget.DrawTextLayout(new RawVector2(x, y), txtLayout, brush);
#else
                            renderTarget.DrawTextLayout(new D2D1Point2F(x, y), txtLayout, brush);
#endif
                            }

                            //Rect 표시
                            brush.Opacity = 0.3f;
                            x = (float)((defect.X - real.X) * ScaleRealX + view.Left);
                            y = (float)((defect.Y - real.Y) * ScaleRealY + view.Top);
                            w = (float)((defect.X - real.X + defect.Width) * ScaleRealX + view.Left);
                            h = (float)((defect.Y - real.Y + defect.Height) * ScaleRealY + view.Top);
#if USE_SHARP_DX
                            renderTarget.FillRectangle(new RawRectangleF(x, y, w, h), brush);
#else
                        renderTarget.FillRectangle(new D2D1RectF(x, y, w, h), brush);
#endif
                        }
                        else
                        {
                            var symbol = param.Symbol;

                            if (UseSameDefect && defect.Dclass % k10LevelDelimiterNgType < 6)
                            {
                                var samenum = defect.Dclass % k10LevelDelimiterNgType;
                                if (!ShowSameDefect[samenum - 1]) return;
                            }
                            //주기결점 처리
                            else if (ShowPeriod)
                            {
                                var period = defect.DefectToPeriod10Level(defect.Dclass);
                                if (period > 0)
                                    symbol = period.ToString();
                            }
                            //파라미터에 주기 0으로 되어있어도 클라이언트에서 주기로 넘어올 때 예외처리 @ATW 240604
                            if (UsePeriodClassification && this.CycleParam.Count > 0)
                            {
                                var period = defect.DefectToPeriod10Level(defect.Dclass);
                                var optic = param.Optic;
                                var cycleindex = this.CycleParam.IndexOf(period);
                                if (period > 0 && optic >= 0 && optic < ShowPeriodDefect.GetLength(0))
                                {
                                    if (cycleindex == -1)
                                    {
                                        if (!ShowPeriodDefect[optic, this.CycleParam.Count - 1]) return;
                                    }
                                    else
                                    {
                                        if (!ShowPeriodDefect[optic, cycleindex]) return;
                                    }
                                }
                            }
                            //이전공정 동일결점 처리 @ATW 240427
                            //if (UsePrevCompare)
                            //{
                            //    foreach (var item in prevDrawPoint)
                            //    {

                            //    }
                            //}

                            brush.Opacity = 1f;
                            x = (float)(((defect.X - real.X) * ScaleRealX + view.Left) + offsetX + mapOffsetX);
                            y = (float)(((defect.Y - real.Y) * ScaleRealY + view.Top) + offsetY);
                            var txtLayout = GetTextLayout(writeFactory, txtFormat, symbol);
#if USE_SHARP_DX
                            renderTarget.DrawTextLayout(new RawVector2(x, y), txtLayout, brush);
#else
                        renderTarget.DrawTextLayout(new D2D1Point2F(x, y), txtLayout, brush);
#endif
                        }
                    };

                    //이전공정 결점비교 롤맵 그리는 부분 추가
                    if (text != "FormRollMap" && prevCompareRollmapDefect.Count > 0 && UsePrevCompare)
                    {
                        brush.Opacity = 1f;
                        var offsetX = -(8 / 3);
                        var offsetY = -(8 + 1);
                        for (int i = 0; i < prevCompareRollmapDefect.Count; i++)
                        {
                            string symbol = "*";
                            brush.Color = ColorConverter.ColorToRawColor(Color.Green);
                            //x = (float)(((compareDefect[i].calcX - real.X) * ScaleRealX + view.Left) + offsetX);
                            //y = (float)(((compareDefect[i].calcY - real.Y) * ScaleRealY + view.Top) + offsetY);
                            x = (float)(((prevCompareRollmapDefect[i].X - real.X) * ScaleRealX + view.Left) + offsetX);
                            y = (float)(((prevCompareRollmapDefect[i].Y - real.Y) * ScaleRealY + view.Top) + offsetY);
                            var txtLayout = GetTextLayout(writeFactory, txtFormat, symbol);
#if USE_SHARP_DX
                            renderTarget.DrawTextLayout(new RawVector2(x, y), txtLayout, brush);
#else
                        renderTarget.DrawTextLayout(new D2D1Point2F(x, y), txtLayout, brush);
#endif
                        }
                        return;
                    }

                    if (useQueue)
                    {
                        //실시간맵
                        lock (locker)
                        {
                            foreach (var serie in _seriesQueue)
                            {
                                var ss = serie.Value;
                                if (ss.IsVisable)
                                {
                                    foreach (var defect in ss.Defects)
                                    {
                                        if (real.Contains(defect.X, defect.Y))
                                        {
                                            if (ShowDefect(defect))
                                            {
                                                var color = (useColor2 == true) ? (defect.Level > 0 ? ss.Param.Color2 : ss.Param.Color) : //강불량은 Color, 약불량은 Color2
                                                                                ((showDefectColor == false && defect.Marking == false) ? nonMarkColor : ss.Param.Color);   //마킹불량은 Color, 미마킹은 nonMarkColor

                                                //이전공정 동일결점 체크 @ATW 240427
                                                //if (UsePrevCompare && PrevCompareSameRange.X > 0 && PrevCompareSameRange.Y > 0)
                                                //{
                                                //    foreach (var item in prevDrawPoint)
                                                //    {
                                                //        if (item.optic == ss.Param.Optic && Math.Abs(item.posx - defect.X) < PrevCompareSameRange.X && Math.Abs(item.posy - defect.Y) < PrevCompareSameRange.Y)
                                                //            ss.Param.Symbol = "□";
                                                //    }
                                                //}
                                                
                                                drawDefect(defect, ss.Param, color);
                                            }
                                        }
                                    }
                                }
                            }
                            //클릭한결점은 다시 그려준다
                            if (oldClicked.Id >= 0 && oldClicked.Serie >= 0)
                            {
                                var serie = _seriesQueue.Where(e => e.Key == oldClicked.Serie)
                                    .Select(e => (KeyValuePair<int, PointsSerieQueue>?)e).FirstOrDefault();
                                if (serie != null)
                                {
                                    if (ShowDefect(oldClicked))
                                    {
                                        var ss = serie.Value.Value;
                                        drawDefect(oldClicked, ss.Param, ClickedColor);
                                    }
                                }
                            }

                            //이전공정 추가 @ATW 240320
                            if (UsePrevCompare)
                            {
                                brush.Opacity = 1f;
                                var offsetX = -(8 / 3);
                                var offsetY = -(8 + 1);
                                foreach (var item in prevDrawPoint)
                                {
                                    string symbol = item.symbol;
                                    brush.Color = ColorConverter.ColorToRawColor(item.color);
                                    //x = (float)(((compareDefect[i].calcX - real.X) * ScaleRealX + view.Left) + offsetX);
                                    //y = (float)(((compareDefect[i].calcY - real.Y) * ScaleRealY + view.Top) + offsetY);
                                    x = (float)(((item.posx - real.X) * ScaleRealX + view.Left) + offsetX);
                                    y = (float)(((item.posy - real.Y) * ScaleRealY + view.Top) + offsetY);
                                    var txtLayout = GetTextLayout(writeFactory, txtFormat, symbol);
    #if USE_SHARP_DX
                                    renderTarget.DrawTextLayout(new RawVector2(x, y), txtLayout, brush);
    #else
                            renderTarget.DrawTextLayout(new D2D1Point2F(x, y), txtLayout, brush);
    #endif
                                }
                            }
                        }
                    }
                    else
                    {
                        //Load맵
                        lock (locker)
                        {
                            foreach (var serie in _series)
                            {
                                var ss = serie.Value;
                                if (ss.IsVisable || (IsAiMode && _viewAiClass.Count(flag => flag == true) > 0))
                                {
                                    foreach (var defect in ss.Defects)
                                    {
                                        if (!ss.IsVisable && defect.AiClass < 0) continue;
                                        if (real.Contains(defect.X, defect.Y))
                                        {
                                            if (ShowDefect(defect))
                                            {
                                                var color = (useColor2 == true) ? (defect.Level > 0 ? ss.Param.Color2 : ss.Param.Color) : //강불량은 Color, 약불량은 Color2
                                                                                ((showDefectColor == false && defect.Marking == false) ? nonMarkColor : ss.Param.Color);   //마킹불량은 Color, 미마킹은 nonMarkColor
                                                //동일결점 회색으로 표시되어 수정 @ATW 231121
                                                if (defect.Dclass % k10LevelDelimiterNgType <= kMaxSameDefect)
                                                    color = ss.Param.Color;
                                                
                                                drawDefect(defect, ss.Param, color);
                                            }
                                        }
                                    }
                                }
                            }

                            
                            brush.Opacity = 1f;
                            var offsetX = -(8 / 3);
                            var offsetY = -(8 + 1);
                            for (int i = 0; i < prevDrawPoint.Count; i++)
                            {
                                //if (!CompareShowDefect[compareDefect[i].optic]) continue;
                                string symbol = prevDrawPoint[i].symbol;
                                brush.Color = ColorConverter.ColorToRawColor(prevDrawPoint[i].color);
                                x = (float)(((prevDrawPoint[i].posx - real.X) * ScaleRealX + view.Left) + offsetX);
                                y = (float)(((prevDrawPoint[i].posy - real.Y) * ScaleRealY + view.Top) + offsetY);
                                var txtLayout = GetTextLayout(writeFactory, txtFormat, symbol);
#if USE_SHARP_DX
                                renderTarget.DrawTextLayout(new RawVector2(x, y), txtLayout, brush);
#else
                    renderTarget.DrawTextLayout(new D2D1Point2F(x, y), txtLayout, brush);
#endif
                            }

                            for (int i = 0; i < prevErrorArea.Count; i++) 
                            {
                                brush.Color = new RawColor4(1.0f, 0.0f, 0.0f, 0.5f);
                                float x1 = (float)((prevErrorArea[i].startX - real.X) * ScaleRealX + view.Left);
                                float y1 = (float)((prevErrorArea[i].startY - real.Y) * ScaleRealY + view.Top);
                                float x2 = (float)((prevErrorArea[i].endX - real.X) * ScaleRealX + view.Left);
                                float y2 = (float)((prevErrorArea[i].endY - real.Y) * ScaleRealY + view.Top);
                                if (y2 - y1 < 15)
                                    y2 = y1 + 15;
                                renderTarget.FillRectangle(new RawRectangleF(x1, y1, x2, y2), brush);
                            }

                            //클릭한결점은 다시 그려준다
                            if (oldClicked.Id >= 0 && oldClicked.Serie >= 0)
                            {
                                var serie = _series.Where(e => e.Key == oldClicked.Serie)
                                    .Select(e => (KeyValuePair<int, PointsSerie>?)e).FirstOrDefault();
                                if (serie != null)
                                {
                                    if (ShowDefect(oldClicked))
                                    {
                                        var ss = serie.Value.Value;
                                        drawDefect(oldClicked, ss.Param, ClickedColor);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            catch(Exception ex)
            {
            }
        }

        private RollmapDefect oldClicked = new RollmapDefect(-1, 0, 0, 0, 0, 0, false, 0);
        /// <summary>
        /// 롤맵에서 클릭한 위치에서 가장 가까운 결점 1개를 검색
        /// </summary>
        /// <returns>찾은 결점</returns>
#if USE_SHARP_DX
        public RollmapDefect GetNearestDefect(RenderTarget renderTarget, SharpDX.DirectWrite.Factory writeFactory, TextFormat txtFormat, SolidColorBrush brush, Point realP, Point gdiP, RectangleF real, RectangleF view, int searchRange, bool mark)
#else
        public RollmapDefect GetNearestDefect(D2D1RenderTarget renderTarget, DWriteFactory writeFactory, DWriteTextFormat txtFormat, D2D1SolidColorBrush brush, Point realP, Point gdiP, RectangleF real, RectangleF view, int searchRange, bool mark)
#endif
        {
            renderTarget.BeginDraw();

            var ScaleRealX = (double)view.Width / real.Width;
            var ScaleRealY = (double)view.Height / real.Height;
            double searchRangeX = (double)searchRange / ScaleRealX; //mm
            double searchRangeY = (double)searchRange / ScaleRealY; //mm
            double maxSearchY = 10000;  //클릭위치에서 10M넘으면 Search를 중지


            double minDistance = double.MaxValue;
            RollmapDefect minD = new RollmapDefect(-1, 0, 0, 0, 0, 0, false, 0);
            RollmapDefect minDOld = new RollmapDefect(-1, 0, 0, 0, 0, 0, false, 0);
            double minDistanceOld = double.MaxValue;

            float x = 0;
            float y = 0;
            Action<RollmapDefect, SerieParameter, Color> drawDefect = (defect, param, color) =>
            {
#if USE_SHARP_DX
                brush.Color = ColorConverter.ColorToRawColor(color);
#else
                brush.Color = new D2D1ColorF(ColorConverter.ColorToUint(color));
#endif

                var offsetX = -(param.Font.Size / 3);
                if (param.Symbol == "Circle") offsetX = -(param.Font.Size);
                var offsetY = -(param.Font.Size + 1);
                if (param.Symbol == "Circle") offsetY = -(param.Font.Size + 9);

                if (param.Symbol == "Rectangle")
                {
                    //Text정보 표시
                    if (!string.IsNullOrWhiteSpace(defect.Info))
                    {
                        brush.Opacity = 1f;
                        x = (float)(((defect.X - real.X) * ScaleRealX + view.Left) + offsetX);
                        y = (float)(((defect.Y - real.Y) * ScaleRealY + view.Top) + offsetY);
                        var txtLayout = GetTextLayout(writeFactory, txtFormat, defect.Info);
#if USE_SHARP_DX
                        renderTarget.DrawTextLayout(new RawVector2(x, y), txtLayout, brush);
#else
                        renderTarget.DrawTextLayout(new D2D1Point2F(x, y), txtLayout, brush);
#endif
                    }

                    ////Rect 표시
                    //brush.Opacity = 0.3f;
                    //x = (float)((defect.X - real.X) * ScaleRealX + view.Left);
                    //y = (float)((defect.Y - real.Y) * ScaleRealY + view.Top);
                    //w = (float)((defect.X - real.X + defect.Width) * ScaleRealX + view.Left);
                    //h = (float)((defect.Y - real.Y + defect.Height) * ScaleRealY + view.Top);
                    //renderTarget.FillRectangle(new D2D1RectF(x, y, w, h), brush);
                }
                else
                {
                    var symbol = param.Symbol;
                    //주기결점 처리
                    if (ShowPeriod)
                    {
                        var period = defect.DefectToPeriod10Level(defect.Dclass);
                        if (period > 0)
                            symbol = period.ToString();
                    }
                    if (UsePeriodClassification)
                    {
                        var period = defect.DefectToPeriod10Level(defect.Dclass);
                        var optic = param.Optic;
                        var cycleindex = this.CycleParam.IndexOf(period);
                        if (period > 0 && optic >= 0 && optic < ShowPeriodDefect.GetLength(0))
                        {
                            if (cycleindex == -1)
                            {
                                if (this.CycleParam.Count > 0)
                                {
                                    if (!ShowPeriodDefect[optic, this.CycleParam.Count - 1]) return;
                                }
                            }
                            else
                            {
                                if (!ShowPeriodDefect[optic, cycleindex]) return;
                            }
                        }
                    }

                    brush.Opacity = 1f;
                    x = (float)(((defect.X - real.X) * ScaleRealX + view.Left) + offsetX + DefectOffsetX);
                    y = (float)(((defect.Y - real.Y) * ScaleRealY + view.Top) + offsetY);
                    var txtLayout = GetTextLayout(writeFactory, txtFormat, symbol);
#if USE_SHARP_DX
                    renderTarget.DrawTextLayout(new RawVector2(x, y), txtLayout, brush);
#else
                    renderTarget.DrawTextLayout(new D2D1Point2F(x, y), txtLayout, brush);
#endif
                }
            };

            if (useQueue)
            {
                lock (locker)
                {
                    //이전 찾은 결점은 색상을 원래대로 바꿈
                    if (mark && oldClicked.Id >= 0)
                    {
                        var param = _seriesQueue[oldClicked.Serie].Param;

                        var color = (useColor2 == true) ? (oldClicked.Level > 0 ? param.Color2 : param.Color) :     //강불량은 Color, 약불량은 Color2
                        ((showDefectColor == false && oldClicked.Marking == false) ? nonMarkColor : param.Color);   //마킹불량은 Color, 미마킹은 nonMarkColor

                        drawDefect(oldClicked, param, color);
                    }

                    foreach (var serie in _seriesQueue)
                    {
                        var ss = serie.Value;
                        if (ss.IsVisable)
                        {
                            foreach (var defect in ss.Defects)
                            {
                                if (defect.Dclass < 10) continue; // 마킹비전 결점은 선택하지 않음.
                                if (defect.ClickDisable) continue;

                                if (real.Contains(defect.X, defect.Y))
                                {
                                    if (ShowDefect(defect))
                                    {
                                        //우선은 click한 결점좌표로 허용범위내의 결점을 찾고
                                        if (oldClicked.Id != defect.Id)
                                        {
                                            if (Math.Abs(defect.Y - realP.Y) < searchRangeY)
                                            {
                                                if (Math.Abs(defect.X - realP.X) < searchRangeX)
                                                {
                                                    //클릭한 위치를 기준으로 가장가까운 pixel에 위치한 결점을 찾는다.
                                                    int px = (int)(ScaleRealX * (defect.X - real.X));
                                                    int py = (int)(ScaleRealY * (defect.Y - real.Y));
                                                    var d = (px - gdiP.X) * (px - gdiP.X) + (py - gdiP.Y) * (py - gdiP.Y);
                                                    if (d < minDistance)
                                                    {
                                                        minD = defect;
                                                        minDistance = d;
                                                    }
                                                }
                                            }
                                            else if (realP.Y + maxSearchY < defect.Y)
                                            {
                                                //클릭한위치로부터 검색범위를 넘으면 중단
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            //클릭한 위치를 기준으로 가장가까운 pixel에 위치한 결점을 찾는다.
                                            if (Math.Abs(defect.Y - realP.Y) < searchRangeY)
                                            {
                                                if (Math.Abs(defect.X - realP.X) < searchRangeX)
                                                {
                                                    //클릭한 위치를 기준으로 가장가까운 pixel에 위치한 결점을 찾는다.
                                                    int px = (int)(ScaleRealX * (defect.X - real.X));
                                                    int py = (int)(ScaleRealY * (defect.Y - real.Y));
                                                    var d = (px - gdiP.X) * (px - gdiP.X) + (py - gdiP.Y) * (py - gdiP.Y);

                                                    if (d < minDistance)
                                                    {
                                                        minDOld = defect;
                                                        minDistanceOld = d;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if (minD.Id < 0 && minDOld.Id >= 0)
                {
                    minD = minDOld;
                    minDistance = minDistanceOld;
                }
                if (mark && minD.Id >= 0)
                {
                    //새로 찾은 결점 색상을 바꿈
                    var ss = _seriesQueue.FirstOrDefault(e => e.Key == minD.Serie).Value;
                    drawDefect(minD, ss.Param, ClickedColor);

                    oldClicked = minD;
                }
                else
                {
                    oldClicked.Id = -1;
                }
            }
            else
            {
                lock (locker)
                {
                    //이전 찾은 결점은 색상을 원래대로 바꿈
                    if (mark && oldClicked.Id >= 0)
                    {
                        var param = _series[oldClicked.Serie].Param;

                        var color = (useColor2 == true) ? (oldClicked.Level > 0 ? param.Color2 : param.Color) :     //강불량은 Color, 약불량은 Color2
                        ((showDefectColor == false && oldClicked.Marking == false) ? nonMarkColor : param.Color);   //마킹불량은 Color, 미마킹은 nonMarkColor

                        drawDefect(oldClicked, param, color);
                    }

                    foreach (var serie in _series)
                    {
                        var ss = serie.Value;
                        if (ss.IsVisable || (IsAiMode && _viewAiClass.Count(flag => flag == true) > 0))
                        {
                            foreach (var defect in ss.Defects)
                            {
                                if (!ss.IsVisable && defect.AiClass < 0) continue;
                                if (defect.Dclass < 10) continue; // 마킹비전 결점은 선택하지 않음.
                                if (defect.ClickDisable) continue;

                                if (real.Contains(defect.X, defect.Y))
                                {
                                    if (ShowDefect(defect))
                                    {
                                        //우선은 click한 결점좌표로 허용범위내의 결점을 찾고
                                        if (oldClicked.Id != defect.Id)
                                        {
                                            if (Math.Abs(defect.Y - realP.Y) < searchRangeY)
                                            {
                                                if (Math.Abs(defect.X - realP.X) < searchRangeX)
                                                {
                                                    //클릭한 위치를 기준으로 가장가까운 pixel에 위치한 결점을 찾는다.
                                                    int px = (int)(ScaleRealX * (defect.X - real.X));
                                                    int py = (int)(ScaleRealY * (defect.Y - real.Y));
                                                    var d = (px - gdiP.X) * (px - gdiP.X) + (py - gdiP.Y) * (py - gdiP.Y);

                                                    if (d < minDistance)
                                                    {
                                                        minD = defect;
                                                        minDistance = d;
                                                    }
                                                }
                                            }
                                            //결점이 순차적으로 들어오지는 않아서 전체를 탐색해야 해서 삭제
                                            //else if (realP.Y + maxSearchY < defect.Y)
                                            //{
                                            //    break;
                                            //}
                                        }
                                        else
                                        {
                                            //클릭한 위치를 기준으로 가장가까운 pixel에 위치한 결점을 찾는다.
                                            if (Math.Abs(defect.Y - realP.Y) < searchRangeY)
                                            {
                                                if (Math.Abs(defect.X - realP.X) < searchRangeX)
                                                {
                                                    //클릭한 위치를 기준으로 가장가까운 pixel에 위치한 결점을 찾는다.
                                                    int px = (int)(ScaleRealX * (defect.X - real.X));
                                                    int py = (int)(ScaleRealY * (defect.Y - real.Y));
                                                    var d = (px - gdiP.X) * (px - gdiP.X) + (py - gdiP.Y) * (py - gdiP.Y);

                                                    if (d < minDistance)
                                                    {
                                                        minDOld = defect;
                                                        minDistanceOld = d;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }

                            }
                        }
                    }
                }
                if (minD.Id < 0 && minDOld.Id >= 0)
                {
                    minD = minDOld;
                    minDistance = minDistanceOld;
                }
                if (mark && minD.Id >= 0)
                {
                    //새로 찾은 결점 색상을 바꿈
                    var ss = _series.FirstOrDefault(e => e.Key == minD.Serie).Value;

#if USE_SHARP_DX
                    brush.Color = ColorConverter.ColorToRawColor(ClickedColor);
#else
                    brush.Color = new D2D1ColorF(ColorConverter.ColorToUint(ClickedColor));
#endif
                    drawDefect(minD, ss.Param, ClickedColor);
                    oldClicked = minD;
                }
                else
                {
                    oldClicked.Id = -1;
                }
            }

            renderTarget.EndDraw();


            return minD;
        }
        /// <summary>
        /// 롤맵에서 클릭한 위치에서 가장 가까운 결점을 광학계별로 찾는다.
        /// </summary>
        /// <returns>찾은결점들</returns>
#if USE_SHARP_DX
        public Dictionary<int, RollmapDefect> GetNearestDefects(RenderTarget renderTarget, SharpDX.DirectWrite.Factory writeFactory, TextFormat txtFormat, SolidColorBrush brush, Point realP, Point gdiP, RectangleF real, RectangleF view, int searchRange, bool mark)
#else
        public Dictionary<int, RollmapDefect> GetNearestDefects(D2D1RenderTarget renderTarget, DWriteFactory writeFactory, DWriteTextFormat txtFormat, D2D1SolidColorBrush brush, Point realP, Point gdiP, RectangleF real, RectangleF view, int searchRange, bool mark)
#endif
        {
            var ScaleRealX = (double)view.Width / real.Width;
            var ScaleRealY = (double)view.Height / real.Height;
            double searchRangeX = (double)searchRange / ScaleRealX; //mm
            double searchRangeY = (double)searchRange / ScaleRealY; //mm
            double maxSearchY = 10000;  //클릭위치에서 10M넘으면 Search를 중지

            //클릭한 위치의 가장 가까운 결점을 찾는다.
            var found = GetNearestDefect(renderTarget, writeFactory, txtFormat, brush, realP, gdiP, real, view, searchRange, mark);

            var minDefects = new Dictionary<int, RollmapDefect>();
            foreach (var optic in _optics)
                minDefects.Add(optic, new RollmapDefect(-1, 0, 0, 0, 0, 0, false, 0));

            //범위안에 결점이 있다면, 해당 결점으로 optic별로 가장 가까운 결점을 찾는다.
            if (found.Id != -1)
            {
                if (useQueue)
                {
                    lock (locker)
                    {
                        foreach (var optic in _optics)
                        {
                            double minDistance = double.MaxValue;
                            if (optic == ((found.Dclass % 100) / 10) - 1)
                            {
                                minDefects[optic] = found;
                                minDistance = 0;
                            }
                            else
                            {
                                foreach (var serie in _seriesQueue)
                                {
                                    var ss = serie.Value;
                                    if (ss.IsVisable && optic == serie.Value.Param.Optic)
                                    {
                                        foreach (var defect in ss.Defects)
                                        {
                                            if (defect.Dclass < 10) continue; // 마킹비전 결점은 선택하지 않음.
                                            if (defect.ClickDisable) continue;

                                            if (real.Contains(defect.X, defect.Y) && ShowDefect(defect))
                                            {
                                                if (Math.Abs(defect.Y - found.Y) < searchRangeY && Math.Abs(defect.X - found.X) < searchRangeX)
                                                {
                                                    //클릭한 위치를 기준으로 가장가까운 pixel에 위치한 결점을 찾는다.
                                                    int px = (int)(ScaleRealX * (defect.X - found.X));
                                                    int py = (int)(ScaleRealY * (defect.Y - found.Y));
                                                    var d = (px - gdiP.X) * (px - gdiP.X) + (py - gdiP.Y) * (py - gdiP.Y);
                                                    if (d < minDistance)
                                                    {
                                                        minDefects[optic] = defect;
                                                        minDistance = d;
                                                    }
                                                }
                                                else if (found.Y + maxSearchY < defect.Y)
                                                {
                                                    //클릭한위치로부터 검색범위를 넘으면 중단
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    lock (locker)
                    {
                        foreach (var optic in _optics)
                        {
                            double minDistance = double.MaxValue;
                            if (optic == ((found.Dclass % 100) / 10) - 1)
                            {
                                minDefects[optic] = found;
                                minDistance = 0;
                            }
                            else
                            {
                                foreach (var serie in _series)
                                {
                                    var ss = serie.Value;
                                    if (ss.IsVisable && optic == serie.Value.Param.Optic)
                                    {
                                        foreach (var defect in ss.Defects)
                                        {
                                            if (defect.Dclass < 10) continue; // 마킹비전 결점은 선택하지 않음.
                                            if (defect.ClickDisable) continue;

                                            if (real.Contains(defect.X, defect.Y) && ShowDefect(defect))
                                            {
                                                if (Math.Abs(defect.Y - found.Y) < searchRangeY && Math.Abs(defect.X - found.X) < searchRangeX)
                                                {
                                                    //클릭한 위치를 기준으로 가장가까운 pixel에 위치한 결점을 찾는다.
                                                    int px = (int)(ScaleRealX * (defect.X - found.X));
                                                    int py = (int)(ScaleRealY * (defect.Y - found.Y));
                                                    var d = (px - gdiP.X) * (px - gdiP.X) + (py - gdiP.Y) * (py - gdiP.Y);
                                                    if (d < minDistance)
                                                    {
                                                        minDefects[optic] = defect;
                                                        minDistance = d;
                                                    }
                                                }
                                                else if (found.Y + maxSearchY < defect.Y)
                                                {
                                                    //클릭한위치로부터 검색범위를 넘으면 중단
                                                    //break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }                        
                    }
                }
            }

            if (ComparePostion)
            {
                NelCompareDefect temp = new NelCompareDefect();
                double min = double.MaxValue;
                foreach (var item in compareDefect)
                {
                    if (!CompareShowDefect[item.optic]) continue;
                    if (Math.Abs(realP.X - item.calcX) < searchRangeX && Math.Abs(realP.Y - item.calcY) < searchRangeY)
                    {
                        var d = (realP.X - item.calcX) * (realP.X - item.calcX) + (realP.Y - item.calcY) * (realP.Y - item.calcY);
                        if (d < min)
                        {
                            temp = item;
                            min = d;
                        }
                    }
                }

                //클릭한 위치에 결점이 없으면 초기화 => 호출한 Form에서 결점없음을 인식할수 있게 변경
                //if (temp.posX > 0 && temp.posY > 0)
                    SetNelViewDefect(temp);
                    //MessageBox.Show($"PosX : {temp.posX}\nPosY : {temp.posY}\nSize : {temp.size}\nCam : {temp.cam}\nPath : {temp.path}");
            }

            return minDefects;
        }

        public void ResetNelPos()
        {
            compareDefect.Clear();
        }

        public void SetNelPos(double posX, double posY, int optic, Color color, int cam, double size, string path, double calcX, double calcY, double sttPos, string date)
        {
            compareDefect.Add(new NelCompareDefect(optic, posX, posY, color, cam, size, path, calcX, calcY, sttPos, date));
        }

        public List<NelCompareDefect> GetNelPos()
        {
            if (compareDefect.Count > 0)
                return compareDefect;
            else
                return null;
        }

        NelCompareDefect viewDefect = new NelCompareDefect();
        public void SetNelViewDefect(NelCompareDefect temp)
        {
            viewDefect = temp;
        }

        public string[] GetNelViewDefect()
        {
            if (viewDefect.posX <= 0 && viewDefect.posY <= 0)
                return null;
            string[] tempStr = new string[9];
            tempStr[0] = viewDefect.posX.ToString();
            tempStr[1] = viewDefect.posY.ToString();
            tempStr[2] = viewDefect.size.ToString();
            tempStr[3] = viewDefect.cam.ToString();
            tempStr[4] = viewDefect.path.ToString();
            tempStr[5] = viewDefect.calcX.ToString();
            tempStr[6] = viewDefect.calcY.ToString();
            tempStr[7] = viewDefect.sttPos.ToString();
            tempStr[8] = viewDefect.date;
            return tempStr;
        }

        //이전공정 결점비교 좌표 초기화
        public void ResetPrevCompareDefectPos()
        {
            prevCompareRollmapDefect.Clear();
        }

        //이전공정 결점비교 좌표 셋팅
        public void SetPrevCompareDefectPos(double posX, double posY)
        {
            prevCompareRollmapDefect.Add(new PointF((float)posX, (float)posY));
        }

        public List<PointF> GetPrevCompareDefectPos()
        {
            if (prevCompareRollmapDefect.Count > 0)
                return prevCompareRollmapDefect;
            else
                return null;
        }
    }

    public class RollmapMarked
    {
        public RollmapDefect defect;
        public int originalSerie;

        public RollmapMarked(RollmapDefect defect, int originalSerie)
        {
            this.defect = defect;
            this.originalSerie = originalSerie;
        }
    }

    /// <summary>
    /// 표시할 결점 구조체
    /// <remarks> 30~40btyes 이내면서 주기적으로 소멸된다면 구조체형식이 유리함 </remarks>
    /// </summary>
    public struct RollmapDefect
    {
        // 주기판단용 (Coss.Web.Server.Convert 에도 동일한 상수 정의됨)
        const int k10LevelDelimiterLevel = 1000000;
        const int k10LevelDelimiterNgType = 100;
        const int k10LevelDelimiterOptic = 10;

        /// <summary>
        /// Lot에서 유일한 ID
        /// </summary>
        public int Id { get; set; }
        /// <summary>
        /// X좌표 (mm)
        /// </summary>
        public float X { get; set; }
        /// <summary>
        /// y좌표 (mm)
        /// </summary>
        public float Y { get; set; }
        /// <summary>
        /// 결점클래스
        /// </summary>
        public int Dclass { get; set; }
        /// <summary>
        /// 전체결점종류의 Index;
        /// </summary>
        public int Serie { get; set; }
        /// <summary>
        /// 결점 level (0 index)
        /// </summary>
        public int Level { get; set; }
        /// <summary>
        /// true이면 마킹하는 결점
        /// </summary>
        public bool Marking { get; set; }
        /// <summary>
        /// 1이상이면 마킹기에서 마킹한 결점
        /// </summary>
        public int Marked { get; set; }
        /// <summary>
        /// 클릭시 선택 안되게 하기위한 flag
        /// </summary>
        public bool ClickDisable { get; set; }
        /// <summary>
        /// 폭 (mm)
        /// </summary>
        public float Width { get; set; }
        /// <summary>
        /// 높이 (mm)
        /// </summary>
        public float Height { get; set; }
        /// <summary>
        /// Rollmap에 표시하는 정보
        /// </summary>
        public string Info { get; set; }
        /// <summary>
        /// Ai Class
        /// </summary>
        public int AiClass { get; set; }
        /// <summary>
        /// 밀집기포 그룹인덱스
        /// </summary>
        public int CrowdedIdx { get; set; }
        /// <summary>
        /// 프레임 번호
        /// </summary>
        public int Frame { get; set; }
        /// <summary>
        /// PC NO
        /// </summary>
        public string PcNo { get; set; }
        /// <summary>
        /// Ai Score
        /// </summary>
        public float AiScore { get; set; }
        public RollmapDefect(RollmapDefect s)
        {
            Id = s.Id;
            X = s.X;
            Y = s.Y;
            Dclass = s.Dclass;
            Serie = s.Serie;
            Level = s.Level;
            Marking = s.Marking;
            Marked = s.Marked;
            ClickDisable = s.ClickDisable;
            Width = s.Width;
            Height = s.Height;
            Info = s.Info;
            AiClass = s.AiClass;
            CrowdedIdx = s.CrowdedIdx;
            Frame = s.Frame;
            PcNo = s.PcNo;
            AiScore = s.AiScore;
        }
        public RollmapDefect(int id, float x, float y, int dclass, int serie, int level, bool marking, int marked, bool clickDisable = false, float width = 0.0f, float height = 0.0f, string info = "", int aiClass = -1, int crowdedIdx = -1, int frame = -1, string pcNo = null, float aiScore = 0)
        {
            Id = id;
            X = x;
            Y = y;
            Dclass = dclass;
            Serie = serie;
            Level = level;
            Marking = marking;
            Marked = marked;
            ClickDisable = clickDisable;
            Width = width;
            Height = height;
            Info = info;
            AiClass = aiClass;
            CrowdedIdx = crowdedIdx;
            Frame = frame;
            PcNo = pcNo;
            AiScore = aiScore;
        }

#if USE_SHARP_DX
        public void Draw(RenderTarget renderTarget, SharpDX.DirectWrite.TextLayout txtLayout, SolidColorBrush brush, RectangleF real, RectangleF view, string symbol, System.Drawing.Font font, Color color)
#else
        public void Draw(D2D1RenderTarget renderTarget, DWriteTextLayout txtLayout, D2D1SolidColorBrush brush, RectangleF real, RectangleF view, string symbol, Font font, Color color)
#endif
        {
            var ScaleRealX = (double)view.Width / real.Width;
            var ScaleRealY = (double)view.Height / real.Height;

#if USE_SHARP_DX
            brush.Color = ColorConverter.ColorToRawColor(color);
#else
            brush.Color = new D2D1ColorF(ColorConverter.ColorToUint(color));
#endif
            brush.Opacity = 1f;

            var offsetX = -(font.Size / 3);
            if (symbol == "Circle") offsetX = -(font.Size);
            var offsetY = -(font.Size + 1);
            if (symbol == "Circle") offsetY = -(font.Size + 9);

#if USE_SHARP_DX
            renderTarget.DrawTextLayout(new RawVector2((float)(((this.X - real.X) * ScaleRealX + view.Left) + offsetX),
                (float)(((this.Y - real.Y) * ScaleRealY + view.Top) + offsetY)),
                txtLayout, brush);
#else
            renderTarget.DrawTextLayout(new D2D1Point2F((float)(((this.X - real.X) * ScaleRealX + view.Left) + offsetX),
                (float)(((this.Y - real.Y) * ScaleRealY + view.Top) + offsetY)),
                txtLayout, brush);
#endif
        }
#if USE_SHARP_DX
        public void DrawRectangle(RenderTarget renderTarget, SharpDX.DirectWrite.TextLayout txtLayout, SolidColorBrush brush, RectangleF real, RectangleF view, string symbol, System.Drawing.Font font, Color color)
#else
        public void DrawRectangle(D2D1RenderTarget renderTarget, DWriteTextLayout txtLayout, D2D1SolidColorBrush brush, RectangleF real, RectangleF view, string symbol, Font font, Color color)
#endif
        {
            var ScaleRealX = (double)view.Width / real.Width;
            var ScaleRealY = (double)view.Height / real.Height;
            var offsetX = -(font.Size / 3);
            var offsetY = -(font.Size + 1);

#if USE_SHARP_DX
            brush.Color = ColorConverter.ColorToRawColor(color);
#else
            brush.Color = new D2D1ColorF(ColorConverter.ColorToUint(color));
#endif

            float x = 0;
            float y = 0;
            float w = 0;
            float h = 0;

            //Text정보 표시
            if (!string.IsNullOrWhiteSpace(this.Info))
            {
                brush.Opacity = 1f;
                x = (float)(((this.X - real.X) * ScaleRealX + view.Left) + offsetX);
                y = (float)(((this.Y - real.Y) * ScaleRealY + view.Top) + offsetY);

#if USE_SHARP_DX
                renderTarget.DrawTextLayout(new RawVector2(x, y), txtLayout, brush);
#else
                renderTarget.DrawTextLayout(new D2D1Point2F(x, y), txtLayout, brush);
#endif
            }

            //Rect 표시
            brush.Opacity = 0.3f;
            x = (float)((this.X - real.X) * ScaleRealX + view.Left);
            y = (float)((this.Y - real.Y) * ScaleRealY + view.Top);
            w = (float)((this.X - real.X + this.Width) * ScaleRealX + view.Left);
            h = (float)((this.Y - real.Y + this.Height) * ScaleRealY + view.Top);
#if USE_SHARP_DX
            renderTarget.FillRectangle(new RawRectangleF(x, y, w, h), brush);
#else
            renderTarget.FillRectangle(new D2D1RectF(x, y, w, h), brush);
#endif
        }
        /// <summary>
        /// 결점클래스의 주기를 반환
        /// </summary>
        /// <param name="defecClass">결점클래스</param>
        /// <returns>주기결점이 아니면 0, 주기결점이면 주기를 반환</returns>
        public int DefectToPeriod10Level(int defecClass)
        {
            return (defecClass % k10LevelDelimiterLevel) / k10LevelDelimiterNgType;
        }
    }

    public class SerieParameter
    {
        public SerieParameter(int optic, String label, Color color, Color color2, System.Drawing.Brush brush, String symbol, System.Drawing.Font font)
        {
            this.Optic = optic;
            this.Label = label;
            this.Color = color;
            this.Color2 = color2;
            this.Brush = brush;
            this.Symbol = symbol;
            this.Font = font;
        }
        public int Optic { get; set; }
        public String Label { get; set; }
        //강불량(정코) 또는 마킹불량 색상(연신,코팅)
        public Color Color { get; set; }
        //약불량 색상(정코)
        public Color Color2 { get; set; }
        public System.Drawing.Brush Brush { get; set; }
        public String Symbol { get; set; }
        public System.Drawing.Font Font { get; set; }
    }
    /// <summary>
    /// 1종류의 결점 리스트
    /// </summary>
    public class PointsSerie
    {

        public SerieParameter Param { get; set; }

        public bool IsDirty { get; set; }
        public bool IsVisable { get; set; }
        public List<RollmapDefect> Defects { get; set; }

        public PointsSerie(SerieParameter param)
        {
            Defects = new List<RollmapDefect>();
            this.Param = param;
            this.IsVisable = true;
        }
    }

    /// <summary>
    /// 
    /// </summary>
    public class PointsSerieQueue
    {
        public SerieParameter Param { get; set; }
        public bool IsDirty { get; set; }
        public bool IsVisable { get; set; }
        public Queue<RollmapDefect> Defects { get; set; }

        public PointsSerieQueue(SerieParameter param)
        {
            Defects = new Queue<RollmapDefect>();
            this.Param = param;
            this.IsVisable = true;
        }
    }

    /// <summary>
    /// 
    /// </summary>
    public class GridFrame
    {
        public Color BgTopColor { get; set; } = Color.Black;
        public Color BgBottomColor { get; set; } = Color.Black;
        public Color BgGridColor { get; set; } = Color.DarkGray;
        public System.Drawing.Font GridMeterFont { get; set; } = new System.Drawing.Font("굴림", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
        public System.Drawing.Brush GridMeterBrush { get; set; } = new SolidBrush(Color.DarkGray);
        public Pen GridLinePen { get; set; } = new Pen(Color.DarkGray, 2);
        public Pen GridDashLinePen { get; set; } = new Pen(Color.DarkGray, 2);
        public Pen GridDashLinePen2 { get; set; } = new Pen(Color.DarkGray, 1);
        public Pen SimpleGridLinePen { get; set; } = new Pen(Color.DarkGray, 2);
        public Pen RullerGridLinePen1 { get; set; } = new Pen(Color.DarkGray, 2);
        public Pen RullerGridLinePen2 { get; set; } = new Pen(Color.DarkGray, 1);
        public int GridXLines { get; set; } = 0;
        public int GridYLines { get; set; } = 0;
        public int GridYRange { get; set; }
        public int GridXRange { get; set; }
        public Pen OsDsPosXPen { get; set; } = new Pen(Color.Yellow, 2);
        public System.Drawing.Color OsDsPosXColor { get; set; } = Color.Yellow;

        private System.Drawing.Font osdsFont = new System.Drawing.Font(UserFont.Bold, 20);
        public float OsdsOpacity { get { return osdsOpacity; } set { osdsOpacity = value; } }
        private float osdsOpacity = 0.2f;
        public GridFrame()
        {
        }

#if USE_SHARP_DX
        private GradientStop[] gradientStops = null;
#else
        private D2D1GradientStop[] gradientStops = null;
#endif

#if USE_SHARP_DX
        public void Draw(SharpDX.Direct2D1.Factory factory, RenderTarget renderTarget, RectangleF real, RectangleF view, float width, float height, tagShowGridLine showGridLine, bool showGridLineMeter, bool showOS)
#else
        public void Draw(D2D1Factory factory, D2D1RenderTarget renderTarget, RectangleF real, RectangleF view, float width, float height, tagShowGridLine showGridLine, bool showGridLineMeter, bool showOS)
#endif
        {
#if USE_SHARP_DX
            var rbgn = new RawRectangleF(0f, 0f, width, height);

            if (renderTarget is WicRenderTarget)
            {
                BgTopColor = Color.White;
                BgBottomColor = Color.White;
                BgGridColor = Color.Black;
            }
            else
            {
                BgTopColor = Color.Black;
                BgBottomColor = Color.Black;
                BgGridColor = Color.Gray;
            }

            if (BgTopColor != BgBottomColor)
            {
                using (var backGradientStops = new GradientStopCollection(renderTarget, gradientStops))
                {
                    if (gradientStops == null)
                    {
                        gradientStops = new GradientStop[]
                        {
                           new GradientStop() {Position = 0f, Color = ColorConverter.ColorToRawColor(BgTopColor)},
                           new GradientStop() {Position = 1f, Color = ColorConverter.ColorToRawColor(BgBottomColor)}
                        };
                    }

                    using (var linearGradientBrush = new SharpDX.Direct2D1.LinearGradientBrush(renderTarget, new LinearGradientBrushProperties { StartPoint = new RawVector2(0, 0), EndPoint = new RawVector2(0, height) }, backGradientStops))
                    {
                        renderTarget.FillRectangle(rbgn, linearGradientBrush);
                    }
                }
            }
            else
            {
                using (var bgColorBrush = new SolidColorBrush(renderTarget, ColorConverter.ColorToRawColor(BgTopColor)))
                {
                    renderTarget.FillRectangle(rbgn, bgColorBrush);
                }
            }
#else
            var rbgn= new D2D1RectF(0f, 0f, width, height);

            if (BgTopColor != BgBottomColor)
            {
                using (var backGradientStops = renderTarget.CreateGradientStopCollection(gradientStops))
                {
                    if (gradientStops == null)
                    {
                        gradientStops = new D2D1GradientStop[]
                        {
                           new D2D1GradientStop() {Position = 0f, Color = new D2D1ColorF(ColorConverter.ColorToUint(BgTopColor))},
                           new D2D1GradientStop() {Position = 1f, Color = new D2D1ColorF(ColorConverter.ColorToUint(BgBottomColor))}
                        };
                    }

                    using (var linearGradientBrush = renderTarget.CreateLinearGradientBrush(new D2D1LinearGradientBrushProperties { StartPoint = new D2D1Point2F(0, 0), EndPoint = new D2D1Point2F(0, height) }, backGradientStops))
                    {
                        renderTarget.FillRectangle(rbgn, linearGradientBrush);
                    }
                }
            }
            else
            {
                using (var bgColorBrush = renderTarget.CreateSolidColorBrush(new D2D1ColorF(ColorConverter.ColorToUint(BgTopColor))))
                {
                    renderTarget.FillRectangle(rbgn, bgColorBrush);
                }
            }
#endif


            //M수 우측정렬
            //StringFormat stringFormat = new StringFormat();
            //stringFormat.Alignment = StringAlignment.Far;

            var scaleX = (double)view.Width / real.Width;
            var scaleY = (double)view.Height / real.Height;

            if (showGridLine == tagShowGridLine.All || showGridLine == tagShowGridLine.AllDash)
            {
                GridYRange = (int)(real.Height / 20);   //MD방향으로 기본 20개 라인을 긋는다
                GridXRange = 100;
            }
            else if (showGridLine == tagShowGridLine.AllSimple)
            {
                GridYRange = (int)(real.Height / 10); ; //MD방향으로 기본 10개 라인을 긋는다
                GridXRange = 500;
            }
            else if (showGridLine == tagShowGridLine.AllRuller)
            {
                GridYRange = (int)(real.Height / 4); ;  //MD방향으로 기본 4개 라인을 긋는다
                GridXRange = 100;
            }

#if USE_SHARP_DX
            var lineBrush = new SharpDX.Direct2D1.SolidColorBrush(renderTarget, ColorConverter.ColorToRawColor(BgGridColor));
            lineBrush.Opacity = 0.5f;
            var dWriteFactory = new SharpDX.DirectWrite.Factory(SharpDX.DirectWrite.FactoryType.Isolated);
            var textFormat = new SharpDX.DirectWrite.TextFormat(dWriteFactory, "굴림", null, FontWeight.Normal, SharpDX.DirectWrite.FontStyle.Normal, FontStretch.Normal, 12, "");
#else
            var lineBrush = renderTarget.CreateSolidColorBrush(new D2D1ColorF(D2D1KnownColor.DarkGray));
			lineBrush.Opacity = 0.5f;
            var dWriteFactory = DWriteFactory.Create(DWriteFactoryType.Isolated);
            var textFormat = dWriteFactory.CreateTextFormat("굴림", null, DWriteFontWeight.Normal, DWriteFontStyle.Normal, DWriteFontStretch.Normal, 12, "");
#endif

            if (showGridLine != tagShowGridLine.None)
            {
                if (showGridLine == tagShowGridLine.All || showGridLine == tagShowGridLine.Horizontal)
                {
                    if (GridYLines > 0)
                    {
                        for (int i = 0; i < GridYLines + 1; i++)
                        {
#if USE_SHARP_DX
                            renderTarget.DrawLine(new RawVector2(view.Left, (float)((i * (real.Height / GridYLines)) * scaleY + view.Top)),
                                new RawVector2(view.Right, (float)((i * (real.Height / GridYLines)) * scaleY + view.Top)), lineBrush, 2.5f);
#else
                            renderTarget.DrawLine(new D2D1Point2F(view.Left, (float)( ( i * ( real.Height / GridYLines ) ) * scaleY + view.Top )),
								new D2D1Point2F(view.Right, (float)( ( i * ( real.Height / GridYLines ) ) * scaleY + view.Top )), lineBrush, 2.5f);
#endif
                        }
                        if (showGridLineMeter)
                        {
                            lineBrush.Opacity = 0.9f;
                            for (int i = 0; i < GridYLines; i++)
                            {
                                string txt = ((real.Top + i * (real.Height / GridYLines)) / 1000).ToString() + "M";
#if USE_SHARP_DX
                                var rt = new RawRectangleF(view.Right - 50, (float)(((i * (real.Height / GridYLines)) * scaleY + view.Top)), view.Right, (float)(((i * (real.Height / GridYLines)) * scaleY + view.Top) + 15));
                                renderTarget.DrawText(txt, textFormat, rt, lineBrush);
#else
                                var rt = new D2D1RectF(view.Right - 50, (float)(((i * (real.Height / GridYLines)) * scaleY + view.Top)), view.Right, (float)(((i * (real.Height / GridYLines)) * scaleY + view.Top) + 15));
                                renderTarget.DrawText(txt, textFormat, rt, lineBrush);
#endif
                            }
                            lineBrush.Opacity = 0.5f;
                        }
                    }
                    else
                    {
                        int stt = (int)(((real.Top >= 0) ? (real.Top + GridYRange - 1) : (real.Top)) / GridYRange);
                        if (real.Top == 0) stt = 1; //시작점이 0M라면 0M의 Line은 그리지 않는다.
                        int end = (int)(((real.Bottom >= 0) ? (real.Bottom + GridYRange - 1) : (real.Bottom)) / GridYRange);

                        for (int i = stt; i < end; i++)
                        {
#if USE_SHARP_DX
                            renderTarget.DrawLine(new RawVector2(view.Left, (float)((i * GridYRange - real.Top) * scaleY + view.Top)),
                                new RawVector2(view.Right, (float)((i * GridYRange - real.Top) * scaleY + view.Top)), lineBrush, 2.5f);
#else
                            renderTarget.DrawLine(new D2D1Point2F(view.Left, (float)( ( i * GridYRange - real.Top ) * scaleY + view.Top )),
								new D2D1Point2F(view.Right, (float)( ( i * GridYRange - real.Top ) * scaleY + view.Top )), lineBrush, 2.5f);
#endif

                            if (showGridLineMeter)
                            {
                                lineBrush.Opacity = 0.9f;
                                string txt = (i * GridYRange / 1000).ToString() + "M";
#if USE_SHARP_DX
                                var rt = new RawRectangleF(view.Right - 50, (float)(((i * GridYRange - real.Top) * scaleY + view.Top)), view.Right, (float)(((i * GridYRange - real.Top) * scaleY + view.Top) + 15));
                                renderTarget.DrawText(txt, textFormat, rt, lineBrush);
#else
                                var rt = new D2D1RectF(view.Right-50, (float)(((i * GridYRange - real.Top) * scaleY + view.Top)), view.Right, (float)(((i * GridYRange - real.Top) * scaleY + view.Top) + 15));
                                renderTarget.DrawText(txt, textFormat, rt, lineBrush);
#endif
                                lineBrush.Opacity = 0.5f;
                            }
                        }
                    }
                }

                else if (showGridLine == tagShowGridLine.AllDash)
                {
                    int stt = (int)(((real.Top >= 0) ? (real.Top + GridYRange - 1) : (real.Top)) / GridYRange);
                    int end = (int)(((real.Bottom >= 0) ? (real.Bottom + GridYRange - 1) : (real.Bottom)) / GridYRange);
                    for (int i = stt; i < end; i++)
                    {
                        float strokeWidth = 2.0f;
                        bool isDot = false;
                        float[] dashes = { 2.0f, 2.0f, 2.0f };
                        string txt = (i * GridYRange / 1000).ToString() + "M";
                        if (i * GridYRange % (1000 * 1000) == 0)
                            strokeWidth = 2.0f;
                        else if (i * GridYRange % (100 * 1000) == 0)
                            strokeWidth = 2.0f;
                        else if (i * GridYRange % (10 * 1000) == 0)
                        {
                            strokeWidth = 2.0f;
                            isDot = true;
                            dashes[0] = dashes[1] = dashes[2] = 2.5f;
                        }
                        else if (i * GridYRange % (1 * 1000) == 0)
                        {
                            strokeWidth = 2.0f;
                            isDot = true;
                        }
                        if (isDot)
                        {
#if USE_SHARP_DX
                            var stokeStyleProperties = new StrokeStyleProperties
                            {
                                StartCap = CapStyle.Flat,
                                EndCap = CapStyle.Flat,
                                DashCap = CapStyle.Flat,
                                LineJoin = SharpDX.Direct2D1.LineJoin.Miter,
                                MiterLimit = 1.0f,
                                DashStyle = SharpDX.Direct2D1.DashStyle.Custom,
                                DashOffset = 0.0f
                            };
                            var st = new StrokeStyle(factory, stokeStyleProperties, dashes);
                            renderTarget.DrawLine(new RawVector2(view.Left, (float)((i * GridYRange - real.Top) * scaleY + view.Top)),
                                new RawVector2(view.Right, (float)((i * GridYRange - real.Top) * scaleY + view.Top)), lineBrush, strokeWidth, st);
#else
                            var st = factory.CreateStrokeStyle(new D2D1StrokeStyleProperties(D2D1CapStyle.Flat, D2D1CapStyle.Flat, D2D1CapStyle.Flat, D2D1LineJoin.Miter, 1.0f, D2D1DashStyle.Custom, 0.0f), dashes);
                            renderTarget.DrawLine(new D2D1Point2F(view.Left, (float)((i * GridYRange - real.Top) * scaleY + view.Top)),
                                new D2D1Point2F(view.Right, (float)((i * GridYRange - real.Top) * scaleY + view.Top)), lineBrush, strokeWidth, st);
#endif
                            st.Dispose();
                        }
                        else
#if USE_SHARP_DX
                            renderTarget.DrawLine(new RawVector2(view.Left, (float)((i * GridYRange - real.Top) * scaleY + view.Top)),
                                    new RawVector2(view.Right, (float)((i * GridYRange - real.Top) * scaleY + view.Top)), lineBrush, strokeWidth);
#else
                            renderTarget.DrawLine(new D2D1Point2F(view.Left, (float)((i * GridYRange - real.Top) * scaleY + view.Top)),
                                    new D2D1Point2F(view.Right, (float)((i * GridYRange - real.Top) * scaleY + view.Top)), lineBrush, strokeWidth);
#endif

                        if (showGridLineMeter)
                        {
                            lineBrush.Opacity = 0.9f;
#if USE_SHARP_DX
                            var rt = new RawRectangleF(view.Right - 50, (float)(((i * GridYRange - real.Top) * scaleY + view.Top)), view.Right, (float)(((i * GridYRange - real.Top) * scaleY + view.Top) + 15));
#else
                            var rt = new D2D1RectF(view.Right - 50, (float)(((i * GridYRange - real.Top) * scaleY + view.Top)), view.Right, (float)(((i * GridYRange - real.Top) * scaleY + view.Top) + 15));
#endif
                            renderTarget.DrawText(txt, textFormat, rt, lineBrush);
                            lineBrush.Opacity = 0.5f;
                        }
                    }
                }
                else if (showGridLine == tagShowGridLine.AllSimple)
                {
                    int stt = (int)(((real.Top >= 0) ? (real.Top + GridYRange - 1) : (real.Top)) / GridYRange);
                    int end = (int)(((real.Bottom >= 0) ? (real.Bottom + GridYRange - 1) : (real.Bottom)) / GridYRange);

                    for (int i = stt; i < end; i++)
                    {
#if USE_SHARP_DX
                        renderTarget.DrawLine(new RawVector2(view.Right - 80, (float)((i * GridYRange - real.Top) * scaleY + view.Top)),
                                new RawVector2(view.Right, (float)((i * GridYRange - real.Top) * scaleY + view.Top)), lineBrush);
#else
                        renderTarget.DrawLine(new D2D1Point2F(view.Right - 80, (float)( ( i * GridYRange - real.Top ) * scaleY + view.Top )),
							new D2D1Point2F(view.Right, (float)( ( i * GridYRange - real.Top ) * scaleY + view.Top )), lineBrush);
#endif

                        if (showGridLineMeter)
                        {
                            lineBrush.Opacity = 0.9f;
                            string txt = (i * GridYRange / 1000).ToString() + "M";
#if USE_SHARP_DX
                            var rt = new RawRectangleF(view.Right - 50, (float)(((i * GridYRange - real.Top) * scaleY + view.Top)), view.Right, (float)(((i * GridYRange - real.Top) * scaleY + view.Top) + 15));
#else
                            var rt = new D2D1RectF(view.Right - 50, (float)(((i * GridYRange - real.Top) * scaleY + view.Top)), view.Right, (float)(((i * GridYRange - real.Top) * scaleY + view.Top) + 15));
#endif
                            renderTarget.DrawText(txt, textFormat, rt, lineBrush);
                            lineBrush.Opacity = 0.5f;
                        }
                    }
                }
                else if (showGridLine == tagShowGridLine.AllRuller)
                {
                    int stt = (int)(((real.Top >= 0) ? (real.Top + GridYRange - 1) : (real.Top)) / GridYRange);
                    int end = (int)(((real.Bottom >= 0) ? (real.Bottom + GridYRange - 1) : (real.Bottom)) / GridYRange);
                    for (int i = stt; i < end; i++)
                    {
                        float strokeWidth = 2.0f;
                        int gridLineSttOffset = 0;
                        string txt = (i * GridYRange / 1000).ToString() + "M";
                        if (i * GridYRange % (1000 * 1000) == 0)
                        {
                            gridLineSttOffset = 80;
                        }
                        else if (i * GridYRange % (500 * 1000) == 0)
                        {
                            gridLineSttOffset = 70;
                        }
                        else if (i * GridYRange % (100 * 1000) == 0)
                        {
                            gridLineSttOffset = 60;
                        }
                        else if (i * GridYRange % (50 * 1000) == 0)
                        {
                            strokeWidth = 1.0f; gridLineSttOffset = 50;
                        }
                        else if (i * GridYRange % (10 * 1000) == 0)
                        {
                            strokeWidth = 1.0f; gridLineSttOffset = 45;
                        }
                        else if (i * GridYRange % (5 * 1000) == 0)
                        {
                            strokeWidth = 1.0f; gridLineSttOffset = 40;
                        }
#if USE_SHARP_DX
                        renderTarget.DrawLine(new RawVector2(view.Right - gridLineSttOffset, (float)((i * GridYRange - real.Top) * scaleY + view.Top)),
                                new RawVector2(view.Right, (float)((i * GridYRange - real.Top) * scaleY + view.Top)), lineBrush, strokeWidth);
#else
                        renderTarget.DrawLine(new D2D1Point2F(view.Right - gridLineSttOffset, (float)( ( i * GridYRange - real.Top ) * scaleY + view.Top )),
							new D2D1Point2F(view.Right, (float)( ( i * GridYRange - real.Top ) * scaleY + view.Top )), lineBrush, strokeWidth);
#endif

                        if (showGridLineMeter)
                        {
                            lineBrush.Opacity = 0.9f;
#if USE_SHARP_DX
                            var rt = new RawRectangleF(view.Right - 50, (float)(((i * GridYRange - real.Top) * scaleY + view.Top)), view.Right, (float)(((i * GridYRange - real.Top) * scaleY + view.Top) + 15));
#else
                            var rt = new D2D1RectF(view.Right - 50, (float)(((i * GridYRange - real.Top) * scaleY + view.Top)), view.Right, (float)(((i * GridYRange - real.Top) * scaleY + view.Top) + 15));
#endif
                            renderTarget.DrawText(txt, textFormat, rt, lineBrush);
                            lineBrush.Opacity = 0.5f;
                        }
                    }
                }

                if (showGridLine == tagShowGridLine.All)
                {
                    if (GridXLines > 0)
                    {
                        for (int i = 0; i < GridXLines + 1; i++)
                        {
#if USE_SHARP_DX
                            renderTarget.DrawLine(new RawVector2((float)((i * (view.Width / GridXLines)) * scaleX + view.Left), view.Top),
                                    new RawVector2((float)((i * (view.Width / GridXLines)) * scaleX + view.Left), view.Bottom), lineBrush, 2.5f);
#else
                            renderTarget.DrawLine(new D2D1Point2F((float)( ( i * ( view.Width / GridXLines ) ) * scaleX + view.Left ), view.Top),
								new D2D1Point2F((float)( ( i * ( view.Width / GridXLines ) ) * scaleX + view.Left ), view.Bottom), lineBrush, 2.5f);
#endif
                        }
                    }
                    else
                    {
                        int stt = (int)((real.Left + GridXRange - 1) / GridXRange);
                        int end = (int)((real.Right + GridXRange - 1) / GridXRange);

                        for (int i = stt; i < end - 1; i++) //M수가 가려져서 마지막라인 표시안함
                        {
#if USE_SHARP_DX
                            renderTarget.DrawLine(new RawVector2((float)((i * GridXRange) * scaleX + view.Left), view.Top),
                                    new RawVector2((float)((i * GridXRange) * scaleX + view.Left), view.Bottom), lineBrush, 2.5f);
#else
                            renderTarget.DrawLine(new D2D1Point2F((float)( ( i * GridXRange ) * scaleX + view.Left ), view.Top),
								new D2D1Point2F((float)( ( i * GridXRange ) * scaleX + view.Left ), view.Bottom), lineBrush, 2.5f);
#endif
                        }
                    }
                }
                else if (showGridLine == tagShowGridLine.AllDash)
                {
                    int stt = (int)((real.Left + GridXRange - 1) / GridXRange);
                    int end = (int)((real.Right + GridXRange - 1) / GridXRange);

                    for (int i = stt + 1; i < end - 1; i++)
                    {
                        if (i * GridXRange % (1000) == 0)
                        {
#if USE_SHARP_DX
                            renderTarget.DrawLine(new RawVector2((float)((i * GridXRange - real.Left) * scaleX + view.Left), view.Top),
                                    new RawVector2((float)((i * GridXRange - real.Left) * scaleX + view.Left), view.Bottom), lineBrush, 2.0f);
#else
                            renderTarget.DrawLine(new D2D1Point2F((float)( ( i * GridXRange - real.Left ) * scaleX + view.Left ), view.Top),
								new D2D1Point2F((float)( ( i * GridXRange - real.Left ) * scaleX + view.Left ), view.Bottom), lineBrush, 2.0f);
#endif
                        }
                        else if (i * GridXRange % (500) == 0)
                        {
#if USE_SHARP_DX
                            renderTarget.DrawLine(new RawVector2((float)((i * GridXRange - real.Left) * scaleX + view.Left), view.Top),
                                    new RawVector2((float)((i * GridXRange - real.Left) * scaleX + view.Left), view.Bottom), lineBrush, 1.0f);
#else
                            renderTarget.DrawLine(new D2D1Point2F((float)( ( i * GridXRange - real.Left ) * scaleX + view.Left ), view.Top),
								new D2D1Point2F((float)( ( i * GridXRange - real.Left ) * scaleX + view.Left ), view.Bottom), lineBrush, 1.0f);
#endif
                        }
                        else
                        {
#if USE_SHARP_DX
                            renderTarget.DrawLine(new RawVector2((float)((i * GridXRange - real.Left) * scaleX + view.Left), view.Top),
                                    new RawVector2((float)((i * GridXRange - real.Left) * scaleX + view.Left), view.Bottom), lineBrush, 2.0f);
#else
                            renderTarget.DrawLine(new D2D1Point2F((float)( ( i * GridXRange - real.Left ) * scaleX + view.Left ), view.Top),
								new D2D1Point2F((float)( ( i * GridXRange - real.Left ) * scaleX + view.Left ), view.Bottom), lineBrush, 2.0f);
#endif
                        }
                    }
                }
                else if (showGridLine == tagShowGridLine.AllRuller)
                {
                    int stt = (int)((real.Left + GridXRange - 1) / GridXRange);
                    int end = (int)((real.Right + GridXRange - 1) / GridXRange);

                    for (int i = stt + 1; i < end - 1; i++)
                    {
                        if (i * GridXRange % (1000) == 0)
                        {
#if USE_SHARP_DX
                            renderTarget.DrawLine(new RawVector2((float)((i * GridXRange - real.Left) * scaleX + view.Left), view.Top),
                                    new RawVector2((float)((i * GridXRange - real.Left) * scaleX + view.Left), view.Top + 30), lineBrush, 2.0f);

                            renderTarget.DrawLine(new RawVector2((float)((i * GridXRange - real.Left) * scaleX + view.Left), view.Bottom - 30),
                                    new RawVector2((float)((i * GridXRange - real.Left) * scaleX + view.Left), view.Bottom), lineBrush, 2.0f);
#else
                            renderTarget.DrawLine(new D2D1Point2F((float)( ( i * GridXRange - real.Left ) * scaleX + view.Left ), view.Top),
								new D2D1Point2F((float)( ( i * GridXRange - real.Left ) * scaleX + view.Left ), view.Top + 30), lineBrush, 2.0f);

                            renderTarget.DrawLine(new D2D1Point2F((float)( ( i * GridXRange - real.Left ) * scaleX + view.Left ), view.Bottom - 30),
								new D2D1Point2F((float)( ( i * GridXRange - real.Left ) * scaleX + view.Left ), view.Bottom), lineBrush, 2.0f);
#endif
                        }
                        else if (i * GridXRange % (500) == 0)
                        {
#if USE_SHARP_DX
                            renderTarget.DrawLine(new RawVector2((float)((i * GridXRange - real.Left) * scaleX + view.Left), view.Top),
                                    new RawVector2((float)((i * GridXRange - real.Left) * scaleX + view.Left), view.Top + 20), lineBrush, 1.0f);

                            renderTarget.DrawLine(new RawVector2((float)((i * GridXRange - real.Left) * scaleX + view.Left), view.Bottom - 20),
                                    new RawVector2((float)((i * GridXRange - real.Left) * scaleX + view.Left), view.Bottom), lineBrush, 1.0f);
#else
                            renderTarget.DrawLine(new D2D1Point2F((float)( ( i * GridXRange - real.Left ) * scaleX + view.Left ), view.Top),
								new D2D1Point2F((float)( ( i * GridXRange - real.Left ) * scaleX + view.Left ), view.Top + 20), lineBrush, 1.0f);

                            renderTarget.DrawLine(new D2D1Point2F((float)( ( i * GridXRange - real.Left ) * scaleX + view.Left ), view.Bottom - 20),
								new D2D1Point2F((float)( ( i * GridXRange - real.Left ) * scaleX + view.Left ), view.Bottom), lineBrush, 1.0f);
#endif
                        }
                    }

                    if (showGridLineMeter)
                    {
                        for (int i = stt + 1; i < end - 1; i++)
                        {
                            lineBrush.Opacity = 0.9f;
                            string txt = (i * GridXRange).ToString() + "mm";
#if USE_SHARP_DX
                            var rt = new RawRectangleF((float)((i * GridXRange - real.Left) * scaleX + view.Left) + 2, (float)(view.Bottom - 16), (float)((i * GridXRange - real.Left) * scaleX + view.Left) + 2, (float)(view.Bottom - 16));
#else
                            D2D1RectF rt = new D2D1RectF((float)((i * GridXRange - real.Left) * scaleX + view.Left) + 2, (float)(view.Bottom - 16), (float)((i * GridXRange - real.Left) * scaleX + view.Left) + 2, (float)(view.Bottom - 16));
#endif
                            renderTarget.DrawText(txt, textFormat, rt, lineBrush);
                            lineBrush.Opacity = 0.5f;
                        }
#if USE_SHARP_DX
                        renderTarget.DrawLine(new RawVector2(view.Left, view.Top), new RawVector2(view.Right, view.Top), lineBrush, 1.0f);
                        renderTarget.DrawLine(new RawVector2(view.Left, view.Bottom - 1), new RawVector2(view.Right, view.Bottom - 1), lineBrush, 1.0f);
#else
                        renderTarget.DrawLine(new D2D1Point2F(view.Left, view.Top), new D2D1Point2F(view.Right, view.Top), lineBrush, 1.0f);
                        renderTarget.DrawLine(new D2D1Point2F(view.Left, view.Bottom - 1), new D2D1Point2F(view.Right, view.Bottom - 1), lineBrush, 1.0f);
#endif
                    }
                }
                if (showOS)
                {
#if USE_SHARP_DX
                    var osDsFont = new SharpDX.DirectWrite.TextFormat(dWriteFactory, "굴림", null, FontWeight.Bold, SharpDX.DirectWrite.FontStyle.Normal, FontStretch.Normal, 20, "");
                    lineBrush.Color = ColorConverter.ColorToRawColor(OsDsPosXColor);
                    lineBrush.Opacity = osdsOpacity;
                    renderTarget.DrawText("OS", osDsFont, new RawRectangleF(3, 3, 35, 15), lineBrush);
#else
                    var osDsFont = dWriteFactory.CreateTextFormat("굴림", null, DWriteFontWeight.Bold, DWriteFontStyle.Normal, DWriteFontStretch.Normal, 20, "");
                    lineBrush.Color = new D2D1ColorF(255, 255, 255, 10);
                    lineBrush.Opacity = osdsOpacity;
                    renderTarget.DrawText("OS", osDsFont, new D2D1RectF(3,3,35,15), lineBrush);
#endif

                    osDsFont.Dispose();
                }

            }

            lineBrush?.Dispose();
            textFormat?.Dispose();
            dWriteFactory?.Dispose();
        }

#if USE_SHARP_DX
        public void DrawOsDsPosX(RenderTarget renderTarget, TextFormat txtFormat, SolidColorBrush brush, RectangleF real, RectangleF view, float width, float height, float os, float ds, int offsetY)
#else
        public void DrawOsDsPosX(D2D1RenderTarget renderTarget, DWriteTextFormat txtFormat, D2D1SolidColorBrush brush, RectangleF real, RectangleF view, float width, float height, float os, float ds, int offsetY)
#endif
        {
            Rectangle rbgn = new Rectangle(0, 0, (int)width, (int)height);

            var scaleX = (double)view.Width / real.Width;
            var scaleY = (double)view.Height / real.Height;

            if (offsetY > real.Top && offsetY < real.Bottom)
            {
                int margin = 1000;  //1000mm 아래 그린다
                int gridXRange = 200;
                int stt = (int)os;
                int end = (int)(ds + gridXRange - 1);

                var y = (float)((offsetY - real.Top + margin) * scaleY);

                for (int i = stt; i < end; i += gridXRange)
                {
                    string txt = string.Empty;
                    var pos = (float)(i * scaleX + view.Left);
                    if (i == stt)
                    {
                        txt = "OS";
                    }
                    else if (i >= ds)
                    {
                        txt = "DS";
                        pos = (float)(ds * scaleX + view.Left);
                    }
                    else if (i >= ds - gridXRange)
                    {
                        continue;
                    }
                    else
                    {
                        txt = (i - stt).ToString();
                    }
#if USE_SHARP_DX
                    brush.Color = ColorConverter.ColorToRawColor(OsDsPosXColor);
                    renderTarget.DrawLine(new RawVector2(pos, y), new RawVector2(pos, (float)(y + 10)), brush, 2f);

                    var rt = new RawRectangleF(pos, (float)(y + 10), (float)(pos + 30), (float)(y + 10));
                    renderTarget.DrawText(txt, txtFormat, rt, brush);
#else
                    brush.Color = new D2D1ColorF(ColorConverter.ColorToUint(Color.Yellow));
                    renderTarget.DrawLine(new D2D1Point2F(pos, y), new D2D1Point2F(pos, (float)(y + 10)), brush, 2f);

                    var rt = new D2D1RectF(pos, (float)(y + 10), (float)(pos + 30), (float)(y + 10));
                    renderTarget.DrawText(txt, txtFormat, rt, brush);
#endif
                }
#if USE_SHARP_DX
                renderTarget.DrawLine(new RawVector2((float)(view.Left), (float)y), new RawVector2((float)(view.Right), (float)y), brush, 2f);
#else
                renderTarget.DrawLine(new D2D1Point2F((float)(view.Left), (float)y), new D2D1Point2F((float)(view.Right), (float)y), brush, 2f);
#endif
            }
        }

#if USE_SHARP_DX
        public void DrawOpticDistance(RenderTarget renderTarget, TextFormat txtFormat, SolidColorBrush brush, RectangleF real, RectangleF view, float width, float height, double posY, float os = 0, float ds = 0)
#else
        public void DrawOpticDistance(D2D1RenderTarget renderTarget, DWriteTextFormat txtFormat, D2D1SolidColorBrush brush, RectangleF real, RectangleF view, float width, float height, double posY, float os = 0, float ds = 0)
#endif
        {
            Rectangle rbgn = new Rectangle(0, 0, (int)width, (int)height);

            var scaleX = (double)view.Width / real.Width;
            var scaleY = (double)view.Height / real.Height;

            var y = (float)((posY - real.Top) * scaleY + view.Top);

            bool drawBeside = false;
            //그려지는 곳이 밑에 붙어있으면, 옆에 그림
            if (y + 20 >= view.Bottom)
                drawBeside = true;

#if USE_SHARP_DX
            renderTarget.DrawLine(new RawVector2((float)(view.Left), (float)y), new RawVector2((float)(view.Right), (float)y), brush, 2f);
#else
                renderTarget.DrawLine(new D2D1Point2F((float)(view.Left), (float)y), new D2D1Point2F((float)(view.Right), (float)y), brush, 2f);
#endif
            if (os > 0 || ds > 0)
            {
                int gridXRange = 200;
                int stt = (int)os;
                int end = (int)(ds + gridXRange - 1);

                for (int i = stt; i < end; i += gridXRange)
                {
                    string txt = string.Empty;
                    var pos = (float)(i * scaleX + view.Left);
                    if (i == stt)
                    {
                        txt = "OS";
                    }
                    else if (i >= ds)
                    {
                        txt = "DS";
                        pos = (float)(ds * scaleX + view.Left);
                    }
                    else if (i >= ds - gridXRange)
                    {
                        continue;
                    }
                    else
                    {
                        txt = (i - stt).ToString();
                    }
#if USE_SHARP_DX
                    brush.Color = ColorConverter.ColorToRawColor(OsDsPosXColor);
                    renderTarget.DrawLine(new RawVector2(pos, y), new RawVector2(pos, (float)(y + 10)), brush, 2f);

                    if (!drawBeside)
                    {
                        var rt = new RawRectangleF(pos, (float)(y + 10), (float)(pos + 30), (float)(y + 10));
                        renderTarget.DrawText(txt, txtFormat, rt, brush);
                    }
                    else
                    {
                        var rt = new RawRectangleF(pos + 5, (float)(y + 3), (float)(pos + 35), (float)(y + 3));
                        renderTarget.DrawText(txt, txtFormat, rt, brush);
                    }

#else
                    brush.Color = new D2D1ColorF(ColorConverter.ColorToUint(Color.Yellow));
                    renderTarget.DrawLine(new D2D1Point2F(pos, y), new D2D1Point2F(pos, (float)(y + 10)), brush, 2f);

                    var rt = new D2D1RectF(pos, (float)(y + 10), (float)(pos + 30), (float)(y + 10));
                    renderTarget.DrawText(txt, txtFormat, rt, brush);
#endif
                }
            }
        }
    }

    public class MovingRect
    {
        public bool Moving { get; set; }
        public bool Show { get; set; }

        public void Init(bool show, int penWidth, Color backColor, Color foreColor)
        {
            Show = show;
            lock (drawLock)
            {
                if (this.backBrush != null)
                    this.backBrush.Dispose();
                this.backBrush = new SolidBrush(backColor);

                if (this.forePen != null)
                    this.forePen.Dispose();
                this.forePen = new Pen(foreColor, penWidth);
            }
        }

        public Rectangle GetRect()
        {
            return new Rectangle(
                Math.Min(left, right),
                Math.Min(top, bottom),
                Math.Abs(left - right),
                Math.Abs(top - bottom));
        }
        public void SetRect(Rectangle rect)
        {
            this.left = rect.Left;
            this.right = rect.Right;
            this.top = rect.Top;
            this.bottom = rect.Bottom;
        }
        public void SetRect(int left, int top, int right, int bottom)
        {
            this.left = left;
            this.right = right;
            this.top = top;
            this.bottom = bottom;
        }
#if USE_SHARP_DX
        public void Draw(RenderTarget renderTarget)
#else
        public void Draw(D2D1RenderTarget renderTarget)
#endif
        {
            if (Show)
            {
                var rect = GetRect();
#if USE_SHARP_DX
                var colorBrush = new SharpDX.Direct2D1.SolidColorBrush(renderTarget, ColorConverter.ColorToRawColor(backBrush.Color));
                renderTarget.FillRectangle(new RawRectangleF(rect.Left, rect.Top, rect.Right, rect.Bottom), colorBrush);
                colorBrush.Color = ColorConverter.ColorToRawColor(forePen.Color);
                renderTarget.DrawRectangle(new RawRectangleF(rect.Left, rect.Top, rect.Right, rect.Bottom), colorBrush);
#else
                var colorBrush = renderTarget.CreateSolidColorBrush(new D2D1ColorF(ColorConverter.ColorToUint(backBrush.Color)));
				renderTarget.FillRectangle(new D2D1RectF(rect.Left, rect.Top, rect.Right, rect.Bottom), colorBrush);
				colorBrush.Color = new D2D1ColorF(ColorConverter.ColorToUint(forePen.Color));
				renderTarget.DrawRectangle(new D2D1RectF(rect.Left, rect.Top, rect.Right, rect.Bottom), colorBrush);
#endif
                colorBrush?.Dispose();
            }
        }

        private int left;
        private int top;
        private int right;
        private int bottom;
        private Pen forePen;
        private SolidBrush backBrush;

        private object drawLock = new object();
    }

    public class Edge
    {
        public double X1 { get; set; }
        public double X2 { get; set; }
        public double Y { get; set; }
        public double Width { get; set; }
        public string DateTime { get; set; }

        public Edge()
        {
        }
        public Edge(Edge s)
        {
            X1 = s.X1;
            X2 = s.X2;
            Y = s.Y;
            Width = s.Width;
            DateTime = s.DateTime;
        }
        public Edge(double x1, double x2, double y, double width, string dateTime)
        {
            X1 = x1;
            X2 = x2;
            Y = y;
            Width = width;
            DateTime = dateTime;
        }
        public void Reset()
        {
            X1 = double.MinValue;
            X2 = double.MaxValue;
            Y = 0.0f;
            Width = 0.0f;
            DateTime = String.Empty;
        }
    }

    public class GridEdges
    {
        Color EdgeColor = Color.Blue;
        Color WidthColor = Color.Red;
        Color EdgeBgColor = Color.FromArgb(128, Color.DarkGray);
        Pen EdgePen = new Pen(Color.Blue);
        Pen WidthPen = new Pen(Color.Red);
        const int kMaxEdgeCount = 50000;
        public GridEdges()
        {

        }
        public void Clear()
        {
            lock (locker)
            {
                edges.Clear();
            }
        }

        public void RedrawAll()
        {
        }
        public void Add(Edge edge)
        {
            if (edges.Count < kMaxEdgeCount)
            {
                lock (locker)
                {
                    edges.Add(edge);
                }
            }
        }
        public void Add(Edge[] edge)
        {
            lock (locker)
            {
                foreach (var e in edge)
                {
                    edges.Add(e);
                }
            }
        }
        public float RecentX(int average, int type)
        {
            lock (locker)
            {
                double avg = 0.0f;
                int c = 0;
                for (int i = edges.Count - 1; i >= 0 && c < average; i--, c++)
                {
                    avg += type == 0 ? edges[i].X1 : edges[i].X2;
                }
                if (c > 0)
                {
                    avg /= c;
                }
                return (float)avg;
            }
        }
#if USE_SHARP_DX
        public void Draw(RenderTarget renderTarget, SharpDX.Direct2D1.Factory factory, SharpDX.Direct2D1.SolidColorBrush brush, RectangleF real, RectangleF view)
#else
        public void Draw(D2D1RenderTarget renderTarget, D2D1Factory factory, D2D1SolidColorBrush brush, RectangleF real, RectangleF view)
#endif
        {
            var ScaleRealX = (double)view.Width / real.Width;
            var ScaleRealY = (double)view.Height / real.Height;

            lock (locker)
            {
                //using (SolidBrush sb1 = new SolidBrush(EdgeBgColor))
                {
                    brush.Opacity = 0.3f;
#if USE_SHARP_DX
                    brush.Color = ColorConverter.ColorToRawColor(EdgeBgColor);
                    var pathGeometry = new PathGeometry(factory);
                    var points = new RawVector2[4];
#else
                    brush.Color = new D2D1ColorF(ColorConverter.ColorToUint(EdgeBgColor));
					var pathGeometry = factory.CreatePathGeometry();
                    var points = new D2D1Point2F[4];
#endif
                    var sink = pathGeometry.Open();
                    Edge prev = null;
                    foreach (var edge in edges)
                    {
                        if (edge.X1 < 0) edge.X1 = 0;
                        if (real.Contains((int)edge.X1, (int)(edge.Y * 1000)))
                        {
                            if (prev != null && prev.Y < edge.Y)
                            {
                                points[0].X = (float)((prev.X1 - real.X) * ScaleRealX + view.Left);
                                points[0].Y = (float)((prev.Y * 1000 - real.Y) * ScaleRealY + view.Top); //mm로 변경
                                points[3].X = (float)((prev.X2 - real.X) * ScaleRealX + view.Left);
                                points[3].Y = points[0].Y;
                                points[1].X = (float)((edge.X1 - real.X) * ScaleRealX + view.Left);
                                points[1].Y = (float)((edge.Y * 1000 - real.Y) * ScaleRealY + view.Top); //mm로 변경
                                points[2].X = (float)((edge.X2 - real.X) * ScaleRealX + view.Left);
                                points[2].Y = points[1].Y;
#if USE_SHARP_DX
                                sink.BeginFigure(points[0], FigureBegin.Filled);
                                sink.AddLines(points);
                                sink.EndFigure(FigureEnd.Closed);
#else
                                sink.BeginFigure(points[0], D2D1FigureBegin.Filled);
								sink.AddLines(points);
								sink.EndFigure(D2D1FigureEnd.Closed);
#endif
                                //g.FillPolygon(sb1, points);
                            }
                        }
                        prev = edge;
                    }

                    sink.Close();

                    renderTarget.FillGeometry(pathGeometry, brush);

#if USE_SHARP_DX
                    pathGeometry.Dispose();
                    sink.Release();
                    sink.Dispose();
#else
                    pathGeometry.Release();
					pathGeometry.Dispose();
					sink.Release();
					sink.Dispose();
#endif
                }
            }

            brush.Opacity = 1f;
        }

        private List<Edge> edges = new List<Edge>();
        private object locker = new object();
    }

    public class RollmapSheet
    {
        public PointF[] points = new PointF[4];
        public int Idx;
        public int ColIdx;
        public int IsSetted = 0;    // 0: 초기화 됨, 1: 양품 처리되어 있음, 2: 불량임.
        public bool IsNg = false;

        public RectangleF Rect
        {
            get
            {
                float minX = Math.Min(Math.Min(points[0].X, points[1].X), Math.Min(points[2].X, points[3].X));
                float minY = Math.Min(Math.Min(points[0].Y, points[1].Y), Math.Min(points[2].Y, points[3].Y));
                float maxX = Math.Max(Math.Max(points[0].X, points[1].X), Math.Max(points[2].X, points[3].X));
                float maxY = Math.Max(Math.Max(points[0].Y, points[1].Y), Math.Max(points[2].Y, points[3].Y));
                return new RectangleF(minX, minY, maxX - minX, maxY - minY);
            }
        }

        public RollmapSheet(RollmapSheet s)
        {
            points[0] = s.points[0];
            points[1] = s.points[1];
            points[2] = s.points[2];
            points[3] = s.points[3];
            Idx = s.Idx;
            IsNg = s.IsNg;
            ColIdx = s.ColIdx;
            IsSetted = s.IsSetted;
        }
        public RollmapSheet(int id, int colIdx, PointF ptLT, PointF ptRT, PointF ptRB, PointF ptLB)
        {
            points[0] = ptLT;
            points[1] = ptRT;
            points[2] = ptRB;
            points[3] = ptLB;
            Idx = id;
            ColIdx = colIdx;
        }

        public RollmapSheet(int id, int colIdx, double ltX, double ltY, double rtX, double rtY, double rbX, double rbY, double lbX, double lbY)
        {
            points[0] = new PointF((float)ltX, (float)ltY);
            points[1] = new PointF((float)rtX, (float)rtY);
            points[2] = new PointF((float)rbX, (float)rbY);
            points[3] = new PointF((float)lbX, (float)lbY);
            Idx = id;
            ColIdx = colIdx;
        }


#if USE_SHARP_DX
        public void Draw(RenderTarget renderTarget, SolidColorBrush brush, float stWidth, StrokeStyle st, RectangleF real, RectangleF view, Color color, float opacity = 0.4f)
#else
        public void Draw(D2D1RenderTarget renderTarget, DWriteTextLayout txtLayout, D2D1SolidColorBrush brush, RectangleF real, RectangleF view, string symbol, Font font, Color color)
#endif
        {
            try
            {
                var ScaleRealX = (double)view.Width / real.Width;
                var ScaleRealY = (double)view.Height / real.Height;

#if USE_SHARP_DX
                brush.Color = ColorConverter.ColorToRawColor(color);
#else
            brush.Color = new D2D1ColorF(ColorConverter.ColorToUint(color));
#endif
                brush.Opacity = opacity;

#if USE_SHARP_DX
                using (var path = new PathGeometry(renderTarget.Factory))
                {
                    using (var sink = path.Open())
                    {
                        sink.BeginFigure(new RawVector2((float)((this.points[0].X - real.X) * ScaleRealX + view.Left),
                                                        (float)(((this.points[0].Y - real.Y) * ScaleRealY + view.Top))), FigureBegin.Filled);
                        var len = points.Length;
                        for (var i = 1; i < len; ++i)
                        {
                            var pt = points[i];
                            sink.AddLine(new RawVector2((float)(((this.points[i].X - real.X) * ScaleRealX + view.Left)),
                                                        (float)(((this.points[i].Y - real.Y) * ScaleRealY + view.Top))));
                        }
                        sink.EndFigure(FigureEnd.Closed);
                        sink.Close();
                    }
                    renderTarget.DrawGeometry(path, brush, stWidth, st);
                }
#else
                renderTarget.DrawLine(new D2D1Point2F((float)(((this.points[0].X - real.X) * ScaleRealX + view.Left)),
                (float)(((this.points[0].Y - real.Y) * ScaleRealY + view.Top))),
                new D2D1Point2F((float)(((this.points[1].X - real.X) * ScaleRealX + view.Left)),
                (float)(((this.points[1].Y - real.Y) * ScaleRealY + view.Top))), 
                brush);

            renderTarget.DrawLine(new D2D1Point2F((float)(((this.points[1].X - real.X) * ScaleRealX + view.Left)),
                (float)(((this.points[1].Y - real.Y) * ScaleRealY + view.Top))),
                new D2D1Point2F((float)(((this.points[2].X - real.X) * ScaleRealX + view.Left)),
                (float)(((this.points[2].Y - real.Y) * ScaleRealY + view.Top))),
                brush);

            renderTarget.DrawLine(new D2D1Point2F((float)(((this.points[2].X - real.X) * ScaleRealX + view.Left)),
                (float)(((this.points[2].Y - real.Y) * ScaleRealY + view.Top))),
                new D2D1Point2F((float)(((this.points[3].X - real.X) * ScaleRealX + view.Left)),
                (float)(((this.points[3].Y - real.Y) * ScaleRealY + view.Top))),
                brush);

            renderTarget.DrawLine(new D2D1Point2F((float)(((this.points[3].X - real.X) * ScaleRealX + view.Left)),
                (float)(((this.points[3].Y - real.Y) * ScaleRealY + view.Top))),
                new D2D1Point2F((float)(((this.points[0].X - real.X) * ScaleRealX + view.Left)),
                (float)(((this.points[0].Y - real.Y) * ScaleRealY + view.Top))),
                brush);
#endif
            }
            catch
            {

            }
        }
    }

    /// <summary>
    /// 1M 단위 재단 상태
    /// </summary>
    public struct SummeryItem
    {
        public int Total;
        public int NgCount;
        public float SttX;
        public float EndX;
    }

    /// <summary>
    /// 1M 단위 재단 상태
    /// </summary>
    public class RollSummery
    {
        /// <summary>
        /// 몇 M 인지 표시
        /// </summary>
        public int IDX { get; private set; } = 0;
        public int Col { get; private set; } = 0;

        public SummeryItem this[int index]
        {
            get
            {
                return Items[index];
            }
            set
            {
                Items[index] = value;
            }
        }

        private SummeryItem[] Items = null;
        public RollSummery(int Pos, int count)
        {
            Set(Pos, count);
        }

        public void Set(int idx, int count)
        {
            IDX = idx;
            Col = count;

            Items = new SummeryItem[count];

            for (int i = 0; i < count; i++)
            {
                SummeryItem item = new SummeryItem();

                item.Total = 0;
                item.NgCount = 0;
                item.SttX = 0;
                item.EndX = 0;
                Items[i] = item;
            }
        }

        public void Reset()
        {
            for (int i = 0; i < Col; i++)
            {
                Items[i].Total = 0;
                Items[i].NgCount = 0;
                Items[i].SttX = 0;
                Items[i].EndX = 0;
            }
        }


    }

    public class GridRollSummery
    {
        const int arraySize = 10000;
        const int cntPerView = 20;
        public int ColCount { get; private set; }

        public Color[] MapColor { get; set; } = null;

        /// <summary>
        /// 10KM 기준으로 1M 단위로 Roll 요약 정보를 저장하도록 함.
        /// </summary>
        public RollSummery[] Summeries
        {
            get { return _summeries; }
            private set { _summeries = value; }
        }
        private RollSummery[] _summeries = new RollSummery[arraySize];

        public GridRollSummery()
        {
            CreateMapBaseColor();
        }

        public void CreateMapBaseColor()
        {
            MapColor = new Color[11];

            MapColor[0] = Color.Red;
            MapColor[1] = Color.Red;
            MapColor[2] = Color.Red;
            MapColor[3] = Color.Red;
            MapColor[4] = Color.Red;
            MapColor[5] = Color.Orange;
            MapColor[6] = Color.Yellow;
            MapColor[7] = Color.Green;
            MapColor[8] = Color.LightGreen;
            MapColor[9] = Color.Cyan;
            MapColor[10] = Color.Blue;
        }

        public void CreateSummery(int count)
        {
            ColCount = count;

            for (int i = 0; i < arraySize; i++)
                Summeries[i] = new RollSummery(i, count);
        }

        public void ResetSummery()
        {
            for (int i = 0; i < arraySize; i++)
                Summeries[i]?.Reset();

        }

        public void SetColPos(float posY, int idx, float sttX, float endX)
        {
            try
            {
                int row = (int)(posY / 1000.0);
                SummeryItem item = Summeries[row][idx];
                item.SttX = sttX;
                item.EndX = endX;
                Summeries[row][idx] = item;
            }
            catch (Exception ex)
            {
            }
        }

        public void SetData(double posY, int index, bool isNg, bool bAddTotal, bool bAddNG)
        {
            try
            {
                int row = (int)(posY / 1000.0);
                SummeryItem item = Summeries[row][index];

                // 중복으로 시트가 데이터를 넣을 수 있음. 시트가 최초로 데이터를 설정하는 것이라면 Count를 1 올려줌. 
                if (bAddTotal == true)
                    item.Total++;
                if (isNg == true && bAddNG == true) item.NgCount++;
                Summeries[row][index] = item;
            }
            catch (Exception ex)
            {
            }
        }

        public void Draw(RenderTarget renderTarget, RectangleF real, RectangleF view, float opacity = 0.3f)
        {
            var fieldBrush = new SharpDX.Direct2D1.SolidColorBrush(renderTarget, ColorConverter.ColorToRawColor(MapColor[0]));
            int i, idx, j;
            int brushIdx;
            try
            {
                int stY = (int)(real.Top / 1000.0f);
                int endY = (int)(real.Bottom / 1000.0f);

                int step = 1;
                if (endY - stY > cntPerView)
                {
                    step = (endY - stY) / cntPerView;
                }

                int total, ngCnt;
                float ySinkRange = (float)(step) * 1000.0f * 0.05f;
                // 그리기 해야함.....
                double scaleRX = (double)view.Width / real.Width;
                double scaleRY = (double)view.Height / real.Height;
                float sttX, endX;
                float sinkRange;

                if (stY < 0) stY = 0;

                for (i = stY; i <= endY; i += step)
                {

                    for (idx = 0; idx < ColCount; idx++)
                    {
                        total = ngCnt = 0;
                        for (j = i; j < i + step && j < Summeries.Length; j++) //인덱스가 배열을 넘기는 경우에 대한 예외 추가 @YKS 231123
                        {
                            if (j < 0) continue;
                            total += Summeries[j][idx].Total;
                            ngCnt += Summeries[j][idx].NgCount;
                        }
                        if (total > 0)
                        {
                            brushIdx = total == 0 ? 9 : (int)((float)(total - ngCnt) / (float)total * 10.0f);

                            fieldBrush.Color = ColorConverter.ColorToRawColor(MapColor[brushIdx]);
                            fieldBrush.Opacity = 0.3f;

                            if (ColCount == 1)
                            {
                                sttX = Summeries[i][idx].SttX;
                                endX = Summeries[i][idx].EndX;
                            }
                            else if (idx == 0)
                            {
                                sttX = Summeries[i][idx].SttX;
                                endX = (Summeries[i][idx].EndX + Summeries[i][idx + 1].SttX) / 2.0f;
                            }
                            else if (idx == ColCount - 1)
                            {
                                sttX = (Summeries[i][idx - 1].EndX + Summeries[i][idx].SttX) / 2.0f;
                                endX = Summeries[i][idx].EndX;
                            }
                            else
                            {
                                sttX = (Summeries[i][idx - 1].EndX + Summeries[i][idx].SttX) / 2.0f;
                                endX = (Summeries[i][idx].EndX + Summeries[i][idx + 1].SttX) / 2.0f;
                            }

                            // 영역 조정. 이웃 영역이 전부 붙어버림.... 
                            sinkRange = (endX - sttX) * 0.05f;
                            sttX += sinkRange;
                            endX -= sinkRange;

                            var drawRect = new RawRectangleF((float)((sttX - real.X) * scaleRX + view.Left), (float)(((double)i * 1000.0 - real.Y + ySinkRange) * scaleRY + view.Top),
                               (float)((endX - real.X) * scaleRX + view.Left), (float)(((double)(i + step) * 1000.0 - ySinkRange - real.Y) * scaleRY + view.Top));
                            renderTarget.FillRectangle(drawRect, fieldBrush);
                        }
                    }
                }


            }
            catch (Exception ex)
            {
            }
            finally
            {
                fieldBrush.Dispose();
            }
        }
    }

    public class GridSheet
    {
        public bool DisableDraw { get; set; } = false;
        public bool DrawReverse = false;

        public GridRollSummery Summery = null;

        public float MaxDrawingSheetRange { get; set; } = 20000.0f;

        public Queue<RollmapSheet> Sheets
        {
            get { return _sheets; }
            set
            {
                bool oldDisableDraw = DisableDraw;
                DisableDraw = true;
                _sheets = value;
                DisableDraw = oldDisableDraw;
            }
        }
        private Queue<RollmapSheet> _sheets = null;
        private Dictionary<int, RollmapSheet> dicSheets = new Dictionary<int, RollmapSheet>();

        private float _sttX;
        private float _endX;

        public Color NormalColor
        {
            get { return _normalColor; }
            set
            {
                _normalColor = value;
            }
        }
        private Color _normalColor = Color.Cyan;

        public Color NgColor
        {
            get { return _ngColor; }
            set { _ngColor = value; }
        }
        private Color _ngColor = Color.Magenta;


        public GridSheet()
        {
            _sheets = new Queue<RollmapSheet>();
            Summery = new GridRollSummery();
        }

        public void Term()
        {
            RemoveAll();
        }

        public void SetSheetXRange(int count, float stX, float edX)
        {
            _sttX = stX;
            _endX = edX;

            Summery.CreateSummery(count);
        }

        public void ResetSummery()
        {
            _sttX = _endX = 0;
            Summery.ResetSummery();
        }

        public void Add(int id, int colIdx, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
        {
            RollmapSheet sheet = new RollmapSheet(id, colIdx, x1, y1, x2, y2, x3, y3, x4, y4);
            try
            {
                bool oldDisableDraw = DisableDraw;
                DisableDraw = true;
                _sheets.Enqueue(sheet);
                dicSheets.Add(id, sheet);
                DisableDraw = oldDisableDraw;

                float posY = y1 > y2 ? y2 : y1;
                float sttX = x1;
                float endX = x1;
                if (sttX > x2) sttX = x2; if (sttX > x3) sttX = x3; if (sttX > x4) sttX = x4;
                if (endX < x2) endX = x2; if (endX < x3) endX = x3; if (endX < x4) endX = x4;

                Summery.SetColPos(posY, colIdx, sttX, endX);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        public void RemoveAll()
        {
            bool oldDisableDraw = DisableDraw;
            DisableDraw = true;
            dicSheets.Clear();
            Sheets.Clear();
            DisableDraw = oldDisableDraw;
        }

        public void SetStatus(int index, bool isNG)
        {
            if (dicSheets.ContainsKey(index) == true)
            {
                bool isCount = false;
                bool isNgCount = false;
                dicSheets[index].IsNg = isNG;

                RollmapSheet sheet = dicSheets[index];

                double posY = sheet.points[0].Y > sheet.points[1].Y ? sheet.points[1].Y : sheet.points[0].Y;

                if (sheet.IsSetted == 0) // 최초 1회면 카운트한다.        
                    isCount = true;
                if (sheet.IsSetted != 2 && isNG == true) // NG 처리가 되지 않았으면 NG 처리한다.
                    isNgCount = true;

                Summery.SetData(posY, sheet.ColIdx, isNG, isCount, isNgCount);

                if (isNG == true) sheet.IsSetted = 2;
                else if (sheet.IsSetted == 0 && isNG == false) sheet.IsSetted = 1;
            }
        }

#if USE_SHARP_DX
        public void Draw(RenderTarget renderTarget, SharpDX.Direct2D1.Factory factory, RectangleF real, RectangleF view)
#else
        public void Draw(D2D1RenderTarget renderTarget, DWriteFactory writeFactory, RectangleF real, RectangleF view)
#endif
        {
            if (Sheets == null) return;
            if (DisableDraw) return;
            float[] dashes = { 2.0f, 2.0f, 2.0f };
            var stokeStyleProperties = new StrokeStyleProperties
            {
                StartCap = CapStyle.Flat,
                EndCap = CapStyle.Flat,
                DashCap = CapStyle.Flat,
                LineJoin = SharpDX.Direct2D1.LineJoin.Miter,
                MiterLimit = 1.0f,
                DashStyle = SharpDX.Direct2D1.DashStyle.Custom,
                DashOffset = 0.0f
            };
            var st = new StrokeStyle(factory, stokeStyleProperties, dashes);
            var lineBrush = new SharpDX.Direct2D1.SolidColorBrush(renderTarget, ColorConverter.ColorToRawColor(_normalColor));

            try
            {
                var ScaleRealX = (double)view.Width / real.Width;
                var ScaleRealY = (double)view.Height / real.Height;

                lineBrush.Color = ColorConverter.ColorToRawColor(Color.Yellow);
                lineBrush.Opacity = 1.0f;
                if (this._sttX != 0 && this._endX != 0)
                {
                    renderTarget.DrawLine(new RawVector2((float)((this._sttX - real.X) * ScaleRealX + view.Left), view.Top),
                                      new RawVector2((float)((this._sttX - real.X) * ScaleRealX + view.Left), view.Bottom),
                                     lineBrush, 1.0f);

                    renderTarget.DrawLine(new RawVector2((float)((this._endX - real.X) * ScaleRealX + view.Left), view.Top),
                                          new RawVector2((float)((this._endX - real.X) * ScaleRealX + view.Left), view.Bottom),
                                         lineBrush, 1.0f);
                }

                // MaxDrawingSheetRange 이하인 경우에만 재단 이미지 표시
                if (real.Height < MaxDrawingSheetRange)
                {
                    foreach (var sheet in Sheets)
                    {
                        // 그리다가 그냥 냐가.... 
                        if (DisableDraw == true)
                        {
                            lineBrush.Dispose();
                            return;
                        }

                        if (real.Contains(sheet.Rect.Left, sheet.Rect.Top) || real.Contains(sheet.Rect.Right, sheet.Rect.Bottom) ||
                            real.Contains(sheet.Rect.Right, sheet.Rect.Top) || real.Contains(sheet.Rect.Left, sheet.Rect.Bottom))
                        {
                            if (sheet.IsNg == true) sheet.Draw(renderTarget, lineBrush, 1.0f, null, real, view, _ngColor, 1.0f);
                            else sheet.Draw(renderTarget, lineBrush, 1.0f, st, real, view, _normalColor);
                        }
                    }
                }
                else // 면적으로 나눠서 해당 영역 사각형으로 표시하고 수율을 처리한다. 
                {
                    Summery.Draw(renderTarget, real, view);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            finally
            {
                lineBrush.Dispose();
            }
        }
    }

    /// <summary>
    /// N사 좌표비교 시 사용할 데이터 구조체
    /// </summary>
    public struct NelCompareDefect
    {
        public NelCompareDefect(int optic, double posX, double posY, Color color, int cam, double size, string path, double calcX, double calcY, double sttPos, string date, int vValue = 0)
        {
            this.optic = optic;
            this.posX = posX;
            this.posY = posY;
            this.color = color;
            this.cam = cam;
            this.size = size;
            this.path = path;
            this.calcX = calcX;
            this.calcY = calcY;
            this.sttPos = sttPos;
            this.date = date;
            this.vValue = vValue;
        }
        public int optic { get; set; }
        public double posX { get; set; }
        public double posY { get; set; }
        public Color color { get; set; }
        public int cam { get; set; }
        public double size { get; set; }
        public string path { get; set; }
        public double calcX { get; set; }
        public double calcY { get; set; }
        public double sttPos { get; set; }
        public string date { get; set; }
        public int vValue { get; set; }
    }

    /// <summary>
    /// 이전공정 비교 시 사용할 구조체
    /// </summary>
    public struct PrevCompareDefect
    {
        public PrevCompareDefect(int optic, double posx, double posy, double size, int pcno, int id, Color color, string symbol)
        {
            this.optic = optic;
            this.posx = posx;
            this.posy = posy;
            this.size = size;
            this.pcno = pcno;
            this.id = id;
            this.color = color;
            this.symbol = symbol;
        }

        public int optic { get; set; }
        public double posx { get; set; }
        public double posy { get; set; }
        public double size { get; set; }
        public int pcno { get; set; }
        public int id { get; set; }
        public Color color { get; set; }
        public string symbol { get; set; }
    }

    public struct PrevErrorAreaPosition
    {
        public double startX { get; set; }
        public double startY { get; set; }
        public double endX { get; set; }
        public double endY { get; set; }
        public PrevErrorAreaPosition(double startX, double startY, double endX, double endY)
        {
            this.startX = startX;
            this.startY = startY;
            this.endX = endX;
            this.endY = endY;
        }
    }
}