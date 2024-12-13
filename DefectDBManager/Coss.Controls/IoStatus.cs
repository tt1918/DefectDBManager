using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Coss.Controls
{
    public partial class IoStatus : UserControl
    {
        public enum LedType
        {
            Gray,
            Green,
            Yellow,
            Red,
            Blue
        }
        public IoStatus()
        {
            InitializeComponent();
            SetImage(LedType.Gray);
        }


        public void SetText(string text)
        {
            label1.Text = text;
        }

        public void SetTextColor(Color color)
        {
            label1.ForeColor = color;
        }
        public void SetTextBackColor(Color color)
        {
            label1.BackColor = color;
        }

        public void SetImage(LedType led)
        {
            this.pictureBox1.Image = this.imageList1.Images[(int)led];
        }

        public void SetImageBackColor(Color color)
        {
            this.pictureBox1.BackColor = color;
        }
    }
}
