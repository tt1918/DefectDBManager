using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager.Preproc
{
    public class AiMonitorItem : ICloneable
    {
        public string Name { get; set; } = string.Empty;

        public List<SjModelInfo> Infos { get; set; } = new List<SjModelInfo>();

        public object Clone()
        {
            AiMonitorItem clone = new AiMonitorItem
            {
                Name = this.Name,
            };
            clone.Infos = new List<SjModelInfo>();
            foreach (var item in this.Infos)
            {
                clone.Infos.Add((SjModelInfo)item.Clone());
            }
            return clone;
        }
    }

    public class AiMonitorParam : ICloneable
    {
        public List<AiMonitorItem> ModeItems { get; set; } = new List<AiMonitorItem>();

        public AiMonitorParam()
        {
        }

        public void Save()
        {
            string path = Define.AiMonitorParamPath;
            string jsonString = Newtonsoft.Json.JsonConvert.SerializeObject(this, Newtonsoft.Json.Formatting.Indented);
            System.IO.File.WriteAllText(path, jsonString);
        }

        public bool Load()
        {
            string path = Define.AiMonitorParamPath;
            string jsonString = "";

            if(this.ModeItems!=null) this.ModeItems.Clear();
            else this.ModeItems = new List<AiMonitorItem>();

            if (System.IO.File.Exists(path)) jsonString = System.IO.File.ReadAllText(path);
            else return false;

            var obj = Newtonsoft.Json.JsonConvert.DeserializeObject<AiMonitorParam>(jsonString);

            foreach (var item in obj.ModeItems)
                this.ModeItems.Add(item);

            return true;
        }

        public object Clone()
        {
            AiMonitorParam clone = (AiMonitorParam)this.MemberwiseClone();
            clone.ModeItems = new List<AiMonitorItem>();
            foreach (var modeItem in this.ModeItems)
            {
                clone.ModeItems.Add((AiMonitorItem)modeItem.Clone());
            }
            return clone;
        }
    }

}
