using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DefectDBManager
{
    static class Program
    {
        /// <summary>
        /// 해당 애플리케이션의 주 진입점입니다.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            Defects _Defects = new Defects();
            FormTest _TestForm = new FormTest();
            _TestForm._TemDefects = _Defects;
            _Defects._FormDB_Now._FormCloseBtnType = false;
            _Defects._FormDB_Next._FormCloseBtnType = false;
            _Defects._FormDB_Now._LangType = 0;
            Application.Run(_TestForm);
        }
    }
}
