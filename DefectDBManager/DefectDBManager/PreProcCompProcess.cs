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
    public delegate void DelegateEvent();
    public delegate void DelegatePopupError(string errString); 
    public delegate void DelegateRequestMKCD_ModelName(eDbIdWhen dbWhen);

    public sealed class PreProcCompProcess : IDisposable
    {
        // 상위 이벤트 보고 
        // 오늘자 생산 예정 PTRY0P 탐색
        public event DelegateEvent OnEndTodayProductSearching = null;
        // 현재 생산하고 있는 BCNO기준 INSPDAT 데이터 완료
        public event DelegateEvent OnEndSearchingAvailableLot = null;
        // 랏 변경 완료 이벤트 
        public event DelegateEvent OnEndLotChange = null;
        // 현재 수신한 MKCD Model 이름을 Form에 업데이트한다.
        public event DelegateEvent OnUpdateMKCD_ModelName = null;
        // 프로세스 상에 발생하는 이벤트 보고용
        public event DelegateProcessEvent OnProcessEvent = null;
        // MKCD 모델 요청용. 요청한 모델을 이용하여 데이터를 탐색한다. 
        public event DelegateRequestMKCD_ModelName OnRequestMKCD_ModelName=null;

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

        /// <summary>
        /// 가동 중 불량 검색 가능 여부 확인 Flag
        /// </summary>
        private bool _enaDefectSearch = false;


        private MkcdParam[] _mkcdParam=null;

        private bool _isRunSearchDailyLot = false;

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
                this.CheckAvailableLotthread?.Abort();
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
            
            StopCheckingBCNO();
            StopCheckAvaliableINSPDAT();
            PreProcCompDB procNow = _DBProc[(int)eDbIdWhen.Now];

            string lotID = procNow.SearchLotName;

            // 오늘자 PTRY0P 탐색 -> INSPDAT 탐색
            int firstIdx = -1;
            
            if (procNow.SearchTodayPTRY0PList(out firstIdx) == true)
            {
                _isRunSearchDailyLot = true;
                // 검사 완료 처리
                OnEndTodayProductSearching?.Invoke();
                OnEndSearchingAvailableLot?.Invoke();
               
                // 체크 스레드 시작
                NextY0KLOTIdx = (ushort)(firstIdx+1);
                RunCheckingBCNO();
                StartCheckAvaliableINSPDAT();
            }

            _isRunSearchDailyLot = false;
        }

        public void SearchDailyLot()
        {
            PreProcCompDB proc = null;
            proc = _DBProc[(int)eDbIdWhen.Now];

            proc.SearchLotName = "";
            proc.SearchY0LNCD = _DestConfig.MainLNCD;

            if(_isRunSearchDailyLot==false)
            {
                Task task = new Task(searchDailyLot, proc);
                task.Start();
            }
        }

        #region Daily Lot 탐색 후 생산 데이터 정보 확인하는 Thread
        /// <summary>
        /// 현재 입력된 BCNO 정보
        /// </summary>
        private string _crtBCNO = "";

        /// <summary>
        /// 현재 롤의 위치 정보
        /// </summary>
        private double _crtRollPosY = 0.0;

        /// <summary>
        /// 랏 데이터 감시 
        /// </summary>
        private bool _enaCheckINSPDAT = false;

        /// <summary>
        /// 랏 데이터 감시 스레드
        /// </summary>
        private Thread CheckAvailableLotthread = null;

        /// <summary>
        /// 랏 데이터 감시 스레드 flag
        /// </summary>
        private bool _runAvailableLotCheck = false; 


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
                this._runAvailableLotCheck = false;
                this.CheckAvailableLotthread.Abort();
                this.CheckAvailableLotthread.Join(2000);
                this.CheckAvailableLotthread = null;
            }
        }

        // 수정 필요
        // 인덱스 기준으로 현재/예약 랏 검색 방식 변경이 필요함. 
        private void threadCheckAvaliableINSPDAT()
        {
            _runAvailableLotCheck = true;

            int checkAvailableLot = 0;
            bool isFindMatchedBCNO=false;

            while (true)
            {
                if (_runAvailableLotCheck == false)
                    break;


                // 오늘자 생산 정보가 탐색 인덱스보다 큰 경우 알람 처리
                if (_DBProc[(int)eDbIdWhen.Now].PTRY0P_Today_Data.Count <= NextY0KLOTIdx)
                {
                    _runAvailableLotCheck = false;
                    OnProcessEvent?.Invoke((int)eEventReport.eEmptyDailyLotData);
                    Log.Write("탐색 인덱스가 현재 존재하는 데이터 범위를 넘어섰습니다.");
                    break;
                }

                // 검색 처리
                // 검색 인덱스가 넘어가면 대기 처리함.
                if (_enaCheckINSPDAT == false)
                {
                    Thread.Sleep(100);
                    continue;
                }

                //현재 생산하고 있는 랏이 데이터에 없으면 다음 Lot을 탐색한다. 
                bool isAvaliable = _DBProc[(int)eDbIdWhen.Now].IsCrtDataAvaliable(_crtBCNO, _crtRollPosY);
                if (isAvaliable == false )
                {
                    checkAvailableLot++;
                    if (checkAvailableLot < 100)
                    {
                        Thread.Sleep(200);
                        continue;
                    }

                    checkAvailableLot = 0;
               
                    // 다음 랏을 기준으로 탐색한다.
                    string strLotID;
                    bool success = false;

                    // 탐색 가능 여부를 False로 변경함
                    _enaDefectSearch = false;

                    // 전공정 데이터 초기화 진행
                    _DBProc[(int)eDbIdWhen.Now].ResetDataAll();


                    // 금일자 생산 데이터에서 랏 정보 얻어옴
                    strLotID = _DBProc[(int)eDbIdWhen.Now].PTRY0P_Today_Data[NextY0KLOTIdx].Y0KLOT;

                    if (success == true) success = _DBProc[(int)eDbIdWhen.Now].SearchPTRYLP(strLotID);

                    if (success == true) success = _DBProc[(int)eDbIdWhen.Now].SearchXOFSMST(strLotID);
                    
                    // PTRY0P 탐색
                    success = _DBProc[(int)eDbIdWhen.Now].SearchPTRY0P(strLotID);

                    // SearchPTRY0P 문제가 없으면 INSPDAT 탐색함
                    if (success == true) success = _DBProc[(int)eDbIdWhen.Now].SearchINSPDAT(strLotID);

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
                    if (_DBProc[(int)eDbIdWhen.Now].SearchMatchedBCNOLot(_crtBCNO, _crtRollPosY, false) == true )
                    {
                        // 현재 랏 인덱스 정보를 업데이트 함
                        CrtY0KLOTIdx = NextY0KLOTIdx;

                        int errNum = -1;
                        string lotName = _DBProc[(int)eDbIdWhen.Now].PTRY0P_Today_Data[CrtY0KLOTIdx].Y0KLOT;

                        ////////////////////////////////////////////////////////////////////////////////////////////
                        /// 현재랏 데이터 검색
                        success = _DBProc[(int)eDbIdWhen.Now].SearchLot(lotName, true, ref errNum);

                        // 불량 탐색 완료 후 Flag 변경
                        _enaDefectSearch = success;
                        if (success == true) // 검색 완료 결과 보고
                        {
                            OnProcessEvent?.Invoke((int)eEventReport.eFinishedSearchDailyLotData);

                            // 검색 결과 상위 업데이트 함
                            if (OnEndSearchingAvailableLot != null) OnEndSearchingAvailableLot();
                        }
                        else // 실패 보고
                        {
                            if(errNum == 5)
                                OnProcessEvent?.Invoke((int)eEventReport.eNoMKCD_Model);
                            else
                                OnProcessEvent?.Invoke((int)eEventReport.eEmptyDailyLotFaultData);
                        }
                        ////////////////////////////////////////////////////////////////////////////////////////////
                    }
                    else if(NextY0KLOTIdx>= _DBProc[(int)eDbIdWhen.Now].PTRY0P_Today_Data.Count-1) // 실패 보고 
                        OnProcessEvent?.Invoke((int)eEventReport.eFailedSearchDailyLotData);

                    // 검색 완료되면 실폐든 아니든 인덱스 업데이트함
                    NextY0KLOTIdx++;
                }
                else if(isAvaliable==true)
                {
                    if (isFindMatchedBCNO == true)
                        continue;

                    if (_DBProc[(int)eDbIdWhen.Now].SearchMatchedBCNOLot(_crtBCNO, _crtRollPosY) == true)
                    {
                        isFindMatchedBCNO = true;
                    }
                }
                else
                {
                    checkAvailableLot++;
                }

                Thread.Sleep(200);
            }
            _runAvailableLotCheck = false;
        }

        public bool SearchLotData(eDbIdWhen when, string lotName, string mkcdName)
        {
            bool success = true;
            int errIdx = -1;

            // 전공정 데이터 초기화 진행
            _DBProc[(int)when].ResetDataAll();

            // MKCD Model 이름을 적용한다. 
            //_DBProc[(int)when].SetMKCDModel(mkcdName);

            // 랏을 탐색한다. 
            success = _DBProc[(int)when].SearchLot(lotName, true, ref errIdx);
            if (success == true)
            {
                // 불량 탐색 완료 후 Flag 변경
                _enaDefectSearch = success;
                if (success == true) // 검색 완료 결과 보고
                {
                    OnProcessEvent?.Invoke((int)eEventReport.eFinishedSearchDailyLotData);
                }
                else // 실패 보고
                    OnProcessEvent?.Invoke((int)eEventReport.eEmptyDailyLotFaultData);
            }
            OnEndSearchingAvailableLot?.Invoke();
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

            //////////////////////////////////////////////////////////////////////////
            // 예약 랏 -> 현재 랏 DB 데이터 이전
            _DBProc[0]._DbResult = _DBProc[1]._DbResult;
            _DBProc[1]._DbResult = new DbSearchResult();
            //////////////////////////////////////////////////////////////////////////

            //////////////////////////////////////////////////////////////////////////
            // 예약 랏 -> 현재 랏 FLTDAT 데이터 이전
            PrePocResultData oldMarkingData;
            oldMarkingData = _DBProc[0].FaultData;
            _DBProc[0].FaultData = _DBProc[1].FaultData;

            // 이전 현재랏으 데이터 초기화
            oldMarkingData.ResetAll();
            //////////////////////////////////////////////////////////////////////////

            // 예약랏 DB 옵션 복사.
            // 출하처 사용하지 않아 실제 필요하지는 않지만 이전 프로그램과 동일하게
            // 처리하기 위해 복사
            _DBProc[0].DbOption.Copy(_DBProc[1].DbOption);

            // 예약 랏 결점 데이터 초기화 처리
            _DBProc[1].FaultData = new PrePocResultData();
            _DBProc[1].ResetDataAll();

            //////////////////////////////////////////////////////////////////////////
            // 현재 랏의 MKCD 모델 데이터를 업데이트한다.
            _DBProc[0].MKCD_Param = _DBProc[1].MKCD_Param;
            _DBProc[0].MKCD_Model = _DBProc[1].MKCD_Model;
            OnUpdateMKCD_ModelName?.Invoke();

            // 예약랏에는 모델 데이터를 신규로 생성한다.
            _DBProc[1].MKCD_Param = new MkcdParam();
            _DBProc[1].MKCD_Model = new MKCD_MODEL();
            //////////////////////////////////////////////////////////////////////////

            // 상부에 랏 변경 보고
            OnEndLotChange?.Invoke();

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
            string[] bcnoData = bcno.Split('_');
            if(bcnoData.Length>1)
                _crtBCNO = bcnoData[0];
            else
                _crtBCNO = bcno;
            

            // 검사 진행 거리는 중간 지점으로 처리함
            _crtRollPosY = (stY + edY) / 2.0;

            return _DBProc[(int)eDbIdWhen.Now].FaultData?.GetDefectPts(_crtBCNO, stY, edY);
        }

        /// <summary>
        /// 각 연신/도공/ECT 별 검색한 LNCD CODE 갯수
        /// </summary>
        /// <returns> 각 공정  </returns>
        public int[] GetCurrentInspDatCnt()
        {
            return _DBProc[(int)eDbIdWhen.Now].GetCurrentInspDatCnt();
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
            _DBProc[(int)eDbIdWhen.Now].GetSelectedPreprocDefects(fcd, index, out LNCD, out pts);
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

            foreach (PreProcDefect data in _DBProc[(int)eDbIdWhen.Now].FaultData.FLTDAT[(int)fcd])
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
        /// <param name="when">현재랏/예약랏 설정</param>
        public void SetMKCDModel(string name, eDbIdWhen when)
        {
            _DBProc[(int)when].SetMKCDModel(name);
            OnUpdateMKCD_ModelName?.Invoke();
        }
    }
}
