using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.ComponentModel;
using System.Windows.Forms;

namespace Coss.Controls
{
    public class Light : UserControl
    {
        private PictureBox pictureBox1;
        private IContainer components;
        private Label label1;
        private ImageList imageList1;
        private List<string> channelNames = new List<string>();

        public Light()
        {
            InitializeComponent();
        }

        private LedColor ledColor = LedColor.Green;
        [Category("Light")]
        [DefaultValue(LedColor.Green)]
        public LedColor Color
        {
            get
            {
                return ledColor;
            }
            set
            {
                ledColor = value;
                this.pictureBox1.Image = this.imageList1.Images[(int)ledColor];
            }
        }

        private int lightValue = 0;
        [Category("Light")]
        [DefaultValue(0)]
        public int LightValue
        {
            get
            {
                return lightValue;
            }
            set
            {
                lightValue = value;
                UpdateLabel();
            }
        }

        private int channel = -1;
        [Category("Light")]
        [DefaultValue(-1)]
        public int Channel
        {
            get
            {
                return channel;
            }
            set
            {
                channel = value;
                UpdateLabel();
            }
        }

        public void SetLightChannelNames(string[] names)
        {
            channelNames.Clear();
            channelNames.AddRange(names);
        }

        public string[] GetLightChannelNames()
        {
            return channelNames.ToArray();
        }

        public override Color BackColor
        {
            get
            {
                return base.BackColor;
            }

            set
            {
                base.BackColor = value;
                this.imageList1.TransparentColor = value;
                this.pictureBox1.BackColor = value;
            }
        }

        private void UpdateLabel()
        {
            string text;
            if (channel == -1)
            {
                text = string.Format("{0}%", lightValue);
            }
            else
            {
                if (channel >= 0 && channel < channelNames.Count)
                    text = string.Format("{0}{1}%", channelNames[channel], lightValue);
                else
                    text = string.Format("CH{0}-{1}%", channel, lightValue);
            }
            if (label1.InvokeRequired)
            {
                label1.BeginInvoke(new Action(() =>
                {
                    label1.Text = text;
                }));
            }
            else
            {
                label1.Text = text;
            }
        }

        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Light));
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.label1 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "LED_GRAY.png");
            this.imageList1.Images.SetKeyName(1, "LED_GREEN.png");
            this.imageList1.Images.SetKeyName(2, "LED_YELLOW.png");
            this.imageList1.Images.SetKeyName(3, "LED_RED.png");
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox1.Dock = System.Windows.Forms.DockStyle.Left;
            this.pictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox1.Image")));
            this.pictureBox1.InitialImage = null;
            this.pictureBox1.Location = new System.Drawing.Point(0, 0);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(24, 24);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // label1
            // 
            this.label1.Font = new System.Drawing.Font("Impact", 11.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(24, 0);
            this.label1.Margin = new System.Windows.Forms.Padding(0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(64, 18);
            this.label1.TabIndex = 1;
            this.label1.Text = "100%";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // Light
            // 
            this.BackColor = System.Drawing.Color.Transparent;
            this.Controls.Add(this.label1);
            this.Controls.Add(this.pictureBox1);
            this.Name = "Light";
            this.Size = new System.Drawing.Size(69, 24);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

        }
    }
}
