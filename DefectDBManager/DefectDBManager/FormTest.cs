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
    public partial class FormTest : Form
    {
        public Defects _TemDefects;
        public FormTest()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            _TemDefects.ShowViewer(false);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            _TemDefects.ShowViewer(true);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            _TemDefects.LotChange();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            _TemDefects.SearchLot("LNN0227-11", false, 0, true, true, false);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            _TemDefects.SearchLot("TKN0710-33", true, 0, true, true, false);
        }

        private void button6_Click(object sender, EventArgs e)
        {
            _TemDefects.SearchModel("LQN1031-03");
        }

        private void button7_Click(object sender, EventArgs e)
        {
            FLTDAT_WRK3_Query query = new FLTDAT_WRK3_Query();
            query.InspRange = 10;
            query.FromPos[0] = 0;
            query.FromPos[1] = 1000;
            query.ToPos[0] = 1000000;
            query.ToPos[1] = 100000000;
            List<string> CTLNO = new List<string>();

            CTLNO.Add("11111111");
            CTLNO.Add("22222222");
            CTLNO.Add("33333333");

            string strQuery = query.GetQuery(CTLNO);

            MessageBox.Show(strQuery);
        }
    }
}
