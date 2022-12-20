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

        public eProcessState State
        {
            get;
            private set;
        }

        public DbProgress Progress { get; set; }

        private bool isOn = false;
        private System.Windows.Forms.Timer timer;
        private Image ledOn;
        private Image ledOff;

        private bool disposed = false;
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
            if (disposed == true)
                return;
            DisposeCtrl(false);
        }

        new public void Dispose()
        {
            DisposeCtrl(true);
            GC.SuppressFinalize(this);
        }

        private void DisposeCtrl(bool disposing)
        {
            if (this.disposed)
                return;
            if (disposing)
            {
                ledOn?.Dispose();
                ledOff?.Dispose();
            }
            this.disposed = true;
        }

        private void dispState(object sender, EventArgs e)
        {
            if(Progress.IsComplete() == true)
            {
                timer.Stop();
                lblLED.Image = ledOn;
                if (Progress.IsSkip == true)    lblState.Text = "SKIP";
                else                            lblState.Text = "COMPLETE";
                State = eProcessState.Complete;
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
            timer.Stop();
            lblLED.Image = ledOff;
            lblState.Text = "READY";
            State = eProcessState.Ready;
        }
        
        public void Set()
        {
            timer.Start();
            lblLED.Image = ledOn;
            isOn = true;
            lblState.Text = "PROCESS";
            State = eProcessState.Run;
        }

        public void Complete()
        {
            timer.Stop();
            lblLED.Image = ledOn;
            isOn = true;
            lblState.Text = "COMPLETE";
            State = eProcessState.Complete;
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
            State = eProcessState.Error;
        }
    }
}
