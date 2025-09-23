using DefectDBManager;
using DefectDBManager.Preproc;
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
    public delegate void UpdateErrorLots(string[] errLots);
    public delegate void RunSelectedLotComparing(List<string> lots, LotSelProcParam param);

    public delegate void UpdateSelectedLotInfo(List<string> lotInfo);
}
