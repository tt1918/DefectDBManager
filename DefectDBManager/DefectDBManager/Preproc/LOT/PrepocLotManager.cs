using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    // 이전 공정 랏 데이터에 대한 
    public class PrepocLotManager
    {
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
        private Dictionary<string, List<PreprocLot>> _lot;
        #endregion

        #region 각 공정에 대한 품종 별 MRKCTLMST_MODEL
        /// <summary>
        /// 각 공정별 MRKCTLMST 모델 데이터
        /// </summary>
        public Dictionary<string, List<MRKCTLMST_MODEL>> MRKCTLMST
        {
            get { return _mrkctlmst; }
            set { _mrkctlmst = value; }
        }
        Dictionary<string, List<MRKCTLMST_MODEL>> _mrkctlmst = null;
        #endregion

        public PrepocLotManager()
        {
            _lot = new Dictionary<string, List<PreprocLot>>();
            _mrkctlmst = new Dictionary<string, List<MRKCTLMST_MODEL>>();
        }


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

        /// <summary>
        /// 전체 데이터 삭제
        /// </summary>
        public void ClearLot()
        {
            LOT.Clear();
        }

        public void AddMRKCTLMST_ModelData(string lncd, MRKCTLMST_MODEL model)
        {

        }
    }
}
