using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DefectDBManager.DB.Form
{
    public partial class ucProgress : UserControl
    {
        public string Title
        {
            get { return this.lblProcessName.Text; }
            set { this.lblProcessName.Text = value; }
        }

        public string State
        {
            get { return this.lblState.Text; }
            set { this.lblState.Text = value; }
        }

        public DbProgress Progress { get; set; }

        private bool isOn = false;
        private System.Windows.Forms.Timer timer;
        private Image ledOn;
        private Image ledOff;
        public ucProgress()
        {
            InitializeComponent();

            timer = new System.Windows.Forms.Timer();
            timer.Interval = 500;
            timer.Tick += new EventHandler(dispState);

            ledOn = Properties.Resources.icons8_green_square_16;
            ledOff = Properties.Resources.icons8_black_medium_square_16;
        }
        ~ucProgress()
        {
            ledOn?.Dispose(); 
            ledOff?.Dispose();
        }

        private void dispState(object sender, EventArgs e)
        {
            if(Progress.IsComplete() == true || Progress.IsSkip == true)
            {
                timer.Stop();
                lblLED.Image = ledOn;
                if (Progress.IsSkip == true)    lblState.Text = "SKIP";
                else                            lblState.Text = "COMPLETE";
            }
            else
            {
                if(isOn==true)  lblLED.Image = ledOn;
                else            lblLED.Image = ledOff;

                isOn = !isOn;
            }
        }

        public void Reset()
        {
            lblLED.Image = ledOff;
            lblState.Text = "READY";
            timer.Stop();
        }
        
        public void Set()
        {
            lblLED.Image = ledOn;
            isOn = true;
            timer.Start();
            lblState.Text = "PROCESS";
        }

        public void Stop()
        {
            timer.Stop();
        }

        public void SetError()
        {
            timer.Stop();
            lblLED.Image = ledOn;
            lblState.Text = "ERROR";
        }
    }
}
