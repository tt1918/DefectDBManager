using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    public static class Log
    {
        public static string LotLog { get; set; }
        public static void WriteLoadData(string str, int nc, string name, double time)
        {
            string path = Path.Combine(Define.BCRPath, LotLog);
            if(Directory.Exists(path)==false)
                Directory.CreateDirectory(path);
            path = Path.Combine(path, $"{name}_{Define.DBResultName}");

            try
            {
                using (StreamWriter sw = File.AppendText(path))
                {
                    if (time == 0)
                        sw.WriteLine(str);
                    else
                    {
                        sw.Write($"[{time}] ");
                        sw.WriteLine(str);
                    }
                }
            }
            finally
            {
                
            }
        }
    }
}
