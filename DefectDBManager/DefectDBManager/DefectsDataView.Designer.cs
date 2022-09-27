namespace DefectDBManager
{
	partial class DefectsDataView
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.panel1 = new System.Windows.Forms.Panel();
			this.defectListView = new MetroFramework.Controls.MetroListView();
			this.btnLoadDB = new MetroFramework.Controls.MetroButton();
			this.pageNumber = new System.Windows.Forms.NumericUpDown();
			this.panel1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.pageNumber)).BeginInit();
			this.SuspendLayout();
			// 
			// panel1
			// 
			this.panel1.Controls.Add(this.defectListView);
			this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
			this.panel1.Location = new System.Drawing.Point(20, 60);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(1162, 488);
			this.panel1.TabIndex = 0;
			// 
			// defectListView
			// 
			this.defectListView.Dock = System.Windows.Forms.DockStyle.Fill;
			this.defectListView.Font = new System.Drawing.Font("Segoe UI", 12F);
			this.defectListView.FullRowSelect = true;
			this.defectListView.Location = new System.Drawing.Point(0, 0);
			this.defectListView.Name = "defectListView";
			this.defectListView.OwnerDraw = true;
			this.defectListView.Size = new System.Drawing.Size(1162, 488);
			this.defectListView.TabIndex = 0;
			this.defectListView.UseCompatibleStateImageBehavior = false;
			this.defectListView.UseSelectable = true;
			// 
			// btnLoadDB
			// 
			this.btnLoadDB.Location = new System.Drawing.Point(1107, 31);
			this.btnLoadDB.Name = "btnLoadDB";
			this.btnLoadDB.Size = new System.Drawing.Size(75, 23);
			this.btnLoadDB.TabIndex = 1;
			this.btnLoadDB.Text = "Load DB";
			this.btnLoadDB.UseSelectable = true;
			this.btnLoadDB.Click += new System.EventHandler(this.btnLoadDB_Click);
			// 
			// pageNumber
			// 
			this.pageNumber.Location = new System.Drawing.Point(1062, 554);
			this.pageNumber.Name = "pageNumber";
			this.pageNumber.Size = new System.Drawing.Size(120, 21);
			this.pageNumber.TabIndex = 2;
			this.pageNumber.ValueChanged += new System.EventHandler(this.pageNumber_ValueChanged);
			// 
			// DefectsDataView
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(1202, 671);
			this.Controls.Add(this.pageNumber);
			this.Controls.Add(this.btnLoadDB);
			this.Controls.Add(this.panel1);
			this.Name = "DefectsDataView";
			this.Text = "DefectsDataView";
			this.Load += new System.EventHandler(this.DefectsDataView_Load);
			this.panel1.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.pageNumber)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Panel panel1;
		private MetroFramework.Controls.MetroListView defectListView;
		private MetroFramework.Controls.MetroButton btnLoadDB;
		private System.Windows.Forms.NumericUpDown pageNumber;
	}
}