using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Drawing.Drawing2D;

namespace Coss.Controls
{
    internal static class RoundedRectangle
    {
        public static GraphicsPath RoundedRect(Rectangle bounds, int radius)
        {
            int diameter = radius * 2;
            Size size = new Size(diameter, diameter);
            Rectangle arc = new Rectangle(bounds.Location, size);
            GraphicsPath path = new GraphicsPath();

            if (radius == 0)
            {
                path.AddRectangle(bounds);
                return path;
            }

            // top left arc  
            path.AddArc(arc, 180, 90);

            // top right arc  
            arc.X = bounds.Right - diameter;
            path.AddArc(arc, 270, 90);

            // bottom right arc  
            arc.Y = bounds.Bottom - diameter;
            path.AddArc(arc, 0, 90);

            // bottom left arc 
            arc.X = bounds.Left;
            path.AddArc(arc, 90, 90);

            path.CloseFigure();
            return path;
        }

        public static GraphicsPath UpperRoundedRect(Rectangle bounds, int radius)
        {
            int diameter = radius * 2;
            Size size = new Size(diameter, diameter);
            Rectangle arc = new Rectangle(bounds.Location, size);
            GraphicsPath path = new GraphicsPath();

            if (radius == 0)
            {
                path.AddRectangle(bounds);
                return path;
            }

            // top left arc  
            path.AddArc(arc, 180, 90);

            // top right arc  
            arc.X = bounds.Right - diameter;
            path.AddArc(arc, 270, 90);

            path.AddLine(new Point(bounds.Right, bounds.Top + diameter), new Point(bounds.Right, bounds.Bottom));
            path.AddLine(new Point(bounds.Right, bounds.Bottom), new Point(bounds.Left, bounds.Bottom));
            path.AddLine(new Point(bounds.Left, bounds.Bottom), new Point(bounds.Left, bounds.Top + diameter));

            path.CloseFigure();
            return path;
        }

        public static GraphicsPath LowerRoundedRect(Rectangle bounds, int radius)
        {
            int diameter = radius * 2;
            Size size = new Size(diameter, diameter);
            Rectangle arc = new Rectangle(bounds.Location, size);
            GraphicsPath path = new GraphicsPath();

            if (radius == 0)
            {
                path.AddRectangle(bounds);
                return path;
            }

            path.AddLine(new Point(bounds.Left, bounds.Top), new Point(bounds.Right, bounds.Top));
            path.AddLine(new Point(bounds.Right, bounds.Top), new Point(bounds.Right, bounds.Bottom - diameter));

            // bottom right arc  
            arc.X = bounds.Right - diameter;
            arc.Y = bounds.Bottom - diameter;
            path.AddArc(arc, 0, 90);

            // bottom left arc 
            arc.X = bounds.Left;
            path.AddArc(arc, 90, 90);

            path.AddLine(new Point(bounds.Left, bounds.Bottom - diameter), new Point(bounds.Left, bounds.Top));

            path.CloseFigure();
            return path;
        }

        public static void FillRoundedRectangle(Graphics graphics, GraphicsPath path, Brush brush)
        {
            if (graphics == null)
                throw new ArgumentNullException("graphics");
            if (brush == null)
                throw new ArgumentNullException("brush");

            graphics.FillPath(brush, path);
        }

        public static void DrawRoundedRectangle(Graphics graphics, GraphicsPath path, Pen pen)
        {
            if (graphics == null)
                throw new ArgumentNullException("graphics");
            if (pen == null)
                throw new ArgumentNullException("pen");

            graphics.DrawPath(pen, path);
        }
    }
}
