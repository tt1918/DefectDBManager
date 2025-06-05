using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MarkCompare
{
    public partial class FormLotSummery : Form
    {
        enum eShowGroup
        {
            Normal,
            NG,
            All,
            Total
        }

        private eShowGroup _showGroup = eShowGroup.All;

        #region Form
        public FormLotSummery()
        {
            InitializeComponent();
            initLotSummary();
            // 전체 보기를 기본으로 처리함
            rbShowAll.Checked = true;
        }

        private void FormLotSummery_FormClosing(object sender, FormClosingEventArgs e)
        {
            closeLotSummary();
        }

        #endregion


        #region Lot Summary Flow Layout Panel
        /// <summary>
        /// 랏 정보에 대한 요약 데이터 처리
        /// key : 라인 정보로 입력함
        /// value : 각 랏의 정보를 입력함
        /// </summary>
        Dictionary<string, List<FormLotSummaryData>> _dicLotSummary = null;

        private void initLotSummary()
        {
            closeLotSummary();
            _dicLotSummary = new Dictionary<string, List<FormLotSummaryData>>();
        }

        private void closeLotSummary()
        {
            if(_dicLotSummary!=null)
            {
                foreach( var list in _dicLotSummary.Values )
                {
                    foreach(var form in list)
                        form.Dispose();
                    list.Clear();
                }
                _dicLotSummary.Clear();
            }
            
            _dicLotSummary = null;
        }

        public void SetLotSummary(string lncd, List<DefectDBManager.PreprocLot> lotSummary, DefectDBManager.PreprocLotManager lotManager, string filter)
        {
            try
            {
                //flpLotSummary.Controls.Clear();

                foreach (var lot in lotSummary)
                {
                    BeginInvoke(new Action(delegate 
                    {
                        FormLotSummaryData form = new FormLotSummaryData();
                        DefectDBManager.Preproc.PreprocItem procItem = new DefectDBManager.Preproc.PreprocItem();
                        foreach (var item in lotManager.LiveProduct)
                        {
                            var keyData = item.Key.Split('_');
                            for (int i = 0; i < lotManager.ProcSetting.Count; i++)
                            {
                                if (lotManager.ProcSetting[i].Name == keyData[2])
                                    procItem = lotManager.ProcSetting[i];
                            }
                        }
                        form.ProcItem = procItem;
                        form.Filter = filter;
                        form.LotSummery = lot;
                        form.TopLevel = false;
                        form.Show();

                        if(_dicLotSummary==null)
                            _dicLotSummary = new Dictionary<string, List<FormLotSummaryData>>();

                        if (_dicLotSummary.ContainsKey(lncd))
                        {
                            _dicLotSummary[lncd].Add(form);
                            flpLotSummary.Controls.Add(form);
                        }
                        else
                        {
                            _dicLotSummary.Add(lncd, new List<FormLotSummaryData>());
                            _dicLotSummary[lncd].Add(form);
                            flpLotSummary.Controls.Add(form);
                        }
                    }));
                }
            }
            catch
            {

            }
        }

        private void clearAllData()
        {
            try
            {
                closeLotSummary();
                //flpLotSummary.Controls.Clear();
            }
            catch
            {

            }
        }

        private void clearNormalData()
        {
            try
            {
                if (_dicLotSummary != null) 
                {
                    foreach (var item in _dicLotSummary.Values)
                    {
                        foreach (var form in item)
                        {
                            if (!form.IsError)
                                form.Dispose();
                        }
                    }
                }
            }
            catch (Exception e)
            {
                SystemLog.DisplaySystemLog($"[ClearNormalData] - {e.Message}");
            }
        }

        private void clearNGData()
        {
            try
            {
                if (_dicLotSummary != null)
                {
                    foreach (var item in _dicLotSummary.Values)
                    {
                        foreach (var form in item)
                        {
                            if (form.IsError)   form.Dispose();
                        }
                    }
                }
            }
            catch (Exception e)
            {
                SystemLog.DisplaySystemLog($"[ClearNGData] - {e.Message}");
            }
        }

        private void clearData()
        {
            switch(_showGroup)
            {
                case eShowGroup.Normal:
                    clearNormalData();
                    break;

                case eShowGroup.NG:
                    clearNGData();
                    break;

                case eShowGroup.All:    
                    clearAllData(); 
                    break;
            }
        }

        private void showAllData()
        {
            try
            {
                foreach (var item in _dicLotSummary.Values)
                {
                    foreach (var form in item)
                        form.Visible = true;
                }
            }
            catch (Exception e)
            {
                SystemLog.DisplaySystemLog($"[ShowAllData] - {e.Message}");
            }
        }

        private void showNGData()
        {
            try
            {
                foreach (var item in _dicLotSummary.Values)
                {
                    foreach (var form in item)
                    {
                        if (form.IsError)   form.Visible = true;
                        else                form.Visible = false;
                    }
                }
            }
            catch (Exception e)
            {
                SystemLog.DisplaySystemLog($"[ShowNGData] - {e.Message}");
            }
        }

        private void showNormalData()
        {
            try 
            {
                foreach (var item in _dicLotSummary.Values)
                {
                    foreach (var form in item)
                    {
                        if (!form.IsError)
                            form.Visible = true;
                        else
                            form.Visible = false;
                    }
                }
            }
            catch (Exception e)
            {
                SystemLog.DisplaySystemLog($"[ShowNormalData] - {e.Message}");
            }
        }
        public void ShowData()
        {
            if (rbShowNormal.Checked == true)
            { 
                _showGroup = eShowGroup.Normal; 
                showNormalData(); 
            } 
            else if(rbShowNG.Checked == true)
            { 
                _showGroup = eShowGroup.NG; 
                showNGData(); 
            }  
            else if(rbShowAll.Checked==true)
            { 
                _showGroup = eShowGroup.All; 
                showAllData();
            } 
        }
        #endregion

        #region Control
        private void btnClearHistory_Click(object sender, EventArgs e)
        {
            clearData();
        }

        private void rbShow_Click(object sender, EventArgs e)
        {
            ShowData();
        }
        #endregion

        #region 언어 변경
        public void UpdateLanguage(string curtule)
        {
            this.BeginInvoke(new Action(() =>
            {
                string fontName = Functions.GetCultureFontName(curtule);
                Font newFont = new Font(fontName, 9, FontStyle.Bold);

                btnClearHistory.Font = newFont;
                btnClearHistory.Text = Lang.clearHistory;

            }));
        }
        #endregion
    }
}
