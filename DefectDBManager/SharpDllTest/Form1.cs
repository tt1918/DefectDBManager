using DefectDBManager;
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
        DefectDBManager.PreProcCompProcess defectProc = null;
        public Form1()
        {
            InitializeComponent();

            defectProc = new DefectDBManager.PreProcCompProcess(this);

            formDB = new DefectDBManager.FormDB(this);
            formDB.Dock = DockStyle.Fill;
            formDB.Location = new System.Drawing.Point(10, 10);
            formDB.Margin = new System.Windows.Forms.Padding(3,4,3,4);
            formDB.Name = "FormDB";
            formDB.TodayDataBase = defectProc._DBProc[(int)(eDbIdWhen.Now)];
            formDB.TopLevel = false;

            this.MainPanel.Controls.Add(formDB);
            formDB.Show();
        }

        public void SearchTodayProductLot(string lotID)
        {
            defectProc.SearchDailyLot(lotID);
        }

        public List<PointF> GetDefectData(string bcno, float start, float end)
        {
            return defectProc.GetMarkDefectData(bcno, start, end);
        }

    }
}
