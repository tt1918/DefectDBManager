namespace MarkrCompare
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
            this.lblStatus = new CustomControls.RoundLabel();
            this.lblLotName = new CustomControls.RoundLabel();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.lblProcess = new CustomControls.RoundLabel();
            this.cbViewSelect = new System.Windows.Forms.CheckBox();
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
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 10F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 78F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 8F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(655, 128);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 4;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 126F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 8F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel2.Controls.Add(this.lblStatus, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.lblLotName, 2, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel2.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(655, 35);
            this.tableLayoutPanel2.TabIndex = 0;
            // 
            // lblStatus
            // 
            this.lblStatus.AutoSize = true;
            this.lblStatus.BkColor = System.Drawing.Color.MidnightBlue;
            this.lblStatus.BorderColor = System.Drawing.Color.LightSteelBlue;
            this.lblStatus.CornerR = 10;
            this.lblStatus.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblStatus.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblStatus.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.lblStatus.IsFillLB = false;
            this.lblStatus.IsFillLT = false;
            this.lblStatus.IsFillRB = false;
            this.lblStatus.IsFillRT = false;
            this.lblStatus.Location = new System.Drawing.Point(1, 1);
            this.lblStatus.Margin = new System.Windows.Forms.Padding(1);
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.Size = new System.Drawing.Size(124, 33);
            this.lblStatus.TabIndex = 2;
            this.lblStatus.Text = "Status";
            this.lblStatus.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.lblStatus.Thickness = 1;
            // 
            // lblLotName
            // 
            this.lblLotName.AutoSize = true;
            this.lblLotName.BkColor = System.Drawing.Color.MidnightBlue;
            this.lblLotName.BorderColor = System.Drawing.Color.LightSteelBlue;
            this.lblLotName.CornerR = 10;
            this.lblLotName.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblLotName.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblLotName.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.lblLotName.IsFillLB = false;
            this.lblLotName.IsFillLT = false;
            this.lblLotName.IsFillRB = false;
            this.lblLotName.IsFillRT = false;
            this.lblLotName.Location = new System.Drawing.Point(135, 1);
            this.lblLotName.Margin = new System.Windows.Forms.Padding(1);
            this.lblLotName.Name = "lblLotName";
            this.lblLotName.Size = new System.Drawing.Size(419, 33);
            this.lblLotName.TabIndex = 2;
            this.lblLotName.Text = "LOT";
            this.lblLotName.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.lblLotName.Thickness = 1;
            this.lblLotName.DoubleClick += new System.EventHandler(this.lblLotName_DoubleClick);
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.ColumnCount = 3;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 50F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel3.Controls.Add(this.lblProcess, 1, 0);
            this.tableLayoutPanel3.Controls.Add(this.cbViewSelect, 0, 0);
            this.tableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel3.Location = new System.Drawing.Point(0, 45);
            this.tableLayoutPanel3.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 1;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(655, 78);
            this.tableLayoutPanel3.TabIndex = 0;
            // 
            // lblProcess
            // 
            this.lblProcess.AutoSize = true;
            this.lblProcess.BkColor = System.Drawing.Color.MidnightBlue;
            this.lblProcess.BorderColor = System.Drawing.Color.LightSteelBlue;
            this.lblProcess.CornerR = 10;
            this.lblProcess.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblProcess.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblProcess.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.lblProcess.IsFillLB = false;
            this.lblProcess.IsFillLT = false;
            this.lblProcess.IsFillRB = false;
            this.lblProcess.IsFillRT = false;
            this.lblProcess.Location = new System.Drawing.Point(51, 1);
            this.lblProcess.Margin = new System.Windows.Forms.Padding(1);
            this.lblProcess.Name = "lblProcess";
            this.lblProcess.Size = new System.Drawing.Size(553, 76);
            this.lblProcess.TabIndex = 2;
            this.lblProcess.Text = "Case 1:";
            this.lblProcess.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.lblProcess.Thickness = 1;
            // 
            // cbViewSelect
            // 
            this.cbViewSelect.AutoSize = true;
            this.cbViewSelect.CheckAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.cbViewSelect.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cbViewSelect.Location = new System.Drawing.Point(3, 3);
            this.cbViewSelect.Name = "cbViewSelect";
            this.cbViewSelect.Size = new System.Drawing.Size(44, 72);
            this.cbViewSelect.TabIndex = 3;
            this.cbViewSelect.UseVisualStyleBackColor = true;
            this.cbViewSelect.Visible = false;
            this.cbViewSelect.CheckedChanged += new System.EventHandler(this.cbViewSelect_CheckedChanged);
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
            this.tableLayoutPanel2.PerformLayout();
            this.tableLayoutPanel3.ResumeLayout(false);
            this.tableLayoutPanel3.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private CustomControls.RoundLabel lblStatus;
        private CustomControls.RoundLabel lblLotName;
        private CustomControls.RoundLabel lblProcess;
        private System.Windows.Forms.CheckBox cbViewSelect;
    }
}