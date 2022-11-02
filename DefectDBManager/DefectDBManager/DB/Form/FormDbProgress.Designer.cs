namespace DefectDBManager
{
    partial class FormDbProgress
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
            this.panelTitle = new System.Windows.Forms.Panel();
            this.lblTitle = new System.Windows.Forms.Label();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.listBoxProcess = new System.Windows.Forms.ListBox();
            this.flpProgress = new System.Windows.Forms.FlowLayoutPanel();
            this.panelTitle.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // panelTitle
            // 
            this.panelTitle.BackColor = System.Drawing.Color.DarkGray;
            this.panelTitle.Controls.Add(this.lblTitle);
            this.panelTitle.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelTitle.Location = new System.Drawing.Point(3, 3);
            this.panelTitle.Name = "panelTitle";
            this.panelTitle.Size = new System.Drawing.Size(478, 22);
            this.panelTitle.TabIndex = 1;
            // 
            // lblTitle
            // 
            this.lblTitle.AutoSize = true;
            this.lblTitle.Font = new System.Drawing.Font("굴림", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(129)));
            this.lblTitle.ForeColor = System.Drawing.Color.White;
            this.lblTitle.Location = new System.Drawing.Point(5, 5);
            this.lblTitle.Name = "lblTitle";
            this.lblTitle.Size = new System.Drawing.Size(173, 13);
            this.lblTitle.TabIndex = 0;
            this.lblTitle.Text = "DATA BASE PROGRESS";
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Controls.Add(this.panelTitle, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 6.540084F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 93.45992F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(484, 434);
            this.tableLayoutPanel1.TabIndex = 2;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 2;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 31.54206F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 68.45795F));
            this.tableLayoutPanel2.Controls.Add(this.listBoxProcess, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.flpProgress, 1, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(3, 31);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(478, 400);
            this.tableLayoutPanel2.TabIndex = 2;
            // 
            // listBoxProcess
            // 
            this.listBoxProcess.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listBoxProcess.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.listBoxProcess.FormattingEnabled = true;
            this.listBoxProcess.ItemHeight = 18;
            this.listBoxProcess.Location = new System.Drawing.Point(3, 3);
            this.listBoxProcess.Name = "listBoxProcess";
            this.listBoxProcess.Size = new System.Drawing.Size(144, 394);
            this.listBoxProcess.TabIndex = 0;
            this.listBoxProcess.DrawItem += new System.Windows.Forms.DrawItemEventHandler(this.listBoxProcess_DrawItem);
            this.listBoxProcess.MeasureItem += new System.Windows.Forms.MeasureItemEventHandler(this.listBoxProcess_MeasureItem);
            this.listBoxProcess.VisibleChanged += new System.EventHandler(this.listBoxProcess_VisibleChanged);
            // 
            // flpProgress
            // 
            this.flpProgress.BackColor = System.Drawing.Color.White;
            this.flpProgress.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flpProgress.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
            this.flpProgress.Location = new System.Drawing.Point(153, 3);
            this.flpProgress.Name = "flpProgress";
            this.flpProgress.Size = new System.Drawing.Size(322, 394);
            this.flpProgress.TabIndex = 1;
            // 
            // FormDbProgress
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(484, 434);
            this.Controls.Add(this.tableLayoutPanel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "FormDbProgress";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "FormDbProgress";
            this.panelTitle.ResumeLayout(false);
            this.panelTitle.PerformLayout();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panelTitle;
        private System.Windows.Forms.Label lblTitle;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.ListBox listBoxProcess;
        private System.Windows.Forms.FlowLayoutPanel flpProgress;
    }
}