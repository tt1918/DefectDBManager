using Coss.Controls;
using DefectDBManager.DBLotInfo;
using DefectDBManager.Preproc;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MarkrCompare
{
    public partial class FormSetting : Form
    {
        #region Param
        public DefectDBManager.Preproc.PreprocSet PreprocSet
        {
            get { return _preprocSet; }
            set { _preprocSet = value; }
        }
        DefectDBManager.Preproc.PreprocSet _preprocSet = null;

        //Dictionary<string, DefectDBManager.Preproc.PreprocItem> dicPreprocItem = null;
        #endregion


        public FormSetting(DefectDBManager.Preproc.PreprocSet preprocSet)
        {
            InitializeComponent();

            lblTitle.MouseDown += lblTitle_MouseDown;
            lblTitle.MouseMove += lblTitle_MouseMove;

            _preprocSet = new DefectDBManager.Preproc.PreprocSet();

            int count = 0;
            foreach (var item in preprocSet.Data)
            {
                _preprocSet.Add(item.Clone());
                count++;
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

        #region Form Control

        private void FormSetting_Load(object sender, EventArgs e)
        {
            initDataList();
            initDgvProcess();
            initDgvCompRange();
            initReferenceProcessCtrl();
            initDgvCompProc();
        }

        private void FormSetting_FormClosing(object sender, FormClosingEventArgs e)
        {

        }

        private void FormSetting_VisibleChanged(object sender, EventArgs e)
        {
            if(this.Visible==true)
            {
                displayDataList();
                copyCompData();
                displayAllDgvCtrl();
            }
        }
        #endregion

        #region Data Grid View 전체 표시
        private void displayAllDgvCtrl()
        {
            displayDgvProcess();
            displayJudgeRange();
            displayDgvCompRange();
            displayReferenceProcessCtrl();
            displayDgvCompProc();
        }

        /// <summary>
        /// 컨트롤의 데이터를 메모리로 업데이트함
        /// </summary>
        private void updateAllDgvCtrl()
        {
            
            updateJudgeRange();
            updateDgvCompRange();
            updateReferenceProcessCtrl();
            updateDgvProcess();
        }
        #endregion

        #region 공통 사용 함수
        private int getValidTaskIdx(string name)
        {
            int selIdx = -1;
            for (int i = 0; i < _preprocSet.Count; i++)
            {
                if (_preprocSet[i].Name == name) selIdx = i;
            }

            return selIdx;
        }
        #endregion


        #region Model Name List
        readonly string[] ListModelHeader = { "No.", "Name" };
        readonly int[] ListModelWidth = { 40, 170 };

        private string _selSetName = "";
        private void initDataList()
        {

            lvSetList.View = View.Details;
            lvSetList.FullRowSelect = true;
            for (int i = 0; i < ListModelHeader.Length; i++)
                lvSetList.Columns.Add(ListModelHeader[i], ListModelWidth[i], HorizontalAlignment.Center);
        }

        private void displayDataList()
        {
            try
            {
                lvSetList.BeginUpdate();
                lvSetList.Items.Clear();
                lblName.Text = _selSetName;
                int count = 0;
                if (_preprocSet == null) return;
                foreach (var data in _preprocSet.Data)
                {
                    ListViewItem item = new ListViewItem($"{++count}");
                    item.SubItems.Add(data.Name);
                    lvSetList.Items.Add(item);
                }
            }
            finally
            {
                lvSetList.EndUpdate();
            }
        }

        private void lvSetList_DoubleClick(object sender, EventArgs e)
        {
            _selSetName = "";
            _selCompName = "";
            _selCompLNCD = "";
            
            if (lvSetList.SelectedItems.Count == 0)    return;

            int index = lvSetList.SelectedItems[0].Index;
            _selSetName = lvSetList.Items[index].SubItems[1].Text;
            lblName.Text = _selSetName;

            // Compare Process 데이터 복사
            copyCompData();

            // 영상 표시
            displayAllDgvCtrl();
        }
        #endregion

        #region Setting Item 관련 Control
        private void btnAdd_Click(object sender, EventArgs e)
        {
            FormAddDel form = new FormAddDel("작업 추가", "이름", "추가", "취소");
            if (form.ShowDialog() != DialogResult.OK) return;

            if (form.DataName == "")
            {
                MessageBox.Show($"이름이 비어있습니다.", "경고");
                return;
            }
            DefectDBManager.Preproc.PreprocItem item = new DefectDBManager.Preproc.PreprocItem(form.DataName);
            _preprocSet.Add(item);

            displayDataList();
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            int index = lvSetList.SelectedItems[0].Index;
            string name = lvSetList.SelectedItems[0].SubItems[1].Text;
            if (MessageBox.Show($"{name} 데이터를 삭제하시겠습니까?", "데이터 삭제하기", MessageBoxButtons.YesNo) == DialogResult.No)
                return;

            _preprocSet.Remove(_selSetName);

            displayDataList();
        }

        private void btnLoad_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show($"데이터를 불러오겠습니까?", "데이터 불러오기", MessageBoxButtons.YesNo) == DialogResult.No)
                return;

            _preprocSet.Load();

            displayDataList();
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show($"현재 내용을 저장하시겠습니까?", "모델 저장하기", MessageBoxButtons.YesNo) == DialogResult.No)
                return;

            _preprocSet.Save();
        }
        #endregion

        #region Bottom Area
        /// <summary>
        /// 세부 파라미터 설정 처리
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnApply_Click(object sender, EventArgs e)
        {
            updateAllDgvCtrl();
        }

        /// <summary>
        /// 저장하지 않고 데이터 종료
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        /// <summary>
        /// 저장하고 데이터 종료
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnOK_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }
        #endregion


        #region Process 이름 추가
        static string[] _strdgvListHeader = { "No", "Line ID", "LNCD" };
        static int[] _dgvListLength = { 50, 100, 100};
        enum eDgvPrcess { No, LineID, LNCD, Total};

        List<ProcessData> _tmpCompProc = new List<ProcessData>();

        private void copyCompData()
        {
            _tmpCompProc = new List<ProcessData>();

            int selIdx = -1;
            for (int i = 0; i < _preprocSet.Count; i++)
                if (_preprocSet[i].Name == _selSetName) selIdx = i;
            if (selIdx == -1) return;

            foreach (var item in _preprocSet[selIdx].Compare)
                _tmpCompProc.Add(item.Clone());

        }

        private void initDgvProcess()
        {
            dgvProcess.SelectionMode = DataGridViewSelectionMode.CellSelect;
            dgvProcess.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
            dgvProcess.AllowUserToAddRows = false;
            dgvProcess.RowHeadersVisible = false;
            dgvProcess.ColumnCount = (int)eDgvPrcess.Total;
            for (int i = 0; i < dgvProcess.ColumnCount; i++)
            {
                dgvProcess.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
                dgvProcess.Columns[i].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
                dgvProcess.Columns[i].Name = _strdgvListHeader[i];
                dgvProcess.Columns[i].Width = _dgvListLength[i];
            }

            dgvProcess.Columns[(int)eDgvPrcess.No].ReadOnly = true;
            
        }

        private void displayDgvProcess()
        {
            int selIdx = getValidTaskIdx(_selSetName);
            try
            {
                dgvProcess.SuspendLayout();
                dgvProcess.Rows.Clear();
                if (selIdx != -1)
                {
                    int idx = 0;
                    foreach (var item in _tmpCompProc)
                    {
                        string[] data = new string[(int)eDgvPrcess.Total];
                        data[(int)eDgvPrcess.No] = Convert.ToString(idx);
                        data[(int)eDgvPrcess.LineID] = item.LineID;
                        data[(int)eDgvPrcess.LNCD] = item.LNCD;

                        dgvProcess.Rows.Add(data);
                        idx++;
                    }
                }
            }
            catch
            {

            }
            finally
            {
                dgvProcess.ResumeLayout();
            }
            
        }

        private void updateDgvProcess()
        {
            try
            {
                // Data Grid View에 포함되어 있는 데이터를 먼저 업데이트 해야함.
                updateDgvCompProc();

                int selIdx = -1;
                for (int i = 0; i < _preprocSet.Count; i++)
                    if (_preprocSet[i].Name == _selSetName) selIdx = i;

                _preprocSet[selIdx].Compare.Clear();

                foreach (var item in _tmpCompProc)
                    _preprocSet[selIdx].Compare.Add(item.Clone());
            }
            catch
            {
                
            }
            finally
            {

            }
        }

        private void addDgvProcess(string lineID, string lncd)
        {
            dgvProcess.SuspendLayout();
            try
            {
                int idx = dgvProcess.Rows.Count;
                string[] data = new string[(int)eDgvPrcess.Total];
                data[(int)eDgvPrcess.No] = Convert.ToString(idx);
                data[(int)eDgvPrcess.LineID] = lineID;
                data[(int)eDgvPrcess.LNCD] = lncd;
                dgvProcess.Rows.Add(data);
                
                //추가 데이터 삽입
                ProcessData process = new ProcessData(lineID, lncd);
                _tmpCompProc.Add(process);

            }
            catch
            {

            }
            finally
            {
                dgvProcess.ResumeLayout();
            }
        }

        private void deleteDgvProcess()
        {
            dgvProcess.SuspendLayout();
            try
            {
                int idx = dgvProcess.SelectedRows[0].Index;
                _tmpCompProc.RemoveAt(idx);

                displayDgvProcess();
                
                clearDgvCompProc();
            }
            catch
            {

            }
            finally
            {
                dgvProcess.ResumeLayout();
            }
        }

        private void dgvProcess_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            try
            {
                if (_selSetName == "") return;
                if (dgvProcess.SelectedCells.Count == 0) return;
                int rowIdx = dgvProcess.SelectedCells[0].RowIndex;
                
                _selCompName = dgvProcess.Rows[rowIdx].Cells[(int)eDgvPrcess.LineID].Value as string;
                _selCompLNCD = dgvProcess.Rows[rowIdx].Cells[(int)eDgvPrcess.LNCD].Value as string;
                displayDgvCompProc();
            }
            catch
            {

            }
        }

        private void dgvProcess_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            int idx = e.RowIndex;

            if (_tmpCompProc.Count <= idx) return;
            _tmpCompProc[idx].LineID = dgvProcess.Rows[idx].Cells[(int)eDgvPrcess.LineID].Value as string;
            _tmpCompProc[idx].LNCD = dgvProcess.Rows[idx].Cells[(int)eDgvPrcess.LNCD].Value as string;
        }

        private void btnAddProc_Click(object sender, EventArgs e)
        {
            int selIdx = -1;
            for (int i = 0; i < _preprocSet.Count; i++)
            {
                if (_preprocSet[i].Name == _selSetName) selIdx = i;
            }

            if (selIdx == -1)
            {
                MessageBox.Show("작업이 설정되지 않았습니다."); return;
            }

            string lineID, lncd;
            FormAddDel form = new FormAddDel("프로세스 추가", "LINE ID", "추가", "취소");
            if (form.ShowDialog() != DialogResult.OK) return;
            if (form.DataName == "")
            {
                MessageBox.Show($"Line ID가 비어있습니다.", "경고");
                return;
            }

            lineID = form.DataName;

            form.ScriptName = "LNCD";
            form.DataName = "";
            if (form.ShowDialog() != DialogResult.OK) return;
            if (form.DataName == "")
            {
                MessageBox.Show($"LNCD가 비어있습니다.", "경고");
                return;
            }

            lncd = form.DataName;

            // 같은 이름이 있는지 확인
            bool isExist = false;
            foreach(var item in _tmpCompProc)
            {
                if(lineID == item.LineID) { isExist =true; break; }
            }

            if(isExist==true)
            {
                MessageBox.Show($"동일한 이름의 Line ID가 존재합니다.", "경고");
                return;
            }

            addDgvProcess(lineID, lncd);
        }

        private void btnDelProc_Click(object sender, EventArgs e)
        {
            int selIdx = -1;
            for (int i = 0; i < _preprocSet.Count; i++)
            {
                if (_preprocSet[i].Name == _selSetName) selIdx = i;
            }

            if (selIdx == -1)
            {
                MessageBox.Show("작업이 설정되지 않았습니다."); return;
            }

            deleteDgvProcess();
        }

        #endregion

        #region Process DataGridView 헤더
        static string[] _strDgvProcDataHeader = { "No", "ID", "Size" };
        static int[] _DgvProcDataLength = { 50, 110, 110 };
        enum eDgvProcData { No, ID, Size, Total };
        #endregion

        #region Compare Process Data Grid Veiw 처리
        string _selCompName = "";
        string _selCompLNCD = "";

        private void initDgvCompProc()
        {
            dgvCompProc.SelectionMode = DataGridViewSelectionMode.CellSelect;
            dgvCompProc.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
            dgvCompProc.AllowUserToAddRows = false;
            dgvCompProc.RowHeadersVisible = false;
            dgvCompProc.ColumnCount = (int)eDgvProcData.Total;
            for (int i = 0; i < dgvCompProc.ColumnCount; i++)
            {
                dgvCompProc.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
                dgvCompProc.Columns[i].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;

                dgvCompProc.Columns[i].Name = _strDgvProcDataHeader[i];
                dgvCompProc.Columns[i].Width = _DgvProcDataLength[i];
            }

            dgvCompProc.Columns[(int)eDgvProcData.No].ReadOnly = true;
        }

        private void displayDgvCompProc()
        {
            try
            {
                int selProcIdx = getValidTaskIdx(_selSetName);
                if (selProcIdx == -1 || _selSetName == "") return;

                lblCompProcLineIDData.Text = _selCompName;
                lblCompLNCDData.Text = _selCompLNCD;

                ProcessData refer = null;

                foreach(var item in _tmpCompProc)
                {
                    if (item.LineID == _selCompName)    refer = item;
                }

                // 데이터가 없으면 리턴
                if (refer == null) return;

                dgvCompProc.Rows.Clear();
                int idx = 0;
                foreach (var item in refer.FltInfos)
                {
                    string[] s = new string[(int)eDgvProcData.Total];
                    s[(int)eDgvProcData.No] = idx.ToString();
                    s[(int)eDgvProcData.ID] = item.ID;
                    s[(int)eDgvProcData.Size] = item.Size.ToString();
                    dgvCompProc.Rows.Add(s);
                    idx++;
                }

                cbCompFltAll.Checked = refer.IsFltAll;
            }
            catch
            {

            }
        }

        private void updateDgvCompProc()
        {
            try
            {
                int selProcIdx = getValidTaskIdx(_selSetName);
                if (selProcIdx == -1) return;

                string name = lblCompProcLineIDData.Text;
                int selIdx = -1;

                for (int i = 0; i < _tmpCompProc.Count; i++)
                {
                    if (_tmpCompProc[i].LineID == name)
                        selIdx = i;
                }

                if (selIdx != -1)
                {
                    List<FltInfo> listInfo = new List<FltInfo>();
                    foreach (DataGridViewRow item in dgvCompProc.Rows)
                    {
                        FltInfo fltInfo = new FltInfo();
                        fltInfo.ID = item.Cells[(int)eDgvProcData.ID].Value as string;
                        fltInfo.Size = (float)Convert.ToDouble(item.Cells[(int)eDgvProcData.Size].Value);
                        listInfo.Add(fltInfo);
                    }
                    _tmpCompProc[selIdx].FltInfos = listInfo;
                    _tmpCompProc[selIdx].SetIsFltAll(cbCompFltAll.Checked);
                }
            }
            catch
            {

            }
        }

        private void btnAddCompFlt_Click(object sender, EventArgs e)
        {
            if (getValidTaskIdx(_selSetName) == -1) return;
            addCompProcData();
        }

        private void btnDelCompFlt_Click(object sender, EventArgs e)
        {
            if (getValidTaskIdx(_selSetName) == -1) return;
            deleteCompProcData();
        }

        private void addCompProcData()
        {
            dgvCompProc.SuspendLayout();
            try
            {
                int idx = dgvCompProc.Rows.Count;
                string[] data = new string[(int)eDgvProcData.Total];
                data[(int)eDgvProcData.No] = Convert.ToString(idx);
                data[(int)eDgvProcData.ID] = "";
                data[(int)eDgvProcData.Size] = "0.0";
                dgvCompProc.Rows.Add(data);
            }
            catch
            {

            }
            finally
            {
                dgvCompProc.ResumeLayout();
            }
        }

        private void deleteCompProcData()
        {
            dgvCompProc.SuspendLayout();
            try
            {
                int idx = dgvCompProc.SelectedRows[0].Index;
                if (idx != 0)
                {
                    dgvCompProc.Rows.RemoveAt(idx);

                    for (int i = 1; i < dgvCompProc.Rows.Count; i++)
                        dgvCompProc.Rows[i].Cells[(int)eDgvPrcess.No].Value = Convert.ToString(i);
                }
            }
            catch
            {

            }
            finally
            {
                dgvCompProc.ResumeLayout();
            }
        }

        private void clearDgvCompProc()
        {
            dgvCompProc.SuspendLayout();
            try
            {
                dgvCompProc.Rows.Clear();
                _selCompName = "";
                _selCompLNCD = "";
                lblCompProcLineIDData.Text = "";
                lblCompLNCDData.Text = "";
            }
            catch
            {

            }
            finally
            {
                dgvCompProc.ResumeLayout();
            }
        }

        #endregion

        #region Reference Process

        private void initReferenceProcessCtrl()
        {
            dgvRefProc.SelectionMode = DataGridViewSelectionMode.CellSelect;
            dgvRefProc.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
            dgvRefProc.AllowUserToAddRows = false;
            dgvRefProc.RowHeadersVisible = false;
            dgvRefProc.ColumnCount = (int)eDgvProcData.Total;
            for (int i = 0; i < dgvRefProc.ColumnCount; i++)
            {
                dgvRefProc.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
                dgvRefProc.Columns[i].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;

                dgvRefProc.Columns[i].Name = _strDgvProcDataHeader[i];
                dgvRefProc.Columns[i].Width = _DgvProcDataLength[i];
            }

            dgvRefProc.Columns[(int)eDgvProcData.No].ReadOnly = true;
        }

        private void displayReferenceProcessCtrl()
        {
            try
            {
                int selProcIdx = getValidTaskIdx(_selSetName);
                if (selProcIdx == -1 || _selSetName == "") return;

                ProcessData refer = _preprocSet[selProcIdx].Reference;
                tbRefProcLineID.Texts = refer.LineID;
                tbRefLNCD.Texts = refer.LNCD;

                dgvRefProc.Rows.Clear();
                int idx = 0;
                foreach (var item in refer.FltInfos)
                {
                    string[] s = new string[(int)eDgvProcData.Total];
                    s[(int)eDgvProcData.No] = idx.ToString();
                    s[(int)eDgvProcData.ID] = item.ID;
                    s[(int)eDgvProcData.Size] = item.Size.ToString();
                    dgvRefProc.Rows.Add(s);
                    idx++;
                }

                cbRefFltAll.Checked = refer.IsFltAll;
            }
            catch
            {

            }
        }

        private void updateReferenceProcessCtrl()
        {
            int selProcIdx = getValidTaskIdx(_selSetName);

            ProcessData refer = _preprocSet[selProcIdx].Reference;
            try
            {
                refer.LineID = tbRefProcLineID.Texts;
                refer.LNCD = tbRefLNCD.Texts;

                List<FltInfo> listInfo = new List<FltInfo>();
                foreach (DataGridViewRow item in dgvRefProc.Rows)
                {
                    FltInfo fltInfo = new FltInfo();
                    fltInfo.ID = item.Cells[(int)eDgvProcData.ID].Value as string;
                    fltInfo.Size = (float)Convert.ToDouble(item.Cells[(int)eDgvProcData.Size].Value);
                    listInfo.Add(fltInfo);
                }

                refer.FltInfos = listInfo;
                refer.SetIsFltAll(cbRefFltAll.Checked);
            }
            catch
            {

            }
            
        }

        private void addRefFlt()
        {
            dgvRefProc.SuspendLayout();
            try
            {
                int idx = dgvRefProc.Rows.Count;
                string[] data = new string[(int)eDgvProcData.Total];
                data[(int)eDgvProcData.No] = Convert.ToString(idx);
                data[(int)eDgvProcData.ID] = "";
                data[(int)eDgvProcData.Size] = "0.0";
                dgvRefProc.Rows.Add(data);
            }
            catch
            {

            }
            finally
            {
                dgvRefProc.ResumeLayout();
            }
        }

        private void deleteRefFlt()
        {
            dgvRefProc.SuspendLayout();
            try
            {
                int idx = dgvRefProc.SelectedRows[0].Index;
                if (idx != 0)
                {
                    dgvRefProc.Rows.RemoveAt(idx);

                    for (int i = 1; i < dgvRefProc.Rows.Count; i++)
                        dgvRefProc.Rows[i].Cells[(int)eDgvPrcess.No].Value = Convert.ToString(i);
                }
            }
            catch
            {

            }
            finally
            {
                dgvRefProc.ResumeLayout();
            }
        }


        private void btnAddRefFlt_Click(object sender, EventArgs e)
        {
            int selProcIdx = getValidTaskIdx(_selSetName);
            if (selProcIdx == -1) return;

            addRefFlt();
        }

        private void btnDelRefFlt_Click(object sender, EventArgs e)
        {
            int selProcIdx = getValidTaskIdx(_selSetName);
            if (selProcIdx == -1) return;

            deleteRefFlt();
        }

        #endregion

        #region Compare Range
        static string[] _strDgvCompRangeHeader = { "No", "Min", "Max", "Rate" };
        static int[] _DgvCompRangeLength = { 50, 70, 70, 70 };
        enum eDgvCompRange { No, Min, Max, Rate, Total };

        private void initDgvCompRange()
        {
            dgvCompRange.SelectionMode = DataGridViewSelectionMode.CellSelect;
            dgvCompRange.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;
            dgvCompRange.AllowUserToAddRows = false;
            dgvCompRange.RowHeadersVisible = false;
            dgvCompRange.ColumnCount = (int)eDgvCompRange.Total;
            for (int i = 0; i < dgvCompRange.ColumnCount; i++)
            {
                dgvCompRange.Columns[i].SortMode = DataGridViewColumnSortMode.NotSortable;
                dgvCompRange.Columns[i].DefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;

                dgvCompRange.Columns[i].Name = _strDgvCompRangeHeader[i];
                dgvCompRange.Columns[i].Width = _DgvCompRangeLength[i];
            }

            dgvCompRange.Columns[(int)eDgvCompRange.No].ReadOnly = true;
        }

        private void displayDgvCompRange()
        {
            int selProcIdx = getValidTaskIdx(_selSetName);

            dgvCompRange.Rows.Clear();

            if (selProcIdx == -1 || _selSetName == "") return;

            List<CompRange> range = _preprocSet[selProcIdx].CompRange;

            string[] sR = new string[(int)eDgvCompRange.Total];
            sR[(int)eDgvCompRange.No] = "R";

            sR[(int)eDgvCompRange.Min] = _preprocSet[selProcIdx].BasicRange.MinRange.ToString();
            sR[(int)eDgvCompRange.Max] = _preprocSet[selProcIdx].BasicRange.MaxRange.ToString();
            sR[(int)eDgvCompRange.Rate] = _preprocSet[selProcIdx].BasicRange.Accuracy.ToString();
            dgvCompRange.Rows.Add(sR);

            int idx = 1;
            foreach(CompRange compRange in range)
            {
                string[] s = new string[(int)eDgvCompRange.Total];
                s[(int)eDgvCompRange.No] = idx.ToString();

                s[(int)eDgvCompRange.Min] = compRange.MinRange.ToString();
                s[(int)eDgvCompRange.Max] = compRange.MaxRange.ToString();
                s[(int)eDgvCompRange.Rate] = compRange.Accuracy.ToString();
                dgvCompRange.Rows.Add(s);

                idx++;
            }
        }

        private void updateDgvCompRange()
        {
            int selProcIdx = getValidTaskIdx(_selSetName);
            if (selProcIdx == -1) return;

            List<CompRange> rangeList = new List<CompRange>();
            for (int i=0; i<dgvCompRange.Rows.Count; i++)
            {
                if (i == 0)
                {
                    CompRange rangeData = _preprocSet[selProcIdx].BasicRange;
                    rangeData.MinRange = (float)Convert.ToDouble(dgvCompRange.Rows[i].Cells[(int)eDgvCompRange.Min].Value);
                    rangeData.MaxRange = (float)Convert.ToDouble(dgvCompRange.Rows[i].Cells[(int)eDgvCompRange.Max].Value);
                    rangeData.Accuracy = (float)Convert.ToDouble(dgvCompRange.Rows[i].Cells[(int)eDgvCompRange.Rate].Value);
                }
                else
                {
                    CompRange rangeData = new CompRange();
                    rangeData.MinRange = (float)Convert.ToDouble(dgvCompRange.Rows[i].Cells[(int)eDgvCompRange.Min].Value);
                    rangeData.MaxRange = (float)Convert.ToDouble(dgvCompRange.Rows[i].Cells[(int)eDgvCompRange.Max].Value);
                    rangeData.Accuracy = (float)Convert.ToDouble(dgvCompRange.Rows[i].Cells[(int)eDgvCompRange.Rate].Value);
                    rangeList.Add(rangeData);   
                }
            }

            _preprocSet[selProcIdx].CompRange = rangeList;
        }

        private void addDgvCompRange()
        {
            dgvCompRange.SuspendLayout();
            try
            {
                int idx = dgvCompRange.Rows.Count;
                string[] data = new string[(int)eDgvCompRange.Total];
                data[(int)eDgvCompRange.No] = Convert.ToString(idx);
                data[(int)eDgvCompRange.Min] = "0.0";
                data[(int)eDgvCompRange.Max] = "0.0";
                data[(int)eDgvCompRange.Rate] = "0.0";
                dgvCompRange.Rows.Add(data);
            }
            catch
            {

            }
            finally
            {
                dgvCompRange.ResumeLayout();
            }
        }

        private void deleteDgvCompRange()
        {
            dgvCompRange.SuspendLayout();
            try
            {
                int idx = dgvCompRange.SelectedRows[0].Index;
                if (idx != 0)
                {
                    dgvCompRange.Rows.RemoveAt(idx);
                    dgvCompRange.Rows[0].Cells[(int)eDgvPrcess.No].Value = "R";

                    for (int i = 1; i < dgvProcess.Rows.Count; i++)
                        dgvCompRange.Rows[i].Cells[(int)eDgvPrcess.No].Value = Convert.ToString(i);
                }
            }
            catch
            {

            }
            finally
            {
                dgvCompRange.ResumeLayout();
            }
        }

        private void btnAddCompRange_Click(object sender, EventArgs e)
        {
            if (getValidTaskIdx(_selSetName) == -1) return;

            addDgvCompRange();
        }

        private void btnDelCompRange_Click(object sender, EventArgs e)
        {
            if (getValidTaskIdx(_selSetName) == -1) return;

            deleteDgvCompRange();
        }

        #endregion

        #region Judge Range

        /// <summary>
        /// Ctrl에 입력되어 있는 정보를 얻어온다.
        /// </summary>
        private void updateJudgeRange()
        {
            int selProcIdx = getValidTaskIdx(_selSetName);
            if (selProcIdx == -1) return;

            try
            {
                _preprocSet[selProcIdx].Judge.X = (float)Convert.ToDouble(tbJudgeRangeX.Texts);
                _preprocSet[selProcIdx].Judge.Y = (float)Convert.ToDouble(tbJudgeRangeY.Texts);
            }
            catch
            {

            }
        }

        /// <summary>
        /// Ctrl에 현재 정보를 업데이트 한다.
        /// </summary>
        private void displayJudgeRange()
        {
            int selProcIdx = getValidTaskIdx(_selSetName);
            if (selProcIdx == -1 || _selSetName == "") return;

            try
            {
                tbJudgeRangeX.Texts = _preprocSet[selProcIdx].Judge.X.ToString();
                tbJudgeRangeY.Texts = _preprocSet[selProcIdx].Judge.Y.ToString();
            }
            catch
            {

            }
        }
        #endregion

    }


}
