using DefectDBManager.DB;
using DefectDBManager.Preproc;
using DefectDBManager.Preproc.LOT;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    /// <summary>
    /// 이전 결점 정보 취합 
    /// 서버에서 데이터 참조할 타겟
    /// </summary>
    public class PreprocLot
    {
        /// <summary>
        /// Lot 이름
        /// </summary>
        public string LotName { get; set; }

        public string ProcName { get; set; }

        public PTRYLPList PTRLYP_Data { get; private set; }
        public XOFSMSTList XOFSMST_Data { get; private set; }
        public PTRY0PList[] PTRY0P_Data { get; private set; }
        public List<INSPDATList>[] INSPDAT { get; private set; }

        /// <summary>
        /// 결점 정보 데이터
        /// </summary>
        public PreProcResultData FaultData { get; private set; }

        /// <summary>
        /// 마킹 오차 비교 데이터
        /// </summary>
        public MarkCompData MarkCompList { get; set; } = null;

        public eCompResult CompResult { get; private set; }

        public List<int[,]> Comp1Cnt { get; private set; }

        public Preproc.LOT.CompResult LotSummary { get; private set; } = null;

        /// <summary>
        /// 초기화
        /// </summary>
        public PreprocLot()
        {
            Init();
        }

        /// <summary>
        /// 데이터 초기화
        /// </summary>
        /// <param name="lotName"></param>
        /// <param name="dbResult"></param>
        /// <param name="fault"></param>
        public PreprocLot(string lotName, DBLotInfo.LotData dbResult, PreProcResultData fault)
        {
            Init();

            LotName = lotName;
            SetQueryResult(dbResult);
            SetFaultData(fault);
        }

        public void Init()
        {
            PTRLYP_Data = new PTRYLPList();

            XOFSMST_Data = new XOFSMSTList();

            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            PTRY0P_Data = new PTRY0PList[count];
            for (int i = 0; i < count; i++)
                PTRY0P_Data[i] = new PTRY0PList();

            INSPDAT = new List<INSPDATList>[count];
            for (int i = 0; i < count; i++)
                INSPDAT[i] = new List<INSPDATList>();

            CompResult = eCompResult.None;

            LotSummary = new Preproc.LOT.CompResult();
        }

        /// <summary>
        /// 쿼리 데이터 카피
        /// </summary>
        /// <param name="result"></param>
        public void SetQueryResult(DBLotInfo.LotData result)
        {
            if (result == null) return;
            PTRLYP_Data.Copy(result.PTRLYP);

            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            for (int i = 0; i < count; i++)
            {
                foreach (var data in result.PTRY0P[i].Data)
                    PTRY0P_Data[i].Add(data.Clone());
            }

            for(int i=0; i<count; i++)
            {
                foreach(var data in result.INSPDAT)
                {
                    List<INSPDATList> inspList = new List<INSPDATList>(); 
                    foreach(var data1 in data)
                    {
                        INSPDATList inspList1 = new INSPDATList();
                        foreach(var data2 in data1.Data)
                            inspList1.Add(data2.Clone());
                        inspList.Add(inspList1);
                    }
                    INSPDAT[i] = inspList;
                }
            }

        }

        public void SetFaultData(PreProcResultData data)
        {
            FaultData = data.Copy();
        }

        public void ComparePosition(PreprocItem procData)
        {
            double minX, maxX, minY, maxY;
            double posX, posY;


            ProcName = procData.Name;

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
                    foreach( var preItem1 in preItem)
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
                        comp.AddComp1Data(preItem1.LNCD, preItem1.CTLNO, compIdx, subData);
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
                            foreach(var ccComp in procData.Compare)
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
            CheckComparision(procData);
        }

        public void CompareCsvPos(PreprocItem procData)
        {
            double minX, maxX, minY, maxY;
            double posX, posY;

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
                        subData = preItem1.Data.FindAll(x => Math.Abs(x.XPOS_M - posX) < maxX && Math.Abs(x.XPOS_M - posX) >= minX &&
                                                        Math.Abs(x.OFFSET - posY) < maxY && Math.Abs(x.OFFSET - posY) >= minY
                                                        /*&& x.FAULTID == item.FAULTID*/); // 결점 ID가 같고 영역 내에 들어오는 경우

                        comp.AddComp1Data(preItem1.LNCD, preItem1.CTLNO, compIdx, subData);
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
            CheckComparision(procData);
        }

        public void CompareDBPos(LotSelProcParam param, string refLNCD, List<string> compLNCD)
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

                            //foreach (var preItem2 in subData)
                            //    comp.AddCompData(lncdIdx, compIdx, preItem2);
                        }
                    }
                }
                #endregion

                // 최상위 데이터에 추가된 데이터 입력함.
                compList.Add(comp);
            }

            MarkCompList = compList;
            // 에러율 처리
            CheckComparision(param, refLNCD, compLNCD);
        }

        /// <summary>
        /// 비교 데이터 정리하여 에러율 처리
        /// </summary>
        /// <param name="procData"></param>
        public void CheckComparision(PreprocItem procData)
        {
            CompRange basicRange = procData.BasicRange;
            List<CompRange> compRange = procData.CompRange;

            MarkCompList.SetCTLNOArray(procData.Compare.Count);
            Comp1Cnt = new List<int[,]>();

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
                        bool exists = MarkCompList.Data[0].Comp1.Keys.Any(k => k.Item1 == procData.Compare[i].LNCD);

                        if (exists == true)
                        {
                            foreach (var aaa in MarkCompList.Data[0].Comp1)
                            {
                                if (aaa.Key.Item1 == procData.Compare[i].LNCD)
                                {
                                    MarkCompList.CTLNO[i].Add(aaa.Key.Item2);
                                    count++;
                                }
                            }

                            if (count == 0) Comp1Cnt.Add(new int[1, procData.CompRange.Count + 1]);
                            else // 여기서 데이터 추가함
                            {
                                Comp1Cnt.Add(new int[count, procData.CompRange.Count + 1]);

                                count = 0;
                                int idx = Comp1Cnt.Count - 1;
                                foreach (var aaa in MarkCompList.Data[0].Comp1)
                                {
                                    if (aaa.Key.Item1 == procData.Compare[i].LNCD)
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
                            Comp1Cnt.Add(new int[1, procData.CompRange.Count + 1]);
                    }
                }
                else
                {
                    Comp1Cnt.Add(new int[1, procData.CompRange.Count + 1]);
                    int idx = Comp1Cnt.Count - 1;
                    foreach (var aaa in MarkCompList.Data[0].Comp1)
                    {
                        if (aaa.Key.Item1 == procData.Compare[i].LNCD)
                        {
                            foreach (var item in MarkCompList.Data)
                            {
                                if (item.Comp1[aaa.Key][0].Count > 0)
                                    Comp1Cnt[idx][0, 0]++;

                                for (int j = 1; j < item.Comp1[aaa.Key].GetLength(0); j++)
                                    if (item.Comp1[aaa.Key][j].Count > 0)   Comp1Cnt[idx][0, j]++;
                            }
                        }
                    }
                }
            }

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
                            if (Math.Abs(result[0] - result[i]) > procData.CompRange[i - 1].Accuracy)
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
                        summary.Name = procData.Compare[idx].LNCD;
                        
                        LotSummary.Summary.Add(summary);
                        summary.BasicCount = Comp1Cnt[idx][idx1, 0];
                        isEmpty = true;

                        if (MarkCompList.CTLNO[idx].Count >= idx1 + 1)  summary.CTLNO = MarkCompList.CTLNO[idx][idx1];
                        else                                            summary.CTLNO = string.Empty;

                        for (int i = 0; i < Comp1Cnt[idx].GetLength(1); i++)
                        {
                            if (i > 0) summary.CompCount.Add(Comp1Cnt[idx][idx1, i]);
                            if (Comp1Cnt[idx][idx1, i] > 0) isEmpty = false;
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
                            if (Math.Abs(result[0] - result[i]) > procData.CompRange[i - 1].Accuracy)
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

        public void CheckComparision(LotSelProcParam param, string refLNCD, List<string> compLNCD)
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

                        if (MarkCompList.CTLNO[idx].Count >= idx1 + 1)  summary.CTLNO = MarkCompList.CTLNO[idx][idx1];
                        else                                            summary.CTLNO = string.Empty;

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
            if (isError == false) LotSummary.Judge=CompResult = eCompResult.ProcOk;
            else LotSummary.Judge = CompResult = eCompResult.ProcNg;
        }
    }
}
