using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Data.SQLite;
using System.IO;

namespace DefectDBManager
{
	public partial class DefectsDataView : MetroFramework.Forms.MetroForm
	{
		List<Defect> defectList = new List<Defect>();
		private int pageNum = 0;
		public DefectsDataView()
		{
			InitializeComponent();
		}

		private void DefectsDataView_Load(object sender, EventArgs e)
		{
			SettingDefectListView();
			pageNumber.Value = pageNum;
			pageNumber.DecimalPlaces = 0;
			pageNumber.Increment = 1;
		}

		private void SettingDefectListView()
		{
			// listview 아이템을 업데이트 하기 시작.
			// 업데이트가 끝날 때까지 UI 갱신 중지.
			defectListView.BeginUpdate();

			// 뷰 모드 지정
			defectListView.View = View.Details;
			
			defectListView.Columns.Add("Time", 100, HorizontalAlignment.Center);
			defectListView.Columns.Add("FrameNum", 100, HorizontalAlignment.Center);
			defectListView.Columns.Add("PcNo", 100, HorizontalAlignment.Center);
			defectListView.Columns.Add("DefectClass", 100, HorizontalAlignment.Center);
			defectListView.Columns.Add("Symbol", 100, HorizontalAlignment.Center);
			defectListView.Columns.Add("X", 100, HorizontalAlignment.Center);
			defectListView.Columns.Add("Y", 100, HorizontalAlignment.Center);
			defectListView.Columns.Add("SizeX", 100, HorizontalAlignment.Center);
			defectListView.Columns.Add("SizeY", 100, HorizontalAlignment.Center);
			defectListView.Columns.Add("Marking", 100, HorizontalAlignment.Center);
			defectListView.Columns.Add("Vmax", 100, HorizontalAlignment.Center);

			defectListView.EndUpdate();
			this.Refresh();
		}

		private void btnLoadDB_Click(object sender, EventArgs e)
		{
			var fileContent = string.Empty;
			var filePath = string.Empty;
			using (OpenFileDialog openFileDialog = new OpenFileDialog())
			{
				openFileDialog.InitialDirectory = "C:\\";
				openFileDialog.Filter = "DB files (*.db)|*.db|All files (*.*)|*.*";
				openFileDialog.FilterIndex = 2;
				openFileDialog.RestoreDirectory = true;

				if (openFileDialog.ShowDialog() == DialogResult.OK)
				{
					filePath = openFileDialog.FileName;
					ReadDBFile(filePath);
				}
			}
		}

		public void ReadDBFile(string path)
		{
			string strConn = path;
			if (!File.Exists(path))
			{
				MessageBox.Show($"{path} is not exists");
				return;
			}
			using (SQLiteConnection conn = new SQLiteConnection("Data Source=" + strConn))
			{
				conn.Open();
				var sql = $"SELECT * FROM Defect";
				var cmd = new SQLiteCommand(sql, conn);
				try
				{
					var rdr = cmd.ExecuteReader();
					while (rdr.Read())
					{
						var data = new Defect();
						DateTime time = rdr.GetDateTime(1);
						data.time = time.ToString("yyyyMMddHHmmss");
						Int32.TryParse(rdr["FrameNum"].ToString(), out data.frameNum);
						Int32.TryParse(rdr["PcNo"].ToString(), out data.PcNo);
						Int32.TryParse(rdr["DefectClass"].ToString(), out data.defectClass);
						double.TryParse(rdr["PosX"].ToString(), out data.posX);
						double.TryParse(rdr["PosY"].ToString(), out data.posY);
						double.TryParse(rdr["SizeX"].ToString(), out data.sizeX);
						double.TryParse(rdr["SizeY"].ToString(), out data.sizeY);
						Int32.TryParse(rdr["Marking"].ToString(), out data.marking);
						Int32.TryParse(rdr["Vmax"].ToString(), out data.vMax);
						defectList.Add(data);

					}
				}
				catch (Exception ex)
				{
					MessageBox.Show($"{ex.ToString()}");
				}

				Defects.DefectsList = defectList;

				defectListView.BeginUpdate();
				pageNum = 0;

				var listData = defectList.GetRange(0, 100);

				foreach (var defect in listData)
				{
					ListViewItem lvi = new ListViewItem(defect.time);
					lvi.SubItems.Add(defect.frameNum.ToString());
					lvi.SubItems.Add(defect.PcNo.ToString());
					lvi.SubItems.Add(defect.defectClass.ToString());
					lvi.SubItems.Add("");
					lvi.SubItems.Add(defect.posX.ToString());
					lvi.SubItems.Add(defect.posY.ToString());
					lvi.SubItems.Add(defect.sizeX.ToString());
					lvi.SubItems.Add(defect.sizeY.ToString());
					lvi.SubItems.Add(defect.marking.ToString());
					lvi.SubItems.Add(defect.vMax.ToString());

					defectListView.Items.Add(lvi);

				}

				defectListView.EndUpdate();
				this.Refresh();
			}
		}

		private void pageNumber_ValueChanged(object sender, EventArgs e)
		{
			pageNum = (int)pageNumber.Value;
			if (defectList != null && pageNum * 100 > defectList.Count)
			{
				pageNumber.Value = 0;
				pageNum = 0;
			}
			UpdateDefectListView();
		}

		private void UpdateDefectListView()
		{
			if (defectListView.InvokeRequired)
			{
				this.Invoke(new MethodInvoker(delegate ()
				{
					UpdateDefectListView();
				}));
			}
			else
			{
				defectListView.BeginUpdate();
				defectListView.Items.Clear();
				int idx = pageNum * 100;
				int count = 100;
				var size = defectList.Count;
				if (idx + count > size)
					count = size - idx;

				var listData = defectList.GetRange(idx, count);

				foreach (var defect in listData)
				{
					ListViewItem lvi = new ListViewItem(defect.time);
					lvi.SubItems.Add(defect.frameNum.ToString());
					lvi.SubItems.Add(defect.PcNo.ToString());
					lvi.SubItems.Add(defect.defectClass.ToString());
					lvi.SubItems.Add("");
					lvi.SubItems.Add(defect.posX.ToString());
					lvi.SubItems.Add(defect.posY.ToString());
					lvi.SubItems.Add(defect.sizeX.ToString());
					lvi.SubItems.Add(defect.sizeY.ToString());
					lvi.SubItems.Add(defect.marking.ToString());
					lvi.SubItems.Add(defect.vMax.ToString());

					defectListView.Items.Add(lvi);
				}

				defectListView.EndUpdate();
				this.Refresh();
			}
		}

		public bool ReadCSVFile(string path)
        {
            string strConn = path;
            if (!File.Exists(path))
            {
                MessageBox.Show($"{path} is not exists");
                return false;
            }

			using (StreamReader readerObject = new StreamReader(path))
            {
				string line;
				while ((line = readerObject.ReadLine()) != null)
                {
					string[] datas = line.Split(',');

                }
            }

			return true;
        }

	}
}
