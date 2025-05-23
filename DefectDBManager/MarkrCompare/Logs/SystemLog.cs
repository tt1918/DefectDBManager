///////////////////////////////////////////////////////////////////////////////////
/// SystemLog.cs
/// 전역 시스템 로그 클래스
///////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using log4net;
using log4net.Config;

namespace MarkCompare
{
    public delegate void DisplayLogHandler(string text, Log.Level level = Log.Level.Info, bool write = true, bool duplicate = false);

    class SystemLog
    {
        public static Log System = new Log("System");
        public static Log Network = new Log("Network");
        public static Log FileServer = new Log("FileServer");
        public static Log Alarm = new Log("Alarm");

        static public DisplayLogHandler DisplaySystemLog;
        static public DisplayLogHandler DisplayNetworkLog;
        static public DisplayLogHandler DisplayFileServerLog;
        static public DisplayLogHandler DisplayAlarmLog;
        static public DisplayLogHandler OnDisplayLogData;
    }
}
