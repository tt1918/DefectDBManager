using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager.DBLotInfo
{
    #region DB Search Result 
    using DicStrFloat = Dictionary<string, float>;
    using DicStrBool = Dictionary<string, bool>;
    using DicStrInt = Dictionary<string, int>;

    public class LotData
    {
        public DB.PTRYLPList PTRLYP;
        public DB.PTRY0PList[] PTRY0P;
        public DB.MRKCTLMSTList MRKCTLMST;
        public List<DB.INSPDATList>[] INSPDAT;
        public DB.XOFSMSTList XOFSMST;
        public DB.AREADELList AREADEL;

        public List<DicStrFloat>[] dicSizeMRKCTLMST;
        public List<DicStrBool>[] dicMRKF1MRKCTLMST;
        public List<DB.MRKCTLMST_DE_Data>[] _MRKCTLMST_DE;

        public DicStrFloat dicSizeData;
        public DicStrBool dicMRKF1Data;

        public DicStrInt DicCSVDefectCnt;

        public List<string> LoadedBcNo;

        public List<DateTime> ProductEndTime;
        public List<string> ProductLotName;


        // 현재 생산중인 Lot의 InspData
        public DB.INSPDATList[] INSPDATArray;

        public string SelectedDbLNCD = string.Empty;

        public LotData()
        {
            Init();
        }

        public void Init()
        {
            PTRLYP = new DB.PTRYLPList();
            XOFSMST = new DB.XOFSMSTList();
            AREADEL = new DB.AREADELList();

            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            PTRY0P = new DB.PTRY0PList[count];
            for (int i = 0; i < count; i++)
                PTRY0P[i] = new DB.PTRY0PList();

            INSPDAT = new List<DB.INSPDATList>[count];
            for (int i = 0; i < count; i++)
                INSPDAT[i] = new List<DB.INSPDATList>();

            MRKCTLMST = new DB.MRKCTLMSTList();

            dicSizeMRKCTLMST = new List<DicStrFloat>[count];
            dicMRKF1MRKCTLMST = new List<DicStrBool>[count];
            _MRKCTLMST_DE = new List<DB.MRKCTLMST_DE_Data>[count];
            for (int i = 0; i < count; i++)
            {
                dicSizeMRKCTLMST[i] = new List<DicStrFloat>();
                dicMRKF1MRKCTLMST[i] = new List<DicStrBool>();
                _MRKCTLMST_DE[i] = new List<DB.MRKCTLMST_DE_Data>();
            }

            dicSizeData = new DicStrFloat();
            dicMRKF1Data = new DicStrBool();

            LoadedBcNo = new List<string>();

            ProductEndTime = new List<DateTime>();
            ProductLotName = new List<string>();

            DicCSVDefectCnt = new DicStrInt();

            INSPDATArray = new DB.INSPDATList[count];
            for (int i = 0; i < count; i++)
                INSPDATArray[i] = new DB.INSPDATList();
        }

        public void ClearAll()
        {
            LoadedBcNo.Clear();
            AREADEL.Clear();
            XOFSMST.Clear();
            PTRLYP.Clear();
            MRKCTLMST.Clear();

            LoadedBcNo.Clear();

            ProductEndTime.Clear();
            ProductLotName.Clear();

            for (int i = 0; i < PTRY0P.Length; i++) PTRY0P[i].Clear();

            for (int i = 0; i < INSPDAT.Length; i++)
            {
                for (int j = 0; j < INSPDAT[i].Count; j++)
                    INSPDAT[i][j].Clear();
                INSPDAT[i].Clear();
            }

            DicCSVDefectCnt.Clear();


            for (int i = 0; i < INSPDATArray.Length; i++)
            {
                for (int j = 0; j < INSPDATArray[i].Count; j++)
                    INSPDATArray[i].Clear();
            }
        }

        public void ClearProductInfo()
        {
            LoadedBcNo.Clear();
            ProductEndTime.Clear();
            ProductLotName.Clear();
        }

        public void CheckAndUpdateProductInfo(int i, int j, DateTime edTime)
        {
            if (PTRY0P[i][j].Y0KLOT.Substring(0, 2).ToUpper() == "LL")
            {
                ProductEndTime.Add(edTime);
                ProductLotName.Add(PTRY0P[i][j].Y0KLOT);
            }
        }
        public bool IsProductAvaliable(Option opt, LogDB log)
        {
            DateTime dt = DateTime.Now;
            Debug.Assert(ProductEndTime.Count == ProductLotName.Count);
            string strE, strC, str;
            strC = dt.ToString("yyyyMMddHHmmdd");

            int count = 0;

            TimeSpan refTs = new TimeSpan(0, opt.prodAvaliableSpan, 0, 0);
            foreach (DateTime time in ProductEndTime)
            {
                TimeSpan sp = dt - time;
                strE = time.ToString("yyyyMMddHHmmdd");
                str = string.Format($"{opt.dbWhen.ToString()}, {ProductLotName[count]}, 현재시각:{strC}, " +
                    $"생산종료시각:{strE}, 설정시간:{opt.prodAvaliableSpan}, 차이시간:{sp.Hours}");
                log.WriteLoadData(str, 0, "PRODUCTION_ABLE", 0);
                count++;

                if (sp < refTs)
                {
                    log.WriteLoadData("생산가능시간 NG", 0, "PRODUCTION_ABLE", 0);
                    return false;
                }
            }
            return true;
        }

        public void ClearSplit()
        {
            PTRLYP.Clear();
            AREADEL.Clear();
            XOFSMST.Clear();
            MRKCTLMST.Clear();

            for (int i = 0; i < PTRY0P.Length; i++) PTRY0P[i].Clear();

            for (int i = 0; i < INSPDAT.Length; i++)
            {
                for (int j = 0; j < INSPDAT[i].Count; j++)
                    INSPDAT[i][j].Clear();
                INSPDAT[i].Clear();
            }
        }
        public void ResetData_DE()
        {
            int count = System.Enum.GetValues(typeof(eFCD)).Length;
            for (int i = 0; i < count; i++)
            {
                for (int j = 0; j < dicSizeMRKCTLMST[i].Count; j++)
                    dicSizeMRKCTLMST[i][j].Clear();

                for (int j = 0; j < dicMRKF1MRKCTLMST[i].Count; j++)
                    dicMRKF1MRKCTLMST[i][j].Clear();

                _MRKCTLMST_DE[i].Clear();
            }
        }

        public void ClearDicMRKCTLMST(int fdIdx, int ptryoIdx)
        {
            dicSizeMRKCTLMST[fdIdx][ptryoIdx].Clear();
            dicMRKF1MRKCTLMST[fdIdx][ptryoIdx].Clear();
        }
        public void CheckDicMRKCTLMSTSize(int targetCnt, int fcdIdx)
        {
            // Dic 부족한 인덱스 만큼 초기화 처리
            if (targetCnt > dicSizeMRKCTLMST[fcdIdx].Count)
                for (int dicIdx = dicSizeMRKCTLMST[fcdIdx].Count; dicIdx < targetCnt; dicIdx++)
                    dicSizeMRKCTLMST[fcdIdx].Add(new DicStrFloat());

            if (targetCnt > dicMRKF1MRKCTLMST[fcdIdx].Count)
                for (int dicIdx = dicMRKF1MRKCTLMST[fcdIdx].Count; dicIdx < targetCnt; dicIdx++)
                    dicMRKF1MRKCTLMST[fcdIdx].Add(new DicStrBool());
        }

        public void AddDicMRKCTLMST(int fdIdx, int ptryoIdx, DB.MRKCTLMSTData data)
        {
            if (dicSizeMRKCTLMST[fdIdx][ptryoIdx].ContainsKey(data.FLTID) == true)
                dicSizeMRKCTLMST[fdIdx][ptryoIdx][data.FLTID] = data.SIZE;
            else
                dicSizeMRKCTLMST[fdIdx][ptryoIdx].Add(data.FLTID, data.SIZE);

            int valMRKF1 = Int32.Parse(data.MRKF1);
            bool boolMRKF1 = false;
            if (valMRKF1 == 1) boolMRKF1 = true;
            if (dicMRKF1MRKCTLMST[fdIdx][ptryoIdx].ContainsKey(data.FLTID) == true)
                dicMRKF1MRKCTLMST[fdIdx][ptryoIdx][data.FLTID] = boolMRKF1;
            else
                dicMRKF1MRKCTLMST[fdIdx][ptryoIdx].Add(data.FLTID, boolMRKF1);
        }

        public void UpdateDicSizeData(int fcd, int op)
        {
            dicSizeData.Clear();
            foreach (KeyValuePair<string, float> pair in dicSizeMRKCTLMST[fcd][op])
                dicSizeData.Add(pair.Key, pair.Value);
        }

        public void UpdateDicMRKF1Data(int fcd, int op)
        {
            dicMRKF1Data.Clear();
            foreach (KeyValuePair<string, bool> pair in dicMRKF1MRKCTLMST[fcd][op])
                dicMRKF1Data.Add(pair.Key, pair.Value);
        }

        public bool CheckValidSize(string key, float size)
        {
            bool bValid = false;
            if (dicSizeData.ContainsKey(key) == true && dicMRKF1Data.ContainsKey(key) == true)
            {
                // 원래는 사이즈 비교만 하였으나 Defect Edit 시 버퍼 내용에서 마킹 체크가 변경이 가능하기 때문에 
                // MRKF1도 확인해서 비교하도록 해야함. 
                if (dicSizeData[key] <= (size + 0.00001f) && dicMRKF1Data[key] == true)
                    bValid = true;
            }

            return bValid;
        }
    }
    #endregion
}
