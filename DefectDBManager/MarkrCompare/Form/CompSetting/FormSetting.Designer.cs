namespace MarkrCompare
{
    partial class FormSetting
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
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.lvSetList = new System.Windows.Forms.ListView();
            this.lblName = new CustomControls.RoundLabel();
            this.tableLayoutPanel4 = new System.Windows.Forms.TableLayoutPanel();
            this.btnAdd = new CustomControls.RoundButton();
            this.btnDelete = new CustomControls.RoundButton();
            this.btnLoad = new CustomControls.RoundButton();
            this.btnSave = new CustomControls.RoundButton();
            this.tableLayoutPanel6 = new System.Windows.Forms.TableLayoutPanel();
            this.gpReference = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel9 = new System.Windows.Forms.TableLayoutPanel();
            this.dgvRefProc = new System.Windows.Forms.DataGridView();
            this.tableLayoutPanel10 = new System.Windows.Forms.TableLayoutPanel();
            this.cbRefFltAll = new System.Windows.Forms.CheckBox();
            this.btnAddRefFlt = new CustomControls.RoundButton();
            this.btnDelRefFlt = new CustomControls.RoundButton();
            this.tableLayoutPanel17 = new System.Windows.Forms.TableLayoutPanel();
            this.lblLNCD = new CustomControls.RoundLabel();
            this.tbRefProcLineID = new CustomControls.RoundTextBox();
            this.lblLineID = new CustomControls.RoundLabel();
            this.tbRefLNCD = new CustomControls.RoundTextBox();
            this.groupProcess = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel7 = new System.Windows.Forms.TableLayoutPanel();
            this.dgvProcess = new System.Windows.Forms.DataGridView();
            this.tableLayoutPanel8 = new System.Windows.Forms.TableLayoutPanel();
            this.btnAddProc = new CustomControls.RoundButton();
            this.btnDelProc = new CustomControls.RoundButton();
            this.gpCompProc = new System.Windows.Forms.GroupBox();
            this.tableLayoutPanel13 = new System.Windows.Forms.TableLayoutPanel();
            this.dgvCompProc = new System.Windows.Forms.DataGridView();
            this.tableLayoutPanel14 = new System.Windows.Forms.TableLayoutPanel();
            this.cbCompFltAll = new System.Windows.Forms.CheckBox();
            this.btnAddCompFlt = new CustomControls.RoundButton();
            this.btnDelCompFlt = new CustomControls.RoundButton();
            this.tableLayoutPanel18 = new System.Windows.Forms.TableLayoutPanel();
            this.lblCompLineID = new CustomControls.RoundLabel();
            this.lblCompProcLineIDData = new CustomControls.RoundLabel();
            this.lblCompLNCD = new CustomControls.RoundLabel();
            this.lblCompLNCDData = new CustomControls.RoundLabel();
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
            this.tableLayoutPanel5 = new System.Windows.Forms.TableLayoutPanel();
            this.btnCancel = new CustomControls.RoundButton();
            this.btnOK = new CustomControls.RoundButton();
            this.btnApply = new CustomControls.RoundButton();
            this.tableLayoutPanel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.tableLayoutPanel3.SuspendLayout();
            this.tableLayoutPanel4.SuspendLayout();
            this.tableLayoutPanel6.SuspendLayout();
            this.gpReference.SuspendLayout();
            this.tableLayoutPanel9.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvRefProc)).BeginInit();
            this.tableLayoutPanel10.SuspendLayout();
            this.tableLayoutPanel17.SuspendLayout();
            this.groupProcess.SuspendLayout();
            this.tableLayoutPanel7.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvProcess)).BeginInit();
            this.tableLayoutPanel8.SuspendLayout();
            this.gpCompProc.SuspendLayout();
            this.tableLayoutPanel13.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvCompProc)).BeginInit();
            this.tableLayoutPanel14.SuspendLayout();
            this.tableLayoutPanel18.SuspendLayout();
            this.tableLayoutPanel15.SuspendLayout();
            this.groupCompRange.SuspendLayout();
            this.tableLayoutPanel11.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvCompRange)).BeginInit();
            this.tableLayoutPanel12.SuspendLayout();
            this.gpJudgeRange.SuspendLayout();
            this.tableLayoutPanel16.SuspendLayout();
            this.tableLayoutPanel19.SuspendLayout();
            this.tableLayoutPanel5.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this.lblTitle, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel5, 0, 2);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel1.Margin = new System.Windows.Forms.Padding(1);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 3;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 30F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 51F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(863, 623);
            this.tableLayoutPanel1.TabIndex = 0;
            // 
            // lblTitle
            // 
            this.lblTitle.AutoSize = true;
            this.lblTitle.BkColor = System.Drawing.Color.DarkSlateBlue;
            this.lblTitle.BorderColor = System.Drawing.Color.MidnightBlue;
            this.lblTitle.CornerR = 10;
            this.lblTitle.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblTitle.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblTitle.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.lblTitle.IsFillLB = false;
            this.lblTitle.IsFillLT = false;
            this.lblTitle.IsFillRB = false;
            this.lblTitle.IsFillRT = false;
            this.lblTitle.Location = new System.Drawing.Point(2, 2);
            this.lblTitle.Margin = new System.Windows.Forms.Padding(2);
            this.lblTitle.Name = "lblTitle";
            this.lblTitle.Size = new System.Drawing.Size(859, 26);
            this.lblTitle.TabIndex = 1;
            this.lblTitle.Text = "SETTING";
            this.lblTitle.Thickness = 1;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 2;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 26.99884F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 73.00116F));
            this.tableLayoutPanel2.Controls.Add(this.tableLayoutPanel3, 0, 0);
            this.tableLayoutPanel2.Controls.Add(this.tableLayoutPanel6, 1, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(0, 30);
            this.tableLayoutPanel2.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 542F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(863, 542);
            this.tableLayoutPanel2.TabIndex = 2;
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.ColumnCount = 1;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.Controls.Add(this.lvSetList, 0, 1);
            this.tableLayoutPanel3.Controls.Add(this.lblName, 0, 0);
            this.tableLayoutPanel3.Controls.Add(this.tableLayoutPanel4, 0, 2);
            this.tableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel3.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel3.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 3;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 31F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 38F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(233, 542);
            this.tableLayoutPanel3.TabIndex = 0;
            // 
            // lvSetList
            // 
            this.lvSetList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lvSetList.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.lvSetList.HideSelection = false;
            this.lvSetList.Location = new System.Drawing.Point(2, 33);
            this.lvSetList.Margin = new System.Windows.Forms.Padding(2);
            this.lvSetList.MultiSelect = false;
            this.lvSetList.Name = "lvSetList";
            this.lvSetList.Size = new System.Drawing.Size(229, 469);
            this.lvSetList.TabIndex = 3;
            this.lvSetList.UseCompatibleStateImageBehavior = false;
            this.lvSetList.View = System.Windows.Forms.View.List;
            this.lvSetList.DoubleClick += new System.EventHandler(this.lvSetList_DoubleClick);
            // 
            // lblName
            // 
            this.lblName.AutoSize = true;
            this.lblName.BkColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(70)))));
            this.lblName.BorderColor = System.Drawing.Color.Silver;
            this.lblName.CornerR = 10;
            this.lblName.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblName.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblName.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.lblName.IsFillLB = false;
            this.lblName.IsFillLT = false;
            this.lblName.IsFillRB = false;
            this.lblName.IsFillRT = false;
            this.lblName.Location = new System.Drawing.Point(2, 2);
            this.lblName.Margin = new System.Windows.Forms.Padding(2);
            this.lblName.Name = "lblName";
            this.lblName.Size = new System.Drawing.Size(229, 27);
            this.lblName.TabIndex = 2;
            this.lblName.Text = "AAA";
            this.lblName.Thickness = 1;
            // 
            // tableLayoutPanel4
            // 
            this.tableLayoutPanel4.ColumnCount = 4;
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel4.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            this.tableLayoutPanel4.Controls.Add(this.btnAdd, 0, 0);
            this.tableLayoutPanel4.Controls.Add(this.btnDelete, 1, 0);
            this.tableLayoutPanel4.Controls.Add(this.btnLoad, 2, 0);
            this.tableLayoutPanel4.Controls.Add(this.btnSave, 3, 0);
            this.tableLayoutPanel4.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel4.Location = new System.Drawing.Point(0, 504);
            this.tableLayoutPanel4.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel4.Name = "tableLayoutPanel4";
            this.tableLayoutPanel4.RowCount = 1;
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel4.Size = new System.Drawing.Size(233, 38);
            this.tableLayoutPanel4.TabIndex = 0;
            // 
            // btnAdd
            // 
            this.btnAdd.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAdd.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAdd.BorderColor = System.Drawing.Color.Lavender;
            this.btnAdd.BorderRadius = 5;
            this.btnAdd.BorderSize = 2;
            this.btnAdd.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnAdd.FlatAppearance.BorderSize = 0;
            this.btnAdd.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnAdd.ForeColor = System.Drawing.Color.White;
            this.btnAdd.Location = new System.Drawing.Point(1, 1);
            this.btnAdd.Margin = new System.Windows.Forms.Padding(1);
            this.btnAdd.Name = "btnAdd";
            this.btnAdd.Size = new System.Drawing.Size(56, 36);
            this.btnAdd.TabIndex = 0;
            this.btnAdd.Text = "ADD";
            this.btnAdd.TextColor = System.Drawing.Color.White;
            this.btnAdd.UseVisualStyleBackColor = false;
            this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
            // 
            // btnDelete
            // 
            this.btnDelete.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnDelete.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnDelete.BorderColor = System.Drawing.Color.Lavender;
            this.btnDelete.BorderRadius = 5;
            this.btnDelete.BorderSize = 2;
            this.btnDelete.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnDelete.FlatAppearance.BorderSize = 0;
            this.btnDelete.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnDelete.ForeColor = System.Drawing.Color.White;
            this.btnDelete.Location = new System.Drawing.Point(59, 1);
            this.btnDelete.Margin = new System.Windows.Forms.Padding(1);
            this.btnDelete.Name = "btnDelete";
            this.btnDelete.Size = new System.Drawing.Size(56, 36);
            this.btnDelete.TabIndex = 0;
            this.btnDelete.Text = "DELETE";
            this.btnDelete.TextColor = System.Drawing.Color.White;
            this.btnDelete.UseVisualStyleBackColor = false;
            this.btnDelete.Click += new System.EventHandler(this.btnDelete_Click);
            // 
            // btnLoad
            // 
            this.btnLoad.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnLoad.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnLoad.BorderColor = System.Drawing.Color.Lavender;
            this.btnLoad.BorderRadius = 5;
            this.btnLoad.BorderSize = 2;
            this.btnLoad.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnLoad.FlatAppearance.BorderSize = 0;
            this.btnLoad.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnLoad.ForeColor = System.Drawing.Color.White;
            this.btnLoad.Location = new System.Drawing.Point(117, 1);
            this.btnLoad.Margin = new System.Windows.Forms.Padding(1);
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.Size = new System.Drawing.Size(56, 36);
            this.btnLoad.TabIndex = 0;
            this.btnLoad.Text = "LOAD";
            this.btnLoad.TextColor = System.Drawing.Color.White;
            this.btnLoad.UseVisualStyleBackColor = false;
            this.btnLoad.Click += new System.EventHandler(this.btnLoad_Click);
            // 
            // btnSave
            // 
            this.btnSave.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnSave.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnSave.BorderColor = System.Drawing.Color.Lavender;
            this.btnSave.BorderRadius = 5;
            this.btnSave.BorderSize = 2;
            this.btnSave.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnSave.FlatAppearance.BorderSize = 0;
            this.btnSave.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnSave.ForeColor = System.Drawing.Color.White;
            this.btnSave.Location = new System.Drawing.Point(175, 1);
            this.btnSave.Margin = new System.Windows.Forms.Padding(1);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(57, 36);
            this.btnSave.TabIndex = 0;
            this.btnSave.Text = "SAVE";
            this.btnSave.TextColor = System.Drawing.Color.White;
            this.btnSave.UseVisualStyleBackColor = false;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // tableLayoutPanel6
            // 
            this.tableLayoutPanel6.ColumnCount = 2;
            this.tableLayoutPanel6.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 48.73524F));
            this.tableLayoutPanel6.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 51.26476F));
            this.tableLayoutPanel6.Controls.Add(this.gpReference, 1, 0);
            this.tableLayoutPanel6.Controls.Add(this.groupProcess, 0, 0);
            this.tableLayoutPanel6.Controls.Add(this.gpCompProc, 1, 1);
            this.tableLayoutPanel6.Controls.Add(this.tableLayoutPanel15, 0, 1);
            this.tableLayoutPanel6.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel6.Location = new System.Drawing.Point(233, 0);
            this.tableLayoutPanel6.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel6.Name = "tableLayoutPanel6";
            this.tableLayoutPanel6.Padding = new System.Windows.Forms.Padding(2);
            this.tableLayoutPanel6.RowCount = 2;
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 43.35793F));
            this.tableLayoutPanel6.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 56.64207F));
            this.tableLayoutPanel6.Size = new System.Drawing.Size(630, 542);
            this.tableLayoutPanel6.TabIndex = 1;
            // 
            // gpReference
            // 
            this.gpReference.Controls.Add(this.tableLayoutPanel9);
            this.gpReference.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gpReference.Location = new System.Drawing.Point(309, 4);
            this.gpReference.Margin = new System.Windows.Forms.Padding(2);
            this.gpReference.Name = "gpReference";
            this.gpReference.Padding = new System.Windows.Forms.Padding(1);
            this.gpReference.Size = new System.Drawing.Size(317, 229);
            this.gpReference.TabIndex = 3;
            this.gpReference.TabStop = false;
            this.gpReference.Text = "REFERENCE PROCESS";
            // 
            // tableLayoutPanel9
            // 
            this.tableLayoutPanel9.ColumnCount = 1;
            this.tableLayoutPanel9.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel9.Controls.Add(this.dgvRefProc, 0, 1);
            this.tableLayoutPanel9.Controls.Add(this.tableLayoutPanel10, 0, 2);
            this.tableLayoutPanel9.Controls.Add(this.tableLayoutPanel17, 0, 0);
            this.tableLayoutPanel9.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel9.Location = new System.Drawing.Point(1, 17);
            this.tableLayoutPanel9.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel9.Name = "tableLayoutPanel9";
            this.tableLayoutPanel9.RowCount = 3;
            this.tableLayoutPanel9.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel9.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel9.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel9.Size = new System.Drawing.Size(315, 211);
            this.tableLayoutPanel9.TabIndex = 1;
            // 
            // dgvRefProc
            // 
            this.dgvRefProc.AllowUserToAddRows = false;
            this.dgvRefProc.AllowUserToDeleteRows = false;
            this.dgvRefProc.AllowUserToResizeColumns = false;
            this.dgvRefProc.AllowUserToResizeRows = false;
            this.dgvRefProc.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvRefProc.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgvRefProc.Location = new System.Drawing.Point(1, 36);
            this.dgvRefProc.Margin = new System.Windows.Forms.Padding(1);
            this.dgvRefProc.Name = "dgvRefProc";
            this.dgvRefProc.RowTemplate.Height = 23;
            this.dgvRefProc.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dgvRefProc.Size = new System.Drawing.Size(313, 139);
            this.dgvRefProc.TabIndex = 5;
            // 
            // tableLayoutPanel10
            // 
            this.tableLayoutPanel10.ColumnCount = 4;
            this.tableLayoutPanel10.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel10.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel10.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel10.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel10.Controls.Add(this.cbRefFltAll, 0, 0);
            this.tableLayoutPanel10.Controls.Add(this.btnAddRefFlt, 2, 0);
            this.tableLayoutPanel10.Controls.Add(this.btnDelRefFlt, 3, 0);
            this.tableLayoutPanel10.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel10.Location = new System.Drawing.Point(0, 176);
            this.tableLayoutPanel10.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel10.Name = "tableLayoutPanel10";
            this.tableLayoutPanel10.RowCount = 1;
            this.tableLayoutPanel10.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel10.Size = new System.Drawing.Size(315, 35);
            this.tableLayoutPanel10.TabIndex = 0;
            // 
            // cbRefFltAll
            // 
            this.cbRefFltAll.AutoSize = true;
            this.cbRefFltAll.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cbRefFltAll.Location = new System.Drawing.Point(3, 3);
            this.cbRefFltAll.Name = "cbRefFltAll";
            this.cbRefFltAll.Size = new System.Drawing.Size(54, 29);
            this.cbRefFltAll.TabIndex = 10;
            this.cbRefFltAll.Text = "ALL";
            this.cbRefFltAll.UseVisualStyleBackColor = true;
            this.cbRefFltAll.CheckStateChanged += new System.EventHandler(this.cbRefFltAll_CheckStateChanged);
            // 
            // btnAddRefFlt
            // 
            this.btnAddRefFlt.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAddRefFlt.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAddRefFlt.BorderColor = System.Drawing.Color.Lavender;
            this.btnAddRefFlt.BorderRadius = 5;
            this.btnAddRefFlt.BorderSize = 2;
            this.btnAddRefFlt.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnAddRefFlt.FlatAppearance.BorderSize = 0;
            this.btnAddRefFlt.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnAddRefFlt.ForeColor = System.Drawing.Color.White;
            this.btnAddRefFlt.Location = new System.Drawing.Point(196, 1);
            this.btnAddRefFlt.Margin = new System.Windows.Forms.Padding(1);
            this.btnAddRefFlt.Name = "btnAddRefFlt";
            this.btnAddRefFlt.Size = new System.Drawing.Size(58, 33);
            this.btnAddRefFlt.TabIndex = 1;
            this.btnAddRefFlt.Text = "ADD";
            this.btnAddRefFlt.TextColor = System.Drawing.Color.White;
            this.btnAddRefFlt.UseVisualStyleBackColor = false;
            this.btnAddRefFlt.Click += new System.EventHandler(this.btnAddRefFlt_Click);
            // 
            // btnDelRefFlt
            // 
            this.btnDelRefFlt.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnDelRefFlt.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnDelRefFlt.BorderColor = System.Drawing.Color.Lavender;
            this.btnDelRefFlt.BorderRadius = 5;
            this.btnDelRefFlt.BorderSize = 2;
            this.btnDelRefFlt.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnDelRefFlt.FlatAppearance.BorderSize = 0;
            this.btnDelRefFlt.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnDelRefFlt.ForeColor = System.Drawing.Color.White;
            this.btnDelRefFlt.Location = new System.Drawing.Point(256, 1);
            this.btnDelRefFlt.Margin = new System.Windows.Forms.Padding(1);
            this.btnDelRefFlt.Name = "btnDelRefFlt";
            this.btnDelRefFlt.Size = new System.Drawing.Size(58, 33);
            this.btnDelRefFlt.TabIndex = 1;
            this.btnDelRefFlt.Text = "DEL";
            this.btnDelRefFlt.TextColor = System.Drawing.Color.White;
            this.btnDelRefFlt.UseVisualStyleBackColor = false;
            this.btnDelRefFlt.Click += new System.EventHandler(this.btnDelRefFlt_Click);
            // 
            // tableLayoutPanel17
            // 
            this.tableLayoutPanel17.ColumnCount = 4;
            this.tableLayoutPanel17.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 70F));
            this.tableLayoutPanel17.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel17.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 70F));
            this.tableLayoutPanel17.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel17.Controls.Add(this.lblLNCD, 0, 0);
            this.tableLayoutPanel17.Controls.Add(this.tbRefProcLineID, 0, 0);
            this.tableLayoutPanel17.Controls.Add(this.lblLineID, 0, 0);
            this.tableLayoutPanel17.Controls.Add(this.tbRefLNCD, 3, 0);
            this.tableLayoutPanel17.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel17.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel17.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel17.Name = "tableLayoutPanel17";
            this.tableLayoutPanel17.RowCount = 1;
            this.tableLayoutPanel17.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel17.Size = new System.Drawing.Size(315, 35);
            this.tableLayoutPanel17.TabIndex = 6;
            // 
            // lblLNCD
            // 
            this.lblLNCD.AutoSize = true;
            this.lblLNCD.BkColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(70)))));
            this.lblLNCD.BorderColor = System.Drawing.Color.Silver;
            this.lblLNCD.CornerR = 10;
            this.lblLNCD.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblLNCD.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblLNCD.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.lblLNCD.IsFillLB = false;
            this.lblLNCD.IsFillLT = false;
            this.lblLNCD.IsFillRB = false;
            this.lblLNCD.IsFillRT = false;
            this.lblLNCD.Location = new System.Drawing.Point(159, 2);
            this.lblLNCD.Margin = new System.Windows.Forms.Padding(2);
            this.lblLNCD.Name = "lblLNCD";
            this.lblLNCD.Size = new System.Drawing.Size(66, 31);
            this.lblLNCD.TabIndex = 9;
            this.lblLNCD.Text = "LNCD";
            this.lblLNCD.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblLNCD.Thickness = 1;
            // 
            // tbRefProcLineID
            // 
            this.tbRefProcLineID.BackColor = System.Drawing.SystemColors.Window;
            this.tbRefProcLineID.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(44)))), ((int)(((byte)(61)))), ((int)(((byte)(90)))));
            this.tbRefProcLineID.BorderFocusColor = System.Drawing.Color.FromArgb(((int)(((byte)(44)))), ((int)(((byte)(61)))), ((int)(((byte)(90)))));
            this.tbRefProcLineID.BorderRadius = 5;
            this.tbRefProcLineID.BorderSize = 2;
            this.tbRefProcLineID.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbRefProcLineID.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbRefProcLineID.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.tbRefProcLineID.Location = new System.Drawing.Point(71, 1);
            this.tbRefProcLineID.Margin = new System.Windows.Forms.Padding(1);
            this.tbRefProcLineID.Multiline = false;
            this.tbRefProcLineID.Name = "tbRefProcLineID";
            this.tbRefProcLineID.Padding = new System.Windows.Forms.Padding(10, 7, 10, 7);
            this.tbRefProcLineID.PasswordChar = false;
            this.tbRefProcLineID.PlaceholderColor = System.Drawing.Color.DarkGray;
            this.tbRefProcLineID.PlaceholderText = "";
            this.tbRefProcLineID.Size = new System.Drawing.Size(85, 32);
            this.tbRefProcLineID.TabIndex = 8;
            this.tbRefProcLineID.Texts = "";
            this.tbRefProcLineID.UnderlinedStyle = false;
            // 
            // lblLineID
            // 
            this.lblLineID.AutoSize = true;
            this.lblLineID.BkColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(70)))));
            this.lblLineID.BorderColor = System.Drawing.Color.Silver;
            this.lblLineID.CornerR = 10;
            this.lblLineID.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblLineID.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblLineID.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.lblLineID.IsFillLB = false;
            this.lblLineID.IsFillLT = false;
            this.lblLineID.IsFillRB = false;
            this.lblLineID.IsFillRT = false;
            this.lblLineID.Location = new System.Drawing.Point(2, 2);
            this.lblLineID.Margin = new System.Windows.Forms.Padding(2);
            this.lblLineID.Name = "lblLineID";
            this.lblLineID.Size = new System.Drawing.Size(66, 31);
            this.lblLineID.TabIndex = 6;
            this.lblLineID.Text = "ID";
            this.lblLineID.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblLineID.Thickness = 1;
            // 
            // tbRefLNCD
            // 
            this.tbRefLNCD.BackColor = System.Drawing.SystemColors.Window;
            this.tbRefLNCD.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(44)))), ((int)(((byte)(61)))), ((int)(((byte)(90)))));
            this.tbRefLNCD.BorderFocusColor = System.Drawing.Color.FromArgb(((int)(((byte)(44)))), ((int)(((byte)(61)))), ((int)(((byte)(90)))));
            this.tbRefLNCD.BorderRadius = 5;
            this.tbRefLNCD.BorderSize = 2;
            this.tbRefLNCD.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbRefLNCD.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbRefLNCD.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.tbRefLNCD.Location = new System.Drawing.Point(228, 1);
            this.tbRefLNCD.Margin = new System.Windows.Forms.Padding(1);
            this.tbRefLNCD.Multiline = false;
            this.tbRefLNCD.Name = "tbRefLNCD";
            this.tbRefLNCD.Padding = new System.Windows.Forms.Padding(10, 7, 10, 7);
            this.tbRefLNCD.PasswordChar = false;
            this.tbRefLNCD.PlaceholderColor = System.Drawing.Color.DarkGray;
            this.tbRefLNCD.PlaceholderText = "";
            this.tbRefLNCD.Size = new System.Drawing.Size(86, 32);
            this.tbRefLNCD.TabIndex = 8;
            this.tbRefLNCD.Texts = "";
            this.tbRefLNCD.UnderlinedStyle = false;
            // 
            // groupProcess
            // 
            this.groupProcess.Controls.Add(this.tableLayoutPanel7);
            this.groupProcess.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupProcess.Location = new System.Drawing.Point(2, 2);
            this.groupProcess.Margin = new System.Windows.Forms.Padding(0);
            this.groupProcess.Name = "groupProcess";
            this.groupProcess.Padding = new System.Windows.Forms.Padding(2);
            this.groupProcess.Size = new System.Drawing.Size(305, 233);
            this.groupProcess.TabIndex = 2;
            this.groupProcess.TabStop = false;
            this.groupProcess.Text = "PROCESS";
            // 
            // tableLayoutPanel7
            // 
            this.tableLayoutPanel7.ColumnCount = 1;
            this.tableLayoutPanel7.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel7.Controls.Add(this.dgvProcess, 0, 0);
            this.tableLayoutPanel7.Controls.Add(this.tableLayoutPanel8, 0, 1);
            this.tableLayoutPanel7.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel7.Location = new System.Drawing.Point(2, 18);
            this.tableLayoutPanel7.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel7.Name = "tableLayoutPanel7";
            this.tableLayoutPanel7.RowCount = 2;
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel7.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel7.Size = new System.Drawing.Size(301, 213);
            this.tableLayoutPanel7.TabIndex = 1;
            // 
            // dgvProcess
            // 
            this.dgvProcess.AllowUserToAddRows = false;
            this.dgvProcess.AllowUserToDeleteRows = false;
            this.dgvProcess.AllowUserToResizeColumns = false;
            this.dgvProcess.AllowUserToResizeRows = false;
            this.dgvProcess.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvProcess.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgvProcess.Location = new System.Drawing.Point(2, 2);
            this.dgvProcess.Margin = new System.Windows.Forms.Padding(2);
            this.dgvProcess.Name = "dgvProcess";
            this.dgvProcess.RowTemplate.Height = 23;
            this.dgvProcess.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dgvProcess.Size = new System.Drawing.Size(297, 174);
            this.dgvProcess.TabIndex = 5;
            this.dgvProcess.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dgvProcess_CellClick);
            this.dgvProcess.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dgvProcess_CellEndEdit);
            // 
            // tableLayoutPanel8
            // 
            this.tableLayoutPanel8.ColumnCount = 3;
            this.tableLayoutPanel8.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel8.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel8.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel8.Controls.Add(this.btnAddProc, 1, 0);
            this.tableLayoutPanel8.Controls.Add(this.btnDelProc, 2, 0);
            this.tableLayoutPanel8.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel8.Location = new System.Drawing.Point(0, 178);
            this.tableLayoutPanel8.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel8.Name = "tableLayoutPanel8";
            this.tableLayoutPanel8.RowCount = 1;
            this.tableLayoutPanel8.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel8.Size = new System.Drawing.Size(301, 35);
            this.tableLayoutPanel8.TabIndex = 0;
            // 
            // btnAddProc
            // 
            this.btnAddProc.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAddProc.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAddProc.BorderColor = System.Drawing.Color.Lavender;
            this.btnAddProc.BorderRadius = 5;
            this.btnAddProc.BorderSize = 2;
            this.btnAddProc.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnAddProc.FlatAppearance.BorderSize = 0;
            this.btnAddProc.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnAddProc.ForeColor = System.Drawing.Color.White;
            this.btnAddProc.Location = new System.Drawing.Point(182, 1);
            this.btnAddProc.Margin = new System.Windows.Forms.Padding(1);
            this.btnAddProc.Name = "btnAddProc";
            this.btnAddProc.Size = new System.Drawing.Size(58, 33);
            this.btnAddProc.TabIndex = 1;
            this.btnAddProc.Text = "ADD";
            this.btnAddProc.TextColor = System.Drawing.Color.White;
            this.btnAddProc.UseVisualStyleBackColor = false;
            this.btnAddProc.Click += new System.EventHandler(this.btnAddProc_Click);
            // 
            // btnDelProc
            // 
            this.btnDelProc.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnDelProc.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnDelProc.BorderColor = System.Drawing.Color.Lavender;
            this.btnDelProc.BorderRadius = 5;
            this.btnDelProc.BorderSize = 2;
            this.btnDelProc.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnDelProc.FlatAppearance.BorderSize = 0;
            this.btnDelProc.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnDelProc.ForeColor = System.Drawing.Color.White;
            this.btnDelProc.Location = new System.Drawing.Point(242, 1);
            this.btnDelProc.Margin = new System.Windows.Forms.Padding(1);
            this.btnDelProc.Name = "btnDelProc";
            this.btnDelProc.Size = new System.Drawing.Size(58, 33);
            this.btnDelProc.TabIndex = 1;
            this.btnDelProc.Text = "DEL";
            this.btnDelProc.TextColor = System.Drawing.Color.White;
            this.btnDelProc.UseVisualStyleBackColor = false;
            this.btnDelProc.Click += new System.EventHandler(this.btnDelProc_Click);
            // 
            // gpCompProc
            // 
            this.gpCompProc.Controls.Add(this.tableLayoutPanel13);
            this.gpCompProc.Dock = System.Windows.Forms.DockStyle.Fill;
            this.gpCompProc.Location = new System.Drawing.Point(309, 237);
            this.gpCompProc.Margin = new System.Windows.Forms.Padding(2);
            this.gpCompProc.Name = "gpCompProc";
            this.gpCompProc.Padding = new System.Windows.Forms.Padding(1);
            this.gpCompProc.Size = new System.Drawing.Size(317, 301);
            this.gpCompProc.TabIndex = 3;
            this.gpCompProc.TabStop = false;
            this.gpCompProc.Text = "COMPARE PROCESS";
            // 
            // tableLayoutPanel13
            // 
            this.tableLayoutPanel13.ColumnCount = 1;
            this.tableLayoutPanel13.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel13.Controls.Add(this.dgvCompProc, 0, 1);
            this.tableLayoutPanel13.Controls.Add(this.tableLayoutPanel14, 0, 2);
            this.tableLayoutPanel13.Controls.Add(this.tableLayoutPanel18, 0, 0);
            this.tableLayoutPanel13.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel13.Location = new System.Drawing.Point(1, 17);
            this.tableLayoutPanel13.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel13.Name = "tableLayoutPanel13";
            this.tableLayoutPanel13.RowCount = 3;
            this.tableLayoutPanel13.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel13.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel13.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel13.Size = new System.Drawing.Size(315, 283);
            this.tableLayoutPanel13.TabIndex = 1;
            // 
            // dgvCompProc
            // 
            this.dgvCompProc.AllowUserToAddRows = false;
            this.dgvCompProc.AllowUserToDeleteRows = false;
            this.dgvCompProc.AllowUserToResizeColumns = false;
            this.dgvCompProc.AllowUserToResizeRows = false;
            this.dgvCompProc.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvCompProc.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgvCompProc.Location = new System.Drawing.Point(1, 36);
            this.dgvCompProc.Margin = new System.Windows.Forms.Padding(1);
            this.dgvCompProc.Name = "dgvCompProc";
            this.dgvCompProc.RowTemplate.Height = 23;
            this.dgvCompProc.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dgvCompProc.Size = new System.Drawing.Size(313, 211);
            this.dgvCompProc.TabIndex = 5;
            // 
            // tableLayoutPanel14
            // 
            this.tableLayoutPanel14.ColumnCount = 6;
            this.tableLayoutPanel14.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel14.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel14.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel14.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 11F));
            this.tableLayoutPanel14.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel14.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel14.Controls.Add(this.cbCompFltAll, 0, 0);
            this.tableLayoutPanel14.Controls.Add(this.btnAddCompFlt, 4, 0);
            this.tableLayoutPanel14.Controls.Add(this.btnDelCompFlt, 5, 0);
            this.tableLayoutPanel14.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel14.Location = new System.Drawing.Point(0, 248);
            this.tableLayoutPanel14.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel14.Name = "tableLayoutPanel14";
            this.tableLayoutPanel14.RowCount = 1;
            this.tableLayoutPanel14.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel14.Size = new System.Drawing.Size(315, 35);
            this.tableLayoutPanel14.TabIndex = 0;
            // 
            // cbCompFltAll
            // 
            this.cbCompFltAll.AutoSize = true;
            this.cbCompFltAll.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cbCompFltAll.Location = new System.Drawing.Point(3, 3);
            this.cbCompFltAll.Name = "cbCompFltAll";
            this.cbCompFltAll.Size = new System.Drawing.Size(54, 29);
            this.cbCompFltAll.TabIndex = 9;
            this.cbCompFltAll.Text = "ALL";
            this.cbCompFltAll.UseVisualStyleBackColor = true;
            this.cbCompFltAll.CheckedChanged += new System.EventHandler(this.cbCompFltAll_CheckedChanged);
            // 
            // btnAddCompFlt
            // 
            this.btnAddCompFlt.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAddCompFlt.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAddCompFlt.BorderColor = System.Drawing.Color.Lavender;
            this.btnAddCompFlt.BorderRadius = 5;
            this.btnAddCompFlt.BorderSize = 2;
            this.btnAddCompFlt.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnAddCompFlt.FlatAppearance.BorderSize = 0;
            this.btnAddCompFlt.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnAddCompFlt.ForeColor = System.Drawing.Color.White;
            this.btnAddCompFlt.Location = new System.Drawing.Point(196, 1);
            this.btnAddCompFlt.Margin = new System.Windows.Forms.Padding(1);
            this.btnAddCompFlt.Name = "btnAddCompFlt";
            this.btnAddCompFlt.Size = new System.Drawing.Size(58, 33);
            this.btnAddCompFlt.TabIndex = 1;
            this.btnAddCompFlt.Text = "ADD";
            this.btnAddCompFlt.TextColor = System.Drawing.Color.White;
            this.btnAddCompFlt.UseVisualStyleBackColor = false;
            this.btnAddCompFlt.Click += new System.EventHandler(this.btnAddCompFlt_Click);
            // 
            // btnDelCompFlt
            // 
            this.btnDelCompFlt.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnDelCompFlt.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnDelCompFlt.BorderColor = System.Drawing.Color.Lavender;
            this.btnDelCompFlt.BorderRadius = 5;
            this.btnDelCompFlt.BorderSize = 2;
            this.btnDelCompFlt.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnDelCompFlt.FlatAppearance.BorderSize = 0;
            this.btnDelCompFlt.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnDelCompFlt.ForeColor = System.Drawing.Color.White;
            this.btnDelCompFlt.Location = new System.Drawing.Point(256, 1);
            this.btnDelCompFlt.Margin = new System.Windows.Forms.Padding(1);
            this.btnDelCompFlt.Name = "btnDelCompFlt";
            this.btnDelCompFlt.Size = new System.Drawing.Size(58, 33);
            this.btnDelCompFlt.TabIndex = 1;
            this.btnDelCompFlt.Text = "DEL";
            this.btnDelCompFlt.TextColor = System.Drawing.Color.White;
            this.btnDelCompFlt.UseVisualStyleBackColor = false;
            this.btnDelCompFlt.Click += new System.EventHandler(this.btnDelCompFlt_Click);
            // 
            // tableLayoutPanel18
            // 
            this.tableLayoutPanel18.ColumnCount = 4;
            this.tableLayoutPanel18.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 70F));
            this.tableLayoutPanel18.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel18.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 70F));
            this.tableLayoutPanel18.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel18.Controls.Add(this.lblCompLineID, 0, 0);
            this.tableLayoutPanel18.Controls.Add(this.lblCompProcLineIDData, 1, 0);
            this.tableLayoutPanel18.Controls.Add(this.lblCompLNCD, 2, 0);
            this.tableLayoutPanel18.Controls.Add(this.lblCompLNCDData, 3, 0);
            this.tableLayoutPanel18.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel18.Location = new System.Drawing.Point(1, 1);
            this.tableLayoutPanel18.Margin = new System.Windows.Forms.Padding(1);
            this.tableLayoutPanel18.Name = "tableLayoutPanel18";
            this.tableLayoutPanel18.RowCount = 1;
            this.tableLayoutPanel18.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel18.Size = new System.Drawing.Size(313, 33);
            this.tableLayoutPanel18.TabIndex = 6;
            // 
            // lblCompLineID
            // 
            this.lblCompLineID.AutoSize = true;
            this.lblCompLineID.BkColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(70)))));
            this.lblCompLineID.BorderColor = System.Drawing.Color.Silver;
            this.lblCompLineID.CornerR = 10;
            this.lblCompLineID.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblCompLineID.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblCompLineID.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.lblCompLineID.IsFillLB = false;
            this.lblCompLineID.IsFillLT = false;
            this.lblCompLineID.IsFillRB = false;
            this.lblCompLineID.IsFillRT = false;
            this.lblCompLineID.Location = new System.Drawing.Point(2, 2);
            this.lblCompLineID.Margin = new System.Windows.Forms.Padding(2);
            this.lblCompLineID.Name = "lblCompLineID";
            this.lblCompLineID.Size = new System.Drawing.Size(66, 29);
            this.lblCompLineID.TabIndex = 5;
            this.lblCompLineID.Text = "ID";
            this.lblCompLineID.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblCompLineID.Thickness = 1;
            // 
            // lblCompProcLineIDData
            // 
            this.lblCompProcLineIDData.AutoSize = true;
            this.lblCompProcLineIDData.BkColor = System.Drawing.Color.White;
            this.lblCompProcLineIDData.BorderColor = System.Drawing.Color.Silver;
            this.lblCompProcLineIDData.CornerR = 10;
            this.lblCompProcLineIDData.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblCompProcLineIDData.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblCompProcLineIDData.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(120)))));
            this.lblCompProcLineIDData.IsFillLB = false;
            this.lblCompProcLineIDData.IsFillLT = false;
            this.lblCompProcLineIDData.IsFillRB = false;
            this.lblCompProcLineIDData.IsFillRT = false;
            this.lblCompProcLineIDData.Location = new System.Drawing.Point(72, 2);
            this.lblCompProcLineIDData.Margin = new System.Windows.Forms.Padding(2);
            this.lblCompProcLineIDData.Name = "lblCompProcLineIDData";
            this.lblCompProcLineIDData.Size = new System.Drawing.Size(82, 29);
            this.lblCompProcLineIDData.TabIndex = 5;
            this.lblCompProcLineIDData.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblCompProcLineIDData.Thickness = 1;
            // 
            // lblCompLNCD
            // 
            this.lblCompLNCD.AutoSize = true;
            this.lblCompLNCD.BkColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(70)))));
            this.lblCompLNCD.BorderColor = System.Drawing.Color.Silver;
            this.lblCompLNCD.CornerR = 10;
            this.lblCompLNCD.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblCompLNCD.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblCompLNCD.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.lblCompLNCD.IsFillLB = false;
            this.lblCompLNCD.IsFillLT = false;
            this.lblCompLNCD.IsFillRB = false;
            this.lblCompLNCD.IsFillRT = false;
            this.lblCompLNCD.Location = new System.Drawing.Point(158, 2);
            this.lblCompLNCD.Margin = new System.Windows.Forms.Padding(2);
            this.lblCompLNCD.Name = "lblCompLNCD";
            this.lblCompLNCD.Size = new System.Drawing.Size(66, 29);
            this.lblCompLNCD.TabIndex = 5;
            this.lblCompLNCD.Text = "LNCD";
            this.lblCompLNCD.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblCompLNCD.Thickness = 1;
            // 
            // lblCompLNCDData
            // 
            this.lblCompLNCDData.AutoSize = true;
            this.lblCompLNCDData.BkColor = System.Drawing.Color.White;
            this.lblCompLNCDData.BorderColor = System.Drawing.Color.Silver;
            this.lblCompLNCDData.CornerR = 10;
            this.lblCompLNCDData.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblCompLNCDData.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblCompLNCDData.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(120)))));
            this.lblCompLNCDData.IsFillLB = false;
            this.lblCompLNCDData.IsFillLT = false;
            this.lblCompLNCDData.IsFillRB = false;
            this.lblCompLNCDData.IsFillRT = false;
            this.lblCompLNCDData.Location = new System.Drawing.Point(228, 2);
            this.lblCompLNCDData.Margin = new System.Windows.Forms.Padding(2);
            this.lblCompLNCDData.Name = "lblCompLNCDData";
            this.lblCompLNCDData.Size = new System.Drawing.Size(83, 29);
            this.lblCompLNCDData.TabIndex = 6;
            this.lblCompLNCDData.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblCompLNCDData.Thickness = 1;
            // 
            // tableLayoutPanel15
            // 
            this.tableLayoutPanel15.ColumnCount = 1;
            this.tableLayoutPanel15.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel15.Controls.Add(this.groupCompRange, 0, 0);
            this.tableLayoutPanel15.Controls.Add(this.gpJudgeRange, 0, 1);
            this.tableLayoutPanel15.Controls.Add(this.tableLayoutPanel19, 0, 2);
            this.tableLayoutPanel15.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel15.Location = new System.Drawing.Point(4, 237);
            this.tableLayoutPanel15.Margin = new System.Windows.Forms.Padding(2);
            this.tableLayoutPanel15.Name = "tableLayoutPanel15";
            this.tableLayoutPanel15.RowCount = 3;
            this.tableLayoutPanel15.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 79.10448F));
            this.tableLayoutPanel15.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 20.89552F));
            this.tableLayoutPanel15.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 29F));
            this.tableLayoutPanel15.Size = new System.Drawing.Size(301, 301);
            this.tableLayoutPanel15.TabIndex = 4;
            // 
            // groupCompRange
            // 
            this.groupCompRange.Controls.Add(this.tableLayoutPanel11);
            this.groupCompRange.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupCompRange.Location = new System.Drawing.Point(2, 2);
            this.groupCompRange.Margin = new System.Windows.Forms.Padding(2);
            this.groupCompRange.Name = "groupCompRange";
            this.groupCompRange.Padding = new System.Windows.Forms.Padding(1);
            this.groupCompRange.Size = new System.Drawing.Size(297, 211);
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
            this.tableLayoutPanel11.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel11.Name = "tableLayoutPanel11";
            this.tableLayoutPanel11.RowCount = 2;
            this.tableLayoutPanel11.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel11.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel11.Size = new System.Drawing.Size(295, 193);
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
            this.dgvCompRange.Location = new System.Drawing.Point(1, 1);
            this.dgvCompRange.Margin = new System.Windows.Forms.Padding(1);
            this.dgvCompRange.Name = "dgvCompRange";
            this.dgvCompRange.RowTemplate.Height = 23;
            this.dgvCompRange.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dgvCompRange.Size = new System.Drawing.Size(293, 156);
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
            this.tableLayoutPanel12.Location = new System.Drawing.Point(0, 158);
            this.tableLayoutPanel12.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel12.Name = "tableLayoutPanel12";
            this.tableLayoutPanel12.RowCount = 1;
            this.tableLayoutPanel12.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel12.Size = new System.Drawing.Size(295, 35);
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
            this.btnAddCompRange.Location = new System.Drawing.Point(176, 1);
            this.btnAddCompRange.Margin = new System.Windows.Forms.Padding(1);
            this.btnAddCompRange.Name = "btnAddCompRange";
            this.btnAddCompRange.Size = new System.Drawing.Size(58, 33);
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
            this.btnDelCompRange.Location = new System.Drawing.Point(236, 1);
            this.btnDelCompRange.Margin = new System.Windows.Forms.Padding(1);
            this.btnDelCompRange.Name = "btnDelCompRange";
            this.btnDelCompRange.Size = new System.Drawing.Size(58, 33);
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
            this.gpJudgeRange.Location = new System.Drawing.Point(2, 217);
            this.gpJudgeRange.Margin = new System.Windows.Forms.Padding(2);
            this.gpJudgeRange.Name = "gpJudgeRange";
            this.gpJudgeRange.Padding = new System.Windows.Forms.Padding(1);
            this.gpJudgeRange.Size = new System.Drawing.Size(297, 52);
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
            this.tableLayoutPanel16.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 36F));
            this.tableLayoutPanel16.Controls.Add(this.tbJudgeRangeX, 1, 0);
            this.tableLayoutPanel16.Controls.Add(this.lblRangeX, 0, 0);
            this.tableLayoutPanel16.Controls.Add(this.lblRangeY, 3, 0);
            this.tableLayoutPanel16.Controls.Add(this.label1, 2, 0);
            this.tableLayoutPanel16.Controls.Add(this.label2, 5, 0);
            this.tableLayoutPanel16.Controls.Add(this.tbJudgeRangeY, 4, 0);
            this.tableLayoutPanel16.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel16.Location = new System.Drawing.Point(1, 17);
            this.tableLayoutPanel16.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel16.Name = "tableLayoutPanel16";
            this.tableLayoutPanel16.RowCount = 1;
            this.tableLayoutPanel16.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel16.Size = new System.Drawing.Size(295, 34);
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
            this.tbJudgeRangeX.Location = new System.Drawing.Point(36, 1);
            this.tbJudgeRangeX.Margin = new System.Windows.Forms.Padding(1);
            this.tbJudgeRangeX.Multiline = false;
            this.tbJudgeRangeX.Name = "tbJudgeRangeX";
            this.tbJudgeRangeX.Padding = new System.Windows.Forms.Padding(10, 7, 10, 7);
            this.tbJudgeRangeX.PasswordChar = false;
            this.tbJudgeRangeX.PlaceholderColor = System.Drawing.Color.DarkGray;
            this.tbJudgeRangeX.PlaceholderText = "";
            this.tbJudgeRangeX.Size = new System.Drawing.Size(75, 32);
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
            this.lblRangeX.Location = new System.Drawing.Point(2, 2);
            this.lblRangeX.Margin = new System.Windows.Forms.Padding(2);
            this.lblRangeX.Name = "lblRangeX";
            this.lblRangeX.Size = new System.Drawing.Size(31, 30);
            this.lblRangeX.TabIndex = 3;
            this.lblRangeX.Text = "X";
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
            this.lblRangeY.Location = new System.Drawing.Point(149, 2);
            this.lblRangeY.Margin = new System.Windows.Forms.Padding(2);
            this.lblRangeY.Name = "lblRangeY";
            this.lblRangeY.Size = new System.Drawing.Size(31, 30);
            this.lblRangeY.TabIndex = 3;
            this.lblRangeY.Text = "Y";
            this.lblRangeY.Thickness = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.label1.ImageAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.label1.Location = new System.Drawing.Point(114, 19);
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
            this.label2.Location = new System.Drawing.Point(261, 19);
            this.label2.Margin = new System.Windows.Forms.Padding(2, 0, 0, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(34, 15);
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
            this.tbJudgeRangeY.Location = new System.Drawing.Point(183, 1);
            this.tbJudgeRangeY.Margin = new System.Windows.Forms.Padding(1);
            this.tbJudgeRangeY.Multiline = false;
            this.tbJudgeRangeY.Name = "tbJudgeRangeY";
            this.tbJudgeRangeY.Padding = new System.Windows.Forms.Padding(10, 7, 10, 7);
            this.tbJudgeRangeY.PasswordChar = false;
            this.tbJudgeRangeY.PlaceholderColor = System.Drawing.Color.DarkGray;
            this.tbJudgeRangeY.PlaceholderText = "";
            this.tbJudgeRangeY.Size = new System.Drawing.Size(75, 32);
            this.tbJudgeRangeY.TabIndex = 6;
            this.tbJudgeRangeY.Texts = "";
            this.tbJudgeRangeY.UnderlinedStyle = false;
            // 
            // tableLayoutPanel19
            // 
            this.tableLayoutPanel19.ColumnCount = 2;
            this.tableLayoutPanel19.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 164F));
            this.tableLayoutPanel19.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 139F));
            this.tableLayoutPanel19.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel19.Controls.Add(this.cbUseMNTTAN, 0, 0);
            this.tableLayoutPanel19.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel19.Location = new System.Drawing.Point(0, 271);
            this.tableLayoutPanel19.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel19.Name = "tableLayoutPanel19";
            this.tableLayoutPanel19.RowCount = 1;
            this.tableLayoutPanel19.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel19.Size = new System.Drawing.Size(301, 30);
            this.tableLayoutPanel19.TabIndex = 5;
            // 
            // cbUseMNTTAN
            // 
            this.cbUseMNTTAN.AutoSize = true;
            this.cbUseMNTTAN.Dock = System.Windows.Forms.DockStyle.Fill;
            this.cbUseMNTTAN.Location = new System.Drawing.Point(3, 3);
            this.cbUseMNTTAN.Name = "cbUseMNTTAN";
            this.cbUseMNTTAN.Size = new System.Drawing.Size(158, 24);
            this.cbUseMNTTAN.TabIndex = 10;
            this.cbUseMNTTAN.Text = "USE AI RESULT";
            this.cbUseMNTTAN.UseVisualStyleBackColor = true;
            // 
            // tableLayoutPanel5
            // 
            this.tableLayoutPanel5.ColumnCount = 5;
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 528F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            this.tableLayoutPanel5.Controls.Add(this.btnCancel, 3, 0);
            this.tableLayoutPanel5.Controls.Add(this.btnOK, 4, 0);
            this.tableLayoutPanel5.Controls.Add(this.btnApply, 1, 0);
            this.tableLayoutPanel5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel5.Location = new System.Drawing.Point(0, 572);
            this.tableLayoutPanel5.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel5.Name = "tableLayoutPanel5";
            this.tableLayoutPanel5.RowCount = 1;
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel5.Size = new System.Drawing.Size(863, 51);
            this.tableLayoutPanel5.TabIndex = 3;
            // 
            // btnCancel
            // 
            this.btnCancel.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnCancel.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnCancel.BorderColor = System.Drawing.Color.Lavender;
            this.btnCancel.BorderRadius = 5;
            this.btnCancel.BorderSize = 2;
            this.btnCancel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnCancel.FlatAppearance.BorderSize = 0;
            this.btnCancel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnCancel.ForeColor = System.Drawing.Color.White;
            this.btnCancel.Location = new System.Drawing.Point(654, 1);
            this.btnCancel.Margin = new System.Windows.Forms.Padding(1);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(103, 49);
            this.btnCancel.TabIndex = 1;
            this.btnCancel.Text = "CANCEL";
            this.btnCancel.TextColor = System.Drawing.Color.White;
            this.btnCancel.UseVisualStyleBackColor = false;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // btnOK
            // 
            this.btnOK.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnOK.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnOK.BorderColor = System.Drawing.Color.Lavender;
            this.btnOK.BorderRadius = 5;
            this.btnOK.BorderSize = 2;
            this.btnOK.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnOK.FlatAppearance.BorderSize = 0;
            this.btnOK.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnOK.ForeColor = System.Drawing.Color.White;
            this.btnOK.Location = new System.Drawing.Point(759, 1);
            this.btnOK.Margin = new System.Windows.Forms.Padding(1);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(103, 49);
            this.btnOK.TabIndex = 1;
            this.btnOK.Text = "OK";
            this.btnOK.TextColor = System.Drawing.Color.White;
            this.btnOK.UseVisualStyleBackColor = false;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // btnApply
            // 
            this.btnApply.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnApply.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnApply.BorderColor = System.Drawing.Color.Lavender;
            this.btnApply.BorderRadius = 5;
            this.btnApply.BorderSize = 2;
            this.btnApply.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnApply.FlatAppearance.BorderSize = 0;
            this.btnApply.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnApply.ForeColor = System.Drawing.Color.White;
            this.btnApply.Location = new System.Drawing.Point(529, 1);
            this.btnApply.Margin = new System.Windows.Forms.Padding(1);
            this.btnApply.Name = "btnApply";
            this.btnApply.Size = new System.Drawing.Size(103, 49);
            this.btnApply.TabIndex = 1;
            this.btnApply.Text = "APPLY";
            this.btnApply.TextColor = System.Drawing.Color.White;
            this.btnApply.UseVisualStyleBackColor = false;
            this.btnApply.Click += new System.EventHandler(this.btnApply_Click);
            // 
            // FormSetting
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.WhiteSmoke;
            this.ClientSize = new System.Drawing.Size(863, 623);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "FormSetting";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "FormSetting";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormSetting_FormClosing);
            this.Load += new System.EventHandler(this.FormSetting_Load);
            this.VisibleChanged += new System.EventHandler(this.FormSetting_VisibleChanged);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel3.ResumeLayout(false);
            this.tableLayoutPanel3.PerformLayout();
            this.tableLayoutPanel4.ResumeLayout(false);
            this.tableLayoutPanel6.ResumeLayout(false);
            this.gpReference.ResumeLayout(false);
            this.tableLayoutPanel9.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dgvRefProc)).EndInit();
            this.tableLayoutPanel10.ResumeLayout(false);
            this.tableLayoutPanel10.PerformLayout();
            this.tableLayoutPanel17.ResumeLayout(false);
            this.tableLayoutPanel17.PerformLayout();
            this.groupProcess.ResumeLayout(false);
            this.tableLayoutPanel7.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dgvProcess)).EndInit();
            this.tableLayoutPanel8.ResumeLayout(false);
            this.gpCompProc.ResumeLayout(false);
            this.tableLayoutPanel13.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dgvCompProc)).EndInit();
            this.tableLayoutPanel14.ResumeLayout(false);
            this.tableLayoutPanel14.PerformLayout();
            this.tableLayoutPanel18.ResumeLayout(false);
            this.tableLayoutPanel18.PerformLayout();
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
            this.tableLayoutPanel5.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        private void LvSetList_MouseDoubleClick(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            throw new System.NotImplementedException();
        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private CustomControls.RoundLabel lblTitle;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private CustomControls.RoundLabel lblName;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel4;
        private CustomControls.RoundButton btnDelete;
        private CustomControls.RoundButton btnLoad;
        private CustomControls.RoundButton btnSave;
        private System.Windows.Forms.ListView lvSetList;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel5;
        private CustomControls.RoundButton btnCancel;
        private CustomControls.RoundButton btnOK;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel6;
        private CustomControls.RoundButton btnAdd;
        private System.Windows.Forms.GroupBox gpReference;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel9;
        private System.Windows.Forms.DataGridView dgvRefProc;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel10;
        private CustomControls.RoundButton btnAddRefFlt;
        private CustomControls.RoundButton btnDelRefFlt;
        private System.Windows.Forms.GroupBox groupProcess;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel7;
        private System.Windows.Forms.DataGridView dgvProcess;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel8;
        private CustomControls.RoundButton btnAddProc;
        private CustomControls.RoundButton btnDelProc;
        private System.Windows.Forms.GroupBox groupCompRange;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel11;
        private System.Windows.Forms.DataGridView dgvCompRange;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel12;
        private CustomControls.RoundButton btnAddCompRange;
        private CustomControls.RoundButton btnDelCompRange;
        private System.Windows.Forms.GroupBox gpCompProc;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel13;
        private System.Windows.Forms.DataGridView dgvCompProc;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel14;
        private CustomControls.RoundButton btnAddCompFlt;
        private CustomControls.RoundButton btnDelCompFlt;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel15;
        private System.Windows.Forms.GroupBox gpJudgeRange;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel16;
        private CustomControls.RoundLabel lblRangeX;
        private CustomControls.RoundLabel lblRangeY;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private CustomControls.RoundTextBox tbJudgeRangeX;
        private CustomControls.RoundTextBox tbJudgeRangeY;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel17;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel18;
        private CustomControls.RoundTextBox tbRefProcLineID;
        private CustomControls.RoundLabel lblLineID;
        private CustomControls.RoundLabel lblCompLineID;
        private System.Windows.Forms.CheckBox cbCompFltAll;
        private System.Windows.Forms.CheckBox cbRefFltAll;
        private CustomControls.RoundButton btnApply;
        private CustomControls.RoundLabel lblCompProcLineIDData;
        private CustomControls.RoundLabel lblLNCD;
        private CustomControls.RoundTextBox tbRefLNCD;
        private CustomControls.RoundLabel lblCompLNCD;
        private CustomControls.RoundLabel lblCompLNCDData;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel19;
        private System.Windows.Forms.CheckBox cbUseMNTTAN;
    }
}