using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager.Preproc
{
    public class SjData
    {
        public int FltNo { get; set; } = 0;
        public string PicName { get; set; } = string.Empty;
        public string FLTID { get; set; } = string.Empty;
        public string SECFLTID { get; set; } = string.Empty;
    }

    public class SjJudgement
    {
        public List<string> FLTID { get; set; } = new List<string>();
        public string SECFLTID { get; set; } = string.Empty;
        public int Total { get; set; } = 0;
        public int Converted { get; set; } = 0;
        public double NoneConvertRate { get; set; } = 0.0;
        public bool Judgement { get; set; } = false;
    }

    public class SjMonitorData
    {
        public string LNCD { get; set; } = string.Empty;
        public string CTLNO { get; set; } = string.Empty;
        public int ModeNo { get; set; } = 0;
        public bool IsFinished { get; set; } = false;
        public Dictionary<int, List<SjData>> DefectInfo { get; set; } = new Dictionary<int, List<SjData>>();
        public Dictionary<int, SjJudgement> Judgement { get; set; } = new Dictionary<int, SjJudgement>();

        public override string ToString()
        {
            return $"CTLNO: {CTLNO}, SJMode: {ModeNo}, IsFinished: {IsFinished}";
        }

        public SjMonitorData()
        {
        }

        public SjMonitorData(string ctlno, int modeNo, bool isFinished)
        {
            CTLNO = ctlno;
            ModeNo = modeNo;
            IsFinished = isFinished;
        }

        public void AddDefect(int modeNo, SjData defect)
        {
            if (!DefectInfo.ContainsKey(modeNo))
                DefectInfo.Add(modeNo, new List<SjData>());
            
            DefectInfo[modeNo].Add(defect);
        }

        public void ClearDefect()
        {
            DefectInfo.Clear();
        }

        public double SetDefectJudgement(int modeNo, SjModelItem model)
        {
            if (DefectInfo.ContainsKey(modeNo))
            {
                var defects = DefectInfo[modeNo];
                if (defects.Count > 0)
                {
                    double totalRate = defects.Count(d => model.FLTID.Contains(d.FLTID));
                    double convRate = defects.Count(d => model.FLTID.Contains(d.FLTID) && d.SECFLTID == model.SECFLTID);
                    double mismatchRate = 1.0 - (convRate / totalRate);

                    // 데이터가 없으면 무시한다.
                    if (totalRate == 0) return 0.0;

                    string fltId = string.Join(",", model.FLTID);
                    double rate = model.Rate;

                    if (!Judgement.ContainsKey(modeNo))
                    {
                        Judgement.Add(modeNo, new SjJudgement
                        {
                            FLTID = model.FLTID,
                            SECFLTID = model.SECFLTID,
                            Total = (int)totalRate,
                            Converted = (int)convRate,
                            NoneConvertRate = mismatchRate,
                            Judgement = mismatchRate * 100.0 >= rate ? false : true
                        });
                    }
                    else
                    {
                        Judgement[modeNo] = new SjJudgement
                        {
                            FLTID = model.FLTID,
                            SECFLTID = model.SECFLTID,
                            Total = (int)totalRate,
                            Converted = (int)convRate,  
                            NoneConvertRate = mismatchRate,
                            Judgement = mismatchRate * 100.0 >= rate ? false : true
                        };
                    }

                    return mismatchRate * 100.0;
                }
            }
            return 0.0;
        }

        public double GetOverallDefectRate()
        {
            double totalDefects = 0;
            double totalConverted = 0;
            foreach (var mode in DefectInfo)
            {
                totalDefects += mode.Value.Count;
                totalConverted += mode.Value.Count(d => d.FLTID != d.SECFLTID);
            }
            if (totalDefects > 0)
                return (totalConverted / totalDefects) * 100.0;
            return 0.0;
        }

    }

    public class SJModeMonotorDataList
    {
        public List<SjMonitorData> DataList { get; set; } = new List<SjMonitorData>();
        public void Add(SjMonitorData data)
        {
            DataList.Add(data);
        }
        
        public void Remove(SjMonitorData data)
        {
            DataList.Remove(data);
        }

        public void Remove(string ctlno)
        {
            var item = DataList.FirstOrDefault(x => x.CTLNO == ctlno);
            if (item != null)
                DataList.Remove(item);
        }

        public void Clear()
        {
            DataList.Clear();
        }

        public bool Exist(SjMonitorData data)
        { 
            var item =  DataList.FirstOrDefault(x=>x.CTLNO == data.CTLNO);
            return item != null ? true : false;
        }
        public bool Exist(string ctrno)
        {
            var item = DataList.FirstOrDefault(x => x.CTLNO == ctrno);
            return item != null ? true : false;
        }
    }
}
