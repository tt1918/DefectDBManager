using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Windows.Forms;
using System.Drawing.Drawing2D;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace Coss.Controls
{
    public enum LightStatus
    {
        Off = 0,
        On,
        Adjust,
        Error,
        None,
    }
    public partial class DefectInfo : UserControl
    {
        [Category("DefectInfo")]
        private Color parentBackColor = Color.DimGray;
        public Color ParentBackColor
        {
            get { return parentBackColor; }
            set { parentBackColor = value; Invalidate(); }
        }


        [Category("DefectInfo")]
        [DefaultValue(11)]
        public int Diameter
        {
            get { return diameter; }
            set { diameter = value; Invalidate(); }
        }


        [Category("DefectInfo")]
        public string Title
        {
            get { return label1.Text; }
            set { label1.Text = value; }
        }


        Color titleColor = Color.Transparent;
        [Category("DefectInfo")]
        public Color TitleColor
        {
            get { return titleColor; }
            set { titleColor = value; Invalidate(); }
        }

        [Category("DefectInfo")]
        public Font TitleFont
        {
            get { return label1.Font; }
            set { label1.Font.Dispose(); label1.Font = value; }
        }

        [Category("DefectInfo")]
        public Font DataFont
        {
            get { return this.label2.Font; }
            set { this.label2.Font.Dispose(); this.label2.Font = value; }
        }

        bool showRelativePosX = false;
        [Category("DefectInfo")]
        public bool ShowRelativePosX
        {
            get { return showRelativePosX; }
            set { showRelativePosX = value; }
        }
        bool showMarkingInfo = false;
        [Category("DefectInfo")]
        public bool ShowMarkingInfo
        {
            get { return showMarkingInfo; }
            set { showMarkingInfo = value; }
        }
        bool showLight = false;
        [Category("DefectInfo")]
        public bool ShowLight
        {
            get { return showLight; }
            set { showLight = value; ShowLightControl(value); }
        }
        int lightPower = 0;
        [Category("DefectInfo")]
        public int LightPower
        {
            get { return lightPower; }
            set { lightPower = value; SetLightValue(value); }
        }
        LightStatus lightLed = LightStatus.Off;
        [Category("DefectInfo")]
        public LightStatus LightLed
        {
            get { return lightLed; }
            set { lightLed = value; SetLightLed(value); }
        }
        int lightChannel = -1;
        [Category("DefectInfo")]
        public int LightChannel
        {
            get { return lightChannel; }
            set { lightChannel = value; SetLightChannel(value); }
        }
        bool showHeader = false;
        [Category("DefectInfo")]
        public bool ShowHeader
        {
            get { return showHeader; }
            set { showHeader = value; ShowHeaderControl(value); }
        }
        bool showStatus = false;
        [Category("DefectInfo")]
        public bool ShowStatus
        {
            get { return showStatus; }
            set { showStatus = value; Invalidate(); }
        }

        Color statusColor = Color.Transparent;
        [Category("DefectInfo")]
        public Color StatusColor
        {
            get { return statusColor; }
            set { statusColor = value; Invalidate(); }
        }
        bool showOpticName = false;
        [Category("DefectInfo")]
        public bool ShowOpticName
        {
            get { return showOpticName; }
            set { showOpticName = value; }
        }

        public class DefectData
        {
            public string OpticName { get; set; }
            public int Level { get; set; }
            public string PcNo { get; set; }
            public double SizeX { get; set; }
            public double SizeY { get; set; }
            public double PosX { get; set; }
            public double PosY { get; set; }
            public int VValue { get; set; }
            public bool Mark { get; set; }
            public string DefectName { get; set; }
            public int DefectClass { get; set; }
            public int Frame { get; set; }
            public string FileName { get; set; }
            public string AiInfo { get; set; }

            public DefectData()
            {
            }
            public DefectData(string opticName, int level, string pcNo, double sizeX, double sizeY, double posX, double posY, 
                              int vValue, bool mark, string defectName, int defectClass, int frame, string fileName, string aiInfo = null)
            {
                OpticName = opticName;
                Level = level;
                PcNo = pcNo;
                SizeX = sizeX;
                SizeY = sizeY;
                PosX = posX;
                PosY = posY;
                VValue = vValue;
                Mark = mark;
                DefectClass = defectClass;
                DefectName = defectName;
                Frame = frame;
                FileName = fileName;
                AiInfo = aiInfo;
            }
            public void Set(DefectData value)
            {
                OpticName = value.OpticName;
                Level = value.Level;
                PcNo = value.PcNo;
                SizeX = value.SizeX;
                SizeY = value.SizeY;
                PosX = value.PosX;
                PosY = value.PosY;
                VValue = value.VValue;
                Mark = value.Mark;
                DefectClass = value.DefectClass;
                DefectName = value.DefectName;
                Frame = value.Frame;
                FileName = value.FileName;
                AiInfo = value.AiInfo;
            }
        }
        DefectData info = new DefectData();
        [Category("DefectInfo")]
        public DefectData Info
        {
            get { return info; }
            set { info.Set(value); SetInfo(); }
        }

        public enum tagViewSize
        {
            Small = 0,
            Medium
        }
        tagViewSize viewSize = tagViewSize.Small;
        [Category("DefectInfo")]
        public tagViewSize ViewSize
        {
            get { return viewSize; }
            set { viewSize = value; SetShowInfoVertical(viewSize==tagViewSize.Medium?true:false); }
        }

        private bool showImage = true;
        [Category("DefectInfo")]
        public bool ShowImage
        {
            get { return showImage; }
            set { showImage = value; SetShowImage(value); }
        }

        /// <summary>
        /// 광학계 이름 표시 부분 높이
        /// </summary>
        private int labelHeight = 30;

        public void SetImage(string path)
        {
            if (this.pictureBox1.Image != null)
                this.pictureBox1.Image.Dispose();
            this.pictureBox1.Image = Image.FromFile(path);
        }
        public void SetImage(Image value)
        {
            if (this.pictureBox1.Image != null)
                this.pictureBox1.Image.Dispose();
            this.pictureBox1.Image = value;
        }
        public Image GetImage()
        {
            if (this.pictureBox1.Image != null)
            {
                return (Image)this.pictureBox1.Image.Clone();
            }
            return null;
        }
        public void SetShowInfoVertical(bool value)
        {
            if (value)
            {
                this.pictureBox1.Dock = DockStyle.Top;
            }
            else
            {
                this.pictureBox1.Dock = DockStyle.Left;
            }

            if (viewSize == tagViewSize.Small)
            {
                labelHeight = 26;
                this.panel1.Height = labelHeight;
                this.pictureBox1.SizeMode = PictureBoxSizeMode.Normal;
            }
            else
            {
                labelHeight = 30;
                this.panel1.Height = labelHeight + 3;
                this.pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;
            }
            OnResize(null);


            if (viewSize == tagViewSize.Small)
            {
                this.Padding = new Padding(2, 1, 2, 2);

                //128x128로 고정
                int width = 128;
                this.pictureBox1.Size = new Size(width, 0);
                this.label2.TextAlign = ContentAlignment.MiddleLeft;
            }
            else
            {
                //height=160 고정
                int width = 160;
                this.Padding = new Padding(2, 2, 2, 2);
                this.pictureBox1.Size = new Size(width, width);
                this.label2.TextAlign = ContentAlignment.MiddleCenter;
            }
            SetInfo();
        }

        public void SetInfo()
        {
            string mark = language_kr ? (info.Mark ? "마킹" : "미마킹") : (info.Mark ? "Marking" : "Non Marking");
            if (viewSize == tagViewSize.Small)
            {
                if (showOpticName)
                {
                    if (!string.IsNullOrWhiteSpace(info.AiInfo))
                    {
                        this.label2.Text = String.Format("PC: {0}({1})\nS: {2:f3}mm\nV: {3}\nX: {4:f1}mm\nY: {5:f3}M\nT: {6}/ Lv: {7}\nAi: {8}",
                        info.PcNo, info.OpticName, (info.SizeX + info.SizeY) / 2, info.VValue, info.PosX, info.PosY / 1000, info.DefectName, info.Level + 1, info.AiInfo);
                    }
                    else
                    {
                        this.label2.Text = String.Format("PC: {0}({1})\nS: {2:f3}mm\nV: {3}\nX: {4:f1}mm\nY: {5:f3}M\nT: {6}/ Lv: {7}",
                        info.PcNo, info.OpticName, (info.SizeX + info.SizeY) / 2, info.VValue, info.PosX, info.PosY / 1000, info.DefectName, info.Level + 1);
                    }
                }
                else
                {
                    if (!string.IsNullOrWhiteSpace(info.AiInfo))
                    {
                        this.label2.Text = String.Format("PC: {0}\nS: {1:f3}mm\nV: {2}\nX: {3:f1}mm\nY: {4:f3}M\nT: {5}/ Lv: {6}\nAi: {7}",
                        info.PcNo, (info.SizeX + info.SizeY) / 2, info.VValue, info.PosX, info.PosY / 1000, info.DefectName, info.Level + 1, info.AiInfo);
                    }
                    else
                    {
                        this.label2.Text = String.Format("PC: {0}\nS: {1:f3}mm\nV: {2}\nX: {3:f1}mm\nY: {4:f3}M\nT: {5}/ Lv: {6}",
                        info.PcNo, (info.SizeX + info.SizeY) / 2, info.VValue, info.PosX, info.PosY / 1000, info.DefectName, info.Level + 1);
                    }
                }
            }
            else
            {
                if (!string.IsNullOrWhiteSpace(info.AiInfo))
                {
                    this.label2.Text = String.Format("PC: {0}  {1}({2})\nX: {3:f1}mm   Y: {4:f3}M\nLv: {5}   S: {6:f2}mm   V: {7}\nF: {8}\n{9}\nAi: {10}",
                    info.PcNo, info.DefectName, info.OpticName, info.PosX, info.PosY / 1000, info.Level + 1, (info.SizeX + info.SizeY) / 2, info.VValue, info.Frame, info.FileName, info.AiInfo);
                }
                else
                {
                    this.label2.Text = String.Format("PC: {0}  {1}({2})\nX: {3:f1}mm   Y: {4:f3}M\nLv: {5}   S: {6:f2}mm   V: {7}\nF: {8}\n{9}",
                    info.PcNo, info.DefectName, info.OpticName, info.PosX, info.PosY / 1000, info.Level + 1, (info.SizeX + info.SizeY) / 2, info.VValue, info.Frame, info.FileName);
                }
            }
        }
        public void ShowFilterControl(bool show)
        {
            if (show) ledThin1.Show();
            else ledThin1.Hide();
        }
        public string GetFilterText()
        {
            return ledThin1.Text;
        }

        public void SetFilterText(string text)
        {
            ledThin1.Text = text;
        }
        public void SetFilterCount(int count)
        {
            ledThin1.Count = count;
        }
        public void SetFilterLed(int index, LightStatus status)
        {
            LedColor color = LedColor.Gray;
            if (status == LightStatus.Off)
                color = LedColor.Gray;
            else if (status == LightStatus.On)
                color = LedColor.Green;
            else if (status == LightStatus.Adjust)
                color = LedColor.Yellow;
            else if (status == LightStatus.Error)
                color = LedColor.Red;

            if (index == -1)
                ledThin1.Color = color;
            else if(index >= 0 && index < ledThin1.Count)
                ledThin1.SetColor(index,color);
        }
        public void SetLightChannelNames(string[] names)
        {
            light1.SetLightChannelNames(names);
        }
        public string[] GetLightChannelNames()
        {
            return light1.GetLightChannelNames();
        }

        private void ShowLightControl(bool show)
        {
            if(show)
                light1.Show();
            else
                light1.Hide();
        }

        private void SetLightValue(int value)
        {
            light1.LightValue = value;
        }
        private void SetLightChannel(int value)
        {
            light1.Channel = value;

            //채널정보가 들어가면, 폭이 커져야 한다.
            if (light1.InvokeRequired)
            {
                light1.BeginInvoke(new Action(() =>
                {
                    light1.Width = 90;
                }));
            }
            else
            {
                light1.Width = 90;
            }
        }
        
        private void SetLightLed(LightStatus status)
        {
            if (status == LightStatus.Off)
                light1.Color = LedColor.Gray;
            else if (status == LightStatus.On)
                light1.Color = LedColor.Green;
            else if (status == LightStatus.Adjust)
                light1.Color = LedColor.Yellow;
            else if (status == LightStatus.Error)
                light1.Color = LedColor.Red;
        }
        private void ShowHeaderControl(bool show)
        {
            panel1.Visible = show;
        }

        public void Reset()
        {
            this.label2.Text = " - ";
            this.pictureBox1.Image = null;
        }

        bool language_kr = true;
        public DefectInfo()
        {
            InitializeComponent();
            SetStyle(ControlStyles.SupportsTransparentBackColor, true);
            SetStyle(ControlStyles.UserPaint, true);

            BackColor = Color.Transparent;
            this.Size = new Size(187, 117);
            this.label1.Margin = new Padding(0, 0, 0, 0);
            this.label1.TextAlign = ContentAlignment.TopCenter;
            this.label2.Padding = new Padding(0, 0, 0, 0);
            this.pictureBox1.Margin = new Padding(0, 0, 0, 0);

            DoubleBuffered = true;

            Region = System.Drawing.Region.FromHrgn(Win32.CreateRoundRectRgn(0, 0, Width, Height, 10, 10));

            language_kr = Thread.CurrentThread.CurrentUICulture.Name == "ko-KR" ? true : false;
        }


        private GraphicsPath StatusShape = null;
        private GraphicsPath Shape = null;
        private int diameter = 11;
        private IntPtr rgnHandle = IntPtr.Zero;
        private int statusWidth = 20;

        private void ResizeShape()
        {
            if (Shape != null)
            {
                Shape.Dispose();
                Shape = null;
            }
            Shape = new GraphicsPath();
            var shape = Shape;

            // diameter can't exceed width or height
            if (diameter > this.Width) diameter = this.Width;
            if (diameter > this.Height) diameter = this.Height;

            Rectangle arc = new Rectangle(0, 0, diameter, diameter);
            Rectangle bound = new Rectangle(0, 0, this.Width, labelHeight);

            // top left
            shape.AddArc(arc, 180, 90);

            // top right arc  
            arc.X = bound.Width - diameter - 1;
            shape.AddArc(arc, 270, 90);

            shape.AddLine(new Point(bound.Width, diameter), new Point(bound.Width, bound.Height));
            shape.AddLine(new Point(bound.Width, bound.Height), new Point(bound.Left, bound.Height));
            shape.AddLine(new Point(bound.Left, bound.Height), new Point(bound.Left, bound.Top + diameter));

            shape.CloseFigure();
        }
        private void ResizeStatusShape()
        {
            if (StatusShape != null)
            {
                StatusShape.Dispose();
                StatusShape = null;
            }
            StatusShape = new GraphicsPath();
            var shape = StatusShape;
            var width = statusWidth;

            Rectangle arc = new Rectangle(0, 0, diameter, diameter);
            Rectangle bound = new Rectangle(0, 0, width, labelHeight);

            // top left
            shape.AddArc(arc, 180, 90);

            shape.AddLine(new Point(bound.Left + diameter, bound.Top), new Point(bound.Width, bound.Top));
            shape.AddLine(new Point(bound.Width, bound.Top), new Point(bound.Width, bound.Height));
            shape.AddLine(new Point(bound.Width, bound.Height), new Point(bound.Left, bound.Height));
            shape.AddLine(new Point(bound.Left, bound.Height), new Point(bound.Left, bound.Top + diameter));

            shape.CloseFigure();
        }
        protected override void OnResize(System.EventArgs e)
        {
            base.OnResize(e);

            ResizeShape();
            ResizeStatusShape();

            if (rgnHandle != IntPtr.Zero)
            {
                Win32.DeleteObject(rgnHandle);
                rgnHandle = IntPtr.Zero;
            }
            rgnHandle = Win32.CreateRoundRectRgn(0, 0, Width, Height, 10, 10);
            if (Region != null)
            {
                Region.Dispose();
                Region = null;
            }
            Region = System.Drawing.Region.FromHrgn(rgnHandle);
        }

        protected override void OnPaint(System.Windows.Forms.PaintEventArgs e)
        {
            base.OnPaint(e);
            using (var bufferedgraphic = BufferedGraphicsManager.Current.Allocate(e.Graphics, this.ClientRectangle))
            {
                bufferedgraphic.Graphics.Clear(parentBackColor);
                bufferedgraphic.Graphics.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.Bilinear;
                bufferedgraphic.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.Default;

                if (showHeader)
                {
                    using (Brush brush = new SolidBrush(TitleColor))
                    {
                        bufferedgraphic.Graphics.FillPath(brush, Shape); // Draw RTB background
                    }
                    if (showStatus)
                    {
                        using (Brush brush = new SolidBrush(StatusColor))
                        {
                            bufferedgraphic.Graphics.FillPath(brush, StatusShape); // Draw RTB background
                        }
                    }
                }
                bufferedgraphic.Render(e.Graphics);
            }
        }

        private void SetShowImage(bool showImage) 
        {
            if (showImage)
                pictureBox1.Show();
            else 
                pictureBox1.Hide();
        }
    }

    internal class Win32
    {
        public enum Bool
        {
            False = 0,
            True
        };


        [StructLayout(LayoutKind.Sequential)]
        public struct Point
        {
            public Int32 x;
            public Int32 y;

            public Point(Int32 x, Int32 y) { this.x = x; this.y = y; }
        }


        [StructLayout(LayoutKind.Sequential)]
        public struct Size
        {
            public Int32 cx;
            public Int32 cy;

            public Size(Int32 cx, Int32 cy) { this.cx = cx; this.cy = cy; }
        }


        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        struct ARGB
        {
            public byte Blue;
            public byte Green;
            public byte Red;
            public byte Alpha;
        }


        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public struct BLENDFUNCTION
        {
            public byte BlendOp;
            public byte BlendFlags;
            public byte SourceConstantAlpha;
            public byte AlphaFormat;
        }


        public const Int32 ULW_COLORKEY = 0x00000001;
        public const Int32 ULW_ALPHA = 0x00000002;
        public const Int32 ULW_OPAQUE = 0x00000004;

        public const byte AC_SRC_OVER = 0x00;
        public const byte AC_SRC_ALPHA = 0x01;


        [DllImport("user32.dll", ExactSpelling = true, SetLastError = true)]
        public static extern Bool UpdateLayeredWindow(IntPtr hwnd, IntPtr hdcDst, ref Point pptDst, ref Size psize, IntPtr hdcSrc, ref Point pprSrc, Int32 crKey, ref BLENDFUNCTION pblend, Int32 dwFlags);

        [DllImport("user32.dll", ExactSpelling = true, SetLastError = true)]
        public static extern IntPtr GetDC(IntPtr hWnd);

        [DllImport("user32.dll", ExactSpelling = true)]
        public static extern int ReleaseDC(IntPtr hWnd, IntPtr hDC);

        [DllImport("gdi32.dll", ExactSpelling = true, SetLastError = true)]
        public static extern IntPtr CreateCompatibleDC(IntPtr hDC);

        [DllImport("gdi32.dll", ExactSpelling = true, SetLastError = true)]
        public static extern Bool DeleteDC(IntPtr hdc);

        [DllImport("gdi32.dll", ExactSpelling = true)]
        public static extern IntPtr SelectObject(IntPtr hDC, IntPtr hObject);

        [DllImport("gdi32.dll", ExactSpelling = true, SetLastError = true)]
        public static extern Bool DeleteObject(IntPtr hObject);

        [DllImport("Gdi32.dll", EntryPoint = "CreateRoundRectRgn")]
        public static extern IntPtr CreateRoundRectRgn
        (
            int nLeftRect,     // x-coordinate of upper-left corner
            int nTopRect,      // y-coordinate of upper-left corner
            int nRightRect,    // x-coordinate of lower-right corner
            int nBottomRect,   // y-coordinate of lower-right corner
            int nWidthEllipse, // width of ellipse
            int nHeightEllipse // height of ellipse
        );
    }
}
