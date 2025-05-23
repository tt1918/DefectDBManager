namespace MarkCompare
{
    partial class FormMonitorSearch
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
            this.lvFilterList = new System.Windows.Forms.ListView();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.btnStart = new CustomControls.RoundButton();
            this.btnStop = new CustomControls.RoundButton();
            this.lblProcess = new CustomControls.RoundLabel();
            this.tableLayoutPanel5 = new System.Windows.Forms.TableLayoutPanel();
            this.btnCsv = new CustomControls.RoundButton();
            this.lblTime = new CustomControls.RoundLabel();
            this.btnMaterialFilter = new CustomControls.RoundButton();
            this.label1 = new System.Windows.Forms.Label();
            this.timePickerStart = new System.Windows.Forms.DateTimePicker();
            this.timePickerEnd = new System.Windows.Forms.DateTimePicker();
            this.tableLayoutPanel2.SuspendLayout();
            this.tableLayoutPanel3.SuspendLayout();
            this.tableLayoutPanel5.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 1;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Controls.Add(this.lvFilterList, 0, 3);
            this.tableLayoutPanel2.Controls.Add(this.tableLayoutPanel3, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.tableLayoutPanel5, 0, 2);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel2.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 4;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 5F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(545, 139);
            this.tableLayoutPanel2.TabIndex = 2;
            // 
            // lvFilterList
            // 
            this.lvFilterList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lvFilterList.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.lvFilterList.HideSelection = false;
            this.lvFilterList.Location = new System.Drawing.Point(3, 68);
            this.lvFilterList.Name = "lvFilterList";
            this.lvFilterList.Size = new System.Drawing.Size(539, 68);
            this.lvFilterList.TabIndex = 4;
            this.lvFilterList.UseCompatibleStateImageBehavior = false;
            this.lvFilterList.View = System.Windows.Forms.View.List;
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.ColumnCount = 4;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 11F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 90F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 90F));
            this.tableLayoutPanel3.Controls.Add(this.btnStart, 2, 0);
            this.tableLayoutPanel3.Controls.Add(this.btnStop, 3, 0);
            this.tableLayoutPanel3.Controls.Add(this.lblProcess, 0, 0);
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
            this.btnStart.Location = new System.Drawing.Point(366, 1);
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
            this.btnStop.Location = new System.Drawing.Point(457, 1);
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
            this.lblProcess.Size = new System.Drawing.Size(350, 26);
            this.lblProcess.TabIndex = 1;
            this.lblProcess.Text = "Monitoring......";
            this.lblProcess.Thickness = 1;
            // 
            // tableLayoutPanel5
            // 
            this.tableLayoutPanel5.ColumnCount = 6;
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 65F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 90F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 92F));
            this.tableLayoutPanel5.Controls.Add(this.btnCsv, 4, 0);
            this.tableLayoutPanel5.Controls.Add(this.lblTime, 0, 0);
            this.tableLayoutPanel5.Controls.Add(this.btnMaterialFilter, 5, 0);
            this.tableLayoutPanel5.Controls.Add(this.label1, 2, 0);
            this.tableLayoutPanel5.Controls.Add(this.timePickerStart, 1, 0);
            this.tableLayoutPanel5.Controls.Add(this.timePickerEnd, 3, 0);
            this.tableLayoutPanel5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel5.Location = new System.Drawing.Point(0, 35);
            this.tableLayoutPanel5.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel5.Name = "tableLayoutPanel5";
            this.tableLayoutPanel5.RowCount = 1;
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel5.Size = new System.Drawing.Size(545, 30);
            this.tableLayoutPanel5.TabIndex = 2;
            // 
            // btnCsv
            // 
            this.btnCsv.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(80)))));
            this.btnCsv.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(80)))));
            this.btnCsv.BorderColor = System.Drawing.Color.PaleVioletRed;
            this.btnCsv.BorderRadius = 5;
            this.btnCsv.BorderSize = 0;
            this.btnCsv.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnCsv.FlatAppearance.BorderSize = 0;
            this.btnCsv.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnCsv.ForeColor = System.Drawing.Color.White;
            this.btnCsv.Location = new System.Drawing.Point(364, 1);
            this.btnCsv.Margin = new System.Windows.Forms.Padding(1);
            this.btnCsv.Name = "btnCsv";
            this.btnCsv.Size = new System.Drawing.Size(88, 28);
            this.btnCsv.TabIndex = 5;
            this.btnCsv.Text = "CSV";
            this.btnCsv.TextColor = System.Drawing.Color.White;
            this.btnCsv.UseVisualStyleBackColor = false;
            this.btnCsv.Click += new System.EventHandler(this.btnCsv_Click);
            // 
            // lblTime
            // 
            this.lblTime.AutoSize = true;
            this.lblTime.BkColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(90)))));
            this.lblTime.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(150)))));
            this.lblTime.CornerR = 10;
            this.lblTime.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblTime.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblTime.ForeColor = System.Drawing.Color.White;
            this.lblTime.IsFillLB = false;
            this.lblTime.IsFillLT = false;
            this.lblTime.IsFillRB = false;
            this.lblTime.IsFillRT = false;
            this.lblTime.Location = new System.Drawing.Point(1, 1);
            this.lblTime.Margin = new System.Windows.Forms.Padding(1);
            this.lblTime.Name = "lblTime";
            this.lblTime.Size = new System.Drawing.Size(63, 28);
            this.lblTime.TabIndex = 4;
            this.lblTime.Text = "TIME";
            this.lblTime.Thickness = 1;
            // 
            // btnMaterialFilter
            // 
            this.btnMaterialFilter.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(80)))));
            this.btnMaterialFilter.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(80)))));
            this.btnMaterialFilter.BorderColor = System.Drawing.Color.PaleVioletRed;
            this.btnMaterialFilter.BorderRadius = 5;
            this.btnMaterialFilter.BorderSize = 0;
            this.btnMaterialFilter.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnMaterialFilter.FlatAppearance.BorderSize = 0;
            this.btnMaterialFilter.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnMaterialFilter.ForeColor = System.Drawing.Color.White;
            this.btnMaterialFilter.Location = new System.Drawing.Point(454, 1);
            this.btnMaterialFilter.Margin = new System.Windows.Forms.Padding(1);
            this.btnMaterialFilter.Name = "btnMaterialFilter";
            this.btnMaterialFilter.Size = new System.Drawing.Size(90, 28);
            this.btnMaterialFilter.TabIndex = 3;
            this.btnMaterialFilter.Text = "FILTER";
            this.btnMaterialFilter.TextColor = System.Drawing.Color.White;
            this.btnMaterialFilter.UseVisualStyleBackColor = false;
            this.btnMaterialFilter.Click += new System.EventHandler(this.btnMaterialFilter_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label1.Location = new System.Drawing.Point(204, 0);
            this.label1.Margin = new System.Windows.Forms.Padding(0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(20, 30);
            this.label1.TabIndex = 1;
            this.label1.Text = "~";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // timePickerStart
            // 
            this.timePickerStart.Dock = System.Windows.Forms.DockStyle.Fill;
            this.timePickerStart.Location = new System.Drawing.Point(68, 3);
            this.timePickerStart.Name = "timePickerStart";
            this.timePickerStart.Size = new System.Drawing.Size(133, 23);
            this.timePickerStart.TabIndex = 2;
            // 
            // timePickerEnd
            // 
            this.timePickerEnd.Dock = System.Windows.Forms.DockStyle.Fill;
            this.timePickerEnd.Location = new System.Drawing.Point(227, 3);
            this.timePickerEnd.Name = "timePickerEnd";
            this.timePickerEnd.Size = new System.Drawing.Size(133, 23);
            this.timePickerEnd.TabIndex = 2;
            // 
            // FormMonitorSearch
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.WhiteSmoke;
            this.ClientSize = new System.Drawing.Size(545, 139);
            this.Controls.Add(this.tableLayoutPanel2);
            this.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "FormMonitorSearch";
            this.Text = "FormMornitorLive";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormMornitorSearch_FormClosing);
            this.Load += new System.EventHandler(this.FormMornitorSearch_Load);
            this.VisibleChanged += new System.EventHandler(this.FormMornitorSearch_VisibleChanged);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel3.ResumeLayout(false);
            this.tableLayoutPanel3.PerformLayout();
            this.tableLayoutPanel5.ResumeLayout(false);
            this.tableLayoutPanel5.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private CustomControls.RoundButton btnStart;
        private CustomControls.RoundButton btnStop;
        private CustomControls.RoundLabel lblProcess;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel5;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.DateTimePicker timePickerStart;
        private System.Windows.Forms.DateTimePicker timePickerEnd;
        private CustomControls.RoundButton btnMaterialFilter;
        private CustomControls.RoundLabel lblTime;
        private System.Windows.Forms.ListView lvFilterList;
        private CustomControls.RoundButton btnCsv;
    }
}