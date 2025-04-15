namespace MarkrCompare
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
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.btnClearHistory = new CustomControls.RoundButton();
            this.rbShowNormal = new System.Windows.Forms.RadioButton();
            this.rbShowNG = new System.Windows.Forms.RadioButton();
            this.rbShowAll = new System.Windows.Forms.RadioButton();
            this.panel1 = new System.Windows.Forms.Panel();
            this.flpLotSummary = new System.Windows.Forms.FlowLayoutPanel();
            this.tableLayoutPanel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.panel1, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(1);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(536, 562);
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
            this.tableLayoutPanel2.Controls.Add(this.btnClearHistory, 3, 0);
            this.tableLayoutPanel2.Controls.Add(this.rbShowNormal, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.rbShowNG, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.rbShowAll, 2, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(1, 1);
            this.tableLayoutPanel2.Margin = new System.Windows.Forms.Padding(1);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(534, 33);
            this.tableLayoutPanel2.TabIndex = 0;
            // 
            // btnClearHistory
            // 
            this.btnClearHistory.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnClearHistory.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnClearHistory.BorderColor = System.Drawing.Color.Lavender;
            this.btnClearHistory.BorderRadius = 5;
            this.btnClearHistory.BorderSize = 2;
            this.btnClearHistory.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnClearHistory.FlatAppearance.BorderSize = 0;
            this.btnClearHistory.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnClearHistory.ForeColor = System.Drawing.Color.White;
            this.btnClearHistory.Location = new System.Drawing.Point(301, 1);
            this.btnClearHistory.Margin = new System.Windows.Forms.Padding(1);
            this.btnClearHistory.Name = "btnClearHistory";
            this.btnClearHistory.Size = new System.Drawing.Size(98, 31);
            this.btnClearHistory.TabIndex = 11;
            this.btnClearHistory.Text = "이력 삭제";
            this.btnClearHistory.TextColor = System.Drawing.Color.White;
            this.btnClearHistory.UseVisualStyleBackColor = false;
            this.btnClearHistory.Click += new System.EventHandler(this.btnClearHistory_Click);
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
            // panel1
            // 
            this.panel1.AutoScroll = true;
            this.panel1.AutoSize = true;
            this.panel1.Controls.Add(this.flpLotSummary);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(3, 38);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(530, 521);
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
            this.flpLotSummary.Size = new System.Drawing.Size(530, 521);
            this.flpLotSummary.TabIndex = 2;
            // 
            // FormLotSummery
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(536, 562);
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
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.RadioButton rbShowNormal;
        private System.Windows.Forms.RadioButton rbShowNG;
        private System.Windows.Forms.RadioButton rbShowAll;
        private CustomControls.RoundButton btnClearHistory;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.FlowLayoutPanel flpLotSummary;
    }
}