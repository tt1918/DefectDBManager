using log4net;
using System;
using System.Windows.Forms;

namespace MarkCompare
{
    /// <summary>
    /// FormLotSummaryData를 더 큰 크기의 팝업 다이얼로그로 표시하는 Form
    /// </summary>
    public partial class FormLotSummaryDataPopup : Form
    {
        private FormLotSummaryData _summaryDataPopup = null;

        public FormLotSummaryDataPopup()
        {
            InitializeComponent();
        }

        /// <summary>
        /// 원본 FormLotSummaryData의 데이터를 복사하여 팝업에 표시합니다.
        /// 원본 컨트롤은 유지되고, 팝업에는 새로운 인스턴스를 생성합니다.
        /// </summary>
        public void SetData(FormLotSummaryData sourceData)
        {
            if (sourceData == null)
                return;

            try
            {
                // 기존 팝업 콘텐츠 제거
                if (this.panel1.Controls.Count > 0)
                {
                    foreach (Control ctrl in this.panel1.Controls)
                    {
                        ctrl.Dispose();
                    }
                    this.panel1.Controls.Clear();
                }

                // 새로운 FormLotSummaryData 인스턴스 생성
                _summaryDataPopup = new FormLotSummaryData();

                // TopLevel을 false로 설정하여 컨트롤처럼 동작하게 함
                _summaryDataPopup.TopLevel = false;
                _summaryDataPopup.Parent = this.panel1;
                _summaryDataPopup.Dock = DockStyle.Fill;

                // 원본 데이터를 새 인스턴스에 복사
                // 주의: LotManager를 먼저 설정해야 LotSummery setter에서 displaySummary() 호출 시 null 참조 오류 방지

                _summaryDataPopup.LotManager = sourceData.LotManager;
                _summaryDataPopup.Filter = sourceData.Filter;
                _summaryDataPopup.MODE = sourceData.MODE;
                if(sourceData.IsTypeDB==false) _summaryDataPopup.ProcItem = sourceData.ProcItem;
                else                           _summaryDataPopup.SelParam = sourceData.SelParam;
                _summaryDataPopup.RefLNCD = sourceData.RefLNCD;
                _summaryDataPopup.CompLNCD = sourceData.CompLNCD;
                _summaryDataPopup.IsError = sourceData.IsError;
                _summaryDataPopup.ShowCheckbox = sourceData.ShowCheckbox;
                _summaryDataPopup.StateCheckbox = sourceData.StateCheckbox;
                _summaryDataPopup.IsCSV = sourceData.IsCSV;

                // LotSummery는 마지막에 설정 (displaySummary 호출 유발)
                _summaryDataPopup.LotSummery = sourceData.LotSummery;

                // panel1에 추가
                this.panel1.Controls.Add(_summaryDataPopup);
                _summaryDataPopup.Visible = true;

            }
            catch (Exception ex)
            {
                MessageBox.Show($"팝업 데이터 설정 중 오류: {ex.Message}", "오류",
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        protected override void OnShown(EventArgs e)
        {
            base.OnShown(e);

            // 팝업 폼 크기 조정 (충분한 공간 제공)
            // 최소: 기본 크기보다 약 2배
            // 최대: 화면 크기의 80%
            int width = Math.Min(1200, Screen.PrimaryScreen.WorkingArea.Width - 40);
            int height = Math.Min(600, Screen.PrimaryScreen.WorkingArea.Height - 100);

            this.Size = new System.Drawing.Size(width, height);

            // 화면 중앙에 배치
            this.CenterToScreen();
        }

        private void FormLotSummaryDataPopup_FormClosing(object sender, FormClosingEventArgs e)
        {
            // 팝업 콘텐츠 정리
            if (_summaryDataPopup != null)
            {
                this.panel1.Controls.Remove(_summaryDataPopup);
                _summaryDataPopup.Dispose();
                _summaryDataPopup = null;
            }
        }
    }
}
