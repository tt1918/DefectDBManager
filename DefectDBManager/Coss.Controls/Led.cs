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
    public class Led : UserControl
    {
        private PictureBox pictureBox1;
        private IContainer components;
        private ImageList imageList1;
        public Led()
        {
            InitializeComponent();
        }

        private LedColor ledColor = LedColor.Green;
        [Category("Led")]
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

        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Led));
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "LED_GRAY_32x16.png");
            this.imageList1.Images.SetKeyName(1, "LED_GREEN_32x16.png");
            this.imageList1.Images.SetKeyName(2, "LED_YELLOW_32x16.png");
            this.imageList1.Images.SetKeyName(3, "LED_RED_32x16.png");
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox1.Image = global::Coss.Controls.Properties.Resources.LED_GRAY_32x16;
            this.pictureBox1.Location = new System.Drawing.Point(0, 0);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(32, 16);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // Led
            // 
            this.BackColor = System.Drawing.Color.Transparent;
            this.Controls.Add(this.pictureBox1);
            this.Name = "Led";
            this.Size = new System.Drawing.Size(35, 18);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

        }
    }
}
