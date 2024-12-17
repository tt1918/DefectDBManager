using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MarkrCompare
{
    public partial class FormMornitorSearch : Form
    {
        #region Param
        DefectDBManager.PreprocLotManager _lotManager = null;
        #endregion

        #region Create/Destroy
        public FormMornitorSearch()
        {
            InitializeComponent();
        }

        public FormMornitorSearch(DefectDBManager.PreprocLotManager lotManager)
        {
            InitializeComponent();
            _lotManager = lotManager;
        }
        private void FormMornitorSearch_Load(object sender, EventArgs e)
        {
            initLNCDCtrl();
        }

        private void FormMornitorSearch_FormClosing(object sender, FormClosingEventArgs e)
        {

        }
        #endregion


        #region 체크 버튼 인식
        List<CheckBox> _lncdCheckBox = null;

        private void initLNCDCtrl()
        {
            if (_lotManager == null) return;

            if (_lncdCheckBox != null)
                _lncdCheckBox.Clear();
            if (_lncdCheckBox == null)
                _lncdCheckBox = new List<CheckBox>();

            int size = _lotManager.ProcLNCD.Info.Count;

            tlLncd.Controls.Clear();
            tlLncd.ColumnStyles.Clear();
            tlLncd.RowStyles.Clear();

            tlLncd.Dock = DockStyle.Fill;

            tlLncd.ColumnCount = size;
            tlLncd.RowCount = 1;

            int index = 0;
            tlLncd.RowStyles.Add(new RowStyle(SizeType.AutoSize));

            foreach (var item in _lotManager.ProcLNCD.Info)
            {
                CheckBox checkBox = new CheckBox();
                checkBox.Text = item.Name;
                checkBox.UseVisualStyleBackColor = true;
                checkBox.Checked = item.Use;
                checkBox.AutoSize = true;

                tlLncd.ColumnStyles.Add(new ColumnStyle(SizeType.AutoSize));
                tlLncd.Controls.Add(checkBox, index, 0);

                checkBox.Dock = DockStyle.Fill;

                _lncdCheckBox.Add(checkBox);
                index++;
            }
        }

        /// <summary>
        /// tlLncd -> _lotManager.ProcLNCD.Info
        /// </summary>
        private void getLNCDCtrlData()
        {
            try
            {
                tlLncd.SuspendLayout();
                foreach (var item in _lncdCheckBox)
                {
                    for (int i = 0; i < _lotManager.ProcLNCD.Info.Count; i++)
                    {
                        if (_lotManager.ProcLNCD.Info[i].Name == item.Text)
                        {
                            _lotManager.ProcLNCD.Info[i].Use = item.Checked;
                            break;
                        }
                    }
                }
            }
            catch
            {

            }
            finally
            {
                tlLncd.ResumeLayout();
            }

        }

        /// <summary>
        /// _lotManager.ProcLNCD.Info -> tlLncd
        /// </summary>
        private void setLNCDCtrlData()
        {
            int ctrlCount = _lncdCheckBox.Count;

            try
            {
                tlLncd.SuspendLayout();
                foreach (var item in _lotManager.ProcLNCD.Info)
                {
                    for (int i = 0; i < ctrlCount; i++)
                    {
                        CheckBox chk = _lncdCheckBox[i];
                        if (chk.Text == item.Name)
                        {
                            chk.Checked = item.Use;
                            break;
                        }
                    }
                }
            }
            catch
            {

            }
            finally
            {
                tlLncd.ResumeLayout();
            }


        }
        #endregion

        private void btnStart_Click(object sender, EventArgs e)
        {
        }

        private void btnStop_Click(object sender, EventArgs e)
        {

        }

        private void btnMaterialFilter_Click(object sender, EventArgs e)
        {

        }
    }
}
