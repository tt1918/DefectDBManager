namespace MarkCompare
{
    partial class FormMonitorSelectedLot
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
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.btnStart = new CustomControls.RoundButton();
            this.btnStop = new CustomControls.RoundButton();
            this.lblProcess = new CustomControls.RoundLabel();
            this.btnSetLot = new CustomControls.RoundButton();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.lvLotList = new System.Windows.Forms.ListView();
            this.lvFilterList = new System.Windows.Forms.ListView();
            this.tableLayoutPanel2.SuspendLayout();
            this.tableLayoutPanel3.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 1;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Controls.Add(this.tableLayoutPanel3, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.tableLayoutPanel1, 0, 2);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel2.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 3;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 5F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(545, 139);
            this.tableLayoutPanel2.TabIndex = 2;
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.ColumnCount = 5;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 11F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 90F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 90F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 90F));
            this.tableLayoutPanel3.Controls.Add(this.btnStart, 2, 0);
            this.tableLayoutPanel3.Controls.Add(this.btnStop, 3, 0);
            this.tableLayoutPanel3.Controls.Add(this.lblProcess, 0, 0);
            this.tableLayoutPanel3.Controls.Add(this.btnSetLot, 4, 0);
            this.tableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel3.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel3.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 1;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(545, 30);
            this.tableLayoutPanel3.TabIndex = 0;
            // 
            // btnStart
            // 
            this.btnStart.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(80)))));
            this.btnStart.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(80)))));
            this.btnStart.BorderColor = System.Drawing.Color.PaleVioletRed;
            this.btnStart.BorderRadius = 5;
            this.btnStart.BorderSize = 0;
            this.btnStart.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnStart.FlatAppearance.BorderSize = 0;
            this.btnStart.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnStart.ForeColor = System.Drawing.Color.White;
            this.btnStart.Location = new System.Drawing.Point(276, 1);
            this.btnStart.Margin = new System.Windows.Forms.Padding(1, 1, 2, 1);
            this.btnStart.Name = "btnStart";
            this.btnStart.Size = new System.Drawing.Size(87, 28);
            this.btnStart.TabIndex = 0;
            this.btnStart.Text = "START";
            this.btnStart.TextColor = System.Drawing.Color.White;
            this.btnStart.UseVisualStyleBackColor = false;
            this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
            // 
            // btnStop
            // 
            this.btnStop.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(80)))));
            this.btnStop.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(80)))));
            this.btnStop.BorderColor = System.Drawing.Color.PaleVioletRed;
            this.btnStop.BorderRadius = 5;
            this.btnStop.BorderSize = 0;
            this.btnStop.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnStop.FlatAppearance.BorderSize = 0;
            this.btnStop.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnStop.ForeColor = System.Drawing.Color.White;
            this.btnStop.Location = new System.Drawing.Point(367, 1);
            this.btnStop.Margin = new System.Windows.Forms.Padding(2, 1, 1, 1);
            this.btnStop.Name = "btnStop";
            this.btnStop.Size = new System.Drawing.Size(87, 28);
            this.btnStop.TabIndex = 0;
            this.btnStop.Text = "STOP";
            this.btnStop.TextColor = System.Drawing.Color.White;
            this.btnStop.UseVisualStyleBackColor = false;
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            // 
            // lblProcess
            // 
            this.lblProcess.AutoSize = true;
            this.lblProcess.BkColor = System.Drawing.Color.LightGray;
            this.lblProcess.BorderColor = System.Drawing.Color.DarkGray;
            this.lblProcess.CornerR = 10;
            this.lblProcess.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblProcess.IsFillLB = false;
            this.lblProcess.IsFillLT = false;
            this.lblProcess.IsFillRB = false;
            this.lblProcess.IsFillRT = false;
            this.lblProcess.Location = new System.Drawing.Point(2, 2);
            this.lblProcess.Margin = new System.Windows.Forms.Padding(2);
            this.lblProcess.Name = "lblProcess";
            this.lblProcess.Size = new System.Drawing.Size(260, 26);
            this.lblProcess.TabIndex = 1;
            this.lblProcess.Text = "Monitoring......";
            this.lblProcess.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblProcess.Thickness = 1;
            // 
            // btnSetLot
            // 
            this.btnSetLot.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(80)))));
            this.btnSetLot.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(80)))));
            this.btnSetLot.BorderColor = System.Drawing.Color.PaleVioletRed;
            this.btnSetLot.BorderRadius = 5;
            this.btnSetLot.BorderSize = 0;
            this.btnSetLot.FlatAppearance.BorderSize = 0;
            this.btnSetLot.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnSetLot.ForeColor = System.Drawing.Color.White;
            this.btnSetLot.Location = new System.Drawing.Point(456, 1);
            this.btnSetLot.Margin = new System.Windows.Forms.Padding(1);
            this.btnSetLot.Name = "btnSetLot";
            this.btnSetLot.Size = new System.Drawing.Size(88, 28);
            this.btnSetLot.TabIndex = 6;
            this.btnSetLot.Text = "SETTING";
            this.btnSetLot.TextColor = System.Drawing.Color.White;
            this.btnSetLot.UseVisualStyleBackColor = false;
            this.btnSetLot.Click += new System.EventHandler(this.btnSetLot_Click);
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Controls.Add(this.lvLotList, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.lvFilterList, 0, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(1, 36);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(1);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 1;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(543, 102);
            this.tableLayoutPanel1.TabIndex = 5;
            // 
            // lvLotList
            // 
            this.lvLotList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lvLotList.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.lvLotList.HideSelection = false;
            this.lvLotList.Location = new System.Drawing.Point(274, 3);
            this.lvLotList.Name = "lvLotList";
            this.lvLotList.Size = new System.Drawing.Size(266, 96);
            this.lvLotList.TabIndex = 5;
            this.lvLotList.UseCompatibleStateImageBehavior = false;
            this.lvLotList.View = System.Windows.Forms.View.List;
            // 
            // lvFilterList
            // 
            this.lvFilterList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lvFilterList.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.lvFilterList.HideSelection = false;
            this.lvFilterList.Location = new System.Drawing.Point(3, 3);
            this.lvFilterList.Name = "lvFilterList";
            this.lvFilterList.Size = new System.Drawing.Size(265, 96);
            this.lvFilterList.TabIndex = 4;
            this.lvFilterList.UseCompatibleStateImageBehavior = false;
            this.lvFilterList.View = System.Windows.Forms.View.List;
            // 
            // FormMonitorSelectedLot
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.WhiteSmoke;
            this.ClientSize = new System.Drawing.Size(545, 139);
            this.Controls.Add(this.tableLayoutPanel2);
            this.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "FormMonitorSelectedLot";
            this.Text = "FormMornitorLive";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormMornitorSearch_FormClosing);
            this.Load += new System.EventHandler(this.FormMornitorSearch_Load);
            this.VisibleChanged += new System.EventHandler(this.FormMornitorSearch_VisibleChanged);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel3.ResumeLayout(false);
            this.tableLayoutPanel3.PerformLayout();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private CustomControls.RoundButton btnStart;
        private CustomControls.RoundButton btnStop;
        private CustomControls.RoundLabel lblProcess;
        private CustomControls.RoundButton btnSetLot;
        private System.Windows.Forms.ListView lvFilterList;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.ListView lvLotList;
    }
}