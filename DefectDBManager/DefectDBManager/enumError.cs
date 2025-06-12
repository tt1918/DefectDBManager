using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    public enum eSearchError
    {
        Normal = 0,
        PTRYLPEmpty,
        PTRLYPSearchErr,
        XOFSMSTSearchErr,
        PTRY0PSearchErr,
        INSPDATSearchErr,
        FLTDATSearchErr,
        SpliceExistErr
    }
}
