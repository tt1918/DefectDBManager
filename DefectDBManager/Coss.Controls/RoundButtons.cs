using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace Coss.Controls
{
    public class RoundButton : Control
    {

        #region Variables

        private int MouseState;
        private GraphicsPath Shape;
        private LinearGradientBrush InactiveGB;
        private LinearGradientBrush PressedGB;
        private LinearGradientBrush PressedContourGB;
        private LinearGradientBrush InactiveGB2;
        private LinearGradientBrush PressedGB2;

        private Rectangle R1;
        private Pen P1;
        private Pen P2;
        private Pen P3;
        private Image _Image;
        private Size _ImageSize;
        private StringAlignment _TextAlignment = StringAlignment.Center;
        private ContentAlignment _ImageAlign = ContentAlignment.MiddleLeft;
        private bool _ButtonPressed;

        private Color _InactiveGBColor1 = Color.FromArgb(0, 176, 231);
        private Color _InactiveGBColor2 = Color.FromArgb(0, 152, 224);
        private Color _PressedGBColor1 = Color.FromArgb(0, 118, 176);
        private Color _PressedGBColor2 = Color.FromArgb(0, 149, 222);
        private Color _InactiveGB2Color1 = Color.FromArgb(246, 168, 0);
        private Color _InactiveGB2Color2 = Color.FromArgb(246, 197, 0);
        private Color _PressedGB2Color1 = Color.FromArgb(200, 120, 0);
        private Color _PressedGB2Color2 = Color.FromArgb(246, 197, 0);
        private Color _ForeColor2 = Color.FromArgb(0, 0, 0);
        private Color _OutlineColor = Color.Transparent;
        private Color _OutlineColor2 = Color.Transparent;
        private bool _UserText = false;

        #endregion
        #region Image Designer

        private static PointF ImageLocation(StringFormat SF, SizeF Area, SizeF ImageArea)
        {
            PointF MyPoint = default(PointF);
            switch (SF.Alignment)
            {
                case StringAlignment.Center:
                    MyPoint.X = Convert.ToSingle((Area.Width - ImageArea.Width) / 2);
                    break;
                case StringAlignment.Near:
                    MyPoint.X = 2;
                    break;
                case StringAlignment.Far:
                    MyPoint.X = Area.Width - ImageArea.Width - 2;
                    break;
            }

            switch (SF.LineAlignment)
            {
                case StringAlignment.Center:
                    MyPoint.Y = Convert.ToSingle((Area.Height - ImageArea.Height) / 2);
                    break;
                case StringAlignment.Near:
                    MyPoint.Y = 2;
                    break;
                case StringAlignment.Far:
                    MyPoint.Y = Area.Height - ImageArea.Height - 2;
                    break;
            }
            return MyPoint;
        }

        private StringFormat GetStringFormat(ContentAlignment _ContentAlignment)
        {
            StringFormat SF = new StringFormat();
            switch (_ContentAlignment)
            {
                case ContentAlignment.MiddleCenter:
                    SF.LineAlignment = StringAlignment.Center;
                    SF.Alignment = StringAlignment.Center;
                    break;
                case ContentAlignment.MiddleLeft:
                    SF.LineAlignment = StringAlignment.Center;
                    SF.Alignment = StringAlignment.Near;
                    break;
                case ContentAlignment.MiddleRight:
                    SF.LineAlignment = StringAlignment.Center;
                    SF.Alignment = StringAlignment.Far;
                    break;
                case ContentAlignment.TopCenter:
                    SF.LineAlignment = StringAlignment.Near;
                    SF.Alignment = StringAlignment.Center;
                    break;
                case ContentAlignment.TopLeft:
                    SF.LineAlignment = StringAlignment.Near;
                    SF.Alignment = StringAlignment.Near;
                    break;
                case ContentAlignment.TopRight:
                    SF.LineAlignment = StringAlignment.Near;
                    SF.Alignment = StringAlignment.Far;
                    break;
                case ContentAlignment.BottomCenter:
                    SF.LineAlignment = StringAlignment.Far;
                    SF.Alignment = StringAlignment.Center;
                    break;
                case ContentAlignment.BottomLeft:
                    SF.LineAlignment = StringAlignment.Far;
                    SF.Alignment = StringAlignment.Near;
                    break;
                case ContentAlignment.BottomRight:
                    SF.LineAlignment = StringAlignment.Far;
                    SF.Alignment = StringAlignment.Far;
                    break;
            }
            return SF;
        }

        #endregion
        #region Properties

        public Image Image
        {
            get { return _Image; }
            set
            {
                if (value == null)
                {
                    _ImageSize = Size.Empty;
                }
                else
                {
                    _ImageSize = value.Size;
                }

                _Image = value;
                Invalidate();
            }
        }

        public StringAlignment TextAlignment
        {
            get { return this._TextAlignment; }
            set
            {
                this._TextAlignment = value;
                this.Invalidate();
            }
        }

        protected Size ImageSize
        {
            get { return _ImageSize; }
        }

        public ContentAlignment ImageAlign
        {
            get { return _ImageAlign; }
            set
            {
                _ImageAlign = value;
                Invalidate();
            }
        }

        public bool ButtonPressed
        {
            get { return _ButtonPressed; }
            set
            {
                _ButtonPressed = value;
                Invalidate();
            }
        }

        public Color InactiveGBColor1
        {
            get { return _InactiveGBColor1; }
            set
            {
                _InactiveGBColor1 = value;
                UpdateColor();
                Invalidate();
            }
        }
        public Color InactiveGBColor2
        {
            get { return _InactiveGBColor2; }
            set
            {
                _InactiveGBColor2 = value;
                UpdateColor();
                Invalidate();
            }
        }

        public Color PressedGBColor1
        {
            get { return _PressedGBColor1; }
            set
            {
                _PressedGBColor1 = value;
                UpdateColor();
                Invalidate();
            }
        }
        public Color PressedGBColor2
        {
            get { return _PressedGBColor2; }
            set
            {
                _PressedGBColor2 = value;
                UpdateColor();
                Invalidate();
            }
        }

        public Color InactiveGB2Color1
        {
            get { return _InactiveGB2Color1; }
            set
            {
                _InactiveGB2Color1 = value;
                UpdateColor();
                Invalidate();
            }
        }
        public Color InactiveGB2Color2
        {
            get { return _InactiveGB2Color2; }
            set
            {
                _InactiveGB2Color2 = value;
                UpdateColor();
                Invalidate();
            }
        }

        public Color PressedGB2Color1
        {
            get { return _PressedGB2Color1; }
            set
            {
                _PressedGB2Color1 = value;
                UpdateColor();
                Invalidate();
            }
        }
        public Color PressedGB2Color2
        {
            get { return _PressedGB2Color2; }
            set
            {
                _PressedGB2Color2 = value;
                UpdateColor();
                Invalidate();
            }
        }

        public Color ForeColor2
        {
            get { return _ForeColor2; }
            set
            {
                _ForeColor2 = value;
                Invalidate();
            }
        }

        public Color OutlineColor
        {
            get { return _OutlineColor; }
            set
            {
                _OutlineColor = value;
                P1.Dispose();
                P1 = new Pen(_OutlineColor);
                Invalidate();
            }
        }

        public Color OutlineColor2
        {
            get { return _OutlineColor2; }
            set
            {
                _OutlineColor2 = value;
                P2.Dispose();
                P2 = new Pen(_OutlineColor);
                Invalidate();
            }
        }

        public bool UserText
        {
            get { return _UserText; }
            set { _UserText = value;  }
        }


        #endregion
        #region EventArgs

        protected override void OnMouseUp(MouseEventArgs e)
        {
            MouseState = 0;
            Invalidate();
            base.OnMouseUp(e);
        }
        protected override void OnMouseDown(MouseEventArgs e)
        {
            MouseState = 1;
            Invalidate();
            base.OnMouseDown(e);
        }

        protected override void OnMouseLeave(EventArgs e)
        {
            MouseState = 0;
            // [Inactive]
            Invalidate();
            // Update control
            base.OnMouseLeave(e);
        }

        protected override void OnTextChanged(System.EventArgs e)
        {
            Invalidate();
            base.OnTextChanged(e);
        }

        #endregion

        public RoundButton()
        {
            SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.OptimizedDoubleBuffer | ControlStyles.ResizeRedraw | ControlStyles.SupportsTransparentBackColor | ControlStyles.UserPaint, true);

            BackColor = Color.Transparent;
            DoubleBuffered = true;
            Font = new Font("Segoe UI", 14);
            ForeColor = Color.White;
            ForeColor2 = Color.Black;
            Size = new Size(166, 40);
            _TextAlignment = StringAlignment.Center;
            P1 = new Pen(Color.FromArgb(0, 118, 176));
            P2 = new Pen(Color.FromArgb(0, 118, 176));
        }

        private void UpdateColor()
        {
            InactiveGB?.Dispose();
            InactiveGB = new LinearGradientBrush(new Rectangle(0, 0, Width, Height), InactiveGBColor1, InactiveGBColor2, 90f);
            PressedGB?.Dispose();
            PressedGB = new LinearGradientBrush(new Rectangle(0, 0, Width, Height), PressedGBColor1, PressedGBColor2, 90f);
            InactiveGB2?.Dispose();
            InactiveGB2 = new LinearGradientBrush(new Rectangle(0, 0, Width, Height), InactiveGB2Color1, InactiveGB2Color2, 90f);
            PressedGB2?.Dispose();
            PressedGB2 = new LinearGradientBrush(new Rectangle(0, 0, Width, Height), PressedGB2Color1, PressedGB2Color2, 90f);
        }

        protected override void OnResize(System.EventArgs e)
        {
            base.OnResize(e);
            if (Width > 0 && Height > 0)
            {
                Shape = new GraphicsPath();
                R1 = new Rectangle(0, 0, Width, Height);

                UpdateColor();
                PressedContourGB = new LinearGradientBrush(new Rectangle(0, 0, Width, Height), Color.FromArgb(0, 118, 176), Color.FromArgb(0, 118, 176), 90f);
                P3 = new Pen(PressedContourGB);
            }

            var _Shape = Shape;
            _Shape.AddArc(0, 0, 10, 10, 180, 90);
            _Shape.AddArc(Width - 11, 0, 10, 10, -90, 90);
            _Shape.AddArc(Width - 11, Height - 11, 10, 10, 0, 90);
            _Shape.AddArc(0, Height - 11, 10, 10, 90, 90);
            _Shape.CloseAllFigures();

            Invalidate();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            var _G = e.Graphics;
            _G.SmoothingMode = SmoothingMode.HighQuality;

            PointF ipt = ImageLocation(GetStringFormat(ImageAlign), Size, ImageSize);

            switch (MouseState)
            {
                case 0:
                    _G.FillPath(_ButtonPressed? InactiveGB2 : InactiveGB, Shape);
                    _G.DrawPath(_ButtonPressed ? P2:P1, Shape);
                    if ((Image == null))
                    {
                        _G.DrawString(Text, Font, new SolidBrush(_ButtonPressed ? ForeColor2 : ForeColor), R1, new StringFormat
                        {
                            Alignment = _TextAlignment,
                            LineAlignment = StringAlignment.Center
                        });
                    }
                    else
                    {
                        _G.DrawImage(_Image, ipt.X, ipt.Y, ImageSize.Width, ImageSize.Height);
                        _G.DrawString(Text, Font, new SolidBrush(_ButtonPressed ? ForeColor2 : ForeColor), R1, new StringFormat
                        {
                            Alignment = _TextAlignment,
                            LineAlignment = StringAlignment.Center
                        });
                    }
                    break;
                case 1:
                    _G.FillPath(_ButtonPressed ? PressedGB2 : PressedGB, Shape);
                    _G.DrawPath(P3, Shape);
                    if ((Image == null))
                    {
                        _G.DrawString(Text, Font, new SolidBrush(_ButtonPressed ? ForeColor2 : ForeColor), R1, new StringFormat
                        {
                            Alignment = _TextAlignment,
                            LineAlignment = StringAlignment.Center
                        });
                    }
                    else
                    {
                        _G.DrawImage(_Image, ipt.X, ipt.Y, ImageSize.Width, ImageSize.Height);
                        _G.DrawString(Text, Font, new SolidBrush(_ButtonPressed ? ForeColor2 : ForeColor), R1, new StringFormat
                        {
                            Alignment = _TextAlignment,
                            LineAlignment = StringAlignment.Center
                        });
                    }
                    break;
            }
            base.OnPaint(e);
        }
    }
}
