using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Text;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AI_Server.Control
{
    public partial class ucClock : UserControl
    {
        private PrivateFontCollection _privateFont = new PrivateFontCollection();
        Font font, font1;
        public ucClock()
        {
            InitializeComponent();
        }

        private void ucClock_Load(object sender, EventArgs e)
        {
            initClock();
        }

        #region 시계 타이머 
        System.Windows.Forms.Timer clockTimer = new System.Windows.Forms.Timer();
        private void initClock()
        {
            try
            {
                _privateFont.AddFontFile("Font\\DS-DIGIB.TTF");
                font = new Font(_privateFont.Families[0], 28.0f);
                lblTime.Font = font;

                font1 = new Font(_privateFont.Families[0], 14.0f);
                lblDate.Font = font1;
                lblSec.Font = font1;
                lbAmPm.Font = font1;
                setClockTimer();
            }
            catch
            {

            }
        }

        private void setClockTimer()
        {
            clockTimer.Interval = 1000;
            clockTimer.Tick += new EventHandler(timer_Clock);
            clockTimer.Start();
        }

        private void timer_Clock(object sender, EventArgs e)
        {
            DateTime time = DateTime.Now;
            lblDate.Text = time.ToString("yyyy-MM-dd");
            lblTime.Text = time.ToString("hh:mm");
            lblSec.Text = time.ToString("ss");
            lbAmPm.Text = time.ToString("tt", CultureInfo.InvariantCulture);
        }
        #endregion
    }
}
