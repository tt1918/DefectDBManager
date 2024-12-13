using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.Design;

namespace Coss.Controls
{
    public partial class LedAlarm : UserControl
    {
        private tagAlarmStatus status = tagAlarmStatus.Disabled;
        [Category("LedAlarm")]
        [DefaultValue(tagAlarmStatus.Disabled)]
        public tagAlarmStatus Status
        {
            get { return status; }
            set
            {
                status = value;
                SetVisibleCore(status != tagAlarmStatus.Hide);
                switch (status)
                {
                    case tagAlarmStatus.Disabled: this.BackColor = Color.Gray; break;
                    case tagAlarmStatus.Noraml: this.BackColor = Color.FromArgb(0, 255, 0); break;
                    case tagAlarmStatus.Warning: this.BackColor = Color.Magenta; break;
                    case tagAlarmStatus.Error: this.BackColor = Color.Red; break;
                }

                if(this.useTimer)
                {
                    if (status == tagAlarmStatus.Warning || status == tagAlarmStatus.Error)
                    {
                        if (this.blink.Interval == Int32.MaxValue)
                        {
                            this.blink.Interval = 500;
                            this.blink.Start();
                        }
                    }
                    else
                    {
                        this.blink.Stop();
                        this.blink.Interval = Int32.MaxValue;
                    }
                }
            }
        }
        private string alarmText;
        [Category("LedAlarm")]
        [DefaultValue("")]
        public string AlarmText
        {
            get { return alarmText; }
            set { alarmText = value; Invalidate(); }
        }

        private StringAlignment align;
        [Category("LedAlarm")]
        [DefaultValue(StringAlignment.Near)]
        public StringAlignment Align
        {
            get { return align; }
            set { align = value; Invalidate(); }
        }


        private Timer blink;
        private void OnBlinkTimer(object sender, EventArgs e)
        {
            if (this.BackColor != Color.Transparent)
            {
                this.BackColor = Color.Transparent;
            }
            else
            {
                Status = status;
            }
            
        }

        public void Toggle()
        {
            if (status == tagAlarmStatus.Warning || status == tagAlarmStatus.Error)
            {
                if (this.BackColor != Color.Transparent)
                {
                    this.BackColor = Color.Transparent;
                }
                else
                {
                    Status = status;
                }
            }
            else
            {
                Status = status;
            }
        }

        private bool useTimer = false;
        public LedAlarmDoubleClickedHander DoubleClicked;
        public LedAlarm(bool useTimer)
        {
            this.useTimer = useTimer;

            InitializeComponent();

            if(this.useTimer)
            {
                this.blink = new System.Windows.Forms.Timer();
                this.blink.Interval = Int32.MaxValue; ;
                this.blink.Tick += new EventHandler(OnBlinkTimer);
            }
        }

        public void Init(LedAlarmDoubleClickedHander handle)
        {
            if (handle != null)
                DoubleClicked += handle;
        }

        private void LedAlarm_Load(object sender, EventArgs e)
        {
        }

        private void LedAlarm_Paint(object sender, PaintEventArgs e)
        {
             e.Graphics.DrawString(AlarmText, Font, new SolidBrush(ForeColor), ClientRectangle, new StringFormat
             {
                 Alignment = align,
                 LineAlignment = StringAlignment.Center
             });
        }

        private void LedAlarm_DoubleClick(object sender, EventArgs e)
        {
            if (DoubleClicked != null)
                DoubleClicked(sender, e);
        }
    }
}
