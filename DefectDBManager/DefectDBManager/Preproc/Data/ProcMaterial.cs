using DefectDBManager.DB;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager.Preproc
{
    public class ProcMaterial
    {
        [Category("items")]
        [Description("Name")]
        public string Name { get; set; } = string.Empty;

        [Category("items")]
        [Description("Material")]
        public List<string> Items { get; set; } = null;

        [JsonIgnore]
        public string this[int idx]
        {
            get { return Items[idx]; }
            set { Items[idx] = value; }
        }

        public ProcMaterial()
        {
            Items = new List<string>();
        }
        public ProcMaterial(string name)
        {
            Name = name;
            Items = new List<string>();
        }

        public ProcMaterial Clone()
        {
            ProcMaterial material = new ProcMaterial();

            material.Name = this.Name;

            foreach (var item in Items)
                material.Items.Add(item);

            return material;
        }
    }

    public class ProcMaterialList : ItemList<ProcMaterial>
    {
        public ProcMaterialList()
        {
            _data = new List<ProcMaterial>();
        }

        public void Copy(List<ProcMaterial> input)
        {
            _data.Clear();
            foreach (ProcMaterial lpData in input)
                _data.Add(lpData.Clone());
        }

        public void Copy(ProcMaterialList input)
        {
            _data.Clear();
            foreach (ProcMaterial lpData in input.Data)
                _data.Add(lpData.Clone());
        }

        public void Save()
        {
            string path = Define.MaterialListPath;
            string jsonString = Newtonsoft.Json.JsonConvert.SerializeObject(this, Newtonsoft.Json.Formatting.Indented);
            System.IO.File.WriteAllText(path, jsonString);
        }

        public bool Load()
        {
            string path = Define.MaterialListPath;
            string jsonString = "";

            this.Clear();
            if (System.IO.File.Exists(path)) jsonString = System.IO.File.ReadAllText(path);
            else return false;

            var obj = Newtonsoft.Json.JsonConvert.DeserializeObject<ProcMaterialList>(jsonString);

            foreach (var item in obj.Data)
                this.Add(item);

            return true;
        }
    }

}
