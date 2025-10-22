using DefectDBManager.DB;
using DefectDBManager.Preproc;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    public class CompLotList
    {
        /// <summary>
        /// 데이터 비교 결과 리스트
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
                if (_lot == null) return 0;

                int val = 0;
                foreach (var list in _lot)
                    val += list.Value.Count;
                return val;
            }
        }

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
                if (_product == null) return 0;
                int val = 0;
                foreach (var list in _product)
                    val += list.Value.Count;
                return val;
            }
        }
        public Preproc.LotHistory LotHistory
        {
            get { return _lotHistroy; }
            private set { _lotHistroy = value; }
        }
        private Preproc.LotHistory _lotHistroy = null;

        public eProc Type { get; private set; }


        public CompLotList(eProc index)
        {
            Type = index;

            if (Type == eProc.Live)
                _lotHistroy = new LotHistory();

            _product = new Dictionary<string, PTRY0PList>();
            _lot = new Dictionary<string, List<PreprocLot>>();
        }

        public void CreateLotHistory()
        {
            _lotHistroy = new LotHistory();
        }

        /// <summary>
        /// 랏관리 데이터에 신규 랏 정보를 추가
        /// </summary>
        /// <param name="key">필터 이름</param>
        /// <param name="info">불량 랏 정보</param>
        public void AddLot(string key, PreprocLot info)
        {
            if (LOT.ContainsKey(key) == true)
                LOT[key].Add(info);
            else
            {
                List<PreprocLot> list = new List<PreprocLot>();
                list.Add(info);
                LOT.Add(key, list);
            }

            if(Type == eProc.Live)
            {
                DateTime day = DateTime.Now;
                string strDay = day.ToString("yyyyMMdd");
                int nDay = int.Parse(strDay);
                LotHistory.AddHistory(nDay, key, info.LotName);
            }
        }

        /// <summary>
        /// 선택한 랏 이름을 삭제한다.
        /// </summary>
        /// <param name="lncd">라인 코드</param>
        /// <param name="lotName">랏 이름</param>
        /// <returns></returns>
        public bool DeleteLot(string key, string lotName)
        {
            bool isSuccess = true;
            if (LOT.ContainsKey(key) == true)
            {
                foreach (PreprocLot info in LOT[key])
                {
                    // 데이터 삭제
                    if (info.LotName == lotName) LOT[key].Remove(info);
                }
            }
            else isSuccess = false;
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

        public int LotSize(string key)
        {
            if (LOT.ContainsKey(key) == true)
                return LOT[key].Count;
            else
                return 0;
        }

        public int ProductSize(string key)
        {
            if (Product.ContainsKey(key) == true)
                return Product[key].Count;
            else
                return 0;
        }

        public void AddProduct(string key, PTRY0PList list)
        {
            Product.Add(key, list);
        }

        /// <summary>
        /// 선택한 랏 이름을 삭제한다.
        /// </summary>
        /// <param name="key">라인 코드</param>
        /// <param name="lotName">랏 이름</param>
        /// <returns></returns>
        public void DeleteProduct(string key, string lotName)
        {
            if (Product.ContainsKey(key))
            {
                foreach (var item in Product[key].Data)
                {
                    if (item.Y0KLOT == lotName)
                        Product[key].Data.Remove(item);
                }
            }
        }

        public void ClearData()
        {
            foreach (var lot in Product)
                lot.Value.Clear();
            Product.Clear();

            ClearLot();
        }
    }
}
