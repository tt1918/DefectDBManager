using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using static DefectDBManager.UserDefectClass;

namespace DefectDBManager
{
    public sealed class DbManager : IDisposable
    {
        /// <summary>
        /// DB Query 및 탐색
        /// </summary>
        public NittoDB[] _DbProc;
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

        /// <summary>
        /// 데이터 탐색 파라미터
        /// </summary>
        public Param _Param;

        /// <summary>
        /// DB 검색 후 최종 불량 데이터
        /// </summary>
        public ResultData[] _ResultData;

        public FormDB _FormDB { get { return formDB; } }
        /// <summary>
        /// DB에서 받은 데이터 표시 및 컨트롤 
        /// </summary>
        private FormDB formDB = null;
        /// <summary>
        /// FormDB에 표시하는 데이터 상태
        /// Current Data : false
        /// Next Data : true
        /// </summary>
        private bool isNextDBView = false;
        private bool isFirst = true;

        bool disposed = false;

        private object parent = null;
        public DbManager(object parent)
        {
            this.parent = parent;

            _DestConfig = new DestConfig();
            _CodeConfig = new CodeConfig();
            _Option = new Option();
            _Param = new Param();
            _DbConn = new OracleDbConnection();

            int cnt = System.Enum.GetValues(typeof(eDbIdWhen)).Length;
            _ResultData = new ResultData[cnt];
            _DbProc = new NittoDB[cnt];
            for (int i = 0; i < cnt; i++)
            {
                _ResultData[i] = new ResultData();
                _DbProc[i] = new NittoDB(this, _DbConn);

                _DbProc[i].DbDestConfig = _DestConfig;
                _DbProc[i].DBCodeConfig = _CodeConfig;
                _DbProc[i].DbOption = _Option;
                _DbProc[i].CrtParam = _Param;
                _DbProc[i].ResultDefect = _ResultData[i];
            }

            //Dest.Ini 파일 읽어들임
            _DestConfig.Read();
            _Param._UserDefectClass.Load();

            formDB = new FormDB(this);
            formDB.DBConn = _DbConn;
            this.parent = parent;
        }

        ~DbManager()
        {
            if (this.disposed)
                return;
            this.Dispose(false);
        }
        public void Dispose()
        {
            this.Dispose(true);
            formDB.Dispose();
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

        /// <summary>
        /// 현재랏/이전랏 연결 처리
        /// </summary>
        /// <param name="isNext"></param>
        public void SetDataState(bool isNext)
        {
            if (formDB == null) return;

            if (isFirst==true || isNextDBView != isNext)
            {
                // Data 연결
                if (isNext == false)    // 현재랏
                {
                    this._Option.dbWhen = eDbIdWhen.Now;
                    formDB.DataBase = _DbProc[(int)eDbIdWhen.Now];
                }
                else // 예약랏
                {
                    this._Option.dbWhen = eDbIdWhen.Next;
                    formDB.DataBase = _DbProc[(int)eDbIdWhen.Next];
                }

                formDB.RedrawAll = true;
                isNextDBView = isNext;
                isFirst =false;
            }
        }

        public void ShowDBViewer(bool isNext)
        {
            if (formDB == null) return;

            SetDataState(isNext);

            formDB.Show();
        }

        public void HideDBViewer()
        {
            formDB.Hide();
        }

    }
}
