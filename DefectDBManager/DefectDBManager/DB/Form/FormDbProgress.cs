using DefectDBManager.DB.Form;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Diagnostics.Eventing.Reader;
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
        public eNittoDBProgress _LastProgress;
        public NittoDBProgress _DbProgress;
        public int _DispType = 0; // 0: Full Search, 1: Model Search

        private List<ucProgress> _formProgress = new List<ucProgress>();

        private System.Windows.Forms.Timer timer;
        private System.Windows.Forms.Timer timerClose;

        private FormError formError = null;
        public FormDbProgress()
        {
            InitializeComponent();

            this.FormClosing += Form_Closing;

            panelTitle.MouseDown += lblTitle_MouseDown;
            panelTitle.MouseMove += lblTitle_MouseMove;

            timer = new System.Windows.Forms.Timer();
            timer.Interval = 500;
            timer.Tick += new EventHandler(ProgressCheck);

            timerClose = new System.Windows.Forms.Timer();
            timerClose.Interval = 1000;
            timerClose.Tick += new EventHandler(CheckCloseTime);
            initProcessCtrl();
        }

        private void Form_Closing(object sender, FormClosingEventArgs e)
        {
            timer.Dispose();
            timerClose.Dispose();

            if (_formProgress != null)
            {
                for (int i = 0; i < _formProgress.Count; i++)
                    this._formProgress[i]?.Dispose();
                this._formProgress.Clear();
            }
        }


        #region 프로세스 진행 상태 확인
        int oldListStep = -1;
        int oldProcessStep = -1;
        private void ProgressCheck(object sender, EventArgs e)
        {
            if (_Step == -1 || _Total == -1) return;

            bool isComp = true;

            if (oldListStep != _Step)
            {
                for (int i = 0; i <= (int)_LastProgress; i++)
                    _formProgress[i].Reset();
                oldListStep = _Step;
            }

            for (int i = 0; i <= (int)_LastProgress; i++)
            {
                if (_DbProgress._Progress[i].State != _formProgress[i].State)
                {
                    if (_DbProgress._Progress[i].State == eProcessState.Run)
                    {
                        _formProgress[i].Set();
                    }
                    else if (_DbProgress._Progress[i].State == eProcessState.Complete)
                    {
                        _formProgress[i].Complete();
                    }
                    else if (_DbProgress._Progress[i].State == eProcessState.Error)
                    {
                        if (_formProgress[i].State != eProcessState.Error)
                            _formProgress[i].SetError();
                        timer.Stop();
                        formError = new FormError();
                        formError.Show();
                        formError.Left = this.Right + 2;
                        formError.Top = this.Top + 2;
                        btnClose.Visible = true;
                        return;
                    }
                }
            }

            for (int i = 0; i <= (int)_LastProgress; i++)
            {
                isComp &= _DbProgress._Progress[i].IsComplete();
            }

            // 마지막 스텝이고 전체 완료되면 종료 처리
            if (_Step == _Total - 1 && isComp == true)
            {
                if (_DbProgress.IsError == true)
                {
                    btnClose.Visible = true;
                }
                else
                {
                    timerClose.Start();
                }

                btnClose.Visible = true;
                timer.Stop();
            }
        }
        #endregion

        #region 화면 닫기
        int countdown = 5;
        private void CheckCloseTime(object sender, EventArgs e)
        {
            countdown--;
            this.btnClose.Text = $"COLSE ({countdown}S)";
            if (countdown == 0)
                this.Hide();
        }
        #endregion

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

        private void initProcessCtrl()
        {
            int count = System.Enum.GetValues(typeof(eNittoDBProgress)).Length;
            for (int i = 0; i < count; i++)
            {
                var view = new ucProgress();
                view.Title = ((eNittoDBProgress)i).ToString();
                //view.Progress = _DbProgress._Progress[i];
                _formProgress.Add(view);
                //flpProgress.Controls.Add(view);
            }
        }

        private void setProcessCtrl()
        {
            int count = System.Enum.GetValues(typeof(eNittoDBProgress)).Length;
            flpProgress.Controls.Clear();
            for (int i = 0; i <= (int)_LastProgress; i++)
            {
                flpProgress.Controls.Add(_formProgress[i]);
                _formProgress[i].Progress = _DbProgress._Progress[i];
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
                if (_Unit == null)
                {
                    _Total++;
                    listBoxProcess.Items.Add(_LotName);
                }
                else
                {
                    _Total++;
                    listBoxProcess.Items.Add(_LotName);
                }
            }
            catch (Exception ex)
            {
                string text = $"[Error] Process List Bug :[{ex.Message}]";
                Log.WriteLog(text);
                lblText.Text = text;
                listBoxProcess.EndUpdate();
            }
            finally
            {
                listBoxProcess.EndUpdate();
            }
        }

        private void listBoxProcess_VisibleChanged(object sender, EventArgs e)
        {
            if (Visible == true)
            {
                setProcessCtrl();
                setProcessList();
                btnClose.Visible = false;

                oldListStep = -1;
                oldProcessStep = -1;
                countdown = 5;

                this.btnClose.Text = "CLOSE";
                lblText.Text = "";
                timer.Start();
            }
            else
            {
                timer.Stop();
                timerClose.Stop();
            }
        }


        /// <summary>
        ///  step 확인하여 font bold 처리함
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void listBoxProcess_DrawItem(object sender, DrawItemEventArgs e)
        {
            if (this.Visible == false) return;
            e.DrawBackground();
            if (e.Index <= _Step && _Step != -1)
                e.Graphics.DrawString(listBoxProcess.Items[e.Index].ToString(), new Font("Arial", 10, FontStyle.Bold), Brushes.Black, e.Bounds);
            else
                e.Graphics.DrawString(listBoxProcess.Items[e.Index].ToString(), new Font("Arial", 10, FontStyle.Regular), Brushes.Black, e.Bounds);
            e.DrawFocusRectangle();
        }

        private void btnClose_Click(object sender, EventArgs e)
        {
            if (formError?.Visible == true)
                formError.Close();
            this.Hide();
        }
    }
}
