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
    public class LedThin : UserControl
    {
        /// <summary>
        /// LED구역은 최대 6개까지만 지원
        /// </summary>
        const int kMaxCount = 6;
        private PictureBox pictureBox1;
        private IContainer components;
        private Label label1;
        private ImageList imageList1;
        private LedColor[] colors = new LedColor[kMaxCount] { LedColor.Gray, LedColor.Gray, LedColor.Gray, LedColor.Gray, LedColor.Gray, LedColor.Gray };

        public LedThin()
        {
            InitializeComponent();
        }

        //표시되는 LED 개수
        private int count = 1;
        [Category("LedThin")]
        [DefaultValue(1)]
        public int Count
        {
            get { return count; }
            set
            {
                if (value > 0 && value <= kMaxCount) 
                {
                    count = value;
                    Color = ledColor;
                }
            }
        }

        private LedColor ledColor = LedColor.Gray;
        [Category("LedThin")]
        [DefaultValue(LedColor.Gray)]
        public LedColor Color
        {
            get
            {
                return ledColor;
            }
            set
            {
                ledColor = value;
                var image = (Image)this.imageList1.Images[(int)ledColor].Clone();
                this.pictureBox1.Image?.Dispose();
                this.pictureBox1.Image = image;
            }
        }

        private string text = String.Empty;
        [Category("Light")]
        [DefaultValue(0)]
        public override string Text
        {
            get
            {
                return text;
            }
            set
            {
                text = value;
                label1.Text = value;
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


        public void SetColor(int index, LedColor ledColor)
        {
            colors[index] = ledColor;

            bool allSame = true;
            for(int i=0; i < count && i < kMaxCount; i++)
            {
                if (colors[i] != ledColor) 
                {
                    allSame = false;
                    break;
                }
            }

            if (allSame)
            {
                this.pictureBox1.Image?.Dispose();
                this.pictureBox1.Image = (Image)this.imageList1.Images[(int)ledColor].Clone();
            }
            else
            {
                var gray = this.imageList1.Images[(int)LedColor.Gray];
                //빈 배경을 만들고, 선택된 색상으로 순차적으로 그려준다
                Bitmap canvas = new Bitmap(gray.Width, gray.Height);
                using (System.Drawing.Graphics g = System.Drawing.Graphics.FromImage(canvas))
                {
                    if(count <= 3)
                    {
                        for (int i = 0; i < count; i++)
                        {
                            using (Bitmap set = new Bitmap(this.imageList1.Images[(int)colors[i]]))
                                g.DrawImage(set, new Rectangle(0, i * canvas.Height / count, canvas.Width, (i + 1) * canvas.Height / count), 0, i * set.Height / count, set.Width, (i + 1) * set.Height / count, GraphicsUnit.Pixel);
                        }
                    }
                    else
                    {
                        int rowCount = (int)((count + 1) / 2);
                        for (int i = 0; i < count; i++)
                        {
                            int rowIndex = (int)(i / 2);
                            if(i % 2 == 0)
                            {
                                //왼쪽
                                using (Bitmap set = new Bitmap(this.imageList1.Images[(int)colors[i]]))
                                    g.DrawImage(set, new Rectangle(0, rowIndex * canvas.Height / rowCount, canvas.Width / 2, (rowIndex + 1) * canvas.Height / rowCount), 
                                        0, rowIndex * set.Height / rowCount, set.Width / 2, (rowIndex + 1) * set.Height / rowCount, GraphicsUnit.Pixel);
                            }
                            else
                            {
                                //오른쪽
                                using (Bitmap set = new Bitmap(this.imageList1.Images[(int)colors[i]]))
                                    g.DrawImage(set, new Rectangle(canvas.Width / 2, rowIndex * canvas.Height / rowCount, canvas.Width, (rowIndex + 1) * canvas.Height / rowCount),
                                        set.Width / 2, rowIndex * set.Height / rowCount, set.Width, (rowIndex + 1) * set.Height / rowCount, GraphicsUnit.Pixel);
                            }
                        }
                    }
                }
                this.pictureBox1.Image?.Dispose();
                this.pictureBox1.Image = canvas;
            }
        }
        /// <summary>
        /// 색상을 gray로 초기화
        /// </summary>
        public void ResetColor()
        {
            Array.Clear(colors, 0, colors.Length);
            Color = LedColor.Gray;
        }

        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(LedThin));
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.label1 = new System.Windows.Forms.Label();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "LED_GRAY_16x24.png");
            this.imageList1.Images.SetKeyName(1, "LED_GREEN_16x24.png");
            this.imageList1.Images.SetKeyName(2, "LED_YELLOW_16x24.png");
            this.imageList1.Images.SetKeyName(3, "LED_RED_16x24.png");
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Impact", 11.25F);
            this.label1.Location = new System.Drawing.Point(17, 2);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(25, 19);
            this.label1.TabIndex = 1;
            this.label1.Text = "Off";
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.Color.Transparent;
            this.pictureBox1.Location = new System.Drawing.Point(0, 0);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(16, 24);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // LedThin
            // 
            this.BackColor = System.Drawing.Color.Transparent;
            this.Controls.Add(this.label1);
            this.Controls.Add(this.pictureBox1);
            this.Name = "LedThin";
            this.Size = new System.Drawing.Size(39, 25);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }
    }
}
