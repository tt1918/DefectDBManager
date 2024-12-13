using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Coss.Controls
{
    public partial class SelectCameras : UserControl
    {
        public bool UseOptic  
        { 
            get { return checkBoxOptic.Checked; }
            set { checkBoxOptic.Checked = value; }
        }

        bool[] useCameras;

        public int OpticKey { get; set; }

        Control[] checkboxes;
        const int kMaxCamera = 36;

        public SelectCameras()
        {
            InitializeComponent();
        }
        public bool Init(int opticKey, string opticName, Color opticColor, int cameras)
        {
            DoubleBuffered = true;
            OpticKey = opticKey;

            if (checkboxes != null)
            {
                for (int i = 0; i < checkboxes.Length; i++)
                    checkboxes[i] = null;
            }

            checkBoxOptic.Text = opticName;
            checkBoxOptic.BackColor = opticColor;

            if (cameras <= kMaxCamera)
            {
                useCameras = new bool[cameras];
                checkboxes = new Control[cameras];
                for (int i = 0; i < useCameras.Length; i++)
                {
                    var cb = new CheckBox();
                    cb.Parent = this;
                    cb.Text = String.Empty;
                    cb.Name = i.ToString();
                    cb.CheckedChanged += checkBox_CheckedChanged;
                    cb.AutoSize = true;

                    checkboxes[i] = cb;
                    flowLayoutPanel1.Controls.Add(checkboxes[i]);
                }

                return true;
            }

            return false;
        }
        public void Term()
        {
            if (checkboxes != null)
            {
                for (int i = 0; i < checkboxes.Length; i++)
                    checkboxes[i] = null;
            }
        }
        
        public void SetCamera(int index, bool use)
        {
            useCameras[index] = use;
            SetCheckBox(index, use);
        }
        public bool GetCamera(int index)
        {
            if(index < useCameras.Length)
                return useCameras[index];

            return false;
        }

        private void checkBoxOptic_CheckedChanged(object sender, EventArgs e)
        {
            foreach (var each in checkboxes)
                (each as CheckBox).Checked = checkBoxOptic.Checked;
        }

        private void checkBox_CheckedChanged(object sender, EventArgs e)
        {
            for (int i = 0; i < checkboxes.Length; i++)
                useCameras[i] = (checkboxes[i] as CheckBox).Checked;
        }
        private void SetCheckBox(int index, bool value)
        {
            if(index < checkboxes.Length)
                (checkboxes[index] as CheckBox).Checked = value;
        }
    }
}
