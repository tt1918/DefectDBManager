using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{

    public interface IRollDefectInfo
    {
        string LotNo { get; }
        int BadCnt { get; }

        float StartM { get; }
        float EndM { get; }

        void InitData(string lotNo, bool reset=true);
        void AddYPos(float val);
        float GetDefectPerM();

    }

    public class RollDefectInfo : IRollDefectInfo
    {
        public string LotNo { get; private set; }
        public int BadCnt { get { return badCnt; } }
        private int badCnt;

        public float StartM { get; private set; }
        private float startM;
        public float EndM { get; private set; }    
        private float endM;

        private float defectPerM;

        public RollDefectInfo()
        {
            LotNo = "";
            badCnt = 0;
            defectPerM = 0.0f;

            startM = float.MaxValue;
            endM = float.MinValue;
        }

        private void reset()
        {
            badCnt = 0;
            defectPerM = 0.0f;

            startM = float.MaxValue;
            endM = float.MinValue;
        }

        public void InitData(string lotNo, bool reset = true)
        {
            LotNo = lotNo;

            if (reset == true)
                this.reset();
        }

        public void AddYPos(float val)
        {
            if (startM > val) startM = val;
            else if (endM < val) endM = val;
            badCnt++;
        }

        public float GetDefectPerM()
        {
            if (badCnt == 0) return 0;

            defectPerM = badCnt / (endM - startM);
            return defectPerM;
        }

    }

    public class RollDefectInfo_Kor : IRollDefectInfo
    {
        public string LotNo { get; private set; }
        public int BadCnt { get { return badCnt; } }
        private int badCnt;

        public float StartM { get; private set; }
        private float startM;
        public float EndM { get; private set; }
        private float endM;

        private float defectPerM;

        public RollDefectInfo_Kor()
        {
            LotNo = "";
            badCnt = 0;
            defectPerM = 0.0f;

            startM = float.MaxValue;
            endM = float.MinValue;
        }

        private void reset()
        {
            badCnt = 0;
            defectPerM = 0.0f;

            startM = float.MaxValue;
            endM = float.MinValue;
        }

        public void InitData(string lotNo, bool reset = true)
        {
            LotNo = lotNo;

            if (reset == true)
                this.reset();
        }

        public void AddYPos(float val)
        {
            if (startM > val) startM = val;
            else if (endM < val) endM = val;
            badCnt++;
        }

        public void SetDefectPerM(float val)
        {
            defectPerM = val;
        }

        public float GetDefectPerM()
        {
            if (badCnt == 0) return 0;

            defectPerM = badCnt / (endM - startM);
            return defectPerM;
        }
    }

    public class CSV_DEFECT_HEADER
    {
        public string bcrInfo;
        public string lotNo;
        public string rollNo;
        public string startY;
        public string endY;
        public string rollY;
        public string rollSY;
        public string rollEY;
        public string jig;
        public string machine;
    }
};
