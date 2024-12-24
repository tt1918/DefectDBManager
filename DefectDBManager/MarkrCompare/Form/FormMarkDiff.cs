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
    

    public partial class FormMarkDiff : Form
    {
        #region Param
        DefectDBManager.PreprocLotManager _lotManager = null;
        #endregion

        #region Event
        public event MarkrCompare.Delegate.UpdateEvent OnUpdateLiveLNCDInfo = null;
        public event MarkrCompare.Delegate.UpdateEvent OnUpdateSearchLNCDInfo = null;
        #endregion

        public FormMarkDiff()
        {
            InitializeComponent();
        }

        public FormMarkDiff(DefectDBManager.PreprocLotManager lotManager)
        {
            InitializeComponent();
            _lotManager = lotManager;
        }

        #region Create/Destroy
        private void FormMarkDiff_Load(object sender, EventArgs e)
        {
            initTabSearchSetting();
            initCrtLotForm();
            initRollMapForm();
            initLotListForms();
            
        }

        private void FormMarkDiff_FormClosing(object sender, FormClosingEventArgs e)
        {
            CloseTabSearchSetting();
            CloseLotListForms();
            CloseCrtLotForm();
        }
        #endregion

        #region Tab Serach Setting
        private FormMornitorLive _formMorLive;
        private FormMornitorSearch _formMorSearch;

        private void initTabSearchSetting()
        {
            _formMorLive = new FormMornitorLive(_lotManager);
            _formMorLive.TopLevel = false;

            _formMorSearch = new FormMornitorSearch(_lotManager);
            _formMorSearch.TopLevel = false;

            // Live Tab
            tabSearchSet.TabPages[0].Text = "LIVE";
            tabSearchSet.TabPages[0].Controls.Add(_formMorLive.Controls[0]);
            _formMorLive.Dock = DockStyle.Fill;
            _formMorLive.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            _formMorLive.OnUpdatePrepLncdInfo += initLotListForms;
            OnUpdateLiveLNCDInfo += _formMorLive.UpdateLNCDCtrlData;
            _formMorLive.Show();

            // Search Tab
            tabSearchSet.TabPages[1].Text = "SEARCH";
            tabSearchSet.TabPages[1].Controls.Add(_formMorSearch.Controls[0]);
            _formMorSearch.Dock = DockStyle.Fill;
            _formMorSearch.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            _formMorSearch.OnUpdatePrepLncdInfo += initLotListForms;
            OnUpdateSearchLNCDInfo += _formMorSearch.UpdateLNCDCtrlData;
            _formMorSearch.Show();
        }

        private void CloseTabSearchSetting()
        {
            _formMorLive.OnUpdatePrepLncdInfo -= initLotListForms;
            _formMorSearch.OnUpdatePrepLncdInfo -= initLotListForms;
            OnUpdateLiveLNCDInfo -= _formMorLive.UpdateLNCDCtrlData;
            OnUpdateSearchLNCDInfo -= _formMorSearch.UpdateLNCDCtrlData;
            _formMorLive?.Close();
            _formMorSearch?.Close(); 
        }

        private void tabSearchSet_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch((sender as TabControl).SelectedIndex)
            {
                case 0: // Live Form
                    OnUpdateLiveLNCDInfo?.Invoke();
                    break;

                case 1: // Search Form
                    OnUpdateSearchLNCDInfo?.Invoke();
                    break;
            }
        }
        #endregion


        #region Roll Map Form
        private FormRollMap _rollMapForm;

        private void initRollMapForm()
        {
            _rollMapForm = new FormRollMap();
            _rollMapForm.TopLevel = false;
            _rollMapForm.InitRollMap();

            tableLayoutPanel3.Controls.Add(_rollMapForm.Controls[0], 1, 0);
            _rollMapForm.Dock = DockStyle.Fill;
            _rollMapForm.Show();
        }

        #endregion Roll Map Form


        #region Lot List of Product Line
        private List<FormLotList> _lotListForms = null;

        private void initLotListForms()
        {
            try
            {
                if (_lotListForms != null)
                    CloseLotListForms();

                if (_lotListForms == null)
                    _lotListForms = new List<FormLotList>();

                tabLineList.TabPages.Clear();

                foreach (var item in _lotManager.ProcLNCD.Info)
                {
                    // 사용하지 않으면 탭을 추가하지 않음.
                    if (item.Use == false) continue;

                    FormLotList form = new FormLotList(this, item.Name);
                    form.TopLevel = false;
                    _lotListForms.Add(form);
                    TabPage page = new TabPage();

                    page.Font = new Font(tabLineList.Font, FontStyle.Regular);
                    page.Text = item.Name;
                    page.Controls.Add(form.Controls[0]);
                    tabLineList.TabPages.Add(page);
                    form.WindowState = System.Windows.Forms.FormWindowState.Maximized;
                    form.Dock = DockStyle.Fill;
                    form.OnUpdatePrepLot += _crtLotForm.OnUpdateLot;
                    form.OnUpdatePrepLot += _rollMapForm.OnUpdateLotInfo;
                    form.Show();
                }
            }
            catch
            {

            }
        }

        private void CloseLotListForms()
        {
            for (int i = 0; i < _lotListForms.Count; i++)
            {
                if (_lotListForms[i] == null) continue;
                
                _lotListForms[i].Close();
                _lotListForms[i].OnUpdatePrepLot -= _crtLotForm.OnUpdateLot;
                _lotListForms[i].OnUpdatePrepLot -= _rollMapForm.OnUpdateLotInfo;
            }
            

            _lotListForms.Clear();
        }
        #endregion

        #region Current Lot Info
        private FormCrtLot _crtLotForm = null;

        private void initCrtLotForm()
        {
            try
            {
                CloseCrtLotForm();
                _crtLotForm = new FormCrtLot(this);
                _crtLotForm.TopLevel = false;

                tlpLineData.Controls.Add(_crtLotForm.Controls[0], 0, 1);
                _crtLotForm.Dock = DockStyle.Fill;
                _crtLotForm.WindowState = System.Windows.Forms.FormWindowState.Maximized;
                _crtLotForm.Show();
            }
            catch
            {

            }
        }

        private void CloseCrtLotForm()
        {
            _crtLotForm?.Close();
        }

        #endregion

    }
}
