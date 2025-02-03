using Coss.Controls;
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
using static System.Windows.Forms.VisualStyles.VisualStyleElement.TaskbarClock;

namespace MarkrCompare
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
        #endregion

        #region Event
        //public event MarkrCompare.Delegate.UpdatePrepLot OnUpdatePrepLot;
        #endregion

        #region Create/Destroy
        public FormLotList()
        {
            InitializeComponent();

            _dicFormSummary = new Dictionary<string, List<FormLotSummaryData>>();
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
        }

        private void FormLotList_FormClosing(object sender, FormClosingEventArgs e)
        {

        }
        private void FormLotList_VisibleChanged(object sender, EventArgs e)
        {
            if(this.Visible)
            {
                //for(int i=0; i<flowLayoutPanel1.Controls.Count; i++)
                //{
                //    flowLayoutPanel1.Controls[i].Show();
                //}
            }
        }
        #endregion

        #region Lot Summary Flow Layout Panel 처리
        private void initFlpInfo()
        {
            clearFlpInfo();
        }

        private void clearFlpInfo()
        {
            flowLayoutPanel1.Controls.Clear();
        }
        #endregion

        #region Data Control
        private void clearSummary()
        {
            foreach(var list in _dicFormSummary)
            {
                foreach(var form in list.Value)
                    form.Dispose();
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

        public void AddSummaryData()
        {
            FormLotSummaryData form = new FormLotSummaryData();
            form.TopLevel = false;
            form.Parent = this.flowLayoutPanel1;
            form.Show();
            if (_dicFormSummary.ContainsKey("TEST"))
            {
                _dicFormSummary["TEST"].Add(form);
            }
            else
            {
                _dicFormSummary.Add("TEST", new List<FormLotSummaryData>());

                _dicFormSummary["TEST"].Add(form);
                flowLayoutPanel1.Controls.Add(form);
            }
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
            form.Parent = this.flowLayoutPanel1;
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
                flowLayoutPanel1.Controls.Add(form);
            }
        }

        #endregion

        public void DoubleClickSummaryData(FormLotSummaryData data)
        {
            foreach (var item in _dicFormSummary.Values)
            {
                if (item.Contains(data))
                {
                    ((FormMarkDiff)this.ParentForm).UpdateRollmap(data.LotSummery);
                    break;
                }
            }
        }
    }
}
