namespace MarkCompare
{
    partial class FormLotSummaryData
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
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.lblStatus = new CustomControls.ShadedLabel();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.cbViewSelect = new System.Windows.Forms.CheckBox();
            this.flpResult = new System.Windows.Forms.FlowLayoutPanel();
            this.lblProcess = new CustomControls.ShadedLabel();
            this.lblLotName = new CustomControls.GradiantLabel();
            this.tableLayoutPanel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.tableLayoutPanel3.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel3, 0, 2);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(1);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 4;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 5F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 8F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(655, 128);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 4;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 150F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 8F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 10F));
            this.tableLayoutPanel2.Controls.Add(this.lblStatus, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.lblLotName, 2, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel2.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(655, 35);
            this.tableLayoutPanel2.TabIndex = 0;
            // 
            // lblStatus
            // 
            this.lblStatus.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.lblStatus.BackColor = System.Drawing.Color.Transparent;
            this.lblStatus.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(18)))), ((int)(((byte)(27)))), ((int)(((byte)(38)))));
            this.lblStatus.BorderRadius = 3;
            this.lblStatus.BorderSize = 1;
            this.lblStatus.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblStatus.FillColor = System.Drawing.Color.MidnightBlue;
            this.lblStatus.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblStatus.ImagePadding = 6;
            this.lblStatus.ImageSize = 18;
            this.lblStatus.LabelImage = null;
            this.lblStatus.Location = new System.Drawing.Point(1, 1);
            this.lblStatus.Margin = new System.Windows.Forms.Padding(1);
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.ShowAccentLine = false;
            this.lblStatus.Size = new System.Drawing.Size(148, 33);
            this.lblStatus.TabIndex = 7;
            this.lblStatus.Text = "STATUS";
            this.lblStatus.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblStatus.TextColor = System.Drawing.Color.WhiteSmoke;
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.ColumnCount = 3;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel3.Controls.Add(this.cbViewSelect, 0, 0);
            this.tableLayoutPanel3.Controls.Add(this.flpResult, 1, 0);
            this.tableLayoutPanel3.Controls.Add(this.lblProcess, 2, 0);
            this.tableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel3.Location = new System.Drawing.Point(0, 40);
            this.tableLayoutPanel3.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 1;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(655, 80);
            this.tableLayoutPanel3.TabIndex = 0;
            // 
            // cbViewSelect
            // 
            this.cbViewSelect.AutoSize = true;
            this.cbViewSelect.CheckAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.cbViewSelect.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cbViewSelect.Location = new System.Drawing.Point(3, 3);
            this.cbViewSelect.Name = "cbViewSelect";
            this.cbViewSelect.Size = new System.Drawing.Size(44, 74);
            this.cbViewSelect.TabIndex = 3;
            this.cbViewSelect.UseVisualStyleBackColor = true;
            this.cbViewSelect.Visible = false;
            this.cbViewSelect.CheckedChanged += new System.EventHandler(this.cbViewSelect_CheckedChanged);
            // 
            // flpResult
            // 
            this.flpResult.AutoScroll = true;
            this.flpResult.BackColor = System.Drawing.Color.White;
            this.flpResult.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flpResult.Location = new System.Drawing.Point(51, 1);
            this.flpResult.Margin = new System.Windows.Forms.Padding(1);
            this.flpResult.Name = "flpResult";
            this.flpResult.Size = new System.Drawing.Size(553, 78);
            this.flpResult.TabIndex = 4;
            this.flpResult.WrapContents = false;
            // 
            // lblProcess
            // 
            this.lblProcess.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.lblProcess.BackColor = System.Drawing.Color.Transparent;
            this.lblProcess.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(18)))), ((int)(((byte)(27)))), ((int)(((byte)(38)))));
            this.lblProcess.BorderRadius = 3;
            this.lblProcess.BorderSize = 1;
            this.lblProcess.FillColor = System.Drawing.Color.MidnightBlue;
            this.lblProcess.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblProcess.ImagePadding = 6;
            this.lblProcess.ImageSize = 18;
            this.lblProcess.LabelImage = null;
            this.lblProcess.Location = new System.Drawing.Point(606, 1);
            this.lblProcess.Margin = new System.Windows.Forms.Padding(1);
            this.lblProcess.Name = "lblProcess";
            this.lblProcess.ShowAccentLine = false;
            this.lblProcess.Size = new System.Drawing.Size(48, 36);
            this.lblProcess.TabIndex = 5;
            this.lblProcess.Text = "CASE 1:";
            this.lblProcess.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.lblProcess.TextColor = System.Drawing.Color.WhiteSmoke;
            // 
            // lblLotName
            // 
            this.lblLotName.CornerRadius = 4;
            this.lblLotName.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblLotName.FillColor = System.Drawing.Color.DarkBlue;
            this.lblLotName.FillColor2 = System.Drawing.Color.MidnightBlue;
            this.lblLotName.Font = new System.Drawing.Font("Segoe UI", 30F, System.Drawing.FontStyle.Bold);
            this.lblLotName.Location = new System.Drawing.Point(161, 3);
            this.lblLotName.Name = "lblLotName";
            this.lblLotName.Padding = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblLotName.Size = new System.Drawing.Size(481, 29);
            this.lblLotName.TabIndex = 8;
            this.lblLotName.Text = "-";
            this.lblLotName.UseBestFitFont = true;
            this.lblLotName.UseGradient = true;
            // 
            // FormLotSummaryData
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(655, 128);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "FormLotSummaryData";
            this.Text = "FormLotSummary";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormLotSummaryData_FormClosing);
            this.Load += new System.EventHandler(this.FormLotSummaryData_Load);
            this.VisibleChanged += new System.EventHandler(this.FormLotSummaryData_VisibleChanged);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel3.ResumeLayout(false);
            this.tableLayoutPanel3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private System.Windows.Forms.CheckBox cbViewSelect;
        private System.Windows.Forms.FlowLayoutPanel flpResult;
        private CustomControls.ShadedLabel lblProcess;
        private CustomControls.ShadedLabel lblStatus;
        private CustomControls.GradiantLabel lblLotName;
    }
}