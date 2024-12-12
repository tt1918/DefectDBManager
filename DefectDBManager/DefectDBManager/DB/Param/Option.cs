using System;
using System.Collections.Generic;
using System.Drawing;
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

        public void Copy(ref SearchOption src)
        {
            useDefectEdit = src.useDefectEdit;
            useMask = src.useMask;
            useSplit = src.useSplit;
            MKCD = src.MKCD;
            Title = src.Title;
            splitStartX = src.splitStartX;
            splitEndX = src.splitEndX;
        }
    }

    public class Option
    {
        public bool isLoadCSV;
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

        public SearchOption searchOP;

        public Option(int isNext)
        {
            dbWhen = (eDbIdWhen)isNext;
            
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

            isLoadCSV = false;

            searchOP = new SearchOption();
        }

        public void Copy(Option src)
        {
            isLoadCSV = src.isLoadCSV;
            lotName = src.lotName;

            vendor = src.vendor;

            checkES = src.checkES;
            checkTG = src.checkTG;
            checkETC = src.checkETC;

            timeGabStMinute1 = src.timeGabStMinute1;
            timeGabStMinute2 = src.timeGabStMinute2;
            timeGabEdMinute1 = src.timeGabEdMinute1;
            timeGabEdMinute2 = src.timeGabEdMinute2;

            timeGabEsStMinute1 = src.timeGabEsStMinute1;
            timeGabEsStMinute2 = src.timeGabEsStMinute2;
            timeGabEsEdMinute1 = src.timeGabEsEdMinute1;
            timeGabEsEdMinute2 = src.timeGabEsEdMinute2;

            useESTime = src.useESTime;
            useOffsetX = src.useOffsetX;
            useSameDefect = src.useSameDefect;

            useKT = src.useKT;

            useAIfromDB = src.useAIfromDB;

            FWPlace = src.FWPlace;


            // 도공 광학계 선택
            useCSVAll = src.useCSVAll;
            useCSVCross = src.useCSVCross;
            useCSVLimit = src.useCSVLimit;
            useCSVSlit = src.useCSVSlit;
            useCSVStraight = src.useCSVStraight;
            useCSVSVJH = src.useCSVSVJH;
            useCSVSVMH = src.useCSVSVMH;

            // 생산 가능 경과시간
            prodAvaliableSpan = src.prodAvaliableSpan;
            xOffset = src.xOffset;
        }
    }

}
