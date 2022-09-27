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

		void ShowViewer();
	}

	[Guid("83AF4738-A82D-4D9C-917D-8E4202727D57")]
	public class Defects : ICallClass
	{
		private bool isDownloadComplete = false;
		private static List<Defect> defects = null;

		public DbManager DBManager
		{
			get { return dbManager; }
		}
		private DbManager dbManager = null;

		public FormDB _FormDB
		{
			get { return dbManager._FormDB; }
		}

        public Defects()
		{
			defects = new List<Defect>();
            dbManager = new DbManager();
        }
		~Defects()
		{
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

		public void ShowViewer()
		{
			//DbManager dbManager = new DbManager();
			//dbManager.ShowDBViewer(false);
            dbManager.ShowDBViewer(false);
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
	}
}
