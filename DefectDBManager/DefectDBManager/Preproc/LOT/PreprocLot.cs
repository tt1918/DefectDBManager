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
        }

        /// <summary>
        /// 쿼리 데이터 카피
        /// </summary>
        /// <param name="result"></param>
        public void SetQueryResult(DBLotInfo.LotData result)
        {
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
            double limitMin, limitMax;
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

                posX = item.OFFSET;
                posY = item.XPOS_M;

                #region 기준 검사 구역 
                int compIdx = 0;
                limitMin = procData.BasicRange.MinRange;
                limitMax = procData.BasicRange.MaxRange;

                // Base는 인덱스 0번 사용
                List<MarkingFaultDatum> subData = null;
                int lineCnt = 0;
                foreach (var preItem in FaultData.PreMarkData)
                {
                    foreach( var preItem1 in preItem)
                    {
                        subData = preItem1.Data.FindAll(x => Math.Abs(x.XPOS_M - posX) < limitMax && Math.Abs(x.XPOS_M - posX) >= limitMin
                                                        && Math.Abs(x.OFFSET - posY) < limitMax && Math.Abs(x.OFFSET - posY) >= limitMin
                                                        && x.FAULTID == item.FAULTID); // 결점 ID가 같고 영역 내에 들어오는 경우

                        foreach (var preItem2 in subData)
                            comp.AddCompData(lineCnt, compIdx, preItem2);
                    }
                    lineCnt++;
                }
                #endregion

                #region 비교 검사 구역
                // 추가 비교 부터는 인덱스 1~N번 사용
                foreach (var compItem in procData.CompRange)
                {
                    compIdx++;

                    limitMin = procData.CompRange[compIdx-1].MinRange;
                    limitMax = procData.CompRange[compIdx-1].MaxRange;

                    lineCnt = 0;
                    foreach (var preItem in FaultData.PreMarkData)
                    {
                        foreach (var preItem1 in preItem)
                        {
                            subData = preItem1.Data.FindAll(x => Math.Abs(x.XPOS_M - posX) < limitMax && Math.Abs(x.XPOS_M - posX) >= limitMin
                                                            && Math.Abs(x.OFFSET - posY) < limitMax && Math.Abs(x.OFFSET - posY) >= limitMin
                                                            && x.FAULTID == item.FAULTID); // 결점 정보가 같고

                            foreach (var preItem2 in subData)
                                comp.AddCompData(lineCnt, compIdx, preItem2);
                        }
                        lineCnt++;
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

        }

    }
}
