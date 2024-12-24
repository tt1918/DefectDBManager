using DefectDBManager.DB;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.Header;

namespace DefectDBManager.Preproc
{
    #region 불량 정보 및 사이즈
    public class FltInfo
    {
        [Category("Info")]
        [Description("Code")]
        public string ID { get; set; }

        [Category("Info")]
        [Description("Size")]
        public float Size { get; set; }

        public FltInfo()
        {
            ID = "";
            Size = 0.0f;
        }

        public FltInfo(string name)
        {
            ID = name;
            Size = 0.0f;
        }

        public FltInfo Clone()
        {
            FltInfo info = new FltInfo();
            info.ID = ID;
            info.Size = Size;

            return info;
        }
    }
    #endregion

    #region 공정의 불량 필터 정보
    public class ProcessData
    {
        /// <summary>
        /// 프로세스 이름
        /// </summary>
        [Category("Process")]
        [Description("Name")]
        public string Name { get; set; }

        /// <summary>
        /// 프로세스 항목
        /// </summary>
        [Category("Process")]
        [Description("Fault")]
        public List<FltInfo> FltInfos { get; set; }

        /// <summary>
        /// 전체 결점 비교 여부
        /// </summary>
        [Category("Process")]
        [Description("Fault All")]
        public bool IsFltAll { get; private set; }

        public ProcessData()
        {
            IsFltAll = false;
            FltInfos = new List<FltInfo>();
        }

        public ProcessData(string name)
        {
            Name = name;
            IsFltAll = false;
            FltInfos = new List<FltInfo>();
        }

        public ProcessData Clone()
        {
            ProcessData item = new ProcessData();
            
            item.Name = this.Name;
            
            foreach(var info in FltInfos)
                item.FltInfos.Add(info.Clone());

            item.IsFltAll = this.IsFltAll;

            return item;
        }

        public void SetIsFltAll(bool isState)
        {
            IsFltAll = isState;
        }
    }
    #endregion

    #region 결점 비교 영역
    public class CompRange
    {
        [Category("Range")]
        [Description("Min")]
        public float MinRange { get; set; }

        [Category("Range")]
        [Description("Max")]
        public float MaxRange { get; set; }

        [Category("Range")]
        [Description("Max")]
        public float Accuracy { get; set; }

        public CompRange()
        {
            MinRange = 0.0f;
            MaxRange = 20.0f;
            Accuracy = 0.0f;
        }

        public CompRange Clone()
        {
            CompRange item = new CompRange();

            item.MinRange = this.MinRange;
            item.MaxRange = this.MaxRange;
            item.Accuracy = this.Accuracy;

            return item;
        }
    }
    #endregion

    public class JudgeRange
    {
        /// <summary>
        /// mm 단위로 저장
        /// </summary>
        [Category("Range")]
        [Description("X")]
        public float X { get; set; }

        /// <summary>
        /// 변수 저장은 mm 단위/상위 표현은 M 단위
        /// </summary>
        [Category("Range")]
        [Description("Y")]
        public float Y 
        {
            get { return _y; }
            set { _y = value; } 
        }
        [JsonIgnore]
        private float _y;

        public JudgeRange()
        {
            X = 200.0f;
            Y = 1.0f;
        }

        public JudgeRange Clone()
        {
            JudgeRange item = new JudgeRange();
            item.X = this.X;
            item.Y = this.Y;

            return item;
        }
    }

    public class PreprocItem
    {
        [Category("items")]
        [Description("Name")]
        public string Name { get; set; } = "";

        //[Category("items")]
        //[Description("Process")]
        //public List<string> Process { get; set; } = new List<string>();

        [Category("items")]
        [Description("Reference")]
        public ProcessData Reference { get; set; } = new ProcessData();

        [Category("items")]
        [Description("Compare")]
        public List<ProcessData> Compare { get; set; } = new List<ProcessData>();

        #region Compare Range
        [Category("items")]
        [Description("Basic Range")]
        public CompRange BasicRange { get; set; } = new CompRange();

        [Category("items")]
        [Description("Compare Range")]
        public List<CompRange> CompRange { get; set; } = new List<CompRange>();
        #endregion

        [Category("items")]
        [Description("Judge Range")]
        public JudgeRange Judge { get; set; } = new JudgeRange();

        public PreprocItem()
        {
        }

        public PreprocItem(string name)
        {
            Name = name;
        }


        public PreprocItem Clone()
        {
            PreprocItem data = new PreprocItem();

            data.Name = this.Name;

            //foreach(var item in this.Process)
            //    data.Process.Add(item);

            data.Reference = this.Reference.Clone();

            foreach(var item in this.Compare)
                data.Compare.Add(item.Clone());

            foreach (var item in this.CompRange)
                data.CompRange.Add(item.Clone());

            data.Judge = this.Judge.Clone();
            return data;
        }
    }

    public class PreprocSet : ItemList<PreprocItem>
    {
        public PreprocSet()
        {
            _data = new List<PreprocItem>();
        }

        public void Copy(List<PreprocItem> input)
        {
            _data.Clear();
            foreach (PreprocItem lpData in input)
                _data.Add(lpData.Clone());
        }

        public void Copy(PreprocSet input)
        {
            _data.Clear();
            foreach (PreprocItem lpData in input.Data)
                _data.Add(lpData.Clone());
        }

        public void Remove(string name)
        {
            for(int i=0; i<_data.Count; i++)
            {
                if (_data[i].Name==name)
                {
                    _data.RemoveAt(i);
                    break;
                }
            }
        }


        /// <summary>
        /// 데이터 저장
        /// </summary>
        /// <returns></returns>
        public bool Save()
        {
            string path = Define.PreprocSetPath;
            string jsonString = Newtonsoft.Json.JsonConvert.SerializeObject(this, Newtonsoft.Json.Formatting.Indented);
            System.IO.File.WriteAllText(path, jsonString);

            return true;
        }

        /// <summary>
        /// 데이터 읽어오기
        /// </summary>
        /// <returns></returns>
        public bool Load()
        {
            string path = Define.PreprocSetPath;
            string jsonString = "";

            this.Clear();
            if (System.IO.File.Exists(path)) jsonString = System.IO.File.ReadAllText(path);
            else return false;


            var obj = Newtonsoft.Json.JsonConvert.DeserializeObject<PreprocSet>(jsonString);

            foreach(var item in obj.Data)
                this.Add(item);

            return true;
        }
    }
}
