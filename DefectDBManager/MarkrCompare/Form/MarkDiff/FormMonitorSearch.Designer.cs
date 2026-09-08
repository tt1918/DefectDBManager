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
            this.lblProcess = new CustomControls.ShadedLabel();
            this.btnStart = new CustomControls.ShadedButton();
            this.btnStop = new CustomControls.ShadedButton();
            this.tableLayoutPanel5 = new System.Windows.Forms.TableLayoutPanel();
            this.lblTime = new CustomControls.ShadedLabel();
            this.btnMaterialFilter = new CustomControls.ShadedButton();
            this.bntCSV = new CustomControls.ShadedButton();
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
            this.tableLayoutPanel3.Controls.Add(this.lblProcess, 0, 0);
            this.tableLayoutPanel3.Controls.Add(this.btnStart, 2, 0);
            this.tableLayoutPanel3.Controls.Add(this.btnStop, 3, 0);
            this.tableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel3.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel3.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 1;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(545, 30);
            this.tableLayoutPanel3.TabIndex = 0;
            // 
            // lblProcess
            // 
            this.lblProcess.AccentColor = System.Drawing.Color.Black;
            this.lblProcess.BackColor = System.Drawing.Color.Transparent;
            this.lblProcess.BorderColor = System.Drawing.Color.Black;
            this.lblProcess.BorderRadius = 5;
            this.lblProcess.BorderSize = 0;
            this.lblProcess.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblProcess.FillColor = System.Drawing.Color.DimGray;
            this.lblProcess.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.lblProcess.ImagePadding = 6;
            this.lblProcess.ImageSize = 18;
            this.lblProcess.LabelImage = null;
            this.lblProcess.Location = new System.Drawing.Point(1, 1);
            this.lblProcess.Margin = new System.Windows.Forms.Padding(1);
            this.lblProcess.Name = "lblProcess";
            this.lblProcess.ShowAccentLine = false;
            this.lblProcess.Size = new System.Drawing.Size(352, 28);
            this.lblProcess.TabIndex = 2;
            this.lblProcess.Text = "Monitoring......";
            this.lblProcess.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblProcess.TextColor = System.Drawing.Color.White;
            // 
            // btnStart
            // 
            this.btnStart.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnStart.BackColor = System.Drawing.Color.Transparent;
            this.btnStart.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnStart.BorderRadius = 5;
            this.btnStart.BorderSize = 1;
            this.btnStart.ButtonImage = null;
            this.btnStart.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnStart.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnStart.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnStart.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnStart.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnStart.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnStart.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnStart.IconPadding = 6;
            this.btnStart.IconSize = 20;
            this.btnStart.Location = new System.Drawing.Point(366, 1);
            this.btnStart.Margin = new System.Windows.Forms.Padding(1);
            this.btnStart.Name = "btnStart";
            this.btnStart.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(80)))));
            this.btnStart.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnStart.ShowAccentLine = false;
            this.btnStart.Size = new System.Drawing.Size(88, 28);
            this.btnStart.TabIndex = 3;
            this.btnStart.Text = "START";
            this.btnStart.TextColor = System.Drawing.Color.White;
            this.btnStart.TextOffsetX = 0;
            this.btnStart.TextOffsetY = 0;
            this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
            // 
            // btnStop
            // 
            this.btnStop.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnStop.BackColor = System.Drawing.Color.Transparent;
            this.btnStop.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnStop.BorderRadius = 5;
            this.btnStop.BorderSize = 1;
            this.btnStop.ButtonImage = null;
            this.btnStop.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnStop.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnStop.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnStop.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnStop.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnStop.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnStop.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnStop.IconPadding = 6;
            this.btnStop.IconSize = 20;
            this.btnStop.Location = new System.Drawing.Point(456, 1);
            this.btnStop.Margin = new System.Windows.Forms.Padding(1);
            this.btnStop.Name = "btnStop";
            this.btnStop.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(80)))));
            this.btnStop.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnStop.ShowAccentLine = false;
            this.btnStop.Size = new System.Drawing.Size(88, 28);
            this.btnStop.TabIndex = 4;
            this.btnStop.Text = "STOP";
            this.btnStop.TextColor = System.Drawing.Color.White;
            this.btnStop.TextOffsetX = 0;
            this.btnStop.TextOffsetY = 0;
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
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
            this.tableLayoutPanel5.Controls.Add(this.lblTime, 0, 0);
            this.tableLayoutPanel5.Controls.Add(this.btnMaterialFilter, 5, 0);
            this.tableLayoutPanel5.Controls.Add(this.bntCSV, 4, 0);
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
            // lblTime
            // 
            this.lblTime.AccentColor = System.Drawing.Color.Black;
            this.lblTime.BackColor = System.Drawing.Color.Transparent;
            this.lblTime.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(150)))));
            this.lblTime.BorderRadius = 5;
            this.lblTime.BorderSize = 0;
            this.lblTime.FillColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(90)))));
            this.lblTime.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblTime.ImagePadding = 6;
            this.lblTime.ImageSize = 18;
            this.lblTime.LabelImage = null;
            this.lblTime.Location = new System.Drawing.Point(1, 1);
            this.lblTime.Margin = new System.Windows.Forms.Padding(1);
            this.lblTime.Name = "lblTime";
            this.lblTime.ShowAccentLine = false;
            this.lblTime.Size = new System.Drawing.Size(63, 28);
            this.lblTime.TabIndex = 5;
            this.lblTime.Text = "시간";
            this.lblTime.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblTime.TextColor = System.Drawing.Color.White;
            // 
            // btnMaterialFilter
            // 
            this.btnMaterialFilter.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnMaterialFilter.BackColor = System.Drawing.Color.Transparent;
            this.btnMaterialFilter.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnMaterialFilter.BorderRadius = 5;
            this.btnMaterialFilter.BorderSize = 1;
            this.btnMaterialFilter.ButtonImage = null;
            this.btnMaterialFilter.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnMaterialFilter.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnMaterialFilter.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnMaterialFilter.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnMaterialFilter.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnMaterialFilter.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnMaterialFilter.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnMaterialFilter.IconPadding = 6;
            this.btnMaterialFilter.IconSize = 20;
            this.btnMaterialFilter.Location = new System.Drawing.Point(454, 1);
            this.btnMaterialFilter.Margin = new System.Windows.Forms.Padding(1);
            this.btnMaterialFilter.Name = "btnMaterialFilter";
            this.btnMaterialFilter.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(80)))));
            this.btnMaterialFilter.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnMaterialFilter.ShowAccentLine = false;
            this.btnMaterialFilter.Size = new System.Drawing.Size(90, 28);
            this.btnMaterialFilter.TabIndex = 6;
            this.btnMaterialFilter.Text = "FILTER";
            this.btnMaterialFilter.TextColor = System.Drawing.Color.White;
            this.btnMaterialFilter.TextOffsetX = 0;
            this.btnMaterialFilter.TextOffsetY = 0;
            this.btnMaterialFilter.Click += new System.EventHandler(this.btnMaterialFilter_Click);
            // 
            // bntCSV
            // 
            this.bntCSV.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.bntCSV.BackColor = System.Drawing.Color.Transparent;
            this.bntCSV.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.bntCSV.BorderRadius = 5;
            this.bntCSV.BorderSize = 1;
            this.bntCSV.ButtonImage = null;
            this.bntCSV.Cursor = System.Windows.Forms.Cursors.Hand;
            this.bntCSV.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.bntCSV.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.bntCSV.Dock = System.Windows.Forms.DockStyle.Fill;
            this.bntCSV.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bntCSV.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.bntCSV.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.bntCSV.IconPadding = 6;
            this.bntCSV.IconSize = 20;
            this.bntCSV.Location = new System.Drawing.Point(364, 1);
            this.bntCSV.Margin = new System.Windows.Forms.Padding(1);
            this.bntCSV.Name = "bntCSV";
            this.bntCSV.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(80)))));
            this.bntCSV.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.bntCSV.ShowAccentLine = false;
            this.bntCSV.Size = new System.Drawing.Size(88, 28);
            this.bntCSV.TabIndex = 5;
            this.bntCSV.Text = "CSV";
            this.bntCSV.TextColor = System.Drawing.Color.White;
            this.bntCSV.TextOffsetX = 0;
            this.bntCSV.TextOffsetY = 0;
            this.bntCSV.Click += new System.EventHandler(this.btnCsv_Click);
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
            this.tableLayoutPanel5.ResumeLayout(false);
            this.tableLayoutPanel5.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel5;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.DateTimePicker timePickerStart;
        private System.Windows.Forms.DateTimePicker timePickerEnd;
        private System.Windows.Forms.ListView lvFilterList;
        private CustomControls.ShadedLabel lblProcess;
        private CustomControls.ShadedButton btnStart;
        private CustomControls.ShadedButton btnStop;
        private CustomControls.ShadedButton bntCSV;
        private CustomControls.ShadedButton btnMaterialFilter;
        private CustomControls.ShadedLabel lblTime;
    }
}