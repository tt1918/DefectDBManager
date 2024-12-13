using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Text;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace Coss.Controls
{
    public class RoundPanel : ContainerControl
    {
        [Category("RoundPanel")]
        private Color parentBackColor = Color.DimGray;
        public Color ParentBackColor
        {
            get { return parentBackColor; }
            set { parentBackColor = value; Invalidate(); }
        }

        [Category("RoundPanel")]
        [DefaultValue(11)]
        public int Diameter
        {
            get { return diameter; }
            set { diameter = value; Invalidate(); }
        }

        public RoundPanel()
        {
            SetStyle(ControlStyles.SupportsTransparentBackColor, true);
            SetStyle(ControlStyles.UserPaint, true);

            BackColor = Color.Transparent;
            this.Size = new Size(187, 117);
            Padding = new Padding(5, 5, 5, 5);
            DoubleBuffered = true;
        }

        private GraphicsPath Shape = null;
        private int diameter = 11;


        protected override void OnResize(System.EventArgs e)
        {
            base.OnResize(e);

            // diameter can't exceed width or height
            if (diameter > this.Width) diameter = this.Width;
            if (diameter > this.Height) diameter = this.Height;

            if (diameter <= 0) return;

            Rectangle arc = new Rectangle(0, 0, diameter, diameter);

            Shape = new GraphicsPath();
            var shape = Shape;

            // top left
            shape.AddArc(arc, 180, 90);

            // top right arc  
            arc.X = Width - diameter -1;
            shape.AddArc(arc, 270, 90);
            // bottom right arc  
            arc.Y = Height - diameter -1;
            shape.AddArc(arc, 0, 90);
            // bottom left arc 
            arc.X = 0;
            shape.AddArc(arc, 90, 90);
            shape.CloseFigure();
        }

        protected override void OnPaint(System.Windows.Forms.PaintEventArgs e)
        {
            base.OnPaint(e);
            using (var bufferedgraphic = BufferedGraphicsManager.Current.Allocate(e.Graphics, this.ClientRectangle))
            {
                bufferedgraphic.Graphics.Clear(parentBackColor);
                bufferedgraphic.Graphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.Bilinear;
                bufferedgraphic.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.Default;

                using (Brush brush = new SolidBrush(BackColor))
                {
                    bufferedgraphic.Graphics.FillPath(brush, Shape); // Draw RTB background
                    bufferedgraphic.Graphics.DrawPath(new Pen(ForeColor), Shape); // Draw border
                }
                bufferedgraphic.Render(e.Graphics);
            }
        }
    }
}
