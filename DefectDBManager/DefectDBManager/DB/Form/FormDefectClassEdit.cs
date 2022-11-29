using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.Entity.Core.Common.CommandTrees.ExpressionBuilder;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace DefectDBManager
{
    public partial class FormDefectClassEdit : Form
    {
        public UserDefectClass _UserDefectClass = null;

        private const int defectRow = 20;
        public FormDefectClassEdit()
        {
            InitializeComponent();
        }

        private void FormDefectClassEdit_Load(object sender, EventArgs e)
        {
            initListViewDefect();
            initListViewFTL();
        }

        private void FormDefectClassEdit_VisibleChanged(object sender, EventArgs e)
        {
            if (this.Visible == true)
            {
                displayDgvDefectClass();
                displayDgvFLT();
            }
            else
            {

            }
        }

        #region Defect List View
        private void initListViewDefect()
        {
            dgvDefectClass.RowCount = Global.MaxEditDefectClassCnt;
            dgvDefectClass.SelectionMode = DataGridViewSelectionMode.CellSelect;

            foreach (DataGridViewColumn column in dgvDefectClass.Columns)
            {
                column.SortMode = DataGridViewColumnSortMode.NotSortable;
            }

            int index = 1;
            foreach (DataGridViewRow rowHeader in dgvDefectClass.Rows)
            {
                rowHeader.HeaderCell.Value = String.Format("{0}", index);
                index++;
            }

            dgvDefectClass.ClearSelection();
        }

        private void clearDgvDefectClass()
        {
            int cnt = System.Enum.GetValues(typeof(eUserDefectClass)).Length;

            for (int i = 0; i < cnt; i++)
            {
                int rowSize = dgvDefectClass.Rows.Count;
                for (int j = 0; j < rowSize; j++)
                    dgvDefectClass.Rows[j].Cells[i].Value = "";
            }
        }

        private void displayDgvDefectClass()
        {
            if (_UserDefectClass == null)
                return;

            clearDgvDefectClass();
            int cnt = System.Enum.GetValues(typeof(eUserDefectClass)).Length;

            for (int i = 0; i < cnt; i++)
            {
                int rowSize = _UserDefectClass.DefectClass[i].Count;
                for(int j=0; j<rowSize; j++)
                    dgvDefectClass.Rows[j].Cells[i].Value = _UserDefectClass.DefectClass[i][j];
            }

        }

        private void updateDgvDefectClass()
        {
            if (_UserDefectClass == null)
                return;

            int cnt = System.Enum.GetValues(typeof(eUserDefectClass)).Length;

            for (int i = 0; i < cnt; i++)
            {
                int rowSize = dgvDefectClass.Rows.Count;
                _UserDefectClass.DefectClass[i].Clear();
                for (int j = 0; j < rowSize; j++)
                {
                    if (dgvDefectClass.Rows[j].Cells[i].Value == null) continue;
                    string data = dgvDefectClass.Rows[j].Cells[i].Value as string;
                    if (data.Length>0)
                    {
                        _UserDefectClass.DefectClass[i].Add(data);
                    }
                }
            }
        }
        #endregion

        #region FTL List View
        private void initListViewFTL()
        {
            dgvFLT.RowCount = 1;
        }

        private void clearDgvFTL()
        {
            int cnt = System.Enum.GetValues(typeof(eUserDefectClass)).Length;
            dgvFLT.Rows[0].Cells[0].Value = _UserDefectClass.UseClassTrans;
            for (int j = 1; j <= cnt; j++)
                dgvFLT.Rows[0].Cells[j].Value = "";
        }

        private void updateDgvFLT()
        {
            int cnt = System.Enum.GetValues(typeof(eUserDefectClass)).Length;

            if (dgvFLT.Rows[0].Cells[0].Value == null)
                _UserDefectClass.UseClassTrans = false;
            else
                _UserDefectClass.UseClassTrans= (bool)(dgvFLT.Rows[0].Cells[0].Value);

            for (int i = 0; i < cnt; i++)
            {
                if(dgvFLT.Rows[0].Cells[i + 1].Value==null)
                {
                    _UserDefectClass.TransFLTID[i] = "";
                }
                else
                {
                    _UserDefectClass.TransFLTID[i] = (string)(dgvFLT.Rows[0].Cells[i + 1].Value);
                }
            }
        }

        private void displayDgvFLT()
        {
            if (_UserDefectClass == null) return;
            
            clearDgvFTL();
            int cnt = System.Enum.GetValues(typeof(eUserDefectClass)).Length;
            try
            {
                dgvFLT.Rows[0].Cells[0].Value = _UserDefectClass.UseClassTrans;

                for(int i=0; i<cnt; i++)
                {
                    dgvFLT.Rows[0].Cells[i + 1].Value = _UserDefectClass.TransFLTID[i];
                }
            }
            finally
            {

            }
        }

        #endregion

        /// <summary>
        /// 중복 확인하여 알람 처리
        /// </summary>
        /// <returns></returns>
        private bool checkDuplicated()
        {
            int cnt = System.Enum.GetValues(typeof(eUserDefectClass)).Length;

            for (int i = 0; i < cnt; i++)
            {
                int rowSize = dgvDefectClass.Rows.Count;
                for (int j = 0; j < rowSize; j++)
                {
                    if (dgvDefectClass.Rows[j].Cells[i].Value == null) continue;
                    else
                    {
                        string text = ((string)(dgvDefectClass.Rows[j].Cells[i].Value)).ToUpper();
                        if (text.Length > 0)
                        {
                            for (int k = 0; k < cnt; k++)
                            {
                                for (int l = 0; l < rowSize; l++)
                                {
                                    if (i == k && j == l) continue;
                                    if (dgvDefectClass.Rows[l].Cells[k].Value == null) continue;
                                    string text1 = ((string)(dgvDefectClass.Rows[l].Cells[k].Value)).ToUpper();
                                    if(text1.Length > 0 && text==text1)
                                    {
                                        MessageBox.Show($"Defect Class[{i+1}][{j+1}]: {text} is the same as Defect Class[{k+1}][{l+1}]: {text} ");
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            return false;
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            if (checkDuplicated() == true) return;

            if(_UserDefectClass!=null)
            {
                updateDgvFLT();
                updateDgvDefectClass();
                _UserDefectClass.Save();
            }
            
            this.Hide();
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        private void dgvDefectClass_RowPrePaint(object sender, DataGridViewRowPrePaintEventArgs e)
        {
            e.PaintCells(e.ClipBounds, DataGridViewPaintParts.All);

            e.PaintHeader(DataGridViewPaintParts.Background 
                | DataGridViewPaintParts.Border 
                | DataGridViewPaintParts.Focus 
                | DataGridViewPaintParts.SelectionBackground
                | DataGridViewPaintParts.ContentForeground);

            e.Handled = true;
        }

        #region Language Update
        public void UpdateLanguage()
        {

        }
        #endregion Language Update

    }
}
