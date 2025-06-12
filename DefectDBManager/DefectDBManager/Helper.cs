using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    public static class Helper
    {
        static public string[] SplitKeyData(string key)
        {
            string[] keyData = key.Split('_');
            if (keyData.Length > 3)
            {
                string[] tmpData = new string[3];
                tmpData[0] = keyData[0];
                tmpData[2] = keyData[keyData.Length - 1];

                StringBuilder sb = new StringBuilder();
                for (int i = 1; i < keyData.Length - 1; i++)
                {
                    sb.Append(keyData[i]);
                    if (i != keyData.Length - 2)
                        sb.Append('_');
                }
                tmpData[1] = sb.ToString();
                keyData = tmpData;
            }

            return keyData;
        }

        static public string ReplaceInvalidPathChar(string path)
        {
            string newPath = path.Replace("*", "@");
            newPath = newPath.Replace("<", "^");
            newPath = newPath.Replace(">", "`");

            return newPath;
        }
    }
}
