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
        public static void Write(string str)
        {
            int nDay;

            DateTime time = DateTime.Now;
            string data, fileName;

            nDay = time.Day;
            
            fileName = $"{Define.LogPath}\\Day_{nDay}.txt";
            try
            {
                using (StreamWriter sw = File.AppendText(fileName))
                {
                    data = $"{time.Hour}:{time.Minute}:{time.Second} {str}";
                    sw.WriteLine(data);
                }
            }
            finally
            {
                
            }
        }
    }

    public class LogDB
        {
        public string Lot { get; set; }
        public void WriteLoadData(string str, int nc, string name, double time, bool clear=false)
        {
            string path = Path.Combine(Define.BCRPath, Lot);
            if (Directory.Exists(path) == false)
                Directory.CreateDirectory(path);
            path = Path.Combine(path, $"{name}_{Define.DBResultName}");
            
            if(clear==true)
            {
                File.Delete(path);
            }

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

        public void DeleteFolder(string strLot)
        {
            try
            {
                string path = Path.Combine(Define.BCRPath, strLot);
                if (Directory.Exists(path) == true)
                    Directory.Delete(path, true);
            }
            catch
            {
                Log.Write($"Deleting Folder is failed");
            }
        }

    }
}
