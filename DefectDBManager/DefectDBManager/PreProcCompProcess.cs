using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DefectDBManager
{
    public delegate void DelegateEndSearchingTodayProduct();
    public delegate void DelegateEndSearchingAvailableLot();
    public delegate void DelegatePopupError(string errString);

    public sealed class PreProcCompProcess : IDisposable
    {
        // 상위 이벤트 보고 
        public event DelegateEndSearchingTodayProduct OnEndTodayProductSearching;
        public event DelegateEndSearchingAvailableLot OnEndSearchingAvailableLot;
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
            PreProcCompDB procNext = _DBProc[(int)eDbIdWhen.Next];
            string lotID = procNow.SearchLotName;
            if(procNow.SearchLot(lotID)==true)
            {
                // 데이터 탐색이 완료되었으면 기본 데이터는 복사
                procNext._DbResult.PTRY0P_Today_Data = procNow._DbResult.PTRY0P_Today_Data;
                
                // 검사 완료 처리
                OnEndTodayProductSearching();
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
        private string crtBCNO = "";
        private double crtRollPosY = 0.0;
        private bool enableCheckINSPDAT = false;
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
                if (enableCheckINSPDAT == false)
                {
                    Thread.Sleep(500);
                    continue;
                }

                //현재 생산하고 있는 랏이 데이터에 없으면 다음 Lot을 탐색한다. 
                if (_DBProc[(int)eDbIdWhen.Now].IsCrtDataAvaliable(crtBCNO, crtRollPosY) == false)
                {
                    // 다음 랏을 기준으로 탐색한다.
                    string strLotID;
                    bool success;

                    // 탐색 가능 여부를 Flase로 변경함
                    _enaDefectSearch = false;
                    
                    if (_DBProc[(int)eDbIdWhen.Now]._DbResult.PTRY0P_Today_Data.Count >= NextY0KLOTIdx)
                    {
                        OnPopupError("탐색 인덱스가 현재 존재하는 데이터 범위를 넘어섰습니다.");
                        return;
                    }

                    strLotID = _DBProc[(int)eDbIdWhen.Now]._DbResult.PTRY0P_Today_Data[NextY0KLOTIdx].Y0KLOT;
                    success = _DBProc[(int)eDbIdWhen.Now].SearchPTRYOP(strLotID);
                    if (success == false)
                    {
                        NextY0KLOTIdx++;
                        continue;
                    }
                    success = _DBProc[(int)eDbIdWhen.Now].SearchINSPDAT(strLotID);

                    if (_DBProc[(int)eDbIdWhen.Now].SearchMatchedBCNOLot(crtBCNO, crtRollPosY) == true)
                    {
                        // 현재 랏 인덱스 정보를 업데이트 함
                        CrtY0KLOTIdx = NextY0KLOTIdx;

                        ////////////////////////////////////////////////////////////////////////////////////////////
                        // 예약랏 데이터 탐색함.
                        NextY0KLOTIdx++;
                        if(NextY0KLOTIdx< _DBProc[(int)eDbIdWhen.Next]._DbResult.PTRY0P_Today_Data.Count)
                        {
                            strLotID = _DBProc[(int)eDbIdWhen.Next]._DbResult.PTRY0P_Today_Data[NextY0KLOTIdx].Y0KLOT;
                            success = _DBProc[(int)eDbIdWhen.Next].SearchPTRYOP(strLotID);
                            success = _DBProc[(int)eDbIdWhen.Now].SearchINSPDAT(strLotID);
                            success = _DBProc[(int)eDbIdWhen.Now].SearchFLTDAT();

                            // 불량 탐색 완료 후 Flag 변경
                            if(success==true) _enaDefectSearch = true;
                        }
                        ////////////////////////////////////////////////////////////////////////////////////////////
                        
                        
                        OnEndSearchingAvailableLot();
                    }
                }

                Thread.Sleep(500);
            }
        }

        /// <summary>
        /// 실시간 검색 데이터 송부
        /// </summary>
        /// <param name="bcno">현재 생산하고 있는 제품의 BCNO</param>
        /// <param name="start">시작 지점</param>
        /// <param name="end">끝 지점</param>
        /// <returns></returns>
        public List<PointF> GetMarkDefectData(string bcno, float start, float end)
        {
            if (_enaDefectSearch == false) return null;

            float stY, edY;
            if(end<start)
            {
                stY = end;
                edY = start;
            }
            else
            {
                stY = start;
                edY = end;
            }
            
            return _DBProc[(int)eDbIdWhen.Now].FaultData.GetDefectPts(bcno, stY, edY);
        }


        #endregion
    }
}
