using DefectDBManager;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MarkrCompare.Delegate
{
    public delegate void UpdatePrepLncdInfo(DefectDBManager.Preproc.eProc proc);
    public delegate void UpdateEvent();
    public delegate void UpdatePrepLot(PreprocLot lot);
}
