using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Security;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DefectDBManager
{
    public delegate void DelegateEvent();
    public delegate void DelegatePopupError(string errString);

    public sealed class PreProcCompProcess : IDisposable
    {
        // 상위 이벤트 보고 
        // 오늘자 생산 예정 PTRY0P 탐색
        public event DelegateEvent OnEndTodayProductSearching;
        // 현재 생산하고 있는 BCNO기준 INSPDAT 데이터 완료
        public event DelegateEvent OnEndSearchingAvailableLot;
        // 랏 변경 완료 이벤트 
        public event DelegateEvent OnEndLotChange;
        // 에러 팝업 이벤트
        public event DelegatePopupError OnPopupError;

        /// <summary>
        /// DB Query 및 탐색
        /// </summary>
        public PreProcCompDB[] _DBProc;
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
        public CodeConfig[] _CodeConfig;
        /// <summary>
        /// Data 탐색 옵션
        /// </summary>
        public Option[] _Option;

        // 현재랏 인덱스 번호
        public UInt16 CrtY0KLOTIdx { get; set; }
        // 다음랏 인덱스 번호
        public UInt16 NextY0KLOTIdx { get; set; }

        bool disposed = false;
        private object parent = null;

        private Thread threadDBConnect = null;
        public event DelegateProcessEvent OnProcessEvent = null;

        /// <summary>
        /// 가동 중 불량 검색 가능 여부 확인 Flag
        /// </summary>
        private bool _enaDefectSearch = false;

        public PreProcCompProcess(object parent)
        {
            this.parent = parent;

            _DestConfig = new DestConfig();
            if (_DestConfig.Read() < 0) //Dest.Ini 파일 읽어들임
            {
                MessageBox.Show($"Failed to read {Define.DestPath}");
            }

            _DbConn = new OracleDbConnection();

            int cnt = System.Enum.GetValues(typeof(eDbIdWhen)).Length + 1;
            _DBProc = new PreProcCompDB[cnt];
            _Option = new Option[cnt];
            _CodeConfig = new CodeConfig[cnt];

            for (int i = 0; i < cnt; i++)
            {
                _Option[i] = new Option(i);
                _CodeConfig[i] = new CodeConfig();

                _DBProc[i] = new PreProcCompDB(this, _DbConn);

                _DBProc[i].DbDestConfig = _DestConfig;
                _DBProc[i].DBCodeConfig = _CodeConfig[i];
                _DBProc[i].DbOption = _Option[i];
            }

            if (this.threadDBConnect != null)
            {
                this.threadDBConnect.Join(100);
                this.threadDBConnect = null;
            }

            this.threadDBConnect = new Thread(this.DbConnect);
            this.threadDBConnect.Start();
            this.parent = parent;
        }

        ~PreProcCompProcess()
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

        private void searchDailyLot(object obj)
        {
            StopCheckAvaliableINSPDAT();
            PreProcCompDB procNow = _DBProc[(int)eDbIdWhen.Now];

            string lotID = procNow.SearchLotName;

            // 오늘자 PTRY0P 탐색 -> INSPDAT 탐색
            if(procNow.SearchTodayPTRY0PList(lotID)==true)
            {                
                // 검사 완료 처리
                if(OnEndTodayProductSearching!=null) OnEndTodayProductSearching();
                
                // 체크 스레드 시작
                StartCheckAvaliableINSPDAT();
            }
        }

        public void SearchDailyLot(string lotID)
        {
            PreProcCompDB proc = null;
            proc = _DBProc[(int)eDbIdWhen.Now];
            
            proc.SearchLotName = lotID;
            proc.SearchY0LNCD = _DestConfig.MainLNCD;

            

            Task task = new Task(searchDailyLot, proc);
            task.Start();
        }

        #region Daily Lot 탐색 후 생산 데이터 정보 확인하는 Thread
        private string _crtBCNO = "";
        private double _crtRollPosY = 0.0;
        private bool _enaCheckINSPDAT = false;
        private Thread CheckAvailableLotthread = null;

        public void StartCheckAvaliableINSPDAT()
        {
            StopCheckAvaliableINSPDAT();

            this.CheckAvailableLotthread = new Thread(this.threadCheckAvaliableINSPDAT);
            this.CheckAvailableLotthread.Start();
        }

        public void StopCheckAvaliableINSPDAT()
        {
            if (this.CheckAvailableLotthread != null)
            {
                this.CheckAvailableLotthread.Abort();
                this.CheckAvailableLotthread.Join(100);
                this.CheckAvailableLotthread = null;
            }
        }

        // 수정 필요
        // 인덱스 기준으로 현재/예약 랏 검색 방식 변경이 필요함. 
        private void threadCheckAvaliableINSPDAT()
        {

            while (true)
            {
                // 검색
                if (_enaCheckINSPDAT == false)
                {
                    Thread.Sleep(500);
                    continue;
                }

                //현재 생산하고 있는 랏이 데이터에 없으면 다음 Lot을 탐색한다. 
                if (_DBProc[(int)eDbIdWhen.Now].IsCrtDataAvaliable(_crtBCNO, _crtRollPosY) == false)
                {
                    // 다음 랏을 기준으로 탐색한다.
                    string strLotID;
                    bool success=false;

                    // 탐색 가능 여부를 False로 변경함
                    _enaDefectSearch = false;

                    // 전공정 데이터 초기화 진행
                    _DBProc[(int)eDbIdWhen.Now].ResetDataAll();

                    // 오늘자 생산 정보가 탐색 인덱스보다 큰 경우 알람 처리
                    if (_DBProc[(int)eDbIdWhen.Now].PTRY0P_Today_Data.Count >= NextY0KLOTIdx)
                    {
                        OnProcessEvent((int)eEventReport.eEmptyDailyLotData);
                        OnPopupError("탐색 인덱스가 현재 존재하는 데이터 범위를 넘어섰습니다.");
                        return;
                    }

                    // 금일자 생산 데이터에서 랏 정보 얻어옴
                    strLotID = _DBProc[(int)eDbIdWhen.Now].PTRY0P_Today_Data[NextY0KLOTIdx].Y0KLOT;

                    // PTRY0P 탐색
                    success = _DBProc[(int)eDbIdWhen.Now].SearchPTRY0P(strLotID);

                    // SearchPTRY0P 문제가 없으면 INSPDAT 탐색함
                    if (success==true) success = _DBProc[(int)eDbIdWhen.Now].SearchINSPDAT(strLotID);

                    // 만얄 문제가 생겼다면, 다음 랏을 탐색.
                    // 무작정 문제가 생긴다고 인덱스 올리면 괜찮을까? 
                    if (success == false)
                    {
                        NextY0KLOTIdx++;
                        Thread.Sleep(100);
                        continue;
                    }

                    // 유효 모델 탐색
                    // 현재 생산하고 있는 Lot의 BCNO와 원단장 거리를 이용하여 현재 생산하는 INSPDAT의 데이터를 비교 
                    if (_DBProc[(int)eDbIdWhen.Now].SearchMatchedBCNOLot(_crtBCNO, _crtRollPosY) == true)
                    {
                        // 현재 랏 인덱스 정보를 업데이트 함
                        CrtY0KLOTIdx = NextY0KLOTIdx;
                        ////////////////////////////////////////////////////////////////////////////////////////////
                        /// 현재랏 데이터 검색
                        /// 위에서 SearchINSPDAT 검색 까지 완료했으므로 Falut data만 검색하면 됨.
                        if (success == true) success = _DBProc[(int)eDbIdWhen.Now].SearchFLTDAT();
                        if(success == true) 
                        {
                            // 불량 탐색 완료 후 Flag 변경
                            _enaDefectSearch = success;
                            if (success == true) // 검색 완료 결과 보고
                            {
                                OnProcessEvent((int)eEventReport.eFinishedSearchDailyLotData);

                                // 검색 결과 상위 업데이트 함
                                OnEndSearchingAvailableLot();
                            }
                            else // 실패 보고
                                OnProcessEvent((int)eEventReport.eEmptyDailyLotFaultData);
                        }
                        ////////////////////////////////////////////////////////////////////////////////////////////

                        ////////////////////////////////////////////////////////////////////////////////////////////
                        /// 예약랏 데이터 탐색함.
                        /// PTRY0P_Today_Data는 현재 랏이 관리함. 
                        /// BCNO 처리 어떻게 할지 확인 필요함. 
                        /// 확인되면 예약랏 불러오기와 랏 체인지 시에 Falut data 바꾸기 필요함. 
                        if (NextY0KLOTIdx + 1 < _DBProc[(int)eDbIdWhen.Now].PTRY0P_Today_Data.Count)
                        {
                            strLotID = _DBProc[(int)eDbIdWhen.Now].PTRY0P_Today_Data[NextY0KLOTIdx + 1].Y0KLOT;

                            // 예약랏 랏 데이터 초기화 진행
                            _DBProc[(int)eDbIdWhen.Next].ResetDataAll();

                            // 검색은 각 단계 별로 작업이 정상 완료되었을 때만 다음 순번 랏의 다운로드 작업을 진행하도록 함
                            success = _DBProc[(int)eDbIdWhen.Next].SearchPTRY0P(strLotID);
                            if (success == true) success = _DBProc[(int)eDbIdWhen.Next].SearchINSPDAT(strLotID);
                            if (success == true)
                            {
                                // 아직 BCNO와 거리를 알지 못하므로 그냥 전체 INSPDAT 복사하여 FLTDAT 검색한다.
                                _DBProc[(int)eDbIdWhen.Next].CopyInspDatToMatchedInspData();

                                // 예약랏 FLTDAT 데이터 검색함.
                                success = _DBProc[(int)eDbIdWhen.Next].SearchFLTDAT();
                            }
                        }
                        ////////////////////////////////////////////////////////////////////////////////////////////
                    }
                    else // 실패 보고 
                        OnProcessEvent((int)eEventReport.eFailedSearchDailyLotData);

                    // 검색 완료되면 실폐든 아니든 인덱스 업데이트함
                    NextY0KLOTIdx++;
                }

                Thread.Sleep(1000);
            }
        }

        public bool SearchLotData(eDbIdWhen when, string lotName)
        {
            bool success = true;

            // PTRY0P 탐색
            success = _DBProc[(int)eDbIdWhen.Now].SearchPTRY0P(lotName);

            // SearchPTRY0P 문제가 없으면 INSPDAT 탐색함
            if (success == true) success = _DBProc[(int)when].SearchINSPDAT(lotName);
            if (success == true) success = _DBProc[(int)when].SearchFLTDAT();
            if (success == true)
            {
                // 불량 탐색 완료 후 Flag 변경
                _enaDefectSearch = success;
                if (success == true) // 검색 완료 결과 보고
                {
                    OnProcessEvent((int)eEventReport.eFinishedSearchDailyLotData);
                }
                else // 실패 보고
                    OnProcessEvent((int)eEventReport.eEmptyDailyLotFaultData);
            }
            return success;
        }
        

        /// <summary>
        /// 검사 시작 시 해당 함수를 실행하여 실시간 BCNO 확인 가능하도록 처리
        /// </summary>
        public void RunCheckingBCNO()
        {
            _enaCheckINSPDAT = true;
        }

        /// <summary>
        /// 검사 증지하여 BCNO 확인 기능을 유휴 상태로 변경
        /// </summary>
        public void StopCheckingBCNO()
        {
            _enaCheckINSPDAT = false;
        }

        /// <summary>
        /// 랏 체인지 시 랏 변경
        /// </summary>
        /// <returns> </returns>
        public bool ChnageLot()
        {
            bool isSuccess = true;
            
            // 예약 랏 -> 현재 랏 DB 데이터 이전
            _DBProc[0]._DbResult = _DBProc[1]._DbResult;
            _DBProc[1]._DbResult = new PreProcCompDBResult();

            // 예약 랏 -> 현재 랏 FLTDAT 데이터 이전
            PrePocResultData oldMarkingData;
            oldMarkingData = _DBProc[0].FaultData;
            _DBProc[0].FaultData = _DBProc[1].FaultData;
            
            // 이전 현재랏으 데이터 초기화
            oldMarkingData.ResetAll();

            // 예약랏 DB 옵션 복사.
            // 출하처 사용하지 않아 실제 필요하지는 않지만 이전 프로그램과 동일하게
            // 처리하기 위해 복사
            _DBProc[0].DbOption.Copy(_DBProc[1].DbOption);

            // 예약 랏 결점 데이터 초기화 처리
            _DBProc[1].FaultData = new PrePocResultData();
            _DBProc[1].ResetDataAll();

            // 상부에 랏 변경 보고
            OnEndLotChange();

            Log.Write($"Changing lot is finished.");

            return isSuccess;
        }

        #endregion

        #region 전공정 데이터 처리
        /// <summary>
        /// 실시간 검색 데이터 송부
        /// </summary>
        /// <param name="bcno">현재 생산하고 있는 제품의 BCNO</param>
        /// <param name="start">시작 지점</param>
        /// <param name="end">끝 지점</param>
        /// <returns></returns>
        public List<MarkingFaultDatum> GetMarkDefectData(string bcno, float start, float end)
        {
            if (_enaDefectSearch == false) return null;

            float stY, edY;
            if (end < start)
            {
                stY = end;
                edY = start;
            }
            else
            {
                stY = start;
                edY = end;
            }

            // 현재 생산하고 있는 BCNO 데이터를 업데이트 함. 
            _crtBCNO = bcno;
            // 검사 진행 거리는 중간 지점으로 처리함
            _crtRollPosY = (stY + edY) / 2.0;

            return _DBProc[(int)eDbIdWhen.Now].FaultData.GetDefectPts(bcno, stY, edY);
        }

        public List<string> GetLineCodeName(eFCD fcd)
        {
            List<string> code = new List<string>();

            foreach(PreProcDefect data in _DBProc[(int)eDbIdWhen.Now].FaultData.FLTDAT[(int)fcd])
            {
                code.Add(data.LNCD.ToString());
            }

            return code;
        }

        #endregion
    }
}
