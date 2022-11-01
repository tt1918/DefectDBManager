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
    public partial class FormDefectItemEditor : Form
    {
        public string EditData {get; set;}

        public FormDefectItemEditor()
        {
            InitializeComponent();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            EditData = tbEditData.Text;

            DialogResult = DialogResult.OK;
            this.Close();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void FormDefectItemEditor_VisibleChanged(object sender, EventArgs e)
        {
            if(Visible==true)
            {
                tbEditData.Text = EditData;
            }
        }
    }
}
