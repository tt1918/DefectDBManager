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
    public class PreprocLotFilter : PreprocLotDefault, IPreprocLot
    {
        public PreprocItem ProcData
        {
            get { return procData; }
            set { procData = value; }
        }
        private PreprocItem procData = new PreprocItem();

        public bool IsCSVMode { get; set; } = false;

        public PreprocLotFilter()
        {
            Init();
        }

        public PreprocLotFilter(string lotName, DBLotInfo.LotData dbResult, PreProcResultData fault)
        {
            Init();

            LotName = lotName;
            SetQueryResult(dbResult);
            SetFaultData(fault);
        }

        public override void ComparePosition()
        {
            double minX, maxX, minY, maxY;
            double posX, posY;

            // CSV MODE가 아니면 공정명을 할당해준다.
            if (IsCSVMode == false) ProcName = procData.Name;

            // 결점 데이터는 최종 데이터 기준으로 이전 데이터를 추가하는 방식을 취함
            MarkCompData compList = new MarkCompData();
            // 비교 공정 갯수 확인
            int lineCompCount = procData.Compare.Count;
            // 비교 영역 갯수
            int compCount = procData.CompRange.Count + 1;

            foreach (var item in FaultData.MarkData.Data.Data)
            {
                CompareResult comp = new CompareResult();
                comp.Base = item;
                comp.IdxSize = compCount;

                posX = item.XPOS_M;
                posY = item.OFFSET;

                #region 기준 검사 구역 
                int compIdx = 0;
                minX = procData.BasicRange.MinXRange;
                maxX = procData.BasicRange.MaxXRange;
                minY = procData.BasicRange.MinYRange;
                maxY = procData.BasicRange.MaxYRange;

                // Base는 인덱스 0번 사용
                List<MarkingFaultDatum> subData = null;
                foreach (var preItem in FaultData.PreMarkData)
                {
                    if (preItem.Count <= 0) continue;
                    foreach (var preItem1 in preItem)
                    {
                        int lncdIdx = 0;
                        foreach (var ccComp in procData.Compare)
                        {
                            if (preItem1.LNCD == ccComp.LNCD)
                                break;
                            lncdIdx++;
                        }

                        //CTLNO 같으면 예외처리 @ATW 250325
                        subData = preItem1.Data.FindAll(x => item.LNCD != x.LNCD &&
                                                        Math.Abs(x.XPOS_M - posX) < maxX && Math.Abs(x.XPOS_M - posX) >= minX &&
                                                        Math.Abs(x.OFFSET - posY) < maxY && Math.Abs(x.OFFSET - posY) >= minY
                                                        /*&& x.FAULTID == item.FAULTID*/); // 결점 ID가 같고 영역 내에 들어오는 경우

                        comp.AddCompData(preItem1.LNCD, preItem1.CTLNO, compIdx, subData);
                    }
                }
                #endregion

                #region 비교 검사 구역
                // 추가 비교 부터는 인덱스 1~N번 사용
                foreach (var compItem in procData.CompRange)
                {
                    compIdx++;

                    minX = procData.CompRange[compIdx - 1].MinXRange;
                    maxX = procData.CompRange[compIdx - 1].MaxXRange;
                    minY = procData.CompRange[compIdx - 1].MinYRange;
                    maxY = procData.CompRange[compIdx - 1].MaxYRange;

                    foreach (var preItem in FaultData.PreMarkData)
                    {
                        if (preItem.Count <= 0) continue;
                        foreach (var preItem1 in preItem)
                        {
                            int lncdIdx = 0;
                            foreach (var ccComp in procData.Compare)
                            {
                                if (preItem1.LNCD == ccComp.LNCD)
                                    break;
                                lncdIdx++;
                            }

                            //CTLNO 같으면 예외처리 @ATW 250325
                            subData = preItem1.Data.FindAll(x => item.LNCD != x.LNCD &&
                                                            Math.Abs(x.XPOS_M - posX) < maxX && Math.Abs(x.XPOS_M - posX) >= minX &&
                                                            Math.Abs(x.OFFSET - posY) < maxY && Math.Abs(x.OFFSET - posY) >= minY
                                                            /*&& x.FAULTID == item.FAULTID*/); // 결점 정보가 같고
                            comp.AddCompData(preItem1.LNCD, preItem1.CTLNO, compIdx, subData);
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
            CompRange basicRange = procData.BasicRange;
            List<CompRange> compRange = procData.CompRange;

            MarkCompList.SetCTLNOArray(procData.Compare.Count);
            CompCnt = new List<int[,]>();

            LotSummary.SetRange(procData);
            LotSummary.Name = LotName;
            LotSummary.Product = procData.Name;
            LotSummary.RefLNCD = FaultData.MarkData.LNCD;

            if (MarkCompList.Data.Count <= 0)
            {
                LotSummary.Judge = CompResult = eCompResult.NoCommPosData;

                for (int idx = 0; idx < procData.Compare.Count; idx++)
                {
                    CompSummary summary = new CompSummary();
                    summary.Name = procData.Compare[idx].LNCD;
                    LotSummary.Summary.Add(summary);
                    summary.BasicCount = 0;
                    for (int i = 0; i < procData.CompRange.Count; i++)
                    {
                        summary.CompCount.Add(0);
                        summary.CompRate.Add(0.0);
                        summary.CompJudge.Add(true);
                    }
                }
                return;
            }

            for (int i = 0; i < procData.Compare.Count; i++)
            {
                int count = 0;
                // 여기서 조건 분기
                if (procData.Compare[i].IsSplitCTLNO == true)
                {
                    if (MarkCompList.Data.Count != 0)
                    {
                        bool exists = MarkCompList.Data[0].Comp.Keys.Any(k => k.Item1 == procData.Compare[i].LNCD);

                        if (exists == true)
                        {
                            foreach (var compItem in MarkCompList.Data[0].Comp)
                            {
                                if (compItem.Key.Item1 == procData.Compare[i].LNCD)
                                {
                                    MarkCompList.CTLNO[i].Add(compItem.Key.Item2);
                                    count++;
                                }
                            }

                            if (count == 0) CompCnt.Add(new int[1, procData.CompRange.Count + 1]);
                            else // 여기서 데이터 추가함
                            {
                                CompCnt.Add(new int[count, procData.CompRange.Count + 1]);

                                count = 0;
                                int idx = CompCnt.Count - 1;
                                foreach (var compItem in MarkCompList.Data[0].Comp)
                                {
                                    if (compItem.Key.Item1 == procData.Compare[i].LNCD)
                                    {
                                        foreach (var item in MarkCompList.Data)
                                        {
                                            if (item.Comp[compItem.Key][0].Count > 0)    CompCnt[idx][count, 0]++;

                                            for (int j = 1; j < item.Comp[compItem.Key].GetLength(0); j++)
                                                if (item.Comp[compItem.Key][j].Count > 0) CompCnt[idx][count, j]++;
                                        }
                                        count++;
                                    }
                                }
                            }
                        }
                        else
                            CompCnt.Add(new int[1, procData.CompRange.Count + 1]);
                    }
                }
                else
                {
                    CompCnt.Add(new int[1, procData.CompRange.Count + 1]);
                    int idx = CompCnt.Count - 1;
                    foreach (var compItem in MarkCompList.Data[0].Comp)
                    {
                        if (compItem.Key.Item1 == procData.Compare[i].LNCD)
                        {
                            foreach (var item in MarkCompList.Data)
                            {
                                if (item.Comp[compItem.Key][0].Count > 0)
                                    CompCnt[idx][0, 0]++;

                                for (int j = 1; j < item.Comp[compItem.Key].GetLength(0); j++)
                                    if (item.Comp[compItem.Key][j].Count > 0) CompCnt[idx][0, j]++;
                            }
                        }
                    }
                }
            }

            #region 결점 다발 계산
            void AddBunchDefects(int[] target, int[] source)
            {
                for (int i = 0; i < target.Length; i++)
                    target[i] += source[i];
            }

            void CheckBunchDefects(CompSummary summary, int[] cntSum)
            {
                for (int i = 0; i < cntSum.Length; i++)
                {
                    if (cntSum[i] >= procData.BunchCount)
                    {
                        summary.IsBunchDefects = true;
                        break;
                    }
                }
            }
            #endregion

            bool isEmpty = true;
            bool isError = false;
            double[] result = new double[procData.CompRange.Count + 1];
            for (int idx = 0; idx < procData.Compare.Count; idx++)
            {
                isEmpty = true;
                if (procData.Compare[idx].IsSplitCTLNO != true)
                {
                    CompSummary summary = new CompSummary();
                    summary.Name = procData.Compare[idx].LNCD;
                    LotSummary.Summary.Add(summary);

                    #region 결점 다발 체크
                    int[] cntSum = new int[10000];
                    AddBunchDefects(cntSum, FaultData.DefectCnt1M);
                    
                    foreach (var line in FaultData.PreMarkData)
                    {
                        foreach (var item in line)
                        {
                            if (item.LNCD == procData.Compare[idx].LNCD)
                                AddBunchDefects(cntSum, item.DefectCnt1M);
                        }
                    }

                    CheckBunchDefects(summary, cntSum);
                    #endregion

                    for (int idx1 = 0; idx1 < CompCnt[idx].GetLength(0); idx1++)
                    {
                        isEmpty = true;
                        summary.BasicCount = CompCnt[idx][idx1, 0];

                        for (int i = 0; i < CompCnt[idx].GetLength(1); i++)
                        {
                            if (i > 0) summary.CompCount.Add(CompCnt[idx][idx1, i]);
                            if (CompCnt[idx][idx1, i] > 0) isEmpty = false;
                        }
                        if (isEmpty)
                        {
                            for (int i = 1; i < procData.CompRange.Count + 1; i++)
                            {
                                summary.CompRate.Add(0.0);
                                summary.CompJudge.Add(true);
                            }
                            continue;
                        }

                        result[0] = 100.0;
                        for (int i = 1; i < procData.CompRange.Count + 1; i++)
                        {
                            if (CompCnt[idx][idx1, 0] > 0)
                            {
                                result[i] = (double)((double)CompCnt[idx][idx1, i] / (double)CompCnt[idx][idx1, 0]) * 100.0;
                                summary.CompRate.Add(result[i]);
                            }
                            else
                            {
                                if (CompCnt[idx][idx1, i] > 0)
                                {
                                    result[i] = (double)CompCnt[idx][idx1, i] * 100.0;
                                    summary.CompRate.Add(result[i]);
                                }
                                else
                                    summary.CompRate.Add(0.0);
                            }
                            if (Math.Abs(result[0] - result[i]) > procData.CompRange[i - 1].Accuracy)
                            {
                                isError = true;
                                summary.CompJudge.Add(false);
                            }
                            else
                                summary.CompJudge.Add(true);
                        }
                    }
                }
                else
                {
                    for (int idx1 = 0; idx1 < CompCnt[idx].GetLength(0); idx1++)
                    {
                        CompSummary summary = new CompSummary();
                        summary.Name = procData.Compare[idx].LNCD;

                        LotSummary.Summary.Add(summary);
                        summary.BasicCount = CompCnt[idx][idx1, 0];
                        isEmpty = true;

                        if (MarkCompList.CTLNO[idx].Count >= idx1 + 1) summary.CTLNO = MarkCompList.CTLNO[idx][idx1];
                        else summary.CTLNO = string.Empty;

                        #region 결점 다발 체크
                        int[] cntSum = new int[10000];
                        AddBunchDefects(cntSum, FaultData.DefectCnt1M);

                        foreach (var line in FaultData.PreMarkData)
                        {
                            foreach (var item in line)
                            {
                                if (item.LNCD == procData.Compare[idx].LNCD && item.CTLNO == summary.CTLNO)
                                    AddBunchDefects(cntSum, item.DefectCnt1M);
                            }
                        }

                        CheckBunchDefects(summary, cntSum);
                        #endregion

                        for (int i = 0; i < CompCnt[idx].GetLength(1); i++)
                        {
                            if (i > 0) summary.CompCount.Add(CompCnt[idx][idx1, i]);
                            if (CompCnt[idx][idx1, i] > 0) isEmpty = false;
                        }

                        if (isEmpty)
                        {
                            for (int i = 1; i < procData.CompRange.Count + 1; i++)
                            {
                                summary.CompRate.Add(0.0);
                                summary.CompJudge.Add(true);
                            }
                            continue;
                        }

                        result[0] = 100.0;
                        for (int i = 1; i < procData.CompRange.Count + 1; i++)
                        {
                            if (CompCnt[idx][idx1, 0] > 0)
                            {
                                result[i] = (double)((double)CompCnt[idx][idx1, i] / (double)CompCnt[idx][idx1, 0]) * 100.0;
                                summary.CompRate.Add(result[i]);
                            }
                            else
                            {
                                if (CompCnt[idx][idx1, i] > 0)
                                {
                                    result[i] = (double)CompCnt[idx][idx1, i] * 100.0;
                                    summary.CompRate.Add(result[i]);
                                }
                                else
                                    summary.CompRate.Add(0.0);
                            }

                            if (Math.Abs(result[0] - result[i]) > procData.CompRange[i - 1].Accuracy)
                            {
                                isError = true;
                                summary.CompJudge.Add(false);
                            }
                            else
                                summary.CompJudge.Add(true);
                        }
                    }
                }
            }

            if (isError == false) LotSummary.Judge = CompResult = eCompResult.ProcOk;
            else LotSummary.Judge = CompResult = eCompResult.ProcNg;
        }
    }
}
