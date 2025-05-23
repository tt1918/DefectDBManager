using DefectDBManager;
using DefectDBManager.Preproc;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MarkCompare
{
    internal static class Program
    {
        /// <summary>
        /// 해당 애플리케이션의 주 진입점입니다.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            DefectDBManager.PreprocLotManager lotManager = new DefectDBManager.PreprocLotManager();
            DefectDBManager.CompPreprocDefect markComp = new CompPreprocDefect(null);
            markComp.LotManager = lotManager;

            FormMain main = new FormMain(markComp);

            Application.Run(main);
        }
    }
}
