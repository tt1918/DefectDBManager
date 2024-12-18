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
            initLotListForms();
            initCrtLotForm();
            initRollMapForm();
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
            _formMorLive.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            _formMorLive.Show();

            // Search Tab
            tabSearchSet.TabPages[1].Text = "SEARCH";
            tabSearchSet.TabPages[1].Controls.Add(_formMorSearch.Controls[0]);
            _formMorSearch.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            _formMorSearch.Show();
        }

        private void CloseTabSearchSetting()
        {
            _formMorLive?.Close();
            _formMorSearch?.Close(); 
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

                    page.Text = item.Name;
                    page.Controls.Add(form.Controls[0]);
                    tabLineList.TabPages.Add(page);
                    form.WindowState = System.Windows.Forms.FormWindowState.Maximized;
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
                _lotListForms[i]?.Close();

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
