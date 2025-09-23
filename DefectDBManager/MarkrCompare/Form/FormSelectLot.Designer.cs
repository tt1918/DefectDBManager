namespace MarkCompare
{
    partial class FormSelectedLot
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
            this.lblTitle = new CustomControls.RoundLabel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.btnOk = new CustomControls.RoundButton();
            this.btnCancel = new CustomControls.RoundButton();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel15 = new System.Windows.Forms.TableLayoutPanel();
            this.groupCompRange = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel11 = new System.Windows.Forms.TableLayoutPanel();
            this.dgvCompRange = new System.Windows.Forms.DataGridView();
            this.tableLayoutPanel12 = new System.Windows.Forms.TableLayoutPanel();
            this.btnAddCompRange = new CustomControls.RoundButton();
            this.btnDelCompRange = new CustomControls.RoundButton();
            this.gpJudgeRange = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel16 = new System.Windows.Forms.TableLayoutPanel();
            this.tbJudgeRangeX = new CustomControls.RoundTextBox();
            this.lblRangeX = new CustomControls.RoundLabel();
            this.lblRangeY = new CustomControls.RoundLabel();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.tbJudgeRangeY = new CustomControls.RoundTextBox();
            this.tableLayoutPanel19 = new System.Windows.Forms.TableLayoutPanel();
            this.cbUseMNTTAN = new System.Windows.Forms.CheckBox();
            this.gbFilterType = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel4 = new System.Windows.Forms.TableLayoutPanel();
            this.lvFilterList = new System.Windows.Forms.ListView();
            this.rbFilterType2 = new System.Windows.Forms.RadioButton();
            this.tableLayoutPanel6 = new System.Windows.Forms.TableLayoutPanel();
            this.btnAddUserFilter = new CustomControls.RoundButton();
            this.rbFilterType1 = new System.Windows.Forms.RadioButton();
            this.tableLayoutPanel7 = new System.Windows.Forms.TableLayoutPanel();
            this.cbDest = new System.Windows.Forms.ComboBox();
            this.ckbES = new System.Windows.Forms.CheckBox();
            this.ckbTG = new System.Windows.Forms.CheckBox();
            this.ckbETC = new System.Windows.Forms.CheckBox();
            this.tableLayoutPanel5 = new System.Windows.Forms.TableLayoutPanel();
            this.btnSaveParam = new CustomControls.RoundButton();
            this.btnLoadParam = new CustomControls.RoundButton();
            this.tableLayoutPanel8 = new System.Windows.Forms.TableLayoutPanel();
            this.dgvLotList = new System.Windows.Forms.DataGridView();
            this.tableLayoutPanel9 = new System.Windows.Forms.TableLayoutPanel();
            this.btnClear = new CustomControls.RoundButton();
            this.btnDel = new CustomControls.RoundButton();
            this.btnAdd = new CustomControls.RoundButton();
            this.tableLayoutPanel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.tableLayoutPanel3.SuspendLayout();
            this.tableLayoutPanel15.SuspendLayout();
            this.groupCompRange.SuspendLayout();
            this.tableLayoutPanel11.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvCompRange)).BeginInit();
            this.tableLayoutPanel12.SuspendLayout();
            this.gpJudgeRange.SuspendLayout();
            this.tableLayoutPanel16.SuspendLayout();
            this.tableLayoutPanel19.SuspendLayout();
            this.gbFilterType.SuspendLayout();
            this.tableLayoutPanel4.SuspendLayout();
            this.tableLayoutPanel6.SuspendLayout();
            this.tableLayoutPanel7.SuspendLayout();
            this.tableLayoutPanel5.SuspendLayout();
            this.tableLayoutPanel8.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvLotList)).BeginInit();
            this.tableLayoutPanel9.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.lblTitle, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel3, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(1);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 3;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 48F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(644, 675);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // lblTitle
            // 
            this.lblTitle.AutoSize = true;
            this.lblTitle.BkColor = System.Drawing.Color.DarkSlateBlue;
            this.lblTitle.BorderColor = System.Drawing.Color.MidnightBlue;
            this.lblTitle.CornerR = 10;
            this.lblTitle.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblTitle.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold);
            this.lblTitle.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.lblTitle.IsFillLB = false;
            this.lblTitle.IsFillLT = false;
            this.lblTitle.IsFillRB = false;
            this.lblTitle.IsFillRT = false;
            this.lblTitle.Location = new System.Drawing.Point(2, 2);
            this.lblTitle.Margin = new System.Windows.Forms.Padding(2);
            this.lblTitle.Name = "lblTitle";
            this.lblTitle.Size = new System.Drawing.Size(640, 26);
            this.lblTitle.TabIndex = 0;
            this.lblTitle.Text = "LOT 지정 검사 설정";
            this.lblTitle.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblTitle.Thickness = 1;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 3;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 72.04969F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 14.44099F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 13.35404F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel2.Controls.Add(this.btnOk, 2, 0);
            this.tableLayoutPanel2.Controls.Add(this.btnCancel, 1, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(0, 627);
            this.tableLayoutPanel2.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(644, 48);
            this.tableLayoutPanel2.TabIndex = 2;
            // 
            // btnOk
            // 
            this.btnOk.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnOk.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnOk.BorderColor = System.Drawing.Color.Lavender;
            this.btnOk.BorderRadius = 1;
            this.btnOk.BorderSize = 2;
            this.btnOk.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnOk.FlatAppearance.BorderSize = 0;
            this.btnOk.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnOk.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.btnOk.ForeColor = System.Drawing.Color.White;
            this.btnOk.Location = new System.Drawing.Point(558, 1);
            this.btnOk.Margin = new System.Windows.Forms.Padding(1);
            this.btnOk.Name = "btnOk";
            this.btnOk.Size = new System.Drawing.Size(85, 46);
            this.btnOk.TabIndex = 7;
            this.btnOk.Text = "OK";
            this.btnOk.TextColor = System.Drawing.Color.White;
            this.btnOk.UseVisualStyleBackColor = false;
            this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnCancel.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnCancel.BorderColor = System.Drawing.Color.Lavender;
            this.btnCancel.BorderRadius = 1;
            this.btnCancel.BorderSize = 2;
            this.btnCancel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnCancel.FlatAppearance.BorderSize = 0;
            this.btnCancel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnCancel.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.btnCancel.ForeColor = System.Drawing.Color.White;
            this.btnCancel.Location = new System.Drawing.Point(465, 1);
            this.btnCancel.Margin = new System.Windows.Forms.Padding(1);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(91, 46);
            this.btnCancel.TabIndex = 3;
            this.btnCancel.Text = "CANCLE";
            this.btnCancel.TextColor = System.Drawing.Color.White;
            this.btnCancel.UseVisualStyleBackColor = false;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.ColumnCount = 2;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 54.07279F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 45.92721F));
            this.tableLayoutPanel3.Controls.Add(this.tableLayoutPanel15, 0, 0);
            this.tableLayoutPanel3.Controls.Add(this.tableLayoutPanel8, 1, 0);
            this.tableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel3.Location = new System.Drawing.Point(3, 34);
            this.tableLayoutPanel3.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 1;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 589F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(638, 589);
            this.tableLayoutPanel3.TabIndex = 10;
            // 
            // tableLayoutPanel15
            // 
            this.tableLayoutPanel15.ColumnCount = 1;
            this.tableLayoutPanel15.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel15.Controls.Add(this.groupCompRange, 0, 1);
            this.tableLayoutPanel15.Controls.Add(this.gpJudgeRange, 0, 2);
            this.tableLayoutPanel15.Controls.Add(this.tableLayoutPanel19, 0, 3);
            this.tableLayoutPanel15.Controls.Add(this.gbFilterType, 0, 0);
            this.tableLayoutPanel15.Controls.Add(this.tableLayoutPanel5, 0, 4);
            this.tableLayoutPanel15.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel15.Location = new System.Drawing.Point(2, 2);
            this.tableLayoutPanel15.Margin = new System.Windows.Forms.Padding(2);
            this.tableLayoutPanel15.Name = "tableLayoutPanel15";
            this.tableLayoutPanel15.Padding = new System.Windows.Forms.Padding(2);
            this.tableLayoutPanel15.RowCount = 5;
            this.tableLayoutPanel15.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 246F));
            this.tableLayoutPanel15.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel15.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 66F));
            this.tableLayoutPanel15.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 31F));
            this.tableLayoutPanel15.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 41F));
            this.tableLayoutPanel15.Size = new System.Drawing.Size(340, 585);
            this.tableLayoutPanel15.TabIndex = 5;
            // 
            // groupCompRange
            // 
            this.groupCompRange.Controls.Add(this.tableLayoutPanel11);
            this.groupCompRange.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupCompRange.Location = new System.Drawing.Point(4, 250);
            this.groupCompRange.Margin = new System.Windows.Forms.Padding(2);
            this.groupCompRange.Name = "groupCompRange";
            this.groupCompRange.Padding = new System.Windows.Forms.Padding(1, 1, 1, 5);
            this.groupCompRange.Size = new System.Drawing.Size(332, 193);
            this.groupCompRange.TabIndex = 3;
            this.groupCompRange.TabStop = false;
            this.groupCompRange.Text = "COMPARE RANGE";
            // 
            // tableLayoutPanel11
            // 
            this.tableLayoutPanel11.ColumnCount = 1;
            this.tableLayoutPanel11.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel11.Controls.Add(this.dgvCompRange, 0, 0);
            this.tableLayoutPanel11.Controls.Add(this.tableLayoutPanel12, 0, 1);
            this.tableLayoutPanel11.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel11.Location = new System.Drawing.Point(1, 17);
            this.tableLayoutPanel11.Margin = new System.Windows.Forms.Padding(2);
            this.tableLayoutPanel11.Name = "tableLayoutPanel11";
            this.tableLayoutPanel11.Padding = new System.Windows.Forms.Padding(2);
            this.tableLayoutPanel11.RowCount = 2;
            this.tableLayoutPanel11.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel11.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 32F));
            this.tableLayoutPanel11.Size = new System.Drawing.Size(330, 171);
            this.tableLayoutPanel11.TabIndex = 1;
            // 
            // dgvCompRange
            // 
            this.dgvCompRange.AllowUserToAddRows = false;
            this.dgvCompRange.AllowUserToDeleteRows = false;
            this.dgvCompRange.AllowUserToResizeColumns = false;
            this.dgvCompRange.AllowUserToResizeRows = false;
            this.dgvCompRange.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvCompRange.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgvCompRange.Location = new System.Drawing.Point(3, 3);
            this.dgvCompRange.Margin = new System.Windows.Forms.Padding(1);
            this.dgvCompRange.Name = "dgvCompRange";
            this.dgvCompRange.RowTemplate.Height = 23;
            this.dgvCompRange.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dgvCompRange.Size = new System.Drawing.Size(324, 133);
            this.dgvCompRange.TabIndex = 5;
            // 
            // tableLayoutPanel12
            // 
            this.tableLayoutPanel12.ColumnCount = 3;
            this.tableLayoutPanel12.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel12.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel12.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel12.Controls.Add(this.btnAddCompRange, 1, 0);
            this.tableLayoutPanel12.Controls.Add(this.btnDelCompRange, 2, 0);
            this.tableLayoutPanel12.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel12.Location = new System.Drawing.Point(2, 137);
            this.tableLayoutPanel12.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel12.Name = "tableLayoutPanel12";
            this.tableLayoutPanel12.RowCount = 1;
            this.tableLayoutPanel12.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel12.Size = new System.Drawing.Size(326, 32);
            this.tableLayoutPanel12.TabIndex = 0;
            // 
            // btnAddCompRange
            // 
            this.btnAddCompRange.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAddCompRange.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAddCompRange.BorderColor = System.Drawing.Color.Lavender;
            this.btnAddCompRange.BorderRadius = 3;
            this.btnAddCompRange.BorderSize = 2;
            this.btnAddCompRange.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnAddCompRange.FlatAppearance.BorderSize = 0;
            this.btnAddCompRange.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnAddCompRange.ForeColor = System.Drawing.Color.White;
            this.btnAddCompRange.Location = new System.Drawing.Point(207, 1);
            this.btnAddCompRange.Margin = new System.Windows.Forms.Padding(1);
            this.btnAddCompRange.Name = "btnAddCompRange";
            this.btnAddCompRange.Size = new System.Drawing.Size(58, 30);
            this.btnAddCompRange.TabIndex = 1;
            this.btnAddCompRange.Text = "ADD";
            this.btnAddCompRange.TextColor = System.Drawing.Color.White;
            this.btnAddCompRange.UseVisualStyleBackColor = false;
            this.btnAddCompRange.Click += new System.EventHandler(this.btnAddCompRange_Click);
            // 
            // btnDelCompRange
            // 
            this.btnDelCompRange.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnDelCompRange.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnDelCompRange.BorderColor = System.Drawing.Color.Lavender;
            this.btnDelCompRange.BorderRadius = 3;
            this.btnDelCompRange.BorderSize = 2;
            this.btnDelCompRange.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnDelCompRange.FlatAppearance.BorderSize = 0;
            this.btnDelCompRange.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnDelCompRange.ForeColor = System.Drawing.Color.White;
            this.btnDelCompRange.Location = new System.Drawing.Point(267, 1);
            this.btnDelCompRange.Margin = new System.Windows.Forms.Padding(1);
            this.btnDelCompRange.Name = "btnDelCompRange";
            this.btnDelCompRange.Size = new System.Drawing.Size(58, 30);
            this.btnDelCompRange.TabIndex = 1;
            this.btnDelCompRange.Text = "DEL";
            this.btnDelCompRange.TextColor = System.Drawing.Color.White;
            this.btnDelCompRange.UseVisualStyleBackColor = false;
            this.btnDelCompRange.Click += new System.EventHandler(this.btnDelCompRange_Click);
            // 
            // gpJudgeRange
            // 
            this.gpJudgeRange.Controls.Add(this.tableLayoutPanel16);
            this.gpJudgeRange.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gpJudgeRange.Location = new System.Drawing.Point(4, 447);
            this.gpJudgeRange.Margin = new System.Windows.Forms.Padding(2);
            this.gpJudgeRange.Name = "gpJudgeRange";
            this.gpJudgeRange.Padding = new System.Windows.Forms.Padding(2);
            this.gpJudgeRange.Size = new System.Drawing.Size(332, 62);
            this.gpJudgeRange.TabIndex = 4;
            this.gpJudgeRange.TabStop = false;
            this.gpJudgeRange.Text = "JUDGE RANGE";
            // 
            // tableLayoutPanel16
            // 
            this.tableLayoutPanel16.ColumnCount = 6;
            this.tableLayoutPanel16.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel16.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel16.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel16.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel16.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel16.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 39F));
            this.tableLayoutPanel16.Controls.Add(this.tbJudgeRangeX, 1, 0);
            this.tableLayoutPanel16.Controls.Add(this.lblRangeX, 0, 0);
            this.tableLayoutPanel16.Controls.Add(this.lblRangeY, 3, 0);
            this.tableLayoutPanel16.Controls.Add(this.label1, 2, 0);
            this.tableLayoutPanel16.Controls.Add(this.label2, 5, 0);
            this.tableLayoutPanel16.Controls.Add(this.tbJudgeRangeY, 4, 0);
            this.tableLayoutPanel16.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel16.Location = new System.Drawing.Point(2, 18);
            this.tableLayoutPanel16.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.tableLayoutPanel16.Name = "tableLayoutPanel16";
            this.tableLayoutPanel16.Padding = new System.Windows.Forms.Padding(2);
            this.tableLayoutPanel16.RowCount = 1;
            this.tableLayoutPanel16.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel16.Size = new System.Drawing.Size(328, 42);
            this.tableLayoutPanel16.TabIndex = 0;
            // 
            // tbJudgeRangeX
            // 
            this.tbJudgeRangeX.BackColor = System.Drawing.SystemColors.Window;
            this.tbJudgeRangeX.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(44)))), ((int)(((byte)(61)))), ((int)(((byte)(90)))));
            this.tbJudgeRangeX.BorderFocusColor = System.Drawing.Color.FromArgb(((int)(((byte)(44)))), ((int)(((byte)(61)))), ((int)(((byte)(90)))));
            this.tbJudgeRangeX.BorderRadius = 5;
            this.tbJudgeRangeX.BorderSize = 2;
            this.tbJudgeRangeX.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbJudgeRangeX.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbJudgeRangeX.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.tbJudgeRangeX.Location = new System.Drawing.Point(38, 3);
            this.tbJudgeRangeX.Margin = new System.Windows.Forms.Padding(1);
            this.tbJudgeRangeX.Multiline = false;
            this.tbJudgeRangeX.Name = "tbJudgeRangeX";
            this.tbJudgeRangeX.Padding = new System.Windows.Forms.Padding(10, 9, 10, 9);
            this.tbJudgeRangeX.PasswordChar = false;
            this.tbJudgeRangeX.PlaceholderColor = System.Drawing.Color.DarkGray;
            this.tbJudgeRangeX.PlaceholderText = "";
            this.tbJudgeRangeX.Size = new System.Drawing.Size(88, 36);
            this.tbJudgeRangeX.TabIndex = 6;
            this.tbJudgeRangeX.Texts = "";
            this.tbJudgeRangeX.UnderlinedStyle = false;
            // 
            // lblRangeX
            // 
            this.lblRangeX.AutoSize = true;
            this.lblRangeX.BkColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(70)))));
            this.lblRangeX.BorderColor = System.Drawing.Color.Silver;
            this.lblRangeX.CornerR = 10;
            this.lblRangeX.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblRangeX.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblRangeX.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.lblRangeX.IsFillLB = false;
            this.lblRangeX.IsFillLT = false;
            this.lblRangeX.IsFillRB = false;
            this.lblRangeX.IsFillRT = false;
            this.lblRangeX.Location = new System.Drawing.Point(4, 4);
            this.lblRangeX.Margin = new System.Windows.Forms.Padding(2);
            this.lblRangeX.Name = "lblRangeX";
            this.lblRangeX.Size = new System.Drawing.Size(31, 34);
            this.lblRangeX.TabIndex = 3;
            this.lblRangeX.Text = "X";
            this.lblRangeX.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblRangeX.Thickness = 1;
            // 
            // lblRangeY
            // 
            this.lblRangeY.AutoSize = true;
            this.lblRangeY.BkColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(70)))));
            this.lblRangeY.BorderColor = System.Drawing.Color.Silver;
            this.lblRangeY.CornerR = 10;
            this.lblRangeY.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblRangeY.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblRangeY.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.lblRangeY.IsFillLB = false;
            this.lblRangeY.IsFillLT = false;
            this.lblRangeY.IsFillRB = false;
            this.lblRangeY.IsFillRT = false;
            this.lblRangeY.Location = new System.Drawing.Point(164, 4);
            this.lblRangeY.Margin = new System.Windows.Forms.Padding(2);
            this.lblRangeY.Name = "lblRangeY";
            this.lblRangeY.Size = new System.Drawing.Size(31, 34);
            this.lblRangeY.TabIndex = 3;
            this.lblRangeY.Text = "Y";
            this.lblRangeY.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblRangeY.Thickness = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.label1.ImageAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.label1.Location = new System.Drawing.Point(129, 25);
            this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 0, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(33, 15);
            this.label1.TabIndex = 4;
            this.label1.Text = "mm";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.label2.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.label2.Location = new System.Drawing.Point(289, 25);
            this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 0, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(37, 15);
            this.label2.TabIndex = 5;
            this.label2.Text = "M";
            // 
            // tbJudgeRangeY
            // 
            this.tbJudgeRangeY.BackColor = System.Drawing.SystemColors.Window;
            this.tbJudgeRangeY.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(44)))), ((int)(((byte)(61)))), ((int)(((byte)(90)))));
            this.tbJudgeRangeY.BorderFocusColor = System.Drawing.Color.FromArgb(((int)(((byte)(44)))), ((int)(((byte)(61)))), ((int)(((byte)(90)))));
            this.tbJudgeRangeY.BorderRadius = 5;
            this.tbJudgeRangeY.BorderSize = 2;
            this.tbJudgeRangeY.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbJudgeRangeY.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbJudgeRangeY.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.tbJudgeRangeY.Location = new System.Drawing.Point(198, 3);
            this.tbJudgeRangeY.Margin = new System.Windows.Forms.Padding(1);
            this.tbJudgeRangeY.Multiline = false;
            this.tbJudgeRangeY.Name = "tbJudgeRangeY";
            this.tbJudgeRangeY.Padding = new System.Windows.Forms.Padding(10, 9, 10, 9);
            this.tbJudgeRangeY.PasswordChar = false;
            this.tbJudgeRangeY.PlaceholderColor = System.Drawing.Color.DarkGray;
            this.tbJudgeRangeY.PlaceholderText = "";
            this.tbJudgeRangeY.Size = new System.Drawing.Size(88, 36);
            this.tbJudgeRangeY.TabIndex = 6;
            this.tbJudgeRangeY.Texts = "";
            this.tbJudgeRangeY.UnderlinedStyle = false;
            // 
            // tableLayoutPanel19
            // 
            this.tableLayoutPanel19.ColumnCount = 2;
            this.tableLayoutPanel19.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 164F));
            this.tableLayoutPanel19.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 222F));
            this.tableLayoutPanel19.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel19.Controls.Add(this.cbUseMNTTAN, 0, 0);
            this.tableLayoutPanel19.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel19.Location = new System.Drawing.Point(2, 511);
            this.tableLayoutPanel19.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel19.Name = "tableLayoutPanel19";
            this.tableLayoutPanel19.RowCount = 1;
            this.tableLayoutPanel19.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel19.Size = new System.Drawing.Size(336, 31);
            this.tableLayoutPanel19.TabIndex = 5;
            // 
            // cbUseMNTTAN
            // 
            this.cbUseMNTTAN.AutoSize = true;
            this.cbUseMNTTAN.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cbUseMNTTAN.Location = new System.Drawing.Point(3, 4);
            this.cbUseMNTTAN.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.cbUseMNTTAN.Name = "cbUseMNTTAN";
            this.cbUseMNTTAN.Size = new System.Drawing.Size(158, 23);
            this.cbUseMNTTAN.TabIndex = 10;
            this.cbUseMNTTAN.Text = "USE AI RESULT";
            this.cbUseMNTTAN.UseVisualStyleBackColor = true;
            // 
            // gbFilterType
            // 
            this.gbFilterType.Controls.Add(this.tableLayoutPanel4);
            this.gbFilterType.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbFilterType.Location = new System.Drawing.Point(5, 6);
            this.gbFilterType.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.gbFilterType.Name = "gbFilterType";
            this.gbFilterType.Padding = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.gbFilterType.Size = new System.Drawing.Size(330, 238);
            this.gbFilterType.TabIndex = 6;
            this.gbFilterType.TabStop = false;
            this.gbFilterType.Text = "Filter Type";
            // 
            // tableLayoutPanel4
            // 
            this.tableLayoutPanel4.ColumnCount = 1;
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel4.Controls.Add(this.lvFilterList, 0, 1);
            this.tableLayoutPanel4.Controls.Add(this.rbFilterType2, 0, 2);
            this.tableLayoutPanel4.Controls.Add(this.tableLayoutPanel6, 0, 0);
            this.tableLayoutPanel4.Controls.Add(this.tableLayoutPanel7, 0, 3);
            this.tableLayoutPanel4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel4.Location = new System.Drawing.Point(3, 20);
            this.tableLayoutPanel4.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.tableLayoutPanel4.Name = "tableLayoutPanel4";
            this.tableLayoutPanel4.RowCount = 4;
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel4.Size = new System.Drawing.Size(324, 214);
            this.tableLayoutPanel4.TabIndex = 0;
            // 
            // lvFilterList
            // 
            this.lvFilterList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lvFilterList.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.lvFilterList.HideSelection = false;
            this.lvFilterList.Location = new System.Drawing.Point(1, 31);
            this.lvFilterList.Margin = new System.Windows.Forms.Padding(1);
            this.lvFilterList.Name = "lvFilterList";
            this.lvFilterList.Size = new System.Drawing.Size(322, 122);
            this.lvFilterList.TabIndex = 5;
            this.lvFilterList.UseCompatibleStateImageBehavior = false;
            this.lvFilterList.View = System.Windows.Forms.View.List;
            // 
            // rbFilterType2
            // 
            this.rbFilterType2.AutoSize = true;
            this.rbFilterType2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rbFilterType2.Location = new System.Drawing.Point(3, 158);
            this.rbFilterType2.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.rbFilterType2.Name = "rbFilterType2";
            this.rbFilterType2.Size = new System.Drawing.Size(318, 22);
            this.rbFilterType2.TabIndex = 0;
            this.rbFilterType2.TabStop = true;
            this.rbFilterType2.Text = "DB 필터";
            this.rbFilterType2.UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanel6
            // 
            this.tableLayoutPanel6.ColumnCount = 2;
            this.tableLayoutPanel6.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 68.82716F));
            this.tableLayoutPanel6.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 31.17284F));
            this.tableLayoutPanel6.Controls.Add(this.btnAddUserFilter, 1, 0);
            this.tableLayoutPanel6.Controls.Add(this.rbFilterType1, 0, 0);
            this.tableLayoutPanel6.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel6.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel6.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel6.Name = "tableLayoutPanel6";
            this.tableLayoutPanel6.RowCount = 1;
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel6.Size = new System.Drawing.Size(324, 30);
            this.tableLayoutPanel6.TabIndex = 1;
            // 
            // btnAddUserFilter
            // 
            this.btnAddUserFilter.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAddUserFilter.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAddUserFilter.BorderColor = System.Drawing.Color.Lavender;
            this.btnAddUserFilter.BorderRadius = 3;
            this.btnAddUserFilter.BorderSize = 2;
            this.btnAddUserFilter.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnAddUserFilter.FlatAppearance.BorderSize = 0;
            this.btnAddUserFilter.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnAddUserFilter.ForeColor = System.Drawing.Color.White;
            this.btnAddUserFilter.Location = new System.Drawing.Point(224, 1);
            this.btnAddUserFilter.Margin = new System.Windows.Forms.Padding(1);
            this.btnAddUserFilter.Name = "btnAddUserFilter";
            this.btnAddUserFilter.Size = new System.Drawing.Size(99, 28);
            this.btnAddUserFilter.TabIndex = 2;
            this.btnAddUserFilter.Text = "ADD";
            this.btnAddUserFilter.TextColor = System.Drawing.Color.White;
            this.btnAddUserFilter.UseVisualStyleBackColor = false;
            this.btnAddUserFilter.Click += new System.EventHandler(this.btnAddUserFilter_Click);
            // 
            // rbFilterType1
            // 
            this.rbFilterType1.AutoSize = true;
            this.rbFilterType1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rbFilterType1.Location = new System.Drawing.Point(3, 4);
            this.rbFilterType1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.rbFilterType1.Name = "rbFilterType1";
            this.rbFilterType1.Size = new System.Drawing.Size(217, 22);
            this.rbFilterType1.TabIndex = 0;
            this.rbFilterType1.TabStop = true;
            this.rbFilterType1.Text = "유저 설정 필터";
            this.rbFilterType1.UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanel7
            // 
            this.tableLayoutPanel7.ColumnCount = 4;
            this.tableLayoutPanel7.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 40F));
            this.tableLayoutPanel7.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 20F));
            this.tableLayoutPanel7.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 20F));
            this.tableLayoutPanel7.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 20F));
            this.tableLayoutPanel7.Controls.Add(this.cbDest, 0, 0);
            this.tableLayoutPanel7.Controls.Add(this.ckbES, 1, 0);
            this.tableLayoutPanel7.Controls.Add(this.ckbTG, 2, 0);
            this.tableLayoutPanel7.Controls.Add(this.ckbETC, 3, 0);
            this.tableLayoutPanel7.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel7.Location = new System.Drawing.Point(1, 185);
            this.tableLayoutPanel7.Margin = new System.Windows.Forms.Padding(1);
            this.tableLayoutPanel7.Name = "tableLayoutPanel7";
            this.tableLayoutPanel7.RowCount = 1;
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel7.Size = new System.Drawing.Size(322, 28);
            this.tableLayoutPanel7.TabIndex = 6;
            // 
            // cbDest
            // 
            this.cbDest.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cbDest.FormattingEnabled = true;
            this.cbDest.Location = new System.Drawing.Point(1, 2);
            this.cbDest.Margin = new System.Windows.Forms.Padding(1, 2, 1, 1);
            this.cbDest.Name = "cbDest";
            this.cbDest.Size = new System.Drawing.Size(126, 23);
            this.cbDest.TabIndex = 0;
            this.cbDest.SelectedIndexChanged += new System.EventHandler(this.cbDest_SelectedIndexChanged);
            // 
            // ckbES
            // 
            this.ckbES.AutoSize = true;
            this.ckbES.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ckbES.Location = new System.Drawing.Point(131, 3);
            this.ckbES.Name = "ckbES";
            this.ckbES.Size = new System.Drawing.Size(58, 22);
            this.ckbES.TabIndex = 1;
            this.ckbES.Text = "ES";
            this.ckbES.UseVisualStyleBackColor = true;
            // 
            // ckbTG
            // 
            this.ckbTG.AutoSize = true;
            this.ckbTG.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ckbTG.Location = new System.Drawing.Point(195, 3);
            this.ckbTG.Name = "ckbTG";
            this.ckbTG.Size = new System.Drawing.Size(58, 22);
            this.ckbTG.TabIndex = 1;
            this.ckbTG.Text = "TG";
            this.ckbTG.UseVisualStyleBackColor = true;
            // 
            // ckbETC
            // 
            this.ckbETC.AutoSize = true;
            this.ckbETC.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ckbETC.Location = new System.Drawing.Point(259, 3);
            this.ckbETC.Name = "ckbETC";
            this.ckbETC.Size = new System.Drawing.Size(60, 22);
            this.ckbETC.TabIndex = 1;
            this.ckbETC.Text = "ETC";
            this.ckbETC.UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanel5
            // 
            this.tableLayoutPanel5.ColumnCount = 3;
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 70F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 70F));
            this.tableLayoutPanel5.Controls.Add(this.btnSaveParam, 2, 0);
            this.tableLayoutPanel5.Controls.Add(this.btnLoadParam, 1, 0);
            this.tableLayoutPanel5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel5.Location = new System.Drawing.Point(5, 546);
            this.tableLayoutPanel5.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.tableLayoutPanel5.Name = "tableLayoutPanel5";
            this.tableLayoutPanel5.RowCount = 1;
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 33F));
            this.tableLayoutPanel5.Size = new System.Drawing.Size(330, 33);
            this.tableLayoutPanel5.TabIndex = 7;
            // 
            // btnSaveParam
            // 
            this.btnSaveParam.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnSaveParam.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnSaveParam.BorderColor = System.Drawing.Color.Lavender;
            this.btnSaveParam.BorderRadius = 3;
            this.btnSaveParam.BorderSize = 2;
            this.btnSaveParam.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnSaveParam.FlatAppearance.BorderSize = 0;
            this.btnSaveParam.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnSaveParam.ForeColor = System.Drawing.Color.White;
            this.btnSaveParam.Location = new System.Drawing.Point(261, 1);
            this.btnSaveParam.Margin = new System.Windows.Forms.Padding(1);
            this.btnSaveParam.Name = "btnSaveParam";
            this.btnSaveParam.Size = new System.Drawing.Size(68, 31);
            this.btnSaveParam.TabIndex = 4;
            this.btnSaveParam.Text = "SAVE";
            this.btnSaveParam.TextColor = System.Drawing.Color.White;
            this.btnSaveParam.UseVisualStyleBackColor = false;
            this.btnSaveParam.Click += new System.EventHandler(this.btnSaveParam_Click);
            // 
            // btnLoadParam
            // 
            this.btnLoadParam.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnLoadParam.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnLoadParam.BorderColor = System.Drawing.Color.Lavender;
            this.btnLoadParam.BorderRadius = 3;
            this.btnLoadParam.BorderSize = 2;
            this.btnLoadParam.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnLoadParam.FlatAppearance.BorderSize = 0;
            this.btnLoadParam.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnLoadParam.ForeColor = System.Drawing.Color.White;
            this.btnLoadParam.Location = new System.Drawing.Point(191, 1);
            this.btnLoadParam.Margin = new System.Windows.Forms.Padding(1);
            this.btnLoadParam.Name = "btnLoadParam";
            this.btnLoadParam.Size = new System.Drawing.Size(68, 31);
            this.btnLoadParam.TabIndex = 3;
            this.btnLoadParam.Text = "LOAD";
            this.btnLoadParam.TextColor = System.Drawing.Color.White;
            this.btnLoadParam.UseVisualStyleBackColor = false;
            this.btnLoadParam.Click += new System.EventHandler(this.btnLoadParam_Click);
            // 
            // tableLayoutPanel8
            // 
            this.tableLayoutPanel8.ColumnCount = 1;
            this.tableLayoutPanel8.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel8.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel8.Controls.Add(this.dgvLotList, 0, 0);
            this.tableLayoutPanel8.Controls.Add(this.tableLayoutPanel9, 0, 1);
            this.tableLayoutPanel8.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel8.Location = new System.Drawing.Point(344, 0);
            this.tableLayoutPanel8.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel8.Name = "tableLayoutPanel8";
            this.tableLayoutPanel8.RowCount = 2;
            this.tableLayoutPanel8.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel8.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 53F));
            this.tableLayoutPanel8.Size = new System.Drawing.Size(294, 589);
            this.tableLayoutPanel8.TabIndex = 6;
            // 
            // dgvLotList
            // 
            this.dgvLotList.AllowUserToAddRows = false;
            this.dgvLotList.AllowUserToDeleteRows = false;
            this.dgvLotList.AllowUserToResizeColumns = false;
            this.dgvLotList.AllowUserToResizeRows = false;
            this.dgvLotList.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvLotList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgvLotList.Location = new System.Drawing.Point(1, 1);
            this.dgvLotList.Margin = new System.Windows.Forms.Padding(1);
            this.dgvLotList.Name = "dgvLotList";
            this.dgvLotList.RowTemplate.Height = 23;
            this.dgvLotList.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dgvLotList.Size = new System.Drawing.Size(292, 534);
            this.dgvLotList.TabIndex = 6;
            this.dgvLotList.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dgvLotList_CellEndEdit);
            // 
            // tableLayoutPanel9
            // 
            this.tableLayoutPanel9.ColumnCount = 3;
            this.tableLayoutPanel9.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.tableLayoutPanel9.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.tableLayoutPanel9.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.tableLayoutPanel9.Controls.Add(this.btnClear, 2, 0);
            this.tableLayoutPanel9.Controls.Add(this.btnDel, 1, 0);
            this.tableLayoutPanel9.Controls.Add(this.btnAdd, 0, 0);
            this.tableLayoutPanel9.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel9.Location = new System.Drawing.Point(3, 539);
            this.tableLayoutPanel9.Name = "tableLayoutPanel9";
            this.tableLayoutPanel9.RowCount = 1;
            this.tableLayoutPanel9.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel9.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 47F));
            this.tableLayoutPanel9.Size = new System.Drawing.Size(288, 47);
            this.tableLayoutPanel9.TabIndex = 3;
            // 
            // btnClear
            // 
            this.btnClear.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnClear.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnClear.BorderColor = System.Drawing.Color.Lavender;
            this.btnClear.BorderRadius = 1;
            this.btnClear.BorderSize = 2;
            this.btnClear.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnClear.FlatAppearance.BorderSize = 0;
            this.btnClear.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnClear.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.btnClear.ForeColor = System.Drawing.Color.White;
            this.btnClear.Location = new System.Drawing.Point(191, 1);
            this.btnClear.Margin = new System.Windows.Forms.Padding(1);
            this.btnClear.Name = "btnClear";
            this.btnClear.Size = new System.Drawing.Size(96, 45);
            this.btnClear.TabIndex = 7;
            this.btnClear.Text = "CLEAR";
            this.btnClear.TextColor = System.Drawing.Color.White;
            this.btnClear.UseVisualStyleBackColor = false;
            // 
            // btnDel
            // 
            this.btnDel.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnDel.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnDel.BorderColor = System.Drawing.Color.Lavender;
            this.btnDel.BorderRadius = 1;
            this.btnDel.BorderSize = 2;
            this.btnDel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnDel.FlatAppearance.BorderSize = 0;
            this.btnDel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnDel.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.btnDel.ForeColor = System.Drawing.Color.White;
            this.btnDel.Location = new System.Drawing.Point(96, 1);
            this.btnDel.Margin = new System.Windows.Forms.Padding(1);
            this.btnDel.Name = "btnDel";
            this.btnDel.Size = new System.Drawing.Size(93, 45);
            this.btnDel.TabIndex = 6;
            this.btnDel.Text = "DEL";
            this.btnDel.TextColor = System.Drawing.Color.White;
            this.btnDel.UseVisualStyleBackColor = false;
            // 
            // btnAdd
            // 
            this.btnAdd.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAdd.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAdd.BorderColor = System.Drawing.Color.Lavender;
            this.btnAdd.BorderRadius = 1;
            this.btnAdd.BorderSize = 2;
            this.btnAdd.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnAdd.FlatAppearance.BorderSize = 0;
            this.btnAdd.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnAdd.Font = new System.Drawing.Font("Segoe UI", 9F);
            this.btnAdd.ForeColor = System.Drawing.Color.White;
            this.btnAdd.Location = new System.Drawing.Point(1, 1);
            this.btnAdd.Margin = new System.Windows.Forms.Padding(1);
            this.btnAdd.Name = "btnAdd";
            this.btnAdd.Size = new System.Drawing.Size(93, 45);
            this.btnAdd.TabIndex = 5;
            this.btnAdd.Text = "ADD";
            this.btnAdd.TextColor = System.Drawing.Color.White;
            this.btnAdd.UseVisualStyleBackColor = false;
            // 
            // FormSelectedLot
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(644, 675);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "FormSelectedLot";
            this.Text = "FormCsv";
            this.VisibleChanged += new System.EventHandler(this.FormSelectedLot_VisibleChanged);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel3.ResumeLayout(false);
            this.tableLayoutPanel15.ResumeLayout(false);
            this.groupCompRange.ResumeLayout(false);
            this.tableLayoutPanel11.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dgvCompRange)).EndInit();
            this.tableLayoutPanel12.ResumeLayout(false);
            this.gpJudgeRange.ResumeLayout(false);
            this.tableLayoutPanel16.ResumeLayout(false);
            this.tableLayoutPanel16.PerformLayout();
            this.tableLayoutPanel19.ResumeLayout(false);
            this.tableLayoutPanel19.PerformLayout();
            this.gbFilterType.ResumeLayout(false);
            this.tableLayoutPanel4.ResumeLayout(false);
            this.tableLayoutPanel4.PerformLayout();
            this.tableLayoutPanel6.ResumeLayout(false);
            this.tableLayoutPanel6.PerformLayout();
            this.tableLayoutPanel7.ResumeLayout(false);
            this.tableLayoutPanel7.PerformLayout();
            this.tableLayoutPanel5.ResumeLayout(false);
            this.tableLayoutPanel8.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dgvLotList)).EndInit();
            this.tableLayoutPanel9.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private CustomControls.RoundLabel lblTitle;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private CustomControls.RoundButton btnCancel;
        private CustomControls.RoundButton btnOk;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private System.Windows.Forms.GroupBox groupCompRange;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel11;
        private System.Windows.Forms.DataGridView dgvCompRange;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel12;
        private CustomControls.RoundButton btnDelCompRange;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel15;
        private System.Windows.Forms.GroupBox gpJudgeRange;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel16;
        private CustomControls.RoundTextBox tbJudgeRangeX;
        private CustomControls.RoundLabel lblRangeX;
        private CustomControls.RoundLabel lblRangeY;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private CustomControls.RoundTextBox tbJudgeRangeY;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel19;
        private System.Windows.Forms.CheckBox cbUseMNTTAN;
        private System.Windows.Forms.GroupBox gbFilterType;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel5;
        private CustomControls.RoundButton btnSaveParam;
        private CustomControls.RoundButton btnLoadParam;
        private CustomControls.RoundButton btnAddCompRange;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel4;
        private System.Windows.Forms.RadioButton rbFilterType2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel6;
        private CustomControls.RoundButton btnAddUserFilter;
        private System.Windows.Forms.RadioButton rbFilterType1;
        private System.Windows.Forms.ListView lvFilterList;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel7;
        private System.Windows.Forms.ComboBox cbDest;
        private System.Windows.Forms.CheckBox ckbES;
        private System.Windows.Forms.CheckBox ckbTG;
        private System.Windows.Forms.CheckBox ckbETC;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel8;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel9;
        private CustomControls.RoundButton btnClear;
        private CustomControls.RoundButton btnDel;
        private CustomControls.RoundButton btnAdd;
        private System.Windows.Forms.DataGridView dgvLotList;
    }
}