using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

namespace Coss.Controls
{
    public partial class Jog : UserControl
    {
        public string Key { get; private set; }
        public string Title { get { return groupBox1.Text; } set { groupBox1.Text = value; } }
        JogEventHandler jogEvent;
        public string[] Names { get; private set; }
        Stopwatch SwkeyDown = new Stopwatch();
        tagJobType keyDownType = tagJobType.LL;
        Timer timerUpdate= new Timer();

        public Jog()
        {
            InitializeComponent();
        }
        public void Init(string key, string title, string[] names, JogEventHandler handle)
        {
            Key = key;
            Title = title;
            Names = new string[names.Length];
            Array.Copy(names, Names, names.Length);
            jogEvent += handle;

            groupBox1.Text = title;

#if false
            groupBox1.ForeColor = Color.White;
            labelLeft.ForeColor = Color.White;
            labelRight.ForeColor = Color.White;
            buttonLL.ForeColor = Color.Black;
            buttonLR.ForeColor = Color.Black;
            buttonRL.ForeColor = Color.Black;
            buttonRR.ForeColor = Color.Black;
#else
            groupBox1.ForeColor = Color.Black;
            labelLeft.ForeColor = Color.Black;
            labelRight.ForeColor = Color.Black;
            buttonLL.ForeColor = Color.Black;
            buttonLR.ForeColor = Color.Black;
            buttonRL.ForeColor = Color.Black;
            buttonRR.ForeColor = Color.Black;
#endif

            timerUpdate.Interval = 200;
            timerUpdate.Tick += new EventHandler(OnTimerUpdate);
            timerUpdate.Start();
        }
        private void OnTimerUpdate(object sender, EventArgs e)
        {
            //1초 이상 누르고 있으면, 계속 움직임
            if (SwkeyDown.ElapsedMilliseconds > 1000)
            {
                switch (keyDownType)
                {
                    case tagJobType.LL:
                    case tagJobType.LR:
                    case tagJobType.RL:
                    case tagJobType.RR:
                        {
                            jogEvent?.Invoke(this, new JogEventArgs(Key, keyDownType));
                        }
                        break;
                }
            }
        }
        public void SetPos(bool left, int pos)
        {
            if (left)
                labelLeft.Text = $"{Names[0]} {(int)((double)pos / 10000)} mm";
            else
                labelRight.Text = $"{Names[1]} {(int)((double)pos / 10000)} mm";
        }

        private void buttonLL_Click(object sender, EventArgs e)
        {
            jogEvent?.Invoke(this, new JogEventArgs(Key, tagJobType.LL));
        }

        private void buttonLR_Click(object sender, EventArgs e)
        {
            jogEvent?.Invoke(this, new JogEventArgs(Key, tagJobType.LR));
        }

        private void buttonRL_Click(object sender, EventArgs e)
        {
            jogEvent?.Invoke(this, new JogEventArgs(Key, tagJobType.RL));
        }

        private void buttonRR_Click(object sender, EventArgs e)
        {
            jogEvent?.Invoke(this, new JogEventArgs(Key, tagJobType.RR));
        }

        private void buttonLL_MouseDown(object sender, MouseEventArgs e)
        {
            keyDownType = tagJobType.LL;
            SwkeyDown.Restart();
        }
        private void buttonLL_MouseUp(object sender, MouseEventArgs e)
        {
            SwkeyDown.Stop();
            SwkeyDown.Reset();
        }
        private void buttonLR_MouseDown(object sender, MouseEventArgs e)
        {
            keyDownType = tagJobType.LR;
            SwkeyDown.Restart();
        }
        private void buttonLR_MouseUp(object sender, MouseEventArgs e)
        {
            SwkeyDown.Stop();
            SwkeyDown.Reset();
        }
        private void buttonRL_MouseDown(object sender, MouseEventArgs e)
        {
            keyDownType = tagJobType.RL;
            SwkeyDown.Restart();
        }

        private void buttonRL_MouseUp(object sender, MouseEventArgs e)
        {
            SwkeyDown.Stop();
            SwkeyDown.Reset();
        }
        private void buttonRR_MouseDown(object sender, MouseEventArgs e)
        {
            keyDownType = tagJobType.RR;
            SwkeyDown.Restart();
        }

        private void buttonRR_MouseUp(object sender, MouseEventArgs e)
        {
            SwkeyDown.Stop();
            SwkeyDown.Reset();
        }
    }
}
