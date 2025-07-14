using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager.Preproc
{
    public enum eProc
    {
        None=-1,
        Live,
        Search,
        Total
    }

    public enum eProcDataType
    {
        None,
        Reference,
        Compare,
        Total
    }
}
