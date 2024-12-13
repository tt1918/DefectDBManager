using DefectDBManager.DB;
using Oracle.ManagedDataAccess.Client;
using System;
using System.Collections.Generic;
using System.Data.Entity;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement.ProgressBar;

namespace DefectDBManager
{
	[Guid("D2BFB37D-6BD6-4E10-8A9C-06BCB04F0CFD")]
	[StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct Defect
	{
		[MarshalAs(UnmanagedType.I4)]
		public Int32 frameNum;
		[MarshalAs(UnmanagedType.I4)]
		public Int32 PcNo;
		[MarshalAs(UnmanagedType.I4)]
		public Int32 index;
		[MarshalAs(UnmanagedType.I4)]
		public Int32 defectClass;
		[MarshalAs(UnmanagedType.I4)]
		public Int32 marking;
		[MarshalAs(UnmanagedType.I4)]
		public Int32 vMax;
		[MarshalAs(UnmanagedType.R8)]
		public double posX;
		[MarshalAs(UnmanagedType.R8)]
		public double posY;
		[MarshalAs(UnmanagedType.R8)]
		public double sizeX;
		[MarshalAs(UnmanagedType.R8)]
		public double sizeY;
		[MarshalAs(UnmanagedType.BStr)]
		public string time;
	}

    [Guid("F5C11344-7DAB-4AE4-B94F-983F777A8A23")]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
	public struct MarkingData
	{
        [MarshalAs(UnmanagedType.BStr)]
        public string BCNO;
        [MarshalAs(UnmanagedType.BStr)]
        public string FLTNO;        // DB 탐색에서 얻어오는 불량 순번
        [MarshalAs(UnmanagedType.BStr)] 
		public string FAULTID;      // 불량 항목 같음
        [MarshalAs(UnmanagedType.R8)]
        public double OFFSET;
        [MarshalAs(UnmanagedType.R4)]
        public float YPOS_M;
        [MarshalAs(UnmanagedType.R4)]
        public float XPOS_M;
        [MarshalAs(UnmanagedType.R4)]
        public float SIZE_X;
        [MarshalAs(UnmanagedType.R4)]
        public float SIZE_Y;
        [MarshalAs(UnmanagedType.R4)]
        public float XOFFSET;
        [MarshalAs(UnmanagedType.Bool)]
        public bool UseCSVResult;
        [MarshalAs(UnmanagedType.I4)]
        public Int32 DefectLine;
        [MarshalAs(UnmanagedType.I4)]
        public Int32 CAM_NO;
    }

    [Guid("0C33BE40-DF66-4F24-9EA5-060A9E10EBB2")]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct MarkingAreaDefect
	{
        [MarshalAs(UnmanagedType.I4)]
        public Int32 idx;
        [MarshalAs(UnmanagedType.R4)]
        public float stX;                // 구간 마킹 시작 위치 X mm
        [MarshalAs(UnmanagedType.R4)]
        public float stY;                // 구간 마킹 시작 위치 Y mm
        [MarshalAs(UnmanagedType.R4)]
        public float edX;				// 구간 마킹 넓이 mm
        [MarshalAs(UnmanagedType.R4)]
        public float edY;				// 구간 마킹 높이 mm
    }

    [Guid("784F167B-0CFE-4343-AAF9-AFD2960ED87E")]
    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct LotSearchResult
	{
        [MarshalAs(UnmanagedType.BStr)]
        public string LotNo;
        [MarshalAs(UnmanagedType.BStr)]
        public string BCNO;
        [MarshalAs(UnmanagedType.BStr)]
		public string Line;
		[MarshalAs(UnmanagedType.BStr)]
		public string DateST;
        [MarshalAs(UnmanagedType.BStr)]
        public string TimeST;
        [MarshalAs(UnmanagedType.BStr)]
        public string DateED;
        [MarshalAs(UnmanagedType.BStr)]
        public string TimeED;
        [MarshalAs(UnmanagedType.I4)]
        public Int32 DefectCnt;
        [MarshalAs(UnmanagedType.R4)]
        public float Length;
        [MarshalAs(UnmanagedType.R4)]
        public float DefectPerM;
    }

    [ComVisible(true)]
    [Guid("33C8457C-7482-479E-8AB0-7A4E295F7360")]
    [InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface ICsvReadingEvents
    {
        void EventEndCsvReading(int evtID);
    }

    [ComVisible(true)]
    [Guid("1D93206A-C196-41BB-9E4A-160D297C6342")]
	[InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
	public interface ICallClass
	{
		bool ReadDBFile(string path);
		Defect[] SearchDefects(int classID);
		bool ReadCSVFile(string path);
        void ReroadDestFile();
        Defect[] GetDefect();
		void ResetDefectsData();
		bool IsDownLoadComplete();
		Defect[] GetDefectListRange(double start, double end);

		void ShowViewer(bool isNext);
		void AddEventCsvReading(ICsvReadingEvents csvReadingEvents);
		void RemoveEventCsvReading(ICsvReadingEvents csvReadingEvents);
		void CelarEventCsvReading();
        MarkingData[] GetMarkingData(bool isNext);
		int GetMarkingDefectMeter();
		MarkingData[] GetMarkDefectData(string bcno, double start, double end);
        bool UseAreaDelCheck();
        MarkingAreaDefect[] GetMarkAreaDelDefectData(string bcno, double start, double end, ref int count);
        string GetLotName(bool isNext);
		int GetCSV_Type();
		bool IsEsUse(bool isNext);
        bool IsTgUse(bool isNext);
        bool IsEtcUse(bool isNext);
        void LotChange();
		int GetSearchResut(bool isNext);
        int GetSearchProgress(bool isNext);
        void SearchLot(string lotName, bool isNext, int vendor, bool useES, bool useTG, bool useETC);
        LotSearchResult[] GetSearchLotResults(bool isNext);
        int GetBcdReadWarningM();
		int GetBcdReadErrorM();
		int SearchModel(string lotName);
        void SearchAreaDel(string lotName, bool isNext);
        void SearchCSV(string lotName, string filePath, bool isNext, int vendor, bool useES, bool useTG, bool useETC);
        void SearchAreaDelCSV(string lotName, string filePath, bool isNext);
        int GetSearchModelCount();
        string[] GetSearchModelResult();
		string[] GetLoadedBCNO(bool isNext, ref int size);
		void SetLanguage(int index);
        void ResetDBData(bool isNext);
    }

    [ComVisible(true)]
    [ClassInterface(ClassInterfaceType.AutoDispatch)]
    [Guid("83AF4738-A82D-4D9C-917D-8E4202727D57")]
	public class Defects : ICallClass
	{
        private bool isDownloadComplete = false;
		private static List<Defect> defects = null;
		private static List<MarkingData> markingData = null;
		private static List<MarkingAreaDefect> markingAreaDefects = null;
        
		private Thread thread = null;

        public DbManager DBManager
		{
			get { return dbManager; }
		}
		private DbManager dbManager = null;

		public FormDB _FormDB_Now
		{
			get { return dbManager._FormDB_Now; }
		}
        public FormDB _FormDB_Next
        {
            get { return dbManager._FormDB_Next; }
        }

        public List<ICsvReadingEvents> _CsvReadingEventsListener =new List<ICsvReadingEvents>();

        public Defects()
		{
			defects = new List<Defect>();
            dbManager = new DbManager(this);
            markingData = new List<MarkingData>();
            markingAreaDefects = new List<MarkingAreaDefect>();
            dbManager._FormDB_Now.OnEndJob += new DelegateEndJob(OnEventEndCsvReding);
            dbManager._FormDB_Next.OnEndJob += new DelegateEndJob(OnEventEndCsvReding);
            dbManager.OnProcessEvent+= new DelegateProcessEvent(OnEventEndCsvReding);

            //Application.ThreadException += Application_ThreadException;
            //Application.SetUnhandledExceptionMode(UnhandledExceptionMode.Automatic);
            //AppDomain.CurrentDomain.UnhandledException += CurrentDomain_UnhandledException;

        }
		~Defects()
		{
			_CsvReadingEventsListener.Clear();

            dbManager._FormDB_Now.OnEndJob -= OnEventEndCsvReding;
            dbManager._FormDB_Next.OnEndJob -= OnEventEndCsvReding;
            dbManager.OnProcessEvent -= OnEventEndCsvReding;
            //dbManager._DestConfig.Write();
            defects?.Clear();
			markingData?.Clear();
			markingAreaDefects?.Clear();
        }
        //private static void Application_ThreadException(object sender, System.Threading.ThreadExceptionEventArgs e)
        //{
        //    Log.Write("[Error] Application_ThreadException  " + e.Exception.Message);
        //    Log.Write("[Error] " + e.Exception.StackTrace);

        //    Directory.CreateDirectory("c:\\COSS\\DUMP");

        //    int pid = Process.GetCurrentProcess().Id;
        //    MinidumpWriter.MakeDump(String.Format(@"{0}\app_{1}.dmp", "c:\\COSS\\DUMP", DateTime.Now.ToString("yyyyMMdd_HHmmss")), pid);
        //    //Application.Exit(new System.ComponentModel.CancelEventArgs(false));
        //}
        //private static void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        //{
        //    Log.Write("[Error] CurrentDomain_UnhandledException  " + ((Exception)e.ExceptionObject).Message);
        //    Log.Write("[Error] " + ((Exception)e.ExceptionObject).StackTrace);
        //    Directory.CreateDirectory("c:\\COSS\\DUMP");

        //    int pid = Process.GetCurrentProcess().Id;
        //    MinidumpWriter.MakeDump(String.Format(@"{0}\domain_{1}.dmp", "c:\\COSS\\DUMP", DateTime.Now.ToString("yyyyMMdd_HHmmss")), pid);
        //}

        public static List<Defect> DefectsList
		{
            get { return defects; }
            set { defects = value; }
		}
		public Defect[] GetDefect()
		{			
			return DefectsList.ToArray();
		}

		public void ResetDefectsData()
		{
			defects.Clear();
		}

		public bool ReadCSVFile(string path)
		{
			DefectsDataView view = new DefectsDataView();
			view.Show();
			return view.ReadCSVFile(path);
		}
        public void ReroadDestFile()
		{
            if (this.thread != null)
            {
                this.thread.Join(100);
                this.thread = null;
            }
            
            thread = new Thread(this.readDestConfig);
            thread.Start();
        }
        private void readDestConfig()
        {
            dbManager._DestConfig.Read();
        }

        public bool ReadDBFile(string path)
		{
			defects.Clear();
			isDownloadComplete = false;
			DefectsDataView view = new DefectsDataView();
			view.Show();
			view.ReadDBFile(path);
			isDownloadComplete = true;
			return true;
		}

		public Defect[] SearchDefects(int classID)
		{
			var datas = defects.FindAll(x => x.defectClass == classID);
			return datas.ToArray();
		}

		public bool IsDownLoadComplete()
		{
			return isDownloadComplete;
		}

		public Defect[] GetDefectListRange(double start, double end)
		{
			var datas = defects.FindAll(x => x.posY >= start && x.posY <= end);
			return datas.ToArray();
		}

        public void ShowViewer(bool isNext)
        {
			int ii = 0;
			if (isNext == true) ii = 1;
            Trace.WriteLine($"isNext = {isNext}, useES={dbManager._DbProc[ii].DbOption.checkES}, useTG={dbManager._DbProc[ii].DbOption.checkTG}, useETC={dbManager._DbProc[ii].DbOption.checkETC}, dbNow = {dbManager._DbProc[ii].DbOption.dbWhen}");
            dbManager.ShowDBViewer(isNext);
        }

        public void AddEventCsvReading(ICsvReadingEvents csvReadingEvents)
		{
			_CsvReadingEventsListener.Add(csvReadingEvents);
        }

		public void RemoveEventCsvReading(ICsvReadingEvents csvReadingEvents)
		{
			_CsvReadingEventsListener.Remove(csvReadingEvents);
        }
		public void CelarEventCsvReading()
		{
			_CsvReadingEventsListener.Clear();
        }

        public void OnEventEndCsvReding(int evtID)
		{
            foreach (ICsvReadingEvents evt in _CsvReadingEventsListener)
			{
				evt.EventEndCsvReading(evtID);
			}
		}

		public MarkingData[] GetMarkingData(bool isNext)
		{
			markingData.Clear();

            MkFltDatumList oriData;
            if (isNext == false)
                oriData = DBManager._DbProc[0].ResultDefect.MarkFault.Data;
			else
                oriData = DBManager._DbProc[1].ResultDefect.MarkFault.Data;

			foreach(MarkingFaultDatum datum in oriData.Data)
			{
				MarkingData item = new MarkingData();
				item.DefectLine = datum.DefectLine;
				item.BCNO = datum.BCNO;
				item.CAM_NO = datum.CAM_NO;
				item.FAULTID = datum.FAULTID;
				item.FLTNO = datum.FLTNO;
				item.OFFSET = datum.OFFSET;
				item.UseCSVResult = datum.UseCSVResult;
				item.XOFFSET = datum.XOFFSET;
				item.XPOS_M = datum.XPOS_M;
				item.YPOS_M = datum.YPOS_M;
				item.SIZE_X = datum.SIZE_X;
                item.SIZE_Y = datum.SIZE_Y;
                markingData.Add(item);
            }

			return markingData.ToArray();
        }

		public MarkingData[] GetMarkDefectData(string bcno, double start, double end)
		{
			markingData.Clear();
            DBManager._DbProc[0].ResultDefect.MarkFault.GetData(bcno, start, end, ref markingData);
            
//          List<MarkingFaultDatum> oriData;
//          oriData = DBManager._DbProc[0].ResultDefect.MarkFault.Data;
//          foreach (MarkingFaultDatum datum in oriData)
//          {
//              if (datum.YPOS_M>=start && datum.YPOS_M <= end)
//	            {
//                 MarkingData item = new MarkingData();
//                 item.DefectLine = datum.DefectLine;
//                 item.BCNO = datum.BCNO;
//                 item.CAM_NO = datum.CAM_NO;
//                 item.FAULTID = datum.FAULTID;
//                 item.FLTNO = datum.FLTNO;
//                 item.OFFSET = datum.OFFSET;
//                 item.UseCSVResult = datum.UseCSVResult;
//                 item.XOFFSET = datum.XOFFSET;
//                 item.XPOS_M = datum.XPOS_M;
//                 item.YPOS_M = datum.YPOS_M;
//                 item.SIZE_X = datum.SIZE_X;
//                 item.SIZE_Y = datum.SIZE_Y;
//                 markingData.Add(item);
//             }
//         }

            return markingData.ToArray();
        }

		public MarkingAreaDefect[] GetMarkAreaDelDefectData(string bcno, double start, double end, ref int count)
		{
			markingAreaDefects.Clear();
            if(dbManager._DestConfig.UseAREADEL==true)
			{
                AREADELList delData = DBManager._DbProc[0]._DbResult.AREADEL;
				int idx = 0;
                foreach (AREADELData item in delData.Data)
                {
                    if (bcno != item.BCNO) continue;

                    if ((item.STR_MD <= start && end < item.END_MD) || (item.END_MD <= start && end < item.STR_MD) ||
                        (start <= item.STR_MD && item.STR_MD < end) || (start <= item.END_MD && item.END_MD < end))
                    {
                        MarkingAreaDefect data = new MarkingAreaDefect();


						//현재위치에 AreaDel마킹영역이 존재시
						data.idx = idx;
                        data.stX = item.STR_WD;
                        data.edX = item.END_WD;
                        data.stY = item.STR_MD;
                        data.edY = item.END_MD;
                        markingAreaDefects.Add(data);
                    }
					idx++;
                }
            }
			count = markingAreaDefects.Count;
            return markingAreaDefects.ToArray();
        }

        public int GetMarkingDefectMeter()
        {
			return dbManager._DestConfig.EverMarkDefectMeter;
		}

        public string GetLotName(bool isNext)
		{
			string lotName="";
			if (isNext == false)
				lotName = dbManager._DbProc[0].SearchLotName;
			else
                lotName = dbManager._DbProc[1].SearchLotName;

			return lotName;
        }

        public int GetCSV_Type()
		{
			int type = (int)dbManager._DestConfig.CSVType;

			return type;
		}


        public bool IsEsUse(bool isNext)
		{
			int idx = 0;
			if (isNext == true)
				idx = 1;
			return dbManager._Option[idx].checkES;
        }
        
		public bool IsTgUse(bool isNext)
		{
            int idx = 0;
            if (isNext == true)
                idx = 1;
            return dbManager._Option[idx].checkTG;
        }

        public bool IsEtcUse(bool isNext)
		{
            int idx = 0;
            if (isNext == true)
                idx = 1;
            return dbManager._Option[idx].checkETC;
        }

        public bool UseAreaDelCheck()
		{
			return dbManager._DestConfig.UseAREADEL;
		}

		// 예약랏을 현재랏으로 변경
		public void LotChange()
		{
            dbManager._DbProc[0]._DbResult = dbManager._DbProc[1]._DbResult;
            dbManager._DbProc[1]._DbResult = new DBLotInfo.LotData();

            if(dbManager._DbProc[1].DbOption.isLoadCSV==true)
            {
                dbManager._DbProc[0]._CSVLoadInfo = dbManager._DbProc[1]._CSVLoadInfo;
                dbManager._DbProc[1]._CSVLoadInfo = new List<CSVLoadInfo>();
            }
            
            ResultData oldMarkingData;
            oldMarkingData = dbManager._DbProc[0].ResultDefect;
            dbManager._DbProc[0].ResultDefect = dbManager._DbProc[1].ResultDefect;
            oldMarkingData.ResetAll();

			dbManager._DbProc[0].DbOption.Copy(dbManager._DbProc[1].DbOption);

            dbManager._DbProc[1].ResultDefect = new ResultData();
            dbManager._DbProc[1].ResetDataAll();
			
            dbManager._FormDB_Now.UpdateListViewFromLotChange();
            dbManager._FormDB_Next.UpdateListViewFromLotChange();

            // 변경한 옵션에 대해서 화면에 표시할 수 있도록 수정
            dbManager._FormDB_Now.IsHoldFW= true;

            Log.Write($"LotChange() useES={dbManager._DbProc[0].DbOption.checkES}, useTG={dbManager._DbProc[0].DbOption.checkTG}, useETC={dbManager._DbProc[0].DbOption.checkETC}, dbNow = {dbManager._DbProc[0].DbOption.dbWhen}");
        }

        public void SearchLot(string lotName, bool isNext, int vendor, bool useES, bool useTG, bool useETC)
		{
            if (dbManager._DbConn.Connection == null || dbManager._DbConn.Connection.State != System.Data.ConnectionState.Open)
                dbManager._DbConn.Connect();

            dbManager.SearchLot(lotName, isNext, vendor, useES, useTG, useETC);
        }

        public void SearchAreaDel(string lotName, bool isNext)
        {
            if (dbManager._DbConn.Connection == null || dbManager._DbConn.Connection.State != System.Data.ConnectionState.Open)
                dbManager._DbConn.Connect();

            dbManager.SearchAreaDel(lotName, isNext);
        }

        public LotSearchResult[] GetSearchLotResults(bool isNext)
		{
			List<LotSearchResult> results = new List<LotSearchResult>();
            if(dbManager._DestConfig.CSVType == eCSV_TYPE.NITTO || dbManager._DestConfig.CSVType == eCSV_TYPE.NITTO_RK ||
                dbManager._DestConfig.CSVType == eCSV_TYPE.NITTO_RTS)
                dbManager.GetSearchLotResultSummery_TOT(isNext, ref results);
            else
                dbManager.GetSearchLotResultSummery(isNext, ref results);

            return results.ToArray();
		}

		public int GetSearchResut(bool isNext)
		{
			int count;
			if (isNext == false)
				count = (int)_FormDB_Now._SearchRes;
			else
                count = (int)_FormDB_Next._SearchRes;
            return count;
		}

        public int GetSearchProgress(bool isNext)
        {
            int step= 0 ;
            if (isNext == false)
                step = (int)dbManager._DbProc[0].DB_Progress._CurrentStep;
            else
                step = (int)dbManager._DbProc[1].DB_Progress._CurrentStep;
            return step;
        }

        public int SearchModel(string lotName)
        {
            if (dbManager._DbConn.Connection==null || 
                dbManager._DbConn.Connection.State != System.Data.ConnectionState.Open)
                dbManager._DbConn.Connect();

            dbManager.SearchModel(lotName);
            return 0;
        }

        public void SearchCSV(string lotName, string filePath, bool isNext, int vendor, bool useES, bool useTG, bool useETC)
        {
            dbManager.SearchCSVFile(lotName, filePath, isNext, vendor, useES, useTG, useETC);
        }

        public void SearchAreaDelCSV(string lotName, string filePath, bool isNext)
        {
            dbManager.SearchAreaDelCSV(lotName, filePath, isNext);
        }

        public void ResetDBData(bool isNext)
        {
            dbManager.ResetDBData(isNext);
        }

        public int SearchModelDummy(string lotName)
        {
            dbManager._DbProc[0].SearchModelDummy(lotName);
            
            return 0;
        }

		public int GetSearchModelCount()
		{
            return dbManager._DbProc[2].SearchModelList.Count;
        }
        public string[] GetSearchModelResult()
        {
            return dbManager._DbProc[2].SearchModelList.ToArray();
        }

        public int GetBcdReadWarningM()
		{
			return dbManager._DestConfig.NoBcrWarning;
        }

		public int GetBcdReadErrorM()
		{
			return dbManager._DestConfig.NoBcrError;
        }
        public string[] GetLoadedBCNO(bool isNext, ref int size)
		{
            size = 0;
            if (isNext == false)// 현재랏
			{
				if(dbManager._DbProc[0]._DbResult.LoadedBcNo !=null)
					size = dbManager._DbProc[0]._DbResult.LoadedBcNo.Count;
                return dbManager._DbProc[0]._DbResult.LoadedBcNo.ToArray();
			}
			else
			{
                if (dbManager._DbProc[1]._DbResult.LoadedBcNo != null)
                    size = dbManager._DbProc[1]._DbResult.LoadedBcNo.Count;
                return dbManager._DbProc[1]._DbResult.LoadedBcNo.ToArray();
            }

			//return null;
        }

        public void SetLanguage(int index)
		{
			_FormDB_Now._LangType = index;
            _FormDB_Next._LangType = index;
        }
    }
}
