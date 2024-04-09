using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.Entity;
using System.Data.Entity.Core.Common.CommandTrees.ExpressionBuilder;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.VisualStyles;
using System.Xml.Linq;
using static System.Net.Mime.MediaTypeNames;
using static System.Windows.Forms.AxHost;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;
using ListView = System.Windows.Forms.ListView;

namespace DefectDBManager
{
    public partial class FormMKCDModel : Form
    {
        #region const param
        readonly string[] Model_Header = { "NAME" };
        readonly int[] listModel_Width = { 500};
        #endregion


        public LogDB _LOG { get; set; }

        private MKCD_MODEL _selModelData;

        public OracleDbConnection   DbConn { get; set; }

        public FormMKCDModel()
        {
            InitializeComponent();


            panelTitle.MouseDown += lblTitle_MouseDown;
            panelTitle.MouseMove += lblTitle_MouseMove;

            initDataGridView();

            _selModelData = null;
        }

        private void FormEditDefect_VisibleChanged(object sender, EventArgs e)
        {
            if (this.Visible == true)
            {
                UpdateLanguage();
                updateDgvModel();
            }
        }

        #region 마우스로 폼 드래그
        private Point mouseDownLocation;
        private void lblTitle_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                this.mouseDownLocation = e.Location;
            }
        }
        private void lblTitle_MouseMove(object sender, MouseEventArgs e)
        {
            if (this.WindowState == FormWindowState.Maximized) return;

            if (e.Button == System.Windows.Forms.MouseButtons.Left)
            {
                this.Left = e.X + this.Left - this.mouseDownLocation.X;
                this.Top = e.Y + this.Top - this.mouseDownLocation.Y;
            }
        }
        #endregion

        private void btnSave_Click(object sender, EventArgs e)
        {
            var cell = dgvModel.SelectedCells;
            string srcName = cell[0].Value as string;

            if(_selModelData==null) return;

            _selModelData.Name = srcName;
            _selModelData.Save();

        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.OK;
            this.Close();
        }

        private void initDataGridView()
        {
            dgvModel.RowCount = 0;
            dgvModel.SelectionMode = DataGridViewSelectionMode.CellSelect;

            foreach (DataGridViewColumn column in dgvModel.Columns)
            {
                column.SortMode = DataGridViewColumnSortMode.NotSortable;
            }

            int index = 1;
            foreach (DataGridViewRow rowHeader in dgvModel.Rows)
            {
                rowHeader.HeaderCell.Value = String.Format("{0}", index);
                index++;
            }

            dgvModel.ClearSelection();
        }


        #region Language Update
        public void UpdateLanguage()
        {
            this.SuspendLayout();
           
            this.ResumeLayout();
        }
        #endregion Language Update

        private void btnLoadDBMCKD_Click(object sender, EventArgs e)
        {
            try
            {
                using (FormDefectItemEditor form = new FormDefectItemEditor())
                {
                    form.Title = "Lot Name";
                    form.EditData = "";

                    if (form.ShowDialog() == DialogResult.OK)
                    {
                        Task task = new Task(loadMKCDFromDB, form.EditData);
                        task.Start();
                    }
                }
            }
            catch (Exception ex)
            {
                Log.Write(ex.Message);
            }
            finally
            {

            }

            return;

            string path = Path.Combine(Define.MKCDModelPath, "Default.txt");

            if (File.Exists(path) != true)
            {
                MessageBox.Show("Default.txt  파일이 존재하지 않습니다.");
                return;
            }

            string[] items;
            string[] subItems;

            MKCD_MODEL mKCD_MODEL = new MKCD_MODEL();
            using (StreamReader sr = new StreamReader(path))
            {
                while (sr.ReadLine() != null)
                {
                    items = sr.ReadLine().Split('\t');
                    subItems = items[2].Split(',');

                    MKCD_Data data = new MKCD_Data();

                    data.FLTID = subItems[1];
                    data.MRKF1 = int.Parse(subItems[3]) == 1 ? true : false;
                    data.SIZE = float.Parse(subItems[5]);
                    mKCD_MODEL.Add(subItems[0], data);
                }

                sr.Close();

                _selModelData = mKCD_MODEL;

                // 화면에 로딩 내용 업데이트
                if (this.InvokeRequired == true)
                {
                    this.Invoke(new MethodInvoker(delegate ()
                    {
                        updateDgvDefect();
                    }));
                }
                else
                {
                    updateDgvDefect();
                }
            }
        }

        private void loadMKCDFromDB(object lotName)
        {
            PreProcCompDB db = new PreProcCompDB(this, DbConn, false);
            string lot = lotName as string;
            if(db.SearchMKCD_Data(lot) ==true)
            {
                MKCD_MODEL mKCD_MODEL = new MKCD_MODEL();
                
                foreach (MRKCTLMSTData data in db._DbResult.MRKCTLMST_Data)
                    mKCD_MODEL.Add(data.LNCD, new MKCD_Data(data));

                _selModelData = mKCD_MODEL;

                // 화면에 로딩 내용 업데이트
                if (this.InvokeRequired == true)
                {
                    this.Invoke(new MethodInvoker(delegate ()
                    {
                        updateDgvDefect();
                    }));
                }
                else
                {
                    updateDgvDefect();
                }
            }
        }

        private void updateDgvModel()
        {
            try
            {
                this.dgvModel.SuspendLayout();
                this.dgvModel.Rows.Clear();

                DirectoryInfo di = new DirectoryInfo(Define.MKCDModelPath);
                
                foreach(FileInfo file in di.GetFiles())
                {
                    string[] data = file.Name.Split('.');
                    if(data[1] == "ini")
                        this.dgvModel.Rows.Add(data[0]);
                }
            }
            catch(Exception ex)
            {
                Log.Write(ex.Message);
            }
            finally
            {
                this.dgvModel.ResumeLayout();
            }
             
        }

        private void updateDgvDefect()
        {
            try
            {
                this.dgvDefect.Rows.Clear();
                this.dgvDefect.SuspendLayout();

                foreach(MKCD_LNCD_Data data in _selModelData.Param.Values)
                {
                    foreach(MKCD_Data item in data.Data.Values)
                    {
                        addItem(data.LNCD, item);
                    }
                }
            }
            catch(Exception ex)
            {
                Log.Write(ex.Message);
            }
            finally
            {
                this.dgvDefect.ResumeLayout();
            }
        }

        private void addItem(string strLNCD, MKCD_Data data)
        {
            string[] strValue = new string[4];
            strValue[0] = data.MRKF1.ToString();

            strValue[1] = $"{strLNCD}";
            strValue[2] = $"{data.FLTID}";
            strValue[3] = $"{data.SIZE}";
            dgvDefect.Rows.Add(strValue);
        }

        private void btnNewModel_Click(object sender, EventArgs e)
        {
            try
            {
                using (FormDefectItemEditor form = new FormDefectItemEditor())
                {
                    form.Title = "New Model";
                    form.EditData = "";

                    if (form.ShowDialog() == DialogResult.OK)
                    {
                        string path = Path.Combine(Define.MKCDModelPath, form.EditData + ".ini");

                        // 이 창은 그냥 모델만 생성함.
                        using (FileStream fs = File.Open(path, FileMode.Create))
                        {
                            if (fs != null)
                                fs.Close();
                        }
                    }
                }
            }
            catch(Exception ex)
            {
                Log.Write(ex.Message);
            }
            finally
            {
                updateDgvModel();
            }
        }

        private void btnRename_Click(object sender, EventArgs e)
        {
            try
            {
                using (FormDefectItemEditor form = new FormDefectItemEditor())
                {
                    form.Title = "Change Model Name";
                    var cell = dgvModel.SelectedCells;
                    string srcName = cell[0].Value as string;
                    form.EditData = srcName;

                    if (form.ShowDialog() == DialogResult.OK)
                    {
                        string srcPath = Path.Combine(Define.MKCDModelPath, srcName + ".ini");
                        string newPath = Path.Combine(Define.MKCDModelPath, form.EditData + ".ini");
                        File.Move(srcPath, newPath);
                    }
                }
            }
            catch(Exception ex)
            {
                Log.Write(ex.Message);
            }
            finally
            {
                updateDgvModel();
            }
        }

        private void btnSaveAs_Click(object sender, EventArgs e)
        {
            try
            {
                using (FormDefectItemEditor form = new FormDefectItemEditor())
                {
                    form.Title = "Save As Model";
                    var cell = dgvModel.SelectedCells;
                    string srcName = cell[0].Value as string;
                    form.EditData = srcName;

                    if (form.ShowDialog() == DialogResult.OK)
                    {
                        string srcPath = Path.Combine(Define.MKCDModelPath, srcName + ".ini");
                        string newPath = Path.Combine(Define.MKCDModelPath, form.EditData + ".ini");
                        File.Copy(srcPath, newPath);   
                    }
                }
            }
            catch (Exception ex)
            {
                Log.Write(ex.Message);
            }
            finally
            {
                updateDgvModel();
            }
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            try
            {
                var cell = dgvModel.SelectedCells;
                string srcName = cell[0].Value as string;

                if (MessageBox.Show($"{srcName}을 삭제하시겠습니까?", "Delete Model", MessageBoxButtons.YesNo)==DialogResult.Yes)
                {
                    string srcPath = Path.Combine(Define.MKCDModelPath, srcName + ".ini");
                    File.Delete(srcPath);
                }
            }
            catch (Exception ex)
            {
                Log.Write(ex.Message);
            }
            finally
            {
                updateDgvModel();
            }
        }

        private void btnLoad_Click(object sender, EventArgs e)
        {
            try
            {
                var cell = dgvModel.SelectedCells;
                string srcName = cell[0].Value as string;

                MKCD_MODEL model = new MKCD_MODEL();
                model.Name = srcName;
                model.Load();

                _selModelData = model;

                updateDgvDefect();

            }
            catch (Exception ex)
            {
                Log.Write(ex.Message);
            }
        }
    }
}
