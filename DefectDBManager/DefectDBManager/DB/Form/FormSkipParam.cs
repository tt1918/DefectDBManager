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
    public partial class FormSkipParam : Form
    {
        #region const param
        private readonly string[] skipParamName = { "경계 반사", "정투과", "주기성 불량", "크로스 불량", "정반사", "미분반사" };
        #endregion
        public DestConfig Config { get; set; }
        public Param _Param { get; set; }

        private ucSkipParam[] skipParamForm=null;

        public string FWPlace { get; set; }

        public FormSkipParam()
        {
            InitializeComponent();
            initSkipParamForm();
        }

        private void FormSkipParam_Load(object sender, EventArgs e)
        {
        }

        private void FormSkipParam_VisibleChanged(object sender, EventArgs e)
        {
            if (this.Visible == true)
            {
                initComboDest();
            }
        }

        #region Skip Size
        private void initComboDest()
        {
            if (Config == null) return;

            int size = Config.DicDest.Count;
            cbDest.Items.Clear();
            DestConfigUnit u = new DestConfigUnit();
            for (int i = 0; i < size; i++)
            {
                if (Config.GetData(i, ref u) == true)
                {
                    cbDest.Items.Add(u.Title);
                }
            }

            // 키값이 있으면
            if (Config.DicDest.ContainsKey(FWPlace) == true)
            {
                cbDest.SelectedItem = FWPlace;
            }
            displaySkipSize();
        }


        private void initSkipParamForm()
        {
            int count = System.Enum.GetValues(typeof(eOpticClass)).Length;
            skipParamForm = new ucSkipParam[count];

            skipParamForm[0] = ucSkipParam1;
            skipParamForm[1] = ucSkipParam2;
            skipParamForm[2] = ucSkipParam3;
            skipParamForm[3] = ucSkipParam4;
            skipParamForm[4] = ucSkipParam5;
            skipParamForm[5] = ucSkipParam6;

            for (int i = 0; i < count; i++)
                skipParamForm[i].Title = skipParamName[i];
        }

        private void updateSkipSize()
        {
            if (cbDest.SelectedItem == null) return;

            string destName = cbDest.SelectedItem.ToString();
            int count = System.Enum.GetValues(typeof(eOpticClass)).Length;
            if (Config.DicDest.ContainsKey(destName) == true)
            {
                DestConfigUnit unit = Config.DicDest[destName];
                for (int i = 0; i < count; i++)
                    unit._SkipSize[i] = skipParamForm[i]._SkipSize;
            }
        }

        private void displaySkipSize()
        {
            string destName = cbDest.SelectedItem.ToString();
            int count = System.Enum.GetValues(typeof(eOpticClass)).Length;

            if (Config.DicDest.ContainsKey(destName) == true)
            {
                for (int i = 0; i < count; i++)
                    skipParamForm[i]._SkipSize = Config.DicDest[destName]._SkipSize[i];
            }
        }

        private void cbDest_SelectedIndexChanged(object sender, EventArgs e)
        {
            displaySkipSize();
        }

        private void btnUpdateSizeSkip_Click(object sender, EventArgs e)
        {
            updateSkipSize();

        }

        #endregion Skip Size

        private void btnClose_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

    }
}
