using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MarkrCompare
{
    public enum CompProc
    {
        None    = 0,
        Proc,
        Stop,
        End
    }

    public enum eLanguage
    {
        [Description("한국어")]
        Korean = 0,
        [Description("English")]
        English,
        [Description("日本語")]
        Japanese,
        [Description("中文")]
        Chinese,
        [Description("台語")]
        TaiwaneseHokkien
    }
}
