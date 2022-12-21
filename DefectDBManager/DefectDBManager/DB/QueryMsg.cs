

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DefectDBManager
{

    public class QueryMsg
    {
        public string Vender;

        private string message;

        public class PTRYLP_Query : QueryMsg
        {
            public PTRYLP_Query()
            {
            }

            public PTRYLP_Query(string vender)
            {
                Vender = vender;
            }

            public string GetQuery()
            {
                string vender = $"'{Vender}%'";
                try
                {
                    message = "SELECT DISTINCT " +
                              "* " +
                              " " +
                              " FROM PTRYLP WHERE YLMLOT LIKE " + vender +
                              " AND YLSGEB LIKE 'LOGROLL%'";
                    return message;
                }
                catch(Exception ex)
                {
                    Log.WriteLog($"[Error] PTRYLP_Query Exception : {ex.Message}");
                    return "";
                }
            }
        }

        public class XOFSMST_Query : QueryMsg
        {
            public XOFSMST_Query()
            {
            }

            public XOFSMST_Query(string vender)
            {
                Vender = vender;
            }

            public string GetQuery()
            {
                string vender = $"'{Vender}'";
                try
                {
                    message = "SELECT * FROM XOFSMST WHERE XOFSMST.KYCD IN " +
                              "(SELECT PTRY0P.Y0KYCD FROM PTRY0P WHERE PTRY0P.Y0KLOT IN(SELECT DISTINCT(TRIM(PTRYLP.YLSLOT)) FROM PTRYLP WHERE PTRYLP.YLMLOT = " + vender + " AND PTRYLP.YLSGEB = 'LOGROLL'))" +
                              " AND XOFSMST.PPCD IN(SELECT PTRY0P.Y0PPCD FROM PTRY0P WHERE PTRY0P.Y0KLOT IN(SELECT DISTINCT(TRIM(PTRYLP.YLSLOT)) FROM PTRYLP WHERE PTRYLP.YLMLOT = " + vender + " AND PTRYLP.YLSGEB = 'LOGROLL'))" +
                              " AND XOFSMST.LNCD IN(SELECT PTRY0P.Y0LNCD FROM PTRY0P WHERE PTRY0P.Y0KLOT IN(SELECT DISTINCT(TRIM(PTRYLP.YLSLOT)) FROM PTRYLP WHERE PTRYLP.YLMLOT = " + vender + " AND PTRYLP.YLSGEB = 'LOGROLL'))" +
                              " AND XOFSMST.YLMZKN2 IN(SELECT DISTINCT PTRYLP.YLMZKN2 FROM PTRYLP WHERE PTRYLP.YLMLOT = " + vender + " AND PTRYLP.YLSGEB = 'LOGROLL')" +
                              " AND XOFSMST.YLSZKN IN(SELECT PTRY0P.Y0ZKNM FROM PTRY0P WHERE PTRY0P.Y0KLOT IN(SELECT DISTINCT(TRIM(PTRYLP.YLSLOT)) FROM PTRYLP WHERE PTRYLP.YLMLOT = + " + vender + " AND PTRYLP.YLSGEB = 'LOGROLL'))";
                    return message;
                }
                catch(System.Exception ex)
                {
                    Log.WriteLog($"[Error] XOFSMST_Query Exception : {ex.Message}");
                    return "";
                }
                
            }
        }

        public class AREADEL_Query : QueryMsg
        {
            public AREADEL_Query()
            {
            }
            public AREADEL_Query(string vender)
            {
                Vender = vender;
            }

            public string GetQuery()
            {
                string vender = $"'{Vender}'";
                try
                {
                    message = "SELECT * FROM AREADEL ADEL WHERE EXISTS (SELECT 'X'  FROM(SELECT TRIM(YL.YLMLOT) YLMLOT," +
                              " TRIM(YL.YLSLOT) YLSLOT FROM PTRYLP YL WHERE YL.YLMLOT = " + vender + ")" +
                              " EYL WHERE ADEL.LOTNO = EYL.YLMLOT OR ADEL.LOTNO = EYL.YLSLOT)";

                    return message;
                }
                catch (System.Exception ex)
                {
                    Log.WriteLog($"[Error] AREADEL_Query Exception : {ex.Message}");
                    return "";
                }
            }
        }

        public class PTRYOP_Query : QueryMsg
        {
            public PTRYOP_Query()
            {

            }

            public PTRYOP_Query(string name)
            {
                Vender = name;
            }

            public string GetQuery(List<PTRYLPdata> data, bool isModelSearch = false)
            {
                try
                {
                    StringBuilder sbMsg = new StringBuilder();
                    sbMsg.Append("SELECT * FROM PTRY0P WHERE");
                    sbMsg.Append($" Y0KLOT='{Vender}'");

                    if (isModelSearch == false)
                    {
                        foreach (PTRYLPdata datum in data)
                        {
                            string subID;
                            int nPos = datum.YLSLOT.IndexOf(' ');
                            if (nPos > 0)
                                subID = datum.YLSLOT.Substring(0, nPos);
                            else
                                subID = datum.YLSLOT;
                            sbMsg.Append($" OR Y0KLOT='{subID}'");
                        }
                    }
                    return sbMsg.ToString();
                }
                catch (System.Exception ex)
                {
                    Log.WriteLog($"[Error] PTRYOP_Query Exception : {ex.Message}");
                    return "";
                }
            }
        }

        public class MRKCTLMST_Query : QueryMsg
        {
            public string MKCD;
            public string Y0KLOT;

            public MRKCTLMST_Query()
            {

            }

            public string GetQuery(eFCD type)
            {
                try
                {
                    string strOption = "";

                    switch (type)
                    {
                        case eFCD.ES:
                            strOption = "PPCD='100'";
                            break;

                        case eFCD.TG:
                            strOption = "PPCD='400'";
                            break;

                        case eFCD.ETC:
                            strOption = "(PPCD <> '100' AND PPCD <> '400')";
                            break;
                    }

                    message = "SELECT * FROM MRKCTLMST, PTRY0P WHERE PTRY0P.Y0KYCD=MRKCTLMST.KYCD AND PTRY0P.Y0PPCD=MRKCTLMST.PPCD AND" +
                              " PTRY0P.Y0LNCD=MRKCTLMST.LNCD AND PTRY0P.Y0ZKNM=MRKCTLMST.ROLLNAME AND PTRY0P.Y0KASS <> 0 AND MRKCTLMST.MKCD='" + MKCD + "'" +
                              " AND PTRY0P.Y0KLOT LIKE '" + Y0KLOT + "%' AND MRKCTLMST.MRKF1='1' AND " + strOption;

                    return message;
                }
                catch (System.Exception ex)
                {
                    Log.WriteLog($"[Error] MRKCTLMST_Query Exception : {ex.Message}");
                    return "";
                }
            }
        }

        public class MRKCTLMST_DE_Query : QueryMsg
        {
            public string MKCD;
            public string Y0KLOT;

            public MRKCTLMST_DE_Query()
            {

            }

            public string GetQuery(eFCD type)
            {
                try
                {
                    string strOption = "";
                    string strMsg = $"PTRY0P.Y0KLOT LIKE '{Y0KLOT}%%'";
                    switch (type)
                    {
                        case eFCD.ES:
                            strOption = "PPCD='100'";
                            break;

                        case eFCD.TG:
                            strOption = "PPCD='400'";
                            break;

                        case eFCD.ETC:
                            strOption = "(PPCD <> '100' AND PPCD <> '400')";
                            break;
                    }

                    message = "SELECT * FROM MRKCTLMST, PTRY0P WHERE PTRY0P.Y0KYCD=MRKCTLMST.KYCD AND PTRY0P.Y0PPCD=MRKCTLMST.PPCD AND" +
                              " PTRY0P.Y0LNCD=MRKCTLMST.LNCD AND PTRY0P.Y0ZKNM=MRKCTLMST.ROLLNAME AND PTRY0P.Y0KASS <> 0 AND MRKCTLMST.MKCD='" + MKCD + "'" +
                              " AND " + strOption + " AND " + strMsg;

                    return message;
                }
                catch (System.Exception ex)
                {
                    Log.WriteLog($"[Error] MRKCTLMST_Query Exception : {ex.Message}");
                    return "";
                }
            }
        }

        public class INSPDATA_Query : QueryMsg
        {
            public enum eTargetTime
            {
                TimeStart = 0,
                TimeEnd,
                ES_TimeStart,
                ES_TimeEnd,
            }

            #region base time
            public DateTime StartTime = DateTime.Today;
            public DateTime EndTime = DateTime.Today;

            private DateTime[] startTime;
            private DateTime[] endTime;

            public int StartTimeGab = 30;
            public int EndTimeGab = 30;
            #endregion

            #region time for ES
            public DateTime ES_Start_Time = DateTime.Today;
            public DateTime ES_End_Time = DateTime.Today;

            private DateTime[] start_ES_Time;
            private DateTime[] end_ES_Time;

            public int Start_ES_TimeGab = 30;
            public int End_ES_TimeGab = 30;
            #endregion

            public string LNCD;

            public INSPDATA_Query()
            {
                startTime = new DateTime[2];
                endTime = new DateTime[2];

                start_ES_Time = new DateTime[2];
                end_ES_Time = new DateTime[2];
            }

            public INSPDATA_Query(string name)
            {
                Vender = name;

                startTime = new DateTime[2];
                endTime = new DateTime[2];

                start_ES_Time = new DateTime[2];
                end_ES_Time = new DateTime[2];
            }

            public bool SetTime(string time, eTargetTime type)
            {
                if (time.Length != 14)
                    return false;

                DateTime cvtTime;

                if (DateTime.TryParseExact(time, "yyyyMMddHHmmss", System.Globalization.CultureInfo.InvariantCulture, System.Globalization.DateTimeStyles.None, out cvtTime) != true)
                {
                    return false;
                }
                switch (type)
                {
                    case eTargetTime.TimeStart: StartTime = cvtTime; break;
                    case eTargetTime.TimeEnd: EndTime = cvtTime; break;
                    case eTargetTime.ES_TimeStart: ES_Start_Time = cvtTime; break;
                    case eTargetTime.ES_TimeEnd: ES_End_Time = cvtTime; break;
                }

                return true;
            }

            public string GetQuery(int type, Option opt)
            {
                string query = "";

                TimeSpan spanTime1 = new TimeSpan(0, opt.timeGabStMinute1, 0);
                TimeSpan spanTime2 = new TimeSpan(0, opt.timeGabStMinute2, 0);
                // 시간 계산
                startTime[0] = StartTime - spanTime1;
                startTime[1] = StartTime + spanTime2;

                spanTime1 = new TimeSpan(0, opt.timeGabEdMinute1, 0);
                spanTime2 = new TimeSpan(0, opt.timeGabEdMinute2, 0);
                endTime[0] = EndTime - spanTime1;
                endTime[1] = EndTime + spanTime2;

                spanTime1 = new TimeSpan(0, opt.timeGabEsStMinute1, 0);
                spanTime2 = new TimeSpan(0, opt.timeGabEsStMinute2, 0);
                start_ES_Time[0] = StartTime - spanTime1;
                start_ES_Time[1] = StartTime + spanTime2;

                spanTime1 = new TimeSpan(0, opt.timeGabEsEdMinute1, 0);
                spanTime2 = new TimeSpan(0, opt.timeGabEsEdMinute2, 0);
                end_ES_Time[0] = EndTime - spanTime1;
                end_ES_Time[1] = EndTime + spanTime2;

                switch (type)
                {
                    case 0:
                        query = "SELECT * FROM INSPDAT WHERE BCNO IN (SELECT DISTINCT BCNO FROM INSPDAT WHERE CONCAT(STRDT,STRTM)>='" + startTime[0].ToString("yyyyMMddHHmmss") +
                                "' AND CONCAT(ENDDT,ENDTM)<='" + endTime[1].ToString("yyyyMMddHHmmss") +
                                "' AND INSPDAT.CUSTCD='" + LNCD +
                                "') AND CONCAT(STRDT,STRTM)>='" + start_ES_Time[0].ToString("yyyyMMddHHmmss") +
                                "' AND CONCAT(ENDDT,ENDTM)<='" + end_ES_Time[1].ToString("yyyyMMddHHmmss") +
                                "' AND INSPDAT.CUSTCD='" + LNCD + "'";
                        break;

                    case 1:
                        query = "SELECT * FROM INSPDAT WHERE CONCAT(STRDT,STRTM)>='" + startTime[0].ToString("yyyyMMddHHmmss") +
                                "' AND CONCAT(ENDDT,ENDTM)<='" + endTime[1].ToString("yyyyMMddHHmmss") +
                                "' AND INSPDAT.CUSTCD='" + LNCD + "'";
                        break;
                }

                return query;
            }
        }

        public class FLTDAT_Query : QueryMsg
        {
            public string CTLNO = "";

            public string GetQuery()
            {
                string message = "";
                message = "SELECT * FROM FAULTDAT WHERE CTLNO='" + CTLNO + "'";
                //message = "FAULTDAT.CTLNO,FAULTDAT.FLTNO,FAULTDAT.OFFSET,FAULTDAT.XPOS_M,FAULTDAT.KND,FAULTDAT.CAMNO,FLTMST.FLTNAM,FAULTDAT.FLTID, " +
                //          "MRK_WRK_4.PPCD, FAULTDAT.YPOS_M,FAULTDAT.WID_M, MRK_WRK_4.X_OFFSET, INSPDAT.WIDTH FROM FAULTDAT,FLTMST,MRK_WRK_4,INSPDAT " + 
                //          "WHERE FAULTDAT.FLTID=MRK_WRK_4.FLTID AND FAULTDAT.FLTID=FLTMST.FLTID AND FAULTDAT.CTLNO = MRK_WRK_4.CTLNO AND " + 
                //          "FAULTDAT.AREA_M >= MRK_WRK_4.MIN_SIZE AND FAULTDAT.CTLNO = INSPDAT.CTLNO AND MRK_WRK_4.PPCD = INSPDAT.KTCD AND " + 
                //          "MRK_WRK_4.LNCD = INSPDAT.CUSTCD AND ( (INSPDAT.S_INSP<=INSPDAT.E_INSP AND FAULTDAT.OFFSET>=(INSPDAT.S_INSP-150000) " + 
                //          "AND FAULTDAT.OFFSET<=(INSPDAT.E_INSP+150000))   "+
                //          "OR (INSPDAT.S_INSP>INSPDAT.E_INSP AND FAULTDAT.OFFSET>=(INSPDAT.E_INSP-150000) AND FAULTDAT.OFFSET<=(INSPDAT.S_INSP+150000)) )AND " + 
                //          "INSPDAT.BCNO = 'EE11006-09'  AND (INSPDAT.KTCD = '100' OR INSPDAT.KTCD = '400' OR (INSPDAT.KTCD <> '100' AND INSPDAT.KTCD <> '400')) ORDER BY FAULTDAT.OFFSET";
                return message;
            }
        }

        public class FLTDAT_FAST_Query : QueryMsg
        {
            public string BCNO = "";
            public string GetQuery()
            {
                string strBCNO = $"'{BCNO}'";
                StringBuilder sbMsg = new StringBuilder(); 
                sbMsg.Append("SELECT FAULTDAT.CTLNO,FAULTDAT.FLTNO,FAULTDAT.OFFSET,FAULTDAT.XPOS_M,FAULTDAT.KND,FAULTDAT.CAMNO,FLTMST.FLTNAM,FAULTDAT.FLTID, ");
                sbMsg.Append("MRK_WRK_4.PPCD, FAULTDAT.YPOS_M,FAULTDAT.WID_M, MRK_WRK_4.X_OFFSET, INSPDAT.WIDTH ");
                sbMsg.Append("FROM FAULTDAT,FLTMST,MRK_WRK_4,INSPDAT ");
                sbMsg.Append("WHERE ");
                sbMsg.Append("FAULTDAT.FLTID=MRK_WRK_4.FLTID AND ");
                sbMsg.Append("FAULTDAT.FLTID=FLTMST.FLTID AND ");
                sbMsg.Append("FAULTDAT.CTLNO = MRK_WRK_4.CTLNO AND ");
                sbMsg.Append("FAULTDAT.AREA_M >= MRK_WRK_4.MIN_SIZE AND ");
                sbMsg.Append("FAULTDAT.CTLNO = INSPDAT.CTLNO AND ");
                sbMsg.Append("MRK_WRK_4.PPCD = INSPDAT.KTCD AND ");
                sbMsg.Append("MRK_WRK_4.LNCD = INSPDAT.CUSTCD AND ");
                sbMsg.Append("( (INSPDAT.S_INSP<=INSPDAT.E_INSP AND FAULTDAT.OFFSET>=(INSPDAT.S_INSP-150000) AND FAULTDAT.OFFSET<=(INSPDAT.E_INSP+150000))   ");
                sbMsg.Append("OR (INSPDAT.S_INSP>INSPDAT.E_INSP AND FAULTDAT.OFFSET>=(INSPDAT.E_INSP-150000) AND FAULTDAT.OFFSET<=(INSPDAT.S_INSP+150000)) )AND ");
                sbMsg.Append("INSPDAT.BCNO = ");
                sbMsg.Append(strBCNO);
                sbMsg.Append("  AND ");
                sbMsg.Append("(INSPDAT.KTCD = '100' OR INSPDAT.KTCD = '400' OR (INSPDAT.KTCD <> '100' AND INSPDAT.KTCD <> '400')) ORDER BY FAULTDAT.OFFSET");

                return sbMsg.ToString();
            }
        }
    }
}
