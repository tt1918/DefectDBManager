using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager.Preproc
{
    public class PreprocLNCDInfo : ICloneable
    {
        [Category("Info")]
        [Description("Name")]
        public string Name { get; set; }

        [Category("Info")]
        [Description("LNCD")]
        public string LNCD { get; set; }

        [Category("Info")]
        [Description("Use")]
        public bool Use { get; set; }

        public PreprocLNCDInfo()
        {
            Init();
        }

        public void Init()
        {
            Name = "";
            LNCD = "";
            Use = false;
        }

        public void Set(PreprocLNCDInfo s)
        {
            this.Name = s.Name;
            this.LNCD = s.LNCD;
            this.Use = s.Use;
        }
        
        public object Clone()
        {
            return this.MemberwiseClone();
        }
    }

    /// <summary>
    /// 프록그램에서 공정 검색할 대상 라인 코드 정보
    /// </summary>
    public class PreprocLNCD
    {
        public List<PreprocLNCDInfo> Info 
        {
            get { return _lncd_Info; }
            private set { _lncd_Info = value; } 
        }
        private List<PreprocLNCDInfo> _lncd_Info;

        public PreprocLNCD()
        {
            Info = new List<PreprocLNCDInfo>();
        }

        public bool Load()
        {
            string path = Define.LNCD_InfoPath;
            string jsonString = "";

            if (System.IO.File.Exists(path)) jsonString = System.IO.File.ReadAllText(path);
            else return false;

            var obj = Newtonsoft.Json.JsonConvert.DeserializeObject<PreprocLNCD>(jsonString);

            this._lncd_Info = obj._lncd_Info;

            return true;
        }

        public void Save()
        {
            string path = Define.LNCD_InfoPath;
            string jsonString = Newtonsoft.Json.JsonConvert.SerializeObject(this, Newtonsoft.Json.Formatting.Indented);
            System.IO.File.WriteAllText(path, jsonString);
        }

        public string GetLNCD(string line)
        {
            string code="";
            foreach(var item in Info)
            {
                if (item.Name == line)
                {
                    code = item.LNCD;
                    break;
                }
            }

            return code;
        }
    }
}
