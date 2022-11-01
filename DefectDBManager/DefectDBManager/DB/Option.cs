using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    public class SearchOption
    {
        public bool useDefectEdit;
        public bool useMask;
        public bool useSplit;

        public string MKCD;

        public string Title;

        public float splitStartX;
        public float splitEndX;
        public SearchOption()
        {

        }

    }

    public class Option
    {
        public string lotName;
        public eDbIdWhen dbWhen;

        public int vendor;

        public bool checkES;
        public bool checkTG;
        public bool checkETC;

        public int timeGabStMinute1;
        public int timeGabStMinute2;
        public int timeGabEdMinute1;
        public int timeGabEdMinute2;

        public int timeGabEsStMinute1;
        public int timeGabEsStMinute2;
        public int timeGabEsEdMinute1;
        public int timeGabEsEdMinute2;

        public bool useESTime;
        public bool useOffsetX;
        public bool useSameDefect;
        
        public bool useKT;

        public bool useAIfromDB;

        public string FWPlace;
        

        // 도공 광학계 선택
        public bool useCSVAll;
        public bool useCSVCross;
        public bool useCSVLimit;
        public bool useCSVSlit;
        public bool useCSVStraight;
        public bool useCSVSVJH;
        public bool useCSVSVMH;

        // 생산 가능 경과시간
        public int prodAvaliableSpan;
        public float xOffset;

        public  SearchOption searchOP;

        public Option()
        {
            timeGabEsStMinute1 = 5;
            timeGabEsStMinute2 = 5;
            timeGabEsEdMinute1 = 5;
            timeGabEsEdMinute2 = 5;

            timeGabStMinute1 = 5;
            timeGabStMinute2 = 5;
            timeGabEdMinute1 = 5;
            timeGabEdMinute2 = 5;

            useCSVCross = true;
            useCSVLimit = true;
            useCSVSlit = true;
            useCSVStraight = true;
            useCSVSVJH = true;
            useCSVSVMH = true;

            prodAvaliableSpan = 0;
            xOffset = 0;

            searchOP = new SearchOption();
        }
    }

}
