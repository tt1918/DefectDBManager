using DefectDBManager.DB;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace DefectDBManager
{
    // 이전 공정 랏 데이터에 대한 
    public class PreprocLotManager
    {
        #region Param
        #region LOT 정보
        /// <summary>
        /// 각 공정별 검사 결과 데이터 저장
        /// string : 각 공정 데이터
        /// List<PreprocLog> : Lot 정보
        /// </summary>
        public Dictionary<string, List<PreprocLot>> LOT
        {
            get { return _lot; }
            private set { _lot = value; }
        }
        private Dictionary<string, List<PreprocLot>> _lot = null;

        public int TotalLot
        {
            get
            {
                int val = 0;
                foreach (var list in _lot)
                    val += list.Value.Count;
                return val;
            }
        }

        /// <summary>
        /// 공정 생산 정보를 저장
        /// </summary>
        public Dictionary<string, DB.PTRY0PList> Product
        {
            get { return _product; }
            private set { _product = value; }
        }

        private Dictionary<string, DB.PTRY0PList> _product = null;

        public int TotalProduct
        {
            get
            {
                int val = 0;
                foreach (var list in _product)
                    val += list.Value.Count;
                return val;
            }
        }
        #endregion LOT 정보

        #region 공정 정보 
        public Preproc.PreprocLNCD ProcLNCD
        {
            get { return _procLNCD; }
            private set { _procLNCD = value; }
        }
        public Preproc.PreprocLNCD _procLNCD = null;
        #endregion 공정 정보

        public bool UseMrkctlmstModel { get; set; }

        #endregion

        public PreprocLotManager()
        {
            _lot = new Dictionary<string, List<PreprocLot>>();
            _procLNCD = new Preproc.PreprocLNCD();

            LoadProcLNCD();
        }

        public DateTime StartTime { get; set; } = DateTime.Now;
        public DateTime EndTime { get; set; } = DateTime.Now;

        #region Lot 데이터 관리
        /// <summary>
        /// 랏관리 데이터에 신규 랏 정보를 추가
        /// </summary>
        /// <param name="lncd">라인 코드</param>
        /// <param name="info">불량 랏 정보</param>
        public void AddLot(string lncd, PreprocLot info)
        {
            if (LOT.ContainsKey(lncd) == true)
                LOT[lncd].Add(info);
            else
            {
                List<PreprocLot> list = new List<PreprocLot>();
                list.Add(info);
                LOT.Add(lncd, list);
            }
        }

        /// <summary>
        /// 선택한 랏 이름을 삭제한다.
        /// </summary>
        /// <param name="lncd">라인 코드</param>
        /// <param name="lotName">랏 이름</param>
        /// <returns></returns>
        public bool DeleteLot(string lncd, string lotName)
        {
            bool isSuccess = true;
            if (LOT.ContainsKey(lncd) == true)
            {
                foreach (PreprocLot info in LOT[lncd])
                {
                    // 데이터 삭제
                    if(info.LotName == lotName)     LOT[lncd].Remove(info);
                }
            }
            else    isSuccess = false;
            return isSuccess;
        }

        public void ClearLot()
        {
            // 각 공정 별 랏 정보 삭제
            foreach(var lot in LOT)
                lot.Value.Clear();

            // Dictionary 삭제
            LOT.Clear();
        }

        public int ProductSize(string key)
        {
            if (Product.ContainsKey(key) == true)
                return Product[key].Count;
            else
                return 0;
        }

        public int LotSize(string key)
        {
            if (LOT.ContainsKey(key) == true)
                return LOT[key].Count;
            else
                return 0;
        }
        #endregion


        #region 데이터 검색에서 사용할 라인코드와 검색 여부 처리
        /// <summary>
        /// 
        /// </summary>
        public void LoadProcLNCD()
        {
            ProcLNCD = new Preproc.PreprocLNCD();
            ProcLNCD.Load();
        }

        public void SetUse(string name, bool use)
        {
            for(int i=0; i< ProcLNCD.Info.Count; i++)
            {
                if (ProcLNCD.Info[i].Name == name)
                    ProcLNCD.Info[i].Use = use;
            }
        }

        public void SetUse(bool[] use)
        {
            for (int i = 0; i < ProcLNCD.Info.Count; i++)
            {
                if(use.Length > i)
                    ProcLNCD.Info[i].Use = use[i];
            }
        }
        #endregion
    }
}
