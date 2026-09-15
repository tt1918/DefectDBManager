using System;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace CustomControls
{
    public enum ShadedButtonIconAlign
    {
        Left,
        Right,
        Top,
        Bottom,
        Center,
    }

    [DefaultEvent("Click")]
    public class ShadedButton : Control
    {
        #region Fields

        private Color _normalColor = Color.FromArgb(18, 27, 38);
        private Color _hoverColor = Color.FromArgb(35, 50, 70);
        private Color _pressedColor = Color.FromArgb(10, 15, 25);
        private Color _disabledColor = Color.FromArgb(50, 50, 50);
        private Color _textColor = Color.White;
        private Color _disabledTextColor = Color.FromArgb(120, 120, 120);
        private Color _accentColor = Color.FromArgb(40, 130, 255);
        private Color _borderColor = Color.FromArgb(120, 40, 130, 255);
        private int _borderSize = 1;
        private int _borderRadius = 8;
        private Image _icon = null;
        private int _iconSize = 20;
        private ShadedButtonIconAlign _iconAlign = ShadedButtonIconAlign.Left;
        private int _iconPadding = 6;
        private int _textOffsetX = 0;
        private int _textOffsetY = 0;
        private bool _isHovered = false;
        private bool _isPressed = false;
        private bool _showAccentLine = true;

        #endregion

        #region Properties

        [Category("ShadedButton"), Description("일반 상태 배경색")]
        public Color NormalColor
        {
            get => _normalColor;
            set { _normalColor = value; Invalidate(); }
        }

        [Category("ShadedButton"), Description("마우스 오버 배경색")]
        public Color HoverColor
        {
            get => _hoverColor;
            set { _hoverColor = value; Invalidate(); }
        }

        [Category("ShadedButton"), Description("클릭(눌림) 배경색")]
        public Color PressedColor
        {
            get => _pressedColor;
            set { _pressedColor = value; Invalidate(); }
        }

        [Category("ShadedButton"), Description("비활성화 배경색")]
        public Color DisabledColor
        {
            get => _disabledColor;
            set { _disabledColor = value; Invalidate(); }
        }

        [Category("ShadedButton"), Description("텍스트 색상")]
        public Color TextColor
        {
            get => _textColor;
            set { _textColor = value; Invalidate(); }
        }

        [Category("ShadedButton"), Description("비활성화 텍스트 색상")]
        public Color DisabledTextColor
        {
            get => _disabledTextColor;
            set { _disabledTextColor = value; Invalidate(); }
        }

        [Category("ShadedButton"), Description("강조 색상 (상단 라인, 테두리)")]
        public Color AccentColor
        {
            get => _accentColor;
            set { _accentColor = value; Invalidate(); }
        }

        [Category("ShadedButton"), Description("테두리 색상")]
        public Color BorderColor
        {
            get => _borderColor;
            set { _borderColor = value; Invalidate(); }
        }

        [Category("ShadedButton"), Description("테두리 두께")]
        public int BorderSize
        {
            get => _borderSize;
            set { _borderSize = Math.Max(0, value); Invalidate(); }
        }

        [Category("ShadedButton"), Description("모서리 반경 (0 = 직각)")]
        public int BorderRadius
        {
            get => _borderRadius;
            set { _borderRadius = Math.Max(0, value); Invalidate(); }
        }

        [Category("ShadedButton"), Description("버튼에 표시할 이미지")]
        public Image ButtonImage
        {
            get => _icon;
            set { _icon = value; Invalidate(); }
        }

        [Category("ShadedButton"), Description("아이콘 크기 (px)")]
        public int IconSize
        {
            get => _iconSize;
            set { _iconSize = Math.Max(8, value); Invalidate(); }
        }

        [Category("ShadedButton"), Description("아이콘 위치 (Left/Right/Top/Bottom/Center)")]
        public ShadedButtonIconAlign IconAlign
        {
            get => _iconAlign;
            set { _iconAlign = value; Invalidate(); }
        }

        [Category("ShadedButton"), Description("아이콘과 텍스트 사이 간격 (px)")]
        public int IconPadding
        {
            get => _iconPadding;
            set { _iconPadding = Math.Max(0, value); Invalidate(); }
        }

        [Category("ShadedButton"), Description("텍스트 수평 오프셋 (px, 양수=오른쪽, 음수=왼쪽)")]
        public int TextOffsetX
        {
            get => _textOffsetX;
            set { _textOffsetX = value; Invalidate(); }
        }

        [Category("ShadedButton"), Description("텍스트 수직 오프셋 (px, 양수=아래, 음수=위)")]
        public int TextOffsetY
        {
            get => _textOffsetY;
            set { _textOffsetY = value; Invalidate(); }
        }

        [Category("ShadedButton"), Description("상단 강조 라인 표시 여부")]
        public bool ShowAccentLine
        {
            get => _showAccentLine;
            set { _showAccentLine = value; Invalidate(); }
        }

        #endregion

        #region Constructor

        public ShadedButton()
        {
            SetStyle(
                ControlStyles.UserPaint |
                ControlStyles.AllPaintingInWmPaint |
                ControlStyles.OptimizedDoubleBuffer |
                ControlStyles.ResizeRedraw |
                ControlStyles.SupportsTransparentBackColor,
                true);

            Size = new Size(120, 40);
            Font = new Font("Segoe UI", 10f, FontStyle.Regular);
            BackColor = Color.Transparent;
            Cursor = Cursors.Hand;
        }

        #endregion

        #region Helper Methods

        /// <summary>
        /// 색상을 밝게 만듭니다.
        /// </summary>
        private Color LightenColor(Color color, float factor = 1.3f)
        {
            int r = Math.Min(255, (int)(color.R * factor));
            int g = Math.Min(255, (int)(color.G * factor));
            int b = Math.Min(255, (int)(color.B * factor));
            return Color.FromArgb(color.A, r, g, b);
        }

        /// <summary>
        /// 색상을 어둡게 만듭니다.
        /// </summary>
        private Color DarkenColor(Color color, float factor = 0.7f)
        {
            int r = (int)(color.R * factor);
            int g = (int)(color.G * factor);
            int b = (int)(color.B * factor);
            return Color.FromArgb(color.A, r, g, b);
        }

        #endregion

        #region Mouse Events

        protected override void OnMouseEnter(EventArgs e)
        {
            base.OnMouseEnter(e);
            _isHovered = true;
            Invalidate();
        }

        protected override void OnMouseLeave(EventArgs e)
        {
            base.OnMouseLeave(e);
            _isHovered = false;
            _isPressed = false;
            Invalidate();
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(e);
            if (e.Button == MouseButtons.Left)
            {
                _isPressed = true;
                Invalidate();
            }
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);
            _isPressed = false;
            Invalidate();
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

            int radius = Math.Min(_borderRadius, Math.Min(rect.Width, rect.Height) / 2);

            // 부모 상태에 따른 배경색 결정
            Color bgColor;
            if (!Enabled)
                bgColor = _disabledColor;
            else if (_isPressed)
                bgColor = _pressedColor;
            else if (_isHovered)
                bgColor = _hoverColor;
            else
                bgColor = _normalColor;

            // 배경 그리기 (단색)
            using (GraphicsPath path = GetRoundedPath(rect, radius))
            using (SolidBrush bgBrush = new SolidBrush(bgColor))
            {
                g.FillPath(bgBrush, path);
            }

            // 섬세한 내부 그라데이션 (사용자가 선택한 색상 기반으로 동적 생성)
            Rectangle gradientRect = new Rectangle(
                rect.X,
                rect.Y,
                rect.Width,
                rect.Height);

            // 사용자가 선택한 색상을 기반으로 그라데이션 생성
            Color gradientStart = LightenColor(bgColor, 1.15f);
            Color gradientEnd = DarkenColor(bgColor, 0.8f);

            using (GraphicsPath path = GetRoundedPath(gradientRect, radius))
            using (LinearGradientBrush brush =
                new LinearGradientBrush(
                    gradientRect,
                    gradientStart,
                    gradientEnd,
                    90f))
            {
                g.FillPath(brush, path);
            }

            // 테두리 그리기
            using (GraphicsPath path = GetRoundedPath(rect, radius))
            using (Pen pen = new Pen(_borderColor, _borderSize))
            {
                g.DrawPath(pen, path);
            }

            // 상단 강조 라인 (Accent line)
            if (_showAccentLine && Enabled)
            {
                float lineWidth = _isPressed ? 1.5f : (_isHovered ? 2f : 1.5f);
                using (Pen pen = new Pen(_accentColor, lineWidth))
                {
                    g.DrawLine(
                        pen,
                        rect.X + radius + 5,
                        rect.Y + 1,
                        rect.Right - radius - 5,
                        rect.Y + 1);
                }
            }

            // 텍스트 및 아이콘 그리기
            Color fgColor = Enabled ? _textColor : _disabledTextColor;
            DrawIconAndText(g, fgColor);
        }

        private void DrawIconAndText(Graphics g, Color fgColor)
        {
            bool hasIcon = _icon != null;
            bool hasText = !string.IsNullOrEmpty(Text);
            int sz = _iconSize;
            int pad = _iconPadding;

            if (!hasIcon)
            {
                // 텍스트만 중앙
                if (hasText)
                    DrawText(g, fgColor, new RectangleF(0, 0, Width, Height), StringAlignment.Center, StringAlignment.Center);
                return;
            }

            switch (_iconAlign)
            {
                case ShadedButtonIconAlign.Left:
                    {
                        int iconX = pad;
                        int iconY = (Height - sz) / 2;
                        g.DrawImage(_icon, new Rectangle(iconX, iconY, sz, sz));
                        if (hasText)
                        {
                            int textAreaX = iconX + sz + pad;
                            var textRect = new RectangleF(textAreaX + _textOffsetX, _textOffsetY, Width - textAreaX - pad, Height);
                            DrawText(g, fgColor, textRect, StringAlignment.Center, StringAlignment.Center);
                        }
                        break;
                    }
                case ShadedButtonIconAlign.Right:
                    {
                        int iconX = Width - sz - pad;
                        int iconY = (Height - sz) / 2;
                        if (hasText)
                        {
                            var textRect = new RectangleF(pad + _textOffsetX, _textOffsetY, iconX - pad, Height);
                            DrawText(g, fgColor, textRect, StringAlignment.Center, StringAlignment.Center);
                        }
                        g.DrawImage(_icon, new Rectangle(iconX, iconY, sz, sz));
                        break;
                    }
                case ShadedButtonIconAlign.Top:
                    {
                        int textH = hasText ? (int)Math.Ceiling(g.MeasureString(Text, Font).Height) : 0;
                        int totalH = sz + (hasText ? pad + textH : 0);
                        int startY = (Height - totalH) / 2;
                        int iconX = (Width - sz) / 2;
                        g.DrawImage(_icon, new Rectangle(iconX, startY, sz, sz));
                        if (hasText)
                        {
                            var textRect = new RectangleF(2 + _textOffsetX, startY + sz + pad + _textOffsetY, Width - 4, textH + 2);
                            DrawText(g, fgColor, textRect, StringAlignment.Center, StringAlignment.Near);
                        }
                        break;
                    }
                case ShadedButtonIconAlign.Bottom:
                    {
                        int textH = hasText ? (int)Math.Ceiling(g.MeasureString(Text, Font).Height) : 0;
                        int totalH = sz + (hasText ? pad + textH : 0);
                        int startY = (Height - totalH) / 2;
                        int iconX = (Width - sz) / 2;
                        if (hasText)
                        {
                            var textRect = new RectangleF(2 + _textOffsetX, startY + _textOffsetY, Width - 4, textH + 2);
                            DrawText(g, fgColor, textRect, StringAlignment.Center, StringAlignment.Far);
                        }
                        g.DrawImage(_icon, new Rectangle(iconX, startY + textH + pad, sz, sz));
                        break;
                    }
                case ShadedButtonIconAlign.Center:
                    {
                        int iconX = (Width - sz) / 2;
                        int iconY = (Height - sz) / 2;
                        g.DrawImage(_icon, new Rectangle(iconX, iconY, sz, sz));
                        break;
                    }
            }
        }

        private void DrawText(Graphics g, Color color, RectangleF rect, StringAlignment hAlign, StringAlignment vAlign)
        {
            using (Brush textBrush = new SolidBrush(color))
            {
                StringFormat sf = new StringFormat
                {
                    Alignment = hAlign,
                    LineAlignment = vAlign
                };

                g.DrawString(Text, Font, textBrush, rect, sf);
            }
        }

        private GraphicsPath GetRoundedPath(Rectangle rect, int radius)
        {
            GraphicsPath path = new GraphicsPath();

            int d = radius * 2;

            if (d > rect.Width)
                d = rect.Width;

            if (d > rect.Height)
                d = rect.Height;

            if (d > 0)
            {
                path.AddArc(rect.X, rect.Y, d, d, 180, 90);
                path.AddArc(rect.Right - d, rect.Y, d, d, 270, 90);
                path.AddArc(rect.Right - d, rect.Bottom - d, d, d, 0, 90);
                path.AddArc(rect.X, rect.Bottom - d, d, d, 90, 90);
            }
            else
            {
                path.AddRectangle(rect);
            }

            path.CloseFigure();

            return path;
        }

        #endregion
    }
}
