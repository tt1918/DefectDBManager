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
        public CompLotList Search
        {
            get { return _searchLot; }
            private set { _searchLot = value; }
        }
        private CompLotList _searchLot = new CompLotList(eProc.Search);
        #endregion LOT 정보

        #region Live LOT 정보
        public CompLotList Live
        {
            get { return _liveLot; }
            private set { _liveLot = value; }
        }
        private CompLotList _liveLot = new CompLotList(eProc.Live);
        #endregion LOT 정보

        #region Selected Lot 정보
        public CompLotList Selected
        {
            get { return _selectedLot; }
            private set { _selectedLot = value; }
        }
        private CompLotList _selectedLot = new CompLotList(eProc.Selected);

        #endregion

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
            
            _procLNCD = new Preproc.PreprocLNCD();
            LoadProcLNCD();

            _precSetting.Load();

            _crtProcFilter = new ProcFilterSet();
            _crtProcFilter.Load();
        }

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
