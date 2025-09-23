using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Timers;

namespace DefectDBManager.Preproc
{
    public class LotHistory
    {
        /// <summary>
        /// 이력 관리용 저장소
        /// Key1 : 검색 날짜
        /// Key2 : Filter 인덱스
        /// Value : Lot 이름
        /// </summary>
        public Dictionary<(int, string), List<string>> Histroy
        {
            get { return _history; }
            private set { _history = value; }
        }

        private Dictionary<(int, string), List<string>> _history = null;

        public LotHistory()
        {
            _history = new Dictionary<(int, string), List<string>>();

            initCheckHistoryTimer();

            startCheckHistroy();
        }

        ~LotHistory()
        {
            closeCheckHistoryTimer();
            Clear();
        }

        public void AddHistory(int day, string filter, string lotName)
        {
            if(_history.ContainsKey((day, filter))==false)
                _history[(day, filter)] = new List<string>();
            
            _history[(day, filter)].Add(lotName);
        }
        public void Clear()
        {
            if (_history != null)
            {
                _history.Clear();
                _history = null;
            }
        }

        public bool IsLotExist(string filter, string lotName)
        {
            return _history
                .Where(kvp => kvp.Key.Item2 == filter) // filter에 해당하는 항목만
                .Any(kvp => kvp.Value.Contains(lotName)); // 해당 항목의 리스트에 lotName이 있는지 확인
        }

        #region Key Check Timer
        System.Timers.Timer _timerCheckHistory = null;

        private void initCheckHistoryTimer()
        {
            closeCheckHistoryTimer();

            _timerCheckHistory = new System.Timers.Timer();
            _timerCheckHistory.Interval = 60 * 1000; // 1분에 1번씩 탐색하도록 함
            _timerCheckHistory.Elapsed += new ElapsedEventHandler(checkLotHistory);
        }

        private void closeCheckHistoryTimer()
        {
            if (_timerCheckHistory != null)
            {
                _timerCheckHistory.Stop();
                _timerCheckHistory.Elapsed -= checkLotHistory;
                _timerCheckHistory.Dispose();
                _timerCheckHistory = null;
            }
        }

        private void startCheckHistroy()
        {
            _timerCheckHistory.Start();
        }

        private void checkLotHistory(object sender, ElapsedEventArgs e)
        {
            var key1List = _history.Keys.Select(k => k.Item1).Distinct();
            DateTime today = DateTime.Now;
            int nToday = int.Parse(today.ToString("yyyyMMdd"));

            List<int> delKey = new List<int>();

            foreach (var date in key1List)
            {
                if(date< nToday)    delKey.Add(date);
            }

            foreach (var key in delKey)
            {
                var keysToRemove = _history.Where(kvp => kvp.Key.Item1 == key)
                    .Select(kvp=>kvp.Key).ToList();

                foreach(var key1 in keysToRemove)
                    _history.Remove(key1);
            }
        }
        #endregion


    }
}
