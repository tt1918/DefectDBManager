using System;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace CustomControls
{
    public class GradiantLabel : Control
    {
        private Color _fillColor = Color.FromArgb(0, 120, 215);
        private Color _fillColor2 = Color.FromArgb(0, 84, 153);
        private bool _useGradient = true;
        private bool _useBestFitFont = true;
        private Color _textColor = Color.White;

        [Category("GradiantLabel"), Description("모서리 반경")]
        public int CornerRadius { get; set; } = 5;

        [Category("GradiantLabel"), Description("배경 시작 색상")]
        public Color FillColor
        {
            get => _fillColor;
            set { _fillColor = value; Invalidate(); }
        }

        [Category("GradiantLabel"), Description("배경 끝 색상 (그라데이션 사용 시)")]
        public Color FillColor2
        {
            get => _fillColor2;
            set { _fillColor2 = value; Invalidate(); }
        }

        [Category("GradiantLabel"), Description("그라데이션 사용 여부")]
        public bool UseGradient
        {
            get => _useGradient;
            set { _useGradient = value; Invalidate(); }
        }

        [Category("GradiantLabel"), Description("텍스트 색상")]
        [DefaultValue(typeof(Color), "White")]
        public Color TextColor
        {
            get => _textColor;
            set
            {
                if (_textColor == value)
                    return;

                _textColor = value;
                base.ForeColor = value;
                Invalidate();
            }
        }

        [Category("GradiantLabel"), Description("최적 글꼴 크기 사용 여부")]
        public bool UseBestFitFont
        {
            get => _useBestFitFont;
            set { _useBestFitFont = value; Invalidate(); }
        }

        public override string Text
        {
            get { return base.Text; }
            set
            {
                if (base.Text != value)
                {
                    base.Text = value;
                    Invalidate();
                }
            }
        }
        public GradiantLabel()
        {
            SetStyle(ControlStyles.AllPaintingInWmPaint |
                     ControlStyles.UserPaint |
                     ControlStyles.OptimizedDoubleBuffer |
                     ControlStyles.ResizeRedraw |
                     ControlStyles.SupportsTransparentBackColor, true);

            DoubleBuffered = true;
            TextColor = Color.White;
            Font = new Font("Segoe UI", 30f, FontStyle.Bold, GraphicsUnit.Point);
            Text = "STATUS";
            Padding = new Padding(0, 4, 0, 4);
            Size = new Size(260, 120);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;

            Rectangle rect = ClientRectangle;
            if (rect.Width <= 0 || rect.Height <= 0)
                return;

            rect.Width -= 1;
            rect.Height -= 1;

            using (GraphicsPath path = CreateRoundRectPath(rect, CornerRadius))
            {
                DrawBackground(e.Graphics, path, rect);
                DrawCenterText(e.Graphics);
            }
        }

        protected override void OnForeColorChanged(EventArgs e)
        {
            base.OnForeColorChanged(e);

            if (_textColor != ForeColor)
            {
                _textColor = ForeColor;
                Invalidate();
            }
        }

        private void DrawBackground(Graphics graphics, GraphicsPath path, Rectangle rect)
        {
            if (UseGradient)
            {
                using (LinearGradientBrush brush = new LinearGradientBrush(rect, FillColor, FillColor2, LinearGradientMode.Vertical))
                {
                    graphics.FillPath(brush, path);
                }
            }
            else
            {
                using (SolidBrush brush = new SolidBrush(FillColor))
                {
                    graphics.FillPath(brush, path);
                }
            }
        }

        private void DrawCenterText(Graphics graphics)
        {
            Rectangle textRect = new Rectangle(
                Padding.Left,
                Padding.Top,
                Math.Max(1, Width - Padding.Left - Padding.Right),
                Math.Max(1, Height - Padding.Top - Padding.Bottom));

            using (Font fitFont = GetBestFitFont(graphics, Text ?? string.Empty, textRect))
            {
                TextRenderer.DrawText(
                    graphics,
                    Text ?? string.Empty,
                    fitFont,
                    textRect,
                    TextColor,
                    TextFormatFlags.HorizontalCenter |
                    TextFormatFlags.VerticalCenter |
                    TextFormatFlags.SingleLine |
                    TextFormatFlags.NoPadding |
                    TextFormatFlags.EndEllipsis);
            }
        }

        private Font GetBestFitFont(Graphics graphics, string text, Rectangle bounds)
        {
            if (_useBestFitFont == false)
                return new Font(this.Font, this.Font.Style);

            float maxFontSize  = this.Font.Size;

            float maxSize = Math.Min(maxFontSize, Math.Max(6f, bounds.Height * 0.9f));
            FontFamily family = Font?.FontFamily ?? SystemFonts.DefaultFont.FontFamily;
            FontStyle style = Font?.Style ?? FontStyle.Bold;

            for (float size = maxSize; size >= 8f; size -= 1f)
            {
                Font testFont = new Font(family, size, style, GraphicsUnit.Point);
                Size measured = TextRenderer.MeasureText(
                    graphics,
                    string.IsNullOrEmpty(text) ? " " : text,
                    testFont,
                    new Size(int.MaxValue, int.MaxValue),
                    TextFormatFlags.SingleLine | TextFormatFlags.NoPadding);

                if (measured.Width <= bounds.Width && measured.Height <= bounds.Height)
                    return testFont;

                testFont.Dispose();
            }

            return new Font(family, 8f, style, GraphicsUnit.Point);
        }

        private GraphicsPath CreateRoundRectPath(Rectangle bounds, int radius)
        {
            GraphicsPath path = new GraphicsPath();
            int r = Math.Max(0, radius * 2);

            if (r <= 0)
            {
                path.AddRectangle(bounds);
                path.CloseFigure();
                return path;
            }

            Rectangle arc = new Rectangle(bounds.Location, new Size(r, r));
            path.AddArc(arc, 180, 90);
            arc.X = bounds.Right - r;
            path.AddArc(arc, 270, 90);
            arc.Y = bounds.Bottom - r;
            path.AddArc(arc, 0, 90);
            arc.X = bounds.Left;
            path.AddArc(arc, 90, 90);
            path.CloseFigure();

            return path;
        }
    }
}
