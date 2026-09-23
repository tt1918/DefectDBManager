using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace MarkCompare.Summery
{
    public class SummeryInfo
    {
        public bool IsMarkingError { get; set; } = false;
        public bool IsAiProcError { get; set; } = false;
        public string Info { get; set; } = string.Empty;
    }

    public class SummeryInfoList
    {
        public SummeryInfo this[int index]
        {
            get
            {
                if (index < 0 || index >= Infos.Count)
                {
                    throw new IndexOutOfRangeException("Index is out of range.");
                }
                return Infos[index];
            }
        }
        public List<SummeryInfo> Infos { get; set; } = new List<SummeryInfo>();
    }
}
