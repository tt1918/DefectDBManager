using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MarkCompare
{
    public static class Functions
    {
        public static void ChangeFont(Control parent, Font font)
        {
            parent.Font = font;
            foreach (Control ctrl in parent.Controls)
            {
                ChangeFont(ctrl, font);
            }
        }

        public static string GetCultureFontName(string curtule)
        {
            string name = "";
            switch (curtule)
            {
                case "Ko-Kr":   name = "Segoe UI";              break;
                case "en-US":   name = "Segoe UI";              break;
                case "ja-JP":   name = "Yu Gothic UI";          break;
                case "zh-CN":   name = "Microsoft YaHei UI";    break;
                case "zh-TW":   name = "Microsoft JhengHei UI"; break;
                default:        name = "Segoe UI";              break;
            }

            return name;
        }

        public static string GetVersion()
        {
            string version = null;
            var name = System.Reflection.Assembly.GetExecutingAssembly().GetName();
            string path = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, $"{name.Name}.exe");
            var info = new System.IO.FileInfo(path);
            version = $"Ver.{info.LastWriteTime.ToString("yyMMddHH")}";

            return version;
        }
    }
}
