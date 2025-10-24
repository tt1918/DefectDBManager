using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager.Preproc.LOT
{
    public class CompSummary
    {
        /// <summary>
        /// Lot 이름
        /// </summary>
        public string Name { get; set; } = string.Empty;
        /// <summary>
        /// CTLNO
        /// Lot Split한 경우 CTLNO 표시 필요함
        /// </summary>
        public string CTLNO { get; set; } = string.Empty;
        /// <summary>
        ///  
        /// </summary>
        public int BasicCount{ get; set; }
        public List<int> CompCount { get; set; } = new List<int>();
        public List<double> CompRate {  get; set; } = new List<double>();
        public List<bool> CompJudge { get; set; }  = new List<bool>();
        /// <summary>
        /// 결점 다발 여부 확인
        /// </summary>
        public bool IsBunchDefects { get; set; } = false;

        public CompSummary()
        {
            
        }
    }


    public class CompResult
    {
        /// <summary>
        /// LOT 이름
        /// </summary>
        public string Name { get; set; } = string.Empty;
        public string Product { get; set; } = string.Empty;
        /// <summary>
        /// 판정
        /// </summary>
        public eCompResult Judge { get; set; }
        /// <summary>
        /// 기준 LNCD
        /// </summary>
        public string RefLNCD { get; set; } = string.Empty;
        /// <summary>
        /// 기준 데이터
        /// </summary>
        public CompRange BasicRange { get; set; } = new CompRange();
        /// <summary>
        /// 비교 데이터
        /// </summary>
        public List<CompRange> CompRanges { get; set; } = new List<CompRange> ();

        public List<CompSummary> Summary { get; set; } = new List<CompSummary>();

        public CompResult() 
        { 

        }

        public void SetRange(PreprocItem param)
        {
            BasicRange = param.BasicRange.Clone();

            CompRanges.Clear();
            foreach (var item in param.CompRange)
                CompRanges.Add(item.Clone());
        }

        public void SetRange(LotSelProcParam param)
        {
            BasicRange = param.BasicRange.Clone();

            CompRanges.Clear();
            foreach (var item in param.CompRange)
                CompRanges.Add(item.Clone());
        }

        /// <summary>
        /// 시간까지 포함된 데이터를 표시한다.
        /// </summary>
        /// <param name="path"></param>
        public void SaveSummary(string path, int index)
        {
            string filePath = path+"_InspectResult_List.txt";
            using (StreamWriter sw = new StreamWriter(filePath, true))
            {
                foreach(var item in Summary)
                {
                    StringBuilder sb = new StringBuilder();
                    sb.Append($"{index}.");
                    sb.Append(Name);
                    sb.Append("_");
                    sb.Append(Product);
                    sb.Append("_");

                    bool isNg = item.CompJudge.Any(judge => judge == false);

                    if(isNg)
                    {
                        sb.Append(Language.judgeNG);
                    }
                    else
                    {
                        if(item.CompCount.All(count=>count==0))
                            sb.Append(Language.judgeNoCompData);
                        else
                            sb.Append(Language.judgeOK);
                    }

                    sb.Append("_");
                    sb.Append($"{RefLNCD}-{item.Name}");
                    
                    if (item.CTLNO != string.Empty)
                    {
                        sb.Append($"_{item.CTLNO}");
                        if (isNg)    sb.Append("_");
                    }

                    if (isNg)
                    {
                        sb.Append("(");
                        sb.Append($"Ref:100%");
                        if (item.CompCount.Count > 0) sb.Append(",");

                        for(int i=0; i<item.CompCount.Count; i++)
                        {
                            string judge = item.CompJudge[i] == true ? "P" : "N";
                            sb.Append($"Case{i + 1}:{item.CompRate[i]:F0}%({judge})");
                            if (i != item.CompCount.Count - 1)
                                sb.Append(",");
                        }
                        sb.Append(")");
                    }
                    sw.WriteLine(sb.ToString());
                }
                sw.WriteLine("");
                sw.Close();
            }
        }

        public void SaveDetail(string path, int index)
        {
            string filePath = path + "_InspectResult_Detail.txt";
            using (StreamWriter sw = new StreamWriter(filePath, true))
            {
                StringBuilder sb = new StringBuilder();

                #region Title
                sb.Append($"{index}.");
                sb.Append(Name);
                sb.Append("_");
                sb.Append(Product);
                sb.Append("_");
                switch(Judge)
                {
                    case eCompResult.ProcOk: sb.Append(Language.judgeOK); break;
                    case eCompResult.ProcNg: sb.Append(Language.judgeNG); break;
                    case eCompResult.NoDbData: sb.Append(Language.judgeNoData); break;
                    case eCompResult.NoCommPosData: sb.Append(Language.judgeNoCompData); break;
                }
                sw.WriteLine(sb.ToString());
                #endregion

                #region Reference Range
                sb.Clear();
                sb.Append($" Range_Reference:");
                sb.Append($"[X:{BasicRange.MinXRange}_{BasicRange.MaxXRange}]");
                sb.Append($"[Y:{BasicRange.MinYRange}_{BasicRange.MaxYRange}]");
                sb.Append($"[Range:{BasicRange.Accuracy}]");
                sw.WriteLine(sb.ToString());
                #endregion

                #region Comp Range
                int idx = 1;
                foreach(var range in CompRanges)
                {
                    sb.Clear();
                    sb.Append($" Range_Case{idx}:");
                    sb.Append($"[X:{range.MinXRange}_{range.MaxXRange}]");
                    sb.Append($"[Y:{range.MinYRange}_{range.MaxYRange}]");
                    sb.Append($"[Range:{range.Accuracy}]");
                    sw.WriteLine(sb.ToString());
                    idx++;
                }
                #endregion

                #region Compare Data
                foreach (var item in Summary)
                {
                    sb.Clear();

                    bool isNG = item.CompJudge.Any(judge=> judge==false);
                    
                    sb.Append($" {RefLNCD}-{item.Name}_");
                    if (item.CTLNO != string.Empty)
                    {
                        sb.Append($"{item.CTLNO}_");
                    }

                    if(item.BasicCount>0)
                    {
                        sb.Append("(");
                        sb.Append($"Ref:100%");
                        if (item.CompCount.Count > 0) sb.Append(",");

                        for (int i = 0; i < item.CompCount.Count; i++)
                        {
                            string judge = item.CompJudge[i] == true ? "P" : "N";
                            sb.Append($"Case{i + 1}:{item.CompRate[i]:F0}%({judge})");
                            if (i != item.CompCount.Count - 1)
                                sb.Append(",");
                        }
                        sb.Append(")");
                    }
                    else
                    {
                        sb.Append(Language.judgeNoData);
                    }

                    sw.WriteLine(sb.ToString());
                }
                sw.WriteLine("");
                #endregion                
                sw.Close();
            }
        }
    }
}
