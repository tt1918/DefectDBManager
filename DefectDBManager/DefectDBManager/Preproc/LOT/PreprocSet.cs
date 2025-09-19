using DefectDBManager.DB;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.ComponentModel;


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
        [Description("Line ID")]
        public string LineID { get; set; }

        /// <summary>
        /// 라인코드
        /// </summary>
        [Category("Process")]
        [Description("LNCD")]
        public string LNCD { get; set; }

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
        public bool IsFltAll { get; set; }

        [Category("Process")]
        [Description("Split CTLNO")]
        public bool IsSplitCTLNO { get; set; }

        public ProcessData()
        {
            IsFltAll = false;
            IsSplitCTLNO = false;
            FltInfos = new List<FltInfo>();
        }

        public ProcessData(string lineID="", string lncd="")
        {
            LineID = lineID;
            LNCD = lncd;
            IsFltAll = false;
            IsSplitCTLNO = false;
            FltInfos = new List<FltInfo>();
        }

        public ProcessData Clone()
        {
            ProcessData item = new ProcessData();
            
            item.LineID = this.LineID;
            item.LNCD = this.LNCD;
            
            foreach(var info in FltInfos)
                item.FltInfos.Add(info.Clone());

            item.IsFltAll = this.IsFltAll;
            item.IsSplitCTLNO = this.IsSplitCTLNO;

            return item;
        }

        public bool IsValidFLTID(string key, float size)
        {
            if (IsFltAll) return true;
            foreach (FltInfo info in FltInfos)
            {
                if (info.ID == key)
                {
                    // 사이즈 만족하면 TRUE, 그렇지 않으면  FALSE
                    if (info.Size <= size)  return true;
                    else                    return false;   
                }
            }
            return false;
        }
    }
    #endregion

    #region 결점 비교 영역
    public class CompRange
    {
        [Category("Range")]
        [Description("MinX")]
        public float MinXRange { get; set; }

        [Category("Range")]
        [Description("MaxX")]
        public float MaxXRange { get; set; }
        
        [Category("Range")]
        [Description("MinY")]
        public float MinYRange { get; set; }

        [Category("Range")]
        [Description("MaxY")]
        public float MaxYRange { get; set; }

        [Category("Range")]
        [Description("Max")]
        public float Accuracy { get; set; }

        public CompRange()
        {
            MinXRange = 0.0f;
            MaxXRange = 20.0f;
            MinYRange = 0.0f;
            MaxYRange = 20.0f;
            Accuracy = 0.0f;
        }

        public CompRange Clone()
        {
            CompRange item = new CompRange();

            item.MinXRange = this.MinXRange;
            item.MaxXRange = this.MaxXRange;
            item.MinYRange = this.MinYRange;
            item.MaxYRange = this.MaxYRange;
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

    #region 공정 검사 정보

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

        [Category("items")]
        [Description("Defect ID Type")]
        public bool UseAiResult { get; set; } = false;

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

            foreach (var item in this.Compare)
                data.Compare.Add(item.Clone());

            foreach (var item in this.CompRange)
                data.CompRange.Add(item.Clone());

            data.Judge = this.Judge.Clone();

            data.UseAiResult = this.UseAiResult;
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
            for (int i = 0; i < _data.Count; i++)
            {
                if (_data[i].Name == name)
                {
                    _data.RemoveAt(i);
                    break;
                }
            }
        }
        
        public PreprocItem Get(string name)
        {
            for (int i = 0; i < _data.Count; i++)
            {
                if (_data[i].Name == name)
                {
                    return _data[i].Clone();
                }
                    
            }

            return null;
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

            foreach (var item in obj.Data)
                this.Add(item);

            return true;
        }
    }
    #endregion

    #region 검사 필터 데이터
    public class ProcFilter
    {
        public string Line { get; set; } = string.Empty;
        public string Product { get; set; } = string.Empty;
        public string Model { get; set; } = string.Empty;

        /// <summary>
        /// 수동 검사에서 사용 여부
        /// </summary>
        public bool Use { get; set; } = true;

        /// <summary>
        /// 검사 주가
        /// </summary>
        public int Duration { get; set; } = 0;
        
        /// <summary>
        /// 검사 후 표시 여부
        /// </summary>
        public bool IsSkip { get; set; } = false;

        private DateTime _setTime;
        private int      _spanHour = 0;
        private bool     _isFirst = true;
       

        /// <summary>
        /// 인덱스 확인용
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            return $"{Line}_{Product}_{Model}";
        }

        public void SetTime()
        {
            _setTime = DateTime.Now;
            _spanHour = Duration;
            _isFirst = true;
        }

        public void SetTime(int spanHour)
        {
            _setTime = DateTime.Now;
            _spanHour = spanHour;
            _isFirst = true;
        }

        public void ResetTime()
        {
            _isFirst = false;
            _setTime = DateTime.Now;
        }

        public bool IsInTime()
        {
            DateTime crtTime = DateTime.Now;
            TimeSpan ts = crtTime - _setTime;

            // 제일 처음이면 바로 검사 시작
            if(_isFirst==true)
                return true;

            // 설정 시간 보다 넘어가면 true를 반환
            if (ts.Hours >= _spanHour) return true;

            return false;
        }
    }

    public class ProcFilterList : ItemList<ProcFilter>
    {
        public ProcFilterList()
        {

        }
    }

    public class ProcFilterSet
    {
        public bool UseLiveSync { get; set; } = false;
        public int SyncDuration { get; set; } = 1;
        
        public ProcFilterList this[int idx]
        {
            get { return Filter[idx]; }
            set { Filter[idx] = value; }
        }
        public ProcFilterList[] Filter { get; set; } = null;

        public ProcFilterSet()
        {
            int size = (int)Preproc.eProc.Total;
            Filter = new ProcFilterList[size];
            for (int i = 0; i < size; i++)
                Filter[i] = new ProcFilterList();
        }

        #region Filter 설정
        public void Save()
        {
            string path = Define.FilterSetPath;
            string jsonString = Newtonsoft.Json.JsonConvert.SerializeObject(this, Newtonsoft.Json.Formatting.Indented);
            System.IO.File.WriteAllText(path, jsonString);
        }

        public void Load()
        {
            try
            {
                string path = Define.FilterSetPath;
                string jsonString = "";


                if (System.IO.File.Exists(path)) jsonString = System.IO.File.ReadAllText(path);
                else return;

                var obj = Newtonsoft.Json.JsonConvert.DeserializeObject<ProcFilterSet>(jsonString);
                this.UseLiveSync = obj.UseLiveSync;
                this.SyncDuration = obj.SyncDuration;
                this.Filter = obj.Filter;
            }
            catch(Exception ex)
            {
            }
        }
        #endregion
    }

    #endregion

    #region CSV 프로세스 파라미터
    public class CSVProcParam
    {
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

        [Category("items")]
        [Description("Defect ID Type")]
        public bool UseAiResult { get; set; } = false;

        [Category("items")]
        [Description("Comp Type")]
        public int CompType { get; set; } = 0;

        public void Save()
        {
            string path = Define.CsvParamSetPath;
            string jsonString = Newtonsoft.Json.JsonConvert.SerializeObject(this, Newtonsoft.Json.Formatting.Indented);
            System.IO.File.WriteAllText(path, jsonString);
        }

        public void Load()
        {
            try
            {
                string path = Define.CsvParamSetPath;
                string jsonString = "";

                if (System.IO.File.Exists(path)) jsonString = System.IO.File.ReadAllText(path);
                else return;

                var obj = Newtonsoft.Json.JsonConvert.DeserializeObject<CSVProcParam>(jsonString);
                this.BasicRange = obj.BasicRange;
                this.CompRange = obj.CompRange;
                this.Judge = obj.Judge;
                this.UseAiResult = obj.UseAiResult;
            }
            catch (Exception ex)
            {

            }
        }
    }
    #endregion

    #region LOT 지정 검사
    public enum FilterType
    {
        UserFilter = 0,
        DbFilter
    }

    public class DBFilter
    {
        [Category("DB Filter")]
        [Description("Title")]
        public string Title { get; set; } =string.Empty;
        [Category("DB Filter")]
        [Description("MKCD")]
        public string MKCD { get; set; } = string.Empty;
        [Category("DB Filter")]
        [Description("Use ES")]
        public bool UseES { get; set; } = false;
        [Category("DB Filter")]
        [Description("Use TG")] 
        public bool UseTG { get; set; } = false;
        [Category("DB Filter")]
        [Description("Use ETC")]
        public bool UseETC { get; set; } = false;

        public DBFilter()
        {

        }
    }

    public class LotSelProcParam
    {
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

        [Category("items")]
        [Description("Defect ID Type")]
        public bool UseAiResult { get; set; } = false;

        [Category("items")]
        [Description("Filter Type")]
        public FilterType FilterType { get; set; } = FilterType.UserFilter;

        [Category("items")]
        [Description("DB Filter")]
        public DBFilter DBFilter { get; set; } = new DBFilter();

        [Category("items")]
        [Description("User Filter")]
        public ProcFilterList UserFilter { get; set; } = new ProcFilterList();

        public void Save()
        {
            try
            {
                string path = Define.CsvParamSetPath;
                string jsonString = Newtonsoft.Json.JsonConvert.SerializeObject(this, Newtonsoft.Json.Formatting.Indented);
                System.IO.File.WriteAllText(path, jsonString);
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] Save Lot Select Process Param : {ex.Message}");
            }
        }

        public void Load()
        {
            try
            {
                string path = Define.CsvParamSetPath;
                string jsonString = "";
                
                if (System.IO.File.Exists(path)) jsonString = System.IO.File.ReadAllText(path);
                else return;

                var obj = Newtonsoft.Json.JsonConvert.DeserializeObject<LotSelProcParam>(jsonString);
                this.BasicRange = obj.BasicRange;
                this.CompRange = obj.CompRange;
                this.Judge = obj.Judge;
                this.UseAiResult = obj.UseAiResult;
                this.FilterType = obj.FilterType;
                this.DBFilter = obj.DBFilter;
                this.UserFilter = obj.UserFilter;
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] Load Lot Select Process Param : {ex.Message}");
            }
        }
    }
    #endregion

}
