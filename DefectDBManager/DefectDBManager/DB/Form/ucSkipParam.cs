using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DefectDBManager

{
    public partial class ucSkipParam : UserControl
    {
        public SkipSize _SkipSize 
        {
            get
            {
                UpdateData();
                return skipSize;
            } 
            set
            {
                skipSize = value;
                DisplayData();
            } 
        }
        private SkipSize skipSize;
        
        public string Title 
        { 
            get { return gbCtrlName.Text; } 
            set { gbCtrlName.Text = value; } 
        }

        public ucSkipParam()
        {
            InitializeComponent();
        }

        private void ucSkipParam_Load(object sender, EventArgs e)
        {
        }

        private void ucSkipParam_VisibleChanged(object sender, EventArgs e)
        {
            if(this.Visible==true)
            {
                DisplayData();
            }
        }

        private void DisplayData()
        {
            if (skipSize == null) return;

            tbSizeXMin.Text = $"{skipSize.minX:F0}";
            tbSizeXMax.Text = $"{skipSize.maxX:F0}";
            tbSizeYMin.Text = $"{skipSize.minY:F0}";
            tbSizeYMax.Text = $"{skipSize.maxY:F0}";
            tbSizeMin.Text = $"{skipSize.min:F0}";
            tbSizeMax.Text = $"{skipSize.max:F0}";
        }

        private void UpdateData()
        {
            float val;

            if(float.TryParse(tbSizeXMin.Text, out val)==true)
                skipSize.minX = val;

            if (float.TryParse(tbSizeXMax.Text, out val) == true)
                skipSize.maxX = val;

            if (float.TryParse(tbSizeYMin.Text, out val) == true)
                skipSize.minY = val;

            if (float.TryParse(tbSizeYMax.Text, out val) == true)
                skipSize.maxY = val;

            if (float.TryParse(tbSizeMin.Text, out val) == true)
                skipSize.min = val;

            if (float.TryParse(tbSizeMax.Text, out val) == true)
                skipSize.max = val;
        }
    }
}
