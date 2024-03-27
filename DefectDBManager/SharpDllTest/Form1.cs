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

namespace SharpDllTest
{
    public partial class Form1 : Form
    {
        DefectDBManager.FormPreCompDB formDB = null;
        DefectDBManager.PreProcCompProcess defectProc = null;
        public Form1()
        {
            InitializeComponent();

            defectProc = new DefectDBManager.PreProcCompProcess(this);

            formDB = new DefectDBManager.FormPreCompDB(this);
            formDB.Dock = DockStyle.Fill;
            formDB.Location = new System.Drawing.Point(10, 10);
            formDB.Margin = new System.Windows.Forms.Padding(3,4,3,4);
            formDB.Name = "FormDB";

            ///////////////////////////////////////////////////////////////////////
            // 화면은 1개로 동일하게 사용하고 DB 검색 데이터만 연결 처리한다. 
            // 현재 랏인 경우
            formDB.Process = defectProc;
            formDB.PreCompDB = defectProc._DBProc[(int)(eDbIdWhen.Now)];
            formDB.DBConn = defectProc._DbConn;

            defectProc.OnEndTodayProductSearching += formDB.OnUpdateDailyLotInfo;

            // 예약 랏인 경우
            //formDB.TodayDataBase = defectProc._DBProc[(int)(eDbIdWhen.Next)];
            ///////////////////////////////////////////////////////////////////////

            formDB.TopLevel = false;

            // 정보 데이터 검색 완료 후 화면 표시
            defectProc.OnEndSearchingAvailableLot += formDB.OnUpdateAvailableLot;

            this.MainPanel.Controls.Add(formDB);
            formDB.Show();
        }

        ~Form1()
        {
            
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            formDB.Close();
            defectProc.Dispose();
        }


        public void SearchTodayProductLot(string lotID)
        {
            defectProc.SearchDailyLot(lotID);
        }

        public List<MarkingFaultDatum> GetDefectData(string bcno, float start, float end)
        {
            return defectProc.GetMarkDefectData(bcno, start, end);
        }

        public void GetLNCD(DefectDBManager.eFCD fcd)
        {
            List<string> strCode = defectProc.GetLineCodeName(fcd);
        }

    }
}
