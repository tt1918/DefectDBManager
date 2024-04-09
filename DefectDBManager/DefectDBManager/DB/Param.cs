using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    public class SkipOffsetParam
    {
        public float Left;
        public float Right;
        public float Top;
        public float Bottom;

        public bool CheckSkip(float posX, double posY)
        {
            bool bSkip = false;
            if (Top > 0 || Bottom > 0)
                if (posY > Top && posY < Bottom)
                    return true;
            if (Left > 0 || Right > 0)
                if (posX > Left && posX < Right)
                    return true;
            return bSkip;
        }
    }


    // 공통으로 사용되는  임시 파라미터
    public class Param
    {
        public List<SkipOffsetParam> OffsetSkip;
        public UserDefectClass _UserDefectClass;

        public List<string> FAULTDATFLTID;
        public List<string> MRKCTLMSTFLTID;

        #region 불량 갯수
        public int[] DBFaultCount = new int[(int)eFLTID.MAX_PARAM];
        public int[] CSVFalutCount = new int[(int)eFLTID.MAX_PARAM];
        public int ESFalutCount;
        public int[] ESClassDefectCnt = new int[(int)eESDefectClass.MAX];

        public int Optic1Cnt = 0;
        public int Optic1ScCount = 0;
        public int Optic2Cnt = 0;
        public int Optic3Cnt = 0;
        public int Optic4DotCnt = 0;
        public int Optic4LineCnt = 0;
        public int OpticFreqCnt = 0;
        public int OpticSameCnt = 0;

        public bool FLTIDCheckError = false;
        public bool InspRollCheckError = false;

        #endregion

        public string Model { get; set; }
        public string MKCDModel { get; set; }

        public bool isProductAvaliable = false;
        public bool isXOffsetError = false;

        public Param()
        {
            OffsetSkip = new List<SkipOffsetParam>();
            _UserDefectClass = new UserDefectClass();

            FAULTDATFLTID = new List<string>();
            MRKCTLMSTFLTID = new List<string>();
        }

        ~Param()
        {
            if (OffsetSkip?.Count > 0) OffsetSkip.Clear();
            if (FAULTDATFLTID?.Count > 0) FAULTDATFLTID.Clear();
        }

        public void ClearEachOpticDefectCnt()
        {
            Optic1Cnt = 0;
            Optic1ScCount = 0;
            Optic2Cnt = 0;
            Optic3Cnt = 0;
            Optic4DotCnt = 0;
            Optic4LineCnt = 0;
            OpticFreqCnt = 0;
            OpticSameCnt = 0;

            DBFaultCount.Initialize();
            CSVFalutCount.Initialize();
            ESClassDefectCnt.Initialize();
            ESFalutCount = 0;

            FLTIDCheckError = false;
            InspRollCheckError = false;
        }

        public void VerifyFLTID()
        {
            bool bMatch = true;
            int i=0, j=0;
            int size1 = FAULTDATFLTID.Count;
            if (size1 == 0)
                return;

            int size2 = MRKCTLMSTFLTID.Count;

            for(i= 0; i < size1; i++) 
            {
                for(j = 0; j < size2; j++) 
                    if (FAULTDATFLTID[i] != MRKCTLMSTFLTID[j])
                        break;
                if (j == size2 && j != 0)
                    bMatch = false;
            }

            if (i == size1 && j == size2 && i != 0 && j != 0)
                bMatch = false;

            if (size2 == 0) bMatch = false;
            FAULTDATFLTID.Clear();
            MRKCTLMSTFLTID.Clear();

            if(FLTIDCheckError==false && bMatch==false)
                FLTIDCheckError = true;
        }

        public void AddCSVFalut(int id)
        {
            CSVFalutCount[id]++;
        }
        public void AddESCount(int id)
        {
            if (id == 2305 || id == 2321) ESClassDefectCnt[(int)eESDefectClass.CROSS1]++;
            else if (id == 2306 || id == 2322) ESClassDefectCnt[(int)eESDefectClass.CROSS2]++;
            else if (id == 2307 || id == 2323) ESClassDefectCnt[(int)eESDefectClass.CROSS3]++;
            else if (id == 2308 || id == 2324) ESClassDefectCnt[(int)eESDefectClass.CROSS4]++;
            else if (id == 2310 || id == 2326) ESClassDefectCnt[(int)eESDefectClass.CROSS5]++;
            else if (id == 2311 || id == 2327) ESClassDefectCnt[(int)eESDefectClass.CROSS6]++;
            else if (id == 2309 || id == 2325) ESClassDefectCnt[(int)eESDefectClass.CROSS7]++;
            else if (id == 1025 || id == 1041) ESClassDefectCnt[(int)eESDefectClass.THRU1]++;
            else if (id == 1026 || id == 1042) ESClassDefectCnt[(int)eESDefectClass.THRU2]++;
            else if (id == 1027 || id == 1043) ESClassDefectCnt[(int)eESDefectClass.THRU3]++;
            else if (id == 1030 || id == 1046) ESClassDefectCnt[(int)eESDefectClass.THRU4]++;
            else if (id >= 1537 && id < 1567) ESClassDefectCnt[(int)eESDefectClass.REF1]++;
            else if (id == 513) ESClassDefectCnt[(int)eESDefectClass.FREQ1]++;
            else if (id == 514) ESClassDefectCnt[(int)eESDefectClass.FREQ2]++;
            else if (id == 515) ESClassDefectCnt[(int)eESDefectClass.FREQ3]++;
            else if (id == 516) ESClassDefectCnt[(int)eESDefectClass.FREQ4]++;
            else if (id == 517) ESClassDefectCnt[(int)eESDefectClass.FREQ5]++;
            else if (id == 518) ESClassDefectCnt[(int)eESDefectClass.FREQ6]++;
            else if (id == 593) ESClassDefectCnt[(int)eESDefectClass.SAME1]++;
            else if (id == 594) ESClassDefectCnt[(int)eESDefectClass.SAME2]++;
            else if (id == 595) ESClassDefectCnt[(int)eESDefectClass.SAME3]++;
            else if (id == 596) ESClassDefectCnt[(int)eESDefectClass.SAME4]++;
            else if (id == 597) ESClassDefectCnt[(int)eESDefectClass.SAME5]++;
            else ESClassDefectCnt[(int)eESDefectClass.ETC]++;
        }

        public int GetTotalDefect()
        {
            int total = 0;  
            for (int i = 0; i < ESClassDefectCnt.Length; i++)
                total += ESClassDefectCnt[i];

            return total;
        }
    }


    #region mrkctlmst 변수 (Defect Edit에서 사용)
    public class MRKCTLMST_DE_Data
    {
        public List<MRKCTLMSTData> data;
        public string query { get; set; }

        public MRKCTLMST_DE_Data()
        {
            data = new List<MRKCTLMSTData>();
        }
    }
    #endregion

    #region MKCD Model Data
    public class MKCD_Data
    {
        public string FLTID { get; set; }
        public bool MRKF1 {  get; set; }
        public float SIZE {  get; set; }

        public override string ToString()
        {
            int val1 = MRKF1 == true ? 1 : 0;
            return $"{FLTID},{val1},{SIZE:F3}";
        }
    }

    public class MKCD_LNCD_Data
    {
        public string LNCD { get; set; }
        public Dictionary<string, MKCD_Data> Data { get; set; } = null;

        public MKCD_LNCD_Data(string name)
        {
            LNCD = name;
            Data = new Dictionary<string, MKCD_Data>();
        }

        public void Add(MKCD_Data input)
        {
            if (Data.ContainsKey(input.FLTID) == false)
                Data.Add(input.FLTID, input);
            else
            {
                if(input.MRKF1==true && 
                    ((input.SIZE>0.0 && input.SIZE <= Data[input.FLTID].SIZE) || 
                    (input.SIZE!=0.0 &&  Data[input.FLTID].SIZE==0.0))
                )
                    Data[input.FLTID] = input;
            }
        }

        public List<MKCD_Data> Get()
        {
            List<MKCD_Data> data = Data.Values.ToList();
            return data;
        }
    }

    public class MKCD_MODEL
    {
        public string Name { get; set; }
        public Dictionary<string, MKCD_LNCD_Data> Param { get; set; }

        public MKCD_MODEL()
        {
            Param = new Dictionary<string, MKCD_LNCD_Data>();
        }

        public void Add(string LNCD,  MKCD_Data input)
        {
            if(Param.ContainsKey(LNCD)==false)
            {
                MKCD_LNCD_Data data = new MKCD_LNCD_Data(LNCD);
                Param.Add(LNCD, data);
                Param[LNCD].Add(input);
            }
            else
            {
                Param[LNCD].Add(input);
            }
        }

        public void Save()
        {
            string path = System.IO.Path.Combine(Define.MKCDModelPath, $"{Name}.ini");

            if(File.Exists(path)==true)
                File.Delete(path); 

            using(StreamWriter sw = new StreamWriter(path))
            {
                foreach(MKCD_LNCD_Data data in Param.Values)
                {
                    foreach(MKCD_Data item in data.Data.Values)
                    {
                        sw.WriteLine($"{data.LNCD},{item.ToString()}");
                    }
                }
                sw.Close();
            }

        }

        public void Load()
        {
            try
            {
                string path = System.IO.Path.Combine(Define.MKCDModelPath, $"{Name}.ini");
                if (File.Exists(path) == false)
                    return;

                Param.Clear();

                string data;
                string[] item;
                using(StreamReader sr = new StreamReader(path))
                {
                    while((data=sr.ReadLine())!=null)
                    {
                        item = data.Split(',');

                        MKCD_Data mKCD_Data = new MKCD_Data();
                        mKCD_Data.FLTID = item[1];
                        mKCD_Data.MRKF1 = int.Parse(item[2]) == 1 ? true : false;
                        mKCD_Data.SIZE = float.Parse(item[3]);
                        Add(item[0], mKCD_Data);
                    }
                }


            }
            catch (Exception ex)
            {

            }
            
        }
    }

    #endregion
}
