using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
#if USE_SHARP_DX
using SharpDX.Direct2D1;
//using SharpDX.DirectWrite;
using SharpDX.Mathematics.Interop;
#else
using JeremyAnsel.DirectX.D2D1;
using JeremyAnsel.DirectX.DWrite;
#endif

namespace Coss.Controls
{
    public class RollmapGraphics
    {
        public enum GraphicType
        {
            Text = 0,
            Cross,
            Rect,
            Line,
            Circle,
            RectAngle,
            Ellipse,
            Pixels
        }

        GraphicType type_;
        Font font_;
        string text_;
        System.Drawing.Brush brush_;
        System.Drawing.Color color_;
        Pen pen_;
        Point[] points_;
        UInt32 pixelColor_;
        int x1_, y1_;
        int x2_, y2_;
        int size_;

        public RollmapGraphics(GraphicType type, int x, int y, string text, Font font, Color color)
        {
            type_ = type;
            x1_ = x;
            y1_ = y;
            text_ = text;
            if (font_ != null)
            {
                font_.Dispose();
                font_ = null;
            }
            font_ = (Font)font.Clone();
            if (brush_ != null)
            {
                brush_.Dispose();
                brush_ = null;
            }
            brush_ = new SolidBrush(color);
            color_ = color;
        }
        public RollmapGraphics(GraphicType type, int x, int y, Pen pen)
        {
            type_ = type;
            x1_ = x;
            y1_ = y;
            if (pen_ != null)
            {
                pen_.Dispose();
                pen_ = null;
            }
            pen_ = (Pen)pen.Clone();
        }
        public RollmapGraphics(GraphicType type, int x1, int y1, int x2, int y2, Pen pen)
        {
            type_ = type;
            x1_ = x1;
            y1_ = y1;
            x2_ = x2;
            y2_ = y2;
            if (pen_ != null)
            {
                pen_.Dispose();
                pen_ = null;
            }
            pen_ = (Pen)pen.Clone();
            points_ = new Point[4] { new Point(x1_, y1_), new Point(x2_, y1_), new Point(x2_, y2_), new Point(x1_, y2_) };
            color_ = pen.Color;
        }
        public RollmapGraphics(GraphicType type, Rectangle rect, Pen pen)
        {
            type_ = type;
            x1_ = rect.Left;
            y1_ = rect.Top;
            x2_ = rect.Right;
            y2_ = rect.Bottom;
            if (pen_ != null)
            {
                pen_.Dispose();
                pen_ = null;
            }
            pen_ = (Pen)pen.Clone();
            points_ = new Point[4] { new Point(x1_, y1_), new Point(x2_, y1_), new Point(x2_, y2_), new Point(x1_, y2_) };
        }

        public RollmapGraphics(GraphicType type, int x, int y, int size, Pen pen)
        {
            type_ = type;
            x1_ = x;
            y1_ = y;
            size_ = size;
            if (pen_ != null)
            {
                pen_.Dispose();
                pen_ = null;
            }
            pen_ = (Pen)pen.Clone();
        }
        public RollmapGraphics(GraphicType type, Point[] point, Pen pen)
        {
            type_ = type;
            points_ = new Point[4];
            Array.Copy(point, points_, point.Length);
            if (pen_ != null)
            {
                pen_.Dispose();
                pen_ = null;
            }
            pen_ = (Pen)pen.Clone();
        }
        public RollmapGraphics(GraphicType type, Point[] point, Color color)
        {
            type_ = type;
            points_ = new Point[point.Length];
            Array.Copy(point, points_, point.Length);
            pixelColor_ = (uint)((color.B << 16) | (color.G << 8) | (color.R));
        }

#if USE_SHARP_DX
        public void Draw(RenderTarget renderTarget, SolidColorBrush brush, RectangleF real, RectangleF view, float width, float height, bool noScale = false)
#else
        public void Draw(D2D1RenderTarget renderTarget, D2D1SolidColorBrush brush, RectangleF real, RectangleF view, float width, float height)
#endif
        {
            //TODO:KHAN

            Rectangle rbgn = new Rectangle(0, 0, (int)width, (int)height);
            var scaleX = (double)view.Width / real.Width;
            var scaleY = (double)view.Height / real.Height;

            int x1 = (int)((x1_ - real.Left) * scaleX + view.Left);
            int x2 = (int)((x2_ - real.Left) * scaleX + view.Left);
            int y1 = (int)((y1_ - real.Top) * scaleY + view.Top);
            int y2 = (int)((y2_ - real.Top) * scaleY + view.Top);

            //기타 맵 x 축은 스케일 필요없음 @ATW 231222
            if (noScale) 
            {
                x1 = (int)((x1_ - real.Left) + view.Left);
                x2 = (int)((x2_ - real.Left) + view.Left);
            }

            Point[] points = null;
            if (points_ != null)
            {
                points = new Point[points_.Count()];
                for (int i = 0; i < points.Count(); i++)
                {
                    points[i].X = (int)((points_[i].X - real.Left) * scaleX + view.Left);
                    points[i].Y = (int)((points_[i].Y - real.Top) * scaleY + view.Top);
                }
            }
            switch (type_)
            {
                case GraphicType.Text:
                    {
                        //e.DrawString(text_, font_, brush_, x1, y1);
#if USE_SHARP_DX
                        var dWriteFactory = new SharpDX.DirectWrite.Factory(SharpDX.DirectWrite.FactoryType.Isolated);
                        var textFormat = new SharpDX.DirectWrite.TextFormat(dWriteFactory, "굴림", null, SharpDX.DirectWrite.FontWeight.Normal, SharpDX.DirectWrite.FontStyle.Normal, SharpDX.DirectWrite.FontStretch.Normal, 12, "");
                        var rt = new RawRectangleF(x1 - 50, y1, x1+50, y1 + 15);
#else
                        var dWriteFactory = DWriteFactory.Create(DWriteFactoryType.Isolated);
                        var textFormat = dWriteFactory.CreateTextFormat("굴림", null, DWriteFontWeight.Normal, DWriteFontStyle.Normal, DWriteFontStretch.Normal, 12, "");
                        var rt = new D2D1RectF(x1 - 50, y1, x1+50, y1 + 15);
#endif
                        renderTarget.DrawText(text_, textFormat, rt, brush);
                    }
                    break;
                case GraphicType.Cross:
                    //e.DrawLine(pen_, x1 - size_, y1, x1 + size_, y1);
                    //e.DrawLine(pen_, x1, y1 - size_, x1, y1 + size_);
                    break;
                case GraphicType.Rect:
#if USE_SHARP_DX
                    brush.Color = ColorConverter.ColorToRawColor(color_);
                    renderTarget.DrawRectangle(new RawRectangleF(x1, y1, x2, y2), brush);
#else
                    brush.Color = new D2D1ColorF(ColorConverter.ColorToUint(color_));
                    renderTarget.DrawRectangle(new D2D1RectF(x1, y1, x2, y2), brush);
#endif
                    break;
                case GraphicType.Line:
#if USE_SHARP_DX
                    brush.Color = ColorConverter.ColorToRawColor(color_);
                    renderTarget.DrawLine(new RawVector2(x1, y1), new RawVector2(x2, y2), brush);
#else
                    brush.Color = new D2D1ColorF(ColorConverter.ColorToUint(color_));
                    renderTarget.DrawLine(new D2D1Point2F(x1, y1), new D2D1Point2F(x2, y2), brush);
#endif
                    //e.DrawLine(pen_, x1, y1, x2, y2);
                    break;
                case GraphicType.RectAngle:
                    //e.DrawPolygon(pen_, points);
                    break;
                case GraphicType.Circle:
                case GraphicType.Ellipse:
                    //e.DrawEllipse(pen_, x1, y1, x2 - x1, y2 - y1);
                    break;
                case GraphicType.Pixels:
                    //{
                    //    var hdc = e.GetHdc();
                    //    foreach (var p in points)
                    //    {
                    //        GDI.SetPixel(hdc, p.X, p.Y, pixelColor_);
                    //    }
                    //    e.ReleaseHdc(hdc);
                    //}
                    break;
            }
			
        }
    }
    public class GDI
    {
        [System.Runtime.InteropServices.DllImport("gdi32.dll")]
        internal static extern bool SetPixel(IntPtr hdc, int X, int Y, uint crColor);
    }
}
