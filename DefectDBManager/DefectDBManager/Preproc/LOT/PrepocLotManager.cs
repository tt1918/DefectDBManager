using DefectDBManager.DB;
using DefectDBManager.Preproc;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace DefectDBManager
{
    public class TimeTable
    {
        public DateTime StartTime { get; set; } = DateTime.Now;
        public DateTime EndTime { get; set; } = DateTime.Now;

        public TimeTable()
        {

        }

        public void SetTime(DateTime stTime, DateTime edTime)
        {
            StartTime = stTime;
            EndTime = edTime;
        }

    }

    // 이전 공정 랏 데이터에 대한 
    public class PreprocLotManager
    {
        #region Param
        #region Search LOT 정보
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
                if (_lot == null) return 0;

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
                if(_product==null) return 0;
                int val = 0;
                foreach (var list in _product)
                    val += list.Value.Count;
                return val;
            }
        }
        #endregion LOT 정보

        #region Live LOT 정보
        /// <summary>
        /// 각 공정별 검사 결과 데이터 저장
        /// string : 각 공정 데이터
        /// List<PreprocLog> : Lot 정보
        /// </summary>
        public Dictionary<string, List<PreprocLot>> LiveLot
        {
            get { return _Livelot; }
            private set { _Livelot = value; }
        }
        private Dictionary<string, List<PreprocLot>> _Livelot = null;

        public int TotalLiveLot
        {
            get
            {
                if (_Livelot == null) return 0;

                int val = 0;
                foreach (var list in _Livelot)
                    val += list.Value.Count;
                return val;
            }
        }

        /// <summary>
        /// 공정 생산 정보를 저장
        /// </summary>
        public Dictionary<string, DB.PTRY0PList> LiveProduct
        {
            get { return _liveProduct; }
            private set { _liveProduct = value; }
        }

        private Dictionary<string, DB.PTRY0PList> _liveProduct = null;

        public int TotalLiveProduct
        {
            get
            {
                if (_liveProduct == null) return 0;
                int val = 0;
                foreach (var list in _liveProduct)
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


        public Preproc.PreprocSet ProcSetting
        {
            get { return _precSetting; }
            private set { _precSetting = value; }
        }
        private Preproc.PreprocSet _precSetting=new Preproc.PreprocSet();


        public ProcFilterSet CrtProcFilter
        {
            get { return _crtProcFilter; }
            private set { _crtProcFilter = value; }
        }
        public ProcFilterSet _crtProcFilter = null;
        #endregion

        #region 검색 시간 설정 
        public TimeTable SearchTime { get; set; } = new TimeTable();
        #endregion


        public PreprocLotManager()
        {
            _product = new Dictionary<string, PTRY0PList>();
            _lot = new Dictionary<string, List<PreprocLot>>();
            _procLNCD = new Preproc.PreprocLNCD();
            _liveProduct = new Dictionary<string, PTRY0PList>();
            _Livelot = new Dictionary<string, List<PreprocLot>>();

            LoadProcLNCD();

            _precSetting.Load();

            _crtProcFilter = new ProcFilterSet();
            _crtProcFilter.Load();
        }


        #region Lot 데이터 관리
        /// <summary>
        /// 랏관리 데이터에 신규 랏 정보를 추가
        /// </summary>
        /// <param name="filter">필터 이름</param>
        /// <param name="info">불량 랏 정보</param>
        public void AddLot(string filter, PreprocLot info)
        {
            if (LOT.ContainsKey(filter) == true)
                LOT[filter].Add(info);
            else
            {
                List<PreprocLot> list = new List<PreprocLot>();
                list.Add(info);
                LOT.Add(filter, list);
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


        #region Live Lot 데이터 관리

        public void DeleteLiveProduct(string lncd, string lotName)
        {
            if (LiveProduct.ContainsKey(lncd))
            {
                foreach(var item in LiveProduct[lncd].Data)
                {
                    if (item.Y0KLOT == lotName)
                        LiveProduct[lncd].Data.Remove(item);
                }
            }
        }

        /// <summary>
        /// 랏관리 데이터에 신규 랏 정보를 추가
        /// </summary>
        /// <param name="lncd">라인 코드</param>
        /// <param name="info">불량 랏 정보</param>
        public void AddLiveLot(string lncd, PreprocLot info)
        {
            if (LiveLot.ContainsKey(lncd) == true)
                LiveLot[lncd].Add(info);
            else
            {
                List<PreprocLot> list = new List<PreprocLot>();
                list.Add(info);
                    LiveLot.Add(lncd, list);
            }
        }

        /// <summary>
        /// 선택한 랏 이름을 삭제한다.
        /// </summary>
        /// <param name="lncd">라인 코드</param>
        /// <param name="lotName">랏 이름</param>
        /// <returns></returns>
        public bool DeleteLiveLot(string lncd, string lotName)
        {
            bool isSuccess = true;
            if (LiveLot.ContainsKey(lncd) == true)
            {
                foreach (PreprocLot info in LiveLot[lncd])
                {
                    // 데이터 삭제
                    if (info.LotName == lotName) LiveLot[lncd].Remove(info);
                }
            }
            else isSuccess = false;
            return isSuccess;
        }

        public void ClearLiveLot()
        {
            // 각 공정 별 랏 정보 삭제
            foreach (var lot in LiveLot)
                lot.Value.Clear();

            // Dictionary 삭제
            LiveLot.Clear();
        }

        public int LiveProductSize(string key)
        {
            if (LiveProduct.ContainsKey(key) == true)
                return LiveProduct[key].Count;
            else
                return 0;
        }

        public int LiveLotSize(string key)
        {
            if (LiveLot.ContainsKey(key) == true)
                return LiveLot[key].Count;
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

        public void UpdatePreprocSet(Preproc.PreprocSet set)
        {
            ProcSetting = set;
        }

        public void SetLNCDData(PreprocLNCD data)
        {
            this.ProcLNCD = data;
        }
        #endregion
    }
}
