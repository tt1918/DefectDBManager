///////////////////////////////////////////////////////////////////////////////////
/// Log.cs
/// log4net 라이브러리 사용해서 로그 저장
/// 설정은 Logs/LogConfig.xml
///////////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using log4net;
using log4net.Config;
using log4net.Appender;
using System.Reflection;
using DefectDBManager;

namespace MarkCompare
{
    public class Log
    {
        static bool initialized = false;
        ILog log;
        public enum Level
        {
            Debug = 0,
            Info,
            Warn,
            Error,
            Fatal,
        }
        public bool WriteCallerName { get; set; }
        public string Key { get; private set; }
        public string Directory { get; private set; }
        public string DatePattern { get; private set; }
        private static object consoleLock = new object();
        public Log(string key)
        {
            if(!initialized)
            {
                initialized = true;
                XmlConfigurator.Configure(new System.IO.FileInfo(String.Format("{0}/Logs/LogConfig.xml", Define.MainPath)));
            }
            Init(key);
        }

        public void Init(string key)
        {
            this.Key = key;
            this.log = LogManager.GetLogger(key);
            var rootRepository = LogManager.GetRepository();
            foreach (var appender in rootRepository.GetAppenders())
            {
                if (appender.Name.Equals(key + "Log") && appender is RollingFileAppender)
                {
                    var fileAppender = appender as log4net.Appender.RollingFileAppender;
                    Directory = Path.GetDirectoryName(fileAppender.File);
                    DatePattern = fileAppender.DatePattern;
                }
            }
        }

        public void Write(Level level, string text)
        {
            if(this.log != null)
            {
                switch(level)
                {
                    case Level.Debug:
                        this.log.DebugFormat("{0}", text);
                        break;
                    case Level.Info:
                        this.log.InfoFormat("{0}", text);
                        break;
                    case Level.Warn:
                        this.log.WarnFormat("{0}", text);
                        break;
                    case Level.Error:
                        this.log.ErrorFormat("{0}", text);
                        break;
                    case Level.Fatal:
                        this.log.FatalFormat("{0}", text);
                        break;
                    default:
                        break;
                }

                lock(consoleLock)
                {
                    switch (level)
                    {
                        case Level.Debug:
                            Console.ForegroundColor = ConsoleColor.Gray;
                            break;
                        case Level.Info:
                            Console.ForegroundColor = ConsoleColor.Blue;
                            break;
                        case Level.Warn:
                            Console.ForegroundColor = ConsoleColor.White;
                            break;
                        case Level.Error:
                            Console.ForegroundColor = ConsoleColor.Yellow;
                            break;
                        case Level.Fatal:
                            Console.ForegroundColor = ConsoleColor.Red;
                            break;
                        default:
                            break;
                    }

                    Console.WriteLine("{0} {1} {2}\n", Key, level.ToString(), text);
                }
            }
        }
    }
}
