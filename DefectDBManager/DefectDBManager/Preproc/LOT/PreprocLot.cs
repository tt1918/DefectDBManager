using DefectDBManager.DB;
using DefectDBManager.Preproc;
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
        public int[,] CompCnt { get; private set; }

        public List<int[,]> Comp1Cnt { get; private set; }

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
            CompCnt = null;
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
                comp.SetCompRange(lineCompCount, compCount);

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

                        foreach (var preItem2 in subData)
                            comp.AddCompData(lncdIdx, compIdx, preItem2);
                            
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

                            foreach (var preItem2 in subData)
                                comp.AddCompData(lncdIdx, compIdx, preItem2);

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
                comp.SetCompRange(lineCompCount, compCount);

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

                        foreach (var preItem2 in subData)
                            comp.AddCompData(lncdIdx, compIdx, preItem2);
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

                            foreach (var preItem2 in subData)
                                comp.AddCompData(lncdIdx, compIdx, preItem2);
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

        /// <summary>
        /// 비교 데이터 정리하여 에러율 처리
        /// </summary>
        /// <param name="procData"></param>
        public void CheckComparision(PreprocItem procData)
        {
            CompRange basicRange = procData.BasicRange;
            List<CompRange> compRange = procData.CompRange;

            if(MarkCompList.Data.Count<=0)
            {
                CompResult = eCompResult.NoDbData;
                return;
            }

            MarkCompList.SetCTLNOArray(procData.Compare.Count);

            CompCnt = new int[procData.Compare.Count, procData.CompRange.Count + 1];

            Comp1Cnt = new List<int[,]>();
            for(int i=0; i< procData.Compare.Count; i++)
            {
                int count = 0;
                // 여기서 조건 분기
                if (procData.Compare[i].IsSplitCTLNO==true)
                {
                    if(MarkCompList.Data.Count!=0)
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
                                int iter = 0;
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
                                                if (item.Comp1[aaa.Key][j].Count > 0) Comp1Cnt[idx][count, j]++; ;
                                        }
                                        count++;
                                    }
                                    iter++;
                                }
                            }
                        }
                        else
                            Comp1Cnt.Add(new int[1, procData.CompRange.Count + 1]);
                    }
                }
                else
                    Comp1Cnt.Add(new int[1, procData.CompRange.Count + 1]);
            }

            foreach (var item in MarkCompList.Data)
            {
                for (int i = 0; i < item.Comp.GetLength(0); i++)
                {
                    if (item.Comp[i, 0].Count > 0) CompCnt[i, 0]++;

                    for (int j = 1; j < item.Comp.GetLength(1); j++)
                        if (item.Comp[i, j].Count > 0) CompCnt[i, j]++;
                }
            }

            bool isEmpty = true;
            foreach (var cnt in CompCnt)
                if (cnt > 0) isEmpty = false;
           
            if (isEmpty)
            {
                // 데이터 처리 과정 필요함
                CompResult = eCompResult.NoCommPosData;
                return;
            }

            bool isError = false;
            double[] result = new double[procData.CompRange.Count + 1];
            for (int idx = 0; idx < procData.Compare.Count; idx++)
            {
                isEmpty = true;
                if (procData.Compare[idx].IsSplitCTLNO != true)
                {
                    for (int i = 0; i < CompCnt.GetLength(1); i++)
                        if (CompCnt[idx, i] > 0) isEmpty = false;
                    if (isEmpty) continue;

                    result[0] = 100.0;
                    for (int i = 1; i < procData.CompRange.Count + 1; i++)
                    {
                        if (CompCnt[idx, 0] > 0)
                            result[i] = (double)((double)CompCnt[idx, i] / (double)CompCnt[idx, 0]) * 100.0;
                        else
                        {
                            if (CompCnt[idx, i] > 0)
                                result[i] = (double)CompCnt[idx, i] * 100.0;
                        }

                        if (Math.Abs(result[0] - result[i]) > procData.CompRange[i - 1].Accuracy)
                            isError = true;
                    }
                }
                else
                {
                    for(int idx1 = 0; idx1< Comp1Cnt[idx].GetLength(0); idx1++)
                    {
                        isEmpty = true;
                        for (int i = 0; i < Comp1Cnt[idx].GetLength(1); i++)
                            if (Comp1Cnt[idx][idx1, i] > 0) isEmpty = false;
                        if(isEmpty) continue;

                        result[0] = 100.0;
                        for(int i=1; i<procData.CompRange.Count + 1;i++)
                        {
                            if (Comp1Cnt[idx][idx1, 0] > 0)
                                result[i] = (double)((double)Comp1Cnt[idx][idx1, i] / (double)Comp1Cnt[idx][idx1, 0]) * 100.0;
                            else
                            {
                                if (Comp1Cnt[idx][idx1, i] > 0)
                                    result[i] = (double)Comp1Cnt[idx][idx1, i] * 100.0;
                            }
                            if (Math.Abs(result[0] - result[i]) > procData.CompRange[i - 1].Accuracy)
                                isError = true;
                        }
                    }
                }
            }

            if (isError == false) CompResult = eCompResult.ProcOk;
            else CompResult = eCompResult.ProcNg;
        }

    }
}
