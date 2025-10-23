using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DefectDBManager.DB;
using DefectDBManager.Preproc;
using DefectDBManager.Preproc.LOT;

namespace DefectDBManager
{
    public class PreprocLotDB : PreprocLotDefault, IPreprocLot
    {
        public LotSelProcParam Param
        {
            get { return param; }
            set { param = value; }
        }
        private LotSelProcParam param = new LotSelProcParam();
        public string RefLNCD
        {
            get { return refLNCD; }
            set { refLNCD = value; }
        }
        private string refLNCD = string.Empty;
        public List<string> CompLNCD
        {
            get { return compLNCD; }
            set { compLNCD = value; }
        }
        private List<string> compLNCD = new List<string>();

        public PreprocLotDB()
        {
            Init();
        }

        public PreprocLotDB(string lotName, DBLotInfo.LotData dbResult, PreProcResultData fault)
        {
            Init();

            LotName = lotName;
            SetQueryResult(dbResult);
            SetFaultData(fault);
        }

        public void SetInfo(LotSelProcParam param, string refLNCD, List<string> compLNCD)
        {
            this.param = param;
            this.refLNCD = refLNCD;
            this.compLNCD = compLNCD;
        }

        public override void ComparePosition()
        {
            double minX, maxX, minY, maxY;
            double posX, posY;

            // 결점 데이터는 최종 데이터 기준으로 이전 데이터를 추가하는 방식을 취함
            MarkCompData compList = new MarkCompData();

            // 비교 공정 갯수 확인
            int lineCompCount = compLNCD.Count;

            // 비교 영역 갯수
            int compCount = param.CompRange.Count + 1;

            foreach (var item in FaultData.MarkData.Data.Data)
            {
                CompareResult comp = new CompareResult();
                comp.Base = item;
                comp.IdxSize = compCount;

                posX = item.XPOS_M;
                posY = item.OFFSET;

                #region 기준 검사 구역 
                int compIdx = 0;
                minX = param.BasicRange.MinXRange;
                maxX = param.BasicRange.MaxXRange;
                minY = param.BasicRange.MinYRange;
                maxY = param.BasicRange.MaxYRange;

                // Base는 인덱스 0번 사용
                List<MarkingFaultDatum> subData = null;
                foreach (var preItem in FaultData.PreMarkData)
                {
                    if (preItem.Count <= 0) continue;
                    foreach (var preItem1 in preItem)
                    {
                        int lncdIdx = 0;
                        foreach (var ccComp in compLNCD)
                        {
                            if (preItem1.LNCD == ccComp)
                                break;
                            lncdIdx++;
                        }

                        //CTLNO 같으면 예외처리 @ATW 250325
                        subData = preItem1.Data.FindAll(x => Math.Abs(x.XPOS_M - posX) < maxX && Math.Abs(x.XPOS_M - posX) >= minX &&
                                                        Math.Abs(x.OFFSET - posY) < maxY && Math.Abs(x.OFFSET - posY) >= minY
                                                        /*&& x.FAULTID == item.FAULTID*/); // 결점 ID가 같고 영역 내에 들어오는 경우

                        comp.AddComp1Data(preItem1.LNCD, preItem1.CTLNO, compIdx, subData);
                    }
                }
                #endregion

                #region 비교 검사 구역
                // 추가 비교 부터는 인덱스 1~N번 사용
                foreach (var compItem in param.CompRange)
                {
                    compIdx++;

                    minX = param.CompRange[compIdx - 1].MinXRange;
                    maxX = param.CompRange[compIdx - 1].MaxXRange;
                    minY = param.CompRange[compIdx - 1].MinYRange;
                    maxY = param.CompRange[compIdx - 1].MaxYRange;

                    foreach (var preItem in FaultData.PreMarkData)
                    {
                        if (preItem.Count <= 0) continue;
                        foreach (var preItem1 in preItem)
                        {
                            int lncdIdx = 0;
                            foreach (var ccComp in compLNCD)
                            {
                                if (preItem1.LNCD == ccComp)
                                    break;
                                lncdIdx++;
                            }

                            //CTLNO 같으면 예외처리 @ATW 250325
                            subData = preItem1.Data.FindAll(x => Math.Abs(x.XPOS_M - posX) < maxX && Math.Abs(x.XPOS_M - posX) >= minX &&
                                                            Math.Abs(x.OFFSET - posY) < maxY && Math.Abs(x.OFFSET - posY) >= minY
                                                            /*&& x.FAULTID == item.FAULTID*/); // 결점 정보가 같고

                            comp.AddComp1Data(preItem1.LNCD, preItem1.CTLNO, compIdx, subData);
                        }
                    }
                }
                #endregion

                // 최상위 데이터에 추가된 데이터 입력함.
                compList.Add(comp);
            }

            MarkCompList = compList;
            // 에러율 처리
            virify();
        }

        void virify()
        {
            CompRange basicRange = param.BasicRange;
            List<CompRange> compRange = param.CompRange;

            LotSummary.SetRange(param);
            LotSummary.Name = LotName;
            LotSummary.Product = param.DBFilter.Title;
            LotSummary.RefLNCD = refLNCD;

            if (MarkCompList.Data.Count <= 0)
            {
                LotSummary.Judge = CompResult = eCompResult.NoCommPosData;

                for (int idx = 0; idx < compLNCD.Count; idx++)
                {
                    CompSummary summary = new CompSummary();
                    summary.Name = compLNCD[idx];
                    LotSummary.Summary.Add(summary);
                    summary.BasicCount = 0;
                    for (int i = 0; i < param.CompRange.Count; i++)
                    {
                        summary.CompCount.Add(0);
                        summary.CompRate.Add(0.0);
                        summary.CompJudge.Add(true);
                    }
                }
                return;
            }

            MarkCompList.SetCTLNOArray(compLNCD.Count);
            Comp1Cnt = new List<int[,]>();
            for (int i = 0; i < compLNCD.Count; i++)
            {
                if (param.UseSplit == true)
                {
                    int count = 0;
                    // 여기서 조건 분기
                    if (MarkCompList.Data.Count != 0)
                    {
                        bool exists = MarkCompList.Data[0].Comp1.Keys.Any(k => k.Item1 == compLNCD[i]);
                        if (exists == true)
                        {
                            foreach (var aaa in MarkCompList.Data[0].Comp1)
                            {
                                if (aaa.Key.Item1 == compLNCD[i])
                                {
                                    MarkCompList.CTLNO[i].Add(aaa.Key.Item2);
                                    count++;
                                }
                            }
                            if (count == 0) Comp1Cnt.Add(new int[1, param.CompRange.Count + 1]);
                            else // 여기서 데이터 추가함
                            {
                                Comp1Cnt.Add(new int[count, param.CompRange.Count + 1]);
                                count = 0;
                                int idx = Comp1Cnt.Count - 1;
                                foreach (var aaa in MarkCompList.Data[0].Comp1)
                                {
                                    if (aaa.Key.Item1 == compLNCD[i])
                                    {
                                        foreach (var item in MarkCompList.Data)
                                        {
                                            if (item.Comp1[aaa.Key][0].Count > 0)
                                                Comp1Cnt[idx][count, 0]++;
                                            for (int j = 1; j < item.Comp1[aaa.Key].GetLength(0); j++)
                                                if (item.Comp1[aaa.Key][j].Count > 0) Comp1Cnt[idx][count, j]++;
                                        }
                                        count++;
                                    }
                                }
                            }
                        }
                        else
                            Comp1Cnt.Add(new int[1, param.CompRange.Count + 1]);
                    }
                }
                else
                {
                    Comp1Cnt.Add(new int[1, param.CompRange.Count + 1]);
                    int idx = Comp1Cnt.Count - 1;
                    foreach (var aaa in MarkCompList.Data[0].Comp1)
                    {
                        if (aaa.Key.Item1 == compLNCD[i])
                        {
                            foreach (var item in MarkCompList.Data)
                            {
                                if (item.Comp1[aaa.Key][0].Count > 0)
                                    Comp1Cnt[idx][0, 0]++;

                                for (int j = 1; j < item.Comp1[aaa.Key].GetLength(0); j++)
                                    if (item.Comp1[aaa.Key][j].Count > 0) Comp1Cnt[idx][0, j]++;
                            }
                        }
                    }
                }
            }

            bool isEmpty = true;
            bool isError = false;
            double[] result = new double[param.CompRange.Count + 1];
            for (int idx = 0; idx < compLNCD.Count; idx++)
            {
                isEmpty = true;

                if (param.UseSplit != true)
                {
                    CompSummary summary = new CompSummary();
                    summary.Name = compLNCD[idx];
                    LotSummary.Summary.Add(summary);

                    for (int idx1 = 0; idx1 < Comp1Cnt[idx].GetLength(0); idx1++)
                    {
                        isEmpty = true;
                        summary.BasicCount = Comp1Cnt[idx][idx1, 0];

                        for (int i = 0; i < Comp1Cnt[idx].GetLength(1); i++)
                        {
                            if (i > 0) summary.CompCount.Add(Comp1Cnt[idx][idx1, i]);
                            if (Comp1Cnt[idx][idx1, i] > 0) isEmpty = false;
                        }
                        if (isEmpty)
                        {
                            summary.CompRate.Add(0.0);
                            summary.CompJudge.Add(true);
                            continue;
                        }

                        result[0] = 100.0;

                        for (int i = 1; i < param.CompRange.Count + 1; i++)
                        {
                            if (Comp1Cnt[idx][idx1, 0] > 0)
                            {
                                result[i] = (double)((double)Comp1Cnt[idx][idx1, i] / (double)Comp1Cnt[idx][idx1, 0]) * 100.0;
                                summary.CompRate.Add(result[i]);
                            }
                            else
                            {
                                if (Comp1Cnt[idx][idx1, i] > 0)
                                {
                                    result[i] = (double)Comp1Cnt[idx][idx1, i] * 100.0;
                                    summary.CompRate.Add(result[i]);
                                }
                                else
                                {
                                    summary.CompRate.Add(0.0);
                                }
                            }
                            if (Math.Abs(result[0] - result[i]) > param.CompRange[i - 1].Accuracy)
                            {
                                isError = true;
                                summary.CompJudge.Add(false);
                            }
                            else
                            {
                                summary.CompJudge.Add(true);
                            }
                        }
                    }
                }
                else
                {
                    for (int idx1 = 0; idx1 < Comp1Cnt[idx].GetLength(0); idx1++)
                    {
                        CompSummary summary = new CompSummary();
                        summary.Name = compLNCD[idx];

                        if (MarkCompList.CTLNO[idx].Count >= idx1 + 1) summary.CTLNO = MarkCompList.CTLNO[idx][idx1];
                        else summary.CTLNO = string.Empty;

                        LotSummary.Summary.Add(summary);
                        summary.BasicCount = Comp1Cnt[idx][idx1, 0];
                        isEmpty = true;
                        for (int i = 0; i < Comp1Cnt[idx].GetLength(1); i++)
                        {
                            if (i > 0) summary.CompCount.Add(Comp1Cnt[idx][idx1, i]);
                            if (Comp1Cnt[idx][idx1, i] > 0) isEmpty = false;
                        }
                        if (isEmpty)
                        {
                            summary.CompRate.Add(0.0);
                            summary.CompJudge.Add(true);
                            continue;
                        }
                        result[0] = 100.0;
                        for (int i = 1; i < param.CompRange.Count + 1; i++)
                        {
                            if (Comp1Cnt[idx][idx1, 0] > 0)
                            {
                                result[i] = (double)((double)Comp1Cnt[idx][idx1, i] / (double)Comp1Cnt[idx][idx1, 0]) * 100.0;
                                summary.CompRate.Add(result[i]);
                            }
                            else
                            {
                                if (Comp1Cnt[idx][idx1, i] > 0)
                                {
                                    result[i] = (double)Comp1Cnt[idx][idx1, i] * 100.0;
                                    summary.CompRate.Add(result[i]);
                                }
                                else
                                {
                                    summary.CompRate.Add(0.0);
                                }
                            }
                            if (Math.Abs(result[0] - result[i]) > param.CompRange[i - 1].Accuracy)
                            {
                                isError = true;
                                summary.CompJudge.Add(false);
                            }
                            else
                            {
                                summary.CompJudge.Add(true);
                            }
                        }
                    }
                }
            }
            if (isError == false) LotSummary.Judge = CompResult = eCompResult.ProcOk;
            else LotSummary.Judge = CompResult = eCompResult.ProcNg;
        }
    }
}
