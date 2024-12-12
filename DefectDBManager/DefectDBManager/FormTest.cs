using DefectDBManager.DB;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DefectDBManager
{
    public partial class FormTest : Form
    {
        public Defects _TemDefects;
        public FormTest()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            _TemDefects.ShowViewer(false);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            _TemDefects.ShowViewer(true);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            _TemDefects.LotChange();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            _TemDefects.SearchLot("LNN0227-11", true, 0, true, true, true);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            _TemDefects.SearchLot("LNN0227-12", true, 0, true, false, true);
            //_TemDefects.SearchCSV("TKN0710-33", "D:\\Project\\TOT\\01. DOC\\Nitto_CSV_샘플\\Nitto_CSV\\500mm,1000mm마다마킹_PPCD별구분\\T7F0408-07C(100)-1.csv", true, 0, true, true, false);
        }

        private void button6_Click(object sender, EventArgs e)
        {
            _TemDefects.SearchModel("LQN1031-03");
        }

        private void button7_Click(object sender, EventArgs e)
        {
        }

        private void button9_Click(object sender, EventArgs e)
        {

        }

        private void button8_Click(object sender, EventArgs e)
        {
            PTRY0PData opd1 = new PTRY0PData();
            opd1.Y0KYCD = "070";
            opd1.Y0KLOT = "LHP0704-01";
            opd1.Y0LNSN = "LH";
            opd1.Y0KKOL = "20230704064800";
            opd1.Y0KSOL = "20230704082100";
            opd1.Y0ZKNM = "NSPZ10DZQ1473P-XJ1UHC3-LW";
            opd1.LNCD = "L013";
            opd1.Y0KASS = "1354";

            _TemDefects.DBManager._DbProc[0]._DbResult.PTRY0P[0].Add(opd1);

            PTRY0PData opd2 = new PTRY0PData();
            opd2.Y0KYCD = "070";
            opd2.Y0KLOT = "T8P0611-10";
            opd2.Y0LNSN = "T8";
            opd2.Y0KKOL = "20230611081200";
            opd2.Y0KSOL = "20230611092100";
            opd2.Y0ZKNM = "NSPZ10-DZQ1473P-XJ1UHC3-LW";
            opd2.LNCD = "T008";
            opd2.Y0KASS = "1375";

            _TemDefects.DBManager._DbProc[0]._DbResult.PTRY0P[1].Add(opd2);

            PTRY0PData opd3 = new PTRY0PData();
            opd3.Y0KYCD = "070";
            opd3.Y0KLOT = "TNP0609-02";
            opd3.Y0LNSN = "TN";
            opd3.Y0KKOL = "20230609053100";
            opd3.Y0KSOL = "20230609064400";
            opd3.Y0ZKNM = "NSPZ10-DZQ5473P-UHC3<PS";
            opd3.LNCD = "T023";
            opd3.Y0KASS = "1377";
            _TemDefects.DBManager._DbProc[0]._DbResult.PTRY0P[1].Add(opd3);

            PTRY0PData opd4 = new PTRY0PData();
            opd4.Y0KYCD = "070";
            opd4.Y0KLOT = "EDP0615-07";
            opd4.Y0LNSN = "ED";
            opd4.Y0KKOL = "20230615083900";
            opd4.Y0KSOL = "20230615100500";
            opd4.Y0ZKNM = "NSPZ10-DZQ1473P-XJ1UHC3-LW";
            opd4.LNCD = "ES02";
            opd4.Y0KASS = "1369";
            _TemDefects.DBManager._DbProc[0]._DbResult.PTRY0P[1].Add(opd4);

            PTRY0PData opd5 = new PTRY0PData();
            opd5.Y0KYCD = "020";
            opd5.Y0KLOT = "U1P0513-002";
            opd5.Y0LNSN = "A1";
            opd5.Y0KKOL = "20230513055700";
            opd5.Y0KSOL = "20230513111600";
            opd5.Y0ZKNM = "PSR 50 W 0-96(1350X1(6FP1450";
            opd5.LNCD = "T131";
            opd5.Y0KASS = "6243";
            _TemDefects.DBManager._DbProc[0]._DbResult.PTRY0P[2].Add(opd5);

            PTRY0PData opd6 = new PTRY0PData();
            opd6.Y0KYCD = "070";
            opd6.Y0KLOT = "S4P0524-02";
            opd6.Y0LNSN = "S4";
            opd6.Y0KKOL = "20230524084700";
            opd6.Y0KSOL = "20230524135900";
            opd6.Y0ZKNM = "PSZ 50 35-85(1260";
            opd6.LNCD = "S004";
            opd6.Y0KASS = "6605";
            _TemDefects.DBManager._DbProc[0]._DbResult.PTRY0P[2].Add(opd6);

            PTRY0PData opd7 = new PTRY0PData();
            opd7.Y0KYCD = "070";
            opd7.Y0KLOT = "D3P0602-07";
            opd7.Y0LNSN = "D3";
            opd7.Y0KKOL = "20230602180700";
            opd7.Y0KSOL = "20230602201000";
            opd7.Y0ZKNM = "DNZZ5473P-UHC3-098B(SL(1270X1-1230-WET-T";
            opd7.LNCD = "S101";
            opd7.Y0KASS = "1404";
            _TemDefects.DBManager._DbProc[0]._DbResult.PTRY0P[2].Add(opd7);

            PTRY0PData opd8 = new PTRY0PData();
            opd8.Y0KYCD = "070";
            opd8.Y0KLOT = "E6P0529-06";
            opd8.Y0LNSN = "E6";
            opd8.Y0KKOL = "20230529115400";
            opd8.Y0KSOL = "20230529141700";
            opd8.Y0ZKNM = "DNZZ5473P-UHC3-098B";
            opd8.LNCD = "E106";
            opd8.Y0KASS = "1415";
            _TemDefects.DBManager._DbProc[0]._DbResult.PTRY0P[2].Add(opd8);

            _TemDefects.DBManager._DbProc[0].SearchMRKCTLMST("LHP0704-01");
        }
    }
}
