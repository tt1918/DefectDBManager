using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Security;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DefectDBManager
{
    public delegate void DelegateAddPreprocLotData(string lncd, PreprocLot preprocLot);

    public sealed class ComparePreprocData : IDisposable
    {
        // 상위 이벤트 보고 
        // 오늘자 생산 예정 PTRY0P 탐색
        public event DelegateEvent OnEndTodayProductSearching = null;
        // 랏 변경 완료 이벤트 
        public event DelegateEvent OnEndLotChange = null;
        // 현재 수신한 MKCD Model 이름을 Form에 업데이트한다.
        public event DelegateEvent OnUpdateMKCD_ModelName = null;
        // 프로세스 상에 발생하는 이벤트 보고용
        public event DelegateProcessEvent OnProcessEvent = null;
        // MKCD 모델 요청용. 요청한 모델을 이용하여 데이터를 탐색한다. 
        public event DelegateRequestMKCD_ModelName OnRequestMKCD_ModelName=null;
        // 랏 검색 결과를 상위 저장소로 전달하도록 함
        public event DelegateAddPreprocLotData OnAddPreprocLotData = null;

        /// <summary>
        /// DB Query 및 탐색
        /// </summary>
        public PreProcCompDB _DBProc;

        /// <summary>
        /// DB 접근
        /// </summary>
        public OracleDbConnection _DbConn;
        
        /// <summary>
        /// Destination configuration
        /// </summary>
        public DestConfig _DestConfig;
        
        /// <summary>
        /// Code configuration
        /// </summary>
        public CodeConfig _CodeConfig;
        
        /// <summary>
        /// Data 탐색 옵션
        /// </summary>
        public Option _Option;

        bool disposed = false;
        private object parent = null;

        private Thread threadDBConnect = null;

        /// <summary>
        /// 가동 중 불량 검색 가능 여부 확인 Flag
        /// </summary>
        private bool _enaDefectSearch = false;

        private MRKCTLMSTParam _mkcdParam=null;

        public ComparePreprocData(object parent)
        {
            this.parent = parent;

            _DestConfig = new DestConfig();
            if (_DestConfig.Read() < 0) //Dest.Ini 파일 읽어들임
            {
                MessageBox.Show($"Failed to read {Define.DestPath}");
            }

            _DbConn = new OracleDbConnection();

            // 현재 랏 기준으로 데이터 생성
            _Option = new Option(0);
            _CodeConfig = new CodeConfig();

            _DBProc = new PreProcCompDB(this, _DbConn);

            _DBProc.DbDestConfig = _DestConfig;
            _DBProc.DBCodeConfig = _CodeConfig;
            _DBProc.DbOption = _Option;
            
            if (this.threadDBConnect != null)
            {
                this.threadDBConnect.Join(100);
                this.threadDBConnect = null;
            }

            this.threadDBConnect = new Thread(this.DbConnect);
            this.threadDBConnect.Start();
            this.parent = parent;
        }

        ~ComparePreprocData()
        {
            if (this.disposed)
                return;
            this.Dispose(false);
        }
        public void Dispose()
        {
            this.Dispose(true);
            GC.SuppressFinalize(this);
        }

        private void Dispose(bool disposing)
        {
            if (this.disposed)
                return;
            if (disposing)
            {
                _DbConn.Dispose();
            }
            this.disposed = true;
        }

        private void DbConnect()
        {
            try
            {
                _DbConn.Connect();
            }
            catch (Exception ex)
            {
                Log.Write($"[Error] : {ex.Message}");
            }
        }

        private void searchLot(object obj)
        {
            string lotID = _DBProc.SearchLotName;

            // 오늘자 PTRY0P 탐색 -> INSPDAT 탐색
            int firstIdx = -1;
            
            if (_DBProc.SearchTodayPTRY0PList(out firstIdx) == true)
            {
                // 검사 완료 처리
                OnEndTodayProductSearching?.Invoke();
            }
        }

        public void SearchLot(string lncd, string lotName)
        {
            _DBProc.SearchLotName = lotName;
            _DBProc.SearchY0LNCD = lncd;


            //OnAddPreprocLotData?.Invoke(lncd, );
        }

        #region Daily Lot 탐색 후 생산 데이터 정보 확인하는 Thread
    
       
        public bool SearchLotData(eDbIdWhen when, string lotName, string mkcdName)
        {
            bool success = true;
            int errIdx = -1;

            // 전공정 데이터 초기화 진행
            _DBProc.ResetDataAll();

            // MKCD Model 이름을 적용한다. 
            //_DBProc[(int)when].SetMKCDModel(mkcdName);

            // 랏을 탐색한다. 
            success = _DBProc.SearchLot(lotName, true, ref errIdx);
            if (success == true)
            {
                // 불량 탐색 완료 후 Flag 변경
                _enaDefectSearch = success;
                if (success == true) // 검색 완료 결과 보고
                    OnProcessEvent?.Invoke((int)eEventReport.eFinishedSearchDailyLotData);
                else // 실패 보고
                    OnProcessEvent?.Invoke((int)eEventReport.eEmptyDailyLotFaultData);
            }
            
            return success;
        }

        #endregion

        #region 전공정 데이터 처리
        /// <summary>
        /// 각 연신/도공/ECT 별 검색한 LNCD CODE 갯수
        /// </summary>
        /// <returns> 각 공정  </returns>
        public int[] GetCurrentInspDatCnt()
        {
            return _DBProc.GetCurrentInspDatCnt();
        }

        /// <summary>
        /// 선택한 공정에 대한 결점 포인트 정보를 전달
        /// </summary>
        /// <param name="fcd"></param>
        /// <param name="index"></param>
        /// <param name="LNCD"></param>
        /// <param name="pts"></param>
        public void GetSelectedPreprocDefects(eFCD fcd, int index, out string LNCD, out List<System.Drawing.PointF> pts)
        {
            LNCD = "";
            _DBProc.GetSelectedPreprocDefects(fcd, index, out LNCD, out pts);
        }


        /// <summary>
        /// 연신/도공/ECT에 대한 세보 공정 라인 코드명을 돌려준다. 
        /// 각 공정에 속하는 이름을 리스트로 전달한다.
        /// </summary>
        /// <param name="fcd">공정 </param>
        /// <returns></returns>
        public List<string> GetLineCodeName(eFCD fcd)
        {
            List<string> code = new List<string>();

            foreach (PreProcDefect data in _DBProc.FaultData.FLTDAT[(int)fcd])
            {
                code.Add(data.LNCD.ToString());
            }

            return code;
        }
        #endregion

        /// <summary>
        /// MKCD 모델을 적용한다. 
        /// </summary>
        /// <param name="name">모델 이름</param>
        /// <returns></returns>
        public void SetMKCDModel(string name)
        {
            _DBProc.SetMKCDModel(name);
            OnUpdateMKCD_ModelName?.Invoke();
        }
    }
}
