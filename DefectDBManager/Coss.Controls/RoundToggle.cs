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
    #region Toggle Button

    [DefaultEvent("ToggledChanged")]
    public class RoundToggle : Control
    {

        #region Designer
        public class PillStyle
        {
            public bool Left;
            public bool Right;
        }

        public GraphicsPath Pill(Rectangle Rectangle, PillStyle PillStyle)
        {
            GraphicsPath functionReturnValue = default(GraphicsPath);
            functionReturnValue = new GraphicsPath();

            if (PillStyle.Left)
            {
                functionReturnValue.AddArc(new Rectangle(Rectangle.X, Rectangle.Y, Rectangle.Height, Rectangle.Height), -270, 180);
            }
            else
            {
                functionReturnValue.AddLine(Rectangle.X, Rectangle.Y + Rectangle.Height, Rectangle.X, Rectangle.Y);
            }

            if (PillStyle.Right)
            {
                functionReturnValue.AddArc(new Rectangle(Rectangle.X + Rectangle.Width - Rectangle.Height, Rectangle.Y, Rectangle.Height, Rectangle.Height), -90, 180);
            }
            else
            {
                functionReturnValue.AddLine(Rectangle.X + Rectangle.Width, Rectangle.Y, Rectangle.X + Rectangle.Width, Rectangle.Y + Rectangle.Height);
            }

            functionReturnValue.CloseAllFigures();
            return functionReturnValue;
        }

        public object Pill(int X, int Y, int Width, int Height, PillStyle PillStyle)
        {
            return Pill(new Rectangle(X, Y, Width, Height), PillStyle);
        }

        #endregion
        #region Enums

        public enum _Type
        {
            YesNo,
            OnOff,
            User
        }

        #endregion
        #region Variables

        private Timer AnimationTimer = new Timer { Interval = 1 };
        private Timer AnimationTimer2 = new Timer { Interval = 100 };
        private int ToggleLocation = 0;
        public event ToggledChangedEventHandler ToggledChanged;
        public delegate void ToggledChangedEventHandler();
        private bool _Toggled;
        private _Type ToggleType;
        private Rectangle Bar;
        private Size cHandle = new Size(15, 20);
        private bool _Waiting;
        private int WaitingLocation = -1;
        private bool _UseWaiting;
        #endregion
        #region Properties

        public bool Toggled
        {
            get { return _Toggled; }
            set
            {
                if (_UseWaiting)
                {
                    if(_Toggled != value)
                        Waiting = true;
                }

                _Toggled = value;
                Invalidate();

                if (ToggledChanged != null)
                {
                    ToggledChanged();
                }
            }
        }

        public _Type Type
        {
            get { return ToggleType; }
            set
            {
                ToggleType = value;
                Invalidate();
            }
        }

        string[] userText = new string[2];
        public string[] UserText
        {
            get { return userText; }
            set
            {
                userText = value;
                Invalidate();
            }
        }

        Color selectedColor = Color.FromArgb(0, 255, 0);
        public Color SelectedColor
        {
            get { return selectedColor; }
            set
            {
                selectedColor = value;
                Invalidate();
            }
        }
        Color unselectedColor = Color.LightGray;
        public Color UnSelectedColor
        {
            get { return unselectedColor; }
            set
            {
                unselectedColor = value;
                Invalidate();
            }
        }
        public bool Waiting
        {
            get { return _Waiting; }
            set {
                _Waiting = value;
                if (_Waiting == true)
                {
                    WaitingLocation = 0;
                    AnimationTimer2.Start();
                }
                else
                {
                    AnimationTimer2.Stop();
                    WaitingLocation = -1;
                }
                Invalidate(false);
            }
        }

        public bool UseWaiting
        {
            get { return _UseWaiting; }
            set { _UseWaiting = value; }
        }

        #endregion
        #region EventArgs

        protected override void OnResize(EventArgs e)
        {
            base.OnResize(e);
            cHandle = new Size((int)(this.Width * 0.35), (int)(this.Height * 0.9));
    }

        protected override void OnMouseUp(System.Windows.Forms.MouseEventArgs e)
        {
            base.OnMouseUp(e);
            Toggled = !Toggled;
        }

        #endregion

        public RoundToggle()
        {
            SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.DoubleBuffer | ControlStyles.ResizeRedraw | ControlStyles.UserPaint, true);
            AnimationTimer.Tick += new EventHandler(AnimationTimer_Tick);
            AnimationTimer2.Tick += new EventHandler(AnimationTimer_Tick_Pie);
            Font = new Font("Segoe UI", 7f, FontStyle.Regular);

        }
        protected override void OnHandleCreated(EventArgs e)
        {
            base.OnHandleCreated(e);
            AnimationTimer.Start();
        }

        void AnimationTimer_Tick(object sender, EventArgs e)
        {
            //  Create a slide animation when toggled on/off
            if ((_Toggled == true))
            {
                if ((ToggleLocation < 100))
                {
                    ToggleLocation += 10;
                    this.Invalidate(false);
                }
            }
            else if ((ToggleLocation > 0))
            {
                ToggleLocation -= 10;
                this.Invalidate(false);
            }
        }
        void AnimationTimer_Tick_Pie(object sender, EventArgs e)
        {
            // Circle안에 들어가는 파이
            if ((_Waiting == true))
            {
                WaitingLocation = (WaitingLocation + 10) % 360;
                this.Invalidate(false);
            }
            else
            {
                WaitingLocation = -1;
                this.Invalidate(false);
            }
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            Graphics G = e.Graphics;
            G.Clear(Parent.BackColor);
            checked
            {
                bool toggled = this.Toggled;
                Point point = new Point(0, (int)Math.Round(unchecked((double)this.Height / 2.0 - (double)this.cHandle.Height / 2.0)));
                Point arg_A8_0 = point;
                Point point2 = new Point(0, (int)Math.Round(unchecked((double)this.Height / 2.0 + (double)this.cHandle.Height / 2.0)));
                //LinearGradientBrush Gradient = new LinearGradientBrush(arg_A8_0, point2, Color.FromArgb(250, 250, 250), Color.FromArgb(240, 240, 240));
                var brush = new SolidBrush(toggled ? SelectedColor : UnSelectedColor);
                this.Bar = new Rectangle((int)(this.Width*0.20), (int)(this.Height * 0.45), (int)(this.Width * 0.48), (int)(this.Height * 0.09));

                G.SmoothingMode = SmoothingMode.AntiAlias;
                G.FillPath(brush, (GraphicsPath)this.Pill(0, (int)Math.Round(unchecked((double)this.Height / 2.0 - (double)this.cHandle.Height / 2.0)), this.Width - 1, this.cHandle.Height - 5, new PillStyle
                {
                    Left = true,
                    Right = true
                }));
                //G.DrawPath(new Pen(Color.FromArgb(177, 177, 176)), (GraphicsPath)this.Pill(0, (int)Math.Round(unchecked((double)this.Height / 2.0 - (double)this.cHandle.Height / 2.0)), this.Width - 1, this.cHandle.Height - 5, new PillStyle
                //{
                //    Left = true,
                //    Right = true
                //}));
                brush.Dispose();

                switch (this.ToggleType)
                {
                    case _Type.YesNo:
                        {
                            if (toggled)
                            {
                                G.DrawString("Yes", Font, Brushes.Black, new RectangleF((float)(this.Width * 0.1), (float)(this.Height * 0.2), (float)(this.Width * 0.9) - cHandle.Width, (float)(this.Height * 0.6)), new StringFormat
                                {
                                    Alignment = StringAlignment.Center,
                                    LineAlignment = StringAlignment.Center
                                });
                            }
                            else
                            {
                                G.DrawString("No", Font, Brushes.Black, new RectangleF((float)(this.Width * 0.1) + cHandle.Width / 2 + 5, (float)(this.Height * 0.2), (float)(this.Width * 0.9) - cHandle.Width / 2, (float)(this.Height * 0.6)), new StringFormat
                                {
                                    Alignment = StringAlignment.Center,
                                    LineAlignment = StringAlignment.Center
                                });
                            }
                            break;
                        }
                    case _Type.OnOff:
                        {
                            if (toggled)
                            {
                                G.DrawString("On", Font, Brushes.Black, new RectangleF((float)(this.Width * 0.1), (float)(this.Height * 0.2), (float)(this.Width * 0.9) - cHandle.Width, (float)(this.Height * 0.6)), new StringFormat
                                {
                                    Alignment = StringAlignment.Center,
                                    LineAlignment = StringAlignment.Center
                                });
                            }
                            else
                            {
                                G.DrawString("Off", Font, Brushes.Black, new RectangleF((float)(this.Width * 0.1) + cHandle.Width / 2 + 5, (float)(this.Height * 0.2), (float)(this.Width * 0.9) - cHandle.Width / 2, (float)(this.Height * 0.6)), new StringFormat
                                {
                                    Alignment = StringAlignment.Center,
                                    LineAlignment = StringAlignment.Center
                                });
                            }
                            break;
                        }
                    case _Type.User:
                        {
                            if (toggled)
                            {
                                G.DrawString(userText[1], Font, Brushes.Black, new RectangleF((float)(this.Width * 0.1), (float)(this.Height * 0.2), (float)(this.Width * 0.9) - cHandle.Width, (float)(this.Height * 0.6)), new StringFormat
                                {
                                    Alignment = StringAlignment.Center,
                                    LineAlignment = StringAlignment.Center
                                });
                            }
                            else
                            {
                                G.DrawString(userText[0], Font, Brushes.Black, new RectangleF((float)(this.Width * 0.1) +  cHandle.Width / 2 + 5, (float)(this.Height * 0.2), (float)(this.Width * 0.9) - cHandle.Width / 2, (float)(this.Height*0.6)), new StringFormat
                                {
                                    Alignment = StringAlignment.Center,
                                    LineAlignment = StringAlignment.Center
                                });
                            }
                            break;
                        }
                }
                var rect = new Rectangle(this.Bar.X + (int)Math.Round(unchecked((double)this.Bar.Width * ((double)this.ToggleLocation / 80.0))) - (int)Math.Round((double)this.cHandle.Width / 2.0), this.Bar.Y + (int)Math.Round((double)this.Bar.Height / 2.0) - (int)Math.Round(unchecked((double)this.cHandle.Height / 2.0 - 1.0)), this.cHandle.Width, this.cHandle.Height - 5);
                G.FillEllipse(new SolidBrush(Color.FromArgb(108, 118, 125)), rect);
                G.DrawEllipse(new Pen(Color.FromArgb(177, 177, 176)), rect);
                if(UseWaiting)
                {
                    if (WaitingLocation >= 0)
                    {
                        G.FillPie(new SolidBrush(SelectedColor), rect, WaitingLocation, 90);
                    }
                    else
                    {
                        G.DrawLine(new Pen(SelectedColor, 5), (int)(rect.X + rect.Width * 0.3), (int)(rect.Y + rect.Height * 0.3), (int)(rect.X + rect.Width * 0.5), (int)(rect.Y + rect.Height * 0.7));
                        G.DrawLine(new Pen(SelectedColor, 5), (int)(rect.X + rect.Width * 0.5), (int)(rect.Y + rect.Height * 0.7), (int)(rect.X + rect.Width * 0.7), (int)(rect.Y + rect.Height * 0.3));
                    }
                }
            }
        }
    }
    #endregion
}
