using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{
    interface ICSVFile
    {
        int Type { get; }
        string Path { get; }
        NittoDB DB { get; }

        bool Load();
        bool Save();
    }

    public class CSVKoh : ICSVFile
    {
        public int Type { get; }
        public string Path { get; private set; }
        public NittoDB DB { get; private set; }
        
        private Option option;
        private ResultData deferct;


        public CSVKoh(NittoDB db, string path)
        {
            Type = 0;
            Path = path;
            DB = db;
            option = DB.DbOption;
            deferct = db.ResultDefect;
        }

        public bool Load()
        {
            string text;
            bool useMask = option.searchOP.useMask;
            // 우선 fault Data 초기화.. 
            // 나중에 CrtParam._FaultData 내에 다른 데이터 초기화 해야하는지 확인이 필요함.
            // CrtParam._FaultData는 인덱스가 0으로 바뀌는데 CrtParam._MarkFaultData는 초기화 안 함
            // 나중에 확인 필요
            DB.ResultDefect.Data.Clear();

            int total = 0;

            RollDefectInfo rollInfo = new RollDefectInfo();
            string tmpBCInfo = null;
            rollInfo.InitData("");

            using (var file = new StreamReader(Path, Encoding.Default))
            {
                file.ReadLine(); // Title 

                while ((text = file.ReadLine()) != null)
                {
                    text = text.Replace("\"", "");
                    string[] items = text.Split(',');
                    FaultDatum tmpData = new FaultDatum();


                    // 데이터 인덱스 수정 필요함
                    tmpData.FLTNO = items[0].Trim();
                    tmpData.OFFSET = double.Parse(items[14]);
                    tmpData.XPOS_M = float.Parse(items[7]);
                    tmpData.YPOS_M = float.Parse(items[8]);
                    tmpData.SIZE_AREA = float.Parse(items[11]);
                    tmpData.MACNO = items[2].Trim();
                    tmpData.FLTID = items[5];
                    tmpData.CAM_NO = Int32.Parse(items[4]);

                    tmpBCInfo = items[12].Substring(0, items[12].IndexOf('_'));

                    deferct.Data.Add(tmpData);
                    if (useMask == true && DB.IsMaskedDefect(tmpData.XPOS_M, tmpData.YPOS_M) == true)
                        continue;

                    MarkingFaultDatum tmpMarkData = new MarkingFaultDatum();
                    tmpMarkData.FLTNO = tmpData.FLTNO;
                    tmpMarkData.OFFSET = tmpData.OFFSET;
                    tmpMarkData.YPOS_M = tmpData.YPOS_M;
                    tmpMarkData.XPOS_M = tmpData.XPOS_M;
                    tmpMarkData.SIZE = tmpData.SIZE_AREA;
                    tmpMarkData.FAULTID = tmpData.FLTID;
                    tmpMarkData.XOFFSET = 0.0f;
                    tmpMarkData.UseCSVResult = true;
                    tmpMarkData.CTLNO = "CSV";
                    tmpMarkData.MACNO = tmpData.MACNO;
                    tmpMarkData.CAM_NO = tmpData.CAM_NO;

                    if (option.checkES == true && option.checkTG == false && option.checkETC == false) // 연신
                        tmpMarkData.DefectLine = 8;
                    else if (option.checkES == false && option.checkTG == true && option.checkETC == false) // 점착    
                        tmpMarkData.DefectLine = 9;
                    else
                        tmpMarkData.DefectLine = 7;

                    deferct.MarkFault.Add(tmpMarkData);
                    rollInfo.AddYPos(tmpMarkData.YPOS_M);

                    total++;
                }

                if (tmpBCInfo != null)
                    rollInfo.InitData(tmpBCInfo, false);
            }

            DB._RollDefectInfo = rollInfo;

            CSVLoadInfo csvInfo = new CSVLoadInfo();
            csvInfo.DefectPerM = rollInfo.GetDefectPerM();
            csvInfo.BadCnt = rollInfo.BadCnt;
            csvInfo.LotNo = rollInfo.LotNo + "(CSV)";
            DB._CSVLoadInfo.Add(csvInfo);

            if (total == 0) return false;

            return true;
        }

        public bool Save()
        {
            bool isES = option.checkES;
            bool isTG = option.checkTG;

            StreamWriter wr = new StreamWriter(Path);

            wr.WriteLine("Header line");
            int idx = 0;
            foreach (MarkingFaultDatum data in DB.ResultDefect.MarkFault.Data)
            {
                StringBuilder sb = new StringBuilder();

                sb.Append($"\"{idx}\",");           // 0
                sb.Append($"\"\",");                // 1
                sb.Append($"\"{data.FAULTID}\",");    // 2

                //3
                if (isES) sb.Append("\"100\",");
                else if (isTG) sb.Append("\"400\",");
                else sb.Append("\"200\",");

                sb.Append($"\"{data.YPOS_M:F2}\","); // 4
                sb.Append($"\"{data.XPOS_M:F2}\","); // 5
                sb.Append($"\"{data.SIZE:F2}\",");   // 6
                sb.Append($"\"{data.SIZE_Y:F2}\","); // 7
                sb.Append($"\"{data.SIZE_X:F2}\","); // 8
                sb.Append($"\"{data.OFFSET:F2}\","); // 9
                sb.Append($"\"{data.CAM_NO}\",");    // 10
                sb.Append($"\"\",");                 // 11
                sb.Append($"\"\",");                 // 12
                sb.Append($"\"\",");                 // 13
                sb.Append($"\"\",");                 // 14
                sb.Append($"\"0\",");                // 15
                sb.Append($"\"{data.BCNO}\",");      // 16
                sb.Append("\"0\"");                  // 17
                wr.WriteLine(sb.ToString());

                idx++;
            }

            wr.Close();

            return true;
        }
    }

    public class CSVNitto : ICSVFile
    {
        public int Type { get; }
        public string Path { get; private set; }
        public NittoDB DB { get; private set; }

        private DestConfig destCfg;
        private Option option;
        private Param param;
        private ResultData deferct;
        public CSVNitto(NittoDB db, string path)
        {
            Type = 1;
            Path = path;
            DB = db;
            destCfg = db.DbDestConfig;
            option = db.DbOption;
            param = db.CrtParam;
            deferct = db.ResultDefect;
        }

        public bool Load()
        {
            string text;
            bool useMask = option.searchOP.useMask;

            // 에러 대비 초기화
            DB._RollDefectInfo = null;
            RollDefectInfo rollInfo = new RollDefectInfo();
            bool isInit = false;

            // fault data만 초기화 함
            DB.ResultDefect.Data.Clear();

            int dataCnt = 0;
            string strMACNO, strFltID, strbcr, strCode, strCamNo;
            string[] items;
            bool bBcrExist = false;

            float fVal;
            double dVal;
            int iVal;

            int totalLine = System.IO.File.ReadAllLines(Path).Length;
            Dictionary<string, int> dicCSVDefectCnt = DB._DbResult.DicCSVDefectCnt;

            int bcrDefectCnt;

            using (var file = new StreamReader(Path, Encoding.Default))
            {
                if (file == null)
                    return false;

                text = file.ReadLine(); // Title 

                while ((text = file.ReadLine()) != null)
                {
                    string text1 = text.Replace("\"", "");
                    items = text1.Split(',');

                    if (items.Length < 18) break;

                    FaultDatum tmpData = new FaultDatum();
                    MarkingFaultDatum tmpMarkData = new MarkingFaultDatum();

                    strMACNO = items[0];
                    strFltID = items[2];
                    strCode = items[3];
                    
                    if (float.TryParse(items[4], out fVal) == true) tmpData.YPOS_M = fVal;
                    else                                            tmpData.YPOS_M = 0.0f;
                    if (float.TryParse(items[5], out fVal) == true) tmpData.XPOS_M = fVal;
                    else                                            tmpData.XPOS_M = 0.0f;
                    if (float.TryParse(items[6], out fVal) == true) tmpData.SIZE_AREA = fVal;
                    else                                            tmpData.SIZE_AREA = 0.0f;
                    if (double.TryParse(items[9], out dVal) == true) tmpData.OFFSET = dVal;
                    else                                             tmpData.OFFSET = 0.0; 

                    strCamNo = items[10];
                    // BCNO 데이터 처리
                    strbcr = items[16];
                    tmpData.TBCNO = strbcr;
                    bBcrExist = false;
                    foreach (string bcr in DB._DbResult.LoadedBcNo)
                    {
                        if (bcr == strbcr)
                        {
                            bBcrExist = true;
                            break;
                        }
                    }
                    if (bBcrExist == false) DB._DbResult.LoadedBcNo.Add(strbcr);

                    if (useMask == true && DB.IsMaskedDefect(tmpData.XPOS_M, tmpData.OFFSET))
                        continue;

                    tmpData.MACNO = strMACNO;
                    if (Int32.TryParse(strCamNo, out iVal) == true) tmpData.CAM_NO = iVal;
                    else
                        tmpData.CAM_NO = 1;

                    tmpMarkData.OFFSET = tmpData.OFFSET;
                    tmpMarkData.YPOS_M = tmpData.YPOS_M;
                    tmpMarkData.XPOS_M = tmpData.XPOS_M;
                    tmpMarkData.SIZE = tmpData.SIZE_AREA;
                    tmpMarkData.UseCSVResult = true;
                    tmpMarkData.CAM_NO = tmpData.CAM_NO;
                    tmpMarkData.XOFFSET = 0.0f;
                    tmpMarkData.CTLNO = "CSV";
                    tmpMarkData.FAULTID = strFltID;
                    tmpMarkData.MACNO = tmpData.MACNO;
                    tmpMarkData.BCNO = tmpData.TBCNO;   // BCNO 데이터 입력 추가

                    int defectLine = 0;
                    if (destCfg.CSV_Ver == 1)
                    {
                        int code = Int32.Parse(strCode);
                        if (code == 100) defectLine = 8;// 연신
                        else if (code == 400) defectLine = 9;// 점착
                        else defectLine = 7;// 그 외
                        defectLine += Global.MaxDefectLine * Int32.Parse(strCamNo);

                        tmpMarkData.DefectLine = defectLine;

                        if (param._UserDefectClass.UpdateDefectLine(strFltID, ref defectLine) == true)
                            tmpMarkData.DefectLine = defectLine;
                    }
                    else
                    {
                        if (option.checkES == true && option.checkTG == false && option.checkETC == false) defectLine = 8;// 연신
                        else if (option.checkES == false && option.checkTG == true && option.checkETC == false) defectLine = 9;// 점착
                        else defectLine = 7;// 그 외
                    }

                    tmpMarkData.DefectLine = defectLine;

                    deferct.Data.Add(tmpData);
                    deferct.MarkFault.Add(tmpMarkData);

                    if (isInit == false && strbcr != null)
                    {
                        rollInfo.InitData(strbcr);
                        isInit = true;
                    }
                    rollInfo.AddYPos((float)(tmpData.YPOS_M / 1000.0f));

                    if(dicCSVDefectCnt.ContainsKey(strbcr))
                    {
                        bcrDefectCnt = dicCSVDefectCnt[strbcr];
                        bcrDefectCnt++;
                        dicCSVDefectCnt[strbcr] = bcrDefectCnt;
                    }
                    else
                        dicCSVDefectCnt.Add(strbcr, 1);

                    dataCnt++;
                }
            }

            DB._RollDefectInfo = rollInfo;
            DB._DbResult.DicCSVDefectCnt = dicCSVDefectCnt;

            // 데이터 후처리 추가 필요
            CSVLoadInfo csvInfo = new CSVLoadInfo();
            csvInfo.DefectPerM = rollInfo.GetDefectPerM();
            csvInfo.BadCnt = rollInfo.BadCnt;
            csvInfo.LotNo = rollInfo.LotNo + "(CSV)";
            DB._CSVLoadInfo.Add(csvInfo);
            if (dataCnt == 0) return false;

            return true;
        }

        public bool Save()
        {
            bool isES = option.checkES;
            bool isTG = option.checkTG;

            StreamWriter wr = new StreamWriter(Path);

            wr.WriteLine("Header line");
            int idx = 0;
            foreach (MarkingFaultDatum data in DB.ResultDefect.MarkFault.Data)
            {
                StringBuilder sb = new StringBuilder();

                sb.Append($"\"{idx}\",");           // 0
                sb.Append($"\"\",");                // 1
                sb.Append($"\"{data.FAULTID}\",");    // 2

                //3
                if (isES) sb.Append("\"100\",");
                else if (isTG) sb.Append("\"400\",");
                else sb.Append("\"200\",");

                sb.Append($"\"{data.YPOS_M:F2}\","); // 4
                sb.Append($"\"{data.XPOS_M:F2}\","); // 5
                sb.Append($"\"{data.SIZE:F2}\",");   // 6
                sb.Append($"\"{data.SIZE_Y:F2}\","); // 7
                sb.Append($"\"{data.SIZE_X:F2}\","); // 8
                sb.Append($"\"{data.OFFSET:F2}\","); // 9
                sb.Append($"\"{data.CAM_NO}\",");    // 10
                sb.Append($"\"\",");                 // 11
                sb.Append($"\"\",");                 // 12
                sb.Append($"\"\",");                 // 13
                sb.Append($"\"\",");                 // 14
                sb.Append($"\"0\",");                // 15
                sb.Append($"\"{data.BCNO}\",");      // 16
                sb.Append("\"0\"");                  // 17
                wr.WriteLine(sb.ToString());

                idx++;
            }

            wr.Close();

            return true;
        }
    }

    public class CSVKor : ICSVFile
    {
        public int Type { get; }
        public string Path { get; private set; }
        public NittoDB DB { get; private set; }

        private Option option;
        private DestConfig destCfg;
        private Param param;
        private ResultData deferct;
        public CSVKor(NittoDB db, string path)
        {
            Type = 2;
            Path = path;
            DB = db;
            option = db.DbOption;
            param = db.CrtParam;
            destCfg = db.DbDestConfig;
            deferct = db.ResultDefect;
        }

        public bool Load()
        {
            string text;
            bool useMask = option.searchOP.useMask;
            // 우선 fault Data 초기화.. 
            // 나중에 CrtParam._FaultData 내에 다른 데이터 초기화 해야하는지 확인이 필요함.
            // CrtParam._FaultData는 인덱스가 0으로 바뀌는데 CrtParam._MarkFaultData는 초기화 안 함
            // 나중에 확인 필요
            DB.ResultDefect.Data.Clear();

            int Optic1Cnt = 0;
            int Optic1ScCount = 0;
            int Optic2Cnt = 0;
            int Optic3Cnt = 0;
            int Optic4DotCnt = 0;
            int Optic4LineCnt = 0;
            int OpticFreqCnt = 0;
            int OpticSameCnt = 0;

            int nThru, nDiff, nCross, nThru1, nThru2, nFreq, nSame;
            nDiff = nThru = nCross = nThru1 = nThru2 = nFreq = nSame = 0;

            string tmpDefect;
            int tmpDefectId = 0, defectKind = 0;

            bool bCSVOption = false;

            float tmpLeng1, tmpLeng2;

            bool isEsCSV = false;

            RollDefectInfo_Kor rollInfo = new RollDefectInfo_Kor();

            using (var file = new StreamReader(Path, Encoding.Default))
            {
                if (file == null) return false;

                CSV_DEFECT_HEADER csvInspData = new CSV_DEFECT_HEADER();

                file.ReadLine(); // Title 
                text = file.ReadLine();
                text = text.Replace("\"", "");
                string[] items = text.Split(',');

                // INSPDATA 표시용 데이터 처리
                csvInspData.bcrInfo = items[2].Trim(' ');
                csvInspData.lotNo = csvInspData.bcrInfo.Substring(0, 7);
                csvInspData.rollNo = csvInspData.bcrInfo.Substring(csvInspData.bcrInfo.Length - 2, 2);
                csvInspData.startY = items[9];
                csvInspData.endY = items[10];
                csvInspData.rollY = items[13].Trim(' ');
                csvInspData.rollSY = items[14].Trim(' ');
                csvInspData.rollEY = items[15].Trim(' ');
                csvInspData.jig = items[3];
                csvInspData.machine = items[8];
                DB._CsvDefectHeader = csvInspData;

                rollInfo.InitData(csvInspData.bcrInfo);

                file.ReadLine();
                file.ReadLine();

                while ((text = file.ReadLine()) != null)
                {
                    text = text.Replace("\"", " ");
                    items = text.Split(',');
                    FaultDatum tmpData = new FaultDatum();
                    MarkingFaultDatum tmpMarkData = new MarkingFaultDatum();

                    tmpDefect = items[1].Trim(' ');
                    if (Int32.TryParse(tmpDefect, out int intVal) == true) tmpDefectId = intVal;
                    else tmpDefectId = 0;

                    if (Int32.TryParse(items[4], out intVal) == true) defectKind = intVal;
                    else defectKind = 0;

                    if (float.TryParse(items[7], out float floatVal) == true) tmpData.XPOS_M = floatVal;
                    else tmpData.XPOS_M = 0;

                    if (float.TryParse(items[8], out floatVal) == true) tmpData.YPOS_M = floatVal;
                    else tmpData.YPOS_M = 0;

                    if (float.TryParse(items[9], out floatVal) == true) tmpData.SIZE_AREA = floatVal;
                    else tmpData.SIZE_AREA = 0;

                    if (float.TryParse(items[10], out floatVal) == true) tmpData.SIZE_X = floatVal;
                    else tmpData.SIZE_X = 0;

                    if (float.TryParse(items[11], out floatVal) == true) tmpData.SIZE_Y = floatVal;
                    else tmpData.SIZE_Y = 0;

                    if (float.TryParse(items[14], out floatVal) == true) tmpData.OFFSET = floatVal;
                    else tmpData.OFFSET = 0;

                    if (tmpDefectId < 256) // 점착은 Class가 256 미만
                    {
                        bCSVOption = false;
                        switch (defectKind)
                        {
                            case 1: if (option.useCSVCross == true) bCSVOption = true; break;
                            case 2: if (option.useCSVLimit == true) bCSVOption = true; break;
                            case 3: if (option.useCSVSlit == true) bCSVOption = true; break;
                            case 4: if (option.useCSVStraight == true) bCSVOption = true; break;
                            case 5: if (option.useCSVSVJH == true) bCSVOption = true; break;
                            case 6: if (option.useCSVSVMH == true) bCSVOption = true; break;
                        }

                        nDiff = nThru = nCross = nThru1 = nThru2 = nFreq = nSame = 0;

                        if (option.useCSVAll == true || bCSVOption == true) // All or Check
                        {
                            if (tmpDefectId == 5)
                            {
                                if (destCfg.SkipData[(int)eOpticClass.OpticClass3].Check(tmpData.SIZE_X, tmpData.SIZE_Y, tmpData.SIZE_AREA))
                                    continue;
                                nFreq = 1;
                            }
                            else if (defectKind == 2)
                            {
                                if (destCfg.SkipData[(int)eOpticClass.OpticClass1].Check(tmpData.SIZE_X, tmpData.SIZE_Y, tmpData.SIZE_AREA))
                                    continue;
                                nDiff = 1;
                            }
                            else if (defectKind == 4)
                            {
                                tmpLeng1 = tmpData.SIZE_X / tmpData.SIZE_Y;
                                tmpLeng2 = tmpData.SIZE_Y / tmpData.SIZE_X;
                                if (tmpLeng1 > 2 || tmpLeng2 > 2) // 선
                                    nThru2 = 1;
                                else // 점
                                    nThru1 = 1;

                                if (tmpDefectId == 8)//동일 결점
                                {
                                    nSame = 1;
                                    nThru1 = nThru2 = 0;
                                }
                                else
                                {
                                    if (destCfg.SkipData[(int)eOpticClass.OpticClass2].Check(tmpData.SIZE_X, tmpData.SIZE_Y, tmpData.SIZE_AREA))
                                        continue;
                                }
                            }
                            else if (defectKind == 1)
                            {
                                if (tmpDefectId == 9)
                                    nCross = 9;
                                else
                                {
                                    nCross = 1;
                                    if (destCfg.SkipData[(int)eOpticClass.OpticClass4].Check(tmpData.SIZE_X, tmpData.SIZE_Y, tmpData.SIZE_AREA))
                                        continue;
                                }
                            }
                            else if (defectKind == 3)
                            {
                                nCross = 3;
                                if (destCfg.SkipData[(int)eOpticClass.OpticClass4].Check(tmpData.SIZE_X, tmpData.SIZE_Y, tmpData.SIZE_AREA))
                                    continue;
                            }
                            else if (defectKind == 5)
                            {
                                if (destCfg.SkipData[(int)eOpticClass.OpticClass5].Check(tmpData.SIZE_X, tmpData.SIZE_Y, tmpData.SIZE_AREA))
                                    continue;
                            }
                            else if (defectKind == 6)
                            {
                                if (destCfg.SkipData[(int)eOpticClass.OpticClass6].Check(tmpData.SIZE_X, tmpData.SIZE_Y, tmpData.SIZE_AREA))
                                    continue;
                            }
                            else
                            {
                                if (destCfg.SkipData[(int)eOpticClass.OpticClass2].Check(tmpData.SIZE_X, tmpData.SIZE_Y, tmpData.SIZE_AREA))
                                    continue;
                                nDiff = 1;
                            }

                            if (useMask && DB.IsMaskedDefect(tmpData.XPOS_M, tmpData.OFFSET) == true)
                                continue;

                            // thru 4 diff 2
                            if (nSame == 1)
                            {
                                if (option.useSameDefect)
                                {
                                    tmpMarkData.DefectLine = 19; //CSV-정투과(동일결점)
                                    OpticSameCnt++;
                                }
                                else continue;
                            }
                            else if (nThru1 == 1)
                            {
                                tmpMarkData.DefectLine = 4;	//CSV-정투과(점)
                                Optic4DotCnt++;
                            }
                            else if (nThru2 == 1)
                            {
                                tmpMarkData.DefectLine = 5;	//CSV-정투과(실)
                                Optic4LineCnt++;
                            }
                            else if (nDiff == 1)
                            {
                                tmpMarkData.DefectLine = 2;	//CSV-경계
                                Optic2Cnt++;
                            }
                            else if (nCross == 1)
                            {
                                tmpMarkData.DefectLine = 1;	//CSV-크로스
                                Optic1Cnt++;
                            }
                            else if (nCross == 3)
                            {
                                tmpMarkData.DefectLine = 3;	//CSV-크로스 휘점
                                Optic1Cnt++;
                            }
                            else if (nCross == 9)
                            {
                                tmpMarkData.DefectLine = 10;	//CSV-크로스 스크래치
                                Optic1ScCount++;
                            }
                            else if (nFreq == 1)
                            {
                                tmpMarkData.DefectLine = 7;	//CSV-주기성 불량
                                OpticFreqCnt++;
                            }
                            else if (tmpDefectId >= 10 && tmpDefectId <= 16)
                            {
                                tmpMarkData.DefectLine = (int)eFLTID.JH_DOT_W_31 + (tmpDefectId - 10);
                                param.AddCSVFalut(tmpMarkData.DefectLine);
                            }
                            else if (tmpDefectId >= 17 && tmpDefectId <= 23)
                            {
                                tmpMarkData.DefectLine = (int)eFLTID.MH_DOT_W_51 + (tmpDefectId - 17);
                                param.AddCSVFalut(tmpMarkData.DefectLine);
                            }
                            else if (tmpDefectId >= 24 && tmpDefectId <= 26)
                            {
                                tmpMarkData.DefectLine = (int)eFLTID.SAME2_81 + (tmpDefectId - 24);
                                param.AddCSVFalut(tmpMarkData.DefectLine);
                            }
                            else if (tmpDefectId >= 28 && tmpDefectId <= 32)
                            {
                                tmpMarkData.DefectLine = (int)eFLTID.NEL7_HJK_5_61 + (tmpDefectId - 28);
                                param.AddCSVFalut(tmpMarkData.DefectLine);
                            }
                            else if (tmpDefectId >= 33 && tmpDefectId <= 35)
                            {
                                tmpMarkData.DefectLine = (int)eFLTID.NEL8_W_71 + (tmpDefectId - 33);
                                param.AddCSVFalut(tmpMarkData.DefectLine);
                            }
                            else if (tmpDefectId >= 36 && tmpDefectId <= 39)
                            {
                                tmpMarkData.DefectLine = (int)eFLTID.SAME5_84 + (tmpDefectId - 36);
                                param.AddCSVFalut(tmpMarkData.DefectLine);
                            }
                            else
                            {
                                tmpMarkData.DefectLine = 0;
                                Optic3Cnt++;
                            }
                        }
                    }
                    else
                    {
                        if (useMask == true && DB.IsMaskedDefect(tmpData.XPOS_M, tmpData.OFFSET) == true)
                            continue;
                        param.AddESCount(tmpDefectId);

                        isEsCSV = true;
                        tmpMarkData.DefectLine = 0;
                    }

                    tmpData.MACNO = csvInspData.jig;

                    tmpMarkData.OFFSET = tmpData.OFFSET;
                    tmpMarkData.YPOS_M = tmpData.YPOS_M;
                    tmpMarkData.XPOS_M = tmpData.XPOS_M;
                    tmpMarkData.FAULTID = tmpDefectId.ToString();
                    tmpMarkData.MACNO = tmpData.MACNO;
                    tmpMarkData.UseCSVResult = true;
                    tmpMarkData.CTLNO = "CSV";
                    tmpMarkData.XOFFSET = 0.0f;
                    tmpMarkData.SIZE = tmpData.SIZE_AREA;
                    tmpMarkData.SIZE_X = tmpData.SIZE_X;
                    tmpMarkData.SIZE_Y = tmpData.SIZE_Y;

                    deferct.Data.Add(tmpData);
                    deferct.MarkFault.Add(tmpMarkData);
                    rollInfo.AddYPos(tmpData.YPOS_M);
                }

                float defect2M = 0;
                float fEnd = 0.0f, fRoll = 0.0f;
                bool isParse = true;
                if (float.TryParse(csvInspData.endY, out float tmpVal) == true) fEnd = tmpVal;
                else isParse = false;
                if (float.TryParse(csvInspData.rollY, out tmpVal) == true) fRoll = tmpVal;
                else isParse = false;

                if (isParse == true && fEnd > 0.0 && fRoll > 0.0)
                    defect2M = (float)rollInfo.BadCnt / ((fEnd * fRoll) / 1000.0f / 1000.0f);
                rollInfo.SetDefectPerM(defect2M);

                DB._RollDefectInfo = rollInfo;

                option.useKT = true;
            }

            int ktTotal = 0;

            if (isEsCSV == true)
            {
                ktTotal = param.GetTotalDefect();
            }
            else
            {
                param.Optic1Cnt = Optic1Cnt;
                param.Optic1ScCount = Optic1ScCount;
                param.Optic2Cnt = Optic2Cnt;
                param.Optic3Cnt = Optic3Cnt;
                param.Optic4DotCnt = Optic4DotCnt;
                param.Optic4LineCnt = Optic4LineCnt;
                param.OpticFreqCnt = OpticFreqCnt;
                param.OpticSameCnt = OpticSameCnt;

                if (option.useSameDefect == true)
                    ktTotal = Optic1Cnt + Optic2Cnt + Optic3Cnt + Optic4DotCnt + Optic4LineCnt + OpticFreqCnt + Optic1ScCount + OpticSameCnt;
                else
                    ktTotal = Optic1Cnt + Optic2Cnt + Optic3Cnt + Optic4DotCnt + Optic4LineCnt + OpticFreqCnt + Optic1ScCount;

                for (int i = 0; i < param.CSVFalutCount.Length; i++)
                    ktTotal += param.CSVFalutCount[i];
            }

            // 화면 표시는 상위 Form 단에서 처리
            CSVLoadInfo csvInfo = new CSVLoadInfo();
            csvInfo.DefectPerM = rollInfo.GetDefectPerM();
            csvInfo.BadCnt = rollInfo.BadCnt;
            csvInfo.LotNo = rollInfo.LotNo + "(CSV)";
            DB._CSVLoadInfo.Add(csvInfo);

            if (ktTotal == 0) return false;

            return true;
        }

        public bool Save()
        {
            bool isES = option.checkES;
            bool isTG = option.checkTG;

            StreamWriter wr = new StreamWriter(Path);

            wr.WriteLine("Header line");
            int idx = 0;
            foreach (MarkingFaultDatum data in DB.ResultDefect.MarkFault.Data)
            {
                StringBuilder sb = new StringBuilder();

                sb.Append($"\"{idx}\",");           // 0
                sb.Append($"\"\",");                // 1
                sb.Append($"\"{data.FAULTID}\",");    // 2

                //3
                if (isES) sb.Append("\"100\",");
                else if (isTG) sb.Append("\"400\",");
                else sb.Append("\"200\",");

                sb.Append($"\"{data.YPOS_M:F2}\","); // 4
                sb.Append($"\"{data.XPOS_M:F2}\","); // 5
                sb.Append($"\"{data.SIZE:F2}\",");   // 6
                sb.Append($"\"{data.SIZE_Y:F2}\","); // 7
                sb.Append($"\"{data.SIZE_X:F2}\","); // 8
                sb.Append($"\"{data.OFFSET:F2}\","); // 9
                sb.Append($"\"{data.CAM_NO}\",");    // 10
                sb.Append($"\"\",");                 // 11
                sb.Append($"\"\",");                 // 12
                sb.Append($"\"\",");                 // 13
                sb.Append($"\"\",");                 // 14
                sb.Append($"\"0\",");                // 15
                sb.Append($"\"{data.BCNO}\",");      // 16
                sb.Append("\"0\"");                  // 17
                wr.WriteLine(sb.ToString());

                idx++;
            }

            wr.Close();

            return true;
        }
    }

    public static class DefectCSV
    {
        #region Control CSV 
        public static bool Open(string path, NittoDB db)
        {
            ICSVFile file = null;
            switch (db.DbDestConfig.CSVType)
            {
                case eCSV_TYPE.NITTO:
                    file = new CSVKoh(db, path);
                    break;

                case eCSV_TYPE.NITTO_RTS:
                case eCSV_TYPE.NITTO_RK:
                    file = new CSVNitto(db, path);
                    break;

                case eCSV_TYPE.KORENO:
                case eCSV_TYPE.KORENO_RK:
                case eCSV_TYPE.KORENO_RK_IJP:
                    file = new CSVKor(db, path);
                    break;
            }

            return file.Load();
        }

        public static void Save(string path, NittoDB db)
        {
            ICSVFile file = null;
            switch (db.DbDestConfig.CSVType)
            {
                case eCSV_TYPE.NITTO:
                    file = new CSVKoh(db, path);
                    break;

                case eCSV_TYPE.NITTO_RTS:
                case eCSV_TYPE.NITTO_RK:
                    file = new CSVNitto(db, path);
                    break;

                case eCSV_TYPE.KORENO:
                case eCSV_TYPE.KORENO_RK:
                case eCSV_TYPE.KORENO_RK_IJP:
                    file = new CSVKor(db, path);
                    break;
            }

            file.Save();
        }
    }

    public static class AreaDelCSV
    { 
        public static bool Load(string path, NittoDB db)
        {
            bool bRes = true;
            string text;
            string[] items;

            if (path == "") return false;
            if (File.Exists(path) == false) return false;

            using (var file = new StreamReader(path, Encoding.Default))
            {
                if (file == null)
                    return false;

                text = file.ReadLine(); // Title 

                while ((text = file.ReadLine()) != null)
                {
                    string text1 = text.Replace("\"", "");
                    items = text1.Split(',');

                    if(items.Length!=9)
                    {
                        bRes = false;
                        break;
                    }

                    AREADELData data = new AREADELData();

                    data.KYCD = items[0];
                    data.PPCD = items[1];
                    data.LNCD = items[2];
                    data.LOTNO = items[3];
                    if (float.TryParse(items[4], out float valf) == true) data.STR_WD = valf;
                    else data.STR_WD = 0.0f;
                    if (float.TryParse(items[5], out valf) == true) data.END_WD = valf;
                    else data.END_WD = 0.0f;
                    if (float.TryParse(items[6], out valf) == true) data.STR_MD = valf;
                    else data.STR_MD = 0.0f;
                    if (float.TryParse(items[7], out valf) == true) data.END_MD = valf;
                    else data.END_MD = 0.0f;
                    data.BCNO = items[8];

                    db._DbResult.AREADEL_Data.Add(data);
                }
            }
            return bRes;
        }

        public static bool Save(string path, NittoDB db)
        {
            bool bRes = true;

            StreamWriter wr = new StreamWriter(path);

            wr.WriteLine("KYCD,PPCD,LNCD,LOTNO,STR_WD,END_WD,STR_MD,END_MD,BCNO");
            int idx = 0;
            foreach (AREADELData data in db._DbResult.AREADEL_Data)
            {
                StringBuilder sb = new StringBuilder();

                sb.Append($"\"{data.KYCD}\",");         // 1
                sb.Append($"\"{data.PPCD}\",");         // 2
                sb.Append($"\"{data.LNCD}\",");         // 3
                sb.Append($"\"{data.LOTNO}\",");        // 4
                sb.Append($"\"{data.STR_WD:F2}\",");    // 5
                sb.Append($"\"{data.END_WD:F2}\",");    // 6
                sb.Append($"\"{data.STR_MD:F2}\",");    // 7
                sb.Append($"\"{data.END_MD:F2}\",");     // 8
                sb.Append($"\"{data.BCNO}\"");          // 9
                wr.WriteLine(sb.ToString());

                idx++;
            }

            wr.Close();

            return bRes;
        }
        #endregion

    }
}
