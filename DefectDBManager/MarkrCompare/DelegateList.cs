using DefectDBManager;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MarkCompare.Delegate
{
    public delegate void UpdatePrepLncdInfo(DefectDBManager.Preproc.eProc proc);
    public delegate void UpdateEvent();
    public delegate void UpdatePrepLot(PreprocLot lot);
    public delegate void DeleUpdateLanguage(string culture);
    public delegate void UpdateSummaryCheck(object obj);
}
