namespace MarkCompare
{
    partial class FormLotSummery
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
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.rbShowNormal = new System.Windows.Forms.RadioButton();
            this.rbShowNG = new System.Windows.Forms.RadioButton();
            this.rbShowAll = new System.Windows.Forms.RadioButton();
            this.btnClearHistory = new CustomControls.ShadedButton();
            this.panel1 = new System.Windows.Forms.Panel();
            this.flpLotSummary = new System.Windows.Forms.FlowLayoutPanel();
            this.dgvAiMonitor = new System.Windows.Forms.DataGridView();
            this.lblSJMonitor = new CustomControls.ShadedLabel();
            this.tableLayoutPanel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvAiMonitor)).BeginInit();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 2;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 40F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 60F));
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.panel1, 1, 1);
            this.tableLayoutPanel1.Controls.Add(this.dgvAiMonitor, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.lblSJMonitor, 0, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(1);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(851, 562);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 5;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Controls.Add(this.rbShowNormal, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.rbShowNG, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.rbShowAll, 2, 0);
            this.tableLayoutPanel2.Controls.Add(this.btnClearHistory, 3, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(341, 1);
            this.tableLayoutPanel2.Margin = new System.Windows.Forms.Padding(1);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(509, 33);
            this.tableLayoutPanel2.TabIndex = 0;
            // 
            // rbShowNormal
            // 
            this.rbShowNormal.AutoSize = true;
            this.rbShowNormal.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rbShowNormal.Location = new System.Drawing.Point(3, 3);
            this.rbShowNormal.Name = "rbShowNormal";
            this.rbShowNormal.Size = new System.Drawing.Size(94, 27);
            this.rbShowNormal.TabIndex = 0;
            this.rbShowNormal.TabStop = true;
            this.rbShowNormal.Text = "Normal";
            this.rbShowNormal.UseVisualStyleBackColor = true;
            this.rbShowNormal.Click += new System.EventHandler(this.rbShow_Click);
            // 
            // rbShowNG
            // 
            this.rbShowNG.AutoSize = true;
            this.rbShowNG.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rbShowNG.Location = new System.Drawing.Point(103, 3);
            this.rbShowNG.Name = "rbShowNG";
            this.rbShowNG.Size = new System.Drawing.Size(94, 27);
            this.rbShowNG.TabIndex = 0;
            this.rbShowNG.TabStop = true;
            this.rbShowNG.Text = "NG";
            this.rbShowNG.UseVisualStyleBackColor = true;
            this.rbShowNG.Click += new System.EventHandler(this.rbShow_Click);
            // 
            // rbShowAll
            // 
            this.rbShowAll.AutoSize = true;
            this.rbShowAll.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rbShowAll.Location = new System.Drawing.Point(203, 3);
            this.rbShowAll.Name = "rbShowAll";
            this.rbShowAll.Size = new System.Drawing.Size(94, 27);
            this.rbShowAll.TabIndex = 0;
            this.rbShowAll.TabStop = true;
            this.rbShowAll.Text = "ALL";
            this.rbShowAll.UseVisualStyleBackColor = true;
            this.rbShowAll.Click += new System.EventHandler(this.rbShow_Click);
            // 
            // btnClearHistory
            // 
            this.btnClearHistory.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnClearHistory.BackColor = System.Drawing.Color.Transparent;
            this.btnClearHistory.BorderColor = System.Drawing.Color.SlateGray;
            this.btnClearHistory.BorderRadius = 5;
            this.btnClearHistory.BorderSize = 2;
            this.btnClearHistory.ButtonImage = null;
            this.btnClearHistory.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnClearHistory.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnClearHistory.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnClearHistory.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnClearHistory.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnClearHistory.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnClearHistory.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnClearHistory.IconPadding = 6;
            this.btnClearHistory.IconSize = 20;
            this.btnClearHistory.Location = new System.Drawing.Point(301, 1);
            this.btnClearHistory.Margin = new System.Windows.Forms.Padding(1);
            this.btnClearHistory.Name = "btnClearHistory";
            this.btnClearHistory.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnClearHistory.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnClearHistory.ShowAccentLine = false;
            this.btnClearHistory.Size = new System.Drawing.Size(98, 31);
            this.btnClearHistory.TabIndex = 12;
            this.btnClearHistory.Text = "이력 삭제";
            this.btnClearHistory.TextColor = System.Drawing.Color.White;
            this.btnClearHistory.TextOffsetX = 0;
            this.btnClearHistory.TextOffsetY = 0;
            this.btnClearHistory.Click += new System.EventHandler(this.btnClearHistory_Click);
            // 
            // panel1
            // 
            this.panel1.AutoScroll = true;
            this.panel1.AutoSize = true;
            this.panel1.Controls.Add(this.flpLotSummary);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(343, 38);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(505, 521);
            this.panel1.TabIndex = 1;
            // 
            // flpLotSummary
            // 
            this.flpLotSummary.AutoScroll = true;
            this.flpLotSummary.BackColor = System.Drawing.Color.White;
            this.flpLotSummary.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flpLotSummary.Location = new System.Drawing.Point(0, 0);
            this.flpLotSummary.Margin = new System.Windows.Forms.Padding(1);
            this.flpLotSummary.Name = "flpLotSummary";
            this.flpLotSummary.Size = new System.Drawing.Size(505, 521);
            this.flpLotSummary.TabIndex = 2;
            // 
            // dgvAiMonitor
            // 
            this.dgvAiMonitor.AllowUserToAddRows = false;
            this.dgvAiMonitor.AllowUserToDeleteRows = false;
            this.dgvAiMonitor.AllowUserToResizeColumns = false;
            this.dgvAiMonitor.AllowUserToResizeRows = false;
            this.dgvAiMonitor.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.dgvAiMonitor.ColumnHeadersBorderStyle = System.Windows.Forms.DataGridViewHeaderBorderStyle.Single;
            this.dgvAiMonitor.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleCenter;
            dataGridViewCellStyle1.BackColor = System.Drawing.SystemColors.Window;
            dataGridViewCellStyle1.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridViewCellStyle1.ForeColor = System.Drawing.SystemColors.ControlText;
            dataGridViewCellStyle1.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle1.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle1.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
            this.dgvAiMonitor.DefaultCellStyle = dataGridViewCellStyle1;
            this.dgvAiMonitor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgvAiMonitor.Location = new System.Drawing.Point(3, 38);
            this.dgvAiMonitor.MultiSelect = false;
            this.dgvAiMonitor.Name = "dgvAiMonitor";
            this.dgvAiMonitor.ReadOnly = true;
            this.dgvAiMonitor.RowHeadersVisible = false;
            this.dgvAiMonitor.RowHeadersWidthSizeMode = System.Windows.Forms.DataGridViewRowHeadersWidthSizeMode.DisableResizing;
            this.dgvAiMonitor.RowTemplate.Height = 23;
            this.dgvAiMonitor.Size = new System.Drawing.Size(334, 521);
            this.dgvAiMonitor.TabIndex = 2;
            // 
            // lblSJMonitor
            // 
            this.lblSJMonitor.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.lblSJMonitor.BackColor = System.Drawing.Color.Transparent;
            this.lblSJMonitor.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.lblSJMonitor.BorderRadius = 4;
            this.lblSJMonitor.BorderSize = 1;
            this.lblSJMonitor.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblSJMonitor.FillColor = System.Drawing.Color.FromArgb(((int)(((byte)(18)))), ((int)(((byte)(27)))), ((int)(((byte)(38)))));
            this.lblSJMonitor.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblSJMonitor.ImagePadding = 6;
            this.lblSJMonitor.ImageSize = 18;
            this.lblSJMonitor.LabelImage = null;
            this.lblSJMonitor.Location = new System.Drawing.Point(1, 1);
            this.lblSJMonitor.Margin = new System.Windows.Forms.Padding(1);
            this.lblSJMonitor.Name = "lblSJMonitor";
            this.lblSJMonitor.ShowAccentLine = false;
            this.lblSJMonitor.Size = new System.Drawing.Size(338, 33);
            this.lblSJMonitor.TabIndex = 3;
            this.lblSJMonitor.Text = "AI PROCESSING MONITOR";
            this.lblSJMonitor.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblSJMonitor.TextColor = System.Drawing.Color.White;
            // 
            // FormLotSummery
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(851, 562);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "FormLotSummery";
            this.Text = "FormLotSummery";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormLotSummery_FormClosing);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel2.PerformLayout();
            this.panel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dgvAiMonitor)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.RadioButton rbShowNormal;
        private System.Windows.Forms.RadioButton rbShowNG;
        private System.Windows.Forms.RadioButton rbShowAll;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.FlowLayoutPanel flpLotSummary;
        private System.Windows.Forms.DataGridView dgvAiMonitor;
        private CustomControls.ShadedLabel lblSJMonitor;
        private CustomControls.ShadedButton btnClearHistory;
    }
}