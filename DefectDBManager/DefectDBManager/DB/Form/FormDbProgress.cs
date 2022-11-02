using DefectDBManager.DB.Form;
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
    public partial class FormDbProgress : Form
    {
        public DestConfigUnit _Unit;
        public string _LotName;
        public int _Total;
        public int _Step;
        public NittoDBProgress _DbProgress;
        public int _ProgressStep;
        public FormDbProgress()
        {
            InitializeComponent();

            panelTitle.MouseDown += lblTitle_MouseDown;
            panelTitle.MouseMove += lblTitle_MouseMove;

            _ProgressStep = 0;
//            listBoxProcess.DrawMode = DrawMode.OwnerDrawVariable;

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

        private List<ucProgress> _dbProgress = new List<ucProgress>(); 
        private void setProcessCtrl()
        {
            int count = System.Enum.GetValues(typeof(eNittoDBProgress)).Length;
            for(int i=0; i< count; i++)
            {
                var view = new ucProgress();
                view.Title = ((eNittoDBProgress)i).ToString();
                view.Progress = _DbProgress._Progress[i];
                _dbProgress.Add(view);
                flpProgress.Controls.Add(view);
            }
        }

        private void setProcessList()
        {
            try
            {
                listBoxProcess.BeginUpdate();
                listBoxProcess.Items.Clear();
                _Total = 0;
                _Step = -1;
                if (_Unit.IsSplit==false)
                {
                    _Total++;
                    listBoxProcess.Items.Add(_LotName);
                }
                else
                {
                    if (_Unit.UnitA.IsUse == true)
                    {
                        _Total++;
                        string text = $"{_LotName}_UnitA";
                        listBoxProcess.Items.Add(text);
                    }
                    if(_Unit.UnitB.IsUse == true)
                    {
                        _Total++;
                        string text = $"{_LotName}_UnitB";
                        listBoxProcess.Items.Add(text);
                    }
                    if (_Unit.UnitC.IsUse == true)
                    {
                        _Total++;
                        string text = $"{_LotName}_UnitC";
                        listBoxProcess.Items.Add(text);
                    }
                }
            }
            catch(Exception ex)
            {
                Log.WriteLog($"[Error] Process List Bug :[{ex.Message}]");
                listBoxProcess.EndUpdate();
            }
            finally
            {
                listBoxProcess.EndUpdate();
            }
        }

        private void listBoxProcess_VisibleChanged(object sender, EventArgs e)
        {
            if(Visible==true)
            {
                setProcessCtrl();
                setProcessList();
            }
        }


        /// <summary>
        ///  step 확인하여 font bold 처리함
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void listBoxProcess_DrawItem(object sender, DrawItemEventArgs e)
        {
            e.DrawBackground();
            if (e.Index<= _Step && _Step!=-1)
                e.Graphics.DrawString(listBoxProcess.Items[e.Index].ToString(), new Font("Arial", 10, FontStyle.Bold), Brushes.Black, e.Bounds);
            else
                e.Graphics.DrawString(listBoxProcess.Items[e.Index].ToString(), new Font("Arial", 10, FontStyle.Regular), Brushes.Black, e.Bounds);
            e.DrawFocusRectangle();
        }

        private void listBoxProcess_MeasureItem(object sender, MeasureItemEventArgs e)
        {
            e.ItemHeight = 18;
        }

        public void Start(int step)
        {
            if (step >= _dbProgress.Count)
                return;

            _dbProgress[step].Set();
        }

        public void SetError(int step)
        {
            if (step >= _dbProgress.Count)
                return;

            _dbProgress[step].SetError();
        }
    }
}
