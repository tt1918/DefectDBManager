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
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.lvLotList = new System.Windows.Forms.ListView();
            this.lvFilterList = new System.Windows.Forms.ListView();
            this.lblProcess = new CustomControls.ShadedLabel();
            this.btnStart = new CustomControls.ShadedButton();
            this.btnStop = new CustomControls.ShadedButton();
            this.btnSetLot = new CustomControls.ShadedButton();
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
            this.tableLayoutPanel3.Controls.Add(this.lblProcess, 0, 0);
            this.tableLayoutPanel3.Controls.Add(this.btnStart, 2, 0);
            this.tableLayoutPanel3.Controls.Add(this.btnStop, 3, 0);
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
            this.lblProcess.Size = new System.Drawing.Size(262, 28);
            this.lblProcess.TabIndex = 7;
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
            this.btnStart.Location = new System.Drawing.Point(276, 1);
            this.btnStart.Margin = new System.Windows.Forms.Padding(1);
            this.btnStart.Name = "btnStart";
            this.btnStart.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(80)))));
            this.btnStart.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnStart.ShowAccentLine = false;
            this.btnStart.Size = new System.Drawing.Size(88, 28);
            this.btnStart.TabIndex = 8;
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
            this.btnStop.Location = new System.Drawing.Point(366, 1);
            this.btnStop.Margin = new System.Windows.Forms.Padding(1);
            this.btnStop.Name = "btnStop";
            this.btnStop.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(80)))));
            this.btnStop.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnStop.ShowAccentLine = false;
            this.btnStop.Size = new System.Drawing.Size(88, 28);
            this.btnStop.TabIndex = 9;
            this.btnStop.Text = "STOP";
            this.btnStop.TextColor = System.Drawing.Color.White;
            this.btnStop.TextOffsetX = 0;
            this.btnStop.TextOffsetY = 0;
            this.btnStop.Click += new System.EventHandler(this.btnStop_Click);
            // 
            // btnSetLot
            // 
            this.btnSetLot.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnSetLot.BackColor = System.Drawing.Color.Transparent;
            this.btnSetLot.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnSetLot.BorderRadius = 5;
            this.btnSetLot.BorderSize = 1;
            this.btnSetLot.ButtonImage = null;
            this.btnSetLot.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnSetLot.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnSetLot.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnSetLot.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnSetLot.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSetLot.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnSetLot.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnSetLot.IconPadding = 6;
            this.btnSetLot.IconSize = 20;
            this.btnSetLot.Location = new System.Drawing.Point(456, 1);
            this.btnSetLot.Margin = new System.Windows.Forms.Padding(1);
            this.btnSetLot.Name = "btnSetLot";
            this.btnSetLot.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(80)))));
            this.btnSetLot.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnSetLot.ShowAccentLine = false;
            this.btnSetLot.Size = new System.Drawing.Size(88, 28);
            this.btnSetLot.TabIndex = 10;
            this.btnSetLot.Text = "SETTING";
            this.btnSetLot.TextColor = System.Drawing.Color.White;
            this.btnSetLot.TextOffsetX = 0;
            this.btnSetLot.TextOffsetY = 0;
            this.btnSetLot.Click += new System.EventHandler(this.btnSetLot_Click);
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
            this.tableLayoutPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private System.Windows.Forms.ListView lvFilterList;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.ListView lvLotList;
        private CustomControls.ShadedLabel lblProcess;
        private CustomControls.ShadedButton btnStart;
        private CustomControls.ShadedButton btnStop;
        private CustomControls.ShadedButton btnSetLot;
    }
}