using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Runtime.InteropServices.ComTypes;
using System.Diagnostics;

namespace DefectDBManager
{
    public class BaseUnit
    {
        public string Title;

        public BaseUnit()
        {

        }

        public BaseUnit(string title)
        {
            this.Title = title;
        }
    }


    public class DestSplitUnit : BaseUnit
    {
        public bool IsUse { get; set; }
        public float StartX { get; set; }
        public float EndX { get; set; }
        public string MKCD { get; set; }
        public bool TG { get; set; }
        public bool ES { get; set; }
        public bool ETC { get; set; }

        public DestSplitUnit()
        {
            Reset();
        }

        public void Reset()
        {
            Title = "";
            IsUse = false;
            StartX = 0;
            EndX = 0;
            MKCD = "";
            TG = ES = ETC = false;
        }
    }

    public class SkipSize
    {
        public float minX;
        public float maxX;
        public float minY;
        public float maxY;
        public float min;  // 크기
        public float max;  // 크기

        public void Reset()
        {
            minX = 0.0f;
            maxX = 0.0f;
            minY = 0.0f;
            maxY = 0.0f;
            min = 0.0f;
            max = 0.0f;
        }

        public void SetData(SkipSize src)
        {
            this.minX = src.minX;
            this.maxX = src.maxX;
            this.minY = src.minY;
            this.maxY = src.maxY;
            this.min = src.min;
            this.max = src.max;
        }

        public bool Check(float sizeX, float sizeY, float size)
        {
            if (sizeX < minX || sizeX > maxX || sizeY < minY || sizeY > maxY || size < min || size > max)
                return true;
            return false;
        }
    }

    public class DestConfigUnit : BaseUnit
    {
        public string MKCD;
        public bool UseTG;
        public bool UseES;
        public bool UseETC;
        public bool UseSameDefect;

        // Edge Defect Skip
        public int SkipLeftMM;
        public int SkipLeftCnt;
        public int SkipRightMM;
        public int SkipRightCnt;

        // Marking
        public int OPTIC1;
        public int OPTIC2;
        public int OPTIC3;
        public int OPTIC4;
        public int DB_ES;
        public int DB_DT;

        public int Index; // data index
        public int LengErrorRangeMinus;
        public int LengErrorRangePlus;
        public bool useFaltIDCheck;
        public bool UsePTRYLPYLMYKHCheck;
        public string[] FLTIDCheck = new string[10];

        public DestConfigUnit()
        {
            int count = System.Enum.GetValues(typeof(eOpticClass)).Length;
            Reset();
        }

        ~DestConfigUnit()
        {

        }

        public void Reset()
        {
            Title = "";
            MKCD = "";
            UseTG = false;
            UseES = false;
            UseETC = false;
            UseSameDefect = false;

            SkipLeftMM = 0;
            SkipLeftCnt = 0;
            SkipRightMM = 0;
            SkipRightCnt = 0;

            OPTIC1 = 1;
            OPTIC2 = 1;
            OPTIC3 = 1;
            OPTIC4 = 1;
            DB_ES = 1;
            DB_DT = 1;

            Index = 0;
        }

        public void CopyData(DestConfigUnit u)
        {
            Title = u.Title;
            MKCD = u.MKCD;
            UseTG = u.UseTG;
            UseES = u.UseES;
            UseETC = u.UseETC;
            UseSameDefect = u.UseSameDefect;

            SkipLeftMM = u.SkipLeftMM;
            SkipLeftCnt = u.SkipLeftCnt;
            SkipRightMM = u.SkipRightMM;
            SkipRightCnt = u.SkipRightCnt;

            OPTIC1 = u.OPTIC1;
            OPTIC2 = u.OPTIC2;
            OPTIC3 = u.OPTIC3;
            OPTIC4 = u.OPTIC4;
            DB_ES = u.DB_ES;
            DB_DT = u.DB_DT;

            Index = u.Index;
        }

        public int GetSize()
        {
            int size = Marshal.SizeOf(new DestConfigUnit());
            return size;
        }
    }



    public class TimeIndex
    {
        public int start;
        public int end;

        public void SearchTime(ref int sTime, ref int eTime)
        {
            sTime = start;
            eTime = end;
        }
    }

    public class EsTimeIndex : TimeIndex
    {
        public bool IsUse;
    }

    public class DBLoginInfo
    {
        public string   ID;
        public string   PW;
        public string   Name;
        public string   Port;
        public string   IP;
        public int      StringType;

        public DBLoginInfo()
        {
            Read();
        }

        public bool GetDBLoginInfo(ref string id, ref string pw, ref string name)
        {
            id = ID;
            pw = PW;
            name = Name;

            return true;
        }

        public void Read()
        {
            string key = "DB_CONFIG";
            ID = NativeFunc.ReadIni(Define.DbInfoPath, key, "DB_ID", "");
            PW = NativeFunc.ReadIni(Define.DbInfoPath, key, "DB_PW", "");
            Name = NativeFunc.ReadIni(Define.DbInfoPath, key, "DB_NAME", "");
            Port = NativeFunc.ReadIni(Define.DbInfoPath, key, "DB_PORT", "");
            IP = NativeFunc.ReadIni(Define.DbInfoPath, key, "DB_IP", "");
            StringType = NativeFunc.ReadIni(Define.DbInfoPath, key, "DB_CON_STRING_TYPE", 0);
        }

        public void Write()
        {
            string key = "DB_CONFIG";
            NativeFunc.WriteIni(Define.DbInfoPath, key, "DB_ID", ID);
            NativeFunc.WriteIni(Define.DbInfoPath, key, "DB_PW", PW);
            NativeFunc.WriteIni(Define.DbInfoPath, key, "DB_NAME", Name);
            NativeFunc.WriteIni(Define.DbInfoPath, key, "DB_PORT", Port);
            NativeFunc.WriteIni(Define.DbInfoPath, key, "DB_IP", IP);
            NativeFunc.WriteIni(Define.DbInfoPath, key, "DB_CON_STRING_TYPE", StringType);
        }
    }

    public class DestConfig
    {
        public const int MaxOptic = 6;

        public Dictionary<string, DestConfigUnit> DicDest;

        // DB
        public TimeIndex DbTime = new TimeIndex();
        // ES
        public EsTimeIndex ESDbTime = new EsTimeIndex();

        // DB Login
        public DBLoginInfo dbLogin = new DBLoginInfo();

        public SkipSize[] SkipData;

        // DB CSB Type
        public eCSV_TYPE CSVType;

        // System
        public string MarkingIP; // Marking기 IP Address
        public int NoBcrWarning;
        public int NoBcrError;

        public int EverMarkDefectMeter
        {
            get { return everMarkDefectMeter; }
        }
        private int everMarkDefectMeter; //에버테크노 마킹기에 보내는 M당 결점갯수를 몇 M 전에 보낼지 정함. 

        public int CSV_Ver
        {
            get { return csvVer; }
        }
        private int csvVer;              // CSV 버전

        public bool UseXOffset
        {
            get { return useXOffset; }
        }
        private bool useXOffset;

        public bool UseXOffsetAlarm
        {
            get { return useXOffsetAlarm; }
        }
        private bool useXOffsetAlarm;

        public bool UseAREADEL
        {
            get { return useAREADEL; }
        }
        private bool useAREADEL;

        public DestConfigUnit SelDestUnit;

        public DestConfig()
        {
            DicDest = new Dictionary<string, DestConfigUnit>();

            SkipData = new SkipSize[MaxOptic];
            for (int i = 0; i < MaxOptic; i++)
            {
                SkipData[i] = new SkipSize();
            }

            Reset();
        }

        ~DestConfig()
        {
            //Reset();
        }

        public void Reset()
        {
            DicDest.Clear();
        }

        public int Read()
        {
            try
            {
                if (File.Exists(Define.DestPath) == false) return -1;

                int opticSize = System.Enum.GetValues(typeof(eOpticClass)).Length;
                string key;

                key = "SKIP SIZE";
                for (int j = 0; j < opticSize; j++)
                {
                    SkipData[j].minX = NativeFunc.ReadIni(Define.DestPath, key, $"SKIP_SIZE_X_MIN{j + 1}", 0.0f);
                    SkipData[j].minY = NativeFunc.ReadIni(Define.DestPath, key, $"SKIP_SIZE_Y_MIN{j + 1}", 0.0f);
                    SkipData[j].min = NativeFunc.ReadIni(Define.DestPath, key, $"SKIP_SIZE_MIN{j + 1}", 0.0f);

                    SkipData[j].maxX = NativeFunc.ReadIni(Define.DestPath, key, $"SKIP_SIZE_X_MAX{j + 1}", 99990.0f);
                    SkipData[j].maxY = NativeFunc.ReadIni(Define.DestPath, key, $"SKIP_SIZE_Y_MAX{j + 1}", 99990.0f);
                    SkipData[j].max = NativeFunc.ReadIni(Define.DestPath, key, $"SKIP_SIZE_MAX{j + 1}", 99990.0f);
                }

                this.DicDest.Clear();
                for (int i = 0; i < Global.MaxDestItemCnt; i++)
                {
                    DestConfigUnit unit = new DestConfigUnit();
                    key = $"DEST-{i}";
                    unit.Title = NativeFunc.ReadIni(Define.DestPath, key, "TITLE", "");
                    unit.MKCD = NativeFunc.ReadIni(Define.DestPath, key, "MKCD", "62");
                    unit.UseTG = NativeFunc.ReadIni(Define.DestPath, key, "TG", false);
                    unit.UseES = NativeFunc.ReadIni(Define.DestPath, key, "ES", false);
                    unit.UseETC = NativeFunc.ReadIni(Define.DestPath, key, "ETC", false);
                    unit.UseSameDefect = NativeFunc.ReadIni(Define.DestPath, key, "SAME_DEFECT", false);

                    unit.SkipLeftMM = NativeFunc.ReadIni(Define.DestPath, key, "SKIP_LEFT_MM", 0);
                    unit.SkipRightMM = NativeFunc.ReadIni(Define.DestPath, key, "SKIP_RIGHT_MM", 0);
                    unit.SkipLeftCnt = NativeFunc.ReadIni(Define.DestPath, key, "SKIP_LEFT_CNT", 0);
                    unit.SkipRightCnt = NativeFunc.ReadIni(Define.DestPath, key, "SKIP_RIGHT_CNT", 0);

                    unit.OPTIC1 = NativeFunc.ReadIni(Define.DestPath, key, "OPTIC_1", 1);
                    unit.OPTIC2 = NativeFunc.ReadIni(Define.DestPath, key, "OPTIC_2", 1);
                    unit.OPTIC3 = NativeFunc.ReadIni(Define.DestPath, key, "OPTIC_3", 1);
                    unit.OPTIC4 = NativeFunc.ReadIni(Define.DestPath, key, "OPTIC_4", 1);

                    unit.DB_ES = NativeFunc.ReadIni(Define.DestPath, key, "DB_ES", 1);
                    unit.DB_DT = NativeFunc.ReadIni(Define.DestPath, key, "DB_DT", 1);

                    unit.LengErrorRangeMinus = NativeFunc.ReadIni(Define.DestPath, key, "LengErrorRangeMinus", 9999999);
                    unit.LengErrorRangePlus = NativeFunc.ReadIni(Define.DestPath, key, "LengErrorRangePlus", 9999999);
                    unit.useFaltIDCheck = NativeFunc.ReadIni(Define.DestPath, key, "UseFaltIDCheck", false);
                    unit.UsePTRYLPYLMYKHCheck = NativeFunc.ReadIni(Define.DestPath, key, "UsePTRYLPYLMYKHCheck", false);

                    for (int j = 0; j < unit.FLTIDCheck.Length; j++)
                        unit.FLTIDCheck[j] = NativeFunc.ReadIni(Define.DestPath, key, $"FLTID_CHECK[{j}]", "");

                    unit.Index = i;

                    if (unit.Title.Length > 0 && unit.Title != "")
                        this.DicDest.Add(unit.Title, unit);
                }

                key = "DB_CONFIG";
                this.CSVType = (eCSV_TYPE)NativeFunc.ReadIni(Define.DestPath, key, "CSV_TYPE", (int)eCSV_TYPE.None);
                this.csvVer = NativeFunc.ReadIni(Define.DestPath, key, "CSV_VER", 0);
                this.useXOffset = NativeFunc.ReadIni(Define.DestPath, key, "USE_XOFSMST", true);
                this.useXOffsetAlarm = NativeFunc.ReadIni(Define.DestPath, key, "USE_XOFSMST_ALARM", false);
                this.useAREADEL = NativeFunc.ReadIni(Define.DestPath, key, "USE_AREADEL", false);

                DbTime.start = NativeFunc.ReadIni(Define.DestPath, key, "DB_START", 10);
                DbTime.end = NativeFunc.ReadIni(Define.DestPath, key, "DB_END", 10);

                ESDbTime.IsUse = NativeFunc.ReadIni(Define.DestPath, key, "ES_USE", true);
                ESDbTime.start = NativeFunc.ReadIni(Define.DestPath, key, "ES_START", 120);
                ESDbTime.end = NativeFunc.ReadIni(Define.DestPath, key, "ES_END", 10);

                key = "SYSTEM";
                this.MarkingIP = NativeFunc.ReadIni(Define.DestPath, key, "MARKING_ADDR", "");
                this.NoBcrWarning = NativeFunc.ReadIni(Define.DestPath, key, "NO_BCR_WARNING", 10);
                this.NoBcrError = NativeFunc.ReadIni(Define.DestPath, key, "NO_BCR_ERROR", 30);

                everMarkDefectMeter = NativeFunc.ReadIni(Define.DestPath, key, "EVER_MARK_DEFECT_METER", 10);
            }
            catch(Exception ex)
            {
                Trace.WriteLine(ex.Message);
                Log.Write(ex.Message);
                return 0;
            }
            return 0;
        }

        public void Write()
        {
            try
            {
                string key;
                DestConfigUnit unit = null;
                int opticSize = System.Enum.GetValues(typeof(eOpticClass)).Length;

                File.Delete(Define.DestPath);

                key = "SKIP SIZE";
                for (int j = 0; j < opticSize; j++)
                {
                    NativeFunc.WriteIni(Define.DestPath, key, $"SKIP_SIZE_X_MIN{j + 1}", SkipData[j].minX);
                    NativeFunc.WriteIni(Define.DestPath, key, $"SKIP_SIZE_Y_MIN{j + 1}", SkipData[j].minY);
                    NativeFunc.WriteIni(Define.DestPath, key, $"SKIP_SIZE_MIN{j + 1}", SkipData[j].min);

                    NativeFunc.WriteIni(Define.DestPath, key, $"SKIP_SIZE_X_MAX{j + 1}", SkipData[j].maxX);
                    NativeFunc.WriteIni(Define.DestPath, key, $"SKIP_SIZE_Y_MAX{j + 1}", SkipData[j].maxY);
                    NativeFunc.WriteIni(Define.DestPath, key, $"SKIP_SIZE_MAX{j + 1}", SkipData[j].max);
                }

                for (int i = 0; i < this.DicDest.Count; i++)
                {
                    if (GetData(i, ref unit) == true)
                    {
                        key = $"DEST-{i}";
                        NativeFunc.WriteIni(Define.DestPath, key, "TITLE", unit.Title);
                        NativeFunc.WriteIni(Define.DestPath, key, "MKCD", unit.MKCD);
                        NativeFunc.WriteIni(Define.DestPath, key, "TG", unit.UseTG);
                        NativeFunc.WriteIni(Define.DestPath, key, "ES", unit.UseES);
                        NativeFunc.WriteIni(Define.DestPath, key, "ETC", unit.UseETC);
                        NativeFunc.WriteIni(Define.DestPath, key, "SAME_DEFECT", unit.UseSameDefect);

                        NativeFunc.WriteIni(Define.DestPath, key, "SKIP_LEFT_MM", unit.SkipLeftMM);
                        NativeFunc.WriteIni(Define.DestPath, key, "SKIP_RIGHT_MM", unit.SkipRightMM);
                        NativeFunc.WriteIni(Define.DestPath, key, "SKIP_LEFT_CNT", unit.SkipLeftCnt);
                        NativeFunc.WriteIni(Define.DestPath, key, "SKIP_RIGHT_CNT", unit.SkipRightCnt);

                        NativeFunc.WriteIni(Define.DestPath, key, "OPTIC_1", unit.OPTIC1);
                        NativeFunc.WriteIni(Define.DestPath, key, "OPTIC_2", unit.OPTIC2);
                        NativeFunc.WriteIni(Define.DestPath, key, "OPTIC_3", unit.OPTIC3);
                        NativeFunc.WriteIni(Define.DestPath, key, "OPTIC_4", unit.OPTIC4);

                        NativeFunc.WriteIni(Define.DestPath, key, "DB_ES", unit.DB_ES);
                        NativeFunc.WriteIni(Define.DestPath, key, "DB_DT", unit.DB_DT);

                        NativeFunc.WriteIni(Define.DestPath, key, "LengErrorRangeMinus", unit.LengErrorRangeMinus);
                        NativeFunc.WriteIni(Define.DestPath, key, "LengErrorRangePlus", unit.LengErrorRangePlus);
                        NativeFunc.WriteIni(Define.DestPath, key, "UseFaltIDCheck", unit.useFaltIDCheck);
                        NativeFunc.WriteIni(Define.DestPath, key, "UsePTRYLPYLMYKHCheck", unit.UsePTRYLPYLMYKHCheck);

                        for (int j = 0; j < unit.FLTIDCheck.Length; j++)
                            NativeFunc.WriteIni(Define.DestPath, key, $"FLTID_CHECK[{j}]", unit.FLTIDCheck[j]);
                    }

                }

                key = "DB_CONFIG";
                NativeFunc.WriteIni(Define.DestPath, key, "CSV_TYPE", (int)this.CSVType);
                NativeFunc.WriteIni(Define.DestPath, key, "CSV_VER", this.csvVer);
                NativeFunc.WriteIni(Define.DestPath, key, "USE_XOFSMST", this.UseXOffset);
                NativeFunc.WriteIni(Define.DestPath, key, "USE_XOFSMST_ALARM", this.useXOffsetAlarm);
                NativeFunc.WriteIni(Define.DestPath, key, "USE_AREADEL", this.UseAREADEL);

                NativeFunc.WriteIni(Define.DestPath, key, "DB_START", DbTime.start);
                NativeFunc.WriteIni(Define.DestPath, key, "DB_END", DbTime.end);

                NativeFunc.WriteIni(Define.DestPath, key, "ES_USE", ESDbTime.IsUse);
                NativeFunc.WriteIni(Define.DestPath, key, "ES_START", ESDbTime.start);
                NativeFunc.WriteIni(Define.DestPath, key, "ES_END", ESDbTime.end);

                key = "SYSTEM";
                NativeFunc.WriteIni(Define.DestPath, key, "MARKING_ADDR", this.MarkingIP);
                NativeFunc.WriteIni(Define.DestPath, key, "NO_BCR_WARNING", this.NoBcrWarning);
                NativeFunc.WriteIni(Define.DestPath, key, "NO_BCR_ERROR", this.NoBcrError);

                NativeFunc.WriteIni(Define.DestPath, key, "EVER_MARK_DEFECT_METER", EverMarkDefectMeter);
            }
            catch(Exception ex)
            {
                Trace.WriteLine(ex.Message);
                Log.Write(ex.Message);
            }
        }

        public int GetSize()
        {
            return DicDest.Count();
        }

        public bool GetData(int index, ref DestConfigUnit p)
        {
            foreach (var unit in this.DicDest)
            {
                if (unit.Value.Index == index)
                {
                    p = unit.Value;
                    return true;
                }
            }
            return false;
        }

        public bool GetData(string dest, ref DestConfigUnit p)
        {
            if (DicDest.ContainsKey(dest) == true)
                p = DicDest[dest];
            else
                return false;

            return true;
        }

        public bool SetData(int index, DestConfigUnit p)
        {

            return true;
        }

        public eCSV_TYPE GetCsvType()
        {
            return CSVType;
        }

        public bool SetSelDest(int index)
        {
            bool bRes = false;
            DestConfigUnit u = new DestConfigUnit();
            if (GetData(index, ref u)==true)
            {
                SelDestUnit = u;
                bRes = true;
            }
            return bRes;
        }

        public bool SetSelDest(string dest)
        {
            bool bRes = false;
            DestConfigUnit u = new DestConfigUnit();
            if (GetData(dest, ref u) == true)
            {
                SelDestUnit = u;
                bRes = true;
            }
            return bRes;
        }
    }
}
