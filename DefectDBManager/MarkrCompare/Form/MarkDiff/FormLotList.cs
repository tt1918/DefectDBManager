using Coss.Controls;
using DefectDBManager;
using DefectDBManager.Preproc;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.TaskbarClock;

namespace MarkCompare
{
    public partial class FormLotList : Form
    {
        #region Param
        object _owner = null;
        
        /// <summary>
        /// 요약 정보 할당
        /// </summary>
        public Dictionary<string, List<FormLotSummaryData>> DicFormSummary
        {
            get { return _dicFormSummary; }
        }
        private Dictionary<string, List<FormLotSummaryData>> _dicFormSummary = null;
        private Dictionary<string ,FlowLayoutPanel> _flpSummeryDic = new Dictionary<string, FlowLayoutPanel>();
        eProc _procType = eProc.None;
        #endregion

        #region Event
        //public event MarkrCompare.Delegate.UpdatePrepLot OnUpdatePrepLot;
        #endregion

        #region Create/Destroy
        public FormLotList(eProc proc)
        {
            InitializeComponent();

            _dicFormSummary = new Dictionary<string, List<FormLotSummaryData>>();
            _procType = proc;
        }

        public FormLotList(object owner, string name)
        {
            InitializeComponent();
            _owner = owner;
            Name = name;
        }

        private void FormLotList_Load(object sender, EventArgs e)
        {
            initFlpInfo();
            if (_procType == eProc.Live)    setLiveTapControl();
        }

        private void FormLotList_FormClosing(object sender, FormClosingEventArgs e)
        {

        }
        private void FormLotList_VisibleChanged(object sender, EventArgs e)
        {
            if(this.Visible)
            {

            }
        }
        #endregion

        #region Lot Summary Flow Layout Panel 처리
        
        private void setLiveTapControl()
        {
            FlowLayoutPanel flowPanel = new FlowLayoutPanel();
            flowPanel.FlowDirection = FlowDirection.LeftToRight;
            flowPanel.Dock = DockStyle.Fill;
            flowPanel.AutoScroll = true;
            flowPanel.BackColor = Color.White;
            tpLotSummery.Controls.Add(flowPanel);
            tcLotSummary.TabPages[0].Text = "LINE MORNITOR";
            if (_flpSummeryDic.ContainsKey("ErrorCheck") == false)
                _flpSummeryDic.Add("ErrorCheck", flowPanel);


        }
        
        public void SetTapControl(ProcFilterList filters)
        {
            int count = filters.Count;

            tcLotSummary.Controls.Clear();
            foreach(var panel in _flpSummeryDic)
            {
                FlowLayoutPanel flowPanel = panel.Value;

                foreach (var item in flowPanel.Controls)
                {
                    FormLotSummaryData form = item as FormLotSummaryData;
                    form.Dispose();
                }

                flowPanel.Dispose();
            }
            _flpSummeryDic.Clear();

            for (int i = 0; i < count; i++)
            {
                if (filters.Data[i].IsSkip == true) continue;
                string key = filters.Data[i].ToString();
                TabPage tabPage = new TabPage(key);
                FlowLayoutPanel flowPanel = new FlowLayoutPanel();
                flowPanel.FlowDirection = FlowDirection.LeftToRight;
                flowPanel.Dock = DockStyle.Fill;
                flowPanel.AutoScroll = true;
                flowPanel.BackColor = Color.White;
                tabPage.Controls.Add(flowPanel);

                if (_flpSummeryDic.ContainsKey(key) == false)
                    _flpSummeryDic.Add(key, flowPanel);
                tcLotSummary.TabPages.Add(tabPage);
            }
        }

        public void SetTapControlCsv()
        {
            tcLotSummary.Controls.Clear();
            foreach (var panel in _flpSummeryDic)
            {
                FlowLayoutPanel flowPanel = panel.Value;

                foreach (var item in flowPanel.Controls)
                {
                    FormLotSummaryData form = item as FormLotSummaryData;
                    form.Dispose();
                }

                flowPanel.Dispose();
            }
            _flpSummeryDic.Clear();

            
            string key = "CSV";
            TabPage tabPage = new TabPage(key);
            FlowLayoutPanel flowPanel1 = new FlowLayoutPanel();
            flowPanel1.FlowDirection = FlowDirection.LeftToRight;
            flowPanel1.Dock = DockStyle.Fill;
            flowPanel1.AutoScroll = true;
            flowPanel1.BackColor = Color.White;
            tabPage.Controls.Add(flowPanel1);

            if (_flpSummeryDic.ContainsKey(key) == false)
                _flpSummeryDic.Add(key, flowPanel1);
            tcLotSummary.TabPages.Add(tabPage);
        }

        public void SetTapControlDB()
        {
            tcLotSummary.Controls.Clear();
            foreach (var panel in _flpSummeryDic)
            {
                FlowLayoutPanel flowPanel = panel.Value;

                foreach (var item in flowPanel.Controls)
                {
                    FormLotSummaryData form = item as FormLotSummaryData;
                    form.Dispose();
                }

                flowPanel.Dispose();
            }
            _flpSummeryDic.Clear();


            string key = "LOT INSP";
            TabPage tabPage = new TabPage(key);
            FlowLayoutPanel flowPanel1 = new FlowLayoutPanel();
            flowPanel1.FlowDirection = FlowDirection.LeftToRight;
            flowPanel1.Dock = DockStyle.Fill;
            flowPanel1.AutoScroll = true;
            flowPanel1.BackColor = Color.White;
            tabPage.Controls.Add(flowPanel1);

            if (_flpSummeryDic.ContainsKey(key) == false)
                _flpSummeryDic.Add(key, flowPanel1);
            tcLotSummary.TabPages.Add(tabPage);
        }

        private void initFlpInfo()
        {
            clearFlpInfo();
        }

        private void clearFlpInfo()
        {
            
        }
        #endregion

        #region Data Control
        private void clearSummary()
        {
            foreach(var list in _dicFormSummary)
            {
                foreach (var form in list.Value)
                {
                    form.OnClickSummaryCheck-= SelectLot;
                    form.Dispose();
                }
            }
            _dicFormSummary.Clear();
        }

        public void OnClearSummaryData()
        {
            clearSummary();
            clearFlpInfo();
        }

        public void AddSummaryData(bool[] lots)
        {

        }

        public void AddSummaryData(List<IPreprocLot> lotSummary, PreprocItem procItem, PreprocLotManager lotManager)
        {
            BeginInvoke(new Action(delegate
            {
                foreach (var summary in lotSummary)
                {
                    FormLotSummaryData form = new FormLotSummaryData();
                    form.TopLevel = false;

                    if (_flpSummeryDic.ContainsKey("CSV"))
                        form.Parent = this._flpSummeryDic["CSV"];
                    form.LotManager = lotManager;
                    form.ProcItem = procItem;
                    form.Filter = null;
                    form.LotSummery = summary;
                    form.IsCSV = true;
                    form.ShowCheckbox = true;
                    form.TopLevel = false;
                    form.Show();
                    form.OnClickSummaryCheck += SelectLot;
                    if (_dicFormSummary.ContainsKey("Search"))
                    {
                        _dicFormSummary["Search"].Add(form);
                        if (_flpSummeryDic.ContainsKey("CSV"))
                            this._flpSummeryDic["CSV"].Controls.Add(form);
                    }
                    else
                    {
                        _dicFormSummary.Add("Search", new List<FormLotSummaryData>());

                        _dicFormSummary["Search"].Add(form);
                        if (_flpSummeryDic.ContainsKey("CSV"))
                            this._flpSummeryDic["CSV"].Controls.Add(form);
                    }
                }
                this._flpSummeryDic["CSV"].Show();
            }));
        }

        public void AddSummaryData(List<IPreprocLot> lotSummary, PreprocItem procItem, string filter, PreprocLotManager lotManager)
        {
            BeginInvoke(new Action(delegate
            {
                foreach (var summary in lotSummary)
                {
                    FormLotSummaryData form = new FormLotSummaryData();
                    form.TopLevel = false;
                    
                    if(_flpSummeryDic.ContainsKey(filter))
                        form.Parent = this._flpSummeryDic[filter];
                    form.LotManager = lotManager;
                    form.ProcItem = procItem;
                    form.Filter = filter;
                    form.LotSummery = summary;
                    form.IsCSV = false;
                    form.ShowCheckbox = true;
                    form.TopLevel = false;
                    form.Show();
                    form.OnClickSummaryCheck += SelectLot;
                    if (_dicFormSummary.ContainsKey("Search"))
                    {
                        _dicFormSummary["Search"].Add(form);
                        if (_flpSummeryDic.ContainsKey(filter))
                        {
                            if (summary.CompResult != eCompResult.ProcNg)
                                this._flpSummeryDic[filter].Controls.Add(form);
                            else
                                this._flpSummeryDic[filter].Controls.SetChildIndex(form, 0);
                        }
                    }
                    else
                    {
                        _dicFormSummary.Add("Search", new List<FormLotSummaryData>());

                        _dicFormSummary["Search"].Add(form);
                        if (_flpSummeryDic.ContainsKey(filter))
                            this._flpSummeryDic[filter].Controls.Add(form);
                    }
                }
                this._flpSummeryDic[filter].Show();
            }));
        }

        public void AddSummaryData(List<IPreprocLot> lotSummary, LotSelProcParam procItem, string filter, PreprocLotManager lotManager)
        {
            BeginInvoke(new Action(delegate
            {
                foreach (var summary in lotSummary)
                {
                    FormLotSummaryData form = new FormLotSummaryData();
                    form.TopLevel = false;

                    if (_flpSummeryDic.ContainsKey(filter))
                        form.Parent = this._flpSummeryDic[filter];
                    form.LotManager = lotManager;
                    form.SelParam = procItem;
                    form.Filter = filter;
                    form.LotSummery = summary;
                    form.IsCSV = false;
                    form.ShowCheckbox = true;
                    form.TopLevel = false;
                    form.Show();
                    form.OnClickSummaryCheck += SelectLot;
                    if (_dicFormSummary.ContainsKey("Search"))
                    {
                        _dicFormSummary["Search"].Add(form);
                        if (summary.CompResult != eCompResult.ProcNg)
                            this._flpSummeryDic[filter].Controls.Add(form);
                        else
                            this._flpSummeryDic[filter].Controls.SetChildIndex(form, 0);
                    }
                    else
                    {
                        _dicFormSummary.Add("Search", new List<FormLotSummaryData>());

                        _dicFormSummary["Search"].Add(form);
                        if (_flpSummeryDic.ContainsKey(filter))
                            this._flpSummeryDic[filter].Controls.Add(form);
                    }
                }
                this._flpSummeryDic[filter].Show();
            }));
        }

        public void RemoveSummary(bool[] lots)
        {

        }

        public void RemoveSummary(bool lot)
        {

        }

        public void RemoveSummary(string line, string name)
        {

        }

        public void AddErrorCheckMode(string line, string ip, int duration)
        {
            FormLotSummaryData form = new FormLotSummaryData();
            form.TopLevel = false;
            if (_flpSummeryDic.ContainsKey("ErrorCheck"))
                form.Parent = this._flpSummeryDic["ErrorCheck"];
            form.SetStatusCheck(line, ip, duration);
            form.Show();
            
            if (_dicFormSummary.ContainsKey("ErrorCheck"))
            {
                _dicFormSummary["ErrorCheck"].Add(form);
            }
            else
            {
                _dicFormSummary.Add("ErrorCheck", new List<FormLotSummaryData>());

                _dicFormSummary["ErrorCheck"].Add(form);
                if (_flpSummeryDic.ContainsKey("ErrorCheck"))
                    _flpSummeryDic["ErrorCheck"].Controls.Add(form);
            }
        }

        #endregion

        public void DoubleClickSummaryData(FormLotSummaryData data)
        {
            foreach (var item in _dicFormSummary.Values)
            {
                if (item.Contains(data))
                {
                    switch(_procType)
                    {
                        case eProc.Search:
                            if(data.Filter!=null)
                                ((FormMarkDiff)this.ParentForm).UpdateRollmap(data.LotSummery, data.Filter);
                            else
                                ((FormMarkDiff)this.ParentForm).UpdateRollmapCSV(data.LotSummery);
                            break;

                        case eProc.Live:
                            ((FormMarkDiff)this.ParentForm).UpdateRollmap(data.LotSummery, data.Filter);
                            break;

                        case eProc.Selected:
                            if(data.Filter=="LOT INSP")
                                ((FormMarkDiff)this.ParentForm).UpdateRollmapDB(data.LotSummery);
                            else
                                ((FormMarkDiff)this.ParentForm).UpdateRollmap(data.LotSummery, data.Filter);
                            break;
                    }
                    
                    break;
                }
            }

            MessageBox.Show(Lang.SelectedLotIsLoaded);
        }

        public void AddCsvData()
        {
            BeginInvoke(new Action(() => 
            {

            }));
        }

        public void ShowPanel(bool visible)
        {
            this.panel1.Visible = visible;
        }

        private void btnLoad_Click(object sender, EventArgs e)
        {
            ((FormMarkDiff)this.ParentForm).RemoveAll();
            foreach (var form in _dicFormSummary["Search"])
            {
                if (form.StateCheckbox)
                {
                    ((FormMarkDiff)this.ParentForm).UpdateRollmap(form.LotSummery, form.Filter);
                }
            }

            MessageBox.Show(Lang.SelectedLotIsLoaded);
        }

        #region CheckBox 후처리
        private void SelectLot(object obj)
        {
            FormLotSummaryData form = (FormLotSummaryData)obj;

            bool isCheck = form.StateCheckbox;

            // 선택이 되어있으면 다른 check 박스를 확인해서 체크박스 해제
            if(isCheck==true)
            {
                foreach (var form1 in _dicFormSummary["Search"])
                {
                    if (form.IsCSV == false)
                    {
                        if (form1.StateCheckbox &&
                            form.Filter!=form1.Filter ||
                            (form.LotSummery.LotName != form1.LotSummery.LotName ||
                            form.ProcItem.ToString() != form1.ProcItem.ToString()))
                        {
                            form1.StateCheckbox = false;
                        }
                    }
                    else
                    {
                        if (form1.StateCheckbox &&
                            (form.LotSummery.LotName != form1.LotSummery.LotName))
                            form1.StateCheckbox = false;
                    }
                }
            }
        }

        #endregion

        #region 언어 변경
        public void UpdateLanguage(string culture)
        {

        }
        #endregion
    }
}
