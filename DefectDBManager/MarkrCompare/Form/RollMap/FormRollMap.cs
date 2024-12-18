using Coss.Controls;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.Button;

namespace MarkrCompare
{
    public partial class FormRollMap : Form
    {
        #region RollMap
        public RollmapMouseClickedHandler MouseClicked;
        public RollmapDefectClickedHandler DefectClicked;
        public RollmapViewAreaChangedHandler ViewAreaChanged;

        public FormRollMap()
        {
            InitializeComponent();
        }


        public void InitRollMap()
        {
            Rollmap.OffsetX = 0;
            Rollmap.OffsetY = 0;
            Rollmap.WholeWidth = 1350;
            Rollmap.WholeHeight = 2000000;
            Rollmap.Init(false, false, false, this.Text == "Load" ? false : true);
            Rollmap.UseVscroll = true;

            Rollmap.AutomapViewOffsetY = 0;

            Rollmap.ShowOsDsPosX = false;
            //표시안해서 상관없음
            Rollmap.OsDsPosXbyEdge = false;

            Rollmap.Frames.OsdsOpacity = 0.5f;

            Rollmap.UseColor2 = false;
            Rollmap.ShowPeriod = true;

            this.Rollmap.MouseDoubleClicked += OnRollMapMouseDoubleClicked;
            this.Rollmap.MouseClicked += OnRollMapMouseClicked;
            this.Rollmap.ViewAreaChanged += OnViewAreaChanged;
        }

        private void OnRollMapMouseDoubleClicked(Point real, Point gdi, MouseButtons btn)
        {

        }

        private void OnRollMapMouseClicked(Point real, Point gdi, MouseButtons btn)
        {
            MouseClicked?.Invoke(real, gdi, btn);
            RollmapDefect defect;

            var defects = Rollmap.GetNearestDefects(real, gdi, true);
            foreach (var d in defects)
                DefectClicked?.Invoke(real, gdi, d.Key, d.Value);

            defect = defects.FirstOrDefault(e => e.Value.Id != -1).Value;
        }

        private void OnViewAreaChanged(object sender, MapViewOptionArgs viewOption, Rectangle rect)
        {
            ViewAreaChanged?.Invoke(this, viewOption, rect);
        }

        #endregion RollMap

        #region Radio Control 
        private void radioButton_CheckedChanged(object sender, EventArgs e)
        {
            System.Windows.Forms.RadioButton rb = sender as System.Windows.Forms.RadioButton;
            if (rb == null) return; //중복호출 방지

            if (rbScale1.Checked)       this.Rollmap.MapViewOption(MapViewOptionArgs.tagMapViewRange.View10M);
            else if (rbScale2.Checked)  this.Rollmap.MapViewOption(MapViewOptionArgs.tagMapViewRange.View100M);
            else if (rbScale3.Checked)  this.Rollmap.MapViewOption(MapViewOptionArgs.tagMapViewRange.View500M);
            else if (rbScale4.Checked)  this.Rollmap.MapViewOption(MapViewOptionArgs.tagMapViewRange.View1000M);
        }
        #endregion
    }
}
