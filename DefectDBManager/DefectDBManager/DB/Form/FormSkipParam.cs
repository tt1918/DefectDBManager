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
        private readonly string[] MaskHeader = {" ", "X LEFT", "X RIGHT", "Y TOP", "Y BOTTOM" };
        private readonly int[] listMaskWidth = {10, 60, 60, 60, 80 };
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
            initListViewOffsetSkip();
        }

        private void FormSkipParam_VisibleChanged(object sender, EventArgs e)
        {
            if (this.Visible == true)
            {
                initComboDest();
                displayListViewOffsetSkip();
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

        #region Offset Skip

        private void initListViewOffsetSkip()
        {
            for (int i = 0; i < MaskHeader.Length; i++)
                lvOffsetSkip.Columns.Add(MaskHeader[i], listMaskWidth[i]);
        }

        private void displayListViewOffsetSkip()
        {
            if (_Param?.OffsetSkip == null) return;
            try
            {
                lvOffsetSkip.BeginUpdate();
                lvOffsetSkip.Items.Clear();

                foreach (var data in _Param.OffsetSkip)
                {
                    ListViewItem item = new ListViewItem("");
                    item.SubItems.Add($"{data.Left:F3}");
                    item.SubItems.Add($"{data.Right:F3}");
                    item.SubItems.Add($"{data.Top:F3}");
                    item.SubItems.Add($"{data.Bottom:F3}");
                }
            }
            finally
            {
                lvOffsetSkip.EndUpdate();
            }
        }

        private void updateListViewOffsetSkip()
        {
            List<SkipOffsetParam> param = new List<SkipOffsetParam>();

            foreach (ListViewItem item in lvOffsetSkip.Items)
            {
                SkipOffsetParam data = new SkipOffsetParam();
                if (float.TryParse(item.SubItems[1].Text, out float value) == true)
                    data.Left = value;
                else continue;

                if (float.TryParse(item.SubItems[2].Text, out value) == true)
                    data.Right = value;
                else continue;

                if (float.TryParse(item.SubItems[3].Text, out value) == true)
                    data.Top = value;
                else continue;

                if (float.TryParse(item.SubItems[4].Text, out value) == true)
                    data.Bottom = value;
                else continue;

                param.Add(data);
            }

            _Param.OffsetSkip = param;
        }

        private void btnUpdateSkipOffset_Click(object sender, EventArgs e)
        {
            updateListViewOffsetSkip();
        }

        private void btnAddOffset_Click(object sender, EventArgs e)
        {
            ListViewItem item = new ListViewItem();

            item.SubItems.Add(tbStartX.Text);
            item.SubItems.Add(tbEndX.Text);
            item.SubItems.Add(tbStartY.Text);
            item.SubItems.Add(tbEndY.Text);
            lvOffsetSkip.Items.Add(item);
        }

        private void btnDelOffset_Click(object sender, EventArgs e)
        {
            int selIndex = lvOffsetSkip.FocusedItem.Index;
            lvOffsetSkip.Items.RemoveAt(selIndex);
        }
        #endregion Offset Skip

        private void btnClose_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

    }
}
