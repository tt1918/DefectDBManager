using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    public class Define
    {
        public static string MainPath { get { return "C:\\COSS"; } }
        public static string UserDefectClassPath { get { return Path.Combine(MainPath, "UserDefectClass.Ini"); } }
        public static string BCRPath { get { return Path.Combine(MainPath, "Barcode"); } }
        public static string DestPath { get { return Path.Combine(MainPath, "Dest.Ini"); } }
        public static string DbInfoPath { get { return Path.Combine(MainPath, "DBInfo.Ini"); } }
        public static string DBResultName { get { return "DBResult.txt"; } }

        public static string LogPath { get { return Path.Combine(MainPath, "SystemLog"); } }
    }

    public class Global
    {
        public const int MaxDefectLine = 1000;
        public const int LotNameLength = 10;
        public const int MaxDestItemCnt = 100;
        public const int MaxEditDefectClassCnt = 20;
    }
}
