namespace MarkCompare
{
    partial class FormCsv
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
            this.lblTitle = new CustomControls.ShadedLabel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.dgvCSVList = new System.Windows.Forms.DataGridView();
            this.tableLayoutPanel15 = new System.Windows.Forms.TableLayoutPanel();
            this.groupCompRange = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel11 = new System.Windows.Forms.TableLayoutPanel();
            this.dgvCompRange = new System.Windows.Forms.DataGridView();
            this.tableLayoutPanel12 = new System.Windows.Forms.TableLayoutPanel();
            this.gpJudgeRange = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel16 = new System.Windows.Forms.TableLayoutPanel();
            this.lblRangeY = new CustomControls.ShadedLabel();
            this.lblRangeX = new CustomControls.ShadedLabel();
            this.tbJudgeRangeX = new CustomControls.RoundTextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.tbJudgeRangeY = new CustomControls.RoundTextBox();
            this.tableLayoutPanel19 = new System.Windows.Forms.TableLayoutPanel();
            this.cbUseMNTTAN = new System.Windows.Forms.CheckBox();
            this.gbType = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel4 = new System.Windows.Forms.TableLayoutPanel();
            this.rbType1 = new System.Windows.Forms.RadioButton();
            this.rbType4 = new System.Windows.Forms.RadioButton();
            this.rbType2 = new System.Windows.Forms.RadioButton();
            this.rbType3 = new System.Windows.Forms.RadioButton();
            this.tableLayoutPanel5 = new System.Windows.Forms.TableLayoutPanel();
            this.btnAddCompRange = new CustomControls.ShadedButton();
            this.btnDelCompRange = new CustomControls.ShadedButton();
            this.btnLoadParam = new CustomControls.ShadedButton();
            this.btnSaveParam = new CustomControls.ShadedButton();
            this.btnAdd = new CustomControls.ShadedButton();
            this.btnDel = new CustomControls.ShadedButton();
            this.btnClear = new CustomControls.ShadedButton();
            this.btnSave = new CustomControls.ShadedButton();
            this.btnLoad = new CustomControls.ShadedButton();
            this.btnCancel = new CustomControls.ShadedButton();
            this.btnOk = new CustomControls.ShadedButton();
            this.tableLayoutPanel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.tableLayoutPanel3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvCSVList)).BeginInit();
            this.tableLayoutPanel15.SuspendLayout();
            this.groupCompRange.SuspendLayout();
            this.tableLayoutPanel11.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvCompRange)).BeginInit();
            this.tableLayoutPanel12.SuspendLayout();
            this.gpJudgeRange.SuspendLayout();
            this.tableLayoutPanel16.SuspendLayout();
            this.tableLayoutPanel19.SuspendLayout();
            this.gbType.SuspendLayout();
            this.tableLayoutPanel4.SuspendLayout();
            this.tableLayoutPanel5.SuspendLayout();
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
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 51F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(778, 610);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // lblTitle
            // 
            this.lblTitle.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.lblTitle.BackColor = System.Drawing.Color.Transparent;
            this.lblTitle.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.lblTitle.BorderRadius = 3;
            this.lblTitle.BorderSize = 1;
            this.lblTitle.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblTitle.FillColor = System.Drawing.Color.DarkSlateBlue;
            this.lblTitle.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblTitle.ImagePadding = 6;
            this.lblTitle.ImageSize = 18;
            this.lblTitle.LabelImage = null;
            this.lblTitle.Location = new System.Drawing.Point(1, 1);
            this.lblTitle.Margin = new System.Windows.Forms.Padding(1);
            this.lblTitle.Name = "lblTitle";
            this.lblTitle.ShowAccentLine = false;
            this.lblTitle.Size = new System.Drawing.Size(776, 28);
            this.lblTitle.TabIndex = 16;
            this.lblTitle.Text = "CSV";
            this.lblTitle.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblTitle.TextColor = System.Drawing.Color.White;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 8;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel2.Controls.Add(this.btnClear, 2, 0);
            this.tableLayoutPanel2.Controls.Add(this.btnSave, 3, 0);
            this.tableLayoutPanel2.Controls.Add(this.btnCancel, 6, 0);
            this.tableLayoutPanel2.Controls.Add(this.btnLoad, 4, 0);
            this.tableLayoutPanel2.Controls.Add(this.btnDel, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.btnAdd, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.btnOk, 7, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(0, 559);
            this.tableLayoutPanel2.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(778, 51);
            this.tableLayoutPanel2.TabIndex = 2;
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.ColumnCount = 2;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 40.54404F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 59.45596F));
            this.tableLayoutPanel3.Controls.Add(this.dgvCSVList, 1, 0);
            this.tableLayoutPanel3.Controls.Add(this.tableLayoutPanel15, 0, 0);
            this.tableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel3.Location = new System.Drawing.Point(3, 33);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 1;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(772, 523);
            this.tableLayoutPanel3.TabIndex = 10;
            // 
            // dgvCSVList
            // 
            this.dgvCSVList.AllowUserToAddRows = false;
            this.dgvCSVList.AllowUserToDeleteRows = false;
            this.dgvCSVList.AllowUserToResizeColumns = false;
            this.dgvCSVList.AllowUserToResizeRows = false;
            this.dgvCSVList.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvCSVList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgvCSVList.Location = new System.Drawing.Point(316, 3);
            this.dgvCSVList.Name = "dgvCSVList";
            this.dgvCSVList.RowTemplate.Height = 23;
            this.dgvCSVList.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.dgvCSVList.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dgvCSVList.Size = new System.Drawing.Size(453, 517);
            this.dgvCSVList.TabIndex = 1;
            this.dgvCSVList.RowPostPaint += new System.Windows.Forms.DataGridViewRowPostPaintEventHandler(this.dataGridView1_RowPostPaint);
            // 
            // tableLayoutPanel15
            // 
            this.tableLayoutPanel15.ColumnCount = 1;
            this.tableLayoutPanel15.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel15.Controls.Add(this.groupCompRange, 0, 1);
            this.tableLayoutPanel15.Controls.Add(this.gpJudgeRange, 0, 2);
            this.tableLayoutPanel15.Controls.Add(this.tableLayoutPanel19, 0, 3);
            this.tableLayoutPanel15.Controls.Add(this.gbType, 0, 0);
            this.tableLayoutPanel15.Controls.Add(this.tableLayoutPanel5, 0, 4);
            this.tableLayoutPanel15.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel15.Location = new System.Drawing.Point(2, 2);
            this.tableLayoutPanel15.Margin = new System.Windows.Forms.Padding(2);
            this.tableLayoutPanel15.Name = "tableLayoutPanel15";
            this.tableLayoutPanel15.Padding = new System.Windows.Forms.Padding(2);
            this.tableLayoutPanel15.RowCount = 5;
            this.tableLayoutPanel15.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 52F));
            this.tableLayoutPanel15.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel15.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 62F));
            this.tableLayoutPanel15.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel15.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 45F));
            this.tableLayoutPanel15.Size = new System.Drawing.Size(309, 519);
            this.tableLayoutPanel15.TabIndex = 5;
            // 
            // groupCompRange
            // 
            this.groupCompRange.Controls.Add(this.tableLayoutPanel11);
            this.groupCompRange.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupCompRange.Location = new System.Drawing.Point(4, 56);
            this.groupCompRange.Margin = new System.Windows.Forms.Padding(2);
            this.groupCompRange.Name = "groupCompRange";
            this.groupCompRange.Padding = new System.Windows.Forms.Padding(1, 1, 1, 4);
            this.groupCompRange.Size = new System.Drawing.Size(301, 332);
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
            this.tableLayoutPanel11.Location = new System.Drawing.Point(1, 15);
            this.tableLayoutPanel11.Margin = new System.Windows.Forms.Padding(2);
            this.tableLayoutPanel11.Name = "tableLayoutPanel11";
            this.tableLayoutPanel11.Padding = new System.Windows.Forms.Padding(2);
            this.tableLayoutPanel11.RowCount = 2;
            this.tableLayoutPanel11.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel11.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel11.Size = new System.Drawing.Size(299, 313);
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
            this.dgvCompRange.Size = new System.Drawing.Size(293, 272);
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
            this.tableLayoutPanel12.Location = new System.Drawing.Point(2, 276);
            this.tableLayoutPanel12.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel12.Name = "tableLayoutPanel12";
            this.tableLayoutPanel12.RowCount = 1;
            this.tableLayoutPanel12.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel12.Size = new System.Drawing.Size(295, 35);
            this.tableLayoutPanel12.TabIndex = 0;
            // 
            // gpJudgeRange
            // 
            this.gpJudgeRange.Controls.Add(this.tableLayoutPanel16);
            this.gpJudgeRange.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gpJudgeRange.Location = new System.Drawing.Point(4, 392);
            this.gpJudgeRange.Margin = new System.Windows.Forms.Padding(2);
            this.gpJudgeRange.Name = "gpJudgeRange";
            this.gpJudgeRange.Padding = new System.Windows.Forms.Padding(2);
            this.gpJudgeRange.Size = new System.Drawing.Size(301, 58);
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
            this.tableLayoutPanel16.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 38F));
            this.tableLayoutPanel16.Controls.Add(this.lblRangeY, 3, 0);
            this.tableLayoutPanel16.Controls.Add(this.lblRangeX, 0, 0);
            this.tableLayoutPanel16.Controls.Add(this.tbJudgeRangeX, 1, 0);
            this.tableLayoutPanel16.Controls.Add(this.label1, 2, 0);
            this.tableLayoutPanel16.Controls.Add(this.label2, 5, 0);
            this.tableLayoutPanel16.Controls.Add(this.tbJudgeRangeY, 4, 0);
            this.tableLayoutPanel16.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel16.Location = new System.Drawing.Point(2, 16);
            this.tableLayoutPanel16.Name = "tableLayoutPanel16";
            this.tableLayoutPanel16.Padding = new System.Windows.Forms.Padding(2);
            this.tableLayoutPanel16.RowCount = 1;
            this.tableLayoutPanel16.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel16.Size = new System.Drawing.Size(297, 40);
            this.tableLayoutPanel16.TabIndex = 0;
            // 
            // lblRangeY
            // 
            this.lblRangeY.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.lblRangeY.BackColor = System.Drawing.Color.Transparent;
            this.lblRangeY.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.lblRangeY.BorderRadius = 5;
            this.lblRangeY.BorderSize = 1;
            this.lblRangeY.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblRangeY.FillColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.lblRangeY.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblRangeY.ImagePadding = 6;
            this.lblRangeY.ImageSize = 18;
            this.lblRangeY.LabelImage = null;
            this.lblRangeY.Location = new System.Drawing.Point(148, 3);
            this.lblRangeY.Margin = new System.Windows.Forms.Padding(1);
            this.lblRangeY.Name = "lblRangeY";
            this.lblRangeY.ShowAccentLine = false;
            this.lblRangeY.Size = new System.Drawing.Size(33, 34);
            this.lblRangeY.TabIndex = 10;
            this.lblRangeY.Text = "Y";
            this.lblRangeY.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblRangeY.TextColor = System.Drawing.Color.White;
            // 
            // lblRangeX
            // 
            this.lblRangeX.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.lblRangeX.BackColor = System.Drawing.Color.Transparent;
            this.lblRangeX.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.lblRangeX.BorderRadius = 5;
            this.lblRangeX.BorderSize = 1;
            this.lblRangeX.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblRangeX.FillColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.lblRangeX.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblRangeX.ImagePadding = 6;
            this.lblRangeX.ImageSize = 18;
            this.lblRangeX.LabelImage = null;
            this.lblRangeX.Location = new System.Drawing.Point(3, 3);
            this.lblRangeX.Margin = new System.Windows.Forms.Padding(1);
            this.lblRangeX.Name = "lblRangeX";
            this.lblRangeX.ShowAccentLine = false;
            this.lblRangeX.Size = new System.Drawing.Size(33, 34);
            this.lblRangeX.TabIndex = 8;
            this.lblRangeX.Text = "X";
            this.lblRangeX.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblRangeX.TextColor = System.Drawing.Color.White;
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
            this.tbJudgeRangeX.Padding = new System.Windows.Forms.Padding(10, 7, 10, 7);
            this.tbJudgeRangeX.PasswordChar = false;
            this.tbJudgeRangeX.PlaceholderColor = System.Drawing.Color.DarkGray;
            this.tbJudgeRangeX.PlaceholderText = "";
            this.tbJudgeRangeX.Size = new System.Drawing.Size(73, 32);
            this.tbJudgeRangeX.TabIndex = 6;
            this.tbJudgeRangeX.Texts = "";
            this.tbJudgeRangeX.UnderlinedStyle = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.label1.ImageAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.label1.Location = new System.Drawing.Point(114, 26);
            this.label1.Margin = new System.Windows.Forms.Padding(2, 0, 0, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(33, 12);
            this.label1.TabIndex = 4;
            this.label1.Text = "mm";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.label2.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.label2.Location = new System.Drawing.Point(259, 26);
            this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 0, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(36, 12);
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
            this.tbJudgeRangeY.Location = new System.Drawing.Point(183, 3);
            this.tbJudgeRangeY.Margin = new System.Windows.Forms.Padding(1);
            this.tbJudgeRangeY.Multiline = false;
            this.tbJudgeRangeY.Name = "tbJudgeRangeY";
            this.tbJudgeRangeY.Padding = new System.Windows.Forms.Padding(10, 7, 10, 7);
            this.tbJudgeRangeY.PasswordChar = false;
            this.tbJudgeRangeY.PlaceholderColor = System.Drawing.Color.DarkGray;
            this.tbJudgeRangeY.PlaceholderText = "";
            this.tbJudgeRangeY.Size = new System.Drawing.Size(73, 32);
            this.tbJudgeRangeY.TabIndex = 6;
            this.tbJudgeRangeY.Texts = "";
            this.tbJudgeRangeY.UnderlinedStyle = false;
            // 
            // tableLayoutPanel19
            // 
            this.tableLayoutPanel19.ColumnCount = 2;
            this.tableLayoutPanel19.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 164F));
            this.tableLayoutPanel19.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 145F));
            this.tableLayoutPanel19.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel19.Controls.Add(this.cbUseMNTTAN, 0, 0);
            this.tableLayoutPanel19.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel19.Location = new System.Drawing.Point(2, 452);
            this.tableLayoutPanel19.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel19.Name = "tableLayoutPanel19";
            this.tableLayoutPanel19.RowCount = 1;
            this.tableLayoutPanel19.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel19.Size = new System.Drawing.Size(305, 20);
            this.tableLayoutPanel19.TabIndex = 5;
            // 
            // cbUseMNTTAN
            // 
            this.cbUseMNTTAN.AutoSize = true;
            this.cbUseMNTTAN.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cbUseMNTTAN.Location = new System.Drawing.Point(3, 3);
            this.cbUseMNTTAN.Name = "cbUseMNTTAN";
            this.cbUseMNTTAN.Size = new System.Drawing.Size(158, 14);
            this.cbUseMNTTAN.TabIndex = 10;
            this.cbUseMNTTAN.Text = "USE AI RESULT";
            this.cbUseMNTTAN.UseVisualStyleBackColor = true;
            // 
            // gbType
            // 
            this.gbType.Controls.Add(this.tableLayoutPanel4);
            this.gbType.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbType.Location = new System.Drawing.Point(5, 5);
            this.gbType.Name = "gbType";
            this.gbType.Size = new System.Drawing.Size(299, 46);
            this.gbType.TabIndex = 6;
            this.gbType.TabStop = false;
            this.gbType.Text = "Compare Type";
            // 
            // tableLayoutPanel4
            // 
            this.tableLayoutPanel4.ColumnCount = 4;
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 55F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 55F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 55F));
            this.tableLayoutPanel4.Controls.Add(this.rbType1, 0, 0);
            this.tableLayoutPanel4.Controls.Add(this.rbType4, 3, 0);
            this.tableLayoutPanel4.Controls.Add(this.rbType2, 1, 0);
            this.tableLayoutPanel4.Controls.Add(this.rbType3, 2, 0);
            this.tableLayoutPanel4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel4.Location = new System.Drawing.Point(3, 17);
            this.tableLayoutPanel4.Name = "tableLayoutPanel4";
            this.tableLayoutPanel4.RowCount = 1;
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel4.Size = new System.Drawing.Size(293, 26);
            this.tableLayoutPanel4.TabIndex = 1;
            // 
            // rbType1
            // 
            this.rbType1.AutoSize = true;
            this.rbType1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rbType1.Location = new System.Drawing.Point(3, 3);
            this.rbType1.Name = "rbType1";
            this.rbType1.Size = new System.Drawing.Size(122, 20);
            this.rbType1.TabIndex = 0;
            this.rbType1.TabStop = true;
            this.rbType1.Text = "multi";
            this.rbType1.UseVisualStyleBackColor = true;
            // 
            // rbType4
            // 
            this.rbType4.AutoSize = true;
            this.rbType4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rbType4.Location = new System.Drawing.Point(241, 3);
            this.rbType4.Name = "rbType4";
            this.rbType4.Size = new System.Drawing.Size(49, 20);
            this.rbType4.TabIndex = 0;
            this.rbType4.TabStop = true;
            this.rbType4.Text = "ETC";
            this.rbType4.UseVisualStyleBackColor = true;
            // 
            // rbType2
            // 
            this.rbType2.AutoSize = true;
            this.rbType2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rbType2.Location = new System.Drawing.Point(131, 3);
            this.rbType2.Name = "rbType2";
            this.rbType2.Size = new System.Drawing.Size(49, 20);
            this.rbType2.TabIndex = 0;
            this.rbType2.TabStop = true;
            this.rbType2.Text = "ES";
            this.rbType2.UseVisualStyleBackColor = true;
            // 
            // rbType3
            // 
            this.rbType3.AutoSize = true;
            this.rbType3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rbType3.Location = new System.Drawing.Point(186, 3);
            this.rbType3.Name = "rbType3";
            this.rbType3.Size = new System.Drawing.Size(49, 20);
            this.rbType3.TabIndex = 0;
            this.rbType3.TabStop = true;
            this.rbType3.Text = "TG";
            this.rbType3.UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanel5
            // 
            this.tableLayoutPanel5.ColumnCount = 3;
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 70F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 70F));
            this.tableLayoutPanel5.Controls.Add(this.btnLoadParam, 1, 0);
            this.tableLayoutPanel5.Controls.Add(this.btnSaveParam, 2, 0);
            this.tableLayoutPanel5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel5.Location = new System.Drawing.Point(5, 475);
            this.tableLayoutPanel5.Name = "tableLayoutPanel5";
            this.tableLayoutPanel5.RowCount = 1;
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel5.Size = new System.Drawing.Size(299, 39);
            this.tableLayoutPanel5.TabIndex = 7;
            // 
            // btnAddCompRange
            // 
            this.btnAddCompRange.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnAddCompRange.BackColor = System.Drawing.Color.Transparent;
            this.btnAddCompRange.BorderColor = System.Drawing.Color.SlateGray;
            this.btnAddCompRange.BorderRadius = 5;
            this.btnAddCompRange.BorderSize = 2;
            this.btnAddCompRange.ButtonImage = null;
            this.btnAddCompRange.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnAddCompRange.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnAddCompRange.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnAddCompRange.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnAddCompRange.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnAddCompRange.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnAddCompRange.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnAddCompRange.IconPadding = 6;
            this.btnAddCompRange.IconSize = 20;
            this.btnAddCompRange.Location = new System.Drawing.Point(176, 1);
            this.btnAddCompRange.Margin = new System.Windows.Forms.Padding(1);
            this.btnAddCompRange.Name = "btnAddCompRange";
            this.btnAddCompRange.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAddCompRange.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnAddCompRange.ShowAccentLine = false;
            this.btnAddCompRange.Size = new System.Drawing.Size(58, 33);
            this.btnAddCompRange.TabIndex = 14;
            this.btnAddCompRange.Text = "ADD";
            this.btnAddCompRange.TextColor = System.Drawing.Color.White;
            this.btnAddCompRange.TextOffsetX = 0;
            this.btnAddCompRange.TextOffsetY = 0;
            this.btnAddCompRange.Click += new System.EventHandler(this.btnAddCompRange_Click);
            // 
            // btnDelCompRange
            // 
            this.btnDelCompRange.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnDelCompRange.BackColor = System.Drawing.Color.Transparent;
            this.btnDelCompRange.BorderColor = System.Drawing.Color.SlateGray;
            this.btnDelCompRange.BorderRadius = 5;
            this.btnDelCompRange.BorderSize = 2;
            this.btnDelCompRange.ButtonImage = null;
            this.btnDelCompRange.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnDelCompRange.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnDelCompRange.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnDelCompRange.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnDelCompRange.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnDelCompRange.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnDelCompRange.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnDelCompRange.IconPadding = 6;
            this.btnDelCompRange.IconSize = 20;
            this.btnDelCompRange.Location = new System.Drawing.Point(236, 1);
            this.btnDelCompRange.Margin = new System.Windows.Forms.Padding(1);
            this.btnDelCompRange.Name = "btnDelCompRange";
            this.btnDelCompRange.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnDelCompRange.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnDelCompRange.ShowAccentLine = false;
            this.btnDelCompRange.Size = new System.Drawing.Size(58, 33);
            this.btnDelCompRange.TabIndex = 14;
            this.btnDelCompRange.Text = "DEL";
            this.btnDelCompRange.TextColor = System.Drawing.Color.White;
            this.btnDelCompRange.TextOffsetX = 0;
            this.btnDelCompRange.TextOffsetY = 0;
            this.btnDelCompRange.Click += new System.EventHandler(this.btnDelCompRange_Click);
            // 
            // btnLoadParam
            // 
            this.btnLoadParam.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnLoadParam.BackColor = System.Drawing.Color.Transparent;
            this.btnLoadParam.BorderColor = System.Drawing.Color.SlateGray;
            this.btnLoadParam.BorderRadius = 5;
            this.btnLoadParam.BorderSize = 2;
            this.btnLoadParam.ButtonImage = null;
            this.btnLoadParam.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnLoadParam.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnLoadParam.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnLoadParam.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnLoadParam.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnLoadParam.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnLoadParam.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnLoadParam.IconPadding = 6;
            this.btnLoadParam.IconSize = 20;
            this.btnLoadParam.Location = new System.Drawing.Point(160, 1);
            this.btnLoadParam.Margin = new System.Windows.Forms.Padding(1);
            this.btnLoadParam.Name = "btnLoadParam";
            this.btnLoadParam.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnLoadParam.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnLoadParam.ShowAccentLine = false;
            this.btnLoadParam.Size = new System.Drawing.Size(68, 37);
            this.btnLoadParam.TabIndex = 14;
            this.btnLoadParam.Text = "LOAD";
            this.btnLoadParam.TextColor = System.Drawing.Color.White;
            this.btnLoadParam.TextOffsetX = 0;
            this.btnLoadParam.TextOffsetY = 0;
            this.btnLoadParam.Click += new System.EventHandler(this.btnLoadParam_Click);
            // 
            // btnSaveParam
            // 
            this.btnSaveParam.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnSaveParam.BackColor = System.Drawing.Color.Transparent;
            this.btnSaveParam.BorderColor = System.Drawing.Color.SlateGray;
            this.btnSaveParam.BorderRadius = 5;
            this.btnSaveParam.BorderSize = 2;
            this.btnSaveParam.ButtonImage = null;
            this.btnSaveParam.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnSaveParam.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnSaveParam.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnSaveParam.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnSaveParam.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSaveParam.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnSaveParam.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnSaveParam.IconPadding = 6;
            this.btnSaveParam.IconSize = 20;
            this.btnSaveParam.Location = new System.Drawing.Point(230, 1);
            this.btnSaveParam.Margin = new System.Windows.Forms.Padding(1);
            this.btnSaveParam.Name = "btnSaveParam";
            this.btnSaveParam.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnSaveParam.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnSaveParam.ShowAccentLine = false;
            this.btnSaveParam.Size = new System.Drawing.Size(68, 37);
            this.btnSaveParam.TabIndex = 14;
            this.btnSaveParam.Text = "SAVE";
            this.btnSaveParam.TextColor = System.Drawing.Color.White;
            this.btnSaveParam.TextOffsetX = 0;
            this.btnSaveParam.TextOffsetY = 0;
            this.btnSaveParam.Click += new System.EventHandler(this.btnSaveParam_Click);
            // 
            // btnAdd
            // 
            this.btnAdd.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnAdd.BackColor = System.Drawing.Color.Transparent;
            this.btnAdd.BorderColor = System.Drawing.Color.SlateGray;
            this.btnAdd.BorderRadius = 5;
            this.btnAdd.BorderSize = 2;
            this.btnAdd.ButtonImage = null;
            this.btnAdd.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnAdd.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnAdd.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnAdd.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnAdd.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnAdd.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnAdd.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnAdd.IconPadding = 6;
            this.btnAdd.IconSize = 20;
            this.btnAdd.Location = new System.Drawing.Point(1, 1);
            this.btnAdd.Margin = new System.Windows.Forms.Padding(1);
            this.btnAdd.Name = "btnAdd";
            this.btnAdd.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAdd.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnAdd.ShowAccentLine = false;
            this.btnAdd.Size = new System.Drawing.Size(98, 49);
            this.btnAdd.TabIndex = 14;
            this.btnAdd.Text = "ADD";
            this.btnAdd.TextColor = System.Drawing.Color.White;
            this.btnAdd.TextOffsetX = 0;
            this.btnAdd.TextOffsetY = 0;
            this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
            // 
            // btnDel
            // 
            this.btnDel.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnDel.BackColor = System.Drawing.Color.Transparent;
            this.btnDel.BorderColor = System.Drawing.Color.SlateGray;
            this.btnDel.BorderRadius = 5;
            this.btnDel.BorderSize = 2;
            this.btnDel.ButtonImage = null;
            this.btnDel.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnDel.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnDel.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnDel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnDel.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnDel.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnDel.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnDel.IconPadding = 6;
            this.btnDel.IconSize = 20;
            this.btnDel.Location = new System.Drawing.Point(101, 1);
            this.btnDel.Margin = new System.Windows.Forms.Padding(1);
            this.btnDel.Name = "btnDel";
            this.btnDel.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnDel.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnDel.ShowAccentLine = false;
            this.btnDel.Size = new System.Drawing.Size(98, 49);
            this.btnDel.TabIndex = 14;
            this.btnDel.Text = "DELETE";
            this.btnDel.TextColor = System.Drawing.Color.White;
            this.btnDel.TextOffsetX = 0;
            this.btnDel.TextOffsetY = 0;
            this.btnDel.Click += new System.EventHandler(this.btnDel_Click);
            // 
            // btnClear
            // 
            this.btnClear.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnClear.BackColor = System.Drawing.Color.Transparent;
            this.btnClear.BorderColor = System.Drawing.Color.SlateGray;
            this.btnClear.BorderRadius = 5;
            this.btnClear.BorderSize = 2;
            this.btnClear.ButtonImage = null;
            this.btnClear.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnClear.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnClear.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnClear.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnClear.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnClear.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnClear.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnClear.IconPadding = 6;
            this.btnClear.IconSize = 20;
            this.btnClear.Location = new System.Drawing.Point(201, 1);
            this.btnClear.Margin = new System.Windows.Forms.Padding(1);
            this.btnClear.Name = "btnClear";
            this.btnClear.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnClear.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnClear.ShowAccentLine = false;
            this.btnClear.Size = new System.Drawing.Size(98, 49);
            this.btnClear.TabIndex = 14;
            this.btnClear.Text = "CLEAR";
            this.btnClear.TextColor = System.Drawing.Color.White;
            this.btnClear.TextOffsetX = 0;
            this.btnClear.TextOffsetY = 0;
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);
            // 
            // btnSave
            // 
            this.btnSave.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnSave.BackColor = System.Drawing.Color.Transparent;
            this.btnSave.BorderColor = System.Drawing.Color.SlateGray;
            this.btnSave.BorderRadius = 5;
            this.btnSave.BorderSize = 2;
            this.btnSave.ButtonImage = null;
            this.btnSave.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnSave.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnSave.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnSave.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnSave.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSave.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnSave.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnSave.IconPadding = 6;
            this.btnSave.IconSize = 20;
            this.btnSave.Location = new System.Drawing.Point(301, 1);
            this.btnSave.Margin = new System.Windows.Forms.Padding(1);
            this.btnSave.Name = "btnSave";
            this.btnSave.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnSave.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnSave.ShowAccentLine = false;
            this.btnSave.Size = new System.Drawing.Size(98, 49);
            this.btnSave.TabIndex = 14;
            this.btnSave.Text = "SAVE";
            this.btnSave.TextColor = System.Drawing.Color.White;
            this.btnSave.TextOffsetX = 0;
            this.btnSave.TextOffsetY = 0;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // btnLoad
            // 
            this.btnLoad.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnLoad.BackColor = System.Drawing.Color.Transparent;
            this.btnLoad.BorderColor = System.Drawing.Color.SlateGray;
            this.btnLoad.BorderRadius = 5;
            this.btnLoad.BorderSize = 2;
            this.btnLoad.ButtonImage = null;
            this.btnLoad.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnLoad.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnLoad.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnLoad.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnLoad.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnLoad.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnLoad.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnLoad.IconPadding = 6;
            this.btnLoad.IconSize = 20;
            this.btnLoad.Location = new System.Drawing.Point(401, 1);
            this.btnLoad.Margin = new System.Windows.Forms.Padding(1);
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnLoad.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnLoad.ShowAccentLine = false;
            this.btnLoad.Size = new System.Drawing.Size(98, 49);
            this.btnLoad.TabIndex = 14;
            this.btnLoad.Text = "LOAD";
            this.btnLoad.TextColor = System.Drawing.Color.White;
            this.btnLoad.TextOffsetX = 0;
            this.btnLoad.TextOffsetY = 0;
            this.btnLoad.Click += new System.EventHandler(this.btnLoad_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnCancel.BackColor = System.Drawing.Color.Transparent;
            this.btnCancel.BorderColor = System.Drawing.Color.SlateGray;
            this.btnCancel.BorderRadius = 5;
            this.btnCancel.BorderSize = 2;
            this.btnCancel.ButtonImage = null;
            this.btnCancel.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnCancel.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnCancel.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnCancel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnCancel.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnCancel.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnCancel.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnCancel.IconPadding = 6;
            this.btnCancel.IconSize = 20;
            this.btnCancel.Location = new System.Drawing.Point(579, 1);
            this.btnCancel.Margin = new System.Windows.Forms.Padding(1);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnCancel.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnCancel.ShowAccentLine = false;
            this.btnCancel.Size = new System.Drawing.Size(98, 49);
            this.btnCancel.TabIndex = 14;
            this.btnCancel.Text = "CANCEL";
            this.btnCancel.TextColor = System.Drawing.Color.White;
            this.btnCancel.TextOffsetX = 0;
            this.btnCancel.TextOffsetY = 0;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // btnOk
            // 
            this.btnOk.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnOk.BackColor = System.Drawing.Color.Transparent;
            this.btnOk.BorderColor = System.Drawing.Color.SlateGray;
            this.btnOk.BorderRadius = 5;
            this.btnOk.BorderSize = 2;
            this.btnOk.ButtonImage = null;
            this.btnOk.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnOk.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnOk.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnOk.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnOk.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnOk.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnOk.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnOk.IconPadding = 6;
            this.btnOk.IconSize = 20;
            this.btnOk.Location = new System.Drawing.Point(679, 1);
            this.btnOk.Margin = new System.Windows.Forms.Padding(1);
            this.btnOk.Name = "btnOk";
            this.btnOk.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnOk.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnOk.ShowAccentLine = false;
            this.btnOk.Size = new System.Drawing.Size(98, 49);
            this.btnOk.TabIndex = 14;
            this.btnOk.Text = "OK";
            this.btnOk.TextColor = System.Drawing.Color.White;
            this.btnOk.TextOffsetX = 0;
            this.btnOk.TextOffsetY = 0;
            this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // FormCsv
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(778, 610);
            this.Controls.Add(this.tableLayoutPanel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "FormCsv";
            this.Text = "FormCsv";
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel3.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dgvCSVList)).EndInit();
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
            this.gbType.ResumeLayout(false);
            this.tableLayoutPanel4.ResumeLayout(false);
            this.tableLayoutPanel4.PerformLayout();
            this.tableLayoutPanel5.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.DataGridView dgvCSVList;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private System.Windows.Forms.GroupBox groupCompRange;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel11;
        private System.Windows.Forms.DataGridView dgvCompRange;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel12;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel15;
        private System.Windows.Forms.GroupBox gpJudgeRange;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel16;
        private CustomControls.RoundTextBox tbJudgeRangeX;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private CustomControls.RoundTextBox tbJudgeRangeY;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel19;
        private System.Windows.Forms.CheckBox cbUseMNTTAN;
        private System.Windows.Forms.GroupBox gbType;
        private System.Windows.Forms.RadioButton rbType1;
        private System.Windows.Forms.RadioButton rbType4;
        private System.Windows.Forms.RadioButton rbType3;
        private System.Windows.Forms.RadioButton rbType2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel4;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel5;
        private CustomControls.ShadedLabel lblTitle;
        private CustomControls.ShadedLabel lblRangeX;
        private CustomControls.ShadedLabel lblRangeY;
        private CustomControls.ShadedButton btnAdd;
        private CustomControls.ShadedButton btnAddCompRange;
        private CustomControls.ShadedButton btnDelCompRange;
        private CustomControls.ShadedButton btnLoadParam;
        private CustomControls.ShadedButton btnSaveParam;
        private CustomControls.ShadedButton btnClear;
        private CustomControls.ShadedButton btnSave;
        private CustomControls.ShadedButton btnCancel;
        private CustomControls.ShadedButton btnLoad;
        private CustomControls.ShadedButton btnDel;
        private CustomControls.ShadedButton btnOk;
    }
}