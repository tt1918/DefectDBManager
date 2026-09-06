using System;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace CustomControls
{
    [DefaultEvent("Click")]
    public class ShadedLabel : Control
    {
        #region Fields

        private Color _fillColor = Color.FromArgb(18, 27, 38);
        private Color _textColor = Color.White;
        private Color _borderColor = Color.FromArgb(120, 40, 130, 255);
        private Color _accentColor = Color.FromArgb(40, 130, 255);
        private int _borderSize = 1;
        private int _borderRadius = 8;
        private bool _showAccentLine = true;

        private ContentAlignment _textAlign = ContentAlignment.MiddleCenter;
        private Image _labelImage = null;
        private int _imageSize = 18;
        private int _imagePadding = 6;

        #endregion

        #region Properties

        [Category("ShadedLabel"), Description("배경 채우기 색상")]
        public Color FillColor
        {
            get { return _fillColor; }
            set { _fillColor = value; Invalidate(); }
        }

        [Category("ShadedLabel"), Description("텍스트 색상")]
        public Color TextColor
        {
            get { return _textColor; }
            set { _textColor = value; Invalidate(); }
        }

        [Category("ShadedLabel"), Description("테두리 색상")]
        public Color BorderColor
        {
            get { return _borderColor; }
            set { _borderColor = value; Invalidate(); }
        }

        [Category("ShadedLabel"), Description("강조 색상 (상단 라인)")]
        public Color AccentColor
        {
            get { return _accentColor; }
            set { _accentColor = value; Invalidate(); }
        }

        [Category("ShadedLabel"), Description("테두리 두께")]
        public int BorderSize
        {
            get { return _borderSize; }
            set { _borderSize = Math.Max(0, value); Invalidate(); }
        }

        [Category("ShadedLabel"), Description("모서리 반경 (0 = 직각)")]
        public int BorderRadius
        {
            get { return _borderRadius; }
            set { _borderRadius = Math.Max(0, value); Invalidate(); }
        }

        [Category("ShadedLabel"), Description("상단 강조 라인 표시 여부")]
        public bool ShowAccentLine
        {
            get { return _showAccentLine; }
            set { _showAccentLine = value; Invalidate(); }
        }

        [Category("ShadedLabel"), Description("텍스트 정렬")]
        public ContentAlignment TextAlign
        {
            get { return _textAlign; }
            set { _textAlign = value; Invalidate(); }
        }

        [Category("ShadedLabel"), Description("레이블에 표시할 이미지")]
        public Image LabelImage
        {
            get { return _labelImage; }
            set { _labelImage = value; Invalidate(); }
        }

        [Category("ShadedLabel"), Description("이미지 크기 (px)")]
        public int ImageSize
        {
            get { return _imageSize; }
            set { _imageSize = Math.Max(8, value); Invalidate(); }
        }

        [Category("ShadedLabel"), Description("이미지 간격 (px)")]
        public int ImagePadding
        {
            get { return _imagePadding; }
            set { _imagePadding = Math.Max(0, value); Invalidate(); }
        }

        public override string Text
        {
            get { return base.Text; }
            set { base.Text = value; Invalidate(); }
        }

        #endregion

        #region Constructor

        public ShadedLabel()
        {
            SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer |
                ControlStyles.ResizeRedraw |
                ControlStyles.SupportsTransparentBackColor,
                true);

            Size = new Size(120, 36);
            Font = new Font("Segoe UI", 10f, FontStyle.Regular);
            BackColor = Color.Transparent;
        }

        #endregion

        #region Paint

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);

            Graphics g = e.Graphics;
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.InterpolationMode = InterpolationMode.HighQualityBicubic;
            g.PixelOffsetMode = PixelOffsetMode.HighQuality;
            g.TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;

            Rectangle rect = new Rectangle(
                _borderSize,
                _borderSize,
                Width - (_borderSize * 2) - 1,
                Height - (_borderSize * 2) - 1);

            if (rect.Width <= 0 || rect.Height <= 0)
                return;

            int radius = Math.Min(_borderRadius, Math.Min(rect.Width, rect.Height) / 2);

            using (GraphicsPath path = GetRoundedPath(rect, radius))
            {
                using (SolidBrush bgBrush = new SolidBrush(_fillColor))
                    g.FillPath(bgBrush, path);

                Rectangle gradientRect = rect;
                Color gradientStart = LightenColor(_fillColor, 1.15f);
                Color gradientEnd = DarkenColor(_fillColor, 0.8f);

                using (LinearGradientBrush gradientBrush = new LinearGradientBrush(gradientRect, gradientStart, gradientEnd, 90f))
                    g.FillPath(gradientBrush, path);

                if (_borderSize > 0 && _borderColor != Color.Transparent)
                {
                    using (Pen borderPen = new Pen(_borderColor, _borderSize))
                    {
                        borderPen.Alignment = PenAlignment.Inset;
                        g.DrawPath(borderPen, path);
                    }
                }

                if (_showAccentLine)
                {
                    using (Pen accentPen = new Pen(_accentColor, 1.5f))
                    {
                        int lineStartX = rect.X + radius + 5;
                        int lineEndX = rect.Right - radius - 5;

                        if (lineEndX > lineStartX)
                        {
                            g.DrawLine(
                                accentPen,
                                lineStartX,
                                rect.Y + 1,
                                lineEndX,
                                rect.Y + 1);
                        }
                    }
                }

                DrawContent(g);
            }
        }

        private void DrawContent(Graphics g)
        {
            using (SolidBrush textBrush = new SolidBrush(_textColor))
            {
                if (_labelImage != null)
                {
                    int imgSize = Math.Min(_imageSize, Math.Max(8, Height - 10));
                    int imgX = _imagePadding;
                    int imgY = (Height - imgSize) / 2;
                    g.DrawImage(_labelImage, new Rectangle(imgX, imgY, imgSize, imgSize));

                    Rectangle textRect = new Rectangle(
                        imgX + imgSize + _imagePadding,
                        0,
                        Width - (imgX + imgSize + (_imagePadding * 2)),
                        Height);

                    StringFormat sf = GetStringFormat(_textAlign);
                    sf.Alignment = StringAlignment.Near;
                    g.DrawString(Text, Font, textBrush, textRect, sf);
                    sf.Dispose();
                }
                else
                {
                    StringFormat sf = GetStringFormat(_textAlign);
                    g.DrawString(Text, Font, textBrush, new RectangleF(0, 0, Width, Height), sf);
                    sf.Dispose();
                }
            }
        }

        private Color LightenColor(Color color, float factor)
        {
            int r = Math.Min(255, (int)(color.R * factor));
            int g = Math.Min(255, (int)(color.G * factor));
            int b = Math.Min(255, (int)(color.B * factor));
            return Color.FromArgb(color.A, r, g, b);
        }

        private Color DarkenColor(Color color, float factor)
        {
            int r = (int)(color.R * factor);
            int g = (int)(color.G * factor);
            int b = (int)(color.B * factor);
            return Color.FromArgb(color.A, r, g, b);
        }

        private GraphicsPath GetRoundedPath(Rectangle rect, int radius)
        {
            GraphicsPath path = new GraphicsPath();

            if (radius <= 0)
            {
                path.AddRectangle(rect);
                return path;
            }

            int d = radius * 2;
            path.AddArc(rect.X, rect.Y, d, d, 180, 90);
            path.AddArc(rect.Right - d, rect.Y, d, d, 270, 90);
            path.AddArc(rect.Right - d, rect.Bottom - d, d, d, 0, 90);
            path.AddArc(rect.X, rect.Bottom - d, d, d, 90, 90);
            path.CloseFigure();

            return path;
        }

        private StringFormat GetStringFormat(ContentAlignment align)
        {
            StringFormat sf = new StringFormat();
            sf.Trimming = StringTrimming.EllipsisCharacter;

            switch (align)
            {
                case ContentAlignment.TopLeft:
                    sf.Alignment = StringAlignment.Near;
                    sf.LineAlignment = StringAlignment.Near;
                    break;
                case ContentAlignment.TopCenter:
                    sf.Alignment = StringAlignment.Center;
                    sf.LineAlignment = StringAlignment.Near;
                    break;
                case ContentAlignment.TopRight:
                    sf.Alignment = StringAlignment.Far;
                    sf.LineAlignment = StringAlignment.Near;
                    break;
                case ContentAlignment.MiddleLeft:
                    sf.Alignment = StringAlignment.Near;
                    sf.LineAlignment = StringAlignment.Center;
                    break;
                case ContentAlignment.MiddleCenter:
                    sf.Alignment = StringAlignment.Center;
                    sf.LineAlignment = StringAlignment.Center;
                    break;
                case ContentAlignment.MiddleRight:
                    sf.Alignment = StringAlignment.Far;
                    sf.LineAlignment = StringAlignment.Center;
                    break;
                case ContentAlignment.BottomLeft:
                    sf.Alignment = StringAlignment.Near;
                    sf.LineAlignment = StringAlignment.Far;
                    break;
                case ContentAlignment.BottomCenter:
                    sf.Alignment = StringAlignment.Center;
                    sf.LineAlignment = StringAlignment.Far;
                    break;
                case ContentAlignment.BottomRight:
                    sf.Alignment = StringAlignment.Far;
                    sf.LineAlignment = StringAlignment.Far;
                    break;
                default:
                    sf.Alignment = StringAlignment.Center;
                    sf.LineAlignment = StringAlignment.Center;
                    break;
            }

            return sf;
        }

        #endregion
    }
}
