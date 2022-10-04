using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

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
        [MarshalAs(UnmanagedType.R4)]
        public float OFFSET;
        [MarshalAs(UnmanagedType.R4)]
        public float YPOS_M;
        [MarshalAs(UnmanagedType.R4)]
        public float XPOS_M;
        [MarshalAs(UnmanagedType.R4)]
        public float XOFFSET;
        [MarshalAs(UnmanagedType.Bool)]
        public bool UseCSVResult;
        [MarshalAs(UnmanagedType.I4)]
        public Int32 DefectLine;
        [MarshalAs(UnmanagedType.I4)]
        public Int32 CAM_NO;
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
		Defect[] GetDefect();
		void ResetDefectsData();
		bool IsDownLoadComplete();
		Defect[] GetDefectListRange(double start, double end);

		void ShowViewer(bool isNext);
		void AddEventCsvReading(ICsvReadingEvents csvReadingEvents);
		void RemoveEventCsvReading(ICsvReadingEvents csvReadingEvents);
        MarkingData[] GetMarkingData(bool isNext);
	}


    [ComVisible(true)]
    [Guid("83AF4738-A82D-4D9C-917D-8E4202727D57")]
	public class Defects : ICallClass
	{
        private bool isDownloadComplete = false;
		private static List<Defect> defects = null;
		private static List<MarkingData> markingData = null;

		public DbManager DBManager
		{
			get { return dbManager; }
		}
		private DbManager dbManager = null;

		public FormDB _FormDB
		{
			get { return dbManager._FormDB; }
		}

		public List<ICsvReadingEvents> _CsvReadingEventsListener =new List<ICsvReadingEvents>();

        public Defects()
		{
			defects = new List<Defect>();
            dbManager = new DbManager(this);

			dbManager._FormDB.OnEndCsvReading += new DelegateEndCsvReading(OnEventEndCsvReding);
        }
		~Defects()
		{
			dbManager._FormDB.OnEndCsvReading -= OnEventEndCsvReding;
            dbManager._DestConfig.Write();
            defects.Clear();
		}
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

		public void OnEventEndCsvReding(int evtID)
		{
            foreach (ICsvReadingEvents evt in _CsvReadingEventsListener)
			{
				evt.EventEndCsvReading(evtID);
			}
		}

		public MarkingData[] GetMarkingData(bool isNext)
		{
			if (markingData != null && markingData.Count > 0)
			{
				markingData.Clear();
				markingData = null;
			}

            markingData = new List<MarkingData>();
			List<MarkingFaultDatum> oriData;
            if (isNext == false)
                oriData = DBManager._ResultData[0].MarkFault.Data;
			else
                oriData = DBManager._ResultData[1].MarkFault.Data;

			foreach(MarkingFaultDatum datum in oriData)
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
				markingData.Add(item);
            }

			return markingData.ToArray();
        }
    }
}
