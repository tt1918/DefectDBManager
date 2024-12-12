using System;
using System.Collections.Generic;
using System.Data.Entity.Core.Common.EntitySql;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DefectDBManager
{
    public class UserDefectClass
    {
        public bool UseClassTrans;
        public List<string>[] DefectClass;
        public string[] TransFLTID;

        public UserDefectClass()
        {
            int cnt = System.Enum.GetValues(typeof(eUserDefectClass)).Length;

            DefectClass = new List<string>[cnt];
            for (int i = 0; i < cnt; i++)
                DefectClass[i] = new List<string>();
            TransFLTID = new string[cnt];
        }

        ~UserDefectClass()
        {
            if(DefectClass==null) return;
            for (int i = 0; i < DefectClass.Length; i++)
            {
                if (DefectClass[i]!=null && DefectClass[i].Count > 0)
                {
                    DefectClass[i].Clear();
                }
            }
        }

        public void Save()
        {
            string path = Define.UserDefectClassPath;

            File.Delete(path);

            NativeFunc.WriteIni(path, "OPTION", "Use Class Trans", this.UseClassTrans);

            for (int i = 0; i < DefectClass.Length; i++)
            {
                for (int j = 0; j < DefectClass[i].Count; j++)
                {
                    NativeFunc.WriteIni(path, $"DefectClass_{i}", $"DATA_{j}", this.DefectClass[i][j]);
                }
            }

            for (int i = 0; i < TransFLTID.Length; i++)
                NativeFunc.WriteIni(path, "TransFLTID", $"FLTID[{i}]", this.TransFLTID[i]);
        }

        public void Load()
        {
            int cnt = System.Enum.GetValues(typeof(eUserDefectClass)).Length;

            for (int i = 0; i < cnt; i++)
                DefectClass[i].Clear();

            string path = Define.UserDefectClassPath;
            this.UseClassTrans = NativeFunc.ReadIni(path, "OPTION", "Use Class Trans", false);

            for (int i = 0; i < cnt; i++)
            {
                for (int j = 0; j < Global.MaxEditDefectClassCnt; j++)
                {
                    string data = NativeFunc.ReadIni(path, $"DefectClass_{i}", $"DATA_{j}", "");
                    if (data.Length > 0)
                        DefectClass[i].Add(data);
                }
            }

            for (int i = 0; i < TransFLTID.Length; i++)
                this.TransFLTID[i] = NativeFunc.ReadIni(path, "TransFLTID", $"FLTID[{i}]", "");
        }

        public bool UpdateDefectLine(string faltID, ref int defectline)
        {
            int cnt = System.Enum.GetValues(typeof(eUserDefectClass)).Length;

            for (int crtClass = 0; crtClass < cnt; crtClass++)
            {
                foreach (string data in DefectClass[crtClass])
                {
                    if (data == faltID)
                    {
                        defectline = 21 + crtClass;
                        return true;
                    }
                }
            }
            return false;
        }

        public int ConvetLine2DefectType(int resultLine, eCSV_TYPE type, int bMark, int cMark, int tMark, ref int refClass)
        {
            int classNum = 0;
            int lineNum = resultLine % Global.MaxDefectLine;
            int camNum = resultLine / Global.MaxDefectLine;

            if (type == eCSV_TYPE.NITTO)
            {
                if (lineNum == 0) camNum = DefectType.ES_DOT;
                else if (lineNum == 1)
                {
                    if (cMark == 1) classNum = DefectType.ES_DOT;
                    else classNum = DefectType.KT_CROSSKUNIC;
                }
                else if (lineNum == 2)
                {
                    if (bMark == 1) classNum = DefectType.ES_DOT;
                    else classNum = DefectType.KT_DIFF;
                }
                else if (lineNum == 4)
                {
                    if (tMark == 1) classNum = DefectType.ES_DOT;
                    else classNum = DefectType.KT_DOT;
                }
                else if (lineNum == 5)
                {
                    if (tMark == 1) classNum = DefectType.ES_DOT;
                    else classNum = DefectType.KT_LINE;
                }
                else if (lineNum == 6) classNum = DefectType.BrcKoh;  //bMarking
                else if (lineNum == 8) classNum = DefectType.ES_DOT;
                else if (lineNum == 9) classNum = DefectType.DBKT_DOT;

                else if (lineNum == 21) classNum = DefectType.BrcKoh;
                else if (lineNum == 22) classNum = DefectType.BrcKoh;
                else if (lineNum == 23) classNum = DefectType.BrcKoh;
                else if (lineNum == 24) classNum = DefectType.BrcKoh;
                else if (lineNum == 25) classNum = DefectType.BrcKoh;
                else if (lineNum == 26) classNum = DefectType.BrcKoh;
                else classNum = DefectType.KT_DOT;
            }
            else if (type == eCSV_TYPE.NITTO_RTS)
            {
                if (lineNum == 6) classNum = DefectType.RK_BARCODE;         //바코드
                else if (lineNum == 7) classNum = DefectType.RK_ETC + camNum;   //DB-그외
                else if (lineNum == 8) classNum = DefectType.RK_ES + camNum;        //DB-연신
                else if (lineNum == 9) classNum = DefectType.RK_TG + camNum;        //DB-점착
                else if (lineNum == 21) classNum = DefectType.RK_USER_1;
                else if (lineNum == 22) classNum = DefectType.RK_USER_2;
                else if (lineNum == 23) classNum = DefectType.RK_USER_3;
                else if (lineNum == 24) classNum = DefectType.RK_USER_4;
                else if (lineNum == 25) classNum = DefectType.RK_USER_5;
                else if (lineNum == 26) classNum = DefectType.RK_USER_6;
                else classNum = DefectType.RK_ES;
            }
            else if (type == eCSV_TYPE.NITTO_RK || type == eCSV_TYPE.KORENO_RK_IJP)
            {
                if (lineNum == 6) classNum = DefectType.RK_BARCODE;         //바코드
                else if (lineNum == 7) classNum = DefectType.RK_ETC + camNum;       //DB-그외
                else if (lineNum == 8) classNum = DefectType.RK_ES + camNum;        //DB-연신
                else if (lineNum == 9) classNum = DefectType.RK_TG + camNum;        //DB-점착
                else if (lineNum == 21) classNum = DefectType.RK_USER_1;            //User Define 0x0161
                else if (lineNum == 22) classNum = DefectType.RK_USER_2;            //User Define 0x0162
                else if (lineNum == 23) classNum = DefectType.RK_USER_3;            //User Define 0x0163
                else if (lineNum == 24) classNum = DefectType.RK_USER_4;            //User Define 0x0164
                else if (lineNum == 25) classNum = DefectType.RK_USER_5;            //User Define 0x0165
                else if (lineNum == 26) classNum = DefectType.RK_USER_6;            //User Define 0x0166
                else if (lineNum == 27) classNum = DefectType.RK_USER_7;            //User Define 0x0167		
                else if (lineNum == 28) classNum = DefectType.RK_USER_8;            //User Define 0x0168		
                else if (lineNum == 29) classNum = DefectType.RK_USER_9;            //User Define 0x0169		
                else if (lineNum == 30) classNum = DefectType.RK_USER_10;           //User Define 0x016A	
                else classNum = DefectType.RK_ES;
            }
            else if (type == eCSV_TYPE.KORENO_RK)
            {
                //////////////////////////////////////////////////////////////////////////
                //서버
                if (lineNum == 0) classNum = DefectType.RK_ES;
                else if (lineNum == 1) classNum = DefectType.RK_TG + 1;         //CSV-크로스
                else if (lineNum == 2) classNum = DefectType.RK_TG + 2;         //CSV-경계
                else if (lineNum == 3) classNum = DefectType.RK_TG + 3;         //CSV-크로스(휘점)
                else if (lineNum == 4) classNum = DefectType.RK_TG + 4;         //CSV-정투과(점)
                else if (lineNum == 5) classNum = DefectType.RK_TG + 5;         //CSV-정투과(실)
                else if (lineNum == 6) classNum = DefectType.RK_BARCODE;
                else if (lineNum == 7) classNum = DefectType.RK_TG + 7;         //CSV-주기성	
                else if (lineNum == 8) classNum = DefectType.RK_TG + 8;         //CSV-동일결점	
                else if (lineNum == 9) classNum = DefectType.RK_BARCODE;
                else if (lineNum == 10) classNum = DefectType.RK_TG + 9;            //CSV,DB-스크래치
                else if (lineNum == 11) classNum = DefectType.RK_TG + 1;            //DB-크로스
                else if (lineNum == 12) classNum = DefectType.RK_TG + 2;            //DB-경계
                else if (lineNum == 13) classNum = DefectType.RK_TG;                //DB-에러
                else if (lineNum == 14) classNum = DefectType.RK_TG + 4;            //DB-정투과(점)
                else if (lineNum == 15) classNum = DefectType.RK_TG + 5;            //DB-정투과(실)
                else if (lineNum == 16) classNum = DefectType.RK_BARCODE;
                else if (lineNum == 17) classNum = DefectType.RK_TG + 3;            //DB 크로스(휘점)
                else if (lineNum == 18) classNum = DefectType.RK_TG + 7;            //DB 주기성					
                else if (lineNum == 19) classNum = DefectType.RK_TG + 8;            //DB-동일결점	

                else if (lineNum == 21) classNum = DefectType.RK_USER_1;            //User Define 0x0161
                else if (lineNum == 22) classNum = DefectType.RK_USER_2;            //User Define 0x0162
                else if (lineNum == 23) classNum = DefectType.RK_USER_3;            //User Define 0x0163
                else if (lineNum == 24) classNum = DefectType.RK_USER_4;            //User Define 0x0164
                else if (lineNum == 25) classNum = DefectType.RK_USER_5;            //User Define 0x0165
                else if (lineNum == 26) classNum = DefectType.RK_USER_6;            //User Define 0x0166
                else if (lineNum == 27) classNum = DefectType.RK_USER_7;            //User Define 0x0167		
                else if (lineNum == 28) classNum = DefectType.RK_USER_8;            //User Define 0x0168		
                else if (lineNum == 29) classNum = DefectType.RK_USER_9;            //User Define 0x0169		
                else if (lineNum == 30) classNum = DefectType.RK_USER_10;           //User Define 0x016A	
                else if (lineNum == (int)eFLTID.SAME2_81) classNum = DefectType.RK_TG + 8;            //동일결점				
                else if (lineNum == (int)eFLTID.SAME3_82) classNum = DefectType.RK_TG + 8;            //동일결점				
                else if (lineNum == (int)eFLTID.SAME4_83) classNum = DefectType.RK_TG + 8;            //동일결점				
                else classNum = DefectType.RK_TG;
                // AG는 일단은 Default 로 한다 @기선 140605
                //////////////////////////////////////////////////////////////////////////

                //////////////////////////////////////////////////////////////////////////
                //마킹기
                if (lineNum == 0) refClass = DefectType.ES_DOT;
                else if (lineNum == 1) refClass = DefectType.KT_CROSSKUNIC;        //CSV-크로스
                else if (lineNum == 2) refClass = DefectType.KT_DIFF;          //CSV-경계
                else if (lineNum == 3) refClass = DefectType.KT_CROSS;         //CSV-경계
                else if (lineNum == 4) refClass = DefectType.KT_DOT;           //CSV-정투과(점)
                else if (lineNum == 5) refClass = DefectType.KT_LINE;          //CSV-정투과(실)
                else if (lineNum == 6) refClass = DefectType.BrcKor;
                else if (lineNum == 7) refClass = DefectType.KT_PERIOD;            //CSV-주기성	
                else if (lineNum == 8) refClass = DefectType.KT_DOT;           //CSV-동일결점	
                else if (lineNum == 9) refClass = DefectType.BCR_MARK;
                else if (lineNum == 10) refClass = DefectType.Scratch;         //CSV,DB-스크래치
                else if (lineNum == 11) refClass = DefectType.DBKT_CROSSKUNIC; //203	//DB-크로스
                else if (lineNum == 12) refClass = DefectType.DBKT_DIFF;           //DB-경계
                else if (lineNum == 13) refClass = DefectType.DBKT_SLIT;           //DB-에러
                else if (lineNum == 14) refClass = DefectType.DBKT_DOT4;           //DB-정투과(점)
                else if (lineNum == 15) refClass = DefectType.DBKT_LINE4;      //DB-정투과(실)
                else if (lineNum == 16) refClass = DefectType.BrcKor;
                else if (lineNum == 17) refClass = DefectType.DBKT_CROSS;
                else if (lineNum == 18) refClass = DefectType.DBKT_PERIOD;
                else if (lineNum == 19) refClass = DefectType.DBKT_DOT4;
                else if (lineNum == 20) refClass = DefectType.DBKT_DOT;
                else if (lineNum == 21) refClass = DefectType.Spv;
                else if (lineNum == 22) refClass = DefectType.Spv;
                else if (lineNum == 23) refClass = DefectType.Spv;
                else if (lineNum == 24) refClass = DefectType.Spv;
                else if (lineNum == 25) refClass = DefectType.Spv;
                else if (lineNum == 26) refClass = DefectType.Spv;
                else if (lineNum == 27) refClass = DefectType.Spv;
                else if (lineNum == 28) refClass = DefectType.Spv;
                else if (lineNum == 29) refClass = DefectType.Spv;
                else if (lineNum == 30) refClass = DefectType.Spv;

                else if (lineNum == (int)eFLTID.JH_DOT_W_31) refClass = DefectType.Dot;
                else if (lineNum == (int)eFLTID.JH_DOT_B_32) refClass = DefectType.Dot;
                else if (lineNum == (int)eFLTID.JH_CIRCLE_W_33) refClass = DefectType.Dot;
                else if (lineNum == (int)eFLTID.JH_CIRCLE_B_34) refClass = DefectType.Dot;
                else if (lineNum == (int)eFLTID.JH_LINE_W_35) refClass = DefectType.Dot;
                else if (lineNum == (int)eFLTID.JH_LINE_B_36) refClass = DefectType.Dot;
                else if (lineNum == (int)eFLTID.JH_GROUP_37) refClass = DefectType.Line;

                else if (lineNum == (int)eFLTID.MH_DOT_W_51) refClass = DefectType.Dot;
                else if (lineNum == (int)eFLTID.MH_DOT_B_52) refClass = DefectType.Dot;
                else if (lineNum == (int)eFLTID.MH_CIRCLE_W_53) refClass = DefectType.Circle;
                else if (lineNum == (int)eFLTID.MH_CIRCLE_B_54) refClass = DefectType.Circle;
                else if (lineNum == (int)eFLTID.MH_LINE_W_55) refClass = DefectType.Line;
                else if (lineNum == (int)eFLTID.MH_LINE_B_56) refClass = DefectType.Line;
                else if (lineNum == (int)eFLTID.MH_GROUP_57) refClass = DefectType.Line;

                else if (lineNum == (int)eFLTID.NEL7_HJK_5_61) refClass = DefectType.Dot;
                else if (lineNum == (int)eFLTID.NEL7_HJK_4_62) refClass = DefectType.Dot;
                else if (lineNum == (int)eFLTID.NEL7_HJK_3_63) refClass = DefectType.Dot;
                else if (lineNum == (int)eFLTID.NEL7_HJK_2_64) refClass = DefectType.Dot;
                else if (lineNum == (int)eFLTID.NEL7_HJK_1_65) refClass = DefectType.Dot;

                else if (lineNum == (int)eFLTID.NEL8_W_71) refClass = DefectType.Dot;
                else if (lineNum == (int)eFLTID.NEL8_B_72) refClass = DefectType.Dot;
                else if (lineNum == (int)eFLTID.NEL8_WB_73) refClass = DefectType.Dot;

                else if (lineNum == (int)eFLTID.SAME2_81) refClass = DefectType.DBKT_DOT4;
                else if (lineNum == (int)eFLTID.SAME3_82) refClass = DefectType.DBKT_DOT4;
                else if (lineNum == (int)eFLTID.SAME4_83) refClass = DefectType.DBKT_DOT4;
                else if (lineNum == (int)eFLTID.SAME5_84) refClass = DefectType.DBKT_DOT4;
                else if (lineNum == (int)eFLTID.SAME6_85) refClass = DefectType.DBKT_DOT4;
                else if (lineNum == (int)eFLTID.SAME7_86) refClass = DefectType.DBKT_DOT4;
                else if (lineNum == (int)eFLTID.SAME8_87) refClass = DefectType.DBKT_DOT4;
                else refClass = DefectType.KT_DOT;
            }
            else
            {
                //server
                if (lineNum == 0) classNum = DefectType.ES_DOT;
                else if (lineNum == 1) classNum = DefectType.KT_CROSSKUNIC;     //CSV-크로스
                else if (lineNum == 2) classNum = DefectType.KT_DIFF;           //CSV-경계
                else if (lineNum == 3) classNum = DefectType.KT_CROSS;          //CSV-경계
                else if (lineNum == 4) classNum = DefectType.KT_DOT;            //CSV-정투과(점)
                else if (lineNum == 5) classNum = DefectType.KT_LINE;           //CSV-정투과(실)
                else if (lineNum == 6) classNum = DefectType.BrcKor;
                else if (lineNum == 7) classNum = DefectType.KT_PERIOD;         //CSV-주기성	
                else if (lineNum == 8) classNum = DefectType.KT_DOT;            //CSV-동일결점	
                else if (lineNum == 9) classNum = DefectType.BCR_MARK;
                else if (lineNum == 10) classNum = DefectType.Scratch;          //CSV,DB-스크래치
                else if (lineNum == 11) classNum = DefectType.DBKT_CROSSKUNIC;  //	203	//DB-크로스
                else if (lineNum == 12) classNum = DefectType.DBKT_DIFF;            //DB-경계
                else if (lineNum == 13) classNum = DefectType.DBKT_SLIT;            //DB-에러
                else if (lineNum == 14) classNum = DefectType.DBKT_DOT4;            //DB-정투과(점)
                else if (lineNum == 15) classNum = DefectType.DBKT_LINE4;       //DB-정투과(실)
                else if (lineNum == 16) classNum = DefectType.BrcKor;
                else if (lineNum == 17) classNum = DefectType.DBKT_CROSS;
                else if (lineNum == 18) classNum = DefectType.DBKT_PERIOD;
                else if (lineNum == 19) classNum = DefectType.DBKT_DOT4;
                else if (lineNum == 20) classNum = DefectType.DBKT_DOT;
                else if (lineNum == 21) classNum = DefectType.Spv;
                else if (lineNum == 22) classNum = DefectType.Spv;
                else if (lineNum == 23) classNum = DefectType.Spv;
                else if (lineNum == 24) classNum = DefectType.Spv;
                else if (lineNum == 25) classNum = DefectType.Spv;
                else if (lineNum == 26) classNum = DefectType.Spv;
                else if (lineNum == 27) classNum = DefectType.Spv;
                else if (lineNum == 28) classNum = DefectType.Spv;
                else if (lineNum == 29) classNum = DefectType.Spv;
                else if (lineNum == 30) classNum = DefectType.Spv;
                else classNum = DefectType.KT_DOT;
            }

            if (UseClassTrans == true)
            {
                int maxDefectClass = System.Enum.GetValues(typeof(eUserDefectClass)).Length;

                if (type == eCSV_TYPE.KORENO_RK)
                {
                    for (int i = 0; i < maxDefectClass; i++)
                    {
                        if ((lineNum == 21 + i) && (TransFLTID[i].Length > 0))
                            refClass = strToDecimal(TransFLTID[i]);
                    }
                }
                else if (type == eCSV_TYPE.NITTO ||
                    type == eCSV_TYPE.KORENO ||
                    type == eCSV_TYPE.NITTO_RTS ||
                    type == eCSV_TYPE.NITTO_RK ||
                    type == eCSV_TYPE.KORENO_RK_IJP)
                {
                    for (int i = 0; i < maxDefectClass; i++)
                    {
                        if ((lineNum == 21 + i) && (TransFLTID[i].Length > 0))
                        {
                            classNum = refClass = strToDecimal(TransFLTID[i]);
                        }
                    }
                }
            }
            return classNum;
        }

        private int strToDecimal(string str)
        {
            int val = int.MaxValue;

            if (str.Length == 0) return val;

            string upStr = str.ToUpper();
            if (upStr.IndexOf("0X") < 0)    val = Int32.Parse(upStr, System.Globalization.NumberStyles.HexNumber);
            else                            val = Int32.Parse(upStr.Substring(2), System.Globalization.NumberStyles.HexNumber);

            return val;
        }
    }

    
}
