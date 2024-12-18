namespace MarkrCompare
{
    partial class FormRollMap
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
            this.tableLayoutPanel4 = new System.Windows.Forms.TableLayoutPanel();
            this.Rollmap = new Coss.Controls.Rollmap();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.rbScale1 = new System.Windows.Forms.RadioButton();
            this.rbScale2 = new System.Windows.Forms.RadioButton();
            this.rbScale3 = new System.Windows.Forms.RadioButton();
            this.rbScale4 = new System.Windows.Forms.RadioButton();
            this.tableLayoutPanel4.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel4
            // 
            this.tableLayoutPanel4.ColumnCount = 1;
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel4.Controls.Add(this.Rollmap, 0, 0);
            this.tableLayoutPanel4.Controls.Add(this.tableLayoutPanel1, 0, 1);
            this.tableLayoutPanel4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel4.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel4.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel4.Name = "tableLayoutPanel4";
            this.tableLayoutPanel4.RowCount = 2;
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 28F));
            this.tableLayoutPanel4.Size = new System.Drawing.Size(493, 748);
            this.tableLayoutPanel4.TabIndex = 1;
            // 
            // Rollmap
            // 
            this.Rollmap.Captured = false;
            this.Rollmap.ClickedColor = System.Drawing.Color.White;
            this.Rollmap.CompareShowDefect = null;
            this.Rollmap.CycleParam = null;
            this.Rollmap.DefectFont = null;
            this.Rollmap.DefectOffsetX = 0;
            this.Rollmap.DefectSearchRange = 20;
            this.Rollmap.DisableDraw = false;
            this.Rollmap.Dock = System.Windows.Forms.DockStyle.Fill;
            this.Rollmap.DsPosX = 0F;
            this.Rollmap.GraphType = Coss.Controls.Rollmap.OptionGraphType.kVerticalScroll;
            this.Rollmap.IsAiMode = false;
            this.Rollmap.Location = new System.Drawing.Point(3, 4);
            this.Rollmap.MapManualStartPos = 0;
            this.Rollmap.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Rollmap.MarkingSerieIndex = -1;
            this.Rollmap.Name = "Rollmap";
            this.Rollmap.OsPosX = 0F;
            this.Rollmap.OsViewPosX = 0F;
            this.Rollmap.PrevCompareSameRange = new System.Drawing.Point(0, 0);
            this.Rollmap.RelativeCoordinate = false;
            this.Rollmap.ShowDefect = true;
            this.Rollmap.ShowEdge = false;
            this.Rollmap.ShowGraphics = false;
            this.Rollmap.ShowOS = true;
            this.Rollmap.ShowPeriod = false;
            this.Rollmap.ShowPeriodDefect = null;
            this.Rollmap.ShowSameDefect = null;
            this.Rollmap.Size = new System.Drawing.Size(487, 712);
            this.Rollmap.StdOptic = 0;
            this.Rollmap.TabIndex = 0;
            this.Rollmap.UseColor2 = false;
            this.Rollmap.UseComparePosition = false;
            this.Rollmap.UseDefectDelete = false;
            this.Rollmap.UseGradation = false;
            this.Rollmap.UsePeriodClassification = false;
            this.Rollmap.UsePrevCompare = false;
            this.Rollmap.UseSameDefect = false;
            this.Rollmap.UseVscroll = false;
            this.Rollmap.ViewRealRect = new System.Drawing.Rectangle(0, 0, 0, 0);
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.AutoSize = true;
            this.tableLayoutPanel1.ColumnCount = 6;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 10F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 65F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 65F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 70F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.rbScale1, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.rbScale2, 2, 0);
            this.tableLayoutPanel1.Controls.Add(this.rbScale3, 3, 0);
            this.tableLayoutPanel1.Controls.Add(this.rbScale4, 4, 0);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 720);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 1;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(493, 28);
            this.tableLayoutPanel1.TabIndex = 1;
            // 
            // rbScale1
            // 
            this.rbScale1.AutoSize = true;
            this.rbScale1.Checked = true;
            this.rbScale1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rbScale1.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbScale1.Location = new System.Drawing.Point(13, 3);
            this.rbScale1.Name = "rbScale1";
            this.rbScale1.Size = new System.Drawing.Size(54, 22);
            this.rbScale1.TabIndex = 0;
            this.rbScale1.TabStop = true;
            this.rbScale1.Text = "10M";
            this.rbScale1.UseVisualStyleBackColor = true;
            this.rbScale1.CheckedChanged += new System.EventHandler(this.radioButton_CheckedChanged);
            // 
            // rbScale2
            // 
            this.rbScale2.AutoSize = true;
            this.rbScale2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rbScale2.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbScale2.Location = new System.Drawing.Point(73, 3);
            this.rbScale2.Name = "rbScale2";
            this.rbScale2.Size = new System.Drawing.Size(59, 22);
            this.rbScale2.TabIndex = 0;
            this.rbScale2.Text = "100M";
            this.rbScale2.UseVisualStyleBackColor = true;
            this.rbScale2.CheckedChanged += new System.EventHandler(this.radioButton_CheckedChanged);
            // 
            // rbScale3
            // 
            this.rbScale3.AutoSize = true;
            this.rbScale3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rbScale3.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbScale3.Location = new System.Drawing.Point(138, 3);
            this.rbScale3.Name = "rbScale3";
            this.rbScale3.Size = new System.Drawing.Size(59, 22);
            this.rbScale3.TabIndex = 0;
            this.rbScale3.Text = "200M";
            this.rbScale3.UseVisualStyleBackColor = true;
            this.rbScale3.CheckedChanged += new System.EventHandler(this.radioButton_CheckedChanged);
            // 
            // rbScale4
            // 
            this.rbScale4.AutoSize = true;
            this.rbScale4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rbScale4.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rbScale4.Location = new System.Drawing.Point(203, 3);
            this.rbScale4.Name = "rbScale4";
            this.rbScale4.Size = new System.Drawing.Size(64, 22);
            this.rbScale4.TabIndex = 0;
            this.rbScale4.Text = "1000M";
            this.rbScale4.UseVisualStyleBackColor = true;
            this.rbScale4.CheckedChanged += new System.EventHandler(this.radioButton_CheckedChanged);
            // 
            // FormRollMap
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.Azure;
            this.ClientSize = new System.Drawing.Size(493, 748);
            this.Controls.Add(this.tableLayoutPanel4);
            this.DoubleBuffered = true;
            this.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "FormRollMap";
            this.Text = "FormRollMap";
            this.tableLayoutPanel4.ResumeLayout(false);
            this.tableLayoutPanel4.PerformLayout();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel4;
        private Coss.Controls.Rollmap Rollmap;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.RadioButton rbScale1;
        private System.Windows.Forms.RadioButton rbScale2;
        private System.Windows.Forms.RadioButton rbScale3;
        private System.Windows.Forms.RadioButton rbScale4;
    }
}