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
                string vender = $"'{Vender}'";
                string message = "SELECT DISTINCT " +
                                 "* " +
                                 " " +
                                 " FROM PTRYLP WHERE YLMLOT LIKE " + vender +
                                 " AND YLSGEB LIKE 'LOGROLL%'";

                return message;
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
                string message = "SELECT * FROM XOFSMST WHERE XOFSMST.KYCD IN " +
                                    "(SELECT PTRY0P.Y0KYCD FROM PTRY0P WHERE PTRY0P.Y0KLOT IN(SELECT DISTINCT(TRIM(PTRYLP.YLSLOT)) FROM PTRYLP WHERE PTRYLP.YLMLOT = " + vender + " AND PTRYLP.YLSGEB = 'LOGROLL'))" +
                                    " AND XOFSMST.PPCD IN(SELECT PTRY0P.Y0PPCD FROM PTRY0P WHERE PTRY0P.Y0KLOT IN(SELECT DISTINCT(TRIM(PTRYLP.YLSLOT)) FROM PTRYLP WHERE PTRYLP.YLMLOT = " + vender + " AND PTRYLP.YLSGEB = 'LOGROLL'))" +
                                    " AND XOFSMST.LNCD IN(SELECT PTRY0P.Y0LNCD FROM PTRY0P WHERE PTRY0P.Y0KLOT IN(SELECT DISTINCT(TRIM(PTRYLP.YLSLOT)) FROM PTRYLP WHERE PTRYLP.YLMLOT = " + vender + " AND PTRYLP.YLSGEB = 'LOGROLL'))" +
                                    " AND XOFSMST.YLMZKN2 IN(SELECT DISTINCT PTRYLP.YLMZKN2 FROM PTRYLP WHERE PTRYLP.YLMLOT = " + vender + " AND PTRYLP.YLSGEB = 'LOGROLL')" +
                                    " AND XOFSMST.YLSZKN IN(SELECT PTRY0P.Y0ZKNM FROM PTRY0P WHERE PTRY0P.Y0KLOT IN(SELECT DISTINCT(TRIM(PTRYLP.YLSLOT)) FROM PTRYLP WHERE PTRYLP.YLMLOT = + " + vender + " AND PTRYLP.YLSGEB = 'LOGROLL'))";
                return message;
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
                string message = "SELECT * FROM AREADEL ADEL WHERE EXISTS (SELECT 'X'  FROM(SELECT TRIM(YL.YLMLOT) YLMLOT," +
                                    " TRIM(YL.YLSLOT) YLSLOT FROM PTRYLP YL WHERE YL.YLMLOT = " + vender + ")" +
                                    " EYL WHERE ADEL.LOTNO = EYL.YLMLOT OR ADEL.LOTNO = EYL.YLSLOT)";

                return message;
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

            public string GetQuery(List<PTRYLPdata> data, bool isModelSearch=false)
            {
                StringBuilder sbMsg = new StringBuilder();
                sbMsg.Append("SELECT * FROM PTRY0P WHERE");
                sbMsg.Append($" Y0KLOT='{Vender}'");

                if(isModelSearch==false)
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
                string message = "";

                string strOption = "";

                switch (type)
                {
                    case eFCD.ES:
                        strOption = "PPCD='100'";
                        break;

                    case eFCD.TG:
                        strOption = "(PPCD <> '100' AND PPCD <> '400')";
                        break;

                    case eFCD.ETC:
                        strOption = "PPCD='400'";
                        break;
                }

                message = "SELECT * FROM MRKCTLMST, PTRY0P WHERE PTRY0P.Y0KYCD=MRKCTLMST.KYCD AND PTRY0P.Y0PPCD=MRKCTLMST.PPCD AND" +
                                    " PTRY0P.Y0LNCD=MRKCTLMST.LNCD AND PTRY0P.Y0ZKNM=MRKCTLMST.ROLLNAME AND PTRY0P.Y0KASS <> 0 AND MRKCTLMST.MKCD='" + MKCD + "'" +
                                    " AND PTRY0P.Y0KLOT LIKE '" + Y0KLOT + "%' AND MRKCTLMST.MRKF1='1' AND " + strOption;

                return message;
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
                        query = "SELECT * FROM INSPDAT WHERE BCNO IN (SELECT DISTINCT BCNO FROM INSPDAT WHERE CONCAT(STRDT,STRTM)>='" + startTime[0].ToString("yyyyMMddhhmmss") +
                                "' AND CONCAT(ENDDT,ENDTM)<='" + endTime[1].ToString("yyyyMMddhhmmss") +
                                "' AND INSPDAT.CUSTCD='" + LNCD +
                                "') AND CONCAT(STRDT,STRTM)>='" + start_ES_Time[0].ToString("yyyyMMddhhmmss") +
                                "' AND CONCAT(ENDDT,ENDTM)<='" + end_ES_Time[1].ToString("yyyyMMddhhmmss") +
                                "' AND INSPDAT.CUSTCD='" + LNCD + "'";
                        break;

                    case 1:
                        query = "SELECT * FROM INSPDAT WHERE CONCAT(STRDT,STRTM)>='" + endTime[0].ToString("yyyyMMddhhmmss") +
                                "' AND CONCAT(ENDDT,ENDTM)<='" + endTime[1].ToString("yyyyMMddhhmmss") +
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

                message = "SELECT * FORM FAULTDAT WHERE CTLNO='" + CTLNO + "'";

                return message;
            }
        }
    }
}
