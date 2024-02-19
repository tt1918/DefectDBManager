using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SharpDllTest
{
    public partial class Form1 : Form
    {
        DefectDBManager.FormDB formDB = null;
        DefectDBManager.Defects defectProc = null;
        public Form1()
        {
            InitializeComponent();

            defectProc = new DefectDBManager.Defects();

            formDB = new DefectDBManager.FormDB(this);
            formDB.Dock = DockStyle.Fill;
            formDB.Location = new System.Drawing.Point(10, 10);
            formDB.Margin = new System.Windows.Forms.Padding(3,4,3,4);
            formDB.Name = "FormDB";
            formDB = defectProc._FormDB_Now;
            formDB.TopLevel = false;

            this.MainPanel.Controls.Add(formDB);
            formDB.Show();
        }
    }
}
