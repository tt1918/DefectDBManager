using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace CustomControls
{
    public class RoundLabel :Label 
    {
        #region Param
        [Browsable(true)]
        [ReadOnly(false)]
        [Category("Basic")]
        [DisplayName("Corner Radius")]
        [Description("Radius of corner")]
        public int CornerR
        {
            get { return cornerRadius; }
            set { cornerRadius = value; }
        }
        private int cornerRadius = 15; //라운드 너비

        [Browsable(true)]
        [ReadOnly(false)]
        [Category("Basic")]
        [DisplayName("Border Color")]
        [Description("Border Color")]
        public Color BorderColor
        {
            get { return borderColor; }
            set { borderColor = value; }
        }
        private Color borderColor = Color.DarkGray;//외곽선 색상

        [Browsable(true)]
        [ReadOnly(false)]
        [Category("Basic")]
        [DisplayName("Thickness")]
        [Description("Thickness of border")]
        public int Thickness
        {
            get { return thickness; }
            set { thickness = value; }
        }
        private int thickness = 1;//외곽선 두께

        [Browsable(true)]
        [ReadOnly(false)]
        [Category("Basic")]
        [DisplayName("Back Color")]
        [Description("Color of background")]
        public Color BkColor
        {
            get { return backColor;}
            set { backColor = value; }  
        }
        private Color backColor = Color.LightGray;//배경 색상


        /// <summary>
        /// 왼쪽위 사각으로 채우기(라운드 적용X)
        /// </summary>
        [Browsable(true)]
        [ReadOnly(false)]
        [Category("Fill Corner")]
        [DisplayName("Left Top")]
        [Description("Fill the left top")]
        public bool IsFillLT 
        { 
            get { return isFill[0]; } 
            set { isFill[0] = value; }
        }

        /// <summary>
        /// 오른쪽위 사각으로 채우기(라운드 적용X)
        /// </summary>
        [Browsable(true)]
        [ReadOnly(false)]
        [Category("Fill Corner")]
        [DisplayName("Right Top")]
        [Description("Fill the left top")]
        public bool IsFillRT
        {
            get { return isFill[1]; }
            set { isFill[1] = value; }
        }

        /// <summary>
        /// 왼쪽아래 사각으로 채우기(라운드 적용X)
        /// </summary>
        [Browsable(true)]
        [ReadOnly(false)]
        [Category("Fill Corner")]
        [DisplayName("Left Bottom")]
        [Description("Fill the left top")]
        public bool IsFillLB
        {
            get { return isFill[2]; }
            set { isFill[2] = value; }
        }

        /// <summary>
        /// 오른쪽아래 사각으로 채우기(라운드 적용X)
        /// </summary>
        [Browsable(true)]
        [ReadOnly(false)]
        [Category("Fill Corner")]
        [DisplayName("Right Bottom")]
        [Description("Fill the left top")]
        public bool IsFillRB
        {
            get { return isFill[3]; }
            set { isFill[3] = value; }
        }

        /// <summary>
        /// 사각형 채우기
        /// </summary>
        private bool[] isFill = { false, false, false, false};
        #endregion

        public RoundLabel()
        {
            this.DoubleBuffered = true;
        }

        protected override void OnPaint(PaintEventArgs e) 
        {
            base.OnPaint(e);
            using(var graphicsPath = _getRoundRectangle(this.ClientRectangle)) 
            {
                e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;

                var brush = new SolidBrush(backColor);
                var pen = new Pen(borderColor, thickness);
                e.Graphics.FillPath(brush, graphicsPath);
                e.Graphics.DrawPath(pen, graphicsPath);

                TextRenderer.DrawText(e.Graphics, Text, this.Font, this.ClientRectangle, this.ForeColor);
            }
        }

        private GraphicsPath _getRoundRectangle(Rectangle rectangle) 
        {
            GraphicsPath path = new GraphicsPath();

            int left = rectangle.X;
            int top = rectangle.Y;
            int right = rectangle.X + rectangle.Width - thickness;
            int bottom = rectangle.Y + rectangle.Height - thickness;

            //좌상
            if (IsFillLT) 
            {
                path.AddLine(left, top + cornerRadius, left, top);
                path.AddLine(left, top, left + cornerRadius, top);
            } else
            {
                path.AddArc(rectangle.X, rectangle.Y, cornerRadius, cornerRadius, 180, 90);
            }
            //우상
            if (IsFillRT) 
            {
                path.AddLine(right - cornerRadius, top, right, top);
                path.AddLine(right, top, right, top + cornerRadius);
            } 
            else 
            {
                path.AddArc(rectangle.X + rectangle.Width - cornerRadius - thickness, rectangle.Y, cornerRadius, cornerRadius, 270, 90);
            }

            //우하
            if (IsFillRB)
            {
                path.AddLine(right, bottom - cornerRadius, right, bottom);
                path.AddLine(right, bottom, right - cornerRadius, bottom);
            } else
            {
                path.AddArc(rectangle.X + rectangle.Width - cornerRadius - thickness, rectangle.Y + rectangle.Height - cornerRadius - thickness, cornerRadius, cornerRadius, 0, 90);
            }
            //좌하
            if (IsFillLB) 
            {
                path.AddLine(left + cornerRadius, bottom, left, bottom);
                path.AddLine(left, bottom, left, bottom - cornerRadius);
            } else 
            {
                path.AddArc(rectangle.X, rectangle.Y + rectangle.Height - cornerRadius - thickness, cornerRadius, cornerRadius, 90, 90);
            }

            path.CloseAllFigures();

            return path;
        }
    }
}
