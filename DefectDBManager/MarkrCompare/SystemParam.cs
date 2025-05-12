using DefectDBManager;
using DefectDBManager.Preproc;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MarkrCompare
{
    public class SystemParam
    {
        [CategoryAttribute("Param")]
        [DisplayName("Language")]
        [DescriptionAttribute("Display Language")]
        [TypeConverter(typeof(EnumToStringConverter))]
        public eLanguage Language { get { return _language; } set { _language = value; } }
        private eLanguage _language = eLanguage.Korean;

        public SystemParam()
        {

        }

        public bool Load()
        {
            string path = Define.SystemParamPath;
            string jsonString = "";

            if (System.IO.File.Exists(path)) jsonString = System.IO.File.ReadAllText(path);
            else return false;

            var obj = Newtonsoft.Json.JsonConvert.DeserializeObject<SystemParam>(jsonString);

            this._language = obj.Language;

            return true;
        }

        public void Save()
        {
            string path = Define.SystemParamPath;
            string jsonString = Newtonsoft.Json.JsonConvert.SerializeObject(this, Newtonsoft.Json.Formatting.Indented);
            System.IO.File.WriteAllText(path, jsonString);
        }
    }
}
