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
            this.lblTitle = new CustomControls.ShadedLabel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
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
            this.cbAiMonitoring = new System.Windows.Forms.CheckBox();
            this.cbUseMNTTAN = new System.Windows.Forms.CheckBox();
            this.cbUseSplit = new System.Windows.Forms.CheckBox();
            this.gbFilterType = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel4 = new System.Windows.Forms.TableLayoutPanel();
            this.lvFilterList = new System.Windows.Forms.ListView();
            this.rbFilterType2 = new System.Windows.Forms.RadioButton();
            this.tableLayoutPanel6 = new System.Windows.Forms.TableLayoutPanel();
            this.rbFilterType1 = new System.Windows.Forms.RadioButton();
            this.tableLayoutPanel7 = new System.Windows.Forms.TableLayoutPanel();
            this.cbbAiMonitorParam = new System.Windows.Forms.ComboBox();
            this.cbDest = new System.Windows.Forms.ComboBox();
            this.ckbES = new System.Windows.Forms.CheckBox();
            this.ckbTG = new System.Windows.Forms.CheckBox();
            this.ckbETC = new System.Windows.Forms.CheckBox();
            this.lblDBAiMonitorName = new CustomControls.ShadedLabel();
            this.tableLayoutPanel5 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel8 = new System.Windows.Forms.TableLayoutPanel();
            this.dgvLotList = new System.Windows.Forms.DataGridView();
            this.tableLayoutPanel9 = new System.Windows.Forms.TableLayoutPanel();
            this.btnLoadParam = new CustomControls.ShadedButton();
            this.btnSaveParam = new CustomControls.ShadedButton();
            this.btnAddUserFilter = new CustomControls.ShadedButton();
            this.btnAddCompRange = new CustomControls.ShadedButton();
            this.btnDelCompRange = new CustomControls.ShadedButton();
            this.btnAddList = new CustomControls.ShadedButton();
            this.btnAdd = new CustomControls.ShadedButton();
            this.btnDel = new CustomControls.ShadedButton();
            this.btnClear = new CustomControls.ShadedButton();
            this.btnCancel = new CustomControls.ShadedButton();
            this.btnOk = new CustomControls.ShadedButton();
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
            this.tableLayoutPanel1.Size = new System.Drawing.Size(694, 815);
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
            this.lblTitle.Size = new System.Drawing.Size(692, 28);
            this.lblTitle.TabIndex = 14;
            this.lblTitle.Text = "LOT 지정 검사 설정";
            this.lblTitle.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblTitle.TextColor = System.Drawing.Color.White;
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
            this.tableLayoutPanel2.Controls.Add(this.btnCancel, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.btnOk, 2, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(0, 767);
            this.tableLayoutPanel2.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(694, 48);
            this.tableLayoutPanel2.TabIndex = 2;
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.ColumnCount = 2;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50.2907F));
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 49.7093F));
            this.tableLayoutPanel3.Controls.Add(this.tableLayoutPanel15, 0, 0);
            this.tableLayoutPanel3.Controls.Add(this.tableLayoutPanel8, 1, 0);
            this.tableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel3.Location = new System.Drawing.Point(3, 34);
            this.tableLayoutPanel3.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 1;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 589F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(688, 729);
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
            this.tableLayoutPanel15.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 268F));
            this.tableLayoutPanel15.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel15.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 66F));
            this.tableLayoutPanel15.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 62F));
            this.tableLayoutPanel15.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 41F));
            this.tableLayoutPanel15.Size = new System.Drawing.Size(342, 725);
            this.tableLayoutPanel15.TabIndex = 5;
            // 
            // groupCompRange
            // 
            this.groupCompRange.Controls.Add(this.tableLayoutPanel11);
            this.groupCompRange.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupCompRange.Location = new System.Drawing.Point(4, 272);
            this.groupCompRange.Margin = new System.Windows.Forms.Padding(2);
            this.groupCompRange.Name = "groupCompRange";
            this.groupCompRange.Padding = new System.Windows.Forms.Padding(1, 1, 1, 5);
            this.groupCompRange.Size = new System.Drawing.Size(334, 280);
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
            this.tableLayoutPanel11.Size = new System.Drawing.Size(332, 258);
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
            this.dgvCompRange.Size = new System.Drawing.Size(326, 220);
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
            this.tableLayoutPanel12.Location = new System.Drawing.Point(2, 224);
            this.tableLayoutPanel12.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel12.Name = "tableLayoutPanel12";
            this.tableLayoutPanel12.RowCount = 1;
            this.tableLayoutPanel12.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel12.Size = new System.Drawing.Size(328, 32);
            this.tableLayoutPanel12.TabIndex = 0;
            // 
            // gpJudgeRange
            // 
            this.gpJudgeRange.Controls.Add(this.tableLayoutPanel16);
            this.gpJudgeRange.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gpJudgeRange.Location = new System.Drawing.Point(4, 556);
            this.gpJudgeRange.Margin = new System.Windows.Forms.Padding(2);
            this.gpJudgeRange.Name = "gpJudgeRange";
            this.gpJudgeRange.Padding = new System.Windows.Forms.Padding(2);
            this.gpJudgeRange.Size = new System.Drawing.Size(334, 62);
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
            this.tableLayoutPanel16.Controls.Add(this.lblRangeY, 3, 0);
            this.tableLayoutPanel16.Controls.Add(this.lblRangeX, 0, 0);
            this.tableLayoutPanel16.Controls.Add(this.tbJudgeRangeX, 1, 0);
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
            this.tableLayoutPanel16.Size = new System.Drawing.Size(330, 42);
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
            this.lblRangeY.Location = new System.Drawing.Point(164, 3);
            this.lblRangeY.Margin = new System.Windows.Forms.Padding(1);
            this.lblRangeY.Name = "lblRangeY";
            this.lblRangeY.ShowAccentLine = false;
            this.lblRangeY.Size = new System.Drawing.Size(33, 36);
            this.lblRangeY.TabIndex = 9;
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
            this.lblRangeX.Size = new System.Drawing.Size(33, 36);
            this.lblRangeX.TabIndex = 7;
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
            this.tbJudgeRangeX.Padding = new System.Windows.Forms.Padding(10, 9, 10, 9);
            this.tbJudgeRangeX.PasswordChar = false;
            this.tbJudgeRangeX.PlaceholderColor = System.Drawing.Color.DarkGray;
            this.tbJudgeRangeX.PlaceholderText = "";
            this.tbJudgeRangeX.Size = new System.Drawing.Size(89, 36);
            this.tbJudgeRangeX.TabIndex = 6;
            this.tbJudgeRangeX.Texts = "";
            this.tbJudgeRangeX.UnderlinedStyle = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.label1.ImageAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.label1.Location = new System.Drawing.Point(130, 25);
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
            this.label2.Location = new System.Drawing.Point(291, 25);
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
            this.tbJudgeRangeY.Location = new System.Drawing.Point(199, 3);
            this.tbJudgeRangeY.Margin = new System.Windows.Forms.Padding(1);
            this.tbJudgeRangeY.Multiline = false;
            this.tbJudgeRangeY.Name = "tbJudgeRangeY";
            this.tbJudgeRangeY.Padding = new System.Windows.Forms.Padding(10, 9, 10, 9);
            this.tbJudgeRangeY.PasswordChar = false;
            this.tbJudgeRangeY.PlaceholderColor = System.Drawing.Color.DarkGray;
            this.tbJudgeRangeY.PlaceholderText = "";
            this.tbJudgeRangeY.Size = new System.Drawing.Size(89, 36);
            this.tbJudgeRangeY.TabIndex = 6;
            this.tbJudgeRangeY.Texts = "";
            this.tbJudgeRangeY.UnderlinedStyle = false;
            // 
            // tableLayoutPanel19
            // 
            this.tableLayoutPanel19.ColumnCount = 2;
            this.tableLayoutPanel19.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 164F));
            this.tableLayoutPanel19.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 222F));
            this.tableLayoutPanel19.Controls.Add(this.cbAiMonitoring, 0, 1);
            this.tableLayoutPanel19.Controls.Add(this.cbUseMNTTAN, 0, 0);
            this.tableLayoutPanel19.Controls.Add(this.cbUseSplit, 1, 0);
            this.tableLayoutPanel19.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel19.Location = new System.Drawing.Point(2, 620);
            this.tableLayoutPanel19.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel19.Name = "tableLayoutPanel19";
            this.tableLayoutPanel19.RowCount = 2;
            this.tableLayoutPanel19.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel19.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel19.Size = new System.Drawing.Size(338, 62);
            this.tableLayoutPanel19.TabIndex = 5;
            // 
            // cbAiMonitoring
            // 
            this.cbAiMonitoring.AutoSize = true;
            this.cbAiMonitoring.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cbAiMonitoring.Location = new System.Drawing.Point(3, 35);
            this.cbAiMonitoring.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.cbAiMonitoring.Name = "cbAiMonitoring";
            this.cbAiMonitoring.Size = new System.Drawing.Size(158, 23);
            this.cbAiMonitoring.TabIndex = 11;
            this.cbAiMonitoring.Text = "USE AI MONITORING";
            this.cbAiMonitoring.UseVisualStyleBackColor = true;
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
            // cbUseSplit
            // 
            this.cbUseSplit.AutoSize = true;
            this.cbUseSplit.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cbUseSplit.Location = new System.Drawing.Point(167, 4);
            this.cbUseSplit.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.cbUseSplit.Name = "cbUseSplit";
            this.cbUseSplit.Size = new System.Drawing.Size(216, 23);
            this.cbUseSplit.TabIndex = 10;
            this.cbUseSplit.Text = "USE SPLIT CTRNO";
            this.cbUseSplit.UseVisualStyleBackColor = true;
            // 
            // gbFilterType
            // 
            this.gbFilterType.Controls.Add(this.tableLayoutPanel4);
            this.gbFilterType.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gbFilterType.Location = new System.Drawing.Point(5, 6);
            this.gbFilterType.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.gbFilterType.Name = "gbFilterType";
            this.gbFilterType.Padding = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.gbFilterType.Size = new System.Drawing.Size(332, 260);
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
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 54F));
            this.tableLayoutPanel4.Size = new System.Drawing.Size(326, 236);
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
            this.lvFilterList.Size = new System.Drawing.Size(324, 120);
            this.lvFilterList.TabIndex = 5;
            this.lvFilterList.UseCompatibleStateImageBehavior = false;
            this.lvFilterList.View = System.Windows.Forms.View.List;
            // 
            // rbFilterType2
            // 
            this.rbFilterType2.AutoSize = true;
            this.rbFilterType2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rbFilterType2.Location = new System.Drawing.Point(3, 156);
            this.rbFilterType2.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.rbFilterType2.Name = "rbFilterType2";
            this.rbFilterType2.Size = new System.Drawing.Size(320, 22);
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
            this.tableLayoutPanel6.Size = new System.Drawing.Size(326, 30);
            this.tableLayoutPanel6.TabIndex = 1;
            // 
            // rbFilterType1
            // 
            this.rbFilterType1.AutoSize = true;
            this.rbFilterType1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rbFilterType1.Location = new System.Drawing.Point(3, 4);
            this.rbFilterType1.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.rbFilterType1.Name = "rbFilterType1";
            this.rbFilterType1.Size = new System.Drawing.Size(218, 22);
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
            this.tableLayoutPanel7.Controls.Add(this.cbbAiMonitorParam, 1, 1);
            this.tableLayoutPanel7.Controls.Add(this.cbDest, 0, 0);
            this.tableLayoutPanel7.Controls.Add(this.ckbES, 1, 0);
            this.tableLayoutPanel7.Controls.Add(this.ckbTG, 2, 0);
            this.tableLayoutPanel7.Controls.Add(this.ckbETC, 3, 0);
            this.tableLayoutPanel7.Controls.Add(this.lblDBAiMonitorName, 0, 1);
            this.tableLayoutPanel7.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel7.Location = new System.Drawing.Point(1, 183);
            this.tableLayoutPanel7.Margin = new System.Windows.Forms.Padding(1);
            this.tableLayoutPanel7.Name = "tableLayoutPanel7";
            this.tableLayoutPanel7.RowCount = 2;
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel7.Size = new System.Drawing.Size(324, 52);
            this.tableLayoutPanel7.TabIndex = 6;
            // 
            // cbbAiMonitorParam
            // 
            this.tableLayoutPanel7.SetColumnSpan(this.cbbAiMonitorParam, 3);
            this.cbbAiMonitorParam.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cbbAiMonitorParam.FormattingEnabled = true;
            this.cbbAiMonitorParam.Location = new System.Drawing.Point(130, 28);
            this.cbbAiMonitorParam.Margin = new System.Windows.Forms.Padding(1, 2, 1, 1);
            this.cbbAiMonitorParam.Name = "cbbAiMonitorParam";
            this.cbbAiMonitorParam.Size = new System.Drawing.Size(193, 23);
            this.cbbAiMonitorParam.TabIndex = 3;
            this.cbbAiMonitorParam.SelectedIndexChanged += new System.EventHandler(this.cbbAiMonitorParam_SelectedIndexChanged);
            // 
            // cbDest
            // 
            this.cbDest.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cbDest.FormattingEnabled = true;
            this.cbDest.Location = new System.Drawing.Point(1, 2);
            this.cbDest.Margin = new System.Windows.Forms.Padding(1, 2, 1, 1);
            this.cbDest.Name = "cbDest";
            this.cbDest.Size = new System.Drawing.Size(127, 23);
            this.cbDest.TabIndex = 0;
            this.cbDest.SelectedIndexChanged += new System.EventHandler(this.cbDest_SelectedIndexChanged);
            // 
            // ckbES
            // 
            this.ckbES.AutoSize = true;
            this.ckbES.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ckbES.Location = new System.Drawing.Point(132, 3);
            this.ckbES.Name = "ckbES";
            this.ckbES.Size = new System.Drawing.Size(58, 20);
            this.ckbES.TabIndex = 1;
            this.ckbES.Text = "ES";
            this.ckbES.UseVisualStyleBackColor = true;
            // 
            // ckbTG
            // 
            this.ckbTG.AutoSize = true;
            this.ckbTG.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ckbTG.Location = new System.Drawing.Point(196, 3);
            this.ckbTG.Name = "ckbTG";
            this.ckbTG.Size = new System.Drawing.Size(58, 20);
            this.ckbTG.TabIndex = 1;
            this.ckbTG.Text = "TG";
            this.ckbTG.UseVisualStyleBackColor = true;
            // 
            // ckbETC
            // 
            this.ckbETC.AutoSize = true;
            this.ckbETC.Dock = System.Windows.Forms.DockStyle.Fill;
            this.ckbETC.Location = new System.Drawing.Point(260, 3);
            this.ckbETC.Name = "ckbETC";
            this.ckbETC.Size = new System.Drawing.Size(61, 20);
            this.ckbETC.TabIndex = 1;
            this.ckbETC.Text = "ETC";
            this.ckbETC.UseVisualStyleBackColor = true;
            // 
            // lblDBAiMonitorName
            // 
            this.lblDBAiMonitorName.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.lblDBAiMonitorName.BackColor = System.Drawing.Color.Transparent;
            this.lblDBAiMonitorName.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.lblDBAiMonitorName.BorderRadius = 5;
            this.lblDBAiMonitorName.BorderSize = 1;
            this.lblDBAiMonitorName.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblDBAiMonitorName.FillColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.lblDBAiMonitorName.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblDBAiMonitorName.ImagePadding = 6;
            this.lblDBAiMonitorName.ImageSize = 18;
            this.lblDBAiMonitorName.LabelImage = null;
            this.lblDBAiMonitorName.Location = new System.Drawing.Point(1, 27);
            this.lblDBAiMonitorName.Margin = new System.Windows.Forms.Padding(1);
            this.lblDBAiMonitorName.Name = "lblDBAiMonitorName";
            this.lblDBAiMonitorName.ShowAccentLine = false;
            this.lblDBAiMonitorName.Size = new System.Drawing.Size(127, 24);
            this.lblDBAiMonitorName.TabIndex = 2;
            this.lblDBAiMonitorName.Text = "AI PARAM";
            this.lblDBAiMonitorName.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblDBAiMonitorName.TextColor = System.Drawing.Color.White;
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
            this.tableLayoutPanel5.Location = new System.Drawing.Point(5, 686);
            this.tableLayoutPanel5.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.tableLayoutPanel5.Name = "tableLayoutPanel5";
            this.tableLayoutPanel5.RowCount = 1;
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 33F));
            this.tableLayoutPanel5.Size = new System.Drawing.Size(332, 33);
            this.tableLayoutPanel5.TabIndex = 7;
            // 
            // tableLayoutPanel8
            // 
            this.tableLayoutPanel8.ColumnCount = 1;
            this.tableLayoutPanel8.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel8.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel8.Controls.Add(this.dgvLotList, 0, 0);
            this.tableLayoutPanel8.Controls.Add(this.tableLayoutPanel9, 0, 1);
            this.tableLayoutPanel8.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel8.Location = new System.Drawing.Point(346, 0);
            this.tableLayoutPanel8.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel8.Name = "tableLayoutPanel8";
            this.tableLayoutPanel8.RowCount = 2;
            this.tableLayoutPanel8.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel8.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 53F));
            this.tableLayoutPanel8.Size = new System.Drawing.Size(342, 729);
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
            this.dgvLotList.Size = new System.Drawing.Size(340, 674);
            this.dgvLotList.TabIndex = 6;
            this.dgvLotList.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dgvLotList_CellEndEdit);
            // 
            // tableLayoutPanel9
            // 
            this.tableLayoutPanel9.ColumnCount = 4;
            this.tableLayoutPanel9.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel9.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel9.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel9.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel9.Controls.Add(this.btnDel, 2, 0);
            this.tableLayoutPanel9.Controls.Add(this.btnClear, 3, 0);
            this.tableLayoutPanel9.Controls.Add(this.btnAdd, 1, 0);
            this.tableLayoutPanel9.Controls.Add(this.btnAddList, 0, 0);
            this.tableLayoutPanel9.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel9.Location = new System.Drawing.Point(3, 679);
            this.tableLayoutPanel9.Name = "tableLayoutPanel9";
            this.tableLayoutPanel9.RowCount = 1;
            this.tableLayoutPanel9.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel9.Size = new System.Drawing.Size(336, 47);
            this.tableLayoutPanel9.TabIndex = 3;
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
            this.btnLoadParam.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnLoadParam.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnLoadParam.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnLoadParam.IconPadding = 6;
            this.btnLoadParam.IconSize = 20;
            this.btnLoadParam.Location = new System.Drawing.Point(193, 1);
            this.btnLoadParam.Margin = new System.Windows.Forms.Padding(1);
            this.btnLoadParam.Name = "btnLoadParam";
            this.btnLoadParam.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnLoadParam.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnLoadParam.ShowAccentLine = false;
            this.btnLoadParam.Size = new System.Drawing.Size(68, 31);
            this.btnLoadParam.TabIndex = 17;
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
            this.btnSaveParam.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSaveParam.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnSaveParam.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnSaveParam.IconPadding = 6;
            this.btnSaveParam.IconSize = 20;
            this.btnSaveParam.Location = new System.Drawing.Point(263, 1);
            this.btnSaveParam.Margin = new System.Windows.Forms.Padding(1);
            this.btnSaveParam.Name = "btnSaveParam";
            this.btnSaveParam.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnSaveParam.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnSaveParam.ShowAccentLine = false;
            this.btnSaveParam.Size = new System.Drawing.Size(68, 31);
            this.btnSaveParam.TabIndex = 17;
            this.btnSaveParam.Text = "SAVE";
            this.btnSaveParam.TextColor = System.Drawing.Color.White;
            this.btnSaveParam.TextOffsetX = 0;
            this.btnSaveParam.TextOffsetY = 0;
            this.btnSaveParam.Click += new System.EventHandler(this.btnSaveParam_Click);
            // 
            // btnAddUserFilter
            // 
            this.btnAddUserFilter.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnAddUserFilter.BackColor = System.Drawing.Color.Transparent;
            this.btnAddUserFilter.BorderColor = System.Drawing.Color.SlateGray;
            this.btnAddUserFilter.BorderRadius = 5;
            this.btnAddUserFilter.BorderSize = 2;
            this.btnAddUserFilter.ButtonImage = null;
            this.btnAddUserFilter.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnAddUserFilter.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnAddUserFilter.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnAddUserFilter.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnAddUserFilter.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnAddUserFilter.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnAddUserFilter.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnAddUserFilter.IconPadding = 6;
            this.btnAddUserFilter.IconSize = 20;
            this.btnAddUserFilter.Location = new System.Drawing.Point(225, 1);
            this.btnAddUserFilter.Margin = new System.Windows.Forms.Padding(1);
            this.btnAddUserFilter.Name = "btnAddUserFilter";
            this.btnAddUserFilter.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAddUserFilter.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnAddUserFilter.ShowAccentLine = false;
            this.btnAddUserFilter.Size = new System.Drawing.Size(100, 28);
            this.btnAddUserFilter.TabIndex = 17;
            this.btnAddUserFilter.Text = "ADD";
            this.btnAddUserFilter.TextColor = System.Drawing.Color.White;
            this.btnAddUserFilter.TextOffsetX = 0;
            this.btnAddUserFilter.TextOffsetY = 0;
            this.btnAddUserFilter.Click += new System.EventHandler(this.btnAddUserFilter_Click);
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
            this.btnAddCompRange.Location = new System.Drawing.Point(209, 1);
            this.btnAddCompRange.Margin = new System.Windows.Forms.Padding(1);
            this.btnAddCompRange.Name = "btnAddCompRange";
            this.btnAddCompRange.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAddCompRange.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnAddCompRange.ShowAccentLine = false;
            this.btnAddCompRange.Size = new System.Drawing.Size(58, 30);
            this.btnAddCompRange.TabIndex = 17;
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
            this.btnDelCompRange.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnDelCompRange.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnDelCompRange.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnDelCompRange.IconPadding = 6;
            this.btnDelCompRange.IconSize = 20;
            this.btnDelCompRange.Location = new System.Drawing.Point(269, 1);
            this.btnDelCompRange.Margin = new System.Windows.Forms.Padding(1);
            this.btnDelCompRange.Name = "btnDelCompRange";
            this.btnDelCompRange.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnDelCompRange.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnDelCompRange.ShowAccentLine = false;
            this.btnDelCompRange.Size = new System.Drawing.Size(58, 28);
            this.btnDelCompRange.TabIndex = 17;
            this.btnDelCompRange.Text = "DEL";
            this.btnDelCompRange.TextColor = System.Drawing.Color.White;
            this.btnDelCompRange.TextOffsetX = 0;
            this.btnDelCompRange.TextOffsetY = 0;
            this.btnDelCompRange.Click += new System.EventHandler(this.btnDelCompRange_Click);
            // 
            // btnAddList
            // 
            this.btnAddList.AccentColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(130)))), ((int)(((byte)(255)))));
            this.btnAddList.BackColor = System.Drawing.Color.Transparent;
            this.btnAddList.BorderColor = System.Drawing.Color.SlateGray;
            this.btnAddList.BorderRadius = 5;
            this.btnAddList.BorderSize = 2;
            this.btnAddList.ButtonImage = null;
            this.btnAddList.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnAddList.DisabledColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.btnAddList.DisabledTextColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.btnAddList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnAddList.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnAddList.HoverColor = System.Drawing.Color.FromArgb(((int)(((byte)(35)))), ((int)(((byte)(50)))), ((int)(((byte)(70)))));
            this.btnAddList.IconAlign = CustomControls.ShadedButtonIconAlign.Left;
            this.btnAddList.IconPadding = 6;
            this.btnAddList.IconSize = 20;
            this.btnAddList.Location = new System.Drawing.Point(1, 1);
            this.btnAddList.Margin = new System.Windows.Forms.Padding(1);
            this.btnAddList.Name = "btnAddList";
            this.btnAddList.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAddList.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnAddList.ShowAccentLine = false;
            this.btnAddList.Size = new System.Drawing.Size(82, 45);
            this.btnAddList.TabIndex = 17;
            this.btnAddList.Text = "ADD LIST";
            this.btnAddList.TextColor = System.Drawing.Color.White;
            this.btnAddList.TextOffsetX = 0;
            this.btnAddList.TextOffsetY = 0;
            this.btnAddList.Click += new System.EventHandler(this.btnAddList_Click);
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
            this.btnAdd.Location = new System.Drawing.Point(85, 1);
            this.btnAdd.Margin = new System.Windows.Forms.Padding(1);
            this.btnAdd.Name = "btnAdd";
            this.btnAdd.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAdd.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnAdd.ShowAccentLine = false;
            this.btnAdd.Size = new System.Drawing.Size(82, 45);
            this.btnAdd.TabIndex = 17;
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
            this.btnDel.Location = new System.Drawing.Point(169, 1);
            this.btnDel.Margin = new System.Windows.Forms.Padding(1);
            this.btnDel.Name = "btnDel";
            this.btnDel.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnDel.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnDel.ShowAccentLine = false;
            this.btnDel.Size = new System.Drawing.Size(82, 45);
            this.btnDel.TabIndex = 17;
            this.btnDel.Text = "DEL";
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
            this.btnClear.Location = new System.Drawing.Point(253, 1);
            this.btnClear.Margin = new System.Windows.Forms.Padding(1);
            this.btnClear.Name = "btnClear";
            this.btnClear.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnClear.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnClear.ShowAccentLine = false;
            this.btnClear.Size = new System.Drawing.Size(82, 45);
            this.btnClear.TabIndex = 17;
            this.btnClear.Text = "CLEAR";
            this.btnClear.TextColor = System.Drawing.Color.White;
            this.btnClear.TextOffsetX = 0;
            this.btnClear.TextOffsetY = 0;
            this.btnClear.Click += new System.EventHandler(this.btnClear_Click);
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
            this.btnCancel.Location = new System.Drawing.Point(501, 1);
            this.btnCancel.Margin = new System.Windows.Forms.Padding(1);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnCancel.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnCancel.ShowAccentLine = false;
            this.btnCancel.Size = new System.Drawing.Size(98, 46);
            this.btnCancel.TabIndex = 17;
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
            this.btnOk.Location = new System.Drawing.Point(601, 1);
            this.btnOk.Margin = new System.Windows.Forms.Padding(1);
            this.btnOk.Name = "btnOk";
            this.btnOk.NormalColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnOk.PressedColor = System.Drawing.Color.FromArgb(((int)(((byte)(10)))), ((int)(((byte)(15)))), ((int)(((byte)(25)))));
            this.btnOk.ShowAccentLine = false;
            this.btnOk.Size = new System.Drawing.Size(92, 46);
            this.btnOk.TabIndex = 17;
            this.btnOk.Text = "OK";
            this.btnOk.TextColor = System.Drawing.Color.White;
            this.btnOk.TextOffsetX = 0;
            this.btnOk.TextOffsetY = 0;
            this.btnOk.Click += new System.EventHandler(this.btnOk_Click);
            // 
            // FormSelectedLot
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(694, 815);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "FormSelectedLot";
            this.Text = "FormCsv";
            this.VisibleChanged += new System.EventHandler(this.FormSelectedLot_VisibleChanged);
            this.tableLayoutPanel1.ResumeLayout(false);
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
        private System.Windows.Forms.GroupBox gbFilterType;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel5;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel4;
        private System.Windows.Forms.RadioButton rbFilterType2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel6;
        private System.Windows.Forms.RadioButton rbFilterType1;
        private System.Windows.Forms.ListView lvFilterList;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel7;
        private System.Windows.Forms.ComboBox cbDest;
        private System.Windows.Forms.CheckBox ckbES;
        private System.Windows.Forms.CheckBox ckbTG;
        private System.Windows.Forms.CheckBox ckbETC;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel8;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel9;
        private System.Windows.Forms.DataGridView dgvLotList;
        private System.Windows.Forms.CheckBox cbUseSplit;
        private System.Windows.Forms.CheckBox cbAiMonitoring;
        private CustomControls.ShadedLabel lblDBAiMonitorName;
        private System.Windows.Forms.ComboBox cbbAiMonitorParam;
        private CustomControls.ShadedLabel lblTitle;
        private CustomControls.ShadedLabel lblRangeY;
        private CustomControls.ShadedLabel lblRangeX;
        private CustomControls.ShadedButton btnAddCompRange;
        private CustomControls.ShadedButton btnAddUserFilter;
        private CustomControls.ShadedButton btnLoadParam;
        private CustomControls.ShadedButton btnSaveParam;
        private CustomControls.ShadedButton btnDelCompRange;
        private CustomControls.ShadedButton btnAdd;
        private CustomControls.ShadedButton btnAddList;
        private CustomControls.ShadedButton btnCancel;
        private CustomControls.ShadedButton btnOk;
        private CustomControls.ShadedButton btnDel;
        private CustomControls.ShadedButton btnClear;
    }
}