using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    public class SkipOffsetParam
    {
        public float Left;
        public float Right;
        public float Top;
        public float Bottom;

        public bool CheckSkip(float posX, float posY)
        {
            bool bSkip = false;
            if (Top > 0 || Bottom > 0)
                if (posY > Top && posY < Bottom)
                    return true;
            if (Left > 0 || Right > 0)
                if (posX > Left && posX < Right)
                    return true;
            return bSkip;
        }
    }

    // 공통으로 사용되는  임시 파라미터
    public class Param
    {
        public List<SkipOffsetParam> OffsetSkip;
        public UserDefectClass _UserDefectClass;

        public List<string> FAULTDATFLTID;
        public List<string> MRKCTLMSTFLTID;

        #region 불량 갯수
        public int[] DBFaultCount = new int[(int)eFLTID.MAX_PARAM];
        public int[] CSVFalutCount = new int[(int)eFLTID.MAX_PARAM];
        public int ESFalutCount;
        public int[] ESClassDefectCnt = new int[(int)eESDefectClass.MAX];

        public int Optic1Cnt = 0;
        public int Optic1ScCount = 0;
        public int Optic2Cnt = 0;
        public int Optic3Cnt = 0;
        public int Optic4DotCnt = 0;
        public int Optic4LineCnt = 0;
        public int OpticFreqCnt = 0;
        public int OpticSameCnt = 0;

        #endregion

        public int UseKT = 0;
        public string Model;

        public bool isProductAvaliable = false;
        public bool isXOffsetError = false;

        public Param()
        {
            OffsetSkip = new List<SkipOffsetParam>();
            _UserDefectClass = new UserDefectClass();

            FAULTDATFLTID = new List<string>();
            MRKCTLMSTFLTID = new List<string>();
        }

        ~Param()
        {
            if (OffsetSkip?.Count > 0) OffsetSkip.Clear();
            if (FAULTDATFLTID?.Count > 0) FAULTDATFLTID.Clear();
        }

        public void ClearEachOpticDefectCnt()
        {
            Optic1Cnt = 0;
            Optic1ScCount = 0;
            Optic2Cnt = 0;
            Optic3Cnt = 0;
            Optic4DotCnt = 0;
            Optic4LineCnt = 0;
            OpticFreqCnt = 0;
            OpticSameCnt = 0;
        }
    }


    #region mrkctlmst 변수 (Defect Edit에서 사용)
    public class MRKCTLMST_DE
    {
        //public int sizeIdx;
        public string LNCD;
        public string FLTID;
        public string PPCD;
        public float SIZE;
        public string MRKF1;
        public string ROLLNAME;

        public override string ToString()
        {
            string msg = $"{FLTID}, , {MRKF1}, , ";
            return msg;
        }
    }

    public class MRKCTLMST_DE_Data
    {
        public List<MRKCTLMST_DE> data;
        public string query;

        public MRKCTLMST_DE_Data()
        {
            data = new List<MRKCTLMST_DE>();
        }
    }

    #endregion
}
