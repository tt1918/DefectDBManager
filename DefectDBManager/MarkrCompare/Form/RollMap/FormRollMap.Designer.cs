namespace MarkCompare
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
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.rbScale1 = new System.Windows.Forms.RadioButton();
            this.rbScale2 = new System.Windows.Forms.RadioButton();
            this.rbScale3 = new System.Windows.Forms.RadioButton();
            this.rbScale4 = new System.Windows.Forms.RadioButton();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.lblLotName = new CustomControls.ShadedLabel();
            this.cbProcess = new System.Windows.Forms.ComboBox();
            this.tableLayoutPanel5 = new System.Windows.Forms.TableLayoutPanel();
            this.Rollmap = new Coss.Controls.Rollmap();
            this.tableLayoutPanel6 = new System.Windows.Forms.TableLayoutPanel();
            this.lblCompInfo = new CustomControls.ShadedLabel();
            this.lblRefInfo = new CustomControls.ShadedLabel();
            this.lbxCompItem = new System.Windows.Forms.ListBox();
            this.lbxRefInfo = new System.Windows.Forms.ListBox();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel4.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.tableLayoutPanel3.SuspendLayout();
            this.tableLayoutPanel5.SuspendLayout();
            this.tableLayoutPanel6.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel4
            // 
            this.tableLayoutPanel4.ColumnCount = 1;
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel4.Controls.Add(this.tableLayoutPanel1, 0, 2);
            this.tableLayoutPanel4.Controls.Add(this.tableLayoutPanel3, 0, 0);
            this.tableLayoutPanel4.Controls.Add(this.tableLayoutPanel5, 0, 1);
            this.tableLayoutPanel4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel4.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel4.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel4.Name = "tableLayoutPanel4";
            this.tableLayoutPanel4.RowCount = 3;
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 28F));
            this.tableLayoutPanel4.Size = new System.Drawing.Size(843, 748);
            this.tableLayoutPanel4.TabIndex = 1;
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
            this.tableLayoutPanel1.Size = new System.Drawing.Size(843, 28);
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
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.ColumnCount = 3;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 200F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 364F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.Controls.Add(this.lblLotName, 0, 0);
            this.tableLayoutPanel3.Controls.Add(this.cbProcess, 1, 0);
            this.tableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel3.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel3.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 1;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(843, 30);
            this.tableLayoutPanel3.TabIndex = 2;
            // 
            // lblLotName
            // 
            this.lblLotName.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.lblLotName.BackColor = System.Drawing.Color.Transparent;
            this.lblLotName.BorderColor = System.Drawing.Color.MidnightBlue;
            this.lblLotName.BorderRadius = 3;
            this.lblLotName.BorderSize = 1;
            this.lblLotName.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblLotName.FillColor = System.Drawing.Color.DimGray;
            this.lblLotName.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblLotName.ImagePadding = 6;
            this.lblLotName.ImageSize = 18;
            this.lblLotName.LabelImage = null;
            this.lblLotName.Location = new System.Drawing.Point(1, 1);
            this.lblLotName.Margin = new System.Windows.Forms.Padding(1);
            this.lblLotName.Name = "lblLotName";
            this.lblLotName.ShowAccentLine = false;
            this.lblLotName.Size = new System.Drawing.Size(198, 28);
            this.lblLotName.TabIndex = 17;
            this.lblLotName.Text = "LINE DATA SETTING";
            this.lblLotName.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblLotName.TextColor = System.Drawing.Color.White;
            // 
            // cbProcess
            // 
            this.cbProcess.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cbProcess.FormattingEnabled = true;
            this.cbProcess.Location = new System.Drawing.Point(203, 3);
            this.cbProcess.Name = "cbProcess";
            this.cbProcess.Size = new System.Drawing.Size(358, 23);
            this.cbProcess.TabIndex = 0;
            this.cbProcess.SelectedIndexChanged += new System.EventHandler(this.cbProcess_SelectedIndexChanged);
            // 
            // tableLayoutPanel5
            // 
            this.tableLayoutPanel5.ColumnCount = 2;
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 78.73357F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 21.26643F));
            this.tableLayoutPanel5.Controls.Add(this.Rollmap, 0, 0);
            this.tableLayoutPanel5.Controls.Add(this.tableLayoutPanel6, 1, 0);
            this.tableLayoutPanel5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel5.Location = new System.Drawing.Point(0, 30);
            this.tableLayoutPanel5.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel5.Name = "tableLayoutPanel5";
            this.tableLayoutPanel5.RowCount = 1;
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 690F));
            this.tableLayoutPanel5.Size = new System.Drawing.Size(843, 690);
            this.tableLayoutPanel5.TabIndex = 3;
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
            this.Rollmap.Location = new System.Drawing.Point(3, 6);
            this.Rollmap.MapManualStartPos = 0;
            this.Rollmap.Margin = new System.Windows.Forms.Padding(3, 6, 3, 6);
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
            this.Rollmap.Size = new System.Drawing.Size(657, 678);
            this.Rollmap.StdOptic = 0;
            this.Rollmap.TabIndex = 3;
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
            // tableLayoutPanel6
            // 
            this.tableLayoutPanel6.ColumnCount = 1;
            this.tableLayoutPanel6.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel6.Controls.Add(this.lblCompInfo, 0, 2);
            this.tableLayoutPanel6.Controls.Add(this.lblRefInfo, 0, 0);
            this.tableLayoutPanel6.Controls.Add(this.lbxCompItem, 0, 3);
            this.tableLayoutPanel6.Controls.Add(this.lbxRefInfo, 0, 1);
            this.tableLayoutPanel6.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel6.Location = new System.Drawing.Point(664, 1);
            this.tableLayoutPanel6.Margin = new System.Windows.Forms.Padding(1);
            this.tableLayoutPanel6.Name = "tableLayoutPanel6";
            this.tableLayoutPanel6.RowCount = 4;
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 25F));
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 25F));
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel6.Size = new System.Drawing.Size(178, 688);
            this.tableLayoutPanel6.TabIndex = 4;
            // 
            // lblCompInfo
            // 
            this.lblCompInfo.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.lblCompInfo.BackColor = System.Drawing.Color.Transparent;
            this.lblCompInfo.BorderColor = System.Drawing.Color.MidnightBlue;
            this.lblCompInfo.BorderRadius = 5;
            this.lblCompInfo.BorderSize = 0;
            this.lblCompInfo.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblCompInfo.FillColor = System.Drawing.Color.DarkGray;
            this.lblCompInfo.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblCompInfo.ImagePadding = 6;
            this.lblCompInfo.ImageSize = 18;
            this.lblCompInfo.LabelImage = null;
            this.lblCompInfo.Location = new System.Drawing.Point(1, 345);
            this.lblCompInfo.Margin = new System.Windows.Forms.Padding(1);
            this.lblCompInfo.Name = "lblCompInfo";
            this.lblCompInfo.ShowAccentLine = false;
            this.lblCompInfo.Size = new System.Drawing.Size(176, 23);
            this.lblCompInfo.TabIndex = 20;
            this.lblCompInfo.Text = "COMPARING";
            this.lblCompInfo.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblCompInfo.TextColor = System.Drawing.Color.Black;
            // 
            // lblRefInfo
            // 
            this.lblRefInfo.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.lblRefInfo.BackColor = System.Drawing.Color.Transparent;
            this.lblRefInfo.BorderColor = System.Drawing.Color.MidnightBlue;
            this.lblRefInfo.BorderRadius = 5;
            this.lblRefInfo.BorderSize = 0;
            this.lblRefInfo.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblRefInfo.FillColor = System.Drawing.Color.DarkGray;
            this.lblRefInfo.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblRefInfo.ImagePadding = 6;
            this.lblRefInfo.ImageSize = 18;
            this.lblRefInfo.LabelImage = null;
            this.lblRefInfo.Location = new System.Drawing.Point(1, 1);
            this.lblRefInfo.Margin = new System.Windows.Forms.Padding(1);
            this.lblRefInfo.Name = "lblRefInfo";
            this.lblRefInfo.ShowAccentLine = false;
            this.lblRefInfo.Size = new System.Drawing.Size(176, 23);
            this.lblRefInfo.TabIndex = 18;
            this.lblRefInfo.Text = "BASE";
            this.lblRefInfo.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblRefInfo.TextColor = System.Drawing.Color.Black;
            // 
            // lbxCompItem
            // 
            this.lbxCompItem.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lbxCompItem.FormattingEnabled = true;
            this.lbxCompItem.ItemHeight = 15;
            this.lbxCompItem.Location = new System.Drawing.Point(3, 372);
            this.lbxCompItem.Name = "lbxCompItem";
            this.lbxCompItem.Size = new System.Drawing.Size(172, 313);
            this.lbxCompItem.TabIndex = 6;
            // 
            // lbxRefInfo
            // 
            this.lbxRefInfo.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lbxRefInfo.FormattingEnabled = true;
            this.lbxRefInfo.ItemHeight = 15;
            this.lbxRefInfo.Location = new System.Drawing.Point(3, 28);
            this.lbxRefInfo.Name = "lbxRefInfo";
            this.lbxRefInfo.Size = new System.Drawing.Size(172, 313);
            this.lbxRefInfo.TabIndex = 7;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 1;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel2.Controls.Add(this.tableLayoutPanel4, 0, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel2.Margin = new System.Windows.Forms.Padding(1);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(843, 748);
            this.tableLayoutPanel2.TabIndex = 2;
            // 
            // FormRollMap
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.WhiteSmoke;
            this.ClientSize = new System.Drawing.Size(843, 748);
            this.Controls.Add(this.tableLayoutPanel2);
            this.DoubleBuffered = true;
            this.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "FormRollMap";
            this.Text = "FormRollMap";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormRollMap_FormClosing);
            this.Load += new System.EventHandler(this.FormRollMap_Load);
            this.tableLayoutPanel4.ResumeLayout(false);
            this.tableLayoutPanel4.PerformLayout();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.tableLayoutPanel3.ResumeLayout(false);
            this.tableLayoutPanel5.ResumeLayout(false);
            this.tableLayoutPanel6.ResumeLayout(false);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel4;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.RadioButton rbScale1;
        private System.Windows.Forms.RadioButton rbScale2;
        private System.Windows.Forms.RadioButton rbScale3;
        private System.Windows.Forms.RadioButton rbScale4;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private System.Windows.Forms.ComboBox cbProcess;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel5;
        private Coss.Controls.Rollmap Rollmap;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel6;
        private System.Windows.Forms.ListBox lbxCompItem;
        private System.Windows.Forms.ListBox lbxRefInfo;
        private CustomControls.ShadedLabel lblLotName;
        private CustomControls.ShadedLabel lblCompInfo;
        private CustomControls.ShadedLabel lblRefInfo;
    }
}