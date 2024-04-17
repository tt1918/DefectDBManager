using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    #region MKCD 데이터 요청 처리
    public class MkcdParam
    {
        public bool IsReceived { get; private set; }
        public string Name { get; private set; }

        public MkcdParam()
        {
            IsReceived = false;
            Name = "";
        }

        public void Reset()
        {
            IsReceived = false;
            Name = "";
        }

        public void Set(string name)
        {
            IsReceived = true;
            Name = name;
        }
    }

    #endregion

    #region MKCD Model Data
    public class MKCD_Data
    {
        public string FLTID { get; set; }
        public bool MRKF1 { get; set; }
        public float SIZE { get; set; }

        public MKCD_Data()
        {
            FLTID = "";
            MRKF1 = false;
            SIZE  = 0.0f;
        }

        public MKCD_Data(MRKCTLMSTData data)
        {
            try
            {
                this.FLTID = data.FLTID;
                this.MRKF1 = int.Parse(data.MRKF1) == 1 ? true : false;
                this.SIZE = data.SIZE;
            }
            catch (Exception ex)
            {
                Log.Write(ex.Message);
            }

        }

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
                if (input.MRKF1 == true &&
                    ((input.SIZE > 0.0 && input.SIZE <= Data[input.FLTID].SIZE) ||
                    (input.SIZE != 0.0 && Data[input.FLTID].SIZE == 0.0))
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

        public void Add(string LNCD, MKCD_Data input)
        {
            if (Param.ContainsKey(LNCD) == false)
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

            if (File.Exists(path) == true)
                File.Delete(path);

            using (StreamWriter sw = new StreamWriter(path))
            {
                foreach (MKCD_LNCD_Data data in Param.Values)
                {
                    foreach (MKCD_Data item in data.Data.Values)
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
                using (StreamReader sr = new StreamReader(path))
                {
                    while ((data = sr.ReadLine()) != null)
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
