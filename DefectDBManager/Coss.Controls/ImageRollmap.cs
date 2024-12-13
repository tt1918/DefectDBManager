using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace Coss.Controls
{
    public class MapViewOptionArgs
    {
        public MapViewOptionArgs(bool autoScroll, tagMapViewRange mapViewRange)
        {
            this.AutoScroll = autoScroll;
            this.mapViewRange = mapViewRange;
        }

        public bool AutoScroll;
        public enum tagMapViewRange
        {
            //Default = 0,       //default
            //Auto,           //실시간맵
            View200mm,
            View1M,
            View10M,
            View20M,
            View100M,
            View200M,         //사용자선택구간표시
            View500M,
            View1000M,
            Zoom,           //사용자정의
            Fit,            //0 ~ MapManualPosition 까지 표시 (프린트시사용)
            Whole,          //0 ~ MapLength 까지 표시
        }
        public tagMapViewRange mapViewRange;
    }

    public struct tagEnhanceImage
    {
        public int brightness;
        public float contrast;
        public int cutMin;
        public int cutMax;
        public tagEnhanceImage(int brightness, float contrast, int cutMin, int cutMax)
        {
            this.brightness = brightness;
            this.contrast = contrast;
            this.cutMin = cutMin;
            this.cutMax = cutMax;
        }
    }

    /// <summary>
    /// View: GDI 좌표
    /// Image: bitmap 좌표
    /// Real: mm 좌표
    /// </summary>
    public partial class ImageRollmap : UserControl
    {
        private Bitmap bitmap;
        private object bitmapLock = new Object();

        [Browsable(false)]
        public Rectangle BitmapRect { get; private set; }


        [Browsable(false)]
        public tagEnhanceImage EnhanceImageType { get; set; }


        [Category("ImageRollmap")]
        [DefaultValue(200 * 1000)]
        public int MapAutoRange { get; set; } = 200 * 1000;

        private int mapAutoPosion = 0;
        [Category("ImageRollmap")]
        [DefaultValue(0)]
        public int MapAutoPosion
        {
            get { return mapAutoPosion; }
            set
            {
                mapAutoPosion = value;
            }
        }

        [Category("ImageRollmap")]
        [DefaultValue(200 * 1000)]
        public int MapManualRange { get; set; } = 200 * 1000;

        private int fitLength = 2000 * 1000;
        [Category("ImageRollmap")]
        [DefaultValue(2000 * 1000)]
        public int FitLength
        {
            get { return fitLength; }
            set
            {
                fitLength = value;
                if (mapViewOption.mapViewRange == MapViewOptionArgs.tagMapViewRange.Fit)
                    UpdateImageRect();
            }
        }

        [Category("ImageRollmap")]
        [DefaultValue(4000 * 1000)]
        public int WholeRange { get; set; } = 4000 * 1000;

        private MapViewOptionArgs mapViewOption = new MapViewOptionArgs(false, MapViewOptionArgs.tagMapViewRange.Whole);
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
            SetMapViewOption(mapViewOption.AutoScroll, value);
        }
        public void MapViewOption(MapViewOptionArgs value, Rectangle viewRealRect, bool update)
        {
            if(viewRealRect != Rectangle.Empty)
                SetViewRealImageRect(viewRealRect);

            SetMapViewOption(value.AutoScroll, value.mapViewRange, forceUpdate:update);
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
            int y =0;
            if (!lastClicked.IsEmpty)
            {
                x = lastClicked.X;
                y = lastClicked.Y;
            }

            SetMapViewOption(mapViewOption.AutoScroll, MapViewOptionArgs.tagMapViewRange.Zoom, x, y, zoom);
        }

        private int mapViewRange = 4000*1000;

        [Category("ImageRollmap")]
        [DefaultValue(1.7)]
        public double MapImageScaleX { get; set; } = 2.23f;

        [Category("ImageRollmap")]
        [DefaultValue(102.4)]
        public double MapImageScaleY { get; set; } = 102.4f;


        [Browsable(false)]
        public bool NeedDispose { get; set; } = true;

        [Category("ImageRollmap")]
        public RollmapViewAreaChangedHandler ViewAreaChanged;

        public ImageRollmap()
        {
            this.SetStyle(ControlStyles.DoubleBuffer, true);
            this.SetStyle(ControlStyles.UserPaint, true);
            this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);

            InitializeComponent();

            options[kOptionGuiThema] = Color.Black.ToString();
        }
        public void LoadImage(Bitmap bitmap)
        {
            lock(bitmapLock)
            {
                if (NeedDispose && this.bitmap!=null)
                {
                    this.bitmap.Dispose();
                    this.bitmap = null;
                }
                this.bitmap = bitmap;

                BitmapRect = new Rectangle(0, 0, this.bitmap.Width, this.bitmap.Height);
                viewImageRect = new Rectangle(0, 0, this.bitmap.Width, this.bitmap.Height);
            }
            UpdateImageRect(true);
        }

        public void SetImageData(Rectangle rect, byte[] data)
        {
            if (this.bitmap == null) return;
            lock (bitmapLock)
            {
                BitmapData bmp = this.bitmap.LockBits(rect, ImageLockMode.WriteOnly, PixelFormat.Format8bppIndexed);
                EnhanceImage(EnhanceImageType, ref data);
                Marshal.Copy(data, 0, bmp.Scan0, data.Length);
                this.bitmap.UnlockBits(bmp);
            }
        }
        public void CopyImageData(Rectangle src, Rectangle dst, int length)
        {
            if (this.bitmap == null) return;
            byte[] data = new byte[length];

            lock (bitmapLock)
            {
                BitmapData bmpSrc = this.bitmap.LockBits(src, ImageLockMode.WriteOnly, PixelFormat.Format8bppIndexed);
                Marshal.Copy(bmpSrc.Scan0, data, 0, length);
                this.bitmap.UnlockBits(bmpSrc);

                BitmapData bmpDst = this.bitmap.LockBits(dst, ImageLockMode.WriteOnly, PixelFormat.Format8bppIndexed);
                Marshal.Copy(data, 0, bmpDst.Scan0, length);
                this.bitmap.UnlockBits(bmpDst);
            }
        }

        public Color GetImagePixel(int x, int y)
        {
            if (this.bitmap == null) return Color.Black;
            lock (bitmapLock)
            {
                return this.bitmap.GetPixel(x, y);
            }
        }

        public void SetEnhanceImage(tagEnhanceImage type)
        {
            EnhanceImageType = type;
        }

        public void EnhanceImage(tagEnhanceImage type, ref byte[] data)
        {
            float a = type.contrast;
            int b = type.brightness;
            int cutMin = type.cutMin;
            int cutMax = type.cutMax;
            byte pixel;
            float tmp;
            for (int i = 0; i < data.Length; i++)
            {
                pixel = data[i];
                if (pixel > 0 && (pixel >= cutMin && pixel <= cutMax) && cutMax > 0)
                    tmp = a * cutMin + b;
                else
                    tmp = a * pixel + b;

                if (tmp > 255) data[i] = 255;
                else if(tmp < 0) data[i] = 0;
                else data[i] = (byte)tmp;
            }
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

        /// <summary>
        /// real 좌표로 표시영역 수신하면 image좌표로 변환해서 표시
        /// </summary>
        public void OnViewAreaChanged(MapViewOptionArgs viewOption, Rectangle rect)
        {
            viewImageRect = ConvertRealToImage(rect);
            Invalidate();
        }

        public void SignalCurViewImageRect()
        {
            if (ViewAreaChanged != null)
            {
                //Trace.WriteLine(String.Format("1 {0},{1},{2},{3}", viewImageRect.Left, viewImageRect.Top, viewImageRect.Right, viewImageRect.Bottom));

                var real = ConvertImageToReal(viewImageRect);
                ViewAreaChanged(this, mapViewOption, real);
            }
        }

        private void SetViewRealImageRect(Rectangle realRect)
        {
            viewImageRect = ConvertRealToImage(realRect);
            //Trace.WriteLine(String.Format("2 {0},{1},{2},{3}", viewImageRect.Left, viewImageRect.Top, viewImageRect.Right, viewImageRect.Bottom));
        }

        public void Redraw()
        {
            UpdateImageRect();
        }

        public ImagemapMouseClickedHandler MouseDoubleClicked;
        public ImagemapMouseClickedHandler MouseClicked;
        public static readonly String kOptionGuiThema = "gui.thema";

        public Rectangle ViewImageRect { get { return viewImageRect; } }
        private Rectangle viewImageRect;

        private Rectangle viewImageRectOld;
        private Dictionary<String, String> options = new Dictionary<String, String>();
        private GridFrameSimple frames = new GridFrameSimple(10 * 1000, 100 * 1000, 500 * 1000);    //100m마다, 500M마다 거리 표시

        private int eachImageOffset = 0;
        private int eachImageWidth = 128;
        private double[] calXs;
        private double[] pixelScales;
        private int[] ClickedXoffset = new int[2];

        public void SetCamInfo(double[] calX, double[] scale, int eachImageWidth, int eachImageOffset)
        {
            if (calX.Length != scale.Length)
                return;

            if(calXs == null || calX.Length != calXs.Length)
            {
                if (calXs != null)
                    calXs = null;
                calXs = new double[calX.Length];
            }
            for (int i=0;i< calX.Length;i++)
                calXs[i] = calX[i];

            if (pixelScales == null || scale.Length != pixelScales.Length)
            {
                if (pixelScales != null)
                    pixelScales = null;
                pixelScales = new double[scale.Length];
            }
            for (int i = 0; i < scale.Length; i++)
                pixelScales[i] = scale[i];

            this.eachImageWidth = eachImageWidth;
            this.eachImageOffset = eachImageOffset;
        }

        #region ConvertCoordinate
        private Point ConvertViewToReal(Point view)
        {
            return new Point((int)(MapImageScaleX * (viewImageRect.X + (double)view.X * viewImageRect.Width / this.Width)),
                (int)(MapImageScaleY * (viewImageRect.Y + (double)view.Y * viewImageRect.Height / this.Height)));
        }

        private Rectangle ConvertViewToReal(Rectangle view)
        {
            return new Rectangle(
                (int)(MapImageScaleX * ((double)view.X * viewImageRect.Width / this.Width)),
                (int)(MapImageScaleY * ((double)view.Y * viewImageRect.Height / this.Height)),
                (int)(MapImageScaleX * ((double)view.Width * viewImageRect.Width / this.Width)),
                (int)(MapImageScaleX * ((double)view.Height * viewImageRect.Height / this.Height)));
        }

        private Point ConvertViewToImage(Point view)
        {
            return new Point((int)(viewImageRect.X + (double)view.X * viewImageRect.Width / this.Width),
                (int)(viewImageRect.Y + (double)view.Y * viewImageRect.Height / this.Height));
        }

        private Rectangle ConvertViewToImage(Rectangle view)
        {
            return new Rectangle(
                (int)((double)view.X * viewImageRect.Width / this.Width), (int)((double)view.Y * viewImageRect.Height / this.Height),
                (int)((double)view.Width * viewImageRect.Width / this.Width), (int)((double)view.Height * viewImageRect.Height / this.Height));
        }

        public Point ConvertRealToImage(Point real)
        {
            return new Point((int)((double)real.X / MapImageScaleX), (int)((double)real.Y / MapImageScaleY));
        }

        private Rectangle ConvertRealToImage(Rectangle real)
        {
            return new Rectangle((int)((double)real.X / MapImageScaleX), (int)((double)real.Y / MapImageScaleY),
                (int)((double)real.Width / MapImageScaleX), (int)((double)real.Height / MapImageScaleY));
        }
        public Point ConvertImageToReal(Point image)
        {
            return new Point((int)((double)image.X * MapImageScaleX), (int)((double)image.Y * MapImageScaleY));
        }

        public Rectangle ConvertImageToReal(Rectangle image)
        {
            return new Rectangle((int)((double)image.X * MapImageScaleX), (int)((double)image.Y * MapImageScaleY),
                (int)((double)image.Width * MapImageScaleX), (int)((double)image.Height * MapImageScaleY));
        }
        private Rectangle ConvertImageToView(Rectangle image)
        {
            int height = (int)((double)image.Height * this.Height / viewImageRect.Height);
            if (height < 1) height = 1;
            return new Rectangle((int)((double)image.X * this.Width / viewImageRect.Width), (int)((double)image.Y * this.Height / viewImageRect.Height),
                (int)((double)image.Width * this.Width / viewImageRect.Width), height);
        }
        #endregion


        private void SetMapViewOption(bool autoScroll, MapViewOptionArgs.tagMapViewRange viewRange, 
                                      int x = 0, int y = 0, float zoom = 1.0f, bool forceUpdate = false)
        {
            if (mapViewOption.AutoScroll != autoScroll || mapViewOption.mapViewRange != viewRange || zoom != 1.0f || forceUpdate)
            {
                int range = 0;
                switch (viewRange)
                {
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
                        range = WholeRange;
                        break;
                    case MapViewOptionArgs.tagMapViewRange.Zoom:
                        range = (int)(mapViewRange * zoom);
                        if (range > WholeRange)
                            range = WholeRange;
                        else if (range < 10 * 1000)
                            return;
                        break;
                    default:
                        range = WholeRange;
                        break;
                }
                mapViewOption = new MapViewOptionArgs(autoScroll, viewRange);
                mapViewRange = range;
                UpdateImageRect(true, x, y);
            }
        }
        private void UpdateImageRect(bool changed = false, int x = 0, int y = 0)
        {
            if (BitmapRect.Width == 0 || BitmapRect.Height == 0) return;

            int range = mapViewRange;

            if (mapViewOption.mapViewRange == MapViewOptionArgs.tagMapViewRange.Whole)
            {
                viewImageRect = new Rectangle(0, 0, BitmapRect.Width, BitmapRect.Height);
            }
            else if(mapViewOption.mapViewRange == MapViewOptionArgs.tagMapViewRange.Fit)
            {
                viewImageRect = new Rectangle(0, 0, BitmapRect.Width, (int)(fitLength / MapImageScaleY));
            }
            else
            {
                if (mapViewOption.AutoScroll)
                {
                    var destRealRect = new Rectangle(0, 0, (int)(BitmapRect.Width * MapImageScaleX), (int)(BitmapRect.Height * MapImageScaleY));
                    destRealRect.Y = MapAutoPosion - mapViewRange;
                    destRealRect.Height = mapViewRange;

                    var destImageRect = ConvertRealToImage(destRealRect);
                    if (destImageRect.Bottom > BitmapRect.Height) destImageRect.Y = destImageRect.Bottom - (int)(mapViewRange / MapImageScaleY);
                    if (destImageRect.Top < 0) destImageRect.Y = 0;

                    viewImageRect = destImageRect;
                }
                else
                {
                    if(x > 0 && y > 0 || changed == true)
                    {
                        var clicked = ConvertViewToReal(new Point(x, y));
                        var destRealRect = new Rectangle(0, 0, (int)(BitmapRect.Width * MapImageScaleX), (int)(BitmapRect.Height * MapImageScaleY));
                        destRealRect.Y = clicked.Y - (int)((double)y / this.Size.Height * range);
                        destRealRect.Height = mapViewRange;

                        var destImageRect = ConvertRealToImage(destRealRect);
                        if (destImageRect.Bottom > BitmapRect.Height) destImageRect.Y = destImageRect.Bottom - (int)(mapViewRange / MapImageScaleY);
                        if (destImageRect.Top < 0) destImageRect.Y = 0;

                        viewImageRect = destImageRect;
                    }
                }
            }

            if (viewImageRectOld != viewImageRect || changed == true)
            {
                SignalCurViewImageRect();
                viewImageRectOld = viewImageRect;
            }

            Invalidate();
        }

        private void SetGuiThema(string value)
        {
            if (value == Color.Black.ToString())
            {
                frames.BgBottomColor = Color.Black;
                frames.BgTopColor = Color.FromArgb(unchecked((int)0xFF303030));
            }
            else if (value == Color.White.ToString())
            {
                frames.BgBottomColor = Color.White;
                frames.BgTopColor = Color.White;
            }
            Invalidate();
        }

        public void Redraw(Rectangle rect)
        {
            //TODO. ConvertImageToView 재작성필요
            //var rc = ConvertImageToView(rect);
            Invalidate(false);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            try
            {
                using (var bufferedgraphic = BufferedGraphicsManager.Current.Allocate(e.Graphics, this.ClientRectangle))
                {
                    bufferedgraphic.Graphics.Clear(Color.Silver);
                    bufferedgraphic.Graphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.Bilinear;
                    bufferedgraphic.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighSpeed;
                    bufferedgraphic.Graphics.TranslateTransform(this.AutoScrollPosition.X, this.AutoScrollPosition.Y);
                    if(this.bitmap != null)
                    {
                        lock (bitmapLock)
                        {
                            bufferedgraphic.Graphics.DrawImage((Bitmap)this.bitmap.Clone(), new Rectangle(0, 0, this.Width, this.Height), viewImageRect, GraphicsUnit.Pixel);
                        }
                    }
                    frames.Draw(bufferedgraphic.Graphics, ConvertImageToReal(viewImageRect), this.Width, this.Height, 0, 0);
                    bufferedgraphic.Render(e.Graphics);
                }
            }
            catch (Exception ex)
            {
                Console.Write("exception : " + ex.Message);
            }
            base.OnPaint(e);
        }

        private Point lastClicked = Point.Empty;

        private void ImageRollmap_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            var rPt = ConvertViewToReal(e.Location);
            var iPt = ConvertViewToImage(e.Location);
            if (MouseDoubleClicked != null) MouseDoubleClicked(rPt, iPt, e.Location, string.Empty);

            lastClicked = e.Location;
        }

        private void ImageRollmap_MouseWheel(object sender, MouseEventArgs e)
        {
            if (this.bitmap == null) return;
            if(Control.ModifierKeys == Keys.Control)
            {
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
                if (mapViewOption.mapViewRange != MapViewOptionArgs.tagMapViewRange.Whole)
                {
                    var rect = viewImageRect;
                    rect.Offset(0, e.Delta > 0 ? -1 * viewImageRect.Height : viewImageRect.Height);

                    int oldH = rect.Height;
                    if (rect.Top < 0)
                    {
                        rect.Y = 0;
                        rect.Height = oldH;
                    }
                    if (rect.Bottom > BitmapRect.Height)
                    {
                        rect.Y = BitmapRect.Height - oldH;
                    }
                    viewImageRect = rect;
                    UpdateImageRect();
                }
            }
        }

        public int GetPosX(byte[] data, bool os, int x)
        {
            if (x < 0) return -1;
            if (x > data.Length) return -1;

            byte refValue = 0;
            if (os)
            {
                for (int i = 0; i < data.Length / 2; i++)
                {
                    if (i == 0) refValue = data[i];
                    if (data[i] != refValue)
                    {
                        return i;
                    }
                }
            }
            else
            {
                for (int i = data.Length - 1; i >= data.Length / 2; i--)
                {
                    if (i == data.Length - 1) refValue = data[i];
                    if (data[i] != refValue)
                    {
                        return i;
                    }
                }
            }
            return 0;
        }


        private void ImageRollmap_MouseUp(object sender, MouseEventArgs e)
        {
            var rPt = ConvertViewToReal(e.Location);
            var iPt = ConvertViewToImage(e.Location);

            var view = e.Location;
            int imageX = (int)(viewImageRect.X + (double)view.X * viewImageRect.Width / this.Width);
            int index = imageX / eachImageWidth;
            int offset = imageX % eachImageWidth + eachImageOffset;
            if (index < 0 || index >= calXs.Length)
                return;

            int realX = (int)(calXs[index] + pixelScales[index] * offset);
            //real x 및 image x 모두 offset 보정된 위치를 넘겨준다
            rPt.X = realX;
            iPt.X = iPt.X + (index * 2 + 1 ) * eachImageOffset;

            //클릭좌표의 OS, DS 경계 Pos X를 구함
            int width = BitmapRect.Width;
            byte[] data = new byte[width];
            Rectangle src = new Rectangle(0, iPt.Y, width, 1);

            BitmapData bmpSrc=null;
            int os = 0;
            int ds = 0;
            try
            {
                lock (this.bitmap)
                {
                    bmpSrc = this.bitmap.LockBits(src, ImageLockMode.WriteOnly, PixelFormat.Format8bppIndexed);
                    Marshal.Copy(bmpSrc.Scan0, data, 0, width);
                    this.bitmap.UnlockBits(bmpSrc);
                    bmpSrc = null;

                    os = GetPosX(data, true, iPt.X);
                    ds = GetPosX(data, false, iPt.X);
                }
            }
            catch(Exception ee)
            {
                //드물게 bitmap이 다른데서 사용중이라고 예외발생함. 무시하고 재클릭하도록 함
                Trace.WriteLine(ee);
            }
            finally
            {
                if(bmpSrc!=null)
                {
                    this.bitmap.UnlockBits(bmpSrc);
                    bmpSrc = null;
                }
            }
            string osds = string.Empty;

            if(os > 0 && ds > 0)
            {
                int index_os = os / eachImageWidth;
                int offset_os = os % eachImageWidth + eachImageOffset;
                int index_ds = ds / eachImageWidth;
                int offset_ds = ds % eachImageWidth + eachImageOffset;
                if (index_os >= 0 && index_os < calXs.Length && index_ds >= 0 && index_ds < calXs.Length)
                {
                    int realOs = (int)(calXs[index_os] + pixelScales[index_os] * offset_os);
                    int realDs = (int)(calXs[index_ds] + pixelScales[index_ds] * offset_ds);

                    osds = String.Format("OS={0:f1}mm, DS={1:f1}mm", realX- realOs + ClickedXoffset[0], realDs- realX + ClickedXoffset[1]);
                }
            }

            if (MouseClicked!=null) MouseClicked(rPt, iPt, e.Location, osds);

            lastClicked = e.Location;
        }
        public void SetXOffset(int os, int ds)
        {
            ClickedXoffset[0] = os;
            ClickedXoffset[1] = ds;
        }
    }
}
