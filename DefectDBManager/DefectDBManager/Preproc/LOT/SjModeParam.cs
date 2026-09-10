using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager.Preproc
{
    public class SjModelItem : ICloneable
    {
        public string SECFLTID { get; set; } = string.Empty;
        public bool Use { get; set; } = false;
        public double Rate { get; set; } = 100.0;
        public List<string> FLTID { get; set; } = new List<string>();
        public object Clone()
        {
            SjModelItem clone = new SjModelItem
            {
                FLTID = new List<string>(this.FLTID),
                Use = this.Use,
                SECFLTID = this.SECFLTID,
                Rate = this.Rate
            };

            return clone;
        }
    }

    public class SjModeIPath : ICloneable
    {
        public string Name { get; set; } = string.Empty;
        public string LNCD { get; set; } = string.Empty;
        public string ModelName { get; set; } = string.Empty;
        public string MainIP { get; set; } = string.Empty;
        public string MainFolderName { get; set; } = string.Empty;
        public string SubFolderName { get; set; } = string.Empty;

        public string MainPath
        {
            get
            {
                return $"{MainIP}\\{MainFolderName}";
            }
        }
        public string NetPathSummery(string ctlno)
        {
            return $"{MainIP}\\{MainFolderName}\\{ctlno}\\{SubFolderName}";
        }

        public List<SjModelItem> DefectInfo { get; set; } = new List<SjModelItem>();
        public object Clone()
        {
            SjModeIPath clone = new SjModeIPath
            {
                Name = this.Name,
                LNCD = this.LNCD,
                ModelName = this.ModelName,
                MainIP = this.MainIP,
                MainFolderName = this.MainFolderName,
                SubFolderName = this.SubFolderName
            };
            clone.DefectInfo = new List<SjModelItem>();
            foreach (var defect in this.DefectInfo)
            {
                clone.DefectInfo.Add((SjModelItem)defect.Clone());
            }
            return clone;
        }
    }

    public class SjModeParam : ICloneable
    {
        public int CycleTime { get; set; } = 1000;
        public List<SjModeIPath> ModeItems { get; set; } = new List<SjModeIPath>();

        public SjModeParam()
        {
        }

        public void Save()
        {
            string path = Define.SjModeParamPath;
            string jsonString = Newtonsoft.Json.JsonConvert.SerializeObject(this, Newtonsoft.Json.Formatting.Indented);
            System.IO.File.WriteAllText(path, jsonString);
        }

        public bool Load()
        {
            string path = Define.SjModeParamPath;
            string jsonString = "";

            if(this.ModeItems!=null) this.ModeItems.Clear();
            else this.ModeItems = new List<SjModeIPath>();

            if (System.IO.File.Exists(path)) jsonString = System.IO.File.ReadAllText(path);
            else return false;

            var obj = Newtonsoft.Json.JsonConvert.DeserializeObject<SjModeParam>(jsonString);

            this.CycleTime = obj.CycleTime;
            foreach (var item in obj.ModeItems)
                this.ModeItems.Add(item);

            return true;
        }

        public object Clone()
        {
            SjModeParam clone = (SjModeParam)this.MemberwiseClone();
            this.CycleTime = clone.CycleTime;
            clone.ModeItems = new List<SjModeIPath>();
            foreach (var modeItem in this.ModeItems)
            {
                clone.ModeItems.Add((SjModeIPath)modeItem.Clone());
            }
            return clone;
        }
    }

}
