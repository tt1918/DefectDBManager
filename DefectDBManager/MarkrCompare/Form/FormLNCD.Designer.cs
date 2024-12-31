namespace MarkrCompare
{
    partial class FormLNCD
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
            this.tableLayoutPanel5 = new System.Windows.Forms.TableLayoutPanel();
            this.btnCancel = new CustomControls.RoundButton();
            this.btnOK = new CustomControls.RoundButton();
            this.btnApply = new CustomControls.RoundButton();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.lblTitle = new CustomControls.RoundLabel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel9 = new System.Windows.Forms.TableLayoutPanel();
            this.dgvMaterial = new System.Windows.Forms.DataGridView();
            this.tableLayoutPanel10 = new System.Windows.Forms.TableLayoutPanel();
            this.btnAddMaterial = new CustomControls.RoundButton();
            this.btnDelMaterial = new CustomControls.RoundButton();
            this.tableLayoutPanel17 = new System.Windows.Forms.TableLayoutPanel();
            this.tbLNCD = new CustomControls.RoundTextBox();
            this.lblLNCD = new CustomControls.RoundLabel();
            this.tableLayoutPanel3 = new System.Windows.Forms.TableLayoutPanel();
            this.lvLNCDList = new System.Windows.Forms.ListView();
            this.lblSelLNCD = new CustomControls.RoundLabel();
            this.tableLayoutPanel4 = new System.Windows.Forms.TableLayoutPanel();
            this.btnAdd = new CustomControls.RoundButton();
            this.btnDelete = new CustomControls.RoundButton();
            this.btnLoad = new CustomControls.RoundButton();
            this.btnSave = new CustomControls.RoundButton();
            this.tableLayoutPanel5.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.tableLayoutPanel9.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvMaterial)).BeginInit();
            this.tableLayoutPanel10.SuspendLayout();
            this.tableLayoutPanel17.SuspendLayout();
            this.tableLayoutPanel3.SuspendLayout();
            this.tableLayoutPanel4.SuspendLayout();
            this.SuspendLayout();
            // 
            // tableLayoutPanel5
            // 
            this.tableLayoutPanel5.ColumnCount = 5;
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel5.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 100F));
            this.tableLayoutPanel5.Controls.Add(this.btnCancel, 3, 0);
            this.tableLayoutPanel5.Controls.Add(this.btnOK, 4, 0);
            this.tableLayoutPanel5.Controls.Add(this.btnApply, 1, 0);
            this.tableLayoutPanel5.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel5.Location = new System.Drawing.Point(0, 511);
            this.tableLayoutPanel5.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel5.Name = "tableLayoutPanel5";
            this.tableLayoutPanel5.RowCount = 1;
            this.tableLayoutPanel5.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel5.Size = new System.Drawing.Size(591, 51);
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
            this.btnCancel.Location = new System.Drawing.Point(392, 1);
            this.btnCancel.Margin = new System.Windows.Forms.Padding(1);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(98, 49);
            this.btnCancel.TabIndex = 1;
            this.btnCancel.Text = "CANCEL";
            this.btnCancel.TextColor = System.Drawing.Color.White;
            this.btnCancel.UseVisualStyleBackColor = false;
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
            this.btnOK.Location = new System.Drawing.Point(492, 1);
            this.btnOK.Margin = new System.Windows.Forms.Padding(1);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(98, 49);
            this.btnOK.TabIndex = 1;
            this.btnOK.Text = "OK";
            this.btnOK.TextColor = System.Drawing.Color.White;
            this.btnOK.UseVisualStyleBackColor = false;
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
            this.btnApply.Location = new System.Drawing.Point(272, 1);
            this.btnApply.Margin = new System.Windows.Forms.Padding(1);
            this.btnApply.Name = "btnApply";
            this.btnApply.Size = new System.Drawing.Size(98, 49);
            this.btnApply.TabIndex = 1;
            this.btnApply.Text = "APPLY";
            this.btnApply.TextColor = System.Drawing.Color.White;
            this.btnApply.UseVisualStyleBackColor = false;
            this.btnApply.Click += new System.EventHandler(this.btnApply_Click);
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
            this.tableLayoutPanel1.Size = new System.Drawing.Size(591, 562);
            this.tableLayoutPanel1.TabIndex = 1;
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
            this.lblTitle.Size = new System.Drawing.Size(587, 26);
            this.lblTitle.TabIndex = 1;
            this.lblTitle.Thickness = 1;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 2;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 43.48562F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 56.51438F));
            this.tableLayoutPanel2.Controls.Add(this.tableLayoutPanel9, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.tableLayoutPanel3, 0, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(0, 30);
            this.tableLayoutPanel2.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 542F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(591, 481);
            this.tableLayoutPanel2.TabIndex = 2;
            // 
            // tableLayoutPanel9
            // 
            this.tableLayoutPanel9.ColumnCount = 1;
            this.tableLayoutPanel9.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel9.Controls.Add(this.dgvMaterial, 0, 1);
            this.tableLayoutPanel9.Controls.Add(this.tableLayoutPanel10, 0, 2);
            this.tableLayoutPanel9.Controls.Add(this.tableLayoutPanel17, 0, 0);
            this.tableLayoutPanel9.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel9.Location = new System.Drawing.Point(257, 0);
            this.tableLayoutPanel9.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel9.Name = "tableLayoutPanel9";
            this.tableLayoutPanel9.RowCount = 3;
            this.tableLayoutPanel9.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel9.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel9.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel9.Size = new System.Drawing.Size(334, 481);
            this.tableLayoutPanel9.TabIndex = 1;
            // 
            // dgvMaterial
            // 
            this.dgvMaterial.AllowUserToAddRows = false;
            this.dgvMaterial.AllowUserToDeleteRows = false;
            this.dgvMaterial.AllowUserToResizeColumns = false;
            this.dgvMaterial.AllowUserToResizeRows = false;
            this.dgvMaterial.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvMaterial.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgvMaterial.Location = new System.Drawing.Point(1, 36);
            this.dgvMaterial.Margin = new System.Windows.Forms.Padding(1);
            this.dgvMaterial.Name = "dgvMaterial";
            this.dgvMaterial.RowTemplate.Height = 23;
            this.dgvMaterial.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.FullRowSelect;
            this.dgvMaterial.Size = new System.Drawing.Size(332, 409);
            this.dgvMaterial.TabIndex = 5;
            // 
            // tableLayoutPanel10
            // 
            this.tableLayoutPanel10.ColumnCount = 4;
            this.tableLayoutPanel10.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel10.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel10.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel10.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 60F));
            this.tableLayoutPanel10.Controls.Add(this.btnAddMaterial, 2, 0);
            this.tableLayoutPanel10.Controls.Add(this.btnDelMaterial, 3, 0);
            this.tableLayoutPanel10.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel10.Location = new System.Drawing.Point(0, 446);
            this.tableLayoutPanel10.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel10.Name = "tableLayoutPanel10";
            this.tableLayoutPanel10.RowCount = 1;
            this.tableLayoutPanel10.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel10.Size = new System.Drawing.Size(334, 35);
            this.tableLayoutPanel10.TabIndex = 0;
            // 
            // btnAddMaterial
            // 
            this.btnAddMaterial.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAddMaterial.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnAddMaterial.BorderColor = System.Drawing.Color.Lavender;
            this.btnAddMaterial.BorderRadius = 5;
            this.btnAddMaterial.BorderSize = 2;
            this.btnAddMaterial.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnAddMaterial.FlatAppearance.BorderSize = 0;
            this.btnAddMaterial.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnAddMaterial.ForeColor = System.Drawing.Color.White;
            this.btnAddMaterial.Location = new System.Drawing.Point(215, 1);
            this.btnAddMaterial.Margin = new System.Windows.Forms.Padding(1);
            this.btnAddMaterial.Name = "btnAddMaterial";
            this.btnAddMaterial.Size = new System.Drawing.Size(58, 33);
            this.btnAddMaterial.TabIndex = 1;
            this.btnAddMaterial.Text = "ADD";
            this.btnAddMaterial.TextColor = System.Drawing.Color.White;
            this.btnAddMaterial.UseVisualStyleBackColor = false;
            this.btnAddMaterial.Click += new System.EventHandler(this.btnAddMaterial_Click);
            // 
            // btnDelMaterial
            // 
            this.btnDelMaterial.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnDelMaterial.BackgroundColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(80)))));
            this.btnDelMaterial.BorderColor = System.Drawing.Color.Lavender;
            this.btnDelMaterial.BorderRadius = 5;
            this.btnDelMaterial.BorderSize = 2;
            this.btnDelMaterial.Dock = System.Windows.Forms.DockStyle.Fill;
            this.btnDelMaterial.FlatAppearance.BorderSize = 0;
            this.btnDelMaterial.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnDelMaterial.ForeColor = System.Drawing.Color.White;
            this.btnDelMaterial.Location = new System.Drawing.Point(275, 1);
            this.btnDelMaterial.Margin = new System.Windows.Forms.Padding(1);
            this.btnDelMaterial.Name = "btnDelMaterial";
            this.btnDelMaterial.Size = new System.Drawing.Size(58, 33);
            this.btnDelMaterial.TabIndex = 1;
            this.btnDelMaterial.Text = "DEL";
            this.btnDelMaterial.TextColor = System.Drawing.Color.White;
            this.btnDelMaterial.UseVisualStyleBackColor = false;
            this.btnDelMaterial.Click += new System.EventHandler(this.btnDelMaterial_Click);
            // 
            // tableLayoutPanel17
            // 
            this.tableLayoutPanel17.ColumnCount = 2;
            this.tableLayoutPanel17.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 83F));
            this.tableLayoutPanel17.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel17.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel17.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel17.Controls.Add(this.tbLNCD, 0, 0);
            this.tableLayoutPanel17.Controls.Add(this.lblLNCD, 0, 0);
            this.tableLayoutPanel17.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel17.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel17.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel17.Name = "tableLayoutPanel17";
            this.tableLayoutPanel17.RowCount = 1;
            this.tableLayoutPanel17.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel17.Size = new System.Drawing.Size(334, 35);
            this.tableLayoutPanel17.TabIndex = 6;
            // 
            // tbLNCD
            // 
            this.tbLNCD.BackColor = System.Drawing.SystemColors.Window;
            this.tbLNCD.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(44)))), ((int)(((byte)(61)))), ((int)(((byte)(90)))));
            this.tbLNCD.BorderFocusColor = System.Drawing.Color.FromArgb(((int)(((byte)(44)))), ((int)(((byte)(61)))), ((int)(((byte)(90)))));
            this.tbLNCD.BorderRadius = 5;
            this.tbLNCD.BorderSize = 2;
            this.tbLNCD.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tbLNCD.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tbLNCD.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(64)))), ((int)(((byte)(64)))));
            this.tbLNCD.Location = new System.Drawing.Point(84, 1);
            this.tbLNCD.Margin = new System.Windows.Forms.Padding(1);
            this.tbLNCD.Multiline = false;
            this.tbLNCD.Name = "tbLNCD";
            this.tbLNCD.Padding = new System.Windows.Forms.Padding(10, 7, 10, 7);
            this.tbLNCD.PasswordChar = false;
            this.tbLNCD.PlaceholderColor = System.Drawing.Color.DarkGray;
            this.tbLNCD.PlaceholderText = "";
            this.tbLNCD.Size = new System.Drawing.Size(249, 32);
            this.tbLNCD.TabIndex = 8;
            this.tbLNCD.Texts = "";
            this.tbLNCD.UnderlinedStyle = false;
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
            this.lblLNCD.Location = new System.Drawing.Point(2, 2);
            this.lblLNCD.Margin = new System.Windows.Forms.Padding(2);
            this.lblLNCD.Name = "lblLNCD";
            this.lblLNCD.Size = new System.Drawing.Size(79, 31);
            this.lblLNCD.TabIndex = 6;
            this.lblLNCD.Text = "LNCD";
            this.lblLNCD.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            this.lblLNCD.Thickness = 1;
            // 
            // tableLayoutPanel3
            // 
            this.tableLayoutPanel3.ColumnCount = 1;
            this.tableLayoutPanel3.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.Controls.Add(this.lvLNCDList, 0, 1);
            this.tableLayoutPanel3.Controls.Add(this.lblSelLNCD, 0, 0);
            this.tableLayoutPanel3.Controls.Add(this.tableLayoutPanel4, 0, 2);
            this.tableLayoutPanel3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel3.Location = new System.Drawing.Point(0, 0);
            this.tableLayoutPanel3.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel3.Name = "tableLayoutPanel3";
            this.tableLayoutPanel3.RowCount = 3;
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 31F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel3.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel3.Size = new System.Drawing.Size(257, 481);
            this.tableLayoutPanel3.TabIndex = 0;
            // 
            // lvLNCDList
            // 
            this.lvLNCDList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lvLNCDList.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
            this.lvLNCDList.HideSelection = false;
            this.lvLNCDList.Location = new System.Drawing.Point(2, 33);
            this.lvLNCDList.Margin = new System.Windows.Forms.Padding(2);
            this.lvLNCDList.MultiSelect = false;
            this.lvLNCDList.Name = "lvLNCDList";
            this.lvLNCDList.Size = new System.Drawing.Size(253, 411);
            this.lvLNCDList.TabIndex = 3;
            this.lvLNCDList.UseCompatibleStateImageBehavior = false;
            this.lvLNCDList.View = System.Windows.Forms.View.List;
            this.lvLNCDList.DoubleClick += new System.EventHandler(this.lvLNCDList_DoubleClick);
            // 
            // lblSelLNCD
            // 
            this.lblSelLNCD.AutoSize = true;
            this.lblSelLNCD.BkColor = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(70)))));
            this.lblSelLNCD.BorderColor = System.Drawing.Color.Silver;
            this.lblSelLNCD.CornerR = 10;
            this.lblSelLNCD.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lblSelLNCD.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblSelLNCD.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.lblSelLNCD.IsFillLB = false;
            this.lblSelLNCD.IsFillLT = false;
            this.lblSelLNCD.IsFillRB = false;
            this.lblSelLNCD.IsFillRT = false;
            this.lblSelLNCD.Location = new System.Drawing.Point(2, 2);
            this.lblSelLNCD.Margin = new System.Windows.Forms.Padding(2);
            this.lblSelLNCD.Name = "lblSelLNCD";
            this.lblSelLNCD.Size = new System.Drawing.Size(253, 27);
            this.lblSelLNCD.TabIndex = 2;
            this.lblSelLNCD.Text = "AAA";
            this.lblSelLNCD.Thickness = 1;
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
            this.tableLayoutPanel4.Location = new System.Drawing.Point(0, 446);
            this.tableLayoutPanel4.Margin = new System.Windows.Forms.Padding(0);
            this.tableLayoutPanel4.Name = "tableLayoutPanel4";
            this.tableLayoutPanel4.RowCount = 1;
            this.tableLayoutPanel4.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel4.Size = new System.Drawing.Size(257, 35);
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
            this.btnAdd.Size = new System.Drawing.Size(62, 33);
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
            this.btnDelete.Location = new System.Drawing.Point(65, 1);
            this.btnDelete.Margin = new System.Windows.Forms.Padding(1);
            this.btnDelete.Name = "btnDelete";
            this.btnDelete.Size = new System.Drawing.Size(62, 33);
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
            this.btnLoad.Location = new System.Drawing.Point(129, 1);
            this.btnLoad.Margin = new System.Windows.Forms.Padding(1);
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.Size = new System.Drawing.Size(62, 33);
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
            this.btnSave.Location = new System.Drawing.Point(193, 1);
            this.btnSave.Margin = new System.Windows.Forms.Padding(1);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(63, 33);
            this.btnSave.TabIndex = 0;
            this.btnSave.Text = "SAVE";
            this.btnSave.TextColor = System.Drawing.Color.White;
            this.btnSave.UseVisualStyleBackColor = false;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // FormLNCD
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(591, 562);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "FormLNCD";
            this.Text = "FormLNCD";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormLNCD_FormClosing);
            this.Load += new System.EventHandler(this.FormLNCD_Load);
            this.tableLayoutPanel5.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.tableLayoutPanel2.ResumeLayout(false);
            this.tableLayoutPanel9.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dgvMaterial)).EndInit();
            this.tableLayoutPanel10.ResumeLayout(false);
            this.tableLayoutPanel17.ResumeLayout(false);
            this.tableLayoutPanel17.PerformLayout();
            this.tableLayoutPanel3.ResumeLayout(false);
            this.tableLayoutPanel3.PerformLayout();
            this.tableLayoutPanel4.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel5;
        private CustomControls.RoundButton btnCancel;
        private CustomControls.RoundButton btnOK;
        private CustomControls.RoundButton btnApply;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private CustomControls.RoundLabel lblTitle;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel3;
        private System.Windows.Forms.ListView lvLNCDList;
        private CustomControls.RoundLabel lblSelLNCD;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel4;
        private CustomControls.RoundButton btnAdd;
        private CustomControls.RoundButton btnDelete;
        private CustomControls.RoundButton btnLoad;
        private CustomControls.RoundButton btnSave;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel9;
        private System.Windows.Forms.DataGridView dgvMaterial;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel10;
        private CustomControls.RoundButton btnAddMaterial;
        private CustomControls.RoundButton btnDelMaterial;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel17;
        private CustomControls.RoundTextBox tbLNCD;
        private CustomControls.RoundLabel lblLNCD;
    }
}